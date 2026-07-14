# PPC equivalence check

This check proves or refutes equivalence of supported Wii Broadway PowerPC
blocks even when their instruction bytes differ. Both blocks execute
from the same symbolic input state; Z3 searches for a difference in the state
selected by the contract.

It complements, but does not replace:

- objdiff / `FULL_MATCH`, which remains the acceptance bar for decomp targets;
- host behaviour tests, which remain mandatory below `FULL_MATCH`;
- the Dolphin PPC harness, which supplies independent concrete evidence.

An `equivalent` result is scoped to the listed observables and assumptions. An
unsupported instruction, timeout, or solver `unknown` is always
`inconclusive`; the check never treats unsupported behavior as a no-op.

## Install

From the repository root:

```bash
python3 -m pip install -r tools/ppc_equivalence/requirements.txt
```

The pinned runtime dependencies are Capstone (decoder cross-check) and Z3.
The semantic decoder also validates instruction fields directly; semantics do
not depend on formatted disassembly text.

## Quick checks

Prove that `rlwinm r3,r3,2,0,29` is equivalent to `mulli r3,r3,4` when only
`r3` is observable:

```bash
python3 tools/coop/run.py equivalence check-hex \
  --original 5463103a \
  --candidate 1c630004
```

The co-op runner supplies `--contract ppc-eabi` when neither `--contract` nor
`--observe` is present.

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

Decode a block or check raw binary files:

```bash
python3 tools/coop/run.py equivalence decode \
  --hex "38630004 5463103a" --base 0x80001000

python3 tools/coop/run.py equivalence check \
  --original original.bin --candidate candidate.bin \
  --base-original 0x80001000 --base-candidate 0x80002000 \
  --observe r3,cr0,xer.ca --json
```

## Contract and exit codes

Choose one named `--contract` or a manual `--observe` list. The co-op runner
defaults checks to `--contract ppc-eabi`; the standalone checker requires an
explicit choice.

| Contract | Compared state |
|---|---|
| `ppc-eabi` | `r1`, `r2`, return pair `r3:r4`, `r13`–`r31`, `CR2`–`CR4`, and all final memory |
| `strict` | All modeled GPRs, complete CR, `XER.CA/OV/SO`, LR, CTR, and all final memory |
| `live-out` | Conservative automatic over-approximation: every modeled component written by either block |

The ABI preset is for completed function boundaries. It intentionally excludes
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
`cr`, `cr0`–`cr7`, `xer.ca`, `xer.ov`, `xer.so`, `lr`, `ctr`, and `memory`.

| Exit | Meaning |
|---:|---|
| 0 | Equivalent under the printed contract |
| 1 | Not equivalent; a concrete model exists |
| 2 | Inconclusive (unsupported, timeout, or solver unknown) |
| 3 | Invalid input or contract |
| 4 | Missing dependency or internal tool error |

JSON proof results record the architecture model, observables, assumptions,
instruction counts, solver/version/timing, and—when applicable—the first
observable mismatch and replayable input state.

## Supported model (phases 1 and 2)

The current `broadway-ppc32-be-v2` model supports:

- integer add/subtract families, carry, `OE`, sticky `XER.SO`, multiply-high,
  multiply-low, signed/unsigned divide, negate, sign extension, and count-zero;
- immediate and register logical operations, rotates, masks, logical/arithmetic
  shifts, record forms, integer compares, and CR logical operations;
- `mfcr`, `mtcrf`, and user-visible XER/LR/CTR `mfspr`/`mtspr` forms;
- byte, halfword, and word integer loads/stores in D-form and indexed form,
  update forms, `lmw`/`stmw`, and byte-reversed halfword/word forms;
- a shared symbolic byte-addressed memory array with big-endian multi-byte
  access and exact final-array comparison;
- `b`, `bc`, `bclr`, and `bcctr`, including AA/LK, CR tests, CTR decrement/test,
  LR updates, indirect aligned targets, acyclic CFG paths, and exit comparison;
- configurable `--max-instructions` and `--max-paths` bounds.

Loops/back-edges, continuations after external calls, floating point,
paired-single, VMX, atomics/reservations, cache/MMIO behavior, privileged state,
and memory/protection/alignment exceptions return inconclusive or are outside
the declared model. Division outputs are compared only where the ISA defines
the quotient. Direct calls are compared as terminal call exits (target, LR,
contract state); the callee is not guessed or silently skipped.

See [ISA references](REFERENCES.md) for the source hierarchy and modeled
assumptions.

## Development and quality gate

```bash
python3 -m unittest discover -s tools/ppc_equivalence/tests -v
python3 -m unittest discover -s tools/test/compare_behaviour/tests -v
python3 tools/coop/run.py behaviour ppc ppc-equivalence-broadway
python3 tools/ppc_equivalence/run.py --help
```

The Dolphin command runs 40 representative Broadway instruction scenarios and
one memory-layout assertion through Dolphin's interpreter. It is the independent
concrete ISA oracle for the phase-1/phase-2 model; failures print actual and
expected result/CR/XER triples.

Every new opcode must add encoding fixtures, concrete edge cases, symbolic
positive and negative equivalence cases, state-preservation checks, and an
independent Dolphin/QEMU/reference test before it is advertised as supported.
Real-game regression cases must not commit proprietary binaries; store legal
small fixtures or hashes/extraction instructions instead.
