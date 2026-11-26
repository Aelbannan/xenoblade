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

- (W4 reviewer) MPSDEC_DecHdMpeg1 (W4-17, dropped): retail's high-word-first pair loads need pair-typed/reordered MPS_SYSHDR members in mps_types.h, which was outside the worker's file set. Header task: retype those members with evidence, then retry.
- (W4 reviewer) ScheduleList::add (W4-20, landed): the tail is reslist::push_back copied inline. Check whether a plain `mRes.push_back(p)` also gives 100% (const T& might force `p` onto the stack). If it does, use the call.
- (W4 reviewer) UnkClass_80466348.cpp: `u8ToF_a638`/`s32ToF_a628` (lines 120-121) are invented trivial cast inlines. W4-18 replaced them in setAmbColorWithAlpha2 with direct casts and reached 100%. Sweep the other call sites (lines ~641-677) the same way and drop the helpers if every function stays ✓.
- (W4 reviewer) code_804A6C60 ColiInitFlatSegment (W4-24, held addr-extern): retail reaches SDA2 1.0f/0.0f at lbl_eu_8066AE3C/8066AE44 through sda21 relocs. Check .sdata2 ownership for this TU (split) so plain literals pool to those addresses.
- (W4 reviewer) code_800B06A4.hpp:1894 `__ct__cf_CfHikariItemManager` alias is unused after W4-07 lands; delete it, together with the matching ctx.c line on regen.

- (W5 reviewer) isMCGetItemBoxActive (CMCGetItemBox.hpp:131 / .cpp:494) returns `int` in retail, not `u8`: the 5-65 ds probe with `int` on the declaration, definition and parent reached 100% for CheckReady, the callee and the caller. Callee-signature prestep, then re-run 5-65 (CModelDispMakeCrystal).
- (W5 reviewer) CfObject slot +0x88 / CfObject_pushRefreshValue (5-72, BLOCKED): declaring it `void` byte-matches, but MWCC rejects the override while CfObject.hpp:45 / CfObjectMove.hpp:435 say `void*`, and the CfObjectMove body needs `return this`. Header design: check whether these are really the same vtable slot / the same virtual before retyping the chain.
- (W5 reviewer) CVS_THREAD_HP (5-17): retail's triple-copy PTMF idiom reaches 95.1% only through the data at lbl_eu_80539C04. Find that object's owner and real symbol (split/data), then retry.
- (W5 reviewer) Unit compiler/flags question: mpv_err (5-22: Wii/1.1 fixes the stw/cmpi order) and CfResReloadImpl (5-33: Wii/1.1 or -O3 give retail store order, GC/3.0a5.2 -O4,p always swaps). Check these units' compiler version/flags against their neighbours, together with the W4 MPVDEC_CheckVersion note.
- (W5 reviewer) MPV_SkipFrmSj (5-23, held steer-temp): the patch reached 100% with a `MpvHn* h = (MpvHn*)hn` copy. Retry with the parameter itself typed `MpvHn* hn` (as MPV_RequestStop does) plus the `p = sj` alias removal. Cheap hy4 task.
- (W5 reviewer) SchedRes_AllocEntries (code_804DEDA8, 5-34 held): the `u32 size` parameter is the buffer address (it is aligned to 32 and stored as the table base). Retype it to `void* buf` with evidence and try `buf = aligned; ... return (u8*)buf + count * 0x18`. Do not reuse it for the end pointer.
- (W5 reviewer) CPartyStateWin (5-18 landed): CopyPTSubBlob (0x801933A4, size 0x5E0) is probably the out-of-line compiler-generated operator= of the real 0x53C class. Once that class is identified, replace the forwarding inline operator= in CPartyStateWin.hpp with the real class.

## Tasks parked from earlier waves
- NW4HBM-LINKLIST-INIT, CAMMAN-VIEWFALLOFF, OPEN-CFTFILE-FMTSTR.
- CtrlPc: regain after the approved revert (faithful 17 -> 26).
- code_804BC9EC.hpp label 1 -> 2.

## Questions for the human (milestone)
- DECIDED 2026-11-24 (user): bdat column first-byte read idiom (retail `stw r3` then `lbz`/`lhz +0`, ~10 TUs) -> declare ONE canonical `BdatValue` union (u32/u16/u8 members) in the bdat header and read `.b`/`.h`; no pointer puns at call sites. Task: hub-header change (single owner) + includer sweep.
- DECIDED 2026-11-24 (user): nw4r g3d assert inlines -> retail Panic filename (`g3d_rescommon_ac.h`, `g3d_resnode_ac.h`) + line numbers are sufficient evidence; place the real assert inline at that line in the real nw4r header (bytes must still match; per-library assert flag) and replace the hand Panic calls. Task: NW4R-G3D-ASSERT-INLINES (single owner).
- RESOLVED (W2 follow-up batch): EvtSeqGetCounter100 is declared once in EvtSeqApi.hpp as `extern "C" int`, and W2-19 re-landed with it. Original note: W2-19 (CREvtEffect refreshChrAnim, 100% in-unit) was gate-rejected: "full build failed" because its `extern "C" int EvtSeqGetCounter100()` in CREvtEffect.hpp conflicts with the u32 declarations above. Canonicalise the declaration first (one header, one type, verified against retail callers), then re-land the patch (/tmp/xb/plans/W2-19.patch).
- (W3 planner) String-pool order debt: CItemBoxGrid (RefreshCategoryTabs__12CItemBoxGridFv: literals at retail +1312/+1327 vs decomp +496/+511).
- (W3 planner) refreshNpcRowFlags (CtrlMoveNpc): decomp-only relocs at +0x40/+0x44 where retail resolves inline — check symbol/split before re-queuing.
- (W3 planner) func_8009E120 (CtrlObjectParam) uses the DECOMP_PPC_RLWINM escape; suspected bdatResolveItem 2nd param is u16 (callee-signature; sent to W3 as ds).
- (W4 planner) code_800B06A4.hpp declares `extern "C" void* __ct__cf_CfHikariItemManager(void*)`, a hand alias of the real ctor `__ct__Q22cf19CfHikariItemManagerFv` (retail reloc at buildSingleton+0x140). Sent to W4-07; if not landed, replace it with `new (handle) cf::CfHikariItemManager` mechanically.
- (W4 planner) kyoshin/cf/CItem .text is 0x10 over its split budget (0x69D0 vs 0x69C0); find the long function.
- (W4 planner) MPVDEC_CheckVersion: in-source note documents a wall (switch gives signed cmpi, ifs merge the two `li -1` blocks) under GC/3.0a5.2; check the mpv_dec unit's compiler version/flags against the other CRI sofdec units.
- (W5 planner) CfObjectActor::CfObject_pushRefreshValue(float): retail has no `mr r3, r31` before the epilogue, so it returns void, not `void* this`. If the CfObjectMove base / vtable slot +0x5C4 declare `void*`, fix the signature across the header (sent to 5-72 as ds).
