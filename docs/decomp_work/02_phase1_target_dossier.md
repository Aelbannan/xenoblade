# Phase 1 — Target Dossier

**Source:** Section 9 of the LLM Decompilation Improvement Plan  
**Covers:** Dossier schema, target identity, signature, decoded retail instructions, CFG, data-flow summary, declarations, caller snippets, accepted siblings, symbol inventory, size limits, validation  
**PR:** 2 — Typed dossier and deterministic assembly context

---

## 9.1 Dossier schema

```python
@dataclass
class TargetDossier:
    schema_version: int
    target: TargetIdentity
    signature: SignatureContext
    retail: RetailProgramContext
    source: SourceContext
    calls: CallContext
    types: TypeContext
    symbols: SymbolInventory
    accepted_examples: list[AcceptedExample]
    knowledge: list[KnowledgeRecord]
    prior_attempt_summary: list[AttemptCluster]
    constraints: PromptConstraints
```

Serialize it to both:

- `dossier.json` for machine consumption
- `DOSSIER.md` for model readability and debugging

## 9.2 Target identity

```json
{
  "target_id": "SomeClass__func",
  "mangled_name": "...",
  "demangled_name": "SomeClass::func(int)",
  "source_file": "src/...",
  "object_file": "build/...",
  "translation_unit": "...",
  "retail_address": "0x80123450",
  "retail_size": 96
}
```

## 9.3 Exact signature

Include:

- Return type
- Function name
- Parameter names where known
- Parameter types
- Qualifiers
- Namespace/class scope
- Calling convention assumptions
- Whether the signature is authoritative

Example:

```json
{
  "declaration": "bool SomeClass::func(const Item& item, int index)",
  "authoritative": true,
  "implicit_this": {
    "type": "SomeClass*",
    "register": "r3"
  },
  "parameters": [
    {"name": "item", "type": "const Item&", "register": "r4"},
    {"name": "index", "type": "int", "register": "r5"}
  ],
  "return": {
    "type": "bool",
    "register": "r3"
  }
}
```

## 9.4 Decoded retail instructions

Use a stable format:

```json
{
  "address": "0x80123450",
  "offset": 0,
  "raw_word": "9421fff0",
  "mnemonic": "stwu",
  "operands": "r1, -0x10(r1)",
  "instruction_class": "store",
  "basic_block": "B0",
  "branch_target": null,
  "relocation": null
}
```

For relocated calls:

```json
{
  "address": "0x80123468",
  "offset": 24,
  "raw_word": "48000001",
  "mnemonic": "bl",
  "operands": "SomeNamespace::callee",
  "instruction_class": "call",
  "basic_block": "B1",
  "branch_target": null,
  "relocation": {
    "type": "R_PPC_REL24",
    "symbol": "SomeNamespace::callee",
    "addend": 0
  }
}
```

## 9.5 Control-flow graph

At minimum include:

- Entry block
- Basic blocks
- Instruction ranges
- Successors
- Predecessors
- Branch condition when recognized
- Exit blocks
- Backedges
- Loop headers
- Unreachable regions if any

```json
{
  "entry": "B0",
  "blocks": [
    {
      "id": "B0",
      "start": "0x80123450",
      "end": "0x80123464",
      "successors": ["B1", "B3"],
      "predecessors": [],
      "terminator": {
        "kind": "conditional",
        "condition_register": "cr0",
        "taken": "B3",
        "fallthrough": "B1"
      }
    }
  ],
  "exit_blocks": ["B3"],
  "backedges": []
}
```

## 9.6 Data-flow summary

Initial implementation can be conservative.

Include:

- Likely argument register use
- Return register writes
- Stack frame size
- Saved/restored registers
- Observed loads and stores
- Referenced offsets
- Direct calls
- Constants
- Global symbols
- Condition register usage

Do not claim high-level semantics not proven by deterministic analysis.

```json
{
  "stack_frame_size": 16,
  "saved_registers": ["lr", "r31"],
  "memory_accesses": [
    {
      "kind": "load",
      "width_bits": 8,
      "base": "r31",
      "offset": 33,
      "likely_expression": "this+0x21"
    }
  ],
  "constants": ["0x20", "0xFF"],
  "calls": ["SomeNamespace::callee"]
}
```

## 9.7 Relevant declarations

Build a bounded declaration extractor.

Start from:

- Target function declaration
- Containing class declaration
- Types appearing in target signature
- Types appearing in direct callee declarations
- Symbols referenced by relocations
- Types used in selected caller snippets
- Explicit project metadata hints

Cap recursively included declarations by:

- Depth
- Character count
- Number of types
- Relevance

Avoid inserting implementation-heavy headers wholesale.

## 9.8 Caller snippets

Select two or three representative callsites.

Prioritize:

1. Callers in the same subsystem
2. Callers with clearly typed arguments
3. Callers that inspect the return value
4. Accepted callers
5. Small snippets with minimal unrelated code

Each snippet should contain enough surrounding source to show argument meaning without exposing an entire file.

## 9.9 Accepted sibling functions

Wire the existing accepted-context helper into dossier construction.

Each example should include:

```json
{
  "target_id": "...",
  "demangled_name": "...",
  "relationship": "same_class",
  "reason_selected": [
    "same class",
    "similar call relocation",
    "same field offsets"
  ],
  "source": "...",
  "retail_summary": {
    "size": 64,
    "calls": ["..."],
    "cfg_shape": "single conditional"
  }
}
```

Default maximum: four.

## 9.10 Allowed symbol inventory

The prompt should distinguish known symbols from model guesses.

```json
{
  "members": [
    {
      "name": "mState",
      "type": "u8",
      "offset": 33,
      "source": "class declaration"
    }
  ],
  "functions": [
    {
      "name": "setState",
      "declaration": "void setState(State state)",
      "source": "header"
    }
  ],
  "globals": [],
  "enums": []
}
```

Prompt rule:

> Use only symbols and declarations in this dossier. If the retail behavior cannot be represented using the available declarations, return a blocked reason rather than inventing a member or function.

## 9.11 Context size limits

Suggested defaults:

```json
{
  "dossier": {
    "max_decoded_instructions": 400,
    "max_declaration_chars": 20000,
    "max_callers": 3,
    "max_caller_chars_each": 3000,
    "max_accepted_siblings": 4,
    "max_accepted_chars_each": 5000,
    "max_knowledge_records": 10,
    "max_attempt_clusters": 8,
    "include_raw_hex": true
  }
}
```

Large functions above instruction limits should be explicitly marked truncated and may require a different workflow.

## 9.12 Dossier validation

Before any model call:

```python
def validate_dossier(d: TargetDossier) -> list[str]:
    errors = []

    if not d.signature.declaration:
        errors.append("missing target declaration")

    if not d.retail.instructions:
        errors.append("missing decoded retail instructions")

    if d.signature.authoritative is False:
        errors.append("signature is not authoritative")

    unresolved_call_symbols = [
        call.symbol
        for call in d.retail.calls
        if call.symbol not in d.symbols.functions
    ]

    if unresolved_call_symbols:
        d.warnings.append(
            f"unresolved call declarations: {unresolved_call_symbols}"
        )

    return errors
```

Fatal dossier errors should block generation rather than spend model budget.
