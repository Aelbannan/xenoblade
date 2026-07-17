# P2-04 — Add solver diagnostics and query audit artifacts

**Priority:** P2

---

## Goal

When performance or correctness is questioned, reviewers need visibility into the actual query.

## Optional debug artifacts

- SMT-LIB2 query dump;
- normalized assertion count by category;
- bitvector/array/function symbol counts;
- path-pair count;
- memory-touch count;
- relocation/layout constraint count;
- solver statistics;
- phase timings;
- reason for `unknown`.

## Safety

- disabled by default for normal runs;
- deterministic naming by request hash;
- no cache acceptance based on debug files;
- queries may be large, so apply size limits/compression.

## Differential query checker

For small generated cases, cross-check with a second formulation where possible, such as full finite-memory concretization or exhaustive enumeration over reduced bit widths/test semantics. This is not a replacement for the production query but can detect construction mistakes.
