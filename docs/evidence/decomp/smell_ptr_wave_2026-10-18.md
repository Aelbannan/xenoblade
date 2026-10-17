# Code-smell pointer-arithmetic cleanup wave — 2026-10-18

Byte-identical (matching-neutral) cleanup of the `docs/CODE_SMELLS.md`
**"Actionable pointer arithmetic"** backlog: raw offset expressions inside
already-`FULL_MATCH`/`EQUIVALENT_MATCH` functions replaced with typed member /
struct-field access. Types are erased at codegen, so a layout-correct rewrite
compiles to the same object code; **proof for every file is that the unit's
`hexdiff --all` table is byte-identical before/after.**

All work ran via paseo subagents in the shared workspace
(`wks_97ed1b37ab43aec7`), provider `pi`, model
`opencode-go/muse-spark-1.3-contributor`. Each agent owned a disjoint set of
files; none edited `tools/coop/targets.json`, `docs/CODE_SMELLS.md`, or added
`extern "C"`/`void*`/`self` params (see *Disclosures*).

## Headline numbers (live `smell_report.py` scan, final)

| metric | before | after | Δ |
|---|---|---|---|
| `ptr_sites` (all expressions) | 9102 | 8623 | **-479** |
| `ptr_sites` inside accepted fns | 940\* | 623 | **-317\*** |
| `ptr-arith` (line proxy) | 6294 | 5954 | **-340** |
| `deref-arith` (line proxy) | 2599 | 2459 | **-140** |
| `void*` | 7938 | 7931 | -7 |
| extern "C" decl / def | 8804 / 5108 | 8804 / 5108 | 0 |
| `self` (see caveat) | 13712 | 13765 | +53\* |

\* The `940` accepted total was produced by the old **bare-last-name** accepted
lookup, which over-counted (e.g. 22 `CUICfManager::Move` HIGH_MATCH sites
counted as accepted via the accepted `CTTask<12CUICfManager>::Move`, and 5
`CArrow3D::cbRenderBefore` STRUCTURAL sites via `CMenuSymbolMark::cbRenderBefore`).
`tools/coop/ptr_sites.py` was fixed to match **class-qualified** definitions
(below); the corrected series is not directly comparable to 940. Agents
converted **~465 textual sites**; the `self` rise is entirely the line-based
detector counting cast-at-use spellings (caveat below).

`docs/CODE_SMELLS.md` was regenerated with `smell_report.py --write` after the
wave (`--check --no-strict` freshness gate: **ok**); 527 coop tests pass.

## Per-file results (all `hexdiff --all` tables byte-identical)

