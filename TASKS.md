# Decompilation task checklist

Agent-facing checklist derived from [`DECOMP_MAP.md`](DECOMP_MAP.md). Check off functions only after they reach **`FULL_MATCH`** via `python tools/coop/run.py cycle <target-id>` (or equivalent `diff` + evidence).

## How to use

1. Pick an unchecked task in priority order (**P0 → P1 → P2 → P3**).
2. Read the matching section in `DECOMP_MAP.md` and claim the symbol in `docs/ownership.csv` (see `PLAN.md` §6).
3. Decompile using the loop in [`.cursor/skills/xenoblade-decomp/SKILL.md`](.cursor/skills/xenoblade-decomp/SKILL.md).
4. When `cycle` passes at **FULL_MATCH**, change `- [ ]` to `- [x]` here and log evidence in `docs/evidence/decomp/attempts.jsonl`.
5. If the translation unit is newly matched, flip `NonMatching` → `Matching` in `configure.py` for that object.

**Policy:** every function below must reach `FULL_MATCH` before it is considered done. The *Map level* column in `DECOMP_MAP.md` is planning guidance only.

## Progress summary

- **Total tracked functions:** 106
- **Completed:** 5
- **Remaining:** 101

- **P0:** 5 / 41
- **P1:** 0 / 55
- **P2:** 0 / 5
- **P3:** 0 / 5

---

## P0 — Required to prove split-screen rendering.

### 5.1 src/kyoshin/CGame.cpp

- [x] **`CGame::wkUpdate()`** (`wkUpdate__5CGameFv`) · `0x800397D8` · size `0x104`
  - Map level: FULL_MATCH
  - Proves the authoritative update boundary and must run once.
- [x] **`CGame::wkRender()`** (`wkRender__5CGameFv`) · `0x800398DC` · size `0xA4` · target `game-wk-render`
  - Map level: FULL_MATCH
  - Identifies layout animation, draw dispatch, and the unsafe top-level duplicate-render path.
- [x] **`CGame::setViewRect(CView*,...)`** (`setViewRect__5CGameFP5CViewssss`) · `0x80039A30` · size `0x34`
  - Map level: FULL_MATCH
  - Known viewport rectangle helper; useful for initial split proof.
- [ ] **`CGame::wkStandbyLogin()`** (`wkStandbyLogin__5CGameFv`) · `0x80039A64` · size `0x1D8`
  - Map level: STRUCTURAL
  - Shows creation/attachment of the primary view and top-level tasks.
- [ ] **`CGame::wkStandbyLogout()`** (`wkStandbyLogout__5CGameFv`) · `0x80039C3C` · size `0xEC`
  - Map level: STRUCTURAL
  - Required for safe teardown and runtime co-op toggles.
- [x] **`CGame::OnPauseTrigger(bool)`** (`OnPauseTrigger__5CGameFb`) · `0x80039FB4` · size `0x100`
  - Map level: CODE_MATCH
  - Useful for modal menu/pause gating and input suppression.
- [ ] **`CGame::GameMain()`** (`GameMain__5CGameFv`) · `0x80039DDC` · size `0x94`
  - Map level: STRUCTURAL
  - Maps frame ordering around update, render, and device presentation.

### 5.2 Task/process traversal

- [ ] **`CTaskManager::Move()`** (`Move__12CTaskManagerFv`) · `0x8043E8E4` · size `0x24`
  - Map level: CODE_MATCH
  - Confirms update traversal entry.
- [ ] **`CTaskManager::Draw()`** (`Draw__12CTaskManagerFv`) · `0x8043E908` · size `0x4`
  - Map level: CODE_MATCH
  - Confirms draw traversal entry and whether it is only a thunk.
- [ ] **`CProcessMan::Draw()`** (`Draw__11CProcessManFv`) · `0x80447E70` · size `0x498`
  - Map level: FULL_MATCH
  - Shows draw plus tail ordering and global traversal side effects.
- [ ] **`CProcessMan::DrawImpl(CProcess*)`** (`DrawImpl__11CProcessManFP8CProcess`) · `0x80448308` · size `0x1F8`
  - Map level: FULL_MATCH
  - Classifies per-process Draw callbacks and view filtering.
