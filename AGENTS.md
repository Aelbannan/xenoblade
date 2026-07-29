# Xenoblade co-op fork — agent entry point

This repository is a **private/downstream** Xenoblade Chronicles Wii decompilation and split-screen co-op mod fork.

## Required reading order

1. **`.codex/skills/xenoblade-decomp/SKILL.md`** (also `.cursor/skills/` and `.claude/skills/`) — operational workflow and acceptance policy. Follow it unless the user explicitly asks for something else.
2. **`tools/coop/targets.json`** — sole source of truth for function identity and current target state; use `targets show/status`, not hand-maintained checklists.
3. **`PLAN.md` §§2, 3, 17** — legal boundaries, architecture invariants, and matching policy. Read other sections only when the task touches that subsystem.
4. **`COOP_IMPLEMENTATION_MAP.md`** — capability graph and feature handoffs; read for co-op architecture work.
5. **`docs/MWCC_REFERENCE.md`** — consult relevant patterns for matching work.
6. Read the behaviour, equivalence, symbol-recovery, or opcode documentation only when invoking that subsystem.

## Quick commands

**Always use `.venv/bin/python3`**, not the system `python3`. The venv has Python 3.13.6;
the system Python (3.9.6) will fail on modern syntax used in project tools.

```bash
cp tools/coop/coop.example.json coop.json   # first time only
.venv/bin/python3 tools/coop/run.py status
.venv/bin/python3 tools/coop/run.py targets validate
.venv/bin/python3 tools/coop/run.py targets status
.venv/bin/python3 tools/coop/run.py targets show <target-id>
.venv/bin/python3 tools/coop/run.py targets brief <target-id>
.venv/bin/python3 tools/coop/run.py targets sync-calls
.venv/bin/python3 tools/coop/run.py targets recertify --bottom-up --dry-run
.venv/bin/python3 tools/coop/run.py harness --selection ready --include-catalog --dry-run
.venv/bin/python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."
.venv/bin/python3 tools/coop/batch-cycle.py <target-id> [<target-id> ...] \
    --hypothesis-map map.json            # mass-cycle after matching
.venv/bin/python3 tools/coop/batch-cycle.py <target-id> \
    --default-hypothesis "..." --default-next-change "..." \
    --summary sum.json                   # with structured report
.venv/bin/python3 tools/coop/run.py atlas index --vectors   # Decomp Atlas catalog
.venv/bin/python3 tools/coop/run.py atlas serve             # http://127.0.0.1:8765

# pi-harness: TU-level batch matching with pi SDK agents (tools/pi_harness)
cd tools/pi_harness && npm install                # first time only
cp tools/pi_harness/pi-harness.example.json pi-harness.json   # repo root; set models
npm --prefix tools/pi_harness run pi-harness -- --tu kyoshin/CGame --dry-run
npm --prefix tools/pi_harness run pi-harness -- --tu kyoshin/CGame [--tu other/Unit]
```

## CI commands

```bash
# Fast PR gate (everything except Dolphin)
python tools/ppc_equivalence/gen_fixture_blob.py --check
python -m unittest discover -s tools/ppc_equivalence/tests -p "test_*.py"
python -m tools.ppc_equivalence differential
```

```bash
# Documentation sync
python -m tools.ppc_equivalence.docs_sync --write
python -m tools.ppc_equivalence.docs_sync --check
```

## Do not

- Submit LLM-assisted matching work to upstream `xbret/xenoblade`.
- **Prefer `hexdiff` over raw `ninja`** — use `python3 tools/coop/hexdiff.py <unit> --symbol <sym>` for build+diff feedback. hexdiff performs the build itself and holds the repo-wide build lock (`build/<region>/.hexdiff.lock`), making it safe for concurrent agents. Only run `ninja`/`configure.py` directly when hexdiff cannot express the operation (e.g. full-tree rebuild after reconfiguration).
- Commit `orig/`, `main.dol`, RELs, or disc assets.
- **Decompile to assembly or registers** — matched code in `src/**` and `libs/**` must be **high-level C or C++**. **Inline asm of any kind (`asm { }`, `asm void`) is not allowed.** See `PLAN.md` **§17.6** for narrow intrinsics exceptions only.
- Run Dolphin inside the restricted process — its universal binary reports missing NEON, so it must be launched outside the restricted process.
