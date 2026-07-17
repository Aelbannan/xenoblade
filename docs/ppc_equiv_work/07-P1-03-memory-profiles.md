# P1-03 — Add enforceable memory-environment profiles

**Priority:** P1  
**Primary owner areas:** engine, CLI, result schema, promotion policy

---

## Problem statement

The current array model can represent any 32-bit address, while assumptions such as "mapped ordinary RAM," no MMIO, valid protection, and ordinary cache behavior are largely external. For a symbolic pointer, the solver may choose an address that the model treats like normal RAM even though real hardware would access MMIO or fault.

A proof can still be useful under that assumption, but acceptance policy must know whether the assumption was enforced or merely declared.

## Profiles

Introduce explicit profiles:

### `assumed-ordinary-ram`

Current broad behavior:

- addresses are unconstrained except for modeled alignment/definedness;
- all accesses are interpreted as ordinary byte-array memory;
- result clearly records this as an external assumption;
- default auto-promotion should not classify this above Tier C when pointers are symbolic and unconstrained.

### `bounded-ordinary-ram`

- caller supplies one or more valid RAM ranges;
- every touched byte of every memory operation must lie within a configured range;
- no address wraparound;
- alignment constraints are enforced;
- suitable for Tier B when ranges are appropriate for the executable/environment.

### `stack-and-known-globals`

- stack accesses constrained to a configured valid stack region;
- relocation-backed globals constrained to known DOL sections or declared RAM ranges;
- unknown pointer accesses are rejected/inconclusive unless separately bounded.

### `hardware-aware`

Future profile. Until actual MMIO/protection/cache semantics exist, unsupported regions must fail closed rather than be modeled as ordinary memory.

## Range constraint helper

Be careful about width and overflow:

```python
def access_within_range(addr, width_bytes: int, low: int, high: int):
    if width_bytes <= 0:
        raise ValueError("width_bytes must be positive")

    width = z3.BitVecVal(width_bytes - 1, 32)
    last = addr + width

    return z3.And(
        z3.UGE(addr, z3.BitVecVal(low, 32)),
        z3.ULE(last, z3.BitVecVal(high, 32)),
        z3.UGE(last, addr),  # reject 32-bit wraparound
    )


def access_within_any_range(addr, width_bytes, ranges):
    return z3.Or(*[
        access_within_range(addr, width_bytes, low, high)
        for low, high in ranges
    ])
```

Use each instruction's actual width. For vector/paired accesses or multi-part effects, cover all touched bytes.

## Policy choice for out-of-profile paths

Do not simply constrain away illegal addresses and claim universal equivalence. The theorem must say equivalence is restricted to initial states satisfying the profile. Record the restriction prominently.

A separate "prove domain agreement" query may be useful:

- can one implementation access outside the profile while the other remains within it?
- can their access validity differ?

If so, classify according to the formal definedness policy rather than hiding it.

## Result schema

```json
{
  "environment": {
    "memory_profile": "bounded-ordinary-ram",
    "ranges": [
      {"start": "0x80000000", "end": "0x817fffff", "kind": "ram"}
    ],
    "alignment": "instruction-natural",
    "mmio": "excluded-by-range",
    "address_wraparound": "rejected"
  }
}
```

## Tests

- start/end boundaries for every access width;
- access whose first byte is in range but final byte is out;
- 32-bit wraparound;
- disjoint ranges;
- symbolic address inside/outside witness;
- one side accesses MMIO-excluded region;
- stack range interaction;
- result/promotion tier changes by profile.
