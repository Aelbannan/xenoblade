# Category B — linker-behavior emulation (UNIT_RULES burn-down) — 2026-09-28

Prompt: `docs/handoff/unit_rules/b_linker_behavior.md`. Owner tag: `category-b`.
Predecessor: `docs/handoff/unit_rules/README.md` (freeze rules, evidence protocol)
and `docs/evidence/decomp/unit_rules_category_a.md` (A's handoff to B).

Result: **16 entries retired** (15 keys + 1 payload field) — 7 proved to be
bit-identical no-ops, 9 by repairing the source declaration the rule was
compensating for. **22 further payload fields** were pruned as provably
redundant (§4b) and every remaining in-scope entry carries a per-entry verdict
in the frozen manifest (§6). Five units moved from raw-gate `MISMATCH` to
`MATCH`. A's `set_data_align` handoff is **corrected**: MWCC cannot express the
alignment the retail splitter records, so that handoff was based on a false
premise (§5). The full-link SIGSEGV was bisected to a **tool bug in
`trim_text_section`** (SHN_ABS symbol rewrite), fixed (§7.3); the remaining link
blocker is narrowed to the `drop_text_symbols*` symbol rewrite, which no object
shape can express under mwldeppc (§7.4).

## 1. Result summary

| metric | before | after |
|---|---|---|
| `UNIT_RULES` entries | 429 | 413 |
| `check_unit_rules_frozen.py --report` baseline | 429 | 413 (ratcheted 429→422→414→413) |
| retired (manifest tombstones) | 86 | 102 |
| retired payload fields | — | 25 (22 prunes + `HBMAxSound.drop_nobits_range` + `CDeviceGX.set_data_align` + `CArcItem.drop_text_symbols` via a source fix, §7.5), plus one payload-value change (`code_804B2FF0.set_data_align` pair prune) |
| derived class `layout` / `content` / `rename` | 390 / 28 / 11 | 369 / 28 / 16 |
| `set_data_align` payload entries in the table | 26 | 22 (4 no-op payloads retired: CSysWinScenarioLog's key, CView/CLibCriMoviePlay pairs and CDeviceGX's whole field) |
| **B scope** (non-EDS, layout-or-`set_data_align`), entries / live | 111 / 40 | **88 / 27** |
| objects rewritten by the link (`RELOCPOST` steps in `build.ninja`) | 99 | 85 |
| in-scope entries with a recorded per-entry verdict | 0 | 88 (+7 EDS-bearing `set_data_align` keys annotated for B, payload for D) |
| in-scope units whose raw data gate is `MATCH` (scope-wide sweep, §6) | — | 23 (7 of them moved FAIL→MATCH in §4) |

Scratch artifacts for this batch: `.scratch/unitrules_b/` (census, effect
audit, gate sweep, probes, link A/B).

## 2. Method: the rule-effect audit (`.scratch/unitrules_b/effect_audit2.py`)

Every in-scope key is applied to a *copy of the currently built raw object* and
the result is compared with the input at full semantic fidelity (all sections
incl. bytes/size/align, the whole symbol table, all `.rela*` sets) plus a
bit-level file comparison (`noop_verify2.py`). Two verdicts fall out:

* **bit-identical** ⇒ the rule cannot affect the link (the linker consumes only
  the object's content) ⇒ delete it;
* **effective** ⇒ the rule still changes bytes/symbols/relocs ⇒ it needs a
  source fix, a documented exception, or a demotion.

This is strictly stronger than "the target symbols are absent" (category A's
no-op test) because it also catches value-equal writes, size no-ops
(`pad_data_section` to the current size, `set_data_align` to the current
alignment, `drop_data_tail` at/after the current size) and empty rename sets.

## 3. Batch 1 — rules that are now bit-identical no-ops (7 keys)

| key | fields retired | why |
|---|---|---|
| `bta_dm_act.o` | `drop_data_tail .data 0x13B`, `drop_nobits_range .bss` | `.data` is already 0x13B, `.bss` already 0x2D |
| `CViewRoot.o` | `drop_data_range`, `drop_data_tail .rodata` | targets absent (unit is `linked False`; retail split object is used) |
| `CProcRoot.o` | `trim_text_size 0x1C8`, 3 `exact_renames` | `.text` already 0x1C8, all rename targets absent |
| `CRsrcData.o` | `trim_text_size 0x42C`, 4 `exact_renames` | idem |
| `g3d_resanm.o` | `drop_text_symbols __dt__Q34nw4r2ut5ColorFv` | MWCC no longer emits that weak dtor here |
| `CSysWinScenarioLog.o` | `add_symbols` ×7, `set_data_align .sbss 8` | no-op (and the unit is `linked False`) |
| `lyt_arcResourceAccessor.o#Q34nw4r3lyt` | `data_pool_patterns`, `exact_renames`, `pad_data_section .sdata 8` | pool already named, `.sdata` already 8, `__vt__…` absent |

Proof for the four live keys (`bta_dm_act`, `CProcRoot`, `CRsrcData`,
`g3d_resanm`): the postprocess is a **bit-identical** no-op on the current
object, and after `configure.py` the link input for those units is the raw
`.o`, which `cmp`s identical to the `.reloc.o` the link used before:

```
IDENTICAL  build/us/src/RVL_SDK/.../bta_dm_act
IDENTICAL  build/us/src/monolib/src/core/CProcRoot
IDENTICAL  build/us/src/monolib/src/core/CRsrcData
IDENTICAL  build/us/src/nw4r/src/g3d/res/g3d_resanm
```

The non-live keys (`CViewRoot`, `CSysWinScenarioLog`) were already inert: both
units are `linked False`, so `tools/project.py` never applies the rule.

## 4. Batch 2 — stale source pads / wrong source sizes (9 entries, gate improvements)

B's root cause 2 ("extra/missing data") in its purest form: the source carried
`*_pad[N]` statics or oversized string/struct declarations whose only purpose
was to reproduce the retail linker's *alignment gap*; the rule then removed the
pad again at link time. Removing the source pad makes the raw object the retail
shape, which **fixes the raw data gate** (it was failing) and makes the rule a
verified no-op.

| unit | source fix | gate before | gate after |
|---|---|---|---|
| `usb.o` | removed `s_usb_sbss_pad[4]` | FAIL `.sbss` 0x9 vs 0x10 | **MATCH** |
| `dvdfs.o` | removed `dvdfs_sbss_pad{,2,3}` (24 B) | FAIL `.sbss` 0x20 vs 0x38 | **MATCH** |
| `OSError.o` | removed `oserror_bss_pad[12]` | FAIL `.bss` 0x44 vs 0x50 | **MATCH** |
| `bte_logmsg.o` | `lbl_80665908[8]→[4]`, removed `bte_logmsg_bss_pad[16]` | FAIL `.sdata` 0x4 vs 0x8, `.bss` 0x7D0 vs 0x7E0 | **MATCH** |
| `gap_utils.o` | `tGAP_CB.tail[0x3B0-0x84]→[0x3AC-0x84]` | FAIL `.bss` 0x3AC vs 0x3B0 | **MATCH** |
| `OSNandbootInfo.o` | `NandbootInfoPath[0x20]→[0x1A]` | FAIL `.data` 0x1A vs 0x20 | **MATCH** |
| `scapi_prdinfo.o` | `Product*String[8]→[5] __attribute__((aligned(8)))` | FAIL `.sdata` 0xD vs 0x10 | **MATCH** |
| `CSysWinSave.o` | removed duplicate `_pad_80664A0C` | FAIL `.sbss` 0x8 vs 0xC | `.sbss` OK (unit keeps pre-existing `.sdata2` byte / `.sbss2` size drift) |
| `HBMAxSound.o` (field only) | removed `hbmAxSound_bss_pad[4]` | FAIL `.bss` 4 vs 8 (+`.rodata`) | `.bss` OK; `.rodata` 0x34 vs 0x38 still needs `pad_data_section` |

Proof (`.scratch/unitrules_b/noop_verify2.py`): after the source fix the rule's
remaining payload is a bit-identical no-op for all nine entries
(`pp_changed=False bytes_equal=True`), except `HBMAxSound` where only the
retired `drop_nobits_range` field is a no-op and `pad_data_section .rodata 0x38`
still changes the object. Code does not regress: `hexdiff --all` on all nine
units reports every function fully matched (e.g. usb 14/14, dvdfs 11/11,
OSError 4/4, bte_logmsg 8/8, HBMAxSound 10/10, CSysWinSave 10/14 — the four
unmatched CSysWinSave functions were unmatched before, the rule never touched
`.text`).

Why the link is unchanged: the removed source bytes were exactly the bytes the
rule deleted at link time, so the linked section content and size are identical;
the following unit's section still starts at the same (8-aligned) address
because the linker re-adds the same alignment padding — verified against
`config/us/splits.txt`, where every removed pad equals the unowned gap between
the retail slice and the next unit's slice:

```
usb.c        .sbss 0x806652A8-0x806652B1 (0x9)  next @0x806652B8  gap 7
dvdfs.c      .sbss 0x80664EC8-0x80664EE8 (0x20) next @0x80664F00  gap 0x18
OSError.c    .bss  0x805D0D30-0x805D0D74 (0x44) next @0x805D0D80  gap 12
bte_logmsg.c .bss  0x805BA5C0-0x805BAD90 (0x7D0) next @0x805BADA0 gap 16
HBMAxSound.c .bss  0x805CA058-0x805CA05C (0x4)  next @0x805CA060  gap 4
```

## 4b. Per-field pruning — 22 payload fields proved redundant (`.scratch/unitrules_b/field_prune.py`)

For every in-scope entry and every field, the audit compares
`apply(entry)` with `apply(entry minus that field)` on the currently built raw
object. Identical result ⇒ the field cannot affect the link ⇒ it is dead
payload and is removed (field removals are burn-down progress per the freeze
rules). 22 fields over 18 keys:

| key | redundant fields |
|---|---|
| `CNReqtaskSaveBanner.o` | `drop_data_tail` |
| `CGame.o` | `exact_renames` |
| `CtrlNpc.o` | `drop_nobits_range` (`.sbss2`) |
| `CfRes.o` | `zero_nobits`, `exact_renames` |
| `CView.o` | `set_data_align`, `pool_patterns` |
| `CWorkSystemMem.o` | `exact_renames` |
| `CBattleState.o` | `zero_data_range` |
| `dw_Window.o` | `repack_after_drop` |
| `code_80296898.o` | `retarget_relocs` |
| `db_assert.o` | `repack_after_drop` |
| `snd_PlayerHeap.o` | `exact_renames` |
| `CLibCriMoviePlay.o` | `set_data_align` |
| `lyt_bounding.o` | `zero_nobits` |
| `CDeviceFileJob.o` | `repack_after_drop` |
| `snd_VoiceManager.o` | `pad_data_section`, `exact_renames` |
| `snd_WaveSound.o` | `pad_data_section` |
| `CActorParam.o` | `drop_data_range` |
| `OSReset.o` | `pad_data_section` |

After the prune a re-run reports **0 redundant fields** over the remaining 88
in-scope entries: every remaining payload byte is load-bearing on the current
object (and therefore on the link). Claims recorded per key with
`--owner category-b`.

One more key was retired in this pass: **`OSRtc.o`** — the source `OSScb`
struct declared two trailing `UNKWORD` placeholders (`WORD_0x50`,
`WORD_0x54`), both unreferenced; retail's `.bss` slice is 0x54, so the second
one was extra. Removing it makes the raw gate MATCH (was `FAIL` `.bss` 0x54 vs
0x58) and `drop_nobits_range` a verified no-op; `hexdiff --all` 9/9 functions.

## 4c. The rest of the stale-pad cluster — three distinct verdicts

The remaining `drop_nobits_range` cases are *not* one class:

| key | shape | verdict |
|---|---|---|
| `CtrlNpc.o` | `.sbss2` (0,4) on a section that no longer exists | field pruned (§4b) |
| `OSRtc.o` | extra source placeholder field | source fix, key retired (§4b) |
| `CWorkThread.o` | `.sbss`/`.sdata` 8-byte object at +8 vs retail +4 | **code-shape handoff**: MWCC aligns any 8-byte `.sbss`/`.sdata` object to 8 even for `float[2]`/`void*[2]`, and `__attribute__((aligned(4)))` is ignored (probes `t10`–`t13`); retail's +4 means its object was two 4-byte statics, so the fix would have to split `lbl_eu_80665598[2]` into two variables and re-shape every indexed use — a function-matching change, not a layout tweak |
| `CActorParam.o` | `.sbss` (0,4) from two function-local `sv` guard statics | **code-shape handoff**: retail's functions have no local statics; the fix is in `CActorParam_accumulateTension`/`UnkVirtualFunc159` themselves (unit is `linked False` today) |

## 5. A's `set_data_align` handoff — the premise is wrong (22 entries remain)

A §5 concluded: "the retail start addresses prove the retail input alignment was
4, so fix the source declaration". A §5 also proved that *removing* the rule in
the honest link moves the section. Both observations are confirmed here, and
together they show the source cannot express the fix:

**Probe 1 — MWCC emits align 8 for every probed case** (`.scratch/unitrules_b/probe/`,
compiled with a real unit's `cflags` via `.scratch/unitrules_all/compile_probe.py`):

| source | section | size | align |
|---|---|---|---|
| `float a=1.0f; float b=2.0f;` | `.sdata` | 8 | **8** |
| `double d=1.0; float f=1.0f;` | `.sdata` | 12 | **8** |
| `struct {char a,b,c;} s={1,2,3};` | `.sdata` | 3 | **8** |
| `char buf[7];` | `.sbss` | 7 | **8** |
| `int a[3]={1,2,3};` | `.data` | 12 | **8** |

There is no content-dependent or attribute-free way to make MWCC emit 4, so
"the source declaration decides" has no source-level expression.

**Probe 2 — mwldeppc honours input alignment in a full link** (same LCF as
`main.elf`, `.scratch/unitrules_b/link/`): two objects with 12-byte `.data`:

```
a.o (align 8) + b.o (align 8) -> a1 @0x80004020, b1 @0x80004030  (.data 0x1C)
a.o (align 8) + b.o (align 4, objcopy --set-section-alignment) ->
                                 a1 @0x80004020, b1 @0x8000402C  (.data 0x18)
```

So the field is load-bearing for the reconstructed LCF, and the retail 4-mod-8
slice starts are most plausibly the result of the original LCF placing units
explicitly rather than relying on input alignment. **This is an inference, not a
proof** — it assumes the original inputs were also align 8 (our compiler and
flags cannot emit 4, and the unowned gaps before 8-aligned slice starts, e.g.
`usb.c` 7 B and `dvdfs.c` 0x18 B, show the original objects demanded 8 for at
least those sections) and that the original toolchain behaved like ours. The
alternatives — an original compiler/flag set that emitted 4, or a different
original linker version — cannot be excluded from the DOL alone, because the
original objects and build script are not in the repo.

One such alternative **was tested and refuted**: a partial link (`mwldeppc -r
-sdata 0 -sdata2 0`, the way a multi-object build might merge sections first)
still pads per input alignment — two 12-byte `.data` objects land at `+0x0` and
`+0x10`, not `+0x0` and `+0xC` (`.scratch/unitrules_b/plink/`). So the packing
mechanism is not partial linking.

What is *proven* regardless of the history: our source cannot express the
retail alignment, our link pads per input alignment in both full and partial
links, and deleting the rules moves the sections. **Verdict: not a source-shape
fix; a tooling/LCF-fidelity question (or an accepted exception).** Honest
options:

1. move the emulation into the ldscript generation (`configure.py` writes
   `build/us/ldscript.lcf` from `splits.txt`), i.e. place each unit's input
   sections by the retail address/order so input alignment cannot add padding —
   then all 25 entries can go at once; or
2. keep them as owner-approved §17.6 exceptions with `"policy_exception": true`
   (the per-entry address evidence already exists in
   `.scratch/unitrules_a/align_audit2.py`).

Recommendation: (1) is the only outcome that retires the entries; it is a
tooling change with a project-wide blast radius and needs the coordinator's
sign-off before anyone edits `configure.py`.

### 5b. Per-entry evidence table for the exception packet (regenerated by
`.scratch/unitrules_b/align_entries.py`)

`rule`/`decomp` are the sh_addralign the rule sets vs the built object's header;
`retail start` is the unit's slice start from `config/us/splits.txt` and `%8`
its residue. Current state: **22 entries / 26 (section, align) pairs, all
effective** — 25 pairs are `rule 4` vs `decomp 8` with a retail slice start
≡ 4 (mod 8), and one (`snd_StrmPlayer.o .bss`, rule 8 vs decomp 32) has a
0-mod-8 start, i.e. it is gate/align bookkeeping rather than a placement fix.
The three `no-op` pairs that used to be dead weight inside live fields were
pruned 2026-09-28 (CDeviceGX's whole `set_data_align` disappeared because both
its pairs were no-ops, `code_804B2FF0`'s `.bss` pair was removed).

```
set_data_align entries: 23
key                                    section  rule decomp retail start  %8       uses EDS
CDesktop.o                             .sbss       4      8   0x806656B4   4  effective 
CDeviceGX.o                            .data       8      8   0x8056C960   0      no-op Y
CDeviceGX.o                            .sbss       8      8   0x80665698   0      no-op Y
CETrail.o                              .sbss       4      8   0x806659BC   4  effective Y
CMdlAnmUV.o                            .sdata      4      8   0x80663C74   4  effective 
CMdlDynamics.o                         .data       4      8   0x805701FC   4  effective Y
CMdlMouth.o                            .rodata     4      8   0x805247B4   4  effective Y
CNReqtaskSave.o                        .rodata     4      8   0x805245D4   4  effective 
CNReqtaskSaveBanner.o                  .rodata     4      8   0x80524894   4  effective 
CPackItem.o                            .rodata     4      8   0x805246FC   4  effective 
CScnFilter.o                           .rodata     4      8   0x805240AC   4  effective Y
CSysWinSelect.o                        .sbss       4      8   0x80663FDC   4  effective 
CWorkThread.o                          .rodata     4      8   0x80522474   4  effective 
CWorkThread.o                          .sdata      4      8   0x8066351C   4  effective 
CWorkThread.o                          .sbss       4      8   0x80665594   4  effective 
CfCam.o                                .bss        4      8   0x80570A2C   4  effective 
CfCam.o                                .sdata2     4      8   0x8066629C   4  effective 
UnkClass_8046368C.o                    .data       4      8   0x8056D71C   4  effective 
adx_suwii.o                            .rodata     4      8   0x80519744   4  effective 
adx_tlk.o                              .bss        4      8   0x805E26D4   4  effective 
ax_rna.o                               .rodata     4      8   0x8051914C   4  effective 
code_80468434.o                        .sbss       4      8   0x8066576C   4  effective 
code_804B2FF0.o                        .data       4      8   0x8056F3FC   4  effective 
code_804B2FF0.o                        .bss        8      8   0x8065D138   0      no-op 
code_804B2FF0.o                        .sbss       4      8   0x80665944   4  effective 
code_804BC9EC.o                        .bss        4      8   0x8065F32C   4  effective 
code_804BD8E8.o                        .sbss       4      8   0x8066597C   4  effective Y
snd_StrmPlayer.o                       .bss        8     32   0x8064FE00   0  effective Y
yvm2.o                                 .data       4      8   0x8056F014   4  effective 
```

The 6 `EDS` rows also carry an `extern_data_sections` payload: the
`set_data_align` field is B's (§5), the payload is category D's (see
`d_data_architecture.md`).

## 6. Triage of the remaining 88 in-scope entries (27 live)

Raw-gate sweep (`.scratch/unitrules_b/gate_all.py`, in-process
`check_data_sections` over every in-scope unit): **23 MATCH / 72 FAIL / 1
no-object** (numbers from the 96-entry state before the §4b prunes; the gate is
a per-unit property, so retirements only change the population). Failure
classes: 86 `size`, 7 `align` (4 entries are align-only: `CDesktop.o`,
`CSysWinSelect.o`, `code_80468434.o`, `code_804B2FF0.o`), 0 `bytes`. Final
field census of the scope (`scope_census.txt`):

```
field                            entries
drop_data_tail                        37
exact_renames                         22
set_data_align                        15 (of 22 in the table; 6 carry EDS)
drop_text_symbols                     16
drop_data_range                       16
pad_data_section                      15
retarget_relocs                       13
pool_patterns                         10
drop_text_symbols_as_undef             5
add_symbols / data_pool_patterns       4 each
repack_after_drop                      4
...
```

**The dominant finding: the bytes these rules remove are live in the emitting
unit.** `.scratch/unitrules_b/dropped_refs.py` checks every `drop_data_tail` /
`drop_data_range` / `trim_sdata2_size` payload against the object's own reloc
set:

```
CTaskManager.o      dropped=2  referenced=2   (__vt__48CTTask<...CRootProc>, ...)
CfGimmick.o         dropped=8  referenced=8   (.sdata2 @14167..@14229)
CActorParam.o       dropped=23 referenced=23  (.sdata2 @14653..@16209)
CDevice.o           dropped=14 referenced=13
CDeviceVI.o         dropped=7  referenced=7
sfd_tim.o           dropped=11 referenced=0   (trailing .rodata strings)
CScnItemCameraNw4r  dropped=1  referenced=0   (__vt__18CScnItemCameraNw4r)
...
```

Two sub-classes, with different owners:

| sub-class | examples | honest retirement path |
|---|---|---|
| constants used by this TU's own (still unmatched) code | the `.sdata2`/`.rodata` `@N` tails (`CfGimmick`, `CActorParam`, `CtrlNpc`, `CfObjectEne`, `I…`, `CItemBoxInfo`, …) | match the functions that emit them (code work); the rule is only cosmetic on the *linked* bytes |
| weak template vtables / RTTI structs the retail linker GC'd or that live in a shared data object | `__vt__23reslist<…>`, `__vt__29_reslist_base<…>`, `__RTTI__*`, `__vt__Q44nw4r3snd…`, `ut_RomFont`/`CScnItemCameraNw4r` vtables | D's data-ownership model (`extern "C"` declarations / `extern_data_sections`) — B must not delete `extern_data_sections` from a unit whose source still defines the data (prompt §Must not) |

`drop_text_symbols` / `drop_text_symbols_as_undef` / `trim_text_size` /
`repack_after_drop` (13 in-scope entries after the prunes) were investigated with
`.scratch/unitrules_b/text_gc.py` and `.scratch/unitrules_b/retail_defines.py`:

* **the retail split object never defines any of the dropped symbols** (every
  one of the ~250 checked pairs is `decomp_def=1 retail_def=0`), so the rule is
  not emulating "the retail object had the weak copy and the linker collapsed
  it" — the retail *object* references them as UNDEF;
* the dropped names are of three kinds:
  * decomp-side forced stubs (`FORCEACTIVECGame_cpp_wkStandbyLogin__Fv`,
    `dummy__Q24nw4r2dbFPQ34nw4r2ut10CharWriter`, `__ct_CTask_CScn_name_carrier_N`)
    whose .text bytes are dropped after they shape a data pool — removing them
    means replacing the stub with a direct data definition (per-unit work);
  * weak inline-virtual dtors/templates (`__dt__Q34nw4r2ut5ColorFv`,
    `__dt__Q34nw4r2ut12LinkListNodeFv`, `LinkList<...>` dtors) — the codebase
    already has the pattern for this (see the `IWorkEvent` header note: dtor
    declared out-of-line, per-TU `IWORK_EVENT_INLINE_DTOR` opt-in, strong copy
    placed in `CTaskGame.cpp`);
  * functions the retail split assigns to *another* unit
    (`Panic__Q24nw4r2dbFPCciPCce` is owned by `monolib/src/util/CErrorWii.cpp`
    in retail, not by `nw4r/src/db/db_assert.cpp`) — a code-ownership
    (dissolution) question, the code-side analogue of `extern_data_sections`.

**No `drop_text_symbols` entry is retireable by a B-side layout change**; they
are per-unit source/ownership work. Their rewritten object is also the
remaining link blocker (§7.4).

`pad_data_section` / `pad_sdata2_size` / `pad_text_size` (15 in-scope entries)
are the "padding the retail linker produced after the last object" class. Note
that `pad_data_section` is the *inverse* counterpart of the batch-2 source
fixes: where the retail slice is *larger* than the source object (e.g.
`HBMAxSound .rodata` 0x38 vs 0x34, `synpitch .rodata` 0x18 vs 0x14,
`ut_TextWriterBase#Q36nw4hbm2ut .rodata` 0x18 vs 0x14), the missing bytes are
zeros the retail object really had — the honest fix is a source definition of
that data, and only if the content is unknown should the pad stay.

### 6b. Per-entry verdicts (all 88 entries annotated in the manifest)

`.scratch/unitrules_b/verdicts.py` emits `verdicts.json`/`verdicts.txt` and
every in-scope entry was claimed in the frozen manifest with its verdict as the
`reason` (`--owner category-b --class layout`), so `--report` now resolves the
remaining scope without re-deriving it. The machine-readable **per-entry
packets** — dropped symbols, whether any link input defines them, section
deltas, retail tail symbols — are in
`docs/evidence/decomp/unit_rules_category_b_packets.md` (generated by
`.scratch/unitrules_b/packets.py`):

| verdict | entries | meaning |
|---|---|---|
| HANDOFF-CODE | 32 | rule drops content this TU's own (still unmatched) code emits |
| EXCEPTION-CANDIDATE | 19 | `set_data_align`-only or retail-slice-larger padding: not source-expressible, needs owner approval (PLAN.md §17.6 table) |
| HANDOFF-D | 15 | rule drops unreferenced weak vtables/RTTI/zero-init objects owned by the shared retail data objects (category D) |
| HANDOFF/TOOL | 13 | weak-symbol emission vs retail GC; needs the `IWorkEvent`-style out-of-line pattern per unit |
| OPEN/mixed (hand-assigned) | 8 | dissolved-TU vtable retargets (`CDesktop`, `code_804B2FF0`), `zero_nobits` data ownership (`code_80296898`), shared-data retargets (`ut_DvdFileStream`, `ut_RomFont`, `CDeviceVI`, `CModelDispMakeCrystal`, `ax_rna`) |
| no source object | 1 | `CCol4.o` (link uses the retail split object) |

## 7. Link blockers (link now error-free; the residue is a tool/linker limit)

The full link is red because of the rule table's *own* output, not the
burn-down. §7.1–§7.3 were found and **fixed**; §7.4 is the remaining blocker
(a mwldeppc/wibo limitation whose exit is source-side, §B1).

### 7.1 FIXED — `CDeviceGX::cacheInstance` missing `extern` + stale `.sbss` pad

`libs/monolib/src/lib/CLibCriMoviePlay.cpp:57` declared
`CGXCache* cacheInstance__9CDeviceGX;` inside `extern "C" {}` **without
`extern`**, making it a tentative definition (`.sbss+4`). The *retail* split
object references the name as an UNDEF (4 × `R_PPC_EMB_SDA21` relocs at
`.text+0x140/0x16C/0x1C8/0x1E4`) and defines no `.sbss` at that offset, while
the decomp object defined it and collided with `CDeviceGX.cpp`'s
`extern "C" CGXCache* cacheInstance__9CDeviceGX = nullptr;`.

Fixes applied:

* `extern` added in `CLibCriMoviePlay.cpp`;
* the 4 retail bytes at `0x806656E4` are referenced nowhere in the DOL, so the
  unit's slice tail is linker padding: `config/us/symbols.txt`
  (`lbl_eu_806656E0` `size:0x8`→`0x4`) and `config/us/splits.txt`
  (`.sbss end:0x806656E8`→`0x806656E4`) now leave them as an unowned gap (same
  model as `CDeviceVI.o`'s existing 3-byte gap); re-split applied.

Verification: `run.py data diff monolib/src/lib/CLibCriMoviePlay` → **MATCH**
(`.sbss` 0x4/0x4, align 8/8, every other section identical); `hexdiff --all`
unchanged (14/22 — the unit's pre-existing state); the
`CDeviceGX::cacheInstance` multiply-defined link error is gone.

### 7.2 FIXED — `cf::CHelp_OpenPartyMenu::isPartyMenuReady()` mis-named in `symbols.txt`

`config/us/symbols.txt` mapped the *same* mangled name to `.text:0x802BAF5C`
(72 bytes, inside `CHelp_ClosePartyMenu.cpp`) and `.text:0x802BB00C` (8 bytes,
`CHelp_OpenPartyMenu.cpp`); both split objects are linked and both exported it
globally. Disassembly shows the 0x48-byte function is a *different* function
(`r = (PTNotice_IsActive_3C10() != 0) || (menuPTStateIsActive() != 0)`, no
`this` use), so the `0x802BAF5C` entry is now
**`isPartyMenuReady__Q22cf20CHelp_ClosePartyMenuFv`** (same
`Q22cf20CHelp_ClosePartyMenuFv` qualifier as the neighbouring
`checkHelpCondition__Q22cf20CHelp_ClosePartyMenuFv`). The re-split also fixed a
latent reference bug: `code_802B8A3C.o`'s data table referenced the duplicate
name twice, so one of its two slots resolved to the wrong address; it now emits
one slot per function. The Close-class method itself is still undecompiled
(code-category work).

### 7.3 FIXED — `trim_text_section` rewrote symbols to SHN_ABS and crashed mwldeppc

The 1161-input link SIGSEGV/SIGBUS bisected (then delta-shrunk) to a **two
object** reproducer:

```
build/us/src/RVL_SDK/src/revolution/os/__start.o
build/us/src/monolib/src/work/CWorkSystemMem.reloc.o   # postprocessed
```

`CWorkSystemMem.reloc.o` is the only postprocessed object involved: the rule
`trim_text_size=0x160` cuts `.text` 0x178→0x160, and the toll then rewrote the
symbol that crosses the cut (`wkStandbyLogout__14CWorkSystemMemFv`) to
**SHN_ABS** with its section-relative value (0xE0) and size 0, while two
surviving relocs (`.data+0x98` vtable slot, `extabindex+0x18`) still target it.

A/B on variants of the raw object (`.scratch/unitrules_b/ab_variants.py`; pair
linked with the real LCF):

| variant | `.text` | symbol rewrite | link result |
|---|---|---|---|
| raw | 0x178 | — | rc=1 (clean errors) |
| a | 0x160 | none (symbol still crosses the end) | rc=1 |
| b | 0x160 | UNDEF (shndx 0, value 0, size 0) | **SIGSEGV** |
| c | 0x178 | ABS (0xFFF1, value 0xE0) | **SIGSEGV** |
| d | 0x160 | stays `.text` FUNC, size 0 | rc=1 |
| e = tool output | 0x160 | ABS, value 0 | **SIGSEGV** |

**Fix applied** (`tools/postprocess_reloc_names.py`, `trim_text_section`): zero
`st_size` and keep the symbol defined in `.text` (clamping `st_value` to the new
section size when it is entirely past the cut) instead of rewriting it to
`SHN_ABS`. The 2-object reproducer now links (rc=1), and
`tools/coop/tests` (457 tests) stays green. This is also the *retail-correct*
shape: the trimmed function is still owned by the unit.

### 7.4 OPEN — the `drop_text_symbols*` rewrite has no linkable object shape

Eleven link inputs carry the same rewrite for *reloc-targeted* weak symbols
(the 9 ABS rows below are over-produced *tail* functions with **no definer** in
the link; the 2 UNDEF rows are units that need the inline base body for a
matched dtor shape, §7.5)
(scan: `.scratch/unitrules_b/abs_scan.py`): 9 objects with ABS FUNCs from
(`dvd_broadway`, `lyt_*`, `g3d_scnobj`, `CLib`,
`HBMAnmController`, …) and 2 with UNDEF FUNCs from
`drop_text_symbols_as_undef` (`CDeviceGX`, `CLibStaticData`). Full-link A/B
(`abs_ab.py`, `undef_ab.py`, `undef_abs_ab.py`):

| rewrite of the 11 symbols | full link |
|---|---|
| keep ABS (current table) | **SIGSEGV** |
| UNDEF (retail object's shape) | **SIGSEGV** |
| keep the definition (shndx restored, size 0) | rc=1 — but `internal linker error: File: 'ELF_linker.c' Line: 7182` |
| keep the definition + the two duplicate definitions renamed | rc=1 — same internal error, no multiply-defined |

So for these 11 objects there is **no symbol shape that both links and keeps
the retail (UNDEF) reference**, and the retail-correct shape crashes. This is a
mwldeppc/wibo limitation, not a burn-down bug; the way out is to remove the
*need* for the rewrite (the `IWorkEvent`-style out-of-line pattern per unit,
§6), after which no symbol surgery is required. A synthetic probe of the
linker's undefined-FUNC path is the next step for whoever owns the toolchain
(wine vs wibo is untested here).

Consequence for B: DOL-byte neutrality still could not be demonstrated by two
linked DOLs (the link stops at 7.4 + 7.2). The retirements in §3/§4 do not need
it (bit-identity at the link-input level), but any future `set_data_align`/LCF
change does.

### 7.5 B1 progress — one source fix landed, with the recipe proven (2026-09-28)

**`CArcItem.o` is fixed** (the `drop_text_symbols` field is retired):
CArcItem's dtor is a **hand-written `extern "C"` free function** that never
calls `~IWorkEvent`, so the TU does not need the inline-empty base body at all.
Removing `IWORK_EVENT_INLINE_DTOR` from `CArcItem.cpp` makes the vtable
sub-slot stay an **UNDEF reference** to `CTaskGame.cpp`'s strong copy, and MWCC
no longer emits the weak 0x40 stub. Measured with the table applied:

| | `.text` | `__dt__8CArcItemFv` | extab | extabindex |
|---|---|---|---|---|
| old pipeline (stub + `drop_text_symbols` + repack) | 0x294 | 0x9C+0x80 | 60 | 60 |
| new pipeline (no stub + repack) | **0x294** | **0x9C+0x80** | 52 | **48** |
| retail split object | 0x294 | 0x9C+0x80 | 72 | 48 |

So the change is equivalent on `.text`/dtor placement and *better* on the unwind
tables: the rewrite used to leave the dropped function's `extab` (+8 B) and
`extabindex` (+12 B) entries orphaned in the object — the data gate never
checks those sections (`DATA_SECTIONS` excludes them) but the linked DOL does
carry them, so **the other ten rewritten objects have the same defect today**.

**Negative result (important for the remaining twelve):** the same macro
removal is *wrong* where the dtor is compiler-generated — MWCC then emits the
out-of-line base-dtor call and the dtor grows by 0xC, breaking a matched
function. Verified and reverted: `CLibStaticData` `CItem` dtor 0xC4 → 0xD0 and
`CPackItem` dtor 0xAC → 0xB8. Those two units need the inline body for their
shape, so their `drop_text_symbols_as_undef` rewrite is not removable by this
recipe; the same applies to the remaining `lyt_*`/`g3d_*`/`CLib`/`dvd_broadway`
rows (over-produced tail functions, `definers=0`, whose owner TU must be found
and given the definition).

### 7.6 Per-row diagnosis of the remaining B1 rows (`.scratch/unitrules_b/tail_diag.py`)

Three distinct mechanisms, all measured on the current objects:

**(a) Interleaved unreferenced weak inline-dtor copies** — `lyt_drawInfo`,
`lyt_group` (×2), `lyt_window`, `lyt_arcResourceAccessor`, `HBMAnmController`.
Example `lyt_drawInfo.o`:

```
ours   .text 0x100:  __ct__DrawInfo @0x00+0x7C, __dt__Rect @0x80+0x40 (WEAK), __dt__DrawInfo @0xC0+0x40
retail .text 0x0C0:  __ct__DrawInfo @0x00+0x7C,                            __dt__DrawInfo @0x80+0x40
```

The copy is interleaved between the ctor and the dtor (so the postprocess drop
is *correct*: it yields `.text` 0xC0 and the dtor at 0x80 = retail), it is
**not referenced** by any reloc *and not branched to* (`objdump` shows no `bl
0x80`), and the unit's real functions are 2/2 matched. MWCC emits the copy
because the inline-empty dtor is odr-used (a `Rect` member destroyed somewhere
in the TU) — a minimal probe (`struct S { ~S() {} int a; }; ... S s;`) emits
**no** copy, so the trigger is TU-specific and must be found and removed source-
side (or the dtor given an out-of-line home). Until then the rule's byte drop is
load-bearing and the symbol rewrite crashes the linker (§7.4).

**(b) Base-vtable ownership drift** — `g3d_scnobj` (`ScnLeaf`), `lyt_animation`
(`AnimTransform`). Our object emits the base class's vtable *and* its weak dtor
(our `.data+0xA0` / `.data+0x60` reference them); retail's object has neither
(its same slots belong to `ScnObj` / `AnimTransformBasic`). Retail's `ScnLeaf`
methods *are* in `g3d_scnobj.o`, but its vtable is not — so retail's key function
(`~ScnLeaf`) is defined in another TU, which emits the vtable there. Fix = move
the dtor definition (and with it the vtable) to the retail-owning TU.

**(c) Functions absent from the DOL** — `CLib::createLibs` (GLOBAL, 0x158) and
`dvd_broadway`'s four (`DVDLowOpenPartitionWithTmdAndTicket`, `DVDLowGetCoverStatus`,
`DVDLowGetCoverReg`, `initDvdContexts`; GLOBAL). The masked byte search finds no
copy anywhere in the DOL, and `CLib`'s own `extabindex` site corresponds to
retail's `wkStandbyLogout__4CLibFv` — so our TU's function order/content differs
from retail's; these need matching (most likely retail inlined them, or they
live in another TU with different code).

Link-crash set: the 9 objects above (ABS rewrite) plus the 2 `as_undef` objects
(`CDeviceGX`, `CLibStaticData`).

### 7.7 The weak-copy emitter is out-of-line `__dt__` member destruction

Probe (`.scratch/unitrules_b/probe/d3.cpp`, compiled with the unit's own cflags):

```cpp
struct S { ~S() {} int a; };
struct C { virtual ~C(); S s; };
C::~C() {}
extern "C" void __start() { C c; (void)c; }
```

MWCC emits **both** `__dt__1CFv` (the real dtor) and an unreferenced weak
`__dt__1SFv` copy (0x40). Variants: a *trivial* inline dtor (`struct S { int a; }`)
or an implicit/inline containing dtor (`struct U { S s; }; void f() { U u; }`) emit
**nothing**. So the rule for the whole family is:

> an orphan weak copy exists iff some **out-of-line** `__dt__` destroys a member
> whose class has a **user-provided inline dtor**.

That splits the rows into two classes:

**Fixable source-side (bogus inline dtors on trivially destructible types):**

- `lyt_drawInfo` (hbm **and** nw4r): `ut::Rect`'s inline-empty `~Rect() {}` was
  invented by an earlier pass; upstream `ut::Rect` has no dtor and no
  `__dt__Rect` exists anywhere in the DOL. Removed from
  `nw4hbm/ut/ut_Rect.h` and `nw4r/ut/ut_Rect.h`. Result: object `.text`
  0x100 → **0xC0** = retail, `__dt__DrawInfo` back at retail's 0x80, 2/2 funcs
  matched, gate MATCH *without* `drop_text_symbols` (field retired).
- `lyt_window`: **experiment negative, reverted.** `Window::Content`'s
  `~Content() {}` is *not* removable: the wrapper's body is a `__destroy_arr`
  of `vtxColors[4]` (`ut::Color` is deliberately non-trivial in this
  reconstruction — its strong ctor/dtor live in `lyt_material`/`lyt_bounding`
  per retail evidence), so MWCC re-emits the same 0x64 wrapper from the
  implicit dtor even after the inline body is deleted. It is a genuine
  retail-linker-GC orphan; header and rule (`drop_text_symbols` +
  `repack_after_drop`) were restored.

**Not fixable source-side (retail-linker-GC orphans):** for the polymorphic
bases the inline virtual dtor is *required by retail's own matched bytes*:

- `HBMAnmController` / `FrameController`: removing the inline virtual dtor
  regressed `do_calc__GroupAnmController` **3/3 → 2/3** (the base vtable slot is
  part of the matched instruction stream). Rule kept, with the A/B result
  recorded in the table.
- `lyt_animation`/`lyt_layout` / `AnimTransform`: removing it regressed
  `lyt_layout` **13/15 → 11/15**. Rule kept.
- `lyt_group` (×2) / `lyt_arcResourceAccessor` / `lyt_layout` / `LinkList<T,N>`
  and `g3d_scnobj` / `ScnLeaf`: same shape — the implicit template/base dtor is
  semantically required, the orphan only disappears if the original *linker*
  dead-stripped it. `mwldeppc -help search=dead|weak|garbage` has **no GC
  option** (only `-strip_partial`), so these stay in the postprocess drop list.
- `CLib::createLibs` and `dvd_broadway`'s four are a different row type
  (functions absent from the DOL; matching work, no byte copy anywhere).

Pipeline nuance confirmed while verifying: `build/us/src/X.o` is the **raw**
decomp object; the gate compares it against retail by default and
`--postprocess` retries with UNIT_RULES, while the link gets the reshaped
`X.reloc.o`. The *source* fix above makes the raw object already retail-shaped
(`hexdiff` split line: PASS 0xC0/0xC0 for `lyt_drawInfo`, and PASS 0xB4/0xB4 for
its nw4r twin), which is what removes the ABS symbol rewrite (and therefore the
link crash) for those units. Crash set after this round: **11 → 9 objects**
(`CArcItem` §7.5, `lyt_drawInfo` hbm+nw4r §7.7); the remaining seven drop rows
are GC orphans, the two `CLib`/`dvd_broadway` rows are absent-from-DOL matching
work, and `CDeviceGX`/`CLibStaticData` still need their base-dtor ownership
restructured.

## 8. Manifest / claims

* `--ratchet` three times: `baseline_count 429 → 422 → 414 → 413`;
  the payload changes recorded were `HBMAxSound.o.drop_nobits_range` and the 22
  §4b field prunes.
* 118 manifest keys are `--owner category-b`: 16 retired keys, 95 §6b verdict
  annotations, 7 EDS-bearing `set_data_align` keys (B's field, D's payload).
  No in-scope key is left unowned.
* `check_unit_rules_frozen.py --check` → OK (`current 413, baseline 413,
  retired 102`).
* `configure.py` regenerated `build.ninja` + `objdiff.json`: the link now
  rewrites **85** objects instead of 99; all 16 retired keys' live units use
  their raw `.o`.
* Tool change: `trim_text_section` no longer writes `SHN_ABS` (§7.3); the
  coop test suite (457 tests) passes.
* Source/config fixes outside the table: `CLibCriMoviePlay.cpp` `extern`,
  `config/us/symbols.txt` (two entries: the Close-class rename + the
  `lbl_eu_806656E0` size) and `config/us/splits.txt` (one `.sbss` end), followed
  by a `dtk dol split --no-update` re-split (§7.1/§7.2). The full link now
  reports **0 multiply-defined errors** (was 2) and still SIGSEGVs on §7.4.

## 9. Handoffs / residuals

| residual | owner | evidence |
|---|---|---|
| 22 `set_data_align` entries: LCF-fidelity emulation, not source-fixable | **human decision** (tooling change in `configure.py`/ldscript generation, or §17.6 exception in PLAN.md) | §5 probes; `align_audit2.py` addresses |
| 32 HANDOFF-CODE entries: drops live content emitted by unmatched functions | code matching | `verdicts.json`, `dropped_refs.py` |
| 15 HANDOFF-D entries: weak vtables/RTTI/zero-init objects owned by shared data | D (data ownership) | `verdicts.json`; prompt §Must not |
| 13 HANDOFF/TOOL entries: weak-symbol emission vs retail GC; per-unit out-of-line dtor pattern; also the §7.4 blocker | B (per-unit source work) + toolchain owner | `text_gc.py`, `retail_defines.py` |
| 6 mixed/OPEN entries (dissolved-TU vtable retargets, `zero_nobits`, shared-data retargets) | code/D | `verdicts.json` |
| `CWorkThread.o` / `CActorParam.o` no-op vs alignment/static-local shape | code matching | `verdicts.json`, §4c probes |
| `CLibCriMoviePlay.cpp` missing `extern` + `.sbss` pad bookkeeping | **done** (source + `symbols.txt`/`splits.txt`, gate MATCH, §7.1) | — |
| `symbols.txt` duplicate `isPartyMenuReady` (link blocker) | **done** (renamed + re-split, duplicate gone, §7.2) | — |
| `drop_text_symbols*` symbol rewrite (link blocker) | toolchain owner; removal path is source-side (§7.4, B1 packets) | `abs_scan.py`, `abs_ab.py`, `definers.py` |
| `trim_text_section` SHN_ABS crash | **done** — fixed in the tool (§7.3) | `ab_variants.py` |
| `CCol4.o` (the one no-object in the scope) | triage when the unit is linked from source | `gate_all.json` |

## 10. Reproduction

```bash
# census / scope
.venv/bin/python3 .scratch/unitrules_b/census.py
# rule-effect audit (bit-level A/B over every in-scope key)
.venv/bin/python3 .scratch/unitrules_b/effect_audit2.py
.venv/bin/python3 .scratch/unitrules_b/field_prune.py          # per-field redundancy
.venv/bin/python3 .scratch/unitrules_b/noop_verify2.py usb.o dvdfs.o OSError.o \
    bte_logmsg.o HBMAxSound.o gap_utils.o OSNandbootInfo.o scapi_prdinfo.o CSysWinSave.o
# raw data gate + per-entry verdicts
.venv/bin/python3 .scratch/unitrules_b/gate_all.py
.venv/bin/python3 .scratch/unitrules_b/verdicts.py
.venv/bin/python3 .scratch/unitrules_b/annotate.py             # --claim every verdict
.venv/bin/python3 .scratch/unitrules_b/packets.py              # per-entry packets doc
# are the dropped bytes live? does the retail object define them?
.venv/bin/python3 .scratch/unitrules_b/dropped_refs.py
.venv/bin/python3 .scratch/unitrules_b/text_gc.py
.venv/bin/python3 .scratch/unitrules_b/retail_defines.py
# set_data_align / MWCC alignment evidence
.venv/bin/python3 .scratch/unitrules_all/compile_probe.py monolib/src/scn/CMdlAnmUV.cpp \
    .scratch/unitrules_b/probe/t6.cpp .scratch/unitrules_b/probe/t6.out   # -> .sbss align 8
# mwldeppc input-alignment A/B
build/tools/wibo build/compilers/Wii/1.1/mwldeppc.exe -fp hardware -nodefaults \
    -lcf .scratch/unitrules_b/lnk/real.lcf -o /tmp/ab.elf \
    .scratch/unitrules_b/lnk/a.o .scratch/unitrules_b/lnk/b.o
# link crash: bisect + shrink + minimal pair
.venv/bin/python3 .scratch/unitrules_b/bisect_link.py prefix 1033
.venv/bin/python3 .scratch/unitrules_b/shrink.py
.venv/bin/python3 .scratch/unitrules_b/bisect_link.py run .scratch/unitrules_b/bisect/pair.rsp
.venv/bin/python3 .scratch/unitrules_b/ab_variants.py         # §7.3 A/B table
.venv/bin/python3 .scratch/unitrules_b/abs_scan.py            # §7.4 affected inputs
.venv/bin/python3 .scratch/unitrules_b/undef_abs_ab.py       # §7.4 full-link A/B
# freeze gates
.venv/bin/python3 tools/check_unit_rules_frozen.py --check --report
.venv/bin/python3 -m unittest discover -s tools/coop/tests -t .
.venv/bin/python3 configure.py
```
