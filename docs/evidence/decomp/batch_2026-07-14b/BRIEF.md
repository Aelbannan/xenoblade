# Batch 2026-07-14b — five new decomp targets

Read and follow `.cursor/skills/xenoblade-decomp/SKILL.md`, `docs/MWCC_REFERENCE.md` (esp. **§3 Retail SDA global names**, camera `lbl_eu_80666268` float-0 patterns, CViewRoot `lbl_eu_806655D0`), and `tools/test/compare_behaviour/README.md` if static match &lt; 100%.

## Policy

- Goal: **FULL_MATCH** via `python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."`
- High-level C/C++ only in `src/**` / `libs/**` (no `register rN`, no whole-function asm)
- Below 100%: mandatory behaviour host tests + `behaviour audit`
- Run `coop run size <unit>` — must PASS (under budget OK)
- **Edit only your exclusive files** — do not touch other batch targets' sources

## Targets (one agent each)

| id | symbol | address | size | unit | exclusive edit |
|----|--------|---------|------|------|----------------|
| cam-dir-set-dir | `CfCamDirectionIntf_setDir` | `0x8006BE70` | `0x74` | `kyoshin/cf/CfCamDirectionIntf` | `src/kyoshin/cf/CfCamDirectionIntf.cpp` (+ hpp only if needed) |
| cam-tgt-set-pos | `CfCamTargetIntf_setPos` | `0x8006C030` | `0x30` | `kyoshin/cf/CfCamTargetIntf` | `src/kyoshin/cf/CfCamTargetIntf.cpp` (+ hpp) |
| cam-lookat-set-dir | `CamLookatIntf_setDir` | `0x8006BF94` | `0x74` | `kyoshin/cf/CfCamLookatIntf` | `src/kyoshin/cf/CfCamLookatIntf.cpp` (+ hpp) |
| mem-get-handle-mem2 | `getHandleMEM2__Q23mtl10MemManagerFv` | `0x80436D38` | `0x8` | `monolib/src/util/MemManager` | `libs/monolib/src/util/MemManager.cpp` (+ hpp) |
| viewroot-get-view | `getView__9CViewRootFUl` | `0x80445810` | `0xA0` | `monolib/src/core/CViewRoot` | **only** `CViewRoot::getView` in `libs/monolib/src/core/CViewRoot.cpp` (+ hpp) — leave `setCurrent`/`renderView`/`create`/`getFullScreenView` alone |

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm

### `CfCamDirectionIntf_setDir` (`asm_CfCamDirectionIntf_setDir.s`)
- Frame `-0x20`; saves r30=camEvent (r4), r31=dir (r5)
- `lfs f1, 0x284(r4)`; `addi r4, r4, 0x274`; `bl func_80074900` with out at `sp+8`
- Word-copy result → camEvent `0x268/0x26c/0x270` (`mLookat`); copy dir → `0x25c/0x260/0x264` (`unk25C`)
- Existing draft in TU may already be close — close reloc/stack gaps to FULL_MATCH

### `CfCamTargetIntf_setPos` (`asm_CfCamTargetIntf_setPos.s`)
- Leaf: load pos xyz from r5; `lfs f0, lbl_eu_80666268@sda21`; store pos to `0x28` and `0x274`; `stfs f0, 0x264(r4)` (unk25C.z = 0)
- Need `extern const f32 lbl_eu_80666268` (same pattern as sibling `setLookat`)

### `CamLookatIntf_setDir` (`asm_CamLookatIntf_setDir.s`)
- Same shape as dir setDir but `bl func_80074488` with `addi r4, r4, 0x268` (`&mLookat`); write result to `0x274/0x278/0x27c` (`unk274`); copy dir to `unk25C`

### `getHandleMEM2` (`asm_getHandleMEM2.s`)
- Exact retail: `lwz r3, lbl_eu_8066350C@sda21; blr`
- Thin `return sHandleMEM2;` is insn-identical but **wrong reloc name** — use `extern "C" ALLOC_HANDLE lbl_eu_8066350C;` and return that (MWCC_REFERENCE §3 / getHeapSize / getInstance pattern)

### `CViewRoot::getView` (`asm_getView_CViewRoot.s`)
- Frame `-0x10`; **r3 is the `Ul` work id** (saved r30); instance from **`lbl_eu_806655D0@sda21`** (not a `getInstance()` call if that yields wrong reloc)
- Walk `mChildren` node list (`mStartNodePtr` @0x60); each iter `bl getWorkThread__9CWorkUtilFUl` with id in r3
- On hit: type gate `*(thread+0x50)` in `[0x30, 0x35)` else null — mirror `CView::convertToView` semantics without breaking it
- Do **not** regress other CViewRoot symbols

## Shared repo resources

```bash
python3 tools/coop/run.py targets show <id>
python3 tools/coop/run.py cycle <id> --hypothesis "..." --next-change "..."
python3 tools/coop/run.py diff <unit> --symbol <sym>
python3 tools/coop/run.py size <unit>
python3 tools/coop/run.py ctx <source.cpp>
```

- Retail reference trees: `build/us/asm/kyoshin/cf/`, `build/us/asm/monolib/src/`
- Log attempts to `docs/evidence/decomp/attempts.jsonl`
- Append reusable breakthroughs to `docs/MWCC_REFERENCE.md`
- Claim already recorded in `docs/ownership.csv`

## Done criteria

Report: final match %, `cycle` exit code, size PASS/FAIL, files changed, and whether FULL_MATCH was reached. If stuck below 100%, ship behaviour host coverage meeting the skill thresholds.
