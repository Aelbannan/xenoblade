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

## 2. Where we are (2026-11-18, HEAD of `agents/matching-pass-2026-10-23`)

| Measure | Value |
|---|---|
| Functions at 100% | 13,519 / 19,310 (70.0%) |
| Units complete (linked) | 617 / 1,191 |
| Matched code / data bytes | 40.4% / 52.2% |
| Unmatched functions by area | kyoshin 3,712 · monolib 1,096 · CriWare 405 · nw4r 346 · RVL_SDK 230 · MSL 2 |
| Faithful hard counts | label 48,460 · mangled_c 2,941 · section 1,458 · offset_cast 6,089 · view_struct 3,228 · rule 276 · rtti_off 20 · foreign_data 643 |
| Faithful soft counts | field_0x 27,891 · unk_name 1,125 · addr_func 1,013 · noinline 1,880 |
| UNIT_RULES baseline | 269 (shrink-only) |

Largest unmatched kyoshin units (unmatched / total functions): `code_800B06A4` 148/253,
`cf/CItem` 91/169, `cf/CfRes` 70/224, `cf/CfCam` 69/181, `CArtsInfo` 66/93, `plugin/pluginCfs`
65/73, `action/CActParamData` 64/178, `CFloorMap` 64/75, `cf/CfGameManager` 62/352,
`action/CActParamAnim` 61/144, `cf/voice/CCharVoiceMan` 57/60, `CUICfManager` 56/89.

Gate state at the checkpoint commit: link OK, extab OK, text-order OK, frozen rules OK, registry
OK. Faithful ratchet FAILS on three units (see §5 "Immediate debts").

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
