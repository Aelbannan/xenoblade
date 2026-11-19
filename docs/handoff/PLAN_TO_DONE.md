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

## 2. Where we are (2026-11-20, checkpoint 10 on `agents/matching-pass-2026-10-23`)

| Measure | Value |
|---|---|
| Functions at 100% | 15,110 / 19,310 (78.2%); checkpoint 9 was 14,720 (+410 reached 100%, 20 apparent losses: 15 renames, 2 proven-phantom CfGimmick checkers, 3 pool-reloc win-backs logged in §5 c10) |
| Units complete (linked) | 630 / 1,198 (c9: 626) |
| Matched code / data bytes | 45.2% / 53.2% |
| Unmatched functions by area | kyoshin 2,677 · monolib 835 · CriWare 316 · nw4r 197 · RVL_SDK 174 · PowerPC_EABI_Support 1 |
| Faithful hard counts | label 24,195 · mangled_c 2,053 · section 882 · offset_cast 1,841 · view_struct 1,191 · noinline 133 · decl_shield 17 · volatile 117 · rule 272 · rtti_off 15 · foreign_data 410 |
| Faithful soft counts | field_0x 27,917 · unk_name 1,135 · addr_func 1,015 |
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

Largest unmatched kyoshin units (unmatched / total functions): `code_800B06A4` 116/253, `CArtsInfo` 66/93, `cf/CfGameManager` 62/352, `action/CActParamAnim` 61/144, `CUICfManager` 56/89, `cf/CtrlObjectParam` 47/109, `CFloorMap` 47/75, `cf/CPartsChange` 45/84, `CPassiveSkill` 44/127, `code_80135FDC` 43/124, `cf/CfNandManager` 43/48, `cf/CfSoundMan` 40/63.

Checkpoint 7 (wave c8 + hy4 stopgap): noinline 1,108 → 485 via real unit flags (`-inline noauto`; `-O4,s` always
paired with `-func_align 4` because `-O4,s` resets func_align to 16), real CScn/CScnRoot/CVirtualLight classes with
compiler-emitted MI vtables, CSysWinSelect merged into CSystemWindow, CTagProcessor in-TU tag table, RVL scalar
.sbss flags, all 37 CriWare adx .bss ranges re-cut to the JP ownership in US/EU.

Gate state at checkpoint 7: link OK (missing 43), extab OK (ratcheted), text-order OK (11), frozen rules OK (269;
CScn_80496B0C pad 0x130 → 0x40 ratcheted), registry OK, faithful OK (ratcheted, 68 units).
Report: `/tmp/xb_ckpt/report_c7.json` (c6: `report_c6.json`).
NB `run.py progress` only rebuilds `build/us/report.json` when it is missing — move it aside first.

Checkpoint 8 (hy4-flash-free wave, ~60 agents in match + cleanup pipelines): noinline 485 → 136, view_struct
2,736 → 1,222, offset_cast 2,669 → 1,865, mangled_c 2,585 → 2,222, label 45,864 → 44,589. Compiler-emitted
vtables replaced hand tables in `_reslist_base<IUIWindow*>`, CLibCriMoviePlay, CUIBattleManager and CLoad; real
`nw4r::lyt::Pane` visibility API replaced views; RTTI on for CBattleManager. Registry: 169 stale `FULL_MATCH`
rows (accepted on objdiff-era 100%, failing the current byte gate on pool relocs) downgraded to `CODE_MATCH`;
census in `/tmp/xb/h22/census_summary.md` (content alone can't pick a retail pool name: 199 `.sdata2` slots hold
the same 2^52 magic). Orchestrator reverted hy4 counter dodges: `#define` offset names (adx_fs/mwsfdsvr), a
TU-local CVec3 pun (CfCamEvent_1), a blank-line padding swap (CFloorMap.hpp, cost CMenuMapSelectSC 6), invented
vtable/singleton names (CUIBattleManager, redone with compiler-emitted tables). Those bans are now in the wave brief.

**`-ipa off` lesson:** CCollepedia's `-ipa off` was compensating for a `{ return 0; }` stub that `-ipa file`
folded into callers; with the real bodies restored (they existed under invented names) the flag is neutral-to-worse
and was dropped. Treat any `-ipa off` as suspect until the TU has no stubs.

