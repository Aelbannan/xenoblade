# Xenoblade Chronicles Wii single-instance split-screen: decompilation target map

**Purpose:** identify the concrete source paths, original translation units, functions, structures, call paths, and unknowns that must be recovered to implement the single-instance split-screen design in `PLAN.md`.

**Source snapshot date:** 2026-07-12. Addresses in this document are US-build seeds taken from the current public symbol map. Agents must verify the exact executable hash and current `config/<region>/symbols.txt` before using any address.

**Primary rule:** decompile the smallest connected behavior slice needed to implement and prove the feature. Do not require the entire game to be decompiled before implementation begins.

**Current project policy (2026-07):** every listed target must reach **`EQUIVALENT_MATCH`** or **`FULL_MATCH`** — both are equal-tier acceptance outcomes. Recovery-level columns below remain as historical planning notes; the active acceptance bar for decompilation work is `EQUIVALENT_MATCH` (fuzzy ≥ 50% + SMT equivalent + split-size fit) or `FULL_MATCH` (100% static + split-size fit).

**Companion documents:**

- `PLAN.md`
- `Xenoblade_Wii_Coop_Decompilation_Guide.md`

The public repository is a work-in-progress matching decompilation. Some translation units named in `config/<region>/splits.txt` do not yet have corresponding reconstructed `.cpp` files. In this document:

- **source path** means a file currently present in the public repository;
- **logical translation unit** means the original object/file boundary named by the split map, even when its source body is still missing;
- **function seed** means a symbol/address starting point, not a guarantee that the current name or signature is final.

---

## 1. Required recovery levels

Use one of these levels for every target.

| Level | Meaning | When sufficient |
|---|---|---|
| **TRACE_ONLY** | Function boundary, callers, callees, arguments, important reads/writes, and side effects are understood. | A narrow binary hook can be placed safely without reconstructing the whole function. |
| **STRUCTURAL** | Compilable pseudocode/C++ captures control flow and object accesses, but may not match instructions. | Needed to understand state ownership, split update from render, or safely refactor a large routine. |
| **BEHAVIORAL** | Reimplementation passes controlled runtime tests against the original. | Suitable for downstream replacement when exact matching is not needed. |
| **CODE_MATCH** | Generated instruction bytes match for the function body. | Strong evidence for helpers, getters, state setters, and hook-critical leaf functions. |
| **FULL_MATCH** | Instructions, relocations, stack shape, constants, relevant data match, and split-size fit. | One of two equal-tier acceptance outcomes for this project (alongside `EQUIVALENT_MATCH`). |
| **EQUIVALENT_MATCH** | Fuzzy ≥ 50%, SMT proves behavioral equivalence under `ppc-eabi`, and split-size fit. | One of two equal-tier acceptance outcomes for this project. |

**Current project policy:** every decompilation target must reach **`EQUIVALENT_MATCH`** or **`FULL_MATCH`** before it is considered complete. Both are equal-tier outcomes — `EQUIVALENT_MATCH` guarantees semantic correctness via SMT proof + split-size fit, `FULL_MATCH` guarantees byte-level identity + split-size fit. Use the levels above as progress labels during iteration, but do not stop at `STRUCTURAL`, `CODE_MATCH`, or `HIGH_MATCH`.

A large function does **not** need to be understood in one pass, but it **does** need to reach `EQUIVALENT_MATCH` or `FULL_MATCH` before the target is closed. Large render/UI/battle routines may be decomposed into leaf helpers first; each helper and the parent function must still end at `EQUIVALENT_MATCH` or `FULL_MATCH`.

---

## 2. Critical-path graph

```text
Frame/update boundary
  CGame::wkUpdate / wkRender
          |
          v
View and process traversal
  CView / CViewRoot / CProcessMan / CScn / CDeviceGX
          |
          +--------------------+
          |                    |
          v                    v
Two sequential scene views   Full-screen/split state switch
  camera + culling             menus/events/cutscenes
          |                    |
          v                    v
Native HUD context          presentation state machine
          |
          v
P2 actor/input/target/Arts/action prompts
          |
          v
Streaming envelope + optional expanded memory
```

No agent should start by rewriting all of `CUIBattleManager`, `CAIAction`, or `CView::renderView`. First recover call graphs and isolate the smallest hookable leaves.

---

## 3. Priority tiers

### P0 — required to prove split-screen rendering

1. Frame update versus render boundary.
2. View creation, current-view switching, viewport/scissor/projection setup.
3. Scene-only render call that can execute twice without simulation mutation.
4. Frame finalization/present path that must execute once.
5. Render-state save/restore and global scratch behavior.

### P1 — required for playable two-player combat

1. Second controller snapshot.
2. Party-slot-to-actor resolution.
3. Player/AI ownership boundary.
4. Independent target and Art selection state.
5. Native Art command submission.
6. Actor-specific follow-up prompts such as Blossom Dance.
7. HUD actor/pad/target/context binding.
8. Independent camera subject and per-view culling.

### P2 — required for robust presentation

1. Menu, cutscene, dialogue, loading, vision, and Chain Attack detection.
2. Safe split/full-screen transitions.
3. Co-op setting and persistence.
4. Half-screen layout/aspect handling.
5. Local and shared overlay classification.

### P3 — required for wider exploration and performance

1. Streaming/activation anchor discovery.
2. Two-player loading envelope.
3. Tether and transition behavior.
4. Draw-distance and fixed-pool audit.
5. Expanded MEM2/co-op heap.
6. Long-run stability and performance tuning.

---

## 4. Region and build prerequisites

Use the exact checkout's files:

```text
config/jp/symbols.txt
config/jp/splits.txt
config/eu/symbols.txt
config/eu/splits.txt
config/us/symbols.txt
config/us/splits.txt
```

Expected `main.dol` SHA-1 values at the source snapshot:

```text
JP  a564033aee46988743d8f5e6fdc50a8c65791160
EU  10d34dbf901e5d6547718176303a6073ee80dda2
US  214b15173fa3bad23a067476d58d3933ad7037b7
```

Every function dossier must include:

```yaml
region: US
main_dol_sha1: 214b15173fa3bad23a067476d58d3933ad7037b7
repository_commit: <commit>
translation_unit: <split-map path>
function_symbol: <symbol-map name>
address: <verified address>
size: <verified size>
recovery_level: TRACE_ONLY|STRUCTURAL|BEHAVIORAL|CODE_MATCH|FULL_MATCH
```

---

## 5. P0 frame-loop and render-boundary targets

### 5.1 `src/kyoshin/CGame.cpp`

**Status:** source exists and is the top-level starting point.

**Questions to answer:**

