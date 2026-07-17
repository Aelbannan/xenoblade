#!/usr/bin/env bash
# Apply all llm-harness winners to the repo.
# Usage: bash tools/coop/apply-harness-winners.sh [--dry-run]
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
DRY_RUN=false
if [ "${1:-}" = "--dry-run" ]; then
    DRY_RUN=true
fi

OWNER="agent"

# All winners from experiments.jsonl with status >= COMPILES
# format: target_id experiment_dir status
CANDIDATES=(
    "us-800395a0 build/llm-harness/us-800395a0/20260716T235412Z-9eb254b1 FULL_MATCH"
    "us-80039648 build/llm-harness/us-80039648/20260716T235412Z-bf93e758 FULL_MATCH"
    "us-800396c0 build/llm-harness/us-800396c0/20260716T235412Z-76437f6b FULL_MATCH"
    "us-800396c8 build/llm-harness/us-800396c8/20260716T235412Z-350981e5 FULL_MATCH"
    "us-8003a578 build/llm-harness/us-8003a578/20260716T234504Z-31df05be FULL_MATCH"
    "us-80040a64 build/llm-harness/us-80040a64/20260716T234504Z-8b8f8fa1 CODE_MATCH"
    "us-80040a6c build/llm-harness/us-80040a6c/20260716T234504Z-c3e721ea CODE_MATCH"
    "us-80044870 build/llm-harness/us-80044870/20260716T234504Z-9a77aaa3 CODE_MATCH"
    "us-80044978 build/llm-harness/us-80044978/20260716T234504Z-617a074f CODE_MATCH"
    "us-80044f3c build/llm-harness/us-80044f3c/20260716T234504Z-ae283dd8 COMPILES"
    "us-8004b6ec build/llm-harness/us-8004b6ec/20260716T234504Z-b62a7e7b FULL_MATCH"
    "us-80061f5c build/llm-harness/us-80061f5c/20260716T234615Z-c62e39e1 FULL_MATCH"
    "us-8006be44 build/llm-harness/us-8006be44/20260716T234640Z-01b01e1f FULL_MATCH"
    "us-8006bf60 build/llm-harness/us-8006bf60/20260716T234642Z-fc369167 FULL_MATCH"
    "us-8006c008 build/llm-harness/us-8006c008/20260716T234644Z-696387f8 FULL_MATCH"
    "us-8006c02c build/llm-harness/us-8006c02c/20260716T234700Z-cbdef311 FULL_MATCH"
    "us-8007ce54 build/llm-harness/us-8007ce54/20260716T234716Z-fd88e3d7 FULL_MATCH"
    "us-80087c10 build/llm-harness/us-80087c10/20260716T234717Z-2b74ee9e FULL_MATCH"
    "us-800aaa80 build/llm-harness/us-800aaa80/20260716T234720Z-73c1de51 COMPILES"
    "us-800aab88 build/llm-harness/us-800aab88/20260716T234741Z-82bcf07e COMPILES"
    "us-800aab9c build/llm-harness/us-800aab9c/20260716T234751Z-15e21945 COMPILES"
)

PROMOTED=()
FAILED=()

for entry in "${CANDIDATES[@]}"; do
    read -r target_id exp_dir status <<< "$entry"
    echo ""
    echo "================================================================================"
    echo "  $target_id ($status)"
    echo "========================================================================"

    # 1. Claim the target (skip if already claimed)
    echo "  [1/3] Claiming $target_id ..."
    if $DRY_RUN; then
        echo "        (dry-run) would claim $target_id for $OWNER"
    else
        claim_out=$(python3 "$ROOT/tools/coop/run.py" targets claim "$target_id" --owner "$OWNER" --note "Harness $status promotion" 2>&1 || true)
        echo "        $claim_out"
    fi

    # 2. Promote (write candidate source)
    echo "  [2/3] Promoting $target_id ..."
    if $DRY_RUN; then
        echo "        (dry-run) would promote $exp_dir"
    else
        promote_out=$(python3 "$ROOT/tools/llm_harness/run.py" promote "$ROOT/$exp_dir" --write --owner "$OWNER" 2>&1 || true)
        echo "        $promote_out"
    fi

    # 3. Run cycle to verify and update targets.json
    echo "  [3/3] Running cycle for $target_id ..."
    if $DRY_RUN; then
        echo "        (dry-run) would run cycle $target_id"
    else
        cycle_out=$(python3 "$ROOT/tools/coop/run.py" cycle "$target_id" --hypothesis "Harness $status auto-promotion" --next-change "None" 2>&1 || true)
        # Extract just the key summary lines
        echo "$cycle_out" | grep -E '(status:|size:|equivalence:|certificate:)' || echo "        (cycle complete — no status summary line found)"
    fi

    PROMOTED+=("$target_id")
done

echo ""
echo "================================================================================"
echo "  Summary: ${#PROMOTED[@]} targets processed"
echo "================================================================================"
echo ""
echo "Promoted targets:"
for t in "${PROMOTED[@]}"; do echo "  $t"; done
echo ""
python3 "$ROOT/tools/coop/run.py" targets status 2>&1 | grep -E "(us-8003|us-8004|us-8006|us-8007|us-800a|us-8008|us-8000)" | head -30
