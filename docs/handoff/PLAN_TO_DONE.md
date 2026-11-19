# Plan to done: full match and source-faithful

Written 2026-11-18 at the end of the "matching-pass-2026-10-23" session. Companion to
`docs/handoff/HANDOFF_PROMPT.md` (the prompt for the agent that continues) and
`docs/handoff/CLASS_CONVERSION_BRIEF.md` (reusable brief for class-conversion subagents).

## 1. Definition of done

1. **Full match (US).** Every function in `build/us/report.json` is at 100%, every section
   (`.text`, `.data`, `.rodata`, `.sdata*`, `.sbss`, `extab`, `extabindex`, `.ctors`) of every unit
   is byte-identical, every unit is linked from source, and the linked `main.dol` hash check passes.
2. **Faithful / human-readable.** `tools/coop/run.py faithful` reports zero in every *hard*
   category (label, mangled_c, section, offset_cast, view_struct, rule, rtti_off, foreign_data)
   and the *soft* categories (field_0x, unk_name, addr_func, noinline) are driven down as far as
   evidence allows. In practice: real classes with typed members and compiler-emitted
   vtables/RTTI/ctors/dtors/thunks/operator=; no `extern "C"` pseudo-members or pre-mangled
   identifiers; no `lbl_eu_*` data labels; no `__declspec(section)`/`#pragma dont_inline`/
   `noinline`/`volatile` codegen forcing; no `*Imports.hpp` shadow declarations; no `code_XXXXXXXX`
   units; UNIT_RULES empty; names last.
3. **EU and JP** build and match with the same sources (currently only US is ever built).
4. All gates green: link, extab, text-order, faithful ratchet, frozen unit rules, registry check.

