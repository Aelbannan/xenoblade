# Scheduling & loop optimization — the `-O4,p` vs `-O4,s` split

How MWCC orders instructions and shapes loops, and how to steer it from
high-level C/C++ or per-TU flags to close a `structural` scheduling residual.

**Source of truth:** the backend pipeline in `mwcc-decomp` (`docs/PASS_PIPELINE.md`,
`src/backend/Scheduler.c`) plus the flag vocabulary recovered from the Wii/1.1
binary. Empirical rules verified by compiling probes at `-O4,p`/`-O4,s`/`-O3`/`-O2`
and diffing the PPC (`.scratch/sched_probe.c`).

## When to read this

- **Read when:** hexdiff shows `structural` residuals with the same instruction set
  (different order / loop shape), or the one-line verdict implicates scheduling flags.
- **Skip it when:** pure `reg_swap` with identical order and instructions →
  `register_mapping.md`.

## Fast path — first moves in order

1. Check the unit's `mw_version` / `extra_cflags` — hexdiff prints the config line on every run.
2. **Loop body differs** (unrolled pointer-walk vs indexed `lwzx`/`stwx`) → flip the unit's `-O4,p` ↔ `-O4,s` in configure.py, re-`hexdiff`, revert if it doesn't help.
3. Nop padding around `mtctr`/`bdnz` or unrolled bodies → loop-align flags / `-func_align 4|16` (separate lever).
4. After a scheduling fix, re-examine **colors** — scheduling changes live ranges, so new `reg_swap`s are expected; re-route to `register_mapping.md`.

## TL;DR

1. **Instruction scheduling is gated at O4**, not at the `p`/`s` suffix: O2/O3
   emit natural order (all loads, then arithmetic); O4 interleaves to hide
   latency. `-O4,p` and `-O4,s` schedule identically.
2. **`-O4,p` vs `-O4,s` is about loops**: `p` unrolls (8×) and strength-reduces
   to a pointer walk; `s` keeps the indexed `lwzx`/`stwx` form and does not
   unroll. Non-loop code is byte-identical between the two.
3. The scheduler runs **twice** — before register coloring, and as "final
   scheduling" before emission — and is **mode-dispatched** on the optimization
   level.

## Where the scheduler sits

`CodeGen_Generator` (`0x004351c0`) runs the level-specific optimizer, then the
shared backend tail in this order (trace markers confirmed in the binary):

```
initial PCode
optional instruction scheduling      <- "BEFORE SCHEDULING"
forward peephole optimization        <- "AFTER PEEPHOLE FORWARD"
register coloring (0x004cdef0)       <- the allocator (see register_mapping.md)
EABI prologue/epilogue (0x004abe90)
prologue/epilogue merge
final peephole optimization          <- "AFTER PEEPHOLE OPTIMIZATION"
optional final instruction scheduling <- "FINAL CODE AFTER INSTRUCTION SCHEDULING"
emission
```

So scheduling happens **before** coloring (reorders the PCode the allocator sees)
and **after** the prologue is laid down. A scheduling change therefore cascades
into register allocation — this is why many "reg-swap" residuals are actually
scheduling-driven.

## The scheduler is mode-dispatched

`Scheduler_Schedule` (`0x004ccae0` in GC/1.2.5) reads the optimization-level byte
(`0x584224`) and selects a scheduling-mode table (stored in `0x581b80`):

| opt byte | scheduling table |
|---|---|
| 1 | `0x578320` |
| 2, 5 | `0x575880` |
| 3, 6 | `0x5763f8` |
| 4 | `0x576f08` |
| 7 | `0x577640` |
| 9 | `0x578e30` |
| default | `0x574d70` |

The table holds function pointers; the mode invokes a per-block scheduling pass
and `Scheduler_004ccf10` (dependency construction from 12-byte PCode operands).
The exact level-byte encoding for each `-O` spelling is recovered below via the
flag table, but the practical takeaway is: **the O-level picks a scheduling
strategy, and the `p`/`s` suffix selects loop-optimization behavior on top of it.**

## The flags (recovered from the Wii/1.1 binary)

Scheduler / peephole:

- `scheduling` — instruction scheduling (positive-only flag; implied at O4).
- `schedule_factor` — scheduler tuning.
- `peephole`, `globaloptimizer` — forward/final peephole passes.

Loop optimization:

- `unrolling`, `unrollfactor`, `fullunrolllimit`, `unrollinstrfactor`,
  `findoptimalunrollfactor`, `unroll_speculative`, `unroll_instr_limit`,
  `unroll_factor_limit`.