- [ ] **`CProcessMan::TailImpl(CProcess*)`** (`TailImpl__11CProcessManFP8CProcess`) · `0x80448500` · size `0x1F8`
  - Map level: FULL_MATCH
  - Identifies finalization that must not run per viewport.
- [ ] **`CProcessMan::Move()`** (`Move__11CProcessManFv`) · `0x804478F0` · size `0x2B8`
  - Map level: CODE_MATCH
  - Confirms simulation traversal and process ordering.
- [ ] **`CProc::pssCreateView(...)`** (`pssCreateView__5CProcFPCcP11CWorkThreadi`) · `0x8043BC8C` · size `0x3AC`
  - Map level: FULL_MATCH
  - May create a second native view attached to the game process.
- [ ] **`CProc::pssDetachView(viewId)`** (`pssDetachView__5CProcFUl`) · `0x8043BBF0` · size `0x9C`
  - Map level: FULL_MATCH
  - Required for safe teardown and runtime toggle.
- [ ] **`CProc::pssSetFocus()`** (`pssSetFocus__5CProcFv`) · `0x8043BB40` · size `0xB0`
  - Map level: CODE_MATCH
  - Determines input/focus side effects of multiple views.

### 5.3 View system

- [ ] **`CView::CView(...)`** (`__ct__5CViewFPCcP11CWorkThread`) · `0x8043EC5C` · size `0x2D8`
  - Map level: STRUCTURAL
  - Recovers layout, owned frame objects, and initialization invariants.
- [x] **`CView::setCurrent()`** (`setCurrent__5CViewFv`) · `0x8043F3D8` · size `0xBC` · target `view-set-current`
  - Map level: FULL_MATCH
  - Switches global/current view state before each render pass.
- [x] **`CView::setRect(CRect16 const&)`** (`setRect__5CViewFRCQ22ml7CRect16`) · `0x8043F514` · size `0x150` · target `view-set-rect`
  - Map level: FULL_MATCH
  - Sets viewport rectangle and likely projection/scissor state.
- [ ] **`CView::setDisp(bool,bool)`** (`setDisp__5CViewFbb`) · `0x8043F7B8` · size `0x70`
  - Map level: FULL_MATCH
  - Controls visibility/display participation.
- [ ] **`CView::getSplitLine()`** (`getSplitLine__5CViewFv`) · `0x8043F8D8` · size `0x94`
  - Map level: FULL_MATCH
  - Existing split-related field may reveal native layout support.
- [ ] **`CView::setSplitLine(short)`** (`setSplitLine__5CViewFs`) · `0x8043F96C` · size `0x9C`
  - Map level: FULL_MATCH
  - Potentially configures a native split boundary. Must be understood before inventing a new mechanism.
- [ ] **`CView::updateMsg()`** (`updateMsg__5CViewFv`) · `0x8043FA08` · size `0x798`
  - Map level: STRUCTURAL
  - Classify per-frame view messages and side effects.
- [ ] **`CView::attachRenderWork(CWorkThread*)`** (`attachRenderWork__5CViewFP11CWorkThread`) · `0x804401A0` · size `0x1E0`
  - Map level: FULL_MATCH
  - Shows how render jobs are associated with a view.
- [x] **`CView::detachRenderWork(CWorkThread*)`** (`detachRenderWork__5CViewFP11CWorkThread`) · `0x80441470` · size `0x8`
  - Map level: FULL_MATCH
  - Needed for safe destruction/toggle.
- [ ] **`CView::wkUpdate()`** (`wkUpdate__5CViewFv`) · `0x80441478` · size `0x14C`
  - Map level: STRUCTURAL
  - Determine whether view update is camera-only, presentation-only, or stateful.
- [ ] **`CView::renderView()`** (`renderView__5CViewFv`) · `0x804415C4` · size `0xCB4`
  - Map level: STRUCTURAL
  - Primary candidate for a repeatable per-view render pass.