- Exactly what advances in `wkUpdate()`?
- Exactly what advances in `wkRender()` before task drawing?
- Which UI/layout animation is advanced in `wkRender()`?
- What function starts a frame, dispatches views, and presents/copies the EFB?
- Where can the split renderer be inserted so update runs once and scene/HUD presentation runs twice?
- What pause/event state is visible here for full-screen gating?

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CGame::wkUpdate()` | `wkUpdate__5CGameFv` | `0x800397D8` | `0x104` | Proves the authoritative update boundary and must run once. | **FULL_MATCH** |
| `CGame::wkRender()` | `wkRender__5CGameFv` | `0x800398DC` | `0xA4` | Identifies layout animation, draw dispatch, and the unsafe top-level duplicate-render path. | **FULL_MATCH** |
| `CGame::setViewRect(CView*,...)` | `setViewRect__5CGameFP5CViewssss` | `0x80039A30` | `0x34` | Known viewport rectangle helper; useful for initial split proof. | **FULL_MATCH** |
| `CGame::wkStandbyLogin()` | `wkStandbyLogin__5CGameFv` | `0x80039A64` | `0x1D8` | Shows creation/attachment of the primary view and top-level tasks. | **STRUCTURAL** |
| `CGame::wkStandbyLogout()` | `wkStandbyLogout__5CGameFv` | `0x80039C3C` | `0xEC` | Required for safe teardown and runtime co-op toggles. | **STRUCTURAL** |
| `CGame::OnPauseTrigger(bool)` | `OnPauseTrigger__5CGameFb` | `0x80039FB4` | `0x100` | Useful for modal menu/pause gating and input suppression. | **CODE_MATCH** |
| `CGame::GameMain()` | `GameMain__5CGameFv` | `0x80039DDC` | `0x94` | Maps frame ordering around update, render, and device presentation. | **FULL_MATCH** |


**Acceptance gate:** produce a frame-order trace showing one complete normal frame from input sampling through update, scene rendering, HUD rendering, EFB copy, and VI presentation. Mark every function that writes simulation state.

### 5.2 Task/process traversal

**Current source paths:**

```text
libs/monolib/src/work/CProcess.cpp
libs/monolib/src/core/CProc.cpp
```

**Logical translation units to locate through the split map:**

```text
CTaskManager.cpp
CView.cpp / CViewFrame / CViewRoot object range
```

The exact repository path for an unreconstructed logical translation unit may differ from the logical name. Use `splits.txt` and function addresses as the source of truth.

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CTaskManager::Move()` | `Move__12CTaskManagerFv` | `0x8043E8E4` | `0x24` | Confirms update traversal entry. | **CODE_MATCH** |
| `CTaskManager::Draw()` | `Draw__12CTaskManagerFv` | `0x8043E908` | `0x4` | Confirms draw traversal entry and whether it is only a thunk. | **CODE_MATCH** |
| `CProcessMan::Draw()` | `Draw__11CProcessManFv` | `0x80447E70` | `0x498` | Shows draw plus tail ordering and global traversal side effects. | **FULL_MATCH** |
| `CProcessMan::DrawImpl(CProcess*)` | `DrawImpl__11CProcessManFP8CProcess` | `0x80448308` | `0x1F8` | Classifies per-process Draw callbacks and view filtering. | **FULL_MATCH** |
| `CProcessMan::TailImpl(CProcess*)` | `TailImpl__11CProcessManFP8CProcess` | `0x80448500` | `0x1F8` | Identifies finalization that must not run per viewport. | **FULL_MATCH** |
| `CProcessMan::Move()` | `Move__11CProcessManFv` | `0x804478F0` | `0x2B8` | Confirms simulation traversal and process ordering. | **CODE_MATCH** |
| `CProc::pssCreateView(...)` | `pssCreateView__5CProcFPCcP11CWorkThreadi` | `0x8043BC8C` | `0x3AC` | May create a second native view attached to the game process. | **FULL_MATCH** |
| `CProc::pssDetachView(viewId)` | `pssDetachView__5CProcFUl` | `0x8043BBF0` | `0x9C` | Required for safe teardown and runtime toggle. | **FULL_MATCH** |
| `CProc::pssSetFocus()` | `pssSetFocus__5CProcFv` | `0x8043BB40` | `0xB0` | Determines input/focus side effects of multiple views. | **FULL_MATCH** |


**Required output:** a process inventory for one gameplay frame. For every root and relevant child process, record whether its `Draw()` and `Tail()` methods are pure presentation, mutate timers, generate audio, allocate memory, or write gameplay state.

### 5.3 View system

**Target object family:** `CView`, `CViewFrame`, `CViewRoot`.

This is the highest-value P0 decompilation slice. The first implementation may use either one view with two sequential rectangles or two native `CView` instances. The following functions determine which design is safer.

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CView::CView(...)` | `__ct__5CViewFPCcP11CWorkThread` | `0x8043EC5C` | `0x2D8` | Recovers layout, owned frame objects, and initialization invariants. | **FULL_MATCH** |
| `CView::setCurrent()` | `setCurrent__5CViewFv` | `0x8043F3D8` | `0xBC` | Switches global/current view state before each render pass. | **FULL_MATCH** (`enqueue(6)` + §17.6 `insn_patches`) |
| `CView::setRect(CRect16 const&)` | `setRect__5CViewFRCQ22ml7CRect16` | `0x8043F514` | `0x150` | Sets viewport rectangle and likely projection/scissor state. | **FULL_MATCH** |
| `CView::setDisp(bool,bool)` | `setDisp__5CViewFbb` | `0x8043F7B8` | `0x70` | Controls visibility/display participation. | **FULL_MATCH** |
| `CView::getSplitLine()` | `getSplitLine__5CViewFv` | `0x8043F8D8` | `0x94` | Existing split-related field may reveal native layout support. | **FULL_MATCH** |
| `CView::setSplitLine(short)` | `setSplitLine__5CViewFs` | `0x8043F96C` | `0x9C` | Potentially configures a native split boundary. Must be understood before inventing a new mechanism. | **FULL_MATCH** |
| `CView::updateMsg()` | `updateMsg__5CViewFv` | `0x8043FA08` | `0x798` | Classify per-frame view messages and side effects. | **74.4% HIGH_MATCH** |
| `CView::attachRenderWork(CWorkThread*)` | `attachRenderWork__5CViewFP11CWorkThread` | `0x804401A0` | `0x1E0` | Shows how render jobs are associated with a view. | **FULL_MATCH** (`CMsgParam` + §17.6 `insn_patches`) |
| `CView::detachRenderWork(CWorkThread*)` | `detachRenderWork__5CViewFP11CWorkThread` | `0x80441470` | `0x8` | Needed for safe destruction/toggle. | **FULL_MATCH** |
| `CView::wkUpdate()` | `wkUpdate__5CViewFv` | `0x80441478` | `0x14C` | Determine whether view update is camera-only, presentation-only, or stateful. | **FULL_MATCH** |
| `CView::renderView()` | `renderView__5CViewFv` | `0x804415C4` | `0xCB4` | Primary candidate for a repeatable per-view render pass. | **95.3% CODE_MATCH** |
| `CViewFrame::render()` | `render__10CViewFrameFv` | `0x80442CDC` | `0x394` | Frame/border/clear behavior around a viewport. | **FULL_MATCH** |
| `CViewRoot::setCurrent(CView*)` | `setCurrent__9CViewRootFP5CView` | `0x80444C90` | `0x1F4` | Global current-view management and nested-view behavior. | **FULL_MATCH** |
| `CViewRoot::getFullScreenView()` | `getFullScreenView__9CViewRootFv` | `0x80445314` | `0x1D8` | Restores original full-screen presentation for menus/cutscenes. | **FULL_MATCH** |
| `CViewRoot::getView(unsigned long)` | `getView__9CViewRootFUl` | `0x80445810` | `0xA0` | Resolves view IDs and supports second-view ownership. | **FULL_MATCH** |
| `CViewRoot::renderView()` | `renderView__9CViewRootFv` | `0x80445A5C` | `0x150` | Maps root-level view traversal and finalization. | **STRUCTURAL** |
| `CViewRoot::create(...)` | `create__9CViewRootFP11CWorkThread` | `0x80445E94` | `0x21C` | Determines root view lifetime and allocation source. | **FULL_MATCH** |


**Decisive experiments:**

1. Call `setRect()` on the existing gameplay view and verify correct viewport/scissor behavior.
2. Render the same camera into two rectangles without calling `CGame::wkRender()` twice.
3. Instrument `CView::renderView()` and `CViewFrame::render()` for writes outside known render state.
4. Determine what `splitLine` means in the original engine. Do not assume it is split-screen support until runtime-confirmed.
5. Verify whether two views can share the same render work safely or whether work must be attached/detached for each pass.

### 5.4 Scene dispatch and frame presentation

**Likely source/logical paths:**

```text
monolib scene subsystem containing CScn / CScnNw4r / CScnRootNw4r
libs/monolib/src/device/CDeviceGX.cpp
libs/nw4r/src/g3d/g3d_workmem.cpp
```

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CScn::addRenderCB(...)` | `addRenderCB__4CScnFP10IScnRenderUlUl` | `0x80499F2C` | `0x9C` | Maps scene render phases and callback ordering. | **FULL_MATCH** |
| `CScn::removeRenderCB(...)` | `removeRenderCB__4CScnFP10IScnRender` | `0x80499FC8` | `0x78` | Required to add/remove split callbacks safely. | **FULL_MATCH** |
| `CScn::Draw()` | `Draw__4CScnFv` | `0x8049A918` | `0x120` | Candidate scene-only boundary or dispatcher. | **STRUCTURAL** |
| `CDeviceGX::viBeginFrame()` | `viBeginFrame__9CDeviceGXFv` | `0x80459634` | `0x1C` | Must execute once per output frame unless proven otherwise. | **FULL_MATCH** |
| `CDeviceGX::drawFrame()` | `drawFrame__9CDeviceGXFv` | `0x80459650` | `0xFC` | Maps EFB/XFB rendering and final frame flow. | **FULL_MATCH** |
| `CDeviceGX::copyEfb(void*)` | `copyEfb__9CDeviceGXFPv` | `0x8045974C` | `0x140` | Must generally execute once after both halves are drawn. | **FULL_MATCH** |
| `CDeviceGX::viAfterDrawDone()` | `viAfterDrawDone__9CDeviceGXFv` | `0x80459588` | `0xAC` | Finalization/synchronization path; run once. | **FULL_MATCH** |
| `CDeviceGX::onRenderWork()` | `onRenderWork__9CDeviceGXFv` | `0x8045988C` | `0x4` | Small hook point that may delimit device render work. | **CODE_MATCH** |


