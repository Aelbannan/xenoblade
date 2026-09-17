# Category C — Byte falsification (content rules)

**Hand to:** one or two strong matching agents. **Goal:** eliminate every
rule that changes bytes/relocations/data the honest build would not produce.
Each entry gets the real source shape, or its targets get demoted. Never
patch. Prerequisite reading: `docs/handoff/unit_rules/README.md`.

## Scope: 31 content entries, 10 live

Live (i.e. applied in the current `main.elf` link), in priority order:

| entry | fields | note |
|---|---|---|
| `OS.o` | `bake_linker_addrs`, `exact_renames`, `force_symbol_relocs` | probably representation-only ⇒ verify and hand to A |
| `OSThread.o` | `bake_linker_addrs` | representation ⇒ A |
| `__start.o` | `bake_linker_addrs` | representation ⇒ A |
| `CProc.o` | `exact_renames`, `extern_data_sections`, `inject_relocs`, `pad_data_section`, `pool_patterns`, `swap_data_blocks`, `trim_text_size` | mixed: content core is inject/swap/trim |
| `CTaskManager.o` | `drop_data_range`, `inject_relocs` | removing real retail data + manufacturing a slot |
| `mtx.o` | `swap_sdata2_leading_f32_words` | literal pool order |
| `AXFXReverbHiExp.o` | `swap_sdata2_leading_f32_pair` | literal pool order |
| `AXFXChorusExp.o`, `AXFXChorusExpDpl2.o` | `swap_data_blocks` | vtable/typeinfo emission order |
| `OSNet.o` | `swap_data_blocks` | vtable/typeinfo emission order |

Latent (same triage, not currently linked through the rule):
`CNBanner.o`, `CGXCache.o`, `CScn_80496B0C.o`, `KPAD.o`, `CTaskEnvironment.o`,
`code_804BC9EC.o`, `CView.o`, `CWorkThread.o`, `CChildListNode.o`,
`lyt_layout.o`, `lyt_pane.o#Q36nw4hbm3lyt`, `CDeviceFileCri.o`, `CDevice.o`,
`CDeviceClock.o`, `g3d_state.o`, `code_80135FDC.o`, `vi.o`, `WUD.o`,
`dct_isr.o`, `sfd_tim.o`, `rfc_mx_fsm.o`, `rfc_port_fsm.o`.

## Triage protocol (per entry)

1. **Byte-affecting or representation?** Comment out the entry's fields,
   rebuild via hexdiff, and diff the objects (`readelf -S/-r`, objdump words).
   - Only reloc presence/name/addend, alignment, or a linker-constant
     difference ⇒ hand to **Category A** with the evidence.
   - Actual instruction/data byte or section-size difference ⇒ continue.
2. **Find the source shape.** Use `hexdiff` (`--asm`, `--relocs`),
   `mwcc_kb.py search`, `docs/MWCC_CASES.md` / `MWCC_PATTERNS.md`, and
   `reloc_map.py`. Keep the code high-level C/C++.
3. **If impossible in C++**: record the residual as a near-miss
   (`attempts.jsonl` open-item packet), demote the affected target(s) in
   `targets.json`, delete the entry. §17.6 escapes only for cases already
   listed in PLAN.md, with owner approval and a `"policy_exception": true`
   log. "MWCC emits it in a different order" is **not** an exception.

## Per-field playbook

**`inject_relocs`** (manufactured vtable/typeinfo words — `CProc`,
`CChildListNode`, `CTaskEnvironment`, `CDevice`, `CDeviceClock`, `g3d_state`,
`WUD`, `lyt_pane#…`, `CTaskManager`): every injected reloc is a word retail
has and MWCC zeroed. Find why the word is missing:
`-RTTI` on/off per `Object()`, `__declspec(novtable)`, explicit vtable/typeinfo
definitions in the owning TU, base-list emission. If the word genuinely comes
from the original linker (unspellable in C++), accept the data drift —
EQUIVALENT_MATCH does not require data identity — and delete the rule.

**`swap_data_blocks`** (`AXFX*`, `CProc`, `CView`, `lyt_layout`, `lyt_pane#…`,
`OSNet`): MWCC emits globals-then-vtables; retail interleaved them. Levers:
definition order in source, splitting the unit, RTTI flags, explicit
definitions. If not reachable, accept drift.

**Pool words** (`permute_sdata2_words`, `swap_sdata2_*`,
`reverse_sdata2_trailing_f32x4`, `patch_unsigned_magic`,
`trim/pad_sdata2_size`): literal-pool content/order. Source first-use order
fixes many; the int→double magic pair cannot be spelled in C++ → accept the
drift at EQUIVALENT (semantics unaffected) and delete the rule. Never permute
bytes to fake the pool.

**`addend_patches` / `addend_sets`** (jumptable case-label addends: `KPAD`,
`CWorkThread`, `vi`, `code_804BC9EC`, `dct_isr`, `sfd_tim`, `rfc_*`): these
exist because the *function's code length differs from retail*; patching the
addend for an unmatched function falsifies the data comparison. Match the
function, or demote it and delete the rule.

**`drop_data_range` / `drop_data_tail` inside content entries**
(`CTaskManager`, `CNBanner`, `CGXCache`, `KPAD`, `CView`, `CWorkThread`,
`CDevice*`): deleting real retail data to dodge a layout mismatch. Recover
the data in source (types, order, ownership) or demote/delete the rule.
Coordinate with B: EDS entries remain D's scope until D's model lands.

**`force_symbol_relocs` / `bake_linker_addrs`**: verify the resolution proof
(see `a_representation.md`, worked cases 1-2) and hand to A when neutral.

**Zero-user legacy fields** (`copy_data_sections`, `insn_patches`,
`insn_patches_post`, `insert_insns`, `reloc_offset_moves`,
`data_reloc_offset_moves`): nothing to do today, but if any reappear treat
them as critical and delete them immediately.

## Method details

- Always rebuild before evaluating: `hexdiff` recompiles the unit; a stale
  `.o` lies. `hexdiff --all` is the fastest per-function table.
- The acceptance gate is `run.py cycle <target-id> --hypothesis "..."
  --next-change "..."` (witness runs inside; never `--smt`/`--linked`/plain
  `run.py diff` in this fork).
- Data side: `run.py data diff <unit>` is raw-only now. A unit only counts as
  data-matched when its unmodified object matches retail.
- If the content rule masks a *code* residual (e.g. `addend_*`), the correct
  outcome may be a recorded near-miss at the current match% — that is
  acceptable and expected.

## Evidence / definition of done

- Per entry: honest-vs-retail byte evidence, the source change or the
  demotion record, raw-gate output, affected targets.
- Append to `docs/evidence/decomp/attempts.jsonl`; per-target records to
  `docs/MWCC_CASES.md`; reusable recipes to `MWCC_PATTERNS.md`.
- `--claim "<key>" --owner <you> --class content --reason "..."` as you go;
  `--ratchet` after a reviewed batch.
- Done when the 10 live content entries are gone or demoted (with A handling
  the representation fields), no rule in this class changes a byte the source
  cannot justify, and every affected target has been recertified from raw.

**Report back:** per entry (deleted / demoted / handed to A / exception),
byte evidence summary, demoted target ids, and the new `--report` numbers.
