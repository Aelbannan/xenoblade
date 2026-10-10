# Category D — extern_data_sections full inventory (279 entries)

Derived 2026-10-10 in the paseo-unitrules-d worktree (static-only; no build).
Companion to `unit_rules_category_d_decision.md` (§1); that doc holds the analysis,
options, and ratchet plan. This file is the per-entry record.

Method: `UNIT_RULES` entries with a truthy `extern_data_sections` field. Live = basename
is `Matching` in `configure.py` (proxy; coordinator re-derives link-live from a configured
`build.ninja` via the README snippet). Retail sizes = `config/us/splits.txt` slices of the
stripped sections in the TU(s) the rule applies to (scoped `#` keys resolve to the matching
twin only — all 279 are retail-empty where applicable, i.e. the vacuous-MATCH set). Owners =
splits-block owner of each pool/rename/retarget label address from `config/us/symbols.txt`; `GAP`
= dissolved-blob region from removed `split1.s`/`nw4r_data.s` headers (cf.
`splits.txt.bak_pre_nw4r`). Bucket: STANDALONE = EDS only; FUSED-RENAME = EDS +
rename/pool/align-only fields; FUSED-LAYOUT = EDS + drop/pad/trim/retarget/zero/text fields;
CONTENT-FUSED = EDS + byte-content field (needs category-C clearance); STRING-BUG = bare-string
field (inert; coordinator tooling fix).

Bucket counts: STANDALONE-live 7, STANDALONE-latent 22, FUSED-RENAME-live 18,
FUSED-RENAME-latent 159, FUSED-LAYOUT-live 17, FUSED-LAYOUT-latent 43, CONTENT-FUSED 6,
STRING-BUG 7. Live keys total 43 over 40 bases (live list in the decision doc §1.1).

