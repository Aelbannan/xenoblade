# FULL_MATCH demotions — sweep_stale_full_match.py (2026-09-19, post-rebuild)

Demoted **131** rows from `FULL_MATCH` → `NOT_STARTED` (non-byte-identical bodies via `_byte_identical_with_relocs`).

Source: `python3 tools/coop/sweep_stale_full_match.py` (r6 F1), run after restoring `build.ninja` and rebuilding previously missing decomp base objects.

Pre-apply scan found 238 stale candidates (FULL_MATCH≈14107, scanned≈13440); applying pass reported **131** demotions (FULL_MATCH rows scanned: 14187; skipped FileNotFoundError=254, ElfSymbolError=413).

Official apply count: **131**. This table lists **128** rows from the pre-apply stale candidate set that are now `NOT_STARTED`/`BACKLOG` in `tools/coop/targets.json` (3 apply-time demotion(s) were outside the pre-scan set due to concurrent registry/build drift).

Post-sweep registry: `FULL_MATCH=14191`, of which `instruction_match>=100`: `13928`, `instruction_match==0`: `174`.

## By unit (top)

| unit | n |
|---|--:|
| `kyoshin/plugin/pluginSnd` | 12 |
| `CriWare/src/adx/cvfs/cri_cvfs` | 9 |
| `CriWare/src/adx/gcci/gcci` | 8 |
| `CriWare/src/adx/lsc/lsc` | 8 |
| `nw4r/src/g3d/res/g3d_resfile` | 8 |
| `CriWare/src/adx/adxf/adx_fs` | 7 |
| `CriWare/src/adx/sj/sj_rbf` | 5 |
| `CriWare/src/adx/svm/svm` | 5 |
| `nw4r/src/g3d/g3d_anmtexsrt` | 5 |
| `RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_pane` | 4 |
| `RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_CharWriter` | 4 |
| `RVL_SDK/src/revolution/mix/remote` | 4 |
| `CriWare/src/sofdec/sfx/sfx_lib` | 4 |
| `nw4r/src/g3d/g3d_anmclr` | 4 |
| `CriWare/src/adx/sj/sj_uni` | 3 |
| `nw4r/src/math/math_triangular` | 3 |
| `RVL_SDK/src/revolution/bte/stack/sdp/sdp_server` | 2 |
| `RVL_SDK/src/revolution/mtx/mtx` | 2 |
| `CriWare/src/adx/adxt/adx_tsvr` | 2 |
| `nw4r/src/g3d/g3d_scnmdl` | 2 |
| `nw4r/src/snd/snd_RemoteSpeakerManager` | 2 |
| `monolib/src/device/CDeviceFileJobReadDvd` | 2 |
| `kyoshin/help/CHelp_GameOver` | 1 |
| `kyoshin/help/CHelp_Talk` | 1 |
| `RVL_SDK/src/revolution/bte/stack/btm/btm_sec` | 1 |
| `RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_layout` | 1 |
| `RVL_SDK/src/revolution/kpad/KPAD` | 1 |
| `RVL_SDK/src/revolution/wpad/WPAD` | 1 |
| `CriWare/src/adx/adxt/adx_sjd` | 1 |
| `CriWare/src/adx/lsc/lsc_ini` | 1 |
| `CriWare/src/adx/mfci/mfci` | 1 |
| `CriWare/src/adx/sj/sj_mem` | 1 |
| `CriWare/src/adx/adxt/adx_bsps` | 1 |
| `CriWare/src/adx/wiirna/ax_rna` | 1 |
| `CriWare/src/adx/adxt/srcwii/adx_mwii` | 1 |
| `CriWare/src/sofdec/mwply/mwsfdply` | 1 |
| `CriWare/src/sofdec/sfdcore/mpv/mpv_deli` | 1 |
| `CriWare/src/sofdec/sfdcore/sfd/sfd_adxt` | 1 |
| `nw4r/src/g3d/res/g3d_resanmclr` | 1 |
| `nw4r/src/g3d/g3d_scnmdlexpand` | 1 |
| … (5 more units) | |

