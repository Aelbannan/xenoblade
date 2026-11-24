---
name: xb-wave-planner
description: >-
  Plan one Xenoblade matching wave (Opus role): trim /tmp/xb/waves/<W>/targets.tsv to the ~20
  targets cheap agents can finish faithfully, route each to hy4 or DeepSeek, write per-task hints.
  Invoke when told "use the xb-wave-planner skill. Wave: W…".
---

# Wave planner

You plan matching wave `W` (given in the launch message) for the Xenoblade Wii decomp at
/Users/ahmed/projects/xenoblade. The workers are cheap models (hy4 flash, DeepSeek flash), and each
works on one function with a generated brief (a "pack"). Your output decides what they attempt.

Read first:
- `CLAUDE.md`
- `docs/land_gate.md`
- the `RULES` in `tools/coop/land/pack.py` (banned shapes)
- the "Mechanical / data" section of `docs/orchestrator_followups.md`, so you don't re-plan known
  debt

## Input
`/tmp/xb/waves/W/targets.tsv`: 8 tab-separated columns, in this order: task, model, unit, symbol, pct,
structural, size, src. `wave select` has already removed ✓ functions, register-allocation-only
residuals, functions over 0x400 bytes and functions already tried at this source state.

## You may
- Run `.venv/bin/python3 -m tools.coop.land pack <unit> <symbol>` for at most 12 targets.
- Run `.venv/bin/python3 tools/coop/hexdiff.py <unit> --symbol <sym>`.

## You may not
- Edit any repo file other than `docs/orchestrator_followups.md`.
- Run git, or `tools.coop.land check/apply`.

## Produce exactly
1. **The rewritten `/tmp/xb/waves/W/targets.tsv`.**
   - Same 8 columns; keep the best ~20 rows (10–24); leave task names unchanged.
   - Drop targets whose residual needs one of these, and list them in plan.md instead:
     - a hub or header redesign;
     - an SDK or middleware idiom change (nw4r, CRI, BTE, MSL, RVL SDK keep their idioms);
     - data, split or symbol work;
     - anything a worker could only "fix" with a banned shape.
   - Set column 2 to `hy4` (default) or `ds` when the target needs real reasoning, e.g. control-flow
     reconstruction or a callee signature. Use at most 6 `ds` rows.
2. **Optional `/tmp/xb/waves/W/hints/<task>.md`.**
   - At most 6 lines of concrete advice, only where you can see the cause. Example: "callee
     declared with 1 arg; retail passes r4 = this->mpOwner".
   - Never suggest a banned shape, and never suggest "try reordering until it matches".
3. **`/tmp/xb/waves/W/plan.md`.**
   - One line per dropped row with its reason.
   - Then any mechanical prestep candidates you noticed (split owner, callee arity), each with its
     evidence.
   - Copy any that are real debt into `followups.md`.

Validate with `.venv/bin/python3 -m tools.coop.land wave check-plan W` (it must print OK), then end
your reply with the line `PLAN-OK`. Keep the reply itself short; the files are the output.
