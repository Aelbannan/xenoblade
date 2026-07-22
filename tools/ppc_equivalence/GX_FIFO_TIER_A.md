# GX FIFO Tier-A capability contracts

Architecture-model consumers: `broadway-ppc32-be-v43` and later.
Hardware profile: `wii-broadway-xenoblade-us-v2` (immutable; do not mutate v1).

This document freezes the Tier-A domain for:

- `gx-fifo-write-trace` → model `gx-fifo-trace-v2`
- `gx-fifo-read` → model `gx-fifo-read-v1` (outcome 3 / unsupported)
- `mmio-loop-emission` → model `mmio-loop-emission-v1`

Unsupported shapes remain `INCONCLUSIVE_UNSUPPORTED` and do not weaken Tier-A claims.

---

## Shared foundations

All three capabilities require:

1. Reviewed hardware profile `wii-broadway-xenoblade-us-v2` (hash-bound).
2. Independent Dolphin corpus evidence (DOL harness; not Python device-model outputs).
3. Recomputable UNSAT digests (query SHA-256, solver metadata, no placeholders).
4. Ad-hoc caller-built bus maps grade incomplete forever.
5. `automatic_promotion=false`; allowlist one model version at a time; bottom-up recertify.

GX write-gather pipe (SDK `WGPIPE`): base `0xCC008000`.

---

## `gx-fifo-write-trace` / `gx-fifo-trace-v2`

### Domain

Ordinary (non-summarized) FIFO write event semantics:

- Device theory `gxfifo-stream` at the profile-bound GX region.
- Widths `{1, 2, 4}`; big-endian byte lanes; alignment required for width.
- Each write appends one ordered event with address, width, value, and
  pre/post visible-state digests.
- Event count ≤ profile `max_fifo_events` (default 256).
- Symbolic and concrete traces compare extensionally on device id, order,
  kind=write, address, width, value, and event cursor.

### Out of domain

- FIFO reads (see read contract).
- Summarized loop emission (requires `mmio-loop-emission-v1`).
- Symbolic addresses, DMA, interrupts, other MMIO devices in the same access.
- Unbounded / symbolic trip emission without a discharged loop plan.

### Fail-closed

Misalignment, out-of-range, unsupported width, bound overflow, and ad-hoc maps
are inconclusive or incomplete — never silent RAM fallback.

---

## `gx-fifo-read` / `gx-fifo-read-v1` (outcome 3)

### Authoritative stance

In-tree SDK evidence (`GXHardware.h` `WGPIPE`) documents a **FIFO write/gather
pipe**. There is no authoritative measurement of CPU load results from
`0xCC008000`. Therefore Tier A adopts **outcome 3**:

> Architecturally reserved / undefined for equivalence purposes.

We do **not** assign:

1. A deterministic return value or fault code, or
2. An external/nondeterministic `ReadOracle` value for WGPIPE loads.

An unconstrained value chosen independently per side would produce false
negatives; blindly sharing one unconstrained value would be unsound.

### Tier-A meaning under outcome 3

`gx-fifo-read-v1` is a **policy capability**, not a value model:

- Concrete and symbolic loads targeting the GX FIFO region fail closed
  (`AccessOutcome.UNSUPPORTED` / `gxfifo-read-unsupported`).
- Targets that perform a reachable GX read demand this capability and cannot
  reach Tier A via a value model.
- Targets may still reach Tier A **without** demanding `gx-fifo-read` when
  every path that would load from GX is proved UNSAT
  (`path ∧ GX-read` → UNSAT) under ordinary unsupported-access discharge.
- Allowlisting `gx-fifo-read-v1` means the unsupported-read policy, corpus
  negatives, and recomputation gates are complete — **not** that loads return
  modeled data.

### Dependencies

Because reads are unsupported (no external input, no read side effects),
`gx-fifo-read-v1` does **not** demand Tier-A `mmio-external-input` or
`mmio-read-side-effects`. Those remain incomplete for other MMIO classes.

---

## `mmio-loop-emission` / `mmio-loop-emission-v1`

### Initial domain (deliberately narrow)

| Constraint | Requirement |
|---|---|
| Shape | Natural single-header CTR loop (`mtctr` + body + `bdnz`) |
| Emission | One FIFO write, or a statically fixed short write sequence, per iteration |
| Device | Concrete / proven GX device and address from the reviewed profile |
| Width | `{1, 2, 4}` |
| Trip count | Positive concrete count, or explicitly guarded zero-skip |
| Bound | `N × writes_per_iteration ≤ max_fifo_events` |
| Values | Invariant or simple affine register values only |
| Prohibited | Calls, indirect branches, early exits, FIFO reads, other MMIO, interrupts, DMA |

### Representation

Ordinary non-loop writes remain explicit events. Recognized loops append a
`RepeatedEmission` segment:

```text
RepeatedEmission:
    count
    writes_per_iteration
    address(k) / width / value(k)   # per write in the iteration group
```

Trace comparison proves extensional equality with a symbolic mismatch index
`0 ≤ k < count ∧ event_original(k) ≠ event_candidate(k)` — UNSAT means every
summarized event agrees without quantified formulas.

### Authorization

Recognition never authorizes equivalence. Each used plan on each side must
discharge real UNSAT blocks:

1. Entry guard
2. Body step
3. Event identity
4. Postcondition
5. Termination
6. Bound
7. Footprint / no-extra-effects

Policy:

- UNSAT → block discharged
- SAT during per-side refinement → `INTERNAL_ERROR`
- Unknown/timeout → inconclusive
- SAT in final original-vs-candidate trace compare → `NOT_EQUIVALENT`

The old digest-only `gx-fifo-loop-refinement-v1` shape is never promotion-grade.

### Later model versions (explicitly out of v1)

- RAM-to-FIFO streaming
- Symbolic trip counts
- Multi-header / irregular control flow
- FIFO reads inside the loop

---

## Capability split

| Capability | Authorizes |
|---|---|
| `gx-fifo-write-trace` | Ordinary FIFO write event semantics |
| `gx-fifo-read` | Unsupported-read policy / path∧GX-read UNSAT |
| `mmio-loop-emission` | Replacing a recognized CTR loop with summarized emission |

A loop target normally demands both `gx-fifo-write-trace` and
`mmio-loop-emission`.

---

## Rollout order

1. Land profile, corpus, models, obligations, tests with allowlists empty.
2. Run differential and unit gates.
3. Authoritative canary for `gx-fifo-trace-v2`; allowlist; recertify bottom-up.
4. Allowlist `gx-fifo-read-v1` (unsupported policy); recertify.
5. Allowlist `mmio-loop-emission-v1`; recertify.
6. Audit positive and negative canaries before any default-policy change.

Keep `automatic_promotion=false`.
