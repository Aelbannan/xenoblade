# Category C — Byte falsification (content rules)

**Hand to:** one or two strong matching agents. **Goal:** eliminate every
rule that changes bytes/relocations/data the honest build would not produce.
Each entry gets the real source shape, or its targets get demoted. Never
patch. Prerequisite reading: `docs/handoff/unit_rules/README.md`.

**Status (2026-09-28) — what category A left you:** A is closed
(`docs/evidence/decomp/unit_rules_category_a.md`). Relevant outcomes:

* `OS.o`, `OSThread.o` and `__start.o` are **gone**: their `bake_linker_addrs`
  and `force_symbol_relocs` entries were proven representation-only and
  deleted (A §2 and §3), and a CI gate now re-checks the linker-absolute
  formulas.
* `rfc_mx_fsm.o` / `rfc_port_fsm.o` (`addend_sets`) are **live** in the current
  link; the live/latent split below must be re-derived with the README snippet
  (the scoped-key routing fix changed which keys reach the link).
* A representation-class finding is no longer "hand to A": prove it with the
  canonical comparator (`tools/coop/lib/reloc_canon.py`,
  `data_match.canonical_reloc_key`, opt-in `strict_relocs`). Alignment is
  **not** comparator tolerance (A §5).

## Scope: 28 content-class entries, 9 live

Live (i.e. applied in the current `main.elf` link), in priority order:

| entry | fields | note |
|---|---|---|
| `CProc.o` | `exact_renames`, `extern_data_sections`, `inject_relocs`, `pad_data_section`, `pool_patterns`, `swap_data_blocks`, `trim_text_size` | mixed: content core is inject/swap/trim |
| `CTaskManager.o` | `drop_data_range`, `inject_relocs` | removing real retail data + manufacturing a slot |
| `mtx.o` | `swap_sdata2_leading_f32_words` | literal pool order |
| `AXFXReverbHiExp.o` | `swap_sdata2_leading_f32_pair` | literal pool order |
| `AXFXChorusExp.o`, `AXFXChorusExpDpl2.o` | `swap_data_blocks` | vtable/typeinfo emission order |
| `OSNet.o` | `swap_data_blocks` | vtable/typeinfo emission order |
| `rfc_mx_fsm.o`, `rfc_port_fsm.o` | `addend_sets` | jumptable case-label addends; function length differs from retail |

Latent (same triage, not currently linked through the rule):
`CNBanner.o`, `CGXCache.o`, `CScn_80496B0C.o`, `KPAD.o`, `CTaskEnvironment.o`,
`code_804BC9EC.o`, `CView.o`, `CWorkThread.o`, `CChildListNode.o`,
`lyt_layout.o`, `lyt_pane.o#Q36nw4hbm3lyt`, `CDeviceFileCri.o`, `CDevice.o`,
`CDeviceClock.o`, `g3d_state.o`, `code_80135FDC.o`, `vi.o`, `WUD.o`,
`dct_isr.o`, `sfd_tim.o`.

## Triage protocol (per entry)

1. **Byte-affecting or representation?** Comment out the entry's fields,
   rebuild via hexdiff, and diff the objects (`readelf -S/-r`, objdump words).
   - Only reloc presence/name/addend or a linker-constant difference ⇒ prove
     it with the canonical comparator (`tools/coop/lib/reloc_canon.py`,
     `data_match.canonical_reloc_key`, opt-in `strict_relocs`) and delete the
     rule with the proof. Alignment differences are **not** representation:
     `set_data_align` entries belong to B/C source work (A §5).
   - Actual instruction/data byte or section-size difference ⇒ continue.
2. **Find the source shape.** Use `hexdiff` (`--asm`, `--relocs`),
   `mwcc_kb.py search`, `docs/MWCC_CASES.md` / `MWCC_PATTERNS.md`, and
   `reloc_map.py`. Keep the code high-level C/C++.
