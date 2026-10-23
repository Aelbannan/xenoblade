# Header consolidation pilot: CScn and CTaskGame (2026-10-23)

Goal: collapse duplicate class definitions into one canonical header per class, so
TUs can include real headers instead of local "minimal"/"view" copies. Gate: zero
byte regressions.

## Result

| Class | Definitions before | After | Kept copies |
|---|---|---|---|
| `CScn` | 29 | 3 | `libs/monolib/src/scn/CScnMem.hpp`, `src/kyoshin/menu/CMenuKizunaTalk.hpp` |
| `CTaskGame` | 26 | 3 | `src/kyoshin/CMiniMap.hpp`, `src/kyoshin/menu/CMenuQuestLog.cpp` |

- Canonical headers:
  - `libs/monolib/include/monolib/scn/CScn.hpp`. Lean: only `CTTask.hpp` and `IWorkEvent.hpp`,
    plus forward declarations. It merges the private copy's knowledge: `virtual ~CScn()`
    (retail `__dt__4CScnFv`), `Term`, `initSubsystems`, and `mFrame84` at +0x84. The
    CScn-TU-only `extern "C"` imports moved out to the private header.
  - `src/kyoshin/CTaskGame.hpp`. Lean: the class, its interface bases (`ITitleMenu`,
    `IErrMesWinSel`), `CTaskGameSlotObj` and `UnkClass_8004041C`. The 560 lines of
    CTaskGame-TU imports (flat retail names, labels, layout views, the `CBattery` view) moved to the
    new `src/kyoshin/CTaskGameImports.hpp`, which includes the canonical header.
- `libs/monolib/src/scn/CScn.hpp` (private) no longer redefines `CScn`, `ScnRenderCB`,
  `UnkScn68` or `UnkScn8C`. It includes the public header and adds only TU helpers.
- `CScnMaruShadowNw4r.hpp`: the fake `struct CScn { u8 storage[4]; }` became `class CScn;`.
- View structs removed: `CScnFlagView` (CMakeCrystalWin.hpp), `CPartyStateWinScnFlags` and
  `CTaskGameProcView` (CPartyStateWin.hpp). The +0x39 write now goes through
  `CProcess::SetRemove()` and the +0x3E4 write through `CScn::unk_3E4`.
- Casts removed: 6 `reinterpret_cast`/C-style casts (2 in CMakeCrystalWin.cpp, 3 in
  CPartyStateWin.cpp, and 1 `(CScn*)` in CMenuMakeCrystal.cpp, where `MCSubObjView::scene5C` is now typed
  `CScn*`). Added: 2 `static_cast<CProc*>` downcasts on `CTaskGame::unk6C`. The field is typed
  `CWorkThread*` per `__ct__9CTaskGameFP5CViewP11CWorkThreadi`, and the callers use `CProc::pssDetachView`.
- Side fix: `hasOptionMenu` lost its C linkage once CTaskGame.hpp stopped acting as an umbrella.
  Its `extern "C"` declaration now lives in its owner header, `kyoshin/menu/CMenuOption.hpp`.
- Regressions: **0**.
  - All 1271 compiled objects are identical to the pre-pilot objects in .text, .data,
    .rodata, .sdata, .sdata2, .ctors, .bss and .sbss, in relocations, and in symbol tables. The
    only difference is the numbering of anonymous `@NNNN` labels.
  - `regcheck.py` over the 179 affected units gives the same table as baseline.
  - `./ninja -k 0 all_source`: 0 FAILED.
  - `run.py link-gate`: OK (missing=86, unchanged).
- Effort: about 3 agent-hours and about 12 locked full builds (about 2 min each, incremental).
  Most of the time went into the gate, not the edits.

## Copies kept, and why

| Copy | Reason |
|---|---|
| `CScnMem.hpp` CScn | The constructor stores vptrs by hand (`field_0x10` / `field_0x54` data surgery), and `~CScnNw4r` needs a single-vptr CScn with `CScnRootNw4rLayout*` at +0x8C. Including the canonical header fails to compile (`field_0x10` undefined). Removing it needs the vtable surgery reworked first. |
| `CMenuKizunaTalk.hpp` CScn | The TU defines its own manual-vtable `struct IWorkEvent`, which clashes with the real `IWorkEvent` base (MWCC 10296). |
| `CMiniMap.hpp` CTaskGame | The TU defines a dtor-less local `IScnRender` (retail uses a manual dtor thunk). The canonical CTaskGame needs the real `IScnRender` base. |
| `CMenuQuestLog.cpp` CTaskGame | Same reason: a local dtor-less `IScnRender` avoids an orphan `__dt__10IScnRenderFv`. |

Each kept copy has a `KEEP (header-consolidation pilot 2026-10-23)` comment.

## Blocked view removals (not CScn/CTaskGame conflicts)

