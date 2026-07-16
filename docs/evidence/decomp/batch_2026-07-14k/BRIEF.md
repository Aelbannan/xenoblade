# Batch 2026-07-14k — five **new** decomp targets

Read and follow `.cursor/skills/xenoblade-decomp/SKILL.md`, `docs/MWCC_REFERENCE.md` (esp. **§3 Retail SDA global names**), and `tools/test/compare_behaviour/README.md` if static match &lt; 100%.

## Policy

- Historical stretch goal: **FULL_MATCH**; acceptance follows the current registry policy via `python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."`
- High-level C/C++ only in `src/**` / `libs/**` (no `register rN`, no whole-function asm)
- Below 100%: host dual-oracle tests are retired; continue matching, use automatic SMT when eligible, and optional PPC when registered
- Run `coop run size <unit>` — must PASS (under budget OK; trim TU extras if over)
- **Edit only your exclusive files / methods** — do not touch other batch targets' exclusive regions
- Prefer `StrReplace` scoped to your function body only (shared TUs!)

## Targets (one agent each)

| id | symbol | address | size | unit | exclusive edit |
|----|--------|---------|------|------|----------------|
| battlestate-vfunc10 | `CBattleState_UnkVirtualFunc10__Q22cf12CBattleStateFv` | `0x80148A18` | `0x444` | `kyoshin/cf/object/CBattleState` | **only** `CBattleState_UnkVirtualFunc10` — leave ctor / all other vfuncs alone |
| menu-arts-ctor | `__ct__CMenuArtsSelect` | `0x80102B08` | `0x288` | `kyoshin/menu/CMenuArtsSelect` | **only** ctor — leave `Term` / `Move` / `cbRenderBefore` alone |
| menu-bps-ctor | `__ct__CMenuBattlePlayerState` | `0x8010B880` | `0x580` | `kyoshin/menu/CMenuBattlePlayerState` | **only** ctor — leave `Term` / `Move` / `cbRenderBefore` alone |
| cam-follow-ctor | `__ct__cf_CfCamFollow` | `0x8006C684` | `0x238` | `kyoshin/cf/CfCam` | create `src/kyoshin/cf/CfCam.cpp` (+ hpp) with **only** this ctor (configure already lists TU) |
| menu-enemy-ctor | `__ct__CMenuEnemyState` | `0x8010FC4C` | `0x250` | `kyoshin/menu/CMenuEnemyState` | **only** ctor — leave `Move` / `cbRenderBefore` alone |

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm

### `cf::CBattleState::CBattleState_UnkVirtualFunc10` (`asm_CBattleState_UnkVirtualFunc10.s`)
- Frame `-0xA0` + `stmw r22`; **fake-`Fv` + `r4=arg`** (loads halfword `+0x2E`, fields `+0x2C/+0x30`, vt at `r4+0`)
- Early `beq` when `lhz 0x2E(r4)==0`
- Walk/fill paths involving `this+0x8` and related state — recover structurally from asm; declare helpers `extern`
- Use `extern "C"` multi-arg entry like other CBattleState vfuncs (vfunc6/8/11); do **not** edit other vfuncs

### `CMenuArtsSelect::CMenuArtsSelect` (`asm_ct_CMenuArtsSelect.s`)
- Frame `-0x20`; saves r29–r31; arg `r4` kept in r29
- `__ct__8CProcessFv` then set `this+0x10` = `lbl_eu_8052C1C0`, PTMFs from `__ptmf_null`, more vt pieces from `lbl_eu_8052C084` (+0x24/+0xAC/+0xBC)
- Continue field inits / embeds from asm; extend hpp only as needed for ctor fields
- **Do not** change `Term` / `Move` / `cbRenderBefore`

### `CMenuBattlePlayerState::CMenuBattlePlayerState` (`asm_ct_CMenuBattlePlayerState.s`)
- Large frame `-0x2B0` + `_savegpr_21`; `__ct__8CProcessFv`
- Vtables `lbl_eu_8052C1C0` / `lbl_eu_8052C330` + `__ptmf_null` PTMF block (same family as ArtsSelect)
- Nested inits / loops for party slots — follow full asm; leave Term/Move/cbRenderBefore untouched

### `cf::CfCamFollow::CfCamFollow` (`asm_ct_CfCamFollow.s`)
- Symbol mangling is **`__ct__cf_CfCamFollow`** (retail); place in new `src/kyoshin/cf/CfCam.cpp`
- Calls `__ct__cf_CfCam` then sets vt `lbl_eu_80527260`; inits embeds at `+0x1C/+0x28/+0x34` via `func_8004B0B0` / `func_8004B60C` with `lbl_eu_806662DC` SDA floats
- Budget for whole `CfCam` split is large (`0x97F0`) — implement **only** this ctor; under-budget is OK; do not pull in the rest of the TU
- Prefer `extern "C"` entry matching retail symbol name if member mangling differs

### `CMenuEnemyState::CMenuEnemyState` (`asm_ct_CMenuEnemyState.s`)
- Frame `-0x60`; `__ct__8CProcessFv`; vt `lbl_eu_8052BF70` / `lbl_eu_8052C438` + `__ptmf_null`
- Same Process/PTMF constructor family as BPS/ArtsSelect — mirror proven Init/Term patterns from siblings
- Leave `Move` / `cbRenderBefore` bodies alone

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
