# Xenoblade Chronicles Wii single-instance split-screen: project plan

**Purpose:** direct a coordinated set of reverse-engineering and implementation agents to add local two-player, single-process split-screen co-op to the Wii version of *Xenoblade Chronicles* running in Dolphin.

**Primary design:** one authoritative Xenoblade simulation updates once per frame; two controllers operate two party actors; normal gameplay is rendered sequentially through two camera/HUD contexts into two viewports; menus, cutscenes, loading screens, scripted cameras, visions, and initially Chain Attacks use the original full-screen presentation.

**Source snapshot date:** 2026-07-12. Re-check all symbols, layouts, addresses, repository policies, and Dolphin configuration keys against the exact commits used by the project.

**Companion documents:**

- `DECOMP_MAP.md` — decompilation targets, symbols, and critical-path functions
- `Xenoblade_Wii_Coop_Decompilation_Guide.md` — deeper per-function decompilation procedures (when present)

---

## 1. Mission and success definition

Build a downstream mod and, only where necessary, a custom Dolphin fork that provides:

1. A co-op enable/disable setting.
2. Two local controller inputs in one Dolphin process.
3. Player 1 controls party actor 0; Player 2 controls a configurable party actor, initially slot 1.
4. Both players can move, target, select Arts, execute Arts, and use actor-specific follow-up/action-command inputs.
5. Normal exploration and ordinary battles render in split-screen.
6. Each half has the native camera following its assigned actor.
7. Each half has a native HUD showing its assigned actor's HP, portrait, equipped Arts, recharge, Talent state, cursor, target, prompts, and relevant status.
8. Menus, cutscenes, dialogue, loading screens, scripted cameras, visions, and initially Chain Attacks render once at full screen.
9. The simulation, AI, RNG, quests, battles, saving, world state, and audio update once.
10. Optional expanded MEM2 is used through a dedicated co-op heap, without silently changing the original game's allocation order.
11. Nearby independent exploration is supported through a deterministic two-player streaming/activation envelope and a tested tether.
12. The implementation boots and remains stable in Dolphin across repeatable test scenarios and long-duration play.

### First shippable scope

The first stable release is allowed to impose these restrictions:

- exactly two human players;
- both players remain in the same loaded map instance;
- map transitions initiated by either player move both players;
- Player 1 owns modal menus and dialogue choices;
- cutscenes and scripted events suppress both players' field controls;
- Chain Attacks and visions use shared full-screen presentation;
- Player 2 returns to AI control when co-op is disabled or the second controller disconnects;
- a configurable tether prevents separation beyond verified streaming limits;
- vertical split is the default; horizontal split may follow later.

### Not required for the first release

- two independent maps loaded simultaneously;
- independent quest progression;
- independent save files;
- online play;
- physical Wii support;
- a perfect 100% decompilation of the game;
- a native PC port;
- concurrent or multithreaded rendering of the two views.

---

## 2. Legal, repository, and contribution boundaries

Use a legally obtained game extraction. The public decompilation requires the user's own `main.dol` and REL files and does not distribute original game assets or assembly.

Do not commit or distribute:

- disc images;
- `main.dol` or REL binaries;
- extracted proprietary assets;
- complete rebuilt proprietary executables;
- large binary excerpts that reconstruct copyrighted code.

Distribute only original tooling, source modifications, documentation, symbol notes, and legally appropriate patch/delta data.

The public `xbret/xenoblade` repository currently states that LLM-assisted contributions are not accepted upstream. All agent-generated work must remain in a clearly labeled private or downstream fork. Do not submit AI-assisted reconstruction to the upstream repository.

Cloud agents should receive minimal function dossiers where practical: relevant disassembly, decompiler output, types, symbols, traces, and diffs. Prefer local agents for full-binary access.

---

## 3. Non-negotiable architecture invariants

Every agent must preserve these rules.

### 3.1 One simulation, two presentations

```text
Controller 1 -> PlayerState[0] -> party actor 0 --+
                                                  +-> one authoritative simulation update
Controller 2 -> PlayerState[1] -> party actor 1 --+

simulation result -> camera/HUD context 0 -> viewport 0
                  -> camera/HUD context 1 -> viewport 1
```

Gameplay updates once. Presentation renders twice.

### 3.2 Never call the complete frame update twice

Do not call any of these twice merely to obtain a second view:

- actor/AI update;
- battle update;
- task movement/update traversal;
- animation time advancement;
- particle simulation;
- UI state update or animation advancement;
- audio event generation;
- RNG-consuming logic;
- streaming decisions;
- save/quest/event logic.

### 3.3 Do not call the current top-level render path twice until proven safe

The recovered `CGame::wkRender()` advances a layout animation and calls the task draw path. The recovered process draw manager performs draw traversal and then tail traversal. Calling the entire path twice risks:

- UI animation advancing twice;
- frame-finalization or tail callbacks running twice;
- effect queues being flushed twice;
- cleanup occurring between views;
- audio or event side effects;
- global renderer scratch being reused incorrectly.

The render agent must identify a lower, scene-only and HUD-render-only boundary.

### 3.4 Sequential rendering only

NW4R G3D uses global temporary work memory. Treat the renderer as non-reentrant.

```text
render view 0 completely
restore/prepare global render state
render view 1 completely
finalize/present once
```

Do not attempt concurrent render threads.

### 3.5 Player identity is not presentation identity

The global game leader/current player must remain authoritative and stable unless gameplay code explicitly requires otherwise.

Do not implement split-screen by globally changing the current-player actor around the whole frame.

Allowed scoped differences:

- camera subject;
- view rectangle and projection;
- culling frustum;
- HUD subject;
- HUD cursor and displayed target;
- visible action prompt;
- local screen-space effects;
- optional local audio emphasis.

Disallowed local-only differences:

- whether an Art executes;
- actor movement;
- target ownership in authoritative battle state;
- AI ownership;
- damage, cooldowns, buffs, aggro;
- RNG;
- quest or event state;
- loaded/active world objects.

### 3.6 Action prompts are gameplay, not decoration

Blossom Dance-style follow-up presses must update the actor's native action state once during simulation. Rendering the prompt can differ by viewport, but accepting the input cannot happen only inside a HUD draw pass.

### 3.7 Extra memory is opt-in and isolated

Dolphin may provide expanded MEM2, but the original Xenoblade heaps must remain unchanged initially. New co-op allocations go to a dedicated expanded-memory heap until the original allocator and every affected pool are understood.

---

## 4. Ground truth from the current public code

Agents must revalidate these facts against the exact checkout.

### 4.1 Build and region hashes

The public project identifies these `main.dol` SHA-1 values:

```text
JP  a564033aee46988743d8f5e6fdc50a8c65791160
EU  10d34dbf901e5d6547718176303a6073ee80dda2
US  214b15173fa3bad23a067476d58d3933ad7037b7
```

Never apply a hardcoded address to an unknown hash.

### 4.2 Update and render are separated at the top level

Recovered `CGame` code has separate `wkUpdate()` and `wkRender()` paths. `wkUpdate()` advances the task manager. `wkRender()` advances a layout animation and dispatches task drawing. `CGame` also owns a `CView*`, creates it during login, and can update its rectangle through `setViewRect()`.

This supports the overall approach, but it does not identify the safe lower render boundary.

### 4.3 Processes can attach to views

Recovered `CProc` code stores a list of view IDs, creates views, attaches render work to a view, detaches it, and sets a view current. This is strong evidence that the engine has a real multi-view abstraction.

The first render experiments should determine whether a second gameplay `CView` can be created and whether existing render work can safely attach to both views.

### 4.4 Draw traversal includes tail traversal

Recovered `CProcessMan::Draw()` performs all `Draw()` callbacks and then all `Tail()` callbacks. Do not invoke it twice without classifying every relevant tail side effect.

