# logstat

Summarizes web server access logs (Common or Combined Log Format): totals, then the top groups by IP, user, method, path, status or hour.

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

Malformed lines are counted, and the first five are reported on stderr. The exit status is 0 on success, 1 for an unreadable file and 2 for a bad command line.

## Layout

The directory has a `gem.toml`, which makes it a project root, so modules load by their path from here (`load "lib/stats"`).

| File | What it holds |
|---|---|
| `main.gem` | the command line (`std/args`), reads each input with `std/io`, prints the report |
| `lib/clf.gem` | parses one log line |
| `lib/stats.gem` | per-group counts and the top N |
| `lib/report.gem` | the text table and the JSON |
| `gen.gem` | writes a deterministic synthetic log |
| `test.gem` | unit tests; `tests/check_example_apps.sh` also runs the program |

## Performance

`benchmarks/logstat/run.sh` times it against the same program in Python and checks that both print the same report. On 1M lines (123 MB) on an M1 Pro, grouped by IP:

| | time | peak RSS |
|---|---|---|
| logstat, stdin | 1.77 s | 4 MB |
| logstat, file | 2.05 s | 6 MB |
| Python | 2.26 s | 22 MB |

Files and stdin are both read a line at a time, so memory stays flat.
