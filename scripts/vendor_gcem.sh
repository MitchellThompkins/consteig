#!/usr/bin/env sh
# Vendor gcem into include/consteig/optional_dependencies/gcem/.
#
# Usage:
#   ./scripts/vendor_gcem.sh [REPO_URL] [REF]
#
# To update to a new branch or tag, pass different arguments:
#   ./scripts/vendor_gcem.sh https://github.com/MitchellThompkins/gcem.git v2.0.0

set -e

REPO_URL="${1:-https://github.com/MitchellThompkins/gcem.git}"
REF="${2:-develop}"
DEST="include/consteig/optional_dependencies/gcem"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
TMPDIR_CLONE="$(mktemp -d)"

cleanup() {
    rm -rf "$TMPDIR_CLONE"
}
trap cleanup EXIT

echo "Vendoring gcem"
echo "  repo: $REPO_URL"
echo "  ref:  $REF"
echo "  dest: $DEST"
echo ""

echo "Cloning..."
git clone --quiet --depth=1 --branch "$REF" "$REPO_URL" "$TMPDIR_CLONE"

echo "Removing old vendored files..."
rm -rf "${REPO_ROOT:?}/${DEST:?}"
mkdir -p "$REPO_ROOT/$DEST"

echo "Copying headers..."
cp "$TMPDIR_CLONE/include/gcem.hpp" "$REPO_ROOT/$DEST/gcem.hpp"
cp -r "$TMPDIR_CLONE/include/gcem_incl" "$REPO_ROOT/$DEST/gcem_incl"

NFILES=$(find "$REPO_ROOT/$DEST" -name "*.hpp" | wc -l | tr -d ' ')
echo ""
echo "Done. Copied $NFILES header files to $DEST"
