# UNIT_RULES burn-down — shared operating manual

This directory contains four agent handoff prompts for retiring the link-time
object-rewrite table (`tools/postprocess_reloc_names.py`, `UNIT_RULES`, 515
entries). Read this file before the category prompt you are executing.

| prompt | category | scope |
|---|---|---|
| `a_representation.md` | representation repair | name/reloc/alignment/linker-constant diffs vs the splitter; comparator canonicalization |
| `b_linker_behavior.md` | linker-behavior emulation | 104 non-EDS layout entries (drops/pads/trims/GC); source levers or approved near-miss |
| `c_byte_falsification.md` | byte falsification | 31 content entries (10 live); real source shape or demotion |
| `d_data_architecture.md` | data ownership | 295 `extern_data_sections` entries; owner decision + explicit code-only model |

## The problem in one paragraph

`tools/project.py` routes every source-built object whose basename is in
`UNIT_RULES` through `link_reloc_postprocess` (`cp Foo.o Foo.reloc.o && python
tools/postprocess_reloc_names.py Foo.reloc.o`) and links the rewritten copy
into `main.elf`. `PLAN.md` §17.6 says object post-processing is **not
approved**, `tools/coop/reloc_map.py` calls it deprecated, and `0` of 32,891
`attempts.jsonl` lines carry a `policy_exception` log — yet 119 objects are
rewritten in the current link and ~69.5% of accepted targets live in rule TUs.
The table grew because the gate let it: until this week `run.py data diff`
re-applied the same rules to a temp copy on a raw failure, and unit promotion
depends on that gate. Both are now fixed (raw comparison is the default; the
linked-bytes proof fallback fails closed for rule units), so the table can only
shrink from here.

## Table freeze (do not break this)

* `tools/postprocess_reloc_names.frozen.json` pins every entry's payload.
* `python3 tools/check_unit_rules_frozen.py --check` (CI:
  `.github/workflows/unit-rules.yml`) fails on **new keys**, **new fields** and
  **changed field values**. Removing keys/fields is allowed and is the goal.
* `tools/pi_harness/lint.py` flags added `"Foo.o": UnitRules(` keys.
* **Never add a rule.** Fix source or accept a near-miss; those are the only
  two honest outcomes.
* Annotate the entries you own: `tools/check_unit_rules_frozen.py --claim
  "<key>" --owner <you> --class content|layout|rename|other --reason "..."`.
* After a reviewed batch of removals: `--ratchet` updates `baseline_count`
  and prunes retired fields. Run it in small batches; it prints the payload
  changes it records. One coordinator per ratchet.
* The table keys may carry a `#<symbol-substring>` scope
  (`lyt_pane.o#Q36nw4hbm3lyt`). Known bug: `tools/project.py` uses a plain
  dict lookup, so scoped-only keys never reach the link while the checker's
  `select_unit_rules()` does apply them. Category A owns fixing that with a
  single shared resolver.

## Current census (from `--report` + `build.ninja`)

| class | entries | live keys | notes |
|---|---|---|---|
| content | 31 | 10 | literal bytes/relocs/addends/pools |
| layout | 394 | 77 | 295 `extern_data_sections`, 104 other drops/pads/trims |
| rename | 76 | 29 | names/labels only |
| other | 14 | 5 | triage needed |

(121 live keys over 119 objects: scoped `#` twins share a basename.)

Live content units: `OS.o`, `CProc.o`, `CTaskManager.o`, `mtx.o`,
`AXFXChorusExp.o`, `AXFXChorusExpDpl2.o`, `AXFXReverbHiExp.o`, `OSNet.o`,
`OSThread.o`, `__start.o`.

## Hard rules (repo-wide)

* **Never run `git`.** Leave version control to the human.
* Use `.venv/bin/python3` (system python is 3.9 and fails on project syntax).
* Builds: prefer `python3 tools/coop/hexdiff.py <unit> --all` / `--symbol`;
  it owns `build/<region>/.hexdiff.lock` and is safe for concurrent agents.
  Raw `ninja`/`configure.py` only when hexdiff cannot express the operation.
* No `register rN`, fake `sp[]`, inline `asm {}`, `.s` units, or transcribed
  retail asm outside the PLAN.md §17.6 exceptions. Matched code stays
  high-level C/C++. `libs/PowerPC_EABI_Support` SDK asm is pre-existing.
* Scratch work goes in `.scratch/` (gitignored), never the repo root.
* Never submit to `xbret/xenoblade`; this is a private fork.

## Tool cheat-sheet

```bash
python3 tools/coop/hexdiff.py <unit> --all            # per-function code table
python3 tools/coop/hexdiff.py <unit> --symbol <mangled> [--asm] [--relocs]
python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."
python3 tools/coop/run.py data diff <unit>             # RAW gate (default)
python3 tools/coop/run.py data diff <unit> --postprocess  # diagnostic only
python3 tools/check_unit_rules_frozen.py --check|--report|--claim|--ratchet
python3 tools/mwcc_kb.py search "<symbol or mismatch terms>" --kind reference
python3 tools/coop/reloc_map.py diff <unit>            # reloc-name drift miner
```

## Evidence protocol

* Live state lives in `tools/coop/targets.json` (`targets show/status`).
* Every acceptance change: `run.py cycle` (witness runs inside; never
  `--smt`/`--linked`/plain `run.py diff` in this fork).
* Append to `docs/evidence/decomp/attempts.jsonl`:
  `target_id, function, status, instruction_match, hypothesis, next_change`
  (plus `runtime_test` and, for any §17.6 use,
  `"policy_exception": true` with a one-line justification).
* Reusable recipes → `docs/MWCC_PATTERNS.md`; per-target records →
  `docs/MWCC_CASES.md` (formats at the top of those files).

## Definition of done for one rule

1. The rule is proven unnecessary (raw gate passes / linked bytes unchanged)
   or impossible, with a minimal reproduction.
2. The entry is deleted from `UNIT_RULES` (or converted to a documented
   §17.6 exception plus owner approval — coordinate first).
3. Affected targets are recertified from raw objects; demotions recorded.
4. Evidence appended; manifest metadata updated; `--ratchet` after review.

Report back with: entries touched, verdict per entry (deleted / exception /
demoted), raw-vs-retail evidence, DOL-neutrality evidence where relevant,
and the new `--report` numbers.