### 4.5 Renderer temporary memory is global

Recovered `nw4r::g3d::detail::workmem` contains one global temporary union used for matrices, skinning, billboard calculations, animation results, and other render scratch. This requires sequential, not concurrent, views.

### 4.6 Input currently assumes one primary controller

Recovered `CfPadTask` code selects one current controller, filters or disconnects secondary Wii Remote channels, and copies one controller into one global `CfPadData` snapshot.

The co-op patch must retain separate input snapshots and route them by actor ownership.

### 4.7 Party actors and Arts are separately addressable

Recovered `CfGameManager` declares:

```cpp
static CfObjectMove* func_80082D54(int playerIndex);
```

Current recovered layout includes three consecutive candidate party movement-subobject pointers at offset `0x94`.

Recovered actor layout:

```text
0x0000-0x337F  CActorParam
0x3380-0x3E9B  CAIAction
0x3E9C-0x45B3  CfObjectMove
```

Recovered Arts data:

```text
CActorParam::mArtsSet offset    0x19E8
CArtsSet size                   0x0D58
CArtsParam array offset         0x0038
CArtsParam count                24
CArtsParam size                 0x008C
```

Candidate conversions, requiring runtime validation:

```cpp
std::byte* actor_base =
    reinterpret_cast<std::byte*>(move) - 0x3E9C;

CAIAction* ai_action =
    reinterpret_cast<CAIAction*>(actor_base + 0x3380);

CArtsSet* arts =
    reinterpret_cast<CArtsSet*>(actor_base + 0x19E8);

CArtsSet* arts_from_move =
    reinterpret_cast<CArtsSet*>(
        reinterpret_cast<std::byte*>(move) - 0x24B4
    );
```

Do not assume entries 0-7 of the 24-entry array are the visible equipped palette.

### 4.8 Camera position and look-at are writable

Recovered camera interfaces expose position and look-at mutation. The ideal implementation may discover a direct follow-subject pointer; otherwise it can maintain separate native camera states and apply position/look-at values per view.

### 4.9 Dolphin supports guest RAM overrides

Current Dolphin configuration defines:

```text
RAMOverrideEnable
MEM1Size
MEM2Size
```

Current Dolphin memory initialization uses those settings to size MEM1 and MEM2 guest regions. MEM2 is mapped from physical address `0x10000000`.

Recommended first experiment:

```text
MEM1: retail size
MEM2: 128 MiB total
original retail MEM2: 0x10000000-0x13FFFFFF
candidate added range: 0x14000000-0x17FFFFFF
```

This does not mean Xenoblade automatically uses the range.

---

## 5. Region-specific symbol seeds

Use `config/<region>/symbols.txt` from the exact checkout as the source of truth. These are starting seeds only.

| Function | US/EU address | JP address | Size |
|---|---:|---:|---:|
| `CfGameManager::getInstance()` | `0x8007E418` | `0x8007DA7C` | `0x64` |
| `CfGameManager::func_80082D54(int)` | `0x800836D8` | `0x80082D54` | `0x3C` |
| `CfGameManager::getCurrentPad()` | `0x80087B94` | `0x800871BC` | `0x4C` |
| `CfGameManager::getCurrentPadChannel()` | `0x80087BE0` | `0x80087208` | `0x08` |
| `CfGameManager::getCfPadData()` | `0x80087C10` | `0x80087238` | `0x0C` |
| `CMenuArtsSelect::Init()` | `0x80102EC4` | `0x801023DC` | `0xC5C` |
| `CMenuArtsSelect::Move()` | `0x80103D68` | `0x80103280` | `0xBB4` |
| `CMenuArtsSelect::cbRenderBefore()` | `0x8010491C` | `0x80103E34` | `0x3C0` |
| `CMenuBattlePlayerState::Move()` | `0x8010CE0C` | `0x8010C330` | `0x8E8` |
| `CMenuEnemyState::Move()` | `0x80110888` | `0x8010FDAC` | `0x9B8` |
| `CUIBattleManager::Move()` | `0x8012F270` | `0x8012E79C` | `0xB00` |
| `CMenuPTGauge::Move()` | `0x80188714` | `0x8018728C` | `0x35C` |
| `CMenuBattleMode::Move()` | `0x801A1794` | `0x801A0078` | `0x248` |
| `CfPadTask::updateCfPadData()` | `0x801C35C8` | `0x801C1C04` | `0x364` |
| `CfPadTask::Move()` | `0x801C392C` | `0x801C1F68` | `0x194` |
| `CfPadTask::update()` | `0x801C3AC0` | `0x801C20FC` | `0x7C8` |

High-value US/EU translation-unit ranges:

| Translation unit | `.text` range |
|---|---:|
| `kyoshin/cf/CfCam.cpp` | `0x8006C0F8-0x800758E8` |
| `kyoshin/cf/CfCamEvent_1.cpp` | `0x800758E8-0x8007CA94` |
| `kyoshin/cf/CfGameManager.cpp` | `0x8007CA94-0x80087F80` |
| `kyoshin/cf/CtrlPc.cpp` | `0x80097274-0x800995BC` |
| `kyoshin/cf/CtrlRemote.cpp` | `0x800995BC-0x8009DFDC` |
| `kyoshin/menu/CMenuArtsSelect.cpp` | `0x80102B08-0x80109740` |
| `kyoshin/menu/CMenuBattlePlayerState.cpp` | `0x8010B880-0x8010F540` |
| `kyoshin/menu/CMenuEnemyState.cpp` | `0x8010F540-0x801140BC` |
| `kyoshin/CUIBattleManager.cpp` | `0x8012EA60-0x80130A48` |
| `kyoshin/cf/object/CBattleState.cpp` | `0x80146520-0x8014A184` |
| `kyoshin/cf/object/CAIAction.cpp` | `0x8014B270-0x80154630` |
| `kyoshin/cf/CArtsSet.cpp` | `0x80154630-0x80154944` |
| `kyoshin/cf/CfPadTask.cpp` | `0x801C331C-0x801C45D8` |

Before using an address:

1. verify the executable hash;
2. verify the expected first instructions or stable signature;
3. verify the function boundary in the current symbol map;
4. record evidence and region in the function report.

---

## 6. Multi-agent organization

Use one lead/integration agent and specialized agents. Agents may be separate processes or roles, but ownership must be explicit.

### Agent L: lead architect and integrator

Owns:

- project architecture and invariants;
- interface contracts;
- milestone gates;
- merge order;
- function ownership ledger;
- conflict resolution;
- final acceptance.

Must not accept a claim without evidence artifacts.

### Agent D: decompilation and binary-analysis agent

Owns:

- Ghidra project and symbol import;
- function dossiers;
- byte-for-byte matching loop;
- field/structure validation;
- identification of safe hook sites;
- separation of update and render side effects.

### Agent R: render/view pipeline agent

Owns:

- safe scene-only render boundary;
- second `CView` or equivalent viewport path;
- EFB viewport/scissor/projection handling;
- sequential world render passes;
- render-global snapshot/restore;
- frame-tail and present-once behavior.

### Agent G: gameplay and input agent

Owns:

- second controller snapshot;
- P2 party actor ownership;
- movement and targeting;
- native Art request path;
- AI decision suppression and restoration;
- disconnect behavior.

### Agent U: HUD and action-prompt agent

Owns:

- native HUD actor binding;
- equipped Arts and recharge mapping;
- independent cursors and targets;
- per-player HUD model state;
- Blossom Dance and other action-command prompts;
- HUD scaling/layout within split rectangles.

### Agent C: camera and culling agent

Owns:

