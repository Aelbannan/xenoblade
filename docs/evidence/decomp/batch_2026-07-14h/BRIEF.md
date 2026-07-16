# Batch 2026-07-14h — five decomp targets (resume + one new)

Read and follow `.cursor/skills/xenoblade-decomp/SKILL.md`, `docs/MWCC_REFERENCE.md` (esp. **§3 Retail SDA global names**), and `tools/test/compare_behaviour/README.md` if static match &lt; 100%.

## Policy

- Historical stretch goal: **FULL_MATCH**; acceptance follows the current registry policy via `python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."`
- High-level C/C++ only in `src/**` / `libs/**` (no `register rN`, no whole-function asm)
- Below 100%: host dual-oracle tests are retired; continue matching, use automatic SMT when eligible, and optional PPC when registered
- Run `coop run size <unit>` — must PASS (under budget OK; trim TU extras if over)
- **Edit only your exclusive files / methods** — do not touch other batch targets' exclusive regions
- Prefer `StrReplace` scoped to your function body only (shared TUs!)

## Targets (one agent each)

| id | symbol | address | size | unit | current | exclusive edit |
|----|--------|---------|------|------|---------|----------------|
| battlestate-vfunc11 | `CBattleState_UnkVirtualFunc11__Q22cf12CBattleStateFv` | `0x80148FC8` | `0x174` | `kyoshin/cf/object/CBattleState` | ~96.2% CODE_MATCH | **only** `CBattleState_UnkVirtualFunc11` — leave ctor / vfunc6 / vfunc26 / vfunc31 alone |
| battlestate-vfunc26 | `CBattleState_UnkVirtualFunc26__Q22cf12CBattleStateFv` | `0x80148364` | `0x12C` | `kyoshin/cf/object/CBattleState` | ~54.7% STRUCTURAL | **only** `CBattleState_UnkVirtualFunc26` — leave ctor / vfunc6 / vfunc11 / vfunc31 alone |
| menu-enemy-move | `Move__15CMenuEnemyStateFv` | `0x80110888` | `0x9B8` | `kyoshin/menu/CMenuEnemyState` | ~76.8% HIGH_MATCH | **only** `Move` — leave `cbRenderBefore` alone |
| uicf-func-80133324 | `func_80133324__12CUICfManagerFv` | `0x80133DF8` | `0x3C0` | `kyoshin/CUICfManager` | ~33.5% STRUCTURAL | **only** `func_80133324` — leave `Init` / `Term` alone |
| menu-bps-move | `Move__22CMenuBattlePlayerStateFv` | `0x8010CE0C` | `0x8E8` | `kyoshin/menu/CMenuBattlePlayerState` | NEW / stub | **only** `Move` — leave `Term` / `cbRenderBefore` alone |

`battlestate-vfunc31` is already **FULL_MATCH** — do not re-edit it.

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm / prior attempts

### `cf::CBattleState::CBattleState_UnkVirtualFunc11` (`asm_CBattleState_UnkVirtualFunc11.s`)
- Already ~96.2%: remaining gap is Chaitin register coloring (`r5` vs retail `r12` vtable temp; `r3` vs retail `r0` found-flag; missing dead `addi r3,+7`)
- Try declaration-order / block-scope / goto variants from `docs/MWCC_REFERENCE.md` §8; if soft-capped ≥95%, ship behaviour host (≥8 scenarios) + log policy note
- Do **not** touch other vfuncs in this TU

### `cf::CBattleState::CBattleState_UnkVirtualFunc26` (`asm_CBattleState_UnkVirtualFunc26.s`)
- Fake-`Fv` + **`r4=src`**; call virtual **vt+0x78** then loop `i=0..7` building stack entries → **vt+0x1C** (`UnkVirtualFunc6`)
- Tail: copy 16× u64-style pairs from `src-4` into `this+0x1528` via `lwzu`/`stwu` ctr loop
- Prior attempt ~54.7% — compare `objdump` vs retail frame/`stmw`/`mtctr` shape; match entry-zero skip and `memset(entry,0,0x34)` fill order
- Do **not** implement UnkVirtualFunc6/29 bodies; only call through vtable

### `CMenuEnemyState::Move` (`asm_Move_CMenuEnemyState.s`)
- ~76.8%: retail saves **r22–r30** (frame `-0xe0`); decomp only **r25–r29** (`-0x90`) — widen live ranges / hoist locals so MWCC allocates matching callee-saved GPRs
- Early gates: `CTaskGame` / bit21 / `func_8013BE50` / mask **`0xAFA40000`** / `CfGameManager::func_8008585C` / `func_80082D54(0)`
- Leave `cbRenderBefore` untouched

### `CUICfManager::func_80133324` (`asm_func_80133324_CUICfManager.s`)
- Fake-`Fv` + **`r4,r5,r6`**; early-out if `r5==r6`
- Nested `cmpwi` ranges → helpers + SDA `lbl_eu_80664050` / `lbl_eu_80664054`
- Prior ~33% — re-diff carefully; keep `extern "C"` free-function ABI; SDA `lbl_eu_*` names only
- Leave `Init`/`Term` alone

### `CMenuBattlePlayerState::Move` (`asm_Move_CMenuBattlePlayerState.s`) **NEW**
- Frame `-0xc0` + paired floats f26–f31 + `_savegpr_20`
- Same gate family as other HUD Moves: `CTaskGame::getInstance` / `func_800426F0` / `lbl_eu_80663E28` bit / `func_8013BE50` / mask
- Large body: party slot walk, HP/tension/status updates, layout pane sizing — follow full asm
- Extend hpp with fields `Move` needs; **do not** edit `Term` or `cbRenderBefore`
- Prefer PTGauge/Enemy gate pattern (§17.6 single-insn `b done` if MWCC collapses `rlwinm.`/`beq`/`b`)

## Shared repo resources

```bash
python3 tools/coop/run.py targets show <id>
python3 tools/coop/run.py cycle <id> --hypothesis "..." --next-change "..."
python3 tools/coop/run.py diff <unit> --symbol <sym>
python3 tools/coop/run.py size <unit>
python3 tools/coop/run.py ctx <source.cpp>
```

- Retail reference: `build/us/asm/kyoshin/`
- Log attempts to `docs/evidence/decomp/attempts.jsonl`
- Append reusable breakthroughs to `docs/MWCC_REFERENCE.md`
- Claims recorded in `docs/ownership.csv`

## Done criteria

Report: final match %, `cycle` exit code, size PASS/FAIL, files changed, and whether FULL_MATCH was reached. If stalled below acceptance, preserve the best candidate and report the exact mismatch categories, attempts, and next bounded experiments.