3. **If impossible in C++**: record the residual as a near-miss
   (`attempts.jsonl` open-item packet), demote the affected target(s) in
   `targets.json`, delete the entry. §17.6 escapes only for cases already
   listed in PLAN.md, with owner approval and a `"policy_exception": true`
   log. "MWCC emits it in a different order" is **not** an exception.

## Per-field playbook

**`inject_relocs`** (manufactured vtable/typeinfo words — `CProc`,
`CChildListNode`, `CTaskEnvironment`, `CDevice`, `CDeviceClock`, `g3d_state`,
`WUD`, `lyt_pane#…`, `CTaskManager`): every injected reloc is a word retail
has and MWCC zeroed. Find why the word is missing:
`-RTTI` on/off per `Object()`, `__declspec(novtable)`, explicit vtable/typeinfo
definitions in the owning TU, base-list emission. If the word genuinely comes
from the original linker (unspellable in C++), accept the data drift —
EQUIVALENT_MATCH does not require data identity — and delete the rule.

**`swap_data_blocks`** (`AXFX*`, `CProc`, `CView`, `lyt_layout`, `lyt_pane#…`,
`OSNet`): MWCC emits globals-then-vtables; retail interleaved them. Levers:
definition order in source, splitting the unit, RTTI flags, explicit
definitions. If not reachable, accept drift.

**Pool words** (`permute_sdata2_words`, `swap_sdata2_*`,
`reverse_sdata2_trailing_f32x4`, `patch_unsigned_magic`,
`trim/pad_sdata2_size`): literal-pool content/order. Source first-use order
fixes many; the int→double magic pair cannot be spelled in C++ → accept the
drift at EQUIVALENT (semantics unaffected) and delete the rule. Never permute
bytes to fake the pool.

**`addend_patches` / `addend_sets`** (jumptable case-label addends: `KPAD`,
`CWorkThread`, `vi`, `code_804BC9EC`, `dct_isr`, `sfd_tim`, `rfc_*`): these
exist because the *function's code length differs from retail*; patching the
addend for an unmatched function falsifies the data comparison. Match the
function, or demote it and delete the rule.

**`drop_data_range` / `drop_data_tail` inside content entries**
(`CTaskManager`, `CNBanner`, `CGXCache`, `KPAD`, `CView`, `CWorkThread`,
`CDevice*`): deleting real retail data to dodge a layout mismatch. Recover
the data in source (types, order, ownership) or demote/delete the rule.
Coordinate with B: EDS entries remain D's scope until D's model lands.

**`force_symbol_relocs` / `bake_linker_addrs`** — **resolved by category A**
(the fields no longer exist in the table; `OS.o`/`OSThread.o`/`__start.o` are
gone, and `reloc_canon.py --check` is a CI gate). No action here.

**Zero-user legacy fields** (`copy_data_sections`, `insn_patches`,
`insn_patches_post`, `insert_insns`, `reloc_offset_moves`,
`data_reloc_offset_moves`): nothing to do today, but if any reappear treat
them as critical and delete them immediately.

## Method details

- Always rebuild before evaluating: `hexdiff` recompiles the unit; a stale
  `.o` lies. `hexdiff --all` is the fastest per-function table.
- The acceptance gate is `run.py cycle <target-id> --hypothesis "..."
  --next-change "..."` (witness runs inside; never `--smt`/`--linked`/plain
  `run.py diff` in this fork).
- Data side: `run.py data diff <unit>` is raw-only now. A unit only counts as
  data-matched when its unmodified object matches retail.
- If the content rule masks a *code* residual (e.g. `addend_*`), the correct
  outcome may be a recorded near-miss at the current match% — that is
  acceptable and expected.

## Evidence / definition of done

- Per entry: honest-vs-retail byte evidence, the source change or the
  demotion record, raw-gate output, affected targets.
- Append to `docs/evidence/decomp/attempts.jsonl`; per-target records to
  `docs/MWCC_CASES.md`; reusable recipes to `MWCC_PATTERNS.md`.
