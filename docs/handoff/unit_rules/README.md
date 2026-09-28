# UNIT_RULES burn-down — shared operating manual

This directory contains four agent handoff prompts for retiring the link-time
object-rewrite table (`tools/postprocess_reloc_names.py`, `UNIT_RULES`, 429
entries). Read this file before the category prompt you are executing.

**Status (2026-09-28):** category A is **closed** —
`docs/evidence/decomp/unit_rules_category_a.md` is its evidence doc, and its §8
residual table is the authoritative list of what is left (source, B/C, D).
Categories B/C/D are live; their prompts carry a status note where A's outcome
changed them.

**Category B progress (2026-09-28):** `docs/evidence/decomp/unit_rules_category_b.md`
records 16 retirements (7 provably bit-identical no-ops + 9 stale source-pad/
size repairs; 7 units moved raw-gate MISMATCH→MATCH), 25 payload-field
retirements, and a **per-entry verdict for every remaining in-scope entry**
(manifest `--claim` reasons + `docs/evidence/decomp/unit_rules_category_b_packets.md`).
Baseline is now **413** (`--ratchet` 429→422→414→413); the B scope is **88
entries / 27 live** (was 111/40) and no in-scope key is unowned. B1 also landed
its first source fix (`CArcItem.o`: the `drop_text_symbols` rewrite retired,
with the reusable recipe and its negative result in B doc §7.5).

**Link state after B's fixes:** the two duplicate-definition errors are **gone**
(§A2/§A3 below) — the full `main.elf` link now reports **0 link errors** and
still SIGSEGVs, entirely because of the `drop_text_symbols*` symbol rewrite
(B §7.4: ABS → SIGSEGV, UNDEF → SIGSEGV, keep-defined → `ELF_linker.c:7182`
internal error). B also fixed a real tool bug that was crashing the link with a
two-object reproducer (`trim_text_section` rewrote trimmed symbols to
`SHN_ABS`; §7.3). **§7.7 update:** the crash set is down from 11 to **9
objects** — `CArcItem` (§7.5, `IWorkEvent` out-of-line dtor) and `lyt_drawInfo`
hbm + nw4r (deleting the bogus inline-empty `~Rect`, object now `.text` 0xC0 =
retail with the drop rule retired). Recommended next step for whoever takes the
link: remove the need for the remaining rewrites source-side where possible
(§7.7 records which are genuine retail-linker-GC orphans: `FrameController`,
`AnimTransform`, `Content`/`ut::Color` and the `LinkList` template wrappers) — the
per-entry packets name each symbol, whether any link input
defines it, and the `.text` delta.

**Handoff routing from category B** (details in the prompts):

| handed to | what | where |
|---|---|---|
| you (owner) | 22 `set_data_align` LCF/exception decision; `PLAN.md` §17.6 rows | B doc §5 |
| category C | 15 missing-tail `pad_*` entries (retail bytes are zeros; per-entry retail symbol named) + 32 code-match packets | `c_byte_falsification.md` "Handed over from category B" |
| category D | 15 HANDOFF-D entries, 8 mixed, `CfRes`/`CDeviceVI`/`ut_RomFont` data-layout items, the 7 EDS-bearing `set_data_align` keys | `d_data_architecture.md` "Handed over from category B" |
| toolchain/pipeline | §7.4 `drop_text_symbols*`/mwldeppc blocker; wine-vs-wibo probe | B doc §7.4 |
| done | `CLibCriMoviePlay` `extern` + `symbols.txt`/`splits.txt` pad fix; `CHelp_ClosePartyMenu` symbol rename; `trim_text_section` SHN_ABS crash; `CArcItem` weak-stub source fix (+ its negative result for compiler-generated dtors) | B doc §7.1–7.3, §7.5 |


