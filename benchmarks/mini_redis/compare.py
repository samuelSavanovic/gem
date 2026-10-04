#!/usr/bin/env python3
"""Prints redis-benchmark --csv results for redis-server and mini_redis
side by side: requests per second, the ratio, and p50/p99 latency.

    compare.py PHASE REDIS_CSV GEM_CSV
"""
import csv
import sys


def load(path):
    with open(path, newline="") as f:
        return {row["test"]: row for row in csv.DictReader(f)}


def main():
    phase, redis_path, gem_path = sys.argv[1:4]
    redis, gem = load(redis_path), load(gem_path)
    print(f"{phase}")
    print(f"  {'test':<36} {'redis rps':>10} {'gem rps':>10} {'gem/redis':>9}"
          f" {'redis p50':>9} {'gem p50':>8} {'redis p99':>9} {'gem p99':>8}")
    for test, r in redis.items():
        g = gem.get(test)
        if g is None:
            print(f"  {test:<36} {float(r['rps']):>10.0f} {'missing':>10}")
            continue
        rr, gr = float(r["rps"]), float(g["rps"])
        print(f"  {test:<36} {rr:>10.0f} {gr:>10.0f} {gr / rr:>9.2f}"
              f" {float(r['p50_latency_ms']):>9.3f} {float(g['p50_latency_ms']):>8.3f}"
              f" {float(r['p99_latency_ms']):>9.3f} {float(g['p99_latency_ms']):>8.3f}")


if __name__ == "__main__":
    main()