- [ ] **`CViewFrame::render()`** (`render__10CViewFrameFv`) · `0x80442CDC` · size `0x394`
  - Map level: STRUCTURAL
  - Frame/border/clear behavior around a viewport.
- [ ] **`CViewRoot::setCurrent(CView*)`** (`setCurrent__9CViewRootFP5CView`) · `0x80444C90` · size `0x1F4`
  - Map level: FULL_MATCH
  - Global current-view management and nested-view behavior.
- [ ] **`CViewRoot::getFullScreenView()`** (`getFullScreenView__9CViewRootFv`) · `0x80445314` · size `0x1D8`
  - Map level: FULL_MATCH
  - Restores original full-screen presentation for menus/cutscenes.
- [ ] **`CViewRoot::getView(unsigned long)`** (`getView__9CViewRootFUl`) · `0x80445810` · size `0xA0`
  - Map level: FULL_MATCH
  - Resolves view IDs and supports second-view ownership.
- [ ] **`CViewRoot::renderView()`** (`renderView__9CViewRootFv`) · `0x80445A5C` · size `0x150`
  - Map level: STRUCTURAL
  - Maps root-level view traversal and finalization.
- [ ] **`CViewRoot::create(...)`** (`create__9CViewRootFP11CWorkThread`) · `0x80445E94` · size `0x21C`
  - Map level: STRUCTURAL
  - Determines root view lifetime and allocation source.

### 5.4 Scene dispatch and frame presentation

- [ ] **`CScn::addRenderCB(...)`** (`addRenderCB__4CScnFP10IScnRenderUlUl`) · `0x80499F2C` · size `0x9C`
  - Map level: FULL_MATCH
  - Maps scene render phases and callback ordering.
- [ ] **`CScn::removeRenderCB(...)`** (`removeRenderCB__4CScnFP10IScnRender`) · `0x80499FC8` · size `0x78`
  - Map level: FULL_MATCH
  - Required to add/remove split callbacks safely.
- [ ] **`CScn::Draw()`** (`Draw__4CScnFv`) · `0x8049A918` · size `0x120`
  - Map level: STRUCTURAL
  - Candidate scene-only boundary or dispatcher.
- [ ] **`CDeviceGX::viBeginFrame()`** (`viBeginFrame__9CDeviceGXFv`) · `0x80459634` · size `0x1C` · target `device-vi-begin-frame`
  - Map level: FULL_MATCH
  - Must execute once per output frame unless proven otherwise.
- [ ] **`CDeviceGX::drawFrame()`** (`drawFrame__9CDeviceGXFv`) · `0x80459650` · size `0xFC`
  - Map level: STRUCTURAL
  - Maps EFB/XFB rendering and final frame flow.
- [ ] **`CDeviceGX::copyEfb(void*)`** (`copyEfb__9CDeviceGXFPv`) · `0x8045974C` · size `0x140`
  - Map level: FULL_MATCH
  - Must generally execute once after both halves are drawn.
- [ ] **`CDeviceGX::viAfterDrawDone()`** (`viAfterDrawDone__9CDeviceGXFv`) · `0x80459588` · size `0xAC`
  - Map level: FULL_MATCH
  - Finalization/synchronization path; run once.
- [ ] **`CDeviceGX::onRenderWork()`** (`onRenderWork__9CDeviceGXFv`) · `0x8045988C` · size `0x4`
  - Map level: CODE_MATCH
  - Small hook point that may delimit device render work.

---

## P1 — Required for playable two-player combat.

### 6.1 src/kyoshin/cf/CfPadTask.cpp

- [ ] **`CfPadTask::copyInputFlag(...)`** (`copyInputFlag__Q22cf9CfPadTaskFP4CPadUlUl`) · `0x801C3494` · size `0xC4` · target `pad-copy-input-flag`
  - Map level: FULL_MATCH
  - Maps held/pressed/repeat flags into game input.
- [ ] **`CfPadTask::updateCfPadData(...)`** (`updateCfPadData__Q22cf9CfPadTaskFPQ22cf9CfPadDataPC4CPad`) · `0x801C35C8` · size `0x364` · target `pad-update-cf-pad-data`
  - Map level: FULL_MATCH
  - Converts one CPad into one CfPadData; should become reusable for both players.