- `strengthreduction`, `strengthreductionstrict`.
- `vectorizeloops`, `unswitchloops`, `rotate_loops`, `removeemptyloops`,
  `transformcounterloops`, `markcounterloops`, `addgotounrolledlooptest`.
- `full_loop_align`, `aggressive_loop_align`, `unrolled_loop_align`.

Optimization level / size:

- `optimizationlevel`, `optimizesize`, `limitedoptlevel`, `opt_partial|r1`.
- `opt_tail_call` — tail-call optimization.

Other backend toggles worth knowing: `pool_fp_consts`,
`remove_frsp_aggressive`, `ppc_dont_balance_ir`, `power_of_2_func_align`,
`merge_float_consts`.

## The empirical `-O4,p` vs `-O4,s` contract

Probe `sched_loads` (four independent loads + `mul` + `add` + `sub`):

```
-O2 / -O3 (no scheduling):                 -O4,p / -O4,s (scheduled):
lwz r6, g3                                lwz r0, g1
lwz r5, g2                                lwz r4, g2
lwz r4, g0                                mullw r3, r0, r3     <- mul hoisted
lwz r0, g1                                lwz r0, g0
mullw r3, r0, r3                          lwz r5, g3
add  r0, r5, r4                           add  r0, r4, r0
add  r0, r3, r0                           add  r0, r3, r0
subf r3, r6, r0                           subf r3, r5, r0
```

`-O4,p` and `-O4,s` are **byte-identical** here. Scheduling (interleaving the
`mullw` between loads to hide the multiply latency) is a **level-4** behavior.

Probe `sched_loop` (indexed accumulation loop) — this is where `p`/`s` diverge:

```
-O4,p (unroll + strength-reduce):         -O4,s (no unroll, indexed):
cmpwi cr1, r3, 0                          li r6, 0
li r8, 0 ; li r9, 0                       li r5, 0
... 8x-unrolled body:                     mtctr r3
lwz r5,0(r7); add r8,r8,r5                cmpwi r3,0
lwz r0,4(r7); add r8,r8,r0                ble  exit
...                                       loop:
addi r7,r7,32   <- pointer walk            lwzx r0,r4,r5
bdnz loop                                 addi r5,r5,4
tail: slwi r5,r9,2; mtctr r0; ...         add r6,r6,r0
                                          bdnz loop
```

So `-O4,p` adds: loop unrolling (8×), strength reduction (`addi r7,r7,32` pointer
walk instead of `lwzx r0,r4,r5`), a counted remainder loop, and the trip-count
guard. `-O4,s` keeps the naive indexed loop. This is the exact `stwx`/`lwzx`
vs pointer-walk distinction the reference already documents for
`CVS_THREAD_HP`/`mix.c`.

## Actionable rules for matching

1. **A `structural` scheduling diff (loads/adds in a different order) with
   identical instruction set means the O-level is wrong, not the source.**
   Check the TU's `mw_version` + `extra_cflags`: retail at `-O4,p` vs decomp at
   `-O4,s` (or vice versa) is the single most common cause.

2. **`-O4,p` vs `-O4,s` only matters for loops.** If the residual is a loop body
   (unrolled vs indexed, `addi rN,rN,4` vs `lwzx`), the lever is the `-O4,p`/
   `-O4,s` flag, not declaration order. A non-loop function will not change
   between the two.

3. **Scheduling interacts with the allocator.** A scheduling reorder changes
   live ranges and therefore re-colors registers. Do not chase a "reg-swap" with
   declaration order if the underlying cause is that retail scheduled a load
   earlier — fix the O-level first, then re-examine the colors.

4. **`-schedule` and `-peephole` are positive-only** ("option only has a
   positive form" in the flag table); they are implied at O4 and cannot be
   turned off individually at O4.

5. **Loop alignment** (`full_loop_align`, `aggressive_loop_align`,
   `unrolled_loop_align`) and `-func_align 4/16` control the nop padding around
   `mtctr`/`bdnz` and unrolled bodies — a separate lever from scheduling itself.

## Evidence basis

- **Pipeline order + double scheduling**: `docs/PASS_PIPELINE.md` + trace strings
  `BEFORE SCHEDULING` … `FINAL CODE AFTER INSTRUCTION SCHEDULING` in the binary.
- **Mode dispatch**: `Scheduler_Schedule` disassembly (`0x004ccae0`, levels 1–9 →
  tables `0x574d70`–`0x578e30`).
- **Flag vocabulary**: recovered from the Wii/1.1 flag table (`.data`).
- **`p`/`s` split**: `sched_loads` (scheduling, level-gated) and `sched_loop`
  (unroll + strength-reduction, `p`-gated) compiled at four levels and diffed.