| prompt | category | scope |
|---|---|---|
| `a_representation.md` | representation repair | name/reloc/alignment/linker-constant diffs vs the splitter; comparator canonicalization |
| `b_linker_behavior.md` | linker-behavior emulation | 100 non-EDS layout entries (drops/pads/trims/GC + 26 `set_data_align`); source levers or approved near-miss |
| `c_byte_falsification.md` | byte falsification | 28 content entries (9 live); real source shape or demotion |
| `d_data_architecture.md` | data ownership | 295 `extern_data_sections` entries; owner decision + explicit code-only model |

## The problem in one paragraph

`tools/project.py` routes every source-built object whose basename is in
`UNIT_RULES` through `link_reloc_postprocess` (`cp Foo.o Foo.reloc.o && python
tools/postprocess_reloc_names.py Foo.reloc.o`) and links the rewritten copy
into `main.elf`. `PLAN.md` §17.6 says object post-processing is **not
approved**, `tools/coop/reloc_map.py` calls it deprecated, and no rule in the
table has an owner-approved exception recorded (the two `policy_exception`
flags in `attempts.jsonl` are per-function instruction-shape exemptions, not
rule exceptions) — yet **98 objects (103 keys, scoped twins included) are
rewritten in the current link**, and of the 14,395 accepted targets 55.8% live
in units that still carry a key (10.0% in units whose rule the current link
applies).
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
  (`lyt_pane.o#Q36nw4hbm3lyt`). The old bug (a plain dict lookup in
  `tools/project.py`, so scoped-only keys never reached the link while the
  checker applied them) is **fixed by category A**: `unit_has_rules()` is now
  the one key-name resolver shared by `tools/project.py` and the script's CLI
  gate, and `select_unit_rules()` still does the symbol-aware runtime
  selection. One linked unit changed (`ut_TextWriterBase.o#Q36nw4hbm2ut`);
  scoped keys now reach the link.

## Current census (derived 2026-09-28; `--report` shows the annotated classes)

| class | entries | live keys | notes |
|---|---|---|---|
| content | 28 | 9 | literal bytes/relocs/addends/pools (category C) |
| layout | 390 | 88 | 295 `extern_data_sections` (52 live, category D), 100 other drops/pads/trims (37 live, category B) |
| rename | 11 | 6 | names/labels only (category A's residue: source handoffs) |
| other | 0 | 0 | triage needed |

(103 live keys over 98 rewritten objects: scoped `#` twins share a basename. A
key is "live" when the current `main.elf` link has a `RELOCPOST` copy for its
basename. `--report` class counts come from `--claim` metadata, not the derived
triage class — re-derive both with:

```bash
.venv/bin/python3 - <<'EOF'
import dataclasses, re, sys; sys.path.insert(0, ".")
from pathlib import Path
from collections import Counter
from tools.postprocess_reloc_names import UNIT_RULES
import tools.check_unit_rules_frozen as C
m = re.search(r"^build build/us/main\.elf: link (.*?)(?=^build |\Z)",
              Path("build.ninja").read_text(), re.M | re.S)
bases = {Path(p).name.replace(".reloc.o", ".o")
         for p in re.findall(r"build/[\w./+-]+\.o", m.group(1)) if p.endswith(".reloc.o")}
c = Counter()
for k, r in UNIT_RULES.items():
    c[(C.rule_class(r), k.split("#")[0] in bases)] += 1
for cls in ("content", "layout", "rename", "other"):
    print(f"{cls:8s} entries={c[(cls, True)] + c[(cls, False)]:4d} live={c[(cls, True)]}")
EOF
```

Live content units: `CProc.o`, `CTaskManager.o`, `mtx.o`, `AXFXChorusExp.o`,
`AXFXChorusExpDpl2.o`, `AXFXReverbHiExp.o`, `OSNet.o`, `rfc_mx_fsm.o`,
`rfc_port_fsm.o`. (`OS.o`, `OSThread.o`, `__start.o` were retired by category A
— their `bake_linker_addrs`/`force_symbol_relocs` entries are gone.)

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