- per-player native camera states;
- camera input ownership;
- culling frustum rebuild per view;
- camera collision and smoothing;
- scripted-camera bypass;
- camera state restoration.

### Agent M: memory, streaming, and draw-distance agent

Owns:

- expanded MEM2 verification;
- dedicated co-op allocator;
- fixed-pool audits;
- two-player streaming/activation envelope;
- draw-distance and LOD experiments;
- tether boundary and map-transition policy.

### Agent S: settings and presentation-state agent

Owns:

- external co-op configuration;
- presentation state machine;
- menu/cutscene/full-screen gating;
- native settings-menu integration after stabilization;
- transition polish.

### Agent Q: QA, diagnostics, and performance agent

Owns:

- deterministic test saves;
- gameplay-state mutation guards;
- render-pass mutation tests;
- long-session tests;
- performance metrics;
- backend/resolution matrix;
- regression reports.

### Function ownership rule

No two agents may modify the same original function concurrently without lead approval. Use `docs/ownership.csv`:

```csv
symbol,region,address,owner,branch,status,reason
Move__15CMenuArtsSelectFv,us,0x80103D68,U,agent/hud-arts,CLAIMED,Hud subject and cursor trace
wkRender__5CGameFv,us,<resolve>,R,agent/render-boundary,CLAIMED,Top-level render split investigation
```

---

## 7. Required workspace and tools

```text
XENO_DECOMP=<private/downstream xbret/xenoblade fork>
DOLPHIN_FORK=<custom Dolphin fork, only if required>
GAME_EXTRACT=<legally extracted game>
MAIN_DOL=<GAME_EXTRACT>/sys/main.dol
RELS=<GAME_EXTRACT>/files/rels/*.rel
GHIDRA=<Ghidra installation>
DOLPHIN=<debug-capable Dolphin build>
OBJDIFF=<objdiff executable>
PROJECT_OUT=<reports, traces, captures, artifacts>
```

Recommended layout:

```text
project/
  xenoblade/                    downstream game/decomp fork
  dolphin/                      optional custom Dolphin fork
  docs/
    architecture/
    evidence/
    functions/
    experiments/
    reports/
    ownership.csv
    milestone-status.md
  tools/
    ghidra/
    dolphin/
    tracing/
    hashing/
    capture/
  patches/
    instrumentation/
    coop-core/
    split-render/
    hud/
    camera/
    memory/
    streaming/
    settings/
  test-data/
    save-index.md
    scenarios/
```

Required tools:

- Python;
- Ninja;
- the compiler/toolchain pinned by the decompilation project;
- objdiff;
- Ghidra with headless scripts;
- Dolphin debug build with breakpoints and memory inspection;
- frame capture and performance logging;
- a hex viewer;
- scripts for memory snapshots and semantic-state hashes;
- two physical gamepads or one gamepad plus keyboard debug input.

### Baseline build

```bash
sha1sum "$MAIN_DOL"
git -C "$XENO_DECOMP" rev-parse HEAD
python configure.py --version us --map
ninja
```

Use the selected region instead of `us`. Preserve:

- exact tool versions;
- build logs;
- objdiff configuration;
- baseline boot capture;
- a known-good save and savestate;
- a baseline performance trace.

---

## 8. Shared implementation contracts

Agents must implement against stable interfaces rather than reaching into each other's temporary fields.

### 8.1 Core configuration

```cpp
enum class SplitOrientation : u8
{
    Vertical,
    Horizontal,
};

enum class CoopPresentationMode : u8
{
    Disabled,
    SplitGameplay,
    SharedMenu,
    SharedCutscene,
    SharedDialogue,
    SharedVision,
    SharedChainAttack,
    SharedTransition,
    SharedLoading,
    SharedError,
};

struct CoopConfig
{
    bool enabled;
    SplitOrientation orientation;
    u8 player2_party_slot;
    float maximum_separation;
    float warning_separation;
    bool use_expanded_mem2;
    bool expand_visual_draw_distance;
    bool debug_overlay;
};
```

### 8.2 Per-player authoritative state

```cpp
struct CoopPlayerState
{
    u8 player_index;
    u8 party_slot;
    CfObjectMove* move;
    CfObjectActor* actor;
    CfPadData* input;

    s32 selected_art_slot;
    u32 target_handle;
    bool targeting;
    bool command_pending;
    bool manual_control_enabled;

    // Candidate fields; replace with native types when recovered.
    u32 active_prompt_id;
    u32 active_prompt_stage;
    u32 last_command_result;
};
```

The exact native state should be reused where possible. Do not duplicate native battle rules.

### 8.3 Per-view presentation context

```cpp
struct CoopViewContext
{
    u8 view_index;
    u8 player_index;
    CView* view;
    CRect16 viewport;

    CfObjectActor* camera_subject;
    CfObjectActor* hud_subject;
    CfPadData* hud_input;

    CameraState* camera_state;
    HudPresentationState* hud_state;
};
```

### 8.4 Runtime state

```cpp
struct CoopRuntime
{
    CoopConfig config;
    CoopPresentationMode mode;
    CoopPlayerState players[2];
    CoopViewContext views[2];

    bool second_controller_connected;
    bool split_render_ready;
    bool expanded_memory_ready;
    bool streaming_envelope_ready;
};
```

### 8.5 Strict update/render interface

```cpp
void Coop_UpdateSimulation();       // exactly once per game update
void Coop_UpdatePresentation();     // camera/HUD model update, once per player
void Coop_RenderFrame();            // one or two render passes, no gameplay mutation
```

The QA agent must assert these call counts.

---

## 9. Presentation state machine

Use one centralized mode calculation. Do not scatter arbitrary menu/cutscene checks through render code.

```cpp
CoopPresentationMode DetermineCoopPresentationMode()
{
    if (!gCoop.config.enabled)
        return CoopPresentationMode::Disabled;

    if (IsFatalErrorOrControllerException())
        return CoopPresentationMode::SharedError;

    if (IsLoading())
        return CoopPresentationMode::SharedLoading;

    if (IsMapTransitionActive())
        return CoopPresentationMode::SharedTransition;

    if (IsCutsceneOrScriptedCameraActive())
        return CoopPresentationMode::SharedCutscene;

    if (IsDialogueActive())
        return CoopPresentationMode::SharedDialogue;

    if (IsVisionSequenceActive())
        return CoopPresentationMode::SharedVision;

    if (IsChainAttackActive())
        return CoopPresentationMode::SharedChainAttack;

    if (IsModalMenuOpen())
        return CoopPresentationMode::SharedMenu;

    if (!BothCoopActorsValid())
        return CoopPresentationMode::Disabled;

    return CoopPresentationMode::SplitGameplay;
}
```

### Transition rules

On every mode transition:

1. finish the current frame normally;
2. restore original viewport, scissor, projection, camera, culling, HUD, and GX state;
3. reset or reinitialize P2 camera presentation if entering split mode;
4. suppress P2 field input when entering an exclusive shared state;
5. do not destroy authoritative P2 gameplay state unless the game itself changes map/party;
6. log the transition and reason in debug builds.

```cpp
void SetCoopPresentationMode(CoopPresentationMode next)
{
    if (next == gCoop.mode)
        return;

    Coop_ExitPresentationMode(gCoop.mode);
    RestoreOriginalPresentationGlobals();
    Coop_EnterPresentationMode(next);
    gCoop.mode = next;
}
```

### Expected presentation matrix

| Game state | Presentation | Input owner |
|---|---|---|
| Normal exploration | Split | P1 and P2 |
| Ordinary battle | Split | P1 and P2 |
| Pause/options/inventory | Full screen | P1 initially |
| Dialogue | Full screen | P1 choices |
| In-engine cutscene | Full screen | scripted |
| Pre-rendered movie | Full screen | scripted |
| Loading | Full screen | none |
| Map transition | Full screen | none |
| Vision sequence | Full screen initially | original logic |
| Chain Attack | Full screen initially | original logic or later alternating control |
| Controller error | Full screen | original logic |
| Landmark/quest toast | Remain split | shared overlay or duplicated presentation |

