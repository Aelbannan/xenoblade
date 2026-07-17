# P0-01 — Fix asymmetric private-stack memory masking

**Priority:** P0 / release blocker  
**Primary owner area:** `tools/ppc_equivalence/engine.py`  
**Dependencies:** none  
**Blocks:** automatic equivalence acceptance, model-version bump, re-certification

---

## Problem statement

The final-memory comparison should ignore an implementation's own dead private stack storage while still observing that implementation's writes outside its own private frame.

The current structure is effectively:

```python
private = z3.Or(
    left_private(address),
    right_private(address),
)

left_byte = z3.If(private, initial_byte, z3.Select(left.memory, address))
right_byte = z3.If(private, initial_byte, z3.Select(right.memory, address))
```

This applies a union mask to both sides. It is unsound when the two implementations have different private intervals.

## Counterexample

Illustrative original:

```asm
# 64-byte frame
stwu r1, -64(r1)
addi r1, r1, 64
blr
```

Illustrative candidate:

```asm
# 16-byte frame
stwu r1, -16(r1)
stw  r3, -32(r1)     # writes entry_sp - 48
addi r1, r1, 16
blr
```

At the store:

- candidate `r1 = entry_sp - 16`;
- store effective address is `entry_sp - 48`;
- that address is outside the candidate's 16-byte frame;
- it is inside the original's 64-byte frame.

The candidate write must be observable. A union mask hides it.

## Required implementation

Mask each terminal memory independently:

```python
left_is_private = _private_stack_address(
    address,
    left.stack_low,
    initial.gpr[1],
    left.stack_private,
    ops,
)

right_is_private = _private_stack_address(
    address,
    right.stack_low,
    initial.gpr[1],
    right.stack_private,
    ops,
)

left_byte = z3.If(
    left_is_private,
    initial_byte,
    z3.Select(left.memory, address),
)

right_byte = z3.If(
    right_is_private,
    initial_byte,
    z3.Select(right.memory, address),
)

byte_difference = left_byte != right_byte
```

This expresses the intended quotient over dead private storage:

- the left side's private bytes are replaced with common initial memory only on the left;
- the right side's private bytes are replaced with common initial memory only on the right;
- a write public on one side remains visible even when the same address is private on the other side.

## Important semantic edge cases

The patch must preserve the following behavior:

1. **No write at the address:** both sides still equal common initial memory.
2. **Write private only on the left:** left write is ignored; right public write or unchanged value remains compared.
3. **Write private only on the right:** symmetric case.
4. **Private on both sides:** both sides compare as initial memory.
5. **Stack privacy disabled on one side:** no masking may be applied to that side.
6. **Stack escape:** once the implementation can expose its stack pointer to unknown code or memory, privacy must remain disabled according to current stack-analysis policy.
7. **Calls:** if calls invalidate private-stack reasoning, the comparison must obey that per-side state.
8. **32-bit wraparound:** invalid/upward/wrapping stack layouts must not be classified as ordinary private intervals.
9. **Path-specific frames:** each terminal state's `stack_low` and `stack_private` must be used; do not merge paths prematurely.
10. **Touched-byte optimization:** all addresses written by either side must remain included in the comparison set.

## Required tests

Add a dedicated test class or module, for example:

```text
tools/ppc_equivalence/tests/test_private_stack_memory.py
```

Minimum matrix:

| Test | Expected result |
|---|---|
| Candidate has smaller frame and writes inside original-only private interval | `NOT_EQUIVALENT` |
| Original has smaller frame and writes inside candidate-only private interval | `NOT_EQUIVALENT` |
| Different frame sizes; each side writes only within its own frame | `EQUIVALENT` |
| One side has a private address and the other leaves it unchanged | `EQUIVALENT` if no public output differs |
| Same public write/value on both sides despite different frames | `EQUIVALENT` |
| Different public values at address private to only one side | `NOT_EQUIVALENT` |
| Stack-pointer escape disables masking on original only | Public difference remains visible |
| Stack-pointer escape disables masking on candidate only | Public difference remains visible |
| A call disables or prevents private-stack inference | No unsafe masking |
| Multiple terminal paths with different frame lows | Each pair is handled independently |
| Frame restored before return | Private writes remain treated according to owned frame history/policy |
| Upward/wrapping/invalid stack layout | `INCONCLUSIVE_LAYOUT` or the existing fail-closed equivalent |

