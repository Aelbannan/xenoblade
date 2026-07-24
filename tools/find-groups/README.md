# find-groups — Structural function clustering for the Xenoblade decomp

Finds groups of structurally similar functions by reading structural feature
vectors from the **Decomp Atlas** SQLite database and applying cosine-similarity
clustering. Within each translation unit, the tool runs exact connected-components
clustering. Optionally finds cross-unit patterns using LSH approximation.

## Quick start

```bash
# Prerequisite: build the Atlas vector index
python3 tools/decomp_atlas/run.py index --vectors

# Show top groups of NOT_STARTED functions
python3 tools/find-groups/cluster.py

# Show detailed member names for top 5 groups
python3 tools/find-groups/cluster.py --detail 5

# Show full IDs too
python3 tools/find-groups/cluster.py --detail 2
```

## Use cases

### 1. Find batch-matchable groups

The tool ranks groups by:

1. Most NOT_STARTED members first
2. Same-unit groups (share types/headers) first
3. Lower difficulty first
4. Larger groups first

The top groups are the best candidates for batch matching:

- Match one function from the group, apply the same pattern to the rest
- All functions in the same unit already share the same headers and types

```bash
python3 tools/find-groups/cluster.py --detail 3 --threshold 0.85
```

### 2. Focus on a specific unit

```bash
# All NOT_STARTED in kyoshin/cf
python3 tools/find-groups/cluster.py --unit kyoshin/cf

# Include already-matched functions too (to see what's been done)
python3 tools/find-groups/cluster.py --unit kyoshin/cf/CGame --all-status
```

### 3. Find cross-unit similar functions

Slower but finds shared patterns (getter/setter clusters, vtable dispatch
patterns, etc.):

```bash
python3 tools/find-groups/cluster.py --cross-unit --threshold 0.85
```

### 4. Save as JSON for further analysis

```bash
python3 tools/find-groups/cluster.py --json > groups.json
```

### 5. Adjust sensitivity

Higher threshold = fewer, tighter groups. Lower = broader, looser groups.

```bash
# Very tight: only nearly identical functions
python3 tools/find-groups/cluster.py --threshold 0.95

# Loose: find broader categories
python3 tools/find-groups/cluster.py --threshold 0.70
```

## What makes functions "similar"

The structural feature vectors capture:

- Function size and instruction count
- Branch density (control flow complexity)
- Direct and indirect call counts
- Opcode class distribution (arithmetic, load/store, float, branch, compare, logical)
- Stack frame size
- Relocation count
- Current match percentage

Functions with similar code *shape* get the same vector — setters look like
setters, dispatch functions look like dispatch functions, etc.

## Output interpretation

| Column | Meaning |
| -------- | --------- |
| `#` | Rank (by NOT_STARTED count + same-unit + difficulty + size) |
| `Size` | Total members in this group |
| `NS` | NOT_STARTED members |
| `NS%` | Percentage of members that are NOT_STARTED |
| `SameU` | ✓ = all members in the same translation unit |
| `Diff` | Average difficulty (0-100, higher = harder) |
| `SzB` | Average function size in bytes |
| `Status` | Most common match status in the group |
| `Unit` | Most common translation unit |

## Requirements

- Python 3.10+
- Decomp Atlas SQLite DB with vectors built (`atlas index --vectors`)
- No external Python packages required
