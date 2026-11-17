# extab / extabindex audit (2026-10-24)

## Why

None of the matching gates compared exception tables. `data_match.DATA_SECTIONS` stops at
`.sbss2`, and hexdiff only looks at `.text`. So recent edits checked only as "byte-neutral" on
.text + data could have changed `extab`/`extabindex` without anyone noticing. Examples of such
edits: header consolidation, `extern "C"` to real members, memberization, dtor and
`operator delete` signature changes.

## Method

- Scope: every completed unit in `objdiff.json` (`metadata.complete`, which means `Matching` +
  `MatchingCodeOnly`). That is **616 units**, all with both objects present after a locked
  `ninja -k 0 all_source`.
- Strict check: `tools/coop/lib/data_match.py::check_extab_sections` compares retail
  `unit.target_path` against decomp `unit.base_path`. Per section it checks the size and the raw
  bytes. It also compares relocations keyed by `(offset, type, resolved target)`, where the
  target resolves as follows:
  - Global names resolve to their `symbols.txt` address.
  - In-object targets (`@N` extab labels, local functions) resolve to the linked `.text` base +
    `st_value` when the base can be derived. Otherwise they resolve to `(section, value+addend)`.
  - UNDEF targets resolve through `symbols.txt`/ldscript, else by name.
  
  This means `@N` renumbering never counts as a diff.
- Per-function diagnostic: `extab_function_diffs` ignores order. For each function in
  `extabindex` it checks the function size, the extab record bytes (flags word + cleanup actions),
  and the record's relocs (the dtors called). It separates "the records differ" from "only the
  function/record order differs".
- Regression check: I compared every failing unit's current object with the pre-edit object
  snapshot `.scratch/hdrcons/baseobj/` (taken 2026-10-18 18:40, before the header-consolidation /
  blockers-A/B / extern-C / memberize rounds of 10-23..10-24). I could not look further back,
  because git is off-limits for agents.

## Results

| | count |
|---|---|
| units audited | 616 |
| extab/extabindex mismatches (strict) | **66** |
| ...already identical in the 2026-10-18 snapshot (pre-existing) | **66** |
| ...introduced by the 10-23/10-24 edits (regressions) | **0** |
| fixed in this pass | 0 (no regressions found) |
| remaining (baselined in `docs/evidence/extab_baseline.json`) | 66 |

In all 66 units, the snapshot object's extab is equal to the current one under the strict
comparator. None of the recent source edits changed an exception table in a completed unit.

### Breakdown

- **.text not FULL (32 units).** At least one function is only `EQUIVALENT_MATCH` (register /
  scheduling differences). The extab flags word records the saved-GPR/FPR counts and the frame
  layout, and function sizes go into extabindex. So these diffs follow from the accepted
  equivalence, plus action-record diffs in some units. Fixing them requires FULL_MATCHing those
  functions.
- **.text FULL, order-only (9 units).** Per function, the records are identical, but the
  in-unit function order differs from retail: CCur, CTaskGameEffAfter, CCharVoice,
  CHelp_ClosePartyMenu, CTaskManager, CNReqtaskSaveBanner, CMdlMouth, CScnBlend,
  UnkClass_8047CD0C. This is a `.text` layout issue. The per-function hexdiff hides it, and the
  linked DOL would have its functions in a different order.
- **.text FULL, missing cleanup actions (16 units).** Retail ctors/dtors carry
  `8680001e/f`-style actions that destroy a base/member through a real dtor, usually
  `__dt__10IWorkEventFv` (0x80040858) or `__dt__10IScnRenderFv` (0x80040898). The decomp record
  has no actions. The pattern in the units I inspected (CBattery, CScnIdMan) is that the decomp replaces the real polymorphic base with a manual-vtable
  stand-in. Examples: `CBatteryVtblBase` in `src/kyoshin/CBattery.hpp`, `ISCN_RENDER_NO_DTOR` /
  dtor-less `IScnRender` views, and the CScnIdMan / CSystemWindow / CMenuTitle /
  CMenuSelectShop / CfObjectObj / CfGimmickSaveOff / CREvtMovie / CDeviceFontInfoRom / CLibCri /
  CTaskLOD / CWorkSystemCache / CFade / CfTaskMain / CTaskGamePic / CfObjectPoint bases. These
  stand-ins were introduced to keep weak default dtors out of `.text` and within the split
  budget. They are pre-existing source-shape debt, and extab is the evidence that the original
  had the real base. The fix in each case is to restore the real base class while keeping
  `.text` FULL. That is a per-unit matching task: route it as a near-miss with the next angle
  "real base + dtor placement", not as a gate exception.
