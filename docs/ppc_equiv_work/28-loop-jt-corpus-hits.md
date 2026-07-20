# 28 — Loop and jump-table pattern corpus hits

**Status:** baseline measurement (2026-07-20)  
**Tool:** `python -m tools.ppc_equivalence.census_loop_patterns`  
**Corpus:** `build/us/asm/**/*.s` from the parent working tree (1166 files, 19 311 functions; the isolated worktree has no local `build/us/asm`)

---

## Purpose

Track D productization needs retail (or linked-object) examples for newly supported equivalence patterns:

1. **Compare-affine** loops — `li` / body / `addi -1` / `cmpwi` / `bne`
2. **Memory-loop** constant-stride stores — `mtctr` / `stw|stwu|stb|sth` / `bdnz`
3. **Jump tables** — `cmplwi` / `slwi` / `lwzx` / `mtctr` / `bctr`, plus SDA / dual-base hints

Recognition reuses `loop_summary`, `memory_loop`, and `jump_table` finders. Counts describe **pattern applicability**, not proven `EQUIVALENT` retail functions.

---

## Method

```bash
python -m tools.ppc_equivalence.census_loop_patterns \
  /path/to/build/us/asm \
  --json build/ppc_equiv_work/loop-jt-corpus-census.json
```

Implementation:

- `tools/ppc_equivalence/census_loop_patterns.py` — asm parse, decode per `.fn`, engine finders, text heuristics
- `tools/ppc_equivalence/tests/test_census_loop_patterns.py` — inline fixture coverage

Decode failures (7 functions in SDK init / MetroTRK stubs) are listed in JSON and excluded from engine hits. Text heuristics (`mtctr`/store/`bdnz`, compare-affine latch) run per function block.

---

## Corpus summary

| Pattern | Engine hits | Exact-pattern | Text heuristic | Engine feature |
|---|---:|---:|---:|---|
| Compare-affine | 0 | 0 | 0 latch windows | `affine-loop-summary` / `compare-affine-closed-form` |
| Memory-loop | 4 | 0 | 686 store/`bdnz` windows | `memory-loop-summary` / `constant-stride-store` |
| CTR-affine (`mtctr`/`bdnz`) | 2 | 0 | (included above) | `affine-loop-summary` / `affine-closed-form` |
| Jump-table | 280 | 27 | — | `jump-table-auto` / `readonly-image` (descriptive) |
| Jump-table SDA (`lwzx` base `r2`/`r13`) | 0 | 0 | — | `jump_table_auto.resolve_table_base_va` |
| Dual-base hint | 1 | 0 exact | 1 text window | dual-table pairing in obligations |

### By unit (engine jump-table hits)

| Unit | Jump-table hits | Notes |
|---|---:|---|
| `kyoshin` | 164 | Dense UI/CF switch code (`CBattleManager.s`, `CItemBoxGrid.s`, …) |
| `RVL_SDK` | 74 | GX/DVD/BT stacks |
| `PowerPC_EABI_Support` | 15 | `printf` / exception runtime |
| `monolib` | 15 | VM/device layers |
| `CriWare` / `nw4r` | 6 / 0 exact | Codec ISR tables (255-case bounds) |

---

## Compare-affine (0 retail hits)

**Finding:** US retail assembly contains **no** decoded compare-affine countdown loops and **no** relaxed `addi -1` / `cmpwi` / `bne` latch windows. MWCC instead emits **`mtctr` / `bdnz`** counted loops (686 store bodies alone in `kyoshin`).

The engine path remains covered by synthetic fixtures (`test_loop_summary.py`). Retail validation should stay on decomp-generated candidates until a linked hit appears.

---

## Memory-loop (4 partial, 686 heuristic windows)

Engine recognition finds constant-stride store bodies but rejects discharge without a concrete trip count before `mtctr`. The four documented partial hits load CTR from **register arithmetic** (`srwi` / `andi` remainder tails), not `lwz` of a linked constant — **0/4 become exact** with DOL/ELF readonly hydration alone. Linked-image hydration (`memory_loop_image`) enables exact-pattern discharge when CTR materialization is `addis`/`addi`/`lwz` of a proven `.data`/`.rodata` word (covered by synthetic tests; retail `lwz` CTR loops are follow-up census targets).

| Unit | Symbol | Address | Notes |
|---|---|---|---|
| `RVL_SDK` | `__AXVPBInitCommon` | `0x802D7A38` | CTR = `mtctr r6` after `andi.` remainder |
| `RVL_SDK` | `OSExceptionInit` | `0x80355690` | CTR = `mtctr r3` after `andi.` remainder |
| `RVL_SDK` | `btsnd_hcic_pin_code_req_reply` | `0x802F1AF8` | not a store/`bdnz` loop (doc mis-tag) |
| `RVL_SDK` | (second slot in `AXVPB.s`) | `0x802D7A98` | CTR = `mtctr r5` after `andi.` remainder |

**Heuristic hot spots** (text `mtctr`/store/`bdnz`, not yet exact engine matches):

