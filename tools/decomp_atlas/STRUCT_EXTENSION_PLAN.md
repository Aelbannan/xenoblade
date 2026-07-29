# Decomp Atlas Struct Extension Plan

## Problem Statement

The Decomp Atlas indexes function metadata, call graphs, and attempt history, but has no concept of **struct layouts** or **field definitions**. This means:

1. Agents rediscover struct layouts from scratch each session
2. Field names like `field_0x3F00` persist indefinitely — no propagation from matched functions
3. No shared knowledge base for struct definitions across TUs
4. Duplicate effort when multiple TUs access the same struct (e.g., CVoiceHandle)

## Goal

Extend the Atlas to store, propagate, and serve struct layout information so that:
- Matched functions contribute field names to a shared catalog
- Agents can query "what's at offset 0x3F00 of this struct?" before writing code
- Struct definitions accumulate over time as more functions match
- No LLM calls required — purely deterministic

---

## Design

### 1. New Database Table: `struct_fields`

```sql
CREATE TABLE IF NOT EXISTS struct_fields (
    struct_id TEXT NOT NULL,          -- e.g., "CVoiceHandle", "CVS_THREAD", "CVS_THREAD_Owner"
    offset INTEGER NOT NULL,          -- byte offset from struct base
    width INTEGER NOT NULL,           -- field width in bytes (1, 2, 4, 8)
    field_name TEXT NOT NULL,         -- e.g., "vtable", "field_0x3F00", "readyFlags"
    field_type TEXT,                  -- e.g., "u32", "void*", "CCharVoice*"
    source TEXT NOT NULL,             -- "inferred", "matched_function", "manual", "dwarf"
    confidence REAL DEFAULT 1.0,      -- 0.0-1.0 confidence score
    created_at TEXT NOT NULL,
    updated_at TEXT NOT NULL,
    PRIMARY KEY (struct_id, offset)
);

CREATE TABLE IF NOT EXISTS struct_references (
    struct_id TEXT NOT NULL,
    target_id TEXT NOT NULL,          -- function that references this struct
    offset INTEGER NOT NULL,          -- which field it accesses
    access_type TEXT NOT NULL,        -- "load", "store", "both"
    register TEXT,                    -- base register (r3, r31, etc.)
    created_at TEXT NOT NULL,
    PRIMARY KEY (struct_id, target_id, offset)
);
```

### 2. New Table: `struct_layouts`

```sql
CREATE TABLE IF NOT EXISTS struct_layouts (
    struct_id TEXT PRIMARY KEY,
    size INTEGER,                     -- total struct size in bytes
    parent_id TEXT,                   -- parent struct (for inheritance)
    description TEXT,
    source TEXT NOT NULL,             -- "inferred", "manual", "dwarf"
    confidence REAL DEFAULT 1.0,
    created_at TEXT NOT NULL,
    updated_at TEXT NOT NULL
);
```

### 3. Indexer Extension

Add to `tools/decomp_atlas/lib/indexer.py`:

```python
def _index_struct_info(conn, root: Path, region: str) -> int:
    """Extract struct information from matched functions and inferred layouts."""
    
    # 1. Load existing struct catalog from field_catalog.json
    catalog_path = root / "tools" / "pi_harness" / "field_catalog.json"
    if catalog_path.exists():
        catalog = json.loads(catalog_path.read_text())
        for struct_id, fields in catalog.items():
            for offset, info in fields.items():
                _insert_struct_field(conn, struct_id, int(offset), info)
    
    # 2. Extract field accesses from matched functions
    #    (functions with status FULL_MATCH or EQUIVALENT_MATCH)
    matched = conn.execute(
        "SELECT target_id, unit, symbol FROM functions WHERE status IN ('FULL_MATCH', 'EQUIVALENT_MATCH')"
    ).fetchall()
    
    for target_id, unit, symbol in matched:
        # Extract retail ASM and decode instructions
        # Identify field accesses: lwz/stw rX, N(rY)
        # Group by base register to infer struct identity
        # Store in struct_references table
        pass
    
    # 3. Run struct deduction (same algorithm as type_prepass.py)
    #    but store results in struct_fields table
    pass
```

### 4. API Endpoints

Add to `tools/decomp_atlas/lib/server.py`:

```python
@app.route("/api/structs")
def list_structs():
    """List all known structs."""
    # Query struct_layouts table
    pass

@app.route("/api/structs/<struct_id>")
def get_struct(struct_id: str):
    """Get struct layout with all fields."""
    # Query struct_fields + struct_layouts
    pass

@app.route("/api/structs/lookup")
def lookup_field():
    """Look up a field by offset.
    
    Query params: ?struct=CVoiceHandle&offset=0x3F00
    Returns: { struct_id, offset, field_name, field_type, confidence, sources }
    """
    pass

@app.route("/api/structs/by-function/<target_id>")
def get_structs_for_function(target_id: str):
    """Get all structs referenced by a function."""
    # Query struct_references where target_id = ?
    pass

@app.route("/api/structs/contribute", methods=["POST"])
def contribute_field():
    """Submit a field name from a matched function.
    
    Body: {
        "struct_id": "CVoiceHandle",
        "offset": 16128,
        "field_name": "readyFlags",
        "field_type": "u32",
        "source": "matched_function",
        "target_id": "us-802a8540"
    }
    """
    pass
```

### 5. Field Catalog File

Create `tools/pi_harness/field_catalog.json` as a human-editable seed:

```json
{
  "CVoiceHandle": {
    "0x00": { "name": "vtable", "type": "void**", "confidence": 1.0 },
    "0x04": { "name": "field_0x04", "type": "void*", "confidence": 0.8 },
    "0x3E9C": { "name": "voice", "type": "CCharVoice", "confidence": 1.0 },
    "0x3F00": { "name": "readyFlags", "type": "u32", "confidence": 0.9 }
  },
  "CVS_THREAD": {
    "0x00": { "name": "unk0", "type": "u32", "confidence": 1.0 },
    "0x04": { "name": "unk4", "type": "u32", "confidence": 1.0 },
    "0x1C": { "name": "vtable", "type": "void**", "confidence": 1.0 },
    "0x20": { "name": "field_0x20", "type": "CVoiceHandle*", "confidence": 0.8 },
    "0x24": { "name": "field_0x24", "type": "CVoiceHandle*", "confidence": 0.8 }
  }
}
```

### 6. Integration with pi-harness

Modify `tools/pi_harness/src/orchestrator.ts`:

```typescript
async function runOneTu(repoRoot, unit, config, modelRuntime, dryRun) {
  // ... existing code ...
  
  // Query Atlas for known struct info
  const structInfo = await queryAtlasStructs(repoRoot, unit, targets);
  if (structInfo.length > 0) {
    console.log(`[pi-harness] ${unit}: found ${structInfo.length} struct references from Atlas`);
    // Inject into brief
    brief += buildStructContext(structInfo);
  }
  
  // ... continue with matching session ...
}

// After successful match, contribute field names to Atlas
async function contributeFieldNames(repoRoot, unit, targetId, matchedCode) {
  // Extract field accesses from the matched decompiled code
  // Submit to Atlas API
}
```

### 7. Propagation from Matched Functions

When a function matches (FULL_MATCH or EQUIVALENT_MATCH):

1. **Extract field accesses** from the decompiled source code
   - Parse `ptr->field_0xNN` patterns
   - Parse `struct.field_0xNN` patterns
   - Map to struct identity based on the pointer type

2. **Submit to Atlas** via `/api/structs/contribute`
   - Include the target_id (function that matched)
   - Include the access type (load/store)
   - Include the confidence (1.0 for matched functions)

3. **Update field catalog** if a semantic name is available
   - If the decompiled code uses `ptr->readyFlags` instead of `ptr->field_0x3F00`
   - Update the catalog with the semantic name

### 8. Struct Identity Resolution

The hardest problem: how do we know that `r31` in function A and `r30` in function B point to the same struct?

**Approaches**:
1. **Type annotations from decompiled code**: If function A uses `CVoiceHandle* ptr` and accesses offset 0x3F00, we know that offset belongs to CVoiceHandle.
2. **Allocation size**: If `func_802A34E4(0x28)` is called, the result is a 0x28-byte struct.
3. **Parent-child relationships**: If a function takes `CVS_THREAD*` as first param, and accesses offset 0x20, that field belongs to CVS_THREAD (or a derived class).
4. **Field overlap**: If two struct groups share 50%+ of their offsets, they're likely the same struct.