| file | converted | note |
|---|---|---|
| `src/kyoshin/CUICfManager.cpp` | 4 | 1 revert (schedule shift) |
| `src/kyoshin/CUIWindowManager.cpp` | 0 | 25 `Create*Win` sites locked by the byteOff idiom |
| `src/kyoshin/menu/CMenuSymbolMark.cpp` | 1 | |
| `src/kyoshin/menu/CMenuOption.cpp` | 1 | |
| `src/kyoshin/cf/CItem.cpp` | 40 | 3 reverts; true accepted scope 43 |
| `src/kyoshin/CItemBoxLine.cpp` | 9 | |
| `src/kyoshin/cf/IResInfo.cpp` | 16 | 1 revert |
| `src/kyoshin/cf/CfGimmick.cpp` | 22 | 22/22 accepted sites cleared |
| `src/kyoshin/cf/CPartsChange.cpp` | 15 | |
| `src/kyoshin/cf/CTaskREvtSequence.cpp` | 10 | |
| `src/kyoshin/action/CActParamAnim.cpp` | 21 | |
| `src/kyoshin/action/CActParamData.cpp` | 4 | |
| `src/kyoshin/cf/CfRes.cpp` | 41 | accepted sites 16 -> 0 |
| `libs/monolib/src/scn/CScnEnvLgtCtrl.cpp` | 34 | accepted sites 34 -> 0 |
| `libs/monolib/src/scn/CScnItemModelNw4r.cpp` | 10 | |
| `libs/monolib/src/scn/CScn.cpp` | 4 | |
| `src/kyoshin/code_800B06A4.cpp` | 10 | |
| `src/kyoshin/CMainMenu.cpp` | 17 | |
| `src/kyoshin/CTaskGame.cpp` | 11 | |
| `src/kyoshin/cf/CSuddenCommu.cpp` | 13 | |
| `src/kyoshin/cf/CtrlEnemy.cpp` | 5 | |
| `src/kyoshin/plugin/pluginCfs.cpp` | 9 | 9/9 |
| `src/kyoshin/code_802405F4.cpp` | 13 | |
| `src/kyoshin/menu/code_80187F14.cpp` | 2 | |
| `src/kyoshin/menu/CMenuArtsSelect.cpp` | 0 | candidates are `container_of` back-adjustments |
| `src/kyoshin/CFloorMap.cpp` | 4 | |
| `src/kyoshin/CItemBoxGrid.cpp` | ~70 | 25+ fns; `CibgOuterSlots` etc. |
| `src/kyoshin/CEquipChange.cpp` | 0 | fully reverted (codegen shift) |
| `src/kyoshin/CMiniMap.cpp` | 0 | candidates are this-adjustment thunks |
| `libs/monolib/src/scn/CScnMaruShadowNw4r.cpp` | 8 | |
| `libs/monolib/src/scn/CScnRootNw4r.cpp` | 10 | |
| `libs/monolib/src/lod/LODMemMan.cpp` | ~26 | 13 accepted fns |
| `libs/monolib/src/core/code_804DEDA8.cpp` | 5 | |
| `src/kyoshin/cf/CfCam.cpp` | 5 | wave 3 |
| `src/kyoshin/cf/CChainActorPc.cpp` | 3 | wave 3 |
| `src/kyoshin/cf/chain/CChain.cpp` | 3 | wave 3 |
| `src/kyoshin/cf/CfCamEvent.cpp` | 1 | wave 3 |
| `src/kyoshin/cf/CfCamEvent_1.cpp` | 5 | wave 3 |
| `src/kyoshin/cf/CtrlObjectParam.cpp` | 1 | wave 3 |
| `src/kyoshin/cf/CfGimmickObject.cpp` | 3 | wave 3 |
| `src/kyoshin/cf/CTaskREvent.cpp` | 2 | wave 3 |
| `src/kyoshin/menu/CMenuPTChangeNotice.cpp` | 2 | wave 3 |
| `src/kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_MAIN.cpp` | 5 | wave 3 |
| `src/kyoshin/menu/CMenuGetItemMulti.cpp` | 0 | already typed |
| `libs/monolib/src/scn/code_804BD8E8.cpp` | 0 | reverted (delta-lint re-flags `extern "C" void*`) |

## Negative results — raw-offset shapes that must stay raw

Recorded in `docs/MWCC_PATTERNS.md` (*Raw-offset -> typed-field cleanup*).
Probed and reverted, each with the `hexdiff --all` table restored:

1. **Running-`byteOff` loops** — any `arr[i]` rewrite adds a per-iteration
   `mulli`; 100% -> 86%. Recurred in CUIWindowManager (25 sites), CUICfManager,
   CFloorMap, code_80187F14, code_804DEDA8, code_804BD8E8. KEEP.
2. **`&arr[index]` / hoisted single-load reuse** where retail does two loads
   (no CSE) — 100% -> 23-43% (CScnItemModelNw4r, CfRes, CItem, CfCamEvent_1).
3. **This-adjustment thunks / `container_of`** (`(char*)self - 0xNN`) —
   `static_cast` emits a null-guard or changes size 0x8 -> 0x10 (CScn,
   CScnRootNw4r, CMenuArtsSelect, CItemBoxGrid, CTaskGame, CMiniMap,
   CMenuSymbolMark, CUIWindowManager, CChain, CfCamEvent_1). KEEP.
4. **Integer-pointer-add operand order** `(T*)((u32)roff + (u32)base)` —
   pointer spellings canonicalize base-first (CFloorMap).
5. **TU-local view placement** — a plain-source view block can shift a later
   compiler-warning line citation in hexdiff output; declare views in the
   TU-local header (CMainMenu, code_80187F14).
6. **Shared-`addi` scheduling** — folding `&mField3380+8` to `field_0x3388` split
   an `addi` retail shares with the next call (CfCam `CleanupVoiceEnd`, 100% ->
   97.5%).

## Tool fix — `ptr_sites.py` is now class-aware

`tools/coop/ptr_sites.py` used to key acceptance on the bare last name
component, so `CUICfManager::Move` (HIGH_MATCH) matched the accepted
`CTTask<12CUICfManager>::Move`, and `CArrow3D::cbRenderBefore` (STRUCTURAL)
matched `CMenuSymbolMark::cbRenderBefore`. The report's actionable backlog was
therefore inflated in places.

Fixed:

- `function_spans` now returns the **qualified** definition name
  (`Klass::Method`, `cf::Foo::Bar`, `CTTask<9CTaskGame>::Move`) via `QUAL_RE`.
