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

## 2. Where we are (2026-11-19, checkpoint 4 on `agents/matching-pass-2026-10-23`)

| Measure | Value |
|---|---|
| Functions at 100% | 14,290 / 19,310 (74.0%) — checkpoint 3 was 13,811 (+479; 462 better, 26 worse) |
| Units complete (linked) | 618 / 1,193 |
| Matched code / data bytes | 42.6% / 52.5% |
| Unmatched functions by area | kyoshin 3,366 · monolib 889 · CriWare 340 · nw4r 231 · RVL_SDK 192 · MSL 2 |
| Faithful hard counts | label 46,370 · mangled_c 2,772 · section 1,415 · offset_cast 5,147 · view_struct 3,187 · noinline 2,358 · rule 276 · rtti_off 19 · foreign_data 581 |
| Faithful soft counts | field_0x 28,093 · unk_name 1,129 · addr_func 1,013 |
| UNIT_RULES baseline | 269 (shrink-only) |

**noinline is now a hard smell** (since checkpoint 4): `__declspec(noinline)`, `#pragma dont_inline`
and `#pragma auto_inline off` all count, unless the line carries `// noinline-ok: <what was tried>`.
The baseline was migrated by adding each unit's current count (+2,358 over 171 units, not `--init`).
The count is higher than the old soft 1,870 because the pragmas now count too. Removing them means
checking in order: wrong TU boundary → definition order → unit-wide flags (MWCC_PATTERNS).

Largest unmatched kyoshin units (unmatched / total functions): `code_800B06A4` 137/253,
`cf/CfRes` 67/224, `CArtsInfo` 66/93, `cf/CfGameManager` 62/352, `action/CActParamAnim` 61/144,
`cf/CfCam` 59/181, `cf/CItem` 57/169, `CUICfManager` 56/89, `CFloorMap` 54/75,
`cf/CtrlObjectParam` 47/109, `CPassiveSkill` 45/127, `cf/CPartsChange` 45/84.

Gate state at checkpoint 4: link OK (missing 43), extab OK (27, ratcheted; CMenuBattleChain 3→1),
text-order OK (11), frozen rules OK (269), registry OK, faithful OK (ratcheted, 21 units).
Report: `/tmp/xb_ckpt/report_c4.json` (c3: `/tmp/xb_ckpt/report_c3.json`).
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

0. **Checkpoint 4 debts (2026-11-19).**
   - Identity-sweep-2 left 139 byte-identical but unpaired kyoshin helpers (CfRes 52, code_800B06A4 24,
     CfCam 23, CItem 22, CActParamData 11, CFloorMap 7). `extern "C"` on the definition is blocked by
     conflicting C-linkage declarations with different parameter types (e.g. `CfRes_tryResolveByBits`,
     `CItemBlock_getSlotU16`, `cfCam_pitchFromVec`, `FloorMap_*`). Each needs one canonical declaration.
     First step: make `CfRes_getD80Flag` (code_801A929C.cpp:209) and `CfRes_tryRefreshByBits`
     (CfGameManager.hpp:660) `extern "C"`. Membership-proven candidates (`cf::IFactoryEvent::FactoryEvent{1,3,4}`,
     the `cfCam_*` follow/yaw helpers, `getParamFloat08/0C`, …) need real member conversion, not renames.
   - The handoff's `xargs -n 50 nm -u --format=posix` census recipe is broken (the `file:` prefix drops names);
     run `nm` once per object.
   - nw4r snd: BasicSound has 4 functions at 100% instructions that still fail on the frozen
     `lbl_eu_8056A710` vtable reloc rule (ceiling 31/35). AxManager `Init` touches an unexplained 0x200 static.
   - CMenuQstCnt still overflows its split by 0x3C and has an extab size mismatch. Both predate checkpoint 4
     (Refresh/Sort/GetOrCreate/FinishAnim size diffs).
   - Report WORSE entries are section re-cut side effects (CMenuBattleChain/CMenuBattleEnd/CTalkWindow
     `.rodata`/`extabindex`, CSysWinSave). Re-check them after the sibling TU-tail re-cuts.
   - SKILL.md's template advice (`#pragma auto_inline off`) now counts as a hard smell; reconcile it.

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
