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
files; none ran `git` (see *Disclosures*), edited `tools/coop/targets.json`,
`docs/CODE_SMELLS.md`, or added `extern "C"`/`void*`/`self` params.

## Headline numbers (live `smell_report.py` scan)

| metric | before | after | Δ |
|---|---|---|---|
| `ptr_sites` (all expressions) | 9102 | 8649 | **-453** |
| `ptr_sites` inside accepted fns | 940 | 655 | **-285** |
| `ptr-arith` (line proxy) | 6294 | 5969 | **-325** |
| `deref-arith` (line proxy) | 2599 | 2468 | **-131** |
| `void*` | 7938 | 7931 | -7 |
| extern "C" decl / def | 8804 / 5108 | 8804 / 5108 | 0 |
| `self` (see caveat) | 13712 | 13758 | +46 |

Agents report **~435 textual sites converted**; the detector's accepted-site
total fell 285. The gap is the `ptr_sites.py` heuristic (below), not missing
edits.

`docs/CODE_SMELLS.md` was regenerated with `smell_report.py --write` after the
wave (freshness gate green).

## Per-file results (all tables byte-identical)

| file | converted | note |
|---|---|---|
| `src/kyoshin/CUICfManager.cpp` | 4 | 1 revert (schedule shift) |
| `src/kyoshin/CUIWindowManager.cpp` | 0 | 25 `Create*Win` sites locked by the byteOff idiom |
| `src/kyoshin/menu/CMenuSymbolMark.cpp` | 1 | |
| `src/kyoshin/menu/CMenuOption.cpp` | 1 | |
| `src/kyoshin/cf/CItem.cpp` | 40 | 3 reverts; true accepted scope 43 (heuristic undercount) |
| `src/kyoshin/CItemBoxLine.cpp` | 9 | |
| `src/kyoshin/cf/IResInfo.cpp` | 16 | 1 revert |
| `src/kyoshin/cf/CfGimmick.cpp` | 22 | 22/22 accepted sites cleared |
| `src/kyoshin/cf/CPartsChange.cpp` | 15 | |
| `src/kyoshin/cf/CTaskREvtSequence.cpp` | 10 | |
| `src/kyoshin/action/CActParamAnim.cpp` | 21 | |
| `src/kyoshin/action/CActParamData.cpp` | 4 | |
| `src/kyoshin/cf/CfRes.cpp` | 41 | ACC sites 16 -> 0 |
| `libs/monolib/src/scn/CScnEnvLgtCtrl.cpp` | 34 | ACC sites 34 -> 0 |
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
| `src/kyoshin/menu/CMenuArtsSelect.cpp` | 0 | all candidates are `container_of` back-adjustments |
| `src/kyoshin/CFloorMap.cpp` | 4 | |
| `src/kyoshin/CItemBoxGrid.cpp` | ~70 | 25+ fns; `CibgOuterSlots` etc. |
| `src/kyoshin/CEquipChange.cpp` | 0 | fully reverted (codegen shift) |
| `src/kyoshin/CMiniMap.cpp` | 0 | candidates are this-adjustment thunks |
| `libs/monolib/src/scn/CScnMaruShadowNw4r.cpp` | 8 | |
| `libs/monolib/src/scn/CScnRootNw4r.cpp` | 10 | |
| `libs/monolib/src/lod/LODMemMan.cpp` | ~26 | 13 accepted fns |
| `libs/monolib/src/core/code_804DEDA8.cpp` | 5 | |

## Negative results — raw-offset shapes that must stay raw

Recorded in `docs/MWCC_PATTERNS.md` (see *Raw-offset -> typed-field cleanup*).
These were probed and reverted (each with the `hexdiff --all` table restored):

1. **Running-`byteOff` loops** (`base = start + i*stride; ... base += stride;`) —
   any `arr[i]` rewrite adds a per-iteration `mulli`; 100% -> 86%. Recurred in
   CUIWindowManager (25 sites), CUICfManager, CFloorMap, code_80187F14,
   code_804DEDA8. Load-bearing: keep.
2. **`&arr[index]` / hoisted-single-load reuse** where retail performs two loads
   (no CSE) — 100% -> 23-43% (`CScnItemModelNw4r`, `CfRes`, `CItem`).
3. **This-adjustment thunks / MI subobject recovery** (`(char*)self - 0xNN`,
   `container_of`) — `static_cast` emits a null-guard or changes size 0x8 -> 0x10
   (`CScn.cpp`, `CScnRootNw4r`, `CMenuArtsSelect`, `CItemBoxGrid`, `CTaskGame`,
   `CMiniMap`, `CMenuSymbolMark`, `CUIWindowManager` thunks).
