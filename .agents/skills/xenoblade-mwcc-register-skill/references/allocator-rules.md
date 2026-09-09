# Wii 1.1 allocator rules

All addresses are in Wii 1.1 `mwcceppc.exe`
(`14f99570…fbe5`). Tags: **confirmed** = proven by this binary;
**inferred** = strong conclusion from its control flow/xrefs;
**reference** = GC-era hypothesis, not yet validated here.

## 1. Classes, pools, and capacities

Five classes, processed by the outer driver (`0x005B8E20`) in order
0→4 (**inferred**). Names come from this binary's own `.rdata`
(**confirmed**). Pre-marked permanently-used regs (**confirmed**):
GPR r1, r2, r13 (SP/RTOC/SDA); CRFIELD cr5.

| Class | Name | Bound | Volatile pool `0x00772280` | Claim pool `0x00772528` |
|---|---|---|---|---|
| 0 | SPR | 6 | {0,1,2,4,5} | descending from 3 |
| 1 | CRFIELD | 8 | {0,1,6,7} | descending from 5 |
| 2 | VR | 32 | {v0..v19} | descending from 31 |
| 3 | FPR | 32 | {f0..f13} | descending from 31 |
| 4 | GPR | 32 | {r0,r3..r12} (11 regs) | descending 31→14 |

Pool contents **confirmed** (table fill `0x005959B0`: cuts 3/2/20/14/14,
uppers 3/5/31/31/31, predicate `[0,cut) ∪ (upper,bound)` minus used).
Pool roles resolved per the three-fact pin in the sibling agent guide:
`0x00772280` = volatile, `0x00772528` = claim (saved); every claim
extends the save span.

Normal color preference is the volatile pool ascending; fallback is the
claim pool descending (**inferred**). Baseline capacities: GPR 29
(32 minus r1/r2/r13), FPR 32, VR 32.

Call-crossing pressure (**inferred** from pools + EABI saved sets):
a value live across a call cannot use the volatile colors, leaving
18 nonvolatile GPRs (r14–r31) and 18 FPRs (f14–f31) — so 19
mutually-interfering call-crossing GPR or FPR values reach spill
pressure. VR leaves 12 (v20–v31); a 13th mutually-interfering
call-crossing VR spills.

## 2. Virtual values and coalescing

Per-class virtual numbering starts at the class bound (32 for
VR/FPR/GPR); the per-class driver resets `nextvirt = bound` on retry
(**inferred**). IDs reflect post-optimization creation order, not source
declaration order: optimizer passes, scheduler order, expression shape,
inlining, volatile/address-taken objects, and pragmas all change which
values reach allocation.

Copies are coalesced through the union-find parent map at `0x00739A20`
(identity-initialized in the `0x0063DD60` cold tail; root resolution at
`0x0063E160` with `InterferenceGraph.c:415` bound checks —
**inferred**). An alias follows its root's color and consumes no
separate color.

## 3. Liveness and interference

Graph build (`0x0063DDF2` retry loop → `0x0063F010` per-class
coordinator) dispatches each instruction by flags: `0x10` →
`0x0063F520`, `0x08` → `0x0063F490` (filter, then default),
else → `0x0063F720` default construct pass (**inferred**).
Constraint-marked operands (flag `0x8000`) seed node bits
`0x1000`/`0x2000` (**inferred**).

Overlapping values interfere; non-overlapping values may reuse a color
(**reference** transfer `live_in = use ∪ (live_out − def)`; the Wii
per-instruction query `0x0063FD20` and 5-row bitsets are **inferred**
mechanics). Calls and raw inline-asm registers add fixed physical
neighbors (**inferred**). ABI inputs/outputs are preassigned constraints
(**reference**). CR fields, CTR, LR, XER are instruction constraints,
not graph colors (**reference**). New GPR virtuals gain triple edges
`(v,0)/(v,11)/(v,12)` via `0x00593140` (**inferred**).

Edge storage (**confirmed** mechanics via `0x0063EDC0`/`0x0063EE50`):
0x4000-bucket hash table at `0x00739A34`, triangular index
`((hi−1)·hi/2 + lo) & 0x3FFF`, canonical smaller-first pair at
`+0x0C`/`+0x0E`, chain link at `+0x00`, adjacency lists at node `+0x1C`,
degree counters at `+0x16`/`+0x18` over the 32-byte node array at
`0x0077647C`. Degree bumps double when node flag `0x200` (paired) is
set (**inferred**).

