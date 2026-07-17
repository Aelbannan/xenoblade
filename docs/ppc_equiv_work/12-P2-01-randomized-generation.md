# P2-01 — Add deterministic randomized differential generation

**Priority:** P2  
**Primary owner areas:** generators, tests, Dolphin harness

---

## Goal

Handwritten fixtures cannot cover all operand aliases, immediate boundaries, and interacting state. Add deterministic property-based generation.

## Three-way oracle

For each generated program/input:

1. Execute with `ConcreteOps`.
2. Constrain the symbolic initial state to the same concrete state and evaluate/prove the symbolic final state.
3. Execute the generated DOL in Dolphin interpreter.

All modeled observable states must agree within the declared scope.

## Generator design

Do not use uniform random instruction words. Generate valid instructions by family and deliberately weight difficult cases.

### Operand alias strata

- all registers distinct;
- destination equals source A;
- destination equals source B;
- both sources equal;
- `r0` special cases;
- `r1` stack-sensitive cases;
- CR field boundaries;
- paired FPR aliases.

### Immediate/boundary strata

- `0`, `1`, `-1`;
- signed min/max;
- mask boundaries;
- shifts `0`, word-size minus one, word-size, greater than word-size where encoding permits;
- branch displacement min/max;
- relocation range boundaries;
- carry/overflow transition values.

### Memory strata

- aligned addresses;
- each misalignment class for negative/inconclusive tests;
- width overlap;
- update forms;
- stack frame boundaries;
- range start/end;
- no-wrap/wraparound.

### Floating-point strata

Use explicit bit patterns, not only host-language floats:

- zeros, normals, subnormals, infinities, NaNs;
- tie-to-even boundaries;
- exponent transition values;
- reciprocal estimate table edges;
- PSQ scale/type boundaries.

## Seed and reproduction format

Every run must print/store:

- master seed;
- generator version;
- architecture model;
- selected instruction family;
- exact program bytes;
- exact initial state bits;
- environment profile;
- expected/actual outputs.

Example:

```json
{
  "generator": "ppc-random-v1",
  "seed": 1489112045,
  "architecture_model": "broadway-ppc32-be-vNEXT",
  "program_hex": "...",
  "initial_state": {
    "gpr": ["0x..."],
    "fpr_bits": ["0x..."],
    "cr": "0x...",
    "xer": "0x...",
    "memory_bytes": {"0x80001000": "0xff"}
  }
}
```

## Shrinking

On failure, minimize:

- number of instructions;
- nondefault registers/memory bytes;
- immediate magnitude where possible;
- path complexity.

Preserve the original full reproduction before shrinking.

## Metamorphic properties

Add properties that do not require a second implementation:

- encoding/decoding round trip where supported;
- `x + 0 == x` without unexpected flags;
- rotate by zero preserves value with specified record behavior;
- store then load same width/address returns original bits in ordinary RAM;
- equivalent instruction aliases produce same state;
- branch inversion partitions condition domain;
- byte-swap expectations under big-endian composition.

## Acceptance criteria

- Re-running a stored seed is deterministic.
- CI saves complete failure bundles.
- Coverage statistics show each supported opcode/family and boundary stratum.
- Dolphin and symbolic comparisons use exact bit patterns.
