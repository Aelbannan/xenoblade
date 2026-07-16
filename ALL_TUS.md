# All Translation Units (TUs) — Decompilation Status

Generated from `config/us/splits.txt`, `src/` source tree, `TASKS.md`, and `DECOMP_MAP.md`.

**Legend:**

- **NO SOURCE** — listed in `splits.txt` but no `.cpp` exists yet
- **HAS SOURCE** — `.cpp` exists but key functions below `EQUIVALENT_MATCH` / `FULL_MATCH`
- **DONE** — all target functions at `EQUIVALENT_MATCH` / `FULL_MATCH` per `TASKS.md`

---

## 1. Root `kyoshin/`

| TU | Status | DECOMP_MAP ref |
|---|---|---|
| `kyoshin/CGame.cpp` | **DONE** | §5.1 |
| `kyoshin/main.cpp` | **DONE** | — |
| `kyoshin/CTaskGame.cpp` | **DONE** | — |
| `kyoshin/CTaskGameEff.cpp` | **NO SOURCE** | — |
| `kyoshin/CTimeLightGrp.cpp` | **NO SOURCE** | — |
| `kyoshin/CMainMenu.cpp` | **NO SOURCE** | §10.2, §11 |
| `kyoshin/CMiniMap.cpp` | **NO SOURCE** | — |
| `kyoshin/CQuestWindow.cpp` | **NO SOURCE** | §10.2 |
| `kyoshin/CSystemWindow.cpp` | **NO SOURCE** | §10.2, §11 |
| `kyoshin/CSysWinSelect.cpp` | **NO SOURCE** | §11 |
| `kyoshin/CTagProcessor.cpp` | **NO SOURCE** | — |
| `kyoshin/CTalkWindow.cpp` | **NO SOURCE** | §10.2 |
| `kyoshin/CUIBattleManager.cpp` | **HAS SOURCE** (Move STRUCTURAL) | §8.3 |
| `kyoshin/CUICfManager.cpp` | **HAS SOURCE** (Move 94.3%, helper 98.2%) | §10.1 |
| `kyoshin/CUIWindowManager.cpp` | **HAS SOURCE** (Move 80.2%) | §10.1 |
| `kyoshin/COccCulling.cpp` | **HAS SOURCE** (setFrustum 88.5%) | §9.2 |
| `kyoshin/CBgTex.cpp` | **DONE** | — |
| `kyoshin/CNandData.cpp` | **DONE** | — |
| `kyoshin/CBattery.cpp` | **DONE** | — |
| `kyoshin/CTitleAHelp.cpp` | **DONE** | — |
| `kyoshin/code_80296898.cpp` | **DONE** | — |
| `kyoshin/CCol6System.cpp` | **NO SOURCE** | — |
| `kyoshin/CCol6Invite.cpp` | **NO SOURCE** | — |
| `kyoshin/CSimpleEveTalkWin.cpp` | **NO SOURCE** | — |
| `kyoshin/CLoad.cpp` | **NO SOURCE** | — |
| `kyoshin/CErrMes.cpp` | **NO SOURCE** | — |
| `kyoshin/CUIErrMesWin.cpp` | **NO SOURCE** | — |
| `kyoshin/CItemBoxGrid.cpp` | **NO SOURCE** | — |
| `kyoshin/CItemBoxInfo.cpp` | **NO SOURCE** | — |
| `kyoshin/CItemBoxLine.cpp` | **NO SOURCE** | — |
| `kyoshin/CItemBoxGridSubMenu.cpp` | **NO SOURCE** | — |
| `kyoshin/CCur.cpp` | **NO SOURCE** | — |
| `kyoshin/CSortMenu.cpp` | **NO SOURCE** | — |
| `kyoshin/CNumSelect.cpp` | **NO SOURCE** | — |
| `kyoshin/CScrollBar.cpp` | **NO SOURCE** | — |
| `kyoshin/CPartyStateWin.cpp` | **NO SOURCE** | — |
| `kyoshin/CPartyState.cpp` | **NO SOURCE** | — |
| `kyoshin/CEquipChange.cpp` | **NO SOURCE** | — |
| `kyoshin/CEquipItemBox.cpp` | **NO SOURCE** | — |
| `kyoshin/CModelDisp.cpp` | **NO SOURCE** | — |
| `kyoshin/CMapSel.cpp` | **NO SOURCE** | — |
| `kyoshin/CFade.cpp` | **NO SOURCE** | — |
| `kyoshin/CFloorMap.cpp` | **NO SOURCE** | — |
| `kyoshin/CCollepedia.cpp` | **NO SOURCE** | — |
| `kyoshin/CKizunagram.cpp` | **NO SOURCE** | — |
| `kyoshin/CPcKizunagram.cpp` | **NO SOURCE** | — |
| `kyoshin/CPassiveSkill.cpp` | **NO SOURCE** | — |
| `kyoshin/CKizunaTalkList.cpp` | **NO SOURCE** | — |
| `kyoshin/CArtsInfo.cpp` | **NO SOURCE** | — |
| `kyoshin/CSysWin.cpp` | **NO SOURCE** | — |
| `kyoshin/CSelShopWin.cpp` | **NO SOURCE** | — |
| `kyoshin/CExchangeWin.cpp` | **NO SOURCE** | — |
| `kyoshin/CPresentWin.cpp` | **NO SOURCE** | — |
| `kyoshin/CSysWinBuff.cpp` | **NO SOURCE** | — |
| `kyoshin/CSysWinSave.cpp` | **NO SOURCE** | — |
| `kyoshin/CSysWinScenarioLog.cpp` | **NO SOURCE** | — |
| `kyoshin/CSaveLoad.cpp` | **NO SOURCE** | — |
| `kyoshin/CTaskGamePic.cpp` | **NO SOURCE** | — |
| `kyoshin/CTaskGameEvt.cpp` | **NO SOURCE** | — |
| `kyoshin/CTutorial.cpp` | **NO SOURCE** | — |
| `kyoshin/COption.cpp` | **NO SOURCE** | — |
| `kyoshin/CSkipTimer.cpp` | **NO SOURCE** | — |
| `kyoshin/CTitle.cpp` | **NO SOURCE** | — |
| `kyoshin/CQstLogList.cpp` | **NO SOURCE** | — |
| `kyoshin/CQstLogInfo.cpp` | **NO SOURCE** | — |
| `kyoshin/code_800A3B24.cpp` | **NO SOURCE** | — |
| `kyoshin/code_800A75FC.cpp` | **NO SOURCE** | — |
| `kyoshin/code_800AA008.cpp` | **NO SOURCE** | — |
| `kyoshin/code_800B06A4.cpp` | **NO SOURCE** | — |
| `kyoshin/code_800C17DC.cpp` | **NO SOURCE** | — |
| `kyoshin/code_800F42AC.cpp` | **NO SOURCE** | — |
| `kyoshin/code_80135FDC.cpp` | **NO SOURCE** | — |
| `kyoshin/code_801862C0.cpp` | **NO SOURCE** | — |
| `kyoshin/code_801A929C.cpp` | **NO SOURCE** | — |
| `kyoshin/code_801C2C14.cpp` | **NO SOURCE** | — |
| `kyoshin/code_801F3BE0.cpp` | **NO SOURCE** | — |
| `kyoshin/code_802405F4.cpp` | **NO SOURCE** | — |
| `kyoshin/code_8025FB10.cpp` | **NO SOURCE** | — |
| `kyoshin/code_8027513C.cpp` | **NO SOURCE** | — |
| `kyoshin/code_802B8A3C.cpp` | **NO SOURCE** | — |
| `kyoshin/code_802B9064.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/code_800C17DC.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/code_800F42AC.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/code_801C2C14.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/code_8018F8D8.cpp` | **NO SOURCE** | — |