Where the two goals conflict in the short term, faithfulness wins (user: "I'm fine with lowering
the match count if it's done properly"), but every drop needs a stated structural reason and is a
debt to repay, not an accepted end state.

## 2. Where we are (2026-11-19, checkpoint 7 on `agents/matching-pass-2026-10-23`)

| Measure | Value |
|---|---|
| Functions at 100% | 14,473 / 19,310 (74.9%); checkpoint 6 was 14,427 (+46; 107 better, 49 worse; 3 functions lost 100%, see §5 c7 debts) |
| Units complete (linked) | 618 / 1,199 |
| Matched code / data bytes | 43.0% / 52.4% |
| Unmatched functions by area | kyoshin 3,238 · monolib 885 · CriWare 340 · nw4r 227 · RVL_SDK 191 · PowerPC_EABI_Support 2 |
| Faithful hard counts | label 45,864 · mangled_c 2,585 · section 1,414 · offset_cast 2,669 · view_struct 2,736 · noinline 485 · decl_shield 32 · volatile 153 · rule 275 · rtti_off 18 · foreign_data 555 |
| Faithful soft counts | field_0x 28,228 · unk_name 1,082 · addr_func 1,011 |
| UNIT_RULES baseline | 269 (shrink-only) |

Wave c6/c7 was a faithfulness wave: offset_cast 4,230 → 2,675 (CActorParam 894 → 11, CBattleManager 587 → 232,
LODMemMan/mpv_lib/mps_get/adx_tlk → 0), view_struct 2,905 → 2,741, noinline 1,321 → 1,108 despite counting more
spellings. Match count barely moved because the work was source-shape, not new matches.

**Three new hard smells (checkpoint 6), each migrated by adding the unit's current count, never `--init`:**
- `noinline` now also counts `__attribute__((never_inline))` and `DECOMP_DONT_INLINE` (+111 at migration).
- `volatile`: `(volatile T*)&obj` re-reads of non-volatile objects (+259). Hardware FIFO writes (`&WGPIPE`,
  absolute addresses) are not counted. Exemption: `// volatile-ok: <why the object is volatile>`; prefer
  declaring the member/global volatile at its definition.
- `decl_shield`: `#define name name_xxx` / `name_hidden` renames hiding declaration drift (+135). Fix: one
  canonical declaration from the definition.

**Unit flag lesson:** `-inline noauto` (not `-O4,s`) was the real retail flag for CfRes, CfResPcImpl,
CfGameManager, LODMemMan, code_804CC2B8, code_804E36DC, AXSPB — `-O4,s` dropped CfRes 211 → 141. Under
`-inline noauto` helpers retail does fold need the explicit `inline` keyword.

Largest unmatched kyoshin units (unmatched / total functions): `code_800B06A4` 118/253, `CArtsInfo` 66/93, `cf/CfGameManager` 62/352, `action/CActParamAnim` 61/144, `CUICfManager` 56/89, `cf/CtrlObjectParam` 47/109, `CFloorMap` 47/75, `CPassiveSkill` 45/127, `cf/CPartsChange` 45/84, `cf/CfCam` 43/181, `code_80135FDC` 43/124, `cf/CfNandManager` 43/48.

Checkpoint 7 (wave c8 + hy4 stopgap): noinline 1,108 → 485 via real unit flags (`-inline noauto`; `-O4,s` always
paired with `-func_align 4` because `-O4,s` resets func_align to 16), real CScn/CScnRoot/CVirtualLight classes with
compiler-emitted MI vtables, CSysWinSelect merged into CSystemWindow, CTagProcessor in-TU tag table, RVL scalar
.sbss flags, all 37 CriWare adx .bss ranges re-cut to the JP ownership in US/EU.

Gate state at checkpoint 7: link OK (missing 43), extab OK (ratcheted), text-order OK (11), frozen rules OK (269;
CScn_80496B0C pad 0x130 → 0x40 ratcheted), registry OK, faithful OK (ratcheted, 68 units).
Report: `/tmp/xb_ckpt/report_c7.json` (c6: `report_c6.json`).
NB `run.py progress` only rebuilds `build/us/report.json` when it is missing — move it aside first.

## 3. What this session established (do not re-derive)

- **Method that works:** recover the real class from the retail object's vtable/RTTI
  (`tools/coop/faithful/vt.py`), declare typed members in layout order, let the compiler emit
  vtable/RTTI/ctor/dtor/thunks/operator=, convert *all* pseudo-member free functions to members and
  rename the placeholder symbols to the mangled names, then fix split ownership from relocation
  evidence. Leaf classes first; owners after their members are real.
- **Compiler rules, logged in `docs/MWCC_CASES.md`:** implicit `operator=` of polymorphic widgets is
  out of line and nothrow; "temp + field copy + dtor(-1)" blocks are assignments from temporaries;
  inlining Rule A (`-O4,s` keeps a function with >1 call site in the TU out of line, one weak copy
  after its first user; `-O4,p` inlines) and Rule B (per-caller size budget, all-or-nothing);
  by-value struct returns; MI vtable layout and `@N@name` thunks; BDAT readers return the cell
  width and callers hold narrow locals; MWCC never trusts callee extension (narrow value is
  re-masked at each widening use); a cast on a discarded call changes register allocation;
  `Pane::SetVisible(bool)` needs genuinely `bool` callers.
- **Recovered hierarchies:** `CfObjectActor : CActorParam(+0), CAIAction(+0x3380),
  CfObjectMove(+0x3E9C)`; `CActorParam : CActorState, CBattleState(+8), CDebugState`;
  `CfObjectMove : CfObjectModel : CfObject : CObjectParam : CObjectState`;
  `CfValueItemManager : CfMapItemManager`; `CScheduleChild` is the second base of
  `CREvtEffect`/`CREvtModelObj`.
- **Real classes done:** `CScrollBar`, `CSortMenu`, `cf::CREvtModel` family, `CScnMaruShadowNw4r`,
  `CMCCrystalBox/Info/Data/ParamTable/SelTable`, `CCurNN` dtors, `CItemBoxLine`, `CIBLTabCur`,
  `CNumSelect`, `CItemBoxInfo(2)`, `QstInfo`/`QstData`, `CKizunaTalkList`, `CfMapItemManager`/
  `CfValueItemManager`/`CfMapEffectManager`, `CEquipChange`, `CEquipItemBox` family, `CItemBoxGrid`
  family, `CSaveLoad` family, `CMapSel`, `COption` (shape only), `CBattleState`/`CActorParam`
  (vtables byte-identical), `CfObjectActor` (vtable compiler-emitted), menus `CMenuItem`,
  `CMenuGCItem`, `CMenuShopSell`, `CMenuItemExchange` (members), `CMenuKizunaTalk` (partial).
- **Canonical helper headers:** `src/kyoshin/code_80135FDC_{Bdat,Lyt,State}.hpp`,
  `CQuestWindow_Pane.hpp`, `CItemBoxGrid_Color.hpp`, `CUICfManager_Res.hpp`.

## 4. Phases

Work is ordered so each phase removes the scaffolding the next one would otherwise trip on. Within
a phase, go leaf-first and keep agents on disjoint file sets. After every wave: full build, compare
against the previous report, all gates, fix regressions, commit.

### Phase A — finish the class recovery in `src/kyoshin` (in progress)

A1. **cf object leaves** (interrupted mid-work, see §5): `CfObjectEne`, `CfObjectPc`
    (`code_800C17DC` is a mis-split piece), `CfObjectNpc`, `CfObjectObj`, `CfObjectPoint`,
    `CfObjectImplNpc`, `CfResTboxImpl`, `CfCollSphereImpl`, `CfObjectModel` hand vtables; the ~50
    `CfObjectMove`/`CfObjectModel` slot overrides; real `@8@`/`@16028@` thunk names;
    UVF37/38 (slots 0x460/0x464) signatures; `CActorState` inline.
A2. **Shared UI bases:** a real `IUIWindow : CTTask<IUIWindow>` (fields 0x54–0x6B, inline ctor
    setting layout=0, +0x60=-1, +0x67=1; `IScnRender` at +0x6C) — unblocks real ctors for
    `CMenuItemExchange`, `CMenuKizunaTalk` and every other menu factory. `Cur_*` (14 functions in
    `CCur.cpp`, ~600 references) → `CBaseCur` members; `sysWin*`/`CSysWin_*` → `CSysWin` members;
    then delete every `extern "C" __ct__7CSysWinFUc`/`__dt__7CSysWinFv`/`Cur_*` declaration.
A3. **Member-layout fixes found by generated copies not matching:** `CItemBoxGrid` (`_531` pad:
    aligned 8-byte pair at 0x534, word at 0x53C); `CQuestItem` hand `operator=` →
    implicit (`MenuShopSellCopyData` = `__as__10CQuestItemFRC10CQuestItem`); `CItemBoxPage`
    (`char mName[0x80]` at 0xD9, not `mName[0x67]`+`mValues2[12]`+pad); `CItemBoxLine` explicit
    padding members (`pad_4D`, `pad_6E`, `pad_391`) must not be copied.
A4. **Remaining widget/menu conversions:** `CFloorMap` + `CFade` (then drop `dont_inline` on
    `CMenuMapSelect::Init`, `FloorMapCopy` → operator=), `CPartyStateWin`
    (`PartyStateWin_CopyEquipChange` = `CEquipChange::operator=`), `CModelDispMakeCrystal` →
    `CMakeCrystalWin` → `CMCGetItemBox`/`CMCCrystalList`/`CMenuMakeCrystal`, `CCollepedia`,
    `CMenuSymbolMark`, `CMiniMap`, `CIBLTabFull`, `CfAward`, `COption` bodies, `CArtsInfo`,
    `CPassiveSkill`, `CKizunagram`/`CPcKizunagram`, `CQuestWindow`, `CSysWin*` family,
    `CUICfManager`, `CUIWindowManager`, every remaining `CMenu*`.
    Small copy helpers to turn into implicit operator=: `setQstEntry/copyQstEntry/copyQstEntry2`,
    `CopyEntry9Bytes/CopyGridEntry9`, `setEIBItem/copyEIBItem/copyEIBItemRet`,
    `EquipChange_CopyCursorRec`.
A5. **Rest of cf / action / plugin / realtimeevt / cfsys:** same recipe, driven by
    `targets.json` + RTTI. `CBattleManager`, `CfGameManager`, `CItem`, `CfRes*`, `CfCam`,
    `CtrlMove*`, `CtrlObjectParam`, `CVision`, voice, `CAct*`.

Exit: no hand-written vtable/RTTI data, no `extern "C"` ctor/dtor/pseudo-member declarations, no
`*Imports.hpp` in `src/kyoshin`; faithful `mangled_c`, `rtti_off`, `view_struct` near zero there.

### Phase B — splits and unit identity

B1. Dissolve every `code_XXXXXXXX` unit into its real owner(s) using relocation evidence
    (who references the data, where weak functions/thunks land, `.ctors` order, RTTI ownership).
B2. Re-cut `.data/.rodata/.sdata/.sdata2/.sbss` boundaries so no unit defines data it does not own
    (faithful `foreign_data`, `section`); known: `CREvtModelObj/Pc` data split, `CItemBoxGrid`
    0xB8 over text budget, `CfObjectActor` `.sdata2` 0x38 vs 0x40.
B3. Replace `lbl_eu_*` labels with real statics/string literals/float constants emitted by the
    compiler in the owning TU (faithful `label` — the largest count; mostly mechanical once the
    owner is known: Haiku sweeps, unit by unit, with `data diff` as the check).
B4. Fix stale `unit` fields in `targets.json`; shrink UNIT_RULES to zero.

### Phase C — match the remaining functions

C1. Per unit, after its classes are real: fix member types first (most low-% ctors/Init functions
    are wrong member types, not wrong code: `CItemBoxLine` ctor 19%, `CMapSel` ctor 37%,
    `CMenuShopBuy::Init`), then function bodies. Use `hexdiff`, `docs/MWCC_PATTERNS.md`,
    `docs/register_mapping.md`, `docs/KNOWN_WALLS.md`.
C2. Undecompiled stubs (empty bodies standing in for real functions, e.g. `CMenuGCItem::Init`,
    `CMenuShopSell::Init`, `CMenuItemExchange::Phase2Idle`): decompile for real; remove the
    `noinline` that keeps stubs from folding.
C3. Exception tables: `extab`/`extabindex` mismatches are almost always missing real locals with
    destructors or wrong ctor/dtor shapes — fix the source shape, never the table.
C4. Libraries: monolib (1,096), CriWare (405), nw4r (346), RVL_SDK (230). nw4r/RVL/CriWare have
    public reference decomps of the same or near versions — use them as evidence for source shape
    (respect PLAN.md §2 legal boundaries). monolib needs the same class-recovery treatment as
    kyoshin.

### Phase D — link everything

D1. Flip units to linked as they reach byte-identical in all sections; keep link-gate at zero
    missing/duplicate. D2. Whole-DOL hash check for US.

### Phase E — EU and JP

Build both, fix `config/{eu,jp}/splits.txt`/`symbols.txt` drift (every split/symbol edit this
session was applied to all three but only US was built), resolve version differences with
`#if VERSION_*` only where retail differs.

### Phase F — naming and cleanup (last)

Replace `field_0x*`, `UnkVirtualFunc*`, `func_8XXXXXXX`, `UnkClass_*`, `lbl_*` remnants with
evidence-based names (strings, BDAT column names, RTTI, asserts, sibling titles' symbol maps where
legally usable). Delete dead scaffolding, stale docs, `docs/ownership.csv`; regenerate README
status.

## 5. Immediate debts at the checkpoint (do these first)

0. **Checkpoint 7 debts (2026-11-19).**
   - Functions that lost 100% (fix first): `CCollepedia` `clpNextCatPg`/`clpPrevCatPg` 100 → 83.8% (likely the
     `CItemBlock_getSlotU16`/`getKindSlot` declarations moved to `CItemBlockApi.hpp` with a new signature);
     `CScnNw4r::create` 100 → 99.8% (scn class rewrite).
   - Section slips from re-cuts/real classes to re-verify: CScnMem .rodata/.data, CVirtualLightDir/Amb .rodata,
     CScnRootNw4r .rodata, CSystemWindow .text (merged TU), mix/remote/GXFifo/GXMisc .sbss, ax_rna/adx_inis .bss,
     CMenuPause .sdata2.
   - Kept hacks with measured reasons: CSkipTimer (`-ipa off` is the real fix but breaks assignment inlining in
     BeginLoad/OnFileEvent), CfHikariItemManager (f30/r29 scheduling around the fade helpers).
   - CScnItemModel hierarchy is a multi-session job (deferred). CVS_THREAD_HP alternating size pragmas are probably
     real source pragmas (unproven). CTagProcessor extab residuals.
   - Stale cfCam helper declarations in `include/kyoshin/code_800B06A4.hpp` and
     `src/kyoshin/cf/CfGameManagerUnityHelpers.hpp`; 7 noinline in `CMenuArtsSetImports.hpp`.

0. **Checkpoint 6 debts (2026-11-19).** The wave was cut short by the OpenCode Go weekly usage limit;
   agents c7-ifacevt, c7-kydrift, c7-kynoinline (follow-up), c7-handvt (follow-up), c7-cview (follow-up) and
   c7-volatile (follow-up) died mid-task. The tree builds, all gates pass, and one half-edit (sfd_tim
   `SFTIM_InitHn` `tmp` reload) was restored by the orchestrator. Unfinished items:
   - **Regressions on ✗ functions (report fuzzy, c5 → c6):** `SFTIM_InitHn` 96.7 → 82.0 (volatile
     `SfdTime::vsyncTime`; un-volatiling it drops sfd_tim to 30/46, so the volatile member stays until InitHn's
     reload is re-derived), LODMemMan §extab 67.3 → 10.9 (c7-lodsub's in-class `~LODMemMan` / LODG3dMgr ctor or
     c7-cview's header edits — not yet bisected), `~CView`/`__dt__CMsgParam<10>` 86.4 → 73.4 (MWCC writes
     `__vt__CMsgParam<10>` in the inlined dtor; retail doesn't), `followUpBattleMove` 94.1 → 83.8 (cause unknown),
     `resetArtsStatus` 86.4 → 84.2, `mpfCullMarkLayers` 71.2 → 68.4 (size 0x548 → 0x580, from c7-volatile).
   - **IWorkEvent / IScnObjCallback are each 2 virtual slots short** of retail (`__vt__4CScn` 0xAC vs 0xB4).
     That blocks removing CScn's hand blobs (`lbl_eu_8056E8D0`, `lbl_eu_8056E984`, `inject_relocs` in
     CScn_80496B0C.cpp, `Scn_DtorThunk54`) and CScnItemModelNw4r's 9 `-0x1464/-0x1468` MI shims.
   - **CTagProcessor conversion is proven but blocked by splits:** `kyoshin/CSystemWindow.cpp` has no `.data` range,
     so its 0x100 vtable sits in CTagProcessor's range; CTagProcessor `.data` should start at 0x8052d478. Fix the
     split, then rerun `/tmp/xb/c7-tagproc/CTagProcessor.cpp.converted` + `pairs.txt` (prefix
     `Q227@unnamed@CTagProcessor_cpp@`; see MWCC_CASES).
   - decl_shield residuals (52): CModelDispEquip.hpp, CModelDispMakeCrystal, CfObjectMove (`writeVec3f`,
     `Scn_SetupAnimDefault` 1-arg vs 5-arg, `Scn_IsAnimActiveOrNull`, …) — c7-kydrift died before finishing.
   - volatile residuals (194): kyoshin (CActorParam 32, CfResReloadImpl 16, CItem 14, CfObjectPc 12,
     CfObjectActor 10), CGXCache 26 (inside `CMsgParam<32>::storeMsg*`), CViewRoot 13 (`CViewRootPool::mUsed`
     volatile reproduces retail loads but re-colours a register), and 8 load-bearing in FULL functions
     (Coli_WalkQuery, simSetFlag2OnTree, simSyncBuf824Flags, scnImN4Sync*). ax_rna SetTransSw wait-word and
     mwsfdcre tag word are load-bearing too.
   - CBattleManager keeps `BMSentinelView` / `ActorSub3E9CView` as codegen devices (real member spelling CSEs the
     base conversion: FindActorByKind 100 → 17 %). ~150 casts on an unnamed move/arts stat block.
   - RVL_SDK: 28 of 160 unmatched are instruction-identical, data-address only (`.sbss` reverse declaration
     order); GXFifo 7 / WUD 10 are blocked by MWCC 4-aligning `u8[N]` (retail packs at align 1).
   - CItemBlock_* (13) need the CItemBlock class + static instance recovered before memberizing.
   - targets.json display names are stale for the 9 LODG3dMgr rows (identity is correct).
   - Two `// volatile-ok:` (snd_AxfxImpl RestoreAlloc, sfd_mps sfmps_pesfn) and ~70 `// noinline-ok:` remain.

0. **Checkpoint 5 debts (2026-11-19).**
   - Report losses vs c4, all accepted with causes:
     - CItemBoxInfo's two ctors: `-ipa file` folds the empty `CItemBoxPage::Reset`, and `-ipa off` costs net −11.
     - CCollepedia `clpSetupDetPg`: the `-ipa off` flag needed for the noinline removal.
     - CMenuArtsSet `cbRenderBefore`: 99.88% after the re-cut.
   - CItemBoxInfo keeps one `#pragma optimize_for_size off` on `setItemBoxVtxColor`. Retail compiled that one function in
     speed mode inside a size TU; no flag or compiler version reproduces it, so it is probably a real source pragma.
   - CView: compiler-emitted vtables now, but the generated .data/.rodata order differs from retail. The next step is
     real `reslist<>`/`CMsgParam<10>` members, so `FORCEKEEP_CView_templates` can go. The ctor dropped from 36.8% to
     20.9% (it was not matched before). CFontLayer is still novtable because of the frozen CView.o `exact_renames` rule.
   - mono-vtables re-cut CScnBlend/CScnBloom/CScnCameraMan/CScnEffectActNw4r/CScnFadeMan/CScnFilter .rodata in US only.
     EU/JP got symbol renames only. Check the EU/JP splits.
   - LODMemMan identities are settled: the 0x78 `LOD::LODMemMan` sits at +0xA44 inside the 0x1CE0 manager
     `UnkClass_8046D144`. Its 26 offset_casts and 49 view_structs can now be converted. CMsgParam's vptr at 0x80 is real,
     so CException's 10 casts need a different approach.
   - CriWare: `adx_tlk` .bss starts 4 mod 8, which blocks the adx_fs recipe. `mwsfdlib`'s 0x3720 `MWSFDGlobal` is a
     view over 7 retail .bss symbols and must be decomposed first. `sfd_mpv`'s remaining ✗ functions are incomplete
     logic (missing dispatch arms), not type problems.
   - nw4r: the SeqTrack header contradicts retail offsets (offset map in MWCC_CASES). snd_Voice's `VoiceLayout`
     mirror is now redundant (~100 sites).
   - code_80231950: `-ipa off` fixes `artsStepState` but loses `__dt__9CArtsListFv`. This supports the theory that
     the range holds several TUs, but no boundary evidence has turned up yet.
   - Agents ran `git status` 3 times this wave despite the rule (read-only, nothing changed).

0. **Checkpoint 4 debts (2026-11-19).**
   - ✅ MOSTLY DONE (2026-11-19, agent `canon-decl-ky`): 126 of the 139 are now paired
     (CfRes 52/52, CItem 22/22, CFloorMap 7/7, code_800B06A4 20/24, CActParamData 10/11, CfCam 16/23;
     report functions +126, unpaired 2,188 → 2,060). Method and the two negative results are in
     `MWCC_CASES` "kyoshin identity sweep-2": canonical declaration = the definition's signature, then
     `extern "C"` on the definition, then one cast per non-converting call site. **Do not retry** the
     remaining 13 without a splits decision:
     · 6 `cfCam_*` + `pushParamNode` keep the split declaration because unifying it lets MWCC `-ipa file`
       inline the small body into accepted callers (measured 145→135 ✓ in CfCam). Needs the retail TU
       boundary for that helper cluster (they are interleaved with their callers, so it is a *multi-split*
       question, not one cut).
     · `cf::IFactoryEvent::FactoryEvent{1,3,4}` + `cf::CfObjectMove::CfObjectMove_hasActorScale` are
       mangled member names emitted by free functions with pre-mangled names; converting them needs the
       hand-built `.data` function-pointer tables (CfObjectMove.cpp:2441, CfResTboxImpl.cpp:343,
       CCharVoiceMan) replaced by compiler-emitted vtables.
     Still-membership-proven-but-flat-named (`getParamFloat08/0C`, `resetParamSubB`,
     `CItemBlock_*`, `CfResBuf_popRecord`) are now `extern "C"` and paired; memberizing them is a
     symbol-rename + `rename_syms.py` job for the class-recovery phase.
   - The handoff's `xargs -n 50 nm -u --format=posix` census recipe is broken (the `file:` prefix drops names);
     run `nm` once per object.
   - nw4r snd: BasicSound has 4 functions at 100% instructions that still fail on the frozen
     `lbl_eu_8056A710` vtable reloc rule (ceiling 31/35). AxManager `Init` touches an unexplained 0x200 static.
   - CMenuQstCnt still overflows its split by 0x3C and has an extab size mismatch. Both predate checkpoint 4
     (Refresh/Sort/GetOrCreate/FinishAnim size diffs).
   - Report WORSE entries are section re-cut side effects (CMenuBattleChain/CMenuBattleEnd/CTalkWindow
     `.rodata`/`extabindex`, CSysWinSave). Re-check them after the sibling TU-tail re-cuts.
   - SKILL.md's template advice (`#pragma auto_inline off`) now counts as a hard smell; reconcile it.
   - smell-mono (wave c5): CScnCameraMan member conversion is blocked. `CamMan_FindItemA_B158` is called from
     CScnEnvLgtCtrl and CScnVirtualLight, and CScnCameraMan.hpp declares a local minimal `class CView` that
     contradicts the real CView.hpp. CView's last 2 offset_casts are the `novtable` + hand-vtable debt: let MWCC
     emit the vtable/RTTI/thunks, with the template vtables in retail order. code_804DEDA8's tex views are Monolith's
     own texture record, not `nw4r::g3d::ResTex`. Dead duplicates: `include/monolib/core/CSplitFrame.hpp`
     (unused) and `libs/monolib/src/core/CView.hpp` (stale stub the faithful tool picks as CView's header).
   - tu-tail-ky (wave c5) made 5 TU re-cuts: code_80296E20, menu/code_80228DE4, menu/code_80272AD8,
     menu/code_80231950 and code_8028130C. Their faithful baselines were added at current scores (each split
     is neutral or better).
     - The moved units' data is sliced but still uses `extern lbl_eu_*` (rodata/sdata2/sbss).
     - code_80231950 overflows its split by 0x224: `artsStepState` should be a cmpwi chain with one `bl`
       per case, and there are 71 noinline uses, so the range may hold several TUs.
     - CSysWinBuff and CMenuGameClear: their trailing bodies are the head of the next unit, so the
       neighbour's start needs to move back.
     - CSysWinSave.hpp has 2 dead `lbl_eu_80668BA0*` declarations.
     - `-O4,s` overrides an earlier `-func_align 4`; add `-func_align 4` after it (MWCC_PATTERNS §16).
   - canon-decl-ky (wave c5) paired 127 of the 139 helpers (+128 report). It typed the unified flat helpers'
     parameters from their definitions (`int`), which forced ~22 caller casts such as `(int)ptr` and `(u32)ptr`.
     Integer and pointer parameters compile identically, so the casts point to the original types being pointers.
     Recover the pointer types and drop the casts. Membership-proven helpers that are now `extern "C"`
     (`getParamFloat08/0C`, `resetParamSubB`, `CItemBlock_*`, `CfResBuf_popRecord`) still need memberizing.
   - CTagProcessor: the 23 flat `__ct__CTagCode*(void*)` stubs write vtable labels by hand. They should be real
     `CTagCode*` classes with compiler-emitted ctors and vtables.
   - faithful.py's `view_struct` now exempts classes that retail names via a ctor/dtor/vtable symbol
     (only `CView` today). Lower the baseline at the next checkpoint.

0a. **Checkpoint 3 debts.**
   - Post-checkpoint-3 debts (2026-11-19):
     - `CtrlMoveBase.cpp` reaches the CColiProc ctor through a block-scope pre-mangled
       `extern void __ct__9CColiProcFv(void*);`. A real construction can't reproduce retail: retail has no
       placement-new null guard, and a real static inlines the ctor. Its 14 flat `moveBase*` names need a
       real-member conversion together with CtrlEnemy/CfGimmick* callers.
     - The coli identity sweep made 63 coli definitions flat `extern "C"` (report +55). This also closed
       dangling flat references from kyoshin/scn callers.
     - ICamControlRemote is 10/10 ✓ but its `.data` is MISMATCH (see the CSysWinBuff note below).
     - ✅ `code_804B2FF0` split overrun fixed (ColiXformFixed z-bounds used the wrong extent field; now
       0x28FC exactly).
     - KPAD: the 17 residuals are mostly `.sdata2` reloc order from named static consts folding into
       pool copies.
     - WPAD: `.data` is 0x1BC vs retail 0x424 (missing debug strings), which costs 5 functions.
     - KPAD.c and WPAD.c are now pinned to `Wii/1.0a` (+1 ✓ each over Wii/1.1; 1.0RC1 gives the same
       result; 1.5+ regress). WPADHIDParser stays on the default because its 1.1 match sum is
       higher.
     - CMdlDynamics `MdlDyn_AddVecAll` still uses `volatile` re-reads (policy smell, 34 structural);
       CView's three template vtables are emitted in the wrong order (reslist dtors ×2 + `wkStandbyLogout`
       are off by 12 bytes).
     - CScnCameraMan: 11 `CamMan_*` bodies are defined `extern "C"` so they pair with retail's flat
       placeholders (report 4→10/17). They are really `CScnCameraMan` members, but CScn.cpp calls them
       by flat names. Convert them together with CScn.cpp's callers.
     - `faithful --ratchet` was run once mid-wave by an agent, which ratcheted in-flight improvements from
       other agents. Re-check the faithful baseline at checkpoint 4.
     - Identity sweep (monolib core/effect/scn, g3d_xsi): ~83 bodies were made `extern "C"` to pair with
       flat retail placeholders (report +53 functions). They are interim names; revisit them when callers
       and owners are recovered.
     - UnkClass_80471EC8: its 15 pre-mangled `Fv` placeholders contradict the register ABI. Recover the
       signatures from callers bottom-up first.
     - CScnTexWorkMan is 908 B over its split.
     - CriWare `.bss` blobs: adx_fs, adx_mwii and ahx_dcd are now real objects defined after the code in reverse
       order. **adx_tlk (0x289C)** and **mwsfdlib (0x3720)** are blocked: their `.bss` split ranges also hold
       objects owned by siblings with no `.bss` split (adx_lsc/sfa/sjd/stmc; mwsfdcre/svr/sl/svm/ply; the
       address table is in MWCC_CASES). Fix the split ownership first. adx_mwii keeps one `extern adxm_setup_cnt`
       alias of `adxm_frm+4` (a second retail spelling). g3d_state also has a blob.
     - CMenuBattleChain: the end of the TU was split off into a new `kyoshin/menu/code_802ADB08.cpp` (8/8 ✓).
       The resource names are plain literals now; the agent's `#define` + offset indexing was replaced.
       Sibling candidates with the same `[__sinit][thunks][helpers]` tail: CSysWinSave, CSysWinScenarioLog,
       CSysWinBuff, CMenuArtsSet, CMenuPlayAward, CMenuSymbolMark, CMenuGameClear, CMenuQstCnt.
     - The kyoshin identity sweep (pluginCfs/CfResPcImpl/CfObjectEnumList/code_8018F8D8/CfObjectImplMove,
       report +157) **de-memberized** 5 functions (`getObjectAt`, `getObjectIdAt`, `getEntryAt`,
       `CfObjectImplMoveClearLinkIfMatch`, `MoveImplMatchIdSetRegion`) to pair with flat callers. Restore
       them as members and update the callers. Also: 40 `extern "C"` definitions, and `CfResPcImpl_*` /
       `CfObjectImplMove_*` pseudo-member free functions still need memberizing. COption is 7/48 with 41
       bodies unwritten.
   - `.sdata2` pool ownership: `CGXCache.cpp` still emits a 0x11B6 "dissolved monolibdata2" array
     (875 constants) that ~80 units reach via `extern lbl_eu_*`; ~95% are really each unit's own
     literals. Re-homing must be done per unit with ✓-set diffing (pool order drives reloc identity).
     92 constants in 17 split gaps (auto units) are mostly the preceding unit's string tails.
     Inventory: `/tmp/xb/pool/inventory.md`. CGXCache's `BiasDouble` helper (7 functions) waits on it.
   - Weak IUIWindow family: aliases `lbl_eu_8052D238` / `lbl_eu_80661ED0` stay in us/eu symbols.txt
     until the last 12 TUs are converted; `IUIWINDOW_INLINE_DTOR` / `CTTASK_INLINE_DTOR` opt-in header
     hooks; CMenuUpdate `.sdata` needs `__RTTI__Q22cf6CfBdat` odr-use.
   - `@108@OnFileEvent` thunk shape: **solved** for CSysWinBuff (12/14 ✓) and renamed for
     CQuestWindow by making the C-linkage body the member body — see MWCC_CASES "CSysWinBuff::OnFileEvent".
     CQuestWindow still emits a 4-byte forwarding copy (`onQuestFileEvent` + wrapper); eliminating it
     means writing the 0x2F4 body as `CQuestWindow::OnFileEvent` (or making the override inline in
     the header — line-count sensitive).
   - ✅ DONE (2026-11-19) `CSysWinBuff` / `CSysWinSave` `.data` — both now **MATCH** (256 bytes /
     252+4 tail) with no source hand-blobs. The boundaries were the whole problem: CSysWinBuff
     `.data 0x80537DC0..0x80537EC0` (its 0x10 `camControlTable` is ICamControlRemote.cpp's user
     object — it is followed in the same run by that TU's generated ICamControl vtables) and
     CChainCombo `.data 0x80538988..0x805389A0` (3-word table + the weak `__vt__Q22cf11CChainCombo`)
     with CSysWinSave `0x805389A0..0x80538AA0`. `cf::CChainCombo` is now a real class (members, then
     `virtual ~CChainCombo() {}`: MWCC puts the vptr at 0x14 and emits a **weak** vtable in the
     ctor's TU, which is exactly where retail keeps it; the surviving weak `__dt__` copy is
     CBattleManager.o's). Recipes/evidence: MWCC_CASES "MWCC vtable emission, part 2" + "Split
     boundaries cut by neighbour-owned bytes". JP's splits already had these boundaries.
     Remaining: ICamControlRemote's other 0xC8 bytes (ICamControl/Classic/Gc vtables + RTTI as real
     classes) and its `.sbss` instance cells (`camControlDefault/Classic/Gc`) still live in
     CSysWinBuff's slice; `code_8027513C.cpp`'s own `.data` 0x130 vs 0xC.
   - ✅ RESOLVED as "no change" (2026-11-19) `CSystemWindow` vtable: the block is CSystemWindow's
     *class* data but was emitted by the TU at **CTagProcessor's** link position — all three of
     that unit's slices start exactly there (`"CSystemWindow"` at `.rodata:0x804FEFA8`,
     `__RTTI__13CSystemWindow` at `.sdata:0x80661EE0`, `__vt__`+base list at `.data:0x8052D378`),
     and CSysWinSelect's three slices are exactly consumed by its own vtable/RTTI/name string. So
     the existing US boundaries are consistent; converting CSystemWindow would emit a second copy
     *earlier* in link order and break CSysWinSelect/CTagProcessor. Do not convert. See MWCC_CASES
     "`__vt__13CSystemWindow` is emitted by the TU at CTagProcessor's link position".
   - `code_804DEDA8` moved to Wii/1.1 (configure.py); ReleaseHandle x3 via s16 entry + inlined s32
     worker. `SchedAnim_NoOp` is really a 0x238 colour-apply function (stub).
   - LODMemMan: `teardownG3dObjects` 65.3% (retail does not inline `clearFlagAndResetName`; no
     source/flag reproduces it); `.data` Panic strings belong to `code_80468434` (splits); RTTI
     locator/name-string placement costs `.rodata`/`.sdata`.
   - CColiProc hierarchy (CColiObjCall/CColiResCall) needs three splits changes — agent in flight.
   - CScnEnvLgtCtrl: hand vtable kept (class model must be split first); data now MATCH.
   - CCharVoiceMan: flat placeholder names kept as `extern "C"` definitions — revisit as members.
   - CMenuUpdate: 3 byte-identical functions ✗ on `@stringBase0`; two `__declspec(noinline)`.
   - lyt_material `.text` +0x6C; gcci_work packing; WUD/WPADHIDParser colour plateau;
     snd PrepareStrmImpl pool order; hexdiff ADDR16_HA/LO +2 display bug.

0. **Checkpoint `a50f9b530` debts.**
   - Faithful ratchet rises: `sfd_tim` 106→112, `CCollepedia` 203→205, `CPassiveSkill` 756→758,
     `CUIErrMesWin` 41→42, `CMCGetItemBox` 151→153, `CMenuMakeCrystal` 13→14, `CMenuGameClear`
     9→10, `CMenuGetItemMulti` 970→971. Sweep brief ready: `/tmp/xb/briefs/sweep_icc_syswin.md`
     (one real `cf::CfGameManager::isClassicController(int = -1)` replacing ~17 per-file
     `extern "C"` stand-ins — all 189 retail call sites pass -1; owners holding `CSysWin` as raw
     bytes get a typed member; `sfd_tim` inspected separately). Then `faithful --ratchet`.
   - `CMenuUpdate` 33→28/36: retail keeps the small accessors out of line and calls the base
     ctors; `-inline off` fixes the ctor/IUIWindow/IWorkEvent ctors but breaks the placement-new
     inline in `SpawnProcessForMode`/`RebindLayout`. Hypothesis: helpers defined after first use.
   - CriWare `gcci.c` (agent hit the usage limit mid-rewrite): 10/17, `.data` 0x68 table not
     emitted, `.rodata` 0x290 vs 0x228. WIP copy `/tmp/xb/libs_cri3/wip/gcci.c.wip`.
   - `snd_StrmPlayer` 36/45: remaining 9 functions with hypotheses in
     `/tmp/xb/glm_nw4r/HANDOFF.md`. The GLM work used ogws (Wii Sports decomp) source shapes —
     needs a legal check against PLAN §2 before anything leaves this fork.
   - `CLibCri.cpp` hand-defines `__vt__Q34nw4r3g3d8ScnGroup[8]`, which is really CLibCri's RTTI
     base list (retail `lbl_eu_8056CF28`); data matches, name is wrong. Fix with a real CLibCri class.
   - g3d `Scn*` type-name strings are scaffolding labels and the US `.rodata` splits are wrong
     (JP is right: scnobj 0x30, scnroot 0x10, scnmdlsmpl 0x18, …).
   - `CView` ctor 84.37→84.07 (correctness fix, accepted); `CMenuItemExchange` extabindex noise.
   - Not yet converted to the real `IUIWindow`: `CMenuKizunaTalk`, `CUIWindowManager`'s local
     copy, `CQuestWindow`'s own struct, the `CSysWin`/`CSystemWindow`/`CTalkWindow`/`CMiniMap`
     hand-vtable family; `LODMemMan` (three distinct classes, needs per-function classification).
   - cf leaf state and the CfObject slot canonicalisation blocker: `/tmp/xb/cfleaf2/NOTES.md`,
     `BLOCKER.md` (15 base-slot stubs still undefined; `mineUpdateAll` 9.8→9.3; CfResTboxImpl
     `.data` region holds four classes' data and needs a splits decision).

1. **cf leaf work was interrupted twice** (API timeout, then usage limit) with edits on disk and no
   final report. State: tree builds and links; `CfObjectObj` 22→26 and `CfObjectPoint` 31→34 on
   the faithful ratchet; `CfObjectNpc` `.rodata` 100→84.8 and `loadIconType`/`getRltMeet` just
   under 100; `CfObjectEne` `initEnemyBdatParams`/dtor 99.8; `CfObjectPc` extab 66.6→54.5. Scratch
   and before-tables: `/tmp/pp/work/cfleaf/` (may not survive; the pre-work state is commit
   `523bed673`). Review the diff of `src/kyoshin/cf/object/**` between `523bed673` and HEAD, finish
   or back out each half-done class, and get those five items back to at least their old values.
2. **`CMenuMakeCrystal` 13→14** on the ratchet: interim `extern "C" __as__11CTitleAHelpFRC11CTitleAHelp`
   call (same interim in `CPartyStateWinImports.hpp`); goes away when `CMakeCrystalWin` /
   `CPartyStateWin` are real classes and their `Init` blocks are real assignments.
3. **Loosened declarations** to undo with A2: `CMenuPlayAward.hpp` (`Cur_DrawLayout`,
   `Cur_SetVisible`, `Cur_Update` as `void*`), `CFloorMap.hpp` (`__ct__7CSysWinFUc(void*, int)`).
4. **Small regressions vs the previous checkpoint:** `QstLogInfo_UpdateTypeObjectives`
   94.3→92.6, `__ct__CMenuKizunaTalk` 48.9→47.3 (placement-new null check; needs real
   `IUIWindow`), `CMenuMapSelect::Init` 99.80→99.67, `buildKizLineLayout` 56.46→56.16, extabindex
   fractions on `CTitleAHelp`, `CMenuItemExchange`, `CMCCrystalList`.
5. **Width leftovers** (callers disagree on a helper's width; one true signature each):
   `BdatGetItemId/Type` (u16 kept; `GetItemMulti_ShowItemRank` masks remain), `GetCollectedFlagByte`,
   `GetU16PairEntry`, `clpGetRowDone`; functions not yet looked at: `ItemBoxLine_CalcCountKey`,
   `showCrystalBoxStatus`, `PushUniqueItemId`, `SymMark_IsNew`, `QstData::Refresh`,
   `func_801CCAF0`, `rebuildEIBKind48Page`, `GetItemMulti_ShowDetail`, `InitGimmickTables`,
   `artsRenderRow`, `KizunagramUpdateMoodIcons`, `MiniMapUpdateMapIcons`.
6. **Open decision for the user:** `QstLogList_CopyQstInfo` in `CQstLogList.cpp` is an inferred,
   unreferenced stand-in for a second `QstInfo` assignment (needed so Rule A keeps `operator=` out
   of line). Find the real second assignment or ask whether to keep it.
7. Older known regressions: `PartyStateWin_OpenWindow` 81.6→56.9, `CEquipChange` ctor 99.4,
   `CMenuShopBuy::Init` 2.4% (CItemBoxPage layout, A3), `CMenuQstCnt` lost `__dt__7QstDataFv`,
   `CItemBoxLine` ctor 19%, `CMapSel` ctor 37%, `CfObjectActor` `.data` (10 thunk-name words +
   UVF37/38).

## 6. Working rules that proved necessary

- One orchestrator; subagents on disjoint file sets; shared headers read-only for all but one
  agent per wave; `config/*/symbols.txt` and `targets.json` edits only through
  `tools/coop/faithful/rename_syms.py` (pass each `old=new` as its own argument).
- Subagent prompts must say: convert **all** pseudo-members (placeholder symbol names are not
  evidence), no codegen-forcing pragmas, report what was not verified. Verify every agent report
  against a fresh full build — agents' per-unit numbers and "gates pass" claims were wrong often
  enough (mid-edit trees, baseline ratchets, hand thunks under `dont_inline`).
- Never re-init the faithful baseline to hide a rise; `--ratchet` may only lower entries.
- Header conflicts ("illegal overloading") are the signal that a helper still has several
  hand-written declarations: give it one canonical declaration, do not add another local one.