Suggested test shape:

```python
def test_candidate_public_write_is_not_hidden_by_original_larger_frame():
    original = bytes.fromhex(
        "9421ffc0"  # stwu r1,-64(r1)
        "38210040"  # addi r1,r1,64
        "4e800020"  # blr
    )
    candidate = bytes.fromhex(
        "9421fff0"  # stwu r1,-16(r1)
        "9061ffe0"  # stw r3,-32(r1)
        "38210010"  # addi r1,r1,16
        "4e800020"  # blr
    )

    result = prove_bytes_for_test(
        original,
        candidate,
        contract="auto",
    )

    assert result.status == ProofStatus.NOT_EQUIVALENT
    assert result.counterexample is not None
```

Use the existing project test helpers rather than introducing a second proof setup. The test must first be run against the old implementation and demonstrated to fail by returning `EQUIVALENT`; record that evidence in the pull request.

## Documentation changes

Replace ambiguous wording such as:

> memory outside either implementation's private stack frame

with explicit language:

> Each implementation's own private stack interval is masked independently. An address private to one implementation does not hide a write by the other implementation when that write lies outside the other implementation's own private interval.

Add the asymmetric-frame example to the README or `SOUNDNESS.md`.

## Result metadata

The result should expose whether private-stack masking participated in the proof. Do not serialize a symbolic expression as though it were a concrete address. Use a structured classification:

```json
{
  "memory_scope": {
    "comparison": "touched-byte-extensional",
    "private_stack": {
      "masking_semantics": "per-implementation-independent-v1",
      "original": {
        "enabled_on_all_terminal_paths": true,
        "disabled_reasons": [],
        "frame_relation": "symbolic-below-entry-sp"
      },
      "candidate": {
        "enabled_on_all_terminal_paths": true,
        "disabled_reasons": [],
        "frame_relation": "symbolic-below-entry-sp"
      }
    }
  }
}
```

If useful, include hashes or normalized SMT renderings for diagnostic expressions, but never make promotion depend on unstable pretty-printed Z3 text.

## Acceptance criteria

- The old implementation fails the new asymmetric-frame tests.
- The corrected implementation passes the full existing suite and new matrix.
- A minimized SAT counterexample is produced for the primary one-sided-write case.
- README/soundness wording is unambiguous.
- The architecture model is bumped.
- Old cached/certified results are rejected after the model bump.

## Appendix — Minimal patch sketch

This is a patch sketch, not a copy-paste guarantee. Agents must adapt names/types to the pinned source revision.

```diff
 def _memory_difference(initial, left, right, ops):
     differences = []
     for address in touched_addresses(left, right):
         initial_byte = z3.Select(initial.memory, address)
-
-        private = z3.Or(
-            _private_stack_address(
-                address,
-                left.stack_low,
-                initial.gpr[1],
-                left.stack_private,
-                ops,
-            ),
-            _private_stack_address(
-                address,
-                right.stack_low,
-                initial.gpr[1],
-                right.stack_private,
-                ops,
-            ),
-        )
-
-        left_byte = z3.If(private, initial_byte, z3.Select(left.memory, address))
-        right_byte = z3.If(private, initial_byte, z3.Select(right.memory, address))
+
+        left_is_private = _private_stack_address(
+            address,
+            left.stack_low,
+            initial.gpr[1],
+            left.stack_private,
+            ops,
+        )
+        right_is_private = _private_stack_address(
+            address,
+            right.stack_low,
+            initial.gpr[1],
+            right.stack_private,
+            ops,
+        )
+
+        # Private storage is an implementation-local abstraction. An address
+        # private to one implementation must not mask a public write by the
+        # other implementation.
+        left_byte = z3.If(
+            left_is_private,
+            initial_byte,
+            z3.Select(left.memory, address),
+        )
+        right_byte = z3.If(
+            right_is_private,
+            initial_byte,
+            z3.Select(right.memory, address),
+        )

         differences.append(left_byte != right_byte)

     return z3.Or(*differences) if differences else z3.BoolVal(False)
```
