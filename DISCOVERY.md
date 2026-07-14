# Trace / discovery findings

Static discovery pass for the unchecked **TRACE / discovery** tasks in [`TASKS.md`](TASKS.md) (P2 presentation, P3 streaming/camera, P1 targeting / Blossom Dance).

**Method:** symbols (`config/us/symbols.txt`), splits (`config/us/splits.txt`), headers/sources under `src/kyoshin/**`, retail asm under `build/us/asm/`, and [`DECOMP_MAP.md`](DECOMP_MAP.md) / [`PLAN.md`](PLAN.md). No Dolphin runtime traces yet — confidence is static only.

**Passes:** Pass 1 = seed inventory (§§1–6). Pass 2 = asm body/xref TRACE (§9). Pass 3 = **parsed objects / SDA bit dictionary** (§10) — no Dolphin.

---

## Status legend

| Confidence | Meaning |
|------------|---------|
| **H** | Named symbol + clear role from source/comments or DECOMP_MAP |
| **M** | Strong candidate (unit + size + neighbors); role needs Ghidra/runtime confirm |
| **L** | Seed only; must be proven by call/xref or runtime log |

---

## 1. P2 — presentation predicates & event paths

### 1.1 `presentation-predicates`

Prefer **manager / plugin flags** over widget visibility (`DECOMP_MAP` §10.2).

| Mode | Best seed(s) | Addr | Unit | Conf | Evidence |
|------|--------------|------|------|------|----------|
| **PAUSE_OR_HOME_MENU** | `CGame::OnPauseTrigger(bool)` | `0x80039FB4` | `kyoshin/CGame.cpp` | H | FULL_MATCH; pause gate on top-level game |
| | `CfPadTask::onInitHbm` | `0x801C4490` | `kyoshin/cf/CfPadTask.cpp` | M | HBM entry on pad task |
| **NORMAL_FIELD / battle shared flags** | `CfGameManager::sUnkFlags` + `checkUnkFlag` / `setUnkFlag` | (sbss) | `CfGameManager` | M | Bitfield API recovered; `CTaskGameCf` ors `0x200000` |
| **MODAL_MENU** | `CUICfManager::mFlags` @ `+0xC90` | — | `CUICfManager` | H | `Move` bitflags drive create/teardown of key-assign / enemy-state / symbol-mark / battle-mode / level-up menus |
| | `CUIWindowManager::{unkA0,unkA1}` + window queues | — | `CUIWindowManager` | H | Modal window list + force-update; `Move` HIGH_MATCH ~79% |
| **DIALOGUE** | `CTalkWindow::{Init,Move,cbRenderBefore}` | `0x8012C8A8` / `0x8012D374` / `0x8012D6A0` | TalkWindow TU | H | Named talk window task |
| **IN_ENGINE_CUTSCENE** | `realtimeEventStart/Play/End` | `0x8004722C` / `0x800472C0` / `0x8004731C` | `kyoshin/plugin/pluginEve.cpp` | H | Named realtime-event API |
| | `CTaskREvent::Move` | `0x80166324` size `0xC30` | `kyoshin/cf/CTaskREvent.cpp` | H | Primary REvent task; header size `0x1F8` |
| | `CTaskREvtSequence::*` | `0x80169238+` | REvtSequence TU | H | Sequence runner beside REvent |
| | `CREvtCamera` / `CREvtMovie` / `CREvtModel*` | `0x80181374` / `0x80296E70` / … | CREvt* | M | Scripted cutscene/model/movie pieces |
| **VISION** | `mesVisionON/OFF` | `0x80046A70` / `0x80046AB0` | `pluginUi.cpp` | H | Named vision messages |
| | `isVisionEvent` | `0x8004780C` | `pluginEve.cpp` | H | Vision-event predicate |
| | `setFieldVision` | `0x8004B4E8` | `pluginCfs.cpp` | H | Field vision setter |
| | `cf::CVision` | ctor `0x801A47F8` | Vision TU | H | Huge object (`0x2623C`); header present |
| | `breakVision` (+ `vision`, `selectTgt`) | `0x80187534` | `pluginBtl.cpp` | H | Battle plugin; not CREvt |
| **CHAIN_ATTACK** | `kyoshin/cf/chain/CChain*.cpp` | e.g. `CChain` dtors ~`0x800D96xx` | chain splits | H | Dedicated chain TU family in splits |
| **LOADING** | `dispLoading` | `0x8004B3F0` size `0x2C` | `pluginCfs.cpp` | H | Named loading display |
| | `setFade` / `CFade` / `CMenuFade::Move` | `0x8004AA44` / `0x80246498` / `0x80114500` | pluginCfs / Fade / MenuFade | H | Fade + menu-fade paths |
| **MAP_TRANSITION** | `setMapJumpArea` / `setWarpArea` / `setMapPreloadArea*` | `0x8004785C` / `0x80048238` / `0x800487DC` | `pluginCfs.cpp` | H | Named jump/warp/preload area setters |
| | `CMapSel`, `CTaskGameEvt` | `0x80245264` / `0x80297B60` | map-select / game-evt | M | Map UI + event task |
| **SCRIPTED_CAMERA** | `CREvtCamera::__ct` | `0x80181374` | CREvtCamera | M | Event camera object |
| | `CfCamEvent::*` + large `func_8006*` in `CfCam.cpp` | see §3 | `CfCam.cpp` | M | Event cam interfaces already partly matched |
| **PRE_RENDERED_MOVIE** | `CREvtMovie::__ct` | `0x80296E70` | CREvtMovie | M | Named movie REvt |
| **NORMAL_BATTLE** | `CUIBattleManager`, `cf::CBattleManager`, battle HUD Move | various | battle UI / `CBattleManager` | M | Manager existence known; predicate bit not pinned |

