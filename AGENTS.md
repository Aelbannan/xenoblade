# Xenoblade co-op fork — agent entry point

This repository is a **private/downstream** Xenoblade Chronicles Wii decompilation and split-screen co-op mod fork.

## Required reading order

1. **`.claude/skills/xenoblade-decomp/SKILL.md`** — operational workflow and acceptance policy. Follow it unless the user explicitly asks for something else.
2. **`tools/coop/targets.json`** — sole source of truth for function identity and current target state; use `targets show/status`, not hand-maintained checklists.
3. **`PLAN.md` §§2, 3, 17** — legal boundaries, architecture invariants, and matching policy. Read other sections only when the task touches that subsystem.
4. **`COOP_IMPLEMENTATION_MAP.md`** — capability graph and feature handoffs; read for co-op architecture work.
5. **`docs/MWCC_REFERENCE.md`** — consult relevant patterns for matching work.
6. Read the behaviour, equivalence, symbol-recovery, or opcode documentation only when invoking that subsystem.

## Quick commands

```bash
cp tools/coop/coop.example.json coop.json   # first time only
python3 tools/coop/run.py status
python3 tools/coop/run.py targets validate
python3 tools/coop/run.py targets status
python3 tools/coop/run.py targets show <target-id>
python3 tools/coop/run.py targets brief <target-id>
python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."
```

## Do not

- Submit LLM-assisted matching work to upstream `xbret/xenoblade`.
- Commit `orig/`, `main.dol`, RELs, or disc assets.
- **Decompile to assembly or registers** — matched code in `src/**` and `libs/**` must be **high-level C or C++** (see `PLAN.md` **§17.6** for narrow intrinsics / single-instruction asm exceptions).
- Run Dolphin inside the restricted process — its universal binary reports missing NEON, so it must be launched outside the restricted process.