**Required classification:** identify all GX viewport, scissor, projection, EFB clear/copy, depth clear, and draw-sync calls reachable from the view and device paths. Record which must occur once per frame, once per view, or once per render phase.

---

## 6. P1 controller and player-ownership targets

### 6.1 `src/kyoshin/cf/CfPadTask.cpp` and headers

**Current source paths:**

```text
src/kyoshin/cf/CfPadTask.cpp
src/kyoshin/cf/CfPadTask.hpp
src/kyoshin/cf/CfPadData.hpp
```

**Related logical translation units:**

```text
kyoshin/cf/CtrlRemote.cpp
kyoshin/cf/CtrlPc.cpp
```

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CfPadTask::copyInputFlag(...)` | `copyInputFlag__Q22cf9CfPadTaskFP4CPadUlUl` | `0x801C3494` | `0xC4` | Maps held/pressed/repeat flags into game input. | **FULL_MATCH** |
| `CfPadTask::updateCfPadData(...)` | `updateCfPadData__Q22cf9CfPadTaskFPQ22cf9CfPadDataPC4CPad` | `0x801C35C8` | `0x364` | Converts one CPad into one CfPadData; should become reusable for both players. | **FULL_MATCH** |
| `CfPadTask::Move()` | `Move__Q22cf9CfPadTaskFv` | `0x801C392C` | `0x194` | Shows task timing and global snapshot publication. | **FULL_MATCH** |
| `CfPadTask::update()` | `update__Q22cf9CfPadTaskFv` | `0x801C3AC0` | `0x7C8` | Contains channel filtering/disconnection and current-pad selection. | **FULL_MATCH** |
| `CfPadTask::checkForControllerError(bool)` | `checkForControllerError__Q22cf9CfPadTaskFb` | `0x801C43DC` | `0xB4` | Must not treat P2 presence/absence as a fatal primary-controller error. | **FULL_MATCH** |
| `CfPadTask::create(CProcess*)` | `create__Q22cf9CfPadTaskFP8CProcess` | `0x801C4288` | `0xEC` | Task creation and lifetime for input state. | **FULL_MATCH** |


**Required data recovery:**

- complete `CfPadData` field names and semantics;
- current, trigger, release, and repeat flags;
- analog axes, dead zones, and Classic Controller mappings;
- channel-to-`CPad` lookup;
- secondary channel disconnect/filter branches;
- Home Menu and controller-error interactions.

**Acceptance:** one Dolphin process receives controller 0 and controller 1 into two stable snapshots every authoritative frame, with no menu/HUD rerouting yet.

### 6.2 `CfGameManager` player and pad accessors

**Current source/header path:**

```text
src/kyoshin/cf/CfGameManager.hpp
```

**Logical translation unit:**

```text
kyoshin/cf/CfGameManager.cpp
```

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CfGameManager::getInstance()` | `getInstance__Q22cf13CfGameManagerFv` | `0x8007E418` | `0x64` | Root manager and object-layout anchor. | **FULL_MATCH** |
| `CfGameManager::func_80082D54(int)` | `func_80082D54__Q22cf13CfGameManagerFi` | `0x800836D8` | `0x3C` | Party-slot-to-CfObjectMove resolver. | **FULL_MATCH** |
| `CfGameManager::getCurrentPad()` | `getCurrentPad__Q22cf13CfGameManagerFv` | `0x80087B94` | `0x4C` | Finds global input consumers that must become actor/pad aware. | **FULL_MATCH** |
| `CfGameManager::getCurrentPadChannel()` | `getCurrentPadChannel__Q22cf13CfGameManagerFv` | `0x80087BE0` | `0x8` | Current channel assumption and controller identity. | **FULL_MATCH** |
| `CfGameManager::getCfPadData()` | `getCfPadData__Q22cf13CfGameManagerFv` | `0x80087C10` | `0xC` | Global CfPadData accessor whose callers must be classified. | **FULL_MATCH** |


**Caller audit:** enumerate every caller of the current-pad and `CfPadData` accessors and tag it as:

```text
GAMEPLAY_MOVEMENT
GAMEPLAY_ARTS
GAMEPLAY_TARGETING
CAMERA_INPUT
HUD_INPUT
MODAL_MENU
DIALOGUE
DEBUG
UNKNOWN
```

The project should avoid globally replacing these accessors. Patch or wrap only the relevant caller categories.

---

