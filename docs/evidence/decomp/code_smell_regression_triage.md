# Code-smell regression triage (`smell_report.py --check` vs origin/main)

Read-only analysis; no source files, `targets.json`, or baselines were modified to produce this doc.
Counts reflect the live working tree at analysis time (re-run the command below for fresh numbers).
`--check` prints only the first 20 regressions; the full gate failure at analysis time was **103 entries / 623 lines** across 5 metrics.

## 1. Command used

The module truncates `--check` output to 20 rows, so the full list was produced with this snippet
(same scan, same baseline, no truncation):

```bash
cd /Users/ahmed/projects/xenoblade
.venv/bin/python3 - <<'PY'
import importlib.util, argparse
spec = importlib.util.spec_from_file_location('sr', 'tools/coop/smell_report.py')
m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
rows = m.scan_all()
args = argparse.Namespace(base=None)
base = m.extract_baseline(m.base_report_ref(args))
est = {k for mm in base.values() for k in mm}
regs = []
for path, metrics in sorted(base.items()):
    cur = rows.get(path)
    if cur is None: continue
    for k, disp in m.METRICS:
        if k not in est: continue
        b = int(metrics.get(k, 0)); a = getattr(cur, k)
        if a > b: regs.append((disp, path, b, a, a - b))
for r in sorted(regs, key=lambda x: -x[4]):
    print('\t'.join(str(x) for x in r))
print('TOTAL', len(regs), 'DELTA', sum(r[4] for r in regs))
PY
```

Per-TU acceptance (`PARTIAL a/t`, `COMPLETE`) comes from `python3 tools/coop/smell_report.py --completeness`; per-function accepted/unmatched splits from `tools/coop/ptr_sites.py` (`function_spans` + `accepted_keys` against `tools/coop/targets.json` FULL_MATCH/EQUIVALENT_MATCH). Line classes: `DECL` = file-scope `extern "C"` import declaration, `EXTBLK` = line inside an `extern "C" { }` block, `LBL` = `lbl_*` jump-table/data line, `B`/`C` = inside accepted/unmatched function body.

## 2. Totals

| metric | regression lines (delta) | TUs |
|---|---|---|
| extC-decl | 237 | 18 |
| extC-def | 117 | 34 |
| self | 168 | 32 |
| void* | 99 | 17 |
| ptr-arith | 2 | 2 |
| **total** | **623** | **103** |

| bucket | entries | lines | meaning |
|---|---|---|---|
| A progress-import | 25 | 331 | file-scope `extern "C"` decls / import-block lines / `lbl_*` tables naming retail symbols used by newly-decompiled bodies |
| B accepted-body | 46 | 184 | increase inside FULL_MATCH/EQUIVALENT_MATCH functions (cleanup candidate or accepted retail shape) |
| C unmatched-body | 32 | 108 | increase inside not-yet-accepted bodies (matching churn budget) |
| D test/pad/junk/noise | 0 | 0 | comments, pad symbols, detector noise (none found — asm-region and comment-content checks were clean) |

## 3. Per-entry table (sorted by delta desc)