- [ ] **`CfPadTask::Move()`** (`Move__Q22cf9CfPadTaskFv`) · `0x801C392C` · size `0x194` · target `pad-move`
  - Map level: FULL_MATCH
  - Shows task timing and global snapshot publication.
- [ ] **`CfPadTask::update()`** (`update__Q22cf9CfPadTaskFv`) · `0x801C3AC0` · size `0x7C8`
  - Map level: STRUCTURAL
  - Contains channel filtering/disconnection and current-pad selection.
- [ ] **`CfPadTask::checkForControllerError(bool)`** (`checkForControllerError__Q22cf9CfPadTaskFb`) · `0x801C43DC` · size `0xB4`
  - Map level: CODE_MATCH
  - Must not treat P2 presence/absence as a fatal primary-controller error.
- [ ] **`CfPadTask::create(CProcess*)`** (`create__Q22cf9CfPadTaskFP8CProcess`) · `0x801C4288` · size `0xEC`
  - Map level: CODE_MATCH
  - Task creation and lifetime for input state.

### 6.2 CfGameManager

- [ ] **`CfGameManager::getInstance()`** (`getInstance__Q22cf13CfGameManagerFv`) · `0x8007E418` · size `0x64`
  - Map level: FULL_MATCH
  - Root manager and object-layout anchor.
- [ ] **`CfGameManager::func_80082D54(int)`** (`func_80082D54__Q22cf13CfGameManagerFi`) · `0x800836D8` · size `0x3C`
  - Map level: FULL_MATCH
  - Party-slot-to-CfObjectMove resolver.
- [ ] **`CfGameManager::getCurrentPad()`** (`getCurrentPad__Q22cf13CfGameManagerFv`) · `0x80087B94` · size `0x4C`
  - Map level: FULL_MATCH
  - Finds global input consumers that must become actor/pad aware.
- [ ] **`CfGameManager::getCurrentPadChannel()`** (`getCurrentPadChannel__Q22cf13CfGameManagerFv`) · `0x80087BE0` · size `0x8`
  - Map level: FULL_MATCH
  - Current channel assumption and controller identity.
- [ ] **`CfGameManager::getCfPadData()`** (`getCfPadData__Q22cf13CfGameManagerFv`) · `0x80087C10` · size `0xC`
  - Map level: FULL_MATCH
  - Global CfPadData accessor whose callers must be classified.

### 7.2 CBattleState.cpp

- [ ] **`CBattleState::CBattleState()`** (`__ct__Q22cf12CBattleStateFv`) · `0x80146520` · size `0xA8`
  - Map level: FULL_MATCH
  - Recovers initial state and field defaults.
- [ ] **`CBattleState virtual #5`** (`CBattleState_UnkVirtualFunc5__Q22cf12CBattleStateFv`) · `0x80146DAC` · size `0x13DC`
  - Map level: STRUCTURAL
  - Large core state/update routine; trace player confirm, target, and Art activation.
- [ ] **`CBattleState virtual #6`** (`CBattleState_UnkVirtualFunc6__Q22cf12CBattleStateFv`) · `0x80148210` · size `0x154`
  - Map level: TRACE_ONLY
  - Potential transition/input helper.
- [ ] **`CBattleState virtual #8`** (`CBattleState_UnkVirtualFunc8__Q22cf12CBattleStateFv`) · `0x801485EC` · size `0x428`
  - Map level: TRACE_ONLY
  - Large state routine; classify writes and command calls.
- [ ] **`CBattleState virtual #10`** (`CBattleState_UnkVirtualFunc10__Q22cf12CBattleStateFv`) · `0x80148A18` · size `0x444`
  - Map level: TRACE_ONLY
  - Large state routine; likely mode-specific battle behavior.
- [ ] **`CBattleState virtual #11`** (`CBattleState_UnkVirtualFunc11__Q22cf12CBattleStateFv`) · `0x80148FC8` · size `0x174`
  - Map level: TRACE_ONLY
  - Candidate request/transition helper.