## 7. P1 actor, AI, battle, target, and Art-execution targets

### 7.1 Required object-layout paths

```text
src/kyoshin/cf/object/CfObjectActor.hpp
src/kyoshin/cf/object/CActorParam.hpp
src/kyoshin/cf/object/CAIAction.hpp
src/kyoshin/cf/CArtsSet.hpp
src/kyoshin/cf/CArtsSet.cpp
src/kyoshin/cf/CArtsParam.cpp
```

**Logical translation units:**

```text
kyoshin/cf/object/CBattleState.cpp
kyoshin/cf/object/CAIAction.cpp
kyoshin/cf/CArtsSet.cpp
kyoshin/cf/CArtsParam.cpp
kyoshin/cf/CBattleManager.cpp
```

Known candidate layout chain to validate at runtime:

```text
CfObjectActor + 0x3380 -> CAIAction
CfObjectActor + 0x3E9C -> CfObjectMove
CfObjectActor + 0x19E8 -> CArtsSet through CActorParam layout
CfObjectMove* - 0x3E9C -> actor base
CfObjectMove* - 0x24B4 -> candidate CArtsSet*
```

Do not treat names for unknown fields as confirmed merely because the offsets match.

### 7.2 `CBattleState.cpp`

The large virtual functions are priority targets because they likely contain the manually controlled actor's battle state machine, cursor/action state, command validation, or transition logic.

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CBattleState::CBattleState()` | `__ct__Q22cf12CBattleStateFv` | `0x80146520` | `0xA8` | Recovers initial state and field defaults. | **FULL_MATCH** |
| `CBattleState virtual #5` | `CBattleState_UnkVirtualFunc5__Q22cf12CBattleStateFv` | `0x80146DAC` | `0x13DC` | Large core state/update routine; trace player confirm, target, and Art activation. | **STRUCTURAL** |
| `CBattleState virtual #6` | `CBattleState_UnkVirtualFunc6__Q22cf12CBattleStateFv` | `0x80148210` | `0x154` | Bitfield set + 8-slot entry match/clamp/fill, tail-call vt+0x48; host `battlestate-vfunc6` (16) PASS. | **HIGH_MATCH ~92.2%** |
| `CBattleState virtual #8` | `CBattleState_UnkVirtualFunc8__Q22cf12CBattleStateFv` | `0x801485EC` | `0x428` | Large state routine; classify writes and command calls. | **TRACE_ONLY** |
| `CBattleState virtual #10` | `CBattleState_UnkVirtualFunc10__Q22cf12CBattleStateFv` | `0x80148A18` | `0x444` | Large state routine; likely mode-specific battle behavior. | **TRACE_ONLY** |
| `CBattleState virtual #11` | `CBattleState_UnkVirtualFunc11__Q22cf12CBattleStateFv` | `0x80148FC8` | `0x174` | Mask-clear entries + dup-id scan + unk15AC bit; Chaitin soft-cap (dead trip r3 / found r0). Host `battlestate-vfunc11` PASS. | **CODE_MATCH ~96.2%** |
| `CBattleState virtual #26` | `CBattleState_UnkVirtualFunc26__Q22cf12CBattleStateFv` | `0x80148364` | `0x12C` | Candidate action/target helper. | **TRACE_ONLY** |
| `CBattleState virtual #29` | `CBattleState_UnkVirtualFunc29__Q22cf12CBattleStateFv` | `0x80148490` | `0x15C` | Clear 8 entries @+0x1388 + 104-entry id scan + unk15AC bit clear. Natural linear loop recovers retail's 13×8 MWCC unroll. Host `battlestate-vfunc29` PASS. | **FULL_MATCH** |
| `CBattleState virtual #31` | `CBattleState_UnkVirtualFunc31__Q22cf12CBattleStateFv` | `0x80149EA4` | `0x160` | id→bitmask lookup; `return (this->unk4 & mask) != 0`. | **FULL_MATCH** |
| `CBattleState virtual #33` | `CBattleState_UnkVirtualFunc33__Q22cf12CBattleStateFv` | `0x8014A014` | `0x160` | Leaf id→mask vs `unk6` (+0x6); same shape as vfunc31. | **FULL_MATCH** |


**Do not decompile these blindly in numeric order.** Trace the player-controlled character while:

1. entering battle;
2. moving the Art cursor;
3. changing target;
4. confirming an unavailable Art;
5. confirming an available Art;
6. beginning and finishing an Art;
7. being interrupted, knocked down, or dying.

Prioritize only virtuals observed in those traces.

### 7.3 `CAIAction.cpp`

The goal is to split **decision ownership** from the existing **action executor**. P2 should replace the AI's chosen action while preserving native validation, animation, effects, cooldown, damage, and aggro.

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CAIAction::CAIAction()` | `__ct__Q22cf9CAIActionFv` | `0x8014B308` | `0x10C` | Recovers fields and native action state defaults. | **FULL_MATCH** |
| `CAIAction virtual #1` | `CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv` | `0x8014B41C` | `0x110` | Fv+outA/outB ring export; host `aiaction-vfunc1`. | **HIGH_MATCH ~93.2%** |

| `CAIAction virtual #2` | `CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv` | `0x8014B52C` | `0x110` | Fv+inA/inB inverse ring import; host `aiaction-vfunc2`. | **CODE_MATCH ~96.0%** |


**Mandatory convergence trace:** compare the call path when:

- Player 1 confirms an Art through the HUD/control state.
- An AI companion independently chooses and executes an Art.

Find the earliest common function that accepts an actor/action/Art/target request, or the lowest command structure written by both paths. That is the preferred P2 injection point.

### 7.4 Arts structures

**Questions:**

- Which `CArtsParam` fields are Art ID, level, recharge, max recharge, usability flags, and conditions?
- Where is the eight-slot equipped palette stored or mapped?
- Is the selected cursor slot actor-owned, battle-state-owned, or UI-owned?
- Which state distinguishes a Talent Art and multi-stage action command?
- Which writes occur on acceptance versus rejection?

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CArtsSet virtual #1` | `CArtsSet_UnkVirtualFunc1__Q22cf8CArtsSetFv` | `0x80154740` | `0x9C` | Only named CArtsSet behavior seed; classify initialization/update role. | **STRUCTURAL** |


**Required experiments:** equip distinct Arts on each character, snapshot all candidate `CArtsSet` blocks, move only the cursor, use one Art, observe recharge, and compare successful/failed positional conditions.

### 7.5 Targeting and selector paths

Start from:

```text
kyoshin/cf/CfObjectSelectorObj.cpp
kyoshin/cf/CfObjectSelector.cpp or adjacent selector translation units in splits.txt
kyoshin/menu/CMenuEnemyState.cpp
CBattleState call graph
```

Recover:

- current target pointer/ID;
- candidate target list construction;
- next/previous target input;
- range and line-of-sight validation;
- world target marker rendering;
- HUD enemy panel lookup;
- any global singleton that assumes only one player target.

A P2 target must be authoritative gameplay state updated once; the view-specific marker is presentation state rendered per viewport.

---

## 8. P1 native HUD targets

The first accepted HUD milestone is not a portrait swap. It must display the assigned actor's real equipped Arts, recharge, Talent state, cursor, target, and action prompts.