- New `AcceptedIndex` matches per source: exact qualified match, class-chain
  suffix match (`cf::CBattleManager::Foo` ~ `CBattleManager::Foo`), or
  last-component match for **unqualified** defs/targets (free functions).
  A qualified def no longer matches an unqualified target spelling, which is
  what removes the collisions.
- `accepted_keys()` still returns a `set` (now of qualified names); `scan_roots`
  uses `accepted_index()`.
- Regression test `test_member_name_collision_is_class_aware` pins the fix.

Effect: `src/kyoshin/CUICfManager.cpp` 53 -> 31 accepted sites,
`src/kyoshin/menu/CMenuSymbolMark.cpp` 10 -> 5; total accepted 655 -> 623 (after
the wave's conversions).

## Disclosures / caveats

- **`self`/`void*` rows in `CODE_SMELLS.md` are line-based.** They count
  cast-at-use spellings (`((View*)self)->...`) and move with any reformatting, so
  a byte-identical cleanup can *raise* them without adding real debt.
- **Delta-lint tension.** `tools/pi_harness/lint.py::lint_delta` flags every
  *added* line containing `extern "C"`/`void*`. Rewriting a one-liner such as
  `extern "C" void F(void* self) { return *(u32*)((u8*)self + N); }` is therefore
  flagged as a new violation even though the tokens pre-existed — this blocked
  several sites (recorded as "lint-blocked"), including `code_804BD8E8.cpp`
  entirely. Agents with clean diffs achieved it by splitting/avoiding such lines.
  Genuinely new tokens were fixed by hand (below).
- **`git` disclosure.** One subagent (`ptr-w2c`) ran a single read-only
  `git status`. No mutation, but it violates the repo's hard "never run git"
  rule; recorded rather than hidden.
- **Registry drift observed** (not caused by this wave): `UIWin_FlagBufCanShow`
  is byte-100% per `hexdiff` but `targets.json` lists it `COMPILES`.

### Hand-fixed new tokens (post-wave)

Token-diff (backup vs current) over the 30 touched files: `extern "C"` **0**,
`(void*)` casts **-26**, `void*` declarations **-4**, `UnkClass_*` **0**.
Residual genuinely-new tokens fixed and re-verified (tables identical):

- `CtrlEnemy.cpp`: view field `unk84` -> `mFlags84` (3 tokens).
- `CScnEnvLgtCtrl.cpp`: `void* armed64` -> `u8* armed64`.
- `code_800B06A4.cpp`: `void* ctx` -> `AD8Wrapper* ctx`; `void* field_734` ->
  `u8* field_734` (+`(u8*)arg`).
- `CScn.cpp`: one new use of the pre-existing legacy member name `unk_3E5`
  (declared in the header) — left alone to avoid a cross-file legacy rename.

### `attempts.jsonl` quarantine

Line 40891 was a truncated fragment
(`"symbol": "MiniMapTeardownMarkerPanes", "status": "HIGH_MATCH", "instruction_match": 87.1`)
with no opening `{` — not produced by this wave. It was removed on request
(original preserved at `/tmp/attempts.jsonl.pre-quarantine`); the log now parses
clean (40939 records, 0 bad).

## Gate status

- Freshness: **ok** (`--check --no-strict`).
- Strict regression vs `origin/main`: still reports growth. Per the repo
  convention (cf. `docs/evidence/decomp/unit_rules_category_pool_labels.md`
  §"Conscious smell re-baseline") this is a **conscious re-baseline**: the
  growth is (a) retail-mandated `extern "C"`/`self` linkage for newly-decompiled
  bodies (bucket A in `code_smell_regression_triage.md`), (b) not-yet-accepted
  body churn (bucket C), and (c) the line-based `self`/`void*` artefacts above.
  `docs/CODE_SMELLS.md` is regenerated; the strict gate compares against the
  committed baseline, **so the human re-baselines at commit time** (the tool
  intentionally reads `origin/main`, not the working tree).

## Follow-ups (not done here)

1. Decide the `self`/`void*` metric definition (params only vs cast-at-use) so
   cleanups stop perturbing the gate.
2. Resolve the delta-lint vs cleanup tension for pre-existing `extern "C"`/`void*`
   lines (ignore tokens present on the removed counterpart).
3. Remaining `container_of`/byteOff-idiom sites (mostly non-convertible):
   `CUIWindowManager` (25), `CMenuArtsSelect`, `CEquipChange`, `CMiniMap`,
   `CTaskREvent` residual, `CfCam` residual, `code_804BD8E8`, etc.
