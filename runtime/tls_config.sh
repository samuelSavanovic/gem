#!/bin/sh
# Finds OpenSSL 3 or later for the runtime's TLS support (runtime/gem_builtins_tls.c)
# and writes two files into the build directory given as $1:
#
#   tls_cflags  flags that compile gem_builtins_tls.c against OpenSSL
#               (-DGEM_TLS and the include path)
#   tls_libs    flags that link a program calling tls_connect (the compiler
#               adds them to its cc command)
#
# Both are empty when no OpenSSL 3 or later compiles and links here; tls_connect
# then raises. A file is rewritten only when its contents change, so make
# rebuilds the TLS object only then, and the result is printed only then.
#
# The first candidate that compiles and links a probe wins:
#   1. OPENSSL_CFLAGS / OPENSSL_LIBS from the environment, when either is set
#      (OPENSSL_LIBS defaults to -lssl -lcrypto);
#   2. Homebrew's openssl@3;
#   3. pkg-config's openssl;
#   4. the compiler's default paths with -lssl -lcrypto.
# GEM_TLS=0 builds without TLS. CC is the C compiler (default cc).

set -u
out=${1:?usage: tls_config.sh <build dir>}
cc=${CC:-cc}
tmp=$(mktemp -d "${TMPDIR:-/tmp}/gem_tls.XXXXXX") || exit 1
trap 'rm -rf "$tmp"' EXIT

cat > "$tmp/probe.c" <<'EOF'
#include <openssl/ssl.h>
#if !defined(OPENSSL_VERSION_NUMBER) || OPENSSL_VERSION_NUMBER < 0x30000000L || defined(LIBRESSL_VERSION_NUMBER)
#error "OpenSSL 3 or later is required"
#endif
int main(void) {
    SSL_CTX *c = SSL_CTX_new(TLS_client_method());
    SSL_CTX_free(c);
    return 0;
}
EOF

found_cflags=""
found_libs=""
found_from=""

# try <where> <cflags> <libs>
try() {
    [ -n "$found_from" ] && return 0
    # Word splitting of the flags is intended.
    # shellcheck disable=SC2086
    if $cc -o "$tmp/probe" "$tmp/probe.c" $2 $3 >/dev/null 2>&1; then
        found_cflags=$2
        found_libs=$3
        found_from=$1
    fi
}

if [ "${GEM_TLS:-1}" != "0" ]; then
    if [ -n "${OPENSSL_CFLAGS:-}" ] || [ -n "${OPENSSL_LIBS:-}" ]; then
        try "OPENSSL_CFLAGS/OPENSSL_LIBS" "${OPENSSL_CFLAGS:-}" "${OPENSSL_LIBS:--lssl -lcrypto}"
    fi
    # Homebrew's opt/ path, not the versioned Cellar one pkg-config gives,
    # so the flags survive a `brew upgrade`.
    if command -v brew >/dev/null 2>&1; then
        brew_ssl=$(brew --prefix openssl@3 2>/dev/null)
        if [ -n "$brew_ssl" ] && [ -d "$brew_ssl/include/openssl" ]; then
            try "Homebrew openssl@3" "-I$brew_ssl/include" "-L$brew_ssl/lib -lssl -lcrypto"
        fi
    fi
    if command -v pkg-config >/dev/null 2>&1 && pkg-config --exists openssl 2>/dev/null; then
        try "pkg-config" "$(pkg-config --cflags openssl)" "$(pkg-config --libs openssl)"
    fi
    try "default paths" "" "-lssl -lcrypto"
fi

if [ -n "$found_from" ]; then
    cflags="-DGEM_TLS $found_cflags"
    libs=$found_libs
    msg="TLS: OpenSSL found ($found_from)"
else
    cflags=""
    libs=""
    if [ "${GEM_TLS:-1}" = "0" ]; then
        msg="TLS: off (GEM_TLS=0); tls_connect will raise"
    else
        msg="TLS: no OpenSSL 3 or later found; tls_connect will raise. Install libssl-dev (Debian/Ubuntu), openssl-devel (Fedora) or openssl@3 (Homebrew), or set OPENSSL_CFLAGS/OPENSSL_LIBS, then run make build again"
    fi
fi

changed=0
# write_if_changed <file> <contents>
write_if_changed() {
    printf '%s\n' "$2" > "$tmp/new"
    if ! cmp -s "$tmp/new" "$1"; then
        cp "$tmp/new" "$1"
        changed=1
    fi
}

write_if_changed "$out/tls_cflags" "$cflags"
write_if_changed "$out/tls_libs" "$libs"
[ "$changed" = 1 ] && echo "$msg"
exit 0