| metric | TU | base -> cur (delta) | bucket | recommended action | evidence |
|---|---|---|---|---|---|
| extC-decl | `src/kyoshin/cf/CfScript.cpp` | 3 -> 74 (71) | A | re-baseline (import block for new script-VM bodies) | 70/74 lines inside one `extern "C" { }` import block (L604-688: CfRes_getLazyField50, CfRes_getLimit512b, isResFlag1, …); TU PARTIAL 39/40, i.e. the block names retail symbols used by the newly-decompiled CfScript bodies. |
| void* | `src/kyoshin/CCur.cpp` | 10 -> 47 (37) | A | re-baseline (cursor jump tables + new dtor bodies) | 37/47 are `extern void* lbl_eu_*` jump-table declarations for cursor UI dispatch; other 10 are `void*`-returning `__dt__*` destructor definitions (retail placement-delete shape) in unmatched bodies. TU PARTIAL 35/46. |
| extC-decl | `src/kyoshin/menu/CMenuBattleChain.cpp` | 7 -> 43 (36) | A | re-baseline (retail CProcess/CMenu symbol imports) | All 43 lines are file-scope `extern "C"` import declarations (Reset__14CChildNodeFv, Init/Term__16CMenuBattleChainFv, …); TU PARTIAL 18/23. |
| extC-decl | `src/kyoshin/CLoad.cpp` | 1 -> 36 (35) | A | re-baseline (new-body imports: AnimRewindFrame, CProcess/OnFileEvent helpers) | All 36 lines are file-scope import declarations; TU PARTIAL 12/15. |
| extC-decl | `src/kyoshin/cf/chain/CChainActorList.cpp` | 2 -> 35 (33) | A | re-baseline (chain retail imports + dtor decls) | All 35 lines are file-scope import declarations (`__dt__Q22cf*` dtor decls, Chain_EmitTallyEvents, ChainList_Clear, …); TU PARTIAL 29/40. |
| extC-def | `src/kyoshin/cf/CVision.cpp` | 8 -> 34 (26) | C | re-baseline (vision-system churn; rewrite while matching) | 26/34 defs are full function bodies in not-yet-accepted fns (initVisionSlots, driveVisionRingFx, openVisionPairEngage, checkSecondFusionVision, …); 8 in accepted fns (evalVisionTrigger, getNonZeroSlot1, pollVisionSlots, …). TU PARTIAL 13/38. |
| self | `src/kyoshin/action/CActParamAnim.cpp` | 298 -> 322 (24) | C | re-baseline (anim state-machine churn; opportunistic B cleanups noted below) | 205/322 counted lines in unmatched bodies (steerAnimHeading, blendAnimHeading, dispatchAnimSlot, fire*Gate fns …); 114 in accepted fns (setAnimSubPos, tickAnimFrame, attachAnimObj …). TU PARTIAL 92/144. See clean-now honourable mentions. |
| extC-decl | `src/kyoshin/cf/CfGameManager.cpp` | 117 -> 139 (22) | A | re-baseline (game-mgr retail import surface) | All 139 counted lines are file-scope import declarations (CtrlObjectParam_*, getEffectFlagFromMgr, setGimmickActive, …); TU PARTIAL 301/352. |
| self | `libs/monolib/src/scn/CScn.cpp` | 34 -> 51 (17) | B | clean now (accepted Scn_* helpers) — see Top 5 §1 | 42/51 in FULL_MATCH/EQUIVALENT_MATCH fns: Scn_IsAnimActiveOrNull, Scn_HasWorkItem, Scn_GetCamItem9C/CC, Scn_GetCamList, Scn_SetupAnim, Scn_CallUnk8C_V6/V7, Scn_CopyCamProjMatrix, …; TU PARTIAL 49/52. |
| void* | `src/kyoshin/cf/CfScript.cpp` | 2 -> 18 (16) | A | re-baseline (script-VM switch dispatch tables) | 16/18 are `void* lbl_eu_*` jump tables for the script VM; TU PARTIAL 39/40. |
| void* | `src/kyoshin/cf/chain/CChainActorList.cpp` | 0 -> 16 (16) | A | re-baseline (.sdata chain jump tables) | 12/16 are `.sdata` `lbl_eu_80662Axx` tables + 4 `__dt__Q22cf*` dtor import decls; TU PARTIAL 29/40. |
| self | `libs/monolib/src/scn/CScnItemModel.cpp` | 76 -> 89 (13) | B | clean now (accepted sim* helpers) — see Top 5 §2 | 69/89 in accepted fns: simRemoveFromPool, simPushWordToBuf, simGetLeafActData/AnimDist*, simNotifyVfunc9C, simSetFlag*Link, …; TU PARTIAL 63/69. |
| self | `src/kyoshin/CFade.cpp` | 3 -> 16 (13) | B | clean now (whole TU accepted) — see Top 5 §5 | TU COMPLETE 14/14. Lines in CFade_IsReady/IsVisible one-liners, CFade_MarkLoaded/StepFadeIn/StepFadeOut, CFade_StartLoad/Update/Draw/Unload/FadeIn/FadeOut, OnFileEvent. |
| self | `src/kyoshin/cf/CPartsChange.cpp` | 63 -> 76 (13) | B | clean now for accepted portion (41/76) — see Top 5 §3; re-baseline the 10-line import block | Accepted: CPartsChange_NotifySlotStates, InitChangeRecord, CheckReloadGate, FindEntryById/FreeSlot, UpdateElemSpeeds, …; 10 lines are `extern "C" { }` import-block decls (L79-145); rest in unmatched bodies. TU PARTIAL 54/84. |
| self | `src/kyoshin/code_800B06A4.cpp` | 187 -> 200 (13) | B | clean now for accepted portion (94/200) — see Top 5 §4 | Accepted: allocFactoryPool, ensureFactoryPool, reslistFirstItem/Count, syncPlayerSubMgr, teardownGameMgr, …; rest in unmatched bodies + 20 import decls. TU PARTIAL 114/253. |
| extC-def | `src/kyoshin/CFade.cpp` | 0 -> 12 (12) | B | re-baseline (COMPLETE TU; defs are the accepted retail shape) | TU COMPLETE 14/14. 12 forced-name defs (CFade_IsReady/IsVisible, MarkLoaded, StepFadeIn/Out, StartLoad, Update, Draw, Unload, FadeIn, FadeOut) — all inside accepted scope; `extern "C"` is the retail linkage here, no incidental cleanup. |
| extC-decl | `src/kyoshin/CCur.cpp` | 2 -> 13 (11) | A | re-baseline (cursor initLayout import block) | 11/13 lines inside `extern "C" { }` block (L578-590: initLayout__*Fv) + 2 pane-visibility imports; TU PARTIAL 35/46. |
| extC-def | `src/kyoshin/makecrystal/code_80213488.cpp` | 1 -> 10 (9) | B | re-baseline (retail C-linkage shape, 7/10 accepted) | Accepted MakeCrystal_ResetTable/CollectByTarget/GetFlag/AllFlagsSet/GetSubCategory/GetItemObject defs; 3 unmatched (dtor, SetFlag, func_802137B4). TU PARTIAL 9/11. |
| self | `src/kyoshin/cf/CfGameManager.cpp` | 23 -> 31 (8) | A | re-baseline (import decls carrying self params) | A-pool 21/31 are file-scope `extern "C"` import declarations/prototypes with `self`/`void* self` params (CtrlObjectParam_WriteEquipSlotValue, __ct__cf_CfTFile, KizunagramSetActiveSlot, …); TU PARTIAL 301/352. |
| extC-def | `src/kyoshin/cf/CfResObjImpl.cpp` | 0 -> 8 (8) | B | re-baseline (retail C-linkage shape, 7/8 accepted) | Accepted ResObj_IsInUse_C860/NotifySound_C888/GetWork_CCBC/DispatchState_CCE0/SetWork_CD68/ReleaseModels_C98C/PollActive_C950; 1 unmatched ctor. TU PARTIAL 13/14. |
| self | `src/kyoshin/cf/chain/CChain.cpp` | 240 -> 248 (8) | A | re-baseline (file-scope import decls + view helpers) | A-pool 117/248 are file-scope `extern "C"` import declarations (aiActionClearBits0006, CChain_DiscardVoiceId, …); 30 in accepted fns (CChain_AccumGaugeDelta, CChainActor_FwdRunKey, …), 101 unmatched. TU PARTIAL 41/54. |
| self | `src/kyoshin/cf/voice/CCharVoice.cpp` | 1 -> 9 (8) | B | clean now (all 8 in accepted fns) | 8/9 in accepted fns: attachOwner, updatePosition, stopVoiceA/B, playGated; 1 in unmatched __ct__CCharVoice. TU PARTIAL 6/8. |
| void* | `src/kyoshin/cf/CfGameManager.cpp` | 67 -> 74 (7) | A | re-baseline (jump-table / import data for new bodies) | Counted lines are lbl-star table data / extern void-star import decls (A-pool 42/74); TU PARTIAL (301/352). |
| self | `src/kyoshin/cf/CfRes.cpp` | 147 -> 154 (7) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 54/154 in not-yet-accepted fns (CfRes_extType…); TU PARTIAL (211/224). |
| self | `src/kyoshin/cf/object/CfObjectMove.cpp` | 159 -> 166 (7) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 119/166 in not-yet-accepted fns (CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv…); TU PARTIAL (110/127). |
| extC-decl | `src/kyoshin/cf/CTaskGameEffAfter.cpp` | 0 -> 6 (6) | B | re-baseline (COMPLETE TU; dtor/vfunc import decls) | TU COMPLETE 8/8. 6 `__dt__/Init__/Term__/Draw__17CTaskGameEffAfterFv` decls — accepted scope, no cleanup available. |
| extC-def | `src/kyoshin/cf/CfScript.cpp` | 15 -> 21 (6) | B | re-baseline (retail C-linkage mgr fns, already accepted) | 17/21 in accepted fns: CfScript_InitPath/InitMgr/UpdateMgr/ResetMgr, ClearReq*/FlagSlot*/LoadSlot* stubs; 4 in unmatched (LoadScript, OnFileEvent, ctors). `extern "C"` is the retail shape. |
| extC-decl | `src/kyoshin/menu/CMenuQuestLog.cpp` | 6 -> 12 (6) | A | re-baseline (retail symbol imports for new bodies) | All 12 counted lines are file-scope `extern "C"` import declarations; TU PARTIAL (16/23). |
| extC-def | `libs/monolib/src/lod/CTaskLOD.cpp` | 3 -> 8 (5) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 8/8 in not-yet-accepted fns (acquireLODResource__8CTaskLODFv, dispatchLODArgs__8CTaskLODFv…); TU PARTIAL (37/38). |
| extC-decl | `src/kyoshin/cf/CfGimmick.cpp` | 9 -> 14 (5) | A | re-baseline (retail symbol imports for new bodies) | All 14 counted lines are file-scope `extern "C"` import declarations; TU PARTIAL (43/51). |
| extC-def | `src/kyoshin/cf/CfGimmick.cpp` | 38 -> 42 (4) | B | re-baseline (retail C-linkage shape, 35/42 already accepted) | Accepted: PlaySoundAtPosScaled, StopManagedSound, UpdatePartyAnchorState, InitPartyGlobals, CheckStateFlag*, … (TU 43/51). Forced `extern "C"` names are the retail linkage — no incidental cleanup. |
| self | `src/kyoshin/cf/IResInfo.cpp` | 75 -> 79 (4) | B | clean now for accepted portion (60/81) | Accepted: CfRes_noopEntry, releaseVoiceSlot, getField18IfType*, findEntry*, … (u8*/ResInfoEntry* self params). 18 in unmatched (updateEntry/Grids, findKypEntry*). TU PARTIAL 68/76. |
| void* | `src/kyoshin/menu/CMenuQuestLog.cpp` | 0 -> 4 (4) | A | re-baseline (jump-table / import data for new bodies) | Counted lines are lbl-star table data / extern void-star import decls (A-pool 3/4); TU PARTIAL (16/23). |
| extC-def | `src/kyoshin/realtimeevt/CREvtLight.cpp` | 0 -> 4 (4) | C | re-baseline (light-system churn; half accepted) | 2/4 defs in accepted fns (CREvtLight_SetResource, AngleState), 2 unmatched (__ct__CREvtLight, DistToTarget). TU PARTIAL 6/7. |
| self | `libs/monolib/src/coli/code_804A6C60.cpp` | 226 -> 229 (3) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 88/229 in not-yet-accepted fns (ColiInitAngleSpec, func_804A7834…); TU PARTIAL (55/108). |
| extC-def | `libs/monolib/src/effect/code_804C8684.cpp` | 1 -> 4 (3) | B | re-baseline (retail C-linkage shape in accepted fns) | B-pool 4/4 in FULL_MATCH/EQUIVALENT_MATCH fns; forced `extern "C"` names are the retail linkage — no incidental cleanup. TU PARTIAL (4/5). |
| extC-def | `libs/monolib/src/scn/CMdlAnmEye.cpp` | 2 -> 5 (3) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 3/5 in not-yet-accepted fns (EyeAnm_RegisterMats, __ct__CMdlAnmEye…); TU PARTIAL (4/5). |
| void* | `libs/monolib/src/scn/CScn.cpp` | 23 -> 26 (3) | B | clean now (18/26 in accepted fns) | Accepted: Scn_DtorThunk54, Draw, Scn_SetupAnim, Scn_ReleaseUnk80, Scn_GetCamList/WorkInt, …; TU PARTIAL 49/52. |
| self | `libs/monolib/src/scn/CScn_80496B0C.cpp` | 39 -> 42 (3) | B | clean now for accepted portion (18/42) | Accepted: scn80496CallChild1V5, GetChild1Float, RefreshBlendTag, ReleaseChildren, …; 7 in import block, 17 unmatched. TU PARTIAL 18/29. |
| self | `src/kyoshin/CLoad.cpp` | 15 -> 18 (3) | B | clean now for accepted portion (7/18) | Accepted: CLoadBeginFileRequest, CLoadDrawIfVisible, CLoadTeardownLayout, CLoadAdvanceFadeHold, …; rest are import decls + file-scope one-liner helpers. TU PARTIAL 12/15. |
| void* | `src/kyoshin/cf/CBattleManager.cpp` | 473 -> 476 (3) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 396/476 in not-yet-accepted fns (~_reslist_base…); TU PARTIAL (47/78). |
| extC-def | `src/kyoshin/cf/object/CfObjectObj.cpp` | 0 -> 3 (3) | B | re-baseline (retail C-linkage shape in accepted fns) | B-pool 2/3 in FULL_MATCH/EQUIVALENT_MATCH fns; forced `extern "C"` names are the retail linkage — no incidental cleanup. TU PARTIAL (7/8). |
| extC-def | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_BEGIN.cpp` | 1 -> 4 (3) | B | re-baseline (retail C-linkage shape in accepted fns) | B-pool 3/4 in FULL_MATCH/EQUIVALENT_MATCH fns; forced `extern "C"` names are the retail linkage — no incidental cleanup. TU PARTIAL (7/9). |
| extC-def | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_CHAIN.cpp` | 1 -> 4 (3) | B | re-baseline (retail C-linkage shape in accepted fns) | B-pool 2/4 in FULL_MATCH/EQUIVALENT_MATCH fns; forced `extern "C"` names are the retail linkage — no incidental cleanup. TU PARTIAL (6/7). |
| extC-def | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE.cpp` | 1 -> 4 (3) | B | re-baseline (retail C-linkage shape in accepted fns) | B-pool 2/4 in FULL_MATCH/EQUIVALENT_MATCH fns; forced `extern "C"` names are the retail linkage — no incidental cleanup. TU COMPLETE. |
| self | `src/kyoshin/menu/CMenuBattleChain.cpp` | 28 -> 31 (3) | B | clean now for accepted portion (18/31) | Accepted: switchChainType, applyChainColorScheme, tickChainCounter, reinitChainPlayer, … (TU 18/23); 8 import decls re-baseline; 5 in unmatched dtor. |
| void* | `src/kyoshin/menu/CMenuBattleChain.cpp` | 4 -> 7 (3) | A | re-baseline (jump-table / import data for new bodies) | Counted lines are lbl-star table data / extern void-star import decls (A-pool 6/7); TU PARTIAL (18/23). |
| extC-decl | `libs/monolib/src/lib/CLibCriMoviePlay.cpp` | 50 -> 52 (2) | A | re-baseline (retail symbol imports for new bodies) | All 52 counted lines are file-scope `extern "C"` import declarations; TU PARTIAL (8/22). |
| self | `libs/monolib/src/lib/CLibCriMoviePlay.cpp` | 6 -> 8 (2) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 5/8 in not-yet-accepted fns (viBeginFrame…); TU PARTIAL (8/22). |
| extC-def | `libs/monolib/src/nand/CNReqtaskSave.cpp` | 6 -> 8 (2) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 6/8 in not-yet-accepted fns (FSCOPE-DEF extern "C" __declspec(noinline) void func_804DAF60(CNReqtaskSaveVtbl** dest) {, FSCOPE-DEF extern "C" __declspec(noinline) void sinit_804DAF58() {…); TU PARTIAL (30/33). |
| extC-def | `libs/monolib/src/scn/CScnFogMan.cpp` | 5 -> 7 (2) | B | re-baseline (retail C-linkage shape in accepted fns) | B-pool 4/7 in FULL_MATCH/EQUIVALENT_MATCH fns; forced `extern "C"` names are the retail linkage — no incidental cleanup. TU PARTIAL (7/8). |
| void* | `src/kyoshin/CLoad.cpp` | 0 -> 2 (2) | A | re-baseline (jump-table / import data for new bodies) | Counted lines are lbl-star table data / extern void-star import decls (A-pool 2/2); TU PARTIAL (12/15). |
| self | `src/kyoshin/CSaveLoad.cpp` | 66 -> 68 (2) | B | clean now for accepted portion (38/68) | Accepted: CSLCur_bindSlots/reset/tickAnim, CSaveLoad_bestSlot/cancel/confirm/curDown/curUp/draw, …; TU PARTIAL 73/80. |
| self | `src/kyoshin/CUICfManager.cpp` | 12 -> 14 (2) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 8/14 in not-yet-accepted fns (__ct__CUICfManager, eventDispatchThunk__12CUICfManagerFv…); TU PARTIAL (66/89). |
| extC-decl | `src/kyoshin/cf/CVision.cpp` | 22 -> 24 (2) | A | re-baseline (retail symbol imports for new bodies) | All 24 counted lines are file-scope `extern "C"` import declarations; TU PARTIAL (13/38). |
| self | `src/kyoshin/cf/CfGimmick.cpp` | 27 -> 29 (2) | B | clean now for accepted portion (15/29) | Accepted: StopManagedSound, ClearManagerBinding, LoadBdatAreaPos/Extents*, CheckPartyIdLoaded, …; TU PARTIAL 43/51. |
| extC-def | `src/kyoshin/cf/chain/CChainActorList.cpp` | 27 -> 29 (2) | B | re-baseline (retail C-linkage chain fns, mostly accepted) | 20/29 in accepted fns (ChainList_FindKey/HasCount/StepTargets/SweepDead, Chain_SweepDeadActors, ChainMusic_RestoreSlots …); 9 in unmatched. Forced names are the retail shape. |
| extC-decl | `src/kyoshin/cf/object/CfObjectEne.cpp` | 10 -> 12 (2) | A | re-baseline (retail symbol imports for new bodies) | All 12 counted lines are file-scope `extern "C"` import declarations; TU PARTIAL (127/137). |
| extC-def | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_PARTY_GAGE.cpp` | 0 -> 2 (2) | B | re-baseline (retail C-linkage shape in accepted fns) | B-pool 1/2 in FULL_MATCH/EQUIVALENT_MATCH fns; forced `extern "C"` names are the retail linkage — no incidental cleanup. TU COMPLETE. |
| extC-def | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_BREAK.cpp` | 0 -> 2 (2) | B | re-baseline (retail C-linkage shape in accepted fns) | B-pool 1/2 in FULL_MATCH/EQUIVALENT_MATCH fns; forced `extern "C"` names are the retail linkage — no incidental cleanup. TU PARTIAL (4/6). |
| self | `src/kyoshin/makecrystal/CMCCrystalBox.cpp` | 225 -> 227 (2) | B | clean now for accepted portion (94/227) | Accepted: refreshCrystalBoxName, confirmCrystalSelect/Unselect, drawCrystalBox, initCrystalBoxResources, …; rest unmatched + file-scope prototypes. TU PARTIAL 72/89. |
| extC-def | `libs/monolib/src/device/CDeviceSC.cpp` | 0 -> 1 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 1/1 in not-yet-accepted fns (func_eu_8044A600…); TU PARTIAL (8/10). |
| extC-def | `libs/monolib/src/lib/CLibCriMoviePlay.cpp` | 6 -> 7 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 5/7 in not-yet-accepted fns (__ct__CLibCriMoviePlay, setupMovieGfx__16CLibCriMoviePlayFv…); TU PARTIAL (8/22). |
| ptr-arith | `libs/monolib/src/lib/CLibCriMoviePlay.cpp` | 2 -> 3 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 3/3 in not-yet-accepted fns (__ct__CLibCriMoviePlay…); TU PARTIAL (8/22). |
| extC-decl | `libs/monolib/src/lib/CLibCriStreamingPlay.cpp` | 65 -> 66 (1) | A | re-baseline (retail symbol imports for new bodies) | All 66 counted lines are file-scope `extern "C"` import declarations; TU PARTIAL (10/22). |
| extC-def | `libs/monolib/src/lib/CLibCriStreamingPlay.cpp` | 12 -> 13 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 13/13 in not-yet-accepted fns (__ct__CLibCriStreamingPlay, __dt__20CLibCriStreamingPlayFv…); TU PARTIAL (10/22). |
| self | `libs/monolib/src/lib/CLibCriStreamingPlay.cpp` | 6 -> 7 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 5/7 in not-yet-accepted fns (__dt__20CLibCriStreamingPlayFv…); TU PARTIAL (10/22). |
| ptr-arith | `libs/monolib/src/lod/LODMemMan.cpp` | 45 -> 46 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 39/46 in not-yet-accepted fns (rebuildFromSharedBuffer__Q23LOD9LODMemManFv…); TU PARTIAL (61/80). |
| extC-def | `libs/monolib/src/scn/CScnBlend.cpp` | 4 -> 5 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 3/5 in not-yet-accepted fns (__ct__CScnBlend, __dt__9CScnBlendFv…); TU PARTIAL (4/5). |
| extC-def | `libs/monolib/src/scn/CScnFilter.cpp` | 1 -> 2 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 2/2 in not-yet-accepted fns (FilterUpdateIntensity, __ct__CScnFilter…); TU PARTIAL (2/3). |
| void* | `libs/monolib/src/scn/CScnItemModelNw4r.cpp` | 29 -> 30 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 16/30 in not-yet-accepted fns (ResUserDataNumData, scnImN4UpdateVis…); TU PARTIAL (68/84). |
| extC-decl | `libs/nw4r/src/snd/snd_SeqFile.cpp` | 0 -> 1 (1) | A | re-baseline (retail symbol imports for new bodies) | All 1 counted lines are file-scope `extern "C"` import declarations; TU PARTIAL (2/3). |
| extC-decl | `src/kyoshin/CFade.cpp` | 30 -> 31 (1) | B | re-baseline (retail symbol imports for new bodies) | All 31 counted lines are file-scope `extern "C"` import declarations; TU COMPLETE. |
| void* | `src/kyoshin/CFade.cpp` | 1 -> 2 (1) | B | re-baseline (jump-table / import data for new bodies) | Counted lines are lbl-star table data / extern void-star import decls (A-pool 2/2); TU COMPLETE. |
| self | `src/kyoshin/CFloorMap.cpp` | 93 -> 94 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 76/94 in not-yet-accepted fns (FSCOPE-DEF u8 FloorMap_GetField40Flag(void* self) { return static_cast<CFloorMapFull*>(self)->field_, FloorMap_CursorDown…); TU PARTIAL (18/75). |
| extC-def | `src/kyoshin/CLoad.cpp` | 0 -> 1 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 1/1 in not-yet-accepted fns (__ct__CLoad…); TU PARTIAL (12/15). |
| void* | `src/kyoshin/CSaveLoad.cpp` | 33 -> 34 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 16/34 in not-yet-accepted fns (FSCOPE-DEF struct Layout10View { u8 _00[0x10]; void* root; };, __dt__8028E7F8…); TU PARTIAL (73/80). |
| extC-decl | `src/kyoshin/CSystemWindow.cpp` | 1 -> 2 (1) | A | re-baseline (retail symbol imports for new bodies) | All 2 counted lines are file-scope `extern "C"` import declarations; TU PARTIAL (7/8). |
| void* | `src/kyoshin/CUICfManager.cpp` | 48 -> 49 (1) | B | clean now for accepted portion (21/49) | Accepted: CUICfManager_createInstance, initEventTriplet, queuePauseItemMenu, Move/Term bodies, …; 16 in import block re-baseline. TU PARTIAL 66/89. |
| void* | `src/kyoshin/cf/CArtsSet.cpp` | 0 -> 1 (1) | B | re-baseline (COMPLETE TU; required jump table) | TU COMPLETE 15/15. Single `void* lbl_eu_80573D18[0x1C]` dispatch table — required retail data, not cleanable. |
| extC-decl | `src/kyoshin/cf/CPartsChange.cpp` | 44 -> 45 (1) | A | re-baseline (retail symbol imports for new bodies) | All 45 counted lines are file-scope `extern "C"` import declarations; TU PARTIAL (54/84). |
| self | `src/kyoshin/cf/CSuddenCommu.cpp` | 50 -> 51 (1) | B | clean now for accepted portion (34/51) | Accepted: SuddenCommuResetPairingState, DriveFrame, DispatchFlags/Partner, GatePhase*, …; 12 unmatched (DispatchAttacker, EnterBattle, TickActive). TU PARTIAL 24/31. |
| void* | `src/kyoshin/cf/CTaskGameEffAfter.cpp` | 0 -> 1 (1) | B | re-baseline (COMPLETE TU; dtor decl) | TU COMPLETE 8/8. Single `void* __dt__17CTaskGameEffAfterFv(void*, int)` import decl — accepted scope. |
| self | `src/kyoshin/cf/CfResObjImpl.cpp` | 9 -> 10 (1) | B | clean now (9/10 in accepted fns) | Accepted: ResObj_IsInUse_C860, NotifySound_C888, PollActive_C950, ReleaseModels_C98C, GetWork_CCBC, SetWork_CD68, DispatchState_CCE0; 1 in unmatched ctor. TU PARTIAL 13/14. |
| self | `src/kyoshin/cf/CtrlEnemy.cpp` | 19 -> 20 (1) | B | clean now for accepted portion (9/20) | Accepted: CtrlEnemy_UnkFunc_8008772C/80088620, …; rest unmatched ctor + prototypes. TU PARTIAL 33/41. |
| extC-def | `src/kyoshin/cf/CtrlMoveNpc.cpp` | 3 -> 4 (1) | B | re-baseline (retail C-linkage shape in accepted fns) | B-pool 2/4 in FULL_MATCH/EQUIVALENT_MATCH fns; forced `extern "C"` names are the retail linkage — no incidental cleanup. TU PARTIAL (2/7). |
| extC-def | `src/kyoshin/cf/object/CfObjectEne.cpp` | 123 -> 124 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 122/124 in not-yet-accepted fns (__ct__cf_CfObjectEne, __dt__Q22cf13CfObjectActorFv…); TU PARTIAL (127/137). |
| void* | `src/kyoshin/cf/object/CfObjectModel.cpp` | 30 -> 31 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 20/31 in not-yet-accepted fns (CfObjectModel, CfObject_notifyEventDone__Q22cf13CfObjectModelFv…); TU PARTIAL (44/61). |
| extC-def | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END.cpp` | 0 -> 1 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 1/1 in not-yet-accepted fns (__ct__802A4870…); TU PARTIAL (5/6). |
| self | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END.cpp` | 11 -> 12 (1) | B | clean now for accepted portion (5/12) | Accepted: EndVoice_RemoveVoice/AdvanceSlot/AllocSize, func_802A497C; rest unmatched ctor + 2 import decls. TU PARTIAL 5/6. |
| extC-def | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END_SP.cpp` | 0 -> 1 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 1/1 in not-yet-accepted fns (__ct__802AB5F0…); TU PARTIAL (3/5). |
| self | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_CHAIN.cpp` | 13 -> 14 (1) | B | clean now for accepted portion (11/14) | Accepted: chainVoiceClearIdleSlot/RemoveByPtr/ActorTrigger/LifetimeHandler/PlayById; rest unmatched ctor + 1 import decl. TU PARTIAL 6/7. |
| extC-def | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN.cpp` | 1 -> 2 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 2/2 in not-yet-accepted fns (__ct__802A5B88, downVoiceFixed240…); TU PARTIAL (5/6). |
| extC-def | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_FAINT.cpp` | 6 -> 7 (1) | B | re-baseline (retail C-linkage shape in accepted fns) | B-pool 5/7 in FULL_MATCH/EQUIVALENT_MATCH fns; forced `extern "C"` names are the retail linkage — no incidental cleanup. TU PARTIAL (5/7). |
| self | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_PARTY_GAGE.cpp` | 19 -> 20 (1) | B | clean now (accepted-body smells; hexdiff-verify each) | B-pool 17/20 in accepted fns (PartyGage_NotifyDone_8560…); TU COMPLETE. |
| self | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE.cpp` | 17 -> 18 (1) | B | clean now (accepted-body smells; hexdiff-verify each) | B-pool 15/18 in accepted fns (ReviveVxPlaySlot1…); TU COMPLETE. |
| extC-def | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_SUDDEN.cpp` | 0 -> 1 (1) | B | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 1/1 in not-yet-accepted fns (__ct__802A8C04…); TU COMPLETE. |
| self | `src/kyoshin/cf/voice/cvsys/CVS_THREAD_SUDDEN.cpp` | 11 -> 12 (1) | B | clean now (accepted-body smells; hexdiff-verify each) | B-pool 8/12 in accepted fns (SuddenVxClearIdleSlot…); TU COMPLETE. |
| extC-def | `src/kyoshin/menu/CMenuQuestLog.cpp` | 15 -> 16 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 9/16 in not-yet-accepted fns (__ct__UnkClass_8011C974, __dt__13CMenuQuestLogFv…); TU PARTIAL (16/23). |
| void* | `src/kyoshin/plugin/pluginBtl.cpp` | 38 -> 39 (1) | B | clean now for accepted portion (19/39) | Accepted battle-plugin bodies (attackEne, selectTgt, setTensionLv, breakVision, isEnd, …); 18 in import block re-baseline. TU PARTIAL 14/19. |
| extC-decl | `src/kyoshin/plugin/pluginCfs.cpp` | 81 -> 82 (1) | A | re-baseline (retail symbol imports for new bodies) | All 82 counted lines are file-scope `extern "C"` import declarations; TU PARTIAL (57/73). |
| extC-def | `src/kyoshin/realtimeevt/CREvtMem.cpp` | 0 -> 1 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 1/1 in not-yet-accepted fns (__ct__cf_CREvtMem…); TU PARTIAL (4/7). |
| self | `src/kyoshin/realtimeevt/CREvtMem.cpp` | 0 -> 1 (1) | C | re-baseline (unmatched-body churn; rewrite while matching) | C-pool 1/1 in not-yet-accepted fns (__ct__cf_CREvtMem…); TU PARTIAL (4/7). |

