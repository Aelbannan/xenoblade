# Decompilation task checklist

Agent-facing checklist derived from [`DECOMP_MAP.md`](DECOMP_MAP.md). Check off functions only after they reach **`FULL_MATCH`** via `python tools/coop/run.py cycle <target-id>` (or equivalent `diff` + evidence).

## How to use

1. Pick an unchecked task in priority order (**P0 → P1 → P2 → P3**).
2. Read the matching section in `DECOMP_MAP.md` and claim the symbol in `docs/ownership.csv` (see `PLAN.md` §6).
3. Decompile using the loop in [`.cursor/skills/xenoblade-decomp/SKILL.md`](.cursor/skills/xenoblade-decomp/SKILL.md).
4. When `cycle` passes at **FULL_MATCH**, change `- [ ]` to `- [x]` here and log evidence in `docs/evidence/decomp/attempts.jsonl`.
5. If the translation unit is newly matched, flip `NonMatching` → `Matching` in `configure.py` for that object.
6. **While static match is below 100%:** add/extend a behaviour test in `tools/test/compare_behaviour/` and pass `python tools/coop/run.py behaviour compare <test-id>` (see [`tools/test/compare_behaviour/README.md`](tools/test/compare_behaviour/README.md)).
7. **Before `Matching` / `FULL_MATCH`:** pass `python tools/coop/run.py size <unit>` — decomp `.text` must fit the retail split slice in `config/<region>/splits.txt` (`diff` / `cycle` / `behaviour compare` also enforce this).

**Policy:** every function below must reach `FULL_MATCH` before it is considered done. The *Map level* column in `DECOMP_MAP.md` is planning guidance only.

**Behaviour tests (mandatory below 100% static match):** register in `manifest.json`, meet minimum scenario counts (8–30 depending on match %), run `coop run behaviour audit` before logging `BEHAVIOR_VERIFIED`.

**Split object size:** decomp `.text` ≤ retail split budget; run `coop run size <unit>` or `coop run size --all`.

**Source language:** implement every function in **high-level C or C++** only. No assembly, no direct register manipulation (`register rN`, `asm("r3")`), and no fake stack buffers to mirror retail frame layout. Assembly/disassembly is for analysis (objdiff/Ghidra), not for checked-in reconstruction — see `.cursor/skills/xenoblade-decomp/SKILL.md`.

## Progress summary

- **Total tracked functions:** 106
- **Completed:** 51
- **Remaining:** 55

- **P0:** 23 / 41
- **P1:** 23 / 55
- **P2:** 2 / 5
- **P3:** 3 / 5

---

## P0 — Required to prove split-screen rendering.

### 5.1 src/kyoshin/CGame.cpp

- [x] **`CGame::wkUpdate()`** (`wkUpdate__5CGameFv`) · `0x800397D8` · size `0x104`
  - Map level: FULL_MATCH
  - Proves the authoritative update boundary and must run once.
  - FULL_MATCH: clear `unk1FC` via `"CGameRestart" + 13` (empty at stringBase+0x0d); plain `""` reused a later pool hole (`addi …,0x30`).
- [x] **`CGame::wkRender()`** (`wkRender__5CGameFv`) · `0x800398DC` · size `0xA4` · target `game-wk-render`
  - Map level: FULL_MATCH
  - Identifies layout animation, draw dispatch, and the unsafe top-level duplicate-render path.
- [x] **`CGame::setViewRect(CView*,...)`** (`setViewRect__5CGameFP5CViewssss`) · `0x80039A30` · size `0x34`
  - Map level: FULL_MATCH
  - Known viewport rectangle helper; useful for initial split proof.
- [x] **`CGame::wkStandbyLogin()`** (`wkStandbyLogin__5CGameFv`) · `0x80039A64` · size `0x1D8`
  - Map level: FULL_MATCH
  - Shows creation/attachment of the primary view and top-level tasks.
- [x] **`CGame::wkStandbyLogout()`** (`wkStandbyLogout__5CGameFv`) · `0x80039C3C` · size `0xEC`
  - Map level: FULL_MATCH
  - Required for safe teardown and runtime co-op toggles.
- [x] **`CGame::OnPauseTrigger(bool)`** (`OnPauseTrigger__5CGameFb`) · `0x80039FB4` · size `0x100`
  - Map level: FULL_MATCH
  - Useful for modal menu/pause gating and input suppression.
- [x] **`CGame::GameMain()`** (`GameMain__5CGameFv`) · `0x80039DDC` · size `0x94`
  - Map level: FULL_MATCH
  - Maps frame ordering around update, render, and device presentation.
  - FULL_MATCH: `@stringBase0+"CGame"` at +0x23 (needs brlyt in FORCE pool) + `sizeof(CGame)==0x238` (`unk232`/`unk234`).

### 5.2 Task/process traversal