- **.text FULL, record presence/naming/size (9 units).** Functions are named or split
  differently from retail, or a record exists on one side only. Examples:
  `CDeviceSC`/`CDeviceGX`/`CLib`/`CLibStaticData` have extra decomp records (weak
  `__dt__11CDeviceBaseFv`-style copies), and `CVS_THREAD_*` has retail-only records for functions
  that the decomp folds or names differently.

## Gate changes

- `tools/coop/lib/data_match.py`: `EXTAB_SECTIONS`, reloc-aware `check_extab_sections`, and the
  order-insensitive `extab_function_diffs`. `_parse` now also reads extab, extabindex and their
  `.relaextab*` relocs. The data-section verdicts are unchanged.
- `tools/coop/extab_gate.py` + `run.py extab-gate [--verbose|--ratchet|--init]`: a shrink-only
  baseline, `docs/evidence/extab_baseline.json`, mapping unit to per-function diff count. A new
  failing completed unit FAILs, as does a rise in a baselined unit's count. `--ratchet` only
  removes units or lowers counts. It reads the built objects, so build first under the lock.
- `run.py data diff <unit>` / `--all`: now also prints and enforces the exception tables. A
  baselined unit reports `MISMATCH (pre-existing ...)` and does not fail.
- `hexdiff.py --all`: an informational `extab:` line, plus an `extab` field in `--json`. The exit
  code is still function-level.
- Tests: `tools/coop/tests/test_extab_match.py` (11 tests on hand-built ELF fixtures: label
  renumbering, missing action, dtor reloc change, size change, order-only, data gate unaffected,
  ratchet semantics).

## Per-unit table

`.text` is from `hexdiff --all` (FULL = every function byte-matched). "per-fn extab diffs" is the
baselined count.

