# Type Prepass System — 3-Phase Design

## Problem Statement

The pi-harness matching sessions waste significant time rediscovering struct layouts. Each agent independently figures out:
- CVoiceHandle has a CCharVoice at offset 0x3E9C
- The owner parameter has a flags field at offset 0x3F00
- The vtable pointer is at offset 0x1C in CVS_THREAD

This leads to:
1. **Lint violations** — agents write `((u32*)owner)[0x3F00/4]` instead of `owner->field_0x3F00`
2. **Wrong types** — constructors take `CVS_THREAD_DOWN*` when they should take a game entity pointer
3. **Inconsistent headers** — CVoiceHandle is defined 4 different ways across 5 TUs
4. **Wasted iterations** — agents spend 10+ minutes discovering struct layouts before writing matching code

## Solution Overview

A 3-phase prepass system that runs before matching sessions:

```
Phase 1: Rule-based ASM analysis (fast, mechanical)
    ↓
Phase 2: Symbol recovery integration (rename placeholder types)
    ↓
Phase 3: LLM-assisted type inference (semantic naming, complex layouts)
```

Each phase feeds into the next, progressively enriching type information.

---

## Phase 1: Rule-Based ASM Analysis

**Goal**: Extract field accesses from retail ASM and generate struct definitions.

**Speed**: <1 second per TU  
**Cost**: Zero (no LLM calls)  
**Accuracy**: 95% for simple field accesses

### Implementation

**Location**: `tools/pi_harness/type_prepass.py` (proof of concept exists)

**Algorithm**:
1. Parse retail ASM from `build/<region>/asm/<unit>.s`
2. Extract field accesses: `lwz/stw/lbz/lhz/stb/sth rX, N(rY)`
3. Track register aliases: `mr rX, rY`
4. Group accesses by (function, base_register)
5. Merge groups with overlapping offset sets (50% threshold)
6. Generate struct definitions with `field_0xNN` names

**Output**: Struct definitions with:
- Field offsets and widths
- Access type (load/store/both)
- Which functions use each field

### Example Output

```
Inferred Struct: OwnerObject (size: 0x3F04)
  0x3F00  u32  field_0x3F00  (load)  used in: __ct__802A5B88, __ct__802A6E84, ...

Inferred Struct: CVS_THREAD_Derived (size: 0x30)
  0x001C  u32  field_0x001C  (both)  used in: __ct__802A5B88, func_802A5C90, ...
  0x0020  u32  field_0x0020  (both)  used in: __ct__802A5B88, func_802A5C90, ...
  0x0024  u32  field_0x0024  (both)  used in: __ct__802A5B88, func_802A5D4C, ...
  0x002C  u32  field_0x002C  (load)  used in: __ct__802A5B88
```

### Integration Point

Insert into `runOneTu()` in orchestrator.ts, before `buildBriefs()`:

```typescript
// Phase 1: Rule-based ASM analysis
if (config.enableTypePrepass) {
  const prepassResult = await runTypePrepass(repoRoot, unit, targets);
  if (prepassResult.structs.length > 0) {
    await applyStructDefinitions(repoRoot, unit, prepassResult.structs);
  }
}
```

---

## Phase 2: Symbol Recovery Integration

**Goal**: Rename placeholder types using existing symrecover tooling.

**Speed**: ~5 seconds per TU  
**Cost**: Zero (no LLM calls)  
**Accuracy**: 99% for known types (RTTI/mangling-based)

### Implementation

**Location**: Extend `tools/symrecover.py` with struct-aware renaming

**Algorithm**:
1. Load Phase 1 inferred structs
2. For each struct, check if it matches a known type in the symbol catalog
3. If match found, apply rename (e.g., `UnknownStruct_0` → `CVoiceHandle`)
4. Update headers with semantic names

