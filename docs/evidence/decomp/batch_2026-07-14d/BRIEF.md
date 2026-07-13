# Batch 2026-07-14d — five new decomp targets

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
| task-culling-func-801A2C04 | `func_801A2C04__Q22cf12CTaskCullingFv` | `0x801A4324` | `0x1C` | `kyoshin/cf/CTaskCulling` | **only** `func_801A2C04` in `src/kyoshin/cf/CTaskCulling.cpp` (+ hpp if sig) — leave `ICulling_UnkVirtualFunc1` alone |
| uicf-init | `Init__12CUICfManagerFv` | `0x80132EC8` | `0x2E0` | `kyoshin/CUICfManager` | **only** `Init` in `src/kyoshin/CUICfManager.cpp` (+ hpp) — leave `Term` alone |
| occ-cull-helper | `func_801A1188__11COccCullingFP12CCullFrustum` | `0x801A28A4` | `0x2BC` | `kyoshin/COccCulling` | **only** `func_801A1188` in `src/kyoshin/COccCulling.cpp` (+ hpp) — leave `addFrustum` alone |
| arts-set-vfunc1 | `CArtsSet_UnkVirtualFunc1__Q22cf8CArtsSetFv` | `0x80154740` | `0x9C` | `kyoshin/cf/CArtsSet` | `src/kyoshin/cf/CArtsSet.cpp` (+ hpp) |
| battle-mgr-init | `Init__16CUIBattleManagerFv` | `0x8012EEFC` | `0xB4` | `kyoshin/CUIBattleManager` | create `src/kyoshin/CUIBattleManager.cpp` with `Init` (+ update hpp); configure already lists the TU |

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm

### `func_801A2C04` (`asm_func_801A2C04.s`)
- Exact: `lwz r3, lbl_eu_80664328@sda21`; null → `li r3,0; blr`; else `lwz r3, 0x94(r3); blr`
- **Wrong:** `spInstance` mangled reloc — use `extern "C" cf::CTaskCulling* lbl_eu_80664328;` then `lbl_eu_80664328->unk94` (same pattern as ICulling_UnkVirtualFunc1)

### `CUICfManager::Init` (`asm_Init_CUICfManager.s`)
- Large frame `-0x1A0`; `stmw r22`
- `readFile__(unk118, lbl_eu_806621A8, IWorkEvent*, 0, 0)` → store `mFileHandle@0x114`; `func_8044F154(handle, 3)`
- Allocate `0x54` via `getWorkMem` + `MemManager::allocate`; optional `CProcess` ctor + vtable/`__ptmf_null` PTMF block → store `unk144`; `Regist(proc, lbl_eu_80664054, false)`
- Loop `i=0..?` with `mulli …,0x168` into per-slot embeds at `this+0x150…`; stack template clears; calls `func_8015704C` / helpers — match control flow from asm; declare missing helpers `extern`

### `COccCulling::func_801A1188` (`asm_func_801A1188.s`)
- Set `mInFirstList=1`; if `(mFlags & 3)==0` plane loop with `psq_l` / `lbl_eu_80667C8C`; early return keeps `mInFirstList=1`
- Else clear `mInFirstList`, `PSMTXMultVec` on `unk24+0xCC`, store `-z` to `unk124`, direction/dot gate, five `CPlane::set` arms
- Existing draft in `COccCulling.cpp` is structural — focus on float/`ps` shape + `lbl_eu_80667C8C` SDA; do not regress `addFrustum` `lbl_eu_80667C88`

### `CArtsSet_UnkVirtualFunc1` (`asm_CArtsSet_UnkVirtualFunc1.s`)
- Symbol name is **`CArtsSet_UnkVirtualFunc1`** (not `_sArtsSet_UnkVirtualFunc1`) — fix mangling to match `symbols.txt`
- `sth 0` at `+0`; `memset(this+4, 0, 0x30)`; nested loops `row=0..2`, `col=0..7` calling `vt+8` on entries at `this+0x38 + row*0x460 + col*0x8C` (24× `CArtsParam`)

### `CUIBattleManager::Init` (`asm_Init_CUIBattleManager.s`)
- Mirror Cf Init process-alloc path: `getWorkMem` → `allocate(0x54)` → optional `__ct__8CProcessFv` + `lbl_eu_8052E208`/`lbl_eu_8052E1C0` vt + `__ptmf_null` PTMFs → `this->unk7C = proc`; `Regist(proc, lbl_eu_80664048, false)`
- Extend hpp with `Init()` + field `unk7C` at `0x7C` if missing; stub minimal create/linkage only as needed for this symbol

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
- Claims recorded in `docs/ownership.csv`

## Done criteria

Report: final match %, `cycle` exit code, size PASS/FAIL, files changed, and whether FULL_MATCH was reached. If stuck below 100%, ship behaviour host coverage meeting the skill thresholds.
