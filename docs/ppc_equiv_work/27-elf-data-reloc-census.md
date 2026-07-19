# 27 — ELF data-section relocation census

**Status:** baseline measurement (2026-07-20)  
**Tool:** `python -m tools.ppc_equivalence.census_elf_relocs`  
**Corpus:** `build/us/obj/**/*.o` from the parent working tree (1168 objects; the isolated worktree has no local `build/us/obj`)

---

## Purpose

Before designing jump-table support (`cmplwi` / `lwzx` / `bctr` through `R_PPC_ADDR32` tables in `.rodata` or `.data`), we need a corpus-wide picture of **where relocations land** and which relocation types appear outside `.text`.

This census scans every parseable ELF32 big-endian PowerPC relocatable object (`.o`), groups relocation entries by their **target section** (`SHT_REL` / `SHT_RELA` → `sh_info`), and counts entries by `r_type`.

---

## Method

```bash
python -m tools.ppc_equivalence.census_elf_relocs build/us/obj \
  --json build/ppc_equiv_work/elf-data-reloc-census.json
```

Implementation:

- `tools/ppc_equivalence/census_elf_relocs.py` — CLI, aggregation, JSON export
- `tools/ppc_equivalence/elf_symbols.py` — `list_section_relocations()` shared with function extraction

Parse failures are recorded per file; they do **not** contribute to totals. In this run: **0 failures / 1168 objects parsed**.

---

## Corpus summary

| Metric | Value |
|---|---|
| Objects scanned | 1168 |
| Objects parsed | 1168 |
| Objects with ≥1 relocation | 1111 |
| Total relocation entries | 198,353 |

### By section class

| Class | Relocation entries | Distinct `r_type` values |
|---|---|---|
| `.text` | 150,677 | 6 |
| Data-ish (`.rodata`, `.data`, `.sdata*`, `.ctors`, …) | 28,424 | 1 (`R_PPC_ADDR32` only) |
| Other (mostly `extab` / `extabindex`, plus `.init`) | 19,252 | 6 |

The “other” bucket is dominated by Metrowerks EABI **exception table** sections (`extab`, `extabindex`), not jump tables.

---

## Top relocation types: `.text` vs non-text

### `.text` (150,677 entries)

| `r_type` | Name | Count | Share of text relocs |
|---:|---|---:|---:|
| 10 | `R_PPC_REL24` | 87,797 | 58.3% |
| 109 | `R_PPC_EMB_SDA21` | 31,432 | 20.9% |
| 4 | `R_PPC_ADDR16_LO` | 16,365 | 10.9% |
| 6 | `R_PPC_ADDR16_HA` | 15,051 | 10.0% |
| 5 | `R_PPC_ADDR16_HI` | 29 | <0.1% |
| 11 | `R_PPC_REL14` | 3 | <0.1% |

These match the types already modeled for symbolic text relocations in `ppc_equivalence`.

### Non-text (47,676 entries)

| `r_type` | Name | Count | Notes |
|---:|---|---:|---|
| 1 | `R_PPC_ADDR32` | 47,644 | **Only** type in data-ish sections |
| 10 | `R_PPC_REL24` | 20 | Stray text-style relocs tagged to non-text sections (20 total) |
| 4–6, 109 | ADDR16 / SDA21 | 12 | Noise-level |

**Jump-table signal:** essentially all non-text relocations that matter for absolute word tables are `R_PPC_ADDR32`.

---

## `R_PPC_ADDR32` (type 1) in non-`.text` sections

| Target section | ADDR32 count | Likely role |
|---|---:|---|
| `.data` | 26,137 | Initialized globals, vtables, function-pointer tables |
| `extabindex` | 16,542 | C++ EH index (Metrowerks) |
| `extab` | 2,678 | C++ EH descriptors |
| `.sdata` | 1,341 | Small-data absolute words |
| `.rodata` | 788 | **Read-only absolute tables (jump-table candidates)** |
| `.ctors` | 125 | Static constructor lists |
| `.sdata2` | 30 | Small-data 2 |
| `.ctors$10` / `.dtors$10` / `.dtors$15` | 3 | Compiler-generated lists |

Excluding `extab` / `extabindex`: **28,424** ADDR32 entries remain in ordinary data sections.

### `.rodata` ADDR32 (788 entries, 13 object files)

Only **13** object files carry ADDR32 into `.rodata`. Game code (`kyoshin/`) accounts for **12** of those entries (all in `kyoshin/main.o`). Most `.rodata` ADDR32 volume lives in SDK / monolithic data objects (`split1.o`, `criware_data.o`, `nw4r_data.o`, …).

