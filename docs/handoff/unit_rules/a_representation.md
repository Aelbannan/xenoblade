# Category A — Representation repair (comparator canonicalization)

**Hand to:** one agent, no source-matching work. **Goal:** delete every
`UNIT_RULES` field whose only effect is to reconcile the decomp `.o`
*representation* with the dtk/ppcdis retail split `.o`. Replace the object
rewrite with comparator logic. Zero source changes; the honest link's bytes
must be unchanged. Prerequisite reading: `docs/handoff/unit_rules/README.md`.

## Why this category exists

The `build/us/obj/**.o` "retail" objects are **reconstructions** produced by
dtk/ppcdis from the linked DOL, not the original retail objects. The splitter
guesses which words were relocations (from `config/<region>/symbols.txt` and
built-in heuristics), guesses section alignment, and cannot recover
linker-script-only symbols. The decomp `.o` from MWCC is the real compiler
output. So some object-level diffs are *splitter artifacts*, not source
problems — and the table rewrites the decomp object to imitate the artifact.

## Scope (field usage across the whole table)

| field | entries | what it pretends |
|---|---|---|
| `exact_renames` | 223 | symbol name drift (`@N`, vtables, statics) |
| `pool_patterns` | 201 | `@N` float/double pool → `lbl_eu_*` |
| `set_data_align` | 30 | splitter align=4 vs MWCC align=8 |
| `data_pool_patterns` | 27 | file-backed `@N` pool → retail name |
| `prefix_renames` | 13 | static-local numbering |
| `globalize_symbols` | 11 | LOCAL UNDEF → GLOBAL UNDEF for mwldeppc |
| `add_symbols` | 8 | splitter-invisible subobject labels |
| `bake_linker_addrs` | 3 | linker-script constants baked by the splitter |
| `symbol_sizes` | 2 | symbol st_size |
| `force_symbol_relocs` | 1 | splitter-reconstructed relocs for literals |

Plus the pure rename-class entries: **76 entries, 29 live at link**. Entries
that mix these fields with content/layout fields (`CProc.o`, `CView.o`,
`CWorkThread.o`, `CTaskManager.o`, …) stay with categories B/C until their
other fields are gone; the residue then falls to you. Do not start with mixed
entries.

### Worked case 1: `bake_linker_addrs` (`_stack_addr`)

`OSThread.o`, `__start.o`: `bake_linker_addrs=(("_stack_addr", 0x8067B560),)`;
`OS.o`: `__ArenaLo` (0x8067D560) and `_db_stack_end` (0x8067B560).

- `_stack_addr` is a **linker-script constant**, not a DOL symbol:
  `build/us/ldscript.lcf:27`: `_stack_addr = (_stack_end + 0xFFFF + 0x7) & ~0x7;`
  (= 0x8067B560 on US), with `_db_stack_addr = _stack_addr + 0x2000`,
  `_db_stack_end = _stack_addr`, `__ArenaLo = (_db_stack_addr + 0x1f) & ~0x1f`.
- Retail bytes at `.init+0x234`: `3C208067` (`lis r1,0x8067`) / `6021B560`
  (`ori r1,r1,0xb560`). The retail `.rela.init` has **no** reloc at
  `0x236`/`0x23a` (nearest: `_SDA2_BASE_` at `0x23e`/`0x242`).
- MWCC emits zeros + `R_PPC_ADDR16_HI/LO` there because `@h`/`@l` are
  relocatable operands. `tools/postprocess_reloc_names.py:def
  bake_linker_addrs` writes the immediates from the hard-coded address and
  drops the relocs.
- The honest link resolves the reloc via the LCF to the same value ⇒ the rule
  is DOL-neutral *while the layout matches retail*. It is region-blind
  (0x8067B560 is US) and it hides layout drift, so it must go either way.

**Required fix:** resolve ADDR16 `LO/HI/HA` relocs against LCF symbols once
(`_stack_addr`, `_stack_end`, `_db_stack_end`, `__ArenaLo`, `_SDA_BASE_`,
`_SDA2_BASE_`, `_f_sbss2`, …) and compare the resulting immediate with the
retail object's baked word. Equal ⇒ MATCH. Add a build-time assertion that the
honest link still resolves `_stack_addr` to 0x8067B560 so drift stays loud.

### Worked case 2: `force_symbol_relocs` (`OS.o` `OSInit`)

