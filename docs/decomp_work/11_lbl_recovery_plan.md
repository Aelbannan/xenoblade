# WS-6b — `lbl_*` data-label recovery plan

Remaining item from `frontier_unknowns.md`: **31 `lbl_*` data symbols** referenced by
the ready frontier (source: `tools/coop/frontier_refs.py report --top 400 --json`).

## Inventory (recon already done)

| Group | Labels | Section / unit | Nature |
|---|---|---|---|
| A | ~13 labels, `0x80518xxx` | `.rodata` in `RVL_SDK/.../nw4hbm/lyt/lyt_animation.s` (+ `HBMBase.s`) | float/double constants of the **HBM NW4R-lyt fork** — known library code |
| B | ~10 labels, `0x80560xxx`–`0x80563xxx` | `.data` in `PowerPC_EABI_Support/MetroTRK/__exception.s` etc. | **MetroTRK / MSL / runtime strings** — readable content, known libraries |
| C | `lbl_8065A0C8` (16B, align 8, float) | `.bss` | game global, likely Vec4/mtx — needs xref inference |
| D | `lbl_80666D3C__Q22cf9CfPadTask` (4B), `lbl_80665908` | `.sbss` / `.sdata` | **already mangled** — static member of `cf::CfPadTask`; demangle + check `CfPadTask` headers |

Reference counts are low (max 4, most 2) — expected prompt-quality payoff is **modest**
compared to the getter pass. Do this once, quickly; do not gold-plate.

## Risk tiers (drive sequencing)

1. **Tier 1 — strings (Group B), safest.** Names are annotations over readable
   content; zero codegen impact. Rename directly from string content + known
   MetroTRK/MSL sources.
2. **Tier 2 — rodata constants (Group A), safe.** Decomp source emits literals, not
   symbol refs, so renames can't break code. Name only **distinctive** values
   (skip `0.0f`/`1.0f` — no semantic content; name e.g. frame-time/quaternion
   constants). Cross-check names against public NW4R lyt sources.
3. **Tier 3 — SDA/bss globals (Groups C, D), delicate.** These are referenced via
   `@sda21` relocs and `extern` declarations in **already-matched** source (cf.
   `lbl_eu_80663D7C` in `CfRes.cpp`). Every rename must update all source `extern`
   refs and re-verify every referencing matched function with hexdiff. Respect the
   `bake_linker_addrs` policy — no insn patching.

## Per-label procedure

1. Identify all references: `grep -rn "lbl_XXXX" build/us/asm src/ include/`.
2. Infer semantics from ≤3 referencing functions (Tier 3) or content (Tiers 1–2).
   Timebox ~5 min/label; honest mechanical name beats no rename; skip beats wrong name.
3. Rename in `config/us/symbols.txt` (and `eu`/`jp` at the same address if present —
   the getter pass kept all three regions in sync).
4. Update `extern` declarations / references in `src/**`, `libs/**`, `include/**`.
5. Verify after each tier batch (not per label):
   - `python3 configure.py && ninja` — all compile steps pass (final LINK has a
     **pre-existing** failure, `ELF_gen.c:2802` — not a regression signal).
   - Tier 3 only: `python3 tools/coop/hexdiff.py <unit> --symbol <fn> --json` →
     `mismatch_count == 0` for every matched function referencing the label.
   - `python -m tools.ppc_equivalence.docs_sync --check` stays green.
6. Log one line per rename to `docs/evidence/decomp/attempts.jsonl`.

## Execution plan

Single sequential flash-agent chain (symbols.txt is a single-writer file), three tasks:

1. **Task 1 (Tier 1, ~10 labels):** MetroTRK/MSL strings. Read each string from the
   unit's `.s` data section, name from content. Expected examples: error/format
   strings. Verify build.
2. **Task 2 (Tier 2, ~13 labels):** HBM lyt_animation constants. Read float/double
   values, skip 0.0/1.0, name distinctive ones from NW4R lyt knowledge
   (`kAnimFrame...`, quaternion/PI constants etc.). Verify build.
3. **Task 3 (Tier 3, ~3 labels):** `lbl_80666D3C__Q22cf9CfPadTask` first (demangle →
   declare properly as `cf::CfPadTask` static member in the class header — it likely
   belongs next to the already-matched `CfPadTask` methods), then `lbl_8065A0C8`
   (Vec4-ish bss global; name from xrefs), then `lbl_80665908`. Hexdiff-verify every
   referencing matched function.

## Guardrails

- One writer at a time on `config/*/symbols.txt` and `tools/coop/targets.json`.
  Confirm no background probe sweep is running before Task 1 starts
  (`pgrep -f ws2_sweep`).
- No renames inside matched data pools where the name is load-bearing for reloc
  bake without a build+diff proving it's safe.
- A skip is a valid outcome — record skipped labels + reason in the report.

## Success metric

- ≥20/31 labels renamed, zero match regressions, build compiles.
- Re-run `python3 tools/coop/frontier_refs.py report --top 400 --json` → `lbl_*`
  count drops from 31 to ≤10.