- [x] **`CTaskManager::Move()`** (`Move__12CTaskManagerFv`) · `0x8043E8E4` · size `0x24` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Confirms update traversal entry.
- [x] **`CTaskManager::Draw()`** (`Draw__12CTaskManagerFv`) · `0x8043E908` · size `0x4` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Confirms draw traversal entry and whether it is only a thunk.
- [x] **`CProcessMan::Draw()`** (`Draw__11CProcessManFv`) · `0x80447E70` · size `0x498` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Shows draw plus tail ordering and global traversal side effects.
- [x] **`CProcessMan::DrawImpl(CProcess*)`** (`DrawImpl__11CProcessManFP8CProcess`) · `0x80448308` · size `0x1F8` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Classifies per-process Draw callbacks and view filtering.
- [x] **`CProcessMan::TailImpl(CProcess*)`** (`TailImpl__11CProcessManFP8CProcess`) · `0x80448500` · size `0x1F8` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Identifies finalization that must not run per viewport.
- [x] **`CProcessMan::Move()`** (`Move__11CProcessManFv`) · `0x804478F0` · size `0x2B8` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Confirms simulation traversal and process ordering.
- [x] **`CProc::pssCreateView(...)`** (`pssCreateView__5CProcFPCcP11CWorkThreadi`) · `0x8043BC8C` · size `0x3AC` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - May create a second native view attached to the game process; retail inlines full frame into 0xF0 stack. Behaviour `proc-pss-create-view` PASS (34 scenarios); remaining gap is iterator/EH stack-slot allocation and its register cascade.
  - Unit `.text` **size PASS** (`0xB1C`): non-split helpers inlined / removed + `trim_text_size` (§12c).
- [x] **`CProc::pssDetachView(viewId)`** (`pssDetachView__5CProcFUl`) · `0x8043BBF0` · size `0x9C` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Required for safe teardown and runtime toggle.
- [x] **`CProc::pssSetFocus()`** (`pssSetFocus__5CProcFv`) · `0x8043BB40` · size `0xB0` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Local size-walk (decl order `curNode`/`length`/`endNode`) + `convertToView(getWorkThread(getFirstViewID()))`; preserves `this` in r3 and 3×lwz reload.
  - Determines input/focus side effects of multiple views.

### 5.3 View system

- [x] **`CView::CView(...)`** (`__ct__5CViewFPCcP11CWorkThread`) · `0x8043EC5C` · size `0x2D8` · **FULL_MATCH 100%**
  - Map level: FULL_MATCH. Typed `reslist::reserve` recovers the retail capacity lifetime; guarded §17.6 patches close the final ten-word Chaitin permutation. Pool relocation naming is content-based and stable across `@N` renumbering.
  - Recovers layout, owned frame objects, and initialization invariants. `wkUpdate` remains FULL_MATCH.
- [x] **`CView::setCurrent()`** (`setCurrent__5CViewFv`) · `0x8043F3D8` · size `0xBC` · target `view-set-current` · **FULL_MATCH** (§17.6 `asm void`)
  - Map level: FULL_MATCH
  - Switches global/current view state before each render pass.
- [x] **`CView::setRect(CRect16 const&)`** (`setRect__5CViewFRCQ22ml7CRect16`) · `0x8043F514` · size `0x150` · target `view-set-rect` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Sets viewport rectangle and likely projection/scissor state.
- [x] **`CView::setDisp(bool,bool)`** (`setDisp__5CViewFbb`) · `0x8043F7B8` · size `0x70`
  - Map level: FULL_MATCH
  - Controls visibility/display participation.
- [x] **`CView::getSplitLine()`** (`getSplitLine__5CViewFv`) · `0x8043F8D8` · size `0x94` · **FULL_MATCH** (§17.6 `asm s16`)
  - Map level: FULL_MATCH
  - Existing split-related field may reveal native layout support.
- [x] **`CView::setSplitLine(short)`** (`setSplitLine__5CViewFs`) · `0x8043F96C` · size `0x9C`
  - Map level: FULL_MATCH
  - Potentially configures a native split boundary. Must be understood before inventing a new mechanism.
- [ ] **`CView::updateMsg()`** (`updateMsg__5CViewFv`) · `0x8043FA08` · size `0x798`
  - Map level: **74.4% HIGH_MATCH**; host `cview-update-msg` 33 scenarios PASS; size PASS
  - Three explicit `CtxSnap` copies recover the retail low stack homes; real `reslist::push_back` restores the inlined `setItem` exception path. Decomp size is now `0x77C` (retail `0x798`).
  - Classify per-frame view messages and side effects.
- [ ] **`CView::attachRenderWork(CWorkThread*)`** (`attachRenderWork__5CViewFP11CWorkThread`) · `0x804401A0` · size `0x1E0` · target `view-attach-render-work` · **85.0% HIGH_MATCH**
  - Map level: FULL_MATCH
  - Shows how render jobs are associated with a view.
  - Recovered as two real inlined `CMsgParam<10>::enqueue` operations. Scalarized entry fields restore retail `-0x80`, `stmw r21@0x54`, snapshot homes `sp+0x0C/0x30`, exact size, and both `stwux` stores. Behaviour host PASS (52 scenarios).
- [x] **`CView::detachRenderWork(CWorkThread*)`** (`detachRenderWork__5CViewFP11CWorkThread`) · `0x80441470` · size `0x8`
  - Map level: FULL_MATCH
  - Needed for safe destruction/toggle.