- `realtimeevt/CREvtModel{Map,Obj,Pc}.cpp`: `UnkEvtListEntry` / `EvtBdabModel` / `UnkObj80168514`
  views. Including `cf/CTaskREvtSequence.hpp` fails on `Scn_CallUnk8C_V9`, which has 10 different
  `extern "C"` declarations with different signatures (MWCC 10197). These casts also come from
  memberize misattribution: the model object is passed as `this` of another class.
- `makecrystal/CMenuMakeCrystal.cpp`: the `CMakeCrystalWin` view stays. The real header pulls
  `CMCCrystalBox.hpp`, whose `__ct__UnkClass_8011C974` import clashes (10197).
- `menu/CMenuMapSelect{,SC}.cpp`: the `CMapSelFull` / `CFloorMapFull` views come from the duplicate
  `CCur18View` (CMapSel.hpp vs CFloorMap.hpp).

## Reusable recipe

1. Inventory: `grep -rnE "(class|struct)\s+NAME\s*([:{]|$)" src libs include` (skip `*.ctx.c`).
   Find the affected TUs from `./ninja -t deps`: any source whose deps contain a defining file.
2. Baseline, before any edit:
   - Run a locked `./ninja -k 0 all_source`.
   - Copy every `build/us/src/**/*.o` to scratch.
   - Run `.scratch/regcheck.py` over the affected units.
3. Make the canonical header lean. Use specific includes instead of umbrellas
   (`monolib/work.hpp`, `monolib/scn.hpp`), and forward-declare pointer types. Move `extern "C"`
   imports and layout views into a TU-private `*Imports.hpp` or the TU's private header. A lean
   header is what lets the minimal-stub TUs include it.
4. Merge the members from every copy, checking them against `config/us/symbols.txt`. Declaring
   an out-of-line `virtual ~X()` that retail has, or extra non-virtual members, did not change
   codegen anywhere.
5. Replace the stubs with an include (`.scratch/hdrcons/replace_stub.py NAME header files...`).
   It also removes the `KYOSHIN_MINIMAL_*` guards and the stub comment.
6. Fix compile errors properly:
   - When a TU used the old fat header as an umbrella, add the real owner include, or the
     `*Imports.hpp` for TU-private decls.
   - When an `extern "C"` decl lost its linkage, put it in its owner header.
   - When a TU defines a local copy of a *base* class (IWorkEvent, IScnRender), keep the stub and
     add a KEEP comment.
7. Gate after every step with a locked build plus an object compare
   (`.scratch/hdrcons/step.sh`; normalize `@NNNN` labels, because changing the include order renumbers
   them). Finish with regcheck, link-gate, and a compare of all objects against the saved baseline.
8. Remove the views and casts that existed only because the class was not includable, then
   re-gate.

Tools are in `.scratch/hdrcons/`: `lockbuild.py` (ninja under `build/us/.hexdiff.lock`), `snap.py`,
`step.sh` and `replace_stub.py`.

## Recommendation for the next classes

The real blockers turned out to be **local re-definitions of interface bases** (`IScnRender` ×7,
`IWorkEvent`) and **conflicting `extern "C"` signatures**, not CScn or CTaskGame themselves.
Suggested order:

1. **`IScnRender`** (7 definitions). It unblocks the last two CTaskGame copies, and some CScn users.
   The hard part is the dtor-less variant. Try a header variant without the dtor, controlled by a
   macro (the same scheme as `IWORK_EVENT_INLINE_DTOR`), so that one definition serves both.
2. **`CCur18View`**. It unblocks the CMenuMapSelect/SC `CMapSelFull`/`CFloorMapFull` casts.
3. **`Scn_CallUnk8C_V9` and the other duplicated `extern "C"` imports**. They are not classes, but
   they block CREvtModel* from including `CTaskREvtSequence.hpp`. `Scn_CallUnk8C_V9` is probably a
   CScn member; memberize it into the canonical `CScn`.
4. **`CEventFile`, `IScnRender`-adjacent `CfObject`, `CfGameManager`**. Apply the same recipe:
   make the owner header lean first, then replace the stubs.
5. **`CBattleManager`**: wait until the match-battle agent releases `CBattleManager.cpp`.

Expect most stub copies to be method-only declarations, which are safe to replace. The kept
copies will be the ones that model vtables by hand.

## Round 2 (hdr-consolidate-2)

Every item was gated the same way:
- Snapshot all objects before the item.
- Rebuild under the lock with `-k 0 all_source` (0 FAILED every time).
- Compare every object (normalized, `@NNNN` ignored); only intended objects changed.
- Run `.scratch/regcheck.py` over the defining and including units. There were no regressions.
- `run.py link-gate`: OK (missing=86, multiply_defined=0, sda_reloc=8).

Scratch tools are in `.scratch/hdrcons2/`: `gate.sh`, `snapall.py`, `cmpobj.sh`, `asmdiff.sh`, `err1.sh`, `who.py`, `tryinc.py`, `cscan.py`.

### 1. IScnRender, plus the CTaskGame copies