---

## 10. Rendering strategy

### 10.1 Target frame flow

```cpp
void CoopFrame()
{
    UpdateGameSimulationOnce();
    Coop_UpdateSimulation();
    UpdateStreamingAndActivationOnce();
    UpdateUiAndCameraModelStateOnce();

    SetCoopPresentationMode(DetermineCoopPresentationMode());

    if (gCoop.mode == CoopPresentationMode::SplitGameplay)
        RenderSplitGameplay();
    else
        RenderOriginalFullscreenOnce();
}
```

### 10.2 Split gameplay flow

```cpp
void RenderSplitGameplay()
{
    BeginFrameOnce();
    SaveSharedRenderGlobals();

    PrepareViewContext(gCoop.views[0]);
    BuildCullingForView(gCoop.views[0]);
    RenderWorldSceneOnly(gCoop.views[0]);
    RenderHudOnly(gCoop.views[0]);

    RestoreGlobalsForNextView();

    PrepareViewContext(gCoop.views[1]);
    BuildCullingForView(gCoop.views[1]);
    RenderWorldSceneOnly(gCoop.views[1]);
    RenderHudOnly(gCoop.views[1]);

    RestoreSharedRenderGlobals();
    RunFrameTailOnce();
    PresentOnce();
}
```

Names are placeholders. The render agent must identify the real boundaries.

### 10.3 Render-boundary discovery procedure

Agent R and Agent D must:

1. Map `CGame::wkRender()` calls.
2. Map the task draw traversal and every relevant `Draw()` and `Tail()` callback.
3. Identify world-render submission, HUD rendering, UI animation update, effect simulation, queue flushes, EFB copy, frame tail, and present.
4. Classify every function as:
   - update once;
   - prepare once;
   - render per view;
   - tail once;
   - present once;
   - unknown/unsafe.
5. Add counters and memory mutation guards.
6. First render the same camera twice.
7. Only after passing mutation tests, render two different cameras.

### 10.4 Same-camera proof experiment

The first renderer milestone is not P2 camera support. It is:

- one simulation update;
- same camera rendered into both halves;
- no duplicated UI timers, particles, audio, AI, or frame tail;
- stable for at least 30 minutes in a quiet field and 15 minutes in battle.

If the same-camera experiment fails, do not proceed to camera/HUD differences.

### 10.5 Direct EFB split versus Dolphin render targets

Preferred first approach: direct game-side EFB split.

For each pass:

- set GX viewport;
- set GX scissor;
- set projection aspect appropriate to the half;
- clear or reset depth only for the relevant region, using a method validated for the game's renderer;
- render world and HUD;
- preserve the other half.

Fallback: a custom Dolphin path that captures two explicit render targets and composites them. Use this only if direct EFB partitioning cannot preserve depth/post-processing correctly.

### 10.6 Aspect ratio and view rectangles

Do not simply squash a full-width projection into half the screen.

For vertical split at output dimensions `W x H`:

```text
P1 rectangle: x=0,   y=0, width=W/2, height=H
P2 rectangle: x=W/2, y=0, width=W-W/2, height=H
```

For horizontal split:

```text
P1 rectangle: x=0, y=0,   width=W, height=H/2
P2 rectangle: x=0, y=H/2, width=W, height=H-H/2
```

Each view needs an adjusted projection or field of view. Test:

- original 4:3 mode;
- original widescreen mode;
- 16:9 output;
- ultrawide host output if supported by the custom frontend;
- internal resolutions from native through common high-resolution settings.

### 10.7 Render-global snapshot

Create a typed snapshot only after identifying the true global state. Candidate categories:

```cpp
struct RenderGlobalsSnapshot
{
    CView* current_view;
    CRect16 viewport;
    CRect16 scissor;
    Matrix44 projection;
    CameraState camera;
    CFrustum* active_frustum;
    HudSubjectContext hud_context;
    // Add only confirmed values.
};
```

Never memcpy broad unknown manager objects as a permanent solution. Snapshot explicit confirmed fields.

---

## 11. Camera and culling implementation

### 11.1 Maintain two camera presentation states

Each player needs independent:

- follow subject;
- camera orbit/yaw/pitch;
- zoom;
- smoothing history;
- collision result;
- battle framing state;
- lock-on camera behavior.

Do not update the same native camera object twice in a way that advances smoothing twice. Preferred options, in order:

1. create/clone a second native camera state object;
2. recover a native per-view camera abstraction and attach one camera to each view;
3. snapshot/restore all confirmed camera state around each pass;
4. as a temporary proof only, compute final position/look-at from separate co-op camera state.

### 11.2 Camera input ownership

```cpp
void UpdateCoopCameras()
{
    UpdateCameraStateForPlayer(0, *gCoop.players[0].input);
    UpdateCameraStateForPlayer(1, *gCoop.players[1].input);
}
```

Camera input is presentation state, but it updates once per player before rendering.

### 11.3 Culling

Treat culling as per-view render preparation:

```cpp
SetActiveFrustum(view.camera_state->frustum);
RebuildVisibilityForCurrentView();
RenderVisibleScene();
```

Audit whether visibility generation mutates authoritative object state, LOD hysteresis, streaming, or effect state. If it does, separate pure visibility lists from simulation-affecting decisions.

### 11.4 Scripted cameras

When the presentation mode is shared:

- use the original camera object;
- suspend P1/P2 camera input;
- do not apply per-player follow hooks;
- reset P2 camera position after the scripted state ends to avoid snapping from stale data.

### 11.5 Camera acceptance criteria

- P1 and P2 cameras follow different actors;
- camera input is independent;
- camera collision works for both views;
- no camera update changes actor/battle/RNG state;
- entering and leaving menus/cutscenes restores the original camera cleanly;
- no missing geometry caused by reusing the wrong frustum;
- long camera rotation tests do not corrupt renderer globals.

---

## 12. HUD and native UI implementation

### 12.1 Required P2 HUD content

Player 2's half must eventually show:

- correct character portrait/name;
- current and maximum HP;
- tension and status;
- actual equipped Arts;
- live recharge values;
- Talent Art and gauge;
- Art availability/seal state;
- independent selected-Art cursor;
- independent target panel and marker;
- range and positional feedback;
- active action-command prompts;
- success/miss feedback for multi-stage Arts.

A portrait/HP-only swap is not completion.

### 12.2 HUD implementation ladder

Use the least invasive viable method.

#### Level 1: scoped native HUD context

Temporarily provide the native HUD with a player-specific actor, cursor, target, and input context during its render/model-population path.

```cpp
ScopedHudContext ctx(
    player.actor,
    player.input,
    player.selected_art_slot,
    player.target_handle
);
RenderNativeHud();
```

The scope must not include gameplay update code.

#### Level 2: separate HUD model state, shared widget graph

If the native HUD update mutates state, build two `HudPresentationState` models once per frame, then populate and draw the widget graph sequentially without advancing gameplay.

#### Level 3: duplicate native HUD instances

If one widget graph cannot safely represent two contexts in the same frame, instantiate a second HUD object graph, attach each to a view, and allocate P2-specific presentation memory from the co-op heap.

Do not jump to Level 3 before proving Level 1 or Level 2 impossible.

### 12.3 HUD update versus HUD render

Recover and separate:

```text
read authoritative actor/battle state
update cursor/prompt animation timers
populate widget values
advance layout animation
submit layout draw calls
```

Timers must advance once, not once per view. A duplicated HUD may have separate presentation timers, but each instance still advances once per frame.