- [ ] **`CBattleState virtual #26`** (`CBattleState_UnkVirtualFunc26__Q22cf12CBattleStateFv`) · `0x80148364` · size `0x12C`
  - Map level: TRACE_ONLY
  - Candidate action/target helper.
- [ ] **`CBattleState virtual #29`** (`CBattleState_UnkVirtualFunc29__Q22cf12CBattleStateFv`) · `0x80148490` · size `0x15C`
  - Map level: TRACE_ONLY
  - Candidate mode transition/command helper.
- [ ] **`CBattleState virtual #31`** (`CBattleState_UnkVirtualFunc31__Q22cf12CBattleStateFv`) · `0x80149EA4` · size `0x160`
  - Map level: TRACE_ONLY
  - Candidate late action state helper.
- [ ] **`CBattleState virtual #33`** (`CBattleState_UnkVirtualFunc33__Q22cf12CBattleStateFv`) · `0x8014A014` · size `0x160`
  - Map level: TRACE_ONLY
  - Candidate late action state helper.

### 7.3 CAIAction.cpp

- [ ] **`CAIAction::CAIAction()`** (`__ct__Q22cf9CAIActionFv`) · `0x8014B308` · size `0x10C`
  - Map level: FULL_MATCH
  - Recovers fields and native action state defaults.
- [ ] **`CAIAction virtual #1`** (`CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv`) · `0x8014B41C` · size `0x110`
  - Map level: STRUCTURAL
  - Trace companion decision/update behavior and writes.
- [ ] **`CAIAction virtual #2`** (`CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv`) · `0x8014B52C` · size `0x110`
  - Map level: STRUCTURAL
  - Trace native action runner/transition behavior.

### 7.4 Arts structures

- [ ] **`CArtsSet virtual #1`** (`CArtsSet_UnkVirtualFunc1__Q22cf8CArtsSetFv`) · `0x80154740` · size `0x9C`
  - Map level: STRUCTURAL
  - Only named CArtsSet behavior seed; classify initialization/update role.

### 8.1 kyoshin/menu/CMenuArtsSelect.cpp

- [ ] **`CMenuArtsSelect::CMenuArtsSelect()`** (`__ct__CMenuArtsSelect`) · `0x80102B08` · size `0x288`
  - Map level: STRUCTURAL
  - Recovers widget/state fields and default bindings.
- [ ] **`CMenuArtsSelect::Init()`** (`Init__15CMenuArtsSelectFv`) · `0x80102EC4` · size `0xC5C`
  - Map level: STRUCTURAL
  - Widget creation, resource lookup, actor/Arts references.
- [ ] **`CMenuArtsSelect::Term()`** (`Term__15CMenuArtsSelectFv`) · `0x80103B20` · size `0x248`
  - Map level: STRUCTURAL
  - Safe duplicated/scoped HUD teardown.
- [ ] **`CMenuArtsSelect::Move()`** (`Move__15CMenuArtsSelectFv`) · `0x80103D68` · size `0xBB4`
  - Map level: STRUCTURAL
  - Cursor input, selected Art, prompt state, and UI update.
- [ ] **`CMenuArtsSelect::cbRenderBefore()`** (`cbRenderBefore__15CMenuArtsSelectFv`) · `0x8010491C` · size `0x3C0`
  - Map level: STRUCTURAL
  - Final render-time actor/Art data and transform binding.

### 8.2 kyoshin/menu/CMenuBattlePlayerState.cpp

- [ ] **`CMenuBattlePlayerState::CMenuBattlePlayerState()`** (`__ct__CMenuBattlePlayerState`) · `0x8010B880` · size `0x580`
  - Map level: STRUCTURAL
  - Recovers native HP/portrait/status widget graph.
- [ ] **`CMenuBattlePlayerState::Init()`** (`Init__22CMenuBattlePlayerStateFv`) · `0x8010C000` · size `0xC5C`
  - Map level: STRUCTURAL
  - Finds party actor and layout-resource bindings.