## 4. Top 5 clean-now candidates (largest bucket-B deltas with concrete cleanups)

All five are `self`-param smells inside already-accepted functions: naturalize the free `X* self` helpers to member functions (or typed reference params where the retail shape allows), then re-verify each touched function with `python3 tools/coop/hexdiff.py <unit> --symbol <sym>` (repo lock is held by hexdiff; do not run raw ninja).

1. **`self` `libs/monolib/src/scn/CScn.cpp` +17 (34->51, TU 49/52).** 42/51 lines in accepted fns: `Scn_IsAnimActiveOrNull`, `Scn_HasWorkItem`, `Scn_GetCamItem9C/CC`, `Scn_GetCamList`, `Scn_SetupAnim`, `Scn_CallUnk8C_V6/V7`, `Scn_CopyCamProjMatrix`, `Scn_DtorThunk54`, … — free functions over `u8* self` view casts; convert to members/typed params.
2. **`self` `libs/monolib/src/scn/CScnItemModel.cpp` +13 (76->89, TU 63/69).** 69/89 in accepted fns: `simRemoveFromPool`, `simPushWordToBuf`, `simGetLeafActData`, `simGetLeafAnimDist[234]`, `simGetLeafAnimStatus`, `simNotifyVfunc9C`, `simSetFlag*Link`, `simToggleWordBits`, …
3. **`self` `src/kyoshin/cf/CPartsChange.cpp` +13 (63->76, TU 54/84).** 41/76 in accepted fns: `CPartsChange_NotifySlotStates`, `InitChangeRecord`, `CheckReloadGate`, `FindEntryById`, `FindFreeSlot`, `UpdateElemSpeeds`, `FindPartsElem`, `TeardownContainer`, … (leave the 10-line L79-145 import block alone).
4. **`self` `src/kyoshin/code_800B06A4.cpp` +13 (187->200, TU 114/253).** 94/200 in accepted fns: `allocFactoryPool`, `ensureFactoryPool`, `reslistFirstItem/Count`, `syncPlayerSubMgr`, `teardownGameMgr`, `getObjField70`, …
5. **`self` `src/kyoshin/CFade.cpp` +13 (3->16, TU COMPLETE 14/14).** Whole TU accepted — safest cleanup: `CFade_IsReady`/`CFade_IsVisible` one-liners and `CFade_MarkLoaded`/`StepFadeIn`/`StepFadeOut` to member accessors/mutators; `StartLoad`/`Update`/`Draw`/`Unload`/`FadeIn`/`FadeOut` + `OnFileEvent` likewise.

