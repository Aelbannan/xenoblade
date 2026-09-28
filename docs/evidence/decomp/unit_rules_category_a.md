# Category A — representation repair (UNIT_RULES burn-down) — 2026-09-28

Prompt: `docs/handoff/unit_rules/a_representation.md`. Owner tag: `category-a`.
Result: **78 entries retired** (77 by this batch + `code_80135Fdc.o` from another
category), 3 `bake_linker_addrs` gone, 1 `force_symbol_relocs` gone, 60
rename-class entries gone, comparator canonicalization landed with tests.

Follow-up (same day): the 9 load-bearing `globalize_symbols` entries are also
gone — a generic end-of-pipeline promotion in `postprocess_object` replaces them
(§6.7), so the table now carries no `globalize_symbols` payload at all.

Predecessor: `docs/handoff/unit_rules/README.md` (freeze rules, evidence protocol).

## 1. Result summary

| metric | before | after |
|---|---|---|
| `UNIT_RULES` entries | 512 | 431 |
| `check_unit_rules_frozen.py --report` baseline | 512 | 431 (ratcheted; 1 re-add recorded, §7) |
| auto-derived class `rename` | 75 | 12 |
| auto-derived class `content` / `layout` / `other` | 31 / 392 / 14 | 28 / 391 / 0 |
| `bake_linker_addrs` entries | 3 | 0 |
| `force_symbol_relocs` entries | 1 | 0 |
| `symbol_sizes` entries | 2 | 1 (mixed `snd_SoundPlayer.o`, category B/C) |
| `set_data_align` entries | 30 | 26 (4 no-op/placement-neutral, see §5) |
| `globalize_symbols` entries | 11 | 0 (2 no-ops earlier, 9 replaced by the generic promotion, §6.7) |
| `exact_renames` / `pool_patterns` / `data_pool_patterns` entries | 222 / 201 / 26 | 180 / 186 / 21 |
| strong duplicate global definitions in the link inputs | 6 | 2 |
| unresolved global references in the link inputs | 1333 | 1330 (0 new, 3 fixed) |
| accepted targets at the retired-rule functions | `OSInit` / `__OSThreadInit` = CODE_MATCH | both **FULL_MATCH** via the new ADDR16 comparator (§6.4) |

Full link (`build/us/main.elf`) is **red in this tree for reasons outside this
batch** (two source/config duplicate definitions and a `wibo`/mwldeppc crash in
the current rule set), so DOL-byte neutrality is proven at the resolution level
(§6) plus a full link-input symbol-resolution diff (§6.2), not by comparing two
linked DOLs. The handoff allows this: “Verify DOL-neutrality where a full link is
reachable.”

## 2. `bake_linker_addrs` — all three retired

Entries deleted: `OS.o` (`__ArenaLo`, `_db_stack_end`), `OSThread.o` and
`__start.o` (`_stack_addr`). `OS.o` also carried the two remaining category-A
fields, so the key disappeared entirely.

Retail facts (`build/us/obj/.../__start.o`, `readelf -r`/objdump):

* retail `.init+0x234`: `3C208067` (`lis r1,0x8067`) / `6021B560`
  (`ori r1,r1,0xb560`), **no reloc** at `+0x236`/`+0x23A` (nearest
  `_SDA2_BASE_` at `+0x23E`/`+0x242`);
* decomp `.init+0x234`: `3C200000`/`60210000` with `R_PPC_ADDR16_HI/LO`
  `_stack_addr`.

Resolution proof (`tools/coop/lib/reloc_canon.py --check`, evaluated against
`config/us/splits.txt` + `build/us/ldscript.lcf`):

```
linker-absolute check OK (us, layout from .../build/us/ldscript.lcf):
4 retired bake targets resolve as retail
_stack_addr = 0x8067B560   _db_stack_end = 0x8067B560
__ArenaLo   = 0x8067D560   memcpy        = 0x80004000
```

The honest link therefore installs exactly the immediates the rule used to bake;
the rule was DOL-neutral *while the layout matches retail* and it hid layout
drift, which is why it is gone. The check is now a CI gate
(`.github/workflows/unit-rules.yml`, `python tools/coop/lib/reloc_canon.py
--check`), so `_stack_addr` drift (e.g. a wrong `.sbss2` size) fails loudly
instead of being silently baked. The check falls back to a built-in ldscript
template when the gitignored `build/` tree is absent (CI over committed files).

