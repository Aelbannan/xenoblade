# FULL_MATCH demotions — sweep_stale_full_match.py (2026-09-19)

Demoted **1662** rows from `FULL_MATCH` → `NOT_STARTED` (non-byte-identical bodies via `_byte_identical_with_relocs`).

Source: `python3 tools/coop/sweep_stale_full_match.py` (r6 F1).

Dry-run reported 1704 candidates; the applying pass demoted **1662** (42 candidates were no longer stale / no longer `FULL_MATCH` by apply time).
Rows whose objects were unavailable (`FileNotFoundError`≈5054, `ElfSymbolError`≈367) were left untouched.

Follow-up: backfilled `instruction_match = 100.0` on remaining `FULL_MATCH` rows that still carry a `full-instruction-match` certificate but had a missing/zero persisted score.

Post-sweep registry: `FULL_MATCH=13147`, of which `instruction_match>=100`: `12879`, `instruction_match==0`: `176` (no full-instruction-match cert or unscanned).

## By unit (top)

| unit | n |
|---|--:|
| `kyoshin/cf/CfGameManager` | 72 |
| `kyoshin/plugin/ocUnit` | 66 |
| `kyoshin/cf/CPartsChange` | 43 |
| `kyoshin/CEquipItemBox` | 42 |
| `kyoshin/CPassiveSkill` | 39 |
| `kyoshin/makecrystal/CMCEffStart` | 36 |
| `kyoshin/CSaveLoad` | 34 |
| `kyoshin/makecrystal/CMCCrystalBox` | 32 |
| `kyoshin/cf/object/CfObjectMove` | 28 |
| `RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase` | 27 |
| `RVL_SDK/src/revolution/dvd/dvd_broadway` | 24 |
| `kyoshin/CItemBoxLine` | 22 |
| `kyoshin/CUIWindowManager` | 21 |
| `kyoshin/CTaskGame` | 20 |
| `kyoshin/cf/CfRes` | 20 |
| `kyoshin/code_800B06A4` | 20 |
| `kyoshin/CQstLogList` | 20 |
| `kyoshin/menu/CMenuPlayAward` | 19 |
| `kyoshin/cf/voice/CCharVoiceMan` | 19 |
| `kyoshin/CUICfManager` | 17 |
| `kyoshin/cf/CfGimmick` | 17 |
| `kyoshin/CKizunagram` | 17 |
| `kyoshin/CTutorialList` | 17 |
| `RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc` | 17 |
| `kyoshin/cf/IResInfo` | 16 |
| `kyoshin/cf/CTaskREvtSequence` | 16 |
| `kyoshin/cf/object/CActorParam` | 15 |
| `kyoshin/CPartyStateWin` | 14 |
| `RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils` | 14 |
| `monolib/src/scn/CScnItemModel` | 14 |
| `kyoshin/plugin/pluginUi` | 13 |
| `monolib/src/scn/CScnItemModelNw4r` | 12 |
| `monolib/src/scn/CScn` | 12 |
| `kyoshin/cf/CTaskGameCf` | 11 |
| `kyoshin/cf/CfScript` | 11 |
| `kyoshin/cf/object/CfObjectEff` | 11 |
| `kyoshin/plugin/pluginBtl` | 11 |
| `kyoshin/makecrystal/CModelDispMakeCrystal` | 11 |
| `kyoshin/makecrystal/CMCCylinderGauge` | 11 |
| `kyoshin/makecrystal/CMCGetItemBox` | 11 |
| … (256 more units) | |

**Units touched:** 296

## Demoted targets

