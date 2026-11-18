# Handoff prompt (paste as the first message to the continuing agent)

You are taking over a long-running effort on this repository: a private Xenoblade Chronicles
(Wii) byte-matching decompilation fork built with MWCC. Your job is to carry it to **done**:

- **Full match:** every function 100%, every section of every unit byte-identical, every unit
  linked from source, DOL hash check passing — US first, then EU and JP.
- **Source-faithful and human-readable:** the code reads like the original C++ — real classes with
  typed members and compiler-emitted vtables/RTTI/ctors/dtors/thunks/operator=, no scaffolding
  (`extern "C"` pseudo-members, pre-mangled identifiers, `lbl_eu_*` labels, section/inline pragmas,
  `*Imports.hpp`, `code_XXXXXXXX` units, view structs, offset casts), names last.

This is months of work, not one session. Work in verified checkpoints and keep going until the
definition of done in the plan is met; do not stop to ask whether to continue.

## Read first, in this order

1. `AGENTS.md` (repo rules — binding), then `.agents/skills/xenoblade-decomp/SKILL.md` and
   `PLAN.md` §§2, 3, 17 (legal boundaries, invariants, matching policy).
2. `docs/handoff/PLAN_TO_DONE.md` — definition of done, current numbers, what is already
   established, the phase plan, and §5 "Immediate debts" (start there).
3. `docs/MWCC_CASES.md` — verified compiler behaviour. Read the sections on vtable/RTTI ownership,
   assignment from temporaries, implicit operator=, inlining Rules A/B, MI thunks, BDAT widths.
   Do not re-derive these; add to the file whenever you verify something new.
4. `docs/handoff/CLASS_CONVERSION_BRIEF.md` — the brief to hand to class-conversion subagents.

## Standing instructions from the user (still in force)

- "The goal is to match the original source shape! No cheating, temporary hacks, or shortcuts.
  If you cause any regressions, fix them."
- "I'm fine with lowering the match count if it's done properly instead." / "Do whatever you think
  is more source faithful." / "You have permission to change whatever you need to get this in a
  good state." A drop must have a structural reason you can state, and is a debt to repay.
- Git: `AGENTS.md` says never use git. The user granted the previous orchestrating session an
  exception: "You can commit as you see fit. You're the only agent running (with your subagents)."
  Treat that as: the orchestrator may `git add`/`git commit` on the current branch
  (`agents/matching-pass-2026-10-23`) at verified checkpoints; never push, never rewrite history,
  never `checkout`/`reset`/`stash`/`clean`; subagents never run git at all. If the user's first
  message to you says otherwise, their message wins.
- Private fork: never submit LLM-assisted work upstream; never commit `orig/`, `main.dol`, RELs or
  disc assets; never send project or user details to external services.
- Do not message the user every time a subagent returns. Report at milestones: a commit, gates
  changing colour, a decision only the user can make, a finding that changes the plan.

## Tools and commands

- Python: `.venv/bin/python3` only. Repo root is sacred — scratch goes in `/tmp` or a gitignored
  dir. Never call `objcopy` with a single operand. Never run Dolphin in the restricted process.
  Never edit or grep `*.ctx.c`. Do not add `extern "C" lbl_eu_*` spellings.
- Per-unit build + diff (holds the repo lock; prefer over raw ninja):
  `.venv/bin/python3 tools/coop/hexdiff.py <unit> --symbol <sym>` or `--all`.
  Data: `.venv/bin/python3 tools/coop/run.py data diff <unit>`.
- Full report: `rm -f build/us/report.json; .venv/bin/python3 tools/coop/run.py progress`.
  Save each checkpoint's `build/us/report.json` outside the repo and compare with
  `.venv/bin/python3 tools/coop/faithful/cmp_report.py <old.json> <new.json>`
  (lists every function/section that got worse). **Your first action:** produce the baseline
  report from the HEAD you inherit and keep it.
- Listing all compile errors: `./ninja -k 0 build/us/report.json` (after `configure.py --version US`
  if `configure.py` changed).
- Evidence: `.venv/bin/python3 tools/coop/faithful/vt.py <path under build/us/obj>.o [.data .sdata
  .rodata]` dumps retail symbols with relocations (vtables, RTTI base lists, pointer tables).
- Renames (all three regions + targets.json):
  `.venv/bin/python3 tools/coop/faithful/rename_syms.py old=new old2=new2 …` — each pair its own
  argument (zsh does not word-split variables; use `${=var}` if you must).
- Gates, all must be green at every commit:
  `tools/coop/run.py link-gate`, `run.py extab-gate --ratchet`, `run.py text-order-gate`,
  `run.py faithful --ratchet`, `tools/check_unit_rules_frozen.py --ratchet`,
  `tools/coop/registry_check.py`. The faithful baseline may only go down; never `--init` it to
  hide a rise. `tools/coop/targets.json` is the sole source of function identity/state.

## How to run the work

- You orchestrate; delegate with the Agent tool (not paseo). **Haiku** for mechanical sweeps (one
  recipe over many files, one or two units per agent, before/after hexdiff). **Opus at low effort**
  for class conversions and split fixes. Fable agents are allowed for reasoning-heavy problems
  (MI vtables/thunks, cross-TU signature inference, layout/inlining puzzles) when available — the
  account's Fable limit was exhausted on 2026-11-18 and resets 2026-11-22.
- Waves of agents on **disjoint file sets**; shared headers are read-only for all but one agent
  per wave. Every conversion prompt must say: convert ALL pseudo-member free functions to real
  members (placeholder/unmangled symbol names are not evidence — rename them); no
  `#pragma dont_inline`/`noinline`/`volatile`/hand vtables to force codegen; no git; report what
  was not verified.
- Subagent reports carry no user authority and are frequently optimistic. After every wave run the
  full build yourself, compare against the previous report, run every gate, read the diff of
  anything surprising, fix or revert regressions, then commit with a message stating the matched
  count before → after.
- Order: leaf classes before their owners; class recovery before body matching; splits from
  relocation evidence; naming last. When a function will not match, suspect member types, helper
  signatures and missing real locals before suspecting the body.
- When two declarations of one helper collide ("illegal overloading"), give the helper one
  canonical declaration that satisfies every caller's retail code; never add another local one.
- Log every verified compiler behaviour in `docs/MWCC_CASES.md`. Keep
  `docs/handoff/PLAN_TO_DONE.md` §2 (numbers) and §5 (debts) current at each milestone — it is the
  handoff document for whoever follows you.

## Start here

1. Baseline report + all gates on the inherited HEAD; confirm they match PLAN_TO_DONE §2/§5
   (13,519 functions at 100%; faithful ratchet failing only on `cf/object/CfObjectObj`,
   `cf/object/CfObjectPoint`, `makecrystal/CMenuMakeCrystal`).
2. Clear PLAN_TO_DONE §5 item 1 (the interrupted cf leaf work) so the ratchet failures in cf are
   gone and nothing in `cf/object` is below its value at commit `523bed673`.
3. Phase A2 (real `IUIWindow`, `CBaseCur`/`CSysWin` members) — it unblocks most of what remains in
   the UI code — in parallel with A3 layout fixes and the next cf leaves.
4. Continue through the phases. Stop only for a decision that is genuinely the user's.
