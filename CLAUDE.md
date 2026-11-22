@AGENTS.md

# Orchestrator playbook (Claude)

You are usually the **orchestrator**: you pick work, launch cheap agents, review their edits, and
land everything through the land gate (`docs/land_gate.md`). Your own context is the expensive
part of the pipeline; agents are nearly free. Optimise for **few orchestrator turns per landed ✓**.

## Order of work (cheapest first)

1. **Mechanical fixes (no agents).** Run the censuses, verify each candidate against retail, and
   land it as a prestep script (`/tmp/xb/orch/presteps/<TASK>.sh` with `# TOUCHES:` / `# SYMS:`
   and an evidence comment):
   - `python tools/coop/census_fixes.py splits` — US vs JP split ownership. **Probable
     misassignments** need the same section, the same size and the same real symbol in both ranges
     (that is how LYTMATERIAL-SBSS2 was found: +3 ✓ and a forced-section dummy removed). It also
     lists forced-section padding arrays, which are usually a split-owner or data-reconstruction
     problem.
   - `python tools/coop/census_fixes.py sigs` — local re-declarations whose arity disagrees with
     the canonical header or with other TUs (W1-00 shape: a callee declared with too few
     parameters). Fixing one needs the real call-site arguments, so verify with hexdiff, or give it
     to an agent as a `callee-signature` task.
   - `python tools/coop/census_unit_rules.py [--write]` — UNIT_RULES entries and fields that are
     byte-identical no-ops. Land `--write` as a prestep, then run
     `tools/check_unit_rules_frozen.py --ratchet`.
2. **Low-touch agent waves** (below), hy4 only by default.
3. Opus only for hub/header design decisions; never for matching.

## Wave pipeline (`python -m tools.coop.land wave …`)

```bash
.venv/bin/python3 -m tools.coop.land wave census                # ~5 min, refresh after big landings
.venv/bin/python3 -m tools.coop.land wave select W2 --n 20      # structural targets, one per file
.venv/bin/python3 -m tools.coop.land wave prep W2               # packs + prompts + owned files + originals
#   launch each row: paseo create_agent, provider pi, model from the table, thinking high,
#   prompt = contents of the .prompt file, notifyOnFinish=false (no per-agent turns)
.venv/bin/python3 -m tools.coop.land wave wait W2               # run in background: ONE notification
.venv/bin/python3 -m tools.coop.land wave collect W2            # harvest + auto-screen -> one table
#   read only the 'review' patches in /tmp/xb/plans/<T>.patch; move rejects to plans/held/
.venv/bin/python3 -m tools.coop.land wave record W2 W2-03 hold "reason"   # history for later selection
.venv/bin/python3 -m tools.coop.land wave queue W2 W2-00 W2-05 --out /tmp/xb/orch/batchN.multi
XB_LAND_OWNER=1 .venv/bin/python3 -m tools.coop.land apply /tmp/xb/orch/batchN.multi --log /tmp/xb/orch/batchN.log
```

- **Selection** skips ✓, unpaired functions, functions larger than 0x400 and **register-allocation-only
  residuals** (0 structural diffs). In wave 1 those produced every `BLOCKED regalloc` and most of
  the steering attempts. Functions already tried at the same source sha are skipped too
  (`tools/coop/land/state/wave_history.jsonl`).
- **collect** restores every owned file to its ledgered original, writes the patch, and
  classifies it:
  - `drop` — no change or comment-only;
  - `hold` — any ban/faithful counter rose, so it goes to `plans/held/` automatically;
  - `review` — everything else, with hints (`new static inline`, `new (T*)& cast`,
    `new reinterpret_cast`, `new extern`).
  A `STRAY` row means gated files changed outside the owned sets; resolve it before landing.
- **Review the `review` patches yourself** before queueing them. The gate cannot see:
  - rewrites of SDK/middleware idioms (nw4r, CRI, BTE, MSL, RVL SDK);
  - reorders against a known original;
  - invented helpers;
  - "missing data" fixed with an extern. That is a split problem; fix the split instead.
- Models: `pi` + `opencode-go/hy4-flash-free` (free, no rate limit, steered least in wave 1). Use
  `opencode-go/deepseek-v4.1-flash` only to retry hy4 `BLOCKED` results that are not regalloc, at
  most 3 concurrent (it returns 429 errors beyond that). Never Codex.
- Workspace `wks_5ac8e57131b986e1`, settings `{"thinkingOptionId":"high"}`, titles ≤ 60 chars.
  Deny permission requests for paths outside the repo or malformed paths.

## Landing and checkpoints

- Only the orchestrator runs `check` / `apply` (`XB_LAND_OWNER=1`), and only on a quiet tree
  (`ledger.unledgered()` empty except the queued plans); the runner aborts otherwise.
- Checkpoint = `/tmp/xb/orch/cpNN.sh` checks (report, link/extab/text-order gates, registry, faithful
  ratchet, tests), a commit (the pre-commit hook verifies every staged gated file against the
  ledger), `git push origin <branch>`, then `git fetch . <branch>:main && git push origin main`.
  Push only to origin (Aelbannan/xenoblade); never upstream; never force-push. Agents never use git.
- Record follow-ups in `/tmp/xb/orch/followups.txt` and user questions in
  `/tmp/xb/orch/milestone_questions.txt`; report at milestones, not per agent.
