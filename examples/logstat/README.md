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
their path from here (`load "lib/stats"`) wherever they are loaded from.

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

It is written the way Gem code should be written, and it is slow:
`benchmarks/logstat/run.sh` times it against the same program in Python
and checks that both print the same report. On 1M lines (123 MB, Linux
x86_64):

| | time | peak RSS |
|---|---|---|
| logstat, stdin | 21–28 s | 8–18 MB |
| logstat, file | 29–37 s | 247 MB |
| Python | 4 s | 11 MB |

Where it goes (stdin, `--by ip`): reading the lines takes 0.3 s; parsing
takes 21 s, of which 11 s are the six `string.index_of` calls per line and
3.5 s the one `string.split` (std/string searches in Gem before it reaches
C); the region resets take 4.4 s, most of it copying the group tables
again at every reset. A file is held whole, twice while it is read, and
splitting it into lines costs another `index_of` per line.

These are tracked in `docs/OPTIMIZATIONS.md` ("Search and scan builtins",
"Survivors of a reset are copied again", "`read_file` holds the file
twice"), `docs/ROADMAP.md` ("Line-at-a-time input") and
`docs/KNOWN_BUGS.md` (`input()` splits lines over 4,095 bytes). Keep this
program idiomatic: it is the yardstick for those fixes, not a place to
work around them.
