# Instruction selection & peephole — the copy/load lowering

How MWCC picks the concrete PowerPC instruction for a source operation, and the
peephole transforms it applies on top. This is the layer behind the `mr` vs
`addi rD,rS,0`, `lbzu`-merge, and `li`-float residuals.

**Source of truth:** `mwcc-decomp` `src/backend/{Operands.c,PCodeUtilities.c}`
(frontend lowering) plus the peephole flag vocabulary recovered from the Wii/1.1
binary. Rules verified by compiling probes and diffing the PPC (`.scratch/isel_probe.c`).

## When to read this

- **Read when:** hexdiff shows `mr` vs `addi rD,rS,0`; `lbzu` present/missing on one
  side; load width/signedness differs (`lha`/`lhz`/`lbz`+`extsb` vs `lwz`); or
  float-constant materialisation (`li`-float vs `lfs`).
- **Skip it when:** pure `reg_swap` with identical instructions → `register_mapping.md`.

## Fast path — first moves in order

1. **`mr` in retail, `addi rD,rS,0` in decomp** → TU is on the wrong compiler version: set `mw_version="Wii/1.1"` (Wii/1.1 always lowers a copy to `mr`). No source lever exists.
2. **Retail `addi rD,rS,0` (zero immediate)** → unreachable from high-level source (the peephole rewrites it) — §17.6 `opword` patch or restructure; do not hunt a source shape.
3. **`lbzu` vs `lwz`+`addi`** → loop unroll shape: `-O4,s`/`-O4,p` (see `scheduling.md`).
4. **Load width/signedness** → source type: signed byte `lbz`+`extsb`, `u16` `lhz`, `s16` `lha`, word `lwz`.
5. **`p+N` split into `addi`+load** → a non-folded address (walked pointer / volatile read); constant offsets fold into the load immediate.

## TL;DR

1. **A register copy is `mr` in Wii/1.1, `addi rD,rS,0` in GC/1.2.5** (the
   prologue param-save specifically). Wii/1.1 added an **unconditional
   `addi rD,rS,0 → mr` peephole**; GC/1.2.5 kept the `addi +0` form for copies.
2. **Retail (Wii/1.1) therefore has no `addi rD,rS,0` with a zero immediate** —
   every copy is `mr`, and every `addi` has a non-zero immediate or a symbol
   relocation. A retail `addi rD,base,0` is the known wall (§17.6 `opword`
   workaround).
3. **Load/store selection is type-driven** (`Operands_SelectGPRLoad`):
   `lbz`(+`extsb`) for signed byte, `lhz` for unsigned short, `lha` for signed
   short, `lwz` for word; `lfs`/`lfd` for float/double.
4. **`p + N` folds into the load/store immediate** (`*(p+3)` → `lwz r0,12(r3)`) —
   no `addi` is materialised for constant pointer arithmetic.

## The two layers

Instruction selection happens in two places:

1. **Frontend lowering** (`Operands.c`, `PCodeUtilities.c`) — the first pass
   chooses the basic instruction from the operand's type. Decompiled in
   `mwcc-decomp`.
2. **Peephole optimization** — the forward/final peephole passes rewrite the
   lowered PCode (`addi +0 → mr`, `lwz + addi → lwzu`). Not yet decompiled; gated
   by the `peephole` / `globaloptimizer` flags.

## Frontend lowering (Operands.c)

`Operands_SelectGPRLoad` maps a type to its load:

| type | instruction |
|---|---|
| `char`/`u8` (size 1) | `lbz` (+ `extsb` when signed) |
| `u16` (size 2, unsigned) | `lhz` |
| `s16` (size 2, signed) | `lha` |
| `u32`/`s32`/word | `lwz` |
| indexed variants | `lbzx`/`lhzx`/`lhax`/`lwzx` |

Verified empirically (`isel_loads`): `signed char` → `lbz`+`extsb`, `u16` →
`lhz`, `s16` → `lha`. `Operands_ForceGPR`/`Operands_ForceFPR` materialise a value
in a GPR/FPR (`lfs`/`lfd` for float/double). Float constants load from the
`.sdata2`/`.rodata` constant pool via `lfs` (reloc `@42`), not `li`.

## The copy/move lowering — `mr` vs `addi rD,rS,0`

Compiled `isel_copy` (`int y = x; call(); return y;`):

```
Wii/1.1:                       GC/1.2.5:
mr   r31, r3        <- copy     addi r31, r3, 0    <- copy (prologue save)
li   r3, 0                     li   r3, 0
bl   ext_call                  bl   ext_call
mr   r3, r31        <- return   mr   r3, r31        <- return (same!)
```