4. **Integer-pointer-add operand order** `(T*)((u32)roff + (u32)base)` — pointer
   spellings canonicalize base-first and flip the add (`CFloorMap`).
5. **TU-local view block placement** — declaring views as plain source can shift
   a later compiler-warning line citation in hexdiff output; declaring them in a
   TU-local header keeps the table identical (`CMainMenu`, `code_80187F14`).

## Disclosures / caveats

- **`tools/coop/ptr_sites.py` accepted/unmatched split is unreliable.** It keys
  on the bare last name component, so `Foo::Move` collides with `Bar::Move`, and
  it misses `__declspec`/`;`-bearing one-liners. It both over-counted
  (`CUICfManager` -22, `CMenuSymbolMark` -5 phantom accepted sites) and
  under-counted (`CItem.cpp` accepted scope 43 vs 35). The 940 headline number
  was therefore an estimate; the authoritative check is the enclosing function's
  own status in `tools/coop/targets.json` (which every agent used).
- **`self`/`void*` rows in `CODE_SMELLS.md` are line-based.** They count
  cast-at-use spellings (`((View*)self)->...`) and move with any reformatting, so
  a byte-identical cleanup can *raise* them without adding real debt. This wave
  added +46 `self` counts that are entirely this artefact.
- **Delta-lint tension.** `tools/pi_harness/lint.py::lint_delta` flags every
  *added* line containing `extern "C"`/`void*`. Rewriting a one-liner such as
  `extern "C" void F(void* self) { return *(u32*)((u8*)self + N); }` is therefore
  flagged as a new violation even though the tokens pre-existed. Three
  CF2-wave files achieved a clean delta by splitting/avoiding such lines; the
  rest will report pre-existing tokens on rewritten lines. Genuinely new tokens
  introduced by the wave were fixed by hand (below).
- **`git status` disclosure.** One subagent (`ptr-w2c`) ran a single read-only
  `git status` at the end. No mutation, but it violates the repo's hard
  "never run git" rule; recorded here rather than hidden.
- **Registry drift observed** (not caused by this wave): `UIWin_FlagBufCanShow`
  is byte-100% per `hexdiff` but `targets.json` lists it `COMPILES`.

### Hand-fixed new tokens (post-wave)

Token-diff (backup vs current) over the 30 touched files: `extern "C"` **0**,
`(void*)` casts **-26**, `void*` declarations **-4**, `UnkClass_*` **0**. The
residual genuinely-new tokens were fixed and re-verified (tables identical):

- `CtrlEnemy.cpp`: view field `unk84` -> `mFlags84` (3 tokens).
- `CScnEnvLgtCtrl.cpp`: `void* armed64` -> `u8* armed64`.
- `code_800B06A4.cpp`: `void* ctx` -> `AD8Wrapper* ctx`; `void* field_734` ->
  `u8* field_734` (+`(u8*)arg`).
- `CScn.cpp`: one new use of the pre-existing legacy member name `unk_3E5`
  (declared in the header) — left alone to avoid a cross-file legacy rename.

## Verification

- Every converted file: `hexdiff --all` table identical before/after (per-agent
  baselines in `/tmp/ptr-*_*.before` / `/tmp/*.bak`; the three hand-fixed units
  re-verified here).
- Final hand-fix units: `CtrlEnemy` 33/41, `CScnEnvLgtCtrl` 93/127,
  `code_800B06A4` unchanged — all `TABLE IDENTICAL`.
- `smell_report.py --write` run; freshness gate green.
- `smell_report.py --check` still fails on the strict regression gate vs
  `origin/main` — a pre-existing condition (see
  `docs/evidence/decomp/code_smell_regression_triage.md`), not introduced here;
  the wave resolved `ptr-arith` `LODMemMan` and added only the line-based
  `self`/`void*` artefacts above.

## Follow-ups (not done here)

1. Fix `ptr_sites.py` accepted lookup to key on class-qualified definitions.
2. Decide the `self`/`void*` metric definition (params only vs cast-at-use) or
   consciously re-baseline.
3. `smell_report.py --check` re-baseline once cleanups/`extC` growth are
   reconciled with `origin/main`.
4. Remaining accepted-site TUs not in this wave (mostly `container_of` thunks
   and byteOff loops): `CUIWindowManager` (25), `CMenuArtsSelect` (9),
   `CEquipChange`, `CMiniMap`, `CfCam`, `CfCamEvent*`, `CMenuGetItemMulti`,
   `CTaskREvent`, `CVS_THREAD_BATTLE_MAIN`, `CFloorMap` residual, etc.
