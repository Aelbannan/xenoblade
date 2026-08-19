# Register mapping — the deterministic allocator contract

How MWCC assigns PowerPC physical registers to source values, and how to steer
it from high-level C/C++ to turn a HIGH/CODE_MATCH (pure `reg_swap`) residual
into a `FULL_MATCH`.

**Source of truth for the mechanism:** the decompiled MWCC register allocator in
`/tmp/mwcc-decomp/src/backend/{Registers.c,Coloring.c}` (GC/1.2.5, the Melee
compiler — same binary as `build/compilers/GC/1.2.5/mwcceppc.exe`). The
*boundaries and directions below were re-verified against Wii/1.1 by compiling
probes with both compilers and diffing the PPC (`.scratch/regmap_*.c`).

## When to read this

- **Read when:** hexdiff shows `reg_swap > 0` with `structural == 0` — identical
  instructions, different register *colors*.
- **Skip it when:** `structural > 0` (different instructions — routing is wrong,
  see below); the diff is `mr` vs `addi rD,rS,0` / `lbzu` (→ `scheduling.md` /
  `instruction_selection.md` first); or the function contains `psq_*` or a `bl`
  call (the witness can't certify those — **`FULL_MATCH` is the only route**, see
  Caveats; do not spend time on colors).

## Fast path — first moves in order

1. Confirm the verdict is **pure reg-swap** (no `structural`, no `mismatch`). If not, you're in the wrong doc.
2. **Swap the declaration order of the two swapped locals** (Rule A: first-declared → highest saved register). One change at a time; re-`hexdiff` after each, revert if the count rises.
3. **Swapped saved locals** that persist → Rule C levers (liveness, inlining, expression shape).
4. **Swapped parameter saves** (params crossing a call) → Rule B: fix *call-argument liveness*, never reorder declarations.
5. Stuck after 3 attempts → record an open-item packet (residual, ruled-out orders, next 3 experiments) and switch angle — including `mw_version`/flags if the diff is actually in "Not a register-mapping problem" below.

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
   `p_tle`/`i` and `event_type`/`p_msg` notes in MWCC_CASES).
3. **Inline vs helper** — inlining changes which values cross a call boundary.
   A helper that MWCC inlined retail-side but you wrote out-of-line (or vice
   versa) flips the saved-register set.
4. **Expression shape** — `->data` idiom, pointer-walk vs index, `q++` store
   runs. These change virtual-register *birth order* even when CSE restores the
   same instruction graph.
5. **Live-value count** — adding/removing one value live across a call shifts
   every saved assignment by one register. Sometimes the fix is to *not* hoist
   a value, or to reuse a dead argument register.

## Copy coalescing — when `fmr`/`mr` disappears

The allocator runs a **copy-coalescing** pass (`SpillCode_CoalesceCopies`) before
coloring. It walks every instruction, finds copy opcodes, and merges the source
and destination webs into one when they do not interfere:

| class | copy opcode | PCode opcode |
|---|---|---|
| GPR | `mr` (`or rD,rS,rS`) | `0x8b` |
| FPR | `fmr` | `0x9e` |
| VR | `vor` | `0x18e` |

Rules (`SpillCode_CanCoalesce`):

1. **no interference** — if the copy's source and destination overlap in any
   live range, they stay distinct;
2. **coalescing window** — both virtual registers must be in
   `[CoalesceFirst, CoalesceLast]` for their class (`Registers_BeginCoalesceWindow`
   opens it at object preallocation, `CloseCoalesceWindow` closes it after
   lowering). Copies outside the window are never merged;
3. the merged root is the **lower virtual-register number**, and the copy
   instruction is deleted (it becomes a no-op).

Consequence: a `float y = x; return x + y;` where `x` is still live compiles to
`fadds f1,f1,f1` (the `fmr` is coalesced — `y` and `x` share `f1`). Only when the
source and destination genuinely interfere does the `fmr` survive with two
distinct FPR colors.

### FPR free-slot fill (scratch)

FPR scratch results fill **f0-first, ascending**, and a result reuses the lowest
free FPR once its previous value dies. `t=a*b; u=t+c; v=u*a; return v+b;` emits:

```
fmuls f0,f1,f2   ; t -> f0
fadds f0,f0,f3   ; u -> f0 (t dead, f0 reused)
fmuls f0,f0,f1   ; v -> f0
fadds f1,f0,f2   ; return -> f1
```

Every intermediate lands in `f0`; only the return moves to `f1`. A residual where
retail and decomp differ in an FPR *destination* (`fmuls f0,…` vs `fmuls f1,…`)
is this free-slot fill, driven by how many FPR values are simultaneously live —
not by declaration order.

### FPR member/struct copy coloring