- `monolib/scn/IScnRender.hpp` is canonical. `ISCN_RENDER_NO_DTOR` (define it at the top of the .cpp) selects the dtor-less variant.
  - IScnRender went from 7 definitions plus 1 renamed copy (`IScnRenderCB`) to 1 canonical definition plus 2 KEEP PODs:
    - `CMenuUpdate.hpp` keeps it because of the retail `__ct__IScnRender`.
    - `CMenuKizunaTalk.hpp` keeps it because of its manual vptr stores.
- The CTaskGame copies in `CMiniMap.hpp` and `CMenuQuestLog.cpp` are gone. They now include `kyoshin/CTaskGame.hpp`, which leaves 1 definition (was 3).
- `CMenuSkipTimer`: 4 reinterpret_casts were removed.
  - The only object change is the intended RTTI string `IScnRenderCB` → `IScnRender`.
  - Its data section was already non-matching.
- The CScn KEEP copies (`CScnMem.hpp`, `CMenuKizunaTalk.hpp`) are still blocked.

### 2. CCur18View, CCur18Data, CCursor18

- The canonical type is `CCur18` in `kyoshin/CBaseCur.hpp`. It has typed virtuals: `initLayout`/`cleanup`/`setRootPaneTranslate`/`checkDeactivate`.
  - The ctor and dtor are declared once there: `__ct__CCur18`, `__dt__6CCur18Fv`.
  - The new opt-in `CCUR18_MEMBER_DTOR` gives the implicit member destruction that `UI::CPassiveSkillLine` needs.
- **Result: 0 view/data/cursor copies remain** in `.hpp`/`.cpp` sources. About 30 TUs were converted, including:
  - CMapSel, CTitle, CQuestWindow, CSysWinSave/Select, CQstLogList, CFloorMap, CTutorialList, COption, CItemBoxGrid/Line, CArtsInfo, CEquipItemBox, CCollepedia, CKizunaTalkList, CSaveLoad, CPassiveSkill, CCol6System.
  - The menu/makecrystal users.
- Each view cast became a typed member call. Each copy block became `member = tmp` (implicit `operator=`, which skips the vptr like retail does).
- Three units kept their raw stack buffers, because a typed local changed the frame:
  - CSysWinSave: `u8[0x18]`.
  - CEquipItemBox: `u8[0x1A]`.
  - CSaveLoad: `char[0xC]` for the button position.
- In CMenuPlayAward, MWCC outlined `__as__6CCur18FRC6CCur18`, so the CBaseCur fields are copied explicitly there.
- Four objects changed on purpose, all toward retail:
  - CMenuMapSelect: the whole-CFloorMap copy now skips the cursor vptr.
  - CMCGetItemBox: the old hand dispatch was wrong; it is now `cleanup()` at +0xC.
  - CKizunaTalkList: the dispatch now uses r12, as retail does.
  - CSaveLoad: the old code called slot +0xC through a hoisted pointer. Retail calls `mCur40.initLayout()` directly, and the new code emits exactly retail's shape.