Rule: `(0x108, 0x3C808000, 0x3C800000, 6, "memcpy")` and
`(0x17C, 0x38644000, 0x38640000, 4, "memcpy")` — zeroes `lis r4,0x8000` /
`addi r4,r4,0x4000` and injects `memcpy@ha/@l`. If `config/us/symbols.txt`
places `memcpy` at 0x80004000, the honest link produces those same immediates
via the relocs ⇒ representation-only. Confirm against `symbols.txt` and the
build map, then move it here (delete from C's scope with a note).

### Worked case 3: `set_data_align`

ppcdis writes `sh_addralign=4` on sections MWCC emits with align 8 (identical
content, different header field). The table mutates the decomp section header.
`check_data_sections` compares alignment, so the comparator is what should
relax: alignment is a property of the *retail splitter*, not of the source.
Either (a) treat alignment as satisfied when bytes+size+relocs match, or
(b) fix the extractor to record the true alignment. Do not keep rewriting.

### Worked case 4: name families

`exact_renames` / `prefix_renames` / `pool_patterns` / `data_pool_patterns`:
`tools/coop/reloc_map.py diff <unit>` already classifies `name` / `addend` /
`layout` drift and mines the repo-wide map. The approved source path is
`extern "C" <type> lbl_eu_*;` (lint: `extern "C"` is only allowed for `lbl_*`
names) — but that is source work; here you only need the comparator to treat
two reloc targets as equal when they resolve to the same address/section
(same `(section, symbol address, addend)`, not the same spelling).

## Comparator work (the actual deliverable)

1. **One canonical resolver** used by gate and link, e.g.
   `tools/coop/reloc_canon.py` (or extend `tools/coop/reloc_map.py`):
   - parse `config/<region>/symbols.txt` and `build/<region>/ldscript.lcf`;
   - `canon_reloc(symbol, addend, type) -> (section, resolved_addr, addend, type_class)`;
   - for linker absolutes, resolve via LCF formulas.
2. **`tools/coop/lib/data_match.py`** (~L180-205, offset-keyed reloc sets):
   replace literal name equality with `canon_reloc` comparison; keep the
   existing "bytes identical ⇒ reloc order immaterial" rule.
3. **`tools/coop/lib/equivalence_check.py`**: `_normalize_reloc_dest` (~L254)
   and `_byte_identical_with_relocs` (~L272) are the witness-side hooks;
   extend for LINKER-ABSOLUTE ADDR16 pairs (resolve HA/HI/LO to the immediate
   the linker will write), and for canonical name equality.
4. **`tools/coop/lib/objdiff_report.py`**: objdiff-cli feeds the FULL_MATCH
   certification; check whether its reloc-site comparison needs the same
   canonicalization (see the objdiff #279 note in `reloc_map.py`).
5. **`tools/project.py:1257`** uses `built_obj_path.name in UNIT_RULES`
   (plain dict lookup) while `select_unit_rules()` handles `#<substring>`
   scoped keys. 5 scoped-only keys (`lyt_pane.o#…` ×2,
   `ut_TextWriterBase.o#…` ×2, `lyt_common.o#…`) therefore never reach the
   link although the checker applies them. Unify on one resolver. This is a
   correctness fix, not a rule change.

## Method per entry

1. `python3 tools/coop/hexdiff.py <unit> --all` with the entry enabled;
   record verdicts.
2. Comment out the entry's fields; rebuild via hexdiff; record verdicts.
3. **Stale** (verdict unchanged) ⇒ delete the entry immediately.
4. **Representation** (verdict worsens, resolved bytes provably identical) ⇒
   implement/extend the canonical comparison, re-run the raw gate, delete.
5. **Real byte difference** ⇒ do not patch; hand to B/C with the evidence
   (which bytes/words differ, retail vs honest).
6. Verify DOL-neutrality where a full link is reachable: build the unit both
   ways and compare the linked function bytes (`main.elf` slice) — or prove
   the equality at the resolution level as in the worked cases.

## Evidence / definition of done

- Per retired entry: key, deleted fields, retail vs decomp object facts
  (`readelf -S/-r`, objdump words), the resolution proof, affected units and
  accepted-target count. Append to `docs/evidence/decomp/attempts.jsonl` or
  `docs/evidence/decomp/unit_rules_category_a.md`.
- Comparator changes come with tests: a fixture pair for each canon rule
  (name drift, addend drift, ADDR16 linker absolute) plus a live
  `run.py data diff <unit>` showing MATCH on the raw object.
- `--claim "<key>" --owner <you> --class rename --reason "representation-only"`
  as you go; `--ratchet` after a reviewed batch.
- Done when: pure-rename entries deleted, all three `bake_linker_addrs`
  deleted, `force_symbol_relocs` resolved (deleted or handed to C),
  `set_data_align` resolved in extractor/comparator, scoped-key resolver
  unified, and the comparator tests green.

**Report back:** entries deleted/stale/handed-off, comparator files changed,
test names, per-entry proof summary, and the new `--report` numbers.
