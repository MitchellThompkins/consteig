#!/usr/bin/env -S uv run
"""Analyze profiling CSV results, print summary tables, and generate plots."""

import csv
import os
import sys
from collections import defaultdict

def load_data(csv_path):
    data = defaultdict(list)  # (category, size) -> [compile_time_sec, ...]
    sizes = set()
    categories = set()

    with open(csv_path) as f:
        reader = csv.DictReader(f)
        for row in reader:
            if int(row["exit_code"]) != 0:
                continue
            key = (row["category"], int(row["size"]))
            data[key].append(float(row["compile_time_sec"]))
            sizes.add(int(row["size"]))
            categories.add(row["category"])

    return data, sorted(sizes), sorted(categories)


def print_table(data, sizes, categories):
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


def generate_plot(data, sizes, categories, csv_path):
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    fig, ax = plt.subplots(figsize=(12, 7))

    for cat in categories:
        xs = []
        ys = []
        for s in sizes:
            vals = data.get((cat, s), [])
            if vals:
                xs.append(s)
                ys.append(sum(vals) / len(vals))
        if xs:
            ax.plot(xs, ys, marker="o", label=cat)

    ax.set_xlabel("Matrix Size")
    ax.set_ylabel("Mean Compile Time (s)")
    ax.set_title("Constexpr eigvals() Compile Time by Matrix Size and Category")
    ax.set_xticks(sizes)
    ax.legend(bbox_to_anchor=(1.05, 1), loc="upper left", fontsize=8)
    ax.grid(True, alpha=0.3)
    fig.tight_layout()

    png_path = os.path.splitext(csv_path)[0] + ".png"
    fig.savefig(png_path, dpi=150)
    plt.close(fig)
    print(f"Plot saved: {png_path}")


def main():
    if len(sys.argv) < 2:
        print("Usage: analyze_results.py <compile_times.csv>")
        sys.exit(1)

    csv_path = sys.argv[1]
    data, sizes, categories = load_data(csv_path)

    print_table(data, sizes, categories)

    try:
        generate_plot(data, sizes, categories, csv_path)
    except ImportError:
        print("(matplotlib not available, skipping plot)")


if __name__ == "__main__":
    main()