**Integration with Phase 1**:
```python
# Phase 1 output
structs = analyze_unit(repoRoot, region, unit)

# Phase 2: Symbol recovery
for struct in structs:
    # Check if this struct matches a known type
    known_type = find_matching_type(repoRoot, struct)
    if known_type:
        struct.name = known_type.name
        # Rename fields based on known type's field names
        for field in struct.fields:
            if field.offset in known_type.fields:
                field.name = known_type.fields[field.offset].name
```

### Example Output

```
Before Phase 2:
  UnknownStruct_0 (size: 0x3F04)
    0x3F00  field_0x3F00

After Phase 2:
  CVoiceHandle (size: 0x3F04)
    0x3F00  readyFlags
```

### Integration Point

Runs immediately after Phase 1, before applying changes:

```typescript
// Phase 2: Symbol recovery
const renamed = await applySymbolRecovery(repoRoot, unit, prepassResult.structs);
await applyStructDefinitions(repoRoot, unit, renamed);
```

---

## Phase 3: LLM-Assisted Type Inference

**Goal**: Semantic naming for fields that Phase 1-2 couldn't resolve.

**Speed**: ~30 seconds per TU  
**Cost**: ~$0.01-0.05 per TU (cheap model)  
**Accuracy**: 90% for semantic naming

### When to Use

Phase 3 is **optional** and only runs when:
1. Phase 1 found fields that couldn't be named semantically
2. Phase 2 didn't find matching types in the symbol catalog
3. The TU has complex structs that would benefit from semantic naming

### Implementation

**Location**: New session type in `tools/llm_decomp/` or extend pi-harness

**Algorithm**:
1. Collect Phase 1-2 output (struct definitions with `field_0xNN` names)
2. For each unnamed field, gather context:
   - Which functions access it
   - How it's used (load/store, null checks, bit tests)
   - Nearby named fields
3. Send to LLM with prompt: "Suggest semantic names for these fields based on usage patterns"
4. Apply LLM suggestions (with validation)

**Prompt Template**:
```markdown
## Struct: CVS_THREAD_Derived (size: 0x30)

### Fields to name:
- 0x001C: accessed in 5 functions, used as vtable pointer (lwz r12, 0x1C(r31); lwz r12, 0x8(r12); mtctr r12; bctrl)
- 0x002C: accessed in 1 function, loaded after field_0x0020 (lwz r0, 0x2C(r31))

### Context:
- This is a CVS_THREAD-derived class for voice playback
- field_0x0020 is a CVoiceHandle* pointer
- field_0x0024 is a CVoiceHandle* pointer (second slot)

### Suggest semantic names for the unnamed fields.
```

### Example Output

```
Before Phase 3:
  0x001C  field_0x001C  (vtable pointer)
  0x002C  field_0x002C  (unknown)

After Phase 3:
  0x001C  vtable       (vtable pointer - confirmed by usage pattern)
  0x002C  bufferHandle (loaded after voice handles - likely buffer pointer)
```

### Integration Point

Phase 3 runs as a separate session after Phase 1-2, only if needed:

```typescript
// Phase 3: LLM-assisted naming (optional)
if (config.enableLLMTypeInference && hasUnnamedFields(prepassResult)) {
  const named = await runLLMTypeInference(repoRoot, unit, prepassResult);
  await applyStructDefinitions(repoRoot, unit, named);
}
```

---

## Configuration

Add to `HarnessConfig`:

```typescript
interface HarnessConfig {
  // ... existing fields ...
  
  /** Enable type prepass before matching sessions. */
  enableTypePrepass: boolean;
  
  /** Enable Phase 2 symbol recovery integration. */
  enableSymbolRecovery: boolean;
  
  /** Enable Phase 3 LLM-assisted type inference. */
  enableLLMTypeInference: boolean;
  
  /** Model for Phase 3 (cheap model recommended). */
  typeInferenceModel?: ModelSpec;
  
  /** Maximum number of fields to infer per struct (safety limit). */
  maxFieldsPerStruct: number;
}
```