Next-up B cleanups (smaller): `self` CCharVoice +8 (attachOwner, updatePosition, stopVoiceA/B, playGated — 8/9 accepted); `self` IResInfo +4 (60/81 accepted); `void*` CScn +3; `self` CLoad/CMenuBattleChain/CScn_80496B0C +3 each; `void*` CUICfManager/pluginBtl +1 each (accepted-body portions). `extC-def` increases inside accepted bodies (CfScript +6, CfGimmick +4, CChainActorList +2, CfResObjImpl +8, code_80213488 +9, …) are predominantly retail C-linkage shape — re-baseline rather than clean.

## 5. Re-baseline list (conscious re-baseline candidates)

### Bucket A — progress imports / jump tables (25 entries, 331 lines)

Re-baseline after confirming each block/table is referenced by live bodies (all were at analysis time). Future work: header-ize the cross-TU retail imports instead of per-TU `extern "C"` decls.

- `src/kyoshin/cf/CfScript.cpp` extC-decl +71 (3->74): re-baseline (import block for new script-VM bodies)
- `src/kyoshin/CCur.cpp` void* +37 (10->47): re-baseline (cursor jump tables + new dtor bodies)
- `src/kyoshin/menu/CMenuBattleChain.cpp` extC-decl +36 (7->43): re-baseline (retail CProcess/CMenu symbol imports)
- `src/kyoshin/CLoad.cpp` extC-decl +35 (1->36): re-baseline (new-body imports: AnimRewindFrame, CProcess/OnFileEvent helpers)
- `src/kyoshin/cf/chain/CChainActorList.cpp` extC-decl +33 (2->35): re-baseline (chain retail imports + dtor decls)
- `src/kyoshin/cf/CfGameManager.cpp` extC-decl +22 (117->139): re-baseline (game-mgr retail import surface)
- `src/kyoshin/cf/CfScript.cpp` void* +16 (2->18): re-baseline (script-VM switch dispatch tables)
- `src/kyoshin/cf/chain/CChainActorList.cpp` void* +16 (0->16): re-baseline (.sdata chain jump tables)
- `src/kyoshin/CCur.cpp` extC-decl +11 (2->13): re-baseline (cursor initLayout import block)
- `src/kyoshin/cf/CfGameManager.cpp` self +8 (23->31): re-baseline (import decls carrying self params)
- `src/kyoshin/cf/chain/CChain.cpp` self +8 (240->248): re-baseline (file-scope import decls + view helpers)
- `src/kyoshin/cf/CfGameManager.cpp` void* +7 (67->74): re-baseline (jump-table / import data for new bodies)
- `src/kyoshin/menu/CMenuQuestLog.cpp` extC-decl +6 (6->12): re-baseline (retail symbol imports for new bodies)
- `src/kyoshin/cf/CfGimmick.cpp` extC-decl +5 (9->14): re-baseline (retail symbol imports for new bodies)
- `src/kyoshin/menu/CMenuQuestLog.cpp` void* +4 (0->4): re-baseline (jump-table / import data for new bodies)
- `src/kyoshin/menu/CMenuBattleChain.cpp` void* +3 (4->7): re-baseline (jump-table / import data for new bodies)
- `libs/monolib/src/lib/CLibCriMoviePlay.cpp` extC-decl +2 (50->52): re-baseline (retail symbol imports for new bodies)
- `src/kyoshin/CLoad.cpp` void* +2 (0->2): re-baseline (jump-table / import data for new bodies)
- `src/kyoshin/cf/CVision.cpp` extC-decl +2 (22->24): re-baseline (retail symbol imports for new bodies)
- `src/kyoshin/cf/object/CfObjectEne.cpp` extC-decl +2 (10->12): re-baseline (retail symbol imports for new bodies)
- `libs/monolib/src/lib/CLibCriStreamingPlay.cpp` extC-decl +1 (65->66): re-baseline (retail symbol imports for new bodies)
- `libs/nw4r/src/snd/snd_SeqFile.cpp` extC-decl +1 (0->1): re-baseline (retail symbol imports for new bodies)
- `src/kyoshin/CSystemWindow.cpp` extC-decl +1 (1->2): re-baseline (retail symbol imports for new bodies)
- `src/kyoshin/cf/CPartsChange.cpp` extC-decl +1 (44->45): re-baseline (retail symbol imports for new bodies)
- `src/kyoshin/plugin/pluginCfs.cpp` extC-decl +1 (81->82): re-baseline (retail symbol imports for new bodies)