### 12.4 Arts data

Trace `CMenuArtsSelect` to determine:

- how it obtains the displayed actor;
- how visible slots map to equipped Arts;
- how recharge and disabled states are read;
- where the cursor lives;
- how confirm/cancel input is processed;
- which parts are authoritative gameplay versus presentation.

### 12.5 Blossom Dance and multi-stage Arts

The native action-command path must be reused.

```text
P2 activates Blossom Dance
-> Dunban's native action state begins
-> native prompt window opens
-> P2 B press is routed to the actor/action owner
-> native timing validation advances or ends the sequence
-> P2 HUD renders the actor's prompt state
```

Required separation:

```cpp
// Simulation, once:
UpdateActionCommandForActor(dunban, player2_input);

// Presentation, P2 viewport:
RenderActionPromptForActor(dunban);
```

Do not manually recreate damage, animation, timing, or cooldown logic.

### 12.6 HUD layout in half-screen

The HUD agent must test:

- native scaling versus custom scale;
- layout root translation;
- scissor clipping;
- font readability;
- Arts-bar width;
- party sidebar overlap;
- target panel overlap;
- damage numbers and world markers;
- 4:3 and widescreen resources;
- vertical and horizontal split.

Prefer preserving native asset proportions and moving components over globally shrinking everything.

### 12.7 Shared overlays

Classify overlays:

- per-player: Art bar, own HP, cursor, own target, action prompt;
- shared duplicated: party gauge, party list, battle messages;
- shared once over final output: landmark/quest notifications, controller errors, loading indicators;
- full-screen exclusive: menus, dialogue, cutscenes, visions, Chain Attack UI initially.

---

## 13. Gameplay and second-controller implementation

### 13.1 Preserve two input snapshots

```cpp
CfPadData gPlayerPad[2];
```

Required semantics for each:

- held flags;
- previous-held flags;
- newly pressed flags;
- repeat/turbo flags;
- stick values;
- disconnect state.

Use the recovered `CfPadTask` logic as the semantic reference. Remove or bypass secondary-channel rejection only after logging the original behavior.

### 13.2 Actor ownership

Initial mapping:

```text
controller 0 -> party slot 0
controller 1 -> party slot 1
```

Resolve through the party accessor and validate actor identity every party/map transition.

### 13.3 AI boundary

Disable only companion AI decision selection while P2 owns the actor. Preserve:

- action execution;
- animation;
- auto-attacks if desired;
- hit reactions;
- cooldown processing;
- status processing;
- battle state transitions.

Restore AI when:

- co-op is disabled;
- P2 disconnects;
- the assigned actor leaves the active party;
- a shared scripted state requires native AI;
- the player dies or the actor becomes unavailable, according to policy.

### 13.4 Arts execution

P2 must submit an Art through the native actor action pipeline.

Preferred discovery:

```text
P1 UI decision path ----+
                        +-> common native request/commit path -> action executor
AI decision path -------+
```

Replace the AI decision with P2's selected Art and target; do not simulate damage or cooldown manually.

### 13.5 Targeting

Maintain independent target state for each player. Both targets are authoritative and updated once. Each HUD/camera displays the corresponding target.

### 13.6 Shared states

When a modal menu or cutscene starts:

- suppress P2 field/battle input as appropriate;
- preserve actor state unless the original event system relocates or controls the party;
- avoid P2 input leaking into dialogue/menu confirm actions;
- resume manual control safely after the event.

---

## 14. Co-op setting implementation

### 14.1 Phase 1: external configuration

Use a mod/Dolphin configuration first:

```ini
[Coop]
Enabled = True
SplitOrientation = Vertical
Player2PartySlot = 1
MaximumSeparation = 50.0
UseExpandedMEM2 = True
DebugOverlay = False
```

The exact parser and delivery mechanism may be game-side patch data, a small host-to-guest interface, or a fixed development config compiled into the patch.

Benefits:

- no save-format risk;
- easy experimentation;
- additional debug options;
- settings can be changed without reverse-engineering the native menu.

### 14.2 Phase 2: native setting

After stable implementation, add native menu items:

```text
Co-op Mode              Off / On
Split Layout            Vertical / Horizontal
Player 2 Party Slot     Second / Third
Maximum Separation      Near / Medium / Far
```

Required discovery:

- options-menu class and entry table;
- label/message resources;
- cursor navigation and scrolling;
- value-change callback;
- defaults/reset behavior;
- settings serialization;
- layout capacity.

Do not modify original save structures until serialization is completely understood and backed up. A separate small config file remains an acceptable final solution.

### 14.3 Runtime toggle safety

Turning co-op off must:

1. finish the current frame;
2. return to full-screen original presentation;
3. restore original camera/HUD globals;
4. return P2 actor to AI;
5. clear pending P2 commands safely;
6. retain party state and position;
7. release optional presentation resources only at a safe boundary.

Turning co-op on must validate the controller, actor slot, map state, and render resources before entering split mode.

---

## 15. Expanded memory plan

### 15.1 Dolphin configuration

For the first memory experiment:

```ini
[Core]
RAMOverrideEnable = True
MEM1Size = 0x01800000
MEM2Size = 0x08000000
```

`0x01800000` is 24 MiB MEM1. `0x08000000` is 128 MiB MEM2.

Verify exact key handling in the selected Dolphin commit. Do not assume all frontends expose these settings in UI.

### 15.2 Guest accessibility test

Before allocating:

1. boot with expanded memory but no game patch using it;
2. verify normal stability;
3. write a test pattern into the proposed added range;
4. read it back;
5. confirm the original game never claims the range during long play;
6. verify savestates preserve it;
7. verify different graphics backends do not affect it.

```cpp
constexpr u32 kCoopMemStart = 0x14000000;
constexpr u32 kCoopMemEnd   = 0x18000000;
```

These are candidate addresses for 128 MiB total MEM2 and must be validated.

### 15.3 Dedicated allocator

Start with a simple guarded bump allocator, then replace it with a free-list allocator if needed.

```cpp
struct CoopHeap
{
    u8* start;
    u8* current;
    u8* end;

    void* Allocate(u32 size, u32 alignment);
    void ResetTransient();
};
```

Use canaries, allocation tags, and high-water logging.

### 15.4 Allowed first allocations

- debug traces;
- semantic hash buffers;
- P2 HUD model state;
- P2 camera state;
- second-view visibility lists;
- split-screen scratch structures;
- extra streaming bookkeeping.

### 15.5 Hardware-facing caution

Do not place GX, DMA, audio, IOS, or other hardware-facing buffers in expanded memory without proving addressability and cache behavior. CPU-only structures come first.

### 15.6 Fixed pools

More RAM does not enlarge fixed arrays or loop bounds. Audit:

- visible object lists;
- culling lists;
- map section arrays;
- NPC/enemy pools;
- particle pools;
- effect queues;
- layout/widget pools;
- render command buffers;
- collision residency arrays.

Every capacity change requires auditing allocation size, index type, loops, masks, sentinels, and serialization.

---

## 16. Streaming, draw distance, and player separation

### 16.1 Separate four concepts

1. camera far plane and fog;
2. visual LOD and submission distance;
3. object/NPC/enemy activation distance;
4. map section/collision/asset streaming.

Do not change all distance constants together.

### 16.2 Deterministic two-player envelope

Both actors exist in one simulation, so use both positions as authoritative anchors.

Preferred rule:

```cpp
bool ShouldKeepSectionLoaded(const Section& section)
{
    return IsNear(section, player1.position, radius) ||
           IsNear(section, player2.position, radius);
}
```

Alternative midpoint envelope:

```cpp
midpoint = (p1 + p2) * 0.5f;
required_radius = max(
    normal_radius,
    distance(p1, p2) * 0.5f + safety_margin
);
```