- Kept:
  - The `CMapSel` stand-in in `CMenuMapSelect.hpp` (KEEP), because conflicting imports between CMapSel.hpp and CFloorMap.hpp block the include.
  - The `CMapSelFull`/`CFloorMapFull` casts, because they need symbols.txt renames (the link agent's lane).

### 3. Conflicting extern "C" imports

- `Scn_CallUnk8C_V9` went from 10 conflicting declarations (`u32(CScn*)`, `void*(void*)`, `ALLOC_HANDLE(void*)`, a block-scope `void*`, and others) to 1 declaration in `monolib/scn/CScn.hpp`: `extern "C" u32 Scn_CallUnk8C_V9(CScn*)`. The definition in `CScn.cpp` now returns the v9 result; its object is identical.
  - It is not memberized, because that would need a symbols.txt rename.
  - The remaining outlier is `CScnItemModelNw4r.hpp` (`CScnItemModelNw4r*` param). The scnImN4* factories' first parameter is probably mis-typed.
- The CREvtModel{Map,Obj,Pc} views (UnkEvtListEntry/EvtBdabModel/UnkObj80168514) are removed. These units now include `kyoshin/cf/CTaskREvtSequence.hpp`. Fixes made along the way:
  - `MapModel_{Dis,En}ablePoints` take no argument; the body never used `self`.
  - The CREvtMem exports (`allocEvtMemAligned`, `freeEvtMemPtr`, `getEvtMemSlot`, `freeEvtMemAll`) now have one declaration, in `realtimeevt/CREvtMem.hpp`. CTaskREvtSequence.hpp and CREvtObj.hpp include it, so the wrong `(void*)` forms are gone.
  - The duplicate decls of getGlobalSda, spawnGimmickEntity, getGimmickListHead, getBdatEntryColumn, Tok_Unpack, isVisionPackLoaded and EvtSeqResolvePackedResId were deleted in favour of the header's.
  - CREvtModelObj keeps signed `int` Tok_Unpack outputs at one site, because retail compares with `cmpwi`.
  - The only object change is CREvtModelMap.o, and only the `__LINE__`-derived FORCEACTIVE dummy name changed.
- Repo-wide scan (`.scratch/hdrcons2/cscan.py`, rough): about 1.1k C-linkage names still have more than one declared signature. The worst are:
  - BdatGetU16Direct (13 sigs / 35 files), getUnk80664658 (13 / 16), sysWinSetKindContent (12 / 21).
  - Scn_FindCamItem (10 / 20), sysWinGetPaneScreenPos (10 / 14), sysWinDrawLayout (9 / 22).
  - CtrlRemote_TouchBitByArg (8 / 61), PaneSetTexPaletteByName (8 / 22), getCfPadData (8 / 12).

### 4. CEventFile, CfObject, CfGameManager

- **CEventFile**: the canonical `monolib/work/CEventFile.hpp` now has the owner's 0x20 layout. It went from 8 definitions to 1 canonical plus 2 owner-side KEEP copies.
  - Removed copies: CArtsInfo.hpp, CModelDispEquip.hpp, CNBanner.cpp, `cf::CEventFile` in CfScript.hpp (it is now global `::CEventFile`, which matches the retail mangling), and the dead stub in `src/device/CDeviceFile.hpp`.
  - KEEP: CDeviceFile.cpp and CDeviceFileDvd.cpp. These TUs keep private CBM/CFileHandle copies and need the CBM ctor.
- **CfGameManager**: the canonical signatures were corrected:
  - static `isTimerActive`;
  - static u32 `getControllerWordA37C`/`getGlobalWord64184Dup`/`getGlobalPtr640A8`;
  - static float `updatePadState`. The CfGameManager.o object is identical.
  - Stubs went from 9 to 3. Removed: CMenuMakeCrystal, CMenuEnemyState, CChainActorPc, CMenuKizunaTalkList, CMenuSymbolMark, CfObjectColl.
  - KEEP, with reasons in the source:
    - CfGimmick: CtrlRemote_SetSharedBit 1-arg view, plus the unkB0 layout.
    - CfMapEffectManager: the header's `MapFx_ReleaseHandle(u32)` conflicts with its definition.
    - CMenuGameClear: `getCfPadData` returns `u32*`.
    - CHelp_GameOver: `lbl_eu_80664A10` is typed `CHelpFlagBag*` vs `CHelpManager*`.
- **CfObject**: no change. All 5 stubs are blocked:
  - pluginCfs: a non-virtual stub vs the canonical virtual `CfObject_getMoveHeadAngle`.
  - CVS_THREAD: `int requestVoice` vs `bool`.
  - CfObjectImplWalker, CfObjectPoint, CfObjectColl.hpp: hand-modelled vtables/layouts.

### 5. CBattleManager

Not done. `cf/CBattleManager.hpp` holds `CChain`/`CSuddenCommu`/`CVision` by value, so it pulls in their fat headers. Including it in a method-only stub TU (CUICfManager, pluginUnit) fails on getUnk80664658 / findObjB28ById conflicts. The prerequisite is lean CSuddenCommu/CVision/CChain headers, with their imports moved to TU-private headers. Then the 9 stubs can be replaced.

### Debt relocated

None moved into new Imports headers. Two declarations moved to their owner headers:
- `Scn_CallUnk8C_V9` → CScn.hpp.
- CREvtMem exports → CREvtMem.hpp, via an include.

## Round 3 (blockers-B)

Every task was gated the same way:
- Snapshot all objects before the task (`.scratch/hdrcons2/snapall.py`).
- Rebuild under the lock with `-k 0 all_source` and compare normalized objects (`.scratch/blockersB/gate.sh`, `cmpobj.sh`).
- Run `.scratch/regcheck.py` over the defining and calling units.
- Run `run.py link-gate` and `link-gate --symbols-only`.

Results:
- Final state: 0 FAILED. The build briefly failed during the round on blockers-A's in-flight `BdatGetU16Direct`/`getUnk80664658` edits, never on these changes.
- regcheck: 0 regressions.
- link-gate: OK, missing=86 before and after (no ratchet). symbols dupes: 0.
- symbols.txt edits went through `symbols_lock.symbols_edit_lock` (`.scratch/blockersB/symren.py`), followed by `targets sync-symbols`.

### 1. CMapSelFull / CFloorMapFull

- Renamed in us and eu (jp keeps its flat names):
  - `getMapGridCell__11CMapSelFullFv` → `getMapGridCell__7CMapSelFv`.
  - `GetActiveFlag58__13CFloorMapFullFv` → `GetActiveFlag58__9CFloorMapFv`.
- Both functions are now members of the real classes:
  - `CMapSel::getMapGridCell` reads `mGridData[mSelX + mSelY]`.
  - `CFloorMap::GetActiveFlag58` reads `cursor_active_58`.
- The `CMapSelFull` view is deleted. `CFloorMapFull` stays CFloorMap.cpp-internal.
- The CMapSel.hpp imports and views moved to a new `kyoshin/CMapSelImports.hpp`, included by CMapSel.cpp and code_802405F4.hpp. CMapSel.hpp is now class-only and can be included next to CFloorMap.hpp.
- **The CMapSel stand-in in `CMenuMapSelect.hpp` is removed**; that header now includes `kyoshin/CMapSel.hpp`.
- Removed casts: 3 `reinterpret_cast` in CMenuMapSelect.cpp and 2 in CMenuMapSelectSC.cpp. Calls are now `self->mMapSel.getMapGridCell()` and `self->mFloorMap.GetActiveFlag58()`.
- Objects: only the symbol and relocation names changed, in CFloorMap, CMapSel, CMenuMapSelect and CMenuMapSelectSC.

### 2. CMenuBattleMode `*Adj5C`

- us-801a1bb0 and us-801a1bb8 are 8-byte `subi r3,r3,0x5c; b target` IScnRender (+0x5C) this-adjustors. MWCC emits exactly these as `@92@cbRenderBefore__15CMenuBattleModeFv` and `@92@__dt__15CMenuBattleModeFv`.
- The sibling `@88@__dt__15CMenuBattleModeFv` (IWorkEvent +0x58) already used the compiler name.
- The `CMenuBattleMode_*Adj5C` names were invented, and retail has no names, so symbols.txt was renamed in us, eu and jp to what MWCC naturally emits. No source change apart from a comment.
- Re-cycled with batch-cycle: both are FULL_MATCH, certified.

### 3. Scn_CallUnk8C_V9 → `CScn::getAllocHandle()`

Evidence:
- 0x8049A068 sits in the CScn TU, between `addRenderCB__4CScnFP10IScnRenderUlUl` / `removeRenderCB__4CScnFP10IScnRender` and its V6/V7/V10/V8 siblings.
- r3 is the CScn `this`. The function reads `CScn::mUnk8C` (+0x8C) and tail-calls vtable+0x2C.
- Every caller feeds the result to `mtl::MemManager` as an ALLOC_HANDLE.

Changes:
- symbols.txt (us/eu): `Scn_CallUnk8C_V9` → `getAllocHandle__4CScnFv`.
- The `extern "C"` declaration is removed from monolib/scn/CScn.hpp and the CScnItemModelNw4r.hpp outlier is deleted.
- 25 call sites in 12 TUs became member calls.
- The two `scnImN4BuildByIdx`/`scnImN4CreateModel` sites keep a `reinterpret_cast<CScn*>(self)`. Their first parameter really is the scene (`lbl_eu_80663E14`) but is still typed as the model; retyping those factories is separate work.
- Objects: only the call name changed, in 12 objects.

### 4. Type reconciliations

**`lbl_eu_80664A10` is `cf::CHelpManager*`.**
- Evidence: CHelpManager's ctor stores `this` into it and `destroyHelpManager` deletes it.
- `CHelpFlagBag` was a field view of `CHelpManagerData`. The mapping is +0x14 `mField14` (ItemCole), +0x15 `mField15` (Kizuna) and +0x18 `mField18` (GameOver).
- The views and the `extern "C" CHelpFlagBag*` declarations are removed from CHelp_GameOver, CHelp_Kizuna and CHelp_ItemCole. Those TUs now include `kyoshin/cf/CHelpManager.hpp`.
- **The CHelp_GameOver CfGameManager stub is replaced by `kyoshin/cf/CfGameManager.hpp`.**
- `isHelpPopupReady` is now declared once, in CHelpManager.hpp, as `extern "C" int isHelpPopupReady(cf::CHelpManager*)`. The retail and symbols.txt name is unmangled, and CHelpManager.o used to emit `isHelpPopupReady__FPQ22cf12CHelpManager`.
- Objects: CHelp_* are identical. CHelpManager.o's only change is the symbol name going to `isHelpPopupReady` (toward retail).

**`CfGameManager::unkA0` is `cf::CfMapEffectManager*`.**
- Evidence: it is built by `__ct__cf_CfMapEffectManager`, released by `MapFx_ReleaseHandle(CfMapEffectManager*)` (the owner definition) and deleted through the vtable.
- `getMapEffectManager()` now returns `CfMapEffectManager*` (the mangled name is unchanged).
- The header import is now `MapFx_ReleaseHandle(cf::CfMapEffectManager*)`, and the duplicate in CfGameManagerUnityHelpers.hpp is removed.
- **The CfMapEffectManager CfGameManager stub is replaced by the canonical header.**
- Objects: CfGameManager, CfMapEffectManager and CfResPcImpl are identical.

**CfObject `requestVoice` stays `bool`.**
- Retail callers test r3 with `cmpwi` and the body tail-calls `playGated`. A local MWCC test shows that `bool` and `int` returns give byte-identical code for both patterns, so the asm cannot tell them apart. The owner chain (`playGated`, CCharVoice.cpp) is `bool`.
- The CVS_THREAD `int` stub was dead: the TU does not call it yet, since TryRequestVoice and RequestVoicePlayback are still asm. **The stub is deleted** and CVS_THREAD.o is identical.

## Round 3 (blockers-A)

Gate after every batch:
- Snapshot all objects before the batch.
- Rebuild under the lock with `-k 0 all_source`.
- Compare every object, normalized with `@NNNN` ignored.
- Run `.scratch/regcheck.py` over the touched units.
- Run `run.py link-gate`.

Final state: 0 FAILED. All objects are identical to the last gated baseline. There are 0 regcheck regressions, and link-gate is OK (missing=86, multiply_defined=0, sda_reloc=8).

Every object change from this round moves toward retail. Objects changed by concurrent agents (blockers-B's symbol renames, match-large's mpvabdec) were excluded from attribution.

Scripts are in `.scratch/blockersA/`: `gate.sh`, `chk.sh`, `errs.sh`, `swapdecl.py`, `autofix.py`, `rep.py`.

### Goal 1: conflicting C-linkage declarations

cscan count: 1197 before, 1195 after. The count is rough: it includes parser noise, and the Gim* family was never counted.

**getUnk80664658 plus the Gim\* family (code_801F3BE0)**
- New owner header `kyoshin/code_801F3BE0.hpp` holds:
  - the `CGimmickGlobal` layout;
  - `extern "C"` declarations of getUnk80664658, the ctor/dtor, and every `Gim*` export.
- The owner TU now emits the retail C names. Before, it emitted `getUnk80664658__Fv`, `GimFindPosByRow__FPC14CGimmickGloball`, and so on. This is the only object change.
- 14 view structs and their declarations were removed, replaced by the real type:
  - CMMGlobalGimmick, CUICfGlobalSettings, CMainMenuGimmickGlobal, CtrlUnk64658View, CtrlEnemyGimmickGlobal, CfGlobalSettings, CFunc8008A2C8Global, UnkGimmickGlobalView, CSuddenCommuGlobal, CfGimmickGlobal, CMenuGimmickGlobal, CfWalkGimmickGlobal, the `void*` declarations in code_800B06A4.hpp / CtrlMoveEne.hpp, and pluginCfs `int`.
  - Casts like `((cf::CtrlEnemyGimmickGlobal*)getUnk80664658())` became `getUnk80664658()->mFlags`.
- Other fixes:
  - The `#define getUnk80664658 cbattleGetUnk80664658` rename in CfGameManager.cpp is gone.
  - `UnkClass_805764CC::field_0xCFC` is now typed `CGimmickGlobal*`.
  - `GimSpawnAll` and `GimUpdateGlobal` take `(self, obj)`, matching the `notifyObjCFC*` retail callers.
  - `GimProbeLockAccept` takes a position (`const ml::CVec3*`).
  - The GimDeactivateLocks caller now passes the getter result, which is the retail r3.
  - CMiniMap passes `(u16)dispId`. Retail does `clrlwi`, and the callee's `cmpw` confirms the `s32` parameter.
- Not converted: CFloorMap.{hpp,cpp}, owned by blockers-B. Its view is now the only remaining declaration.

**BdatGetU16Direct**
- Canonical declaration `u16 (const void* bdat, const void* column, int row)` in the new lean owner header `kyoshin/code_80135FDC_Bdat.hpp`. It is included only by the owner .cpp and the callers, not by the fat code_80135FDC.hpp. 28 local declarations were removed.
- Retail has both shapes. Most callers `clrlwi` right after the call, which means a u16 return. But CArtsInfo_CalcDmg52/53, getEIBProgress and clpGetCelIcon use the 32-bit result untruncated.
- KEEP (each has a comment):
  - CArtsInfo.hpp `u32`;
  - CEquipItemBox.cpp `int`;
  - CCollepedia.hpp `u32` (tail call in clpGetCelIcon);
  - CMCGetItemBox.hpp and CMenuSymbolMark.hpp `u32` (the u16 form lowers getMCItemSubText / SymMark_AddTouchMark match);
  - CItemBoxLine.hpp `u32 table`: the TU models every BDAT table as a u32 handle.
- Not converted: CFloorMap.hpp / CMapSelImports.hpp (blockers-B) and CItemBoxInfo.hpp (match-large). Their form is identical to the canonical one, so they do not conflict.

**bindIndexedEffect**
- True signature `void* (int index, void* manager)`, from the definition in CTaskGameEff.cpp. It is declared once in the new thin `kyoshin/CTaskGameEffApi.hpp`, and 10 local declarations were removed.
- The definition now emits the retail C name (it was `bindIndexedEffect__FiPv`). CMenuZeal's call reloc became `bindIndexedEffect` (it was `__FUlPv`).
- KEEP: CBattleManager.cpp's local `(u32, int)` declaration. That file is owned by match-battle, so its call sites with `(int)` casts were not touched.

**ChainMusic_Save/RestoreSlots**
- Not fixed. The definition takes `cf::CChainMusic*`, but the callers pass `&mChainTemp` (`CChainTemp*`): the same object is modelled twice.
- The existing `#define` shield in CChainActorList.cpp now wraps `CChainTimerImports.hpp`, which is where the `void*` declarations moved.

**Not attempted (next angles)**
- sysWin*/CSysWin_* cluster (8 names in the top 20): about 40 caller TUs hold the window as `void*`/`u8*` fields, and the u8 vs int returns are load-bearing. The callers' window fields need to be retyped to `CSysWin*` first.
- Scn_FindCamItem: no source definition (an 8-byte thunk), and every caller casts the result to a TU-local pose view.

### Goal 2: CBattleManager

Stub copies went from 9 to 2.
- Replaced with `#include "kyoshin/cf/CBattleManager.hpp"`: CUICfManager.cpp, code_802B8A3C.hpp, CArtsParam.cpp, pluginUnit.cpp, pluginBtl.cpp, CMenuVision.cpp, CMenuEnemyState.cpp.
- Left:
  - CHelpManager.hpp, owned by blockers-B.
  - CfObjectImplPc.cpp, KEEP: it walks the +0x28/+0x48 list sentinels through its own `CfBattleChainNode` type, while the real class holds `reslist<CfObjectActor*>`.

Lean-owner work:
- `cf/CBattleManager.hpp`:
  - It keeps the class plus the types it holds by value. CfObjectActor is now forward-declared, and `monolib/util.hpp` was replaced by `reslist.hpp`.
  - Its 300 lines of CBattleManager.cpp-only views and imports moved to `cf/CBattleManagerImports.hpp`. The one-line include in CBattleManager.cpp changed.
- `cf/CSuddenCommu.hpp` keeps the class only. Its views and imports moved to `cf/CSuddenCommuImports.hpp`.
- `cf/CVision.hpp` keeps the classes. Its imports moved to `cf/CVisionImports.hpp`.
- `cf/chain/CChain.hpp`, CChainActorList.hpp, CChainTimer.hpp, CChainTime.hpp, CChainCombo.hpp and CChainEffect.hpp keep their classes. Their imports moved to per-header `cf/chain/C*Imports.hpp` files; `CChainImports.hpp` aggregates them for CChain.cpp.
- `chain/UnkClass_800D8DBC.hpp` forward-declares CfObjectActor instead of including CfObjectActor.hpp.
- Owner declarations added:
  - `lbl_eu_80663E28` in CfGameManagerData.hpp (CfCam's duplicate removed);
  - `__RTTI__Q22cf13CfObjectActor` in CfObjectActor.hpp.

Object changes, both toward retail:
- CfGameManager.o: its call reloc is now retail `CBattleMan_FetchVisionObj`. It used to be renamed by CAIAction.hpp's `#define CBattleMan_FetchVisionObj CBattleMan_FetchVisionObj_CAIAction`.
- CfObjectImplEne.o: the `_CAIAction` infix is gone.

**Debt relocated.** These TUs used to receive imports through the fat public headers. They now include the TU-private Imports header that declares them, so visibility is unchanged and the debt is explicit:
- pluginEve.cpp: CVisionImports.
- CAIAction.cpp: CChainActorListImports.
- CActorParam.cpp: CVisionImports and CChainActorListImports.
- CMenuArtsSelect.cpp: CChainImports.
- CMenuBattlePlayerState.cpp: CChainImports.
- CVision.cpp: CChainImports and CSuddenCommuImports.
- CChainTime/CChainTimer/CChainActorList.cpp: CVisionImports, and CChainTime also CSuddenCommuImports.

### symbols.txt renames needed (blockers-B lane)

- `addTableValueWithClamp__Q22cf13CfGameManagerFv` is a pre-mangled `Fv` C name. It collides with the `CfGameManager::addTableValueWithClamp()` member, which forces `#define` renames. Fixing it needs a memberize/rename with the real `(s32,s32,s32)` parameters.
- `chainResolveMemberPtr` / `chainResolveMemberFromSrc`: retail names are unmangled, but the definitions emit `__FUli`/`__FUlPUl`.
- `CBattleMan_FetchVisionObj`: CfObjectImplEne still references the member-mangled `CBattleMan_FetchVisionObj__Q22cf14CBattleManagerFv`; retail is the C name.

## extern-C mangled cleanup (2026-10-23/24)

Goal: retire `extern "C"` declarations and definitions that spell C++ mangled names
(`foo__3BarFv(self)`), so the source states what the symbol already is: a member, a static
member, or a C++ free function. The work was symbol-neutral; nothing was renamed in `symbols.txt`.

Scanner: `.scratch/externc/scan.py` uses `dtk demangle` and indexes classes across all of
`src`, `libs` and `include`. It classifies each name as:
- (a) class defined in one header;
- (a?) class defined 2 to 6 times (a canonical class plus views);
- (b) free or namespace function;
- (c) class only defined in a `.cpp`, or class undefined;
- (x) not actually mangled (`__ct__80043E88`-style placeholder names).

Scan counts, before (pre-edit sources) → after:

| class | names | decl/def sites |
|---|---|---|
| a | 1127 → 1123 | 2384 → 2294 |
| a? | 1326 → 1324 | 4399 → 4370 |
| b | 64 → 59 | 451 → 215 |
| c | 140 → 140 | 163 → 163 |
| total mangled (excl. x) | 2657 → 2646 | 6584 → 6229 |

Converted (5 batches plus 2 smell-fix rounds):
- **Members and statics:**
  - `CLibLayout::createArcResourceAccessor` and `getAllocHandle`: made static, because the bodies only use the singleton. The public `monolib/lib/CLibLayout.hpp` now declares `getAllocHandle` static and returns `mtl::ALLOC_HANDLE`.
  - `CTitleAHelp::isIdle`: now returns `int`. Retail callers test it with `cmpwi`; a `u8` return makes MWCC add a `clrlwi.` mask, and a `bool` field changes the struct copies.
  - `CTaskGame::getInstance` and `isFlag01Set`: calls become `CTaskGame::getInstance()->isFlag01Set()`.
  - `CViewRoot::getInstance`, `CLibCri::getInstance`, `CBdat::getEntry`, `CBdat::setBdatEntry`, `cf::CfSoundMan::playActorSound`.
  - `cf::CfObject::CfObject_UnkVirtualFunc34`: real member definition in ocUnit.cpp.
- **Free and namespace functions:** `operator delete`/`operator delete[]` (from `__dl__FPv`/`__dla__FPv`), `playUISound`, `getFP` (through `ocBdat.hpp`), `getScnHandle` (declared in `monolib/scn/code_804BC9EC.hpp`), `buildLayout`, `bindLayoutAnimTransform`, `advanceAnimTransform`, `nw4r::math::FrSqrt`, `nw4r::db::Panic`.

Real bugs fixed along the way. These are the only object changes, each one verified:
- `CLibHbmControl` and `ut_PackedFont` declared `__dl__FPv` with C++ linkage, so they referenced a bogus `__dl__FPv__FPv`. They now reference the real `__dl__FPv`.
- code_804BC9EC defined `int getScnHandle__Fv(u8*)`, which emitted the bogus `getScnHandle__Fv__FPUc`. It now defines `void* getScnHandle()`; `getScnHandle__Fv` is 100% and is now emitted. Its `FrSqrt` reference was also bogus (`FrSqrt__Q24nw4r4mathFf__Ff`) and is fixed.
- code_80135FDC defined `closeFileHandle__FPP11CFileHandle__FPP11CFileHandle`. It now defines the retail `closeFileHandle__FPP11CFileHandle`, and the data-table reloc follows.
- CSysWinSelect.hpp declared `playUISound` inside an `extern "C"` block (unmangled). That bogus declaration was removed.
- CTaskEnvironment's dead `extern "C" func_800586E8() {}` was removed. There is no US symbol for it; it is an EU name for `copyWord4`, which is defined separately. The `.text` overrun went from 8 to 4 bytes.

KEEP (each has a comment in the source):
- CMenuKizunaTalk `createArcResourceAccessor__10CLibLayoutFv`: CLibLayout.hpp brings the canonical IWorkEvent, which clashes with the TU's KEEP copy.
- code_801A929C and CfMapItemManager `playActorSound__...`: including CfSoundMan.hpp clashes with local copies.
- CfObjectSelectorObj `__dl__FPv`/`__dla__FPv`: `operator delete[]` changes the extab cleanup actions. This is the only extab-sensitive case found; the snapshot gate now hashes `extab` and `extabindex`.
- CTaskEnvironment `Draw__16CTaskEnvironmentFv`, `Reset__14CChildListNodeFv`, `Tail__8CProcessFv`: their addresses are taken as `u32` in hand-built `.data` vtables, and a PTMF cannot be cast to `u32`.

Blocked (next angle):
- `CProcess::Regist` (19 decls): callers pass view classes that don't derive from CProcess. Next: real inheritance for the CMenu*/CScn views.
- `CLibLayout::createPicture`: uses `this` while callers pass nothing (arity conflict in retail). Next: match the body first.
- `closeFileHandle`/`findObjectById` caller conversion: about 20 callers pass `void**`/`u32*`/`int*`, so they need field retyping. The definitions are fixed.
- (a?) classes with 2 to 6 definitions: needs header consolidation first, using the hdrcons recipe.
- (c) classes defined only in a `.cpp`: needs a header split.
- Arity-conflict names (about 560 names; `Fv` names that take arguments, or dtors with a hidden `int`): the symbol is right but the source signature is wrong, so each needs the body matched before it can be declared truthfully.

Gate after every batch: snapshot all objects (`.scratch/externc/snapall.py`, now including extab), then a locked `-k 0 all_source` (0 FAILED). Unexplained diffs were bisected with a pre-edit mirror compile (`.scratch/externc/odiff.sh`, `extab_audit.py` over every touched TU). Then regcheck on touched units (0 regressions in every batch) and `run.py link-gate` (OK, missing=85 throughout).
Scripts are in `.scratch/externc/`: `scan.py`, `conv.py`, `keep.py`, `gate.sh`, `mirror_sync.sh`, `odiff.sh`, `extab_audit.py`, `smlines.py`.