### Bucket C — unmatched-body churn (32 entries)

Re-baseline: these lines sit in bodies that will be rewritten while matching. Largest first:

- `src/kyoshin/cf/CVision.cpp` extC-def +26 (8->34): re-baseline (vision-system churn; rewrite while matching)
- `src/kyoshin/action/CActParamAnim.cpp` self +24 (298->322): re-baseline (anim state-machine churn; opportunistic B cleanups noted below)
- `src/kyoshin/cf/CfRes.cpp` self +7 (147->154): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/cf/object/CfObjectMove.cpp` self +7 (159->166): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/lod/CTaskLOD.cpp` extC-def +5 (3->8): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/realtimeevt/CREvtLight.cpp` extC-def +4 (0->4): re-baseline (light-system churn; half accepted)
- `libs/monolib/src/coli/code_804A6C60.cpp` self +3 (226->229): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/scn/CMdlAnmEye.cpp` extC-def +3 (2->5): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/cf/CBattleManager.cpp` void* +3 (473->476): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/lib/CLibCriMoviePlay.cpp` self +2 (6->8): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/nand/CNReqtaskSave.cpp` extC-def +2 (6->8): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/CUICfManager.cpp` self +2 (12->14): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/device/CDeviceSC.cpp` extC-def +1 (0->1): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/lib/CLibCriMoviePlay.cpp` extC-def +1 (6->7): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/lib/CLibCriMoviePlay.cpp` ptr-arith +1 (2->3): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/lib/CLibCriStreamingPlay.cpp` extC-def +1 (12->13): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/lib/CLibCriStreamingPlay.cpp` self +1 (6->7): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/lod/LODMemMan.cpp` ptr-arith +1 (45->46): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/scn/CScnBlend.cpp` extC-def +1 (4->5): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/scn/CScnFilter.cpp` extC-def +1 (1->2): re-baseline (unmatched-body churn; rewrite while matching)
- `libs/monolib/src/scn/CScnItemModelNw4r.cpp` void* +1 (29->30): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/CFloorMap.cpp` self +1 (93->94): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/CLoad.cpp` extC-def +1 (0->1): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/CSaveLoad.cpp` void* +1 (33->34): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/cf/object/CfObjectEne.cpp` extC-def +1 (123->124): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/cf/object/CfObjectModel.cpp` void* +1 (30->31): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END.cpp` extC-def +1 (0->1): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END_SP.cpp` extC-def +1 (0->1): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN.cpp` extC-def +1 (1->2): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/menu/CMenuQuestLog.cpp` extC-def +1 (15->16): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/realtimeevt/CREvtMem.cpp` extC-def +1 (0->1): re-baseline (unmatched-body churn; rewrite while matching)
- `src/kyoshin/realtimeevt/CREvtMem.cpp` self +1 (0->1): re-baseline (unmatched-body churn; rewrite while matching)

