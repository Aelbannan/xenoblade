# Orchestrator follow-ups (tracked; /tmp/xb is wiped on reboot)

Reviewers append here; the orchestrator never edits source for these.

## Mechanical / data
- 113 arity-drift candidates from `tools/coop/census_fixes.py sigs`: triage as `callee-signature` tasks.
- Forced-section padding (data-reconstruction debt): CPartsChange .sdata/.sdata2 tails, code_800C17DC gap_11, LODMemMan gap_09.
- gpCullingFrustum .sbss: US gives it to g3d_scnroot, JP to g3d_scnobj. Ambiguous; needs evidence before moving.
- Split UNIT_RULES keys by path (nw4r/nw4hbm share basename keys).
- Pre-existing debt under the new ban counters: scalar_ptr_alias 84, cast_addr_alias 77, addr_extern 25, single_use_inline 109.

- (W2 planner) String-pool order debt: CFloorMap (FloorMap_FindFloorRow +0x84: retail +346 vs decomp +28), CEquipItemBox (refreshEIBItemList 703 vs 233), CKizunagram (tickKizMarker 164 vs 72) — literal order/missing literals in those TUs.
- (W2 planner) MPV_Finish: retail `dcbi` but decomp uses `__dcbz` intrinsic — semantically wrong instruction; needs the original cache-op idiom decided (not a worker task).
- (W2 planner) uusb_ppc (BTE): check unit uses GC/3.0a5.2 + `-func_align 4` per KB (uusb_WriteCtrlDataCB scheduling).
- (W2 reviewer) Ctor mangling vs call-site width: `create__17CMenuTutorialList` passes `choice` with no clrlwi but the ctor symbol mangles `Us` (W2-11, 98.1%); `QstCnt_GetOrCreate` passes u16 without extsh but the ctor symbol `__ct__11CMenuQstCntFP4CScnssUcUcUc` mangles `ss` (W2-22; a u16 ctor gives 100% with unchanged ctor bytes). Decide whether these mangled names come from retail or were guessed; if guessed, fix the symbols.txt mangle (prestep) and retype the ctors.
- (W2 reviewer) Float-pool literals: CVirtualLightObj initLightEnv (W2-14) and CMenuArtsSelect checkArtUsable (W2-20) only reach retail's float relocs through `lbl_eu_8066AAA8`/`8066AA98`/`80666F44` externs. Literal pooling/.sdata2 ownership problem; check the splits instead of swapping literals for labels.
- (W2 reviewer) checkArtUsable 100% needs an inlined u8 1..24 battle-phase range predicate that CfObjectPc::CActorParam_UnkVirtualFunc88 shares. Header design: add a real shared inline (cf/kyoshin header, with name-guess) used by both TUs, then retry W2-20.
- (W2 reviewer) EvtSeqGetCounter100: the definition (CTaskREvtSequence.cpp) and CREvtModelMap.cpp/CPcEffect07 say u32; CREvtMovie.hpp, CTaskREvent, CGame and CREvtEffect.hpp (after W2-19) say int. Canonicalise to int in EvtSeqApi.hpp and remove the local re-declarations; the linkage is also mixed (C++ in EvtSeqApi.hpp, extern "C" in the realtimeevt headers).

- (W3 reviewer) ut_PackedFont StreamingConstruct (W3-08): its switch jumptable is `jumptable_eu_8056B050`, which the split gives to ut_ResFontBase.cpp; move it to ut_PackedFont's .rodata/.data range (split fix), then re-check the retry patch.
- (W3 reviewer) WPADHIDParser .bss 0x805DDB80..0x805DDC00 is owned by this TU (splits.txt) but the source only externs `_wpadCalibrationX[5][4]`; retail has five 0x10 objects (_wpadCalibrationX/Y, _wpadCenterX/Y, _wpadRolag) plus _wpadExtRawData. Sent to W3-14 retry-ds; if the retry fails, do it as a data-reconstruction task.
- (W3 reviewer) bdatResolveItem itemId width (W3-05 held): retail func_8009E120 narrows arg 2 (clrlwi) before the tail call. Try `u16 itemId` on the CfBdat.cpp definition (check its body still matches, plus the CItem/CMenuGameClear callers), then make CtrlObjectParam.hpp agree and drop the DECOMP_PPC_RLWINM escape. callee-signature task.
- (W3 reviewer) CViewFrame `getFrame2ViewRect__10CViewFrameFR7CRect16PC10CViewFrame` is defined as an extern "C" free function with 3 args under a pre-mangled name (the mangle uses a global ::CRect16, not ml::CRect16). Decide the real declaration: a member with this=r3? CRect16 in the global namespace? Then fix symbols.txt or the class.
- (W3r reviewer) scnLgtFillLightVec4 (W3-27, held twice): retail computes z, y, x and then stores x, y, z. That is the right-to-left argument order of an inlined constructor. Header design: give CScnEnvLgtCtrlLgtVec3f a real (x, y, z) constructor or operator*, or retype it to ml::CVec3 with evidence, then retry. Keep the `const CScnEnvLgtCtrlLgtParamEntry* ctl` part of the held patch: it is plausible and removes 2 structural diffs.

## Tasks parked from earlier waves
- NW4HBM-LINKLIST-INIT, CAMMAN-VIEWFALLOFF, OPEN-CFTFILE-FMTSTR.
- CtrlPc: regain after the approved revert (faithful 17 -> 26).
- code_804BC9EC.hpp label 1 -> 2.

## Questions for the human (milestone)
- bdat column first-byte read idiom: which spelling is the original?
- nw4r g3d assert-inline layout: what evidence is needed to restore the original layout?
- RESOLVED (W2 follow-up batch): EvtSeqGetCounter100 is declared once in EvtSeqApi.hpp as `extern "C" int`, and W2-19 re-landed with it. Original note: W2-19 (CREvtEffect refreshChrAnim, 100% in-unit) was gate-rejected: "full build failed" because its `extern "C" int EvtSeqGetCounter100()` in CREvtEffect.hpp conflicts with the u32 declarations above. Canonicalise the declaration first (one header, one type, verified against retail callers), then re-land the patch (/tmp/xb/plans/W2-19.patch).
- (W3 planner) String-pool order debt: CItemBoxGrid (RefreshCategoryTabs__12CItemBoxGridFv: literals at retail +1312/+1327 vs decomp +496/+511).
- (W3 planner) refreshNpcRowFlags (CtrlMoveNpc): decomp-only relocs at +0x40/+0x44 where retail resolves inline — check symbol/split before re-queuing.
- (W3 planner) func_8009E120 (CtrlObjectParam) uses the DECOMP_PPC_RLWINM escape; suspected bdatResolveItem 2nd param is u16 (callee-signature; sent to W3 as ds).