A **whole-struct** copy (`*self = *src` on a 12-byte `{x,y,z}`) is lowered to GPR
word moves (`lwz`/`stw`), not float moves. A **member-wise** float copy
(`float x=src->x; float y=src->y; float z=src->z; …`) uses `lfs`/`stfs` and colors
the locals by declaration order: `x->f0, y->f1, z->f2` (scratch, low→high). The
*store* order follows the assignment/statement order. See `MWCC_CASES`
§"3-float struct copy" for the exact load-descending/store-ascending retail shape.

## Dead-register reuse vs fresh allocation

`Coloring_SelectColors` does **not** allocate every saved register exactly once.
When it claims a saved register for a node it adds that register's bit back into
the color mask, so a later non-interfering node **reuses** the dead register
instead of claiming a fresh one. Confirmed in the GC/1.2.5 binary at `0x4ce3ce`
(`orl %eax, (%esp)` = `color_mask |= 1 << color`), matching the decompiled
`Coloring.c`.

Flow, per node popped off the simplify stack (reverse simplify order):

1. `available = color_mask & ~neighbor_colors`
2. if any bit set → take the **lowest** set bit (scratch first, `for color=0..31`)
3. else → `ClaimColor` (highest free saved register), then **`color_mask |= 1<<color`**

The **pop order** is set by `Coloring_SimplifyGraph` (confirmed at `0x4ce410`): it
walks virtual registers 32..N **ascending**, pushes every node with
`degree < available_colors` onto the stack, and puts `degree >= available_colors`
into a "spill" list. When the graph is stuck it picks the lowest
`spill_cost / degree` from that list, pushes it, and re-runs simplify.

So the stack is **low-degree nodes first (birth order), then high-degree
(spill) nodes on top**. `SelectColors` pops the top first, so **high-degree webs
are colored before low-degree webs** — and a low-degree web, colored later,
reuses a high-degree web's dead register.

The reuse/fresh decision is therefore **degree-driven**, not
declaration-order-driven:

- **fresh** — the high-degree (long-lived) web claims the register first;
- **reuse** — the low-degree (short-lived) web, colored later, reuses it.

