# Register mapping — the deterministic allocator contract

How MWCC assigns PowerPC physical registers to source values, and how to steer
it from high-level C/C++ to turn a HIGH/CODE_MATCH (pure `reg_swap`) residual
into a `FULL_MATCH`.

**Source of truth for the mechanism:** the decompiled MWCC register allocator in
`/tmp/mwcc-decomp/src/backend/{Registers.c,Coloring.c}` (GC/1.2.5, the Melee
compiler — same binary as `build/compilers/GC/1.2.5/mwcceppc.exe`). The
*boundaries and directions below were re-verified against Wii/1.1 by compiling
probes with both compilers and diffing the PPC (`.scratch/regmap_*.c`).

## TL;DR

1. MWCC splits the register file into **volatile** (scratch) and **non-volatile**
   (saved) pools and colors them in **opposite directions**.
2. **Saved registers are claimed HIGH → LOW (r31/f31 first, descending).**
   A fresh local that must survive a call gets `r31` (or `f31`) if it is
   declared *first*; each later-declared local takes the next lower register.
   **Reversing declaration order reverses the assignment.**
3. **Scratch registers are claimed LOW → HIGH (r0/r3 first, ascending).**
   Short-lived temporaries that die before the next call land here.
4. A `reg_swap` residual is *not* noise — it is the same interference graph
   colored in a different order because a source-shape detail (declaration
   order, statement order, inlining, expression shape) changed the virtual-
   register birth order.

## The register file (Gekko / PPC EABI, as MWCC models it)

| class | registers | direction claimed | role |
|---|---|---|---|
| GPR volatile | r0, r3–r12 | low → high | args + short-lived temps; clobbered by calls |
| GPR fixed | r1, r2, r13 | never allocated | SP, sda2 base, sda base |
| GPR saved | r14–r31 | **high → low** | values live across calls |
| FPR volatile | f0–f13 | low → high | FP args + short-lived temps |
| FPR saved | f14–f31 | **high → low** | FP values live across calls |
| VR volatile | v0–v19 | low → high | Altivec short-lived |
| VR saved | v20–v31 | high → low | Altivec live across calls |

The boundary constants are hard-coded and verified in both binaries:

- scratch mask ceiling: GPR `0xc` (r12), FPR `0xd` (f13), VR `0x13` (v19)
- saved claim floor: GPR `0xe` (r14), FPR `0xe` (f14), VR `0x14` (v20)
- saved claim scan starts at `0x1f` (r31/f31/v31)

## The mechanism (from `Registers.c` / `Coloring.c`)

The allocator is a Chaitin-style graph-coloring register allocator with a
**two-tier color space**, not a single contiguous pool:

1. **Virtual-register birth.** Objects are assigned virtual registers in creation
   order (`gUsedVirtualRegistersGPR++` in `Registers_AllocateGPR`, when virtual
   numbering is active). For locals, creation order = declaration order (mostly).

2. **Interference + simplify.** `Coloring_SimplifyGraph`/`Coloring_SimplifyLowDegree`
   push degree-`<`-colors nodes onto a stack (Chaitin simplify), spilling the
   lowest `spill_cost/degree` node when stuck.

3. **Select.** `Coloring_SelectColors` pops the stack and computes
   `available = color_mask & ~(neighbors)`. Two fallbacks:
   - if a bit is free in `available`, pick the **lowest-numbered** free bit
     (`for (color = 0; color < 32; color++)` — bit scan from 0);
   - else `Coloring_ClaimColor` → `Registers_FindFree`, which scans **from 31
     down to the floor** and returns the first free register.

`Registers_FindFree` is the saved-register claim:

```c
for (reg = 31; reg >= first; reg--)   /* first = 14 for GPR/FPR, 20 for VR */
    if (used[reg] == 0) { bind(0, reg); return reg; }
```

`Registers_BuildColorMask` is the volatile-register mask:

```c
for (reg = 0; reg <= last; reg++)      /* last = 12 GPR / 13 FPR / 19 VR */
    if (used[reg] == 0) mask |= 1U << reg;
```

So the two directions are literally two different loops in the same file. A
value gets a **volatile** register if its live range fits between calls (and a
low scratch register is free), or a **saved** register if it crosses a call (or
scratch is exhausted), claimed from `r31` downward.

## The actionable rules

### Rule A — declaration order drives saved-register color (the main lever)