The prologue param-save is the discriminant: GC/1.2.5 lowers it as `addi +0`,
Wii/1.1 lowers it as `mr` (`or rD,rS,rS`). The return is `mr` in both. So the
`addi rD,rS,0 → mr` peephole is a **Wii/1.1 addition** that GC/1.2.5 lacks.

Consequence, verified against retail (`WPADHIDParser.s`):

- copies → `mr r27, r3`, `mr r28, r4`;
- address + non-zero immediate → `addi r3, r29, 0x5ec`, `addi r0, r3, 1`;
- address + symbol → `addi r30, r30, __rvl_p_wpadcb@l`;
- **no `addi rD,rS,0` (zero immediate, both operands non-zero) exists in retail.**

So: a retail `addi rD,rS,0` is **unreachable from high-level Wii/1.1** — the
peephole always rewrites it to `mr`. This is `KNOWN_WALLS.md` wall #4; the only
accepted workarounds are the §17.6 `opword 0x387E0000` patch or restructuring to
make the two operands distinct.

## `lbzu` merge (load/store with update)

The forward peephole fuses a load/store immediately followed by its own pointer
increment into the `u` (update) form: `lwz rX,0(rY); addi rY,rY,4` → `lwzu
rX,4(rY)`. It only fires when the increment is the *next* use of the pointer and
the increment is the register size. It does **not** fire in an unrolled body
(the `addi r4,r4,32` in an 8× unroll is separated from any single load, so the
probe emitted `lwz … + addi r4,r4,32`, not `lwzu`). This is the same "merge the
pointer add into `lbzu`" behavior the reference records for `l2cu_find_ccb_by_cid`
(GC merges the pointer add into `lbzu`; Wii/1.1 and GC-with-IPA do not, under
specific `-ipa`/version combinations).

## Flags (recovered from the Wii/1.1 binary)

- `peephole` — forward/final peephole (positive-only flag, implied at O4).
- `globaloptimizer` — the level-4 backend optimizer that feeds the peephole.
- `opt_partial|r1`, `limitedoptlevel` — partial/limited optimization levels that
  disable peephole-style passes.
- `ppc_dont_balance_ir`, `ppc_dont_balance_exp` — IR/expression balancing (affect
  the `addi`/`or` selection and immediate folding).
- `use_isel`, `gen_isel` — Freescale ISEL (not Gekko).
- `remove_frsp_aggressive` — float-rounding `frsp` elimination.

## Actionable rules for matching

1. **`mr` vs `addi rD,rS,0` is a compiler-version instruction-selection
   difference, not a source lever.** Wii/1.1 always emits `mr` for a copy; use
   `mw_version="Wii/1.1"` for retail Wii/1.1 TUs. A `mr` in retail and `addi +0`
   in decomp means the TU is on the wrong `mw_version`.
2. **A retail `addi rD,rS,0` (zero immediate) is a wall** — the peephole rewrites
   it. Reach it only via the §17.6 `opword` patch or by making the operands
   distinct; do not spend calls hunting a source shape.
3. **Constant pointer arithmetic folds into the offset** — `*(p+3)` emits
   `lwz r0,12(r3)`, never `addi rD,rS,12; lwz r0,0(rD)`. If retail has the split
   form, the source has a non-folded address (a walked pointer or a volatile
   read), not `p+3`.
4. **`lbzu` merge is the peephole's pointer-increment fusion** — it fires only on
   an adjacent load + register-size increment. Unrolled loops never show it; a
   single-step pointer walk can. Match the retail loop shape (`-O4,s` vs `-O4,p`,
   see `scheduling.md`) to control whether the body is unrolled and therefore
   whether `lbzu` can appear.
5. **`li`-float / return-constant floats** are a separate float-materialisation
   family (wall #12) — return of a constant float reuses an integer `li`/`lfs`
   path; treat it as float-constant codegen, not as instruction selection.

## Evidence basis

- **Lowering**: `Operands_SelectGPRLoad` (size 1/2 + signedness → LBZ/LHZ/LHA/LWZ)
  from `Operands.c`; `Operands_ForceGPR`/`ForceFPR`, `PCodeUtilities_BuildInstructionV`.
- **Copy lowering**: `isel_copy` compiled at Wii/1.1 vs GC/1.2.5 (prologue save
  `mr` vs `addi +0`).
- **Retail confirmation**: `WPADHIDParser.s` uses `mr` for copies, `addi` only
  for non-zero immediates / symbol relocs; zero `addi rD,rS,0` in retail.
- **Peephole flags**: `peephole`, `globaloptimizer` recovered from the flag table.
- **`lbzu`**: unrolled `isel_loop` emits `lwz`+`addi r4,r4,32` (no fusion);
  single-step pointer walks fuse.
