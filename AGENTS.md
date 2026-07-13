# Xenoblade co-op fork — agent entry point

This repository is a **private/downstream** Xenoblade Chronicles Wii decompilation and split-screen co-op mod fork.

## Required reading order

1. **`.cursor/skills/xenoblade-decomp/SKILL.md`** — decompilation workflow, coop runner, `FULL_MATCH` policy. **Follow this for all work in this repo unless the user explicitly asks for something else.**
2. **`PLAN.md`** — co-op architecture, invariants, milestones, agent boundaries.
3. **`DECOMP_MAP.md`** — per-function targets, symbols, and tiers.
4. **`TASKS.md`** — agent checklist; check off functions at `FULL_MATCH`.
5. **`tools/test/compare_behaviour/README.md`** — retail vs decomp behaviour tests (**mandatory below 100% static match**).

## Quick commands

```bash
cp tools/coop/coop.example.json coop.json   # first time only
python tools/coop/run.py status
python tools/coop/run.py targets list
python tools/coop/run.py behaviour audit    # non-FULL_MATCH host coverage
python tools/coop/run.py size <unit>        # decomp .text vs split budget
```

## Do not

- Submit LLM-assisted matching work to upstream `xbret/xenoblade`.
- Commit `orig/`, `main.dol`, RELs, or disc assets.
- **Decompile to assembly or registers** — matched code in `src/**` and `libs/**` must be **high-level C or C++** (see `PLAN.md` **§17.6** for narrow intrinsics / single-instruction asm exceptions).
