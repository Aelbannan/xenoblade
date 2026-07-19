# PPC equivalence check

This check proves or refutes equivalence of supported Wii Broadway PowerPC
blocks even when their instruction bytes differ. Both blocks execute
from the same symbolic input state; Z3 searches for a difference in the state
selected by the contract.

See [SOUNDNESS.md](SOUNDNESS.md) for the formal theorem statement, normative
definitions, and traceability table mapping claims to implementation and tests.

It complements, but does not replace:

- objdiff / byte-level matching, which remains one of two equal-tier acceptance bars for decomp targets;
- optional real-PPC behaviour harnesses, when a linked test is registered;
- the Dolphin PPC harness, which supplies independent concrete evidence.

An `equivalent` result is scoped to the listed observables and assumptions. An
unsupported instruction, timeout, or solver `unknown` is always
`inconclusive`; the check never treats unsupported behavior as a no-op.

<!-- BEGIN GENERATED PPC_EQUIVALENCE_VERSION -->

- Architecture model: `broadway-ppc32-be-v26`
- Result format: `12`
- Certificate format: `5`

<!-- END GENERATED PPC_EQUIVALENCE_VERSION -->
<!-- BEGIN GENERATED PROOF_STATUS_TABLE -->

| Status | Value |
|---|---|
| `EQUIVALENT` | `equivalent` |
| `NOT_EQUIVALENT` | `not_equivalent` |
| `INCONCLUSIVE_TIMEOUT` | `inconclusive_timeout` |
| `INCONCLUSIVE_UNKNOWN` | `inconclusive_unknown` |
| `INCONCLUSIVE_UNSUPPORTED` | `inconclusive_unsupported` |
| `INCONCLUSIVE_ABSTRACTION` | `inconclusive_abstraction` |
| `INCONCLUSIVE_LAYOUT` | `inconclusive_layout` |
| `INCONCLUSIVE_UNVALIDATED_CALLEE` | `inconclusive_unvalidated_callee` |
| `INCONCLUSIVE_UNMODELED_EXCEPTION` | `inconclusive_unmodeled_exception` |
| `INVALID_INPUT` | `invalid_input` |
| `INTERNAL_ERROR` | `internal_error` |

<!-- END GENERATED PROOF_STATUS_TABLE -->

## Install

From the repository root:

```bash
python3 -m pip install -r tools/ppc_equivalence/requirements.txt
```

For a repository-local environment that is automatically used by both the
standalone checker and `coop run equivalence`, run:

```bash
python3 tools/ppc_equivalence/run.py --install-deps
```

This creates the ignored `tools/ppc_equivalence/.venv/` with the exact pinned
Capstone and Z3 versions. No shell activation is required.

The pinned runtime dependencies are Capstone (optional decoder cross-check),
Z3, and PyYAML for the Broadway ISA census.
The semantic decoder validates instruction fields directly; Capstone, when
installed, only cross-checks instruction boundaries. Semantics do not depend on
formatted disassembly text.

## Quick checks

Prove that `rlwinm r3,r3,2,0,29` is equivalent to `mulli r3,r3,4` when only
`r3` is observable:

```bash
python3 tools/coop/run.py equivalence check-hex \
  --original 5463103a \
  --candidate 1c630004
```

The co-op runner supplies `--contract ppc-eabi` for raw block checks when
neither `--contract` nor `--observe` is present. Object/function checks default
to the effect-aware `auto` contract described below.

Find and save a counterexample:

```bash
python3 tools/coop/run.py equivalence check-hex \
  --original 38630004 \
  --candidate 38630005 \
  --observe r3 \
  --result build/ppc-equivalence/result.json \
  --replay-out build/ppc-equivalence/counterexample.json \
  --smt-out build/ppc-equivalence/query.smt2

python3 tools/coop/run.py equivalence replay \
  build/ppc-equivalence/counterexample.json
```

Decode a block, extract a symbol from an object, or check raw binary files /
named ELF functions:

