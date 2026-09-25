# Demoted FULL_MATCH — remaining base:gone (2026-09-19)

After history restore of 106 definitions, demoted the remaining
`FULL_MATCH` rows where retail still has the symbol but decomp does not
(`base:not-found`), plus a few both-sides missing. History-restored IDs
were **excluded** (they need rebuild + re-sweep, not demotion).

## Summary

- Demoted: **326**
- `base:not-found` (retail ok): **311**
- both not-found: **15**
- Restored IDs excluded: **110**
- Registry `FULL_MATCH`: 14465 → **14139**

## By unit

| n | unit |
|--:|------|
| 80 | `kyoshin/code_800B06A4` |
| 37 | `kyoshin/COption` |
| 36 | `monolib/src/lod/UnkClass_80466348` |
| 14 | `kyoshin/cf/CfNandManager` |
| 13 | `kyoshin/cf/CtrlObjectParam` |
| 11 | `nw4r/src/g3d/res/g3d_resmat` |
| 9 | `kyoshin/code_80135FDC` |
| 8 | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` |
| 7 | `kyoshin/CSysWinSelect` |
| 6 | `kyoshin/plugin/pluginCam` |
| 6 | `kyoshin/cf/object/CfObjectModel` |
| 5 | `kyoshin/cf/CtrlEnemy` |
| 5 | `kyoshin/CScrollBar` |
| 5 | `kyoshin/help/CHelp` |
| 4 | `kyoshin/CGame` |
| 4 | `kyoshin/cf/CtrlPc` |
| 4 | `PowerPC_EABI_Support/src/Runtime/runtime` |
| 4 | `monolib/src/core/CDesktop` |
| 4 | `monolib/src/lod/code_8046A530` |
| 3 | `PowerPC_EABI_Support/src/Runtime/__mem` |
| 3 | `kyoshin/cf/CfBdat` |
| 3 | `RVL_SDK/src/revolution/os/OS` |
| 2 | `RVL_SDK/src/revolution/os/__ppc_eabi_init` |
| 2 | `kyoshin/CTaskGameEff` |
| 2 | `kyoshin/CCol6Invite` |
| 2 | `kyoshin/cf/CTaskREvtSequence` |
| 2 | `kyoshin/CSysWinSave` |
| 2 | `nw4r/src/snd/snd_BasicSound` |
| 2 | `monolib/src/work/CWorkRoot` |
| 2 | `monolib/src/device/CDeviceVI` |
| 2 | `monolib/src/device/CDeviceFileJob` |
| 2 | `monolib/src/lib/CLibCri` |
| 2 | `monolib/src/scn/CMdlMaterial` |
| 1 | `kyoshin/cf/CTaskEnvironment` |
| 1 | `kyoshin/cf/CfTFile` |
| 1 | `kyoshin/cf/CfCamEvent` |
| 1 | `kyoshin/cf/CfCam` |
| 1 | `kyoshin/CUICfManager` |
| 1 | `kyoshin/cf/CArtsParam` |
| 1 | `kyoshin/CCol6System` |
| 1 | `kyoshin/realtimeevt/CREvtMem` |
| 1 | `kyoshin/cf/CPartsChange` |
| 1 | `kyoshin/cf/CTaskCulling` |
| 1 | `kyoshin/CItemBoxGrid` |
| 1 | `kyoshin/cf/CfGimmickEne` |
| 1 | `kyoshin/CSysWinScenarioLog` |
| 1 | `kyoshin/cf/voice/cvsys/CVS_THREAD` |
| 1 | `kyoshin/cf/voice/cvsys/CVS_THREAD_CHAIN` |
| 1 | `kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN` |
| 1 | `kyoshin/cf/voice/cvsys/CVS_THREAD_FAINT` |
| 1 | `RVL_SDK/src/revolution/hbm/HBMBase` |
| 1 | `RVL_SDK/src/revolution/os/OSInterrupt` |
| 1 | `RVL_SDK/src/revolution/os/OSSync` |
| 1 | `nw4r/src/g3d/g3d_anmtexpat` |
| 1 | `nw4r/src/g3d/g3d_anmtexsrt` |
| 1 | `nw4r/src/lyt/lyt_material` |
| 1 | `nw4r/src/snd/snd_BasicPlayer` |
| 1 | `nw4r/src/snd/snd_MidiSeqPlayer` |
| 1 | `nw4r/src/snd/snd_SoundPlayer` |
| 1 | `nw4r/src/snd/snd_WaveFile` |
| 1 | `nw4r/src/ut/ut_IOStream` |
| 1 | `monolib/src/device/CDevice` |
| 1 | `monolib/src/device/CDeviceFontInfoExt` |
| 1 | `monolib/src/lib/UnkClass_80460308` |
| 1 | `monolib/src/effect/CERand` |
| 1 | `monolib/src/nand/CNBanner` |

## Demoted rows

| id | unit | symbol | kind | size |
|----|------|--------|------|-----:|
| `us-80004000` | `PowerPC_EABI_Support/src/Runtime/__mem` | `memcpy` | both-not-found | 0x29C |
| `us-8000429c` | `PowerPC_EABI_Support/src/Runtime/__mem` | `__fill_mem` | both-not-found | 0xB4 |
| `us-80004350` | `PowerPC_EABI_Support/src/Runtime/__mem` | `memset` | both-not-found | 0x30 |
| `us-802bcaf8` | `PowerPC_EABI_Support/src/Runtime/runtime` | `_savefpr_14` | both-not-found | 0x4C |
| `us-802bcb44` | `PowerPC_EABI_Support/src/Runtime/runtime` | `_restfpr_14` | both-not-found | 0x4C |
| `us-802bcb90` | `PowerPC_EABI_Support/src/Runtime/runtime` | `_savegpr_14` | both-not-found | 0x4C |
| `us-802bcbdc` | `PowerPC_EABI_Support/src/Runtime/runtime` | `_restgpr_14` | both-not-found | 0x4C |
| `us-802e1c20` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_search_cancel` | base:not-found | 0x84 |
| `us-802e1ca4` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_discover` | base:not-found | 0x114 |
| `us-802e2504` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_search_timer_cback` | base:not-found | 0x4C |
| `us-802e2720` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_find_services` | base:not-found | 0x1A0 |
| `us-802e28c0` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_discover_next_device` | base:not-found | 0xE0 |
| `us-802e2ac4` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_service_search_remname_cback` | base:not-found | 0x40 |
| `us-802e2b04` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_remname_cback` | base:not-found | 0x8C |
| `us-802e361c` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_compress_cback` | base:not-found | 0x238 |
| `us-80325c80` | `RVL_SDK/src/revolution/hbm/HBMBase` | `__dt__Q36nw4hbm3lyt19ArcResourceAccessorFv` | base:not-found | 0x6C |
| `us-80355790` | `RVL_SDK/src/revolution/os/OS` | `__OSDBINTSTART` | both-not-found | 0x24 |
| `us-803557c0` | `RVL_SDK/src/revolution/os/OS` | `__OSDBJUMPSTART` | both-not-found | 0x4 |
| `us-80355800` | `RVL_SDK/src/revolution/os/OS` | `__OSEVStart` | both-not-found | 0x9C |
| `us-8035b720` | `RVL_SDK/src/revolution/os/OSInterrupt` | `__RAS_OSDisableInterrupts_begin` | both-not-found | 0x14 |
| `us-8035e020` | `RVL_SDK/src/revolution/os/OSSync` | `__OSSystemCallVectorStart` | both-not-found | 0x20 |
| `us-800065c0` | `RVL_SDK/src/revolution/os/__ppc_eabi_init` | `__init_hardware` | both-not-found | 0x24 |
| `us-800065f0` | `RVL_SDK/src/revolution/os/__ppc_eabi_init` | `__flush_cache` | both-not-found | 0x34 |
| `us-80165270` | `kyoshin/CCol6Invite` | `OnFileEvent__9CCol6HintFP10CEventFile` | base:not-found | 0x8 |
| `us-80165290` | `kyoshin/CCol6Invite` | `OnFileEvent__11CCol6SystemFP10CEventFile` | base:not-found | 0x8 |
| `us-80164f38` | `kyoshin/CCol6System` | `Term__11CCol6InviteFv` | base:not-found | 0x10 |
| `us-800396c8` | `kyoshin/CGame` | `getGameState__5CGameFv` | base:not-found | 0x28 |
| `us-800396f0` | `kyoshin/CGame` | `initGameState__5CGameFv` | base:not-found | 0xD4 |
| `us-80039980` | `kyoshin/CGame` | `setGameActive__5CGameFb` | base:not-found | 0xB0 |
| `us-8003a1dc` | `kyoshin/CGame` | `wkStandbyExceptionRetry__11CWorkThreadFUl` | base:not-found | 0x8 |
| `us-801c6bc0` | `kyoshin/CItemBoxGrid` | `__ct__CVisionItem` | base:not-found | 0x18 |
| `us-8029e640` | `kyoshin/COption` | `COptionLoadLayout` | base:not-found | 0xD4 |
| `us-8029e714` | `kyoshin/COption` | `COptionShowPaneA` | base:not-found | 0x88 |
| `us-8029e79c` | `kyoshin/COption` | `COptionShowPaneB` | base:not-found | 0x88 |
| `us-8029e824` | `kyoshin/COption` | `COptionFinishIntroAnim` | base:not-found | 0xAC |
| `us-8029e8d0` | `kyoshin/COption` | `__ct__COption` | base:not-found | 0xD8 |
| `us-8029e9a8` | `kyoshin/COption` | `__dt__7COptionFv` | base:not-found | 0x94 |
| `us-8029ea3c` | `kyoshin/COption` | `COptionRebuildWidgets` | base:not-found | 0x198 |
| `us-8029ebd4` | `kyoshin/COption` | `COptionTickState` | base:not-found | 0xD4 |
| `us-8029eca8` | `kyoshin/COption` | `COptionDraw` | base:not-found | 0xA4 |
| `us-8029ed4c` | `kyoshin/COption` | `COptionTeardown` | base:not-found | 0xC8 |
| `us-8029ee14` | `kyoshin/COption` | `COptionIsWindowReady` | base:not-found | 0x5C |
| `us-8029ee70` | `kyoshin/COption` | `COptionGetLiveFlag` | base:not-found | 0x8 |
| `us-8029ee78` | `kyoshin/COption` | `COptionGetConfirmGate` | base:not-found | 0x8 |
| `us-8029ee80` | `kyoshin/COption` | `COptionGetSecondConfirm` | base:not-found | 0x8 |
| `us-8029ee88` | `kyoshin/COption` | `COptionCursorUp` | base:not-found | 0x11C |
| `us-8029efa4` | `kyoshin/COption` | `COptionCursorDown` | base:not-found | 0x124 |
| `us-8029f0c8` | `kyoshin/COption` | `COptionPageUp` | base:not-found | 0xD4 |
| `us-8029f19c` | `kyoshin/COption` | `COptionPageDown` | base:not-found | 0xE0 |
| `us-8029f27c` | `kyoshin/COption` | `COptionBeginScrollSetup` | base:not-found | 0x94 |
| `us-8029f310` | `kyoshin/COption` | `COptionConfirmSelection` | base:not-found | 0x6C |
| `us-8029f37c` | `kyoshin/COption` | `COptionHandleLeft` | base:not-found | 0x11C |
| `us-8029f498` | `kyoshin/COption` | `COptionHandleRightAccept` | base:not-found | 0x1D0 |
| `us-8029f668` | `kyoshin/COption` | `COptionOpenConfigDialog` | base:not-found | 0xD8 |
| `us-8029f7f8` | `kyoshin/COption` | `COptionAdvanceState2To3` | base:not-found | 0x6C |
| `us-8029f8fc` | `kyoshin/COption` | `COptionAdvanceState6To3` | base:not-found | 0x68 |
| `us-8029f964` | `kyoshin/COption` | `COptionTickConfigSync` | base:not-found | 0x158 |
| `us-8029fabc` | `kyoshin/COption` | `COptionAdvanceState8To9` | base:not-found | 0x68 |
| `us-8029fb24` | `kyoshin/COption` | `COptionAdvanceState10To3` | base:not-found | 0x68 |
| `us-8029fe00` | `kyoshin/COption` | `COptionReadConfigCell` | base:not-found | 0x1C4 |
| `us-8029ffc4` | `kyoshin/COption` | `COptionReadCurrentConfigCell` | base:not-found | 0x14 |
| `us-8029ffd8` | `kyoshin/COption` | `COptionWriteConfigCell` | base:not-found | 0x180 |
| `us-802a0158` | `kyoshin/COption` | `COptionGetCellMax` | base:not-found | 0x24 |
| `us-802a017c` | `kyoshin/COption` | `COptionConfigIncrement` | base:not-found | 0x408 |
| `us-802a0988` | `kyoshin/COption` | `COptionSetupSubAnim20` | base:not-found | 0x88 |
| `us-802a0a10` | `kyoshin/COption` | `COptionSetupSubAnim24` | base:not-found | 0x88 |
| `us-802a0a98` | `kyoshin/COption` | `COptionRefreshCursorPanes` | base:not-found | 0x1A4 |
| `us-802a0e58` | `kyoshin/COption` | `OnFileEvent__7COptionFP10CEventFile` | base:not-found | 0x1DC |
| `us-801f51b0` | `kyoshin/CScrollBar` | `CScrollBar_loadLayoutArc` | base:not-found | 0x4C |
| `us-801f526c` | `kyoshin/CScrollBar` | `CScrollBar_draw` | base:not-found | 0x2C |
| `us-801f5324` | `kyoshin/CScrollBar` | `CScrollBar_isActive` | base:not-found | 0x8 |
| `us-801f5338` | `kyoshin/CScrollBar` | `CScrollBar_requestScrollIn` | base:not-found | 0x20 |
| `us-801f5358` | `kyoshin/CScrollBar` | `CScrollBar_requestScrollOut` | base:not-found | 0x20 |
| `us-80296e20` | `kyoshin/CSysWinSave` | `initChainGauge__FPv` | base:not-found | 0x10 |
| `us-80296e30` | `kyoshin/CSysWinSave` | `resetChainGauge__FPv` | base:not-found | 0x10 |
| `us-802812ec` | `kyoshin/CSysWinScenarioLog` | `OnFileEvent__18CSysWinScenarioLogFP10CEventFile` | base:not-found | 0x8 |
| `us-8012565c` | `kyoshin/CSysWinSelect` | `__ct__CSysWinSelect` | base:not-found | 0x150 |
| `us-801257ac` | `kyoshin/CSysWinSelect` | `__dt__13CSysWinSelectFv` | base:not-found | 0x80 |
| `us-8012582c` | `kyoshin/CSysWinSelect` | `Init__13CSysWinSelectFv` | base:not-found | 0xBC |
| `us-801258e8` | `kyoshin/CSysWinSelect` | `Term__13CSysWinSelectFv` | base:not-found | 0x68 |
| `us-80125950` | `kyoshin/CSysWinSelect` | `Move__13CSysWinSelectFv` | base:not-found | 0x164 |
| `us-80125ab4` | `kyoshin/CSysWinSelect` | `cbRenderBefore__13CSysWinSelectFv` | base:not-found | 0x98 |
| `us-80125dac` | `kyoshin/CSysWinSelect` | `__dt__Q34nw4r2ut5ColorFv` | base:not-found | 0x40 |
| `us-8004552c` | `kyoshin/CTaskGameEff` | `Move__12CTaskGameEffFv` | base:not-found | 0x30 |
| `us-80045a4c` | `kyoshin/CTaskGameEff` | `Draw__12CTaskGameEffFv` | base:not-found | 0x4 |
| `us-80132bc4` | `kyoshin/CUICfManager` | `__dt__801320F0` | base:not-found | 0x40 |
| `us-801560b0` | `kyoshin/cf/CArtsParam` | `__sinit_\CArtsParam_cpp` | base:not-found | 0x7C |
| `us-8019455c` | `kyoshin/cf/CPartsChange` | `__dt__Q22cf12CPartsChangeFv` | base:not-found | 0x40 |
| `task-culling-vfunc1` | `kyoshin/cf/CTaskCulling` | `ICulling_UnkVirtualFunc1__Q22cf12CTaskCullingFPQ22ml8CFrustum` | both-not-found |  |
| `us-8005a044` | `kyoshin/cf/CTaskEnvironment` | `Draw__16CTaskEnvironmentFv` | base:not-found | 0x4 |
| `us-8016be58` | `kyoshin/cf/CTaskREvtSequence` | `Draw__Q22cf17CTaskREvtSequenceFv` | base:not-found | 0x4 |
| `us-8016db2c` | `kyoshin/cf/CTaskREvtSequence` | `OnFileEvent__17CTaskREvtSequenceFP10CEventFile` | base:not-found | 0x8 |
| `us-80141e90` | `kyoshin/cf/CfBdat` | `__dt__Q22cf6CfBdatFv` | base:not-found | 0x40 |
| `us-80142ec0` | `kyoshin/cf/CfBdat` | `OnFileEvent__Q22cf6CfBdatFP10CEventFile` | base:not-found | 0x88 |
| `us-80142f48` | `kyoshin/cf/CfBdat` | `__sinit_\CfBdat_cpp` | base:not-found | 0x18 |
| `us-8006c120` | `kyoshin/cf/CfCam` | `__dt__Q22cf10CfCamEventFv` | base:not-found | 0x58 |
| `cam-init-intf` | `kyoshin/cf/CfCamEvent` | `CfCamEvent_initCamIntfInstances` | base:not-found |  |
| `us-80270948` | `kyoshin/cf/CfGimmickEne` | `__dt__Q22cf12CfGimmickEneFv` | base:not-found | 0xC0 |
| `us-8023e0a4` | `kyoshin/cf/CfNandManager` | `__dt__28CTTask<Q22cf13CfNandManager>Fv` | base:not-found | 0x58 |
| `us-8023e0fc` | `kyoshin/cf/CfNandManager` | `__dt__8023BFCC` | base:not-found | 0x4C |
| `us-8023e148` | `kyoshin/cf/CfNandManager` | `__dt__Q22cf13CfNandManagerFv` | base:not-found | 0xE4 |
| `us-8023e22c` | `kyoshin/cf/CfNandManager` | `Init__Q22cf13CfNandManagerFv` | base:not-found | 0xC |
| `us-8023e238` | `kyoshin/cf/CfNandManager` | `Term__Q22cf13CfNandManagerFv` | base:not-found | 0x4C |
| `us-8023e284` | `kyoshin/cf/CfNandManager` | `Move__Q22cf13CfNandManagerFv` | base:not-found | 0x60 |
| `us-80240618` | `kyoshin/cf/CfNandManager` | `handleFileEvent__Q22cf13CfNandManagerFv` | base:not-found | 0x70 |
| `us-80242188` | `kyoshin/cf/CfNandManager` | `create__Q22cf13CfNandManagerFv` | base:not-found | 0x8C |
| `us-80242718` | `kyoshin/cf/CfNandManager` | `Move__28CTTask<Q22cf13CfNandManager>Fv` | base:not-found | 0x48 |
| `us-80242760` | `kyoshin/cf/CfNandManager` | `Draw__28CTTask<Q22cf13CfNandManager>Fv` | base:not-found | 0x48 |
| `us-802427ac` | `kyoshin/cf/CfNandManager` | `OnFileEvent__Q22cf13CfNandManagerFv` | base:not-found | 0x8 |
| `us-802427b4` | `kyoshin/cf/CfNandManager` | `destroyViaWorkEvent__Q22cf13CfNandManagerFv` | base:not-found | 0x8 |
| `us-802427bc` | `kyoshin/cf/CfNandManager` | `handleRenderCallback__Q22cf13CfNandManagerFv` | base:not-found | 0x8 |
| `us-802427c4` | `kyoshin/cf/CfNandManager` | `destroyViaRender__Q22cf13CfNandManagerFv` | base:not-found | 0x8 |
| `us-8006a878` | `kyoshin/cf/CfTFile` | `__dt__Q22cf7CfTFileFv` | base:not-found | 0x40 |
| `us-80088fc0` | `kyoshin/cf/CtrlEnemy` | `CActorParam_UnkVirtualFunc113__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-80088fe0` | `kyoshin/cf/CtrlEnemy` | `CActorParam_UnkVirtualFunc117__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-80088fe8` | `kyoshin/cf/CtrlEnemy` | `CActorParam_UnkVirtualFunc119__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-80088ff0` | `kyoshin/cf/CtrlEnemy` | `CActorParam_getActorLevel__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-800891bc` | `kyoshin/cf/CtrlEnemy` | `CActorParam_UnkVirtualFunc115__Q22cf11CActorParamFv` | base:not-found | 0x14 |
| `us-8009e998` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_setHp__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-800a0064` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_setStatScale__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-800a006c` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_setGaugeFlagA__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-800a0074` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_setGaugeFlagB__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-800a007c` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_setGaugeRateA__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-800a0084` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_setGaugeRateB__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-800a008c` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_setMoveRate__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-800a0094` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_setStatByte__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-800a00a4` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_setGaugeFloat__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-800a3034` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_addSpentCurrency__Q22cf11CActorParamFv` | base:not-found | 0x24 |
| `us-800a3058` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_addSecondCurrency__Q22cf11CActorParamFv` | base:not-found | 0x24 |
| `us-800a349c` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_addAccumCurrency__Q22cf11CActorParamFv` | base:not-found | 0x24 |
| `us-800a34c0` | `kyoshin/cf/CtrlObjectParam` | `CActorParam_setSpentCurrency__Q22cf11CActorParamFv` | base:not-found | 0x8 |
| `us-80097980` | `kyoshin/cf/CtrlPc` | `__dt__Q22cf7CtrlPadFv` | base:not-found | 0x5C |
| `us-800979dc` | `kyoshin/cf/CtrlPc` | `__ct__cf_CtrlPc` | base:not-found | 0xCC |
| `us-80097aa8` | `kyoshin/cf/CtrlPc` | `__dt__Q22cf6CtrlPcFv` | base:not-found | 0x64 |
| `us-800993d4` | `kyoshin/cf/CtrlPc` | `CAttackParam_getArtsFlagByte__Q22cf12CAttackParamFv` | base:not-found | 0x8 |
| `us-800bb384` | `kyoshin/cf/object/CfObjectModel` | `CfObject_UnkVirtualFunc6__Q22cf13CfObjectModelFv` | base:not-found | 0xB8 |
| `us-800bb480` | `kyoshin/cf/object/CfObjectModel` | `CfObjectModel_UnkVirtualFunc2__Q22cf13CfObjectModelFv` | base:not-found | 0x4 |
| `us-800bbaa4` | `kyoshin/cf/object/CfObjectModel` | `CfObject_UnkVirtualFunc27__Q22cf13CfObjectModelFPv` | base:not-found | 0x7C |
| `us-800bbbec` | `kyoshin/cf/object/CfObjectModel` | `CfObject_UnkVirtualFunc32__Q22cf13CfObjectModelFv` | base:not-found | 0x18 |
| `us-800bbeb4` | `kyoshin/cf/object/CfObjectModel` | `CObjectParam_UnkVirtualFunc2__Q22cf13CfObjectModelFv` | base:not-found | 0x5C |
| `us-800bc508` | `kyoshin/cf/object/CfObjectModel` | `CfObjectModel_UnkVirtualFunc6__Q22cf13CfObjectModelFv` | base:not-found | 0x8 |
| `us-802a61b4` | `kyoshin/cf/voice/cvsys/CVS_THREAD` | `__ct__cf_CVS_THREAD` | base:not-found | 0x4C |
| `us-802a8140` | `kyoshin/cf/voice/cvsys/CVS_THREAD_CHAIN` | `chainVoiceAlwaysOne` | base:not-found | 0x8 |
| `us-802a8600` | `kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN` | `downVoiceFixed240` | base:not-found | 0x8 |
| `us-802a9520` | `kyoshin/cf/voice/cvsys/CVS_THREAD_FAINT` | `func_802A6DEC` | base:not-found | 0x8 |
| `us-800b13a4` | `kyoshin/code_800B06A4` | `__ct__17UnkClass_800B0AD8Fv` | base:not-found | 0x1C |
| `us-800b140c` | `kyoshin/code_800B06A4` | `clearWorkCounters` | base:not-found | 0x10 |
| `us-800b1458` | `kyoshin/code_800B06A4` | `factoryCtorNop` | base:not-found | 0x4 |
| `us-800b18b8` | `kyoshin/code_800B06A4` | `factoryPoolCount` | base:not-found | 0x8 |
| `us-800b1c34` | `kyoshin/code_800B06A4` | `gflagOrMask` | base:not-found | 0x10 |
| `us-800b1d90` | `kyoshin/code_800B06A4` | `factoryIterBegin` | base:not-found | 0x10 |
| `us-800b1dac` | `kyoshin/code_800B06A4` | `factoryIterNext` | base:not-found | 0x10 |
| `us-800b1dbc` | `kyoshin/code_800B06A4` | `factoryIterSentinel` | base:not-found | 0xC |
| `us-800b20e4` | `kyoshin/code_800B06A4` | `storeIterWord` | base:not-found | 0x8 |
| `us-800b20ec` | `kyoshin/code_800B06A4` | `copyIterWord` | base:not-found | 0xC |
| `us-800b20f8` | `kyoshin/code_800B06A4` | `followIterNext` | base:not-found | 0x10 |
| `us-800b2358` | `kyoshin/code_800B06A4` | `tboxIterBegin` | base:not-found | 0x10 |
| `us-800b2368` | `kyoshin/code_800B06A4` | `tboxIterItem` | base:not-found | 0xC |
| `us-800b2374` | `kyoshin/code_800B06A4` | `tboxIterStep` | base:not-found | 0x18 |
| `us-800b238c` | `kyoshin/code_800B06A4` | `copyIterFrom` | base:not-found | 0xC |
| `us-800b24d8` | `kyoshin/code_800B06A4` | `gflagHasMask` | base:not-found | 0x18 |
| `us-800b24f0` | `kyoshin/code_800B06A4` | `gflagSetOrClear` | base:not-found | 0x1C |
| `us-800b2784` | `kyoshin/code_800B06A4` | `gflagClearMask` | base:not-found | 0x10 |
| `us-800b3a7c` | `kyoshin/code_800B06A4` | `testObj64bit9` | base:not-found | 0xC |
| `us-800b3a94` | `kyoshin/code_800B06A4` | `testFlag64b3` | base:not-found | 0xC |
| `us-800b3aac` | `kyoshin/code_800B06A4` | `testObj64bit5` | base:not-found | 0xC |
| `us-800b3ab8` | `kyoshin/code_800B06A4` | `testObj64bit6` | base:not-found | 0xC |
| `us-800b3ad0` | `kyoshin/code_800B06A4` | `testObj64bit14` | base:not-found | 0xC |
| `us-800b428c` | `kyoshin/code_800B06A4` | `getObjIdAt74` | base:not-found | 0x8 |
| `us-800b4320` | `kyoshin/code_800B06A4` | `copyIterWord2` | base:not-found | 0xC |
| `us-800b432c` | `kyoshin/code_800B06A4` | `unlinkMatchingNode` | base:not-found | 0x28 |
| `us-800b4648` | `kyoshin/code_800B06A4` | `testObj6Cmask` | base:not-found | 0x18 |
| `us-800b4e84` | `kyoshin/code_800B06A4` | `setFieldD08Hi` | base:not-found | 0xC |
| `us-800b4e90` | `kyoshin/code_800B06A4` | `testObj64bit16` | base:not-found | 0xC |
| `us-800b5854` | `kyoshin/code_800B06A4` | `testCamBit15` | base:not-found | 0xC |
| `us-800b587c` | `kyoshin/code_800B06A4` | `orObj6Cmask` | base:not-found | 0x10 |
| `us-800b6240` | `kyoshin/code_800B06A4` | `sortBufIdentity` | base:not-found | 0x4 |
| `us-800b6244` | `kyoshin/code_800B06A4` | `sortBufEnd` | base:not-found | 0x10 |
| `us-800b6254` | `kyoshin/code_800B06A4` | `clearField380` | base:not-found | 0xC |
| `us-800b6260` | `kyoshin/code_800B06A4` | `sortBufAt` | base:not-found | 0xC |
| `us-800b626c` | `kyoshin/code_800B06A4` | `sortBufCount` | base:not-found | 0x8 |
| `us-800b6d80` | `kyoshin/code_800B06A4` | `gflagNandMask` | base:not-found | 0x10 |
| `us-800b6d90` | `kyoshin/code_800B06A4` | `scaleAboveZero` | base:not-found | 0x18 |
| `us-800b6da8` | `kyoshin/code_800B06A4` | `testObj68bit6` | base:not-found | 0xC |
| `us-800b6db4` | `kyoshin/code_800B06A4` | `testObj68mask` | base:not-found | 0x18 |
| `us-800b6de0` | `kyoshin/code_800B06A4` | `getObjU16_45C0` | base:not-found | 0x8 |
| `us-800b6de8` | `kyoshin/code_800B06A4` | `testObj64bit26` | base:not-found | 0xC |
| `us-800b6df4` | `kyoshin/code_800B06A4` | `orObj68bits5_6` | base:not-found | 0x10 |
| `us-800b6e04` | `kyoshin/code_800B06A4` | `reslistIsBusy` | base:not-found | 0x18 |
| `us-800b6e40` | `kyoshin/code_800B06A4` | `cmpFloatAt4` | base:not-found | 0x18 |
| `us-800b6fa8` | `kyoshin/code_800B06A4` | `orObj68bit6` | base:not-found | 0x10 |
| `us-800b70e8` | `kyoshin/code_800B06A4` | `getPtrAt1A8` | base:not-found | 0x8 |
| `us-800b70f0` | `kyoshin/code_800B06A4` | `clearField700` | base:not-found | 0xC |
| `us-800b74e8` | `kyoshin/code_800B06A4` | `getReslistB48` | base:not-found | 0x24 |
| `us-800b750c` | `kyoshin/code_800B06A4` | `getReslistB68` | base:not-found | 0x24 |
| `us-800b7530` | `kyoshin/code_800B06A4` | `getReslistB88` | base:not-found | 0x24 |
| `us-800b7554` | `kyoshin/code_800B06A4` | `getReslistBE8` | base:not-found | 0x24 |
| `us-800b7578` | `kyoshin/code_800B06A4` | `getReslistBC8` | base:not-found | 0x24 |
| `us-800b791c` | `kyoshin/code_800B06A4` | `workBufCount` | base:not-found | 0x8 |
| `us-800b7924` | `kyoshin/code_800B06A4` | `workBufSize` | base:not-found | 0x8 |
| `us-800b792c` | `kyoshin/code_800B06A4` | `workBufElem` | base:not-found | 0x28 |
| `us-800b7954` | `kyoshin/code_800B06A4` | `resetMgrReslist` | base:not-found | 0x24 |
| `us-800b7ae4` | `kyoshin/code_800B06A4` | `getGlobalU32_40F4` | base:not-found | 0x8 |
| `us-800b7b2c` | `kyoshin/code_800B06A4` | `getPtrAt720` | base:not-found | 0x8 |
| `us-800b7d08` | `kyoshin/code_800B06A4` | `unlinkListNode` | base:not-found | 0x28 |
| `us-800b7ee0` | `kyoshin/code_800B06A4` | `setObj6Cmask` | base:not-found | 0x28 |
| `us-800b7fc0` | `kyoshin/code_800B06A4` | `firstReslistB28` | base:not-found | 0x28 |
| `us-800b7fe8` | `kyoshin/code_800B06A4` | `firstReslistB48` | base:not-found | 0x28 |
| `us-800b8010` | `kyoshin/code_800B06A4` | `firstReslistB68` | base:not-found | 0x28 |
| `us-800b8038` | `kyoshin/code_800B06A4` | `firstReslistB88` | base:not-found | 0x28 |
| `us-800b8060` | `kyoshin/code_800B06A4` | `firstReslistBE8` | base:not-found | 0x28 |
| `us-800b8088` | `kyoshin/code_800B06A4` | `firstReslistBC8` | base:not-found | 0x28 |
| `us-800b80b0` | `kyoshin/code_800B06A4` | `firstReslistC08` | base:not-found | 0x28 |
| `us-800b80d8` | `kyoshin/code_800B06A4` | `firstReslistBA8` | base:not-found | 0x28 |
| `us-800b81a8` | `kyoshin/code_800B06A4` | `insertReslistB88` | base:not-found | 0x38 |
| `us-800b911c` | `kyoshin/code_800B06A4` | `factoryEventNopB` | base:not-found | 0x4 |
| `us-800b9bd0` | `kyoshin/code_800B06A4` | `gmCallInit1120` | base:not-found | 0x24 |
| `us-800b9c5c` | `kyoshin/code_800B06A4` | `gmCallNopA` | base:not-found | 0x24 |
| `us-800b9c80` | `kyoshin/code_800B06A4` | `gmCallNopB` | base:not-found | 0x24 |
| `us-800b9ca4` | `kyoshin/code_800B06A4` | `gmCallInit1954` | base:not-found | 0x24 |
| `us-800b9e40` | `kyoshin/code_800B06A4` | `gmSpawnAfterAA2` | base:not-found | 0x24 |
| `us-800b9fe8` | `kyoshin/code_800B06A4` | `setObjXY_73A` | base:not-found | 0xC |
| `us-800ba004` | `kyoshin/code_800B06A4` | `copyObjVec724` | base:not-found | 0x1C |
| `us-800ba020` | `kyoshin/code_800B06A4` | `setObjFloat730` | base:not-found | 0x8 |
| `us-800ba34c` | `kyoshin/code_800B06A4` | `clearFixStrBuf` | base:not-found | 0x10 |
| `us-8013743c` | `kyoshin/code_80135FDC` | `setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl` | base:not-found | 0x50 |
| `us-8013748c` | `kyoshin/code_80135FDC` | `setLayoutTextBoxNumber__FPQ34nw4r3lyt6LayoutPcUc` | base:not-found | 0x60 |
| `us-80137800` | `kyoshin/code_80135FDC` | `buildLayout__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc` | base:not-found | 0x84 |
| `us-80137884` | `kyoshin/code_80135FDC` | `bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc` | base:not-found | 0x98 |
| `us-801379b4` | `kyoshin/code_80135FDC` | `drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii` | base:not-found | 0x218 |
| `us-80137dc0` | `kyoshin/code_80135FDC` | `advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf` | base:not-found | 0xE4 |
| `us-80138a3c` | `kyoshin/code_80135FDC` | `playUISound__FUl` | base:not-found | 0x18 |
| `us-80139ae8` | `kyoshin/code_80135FDC` | `releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor` | base:not-found | 0x74 |
| `us-8013bdec` | `kyoshin/code_80135FDC` | `incrementEventCounter__FUl` | base:not-found | 0x454 |
| `us-802ba644` | `kyoshin/help/CHelp` | `__ct__Q22cf5CHelpFv` | base:not-found | 0x18 |
| `us-802ba6d8` | `kyoshin/help/CHelp` | `CHelp_refreshPartyMenu` | base:not-found | 0x48 |
| `us-802ba720` | `kyoshin/help/CHelp` | `setHelpFlag__Q22cf11CHelpSwitchFv` | base:not-found | 0xC |
| `us-802ba72c` | `kyoshin/help/CHelp` | `trySetHelpFlag__Q22cf11CHelpSwitchFv` | base:not-found | 0x28 |
| `us-802ba754` | `kyoshin/help/CHelp` | `updateHelpFlag__Q22cf11CHelpSwitchFv` | base:not-found | 0x1C |
| `us-80060cd0` | `kyoshin/plugin/pluginCam` | `camPluginNoopV64` | base:not-found | 0x4 |
| `us-80060dac` | `kyoshin/plugin/pluginCam` | `camPluginNoopV4C` | base:not-found | 0x4 |
| `us-80060e38` | `kyoshin/plugin/pluginCam` | `camPluginNoopV50` | base:not-found | 0x4 |
| `us-80060ec4` | `kyoshin/plugin/pluginCam` | `camPluginNoopV54` | base:not-found | 0x4 |
| `us-80061108` | `kyoshin/plugin/pluginCam` | `camPluginNoopV68` | base:not-found | 0x4 |
| `us-80061284` | `kyoshin/plugin/pluginCam` | `camPluginNoopV6C` | base:not-found | 0x4 |
| `us-80168dfc` | `kyoshin/realtimeevt/CREvtMem` | `__ct__cf_CREvtMem` | base:not-found | 0x44 |
| `us-8045a0ec` | `monolib/src/core/CDesktop` | `__dt__Q222@unnamed@CDesktop_cpp@18CDesktopBackGroundFv` | base:not-found | 0x58 |
| `us-8045a144` | `monolib/src/core/CDesktop` | `__dt__Q222@unnamed@CDesktop_cpp@17CDesktopExceptionFv` | base:not-found | 0x58 |
| `us-8045a19c` | `monolib/src/core/CDesktop` | `wkStandbyLogout__Q222@unnamed@CDesktop_cpp@17CDesktopExceptionFv` | base:not-found | 0x44 |
| `us-8045a1e0` | `monolib/src/core/CDesktop` | `wkStandbyLogout__Q222@unnamed@CDesktop_cpp@18CDesktopBackGroundFv` | base:not-found | 0x44 |
| `us-80450288` | `monolib/src/device/CDevice` | `getInstance__Q221@unnamed@CDevice_cpp@16CDeviceExceptionFv` | base:not-found | 0x8 |
| `us-80454bcc` | `monolib/src/device/CDeviceFileJob` | `cancel__14CDeviceFileJobFP25CDeviceFileJob_UnkStruct1` | base:not-found | 0x8 |
| `us-80454bdc` | `monolib/src/device/CDeviceFileJob` | `__dt__14CDeviceFileJobFv` | base:not-found | 0x58 |
| `us-8045742c` | `monolib/src/device/CDeviceFontInfoExt` | `__dt__18CDeviceFontInfoExtFv` | base:not-found | 0x88 |
| `us-8044a9c4` | `monolib/src/device/CDeviceVI` | `__dt__9IErrorWiiFv` | base:not-found | 0x40 |
| `us-8044b4b0` | `monolib/src/device/CDeviceVI` | `viBeginFrame__11CDeviceVICbFv` | base:not-found | 0x4 |
| `us-804dfc18` | `monolib/src/effect/CERand` | `__sinit_\CERand_cpp` | base:not-found | 0x38 |
| `us-8045dc90` | `monolib/src/lib/CLibCri` | `@452@viBeginFrame__7CLibCriFv` | base:not-found | 0x8 |
| `us-8045dca0` | `monolib/src/lib/CLibCri` | `@456@errorWiiCB__7CLibCriFv` | base:not-found | 0x8 |
| `us-804642d8` | `monolib/src/lib/UnkClass_80460308` | `processData__17UnkClass_80460308Fv` | base:not-found | 0x420 |
| `us-8046a560` | `monolib/src/lod/UnkClass_80466348` | `tevStageTexRasc__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xA8 |
| `us-8046a608` | `monolib/src/lod/UnkClass_80466348` | `tevStageTexC0AlphaA0__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xA8 |
| `us-8046a6b0` | `monolib/src/lod/UnkClass_80466348` | `tevStageTexKonstC0__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xB4 |
| `us-8046a764` | `monolib/src/lod/UnkClass_80466348` | `tevStageTexKonstSubC0__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xB4 |
| `us-8046a818` | `monolib/src/lod/UnkClass_80466348` | `tevStageTexMulC0__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xA8 |
| `us-8046a8c0` | `monolib/src/lod/UnkClass_80466348` | `tevStageTexAlpha__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xA8 |
| `us-8046a968` | `monolib/src/lod/UnkClass_80466348` | `tevStageTexKonstOnly__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xB4 |
| `us-8046aa1c` | `monolib/src/lod/UnkClass_80466348` | `tevStageTexRascReg0__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x9C |
| `us-8046aab8` | `monolib/src/lod/UnkClass_80466348` | `tevStageTexRascReg1__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x9C |
| `us-8046ab54` | `monolib/src/lod/UnkClass_80466348` | `tevStageC0C1Tex__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xB4 |
| `us-8046ac08` | `monolib/src/lod/UnkClass_80466348` | `tevStageC0C1TexAlpha__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xB4 |
| `us-8046acbc` | `monolib/src/lod/UnkClass_80466348` | `tevStageC0TexAlphaBlend__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xA8 |
| `us-8046ad64` | `monolib/src/lod/UnkClass_80466348` | `tevStageKonstTexReg1__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xA8 |
| `us-8046ae0c` | `monolib/src/lod/UnkClass_80466348` | `tevStageRascTexC0__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xA8 |
| `us-8046aeb4` | `monolib/src/lod/UnkClass_80466348` | `tevStageRascTexSubC0__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xA8 |
| `us-8046af5c` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPosTex__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x6C |
| `us-8046afc8` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPosClrTex__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x78 |
| `us-8046b040` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPosNrmTex__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x78 |
| `us-8046b0b8` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPosNrmClrTex__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x84 |
| `us-8046b13c` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPosNrmTex2__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x78 |
| `us-8046b1b4` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPosNrmClrTex2__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x84 |
| `us-8046b238` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPosClrIndexed__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x6C |
| `us-8046b2a4` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPnPosTex__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x78 |
| `us-8046b31c` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPnPosClrTex__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x84 |
| `us-8046b3a0` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPnPosNrmTex__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x84 |
| `us-8046b424` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPnPosNrmClrTex__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x90 |
| `us-8046b4b4` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPnPosNrmTex3__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x84 |
| `us-8046b538` | `monolib/src/lod/UnkClass_80466348` | `setupVtxDescPnPosNrmClrTex3__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x90 |
| `us-8046b768` | `monolib/src/lod/UnkClass_80466348` | `setAmbColorScaled1__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x100 |
| `us-8046b8b8` | `monolib/src/lod/UnkClass_80466348` | `setAmbColorScaled2__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x100 |
| `us-8046b9b8` | `monolib/src/lod/UnkClass_80466348` | `setAmbColorScaledAlpha1__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x11C |
| `us-8046bad4` | `monolib/src/lod/UnkClass_80466348` | `setAmbColorAlphaSimple__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xD0 |
| `us-8046bba4` | `monolib/src/lod/UnkClass_80466348` | `setAmbColorScaledAlpha2__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x11C |
| `us-8046bf88` | `monolib/src/lod/UnkClass_80466348` | `setAmbColorComposite1__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x124 |
| `us-8046c0ac` | `monolib/src/lod/UnkClass_80466348` | `setAmbColorComposite2__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0xD8 |
| `us-8046c184` | `monolib/src/lod/UnkClass_80466348` | `setAmbColorComposite3__Q23LOD17UnkClass_80466348Fv` | base:not-found | 0x124 |
| `us-8046eaa8` | `monolib/src/lod/code_8046A530` | `isLodActive__Q23LOD17UnkClass_8046A530Fv` | base:not-found | 0x4 |
| `us-8046eaac` | `monolib/src/lod/code_8046A530` | `updateLodPositions__Q23LOD17UnkClass_8046A530Fv` | base:not-found | 0x78 |
| `us-80470f84` | `monolib/src/lod/code_8046A530` | `checkLodFlagState__Q23LOD17UnkClass_8046A530Fv` | base:not-found | 0x24 |
| `us-80470fa8` | `monolib/src/lod/code_8046A530` | `getLodVisState__Q23LOD17UnkClass_8046A530Fv` | base:not-found | 0x8 |
| `us-804f9cc4` | `monolib/src/nand/CNBanner` | `__dt__804F5738` | base:not-found | 0xB8 |
| `us-804e98e0` | `monolib/src/scn/CMdlMaterial` | `__ct__CMdlMaterial` | base:not-found | 0x34 |
| `us-804ea27c` | `monolib/src/scn/CMdlMaterial` | `__dt__804E5DE0` | base:not-found | 0x58 |
| `us-80446674` | `monolib/src/work/CWorkRoot` | `__dt__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThreadFv` | base:not-found | 0x74 |
| `us-80446d24` | `monolib/src/work/CWorkRoot` | `wkStandbyLogout__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThreadFv` | base:not-found | 0x44 |
| `us-803e50e8` | `nw4r/src/g3d/g3d_anmtexpat` | `GetUpdateRate__Q34nw4r3g3d15AnmObjTexPatResCFv` | base:not-found | 0x8 |
| `us-803e5b14` | `nw4r/src/g3d/g3d_anmtexsrt` | `GetUpdateRate__Q34nw4r3g3d15AnmObjTexSrtResCFv` | base:not-found | 0x8 |
| `us-803ddbd4` | `nw4r/src/g3d/res/g3d_resmat` | `CopyTo__Q34nw4r3g3d9ResMatPixCFPv` | base:not-found | 0x3C |
| `us-803ddc10` | `nw4r/src/g3d/res/g3d_resmat` | `CopyTo__Q34nw4r3g3d14ResMatTevColorCFPv` | base:not-found | 0x3C |
| `us-803ddc4c` | `nw4r/src/g3d/res/g3d_resmat` | `CopyTo__Q34nw4r3g3d20ResMatIndMtxAndScaleCFPv` | base:not-found | 0x3C |
| `us-803ddc88` | `nw4r/src/g3d/res/g3d_resmat` | `CopyTo__Q34nw4r3g3d17ResMatTexCoordGenCFPv` | base:not-found | 0x3C |
| `us-803ddd70` | `nw4r/src/g3d/res/g3d_resmat` | `CopyTo__Q34nw4r3g3d9ResTexObjCFPv` | base:not-found | 0x88 |
| `us-803ddea4` | `nw4r/src/g3d/res/g3d_resmat` | `CopyTo__Q34nw4r3g3d10ResTlutObjCFPv` | base:not-found | 0x88 |
| `us-803ddf2c` | `nw4r/src/g3d/res/g3d_resmat` | `CopyTo__Q34nw4r3g3d9ResTexSrtCFPv` | base:not-found | 0xD4 |
| `us-803de0a4` | `nw4r/src/g3d/res/g3d_resmat` | `CopyTo__Q34nw4r3g3d10ResGenModeCFPv` | base:not-found | 0x34 |
| `us-803de17c` | `nw4r/src/g3d/res/g3d_resmat` | `CopyTo__Q34nw4r3g3d10ResMatMiscCFPv` | base:not-found | 0x6C |
| `us-803def14` | `nw4r/src/g3d/res/g3d_resmat` | `CopyTo__Q34nw4r3g3d10ResMatChanCFPv` | base:not-found | 0x5C |
| `us-803df518` | `nw4r/src/g3d/res/g3d_resmat` | `GetResTex__Q34nw4r3g3d14ResTexPlttInfoCFv` | base:not-found | 0xC |
| `us-8040a548` | `nw4r/src/lyt/lyt_material` | `FindAnimationLink__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform` | base:not-found | 0x8 |
| `us-80414770` | `nw4r/src/snd/snd_BasicPlayer` | `SetBiquadFilter__Q44nw4r3snd6detail11BasicPlayerFif` | base:not-found | 0xC |
| `us-80415a98` | `nw4r/src/snd/snd_BasicSound` | `AttachSoundActor__Q44nw4r3snd6detail10BasicSoundFPQ34nw4r3snd10SoundActor` | base:not-found | 0x8 |
| `us-80415aa0` | `nw4r/src/snd/snd_BasicSound` | `AttachExternalSoundPlayer__Q44nw4r3snd6detail10BasicSoundFPQ44nw4r3snd6detail19ExternalSoundPlayer` | base:not-found | 0x8 |
| `us-8041993c` | `nw4r/src/snd/snd_MidiSeqPlayer` | `@208@InvalidateData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv` | base:not-found | 0x8 |
| `us-80422840` | `nw4r/src/snd/snd_SoundPlayer` | `__dt__Q34nw4r2ut44LinkList<Q44nw4r3snd6detail10BasicSound,256>Fv` | base:not-found | 0x58 |
| `us-8042bccc` | `nw4r/src/snd/snd_WaveFile` | `__ct__Q44nw4r3snd6detail14WaveFileReaderFPCQ54nw4r3snd6detail8WaveFile8WaveInfo` | base:not-found | 0x8 |
| `us-8042e154` | `nw4r/src/ut/ut_IOStream` | `__sinit_\ut_IOStream_cpp` | base:not-found | 0xC |

## Notes

- These were not rematchable via history (calls/`extern` only, or never defined
  in the owning TU).
- Next: rematch per unit, or restore bodies from other sources; then `cycle`.
- Excluded restored set still needs ninja rebuild of affected TUs before a
  re-sweep can certify them.
