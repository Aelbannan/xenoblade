# Batch 2026-07-14j — five **new** decomp targets

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
| battlestate-vfunc29 | `CBattleState_UnkVirtualFunc29__Q22cf12CBattleStateFv` | `0x80148490` | `0x15C` | `kyoshin/cf/object/CBattleState` | NEW / stub | **only** `CBattleState_UnkVirtualFunc29` — leave ctor / vfunc6 / 8 / 11 / 26 / 31 / 33 alone |
| battlestate-vfunc8 | `CBattleState_UnkVirtualFunc8__Q22cf12CBattleStateFv` | `0x801485EC` | `0x428` | `kyoshin/cf/object/CBattleState` | NEW / stub | **only** `CBattleState_UnkVirtualFunc8` — leave ctor / other vfuncs alone |
| battle-mgr-move | `Move__16CUIBattleManagerFv` | `0x8012F270` | `0xB00` | `kyoshin/CUIBattleManager` | NEW / stub | **only** `Move` — leave `Init` alone |
| menu-arts-move | `Move__15CMenuArtsSelectFv` | `0x80103D68` | `0xBB4` | `kyoshin/menu/CMenuArtsSelect` | NEW / stub | **only** `Move` — leave `Term` / `cbRenderBefore` alone |
| uicf-move | `Move__12CUICfManagerFv` | `0x801332A4` | `0x97C` | `kyoshin/CUICfManager` | NEW / stub | **only** `Move` — leave `Init` / `Term` / `func_80133324` alone |

Batches **14h/14i** still own other symbols in shared TUs — stay strictly inside your exclusive method.

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm

### `cf::CBattleState::CBattleState_UnkVirtualFunc29` (`asm_CBattleState_UnkVirtualFunc29.s`)
- True `Fv` (only `r3=this`); frame `-0x20` + `_savegpr` via `stmw r26`
- Loop `i=0..7` over entries at `this+0x1388` (stride `0x34` = `CBattleStateEntry`):
  - save `entry->unk0C` (id), `memset(entry,0,0x34)`
  - if `id >= 0x12f` → cleared / skip bit set
  - else search **13** halfwords at `this+0x14 + k*0x34` (`lhz` offsets `0x14,0x48,…`) for match → found flag
  - if not found: clear bit `(1<<id)` in `u32` at `this+0x15AC`
- After loop: `memset(this+0x152C, 0, 0x80)`
- Prefer C++ over the `CBattleStateEntry` type already in `CBattleState.hpp`
- Do **not** edit other vfuncs (esp. vfunc26 which *calls* this)

### `cf::CBattleState::CBattleState_UnkVirtualFunc8` (`asm_CBattleState_UnkVirtualFunc8.s`)
- Fake-`Fv` + **`r4=entry`** (`CBattleStateEntry*`); save `this`/`entry` early
- Large nested `cmpwi` tree on `entry->unk0C` (id) with several outcome regions
- Ends with at least one `memset` and field writes into `this` / entry
- Use `extern "C"` multi-arg entry like vfunc6/11/26; declare friend if needed
- Do **not** implement other vfuncs — call through vtable if retail does

### `CUIBattleManager::Move` (`asm_Move_CUIBattleManager.s`)
- Frame `-0x220` + `stmw r25`; early gates: `CTaskGame::getInstance` / `func_800426F0` / `lbl_eu_80663E28` bit10 / MEM2
- Repeated create/bind patterns via `func_801355A0`, `func_801355F4`, `lbl_eu_80664048`, `func_801096B8`, `func_801ACCE0`
- Bit tests on manager flags; allocate/register child UI objects
- Extend `CUIBattleManager.hpp` fields `Move` needs; **leave `Init` untouched**
- SDA names: `lbl_eu_*` only

### `CMenuArtsSelect::Move` (`asm_Move_CMenuArtsSelect.s`)
- Frame `-0x70` + `_savegpr_20`; same HUD gate family as `cbRenderBefore` / PTGauge:
  - `CTaskGame` / `lbl_eu_80663E28` bit10 / `func_8013BE50` / mask
  - then `lbl_eu_80663E24`, `CfGameManager`, `func_8018A608`, `func_80122448`, buffs, etc.
- Large state machine after gates — follow full asm
- Extend hpp fields for `Move`; **do not** edit `Term` or `cbRenderBefore`
- Prefer §17.6 single-insn `b done` if MWCC collapses gate `rlwinm.`/`beq`/`b`

### `CUICfManager::Move` (`asm_Move_CUICfManager.s`)
- Frame `-0x120`; bitflag-driven create/teardown of UI children via `lbl_eu_80664054`
- Calls `func_801338C8`, `func_80133770`, `__ct__CMenuKeyAssign`, `func_801109D8`, etc.
- Bit tests on `r4` early look like **fake-`Fv` with flags in r4** — verify from callers; may be reading `this` fields into r4
- **Leave `Init` (asm), `Term`, and `func_80133324` alone** — append `Move` only
- SDA: `lbl_eu_80664054` / `lbl_eu_80663E28` only

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
