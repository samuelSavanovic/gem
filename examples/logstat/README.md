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

It is written the way Gem code should be written, and it is slower than
it should be: `benchmarks/logstat/run.sh` times it against the same
program in Python and checks that both print the same report. On 1M lines
(123 MB, macOS arm64, the three `--by` groupings):

| | time | peak RSS |
|---|---|---|
| logstat, stdin | 3.8–5.0 s | 20–33 MB |
| logstat, file | 3.9–5.2 s | 261–274 MB |
| Python | 2.0 s | 22–24 MB |

Where it goes: with `--by ip` or `--by path`, 0.9–1.1 s are the region
resets copying the group records again at every reset; `--by hour`
(24 groups) spends 0.1 s there. The rest is spread over allocation (every
string literal is allocated each time it is evaluated), `substr` copies
and the cost of Gem calls. A file is held whole, twice while it is read.

These are tracked in `docs/OPTIMIZATIONS.md` ("Survivors of a reset are
copied again", "String literals are allocated at every evaluation" and
"`read_file` holds the file twice") and `docs/ROADMAP.md`
("Line-at-a-time input"). Keep this
program idiomatic: it is the yardstick for those fixes, not a place to
work around them.
