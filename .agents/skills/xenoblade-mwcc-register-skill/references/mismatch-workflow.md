# Register-mismatch workflow (Xenoblade / Wii 1.1)

## Triage

1. Fingerprint the compiler and confirm the TU's flags
   (`SKILL.md` §0, `references/xenoblade-profile.md`). Check for
   `-O4,s` overrides on the object.
2. Locate the first meaningful assembly divergence in objdiff.
3. Decide whether the divergence is only a physical-register
   permutation. If opcodes, loads, branches, or instruction order
   differ, inspect types, expression shape, optimization level, and
   scheduling before blaming coloring.
4. Draw live ranges around the divergence and mark calls, ABI
   locations, stack homes, and special-register operations.
5. Identify values that can coalesce or reuse a color because they do
   not overlap (union-find parent map, §2 of allocator rules).
6. Apply driver → simplify → spill-ranking → select order
   (`references/allocator-rules.md`, `scripts/regalloc_explain.py`);
   consider spill pressure only once a 19-value (GPR/FPR) or 13-value
   (VR) call-crossing clique is plausible.
7. Test one minimal source rewrite and verify with objdiff.

## Evidence hierarchy

Prefer, in order:

1. Output from the exact Wii/1.1 compiler executable and game flags.
2. Recovered Wii 1.1 logic for class tables, simplify, spill ranking,
   and coloring (addresses in `references/allocator-rules.md`).
3. Controlled paired probes that alter one variable.
4. Patterns from neighboring matched Xenoblade functions.
5. GC-era allocator folklore only as a hypothesis to test — never as
   evidence. Source-order folklore last of all.

## Source rewrites worth testing

- Swap independent expression subtrees when destructive target choice
  differs.
- Reorder locals only if both survive optimization and their live
  ranges overlap.
- Change initialization placement to alter a branch join or live-range
  boundary.
- Introduce or remove a semantically redundant temporary to test
  coalescing.
- Match signedness and width to the target opcode before tuning
  allocation.
- Use `volatile` only when the target genuinely has a stack home and
  memory traffic.

Do not retain non-idiomatic casts, hardcoded offsets, fake volatility,
or dead operations unless they reflect the original program and remain
acceptable upstream.

## Verification

A successful `ninja` build is insufficient because unlinked objects can
still mismatch. Rebuild and require `100.0` in objdiff for the target
object with no regressions elsewhere. Note which game version the
object is `MatchingFor` (`jp`/`eu`/`us`) — a fix must hold for every
version the object claims.
