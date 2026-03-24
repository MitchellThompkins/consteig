# Compile-Time Profiling

Measures how long the compiler takes to evaluate `constexpr eigvals()` across different matrix sizes and categories.

## Running

From the repository root:

```sh
./profiling/run_profiling.sh [compiler] [timeout_sec]
```

Examples:

```sh
./profiling/run_profiling.sh           # g++, 300s timeout (default)
./profiling/run_profiling.sh clang++   # Clang
./profiling/run_profiling.sh g++ 600   # GCC, 10-minute timeout
```

Results are written to `profiling/results/compile_times_<timestamp>.csv`.

**Requirements:** `g++` or `clang++`, `/usr/bin/time`, `timeout`.

## Plotting Results

`analyze_results.py` prints a summary table and generates a plot. It is run automatically at the end of `run_profiling.sh`, but can also be run manually:

```sh
uv run profiling/analyze_results.py profiling/results/compile_times_<timestamp>.csv
```

This prints a table of mean compile times (seconds) by category and matrix size, and saves a PNG plot alongside the CSV file.

**Requirements:** Python 3.14 (see `.python-version`), managed by `uv`. `matplotlib` is optional; if absent the table is still printed.

## Output Format

The CSV has one row per compilation:

| Column | Description |
|--------|-------------|
| `category` | Matrix category (e.g. `clustered`, `companion`) |
| `size` | Matrix dimension (e.g. `10` for 10x10) |
| `sample` | Sample index within the category/size group |
| `compile_time_sec` | Wall-clock compile time in seconds |
| `max_rss_kb` | Peak resident memory in KB |
| `exit_code` | `0` = success, `124` = timeout, other = failure |

## Source Files

Individual `.cpp` files in `compile_time/` each define a single `constexpr` matrix and call `eigvals()`, forcing full compile-time evaluation. Filenames follow the pattern `profile_<category>_<size>_<sample>.cpp`.
