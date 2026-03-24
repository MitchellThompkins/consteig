#!/usr/bin/env python3
"""Analyze profiling CSV results and print summary tables."""

import csv
import sys
from collections import defaultdict

def main():
    if len(sys.argv) < 2:
        print("Usage: analyze_results.py <compile_times.csv>")
        sys.exit(1)

    data = defaultdict(list)  # (category, size) -> [compile_time_sec, ...]
    sizes = set()
    categories = set()

    with open(sys.argv[1]) as f:
        reader = csv.DictReader(f)
        for row in reader:
            if int(row["exit_code"]) != 0:
                continue
            key = (row["category"], int(row["size"]))
            data[key].append(float(row["compile_time_sec"]))
            sizes.add(int(row["size"]))
            categories.add(row["category"])

    sizes = sorted(sizes)
    categories = sorted(categories)

    # Print table
    header = f"{'category':<25}" + "".join(f"{s:>8}" for s in sizes)
    print(header)
    print("-" * len(header))

    for cat in categories:
        row = f"{cat:<25}"
        for s in sizes:
            vals = data.get((cat, s), [])
            if vals:
                mean = sum(vals) / len(vals)
                row += f"{mean:>8.2f}"
            else:
                row += f"{'---':>8}"
        print(row)

    print("")
    print("Values are mean compile time in seconds across samples.")
    print(f"Successful compilations: {sum(len(v) for v in data.values())}")


if __name__ == "__main__":
    main()