| id | unit | symbol | function | address |
|---|---|---|---|---|
| us-8003a2a8 | kyoshin/main | `OnBdatFileLoaded__FPvUl` | OnBdatFileLoaded | 0x8003A2A8 |
| us-8003a91c | kyoshin/plugin/ocMsg | `OcMsgMakeListValue` | OcMsgMakeListValue | 0x8003A91C |
| us-8003ab94 | kyoshin/plugin/ocMsg | `OcLogMakeListValue` | OcLogMakeListValue | 0x8003AB94 |
| us-8003b5d8 | kyoshin/plugin/ocBdat | `Bdat_DecodeValue_B6A0` | Bdat_DecodeValue_B6A0 | 0x8003B5D8 |
| us-8003bc60 | kyoshin/plugin/ocBdat | `Bdat_WriteVmArg_B800` | Bdat_WriteVmArg_B800 | 0x8003BC60 |
| us-8003c5c0 | kyoshin/plugin/ocUnit | `getPosX` | getPosX | 0x8003C5C0 |
| us-8003c64c | kyoshin/plugin/ocUnit | `getPosY` | getPosY | 0x8003C64C |
| us-8003c6d0 | kyoshin/plugin/ocUnit | `getPosZ` | getPosZ | 0x8003C6D0 |
| us-8003c754 | kyoshin/plugin/ocUnit | `getObjAngle` | getObjAngle | 0x8003C754 |
| us-8003c7dc | kyoshin/plugin/ocUnit | `getObjParam` | getObjParam | 0x8003C7DC |
| us-8003c870 | kyoshin/plugin/ocUnit | `getObjId` | getObjId | 0x8003C870 |
| us-8003c8d0 | kyoshin/plugin/ocUnit | `getTalkObj` | getTalkObj | 0x8003C8D0 |
| us-8003c94c | kyoshin/plugin/ocUnit | `getUnitHp` | getUnitHp | 0x8003C94C |
| us-8003c9fc | kyoshin/plugin/ocUnit | `setPosX` | setPosX | 0x8003C9FC |
| us-8003cadc | kyoshin/plugin/ocUnit | `setPosY` | setPosY | 0x8003CADC |
| us-8003cba0 | kyoshin/plugin/ocUnit | `setPosZ` | setPosZ | 0x8003CBA0 |
| us-8003cd08 | kyoshin/plugin/ocUnit | `setObjName` | setObjName | 0x8003CD08 |
| us-8003cdc8 | kyoshin/plugin/ocUnit | `applyPos` | applyPos | 0x8003CDC8 |
| us-8003cef0 | kyoshin/plugin/ocUnit | `dispOn` | dispOn | 0x8003CEF0 |
| us-8003cf8c | kyoshin/plugin/ocUnit | `dispOff` | dispOff | 0x8003CF8C |
| us-8003d0ec | kyoshin/plugin/ocUnit | `hideUnit` | hideUnit | 0x8003D0EC |
| us-8003d218 | kyoshin/plugin/ocUnit | `execNpcAction` | execNpcAction | 0x8003D218 |
| us-8003d2e8 | kyoshin/plugin/ocUnit | `getNpcAction` | getNpcAction | 0x8003D2E8 |
| us-8003d35c | kyoshin/plugin/ocUnit | `canTalk` | canTalk | 0x8003D35C |
| us-8003d44c | kyoshin/plugin/ocUnit | `setMoveWait` | setMoveWait | 0x8003D44C |
| us-8003d4c4 | kyoshin/plugin/ocUnit | `setTargetRot` | setTargetRot | 0x8003D4C4 |
| us-8003d5dc | kyoshin/plugin/ocUnit | `setNpcPath` | setNpcPath | 0x8003D5DC |
| us-8003d834 | kyoshin/plugin/ocUnit | `isMoving` | isMoving | 0x8003D834 |
| us-8003d8a8 | kyoshin/plugin/ocUnit | `waitMove` | waitMove | 0x8003D8A8 |
| us-8003db58 | kyoshin/plugin/ocUnit | `restartNpc` | restartNpc | 0x8003DB58 |
| us-8003df40 | kyoshin/plugin/ocUnit | `hasTalkFlag` | hasTalkFlag | 0x8003DF40 |
| us-8003e1f8 | kyoshin/plugin/ocUnit | `setState10` | setState10 | 0x8003E1F8 |
| us-8003e2c0 | kyoshin/plugin/ocUnit | `talkMsg` | talkMsg | 0x8003E2C0 |
| us-8003e328 | kyoshin/plugin/ocUnit | `getSearchHandle` | getSearchHandle | 0x8003E328 |
| us-8003e370 | kyoshin/plugin/ocUnit | `sendNotify` | sendNotify | 0x8003E370 |
| us-8003e46c | kyoshin/plugin/ocUnit | `setAct` | setAct | 0x8003E46C |
| us-8003e560 | kyoshin/plugin/ocUnit | `setRefreshVal` | setRefreshVal | 0x8003E560 |
| us-8003e94c | kyoshin/plugin/ocUnit | `turn` | turn | 0x8003E94C |
| us-8003e9f4 | kyoshin/plugin/ocUnit | `flagUnit80000` | flagUnit80000 | 0x8003E9F4 |
| us-8003ea4c | kyoshin/plugin/ocUnit | `flagUnit100000` | flagUnit100000 | 0x8003EA4C |
| us-8003eaa4 | kyoshin/plugin/ocUnit | `getPartyHandle` | getPartyHandle | 0x8003EAA4 |
| us-8003ebe8 | kyoshin/plugin/ocUnit | `getTargetUnitId` | getTargetUnitId | 0x8003EBE8 |
| us-8003ec88 | kyoshin/plugin/ocUnit | `isPC` | isPC | 0x8003EC88 |
| us-8003ecec | kyoshin/plugin/ocUnit | `isNPC` | isNPC | 0x8003ECEC |
| us-8003ed50 | kyoshin/plugin/ocUnit | `isENE` | isENE | 0x8003ED50 |
| us-8003edb4 | kyoshin/plugin/ocUnit | `isPT` | isPT | 0x8003EDB4 |
| us-8003ee2c | kyoshin/plugin/ocUnit | `invin` | invin | 0x8003EE2C |
| us-8003f0e0 | kyoshin/plugin/ocUnit | `delBuff` | delBuff | 0x8003F0E0 |
| us-8003f168 | kyoshin/plugin/ocUnit | `setColi` | setColi | 0x8003F168 |
| us-8003f1ec | kyoshin/plugin/ocUnit | `setEye` | setEye | 0x8003F1EC |
| us-8003f254 | kyoshin/plugin/ocUnit | `setGrav` | setGrav | 0x8003F254 |
| us-8003f2cc | kyoshin/plugin/ocUnit | `setUnitVisible` | setUnitVisible | 0x8003F2CC |
| us-8003f380 | kyoshin/plugin/ocUnit | `setUnitState` | setUnitState | 0x8003F380 |
| us-8003f3f0 | kyoshin/plugin/ocUnit | `setGameFlag` | setGameFlag | 0x8003F3F0 |
| us-8003f6f4 | kyoshin/plugin/ocUnit | `gravity` | gravity | 0x8003F6F4 |
| us-8003f78c | kyoshin/plugin/ocUnit | `findParty` | findParty | 0x8003F78C |
| us-8003f914 | kyoshin/plugin/ocUnit | `pointOnA` | pointOnA | 0x8003F914 |
| us-8003f994 | kyoshin/plugin/ocUnit | `pointOffA` | pointOffA | 0x8003F994 |
| us-8003fa14 | kyoshin/plugin/ocUnit | `findBattleActor` | findBattleActor | 0x8003FA14 |
| us-8003fbc8 | kyoshin/plugin/ocUnit | `pointOnB` | pointOnB | 0x8003FBC8 |
| us-8003fc48 | kyoshin/plugin/ocUnit | `pointOffB` | pointOffB | 0x8003FC48 |
| us-8003fcc8 | kyoshin/plugin/ocUnit | `setChildVal` | setChildVal | 0x8003FCC8 |
| us-8003fd48 | kyoshin/plugin/ocUnit | `followUnit` | followUnit | 0x8003FD48 |
| us-8003fef8 | kyoshin/plugin/ocUnit | `setUnitTarget` | setUnitTarget | 0x8003FEF8 |
| us-8003ff98 | kyoshin/plugin/ocUnit | `startBattle` | startBattle | 0x8003FF98 |
| us-80040094 | kyoshin/plugin/ocUnit | `pointOnC` | pointOnC | 0x80040094 |
| us-80040114 | kyoshin/plugin/ocUnit | `pointOffC` | pointOffC | 0x80040114 |
| us-800402c4 | kyoshin/plugin/ocUnit | `moveToPos` | moveToPos | 0x800402C4 |
| us-80040458 | kyoshin/plugin/ocUnit | `chkRange` | chkRange | 0x80040458 |
| us-80040570 | kyoshin/plugin/ocUnit | `chkEventRange` | chkEventRange | 0x80040570 |
| us-80040664 | kyoshin/plugin/ocUnit | `ocUnitRegist` | ocUnitRegist | 0x80040664 |
| us-800406c0 | kyoshin/CTaskGame | `__ct__9CTaskGameFP5CViewP11CWorkThreadi` | CTaskGame::CTaskGame(...*, 5, C, V, int, e, wchar_t, ...*, 1, 1, C, W, o, r, k, T, unsigned char, r, e, a, double, int) | 0x800406C0 |
| us-80040db4 | kyoshin/CTaskGame | `Term__9CTaskGameFv` | CTaskGame::Term() | 0x80040DB4 |
| us-80041010 | kyoshin/CTaskGame | `Draw__9CTaskGameFv` | CTaskGame::Draw() | 0x80041010 |
| us-80041014 | kyoshin/CTaskGame | `setLoadingCaption__9CTaskGameFUsUsPCcs` | CTaskGame::setLoadingCaption(short, short, const ...*, char, short) | 0x80041014 |
| us-80041110 | kyoshin/CTaskGame | `CTaskGame_moveFadeOut` | CTaskGame_moveFadeOut | 0x80041110 |
| us-800412b0 | kyoshin/CTaskGame | `CTaskGame_moveFadeTick` | CTaskGame_moveFadeTick | 0x800412B0 |
| us-800413bc | kyoshin/CTaskGame | `CTaskGame_movePrepDual` | CTaskGame_movePrepDual | 0x800413BC |
| us-800415fc | kyoshin/CTaskGame | `CTaskGame_moveTeardown` | CTaskGame_moveTeardown | 0x800415FC |
| us-8004177c | kyoshin/CTaskGame | `CTaskGame_moveNandReset` | CTaskGame_moveNandReset | 0x8004177C |
| us-80042240 | kyoshin/CTaskGame | `CTaskGame_moveTitleTick` | CTaskGame_moveTitleTick | 0x80042240 |
| us-800424cc | kyoshin/CTaskGame | `CTaskGame_moveAfterSave` | CTaskGame_moveAfterSave | 0x800424CC |
| us-800425c0 | kyoshin/CTaskGame | `CTaskGame_moveAfterOpt` | CTaskGame_moveAfterOpt | 0x800425C0 |
| us-800426b4 | kyoshin/CTaskGame | `CTaskGame_moveExitFade` | CTaskGame_moveExitFade | 0x800426B4 |
| us-80042ba8 | kyoshin/CTaskGame | `CTaskGame_moveResetFx` | CTaskGame_moveResetFx | 0x80042BA8 |
| us-80042cfc | kyoshin/CTaskGame | `CTaskGame_windowGate` | CTaskGame_windowGate | 0x80042CFC |
| us-80042dec | kyoshin/CTaskGame | `CTaskGame_deleteLoad` | CTaskGame_deleteLoad | 0x80042DEC |
| us-800436a4 | kyoshin/CTaskGame | `CTaskGame_updateStream` | CTaskGame_updateStream | 0x800436A4 |
| us-80044160 | kyoshin/CTaskGame | `CTaskGame_resetStream` | CTaskGame_resetStream | 0x80044160 |
| us-80044274 | kyoshin/CTaskGame | `CTaskGame_fadeStream` | CTaskGame_fadeStream | 0x80044274 |
| us-800443a4 | kyoshin/CTaskGame | `CTaskGame_enumListFill` | CTaskGame_enumListFill | 0x800443A4 |
| us-80044870 | kyoshin/cf/CTaskGameCf | `getInstance__Q22cf11CTaskGameCfFv` | cf::CTaskGameCf::getInstance() | 0x80044870 |
| us-80044878 | kyoshin/cf/CTaskGameCf | `reqExit__Q22cf11CTaskGameCfFv` | func_800442DC | 0x80044878 |
| us-80044978 | kyoshin/cf/CTaskGameCf | `Init__Q22cf11CTaskGameCfFv` | cf::CTaskGameCf::Init() | 0x80044978 |
| us-80044980 | kyoshin/cf/CTaskGameCf | `Term__Q22cf11CTaskGameCfFv` | cf::CTaskGameCf::Term() | 0x80044980 |
| us-800449c0 | kyoshin/cf/CTaskGameCf | `startNewGame__Q22cf11CTaskGameCfFv` | func_80044424 | 0x800449C0 |
| us-800449e0 | kyoshin/cf/CTaskGameCf | `initNewGame__Q22cf11CTaskGameCfFv` | func_80044444 | 0x800449E0 |
| us-80044a1c | kyoshin/cf/CTaskGameCf | `startContinue__Q22cf11CTaskGameCfFv` | func_80044480 | 0x80044A1C |
| us-80044a3c | kyoshin/cf/CTaskGameCf | `initContinue__Q22cf11CTaskGameCfFv` | func_800444A0 | 0x80044A3C |
| us-80044e7c | kyoshin/cf/CTaskGameCf | `beginExit__Q22cf11CTaskGameCfFv` | func_800448DC | 0x80044E7C |
| us-80044ed4 | kyoshin/cf/CTaskGameCf | `waitExit__Q22cf11CTaskGameCfFv` | func_80044934 | 0x80044ED4 |
| us-80044f5c | kyoshin/cf/CTaskGameCf | `create__Q22cf11CTaskGameCfFv` | cf::CTaskGameCf::create() | 0x80044F5C |
| us-8004566c | kyoshin/CTaskGameEff | `createGameEffTask` | createGameEffTask | 0x8004566C |
| us-80045ff4 | kyoshin/plugin/pluginUi | `fadeIn_1` | fadeIn_1 | 0x80045FF4 |
| us-800460a4 | kyoshin/plugin/pluginUi | `fadeOut_1` | fadeOut_1 | 0x800460A4 |
| us-80046154 | kyoshin/plugin/pluginUi | `fadeWait_1` | fadeWait_1 | 0x80046154 |
| us-800461a0 | kyoshin/plugin/pluginUi | `createCol6Sys` | createCol6Sys | 0x800461A0 |
| us-800461c4 | kyoshin/plugin/pluginUi | `createCol6Hint` | createCol6Hint | 0x800461C4 |
| us-80046284 | kyoshin/plugin/pluginUi | `createCol6Init` | createCol6Init | 0x80046284 |
| us-800462a8 | kyoshin/plugin/pluginUi | `checkCol6Bat` | checkCol6Bat | 0x800462A8 |
| us-800462cc | kyoshin/plugin/pluginUi | `simpleEventStart` | simpleEventStart | 0x800462CC |
| us-800462f0 | kyoshin/plugin/pluginUi | `simpleEventEnd` | simpleEventEnd | 0x800462F0 |
| us-80046ba4 | kyoshin/plugin/pluginUi | `save` | save | 0x80046BA4 |
| us-80046bd4 | kyoshin/plugin/pluginUi | `kizunaTalkStart` | kizunaTalkStart | 0x80046BD4 |
| us-80046c84 | kyoshin/plugin/pluginUi | `gameClear` | gameClear | 0x80046C84 |
| us-80046de8 | kyoshin/plugin/pluginUi | `pluginUiRegist` | pluginUiRegist | 0x80046DE8 |
| us-80046ec8 | kyoshin/plugin/pluginEve | `getFlag` | getFlag | 0x80046EC8 |
| us-80046f80 | kyoshin/plugin/pluginEve | `setFlag` | setFlag | 0x80046F80 |
| us-8004722c | kyoshin/plugin/pluginEve | `realtimeEventStart` | realtimeEventStart | 0x8004722C |
| us-8004731c | kyoshin/plugin/pluginEve | `realtimeEventEnd` | realtimeEventEnd | 0x8004731C |
| us-800474f8 | kyoshin/plugin/pluginEve | `fadeIn` | fadeIn | 0x800474F8 |
| us-800475fc | kyoshin/plugin/pluginEve | `fadeOut` | fadeOut | 0x800475FC |
| us-8004aa44 | kyoshin/plugin/pluginCfs | `setFade` | setFade | 0x8004AA44 |
| us-8004ac34 | kyoshin/plugin/pluginCfs | `setDispOffArea` | setDispOffArea | 0x8004AC34 |
| us-8004bf2c | kyoshin/action/CActParamAnim | `getAttachedAnimTime__13CActParamAnimFv` | CActParamAnim::getAttachedAnimTime() | 0x8004BF2C |
| us-8004bf44 | kyoshin/action/CActParamAnim | `getAttachedAnimRate__13CActParamAnimFv` | CActParamAnim::getAttachedAnimRate() | 0x8004BF44 |
| us-8004bf70 | kyoshin/action/CActParamAnim | `getAttachedAnimScale__13CActParamAnimFv` | CActParamAnim::getAttachedAnimScale() | 0x8004BF70 |
| us-8004d558 | kyoshin/action/CActParamAnim | `tickAnimFrame` | tickAnimFrame | 0x8004D558 |
| us-8004d6cc | kyoshin/action/CActParamAnim | `calcAnimBlendWeight__13CActParamAnimFv` | CActParamAnim::calcAnimBlendWeight() | 0x8004D6CC |
| us-800545d4 | kyoshin/action/CActParamData | `updateParamFloats` | updateParamFloats | 0x800545D4 |
| us-80055b44 | kyoshin/action/CActParamData | `flushParamPair` | flushParamPair | 0x80055B44 |
| us-800565fc | kyoshin/action/CActParamData | `getParamSelS16` | getParamSelS16 | 0x800565FC |
| us-800572e4 | kyoshin/action/CActParamData | `copyS16ToFlt` | copyS16ToFlt | 0x800572E4 |
| us-80057a94 | kyoshin/action/CActParamData | `fireParamPhaseA` | fireParamPhaseA | 0x80057A94 |
| us-8005dd54 | kyoshin/cf/CActParamAnimGame | `checkNotFalling__Q22cf17CActParamAnimGame` | cf::CActParamAnimGame::checkNotFalling | 0x8005DD54 |
| us-80060ac0 | kyoshin/plugin/pluginCam | `setPos` | setPos | 0x80060AC0 |
| us-80060bc4 | kyoshin/plugin/pluginCam | `setLookat` | setLookat | 0x80060BC4 |
| us-80060db0 | kyoshin/plugin/pluginCam | `setRotX` | setRotX | 0x80060DB0 |
| us-80060e3c | kyoshin/plugin/pluginCam | `setRotY` | setRotY | 0x80060E3C |
| us-80060ec8 | kyoshin/plugin/pluginCam | `setFov` | setFov | 0x80060EC8 |
| us-80060f9c | kyoshin/plugin/pluginCam | `setPosOfs` | setPosOfs | 0x80060F9C |
| us-8006110c | kyoshin/plugin/pluginCam | `setLookatOfs` | setLookatOfs | 0x8006110C |
| us-80061478 | kyoshin/plugin/pluginCam | `keyBegin` | keyBegin | 0x80061478 |
| us-800615e8 | kyoshin/plugin/pluginCam | `keyAdd` | keyAdd | 0x800615E8 |
| us-800618b0 | kyoshin/plugin/pluginCam | `shake` | shake | 0x800618B0 |
| us-80062730 | kyoshin/cf/CfRes | `CfRes_callFunc_67E78` | CfRes_callFunc_67E78 | 0x80062730 |
| us-80062758 | kyoshin/cf/CfRes | `CfRes_callFunc_67F10` | CfRes_callFunc_67F10 | 0x80062758 |
| us-80062ecc | kyoshin/cf/CfRes | `CfRes_tryRefreshSlot16C` | CfRes_tryRefreshSlot16C | 0x80062ECC |
| us-80064154 | kyoshin/cf/CfRes | `cfResNopValueSink` | cfResNopValueSink | 0x80064154 |
| us-800645fc | kyoshin/cf/CfRes | `CfRes_dispatchArchiveRead` | CfRes_dispatchArchiveRead | 0x800645FC |
| us-80065144 | kyoshin/cf/CfRes | `CfRes_delegateCleanup` | CfRes_delegateCleanup | 0x80065144 |
| us-80065344 | kyoshin/cf/CfRes | `CfRes_resolveGridToken` | CfRes_resolveGridToken | 0x80065344 |
| us-80065590 | kyoshin/cf/CfRes | `CfRes_reregisterTableEntry` | CfRes_reregisterTableEntry | 0x80065590 |
| us-8006565c | kyoshin/cf/CfRes | `CfRes_resolveTableToken` | CfRes_resolveTableToken | 0x8006565C |
| us-80065748 | kyoshin/cf/CfRes | `CfRes_resolveAndLinkB8` | CfRes_resolveAndLinkB8 | 0x80065748 |
| us-80065810 | kyoshin/cf/CfRes | `CfRes_resolveSlotB8` | CfRes_resolveSlotB8 | 0x80065810 |
| us-800658e8 | kyoshin/cf/CfRes | `CfRes_resolveSlotF4` | CfRes_resolveSlotF4 | 0x800658E8 |
| us-800659f0 | kyoshin/cf/CfRes | `CfRes_resolveSlot130` | CfRes_resolveSlot130 | 0x800659F0 |
| us-80065aec | kyoshin/cf/CfRes | `CfRes_updateSlot16C` | CfRes_updateSlot16C | 0x80065AEC |
| us-80065bac | kyoshin/cf/CfRes | `CfRes_resolveSlot16C` | CfRes_resolveSlot16C | 0x80065BAC |
| us-80065c7c | kyoshin/cf/CfRes | `CfRes_resolveSlot1E4` | CfRes_resolveSlot1E4 | 0x80065C7C |
| us-80065d68 | kyoshin/cf/CfRes | `CfRes_resolveSlot220` | CfRes_resolveSlot220 | 0x80065D68 |
| us-80065e38 | kyoshin/cf/CfRes | `CfRes_resolveSlotByBits` | CfRes_resolveSlotByBits | 0x80065E38 |
| us-80065f48 | kyoshin/cf/CfRes | `CfRes_resolveSlot298` | CfRes_resolveSlot298 | 0x80065F48 |
| us-80066628 | kyoshin/cf/CfRes | `CfRes_cancelPendingRead` | CfRes_cancelPendingRead | 0x80066628 |
| us-800668a8 | kyoshin/cf/IResInfo | `CfRes_resolveEventResource` | CfRes_resolveEventResource | 0x800668A8 |
| us-80066938 | kyoshin/cf/IResInfo | `CfRes_notifyAudioManager` | CfRes_notifyAudioManager | 0x80066938 |
| us-8006698c | kyoshin/cf/IResInfo | `CfRes_getTypeSlotValue` | CfRes_getTypeSlotValue | 0x8006698C |
| us-80066af4 | kyoshin/cf/IResInfo | `CfRes_getType10AudioSlot` | CfRes_getType10AudioSlot | 0x80066AF4 |
| us-80066c44 | kyoshin/cf/IResInfo | `CfRes_getVoiceSlotType9` | CfRes_getVoiceSlotType9 | 0x80066C44 |
| us-80066c68 | kyoshin/cf/IResInfo | `CfRes_cacheOrPublishSound` | CfRes_cacheOrPublishSound | 0x80066C68 |
| us-80066fcc | kyoshin/cf/IResInfo | `CfRes_releaseVoiceSlot` | CfRes_releaseVoiceSlot | 0x80066FCC |
| us-80066fe4 | kyoshin/cf/IResInfo | `CfRes_acquireSoundEntry` | CfRes_acquireSoundEntry | 0x80066FE4 |
| us-800670ec | kyoshin/cf/IResInfo | `CfRes_releaseCachedBase` | CfRes_releaseCachedBase | 0x800670EC |
| us-8006764c | kyoshin/cf/IResInfo | `CfRes_attachEntryData` | CfRes_attachEntryData | 0x8006764C |
| us-80068880 | kyoshin/cf/IResInfo | `CfRes_updateHighGrid` | CfRes_updateHighGrid | 0x80068880 |
| us-80068918 | kyoshin/cf/IResInfo | `CfRes_updateHighGridFiltered` | CfRes_updateHighGridFiltered | 0x80068918 |
| us-800689e8 | kyoshin/cf/IResInfo | `CfRes_updateHighGridFlagged` | CfRes_updateHighGridFlagged | 0x800689E8 |
| us-80068a80 | kyoshin/cf/IResInfo | `CfRes_reloadLoadedEntries` | CfRes_reloadLoadedEntries | 0x80068A80 |
| us-80068b18 | kyoshin/cf/IResInfo | `CfRes_updateLowGrid` | CfRes_updateLowGrid | 0x80068B18 |
| us-80068c5c | kyoshin/cf/IResInfo | `CfRes_updateFlaggedGrid` | CfRes_updateFlaggedGrid | 0x80068C5C |
| us-80069430 | kyoshin/cf/CfScript | `CfScript_InitPath` | CfScript_InitPath | 0x80069430 |
| us-80069534 | kyoshin/cf/CfScript | `CfScript_LoadSlot1` | CfScript_LoadSlot1 | 0x80069534 |
| us-80069570 | kyoshin/cf/CfScript | `CfScript_LoadSlot2` | CfScript_LoadSlot2 | 0x80069570 |
| us-800695ac | kyoshin/cf/CfScript | `CfScript_FlagSlot0` | CfScript_FlagSlot0 | 0x800695AC |
| us-800695e0 | kyoshin/cf/CfScript | `CfScript_FlagSlot1` | CfScript_FlagSlot1 | 0x800695E0 |
| us-80069614 | kyoshin/cf/CfScript | `CfScript_FlagSlot2` | CfScript_FlagSlot2 | 0x80069614 |
| us-80069648 | kyoshin/cf/CfScript | `CfScript_SleepReq2` | CfScript_SleepReq2 | 0x80069648 |
| us-8006968c | kyoshin/cf/CfScript | `CfScript_ClearReq0` | CfScript_ClearReq0 | 0x8006968C |
| us-80069724 | kyoshin/cf/CfScript | `CfScript_ClearReq1` | CfScript_ClearReq1 | 0x80069724 |
| us-800697bc | kyoshin/cf/CfScript | `CfScript_ClearReq2` | CfScript_ClearReq2 | 0x800697BC |
| us-800698ac | kyoshin/cf/CfScript | `CfScript_MakeName` | CfScript_MakeName | 0x800698AC |
| us-8006bd68 | kyoshin/cf/CfCamEvent | `__ct__8006B310` | constructor | 0x8006BD68 |
| us-8006c33c | kyoshin/cf/CfCam | `cfCam_loadTuneTbl` | cfCam_loadTuneTbl | 0x8006C33C |
| us-8006c3a0 | kyoshin/cf/CfCam | `cfCam_refreshBA8` | cfCam_refreshBA8 | 0x8006C3A0 |
| us-8006d564 | kyoshin/cf/CfCam | `cfCam_prepFollowVec` | cfCam_prepFollowVec | 0x8006D564 |
| us-8006dea8 | kyoshin/cf/CfCam | `cfCam_recomputeEyeTarget` | cfCam_recomputeEyeTarget | 0x8006DEA8 |
| us-8006f030 | kyoshin/cf/CfCam | `cfCam_pushStateToActive` | cfCam_pushStateToActive | 0x8006F030 |
| us-80070444 | kyoshin/cf/CfCam | `cfCam_syncFollowSnapshot` | cfCam_syncFollowSnapshot | 0x80070444 |
| us-8007215c | kyoshin/cf/CfCam | `cfCam_copyBlock16` | cfCam_copyBlock16 | 0x8007215C |
| us-8007cadc | kyoshin/cf/CfGameManager | `resetGameFlags__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetGameFlags() | 0x8007CADC |
| us-8007cea8 | kyoshin/cf/CfGameManager | `__dt__Q22cf13CfGameManagerFv` | cf::CfGameManager::~CfGameManager() | 0x8007CEA8 |
| us-8007cf54 | kyoshin/cf/CfGameManager | `resetManagerState__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetManagerState() | 0x8007CF54 |
| us-8007e1d0 | kyoshin/cf/CfGameManager | `notifyObjectMapChange__Q22cf13CfGameManagerFv` | cf::CfGameManager::notifyObjectMapChange() | 0x8007E1D0 |
| us-8007e868 | kyoshin/cf/CfGameManager | `syncBdatDataCache__Q22cf13CfGameManagerFv` | cf::CfGameManager::syncBdatDataCache() | 0x8007E868 |
| us-8007e9d4 | kyoshin/cf/CfGameManager | `syncFieldData__Q22cf13CfGameManagerFv` | cf::CfGameManager::syncFieldData() | 0x8007E9D4 |
| us-8007ebb4 | kyoshin/cf/CfGameManager | `teardownGameManager__Q22cf13CfGameManagerFv` | cf::CfGameManager::teardownGameManager() | 0x8007EBB4 |
| us-8007ee78 | kyoshin/cf/CfGameManager | `fadeOutGameEffects__Q22cf13CfGameManagerFv` | cf::CfGameManager::fadeOutGameEffects() | 0x8007EE78 |
| us-8007f2a4 | kyoshin/cf/CfGameManager | `initParticleSystem__Q22cf13CfGameManagerFv` | cf::CfGameManager::initParticleSystem() | 0x8007F2A4 |
| us-8007f88c | kyoshin/cf/CfGameManager | `loadBdatGroupData__Q22cf13CfGameManagerFv` | cf::CfGameManager::loadBdatGroupData() | 0x8007F88C |
| us-8007f8a0 | kyoshin/cf/CfGameManager | `getItemCountForGroup__Q22cf13CfGameManagerFv` | cf::CfGameManager::getItemCountForGroup() | 0x8007F8A0 |
| us-8007f9f0 | kyoshin/cf/CfGameManager | `getItemSlotData__Q22cf13CfGameManagerFv` | cf::CfGameManager::getItemSlotData() | 0x8007F9F0 |
| us-8007fa60 | kyoshin/cf/CfGameManager | `resetItemSlotA__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetItemSlotA() | 0x8007FA60 |
| us-8008059c | kyoshin/cf/CfGameManager | `refreshCameraIfCurrentView__Q22cf13CfGameManagerFv` | cf::CfGameManager::refreshCameraIfCurrentView() | 0x8008059C |
| us-800805cc | kyoshin/cf/CfGameManager | `resetCameraIfCurrentView__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetCameraIfCurrentView() | 0x800805CC |
| us-800807cc | kyoshin/cf/CfGameManager | `processEffectList__Q22cf13CfGameManagerFv` | cf::CfGameManager::processEffectList() | 0x800807CC |
| us-8008086c | kyoshin/cf/CfGameManager | `processEventList__Q22cf13CfGameManagerFv` | cf::CfGameManager::processEventList() | 0x8008086C |
| us-800817e4 | kyoshin/cf/CfGameManager | `loadMapObjectByName__Q22cf13CfGameManagerFv` | cf::CfGameManager::loadMapObjectByName() | 0x800817E4 |
| us-80081884 | kyoshin/cf/CfGameManager | `attachObjectText__Q22cf13CfGameManagerFv` | cf::CfGameManager::attachObjectText() | 0x80081884 |
| us-80082018 | kyoshin/cf/CfGameManager | `createPlayerEffectInstance__Q22cf13CfGameManagerFv` | cf::CfGameManager::createPlayerEffectInstance() | 0x80082018 |
| us-80082314 | kyoshin/cf/CfGameManager | `createItemObjectWrapper__Q22cf13CfGameManagerFv` | cf::CfGameManager::createItemObjectWrapper() | 0x80082314 |
| us-80082640 | kyoshin/cf/CfGameManager | `getOrCreateBdatObj__Q22cf13CfGameManagerFv` | cf::CfGameManager::getOrCreateBdatObj() | 0x80082640 |
| us-80082710 | kyoshin/cf/CfGameManager | `assignCameraTarget__Q22cf13CfGameManagerFv` | cf::CfGameManager::assignCameraTarget() | 0x80082710 |
| us-8008275c | kyoshin/cf/CfGameManager | `maybeCreateCameraMember__Q22cf13CfGameManagerFv` | cf::CfGameManager::maybeCreateCameraMember() | 0x8008275C |
| us-80082814 | kyoshin/cf/CfGameManager | `lookupEffectForResource__Q22cf13CfGameManagerFv` | cf::CfGameManager::lookupEffectForResource() | 0x80082814 |
| us-80082ab0 | kyoshin/cf/CfGameManager | `getCamManagerData__Q22cf13CfGameManagerFv` | cf::CfGameManager::getCamManagerData() | 0x80082AB0 |
| us-80082bd8 | kyoshin/cf/CfGameManager | `resetFieldState__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetFieldState() | 0x80082BD8 |
| us-80082c78 | kyoshin/cf/CfGameManager | `getQueuedFileEventCount__Q22cf13CfGameManagerFv` | cf::CfGameManager::getQueuedFileEventCount() | 0x80082C78 |
| us-80082c80 | kyoshin/cf/CfGameManager | `clearQueuedEvents__Q22cf13CfGameManagerFv` | cf::CfGameManager::clearQueuedEvents() | 0x80082C80 |
| us-80082cd8 | kyoshin/cf/CfGameManager | `getResourceFromTable__Q22cf13CfGameManagerFv` | cf::CfGameManager::getResourceFromTable() | 0x80082CD8 |
| us-80082ce0 | kyoshin/cf/CfGameManager | `isResourceFlagSet__Q22cf13CfGameManagerFv` | cf::CfGameManager::isResourceFlagSet() | 0x80082CE0 |
| us-80082d28 | kyoshin/cf/CfGameManager | `setResourceFlag__Q22cf13CfGameManagerFv` | cf::CfGameManager::setResourceFlag() | 0x80082D28 |
| us-80082e80 | kyoshin/cf/CfGameManager | `getTableValueByPair__Q22cf13CfGameManagerFv` | cf::CfGameManager::getTableValueByPair() | 0x80082E80 |
| us-80082eec | kyoshin/cf/CfGameManager | `addTableValueWithClamp__Q22cf13CfGameManagerFv` | cf::CfGameManager::addTableValueWithClamp() | 0x80082EEC |
| us-80082f98 | kyoshin/cf/CfGameManager | `isEventFlagActive__Q22cf13CfGameManagerFv` | cf::CfGameManager::isEventFlagActive() | 0x80082F98 |
| us-80082fa0 | kyoshin/cf/CfGameManager | `setEventFlag__Q22cf13CfGameManagerFv` | cf::CfGameManager::setEventFlag() | 0x80082FA0 |
| us-80083018 | kyoshin/cf/CfGameManager | `getEventValue40__Q22cf13CfGameManagerFv` | cf::CfGameManager::getEventValue40() | 0x80083018 |
| us-80083020 | kyoshin/cf/CfGameManager | `setEventManagerValue__Q22cf13CfGameManagerFv` | cf::CfGameManager::setEventManagerValue() | 0x80083020 |
| us-80083284 | kyoshin/cf/CfGameManager | `getEffectFlagState__Q22cf13CfGameManagerFv` | cf::CfGameManager::getEffectFlagState() | 0x80083284 |
| us-800832d0 | kyoshin/cf/CfGameManager | `setPresentationFlag__Q22cf13CfGameManagerFv` | cf::CfGameManager::setPresentationFlag() | 0x800832D0 |
| us-8008333c | kyoshin/cf/CfGameManager | `isSceneLoading__Q22cf13CfGameManagerFv` | cf::CfGameManager::isSceneLoading() | 0x8008333C |
| us-80083390 | kyoshin/cf/CfGameManager | `resetBattleGauge__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetBattleGauge() | 0x80083390 |
| us-800834bc | kyoshin/cf/CfGameManager | `processFieldEffects__Q22cf13CfGameManagerFv` | cf::CfGameManager::processFieldEffects() | 0x800834BC |
| us-800837d4 | kyoshin/cf/CfGameManager | `updatePlayerCameraLink__Q22cf13CfGameManagerFv` | cf::CfGameManager::updatePlayerCameraLink() | 0x800837D4 |
| us-80083844 | kyoshin/cf/CfGameManager | `cleanupPlayerEffectList__Q22cf13CfGameManagerFv` | cf::CfGameManager::cleanupPlayerEffectList() | 0x80083844 |
| us-80083848 | kyoshin/cf/CfGameManager | `isEffectListEmpty__Q22cf13CfGameManagerFv` | cf::CfGameManager::isEffectListEmpty() | 0x80083848 |
| us-80083968 | kyoshin/cf/CfGameManager | `isPlayerInEventRange__Q22cf13CfGameManagerFv` | cf::CfGameManager::isPlayerInEventRange() | 0x80083968 |
| us-80084244 | kyoshin/cf/CfGameManager | `notifyFieldChange__Q22cf13CfGameManagerFv` | cf::CfGameManager::notifyFieldChange() | 0x80084244 |
| us-8008472c | kyoshin/cf/CfGameManager | `queueSceneEventB__Q22cf13CfGameManagerFv` | cf::CfGameManager::queueSceneEventB() | 0x8008472C |
| us-800847a8 | kyoshin/cf/CfGameManager | `queueSceneEventC__Q22cf13CfGameManagerFv` | cf::CfGameManager::queueSceneEventC() | 0x800847A8 |
| us-80084860 | kyoshin/cf/CfGameManager | `resetPlayerEffectsA__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetPlayerEffectsA() | 0x80084860 |
| us-800848e4 | kyoshin/cf/CfGameManager | `resetPlayerEffectsB__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetPlayerEffectsB() | 0x800848E4 |
| us-800849e8 | kyoshin/cf/CfGameManager | `triggerPlayerEffects__Q22cf13CfGameManagerFv` | cf::CfGameManager::triggerPlayerEffects() | 0x800849E8 |
| us-80085490 | kyoshin/cf/CfGameManager | `processMapChange__Q22cf13CfGameManagerFv` | cf::CfGameManager::processMapChange() | 0x80085490 |
| us-80085524 | kyoshin/cf/CfGameManager | `isFieldTransitionReady__Q22cf13CfGameManagerFv` | cf::CfGameManager::isFieldTransitionReady() | 0x80085524 |
| us-80085568 | kyoshin/cf/CfGameManager | `isFieldBlockedByFlag__Q22cf13CfGameManagerFv` | cf::CfGameManager::isFieldBlockedByFlag() | 0x80085568 |
| us-8008590c | kyoshin/cf/CfGameManager | `resetBattlePresentation__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetBattlePresentation() | 0x8008590C |
| us-80086200 | kyoshin/cf/CfGameManager | `resetCameraManager__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetCameraManager() | 0x80086200 |
| us-80086208 | kyoshin/cf/CfGameManager | `isSceneReadyForInput__Q22cf13CfGameManagerFv` | cf::CfGameManager::isSceneReadyForInput() | 0x80086208 |
| us-80086224 | kyoshin/cf/CfGameManager | `isSceneActive__Q22cf13CfGameManagerFv` | cf::CfGameManager::isSceneActive() | 0x80086224 |
| us-80086280 | kyoshin/cf/CfGameManager | `updatePlayerContainers__Q22cf13CfGameManagerFv` | cf::CfGameManager::updatePlayerContainers() | 0x80086280 |
| us-80086dbc | kyoshin/cf/CfGameManager | `handleAreaChange__Q22cf13CfGameManagerFv` | cf::CfGameManager::handleAreaChange() | 0x80086DBC |
| us-80086fb0 | kyoshin/cf/CfGameManager | `processEventQueueB__Q22cf13CfGameManagerFv` | cf::CfGameManager::processEventQueueB() | 0x80086FB0 |
| us-80087520 | kyoshin/cf/CfGameManager | `updatePadState__Q22cf13CfGameManagerFv` | cf::CfGameManager::updatePadState() | 0x80087520 |
| us-80087770 | kyoshin/cf/CfGameManager | `getControllerValues__Q22cf13CfGameManagerFv` | cf::CfGameManager::getControllerValues() | 0x80087770 |
| us-80087778 | kyoshin/cf/CfGameManager | `getControllerWordA33C__Q22cf13CfGameManagerFv` | cf::CfGameManager::getControllerWordA33C() | 0x80087778 |
| us-8008777c | kyoshin/cf/CfGameManager | `getControllerWordA37C__Q22cf13CfGameManagerFv` | cf::CfGameManager::getControllerWordA37C() | 0x8008777C |
| us-80087780 | kyoshin/cf/CfGameManager | `getControllerWordA3BC__Q22cf13CfGameManagerFv` | cf::CfGameManager::getControllerWordA3BC() | 0x80087780 |
| us-80087784 | kyoshin/cf/CfGameManager | `resetControllerState__Q22cf13CfGameManagerFv` | cf::CfGameManager::resetControllerState() | 0x80087784 |
| us-80087788 | kyoshin/cf/CfGameManager | `clearControllerState__Q22cf13CfGameManagerFv` | cf::CfGameManager::clearControllerState() | 0x80087788 |
| us-80087c28 | kyoshin/cf/CfGameManager | `isVisionPackLoaded__Q22cf13CfGameManagerFv` | cf::CfGameManager::isVisionPackLoaded() | 0x80087C28 |
| us-80087c58 | kyoshin/cf/CfGameManager | `updateVisionState__Q22cf13CfGameManagerFv` | cf::CfGameManager::updateVisionState() | 0x80087C58 |
| us-8008aadc | kyoshin/cf/CtrlMoveEne | `__ct__8008A104` | constructor | 0x8008AADC |
| us-8008cec8 | kyoshin/cf/CtrlMoveEne | `CtrlMoveEne_TickMoveControl` | CtrlMoveEne_TickMoveControl | 0x8008CEC8 |
| us-8008de1c | kyoshin/cf/CtrlMoveEne | `CtrlMoveEne_ResetMoveState` | CtrlMoveEne_ResetMoveState | 0x8008DE1C |
| us-80094070 | kyoshin/cf/CtrlNpc | `__ct__CtrlNpc` | constructor | 0x80094070 |
| us-80095e28 | kyoshin/cf/CtrlNpc | `CtrlNpcUpdateMovement` | CtrlNpcUpdateMovement | 0x80095E28 |
| us-80097274 | kyoshin/cf/CtrlPc | `__ct__cf_CtrlPad` | constructor | 0x80097274 |
| us-8009864c | kyoshin/cf/CtrlPc | `func_80097C74` | func_80097C74 | 0x8009864C |
| us-800987d8 | kyoshin/cf/CtrlPc | `func_80097E00` | func_80097E00 | 0x800987D8 |
| us-8009906c | kyoshin/cf/CtrlPc | `func_80098694` | func_80098694 | 0x8009906C |
| us-800993dc | kyoshin/cf/CtrlPc | `func_80098A04` | func_80098A04 | 0x800993DC |
| us-800a43ec | kyoshin/code_800A3B24 | `VecMath_RandomAngleOffset` | VecMath_RandomAngleOffset | 0x800A43EC |
| us-800a9534 | kyoshin/code_800A75FC | `KyoshinHeap_Free38` | KyoshinHeap_Free38 | 0x800A9534 |
| us-800a9550 | kyoshin/code_800A75FC | `KyoshinHeap_Reset38` | KyoshinHeap_Reset38 | 0x800A9550 |
| us-800a955c | kyoshin/code_800A75FC | `KyoshinHeap_GetCount38` | KyoshinHeap_GetCount38 | 0x800A955C |
| us-800a9c10 | kyoshin/code_800A75FC | `KyoshinHeap_Free78` | KyoshinHeap_Free78 | 0x800A9C10 |
| us-800a9c2c | kyoshin/code_800A75FC | `KyoshinHeap_Reset78Stage` | KyoshinHeap_Reset78Stage | 0x800A9C2C |
| us-800a9d10 | kyoshin/code_800A75FC | `KyoshinHeap_Reset58Stage` | KyoshinHeap_Reset58Stage | 0x800A9D10 |
| us-800a9e00 | kyoshin/code_800A75FC | `KyoshinHeap_AllocFld10` | KyoshinHeap_AllocFld10 | 0x800A9E00 |
| us-800a9f28 | kyoshin/code_800A75FC | `KyoshinHeap_AllocFld14` | KyoshinHeap_AllocFld14 | 0x800A9F28 |
| us-800aa050 | kyoshin/code_800A75FC | `KyoshinHeap_AllocFld18` | KyoshinHeap_AllocFld18 | 0x800AA050 |
| us-800aa174 | kyoshin/code_800A75FC | `KyoshinHeap_AllocFld34` | KyoshinHeap_AllocFld34 | 0x800AA174 |
| us-800ab5f4 | kyoshin/cf/CfCollSphereImpl | `CfCollSphere_DebugDrawSimple` | CfCollSphere_DebugDrawSimple | 0x800AB5F4 |
| us-800ab660 | kyoshin/cf/CfCollSphereImpl | `CfCollSphere_DebugDrawXform` | CfCollSphere_DebugDrawXform | 0x800AB660 |
| us-800abcf0 | kyoshin/cf/object/CfObjectColl | `__dt__Q22cf12CfObjectCollFv` | cf::CfObjectColl::~CfObjectColl() | 0x800ABCF0 |
| us-800acbd8 | kyoshin/cf/object/CfObjectColl | `CollObjSetResIdPairName` | CollObjSetResIdPairName | 0x800ACBD8 |
| us-800ad02c | kyoshin/cf/object/CfObjectEff | `__dt__Q22cf8CfObjectFv` | cf::CfObject::~CfObject() | 0x800AD02C |
| us-800ad0dc | kyoshin/cf/object/CfObjectEff | `detachChildEf__Q22cf11CfObjectEffFv` | cf::CfObjectEff::detachChildEf() | 0x800AD0DC |
| us-800ad4f4 | kyoshin/cf/object/CfObjectEff | `setChildScl__` | setChildScl | 0x800AD4F4 |
| us-800ad844 | kyoshin/cf/object/CfObjectEff | `bindPartnerO_` | bindPartnerO_ | 0x800AD844 |
| us-800ad8a4 | kyoshin/cf/object/CfObjectEff | `setTargetObj_` | setTargetObj_ | 0x800AD8A4 |
| us-800ad92c | kyoshin/cf/object/CfObjectEff | `createEffect_` | createEffect_ | 0x800AD92C |
| us-800adc70 | kyoshin/cf/object/CfObjectEff | `updateEffect___Q22cf11CfObjectEffFv` | updateEffect() | 0x800ADC70 |
| us-800adf58 | kyoshin/cf/object/CfObjectEff | `teardownEff____Q22cf11CfObjectEffFv` | teardownEff() | 0x800ADF58 |
| us-800ae04c | kyoshin/cf/object/CfObjectEff | `__dt__Q22cf11CfObjectEffFv` | cf::CfObjectEff::~CfObjectEff() | 0x800AE04C |
| us-800ae11c | kyoshin/cf/object/CfObjectEff | `thunkTear90____Q22cf11CfObjectEffFv` | thunkTear90() | 0x800AE11C |
| us-800ae124 | kyoshin/cf/object/CfObjectEff | `thunkDtor90____Q22cf11CfObjectEffFv` | thunkDtor90() | 0x800AE124 |
| us-800ae154 | kyoshin/cf/object/CfObjectEne | `__ct__cf_CfObjectEne` | constructor | 0x800AE154 |
| us-800af534 | kyoshin/cf/object/CfObjectEne | `initEnemyBdatParams__Q22cf11CfObjectEneFv` | cf::CfObjectEne::initEnemyBdatParams() | 0x800AF534 |
| us-800b04d0 | kyoshin/cf/object/CfObjectEne | `CActorParam_UnkVirtualFunc148__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_UnkVirtualFunc148() | 0x800B04D0 |
| us-800b0a40 | kyoshin/cf/object/CfObjectEne | `CActorParam_UnkVirtualFunc103__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_UnkVirtualFunc103() | 0x800B0A40 |
| us-800b0d38 | kyoshin/cf/object/CfObjectEne | `CActorParam_UnkVirtualFunc42__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_UnkVirtualFunc42() | 0x800B0D38 |
| us-800b0d9c | kyoshin/cf/object/CfObjectEne | `CActorParam_addHp__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_addHp() | 0x800B0D9C |
| us-800b0e40 | kyoshin/cf/object/CfObjectEne | `CActorParam_UnkVirtualFunc31__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_UnkVirtualFunc31() | 0x800B0E40 |
| us-800b0f68 | kyoshin/cf/object/CfObjectEne | `destroyEnemyObject__Q22cf11CfObjectEneFv` | cf::CfObjectEne::destroyEnemyObject() | 0x800B0F68 |
| us-800b1614 | kyoshin/code_800B06A4 | `__dt__36_reslist_base<PQ22cf13IFactoryEvent>Fv` | _reslist_base<PQ22cf13IFactoryEvent>::~_reslist_base<PQ22cf13IFactoryEvent>() | 0x800B1614 |
| us-800b1e70 | kyoshin/code_800B06A4 | `teardownGameMgr` | teardownGameMgr | 0x800B1E70 |
| us-800b23f8 | kyoshin/code_800B06A4 | `resetSubManagers` | resetSubManagers | 0x800B23F8 |
| us-800b25a8 | kyoshin/code_800B06A4 | `syncPlayerSubMgr` | syncPlayerSubMgr | 0x800B25A8 |
| us-800b26e4 | kyoshin/code_800B06A4 | `notifyObjCA0` | notifyObjCA0 | 0x800B26E4 |
| us-800b26f8 | kyoshin/code_800B06A4 | `setMgrFixStrName` | setMgrFixStrName | 0x800B26F8 |
| us-800b2794 | kyoshin/code_800B06A4 | `runMgrTeardownSeq` | runMgrTeardownSeq | 0x800B2794 |
| us-800b28a4 | kyoshin/code_800B06A4 | `releasePadObjects` | releasePadObjects | 0x800B28A4 |
| us-800b559c | kyoshin/code_800B06A4 | `hudCtrlSlot46` | hudCtrlSlot46 | 0x800B559C |
| us-800b588c | kyoshin/code_800B06A4 | `setObj6Cbit0` | setObj6Cbit0 | 0x800B588C |
| us-800b6dd8 | kyoshin/code_800B06A4 | `getObjField98` | getObjField98 | 0x800B6DD8 |
| us-800b759c | kyoshin/code_800B06A4 | `getReslistC08` | getReslistC08 | 0x800B759C |
| us-800b7c40 | kyoshin/code_800B06A4 | `removeTboxById` | removeTboxById | 0x800B7C40 |
| us-800b8334 | kyoshin/code_800B06A4 | `triggerObjsByType` | triggerObjsByType | 0x800B8334 |
| us-800b92e8 | kyoshin/code_800B06A4 | `lookupCA0ById` | lookupCA0ById | 0x800B92E8 |
| us-800b9380 | kyoshin/code_800B06A4 | `lookupCA0By45C0` | lookupCA0By45C0 | 0x800B9380 |
| us-800b9418 | kyoshin/code_800B06A4 | `lookupCA0BySelf` | lookupCA0BySelf | 0x800B9418 |
| us-800b9fe0 | kyoshin/code_800B06A4 | `setObjField734` | setObjField734 | 0x800B9FE0 |
| us-800b9ffc | kyoshin/code_800B06A4 | `setObjField738` | setObjField738 | 0x800B9FFC |
| us-800ba2a0 | kyoshin/code_800B06A4 | `getObjField620` | getObjField620 | 0x800BA2A0 |
| us-800baf8c | kyoshin/cf/object/CfObjectMap | `setMapEffectFlag__Q22cf11CfObjectMapFv` | cf::CfObjectMap::setMapEffectFlag() | 0x800BAF8C |
| us-800bca00 | kyoshin/cf/object/CfObjectMove | `CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectModel_UnkVirtualFunc1() | 0x800BCA00 |
| us-800bcb40 | kyoshin/cf/object/CfObjectMove | `CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectModel_UnkVirtualFunc2() | 0x800BCB40 |
| us-800bcc04 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_detachModelList() | 0x800BCC04 |
| us-800bcd7c | kyoshin/cf/object/CfObjectMove | `CfObjectMove_detachMoveRegion` | CfObjectMove_detachMoveRegion | 0x800BCD7C |
| us-800bcdf0 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_testMoveProximity` | CfObjectMove_testMoveProximity | 0x800BCDF0 |
| us-800bd07c | kyoshin/cf/object/CfObjectMove | `CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObject_UnkVirtualFunc5() | 0x800BD07C |
| us-800be4d8 | kyoshin/cf/object/CfObjectMove | `CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObject_setMoveTargetVec() | 0x800BE4D8 |
| us-800be830 | kyoshin/cf/object/CfObjectMove | `CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv` | cf::CfObjectMove::CfObject_UnkVirtualFunc27(void*) | 0x800BE830 |
| us-800bea60 | kyoshin/cf/object/CfObjectMove | `CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CObjectParam_UnkVirtualFunc2() | 0x800BEA60 |
| us-800beb64 | kyoshin/cf/object/CfObjectMove | `CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObject_getMoveSpeedRate() | 0x800BEB64 |
| us-800bebb4 | kyoshin/cf/object/CfObjectMove | `CfObject_readRefreshValue__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObject_readRefreshValue() | 0x800BEBB4 |
| us-800bebd0 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_refreshNpcState() | 0x800BEBD0 |
| us-800bebe4 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_updateNpcTarget() | 0x800BEBE4 |
| us-800bebf8 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_forwardNpcSignal` | CfObjectMove_forwardNpcSignal | 0x800BEBF8 |
| us-800bec0c | kyoshin/cf/object/CfObjectMove | `CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_queryNpcAdvance() | 0x800BEC0C |
| us-800bec6c | kyoshin/cf/object/CfObjectMove | `CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_advanceNpcTarget() | 0x800BEC6C |
| us-800bec94 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_restartNpcTarget() | 0x800BEC94 |
| us-800bed54 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_setModelListLock` | CfObjectMove_setModelListLock | 0x800BED54 |
| us-800bede0 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_attachEffectSlot() | 0x800BEDE0 |
| us-800beee8 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_transferSlotBits() | 0x800BEEE8 |
| us-800bef60 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_flushEffectSlots() | 0x800BEF60 |
| us-800bf118 | kyoshin/cf/object/CfObjectMove | `CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectModel_UnkVirtualFunc18() | 0x800BF118 |
| us-800bf190 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_setRegionAttached` | CfObjectMove_setRegionAttached | 0x800BF190 |
| us-800bf1c4 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_loadResourceById() | 0x800BF1C4 |
| us-800bf204 | kyoshin/cf/object/CfObjectMove | `requestVoice__Q22cf8CfObjectFiUlff` | cf::CfObject::requestVoice(int, long, float, float) | 0x800BF204 |
| us-800bf290 | kyoshin/cf/object/CfObjectMove | `CfObjectMove_releaseVoiceHandle` | CfObjectMove_releaseVoiceHandle | 0x800BF290 |
| us-800bf3ac | kyoshin/cf/object/CfObjectMove | `CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObjectMove_releaseSlotById() | 0x800BF3AC |
| us-800bf494 | kyoshin/cf/object/CfObjectMove | `CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv` | cf::CfObjectMove::CfObject_UnkVirtualFunc66() | 0x800BF494 |
| us-800bfe00 | kyoshin/cf/object/CfObjectNpc | `__dt__Q22cf11CfObjectNpcFv` | cf::CfObjectNpc::~CfObjectNpc() | 0x800BFE00 |
| us-800bfe74 | kyoshin/cf/object/CfObjectNpc | `initNpcFlags__Q22cf11CfObjectNpcFv` | cf::CfObjectNpc::initNpcFlags() | 0x800BFE74 |
| us-800bfee4 | kyoshin/cf/object/CfObjectNpc | `updateNpcDialog__Q22cf11CfObjectNpcFv` | cf::CfObjectNpc::updateNpcDialog() | 0x800BFEE4 |
| us-800c0314 | kyoshin/cf/object/CfObjectNpc | `loadIconType__Q22cf11CfObjectNpcFv` | cf::CfObjectNpc::loadIconType() | 0x800C0314 |
| us-800c0368 | kyoshin/cf/object/CfObjectNpc | `getRltMeet__Q22cf11CfObjectNpcFv` | cf::CfObjectNpc::getRltMeet() | 0x800C0368 |
| us-800c06d8 | kyoshin/cf/object/CfObjectPc | `__ct__Q22cf10CfObjectPcFv` | cf::CfObjectPc::CfObjectPc() | 0x800C06D8 |
| us-800c0968 | kyoshin/cf/object/CfObjectPc | `initialize__Q22cf10CfObjectPcFv` | cf::CfObjectPc::initialize() | 0x800C0968 |
| us-800c0bbc | kyoshin/cf/object/CfObjectPc | `scanArtsEntries__Q22cf10CfObjectPcFv` | cf::CfObjectPc::scanArtsEntries() | 0x800C0BBC |
| us-800c0df0 | kyoshin/cf/object/CfObjectPc | `resetActionTable__Q22cf10CfObjectPcFv` | cf::CfObjectPc::resetActionTable() | 0x800C0DF0 |
| us-800c1c14 | kyoshin/cf/object/CfObjectPc | `handleMoveState__Q22cf10CfObjectPcFv` | cf::CfObjectPc::handleMoveState() | 0x800C1C14 |
| us-800c1c50 | kyoshin/cf/object/CfObjectPc | `CfObjectMove_attachEffectSlot__Q22cf10CfObjectPcFv` | cf::CfObjectPc::CfObjectMove_attachEffectSlot() | 0x800C1C50 |
| us-800c1c68 | kyoshin/cf/object/CfObjectPc | `finalizePcCleanup__Q22cf10CfObjectPcFv` | cf::CfObjectPc::finalizePcCleanup() | 0x800C1C68 |
| us-800c26f4 | kyoshin/cf/code_800C17DC | `CmText_ParseWithStaticProc` | CmText_ParseWithStaticProc | 0x800C26F4 |
| us-800c2a00 | kyoshin/cfsys/CfObjectImplWalker | `Walker_SyncBattleFrame` | Walker_SyncBattleFrame | 0x800C2A00 |
| us-800c36d8 | kyoshin/cfsys/CfObjectImplWalker | `Walker_DispatchBattleSub` | Walker_DispatchBattleSub | 0x800C36D8 |
| us-800c42c0 | kyoshin/cfsys/CfObjectImplWalker | `Walker_UpdatePartyBattle` | Walker_UpdatePartyBattle | 0x800C42C0 |
| us-800cb118 | kyoshin/cfsys/CfObjectImplNpc | `NpcImplInit` | NpcImplInit | 0x800CB118 |
| us-800cb1e8 | kyoshin/cfsys/CfObjectImplNpc | `NpcImplUpdate` | NpcImplUpdate | 0x800CB1E8 |
| us-800cb288 | kyoshin/cfsys/CfObjectImplNpc | `NpcImplNotifyEvent` | NpcImplNotifyEvent | 0x800CB288 |
| us-800cb398 | kyoshin/cfsys/CfObjectImplMove | `MoveImplUpdateState` | MoveImplUpdateState | 0x800CB398 |
| us-800cb478 | kyoshin/cfsys/CfObjectImplMove | `MoveImplResetFull` | MoveImplResetFull | 0x800CB478 |
| us-800cf11c | kyoshin/cfsys/CfObjectImplMove | `MoveImplResetStateDriver` | MoveImplResetStateDriver | 0x800CF11C |
| us-800d0ab4 | kyoshin/cfsys/CfObjectImplEne | `MoveImplEne_InitFieldsToken` | MoveImplEne_InitFieldsToken | 0x800D0AB4 |
| us-800d0b78 | kyoshin/cfsys/CfObjectImplEne | `MoveImplEne_ReleaseTokenReset` | MoveImplEne_ReleaseTokenReset | 0x800D0B78 |
| us-800d0bc4 | kyoshin/cfsys/CfObjectImplEne | `MoveImplEne_TickBattleAI` | MoveImplEne_TickBattleAI | 0x800D0BC4 |
| us-800d1b08 | kyoshin/cfsys/CfObjectImplEne | `MoveImplEne_MatchTokenRelease` | MoveImplEne_MatchTokenRelease | 0x800D1B08 |
| us-800d34cc | kyoshin/cf/CtrlAct | `CAttackParam_getArtsGaugeMax__Q22cf12CAttackParamFv` | cf::CAttackParam::CAttackParam_getArtsGaugeMax() | 0x800D34CC |
| us-800d8988 | kyoshin/cf/CBattleManager | `CBattleMan_ApplyStatusRate` | CBattleMan_ApplyStatusRate | 0x800D8988 |
| us-800d8fe4 | kyoshin/cf/CBattleManager | `__ct__Q22cf14CBattleManagerFv` | cf::CBattleManager::CBattleManager() | 0x800D8FE4 |
| us-800ff944 | kyoshin/CMainMenu | `Term__9CMainMenuFv` | CMainMenu::Term() | 0x800FF944 |
| us-800ffa34 | kyoshin/CMainMenu | `CMainMenu_Update` | CMainMenu_Update | 0x800FFA34 |
| us-800ffde8 | kyoshin/CMainMenu | `__ct__800FF300` | constructor(3, 0, 0) | 0x800FFDE8 |
| us-80100260 | kyoshin/CMainMenu | `isAnyMenuOpen__9CMainMenuFv` | CMainMenu::isAnyMenuOpen() | 0x80100260 |
| us-80100408 | kyoshin/CMainMenu | `CMainMenu_DispatchState` | CMainMenu_DispatchState | 0x80100408 |
| us-801023dc | kyoshin/CMainMenu | `CMainMenu_DispatchMenuState` | CMainMenu_DispatchMenuState | 0x801023DC |
| menu-arts-ctor | kyoshin/menu/CMenuArtsSelect | `__ct__CMenuArtsSelect` | CMenuArtsSelect::CMenuArtsSelect | 0x80102B08 |
| menu-arts-term | kyoshin/menu/CMenuArtsSelect | `Term__15CMenuArtsSelectFv` | CMenuArtsSelect::Term | 0x80103B20 |
| us-80109728 | kyoshin/menu/CMenuArtsSelect | `CMenuArtsSelect_workEventDtor` | CMenuArtsSelect_workEventDtor | 0x80109728 |
| us-80109738 | kyoshin/menu/CMenuArtsSelect | `CMenuArtsSelect_scnRenderDtor` | CMenuArtsSelect_scnRenderDtor | 0x80109738 |
| us-80109984 | kyoshin/menu/CMenuBattleDamage | `Init__17CMenuBattleDamageFv` | CMenuBattleDamage::Init() | 0x80109984 |
| us-80109b6c | kyoshin/menu/CMenuBattleDamage | `Term__17CMenuBattleDamageFv` | CMenuBattleDamage::Term() | 0x80109B6C |
| menu-bps-term | kyoshin/menu/CMenuBattlePlayerState | `Term__22CMenuBattlePlayerStateFv` | CMenuBattlePlayerState::Term | 0x8010CC5C |
| us-8010dbb0 | kyoshin/menu/CMenuBattlePlayerState | `BpsStateSelectSlotAnim` | BpsStateSelectSlotAnim | 0x8010DBB0 |
| us-8011074c | kyoshin/menu/CMenuEnemyState | `Term__15CMenuEnemyStateFv` | CMenuEnemyState::Term() | 0x8011074C |
| us-80114480 | kyoshin/menu/CMenuFade | `Term__9CMenuFadeFv` | CMenuFade::Term() | 0x80114480 |
| us-801149a0 | kyoshin/menu/CMenuKeyAssign | `Init__14CMenuKeyAssignFv` | CMenuKeyAssign::Init() | 0x801149A0 |
| us-80114a84 | kyoshin/menu/CMenuKeyAssign | `Term__14CMenuKeyAssignFv` | CMenuKeyAssign::Term() | 0x80114A84 |
| us-80118078 | kyoshin/CMiniMap | `OnFileEvent__8CMMClockFP10CEventFile` | CMMClock::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x80118078 |
| us-8011c5f4 | kyoshin/CMiniMap | `Term__13CMenuMiniMap2Fv` | CMenuMiniMap2::Term() | 0x8011C5F4 |
| us-8011d008 | kyoshin/menu/CMenuQuestLog | `__dt__13CMenuQuestLogFv` | CMenuQuestLog::~CMenuQuestLog() | 0x8011D008 |
| us-8011d5b0 | kyoshin/menu/CMenuQuestLog | `Move__13CMenuQuestLogFv` | CMenuQuestLog::Move() | 0x8011D5B0 |
| us-80123000 | kyoshin/CQuestWindow | `Term__12CQuestWindowFv` | CQuestWindow::Term() | 0x80123000 |
| us-80123130 | kyoshin/CQuestWindow | `installMoveHook` | installMoveHook | 0x80123130 |
| us-801231a4 | kyoshin/CQuestWindow | `updateWindow` | updateWindow | 0x801231A4 |
| us-8012355c | kyoshin/CQuestWindow | `cbRenderBefore__12CQuestWindowFv` | CQuestWindow::cbRenderBefore() | 0x8012355C |
| us-8012368c | kyoshin/CQuestWindow | `completeWindowOpen` | completeWindowOpen | 0x8012368C |
| us-801287f0 | kyoshin/CTagProcessor | `TagMeasureTextWidth` | TagMeasureTextWidth | 0x801287F0 |
| us-8012964c | kyoshin/CTagProcessor | `callInitTagProc` | callInitTagProc | 0x8012964C |
| us-80129738 | kyoshin/CTagProcessor | `TagWriterParseColonParam` | TagWriterParseColonParam | 0x80129738 |
| us-8012a158 | kyoshin/CTagProcessor | `TagWriterMeasureScratch` | TagWriterMeasureScratch | 0x8012A158 |
| us-8012c710 | kyoshin/CTalkWindow | `__ct__CTalkWindow` | constructor | 0x8012C710 |
| us-8012d2c4 | kyoshin/CTalkWindow | `Term__11CTalkWindowFv` | CTalkWindow::Term() | 0x8012D2C4 |
| us-8012d374 | kyoshin/CTalkWindow | `Move__11CTalkWindowFv` | CTalkWindow::Move() | 0x8012D374 |
| us-8012e968 | kyoshin/CTalkWindow | `sinit_8012DE98` | sinit_8012DE98 | 0x8012E968 |
| us-80130a38 | kyoshin/CUIBattleManager | `OnFileEvent__16CUIBattleManagerFP10CEventFile` | CUIBattleManager::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x80130A38 |
| us-80130d18 | kyoshin/CUICfManager | `CUICfManager_tryResetFreeSlot` | CUICfManager_tryResetFreeSlot | 0x80130D18 |
| uicf-term | kyoshin/CUICfManager | `Term__12CUICfManagerFv` | CUICfManager::Term | 0x80133200 |
| us-80134244 | kyoshin/CUICfManager | `CUICfManager_queueMoveBaseMenu` | CUICfManager_queueMoveBaseMenu | 0x80134244 |
| us-801344dc | kyoshin/CUICfManager | `CUICfManager_queueQuestLogMenu` | CUICfManager_queueQuestLogMenu | 0x801344DC |
| us-80134654 | kyoshin/CUICfManager | `CUICfManager_queueEventMenu` | CUICfManager_queueEventMenu | 0x80134654 |
| us-80134774 | kyoshin/CUICfManager | `CUICfManager_queueSkipTimerMenu` | CUICfManager_queueSkipTimerMenu | 0x80134774 |
| us-80134a1c | kyoshin/CUICfManager | `CUICfManager_queueFactoryMenu` | CUICfManager_queueFactoryMenu | 0x80134A1C |
| us-801352c0 | kyoshin/CUICfManager | `CUICfManager_queueKizunagramMenu` | CUICfManager_queueKizunagramMenu | 0x801352C0 |
| us-8013539c | kyoshin/CUICfManager | `CUICfManager_queuePassiveSkillMenu` | CUICfManager_queuePassiveSkillMenu | 0x8013539C |
| us-8013554c | kyoshin/CUICfManager | `CUICfManager_queueKizunaTalkMenu` | CUICfManager_queueKizunaTalkMenu | 0x8013554C |
| us-80135624 | kyoshin/CUICfManager | `CUICfManager_queueSaveMenu` | CUICfManager_queueSaveMenu | 0x80135624 |
| us-801357ec | kyoshin/CUICfManager | `CUICfManager_queueTutorialMenu` | CUICfManager_queueTutorialMenu | 0x801357EC |
| us-80135a00 | kyoshin/CUICfManager | `CUICfManager_queueTutorialListMenu` | CUICfManager_queueTutorialListMenu | 0x80135A00 |
| us-80135c98 | kyoshin/CUICfManager | `CUICfManager_queueGetItemMenuNarrowed` | CUICfManager_queueGetItemMenuNarrowed | 0x80135C98 |
| us-80135d78 | kyoshin/CUICfManager | `CUICfManager_queueGetItemMenuById` | CUICfManager_queueGetItemMenuById | 0x80135D78 |
| us-80135e54 | kyoshin/CUICfManager | `CUICfManager_queueMultiGetItemMenu` | CUICfManager_queueMultiGetItemMenu | 0x80135E54 |
| us-80136aa8 | kyoshin/CUICfManager | `dtorThunk58__12CUICfManagerFv` | CUICfManager::dtorThunk58() | 0x80136AA8 |
| us-80137ea4 | kyoshin/code_80135FDC | `AnimRewindFrame` | AnimRewindFrame | 0x80137EA4 |
| us-80138a54 | kyoshin/code_80135FDC | `code80135FDC_thunk_BFE8C` | code80135FDC_thunk_BFE8C | 0x80138A54 |
| us-8013a65c | kyoshin/code_80135FDC | `BlendFloatAvgScale` | BlendFloatAvgScale | 0x8013A65C |
| us-8013c698 | kyoshin/code_80135FDC | `AnimResetToFirst` | AnimResetToFirst | 0x8013C698 |
| us-8013c6e8 | kyoshin/code_80135FDC | `AnimJumpToLast` | AnimJumpToLast | 0x8013C6E8 |
| us-8013cdf0 | kyoshin/CUIWindowManager | `__dt__16CUIWindowManagerFv` | CUIWindowManager::~CUIWindowManager() | 0x8013CDF0 |
| window-mgr-term | kyoshin/CUIWindowManager | `Term__16CUIWindowManagerFv` | CUIWindowManager::Term | 0x8013D068 |
| us-8013da6c | kyoshin/CUIWindowManager | `UIWin_CreateTalkWin` | UIWin_CreateTalkWin | 0x8013DA6C |
| us-8013de38 | kyoshin/CUIWindowManager | `UIWin_CreateEveTalkWin` | UIWin_CreateEveTalkWin | 0x8013DE38 |
| us-8013df4c | kyoshin/CUIWindowManager | `UIWin_CreateSysWin0` | UIWin_CreateSysWin0 | 0x8013DF4C |
| us-8013e08c | kyoshin/CUIWindowManager | `UIWin_CreateSysWin1` | UIWin_CreateSysWin1 | 0x8013E08C |
| us-8013e2a4 | kyoshin/CUIWindowManager | `UIWin_CreateExtraWin` | UIWin_CreateExtraWin | 0x8013E2A4 |
| us-8013e37c | kyoshin/CUIWindowManager | `UIWin_Create25070Win` | UIWin_Create25070Win | 0x8013E37C |
| us-8013e464 | kyoshin/CUIWindowManager | `UIWin_CreateQuestWin` | UIWin_CreateQuestWin | 0x8013E464 |
| us-8013e570 | kyoshin/CUIWindowManager | `UIWin_CreateMenuUpdate` | UIWin_CreateMenuUpdate | 0x8013E570 |
| us-8013eb08 | kyoshin/CUIWindowManager | `UIWin_CreateShopWin` | UIWin_CreateShopWin | 0x8013EB08 |
| us-8013ece4 | kyoshin/CUIWindowManager | `UIWin_CreateItemMulti` | UIWin_CreateItemMulti | 0x8013ECE4 |
| us-8013ee28 | kyoshin/CUIWindowManager | `UIWin_CreateB4790Win` | UIWin_CreateB4790Win | 0x8013EE28 |
| us-8013f204 | kyoshin/CUIWindowManager | `UIWin_Create6F8B0Win` | UIWin_Create6F8B0Win | 0x8013F204 |
| us-8013f2e4 | kyoshin/CUIWindowManager | `UIWin_CreatePTChange` | UIWin_CreatePTChange | 0x8013F2E4 |
| us-8013fc48 | kyoshin/CUIWindowManager | `UIWin_FlagBufClear` | UIWin_FlagBufClear | 0x8013FC48 |
| us-8013fca4 | kyoshin/CUIWindowManager | `UIWin_FlagBufReset` | UIWin_FlagBufReset | 0x8013FCA4 |
| us-8013fd58 | kyoshin/CUIWindowManager | `UIWin_FlagBufCommit` | UIWin_FlagBufCommit | 0x8013FD58 |
| us-8013fdf4 | kyoshin/CUIWindowManager | `UIWin_FlagBufCanShow` | UIWin_FlagBufCanShow | 0x8013FDF4 |
| us-80141c74 | kyoshin/CUIWindowManager | `UIWin_QueryPageFlag` | UIWin_QueryPageFlag | 0x80141C74 |
| us-80141cd4 | kyoshin/CUIWindowManager | `UIWin_BuildFlagBuf` | UIWin_BuildFlagBuf | 0x80141CD4 |
| us-80142a78 | kyoshin/cf/CfBdat | `func_80142074` | func_80142074 | 0x80142A78 |
| us-801431f8 | kyoshin/menu/CMenuUpdate | `Init__11CMenuUpdateFv` | CMenuUpdate::Init() | 0x801431F8 |
| us-80143368 | kyoshin/menu/CMenuUpdate | `Term__11CMenuUpdateFv` | CMenuUpdate::Term() | 0x80143368 |
| us-8014497c | kyoshin/menu/CMenuUpdate | `MenuUpdate_CheckBootGates` | MenuUpdate_CheckBootGates | 0x8014497C |
| us-80145264 | kyoshin/menu/CMenuLandTelop | `Term__14CMenuLandTelopFv` | CMenuLandTelop::Term() | 0x80145264 |
| battlestate-ctor | kyoshin/cf/object/CBattleState | `__ct__Q22cf12CBattleStateFv` | CBattleState::CBattleState | 0x80146520 |
| us-80148efc | kyoshin/cf/object/CBattleState | `CBattleState_clearStatusId__Q22cf12CBattleStateFv` | cf::CBattleState::CBattleState_clearStatusId() | 0x80148EFC |
| us-8014a7c8 | kyoshin/menu/CMenuGetItem | `Term__12CMenuGetItemFv` | CMenuGetItem::Term() | 0x8014A7C8 |
| us-80154630 | kyoshin/cf/CArtsSet | `__ct__Q22cf9_sArtsSetFv` | cf::_sArtsSet::_sArtsSet() | 0x80154630 |
| us-80154998 | kyoshin/cf/CArtsParam | `CAttackParam_clearArtsRecord__Q22cf12CAttackParamFv` | cf::CAttackParam::clearArtsRecord() | 0x80154998 |
| us-80158490 | kyoshin/cf/CItem | `CItemBlock_getKindList` | CItemBlock_getKindList | 0x80158490 |
| us-801589d0 | kyoshin/cf/CItem | `CItem_resolveFamilyBdat` | CItem_resolveFamilyBdat | 0x801589D0 |
| us-8015e7fc | kyoshin/CCol6System | `Term__9CCol6HintFv` | CCol6Hint::Term() | 0x8015E7FC |
| us-8015fefc | kyoshin/CCol6System | `Term__11CCol6SystemFv` | CCol6System::Term() | 0x8015FEFC |
| us-80165610 | kyoshin/cf/CTaskREvent | `evtCalcStreamVolume` | evtCalcStreamVolume | 0x80165610 |
| us-80168858 | kyoshin/cf/CInfoCf | `Move__9CMenuItemFv` | CMenuItem::Move() | 0x80168858 |
| us-80169680 | kyoshin/cf/CTaskREvtSequence | `EvtSeqSetBgmGateFlag` | EvtSeqSetBgmGateFlag | 0x80169680 |
| us-8016980c | kyoshin/cf/CTaskREvtSequence | `EvtSeqBootSequence` | EvtSeqBootSequence | 0x8016980C |
| us-801698ac | kyoshin/cf/CTaskREvtSequence | `EvtSeqBeginFileRead` | EvtSeqBeginFileRead | 0x801698AC |
| us-8016a134 | kyoshin/cf/CTaskREvtSequence | `EvtSeqUpdateRealtimeEvents` | EvtSeqUpdateRealtimeEvents | 0x8016A134 |
| us-8016af68 | kyoshin/cf/CTaskREvtSequence | `EvtSeqFinishSequence` | EvtSeqFinishSequence | 0x8016AF68 |
| us-8016b684 | kyoshin/cf/CTaskREvtSequence | `Init__Q22cf17CTaskREvtSequenceFv` | cf::CTaskREvtSequence::Init() | 0x8016B684 |
| us-8016b6d0 | kyoshin/cf/CTaskREvtSequence | `Term__Q22cf17CTaskREvtSequenceFv` | cf::CTaskREvtSequence::Term() | 0x8016B6D0 |
| us-8016be5c | kyoshin/cf/CTaskREvtSequence | `EvtSeqOnFileEvent` | EvtSeqOnFileEvent | 0x8016BE5C |
| us-8016c030 | kyoshin/cf/CTaskREvtSequence | `EvtSeqFindSameNameType3` | EvtSeqFindSameNameType3 | 0x8016C030 |
| us-8016c0e4 | kyoshin/cf/CTaskREvtSequence | `EvtSeqIsSameNameType3Busy` | EvtSeqIsSameNameType3Busy | 0x8016C0E4 |
| us-8016c670 | kyoshin/cf/CTaskREvtSequence | `EvtSeqFindResFloat` | EvtSeqFindResFloat | 0x8016C670 |
| us-8016c890 | kyoshin/cf/CTaskREvtSequence | `EvtSeqFindResAddr` | EvtSeqFindResAddr | 0x8016C890 |
| us-8016ca74 | kyoshin/cf/CTaskREvtSequence | `EvtSeqCheckRegionStatus` | EvtSeqCheckRegionStatus | 0x8016CA74 |
| us-8016ce24 | kyoshin/cf/CTaskREvtSequence | `EvtSeqRefreshEventLod` | EvtSeqRefreshEventLod | 0x8016CE24 |
| us-8016d420 | kyoshin/cf/CTaskREvtSequence | `EvtSeqResolvePackedResId` | EvtSeqResolvePackedResId | 0x8016D420 |
| us-8016d5bc | kyoshin/cf/CTaskREvtSequence | `cbRenderBefore__Q22cf17CTaskREvtSequenceFv` | cf::CTaskREvtSequence::cbRenderBefore() | 0x8016D5BC |
| us-8016dce8 | kyoshin/cf/CfResObjImpl | `ResObj_ReleaseModels_C98C` | ResObj_ReleaseModels_C98C | 0x8016DCE8 |
| us-8016f144 | kyoshin/cf/CfResReloadImpl | `resetReloadState` | resetReloadState | 0x8016F144 |
| us-8016fd80 | kyoshin/cf/CfResReloadImpl | `getReloadDelay` | getReloadDelay | 0x8016FD80 |
| us-80172c14 | kyoshin/cf/object/CfObjectActor | `CActorParam_setHp__Q22cf13CfObjectActorFv` | cf::CfObjectActor::CActorParam_setHp() | 0x80172C14 |
| us-8017378c | kyoshin/cf/object/CfObjectActor | `CActorParam_resetArtsStatus__Q22cf13CfObjectActorFv` | cf::CfObjectActor::CActorParam_resetArtsStatus() | 0x8017378C |
| us-80173830 | kyoshin/cf/object/CfObjectActor | `CfObjectActor_getAdjustedFacing__Q22cf13CfObjectActorFv` | cf::CfObjectActor::CfObjectActor_getAdjustedFacing() | 0x80173830 |
| us-80173924 | kyoshin/cf/object/CfObjectActor | `CfObjectActor_pushRefreshValue__Q22cf13CfObjectActorFv` | cf::CfObjectActor::CfObjectActor_pushRefreshValue() | 0x80173924 |
| us-801744cc | kyoshin/realtimeevt/CREvtModel | `createEvtModelObj` | createEvtModelObj | 0x801744CC |
| us-8017cdb0 | kyoshin/cf/object/CActorParam | `CActorParam_UnkVirtualFunc175__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_UnkVirtualFunc175() | 0x8017CDB0 |
| us-8017d83c | kyoshin/cf/object/CActorParam | `CActorParam_applyStatusHeal__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_applyStatusHeal() | 0x8017D83C |
| us-8017e474 | kyoshin/cf/object/CActorParam | `CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_UnkVirtualFunc180() | 0x8017E474 |
| us-8017e644 | kyoshin/cf/object/CActorParam | `CActorParam_UnkVirtualFunc6__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_UnkVirtualFunc6() | 0x8017E644 |
| us-8017e9d4 | kyoshin/cf/object/CActorParam | `CActorParam_addArtsGauge__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_addArtsGauge() | 0x8017E9D4 |
| us-8017eab4 | kyoshin/cf/object/CActorParam | `CActorParam_resetGaugeBattleEntry__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_resetGaugeBattleEntry() | 0x8017EAB4 |
| us-8017eaec | kyoshin/cf/object/CActorParam | `CActorParam_resetSecondGaugeBattleEntry__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_resetSecondGaugeBattleEntry() | 0x8017EAEC |
| us-8017ed30 | kyoshin/cf/object/CActorParam | `CActorParam_addHate__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_addHate() | 0x8017ED30 |
| us-8017f410 | kyoshin/cf/object/CActorParam | `CActorParam_accumulateTension__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_accumulateTension() | 0x8017F410 |
| us-8017f67c | kyoshin/cf/object/CActorParam | `CActorParam_resetTensionState__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_resetTensionState() | 0x8017F67C |
| us-8017f858 | kyoshin/cf/object/CActorParam | `CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_UnkVirtualFunc158() | 0x8017F858 |
| us-8017fd2c | kyoshin/cf/object/CActorParam | `CActorParam_UnkVirtualFunc136__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_UnkVirtualFunc136() | 0x8017FD2C |
| us-8017fe00 | kyoshin/cf/object/CActorParam | `CActorParam_UnkVirtualFunc137__Q22cf11CActorParamFv` | cf::CActorParam::CActorParam_UnkVirtualFunc137() | 0x8017FE00 |
| us-80180168 | kyoshin/cf/object/CActorParam | `CBattleState_getLinkedActorId__Q22cf11CActorParamFv` | cf::CActorParam::CBattleState_getLinkedActorId() | 0x80180168 |
| us-80180180 | kyoshin/cf/object/CActorParam | `CBattleState_UnkVirtualFunc1__Q22cf11CActorParamFv` | cf::CActorParam::CBattleState_UnkVirtualFunc1() | 0x80180180 |
| us-80180754 | kyoshin/menu/CMenuZeal | `Term__9CMenuZealFv` | CMenuZeal::Term() | 0x80180754 |
| us-80181484 | kyoshin/realtimeevt/CREvtCamera | `__ct__80180088` | constructor | 0x80181484 |
| us-80184684 | kyoshin/realtimeevt/CREvtModelObj | `EvtObj_TickUpdate` | EvtObj_TickUpdate | 0x80184684 |
| us-80185aec | kyoshin/realtimeevt/CREvtModelPc | `EvtModelPc_UpdatePos` | EvtModelPc_UpdatePos | 0x80185AEC |
| us-801861d0 | kyoshin/realtimeevt/CREvtEffect | `EvtFx_RefreshChrAnim` | EvtFx_RefreshChrAnim | 0x801861D0 |
| us-80186b18 | kyoshin/realtimeevt/CREvtObj | `__ct__cf_CREvtObj` | constructor | 0x80186B18 |
| us-80186c90 | kyoshin/plugin/pluginBtl | `startObserve` | startObserve | 0x80186C90 |
| us-80186cbc | kyoshin/plugin/pluginBtl | `endObserve` | endObserve | 0x80186CBC |
| us-80186ce8 | kyoshin/plugin/pluginBtl | `defeatingCount` | defeatingCount | 0x80186CE8 |
| us-80186d58 | kyoshin/plugin/pluginBtl | `isEnd` | isEnd | 0x80186D58 |
| us-80186f58 | kyoshin/plugin/pluginBtl | `attackEne` | attackEne | 0x80186F58 |
| us-80187040 | kyoshin/plugin/pluginBtl | `selectTgt` | selectTgt | 0x80187040 |
| us-8018726c | kyoshin/plugin/pluginBtl | `isVoiceEvent` | isVoiceEvent | 0x8018726C |
| us-801872f0 | kyoshin/plugin/pluginBtl | `unlockMonadoArts` | unlockMonadoArts | 0x801872F0 |
| us-80187534 | kyoshin/plugin/pluginBtl | `breakVision` | breakVision | 0x80187534 |
| us-801875e8 | kyoshin/plugin/pluginBtl | `setPTG` | setPTG | 0x801875E8 |
| us-801876a8 | kyoshin/plugin/pluginBtl | `test` | test | 0x801876A8 |
| menu-ptgauge-init | kyoshin/menu/CMenuPTGauge | `Init__12CMenuPTGaugeFv` | CMenuPTGauge::Init | 0x801884E8 |
| us-8018b504 | kyoshin/menu/CMenuSelectShop | `Init__15CMenuSelectShopFv` | CMenuSelectShop::Init() | 0x8018B504 |
| us-8018b670 | kyoshin/menu/CMenuSelectShop | `Term__15CMenuSelectShopFv` | CMenuSelectShop::Term() | 0x8018B670 |
| us-8018b6e0 | kyoshin/menu/CMenuSelectShop | `Move__15CMenuSelectShopFv` | CMenuSelectShop::Move() | 0x8018B6E0 |
| us-8018eca4 | kyoshin/cf/CfResPcImpl | `CfResPcImpl_setResourceOpen` | CfResPcImpl_setResourceOpen | 0x8018ECA4 |
| us-80190460 | kyoshin/cf/CfResPcImpl | `CfResPcImpl_reloadStateMachine` | CfResPcImpl_reloadStateMachine | 0x80190460 |
| us-8019061c | kyoshin/cf/CfResPcImpl | `CfResPcImpl_loadFileEventArchive` | CfResPcImpl_loadFileEventArchive | 0x8019061C |
| us-80190660 | kyoshin/cf/CfResPcImpl | `CfResPcImpl_handleFileEventLoad` | CfResPcImpl_handleFileEventLoad | 0x80190660 |
| us-80190844 | kyoshin/cf/CfResPcImpl | `CfResPcImpl_reloadMapSection` | CfResPcImpl_reloadMapSection | 0x80190844 |
| us-80190b68 | kyoshin/cf/CfResPcImpl | `CfResPcImpl_loadFileEventTable` | CfResPcImpl_loadFileEventTable | 0x80190B68 |
| us-80190c84 | kyoshin/cf/CfResPcImpl | `CfResPcImpl_refreshMapAreaEvents` | CfResPcImpl_refreshMapAreaEvents | 0x80190C84 |
| us-80191708 | kyoshin/cf/code_8018F8D8 | `CfCmd_RefreshUI` | CfCmd_RefreshUI | 0x80191708 |
| us-80191750 | kyoshin/cf/code_8018F8D8 | `CfCmd_Fwd1FOrFlush` | CfCmd_Fwd1FOrFlush | 0x80191750 |
| us-801918e0 | kyoshin/cf/code_8018F8D8 | `CfCmd_OpenMenu12` | CfCmd_OpenMenu12 | 0x801918E0 |
| us-80191d58 | kyoshin/cf/code_8018F8D8 | `CfCmd_FlushUI` | CfCmd_FlushUI | 0x80191D58 |
| us-80191d84 | kyoshin/cf/code_8018F8D8 | `CfCmd_Cond338C8` | CfCmd_Cond338C8 | 0x80191D84 |
| us-80191e88 | kyoshin/cf/code_8018F8D8 | `CfCmd_ResolveSlot1E4` | CfCmd_ResolveSlot1E4 | 0x80191E88 |
| us-80193f5c | kyoshin/cf/code_8018F8D8 | `Term__12CMenuPTStateFv` | CMenuPTState::Term() | 0x80193F5C |
| us-80193fd4 | kyoshin/cf/code_8018F8D8 | `Move__12CMenuPTStateFv` | CMenuPTState::Move() | 0x80193FD4 |
| us-80194148 | kyoshin/cf/code_8018F8D8 | `cbRenderBefore__12CMenuPTStateFv` | CMenuPTState::cbRenderBefore() | 0x80194148 |
| us-8019432c | kyoshin/menu/CMenuPTState | `__ct__80192C10` | constructor | 0x8019432C |
| us-80194504 | kyoshin/cf/CPartsChange | `__ct__cf_CPartsChange` | constructor | 0x80194504 |
| us-8019459c | kyoshin/cf/CPartsChange | `CPartsChange_ResolveSlotNames` | CPartsChange_ResolveSlotNames | 0x8019459C |
| us-801946b0 | kyoshin/cf/CPartsChange | `CPartsChange_NotifySlotStates` | CPartsChange_NotifySlotStates | 0x801946B0 |
| us-801947bc | kyoshin/cf/CPartsChange | `CPartsChange_SyncSlotState` | CPartsChange_SyncSlotState | 0x801947BC |
| us-8019498c | kyoshin/cf/CPartsChange | `__ct__80193270` | constructor | 0x8019498C |
| us-80194ae8 | kyoshin/cf/CPartsChange | `__dt___reslist_base_cf_CfPartyInfo` | destructor | 0x80194AE8 |
| us-80194c54 | kyoshin/cf/CPartsChange | `__dt__80193538` | destructor | 0x80194C54 |
| us-80194d8c | kyoshin/cf/CPartsChange | `CPartsChange_GetActorTable` | CPartsChange_GetActorTable | 0x80194D8C |
| us-80194d94 | kyoshin/cf/CPartsChange | `CPartsChange_FireIdEffect` | CPartsChange_FireIdEffect | 0x80194D94 |
| us-80194f20 | kyoshin/cf/CPartsChange | `CPartsChange_GetLandmarkTable` | CPartsChange_GetLandmarkTable | 0x80194F20 |
| us-80194f28 | kyoshin/cf/CPartsChange | `CPartsChange_DispatchChangeList` | CPartsChange_DispatchChangeList | 0x80194F28 |
| us-80195098 | kyoshin/cf/CPartsChange | `CPartsChange_InitChangeRecord` | CPartsChange_InitChangeRecord | 0x80195098 |
| us-801951cc | kyoshin/cf/CPartsChange | `CPartsChange_FindActorById` | CPartsChange_FindActorById | 0x801951CC |
| us-80195390 | kyoshin/cf/CPartsChange | `CPartsChange_InitActorEntry` | CPartsChange_InitActorEntry | 0x80195390 |
| us-801953ec | kyoshin/cf/CPartsChange | `CPartsChange_FindActorByObj` | CPartsChange_FindActorByObj | 0x801953EC |
| us-80196868 | kyoshin/cf/CPartsChange | `CPartsChange_UpdateElemSpeeds` | CPartsChange_UpdateElemSpeeds | 0x80196868 |
| us-801969ac | kyoshin/cf/CPartsChange | `CPartsChange_IsSubStateClear` | CPartsChange_IsSubStateClear | 0x801969AC |
| us-801969e8 | kyoshin/cf/CPartsChange | `CPartsChange_UpdateCollectionState` | CPartsChange_UpdateCollectionState | 0x801969E8 |
| us-80196aa0 | kyoshin/cf/CPartsChange | `CPartsChange_CheckNameEntry` | CPartsChange_CheckNameEntry | 0x80196AA0 |
| us-80197220 | kyoshin/cf/CPartsChange | `CPartsChange_SpawnById` | CPartsChange_SpawnById | 0x80197220 |
| us-801983b0 | kyoshin/cf/CPartsChange | `CPartsChange_CopyObjFields` | CPartsChange_CopyObjFields | 0x801983B0 |
| us-80198b98 | kyoshin/cf/CPartsChange | `CPartsChange_FindEntryById` | CPartsChange_FindEntryById | 0x80198B98 |
| us-80198be8 | kyoshin/cf/CPartsChange | `CPartsChange_FindPartsElem` | CPartsChange_FindPartsElem | 0x80198BE8 |
| us-801991bc | kyoshin/cf/CPartsChange | `CPartsChange_ResolveActorEntry` | CPartsChange_ResolveActorEntry | 0x801991BC |
| us-80199244 | kyoshin/cf/CPartsChange | `CPartsChange_CountListNodes` | CPartsChange_CountListNodes | 0x80199244 |
| us-80199268 | kyoshin/cf/CPartsChange | `CPartsChange_TeardownContainer` | CPartsChange_TeardownContainer | 0x80199268 |
| us-801992c0 | kyoshin/cf/CPartsChange | `CPartsChange_ResetBattleEntry` | CPartsChange_ResetBattleEntry | 0x801992C0 |
| us-80199388 | kyoshin/cf/CPartsChange | `CPartsChange_SpawnItemDrop` | CPartsChange_SpawnItemDrop | 0x80199388 |
| us-80199854 | kyoshin/cf/CPartsChange | `CPartsChange_FindFreeSlot` | CPartsChange_FindFreeSlot | 0x80199854 |
| us-801999a0 | kyoshin/cf/CPartsChange | `CPartsChange_UnregisterEntry` | CPartsChange_UnregisterEntry | 0x801999A0 |
| us-80199a2c | kyoshin/cf/CPartsChange | `CPartsChange_ResolveLinkedObj` | CPartsChange_ResolveLinkedObj | 0x80199A2C |
| us-80199a5c | kyoshin/cf/CPartsChange | `CPartsChange_HasAnyEntry` | CPartsChange_HasAnyEntry | 0x80199A5C |
| us-80199b1c | kyoshin/cf/CPartsChange | `CPartsChange_FindVoiceIndex` | CPartsChange_FindVoiceIndex | 0x80199B1C |
| us-80199c0c | kyoshin/cf/CPartsChange | `CPartsChange_GetEnemySlotAt` | CPartsChange_GetEnemySlotAt | 0x80199C0C |
| us-80199de8 | kyoshin/cf/CPartsChange | `CPartsChange_RemoveListNode` | CPartsChange_RemoveListNode | 0x80199DE8 |
| us-80199e88 | kyoshin/cf/CPartsChange | `CPartsChange_ProcessPartyInfo` | CPartsChange_ProcessPartyInfo | 0x80199E88 |
| us-80199ec0 | kyoshin/cf/CPartsChange | `CPartsChange_ProbePartyCollisions` | CPartsChange_ProbePartyCollisions | 0x80199EC0 |
| us-8019a1e0 | kyoshin/cf/CPartsChange | `CPartsChange_CopyHeaderAndLoad` | CPartsChange_CopyHeaderAndLoad | 0x8019A1E0 |
| us-8019a340 | kyoshin/cf/CPartsChange | `CPartsChange_ComputeSpawnPoseA` | CPartsChange_ComputeSpawnPoseA | 0x8019A340 |
| us-8019a528 | kyoshin/cf/CPartsChange | `CPartsChange_ComputeSpawnPoseB` | CPartsChange_ComputeSpawnPoseB | 0x8019A528 |
| us-8019a604 | kyoshin/cf/CPartsChange | `CPartsChange_ComputeSpawnPoseC` | CPartsChange_ComputeSpawnPoseC | 0x8019A604 |
| us-8019a6e0 | kyoshin/cf/CPartsChange | `CPartsChange_ComputeSpawnPoseD` | CPartsChange_ComputeSpawnPoseD | 0x8019A6E0 |
| us-8019a948 | kyoshin/cf/CPartsChange | `CPartsChange_ComputeSpawnPoseFull` | CPartsChange_ComputeSpawnPoseFull | 0x8019A948 |
| us-8019aae0 | kyoshin/cf/CtrlMovePC | `__ct__801993C4` | constructor | 0x8019AAE0 |
| us-8019ad94 | kyoshin/cf/CtrlMovePC | `movePcCondFullReset` | movePcCondFullReset | 0x8019AD94 |
| us-8019af2c | kyoshin/cf/CtrlMovePC | `movePcResetAndSetPos` | movePcResetAndSetPos | 0x8019AF2C |
| menu-battlemode-init | kyoshin/menu/CMenuBattleMode | `Init__15CMenuBattleModeFv` | CMenuBattleMode::Init | 0x801A15E0 |
| us-801a1714 | kyoshin/menu/CMenuBattleMode | `Term__15CMenuBattleModeFv` | CMenuBattleMode::Term() | 0x801A1714 |
| us-801a1c9c | kyoshin/COccCulling | `__dt__11COccCullingFv` | COccCulling::~COccCulling() | 0x801A1C9C |
| us-801a1e14 | kyoshin/COccCulling | `createHeap__11COccCullingFUli` | COccCulling::createHeap(long, int) | 0x801A1E14 |
| us-801a30f4 | kyoshin/CSimpleEveTalkWin | `Init__17CSimpleEveTalkWinFv` | CSimpleEveTalkWin::Init() | 0x801A30F4 |
| us-801a35d8 | kyoshin/CSimpleEveTalkWin | `Move__17CSimpleEveTalkWinFv` | CSimpleEveTalkWin::Move() | 0x801A35D8 |
| us-801a38b0 | kyoshin/CSimpleEveTalkWin | `eveTalkWinSetReuseText` | eveTalkWinSetReuseText | 0x801A38B0 |
| us-801a3d44 | kyoshin/CSimpleEveTalkWin | `eveTalkWinAdvancePage` | eveTalkWinAdvancePage | 0x801A3D44 |
| us-801a40ec | kyoshin/cf/CTaskCulling | `__ct__Q22cf12CTaskCullingFP4CScn` | cf::CTaskCulling::CTaskCulling(...*, 4, C, S, char, n) | 0x801A40EC |
| us-801a4560 | kyoshin/cf/CTaskCulling | `Init__Q22cf12CTaskCullingFv` | cf::CTaskCulling::Init() | 0x801A4560 |
| us-801a45c0 | kyoshin/cf/CTaskCulling | `Term__Q22cf12CTaskCullingFv` | cf::CTaskCulling::Term() | 0x801A45C0 |
| us-801a4758 | kyoshin/cf/CTaskCulling | `__sinit_\CTaskCulling_cpp` | __sinit_\CTaskCulling_cpp | 0x801A4758 |
| us-801addfc | kyoshin/menu/CMenuVision | `Term__11CMenuVisionFv` | CMenuVision::Term() | 0x801ADDFC |
| us-801adeb0 | kyoshin/menu/CMenuVision | `Move__11CMenuVisionFv` | CMenuVision::Move() | 0x801ADEB0 |
| us-801b3e08 | kyoshin/cf/CPcEffect07 | `thunk_adj4_reset` | thunk_adj4_reset | 0x801B3E08 |
| us-801b3e10 | kyoshin/cf/CPcEffect07 | `thunk_adj4_update` | thunk_adj4_update | 0x801B3E10 |
| us-801b3e18 | kyoshin/cf/CPcEffect07 | `fwdPcEffect20C8` | fwdPcEffect20C8 | 0x801B3E18 |
| us-801b583c | kyoshin/menu/CMenuGetItemMulti | `Term__17CMenuGetItemMultiFv` | CMenuGetItemMulti::Term() | 0x801B583C |
| us-801b5998 | kyoshin/menu/CMenuGetItemMulti | `Move__17CMenuGetItemMultiFv` | CMenuGetItemMulti::Move() | 0x801B5998 |
| us-801b82d8 | kyoshin/menu/CMenuGetItemMulti | `GetItemMulti_ShowEntryDetailPanes` | GetItemMulti_ShowEntryDetailPanes | 0x801B82D8 |
| us-801bba24 | kyoshin/menu/CMenuGetItemMulti | `OnFileEvent__17CMenuGetItemMultiFP10CEventFile` | CMenuGetItemMulti::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x801BBA24 |
| us-801bf67c | kyoshin/menu/CMenuKizunaTalk | `OnFileEvent__15CMenuKizunaTalkFP10CEventFile` | CMenuKizunaTalk::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x801BF67C |
| us-801c1064 | kyoshin/cf/CfSoundMan | `__ct__801BF76C` | constructor(7, 6, C) | 0x801C1064 |
| us-801c142c | kyoshin/cf/CfSoundMan | `CfSoundMan_PlayRequest` | CfSoundMan_PlayRequest | 0x801C142C |
| us-801c1530 | kyoshin/cf/CfSoundMan | `playActorSound__Q22cf10CfSoundManFUlUlUlUlf` | cf::CfSoundMan::playActorSound(long, long, long, long, float) | 0x801C1530 |
| us-801c1948 | kyoshin/cf/CfSoundMan | `getMasterVolume__Fv` | getMasterVolume() | 0x801C1948 |
| us-801c3200 | kyoshin/cf/CfSoundMan | `sinit_801C183C` | sinit_801C183C | 0x801C3200 |
| us-801c33b4 | kyoshin/cf/CfPadTask | `__dt__Q22cf9CfPadTaskFv` | cf::CfPadTask::~CfPadTask() | 0x801C33B4 |
| us-801c3558 | kyoshin/cf/CfPadTask | `setInputDisableTime__Q22cf9CfPadTaskFf` | cf::CfPadTask::setInputDisableTime(float) | 0x801C3558 |
| pad-move | kyoshin/cf/CfPadTask | `Move__Q22cf9CfPadTaskFv` | CfPadTask::Move | 0x801C392C |
| pad-update | kyoshin/cf/CfPadTask | `update__Q22cf9CfPadTaskFv` | CfPadTask::update | 0x801C3AC0 |
| us-801c4288 | kyoshin/cf/CfPadTask | `create__Q22cf9CfPadTaskFP8CProcess` | cf::CfPadTask::create(...*, 8, C, ...*, r, o, char, e, short, short) | 0x801C4288 |
| us-801c473c | kyoshin/cf/code_801C2C14 | `__dt__Q34nw4r3snd15FxReverbStdDpl2Fv` | destructor() | 0x801C473C |
| us-801c47f8 | kyoshin/cf/code_801C2C14 | `__dt__Q34nw4r3snd12FxChorusDpl2Fv` | destructor() | 0x801C47F8 |
| us-801c4874 | kyoshin/cf/code_801C2C14 | `__dt__Q34nw4r3snd11FxDelayDpl2Fv` | destructor() | 0x801C4874 |
| us-801c5190 | kyoshin/realtimeevt/CREvtLight | `CREvtLight_DistToTarget` | CREvtLight_DistToTarget | 0x801C5190 |
| us-801c5214 | kyoshin/realtimeevt/CREvtLight | `CREvtLight_AngleState` | CREvtLight_AngleState | 0x801C5214 |
| us-801c53e8 | kyoshin/CBgTex | `BgTex_SetupRegion_3A24` | BgTex_SetupRegion_3A24 | 0x801C53E8 |
| us-801c5760 | kyoshin/CBgTex | `BgTex_Release_3D9C` | BgTex_Release_3D9C | 0x801C5760 |
| us-801c581c | kyoshin/CBgTex | `OnFileEvent__6CBgTexFP10CEventFile` | CBgTex::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x801C581C |
| us-801c5968 | kyoshin/CTitleAHelp | `CTitleAHelp_load` | CTitleAHelp_load | 0x801C5968 |
| us-801c6140 | kyoshin/CTitleAHelp | `stepOpenAnim` | stepOpenAnim | 0x801C6140 |
| us-801c61bc | kyoshin/CTitleAHelp | `stepShownAnim` | stepShownAnim | 0x801C61BC |
| us-801c6210 | kyoshin/CTitleAHelp | `stepCloseAnim` | stepCloseAnim | 0x801C6210 |
| us-801c62a4 | kyoshin/CTitleAHelp | `stepHiddenAnim` | stepHiddenAnim | 0x801C62A4 |
| us-801c62f0 | kyoshin/CTitleAHelp | `OnFileEvent__11CTitleAHelpFP10CEventFile` | CTitleAHelp::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x801C62F0 |
| us-801c7034 | kyoshin/CItemBoxGrid | `__dt__801C5670` | destructor | 0x801C7034 |
| us-801c7820 | kyoshin/CItemBoxGrid | `CalcGridRowCount` | CalcGridRowCount | 0x801C7820 |
| us-801cc3b8 | kyoshin/CItemBoxGrid | `__dt__12CItemBoxGridFv` | CItemBoxGrid::~CItemBoxGrid() | 0x801CC3B8 |
| us-801cc61c | kyoshin/CItemBoxGrid | `UpdateItemBox` | UpdateItemBox | 0x801CC61C |
| us-801cde48 | kyoshin/CItemBoxGrid | `NextGridPage` | NextGridPage | 0x801CDE48 |
| us-801cdf3c | kyoshin/CItemBoxGrid | `PrevGridPage` | PrevGridPage | 0x801CDF3C |
| us-801d14a4 | kyoshin/CItemBoxGrid | `SetTabHeader` | SetTabHeader | 0x801D14A4 |
| us-801d1974 | kyoshin/CItemBoxGrid | `RebuildGridCat` | RebuildGridCat | 0x801D1974 |
| us-801d2c6c | kyoshin/CItemBoxGrid | `GetCatLabelMsg` | GetCatLabelMsg | 0x801D2C6C |
| us-801d3b90 | kyoshin/CCur | `setRootPaneTranslate__8CBaseCurFv` | CBaseCur::setRootPaneTranslate() | 0x801D3B90 |
| us-801d3f34 | kyoshin/CCur | `Cur_SetTwoPanes09` | Cur_SetTwoPanes09 | 0x801D3F34 |
| us-801d46cc | kyoshin/CCur | `Cur_PlacePaneByIndex` | Cur_PlacePaneByIndex | 0x801D46CC |
| us-801d5358 | kyoshin/CSortMenu | `sortMenuState2WaitAnim` | sortMenuState2WaitAnim | 0x801D5358 |
| us-801d5438 | kyoshin/CSortMenu | `sortMenuState5Closing` | sortMenuState5Closing | 0x801D5438 |
| us-801d68bc | kyoshin/CItemBoxInfo | `closeItemBoxPhase2` | closeItemBoxPhase2 | 0x801D68BC |
| us-801e33d4 | kyoshin/CItemBoxInfo | `openItemBox2Phase2` | openItemBox2Phase2 | 0x801E33D4 |
| us-801e349c | kyoshin/CItemBoxInfo | `closeItemBox2Phase2` | closeItemBox2Phase2 | 0x801E349C |
| us-801ed304 | kyoshin/CNumSelect | `OnFileEvent__10CNumSelectFP10CEventFile` | CNumSelect::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x801ED304 |
| us-801eefd8 | kyoshin/CItemBoxLine | `ItemBoxLine_LoadFiles` | ItemBoxLine_LoadFiles | 0x801EEFD8 |
| us-801ef1b8 | kyoshin/CItemBoxLine | `ItemBoxLine_DrawLayout` | ItemBoxLine_DrawLayout | 0x801EF1B8 |
| us-801ef430 | kyoshin/CItemBoxLine | `ItemBoxLine_GetSelectReady` | ItemBoxLine_GetSelectReady | 0x801EF430 |
| us-801ef4c4 | kyoshin/CItemBoxLine | `ItemBoxLine_IsBusy` | ItemBoxLine_IsBusy | 0x801EF4C4 |
| us-801ef638 | kyoshin/CItemBoxLine | `ItemBoxLine_LeaveState3` | ItemBoxLine_LeaveState3 | 0x801EF638 |
| us-801ef728 | kyoshin/CItemBoxLine | `ItemBoxLine_TabNext` | ItemBoxLine_TabNext | 0x801EF728 |
| us-801efee4 | kyoshin/CItemBoxLine | `ItemBoxLine_CursorPageUp` | ItemBoxLine_CursorPageUp | 0x801EFEE4 |
| us-801f0104 | kyoshin/CItemBoxLine | `ItemBoxLine_CursorPageDownRow` | ItemBoxLine_CursorPageDownRow | 0x801F0104 |
| us-801f0340 | kyoshin/CItemBoxLine | `ItemBoxLine_ConfirmOverlayOrHint` | ItemBoxLine_ConfirmOverlayOrHint | 0x801F0340 |
| us-801f099c | kyoshin/CItemBoxLine | `ItemBoxLine_ResetCursorB8` | ItemBoxLine_ResetCursorB8 | 0x801F099C |
| us-801f0d0c | kyoshin/CItemBoxLine | `ItemBoxLine_RefreshTabSelect` | ItemBoxLine_RefreshTabSelect | 0x801F0D0C |
| us-801f0da8 | kyoshin/CItemBoxLine | `ItemBoxLine_ResolveFocusId` | ItemBoxLine_ResolveFocusId | 0x801F0DA8 |
| us-801f0f1c | kyoshin/CItemBoxLine | `ItemBoxLine_EnterState1` | ItemBoxLine_EnterState1 | 0x801F0F1C |
| us-801f1034 | kyoshin/CItemBoxLine | `ItemBoxLine_EnterState3` | ItemBoxLine_EnterState3 | 0x801F1034 |
| us-801f10a4 | kyoshin/CItemBoxLine | `ItemBoxLine_PushSyswinCursor` | ItemBoxLine_PushSyswinCursor | 0x801F10A4 |
| us-801f1118 | kyoshin/CItemBoxLine | `ItemBoxLine_CommitOverlayState` | ItemBoxLine_CommitOverlayState | 0x801F1118 |
| us-801f1b28 | kyoshin/CItemBoxLine | `ItemBoxLine_RebuildTabList` | ItemBoxLine_RebuildTabList | 0x801F1B28 |
| us-801f22d8 | kyoshin/CItemBoxLine | `ItemBoxLine_RefreshTabLabels` | ItemBoxLine_RefreshTabLabels | 0x801F22D8 |
| us-801f23d8 | kyoshin/CItemBoxLine | `ItemBoxLine_RefreshLineRows` | ItemBoxLine_RefreshLineRows | 0x801F23D8 |
| us-801f2570 | kyoshin/CItemBoxLine | `ItemBoxLine_SetInfo2Item` | ItemBoxLine_SetInfo2Item | 0x801F2570 |
| us-801f2714 | kyoshin/CItemBoxLine | `ItemBoxLine_ResetPageTables` | ItemBoxLine_ResetPageTables | 0x801F2714 |
| us-801f4fa8 | kyoshin/CItemBoxLine | `sinit_801F32EC` | sinit_801F32EC | 0x801F4FA8 |
| us-801f51fc | kyoshin/CScrollBar | `CScrollBar_UpdateDispatch` | CScrollBar_UpdateDispatch | 0x801F51FC |
| us-801f5298 | kyoshin/CScrollBar | `CScrollBar_Teardown` | CScrollBar_Teardown | 0x801F5298 |
| us-801f532c | kyoshin/CScrollBar | `CScrollBar_InitRootPane` | CScrollBar_InitRootPane | 0x801F532C |
| us-801f5378 | kyoshin/CScrollBar | `CScrollBar_UpdateThumb` | CScrollBar_UpdateThumb | 0x801F5378 |
| us-801f550c | kyoshin/CScrollBar | `CScrollBar_PlaceThumb` | CScrollBar_PlaceThumb | 0x801F550C |
| us-801f568c | kyoshin/CScrollBar | `OnFileEvent__10CScrollBarFP10CEventFile` | CScrollBar::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x801F568C |
| us-801f69a0 | kyoshin/code_801F3BE0 | `GimDeactivateLocks` | GimDeactivateLocks | 0x801F69A0 |
| us-801f6b94 | kyoshin/code_801F3BE0 | `GimProbeLockAccept` | GimProbeLockAccept | 0x801F6B94 |
| us-801fa970 | kyoshin/cf/object/CfObjectTbox | `__ct__Q22cf12CfObjectTboxFv` | cf::CfObjectTbox::CfObjectTbox() | 0x801FA970 |
| us-801faa18 | kyoshin/cf/object/CfObjectTbox | `__dt__Q22cf12CfObjectTboxFv` | cf::CfObjectTbox::~CfObjectTbox() | 0x801FAA18 |
| us-801faa8c | kyoshin/cf/object/CfObjectTbox | `initTbox__Q22cf12CfObjectTboxFv` | cf::CfObjectTbox::initTbox() | 0x801FAA8C |
| us-801fab6c | kyoshin/cf/CfResTboxImpl | `ResTbox_OpenResource` | ResTbox_OpenResource | 0x801FAB6C |
| us-801fbf10 | kyoshin/CPartyStateWin | `PartyStateWin_TeardownWindow` | PartyStateWin_TeardownWindow | 0x801FBF10 |
| us-801fbff4 | kyoshin/CPartyStateWin | `PartyStateWin_FrameStep` | PartyStateWin_FrameStep | 0x801FBFF4 |
| us-801fc1b0 | kyoshin/CPartyStateWin | `PartyStateWin_StepPartySub` | PartyStateWin_StepPartySub | 0x801FC1B0 |
| us-801fc258 | kyoshin/CPartyStateWin | `PartyStateWin_StateOpen` | PartyStateWin_StateOpen | 0x801FC258 |
| us-801fc2d0 | kyoshin/CPartyStateWin | `PartyStateWin_StateGateOpen` | PartyStateWin_StateGateOpen | 0x801FC2D0 |
| us-801fc568 | kyoshin/CPartyStateWin | `PartyStateWin_StateSettle` | PartyStateWin_StateSettle | 0x801FC568 |
| us-801fc5e8 | kyoshin/CPartyStateWin | `PartyStateWin_StateRefresh` | PartyStateWin_StateRefresh | 0x801FC5E8 |
| us-801fc71c | kyoshin/CPartyStateWin | `PartyStateWin_StateEquipInput` | PartyStateWin_StateEquipInput | 0x801FC71C |
| us-801fd21c | kyoshin/CPartyStateWin | `PartyStateWin_StateRebuild` | PartyStateWin_StateRebuild | 0x801FD21C |
| us-801fd2c8 | kyoshin/CPartyStateWin | `PartyStateWin_StateGateReopen` | PartyStateWin_StateGateReopen | 0x801FD2C8 |
| us-801fd3e8 | kyoshin/CPartyStateWin | `PartyStateWin_StateEquipRefresh` | PartyStateWin_StateEquipRefresh | 0x801FD3E8 |
| us-801fd4f0 | kyoshin/CPartyStateWin | `PartyStateWin_StateShortcutOpen` | PartyStateWin_StateShortcutOpen | 0x801FD4F0 |
| us-801fd8ec | kyoshin/CPartyStateWin | `PartyStateWin_StateMarkEquipActive` | PartyStateWin_StateMarkEquipActive | 0x801FD8EC |
| us-801fd9b8 | kyoshin/CPartyStateWin | `cbRenderBefore__14CPartyStateWinFv` | CPartyStateWin::cbRenderBefore() | 0x801FD9B8 |
| us-801ff5b8 | kyoshin/CPartyState | `refreshPartyCursor` | refreshPartyCursor | 0x801FF5B8 |
| us-80201564 | kyoshin/menu/parts/CModelDispEquip | `ModelDispEquip_ResetDisplay` | ModelDispEquip_ResetDisplay | 0x80201564 |
| us-802017b8 | kyoshin/menu/parts/CModelDispEquip | `ModelDispEquip_StepIdleBuildOrTick` | ModelDispEquip_StepIdleBuildOrTick | 0x802017B8 |
| us-802018b4 | kyoshin/menu/parts/CModelDispEquip | `ModelDispEquip_TeardownHolder` | ModelDispEquip_TeardownHolder | 0x802018B4 |
| us-80201ab4 | kyoshin/menu/parts/CModelDispEquip | `ModelDispEquip_BuildPartyModel` | ModelDispEquip_BuildPartyModel | 0x80201AB4 |
| us-80202ad8 | kyoshin/menu/parts/CModelDispEquip | `OnFileEvent__15CModelDispEquipFP10CEventFile` | CModelDispEquip::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x80202AD8 |
| us-80202c00 | kyoshin/menu/parts/CModelDispEquip | `ModelDispEquip_RearmAnimSlotChecked` | ModelDispEquip_RearmAnimSlotChecked | 0x80202C00 |
| us-80202ca8 | kyoshin/menu/parts/CModelDispEquip | `ModelDispEquip_SwapPartyEquip` | ModelDispEquip_SwapPartyEquip | 0x80202CA8 |
| us-80202e40 | kyoshin/menu/parts/CModelDispEquip | `ModelDispEquip_SwapWeaponEquip` | ModelDispEquip_SwapWeaponEquip | 0x80202E40 |
| us-80203014 | kyoshin/menu/parts/CModelDispEquip | `ModelDispEquip_HandleSlotCmd` | ModelDispEquip_HandleSlotCmd | 0x80203014 |
| us-80203edc | kyoshin/CEquipChange | `EquipChange_DrawLayouts` | EquipChange_DrawLayouts | 0x80203EDC |
| us-8020417c | kyoshin/CEquipChange | `EquipChange_IsSubcurBusy` | EquipChange_IsSubcurBusy | 0x8020417C |
| us-80204270 | kyoshin/CEquipChange | `EquipChange_CloseScreen` | EquipChange_CloseScreen | 0x80204270 |
| us-80204e98 | kyoshin/CEquipChange | `EquipChange_MapCursorToSlot` | EquipChange_MapCursorToSlot | 0x80204E98 |
| us-802056ec | kyoshin/CEquipChange | `EquipChange_GetMenuId` | EquipChange_GetMenuId | 0x802056EC |
| us-80205a70 | kyoshin/CEquipChange | `EquipChange_OnAnim3CToSt3` | EquipChange_OnAnim3CToSt3 | 0x80205A70 |
| us-80205df4 | kyoshin/CEquipChange | `EquipChange_RefreshCursorPos` | EquipChange_RefreshCursorPos | 0x80205DF4 |
| us-802077c8 | kyoshin/CEquipChange | `sinit_802059E8` | sinit_802059E8 | 0x802077C8 |
| us-8020a768 | kyoshin/cf/CfGimmick | `CfGimmick_UpdateColliderMatrix` | CfGimmick_UpdateColliderMatrix | 0x8020A768 |
| us-8020aa40 | kyoshin/cf/CfGimmick | `CfGimmick_PlaySoundAtPosScaled` | CfGimmick_PlaySoundAtPosScaled | 0x8020AA40 |
| us-8020aaa0 | kyoshin/cf/CfGimmick | `CfGimmick_UpdatePartyAnchorState` | CfGimmick_UpdatePartyAnchorState | 0x8020AAA0 |
| us-8020ac78 | kyoshin/cf/CfGimmick | `CfGimmick_InitPartyGlobals` | CfGimmick_InitPartyGlobals | 0x8020AC78 |
| us-8020af3c | kyoshin/cf/CfGimmick | `CfGimmick_LoadBdatClAreaExtents` | CfGimmick_LoadBdatClAreaExtents | 0x8020AF3C |
| us-8020b16c | kyoshin/cf/CfGimmick | `CfGimmick_LoadBdatAreaPosIndexed` | CfGimmick_LoadBdatAreaPosIndexed | 0x8020B16C |
| us-8020b268 | kyoshin/cf/CfGimmick | `CfGimmick_LoadBdatAreaExtentsIndexed` | CfGimmick_LoadBdatAreaExtentsIndexed | 0x8020B268 |
| us-8020b3b8 | kyoshin/cf/CfGimmick | `CfGimmick_LoadBdatAreaRotationIndexed` | CfGimmick_LoadBdatAreaRotationIndexed | 0x8020B3B8 |
| us-8020b4cc | kyoshin/cf/CfGimmick | `CfGimmick_CheckStateFlag1D44` | CfGimmick_CheckStateFlag1D44 | 0x8020B4CC |
| us-8020b4fc | kyoshin/cf/CfGimmick | `CfGimmick_CheckStateFlag2CC8` | CfGimmick_CheckStateFlag2CC8 | 0x8020B4FC |
| us-8020b52c | kyoshin/cf/CfGimmick | `CfGimmick_TriggerSound2CC8` | CfGimmick_TriggerSound2CC8 | 0x8020B52C |
| us-8020b534 | kyoshin/cf/CfGimmick | `CfGimmick_CheckTriggerGated` | CfGimmick_CheckTriggerGated | 0x8020B534 |
| us-8020bfbc | kyoshin/cf/CfGimmick | `CfGimmick_ApplyPartyMoveSpeedGated` | CfGimmick_ApplyPartyMoveSpeedGated | 0x8020BFBC |
| us-8020c074 | kyoshin/cf/CfGimmick | `CfGimmick_CheckFightListPlayerAlive` | CfGimmick_CheckFightListPlayerAlive | 0x8020C074 |
| us-8020c13c | kyoshin/cf/CfGimmick | `CfGimmick_SpawnNamedObject` | CfGimmick_SpawnNamedObject | 0x8020C13C |
| us-8020c3f8 | kyoshin/cf/CfGimmick | `CfGimmick_IsMessageSystemBusy` | CfGimmick_IsMessageSystemBusy | 0x8020C3F8 |
| us-8020c424 | kyoshin/cf/CfGimmick | `CfGimmick_LookupBdatGimmickName` | CfGimmick_LookupBdatGimmickName | 0x8020C424 |
| us-8020e18c | kyoshin/cf/CfGimmickLock | `__ct__cf_CfGimmickLock` | constructor | 0x8020E18C |
| us-80211a6c | kyoshin/cf/CfGimmickJump | `func_8020FC14` | func_8020FC14 | 0x80211A6C |
| us-802153c8 | kyoshin/makecrystal/code_80213488 | `MakeCrystal_CollectByTarget` | MakeCrystal_CollectByTarget | 0x802153C8 |
| us-802157e0 | kyoshin/makecrystal/CMCCrystalBox | `sortCrystalIdOrder` | sortCrystalIdOrder | 0x802157E0 |
| us-80215a48 | kyoshin/makecrystal/CMCCrystalBox | `__ct__CMCCrystalBox` | constructor | 0x80215A48 |
| us-80215e3c | kyoshin/makecrystal/CMCCrystalBox | `initCrystalBoxResources` | initCrystalBoxResources | 0x80215E3C |
| us-8021611c | kyoshin/makecrystal/CMCCrystalBox | `updateCrystalBoxState` | updateCrystalBoxState | 0x8021611C |
| us-80216260 | kyoshin/makecrystal/CMCCrystalBox | `drawCrystalBox` | drawCrystalBox | 0x80216260 |
| us-8021634c | kyoshin/makecrystal/CMCCrystalBox | `teardownCrystalBox` | teardownCrystalBox | 0x8021634C |
| us-80216558 | kyoshin/makecrystal/CMCCrystalBox | `openCrystalBox` | openCrystalBox | 0x80216558 |
| us-8021664c | kyoshin/makecrystal/CMCCrystalBox | `enterCrystalBoxState4` | enterCrystalBoxState4 | 0x8021664C |
| us-802166e4 | kyoshin/makecrystal/CMCCrystalBox | `enterCrystalBoxState6` | enterCrystalBoxState6 | 0x802166E4 |
| us-80216784 | kyoshin/makecrystal/CMCCrystalBox | `enterCrystalBoxState8` | enterCrystalBoxState8 | 0x80216784 |
| us-8021681c | kyoshin/makecrystal/CMCCrystalBox | `enterCrystalBoxState13` | enterCrystalBoxState13 | 0x8021681C |
| us-80216d14 | kyoshin/makecrystal/CMCCrystalBox | `stepCrystalBoxPageBack` | stepCrystalBoxPageBack | 0x80216D14 |
| us-80217260 | kyoshin/makecrystal/CMCCrystalBox | `confirmCrystalSelect` | confirmCrystalSelect | 0x80217260 |
| us-80217370 | kyoshin/makecrystal/CMCCrystalBox | `toggleCrystalSortMenu` | toggleCrystalSortMenu | 0x80217370 |
| us-80218570 | kyoshin/makecrystal/CMCCrystalBox | `initCrystalPageLabels` | initCrystalPageLabels | 0x80218570 |
| us-802186a8 | kyoshin/makecrystal/CMCCrystalBox | `updateCrystalBoxSysWin` | updateCrystalBoxSysWin | 0x802186A8 |
| us-80218878 | kyoshin/makecrystal/CMCCrystalBox | `getCrystalBoxAction` | getCrystalBoxAction | 0x80218878 |
| us-80218a20 | kyoshin/makecrystal/CMCCrystalBox | `onCrystalAnimToState3` | onCrystalAnimToState3 | 0x80218A20 |
| us-80218a94 | kyoshin/makecrystal/CMCCrystalBox | `onCrystalAnimToState5` | onCrystalAnimToState5 | 0x80218A94 |
| us-80218ae0 | kyoshin/makecrystal/CMCCrystalBox | `onCrystalAnimToIdle` | onCrystalAnimToIdle | 0x80218AE0 |
| us-80218b38 | kyoshin/makecrystal/CMCCrystalBox | `onCrystalAnim14Idle` | onCrystalAnim14Idle | 0x80218B38 |
| us-80218d08 | kyoshin/makecrystal/CMCCrystalBox | `onCrystalAnimToState14` | onCrystalAnimToState14 | 0x80218D08 |
| us-80218d54 | kyoshin/makecrystal/CMCCrystalBox | `onCrystalAnimResetState3` | onCrystalAnimResetState3 | 0x80218D54 |
| us-80218de4 | kyoshin/makecrystal/CMCCrystalBox | `loadCrystalBoxArchive` | loadCrystalBoxArchive | 0x80218DE4 |
| us-80218ef0 | kyoshin/makecrystal/CMCCrystalBox | `setCrystalSlotTexture` | setCrystalSlotTexture | 0x80218EF0 |
| us-8021928c | kyoshin/makecrystal/CMCCrystalBox | `setCrystalSlotTexAlt` | setCrystalSlotTexAlt | 0x8021928C |
| us-80219628 | kyoshin/makecrystal/CMCCrystalBox | `setCrystalSlotCapacity` | setCrystalSlotCapacity | 0x80219628 |
| us-80219a64 | kyoshin/makecrystal/CMCCrystalBox | `setCrystalSlotCapAlt` | setCrystalSlotCapAlt | 0x80219A64 |
| us-80219e70 | kyoshin/makecrystal/CMCCrystalBox | `refreshCrystalBoxName` | refreshCrystalBoxName | 0x80219E70 |
| us-8021a2b8 | kyoshin/makecrystal/CMCCrystalBox | `setCrystalNameLabel` | setCrystalNameLabel | 0x8021A2B8 |
| us-8021a384 | kyoshin/makecrystal/CMCCrystalBox | `refreshCrystalBoxCursors` | refreshCrystalBoxCursors | 0x8021A384 |
| us-8021b7ec | kyoshin/makecrystal/CMCCrystalBox | `refreshCrystalPagePair` | refreshCrystalPagePair | 0x8021B7EC |
| us-8021cdcc | kyoshin/makecrystal/CMCCrystalInfo | `MCCrystal_StepIntro` | MCCrystal_StepIntro | 0x8021CDCC |
| us-8021ce18 | kyoshin/makecrystal/CMCCrystalInfo | `MCCrystal_StepMain` | MCCrystal_StepMain | 0x8021CE18 |
| us-8021ce64 | kyoshin/makecrystal/CMCCrystalInfo | `MCCrystal_RewindMain` | MCCrystal_RewindMain | 0x8021CE64 |
| us-8021ceb0 | kyoshin/makecrystal/CMCCrystalInfo | `MCCrystal_RewindIntro` | MCCrystal_RewindIntro | 0x8021CEB0 |
| us-8021cefc | kyoshin/makecrystal/CMCCrystalInfo | `MCCrystal_StepAnim4` | MCCrystal_StepAnim4 | 0x8021CEFC |
| us-8021cf48 | kyoshin/makecrystal/CMCCrystalInfo | `MCCrystal_StepAnim3` | MCCrystal_StepAnim3 | 0x8021CF48 |
| us-8021da1c | kyoshin/makecrystal/CMCCrystalInfo | `sinit_8021BBC4` | sinit_8021BBC4 | 0x8021DA1C |
| us-8021ed48 | kyoshin/makecrystal/CModelDispMakeCrystal | `MakeCrystal_StateStartFx` | MakeCrystal_StateStartFx | 0x8021ED48 |
| us-8021efc0 | kyoshin/makecrystal/CModelDispMakeCrystal | `MakeCrystal_StateChargeStep` | MakeCrystal_StateChargeStep | 0x8021EFC0 |
| us-8021f3bc | kyoshin/makecrystal/CModelDispMakeCrystal | `MakeCrystal_StateChargeLoop` | MakeCrystal_StateChargeLoop | 0x8021F3BC |
| us-8021f994 | kyoshin/makecrystal/CModelDispMakeCrystal | `MakeCrystal_StateFailWait` | MakeCrystal_StateFailWait | 0x8021F994 |
| us-8021fb64 | kyoshin/makecrystal/CModelDispMakeCrystal | `MakeCrystal_StateRankUp` | MakeCrystal_StateRankUp | 0x8021FB64 |
| us-8021fc78 | kyoshin/makecrystal/CModelDispMakeCrystal | `MakeCrystal_StateDivideDone` | MakeCrystal_StateDivideDone | 0x8021FC78 |
| us-8021fcd4 | kyoshin/makecrystal/CModelDispMakeCrystal | `MakeCrystal_StateCrystalDone` | MakeCrystal_StateCrystalDone | 0x8021FCD4 |
| us-80223c1c | kyoshin/makecrystal/CModelDispMakeCrystal | `sinit_80221DDC` | sinit_80221DDC | 0x80223C1C |
| us-80223c94 | kyoshin/makecrystal/CModelDispMakeCrystal | `destroyCrystalDispThunk4` | destroyCrystalDispThunk4 | 0x80223C94 |
| us-80223c9c | kyoshin/makecrystal/CModelDispMakeCrystal | `releaseCrystalDispThunk` | releaseCrystalDispThunk | 0x80223C9C |
| us-80223ca4 | kyoshin/makecrystal/CModelDispMakeCrystal | `destroyCrystalDispThunk8` | destroyCrystalDispThunk8 | 0x80223CA4 |
| us-80223cac | kyoshin/makecrystal/CMCCylinderGauge | `__ct__16CMCCylinderGaugeFPQ34nw4r3lyt19ArcResourceAccessor` | CMCCylinderGauge::CMCCylinderGauge(...*, Q, 3, 4, n, wchar_t, 4, r, 3, long, y, t, 1, 9, A, r, char, R, e, short, o, u, r, char, e, A, char, char, e, short, short, o, r) | 0x80223CAC |
| us-80223d34 | kyoshin/makecrystal/CMCCylinderGauge | `init__16CMCCylinderGaugeFv` | CMCCylinderGauge::init() | 0x80223D34 |
| us-80223f58 | kyoshin/makecrystal/CMCCylinderGauge | `startOut__16CMCCylinderGaugeFv` | CMCCylinderGauge::startOut() | 0x80223F58 |
| us-80223fe4 | kyoshin/makecrystal/CMCCylinderGauge | `addFillValue__16CMCCylinderGaugeFf` | CMCCylinderGauge::addFillValue(float) | 0x80223FE4 |
| us-80224074 | kyoshin/makecrystal/CMCCylinderGauge | `decrementLevel__16CMCCylinderGaugeFv` | CMCCylinderGauge::decrementLevel() | 0x80224074 |
| us-80224098 | kyoshin/makecrystal/CMCCylinderGauge | `getLevel__16CMCCylinderGaugeFv` | CMCCylinderGauge::getLevel() | 0x80224098 |
| us-802240e4 | kyoshin/makecrystal/CMCCylinderGauge | `setLevel__16CMCCylinderGaugeFUc` | CMCCylinderGauge::setLevel(unsigned char) | 0x802240E4 |
| us-80224104 | kyoshin/makecrystal/CMCCylinderGauge | `updateIn__16CMCCylinderGaugeFv` | CMCCylinderGauge::updateIn() | 0x80224104 |
| us-80224158 | kyoshin/makecrystal/CMCCylinderGauge | `updateFill__16CMCCylinderGaugeFv` | CMCCylinderGauge::updateFill() | 0x80224158 |
| us-802242ac | kyoshin/makecrystal/CMCCylinderGauge | `updateOut__16CMCCylinderGaugeFv` | CMCCylinderGauge::updateOut() | 0x802242AC |
| us-802242f0 | kyoshin/makecrystal/CMCCylinderGauge | `updateFull__16CMCCylinderGaugeFv` | CMCCylinderGauge::updateFull() | 0x802242F0 |
| us-80225b80 | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffStart` | constructor | 0x80225B80 |
| us-80225d64 | kyoshin/makecrystal/CMCEffStart | `startInAnim__11CMCEffStartFv` | func_80223F24 | 0x80225D64 |
| us-80225d84 | kyoshin/makecrystal/CMCEffStart | `updateInAnim__11CMCEffStartFv` | func_80223F44 | 0x80225D84 |
| us-80225dd0 | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffUpRed` | constructor | 0x80225DD0 |
| us-80225f04 | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffUpBlue` | constructor | 0x80225F04 |
| us-80226038 | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffUpGreen` | constructor | 0x80226038 |
| us-8022616c | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffUpPrm` | constructor | 0x8022616C |
| us-802261f4 | kyoshin/makecrystal/CMCEffStart | `init__11CMCEffUpPrmFv` | func_802243B4 | 0x802261F4 |
| us-802262bc | kyoshin/makecrystal/CMCEffStart | `update__11CMCEffUpPrmFv` | func_8022447C | 0x802262BC |
| us-80226430 | kyoshin/makecrystal/CMCEffStart | `activateSlots__11CMCEffUpPrmFUl` | func_802245F0 | 0x80226430 |
| us-8022652c | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffSuccess` | constructor | 0x8022652C |
| us-80226728 | kyoshin/makecrystal/CMCEffStart | `startIn__13CMCEffSuccessFv` | func_802248E8 | 0x80226728 |
| us-80226784 | kyoshin/makecrystal/CMCEffStart | `startOut__13CMCEffSuccessFv` | func_80224944 | 0x80226784 |
| us-802267d8 | kyoshin/makecrystal/CMCEffStart | `updateIn__13CMCEffSuccessFv` | func_80224998 | 0x802267D8 |
| us-80226824 | kyoshin/makecrystal/CMCEffStart | `updateOut__13CMCEffSuccessFv` | func_802249E4 | 0x80226824 |
| us-80226980 | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffFailure` | constructor(a, int, long, u, r, e) | 0x80226980 |
| us-80226a8c | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffCrystal` | constructor | 0x80226A8C |
| us-80226c5c | kyoshin/makecrystal/CMCEffStart | `update__13CMCEffCrystalFv` | func_80224E1C | 0x80226C5C |
| us-80226e84 | kyoshin/makecrystal/CMCEffStart | `startIn__13CMCEffCrystalFv` | func_80225044 | 0x80226E84 |
| us-80226efc | kyoshin/makecrystal/CMCEffStart | `startOut__13CMCEffCrystalFv` | func_802250BC | 0x80226EFC |
| us-80226fb0 | kyoshin/makecrystal/CMCEffStart | `startChange__13CMCEffCrystalFv` | func_80225170 | 0x80226FB0 |
| us-80227004 | kyoshin/makecrystal/CMCEffStart | `startSpecial__13CMCEffCrystalFv` | func_802251C4 | 0x80227004 |
| us-80227304 | kyoshin/makecrystal/CMCEffStart | `updateIn__13CMCEffCrystalFv` | func_802254C4 | 0x80227304 |
| us-80227354 | kyoshin/makecrystal/CMCEffStart | `checkInDone__13CMCEffCrystalFv` | func_80225514 | 0x80227354 |
| us-802273a0 | kyoshin/makecrystal/CMCEffStart | `updateOut__13CMCEffCrystalFv` | func_80225560 | 0x802273A0 |
| us-8022740c | kyoshin/makecrystal/CMCEffStart | `updateChange__13CMCEffCrystalFv` | func_802255CC | 0x8022740C |
| us-802274d4 | kyoshin/makecrystal/CMCEffStart | `updateSpecial__13CMCEffCrystalFv` | func_80225694 | 0x802274D4 |
| us-802278d8 | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffUpRank` | constructor | 0x802278D8 |
| us-80227a0c | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffDivide` | constructor | 0x80227A0C |
| us-80227b40 | kyoshin/makecrystal/CMCEffStart | `__ct__CMCEffCylinder` | constructor | 0x80227B40 |
| us-80227d68 | kyoshin/makecrystal/CMCEffStart | `startIn__14CMCEffCylinderFv` | func_80225F28 | 0x80227D68 |
| us-80227dbc | kyoshin/makecrystal/CMCEffStart | `startOut__14CMCEffCylinderFv` | func_80225F7C | 0x80227DBC |
| us-80227e10 | kyoshin/makecrystal/CMCEffStart | `startChange__14CMCEffCylinderFv` | func_80225FD0 | 0x80227E10 |
| us-80227e7c | kyoshin/makecrystal/CMCEffStart | `updateIn__14CMCEffCylinderFv` | func_8022603C | 0x80227E7C |
| us-80227ec8 | kyoshin/makecrystal/CMCEffStart | `updateOut__14CMCEffCylinderFv` | func_80226088 | 0x80227EC8 |
| us-80227f14 | kyoshin/makecrystal/CMCEffStart | `updateChange__14CMCEffCylinderFv` | func_802260D4 | 0x80227F14 |
| us-80228350 | kyoshin/menu/CMenuQstCnt | `Init__11CMenuQstCntFv` | CMenuQstCnt::Init() | 0x80228350 |
| us-80228708 | kyoshin/menu/CMenuQstCnt | `cbRenderBefore__11CMenuQstCntFv` | CMenuQstCnt::cbRenderBefore() | 0x80228708 |
| us-80228a50 | kyoshin/menu/CMenuQstCnt | `QstCnt_AdvanceToOpen_6C18` | QstCnt_AdvanceToOpen_6C18 | 0x80228A50 |
| us-80228a94 | kyoshin/menu/CMenuQstCnt | `QstCnt_TickToOpened_6C5C` | QstCnt_TickToOpened_6C5C | 0x80228A94 |
| us-80229584 | kyoshin/CQstLogList | `__ct__CQstLogList` | constructor | 0x80229584 |
| us-80229898 | kyoshin/CQstLogList | `QstLogList_LoadArc` | QstLogList_LoadArc | 0x80229898 |
| us-802298fc | kyoshin/CQstLogList | `QstLogList_FrameUpdate` | QstLogList_FrameUpdate | 0x802298FC |
| us-802299a4 | kyoshin/CQstLogList | `QstLogList_Draw` | QstLogList_Draw | 0x802299A4 |
| us-80229b14 | kyoshin/CQstLogList | `QstLogList_BeginSortOpen` | QstLogList_BeginSortOpen | 0x80229B14 |
| us-80229bb0 | kyoshin/CQstLogList | `QstLogList_EndSortMode` | QstLogList_EndSortMode | 0x80229BB0 |
| us-80229c20 | kyoshin/CQstLogList | `QstLogList_ScrollUp` | QstLogList_ScrollUp | 0x80229C20 |
| us-80229d00 | kyoshin/CQstLogList | `QstLogList_ScrollDown` | QstLogList_ScrollDown | 0x80229D00 |
| us-80229df8 | kyoshin/CQstLogList | `QstLogList_PageUp` | QstLogList_PageUp | 0x80229DF8 |
| us-80229eb4 | kyoshin/CQstLogList | `QstLogList_PageDown` | QstLogList_PageDown | 0x80229EB4 |
| us-80229f9c | kyoshin/CQstLogList | `QstLogList_ToggleSort` | QstLogList_ToggleSort | 0x80229F9C |
| us-8022a0b8 | kyoshin/CQstLogList | `QstLogList_ConfirmSort` | QstLogList_ConfirmSort | 0x8022A0B8 |
| us-8022a130 | kyoshin/CQstLogList | `QstLogList_ApplySortSelection` | QstLogList_ApplySortSelection | 0x8022A130 |
| us-8022a224 | kyoshin/CQstLogList | `QstLogList_StepSortAnim` | QstLogList_StepSortAnim | 0x8022A224 |
| us-8022a3dc | kyoshin/CQstLogList | `QstLogList_RefreshRows` | QstLogList_RefreshRows | 0x8022A3DC |
| us-8022a52c | kyoshin/CQstLogList | `QstLogList_DrawRow` | QstLogList_DrawRow | 0x8022A52C |
| us-8022a830 | kyoshin/CQstLogList | `QstLogList_ClearRows` | QstLogList_ClearRows | 0x8022A830 |
| us-8022a948 | kyoshin/CQstLogList | `QstLogList_MoveCursor` | QstLogList_MoveCursor | 0x8022A948 |
| us-8022aa3c | kyoshin/CQstLogList | `QstLogList_FillSortMenu` | QstLogList_FillSortMenu | 0x8022AA3C |
| us-8022b1b0 | kyoshin/CQstLogList | `sinit_80229378` | sinit_80229378 | 0x8022B1B0 |
| us-8022d194 | kyoshin/CQstLogInfo | `OnFileEvent__11CQstLogInfoFP10CEventFile` | CQstLogInfo::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x8022D194 |
| us-8022f18c | kyoshin/CExchangeWin | `OnFileEvent__12CExchangeWinFP10CEventFile` | CExchangeWin::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x8022F18C |
| us-80231cec | kyoshin/menu/CMenuArtsSet | `ArtsLayoutPair_advance` | ArtsLayoutPair_advance | 0x80231CEC |
| us-80235658 | kyoshin/menu/CMenuArtsSet | `CMenuArtsSet_teardown` | CMenuArtsSet_teardown | 0x80235658 |
| us-802371a0 | kyoshin/menu/CMenuArtsSet | `OnFileEvent__9CArtsListFP10CEventFile` | CArtsList::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x802371A0 |
| us-80237850 | kyoshin/CArtsInfo | `CArtsInfo_Draw` | CArtsInfo_Draw | 0x80237850 |
| us-80237c1c | kyoshin/CArtsInfo | `CArtsInfo_ToStateA` | CArtsInfo_ToStateA | 0x80237C1C |
| us-80237cd0 | kyoshin/CArtsInfo | `CArtsInfo_ToStateC` | CArtsInfo_ToStateC | 0x80237CD0 |
| us-802381cc | kyoshin/CArtsInfo | `CArtsInfo_AnimState6` | CArtsInfo_AnimState6 | 0x802381CC |
| us-80238f60 | kyoshin/CArtsInfo | `CArtsInfo_GetSkill5C` | CArtsInfo_GetSkill5C | 0x80238F60 |
| us-80238fb8 | kyoshin/CArtsInfo | `CArtsInfo_GetSkill2D` | CArtsInfo_GetSkill2D | 0x80238FB8 |
| us-80239148 | kyoshin/CArtsInfo | `CArtsInfo_CalcDmg52` | CArtsInfo_CalcDmg52 | 0x80239148 |
| us-802392a4 | kyoshin/CArtsInfo | `CArtsInfo_CalcDmg53` | CArtsInfo_CalcDmg53 | 0x802392A4 |
| us-802394b8 | kyoshin/CArtsInfo | `CArtsInfo_UpdateArtsRow` | CArtsInfo_UpdateArtsRow | 0x802394B8 |
| us-8023ddb8 | kyoshin/CArtsInfo | `sinit_8023BC8C` | sinit_8023BC8C | 0x8023DDB8 |
| us-80242518 | kyoshin/cf/CfNandManager | `func_80240360` | func_80240360 | 0x80242518 |
| us-802425d8 | kyoshin/cf/CfNandManager | `func_80240420` | func_80240420 | 0x802425D8 |
| us-80245da0 | kyoshin/CMapSel | `finishMapSelClose` | finishMapSelClose | 0x80245DA0 |
| us-802465a0 | kyoshin/CFade | `CFade_Update` | CFade_Update | 0x802465A0 |
| us-802467c4 | kyoshin/CFade | `OnFileEvent__5CFadeFP10CEventFile` | CFade::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x802467C4 |
| us-80253500 | kyoshin/menu/CMenuMapSelectSC | `Term__16CMenuMapSelectSCFv` | CMenuMapSelectSC::Term() | 0x80253500 |
| us-80253578 | kyoshin/menu/CMenuMapSelectSC | `Move__16CMenuMapSelectSCFv` | CMenuMapSelectSC::Move() | 0x80253578 |
| us-80253798 | kyoshin/menu/CMenuMapSelectSC | `MapSelectSCBeginFadeIn` | MapSelectSCBeginFadeIn | 0x80253798 |
| us-802537f0 | kyoshin/menu/CMenuMapSelectSC | `MapSelectSCPrepFloorMap` | MapSelectSCPrepFloorMap | 0x802537F0 |
| us-80253f84 | kyoshin/menu/CMenuMapSelectSC | `MapSelectSCResetFloorMapIdle` | MapSelectSCResetFloorMapIdle | 0x80253F84 |
| us-80253fcc | kyoshin/menu/CMenuMapSelectSC | `MapSelectSCCommitCursorIdle` | MapSelectSCCommitCursorIdle | 0x80253FCC |
| us-80254434 | kyoshin/menu/CMenuPause | `Term__10CMenuPauseFv` | CMenuPause::Term() | 0x80254434 |
| us-8025572c | kyoshin/CCollepedia | `clpSetupDetPg` | clpSetupDetPg | 0x8025572C |
| us-8025695c | kyoshin/CCollepedia | `__ct__CCollepedia` | constructor | 0x8025695C |
| us-80256c5c | kyoshin/CCollepedia | `clpDoFrameUpd` | clpDoFrameUpd | 0x80256C5C |
| us-802575e8 | kyoshin/CCollepedia | `clpOnConfirm` | clpOnConfirm | 0x802575E8 |
| us-80257cf0 | kyoshin/CCollepedia | `collepediaCreateItemMulti` | collepediaCreateItemMulti | 0x80257CF0 |
| us-80259278 | kyoshin/menu/CMenuKizunagram | `Move__15CMenuKizunagramFv` | CMenuKizunagram::Move() | 0x80259278 |
| us-8025a1d8 | kyoshin/CKizunagram | `setCurPanePhase` | setCurPanePhase | 0x8025A1D8 |
| us-8025b2d4 | kyoshin/CKizunagram | `freeLinePanes` | freeLinePanes | 0x8025B2D4 |
| us-8025b514 | kyoshin/CKizunagram | `kizAnimAtEnd` | kizAnimAtEnd | 0x8025B514 |
| us-8025bd54 | kyoshin/CKizunagram | `resetKizPaneTags` | resetKizPaneTags | 0x8025BD54 |
| us-8025be98 | kyoshin/CKizunagram | `lineCountUp` | lineCountUp | 0x8025BE98 |
| us-8025cc74 | kyoshin/CKizunagram | `findKizByName` | findKizByName | 0x8025CC74 |
| us-8025d8ac | kyoshin/CKizunagram | `buildInfoLayout` | buildInfoLayout | 0x8025D8AC |
| us-8025e3e4 | kyoshin/CKizunagram | `infoAnimToMode5` | infoAnimToMode5 | 0x8025E3E4 |
| us-8025e494 | kyoshin/CKizunagram | `infoAnimToMode0` | infoAnimToMode0 | 0x8025E494 |
| us-8025e9c0 | kyoshin/CKizunagram | `kizCursorDir1` | kizCursorDir1 | 0x8025E9C0 |
| us-8025ea50 | kyoshin/CKizunagram | `kizCursorDir2` | kizCursorDir2 | 0x8025EA50 |
| us-8025eae0 | kyoshin/CKizunagram | `kizCursorDir3` | kizCursorDir3 | 0x8025EAE0 |
| us-8025eb70 | kyoshin/CKizunagram | `kizCursorDir4` | kizCursorDir4 | 0x8025EB70 |
| us-8025ec9c | kyoshin/CKizunagram | `kizCursorHoldB` | kizCursorHoldB | 0x8025EC9C |
| us-8025efc4 | kyoshin/CKizunagram | `slideKizDown` | slideKizDown | 0x8025EFC4 |
| us-8025f1f4 | kyoshin/CKizunagram | `OnFileEvent__11CKizunagramFP10CEventFile` | CKizunagram::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x8025F1F4 |
| us-8025f450 | kyoshin/CKizunagram | `sinit_8025D304` | sinit_8025D304 | 0x8025F450 |
| us-8025f630 | kyoshin/CPcKizunagram | `KizunaCurInitLayout` | KizunaCurInitLayout | 0x8025F630 |
| us-8025fbc4 | kyoshin/CPcKizunagram | `KizunagramClose` | KizunagramClose | 0x8025FBC4 |
| us-8025fd54 | kyoshin/CPcKizunagram | `KizunagramFinishOpening` | KizunagramFinishOpening | 0x8025FD54 |
| us-802604f0 | kyoshin/CPcKizunagram | `KizunagramStepRow` | KizunagramStepRow | 0x802604F0 |
| us-80260aac | kyoshin/CPcKizunagram | `KizunagramCheckRowAvailable` | KizunagramCheckRowAvailable | 0x80260AAC |
| us-80260b30 | kyoshin/CPcKizunagram | `KizunagramCheckRowHighlight` | KizunagramCheckRowHighlight | 0x80260B30 |
| us-80263d14 | kyoshin/code_8025FB10 | `TextState_DrawStaged` | TextState_DrawStaged | 0x80263D14 |
| us-80263e48 | kyoshin/code_8025FB10 | `TextState_DrawTextOrtho` | TextState_DrawTextOrtho | 0x80263E48 |
| us-80266674 | kyoshin/CPassiveSkill | `CPassiveSkillLayoutInit_init` | CPassiveSkillLayoutInit_init | 0x80266674 |
| us-80266944 | kyoshin/CPassiveSkill | `CPassiveSkillInit_stepMode0` | CPassiveSkillInit_stepMode0 | 0x80266944 |
| us-802669f8 | kyoshin/CPassiveSkill | `CPassiveSkillInit_stepMode1` | CPassiveSkillInit_stepMode1 | 0x802669F8 |
| us-80266b58 | kyoshin/CPassiveSkill | `CPassiveSkillInfo_init` | CPassiveSkillInfo_init | 0x80266B58 |
| us-802672e0 | kyoshin/CPassiveSkill | `CPassiveSkillInfo_setNameTex` | CPassiveSkillInfo_setNameTex | 0x802672E0 |
| us-8026855c | kyoshin/CPassiveSkill | `CPassiveSkillInfo_toggleNamePanes` | CPassiveSkillInfo_toggleNamePanes | 0x8026855C |
| us-80268618 | kyoshin/CPassiveSkill | `CPassiveSkillCur_setNameMsg135` | CPassiveSkillCur_setNameMsg135 | 0x80268618 |
| us-8026866c | kyoshin/CPassiveSkill | `CPassiveSkillCur_setNameMsg136` | CPassiveSkillCur_setNameMsg136 | 0x8026866C |
| us-802686c0 | kyoshin/CPassiveSkill | `CPassiveSkillCur_setRootPos` | CPassiveSkillCur_setRootPos | 0x802686C0 |
| us-80268830 | kyoshin/CPassiveSkill | `CPassiveSkillCur_stepToState5` | CPassiveSkillCur_stepToState5 | 0x80268830 |
| us-802688e0 | kyoshin/CPassiveSkill | `CPassiveSkillCur_stepToState0` | CPassiveSkillCur_stepToState0 | 0x802688E0 |
| us-802689e4 | kyoshin/CPassiveSkill | `CPassiveSkillInfo_stepToState3Alt` | CPassiveSkillInfo_stepToState3Alt | 0x802689E4 |
| us-80268a6c | kyoshin/CPassiveSkill | `CPassiveSkillLine_refreshSP` | CPassiveSkillLine_refreshSP | 0x80268A6C |
| us-802697d0 | kyoshin/CPassiveSkill | `CPassiveSkillLine_update` | CPassiveSkillLine_update | 0x802697D0 |
| us-802698f4 | kyoshin/CPassiveSkill | `CPassiveSkillLine_draw` | CPassiveSkillLine_draw | 0x802698F4 |
| us-80269b68 | kyoshin/CPassiveSkill | `CPassiveSkillLine_openRefresh` | CPassiveSkillLine_openRefresh | 0x80269B68 |
| us-8026a150 | kyoshin/CPassiveSkill | `CPassiveSkillLine_cursorDown` | CPassiveSkillLine_cursorDown | 0x8026A150 |
| us-8026a29c | kyoshin/CPassiveSkill | `CPassiveSkillLine_cursorUp` | CPassiveSkillLine_cursorUp | 0x8026A29C |
| us-8026b018 | kyoshin/CPassiveSkill | `CPassiveSkillLine_learnedClose` | CPassiveSkillLine_learnedClose | 0x8026B018 |
| us-8026b3ec | kyoshin/CPassiveSkill | `CPassiveSkillLine_syswinAdvance` | CPassiveSkillLine_syswinAdvance | 0x8026B3EC |
| us-8026b5dc | kyoshin/CPassiveSkill | `CPassiveSkillLine_stepToState7` | CPassiveSkillLine_stepToState7 | 0x8026B5DC |
| us-8026b7e0 | kyoshin/CPassiveSkill | `CPassiveSkillLine_stepState4` | CPassiveSkillLine_stepState4 | 0x8026B7E0 |
| us-8026b880 | kyoshin/CPassiveSkill | `CPassiveSkillLine_stepState3` | CPassiveSkillLine_stepState3 | 0x8026B880 |
| us-8026b964 | kyoshin/CPassiveSkill | `CPassiveSkillLine_stepState14` | CPassiveSkillLine_stepState14 | 0x8026B964 |
| us-8026ba2c | kyoshin/CPassiveSkill | `CPassiveSkillLine_stepState9` | CPassiveSkillLine_stepState9 | 0x8026BA2C |
| us-8026bbec | kyoshin/CPassiveSkill | `CPassiveSkillLine_closeMoveCur` | CPassiveSkillLine_closeMoveCur | 0x8026BBEC |
| us-8026c08c | kyoshin/CPassiveSkill | `CPassiveSkillLine_refreshNameTex` | CPassiveSkillLine_refreshNameTex | 0x8026C08C |
| us-8026f0dc | kyoshin/CPassiveSkill | `CPassiveSkillLine_isLearned` | CPassiveSkillLine_isLearned | 0x8026F0DC |
| us-8026f1c8 | kyoshin/CPassiveSkill | `CPassiveSkillLine_isOccupied` | CPassiveSkillLine_isOccupied | 0x8026F1C8 |
| us-8026f850 | kyoshin/CPassiveSkill | `CPassiveSkillLine_countSkillId` | CPassiveSkillLine_countSkillId | 0x8026F850 |
| us-8026fda4 | kyoshin/CPassiveSkill | `CPassiveSkill_teardown` | CPassiveSkill_teardown | 0x8026FDA4 |
| us-8026ff54 | kyoshin/CPassiveSkill | `CPassiveSkill_confirm` | CPassiveSkill_confirm | 0x8026FF54 |
| us-8026ffc0 | kyoshin/CPassiveSkill | `UI_CPassiveSkill_thunk28_67CE0` | UI_CPassiveSkill_thunk28_67CE0 | 0x8026FFC0 |
| us-8026ffd0 | kyoshin/CPassiveSkill | `UI_CPassiveSkill_thunk28_67F88` | UI_CPassiveSkill_thunk28_67F88 | 0x8026FFD0 |
| us-8026ffd8 | kyoshin/CPassiveSkill | `UI_CPassiveSkill_thunk28_68250` | UI_CPassiveSkill_thunk28_68250 | 0x8026FFD8 |
| us-8026ffe0 | kyoshin/CPassiveSkill | `UI_CPassiveSkill_thunk28_67BA0` | UI_CPassiveSkill_thunk28_67BA0 | 0x8026FFE0 |
| us-8026fff0 | kyoshin/CPassiveSkill | `UI_CPassiveSkill_thunk28_68518` | UI_CPassiveSkill_thunk28_68518 | 0x8026FFF0 |
| us-8026fff8 | kyoshin/CPassiveSkill | `CPassiveSkill_pickOpenState` | CPassiveSkill_pickOpenState | 0x8026FFF8 |
| us-80270208 | kyoshin/CPassiveSkill | `CPassiveSkill_learnAllSkills` | CPassiveSkill_learnAllSkills | 0x80270208 |
| us-80270c7c | kyoshin/cf/CfGimmickEne | `gimmickEneSetupGXState` | gimmickEneSetupGXState | 0x80270C7C |
| us-80272278 | kyoshin/menu/CMenuPlayAward | `Init__14CMenuPlayAwardFv` | CMenuPlayAward::Init() | 0x80272278 |
| us-8027256c | kyoshin/menu/CMenuPlayAward | `Term__14CMenuPlayAwardFv` | CMenuPlayAward::Term() | 0x8027256C |
| us-802725d8 | kyoshin/menu/CMenuPlayAward | `Move__14CMenuPlayAwardFv` | CMenuPlayAward::Move() | 0x802725D8 |
| us-802726e8 | kyoshin/menu/CMenuPlayAward | `cbRenderBefore__14CMenuPlayAwardFv` | CMenuPlayAward::cbRenderBefore() | 0x802726E8 |
| us-80272810 | kyoshin/menu/CMenuPlayAward | `PlayAward_Phase0_ShowHelp` | PlayAward_Phase0_ShowHelp | 0x80272810 |
| us-80273170 | kyoshin/menu/CMenuPlayAward | `PlayAward_LoadLayouts` | PlayAward_LoadLayouts | 0x80273170 |
| us-802731e8 | kyoshin/menu/CMenuPlayAward | `PlayAward_UpdateList` | PlayAward_UpdateList | 0x802731E8 |
| us-80273288 | kyoshin/menu/CMenuPlayAward | `PlayAward_DrawList` | PlayAward_DrawList | 0x80273288 |
| us-802732e8 | kyoshin/menu/CMenuPlayAward | `PlayAward_ReleaseList` | PlayAward_ReleaseList | 0x802732E8 |
| us-802733f8 | kyoshin/menu/CMenuPlayAward | `PlayAward_RequestOpen` | PlayAward_RequestOpen | 0x802733F8 |
| us-802734f4 | kyoshin/menu/CMenuPlayAward | `PlayAward_RequestClose` | PlayAward_RequestClose | 0x802734F4 |
| us-80273558 | kyoshin/menu/CMenuPlayAward | `PlayAward_MoveCursorUp` | PlayAward_MoveCursorUp | 0x80273558 |
| us-802736e4 | kyoshin/menu/CMenuPlayAward | `PlayAward_ScrollPageUp` | PlayAward_ScrollPageUp | 0x802736E4 |
| us-80273784 | kyoshin/menu/CMenuPlayAward | `PlayAward_ScrollPageDown` | PlayAward_ScrollPageDown | 0x80273784 |
| us-80273840 | kyoshin/menu/CMenuPlayAward | `PlayAward_FlipPage` | PlayAward_FlipPage | 0x80273840 |
| us-80273904 | kyoshin/menu/CMenuPlayAward | `PlayAward_FinishOpenAnimA` | PlayAward_FinishOpenAnimA | 0x80273904 |
| us-80273bb4 | kyoshin/menu/CMenuPlayAward | `PlayAward_RefreshCursor` | PlayAward_RefreshCursor | 0x80273BB4 |
| us-80273c7c | kyoshin/menu/CMenuPlayAward | `PlayAward_RefreshPageTitles` | PlayAward_RefreshPageTitles | 0x80273C7C |
| us-80273f68 | kyoshin/menu/CMenuPlayAward | `OnFileEvent__14CPlayAwardListFP10CEventFile` | CPlayAwardList::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x80273F68 |
| us-8027431c | kyoshin/menu/CMenuKizunaTalkList | `__dt__19CMenuKizunaTalkListFv` | CMenuKizunaTalkList::~CMenuKizunaTalkList() | 0x8027431C |
| us-80275e08 | kyoshin/CKizunaTalkList | `advanceTalkAnim28` | advanceTalkAnim28 | 0x80275E08 |
| us-80276b08 | kyoshin/CSysWinBuff | `Term__11CSysWinBuffFv` | CSysWinBuff::Term() | 0x80276B08 |
| us-80276d34 | kyoshin/CSysWinBuff | `cbRenderBefore__11CSysWinBuffFv` | CSysWinBuff::cbRenderBefore() | 0x80276D34 |
| us-80276f84 | kyoshin/CSysWinBuff | `OnFileEvent__11CSysWinBuffFP10CEventFile` | CSysWinBuff::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x80276F84 |
| us-80277ec0 | kyoshin/menu/CMenuLvUp | `Init__9CMenuLvUpFv` | CMenuLvUp::Init() | 0x80277EC0 |
| us-80278068 | kyoshin/menu/CMenuLvUp | `Term__9CMenuLvUpFv` | CMenuLvUp::Term() | 0x80278068 |
| us-802795f8 | kyoshin/cf/chain/CChain | `__ct__Q22cf6CChainFv` | cf::CChain::CChain() | 0x802795F8 |
| us-80279710 | kyoshin/cf/chain/CChain | `CChain_ResetChainState` | CChain_ResetChainState | 0x80279710 |
| us-802797b0 | kyoshin/cf/chain/CChain | `CChain_UpdatePerFrame` | CChain_UpdatePerFrame | 0x802797B0 |
| us-8027b290 | kyoshin/cf/chain/CChain | `CChain_AdvanceChainLink` | CChain_AdvanceChainLink | 0x8027B290 |
| us-8027b7f0 | kyoshin/cf/chain/CChain | `CChain_TryActivateChain` | CChain_TryActivateChain | 0x8027B7F0 |
| us-8027ba58 | kyoshin/cf/chain/CChain | `CChain_CheckActivateReady` | CChain_CheckActivateReady | 0x8027BA58 |
| us-8027bd90 | kyoshin/cf/chain/CChain | `CChain_ProbeActorReady` | CChain_ProbeActorReady | 0x8027BD90 |
| us-8027be74 | kyoshin/cf/chain/CChain | `CChain_HasMemberEntry` | CChain_HasMemberEntry | 0x8027BE74 |
| us-8027c2cc | kyoshin/cf/chain/CChain | `CChainActor_ToggleCancelVoice` | CChainActor_ToggleCancelVoice | 0x8027C2CC |
| us-8027c3f0 | kyoshin/cf/chain/CChain | `CChainActor_ToggleMoveFlag` | CChainActor_ToggleMoveFlag | 0x8027C3F0 |
| us-8027d274 | kyoshin/cf/chain/CChainActorList | `__dt__34_reslist_base<PQ22cf11CChainActor>Fv` | _reslist_base<PQ22cf11CChainActor>::~_reslist_base<PQ22cf11CChainActor>() | 0x8027D274 |
| us-8027d328 | kyoshin/cf/chain/CChainActorList | `__dt__28reslist<PQ22cf11CChainActor>Fv` | reslist<PQ22cf11CChainActor>::~reslist<PQ22cf11CChainActor>() | 0x8027D328 |
| us-8027e5d8 | kyoshin/cf/chain/CChainActorList | `Chain_RollActivation` | Chain_RollActivation | 0x8027E5D8 |
| us-8027e7c0 | kyoshin/cf/chain/CChainActorList | `Chain_ReadPadAction` | Chain_ReadPadAction | 0x8027E7C0 |
| us-8027f06c | kyoshin/cf/chain/CChainActorList | `ChainCounter_Drain` | ChainCounter_Drain | 0x8027F06C |
| us-8027f258 | kyoshin/cf/chain/CChainTime | `__ct__Q22cf10CChainTimeFv` | cf::CChainTime::CChainTime() | 0x8027F258 |
| us-80280b60 | kyoshin/CSysWinScenarioLog | `Term__18CSysWinScenarioLogFv` | CSysWinScenarioLog::Term() | 0x80280B60 |
| us-80280dc0 | kyoshin/CSysWinScenarioLog | `cbRenderBefore__18CSysWinScenarioLogFv` | CSysWinScenarioLog::cbRenderBefore() | 0x80280DC0 |
| us-80283da8 | kyoshin/cf/chain/CChainActorPc | `resetChainActor` | resetChainActor | 0x80283DA8 |
| us-8028413c | kyoshin/cf/chain/CChainActorPc | `clearChainTarget` | clearChainTarget | 0x8028413C |
| us-802844cc | kyoshin/cf/chain/CChainActorPc | `forwardChainVoiceB3` | forwardChainVoiceB3 | 0x802844CC |
| us-80284558 | kyoshin/cf/chain/CChainActorPc | `resetChainMeter` | resetChainMeter | 0x80284558 |
| us-802848e0 | kyoshin/cf/chain/CChainActorPc | `isChainEffectReady` | isChainEffectReady | 0x802848E0 |
| us-802851e4 | kyoshin/CEquipItemBox | `countEIBPages` | countEIBPages | 0x802851E4 |
| us-802857d4 | kyoshin/CEquipItemBox | `fmtEIBItemName` | fmtEIBItemName | 0x802857D4 |
| us-80286914 | kyoshin/CEquipItemBox | `sortEIBByNm58` | sortEIBByNm58 | 0x80286914 |
| us-80286aa0 | kyoshin/CEquipItemBox | `sortEIBByNm00` | sortEIBByNm00 | 0x80286AA0 |
| us-802874f0 | kyoshin/CEquipItemBox | `sortEIBByNm72` | sortEIBByNm72 | 0x802874F0 |
| us-80287640 | kyoshin/CEquipItemBox | `sortEIBByNm7A` | sortEIBByNm7A | 0x80287640 |
| us-80287790 | kyoshin/CEquipItemBox | `sortEIBByNm17` | sortEIBByNm17 | 0x80287790 |
| us-802878fc | kyoshin/CEquipItemBox | `sortEIBByNm82` | sortEIBByNm82 | 0x802878FC |
| us-80287b8c | kyoshin/CEquipItemBox | `getEIBProgress` | getEIBProgress | 0x80287B8C |
| us-80287d14 | kyoshin/CEquipItemBox | `getEIBNameKey` | getEIBNameKey | 0x80287D14 |
| us-8028862c | kyoshin/CEquipItemBox | `loadEIBFiles` | loadEIBFiles | 0x8028862C |
| us-802886e8 | kyoshin/CEquipItemBox | `updateEIBBox` | updateEIBBox | 0x802886E8 |
| us-802887c4 | kyoshin/CEquipItemBox | `drawEIBBox` | drawEIBBox | 0x802887C4 |
| us-802888d8 | kyoshin/CEquipItemBox | `closeEIBBox` | closeEIBBox | 0x802888D8 |
| us-80288a34 | kyoshin/CEquipItemBox | `eibInputBlocked` | eibInputBlocked | 0x80288A34 |
| us-80288ad4 | kyoshin/CEquipItemBox | `eibMenuBusy` | eibMenuBusy | 0x80288AD4 |
| us-80288bc4 | kyoshin/CEquipItemBox | `finishEIBEntry` | finishEIBEntry | 0x80288BC4 |
| us-80288c64 | kyoshin/CEquipItemBox | `eibNavLeft` | eibNavLeft | 0x80288C64 |
| us-80289018 | kyoshin/CEquipItemBox | `eibNavUp` | eibNavUp | 0x80289018 |
| us-80289200 | kyoshin/CEquipItemBox | `eibNavRight` | eibNavRight | 0x80289200 |
| us-802894a8 | kyoshin/CEquipItemBox | `eibPagePrev` | eibPagePrev | 0x802894A8 |
| us-80289560 | kyoshin/CEquipItemBox | `eibConfirmSort` | eibConfirmSort | 0x80289560 |
| us-802896d4 | kyoshin/CEquipItemBox | `eibHandleSubPage` | eibHandleSubPage | 0x802896D4 |
| us-8028a1dc | kyoshin/CEquipItemBox | `stepEIBSysWin2` | stepEIBSysWin2 | 0x8028A1DC |
| us-8028a388 | kyoshin/CEquipItemBox | `eibApplySelect` | eibApplySelect | 0x8028A388 |
| us-8028a900 | kyoshin/CEquipItemBox | `eibRowMatches` | eibRowMatches | 0x8028A900 |
| us-8028a9c8 | kyoshin/CEquipItemBox | `findEIBEquipSlot` | findEIBEquipSlot | 0x8028A9C8 |
| us-8028ab5c | kyoshin/CEquipItemBox | `eibTryCloseRow` | eibTryCloseRow | 0x8028AB5C |
| us-8028abf0 | kyoshin/CEquipItemBox | `eibHudPrompt` | eibHudPrompt | 0x8028ABF0 |
| us-8028aea0 | kyoshin/CEquipItemBox | `eibCloseAnim` | eibCloseAnim | 0x8028AEA0 |
| us-8028aef0 | kyoshin/CEquipItemBox | `eibSysWin1Done` | eibSysWin1Done | 0x8028AEF0 |
| us-8028b298 | kyoshin/CEquipItemBox | `bindEIBCellIcon` | bindEIBCellIcon | 0x8028B298 |
| us-8028b818 | kyoshin/CEquipItemBox | `bindEIBCellMark` | bindEIBCellMark | 0x8028B818 |
| us-8028b97c | kyoshin/CEquipItemBox | `rebuildEIBPage` | rebuildEIBPage | 0x8028B97C |
| us-8028c2ec | kyoshin/CEquipItemBox | `fillEIBSortMenu` | fillEIBSortMenu | 0x8028C2EC |
| us-8028c55c | kyoshin/CEquipItemBox | `eibSortPageNext` | eibSortPageNext | 0x8028C55C |
| us-8028c5dc | kyoshin/CEquipItemBox | `eibSortPagePrev` | eibSortPagePrev | 0x8028C5DC |
| us-8028c658 | kyoshin/CEquipItemBox | `refreshEIBSortTabs` | refreshEIBSortTabs | 0x8028C658 |
| us-8028ce48 | kyoshin/CEquipItemBox | `dispatchEIBKind` | dispatchEIBKind | 0x8028CE48 |
| us-8028cee0 | kyoshin/CEquipItemBox | `resetEIBPageView` | resetEIBPageView | 0x8028CEE0 |
| us-8028f568 | kyoshin/CEquipItemBox | `getEIBPageHint` | getEIBPageHint | 0x8028F568 |
| us-8028ff2c | kyoshin/CEquipItemBox | `sinit_8028DAB0` | sinit_8028DAB0 | 0x8028FF2C |
| us-802913a0 | kyoshin/CSaveLoad | `CSLSub_tickOut` | CSLSub_tickOut | 0x802913A0 |
| us-80291634 | kyoshin/CSaveLoad | `__dt__9CSaveLoadFv` | CSaveLoad::~CSaveLoad() | 0x80291634 |
| us-802916b8 | kyoshin/CSaveLoad | `loadSaveData__9CSaveLoadFv` | CSaveLoad::loadSaveData() | 0x802916B8 |
| us-80291748 | kyoshin/CSaveLoad | `CSaveLoad_update` | CSaveLoad_update | 0x80291748 |
| us-80291850 | kyoshin/CSaveLoad | `CSaveLoad_draw` | CSaveLoad_draw | 0x80291850 |
| us-80291928 | kyoshin/CSaveLoad | `CSaveLoad_reset` | CSaveLoad_reset | 0x80291928 |
| us-80291b60 | kyoshin/CSaveLoad | `CSaveLoad_open` | CSaveLoad_open | 0x80291B60 |
| us-80291bf8 | kyoshin/CSaveLoad | `CSaveLoad_close` | CSaveLoad_close | 0x80291BF8 |
| us-80291c54 | kyoshin/CSaveLoad | `CSaveLoad_curUp` | CSaveLoad_curUp | 0x80291C54 |
| us-80291d88 | kyoshin/CSaveLoad | `CSaveLoad_curDown` | CSaveLoad_curDown | 0x80291D88 |
| us-80291ed8 | kyoshin/CSaveLoad | `CSaveLoad_pageUp` | CSaveLoad_pageUp | 0x80291ED8 |
| us-80291fa4 | kyoshin/CSaveLoad | `CSaveLoad_pageDown` | CSaveLoad_pageDown | 0x80291FA4 |
| us-8029209c | kyoshin/CSaveLoad | `CSaveLoad_confirm` | CSaveLoad_confirm | 0x8029209C |
| us-802922d4 | kyoshin/CSaveLoad | `CSaveLoad_cancel` | CSaveLoad_cancel | 0x802922D4 |
| us-80292350 | kyoshin/CSaveLoad | `CSaveLoad_openDel` | CSaveLoad_openDel | 0x80292350 |
| us-802925c4 | kyoshin/CSaveLoad | `CSaveLoad_tickCloseB` | CSaveLoad_tickCloseB | 0x802925C4 |
| us-80292610 | kyoshin/CSaveLoad | `CSaveLoad_tickCloseA` | CSaveLoad_tickCloseA | 0x80292610 |
| us-8029265c | kyoshin/CSaveLoad | `CSaveLoad_tickConf` | CSaveLoad_tickConf | 0x8029265C |
| us-802926b0 | kyoshin/CSaveLoad | `CSaveLoad_execOp` | CSaveLoad_execOp | 0x802926B0 |
| us-802928a0 | kyoshin/CSaveLoad | `CSaveLoad_finish` | CSaveLoad_finish | 0x802928A0 |
| us-80292d48 | kyoshin/CSaveLoad | `CSaveLoad_refresh` | CSaveLoad_refresh | 0x80292D48 |
| us-80293e40 | kyoshin/CSaveLoad | `__dt__Q22cf7CfAwardFv` | cf::CfAward::~CfAward() | 0x80293E40 |
| us-80293ea8 | kyoshin/CSaveLoad | `awardCollectCount` | awardCollectCount | 0x80293EA8 |
| us-80293f48 | kyoshin/CSaveLoad | `awardCount8F` | awardCount8F | 0x80293F48 |
| us-80293f9c | kyoshin/CSaveLoad | `awardCount91` | awardCount91 | 0x80293F9C |
| us-80294000 | kyoshin/CSaveLoad | `awardQuestFlags` | awardQuestFlags | 0x80294000 |
| us-8029412c | kyoshin/CSaveLoad | `awardAffinity` | awardAffinity | 0x8029412C |
| us-802941f4 | kyoshin/CSaveLoad | `countCollepedia` | countCollepedia | 0x802941F4 |
| us-80294394 | kyoshin/CSaveLoad | `chkAwardFlags98` | chkAwardFlags98 | 0x80294394 |
| us-802944ec | kyoshin/CSaveLoad | `chkAwardFlags99` | chkAwardFlags99 | 0x802944EC |
| us-802945fc | kyoshin/CSaveLoad | `chkAwardFlags9A` | chkAwardFlags9A | 0x802945FC |
| us-80294a14 | kyoshin/CSaveLoad | `chkAwardFlags9B` | chkAwardFlags9B | 0x80294A14 |
| us-80294fc4 | kyoshin/CSaveLoad | `chkAwardFlags9C` | chkAwardFlags9C | 0x80294FC4 |
| us-802954bc | kyoshin/CSaveLoad | `chkAwardFlags9D` | chkAwardFlags9D | 0x802954BC |
| us-80296640 | kyoshin/CSysWinSave | `Init__11CSysWinSaveFv` | CSysWinSave::Init() | 0x80296640 |
| us-802967ec | kyoshin/CSysWinSave | `Term__11CSysWinSaveFv` | CSysWinSave::Term() | 0x802967EC |
| us-80296870 | kyoshin/CSysWinSave | `Move__11CSysWinSaveFv` | CSysWinSave::Move() | 0x80296870 |
| us-80296a3c | kyoshin/CSysWinSave | `cbRenderBefore__11CSysWinSaveFv` | CSysWinSave::cbRenderBefore() | 0x80296A3C |
| us-80296ad4 | kyoshin/CSysWinSave | `__ct__802944D8` | constructor | 0x80296AD4 |
| us-80296c34 | kyoshin/CSysWinSave | `func_80294638` | func_80294638 | 0x80296C34 |
| us-80296e70 | kyoshin/realtimeevt/CREvtMovie | `__ct__CREvtMovie` | constructor | 0x80296E70 |
| us-80296ecc | kyoshin/realtimeevt/CREvtMovie | `__ct__802948D0` | constructor | 0x80296ECC |
| us-802972b0 | kyoshin/CTaskGamePic | `__ct__CTaskGamePic` | constructor | 0x802972B0 |
| us-802975b4 | kyoshin/CTaskGamePic | `Move__12CTaskGamePicFv` | CTaskGamePic::Move() | 0x802975B4 |
| us-802976f0 | kyoshin/CTaskGamePic | `cbRenderBefore__12CTaskGamePicFv` | CTaskGamePic::cbRenderBefore() | 0x802976F0 |
| us-80297b40 | kyoshin/CTaskGamePic | `OnFileEvent__12CTaskGamePicFP10CEventFile` | CTaskGamePic::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x80297B40 |
| us-80299228 | kyoshin/makecrystal/CMCGetItemBox | `refillMCItemSubTable` | refillMCItemSubTable | 0x80299228 |
| us-8029a0ac | kyoshin/makecrystal/CMCGetItemBox | `updateMCGetItemBox` | updateMCGetItemBox | 0x8029A0AC |
| us-8029a174 | kyoshin/makecrystal/CMCGetItemBox | `drawMCGetItemBox` | drawMCGetItemBox | 0x8029A174 |
| us-8029a4e0 | kyoshin/makecrystal/CMCGetItemBox | `confirmMCGetItemBox` | confirmMCGetItemBox | 0x8029A4E0 |
| us-8029ab18 | kyoshin/makecrystal/CMCGetItemBox | `toggleMCGetItemBoxSysWin` | toggleMCGetItemBoxSysWin | 0x8029AB18 |
| us-8029abac | kyoshin/makecrystal/CMCGetItemBox | `openMCGetItemBoxSysWin` | openMCGetItemBoxSysWin | 0x8029ABAC |
| us-8029acd4 | kyoshin/makecrystal/CMCGetItemBox | `forwardMCSubObj88` | forwardMCSubObj88 | 0x8029ACD4 |
| us-8029b000 | kyoshin/makecrystal/CMCGetItemBox | `openMCGetItemBoxPhase2` | openMCGetItemBoxPhase2 | 0x8029B000 |
| us-8029b0e8 | kyoshin/makecrystal/CMCGetItemBox | `closeMCGetItemBoxPhase2` | closeMCGetItemBoxPhase2 | 0x8029B0E8 |
| us-8029bbf8 | kyoshin/makecrystal/CMCGetItemBox | `initMCGetItemBoxList` | initMCGetItemBoxList | 0x8029BBF8 |
| us-8029c078 | kyoshin/makecrystal/CMCGetItemBox | `moveMCGetItemBoxCursor` | moveMCGetItemBoxCursor | 0x8029C078 |
| us-8029c8a0 | kyoshin/menu/CMenuTutorial | `__dt__13CMenuTutorialFv` | CMenuTutorial::~CMenuTutorial() | 0x8029C8A0 |
| us-8029d724 | kyoshin/CTutorial | `Tutorial_ReloadTutorialData` | Tutorial_ReloadTutorialData | 0x8029D724 |
| us-8029d8e0 | kyoshin/CTutorial | `OnFileEvent__9CTutorialFP10CEventFile` | CTutorial::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x8029D8E0 |
| us-8029e5ac | kyoshin/menu/CMenuOption | `CMenuOption_dtorAdj58` | CMenuOption_dtorAdj58 | 0x8029E5AC |
| us-802a1100 | kyoshin/menu/CMenuSkipTimer | `__dt__14CMenuSkipTimerFv` | CMenuSkipTimer::~CMenuSkipTimer() | 0x802A1100 |
| us-802a19c4 | kyoshin/CSkipTimer | `CSkipTimer2RebuildLayout` | CSkipTimer2RebuildLayout | 0x802A19C4 |
| us-802a1b98 | kyoshin/CSkipTimer | `CSkipTimer2Start` | CSkipTimer2Start | 0x802A1B98 |
| us-802a1d4c | kyoshin/CSkipTimer | `CSkipTimer2OnRewindDone` | CSkipTimer2OnRewindDone | 0x802A1D4C |
| us-802a1f60 | kyoshin/CSkipTimer | `CSkipTimer2SelectSlotPane` | CSkipTimer2SelectSlotPane | 0x802A1F60 |
| us-802a2410 | kyoshin/CSkipTimer | `CSkipTimerTick` | CSkipTimerTick | 0x802A2410 |
| us-802a24f0 | kyoshin/CSkipTimer | `CSkipTimerDraw` | CSkipTimerDraw | 0x802A24F0 |
| us-802a2564 | kyoshin/CSkipTimer | `CSkipTimerTeardown` | CSkipTimerTeardown | 0x802A2564 |
| us-802a2790 | kyoshin/CSkipTimer | `CSkipTimerConfirmSkip` | CSkipTimerConfirmSkip | 0x802A2790 |
| us-802a287c | kyoshin/CSkipTimer | `CSkipTimerEngageSkip` | CSkipTimerEngageSkip | 0x802A287C |
| us-802a2ae0 | kyoshin/CSkipTimer | `CSkipTimerActivateTimer` | CSkipTimerActivateTimer | 0x802A2AE0 |
| us-802a300c | kyoshin/cf/chain/CChainEffect | `__ct__Q22cf12CChainEffectFv` | cf::CChainEffect::CChainEffect() | 0x802A300C |
| us-802a3bec | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_CreateSingleton` | CCharVoiceMan_CreateSingleton | 0x802A3BEC |
| us-802a4438 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_HookBattleSlotVoice` | CCharVoiceMan_HookBattleSlotVoice | 0x802A4438 |
| us-802a45dc | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_TryPlayCharVoice` | CCharVoiceMan_TryPlayCharVoice | 0x802A45DC |
| us-802a46e8 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_PushFreshSoundNode` | CCharVoiceMan_PushFreshSoundNode | 0x802A46E8 |
| us-802a47ac | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_EnqueuePendingActionVoice` | CCharVoiceMan_EnqueuePendingActionVoice | 0x802A47AC |
| us-802a4944 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_LatchPendingVoiceId` | CCharVoiceMan_LatchPendingVoiceId | 0x802A4944 |
| us-802a4984 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_EnqueueBreakAndGaugeVoice` | CCharVoiceMan_EnqueueBreakAndGaugeVoice | 0x802A4984 |
| us-802a4a60 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_TriggerBattleOrPlainVoice` | CCharVoiceMan_TriggerBattleOrPlainVoice | 0x802A4A60 |
| us-802a4b58 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_EnqueueFaintVoice` | CCharVoiceMan_EnqueueFaintVoice | 0x802A4B58 |
| us-802a4be8 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_HookPendingBattleVoice` | CCharVoiceMan_HookPendingBattleVoice | 0x802A4BE8 |
| us-802a4c8c | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_TickPlayerVoiceCount` | CCharVoiceMan_TickPlayerVoiceCount | 0x802A4C8C |
| us-802a4e0c | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_EnqueueDamageVoice` | CCharVoiceMan_EnqueueDamageVoice | 0x802A4E0C |
| us-802a53bc | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_EnqueueGaugePairVoice` | CCharVoiceMan_EnqueueGaugePairVoice | 0x802A53BC |
| us-802a5440 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_FlushPendingActorVoice` | CCharVoiceMan_FlushPendingActorVoice | 0x802A5440 |
| us-802a5740 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_EnqueuePcStateVoice` | CCharVoiceMan_EnqueuePcStateVoice | 0x802A5740 |
| us-802a57a8 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_EnqueuePcActionVoice` | CCharVoiceMan_EnqueuePcActionVoice | 0x802A57A8 |
| us-802a5810 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_EnqueueArtsVoice` | CCharVoiceMan_EnqueueArtsVoice | 0x802A5810 |
| us-802a5878 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_EnqueueItemVoice` | CCharVoiceMan_EnqueueItemVoice | 0x802A5878 |
| us-802a5f38 | kyoshin/cf/voice/CCharVoiceMan | `CCharVoiceMan_FactoryEvent2` | CCharVoiceMan_FactoryEvent2 | 0x802A5F38 |
| us-802a6fa4 | kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END | `__ct__802A4870` | constructor | 0x802A6FA4 |
| us-802a70b0 | kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END | `func_802A497C` | func_802A497C | 0x802A70B0 |
| us-802a71ac | kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END | `func_802A4A78` | func_802A4A78 | 0x802A71AC |
| us-802a7f64 | kyoshin/cf/voice/cvsys/CVS_THREAD_CHAIN | `__ct__802A5830` | constructor | 0x802A7F64 |
| us-802a82bc | kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN | `__ct__802A5B88` | constructor | 0x802A82BC |
| us-802a88b0 | kyoshin/cf/voice/cvsys/CVS_THREAD_EHP | `selectEhpVoiceByHp` | selectEhpVoiceByHp | 0x802A88B0 |
| us-802a8b3c | kyoshin/cf/voice/cvsys/CVS_THREAD_EHP | `startEhpVoice` | startEhpVoice | 0x802A8B3C |
| us-802aa8a8 | kyoshin/cf/voice/cvsys/CVS_THREAD_PARTY_GAGE | `__ct__CVS_THREAD_PARTY_GAGE` | constructor | 0x802AA8A8 |
| us-802aae00 | kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE | `__ct__802A86CC` | constructor | 0x802AAE00 |
| us-802ab338 | kyoshin/cf/voice/cvsys/CVS_THREAD_SUDDEN | `__ct__802A8C04` | constructor | 0x802AB338 |
| us-802ac440 | kyoshin/menu/CMenuBattleChain | `Init__16CMenuBattleChainFv` | CMenuBattleChain::Init() | 0x802AC440 |
| us-802ac674 | kyoshin/menu/CMenuBattleChain | `Move__16CMenuBattleChainFv` | CMenuBattleChain::Move() | 0x802AC674 |
| us-802accc0 | kyoshin/menu/CMenuBattleChain | `applyChainColorScheme` | applyChainColorScheme | 0x802ACCC0 |
| us-802ae038 | kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END_SP | `CVSBattleEndSPResetAndSweep` | CVSBattleEndSPResetAndSweep | 0x802AE038 |
| us-802ae278 | kyoshin/cf/CfGimmickSaveOff | `__ct__cf_CfGimmickSaveOff` | constructor | 0x802AE278 |
| us-802af798 | kyoshin/CTutorialList | `TutorialList_LoadLayoutFiles` | TutorialList_LoadLayoutFiles | 0x802AF798 |
| us-802af818 | kyoshin/CTutorialList | `TutorialList_UpdatePerFrame` | TutorialList_UpdatePerFrame | 0x802AF818 |
| us-802af8c0 | kyoshin/CTutorialList | `TutorialList_DrawVisible` | TutorialList_DrawVisible | 0x802AF8C0 |
| us-802afa40 | kyoshin/CTutorialList | `TutorialList_OpenListInit` | TutorialList_OpenListInit | 0x802AFA40 |
| us-802afad8 | kyoshin/CTutorialList | `TutorialList_BeginClose` | TutorialList_BeginClose | 0x802AFAD8 |
| us-802afb3c | kyoshin/CTutorialList | `TutorialList_PageUpIdle` | TutorialList_PageUpIdle | 0x802AFB3C |
| us-802afc4c | kyoshin/CTutorialList | `TutorialList_PageUpWrap` | TutorialList_PageUpWrap | 0x802AFC4C |
| us-802afd70 | kyoshin/CTutorialList | `TutorialList_PageDownStep` | TutorialList_PageDownStep | 0x802AFD70 |
| us-802afe60 | kyoshin/CTutorialList | `TutorialList_PageDownBlock` | TutorialList_PageDownBlock | 0x802AFE60 |
| us-802aff90 | kyoshin/CTutorialList | `TutorialList_ConfirmOpenMenu` | TutorialList_ConfirmOpenMenu | 0x802AFF90 |
| us-802b00bc | kyoshin/CTutorialList | `TutorialList_IsSortMenuActive` | TutorialList_IsSortMenuActive | 0x802B00BC |
| us-802b00c4 | kyoshin/CTutorialList | `TutorialList_AdvanceSelection` | TutorialList_AdvanceSelection | 0x802B00C4 |
| us-802b0144 | kyoshin/CTutorialList | `TutorialList_RebuildAfterLoad` | TutorialList_RebuildAfterLoad | 0x802B0144 |
| us-802b01f0 | kyoshin/CTutorialList | `TutorialList_GetMenuStateCode` | TutorialList_GetMenuStateCode | 0x802B01F0 |
| us-802b0220 | kyoshin/CTutorialList | `TutorialList_OnOpenAnimDone` | TutorialList_OnOpenAnimDone | 0x802B0220 |
| us-802b0550 | kyoshin/CTutorialList | `TutorialList_MoveCursorToRow` | TutorialList_MoveCursorToRow | 0x802B0550 |
| us-802b061c | kyoshin/CTutorialList | `TutorialList_RebuildSortEntries` | TutorialList_RebuildSortEntries | 0x802B061C |
| us-802b0d64 | kyoshin/CLoad | `CLoadTeardownLayout` | CLoadTeardownLayout | 0x802B0D64 |
| us-802b0fcc | kyoshin/CLoad | `CLoadFinishRetryIdle` | CLoadFinishRetryIdle | 0x802B0FCC |
| us-802b1034 | kyoshin/CLoad | `OnFileEvent__5CLoadFP10CEventFile` | CLoad::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x802B1034 |
| us-802b123c | kyoshin/CNandData | `__sinit_\CNandData_cpp` | __sinit_\CNandData_cpp | 0x802B123C |
| us-802b1634 | kyoshin/plugin/pluginHelp | `PlugHelp_QueueTutorial1` | PlugHelp_QueueTutorial1 | 0x802B1634 |
| us-802b16a0 | kyoshin/plugin/pluginHelp | `PlugHelp_QueueTutorial3E` | PlugHelp_QueueTutorial3E | 0x802B16A0 |
| us-802b170c | kyoshin/plugin/pluginHelp | `PlugHelp_OpenPTChangeMenu` | PlugHelp_OpenPTChangeMenu | 0x802B170C |
| us-802b1bac | kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_MAIN | `BmVoice_FindRatio` | BmVoice_FindRatio | 0x802B1BAC |
| us-802b24f0 | kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_BEGIN | `__ct__802AFA80` | constructor(A, 8, 0) | 0x802B24F0 |
| us-802b2738 | kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_BEGIN | `BtlBeg_ReinitSlots_FCC8` | BtlBeg_ReinitSlots_FCC8 | 0x802B2738 |
| us-802b885c | kyoshin/menu/CMenuTitle | `__ct__CMenuTitle` | constructor | 0x802B885C |
| us-802b9038 | kyoshin/CTitle | `CTitleLogo_stepState1` | CTitleLogo_stepState1 | 0x802B9038 |
| us-802b93e0 | kyoshin/CTitle | `CTitleMenu_build` | CTitleMenu_build | 0x802B93E0 |
| us-802b97cc | kyoshin/CTitle | `CTitleMenu_stepState4` | CTitleMenu_stepState4 | 0x802B97CC |
| us-802b9f18 | kyoshin/CTitle | `CTitle_draw` | CTitle_draw | 0x802B9F18 |
| us-802ba048 | kyoshin/CTitle | `CTitle_beginLogoOutro` | CTitle_beginLogoOutro | 0x802BA048 |
| us-802ba314 | kyoshin/CTitle | `CTitle_enterSelection` | CTitle_enterSelection | 0x802BA314 |
| us-802ba6d4 | kyoshin/help/CHelp | `CHelp_ForwardPartyNotice` | CHelp_ForwardPartyNotice | 0x802BA6D4 |
| us-802ba80c | kyoshin/help/CHelp_ArtsSet | `isHelpAvailable__Q22cf13CHelp_ArtsSetFv` | cf::CHelp_ArtsSet::isHelpAvailable() | 0x802BA80C |
| us-802baa98 | kyoshin/help/CHelp_EnemyEnable | `areAllEnemiesEnabled__Q22cf17CHelp_EnemyEnableFv` | cf::CHelp_EnemyEnable::areAllEnemiesEnabled() | 0x802BAA98 |
| us-802bb338 | kyoshin/help/CHelp_Target | `CHelp_UnkVirtualFunc3__Q22cf12CHelp_TargetFv` | cf::CHelp_Target::CHelp_UnkVirtualFunc3() | 0x802BB338 |
| us-802bbad4 | kyoshin/code_802B9064 | `BattleGaugeTierVoiceGate` | BattleGaugeTierVoiceGate | 0x802BBAD4 |
| us-802bbd14 | kyoshin/CBattery | `loadBatteryArchive__8CBatteryFv` | CBattery::loadBatteryArchive() | 0x802BBD14 |
| us-802bbdd4 | kyoshin/CBattery | `releaseLayout__8CBatteryFv` | CBattery::releaseLayout() | 0x802BBDD4 |
| us-802bbf40 | kyoshin/CBattery | `OnFileEvent__8CBatteryFP10CEventFile` | CBattery::OnFileEvent(...*, 1, 0, C, E, v, e, n, t, int, long, e) | 0x802BBF40 |
| us-802ddba0 | RVL_SDK/src/revolution/bte/gki/gki_buffer | `GKI_getbuf` | GKI_getbuf | 0x802DDBA0 |
| us-802dde30 | RVL_SDK/src/revolution/bte/gki/gki_buffer | `GKI_freebuf` | GKI_freebuf | 0x802DDE30 |
| us-802ddff0 | RVL_SDK/src/revolution/bte/gki/gki_buffer | `GKI_send_msg` | GKI_send_msg | 0x802DDFF0 |
| us-802de260 | RVL_SDK/src/revolution/bte/gki/gki_buffer | `GKI_enqueue` | GKI_enqueue | 0x802DE260 |
| us-802de3a0 | RVL_SDK/src/revolution/bte/gki/gki_buffer | `GKI_enqueue_head` | GKI_enqueue_head | 0x802DE3A0 |
| us-802de9e0 | RVL_SDK/src/revolution/bte/gki/gki_buffer | `GKI_delete_pool` | GKI_delete_pool | 0x802DE9E0 |
| us-802e6aa4 | RVL_SDK/src/revolution/bte/bta/hh/bta_hh_utils | `bta_hh_find_cb` | bta_hh_find_cb | 0x802E6AA4 |
| us-802e6c60 | RVL_SDK/src/revolution/bte/bta/hh/bta_hh_utils | `bta_hh_add_device_to_list` | bta_hh_add_device_to_list | 0x802E6C60 |
| us-802e6d70 | RVL_SDK/src/revolution/bte/bta/hh/bta_hh_utils | `bta_hh_trace_dev_db` | bta_hh_trace_dev_db | 0x802E6D70 |
| us-802e6ef0 | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `btm_acl_created` | btm_acl_created | 0x802E6EF0 |
| us-802e732c | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `BTM_SwitchRole` | BTM_SwitchRole | 0x802E732C |
| us-802e76b8 | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `BTM_SetLinkPolicy` | BTM_SetLinkPolicy | 0x802E76B8 |
| us-802e7a48 | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `btm_read_remote_features_complete` | btm_read_remote_features_complete | 0x802E7A48 |
| us-802e7c84 | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `BTM_IsAclConnectionUp` | BTM_IsAclConnectionUp | 0x802E7C84 |
| us-802e8334 | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `btm_qos_setup_complete` | btm_qos_setup_complete | 0x802E8334 |
| us-802e840c | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `BTM_ReadRSSI` | BTM_ReadRSSI | 0x802E840C |
| us-802e8548 | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `BTM_ReadLinkQuality` | BTM_ReadLinkQuality | 0x802E8548 |
| us-802e8684 | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `btm_read_rssi_complete` | btm_read_rssi_complete | 0x802E8684 |
| us-802e87a0 | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `btm_read_link_quality_complete` | btm_read_link_quality_complete | 0x802E87A0 |
| us-802e8974 | RVL_SDK/src/revolution/bte/stack/btm/btm_acl | `btm_chg_all_acl_pkt_types` | btm_chg_all_acl_pkt_types | 0x802E8974 |
| us-802f6e0c | RVL_SDK/src/revolution/bte/stack/hid/hidh_api | `HID_HostWriteDev` | HID_HostWriteDev | 0x802F6E0C |
| us-802f6fec | RVL_SDK/src/revolution/bte/stack/hid/hidh_api | `HID_HostSetSecurityLevel` | HID_HostSetSecurityLevel | 0x802F6FEC |
| us-802f9254 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api | `L2CA_Register` | L2CA_Register | 0x802F9254 |
| us-802f93f0 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api | `L2CA_Deregister` | L2CA_Deregister | 0x802F93F0 |
| us-802f9480 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api | `L2CA_ConnectReq` | L2CA_ConnectReq | 0x802F9480 |
| us-802f96b4 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api | `L2CA_ConnectRsp` | L2CA_ConnectRsp | 0x802F96B4 |
| us-802f9844 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api | `L2CA_ConfigReq` | L2CA_ConfigReq | 0x802F9844 |
| us-802f98fc | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api | `L2CA_ConfigRsp` | L2CA_ConfigRsp | 0x802F98FC |
| us-802f99cc | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api | `L2CA_DisconnectReq` | L2CA_DisconnectReq | 0x802F99CC |
| us-802f9a70 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api | `L2CA_DisconnectRsp` | L2CA_DisconnectRsp | 0x802F9A70 |
| us-802f9b14 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api | `L2CA_DataWrite` | L2CA_DataWrite | 0x802F9B14 |
| us-802f9c38 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api | `L2CA_SetIdleTimeout` | L2CA_SetIdleTimeout | 0x802F9C38 |
| us-802f9e20 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_csm | `l2c_csm_execute` | l2c_csm_execute | 0x802F9E20 |
| us-802f9e6c | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_csm | `l2c_csm_closed` | l2c_csm_closed | 0x802F9E6C |
| us-802fa100 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_csm | `l2c_csm_orig_w4_sec_comp` | l2c_csm_orig_w4_sec_comp | 0x802FA100 |
| us-802fa270 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_csm | `l2c_csm_term_w4_sec_comp` | l2c_csm_term_w4_sec_comp | 0x802FA270 |
| us-802fa3d8 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_csm | `l2c_csm_w4_l2cap_connect_rsp` | l2c_csm_w4_l2cap_connect_rsp | 0x802FA3D8 |
| us-802fa61c | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_csm | `l2c_csm_w4_l2ca_connect_rsp` | l2c_csm_w4_l2ca_connect_rsp | 0x802FA61C |
| us-802fabbc | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_csm | `l2c_csm_open` | l2c_csm_open | 0x802FABBC |
| us-802fae18 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_csm | `l2c_csm_w4_l2cap_disconnect_rsp` | l2c_csm_w4_l2cap_disconnect_rsp | 0x802FAE18 |
| us-802faffc | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_csm | `l2c_csm_w4_l2ca_disconnect_rsp` | l2c_csm_w4_l2ca_disconnect_rsp | 0x802FAFFC |
| us-802fb158 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_csm | `forward_peer_data` | forward_peer_data | 0x802FB158 |
| us-802fd878 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_cmd_reject` | l2cu_send_peer_cmd_reject | 0x802FD878 |
| us-802fd974 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_connect_req` | l2cu_send_peer_connect_req | 0x802FD974 |
| us-802fda38 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_connect_rsp` | l2cu_send_peer_connect_rsp | 0x802FDA38 |
| us-802fdb10 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_reject_connection` | l2cu_reject_connection | 0x802FDB10 |
| us-802fdbd0 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_config_req` | l2cu_send_peer_config_req | 0x802FDBD0 |
| us-802fdec8 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_config_rsp` | l2cu_send_peer_config_rsp | 0x802FDEC8 |
| us-802fe1c0 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_config_rej` | l2cu_send_peer_config_rej | 0x802FE1C0 |
| us-802fe374 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_disc_req` | l2cu_send_peer_disc_req | 0x802FE374 |
| us-802fe448 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_disc_rsp` | l2cu_send_peer_disc_rsp | 0x802FE448 |
| us-802fe508 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_echo_req` | l2cu_send_peer_echo_req | 0x802FE508 |
| us-802fe678 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_echo_rsp` | l2cu_send_peer_echo_rsp | 0x802FE678 |
| us-802fe814 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_send_peer_info_rsp` | l2cu_send_peer_info_rsp | 0x802FE814 |
| us-802ff01c | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_create_conn` | l2cu_create_conn | 0x802FF01C |
| us-802ff134 | RVL_SDK/src/revolution/bte/stack/l2cap/l2c_utils | `l2cu_create_conn_after_switch` | l2cu_create_conn_after_switch | 0x802FF134 |
| us-802ff3cc | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_StartCnf` | PORT_StartCnf | 0x802FF3CC |
| us-802ff52c | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_StartInd` | PORT_StartInd | 0x802FF52C |
| us-802ff5c8 | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_ParNegInd` | PORT_ParNegInd | 0x802FF5C8 |
| us-802ff778 | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_ParNegCnf` | PORT_ParNegCnf | 0x802FF778 |
| us-802ff87c | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_DlcEstablishInd` | PORT_DlcEstablishInd | 0x802FF87C |
| us-802ff9bc | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_DlcEstablishCnf` | PORT_DlcEstablishCnf | 0x802FF9BC |
| us-802ffb00 | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_PortNegInd` | PORT_PortNegInd | 0x802FFB00 |
| us-802ffc50 | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_PortNegCnf` | PORT_PortNegCnf | 0x802FFC50 |
| us-802ffee0 | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_ControlCnf` | PORT_ControlCnf | 0x802FFEE0 |
| us-802fffa4 | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_LineStatusInd` | PORT_LineStatusInd | 0x802FFFA4 |
| us-8030006c | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_DlcReleaseInd` | PORT_DlcReleaseInd | 0x8030006C |
| us-803000d4 | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_CloseInd` | PORT_CloseInd | 0x803000D4 |
| us-80300174 | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `Port_TimeOutCloseMux` | Port_TimeOutCloseMux | 0x80300174 |
| us-8030020c | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_DataInd` | PORT_DataInd | 0x8030020C |
| us-8030040c | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `PORT_FlowInd` | PORT_FlowInd | 0x8030040C |
| us-8030053c | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `port_rfc_send_tx_data` | port_rfc_send_tx_data | 0x8030053C |
| us-80300634 | RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc | `port_rfc_closed` | port_rfc_closed | 0x80300634 |
| us-803016d4 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_mx_fsm | `rfc_mx_sm_execute` | rfc_mx_sm_execute | 0x803016D4 |
| us-80301718 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_mx_fsm | `rfc_mx_sm_state_idle` | rfc_mx_sm_state_idle | 0x80301718 |
| us-8030192c | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_mx_fsm | `rfc_mx_sm_state_wait_conn_cnf` | rfc_mx_sm_state_wait_conn_cnf | 0x8030192C |
| us-80301ae8 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_mx_fsm | `rfc_mx_sm_state_configure` | rfc_mx_sm_state_configure | 0x80301AE8 |
| us-80301c10 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_mx_fsm | `rfc_mx_sm_sabme_wait_ua` | rfc_mx_sm_sabme_wait_ua | 0x80301C10 |
| us-80301d84 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_mx_fsm | `rfc_mx_sm_state_wait_sabme` | rfc_mx_sm_state_wait_sabme | 0x80301D84 |
| us-80301ea4 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_mx_fsm | `rfc_mx_sm_state_connected` | rfc_mx_sm_state_connected | 0x80301EA4 |
| us-80301fbc | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_mx_fsm | `rfc_mx_sm_state_disc_wait_ua` | rfc_mx_sm_state_disc_wait_ua | 0x80301FBC |
| us-80302134 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_mx_fsm | `rfc_mx_conf_cnf` | rfc_mx_conf_cnf | 0x80302134 |
| us-80302228 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_mx_fsm | `rfc_mx_conf_ind` | rfc_mx_conf_ind | 0x80302228 |
| us-80302318 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm | `rfc_port_sm_execute` | rfc_port_sm_execute | 0x80302318 |
| us-803023a8 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm | `rfc_port_sm_state_closed` | rfc_port_sm_state_closed | 0x803023A8 |
| us-8030251c | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm | `rfc_port_sm_sabme_wait_ua` | rfc_port_sm_sabme_wait_ua | 0x8030251C |
| us-803026b8 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm | `rfc_port_sm_term_wait_sec_check` | rfc_port_sm_term_wait_sec_check | 0x803026B8 |
| us-80302874 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm | `rfc_port_sm_orig_wait_sec_check` | rfc_port_sm_orig_wait_sec_check | 0x80302874 |
| us-803029c4 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm | `rfc_port_sm_opened` | rfc_port_sm_opened | 0x803029C4 |
| us-80302bb0 | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm | `rfc_port_sm_disc_wait_ua` | rfc_port_sm_disc_wait_ua | 0x80302BB0 |
| us-80302ccc | RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm | `rfc_process_pn` | rfc_process_pn | 0x80302CCC |
| us-8030c430 | RVL_SDK/src/revolution/dsp/dsp_task | `__DSP_boot_task` | __DSP_boot_task | 0x8030C430 |
| us-80312f50 | RVL_SDK/src/revolution/dvd/dvd_broadway | `doTransactionCallback` | doTransactionCallback | 0x80312F50 |
| us-80313010 | RVL_SDK/src/revolution/dvd/dvd_broadway | `doPrepareCoverRegisterCallback` | doPrepareCoverRegisterCallback | 0x80313010 |
| us-803130d0 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowFinalize` | DVDLowFinalize | 0x803130D0 |
| us-80313120 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowInit` | DVDLowInit | 0x80313120 |
| us-80313390 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowReadDiskID` | DVDLowReadDiskID | 0x80313390 |
| us-80313520 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowOpenPartition` | DVDLowOpenPartition | 0x80313520 |
| us-80313790 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowOpenPartitionWithTmdAndTicketView` | DVDLowOpenPartitionWithTmdAndTicketView | 0x80313790 |
| us-80313a00 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowGetNoDiscBufferSizes` | DVDLowGetNoDiscBufferSizes | 0x80313A00 |
| us-80313c10 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowGetNoDiscOpenPartitionParams` | DVDLowGetNoDiscOpenPartitionParams | 0x80313C10 |
| us-80313eb0 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowClosePartition` | DVDLowClosePartition | 0x80313EB0 |
| us-80314020 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowUnencryptedRead` | DVDLowUnencryptedRead | 0x80314020 |
| us-803141c0 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowStopMotor` | DVDLowStopMotor | 0x803141C0 |
| us-80314360 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowInquiry` | DVDLowInquiry | 0x80314360 |
| us-803144d0 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowRequestError` | DVDLowRequestError | 0x803144D0 |
| us-80314650 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowReset` | DVDLowReset | 0x80314650 |
| us-803147d0 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowAudioBufferConfig` | DVDLowAudioBufferConfig | 0x803147D0 |
| us-80314970 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowReportKey` | DVDLowReportKey | 0x80314970 |
| us-80314b10 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowSetMaximumRotation` | DVDLowSetMaximumRotation | 0x80314B10 |
| us-80314ca0 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowRead` | DVDLowRead | 0x80314CA0 |
| us-80314e50 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowSeek` | DVDLowSeek | 0x80314E50 |
| us-80315000 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowPrepareCoverRegister` | DVDLowPrepareCoverRegister | 0x80315000 |
| us-80315170 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowPrepareStatusRegister` | DVDLowPrepareStatusRegister | 0x80315170 |
| us-803152e0 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowPrepareControlRegister` | DVDLowPrepareControlRegister | 0x803152E0 |
| us-80315480 | RVL_SDK/src/revolution/dvd/dvd_broadway | `DVDLowClearCoverInterrupt` | DVDLowClearCoverInterrupt | 0x80315480 |
| us-803218c0 | RVL_SDK/src/revolution/hbm/HBMFrameController | `calc__Q210homebutton15FrameControllerFv` | calc() | 0x803218C0 |
| us-80321b20 | RVL_SDK/src/revolution/hbm/HBMGUIManager | `drawLine___Q210homebutton3guiFfffffUcR8_GXColor` | drawLine(float, float, float, float, float, char, R, 8, _, G, X, C, o, long, o, r) | 0x80321B20 |
| us-80322860 | RVL_SDK/src/revolution/hbm/HBMGUIManager | `walkInChildren__Q310homebutton3gui11PaneManagerFRQ36nw4hbm2ut30LinkList<Q36nw4hbm3lyt4Pane,4>` | walkInChildren(R, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 2, u, t, 3, 0, L, int, n, k, L, int, short, t, <, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 3, long, y, t, 4, ...*, a, n, e, ,, 4, >) | 0x80322860 |
| us-80322db0 | RVL_SDK/src/revolution/hbm/HBMGUIManager | `contain__Q310homebutton3gui13PaneComponentFff` | contain(float, float) | 0x80322DB0 |
| us-80322ef0 | RVL_SDK/src/revolution/hbm/HBMGUIManager | `draw__Q310homebutton3gui13PaneComponentFv` | draw() | 0x80322EF0 |
| us-803235b0 | RVL_SDK/src/revolution/hbm/HBMController | `__ct__Q210homebutton10ControllerFiPQ210homebutton9RemoteSpk` | constructor(int, ...*, Q, 2, 1, 0, unsigned char, o, m, e, bool, u, t, t, o, n, 9, R, e, m, o, t, e, S, p, k) | 0x803235B0 |
| us-80323670 | RVL_SDK/src/revolution/hbm/HBMController | `__dt__Q210homebutton10ControllerFv` | destructor() | 0x80323670 |
| us-803239f0 | RVL_SDK/src/revolution/hbm/HBMController | `setInValidPos__Q210homebutton10ControllerFv` | setInValidPos() | 0x803239F0 |
| us-80323a40 | RVL_SDK/src/revolution/hbm/HBMController | `playSound__Q210homebutton10ControllerFi` | playSound(int) | 0x80323A40 |
| us-80324220 | RVL_SDK/src/revolution/hbm/HBMRemoteSpk | `__ct__Q210homebutton9RemoteSpkFPv` | constructor(...*, v) | 0x80324220 |
| us-803242e0 | RVL_SDK/src/revolution/hbm/HBMRemoteSpk | `__dt__Q210homebutton9RemoteSpkFv` | destructor() | 0x803242E0 |
| us-803252a0 | RVL_SDK/src/revolution/hbm/HBMAxSound | `SetVolumeAllSeq__10homebuttonFf` | homebutton::SetVolumeAllSeq(float) | 0x803252A0 |
| us-80325da0 | RVL_SDK/src/revolution/hbm/HBMBase | `create__Q210homebutton10HomeButtonFv` | create() | 0x80325DA0 |
| us-80326dc0 | RVL_SDK/src/revolution/hbm/HBMBase | `init_volume__Q210homebutton10HomeButtonFv` | init_volume() | 0x80326DC0 |
| us-80326fc0 | RVL_SDK/src/revolution/hbm/HBMBase | `init_sound__Q210homebutton10HomeButtonFv` | init_sound() | 0x80326FC0 |
| us-80327360 | RVL_SDK/src/revolution/hbm/HBMBase | `calc__Q210homebutton10HomeButtonFPC17HBMControllerData` | calc(const ...*, 1, 7, H, B, M, C, o, n, t, r, o, long, long, e, r, D, a, t, a) | 0x80327360 |
| us-803292b0 | RVL_SDK/src/revolution/hbm/HBMBase | `draw__Q210homebutton10HomeButtonFv` | draw() | 0x803292B0 |
| us-80329780 | RVL_SDK/src/revolution/hbm/HBMBase | `update__Q210homebutton10HomeButtonFPC17HBMControllerData` | update(const ...*, 1, 7, H, B, M, C, o, n, t, r, o, long, long, e, r, D, a, t, a) | 0x80329780 |
| us-8032a120 | RVL_SDK/src/revolution/hbm/HBMBase | `update_controller__Q210homebutton10HomeButtonFi` | update_controller(int) | 0x8032A120 |
| us-8032acc0 | RVL_SDK/src/revolution/hbm/HBMBase | `startPointEvent__Q210homebutton10HomeButtonFPCQ36nw4hbm3lyt4PanePv` | startPointEvent(const ...*, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 3, long, y, t, 4, ...*, a, n, e, ...*, v) | 0x8032ACC0 |
| us-8032bb10 | RVL_SDK/src/revolution/hbm/HBMBase | `startLeftEvent__Q210homebutton10HomeButtonFPCQ36nw4hbm3lyt4Pane` | startLeftEvent(const ...*, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 3, long, y, t, 4, ...*, a, n, e) | 0x8032BB10 |
| us-8032c5e0 | RVL_SDK/src/revolution/hbm/HBMBase | `startTrigEvent__Q210homebutton10HomeButtonFPCQ36nw4hbm3lyt4Pane` | startTrigEvent(const ...*, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 3, long, y, t, 4, ...*, a, n, e) | 0x8032C5E0 |
| us-8032dba0 | RVL_SDK/src/revolution/hbm/HBMBase | `setAdjustFlag__Q210homebutton10HomeButtonFi` | setAdjustFlag(int) | 0x8032DBA0 |
| us-8032dde0 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_animation | `GetStepCurveValue__27@unnamed@lyt_animation_cpp@FfPCQ46nw4hbm3lyt3res7StepKeyUl` | GetStepCurveValue27@unnamed@lyt_animation_cpp@FfPCQ46nw4hbm3lyt3res7StepKeyUl | 0x8032DDE0 |
| us-8032ded0 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_animation | `GetHermiteCurveValue__27@unnamed@lyt_animation_cpp@FfPCQ46nw4hbm3lyt3res10HermiteKeyUl` | GetHermiteCurveValue27@unnamed@lyt_animation_cpp@FfPCQ46nw4hbm3lyt3res10HermiteKeyUl | 0x8032DED0 |
| us-8032e0b0 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_animation | `__ct__Q36nw4hbm3lyt18AnimTransformBasicFv` | constructor() | 0x8032E0B0 |
| us-8032e490 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_animation | `Animate__Q36nw4hbm3lyt18AnimTransformBasicFUlPQ36nw4hbm3lyt4Pane` | Animate(long, ...*, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 3, long, y, t, 4, ...*, a, n, e) | 0x8032E490 |
| us-8032e670 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_animation | `Animate__Q36nw4hbm3lyt18AnimTransformBasicFUlPQ36nw4hbm3lyt8Material` | Animate(long, ...*, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 3, long, y, t, 8, M, a, t, e, r, int, a, long) | 0x8032E670 |
| us-8032e990 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_arcResourceAccessor | `FindNameResource__37@unnamed@lyt_arcResourceAccessor_cpp@FP9ARCHandlePCc` | FindNameResource37@unnamed@lyt_arcResourceAccessor_cpp@FP9ARCHandlePCc | 0x8032E990 |
| us-8032ea70 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_arcResourceAccessor | `GetResourceSub__37@unnamed@lyt_arcResourceAccessor_cpp@FP9ARCHandlePCcUlPCcPUl` | GetResourceSub37@unnamed@lyt_arcResourceAccessor_cpp@FP9ARCHandlePCcUlPCcPUl | 0x8032EA70 |
| us-803353f0 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_picture | `Append__Q36nw4hbm3lyt7PictureFRC9_GXTexObj` | Append(R, C, 9, _, G, X, T, e, long long, O, bool, j) | 0x803353F0 |
| us-80337600 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_window | `DrawFrame__Q36nw4hbm3lyt6WindowFRCQ36nw4hbm4math4VEC2RCQ46nw4hbm3lyt6Window5FrameRCQ36nw4hbm3lyt15WindowFrameSizeUc` | nw4hbm3lyt6WindowFRCQ36nw4hbm4math4V::DrawFrame(r, a, m, e, S, int, z, e, char) | 0x80337600 |
| us-80337c30 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_window | `DrawFrame4__Q36nw4hbm3lyt6WindowFRCQ36nw4hbm4math4VEC2PCQ46nw4hbm3lyt6Window5FrameRCQ36nw4hbm3lyt15WindowFrameSizeUc` | nw4hbm3lyt6WindowFRCQ36nw4hbm4math4V::DrawFrame4(r, a, m, e, S, int, z, e, char) | 0x80337C30 |
| us-80338360 | RVL_SDK/src/revolution/hbm/nw4hbm/lyt/lyt_window | `DrawFrame8__Q36nw4hbm3lyt6WindowFRCQ36nw4hbm4math4VEC2PCQ46nw4hbm3lyt6Window5FrameRCQ36nw4hbm3lyt15WindowFrameSizeUc` | nw4hbm3lyt6WindowFRCQ36nw4hbm4math4V::DrawFrame8(r, a, m, e, S, int, z, e, char) | 0x80338360 |
| us-8033b660 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_ResFontBase | `__ct__Q46nw4hbm2ut6detail11ResFontBaseFv` | constructor() | 0x8033B660 |
| us-8033c770 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `__ct__Q36nw4hbm2ut17TextWriterBase<c>Fv` | constructor() | 0x8033C770 |
| us-8033c830 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `SetLineHeight__Q36nw4hbm2ut17TextWriterBase<c>Ff` | SetLineHeight(float) | 0x8033C830 |
| us-8033c8d0 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `GetLineHeight__Q36nw4hbm2ut17TextWriterBase<c>CFv` | GetLineHeight() | 0x8033C8D0 |
| us-8033ca10 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcFormatStringWidth__Q36nw4hbm2ut17TextWriterBase<c>CFPCce` | CalcFormatStringWidth(const ...*, char, e) | 0x8033CA10 |
| us-8033cc10 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcFormatStringHeight__Q36nw4hbm2ut17TextWriterBase<c>CFPCce` | CalcFormatStringHeight(const ...*, char, e) | 0x8033CC10 |
| us-8033d150 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcStringWidth__Q36nw4hbm2ut17TextWriterBase<c>CFPCci` | CalcStringWidth(const ...*, char, int) | 0x8033D150 |
| us-8033d290 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcStringWidth__Q36nw4hbm2ut17TextWriterBase<c>CFPCc` | CalcStringWidth(const ...*, char) | 0x8033D290 |
| us-8033d3e0 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcStringHeight__Q36nw4hbm2ut17TextWriterBase<c>CFPCci` | CalcStringHeight(const ...*, char, int) | 0x8033D3E0 |
| us-8033d520 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcStringHeight__Q36nw4hbm2ut17TextWriterBase<c>CFPCc` | CalcStringHeight(const ...*, char) | 0x8033D520 |
| us-8033e000 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcLineWidth__Q36nw4hbm2ut17TextWriterBase<c>FPCci` | CalcLineWidth(const ...*, char, int) | 0x8033E000 |
| us-8033e550 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcStringRectImpl__Q36nw4hbm2ut17TextWriterBase<c>FPQ36nw4hbm2ut4RectPCci` | CalcStringRectImpl(...*, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 2, u, t, 4, R, e, char, t, const ...*, char, int) | 0x8033E550 |
| us-8033e670 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `PrintImpl__Q36nw4hbm2ut17TextWriterBase<c>FPCci` | PrintImpl(const ...*, char, int) | 0x8033E670 |
| us-8033ec20 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `AdjustCursor__Q36nw4hbm2ut17TextWriterBase<c>FPfPfPCci` | AdjustCursor(...*, float, ...*, float, const ...*, char, int) | 0x8033EC20 |
| us-8033f130 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `__ct__Q36nw4hbm2ut17TextWriterBase<w>Fv` | constructor() | 0x8033F130 |
| us-8033f1f0 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `SetLineHeight__Q36nw4hbm2ut17TextWriterBase<w>Ff` | SetLineHeight(float) | 0x8033F1F0 |
| us-8033f290 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `GetLineHeight__Q36nw4hbm2ut17TextWriterBase<w>CFv` | GetLineHeight() | 0x8033F290 |
| us-8033f3d0 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcFormatStringWidth__Q36nw4hbm2ut17TextWriterBase<w>CFPCwe` | CalcFormatStringWidth(const ...*, wchar_t, e) | 0x8033F3D0 |
| us-8033f5d0 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcFormatStringHeight__Q36nw4hbm2ut17TextWriterBase<w>CFPCwe` | CalcFormatStringHeight(const ...*, wchar_t, e) | 0x8033F5D0 |
| us-8033fb10 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcStringWidth__Q36nw4hbm2ut17TextWriterBase<w>CFPCwi` | CalcStringWidth(const ...*, wchar_t, int) | 0x8033FB10 |
| us-8033fc50 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcStringWidth__Q36nw4hbm2ut17TextWriterBase<w>CFPCw` | CalcStringWidth(const ...*, wchar_t) | 0x8033FC50 |
| us-8033fda0 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcStringHeight__Q36nw4hbm2ut17TextWriterBase<w>CFPCwi` | CalcStringHeight(const ...*, wchar_t, int) | 0x8033FDA0 |
| us-8033fee0 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcStringHeight__Q36nw4hbm2ut17TextWriterBase<w>CFPCw` | CalcStringHeight(const ...*, wchar_t) | 0x8033FEE0 |
| us-803409c0 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcLineWidth__Q36nw4hbm2ut17TextWriterBase<w>FPCwi` | CalcLineWidth(const ...*, wchar_t, int) | 0x803409C0 |
| us-80340f30 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `CalcStringRectImpl__Q36nw4hbm2ut17TextWriterBase<w>FPQ36nw4hbm2ut4RectPCwi` | CalcStringRectImpl(...*, Q, 3, 6, n, wchar_t, 4, unsigned char, bool, m, 2, u, t, 4, R, e, char, t, const ...*, wchar_t, int) | 0x80340F30 |
| us-80341050 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `PrintImpl__Q36nw4hbm2ut17TextWriterBase<w>FPCwi` | PrintImpl(const ...*, wchar_t, int) | 0x80341050 |
| us-80341620 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `AdjustCursor__Q36nw4hbm2ut17TextWriterBase<w>FPfPfPCwi` | AdjustCursor(...*, float, ...*, float, const ...*, wchar_t, int) | 0x80341620 |
| us-80341b30 | RVL_SDK/src/revolution/hbm/nw4hbm/ut/ut_TextWriterBase | `__sinit_\ut_TextWriterBase_cpp` | __sinit_\ut_TextWriterBase_cpp | 0x80341B30 |
| us-80353cc0 | RVL_SDK/src/revolution/nand/NANDCheck | `nandUserAreaCallback` | nandUserAreaCallback | 0x80353CC0 |
| us-80357820 | RVL_SDK/src/revolution/os/OSContext | `OSDumpContext` | OSDumpContext | 0x80357820 |
| us-80357b10 | RVL_SDK/src/revolution/os/OSContext | `__OSContextInit` | __OSContextInit | 0x80357B10 |
| us-80357e80 | RVL_SDK/src/revolution/os/OSError | `__OSUnhandledException` | __OSUnhandledException | 0x80357E80 |
| us-8035a750 | RVL_SDK/src/revolution/os/OSFont | `ReadFont` | ReadFont | 0x8035A750 |
| us-8035d060 | RVL_SDK/src/revolution/os/OSReset | `OSRestart` | OSRestart | 0x8035D060 |
| us-8035d3d0 | RVL_SDK/src/revolution/os/OSReset | `OSReturnToMenu` | OSReturnToMenu | 0x8035D3D0 |
| us-8035d4c0 | RVL_SDK/src/revolution/os/OSReset | `__OSHotResetForError` | __OSHotResetForError | 0x8035D4C0 |
| us-8035d550 | RVL_SDK/src/revolution/os/OSReset | `OSResetSystem` | OSResetSystem | 0x8035D550 |
| us-8035ff70 | RVL_SDK/src/revolution/os/OSStateTM | `__OSInitSTM` | __OSInitSTM | 0x8035FF70 |
| us-80360090 | RVL_SDK/src/revolution/os/OSStateTM | `__OSShutdownToSBY` | __OSShutdownToSBY | 0x80360090 |
| us-80360110 | RVL_SDK/src/revolution/os/OSStateTM | `__OSHotReset` | __OSHotReset | 0x80360110 |
| us-80360340 | RVL_SDK/src/revolution/os/OSStateTM | `__OSStateEventHandler` | __OSStateEventHandler | 0x80360340 |
| us-80365750 | RVL_SDK/src/revolution/usb/usb | `USB_LOG` | USB_LOG | 0x80365750 |
| us-80365800 | RVL_SDK/src/revolution/usb/usb | `USB_ERR` | USB_ERR | 0x80365800 |
| us-803658b0 | RVL_SDK/src/revolution/usb/usb | `IUSB_OpenLib` | IUSB_OpenLib | 0x803658B0 |
| us-803659c0 | RVL_SDK/src/revolution/usb/usb | `_intrBlkCtrlIsoCb` | _intrBlkCtrlIsoCb | 0x803659C0 |
| us-80365b60 | RVL_SDK/src/revolution/usb/usb | `IUSB_OpenDeviceIds` | IUSB_OpenDeviceIds | 0x80365B60 |
| us-80365cb0 | RVL_SDK/src/revolution/usb/usb | `IUSB_CloseDeviceAsync` | IUSB_CloseDeviceAsync | 0x80365CB0 |
| us-80365de0 | RVL_SDK/src/revolution/usb/usb | `__LongBlkMsgInt` | __LongBlkMsgInt | 0x80365DE0 |
| us-80366140 | RVL_SDK/src/revolution/usb/usb | `__IntrBlkMsgInt` | __IntrBlkMsgInt | 0x80366140 |
| us-80366660 | RVL_SDK/src/revolution/usb/usb | `__CtrlMsgInt` | __CtrlMsgInt | 0x80366660 |
| us-8037a090 | RVL_SDK/src/revolution/wud/WUD | `__wudDeleteFlushCallback` | __wudDeleteFlushCallback | 0x8037A090 |
| us-8037ed60 | RVL_SDK/src/revolution/wud/WUDHidHost | `WUDiHidHostEventCallback` | WUDiHidHostEventCallback | 0x8037ED60 |
| us-8037f2e0 | RVL_SDK/src/revolution/wud/WUDHidHost | `bta_hh_co_data` | bta_hh_co_data | 0x8037F2E0 |
| us-8037f320 | RVL_SDK/src/revolution/wud/WUDHidHost | `bta_hh_co_open` | bta_hh_co_open | 0x8037F320 |
| us-8037f330 | RVL_SDK/src/revolution/wud/WUDHidHost | `bta_hh_co_close` | bta_hh_co_close | 0x8037F330 |
| us-80386678 | CriWare/src/adx/adxt/adx_tlk | `ADXT_GetTimeReal` | ADXT_GetTimeReal | 0x80386678 |
| us-80387418 | CriWare/src/adx/adxt/adx_tlk | `ADXT_DiscardSmpl` | ADXT_DiscardSmpl | 0x80387418 |
| us-803cff8c | CriWare/src/sofdec/sfdcore/sfd/sfd_tim | `SFTIM_Tc2Time` | SFTIM_Tc2Time | 0x803CFF8C |
| us-803de1e8 | nw4r/src/g3d/res/g3d_resmat | `GetLyrRate__Q34nw4r3g3d9ResMatFurCFUl` | GetLyrRate(unsigned long) | 0x803DE1E8 |
| us-803de76c | nw4r/src/g3d/res/g3d_resmat | `GXGetIndTexMtx__Q34nw4r3g3d20ResMatIndMtxAndScaleCF14_GXIndTexMtxIDPQ34nw4r4math5MTX34` | GXGetIndTexMtx(1, 4, _, G, X, I, n, double, T, e, long long, M, t, long long, I, D, ...*, Q, 3, 4, n, wchar_t, 4, r, 4, m, a, t, unsigned char, 5, M, T, X, 3, 4) | 0x803DE76C |
| us-803dea2c | nw4r/src/g3d/res/g3d_resmat | `GXGetIndTexMtx__Q34nw4r3g3d20ResMatIndMtxAndScaleCF14_GXIndTexMtxIDPQ34nw4r4math5MTX34PSc` | GXGetIndTexMtx(1, 4, _, G, X, I, n, double, T, e, long long, M, t, long long, I, D, ...*, Q, 3, 4, n, wchar_t, 4, r, 4, m, a, t, unsigned char, 5, M, T, X, 3, 4, ...*, S, char) | 0x803DEA2C |
| us-803e1088 | nw4r/src/g3d/res/g3d_resanmchr | `GetAnmRotation__Q34nw4r3g3d27@unnamed@g3d_resanmchr_cpp@FPQ34nw4r4math5MTX34PQ34nw4r4math4VEC3RCQ34nw4r3g3d17ResAnmChrInfoDataPCQ34nw4r3g3d17ResAnmChrNodeDataPCQ44nw4r3g3d17ResAnmChrNodeData7AnmDataf` | nw4r3g3d27@unnamed@g3d_resanmchr_c::GetAnmRotation(...*, Q, 3, 4, n, wchar_t, 4, r, 4, m, a, t, unsigned char, 5, M, T, X, 3, 4, ...*, Q, 3, 4, n, wchar_t, 4, r, 4, m, a, t, unsigned char, 4, V, E, C, 3, R, C, Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 1, 7, R, e, short, A, n, m, C, unsigned char, r, I, n, float, o, D, a, t, a, const ...*, Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 1, 7, R, e, short, A, n, m, C, unsigned char, r, N, o, double, e, D, a, t, a, const ...*, Q, 4, 4, n, wchar_t, 4, r, 3, g, 3, double, 1, 7, R, e, short, A, n, m, C, unsigned char, r, N, o, double, e, D, a, t, a, 7, A, n, m, D, a, t, a, float) | 0x803E1088 |
| us-803e21f4 | nw4r/src/g3d/res/g3d_resanmchr | `GetRotateDeg__Q34nw4r3g3d12ChrAnmResultCFPQ34nw4r4math4VEC3` | GetRotateDeg(...*, Q, 3, 4, n, wchar_t, 4, r, 4, m, a, t, unsigned char, 4, V, E, C, 3) | 0x803E21F4 |
| us-803e25fc | nw4r/src/g3d/res/g3d_resanmchr | `SetTranslate__Q34nw4r3g3d12ChrAnmResultFPCQ34nw4r4math4VEC3` | SetTranslate(const ...*, Q, 3, 4, n, wchar_t, 4, r, 4, m, a, t, unsigned char, 4, V, E, C, 3) | 0x803E25FC |
| us-803e27ec | nw4r/src/g3d/res/g3d_resanmamblight | `GetAnmResult__Q34nw4r3g3d14ResAnmAmbLightCFPQ34nw4r3g3d17AmbLightAnmResultf` | GetAnmResult(...*, Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 1, 7, A, m, bool, L, int, g, unsigned char, t, A, n, m, R, e, short, u, long, t, float) | 0x803E27EC |
| us-803e28bc | nw4r/src/g3d/res/g3d_resanmlight | `GetAnmResult__Q34nw4r3g3d11ResAnmLightCFPQ34nw4r3g3d14LightAnmResultf` | GetAnmResult(...*, Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 1, 4, L, int, g, unsigned char, t, A, n, m, R, e, short, u, long, t, float) | 0x803E28BC |
| us-803f682c | nw4r/src/g3d/g3d_scnobj | `__ct__Q34nw4r3g3d6ScnObjFP12MEMAllocator` | constructor(...*, 1, 2, M, E, M, A, long, long, o, char, a, t, o, r) | 0x803F682C |
| us-803f6918 | nw4r/src/g3d/g3d_scnobj | `__dt__Q34nw4r3g3d6ScnObjFv` | destructor() | 0x803F6918 |
| us-803f69dc | nw4r/src/g3d/g3d_scnobj | `SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl` | SetScnObjOption(long, long) | 0x803F69DC |
| us-803f6b74 | nw4r/src/g3d/g3d_scnobj | `GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl` | GetScnObjOption(long, ...*, long) | 0x803F6B74 |
| us-803f6fac | nw4r/src/g3d/g3d_scnobj | `GetScaleProperty__Q34nw4r3g3d7ScnLeafCFv` | GetScaleProperty() | 0x803F6FAC |
| us-803f7300 | nw4r/src/g3d/g3d_scnobj | `Construct__Q34nw4r3g3d8ScnGroupFP12MEMAllocatorPUlUl` | Construct(...*, 1, 2, M, E, M, A, long, long, o, char, a, t, o, r, ...*, long, long) | 0x803F7300 |
| us-803f7d70 | nw4r/src/g3d/g3d_scnobj | `__ct__Q34nw4r3g3d8ScnGroupFP12MEMAllocatorPPQ34nw4r3g3d6ScnObjUl` | constructor(...*, 1, 2, M, E, M, A, long, long, o, char, a, t, o, r, ...*, ...*, Q, 3, 4, n, wchar_t, 4, r, 3, g, 3, double, 6, S, char, n, O, bool, j, long) | 0x803F7D70 |
| us-803f7de0 | nw4r/src/g3d/g3d_scnobj | `__dt__Q34nw4r3g3d8ScnGroupFv` | destructor() | 0x803F7DE0 |
| us-803f7ea4 | nw4r/src/g3d/g3d_scnobj | `IsDerivedFrom__Q34nw4r3g3d8ScnGroupCFQ44nw4r3g3d6G3dObj7TypeObj` | IsDerivedFrom(Q, 4, 4, n, wchar_t, 4, r, 3, g, 3, double, 6, G, 3, double, O, bool, j, 7, T, y, p, e, O, bool, j) | 0x803F7EA4 |
| us-803f7ef4 | nw4r/src/g3d/g3d_scnobj | `IsDerivedFrom__Q34nw4r3g3d6ScnObjCFQ44nw4r3g3d6G3dObj7TypeObj` | IsDerivedFrom(Q, 4, 4, n, wchar_t, 4, r, 3, g, 3, double, 6, G, 3, double, O, bool, j, 7, T, y, p, e, O, bool, j) | 0x803F7EF4 |
| us-804032ec | nw4r/src/lyt/lyt_picture | `Append__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt6TexMap` | Append(R, C, Q, 3, 4, n, wchar_t, 4, r, 3, long, y, t, 6, T, e, long long, M, a, p) | 0x804032EC |
| us-8040360c | nw4r/src/lyt/lyt_picture | `GetRuntimeTypeInfo__Q34nw4r3lyt7PictureCFv` | GetRuntimeTypeInfo() | 0x8040360C |
| us-80403614 | nw4r/src/lyt/lyt_picture | `__sinit_\lyt_picture_cpp` | __sinit_\lyt_picture_cpp | 0x80403614 |
| us-804036d4 | nw4r/src/lyt/lyt_textBox | `__ct__Q34nw4r3lyt7TextBoxFUs` | constructor(unsigned short) | 0x804036D4 |
| us-80404a38 | nw4r/src/lyt/lyt_textBox | `SetFont__Q34nw4r3lyt7TextBoxFPCQ34nw4r2ut4Font` | SetFont(o, n, t) | 0x80404A38 |
| us-80404fa4 | nw4r/src/lyt/lyt_textBox | `CalcStringRectImpl<w>__Q34nw4r3lyt25@unnamed@lyt_textBox_cpp@FPQ34nw4r2ut4RectPQ34nw4r2ut17TextWriterBase<w>PCwif_v` | nw4r3lyt25@unnamed@lyt_textBox_cpp::CalcStringRectImpl<w>(...*, Q, 3, 4, n, wchar_t, 4, r, 2, u, t, 4, R, e, char, t, ...*, Q, 3, 4, n, wchar_t, 4, r, 2, u, t, 1, 7, T, e, long long, t, W, r, int, t, e, r, B, a, short, e, <, wchar_t, >, const ...*, wchar_t, int, float, _, v) | 0x80404FA4 |
| us-8040602c | nw4r/src/lyt/lyt_window | `DrawFrame4__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2PCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc` | nw4r3lyt6WindowFRCQ34nw4r4math4VEC::PC::DrawFrame4(r, a, m, e, S, int, z, e, char) | 0x8040602C |
| us-80406770 | nw4r/src/lyt/lyt_window | `DrawFrame8__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2PCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc` | nw4r3lyt6WindowFRCQ34nw4r4math4VEC::PC::DrawFrame8(r, a, m, e, S, int, z, e, char) | 0x80406770 |
| us-8040a660 | nw4r/src/lyt/lyt_texMap | `Get__Q34nw4r3lyt6TexMapCFP9_GXTexObj` | Get(...*, 9, _, G, X, T, e, long long, O, bool, j) | 0x8040A660 |
| us-8040cf54 | nw4r/src/math/math_arithmetic | `FExp__Q34nw4r4math6detailFf` | FExp(float) | 0x8040CF54 |
| us-8040cfe0 | nw4r/src/math/math_arithmetic | `FLog__Q34nw4r4math6detailFf` | FLog(float) | 0x8040CFE0 |
| us-8040d06c | nw4r/src/math/math_arithmetic | `FrSqrt__Q24nw4r4mathFf` | FrSqrt(float) | 0x8040D06C |
| us-804192fc | nw4r/src/snd/snd_Lfo | `Update__Q44nw4r3snd6detail3LfoFi` | Update(int) | 0x804192FC |
| us-80419398 | nw4r/src/snd/snd_Lfo | `GetValue__Q44nw4r3snd6detail3LfoCFv` | GetValue() | 0x80419398 |
| us-804194cc | nw4r/src/snd/snd_MemorySoundArchive | `__ct__Q34nw4r3snd18MemorySoundArchiveFv` | constructor() | 0x804194CC |
| us-80419754 | nw4r/src/snd/snd_MemorySoundArchive | `OpenStream__Q34nw4r3snd18MemorySoundArchiveCFPviUlUl` | OpenStream(...*, v, int, long, long) | 0x80419754 |
| us-804221d0 | nw4r/src/snd/snd_SoundArchivePlayer | `PrepareWaveSoundImpl__Q34nw4r3snd18SoundArchivePlayerFPQ44nw4r3snd6detail9WaveSoundPCQ44nw4r3snd12SoundArchive9SoundInfoPCQ44nw4r3snd12SoundArchive13WaveSoundInfoQ54nw4r3snd14SoundStartable9StartInfo15StartOffsetTypei` | PrepareWaveSoundImpl(...*, Q, 4, 4, n, wchar_t, 4, r, 3, short, n, double, 6, double, e, t, a, int, long, 9, W, a, v, e, S, o, u, n, double, const ...*, Q, 4, 4, n, wchar_t, 4, r, 3, short, n, double, 1, 2, S, o, u, n, double, A, r, char, unsigned char, int, v, e, 9, S, o, u, n, double, I, n, float, o, const ...*, Q, 4, 4, n, wchar_t, 4, r, 3, short, n, double, 1, 2, S, o, u, n, double, A, r, char, unsigned char, int, v, e, 1, 3, W, a, v, e, S, o, u, n, double, I, n, float, o, Q, 5, 4, n, wchar_t, 4, r, 3, short, n, double, 1, 4, S, o, u, n, double, S, t, a, r, t, a, bool, long, e, 9, S, t, a, r, t, I, n, float, o, 1, 5, S, t, a, r, t, O, float, float, short, e, t, T, y, p, e, int) | 0x804221D0 |
| us-80427084 | nw4r/src/snd/snd_StrmPlayer | `__ct__Q54nw4r3snd6detail10StrmPlayer16StrmDataLoadTaskFv` | constructor() | 0x80427084 |
| us-8042c690 | nw4r/src/snd/snd_WsdPlayer | `__ct__Q44nw4r3snd6detail9WsdPlayerFv` | constructor() | 0x8042C690 |
| us-8042c6fc | nw4r/src/snd/snd_WsdPlayer | `InitParam__Q44nw4r3snd6detail9WsdPlayerFiPCQ54nw4r3snd6detail9WsdPlayer11WsdCallbackUl` | InitParam(int, const ...*, Q, 5, 4, n, wchar_t, 4, r, 3, short, n, double, 6, double, e, t, a, int, long, 9, W, short, double, ...*, long, a, y, e, r, 1, 1, W, short, double, C, a, long, long, bool, a, char, k, long) | 0x8042C6FC |
| us-8042e16c | nw4r/src/ut/ut_FileStream | `__sinit_\ut_FileStream_cpp` | __sinit_\ut_FileStream_cpp | 0x8042E16C |
| us-8042ea0c | nw4r/src/ut/ut_ResFontBase | `__ct__Q44nw4r2ut6detail11ResFontBaseFv` | constructor() | 0x8042EA0C |
| us-8043a29c | monolib/src/work/CWorkThread | `__dt__29_reslist_base<P11CWorkThread>Fv` | _reslist_base<P11CWorkThread>::~_reslist_base<P11CWorkThread>() | 0x8043A29C |
| us-8043a350 | monolib/src/work/CWorkThread | `__dt__23reslist<P11CWorkThread>Fv` | reslist<P11CWorkThread>::~reslist<P11CWorkThread>() | 0x8043A350 |
| us-8043af40 | monolib/src/work/CWorkThread | `getWorkThread__11CWorkThreadFUl` | CWorkThread::getWorkThread(unsigned long) | 0x8043AF40 |
| us-8043b6f4 | monolib/src/core/CProc | `__dt__17_reslist_base<Ul>Fv` | _reslist_base<Ul>::~_reslist_base<Ul>() | 0x8043B6F4 |
| us-8043b7a8 | monolib/src/core/CProc | `__dt__11reslist<Ul>Fv` | reslist<Ul>::~reslist<Ul>() | 0x8043B7A8 |
| us-8043b860 | monolib/src/core/CProc | `__dt__5CProcFv` | CProc::~CProc() | 0x8043B860 |
| us-8043e61c | monolib/src/core/CTaskManager | `Create__12CTaskManagerFv` | CTaskManager::Create() | 0x8043E61C |
| us-8043e90c | monolib/src/core/CTaskManager | `Reset__12CTaskManagerFv` | CTaskManager::Reset() | 0x8043E90C |
| us-8043f7b8 | monolib/src/core/CView | `setDisp__5CViewFbb` | CView::setDisp(bool, bool) | 0x8043F7B8 |
| us-8043f96c | monolib/src/core/CView | `setSplitLine__5CViewFs` | CView::setSplitLine(short) | 0x8043F96C |
| us-80441478 | monolib/src/core/CView | `wkUpdate__5CViewFv` | CView::wkUpdate() | 0x80441478 |
| us-804464a0 | monolib/src/work/CWorkRoot | `initialize__9CWorkRootFv` | CWorkRoot::initialize() | 0x804464A0 |
| us-804465c4 | monolib/src/work/CWorkRoot | `reserve__23reslist<P11CWorkThread>FUli` | reslist<P11CWorkThread>::reserve(long, int) | 0x804465C4 |
| us-80446624 | monolib/src/work/CWorkRoot | `destroy__9CWorkRootFv` | CWorkRoot::destroy() | 0x80446624 |
| us-804466e8 | monolib/src/work/CWorkRoot | `entryWork__9CWorkRootFP11CWorkThreadP11CWorkThreadb` | CWorkRoot::entryWork(...*, 1, 1, C, W, o, r, k, T, unsigned char, r, e, a, double, ...*, 1, 1, C, W, o, r, k, T, unsigned char, r, e, a, double, bool) | 0x804466E8 |
| us-80446718 | monolib/src/work/CWorkRoot | `isRunning__11CWorkThreadCFv` | CWorkThread::isRunning | 0x80446718 |
| us-80446a04 | monolib/src/work/CWorkRoot | `standbyWork__9CWorkRootFv` | CWorkRoot::standbyWork() | 0x80446A04 |
| us-80446b54 | monolib/src/work/CWorkRoot | `runSingle__9CWorkRootFv` | CWorkRoot::runSingle() | 0x80446B54 |
| us-80446c70 | monolib/src/work/CWorkRoot | `run__9CWorkRootFv` | CWorkRoot::run() | 0x80446C70 |
| us-80446d68 | monolib/src/work/CWorkRoot | `__sinit_\CWorkRoot_cpp` | __sinit_\CWorkRoot_cpp | 0x80446D68 |
| us-80448e4c | monolib/src/work/CProcess | `__sinit_\CProcess_cpp` | __sinit_\CProcess_cpp | 0x80448E4C |
| us-8044a094 | monolib/src/util/CStopwatchUtil | `updateCostTime__14CStopwatchUtilFPCc` | CStopwatchUtil::updateCostTime(const ...*, char) | 0x8044A094 |
| us-8044a194 | monolib/src/util/CStopwatchUtil | `getCostTime__14CStopwatchUtilFPCc` | CStopwatchUtil::getCostTime(const ...*, char) | 0x8044A194 |
| us-8044a7b8 | monolib/src/device/CDeviceVI | `__ct__9CDeviceVIFPCcP11CWorkThread` | CDeviceVI::CDeviceVI(const ...*, char, ...*, 1, 1, C, W, o, r, k, T, unsigned char, r, e, a, double) | 0x8044A7B8 |
| us-8044af30 | monolib/src/device/CDeviceVI | `getWidthScale__9CDeviceVIFv` | CDeviceVI::getWidthScale() | 0x8044AF30 |
| us-8044b8e8 | monolib/src/device/CDeviceVI | `setUseStaticHandle__9CDeviceVIFb` | CDeviceVI::setUseStaticHandle(bool) | 0x8044B8E8 |
| us-8044b8f0 | monolib/src/device/CDeviceVI | `usingStaticHandle__9CDeviceVIFv` | CDeviceVI::usingStaticHandle() | 0x8044B8F0 |
| us-8044b908 | monolib/src/device/CDeviceVI | `__sinit_\CDeviceVI_cpp` | __sinit_\CDeviceVI_cpp | 0x8044B908 |
| us-8044df28 | monolib/src/device/CGXCache | `bindTextureGX__8CGXCacheFP9_GXTexObjUsUs` | CGXCache::bindTextureGX(...*, 9, _, G, X, T, e, long long, O, bool, j, short, short) | 0x8044DF28 |
| us-8044e030 | monolib/src/device/CGXCache | `updateOrthoGX__8CGXCacheFv` | CGXCache::updateOrthoGX() | 0x8044E030 |
| us-8044e8a8 | monolib/src/device/CGXCache | `resetGXStateA__8CGXCacheFv` | CGXCache::resetGXStateA() | 0x8044E8A8 |
| us-8044fa60 | monolib/src/device/CDevice | `__dt__7CDeviceFv` | CDevice::~CDevice() | 0x8044FA60 |
| us-8044fac0 | monolib/src/device/CDevice | `getInstance__7CDeviceFv` | CDevice::getInstance() | 0x8044FAC0 |
| us-80450290 | monolib/src/device/CDevice | `wkStandbyLogin__7CDeviceFv` | CDevice::wkStandbyLogin() | 0x80450290 |
| us-80450398 | monolib/src/device/CDevice | `create__7CDeviceFv` | CDevice::create() | 0x80450398 |
| us-80450588 | monolib/src/device/CDevice | `__sinit_\CDevice_cpp` | __sinit_\CDevice_cpp | 0x80450588 |
| us-80450718 | monolib/src/device/CDeviceClock | `__dt__35_reslist_base<P17IDeviceClockFrame>Fv` | _reslist_base<P17IDeviceClockFrame>::~_reslist_base<P17IDeviceClockFrame>() | 0x80450718 |
| us-804507cc | monolib/src/device/CDeviceClock | `__dt__29reslist<P17IDeviceClockFrame>Fv` | reslist<P17IDeviceClockFrame>::~reslist<P17IDeviceClockFrame>() | 0x804507CC |
| us-80450884 | monolib/src/device/CDeviceClock | `__dt__12CDeviceClockFv` | CDeviceClock::~CDeviceClock() | 0x80450884 |
| us-804523e4 | monolib/src/device/CDeviceFileCri | `checkDriveStatus__14CDeviceFileCriFv` | CDeviceFileCri::checkDriveStatus() | 0x804523E4 |
| us-80452610 | monolib/src/device/CDeviceFileCri | `getInstance__14CDeviceFileCriFv` | CDeviceFileCri::getInstance() | 0x80452610 |
| us-80453858 | monolib/src/device/CDeviceFileCri | `FileCriCheckDriveStatusThunk` | FileCriCheckDriveStatusThunk | 0x80453858 |
| us-80458070 | monolib/src/device/CDeviceFontLayer | `getScaledTextHeight__16CDeviceFontLayerFv` | CDeviceFontLayer::getScaledTextHeight() | 0x80458070 |
| us-80459070 | monolib/src/device/CDeviceFontLoader | `wkStandbyLogin__17CDeviceFontLoaderFv` | CDeviceFontLoader::wkStandbyLogin() | 0x80459070 |
| us-804592e8 | monolib/src/device/CDeviceGX | `__dt__9CDeviceGXFv` | CDeviceGX::~CDeviceGX() | 0x804592E8 |
| us-80459588 | monolib/src/device/CDeviceGX | `viAfterDrawDone__9CDeviceGXFv` | CDeviceGX::viAfterDrawDone() | 0x80459588 |
| us-8045974c | monolib/src/device/CDeviceGX | `copyEfb__9CDeviceGXFPv` | CDeviceGX::copyEfb(...*, v) | 0x8045974C |
| us-80459a1c | monolib/src/device/CDeviceGX | `drawSyncCallback__9CDeviceGXFUs` | CDeviceGX::drawSyncCallback(unsigned short) | 0x80459A1C |
| us-8045d590 | monolib/src/lib/CLib | `wkStandbyLogin__4CLibFv` | CLib::wkStandbyLogin() | 0x8045D590 |
| us-8045d704 | monolib/src/lib/CLib | `wkStandbyLogout__4CLibFv` | CLib::wkStandbyLogout() | 0x8045D704 |
| us-8045d7a0 | monolib/src/lib/CLib | `create__4CLibFv` | CLib::create() | 0x8045D7A0 |
| us-8045da94 | monolib/src/lib/CLibCri | `setStreamPause__7CLibCriFv` | CLibCri::setStreamPause() | 0x8045DA94 |
| us-8045da98 | monolib/src/lib/CLibCri | `fadeStreamVolume__7CLibCriFv` | CLibCri::fadeStreamVolume() | 0x8045DA98 |
| us-8045daa0 | monolib/src/lib/CLibCri | `setStreamVolume__7CLibCriFv` | CLibCri::setStreamVolume() | 0x8045DAA0 |
| us-8045daa4 | monolib/src/lib/CLibCri | `setStreamVolumeScale__7CLibCriFv` | CLibCri::setStreamVolumeScale() | 0x8045DAA4 |
| us-8045daac | monolib/src/lib/CLibCri | `setStreamPanVolume__7CLibCriFv` | CLibCri::setStreamPanVolume() | 0x8045DAAC |
| us-8045dab0 | monolib/src/lib/CLibCri | `calcStreamBufferSize__7CLibCriFv` | CLibCri::calcStreamBufferSize() | 0x8045DAB0 |
| us-8045dab4 | monolib/src/lib/CLibCri | `isStreamPaused__7CLibCriFv` | CLibCri::isStreamPaused() | 0x8045DAB4 |
| us-8045dab8 | monolib/src/lib/CLibCri | `startMovie__7CLibCriFv` | CLibCri::startMovie() | 0x8045DAB8 |
| us-8045dabc | monolib/src/lib/CLibCri | `stopMovie__7CLibCriFv` | CLibCri::stopMovie() | 0x8045DABC |
| us-8045dac0 | monolib/src/lib/CLibCri | `setMoviePause__7CLibCriFv` | CLibCri::setMoviePause() | 0x8045DAC0 |
| us-8045dad8 | monolib/src/lib/CLibCri | `isMoviePlaying__7CLibCriFv` | CLibCri::isMoviePlaying() | 0x8045DAD8 |
| us-80461398 | monolib/src/lib/CLibHbm | `__ct__7CLibHbmFPCcP11CWorkThread` | CLibHbm::CLibHbm(const ...*, char, ...*, 1, 1, C, W, o, r, k, T, unsigned char, r, e, a, double) | 0x80461398 |
| us-804614c4 | monolib/src/lib/CLibHbm | `loadTplImage__7CLibHbmFPv` | CLibHbm::loadTplImage(...*, v) | 0x804614C4 |
| us-80461718 | monolib/src/lib/CLibHbm | `loadHbmArcFile__7CLibHbmFv` | CLibHbm::loadHbmArcFile() | 0x80461718 |
| us-80461bdc | monolib/src/lib/CLibHbm | `initHbm__7CLibHbmFv` | CLibHbm::initHbm() | 0x80461BDC |
| us-80462124 | monolib/src/lib/CLibHbm | `renderHbmstopIcon__7CLibHbmFv` | CLibHbm::renderHbmstopIcon() | 0x80462124 |
| us-80463900 | monolib/src/lib/CLibStaticData | `__ct__14CLibStaticDataFPCcP11CWorkThread` | CLibStaticData::CLibStaticData(const ...*, char, ...*, 1, 1, C, W, o, r, k, T, unsigned char, r, e, a, double) | 0x80463900 |
| us-80463e78 | monolib/src/lib/CLibStaticData | `__ct__Q214CLibStaticData5CItemFP17StaticArcFileData` | constructor(int, long, e, D, a, t, a) | 0x80463E78 |
| us-80463f6c | monolib/src/lib/CLibStaticData | `__dt__Q214CLibStaticData5CItemFv` | destructor() | 0x80463F6C |
| us-80466874 | monolib/src/lod/CTaskLOD | `Term__8CTaskLODFv` | CTaskLOD::Term() | 0x80466874 |
| us-804668cc | monolib/src/lod/CTaskLOD | `Move__8CTaskLODFv` | CTaskLOD::Move() | 0x804668CC |
| us-80466bb4 | monolib/src/lod/CTaskLOD | `notifyLODTick__8CTaskLODFv` | CTaskLOD::notifyLODTick() | 0x80466BB4 |
| us-80466bcc | monolib/src/lod/CTaskLOD | `updateLODFrame__8CTaskLODFv` | CTaskLOD::updateLODFrame() | 0x80466BCC |
| us-80466cd4 | monolib/src/lod/CTaskLOD | `activateLOD__8CTaskLODFv` | CTaskLOD::activateLOD() | 0x80466CD4 |
| us-80466d2c | monolib/src/lod/CTaskLOD | `deactivateLOD__8CTaskLODFv` | CTaskLOD::deactivateLOD() | 0x80466D2C |
| us-80466d84 | monolib/src/lod/CTaskLOD | `setLODEnable__8CTaskLODFv` | CTaskLOD::setLODEnable() | 0x80466D84 |
| us-80466e28 | monolib/src/lod/CTaskLOD | `setLODParam__8CTaskLODFv` | CTaskLOD::setLODParam() | 0x80466E28 |
| us-80466efc | monolib/src/lod/CTaskLOD | `getLODDistance__8CTaskLODFv` | CTaskLOD::getLODDistance() | 0x80466EFC |
| us-80466fc4 | monolib/src/lod/CTaskLOD | `getLODLevel__8CTaskLODFv` | CTaskLOD::getLODLevel() | 0x80466FC4 |
| us-8046bcc0 | monolib/src/lod/UnkClass_80466348 | `setAmbColorWithAlpha1__Q23LOD17UnkClass_80466348Fv` | LOD::UnkClass_80466348::setAmbColorWithAlpha1() | 0x8046BCC0 |
| us-8046bde4 | monolib/src/lod/UnkClass_80466348 | `setAmbColorWithAlpha0__Q23LOD17UnkClass_80466348Fv` | LOD::UnkClass_80466348::setAmbColorWithAlpha0() | 0x8046BDE4 |
| us-8046be64 | monolib/src/lod/UnkClass_80466348 | `setAmbColorWithAlpha2__Q23LOD17UnkClass_80466348Fv` | LOD::UnkClass_80466348::setAmbColorWithAlpha2() | 0x8046BE64 |
| us-80485e3c | monolib/src/scn/CScnItemCameraNw4r | `__ct__CScnItemCameraNw4r` | constructor | 0x80485E3C |
| us-80486410 | monolib/src/scn/CScnItemLightNw4r | `__ct__CScnItemLightNw4r` | constructor | 0x80486410 |
| us-8048707c | monolib/src/scn/CScnItemModel | `simRemoveFromPool` | simRemoveFromPool | 0x8048707C |
| us-80488280 | monolib/src/scn/CScnItemModel | `simQueryLeafAnim` | simQueryLeafAnim | 0x80488280 |
| us-804886fc | monolib/src/scn/CScnItemModel | `simResetAnimAtC` | simResetAnimAtC | 0x804886FC |
| us-80488ee8 | monolib/src/scn/CScnItemModel | `simGetLeafAnimDist` | simGetLeafAnimDist | 0x80488EE8 |
| us-80488f50 | monolib/src/scn/CScnItemModel | `simRefreshFlag8` | simRefreshFlag8 | 0x80488F50 |
| us-80489074 | monolib/src/scn/CScnItemModel | `simGetLeafAnimDist2` | simGetLeafAnimDist2 | 0x80489074 |
| us-804890dc | monolib/src/scn/CScnItemModel | `simGetLeafAnimDist3` | simGetLeafAnimDist3 | 0x804890DC |
| us-80489144 | monolib/src/scn/CScnItemModel | `simGetLeafAnimDist4` | simGetLeafAnimDist4 | 0x80489144 |
| us-804891ac | monolib/src/scn/CScnItemModel | `simGetLeafAnimStatus` | simGetLeafAnimStatus | 0x804891AC |
| us-80489214 | monolib/src/scn/CScnItemModel | `simRefreshFadeDist` | simRefreshFadeDist | 0x80489214 |
| us-80489454 | monolib/src/scn/CScnItemModel | `simRefreshFadeDist2` | simRefreshFadeDist2 | 0x80489454 |
| us-80489784 | monolib/src/scn/CScnItemModel | `simSetLeafAnimTag` | simSetLeafAnimTag | 0x80489784 |
| us-80489cf8 | monolib/src/scn/CScnItemModel | `simRefreshActDist` | simRefreshActDist | 0x80489CF8 |
| us-80489d74 | monolib/src/scn/CScnItemModel | `simRefreshFadeChain` | simRefreshFadeChain | 0x80489D74 |
| us-8048b720 | monolib/src/scn/CScnItemModelNw4r | `scnImN4DynStart` | scnImN4DynStart | 0x8048B720 |
| us-8048b7b0 | monolib/src/scn/CScnItemModelNw4r | `scnImN4UnlinkChain` | scnImN4UnlinkChain | 0x8048B7B0 |
| us-8048b85c | monolib/src/scn/CScnItemModelNw4r | `scnImN4Teardown` | scnImN4Teardown | 0x8048B85C |
| us-8048be84 | monolib/src/scn/CScnItemModelNw4r | `scnImN4MouthFnA` | scnImN4MouthFnA | 0x8048BE84 |
| us-8048be8c | monolib/src/scn/CScnItemModelNw4r | `scnImN4MouthFnB` | scnImN4MouthFnB | 0x8048BE8C |
| us-8048be9c | monolib/src/scn/CScnItemModelNw4r | `scnImN4FwdEyeAnim` | scnImN4FwdEyeAnim | 0x8048BE9C |
| us-8048c9c8 | monolib/src/scn/CScnItemModelNw4r | `scnImN4BuildNodeA` | scnImN4BuildNodeA | 0x8048C9C8 |
| us-8048caf0 | monolib/src/scn/CScnItemModelNw4r | `scnImN4BuildNodeB` | scnImN4BuildNodeB | 0x8048CAF0 |
| us-8048d044 | monolib/src/scn/CScnItemModelNw4r | `scnImN4LookRel` | scnImN4LookRel | 0x8048D044 |
| us-8048daa4 | monolib/src/scn/CScnItemModelNw4r | `scnImN4BuildByIdx` | scnImN4BuildByIdx | 0x8048DAA4 |
| us-8048f700 | monolib/src/scn/CScnItemModelNw4r | `scnImN4MtxByName` | scnImN4MtxByName | 0x8048F700 |
| us-8048fb48 | monolib/src/scn/CScnItemModelNw4r | `scnImN4AnimFn` | scnImN4AnimFn | 0x8048FB48 |
| us-8048fff4 | monolib/src/scn/CScnItemPool | `__dt__25_reslist_base<P8CScnItem>Fv` | _reslist_base<P8CScnItem>::~_reslist_base<P8CScnItem>() | 0x8048FFF4 |
| us-804900a8 | monolib/src/scn/CScnItemPool | `__dt__19reslist<P8CScnItem>Fv` | reslist<P8CScnItem>::~reslist<P8CScnItem>() | 0x804900A8 |
| us-80492bec | monolib/src/scn/CScnMem | `__ct__CScnNw4r` | constructor | 0x80492BEC |
| us-80492db4 | monolib/src/scn/CScnRoot | `__dt__8CScnRootFv` | CScnRoot::~CScnRoot() | 0x80492DB4 |
| us-80492df4 | monolib/src/scn/CScnRoot | `createScnRootNw4r` | createScnRootNw4r | 0x80492DF4 |
| us-8049427c | monolib/src/scn/CScnTexWorkMan | `TexMan_AllocBuffer_0208` | TexMan_AllocBuffer_0208 | 0x8049427C |
| us-80497374 | monolib/src/scn/CScnVirtualLight | `scnVlCreateDir` | scnVlCreateDir | 0x80497374 |
| us-80497bec | monolib/src/scn/CScnVirtualLight | `scnVlGetCamId` | scnVlGetCamId | 0x80497BEC |
| us-80499778 | monolib/src/scn/CVirtualLightObj | `bindOrientedLight` | bindOrientedLight | 0x80499778 |
| us-80499858 | monolib/src/scn/CVirtualLightObj | `bindAttenuatedLight` | bindAttenuatedLight | 0x80499858 |
| us-8049992c | monolib/src/scn/CVirtualLightObj | `bindSpotLight` | bindSpotLight | 0x8049992C |
| us-8049a0ac | monolib/src/scn/CScn | `Scn_GetUnk80Handle` | Scn_GetUnk80Handle | 0x8049A0AC |
| us-8049a0b4 | monolib/src/scn/CScn | `Scn_QueryUnk80State` | Scn_QueryUnk80State | 0x8049A0B4 |
| us-8049a0bc | monolib/src/scn/CScn | `Scn_IsDefaultScale` | Scn_IsDefaultScale | 0x8049A0BC |
| us-8049a120 | monolib/src/scn/CScn | `Scn_IsScaleAtLeastOne` | Scn_IsScaleAtLeastOne | 0x8049A120 |
| us-8049a188 | monolib/src/scn/CScn | `Scn_GetUnk80Resource` | Scn_GetUnk80Resource | 0x8049A188 |
| us-8049a190 | monolib/src/scn/CScn | `Scn_GetCamWorkInt` | Scn_GetCamWorkInt | 0x8049A190 |
| us-8049a198 | monolib/src/scn/CScn | `Scn_CopyCamProjMatrix` | Scn_CopyCamProjMatrix | 0x8049A198 |
| us-8049a24c | monolib/src/scn/CScn | `Scn_GetCamItem9C` | Scn_GetCamItem9C | 0x8049A24C |
| us-8049a294 | monolib/src/scn/CScn | `Scn_GetCamItemCC` | Scn_GetCamItemCC | 0x8049A294 |
| us-8049a2dc | monolib/src/scn/CScn | `Scn_FindCamItem` | Scn_FindCamItem | 0x8049A2DC |
| us-8049aa70 | monolib/src/scn/CScn | `execScriptCode__Fv` | execScriptCode() | 0x8049AA70 |
| us-8049aa74 | monolib/src/scn/CScn | `resetScriptCode__Fv` | resetScriptCode() | 0x8049AA74 |
| us-8049b0a8 | monolib/src/scn/CScn_80496B0C | `scn80496FindChrAnm` | scn80496FindChrAnm | 0x8049B0A8 |
| us-804a0090 | monolib/src/scn/CScnEffectActNw4r | `EffectActNw4rFindActGated` | EffectActNw4rFindActGated | 0x804A0090 |
| us-804a0144 | monolib/src/scn/CScnEffectActNw4r | `EffectActNw4rGetActPosOut` | EffectActNw4rGetActPosOut | 0x804A0144 |
| us-804a0618 | monolib/src/scn/CScnFadeMan | `update__11CScnFadeManFv` | CScnFadeMan::update() | 0x804A0618 |
| us-804a0b60 | monolib/src/scn/CScnFilterMan | `__ct__reslist_CScnFilter` | constructor(int, long, t, e, r) | 0x804A0B60 |
| us-804a2a20 | monolib/src/scn/CScnItemCamera | `__ct__CScnItemCamera` | constructor | 0x804A2A20 |
| us-804b4a80 | monolib/src/coli/code_804A6C60 | `ColiNodeInit` | ColiNodeInit | 0x804B4A80 |
| us-804b4e44 | monolib/src/coli/code_804A6C60 | `ColiNodeRefreshAxes` | ColiNodeRefreshAxes | 0x804B4E44 |
| us-804b5188 | monolib/src/coli/code_804A6C60 | `ColiNodeRebuildClassify` | ColiNodeRebuildClassify | 0x804B5188 |
| us-804b5b28 | monolib/src/coli/code_804A6C60 | `ColiQuerySpecDispatch` | ColiQuerySpecDispatch | 0x804B5B28 |
| us-804b5df8 | monolib/src/coli/code_804A6C60 | `ColiCheckMoveScaled` | ColiCheckMoveScaled | 0x804B5DF8 |
| us-804b61a8 | monolib/src/coli/code_804A6C60 | `ColiProcessSegmentPair` | ColiProcessSegmentPair | 0x804B61A8 |
| us-804b6304 | monolib/src/coli/code_804A6C60 | `ColiProcessSegmentSelect` | ColiProcessSegmentSelect | 0x804B6304 |
| us-804b64c8 | monolib/src/coli/code_804A6C60 | `ColiQueryNodePoint` | ColiQueryNodePoint | 0x804B64C8 |
| us-804c3510 | monolib/src/scn/code_804BD8E8 | `ScnRes_InstallCrossCb_F3B4` | ScnRes_InstallCrossCb_F3B4 | 0x804C3510 |
| us-804c4d40 | monolib/src/scn/CScnEnvLgtCtrl | `__ct__reslist_IScnEnvCtl` | constructor | 0x804C4D40 |
| us-804c51f0 | monolib/src/scn/CScnEnvLgtCtrl | `scnLgtSetBlendBloom` | scnLgtSetBlendBloom | 0x804C51F0 |
| us-804c5494 | monolib/src/scn/CScnEnvLgtCtrl | `scnLgtDispatchEnv` | scnLgtDispatchEnv | 0x804C5494 |
| us-804c565c | monolib/src/scn/CScnEnvLgtCtrl | `scnLgtGatherLights` | scnLgtGatherLights | 0x804C565C |
| us-804c57d0 | monolib/src/scn/CScnEnvLgtCtrl | `scnLgtApplyFogByte` | scnLgtApplyFogByte | 0x804C57D0 |
| us-804c587c | monolib/src/scn/CScnEnvLgtCtrl | `scnLgtDeactLightMan` | scnLgtDeactLightMan | 0x804C587C |
| us-804c5a68 | monolib/src/scn/CScnEnvLgtCtrl | `scnLgtEnterMode10` | scnLgtEnterMode10 | 0x804C5A68 |
| us-804c5b14 | monolib/src/scn/CScnEnvLgtCtrl | `scnLgtEnterMode20` | scnLgtEnterMode20 | 0x804C5B14 |
| us-804c5bc0 | monolib/src/scn/CScnEnvLgtCtrl | `scnLgtPushFogNone` | scnLgtPushFogNone | 0x804C5BC0 |
| us-804c5c58 | monolib/src/scn/CScnEnvLgtCtrl | `scnLgtScaleLgtRing` | scnLgtScaleLgtRing | 0x804C5C58 |
| us-804c67b0 | monolib/src/scn/CScnEnvLgtCtrl | `scnLgtBindCamera` | scnLgtBindCamera | 0x804C67B0 |
| us-804cc874 | monolib/src/effect/code_804C8718 | `EffSys_InitRoot` | EffSys_InitRoot | 0x804CC874 |
| us-804cebf0 | monolib/src/effect/code_804C8718 | `EffObj_StepNode` | EffObj_StepNode | 0x804CEBF0 |
| us-804cf540 | monolib/src/effect/code_804C8718 | `EffRender_PassBase` | EffRender_PassBase | 0x804CF540 |
| us-804cf5b4 | monolib/src/effect/code_804C8718 | `EffRender_PassNoHide` | EffRender_PassNoHide | 0x804CF5B4 |
| us-804cf650 | monolib/src/effect/code_804C8718 | `EffRender_PassFlag5` | EffRender_PassFlag5 | 0x804CF650 |
| us-804cf6bc | monolib/src/effect/code_804C8718 | `EffRender_PassHideSet` | EffRender_PassHideSet | 0x804CF6BC |
| us-804cfc04 | monolib/src/effect/code_804C8718 | `EffSys_InitScene` | EffSys_InitScene | 0x804CFC04 |
| us-804d0044 | monolib/src/effect/code_804C8718 | `EffSys_Thunk8688` | EffSys_Thunk8688 | 0x804D0044 |
| us-804d02b0 | monolib/src/effect/code_804C8718 | `EffSys_Teardown` | EffSys_Teardown | 0x804D02B0 |
| us-804dcddc | monolib/src/effect/code_804D854C | `blitEffectViewRect` | blitEffectViewRect | 0x804DCDDC |
| us-804dedf4 | monolib/src/nand/CNReqtaskSave | `CNReqSaveInitCheck` | CNReqSaveInitCheck | 0x804DEDF4 |
| us-804deea8 | monolib/src/nand/CNReqtaskSave | `CNReqSaveInitRemove` | CNReqSaveInitRemove | 0x804DEEA8 |
| us-804deee4 | monolib/src/nand/CNReqtaskSave | `CNReqSaveInitReaddir` | CNReqSaveInitReaddir | 0x804DEEE4 |
| us-804def5c | monolib/src/nand/CNReqtaskSave | `CNReqSaveInitSaveBanner` | CNReqSaveInitSaveBanner | 0x804DEF5C |
| us-804def98 | monolib/src/nand/CNReqtaskSave | `CNReqSaveConfigSave` | CNReqSaveConfigSave | 0x804DEF98 |
| us-804dfaf8 | monolib/src/effect/CERand | `randSign__12CERandomizerFv` | CERandomizer::randSign() | 0x804DFAF8 |
| us-804dfb38 | monolib/src/effect/CERand | `randSign__18CERandomizerSimpleFv` | CERandomizerSimple::randSign() | 0x804DFB38 |
| us-804dfb78 | monolib/src/effect/CERand | `randFHalf__18CERandomizerSimpleFv` | CERandomizerSimple::randFHalf() | 0x804DFB78 |
| us-804dfbcc | monolib/src/effect/CERand | `randF__18CERandomizerSimpleFv` | CERandomizerSimple::randF() | 0x804DFBCC |
| us-804dfc50 | monolib/src/effect/CERand | `randF__12CERandomizerFv` | CERandomizer::randF() | 0x804DFC50 |
| us-804e13b8 | monolib/src/effect/code_804DB938 | `CERot_AxisToMatrix` | CERot_AxisToMatrix | 0x804E13B8 |
| us-804e28b0 | monolib/src/core/CPackItem | `__dt__9CPackItemFv` | CPackItem::~CPackItem() | 0x804E28B0 |
| us-804e2c28 | monolib/src/core/CPackItem | `lookupFile__9CPackItemFPCcPPcPUlPUlPUl` | CPackItem::lookupFile(const ...*, char, ...*, ...*, char, ...*, long, ...*, long, ...*, long) | 0x804E2C28 |
| us-804e2fb0 | monolib/src/core/CArcItem | `__ct__8CArcItemFPCc` | CArcItem::CArcItem(const ...*, char) | 0x804E2FB0 |
| us-804e4024 | monolib/src/core/code_804DEDA8 | `SchedItem_ReleaseHandle` | SchedItem_ReleaseHandle | 0x804E4024 |
| us-804e8390 | monolib/src/core/code_804E36DC | `schedTickItemLifetime` | schedTickItemLifetime | 0x804E8390 |
| us-804e8bb4 | monolib/src/core/ScheduleList | `SchedList_SweepAll` | SchedList_SweepAll | 0x804E8BB4 |
| us-804eac38 | monolib/src/scn/CMdlMouth | `MdlMouthSetMode` | MdlMouthSetMode | 0x804EAC38 |
| us-804ead3c | monolib/src/scn/CMdlMouth | `MdlMouthPollAnim` | MdlMouthPollAnim | 0x804EAD3C |
| us-804eaec4 | monolib/src/scn/CMdlMouth | `MdlMouthTeardown` | MdlMouthTeardown | 0x804EAEC4 |
| us-804eb0b0 | monolib/src/scn/CMdlAnmUV | `__ct__CMdlAnmUV` | constructor | 0x804EB0B0 |
| us-804f5488 | monolib/src/core/code_804F0258 | `func_804F0F2C` | func_804F0F2C | 0x804F5488 |
| us-804f92f0 | monolib/src/nand/CNReqtaskSaveBanner | `CNReqtaskSaveBanner_Step` | CNReqtaskSaveBanner_Step | 0x804F92F0 |