Declaration order only renumbers virtual registers, so it flips the order **only
when both webs are in the same phase** (both low-degree). When one is high-degree
(deferred to spill) and the other low-degree, birth order is ignored and the
reuse is fixed — this is `AXFXChorusExpInit` (`i` reusing `j`'s r28),
`bta_dm_inq_cmpl` (`p_buf` reusing the found-flag's r27), `btm_inq_db_reset`,
`SFUO_Create`, `func_8003E528`, `__wudDeleteHandler`, `func_80164838`. The lever
is to change a web's **degree** — split a long web, shorten its live range across
a call, or add/remove a keeping-use — so it crosses the `available_colors`
boundary, not to reorder declarations.

### The dead-def-range discriminator

The concrete case behind several wall functions is a **dead def**: a value
assigned at the top and overwritten before any read (`bta_dm_inq_cmpl`'s
`found = FALSE` → later `found = …`, zero reads between). Whether that def's
register is reusable is decided before coloring, by `SpillCode_MarkLastUses` /
`SpillCode_IsDeadInstruction`:

- a def with no use before the next def is **removed** (DCE) when
  `gDeleteDeadInstructions > 0`, freeing its register → the later web **reuses** it;
- a def carrying the **dead-code-barrier** flag (`PCodeInstruction_DeadCodeBarrierMask`)
  is **not** removed, so its register stays live across the dead-def range → the
  later web gets a **fresh** register.

Source levers that force the dead def to stay live (block the reuse):

1. **`volatile`** — `volatile BOOL found = FALSE;` makes the store a barrier
   (volatile stores cannot be DCE'd), so `found` holds its register across the
   range;
2. **a keeping-read** — any read of the value between the init and the overwrite
   turns the init into a real def (it now has a use).

So for a reuse-vs-fresh wall where retail keeps the register live and decomp
reuses it, try `volatile` on the flag (or insert a read) before reaching for
degree reshapes. Note: verified against the GC/1.2.5 source; test on Wii/1.1
before trusting the exact volatile DCE boundary.

### Multi-def web continuity (still open)

Several named walls are *not* single dead-defs — they are a value defined on two
CFG paths (`bta_dm_inq_cmpl`'s `found=1` else / `found=0` break). Retail gives it
one continuous web (first def → last use) so it interferes with the web in
between and the later web claims a fresh register; decomp path-splits it, freeing
the register early for reuse.

The interference is decided by `SpillCode_ConstructInterference` walking each
block **backward**: a definition clears its register from the live set, then
adds an edge to every value still live *after* it — so `found` interferes with
`p_buf` **iff `found` is live at `p_buf`'s def** in that walk. That live set is
the **post-scheduling PCode**, not the source statement order: the scheduler runs
before coloring, so a source reorder only *indirectly* reaches the interference.

Recorded negative results (2026-08): (1) a `goto`-label on the def's block does
**not** set `block->flags_2e & 0x03`, and neither found-def block is a merge point
in the current shape — block-barrier theory out; (2) the def/use-ordering
conditions are **already met** by the baseline (`p_buf` def at +0x16c precedes
`found=0` at +0x198, and `found=0` is already a genuine merge target), yet the
reuse persists, and neither a keeping-read nor a statement reorder flips it.

Conclusion: the discriminator lives **below the source**. The pipeline runs
instruction scheduling **before** register coloring (see `scheduling.md`), so the
interference graph is built from the **post-scheduling PCode**, not the source
statement order. A source detail can change the PCode (and hence the interference)
while the scheduler/peephole re-hides it into an identical asm stream — so the
reuse-vs-fresh split on these functions is a **scheduler-driven register
allocation** soft-cap, not directly source-steerable. Record it as such; do not
keep hunting statement-order/block-shape levers.

## Stack-slot placement

When a value cannot live in a register (address taken, spilled, too large, or a
`&x`-forced local), MWCC places it in the frame via
`StackFrameEABI_AllocateObjectSlot` (`0x004ac4a0`), an ascending cursor allocator:

```c
alignment = StackFrameEABI_GetTypeAlignment(object->type);
cursor   = (cursor + alignment - 1) & ~(alignment - 1);  // round up
object->stack_offset = cursor;
cursor  += object->type->size;
```

Rules:

1. **Alignment = the type's alignment** — `char` 1, `short` 2, `int`/`float` 4,
   `long long`/`double` 8, vectors/`aligned` 16/32. Force a slot/frame shape with
   `__attribute__((aligned(N)))` (reference: `titleId`@sp+0x40 / `state`@sp+0x60
   with `aligned(32)` reproduced the retail frame byte-for-byte).
2. **Allocation order = insertion order into the addressed-objects list.**
   `AllocateObjectSlot` runs in a loop over the compiler's addressed-local list
   (`0x57f6c0` in GC/1.2.5), right before `FinalizeLayout`; a second loop handles
   register objects whose `RegisterInfo` says they need a home. Objects enter the
   list in **codegen order** (incoming params → locals → temporaries → spills),
   so it is **not** first-use and **not** spill-cost order. The practical lever
   is declaration order of the **locals**, changed one variable at a time
   (`char buf2[64]; char buf1[64];` with buf2 FIRST flips `branch1→sp+8` /
   `branch2→sp+0x48`).
3. **Direction — first-declared lands at the higher frame offset** for same-size
   locals (`int x,y,z;` → `x@sp+0x10`, `y@sp+0x0c`, `z@sp+0x08`), mirroring the
   saved-register rule (first → highest). Mixed sizes interleave alignment
   padding, so the offset sequence is declaration-order only after each object's
   alignment is applied.
4. **Slot reuse** — a single reused local (`VEC3 tmp` across several calls) keeps
   **one** slot; declaring separate locals for each use balloons the frame
   (+0x20 or more) and shifts every later slot.
5. **Frame size** is the sum of the bands, aligned: linkage area (back chain + LR
   save, 8 bytes) → vector-save → GPR-save (`r14–r31`) → FPR-save (`f14–f31`) →
   local-object area → outgoing-argument area → padding
   (`StackFrameEABI_FinalizeLayout`, `0x004ac240`). A wrong `stwu r1,-0xN` means
   the **spill count or callee-save set** differs — reduce live ranges or split
   helpers, do not pad the frame by hand.

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

For the scheduler itself, the `-O4,p` vs `-O4,s` loop-optimization split, and
the full flag vocabulary, see [`docs/scheduling.md`](scheduling.md). For the
`mr` vs `addi rD,rS,0` copy lowering and `lbzu`/peephole rules, see
[`docs/instruction_selection.md`](instruction_selection.md).

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
- **Cross-version boundary:** `mwcc-decomp` is GC/1.2.5 only. The allocator
  *mechanism* (`FindFree`/`ColorMask`/`SelectColors`/`SimplifyGraph`, incl. the
  `color_mask |= 1<<color` reuse and the degree-threshold simplify) is **verified
  in the GC/1.2.5 binary**, not in Wii/1.1. The Wii/1.1 allocator functions could
  not be located by byte-pattern (different host compiler), so "the mechanism is
  the same in Wii/1.1" is **inferred**, not read: it rests on the shared data
  model (field offsets `+0xe`/`+0x2a`/`+0x2` present), the shared register-class
  ceilings (`0xc`/`0xd`/`0x13`), the shared `shl cl` bit idioms, and the matching
  empirical behavior (probes + the dead-register-reuse wall functions). Treat the
  direction rules as Wii/1.1-verified; treat the mechanism as GC/1.2.5-verified,
  Wii/1.1-inferred.
