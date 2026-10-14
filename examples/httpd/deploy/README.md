# Deploying httpd

What `PLAN.md` ("Deployment", "Success criteria") asks of the host, as
files: a systemd unit, an nftables fragment and a health query. Written
for the honeypot's VPS (`examples/honeypot/deploy/README.md`): Debian or
Ubuntu, 2 vCPUs, 4 GB, with the honeypot's firewall already installed.

| File | Install as | What it does |
|---|---|---|
| `httpd.service` | `/etc/systemd/system/httpd.service` | runs the server unprivileged (`DynamicUser`) in the honeypot's sandbox, restarts it on any exit, caps its memory at 512 MB and its CPU at one vCPU, at a lower priority than the honeypot |
| `httpd.env` | `/etc/httpd.env` | the build's commit and the server's flags |
| `httpd.nft` | `/etc/nftables.d/httpd.nft` | port 80 redirected to 8080, with a counter of new connections; 8080 itself stays closed |
| `health.sql` | | the metrics by day, against the success criteria |

The honeypot's ruleset (`examples/honeypot/deploy/honeypot.nft`) ends
with `include "/etc/nftables.d/*.nft"`, which this fragment needs. Its
egress deny and counter cover every `DynamicUser` UID, this server's
too.

## 1. Build

```sh
cd /opt/gem && git pull && make build
build/gem examples/httpd/main.gem -o /usr/local/bin/httpd
mkdir -p /usr/local/share/httpd && cp -R examples/httpd/static /usr/local/share/httpd/
```

## 2. Install, with port 80 still closed

```sh
cd /opt/gem/examples/httpd/deploy
mkdir -p /etc/nftables.d && cp httpd.nft /etc/nftables.d/
sed "s/^HTTPD_COMMIT=.*/HTTPD_COMMIT=$(git -C /opt/gem rev-parse --short HEAD)/" httpd.env > /etc/httpd.env
cp httpd.service /etc/systemd/system/
systemctl daemon-reload
systemctl enable --now httpd
```

The fragment takes effect at the next nftables reload or boot (step 3),
so the server runs on 8080 with nothing let in yet. Check it there:

```sh
curl -i http://127.0.0.1:8080/                          # the page
curl http://127.0.0.1:8080/stats                        # the counts, with gem_commit
```

After a minute, the health query (below) shows a row with `res_extra`
2 (the listener and the database) and `fds_extra` about 9.

## 3. Open port 80

The installed `/etc/nftables.conf` needs the `include`; one without it
is replaced by the repository's:

```sh
grep -q 'include "/etc/nftables.d' /etc/nftables.conf \
  || cp /opt/gem/examples/honeypot/deploy/honeypot.nft /etc/nftables.conf
nft -c -f /etc/nftables.conf            # check it before it can lock you out
systemctl reload nftables               # reloads /etc/nftables.conf, and so the include
```

The reload drops any rule added to the live ruleset by hand: the
ruleset begins with `flush ruleset`. The provider's firewall (Hetzner's, set in
its console) needs TCP 80 in.

Check it from another machine: `curl -i http://VPS_IP/` gets the page,
`curl http://VPS_IP/stats` the counts, and `curl -m 5 http://VPS_IP:8080/`
times out. On the VPS:

```sh
systemctl status httpd
journalctl -u httpd -f                                  # stderr: std/http's lines, restarts
nft list counter inet honeypot httpd_in                 # new connections to port 80
nft list counter inet honeypot honeypot_egress          # must stay at 0 packets
```

## Upgrading

Rebuild the binary and `HTTPD_COMMIT` together, then restart:

```sh
cd /opt/gem && git pull && make build
build/gem examples/httpd/main.gem -o /usr/local/bin/httpd.new
mv /usr/local/bin/httpd.new /usr/local/bin/httpd
cp -R examples/httpd/static /usr/local/share/httpd/
sed -i "s/^HTTPD_COMMIT=.*/HTTPD_COMMIT=$(git rev-parse --short HEAD)/" /etc/httpd.env
systemctl restart httpd
```

A restart of ours shows in the metrics like a crash would (`uptime_ms`
drops), and the connections open then end as `lost` when it starts
again: note the time, so the health query's restarts can be told apart.

## Reading the data

The data is in `/var/lib/httpd` (a link to `/var/lib/private/httpd`,
which only root can open), one file per week:

```sh
sqlite3 -box /var/lib/httpd/httpd-2026-10-12.db < /opt/gem/examples/httpd/deploy/health.sql
sqlite3 /var/lib/httpd/httpd-2026-10-12.db \
  "SELECT status, count(*) FROM requests GROUP BY 1 ORDER BY 2 DESC"
```

The schema is `SCHEMA` in `examples/httpd/recorder.gem`. Every stored path, header and
byte is attacker-controlled. A crash is replayed from its stored bytes:

```sh
sqlite3 /var/lib/httpd/httpd-2026-10-12.db "SELECT writefile('crash.bin', bytes) FROM crashes LIMIT 1"
nc 127.0.0.1 8080 < crash.bin                           # against a local build
```

## The sandbox

The honeypot's (its deploy README, "The sandbox"). Checked against
`strace -f` of the server under 30 s of the soak's load (all its kinds
of client but the bursts): 49 distinct system calls, all in
`@system-service` but `mincore`, which the unit adds, and no writable
executable mapping. It reads its static files from
`/usr/local/share/httpd`, which `ProtectSystem=strict` leaves readable.

`--max-connections 1000` and `MemoryMax=512M` go together. The soak's
load peaks at 68 MB of RSS (its hour on a Linux x86_64 VM,
`benchmarks/soak/results/2026-10-13_linux-vm_httpd`, with 999
connections at the peak of its bursts). The worst a client can do
is pipeline requests and never read the answers: 900 connections that
each sent 5,000 pipelined GETs took the server to 195 MB (about 210 KB
a connection), and 900 that stalled partway through a 64 KB body or
header to under 180 MB, so 1,000 of the worst take about 220 MB. The
health query's `rss_max_mb` shows each day's peak.

`CPUQuota=100%` and `Nice=5` keep one vCPU for the honeypot whatever
arrives on port 80.