**Recommended defaults**:
```json
{
  "enableTypePrepass": true,
  "enableSymbolRecovery": true,
  "enableLLMTypeInference": false,
  "typeInferenceModel": {
    "provider": "openrouter",
    "model": "deepseek/deepseek-v4-flash",
    "thinkingLevel": "low"
  },
  "maxFieldsPerStruct": 50
}
```

---

## Expected Impact

| Metric | Before | After Phase 1 | After Phase 1-2 | After Phase 1-3 |
|--------|--------|---------------|-----------------|-----------------|
| Time to first match (simple) | ~15 min | ~15 min | ~15 min | ~15 min |
| Time to first match (constructors) | Never | ~25 min | ~20 min | ~18 min |
| Lint violations per session | 5-10 | 1-2 | 0-1 | 0 |
| Iterations to match constructor | N/A | 10-15 | 8-12 | 6-10 |
| Header consistency | 4 different defs | 1 shared def | 1 shared def | 1 shared def |

**Cost per TU**:
- Phase 1: $0 (rule-based)
- Phase 2: $0 (symbol recovery)
- Phase 3: $0.01-0.05 (LLM, optional)

**Time per TU**:
- Phase 1: <1 second
- Phase 2: ~5 seconds
- Phase 3: ~30 seconds (if enabled)

---

## Implementation Order

### Week 1: Phase 1 (Rule-Based)
- [x] Proof of concept: `tools/pi_harness/type_prepass.py`
- [ ] Integrate into orchestrator
- [ ] Generate shared CVoiceHandle header
- [ ] Test on CVS_THREAD_* TUs

### Week 2: Phase 2 (Symbol Recovery)
- [ ] Extend symrecover with struct-aware renaming
- [ ] Map inferred structs to known types
- [ ] Update headers with semantic names

### Week 3: Phase 3 (LLM-Assisted)
- [ ] Design prompt template for field naming
- [ ] Implement LLM session for type inference
- [ ] Add validation (LLM suggestions must not break matches)
- [ ] Test on complex TUs

### Week 4: Integration & Testing
- [ ] Full integration into pi-harness
- [ ] Test on 10+ TUs
- [ ] Measure impact on matching success rate
- [ ] Document usage

---

## Risks and Mitigations

### Risk 1: Inferred struct boundaries are wrong
**Mitigation**: Use 50% overlap threshold for merging; allow manual override via config.

### Risk 2: Phase 2 renames break existing matches
**Mitigation**: Only rename fields that aren't used in matched functions; snapshot before changes.

### Risk 3: Phase 3 LLM suggests wrong names
**Mitigation**: Validate suggestions by checking if renamed code still compiles; revert on failure.

### Risk 4: Performance impact on large TUs
**Mitigation**: Phase 1 is O(n) where n = number of instructions; Phase 2-3 are optional.

---

## Open Questions

1. Should Phase 1 output be cached between runs? (Probably yes, keyed on ASM hash)
2. Should Phase 2 use the existing symrecover API or a new interface?
3. Should Phase 3 be a separate session type or integrated into match sessions?
4. How to handle TUs with no retail ASM? (Skip prepass, fall back to agent discovery)
5. Should the prepass suggest struct sizes for allocation functions? (e.g., `func_802A34E4(0x28)` → allocate CVS_THREAD_DOWN)

---

## Appendix: Proof of Concept Results

Run on CVS_THREAD_* TUs (2026-07-28):

```
Common structs across TUs:
  Size 0x2C (44 bytes): fields 0x0020, 0x0024, 0x0028 — ALL 5 TUs
  Size 0x30 (48 bytes): fields 0x001C, 0x0020, 0x0024, 0x002C — DOWN, HAGE, VISION_BREAK, VISION_TELL
  Size 0x2C0 (704 bytes): field 0x02BC — ALL 5 TUs
  Size 0x3F04 (16132 bytes): field 0x3F00 — DOWN, HAGE, TENSION_UP, VISION_BREAK
```

**Key finding**: All 5 TUs access the same struct layouts, confirming that a shared header would work.
