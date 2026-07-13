# Batch 2026-07-14e — five new decomp targets

Read and follow `.cursor/skills/xenoblade-decomp/SKILL.md`, `docs/MWCC_REFERENCE.md` (esp. **§3 Retail SDA global names**), and `tools/test/compare_behaviour/README.md` if static match &lt; 100%.

## Policy

- Goal: **FULL_MATCH** via `python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."`
- High-level C/C++ only in `src/**` / `libs/**` (no `register rN`, no whole-function asm)
- Below 100%: mandatory behaviour host tests + `behaviour audit`
- Run `coop run size <unit>` — must PASS (under budget OK; trim TU extras if over)
- **Edit only your exclusive files / methods** — do not touch other batch targets' exclusive regions

## Targets (one agent each)

| id | symbol | address | size | unit | exclusive edit |
|----|--------|---------|------|------|----------------|
| worksysmem-ctor | `__ct__14CWorkSystemMemFPCcP11CWorkThread` | `0x80447250` | `0x60` | `monolib/src/work/CWorkSystemMem` | **only** ctor in `libs/monolib/src/work/CWorkSystemMem.cpp` (+ hpp) — leave `getHandle` / `wkStandby*` alone |
| battlestate-ctor | `__ct__Q22cf12CBattleStateFv` | `0x80146520` | `0xA8` | `kyoshin/cf/object/CBattleState` | create `src/kyoshin/cf/object/CBattleState.cpp` with ctor only (+ hpp layout); configure already lists the TU |
| aiaction-ctor | `__ct__Q22cf9CAIActionFv` | `0x8014B308` | `0x10C` | `kyoshin/cf/object/CAIAction` | create `src/kyoshin/cf/object/CAIAction.cpp` with ctor only (+ hpp); configure already lists the TU |
| menu-ptgauge-cbrender | `cbRenderBefore__12CMenuPTGaugeFv` | `0x80188A70` | `0xAC` | `kyoshin/menu/CMenuPTGauge` | create `src/kyoshin/menu/CMenuPTGauge.cpp` (+ hpp) with **only** `cbRenderBefore` — leave `Move` alone |
| menu-ptgauge-move | `Move__12CMenuPTGaugeFv` | `0x80188714` | `0x35C` | `kyoshin/menu/CMenuPTGauge` | **only** `Move` in `src/kyoshin/menu/CMenuPTGauge.cpp` (+ hpp) — leave `cbRenderBefore` alone |

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm

### `CWorkSystemMem::CWorkSystemMem` (`asm_ct_CWorkSystemMem.s`)
- Frame `-0x10`; `li r6,1` then `bl __ct__11CWorkThreadFPCcP11CWorkThreadi`
- Store vt `lbl_eu_8056BAA8`; `mHandle@0x1C4 = -1`; **`stw this → lbl_eu_80665620@sda21`**
- `getHandleMEM2()` → `create(handle, 0xfe0, mName.c_str())` → store `mHandle`
- Existing draft may already be close; fix SDA / `REGION_SIZE` / vt if needed. **Wrong:** writing `spInstance` reloc instead of `lbl_eu_80665620`
- TU `.text` budget `0x160` (splits `0x80447250..0x804473B0`) — do not bloat dtor/helpers

### `cf::CBattleState::CBattleState` (`asm_ct_CBattleState.s`)
- Frame `-0x20`; saves r29–r31; vt = `lbl_eu_8052E9B0`
- `sth 0` at `+4` and `+6`; `memset(this+8, 0, 0x1520)`
- Loop `p=this+0x152c` while `p < this+0x15ac`: `memset(p, 0, 0x10); p+=0x10`
- Then `memset(this+0x152c, 0, 0x80)` and `memset(this, 0, 0x15d4)` (matches retail opcode order — replicate control flow)
- Fix `CBattleState.hpp` size/fields (`0x15DC` class) as needed; stub only what ctor needs

### `cf::CAIAction::CAIAction` (`asm_ct_CAIAction.s`)
- Frame `-0x20`; vt = `lbl_eu_8052F598`; `sth 0` at `+8`
- Loop `p=this+0xc` while `p < this+0x20c`: `memset(p+4,0,0xe)` then `memset(p,0,0x20)`; `p+=0x20`
- Store `*(this+0x218)=0x10`; `*(this+0x20c)=this+0xc`; clear `0x214`/`0x210`
- Loop `q=this+0x21c` while `q < this+0xadc`: `memset(q,0,0xe)`; `q+=0xe`
- `memset(this+0xae0,0,0xe)`; `memset(this+0xadc,0,0x20)`
- Set `*(this+0xafc)=1`, `*(this+4)=1`, clear bytes/words at `0xb00` / `0xb10` / `0xb14` / `0xb18`
- Extend hpp field layout to match offsets (size ~`0xB1C`)

### `CMenuPTGauge::cbRenderBefore` (`asm_cbRenderBefore_CMenuPTGauge.s`)
- Frame `-0x70`; early-outs: `CTaskGame::getInstance()` → `func_800426F0` nonzero; **`lbl_eu_80663E28` bit 10** (`rlwinm.,0,10,10`); `func_8013BE50` null; `*(this+0x8C)==0`; **`lbl_eu_80663E24 & 0xAFA40000`** (`andis. …,0xafa4`)
- Then `GXSetZMode(0,0,0)`; stack `nw4r::lyt::DrawInfo` ctor; `func_80137250(&drawInfo)`; `func_80137038(*(this+0x74), &drawInfo, 0, 1)`; `DrawInfo` dtor `-1`
- Declare missing helpers/`DrawInfo` extern; create minimal class with fields `@0x74` (layout*) and `@0x8C`

### `CMenuPTGauge::Move` (`asm_Move_CMenuPTGauge.s`)
- Same gate prefix as `cbRenderBefore` (CTaskGame / `lbl_eu_80663E28` bit10 / `func_8013BE50` / `lbl_eu_80663E24` andis / `unk8C`)
- Switch on `*(this+0x8C)` cases `0` / `1` / … — follow asm control flow; large body with layout/anim helpers
- Share hpp with `cbRenderBefore` agent; **do not rewrite their method body**
- Declare unknown `bl` targets `extern` until identified

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

Report: final match %, `cycle` exit code, size PASS/FAIL, files changed, and whether FULL_MATCH was reached. If stuck below 100%, ship behaviour host coverage meeting the skill thresholds.
