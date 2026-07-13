# Batch 2026-07-14c — five new decomp targets

Read and follow `.cursor/skills/xenoblade-decomp/SKILL.md`, `docs/MWCC_REFERENCE.md` (esp. **§3 Retail SDA global names**), and `tools/test/compare_behaviour/README.md` if static match &lt; 100%.

## Policy

- Goal: **FULL_MATCH** via `python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."`
- High-level C/C++ only in `src/**` / `libs/**` (no `register rN`, no whole-function asm)
- Below 100%: mandatory behaviour host tests + `behaviour audit`
- Run `coop run size <unit>` — must PASS (under budget OK; trim TU extras if over)
- **Edit only your exclusive files** — do not touch other batch targets' sources

## Targets (one agent each)

| id | symbol | address | size | unit | exclusive edit |
|----|--------|---------|------|------|----------------|
| scn-draw | `Draw__4CScnFv` | `0x8049A918` | `0x120` | `monolib/src/scn/CScn` | **only** `CScn::Draw` in `libs/monolib/src/scn/CScn.cpp` (+ hpp) — leave `addRenderCB`/`removeRenderCB` alone |
| cam-init-intf | `CfCamEvent_initCamIntfInstances` | `0x8006B3B8` | `0x98` | `kyoshin/cf/CfCamEvent` | create/fill `src/kyoshin/cf/CfCamEvent.cpp` (+ hpp if needed); configure already lists the TU |
| worksysmem-get-handle | `getHandle__14CWorkSystemMemFv` | `0x80447308` | `0xC` | `monolib/src/work/CWorkSystemMem` | `libs/monolib/src/work/CWorkSystemMem.cpp` (+ hpp) — restore size budget (`0x160`) if needed without breaking `wkStandbyLogout` |
| task-culling-vfunc1 | `ICulling_UnkVirtualFunc1__Q22cf12CTaskCullingFPQ22ml8CFrustum` | `0x801A4340` | `0x20` | `kyoshin/cf/CTaskCulling` | **only** that method in `src/kyoshin/cf/CTaskCulling.cpp` — leave `func_801A2C04` alone; shrink TU `.text` if size FAIL |
| uicf-term | `Term__12CUICfManagerFv` | `0x80133200` | `0xA4` | `kyoshin/CUICfManager` | create `src/kyoshin/CUICfManager.cpp` with `Term` (+ update hpp); configure already lists the TU |

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm

### `CScn::Draw` (`asm_Draw_CScn.s`)
- Frame `-0x20`; saves r29=this, r30/r31
- Gate: `func_8049AF80(*(this+0x68))`; then `func_8049B0A0` with `extsh(*(work+0x34))`; must equal `CView::getCurrentView()`
- Early-out if `unk_3E4` (byte@0x3E4) nonzero
- Store `this` to **`lbl_eu_80665908@sda21`** before CB walk; clear it after
- Optional dual-flag path `@0x3E5`/`@0x3E6` → virtual call `*(this+0x8C)` vtable+0x18
- Loop `i=0..mRenderCBCount`: if `@0x3E5` or CB flag `@0xC4` → `cb->vt+0xC(cb, this)` (render callback)
- Set byte `@0x3E8=1`; final virtual `*(this+0x8C)` vtable+0x1C; epilogue also sets `@0x3E8=1` on early out
- Extend `CScn.hpp` for new fields/methods at 0x68/0x8C/0x3E5.. if missing — keep `ScnRenderCB` layout

### `CfCamEvent_initCamIntfInstances` (`asm_CfCamEvent_initCamIntfInstances.s`)
- Three once-guards: `lbl_eu_80663DB0`/`DB8`/`DC0` (s8) init pointers `lbl_eu_80527130`→`80663DB4`, `80527108`→`80663DBC`, `805270D8`→`80663DC4`
- Then switch on `*(r3+8)`: `8`→return `&lbl_eu_80663DBC`; `9`→`&lbl_eu_80663DB4`; `0xA`→`&lbl_eu_80663DC4`; else `bnelr`
- C-linkage name exact: `CfCamEvent_initCamIntfInstances`
- Minimal TU OK if only this symbol is needed for match; declare extern SDA symbols with retail names

### `getHandle` (`asm_getHandle_CWorkSystemMem.s`)
- Exact: `lwz r3, lbl_eu_80665620@sda21; lwz r3, 0x1c4(r3); blr`
- **Wrong:** `return spInstance->mHandle;` yields `spInstance` reloc — use `extern "C" CWorkSystemMem* lbl_eu_80665620;` then `return lbl_eu_80665620->mHandle;` (MWCC_REFERENCE §3)
- TU currently **over budget** (`0x260` vs `0x160`) — trim non-split helpers / weak dtors / out-of-line as prior units did; do not regress JP `MatchingFor("jp")` path carelessly

### `ICulling_UnkVirtualFunc1` (`asm_ICulling_UnkVirtualFunc1.s`)
- Load instance from **`lbl_eu_80664328@sda21`** (not `spInstance` mangled)
- Null → return false/`0`; else `addi r3,r3,0x60; b func_801A0F04__11COccCullingFPQ22ml8CFrustum` (tail)
- Current draft uses `spInstance` + call through `mOccCulling` — fix reloc + match tail
- Unit **over budget** (`0x878` vs `0x70C`) — shrink elsewhere in file if needed; still only edit this method's body unless size requires shared trim

### `CUICfManager::Term` (`asm_Term_CUICfManager.s`)
- Frame `-0x10`; teardown: `func_801390E0(&this->unk114)`; three× `PackedFont::Destroy` + `MemManager::deallocate` at `+0x60/+0x9C/+0xD8`
- `func_80139124(unk5C)`; clear `unk5C`; set `*(this+0x118)=-1`
- IFlagEvent path: if this nonzero, `r31=this+0x58`; `func_8009D0B4()` then `func_8009D514(r31)`
- `*(unk144+0x39)=1`; clear **`lbl_eu_80664054@sda21`**
- Create stub `.cpp` implementing only what is needed for this symbol + necessary linkage; declare missing helpers `extern`

## Shared repo resources

```bash
python3 tools/coop/run.py targets show <id>
python3 tools/coop/run.py cycle <id> --hypothesis "..." --next-change "..."
python3 tools/coop/run.py diff <unit> --symbol <sym>
python3 tools/coop/run.py size <unit>
python3 tools/coop/run.py ctx <source.cpp>
```

- Retail reference trees: `build/us/asm/kyoshin/`, `build/us/asm/monolib/src/`
- Log attempts to `docs/evidence/decomp/attempts.jsonl`
- Append reusable breakthroughs to `docs/MWCC_REFERENCE.md`
- Claim already recorded in `docs/ownership.csv`

## Done criteria

Report: final match %, `cycle` exit code, size PASS/FAIL, files changed, and whether FULL_MATCH was reached. If stuck below 100%, ship behaviour host coverage meeting the skill thresholds.