```bash
python3 tools/coop/run.py equivalence decode \
  --hex "38630004 5463103a" --base 0x80001000

python3 tools/coop/run.py equivalence extract \
  --object build/us/obj/kyoshin/CGame.o --list

python3 tools/coop/run.py equivalence extract \
  --object build/us/obj/kyoshin/CGame.o \
  --symbol OnPauseTrigger__5CGameFv --out build/ppc-equivalence/orig.bin

python3 tools/coop/run.py equivalence check \
  --original original.bin --candidate candidate.bin \
  --base-original 0x80001000 --base-candidate 0x80002000 \
  --observe r3,cr0,xer.ca --json

# Direct object pair (retail / decomp .o files from objdiff):
python3 tools/coop/run.py equivalence check-objects \
  --original build/us/obj/kyoshin/CGame.o \
  --candidate build/us/src/kyoshin/CGame.o \
  --symbol OnPauseTrigger__5CGameFv

# Same, resolved from objdiff.json (builds/post-processes the decomp object):
python3 tools/coop/run.py equivalence check-unit kyoshin/CGame \
  --symbol OnPauseTrigger__5CGameFv
```

`check-objects` / `check-unit` load instruction bytes from the named `.text`
symbol in each ELF32 big-endian object (the same pair objdiff compares). Decode
bases default to each symbol’s section address + `st_value`. A function with
supported unresolved `.rela.text` entries is checked with canonical symbolic
addresses, not placeholder instruction fields. The current set is
`R_PPC_ADDR16_LO`, `R_PPC_ADDR16_HI`, `R_PPC_ADDR16_HA`, `R_PPC_REL24`,
`R_PPC_REL14`, and `R_PPC_EMB_SDA21`. Unsupported relocation types and implicit
`.rel.text` addends fail closed. When fuzzy match is in `[50%, 100%)`,
`coop run diff` / `cycle` run this automatically and may promote status to
**`EQUIVALENT_MATCH`** (the fork’s default acceptance bar, alongside
`FULL_MATCH`). Split-size fit remains mandatory.

The solver also enforces the conditions needed for those symbolic values to be
linkable: REL24/REL14 targets must be word-aligned and within the encoded signed
range, and SDA21 must be within signed-16 reach of either the initial r2 or r13.
An impossible set of bindings is `inconclusive_layout`, never a vacuous proof.
`decoder.specialize_relocations` provides an independent concrete linker model
used by boundary tests for HA carry, branch range/alignment, and SDA base choice.

### Linked-bytes fallback (`--linked`)

Symbolic relocation checking is the default. Pass `--linked` to let `diff` /
`cycle` / `equivalence check-unit` retry an unsupported relocation case once
with **already-linked bytes**
extracted from:

- the retail side: `orig/<region>/sys/main.dol`, sliced by `(address, size)`
  looked up in `config/<region>/symbols.txt`, via
  `tools/ppc_equivalence/dol_symbols.extract_by_address`;
- the candidate side: `build/<region>/main.elf` (the linked ELF consumed by
  `elf2dol`; gitignored, produced by `ninja build/<region>/main.elf`), via
  `tools.ppc_equivalence.elf_symbols.extract_function`.

Both sources are relocation-free. Results are decorated with
`fell back to DOL/ELF linked bytes`
in their `detail` field.

```bash
# Run ninja first so the linked ELF exists for the candidate side:
ninja build/us/main.elf

# Then any of:
python3 tools/coop/run.py diff kyoshin/CGame --symbol OnPauseTrigger__5CGameFv --linked
python3 tools/coop/run.py cycle pad-copy-input-flag --linked
python3 tools/coop/run.py equivalence check-unit kyoshin/CGame --symbol OnPauseTrigger__5CGameFv --linked
```

The fallback path returns an address-specialized `equivalent` / `not_equivalent` verdict
whenever the linked bytes decode cleanly; it stays `inconclusive_unsupported`
only when the DOL or ELF is missing, the symbol is absent from `symbols.txt`,
or the engine hits an unsupported instruction. The linked path is a strict
*concretization* of the proof — it bakes in retail’s concrete addresses, so a
decomp function whose only difference is link layout can be reported
`not_equivalent` here. Prefer the default symbolic proof when it supports every
relocation in the function.