| File | Example header | Store |
|---|---|---|
| `kyoshin/CUICfManager.s` | `0x80131384` | `stb` fill loops |
| `kyoshin/code_8025FB10.s` | `0x80263A34` | `stw` init bursts |
| `kyoshin/CEquipItemBox.s` | `0x80284B40` | `stw` grid writes |

These are prime follow-up targets once CTR materialization rules widen.

---

## Jump-table (27 exact, 253 partial)

### Exact-pattern retail examples

| Unit | Symbol | `bctr` PC | Bound | Base reg | Feature |
|---|---|---:|---:|---|---|
| `kyoshin` | `func_8022A904` (`CQstLogInfo.s`) | `0x8022C8B8` | 13 | `r3` | readonly table image + indirect closure |
| `kyoshin` | `__ct__CMapSel` | `0x80245444` | 28 | `r3` | same |
| `kyoshin` | `func_80253B3C` (`CCollepedia.s`) | `0x80255EA4` | 25 | `r3` | same |
| `RVL_SDK` | `DVDCancel` | `0x803119E8` | 38 | `r3` | SDK dispatch |
| `RVL_SDK` | `GXSetVtxAttrFmtv` | `0x8031BD74` | 16 | `r6` | GX attr tables |
| `PowerPC_EABI_Support` | `__pformatter` | `0x802C8D28` | 83 | `r3` | MSL switch |
| `CriWare` | `mpvabdec_IntraBlock_Isr` | `0x803B28B8` | 255 | `r21` | ISR vector table |

Partial hits (missing `cmplwi` in lookback or tail mismatch) dominate game code — e.g. `kyoshin/cf/CBattleManager.s` (41 tail matches, mostly partial).

Existing retail DOL fixture at `0x800e0d8c` (`jump_table_retail.py`) aligns with the exact-pattern shape; asm census adds linked-symbol anchors above.

---

## Dual-base / SDA materialization

| Signal | Count | Example |
|---|---:|---|
| Exact jump table with `lwzx` base `r2`/`r13` | 0 | — |
| Exact jump tables with ≥2 distinct base regs in one function | 0 | — |
| Text window: multiple `lwzx` bases before `bctr` | 1 | `PowerPC_EABI_Support/.../ptmf.s` @ `0x802BC7F0` (`r12` vs `r3`) |

SDA-based table materialization (`addi`/`lwz` off `r2`/`r13` with `R_PPC_EMB_SDA21`) remains a **hydration** problem (see doc 27 ADDR32 census), not a recognizer gap — no exact SDA jump-table tail landed in this scan.

---

## Coop auto-context productization

Retail exact-pattern tails are recognized in asm, but coop equivalence only builds
`JumpTableProofContext` when **all** of the following succeed:

1. **Linked images** — retail `orig/<region>/sys/main.dol` for original-side table
   bytes; linked `build/<region>/main.elf` for candidate-side bytes when VAs differ.
2. **Extended text decode** — `jump_table_auto.expand_jump_table_instructions`
   re-decodes up to 512 bytes of linked text ending at the `bctr` PC so
   `addis`/`addi` chains outside the function slice still reach
   `resolve_table_base_va`.
3. **Hydration + pairing** — `hydrate_jump_table` loads aligned ADDR32 words;
   `pair_jump_table_cases` matches logical case indices.

**Probe helper** (requires DOL in environment; skips cleanly in CI when absent):

```bash
python -m tools.ppc_equivalence.jump_table_corpus_probe --documented
python -m tools.ppc_equivalence.jump_table_corpus_probe --dol orig/us/sys/main.dol --json
python -m tools.ppc_equivalence.jump_table_corpus_probe --branch-pc 0x8022c8b8
```

Offline regression: `tools/ppc_equivalence/tests/test_jump_table_corpus_probe.py`
(embeds the `CQstLogInfo` switch snippet without `main.dol`).

**Known gaps:** coop does not yet supply `sda_bases` / `symbol_addresses` for
SDA21 table materialization; `lwz rN, disp(r2/r13)` pointer-chase remains
fail-closed per `jump_table_auto.py`.

---

## Takeaways

1. **Compare-affine** is engine-ready but **absent from retail asm**; counted work uses **`mtctr`/`bdnz`** (`ctr-affine` / memory-loop tracks).
2. **Memory-loop** store shapes are **common** (686 windows) but retail CTR loads are often **non-literal** → partial only until materialization improves.
3. **Jump-table** exact hits are **real but sparse** (27); game/UI code contributes partial tails worth pairing with ADDR32 `.rodata` tables from doc 27.
4. **Dual-base / SDA** retail signals are **minimal** in this pass — continue synthetic + hydration tests before expecting corpus-backed proofs.

---

## Related

- Doc 27 — ELF data-section relocation census (`R_PPC_ADDR32` table words)
- `tools/ppc_equivalence/tests/fixtures/jump_table_retail.py` — documented DOL fixture
- `SOUNDNESS.md` — loop summary / memory-loop / jump-table proof boundaries