| unit | .text (hexdiff --all) | per-fn extab diffs | kind |
|---|---|---|---|
| `kyoshin/CBattery` | FULL | 1 | action records |
| `kyoshin/CBgTex` | 2 fn not FULL | 2 | action records |
| `kyoshin/CCur` | FULL | 0 | order-only |
| `kyoshin/CFade` | FULL | 1 | action records |
| `kyoshin/CLoad` | 2 fn not FULL | 7 | action records, fn-size, record presence/naming |
| `kyoshin/CSystemWindow` | FULL | 3 | action records |
| `kyoshin/CTaskGamePic` | FULL | 1 | action records |
| `kyoshin/cf/CTaskGameEffAfter` | FULL | 0 | order-only |
| `kyoshin/cf/CVision` | 25 fn not FULL | 22 | action records, fn-size, record presence/naming |
| `kyoshin/cf/CfCollCylinderImpl` | 1 fn not FULL | 1 | fn-size |
| `kyoshin/cf/CfGimmick` | 6 fn not FULL | 6 | fn-size, record presence/naming |
| `kyoshin/cf/CfGimmickItem` | 1 fn not FULL | 12 | action records, record presence/naming |
| `kyoshin/cf/CfGimmickSaveOff` | FULL | 1 | action records |
| `kyoshin/cf/CfScript` | 1 fn not FULL | 4 | action records, fn-size, record presence/naming |
| `kyoshin/cf/CfTaskMain` | FULL | 3 | action records, record presence/naming |
| `kyoshin/cf/CtrlMoveNpc` | 4 fn not FULL | 5 | fn-size, record presence/naming |
| `kyoshin/cf/chain/CChainActorList` | 9 fn not FULL | 7 | action records, fn-size |
| `kyoshin/cf/code_800C17DC` | 3 fn not FULL | 5 | fn-size, record presence/naming |
| `kyoshin/cf/object/CfObjectEne` | 8 fn not FULL | 8 | action records, fn-size |
| `kyoshin/cf/object/CfObjectObj` | FULL | 2 | action records |
| `kyoshin/cf/object/CfObjectPoint` | FULL | 1 | action records |
| `kyoshin/cf/voice/CCharVoice` | FULL | 0 | order-only |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_BEGIN` | 2 fn not FULL | 7 | record presence/naming |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END` | FULL | 7 | fn-size, record presence/naming |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END_SP` | 2 fn not FULL | 7 | action records, record presence/naming |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_CHAIN` | FULL | 6 | record presence/naming |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN` | FULL | 8 | record presence/naming |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_FAINT` | 1 fn not FULL | 0 | order-only |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_PARTY_GAGE` | FULL | 8 | record presence/naming |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_SUDDEN` | FULL | 6 | record presence/naming |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_BREAK` | 2 fn not FULL | 6 | record presence/naming |
| `kyoshin/code_800A75FC` | 4 fn not FULL | 3 | fn-size |
| `kyoshin/help/CHelp_ClosePartyMenu` | FULL | 0 | order-only |
| `kyoshin/menu/CMenuBattleChain` | 1 fn not FULL | 4 | action records, fn-size |
| `kyoshin/menu/CMenuOption` | 1 fn not FULL | 4 | action records, fn-size |
| `kyoshin/menu/CMenuPTState` | 1 fn not FULL | 2 | action records |
| `kyoshin/menu/CMenuQuestLog` | 1 fn not FULL | 2 | action records |
| `kyoshin/menu/CMenuSelectShop` | FULL | 3 | action records |
| `kyoshin/menu/CMenuTitle` | FULL | 1 | action records |
| `kyoshin/realtimeevt/CREvtLight` | 1 fn not FULL | 2 | action records, fn-size |
| `kyoshin/realtimeevt/CREvtMovie` | FULL | 1 | action records |
| `monolib/src/core/CTaskManager` | FULL | 0 | order-only |
| `monolib/src/device/CDeviceFontInfoRom` | FULL | 2 | action records |
| `monolib/src/device/CDeviceGX` | FULL | 1 | record presence/naming |
| `monolib/src/device/CDeviceSC` | FULL | 1 | record presence/naming |
| `monolib/src/effect/CERand` | 4 fn not FULL | 1 | fn-size |
| `monolib/src/lib/CLib` | FULL | 1 | record presence/naming |
| `monolib/src/lib/CLibCri` | FULL | 2 | action records |
| `monolib/src/lib/CLibCriMoviePlay` | 7 fn not FULL | 5 | action records, fn-size |
| `monolib/src/lib/CLibCriStreamingPlay` | 11 fn not FULL | 7 | action records, fn-size, record presence/naming |
| `monolib/src/lib/CLibLayout` | 1 fn not FULL | 1 | action records |
| `monolib/src/lib/CLibStaticData` | FULL | 1 | record presence/naming |
| `monolib/src/lib/UnkClass_80460308` | 1 fn not FULL | 1 | fn-size |
| `monolib/src/lod/CTaskLOD` | FULL | 2 | action records |
| `monolib/src/nand/CNReqtaskSave` | 1 fn not FULL | 2 | record presence/naming |
| `monolib/src/nand/CNReqtaskSaveBanner` | FULL | 0 | order-only |
| `monolib/src/nand/CNand` | 1 fn not FULL | 0 | order-only |
| `monolib/src/scn/CMdlAnmEye` | 1 fn not FULL | 0 | order-only |
| `monolib/src/scn/CMdlMouth` | FULL | 0 | order-only |
| `monolib/src/scn/CScnBlend` | FULL | 0 | order-only |
| `monolib/src/scn/CScnIdMan` | FULL | 2 | action records |
| `monolib/src/scn/CScnLightMan` | 1 fn not FULL | 0 | order-only |
| `monolib/src/scn/UnkClass_8047CD0C` | FULL | 0 | order-only |
| `monolib/src/work/CProcess` | 1 fn not FULL | 1 | action records |
| `monolib/src/work/CWorkControl` | 1 fn not FULL | 0 | order-only |
| `monolib/src/work/CWorkSystemCache` | FULL | 2 | action records |

## Follow-up: order-only units fixed (2026-10-24)

All 14 order-only rows (the 9 `.text` FULL units plus CVS_THREAD_FAINT, CNand, CMdlAnmEye,
CScnLightMan, CWorkControl) now have retail `.text` function order and matching extab. The fix in
each was to reorder the definitions. In CTaskManager, the `CTTask<CRootProc>::Move/Draw`
specializations became `inline`, so MWCC defers them to the weak tail after CRootProc's inline
virtuals, which is where retail has them. The extab baseline was ratcheted. Function order is now
gated separately by `run.py text-order-gate` (`tools/coop/text_order_gate.py`, shrink-only baseline
`docs/evidence/text_order_baseline.json`, tests in `tools/coop/tests/test_text_order_gate.py`). It
covers all completed units, including those without extab, and 71 units are baselined.

## Follow-up: weak-dedup-aware gates and real-base restoration (2026-10-24, base agent)

Restoring the real base (for example `class CBattery : public IWorkEvent`) gives the retail ctor
cleanup action. MWCC then also emits a weak copy of the inline `__dt__10IWorkEventFv`, a weak
`__RTTI__10IWorkEvent`, and that RTTI's local "IWorkEvent" name string. Retail has none of them in
the unit. mwld kept the first copy (in CTaskGame / CGame), discarded this one, and dead-stripped
the name string. Retail split slices and budgets are post-link, so the raw-object gates counted
these copies against the unit. The split went 0x410 against 0x3D0, and `.sdata`, `.rodata` and
extab drifted. That is why the stand-ins existed.

The gates now compare the object as mwld contributes it to the link (PLAN.md §17.8,
`tools/coop/lib/weak_dedup.py`). A symbol is excluded only if it is weak and `symbols.txt` places
the same name in a different split unit. Local data whose every referencing relocation lies in an
excluded range is excluded with it. Retail-only link tail padding (zero bytes, below the section
alignment, no relocation) is accepted in the data compare. Exclusions are always printed. The
weak-dedup and tail-padding rules turned no previously passing unit (data or extab) into a
failure (full scan of every `objdiff.json` unit, raw object without the rules against the linked
view). `link-gate` is unchanged (missing=87, multiply_defined=0). The `@local[CfTaskMain]` FAIL
lines come from another agent's earlier rebuild of CfTaskMain.o, which also happen with the
pre-change CBattery sources.

Effect: CBattery is fixed with its real `IWorkEvent` base and literal strings in place of the
hand-built vtable, RTTI and string pool (10/10 FULL, split PASS, data MATCH, extab MATCH).
CDeviceGX, CDeviceSC and CLibStaticData now pass extab (their extra records were exactly such
weak copies). Per-unit status of the remaining "missing cleanup action" units is in the next
section.

### Per-unit status of the 16 "missing cleanup action" units

Fixed (all functions FULL, split PASS, extab MATCH, ratcheted out of `extab_baseline.json`):

- `kyoshin/CBattery`: real `IWorkEvent` base. Data MATCH.
- `monolib/src/device/CDeviceFontInfoRom`: real `CDeviceFontInfo` base (novtable, inline dtor),
  compiler vtable/RTTI, placeholder method names renamed in us/eu `symbols.txt`. Data MATCH.
- `kyoshin/cf/object/CfObjectPoint`: real inline `virtual ~CfObject()` in place of the
  `destroyObject()` stand-in. Data MATCH.
- `kyoshin/cf/CfTaskMain`: `CTTask<CfTaskMain>, IWorkEvent` bases, compiler adjustor thunk
  (`CfTaskMain_dtorAdj54` renamed to `@84@__dt__Q22cf10CfTaskMainFv`). `.data` now matches;
  `.rodata`/`.sdata` mismatches predate this.
- `kyoshin/CTaskGamePic`: `CTTask<CTaskGamePic>, IWorkEvent, IScnRender` bases, real
  `OnFileEvent`/`cbRenderBefore` virtuals, compiler thunks (5 symbol renames), unit flags
  `-RTTI off -O4,s -func_align 4` in place of per-function pragmas. The `.data` mismatch shrank
  (code-only unit).

Remaining (each needs work across units outside its own TU):

- `kyoshin/CFade`: its ctor is the extern "C" `__ct__CFade`. CMenuKizunagram, CMenuMapSelectSC and
  code_802405F4 construct and destroy CFade by hand through that ctor and `__dt__5CFadeFv`, so they
  have to become real C++ first.
- `kyoshin/CSystemWindow`, `kyoshin/menu/CMenuSelectShop`, `kyoshin/menu/CMenuTitle`: the
  actions destroy the IUIWindow / IWorkEvent / IScnRender bases. The whole menu family models
  `IUIWindow`/`CTTask<IUIWindow>` as raw byte/ptmf layouts with hand-run ctors (see
  `CMenuUpdate.hpp`). A real IUIWindow hierarchy is a family-wide project. CMenuTitle's ctor is
  also called extern "C" from CTaskGame.
- `kyoshin/cf/CfGimmickSaveOff`: the `cf::CfGimmick` base ctor is extern "C" `__ct__cf_CfGimmick`,
  used across the gimmick family (21 references).
- `kyoshin/cf/object/CfObjectObj`: needs real ctor/dtor names (`__ct__cf_CfObjectObj`,
  `__dt__800BFA14`), which touches code_800B06A4, code_800C17DC and CfObjectTbox (whose frozen
  UNIT_RULES `exact_renames` reference them).
- `kyoshin/realtimeevt/CREvtMovie`: needs a declared `cf::CREvtObj` dtor. The placeholder
  `__dt__80185754` is shared by the whole realtimeevt family.
- `monolib/src/scn/CScnIdMan`: its RTTI record `lbl_eu_80663A78` lies in CScnItemCamera's `.sdata`
  split range. The splits must move first. The zero base-list pointer suggests a non-public
  `IScnRender` base.
- `monolib/src/lod/CTaskLOD`: the action destroys the `LOD::LODMemMan` member via the placeholder
  `__dt__8046D144`. It needs a real LODMemMan dtor and a real `new CTaskLOD` in place of the
  hand-inlined `create()`.
- `monolib/src/work/CWorkSystemCache`, `monolib/src/lib/CLibCri`: need a real
  `reslist<CCacheItem*>` template and real CWorkThread/CDeviceVICb/IErrorWii bases in place of the
  shared novtable views and the extern "C" ctor/dtor.

Weak-copy record units: CDeviceGX, CDeviceSC and CLibStaticData pass now. `monolib/src/lib/CLib`
still has a decomp-only record at 0x8045D354. That is a non-weak function naming/split issue, not
weak dedup.
