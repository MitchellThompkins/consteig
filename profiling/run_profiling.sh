#!/bin/sh
#
# Compile-time profiling for consteig.
# Times the compilation of individual .cpp files that force constexpr eigenvalues()
# evaluation at various matrix sizes and categories.
#
# HOW IT WORKS
# ------------
# We want to time how long the compiler takes to evaluate constexpr eigenvalues()
# for each matrix type and size. Each profiling/compile_time/profile_*.cpp file
# contains one matrix and one "static constexpr auto result = eigvals(mat)",
# compiling it forces full constexpr evaluation.
#
# The constexpr limits (e.g. -fconstexpr-ops-limit) are defined once in
# profiling/CMakeLists.txt via consteig_raise_compiler_limits(), the same macro
# used in the main build. To get those exact flags without invoking cmake per
# file (which adds ~1s overhead per file x 1500 files), we:
#
#   1. Run cmake once to configure the project and produce compile_commands.json.
#      compile_commands.json contains the exact compiler invocation cmake would
#      use for every source file, including all flags from CMakeLists.txt.
#
#   2. Extract the compile command for one representative profile_*.cpp file from
#      compile_commands.json, then strip the source/output path arguments to get
#      just the flags (e.g. -std=c++17 -fconstexpr-ops-limit=1000000000 ...).
#
#   3. Invoke the compiler directly for each file using those flags, timed with
#      /usr/bin/time. This is fast with no cmake startup cost per file.
# See profiling/README.md for usage examples.

set -eu

COMPILER="${1:-g++}"
TIMEOUT="${2:-300}"

# Detect whether /usr/bin/time is GNU (Linux) or BSD (macOS).
# GNU time supports -f for format strings; BSD time does not.
OS=$(uname -s)
if [ "$OS" = "Darwin" ]; then
    TIME_FLAVOR="bsd"
else
    TIME_FLAVOR="gnu"
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
RESULTS_DIR="$SCRIPT_DIR/results"
COMPILE_DIR="$SCRIPT_DIR/compile_time"
BUILD_DIR="$SCRIPT_DIR/build"

mkdir -p "$RESULTS_DIR"

# ---- Compiler identification --------------------------------------------------
# We identify the compiler family using preprocessor macros rather than parsing
# the version string. This is reliable regardless of what the binary is named
# (e.g. "c++", "g++-14", etc.). We check __clang__ before __GNUC__ because
# clang defines both for compatibility.

COMPILER_VERSION=$("$COMPILER" --version | head -1)
COMPILER_VER=$("$COMPILER" --version | head -1 | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1)

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

# ---- Step 1: CMake configuration ---------------------------------------------
# Configure the project once. This does two things:
#   - Resolves the correct constexpr flags for this compiler via
#     consteig_raise_compiler_limits() in profiling/CMakeLists.txt
#   - Writes compile_commands.json to $BUILD_DIR, which records the exact
#     compiler invocation for every profile_*.cpp file
#
# CONSTEIG_BUILD_TESTS=OFF avoids downloading Eigen and GoogleTest, which are
# not needed for profiling.
#
# CMAKE_EXPORT_COMPILE_COMMANDS=ON is what causes compile_commands.json to be
# written. Without it cmake only generates build files, not the command database.

echo "Configuring CMake (to extract compiler flags)..."
cmake -S "$REPO_ROOT" -B "$BUILD_DIR" \
    -DCMAKE_CXX_COMPILER="$COMPILER" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCONSTEIG_BUILD_PROFILING=ON \
    -DCONSTEIG_BUILD_TESTS=OFF \
    > /dev/null 2>&1 \
    || { echo "CMake configuration failed"; exit 1; }

# ---- Step 2: Extract compiler flags from compile_commands.json ---------------
# compile_commands.json has one entry per source file. Each entry looks like:
#
#   {
#     "directory": "/path/to/build",
#     "command": "g++ -std=c++17 -fconstexpr-ops-limit=1000000000 ... -c /path/to/src.cpp -o /path/to/out.o",
#     "file": "/path/to/src.cpp"
#   }
#
# We grab the command for any one profile_*.cpp file (they all get the same
# flags), then strip:
#   - The compiler binary itself (first word)
#   - "-c <source_file>" (the input file argument)
#   - "-o <output_file>" (the output file argument)
#
# What remains is purely the flags: -std=c++17, -fconstexpr-ops-limit=..., etc.
# We supply our own -c, source file, and output path when timing each file.

COMPILE_FLAGS=$(python3 - "$BUILD_DIR/compile_commands.json" "$COMPILE_DIR" <<'EOF'
import json, sys, re

db_path  = sys.argv[1]
src_dir  = sys.argv[2]

with open(db_path) as f:
    db = json.load(f)

# Find any entry whose file is inside the profiling compile_time directory
entry = next(e for e in db if src_dir in e["file"])
cmd = entry["command"]

# Remove the compiler binary (first token)
cmd = cmd.split(None, 1)[1]

# Remove -c <file> and -o <file> pairs — we supply those ourselves per-file
cmd = re.sub(r'-c\s+\S+', '', cmd)
cmd = re.sub(r'-o\s+\S+', '', cmd)

print(cmd.strip())
EOF
)