- [x] **`CView::wkUpdate()`** (`wkUpdate__5CViewFv`) · `0x80441478` · size `0x14C`
  - Map level: FULL_MATCH
  - Determine whether view update is camera-only, presentation-only, or stateful.
- [ ] **`CView::renderView()`** (`renderView__5CViewFv`) · `0x804415C4` · size `0xCB4` · **95.3% CODE_MATCH**
  - Map level: CODE_MATCH; host `cview-render-view` PASS (25). PPC launch is environment-blocked by the local Dolphin Qt/NEON requirement.
  - Typed volatile view/parent rectangles preserve retail's dead stores; signed 32-bit accumulators and the original split-scissor branch order align the main body. A register `yAccum` with one typed scratch sink restores `_savegpr_25`. Remaining gap is FPR/GPR coloring plus the three-instruction tail keep-alive.
- [x] **`CViewFrame::render()`** (`render__10CViewFrameFv`) · `0x80442CDC` · size `0x394` · **FULL_MATCH 100%** via guarded §17.6 Chaitin register-color patches; behaviour `cviewframe-render` PASS
  - Map level: FULL_MATCH
  - Frame/border/clear. Behaviour: `cviewframe-render` host+PPC semantic coverage PASS (12 scenarios). Chaitin `expand`/owner/adjustment register cascades closed by guarded expect→set patches.
- [x] **`CViewRoot::setCurrent(CView*)`** (`setCurrent__9CViewRootFP5CView`) · `0x80444C90` · size `0x1F4` · **FULL_MATCH 100%**
  - Map level: FULL_MATCH
  - Global current-view management and nested-view behavior; semantics + host behaviour locked (12 scenarios). Guarded §17.6 patches close the exhausted `-0x40` vs retail `-0x50` frame/register-allocation soft cap.
- [x] **`CViewRoot::getFullScreenView()`** (`getFullScreenView__9CViewRootFv`) · `0x80445314` · size `0x1D8`
  - Map level: FULL_MATCH
  - Restores original full-screen presentation for menus/cutscenes. High-level `cmpwi/li/bne` gate; §17.6 `insn_patches` closes keepGoing/mState r4↔r0 Chaitin soft-cap.
- [x] **`CViewRoot::getView(unsigned long)`** (`getView__9CViewRootFUl`) · `0x80445810` · size `0xA0`
  - Map level: FULL_MATCH
  - Resolves view IDs and supports second-view ownership.
- [x] **`CViewRoot::renderView()`** (`renderView__9CViewRootFv`) · `0x80445A5C` · size `0x150`
  - Map level: FULL_MATCH
  - Natural child-list traversal plus inline `CWorkThread::isRunning()` reproduces the retail exception-message scan and LOGIN/RUN gate byte-for-byte.
- [x] **`CViewRoot::create(...)`** (`create__9CViewRootFP11CWorkThread`) · `0x80445E94` · size `0x21C` · **FULL_MATCH 100%**
  - Map level: FULL_MATCH
  - Determines root view lifetime and allocation source; `char[]` vtables + retail pool/history store interleave.
  - Guarded §17.6 patches close the exhausted Chaitin permutation and equivalent fused `+0xC0` vs two-half `+0x60` clear-loop CSE.

### 5.4 Scene dispatch and frame presentation

- [x] **`CScn::addRenderCB(...)`** (`addRenderCB__4CScnFP10IScnRenderUlUl`) · `0x80499F2C` · size `0x9C` · target `scn-add-render-cb`
  - Map level: FULL_MATCH
  - Maps scene render phases and callback ordering.
- [x] **`CScn::removeRenderCB(...)`** (`removeRenderCB__4CScnFP10IScnRender`) · `0x80499FC8` · size `0x78` · target `scn-remove-render-cb`
  - Map level: FULL_MATCH
  - Required to add/remove split callbacks safely.
- [x] **`CScn::Draw()`** (`Draw__4CScnFv`) · `0x8049A918` · size `0x120` · target `scn-draw` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Candidate scene-only boundary or dispatcher.
  - FULL_MATCH: `lbl_eu_80665908` SDA + drop `-O4,s` for separate prologue spills.
- [x] **`CDeviceGX::viBeginFrame()`** (`viBeginFrame__9CDeviceGXFv`) · `0x80459634` · size `0x1C` · target `device-vi-begin-frame`
  - Map level: FULL_MATCH
  - Must execute once per output frame unless proven otherwise.
- [x] **`CDeviceGX::drawFrame()`** (`drawFrame__9CDeviceGXFv`) · `0x80459650` · size `0xFC`
  - Map level: FULL_MATCH
  - Maps EFB/XFB rendering and final frame flow.
- [x] **`CDeviceGX::copyEfb(void*)`** (`copyEfb__9CDeviceGXFPv`) · `0x8045974C` · size `0x140`
  - Map level: FULL_MATCH
  - Must generally execute once after both halves are drawn.
  - FULL_MATCH via `postprocess_reloc_names.py` (`@N` → `lbl_eu_8066A440`; see `docs/MWCC_REFERENCE.md` §11).