**Units touched:** 45

## Demoted targets

| id | unit | symbol | function | address |
|---|---|---|---|---|
| us-801ac454 | kyoshin/plugin/pluginSnd | `playBgm` | playBgm | 0x801AC454 |
| us-801ac534 | kyoshin/plugin/pluginSnd | `stopBgm` | stopBgm | 0x801AC534 |
| us-801ac8f4 | kyoshin/plugin/pluginSnd | `stopFieldBgm` | stopFieldBgm | 0x801AC8F4 |
| us-801ac9cc | kyoshin/plugin/pluginSnd | `stopTownBgm` | stopTownBgm | 0x801AC9CC |
| us-801acacc | kyoshin/plugin/pluginSnd | `setVolBgm` | setVolBgm | 0x801ACACC |
| us-801acbb8 | kyoshin/plugin/pluginSnd | `playVoice` | playVoice | 0x801ACBB8 |
| us-801acc60 | kyoshin/plugin/pluginSnd | `stopVoice` | stopVoice | 0x801ACC60 |
| us-801acd7c | kyoshin/plugin/pluginSnd | `playSeCommon` | playSeCommon | 0x801ACD7C |
| us-801acf0c | kyoshin/plugin/pluginSnd | `playSeMap` | playSeMap | 0x801ACF0C |
| us-801ad1c4 | kyoshin/plugin/pluginSnd | `stopSeCommon` | stopSeCommon | 0x801AD1C4 |
| us-801ad264 | kyoshin/plugin/pluginSnd | `stopSeMap` | stopSeMap | 0x801AD264 |
| us-801ad304 | kyoshin/plugin/pluginSnd | `seCamPos` | seCamPos | 0x801AD304 |
| us-802bab64 | kyoshin/help/CHelp_GameOver | `isAvailable__Q22cf14CHelp_GameOverFv` | cf::CHelp_GameOver::isAvailable() | 0x802BAB64 |
| us-802bb12c | kyoshin/help/CHelp_Talk | `resetTalkHelp__Q22cf10CHelp_TalkFv` | cf::CHelp_Talk::resetTalkHelp() | 0x802BB12C |
| us-802efca4 | RVL_SDK/src/revolution/bte/stack/btm/btm_sec | `btm_sec_mkey_comp_event` | btm_sec_mkey_comp_event | 0x802EFCA4 |
| us-80309124 | RVL_SDK/src/revolution/bte/stack/sdp/sdp_server | `sdp_server_handle_client_req` | sdp_server_handle_client_req | 0x80309124 |
| us-80309278 | RVL_SDK/src/revolution/bte/stack/sdp/sdp_server | `process_service_search` | process_service_search | 0x80309278 |
| us-803302e0 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_layout | `__ct__Q36nw4hbm3lyt6LayoutFv` | constructor() | 0x803302E0 |
| us-80334050 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_pane | `__ct__Q36nw4hbm3lyt4PaneFPCQ46nw4hbm3lyt3res4Pane` | constructor(const ...*, Q, 4, 6, n, wchar_t, 4, unsigned char, bool, m, 3, long, y, t, 3, r, e, short, 4, ...*, a, n, e) | 0x80334050 |
| us-80334170 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_pane | `__dt__Q36nw4hbm3lyt4PaneFv` | destructor() | 0x80334170 |
| us-80334300 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_pane | `GetPaneRect__Q36nw4hbm3lyt4PaneCFRCQ36nw4hbm3lyt8DrawInfo` | GetPaneRect(R, C, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 3, long, y, t, 8, D, r, a, wchar_t, I, n, float, o) | 0x80334300 |
| us-803350a0 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_pane | `GetVtxPos__Q36nw4hbm3lyt4PaneCFv` | GetVtxPos() | 0x803350A0 |
| us-80339630 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_CharWriter | `__ct__Q36nw4hbm2ut10CharWriterFv` | constructor() | 0x80339630 |
| us-80339a20 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_CharWriter | `SetupGX__Q36nw4hbm2ut10CharWriterFv` | SetupGX() | 0x80339A20 |
| us-8033a3c0 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_CharWriter | `SetGradationMode__Q36nw4hbm2ut10CharWriterFQ46nw4hbm2ut10CharWriter13GradationMode` | SetGradationMode(Q, 4, 6, n, wchar_t, 4, unsigned char, bool, m, 2, u, t, 1, 0, C, unsigned char, a, r, W, r, int, t, e, r, 1, 3, G, r, a, double, a, t, int, o, n, M, o, double, e) | 0x8033A3C0 |
| us-8033a510 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_CharWriter | `SetTextColor__Q36nw4hbm2ut10CharWriterFQ36nw4hbm2ut5ColorQ36nw4hbm2ut5Color` | SetTextColor(Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 2, u, t, 5, C, o, long, o, r, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 2, u, t, 5, C, o, long, o, r) | 0x8033A510 |
| us-8034afb0 | RVL_SDK/src/revolution/kpad/KPAD | `KPADiSamplingCallback` | KPADiSamplingCallback | 0x8034AFB0 |
| us-8034f860 | RVL_SDK/src/revolution/mix/remote | `MIXRmtSetVolumes` | MIXRmtSetVolumes | 0x8034F860 |
| us-8034f8b0 | RVL_SDK/src/revolution/mix/remote | `MIXRmtSetFader` | MIXRmtSetFader | 0x8034F8B0 |
| us-8034f910 | RVL_SDK/src/revolution/mix/remote | `__MIXRmtUpdateSettings` | __MIXRmtUpdateSettings | 0x8034F910 |
| us-8034fd50 | RVL_SDK/src/revolution/mix/remote | `__MIXRmtResetChannel` | __MIXRmtResetChannel | 0x8034FD50 |
| us-80350780 | RVL_SDK/src/revolution/mtx/mtx | `C_MTXLightFrustum` | C_MTXLightFrustum | 0x80350780 |
| us-80350930 | RVL_SDK/src/revolution/mtx/mtx | `C_MTXLightOrtho` | C_MTXLightOrtho | 0x80350930 |
| us-8036fe30 | RVL_SDK/src/revolution/wpad/WPAD | `WPADControlSpeaker` | WPADControlSpeaker | 0x8036FE30 |
| us-80380b94 | CriWare/src/adx/adxf/adx_fs | `ADXF_OpenNw` | ADXF_OpenNw | 0x80380B94 |
| us-80381754 | CriWare/src/adx/adxf/adx_fs | `adxf_ExecOne` | adxf_ExecOne | 0x80381754 |
| us-80381a6c | CriWare/src/adx/adxf/adx_fs | `adxf_Seek` | adxf_Seek | 0x80381A6C |
| us-80381cf8 | CriWare/src/adx/adxf/adx_fs | `ADXF_GetFsizeSct` | ADXF_GetFsizeSct | 0x80381CF8 |
| us-80381f14 | CriWare/src/adx/adxf/adx_fs | `adxf_ChkPrmGfr` | adxf_ChkPrmGfr | 0x80381F14 |
| us-8038203c | CriWare/src/adx/adxf/adx_fs | `adxf_GetFnameRangeEx` | adxf_GetFnameRangeEx | 0x8038203C |
| us-803822a0 | CriWare/src/adx/adxf/adx_fs | `ADXF_GetFnameFromPt` | ADXF_GetFnameFromPt | 0x803822A0 |
| us-80383034 | CriWare/src/adx/adxt/adx_sjd | `adxsjd_decexec_start` | adxsjd_decexec_start | 0x80383034 |
| us-803879a4 | CriWare/src/adx/adxt/adx_tsvr | `adxt_trap_entry` | adxt_trap_entry | 0x803879A4 |
| us-80388744 | CriWare/src/adx/adxt/adx_tsvr | `adxt_ExecHndl` | adxt_ExecHndl | 0x80388744 |
| us-803927fc | CriWare/src/adx/cvfs/cri_cvfs | `cvFsCallUsrErrFn` | cvFsCallUsrErrFn | 0x803927FC |
| us-80393290 | CriWare/src/adx/cvfs/cri_cvfs | `cvFsClose` | cvFsClose | 0x80393290 |
| us-80393350 | CriWare/src/adx/cvfs/cri_cvfs | `cvFsTell` | cvFsTell | 0x80393350 |
| us-80393410 | CriWare/src/adx/cvfs/cri_cvfs | `cvFsSeek` | cvFsSeek | 0x80393410 |
| us-803934d0 | CriWare/src/adx/cvfs/cri_cvfs | `cvFsReqRd` | cvFsReqRd | 0x803934D0 |
| us-80393590 | CriWare/src/adx/cvfs/cri_cvfs | `cvFsStopTr` | cvFsStopTr | 0x80393590 |
| us-8039361c | CriWare/src/adx/cvfs/cri_cvfs | `cvFsExecServer` | cvFsExecServer | 0x8039361C |
| us-80393684 | CriWare/src/adx/cvfs/cri_cvfs | `cvFsGetStat` | cvFsGetStat | 0x80393684 |
| us-80393b00 | CriWare/src/adx/cvfs/cri_cvfs | `cvFsIsAvailableRetry` | cvFsIsAvailableRetry | 0x80393B00 |
| us-80393d50 | CriWare/src/adx/gcci/gcci | `gcCiExecServer` | gcCiExecServer | 0x80393D50 |
| us-80393dac | CriWare/src/adx/gcci/gcci | `gcCiEntryErrFunc` | gcCiEntryErrFunc | 0x80393DAC |
| us-80393dc0 | CriWare/src/adx/gcci/gcci | `gcCiGetFileSize` | gcCiGetFileSize | 0x80393DC0 |
| us-80394194 | CriWare/src/adx/gcci/gcci | `gcCiSeek` | gcCiSeek | 0x80394194 |
| us-80394258 | CriWare/src/adx/gcci/gcci | `gcCiTell` | gcCiTell | 0x80394258 |
| us-80394798 | CriWare/src/adx/gcci/gcci | `gcCiGetStat` | gcCiGetStat | 0x80394798 |
| us-803947fc | CriWare/src/adx/gcci/gcci | `gcCiGetSctLen` | gcCiGetSctLen | 0x803947FC |
| us-80394920 | CriWare/src/adx/gcci/gcci | `gcCiGetNumTr` | gcCiGetNumTr | 0x80394920 |
| us-80394ab0 | CriWare/src/adx/lsc/lsc_ini | `LSC_Init` | LSC_Init | 0x80394AB0 |
| us-80394f98 | CriWare/src/adx/lsc/lsc | `lsc_Create` | lsc_Create | 0x80394F98 |
| us-80395548 | CriWare/src/adx/lsc/lsc | `LSC_Start` | LSC_Start | 0x80395548 |
| us-8039567c | CriWare/src/adx/lsc/lsc | `LSC_Stop` | LSC_Stop | 0x8039567C |
| us-8039574c | CriWare/src/adx/lsc/lsc | `LSC_Pause` | LSC_Pause | 0x8039574C |
| us-80395838 | CriWare/src/adx/lsc/lsc | `LSC_GetStat` | LSC_GetStat | 0x80395838 |
| us-80395898 | CriWare/src/adx/lsc/lsc | `LSC_GetNumStm` | LSC_GetNumStm | 0x80395898 |
| us-803958f4 | CriWare/src/adx/lsc/lsc | `LSC_SetFlowLimit` | LSC_SetFlowLimit | 0x803958F4 |
| us-803959ac | CriWare/src/adx/lsc/lsc | `LSC_SetLpFlg` | LSC_SetLpFlg | 0x803959AC |
| us-80395e2c | CriWare/src/adx/mfci/mfci | `mfCiSeek` | mfCiSeek | 0x80395E2C |
| us-803972d0 | CriWare/src/adx/sj/sj_mem | `sjmem_IsGetChunk` | sjmem_IsGetChunk | 0x803972D0 |
| us-803975d4 | CriWare/src/adx/sj/sj_rbf | `sjrbf_Create` | sjrbf_Create | 0x803975D4 |
| us-80397be8 | CriWare/src/adx/sj/sj_rbf | `sjrbf_GetChunk` | sjrbf_GetChunk | 0x80397BE8 |
| us-80397e6c | CriWare/src/adx/sj/sj_rbf | `sjrbf_PutChunk` | sjrbf_PutChunk | 0x80397E6C |
| us-803980a8 | CriWare/src/adx/sj/sj_rbf | `sjrbf_UngetChunk` | sjrbf_UngetChunk | 0x803980A8 |
| us-80398360 | CriWare/src/adx/sj/sj_rbf | `sjrbf_IsGetChunk` | sjrbf_IsGetChunk | 0x80398360 |
| us-80398928 | CriWare/src/adx/sj/sj_uni | `sjuni_Create` | sjuni_Create | 0x80398928 |
| us-80398cf4 | CriWare/src/adx/sj/sj_uni | `sjuni_Reset` | sjuni_Reset | 0x80398CF4 |
| us-803995a4 | CriWare/src/adx/sj/sj_uni | `sjuni_IsGetChunk` | sjuni_IsGetChunk | 0x803995A4 |
| us-80399c04 | CriWare/src/adx/svm/svm | `SVM_CallErr` | SVM_CallErr | 0x80399C04 |
| us-80399cdc | CriWare/src/adx/svm/svm | `SVM_CallErr1` | SVM_CallErr1 | 0x80399CDC |
| us-8039a53c | CriWare/src/adx/svm/svm | `SVM_GotoSvrBorder` | SVM_GotoSvrBorder | 0x8039A53C |
| us-8039a658 | CriWare/src/adx/svm/svm | `SVM_SetCbLock` | SVM_SetCbLock | 0x8039A658 |
| us-8039a66c | CriWare/src/adx/svm/svm | `SVM_SetCbUnlock` | SVM_SetCbUnlock | 0x8039A66C |
| us-8039b2b4 | CriWare/src/adx/adxt/adx_bsps | `ADXB_CheckSpsd` | ADXB_CheckSpsd | 0x8039B2B4 |
| us-8039c388 | CriWare/src/adx/wiirna/ax_rna | `AXRNA_Init` | AXRNA_Init | 0x8039C388 |
| us-8039e3f4 | CriWare/src/adx/adxt/srcwii/adx_mwii | `ADXM_IsSetupThrd` | ADXM_IsSetupThrd | 0x8039E3F4 |
| us-803a5104 | CriWare/src/sofdec/mwply/mwsfdply | `mwPlyPause` | mwPlyPause | 0x803A5104 |
| us-803a914c | CriWare/src/sofdec/sfdcore/mpv/mpv_deli | `criware_803A59B0` | criware_803A59B0 | 0x803A914C |
| us-803c0c20 | CriWare/src/sofdec/sfdcore/sfd/sfd_adxt | `sfadxt_GetTime` | sfadxt_GetTime | 0x803C0C20 |
| us-803d7ef8 | CriWare/src/sofdec/sfx/sfx_lib | `SFX_SetErrFn` | SFX_SetErrFn | 0x803D7EF8 |
| us-803d820c | CriWare/src/sofdec/sfx/sfx_lib | `SFXLIB_Error` | SFXLIB_Error | 0x803D820C |
| us-803d8240 | CriWare/src/sofdec/sfx/sfx_lib | `SFX_SetCcirFx` | SFX_SetCcirFx | 0x803D8240 |
| us-803d8250 | CriWare/src/sofdec/sfx/sfx_lib | `SFX_GetForceSplitField` | SFX_GetForceSplitField | 0x803D8250 |
| us-803da5d8 | nw4r/src/g3d/res/g3d_resfile | `GetResAnmTexPat__Q34nw4r3g3d7ResFileCFi` | GetResAnmTexPat(int) | 0x803DA5D8 |
| us-803da670 | nw4r/src/g3d/res/g3d_resfile | `GetResAnmTexSrt__Q34nw4r3g3d7ResFileCFi` | GetResAnmTexSrt(int) | 0x803DA670 |
| us-803da708 | nw4r/src/g3d/res/g3d_resfile | `GetResAnmScn__Q34nw4r3g3d7ResFileCFi` | GetResAnmScn(int) | 0x803DA708 |
| us-803da884 | nw4r/src/g3d/res/g3d_resfile | `HasResAnmTexPat__Q34nw4r3g3d7ResFileCFv` | HasResAnmTexPat() | 0x803DA884 |
| us-803da8d0 | nw4r/src/g3d/res/g3d_resfile | `HasResAnmTexSrt__Q34nw4r3g3d7ResFileCFv` | HasResAnmTexSrt() | 0x803DA8D0 |
| us-803da91c | nw4r/src/g3d/res/g3d_resfile | `HasResAnmShp__Q34nw4r3g3d7ResFileCFv` | HasResAnmShp() | 0x803DA91C |
| us-803da968 | nw4r/src/g3d/res/g3d_resfile | `HasResAnmScn__Q34nw4r3g3d7ResFileCFv` | HasResAnmScn() | 0x803DA968 |
| us-803da9b4 | nw4r/src/g3d/res/g3d_resfile | `Bind__Q34nw4r3g3d7ResFileFQ34nw4r3g3d7ResFile` | Bind(int, long, e) | 0x803DA9B4 |
| us-803e0038 | nw4r/src/g3d/res/g3d_resanmclr | `GetAnmResult__Q34nw4r3g3d9ResAnmClrCFPQ34nw4r3g3d12ClrAnmResultUlf` | GetAnmResult(...*, Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 1, 2, C, long, r, A, n, m, R, e, short, u, long, t, long, float) | 0x803E0038 |
| us-803e41e8 | nw4r/src/g3d/g3d_anmclr | `SetFrame__Q34nw4r3g3d15AnmObjMatClrResFf` | SetFrame(float) | 0x803E41E8 |
| us-803e42b0 | nw4r/src/g3d/g3d_anmclr | `SetUpdateRate__Q34nw4r3g3d15AnmObjMatClrResFf` | SetUpdateRate(float) | 0x803E42B0 |
| us-803e436c | nw4r/src/g3d/g3d_anmclr | `UpdateFrame__Q34nw4r3g3d15AnmObjMatClrResFv` | UpdateFrame() | 0x803E436C |
| us-803e444c | nw4r/src/g3d/g3d_anmclr | `Bind__Q34nw4r3g3d15AnmObjMatClrResFQ34nw4r3g3d6ResMdl` | Bind(Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 6, R, e, short, M, double, long) | 0x803E444C |
| us-803e5998 | nw4r/src/g3d/g3d_anmtexsrt | `SetFrame__Q34nw4r3g3d15AnmObjTexSrtResFf` | SetFrame(float) | 0x803E5998 |
| us-803e5a60 | nw4r/src/g3d/g3d_anmtexsrt | `SetUpdateRate__Q34nw4r3g3d15AnmObjTexSrtResFf` | SetUpdateRate(float) | 0x803E5A60 |
| us-803e5b1c | nw4r/src/g3d/g3d_anmtexsrt | `UpdateFrame__Q34nw4r3g3d15AnmObjTexSrtResFv` | UpdateFrame() | 0x803E5B1C |
| us-803e5bfc | nw4r/src/g3d/g3d_anmtexsrt | `Bind__Q34nw4r3g3d15AnmObjTexSrtResFQ34nw4r3g3d6ResMdl` | Bind(Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 6, R, e, short, M, double, long) | 0x803E5BFC |
| us-803e5e58 | nw4r/src/g3d/g3d_anmtexsrt | `ApplyTexSrtAnmResult__Q24nw4r3g3dFQ34nw4r3g3d9ResTexSrtQ34nw4r3g3d20ResMatIndMtxAndScalePCQ34nw4r3g3d15TexSrtAnmResult` | ApplyTexSrtAnmResult(Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 9, R, e, short, T, e, long long, S, r, t, Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 2, 0, R, e, short, M, a, t, I, n, double, M, t, long long, A, n, double, S, char, a, long, e, const ...*, Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 1, 5, T, e, long long, S, r, t, A, n, m, R, e, short, u, long, t) | 0x803E5E58 |
| us-803fd6e0 | nw4r/src/g3d/g3d_scnmdl | `InitBuffer__Q34nw4r3g3d6ScnMdlFv` | InitBuffer() | 0x803FD6E0 |
| us-803fda94 | nw4r/src/g3d/g3d_scnmdl | `CleanMatBuffer__Q34nw4r3g3d6ScnMdlFUlUl` | CleanMatBuffer(long, long) | 0x803FDA94 |
| us-803fe580 | nw4r/src/g3d/g3d_scnmdlexpand | `Insert__Q34nw4r3g3d12ScnMdlExpandFUlPQ34nw4r3g3d6ScnObj` | Insert(long, ...*, Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 6, S, char, n, O, bool, j) | 0x803FE580 |
| us-804095c0 | nw4r/src/lyt/lyt_material | `SetupGX__Q34nw4r3lyt8MaterialFbUc` | SetupGX(bool, char) | 0x804095C0 |
| us-8040d090 | nw4r/src/math/math_triangular | `SinFIdx__Q24nw4r4mathFf` | SinFIdx(float) | 0x8040D090 |
| us-8040d0f8 | nw4r/src/math/math_triangular | `CosFIdx__Q24nw4r4mathFf` | CosFIdx(float) | 0x8040D0F8 |
| us-8040d1bc | nw4r/src/math/math_triangular | `Atan2FIdx__Q24nw4r4mathFff` | Atan2FIdx(float, float) | 0x8040D1BC |
| us-8041b6b8 | nw4r/src/snd/snd_RemoteSpeakerManager | `GetInstance__Q44nw4r3snd6detail20RemoteSpeakerManagerFv` | GetInstance() | 0x8041B6B8 |
| us-8041b840 | nw4r/src/snd/snd_RemoteSpeakerManager | `RemoteSpeakerAlarmProc__Q44nw4r3snd6detail20RemoteSpeakerManagerFP7OSAlarmP9OSContext` | RemoteSpeakerAlarmProc(...*, 7, O, S, A, long, a, r, m, ...*, 9, O, S, C, o, n, t, e, long long, t) | 0x8041B840 |
| us-80427b28 | nw4r/src/snd/snd_TaskManager | `GetInstance__Q44nw4r3snd6detail11TaskManagerFv` | GetInstance() | 0x80427B28 |
| us-80431878 | nw4r/src/ut/ut_CharWriter | `__ct__Q34nw4r2ut10CharWriterFv` | constructor() | 0x80431878 |
| us-80454cd4 | monolib/src/device/CDeviceFileJobReadDvd | `cancel__21CDeviceFileJobReadDvdFPCc` | CDeviceFileJobReadDvd::cancel(const ...*, char) | 0x80454CD4 |
| us-80454d68 | monolib/src/device/CDeviceFileJobReadDvd | `cancel__21CDeviceFileJobReadDvdFP11CFileHandle` | CDeviceFileJobReadDvd::cancel(...*, 1, 1, C, int, long, e, H, a, n, double, long, e) | 0x80454D68 |
| us-804728f0 | monolib/src/lod/LODMemMan | `setElementLodLevel__Q23LOD9LODMemManFv` | LOD::LODMemMan::setElementLodLevel() | 0x804728F0 |
| us-804a2544 | monolib/src/scn/CScnFrame | `update__9CScnFrameFv` | CScnFrame::update() | 0x804A2544 |
