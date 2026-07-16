# Batch 2026-07-14i — five decomp targets (new + polish)

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
| battlestate-vfunc33 | `CBattleState_UnkVirtualFunc33__Q22cf12CBattleStateFv` | `0x8014A014` | `0x160` | `kyoshin/cf/object/CBattleState` | NEW / stub | **only** `CBattleState_UnkVirtualFunc33` — leave ctor / vfunc6 / 11 / 26 / 31 alone |
| menu-ptgauge-move | `Move__12CMenuPTGaugeFv` | `0x80188714` | `0x35C` | `kyoshin/menu/CMenuPTGauge` | ~97.1% CODE_MATCH | **only** `Move` — leave `cbRenderBefore` alone |
| window-mgr-move | `Move__16CUIWindowManagerFv` | `0x8013D0C8` | `0x4DC` | `kyoshin/CUIWindowManager` | ~79.3% HIGH_MATCH | **only** `Move` (+ helpers local to this TU) |
| menu-arts-cbrender | `cbRenderBefore__15CMenuArtsSelectFv` | `0x8010491C` | `0x3C0` | `kyoshin/menu/CMenuArtsSelect` | ~95.2% CODE_MATCH | **only** `cbRenderBefore` — leave `Term` alone |
| occ-set-frustum | `setFrustum__11COccCullingFP12CCullFrustum` | `0x801A2098` | `0x588` | `kyoshin/COccCulling` | ~88.5% HIGH_MATCH | **only** `setFrustum` — leave `addFrustum` / `func_801A1188` / ctor alone |

Batch **14h** is still editing other CBattleState vfuncs — stay strictly inside UnkVirtualFunc33.

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm / prior attempts

### `cf::CBattleState::CBattleState_UnkVirtualFunc33` (`asm_CBattleState_UnkVirtualFunc33.s`)
- Same leaf layout as **FULL_MATCH** `UnkVirtualFunc31`, but final `and` reads **`lhz` at `this+0x6`** (not `+0x4` / `unk4`)
- Fake-`Fv` + **`r4=id`**; early `return 0` if `id >= 0x12f`
- Nested cmpwi tree → bit masks (incl. `0x8000` via `lis`/`addi` for id `0x117`)
- Copy the vfunc31 `extern "C"` multi-arg entry pattern; add halfword field at +0x6 in hpp if missing
- Do **not** edit other vfuncs in this TU

### `CMenuPTGauge::Move` (`asm_Move_CMenuPTGauge.s`)
- Already ~97.1%: close the last gaps (likely Chaitin / branch / float pool / state-machine edge)
- Same gate family as `cbRenderBefore` (`CTaskGame` / bit21 / `func_8013BE50` / mask `0xAFA40000`)
- State machine on `unk8C`; leave `cbRenderBefore` untouched
- Prefer §17.6 single-insn `b done` if MWCC collapses `rlwinm.`/`beq`/`b`

### `CUIWindowManager::Move` (`asm_Move_CUIWindowManager.s`)
- ~79.3%: dual `reslist` walks, timer report via `lbl_eu_8066408C`, mark/remove loops
- Compare frame/`stmw`/iterator shape vs retail; avoid non-trivial iterator ctor forcing zero-init of pending buffer
- SDA: `lbl_eu_80664088` / `lbl_eu_8066408C` only (no mangled `spInstance`)
- Host: `uiwindowmanager_move` may already exist — extend scenarios if still &lt; 100%

### `CMenuArtsSelect::cbRenderBefore` (`asm_cbRenderBefore_CMenuArtsSelect.s`)
- ~95.2%: gates + layout draws; leave `Term` alone
- Host `menu_arts_cbrender` may exist — harden if soft-capped
- Check SDA bit/mask constants and pane FindPaneByName string offsets

### `COccCulling::setFrustum` (`asm_setFrustum_COccCulling.s`)
- ~88.5% HIGH_MATCH; **split .text spare is tiny (~0x14)** — do not add extra methods to the TU
- Matrix scale/rot/mul/invert + plane setup + FLAGS_01 branch
- Reuse existing `lbl_eu_*` float pools; leave `addFrustum` / `func_801A1188` bodies alone
- Diff carefully against retail for float schedule and helper calls

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
