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
BUILD_DIR="$SCRIPT_DIR/build"

mkdir -p "$RESULTS_DIR"

COMPILER_VERSION=$("$COMPILER" --version | head -1)
COMPILER_VER=$("$COMPILER" --version | head -1 | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1)

# Detect compiler family via preprocessor macros — reliable regardless of binary name.
# Check __clang__ before __GNUC__ because clang defines both.
PROBE=$(echo "" | "$COMPILER" -E -dM -x c++ - 2>/dev/null)
if echo "$PROBE" | grep -q "^#define __clang__"; then
    COMPILER_ID="clang"
elif echo "$PROBE" | grep -q "^#define __GNUC__"; then
    COMPILER_ID="gcc"
else
    COMPILER_ID=$(basename "$COMPILER")
    echo "Warning: unrecognized compiler family"
fi

RESULTS_FILE="$RESULTS_DIR/compile_times_${COMPILER_ID}_${COMPILER_VER}.csv"

echo "Compiler: $COMPILER_VERSION"
echo "Family:   $COMPILER_ID"
echo "Timeout:  ${TIMEOUT}s per file"
echo "Output:   $RESULTS_FILE"
echo ""

# Configure cmake (once). Constexpr flags are applied via consteig_raise_compiler_limits
# in profiling/CMakeLists.txt — no manual flag management needed here.
echo "Configuring CMake..."
cmake -S "$REPO_ROOT" -B "$BUILD_DIR" \
    -DCMAKE_CXX_COMPILER="$COMPILER" \
    -DCONSTEIG_BUILD_TESTS=OFF \
    > /dev/null 2>&1 \
    || { echo "CMake configuration failed"; exit 1; }
echo "Done."
echo ""

# Write compiler metadata and CSV header
printf '# compiler: %s\n' "$COMPILER_VERSION" > "$RESULTS_FILE"
echo "category,size,sample,compile_time_sec,max_rss_kb,exit_code" >> "$RESULTS_FILE"

# Count total files for progress
TOTAL=$(find "$COMPILE_DIR" -name 'profile_*.cpp' | wc -l)
CURRENT=0

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

    # Delete the object file to force recompilation of this target
    rm -f "$BUILD_DIR/profiling/CMakeFiles/${basename_noext}.dir/compile_time/${basename_noext}.cpp.o"

    # Use /usr/bin/time for wall clock and RSS, with timeout
    TIME_OUTPUT=$(mktemp)
    EXIT_CODE=0
    /usr/bin/time -f "%e %M" -o "$TIME_OUTPUT" \
        timeout "$TIMEOUT" \
        cmake --build "$BUILD_DIR" --target "$basename_noext" \
        > /dev/null 2>&1 \
        || EXIT_CODE=$?

    WALL_SEC=$(awk 'END{print $1}' "$TIME_OUTPUT")
    MAX_RSS=$(awk 'END{print $2}' "$TIME_OUTPUT")

    if [ "$EXIT_CODE" -eq 0 ]; then
        printf "%ss %sKB\n" "$WALL_SEC" "$MAX_RSS"
    elif [ "$EXIT_CODE" -eq 124 ]; then
        WALL_SEC="$TIMEOUT"
        MAX_RSS=0
        printf "TIMEOUT\n"
    else
        printf "FAILED (exit %d) %ss %sKB\n" "$EXIT_CODE" "$WALL_SEC" "$MAX_RSS"
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