The union-of-two-spheres approach usually avoids loading unnecessary space between distant players.

### 16.3 Initial tether

Implement warning and hard limits based on measured engine behavior:

```cpp
if (distance > warning_separation)
    ShowSeparationWarning();

if (distance > maximum_separation)
    RestrictMovementOrReturnP2NearP1();
```

Do not select a final distance until tests identify when terrain, collision, AI, NPCs, or effects stop behaving correctly.

### 16.4 Map transitions

First-release policy:

- both players must remain in the same map instance;
- any map transition moves both players;
- skip travel moves both players;
- event triggers controlled by P1 unless later generalized;
- P2 cannot remain behind in a previous map.

### 16.5 Draw-distance rollout

1. far plane and fog;
2. terrain/vegetation LOD;
3. already-loaded model submission;
4. NPC/enemy visual range;
5. activation distance;
6. multiple streaming anchors;
7. fixed-pool expansion only after evidence.

Each step receives a separate performance and stability report.

---

## 17. Feature-driven decompilation and byte-matching loop

Every missing function should be approached as a bounded question, not “decompile the renderer.”

### 17.1 Standard loop

```text
select one function/question
-> import symbols and types
-> export assembly, relocations, decompiler output, callers, callees
-> produce conservative candidate C++
-> compile with the pinned original toolchain
-> compare with objdiff
-> classify mismatches
-> revise source shape/types/control flow
-> repeat toward exact code and relocation match
-> instrument original binary in Dolphin
-> verify semantic behavior
-> document evidence and hook safety
```

### 17.2 Match statuses

| Status | Meaning |
|---|---|
| `NOT_STARTED` | No reconstruction attempt. |
| `COMPILES` | Candidate compiles; no correctness claim. |
| `STRUCTURAL` | Control-flow and calls broadly align. |
| `HIGH_MATCH` | Most instructions/relocations match. |
| `CODE_MATCH` | Instruction bytes match; data/relocations may remain. |
| `FULL_MATCH` | Code, relocations, expected stack/function size match. |
| `BEHAVIOR_VERIFIED` | Runtime tests confirm the interpretation. |

A byte match proves faithful code generation, not the semantic name of every unknown field or hook safety.

**Current project policy:** the required acceptance bar for every decompilation target is **`FULL_MATCH`**. Intermediate statuses remain useful for logging progress, but a target is not complete until it reaches `FULL_MATCH`.

### 17.3 Common mismatch categories

- signed versus unsigned comparisons;
- wrong field type/width;
- branch shape;
- switch lowering;
- local-variable lifetime and register allocation;
- expression order;
- inlining;
- virtual or adjusted-this call;
- stack layout;
- constant-pool use;
- missing pragma/compiler context;
- incorrect relocation target.

### 17.4 Agent attempt record

```json
{"function":"CMenuArtsSelect::Move","region":"us","attempt":17,"status":"HIGH_MATCH","instruction_match":94.2,"relocation_match":88.9,"hypothesis":"wrong signedness for selected slot","next_change":"change candidate slot to s16","runtime_test":"HUD-03"}
```

Store records as JSONL for automated review.

### 17.5 Large functions

For large functions under the current **`FULL_MATCH` policy**:

- match leaf helpers first, but do not mark the parent target complete until the parent function also reaches `FULL_MATCH`;
- divide by state-machine cases;
- identify exact call sites;
- use narrow reversible hooks only after the hooked leaf or enclosing function is at `FULL_MATCH`;
- decompose work into smaller symbols/units, but do not accept `STRUCTURAL` or `CODE_MATCH` as the final state for any target.

---

## 18. Instrumentation and safety guards

### 18.1 Frame counters

Track per emulated frame:

```text
simulation update count
P1 gameplay update count
P2 gameplay update count
UI model update count
P1 camera update count
P2 camera update count
world render pass count
HUD render pass count
frame-tail count
present count
```

Expected during split gameplay:

```text
simulation update: 1
P1 gameplay: 1
P2 gameplay: 1
UI model update: 1 per intended HUD model
camera update: 1 per player
world render: 2
HUD render: 2
frame tail: 1
present: 1
```

### 18.2 Render mutation guard

Snapshot authoritative state immediately before each render pass and compare after it.

Include, where identified:

- party positions;
- HP/status;
- action state;
- Art cooldowns;
- target IDs;
- enemy HP/status;
- battle phase;
- party gauge;
- RNG state;
- quest/event flags;
- loaded section IDs;
- AI ownership.

Exclude confirmed presentation-only state.

Any authoritative change during the second view is a blocking failure.

### 18.3 Call tracing

Add lightweight trace events:

```cpp
struct TraceEvent
{
    u32 frame;
    u32 function;
    u32 actor;
    u32 view;
    u32 kind;
    u32 value0;
    u32 value1;
};
```

Write to a ring buffer, preferably in expanded MEM2. Export only on request or failure to avoid timing distortion.

### 18.4 Render-state assertions

Assert before full-screen menus/cutscenes:

- original view current;
- full-screen viewport/scissor;
- original projection restored;
- original camera restored;
- original HUD context restored;
- active frustum valid;
- no per-view scope active.

---

## 19. Milestone gates

Do not skip gates. Each gate requires a report, capture, logs, and rollback commit.

### Gate 0: reproducible baseline

- exact hash recorded;
- baseline builds;
- baseline boots;
- objdiff configured;
- test saves indexed;
- performance baseline recorded.

### Gate 1: instrumentation

- frame counters;
- trace ring buffer;
- authoritative-state hash;
- render mutation guard;
- function ownership ledger.

### Gate 2: two-player gameplay in one unsplit view

- two controller snapshots;
- P2 actor movement;
- independent target and Art cursor;
- native P2 Art execution;
- AI decision suppression/restoration;
- Blossom Dance-style follow-up path identified and functional;
- no split-screen work required yet.

### Gate 3: same-camera dual viewport

- same camera appears in both halves;
- simulation updates once;
- world renders twice;
- tail/present occur once;
- mutation guard passes;
- stable in field and battle.

### Gate 4: independent native cameras

- separate camera states;
- separate inputs;
- culling rebuilt per view;
- no missing geometry within tested radius;
- scripted camera bypass works.

### Gate 5: native HUD per player

- P1 and P2 HUDs show correct actors;
- correct equipped Arts/recharge;
- independent cursor and target;
- P2 action prompts;
- no gameplay mutation during HUD pass;
- layout is readable in selected split orientation.

### Gate 6: shared full-screen presentation

- menus full screen;
- dialogue full screen;
- cutscenes full screen;
- loading/transitions full screen;
- visions and Chain Attacks full screen initially;
- split resumes safely.

### Gate 7: co-op setting

- external setting persists;
- safe runtime enable/disable;
- controller disconnect fallback;
- no save corruption.

### Gate 8: expanded memory

- 128 MiB MEM2 boot verified;
- added range read/write verified;
- dedicated allocator stable;
- savestate behavior documented;
- CPU-only co-op allocations migrated safely.

### Gate 9: two-player streaming envelope

- both nearby regions remain loaded and active;
- collision works for both players;
- tether boundary measured;
- shared transitions work;
- performance remains acceptable.

### Gate 10: stabilization

- long-session tests;
- major areas tested;
- graphics backend matrix;
- resolution/aspect matrix;
- memory high-water and leaks analyzed;
- crash and recovery paths documented.

### Gate 11: native settings integration

Optional after stabilization.

---

## 20. Agent work packages

### WP-L1: architecture ledger

**Owner:** Agent L

Deliver:

- `docs/architecture/invariants.md`;
- interface header for `CoopRuntime`;
- ownership ledger;
- milestone status dashboard;
- merge plan.

