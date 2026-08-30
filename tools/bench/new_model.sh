#!/usr/bin/env bash
# new_model.sh <model-slug> [base-branch]
#
# Create a per-model benchmark worktree + branch for the local-model decomp
# benchmarks.
#
#   branch:  bench/<model-slug>   (branching from the base branch)
#   worktree: ~/Ahmed/xenoblade-bench-<model-slug>
#
# The new worktree is fully wired the same way as the base worktree:
#   - build/  -> symlink to the shared build dir (4.6G; keep ONE build tree)
#   - .venv/  -> symlink to the shared python venv (121M)
#   - orig/   -> copied (retail main.dol + splits, ~5.5M)
#   - .ninja_log/.ninja_deps copied so ninja knows the shared build is warm
#   - build.ninja + compile_commands.json regenerated via configure.py
#
# NOTE on build-dir sharing: every worktree writes compiled objects into the
# SAME build/ tree. Run ONE model at a time, or per-model isolated runs will
# clobber each other's .o files. (The paseo/llm harnesses create their own
# per-run worktrees for isolation; this helper is for the branch scaffold.)
set -euo pipefail

MODEL="${1:?usage: new_model.sh <model-slug> [base-branch]}"
BASE="${2:-base}"

# Repo root of the MAIN checkout that owns the git objects (parent of the
# worktree symlinks target). The script is placed in <worktree>/tools/bench,
# so walk up to the checkout root, then resolve the real path to find the
# main checkout's build/venv dirs.
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# tools/bench -> checkout root
WORKTREE_ROOT="$(cd "$HERE/../.." && pwd)"
# if we are inside a git worktree, the symlink targets live in the main
# checkout whose .venv/build are real dirs; resolve via git rev-parse.
if ! MAIN_ROOT="$(git -C "$WORKTREE_ROOT" rev-parse --show-toplevel 2>/dev/null)"; then
    echo "error: not inside a git checkout: $WORKTREE_ROOT" >&2
    exit 1
fi
# Resolve the main checkout (the first worktree registered, which owns the
# real .venv/build rather than symlinks).
MAIN_REAL="$(git -C "$WORKTREE_ROOT" worktree list | awk 'NR==1{print $1}')"

BRANCH="bench/$MODEL"
WT="/Users/ahmed/projects/xenoblade-bench-$MODEL"

if git -C "$WORKTREE_ROOT" rev-parse --verify -q "$BRANCH" >/dev/null; then
    echo "error: branch '$BRANCH' already exists" >&2
    exit 1
fi
if [ -e "$WT" ]; then
    echo "error: worktree path already exists: $WT" >&2
    exit 1
fi

echo "==> creating branch '$BRANCH' from '$BASE' at $WT"
git -C "$WORKTREE_ROOT" worktree add -b "$BRANCH" "$WT" "$BASE"

echo "==> wiring build/ .venv/ orig/ ninja logs"
ln -s "$MAIN_REAL/build" "$WT/build"
ln -s "$MAIN_REAL/.venv" "$WT/.venv"
# git only tracks the .gitkeep placeholders under orig/; the real retail
# files (sys/main.dol etc.) are gitignored. Drop the placeholder tree first
# so the copy lands at orig/<region>/... and not orig/orig/<region>/...
rm -rf "$WT/orig"
cp -R "$MAIN_REAL/orig" "$WT/orig"
cp "$MAIN_REAL/.ninja_log" "$MAIN_REAL/.ninja_deps" "$WT/"

echo "==> regenerating build.ninja for this branch"
(cd "$WT" && .venv/bin/python3 configure.py)

echo "==> done:"
git -C "$WT" status --short --branch | head -1
echo "    worktree : $WT"
echo "    branch   : $BRANCH"
echo "    next     : cd $WT && tools/coop/hexdiff.py <unit> --all"