| key | EDS strips | fused fields | live | source TU(s) | retail stripped sizes | label owners | bucket |
|---|---|---|---|---|---|---|---|
| CAIAction.o | .sdata2 | pool_patterns+ exact_renames | latent | kyoshin/cf/object/CAIAction.cpp | .sdata2=0x0 | GAPx7 | FUSED-RENAME-latent |
| CActParamAnim.o | .data+ .rodata+ .sdata+ .sdata2 | pool_patterns | latent | kyoshin/action/CActParamAnim.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| CActParamData.o | .data+ .rodata+ .sdata+ .sdata2 | pool_patterns | latent | kyoshin/action/CActParamData.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| CArtsInfo.o | .sdata2+ .data+ .rodata+ .sdata | pool_patterns+ exact_renames | latent | kyoshin/CArtsInfo.cpp | .sdata2=0x0 .data=0x0 .rodata=0x0 .sdata=0x0 | GAPx4 | FUSED-RENAME-latent |
| CArtsParam.o | .sdata2+ .bss | exact_renames | latent | kyoshin/cf/CArtsParam.cpp | .sdata2=0x0 .bss=0x0 | GAPx4 | FUSED-RENAME-latent |
| CAttrTransform.o | .data+ .sdata2 | pool_patterns+ exact_renames | latent | monolib/src/math/CAttrTransform.cpp | .data=0x0 .sdata2=0x0 | FloatUtils.cppx4,GAPx2 | FUSED-RENAME-latent |
| CCamUtil.o | .data+ .sdata2 | pool_patterns+ exact_renames | live | monolib/src/math/CCamUtil.cpp | .data=0x0 .sdata2=0x0 | FloatUtils.cppx6,GAPx2 | FUSED-RENAME-live |
| CChain.o | .sdata2+ .data | pool_patterns+ data_pool_patterns | latent | kyoshin/cf/chain/CChain.cpp | .sdata2=0x0 .data=0x0 | GAPx3 | FUSED-RENAME-latent |
| CChainActorList.o | .data+ .rodata+ .sdata+ .sbss | exact_renames | live | kyoshin/cf/chain/CChainActorList.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sbss=0x0 | GAPx4 | FUSED-RENAME-live |
| CChainEffect.o | .data+ .rodata+ .sdata | exact_renames | latent | kyoshin/cf/chain/CChainEffect.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | GAP | FUSED-RENAME-latent |
| CChainTimer.o | .sdata2+ .data+ .rodata+ .sdata | pool_patterns+ exact_renames | latent | kyoshin/cf/chain/CChainTimer.cpp | .sdata2=0x0 .data=0x0 .rodata=0x0 .sdata=0x0 | GAPx4 | FUSED-RENAME-latent |
| CCol6Invite.o | .sbss | - | latent | kyoshin/CCol6Invite.cpp | .sbss=0x0 | - | STANDALONE-latent |
| CCol6System.o | .data+ .sdata2+ .bss+ .sbss | exact_renames | latent | kyoshin/CCol6System.cpp | .data=0x0 .sdata2=0x0 .bss=0x0 .sbss=0x0 | GAPx2 | FUSED-RENAME-latent |
| CCollepedia.o | .sdata2+ .data+ .rodata+ .sdata | pool_patterns+ exact_renames | latent | kyoshin/CCollepedia.cpp | .sdata2=0x0 .data=0x0 .rodata=0x0 .sdata=0x0 | GAPx3 | FUSED-RENAME-latent |
| CCur.o | .sdata2 | pool_patterns | live | kyoshin/CCur.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-live |
| CDeviceFont.o | .sdata2 | pool_patterns | latent | monolib/src/device/CDeviceFont.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| CDeviceFontLayer.o | .sdata2 | pad_data_section+ pool_patterns | latent | monolib/src/device/CDeviceFontLayer.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-LAYOUT-latent |
| CDeviceGX.o | .sdata2 | drop_data_tail+ drop_data_range+ pool_patterns+ exact_renames+ trim_text_size+ drop_text_symbols+ drop_text_symbols_as_undef+ repack_after_drop | live | monolib/src/device/CDeviceGX.cpp | .sdata2=0x0 | CGXCache.cppx2,CDeviceVICb.cpp,CDeviceRemotePad.cpp | FUSED-LAYOUT-live |
| CDrawGX.o | STR-BUG:.sdata2 | retarget_relocs+ pool_patterns | latent | monolib/src/core/CDrawGX.cpp | STR-BUG:.sdata2=0x0 | CGXCache.cppx5,CDrawGX.cppx2 | STRING-BUG |
| CERand.o | .sdata2 | drop_data_tail+ retarget_relocs+ pool_patterns | live | monolib/src/effect/CERand.cpp | .sdata2=0x0 | CGXCache.cppx2,CERand.cpp | FUSED-LAYOUT-live |
| CETrail.o | .sdata2 | set_data_align+ pool_patterns | latent | monolib/src/effect/CETrail.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| CEquipChange.o | .data | exact_renames | latent | kyoshin/CEquipChange.cpp | .data=0x0 | GAPx4 | FUSED-RENAME-latent |
| CEquipItemBox.o | .sdata2+ .data | pool_patterns+ exact_renames | latent | kyoshin/CEquipItemBox.cpp | .sdata2=0x0 .data=0x0 | GAPx6 | FUSED-RENAME-latent |
| CExchangeWin.o | .sdata2 | pool_patterns | latent | kyoshin/CExchangeWin.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CFloorMap.o | .sdata2+ .data | pool_patterns+ data_pool_patterns | latent | kyoshin/CFloorMap.cpp | .sdata2=0x0 .data=0x0 | GAPx2 | FUSED-RENAME-latent |
| CItem.o | .data+ .bss | exact_renames | latent | kyoshin/cf/CItem.cpp | .data=0x0 .bss=0x0 | GAPx2 | FUSED-RENAME-latent |
| CItemBoxGrid.o | .data+ .sdata2 | retarget_relocs+ pool_patterns+ exact_renames | latent | kyoshin/CItemBoxGrid.cpp | .data=0x0 .sdata2=0x0 | GAPx11 | FUSED-LAYOUT-latent |
| CItemBoxLine.o | .data+ .sdata2 | retarget_relocs+ pool_patterns | latent | kyoshin/CItemBoxLine.cpp | .data=0x0 .sdata2=0x0 | GAPx5 | FUSED-LAYOUT-latent |
| CKizunagram.o | .sdata2+ .data+ .rodata+ .sdata | pool_patterns+ exact_renames | latent | kyoshin/CKizunagram.cpp | .sdata2=0x0 .data=0x0 .rodata=0x0 .sdata=0x0 | GAPx2 | FUSED-RENAME-latent |
| CLODCacheManagerS.o | .sdata2 | pool_patterns | latent | monolib/src/lod/CLODCacheManagerS.cpp | .sdata2=0x0 | CGXCache.cppx2 | FUSED-RENAME-latent |
| CLib.o | .rodata | exact_renames+ drop_text_symbols | live | monolib/src/lib/CLib.cpp | .rodata=0x0 | CException.cpp | FUSED-LAYOUT-live |
| CLibCriStreamingPlay.o | .sdata2 | pool_patterns | live | monolib/src/lib/CLibCriStreamingPlay.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-live |
| CLibHbm.o | .sdata2 | pad_data_section+ pool_patterns | live | monolib/src/lib/CLibHbm.cpp | .sdata2=0x0 | CGXCache.cppx11 | FUSED-LAYOUT-live |
| CLight.o | .sdata2 | pool_patterns+ exact_renames | latent | monolib/src/scn/CLight.cpp | .sdata2=0x0 | CGXCache.cppx2 | FUSED-RENAME-latent |
| CMCCrystalList.o | .data+ .sdata2 | data_pool_patterns | latent | kyoshin/makecrystal/CMCCrystalList.cpp | .data=0x0 .sdata2=0x0 | GAPx3 | FUSED-RENAME-latent |
| CMCCrystalSupport.o | .sdata2 | pool_patterns | latent | kyoshin/makecrystal/CMCCrystalSupport.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CMCCylinderGauge.o | .sdata2+ .rodata | pool_patterns+ exact_renames | live | kyoshin/makecrystal/CMCCylinderGauge.cpp | .sdata2=0x0 .rodata=0x0 | GAPx7 | FUSED-RENAME-live |
| CMCGetItemBox.o | .sdata2 | pool_patterns | latent | kyoshin/makecrystal/CMCGetItemBox.cpp | .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| CMainMenu.o | .data+ .sdata2 | pool_patterns+ exact_renames | latent | kyoshin/CMainMenu.cpp | .data=0x0 .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| CMapSel.o | .sdata2+ .data | pool_patterns+ exact_renames | latent | kyoshin/CMapSel.cpp | .sdata2=0x0 .data=0x0 | GAPx2 | FUSED-RENAME-latent |
| CMat34.o | .data+ .sdata2 | pool_patterns+ exact_renames | latent | monolib/src/math/CMat34.cpp | .data=0x0 .sdata2=0x0 | FloatUtils.cppx8,GAPx6 | FUSED-RENAME-latent |
| CMat44.o | .sdata2 | pool_patterns | latent | monolib/src/math/CMat44.cpp | .sdata2=0x0 | CGXCache.cppx2 | FUSED-RENAME-latent |
| CMdlDynamics.o | .sdata2 | set_data_align+ pool_patterns | latent | monolib/src/scn/CMdlDynamics.cpp | .sdata2=0x0 | CGXCache.cppx2 | FUSED-RENAME-latent |
| CMdlMouth.o | .sdata2 | drop_data_range+ set_data_align+ pool_patterns | live | monolib/src/scn/CMdlMouth.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-LAYOUT-live |
| CMenuArtsSelect.o | .data+ .rodata+ .sdata+ .sdata2 | pool_patterns+ exact_renames | latent | kyoshin/menu/CMenuArtsSelect.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAPx12 | FUSED-RENAME-latent |
| CMenuArtsSet.o | .data+ .sdata2 | pool_patterns+ exact_renames | latent | kyoshin/menu/CMenuArtsSet.cpp | .data=0x0 .sdata2=0x0 | GAPx5 | FUSED-RENAME-latent |
| CMenuBattleChain.o | .data | exact_renames | live | kyoshin/menu/CMenuBattleChain.cpp | .data=0x0 | GAPx2 | FUSED-RENAME-live |
| CMenuBattleDamage.o | .data+ .rodata+ .sdata2 | pool_patterns+ exact_renames | latent | kyoshin/menu/CMenuBattleDamage.cpp | .data=0x0 .rodata=0x0 .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| CMenuBattleMode.o | .data+ .rodata+ .sdata+ .sdata2 | - | latent | kyoshin/menu/CMenuBattleMode.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | - | STANDALONE-latent |
| CMenuBattlePlayerState.o | .data+ .rodata+ .sdata+ .sdata2+ .bss | pool_patterns | latent | kyoshin/menu/CMenuBattlePlayerState.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 .bss=0x0 | GAPx3 | FUSED-RENAME-latent |
| CMenuBattlePlayerState_ct.o | .data+ .rodata+ .sdata+ .sdata2 | exact_renames | latent | kyoshin/menu/CMenuBattlePlayerState_ct.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CMenuCollepedia.o | .data+ .rodata+ .sdata | exact_renames | latent | kyoshin/menu/CMenuCollepedia.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | FUSED-RENAME-latent |
| CMenuFade.o | .data+ .rodata+ .sdata+ .sdata2 | pool_patterns+ exact_renames | latent | kyoshin/menu/CMenuFade.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAPx5 | FUSED-RENAME-latent |
| CMenuGetItemMulti.o | STR-BUG:.sdata2 | retarget_relocs+ pool_patterns | latent | kyoshin/menu/CMenuGetItemMulti.cpp | STR-BUG:.sdata2=0x0 | GAPx8 | STRING-BUG |
| CMenuItemExchange.o | .data | retarget_relocs | latent | kyoshin/menu/CMenuItemExchange.cpp | .data=0x0 | GAP | FUSED-LAYOUT-latent |
| CMenuKizunaTalk.o | .sdata2 | - | latent | kyoshin/menu/CMenuKizunaTalk.cpp | .sdata2=0x0 | - | STANDALONE-latent |
| CMenuKizunaTalkList.o | .data+ .rodata+ .sdata | exact_renames | latent | kyoshin/menu/CMenuKizunaTalkList.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | FUSED-RENAME-latent |
| CMenuKizunagram.o | .data | data_pool_patterns | latent | kyoshin/menu/CMenuKizunagram.cpp | .data=0x0 | GAP | FUSED-RENAME-latent |
| CMenuLandTelop.o | .sdata2 | exact_renames | latent | kyoshin/menu/CMenuLandTelop.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CMenuMapSelectSC.o | .data+ .sbss | exact_renames | latent | kyoshin/menu/CMenuMapSelectSC.cpp | .data=0x0 .sbss=0x0 | GAP | FUSED-RENAME-latent |
| CMenuPTChangeNotice.o | .sdata2 | pool_patterns | latent | kyoshin/menu/CMenuPTChangeNotice.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CMenuPTGauge.o | .sdata2 | exact_renames | latent | kyoshin/menu/CMenuPTGauge.cpp | .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| CMenuPTState.o | .sdata2 | retarget_relocs+ pool_patterns | live | kyoshin/menu/CMenuPTState.cpp | .sdata2=0x0 | GAPx3 | FUSED-LAYOUT-live |
| CMenuPassiveSkill.o | .data+ .rodata+ .sdata | - | latent | kyoshin/menu/CMenuPassiveSkill.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-latent |
| CMenuPlayAward.o | .sdata2 | pool_patterns | latent | kyoshin/menu/CMenuPlayAward.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CMenuQstCnt.o | .sdata2+ .sbss | pool_patterns | latent | kyoshin/menu/CMenuQstCnt.cpp | .sdata2=0x0 .sbss=0x0 | CGXCache.cppx2 | FUSED-RENAME-latent |
| CMenuQuestLog.o | .data+ .rodata+ .sdata | exact_renames | live | kyoshin/menu/CMenuQuestLog.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | GAP | FUSED-RENAME-live |
| CMenuSkipTimer.o | .data+ .rodata+ .sdata+ .sdata2 | retarget_relocs | latent | kyoshin/menu/CMenuSkipTimer.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | CMainMenu.cpp | FUSED-LAYOUT-latent |
| CMenuTutorial.o | .data+ .rodata+ .sdata+ .sdata2 | exact_renames | latent | kyoshin/menu/CMenuTutorial.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | - | FUSED-RENAME-latent |
| CMenuUpdate.o | .sdata2 | exact_renames | latent | kyoshin/menu/CMenuUpdate.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CMiniMap.o | .data+ .rodata+ .sdata+ .sdata2 | pool_patterns | latent | kyoshin/CMiniMap.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAPx3 | FUSED-RENAME-latent |
| CModelDisp.o | .sdata2 | pool_patterns | latent | kyoshin/CModelDisp.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CNandData.o | .bss | exact_renames | latent | kyoshin/CNandData.cpp | .bss=0x0 | GAPx2 | FUSED-RENAME-latent |
| CNumSelect.o | .sdata2 | pool_patterns | latent | kyoshin/CNumSelect.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| COccCulling.o | .rodata+ .data+ .sdata+ .sdata2 | zero_nobits+ pool_patterns+ exact_renames | latent | kyoshin/COccCulling.cpp | .rodata=0x0 .data=0x0 .sdata=0x0 .sdata2=0x0 | GAPx3 | FUSED-LAYOUT-latent |
| CPartsChange.o | STR-BUG:.sdata2 | retarget_relocs+ pool_patterns | latent | kyoshin/cf/CPartsChange.cpp | STR-BUG:.sdata2=0x0 | GAPx3 | STRING-BUG |
| CPartyState.o | .sdata2 | pool_patterns | latent | kyoshin/CPartyState.cpp | .sdata2=0x0 | GAPx3 | FUSED-RENAME-latent |
| CPartyStateWin.o | .data | data_pool_patterns | latent | kyoshin/CPartyStateWin.cpp | .data=0x0 | GAP | FUSED-RENAME-latent |
| CPassiveSkill.o | .sbss2 | pool_patterns+ exact_renames | latent | kyoshin/CPassiveSkill.cpp | .sbss2=0x0 | GAPx2 | FUSED-RENAME-latent |
| CPcEffect07.o | .sdata2 | pool_patterns | latent | kyoshin/cf/CPcEffect07.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CPcKizunagram.o | .sdata2 | pool_patterns | latent | kyoshin/CPcKizunagram.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CPlane.o | .sdata2 | pool_patterns | latent | monolib/src/math/CPlane.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| CProc.o | .sdata2 | pad_data_section+ pool_patterns+ exact_renames+ trim_text_size+ swap_data_blocks+ inject_relocs | live | monolib/src/core/CProc.cpp | .sdata2=0x0 | CProc.cppx4,FloatUtils.cpp | CONTENT-FUSED |
| CQstLogInfo.o | .data | exact_renames | latent | kyoshin/CQstLogInfo.cpp | .data=0x0 | GAPx2 | FUSED-RENAME-latent |
| CQuat.o | .sdata2 | pool_patterns | latent | monolib/src/math/CQuat.cpp | .sdata2=0x0 | CGXCache.cppx3 | FUSED-RENAME-latent |
| CQuestWindow.o | .data+ .sdata2+ .sbss | data_pool_patterns+ exact_renames | latent | kyoshin/CQuestWindow.cpp | .data=0x0 .sdata2=0x0 .sbss=0x0 | GAPx3 | FUSED-RENAME-latent |
| CREvtEffect.o | .sdata2 | exact_renames | latent | kyoshin/realtimeevt/CREvtEffect.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CREvtModelObj.o | .sdata2 | exact_renames | latent | kyoshin/realtimeevt/CREvtModelObj.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CREvtModelPc.o | .sdata2 | exact_renames | latent | kyoshin/realtimeevt/CREvtModelPc.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CScnBloom.o | .sdata2 | pool_patterns | latent | monolib/src/scn/CScnBloom.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| CScnCameraMan.o | .sdata2 | pool_patterns | latent | monolib/src/scn/CScnCameraMan.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| CScnEnvLgtCtrl.o | .sdata2 | pool_patterns | latent | monolib/src/scn/CScnEnvLgtCtrl.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| CScnFilter.o | .sdata2 | set_data_align+ pool_patterns+ exact_renames | live | monolib/src/scn/CScnFilter.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-live |
| CScnFogMan.o | .sdata2 | pool_patterns | live | monolib/src/scn/CScnFogMan.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-live |
| CScnFrame.o | .sdata2 | pool_patterns | latent | monolib/src/scn/CScnFrame.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| CScnItemCamera.o | .sdata2 | pool_patterns | latent | monolib/src/scn/CScnItemCamera.cpp | .sdata2=0x0 | CGXCache.cppx2 | FUSED-RENAME-latent |
| CScnItemPool.o | .sdata | retarget_relocs+ pad_data_section+ data_pool_patterns+ exact_renames+ add_symbols | latent | monolib/src/scn/CScnItemPool.cpp | .sdata=0x0 | CScnItemPool.cppx4,CScnItemCamera.cppx2 | FUSED-LAYOUT-latent |
| CScnMaruShadowNw4r.o | .sdata2 | pool_patterns | latent | monolib/src/scn/CScnMaruShadowNw4r.cpp | .sdata2=0x0 | CGXCache.cppx4 | FUSED-RENAME-latent |
| CScn_80496B0C.o | .sdata2 | zero_data_range+ pad_data_section+ pool_patterns+ exact_renames+ drop_text_symbols_as_undef+ inject_relocs | latent | monolib/src/scn/CScn_80496B0C.cpp | .sdata2=0x0 | CGXCache.cppx2 | CONTENT-FUSED |
| CScrollBar.o | .sdata2 | retarget_relocs | live | kyoshin/CScrollBar.cpp | .sdata2=0x0 | GAPx2 | FUSED-LAYOUT-live |
| CSelShopWin.o | .sdata2 | pool_patterns | latent | kyoshin/CSelShopWin.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CSortMenu.o | .sdata2 | retarget_relocs+ pool_patterns | latent | kyoshin/CSortMenu.cpp | .sdata2=0x0 | GAPx2 | FUSED-LAYOUT-latent |
| CSuddenCommu.o | .data+ .rodata+ .sdata+ .sdata2 | pool_patterns | latent | kyoshin/cf/CSuddenCommu.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CSysWin.o | .sdata2 | pool_patterns | latent | kyoshin/CSysWin.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CTalkWindow.o | .data+ .sdata2 | exact_renames | latent | kyoshin/CTalkWindow.cpp | .data=0x0 .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| CTaskCulling.o | .rodata+ .data+ .sdata+ .sdata2+ .bss+ .sbss | pool_patterns+ exact_renames+ drop_text_symbols | latent | kyoshin/cf/CTaskCulling.cpp | .rodata=0x0 .data=0x0 .sdata=0x0 .sdata2=0x0 .bss=0x0 .sbss=0x0 | GAPx9 | FUSED-LAYOUT-latent |
| CTaskGame.o | .data+ .rodata+ .sdata+ .sdata2+ .sbss | pool_patterns+ exact_renames | latent | kyoshin/CTaskGame.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 .sbss=0x0 | GAPx8 | FUSED-RENAME-latent |
| CTaskGameCf.o | .data+ .rodata+ .sdata+ .sdata2+ .sbss | pool_patterns+ exact_renames | latent | kyoshin/cf/CTaskGameCf.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 .sbss=0x0 | GAPx14 | FUSED-RENAME-latent |
| CTaskGameEff.o | .data+ .rodata+ .sdata+ .sdata2 | - | latent | kyoshin/CTaskGameEff.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | - | STANDALONE-latent |
| CTaskGameEffAfter.o | .data+ .rodata+ .sdata | - | live | kyoshin/cf/CTaskGameEffAfter.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-live |
| CTaskGameEvt.o | .data+ .rodata+ .sdata | - | latent | kyoshin/CTaskGameEvt.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-latent |
| CTaskGamePic.o | .data+ .rodata+ .sdata | - | live | kyoshin/CTaskGamePic.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-live |
| CTaskLOD.o | .sdata2 | retarget_relocs_local+ pool_patterns | live | monolib/src/lod/CTaskLOD.cpp | .sdata2=0x0 | CGXCache.cppx2 | FUSED-LAYOUT-live |
| CTaskREvent.o | .sdata2+ .sbss | pool_patterns | latent | kyoshin/cf/CTaskREvent.cpp | .sdata2=0x0 .sbss=0x0 | GAP | FUSED-RENAME-latent |
| CTaskREvtSequence.o | .sdata2 | exact_renames | latent | kyoshin/cf/CTaskREvtSequence.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CTitleAHelp.o | .data+ .rodata+ .sdata+ .sdata2 | retarget_relocs+ pool_patterns+ exact_renames | latent | kyoshin/CTitleAHelp.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAPx3 | FUSED-LAYOUT-latent |
| CTutorial.o | .data+ .rodata+ .sdata | - | latent | kyoshin/CTutorial.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-latent |
| CUIBattleManager.o | .data+ .rodata+ .sdata+ .bss+ .sbss | exact_renames | latent | kyoshin/CUIBattleManager.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .bss=0x0 .sbss=0x0 | - | FUSED-RENAME-latent |
| CUICfManager.o | .data+ .rodata+ .sdata+ .bss+ .sbss | retarget_relocs+ exact_renames | latent | kyoshin/CUICfManager.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .bss=0x0 .sbss=0x0 | GAPx2 | FUSED-LAYOUT-latent |
| CUIWindowManager.o | .data+ .rodata+ .sdata+ .sbss | exact_renames | latent | kyoshin/CUIWindowManager.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sbss=0x0 | GAPx4 | FUSED-RENAME-latent |
| CVS_THREAD.o | .data+ .rodata+ .sdata | exact_renames | latent | kyoshin/cf/voice/cvsys/CVS_THREAD.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | GAP | FUSED-RENAME-latent |
| CVS_THREAD_BATTLE_MAIN.o | .sdata2 | exact_renames | latent | kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_MAIN.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CVS_THREAD_BUF.o | .data+ .rodata+ .sdata | - | latent | kyoshin/cf/voice/cvsys/CVS_THREAD_BUF.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-latent |
| CVS_THREAD_HAGE.o | .data+ .rodata+ .sdata | - | latent | kyoshin/cf/voice/cvsys/CVS_THREAD_HAGE.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-latent |
| CVS_THREAD_HP.o | .data+ .rodata+ .sdata+ .sdata2 | exact_renames | latent | kyoshin/cf/voice/cvsys/CVS_THREAD_HP.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CVS_THREAD_TENSION_UP.o | .data+ .rodata+ .sdata | - | latent | kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-latent |
| CVS_THREAD_VISION_TELL.o | .data+ .rodata+ .sdata | - | latent | kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_TELL.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-latent |
| CVirtualLightObj.o | .sdata2 | pool_patterns | latent | monolib/src/scn/CVirtualLightObj.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| CVision.o | .data+ .rodata+ .sdata+ .sdata2 | pool_patterns | live | kyoshin/cf/CVision.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAPx2 | FUSED-RENAME-live |
| CWorkSystemCache.o | .sdata2 | - | live | monolib/src/work/CWorkSystemCache.cpp | .sdata2=0x0 | - | STANDALONE-live |
| CfCamEvent.o | .bss | - | latent | kyoshin/cf/CfCamEvent.cpp | .bss=0x0 | - | STANDALONE-latent |
| CfObjectEff.o | .data+ .rodata+ .sdata | drop_data_range+ exact_renames | latent | kyoshin/cf/object/CfObjectEff.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | GAP | FUSED-LAYOUT-latent |
| CfObjectImplEne.o | .data+ .rodata+ .sdata | - | latent | kyoshin/cfsys/CfObjectImplEne.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-latent |
| CfObjectImplMove.o | .data+ .sdata2 | pool_patterns+ exact_renames | latent | kyoshin/cfsys/CfObjectImplMove.cpp | .data=0x0 .sdata2=0x0 | GAPx3 | FUSED-RENAME-latent |
| CfObjectImplWalker.o | .sdata2 | pool_patterns | latent | kyoshin/cfsys/CfObjectImplWalker.cpp | .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| CfObjectNpc.o | .data | drop_data_tail | latent | kyoshin/cf/object/CfObjectNpc.cpp | .data=0x0 | - | FUSED-LAYOUT-latent |
| CfObjectTbox.o | .data+ .rodata+ .sdata | exact_renames | latent | kyoshin/cf/object/CfObjectTbox.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | GAP | FUSED-RENAME-latent |
| CfResReloadImpl.o | .sdata+ .sdata2+ .sbss | - | latent | kyoshin/cf/CfResReloadImpl.cpp | .sdata=0x0 .sdata2=0x0 .sbss=0x0 | - | STANDALONE-latent |
| CfSoundMan.o | .sdata2 | pool_patterns | latent | kyoshin/cf/CfSoundMan.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CfTFile.o | .sdata2 | pool_patterns | latent | kyoshin/cf/CfTFile.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CfTaskMain.o | .data+ .rodata+ .sdata | - | latent | kyoshin/cf/CfTaskMain.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-latent |
| CtrlAct.o | .data+ .sdata2 | retarget_relocs+ pool_patterns+ exact_renames | latent | kyoshin/cf/CtrlAct.cpp | .data=0x0 .sdata2=0x0 | GAPx3 | FUSED-LAYOUT-latent |
| CtrlEnemy.o | .data+ .rodata+ .sdata+ .sdata2 | pool_patterns | latent | kyoshin/cf/CtrlEnemy.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CtrlMoveBase.o | .sdata2 | pool_patterns | latent | kyoshin/cf/CtrlMoveBase.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| CtrlMoveEne.o | .sdata2 | pool_patterns | latent | kyoshin/cf/CtrlMoveEne.cpp | .sdata2=0x0 | GAPx3 | FUSED-RENAME-latent |
| CtrlMovePC.o | .sdata2 | pool_patterns | latent | kyoshin/cf/CtrlMovePC.cpp | .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| CtrlRemote.o | .sdata2 | pool_patterns | latent | kyoshin/cf/CtrlRemote.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| LODMemMan.o | .sdata2 | retarget_relocs+ pool_patterns | latent | monolib/src/lod/LODMemMan.cpp | .sdata2=0x0 | LODMemMan.cppx2,CGXCache.cpp,CGame.cpp | FUSED-LAYOUT-latent |
| MPFDrawDisplayList.o | .sdata2 | pool_patterns | latent | monolib/src/mpfsys/MPFDrawDisplayList.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| MTRand.o | .sdata2 | pool_patterns | live | monolib/src/math/MTRand.cpp | .sdata2=0x0 | FloatUtils.cpp | FUSED-RENAME-live |
| MemManager.o | .sdata2 | pool_patterns+ exact_renames+ drop_text_symbols | latent | monolib/src/util/MemManager.cpp | .sdata2=0x0 | FloatUtils.cpp | FUSED-LAYOUT-latent |
| UnkClass_80466348.o | .sdata2 | pool_patterns | live | monolib/src/lod/UnkClass_80466348.cpp | .sdata2=0x0 | CGXCache.cppx2 | FUSED-RENAME-live |
| UnkClass_80471EC8.o | .sdata2 | pool_patterns | latent | monolib/src/mpfsys/UnkClass_80471EC8.cpp | .sdata2=0x0 | CGXCache.cppx5 | FUSED-RENAME-latent |
| UnkClass_8047E110.o | .sdata2 | pool_patterns | latent | monolib/src/scn/UnkClass_8047E110.cpp | .sdata2=0x0 | CGXCache.cppx2 | FUSED-RENAME-latent |
| adx_dcd.o | .rodata | exact_renames | latent | CriWare/src/adx/adxt/adx_dcd.c | .rodata=0x0 | GAP | FUSED-RENAME-latent |
| ahx_sbf.o | .rodata | - | latent | CriWare/src/adx/ahx/ahx_sbf.c | .rodata=0x0 | - | STANDALONE-latent |
| bta_hh_utils.o | .rodata | - | live | RVL_SDK/src/revolution/bte/bta/hh/bta_hh_utils.c | .rodata=0x0 | - | STANDALONE-live |
| btm_acl.o | .sdata2 | - | latent | RVL_SDK/src/revolution/bte/stack/btm/btm_acl.c | .sdata2=0x0 | - | STANDALONE-latent |
| btm_sec.o | .rodata | pad_sdata2_size | latent | RVL_SDK/src/revolution/bte/stack/btm/btm_sec.c | .rodata=0x0 | - | CONTENT-FUSED |
| btu_hcif.o | .sdata2 | pad_data_section+ exact_renames | live | RVL_SDK/src/revolution/bte/stack/btu/btu_hcif.c | .sdata2=0x0 | - | FUSED-LAYOUT-live |
| code_800A3B24.o | .sdata2 | pool_patterns | latent | kyoshin/code_800A3B24.cpp | .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| code_800A75FC.o | .sdata2 | - | live | kyoshin/code_800A75FC.cpp | .sdata2=0x0 | - | STANDALONE-live |
| code_800AA008.o | .data | exact_renames | latent | kyoshin/code_800AA008.cpp | .data=0x0 | GAPx2 | FUSED-RENAME-latent |
| code_800B06A4.o | .data+ .sdata2 | pool_patterns+ exact_renames | latent | kyoshin/code_800B06A4.cpp | .data=0x0 .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| code_800F42AC.o | .sdata2 | pool_patterns | latent | kyoshin/cf/code_800F42AC.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| code_801862C0.o | .sdata2+ .bss | exact_renames | latent | kyoshin/code_801862C0.cpp | .sdata2=0x0 .bss=0x0 | GAPx2 | FUSED-RENAME-latent |
| code_80187F14.o | .sdata2 | pool_patterns | latent | kyoshin/menu/code_80187F14.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| code_8018F8D8.o | .data+ .rodata+ .sdata+ .sdata2 | pool_patterns | latent | kyoshin/cf/code_8018F8D8.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| code_801C2C14.o | .data+ .rodata+ .sdata | exact_renames | latent | kyoshin/cf/code_801C2C14.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | GAPx3 | FUSED-RENAME-latent |
| code_8025FB10.o | .sdata2+ .sbss | pool_patterns | latent | kyoshin/code_8025FB10.cpp | .sdata2=0x0 .sbss=0x0 | GAPx2 | FUSED-RENAME-latent |
| code_8027513C.o | .data+ .rodata+ .sdata | - | latent | kyoshin/code_8027513C.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 | - | STANDALONE-latent |
| code_802B9064.o | .sdata2 | pool_patterns | live | kyoshin/code_802B9064.cpp | .sdata2=0x0 | GAPx2 | FUSED-RENAME-live |
| code_804645CC.o | .sdata2 | pool_patterns | latent | monolib/src/lod/code_804645CC.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| code_8046A530.o | .sdata2 | pool_patterns | latent | monolib/src/lod/code_8046A530.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| code_8047D2AC.o | .sdata2 | pool_patterns | latent | monolib/src/scn/code_8047D2AC.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| code_804B59C8.o | .sdata2 | pool_patterns | latent | monolib/src/coli/code_804B59C8.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| code_804BD8E8.o | .sdata2 | set_data_align+ pool_patterns+ exact_renames | latent | monolib/src/scn/code_804BD8E8.cpp | .sdata2=0x0 | CGXCache.cppx4 | FUSED-RENAME-latent |
| code_804C8718.o | .sdata2 | pool_patterns | latent | monolib/src/effect/code_804C8718.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| code_804CC2B8.o | .sdata2 | pool_patterns | latent | monolib/src/effect/code_804CC2B8.cpp | .sdata2=0x0 | CGXCache.cppx2 | FUSED-RENAME-latent |
| code_804D854C.o | .sdata2 | pool_patterns | latent | monolib/src/effect/code_804D854C.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| code_804D9274.o | .sdata2 | pool_patterns | latent | monolib/src/effect/code_804D9274.cpp | .sdata2=0x0 | CGXCache.cppx5 | FUSED-RENAME-latent |
| code_804DB938.o | .sdata2 | pool_patterns | latent | monolib/src/effect/code_804DB938.cpp | .sdata2=0x0 | CGXCache.cpp | FUSED-RENAME-latent |
| code_804DEDA8.o | .sdata2 | pool_patterns | latent | monolib/src/core/code_804DEDA8.cpp | .sdata2=0x0 | CGXCache.cppx2 | FUSED-RENAME-latent |
| code_804E36DC.o | .sdata2 | pool_patterns | latent | monolib/src/core/code_804E36DC.cpp | .sdata2=0x0 | CGXCache.cppx4 | FUSED-RENAME-latent |
| code_804F0258.o | .sdata2 | retarget_relocs+ pool_patterns+ exact_renames | latent | monolib/src/core/code_804F0258.cpp | .sdata2=0x0 | CGXCache.cppx4 | FUSED-LAYOUT-latent |
| g3d_anmchr.o | STR-BUG:.rodata | exact_renames | latent | nw4r/src/g3d/g3d_anmchr.cpp | STR-BUG:.rodata=0x0 | GAPx3 | STRING-BUG |
| g3d_anmclr.o | .sdata2 | pool_patterns+ exact_renames | latent | nw4r/src/g3d/g3d_anmclr.cpp | .sdata2=0x0 | GAPx3 | FUSED-RENAME-latent |
| g3d_anmscn.o | .sdata2 | pool_patterns+ exact_renames | latent | nw4r/src/g3d/g3d_anmscn.cpp | .sdata2=0x0 | GAPx6 | FUSED-RENAME-latent |
| g3d_anmtexpat.o | .sdata2 | pool_patterns+ exact_renames | latent | nw4r/src/g3d/g3d_anmtexpat.cpp | .sdata2=0x0 | GAPx4 | FUSED-RENAME-latent |
| g3d_anmtexsrt.o | STR-BUG:.sdata2 | pool_patterns+ exact_renames | latent | nw4r/src/g3d/g3d_anmtexsrt.cpp | STR-BUG:.sdata2=0x0 | GAPx6 | STRING-BUG |
| g3d_calcview.o | .sdata2 | pool_patterns+ exact_renames | latent | nw4r/src/g3d/g3d_calcview.cpp | .sdata2=0x0 | GAPx3 | FUSED-RENAME-latent |
| g3d_calcvtx.o | .sdata2 | pool_patterns | latent | nw4r/src/g3d/g3d_calcvtx.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| g3d_camera.o | .sdata2 | pool_patterns | latent | nw4r/src/g3d/g3d_camera.cpp | .sdata2=0x0 | GAPx11 | FUSED-RENAME-latent |
| g3d_draw.o | .sdata2+ .sbss | pool_patterns | latent | nw4r/src/g3d/g3d_draw.cpp | .sdata2=0x0 .sbss=0x0 | GAP | FUSED-RENAME-latent |
| g3d_draw1mat1shp.o | .sdata2 | pool_patterns+ prefix_renames | latent | nw4r/src/g3d/g3d_draw1mat1shp.cpp | .sdata2=0x0 | GAPx3 | FUSED-RENAME-latent |
| g3d_gpu.o | .sdata2 | - | latent | nw4r/src/g3d/platform/g3d_gpu.cpp | .sdata2=0x0 | - | STANDALONE-latent |
| g3d_light.o | .sdata2 | pool_patterns+ exact_renames | latent | nw4r/src/g3d/g3d_light.cpp | .sdata2=0x0 | GAPx13 | FUSED-RENAME-latent |
| g3d_resanmamblight.o | .sdata2 | retarget_relocs | latent | nw4r/src/g3d/res/g3d_resanmamblight.cpp | .sdata2=0x0 | GAPx2 | FUSED-LAYOUT-latent |
| g3d_resanmchr.o | .sdata2 | pool_patterns+ exact_renames | live | nw4r/src/g3d/res/g3d_resanmchr.cpp | .sdata2=0x0 | GAPx8 | FUSED-RENAME-live |
| g3d_resanmclr.o | .sdata2 | pool_patterns | latent | nw4r/src/g3d/res/g3d_resanmclr.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| g3d_resanmfog.o | .sdata2 | pool_patterns | live | nw4r/src/g3d/res/g3d_resanmfog.cpp | .sdata2=0x0 | GAPx2 | FUSED-RENAME-live |
| g3d_resanmlight.o | .sdata2 | retarget_relocs | latent | nw4r/src/g3d/res/g3d_resanmlight.cpp | .sdata2=0x0 | GAPx2 | FUSED-LAYOUT-latent |
| g3d_resnode.o | .sdata2 | pool_patterns | latent | nw4r/src/g3d/res/g3d_resnode.cpp | .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| g3d_restev.o | STR-BUG:.sdata2 | exact_renames | latent | nw4r/src/g3d/res/g3d_restev.cpp | STR-BUG:.sdata2=0x0 | GAPx3 | STRING-BUG |
| g3d_scnmdl.o | STR-BUG:.rodata | exact_renames | latent | nw4r/src/g3d/g3d_scnmdl.cpp | STR-BUG:.rodata=0x0 | GAP | STRING-BUG |
| g3d_scnmdlexpand.o | .rodata | exact_renames | latent | nw4r/src/g3d/g3d_scnmdlexpand.cpp | .rodata=0x0 | GAP | FUSED-RENAME-latent |
| g3d_scnmdlsmpl.o | .rodata+ .sdata2 | zero_data_range+ drop_nobits_range+ pad_data_section+ pool_patterns+ data_pool_patterns+ exact_renames | latent | nw4r/src/g3d/g3d_scnmdlsmpl.cpp | .rodata=0x0 .sdata2=0x0 | GAPx6 | FUSED-LAYOUT-latent |
| g3d_scnobj.o | .sdata2 | retarget_relocs+ pad_data_section+ drop_text_symbols | live | nw4r/src/g3d/g3d_scnobj.cpp | .sdata2=0x0 | GAPx6 | FUSED-LAYOUT-live |
| g3d_scnproc.o | .sdata2 | pool_patterns+ drop_text_symbols | latent | nw4r/src/g3d/g3d_scnproc.cpp | .sdata2=0x0 | GAP | FUSED-LAYOUT-latent |
| g3d_scnroot.o | .data+ .rodata | exact_renames | latent | nw4r/src/g3d/g3d_scnroot.cpp | .data=0x0 .rodata=0x0 | GAPx2 | FUSED-RENAME-latent |
| g3d_state.o | .sdata2 | retarget_relocs+ drop_nobits_range+ pad_data_section+ pool_patterns+ exact_renames+ inject_relocs | latent | nw4r/src/g3d/g3d_state.cpp | .sdata2=0x0 | GAPx3 | CONTENT-FUSED |
| lyt_animation.o | .sdata2 | drop_data_range+ pool_patterns+ exact_renames+ drop_text_symbols | live | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_animation.cpp;nw4r/src/lyt/lyt_animation.cpp | .sdata2=0x0 | lyt_animation.cppx5,GAPx2 | FUSED-LAYOUT-live |
| lyt_animation.o#Q34nw4r3lyt | .sdata2 | drop_data_range+ pool_patterns+ exact_renames+ drop_text_symbols | live | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_animation.cpp;nw4r/src/lyt/lyt_animation.cpp | .sdata2=0x0 | GAPx4 | FUSED-LAYOUT-live |
| lyt_arcResourceAccessor.o | .sdata | data_pool_patterns+ exact_renames+ drop_text_symbols+ repack_after_drop | latent | nw4r/src/lyt/lyt_arcResourceAccessor.cpp | .sdata=0x0 | GAPx2 | FUSED-LAYOUT-latent |
| lyt_common.o#Q34nw4r | .sdata+ .sdata2 | pad_data_section+ pool_patterns+ exact_renames+ drop_text_symbols | latent | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_common.cpp;nw4r/src/lyt/lyt_common.cpp | .sdata=0x0 .sdata2=0x0 | GAPx5 | FUSED-LAYOUT-latent |
| lyt_group.o#Q34nw4r3lyt | .data | exact_renames | live | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_group.cpp;nw4r/src/lyt/lyt_group.cpp | .data=0x28 | GAPx2 | FUSED-RENAME-live |
| lyt_init.o | .sdata2 | exact_renames | latent | nw4r/src/lyt/lyt_init.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| lyt_layout.o | .sdata2 | pool_patterns+ exact_renames+ drop_text_symbols+ repack_after_drop+ swap_data_blocks | latent | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_layout.cpp;nw4r/src/lyt/lyt_layout.cpp | .sdata2=0x0 | GAPx2,lyt_layout.cpp | CONTENT-FUSED |
| lyt_layout.o#Q34nw4r3lyt | .data+ .sdata+ .sdata2 | pad_data_section+ pool_patterns+ exact_renames | latent | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_layout.cpp;nw4r/src/lyt/lyt_layout.cpp | .data=0x50 .sdata=0x0 .sdata2=0x0 | GAPx4 | FUSED-LAYOUT-latent |
| lyt_material.o | .sdata2 | pool_patterns+ exact_renames | latent | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_material.cpp;nw4r/src/lyt/lyt_material.cpp | .sdata2=0x0 | GAPx5,lyt_material.cpp | FUSED-RENAME-latent |
| lyt_material.o#Q34nw4r3lyt | .sdata+ .sdata2+ .sbss2 | pool_patterns+ exact_renames | latent | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_material.cpp;nw4r/src/lyt/lyt_material.cpp | .sdata=0x0 .sdata2=0x0 .sbss2=0x0 | GAPx10 | FUSED-RENAME-latent |
| lyt_pane.o#Q34nw4r3lyt | .sdata2 | drop_data_range+ pad_data_section+ pool_patterns+ exact_renames+ drop_text_symbols | latent | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_pane.cpp;nw4r/src/lyt/lyt_pane.cpp | .sdata2=0x0 | GAPx8 | FUSED-LAYOUT-latent |
| lyt_pane.o#Q36nw4hbm3lyt | .sdata2 | drop_data_tail+ pool_patterns+ exact_renames+ drop_text_symbols+ repack_after_drop+ swap_data_blocks+ inject_relocs | latent | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_pane.cpp;nw4r/src/lyt/lyt_pane.cpp | .sdata2=0x0 | GAPx6,lyt_pane.cpp | CONTENT-FUSED |
| lyt_picture.o | .sdata2 | retarget_relocs+ pool_patterns+ exact_renames | live | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_picture.cpp;nw4r/src/lyt/lyt_picture.cpp | .sdata2=0x0 | lyt_picture.cppx2,lyt_bounding.cppx2,GAP | FUSED-LAYOUT-live |
| lyt_picture.o#Q34nw4r3lyt | .data+ .sdata2 | pad_data_section+ pool_patterns+ exact_renames | live | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_picture.cpp;nw4r/src/lyt/lyt_picture.cpp | .data=0xa0 .sdata2=0x0 | GAPx3 | FUSED-LAYOUT-live |
| lyt_texMap.o | .sdata2 | pool_patterns | latent | nw4r/src/lyt/lyt_texMap.cpp | .sdata2=0x0 | GAPx4 | FUSED-RENAME-latent |
| lyt_textBox.o | .sdata2 | retarget_relocs+ pool_patterns+ exact_renames+ drop_text_symbols | latent | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_textBox.cpp;nw4r/src/lyt/lyt_textBox.cpp | .sdata2=0x0 | GAPx3,lyt_bounding.cppx2,lyt_textBox.cppx2 | FUSED-LAYOUT-latent |
| lyt_textBox.o#Q34nw4r3lyt | .data+ .sbss+ .sdata2 | drop_data_range+ pool_patterns+ exact_renames | latent | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_textBox.cpp;nw4r/src/lyt/lyt_textBox.cpp | .data=0xb0 .sbss=0x0 .sdata2=0x0 | GAPx6 | FUSED-LAYOUT-latent |
| lyt_window.o | .sdata2 | drop_data_tail+ retarget_relocs+ pool_patterns+ exact_renames+ drop_text_symbols+ repack_after_drop | live | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_window.cpp;nw4r/src/lyt/lyt_window.cpp | .sdata2=0x0 | GAPx2,lyt_bounding.cppx2,lyt_window.cppx2 | FUSED-LAYOUT-live |
| lyt_window.o#Q34nw4r3lyt | .sdata2 | drop_data_tail+ pad_data_section+ pool_patterns+ exact_renames | live | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_window.cpp;nw4r/src/lyt/lyt_window.cpp | .sdata2=0x0 | GAPx6 | FUSED-LAYOUT-live |
| math_arithmetic.o | .sdata2 | retarget_relocs | latent | nw4r/src/math/math_arithmetic.cpp | .sdata2=0x0 | GAPx7 | FUSED-LAYOUT-latent |
| math_geometry.o | .sdata2 | pool_patterns | latent | nw4r/src/math/math_geometry.cpp | .sdata2=0x0 | GAPx5 | FUSED-RENAME-latent |
| math_types.o | .sdata2 | pool_patterns | latent | nw4r/src/math/math_types.cpp | .sdata2=0x0 | GAPx3 | FUSED-RENAME-latent |
| mpv_hdec.o | .rodata | - | latent | CriWare/src/sofdec/sfdcore/mpv/mpv_hdec.c | .rodata=0x0 | - | STANDALONE-latent |
| mpv_mc.o | .data | data_pool_patterns | latent | CriWare/src/sofdec/sfdcore/mpv/mpv_mc.c | .data=0x0 | GAP | FUSED-RENAME-latent |
| mpv_mcy.o | .data | data_pool_patterns | latent | CriWare/src/sofdec/sfdcore/mpv/mpv_mcy.c | .data=0x0 | GAP | FUSED-RENAME-latent |
| mpvabdec.o | .data | exact_renames | latent | CriWare/src/sofdec/sfdcore/mpv/mpvabdec.c | .data=0x0 | GAPx3 | FUSED-RENAME-latent |
| ocBdat.o | .data | exact_renames | latent | kyoshin/plugin/ocBdat.cpp | .data=0x0 | GAPx2 | FUSED-RENAME-latent |
| ocUnit.o | .sdata2+ .sdata | pool_patterns+ exact_renames | latent | kyoshin/plugin/ocUnit.cpp | .sdata2=0x0 .sdata=0x0 | GAPx3,FloatUtils.cpp | FUSED-RENAME-latent |
| pluginBtl.o | .rodata+ .sdata2 | exact_renames | latent | kyoshin/plugin/pluginBtl.cpp | .rodata=0x0 .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| pluginCam.o | .sdata2 | pool_patterns | latent | kyoshin/plugin/pluginCam.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| pluginCfs.o | .sdata2 | pool_patterns | latent | kyoshin/plugin/pluginCfs.cpp | .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| pluginEve.o | .sdata2 | pool_patterns | live | kyoshin/plugin/pluginEve.cpp | .sdata2=0x0 | usb.cx2 | FUSED-RENAME-live |
| pluginSnd.o | .sdata2 | pool_patterns | latent | kyoshin/plugin/pluginSnd.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| pluginUnit.o | .sdata2 | pool_patterns | latent | kyoshin/plugin/pluginUnit.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| port_rfc.o | .sdata2 | - | latent | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc.c | .sdata2=0x0 | - | STANDALONE-latent |
| sfd_adxt.o | .rodata | data_pool_patterns | latent | CriWare/src/sofdec/sfdcore/sfd/sfd_adxt.c | .rodata=0x0 | GAP | FUSED-RENAME-latent |
| sfx_cnv.o | .data | data_pool_patterns | latent | CriWare/src/sofdec/sfx/sfx_cnv.c | .data=0x0 | GAPx3 | FUSED-RENAME-latent |
| snd_AxManager.o | .sdata2 | pad_data_section+ pool_patterns+ exact_renames+ prefix_renames | latent | nw4r/src/snd/snd_AxManager.cpp | .sdata2=0x0 | GAPx8 | FUSED-LAYOUT-latent |
| snd_AxVoice.o | .sdata2 | pool_patterns | latent | nw4r/src/snd/snd_AxVoice.cpp | .sdata2=0x0 | GAPx6 | FUSED-RENAME-latent |
| snd_AxVoiceManager.o | .bss+ .sbss | exact_renames+ prefix_renames | latent | nw4r/src/snd/snd_AxVoiceManager.cpp | .bss=0x0 .sbss=0x0 | GAPx2 | FUSED-RENAME-latent |
| snd_AxfxImpl.o | .sbss | - | live | nw4r/src/snd/snd_AxfxImpl.cpp | .sbss=0x0 | - | STANDALONE-live |
| snd_Bank.o | .sdata2 | pool_patterns+ exact_renames | latent | nw4r/src/snd/snd_Bank.cpp | .sdata2=0x0 | GAPx5 | FUSED-RENAME-latent |
| snd_BasicPlayer.o | .data | exact_renames | latent | nw4r/src/snd/snd_BasicPlayer.cpp | .data=0x0 | GAP | FUSED-RENAME-latent |
| snd_BasicSound.o | .sdata2+ .data | pool_patterns+ exact_renames | latent | nw4r/src/snd/snd_BasicSound.cpp | .sdata2=0x0 .data=0x0 | GAPx4 | FUSED-RENAME-latent |
| snd_BiquadFilterPreset.o | .sdata2 | pool_patterns | latent | nw4r/src/snd/snd_BiquadFilterPreset.cpp | .sdata2=0x0 | GAPx2 | FUSED-RENAME-latent |
| snd_Channel.o | .sdata2 | pool_patterns+ exact_renames+ prefix_renames | latent | nw4r/src/snd/snd_Channel.cpp | .sdata2=0x0 | GAPx8 | FUSED-RENAME-latent |
| snd_DisposeCallbackManager.o | .bss+ .sbss | exact_renames+ prefix_renames+ drop_text_symbols | latent | nw4r/src/snd/snd_DisposeCallbackManager.cpp | .bss=0x0 .sbss=0x0 | GAP | FUSED-LAYOUT-latent |
| snd_FxDelayDpl2.o | .sdata2 | pad_data_section+ pool_patterns+ drop_text_symbols | latent | nw4r/src/snd/snd_FxDelayDpl2.cpp | .sdata2=0x0 | GAPx2 | FUSED-LAYOUT-latent |
| snd_Lfo.o | .sdata2 | retarget_relocs | latent | nw4r/src/snd/snd_Lfo.cpp | .sdata2=0x0 | GAPx2 | FUSED-LAYOUT-latent |
| snd_MmlParser.o | .sdata2 | pad_data_section+ pool_patterns+ exact_renames | latent | nw4r/src/snd/snd_MmlParser.cpp | .sdata2=0x0 | GAPx10 | FUSED-LAYOUT-latent |
| snd_RemoteSpeakerManager.o | .bss+ .sbss | prefix_renames | latent | nw4r/src/snd/snd_RemoteSpeakerManager.cpp | .bss=0x0 .sbss=0x0 | - | FUSED-RENAME-latent |
| snd_SeqPlayer.o | .sdata2+ .data+ .sbss | pool_patterns+ exact_renames | latent | nw4r/src/snd/snd_SeqPlayer.cpp | .sdata2=0x0 .data=0x0 .sbss=0x0 | GAPx5 | FUSED-RENAME-latent |
| snd_SeqTrack.o | .sdata2 | pad_data_section+ pool_patterns+ data_pool_patterns+ exact_renames | latent | nw4r/src/snd/snd_SeqTrack.cpp | .sdata2=0x0 | GAPx11 | FUSED-LAYOUT-latent |
| snd_SoundArchivePlayer.o | .sdata2 | drop_data_tail+ pool_patterns+ exact_renames | latent | nw4r/src/snd/snd_SoundArchivePlayer.cpp | .sdata2=0x0 | GAPx7 | FUSED-LAYOUT-latent |
| snd_SoundPlayer.o | .data+ .rodata+ .sdata+ .sdata2 | pad_data_section+ exact_renames+ prefix_renames+ drop_text_symbols+ symbol_sizes | latent | nw4r/src/snd/snd_SoundPlayer.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | - | FUSED-LAYOUT-latent |
| snd_SoundSystem.o | .bss+ .sbss | prefix_renames | latent | nw4r/src/snd/snd_SoundSystem.cpp | .bss=0x0 .sbss=0x0 | - | FUSED-RENAME-latent |
| snd_SoundThread.o | .data+ .rodata+ .sdata+ .sdata2 | pad_data_section+ prefix_renames+ drop_text_symbols | latent | nw4r/src/snd/snd_SoundThread.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | - | FUSED-LAYOUT-latent |
| snd_StrmPlayer.o | .sdata2 | drop_data_tail+ drop_nobits_range+ pad_data_section+ set_data_align+ exact_renames | latent | nw4r/src/snd/snd_StrmPlayer.cpp | .sdata2=0x0 | GAPx3 | FUSED-LAYOUT-latent |
| snd_StrmSound.o | .sdata2 | drop_data_tail+ pad_data_section+ pool_patterns+ exact_renames | latent | nw4r/src/snd/snd_StrmSound.cpp | .sdata2=0x0 | GAPx2 | FUSED-LAYOUT-latent |
| snd_Task.o | .data | - | live | nw4r/src/snd/snd_Task.cpp | .data=0x0 | - | STANDALONE-live |
| snd_TaskManager.o | .data+ .rodata+ .sdata+ .sdata2 | pad_data_section+ exact_renames+ prefix_renames+ drop_text_symbols | latent | nw4r/src/snd/snd_TaskManager.cpp | .data=0x0 .rodata=0x0 .sdata=0x0 .sdata2=0x0 | - | FUSED-LAYOUT-latent |
| snd_Voice.o | .sdata2 | drop_data_tail+ pool_patterns+ exact_renames | latent | nw4r/src/snd/snd_Voice.cpp | .sdata2=0x0 | GAPx11 | FUSED-LAYOUT-latent |
| snd_WsdPlayer.o | .sdata2 | drop_data_tail+ pool_patterns+ exact_renames | latent | nw4r/src/snd/snd_WsdPlayer.cpp | .sdata2=0x0 | GAPx8 | FUSED-LAYOUT-latent |
| ut_ArchiveFontBase.o | .data | - | latent | nw4r/src/ut/ut_ArchiveFontBase.cpp | .data=0x0 | - | STANDALONE-latent |
| ut_CharWriter.o | .sdata2 | pool_patterns | latent | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_CharWriter.cpp;nw4r/src/ut/ut_CharWriter.cpp | .sdata2=0x0 | GAP | FUSED-RENAME-latent |
| ut_PackedFont.o | .data | retarget_relocs | latent | nw4r/src/ut/ut_PackedFont.cpp | .data=0x0 | GAP | FUSED-LAYOUT-latent |
| ut_TagProcessorBase.o#Q34nw4r | .data+ .sdata2 | pool_patterns+ exact_renames | live | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TagProcessorBase.cpp;nw4r/src/ut/ut_TagProcessorBase.cpp | .data=0x88 .sdata2=0x0 | GAPx3 | FUSED-RENAME-live |
| ut_TextWriterBase.o#Q34nw4r2ut | .sdata2 | retarget_relocs+ pool_patterns+ drop_text_symbols+ drop_text_symbols_as_undef | live | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase.cpp;nw4r/src/ut/ut_TextWriterBase.cpp | .sdata2=0x0 | GAPx4,ut_TextWriterBase.cppx4,UNRESx2 | FUSED-LAYOUT-live |
