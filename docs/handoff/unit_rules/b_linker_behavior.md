# Category B — Linker-behavior emulation

**Hand to:** one or two agents (source work). **Goal:** resolve the 390
layout-class entries to one of three verdicts: *source fix* (delete the rule),
*genuine tooling limit* (owner-approved §17.6 exception), or *near-miss*
(demote the targets, delete the rule). Prerequisite reading:
`docs/handoff/unit_rules/README.md`.

**Progress (2026-09-28):** the first B pass is recorded in
`docs/evidence/decomp/unit_rules_category_b.md` — 16 retirements (7 bit-identical
no-ops, 9 stale source-pad/size repairs), 24 field retirements + 1 payload-value change, a per-entry verdict
for every remaining entry (manifest `--claim` reasons), baseline 429→413, scope
111→88 entries. The per-entry packets (with the evidence each receiving category
needs) are in `docs/evidence/decomp/unit_rules_category_b_packets.md`; the
routing table is in the README. Start from §5 (`set_data_align` is an
LCF/exception decision, not a source fix) and §7 (link state: 0 errors after
B's `extern`/rename fixes; the remaining SIGSEGV is the `drop_text_symbols*`
symbol rewrite, §7.4). B's remaining own work is per-unit matching: the 13
text-GC entries and the 12 missing-tail entries are *not* retireable by rule
edits.

**Status (2026-09-28) — what category A left you:** A is closed
(`docs/evidence/decomp/unit_rules_category_a.md`). Relevant outcomes:

* **26 `set_data_align` entries are yours** (source-side alignment), and A's §5
  verdict is that the comparator must **not** relax alignment: for every
  `retail=4 / decomp=8` entry the retail section start address is not
  8-aligned, so removing the rule moves the section in the honest link. See the
  field table below; evidence: `.scratch/unitrules_a/align_audit2.py`.
* The canonical reloc comparator exists (`tools/coop/lib/reloc_canon.py`,
  `data_match.canonical_reloc_key`); A's handoff items are done, so a
  representation-class finding is no longer "hand to A" — prove it with that
  comparator (opt-in `strict_relocs`) and either delete the rule or open a new
  decision with the evidence.
* `bake_linker_addrs` and `force_symbol_relocs` no longer exist in the table.
* The scoped-key routing bug is fixed, so `#<substring>` keys now reach the link
  (re-derive the live set with the README snippet).

## Scope

- **`extern_data_sections`: 295 entries (36 standalone, 52 live).** These are
  *architecture*, not per-unit bugs. Do **not** touch them until Category D
  records the data-ownership decision; annotate them with `--claim` and move
  on. D will then remove them in batches.
- **Non-EDS layout: 100 entries** (37 live) — this is your real scope, and it
  **includes the 26 `set_data_align` entries** handed over by category A (§5 of
  its evidence doc).
- (Bookkeeping: the derived layout class is 390 = 290 EDS-only layout entries +
  these 100; the other 5 of the 295 EDS entries also carry content fields and
  are classed content, so they stay with C.)

Fields:

| field | meaning (from the table's own docstrings) |
|---|---|
| `drop_data_tail` / `drop_data_range` / `drop_nobits_range` / `zero_data_range` / `zero_nobits` | remove/zero data the retail linker GC'd or packed differently |
| `drop_text_symbols` / `drop_text_symbols_as_undef` | remove weak `.text` FUNCs retail never put in the split |
| `trim_text_size` / `pad_text_size` / `repack_after_drop` | change emitted `.text` size/alignment |
| `pad_data_section` / `pad_sdata2_size` / `trim_sdata2_size` | tail padding / orphan pool entries |
| `set_data_align` | section alignment the source shape decides (A §5: **not** comparator tolerance; 26 entries, evidence `align_audit2.py`) |
| `retarget_relocs` / `retarget_relocs_local` | point a reloc at the retail-correct symbol |

## Root causes and the honest levers

1. **Weak-symbol emission vs retail linker GC.**
   MWCC emits weak inline-virtual stubs, `__RTTI__*`, base-list blobs, and
   template instantiations that the original linker garbage-collected or
   placed in another TU. Source levers:
   - `#pragma auto_inline off` + explicit instantiation + `#pragma pop`
     for template members;
   - move empty virtual bodies out of headers / out-of-line the dtors;
   - `__declspec(novtable)`, `-RTTI` per `Object()` flags (check
     `configure.py` for existing usage);
   - avoid `reslist<...>`-style member templates in headers.
   Record each working recipe in `docs/MWCC_PATTERNS.md`.
2. **Extra/missing data.** If the source defines data the unit does not own,
   make it an `extern "C"` declaration (this is also D's model); if the source
   is missing data or has the wrong type/width/order, fix the definition.
   `data diff` per section tells you exactly which.
3. **Section layout the linker decides.** Zero padding after the last object,
   `.sbss` 8-vs-4 alignment, pool tail words, weak-GC ordering — MWCC +
   mwldeppc cannot reproduce the original linker's byte placement. These are
   the only legitimate §17.6 candidates, and only with owner approval + a
   `"policy_exception": true` log. Default answer: a recorded near-miss.
   **Exception:** `set_data_align` is *not* in this class — the retail start
   addresses prove the retail input alignment was 4, so fix the source
   declaration (A §5) instead of relaxing the comparator.
4. **Linker-absolute / reloc reconstruction.** If the only diff is reloc
   presence/name/addend or a baked linker constant, the canonical comparator
   (`tools/coop/lib/reloc_canon.py` + `data_match.canonical_reloc_key`,
   opt-in `strict_relocs`) either proves it neutral (delete the rule) or shows
   real drift. There is no category-A queue any more — record the proof in the
   evidence doc and decide here.

## Method per entry

1. **Reproduce**: `python3 tools/coop/hexdiff.py <unit> --all` (rebuild),
   then `python3 tools/coop/run.py data diff <unit>` (raw gate). Inspect both
   objects: `readelf -S`, `readelf -r`, `objdump -h/-dr`. Write down the exact
   differing symbols, section sizes, and relocs.
2. **Try the source lever** from the list above. Re-run `hexdiff --all` and
   the raw `data diff`. Code must not regress.
3. **If raw green** ⇒ delete the entry; keep the recipe; append evidence.
4. **If not fixable in source** ⇒ choose:
   - prove it representation-class with the canonical comparator (see root
     cause 4) ⇒ delete the rule with the proof in the evidence doc;
   - owner-approved §17.6 exception ⇒ add the entry to PLAN.md's table with
     requirements, log `"policy_exception": true`, keep the rule (it stays in
     the frozen manifest; annotate with `--claim`);
   - otherwise ⇒ record an open-item packet (`attempts.jsonl`), demote the
     affected targets in `targets.json`, delete the entry.
5. **Verify** the unit's remaining code targets are still accepted from raw
   objects; run `size` for the split budget.

## Must not

- No new `UNIT_RULES` keys/fields (frozen + lint). No weakening of the gate
  to make an entry pass; the comparator landed by category A must stay
  universally justified — do not relax it per unit.
- No `register`, fake stack buffers, inline asm, `.s` units, or transcribed
  retail asm. `#if 0`, `__declspec(section)`, volatile fake stacks are
  lint-forbidden.
- Do not delete `extern_data_sections` from a unit whose source still defines
  the data (duplicate symbols at link). That is D's batch job.

## Evidence / definition of done

- Per entry: verdict, root cause, minimal reproduction, exact source change
  (or exception/near-miss text), affected targets.
- Recipes → `docs/MWCC_PATTERNS.md`; per-target → `docs/MWCC_CASES.md`;
  history → `docs/evidence/decomp/attempts.jsonl`.
- `--claim "<key>" --owner <you>`; `--ratchet` after a reviewed batch.
- Done when every non-EDS layout entry has a verdict, all source-fixable
  entries are deleted, exceptions are in PLAN.md + logged, demotions are
  recorded, and the affected targets have been recertified from raw objects.

**Report back:** entries resolved per verdict, recipes added, exceptions
requested, demotions, and the new `--report` numbers.
