---
description: Fetch N NOT_STARTED targets and match them
argument-hint: "<count> [leaf|callees-accepted|ready]"
---

# Match $1 new target(s)

Scope: ${2:-any}

**Important: Do NOT use `tools/llm_harness/run.py solve` (the llm-harness) for any of these targets. All matching work must be done manually via the standard cycle.**

Fetch $1 targets that are NOT_STARTED${2:+ and match the "${2}" frontier} from the registry and run a full matching cycle on each:

1. **Fetch & Claim** — use `targets claim-smallest` to automatically pick the smallest NOT_STARTED targets and claim them in one step:${2:
   For the `$2` frontier, use `harness --selection $2 --include-catalog` to list candidates, manually pick $1 unstarted targets, then claim each with `targets claim <target-id> --owner <you>` (since `claim-smallest` does not filter by frontier).}
   ```
   python3 tools/coop/run.py targets claim-smallest --owner <you> --num $1
   ```
   This finds the $1 smallest buildable functions by binary size (quick wins first).
2. **Cycle** — for each target, run the standard decomp loop: identity → knowledge search → reference assembly → draft → hexdiff → `cycle` → size check.
3. **Free** — `targets release <target-id> --owner <you>` after the target reaches `EQUIVALENT_MATCH` or `FULL_MATCH` to free it for other agents.
4. **Result** — report each target's final status, static percent, equivalence, and size, plus whether it was claimed and freed.
