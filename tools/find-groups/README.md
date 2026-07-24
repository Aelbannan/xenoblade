# find-groups — Function similarity tools

Two tools for finding groups of similar functions in the Xenoblade decompilation,
targeting different levels of abstraction.

## Tool 1: `cluster.py` — Structural similarity (opcode-class profiles)

Groups functions by **code shape** — which *kinds* of instructions dominate
(loads, stores, arithmetic, branches, float, etc.). Finds functions with similar
overall structure, even if the specific instructions differ.

```bash
python3 tools/find-groups/cluster.py --detail 3

# Focus on one unit
python3 tools/find-groups/cluster.py --unit kyoshin/cf/CBattleManager --detail 3
```

**Best for:** Finding functions in the same translation unit that share
structural patterns → can probably batch-match with similar C code.

## Tool 2: `by-instructions.py` — Instruction-level identity (PPC fingerprints)

Groups functions with **identical instruction sequences** (register-normalized).
Functions in the same group have the exact same PPC — they almost certainly
decompile to the same C code.

```bash
# Level 2 (default): register-normalized — GPRs/FPRs replaced with placeholders
python3 tools/find-groups/by-instructions.py --detail 3

# Level 1: opcode mnemonics only — broader groups
python3 tools/find-groups/by-instructions.py --level 1 --detail 3

# Level 3: full operands — strictest matching (still normalizes registers)
python3 tools/find-groups/by-instructions.py --level 3
```

**Best for:** True batch matching — match one function per group, apply the
exact same C code to all others.

## Instructions tool: What the groups mean

| Pattern | Count | Meaning | C code |
| --------- | ------: | --------- | -------- |
| `bclr` | 70 | Empty function | `void func() {}` |
| `addi R,R,X; bclr` | 101 | Return constant | `return X;` |
| `lwz R,X(R); bclr` | 55 | Simple getter | `return member;` |
| `addis R,R,X; addi R,R,Y; bclr` | 37 | Return pointer | `return ptr + offset;` |
| `lbz R,X(R); bclr` | 17 | Byte getter | `return (u8)member;` |
| `stw R,X(R); bclr` | 15 | Simple setter | `member = value;` |
| `lfs R,X(R); bclr` | 13 | Float getter | `return (float)member;` |

These groups cross translation units because the patterns are universal.

### Deeper groups (5+ instructions)

Level 2 with ≥5 insns finds full functions that have identical PPC across
different contexts — these are the same function (or clone) in different TUs:

```bash
# Show groups with 5+ instruction patterns
python3 tools/find-groups/by-instructions.py --level 1 --detail 3 | head -10
```

## Batch-match workflow

1. Find a group of NOT_STARTED functions all in the same unit
2. Match one using the LLM harness
3. Apply the same `.cpp` change to the rest — same C code, same PPC output

```bash
# Find groups
python3 tools/find-groups/by-instructions.py --detail 1

# Match one target
python3 tools/llm_harness/run.py solve <target-id>

# The rest follow the same pattern
```

## Requirements

- Python 3.10+
- Atlas DB built with `--full --vectors`:

  ```bash
  python3 tools/decomp_atlas/run.py index --full --vectors
  ```

- No external Python packages
