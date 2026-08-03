#!/usr/bin/env bash
# Install the pre-commit hook into this repository (.git/hooks/).
set -euo pipefail

cd "$(git rev-parse --show-toplevel)"

DEST=.git/hooks/pre-commit
if [ -e "$DEST" ] && [ ! -L "$DEST" ]; then
    echo "error: $DEST already exists and is not a symlink" >&2
    echo "       remove it first, or install .githooks/pre-commit manually" >&2
    exit 1
fi

ln -sfn ../../.githooks/pre-commit "$DEST"
echo "installed: $DEST -> .githooks/pre-commit"
echo "runs on every commit; skip with: git commit --no-verify"
