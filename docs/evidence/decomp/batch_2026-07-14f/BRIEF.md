# Batch 2026-07-14f — five new decomp targets

Read and follow `.cursor/skills/xenoblade-decomp/SKILL.md`, `docs/MWCC_REFERENCE.md` (esp. **§3 Retail SDA global names**), and `tools/test/compare_behaviour/README.md` if static match &lt; 100%.

## Policy

- Historical stretch goal: **FULL_MATCH**; acceptance follows the current registry policy via `python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."`
- High-level C/C++ only in `src/**` / `libs/**` (no `register rN`, no whole-function asm)
- Below 100%: host dual-oracle tests are retired; continue matching, use automatic SMT when eligible, and optional PPC when registered
- Run `coop run size <unit>` — must PASS (under budget OK; trim TU extras if over)
- **Edit only your exclusive files / methods** — do not touch other batch targets' exclusive regions

## Targets (one agent each)

| id | symbol | address | size | unit | exclusive edit |
|----|--------|---------|------|------|----------------|
| viewroot-get-fullscreen | `getFullScreenView__9CViewRootFv` | `0x80445314` | `0x1D8` | `monolib/src/core/CViewRoot` | **only** `getFullScreenView` in `libs/monolib/src/core/CViewRoot.cpp` (+ hpp if needed) — leave `setCurrent` / `renderView` / `create` alone |
| menu-bps-term | `Term__22CMenuBattlePlayerStateFv` | `0x8010CC5C` | `0x1B0` | `kyoshin/menu/CMenuBattlePlayerState` | create `src/kyoshin/menu/CMenuBattlePlayerState.cpp` (+ hpp) with **only** `Term` |
| menu-arts-term | `Term__15CMenuArtsSelectFv` | `0x80103B20` | `0x248` | `kyoshin/menu/CMenuArtsSelect` | create `src/kyoshin/menu/CMenuArtsSelect.cpp` (+ hpp) with **only** `Term` |
| menu-enemy-cbrender | `cbRenderBefore__15CMenuEnemyStateFv` | `0x80111240` | `0x274` | `kyoshin/menu/CMenuEnemyState` | create `src/kyoshin/menu/CMenuEnemyState.cpp` (+ hpp) with **only** `cbRenderBefore` |
| aiaction-vfunc1 | `CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv` | `0x8014B41C` | `0x110` | `kyoshin/cf/object/CAIAction` | **only** `CAIAction_UnkVirtualFunc1` in `src/kyoshin/cf/object/CAIAction.cpp` (+ hpp) — leave ctor alone |

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm

### `CViewRoot::getFullScreenView` (`asm_getFullScreenView.s`)
- Frame `-0x30`; `stmw r27`; early-outs on **`lbl_eu_806655D0@sda21`** null and `CDesktop::getView()` null
- Walk linked list from `view+0x60 → *head`; filter nodes by type range `0x30..0x34` at `+0x50`
- Flag/`rlwinm` checks on `+0x7c` bit 27; scan `+0x1ac` count with ring at `+0x1a0`-ish — match existing draft control flow
- Already ~99.7% — close last insn/reg gaps; do not regress other CViewRoot symbols
- SDA: use `lbl_eu_806655D0`, not mangled `spInstance` aliases

### `CMenuBattlePlayerState::Term` (`asm_Term_CMenuBattlePlayerState.s`)
- Frame `-0x20`; saves r28–r31; `waitForDrawDone__9CDeviceVIFv`
- `removeRenderCB(this->unk60, this ? this+0x5c : this)` — null-this adjust pattern
- Loop `i=0..` with stride `0x270`: virtual-delete (`vt+8`, arg `1`) pointers at slot offsets `+0x74`, `+0x7c`, `+0x8c`, `+0x94`, `+0x9c` (and continue in asm) then null them
- Build minimal class/`hpp` with those field offsets; declare `waitForDrawDone` / `removeRenderCB` as needed
- configure.py already lists the TU as NonMatching — create the `.cpp`

### `CMenuArtsSelect::Term` (`asm_Term_CMenuArtsSelect.s`)
- Clears **`lbl_eu_80663F24@sda21`**: zero `+0xb0`, `ori +0x68 |= 0x40`, then store null to the SDA global
- `waitForDrawDone` → `removeRenderCB(*(this+0x64), this+0x5c)`
- Loop freeing pointers at `+0x1b8`, `+0x170`, `+0x104`, and (while `i < 8`) `+0xa4` on a sliding `r30` base — follow full asm for remaining frees/nulls
- **Wrong:** writing mangled instance reloc instead of `lbl_eu_80663F24`

### `CMenuEnemyState::cbRenderBefore` (`asm_cbRenderBefore_CMenuEnemyState.s`)
- Frame `-0xd0`; same gate prefix as PTGauge: `CTaskGame::getInstance` → `func_800426F0` nonzero early-out; **`lbl_eu_80663E28` bit 10**; `func_8013BE50` null; **`lbl_eu_80663E24 & 0xAFA40000`**
- `GXSetZMode(0,0,0)`; stack `nw4r::lyt::DrawInfo` ctor; `func_80137250(&drawInfo)`
- Build/sort index array (bubble on `this + idx*0x4c + 0xbc` floats) then draw via `func_80137038` — replicate loops from asm
- Share patterns with `CMenuPTGauge::cbRenderBefore` if helpful; declare missing externs

### `cf::CAIAction::CAIAction_UnkVirtualFunc1` (`asm_CAIAction_UnkVirtualFunc1.s`)
- Mangled `Fv` but retail uses **`r3=this`, `r4=outA`, `r5=outB`** — recover as a multi-arg method (may need `extern "C"` entry or correct prototype; see MWCC_REFERENCE ABI notes)
- Copies block `this+0xadc..` into `*r4` (words/halfs/float through `+0x1c`)
- Clears `outB+0x204` / `+0x208`; loop `i < *(this+0x214)` indexing ring at `this+0x210` / stride `*(this+0x218)` into `this+0x20c` slots of size `0x20`, writing into `outB` buffer at `+0x200` with modular columns from `outB+0x204/208/20c`
- **Only edit this method** — leave `__ct__Q22cf9CAIActionFv` as-is (batch 14e)

## Shared repo resources

```bash
python3 tools/coop/run.py targets show <id>
python3 tools/coop/run.py cycle <id> --hypothesis "..." --next-change "..."
python3 tools/coop/run.py diff <unit> --symbol <sym>
python3 tools/coop/run.py size <unit>
python3 tools/coop/run.py ctx <source.cpp>
```

- Retail reference: `build/us/asm/kyoshin/`, `build/us/asm/monolib/src/`
- Log attempts to `docs/evidence/decomp/attempts.jsonl`
- Append reusable breakthroughs to `docs/MWCC_REFERENCE.md`
- Claims recorded in `docs/ownership.csv`

## Done criteria

Report: final match %, `cycle` exit code, size PASS/FAIL, files changed, and whether FULL_MATCH was reached. If stalled below acceptance, preserve the best candidate and report the exact mismatch categories, attempts, and next bounded experiments.