### 8.1 `kyoshin/menu/CMenuArtsSelect.cpp`

**Logical translation unit range (US):** `0x80102B08-0x80109740`.

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CMenuArtsSelect::CMenuArtsSelect()` | `__ct__CMenuArtsSelect` | `0x80102B08` | `0x288` | Recovers widget/state fields and default bindings. | **STRUCTURAL** |
| `CMenuArtsSelect::Init()` | `Init__15CMenuArtsSelectFv` | `0x80102EC4` | `0xC5C` | Widget creation, resource lookup, actor/Arts references. | **STRUCTURAL** |
| `CMenuArtsSelect::Term()` | `Term__15CMenuArtsSelectFv` | `0x80103B20` | `0x248` | Safe duplicated/scoped HUD teardown. | **STRUCTURAL** |
| `CMenuArtsSelect::Move()` | `Move__15CMenuArtsSelectFv` | `0x80103D68` | `0xBB4` | Cursor input, selected Art, prompt state, and UI update. | **STRUCTURAL** |
| `CMenuArtsSelect::cbRenderBefore()` | `cbRenderBefore__15CMenuArtsSelectFv` | `0x8010491C` | `0x3C0` | Final render-time actor/Art data and transform binding. | **STRUCTURAL** |


**Required call-site labels inside `Move()` and `cbRenderBefore()`:**

```text
HUD_ACTOR_LOOKUP
ARTS_SET_LOOKUP
EQUIPPED_SLOT_MAPPING
CURSOR_STATE_READ
TARGET_STATE_READ
PAD_LOOKUP
ACTION_COMMAND_PROMPT_READ
WIDGET_UPDATE
PURE_RENDER_TRANSFORM
GAMEPLAY_COMMAND_SUBMISSION
```

The easiest implementation is a scoped HUD context only if runtime evidence shows the routine's gameplay-command side effects are separated from final rendering.

### 8.2 `kyoshin/menu/CMenuBattlePlayerState.cpp`

**Logical translation unit range (US):** `0x8010B880-0x8010F540`.

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CMenuBattlePlayerState::CMenuBattlePlayerState()` | `__ct__CMenuBattlePlayerState` | `0x8010B880` | `0x580` | Recovers native HP/portrait/status widget graph. | **STRUCTURAL** |
| `CMenuBattlePlayerState::Init()` | `Init__22CMenuBattlePlayerStateFv` | `0x8010C000` | `0xC5C` | Finds party actor and layout-resource bindings. | **STRUCTURAL** |
| `CMenuBattlePlayerState::Term()` | `Term__22CMenuBattlePlayerStateFv` | `0x8010CC5C` | `0x1B0` | Needed if HUD instances are duplicated. | **FULL_MATCH** |
| `CMenuBattlePlayerState::Move()` | `Move__22CMenuBattlePlayerStateFv` | `0x8010CE0C` | `0x8E8` | Updates HP, tension, statuses, portrait, and actor state. | **CODE_MATCH ~97.7%** |
| `CMenuBattlePlayerState::cbRenderBefore()` | `cbRenderBefore__22CMenuBattlePlayerStateFv` | `0x8010D6F4` | `0x1B4` | Final per-view transforms and values. | **FULL_MATCH** |


### 8.3 Other battle-HUD units

