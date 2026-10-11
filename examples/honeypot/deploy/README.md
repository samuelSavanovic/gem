# Deploying the honeypot

What `PLAN.md` ("Deployment", "Success criteria") asks of the host,
as files: a systemd unit, an nftables ruleset and a health query. Written
for Debian or Ubuntu on a dedicated 4 GB KVM VPS (a Hetzner CX23).

| File | Install as | What it does |
|---|---|---|
| `honeypot.service` | `/etc/systemd/system/honeypot.service` | runs the honeypot unprivileged (`DynamicUser`) in a sandbox, restarts it on any exit, caps its memory at 2 GB |
| `honeypot.env` | `/etc/honeypot.env` | the build's commit and the honeypot's flags |
| `honeypot.nft` | `/etc/nftables.conf` | port 23 redirected to 2323, SSH in, everything else in dropped; outbound denied but for DNS, NTP and package updates, and the honeypot's user denied even those, with a counter |
| `health.sql` | | the metrics by day, against the success criteria |

## 1. Build

```sh
apt install build-essential git nftables sqlite3 python3
git clone https://github.com/samuelSavanovic/gem /opt/gem && cd /opt/gem
make build
build/gem examples/honeypot/main.gem -o /usr/local/bin/honeypot
```

## 2. Soak on the VPS, before opening port 23

The soak results so far (`benchmarks/README.md`, "Recorded runs") came
from a 15 GB VM with the session cap out of the way. This run uses the
deploy's cap and memory limit on the real machine, for 8 hours:

```sh
TARGETS=honeypot DURATION=8h GUARD_RSS_MB=2048 \
  HONEYPOT_SERVER_ARGS="--max-sessions 1000" HONEYPOT_ARGS="--idle 800" \
  benchmarks/soak/run.sh
```

It holds about 860 sessions open (800 idle, 50 drips, the rest short), below
the cap so no client is refused, and writes about 690 MB of database an
hour (a 10-minute run on an M1 Pro), about 5.5 GB in all, in
`benchmarks/soak/logs/`: delete it afterwards. Every check in its report
must PASS. Run it in `tmux` or under `nohup`, so a dropped SSH connection
doesn't stop it.

## 3. Install

```sh
cd /opt/gem/examples/honeypot/deploy
cp honeypot.nft /etc/nftables.conf
nft -c -f /etc/nftables.conf           # check it before it can lock you out
systemctl enable --now nftables
sed "s/^HONEYPOT_COMMIT=.*/HONEYPOT_COMMIT=$(git -C /opt/gem rev-parse --short HEAD)/" honeypot.env > /etc/honeypot.env
cp honeypot.service /etc/systemd/system/
systemctl daemon-reload
systemctl enable --now honeypot
```

If SSH listens on a port other than 22, set `SSH_PORT` in
`/etc/nftables.conf` first. The ruleset replaces every other rule on the
host (`flush ruleset`), and the provider's firewall should allow the same:
22 and 23 in.

The honeypot listens on IPv4 only (`tcp_listen` has no IPv6,
`docs/ROADMAP.md`), so port 23 on the host's IPv6 address reaches nothing.

Check it from another machine: `nc VPS_IP 23` gets the `login:` prompt,
and `nc VPS_IP 2323` times out. On the VPS:

```sh
systemctl status honeypot
journalctl -u honeypot -f                              # stderr: crashes, restarts
nft list counter inet honeypot honeypot_egress         # must stay at 0 packets
```

After a minute, the health query (below) should show a row with `fds_extra`
about 9 and `res_extra` 2: the sampler reads the fds from `/dev/fd`, which
the unit's private `/dev` must still have.

## Upgrading

Rebuild the binary and `HONEYPOT_COMMIT` together, then restart:

```sh
cd /opt/gem && git pull && make build
build/gem examples/honeypot/main.gem -o /usr/local/bin/honeypot.new
mv /usr/local/bin/honeypot.new /usr/local/bin/honeypot
sed -i "s/^HONEYPOT_COMMIT=.*/HONEYPOT_COMMIT=$(git rev-parse --short HEAD)/" /etc/honeypot.env
systemctl restart honeypot
```

A restart of ours shows in the metrics like a crash would (`uptime_ms`
drops), and the sessions live then end as `lost` when it starts again:
note the time, so the health query's restarts can be told apart.

## Reading the data

The data is in `/var/lib/honeypot` (a link to `/var/lib/private/honeypot`,
which only root can open), one file per week:

```sh
sqlite3 -box /var/lib/honeypot/honeypot-2026-10-12.db < /opt/gem/examples/honeypot/deploy/health.sql
```

`examples/honeypot/README.md` has the schema and more queries. Every
stored string is attacker-controlled.

## The sandbox

`honeypot.service` gives the program no capabilities, a read-only system,
no home directories, a private `/tmp` and `/dev`, IPv4, IPv6 and Unix
sockets only, no writable and executable memory, and the
`@system-service` system calls plus `mincore` (the runtime's stack cache
uses it). These were checked against `strace` of the honeypot under the
soak's load (46 distinct system calls, none outside that set, and no
writable executable mapping) and `systemd-analyze security` (1.6, "OK").
A blocked call fails with `EPERM` rather than killing the process, so a
runtime change that needs a new one shows as an error in the journal.

`--max-sessions 1000` and `MemoryMax=2G` go together: an idle session
holds about 76 KB, but one whose client keeps sending holds up to about
1.1 MB (OPTIMIZATIONS.md, "A loop keeps its high-water memory"), so 1,000
of those take about 1.1 GB. The rest of the program is small beside that:
a peak footprint of 122 MB at 870 sessions in a 10-minute run of the soak
above on an M1 Pro, 133 MB of RSS after a 5,000-session peak on Linux. The other 2 GB of the 4 GB VPS are
for the system and for reading the data. On a 1 GB VPS, start near
`--max-sessions 300` with `MemoryMax=700M`.
