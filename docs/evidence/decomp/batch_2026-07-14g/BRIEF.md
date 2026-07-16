# Batch 2026-07-14g — five new decomp targets

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
| battlestate-vfunc6 | `CBattleState_UnkVirtualFunc6__Q22cf12CBattleStateFv` | `0x80148210` | `0x154` | `kyoshin/cf/object/CBattleState` | **only** `CBattleState_UnkVirtualFunc6` in `src/kyoshin/cf/object/CBattleState.cpp` (+ hpp if needed) — leave ctor alone |
| aiaction-vfunc2 | `CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv` | `0x8014B52C` | `0x110` | `kyoshin/cf/object/CAIAction` | **only** `CAIAction_UnkVirtualFunc2` — leave ctor / UnkVirtualFunc1 alone |
| menu-bps-cbrender | `cbRenderBefore__22CMenuBattlePlayerStateFv` | `0x8010D6F4` | `0x1B4` | `kyoshin/menu/CMenuBattlePlayerState` | **only** `cbRenderBefore` — leave `Term` alone |
| menu-arts-cbrender | `cbRenderBefore__15CMenuArtsSelectFv` | `0x8010491C` | `0x3C0` | `kyoshin/menu/CMenuArtsSelect` | **only** `cbRenderBefore` — leave `Term` alone |
| window-mgr-move | `Move__16CUIWindowManagerFv` | `0x8013D0C8` | `0x4DC` | `kyoshin/CUIWindowManager` | create `src/kyoshin/CUIWindowManager.cpp` with **only** `Move` (+ extend hpp) |

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm

### `cf::CBattleState::CBattleState_UnkVirtualFunc6` (`asm_CBattleState_UnkVirtualFunc6.s`)
- Mangled `Fv` but retail uses **`r3=this`, `r4=arg`** (struct with `+0xc` half, `+0x10` word, `+0x18` signed half, etc.)
- Sets bit into `this+(rlwinm half)+0x15ac` from `arg+0xc`
- Walks 8 entries of stride `0x34` starting at `this+0x1388`; on matching `+0xc` halves, clamps `+0x10`, then **tail-calls** virtual at vt+0x48 with that entry
- Else fills first free slot (half at `+0xc` == 0) from `arg`, then same virtual tail-call
- Prefer `extern "C"` multi-arg entry like CAIAction vfunc1 pattern if needed

### `cf::CAIAction::CAIAction_UnkVirtualFunc2` (`asm_CAIAction_UnkVirtualFunc2.s`)
- Inverse of UnkVirtualFunc1: clears `unk214`/`unk210`, copies **into** trailer `0xadc` from `*r4` (slot layout), loops `i < outB->unk208` copying from `outB->buffer` ring into `this->unk20C` slots
- Same `r4`/`r5` ABI as vfunc1 — add `extern "C" CAIAction_UnkVirtualFunc2__…` alongside existing vfunc1
- **Do not edit** UnkVirtualFunc1 body or ctor

### `CMenuBattlePlayerState::cbRenderBefore` (`asm_cbRenderBefore_CMenuBattlePlayerState.s`)
- Same gate prefix as PTGauge/Enemy: `CTaskGame::getInstance` → `func_800426F0` nonzero early-out; **`lbl_eu_80663E28` bit 10**; `func_8013BE50` null; **`this+0x7c9` nonzero**; **`lbl_eu_80663E24 & 0xAFE40000`**
- `GXSetZMode(0,0,0)`; stack `DrawInfo` + `func_80137250`; loop `i=0..` stride `0x270` drawing layouts at slot offsets via `func_80137038` gated by flags at slot `+0x2d0`
- Leave `Term` untouched; extend hpp for `cbRenderBefore` + field `0x7c9` if needed

### `CMenuArtsSelect::cbRenderBefore` (`asm_cbRenderBefore_CMenuArtsSelect.s`)
- Same early gates + `func_8018A608` / `func_80122448` nonzero early-outs; mask **`0xAFA40000`**
- DrawInfo path; conditional loop over 9 layouts at `+0x1b8` keyed by `this+0x30c` bits; draw `+0x80` with flag-derived arg6 from `+0x308`; more layout draws — follow full asm
- Leave `Term` untouched; add fields used by render (`0x298`, `0x308`, `0x30c`, …)

### `CUIWindowManager::Move` (`asm_Move_CUIWindowManager.s`)
- Frame `-0x50`; early-out if **`lbl_eu_80664088`** null
- Walks linked lists under that SDA object (`+0x60`, `+0x80`) writing timer half to **`lbl_eu_8066408C`**
- Then walks `this+0x60` window list for focus/modal flags (`+0x65`, `this+0xa1`, …) — large state machine; mirror asm control flow in high-level C++
- configure.py already lists the TU; create the `.cpp` implementing `Move` only
- SDA: use `lbl_eu_80664088` / `lbl_eu_8066408C`, not mangled aliases

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