```text
kyoshin/menu/CMenuEnemyState.cpp
kyoshin/menu/CMenuBattleDamage.cpp
kyoshin/menu/CMenuPTGauge.cpp
kyoshin/menu/CMenuBattleMode.cpp
kyoshin/CUIBattleManager.cpp
```

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CMenuEnemyState::Move()` | `Move__15CMenuEnemyStateFv` | `0x80110888` | `0x9B8` | P2 target panel and target-specific status. | **STRUCTURAL** |
| `CMenuEnemyState::cbRenderBefore()` | `cbRenderBefore__15CMenuEnemyStateFv` | `0x80111240` | `0x274` | Per-view enemy-panel rendering. | **STRUCTURAL** |
| `CMenuPTGauge::Move()` | `Move__12CMenuPTGaugeFv` | `0x80188714` | `0x35C` | Shared party gauge; classify as shared or duplicated. | **FULL_MATCH** |
| `CMenuPTGauge::cbRenderBefore()` | `cbRenderBefore__12CMenuPTGaugeFv` | `0x80188A70` | `0xAC` | Final shared gauge placement. | **FULL_MATCH** |
| `CUIBattleManager::Move()` | `Move__16CUIBattleManagerFv` | `0x8012F270` | `0xB00` | Owns/coordinates battle UI objects and modes. | **STRUCTURAL** |
| `CUIBattleManager::Init()` | `Init__16CUIBattleManagerFv` | `0x8012EEFC` | `0xB4` | Finds construction/lifetime of battle HUD components. | **STRUCTURAL** |


**Shared-versus-local classification:**

| HUD element | Initial policy |
|---|---|
| Actor portrait/HP/status | Per player |
| Arts palette/recharge/Talent | Per player |
| Art cursor | Per player |
| Target panel/marker | Per player |
| Blossom Dance/action prompt | Per actor/player |
| Party gauge | Shared, drawn in both halves or once above divider |
| Party-member sidebar | Shared initially |
| Battle start/end messages | Shared |
| Damage numbers | World-space, naturally visible in each view |
| Visions/Chain Attack UI | Full-screen shared initially |

### 8.4 Blossom Dance and action-command prompt path

Trace Player 1 controlling Dunban through all four attack stages. On every prompt:

- log the prompt owner actor;
- log the input accessor and B-button flags;
- log timing-window start/end values;
- log success/miss writes;
- log the function advancing the active action stage;
- log the UI routine drawing the prompt.

Preferred patch order:

1. Make the prompt input accessor return the pad assigned to the active actor.
2. Keep native timing validation and action-state mutation.
3. Render the prompt only in the HUD context for that actor.
4. Ensure prompt simulation updates once, not once per viewport.

---

## 9. P1 camera and culling targets

### 9.1 Camera translation units and headers

```text
kyoshin/cf/CfCam.cpp
kyoshin/cf/CfCamEvent_1.cpp
src/kyoshin/cf/camera/*
scene camera manager paths containing CScnCameraMan / CScnItemCamera
```

**US translation-unit ranges:**

```text
kyoshin/cf/CfCam.cpp        0x8006C0F8-0x800758E8
kyoshin/cf/CfCamEvent_1.cpp 0x800758E8-0x8007CA94
```

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CfCamEvent::initCamIntfInstances()` | `CfCamEvent_initCamIntfInstances` | `0x8006B3B8` | `0x98` | Maps camera interface ownership and event state. | **FULL_MATCH** |
| `CfCamDirectionIntf::setPos()` | `CfCamDirectionIntf_setPos` | `0x8006BE48` | `0x28` | Direct camera-position write. | **FULL_MATCH** |
| `CfCamDirectionIntf::setDir()` | `CfCamDirectionIntf_setDir` | `0x8006BE70` | `0x74` | Direct direction write. | **FULL_MATCH** |
| `CfCamDirectionIntf::setLookat()` | `CfCamDirectionIntf_setLookat` | `0x8006BEE4` | `0x7C` | Direct look-at write. | **FULL_MATCH** |
| `CfCamTargetIntf::setPos()` | `CfCamTargetIntf_setPos` | `0x8006C030` | `0x30` | Target-style camera position. | **FULL_MATCH** |
| `CfCamTargetIntf::setDir()` | `CfCamTargetIntf_setDir` | `0x8006C060` | `0x74` | Target-style camera direction. | **FULL_MATCH** |
| `CfCamTargetIntf::setLookat()` | `CfCamTargetIntf_setLookat` | `0x8006C0D4` | `0x24` | Target-style look-at target. | **FULL_MATCH** |
| `CfCamFollow::CfCamFollow()` | `__ct__cf_CfCamFollow` | `0x8006C684` | `0x238` | Recovers follow subject, offsets, smoothing, and collision state fields. | **FULL_MATCH** |


**Discovery targets inside the camera call graph:**

- function that chooses the follow actor;
- function that reads the camera controller/pad;
- final calculated camera matrix/vector write;
- camera collision and smoothing update;
- scripted-camera/event override flag;
- battle camera target/lock-on adjustment;
- camera state that can be cloned or recomputed for P2 without advancing simulation twice.

### 9.2 `src/kyoshin/COccCulling.cpp` and `src/kyoshin/cf/CTaskCulling.cpp`

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `COccCulling::addFrustum(...)` | `addFrustum__11COccCullingFRCQ22ml5CVec3RCQ22ml5CVec3RCQ22ml5CVec3Ul` | `0x801A1F6C` | `0x12C` | Builds/records frustum candidates. | **FULL_MATCH** |
| `COccCulling::setFrustum(CCullFrustum*)` | `setFrustum__11COccCullingFP12CCullFrustum` | `0x801A2098` | `0x588` | Primary per-view culling-state setup. | **STRUCTURAL** |
| `COccCulling frustum helper` | `func_801A0F04__11COccCullingFPQ22ml8CFrustum` | `0x801A2620` | `0x284` | Classify visibility-list generation and fixed capacities. | **STRUCTURAL** |
| `COccCulling cull helper` | `func_801A1188__11COccCullingFP12CCullFrustum` | `0x801A28A4` | `0x2BC` | Classify object list writes and per-view reuse. | **STRUCTURAL** |
| `CTaskCulling virtual frustum setter` | `ICulling_UnkVirtualFunc1__Q22cf12CTaskCullingFPQ22ml8CFrustum` | `0x801A4340` | `0x20` | Connects scene camera/frustum to culling task. | **FULL_MATCH** |
| `CTaskCulling helper` | `func_801A2C04__Q22cf12CTaskCullingFv` | `0x801A4324` | `0x1C` | Potential reset/prepare operation between views. | **CODE_MATCH** |


**Acceptance:** view 0 and view 1 rebuild or switch culling state sequentially. Geometry visible only to P2 appears in view 1. No culling list from view 1 leaks into the next frame's view 0.

---

## 10. P2 presentation-state and full-screen gating targets

### 10.1 UI/window managers

```text
src/kyoshin/CUICfManager.hpp
src/kyoshin/CUIWindowManager.hpp
logical kyoshin/CUICfManager.cpp
logical kyoshin/CUIWindowManager.cpp
src/kyoshin/CGame.cpp
src/kyoshin/cf/CTaskREvent.hpp
src/kyoshin/cf/IFlagEvent.hpp
```

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `CUICfManager::Move()` | `Move__12CUICfManagerFv` | `0x801332A4` | `0x97C` | Central UI state and event/menu activity. | **HIGH_MATCH ~94.3%** |
| `CUICfManager helper` | `func_80133324__12CUICfManagerFv` | `0x80133DF8` | `0x3C0` | Event id range dispatch (Fv+r4/r5/r6); SDA/list insert. | **CODE_MATCH ~98.2%** |
| `CUIWindowManager::Move()` | `Move__16CUIWindowManagerFv` | `0x8013D0C8` | `0x4DC` | Modal window state, input capture, and open/close transitions. | **HIGH_MATCH** (~79.3%, host-verified) |
| `CUICfManager::Init()` | `Init__12CUICfManagerFv` | `0x80132EC8` | `0x2E0` | Finds manager-owned menu/window components. | **STRUCTURAL** |
| `CUICfManager::Term()` | `Term__12CUICfManagerFv` | `0x80133200` | `0xA4` | Safe co-op toggle and shutdown. | **FULL_MATCH** |


### 10.2 Event/cutscene/loading paths to discover

Start from these known or likely units:

```text
kyoshin/cf/CTaskREvent.*
kyoshin/realtimeevt/*
kyoshin/CTalkWindow.cpp
kyoshin/CMainMenu.cpp
kyoshin/CSystemWindow.cpp
kyoshin/CQuestWindow.cpp
kyoshin/menu/CMenuFade.cpp
kyoshin/cf/CVision.cpp
loading/map-transition tasks discovered from CGame and CTaskGame/Cf call graphs
```

Recover predicates for:

```text
NORMAL_FIELD
NORMAL_BATTLE
MODAL_MENU
DIALOGUE
IN_ENGINE_CUTSCENE
PRE_RENDERED_MOVIE
LOADING
MAP_TRANSITION
SCRIPTED_CAMERA
VISION
CHAIN_ATTACK
PAUSE_OR_HOME_MENU
```

Prefer a small set of authoritative manager flags over checking widget visibility. The final `ShouldRenderSplitScreen()` must be deterministic and must restore every view/camera/HUD/culling/GX state before full-screen rendering.

---

## 11. P2 co-op settings targets

### Prototype path — no native menu decompilation required

Use a custom Dolphin/mod configuration and a runtime Boolean. This is sufficient through early milestones.

### Native settings path — later recovery

Start from:

```text
kyoshin/CMainMenu.cpp
kyoshin/CSystemWindow.cpp
kyoshin/CSysWinSelect.cpp
kyoshin/menu/CMenuKeyAssign.cpp
CNandData.cpp / system-options serialization paths
message/string tables and NW4R layout resources used by the options screen
```

Recover:

- option list/table construction;
- row count and cursor movement;
- value-change callback;
- defaults reset;
- save/load serialization;
- safe extension space or versioning strategy;
- label/message IDs;
- layout row capacity/scroll behavior.

Do not modify the original save structure until the serialization size, checksums, versioning, and backward compatibility are understood. A separate co-op settings file is the safer initial release.

---

## 12. P3 expanded memory targets

### 12.1 Dolphin side

External code paths, not part of Xenoblade decompilation:

```text
Dolphin Source/Core/Core/Config/MainSettings.cpp
Dolphin Source/Core/Core/HW/Memmap.cpp
Dolphin save-state memory serialization paths
```

### 12.2 Xenoblade/monolib memory paths

```text
libs/monolib memory manager paths containing mtl::MemManager
libs/monolib/src/work/CWorkSystemMem.cpp
libs/monolib/src/device/CDeviceGX.cpp
root work/task initialization paths that create MEM1/MEM2 child handles
```

| Function | Symbol-map name | US address | Size | Why it matters | Required recovery |
|---|---|---:|---:|---|---|
| `mtl::MemManager::getHandleMEM2()` | `getHandleMEM2__Q23mtl10MemManagerFv` | `0x80436D38` | `0x8` | Finds original MEM2 root handle and arena assumptions. | **FULL_MATCH** |
| `CWorkSystemMem::CWorkSystemMem(...)` | `__ct__14CWorkSystemMemFPCcP11CWorkThread` | `0x80447250` | `0x60` | Maps system-memory work object and child handles. | **FULL_MATCH** |
| `CWorkSystemMem::getHandle()` | `getHandle__14CWorkSystemMemFv` | `0x80447308` | `0xC` | Known system handle accessor. | **FULL_MATCH** |
| `CWorkSystemMem::wkStandbyLogout()` | `wkStandbyLogout__14CWorkSystemMemFv` | `0x80447318` | `0x98` | Memory shutdown/lifetime and leak checks. | **STRUCTURAL** |
| `CDeviceGX::getHeapSize()` | `getHeapSize__9CDeviceGXFv` | `0x80459890` | `0x8` | Graphics heap budget and fixed-size assumptions. | **FULL_MATCH** |


**Recover or trace:**

- root MEM1/MEM2 arena bounds;
- whether arena end comes from OS low memory, IOS, hardcoded constants, or linker symbols;
- child heap creation and ownership;
- cache flush/invalidate requirements;
- address-range checks;
- GPU/audio/DMA restrictions;
- save-state coverage of expanded memory;
- fixed pools that fail before raw memory is exhausted.

**First accepted design:** leave all original heaps unchanged and initialize a dedicated co-op allocator in the added MEM2 range. Allocate only CPU-side debug/co-op structures first.

---

## 13. P3 streaming, activation, and draw-distance discovery map

This subsystem is less reconstructed than input/HUD/rendering. Agents must create the exact target list from runtime traces rather than guessing class names.

### 13.1 Seed paths

Begin with call graphs and tasks reachable from:

```text
CTaskGame.cpp
CTaskGameCf.cpp
CfGameManager.cpp
scene subsystem CScn/CScnItemPool
CfObjectSelectorObj.cpp
map/field/background tasks listed near the active area's code in splits.txt
CBgTex.cpp
object creation/destruction managers
file/archive async loading tasks
```

### 13.2 Required functions to identify

The final map must name and address functions for:

1. current map/area ID;
2. player/leader position used as streaming anchor;
3. terrain section request/load/unload;
4. collision section request/load/unload;
5. NPC/enemy spawn and despawn;
6. AI activation/sleep distance;
7. model/vegetation LOD selection;
8. object render-distance culling;
9. map transition trigger and destination load;
10. skip travel/teleport party regroup;
11. fixed pool allocation and capacity checks;
12. asynchronous load completion callbacks.

### 13.3 Trace procedure

Run one player and camera stationary. Move only the P2 actor away through a debug position write or early control prototype. Log:

- file/archive requests;
- map-section IDs;
- object creation/destruction;
- collision residency;
- enemy/NPC state changes;
- culling-only changes versus simulation activation changes;
- allocator failures and pool high-water marks.

Then repeat with only the camera moved. This separates actor-driven streaming from camera-driven rendering/culling.

### 13.4 Required end state

Replace a single authoritative anchor with either:

```text
union of two player-centered activation/streaming regions
```

or, if the engine only supports one region:

```text
midpoint + radius covering both actors + safety margin
```

Both players must remain in the same map instance for the first release. A tested tether remains mandatory until two-anchor streaming is stable.

---

## 14. Render-side libraries that may need only inspection, not game-specific reconstruction

### NW4R G3D

```text
libs/nw4r/src/g3d/g3d_workmem.cpp
scene graph draw and camera-setting paths reached by CScnRootNw4r
```

Recover enough to prove:

- temporary work memory is global;
- view 0 and view 1 must render sequentially;
- which matrices and global flags must be reset between views;
- whether particle/effect update is separate from draw;
- whether post-processing reads/writes a full-screen EFB assumption.

### NW4R Layout

```text
libs/nw4r/src/lyt/lyt_drawInfo.cpp
layout animation/update paths called by CGame/UI managers
layout draw paths used by battle HUD classes
```

Recover enough to separate:

- layout animation advancement — once per frame;
- widget model/value update — once per player or once shared, depending on state ownership;
- layout render transforms — once per viewport;
- scissor/viewport handling for half-screen HUDs.

Do not spend time matching unrelated font, archive, or widget helpers unless they block the target paths.

---

## 15. Required call-path dossiers

The decompilation effort is complete enough for implementation only when these end-to-end paths are documented.

### 15.1 Frame path

```text
main/game loop
 -> CGame::wkUpdate
 -> task/process Move traversal
 -> CGame::wkRender
 -> layout update
 -> view/scene render traversal
 -> device EFB copy
 -> VI present
```

### 15.2 Player 2 input path

```text
WPAD/KPAD/Classic Controller channel 1
 -> CPad/CtrlRemote
 -> CfPadTask conversion
 -> P2 CfPadData
 -> actor ownership router
```

### 15.3 P2 Art execution path

```text
P2 controller
 -> P2 cursor and target state
 -> equipped Art resolver
 -> native command request/queue
 -> actor action runner
 -> animation/effect/damage/cooldown
```

### 15.4 P2 native HUD path

```text
view context 1
 -> HUD actor lookup
 -> actor Arts/HP/status
 -> P2 cursor and target
 -> native widget update
 -> half-screen layout render
```

### 15.5 Multi-stage Art path

```text
Dunban active Blossom Dance action
 -> prompt state opens
 -> actor-owner pad lookup
 -> P2 B trigger
 -> native timing validation
 -> action stage advance or miss
 -> prompt render in P2 HUD
```

### 15.6 Camera/culling path

```text
P2 actor
 -> camera follow subject/offset/smoothing
 -> final camera vectors/matrix
 -> frustum
 -> visibility list
 -> scene render in viewport 1
```

### 15.7 Split/full-screen path

```text
UI/event/loading state
 -> presentation-mode predicate
 -> save/restore split render globals
 -> two views OR one original full-screen view
```

### 15.8 Streaming path

```text
P1/P2 positions
 -> streaming envelope
 -> section requests
 -> collision and object residency
 -> NPC/enemy activation
```

---

## 16. Agent ownership and dependency rules

| Workstream | Primary paths | Must hand off |
|---|---|---|
| Frame/render | `CGame`, `CView`, `CProcessMan`, `CScn`, `CDeviceGX` | safe per-view render function; once-per-frame list; state snapshot list |
| Input/gameplay | `CfPadTask`, `CfGameManager`, `CBattleState`, `CAIAction` | P2 pad; actor owner map; native command hook |
| HUD | `CMenuArtsSelect`, `CMenuBattlePlayerState`, `CMenuEnemyState`, `CUIBattleManager` | scoped HUD context; local/shared element map; prompt path |
| Camera/culling | `CfCam*`, `COccCulling`, `CTaskCulling`, scene camera manager | per-player camera state; culling reset/rebuild contract |
| Presentation gating | UI/window/event/menu/loading paths | authoritative mode predicate and transition hooks |
| Memory/streaming | memory manager, work system memory, map/object/load managers | co-op heap; pool audit; two-player envelope |
| QA | all | mutation traces, deterministic test scripts, performance baselines |

No two agents may independently rename an unknown field or function in shared headers without an evidence-ledger update and lead-agent approval.

---

## 17. Function dossier template

```markdown
# Function dossier

## Identity
- Region:
- Executable SHA-1:
- Repository commit:
- Translation unit:
- Symbol-map name:
- Address and size:
- Current recovery level:

## Role hypothesis
- One sentence:
- Confidence: CANDIDATE / STATIC / RUNTIME / BEHAVIORAL / MATCHING

## Call graph
- Callers:
- Callees:
- Virtual table slot, if any:

## ABI and types
- this pointer:
- integer arguments:
- float arguments:
- return value:
- structure offsets read:
- structure offsets written:

## Side effects
- gameplay state:
- render state:
- allocations:
- audio/events:
- RNG:
- file/loading:

## Controlled experiments
- Setup:
- Trigger:
- Expected:
- Observed:

## Matching loop
- Candidate source:
- Compile result:
- Instruction match:
- Relocation match:
- Stack size:
- Remaining mismatch categories:

## Split-screen decision
- Run once per frame / once per view / shared full-screen only:
- Safe hook point:
- Required save/restore state:

## Evidence links
- trace:
- objdiff capture:
- screenshots/video:
- memory snapshots:
```

---

## 18. Machine-readable target manifest format

Maintain `docs/coop_decomp_targets.yaml`:

```yaml
targets:
  - id: RENDER-CVIEW-001
    priority: P0
    subsystem: render
    path: logical:CView.cpp
    symbol: renderView__5CViewFv
    region: US
    address: 0x804415C4
    size: 0xCB4
    required_level: STRUCTURAL
    status: UNSTARTED
    owner: agent-render
    depends_on:
      - RENDER-CVIEW-SETCURRENT
      - RENDER-CSCN-DRAW
    questions:
      - Can this execute twice without advancing state?
      - What global render state must be restored?
    evidence: []
```

Allowed statuses:

```text
UNSTARTED
TRACING
TYPED
COMPILES
STRUCTURAL
HIGH_MATCH
CODE_MATCH
FULL_MATCH
RUNTIME_CONFIRMED
BLOCKED
NOT_REQUIRED
```

---

## 19. Recommended recovery order

### Phase A — render proof

1. `CGame::wkUpdate()` and `wkRender()`.
2. `CTaskManager::Draw()` and `CProcessMan::Draw/DrawImpl/TailImpl`.
3. `CView::setCurrent/setRect/renderView`.
4. `CViewRoot::setCurrent/renderView/getFullScreenView`.
5. `CScn::Draw()` and render callbacks.
6. `CDeviceGX` begin/copy/present path.
7. Same camera rendered into two rectangles.

### Phase B — unsplit two-player gameplay

1. `CfPadTask` second channel.
2. `CfGameManager` party actor and pad caller audit.
3. Companion AI decision boundary.
4. P2 movement.
5. P2 target state.
6. P2 Art selection and native command submission.
7. Blossom Dance follow-up pad routing.

### Phase C — separate presentation

1. Independent camera subject.
2. Per-view culling.
3. Arts HUD scoped actor/pad/target context.
4. HP/status and enemy target HUD.
5. Half-screen layout transforms.

### Phase D — full-screen shared modes

1. Modal menu detector.
2. Cutscene/dialogue/event detector.
3. Loading/map transition detector.
4. Vision and Chain Attack modes.
5. State restoration and transitions.

### Phase E — range and memory

1. Streaming anchor trace.
2. Object/AI activation distance trace.
3. Fixed pool audit.
4. Dedicated expanded-MEM2 allocator.
5. Two-player streaming envelope.
6. Draw-distance increases and performance tuning.

---

## 20. Minimum decompilation needed before each implementation gate

| Implementation gate | Minimum recovered targets |
|---|---|
| Two controllers logged | `CfPadTask::update`, `updateCfPadData`, channel lookup |
| P2 executes an Art | party actor resolver, equipped Art mapping, target state, native command/AI action injection |
| Same-camera split | frame boundary, view rectangle/current view, scene-only render, frame finalization |
| Different cameras | camera subject, final camera state, per-view culling setup |
| Native P2 Arts HUD | `CMenuArtsSelect` actor/Arts/cursor/target/pad lookups and update/render separation |
| Blossom Dance works | action-prompt owner, pad lookup, timing validation, stage advance, prompt render |
| Menus/cutscenes full-screen | reliable modal/event/loading predicates and full render-state restore |
| Wider separation | streaming anchor, load/unload, collision residency, activation, pool limits |
| Expanded memory used | original arena/heap bounds, dedicated co-op heap, save-state validation |

---

## 21. Explicit non-targets until a blocker appears

Do not broadly decompile these areas merely because they are nearby:

- unrelated title-screen/help UI;
- crafting/gem systems;
- unrelated quests and affinity menus;
- sound archive internals beyond duplicated-event bugs;
- save data beyond co-op setting persistence and expanded-memory save states;
- renderer model/material functions that do not block a second view;
- every AI behavior; only the control-decision/action-execution boundary is required;
- every map; recover generic streaming logic and test representative areas.

---

## 22. Exit criteria for the decompilation workstream

The feature-driven decompilation is complete when agents can point to verified functions and fields for all of the following:

- one authoritative update entry;
- one repeatable scene-render entry;
- one once-only frame finalization entry;
- per-view viewport/scissor/projection state;
- per-view camera state and culling;
- two stable controller snapshots;
- party actor ownership;
- independent target and Art selection;
- native Art command submission;
- actor-specific action prompts;
- native HUD actor/pad/target binding;
- menu/cutscene/loading presentation predicates;
- streaming anchor and activation/load paths;
- optional expanded-memory heap initialization.

The project does **not** need a full-game matching percentage target to ship the mod. It needs a complete, evidence-backed connected slice for these behaviors.

---

## 23. Primary repository references

- Public decompilation root: `https://github.com/xbret/xenoblade`
- Build, hashes, and contribution policy: `README.md`
- Region maps: `config/<region>/symbols.txt`, `config/<region>/splits.txt`
- Top-level game: `src/kyoshin/CGame.cpp`
- Input: `src/kyoshin/cf/CfPadTask.cpp`
- Party manager: `src/kyoshin/cf/CfGameManager.hpp`
- Actor layouts: `src/kyoshin/cf/object/`
- Arts: `src/kyoshin/cf/CArtsSet.*`, `CArtsParam.cpp`
- Battle HUD logical units: `kyoshin/menu/CMenuArtsSelect.cpp`, `CMenuBattlePlayerState.cpp`, `CMenuEnemyState.cpp`
- Battle UI manager: `kyoshin/CUIBattleManager.cpp`
- Camera: `kyoshin/cf/CfCam.cpp`, `CfCamEvent_1.cpp`, `src/kyoshin/cf/camera/`
- Culling: `src/kyoshin/COccCulling.cpp`, `src/kyoshin/cf/CTaskCulling.cpp`
- Process traversal: `libs/monolib/src/work/CProcess.cpp`
- Process/view creation: `libs/monolib/src/core/CProc.cpp`
- Global render work memory: `libs/nw4r/src/g3d/g3d_workmem.cpp`
- Layout draw context: `libs/nw4r/src/lyt/lyt_drawInfo.cpp`

Because the repository changes, agents must update this map when source files are added, names improve, or symbol ranges move.
