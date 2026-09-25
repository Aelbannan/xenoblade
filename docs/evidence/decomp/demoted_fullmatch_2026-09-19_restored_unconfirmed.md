# Demoted FULL_MATCH — unconfirmed history restores (2026-09-19)

Rebuilt the 22 TUs that received history-restored bodies, then verified the
110 restored `FULL_MATCH` IDs against current objects.

## Summary

- Restored IDs checked: **110**
- Confirmed identical (kept `FULL_MATCH`): **2**
- Demoted (unconfirmed): **108**
  - still `base:not-found`: **107**
  - extractable but non-identical: **1**
- Registry `FULL_MATCH`: 14139 → **14031**
- Sidecar certs dropped: 42

## Kept (byte-identical after rebuild)

- `us-801f531c` `kyoshin/CScrollBar` `CScrollBar_isVisible`
- `us-80223c78` `kyoshin/makecrystal/CModelDispMakeCrystal` `initCrystalData`

## Rebuild failures (16/22 units)

History restores left many TUs uncompilable (redefinitions, wrong signatures,
missing types). Old `.o` files remain on disk; restored symbols were never
emitted into a fresh object.

- `kyoshin/cf/CfBdat`
- `kyoshin/help/CHelp`
- `kyoshin/help/CHelp_ClosePartyMenu`
- `kyoshin/cf/CfNandManager`
- `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act`
- `CriWare/src/sofdec/sfdcore/mpv/mpv_slice`
- `CriWare/src/adx/adxt/adx_tsvr`
- `CriWare/src/sofdec/sfdcore/mps/mps_dec`
- `CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf`
- `CriWare/src/sofdec/sfx/sfx_cnv`
- `kyoshin/cf/CfRes`
- `kyoshin/cf/CtrlPc`
- `kyoshin/COption`
- `kyoshin/menu/CMenuQstCnt`
- `kyoshin/cf/chain/CChain`
- `monolib/src/core/CPackItem`

## Demoted by unit