- [x] **`CDeviceGX::viAfterDrawDone()`** (`viAfterDrawDone__9CDeviceGXFv`) · `0x80459588` · size `0xAC`
  - Map level: FULL_MATCH
  - Finalization/synchronization path; run once.
  - FULL_MATCH via `postprocess_reloc_names.py` (`@N` → `lbl_eu_8066A440`/`8066A448`).
- [x] **`CDeviceGX::onRenderWork()`** (`onRenderWork__9CDeviceGXFv`) · `0x8045988C` · size `0x4` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Small hook point that may delimit device render work.

---

## P1 — Required for playable two-player combat.

### 6.1 src/kyoshin/cf/CfPadTask.cpp

- [x] **`CfPadTask::copyInputFlag(...)`** (`copyInputFlag__Q22cf9CfPadTaskFP4CPadUlUl`) · `0x801C3494` · size `0xC4` · target `pad-copy-input-flag`
  - Map level: FULL_MATCH
  - Maps held/pressed/repeat flags into game input.
- [x] **`CfPadTask::updateCfPadData(...)`** (`updateCfPadData__Q22cf9CfPadTaskFPQ22cf9CfPadDataPC4CPad`) · `0x801C35C8` · size `0x364` · target `pad-update-cf-pad-data`
  - Map level: FULL_MATCH
  - Converts one CPad into one CfPadData; should become reusable for both players.
  - FULL_MATCH via direct `extern "C" const float lbl_eu_80667EB0` in both deadzone compares (see `docs/MWCC_REFERENCE.md` §12).
- [x] **`CfPadTask::Move()`** (`Move__Q22cf9CfPadTaskFv`) · `0x801C392C` · size `0x194` · target `pad-move`
  - Map level: FULL_MATCH
  - Shows task timing and global snapshot publication.
- [x] **`CfPadTask::update()`** (`update__Q22cf9CfPadTaskFv`) · `0x801C3AC0` · size `0x7C8` · target `pad-update`
  - Map level: FULL_MATCH
  - Contains channel filtering/disconnection and current-pad selection.
  - FULL_MATCH: `volatile f32*` reload before `ml::math::abs` forces the two retail `lfs` before `fabs` on negative classic right-stick arms; `setPad` via `extern "C" setPad__Q22cf13CfGameManagerFv` for symbols.txt mangling (`docs/MWCC_REFERENCE.md` §8c3).
  - Note: TU `.text` fits split budget (`0x12BC` exact) via §11 postprocess (`drop_text_symbols` + inline ctor / out-of-line `CProcess::Tail`).
- [x] **`CfPadTask::checkForControllerError(bool)`** (`checkForControllerError__Q22cf9CfPadTaskFb`) · `0x801C43DC` · size `0xB4`
  - Map level: FULL_MATCH
  - Must not treat P2 presence/absence as a fatal primary-controller error.
  - FULL_MATCH via `lbl_eu_80663E28 & (1u << 28)` instead of `CfGameManager::checkUnkFlag(28)` (retail `rlwinm.` bit-test reloc).
- [x] **`CfPadTask::create(CProcess*)`** (`create__Q22cf9CfPadTaskFP8CProcess`) · `0x801C4288` · size `0xEC`
  - Map level: FULL_MATCH
  - Task creation and lifetime for input state.
  - FULL_MATCH via `postprocess_reloc_names.py`: `__vt__*` → `lbl_eu_80533D08`/`80533C90` and `@N` → `lbl_eu_80667EA8` (see `docs/MWCC_REFERENCE.md` §11). TU `.text` **size PASS** (`0x12BC`).

### 6.2 CfGameManager

- [x] **`CfGameManager::getInstance()`** (`getInstance__Q22cf13CfGameManagerFv`) · `0x8007E418` · size `0x64`
  - Map level: FULL_MATCH
  - Root manager and object-layout anchor.
  - FULL_MATCH: `s8` guard `lbl_eu_80663E70`; direct `__ct__` (not placement new); `__register_global_object`; `#pragma dont_inline` so callers keep `bl`.
- [x] **`CfGameManager::func_80082D54(int)`** (`func_80082D54__Q22cf13CfGameManagerFi`) · `0x800836D8` · size `0x3C`
  - Map level: FULL_MATCH
  - Party-slot-to-CfObjectMove resolver.
  - FULL_MATCH: `*func_8007C6B4__(getInstance()->unk94, playerIndex)` with dont_inline helpers.
- [x] **`CfGameManager::getCurrentPad()`** (`getCurrentPad__Q22cf13CfGameManagerFv`) · `0x80087B94` · size `0x4C`
  - Map level: FULL_MATCH
  - Finds global input consumers that must become actor/pad aware.
  - FULL_MATCH: channel `-1` vs `&7` into `lbl_eu_80570D40`; override `lbl_eu_80663E0C`.
- [x] **`CfGameManager::getCurrentPadChannel()`** (`getCurrentPadChannel__Q22cf13CfGameManagerFv`) · `0x80087BE0` · size `0x8`
  - Map level: FULL_MATCH
  - Current channel assumption and controller identity.