Acceptance: all agents build against the same contracts.

### WP-D1: render call graph

**Owner:** Agent D

Question: where is the lowest safe world-render boundary below top-level animation, update, tail, and present logic?

Deliver:

- annotated call graph;
- function dossiers;
- side-effect classification;
- candidate hook sites;
- objdiff status;
- runtime trace.

Acceptance: Agent R can call a candidate scene pass twice with explicit known side effects.

### WP-R1: same-camera split

**Owner:** Agent R

Deliver:

- two rectangles;
- viewport/scissor/projection setup;
- same-camera world render in both;
- one tail/present;
- mutation-guard pass.

### WP-G1: second controller and P2 control

**Owner:** Agent G

Deliver:

- secondary channel retained;
- separate `CfPadData`;
- P2 actor assignment;
- movement and target;
- AI fallback.

### WP-U1: native HUD subject

**Owner:** Agent U

Deliver:

- actor binding trace;
- equipped Arts mapping;
- P2 native HUD proof;
- cursor/target ownership;
- action-prompt trace.

### WP-C1: camera clone/context

**Owner:** Agent C

Deliver:

- independent camera state;
- actor follow;
- camera input;
- frustum rebuild;
- scripted-state bypass.

### WP-S1: presentation mode detection

**Owner:** Agent S

Deliver:

- reliable predicates for modal menu, dialogue, cutscene, loading, transition, vision, Chain Attack;
- centralized state machine;
- transition logs;
- full-screen restoration.

### WP-M1: expanded memory

**Owner:** Agent M

Deliver:

- Dolphin config profile;
- address-range validation;
- co-op allocator;
- high-water report;
- savestate report.

### WP-M2: streaming envelope

**Owner:** Agent M

Deliver:

- streaming-anchor trace;
- two-anchor implementation;
- loaded-section logging;
- tether recommendation;
- draw-distance stages.

### WP-Q1: automated acceptance harness

**Owner:** Agent Q

Deliver:

- scripted test checklist;
- state hashes;
- frame counters;
- captures;
- performance report generator;
- regression summary.

---

## 21. Test matrix

### 21.1 Gameplay scenarios

| Scenario | Required checks |
|---|---|
| Quiet field | cameras, movement, streaming, stable frame counts |
| Ordinary battle | two targets, two Arts, cooldowns, AI fallback |
| Blossom Dance | all follow-up presses, success and miss |
| P1 death | ownership and camera behavior |
| P2 death | AI/control fallback and HUD |
| Battle start/end | mode and camera transitions |
| Party member swap | actor pointers and HUD/Arts update |
| Controller disconnect | P2 returns to AI, full-screen policy if desired |
| Menu open/close | full-screen restore and split resume |
| Dialogue | no input leakage |
| In-engine cutscene | scripted camera preserved |
| Movie playback | one full-screen output |
| Chain Attack | shared full-screen and correct return |
| Vision | shared full-screen and correct return |
| Map transition | both players move and pointers refresh |
| Skip travel | both players move and state remains valid |
| Save/load | no corruption, co-op config behavior documented |

### 21.2 Area coverage

Test at minimum:

- an uncomplicated open field;
- a dense town;
- a vertical/multi-level area;
- a visually heavy area;
- an area known to stress draw-distance patches;
- indoor and outdoor maps;
- weather changes;
- day/night transition;
- crowded battle;
- boss battle with scripted cameras.

### 21.3 Graphics matrix

- Vulkan;
- Direct3D where applicable;
- OpenGL;
- native internal resolution;
- 2x and 3x common internal resolutions;
- widescreen and 4:3;
- vertical split;
- horizontal split after vertical stabilizes;
- common shader compilation modes;
- EFB copy modes relevant to the game.

### 21.4 Memory matrix

- retail memory;
- expanded MEM2 without using added range;
- expanded MEM2 with debug-only allocations;
- expanded MEM2 with HUD/camera allocations;
- savestate load with matching configuration;
- expected rejection/documented incompatibility with mismatched configuration.

### 21.5 Long tests

- 60 minutes exploration;
- 60 minutes continuous battles;
- repeated menu/cutscene transitions;
- repeated map transitions;
- repeated co-op enable/disable;
- repeated controller disconnect/reconnect;
- memory high-water and leak check.

---

## 22. Performance requirements and profiling

Record separately:

- emulated CPU frame time;
- host GPU frame time;
- draw calls or equivalent submission count;
- EFB operations;
- visibility-list size per view;
- loaded sections and active actors;
- co-op heap high-water;
- frame pacing;
- shader stutter;
- audio underruns.

Expected cost:

- world render traversal approaches 2x;
- simulation remains near 1x;
- culling occurs per view;
- HUD rendering occurs per view;
- streaming and activation may increase with player separation.

Optimization order:

1. correctness;
2. eliminate duplicate updates/tails/presents;
3. cache shared per-frame calculations proven camera-independent;
4. reduce redundant state setup;
5. tune per-view LOD if visually acceptable;
6. optimize expanded streaming;
7. consider Dolphin-side compositor improvements.

Never share a result between views unless it is proven camera-independent.

---

## 23. Failure modes and required responses

### Duplicate animations/effects

Likely cause: update logic occurs in the per-view render path.

Response: trace writes and move the update before both render passes.

### HUD cursor works but executes the wrong Art

Likely cause: display state was rebound but authoritative command state remains global.

Response: separate per-player selected slot and route native command by actor owner.

### Blossom Dance prompt displays but P2 cannot advance it

Likely cause: prompt render was rebound but action-command input still reads the main pad.

Response: route the native prompt input lookup by actor ownership during simulation.

### Geometry missing in P2 view

Likely cause: culling/visibility built for P1 camera only.

Response: rebuild frustum/visibility per view and audit camera-dependent LOD.

### UI or particles run twice as fast

Likely cause: top-level render/update path called twice.

Response: lower the render boundary and enforce frame counters.

### Menus appear in one half

Likely cause: presentation mode did not restore full-screen state before modal UI.

Response: centralize mode transitions and assert viewport/scissor/camera restoration.

### Crashes after increasing draw distance

Likely cause: fixed pool or queue capacity, not raw memory.

Response: identify the exhausted pool before increasing more memory or distances.

### P2 enters unloaded terrain

Likely cause: streaming follows P1 or camera only.

Response: implement two-player envelope or reduce tether.

### Expanded memory appears unused

Likely cause: original allocator still uses retail bounds.

Response: use a dedicated allocator in the added range.

### Save state fails after RAM change

Likely cause: configuration mismatch or unsupported added region serialization.

Response: document configuration lock and patch Dolphin state handling if necessary.

### Agent claims success from plausible pseudocode

Response: require objdiff and runtime evidence before merge.

---

## 24. Evidence and handoff format

Every agent handoff must include:

```markdown
# Handoff: <work package>

- Agent:
- Branch/commit:
- Region and main.dol SHA-1:
- Xenoblade commit:
- Dolphin commit:
- Build mode:

## Objective

## Changed files

## Original functions touched

## Confirmed findings

## Candidate findings

## objdiff results

## Runtime tests

## Captures/logs

## Authoritative-state mutation result

## Performance result

## Known failures

## Rollback instructions

## Next agent action
```

Evidence levels:

| Level | Meaning |
|---|---|
| `UNKNOWN` | no reliable interpretation |
| `CANDIDATE` | plausible hypothesis |
| `CONFIRMED_STATIC` | supported by code/assembly/layout |
| `CONFIRMED_RUNTIME` | reproduced in controlled run |
| `BEHAVIOR_VERIFIED` | feature passes defined tests |
| `MATCHING` | candidate source matches original object code |

---

## 25. Merge and branch policy