#### Ranked authoritative sources for `ShouldRenderSplitScreen()`

1. **`CGame` pause / standby** — `OnPauseTrigger`, logout/login (already FULL_MATCH).
2. **Plugin vision + realtime event APIs** — `isVisionEvent`, `realtimeEvent*`, `mesVision*`, `dispLoading`, map jump/warp setters in `pluginEve` / `pluginUi` / `pluginCfs`.
3. **`CUICfManager::mFlags` + `CUIWindowManager` queues** — modal/menu activity (not field play).
4. **`CTaskREvent` / `CTaskREvtSequence` activity** — in-engine cutscene.
5. **Chain / Vision object liveness** — full-screen shared UI per DECOMP_MAP §8.3.

Do **not** gate split on widget `IsVisible` alone.

### 1.2 `event-cutscene-paths`

Call-graph seeds (static):

```text
CGame::{wkUpdate,wkRender,OnPauseTrigger,GameMain}
  → CTaskGame / CTaskGameCf
  → pluginEve: realtimeEventStart/Play/End
  → pluginUi: mesVisionON/OFF, isVisionEvent
  → pluginCfs: setMap*, getMapID, dispLoading, setFade
  → cf::CTaskREvent::Move (0x80166324)
  → cf::CTaskREvtSequence
  → CREvt{Camera,Model,ModelMap,ModelObj,ModelPc,Effect,Movie,Light,Obj,Mem}
  → CTalkWindow / CMainMenu / CSystemWindow / CQuestWindow
  → cf::CVision
  → CFade / CMenuFade / CMapSel / CTaskGameEvt / CSkipTimer
```

Headers already in tree: `CTaskREvent.hpp`, `CVision.hpp`, `IFlagEvent.hpp`. Most REvt / Talk / MainMenu bodies are **not** decompiled yet.

### 1.3 `should-render-split`

Not implemented. Required restore set is still the DECOMP_MAP §10.2 list (view/camera/HUD/culling/GX). Static prerequisites recovered above; next step is a **boolean classifier** over the ranked flag sources, then a runtime matrix of field / battle / menu / talk / REvent / vision / chain / load / pause.

**Next TRACE (runtime):**

1. Log `CUICfManager::mFlags`, `CfGameManager::sUnkFlags`, `isVisionEvent`, `realtimeEvent*` enter/exit, `dispLoading`, `OnPauseTrigger`.
2. Correlate with full-screen vs freelook frames in Dolphin.
3. Draft `ShouldRenderSplitScreen()` only after predicates stabilize.

---

## 2. P3 — streaming / activation / draw-distance

Split ownership note: named map/stream **plugin** symbols live in `kyoshin/plugin/pluginCfs.cpp` (and neighbors), not only `CfGameManager`. Per §9.1, most of those names are **script VM entry points** — prefer native SDA / `CfGameManager` / `CfBdat` workers for TRACE hooks.

**Concept separation (PLAN §16.1):** frustum/occlusion cull ≠ object render-distance ≠ AI activation ≠ map-section streaming. Do not collapse them into one TRACE bucket.

| Task id | Best candidate(s) | Addr | Unit | Conf | Why |
|---------|-------------------|------|------|------|-----|
| **stream-map-area-id** | **`lbl_eu_80664184`** (u32 map/area id) | sbss | `CfBdat` writers | H | Authoritative store (§9.7 / §10.6); **only CfBdat writes** |
| | `getMapID` → `func_80086B1C` | `0x8004AFAC` | `pluginCfs` (VM) | H | Script wrapper over the SDA word — not a native callee |
| | `setMapDispID` | `0x800497D8` | `pluginCfs` (VM) | M | Display/map id write (script) |
| **stream-anchor-position** | `CfGameManager::func_80082D54(playerIndex)` → `CfObjectMove*` | `0x800836D8` | `CfGameManager` | H | Party-index accessor (354 call sites); also camera fallback subject (§10.5) |
| | Pose helper `func_80496264` (US `0x8049A2DC`) | — | L/M | HUD + camera aux (`follow+0x160`); not the streaming anchor itself |
| **stream-terrain-sections** | `setMapPreloadArea` / `setMapPreloadArea2` | `0x800487DC` / `0x80048C84` | `pluginCfs.cpp` | M | Preload **registration**; load/unload bodies not isolated |
| | `CBgTex::OnFileEvent` (prefer over ctor) | `0x801C581C` size `0x78` | `CBgTex.cpp` | M | Async bg-tex completer; ctor alone is weaker |
| | `CScnItemPool::update` | `0x80490E2C` size `0x124` | `CScnItemPool` | M | Explicit DECOMP_MAP §13.1 seed |
| **stream-collision-sections** | `CCol6System::Move` / `CCol6Hint::Move` | `0x8015FFD8` size `0x1210` / `0x8015E8AC` size `0x32C` | col6 | M | Substantive collision tasks |
| | `OnFileEvent` on Col6 | `0x80165290` / `0x80165270` size `0x8` | col6 | L | Stubs only — do not treat as loaders |
| **stream-spawn-despawn** | `addPopID` / `setPopSheet` / `waitPop` | `0x8004953C` / `0x800495AC` / `0x8004B0E4` | `pluginCfs.cpp` | M | Named population-sheet API (spawn-adjacent) |
| | `CfObjectSelectorObj` | dtor `0x800FE190` | selector | M | DECOMP_MAP §13.1 seed; almost no other named methods |
| | `CfGameManager` large helpers (`func_8007D1A0` size `0x5F4`, etc.) | `0x8007DB3C+` | `CfGameManager.cpp` | L | Need Ghidra naming |
| **stream-ai-activation** | (none named yet) | — | actors / AI | L | Separate actor-distance paths from frustum cull; see TRACE §13.3 |
| **stream-lod-selection** | `CTaskLOD` TU (`create` + helpers; `Move` is `0x8`) | `0x80466800+` | splits `monolib/.../CTaskLOD.cpp` | M | Dedicated TU, **no source yet**; not proven as model/veg LOD |
| **stream-render-cull** | *(distance-based object cull — still open)* | — | — | L | TASKS #8; not the same as frustum occlusion |
| | `COccCulling::{addFrustum,setFrustum,…}` / `CTaskCulling::*` | `0x801A1F6C+` / `0x801A4324+` | cull TUs | H† | †Camera/frustum path (DECOMP_MAP §9.2) — log under camera TRACE, not as stream-distance proof |
| **stream-map-transition** | `mapJump` | `0x80048A5C` size `0x228` | `pluginCfs` (VM) | M | Named jump action — **script** entry; native section load still TBD (§9.7) |
| | `setMapJumpArea` / `setMapJumpAreaBox` / `setWarpArea` | `0x8004785C` / `0x80047E18` / `0x80048238` | `pluginCfs` (VM) | M | Jump/warp area registration (script) |
| | `CMapSel`, `CTaskGameEvt::Move` | `0x80245264` / `0x80297CE0` | map / game-evt | M | Transition UI / task |
| **stream-teleport-regroup** | `partyWarp` | `0x8004B154` size `0x6C` | `pluginCfs` (VM) | M | Best named regroup/teleport **script** candidate |
| | `makeParty` / `addParty` / `delParty` | `0x80049BF8+` | `pluginCfs.cpp` | M | Party rebuild neighbors |
| | `setWarpArea` / `setTownArea` / `setEventArea` | `0x80048238` / `0x80049190` / `0x80048ECC` | `pluginCfs.cpp` | M | Area setters; weaker than `partyWarp` |
| | `setDispOffArea` | `0x8004AC34` | `pluginCfs.cpp` | L | Display-off area; possible visibility/stream gate |
| **stream-pool-audit** | Game object / map / sound fixed pools (TBD) | — | — | L | `CUICfManager::unk13C` is **UI event-node** capacity — not a stream pool |
| **stream-async-callbacks** | `CBgTex::OnFileEvent` + map/archive completers (TBD) | `0x801C581C` etc. | many TUs | M | Pattern exists; large `OnFileEvent`s so far are mostly **UI**; streaming-specific still unnamed |