---

## 2. `kyoshin/cf/` (core framework)

| TU | Status | DECOMP_MAP ref |
|---|---|---|
| `kyoshin/cf/CfPadTask.cpp` | **DONE** | §6.1 |
| `kyoshin/cf/CfGameManager.cpp` | **DONE** | §6.2 |
| `kyoshin/cf/CBattleManager.cpp` | **DONE** | — |
| `kyoshin/cf/CfCam.cpp` | **DONE** | §9.1 |
| `kyoshin/cf/CfCamEvent.cpp` | **DONE** | §9.1 |
| `kyoshin/cf/CfCamDirectionIntf.cpp` | **DONE** | §9.1 |
| `kyoshin/cf/CfCamLookatIntf.cpp` | **DONE** | §9.1 |
| `kyoshin/cf/CfCamTargetIntf.cpp` | **DONE** | §9.1 |
| `kyoshin/cf/CArtsSet.cpp` | **HAS SOURCE** (vfunc#1 STRUCTURAL) | §7.4 |
| `kyoshin/cf/CArtsParam.cpp` | **DONE** | §7.4 |
| `kyoshin/cf/CTaskGameCf.cpp` | **DONE** | — |
| `kyoshin/cf/CTaskCulling.cpp` | **DONE** | §9.2 |
| `kyoshin/cf/CfBdat.cpp` | **DONE** | — |
| `kyoshin/cf/CMcaFile.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CTaskEnvironment.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CActParamAnimGame.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CTaskGameEffAfter.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfRes.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/IResInfo.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfScript.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfTaskMain.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfTFile.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfCamEvent_1.cpp` | **NO SOURCE** | §9.1 |
| `kyoshin/cf/CtrlEnemy.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CtrlMoveBase.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CtrlMoveEne.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CtrlNpc.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CtrlPc.cpp` | **NO SOURCE** | §6.1 |
| `kyoshin/cf/CtrlRemote.cpp` | **NO SOURCE** | §6.1 |
| `kyoshin/cf/CtrlObjectParam.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfCollCircleImpl.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfCollSphereImpl.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfCollAABBImpl.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfCollCylinderImpl.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfCollCapsuleImpl.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CtrlAct.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfObjectEnumList.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfObjectSelectorObj.cpp` | **NO SOURCE** | §7.5 |
| `kyoshin/cf/CItem.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CCharEffect.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CCharEffectEne.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CTaskREvent.cpp` | **NO SOURCE** | §10.2 |
| `kyoshin/cf/CInfoCf.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CTaskREvtSequence.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfResObjImpl.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfResReloadImpl.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfMapEffectManager.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfMapItemManager.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfResPcImpl.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CVision.cpp` | **NO SOURCE** | §10.2 |
| `kyoshin/cf/CPcEffect07.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CSuddenCommu.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfSoundMan.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfGimmickObject.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfResTboxImpl.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfGimmick.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfGimmickElv.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfGimmickLock.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfGimmickWarp.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfGimmickJump.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfGimmickItem.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfGimmickEne.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfGimmickSaveOff.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfHikariItemManager.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfNandManager.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CPartsChange.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CtrlMovePC.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CtrlMoveNpc.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/ICamControlRemote.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/ICamControlGc.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/ICamControlClassic.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CHelpManager.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/CfMapMineManager.cpp` | **NO SOURCE** | — |

---

## 3. `kyoshin/cf/object/`

| TU | Status | DECOMP_MAP ref |
|---|---|---|
| `kyoshin/cf/object/CBattleState.cpp` | **HAS SOURCE** (3/9 FULL_MATCH) | §7.2 |
| `kyoshin/cf/object/CAIAction.cpp` | **HAS SOURCE** (vfunc#1 96%, vfunc#2 96%) | §7.3 |
| `kyoshin/cf/object/CfObjectActor.cpp` | **DONE** | — |
| `kyoshin/cf/object/CActorParam.cpp` | **DONE** | — |
| `kyoshin/cf/object/CfObjectColl.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/object/CfObjectEff.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/object/CfObjectEne.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/object/CfObjectMap.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/object/CfObjectModel.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/object/CfObjectMove.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/object/CfObjectNpc.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/object/CfObjectObj.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/object/CfObjectPc.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/object/CfObjectPoint.cpp` | **NO SOURCE** | — |
| `kyoshin/cf/object/CfObjectTbox.cpp` | **NO SOURCE** | — |

---

## 4. `kyoshin/cf/chain/`

| TU | Status |
|---|---|
| `kyoshin/cf/chain/CChain.cpp` | **DONE** |
| `kyoshin/cf/chain/CChainActorList.cpp` | **DONE** |
| `kyoshin/cf/chain/CChainActorPc.cpp` | **DONE** |
| `kyoshin/cf/chain/CChainCombo.cpp` | **DONE** |
| `kyoshin/cf/chain/CChainTime.cpp` | **NO SOURCE** |
| `kyoshin/cf/chain/CChainTimer.cpp` | **NO SOURCE** |
| `kyoshin/cf/chain/CChainActorEne.cpp` | **NO SOURCE** |
| `kyoshin/cf/chain/CChainEffect.cpp` | **NO SOURCE** |

---

## 5. `kyoshin/cf/voice/` and `kyoshin/cf/voice/cvsys/`

| TU | Status |
|---|---|
| `kyoshin/cf/voice/CCharVoiceMan.cpp` | **DONE** |
| `kyoshin/cf/voice/CCharVoice.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD.cpp` | **DONE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_BUF.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_CHAIN.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_FAINT.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_HAGE.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_HP.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_PARTY_GAGE.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_SUDDEN.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_BREAK.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_TELL.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END_SP.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_MAIN.cpp` | **NO SOURCE** |
| `kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_BEGIN.cpp` | **NO SOURCE** |

---

## 6. `kyoshin/cfsys/` (no directory exists)

| TU | Status |
|---|---|
| `kyoshin/cfsys/CfObjectImplWalker.cpp` | **NO SOURCE** |
| `kyoshin/cfsys/CfObjectImplPc.cpp` | **NO SOURCE** |
| `kyoshin/cfsys/CfObjectImplObj.cpp` | **NO SOURCE** |
| `kyoshin/cfsys/CfObjectImplNpc.cpp` | **NO SOURCE** |
| `kyoshin/cfsys/CfObjectImplMove.cpp` | **NO SOURCE** |
| `kyoshin/cfsys/CfObjectImplEne.cpp` | **NO SOURCE** |

---

## 7. `kyoshin/realtimeevt/` (no directory exists)

| TU | Status |
|---|---|
| `kyoshin/realtimeevt/CREvtMem.cpp` | **NO SOURCE** |
| `kyoshin/realtimeevt/CREvtModel.cpp` | **NO SOURCE** |
| `kyoshin/realtimeevt/CREvtCamera.cpp` | **NO SOURCE** |
| `kyoshin/realtimeevt/CREvtModelMap.cpp` | **NO SOURCE** |
| `kyoshin/realtimeevt/CREvtModelObj.cpp` | **NO SOURCE** |
| `kyoshin/realtimeevt/CREvtModelPc.cpp` | **NO SOURCE** |
| `kyoshin/realtimeevt/CREvtEffect.cpp` | **NO SOURCE** |
| `kyoshin/realtimeevt/CREvtObj.cpp` | **NO SOURCE** |
| `kyoshin/realtimeevt/CREvtLight.cpp` | **NO SOURCE** |
| `kyoshin/realtimeevt/CREvtMovie.cpp` | **NO SOURCE** |

---

## 8. `kyoshin/action/` (no directory exists)

| TU | Status |
|---|---|
| `kyoshin/action/CActParamAnim.cpp` | **NO SOURCE** |
| `kyoshin/action/CActParamData.cpp` | **NO SOURCE** |

---

## 9. `kyoshin/plugin/`

| TU | Status |
|---|---|
| `kyoshin/plugin/pluginDeb.cpp` | **DONE** |
| `kyoshin/plugin/pluginWait.cpp` | **DONE** |
| `kyoshin/plugin/pluginMain.cpp` | **DONE** |
| `kyoshin/plugin/pluginUi.cpp` | **DONE** |
| `kyoshin/plugin/pluginMath.cpp` | **DONE** |
| `kyoshin/plugin/pluginPad.cpp` | **DONE** |
| `kyoshin/plugin/pluginUnit.cpp` | **DONE** |
| `kyoshin/plugin/pluginGame.cpp` | **DONE** |
| `kyoshin/plugin/pluginVoice.cpp` | **DONE** |
| `kyoshin/plugin/ocBuiltin.cpp` | **NO SOURCE** |
| `kyoshin/plugin/ocThread.cpp` | **NO SOURCE** |
| `kyoshin/plugin/ocMsg.cpp` | **NO SOURCE** |
| `kyoshin/plugin/ocBdat.cpp` | **NO SOURCE** |
| `kyoshin/plugin/ocUnit.cpp` | **NO SOURCE** |
| `kyoshin/plugin/ocCfp.cpp` | **NO SOURCE** |
| `kyoshin/plugin/pluginEve.cpp` | **NO SOURCE** |
| `kyoshin/plugin/pluginCfs.cpp` | **NO SOURCE** |
| `kyoshin/plugin/pluginCam.cpp` | **NO SOURCE** |
| `kyoshin/plugin/pluginTime.cpp` | **NO SOURCE** |
| `kyoshin/plugin/pluginBtl.cpp` | **NO SOURCE** |
| `kyoshin/plugin/pluginSnd.cpp` | **NO SOURCE** |
| `kyoshin/plugin/pluginHelp.cpp` | **NO SOURCE** |

---

## 10. `kyoshin/menu/`

| TU | Status | DECOMP_MAP ref |
|---|---|---|
| `kyoshin/menu/CMenuArtsSelect.cpp` | **HAS SOURCE** (0/5 accepted) | §8.1 |
| `kyoshin/menu/CMenuBattlePlayerState.cpp` | **HAS SOURCE** (2/5 FULL_MATCH) | §8.2 |
| `kyoshin/menu/CMenuEnemyState.cpp` | **HAS SOURCE** (Move 85.6%, cbRenderBefore 98.2%) | §8.3 |
| `kyoshin/menu/CMenuFade.cpp` | **DONE** | — |
| `kyoshin/menu/CMenuPTGauge.cpp` | **DONE** | §8.3 |
| `kyoshin/menu/CMenuBattleMode.cpp` | **DONE** | §8.3 |
| `kyoshin/menu/CMenuBattleDamage.cpp` | **NO SOURCE** | §8.3 |
| `kyoshin/menu/CMenuKeyAssign.cpp` | **NO SOURCE** | §11 |
| `kyoshin/menu/CMenuUpdate.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuLandTelop.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuGetItem.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuItem.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuZeal.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuQuestLog.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuSymbolMark.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuSelectShop.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuShopSell.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuShopBuy.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuPTState.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuVision.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuBattleCommu.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuGetItemMulti.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuKizunaTalk.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuItemExchange.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuSave.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuPTChangeNotice.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuTutorial.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuOption.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuSkipTimer.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuBattleChain.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuTutorialList.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuGCItem.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuGameClear.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuTitle.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuMapSelect.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuMapSelectSC.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuPause.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuCollepedia.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuKizunagram.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuPassiveSkill.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuBattleEnd.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuPlayAward.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuKizunaTalkList.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuLvUp.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuQstCnt.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/CMenuArtsSet.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/code_80187F14.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/code_8018C5FC.cpp` | **NO SOURCE** | — |
| `kyoshin/menu/parts/CModelDispEquip.cpp` | **NO SOURCE** | — |

---

## 11. `kyoshin/makecrystal/`

| TU | Status |
|---|---|
| `kyoshin/makecrystal/CMCCylinderGauge.cpp` | **DONE** |
| `kyoshin/makecrystal/CMCEffStart.cpp` | **DONE** |
| `kyoshin/makecrystal/CMenuMakeCrystal.cpp` | **NO SOURCE** |
| `kyoshin/makecrystal/CMakeCrystalWin.cpp` | **NO SOURCE** |
| `kyoshin/makecrystal/code_80213488.cpp` | **NO SOURCE** |
| `kyoshin/makecrystal/CMCCrystalBox.cpp` | **NO SOURCE** |
| `kyoshin/makecrystal/CMCCrystalInfo.cpp` | **NO SOURCE** |
| `kyoshin/makecrystal/CModelDispMakeCrystal.cpp` | **NO SOURCE** |
| `kyoshin/makecrystal/CMCCrystalList.cpp` | **NO SOURCE** |
| `kyoshin/makecrystal/CMCCrystalSupport.cpp` | **NO SOURCE** |
| `kyoshin/makecrystal/CMCGetItemBox.cpp` | **NO SOURCE** |

---

## 12. `kyoshin/help/`

| TU | Status |
|---|---|
| `kyoshin/help/CHelp_Target.cpp` | **DONE** |
| `kyoshin/help/CHelp.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_ArtsAttack.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_ArtsSet.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_CkKizuna.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_CloseItemMenu.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_CloseSysMenu.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_EndEvent.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_EnemyCount.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_EnemyEnable.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_EtherMake.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_Exchange.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_GameOver.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_ItemCole.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_Kizuna.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_LandMark.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_LearnArts.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_ClosePartyMenu.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_OpenPartyMenu.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_Pg.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_CloseQuestMenu.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_ShopBuy.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_ShopSel.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_Sp.cpp` | **NO SOURCE** |
| `kyoshin/help/CHelp_Talk.cpp` | **NO SOURCE** |

---

## 13. Fork-only additions (not in retail splits.txt)

| TU | Note |
|---|---|
| `kyoshin/coop/PresentationGate.cpp` | Co-op addition, source exists |
| `kyoshin/cf/chain/CChainActor.cpp` | Not in splits.txt (local addition?) |
| `kyoshin/cf/chain/CChainActorFactory.cpp` | Not in splits.txt (local addition?) |
| `kyoshin/cf/chain/CChainMember.cpp` | Not in splits.txt (local addition?) |
| `kyoshin/cf/camera/CfCamLookatIntf.cpp` | Duplicate of CfCamLookatIntf? |
| `kyoshin/ErrMesData.cpp` | Not in splits.txt (local addition?) |

---

## Summary

| Category | Count |
|---|---|
| Total retail TUs in `config/us/splits.txt` (`kyoshin/` section) | ~270 |
| TUs with `.cpp` source created | ~60 |
| TUs with **NO source file** | **~210** |
| TUs reaching EQUIVALENT/FULL_MATCH | ~50 |
| TUs partially matched (source exists, below bar) | ~10 |
