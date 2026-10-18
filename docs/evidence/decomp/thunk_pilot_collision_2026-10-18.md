# Thunk-recovery pilot — shared-workspace collision + recovery (2026-10-18)

## What happened

The `thunk-pilot` subagent (class-hierarchy recovery for the `*Thunk*`/
`container_of` raw-offset family) **errored mid-run** (model `server_error`) after
making unverified structural edits, and the workspace was simultaneously being
edited by a **second, independent orchestration** (`paseo` agent `51370f71`,
"Can you start subagents to decompile the remaining TUs?", waves 27-31). Several
apparent "regressions" seen while triaging were that orchestration's in-progress
MATCH work, not this pilot's.

## The pilot's WIP (preserved at `/tmp/thunk_pilot_WIP/`, reverted from the tree)

Hypothesis: the `IScnRender` render-callback virtual is really `cbRenderBefore`
(evidenced by MWCC emitting `@88@cbRenderBefore__...` adjustor thunks — MWCC names
adjustors after the overridden virtual), so declaring it that way lets MWCC emit
the `+0x5C` adjustors itself and the hand-written thunks can be deleted.

Edits it made (unverified when it errored):

| file | change |
|---|---|
| `libs/monolib/include/monolib/scn/IScnRender.hpp` | rename `handleEmptyRender` -> `cbRenderBefore`; comment rewrite |
| `src/kyoshin/CTaskGame.cpp` | `IScnRender::handleEmptyRender()` def -> `IScnRender::cbRenderBefore()` |
| `src/kyoshin/menu/CMenuBattleMode.cpp` | deleted hand-written `CMenuBattleMode_renderBeforeAdj5C` / `_dtorAdj5C` thunks (expect MWCC `@92@...` adjustors) |
| `configure.py` | added a `link_transform` to the `kyoshin/CTaskGame.cpp` object |

## Recovery

Backups existed (`/tmp/thunk_pilot_backup/`), so all four files were restored to
the pre-edit state (WIP preserved separately). Verification after revert:

- `CMenuBattleMode` split FAIL 0x10 -> **0x8** (all non-ctor functions 100%).
- `CTaskGame` split FAIL 0x574 -> **0x564** (`__ct__9CTaskGame...` 100%).
- `__ct__CMenuBattleMode` stays at 14.3% **after** the revert, so that is not
  attributable to the pilot (registry says HIGH_MATCH 87.5 -> stale registry or
  another orchestration's edit).

Separately, a **build-blocking** bug was fixed: an em-dash (`U+2014`) in a
comment in `libs/monolib/src/device/CDeviceFontInfoExt.cpp` (written by the other
orchestration) broke `sjiswrap`; replaced with `--`. That unit now builds and its
split PASSes.

## Consequence / policy

Do **not** run class-hierarchy / shared-header structural waves while another
orchestration is editing the same workspace — they collide on base headers
(`IScnRender.hpp` is included by 225 TUs) and on `configure.py`. The pilot's
hypothesis is still worth a **verified** attempt once the workspace is quiescent:
declare `cbRenderBefore` in `IScnRender`, delete the hand-written adjustor thunks,
and check the emitted `@NN@...` symbols + `hexdiff --all` per includer before
touching `configure.py` (the `link_transform` may not be the right mechanism —
not verified).

## Tools that are safe and done

- `tools/coop/layout_audit.py` (+12 tests) — header struct-layout bugs.
- `tools/coop/symbol_fn_rename.py` (+25 tests) — function-symbol rename /
  member-ization, dry-run planner + gated apply.

Both are self-contained under `tools/coop/` and do not edit the tree.

## Resolution (2026-10-18, verified)

**Stage 1 — thunk recovery: hypothesis TRUE, landed.** MWCC names adjustor thunks
after the overridden virtual, so declaring `IScnRender::cbRenderBefore` (slot 1)
with a separate non-virtual `handleEmptyRender` makes MWCC emit the adjustors at
the retail addresses itself:

```
@92@cbRenderBefore__15CMenuBattleModeFv   subi r3,-92 ; b ->cbRenderBefore
@88@cbRenderBefore__Q22cf12CTaskCullingFv subi r3,-88 ; b ->cbRenderBefore
```

- `CMenuBattleMode`: split FAIL 0x8-over -> **PASS (0x654/0x654)**; gained
  `dtorAdj5C` at 100%; `renderBeforeAdj5C`/`dtorAdj5C` map to the emitted thunks.
- `CTaskCulling`: `@88@cbRenderBefore` 100%; +1 function.
- `CTaskGame`: `handleEmptyRender__10IScnRenderFv` stays 100% (0x800444C0 blr).
- Includer sample unchanged (CMainMenu 29/31, CQuestWindow 26/30,
  CTaskGameEff 25/28, CMenuQuestLog 22/23, CMenuTutorial 13/14).
- `configure.py` `link_transform` on the `CTaskGame` object keeps the historical
  `handleEmptyRender__10IScnRenderFv` name in the linked map; it is a **supported
  mechanism** (`tools/project.py:1150`, 12 uses in `configure.py`), not invented.

**Stage 2 — member-ization: 3 symbols landed, code-neutral.**
`schedUpdate`/`schedStepEntries`/`schedFeedTargetDist` ->
`CSchedule::Update`/`StepEntries`/`FeedTargetDist` in `code_804E36DC.cpp`.
Companion edits: +3 in-class declarations in `CSchedule.hpp`; 4 bare qualified
file-scope declarations demoted to comments (a bare `CSchedule::Update(f32)` at
file scope is MWCC 10127 — it broke `CMenuBattleMode.o` mid-flight and was fixed);
2 real out-of-unit callers repaired (`CVision.cpp:3221`, `CfObjectImplEne.cpp:374`).

Per-unit after: each renamed symbol matches at its exact baseline (`Update`
77.9% 0x30c, `StepEntries` 100% 0x120, `FeedTargetDist` 28.4% 0x160/0x158);
`ScheduleList`/`CVision`/`CfObjectImplEne` zero diff; all four units split PASS.
Registry re-synced (19300 targets, match states preserved).**