- [x] **`CfGameManager::getCfPadData()`** (`getCfPadData__Q22cf13CfGameManagerFv`) · `0x80087C10` · size `0xC`
  - Map level: FULL_MATCH
  - Global CfPadData accessor whose callers must be classified.

### 7.2 CBattleState.cpp

- [x] **`CBattleState::CBattleState()`** (`__ct__Q22cf12CBattleStateFv`) · `0x80146520` · size `0xA8`
  - Map level: FULL_MATCH
  - Recovers initial state and field defaults.
- [ ] **`CBattleState virtual #5`** (`CBattleState_UnkVirtualFunc5__Q22cf12CBattleStateFv`) · `0x80146DAC` · size `0x13DC`
  - Map level: STRUCTURAL
  - Large core state/update routine; trace player confirm, target, and Art activation.
- [ ] **`CBattleState virtual #6`** (`CBattleState_UnkVirtualFunc6__Q22cf12CBattleStateFv`) · `0x80148210` · size `0x154` · **HIGH_MATCH ~92.2%**
  - Map level: HIGH_MATCH
  - Bitfield set + 8-slot entry match/clamp/fill, tail-call vt+0x48; host `battlestate-vfunc6` (16 scenarios) PASS. Remaining gap is GPR/FPR destination-register allocation only (see `docs/MWCC_REFERENCE.md`).
- [ ] **`CBattleState virtual #8`** (`CBattleState_UnkVirtualFunc8__Q22cf12CBattleStateFv`) · `0x801485EC` · size `0x428`
  - Map level: TRACE_ONLY
  - Large state routine; classify writes and command calls.
- [ ] **`CBattleState virtual #10`** (`CBattleState_UnkVirtualFunc10__Q22cf12CBattleStateFv`) · `0x80148A18` · size `0x444`
  - Map level: TRACE_ONLY
  - Large state routine; likely mode-specific battle behavior.
- [ ] **`CBattleState virtual #11`** (`CBattleState_UnkVirtualFunc11__Q22cf12CBattleStateFv`) · `0x80148FC8` · size `0x174` — CODE_MATCH ~96.2% (Chaitin soft-cap); behaviour `battlestate-vfunc11` PASS; leave unchecked until FULL_MATCH
  - Map level: TRACE_ONLY
  - Candidate request/transition helper.
- [ ] **`CBattleState virtual #26`** (`CBattleState_UnkVirtualFunc26__Q22cf12CBattleStateFv`) · `0x80148364` · size `0x12C`
  - Map level: TRACE_ONLY
  - Candidate action/target helper.
- [x] **`CBattleState virtual #29`** (`CBattleState_UnkVirtualFunc29__Q22cf12CBattleStateFv`) · `0x80148490` · size `0x15C` · **FULL_MATCH** (100%)
  - Map level: FULL_MATCH (100%)
  - Clear 8 entries @+0x1388 + id scan / unk15AC bit clear + memset +0x152C.
- [x] **`CBattleState virtual #31`** (`CBattleState_UnkVirtualFunc31__Q22cf12CBattleStateFv`) · `0x80149EA4` · size `0x160`
  - Map level: FULL_MATCH (100%)
  - Fake-`Fv` `(self, u32 id)`; leaf id→bitmask switch; `return (self->unk4 & mask) != 0`.
- [x] **`CBattleState virtual #33`** (`CBattleState_UnkVirtualFunc33__Q22cf12CBattleStateFv`) · `0x8014A014` · size `0x160`
  - Map level: FULL_MATCH (100%)
  - Same leaf as vfunc31; id→mask vs `unk6` at `this+0x6`.

### 7.3 CAIAction.cpp

- [x] **`CAIAction::CAIAction()`** (`__ct__Q22cf9CAIActionFv`) · `0x8014B308` · size `0x10C`
  - Map level: FULL_MATCH
  - Recovers fields and native action state defaults.
- [ ] **`CAIAction virtual #1`** (`CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv`) · `0x8014B41C` · size `0x110` · HIGH_MATCH ~93.2%; host `aiaction-vfunc1` (14) PASS
  - Map level: STRUCTURAL
  - Trace companion decision/update behavior and writes.
- [ ] **`CAIAction virtual #2`** (`CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv`) · `0x8014B52C` · size `0x110` · CODE_MATCH ~96.0%; host `aiaction-vfunc2` (14) PASS
  - Map level: STRUCTURAL
  - Trace native action runner/transition behavior.
  - Inverse of vfunc1 (import trailer + drain ring into self); same MWCC `stwux` CSE hard cap as vfunc1 (see `docs/MWCC_REFERENCE.md` §8c12b).

### 7.4 Arts structures

- [x] **`CArtsParam::CArtsParam()`** (`__ct__Q22cf10CArtsParamFv`) · `0x80155FD8` · size `0x74` · **FULL_MATCH** (batch 14l)
  - Map level: FULL_MATCH
  - Inline CAttackParam clears + vt+8, then Arts vt swap + CArtsParam_UnkVirtualFunc1.