- [ ] **`CMenuBattlePlayerState::Term()`** (`Term__22CMenuBattlePlayerStateFv`) · `0x8010CC5C` · size `0x1B0`
  - Map level: STRUCTURAL
  - Needed if HUD instances are duplicated.
- [ ] **`CMenuBattlePlayerState::Move()`** (`Move__22CMenuBattlePlayerStateFv`) · `0x8010CE0C` · size `0x8E8`
  - Map level: STRUCTURAL
  - Updates HP, tension, statuses, portrait, and actor state.
- [ ] **`CMenuBattlePlayerState::cbRenderBefore()`** (`cbRenderBefore__22CMenuBattlePlayerStateFv`) · `0x8010D6F4` · size `0x1B4`
  - Map level: STRUCTURAL
  - Final per-view transforms and values.

### 8.3 Other battle-HUD units

- [ ] **`CMenuEnemyState::Move()`** (`Move__15CMenuEnemyStateFv`) · `0x80110888` · size `0x9B8`
  - Map level: STRUCTURAL
  - P2 target panel and target-specific status.
- [ ] **`CMenuEnemyState::cbRenderBefore()`** (`cbRenderBefore__15CMenuEnemyStateFv`) · `0x80111240` · size `0x274`
  - Map level: STRUCTURAL
  - Per-view enemy-panel rendering.
- [ ] **`CMenuPTGauge::Move()`** (`Move__12CMenuPTGaugeFv`) · `0x80188714` · size `0x35C`
  - Map level: TRACE_ONLY
  - Shared party gauge; classify as shared or duplicated.
- [ ] **`CMenuPTGauge::cbRenderBefore()`** (`cbRenderBefore__12CMenuPTGaugeFv`) · `0x80188A70` · size `0xAC`
  - Map level: TRACE_ONLY
  - Final shared gauge placement.
- [ ] **`CUIBattleManager::Move()`** (`Move__16CUIBattleManagerFv`) · `0x8012F270` · size `0xB00`
  - Map level: STRUCTURAL
  - Owns/coordinates battle UI objects and modes.
- [ ] **`CUIBattleManager::Init()`** (`Init__16CUIBattleManagerFv`) · `0x8012EEFC` · size `0xB4`
  - Map level: STRUCTURAL
  - Finds construction/lifetime of battle HUD components.

### 9.1 Camera translation units and headers

- [ ] **`CfCamEvent::initCamIntfInstances()`** (`CfCamEvent_initCamIntfInstances`) · `0x8006B3B8` · size `0x98`
  - Map level: FULL_MATCH
  - Maps camera interface ownership and event state.
- [ ] **`CfCamDirectionIntf::setPos()`** (`CfCamDirectionIntf_setPos`) · `0x8006BE48` · size `0x28`
  - Map level: FULL_MATCH
  - Direct camera-position write.
- [ ] **`CfCamDirectionIntf::setDir()`** (`CfCamDirectionIntf_setDir`) · `0x8006BE70` · size `0x74`
  - Map level: FULL_MATCH
  - Direct direction write.
- [ ] **`CfCamDirectionIntf::setLookat()`** (`CfCamDirectionIntf_setLookat`) · `0x8006BEE4` · size `0x7C`
  - Map level: FULL_MATCH
  - Direct look-at write.
- [ ] **`CfCamTargetIntf::setPos()`** (`CfCamTargetIntf_setPos`) · `0x8006C030` · size `0x30`
  - Map level: FULL_MATCH
  - Target-style camera position.
- [ ] **`CfCamTargetIntf::setDir()`** (`CfCamTargetIntf_setDir`) · `0x8006C060` · size `0x74`
  - Map level: FULL_MATCH
  - Target-style camera direction.
- [ ] **`CfCamTargetIntf::setLookat()`** (`CfCamTargetIntf_setLookat`) · `0x8006C0D4` · size `0x24`
  - Map level: FULL_MATCH
  - Target-style look-at target.
- [ ] **`CfCamFollow::CfCamFollow()`** (`__ct__cf_CfCamFollow`) · `0x8006C684` · size `0x238`
  - Map level: STRUCTURAL
  - Recovers follow subject, offsets, smoothing, and collision state fields.