| n | unit |
|--:|------|
| 40 | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` |
| 17 | `kyoshin/cf/CfNandManager` |
| 10 | `kyoshin/cf/CfRes` |
| 8 | `kyoshin/cf/CfBdat` |
| 6 | `CriWare/src/sofdec/sfx/sfx_cnv` |
| 4 | `CriWare/src/sofdec/sfdcore/mpv/mpv_slice` |
| 4 | `nw4r/src/g3d/g3d_calcview` |
| 3 | `kyoshin/cf/CtrlPc` |
| 3 | `CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf` |
| 2 | `kyoshin/realtimeevt/CREvtCamera` |
| 2 | `CriWare/src/sofdec/sfdcore/mps/mps_dec` |
| 1 | `kyoshin/cf/CtrlEnemy` |
| 1 | `kyoshin/menu/CMenuEnemyState` |
| 1 | `kyoshin/menu/CMenuQstCnt` |
| 1 | `kyoshin/cf/chain/CChain` |
| 1 | `kyoshin/COption` |
| 1 | `kyoshin/help/CHelp` |
| 1 | `kyoshin/help/CHelp_ClosePartyMenu` |
| 1 | `CriWare/src/adx/adxt/adx_tsvr` |
| 1 | `monolib/src/core/CPackItem` |

## Demoted rows

| id | unit | symbol | kind |
|----|------|--------|------|
| `us-80387b24` | `CriWare/src/adx/adxt/adx_tsvr` | `adxt_eos_entry` | base:not-found |
| `us-803bdf54` | `CriWare/src/sofdec/sfdcore/mps/mps_dec` | `MPSDEC_Init` | base:not-found |
| `us-803bdf58` | `CriWare/src/sofdec/sfdcore/mps/mps_dec` | `MPSDEC_Finish` | base:not-found |
| `us-803bce64` | `CriWare/src/sofdec/sfdcore/mpv/mpv_slice` | `MPVSL_Init` | base:not-found |
| `us-803bce68` | `CriWare/src/sofdec/sfdcore/mpv/mpv_slice` | `MPVSL_Finish` | base:not-found |
| `us-803bce6c` | `CriWare/src/sofdec/sfdcore/mpv/mpv_slice` | `MPVSL_Create` | base:not-found |
| `us-803bce70` | `CriWare/src/sofdec/sfdcore/mpv/mpv_slice` | `MPVSL_Destroy` | base:not-found |
| `us-803ca8e8` | `CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf` | `SFD_SetPicUsrBuf` | base:not-found |
| `us-803ca968` | `CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf` | `SFMPVF_ResetPicUsrBuf` | base:not-found |
| `us-803caab4` | `CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf` | `SFMPVF_InitPicUsr` | base:not-found |
| `us-803d7600` | `CriWare/src/sofdec/sfx/sfx_cnv` | `SFX_SetCnvFrmCbFunc` | base:not-found |
| `us-803d7608` | `CriWare/src/sofdec/sfx/sfx_cnv` | `SFX_SetCopyAlphaCbFunc` | base:not-found |
| `us-803d7610` | `CriWare/src/sofdec/sfx/sfx_cnv` | `SFX_SetMakeLumiTableCbFunc` | base:not-found |
| `us-803d761c` | `CriWare/src/sofdec/sfx/sfx_cnv` | `SFX_SetMakeAlp3TableCbFunc` | base:not-found |
| `us-803d7628` | `CriWare/src/sofdec/sfx/sfx_cnv` | `SFX_SetMakeAlp3110TableCbFunc` | base:not-found |
| `us-803d7634` | `CriWare/src/sofdec/sfx/sfx_cnv` | `SFX_SetMakeColAdjTableCbFunc` | base:not-found |
| `us-802e1714` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_enable` | base:not-found |
| `us-802e1824` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_disable` | base:not-found |
| `us-802e18d4` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_disable_timer_cback` | base:not-found |
| `us-802e1990` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_set_dev_name` | base:not-found |
| `us-802e1998` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_set_visibility` | base:not-found |
| `us-802e19e0` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_bond` | base:not-found |
| `us-802e1a50` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_pin_reply` | base:not-found |
| `us-802e1aec` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_auth_reply` | base:not-found |
| `us-802e1bc0` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_search_start` | base:not-found |
| `us-802e2594` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_queue_search` | base:not-found |
| `us-802e25dc` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_queue_disc` | base:not-found |
| `us-802e2624` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_search_clear_queue` | base:not-found |
| `us-802e2668` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_search_cancel_cmpl` | base:not-found |
| `us-802e26ac` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_search_cancel_transac_cmpl` | base:not-found |
| `us-802e2708` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_search_cancel_notify` | base:not-found |
| `us-802e29a0` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_sdp_callback` | base:not-found |
| `us-802e29e8` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_inq_results_cb` | base:not-found |
| `us-802e2a78` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_inq_cmpl_cb` | base:not-found |
| `us-802e2b90` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_disc_remname_cback` | base:not-found |
| `us-802e2c18` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_cancel_rmt_name` | base:not-found |
| `us-802e2c1c` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_authorize_cback` | base:not-found |
| `us-802e2ce4` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_pinname_cback` | base:not-found |
| `us-802e2da4` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_pin_cback` | base:not-found |
| `us-802e2ed4` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_link_key_request_cback` | base:not-found |
| `us-802e2edc` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_new_link_key_cback` | base:not-found |
| `us-802e2f78` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_authentication_complete_cback` | base:not-found |
| `us-802e2ffc` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_local_addr_cback` | base:not-found |
| `us-802e3024` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_signal_strength` | base:not-found |
| `us-802e3060` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_signal_strength_timer_cback` | base:not-found |
| `us-802e3154` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_acl_change_cback` | base:not-found |
| `us-802e31c4` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_acl_change` | base:not-found |
| `us-802e3444` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_disable_conn_down_timer_cback` | base:not-found |
| `us-802e3460` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_rssi_cback` | base:not-found |
| `us-802e34cc` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_link_quality_cback` | base:not-found |
| `us-802e3538` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_l2cap_server_compress_cback` | base:not-found |
| `us-802e3854` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_rm_cback` | base:not-found |
| `us-802e395c` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_keep_acl` | base:not-found |
| `us-802e39e0` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_immediate_disable` | base:not-found |
| `us-802e39fc` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_reset_complete` | base:not-found |
| `us-802e3a00` | `RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act` | `bta_dm_send_hci_reset` | base:not-found |
| `us-802a1034` | `kyoshin/COption` | `sinit_8029E7D8` | base:not-found |
| `us-80141ed0` | `kyoshin/cf/CfBdat` | `loadBdatFileTable__Q22cf6CfBdatFv` | base:not-found |
| `us-8014215c` | `kyoshin/cf/CfBdat` | `resetMapBdatFileDataPointers__Q22cf6CfBdatFv` | base:not-found |
| `us-801421bc` | `kyoshin/cf/CfBdat` | `loadMapBdatFileDataPointers__Q22cf6CfBdatFii` | base:not-found |
| `us-80142524` | `kyoshin/cf/CfBdat` | `func_80141B20` | base:not-found |
| `us-80142558` | `kyoshin/cf/CfBdat` | `func_80141B54` | base:not-found |
| `us-801429e4` | `kyoshin/cf/CfBdat` | `func_80141FE0` | base:not-found |
| `us-80142bc8` | `kyoshin/cf/CfBdat` | `func_801421C4` | base:not-found |
| `us-80142eac` | `kyoshin/cf/CfBdat` | `getBdatStringEntry__Q22cf6CfBdatFUs` | base:not-found |
| `us-8023e2e4` | `kyoshin/cf/CfNandManager` | `func_8023C1B4` | base:not-found |
| `us-8023e2f0` | `kyoshin/cf/CfNandManager` | `func_8023C1C0` | base:not-found |
| `us-80240688` | `kyoshin/cf/CfNandManager` | `func_8023E544` | base:not-found |
| `us-80241438` | `kyoshin/cf/CfNandManager` | `func_8023F288` | base:not-found |
| `us-802414a4` | `kyoshin/cf/CfNandManager` | `func_8023F2F4` | base:not-found |
| `us-80241570` | `kyoshin/cf/CfNandManager` | `func_8023F3C0` | base:not-found |
| `us-8024177c` | `kyoshin/cf/CfNandManager` | `func_8023F5CC` | base:not-found |
| `us-80241840` | `kyoshin/cf/CfNandManager` | `func_8023F690` | base:not-found |
| `us-80241a10` | `kyoshin/cf/CfNandManager` | `func_8023F860` | base:not-found |
| `us-80241c14` | `kyoshin/cf/CfNandManager` | `func_8023FA64` | base:not-found |
| `us-80241cd8` | `kyoshin/cf/CfNandManager` | `func_8023FB28` | base:not-found |
| `us-80241d50` | `kyoshin/cf/CfNandManager` | `func_8023FBA0` | base:not-found |
| `us-80241dc8` | `kyoshin/cf/CfNandManager` | `func_8023FC18` | base:not-found |
| `us-80241e7c` | `kyoshin/cf/CfNandManager` | `func_8023FCCC` | base:not-found |
| `us-80241efc` | `kyoshin/cf/CfNandManager` | `func_8023FD4C` | base:not-found |
| `us-80242214` | `kyoshin/cf/CfNandManager` | `func_8024005C` | base:not-found |
| `us-802427a8` | `kyoshin/cf/CfNandManager` | `sinit_802405F0` | base:not-found |
| `us-800643d0` | `kyoshin/cf/CfRes` | `CfRes_vcall34` | base:not-found |
| `us-8006441c` | `kyoshin/cf/CfRes` | `CfRes_vcall38` | base:not-found |
| `us-800645bc` | `kyoshin/cf/CfRes` | `CfRes_vcall14` | base:not-found |
| `us-800645d4` | `kyoshin/cf/CfRes` | `CfRes_vcall02` | base:not-found |
| `us-80064b3c` | `kyoshin/cf/CfRes` | `CfRes_vcall04` | base:not-found |
| `us-80064b5c` | `kyoshin/cf/CfRes` | `CfRes_vcall05` | base:not-found |
| `us-80064b7c` | `kyoshin/cf/CfRes` | `CfRes_vcall08` | base:not-found |
| `us-80064b9c` | `kyoshin/cf/CfRes` | `CfRes_vcall07` | base:not-found |
| `us-80065484` | `kyoshin/cf/CfRes` | `CfRes_vcall17` | base:not-found |
| `us-800666e8` | `kyoshin/cf/CfRes` | `CfRes_vcall16` | base:not-found |
| `us-800891d0` | `kyoshin/cf/CtrlEnemy` | `CActorParam_getActorType__Q22cf11CActorParamFv` | base:not-found |
| `us-80098d88` | `kyoshin/cf/CtrlPc` | `CActorParam_UnkVirtualFunc122__Q22cf11CActorParamFv` | base:not-found |
| `us-800991e8` | `kyoshin/cf/CtrlPc` | `func_80098810` | base:not-found |
| `us-800993cc` | `kyoshin/cf/CtrlPc` | `CAttackParam_UnkVirtualFunc3__Q22cf12CAttackParamFv` | base:not-found |
| `us-8027bb10` | `kyoshin/cf/chain/CChain` | `CChain_chkActorList` | base:not-found |
| `us-802ba65c` | `kyoshin/help/CHelp` | `CHelp_UnkVirtualFunc2__Q22cf5CHelpFv` | base:not-found |
| `us-802baf5c` | `kyoshin/help/CHelp_ClosePartyMenu` | `isPartyMenuReady__Q22cf19CHelp_OpenPartyMenuFv` | base:not-found |
| `us-80113fc4` | `kyoshin/menu/CMenuEnemyState` | `sinit_801134E8` | stale |
| `us-8022905c` | `kyoshin/menu/CMenuQstCnt` | `copyQstInfo` | base:not-found |
| `us-80181da4` | `kyoshin/realtimeevt/CREvtCamera` | `REvtCam_AlwaysTrue1` | base:not-found |
| `us-80181dac` | `kyoshin/realtimeevt/CREvtCamera` | `REvtCam_GetField1CValue` | base:not-found |
| `us-804e2de4` | `monolib/src/core/CPackItem` | `setupHashTable__9CPackItemFv` | base:not-found |
| `us-803ee8e0` | `nw4r/src/g3d/g3d_calcview` | `Calc_BILLBOARD_STD__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@FPQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34bPCQ34nw4r4math5MTX34Q34nw4r3g3d6ResMdlUl` | base:not-found |
| `us-803eee88` | `nw4r/src/g3d/g3d_calcview` | `Calc_BILLBOARD_ROT__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@FPQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34bPCQ34nw4r4math5MTX34Q34nw4r3g3d6ResMdlUl` | base:not-found |
| `us-803ef57c` | `nw4r/src/g3d/g3d_calcview` | `Calc_BILLBOARD_Y__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@FPQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34bPCQ34nw4r4math5MTX34Q34nw4r3g3d6ResMdlUl` | base:not-found |
| `us-803ef85c` | `nw4r/src/g3d/g3d_calcview` | `Calc_BILLBOARD_PERSP_Y__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@FPQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34bPCQ34nw4r4math5MTX34Q34nw4r3g3d6ResMdlUl` | base:not-found |

## Notes

- Restored source blocks are still in the tree (marked with history comments)
  but need compile fixes before they can re-enter `FULL_MATCH`.
- Next: fix TU compile errors, rebuild, then rematch/`cycle` per symbol.