## 6. Follow-up after cleanups land

Do NOT run `--write` until the §4 cleanups have landed (or a conscious decision to re-baseline A/C as-is is recorded). Then, and only then, refresh the checked-in baseline so `--check` gates the new state:

```bash
cd /Users/ahmed/projects/xenoblade
.venv/bin/python3 tools/coop/smell_report.py --write   # rewrites docs/CODE_SMELLS.md baseline
.venv/bin/python3 tools/coop/smell_report.py --check   # must pass afterwards
```

Do not hand-edit `docs/CODE_SMELLS.md`, `tools/coop/targets.json`, or any file under `src/`/`libs/` for this triage.

## Appendix — method and heuristic limits

- Scan replication: the per-line classifiers from `tools/coop/smell_scan.py` were re-run per TU to get matching line numbers (the checked-in scanner only stores counts). Line classes: file-scope `extern "C"` declaration (`;`-terminated, no `{`), `extern "C" { }` block member, `lbl_*` table line, or body line attributed via `ptr_sites.function_spans` and looked up in `accepted_keys` (targets.json FULL_MATCH/EQUIVALENT_MATCH, matched on source path + last `::` name component).
- Known attribution noise (does not change bucket calls above): `function_spans` keys on column-0 definitions, so file headers/prototypes before the first definition, `__declspec`-prefixed definitions, and brace-on-next-line definitions can misattribute; trailing file-scope decls lump into the last span (e.g. CMenuBattleChain `sinit_802AAF24`); mangled `__ct__*`/`__dt__*` names never hit `accepted_keys` (empty pre-`__` key), so ctor/dtor lines in COMPLETE TUs (PARTY_GAGE, REVIVE, SUDDEN, CFade, CTaskGameEffAfter) were manually treated as accepted scope.
- D-bucket verification: every counted line was also tested against asm regions (`asm { }`, `ASM_VOLATILE`, `__asm`) and for metric-matches living only inside `/* */` comments — zero findings, hence D = 0. (`//` comments are already stripped by the scanner.)
- `extC-decl` in a PARTIAL TU is classified A by construction (file-scope import surface for decomp progress); in a COMPLETE TU it is B (accepted scope). `void*`/`self` lines that are themselves import declarations/prototypes count as A even when they sit between bodies.
- Tree-state caveat: the gate compares the live tree against the origin/main baseline, so counts drift as concurrent work lands (the first probe in this session saw 102 entries / 624 lines; the finalized table reflects 103 entries / 623 lines). Re-run the §1 snippet for current numbers before acting.