### Actor-driven vs camera-driven (static separation)

| Class | Candidates | Notes |
|-------|------------|-------|
| **Camera / cull (per view)** | `COccCulling`, `CTaskCulling`, `CScnCameraMan`, `CScnItemCamera*` | Rebuild frustum per view; DECOMP_MAP §9.2 acceptance |
| **Actor / map (once)** | `pluginCfs` (`getMapID`, preload, `mapJump`, `partyWarp`, pop sheet), `CfGameManager` player array, spawn managers | Must not double-run per viewport |
| **LOD** | `CTaskLOD` | Classify as sim-once vs view-dependent after TRACE; may not be game veg LOD |

### Recommended runtime TRACE (refine DECOMP_MAP §13.3)

1. Stationary P1 + camera; move only P2 actor → log `getMapID`, `mapJump` / preload / `partyWarp` / pop-sheet, `CBgTex::OnFileEvent`, col6 `Move`, object create/destroy, real pool failures.
2. Move only camera → log `COccCulling` / `CTaskCulling` / LOD; confirm no map-section churn.
3. Record which **distance** checks fire in (1) vs (2) → AI activation vs render-distance (frustum hits alone do not close those tasks).

---

## 3. P3 — camera discovery

TU ranges (`DECOMP_MAP` §9.1): `CfCam.cpp` `0x8006C0F8–0x800758E8`, `CfCamEvent_1` `0x800758E8–0x8007CA94`. Intf leaves (own TUs, before `CfCam.cpp`): Direction / Lookat / Target `setPos/Dir/Lookat` (FULL_MATCH). Also: `CfCamFollow` ctor, `CfCamEvent_initCamIntfInstances`.

**Pass-3 resolution (§10.5):** follow subject is **not** chosen inside the follow ctor alone. Factory `func_800784A0` (`CfCamEvent_1`) creates follow / event cams into a **3-slot** manager at `*lbl_eu_80663DF0`, with subject → `follow+0x164`, aux → `follow+0x160`, fallback actor `func_80082D54(1)`.

Script layer: `pluginCam.cpp` (`0x800609B8–0x80061BF4`) names `select`, `setTarget`, `setPos`/`setLookat`/`setDir`, keyframes, `shake` — useful for scripted override, secondary to the native factory above.

