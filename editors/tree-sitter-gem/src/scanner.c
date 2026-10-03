// External scanner for the Gem tree-sitter grammar.
//
// Gem's lexer turns a newline (or `;`) into a NEWLINE token, and the parser
// only continues an expression with `(` (call), `[` (subscript) or a binary
// `-` when that token is on the same line. The grammar ignores whitespace,
// so these three postfix/infix tokens come from here: they are produced only
// when no newline or `;` separates them from the expression before them.
// Otherwise the scanner declines and the internal lexer reads a plain `(`,
// `[` or unary `-` that starts the next statement.
//
// After a call's `)`, a `{` on the same line opens a one-expression block
// (`f(x) { x + 1 }`) unless it reads as a table literal: `{}` or `{name:`.
// A `{ |` block comes from the internal lexer as one `{|` token.
//
// It also lexes:
//   - the `pcall` of the `pcall <expr>` form (an identifier `pcall` not
//     followed by `(` or `do`), and
//   - the text of a `"""` string between escapes, interpolations and the
//     closing `"""`, so an interpolation can hold a `"""` string of its own.

#include "tree_sitter/parser.h"

#include <stdbool.h>

enum TokenType {
  CALL_OPEN,
  SUBSCRIPT_OPEN,
  BINARY_MINUS,
  BLOCK_BRACE_OPEN,
  PCALL_PREFIX,
  TRIPLE_STRING_CONTENT,
  ERROR_SENTINEL,
};

void *tree_sitter_gem_external_scanner_create(void) { return NULL; }
void tree_sitter_gem_external_scanner_destroy(void *payload) { (void)payload; }
unsigned tree_sitter_gem_external_scanner_serialize(void *payload, char *buffer) {
  (void)payload;
  (void)buffer;
  return 0;
}
void tree_sitter_gem_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
  (void)payload;
  (void)buffer;
  (void)length;
}

static bool is_ident_char(int32_t c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

static void advance(TSLexer *lexer) { lexer->advance(lexer, false); }
// Skipped characters are whitespace before the token; use it only before
// any character of the token has been consumed.
static void skip(TSLexer *lexer) { lexer->advance(lexer, true); }

// Text of a `"""` string up to (not including) a `\`, a `{` or the
// closing `"""`. One or two quotes not followed by a third are text.
static bool scan_triple_content(TSLexer *lexer) {
  bool has = false;
  for (;;) {
    int32_t c = lexer->lookahead;
    if (lexer->eof(lexer) || c == '\\' || c == '{') break;
    if (c == '"') {
      lexer->mark_end(lexer);
      advance(lexer);
      if (lexer->lookahead == '"') {
        advance(lexer);
        if (lexer->lookahead == '"') {
          if (!has) return false;
          lexer->result_symbol = TRIPLE_STRING_CONTENT;
          return true;
        }
      }
      has = true;
      continue;
    }
    advance(lexer);
    has = true;
  }
  lexer->mark_end(lexer);
  if (!has) return false;
  lexer->result_symbol = TRIPLE_STRING_CONTENT;
  return true;
}

static bool scan_word(TSLexer *lexer, const char *word) {
  for (const char *p = word; *p; p++) {
    if (lexer->lookahead != *p) return false;
    advance(lexer);
  }
  return !is_ident_char(lexer->lookahead);
}

bool tree_sitter_gem_external_scanner_scan(void *payload, TSLexer *lexer, const bool *valid_symbols) {
  (void)payload;
  // In error recovery every symbol is valid; let the internal lexer work.
  if (valid_symbols[ERROR_SENTINEL]) return false;

  if (valid_symbols[TRIPLE_STRING_CONTENT]) return scan_triple_content(lexer);

  bool want_postfix = valid_symbols[CALL_OPEN] || valid_symbols[SUBSCRIPT_OPEN] ||
                      valid_symbols[BINARY_MINUS] || valid_symbols[BLOCK_BRACE_OPEN];
  if (!want_postfix && !valid_symbols[PCALL_PREFIX]) return false;

  bool separated = false;
  for (;;) {
    int32_t c = lexer->lookahead;
    if (c == ' ' || c == '\t' || c == '\r' || c == '\f') {
      skip(lexer);
    } else if (c == '\n' || c == ';') {
      separated = true;
      skip(lexer);
    } else {
      break;
    }
  }

  if (!separated) {
    int32_t c = lexer->lookahead;
    if (c == '(' && valid_symbols[CALL_OPEN]) {
      advance(lexer);
      lexer->mark_end(lexer);
      lexer->result_symbol = CALL_OPEN;
      return true;
    }
    if (c == '[' && valid_symbols[SUBSCRIPT_OPEN]) {
      advance(lexer);
      lexer->mark_end(lexer);
      lexer->result_symbol = SUBSCRIPT_OPEN;
      return true;
    }
    if (c == '-' && valid_symbols[BINARY_MINUS]) {
      advance(lexer);
      if (lexer->lookahead == '=') return false;  // `-=`
      lexer->mark_end(lexer);
      lexer->result_symbol = BINARY_MINUS;
      return true;
    }
    if (c == '{' && valid_symbols[BLOCK_BRACE_OPEN]) {
      advance(lexer);
      lexer->mark_end(lexer);
      while (lexer->lookahead == ' ' || lexer->lookahead == '\t') advance(lexer);
      if (lexer->lookahead == '|' || lexer->lookahead == '}') return false;
      if (is_ident_char(lexer->lookahead) && !(lexer->lookahead >= '0' && lexer->lookahead <= '9')) {
        while (is_ident_char(lexer->lookahead)) advance(lexer);
        if (lexer->lookahead == ':') return false;
      }
      lexer->result_symbol = BLOCK_BRACE_OPEN;
      return true;
    }
  }

  // `pcall <expr>`: `pcall` followed, on the same line, by anything but
  // `(` or `do` (those make it an ordinary call).
  if (valid_symbols[PCALL_PREFIX] && lexer->lookahead == 'p') {
    if (!scan_word(lexer, "pcall")) return false;
    lexer->mark_end(lexer);
    while (lexer->lookahead == ' ' || lexer->lookahead == '\t') advance(lexer);
    int32_t c = lexer->lookahead;
    if (lexer->eof(lexer) || c == '(' || c == '\n' || c == '\r' || c == ';' || c == '#') return false;
    if (c == 'd') {
      advance(lexer);
      if (lexer->lookahead == 'o') {
        advance(lexer);
        if (!is_ident_char(lexer->lookahead)) return false;
      }
    }
    lexer->result_symbol = PCALL_PREFIX;
    return true;
  }

  return false;
}