### 9.2 src/kyoshin/COccCulling.cpp

- [ ] **`COccCulling::addFrustum(...)`** (`addFrustum__11COccCullingFRCQ22ml5CVec3RCQ22ml5CVec3RCQ22ml5CVec3Ul`) · `0x801A1F6C` · size `0x12C`
  - Map level: FULL_MATCH
  - Builds/records frustum candidates.
- [ ] **`COccCulling::setFrustum(CCullFrustum*)`** (`setFrustum__11COccCullingFP12CCullFrustum`) · `0x801A2098` · size `0x588`
  - Map level: STRUCTURAL
  - Primary per-view culling-state setup.
- [ ] **`COccCulling frustum helper`** (`func_801A0F04__11COccCullingFPQ22ml8CFrustum`) · `0x801A2620` · size `0x284`
  - Map level: STRUCTURAL
  - Classify visibility-list generation and fixed capacities.
- [ ] **`COccCulling cull helper`** (`func_801A1188__11COccCullingFP12CCullFrustum`) · `0x801A28A4` · size `0x2BC`
  - Map level: STRUCTURAL
  - Classify object list writes and per-view reuse.
- [ ] **`CTaskCulling virtual frustum setter`** (`ICulling_UnkVirtualFunc1__Q22cf12CTaskCullingFPQ22ml8CFrustum`) · `0x801A4340` · size `0x20`
  - Map level: FULL_MATCH
  - Connects scene camera/frustum to culling task.
- [ ] **`CTaskCulling helper`** (`func_801A2C04__Q22cf12CTaskCullingFv`) · `0x801A4324` · size `0x1C`
  - Map level: CODE_MATCH
  - Potential reset/prepare operation between views.

---

## P2 — Required for robust presentation (menus, cutscenes, transitions).

### 10.1 UI/window managers

- [ ] **`CUICfManager::Move()`** (`Move__12CUICfManagerFv`) · `0x801332A4` · size `0x97C`
  - Map level: STRUCTURAL
  - Central UI state and event/menu activity.
- [ ] **`CUICfManager helper`** (`func_80133324__12CUICfManagerFv`) · `0x80133DF8` · size `0x3C0`
  - Map level: TRACE_ONLY
  - Candidate mode/window state query/update.
- [ ] **`CUIWindowManager::Move()`** (`Move__16CUIWindowManagerFv`) · `0x8013D0C8` · size `0x4DC`
  - Map level: STRUCTURAL
  - Modal window state, input capture, and open/close transitions.
- [ ] **`CUICfManager::Init()`** (`Init__12CUICfManagerFv`) · `0x80132EC8` · size `0x2E0`
  - Map level: STRUCTURAL
  - Finds manager-owned menu/window components.
- [ ] **`CUICfManager::Term()`** (`Term__12CUICfManagerFv`) · `0x80133200` · size `0xA4`
  - Map level: TRACE_ONLY
  - Safe co-op toggle and shutdown.

---

## P3 — Required for wider exploration, streaming, and expanded memory.

### 12.2 Xenoblade/monolib memory paths

- [ ] **`mtl::MemManager::getHandleMEM2()`** (`getHandleMEM2__Q23mtl10MemManagerFv`) · `0x80436D38` · size `0x8`
  - Map level: FULL_MATCH
  - Finds original MEM2 root handle and arena assumptions.
- [ ] **`CWorkSystemMem::CWorkSystemMem(...)`** (`__ct__14CWorkSystemMemFPCcP11CWorkThread`) · `0x80447250` · size `0x60`
  - Map level: STRUCTURAL
  - Maps system-memory work object and child handles.
- [ ] **`CWorkSystemMem::getHandle()`** (`getHandle__14CWorkSystemMemFv`) · `0x80447308` · size `0xC`
  - Map level: FULL_MATCH
  - Known system handle accessor.
- [ ] **`CWorkSystemMem::wkStandbyLogout()`** (`wkStandbyLogout__14CWorkSystemMemFv`) · `0x80447318` · size `0x98`
  - Map level: STRUCTURAL
  - Memory shutdown/lifetime and leak checks.