| Task id | Candidate(s) | Addr | Size | Conf | Evidence / co-op note |
|---------|--------------|------|------|------|------------------------|
| **cam-follow-actor-select** | `func_800784A0` factory + modes | `0x80078E3C` size `0x6C0` | `CfCamEvent_1` | H | Only creator of follow (`func_80074CEC`) and event follow (`__ct__8006B310`); mode @ manager`+0x3C` |
| | Subject / aux fields | `follow+0x164` / `+0x160` | — | H | Ctor stores r4→164, r5→160; factory wires owner / `func_80496264` aux / `func_80082D54(1)` fallback |
| | Cam manager `lbl_eu_80663DF0` | sbss ptr | — | H | 3 active cam slots @ `+0/+4/+8` |
| | `pluginCam::select` / `setTarget` | `0x800609B8` / `0x80060F40` | `0x38` / `0x5C` | M | Script VM entry points (§9.1), not the native subject binder |
| | Near-follow / later giants | `func_8006C1C8`… / `func_80072194`… | various | M | Updates after construct; classify vs factory |
| **cam-controller-pad-read** | `func_80074A3C` → `getCurrentPad` / `getMainGCPad` | `0x800753D8` | — | H | **Proven** CfCam pad reader (§9.4); sibling `func_80074A74` reads `pad+0xE8` |
| | `getCurrentPad` / channel / `CfPadData` | `0x80087B94+` | small | H | Singleton path; P1-centric (65 camera+/HUD callers) |
| | `CfPadTask::{update,updateCfPadData,Move}` | `0x801C3AC0+` | large | H | Sampling pipeline |
| **cam-final-matrix-write** | `CfCamDirectionIntf_*` / `CamLookatIntf_*` / `CfCamTargetIntf_*` | `0x8006BE48+` | small | H | Leaf writes (FULL_MATCH); own TUs |
| | `pluginCam::{setPos,setLookat,setDir,…}` | `0x80060AC0+` | medium | M | Script-facing writers |
| | `CScnCameraMan` / `CScnItemCamera*` | `0x8049EDA0+` | — | M | Scene → G3D apply path |
| **cam-collision-smoothing** | Follow floats `unk1DC…unk238` + `func_8006D450` / `func_8006D8D0` | `0x8006DEA8` / `0x8006E328` | `0x258` / `0x304` | M | Still needs body TRACE |
| **cam-scripted-override** | Factory mode `1`/`2` → `__ct__8006B310` | via `func_800784A0` | — | M | Event-camera construct path |
| | `pluginCam` key* / `shake` | `0x80061478+` | various | M | Script keyframe/script cam |
| | `CfCamEvent` `func_8006ACC0`; `CREvtCamera`; `initCamIntfInstances` | `0x8006B718` / `0x80181374` / `0x8006B3B8` | `0x650` / — / `0x98` | M | Event TU + REvt |
| **cam-battle-lockon** | Target intf + Event_1 / battle call sites | — | — | L | Still open |
| **cam-p2-clone** | Dual follow without double sim | — | — | L | Blockers: **`lbl_eu_80663DF0` 3-slot manager**, `getCurrentPad` singleton, optional `lbl_eu_80663DEC` |

### Data-flow sketch (updated Pass 2/3)

```text
CfPadTask::update → getCurrentPad / getMainGCPad  (via func_80074A3C in CfCam)
func_800784A0 (CfCamEvent_1)
  → func_80074CEC / __ct__8006B310
  → __ct__cf_CfCamFollow(subject→+0x164, aux→+0x160)
  → slots at *lbl_eu_80663DF0
  → follow update (func_8006C1C8 / giants…)
  → Direction/Lookat/Target intf set*
  → CScnCameraMan / CScnItemCamera
  → CTaskCulling / COccCulling frustum
```

**Remaining TRACE:** battle lock-on wiring; collision/smoothing body roles; P2 dual-slot policy on `80663DF0` without double-advancing follow `Move`.

---

## 4. P1 — targeting discovery

| Task id | Candidate(s) | Addr / field | Conf | Evidence |
|---------|--------------|--------------|------|----------|
| **target-current-pointer** | `CMenuEnemyState::unk830` | “last-selected target id” | H | Named in header; Move updates via `func_800B708C(id)` |
| | Actor side: +`0x3f34` pointer read in `CMenuEnemyState::Move` | — | M | Bound through `func_8016FE34()` actor |
| | `CBattleState` entry tables @ `+0x8` / `+0x1388` | vfunc6/8/10/26/29 | M | Command/entry slots; not yet proven as “current target” |
| **target-candidate-list** | `CfObjectSelectorObj` | `0x800FE190` | M | Selector object |
| | Battle actor lists `CBattleManager::mActorList1` | used by ArtsSelect / EnemyState | M | Active engagement set |
| **target-cycle-input** | `CMenuEnemyState::Move` pulse / select (`unk838`, `unk834`) | `0x80110888` | M | Target-highlight + cooldown latch |
| | Pad: `CfGameManager::getCurrentPad` in HUD paths | — | H | Same singleton as Arts |
| **target-range-los** | (unnamed) helpers near enemy panel (`func_80174C98`, vt slots `0x128/0x12C/0x258/0x260`) | — | L | Distance/state reads in Move; LoS not isolated |
| **target-world-marker** | `CMenuSymbolMark::{Move,cbRenderBefore}` | `0x8011E7E0` / `0x8011EE9C` | M | World symbol/mark HUD |
| | `CPcSelectCursor` embed @ EnemyState `+0x7E4` | — | M | Select cursor widget |
| **target-hud-lookup** | `CMenuEnemyState::Move` / `cbRenderBefore` | `0x80110888` / `0x80111240` | H | Panel walk: id @ panel+0 → `func_800B708C` → actor |
| **target-singleton-audit** | See below | | H | |

### Singleton / shared-state audit (dangerous for co-op)

| Symbol / field | Risk |
|----------------|------|
| `CfGameManager::getCurrentPad` / `getCfPadData` / `getCurrentPadChannel` | One “current” pad for HUD + likely camera |
| `CfGameManager::sUnkFlags` | Global mode bits |
| `CUICfManager::spInstance` / `CUIWindowManager::spInstance` | Single UI managers |
| `CMenuEnemyState::unk830` / `unk778` | Last target / last-cleared actor — one HUD instance |
| `CTaskCulling::getInstance` / `lbl_eu_80664328` | Single cull task |
| `lbl_eu_80663F24` (ArtsSelect ref) and similar SDA UI globals | Shared menu refs |
| `CBattleState` on the controlled actor | OK if per-actor; audit accessors that assume party leader only |

**Runtime TRACE order:** (1) log writes to `unk830` and `func_800B708C` args on target cycle; (2) xref constructors of current-target storage on `CfObjectPc` / battle state; (3) LoS/range helpers; (4) symbol-mark draw subject.

---

## 5. P1 — Blossom Dance / action prompts

