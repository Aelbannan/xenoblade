# Phase 2 — Staged Generation and Compile Closure

**Source:** Section 10 of the LLM Decompilation Improvement Plan  
**Covers:** Reconstruction strategies, response schema, compile diagnostic normalization, diagnostic categories, root cause, compile-repair prompt, repair loop, branch selection, early compile success  
**PR:** 3 — Compile diagnostic classifier and repair loop

---

## 10.1 Initial reconstruction strategies

Run three candidates in parallel with explicit strategy labels.

### Strategy A: literal operations

Prompt emphasis:

- Preserve observed calls
- Preserve memory offsets and widths
- Preserve branch count and return paths
- Prefer direct, simple C++
- Avoid creative abstraction

### Strategy B: type-aware idiomatic source

Prompt emphasis:

- Use known members and enums
- Follow accepted sibling conventions
- Use repository-compatible types
- Avoid casts unless required by observed widths

### Strategy C: alternative control-flow hypothesis

Prompt emphasis:

- Explore a distinct but evidence-supported high-level CFG
- Consider early return versus nested branch
- Consider loop forms only if backedges exist
- Do not change observable effects

Record the strategy in candidate lineage.

## 10.2 Reconstruction response schema

```json
{
  "response_schema_version": 2,
  "stage": "reconstruct",
  "source": "bool SomeClass::func(...) { ... }",
  "semantic_summary": {
    "calls": ["SomeNamespace::callee"],
    "memory_reads": [
      {"base": "this", "offset": 33, "width_bits": 8}
    ],
    "memory_writes": [],
    "return_paths": [
      {"condition": "mState == 0", "value": "false"},
      {"condition": "otherwise", "value": "result"}
    ],
    "constants": [0]
  },
  "assumptions": [
    "mState corresponds to the byte read at this+0x21"
  ],
  "blocked_on": [],
  "hypothesis": "The function checks state before delegating to callee.",
  "confidence": 0.72
}
```

The parser must reject:

- Missing source
- Multiple unrelated functions
- Signature changes
- Markdown outside the allowed JSON envelope, if strict mode is enabled
- Unsupported external file edits

## 10.3 Compile diagnostic normalization

Compiler output changes across path names and generated worktrees. Normalize it before fingerprinting.

Normalization steps:

1. Replace workspace roots with `<WORKSPACE>`.
2. Replace experiment IDs with `<EXPERIMENT>`.
3. Normalize slash direction.
4. Remove caret lines and repeated context when unnecessary.
5. Preserve compiler error code if present.
6. Extract file, line, column, symbol, and main message.
7. Remove unstable numeric addresses unless relevant.

Example:

```text
/tmp/worktree-123/src/foo.cpp:91: error: 'mFoo' was not declared
```

becomes:

```json
{
  "category": "unknown_identifier",
  "file": "src/foo.cpp",
  "line": 91,
  "symbol": "mFoo",
  "message": "'mFoo' was not declared",
  "fingerprint": "unknown_identifier:mFoo"
}
```

## 10.4 Diagnostic categories

Implement ordered matching:

```python
DIAGNOSTIC_RULES = [
    DiagnosticRule(
        category="syntax_error",
        patterns=[r"parse error", r"expected .+ before"],
    ),
    DiagnosticRule(
        category="unknown_identifier",
        patterns=[r"'(?P<symbol>[^']+)' was not declared"],
    ),
    DiagnosticRule(
        category="unknown_member",
        patterns=[r"has no member named '(?P<symbol>[^']+)'"],
    ),
    DiagnosticRule(
        category="argument_count",
        patterns=[r"too (?:few|many) arguments"],
    ),
    DiagnosticRule(
        category="invalid_conversion",
        patterns=[r"cannot convert", r"invalid conversion"],
    ),
    DiagnosticRule(
        category="incomplete_type",
        patterns=[r"incomplete type"],
    ),
    DiagnosticRule(
        category="access_control",
        patterns=[r"is private", r"is protected"],
    ),
    DiagnosticRule(
        category="linkage_error",
        patterns=[r"undefined reference", r"unresolved external"],
    ),
]
```

Add MWCC-specific fixture samples gathered from real experiments.

## 10.5 Root diagnostic

The repair prompt should focus on the first causal diagnostic, not every cascading error.

Heuristics:

- Prefer first syntax error before later parse cascades.
- Prefer unknown type before member errors caused by that type.
- Prefer declaration mismatch before argument conversion cascades.
- Prefer compiler errors over linker errors when both exist.
- Group duplicate messages.

## 10.6 Compile-repair prompt

Create `prompts/compile_repair.md`.

Required content:

```markdown
You are repairing one candidate function so that it compiles.

Preserve:
- The exact target signature
- Intended calls
- Memory access widths and offsets
- Constants
- Branch and return behavior

Change only what is required by the compiler diagnostics.

Do not:
- Perform instruction matching yet
- Invent members, functions, types, or globals
- Modify headers or unrelated functions
- Remove behavior merely to silence an error

When the necessary declaration is unavailable, return an appropriate
blocked reason instead of fabricating code.
```

Input sections:

- Dossier
- Candidate source
- Compiler diagnostic report
- Parent semantic summary
- Prior repair fingerprints
- Allowed symbol inventory

## 10.7 Repair-loop algorithm

```python
def run_compile_repairs(
    branch: CandidateBranch,
    budget: int,
) -> CandidateBranch:
    seen_fingerprints: set[str] = set()

    for repair_index in range(1, budget + 1):
        report = branch.latest.evaluation.compile_report

        if report.succeeded:
            return branch

        fingerprint = report.root_fingerprint
        if fingerprint in seen_fingerprints:
            branch.block(
                reason=BlockedReason.REPEATED_DIAGNOSTIC,
                detail=fingerprint,
            )
            return branch

        seen_fingerprints.add(fingerprint)

        response = call_model(
            stage=PipelineStage.COMPILE_REPAIR,
            dossier=branch.dossier,
            parent=branch.latest,
            diagnostic_report=report,
            repair_index=repair_index,
        )

        candidate = evaluate_response(response)
        branch.append(candidate)

    return branch
```

A repeated fingerprint should normally stop that branch, but not necessarily the entire target if another branch remains.

## 10.8 Selecting branches for repair

After initial evaluation, rank compile failures by repairability.

Suggested tuple:

```python
def compile_repair_priority(candidate: CandidateRecord) -> tuple:
    report = candidate.evaluation.compile_report
    return (
        int(candidate.response_parsed),
        int(candidate.signature_preserved),
        repairability_tier(report),
        -len(report.diagnostics),
        int(report.root_fingerprint not in candidate.ancestor_fingerprints),
        candidate.model_confidence,
    )
```

Repair the best branch first. If it becomes blocked and budget remains, repair the second-best branch.

## 10.9 Early compile success

When one branch compiles:

- Do not automatically cancel already-running initial calls; preserve their evidence.
- Do not spend additional compile-repair budget on that branch.
- Continue to structural analysis.
- Consider another compiling initial branch if it has a better preliminary match.