Affected targets (`hexdiff` uses the untouched raw object, so its verdicts are
unchanged by construction): `__init_registers` (`__start`, NOT_STARTED),
`__OSThreadInit` (`OSThread`, CODE_MATCH 99.957), `OSInit` (`OS`, CODE_MATCH
99.889). No accepted target depended on a baked immediate.

## 3. `force_symbol_relocs` (`OS.o` `OSInit`) — retired

Worked case 2 confirmed: `config/us/symbols.txt:1` places
`memcpy = .init:0x80004000`, and the rule's two operands are exactly
`lis r4,0x8000` (`@ha`) and `addi r4,r4,0x4000` (`@l`) for that address
(`addr16_immediate(0x80004000, HA) == 0x8000`, `LO == 0x4000`). The raw decomp
object bakes `0x80004000` inline; the retail split keeps the zeroed immediate
plus the ADDR16 relocs the splitter reconstructed. Both forms install the same
value in the linked image, so the entry is representation-only and was deleted
rather than handed to category C.

`OSInit` remains CODE_MATCH 99.889 (it was never accepted), so no demotion.

## 4. Pure representation renames — retired

Method: per-rename resolution analysis of every rename-class element
(`exact_renames`, `prefix_renames`, `pool_patterns`, `data_pool_patterns`,
`symbol_sizes`) against the full `main.elf` link-input symbol index, then a
link-level A/B diff of the two virtual input sets (§6.2).

* **TU-local targets** (`@N` pools, `@LOCAL@…`, `$N` statics, `...section.0`
  symbols): MWCC emits local binding and every reloc references the symbol by
  *index*, so renaming it cannot change any resolved address. Examples:
  `locale.o` (`@141..@149` → `@140..@148`), `scsystem.o` (38 entries),
  `dvdFatal.o` (12), `HBMBase.o` (145), `snd_EnvGenerator.o` pool patterns,
  `code_80213488.o`, `CfGimmickItem.o`.
* **No-op elements** (the renamed symbol is absent from the current object):
  `COption.o`, `CWorkThreadSystem.o`, `MPFDrawMdlColor.o`, `CSaveLoad.o`,
  `CREvtModelMap.o`, `CTagProcessor.o`, `GXInit.o`, `dvd.o`, `EXIBios.o`,
  `ut_ResFont.o`, `ut_TagProcessorBase.o#Q36nw4hbm`, `CScriptCode.o`,
  `CTitle.o`, `CSkipTimer.o`, `FloatUtils.o`, `SIBios.o`, `ai.o`, `AX.o`,
  `dvdFatal.o`, `encutility.o`.
* **Duplicate definitions removed** (`--redefine-sym` was redefining the TU's
  own GLOBAL definition onto a label another linked object already defines):
  `CErrorWii.o` ×4 (`lbl_eu_80665A60..66` collide with `ScheduleList.o` — this
  was one of the six live link errors), `CfObjectModel.o` ×2, `CfPadTask.o`,
  `snd_MmlSeqTrack.o`, `snd_SeqSound.o` ×2, `ut_IOStream.o`.
* **Renames whose names nobody else references** (global definitions used only
  from their own object): `New.o`, `NANDCore.o`, `HBM*`, `CLibCri.o` (4 of 6),
  `MPFDrawMdlColor.o` (4), etc.

`symbol_sizes` (`CScnTexWorkMan.o`) is representation-only by construction —
`st_size` is not a linker resolution input — and was retired. The remaining
`snd_SoundPlayer.o` entry is a mixed B/C entry.

## 5. `set_data_align` — handoff correction (not representation-only)

The handoff premise (“ppcdis writes align=4 on sections MWCC emits with align
8 … alignment is a property of the retail splitter”) does not hold for the
majority of entries. Evidence:

* For every `retail=4 / decomp=8` entry the retail **section start address is
  not 8-aligned** (e.g. `CNReqtaskSave.o .rodata` at `0x805245D4`,
  `CWorkThread.o .sdata` at `0x8066351C`, `CScnFilter.o .rodata` at
  `0x805240AC`). A section whose input alignment were 8 could not have been
  placed there, so the retail object's true alignment was 4 and the decomp
  object's 8 is a source-shape difference (an 8-aligned member the original
  did not have).