| Task id | Candidate(s) | Addr | Conf | Evidence |
|---------|--------------|------|------|----------|
| **blossom-prompt-owner** | Actor binding in `CMenuArtsSelect` (`actor` vt slots in `Move`) | `Move` `0x80103D68` | M | HUD walks party/actor Arts slots; owner not yet separate “prompt owner” field |
| | Per-slot state `unk2A0[]` / `unk2C0[]` / `unk2E4[]` stage machines | same | M | Multi-stage anim state in Move |
| **blossom-pad-accessor** | `CfGameManager::getCurrentPad()` inside `CMenuArtsSelect::Move` | mid-Move ~confirm bit | H | Presses bit 23 or 10 depending on `func_80086F9C(-1)` |
| | `CfPadData` / `CfPadTask::updateCfPadData` | pad TU | H | Underlying pad buffers |
| **blossom-timing-window** | Arts anim gates via `func_80137444` + frame sets; slot readiness from vt `0x128` | Move cases | M | Timing tied to layout anim + actor float; dedicated QTE window writes not isolated |
| | `CAIAction` trailer/ring (`CAIAction_UnkVirtualFunc1/2`) | `0x8014B41C` / `0x8014B52C` | M | Action slot export/import — companion/action pipeline, not yet proven Blossom-specific |
| **blossom-stage-advance** | `CMenuArtsSelect::Move` case transitions (`0xb→…`, `0xc/0xd/0xe`, helpers `func_80106EC8` / `func_801071B8`) | — | M | Stage FSM in HUD Move |
| | `CBattleState_UnkVirtualFunc5` (large update) | `0x80146DAC` size `0x13DC` | M | Core battle confirm / Art activation TRACE target |
| | `CBattleState_UnkVirtualFunc26/8/10` | `0x80148364` / `0x801485EC` / `0x80148A18` | M | Entry inject / clear helpers (partially drafted) |
| **blossom-prompt-render** | `CMenuArtsSelect::cbRenderBefore` | `0x8010491C` size `0x3C0` | H | Per-frame Arts HUD draw (~95% CODE_MATCH) |

### Simulation-once vs per-view render

| Path | Class |
|------|--------|
| `CBattleState` / `CAIAction` virtuals, pad → action confirm in **update** | **Sim once** |
| `CMenuArtsSelect::Move` input + stage FSM | Prefer **sim once** (DECOMP_MAP §8.4); today uses global current pad |
| `CMenuArtsSelect::cbRenderBefore` | **Per-view** (or per-HUD-context) only |

### Preferred patch order (confirms DECOMP_MAP §8.4)

1. Pad accessor returns pad for **active prompt owner actor**.
2. Keep native timing / action mutation.
3. Draw prompt only in that actor’s HUD context.
4. Ensure prompt simulation does not run twice for two viewports.

**Runtime TRACE (Dunban 4 stages):** on each prompt log owner actor id, `getCurrentPad` result + B-bit, timing window fields, stage variable (`unk2C0`/`unk2A0` or battle-state entry), and whether `cbRenderBefore` vs `Move` mutated state.

---

## 6. Cross-cutting gaps

1. **`ShouldRenderSplitScreen` implemented** (`src/kyoshin/coop/PresentationGate.*`, host `presentation_gate`) — §10.4 bits; chain/movie + full-screen restore + optional Dolphin verify open.
2. **Streaming load/unload + AI distance + pools** — map id object closed (`80664184`); `mapJump`/`partyWarp` are **VM** plugins; native section loaders still TBD. Frustum ≠ render-distance.
3. **Camera follow subject factory closed** (§10.5); collision/smoothing + battle lock-on + P2 dual-slot on `80663DF0` open.
4. **True current-target pointer** on the PC object — EnemyState `unk830` is HUD mirror; see also `pluginBtl::selectTgt` / `CHelp_Target`.
5. **Blossom-specific symbols** — none named; ArtsSelect + BattleState + pad.
6. **`src/kyoshin/realtimeevt/*` / most `plugin*` bodies missing** — splits name the TUs; sources are symbol/asm only.

---

## 7. Suggested work order (next agents)

1. ~~**Ghidra xrefs** on seed symbols~~ → **done** §9.
2. ~~**Parse SDA / object layouts**~~ → **done** §10.
3. ~~**`ShouldRenderSplitScreen` predicate**~~ → **done** (`src/kyoshin/coop/PresentationGate.*`); restore stub remains.
4. **Optional runtime** spot-check of §10 bit dictionary (field / talk / REvent / vision / fade).
5. **Streaming twin experiment** on `lbl_eu_80664184` + file events; keep distance ≠ frustum.
6. **Camera P2** — dual slots on `80663DF0` + pad via `func_80074A3C` / channel, no double follow Move.
7. **Blossom pad routing** — `setPad` / per-actor pad vs `getCurrentPad`.
8. Keep FULL_MATCH matching separate.

---

## 8. Source map (files consulted)

- `TASKS.md` discovery sections; `DECOMP_MAP.md` §§8–10, 13; `PLAN.md` §10.3+
- `config/us/symbols.txt`, `config/us/splits.txt`
- `src/kyoshin/CUICfManager.{hpp,cpp}`, `CUIWindowManager.hpp`
- `src/kyoshin/cf/{CfGameManager,CfCam,CfPadTask,CTaskREvent,CVision,IFlagEvent}.*`
- `src/kyoshin/cf/object/{CBattleState,CAIAction}.*`
- `src/kyoshin/menu/{CMenuArtsSelect,CMenuEnemyState}.*`
- `src/kyoshin/cf/CTaskCulling.*`, `src/kyoshin/COccCulling.*`
- Pass 2: `build/us/asm/kyoshin/plugin/{pluginEve,pluginUi,pluginCfs}.s`, `CfGameManager.s`, `CfCam.s`, `CMenuEnemyState.s`, `CHelp_Target.s`
- Pass 3: object sizes in `config/us/symbols.txt`; bit parse → `docs/evidence/decomp/sda_flags_80663E24_28.json`; `CfCamEvent_1.s` `func_800784A0`; `CfBdat.s` map-id stores

