# Xenoblade co-op fork — agent entry point

This repository is a **private/downstream** Xenoblade Chronicles Wii decompilation and split-screen co-op mod fork.

## Required reading order

1. **`.claude/skills/xenoblade-decomp/SKILL.md`** — Claude Code skill: decompilation workflow, coop runner, `FULL_MATCH` policy. **Follow this for all work in this repo unless the user explicitly asks for something else.**
1. **`.cursor/skills/xenoblade-decomp/SKILL.md`** — Cursor-equivalent of the same skill (used when editing in Cursor).
2. **`PLAN.md`** — co-op architecture, invariants, milestones, agent boundaries.
3. **`DECOMP_MAP.md`** — per-function targets, symbols, and tiers.
4. **`TASKS.md`** — agent checklist; check off functions at `FULL_MATCH`.
5. **`tools/test/compare_behaviour/README.md`** — static/size + optional PPC behaviour checks.
7. **`FORK.md`** — inventory of fork-only tools, PPC harness, docs, and policy vs upstream `xbret/xenoblade`.
8. **`tools/ppc_equivalence/README.md`** — SMT semantic-equivalence check, proof contracts, supported PPC subset, and result/exit-code policy.
9. **`tools/dol_opcodes/`** — opcode census for retail `main.dol` against PPC750CL `isa.yaml`.

## Quick commands

```bash
cp tools/coop/coop.example.json coop.json   # first time only
python tools/coop/run.py status
python tools/coop/run.py targets list
python tools/coop/run.py behaviour audit    # registered-test size budget
python tools/coop/run.py size <unit>        # decomp .text vs split budget
python tools/coop/run.py equivalence --help # supported straight-line PPC semantic proofs
python tools/coop/run.py opcodes            # unique opcodes in main.dol
```

## Do not

- Submit LLM-assisted matching work to upstream `xbret/xenoblade`.
- Commit `orig/`, `main.dol`, RELs, or disc assets.
- **Decompile to assembly or registers** — matched code in `src/**` and `libs/**` must be **high-level C or C++** (see `PLAN.md` **§17.6** for narrow intrinsics / single-instruction asm exceptions).