For **fresh locals that are live across a call**, MWCC assigns saved registers
in declaration order, **first-declared → highest register**.

Empirically confirmed on Wii/1.1 (`regmap_direction.c`, `.scratch/regdir.o`):

```c
int f(int in) {                        /* declared: p, q, r, s */
    int p = g(0) + in;
    int q = g(1) + in;
    int r = g(2) + in;
    int s = g(3) + in;
    h(in);                             /* keeps p,q,r,s in saved regs */
    return p + q + r + s;
}
```
```
add r31,r25,r3   ; p -> r31   (first declared -> highest)
add r30,r25,r3   ; q -> r30
add r29,r25,r3   ; r -> r29
add r28,r25,r3   ; s -> r28   (last declared  -> lowest)
```

Reversing the declarations (`s,r,q,p`) flips it byte-for-byte: `s->r31 …
p->r28`. Six locals map `v0->r31, v1->r30, …, v5->r26`. This is the exact
mechanism behind the reference notes "first declared → higher callee-saved reg"
and "MWCC colors locals from r31 downward."

**Application:** when retail holds value X in r31 and value Y in r30 but your
decomp has them swapped, declare X before Y.

### Rule B — parameters preserve incoming ABI order (opposite of locals)

Function **parameters** are already in r3–r10. When they must be saved across a
call, MWCC re-maps them preserving relative order, so **first param → lowest
saved register** (ascending) — the *opposite* of the locals rule:

```c
int f(int a, int b, int c, int d) {    /* params a,b,c,d in r3,r4,r5,r6 */
    h(0);
    return (a+1)+(b+2)+(c+3)+(d+4);
}
```
```
mr r31,r6   ; d (4th, r6) -> r31
mr r30,r5   ; c        -> r30
mr r29,r4   ; b        -> r29
mr r28,r3   ; a (1st, r3) -> r28   (first param -> lowest)
```

Do not apply the locals rule to parameters. A param-live-across-call mismatch is
fixed by matching the *call-argument liveness* (which params survive), not by
reordering declarations.

### Rule C — scratch temporaries go low → high

Short-lived values (dead before the next call, or a sequence of independent
temporaries) land in volatile registers in ascending order (`r0`/`r3` first, then
`r4`, `r5`, …). The `Coloring_SelectColors` bit scan from 0 produces this. A
"ret-default in r0 vs r4" or "r5↔r8 rotation" residual is a scratch-pool effect,
driven by how many temporaries are simultaneously live, not by declaration order
of saved locals.

## Why a `reg_swap` happens

The interference graph is identical, but the **coloring order** differs. The
order depends on:

1. **virtual-register birth order** — declaration order, statement order,
   inlining boundaries, expression shape (`(p+1)->data` vs `->data` idiom);
2. **which values are live across a call** — a value that the retail compiler
   kept in a saved register may be born/die differently in your shape, pushing
   it into scratch or shifting every other saved assignment;
3. **coalescing** — copy coalescing merges webs and changes node count/degree.

Every one of these is a *source-shape* lever, not a register-level trick. You
never write `register rN` or asm; you change the high-level C/C++ shape until the
virtual-register birth order matches retail.

## Levers, ranked

1. **Declaration order of locals** (Rule A) — the cheapest, most reliable fix for
   a pure callee-saved swap. Change one declaration order, re-hexdiff.
2. **Statement / assignment order** — where a value is assigned changes its
   live range; retail often assigns in the loop condition vs body (see
   `p_tle`/`i` and `event_type`/`p_msg` notes in MWCC_REFERENCE).
3. **Inline vs helper** — inlining changes which values cross a call boundary.
   A helper that MWCC inlined retail-side but you wrote out-of-line (or vice
   versa) flips the saved-register set.
4. **Expression shape** — `->data` idiom, pointer-walk vs index, `q++` store
   runs. These change virtual-register *birth order* even when CSE restores the
   same instruction graph.
5. **Live-value count** — adding/removing one value live across a call shifts
   every saved assignment by one register. Sometimes the fix is to *not* hoist
   a value, or to reuse a dead argument register.

## Not a register-mapping problem (different levers)

These produce *different instructions*, not just different colors, and the
register-renaming witness will not certify them. They are fixed with
`mw_version`/flags, not declaration order:

| symptom | cause | fix |
|---|---|---|
| `mr rX,rY` vs `addi rX,rY,0` | instruction selection (`makecopyinstruction`/`copy_opcode_for_type`) | compiler version — Wii/1.1 emits `mr`, GC/1.2.5 emits `addi` |
| `_savegpr_N`/`_restgpr_N` vs inline `stmw`/`lmw` | `use_lmw_stmw` flag | `-use_lmw_stmw on` → inline `stmw`; default → `_savegpr_*` helpers. Per-library in `configure.py` (game/MSL/runtime/TRK/CriWare set it; SDK/HBM/nw4r do not) |
| `stfd` + `psq_st`/`psq_l` pairs vs plain `stfd` | Gekko paired-single FPR save (`gecko_load_store_register`, `gprfloatcopy`, `gecko_float_typecons`) | Wii/1.1 PS backend; witness-ineligible (§17.6) |
| `slwi r3,r0,1` vs literal adds | algebraic simplification (`propagatecopy/addinstructions`) | compiler version / optimizer (`-O4,p` vs `-O4,s`) |
| FPR add hoisted above the `bl` | scheduling | compiler version / `-O4,s` |
| `li`/`lis` ordering, `lbzu` merge | scheduling / immediate selection | `mw_version`, `-func_align 4/16`, `-ipa off` |

The prologue/epilogue and PS decisions are **flag-controlled**, not source-steerable.
Recovered flag/helper vocabulary from the Wii/1.1 binary: `use_lmw_stmw`,
`no_register_save_helpers` (internal, not a `-flag`), `prologue_helper_functions`,
`epilogue_helper_functions`, `save_fprs`/`save_altivec`/`save_spe`, `gprfloatcopy`,
`gecko_float_typecons`, `merge_float_consts`, `always_fold_floats`; helper templates
`_savegpr_%d` / `_save32gpr_%d` / `_save64gpr_%d` / `__create_frame_and_save_gpr_%d`
/ `_restgpr_%d` / `_rest32gpr_%d` / `_rest64gpr_%d` / `__restore_gpr_%d_exit` /
`_savefpr_%d` / `_savevr%d`.

## Caveats

- **GPR pairs** (64-bit `long long` / paired values) allocate two adjacent
  registers via `Registers_AllocateGPRPair`; they claim two saved registers at
  once and can shift everything below them.
- **The Chaitin spill path** (more than ~32 virtual registers) re-orders the
  stack on `spill_cost/degree`; the simple "declaration order → register" rule
  still applies but the mapping can be non-monotonic under spilling.
- **FPR and GPR are independent pools** — a float local and an int local do not
  compete; fix them separately.
- **The witness only certifies pure reg-swaps** with no `psq_*`, no `bl`, no loop
  path-limit blowup, and no call-argument-register conflicts (see
  `docs/ppc_equiv_work/31-reg-swap-witness.md`). If retail reuses a register for
  a call argument, no global ρ exists and you must reach `FULL_MATCH`.

## Verification recipe

```
python3 tools/coop/hexdiff.py <unit> --symbol <mangled> --brief
```
Watch the `reg_swap` and `structural` counts:

- `structural` > 0 → instruction shape differs; fix the source shape first.
- `reg_swap` > 0, `structural` == 0 → pure color mismatch; apply Rule A/B/C,
  re-hexdiff after *each* single declaration-order change (not several at once).
- Reverting an edit that raises the count is a signal, not a failure — record the
  ruled-out order and try the next one.

## Evidence basis

- **Mechanism (confirmed, GC/1.2.5 source):** `Registers_FindFree` (31→floor),
  `Registers_BuildColorMask` (0→ceiling), `Coloring_SelectColors` (lowest bit
  then high-to-low claim). Boundary constants verified in the binary via objdump
  (`0x4c1a20`–`0x4c1ac0`).
- **Direction rules (confirmed, Wii/1.1 probes):** `locals_orderA/B`,
  `locals_six`, `params_four`, `regmap_fpr`, `regmap_mixed` — saved locals
  first→r31/f31 (descending), params first→lowest (ascending), scratch low-first.
- **Cross-version boundary:** `mwcc-decomp` is GC/1.2.5 only; the register
  *mapping* is identical to Wii/1.1, but instruction selection / prologue /
  scheduling differ (see the "not a register-mapping problem" table). Do not
  transplant GC/1.2.5 byte-level expectations to Wii/1.1.