* MW ld honours input-section alignment: a partial link of a 4-byte object
  followed by one containing a `double` places the `double` at `+8`
  (4 bytes of padding) — `.scratch/unitrules_a/align/` reproduction,
  `mwldeppc -r a_ro.o b_ro.o`.
* Therefore removing those rules would move the section in the honest link
  (a layout change, category B/C), even though the comparator could be relaxed.

Actions taken:

* retracted (no-op or placement-neutral): `code_804B2FF0.o .bss`,
  `CDeviceGX.o .data/.sbss`, `CView.o .sdata`,
  `CLibCriMoviePlay.o .rodata/.data/.sdata` (rule value == decomp value), and
  `AXCL.o .bss`, `DSPCode.o .data`, `uusb_ppc.o .bss`, `bte_main.o .bss`,
  `snd_StrmPlayer.o .bss` (rule raises/lowers alignment but the retail start is
  divisible by the decomp alignment, so placement cannot change);
* kept as a **layout handoff**: the 26 remaining entries, with the address
  evidence per entry in `.scratch/unitrules_a/align_audit2.py` output. These
  belong to category B/C (fix the source declaration's alignment) or stay.

No `data_match` alignment relaxation was added: making the gate ignore a
NOBITS align difference that provably moves the section would certify a
non-matching link. (File-backed sections already pass on byte identity; that
under-strictness is pre-existing and unchanged.)

## 6. Verification

### 6.1 Comparator (new)

* `tools/coop/lib/reloc_canon.py` — `LinkerLayout` (parses
  `config/<region>/symbols.txt`, `config/<region>/splits.txt`, and
  `build/<region>/ldscript.lcf`), `canon_reloc`, `addr16_immediate`,
  `RETAIL_LINKER_ABSOLUTES`, `verify_known_absolutes`, CLI `--check`.
  Key correctness rule: an object-local definition wins over `symbols.txt`
  (TU-local `@86` collides with another unit's `@86`).
* `tools/coop/lib/data_match.py` — `ParsedObject`/`Reloc` parse (symbol table
  included, `sh_link` honoured with the MWCC fallback), `derive_section_bases`
  (section base from the object's globally-named symbols), `canonical_reloc_key`
  (class + resolved address; name fallback is fail-closed),
  `compare_reloc_sets`, and an **opt-in** `strict_relocs` flag on
  `check_data_sections`. Default gate behaviour is unchanged (bytes identical ⇒
  reloc order/name immaterial), which is what the handoff asked to keep.
  Measured blast radius of the strict mode with canonical targets:
  **995 units clean / 144 with real drift** (presence/type/addend and
  `.text`-offset differences; `.scratch/unitrules_a/strict_probe2.py`), so the
  strict mode stays opt-in and the default gate is not re-broken.
* `tools/coop/tests/test_reloc_canon.py` — 19 tests (ADDR16 HA/HI/LO immediates,
  ldscript evaluation + drift, real `us` layout check, name drift with equal
  resolved address, addend/type/presence drift, TU-local `@86` collision,
  section-base derivation). `python3 -m unittest tools.coop.tests.test_reloc_canon`
  → OK; full coop suite 447 tests OK.
* `tools/project.py` link routing now uses `unit_has_rules(built_obj_path.name)`
  (exported from `postprocess_reloc_names.py`) instead of a plain
  `name in UNIT_RULES` dict lookup, so `#<substring>` scoped keys reach the
  link; `select_unit_rules()` still does the runtime (symbol-aware) selection.
  This is the handoff's correctness fix. Only one linked unit was affected
  (`ut_TextWriterBase.o#Q36nw4hbm2ut`, a `pad_data_section` rule); the other
  four scoped-only keys belong to units that are not linked from source.
* `tools/coop/lib/objdiff_report.py` (handoff comparator item 4) — **checked, no
  change needed**: it drives the external objdiff-cli and consumes its report;
  the reloc-site decision that gates FULL_MATCH is `equivalence_check`'s
  `_normalize_reloc_dest`/`certify_unit_symbol` (canonical, §6.4), whose
  `evidence == "full-instruction-match"` overrides the objdiff under-score.

### 6.2 Link-input resolution diff (before/after the batch)

`.scratch/unitrules_a/verify_link_symbols.py` builds two virtual input sets —
BEFORE (raw objects with the pre-edit table applied to scratch copies) and AFTER
(the current `*.reloc.o`) — and diffs definitions, duplicates and unresolved
names:

```
=== BEFORE: 1161 inputs   duplicates 18 (6 strong)   unresolved 1333
=== AFTER : 1161 inputs   duplicates 11 (2 strong)   unresolved 1330
new duplicates: []
fixed duplicates: GetFont__Q34nw4r3lyt16ResourceAccessorFPCc,
  __ct__/__dt__Q34nw4r3lyt16ResourceAccessorFv, lbl_eu_80665A60/64/65/66
new unresolved: []
fixed unresolved: OS_PHYS_DEBUG_INTERFACE, __ct__/__dt__Q36nw4hbm3lyt16ResourceAccessorFv
referenced names with changed definers (redirects): 2 — both now defined
```

A second pass (`RELOC-SITE DELTA`) compares every reloc site's target symbol and
definer set: 16 sites changed, all accounted for as
(a) local/own-object symbols whose *name* changed while the `st_value` did not
(`ut_TagProcessorBase.o#Q36nw4hbm` `@2491/@2492` → `@2839/@2840`, both at
`.data+0x14`/`+0x5C`; `CLibCri.o` `@452@__dt__7CLibCriFv` → `thunk452_dt`, both
at `.text+0x458`) or (b) a twin-basename artifact in the before-input
reconstruction (`lyt_resourceAccessor.o`). No site moved to a different
defining object.

Caveat: the table is edited concurrently by the other categories; the numbers
above are the A/B for this batch's window (BEFORE = the pre-edit table applied
to scratch copies, AFTER = the `*.reloc.o` inputs at that moment).

### 6.2b Link-input shape after the batch

After `configure.py` regenerated `build.ninja`, the link posts 99 rewritten
objects (from 128 before the batch) — every retired key whose unit is linked
from source now links its **raw** object directly, so its link-input content is
byte-identical by construction and no `*.reloc.o` copy exists at all.

Nothing that was resolved before resolves elsewhere now; seven duplicate
definitions are gone and none were added. The two remaining strong duplicates
(`CDeviceGX::cacheInstance` across two decomp TUs, `cf::CHelp_OpenPartyMenu::
isPartyMenuReady()` between a retail split and a decomp TU) are pre-existing
source/config debt outside UNIT_RULES.

### 6.3 Raw gate

`UNIT_RULES` never touches the raw objects (`run.py data diff`/`hexdiff` read
`build/us/src/*.o`; the link postprocess writes `*.reloc.o` only), so every
target and data-gate verdict is unchanged by construction. The default
`check_data_sections` path is still size/bytes/align only (the canonical reloc
comparison is opt-in), so the parser rework cannot move a verdict either.
`run.py data diff --all` runs clean end to end: 1161 units scanned, 400
pre-existing data-mismatch failures (the migration backlog).
Per-retired-entry status for the record (`.scratch/unitrules_a/retired_evidence.py`): 39 MATCH,
37 FAIL, 5 extern-only (81 unit rows over the 78 retired keys); the 5 live FAILs (`AXCL.o`, `CErrorWii.o`,
`CfGimmickItem.o`, `bte_main.o`, `code_80213488.o`) failed before the batch too.

`python3 tools/check_unit_rules_frozen.py --check` → OK (`current 434, baseline
434, retired 81`); `python3 tools/coop/lib/reloc_canon.py --check` → OK.

### 6.4 Witness-side ADDR16 equivalence (doc item 3) — and two targets accepted

`tools/coop/lib/equivalence_check.py` now canonicalizes reloc sites beyond the
name-level reloc map:

* `_addr16_only_differences` / `_addr16_materialized_ok`: a body pair whose
  differences are all ADDR16 immediate fields is accepted when the side with the
  reloc resolves (via `reloc_canon`) to an immediate equal to the other side's
  baked field.  Difference-driven, so unrelated ADDR16 sites (TU-local `@N`
  pools the reloc map canonicalizes) cannot block; unresolvable targets fail
  closed (`linker_layout=None` restores the old strict behaviour).
* `_resolve_reloc_address`: two different destination names that resolve to the
  same linked address (`symbols.txt` / ldscript) now compare equal.

Real-data validation (`extract_function_pair` + `_byte_identical_with_relocs`):

| function | plain | extended |
|---|---|---|
| `OSInit` | False | True |
| `__OSThreadInit` | False | True |
| `OSCreateThread` | True | True |

`OSInit` differs in 10 fields: 2 decomp `memcpy` ADDR16 relocs (the retired
`force_symbol_relocs` rule) plus 8 retail `__ArenaLo`/`_db_stack_end` relocs the
decomp source bakes.  `certify_unit_symbol` returns `ProofStatus.EQUIVALENT`
with `certificate.evidence == "full-instruction-match"` for both functions and
`classify_status` maps that to FULL_MATCH regardless of the objdiff under-score;
the split-size gate passes (`OS` 0x1540/0x1540, `OSThread` 0x1628/0x1630).
`run.py cycle` was run for both:

```
us-80354fb0 OSInit         -> PASS: meets required level EQUIVALENT_MATCH (FULL_MATCH)
us-8035e0b0 __OSThreadInit -> PASS: meets required level EQUIVALENT_MATCH (FULL_MATCH)
```

So the two functions the retired bakes used to paper over are now certified from
the raw objects with no rule at all.  (Fixed while verifying: a literal
`99.7% CPU` in `run.py cycle`'s argparse help aborted `cycle --help` with
`ValueError: unsupported format character`.)

### 6.5 Second-pass retirements (CVec4/CCol3/mwsfdply/CMenuBattlePlayerState)

All four units are `NonMatching` (not linked from source), so the link inputs
are unchanged:

* `CVec4.o`, `CCol3.o`: `...bss.0` (LOCAL section symbol) renames whose target
  name is either already a GLOBAL definition in the same object (`CVec4`) or
  defined/referenced nowhere in the link inputs (`CCol3`).
* `mwsfdply.o`: the globalize target is already GLOBAL in the object and nothing
  references it.
* `CMenuBattlePlayerState.o.globalize_symbols`: both names are absent from the
  object (the current `pool_patterns` set no longer produces them).

### 6.6 `globalize_symbols` — verified tooling requirement, not representation

Decisive experiment (`.scratch/unitrules_a/globalize/`): two tiny MWCC objects,
one defining `g_sym`, the other referencing it with a GLOBAL UNDEF (control) or
a `.symtab`-patched LOCAL UNDEF (the state the strip leaves behind).  Linked
with the project's ldscript:

```
control (GLOBAL UNDEF): link ok, a_p = 0x80004058 (g_sym)
LOCAL  UNDEF          : ### mwldeppc.exe Linker Error: undefined: 'g_sym'
```

So the field is a real mwldeppc constraint, not a representation artifact.  Per
element (`globalize_audit.py`, applying each entry with and without the field):

* retired as provable no-ops: `CMenuBattlePlayerState.o` ×2 (name absent),
  `mwsfdply.o` ×1 (already GLOBAL);
* load-bearing, kept: `CtrlAct.o`, `snd_StrmSound.o`, `mpv_mcy.o`, `adx_dcd.o`,
  `sfd_adxt.o`, `sfh_ver1.o` (LOCAL UNDEF after the strip);
* `mpv_mc.o`, `mpvabdec.o` (objects not built) and `sfx_cnv.o` ×3 (LOCAL
  definition export) — kept conservatively.

All nine were retired once the promotion became generic in the tool (§6.7);
`globalize_audit.py` and the audit above remain the per-element evidence that
the promotion is what the fields were doing.

### 6.7 `globalize_symbols` retired — generic end-of-pipeline promotion

**Mechanism** (`tools/postprocess_reloc_names.py`): at the end of
`postprocess_object`, `globalize_local_undefs(path,
rule_produced_symbol_names(rules))` promotes every `STB_LOCAL`/`SHN_UNDEF`
symtab entry whose name one of *this entry's* rules produces (`exact_renames`,
`prefix_renames`, `pool_patterns`, `data_pool_patterns`, `add_symbols`,
`retarget_relocs`, `inject_relocs` targets), when a relocation in an `SHF_ALLOC`
section targets that entry and the object does not also define the name
globally. The filter keeps the pass scoped to names the entry itself created, so
unrelated LOCAL symbols are never touched (`prefix_renames`/`add_symbols` names
were not part of any `globalize_symbols` payload, so they gain 11 promotions the
fields never covered; each resolves to the same unit's retail object).

Why it cannot regress the link:

* a reloc-targeted LOCAL UNDEF is a hard mwldeppc error whether or not a
  definition exists elsewhere (repro below), so promotion can only turn that
  error into a name-based resolution;
* only UNDEF entries are promoted, so the pass can never add a definition:
  duplicate-global counts are unchanged;
* objcopy's `--globalize-symbol` also flips *same-value aliases* (`@N` twins of a
  promoted name) from LOCAL UNDEF to GLOBAL UNDEF. Measured over the frozen raw
  inputs (521 rule objects): 470 produced-name promotions, 275 alias binding
  flips, **0 new global definitions**.

**Repro** (`.scratch/unitrules_all/gl/repro_full.log`; `b_g.o` defines `g_sym`,
`a_g.o` references it GLOBAL UNDEF, `a_local.o` LOCAL UNDEF):

| link | result |
|---|---|
| `entry.o a_g.o b_g.o` | 1104-byte ELF |
| `entry.o a_local.o b_g.o` | `undefined: 'g_sym'` ×2, link failed |
| `entry.o a_g.o` (no definer anywhere) | `undefined: 'g_sym'` ×2, link failed |
| `entry.o a_local.o` (no definer anywhere) | `undefined: 'g_sym'` ×2, link failed |

So the flip is a strict improvement: reloc-targeted refs that only the fields
used to rescue now resolve by name, and refs with no definition at all are red in
both bindings (identical mwldeppc message).

**Measurements** (`pp_ab.py`, `classify.py`, `final_ab.py`,
`promoted_resolution_decomp.py`):

| level | result |
|---|---|
| 521 raw postprocessed objects, pass on/off (`pp_ab.py` + `classify.py`) | 203 objects change: 470 produced-name promotions, 186 alias UNDEF flips, 89 other alias-entry flips, 0 new global definitions |
| 1161 link inputs, decomp inputs regenerated from raw with the pass on/off (`final_ab.py`) | duplicate global defs 11 → 11 (0 new, 0 fixed); unresolved global refs 1330 → 1356, and the 26 added names are exactly the promoted aliases with **no definer anywhere** (they were LOCAL UNDEF, i.e. `undefined` for mwldeppc, before — same error, now visible to the analyzer) |
| installed decomp link inputs: referenced rule-produced LOCAL UNDEF left over (`promoted_resolution_decomp.py`) | **0** — nothing the fields would have promoted is still local |
| `ninja build/us/main.elf` before/after (99 RELOCPOST steps re-ran with the final tool) | identical linker diagnostics (2 `multiply-defined`, the FORCEACTIVE warnings, `FAILED: [code=139]`); the SIGSEGV still blocks a DOL-vs-DOL comparison |

Three per-unit notes: `mpv_mc.o`/`mpvabdec.o` are not built at all; `sfx_cnv.o`
also had its three LOCAL definitions *exported* by the field, but that unit is
not a link input (only the retail `sfx_cnv.o`/`sfx_cnv_to_Y84C44.o` are), no link
input references the names globally, and the pass deliberately leaves definitions
alone (no new global symbols); the six built units (`CtrlAct`, `snd_StrmSound`,
`mpv_mcy`, `adx_dcd`, `sfd_adxt`, `sfh_ver1`) leave the pipeline with their
target names GLOBAL UNDEF exactly as the field did
(`.scratch/unitrules_all/gl/nine_units.txt`).

## 7. Manifest / claims

* `--ratchet` recorded the batch: `baseline_count 512 → 434`; the only payload
  change was `CLibCri.o.exact_renames` (4 of 6 elements retired).
* `CtrlMoveNpc.o` was retired in the first pass and **re-added in the same
  session**: the link-level diff showed `__ct__cf_CtrlMoveNpc` becoming the one
  new unresolved name — the retail `CtrlNpc.o` carries
  `R_PPC_REL24 __ct__cf_CtrlMoveNpc` and the rename was its only provider. The
  key is restored with the original payload and a comment; it is a category-A
  handoff (the source should declare the retail name, or the rule stays).
* 401 keys are annotated: retired keys as `representation-only` /
  `linker absolute resolved by reloc_canon`; the kept A-only keys and the mixed
  entries with their handoff reason (`--owner category-a`). Remaining
  unassigned keys belong to the other categories.
* Second pass (`--ratchet 434 → 431`, 4 more claims): `CVec4.o`, `CCol3.o`,
  `mwsfdply.o` retired with reasons, `CMenuBattlePlayerState.o` annotated for
  the retired globalize elements.
* Globalize follow-up (`--ratchet 429 → 429`, 9 claims, owner `category-a`):
  the nine `globalize_symbols` payloads dropped; the dataclass field and its
  per-unit call path are gone, so the table can no longer carry a per-unit
  globalize at all (`check_unit_rules_frozen.py` no longer knows the name).

## 8. Handoffs / residuals

| residual | owner | evidence |
|---|---|---|
| `CLibCri.o` (`rtti_*` → `__RTTI__*`), `CLibLayout.o`, `CArtsSet.o`, `CChainTime.o`: global UNDEF reference retargeted by name | source (`extern "C" … __RTTI__…`) | `symres` LOAD_BEARING; retail objects reference the target names |
| `CGXCache.o`, `CSysWinScenarioLog.o` `add_symbols`: GLOBAL subobject labels other link inputs reference | source (`extern "C" lbl_eu_*` decls) | `live_rename_report` external-ref counts |
| 9 `globalize_symbols` entries / 14 elements: mwldeppc cannot resolve a LOCAL UNDEF (§6.6) | **done** — generic end-of-pipeline promotion replaces the fields (§6.7); link diagnostics unchanged, 0 referenced LOCAL UNDEF left, 0 new definitions | — |
| 26 `set_data_align` entries: source-side section alignment (§5) | B/C | `align_audit2.py` address table |
| `CErrorWii.o`: the rename removal clears the `lbl_eu_80665A60..66` duplicate link error, but the TU still defines 7 bytes of `.sbss` that the retail split has as size 0; the source should declare the statics `extern "C" lbl_eu_*` (no definition) once the shared data object owns them | D | raw gate `.sbss: retail 0x0 != decomp 0x7`; link error before the batch |
| witness-side ADDR16 baked-vs-reloc equivalence | **done** — `equivalence_check` extended, 10 tests, `OSInit`/`__OSThreadInit` accepted (§6.4) | — |

## 9. Reproduction

```bash
.venv/bin/python3 tools/coop/lib/reloc_canon.py --check
.venv/bin/python3 -m unittest tools.coop.tests.test_reloc_canon
.venv/bin/python3 tools/check_unit_rules_frozen.py --check --report
.venv/bin/python3 .scratch/unitrules_a/verify_link_symbols.py        # link-input A/B
.venv/bin/python3 .scratch/unitrules_a/retired_evidence.py           # per-unit gate table
.venv/bin/python3 .scratch/unitrules_a/strict_probe2.py              # strict-mode blast radius
.venv/bin/python3 .scratch/unitrules_a/globalize_audit.py            # globalize no-op vs load-bearing
# generic globalize pass (§6.7):
.venv/bin/python3 .scratch/unitrules_all/pp_ab.py                    # raw pipeline A/B (pass on/off)
.venv/bin/python3 .scratch/unitrules_all/classify.py                 # what the pass changed (classes/risks)
.venv/bin/python3 .scratch/unitrules_all/final_ab.py                 # link-input resolution A/B
.venv/bin/python3 .scratch/unitrules_all/promoted_resolution_decomp.py
.venv/bin/python3 .scratch/unitrules_all/link_gen.py /tmp/gen.elf --generic-globalize
cat .scratch/unitrules_all/gl/repro_full.log                         # LOCAL vs GLOBAL UNDEF repro
cat .scratch/unitrules_all/gl/nine_units.txt                         # the 9 retired entries' symbols
```

Scratch artifacts (plan, pre-edit table copy, probes) live under
`.scratch/unitrules_a/`; the pre-edit table is
`.scratch/unitrules_a/postprocess_reloc_names.py.pre-a`. The §6.7 globalize-pass
probes (`pp_ab.py`, `classify.py`, `final_ab.py`,
`promoted_resolution_decomp.py`, `link_gen.py`, `gl/`) live under
`.scratch/unitrules_all/`.