---

## 9. Trace Pass 2 — asm xrefs (2026-07-14)

Static call-graph / body TRACE of Pass-1 seeds via retail asm under `build/us/asm/`. No Dolphin run yet.

### 9.1 Critical: plugin seeds are **script VM entry points**

Named symbols like `getMapID`, `isVisionEvent`, `realtimeEvent*`, `mesVision*`, `setMapJumpArea`, `dispLoading`, `setFieldVision` are **not** normal C callees. Bodies use `vmArg*` / `vmRetValSet` / `vmWaitModeSet` and register via `vmPluginRegist` (`pluginEveRegist`, `pluginCfsRegist`, …).

| Implication | Detail |
|-------------|--------|
| No `bl getMapID` in game code | Scripts invoke them; native code touches the **same SDA / CfGameManager helpers** the plugins wrap |
| Predicate source of truth | `lbl_eu_80663E24`, `lbl_eu_80663E28`, map id `lbl_eu_80664184`, plus `CfGameManager` workers |
| Asm label caveat | Some CfCam `bl func_8004B3F0` labels are **mis-relocs** (decoded target `0x8004BAC8` in pluginMath, not `dispLoading`) — ignore those counts |

### 9.2 Authoritative SDA / globals

| Symbol | Role | Writes (files) | Notes |
|--------|------|----------------|-------|
| **`lbl_eu_80663E24`** | Primary presentation/event bitfield | 95 stores / 15 files; top: `CfGameManager`, walkers, `CTaskREvent`, `CVision`, `pluginCfs` | `isVisionEvent` = IBM **bit 5**; `isTalkEvent` = IBM **bit 7**; realtime start ors `0x02000000` (`oris …,0x200`); `setPcCtrl` toggles `0x2000` |
| **`lbl_eu_80663E28`** | Secondary mode / loading-ish flags | 72 stores / 17 files; top: `CfGameManager`, `CfResPcImpl`, `pluginCfs` (`setFieldVision` ors `0x20000000`), `pluginEve` fade, `CTaskGameCf` | realtime start sets **bit 0**; `CMenuArtsSelect` / Talk / Vision menus gate on it |
| **`lbl_eu_80664184`** | **Current map / area id** | **only `CfBdat`** (`func_801414CC`, `func_80141C6C`) | `getMapID` → `CfGameManager::func_80086B1C` → `lwz` this global. Readers: MiniMap, FloorMap, BgTex, CUICfManager, MapSel, SymbolMark, … |
| **`lbl_eu_80661BC8`** | Current pad **channel** | `CfGameManager::setPad` only | `getCurrentPadChannel` is a direct load |
| **`lbl_eu_80663E0C`** | Pad **pointer override** | `setCurrentPadPtr`, one `CfGameManager` helper | If set, `getCurrentPad` returns it instead of slot |
| **`lbl_eu_80570D40`** | Pad object array (`×0xF8`) | — | Indexed by low 3 bits of channel |
| **`lbl_eu_80571500`** | `getCfPadData()` base | — | Shared CfPadData blob |

IBM bit *n* (MSB=0) ↔ mask `1 << (31-n)`.

### 9.3 Seed body → native worker map

| Seed | Addr | Body summary | Native worker / state |
|------|------|--------------|------------------------|
| `getMapID` | `0x8004AFAC` | VM ret of `func_80086B1C` | **`lbl_eu_80664184`** (via 1-insn thunk) |
| `isVisionEvent` | `0x8004780C` | Test IBM bit 5 of `80663E24` | Native: same SDA + `CVision` writers |
| `isTalkEvent` | `0x800477D0` | Test IBM bit 7 of `80663E24` | Dialogue predicate sibling |
| `isEvent` | `0x800477…` | (nearby pluginEve) | Broader event busy |
| `realtimeEventStart` | `0x8004722C` | Gate on `80663E24 & 0xAFA4_0000`; then `func_800862D0` + battle mgr + `func_800F4004` | **`func_800862D0`** sets `80663E24\|=0x02000000`, `80663E28\|=1`, player index 0 side effects |
| `realtimeEventPlay` | `0x800472C0` | String arg; if `80663E28&1` wait else `func_800863F4` | Play path |
| `realtimeEventEnd` | `0x8004731C` | `func_80086490` | End / clear |
| `waitRealtimeEvent` | `0x80047340` | `func_800865E8` | Wait helper |
| `mesVisionON/OFF` | `0x80046A70/AB0` | `func_80136190` + `func_8013D55C` with msg ids | UI message path (not the bit itself) |
| `setFieldVision` | `0x8004B4E8` | `80663E28 \|= 0x20000000` | Field-vision mode bit |
| `dispLoading` | `0x8004B3F0` | `func_8004302C(1,0)` | Thin wrapper to task/game loading UI |
| `setPcCtrl` | `0x8004B49C` | Toggle `80663E24` bit `0x2000` from bool | PC control lock |

### 9.4 Camera TRACE deepen

| Finding | Detail | Confidence |
|---------|--------|------------|
| **Follow subject slots** | `__ct__cf_CfCamFollow` stores ctor **r4→`+0x164`**, **r5→`+0x160`** after base/`CfCam` init | H |
| **Ctor call sites** | 4× from `func_8006C1C8` (+ related), 1× `func_80074CEC`, 1× `CfCamEvent` `__ct__8006B310` | H |
| **cam-controller-pad-read** | `func_80074A3C` (`0x800753D8`): if `func_8006BFC4` nonzero → `getMainGCPad`, else **`getCurrentPad`**. Sibling `func_80074A74` loads `pad+0xE8` and compares to 4 | H |
| **getCurrentPad** | 65 `bl`s / 40 files — HUD, Talk, MainMenu, CtrlPc, Col6, ArtsSelect, **CfCam**, CTaskGame, Chain, … | H |
| **Mislabel warning** | Do not trust CfCam→`dispLoading` reloc labels without decoding the branch | H |