**Initial implementation**: Use type annotations from decompiled code (most reliable). Fall back to field overlap for inferred structs.

---

## Implementation Phases

### Phase 1: Database Schema + API (2-3 days)

- [ ] Add `struct_fields`, `struct_references`, `struct_layouts` tables
- [ ] Add API endpoints: `/api/structs`, `/api/structs/<id>`, `/api/structs/lookup`
- [ ] Add `/api/structs/contribute` endpoint
- [ ] Create `field_catalog.json` seed file

### Phase 2: Indexer Extension (2-3 days)

- [ ] Load `field_catalog.json` into struct_fields on index
- [ ] Extract field accesses from matched functions
- [ ] Run struct deduction algorithm (same as type_prepass.py)
- [ ] Store results in struct_fields table

### Phase 3: pi-harness Integration (2-3 days)

- [ ] Query Atlas for struct info before matching sessions
- [ ] Inject struct context into agent briefs
- [ ] Contribute field names after successful matches
- [ ] Update field_catalog.json with new names

### Phase 4: Propagation + Deduplication (1 week)

- [ ] Implement struct identity resolution
- [ ] Handle conflicting field names (merge strategy)
- [ ] Add confidence scoring based on source
- [ ] Add manual override capability

---

## Expected Impact

| Metric | Before | After |
|--------|--------|-------|
| Struct knowledge per TU | None (rediscovered each time) | Shared across all TUs |
| Field name propagation | Manual only | Automatic from matched functions |
| Agent time on struct discovery | 10+ min per session | 0 min (pre-loaded) |
| Header consistency | 4 different CVoiceHandle defs | 1 shared definition |

**Cost**: Zero LLM calls — purely deterministic database operations.

**Time to value**: 
- Phase 1-2: 1 week → agents can query struct info
- Phase 3: 2 weeks → agents get struct context in briefs
- Phase 4: 3 weeks → full propagation and deduplication

---

## Risks and Mitigations

### Risk 1: Struct identity resolution is hard
**Mitigation**: Start with type annotations from decompiled code (most reliable). Fall back to field overlap for inferred structs. Accept some false positives — they can be corrected manually.

### Risk 2: Stale struct info
**Mitigation**: Include `source` and `confidence` fields. Manual overrides take precedence. Include `updated_at` for staleness detection.

### Risk 3: Conflicting field names
**Mitigation**: Use confidence scoring. Matched functions (confidence=1.0) override inferred (confidence=0.5). Manual overrides (confidence=1.0) override everything.

### Risk 4: Performance on large TUs
**Mitigation**: Cache struct queries. Index on (struct_id, offset). Limit queries to relevant structs only.

---

## Comparison with Alternatives

| Approach | Accuracy | Cost | Speed | Maintenance |
|----------|----------|------|-------|-------------|
| **Atlas extension** | High (grows over time) | Zero | Fast (SQLite queries) | Low (automatic) |
| Rule-based prepass | Medium (60-70%) | Zero | Fast (<1s) | Medium (needs tuning) |
| DWARF parsing | High (ground truth) | Zero | Medium (build required) | Low (but build must work) |
| LLM inference | Low (hallucination risk) | $0.05-0.15/TU | Slow (30s) | High (prompts, validation) |

**Recommendation**: Atlas extension is the best long-term solution. Rule-based prepass is the best short-term win. DWARF is blocked by build issues. LLM is not worth the cost.

---

## Open Questions

1. Should struct_fields be region-specific? (Probably not — struct layouts are the same across regions)
2. How to handle struct inheritance? (Parent-child in struct_layouts, or flatten?)
3. Should we store bitfield information? (Yes, but as a separate table or JSON field)
4. How to handle unions? (Multiple fields at the same offset with different widths)
5. Should the Atlas serve as the source of truth for headers, or just a reference? (Reference — headers are the source of truth)

---

## Appendix: Proof of Concept

The existing `tools/pi_harness/type_prepass.py` demonstrates that:

1. Field accesses can be extracted from retail ASM (23 accesses in CVS_THREAD_DOWN)
2. Struct layouts can be inferred (4 structs identified across 5 TUs)
3. Common structs exist (CVoiceHandle at 0x3F04, CVS_THREAD at 0x30)

The Atlas extension would store this information persistently and make it available to all agents.
