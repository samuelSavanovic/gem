# logstat

Summarizes web server access logs (Common or Combined Log Format): totals,
then the top groups by IP, user, method, path, status or hour.

```sh
cd examples/logstat
../../build/gem gen.gem 100000 > /tmp/access.log      # a synthetic log
../../build/gem main.gem -o logstat
./logstat --by ip --top 5 /tmp/access.log
./logstat --by status --sort bytes --json < /tmp/access.log
./logstat --help
```

```
lines 100000, malformed 5, requests 99995, bytes 910666115, errors 17375 (17.4%)

top 5 of 990 by ip, sorted by requests
requests    bytes  errors  ip
     648  5926882     110  10.0.0.0
     ...
```

Malformed lines are counted and the first five reported on stderr. Exit
status: 0, 1 for an unreadable file, 2 for a bad command line.

## Layout

A `gem.toml` marks this directory as a project root, so modules load by
their path from here (`load "lib/stats"`) for an entry file in this
directory.

| File | What it holds |
|---|---|
| `main.gem` | the command: reads each input, prints the report |
| `lib/args.gem` | command-line parsing and the usage text |
| `lib/source.gem` | the lines of a file or of stdin |
| `lib/clf.gem` | parses one log line |
| `lib/stats.gem` | per-group counts and the top N |
| `lib/report.gem` | the text table and the JSON |
| `gen.gem` | writes a deterministic synthetic log |
| `test.gem` | unit tests; `tests/check_example_apps.sh` also runs the program |

## Performance

`benchmarks/logstat/run.sh` times it against the same program in Python
and checks that both print the same report. On 1M lines (123 MB, the
three `--by` groupings; macOS arm64,
`benchmarks/baselines/2026-10-07_m1pro`):

| | time | peak RSS |
|---|---|---|
| logstat, stdin | 1.29–1.35 s | 3–7 MB |
| logstat, file | 1.32–1.38 s | 239–243 MB |
| Python | 1.67–1.71 s | 21–24 MB |

Where it goes: the region resets take 0.05–0.10 s of a run, since they
promote the group records instead of copying them again
(`GEM_DIAG=1 benchmarks/logstat/run.sh`). The rest is spread over
string-key hashing in table get/set, `substr` copies, std/string's
argument checks and the cost of Gem calls. A file is held whole, twice
while it is read.

These are tracked in `docs/OPTIMIZATIONS.md` ("Hash string table keys
faster" and "`read_file` holds the file twice at its peak") and
`docs/ROADMAP.md` ("Line-at-a-time input from files and stdin"). Keep
this program idiomatic: it is the yardstick for those fixes, not a place
to work around them.
