#!/bin/sh
#
# Compile-time profiling for consteig.
# Times the compilation of individual .cpp files that force constexpr eigvals()
# evaluation at various matrix sizes and categories.
#
# Usage:
#   ./profiling/run_profiling.sh [compiler] [timeout_sec]
#
# Examples:
#   ./profiling/run_profiling.sh g++        # GCC, default 300s timeout
#   ./profiling/run_profiling.sh clang++    # Clang
#   ./profiling/run_profiling.sh g++ 600    # GCC with 10min timeout

set -eu

COMPILER="${1:-g++}"
TIMEOUT="${2:-300}"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
RESULTS_DIR="$SCRIPT_DIR/results"
COMPILE_DIR="$SCRIPT_DIR/compile_time"

mkdir -p "$RESULTS_DIR"

COMPILER_VERSION=$("$COMPILER" --version | head -1)
if "$COMPILER" --version 2>&1 | grep -qi gcc; then
    COMPILER_ID="gcc"
elif "$COMPILER" --version 2>&1 | grep -qi clang; then
    COMPILER_ID="clang"
else
    COMPILER_ID=$(basename "$COMPILER")
fi
COMPILER_VER=$("$COMPILER" --version | head -1 | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1)
RESULTS_FILE="$RESULTS_DIR/compile_times_${COMPILER_ID}_${COMPILER_VER}.csv"

echo "Compiler: $COMPILER_VERSION"
echo "Timeout:  ${TIMEOUT}s per file"
echo "Output:   $RESULTS_FILE"
echo ""

# CSV header
echo "category,size,sample,compile_time_sec,max_rss_kb,exit_code" > "$RESULTS_FILE"

# Count total files for progress
TOTAL=$(find "$COMPILE_DIR" -name 'profile_*.cpp' | wc -l)
CURRENT=0

# Determine constexpr flags based on compiler
if "$COMPILER" --version 2>&1 | grep -qi gcc; then
    CONSTEXPR_FLAGS="-fconstexpr-ops-limit=1000000000 -fconstexpr-depth=1024"
elif "$COMPILER" --version 2>&1 | grep -qi clang; then
    CONSTEXPR_FLAGS="-fconstexpr-steps=1000000000 -fconstexpr-depth=1024"
else
    CONSTEXPR_FLAGS="-fconstexpr-depth=1024"
fi

for src in "$COMPILE_DIR"/profile_*.cpp; do
    CURRENT=$((CURRENT + 1))
    basename_noext=$(basename "$src" .cpp)

    # Parse category, size, sample from filename: profile_<category>_<size>_<sample>.cpp
    # Category may contain underscores, so parse size and sample from the end
    without_prefix="${basename_noext#profile_}"
    sample="${without_prefix##*_}"
    rest="${without_prefix%_*}"
    size="${rest##*_}"
    category="${rest%_*}"

    printf "[%d/%d] %s size=%s sample=%s ... " "$CURRENT" "$TOTAL" "$category" "$size" "$sample"

    # Use /usr/bin/time for wall clock and RSS, with timeout
    TIME_OUTPUT=$(mktemp)
    EXIT_CODE=0
    /usr/bin/time -f "%e %M" -o "$TIME_OUTPUT" \
        timeout "$TIMEOUT" \
        "$COMPILER" -std=c++17 -O0 -I"$REPO_ROOT" \
        $CONSTEXPR_FLAGS \
        -c "$src" -o /tmp/profile_out.o 2>/dev/null \
        || EXIT_CODE=$?

    if [ "$EXIT_CODE" -eq 0 ]; then
        WALL_SEC=$(awk '{print $1}' "$TIME_OUTPUT")
        MAX_RSS=$(awk '{print $2}' "$TIME_OUTPUT")
        printf "%ss %sKB\n" "$WALL_SEC" "$MAX_RSS"
    elif [ "$EXIT_CODE" -eq 124 ]; then
        WALL_SEC="$TIMEOUT"
        MAX_RSS=0
        printf "TIMEOUT\n"
    else
        WALL_SEC=0
        MAX_RSS=0
        printf "FAILED (exit %d)\n" "$EXIT_CODE"
    fi

    echo "$category,$size,$sample,$WALL_SEC,$MAX_RSS,$EXIT_CODE" >> "$RESULTS_FILE"
    rm -f "$TIME_OUTPUT"
done

echo ""
echo "Done. Results: $RESULTS_FILE"
echo ""

# Print summary table
echo "=== Summary (mean compile time in seconds) ==="
uv run "$SCRIPT_DIR/analyze_results.py" "$RESULTS_FILE" 2>/dev/null || \
    echo "(uv not available, run: uv run profiling/analyze_results.py <csv>)"
