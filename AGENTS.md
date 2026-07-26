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

```bash
cp tools/coop/coop.example.json coop.json   # first time only
python3 tools/coop/run.py status
python3 tools/coop/run.py targets validate
python3 tools/coop/run.py targets status
python3 tools/coop/run.py targets show <target-id>
python3 tools/coop/run.py targets brief <target-id>
python3 tools/coop/run.py targets sync-calls
python3 tools/coop/run.py targets recertify --bottom-up --dry-run
python3 tools/coop/run.py harness --selection ready --include-catalog --dry-run
python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."
python3 tools/coop/batch-cycle.py <target-id> [<target-id> ...] \
    --hypothesis-map map.json            # mass-cycle after matching
python3 tools/coop/batch-cycle.py <target-id> \
    --default-hypothesis "..." --default-next-change "..." \
    --summary sum.json                   # with structured report
python3 tools/llm_decomp/run.py solve <target-id> --dry-run   # conversational match session
python3 tools/llm_decomp/run.py solve <target-id>
python3 tools/llm_decomp/run.py pipeline --tu kyoshin/CGame   # whole-TU pipeline
python3 tools/llm_decomp/run.py pipeline --number 4 --dry-run # auto-select TUs
python3 tools/llm_decomp/run.py reconcile                     # restore after crashes
python3 tools/llm_decomp/run.py show-config                   # effective llm-decomp.json
python3 tools/coop/run.py atlas index --vectors   # Decomp Atlas catalog
python3 tools/coop/run.py atlas serve             # http://127.0.0.1:8765

# pi-harness: TU-level batch matching with pi SDK agents (tools/pi_harness)
cd tools/pi_harness && npm install                # first time only
cp tools/pi_harness/pi-harness.example.json pi-harness.json   # repo root; set models
npm --prefix tools/pi_harness run pi-harness -- --tu kyoshin/CGame --dry-run
npm --prefix tools/pi_harness run pi-harness -- --tu kyoshin/CGame [--tu other/Unit]
```

## LLM decompilation (tools/llm_decomp)

Conversational agent harness (replaces the retired `tools/llm_harness`
single-shot solve loop — do not use its commands). One session = one
target: the model gets a markdown brief (retail ASM, locked signature,
writable scope) and tools (read_file, grep, patch, build, diff,
equivalence, submit); the harness owns file writes, verification, and
acceptance (FULL_MATCH / proven EQUIVALENT_MATCH + zero regressions +
split-size). Config in `llm-decomp.json` (models/budgets per session type;
see docs/llm_decomp_design.md §9). Session types: match, type-recovery,
rename, tu-cleanup, size-trim. Transcripts at
build/llm-decomp/sessions/<target>/<session>/conversation.jsonl. Run
`reconcile` after any crash to restore files and abort orphaned sessions.
Full design: docs/llm_decomp_design.md.

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