Gate state at checkpoint 8: link OK (missing 43), extab OK (26), text-order OK (ratcheted, CLibCriMoviePlay 9 → 8),
frozen rules OK, registry OK, faithful OK (ratcheted, 109 units; COption raised 1 → 7 by hand because removing
its `*Imports.hpp` side header exposed 6 labels the header hid — honest rise, not a regression).
Report: `/tmp/xb_ckpt/report_c8.json`.

Checkpoint 9 (hy4 wave i + Opus near-complete/regalloc batches): +222 functions, 9 units linked (snd_SoundArchive,
snd_ExternalSoundPlayer, muldiv, hcicmds, adx_hdr, sfh_ver1, CfObjectNpc, port_rfc, sdp_api); link missing 43 -> 42.
nw4r snd `detail::WaveInfo` swap (real runtime/on-disk split, view structs and `#define private public` removed);
20 hidden vtables recovered as `__vt__` names (CFade, CChain*, CTTask<...>, reslist bases, CLibVM, CPackItem...) —
hand-written vptr stores using old `lbl_eu_*` names must then be deleted (CBattleManager dtors); cvsys PTMF state
handlers became real members and factories real `static create()`; CTitleAHelp::isInitialized member in 19 menus.
Orchestrator rejections: split-budget shrink + hand-written `__dt__XXXX` (CUICfManager), pre-mangled caller externs
(CCharVoiceMan), `_Local` duplicate structs (CfNandManager), dropping `-ipa off` on bta_dm_pm (reverses .text order),
flipping g3d_resfile (multiply-defined lbl) and code_800F42AC (C++-mangled names where retail exports C names).
Orchestrator lint `/tmp/xb/orch/hacklint.py` now also flags padded overlay structs (`u8 _XX[0x..]`).

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

0. **Checkpoint 10 debts (2026-11-20).** Live queue: `/tmp/xb/orch/debt_queue.md` (orchestrator scratch); the durable items:
   - code_800B06A4 `setMonoFlag` (us-800ba1e4) hand-calls `__dynamic_cast(obj, 0, &__RTTI__..., &__RTTI__..., 0)`; rewrite as `dynamic_cast<cf::CfObjectModel*>`, move the per-TU `startEnumObjects`/`getObjectIdAt` externs to their owner header (or revert the function).
   - CDesktop ctor still stores `__vt__8CDesktop` by hand although the vtable is now compiler-emitted; `lbl_eu_80663760` type descriptor is a `__declspec(section ".sdata")` blob.
   - Win-backs from faithful cleanups (do not reintroduce offset reads): CfTFile `CfT_PlayTimeGet` 100->99.44 (decomp-only int->double magic reloc, split overrun 0x6C); CMCCrystalInfo `MCCrystal_ClearSlots/ClearTexts` 100->99.8 (string literals replaced `lbl+offset`); CCollepedia `clpSelectItem` 92.4->75.4 / `clpFillEntry` 76.3->61.7 (partial CLPPage/CLPCell typing); adx_tlk `adxt_ExecServer` 94.1->79.0 (ADXT_ServerWork typed).
   - sfx_lib: split into `sfxlib_header` (0x80619C00, 0x10) + `sfxlib_state` (0x80619C10) gives the four accessors ✓ but `SFX_Init` 81.66->76.09 with split overrun 0x1C; single-object + state-pointer probe gives 0/4. Retail accessors use an interior label +8 while SFX_Init uses the outer base +addi 16 — needs the real struct/TU shape (Opus tier).
   - 668 N1 renames reverted because callers declare the C name (`extern "C"`); migrate callers to the owner header, then retry `rename_syms.py` (list in `/tmp/xb/N1/risky.json`). 1,076 byte-identical-but-unpaired functions (mostly this linkage class) are the cheapest remaining wins.
   - 23 `...Fv` placeholder names whose callers pass r4/f1 payloads: re-mangle to the real signature and write the real member.
   - Opus tier (await approval): 8-byte singleton sbss slots; vtable-before-data ordering (CPartsChange, CWorkSystemPack); leading .sdata2 prefix (CfTFile, CArtsInfo); IWorkEvent RTTI string (CMCCrystalInfo, CArcItem); TU string-pool order (CItemBox*, CArtsInfo, CSaveLoad); CfGameManager.hpp/code_800B06A4.hpp fake reslist (193 includers); ahx_sjd and CTaskREvent volatile; math_triangular hand-written BasicSound vtable blob.
   - Process lesson: hy4 match agents on >=90% residuals yield ~3.5%; packet-format briefs (hexdiff diff + KB/semantic search hits + exemplars) yield ~36% on recipe/linkage blockers. Pure register colouring stays with Opus.

