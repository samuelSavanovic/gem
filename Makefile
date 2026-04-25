.PHONY: test test-json clean

test:
	@bash examples/run_all.sh

test-json:
	@source .venv/bin/activate && python3 bootstrap/gem.py examples/json_parser.gem --run

clean:
	rm -f /tmp/gem_*