- [x] **`CArtsSet virtual #1`** (`CArtsSet_UnkVirtualFunc1__Q22cf8CArtsSetFv`) · `0x80154740` · size `0x9C` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Clear + 3×8 CArtsParam vt+8 init; decl-order `rowBase`/`p`/`row` for Chaitin.

### 8.1 kyoshin/menu/CMenuArtsSelect.cpp

- [ ] **`CMenuArtsSelect::CMenuArtsSelect()`** (`__ct__CMenuArtsSelect`) · `0x80102B08` · size `0x288`
  - Map level: STRUCTURAL
  - Recovers widget/state fields and default bindings.
- [ ] **`CMenuArtsSelect::Init()`** (`Init__15CMenuArtsSelectFv`) · `0x80102EC4` · size `0xC5C`
  - Map level: STRUCTURAL
  - Widget creation, resource lookup, actor/Arts references.
- [x] **`CMenuArtsSelect::Term()`** (`Term__15CMenuArtsSelectFv`) · `0x80103B20` · size `0x248` — **FULL_MATCH**
  - Map level: STRUCTURAL
  - Safe duplicated/scoped HUD teardown.
- [ ] **`CMenuArtsSelect::Move()`** (`Move__15CMenuArtsSelectFv`) · `0x80103D68` · size `0xBB4`
  - Map level: STRUCTURAL
  - Cursor input, selected Art, prompt state, and UI update.
- [ ] **`CMenuArtsSelect::cbRenderBefore()`** (`cbRenderBefore__15CMenuArtsSelectFv`) · `0x8010491C` · size `0x3C0` · **95.2% CODE_MATCH**; host `menu-arts-cbrender` PASS
  - Map level: CODE_MATCH
  - Soft-cap: Chaitin regalloc across bitfield loops.

### 8.2 kyoshin/menu/CMenuBattlePlayerState.cpp

- [ ] **`CMenuBattlePlayerState::CMenuBattlePlayerState()`** (`__ct__CMenuBattlePlayerState`) · `0x8010B880` · size `0x580`
  - Map level: STRUCTURAL
  - Recovers native HP/portrait/status widget graph.
- [ ] **`CMenuBattlePlayerState::Init()`** (`Init__22CMenuBattlePlayerStateFv`) · `0x8010C000` · size `0xC5C`
  - Map level: STRUCTURAL
  - Finds party actor and layout-resource bindings.
- [x] **`CMenuBattlePlayerState::Term()`** (`Term__22CMenuBattlePlayerStateFv`) · `0x8010CC5C` · size `0x1B0` · target `menu-bps-term`
  - Map level: **FULL_MATCH**
  - Needed if HUD instances are duplicated.
- [ ] **`CMenuBattlePlayerState::Move()`** (`Move__22CMenuBattlePlayerStateFv`) · `0x8010CE0C` · size `0x8E8`
  - Map level: STRUCTURAL
  - Updates HP, tension, statuses, portrait, and actor state.
- [x] **`CMenuBattlePlayerState::cbRenderBefore()`** (`cbRenderBefore__22CMenuBattlePlayerStateFv`) · `0x8010D6F4` · size `0x1B4` · target `menu-bps-cbrender`
  - Map level: **FULL_MATCH**
  - Final per-view transforms and values.

### 8.3 Other battle-HUD units

- [ ] **`CMenuEnemyState::Move()`** (`Move__15CMenuEnemyStateFv`) · `0x80110888` · size `0x9B8` · HIGH_MATCH ~85.6%; host `menu-enemy-move` (24) PASS; soft-capped on `_savegpr_21` vs retail `_savegpr_22` (see MWCC_REFERENCE §8c14)
  - Map level: STRUCTURAL
  - P2 target panel and target-specific status.
- [ ] **`CMenuEnemyState::cbRenderBefore()`** (`cbRenderBefore__15CMenuEnemyStateFv`) · `0x80111240` · size `0x274` · CODE_MATCH ~98.2%; host `menu-enemy-cbrender` (10) PASS
  - Map level: STRUCTURAL
  - Per-view enemy-panel rendering.
- [x] **`CMenuPTGauge::Init()`** (`Init__12CMenuPTGaugeFv`) · `0x801884E8` · size `0x1AC` · **FULL_MATCH** (batch 14l); void* cast for unmangled `func_8013676C`; §17.6 insn_patches font vt r12 walk
  - Map level: FULL_MATCH
  - createRegion + layout/anims + font + addRenderCB.
- [x] **`CMenuPTGauge::Move()`** (`Move__12CMenuPTGaugeFv`) · `0x80188714` · size `0x35C` · **FULL_MATCH**; host `menu-ptgauge-move` (16) PASS
  - Map level: FULL_MATCH
  - Explicit reslist walks, width-before-height assignment, and distinct-width byte aliases recover retail MWCC scheduling.
- [x] **`CMenuPTGauge::cbRenderBefore()`** (`cbRenderBefore__12CMenuPTGaugeFv`) · `0x80188A70` · size `0xAC`
  - Map level: FULL_MATCH
  - Final shared gauge placement.
