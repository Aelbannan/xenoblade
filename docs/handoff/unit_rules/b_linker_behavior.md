# Category B — Linker-behavior emulation

**Hand to:** one or two agents (source work). **Goal:** resolve the 394
layout-class entries to one of three verdicts: *source fix* (delete the rule),
*genuine tooling limit* (owner-approved §17.6 exception, or comparator
tolerance handed to A), or *near-miss* (demote the targets, delete the rule).
Prerequisite reading: `docs/handoff/unit_rules/README.md`.

## Scope

- **`extern_data_sections`: 295 entries (36 standalone).** These are
  *architecture*, not per-unit bugs. Do **not** touch them until Category D
  records the data-ownership decision; annotate them with `--claim` and move
  on. D will then remove them in batches.
- **Non-EDS layout: 104 entries** (this is your real scope), plus the live
  layout list (77) from `--report`. Fields:

| field | meaning (from the table's own docstrings) |
|---|---|
| `drop_data_tail` / `drop_data_range` / `drop_nobits_range` / `zero_data_range` / `zero_nobits` | remove/zero data the retail linker GC'd or packed differently |
| `drop_text_symbols` / `drop_text_symbols_as_undef` | remove weak `.text` FUNCs retail never put in the split |
| `trim_text_size` / `pad_text_size` / `repack_after_drop` | change emitted `.text` size/alignment |
| `pad_data_section` / `pad_sdata2_size` / `trim_sdata2_size` | tail padding / orphan pool entries |
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
   `"policy_exception": true` log. Default answer: comparator tolerance
   (hand to A) or a recorded near-miss.
4. **Linker-absolute / reloc reconstruction.** If the only diff is reloc
   presence/name/addend or a baked linker constant, it belongs to Category A —
   hand it over with the evidence instead of "fixing" it here.

## Method per entry

1. **Reproduce**: `python3 tools/coop/hexdiff.py <unit> --all` (rebuild),
   then `python3 tools/coop/run.py data diff <unit>` (raw gate). Inspect both
   objects: `readelf -S`, `readelf -r`, `objdump -h/-dr`. Write down the exact
   differing symbols, section sizes, and relocs.
2. **Try the source lever** from the list above. Re-run `hexdiff --all` and
   the raw `data diff`. Code must not regress.
3. **If raw green** ⇒ delete the entry; keep the recipe; append evidence.
4. **If not fixable in source** ⇒ choose:
   - comparator tolerance (alignment/zero-pad/name-addend-only) ⇒ hand to A;
   - owner-approved §17.6 exception ⇒ add the entry to PLAN.md's table with
     requirements, log `"policy_exception": true`, keep the rule (it stays in
     the frozen manifest; annotate with `--claim`);
   - otherwise ⇒ record an open-item packet (`attempts.jsonl`), demote the
     affected targets in `targets.json`, delete the entry.
5. **Verify** the unit's remaining code targets are still accepted from raw
   objects; run `size` for the split budget.

## Must not

- No new `UNIT_RULES` keys/fields (frozen + lint). No weakening of the gate
  to make an entry pass; comparator changes go through A and must be
  universally justified.
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