- `--claim "<key>" --owner <you> --class content --reason "..."` as you go;
  `--ratchet` after a reviewed batch.
- Done when the 9 live content entries are gone or demoted (or, for the
  representation-class parts, proven neutral with the canonical comparator),
  no rule in this class changes a byte the source cannot justify, and every
  affected target has been recertified from raw.

**Report back:** per entry (deleted / demoted / proven-neutral),
byte evidence summary, demoted target ids, and the new `--report` numbers.

---

## Handed over from category B (2026-09-28)

B closed 16 keys + 23 fields and annotated every remaining entry; its
`docs/evidence/decomp/unit_rules_category_b.md` §6b and the per-entry
`docs/evidence/decomp/unit_rules_category_b_packets.md` are the authoritative
lists. Three B-scope classes are **byte-content** decisions that belong here:

### 1. Missing-tail (`pad_*`) entries — retail has bytes our source does not produce

15 in-scope entries; every extra retail byte is **zero**, and the packet doc
records the retail symbol (`symbols.txt`) that covers those bytes:

| key | section | retail / decomp | retail tail object | reading |
|---|---|---|---|---|
| `btu_init.o` | `.sdata2` | 0x8 / 0x6 | `BT_BD_ANY` size 0x8 | same symbol, 2 bytes longer in retail |
| `hcicmds.o` | `.rodata` | 0x10 / 0xA | `lbl_8050E260` size 0x10 (`data:byte`) | 6-byte object missing (TU is an auto-scaffold) |
| `WPADEncrypt.o` | `.data` | 0x1328 / 0x1327 | `lbl_80562200` size 0xC8 (`data:string_table`) | 1 byte missing (last string/NUL) |
| `synpitch.o` | `.rodata` | 0x18 / 0x14 | `double_80518B80` + `lbl_80518B88@+0x10` size 8 | last object is 4 B where retail has 8 (float vs double/pair literal in an unmatched function) |
| `ut_TextWriterBase.o#Q36nw4hbm2ut` | `.rodata` | 0x18 / 0x14 | `lbl_80518B40@+0` size 8, `lbl_80518B50@+0x10` size 8 | two objects 4 B short (literal type mismatch) |
| `HBMAxSound.o` | `.rodata` | 0x38 / 0x34 | 4th word after `s_volumeZeroPad` (+2 pooled) | 4 zero bytes; `SetVolumeAllSeq` may have a folded `0.0f` |
| `snd_Util.o` | `.data`/`.sdata`/`.sdata2` | 0x10/0xC, 0x8/0x4, 0x0/0x24 | — | pads + an extra local `.sdata2` pool (mixed with the code class) |
| `CfRes.o` | `.data` | 0x220 / 0x50 | dissolved monolib data | real data owned elsewhere → D, not C |
| `snd_FxReverbStdDpl2.o` | `.data` | 0x28 / 0x10 | vtable-shaped | 24 B of data missing → D |
| `UnkClass_80460C34.o` | `.data` | 0x78 / 0x74 | `gap_07_8056D674_data@0x74` | retail gap symbol → linker padding, exception not content |
| `CDeviceVI.o`, `CModelDispMakeCrystal.o`, `ax_rna.o`, `code_804BC9EC.o`, `snd_Util.o` | mixed | see packets | — | combine pads with code/alignment classes |

Decision per entry: reconstruct the real retail object (preferred) or accept a
§17.6 exception. Do **not** add a fake zero static to the source — B removed
nine of those (its §4), they are the same hack in a different place.

### 2. Code-match packets (the bulk)

32 entries drop `.sdata2`/`.rodata`/`.data` tails that their own (still
unmatched) functions reference — `dropped_refs.py` proves the liveness, so the
rule only truncates the pool of an unmatched function. Fixture: match the
function, then `.scratch/unitrules_b/field_prune.py` reports the `drop_*` field
as redundant and it is retired. The packets doc lists all 32 with the
section/size deltas.