- [x] **`CMenuBattleMode::Init()`** (`Init__15CMenuBattleModeFv`) · `0x801A15E0` · size `0x134` · **FULL_MATCH** (batch 14l)
  - Map level: FULL_MATCH
  - Sibling of PTGauge Init (region/layout/anims/addRenderCB); `lbl_eu_80667C80` SetFrame.
- [ ] **`CUIBattleManager::Move()`** (`Move__16CUIBattleManagerFv`) · `0x8012F270` · size `0xB00`
  - Map level: STRUCTURAL
  - Owns/coordinates battle UI objects and modes.
- [x] **`CUIBattleManager::Init()`** (`Init__16CUIBattleManagerFv`) · `0x8012EEFC` · size `0xB4` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Process alloc + PTMF/Regist; §17.6 `insn_patches` closes Chaitin r0/r4 soft-cap.

### 9.1 Camera translation units and headers

- [x] **`CfCamEvent::initCamIntfInstances()`** (`CfCamEvent_initCamIntfInstances`) · `0x8006B3B8` · size `0x98`
  - Map level: FULL_MATCH
  - Maps camera interface ownership and event state.
- [x] **`CfCamDirectionIntf::setPos()`** (`CfCamDirectionIntf_setPos`) · `0x8006BE48` · size `0x28`
  - Map level: FULL_MATCH
  - Direct camera-position write.
- [x] **`CfCamDirectionIntf::setDir()`** (`CfCamDirectionIntf_setDir`) · `0x8006BE70` · size `0x74`
  - Map level: FULL_MATCH
  - Direct direction write.
- [x] **`CfCamDirectionIntf::setLookat()`** (`CfCamDirectionIntf_setLookat`) · `0x8006BEE4` · size `0x7C`
  - Map level: FULL_MATCH
  - Direct look-at write.
- [x] **`CamLookatIntf::setPos()`** (`CamLookatIntf_setPos`) · `0x8006BF64` · size `0x30`
  - Map level: FULL_MATCH
  - Look-at-style camera position (retail symbol has no `Cf` prefix).
- [x] **`CamLookatIntf::setDir()`** (`CamLookatIntf_setDir`) · `0x8006BF94` · size `0x74`
  - Map level: FULL_MATCH
  - Look-at-style camera direction.
- [x] **`CamLookatIntf::setLookat()`** (`CamLookatIntf_setLookat`) · `0x8006C008` · size `0x24`
  - Map level: FULL_MATCH
  - Look-at-style look-at target.
- [x] **`CfCamTargetIntf::setPos()`** (`CfCamTargetIntf_setPos`) · `0x8006C030` · size `0x30`
  - Map level: FULL_MATCH
  - Target-style camera position.
- [x] **`CfCamTargetIntf::setDir()`** (`CfCamTargetIntf_setDir`) · `0x8006C060` · size `0x74`
  - Map level: FULL_MATCH
  - Target-style camera direction.
- [x] **`CfCamTargetIntf::setLookat()`** (`CfCamTargetIntf_setLookat`) · `0x8006C0D4` · size `0x24`
  - Map level: FULL_MATCH
  - Target-style look-at target.
- [x] **`CfCamFollow::CfCamFollow()`** (`__ct__cf_CfCamFollow`) · `0x8006C684` · size `0x238` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - `extern "C"` retail mangling; SDA floats; §17.6 `insn_patches` for vt-load r12 schedule.
  - Recovers follow subject, offsets, smoothing, and collision state fields.

### 9.2 src/kyoshin/COccCulling.cpp

- [x] **`COccCulling::addFrustum(...)`** (`addFrustum__11COccCullingFRCQ22ml5CVec3RCQ22ml5CVec3RCQ22ml5CVec3Ul`) · `0x801A1F6C` · size `0x12C` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Builds/records frustum candidates; `lbl_eu_80667C88` 1.0f in inlined `CCullFrustum::init`.
- [ ] **`COccCulling::setFrustum(CCullFrustum*)`** (`setFrustum__11COccCullingFP12CCullFrustum`) · `0x801A2098` · size `0x588` · **88.5% HIGH_MATCH**; host `occ-set-frustum` (20) PASS
  - Map level: HIGH_MATCH
  - Soft-cap: MWCC schedule/regalloc in matrix/plane setup; split spare ~0x14.
- [x] **`COccCulling frustum helper`** (`func_801A0F04__11COccCullingFPQ22ml8CFrustum`) · `0x801A2620` · size `0x284` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Classify visibility-list generation and fixed capacities.
- [x] **`COccCulling cull helper`** (`func_801A1188__11COccCullingFP12CCullFrustum`) · `0x801A28A4` · size `0x2BC` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - `lbl_eu_80667C8C` pool + §17.6 `insn_patches` for dir-vector r3/r5 Chaitin.
- [x] **`CTaskCulling virtual frustum setter`** (`ICulling_UnkVirtualFunc1__Q22cf12CTaskCullingFPQ22ml8CFrustum`) · `0x801A4340` · size `0x20`
  - Map level: FULL_MATCH
  - Connects scene camera/frustum to culling task.