Direct relocated calls compose modularly. Each named callee is an explicit
matched-callee premise. Same-object matched callees can receive precise
read/write summaries inferred from both bodies (including a second pass that
composes nested precise summaries). Missing or opaque nested callees stay
fail-closed as `nested-call-opaque-eabi`. Validation failures widen to the
union of declared and required effects instead of collapsing to full opaque
EABI. Standalone `check-objects` infers these summaries by default; raw block
checks do not silently assume relocated callees — pass
`--assume-relocated-callees` for opaque EABI or `--callee-contracts` for
explicit summaries. The target workflow is stricter: `cycle` issues a semantic
certificate in
`tools/coop/targets.json` only after both bodies validate against the summary
and have only normal returns. The certificate binds the architecture/result
model, function-local bytes and relocations, summary, and the hashes of every
callee certificate. Before a caller consumes it, the workflow re-extracts and
hashes both current object functions. A missing, malformed, stale, or
transitively stale certificate returns `inconclusive_unvalidated_callee`.
`harness --selection callees-accepted` uses this same certified frontier rather
than match status alone. MWCC `_savegpr_*`, `_restgpr_*`, `_savefpr_*`, and
`_restfpr_*` calls are certificate-bound fixed EABI runtime leaves; the engine
models their exact r11-relative register load/store ranges instead of treating
them as arbitrary memory writers. A contract that writes memory
may modify any caller-frame byte reachable through an alias. Only a validated
memory-free contract preserves the input memory array unchanged. Distinct
callee symbols receive distinct transitions.
Raw/linked calls without a registered callee lemma are inconclusive; result JSON
records every premise actually used, including the contract source and complete
validated/fallback read/write sets. An UNSAT result through these summaries is
still a valid compositional proof, but a SAT model involving one is
`inconclusive_abstraction` rather than `not_equivalent` because an opaque
function may choose behavior that the real matched callee cannot.

## Contract and exit codes

Choose one named `--contract` or a manual `--observe` list. `check-objects` and
`check-unit` default to `auto`; raw block checks through the co-op runner
default to `ppc-eabi`. The standalone raw-block checker requires an explicit
choice.

| Contract | Compared state |
|---|---|
| `auto` | `ppc-eabi`, augmented with each persistent non-ABI component written by either implementation: FPSCR, individual GQRs/SRs, MSR, time base, SRR0/SRR1, and modeled auxiliary SPRs |
| `ppc-eabi` | `r1`, `r2`, return pair `r3:r4`, both lanes of FP return `f1`, `r13`–`r31`, both lanes of nonvolatile `f14`–`f31`, `CR2`–`CR4`, and all final memory |
| `ppc-eabi-fp` | `ppc-eabi` plus FPSCR, available as an explicit fixed alternative to `auto` |
| `strict` | All modeled GPRs, both lanes of every FPR, GQR0–GQR7, SR0–SR15, complete CR/FPSCR, `XER.CA/OV/SO`, LR, CTR, MSR, time base, SRR0/SRR1, every modeled auxiliary SPR, and all final memory |
| `live-out` | Conservative automatic over-approximation: every modeled component written by either block |

Final-memory comparison uses per-implementation independent private-stack
masking: each implementation's own private stack interval is masked
independently. An address private to one implementation does not hide a write
by the other implementation when that write lies outside the other
implementation's own private interval. Masking is disabled after a call or when
an r1-derived value is stored to memory, because the frame may then be
reachable through an alias. Stores outside the interval always remain
observable. The proof additionally requires a well-formed, non-wrapping
downward stack; impossible stack/link layouts are reported as
`inconclusive_layout`.

`auto` is the default completed-function contract. It resolves from the union
of both decoded implementations' write effects, so a state component cannot
escape comparison merely because only one side writes it. Volatile compiler
scratch—GPR/FPR temporaries, volatile CR fields, XER, LR, and CTR—is not added.
Text and JSON results report the `ppc-eabi` base, every automatically added
observable, and which side wrote it.

The fixed ABI preset intentionally excludes
volatile scratch registers and flags. It conservatively treats `r3:r4` as a
return pair; for a `void` function or a known narrower return contract, use a
manual list. For an internal basic block, pass the
actual live-outs manually because a volatile register can still be live. The
automatic `live-out` contract is sound but intentionally conservative; it can
reject a transformation that only changes a dead write:

```bash
python3 tools/coop/run.py equivalence check-hex \
  --original <hex> --candidate <hex> \
  --observe r3,r5,cr0
```

`--observe` may be repeated or comma-separated. Supported names are `r0`–`r31`,
`f0`–`f31`, `cr`, `cr0`–`cr7`, `fpscr`, `xer.ca`, `xer.ov`, `xer.so`, `lr`,
`ctr`, `gqr0`–`gqr7`, `sr0`–`sr15`, `msr`, `time_base`, `srr0`, `srr1`,
the lower-case names listed in `spr.py` (for example `dsisr`, `ibat0u`, `hid0`,
and `pmc1`), `f0.ps1`–`f31.ps1`, and `memory`.

| Exit | Meaning |
|---:|---|
| 0 | Equivalent under the printed contract |
| 1 | Not equivalent; a concrete model exists |
| 2 | Inconclusive (unsupported, timeout, or solver unknown) |
| 3 | Invalid input or contract |
| 4 | Missing dependency or internal tool error |

JSON proof results (format 8) record the architecture model, requested and
resolved contract metadata, observables, assumptions, instruction counts,
solver/version/timing, and—when applicable—the first mismatch and
replayable input state.

Optional `--concrete-samples N` runs a **secondary defense** after SMT:
interesting plus seeded-random ConcreteOps initial states through both CFGs.
A concrete mismatch demotes `equivalent` / inconclusive statuses to
`not_equivalent` (fail-closed), but sampling never promotes to `equivalent`
and is never treated as an SMT certificate. Coop `EQUIVALENT_MATCH` promotion
remains SMT-gated.

## Supported model (phases 1–3)

The current `broadway-ppc32-be-v23` model supports:

- integer add/subtract families, carry, `OE`, sticky `XER.SO`, multiply-high,
  multiply-low, signed/unsigned divide, negate, sign extension, and count-zero;
- immediate and register logical operations, rotates, masks, logical/arithmetic
  shifts, record forms, integer compares, and CR logical operations;
- `mfcr`, `mtcrf`, and all retail XER/LR/CTR/GQR plus exception, MMU, BAT,
  hardware/cache, DMA, performance, and debug `mfspr`/`mtspr` operands as
  explicit architectural values;
- byte, halfword, and word integer loads/stores in D-form and indexed form,
  update forms, `lmw`/`stmw`, and byte-reversed halfword/word forms. Broadway's
  observed `lmw` behavior is modeled when RA lies in the destination range:
  the effective address is latched before any GPR is written. Generic PowerPC
  calls this form boundedly undefined; decoder clients can retain that strict
  rejection with `allow_broadway_lmw_overlap=False`;
- a shared symbolic byte-addressed memory array with big-endian multi-byte
  access and exact final-array comparison;
- cache/order operations `dcbf`, `dcbi`, `dcbst`, `dcbt`, `icbi`, `sync`, and
  `isync` under coherent ordinary-RAM/no-DMA/no-self-modifying-code assumptions,
  plus 32-byte aligned memory zeroing for `dcbz` when HID0.DCE is enabled and
  `dcbz_l` when HID0.DCE plus HID2.LCE are enabled;
- MSR and all 16 segment registers through `mfmsr`, `mtmsr`, `mfsr`, and
  `mtsr`, SRR0/SRR1 SPR transfers, stable-block time-base reads through `mftb`,
  and TBL/TBU writes through `mtspr`;
- synchronous `twi`, `sc`, and `rfi` control exits, including trap predicates,
  exception vectors, SRR0/SRR1 saves, and Broadway MSR entry/restore masks;
- `b`, `bc`, `bclr`, and `bcctr`, including AA/LK, CR tests, CTR decrement/test,
  LR updates, indirect aligned targets, acyclic CFG paths, and exit comparison;
  back-edges are explored with a per-PC visit bound (`max_loop_iterations`,
  default 256)—constant-trip loops whose exit becomes concrete within the bound
  are supported; exhausting the bound is inconclusive and never silently
  truncated;
- scalar FP D-form/indexed loads and stores (`lfs*`, `lfd*`, `stfs*`, `stfd*`,
  `stfiwx`) with big-endian memory and binary32/binary64 conversion;