### 3. Weak-symbol / text-GC packets

13 entries (`drop_text_symbols*`, `trim_text_size`, `repack_after_drop`).
These are **not** rule retirements: each rule truncates `.text` because our TU
*defines* a function retail's TU only references — the per-entry packet doc
records the dropped symbol, whether any link input defines it
(`definers=0` means the rule deletes the only definition and the `SHN_ABS`
rewrite plants a bogus address), and the `.text` delta. Two shapes:

* **weak inline-virtual dtors** (`__dt__10IWorkEventFv` in `CArcItem.o`,
  `CPackItem.o`, `CLibStaticData.o`: `definers=1`, the strong copy is in
  `CTaskGame.o`) — the repo's own recipe is in the `IWorkEvent.hpp` header note
  (out-of-line declaration + per-TU `IWORK_EVENT_INLINE_DTOR`); the fix must
  keep the derived dtor's matched shape *and* not emit the base stub.
  **Recipe (proven 2026-09-28, B doc §7.5):** if the unit's dtor is a
  hand-written `extern "C"` free function (as in `CArcItem.cpp`), delete the
  `#define IWORK_EVENT_INLINE_DTOR` — the vtable sub-slot then stays an UNDEF
  reference to the strong copy, the stub is never emitted, and the rewrite's
  orphan `extab`/`extabindex` entries disappear (postprocess then yields
  `.text` 0x294 / dtor@0x9C = retail). **Negative result:** where the dtor is
  compiler-generated (`CLibStaticData` 0xC4→0xD0, `CPackItem` 0xAC→0xB8,
  verified + reverted) the macro is required for the matched shape, so those
  rows need the base-dtor ownership restructured instead;
  three mechanisms were measured per row (B doc §7.6, `tail_diag.py`):
  **(a)** `lyt_drawInfo`/`lyt_group`/`lyt_window`/`lyt_arcResourceAccessor`/
  `HBMAnmController` carry an *unreferenced* weak inline-dtor copy interleaved
  between real functions (e.g. `__dt__Rect` between `DrawInfo`'s ctor and dtor);
  the emitter was isolated in §7.7 (`probe/d3.cpp`): an **out-of-line** `__dt__`
  destroying a member that has a **user-provided inline dtor** emits the weak
  copy; trivial or implicit dtors emit nothing. One row was fixed by deleting
  the bogus inline dtor (`Rect`; raw .text 0xC0 = retail, split PASS, gate MATCH
  with the drop retired). The others are **not** fixable that way and the
  experiments were reverted with A/B evidence: `Content`'s wrapper survives
  deletion (its body destroys a non-trivial `ut::Color[4]`), `FrameController`
  3/3→2/3 and `AnimTransform` 13/15→11/15 regressed matched functions — all
  genuine retail-linker-GC orphans (mwldeppc has no GC option);
  **(b)** `g3d_scnobj`/`lyt_animation` emit a *base* class's vtable+dtor that
  retail places in another TU (key-function ownership);
  **(c)** `CLib::createLibs` and `dvd_broadway`'s four are GLOBAL functions with
  **no copy anywhere in the DOL** (masked byte search) — matching work.
* **over-produced tail functions** (`g3d_resfile.o` `.text` 0x2354 vs 0x18D0
  with 20 ResFile getters, `dvd_broadway.o` 0x2CA8 vs 0x26B0, `g3d_scnobj.o`,
  `HBMAnmController.o`, `CLib.o`, the nw4hbm `lyt_*` LinkList/Rect dtors) —
  our TU defines functions whose retail bytes live in *another* unit's slice;
  the fix is to move the definition to the owning TU (find it with `hexdiff`
  against the neighbouring slices; they are unnamed `func_*` in `symbols.txt`).
  These same objects are also the **remaining link blocker** (B's doc §7.4):
  the postprocess symbol rewrite cannot produce a linkable object.