**cam-follow-actor-select next:** xref who passes r4/r5 into `func_8006C1C8` / follow ctor (likely player `CfObjectMove*` from `func_80082D54(i)`).

### 9.5 Targeting + Blossom TRACE deepen

| Finding | Detail | Confidence |
|---------|--------|------------|
| **`unk830` writes** | Ctor clears (`stw 0`); **Move** updates when new id `r22 != unk830`: resolve old via `func_800B708C__Fi`, optional `func_800BBA08`, then `stw r22, 0x830`; also clear path `stw 0` | H |
| **id → object** | `func_800B708C__Fi` @ **`0x800B79AC`** size `0x70` | H |
| **Callers of id lookup** | EnemyState Move, ArtsSelect helpers, BattlePlayerState, Vision menu, KeyAssign, **CHelp_Target**, CHelp_Talk, pluginBtl, SelShop, … | H |
| **CHelp_Target** | Uses vt call then `func_800B708C` + `func_800AD860`; checks object `+0x15F0 == 2` | M — help/target UX path |
| **ArtsSelect pad** | `Move` / `func_80104454` / `func_80105D54` all call `getCurrentPad` | H |
| **Player-by-index hot path** | `func_80082D54__Fi` — **354 calls / 73 files**; ArtsSelect alone 34 — strongest “who is party member *i*” API | H |

### 9.6 Draft `ShouldRenderSplitScreen` inputs (native)

Prefer direct SDA / managers over plugin wrappers:

```text
return false if:
  CGame pause / HBM active
  (lbl_eu_80663E24 IBM bit 5)   // vision event (isVisionEvent)
  (lbl_eu_80663E24 IBM bit 7)   // talk event (isTalkEvent)
  (lbl_eu_80663E28 & 1)         // realtime-event busy (start sets this)
  (lbl_eu_80663E28 & 0x20000000) // setFieldVision
  loading UI active (func_8004302C / dispLoading path — confirm bit)
  CUIWindowManager modal queues non-empty OR CUICfManager modal mFlags
  chain-attack / pre-rendered movie (still TBD on same words or separate)
else true for NORMAL_FIELD / NORMAL_BATTLE
```

Refine bit masks via **object parse** (§10) — optional Dolphin spot-check remains.

### 9.7 Streaming TRACE deepen

| Item | Finding |
|------|---------|
| **stream-map-area-id** | Storage = **`lbl_eu_80664184`**, writers = **`CfBdat` only**; accessor thunk `func_80086B1C` |
| **Readers** | MiniMap, FloorMap, BgTex, MapSel, CUICfManager, SymbolMark, Collepedia, PartsChange — good hooks for “did map id change” |
| **Jump/warp/preload** | Still plugin-only from script; native callers of section load remain TBD via `OnFileEvent` / gimmick (`CfGimmickWarp` is a heavy `func_80082D54` user — 22 calls) |

### 9.8 Pass-2 gaps → Pass 3

1. ~~Full **bit dictionary**~~ → §10.
2. ~~Follow ctor subject~~ → §10.3.
3. Where **current battle target id** lives on the PC object (EnemyState `unk830` is HUD mirror; `CHelp_Target` + `+0x15F0` are clues).
4. Confirm **loading** native flag (not only `dispLoading` VM wrapper) — fade bits on `80663E28` partially cover presentation suppress.
5. Runtime twin streaming experiment using `80664184` + file events (optional).

---

## 10. Trace Pass 3 — parsed objects (no runtime)

Parsed `symbols.txt` object sizes + exhaustive and/or/rlwinm ops on the SDA flag words from `build/us/asm/**/*.s`. Machine JSON: [`docs/evidence/decomp/sda_flags_80663E24_28.json`](docs/evidence/decomp/sda_flags_80663E24_28.json).

### 10.1 Parsed global objects

| Symbol | Section | Size | Layout / meaning |
|--------|---------|------|------------------|
| `lbl_eu_80570D40` | `.bss` | `0x7C0` | **8× `CPad`-sized slots** (`0x7C0 / 0xF8`); `getCurrentPad` indexes with `channel & 7` |
| `lbl_eu_80571500` | `.bss` | `0x128` | **`CfPadData` singleton** (`getCfPadData`) |
| `lbl_eu_80571758` | `.bss` | `0xB8` | **`CfGameManager` instance** (matches class size `0xB8`) |
| `lbl_eu_80661BC8` | `.sdata` | `0x4` | Current pad **channel** (written only by `setPad`) |
| `lbl_eu_80663E0C` | `.sbss` | `0x4` | Optional **pad* override** (`setCurrentPadPtr`) |
| `lbl_eu_80663E24` | `.sbss` | `0x4` | Primary presentation / event **bitfield** |
| `lbl_eu_80663E28` | `.sbss` | `0x4` | Secondary mode **bitfield** |
| `lbl_eu_80663DF0` | `.sbss` | `0x8` | **Pointer** to camera-manager object (3 cam slots @ `+0/+4/+8`, mode `@+0x3C`) |
| `lbl_eu_80663DF8` | `.sbss` | `0x4` | Extra flags or’d in realtime start (`oris 0x60 / ori 0x230`) |
| `lbl_eu_80664184` | `.sbss` | `0x4` | **Map/area ID** (u32); cleared to 0 on CfBdat init; set to row index on BDAT match |

Neighbor SDA block `80663E00…80663EE0` is a dense cluster of cf/game control words (mostly untyped) — treat as one “game state blob” when auditing.

