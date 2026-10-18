# extern "C" member-candidate pilot — verified plan, no execution (2026-10-18)

Scope: the 8 bounded files only (`ScheduleList.cpp`, `CScnBloom.cpp`,
`code_8047D2AC.cpp`, `code_8047BB54.cpp`, `CfMapItemManager.cpp`,
`code_8018F8D8.cpp`, `CActParamAnimGame.cpp`, `code_804B59C8.cpp`).
Prior buckets (`invented` ~14, `retail-drift` 34) already mined out; what remains
here is the `void* self` / hand-spelled-mangled population. Methods: `extc.py
scan/plan/member-check/header-drift`, `targets.json` status cross-check,
caller grep (excluding generated `.ctx.c`), live `hexdiff --all` baselines.
**Zero tree edits were made** (STOP branch below); every table below is the
pre-existing baseline, byte-identical by virtue of no change.

Approach key — (a) SYMBOL-IDENTICAL: hand-spelled mangled name moved to the
owning header + `#include`, line-neutral, no rename. (b) MEMBER CONVERSION:
short-name `Self* self` free fn → `Class::method`, changes the emitted symbol,
needs a function-level rename across `config/*/symbols.txt` + all caller TUs.

## Per-class plan table

| Class / cluster | Symbol | Current form | Target form | a/b | Body status (registry / live) | Blast radius | Action |
|---|---|---|---|---|---|---|---|
| SLList (`SchedList_*`, ScheduleList.cpp) | SchedList_StepAll | `void SchedList_StepAll(SLList*,f32)` (C-linkage via fwd decl) | `static StepAll` in `SLList` → `StepAll__6SLListFP6SLListf` | b | FULL_MATCH / 100% ✓ | TU-local only; vtable `lbl_eu_80570078` holds `&Fn` | **BLOCKED** — instance-(b) breaks plain-address table (member-pointer); static-(b) needs function rename, no tool support (see §Why) |
| ^ | SchedList_SweepAll | same pattern | same | b | FULL_MATCH / 100% ✓ | TU-local only | BLOCKED (same) |
| ^ | SchedList_SweepMatched | same pattern | same | b | FULL_MATCH / 100% ✓ | TU-local only | BLOCKED (same) |
| ^ | SchedList_DropItem | same pattern | same | b | FULL_MATCH / 100% ✓ | TU-local only | BLOCKED (same; member-check: possible virtual) |
| ^ | SchedList_DropByField14 | same pattern | same | b | FULL_MATCH / 100% ✓ | TU-local only | BLOCKED (same; possible virtual) |
| ^ | SchedList_DropItem-family cont. | — | — | — | — | — | SchedList_ClearMatchTags, TallyItems, RingHead (`u8* self`), TeardownAll, StepBoth, AdvanceBoth, FindBoth, RouteAddReq, DropBoth, TallyBoth: all FULL_MATCH/100% live, all TU-local — same BLOCKED verdict; SchedList_AddItem STRUCTURAL and `__dt__12ScheduleListFv` CODE_MATCH excluded (not accepted) |
| UnkClass_8047BB54 (code_8047BB54.cpp) | forwardMpfCallA/B/C, flushMpfBuffer, setMpfFloatParam (`__...Fv`, `u8* self`) | `extern "C"` hand-spelled defs | keep defs; (a) decl-consolidation to header | a decl-only | FULL_MATCH / 100% ✓ (all five) | 6+ caller TUs (CfObjectMap.hpp/cpp, CCol6System, code_80135FDC, CfGameManager, CTaskREvent, pluginCfs) with signature drift | **BLOCKED** — member-check: NOT members (r3 never deref'd); Fv-with-hidden-params must stay `extern "C"`; fanout = campaign |
| ^ | initMpfSystem / initMpfDrawBuffer / `__dt__8047BC74` / checkMpfFlags / getMpfStatus / resetMpfInstance | `extern "C"` hand-spelled | keep (Fv hidden params or vtable evidence) | a decl-only | FULL_MATCH / 100% ✓ | same fanout | BLOCKED (same) |
| UnkClass_8047E064 (code_8047D2AC.cpp + UnkClass_8047CD0C.cpp) | resetScnState / initScnDefaults | already `UnkClass_8047E064::` members | — | done | FULL_MATCH / 100% ✓ | — | DONE (prior work; proves self-only hand-spelled → member is symbol-identical) |
| ^ | attachScnNode__17UnkClass_8047E064Fv | def `(self,u16)`; caller decl `(self,u32)` | (a) fix caller decl to `u16` | a | FULL_MATCH / 100% ✓ | def TU + UnkClass_8047CD0C.cpp (2 call sites pass s32 loop var → `clrlwi` codegen risk) | **BLOCKED** — 1 symbol (<3), caller TU out of scope, width (not spelling) drift |
| UnkClass_8047D2AC (code_8047D2AC.cpp) | dispatchScnCommand / updateWalker (`__...Fv` + extra params) | `extern "C"` hand-spelled defs | keep defs | a decl-only | FULL_MATCH / 100% ✓ | CtrlMovePC.cpp decls conflict in arity+type | **BLOCKED** — ABI reconciliation campaign, out of scope |
| ^ | solveWalkStep / solveWalkPair / findWalkRoute / runWalkerTick | same | — | — | STRUCTURAL/HIGH (not accepted) | — | BLOCKED (acceptance) |
| CScnBloom (CScnBloom.cpp) | CScnBloom_destroyAdj48 / renderBloomAdj48 | `extern "C"` short-name fwd decl + def, TU-local | `CScnBloom::` members (rename) | b | FULL_MATCH / tiny thunks 0x8 ✓ | TU-local only (vtable slot use in same TU) | **BLOCKED** — only 2 symbols (<3); single-class follow-up candidate |
| ^ | InitBase/InitParams/RenderFiltered/ExtractBright/RenderCached/… | same file | — | — | STRUCTURAL/HIGH/COMPILES (not accepted; live table ahead of registry in places) | — | BLOCKED (acceptance) |
| CfMapItemManager.cpp | Bdat_GetRowBase_B41C / Bdat_GetMaxRow_B1EC (decls; defs ocBdat.cpp FULL) | cross-TU imports | (a) header consolidation | a | FULL_MATCH ✓ (def TU) | dozens of kyoshin callers | **BLOCKED** — fanout campaign |
| ^ | hikariSetEnableBit0 / hikariRemoveRecordTarget / ClearMenu_GetGlobal10 (decls; defs CfHikariItemManager.cpp / CMenuGameClear.cpp FULL) | cross-TU imports | (a) header consolidation | a | FULL_MATCH ✓ (def TUs) | 2 files; user `setItemEnable` live-100% (registry COMPILES) | **BLOCKED** — defs out of scope; 2–3 symbols but split across owner TUs |
| ^ | updateMapItems / stepItemNotch / advanceItemTimer / testActorStatus | `extern "C"` free fns | (b) members (rename) | b | COMPILES / low live % (not accepted) | — | BLOCKED (acceptance) |
| CActParamAnimGame.cpp | probeStandGnd / snapActorDown / trackGroundAt / applyFallMove / runBattleMove / pushYawToLink | `extern "C"` free fns, retail unmangled C-ABI names per TU comment | (b) members (rename) | b | STRUCTURAL/HIGH/CODE (none FULL) | — | BLOCKED (acceptance; retail names are C-ABI globals, conversion validity unproven) |
| Coli* (code_804B59C8.cpp) | ColiQuery_Reset / Coli_StoreRange / Coli_IsHeightInRange_7044 / Coli_IsUnderCeiling_7074 / Coli_Noop_* | `extern "C"` free fns in dispatch table | (b) members (rename) | b | several FULL_MATCH ✓ | heterogeneous self-types (`u8*`, `CColiMoverState*`, `CColiObj*`, …); Coli_StoreRange also called from code_804BAE10.cpp; table holds `&Fn` | **BLOCKED** — not one class; table breaks instance-(b); cross-TU caller |
| code_8018F8D8.cpp | PartyStateWin_OpenWindow | `extern "C"` free fn | (b) member (rename) | b | HIGH_MATCH (not accepted) | — | BLOCKED (acceptance) |
| CSchedule decls (ScheduleList.cpp:17-28) | schedReleaseAllItems / `__ct__CSchedule` | caller copies of CSchedule.cpp/code_804E36DC.cpp defs | (a) delete if header-identical | a | FULL_MATCH ✓ | CfObjectImplEne.hpp:744 declares `schedReleaseAllItems(void*)` — conflicting spelling | **BLOCKED** — only 2 symbols; cross-header conflict = campaign |

## Why no pilot was executed (STOP branch)

1. No class offers 3–8 all-accepted symbols with a pure approach-(a) edit inside
   the bounded set. Every (a)-viable cluster fails on count (<3), mixed
   acceptance, cross-TU width/arity drift, or 6+-TU fanout.
2. The one 3–8 all-accepted single-TU cluster (SchedList octet, all FULL_MATCH,
   zero external refs) is approach-(b)-only (short names), and (b) is not
   executable here: instance members break the hand-built plain-address vtable
   (`(u32)&Fn` vs member-pointer); static members need a function-level
   `symbols.txt` rename, but `symbols rename-all` is type-oriented and
   hand-edits to `config/*/symbols.txt`/`targets.json` are forbidden. Three of
   eight are additionally flagged possible-virtual, so the true shape may be
   virtual members (a full vtable-class reconstruction — a campaign, not a pilot).
3. Per the brief ("If approach (b) is unavoidable and you are not fully
   confident, STOP and write the plan instead"), the pilot was stopped and the
   plan above is the deliverable. No file outside the two deliverable paths was
   touched; every `hexdiff --all` table below is the unmodified baseline.

## Exact commands (all read-only; reproduce the survey)

```sh
.venv/bin/python3 tools/coop/extc.py scan --json /tmp/extc_scan.json
.venv/bin/python3 tools/coop/extc.py plan SchedList        # + CSchedule ScheduleList CScnBloom CScnFilter CActParamAnimGame CfMapItemManager CfMapItem CColiMover CColiObj UnkClass_8047BB54 UnkClass_8047D2AC UnkClass_8047E064
.venv/bin/python3 tools/coop/extc.py member-check SchedList_StepAll SchedList_SweepAll SchedList_SweepMatched SchedList_DropItem SchedList_DropByField14 SchedList_ClearMatchTags SchedList_TallyItems SchedList_RingHead
.venv/bin/python3 tools/coop/extc.py member-check forwardMpfCallA__17UnkClass_8047BB54Fv forwardMpfCallB__17UnkClass_8047BB54Fv forwardMpfCallC__17UnkClass_8047BB54Fv flushMpfBuffer__17UnkClass_8047BB54Fv setMpfFloatParam__17UnkClass_8047BB54Fv initMpfSystem__17UnkClass_8047BB54Fv initMpfDrawBuffer__17UnkClass_8047BB54Fv __dt__8047BC74
.venv/bin/python3 tools/coop/extc.py header-drift UnkClass_8047BB54   # + CScnBloom CSchedule ScheduleList CScnFilter CfMapItemManager (all: 0 drift)
.venv/bin/python3 tools/coop/hexdiff.py "monolib/src/core/ScheduleList" --all
.venv/bin/python3 tools/coop/hexdiff.py "monolib/src/scn/CScnBloom" --all
.venv/bin/python3 tools/coop/hexdiff.py "monolib/src/scn/code_8047D2AC" --all
.venv/bin/python3 tools/coop/hexdiff.py "monolib/src/mpfsys/code_8047BB54" --all
.venv/bin/python3 tools/coop/hexdiff.py "monolib/src/coli/code_804B59C8" --all
.venv/bin/python3 tools/coop/hexdiff.py "kyoshin/cf/CfMapItemManager" --all
.venv/bin/python3 tools/coop/hexdiff.py "kyoshin/cf/code_8018F8D8" --all
.venv/bin/python3 tools/coop/hexdiff.py "kyoshin/cf/CActParamAnimGame" --all
```

## Baseline tables (live, unmodified — 2026-10-18)

- `monolib/src/core/ScheduleList`: 16/21 matched; octet all 100% (StepAll,
  SweepAll, SweepMatched, DropItem, DropByField14, ClearMatchTags, TallyItems,
  RingHead); AddItem 44.8% STRUCTURAL; dtors CODE/HIGH. Split PASS (0xC0 spare).
- `monolib/src/scn/CScnBloom`: 11/15; destroyAdj48 + renderBloomAdj48 thunks
  100%; RenderFiltered 5.8%, ExtractBright 4.4%, RenderCached 32.3%,
  RenderHalfRes 93.8% (registry lists several of these as HIGH — stale).
- `monolib/src/scn/code_8047D2AC`: 5/9; updateWalker, dispatchScnCommand,
  attachScnNode, initScnDefaults, resetScnState all 100%.
- `monolib/src/mpfsys/code_8047BB54`: 14/18; forwardA/B/C, flushMpfBuffer,
  setMpfFloatParam, initMpfSystem, initMpfDrawBuffer, `__dt__8047BC74`,
  checkMpfFlags, getMpfStatus, resetMpfInstance all 100%.
- `monolib/src/coli/code_804B59C8`: 20/61 matched.
- `kyoshin/cf/CfMapItemManager`: 11/24; setItemEnable 100% live (registry:
  COMPILES — stale).
- `kyoshin/cf/code_8018F8D8`: 35/42 matched.
- `kyoshin/cf/CActParamAnimGame`: 16/33; member-candidate bodies STRUCTURAL or
  worse throughout.

## Follow-ups (not done here)

1. Cleanest future (a)-micro-fix: `attachScnNode` caller decl `u32`→`u16` in
   `UnkClass_8047CD0C.cpp:90` + `hexdiff monolib/src/scn/UnkClass_8047CD0C
   --all` (watch the two call sites' `clrlwi` codegen; caller TU out of this
   brief's scope).
2. If function-level rename tooling ever exists: SchedList static-(b) octet
   (single TU, zero external refs) is the bounded pilot; keep instance-(b) off
   the table while `lbl_eu_80570078` holds plain addresses.
3. Registry is stale ahead/behind live for CScnBloom/CfMapItemManager units
   (not caused here; flagged, not fixed — `smell_report --write` not run).