- scalar `fadd[s]`, `fsub[s]`, `fmuls`, `fdiv[s]`, double `fmul`, `frsp`,
  `fsel`, ordered/unordered `fcmpo`/`fcmpu`, and bit-exact
  `fmr`/`fneg`/`fabs`/`fnabs`, `fctiw`/`fctiwz` conversion, and the complete
  scalar fused family: `fmadd[s]`, `fmsub[s]`, `fnmadd[s]`, and `fnmsub[s]`;
- Broadway table-exact reciprocal estimates `fres` and `frsqrte`, including
  normal/subnormal inputs, signed zero, infinities, NaNs, `ZX`/`VXSNAN`/
  `VXSQRT`, enabled-result suppression, destination-lane behavior, and Rc;
- complete FPSCR access/mutation semantics for `mffs`, `mtfsf`, `mtfsfi`,
  `mtfsb0`, `mtfsb1`, and `mcrfs`, including reserved-bit masking and summary
  recomputation;
- quantized paired-single loads/stores (`psq_l[u][x]` and `psq_st[u][x]`) with
  separate PS1 lane state, all eight GQRs, float/U8/U16/S8/S16 formats,
  signed six-bit scaling, saturation, W=1 lane filling, and update addressing;
- bit-exact paired-single moves/sign operations (`ps_mr`, `ps_neg`, `ps_abs`,
  `ps_nabs`) and all four `ps_merge**` lane-selection forms, including aliasing,
  NaN payload preservation, signed zero, and Rc-to-CR1;
- paired ordered/unordered comparisons (`ps_cmpo0/1`, `ps_cmpu0/1`) over either
  PS lane, including CR-field and FPSCR.FPCC updates, `VXSNAN`/`VXVC`, summary
  bits, and invalid-enable interaction;
- paired basic arithmetic (`ps_add`, `ps_sub`, `ps_mul`, `ps_muls0/1`) with
  independent binary32-rounded lanes, Force25 multiplier handling, PS0 FPRF,
  accumulated lane exceptions, FI/FR clearing/preservation, unconditional
  paired result writeback under enabled invalid exceptions, and Rc-to-CR1;
- paired division (`ps_div`) with independent binary32-rounded lanes,
  accumulated `VXZDZ`/`VXIDI`/`VXSNAN`/`ZX` exceptions, unconditional paired
  writeback under enabled exceptions, PS0 FPRF, and Rc-to-CR1;
- Broadway table-exact paired estimates (`ps_res`, `ps_rsqrte`) across both
  lanes, including zero, infinity, NaN, signaling-NaN, negative-rsqrt,
  ForceSingle rsqrt results, accumulated exceptions, unconditional writeback,
  PS0 FPRF, and Rc-to-CR1;
- paired fused arithmetic (`ps_madd`, `ps_msub`, `ps_nmadd`, `ps_nmsub`,
  `ps_madds0/1`) with independently fused and binary32-rounded lanes,
  broadcast multipliers, Force25 handling, NaN operand priority, accumulated
  lane exceptions, unconditional paired writeback, PS0 FPRF, and Rc-to-CR1;
- paired cross-lane sums (`ps_sum0/1`) with binary32 result/copy forcing,
  arithmetic-lane exception behavior, selected-lane FPRF, and Rc-to-CR1;
- bit-preserving paired selection (`ps_sel`), including signed-zero and NaN
  predicates, independent lanes, unchanged FPSCR, and Rc-to-CR1;
- FPSCR rounding-mode input, FPRF/FPCC result classification, Rc-to-CR1, FP
  compare invalid causes (`VXSNAN`/`VXVC`) with `FX`/`VX`/`FEX` summaries and
  `VE` behavior, scalar add/subtract/multiply/divide invalid causes and
  divide-by-zero (`ZX`) with `VE`/`ZE` result suppression, FP observables, and
  conversion `VXSNAN`/`VXCVI`/`XX`/`FI`/`FR`, packed integer-result format,
  all four rounding modes, EABI FP return/nonvolatile registers;
- configurable `--max-instructions` and `--max-paths` bounds.