### 10.2 Bit dictionary — `lbl_eu_80663E24` (IBM bit 0 = MSB)

Curated from **set/clear owners** + named plugins (`isVisionEvent` / `isTalkEvent` / `isEvent`). Full per-bit set/clear/test lists are in the JSON.

| IBM bit | Mask | Mode (curated) | Conf | Evidence |
|--------:|------|----------------|------|----------|
| **5** | `0x04000000` | **VISION** | H | `isVisionEvent`; set `CVision::func_801A70DC`; clear `func_801A74DC` / `801A7D6C` |
| **6** | `0x02000000` | **IN_ENGINE_CUTSCENE / REvent** | H | `realtimeEventStart`→`func_800862D0` sets; `CTaskREvent::Move` sets; End/`Term`/`func_80086490` clears |
| **7** | `0x01000000` | **DIALOGUE / talk-event** | H | `isTalkEvent`; walker set/clear (`CfObjectImplWalker`) |
| **9** | `0x00400000` | **isEvent / general event** | H | `isEvent` extrwi bit 9; widely tested |
| **3** | `0x10000000` | **BATTLE active-ish** | M | `CBattleManager` set/clear |
| **18** | `0x00002000` | **PC_CTRL lock** | H | `setPcCtrl` ori / rlwinm clear |
| **0,2,4,5,6,7,8,10,13** | in `0xAFA40000` | **UI suppress composite** | H | `andis. …,0xAFA4` / `0xAFE4` in MainMenu / ArtsSelect / CTaskGame render gates (`0xAFE4` adds bit 9) |

### 10.3 Bit dictionary — `lbl_eu_80663E28`

| IBM bit | Mask | Mode (curated) | Conf | Evidence |
|--------:|------|----------------|------|----------|
| **31** | `0x00000001` | **REALTIME_EVENT_BUSY** | H | `func_800862D0` ori 1; `realtimeEventPlay` waits while set |
| **2** | `0x20000000` | **FIELD_VISION** | H | `setFieldVision` `oris …,0x2000` |
| **30** | `0x00000002` | **FADE** | H | `pluginEve` `fadeIn` / `fadeOut` |
| **19** | `0x00001000` | **KIZUNA_TALK** | H | `kizunaTalkStart` / `kizunaTalkEnd` |
| **10** | `0x00200000` | Field / GameCf mode | M | set `CTaskGameCf` + `CfGameManager`; tested across many menus |
| **14** | `0x00002000` | Pad enable (plugin) | M | `pluginPad::pad_enable` |
| **24–28** | low bytes | UI battle/cf manager marks | M | Heavy `CUICfManager` / `CUIBattleManager` Move traffic |

### 10.4 Updated `ShouldRenderSplitScreen` (from parsed bits)

**Implementation:** `src/kyoshin/coop/PresentationGate.{hpp,cpp}`  
**Host oracle:** `tools/test/compare_behaviour/host/presentation_gate.cpp`  
(not in behaviour `manifest.json` — fork-only TU, not an MWCC/objdiff unit yet)

```text
u32 a = lbl_eu_80663E24;
u32 b = lbl_eu_80663E28;

bool suppress =
    CGame paused/HBM
 || (a & 0x04000000)   // VISION
 || (a & 0x01000000)   // TALK
 || (a & 0x02000000)   // REvent / cutscene
 || (a & 0x00400000)   // isEvent
 || (b & 0x00000001)   // realtime busy
 || (b & 0x20000000)   // field vision
 || (b & 0x00000002)   // fade
 || (b & 0x00001000)   // kizuna talk
 || (a & 0xAFA40000)   // same composite MainMenu already uses
 || modal CUIWindowManager / CUICfManager mFlags
 ;

return !suppress;
```

Chain-attack / movie still need dedicated bits (likely elsewhere or unmarked E24 bits).  
`RestoreFullScreenPresentation()` is a stub until the split renderer wires view/camera/HUD/culling/GX restore.

### 10.5 Camera objects — follow subject resolved

| Object / fn | Role |
|-------------|------|
| `func_80074CEC` | Alloc `0x25C` + `__ct__cf_CfCamFollow(subject, aux)` |
| `__ct__8006B310` | Alloc `0x298` + follow-based **event** camera ctor (same subject/aux ABI) |
| `func_800784A0` (`CfCamEvent_1`) | **Only creator** of both; called from `CfCamEvent_1` + `CfGameManager` |
| Mode `r28` | `0` → `func_80074CEC`; `1`/`2` → `__ct__8006B310`; stored at `(*lbl_eu_80663DF0)+0x3C` |
| Subject (`→ follow+0x164`) | Factory arg0 = `r22` (event/camera owner in `func_800784A0`) |
| Aux (`→ follow+0x160`) | `func_80496264(func_80061FD8(), 0)` — pose slot 0 |
| Fallback actor | vt+`0x4C` on owner → id → `func_800B708C__Fi`; else **`func_80082D54(1)`** (party index 1) |
| Active cams | 3 slots walked at `*lbl_eu_80663DF0 + 0/4/8` |

**Note:** Intra-`CfCam` `bl func_8006C684` labels are **EU name collisions** — decoded targets are tiny helpers at `0x8006D0DC` (flags on subject `+0x530`), **not** the follow ctor.

### 10.6 Map-id object write path

`CfBdat` init zeroes `lbl_eu_80664184`; later BDAT string-column scan stores matching **row index** (`r29`) into it. That is the authoritative stream-map-area-id object.

### 10.7 Pass-3 gaps

1. Name remaining E24/E28 bits (JSON has owners; semantics still M/L).
2. PC-object field for battle target (beyond HUD `unk830`).
3. Chain / movie dedicated flags.
4. Optional Dolphin verify of §10.2–10.4.