## 4. Simplify and spill ranking (`0x005B92F0`, inferred)

Low-degree sweep against the class's available-color count; removed
nodes decrement neighbor degrees (by 2 if paired) and go onto the
removal stack. When no low-degree node remains, remove the minimum
`cost / degree` candidate compared with x87 floating point; nodes with
protected flags `0x80` / `0x400` take fixed scores instead of the
ranking.

Spill-cost inputs (**confirmed** pieces): block weights come from block
`+0x24` unless option byte `0x00772A31` selects uniform weight 1
(`0x0063F9E0`, no calls); the default construct pass accumulates
`2 × weight` per definition-side edge and `1 × weight` per second-edge
form into the counters at `0x007691A8+` (branch structure
byte-confirmed in `0x0063F720`). The exact per-node cost formula, the
sweep's scan order, ranking tie-breaks, and the fixed-score values are
**open** — GC-era guesses (`flow_weight·(2·uses+defs)`, highest-vreg
tie-break, ascending scan with head-push reversal) are **reference**
only; do not cite them as Wii facts.

There is no select-time spill heuristic: `0x00593130` is a single-`ret`
stub (**confirmed**, 1 byte `C3`).

## 5. Select and color choice (`0x005B9120`, assert `Coloring.c:396`)

Select restores the per-class snapshot, builds the free-color masks
(combined `0x005B64A0`, volatile-only `0x005B6520`), then pops the
removal stack clearing already-colored neighbor colors (paired nodes
clear 2 bits) and picks the lowest legal color per node. GPR claims go
through the claim path with same-node retry; an uncolorable node sets
the spill flag and select returns 0 (**inferred**; assert file/line
**confirmed**).

Color choice (`0x005930A0`, mechanics **confirmed**, roles
**inferred**): lowest-free-bit scan of a 32-bit mask over
`[0, bound)`; `-1` means spill. GPR-only exception (gated by option
byte `0x00772B9` and node word `+0x14` bit `0x4000`): restrict the mask
to `0xFF0000FF` (regs 0–7,16–31) or `0x00FFFF00` (regs 8–23); the
flag-set path can veto to `-1` via `(~a18) & a1C` tested against
`0xFF0000FF`, the flag-clear path falls back to a full-mask scan.

Retry discipline (**inferred**, assert **confirmed**): fallback
reservation is global and immediate for the pass; if fallback is
exhausted the node is marked spilled, spill code is inserted
(`0x0063F010`), `nextvirt` resets to the bound, and the class retries —
at most 10 attempts per class (`Coloring.c:550` assert at `0x226`).
Allocation after a spill is not the original assignment plus
loads/stores: rewriting renumbers values.

## 6. Commit (`0x005B8FC0`, inferred)

PCode operands are rewritten from node colors (a paired second half
takes color+1); redundant instructions are dropped via `0x005A0160`;
primary colors publish to `RegisterInfo +0x10` (flags `|= 2`),
secondary to `+0x12` (flags `|= 6`); spilled nodes (flag 1) are
skipped. `RegisterInfo` layout `+0x0E` flags / `+0x0F` class /
`+0x10` primary / `+0x12` secondary is independently **confirmed**.

## 7. After coloring (usually not coloring)

- Scheduler: `-proc` offers only z7/gekko/generic spellings and no
  driver case matches, so everything schedules on the default width-2
  model `0x007033E6` (**inferred**). A changed final instruction order
  is scheduling until proven otherwise.
- Peephole: central dispatcher `0x005BF490` with a 120-entry opcode
  table; match-then-rewrite protocol (`0` = no match, `1` = rewrote);
  runs silent with no stage dumps (**inferred**). It selects among
  displacement / update / indexed / multiple load-store forms
  (`LWZ`/`LWZU`/`LFDX`/`LMW`/`STMW`…) — an `LWZ` vs `LWZU` difference is
  peephole selection, not allocation.
- Save/restore: emitter names `_savegpr`/`_restgpr`, `_savefpr`,
  `_savevr`, `__create_frame`, `__restore_frame_and_exit`
  (**confirmed** strings); emission order create → saves → body →
  restores → exit (**inferred** from layout). Frame-slot dword at object
  `+0x48` (**confirmed**); `+0x2A` is refuted. The `stmw`/`lmw`
  threshold is **open** — do not import the GC "five contiguous" rule.
