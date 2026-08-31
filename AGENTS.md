# Xenoblade decomp fork — agent entry point

Private/downstream Xenoblade Chronicles Wii decompilation fork (byte-matching; the former
split-screen co-op feature effort is parked).

## Lazy-load the skill

**Read `SKILL.md` (`.agents/skills/xenoblade-decomp/SKILL.md` — pi loads this path) only when this task is a
decomp/matching task** — decompiling, byte-matching, editing `src/**`/`libs/**`/`configure.py`, or
running the coop runner / objdiff / DECOMP_MAP targets / MWCC `EQUIVALENT_MATCH` / `FULL_MATCH` work.
It holds the workflow, acceptance policy, and residual routing.

**Skip it for non-decomp asks** (planning, docs, tooling, general repo questions). Your default
state should be "no SKILL.md loaded" — load it on demand, not up front.

## Every task

- **Identity/state:** `tools/coop/targets.json` is the sole source of function identity/state — use
  `targets show/status`, not hand-maintained checklists. `docs/ownership.csv` is legacy.
- **Architecture/legal rules:** `PLAN.md` §§2, 3, 17 (legal boundaries, invariants, matching policy).
  Read other sections only when the task touches that subsystem.

## Always

- Use **`.venv/bin/python3`**, never the system `python3` (system is 3.9 and fails on project syntax).
- **Prefer `hexdiff` over raw `ninja`** — `python3 tools/coop/hexdiff.py <unit> --symbol <sym>` runs
  the build itself and holds the repo-wide lock, so it's safe for concurrent agents. Only use
  `ninja`/`configure.py` when hexdiff can't express the operation.

## Never (full policy: `SKILL.md` / `PLAN.md` §17)

- **NEVER USE GIT** — do not run `git` (status, diff, add, commit, push, checkout, stash, rebase,
  reset, clean, or any other subcommand). Leave all version control to the human; agents only edit
  working-tree files.
- Submit LLM-assisted matching upstream; commit `orig/`, `main.dol`, RELs, or disc assets; or decompile to asm/registers outside the §17.6 PS backend.
- Run Dolphin inside the restricted process — its universal binary reports missing NEON, so launch it outside the restricted process.
- **Treat the repo root as sacred** — do not dump scratch files, downloads, binaries, zip archives,
  ad-hoc scripts, notes, or other tmp junk into `/` (e.g. `ninja`, `ninja.zip`, one-off logs).
  Use `/tmp`, a gitignored scratch dir, or delete when done. Leave the root layout unchanged unless
  the task explicitly requires a root-level project file.

## More commands

CI gates, code-smell report, and the pi-harness / coop-runner cheatsheets live in `SKILL.md` (Key
paths, Routing) and `docs/`.
