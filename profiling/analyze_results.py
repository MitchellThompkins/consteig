#!/usr/bin/env -S uv run
"""Analyze profiling CSV results, print summary tables, and generate plots."""

import csv
import os
import sys
from collections import defaultdict

def load_data(csv_path):
    success = defaultdict(list)   # (category, size) -> [compile_time_sec, ...]
    failed  = defaultdict(list)   # (category, size) -> [compile_time_sec, ...]
    sizes = set()
    categories = set()

    with open(csv_path) as f:
        reader = csv.DictReader(f)
        for row in reader:
            key = (row["category"], int(row["size"]))
            t = float(row["compile_time_sec"])
            sizes.add(int(row["size"]))
            categories.add(row["category"])
            if int(row["exit_code"]) == 0:
                success[key].append(t)
            elif int(row["exit_code"]) != 124:  # 124 = timeout, skip
                failed[key].append(t)

    return success, failed, sorted(sizes), sorted(categories)


def print_table(success, failed, sizes, categories):
    header = f"{'category':<25}" + "".join(f"{s:>8}" for s in sizes)
    print(header)
    print("-" * len(header))

    for cat in categories:
        row = f"{cat:<25}"
        for s in sizes:
            svals = success.get((cat, s), [])
            fvals = failed.get((cat, s), [])
            if svals:
                mean = sum(svals) / len(svals)
                row += f"{mean:>8.2f}"
            elif fvals:
                mean = sum(fvals) / len(fvals)
                row += f"{'F'+f'{mean:.1f}':>8}"
            else:
                row += f"{'---':>8}"
        print(row)

    print("")
    print("Values are mean compile time in seconds. F<t> = compiler limit hit at <t>s.")
    print(f"Successful: {sum(len(v) for v in success.values())}  "
          f"Failed: {sum(len(v) for v in failed.values())}")


def generate_plot(success, failed, sizes, categories, csv_path):
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    fig, ax = plt.subplots(figsize=(12, 7))

    prop_cycle = plt.rcParams["axes.prop_cycle"]
    colors = {cat: c["color"] for cat, c in zip(categories, prop_cycle)}

    for cat in categories:
        color = colors[cat]
        xs = [s for s in sizes if success.get((cat, s))]
        ys = [sum(success[(cat, s)]) / len(success[(cat, s)]) for s in xs]
        if xs:
            ax.plot(xs, ys, marker="o", label=cat, color=color)

        fxs = [s for s in sizes if failed.get((cat, s))]
        fys = [sum(failed[(cat, s)]) / len(failed[(cat, s)]) for s in fxs]
        if fxs:
            ax.plot(fxs, fys, marker="x", markersize=10, linestyle="none",
                    color=color)

    ax.set_xlabel("Matrix Size")
    ax.set_ylabel("Mean Compile Time (s)")
    ax.set_title("Constexpr eigvals() Compile Time by Matrix Size and Category")
    ax.set_xticks(sizes)
    ax.legend(bbox_to_anchor=(1.05, 1), loc="upper left", fontsize=8)
    ax.grid(True, alpha=0.3)

    # Annotate the failure region
    fail_sizes = sorted({s for (_, s) in failed})
    if fail_sizes:
        ax.axvspan(fail_sizes[0] - 1, max(sizes) + 1, alpha=0.05, color="red",
                   label="compiler limit")

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
    success, failed, sizes, categories = load_data(csv_path)

    print_table(success, failed, sizes, categories)

    try:
        generate_plot(success, failed, sizes, categories, csv_path)
    except ImportError:
        print("(matplotlib not available, skipping plot)")


if __name__ == "__main__":
    main()
