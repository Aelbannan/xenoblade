# PPC equivalence check

This check proves or refutes equivalence of supported, straight-line Wii
PowerPC blocks even when their instruction bytes differ. Both blocks execute
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
| `ppc-eabi` | `r1`, `r2`, return pair `r3:r4`, `r13`–`r31`, and `CR2`–`CR4` |
| `strict` | All modeled GPRs, complete CR, `XER.CA/OV/SO`, LR, and CTR |

The ABI preset is for completed function boundaries. It intentionally excludes
volatile scratch registers and flags. It conservatively treats `r3:r4` as a
return pair; for a `void` function or a known narrower return contract, use a
manual list. For an internal basic block, pass the
actual live-outs manually because a volatile register can still be live:

```bash
python3 tools/coop/run.py equivalence check-hex \
  --original <hex> --candidate <hex> \
  --observe r3,r5,cr0
```

`--observe` may be repeated or comma-separated. Supported names are `r0`–`r31`,
`cr`, `cr0`–`cr7`, `xer.ca`, `xer.ov`, `xer.so`, `lr`, and `ctr`.

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

## Supported MVP

The current `ppc32-be-v1` model is deliberately narrow:

- straight-line blocks only, up to 256 instructions;
- no loads/stores, calls, branches, exceptions, floating point, paired-single,
  atomics, MMIO, or loops;
- `addi`, `addis`, `mulli`;
- `ori`, `oris`, `xori`, `xoris`, `andi.`, `andis.`;
- `and`, `or`, `xor`, `nor`;
- `rlwinm`, `rlwimi`;
- `add`, `subf` without `OE`;
- `cmpw`, `cmplw`, `cmpwi`, `cmplwi`;
- `extsb`, `extsh`, `cntlzw`;
- record-form CR0 effects and comparison propagation of `XER.SO`.

All other encodings return `inconclusive_unsupported`. Memory and acyclic
control flow should be added as later model versions, with independent
differential tests, rather than silently broadening `ppc32-be-v1`.

## Development and quality gate

```bash
python3 -m unittest discover -s tools/ppc_equivalence/tests -v
python3 tools/ppc_equivalence/run.py --help
```

Every new opcode must add encoding fixtures, concrete edge cases, symbolic
positive and negative equivalence cases, state-preservation checks, and an
independent Dolphin/QEMU/reference test before it is advertised as supported.
Real-game regression cases must not commit proprietary binaries; store legal
small fixtures or hashes/extraction instructions instead.
