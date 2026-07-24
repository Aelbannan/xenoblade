#!/usr/bin/env bash
# ──────────────────────────────────────────────────────────────────────────────
# Run the xenoblade_decomp_fanout workflow.
#
# The workflow handles batch preparation internally (reads targets.json via the
# first agent). You just specify how many functions to target.
#
# Usage:
#   ./tools/run_decomp_workflow.sh [--limit N] [--status DISCOVERY BACKLOG]
#
# Examples:
#   ./tools/run_decomp_workflow.sh --limit 10
#   ./tools/run_decomp_workflow.sh --limit 1000 --status DISCOVERY
#   ./tools/run_decomp_workflow.sh --status BACKLOG
# ──────────────────────────────────────────────────────────────────────────────
set -euo pipefail

LIMIT=""
STATUSES=()

while [[ $# -gt 0 ]]; do
	case "$1" in
	--limit)
		LIMIT="$2"
		shift 2
		;;
	--status)
		shift
		while [[ $# -gt 0 && ! "$1" =~ ^-- ]]; do
			STATUSES+=("$1")
			shift
		done
		;;
	--help | -h)
		echo "Usage: $0 [--limit N] [--status DISCOVERY BACKLOG]"
		echo ""
		echo "Starts the xenoblade_decomp_fanout workflow with the given config."
		echo "The first agent reads targets.json and prepares the batch internally."
		exit 0
		;;
	*)
		echo "Unknown option: $1"
		exit 1
		;;
	esac
done

if [[ ${#STATUSES[@]} -eq 0 ]]; then
	STATUSES=("DISCOVERY" "BACKLOG")
fi

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  Xenoblade Decomp Workflow"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  Statuses: ${STATUSES[*]}"
[[ -n "$LIMIT" ]] && echo "  Limit:    $LIMIT" || echo "  Limit:    (all)"
echo ""
echo "  The workflow's first agent reads targets.json internally."
echo "  Agents have full tool access (bash, read, write)."
echo "  Each agent: reads asm -> decompiles -> runs cycle -> iterates -> releases."
echo ""

echo "Next step - invoke:"
echo ""
echo "  workflow({"
echo '    name: "xenoblade_decomp_fanout",'
echo "    args: {"
echo "      limit: ${LIMIT:-null},"
echo '      statuses: ["'${STATUSES[*]// /\", \"}'"]'
echo "    },"
echo "    maxAgents: $([[ -n "$LIMIT" ]] && echo "$LIMIT + 10" || echo "2000")",
echo "    concurrency: 50"
echo "  })"
echo ""