0. **Checkpoint 9 debts (2026-11-19).**
   - CSkipTimer (wave h, h12): BeginLoad/OnFileEvent use a hand-expanded member-wise copy from a block-scoped temporary instead of the probable original `mSysWin = CSysWin(0);` / `mSkipTimer2 = CSkipTimer2(mLayout);` (implicit operator= is not inlined under the unit's real -ipa off). Accepted because it replaced 26 noinline + 19 pragmas; revisit.
   - CTaskGame (h49): volatile re-reads moved to `volatile` field declarations on UnkClass_8004041C (counter no longer
       sees them). Plausible if the stream state is written from a callback thread; unverified. Removing volatility
       costs CTaskGame_updateStream 100->97.8%.
   - CfCamEvent_1 ctor (us-80075c34) 37.6%: retail inline word copies of ml::CVec3::zero; h46's u32[3] pun reverted
       by orchestrator. h54 researching a faithful trivially-copyable CVec3.
   - Next-wave leads (h42): CActorParam.hpp virtual return types (UnkVirtualFunc17/32/44/45/79/80/93/97/99/104/128/139/151/162/169/172 declared void, bodies return values); CBattleState.hpp readStatusSlot/getEventSlot/fetchStatusSlot real types; CfObjectMove real member at +0x6EC and MI subobject offsets.
   - Next-wave leads (h57): CSaveLoad 5 mangled_c (CfAward dtor address for __register_global_object; processAward Fv with hidden int).
   - CTaskGameCf::create (h50) calls ctor by mangled ABI name to avoid placement-new null guard — counted; find the real allocation shape.
   - String-pool blobs renamed (sMakeCrystalStrings 0x8050888C, CPresentStringPool, UIBattleArcPathStrings, sMenuEnemyNamePool, sBdatColumnNames, adx*/ahx* message tables): references are still `blob + offset`; label count dropped (CMCCrystalBox 251->64) but the original shape is per-use string literals emitted by the compiler. Real fix: replace blob+offset with literals when the TU's pool order/size can be reproduced. Faithful counter should probably count `<named blob> + 0x..` as offset_cast — tooling follow-up.
   - CMenuSymbolMark (h55): hand-built vtables renamed to invented `sSymbolMarkItemVtable`/`sSymbolMarkItemVtableB` (0x8052CDB4/0x8052CD50) — violates the vtable-rename rule added mid-wave; next wave: real classes with compiler-emitted `__vt__` (h63 recipe), else restore honest lbl names.
   - Field-level `volatile` added to dodge the per-use volatile counter: CTaskGame.hpp (UnkClass_8004041C unk4/unk14, h49), CActParamAnimGame.hpp field530 ("retail reloads it"), ahx_sjd `const volatile u32 ahxsjd_init_flags[]` (h38). Faithful counter should count volatile field/object declarations too; then re-justify each.
   - CtrlAct.hpp: cf::CtrlAct still declares placeholder virtuals vf00..vf19 (pre-existing at c7, now one per line). Real slot names needed from the retail vtable lbl_eu_8052B080.
   - COption: new `optionAnimFrame()` inline wrapper around extern lbl_eu_80668C10 (h16) — check whether retail really reads a foreign sdata2 constant or a literal.
   - CVec3 (h54, cancelled): removing the user-declared copy ctor makes MWCC emit retail's inline word copies; sweep was inconclusive (taken during churn). Header reverted to HEAD; CfCamEvent_1 now uses real ml::CVec3 fields (ctor 80.6%, honest). Redo the CVec3 blast-radius sweep on a quiet tree next wave.
   - RESOLVED: CCollepedia `-ipa off` was compensating for dummy stubs (func_80254144 `{return 0;}` folded by -ipa file). Real bodies restored from invented-name twins (CCollepedia_getFieldOffset/getField12/getFieldAtIdx; deleted non-retail CCollepedia_decrementWrap); flag dropped, 64/80. DEBT: type the +0xE8 page catalog (0x140-byte pages, 6x0x34 rows, 5x0xA cells) instead of u8* offset math.
   - CEquipItemBox (9 fns objdiff 100->99.8, hexdiff ✓ unchanged): CEquipItemBoxImports.hpp declares a bogus C++ overload `void* CItem_initItemImplInstances(void*)` so calls mangle to __FPv instead of retail's C symbol. Fix = delete that decl + side header, route all ~53 calls through canonical `extern "C" CItemImpl* CItem_initItemImplInstances(CItemData*)` (CItem.hpp), and remove the leftover CEquipItemBoxItemImplView view (lines ~663/694/2067). Some sites cast the result to CItemInstance* (510/512/2283) — check what retail really calls there.
   - ahx_dcd AHXDCD_DecodeHeader: objdiff 100->99.91 but hexdiff ✓ — reloc-name only; look at it with the CriWare naming pass.
   - COption / CSkipTimer `-ipa off`: re-verify like CCollepedia (that one was compensating for dummy stubs).
   - (wave i, i16) CException: deleting dtor now compiler-emitted from real virtual ~CException, but class marked `__declspec(novtable)` and lbl_eu_8056CCE0 is still a hand vtable referencing the dtor via an `extern "C" __dt__10CExceptionFv` decl. Real fix: compiler-emitted __vt__10CException (find why retail dtor has no vptr restore — maybe the vtable belongs to a different class layout).
   - (wave i, i16) CActParamAnimGame field530 volatile removed via inline predicate isFiringAllowed() (100%). Remaining: flags0C/clearFlag80000 volatile sites.
   - (wave i, i13) CfSoundMan: nw4r SoundStartable::detail_StartSound made public so CfSoundMan_StartSlotSound can call it and compare StartResult (retail shape). Verify against the real NW4R header whether detail_StartSound is public; if not, find the public API form that compiles the same. `__dt__801C18F4` is a hand-written array dtor in an extern "C" block — real shape is a global CfSoundSlot[64] of a class with ctor/dtor so MWCC emits __sinit/__construct_array/the array dtor.
   - (wave i, i5) CtrlAct: pure virtual slots at +0x0C/+0x8C named after the CtrlPc overrides (CtrlPc_UpdatePadState, dispatchCtrlPcActions); confirm the base's real names once CtrlPc is a real class.
   - TagProcCompareIds: two conflicting decls (CTagProcessor.hpp u32(const u32*,const u32*) vs CKizunagram.hpp extern "C" int(const wchar_t**,const wchar_t**)). Retail loads *b first, subf b-a. Unify on one real signature (callers look like iterator cur/end compare) — r2 Opus could not match.
   - o5: kyoshin/main 9/9 fully byte-identical (already Matching). code_800C17DC 11/12, CChainCombo split/extab fixed.
The real fix is function-local `static const ResNameData` inside each Get*/Has* function, plus removing the Keep function. RESOLVED j7: function-local statics duplicate per use and perturb `-ipa file`; the byte-order-preserving fix is descriptive file-scope data objects in resfile plus a local `static u8` table in restev. Both data diffs MATCH and resfile is now Matching.
   - CSysWin: 21/21 code, but .rodata short — CSysWin2 strings (menu/jp/SysWin2.arc etc.) have no owner in source; CSysWinBuff Init/Move stubs. TU ownership decision needed.
   - CArcItem (o6): faithful 40->1 but data now MISMATCH (RTTI base list for IWorkEvent; retail {name,0}). Kept faithful version; unit NonMatching.
   - CMenu* data diff: local @NNN RTTI companions of weak IUICf/CTTask<IUICf> not excluded by linked-view -> tooling gap affecting all CMenu*.
   - sfd_buf (r4): SFBUF_Set/GetUoch param now SfbufUoch* but sfd_uo caller extern still u32* — unify declaration in a header.
   - snd_SoundArchivePlayer still declares ParseAllocTrack / SeqSound::SetSeqUserprocCallback via hand-mangled extern "C" (now real members).
   - ipcProfile: 4/4 but .bss IpcFdArray order differs (already Matching).
   - d3: CDevice -RTTI off flag (retail slice has no RTTI records) — verify via faithful rtti_off category at checkpoint.
   - OSReset/CCol6Invite (o4): need real string literals / real MI classes — structural rewrite candidates.
   - SchedNode_AllocPool: code_804C8718.cpp:312 declares it as C++ u32(u32,u32); real is extern "C" void*(u32,s32) — unify.
   - snd_SoundArchive 24/24 but split FAIL: decomp .text 0x8F8 vs budget 0x358 — split cut likely wrong (retail inlines OpenExtStreamImpl). Investigate splits.txt for snd_SoundArchive.
   - CHECK at c9: CMenuMapSelectSC MapSelectSCHandleInput was same-size (0x670, 38 mm) at census, o6 reports 90.8% OVER(24). Suspects: r2 CFloorMap.hpp field_208/333C u8->bool, or i23 CTitleAHelp conversion. Bisect when build healthy.
   - CChainActorEne (o5): added static inline helpers (special-type check, list scan, actor lookup) — verify they correspond to real retail inlines / existing class members.
   - code_800B06A4 (i9): ObjFlagsEx/SortBuffer/ManagerD08/Vec724Block at src tail are padded view structs (u8 _XX[0x..] gaps) evading view_struct detector -> replace with real object classes; detector gap: flag pad-array overlay structs. Also factoryIter* extern "C" raw-offset helpers. 9 non-retail catalog helpers unwritten (testObj6Cmask etc.) block weak_dedup dead-strip.
   - snd (o8/j4): DONE — Bank::NoteOn 100% (real Channel mAlternateAssign/mReleaseIgnore/mpDisposeCallback/mpWaveInfo + inline setters); AxVoice::Format -> SampleFormat and extern "C" GetSampleByDspAddress removed; AllocChannel 100% via real zero-initialising MoveValue() ctor. Still open: TPrioList retail 0xF0 vs header 0xEC (no non-overlapping node exists).
   - code_800F42AC: must export C names ScMain_*/CfCode_* and own lbl_eu_80666EA8 before Matching.
   - PRE-EXISTING artifacts (hacklint): CException.cpp hand-written primary vtable + __RTTI__ blob + extern __dt__10CExceptionFv -> needs real virtual class layout; code_800B06A4 extern "C" __dt__800B183C raw reslist dtor -> real reslist<T> dtor; CEquipItemBox.hpp mVtable member; CSaveLoad award.mVtbl assignment; CtrlAct.hpp vf01/vf02 pad virtuals.
   - i24 residuals: frozen UNIT_RULES exact_renames map __vt__ back to lbl_eu_* for CArtsSet 8052F5D8, CProc/CViewRoot 8056B298 -> update rules + rename together (snd_BasicSound 8056A710 DONE: symbols + data-owning blob renamed, exact_renames field retired); CBattleManager base vtables 8052BD68/8052BD44 need reslist dtors to reference __vt__ names; CFade .data 0x104 vs 0x108.
   - CTitleAHelp: teardown/updateHelp/drawHelp/beginClose/... are C++ members in CTitleAHelp.cpp but retail exports bare C names -> callers keep extern "C" decls; real fix: decide true linkage (likely C-linkage free functions or retail names in symbols.txt), make one consistent. CPartyStateWin _pad18 cast for embedded CTitleAHelp.
   - CfNandManager (i21): __dt__8023E63C -> declare real virtual dtor + rename_syms the address symbol to the real mangled dtor (all regions); create__Q22cf13CfNandManagerFv retail mangling has NO params -> write static create() with no params (caller "args" likely leftover regs); stubs NandPackEntry/NandTileDescr/NandSaveBuild still empty.
   - c9 non-✓ % drops from cleanup (win back): CSkipTimer_UpdateSlotFrame 94.9->77.1, mpvcmc_InitMcOiTa 87.4->75.1, BasicSound::Update 85.2->80.8 (snd header changes — STILL OPEN: missing SoundActor+0x48/4C/50 refresh; see MWCC_CASES j4), rebuildEIBKind3Page 86.3->84.6; objdiff-only: ItemBoxGetTab 0% in report but ✓ in hexdiff.

0a. **Checkpoint 8 debts (2026-11-19).**
   - CSkipTimer (wave h, h12): BeginLoad/OnFileEvent use a hand-expanded member-wise copy from a block-scoped temporary instead of the probable original `mSysWin = CSysWin(0);` / `mSkipTimer2 = CSkipTimer2(mLayout);` (implicit operator= is not inlined under the unit's real -ipa off). Accepted because it replaced 26 noinline + 19 pragmas; revisit.
   - CTaskGame (h49): volatile re-reads moved to `volatile` field declarations on UnkClass_8004041C (counter no longer
     sees them). Plausible if the stream state is written from a callback thread; unverified. Removing volatility
     costs CTaskGame_updateStream 100->97.8%.
   - CfCamEvent_1 ctor (us-80075c34) 37.6%: retail inline word copies of ml::CVec3::zero; h46's u32[3] pun reverted
     by orchestrator. h54 researching a faithful trivially-copyable CVec3.
   - Next-wave leads (h42): CActorParam.hpp virtual return types (UnkVirtualFunc17/32/44/45/79/80/93/97/99/104/128/139/151/162/169/172 declared void, bodies return values); CBattleState.hpp readStatusSlot/getEventSlot/fetchStatusSlot real types; CfObjectMove real member at +0x6EC and MI subobject offsets.
   - Next-wave leads (h57): CSaveLoad 5 mangled_c (CfAward dtor address for __register_global_object; processAward Fv with hidden int).
   - CTaskGameCf::create (h50) calls ctor by mangled ABI name to avoid placement-new null guard — counted; find the real allocation shape.
   - String-pool blobs renamed (sMakeCrystalStrings 0x8050888C, CPresentStringPool, UIBattleArcPathStrings, sMenuEnemyNamePool, sBdatColumnNames, adx*/ahx* message tables): references are still `blob + offset`; label count dropped (CMCCrystalBox 251->64) but the original shape is per-use string literals emitted by the compiler. Real fix: replace blob+offset with literals when the TU's pool order/size can be reproduced. Faithful counter should probably count `<named blob> + 0x..` as offset_cast — tooling follow-up.
   - CMenuSymbolMark (h55): hand-built vtables renamed to invented `sSymbolMarkItemVtable`/`sSymbolMarkItemVtableB` (0x8052CDB4/0x8052CD50) — violates the vtable-rename rule added mid-wave; next wave: real classes with compiler-emitted `__vt__` (h63 recipe), else restore honest lbl names.
   - Field-level `volatile` added to dodge the per-use volatile counter: CTaskGame.hpp (UnkClass_8004041C unk4/unk14, h49), CActParamAnimGame.hpp field530 ("retail reloads it"), ahx_sjd `const volatile u32 ahxsjd_init_flags[]` (h38). Faithful counter should count volatile field/object declarations too; then re-justify each.
   - CtrlAct.hpp: cf::CtrlAct still declares placeholder virtuals vf00..vf19 (pre-existing at c7, now one per line). Real slot names needed from the retail vtable lbl_eu_8052B080.
   - COption: new `optionAnimFrame()` inline wrapper around extern lbl_eu_80668C10 (h16) — check whether retail really reads a foreign sdata2 constant or a literal.
   - CVec3 (h54, cancelled): removing the user-declared copy ctor makes MWCC emit retail's inline word copies; sweep was inconclusive (taken during churn). Header reverted to HEAD; CfCamEvent_1 now uses real ml::CVec3 fields (ctor 80.6%, honest). Redo the CVec3 blast-radius sweep on a quiet tree next wave.
   - RESOLVED: CCollepedia `-ipa off` was compensating for dummy stubs (func_80254144 `{return 0;}` folded by -ipa file). Real bodies restored from invented-name twins (CCollepedia_getFieldOffset/getField12/getFieldAtIdx; deleted non-retail CCollepedia_decrementWrap); flag dropped, 64/80. DEBT: type the +0xE8 page catalog (0x140-byte pages, 6x0x34 rows, 5x0xA cells) instead of u8* offset math.
   - CEquipItemBox (9 fns objdiff 100->99.8, hexdiff ✓ unchanged): CEquipItemBoxImports.hpp declares a bogus C++ overload `void* CItem_initItemImplInstances(void*)` so calls mangle to __FPv instead of retail's C symbol. Fix = delete that decl + side header, route all ~53 calls through canonical `extern "C" CItemImpl* CItem_initItemImplInstances(CItemData*)` (CItem.hpp), and remove the leftover CEquipItemBoxItemImplView view (lines ~663/694/2067). Some sites cast the result to CItemInstance* (510/512/2283) — check what retail really calls there.
   - ahx_dcd AHXDCD_DecodeHeader: objdiff 100->99.91 but hexdiff ✓ — reloc-name only; look at it with the CriWare naming pass.
   - COption / CSkipTimer `-ipa off`: re-verify like CCollepedia (that one was compensating for dummy stubs).

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
   - nw4r snd: the four BasicSound vtable reloc-name misses are fixed (real `__vt__` symbol, rule retired);
     BasicSound 31/35 ✓. AxManager ctor 100% and 20/26 ✓ after zero-initialising MoveValue();
     `AxManager::Init` still touches an unexplained 0x200 static.
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
