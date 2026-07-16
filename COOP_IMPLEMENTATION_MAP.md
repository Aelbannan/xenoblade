# Xenoblade co-op implementation map

This document maps co-op capabilities to decompilation work. It explains
**why** connected groups of functions matter; it is not a function database or
progress checklist.

The sole source of truth for function identity, address, size, priority,
dependencies, workflow state, and match state is
[`tools/coop/targets.json`](tools/coop/targets.json). Generate a current view:

```bash
python3 tools/coop/run.py targets status
python3 tools/coop/run.py targets status --milestone render
python3 tools/coop/run.py targets status --output TARGET_STATUS.md
python3 tools/coop/run.py targets brief <target-id>
```

`TARGET_STATUS.md`, when requested, is generated output and must not be edited.
Attempt history lives in `docs/evidence/decomp/attempts.jsonl`.

## Capability graph

```text
authoritative update (once)
  ├─ second input snapshot ─ actor ownership ─ native action submission
  │                                           └─ actor action prompts
  └─ presentation mode
       ├─ full-screen shared menus/cutscenes
       └─ sequential view 0 / view 1 rendering
            ├─ viewport + projection
            ├─ camera + culling per player
            ├─ native HUD context per player
            └─ frame tail + present once

two actor positions ─ streaming envelope ─ optional expanded-memory heap
```

Architecture invariants and milestone acceptance gates remain in `PLAN.md`.

## Render boundary and same-camera split

Required outcomes:

- simulation/update traversal executes once;
- scene/world rendering executes once per view, sequentially;
- UI animation, frame tail, EFB copy, and presentation execute only at their
  classified frequency;
- current view, viewport, scissor, projection, camera, culling, and renderer
  globals are restored explicitly;
- render mutation guards detect no authoritative gameplay changes.

Query the live function set with:

```bash
python3 tools/coop/run.py targets list --milestone render
```

## Input, actor ownership, and native actions

Required outcomes:

- preserve two independent controller snapshots;
- map controller 1 to a validated party actor;
- suppress only that actor's AI decision selection;
- submit movement, targeting, Arts, and follow-up prompts through native
  authoritative paths during the single simulation update;
- restore AI safely on disconnect, disable, party changes, and scripted modes.

```bash
python3 tools/coop/run.py targets list --milestone input
python3 tools/coop/run.py targets list --milestone battle
```

## Camera and culling

Required outcomes:

- maintain native camera state per player;
- update each camera once per simulation frame;
- apply the appropriate camera and rebuild culling before each view render;
- restore scripted/full-screen camera state without stale P2 state.

```bash
python3 tools/coop/run.py targets list --milestone camera
```

## Native HUD and action prompts

Required outcomes:

- bind each HUD presentation to the correct actor, pad, target, cursor, Arts,
  recharge, Talent state, and prompt state;
- advance authoritative UI/gameplay state once;
- render player-specific presentation inside each split rectangle;
- classify shared, duplicated, and full-screen-exclusive overlays.

```bash
python3 tools/coop/run.py targets list --milestone hud
```

## Presentation gating

Required outcomes:

- centralize split versus shared-full-screen mode;
- keep menus, dialogue, cutscenes, loading, visions, and Chain Attacks full
  screen until individually proven safe;
- finish transitions at frame boundaries and restore original globals before
  resuming split mode.

```bash
python3 tools/coop/run.py targets list --milestone presentation
```

## Memory, streaming, and range

Required outcomes:

- validate expanded MEM2 independently from retail heaps;
- allocate fork-only state from a dedicated co-op heap;
- trace loading, activation, collision, and draw-distance anchors;
- extend the envelope in measured stages and retain a safe tether policy.

```bash
python3 tools/coop/run.py targets list --milestone memory
```

## Target metadata contract

Each enriched function target should connect binary identity to feature intent:

```json
{
  "id": "cview-render-view",
  "region": "us",
  "kind": "function",
  "symbol": "renderView__5CViewFv",
  "address": "0x804415C4",
  "size": "0xCB4",
  "unit": "monolib/src/core/CView",
  "source": "libs/monolib/src/core/CView.cpp",
  "tier": "P0",
  "milestone": "render",
  "capabilities": ["dual_view_render", "render_boundary"],
  "depends_on": ["view-set-current", "scn-draw"],
  "questions": ["Can this run twice without authoritative mutation?"],
  "required_level": "EQUIVALENT_MATCH",
  "workflow_status": "ACTIVE",
  "status": "CODE_MATCH"
}
```

Imported catalog records begin at `P9`, `unassigned`, and `DISCOVERY`. Promote
and enrich only the symbols that enter a feature slice. Do not hand-copy target
state into this document.

## Registry maintenance

```bash
# Validate uniqueness and status vocabularies
python3 tools/coop/run.py targets validate

# Add every missing function from config/<region>/symbols.txt
python3 tools/coop/run.py targets import-symbols

# Literally add functions, objects, and labels
python3 tools/coop/run.py targets import-symbols --kind all

# Preview either import without writing
python3 tools/coop/run.py targets import-symbols --kind all --dry-run

# One-time migration of latest historical attempt results
python3 tools/coop/run.py targets sync-attempts

# Current coordination state lives in the same registry
python3 tools/coop/run.py targets claim <id> --owner <agent>
python3 tools/coop/run.py targets release <id> --owner <agent>
```

Imports are idempotent. Existing curated records and IDs win; generated IDs
use `<region>-<address>` and remain stable across later semantic renames.