Suggested branches:

```text
agent/decomp-render
agent/render-same-camera
agent/gameplay-p2
agent/hud-p2
agent/camera-p2
agent/presentation-state
agent/memory-expanded
agent/streaming-envelope
agent/qa-harness
integration/split-screen
```

Rules:

- instrumentation commits separate from behavior commits;
- one logical change per commit;
- no permanent anonymous magic addresses;
- region address profiles separate from logic;
- every hook has a signature check and safe failure path;
- every merged work package has a rollback tag;
- integration branch is always bootable;
- feature flags gate incomplete work.

---

## 26. Initial execution plan

### Sprint 1: foundation

Agent L:

- create contracts and ownership ledger.

Agent D:

- verify symbols and export render/input/HUD dossiers.

Agent Q:

- implement frame counters, state hashes, and baseline reports.

Agent G:

- retain second controller and log input only.

### Sprint 2: unsplit co-op gameplay

Agent G and U:

- P2 actor movement;
- target and cursor;
- native Art execution;
- action-command path.

No split-screen dependency.

### Sprint 3: render proof

Agent D and R:

- safe lower render boundary;
- same-camera dual viewport;
- mutation guard.

Agent C:

- trace camera state and culling.

### Sprint 4: independent views

Agent C and R:

- separate camera states;
- frustum per view.

Agent U:

- P2 HUD render context.

### Sprint 5: presentation gating

Agent S:

- shared full-screen states;
- external co-op toggle;
- transition safety.

### Sprint 6: memory and world range

Agent M:

- expanded MEM2 and co-op heap;
- streaming anchors;
- tether and draw-distance experiments.

### Sprint 7: stabilization

Agent Q leads the full matrix and regression closure.

---

## 27. Lead-agent kickoff prompt

Copy this into the lead agent:

```text
You are the lead engineer for a private/downstream Xenoblade Chronicles Wii single-instance split-screen co-op project.

Read PLAN.md in full. Treat its architecture invariants and milestone gates as mandatory.

Goal:
- one authoritative game simulation;
- two controllers and two manually controlled party actors;
- two sequential native world/HUD render passes only during normal gameplay;
- original full-screen presentation for menus, cutscenes, dialogue, loading, visions, Chain Attacks, and scripted cameras;
- optional dedicated heap in expanded Dolphin MEM2;
- two-player streaming envelope and a measured tether.

Do not attempt to decompile the entire game. Recover the smallest connected code slice required for each milestone. Use Ghidra, the original compiler, objdiff, Dolphin runtime tracing, semantic-state hashes, and controlled test saves.

Never report a hypothesis as confirmed. Every function or field claim must be marked UNKNOWN, CANDIDATE, CONFIRMED_STATIC, CONFIRMED_RUNTIME, BEHAVIOR_VERIFIED, or MATCHING.

Do not call the complete CGame::wkRender/task draw path twice. Identify a lower scene-only render boundary. The simulation, UI model updates, camera updates, particles, audio, frame tail, and present must occur only at their intended counts.

Create and maintain:
- docs/ownership.csv;
- docs/milestone-status.md;
- architecture contracts;
- per-function dossiers;
- JSONL objdiff attempt logs;
- experiment reports;
- rollback commits.

Assign agents for decompilation, rendering, gameplay/input, HUD/action prompts, camera/culling, memory/streaming, settings/presentation, and QA. Do not allow two agents to edit the same original function concurrently.

First tasks:
1. verify executable hash and repository/tool commits;
2. build and boot an unmodified baseline;
3. install frame counters and authoritative-state mutation guards;
4. validate party actor and Arts pointers;
5. retain and log the second controller;
6. map CGame::wkRender through draw, tail, EFB copy, and present;
7. produce a side-effect classification and candidate lower render boundary;
8. do not start independent cameras or HUDs until the same-camera dual-viewport gate passes.

At each gate, provide changed files, addresses, objdiff results, runtime evidence, captures, performance, known failures, and rollback instructions.
```

---

## 28. Specialist-agent prompt template

```text
Role: <agent role>
Work package: <ID and title>
Region/hash: <region and SHA-1>
Xenoblade commit: <commit>
Dolphin commit: <commit>
Owned symbols/functions: <list>
Do not modify: <list>

Question to answer:
<one falsifiable question>

Required static evidence:
- assembly and relocations;
- decompiler output;
- callers/callees;
- imported types;
- relevant source and symbol ranges.

Required runtime evidence:
- exact scenario/save;
- breakpoints/watchpoints;
- values to log;
- negative controls;
- authoritative-state mutation check.

Required implementation:
<smallest reversible patch>

Acceptance criteria:
<binary pass/fail conditions>

Required output:
- function dossier;
- evidence status;
- candidate/reconstructed code;
- objdiff attempt log;
- test report;
- branch/commit;
- rollback instructions;
- remaining uncertainty.

Do not widen the scope. Do not declare completion because code compiles or pseudocode looks plausible.
```

---

## 29. Definition of done

The project is complete for the first release when all of the following are true:

- one Dolphin process runs one Xenoblade simulation;
- co-op can be enabled or disabled safely;
- two controllers control two party actors;
- both can move, target, execute normal Arts, and complete actor-specific follow-up prompts;
- normal exploration and battle render in two native views;
- each view has its own native camera and correct HUD;
- menus, cutscenes, dialogue, loading, transitions, visions, and Chain Attacks use stable full-screen presentation;
- no gameplay state changes during the second render pass;
- frame counters prove update/tail/present counts;
- camera/culling work for both views;
- P2 returns to AI on disable/disconnect;
- both players remain in the same map and transitions move both;
- the tested tether prevents invalid streaming states;
- expanded memory, if required, is isolated and documented;
- test matrix and long sessions pass within defined performance limits;
- all unknowns and restrictions are documented;
- distributable output contains no proprietary game files.

---

## 30. Primary source references

Re-check these against the selected commit:

- Xenoblade decompilation repository and build/contribution policy: `https://github.com/xbret/xenoblade`
- Recovered top-level game update/render code: `https://github.com/xbret/xenoblade/blob/main/src/kyoshin/CGame.cpp`
- Recovered process/view attachment code: `https://github.com/xbret/xenoblade/blob/main/libs/monolib/src/core/CProc.cpp`
- Recovered process draw/tail traversal: `https://github.com/xbret/xenoblade/blob/main/libs/monolib/src/work/CProcess.cpp`
- Recovered controller path: `https://github.com/xbret/xenoblade/blob/main/src/kyoshin/cf/CfPadTask.cpp`
- Recovered party manager declaration: `https://github.com/xbret/xenoblade/blob/main/src/kyoshin/cf/CfGameManager.hpp`
- Recovered global G3D work memory: `https://github.com/xbret/xenoblade/blob/main/libs/nw4r/src/g3d/g3d_workmem.cpp`
- Region symbols and splits: `config/jp`, `config/eu`, and `config/us` in the selected Xenoblade checkout
- Dolphin RAM override definitions: `https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/Config/MainSettings.cpp`
- Dolphin guest memory initialization: `https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/HW/Memmap.cpp`

---

## 31. Immediate first assignment

The first coordinated assignment is:

> Prove that a single simulation frame can render the same native world camera into two non-overlapping view rectangles while the simulation update, UI model update, frame-tail callbacks, and final presentation each execute only once.

Parallel supporting tasks:

- retain and log Player 2 input without changing gameplay;
- validate party slot actor pointers and Arts blocks;
- create authoritative-state mutation guards;
- trace menu/cutscene state predicates;
- identify the exact view, viewport, scissor, projection, culling, EFB copy, tail, and present functions used by the current renderer.

Do not begin broad draw-distance expansion or native options-menu work before the same-camera dual-viewport gate passes.
