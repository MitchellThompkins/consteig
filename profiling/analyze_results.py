#!/usr/bin/env -S uv run
# /// script
# dependencies = ["matplotlib"]
# ///
"""Analyze profiling CSV results, print summary tables, and generate plots."""

import csv
import os
import sys
from collections import defaultdict

def load_data(csv_path):
    success = defaultdict(list)   # (category, size) -> [compile_time_sec, ...]
    failed  = defaultdict(list)   # (category, size) -> [compile_time_sec, ...]
    memory  = defaultdict(list)   # (category, size) -> [max_rss_kb, ...]  (success only)
    sizes = set()
    categories = set()

    compiler_info = None
    compiler_family = None
    os_name = None
    with open(csv_path) as f:
        lines = []
        for line in f:
            if line.startswith("# compiler:"):
                compiler_info = line[len("# compiler:"):].strip()
            elif line.startswith("# family:"):
                compiler_family = line[len("# family:"):].strip()
            elif line.startswith("# os:"):
                os_name = line[len("# os:"):].strip()
            elif not line.startswith("#"):
                lines.append(line)

    reader = csv.DictReader(lines)
    for row in reader:
            key = (row["category"], int(row["size"]))
            t = float(row["compile_time_sec"])
            sizes.add(int(row["size"]))
            categories.add(row["category"])
            if int(row["exit_code"]) == 0:
                success[key].append(t)
                memory[key].append(float(row["max_rss_kb"]))
            elif int(row["exit_code"]) != 124:  # 124 = timeout, skip
                failed[key].append(t)

    return success, failed, memory, sorted(sizes), sorted(categories), compiler_info, compiler_family, os_name


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


def generate_memory_plot(memory, sizes, categories, csv_path, compiler_label=None):
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    fig, ax = plt.subplots(figsize=(12, 7))

    import itertools
    prop_cycle = plt.rcParams["axes.prop_cycle"]
    colors = {cat: c["color"] for cat, c in zip(categories, itertools.cycle(prop_cycle))}

    for cat in categories:
        xs = [s for s in sizes if memory.get((cat, s))]
        ys = [sum(memory[(cat, s)]) / len(memory[(cat, s)]) / 1024 for s in xs]  # MB
        if xs:
            ax.plot(xs, ys, marker="o", label=cat, color=colors[cat])

    ax.set_xlabel("Matrix Size")
    ax.set_ylabel("Mean RSS (MB)")
    title = "Constexpr eigvals() Compiler Memory by Matrix Size and Category"
    if compiler_label:
        title += f"\n{compiler_label}"
    ax.set_title(title)
    ax.set_xticks(sizes)
    ax.grid(True, alpha=0.3)

    ax.axvspan(13.5, max(sizes) + 0.5, alpha=0.08, color="red",
               label="consteig_raise_compiler_limits required")

    # Legend placed after the span so its label is included.
    ax.legend(bbox_to_anchor=(1.05, 1), loc="upper left", fontsize=8)
    fig.tight_layout()

    png_path = os.path.splitext(csv_path)[0] + "_memory.png"
    fig.savefig(png_path, dpi=150)
    plt.close(fig)
    print(f"Plot saved: {png_path}")


def generate_plot(success, failed, sizes, categories, csv_path, compiler_label=None):
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    fig, ax = plt.subplots(figsize=(12, 7))

    import itertools
    prop_cycle = plt.rcParams["axes.prop_cycle"]
    colors = {cat: c["color"] for cat, c in zip(categories, itertools.cycle(prop_cycle))}

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
    title = "Constexpr eigvals() Compile Time by Matrix Size and Category"
    if compiler_label:
        title += f"\n{compiler_label}"
    ax.set_title(title)
    ax.set_xticks(sizes)
    ax.grid(True, alpha=0.3)

    # Shaded region from size 14 onwards: this is where the default compiler
    # constexpr budget is exceeded and consteig_raise_compiler_limits is required.
    ax.axvspan(13.5, max(sizes) + 0.5, alpha=0.08, color="red",
               label="consteig_raise_compiler_limits required")

    # Separate shaded region for sizes where failures actually occur in this data.
    fail_sizes = sorted({s for (_, s) in failed})
    if fail_sizes:
        ax.axvspan(fail_sizes[0] - 0.5, max(sizes) + 0.5, alpha=0.08, color="darkred",
                   label="compiler limit exceeded")

    # Legend is placed after the spans so their labels are included.
    ax.legend(bbox_to_anchor=(1.05, 1), loc="upper left", fontsize=8)

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
    success, failed, memory, sizes, categories, compiler_info, compiler_family, os_name = load_data(csv_path)

    # The filename is always compile_times_<family>_<version>.csv — use it as
    # the canonical compiler label for titles regardless of CSV metadata.
    compiler_label = os.path.splitext(os.path.basename(csv_path))[0]
    compiler_label = compiler_label.replace("compile_times_", "").replace("_", " ", 1)
    if os_name:
        compiler_label += f" ({os_name})"

    print(f"Compiler: {compiler_label}")
    print("")

    print_table(success, failed, sizes, categories)

    try:
        generate_plot(success, failed, sizes, categories, csv_path, compiler_label)
        generate_memory_plot(memory, sizes, categories, csv_path, compiler_label)
    except ImportError:
        print("(matplotlib not available, skipping plot)")


if __name__ == "__main__":
    main()
