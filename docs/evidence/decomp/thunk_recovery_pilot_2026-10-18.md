# Thunk-recovery pilot — 2026-10-18

## Family chosen

**CMenuBattleMode IScnRender@0x5C pair** (`CMenuBattleMode_renderBeforeAdj5C`
@0x801A1BB0 + `CMenuBattleMode_dtorAdj5C` @0x801A1BB8, both FULL_MATCH /
ACCEPTED): 2 hand-written `extern "C"` adjusting thunks (`subi r3,-0x5c; b`),
single target class (`CMenuBattleMode`), known secondary base (`IScnRender`
at +0x5C; `IWorkEvent` at +0x58 already emits a compiler `@88@` thunk).
The hierarchy was already declared
(`CMenuBattleMode : CMenuBattleModeBase, IWorkEvent, IScnRender`); only the
base virtual's true name was missing.

## Recovered hierarchy (the TRUE one)

```cpp
class IScnRender {
    virtual ~IScnRender() {}
    virtual void cbRenderBefore();   // was misnamed handleEmptyRender
};
class CMenuBattleMode : public CMenuBattleModeBase, public IWorkEvent,
                        public IScnRender { ... virtual ~CMenuBattleMode();
    void cbRenderBefore(); /* now a real override */ };
```

## Evidence

- **Name-match rule (probe)**: a derived member named differently from the
  base virtual gets NO adjustor thunk (scratch probe2); CTaskCulling's
  in-tree `@88@cbRenderBefore__Q22cf12CTaskCullingFv` therefore PROVES the
  +0x58/+0x5C base declares `cbRenderBefore`. The old `handleEmptyRender`
  spelling was a misnomer (its blr body at 0x800444c0 is the default impl).
- **Emission rule (probes)**: MWCC emits MI adjustor thunks only together
  with the class vtable (key function); `novtable` kills both; thunks land
  at end of `.text` as `@<dec-offset>@<mangled>`, weak when the key is
  absent; within a secondary, non-dtor overrides emit in reverse declaration
  order with the dtor thunk last — exactly retail's order.
- **Retail proof**: the emitted `__vt__15CMenuBattleMode` (0xC0) reproduces
  retail block `lbl_eu_80532DD0` slot-for-slot (primary
  dtor/Reset/Init/Term/Move/Draw/Tail, IWorkEvent secondary with 31 direct
  slots + `@88@` dtor thunk, IScnRender secondary
  `[RTTI,-92,@92@dtor,@92@cbRender,cbRender-direct]`); emitted `@92@`
  thunks sit at the exact retail offsets (0x644/0x64c in-object) with exact
  bytes (`subi r3,r3,92; b` + reloc).
- **No dtor churn**: the dtor stays hand-written `extern "C"` (still
  FULL_MATCH); the key function stays undefined-as-member, so no vptr
  stores appear; member Init/Term/Move/cbRenderBefore bodies are untouched.

## Changes (4 files, all line-neutral)

- `libs/monolib/include/monolib/scn/IScnRender.hpp`: `handleEmptyRender` →
  `virtual void cbRenderBefore()` (12 lines = 12 lines).
- `src/kyoshin/menu/CMenuBattleMode.cpp`: deleted the 2 hand Adj5C thunks
  (+ line-neutral padding).
- `src/kyoshin/CTaskGame.cpp`: base-impl definition renamed to
  `IScnRender::cbRenderBefore` (2 lines = 2 lines; same blr bytes/address).
- `configure.py`: `link_transform` rename
  `cbRenderBefore__10IScnRenderFv → handleEmptyRender__10IScnRenderFv` for
  kyoshin/CTaskGame (CHelp/CBgTex idiom; keeps registry/linked names stable).

## Verify

- BattleMode `hexdiff --all`: 8/9 identical (ctor still 14.3% ACTIVE);
  split FAIL+8 → PASS exact (0x654=0x654). Both deleted thunks pair with the
  weak compiler thunks at 100% (same `-92` bytes; reloc names ignored).
- CTaskGame: 102/113 (1 intended n/a: renamed base impl, linked-identical
  via the rename; re-pair by offset).
- Sweep (~35 TUs: all MI-header includers + vtable emitters + thunk
  families): NO accepted-function regressions; NO split PASS→FAIL flips
  (every FAIL pre-existed or exceeds the +24 B weak-addition bound;
  CGame/CMenuUpdate/Tutorial/EnemyState/SysWinSave re-baselined identical).
  One rename-caused regression found and fixed in-sweep:
  `BpsStateRenderThunk5C` (member call virtualized to bctrl) rewritten as a
  free-pointer direct call — back to 100%, no metric change.
- Bonus: `MiniMap2RenderThunk5C` (was COMPILES/0%) and the Zeal thunks now
  verify 100% with no source change.

## Known side effects (documented, ungated)

- MWCC now also emits a (correct, retail-identical) strong `__vt__` +
  weak RTTIs in MI TUs (+0x190 `.data` for BattleMode) — no hexdiff-text or
  split impact; linked-image/data placement is tooling-limited (see below).
- `smell_report --check` freshness fails (thunk sites removed — the point of
  the pilot) plus 3 unrelated upstream-drift regressions
  (CMdlDynamics/code_80135FDC/pluginSnd — untouched files, no IScnRender
  include). Human must run `--write` (forbidden to agents).
- Tutorial/QuestLog `virtual void handleEmptyRender()` decls are now
  dangling new-virtuals (their vtables are suppressed; verified no .o
  change) — follow-up cleanup, not pilot scope.

## Generalisation notes (which families this unblocks)

- The whole 58/5C/5C idiom (Pause, GetItem, SymbolMark, MainMenu,
  BattlePlayerState, EnemyState, BattleDamage, TalkWindow, SysWinBuff,
  MakeCrystal, PTState, MapSelectSC, Tutorial/List, Zeal, ArtsSet, ...):
  same rename already applies; delete hand thunks where `.text` tiles
  exactly (BattleMode pattern). Where it doesn't tile, hand (named) thunks
  still pair better than weak ones (BattlePlayerState lesson: keep named
  hand thunks until the TU tiles, then re-delete).
- 6C/70 idiom (SysWinSelect/Save/ScenarioLog/Col6Invite/EveTalkWin/...):
  same mechanism; secondaries are IWorkEvent-like + IScnRender-like.
- NOT MI-expressible, excluded: `CModelDispMakeCrystal` release-thunk
  (adjusts `-8` then calls the free function `MakeCrystal_ClearAnimSlot` —
  not a virtual override; hand adapter, keep).
- Unblock recipe for full data promotion: a shrink-compatible path to place
  MWCC-emitted `__vt__`/`__RTTI__` at retail `lbl_eu_*` addresses for NEW
  units (today only UNIT_RULES does this, and it is frozen for new keys;
  `link_transform` renames names but not placement). Until then, promotion
  of converted TUs needs a policy exception.
- Follow-ups: human `rename-all handleEmptyRender → cbRenderBefore`
  (+ drop the Tutorial/QuestLog dangling decls); `smell_report --write`;
  re-attempt member-dtor conversion (needs `~CTask` specialization +
  inline-empty `~IWorkEvent` to match retail's reduced dtor — evaluated as
  infeasible without layout shift for now).