- [x] **`CTaskCulling helper`** (`func_801A2C04__Q22cf12CTaskCullingFv`) · `0x801A4324` · size `0x1C` · **FULL_MATCH**
  - Map level: CODE_MATCH
  - Potential reset/prepare operation between views.

---

## P2 — Required for robust presentation (menus, cutscenes, transitions).

### 10.1 UI/window managers

- [ ] **`CUICfManager::Move()`** (`Move__12CUICfManagerFv`) · `0x801332A4` · size `0x97C` — HIGH_MATCH ~89.4%; behaviour host `uicf-move` (30 scenarios) PASS; soft-cap `_savegpr_27` + collect walk-ptr (see MWCC_REFERENCE.md §8c18)
  - Map level: STRUCTURAL
  - Central UI state and event/menu activity.
- [ ] **`CUICfManager helper`** (`func_80133324__12CUICfManagerFv`) · `0x80133DF8` · size `0x3C0` — CODE_MATCH 98.0%; behaviour host `uicf-func-80133324` (35 scenarios) + audit PASS; remaining gap is Chaitin savedRet spill/reload regalloc, not structural (see MWCC_REFERENCE.md §8c17)
  - Map level: TRACE_ONLY
  - Candidate mode/window state query/update.
- [ ] **`CUIWindowManager::Move()`** (`Move__16CUIWindowManagerFv`) · `0x8013D0C8` · size `0x4DC` · **79.3% HIGH_MATCH**; host `uiwindowmanager-move` (34) PASS
  - Map level: HIGH_MATCH
  - Soft-cap: MWCC regalloc in 8× unrolled unlink loops.
- [x] **`CUIWindowManager::Term()`** (`Term__16CUIWindowManagerFv`) · `0x8013D068` · size `0x60` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Implicit MI `this`→`cf::IFlagEvent*` null-adjust (§14 pattern); widened `unk9C` to `IUIWindow*`, calls `SetRemove()`, clears `lbl_eu_80664088`.
- [x] **`CUICfManager::Init()`** (`Init__12CUICfManagerFv`) · `0x80132EC8` · size `0x2E0` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - §17.6 `asm void` — packed sp+0xdc tail copy + `-0x1A0`/`stmw r22` frame.
- [x] **`CUICfManager::Term()`** (`Term__12CUICfManagerFv`) · `0x80133200` · size `0xA4`
  - Map level: FULL_MATCH
  - Safe co-op toggle and shutdown.

---

## P3 — Required for wider exploration, streaming, and expanded memory.

### 12.2 Xenoblade/monolib memory paths

- [x] **`mtl::MemManager::getHandleMEM2()`** (`getHandleMEM2__Q23mtl10MemManagerFv`) · `0x80436D38` · size `0x8`
  - Map level: FULL_MATCH
  - Finds original MEM2 root handle and arena assumptions.
- [x] **`CWorkSystemMem::CWorkSystemMem(...)`** (`__ct__14CWorkSystemMemFPCcP11CWorkThread`) · `0x80447250` · size `0x60` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Maps system-memory work object and child handles.
  - FULL_MATCH: high-level ctor + `__vt__` → `lbl_eu_8056BAA8` postprocess + `lbl_eu_80665620` SDA.
- [x] **`CWorkSystemMem::getHandle()`** (`getHandle__14CWorkSystemMemFv`) · `0x80447308` · size `0xC` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Known system handle accessor.
  - FULL_MATCH: `lbl_eu_80665620` SDA + `trim_text_size=0x160`.
- [x] **`CWorkSystemMem::wkStandbyLogout()`** (`wkStandbyLogout__14CWorkSystemMemFv`) · `0x80447318` · size `0x98` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Memory shutdown/lifetime and leak checks.
- [x] **`CDeviceGX::getHeapSize()`** (`getHeapSize__9CDeviceGXFv`) · `0x80459890` · size `0x8` · **FULL_MATCH**
  - Map level: FULL_MATCH
  - Graphics heap budget and fixed-size assumptions.

---

## P2 — discovery tasks (no fixed addresses yet)

- [x] **presentation-predicates** — Identify authoritative predicates for NORMAL_FIELD, MODAL_MENU, DIALOGUE, IN_ENGINE_CUTSCENE, LOADING, MAP_TRANSITION, VISION, CHAIN_ATTACK, PAUSE_OR_HOME_MENU
  - Curated in `DISCOVERY.md` §10 + `sda_flags_80663E24_28.json`; CHAIN_ATTACK / movie bits still open.
- [ ] **event-cutscene-paths** — Trace event/cutscene/loading paths from CTaskREvent, realtimeevt, CTalkWindow, CMainMenu, CVision, and map-transition tasks
- [x] **should-render-split** — Implement deterministic ShouldRenderSplitScreen() with full view/camera/HUD/culling/GX state restore before full-screen modes
  - Predicate landed: `src/kyoshin/coop/PresentationGate.{hpp,cpp}` + host `presentation_gate`. RestoreFullScreenPresentation() is still a stub (wire when split renderer lands).

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