### `.data` ADDR32 in `kyoshin/` (game code)

Among decomp game objects, ADDR32 in `.data` is rare:

| Object | `.data` ADDR32 |
|---|---:|
| `kyoshin/CGame.o` | 87 |
| `kyoshin/main.o` | 40 |

Most `.data` ADDR32 mass comes from `split1.o`, SDK libraries, and static data blobs—not per-function translation units.

---

## Example files with non-text ADDR32

Paths relative to `build/us/obj/` (sorted by total non-text ADDR32):

| File | Total ADDR32 (non-text) | Sections |
|---|---:|---|
| `split1.o` | 18,126 | `.data`, `.rodata`, `.sdata`, `.sdata2` |
| `monolibdata2.o` | 3,095 | `.data`, `.sdata` |
| `criware_data.o` | 1,159 | `.data`, `.rodata` |
| `nw4r_data.o` | 1,026 | `.data`, `.rodata`, `.sdata` |
| `monolibdata1.o` | 774 | `.data`, `.sdata` |
| `kyoshin/CItemBoxGrid.o` | 379 | `.ctors`, `extab`, `extabindex` |
| `kyoshin/makecrystal/CModelDispMakeCrystal.o` | 377 | `.ctors`, `extab`, `extabindex` |
| `kyoshin/cf/CfGameManager.o` | 355 | `.ctors`, `extab`, `extabindex` |
| `kyoshin/code_800B06A4.o` | 280 | `.ctors`, `extab`, `extabindex` |
| `RVL_SDK/.../l2c_csm.o` | 261 | `.data` |
| `kyoshin/CGame.o` | (see above) | `.data` |
| `kyoshin/main.o` | 52 total non-extab ADDR32 | `.data`, `.rodata` |

Many high-ranking `kyoshin/*.o` files are **EH metadata**, not switch tables.

---

## Implications for jump-table patterns

### 1. ADDR32 tables exist but are structurally split

- **788** `.rodata` ADDR32 words across the whole corpus → switch/jump tables compiled to read-only storage are **uncommon** in individual `.o` files.
- **26k** `.data` ADDR32 words → mostly static initialization and SDK data; a `lwzx`/`bctr` dispatch may point at `.data` tables that were fully linked before extraction.
- Proofs on **relocatable** `.o` bytes will often still see unresolved ADDR32 in data sections even when retail `.text` already contains concrete addresses post-link.

### 2. `cmplwi` / `lwzx` / `bctr` pattern needs a data-section oracle

Current `ppc_equivalence` resolves text relocations symbolically but rejects unresolved function bytes (`require_relocation_free`). Jump-table loads consume **absolute words** from `.rodata`/`.data`:

- At the object-file layer, each table slot is an `R_PPC_ADDR32` targeting a function symbol (or section-relative label).
- A sound model must either:
  - supply **linked** table words (DOL/ELF slice), or
  - treat table entries as opaque function-pointer symbols with explicit caller contracts.

Without that, `lwzx` + `bctr` through an ADDR32-backed table will stay **inconclusive** on raw `.o` files.

### 3. Exception tables dominate “other” ADDR32

~19k ADDR32 entries in `extab`/`extabindex` are **not** jump tables. Any future “data relocation” profile should classify EH sections separately so they do not pollute switch-table heuristics.

### 4. Text-side coverage is already the main line

For dispatch **code**, the hot reloc types remain `R_PPC_REL24`, `R_PPC_EMB_SDA21`, and ADDR16 pairs—already in scope. Data-section ADDR32 is the **new** surface area for indirect control flow.

### 5. Suggested next steps (design only — not implemented here)

1. **Table discovery:** pair `cmplwi`/`lwz`/`lwzx` sequences with nearby `.rodata`/`.data` symbols whose relocs are all `R_PPC_ADDR32`.
2. **Linked fallback:** extend the existing DOL/ELF linked-byte path to hydrate table words, mirroring text `require_relocation_free` bypass.
3. **Proof contract:** bounded “jump-table entry sets” (finite domain of target labels) rather than arbitrary memory loads.
4. **Re-run census** after major rebuilds: `python -m tools.ppc_equivalence.census_elf_relocs build/us/obj --json …`

---

## Reproduce

```bash
# From repo root with build/us/obj present:
python -m unittest tools.ppc_equivalence.tests.test_census_elf_relocs -v
python -m tools.ppc_equivalence.census_elf_relocs build/us/obj \
  --json build/ppc_equiv_work/elf-data-reloc-census.json
```

JSON output is gitignored under `build/`; regenerate as needed.
