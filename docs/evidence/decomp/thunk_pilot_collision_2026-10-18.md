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

## Wave 2 (2026-10-18): tooling fix + generalized sweep + member batch 2

**Tooling (`symbol_fn_rename.py`; tooling-only, tree untouched):**
- Fixed `_split_top_level`: it special-cased `<>`, so `->` drove angle-depth
  negative and the top-level comma never split
  (`effectArray[i]->mSchedule, lbl` -> 1 part), which corrupted 2 real call sites
  with dropped arguments. Now splits on `()`/`[]`/`{}` depth only, literals
  respected. Verified: repro -> 2 parts; `a->b(x,y)`, `f(g(a,b),c)`,
  `f(std::pair<int,int>)`, `(CSchedule*)x, y` all correct.
- Added in-class member-declaration insertion (exactly one class body required,
  section chosen by surrounding members, idempotent, `--no-header-insert`
  escape hatch) and a guard so file-scope qualified declarations (MWCC 10127)
  become `review` items instead of being auto-rewritten.
- Tests 25 -> 44 in-file; full suite **584 OK**.

**Thunk sweep — precise NEGATIVE result (no new lands).** 183 `*Thunk*` sites
(176 size-`0x8`, all `addi r3,r3,-NN; b target`). Only 4 `.o` contain both an
emitted `W @NN@` adjustor and a hand thunk, and none is cleanly deletable:
`CMenuBattlePlayerState` (deferrable; TU not tiled), `CScn` (vtable pins the
historical name; needs a frozen-`UNIT_RULES` spelling), `CMenuSkipTimer` (retail
has no `@88@__dt__` by design), `CUIWindowManager` (genuine non-virtual helper).
Everything else is either a genuine non-virtual `container_of`/forwarder (MWCC
never emits those) or needs per-class fake-single-inheritance -> real-MI rework.
Conclusion: stage 1's IScnRender family was the recoverable prize; the remaining
~180 thunks are legitimately hand-written.

**Member batch 2 — 6 landed, byte-identical:**
`Sched_RebuildHandles` -> `CSchedule::RebuildHandles` (100%),
`Sched_Init` -> `CSchedule::Init` (86.7%, same as baseline),
`schedLoadItemSlots/StepItemSlots/TickItemLifetime/FeedDistance` ->
`CScheduleItem::LoadSlots/StepSlots/TickLifetime/FeedDistance` (1.8%/33.3%/100%/100%).
Per-unit `hexdiff --all` unchanged except the renamed rows; all units split PASS;
registry synced (19300); includer sweeps (`ScheduleList`, `code_804DEDA8` 64 fns,
`code_804C8718` 34 fns) byte-identical. Refused: `schedClearItemSlots`,
`schedReleaseItemSlots`, `schedInitItemFromData` (arity/type drift),
`schedReleaseItemObjects`, and the 6 `sched*` with stale `void*` declarations in
kyoshin headers -- all NEEDS-REVIEW, none forced.

**Running total: 9 functions member-ized; CMenuBattleMode split FAIL -> PASS.**

## Policy note: bulk member-ization is an explicit maintainer override (2026-10-18)

`SKILL.md` (declaration-drift convergence path, step 6) says the member-ization /
`void* self` structural fix is "matching-neutral -- **never a campaign**", i.e. it
is meant to be retired opportunistically while a TU is being verified for its
matches, not driven as a batch.

A bulk wave over the auto-appliable member candidates was nonetheless**
**explicitly authorized by the maintainer** ("Override") after this conflict was
raised. It is therefore sanctioned here, under these invariants:

- byte-identity gate per unit (address, size, `instruction_match` unchanged; a
  renamed row may differ in name only), with automatic per-unit revert;
- line-neutral edits (preserve `-ipa file` line numbers);
- per-unit includer sweep (`grep -rl <header> src libs`) and `hexdiff --all`;
- `targets sync-symbols` + `.ctx.c` regeneration via the sanctioned commands;
- method NAMES are reviewed (they are source-fidelity choices, codegen-neutral).

This is a documented exception for this tree at this time, not a change to the
standing policy: future agents should still treat member-ization as
matching-neutral cleanup unless the maintainer re-authorizes a campaign.

## Bulk wave 1 result (2026-10-18, driver `tools/coop/memberize_bulk.py`)

Driver built (23 tests; suite 629 OK). Live `plan`: 796 unique candidates ->
185 CLEAN (2 pruned: primitive `char::` classes), 126 NEEDS-REVIEW, 485 blocked.

`apply --execute` over 181 CLEAN candidates, 53 units: **27 units PASS / 26 FAIL,
64 functions member-ized**; all PASS units verified byte-identical (spot-checked
`hideTalkList__15CKizunaTalkListFv`, `OnFwdGate__10CSkipTimerFv`,
`CallLightV26__14CScnEnvLgtCtrlFPvPv` at 100%, splits PASS, registry valid 19300).
Every FAIL unit was reverted cleanly (verified: `CItemBoxGrid` still
`extern "C" ShowCellIcon`). Session total: **77 functions member-ized**.

FAIL triage (all safely reverted):

- 14x post-snapshot `hexdiff` failed (`SPLIT config/us/config.yml`) -- collision
  with the other orchestration's concurrent `configure.py` run.
- 9x "symbol vanished after rename" -- driver comparator false negative (it
  compares by name rather than address); fix in flight.
- 3x baseline build failed -- pre-existing, unrelated to this work.

**Finding that justifies the gate:** `CColiProc` showed `ColiProc_InitQuad`
`decomp_size 24 -> 40`, `mismatch 0 -> 10` -- renaming an `extern "C"`
free function into a member altered `-ipa`/inlining and shifted codegen. So
member-ization is *usually* but NOT universally byte-neutral; the per-unit
gate is load-bearing, not ceremonial.

Remaining: fix the comparator (address-keyed rename pairing) and retry the 23
recoverable units one at a time; leave the 3 pre-existing build failures. 41 of
the landed names keep a partial prefix (`CErrMesSub::Win_UpdateSub`,
`CMdlDynamics::Dyn_SolveContacts`) -- valid and byte-neutral, just less clean.