Modeled arithmetic requires round-to-nearest-even and `FPSCR.NI=0`.
Non-finite results are admitted when caused by non-finite inputs or the modeled
invalid/divide-by-zero cases; finite-input overflow remains outside the domain.
Invalid-operation and divide-by-zero causes plus enabled-result suppression are
modeled for supported scalar add/subtract/multiply/divide. Arithmetic overflow,
underflow, and inexact-result flags,
and architectural trap delivery remain outside the declared value-semantics
model. `fctiw`/`fctiwz` separately model conversion inexact and invalid flags.
Fused-single and paired-fused ConcreteOps follow Broadway's
mixed-precision/Force25 behavior for arbitrary FPR inputs; symbolic proofs
require each finite operand to be an exact binary32 value expanded into an FPR,
matching the dominant compiler use.
The `fsqrt`/`fsqrts` encodings found in later PowerPC revisions are reserved on
Broadway and are rejected by the decoder. Auxiliary SPR values are compared,
but decrementer/performance-counter progression and the behavioral effects of
BAT/SDR1 translation, HID/L2/cache locking, DMA, and debug registers are
explicitly outside the bounded value-semantics model. VMX,
atomics/reservations, detailed cache locking/coherency, asynchronous interrupts,
MMIO behavior, address translation effects, unbounded/symbolic loops that exceed
the visit bound, indirect calls,
and memory/protection/alignment exceptions return inconclusive
or are outside the declared model. Division outputs are compared only where
the ISA defines the result. A direct call is summarized only under a recorded
matched-callee premise; otherwise it fails closed and code after the call is
never silently skipped.

See [ISA references](REFERENCES.md) for the source hierarchy and modeled
assumptions.

## Development and quality gate

```bash
python3 tools/ppc_equivalence/gen_fixture_blob.py
python3 tools/ppc_equivalence/run.py --self-test
python3 tools/coop/run.py equivalence differential
python3 tools/coop/run.py behaviour ppc ppc-equivalence-fixtures
python3 tools/ppc_equivalence/run.py --help
```

| Gate | Required where | Command |
|---|---|---|
| Fixture regenerate freshness | CI + local | `gen_fixture_blob.py --check` |
| Full Python suite in pinned environment | CI + local | `run.py --self-test` |
| ConcreteOps corpus | CI + local | `equivalence differential` |
| Dolphin interpreter DOL | Local before advertising opcodes | `behaviour ppc ppc-equivalence-fixtures` |

The shared fixture corpus in `tools/ppc_equivalence/fixtures/` is the single
source of truth for the closed loop:

1. Python `ConcreteOps` executes every fixture (`equivalence differential` / CI).
2. `gen_fixture_blob.py` emits `ppc_fixture_cases.inc`, `ppc_fixture_payloads.c`,
   and `broadway.jsonl`.
3. The generic Dolphin DOL (`ppc-equivalence-fixtures`) loads each case’s GPR,
   FPR, CR, XER, FPSCR, and memory state, `blrl`s into a generated `.text`
   payload under the interpreter (`CPUCore = 0`), and compares the same
   expected state.
   Headless user inis disable panic handlers and analytics prompts.

Do not hand-edit generated `ppc_fixture_*.inc/.c` files. Edit
`fixtures/corpus.py`, regenerate, then run both gates.

### New opcode / model-change checklist

Before advertising a newly supported opcode or semantic change:

1. Add edge fixtures in `fixtures/corpus.py` (record form, CA/OV/SO, update
   addressing, and at least one negative neighbour when useful).
2. Run `python3 tools/ppc_equivalence/gen_fixture_blob.py`.
3. Pass `python3 tools/coop/run.py equivalence differential`.
4. Pass unit tests under `tools/ppc_equivalence/tests/` (encode + positive /
   negative symbolic cases as applicable).
5. Pass `python3 tools/coop/run.py behaviour ppc ppc-equivalence-fixtures`
   (Dolphin interpreter; required locally when Dolphin is available).
6. Update `REFERENCES.md` / README supported-model notes if the contract or
   assumptions changed.

Real-game regression cases must not commit proprietary binaries; store legal
small fixtures or hashes/extraction instructions instead.