- [ ] **`CDeviceGX::getHeapSize()`** (`getHeapSize__9CDeviceGXFv`) · `0x80459890` · size `0x8`
  - Map level: FULL_MATCH
  - Graphics heap budget and fixed-size assumptions.

---

## P2 — discovery tasks (no fixed addresses yet)

- [ ] **presentation-predicates** — Identify authoritative predicates for NORMAL_FIELD, MODAL_MENU, DIALOGUE, IN_ENGINE_CUTSCENE, LOADING, MAP_TRANSITION, VISION, CHAIN_ATTACK, PAUSE_OR_HOME_MENU
- [ ] **event-cutscene-paths** — Trace event/cutscene/loading paths from CTaskREvent, realtimeevt, CTalkWindow, CMainMenu, CVision, and map-transition tasks
- [ ] **should-render-split** — Implement deterministic ShouldRenderSplitScreen() with full view/camera/HUD/culling/GX state restore before full-screen modes

## P2 — co-op settings (native path, later)

- [ ] **settings-option-list** — Recover option list/table construction in CMainMenu / CSystemWindow / CSysWinSelect
- [ ] **settings-serialization** — Recover save/load serialization and safe extension for co-op toggle

## P3 — streaming and activation (discover from traces)

- [ ] **stream-map-area-id** — Current map/area ID accessor
- [ ] **stream-anchor-position** — Player/leader position used as streaming anchor
- [ ] **stream-terrain-sections** — Terrain section request/load/unload
- [ ] **stream-collision-sections** — Collision section request/load/unload
- [ ] **stream-spawn-despawn** — NPC/enemy spawn and despawn
- [ ] **stream-ai-activation** — AI activation/sleep distance
- [ ] **stream-lod-selection** — Model/vegetation LOD selection
- [ ] **stream-render-cull** — Object render-distance culling
- [ ] **stream-map-transition** — Map transition trigger and destination load
- [ ] **stream-teleport-regroup** — Skip travel/teleport party regroup
- [ ] **stream-pool-audit** — Fixed pool allocation and capacity checks
- [ ] **stream-async-callbacks** — Asynchronous load completion callbacks

## P3 — camera discovery (inside camera call graph)

- [ ] **cam-follow-actor-select** — Function that chooses the follow actor
- [ ] **cam-controller-pad-read** — Function that reads the camera controller/pad
- [ ] **cam-final-matrix-write** — Final calculated camera matrix/vector write
- [ ] **cam-collision-smoothing** — Camera collision and smoothing update
- [ ] **cam-scripted-override** — Scripted-camera/event override flag
- [ ] **cam-battle-lockon** — Battle camera target/lock-on adjustment
- [ ] **cam-p2-clone** — Camera state clone/recompute path for P2 without double simulation

## P1 — targeting discovery

- [ ] **target-current-pointer** — Current target pointer/ID storage
- [ ] **target-candidate-list** — Candidate target list construction
- [ ] **target-cycle-input** — Next/previous target input handling
- [ ] **target-range-los** — Range and line-of-sight validation
- [ ] **target-world-marker** — World target marker rendering (per-view presentation)
- [ ] **target-hud-lookup** — HUD enemy panel lookup
- [ ] **target-singleton-audit** — Audit globals that assume a single player target

## P1 — Blossom Dance / action prompts (trace-driven)

- [ ] **blossom-prompt-owner** — Log prompt owner actor through all four Dunban attack stages
- [ ] **blossom-pad-accessor** — Log input accessor and B-button flags per prompt
- [ ] **blossom-timing-window** — Log timing-window start/end and success/miss writes
- [ ] **blossom-stage-advance** — Log function advancing the active action stage
- [ ] **blossom-prompt-render** — Log UI routine drawing the prompt (per-actor HUD context)

---

## Recommended phase order

See `DECOMP_MAP.md` §19. Start with **Phase A (render proof)** P0 items, then **Phase B** P1 input/battle, then presentation and streaming.

Curated runnable targets (subset) also live in [`tools/coop/targets.json`](tools/coop/targets.json).
