---
name: xenoblade-decomp
description: >-
  Primary agent workflow for the Xenoblade Chronicles Wii co-op decompilation
  fork. Always read and follow at the start of tasks in this repository when
  decompiling, matching, editing src/kyoshin or configure.py, running the coop
  runner, objdiff, DECOMP_MAP targets, or MWCC FULL_MATCH work.
---

# Xenoblade decompilation

> **Auto-loaded** via root `AGENTS.md` and `.cursor/rules/xenoblade-decomp.mdc` (`alwaysApply: true`).

## Before you edit code

1. Read `PLAN.md` §2 (legal boundaries), §3 (architecture invariants), and §17 (decompilation loop).
2. Read the target entry in `DECOMP_MAP.md` and `tools/coop/targets.json`.
3. Confirm this is a **private/downstream fork** — do not upstream LLM-assisted matching work to `xbret/xenoblade`.

**Current policy:** every target must reach **`FULL_MATCH`** (100% function match when a symbol is set; 100% code + data for whole units). `coop run cycle` exits non-zero until the bar is met.

## One-time setup

```bash
cp tools/coop/coop.example.json coop.json
# Place retail files at orig/<region>/sys/main.dol and orig/<region>/files/rels/*.rel

python tools/coop/run.py status
python tools/coop/run.py baseline   # sha1 + configure + ninja
```

Region defaults to `us` in `coop.json`. Change with `--config` or edit `region`.

## Pick a target

```bash
python tools/coop/run.py targets list
python tools/coop/run.py targets show <target-id>
```

Prefer `P0` / `P1` tiers unless the user names a specific function. Skip targets with `buildable=no` until source exists — recover via Ghidra first, add `.cpp` to `configure.py`, reconfigure.

Claim symbols in `docs/ownership.csv` before editing (see `PLAN.md` §6).

## Decompilation loop

For each target:

```text
export assembly/symbols/types (Ghidra or objdiff)
→ draft/edit C++ in the owning translation unit
→ python tools/coop/run.py ctx <source.cpp>
→ python tools/coop/run.py cycle <target-id> \
    --hypothesis "..." --next-change "..." --runtime-test ""
→ if FAIL: inspect objdiff / build/coop-function-diff.json, revise, repeat
→ if PASS: mark target complete; do not edit the same function concurrently
```

Manual steps equivalent to `cycle`:

```bash
python tools/coop/run.py ctx src/kyoshin/cf/CfPadTask.cpp
python tools/coop/run.py build kyoshin/cf/CfPadTask
python tools/coop/run.py diff kyoshin/cf/CfPadTask --symbol <mangled-symbol>
```

### decomp.me (optional)

For stubborn **small** functions: generate ctx → open unit in **objdiff** → Create scratch on decomp.me → paste matched code back → `cycle` again.

### Large functions

Decompose into leaf symbols/units first. Each leaf and the parent must still end at **`FULL_MATCH`** before the target is closed.

## Logging and evidence

- Attempts append to `docs/evidence/decomp/attempts.jsonl` (JSONL, one object per line).
- Required fields: `target_id`, `function`, `status`, `instruction_match`, `hypothesis`, `next_change`.
- After matching, add runtime proof in Dolphin when `PLAN.md` requires it (`BEHAVIOR_VERIFIED`).

## Marking configure.py matching

When a whole translation unit reaches full match, update the object in `configure.py`:

```python
Object(Matching, "kyoshin/cf/CfPadTask.cpp"),  # was NonMatching
```

Then reconfigure and verify `ninja` still succeeds.

## Common mismatch fixes (MWCC / C++)

- signed vs unsigned comparisons
- wrong struct field width or type
- branch/switch lowering shape
- register allocation / expression order
- missing `-O4,s` or per-unit `extra_cflags` from `configure.py`
- incorrect virtual or adjusted-this call
- relocation target wrong — ensure `functionRelocDiffs=data_value` in `coop.json`

## Do not

- Commit `orig/`, `main.dol`, RELs, or disc assets
- Call `CGame::wkRender` or full frame update twice for split-screen experiments
- Accept `STRUCTURAL` / `CODE_MATCH` as final state (project policy is `FULL_MATCH`)
- Submit AI-assisted reconstruction upstream

## Key paths

| Path | Role |
|------|------|
| `PLAN.md` | Co-op architecture, agent rules, milestones |
| `DECOMP_MAP.md` | Function-level decompilation map |
| `TASKS.md` | Agent checklist — check off at `FULL_MATCH` |
| `tools/coop/run.py` | Runner CLI |
| `tools/coop/targets.json` | Curated target queue |
| `configure.py` | Per-object matching flags and compiler options |
| `tools/decompctx.py` | Context for decomp.me / compilation |
| `objdiff.json` | Generated; objdiff project config |

## Additional detail

- Full agent orchestration, Dolphin gates, and render invariants: `PLAN.md`
- Per-function addresses and tiers: `DECOMP_MAP.md`