echo "Flags from CMake: $COMPILE_FLAGS"
echo ""

# ---- Profiling loop ----------------------------------------------------------
# For each profile_*.cpp file:
#   1. Invoke the compiler directly using the flags extracted above
#   2. Time it with /usr/bin/time (wall clock + peak RSS)
#   3. Record the result to the CSV

# Detect OS name for metadata (e.g. "alpine", "macos", "ubuntu").
# /etc/os-release covers most Linux distros; sw_vers covers macOS.
if [ -f /etc/os-release ]; then
    OS_NAME=$(. /etc/os-release && echo "$NAME")
elif sw_vers > /dev/null 2>&1; then
    OS_NAME="macOS $(sw_vers -productVersion)"
else
    OS_NAME="unknown"
fi

# Write compiler metadata and CSV header.
# "family" is the detected compiler family (gcc/clang) used for plot titles.
# "compiler" is the full version string for traceability.
# "os" is the host OS for context when comparing timings across machines.
printf '# family: %s\n' "$COMPILER_ID" > "$RESULTS_FILE"
printf '# compiler: %s\n' "$COMPILER_VERSION" >> "$RESULTS_FILE"
printf '# os: %s\n' "$OS_NAME" >> "$RESULTS_FILE"
echo "category,size,sample,compile_time_sec,max_rss_kb,exit_code" >> "$RESULTS_FILE"

TOTAL=$(find "$COMPILE_DIR" -name 'profile_*.cpp' | wc -l)
CURRENT=0

for src in "$COMPILE_DIR"/profile_*.cpp; do
    CURRENT=$((CURRENT + 1))
    basename_noext=$(basename "$src" .cpp)

    # Parse category, size, sample from filename: profile_<category>_<size>_<sample>.cpp
    # Category may contain underscores, so parse size and sample from the end.
    without_prefix="${basename_noext#profile_}"
    sample="${without_prefix##*_}"
    rest="${without_prefix%_*}"
    size="${rest##*_}"
    category="${rest%_*}"

    printf "[%d/%d] %s size=%s sample=%s ... " "$CURRENT" "$TOTAL" "$category" "$size" "$sample"

    # Compile directly using the flags cmake resolved. Output goes to /tmp so
    # we don't pollute the source tree. The .o file is overwritten each iteration
    # and never used after compilation.
    TIME_OUTPUT=$(mktemp)
    EXIT_CODE=0
    if [ "$TIME_FLAVOR" = "gnu" ]; then
        # GNU time (Linux): -f sets output format, -o writes to file.
        # Output: "<wall_sec> <max_rss_kb>" on the last line.
        /usr/bin/time -f "%e %M" -o "$TIME_OUTPUT" \
            timeout "$TIMEOUT" \
            "$COMPILER" $COMPILE_FLAGS \
                -c "$src" -o /tmp/profile_out.o \
            2>/dev/null \
            || EXIT_CODE=$?
        WALL_SEC=$(awk 'END{print $1}' "$TIME_OUTPUT")
        MAX_RSS=$(awk 'END{print $2}' "$TIME_OUTPUT")
    else
        # BSD time (macOS): no -f flag. Use -l for verbose stats, -o to write
        # to file. Output format:
        #   "        1.23 real         0.00 user         0.00 sys"
        #   "       9437184  maximum resident set size"  (bytes, not KB)
        /usr/bin/time -l -o "$TIME_OUTPUT" \
            timeout "$TIMEOUT" \
            "$COMPILER" $COMPILE_FLAGS \
                -c "$src" -o /tmp/profile_out.o \
            2>/dev/null \
            || EXIT_CODE=$?
        WALL_SEC=$(awk '/real/{print $1}' "$TIME_OUTPUT")
        MAX_RSS=$(awk '/maximum resident set size/{printf "%.0f\n", $1/1024}' "$TIME_OUTPUT")
    fi

    if [ "$EXIT_CODE" -eq 0 ]; then
        printf "%ss %sKB\n" "$WALL_SEC" "$MAX_RSS"
    elif [ "$EXIT_CODE" -eq 124 ]; then
        # timeout(1) returns 124 when the process is killed for exceeding TIMEOUT.
        WALL_SEC="$TIMEOUT"
        MAX_RSS=0
        printf "TIMEOUT\n"
    else
        # Non-zero, non-timeout: compiler hit its constexpr limit or errored.
        # We still record the time; it shows how long the compiler struggled
        # before giving up, which marks the practical ceiling for this category.
        printf "FAILED (exit %d) %ss %sKB\n" "$EXIT_CODE" "$WALL_SEC" "$MAX_RSS"
    fi

    echo "$category,$size,$sample,$WALL_SEC,$MAX_RSS,$EXIT_CODE" >> "$RESULTS_FILE"
    rm -f "$TIME_OUTPUT"
done

echo ""
echo "Done. Results: $RESULTS_FILE"
echo ""

# Print summary table and generate plots
echo "=== Summary (mean compile time in seconds) ==="
uv run "$SCRIPT_DIR/analyze_results.py" "$RESULTS_FILE" 2>/dev/null || \
    echo "(uv not available, run: uv run profiling/analyze_results.py <csv>)"
