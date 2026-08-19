# MWCC Patterns — general, reusable matching knowledge

The file for **how MWCC behaves**: compiler behavior, flags, register-allocation rules,
reloc/ABI/linkage patterns, and template pitfalls. **Per-target function records live in
[`MWCC_CASES.md`](MWCC_CASES.md).** Both files are indexed together by `mwcc_kb.py` (plus
attempts + contributions).

> **When to read this:** before matching a function whose residual hints at a reusable MWCC
> behavior, or to search/contribute knowledge.
> **Skip it:** for per-function stories → `MWCC_CASES.md`; for routing/commands → `SKILL.md`.

## How the matching docs fit together

| File | Role |
|---|---|
| `MWCC_PATTERNS.md` (this) | general/reusable: compiler behavior, flags, regalloc rules, reloc/ABI/linkage, KB protocol |
| `MWCC_CASES.md` | per-target/per-unit record log (one `##` per function) |
| `docs/register_mapping.md` | regalloc contract (routed from `reg_swap`) |
| `docs/scheduling.md` | `-O4,p`/`-O4,s` + scheduler (routed from `structural`) |
| `docs/instruction_selection.md` | `mr`/`addi`/`lbzu`/load selection (routed from those residuals) |
| `docs/KNOWN_WALLS.md` | fixed-codegen walls (FULL_MATCH-only vs dead-end) |
| `docs/mwcc/contributions.jsonl` + `build/mwcc_knowledge.sqlite` | machine layer the index reads |

## Record templates (apply to both files)

**`MWCC_CASES.md` — per target/unit:**

```
## <symbol/unit> — <residual> → <fix> (<compiler flags>, <result>)
- Symptom:   what hexdiff shows
- Cause:     why retail differs
- Fix:       the concrete change
- Result:    FULL_MATCH / EQUIVALENT_MATCH / <n>% near-miss
- Evidence:  <target id / source path>   (optional)
```

**`MWCC_PATTERNS.md` — general/reusable (same core, cross-target fields):**

```
## <pattern name> — <when it applies>
- Symptom:   shared shape across multiple targets
- Cause:     the MWCC behavior
- Fix:       the reusable recipe
- Applies to/a.k.a.: related flags, §17.6 escapes, or filenames
- Confidence: repo_proven / hypothesis / negative_result
- Example:   <one cited target id>        (optional)
```

## Search it (folded KB protocol)

`docs/MWCC_CASES.md` and `docs/evidence/decomp/attempts.jsonl` can be searched together through a generated SQLite FTS5 index. The index is a disposable cache under `build/`; the reviewable Markdown and JSONL files remain the sources of truth.

```bash
python3 tools/mwcc_kb.py build
python3 tools/mwcc_kb.py search "SDA relocation mismatch"
python3 tools/mwcc_kb.py search "stack frame spill" --status CODE_MATCH
python3 tools/mwcc_kb.py search "Chaitin exact size" --kind attempt
python3 tools/mwcc_kb.py search "vtable slot" --tag vtable
python3 tools/mwcc_kb.py search "regalloc prologue spill" --mode any --json
python3 tools/mwcc_kb.py show <result-id>
python3 tools/mwcc_kb.py show <result-id> --json
python3 tools/mwcc_kb.py stats
python3 tools/mwcc_kb.py tags
```

Search, show, and stats automatically rebuild a stale or missing index. Results point back to their source file and line. Reference heading IDs survive unrelated line shifts (renaming a heading changes its ID); attempt IDs include the target and append-only JSONL line number.

The initial inferred taxonomy covers ABI, compiler flags, control flow, inlining, literal pools, loop codegen, policy exceptions, register allocation, relocations, semantics, size, stack frames, struct layout, switch codegen, and vtables. Tags are search aids, not acceptance evidence.

## Agent search protocol

Agents should search from observed evidence, not paste an entire function or ask one oversized natural-language question. For every matching target:

1. Inspect canonical identity and current state with `targets show <target-id>`.
2. Search the exact function name and mangled symbol. This retrieves same-function reference notes and prior attempts.
3. Classify the current objdiff gap, then issue one short query per mismatch category. Useful vocabulary includes `relocation SDA`, `regalloc Chaitin`, `stack frame spill`, `struct layout offset`, `switch case order`, `loop mtctr bdnz`, `literal pool sdata2`, `vtable slot`, `ABI Fv`, and `inlining IPA`.
4. Search `--kind reference` for proven transformations, `--kind attempt` for experiments already tried, and `--kind kb_contribution` for agent-contributed patterns. Do not repeat failed attempts without a new reason.
5. Start with the default `--mode all`. If it returns no useful result, remove the least important term or use `--mode any`; then narrow with `--tag`, `--status`, or `--kind`.
6. Open the best few records with `show <id> --json`. Search snippets are discovery aids; the full record contains caveats, failed variants, and evidence.
7. Name the selected knowledge IDs in the attempt hypothesis and explain why their symptoms match the current diff. Apply one bounded experiment, then run `cycle`.

Example:

```bash
python3 tools/coop/run.py targets show pad-update
python3 tools/mwcc_kb.py search "update__Q22cf9CfPadTaskFv" --json
python3 tools/mwcc_kb.py search "relocation SDA float pool" --kind reference --tag relocation --json
python3 tools/mwcc_kb.py search "float pool frame regression" --kind attempt --mode any --json
python3 tools/mwcc_kb.py show <result-id> --json
```

For automation, use `--json`; index rebuild notices go to stderr, so stdout remains valid JSON. A zero-result search returns a JSON object with an empty `results` array and exit status 1.

## Contributing to the KB (`add_to_kb`)

When an agent discovers a reusable pattern (especially when closing the last few percent of a match), it should contribute it to the knowledge base so future sessions can find it. There are two paths:

### 1. During a `cycle` (inline)

```bash
python3 tools/coop/run.py cycle <target-id> \
    --hypothesis "retail uses lbl_eu_80667EB0 not @N" \
    --next-change "extern the float label" \
    --add-to-kb '{"title":"Direct extern SDA float in a compare",
      "symptoms":["instructions byte-identical but reloc names differ","lfs from @N not lbl_eu_*"],
      "fix":"extern \"C\" const float lbl_eu_*; use verbatim at each site",
      "tags":["relocation","literal_pool"],
      "target_id":"<target-id>","function":"<qualified-name>"}'
```

The contribution is appended to `docs/mwcc/contributions.jsonl` alongside the attempt log.

### 2. Standalone (any agent, any time)

```bash
python3 tools/mwcc_kb.py add '{"title":"...", "symptoms":[...], "fix":"...", "tags":[...]}'
python3 tools/mwcc_kb.py add /path/to/payload.json --from-file
```

This appends to `docs/mwcc/contributions.jsonl` **and** immediately rebuilds the SQLite index — so the contribution is searchable by all subsequent `mwcc_kb.py search` calls within the same session. (It does **not** appear in prompts of already-running agents; their context was frozen at session start.)

### Contribution record format

A minimal record needs `title`. A useful record includes:

| Field | Type | Description |
|-------|------|-------------|
| `title` | string | **Required.** One-line summary of the pattern. |
| `symptoms` | string[] | What objdiff shows (e.g. `"instructions match but relocation names differ"`). |
| `fix` | string | The high-level C++ change that closed the gap. |
| `tags` | string[] | One or more from: `abi`, `compiler_flags`, `control_flow`, `inlining`, `literal_pool`, `loop_codegen`, `policy_exception`, `regalloc`, `relocation`, `semantics`, `size`, `stack_frame`, `struct_layout`, `switch_codegen`, `vtable`. |
| `target_id` | string | The target this was discovered on (for traceability). |
| `function` | string | Qualified function name. |
| `notes` | string | Caveats, pitfalls, or when NOT to use this pattern. |
| `confidence` | string | `repo_proven`, `hypothesis`, or `negative_result`. |

Records are stored as JSONL (one JSON object per line) in `docs/mwcc/contributions.jsonl`. The SQLite index picks them up with `source_kind=kb_contribution`.

## Why SQLite is generated, not canonical

- SQLite FTS gives fast ranked full-text queries and joins the reference with attempt history.
- A binary database is poor for code review, merge conflicts, and blame.
- Markdown remains pleasant to read, while JSONL remains appropriate for append-only attempts.
- Rebuilding prevents stale duplicated truth.

Do not commit `build/mwcc_knowledge.sqlite`.

## Recommended end state

The current parser is a bridge. The long-term source of truth should be one small structured record per reusable pattern, with Markdown generated from those records and SQLite generated for querying.

Suggested layout:

```text
docs/mwcc/
  concepts/                 # stable compiler/ABI explanations
  patterns/
    reloc-retail-sda-name.json
    regalloc-block-scopes.json
  schema.json
docs/MWCC_CASES.md       # generated human-readable handbook
build/mwcc_knowledge.sqlite  # generated search index
```

One-file-per-pattern is preferred over one large JSON/YAML file because concurrent matching work then produces smaller merge conflicts. JSON works with the Python standard library and permits strict schema validation. If authoring comfort becomes more important than zero dependencies, YAML can be added later without changing the logical schema.

Suggested pattern record:

```json
{
  "id": "reloc-retail-sda-name",
  "title": "Use the retail SDA symbol name",
  "kind": "transformation",
  "symptoms": ["instructions match but relocation names differ", "lwz or lfs uses @sda21"],
  "causes": ["source references a mangled singleton or anonymous pool instead of the retail label"],
  "actions": ["declare the retail label with C linkage", "use it directly at the confirmed access site"],
  "avoid": ["blanket replacement of unrelated literals", "objdiff UI-only symbol mappings"],
  "tags": ["relocation", "sda", "linkage"],
  "compiler": {"family": "MWCC", "platform": "Wii/1.1"},
  "evidence": [
    {
      "target_id": "pad-update-cfpad-data",
      "function": "cf::CfPadTask::updateCfPadData",
      "source": "src/kyoshin/cf/CfPadTask.cpp",
      "before_percent": 99.95,
      "after_status": "FULL_MATCH",
      "after_percent": 100.0
    }
  ],
  "confidence": "repo_proven",
  "policy_exception": false
}
```

Important field distinctions:

- `symptoms` describe what objdiff shows; `causes` are explanations; `actions` are bounded experiments.
- `evidence` points to target IDs and source paths rather than duplicating current target state.
- `confidence` should distinguish compiler documentation, repo-proven patterns, hypotheses, and negative results.
- `policy_exception` prevents ordinary C++ advice from being mixed with §17.6 escape hatches.
- Current status remains canonical in `tools/coop/targets.json`; attempts remain append-only history.

## Migration sequence

1. Use the generated index immediately and refine searches/tags from real matching work.
2. Add a JSON Schema and a `validate` command for structured records.
3. Migrate the highest-value families first: relocation/SDA, ABI/linkage, struct layout, control-flow lowering, register allocation, literal pools, and policy exceptions.
4. Generate the handbook from structured records, preserving a short hand-authored concepts section.
5. Add `suggest <target-id>`: combine the target record, recent failed hypotheses, objdiff mismatch categories, and ranked patterns to propose the next bounded experiment.
6. Add feedback fields to attempt logging (`pattern_ids_tried`, `pattern_id_succeeded`) so pattern effectiveness can be measured instead of inferred from prose.

The most valuable future feature is not a generic chatbot over the file. It is a deterministic recommender that can answer: “Given this target's mismatch category, compiler flags, current percentage, size result, and already-tried hypotheses, which repo-proven experiment has the strongest evidence and has not yet been tried?”


## General patterns (migrated from `MWCC_CASES.md`, 2026-08)

> In-progress migration of reusable/cross-target knowledge out of the per-target log. If a pattern is not listed here, search both files via `mwcc_kb.py` — the index covers PATTERNS + CASES.

### Vtable-slot arg keeps a register live → flips ret-default allocation (CfObjectMap func_800B9A70, FULL_MATCH)

`func_800B9A70` (us-800ba38c, CfObjectMap, 0x44) was stuck at ~98% with the ret-default
register `li r0,1 + or r0,r3,r3 / or r3,r0,r0` (retail) vs `li r4,1 + or r4 / lwz-r0-first`
(decomp); the +8/+0xc and +0x30/+0x34 order diffs were consequences of the r0-vs-r4
choice, invariant across ~16 shapes. **Fix: the vtable slot +0x18C
(CfObjectModel_UnkVirtualFunc6) really takes a `void*` arg** (per CfObjectMove.hpp's
vtable decl `m18C(void*)`; the base CfObjectModel.hpp decl was missing the param).
Calling `this->CfObjectModel_UnkVirtualFunc6(this->mTarget70)` keeps r4 live as the
call argument, so MWCC cannot reuse it for the ret default — the default lands in r0
exactly like retail → 100% FULL_MATCH. Reuse: when a leaf's ret-default register
choice (r0 vs a caller-saved GPR) is allocator-fixed, check whether the indirect/
vtable call in the body actually takes the checked pointer as an argument — the
live-arg liveness forces the default into r0. (Same family as the WPAD
"passing status keeps r4 live" fix.)

---



---

### Section padding is a linker artifact — never fabricate it in source

Retail data slices (`splits.txt`) end at the next unit's **alignment boundary**, so
most per-unit `.sdata`/`.sbss`/`.bss`/`.data`/`.rodata` slices carry trailing zero
pad bytes (4 for 8-aligned slices, up to 31 for 32-aligned `.bss`). Those bytes
are **linker-inserted inter-unit alignment**, not content, and they reproduce
automatically when the neighbouring unit's section is aligned at link time. The
original source never contained them.



---

### Trailing no-op `self` guard reproduces an unused-r31 frame across a call (Wii/1.1)

`void f(self, ptr) { if (ptr) deallocate(ptr); }` compiles to a leaf tail-call
(no frame, `b deallocate`) — MWCC sees `self` dead across the call. Adding a
semantic no-op after the call that references `self` (`if (self != NULL)
return;` — the function returns anyway) keeps `this` live across the call, so
MWCC emits the retail's frame (`stwu; mflr; stw r31; mr r31, r3; bl ...; lwz
r31`) byte-for-byte. Verified on kyoshin/cf/CtrlRemote func_8009C980 (0.0% →
100%): the retail saves r31 = self and never reads it — a fixed allocator
artifact that only the live-range trick reproduces. Probed 12+ other shapes
((void)self, empty if/else, do-while-break, explicit return) — all tail-call.

**Antipatterns — do not add these to `libs/**` or `src/**`:**

- fake `.init`-section functions (`__declspec(section ".init") void
  FORCEACTIVE*_sdata(void) { fake_function("\0\0\0"); }`) to keep a zero string
  in `.sdata`;
- file-scope pad globals (`u32 __FooSbssPad;`, `char x[7]`, `s_sdata2Pad = 0.0f`,
  `GXAttrDataPad = (u32)&fn`, `#pragma sdata_threshold 0` + pad global);
- trailing `\0` padding inside string literals (`"USB: \0\0"`, `"...\n\0\0\0\0"`)
  to inflate a pooled string to its retail slot; padded `[8]`/`[2]` arrays whose
  extra element is a pad word; `u8 pad[4]` struct fields that only exist to grow a
  `.bss`/`.sbss` object;
- bare `(void)"string";` statements used only to force pool order.

They are fabrication (the source never looked like that), they add `.init`/`.text`
code and keep dead data alive through fake references (orphan `.init` content is
placed by the linker into the DOL's init segment), and the `fake_function` pattern
cannot link. The data pads themselves never change the DOL bytes: they are
unreferenced, and the linker re-inserts the same zeros from section alignment
(verified: after removing all of them, every touched unit's data slice is
byte-identical to retail in the freshly linked `main.dol`).

**Correct handling:**

1. Declare the object at its true size/type (`u32 x;`, `static char s[] = "str";`).
   The linker re-inserts the pad from the next unit's section alignment; the DOL
   bytes are identical with or without the fake (verified: GXInit/i2c .sdata).
2. objdiff only counts **named symbols**; unattributed pad is not scored. If the
   retail symbol *absorbed* the pad (ppcdis sized it to the next symbol — e.g.
   `__i2c_ident_flag` size 8, string pools like `lbl_8054B610` size 0x48), fix the
   **accounting, not the source**: correct the size in `config/<region>/symbols.txt`
   (true size = first NUL + 1 for strings; 4/2/1 for typed scalars) and regenerate
   the retail split objects (`dtk dol split --no-update` — note it skips existing
   objects, delete `build/<region>/obj` first). Then fix stale
   `symbol_sizes=(...)` overrides in `tools/postprocess_reloc_names.py` that baked
   in the absorbed size (e.g. NANDLogging `lbl_805512D4` 0x2C → 0x27).
3. Real dead-code string pools (a GC'd function whose format strings remain in
   retail `.data`) are **real content** — keep them via the documented
   `DECOMP_FORCEACTIVE` mechanism, not by padding literals.
4. Exception: a `[2]`/`[N]` array or padded literal is load-bearing when codegen
   depends on it (e.g. dvdDeviceError `fgColor[2]` forces the `.sdata2` slot load;
   a plain const folds to an immediate). Keep it and document why.

**objdiff data% quirk:** when the base data section is *smaller* than the target
slice (pad removed), objdiff's top-level `matched_data` can collapse to ~0 even
though per-section fuzzy stays high — the target's synthesized `gap_*` symbols
and the section-size delta confuse its scoring. This is cosmetic (acceptance is
function/code-based); if you want the data% line honest again, trim the slice end
to the last content symbol in `config/<region>/splits.txt` (and fix any symbol
whose ppcdis size absorbed the pad, e.g. `__i2c_ident_flag` size 8 → 4) then
regenerate the split. Do **not** re-add a pad to the source to satisfy it.

**Do not**: pad the decompiled object, add fake sections/symbols, or chase
per-unit data% by growing sections. `EQUIVALENT_MATCH`/`FULL_MATCH` acceptance is
function-level and does not depend on the pad bytes.

---



---

### Isolated Gekko paired-single backends — worked results (nw4r g3d + math)

### monolib effect code_804DB938 — func_804DD89C / func_804DD8C8 lerp kernels (Wii/1.1)

Two 3/4-component vector lerps (out = a + (b - a) * t) are retail
`psq_l/ps_sub/ps_madds0/psq_st` kernels in the load-all-first schedule
(psq_l a.xy + b.xy, then a.z/b.z as W=1 singles, ps_sub both, ps_madds0
both with the scalar t from f1, psq_st both halves).

- **MWCC's -O4,p auto-vectorizer does NOT emit this from scalar field ops**
  (keeps lfs/fsubs/fmadds/stfs, ~0x40 vs retail 0x2c) — the previous
  "straight field ops trigger the vectorizer" comment was wrong.
- **nw4r VEC3Lerp's inline asm interleaves XY-then-Z** (load-sub-madd-store
  per pair) and does NOT match the load-all-first retail schedule.
- The whole retail kernel was shipped as an `asm void` body with `nofralloc`
  in `libs/monolib/include/monolib/effect/code_804DB938_ps.inl`
  (PLAN §17.6, __MWERKS__ && !NONMATCHING guard, scalar fallback in the
  same .inl) → both **100.0% FULL_MATCH** (0x2c/0x2c), unit split PASS.
- `psq_l fD, d(rA), W, I` syntax: W=0 = 64-bit pair, W=1 = 32-bit single;
  the retail uses qr0 (I=0) throughout. The asm bodies reference explicit
  ABI registers (r3=out, r4=a, r5=b, f1=t) and need an explicit `blr`.

---

Four nw4r PS kernels reached byte-exact match by shipping the **retail SDK's own
whole `asm` function bodies** in designated PS backend `.inl` files, instead of
register-operand `ASM()` blocks. Findings that transfer to any PS target:

- **Whole `asm` bodies are the only form that reproduces retail PS kernels.**
  MWCC reschedules register-operand `ASM()` blocks and recolors FPRs (e.g. a
  verbatim retail-order block came back with hoisted loads and a different FPR
  coloring). The retail nw4r SDK compiled these functions as `asm bool/MTX34*`
  bodies with `nofralloc`; identical bodies compiled byte-exact. Reference
  source: the Skyward Sword decomp port `zeldaret/ss` `src/nw4r/g3d/g3d_transform.cpp`
  (same SDK code, byte-identical to Xenoblade retail). `CalcViewNrmMtx`,
  `CalcViewTexMtx`, `CalcInvWorldMtx` = `FULL_MATCH` 100%, semantic certificates,
  split `0x2F0`/`0x2F0` (`libs/nw4r/include/nw4r/g3d/detail/g3d_transform_ps.inl`).
- **Reference retail data-pool symbols instead of defining TU-local constants.**
  Retail loads the epsilon / (65536,65536) pair via `lis/addi` + `psq_l` from
  `lbl_eu_80669B40` / `lbl_eu_80669E50`, which live in the retail data pool
  (`nw4r_data.o`, not the function TU). Declare `extern "C" { extern const f32
  lbl_eu_80669B40; }` and reference the same symbol from the asm. This keeps the
  decomp object free of extra data sections, makes relocations (and objdiff
  `functionRelocDiffs=data_value` fuzzy) match retail exactly.
- **`.inl` included inside a namespace must NOT reopen the namespaces** — the
  g3d kernels first shipped with their own `namespace nw4r { namespace g3d {
  namespace detail {` wrappers and came out double-mangled
  (`CalcViewNrmMtx__Q6nw4r3g3d6detail4nw4r3g3d6detailF...`, objdiff 0%).
- **`MTX34._00(ptr)` field-offset syntax fails in asm bodies in this project's
  TU context** (`<string not found>`); use numeric offsets (`0x0(r4)` etc.).
- **Frame-trigger bug:** a `psq_l/psq_st` whose base is a `register` parameter
  name, inside an asm body that also contains `stwu r1, -0xN(r1)`, makes MWCC
  3.0a5.2 insert its own `stwu/or r31,sp,sp` prologue + `lwz sp,0(sp)` epilogue.
  Writing explicit registers (`0x2c(r3)` instead of `0x2c(pMtx)`) avoids it
  (the parameter is in r3 by ABI).
- **`li r0, X@sda21` cannot be emitted by MWCC 3.0a5.2 asm** — the assembler
  accepts only `@h/@ha/@l/@loword/@hiword`; probed `@sdarel`/`@sda21`/`@sda`
  variants all fail with error 33135 `<string not found>`. The compiler *does*
  emit `addi rX, 0, X@sda21` (R_PPC_EMB_SDA21) for a C++ pointer init
  (`register const f32* p = &sdata2sym[0];`), but only in register-operand
  (non-asm-body) code, which for this kernel adds a frame-pointer save and
  reschedules the PS stream. `MTX34RotXYZFIdx` (retail `li r0,lbl_eu_80669E50@sda21`)
  therefore stays at hexdiff 100% / objdiff 99.943% (the `@l` fallback is
  byte-identical; SMT prove times out on the PS kernel). Recorded for out-of-band acceptance in
  `attempts.jsonl` — the revisit path is a newer MWCC with `@sda21` support or a
  linked-DOL prove with the `li` baked.
- `g3d_transform_ps.inl` / `math_types_ps.inl` are guarded by
  `#if defined(__MWERKS__) && !defined(NONMATCHING)`; the scalar C++ fallbacks
  (CalcAdjugate/IsInvertible/FastReciprocal, SinCosFIdx-based rotation) remain
  in the owning TUs for PC/NONMATCHING builds.

---



---

### Route-scene GQR5 setup remains a compiler artifact

`UnkClass_8047D2AC::{func_8047DF54,func_8047DE3C}` use retail `mtspr GQR5` with `0x03070307` before the signed-fixed-point scene-manager calls and `0x00070007` afterward. The normal high-level candidate preserves the solver calls and state updates, but no source-level `OSSetGQR*` or scalar initialization reproduced the orphaned GQR writes; the bounded candidates stayed at 69.2%/76.1% and split-size PASS. Keep the candidate readable and do not add inline assembly or register tricks; revisit after the `UnkClass_8047E110` solver callee frontier is accepted.

### Scene rectangle-walk reconstruction (`us-80484870`)

`UnkClass_8047E110::func_804808A0` is a bounded frontier walk across adjacent XZ rectangles. Four high-level source details raised it from 56.0% to an exact-size structural match: use `0.25f` only for the goal-cell center but `0.5f` for traversed rectangle extents; use an `s32` adjacency index so MWCC assigns CTR to the inner loop; express the outer limit as `frontierCount != 0 && ++pass < 90`; and materialize `neighborOffset = edgeOffset + 1` separately so retail's two scaled address calculations survive CSE. Explicit `if`/`else` portal min/max assignments produce retail's per-arm stores, unlike ternaries.

Model the current, portal, and candidate bounds as **six separately declared `CVec3` min/max objects**, in reverse stack order (`currentMax/currentMin`, `portalMax/portalMin`, `candidateMax/candidateMin`). A padded scalar rectangle has identical field offsets but MWCC assigns its stack slots by first use, leaving a 72-byte slot rotation; separate vectors reproduce every retail stack offset. Declaring `pass` before the queue pointers also makes queue-next/frontier-count use retail `r8/r9`.

Preserve `graph.edges` in a local **mutable `u16*`**, matching the member's exact type, before deriving the count and neighbor pointers. Converting it immediately to `const u16*` introduces a separate MWCC virtual register and rotates the current-node GPR colors. Also declare `crossingX/Z`, `deltaX/Z`, `distance`, the two slopes, maxima, then minima at function scope; this natural old-style local order gives MWCC the retail virtual-ID order. Retail anchors line crossings at `goalCenter` (mathematically the same line as a position anchor), spells the final comparisons as `bestDistance < distance` and `goalNode == candidateIndex`, and swaps the two queues via `swap = next; next = frontier; frontier = swap`.

Together these shapes reach 98.8% CODE_MATCH, exact `0x650` size, zero structural differences, and 73 opcode-identical register-color mismatches. The residue is three independent Chaitin cycles: goal conversion `f3/f4`, traversal `f28/f30/f31`, and loop `r7/r10/r11/r12`. Aggregate arrays/workspaces lose retail's fourth saved FPR and regress by 8 bytes. Do not use register or stack steering to close the residual cycles.



---

### Quick diagnostic: plateaued at 97–99.9% — what do I check?

| Symptom | Most likely cause | Fix |
|---------|-------------------|-----|
| Instructions byte-identical, match ~99.3–99.9% | Relocation name differs (`spInstance` vs `lbl_eu_*`, `@N` float pool, `@stringBase0` offset) | **§1** — `extern "C" lbl_eu_*` or post-process rename |
| One `mr r3, rN` missing | MWCC coalesced a dead copy | `return this;` if retail ends with `this` in r3; reorder locals |
| `mr r3,r30` vs retail `addi r3,r30,0` | Peephole of identity move / `ptr+0` (even asm `addi`) | §17.6 `opword 0x387E0000` (+ call setup); avoid extra NV locals that reshuffle r30/r31 — **not** `.text` postprocess |
| `lwz r4/r5` vs retail `lwz r12` on manual vptr | Non-virtual FP temps | Fake SI virtual iface (`MenuBpsActorIf` pattern) or further high-level reshape; **not** `asm void` / `insn_patches` |
| Frame size wrong (`stwu r1,-0xN`) | Different spill count / callee-save set | **§5** — reduce live ranges, split into helpers, match retail inline boundaries |
| `bl` target reloc wrong | C++ mangling on callee | **§2** — `extern "C"` with retail mangling |
| Branch layout inverted | Ghidra if/else vs retail | Swap blocks or invert condition |
| `switch` shape wrong | MWCC emits compare-tree vs jump-table | Match asm case order; duplicate "useless" cases |
| Two identical patterns, opposite regalloc | Shared locals forced one color | **§4** — block-scope a fresh pair |
| Wrong register for same opcode | Declaration order | **§4** — reorder locals |
| Wide-arm reuses `r30` for height (`lha r30` vs retail `lha r31`) after non-wide correctly uses `self=r30`/`height=r31` | Precise liveness: `self` dead on wide arm so Chaitin recycles `r30`; keep-alives scramble dual-`getRenderModeObj` schedule | **CGame::func_800395F4** soft-cap ~99.8%: non-wide `s32 height` + `spInstance` reload; leave wide `s16` — do not chase keep-alive commas/ternaries |
| `addi …, @stringBase0, imm` wrong imm / missing Restart→CGame pool | Pointer-table `force_active` does not pack MWCC `@stringBase0`; contiguous pool needs a `.text` FORCEACTIVE that references the literals in order | Keep non-vararg `force_cgame_strings(Restart,"",43,arc,brlyt)` then **`drop_text_symbols`** the 0x1C thunk in `postprocess_reloc_names.py` (`CGame.o`) so stubs still fit `0xD08` |
| Stack slot at wrong offset | MWCC alignment / padding | **§5** — use multiple smaller types (`CPnt16` × N not `CRect16` × 1) |
| Zeros/sign-extension: `extsh` vs none | `s16` local vs `int` | Use `int` unless retail has `extsh` |
| Struct offset off by 4/8 | Missing pad or wrong base layout | Fix header layout before tuning C++ |
| Bitfield `extrwi` pos off by 2 (e.g. bit 19 vs 17) | Adjacent bitfields swapped in MWCC MSB-first layout | Reorder fields in the bitfield struct to match retail extract (nw4r `TexMap::mBits`: `paletteFormat` before `anisotropy`) |
| `unk64`-style flag test: retail `rlwinm. r0,r0,0,28,28` but decomp `rlwinm.,0,27,27` (or branch direction inverted) | **PPC bit numbering is MSB-first**: `rlwinm mb=m,me=m` masks PPC bit `m` whose *value* is `0x80000000>>m` (PPC bit 0 = MSB/0x80000000, PPC bit 31 = LSB/0x1). A C source `(x & 0x8)` maps to PPC bit 28, while `(x >> 30)&1` extracts C-bit 30 = PPC bit 1 (value 0x40000000). Decomp agents repeatedly confused the two (wrote `>> 30` for the value-0x2 flag). Bit-test polarity also flips: retail `beq skip` (continue when bit set) vs `bne skip` (continue when clear) means the ternary is `(flag) ? 1 : 2` not `? 2 : 1` | Translate `extrwi rD,rS,1,b` → `(x >> (31-b)) & 1`; `rlwinm. rD,rS,0,m,m` (test) → `x & (0x80000000>>m)`. Match the exact `beq`/`bne` skip direction (it encodes true/false polarity). `isPC`/`isNPC`/`isENE` (`ocUnit.cpp`): retail `extrwi r0,r0,1,30; neg r5,r0; addi r0,r5,2` ↔ source `retVal.type = ((obj->unk64 >> 1) & 1) ? 1 : 2` (bit-extract cond → MWCC keeps the branchless `neg/addi` idiom) |
| VM plugin funcs call `func_801862C0` with a stale `r3`; decomp's `func_801862C0(pThread)` emits an extra `mr r3,r30` → +4 bytes structural | `func_801862C0` (`us-80187748`, FULL_MATCH) **ignores r3**; callers never need to set it. When a `vmArg*` precedes the call, retail leaves r3 stale while decomp reloads `mr r3,r30` | **RESOLVED**: `func_801862C0` is now `void* func_801862C0(void)` in `code_801862C0.hpp`+`.cpp`; all ~61 callsites in `ocUnit`/`pluginCfs`/`pluginCam`/`pluginBtl`/`CfObjectPoint` call `func_801862C0()`. C-linkage symbol unchanged → impl FULL_MATCH preserved; ~40 ocUnit VM funcs jumped 50%→90%+. (Note: `pluginBtl`/`pluginCfs` have *pre-existing* unrelated compile errors — `cf::CfGameManager::getInstance` / `func_8009D790` overloading — not caused by this change.) |
| `__dynamic_cast` RTTI args: decomp passes the global's **value** `(void*)lbl_eu_806618D8` → MWCC dereferences (`lwz`) | retail passes the **address** (`li r5, lbl_eu_806618D8@sda21`) | Pass `&lbl_eu_806618D8` (address-of), matching `CBattleManager.cpp`/`CVision.cpp`. Fixed across ocUnit.cpp |
| Same RTTI address (0x806618D8) has **two reloc names** across functions | retail uses `__RTTI__Q22cf13CfObjectActor` in `invin`/`lookAt`/`func_8003E974` but `lbl_eu_806618D8` in `setColi`/`func_8003EB64` — **per-function** | Match the exact name each function uses (check `hexdiff --symbol X --brief` “Reloc drift”); declare `extern "C" void* __RTTI__Q22cf13CfObjectActor;` (from `CVision.hpp`) where needed |
| CfObject talk sub-object: source used `obj->mSubObj38` (offset 0x38) | retail reads `obj+0x74` for the `func_8013D07C`/`func_8013D448`/vtable[0x50] sub-object arg | `mSubObj38` (header offset 0x38) is a **different** field used by other TUs — don't retarget the header. In ocUnit.cpp use `*(void**)((u8*)obj + 0x74)` for the talk sub-object (winTalk/func_8003DD44/func_8003DDF4/func_8003E528) |
| SDA vs far addressing | Different insn (`lwz@sda21` vs `lis`/`addi`) | Correct section/size via linker script |
| Wrong callee-save depth | `_savegpr_29` vs `_savegpr_28` | Shrink live ranges across large functions |
| Inlined vs outlined | Extra/missing `bl` | Match retail inline boundaries; IPA pragmas |
| Same body duplicated → ~99.3% regswap vs retail | Manual paste of callee body keeps callee's RA | Prefer `callee();` + `-ipa file` so IPA inlines in *caller* context (different Chaitin colors) |
| Wrong float constant pool | `lfs` from wrong `.sdata2` slot | `extern "C" const float lbl_eu_*` |
| Ternary vs `if/else` codegen | Extra `b` or `sel` | Toggle form |
| Ghidra `r13` SDA | Misleading decompilation | Set SDA bases in Ghidra |
| Retail materialises a struct base (`addi r3,rX,0x3e`) for a long run of stores; all pointer/volatile/field-store forms fold back to direct offsets | MWCC keeps a **walked pointer** in a base register but folds constant-index/field accesses | Declare `u16* q = &obj->sub.vDelta;` and advance with `*q++` per store; start one field before the run so MWCC materialises at the retail base after the first folded store (HBMMIXInitChannel tail, 594/594; **__MIXRmtUpdateSettings phase-3**: walk `u16* q = (u16*)((u8*)out + 0x102)` so the folded cur0 store lands at `258(r30)` and the run materialises `addi r3,r30,260` with `sth 0..28(r3)` — 167 structural → 0, FULL_MATCH us-8034f910) |
| 3-op load-order reg-swaps in a top-level sum (`lwz` order differs, adds identical) | MWCC rotates a top-level sum chain `[s0,s1,s2]` into loads `[s2,s0,s1]` (tree `((s2+s0)+s1)`) | Write the source in rotated order: retail loads `[panFrontL, fader, X]` require `fader + X + panFrontL`; sums nested in a larger tree (`(a+b+c)-30`) are NOT rotated (HBMMIXInitChannel) |
| 0 structural, pure reg-swaps, but `cycle` witness never certifies | **Gate 6 reject-list: any prologue that saves FPRs emits `stfd`+`psq_st` pairs (MWCC always does this for f14–f31 saves), and the register-renaming witness unconditionally rejects `psq_*`** — witness-ineligible no matter how clean the body diff. Also: commutative `add` operand-order swaps break rho (r6 maps to both r6 and r0) and the region-sliced fallback refuses. **Confirmed 2026-08-03: the witness also never applies to any function containing a `bl` call** — 0/11 witness-certified certs in the sidecar contain `bl`; opaque-EABI callee contracts make the terminal-state comparison diverge on callee effects. For ANY call-containing target, the only no-SMT acceptance is FULL_MATCH (byte-identical) | Record stall with `next_change: accept via --smt out-of-band` (SMT is out-of-band per orchestrator). Skip further regalloc attempts — no source lever flips MWCC's callee-save colors for FPR-saving functions (hbm/seq.c `__HBMSEQInitTracks` 12 swaps, `HBMSEQRunAudioFrame` 16 swaps; `__HBMSEQReadHeader` 1 swap via rho conflict); for bl-containing targets the reg-swap levers are equally unlikely to pay off unless a FULL_MATCH shape is reachable (e.g. wpad `WPADiExcludeButton` r5↔r6 pointer color: 3 source variants no-op, 17 swaps — recorded for SMT acceptance) |
| **Branchy pure reg-swap pairs reject as "terminal pair diverges structurally — gpr r0"** | The cheap disjointness simplify (`_z3_simplify`) is a local rewrite: cross-path pairs of branchy functions build `And(X, Not(X))` from per-side byte-read memory equalities that are NOT structurally identical across sides, so `is_false` returns False and the impossible pair is compared (diverges on a lane live on only one path). **FIXED 2026-08 in `run_structural_witness`**: bounded QF_BV `unsat` fallback (`_path_conditions_disjoint_sat`, 10 s timeout, fail-closed on sat/unknown/timeout) proves the cross paths cannot co-occur and skips the pair. Sound: `unsat` for the conjunction is a proof of infeasibility. 142 tests green incl. `test_branchy_cross_pair_disjointness_sat_fallback`. Unblocked `CMMTex::OnFileEvent` (us-801184b8, rho r5↔r6, EQUIVALENT_MATCH witness-certified). Apply to any branchy reg-swap target whose witness reason is `structural … gpr r0` |



---

### How objdiff matching works here

objdiff compares **relocatable `.o` files**: the **target** (split from retail `main.dol`) vs the **base** (built from your C++ source). `coop.json` uses `functionRelocDiffs=data_value` — stricter than upstream default.

| Status | Rule |
|--------|------|
| `FULL_MATCH` | `function_match >= 100.0` |
| `CODE_MATCH` | `>= 95.0` (instructions match; relocs may differ) |
| `HIGH_MATCH` | `>= 70.0` |

**`EQUIVALENT_MATCH`** (fuzzy ≥ 50% + ppc_equivalence + split-size fit) and **`FULL_MATCH`** are equal-tier acceptance outcomes.

### Commands

```bash
python3 tools/coop/run.py build monolib/src/core/CView
python3 tools/coop/run.py diff monolib/src/core/CView --symbol setCurrent__5CViewFv
python3 tools/coop/run.py cycle view-set-current --hypothesis "..." --next-change "..."
python3 tools/coop/run.py ctx libs/monolib/src/core/CView.cpp
python3 tools/coop/run.py size monolib/src/core/CView           # split budget check
```

---



---

### MWCC compiler behavior

MWCC for PowerPC targets the **EABI** with two small-data areas:

| Register | Section | Typical use |
|----------|---------|-------------|
| **r13** | `.sdata` / `.sbss` | Mutable small globals |
| **r2** | `.sdata2` / `.sbss2` | Read-only small data, float constants |

Access is **base + 16-bit offset** (`lwz r3, lbl@sda21(r13)`). Relocations must name the same symbol retail used.

Relevant passes: **IPA** (cross-TU inlining via `-ipa file`), **dead store elimination**, **copy propagation** (eliminates dead `mr` moves — common 96–99% blocker), **Chaitin register allocation** (r3–r12 first, then r31…r28, then stack).

Key flags: `-O4,p` (speed) vs `-O4,s` (size), `-inline auto`, `-use_lmw_stmw on`, `-fp_contract on`, `-func_align 4`.

### `configure.py` flags in this repo

```python
# Global base:
"-O4,p", "-inline auto", "-fp_contract on", "-enc SJIS", "-Cpp_exceptions off"

# Game libs add:
"-ipa file", "-use_lmw_stmw on", "-RTTI on", "-Cpp_exceptions on"

# Per-object overrides:
Object(MatchingFor("jp"), "kyoshin/CGame.cpp", extra_cflags=["-O4,s", "-func_align 4"]),
```

**Matching flip** — only after **entire TU** is 100%:
```python
Object(Matching, "monolib/src/core/CViewRectDataCore.cpp"),  # was NonMatching
```

### RVL_SDK `OS_TICKS_TO_*` macros need a constant bus clock (i2c.c, FULL_MATCH)

**Symptom:** every inlined `WaitMicroTime`/delay loop is bloated (~2x) with
`lis`/`lwz` from `0x800000F8` plus `mulhwu` reciprocal steps; functions save
extra callee-saved GPRs (`_savegpr_27` vs retail `_savegpr_29`-style manual
`stw`), and the TU overflows its split budget. Relocation *layout* otherwise
matches retail exactly.

**Cause:** in this repo `OS_BUS_CLOCK_SPEED` is a memory-mapped **runtime
global** (`OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED, 0x800000F8)` in
`OSHardware.h`), so `OS_TICKS_TO_USEC(x) = ((x)*8)/(OS_TIME_SPEED/125000)`
divides by a runtime value. Retail SDK builds used a **compile-time constant**
`#define OS_BUS_CLOCK_SPEED 243000000`, folding the divisor to `486`
(`li r6, 0x1e6; li r5, 0; bl __div2i`).

**Fix (without touching shared headers):** in the TU, define a local
constant-based macro and use it instead of `OS_TICKS_TO_USEC`:
```c
#define VI_I2C_BUS_CLOCK_SPEED 243000000
#define VI_TICKS_TO_USEC(x) (((x) * 8) / (VI_I2C_BUS_CLOCK_SPEED / 4 / 125000))
```
`243000000/4/125000` folds to 486; `(s64)*8` lowers to `slwi/slwi/rlwimi` and
the s64 division to `__div2i` — byte-identical to retail. Also note `-ipa file`
+ `-inline auto` inlines **global** (non-static) functions within the TU while
keeping the external body (retail `WaitMicroTime` is GLOBAL yet fully inlined
into `sendSlaveAddr`/`__VISendI2CData`), so keep such helpers non-static when
retail symbols are global. Files: `libs/RVL_SDK/src/revolution/vi/i2c.c`
(us-80369770/us-80369800/us-80369b50, all FULL_MATCH).

---



---

### C/C++ → MWCC translation patterns

| C/C++ idiom | Matching tip |
|-------------|--------------|
| `if (a) x=1; else x=2;` | If wrong, swap blocks or invert condition |
| `return cond ? a : b;` | Toggle ternary ↔ if/else |
| `extern "C" void f();` | Required for correct `bl` reloc |
| `Class::method()` call | Wrong mangling → reloc miss |
| Member access `obj->field` | Wrong layout in header → everything wrong |
| Virtual call | Check vtable order and `this` adjustment |
| `Fv` method, arg in r4 | Use proper C++ parameter types, not `register u32* r4` |
| `static inline` / header inline | May disappear; match retail inline boundaries |
| `volatile` | Only for real MMIO; fork discourages fake stack volatiles |
| `switch` | Match asm case order; may need compare-tree not jump-table |

### Fv ABI note

Metrowerks often passes **extra arguments in registers** even on `…Fv` symbol names. Express as **normal C++ parameters** (`const ml::CPnt16&`, etc.). `symbols.txt` must use the mangling MWCC actually emits (e.g. `…FRCQ22ml6CPnt16`), not shortened `…Fv` placeholders.

**Explicit retail-name entry points:** A symbol such as `releaseCacheLocal__5CRsrcFPCv` may have a shortened retail name while its actual ABI still receives `CWorkThread* parent, const void* data`. Define the high-level function as `extern "C" bool releaseCacheLocal__5CRsrcFPCv(CWorkThread* parent, const void* data)` rather than as a `CRsrc` member. A C++ member definition emits `releaseCacheLocal__5CRsrcFP11CWorkThreadPCv`, leaving the retail-named caller unresolved. The same pattern applies to `isExistFile__5CRsrcFPCcPPvPUi` and `isExistDataLocal__5CRsrcFPCv`; the suffix is the preserved retail symbol name, not a reliable complete parameter list. Verified in `libs/monolib/src/core/CRsrc.cpp`: all three sizes and instructions match retail.

**Retail-owned vtable data:** Some split TUs contain only a class's `.text`; the retail vtable/RTTI is supplied by a data object. Marking such a class `__declspec(novtable)` prevents MWCC from adding `.data`, `.rodata`, and RTTI to the TU. If the retail constructor/destructor writes the vtable pointer, assign the known retail vtable label explicitly in those high-level functions. `CRsrcData` then has only `.text`/exception sections, an exact `0x42C` text budget, and no linker-generated duplicate metadata.

**COccCulling vtable label (us-801a1bc0):** retail `__ct__11COccCullingFv` stores `lbl_eu_80532ED0` (0x10-byte vtable in split1 `.data`); the decomp's compiler-generated `__vt__11COccCulling` reloc name drifts (HA/LO pair at the `lis`/`addi`). Fix: `__declspec(novtable)` in the header + `extern "C" void* lbl_eu_80532ED0[];` + explicit `*(void**)this = (void*)lbl_eu_80532ED0;` as the first ctor/dtor statement. Result: the **dtor becomes byte-identical** (store lands where the compiler's implicit one did), but the **ctor's store is scheduled at the end** (13 reg-swaps + 2 structural, ~88% fuzzy) because the member-init-list runs before the body — the implicit store is interleaved after the first member store in retail. The ctor's SMT proof needs the linked `main.elf` (its vtable HA/LO reloc is unresolved in the unlinked `.o` pair); the residual is pure constant stores, so it certifies EQUIVALENT_MATCH once `ninja build/us/main.elf` exists.

**Inline-empty base dtor elides the call in derived dtors (CTaskCulling dtor, us-801a4278):** MWCC only elides a base-class dtor call in a derived dtor when the base dtor's empty body is visible in the same TU. Retail's `IWorkEvent` header was `virtual ~IWorkEvent(){}` (inline-empty) — `~CTaskCulling` then calls only `~CProcess` (+ member `~COccCulling`), no `~IWorkEvent`, and the dtor is 0x78. When the dtor was moved out-of-line (IWorkEvent.cpp), the call reappeared (+0xC, dtor 0x84, unit 8 bytes over split budget). Fix: keep `virtual ~IWorkEvent(){}` inline-empty in the header, and keep a **strong copy** in the key-function TU (`src/kyoshin/CTaskGame.cpp` had `IWorkEvent::~IWorkEvent() {}` matching retail's strong symbol placement — remove it only if the header body replaces it; an out-of-line redefinition of an inline member errors with `(10333) object redefined`). Verified: derived `~Der` with a secondary inline-empty base emits only the offset-0 base call + delete; the decomp `~CTaskCulling` returned to byte-identity and the unit to 0x708 ≤ 0x70C.

**Pool-cookie reloc drift certified by the register-renaming witness (no SMT/linked DOL):** for byte-identical functions whose only diff is a TU-local pool reloc (`@N` vs `lbl_eu_*`), the mined reloc map (`reloc_map.py mine` → `retail_reloc_map.json`) canonicalizes both names via the decoder's `canonical_symbols` hook, so the **pre-SMT witness certifies directly** (`register-renaming-witness: N terminal pair(s) structurally equal under rho`) — no Z3, no `main.elf`. Verified: `func_80222258__16CMCCylinderGaugeFv` (us-80224098, `@6134`→`lbl_eu_80668520`) and `__ct__16CMCCylinderGaugeF…` (us-80223cac, `@6092`→`lbl_eu_80668518`) both accepted EQUIVALENT_MATCH with 99.7% static and exact 0x8D4 size. **Re-mine after any edit that shifts pool numbering** (removing a `.data` vtable via novtable renumbers every `@N` in the TU — the stale map entry then silently un-canonicalizes). `__vt__`-named drift (global symbols, not `unit@` keys) is NOT canonicalized; fix those in source with `__declspec(novtable)` + explicit retail-label assignment (`__vt__6CToken`→`lbl_eu_8056B52C` via `*(void**)this = (void*)lbl_eu_8056B52C;` as the first ctor statement — works byte-identically when the member-init list is empty; with a non-empty init list the manual store is scheduled at the end, see COccCulling above).

When a vtable / data table already references the shortened `…Fv` name (common for help/switch helpers), keep the retail symbol via `extern "C"` and take the extra args on that entry point, e.g. `func_802B7CBC__Q22cf11CHelpSwitchFv(self, u32 flag)`.

**LOD Fv entry-point verification:** `libs/monolib/src/lod/code_804645CC.cpp` confirms that a high-level `extern "C"` definition with explicit ABI parameters can retain a shortened Fv linker name; `func_80465704__Q23LOD17UnkClass_804645CCFv(s32)` reaches 100% static match (0x14 bytes). Do not use `asm("...")` symbol-label syntax with MWCC Wii/1.1 build 151: it fails at compile time with error 33106 (`<string not found>`), including on free functions. Use the explicit `extern "C"` Fv entry-point form instead.

### cf::CHelp layout (manual iface at +0x8)

Retail `__ct__Q22cf5CHelpFv` stores `owner@0`, `param@4`, `lbl_eu_8053B3A0@8` — **not** a C++ vptr at +0. Derived helps add fields from `+0xC` (`s32` thresholds need `s32`/`cmpw`, not `u32`/`cmplw`). Calls through `this+0x8` are a manual interface table; MWCC function-pointer codegen often uses `r4` where retail virtual-style loads use `r12` (~99.3–99.6% near-miss).

High-level source should use `mVtbl->mSlots[N]` (or equivalent `this+0x8` reload between calls). Near-misses that are only vtbl temp register coloring (retail `r12` vs MWCC `r4`) need further high-level C reshaping — do **not** post-process `.text` instruction words and do **not** use inline asm.

**Actor/move deep vtable slots (r12):** function-pointer `vslot` loads color the vptr as `r4`. Cast the object to a fake single-inheritance interface with `virtual` methods at the retail byte offsets and call through that — MWCC emits `lwz r12,0(obj)` / `lwz r12,off(r12)` / `bcctrl`. With kyoshin `-RTTI on`, omit the first **two** pad virtuals so slot `0xN` lands at offset `0xN` (hidden typeinfo slots). See `MenuBpsActorIf` / `MenuBpsMoveIf` in `CMenuBattlePlayerState.cpp` (`menu-bps-move` ~99.8%).

**reslist::size() regalloc:** retail often wants `length` in r4 / `cur` in r3. Declaring `length` first in a *local* inlined helper can match without changing shared `reslist::size()` (that reorder regressed `menu-enemy-cbrender`).

**CETrail reslist destructor loop:** The derived `reslist<CETrail::POINT>` deleting destructor is an inline template body, not a call to the base destructor. Directly spelling the sentinel traversal and ownership guard produces the exact `0xC0` function size and 98.85% static match; using a shared `trailClearListDerived` helper outlines the loop and regresses to `0xB8`/~77%. The remaining seven words are pure r4/r5 sentinel-loop coloring. Keep the readable inline loop and accept only after the `__dl__`/`__dla__` callee certificates are available.

**CViewRoot::create reslist reserve loop (~99.41%):** express the 128-node history clear naturally as `for (int i = 0; i < 128; ++i) mList[i].mNext = nullptr`. MWCC emits retail's eight-iteration CTR loop with 16 stores per iteration exactly; the prior hand-unrolled two-half form left 49 mismatches (four structural). Residual 14 words are one constructor-initialization Chaitin cycle: retail `(histVtTemp, capacity, pool1, pool2, historySentinel) = (r6,r9,r8,r7,r5)`, MWCC `(r5,r8,r7,r6,r9)`. Constructor-shaped helpers, placement construction, ABI-ordered inline parameters, declaration/assignment permutations, signedness, and `-O4,s` did not rotate it without structural regressions. Keep the natural loop and do not restore `.text` instruction patches.

**menu-bps-move Chaitin ceiling (~99.76%):** after r12 virtual ifaces + local size helper, residual is (1) party-fill GPR permutation `party/shift/actors` = retail `r21/r22/r23` vs MWCC `r22/r23/r21`, and (2) loop-hoisted FPR homes for `FB8/FA8/FC0/F90/F94` (+1 rotation on `{f27…f31}`). Ruled out: named FPR NVs, bias helpers, actorsBase-before-party, block-scoped party, IPA `fillActors`, fcmpu swap, `fullHpCount` move. Keep high-level C++; **no** `.text` `insn_patches`.

**menu-enemy-cbrender sort Chaitin (~99.172%):** keep exact size `0x274` with draw counter as `u32 i` and `order[static_cast<u8>(i)]` / `while (i < 0x18)` (u8 counter emits terminal `clrlwi`, +4). Sort: pass-before-order + block-scoped `s32 limit`, outer `for (left=0x17; left!=0; left--)` CTR, inner bottom-tested goto, `&indices[j]` rematerialize, depthB before depthA, XOR store-reload with `swapped=1` after the first store. That locks `order=r9`, `j=r12`, `swapped=r11`. Residual: `pass/limit/pair` = MWCC `r8/r10/r30` vs retail `r10/r5/r8` (pair should reuse vacant `r8`). Hoisting `j` before `order` gets `pass=r10` but pins `j` in `r8` and spills pair to `r30`. Dead `gap` before the loop is DCE'd and does not reserve `r8`. Equivalence blocked by unvalidated callees.

**menu-arts-cbrender bitfield-loop Chaitin (~99.3125%):** exact size `0x3C0`; gates + `unk1B8`/`unk80`/`unk98` draws are byte-identical. Residual is NV homes on the 8-slot (`unk318`/`unkA4`) and 9-slot (`unk310`/`unk314`/`unk104`/`unk170`) bitfield passes only: retail `i8=r28` / `i9=r27` with `bitI=r28`/`bit18=r29`, MWCC coalesces both counters into `r29` (`bitI=r27`/`bit18=r28`). `one=r31` and pointer `r30` already match. Ruled out: distinct `Layout**` walks (~94.8%), predeclared `i8`/`i9` (flat), early `i8=i9=0` overlap (~98.5%), `#pragma scheduling off` (~75%). Keep high-level C++; **no** `.text` `insn_patches`. EQUIVALENT blocked by unvalidated callees (`getInstance`, `func_800426F0`, layout draw helpers, `DrawInfo` ctor/dtor, etc.).

**occ-cull-helper dir-vector Chaitin (~99.829%):** keep `CPlane::isOnPositiveSide`
  + `CVec3::dot(mDir, unk24->unk10C - mPos)` / `lbl_eu_80667C8C` (0.0f pool rename OK).
  Residual six words: retail `r3=&delta(sp+0x14)` / `r5=cam` vs MWCC `r5=&delta` /
  `r3=cam`. Ruled out: scratch/dir/cam decl order (size +8, ~92%), named
  `CVec3::sub` (~99.6%), cam-alias / `CFrustum&` (~98%), two-stage copy (+size).
  Keep high-level C++; **no** `.text` `insn_patches`. Callee leaves
  `PSMTXMultVec` / `PSVECNormalize` / `PSVECCrossProduct` are now FULL+certified;
  remaining EQUIVALENT block is **`CPlane::set` 3-arg** (~79% soft-cap, SMT
  timeout even at 120s) — same psq-interleave class as occ-set-frustum side planes.
  Peak high-level body keeps **`CVec3::sub` temp+`set` wrappers** (not bare
  `nw4r::VEC3Sub`, not `operator-`): direct PS/nw4r paths regress to ~56–75%.
  **`mNormal = cross`** beats `cross(mNormal,…)`, component `set`, or field stores.
  Inline retail zero/`PSVECNormalize` or **`PSVECDotProduct`** also regress.
  Decomp is **8 bytes short** (`0x220` vs retail `0x228`); residual is prologue
  psq-interleave + post-cross stack shuffle + Chaitin coloring, not callee choice.
  Linked prove blocked until **`PSVEC*` certificates refresh** (stale
  `engine_hash` → no matched-callee lemma).

**cview-render-view leaf recovery (2026-07-23):** `getView1`/`getView2` are
  FULL_MATCH (`lwz` WORK_ID @+0x10/+0x14 then `b getView`). `getFrame2ViewOffset`
  is EQUIVALENT_MATCH (~96.5%) with cert. `fontFlush`/`func_8044BE38` are
  FULL_MATCH pure tails (`b` to `CDeviceFont::func_80452CF8` / `func_8044C1FC`);
  keep callee undefined or `#pragma dont_inline` so MWCC does not inline empty
  same-TU stubs into `blr`. `getScissorRect1/2` soft-cap (~68%/66%): declare `split` before `offset` so MWCC
  places split@sp+0x10 / offset@sp+0x8 (first local = higher addr). Exact retail
  size; residual is post-`bl` lha/sth interleave + overlap/clip Chaitin. Dead-result
  `getFrame2ViewOffset` required.

**`func_8043CAFC` / `func_8043FD10` / `func_8043E58C`:** CAFC is the dual-view
  probe shared with `getSplitLine`/`setSplitLine` (return hasView2); high-level C
  soft-caps ~87.5% on the same prologue interleave — use `extern "C" asm` like
  `getSplitLine` (not C++-mangled `asm int name(CView*)`, which emits
  `name__FP5CView`). FD10 is the render() client-rect + border expand helper —
  peak ~99.5% + SMT `EQUIVALENT` (plain field stores; `volatile` lha regresses).
  Cycle may label that `CODE_MATCH` even when `equivalence=equivalent`; promote
  to `EQUIVALENT_MATCH` when the cert is issued. E58C walks `mParent` via
  `convertToView` / `getInstance` accumulating `getFrame2ViewOffset`, then FD10
  for size (~87% schedule); avoid long `cycle` SMT on it.

**`func_8043CE90` / frame draw / GX scissor ring:** CE90 is `lbz` of
  `unk45C[8]` (FULL via tiny `extern "C" asm`). `func_804409D0` draws border
  quads (`begin(9,1)`/`add`/`end`) with `mFrameColor`×`lbl_eu_8066A318` then
  ×`lbl_eu_8066A2F4` (~54% first pass). `func_8044B298` copies optional
  `{u32,u32}` pairs into cache+0x4A8/0x4B0 (fix `C1FCCacheLayout` pad so
  rect4A8 lands at 0x4A8 — `pad1[0x4A8-0x49C]` wrongly started rects at
  0x4AC), finds tags 0xB/0xC with `for (n=size; n!=0; n--)` mtctr/bdnz, dual
  slot recompute for stb then stw, then `CE68(this+4, tag)`. Peak ~88%; CSE of
  `(this+4)` into saved r31 is the +4B over retail (unit split still PASS).
  `CE68` / `CEF8`: ascending `for (i = 0; i < mSize; i++)` (same as C1FC ring
  walk) → `mtctr` + `cmplwi`/`ble` (~88% / ~94%). Countdown `n != 0` stayed on
  `cmpwi`/`beq`. Residual CE68: retail early `mr r9,r3` (decomp late-copies at
  dispatch before `lwz r3,field7`) and match `bne+8; b` vs `beq` (size 140/144).
  `saved=self` + dispatch clobber does not force the early `mr` under `-O4,s`
  (CSE until field7). SMT still hits loop-iteration bounds. `isCurrent` and
  `isCurrentChild` are FULL+cert — mid-level gates are lbl/current only with
  `found=0` on the fail path (`bne` + `li r0,0` + `b`), **no** child/grand null
  checks before descending; recursive self-cert bootstraps via opaque EABI on
  the self edge. `hasCurrent` ~89% (size 104/96). Inlining `42DA8` without
  `poolPairAt` regresses ~94%→~72%; keep the helper. Ring EQUIVALENT needs CE68
  then B298 accepted+certified.

**CViewRoot ring helpers:** `func_80442B54`/`42C68`/`42DA8` push/pop three
  `{u32,u32}` rings in `mPool0/1/2` trailer (`mStartNodePtr`=base, `mList`=index,
  `mUsed`, `mCapacity`) then `CGXCache::func_8044B298`. Peak ~68%/81%/93% before
  schedule regressions; keep high-level divw shape (`sum - (sum/cap)*cap`). Certify of byte-identical callers must skip
  `validate_callee_contract` (path explosion on `getView`→`getWorkThread`);
  rebind certs after certifier-tree edits and recertify leaves bottom-up.



**PSQ certify `IndexError`:** `execute_instruction` used to treat PSQ D-form
  `disp` as `frB` and index `state.fpr[disp]` before the PSQ handler (offsets
  ≥32, e.g. MTX row loads). Handle `_FP_PSQ_OPS` in a dedicated branch. For
  byte-identical FULL_MATCH leaves, `certify_unit_symbol` skips SMT prove
  (incomplete PS capability stubs / timeouts) and synthesizes
  `full-instruction-match` certificates so parents can trust them.

**CView CMsgParam soft-caps:** `attachRenderWork` (~85%) and `setCurrent` (~78%) share the dual-/single-inline enqueue snap-load schedule ceiling. High-level `CMsgParam<10>` already matches frame/`stmw`/`stwux`/size `0x1E0`; residual is Chaitin snap-load coloring (~95/120 insn near-miss). Ruled out for attach: early tag/flag locals (flat 85%), `last()` refs (regresses ~64–67%), WorkID hoist (~83.9%), dual CtxSnap expansions (~72–76%). Prior FULL_MATCH used forbidden insn_patches or whole-function asm (rejected). SMT `EQUIVALENT` for attach times out even at 120s (`layout-feasibility deadline exceeded` on dual uninit-snap enqueue); `setCurrent` (single enqueue) certifies. Demote any ACCEPTED below EQUIVALENT/FULL.

**`view-set-current` certificate path:** SMT proves `equivalent` under `ppc-eabi`.
Certificate issuance used to fail on opaque-eabi validation reporting
callee-save spill writes (`r28`–`r31`). `_nv_spill_false_positive` in
`tools/coop/lib/equivalence_check.py` treats NV-only gaps as OK and strips them
from the callee summary. Private-fork promotion knobs in `coop.json`: `automatic_promotion`, live
`allowed_engine_sha256`, `allowed_confidence_tiers` including `C`,
`capability_assurance_shadow_mode`, and `require_clean_trust_boundary: false`.
ACCEPTED as `EQUIVALENT_MATCH` at ~78.2% static.

**CView::updateMsg (~76%):** size already `0x798`. Gains: three uninit `volatile CtxSnap` only (no fan-copy layer; was 74.4%), plus case 0/1 child fan-out via `CMsgParam<10>` enqueue. Residual: prologue still `-0x140`/`stmw r17` vs retail `-0x150`/`stmw r14`, snap homes `0x80/0x60/0x40` vs `0x48/0x28/0x08`, `switch` tree vs `bctr` jumptable (MWCC rejects GCC `&&label` computed goto). Same caller-stack class as attach/setCurrent — park under high-level C++.

**CUIWindowManager::Move (~84.96%):** peak keeps dual-cursor mark + store-then-SetRemove + poison scalar unlink; function size now `0x4DC` PASS (unit split spare). Residual: retail 8× `mtctr` unlink Chaitin. SMT times out under `ppc-eabi`. Ruled out: IPA `unlinkWindowNode` (~83.9%); CUICfManager goto 8× with dual-cursor (~40%); high-level `while`/`for` 8× threshold (~49%); SetRemove-first + `stwx` byteOff collect (~83.6%); iterator-mark + goto 8× (~40%); empty-guard poison (~83.9%); volatile pending w/o poison (~83.6%). Soft-cap park under high-level C++.

**CUIBattleManager::Move soft-cap (~95.57%):** unit size PASS; function `0xB2C` vs retail `0xB00`. Peak keeps indirect `func_8012F5F8` call (same-TU empty stub is IPA'd away on direct `bl`), pad0C chain, capacity-first slot loads, `framePad[8]`. Residual: decomp `-0x210`/`stmw r24` vs retail `-0x220`/`stmw r25` because `battleWorkEvent` `this+0x54` is CSE'd into an NV across the asset `readFile` loop (4th NV). Volatile `this` reload restores `stmw r25`/`-0x220` but fuzzy drops to ~94.4%. Slot `r8`/`r7` order and pathBuf homes (`0x178`/`0xF8` vs `0x158`/`0xD8`) ride the frame skew. `EQUIVALENT` blocked by unresolved indirect + unvalidated callees. Keep high-level C++; no `asm void`.

`CHelp_Pg::func_802B85A4` int→float uses retail `lbl_eu_80669000@sda21`; MWCC pools `@N` with the signed magic double — rename via `CHelp_Pg.o` `pool_patterns` `(MAGIC_HI, MAGIC_LO) → lbl_eu_80669000` once `.text` already matches.



---

### Core patterns — the 5 things that fix 90% of gaps

### 1. Relocation name drift (99.3–99.9%, instructions identical)

The #1 cause of near-misses: instructions are byte-identical but relocation **symbol names** differ.

#### 1a. SDA globals — `extern "C" lbl_eu_*` instead of `spInstance`

Retail uses `lbl_eu_806656A0@sda21`, not `spInstance__9CDeviceGX`. Thin `return spInstance;` is insn-identical but ~97.5% due to reloc name.

```cpp
extern "C" CDeviceGX* lbl_eu_806656A0;

CDeviceGX* CDeviceGX::getInstance() {
    return lbl_eu_806656A0;
}
```

Common singleton labels: `CDeviceGX` → `lbl_eu_806656A0`, `CDesktop` → `lbl_eu_806656AC`, `CProcRoot` → `lbl_eu_806655A0`, `CViewRoot` → `lbl_eu_806655D0`.

#### 1e. Lazy singleton — prototype >8 bytes to avoid `.sbss`

When a `getInstance` lazy singleton uses a `static` prototype variable (≤8 B, e.g. a virtual class with just a vtable pointer), MWCC places it in `.sbss` with SDA21 addressing (`li r3, obj@sda21`). **Retail** places the prototype in regular `.bss` with full 32-bit addressing (`lis r3, obj@ha` + `addi r3, obj@l`), causing:
- 4-byte size difference (1 SDA21 instruction vs 2 for lis+addi)
- Branch target shifts
- Reg-swap detection on `bne` offset

**Fix:** wrap the prototype in a struct with ≥12 B of padding (total >8 B) so MWCC places it in `.bss`, matching retail's lis+addi pattern:

```cpp
struct PaddedPrototype {
    mpfsys::MPFDrawBillboard obj;   // the actual prototype (4 B vtable)
    u32 padding[4];                  // ≥16 B extra → total >8 B → .bss
};
static PaddedPrototype lbl_eu_8056DC48;
```

**Return-value trick.** When the retail returns the ADDRESS of an SDA pointer (`li r3, ptr@sda21`) rather than its value, the C++ must use `return (T*)&ptr_var;` (address-of, not value):

```cpp
extern "C" { T* lbl_eu_806658A4; }
return (T*)&lbl_eu_806658A4;  // → li r3, lbl_eu_806658A4@sda21
// NOT: return lbl_eu_806658A4;  // → lwz r3, lbl_eu_806658A4@sda21(r0)
```

**Targets fixed:** `us-8047deac`, `us-8047ded4`, `us-8047d814`, `us-80480a58` (all `getInstance` in `mpfsys::MPFDraw*` / `UnkClass_8047CA88`).

**Always** prefer `extern "C" lbl_eu_*` in source first. Ctor/dtor stores should target the same label (don't dual-write `spInstance` + `lbl_eu_*` — bloats `.text`).

#### 1b. Float/double pools — `extern "C" const float lbl_eu_*`

A local `0.5f` literal compiles to byte-identical instructions but pools into TU-local `@N` → ~99.95%. Use the retail symbol directly:

```cpp
extern "C" const float lbl_eu_80667EB0;  // 0.5f deadzone

if (ml::math::abs(pad->mLStickXRaw) < lbl_eu_80667EB0) { ... }
```

**Declaration only** — do not define the constant in this TU. Use the symbol at each site (don't cache in a named local — that can spill an FPR and change frame size).

#### 1c. String pool offsets — force pool ordering

Retail `@stringBase0` offsets must match. A bare `unk1FC = ""` may pick a wrong empty hole. Fix: include all strings in a forced-pool sink, or use offset-from-known-string (`"CGameRestart" + 13`).

**Array-form seed (no extra .data).** `bta_hh_evt_code` (bta_hh_main.c, GC/3.0a5.2): the retail pool carries nine sibling-trace strings (sm_execute / state_name / hdl_event) before the 17 evt strings; the unmatched siblings pooled the wrong strings first, shifting every `addi r3,r4,imm`. A local

```c
static const char *const s_pool[] = { "wrong device handle: [%d]", "BTA_HH_NULL_ST", ... };
(void)s_pool;
```

at the top of the switch function pools all nine in retail order **and** is dropped as dead data by `-O4` (`.data` stays exactly retail size 0x2E0); the function then matches byte-for-byte including the 0x108+ evt offsets. Also: for a jump-table switch the pool order equals the **source case order** — the retail evt pool is DISABLE, ENABLE, OPEN, CLOSE, … (not enum order), so the cases must be written in that order.

**mr-before-store is a GC/3.0a5.2 schedule.** `port_find_dlci_port` (port_utils.c) under Wii/1.1 emits `addi; stb; mr r3,r9` while retail has `addi; mr; stb` (the increment-return tail). Switching the unit to `mw_version="GC/3.0a5.2"` reproduces retail byte-for-byte (same fix as bta_hh_api, KB ref:c257c09888). Also use real array members (`&rfc_cb.port[index]`, not raw `(u8*)&rfc_cb + idx*0xA4 + 0x68` arithmetic) — the array-member form fixed every Chaitin reg-swap in the loop.

#### 1d. Post-process rename (when source can't name the pool)

For MWCC's implicit pools (e.g. int-to-double `0x43300000` magic constant), `objcopy --redefine-sym` after compile:

```bash
powerpc-eabi-objcopy --redefine-sym=@2856=lbl_eu_8066A1D8 path/to/Unit.o
```

Reference implementation: `tools/postprocess_reloc_names.py` (wired into `coop run build/diff/cycle`). **Do not** rely on `objdiff.json` `symbol_mappings` — CLI-ignored (objdiff #279).

#### 1f. Lazy singleton with separate storage — return address-of SDA ptr

Same as §1e but for singletons where the pointer variable and the guard flag are both in SDA, and the prototype storage is a separate array. Pattern: guard check → set guard → return address of storage. Use `sizeof(T)+64` to exceed SDA threshold:

```cpp
extern "C" {
    s8 guard_flag;          // in SDA
    u8 storage[sizeof(T)+64];  // >8 B → .bss, accessed via lis+addi
}

T* getInstance() {
    if(!guard_flag) guard_flag = 1;
    return (T*)&storage;
}
```

**Targets fixed:** `us-80480a58` (`UnkClass_8047CA88::getInstance`).

#### 1g. Paired-single codegen — use nw4r SDK inline ASM helpers, not scalar C++

PS-heavy retail functions (psq_l/ps_muls0/ps_mul/ps_madd/ps_sum0/ps_sub/ps_add) are **not** reproduced by writing scalar `f32` math (MWCC emits lfs/fmuls and never pairs). They ARE reproduced by the nw4r SDK inline functions in `libs/nw4r/include/nw4r/math/math_types.h`, whose Metrowerks `ASM()` blocks emit exactly the retail sequences:

| Retail pattern | Use | Emits |
|---|---|---|
| `psq_l W0 + ps_muls0 + psq_st` ×2 (scale XY + Z) | `VEC3Scale(&v, pIn, s)` | XY pair scale + Z single scale |
| `psq_l pair + ps_mul + ps_madd + ps_sum0` (dot) | `VEC3Dot(pA, pB)` | YZ pair + X single, dot |
| `psq_l pair + ps_sub + psq_st` ×2 | `VEC3Sub(&v, pA, pB)` | XY diff + Z diff |
| `psq_l pair + ps_add + psq_st` ×2 | `VEC3Add(&v, pA, pB)` | XY sum + Z sum |

Key observations from `UnkClass_8047CA88::func_8047CC4C/CAA8` (us-80480c1c / us-80480a78, peaked 83.6% / 92.8%):
- `VEC3Dot(v, v)` (same pointer twice) is **CSE'd by MWCC** to 5 instructions (x-single `psq_l W1` + (y,z) pair), producing the classic `2x²+y²+z²` sum — retail matches this artifact exactly, so do **not** "fix" the double term.
- MWCC **reschedules instructions inside inline-ASM blocks** (e.g. hoists the Z `psq_l` before the XY `psq_st`) — the asm is not emitted verbatim, so don't panic when the block order differs from the header text.
- MWCC **reuses one stack slot** for a reused local `VEC3 tmp` across VEC3Sub/Scale/Add calls (matches retail's single sp+8 slot); separate locals balloon the frame (+0x20+).
- Scalar `x*x + y*y + z*z` reads of struct fields stay scalar; only the SDK asm helpers force PS.
- Residuals after this are pure Chaitin color/scheduling artifacts (FPR colors for constants, GPR target regs) — documented as unresponsive to source reshaping elsewhere in this file.

**Fv-with-hidden-params:** retail symbols ending `Fv` may still take args in r4/r5/r6/f1 (e.g. `func_8047CAA8__17UnkClass_8047CA88Fv(self, param)`). Implement with `extern "C"` + the exact mangled name and explicit params — the `bl` reloc name stays correct and MWCC passes the extra args normally.

**Billboard layered-renderer application:** `MPFDrawBillLayTex`'s billboard quad paths use the NW4R `VEC3Add` high-level helper to reproduce retail paired-single `psq_l`/`ps_add`/`psq_st` sequences. Scalar `Vec.x/y/z` additions compiled to separate `lfs`/`fadds` instructions and reduced `func_8047A330` to 22.9% fuzzy; replacing only the four corner sums with `nw4r::math::VEC3Add` raised it to 59.3% and kept the split within budget. Use the SDK/NW4R vector helper for PS-heavy geometry rather than scalar C++ arithmetic.

**MPFDrawDisplayList triangle-walker (us-80479384, 0x8C4):** a 2D map-grid height/color walker with s16 fast-cast height planes and NW4R PS color interpolation. Reusable findings:
- **s16→f32 via GQR5:** retail uses `psq_l fr, d(rA), 1, qr5` (the SDK `__OSs16tof32` / `OSs16tof32` fast cast from `revolution/os/OSFastCast.h`); scalar casts do not reproduce it. Include `<revolution/os/OSFastCast.h>` and call `__OSs16tof32(&t->h0)` directly.
- **u8→f32 double trick:** the 9 color-byte conversions all go through the `0x43300000` double-slot + `lfd`/`fsubs 2^52` trick; writing member assignments in **b, g, r order** (`c0.b = (f32)p->b; c0.g = ...; c0.r = ...;`) reproduces the retail's right-to-left conversion order (conversions b,g,r). The 2^52 double pools to a TU-local `@N` (value-equal reloc drift vs `lbl_eu_8066A7F8`).
- **Degenerate-copy block placement is worth ~10% fuzzy:** writing the s==0 fallback as `if (s != 0) { interp...; return true; } copy...; return true;` puts the interp as the fall-through and the byte-copy out-of-line — exactly the retail's `beq copy` layout — and raised objdiff fuzzy from 67.7% to 78.2% (HIGH_MATCH) vs the inverted `if (s == 0) {copy; return;}` form.
- **`cror eq,lt,eq / eq,gt,eq + bne` patterns:** MWCC emits the cror trick for strict `<`/`>` compares and direct `bge`/`beq` for `>=`/`==`; the exact mix is scheduling-dependent and mostly not source-controllable.
- **Path structure:** each of the three edge pairs has an x-weight (vertical crossing: `outSlot[i]`, `yCross[i]`) and a y-weight (horizontal crossing: final color) variant; the final block blends the two stored crossings against `pos.z`. `ml::math::abs` reproduces `fabs; frsp`; `lbl_eu_8066A7F0` (1.0f) must be referenced via `extern const f32` to avoid TU-local pool drift.
- **Unresolved:** the retail keeps two dead-store slots at sp+8/sp+0xC for the s16 fast-cast results (8 bytes) that shift every local slot; MWCC eliminates the equivalent stores in our build (register-pressure spill difference). The SMT probe times out on CFG exploration for this ~561-insn FP-heavy function.

**CException compiler-context breakthrough:** `CException::~CException` required TU-local `-O4,s` plus explicit `-func_align 4` to select MWCC's `stmw/lmw r30` save/restore pair; default `-O4,p` emitted four individual `stw/lwz` instructions and was 8 bytes too large. The same flags preserved the exact `__ct__CException` and leaf helper sizes. Keep this as a configure-level optimization context, not a source-level register trick. Files: `configure.py`, `libs/monolib/src/core/CException.cpp`; destructor `0x68`, `FULL_MATCH`.

**LOD fixed-trip matrix cursor loop:** In `UnkClass_8046368C::func_80463C24`, expressing the three matrix uploads as a high-level `while (i < 3)` with explicit `Mtx*` cursors (`workCursor` and `normalCursor`) produced the retail loop and exact `0x120` function size; the remaining 19 hexdiff mismatches were pure GPR color swaps. This is preferable to unrolling or low-level steering. Acceptance remained blocked only by the equivalence engine's symbolic MMIO abstraction around MTX/GX callees.

**Effect TU table/cursor reconstruction:** For `code_804DB938.cpp`, defining the shared sine/cosine BSS as `{value, delta}` entries and taking pointer locals reproduced the retail `slwi` + `add` cursor shape and exact split size for the table-driven rotation helpers. `ml::math::abs` (which wraps the NW4R `FAbs` intrinsic and returns through a float conversion) reproduced retail `fabs` + `frsp`; a direct `fabsf` call did not. Keep these as high-level helpers; remaining differences are compiler register-color/scheduling artifacts. Files: `libs/monolib/src/effect/code_804DB938.cpp`.

#### 1h. Variadic debug-print string literals — `extern char lbl_*` so SMT opaque-callee tokens unify

A debug-print call site like `DEBUGPrint("__wudInitFlushCallback() : %d, Init: %d\n", …)` is **instruction-identical** to retail but the string reloc symbol is MWCC's TU-local pool label (`@5061`) vs retail's named data symbol (`lbl_8056288C`). This does not stop a static match (99.5%), but it **blocks SMT EQUIVALENT_MATCH**: the variadic callee is opaque, its transition token hashes the full pre-call state (all GPRs + memory), and the differing format pointer diverges the token → every post-call fresh value diverges → `exit.target (0x01010100 != 0x00000080)` / `inconclusive_abstraction`, even though the functions are byte-identical.

**Fix:** reference the retail label instead of a literal (repo pattern already used in `WPAD.c`):

```c
void __wudInitFlushCallback(s32 result) {
    extern char lbl_8056288C[];
    DEBUGPrint(lbl_8056288C, result, _wcb.initState);
    _wcb.initState = 5;
}
```

The decomp reloc then has the same name as retail → same canonical symbol on both sides → tokens align → `EQUIVALENT UNDER CONTRACT` (no flag needed; the certified-callee path supplies the opaque-eabi contract for a FULL_MATCH callee like `WUD_DEBUGPrint`, or pass `--assume-relocated-callees` in the raw CLI). Same fix applies to any string pool the retail references by name (`lbl_805627BC`, `lbl_805629E8`, `lbl_80562D24`, …).

**Targets fixed:** `us-8037a990` `__wudInitFlushCallback`, `us-8037b730` `__wudShutdownFlushCallback`, `us-8037db00` `__wudCleanupStackCallback` (all 100% → FULL_MATCH).

**@N-pool variant (no named retail label):** when the retail references the literal via a TU-local pool label (`@688`) rather than a named `lbl_*` symbol, the `extern char lbl_*` trick has nothing to name. This is the archive-global vs per-TU numbering split (retail bte compiled as one `-ipa` unit), so the labels can never match from source. Fixed repo-wide by wiring the decoder's unused `canonical_symbols` hook in `tools/coop/lib/equivalence_check.py`: `_canonical_symbols_for_unit(unit.name)` reads the mined `tools/coop/retail_reloc_map.json` (`reloc-map mine`), maps each decomp `@N` label to the retail name for that unit, and `prove_unit_symbol` passes it to both `decode_block` sides — both sides then share one canonical reloc symbol, the opaque-callee token aligns, and the SMT probe returns `EQUIVALENT` even with pure reg-swaps present. No object patching; the map is regenerable. Targets fixed: `us-803050b0` `rfc_alloc_multiplexer_channel` (99.88% static, 2 lis-scratch reg-swaps → EQUIVALENT_MATCH, previously `exit.target 0x0 != 0x400` / `inconclusive_abstraction`). Remember to `reloc-map mine` after rebuilding the unit so the map carries the current `@N` numbers.

#### 1i. Automated reloc-drift detection + named-symbol map (`tools/coop/reloc_map.py`)

Instead of hunting reloc names by hand, run the detector/map miner (see also SKILL.md quick commands):

```bash
# Per-function reloc drift + concrete fixes (inline in hexdiff too)
python3 tools/coop/reloc_map.py diff <unit> --symbol <mangled-sym> [--no-build]

# Batch-mine the repo map from every retail/decomp objdiff pair
python3 tools/coop/reloc_map.py mine              # → tools/coop/retail_reloc_map.json
python3 tools/coop/reloc_map.py show --global-only
python3 tools/coop/reloc_map.py show --symbol spInstance
```

- Detection aligns relocs **per function pair** (matched by name, equal `.text` size) and classifies each diff: `name` (bytes identical — pure rename), `addend` (only the addend field differs — rename + offset), `layout` (same symbol, offset drift — string pools, report-only), `structural` (not reloc-fixable). Section-level alignment is **not** used: misaligned objects produce false positives (e.g. `WaitingForCoverOpen` → `WaitingForCoverClose` — same `lwz r0, X@sda21(r0)` word in shifted streams).
- The mined map keys named symbols globally (`spInstance__9CDeviceGX` → `lbl_eu_806656A0`, 29×) and TU-local labels (`@N` pools, `...bss.0` section relocs) per-unit (`unit@symbol`). Large addend deltas (> `MAX_MAP_ADDEND_DELTA`) are dropped as misalignment noise. Auto-mined entries reproduce the *legacy* hand-written `exact_renames` rules (e.g. `s_nandUserAreaCallbackFmt` → `lbl_80551294`, `s_nanderrPath` → `lbl_805512B8`), confirming the map is correct — but **do not add new postprocess rules**: object/reloc postprocessing is deprecated (PLAN.md §17.6), so the tool only points at the approved source-level `extern "C"` fix.
- `hexdiff` embeds the same analysis: the terminal output ends with a **Reloc drift** section listing each diverging reloc site with its source fix (the `extern "C"` declaration for name drift; inline/emit-the-symbol guidance for presence drift; builtin/expression guidance for type drift — and an EQUIVALENT_MATCH fallback note when the symbol is an implicit pool that can't be named in source); `--json` adds `reloc_drift` + `reloc_suggestions` keys.

#### 1j. Section-symbol anchors (`...bss.0` / `...rodata.0` / `...data.0`) — layout-driven, not source-renamable

MWCC emits a **section-symbol reloc** (e.g. `...bss.0`) for the base computation of a folded address group instead of the named symbol. Observed triggers:

- **Multiple same-section symbols fold into one base** (WUD `__wudSyncStoredDevInfoToNand`/`__wudSyncTryConnect`/`__wudDeleteHandler`/`__wudInitDevInfo`/…: `p = &_wcb` + `&_scArray.regist` both anchor off one `lis/addi`, retail names `__rvl_wudcb`, decomp emits `...bss.0`). A function that references exactly ONE bss symbol of the section gets the named symbol (`__wudSyncFlushCallback`).
- **The anchored symbol is NOT the first in the section** (SIBios `SITransfer`/`SIInterruptHandler`/`GetTypeCallback`: decomp `.bss` order is XferTime(0), TypeTime(0x20), **Packet(0x40)** — retail has **Packet(0x0)** first — so `&Packet[chan]` becomes `...bss.0 + 0x40` with the offset **baked into the instruction** (`addi r0, r31, 64` vs retail `addi r0, r31, 0`). The witness then fails at gate 3 `fields` (non-register bits), not gate 2.
- **`.bss`/`.rodata` emission order is NOT declaration order** — verified on SIBios: source declares `Packet` before `XferTime` yet the object emits `XferTime` first; `Alarm` is declared last but sits 4th. The order follows first-reference/use order across the TU's functions, so reordering declarations is a no-op; only reordering the function bodies (matching retail `.text` order) would flip it, which risks every sibling FULL_MATCH function in the unit (their named-symbol relocs are order-independent — only the section-anchored functions change). **Do not chase**; the mined map canonicalizes `...bss.0`/`...rodata.0` → retail name for gate 2, and the static match is unaffected (values equal). Same cap for rodata anchors (`HBMAxSound SetVolumeAllSeq`: retail anchors the float pool to `WT_FILENAME__24@unnamed@HBMAxSound_cpp@` at `.rodata` offset 0, decomp emits `...rodata.0`).

#### 1k. Float-pool ordering is TU-wide — per-function literals are not fixable

`HBMAxSound SetVolumeAllSeq` (us-803252a0): the three float constants pool at the SAME offsets (40/44/48) on both sides but the **values are permuted** (retail `[0.0f,10.0f,20.0f]`, decomp `[10.0f,20.0f,0.0f]`), so every `lfs fX, imm(r31)` at the same slot loads a different constant → witness gate 3 `fields`. The pool is TU-wide (other functions' constants interleave), so §1b's `extern "C" const float` route is wrong here (it switches to SDA21 `lfs @sda21`, changing the instruction shape). Record as blocked.

#### 1l. Retail's volatile-scratch address setup vs decomp's direct color — witness ABI-boundary cap

`__wudLinkKeyEventStackCallback` (us-8037e5b0) — the ONE gate-2-fixable case in the reloc sweep: retail sets up `pMsg = _wudWiiRemoteDescriptor` FIRST (`lis r30`), then `p = &_wcb` through a **volatile scratch** (`lis r4; addi r28, r4`); decomp colored the second address directly into a callee-saved reg. Swapping the source declaration order (`char* pMsg = …; WUDCB* p = …;`) makes MWCC set up `pMsg` first — the reloc symbols align per slot (gate 2 passes, explicit addends 0=0; the residual `addend delta +800/+57` is only in the in-place RELA words, irrelevant to gate 2) and reg_swaps drop 79→65. But the retail `r4` scratch maps decomp r29 → gate 5 `abi-boundary` rejects, and the region-sliced witness only triggers on gate-4 `rho` failures, never gate 5. General rule: **any retail use of a volatile GPR as a transient address scratch that MWCC colors into a callee-saved register is a hard witness blocker** (also `__OSPlayTimeFadeLastAIDCallback` r3→r5, `__HBMSYNSetupVolumeEnvelope` r3→r4, `btu_hcif_process_event` r4→r5, `process_service_search_attr_req` r0→r4, `btm_sec_mx_access_request` r3→r4, `l2c_csm_config` r5→r3, `add_attr` r4→r21).

#### 1m. `byte << 8` rlwinm mask width — range-analysis folded, resists cast/intrinsic

`hcisu_h2_receive_msg` (us-802df410): retail emits `rlwinm rD, rS, 8, 8, 23` (mask 0x00FFFF00) for `(UINT16)((byte << 8) + cb->last_byte[...])` while the decomp emits `…, 8, 16, 23` (0x0000FF00) — semantically identical for an 8-bit `byte`. Resisted all four bounded variants: `UINT16 byte` (loads become `lhz` → 6 structural), `(UINT16)byte << 8` pre-cast (no-op), `(UINT16)(byte << 8)` post-truncate (regresses the sibling site), `DECOMP_PPC_RLWINM(byte, 8, 8, 23)` (optimizer range-folds the mask back to 0xFF00 because `byte` ∈ [0,255]). The mask choice is scheduler/range-driven — a sibling site in the SAME function emits 8,8,23 naturally. Record as blocked.

#### 1n. Witness coverage notes for reloc sweeps (2026-08 sweep, 28 RVL functions)

- With the mined map refreshed, **gate 2 passes for every TU-local `@N` ↔ `@M` and `@N` ↔ section-symbol drift**; the sweep's "fails ONLY at gate 2" premise is superseded — the real blockers are gates 3 (`fields`: rlwinm masks, bc displacements, baked pool/bss offsets), 4 (`rho`: local conflicts, e.g. `btm_sec_l2cap_access_req` region [24,34), `l2c_rcv_acl_data` [95,97)), 5 (`abi-boundary`: volatile-scratch rotations, see §1l), and the loop predicate (`btm_acl_role_changed`, `bta_dm_pm_cback`, `process_service_search_attr_rsp`, `SDP_DeleteAttribute`, `btm_event_filter_complete`, `btm_process_inq_complete`, `__wudDeleteHandler` — jump-table `bcctr` switches and backward branches are hard-rejected by the region-sliced first-cut loop predicate).
- **i2f magic-double pools remain the only genuine gate-2 blockers** after re-mining: `AXFXReverbStdExpCallback` (`double_8066BE60` vs `@1000`), `AXFXReverbStdExpCallbackDpl2` (`double_8066BEA0` vs `@1087`), `__dt__Q36nw4hbm3lyt7TextBoxFv` (slot 43 `reloc presence differs`: retail bakes the 2^52 magic offset `lfd f1, 0x5430(r3)`, decomp pools it) — all §7i unfixable in high-level C.

### 2. `extern "C"` on `bl` targets with retail mangling

MWCC emits `bl` to **exact linker symbols**. C++-mangled names on callees cause wrong relocs.

```cpp
extern "C" {
    CViewRoot* getInstance__9CViewRootFv();
    void invalidCurrent__9CViewRootFP5CView(CView* view);
}
```

### 3. Guard variables: `s8` not `u8`

Retail uses `lbz` + `extsb.` + `bne` (signed-byte test with record bit). `u8` compiles to `lbz` + `cmpwi` — a 1-insn diff.

```cpp
extern "C" s8 lbl_eu_80665580;  // signed char

MTRand* MTRand::getInstance() {
    static MTRand instance;
    if (!lbl_eu_80665580) {
        instance.srand(0x012BD6AA);
        lbl_eu_80665580 = 1;
    }
    return &instance;
}
```

Also: remove non-trivial constructors from headers when only `getInstance()` constructs the object — avoids compiler-synthesized `@GUARD@` symbols.

### 4. Regalloc: declaration order and block scoping

MWCC maps locals to registers in **declaration order** (first → higher callee-saved reg). When retail uses r31 for `hasView2` and r30 for `hasView1`, declare `hasView2` first.

When the **same pattern repeats with opposite regalloc order** in retail, give each occurrence its own block-scoped locals:

```cpp
// First gate: r30 then r31
{
    u32 hasView1a, hasView2a;
    hasView2a = 0; hasView1a = 0;
    ...
}

// Second gate: r31 then r30
u32 hasView1, hasView2;
hasView2 = 0; hasView1 = 0;
...
```

Don't reuse one variable pair across both gates — no single declaration order satisfies both.

### 5. Stack frame: multiple small types vs one large type

MWCC may align/pad structs on the stack differently than retail. When retail uses two `getFrame2ViewOffset` homes at `sp+0xC` and `sp+0x14`, a single `ml::CRect16` local may be 8-aligned leaving a hole.

Fix: use individual components (`ml::CPnt16 splitSize, splitPos, normalSize, normalPos` + a `{u16 x; u16 y;} modeSize`) and cast to `CRect16*` for helpers.

### 6. Struct layout before logic

Fix offsets in headers before tuning C++ shape. Wrong layout causes branch/frame divergence even when control flow looks right.

#### 6a. Struct-ifying raw byte-offset arithmetic in SDK C — byte-identical when the layout is exact

For match-pinned SDK C TUs (`libs/RVL_SDK/src`), raw offset arithmetic like `*(u32*)((u8*)p + 0x04)`, `(u8*)base + idx * 0x18`, and `*(s32*)((u8*)synth + 0xAC + (ch << 2))` **can usually be rewritten as struct field access with byte-identical codegen** — verified with a direct MWCC probe (`.scratch/struct_vs_raw.c`, Wii/1.1 `-O4,p`): field access, unrolled word copies, and 2D grid indexing `cell[row][col]` all compiled to exactly the same `lwz rX,N(rY)` / `slwi` / `add` sequences as the raw forms. This is the approved readability cleanup for the RVL report (`docs/CODE_SMELLS_RVL.md`, informational): the report's ptr-arith/deref-arith rows dropped **132→92 / 24→13** across two 2026 passes — pass 1: synvoice, seq, synmix, synctrl, mix/remote, mix, GXFifo, bta_hh_act, OSPlayTime; pass 2: sdp_db, WUD, OSLaunch, rfc_utils, port_utils, AXAux + a second GXFifo site. Every changed function was hexdiff-verified byte-identical (16 TUs, zero regressions).

**Rules that make it safe:**

1. **Verify `sizeof` of every invented struct against the retail stride.** A struct with the right start offset but wrong total size silently changes codegen — no warning, just different bytes. Concrete bug: `typedef struct { u8 data[0x0C]; u16 sampleIndex; } K` is 14 bytes, but the retail keygroup stride is 0x10 — MWCC emitted `mulli r0,r0,14` where retail had `rlwinm r0,r0,4,0,27` (stride 16), cascading a whole-function register shift (synctrl.c `__HBMSYNNoteOn` 100%→34%). Fix: add explicit trailing padding `u8 _pad[0x10 - 0x0E]` — never rely on MWCC's implicit alignment padding. Cross-check with `-align powerpc` sizing: run the same probe file through `mwcceppc` and read the emitted `mulli`/`rlwinm` multiplier.
2. **MWCC has no flexible array members** (C99 `t[]` aborts with a parse error). The BT-stack "payload after header" idiom `(u8*)(p_buf + 1) + p_buf->offset` and variable-stride walks (WPAD `i * fmtSize`, OSPlayTime fade `idx * 0x240` anchored as `&fade->buf[idx * 0x240]`) stay byte-pointer forms; a fixed-size array field with the correct start offset is fine (bounds don't affect codegen, only `sizeof`/offsets do).
3. **hexdiff-verify every function, revert on any regression.** Struct access is *usually* byte-identical, but two known fragile shapes perturb MWCC regalloc/scheduling and must stay raw:
   - **Member arrays used as call args across calls** (synctrl.c `synth->pan[channel]` / `synth->ctrl[channel]`): the member form shares the `synth + ch*4` subexpression with a sibling `instrPtr[channel]` and gets cross-call CSE'd into a callee-saved register, missing retail's `clrlslwi` re-materialization → whole-function register shift (see the synctrl.c note above for the byte-form fix).
   - **Packed-output macro blocks** (synsample.c `O_W32/O_WU32/O_W16` writing a byte-buffer at fixed offsets): a typed out-struct rewrite perturbed the store schedule (retail `stw` before the `addi base+2`; struct form hoisted the add) → 0→2 structural + 28→44 reg-swaps. The macro form is the match shape.
   - **Struct-assignment / `memcpy` for an unrolled word copy** (GXFrameBuf.c `GXAdjustForOverscan` 60-byte copy): `memcpy(rmout,rmin,sizeof)` compiles to a call (0x15c→0x104) and `*dst = *src` on a `u32[15]` word-view struct emits a different store schedule (0x15c→0x134); the retail shape is the unrolled 15×`lwz`/`stw` — keep it.
4. **Not struct-able at all** (leave raw): fixed hardware addresses (`*(u32*)0x80003194`, `__piReg`), cacheline/page alignment math (`((u32)buf + 0x27) & ~0x1F`, `dst += 0x1000`), `container_of` (mem_list `OBJECT_GET_NODE`), dynamic-string layouts (scsystem `raw + item->nameLen + 1`), **cross-boundary word reads** (btm_acl's `*(u32*)((u8*)p_acl_cb + 0x10) == 4` straddles `remote_dc[2]`+`remote_name[0..2]` — not a field, and the local struct may mislabel it), **suspect header layouts** (WUD's 16-byte `LINK_KEY` memcpy from `SCDevInfo+0x14` while the header puts `linkKey` at 0x20 — the header's own offsets are uncertain, so anchor nothing to it), and deliberate raw member reads (ut_TextWriterBase `*(int*)((u8*)this + 84)` where the header layout is already annotated — the raw form is the layout-risk mitigation).
5. **Prefer retail-header types when they exist** (AXVPB/AXPB/BT_HDR/GXFifoObjImpl): `*(s32*)((u8*)ch + 0x18)` → `ch->index`, `*(u16*)((u8*)ch + 0x38)` → `ch->pb.state`, `*(u16*)((char*)buf + 2)` → `((BT_HDR*)buf)->len`. Mirror sibling TUs' structs when the TU lacks the header (synmix/synctrl/synvoice each keep a local partial `HBMSYNSYNTH`; keep offsets consistent).

**Pass-2 lessons (the cheap wins are the ones that were there all along):**

- **Look for the field in the TU's own structs before inventing anything.** sdp_db.c already had `tSDP_RECORD`/`tSDP_ATTRIBUTE` (num_attributes@0x08, attr_id@0x08) — the raw reads became `record->num_attributes` / `attr_ptr->attr_id` / `&record->attribute[i]`; port_utils.c already had `RfcControlBlock.port[5]` at 0x68 — `port_ptr(idx)` became `&rfc_cb.port[idx]`; WUD.c's `WUDCB` already had `stdDevs[]`@0xE4 / `smpDevs[]`@0x4A4 — `(WUDDevInfo*)((u8*)&_wcb + i*0x60 + 0xE4)` became `&_wcb.stdDevs[i]`; rfc_utils.c's `struct RfcPortStruct` was defined with pads covering exactly the raw-read bytes — split `pad_00[0x0D]` into `pad_00[0x01]` + `in_use`@0x01 and `pad_25[0x47]` into `pad_25[0x43]` + `field_0x68`@0x68, and replace the byte-array `tle_0x70[0x18]` with the already-typed `TimerEntry tle` (0x17 padded to 0x18 — aligns the next field). 7 sites → 0, functions stayed 100%.
- **Fixed-offset buffer overlays work when the math is exact.** OSLaunch.c: `NANDBootInfo` is 0x1020 and sits at `buffer+0xFE0` in a 0x2000 allocation — `0xFE0 + 0x1020 = 0x2000`, so `typedef struct { u8 _pad[0xFE0]; NANDBootInfo bootInfo; } OSLaunchBuf;` is a zero-slack overlay; the cast becomes `&((OSLaunchBuf*)buffer)->bootInfo`.
- **2D-array anchoring**: AXAux.c `(void*)((u8*)__AXBufferAuxB + pos * 0x600 + 0x180)` → `&__AXBufferAuxB[pos][0x180]` (the buffer was already declared `u8[AUX_RING_MAX][DPL2_FRAME_SIZE]` with DPL2_FRAME_SIZE = 0x600). Don't use the named channel constants here — the TU's `DPL2_SAMPLE_*_BEGIN` names disagree with the function names (`__AXGetAuxBOutputDpl2Ls` reads 0x300 ≠ `DPL2_SAMPLE_LS_BEGIN`); plain indices avoid propagating the mislabels.
- **Re-scan after edits.** The first GXFifo pass fixed only one of two identical `*(u32*)((u8*)dst + 0x20) = *(u32*)((u8*)realFifo + 0x20)` sites (CPU fifo vs GP fifo variants); a fresh `smell_report.py --rvl` run surfaced the second. Run the detector again after each batch — the same pattern often repeats at a sibling site.

### 7. Small C functions: branch inversion, signed compare, and instruction ordering

MWCC behaviour with small C (not C++) functions — common in SDK/middleware like CRI ADX.

#### 7a. Branch inversion: `== NULL` → `bne`, `!= NULL` → `beq`

MWCC inverts the branch condition for null-pointer checks:

```c
// Generates bne (branch if not-equal) — matches retail
if (self == NULL) return 0;

// Generates beq (branch if equal) — does NOT match retail  
if (self != NULL) { body }
```

**Pattern:** use `== NULL` (equality) to get `bne`-skip matching retail's pointer-null guard. The inverse (`!= NULL`) generates `beq` which is structurally equivalent but bytes differ.

#### 7b. Signed vs unsigned compare: `s32` for `cmpwi`, `u32` for `cmpli`

MWCC uses `cmpwi` (signed compare immediate) for `s32` comparisons and `cmpli` (unsigned) for `u32`. Retail sometimes uses one or the other. Match by choosing the correct signedness:

```c
// Retail uses cmpwi r4, 8 — use s32
void SFBUF_SetTermFlg(void* buf, s32 idx, u32 flg);

// Would generate cmpli (unsigned) — mismatch
// void SFBUF_SetTermFlg(void* buf, u32 idx, u32 flg);
```

#### 7b2. Signed vs unsigned modulo: `s32` for `divw`, `u32` for `divwu`

MWCC's `%` operator on `u32` generates `divwu` (unsigned divide) for the modulus, but on `s32` generates `divw` (signed). When the modulo result feeds a pointer/memory offset via a sequence of `divw` → `mullw` → `subf` (remainder), the signedness of the types determines `divw` vs `divwu`:

```cpp
// Retail uses divw — fields must be s32
u32 idx = (layout->hashAccum + layout->hashCount) % layout->hashDivisor;

// Fix: declare the struct fields as s32, not u32
// s32 hashAccum, hashCount, hashDivisor;
```

Symptom: one byte diff at the `%` → `divw`/`divwu` instruction. All other arithmetic matches.

#### 7c. Instruction ordering: `li` before `stw` soft-cap

MWCC aggressively schedules `li rX, 0` (setting up a later call's argument) **before** an intervening `stw` that writes through the same pointer. No C-level barrier prevents this:

```c
*(u32*)self = 0;
memset(self, 0, 0x3c);
// MWCC emits: li r0,0; li r4,0; stw r0,0(r3); li r5,0x3c; b memset
// Retail has:  li r0,0; stw r0,0(r3); li r4,0; li r5,0x3c; b memset
```

`volatile`, expression nesting `((u32*)self)[0] = 0`, and data dependencies via `memset(self, *(u32*)self, …)` all fail to flip the schedule. Accept as a soft-cap; equivalence proves EQUIVALENT when no callee-register dependency exists.

Confirmed on `libs/CriWare/src/sofdec/sfdcore/sfd/sfd_ply.c` (`SFD_Start`, `SFD_TermSupply`): the same float appears for a **return-constant** `li r31,0` (result=0) vs a following `stw r0, 0x50(r30)` store. Tried 8+ shapes (statement order both ways, goto-out structure, declaration order, `-O3`/`-O4,s`/`-ipa file`, `#pragma scheduling off` — fixes the float but regresses prologue address-const hoisting, `#pragma optimization_level 3`, val locals, `result+1` dependency — all fail). Both functions were at 95–97% CODE_MATCH; EQUIVALENT_MATCH additionally blocked by the `has_indirect_calls` gate (vtable trace calls via `bctrl`), so only FULL_MATCH (100%) can accept — out of reach via byte-identity for now (soft-cap). Source-level attempts here have plateaued; revisit after a tooling/engine change.

**7c2. Unit compiler mismatch is the real fix for many 7c floats (RVL_SDK bte → GC/3.0a5.2):** before treating a `li`-vs-`lis` / `mr`-vs-`stb` schedule float as an irreducible soft-cap, sweep the **other MWCC family** for the whole unit. `libs/RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act.c` was configured `Wii/1.1` (mwcc_43_151) but the retail bte was compiled with **GC/3.0a5.2 (mwcc_41_60831)**: every Wii/1.x and GC 1.x-2.x build hoists `lis r3, bta_dm_cb@ha` above the mask-store `li r0,1` (rssi/link_quality/new_link_key cback dispatch), while GC/3.0a3/3.0a5/3.0a5.2 emits the retail order `li r0,1; lis r3; stb` byte-for-byte. Per-object fix: `Object(NonMatching, "…bta_dm_act.c", mw_version="GC/3.0a5.2")`. Verified zero regressions: 29/56 functions at 100% under GC vs 24 under Wii/1.1 (all previously-accepted stay 100%; `bta_dm_search_start`/`bta_dm_send_hci_reset` newly 100%). Quick probe recipe: compile the minimal reproducer (`bdcpy` + byte-store + `lis`-based indirect call) with each `build/compilers/*/mwcceppc.exe` and diff the `bl`-follow sequence; also sweep `-O3/-O4,p/-O4,s` and `-ipa file` on/off. Residual soft-cap that no compiler version fixes: `bta_dm_compress_cback`'s hoisted `mr r7,r31` (p_srvc copy for trace-log p3 base, 17+ source shapes tried) — semantically trivial, provable by SMT once the LogMsg callee chain (us-802e0830) is accepted.

Same fix applies to **`btm_sec.c`** (all 10 functions in the unit matched at 100% only after `mw_version="GC/3.0a5.2"`): under Wii/1.1 (1) `{0xff,0xff,0xff,0xff,0xff,0xff}` BD_ADDR init coalesces to `lwz/lhz` from the pool vs retail's six `lbz`+`stb` (mkey_comp_event), (2) the `li r0,0; stw param` vs `addi r3` schedule after `btu_stop_timer` inverts (PINCodeReply/disconnected), (3) 8/10 functions matched under both. Also: a `#pragma auto_inline off` stub for the not-yet-recovered `btm_sec_execute_procedure` is required so `-ipa file` doesn't inline the empty body and drop the `bl` call sites (encrypt_change/collision_timeout); `__attribute__((noinline))` is **unsupported by mwcc_41_60831**.
Same fix verified on **`gap_api.c` (`GAP_Init`, us-802f2654)**: under Wii/1.1 the four-instruction block after the hoisted `lis r5,cback0@ha; lis r3,cback1@ha` pair comes out `addi r4,r31,gap_cb@l; li r0,5; addi r5,…; addi r3,…` (LHS + const before the callback addis), while retail is `addi r5,…; addi r4,…; addi r3,…; li r0` (callback addis then the const, per-statement RHS-before-LHS order). Plain Broadcom source (`memset(&gap_cb,0,sizeof)` + three member stores + `gap_conn_init()`) with `mw_version="GC/3.0a5.2"` on the object reproduces retail byte-for-byte (24/24, 0 structural, split 0x0 spare; FULL_MATCH accepted).
Same fix verified on **`gap_conn.c`** (us-802f2724 gap_connect_ind, us-802f2db4 gap_disconnect_ind, us-802f30bc gap_congestion_ind, us-802f2a8c gap_config_ind — all FULL_MATCH): `mw_version="GC/3.0a5.2"` + `-func_align 4` closes the `mr r4`-before-`stb` schedule float on the accept-the-connection store (Wii/1.1 emits `stb` first), and `-func_align 4` alone removes the spurious `nop` MWCC inserts before every `mtctr`-counted (unrolled-×4) find loop. Two source-shape notes for the GC build: (1) the tx-queue drain must be written `while (p_ccb->tx_queue.p_first != NULL) GKI_freebuf(GKI_dequeue(&p_ccb->tx_queue));` — the `(p_buf = GKI_dequeue(...)) != NULL` form emits the rotated while with an `r3` result test, retail reloads the queue head at the bottom (`lwz` at `+0x14`); (2) the MTU-acceptance check must be inverted (`if (!p_cfg->mtu_present || p_cfg->mtu > MAX) rem = MAX; else rem = p_cfg->mtu;`) to reproduce the `beq`-to-inline-`li 0x69b; sth` / `ble`-to-out-of-line-`sth` layout. `gap_find_conn_by_cid` needs `static __inline` (plain `static` leaves a 0xB8 standalone copy that blows the 0xB3C split by exactly 0xB8; retail inlines it at all 8 call sites).
#### 7d. Register allocation for small C functions
For simple C functions with few locals, MWCC's Chaitin allocator may differ from retail:

- **Extra unused param** (`void f(void* self, u32 unused, u32 addend)`) can push the third argument into `r5` matching retail where `addend` naturally lands. The middle param is dead but occupies `r4` so the active value goes to `r5` (same as retail).
- **Global function pointers** (`lbl_eu_*`: `extern void (*lbl)(void)`) may load the symbol address into a different register (`lis r3` vs retail `lis r4`). The reg-swap is harmless for leaf void functions but causes `not_equivalent` in SMT when `r3` is live-out (the equivalence checker treats it as an observable). Use `extern u32 lbl_eu_*[]` + manual cast if register pressure is high, though this rarely changes the allocation.

#### 7d2. State-machine dispatch: goto-chain, not `switch` (CriWare sfd_ply, FULL_MATCH ×3)

For sparse value dispatch (player status / flags / condition code), MWCC's `switch` lowering emits a **balanced compare-tree** (`cmpi root; beq; bge; …`) even for 2–4 consecutive cases; retail is a **linear equality chain with bodies appended after the chain**. `if-else-if` emits bne-skips with inline bodies (wrong layout). Use an explicit **goto-chain** with the case bodies after the tests:

```c
if (flags == 1) goto case1;
if (flags == 2) goto case2;
if (flags != 3) goto caseDefault;   /* last test NEGATED: `if (c) goto A; goto B;` emits `bne B; b A`; the negated form emits retail's `beq A; b B` */
goto case3;
case1: result = 1; goto done;
case2: result = 2; goto done;
case3: /* complex body */ goto done;
caseDefault: result = 3;
done: …;
```

Verified FULL_MATCH: `criware_803C9FC0` (range check `(unsigned)(st-2) <= 2` + equality), `fn_803CC238` (avFlags 1/2/3), `sfply_IsEtrg` (cond 1/2/3/0). Also: `result = 0` must be **initialized after** any early-return guard (retail `li rX,0` sits at the first use point, not the declaration); and the last dispatch test must be written **negated** (`if (x != N) goto default; goto caseN;`) to hit the branch-over-branch peephole — the direct form emits `bne default; b caseN` instead of retail `beq caseN; b default`. **Compiler note:** these dispatch notes (negated form, `SFLIB_SetErr` returns) hold under the *default GC/3.0a5.2* compiler. The retail `sfd_ply.c` was built with a **Wii-family compiler (configure `mw_version = "Wii/1.1"`)**, which changes two things: (1) store scheduling — the retail `li r0,1; stw r0, off(rX)` order (vs GC's `li r0,1; li rX,0; stw` delay) matches only on Wii; this is what unlocks `SFD_RelFrm`, `SFD_RequestStop`, `SFD_Start`, `SFD_TermSupply`, `SFPLY_Init` at 100% (all eight Wii/1.x versions match, all five GC versions fail). (2) dispatch polarity — under Wii/1.1 the **direct** form (`if (x == N) goto caseN; goto default;`) reproduces retail `beq caseN; b default`; the negated form emits `bne default; b caseN`. `SFD_GetFrm`/`SFD_Stop` remain soft-caps: their retail `beq next-block; b target` (conditional targeting the *immediate successor*) is merged to `bne target` by every MWCC version (GC and Wii) regardless of source shape; GetFrm additionally has a 3-way callee-saved rotation (retail self=r29/outFrm=r30/result=r31 vs MWCC outFrm=r31/self=r30/result=r29 — prologue `li result` lands after the parameter moves).

**2026-08 sweep: the whole Sofdec family (sfd/sfh/sfx libs) is Wii/1.1 retail.** Beyond `sfd_ply`, these units were all configured GC/3.0a5.2 and are retail Wii/1.1-built: `sfd_aoap`, `sfd_buf`, `sfd_mps`, `sfd_pl2`, `sfd_mpvf`, `sfd_mpv`, `sfd_pts`, `sfd_see`, `sfd_set`, `sfd_trn`, `sfd_uo`, `sfd_seeki`, `sfd_con`, `sfd_hds`, `sfd_lib`, `sfd_mem`, `sfd_adxt`, `sfd_tmr`, `sfd_tst`, `sfh_ver1`, `sfh_ver2`, `sfh_local`, `sfh_main`, `sfx_lib` → `mw_version = "Wii/1.1"`. Verified wins: SFAOAP_Create 10→100% (unit 0 ✗), SFBUF_VfrmAddRead 92→100%, SFMPS_Init 90.9→100%, SFD_Standby 100%, SFVOM_GetRead 92→100% (unit 0 ✗), SFX_Destroy 91.3→100%, VER1_IsSfdHeader 80→100%, VER2_AnlyPackType/PketSizLen 50→100%, VER1_AnlyHdrSfhVer/ModuleVer 50→100%, VER2_AnlyElemChNum/SmpHz/AnlyFtrFixFlg/ShcFixFlg 100%, and **SFHLOCAL_GetNbyteL** (previously documented as a permanent auto-unroll soft-cap!) 100%. Probe recipe: compile the small function with each `build/compilers/*/mwcceppc.exe` and diff; the li-vs-store schedule and the dispatch polarity (7e) both flip between GC and Wii.

**CriWare `SFLIB_SetErr` returns the error code (s32):** error-handler paths like `SFD_ExecOne`/`SFD_Start`/`SFD_TermSupply` `return SFLIB_SetErr(0, 0xff00xxxx);` (retail reuses r3 = SetErr result for the epilogue, no `li r3,0`). Declaring it `void` produces an extra `li r3,0; b epilogue`. `SFD_ExecOne` (FULL_MATCH) shows the shared-epilogue form: the SetErr path `b` jumps straight to the epilogue restores, skipping the else-path's `li r3, 0`.

#### 7e. s64/s64 locals: struct field access, not `<< 32 |` construction (CRI SFTMR_AddTsum)

For 64-bit loads/stores, write them as **struct field access** — `s64 x = t->field;` emits two plain `lwz` and lets MWCC keep the pair in two registers. Building the value as `((s64)hi << 32) | lo` creates hidden construction temps (shift/or vregs) that shift MWCC's vreg numbering, producing a *perfect schedule* but a consistent register permutation (e.g. min/max pairs landing one register higher) — 14/37 pure reg-swaps at 98.1%.

`SFTMR_AddTsum` (us-803d1a00, FULL_MATCH 100%): retail is a 64-bit accumulate + running min/max of deltas, with the ternary branch shape `beq body; b end; body: moves` for the max merge. The exact retail codegen comes from:

```c
typedef struct SFTMR_Tsum { s64 tsum; s64 min; s64 max; u32 num; } SFTMR_Tsum;

void SFTMR_AddTsum(SFTMR_Tsum *tsum, void *unused, s64 delta) {
    tsum->tsum += delta;
    tsum->min = (delta < tsum->min) ? delta : tsum->min;
    tsum->max = (tsum->max < delta) ? delta : tsum->max;
    tsum->num++;
}
```

Notes: the middle `void *unused` param is real (callers pass garbage in r4, delta arrives in r5:r6); the `?:` ternaries produce the phi-merge copies (`or r10, r6, r6` style) and the two-branch max layout; `if`-statements instead of ternaries move the min stores 4 instructions later (structural mismatch).

#### 7f. `!x` vs `x == 0` in if-return guards — logical-not normalization (CRI Sofdec mwply)

MWCC normalizes a bare logical-not condition (`if (!x) return B;`) into the
positive-condition layout, so `if (!x) return B; return A;` compiles the SAME
as `if (x) return A; return B;` (then-block inline, `beq`/`bne` toward the
out-of-line return). Retail often keeps the OTHER layout (false-path inline,
positive body at the jump target). Write the guard as an explicit comparison
(`== 0` / `!= 1`) to preserve retail's branch direction:

```c
// Retail: bne → body; li r3,0 inline; b end; body at target
if (MWSFD_IsFsBdr(h) == 0)      // ✅ matches (explicit compare)
    return 0;
return !!MWSFSVR_IsSvrBdrHndl(h);

// Compiles to the mirrored layout (beq → li, body inline) — 8 mismatches
if (!MWSFD_IsFsBdr(h))
    return 0;
return !!MWSFSVR_IsSvrBdrHndl(h);
```

Same rule for `if/else` value selection: `if (cond != 1) v = 0; else v = load;`
emits `beq → load` with `li v,0` inline (retail), while `if (cond == 1)
v = load; else v = 0;` emits the mirror. Tested on `MWSFD_IsEndPrepareStop`
(us-803a545c) and `MWSFD_IsColAdjFrame` (us-803a16d4), both 100% FULL_MATCH.

#### 7g. u32 field compared with `== 1` — cast to s32 for `cmpwi`

Comparing a `u32` field against a small literal emits `cmplwi` (unsigned);
retail frequently uses `cmpwi` (signed). Cast the loaded value: `(s32)*(u32*)p == 1`.
Applied in mwPlyGetSfdHn / mwPlyGetStat / criware_803A2258 /
MWSFSET_ExecSetCyclicFrameOutput (all FULL_MATCH).

#### 7h. Calling a same-TU stub with the real signature via cast

When a callee is another agent's in-progress stub (`void mwPlyGetRareStat() {}`)
and its true signature returns a value, declare the old-style stub and call
through a cast so the TU compiles today and keeps matching after the stub
lands: `int stat = ((int (*)(void *))mwPlyGetRareStat)(h);` — emits a plain
`bl mwPlyGetRareStat` with the correct argument registers.

#### 7i. MWCC int→double magic pool (`0x43300000`) reloc drift — fixable by manual bit construction

`(double)(s32)x` requires the `0x4330000080000000` magic (signed-conversion
trick). The shared retail blob `lbl_eu_8051B198` (0x8051B198) holds
4503601774854144 = `0x4330000080000000` — **not** `0x4330000000000000`;
that value is only the `lis` immediate high word, and the `x ^ 0x80000000`
low word is why retail emits `xoris`. MWCC pools the builtin conversion's
magic as a TU-local `@N` label while retail references the shared blob, so
the `lis`/`lfd` pair differs only by reloc symbol name (~98.7% fuzzy; pure
reg-swap on the base register). A **high-level fix exists** — build the bit
pattern manually and subtract the retail blob as an extern double
(statement order matters: the `x ^ 0x80000000` word first, then
`0x43300000`, otherwise MWCC hoists `lis 0x4330` above the extern `lis`s):

```c
extern double lbl_eu_8051B198;
double conv(s32 x) {
    union { double d; u32 w[2]; } u;
    u.w[1] = (u32)x ^ 0x80000000;
    u.w[0] = 0x43300000;
    return u.d - lbl_eu_8051B198;
}
```

Verified with the project's pinned toolchain: under `Wii/1.1 -ipa file`
this reproduces retail's exact instruction schedule with the correct
`lbl_eu_8051B198` / `lbl_eu_8051B190` relocs (pure reg-swap). Caveat: under
the CriWare units' pinned compiler (`GC/3.0a5.2 -lang=c99 -sdata 0 -sdata2
0 -use_lmw_stmw on`, no `-ipa`), the manual pattern schedules the magic
`lfd` three slots later than the builtin conversion — static 69.6% vs the
builtin's 98.696% on `MWSFPLY_SetFlowLimit` (us-803a523c, applied in
`libs/CriWare/src/sofdec/mwply/mwsfdply.c`). Prefer the builtin
`(double)(s32)x` when the unit compiles with GC/3.0a5.2 and accept the
reloc-name-only drift via the EQUIVALENT_MATCH path (SMT + certified callee
chain) or objdiff `functionRelocDiffs=data_value`; use the manual pattern
when the toolchain reproduces retail's early-magic schedule.

**2026-08 correction (hexdiff-verified):** the scale double in retail is
referenced via relocs to **`lbl_eu_8051B190`** (the lis/lfd pair; an earlier
comment claiming the scale lives at 0x8051B170 was wrong — 0x8051B198 is the
2^52 conversion magic). Writing `*(double*)0x8051B170` made MWCC **inline**
the absolute address (`lis 0x8052 / lfd -0x4E90`) with NO reloc, adding
reloc-presence drift; referencing the already-declared `extern double
lbl_eu_8051B190` in the multiply emits the retail relocs and lifts the
function 69.6% -> 82.6% (0 structural, 4 reg_swap). Residual is pure GPR
coloring (magic pool base r6 vs retail r7; xoris result reuses r5 vs retail
r6 — same ABI-boundary class as func_800B7680/801CB9D8). Rule: when a
CriWare/retail pool double is accessed, prefer the named extern over a raw
literal address — raw addresses inline and drop the reloc.

### 7k. Variadic va_start register fusion — struct-pointer local vs char-pointer offsets

For a variadic function that keeps a blob base register across the call
(retail: `lis r30,@ha; addi r30,r30,@l` fused, ap in r31, exactly 2
callee-saved), writing the accesses as `char *base = (char *)&blob;` and
`base + 4` / `base + 8` makes MWCC (Wii/1.1) split the base into a 3rd
callee-saved reg (`lis r30,@ha` scratch + `addi r29,r30,@l`) → frame +0x10,
function +8 bytes — which can blow a zero-slack split budget. Declaring a
struct pointer local and using **field accesses** (`e->cb` / `e->param` /
`e->buf` with offsets 0/4/8) makes MWCC materialize the base ONCE into r30
(fused lis/addi) with ap in r31 — byte-identical to retail. Verified on
`LSC_CallErrFunc_` (us-803949ec, lsc_err.c, Wii/1.1): char-pointer forms
0xCC/3-saved across 15 compilers × -O4,s/-ipa, struct-pointer form 0xC4 /
2-saved / 100% FULL_MATCH. `va_end` does not affect the allocation.

### 7j. CSchedule runtime TU — PS vector subtraction and same-TU inlining

For `monolib/src/core/code_804E36DC.cpp`, `ml::CVec3` subtraction through its high-level `operator-`/`CVec3::sub` path reproduces retail's paired-single `psq_l`/`ps_sub` sequence and the temporary-to-result copy before `PSVECMag`. Scalar component arithmetic does not. The retail `func_804E3B6C` distance helper also uses a same-TU `func_804E424C` call; marking that helper `DECOMP_DONT_INLINE` keeps the TU within its exact `0xC58` split budget. The residual distance-function mismatch is external virtual/callee register scheduling, not a semantic difference.

`func_804E3CCC`/`func_804E39E8` clear a `u16` flag with the retail wrap mask `rlwinm ...,17,15`; ordinary `u16 &= ~0x8000` emits the semantically equivalent `rlwinm ...,17,31`. Use the approved `DECOMP_PPC_RLWINM` intrinsic only when exact opcode selection is required, and log the policy exception.

### 8. Dead return half / Chaitin rotation — `EQUIVALENT_MATCH` workflow

**Symptom:** 95–99% fuzzy, pure register-swap (instructions functionally
identical, only reg names differ between decompiled and retail). The SMT proof
reports `not_equivalent` with `mismatch.name == "r4"` or `"f1"` even though
the register is dead at `blr`. The `auto` contract keeps `r4` because the
body writes it, so `returns_i64=True` and `r4` stays an observable.

**Cause:** `returns_i64` write-based inference in
`tools/ppc_equivalence/abi_infer.py::infer_abi_shape` sets
`returns_i64=False` only when **both** sides return and **neither** writes
`r4`. Under a Chaitin rotation the decompiled body writes `r4` as scratch —
the inference conservatively keeps `returns_i64=True` and the proof fails on a
dead volatile.

**Fix:** set `declared_return` on the target from the C/C++ source return type:

```bash
# Edit targets.json: add "declared_return": "void" (or "i32", "u32", etc.)
python3 tools/coop/run.py targets validate
python3 tools/coop/run.py cycle <target-id> --hypothesis "reg-swap only; r4 dead" \
  --next-change "declared_return on target"
```

The probe combines the declaration with the inferred shape via fail-closed
conjunction, drops `r4` from observables at return/fallthrough exits, and the
proof passes. The result is capped at Tier C.

**Motivating example:** `__prep_buffer` (`us-802c06ec`, MSL `buffer_io.c`).
Pure Chaitin register-swap vs retail, proven memory-equivalent, but the auto
contract kept `r4` as a live-out (i64-return-half assumption). Setting
`declared_return = "void"` from the source return type (`void __prep_buffer()`)
let the proof pass as `EQUIVALENT_MATCH`.

See
[`docs/ppc_equiv_work/29-declared-return-abi-shapes.md`](../../docs/ppc_equiv_work/29-declared-return-abi-shapes.md)
for the full design.

---



---

### Template pitfalls — MWCC and -inline auto

MWCC with `-inline auto` (the default for Xenoblade decomp objects) treats
function templates defined in headers as always-inline and **omits standalone
function bodies**. The retail binary often has these bodies because it was
compiled with different flags or a different MWCC version.

### Symptom

A target is `NOT_STARTED` or `COMPILES` but the function never appears in the
decompiled `.o`. Running `nm build/…/src/…/unit.o | grep <symbol>` returns
nothing, even though the template is clearly used in the file.

### Root cause

MWCC with `-inline auto` sees the full template body in the header and inlines
it at every call site. No standalone global symbol is emitted.

### Solution

Two things are needed together:

**1. `#pragma auto_inline off` around explicit instantiations**

```cpp
#pragma push
#pragma auto_inline off
namespace nw4r { namespace lyt { namespace detail {
    template const BlendMode* ConvertOffsToPtr<BlendMode>(const void*, unsigned int);
    // …
}}}
#pragma pop
```

The `push/pop` saves and restores the inline state so only the instantiation
lines are affected — the rest of the file compiles normally with `-inline auto`.

**2. `unsigned int` vs `u32` — mangling mismatch**

On PowerPC (LP64 data model), both `unsigned int` and `unsigned long` are
32-bit with identical ABI. But C++ name mangling differs:

| Type | Mangling | Example |
|------|----------|---------|
| `unsigned int` | `Ui` | `ConvertOffsToPtr<…>__…FPCvUi_PC…` |
| `unsigned long` | `Ul` | `ConvertOffsToPtr<…>__…FPCvUl_PC…` |

Check the retail binary symbol with `nm build/…/obj/…/unit.o | grep <symbol>`:
if it has `Ui` but your decomp build produces `Ul`, change the template's
parameter type from `u32` to `unsigned int` directly, OR change the `u32`
typedef in your compilation unit.

> **When to change the header template**: if the retail compiled this template
> with `unsigned int` (shown by `Ui` in the mangled name), the template
> definition in our source tree should match. Changing `u32 offset` to
> `unsigned int offset` is safe — both types are 32-bit, ABI-identical, and
> all callers pass `u32` values which implicitly convert.

### What NOT to do

- ❌ `template …` explicit instantiation alone (without `#pragma auto_inline off`)
  → emits warning `(10507)` but no standalone body.
- ❌ `#pragma define_template` — MWCC silently ignores it on this compiler
  version (GC/3.0a5.2).
- ❌ Separate `.s` assembly stub — produces correct symbols but in a different
  `.o` file that objdiff/cycle cannot see (they scan only the unit's `.o`).
- ❌ `extern "C"` with the mangled name — the `<` and `>` characters that C++
  mangling uses in template names are not valid C identifiers.
- ❌ `__declspec(export("name"))` — MWCC parses the `<`/`>` as operators and
  fails with `'(' expected`.
- ❌ Taking the function address via typedef — MWCC reports `<unknown-type>`
  for uninstantiated function templates.

### Real example

All 7 `ConvertOffsToPtr<T>` instantiations in `libs/nw4r/src/lyt/lyt_material.cpp`
were fixed with this pattern. See commit for details.

### CRTP task base (`CTTask<T>`) — declared-only members are mandatory

`monolib/work/CTTask.hpp` (and every derived-task header that used to carry its
own local copy) must keep `~CTTask/Move/Draw` **declared only** (ctor stays
inline). Explicit `template<>` specializations of members that have inline
bodies in the primary template are silently dropped by MWCC (same emission rule
as TextWriterBase above); the CTaskLOD unit demonstrated the failure: its
Move/Draw specializations emitted nothing until the template went declared-only.

Confirmed working pattern (all 100% against retail):

```cpp
// header (the ONE definition, included everywhere)
template <typename TDerived>
class CTTask : public CProcess {
public:
    CTTask() : mMoveFunc(nullptr), mDrawFunc(nullptr) {}
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
protected:
    void (TDerived::*mMoveFunc)();  // 0x3C
    void (TDerived::*mDrawFunc)();  // 0x48
};

// owning TU
// PTMF member is declared as TDerived member, so dispatch needs the downcast:
template<> void CTTask<MyTask>::Move() {
    if (mMoveFunc) (static_cast<MyTask*>(this)->*mMoveFunc)();
}
template<> void CTTask<MyTask>::Draw() {
    if (mDrawFunc) (static_cast<MyTask*>(this)->*mDrawFunc)();
}
template<> CTTask<MyTask>::~CTTask() {}
```

Notes:
- Local copies of the template in kyoshin headers are gone — they all include
  `monolib/work/CTTask.hpp` now; any TU can include any combination of the task
  headers + `work.hpp`.
- The dtor specialization is legal inside an **anonymous namespace** only
  because the template dtor is declared-only; with inline bodies MWCC raises
  `(10333) object redefined`. Anonymous-ns derived classes (e.g.
  `CTaskManager`'s `CRootProc`) must keep their specializations inside the ns
  so the `@unnamed@…` mangling matches the retail data relocs.
- `virtual void Move() { … }` bodies in the OLD template were `0x48` dispatch
  bodies; the declared-only + `template<>` pattern emits byte-identical code
  (static_cast folds away, MWCC lowers to `__ptmf_test`/`__ptmf_scall`).
- Retail emits NO out-of-line `CTTask<X>::CTTask()` ctor, so the ctor stays
  inline (a `template<>` ctor specialization against the declared-only ctor is
  accepted but emits nothing — leave it out).

---



---

### Quick checklist before claiming FULL_MATCH

- [ ] `python3 tools/coop/run.py diff <unit> --symbol <sym>` → 100%
- [ ] Relocations tab clean — 99.7–99.9% with identical insn words → check `@N` vs `lbl_eu_*` (§1)
- [ ] Same `extra_cflags` as `configure.py` for that object
- [ ] No asm / `register rN` / fake `sp[]` in source
- [ ] `python3 tools/coop/run.py size <unit>` PASS (decomp `.text` ≤ split budget)
- [ ] Attempt logged in `docs/evidence/decomp/attempts.jsonl`
- [ ] `tools/coop/targets.json` updated; `configure.py` `Matching` updated for whole TU when appropriate

---



---

### MWCC 1.1 mangling quirks

### Constructor symbol name: MWCC 1.1 emits full mangling, retail has short form

MWCC 1.1 (`build/compilers/Wii/1.1/mwcceppc.exe`) generates full constructor
mangling (`__ct__17CDeviceFontLoaderFPCcP11CWorkThread`) where the retail
binary has a shorter form (`__ct__CDeviceFontLoader`). This is a version
difference — earlier MWCC versions (used for retail) emit the short form.

**Workaround:** the byte-level body still matches; only the symbol name
differs. The hexdiff tool can compare via substring matching. For the co-op
cycle command, the symbol map entry must match what MWCC actually emits.
Update `config/<region>/symbols.txt` to the MWCC 1.1 mangling when needed.

### Extern-C for Fv-mangled functions that take hidden parameters

In the retail binary, some functions have `Fv` mangling (no C++ parameters
beyond `this`) but their bodies read extra register parameters (r4, r5)
passed by the caller. Examples:
- `func_80454F30__17CDeviceFontLoaderFv` (takes `void* arg1, const char* path`)
- `func_8043B574__7CEvent1Fv` (takes `int index` as r4)
- `func_8043B588__7CEvent1Fv` (takes `int index` as r4)

**Fix:** define the function as `extern "C"` with an explicit `self`
pointer as first parameter. This produces the exact `Fv` symbol name
(`extern "C"` strips C++ mangling), and the parameters are passed by C
calling convention (r3=self, r4=arg1, r5=arg2):

```cpp
extern "C" {
void func_80454F30__17CDeviceFontLoaderFv(
    CDeviceFontLoader* self, void* arg1, const char* pPath) {
    self->mSomeData = arg1;
    self->mFileNameLen = strlen(pPath);
    strcpy(self->mFileName, pPath);
}
}
```

This technique works for any `Fv`-mangled function that takes hidden
parameters. The linker symbol matches the retail binary exactly.

**Hidden args also explain "dead" loads:** in
`CDeviceFontLoader::OnFileEvent`, the retail `lwz r4, 4(r5)` (loading
`mFileHandle->mData` then clearing it) looks like a dead load MWCC 1.1
eliminates. It is actually a hidden second argument: the caller passes
`func_80452D80__11CDeviceFontFv(mSomeData, mData)` with `Fv` mangling but
two real params (the callee ignores arg2). Expressing the call with the
explicit second parameter via extern "C" keeps the load AND allocates r4
naturally — FULL_MATCH. Symptom: an "unused" load that won't survive DCE;
check whether the value flows into a subsequent call's r4/r5 slot.

### Constructor via extern "C" (retail short-form `__ct__` mangling)

MWCC 1.1 mangles ctors with the full suffix (`__ct__17CDeviceFontLoaderFPCcP11CWorkThread`)
while the retail binary uses the short form (`__ct__CDeviceFontLoader`).
Write the ctor as a plain extern "C" function carrying the exact retail
symbol, manually calling the base ctor by its mangled name and setting the
vtable via the retail vtable symbol (defined in the data asm, e.g.
`lbl_eu_8056C8A8` in `monolibdata2.s`). Return `self` — the MWCC ctor ABI
returns `this` in r3, and the retail schedules `mr r3, r31` before the
member stores:

```cpp
extern u8 lbl_eu_8056C8A8[];  // retail vtable data symbol
void* __ct__CDeviceFontLoader(CDeviceFontLoader* self, const char* name, CWorkThread* parent) {
    __ct__11CWorkThreadFPCcP11CWorkThreadi(self, name, parent, 0);
    *(void**)self = (void*)lbl_eu_8056C8A8;
    self->mFileName[0] = '\0';
    self->mFileNameLen = 0;
    self->mFileHandle = nullptr;
    self->mType = CWorkThread::THREAD_CDEVICEFONTLOADER;
    return self;
}
```

This reproduces the retail lis/addi `lbl_eu_8056C8A8` relocations exactly
(reloc names must match for objdiff 100%).

**Member-init order controls constant register coloring:** the retail ctor
uses `li r4, 0; ...; li r0, 0x40` and stores the zero-inits with r4 and
`mType` with r0. The init order in the ctor body must be
zero-stores-first, `mType`-last; putting `mType` first flips the constants
to `li r0, 0` / `li r4, 0x40` (reg-swap mismatch).

### Plain struct instead of inheritance to suppress weak virtual stubs

Deriving from a polymorphic base (e.g. CWorkThread) makes MWCC emit a local
vtable plus weak stubs for inherited virtuals (`wkRender__11CWorkThreadFv`,
`wkRenderAfter__11CWorkThreadFv`, `wkStandbyExceptionRetry__11CWorkThreadFUl`
— 0x10 bytes of .text) even when nothing references them, blowing tight
split budgets. Fix: declare the class as a plain `struct` (no inheritance)
with the base class's public member layout duplicated inline, and define
every method as extern "C" with an explicit self pointer. Base-class calls
(`wkSetEvent`, `wkStandbyLogin`, base ctor/dtor, `__dl__FPv`) are made via
extern "C" declarations of their exact mangled names. Function bodies are
otherwise identical (`this->x` == `self->x`), so byte matches are preserved.
The retail vtable must then be referenced directly by the ctor (see above)
because no local `__vt__` is generated.

### MPF model-draw dispatchers: hidden arguments, declaration-order coloring, and split-owned SDA globals

The MPF draw table stores `Fv`-mangled routines that are called through cast function pointers with `self`, draw-data, and list values in r3/r4/r5. Define the bodies as high-level `extern "C"` functions carrying the exact retail symbol names and explicit parameters; this preserves the `Fv` symbol while exposing the ABI arguments. The shared `UnkClass_80471EC8` helpers use the same pattern for their hidden arguments.

For these routines, MWCC's callee-saved GPR coloring followed local declaration order from r31 downward. Declaring the pointer/counter locals in the retail order, and using `poly++, index++` / the variant-specific outer increment order, produced all four 0x188-0x1A0 bodies byte-identically. `getInstance` also matched with the high-level `(T*)&sdaPointer` return idiom.

Finally, `.sbss` globals owned by `monolibdata2.s` must be declared `extern` in the MPF TU. Defining the same `lbl_eu_80665840/60/98/9C` symbols in the TU creates a local `.sbss` section; instruction bytes still match, but objdiff under `functionRelocDiffs=data_value` reports ~99.7% instead of 100%. `extern` declarations restored `FULL_MATCH` for all five targets with a 0x670 split-size pass.

Files: `libs/monolib/src/mpfsys/MPFDrawMdlColor.cpp`, `libs/monolib/include/monolib/mpfsys/MPFDrawMdlColor.hpp`.

### CNand ring-buffer modulo and call-boundary pattern (US)

For signed `s16` ring indices, retail MWCC emitted the signed `% 8` idiom with an `extsh` both before and after the remainder sequence. The high-level form `s16 next = (s16)((s16)(index + 1) % 8);` reproduced `extsh`/`srawi`/`addze`/`rlwinm`/`subf` exactly. Small helpers that retail kept outlined also required `DECOMP_DONT_INLINE`; otherwise `-inline auto` inlined the ring producer into callers and changed their sizes/control flow. With `-O4,s -func_align 4`, the CNand TU reached 13 byte-identical functions; the producer remained a 98.235% pure r5/r6 Chaitin swap and was accepted by effect-aware SMT as `EQUIVALENT_MATCH` at exact size. Files: `libs/monolib/src/nand/CNand.cpp`, `configure.py`.

### 16. stmw/lmw: `-O4,s` vs `-O4,p`

MWCC with `-O4,p` (speed) does NOT use `stmw`/`lmw` for saving 3 consecutive
callee-saved registers (r29-r31); it emits individual `stw`/`lwz` instructions
(+8 bytes prologue, +8 bytes epilogue vs stmw/lmw).

Switching to `-O4,s` (size) triggers `stmw`/`lmw` for 3-register saves.
When using `-O4,s`, also add `-func_align 4` explicitly to prevent default
16-byte function alignment from expanding the `.text` section:

```python
Object(NonMatching, "monolib/src/core/monolib_eu_804F9E98.cpp",
    extra_cflags=["-O4,s", "-func_align 4"]),
```

**Example:** `func_eu_804F9E98` (0x48) — `-O4,p` generated 88 bytes with
3×`stw`/`lwz`; `-O4,s`+`-func_align 4` generated 72 bytes with `stmw`/`lmw`,
matching retail exactly.

### 17. Defeating s16-index strength reduction for FULL_MATCH

`CSchedule::func_804E3614` (0xC8): retail recomputes `&mHandles[count]` per
iteration (`extsh` → `slwi` → `add r4, self, r0` → `sth 0x98(r4)`), but plain
`self->mHandles[count] = handle;` with an `int`/`s16` counter makes MWCC
strength-reduce to a running pointer (`addi ptr, ptr, 2`) and allocate a 4th
callee-saved register (r28) — shifting the whole prologue.

Fix: write the store as explicit pointer arithmetic with the s16 sign
-extension and the array offset LAST, so MWCC keeps the base+displacement
form and the `extsh`-based address recomputation:

```cpp
*(s16*)((u8*)self + (u32)(s16)count * 2 + 0x98) = handle;
```

- `(u32)(s16)count` forces `extsh`; `* 2` forces `slwi` (rlwinm); the
  trailing `+ 0x98` becomes the `sth` displacement instead of an `sthx`
  register-indexed store (putting `0x98` first yields `sthx`).
- Increment order matters: `count++;` must precede `entry++;` in source to
  match retail scheduling (MWCC emits them in source order here).
- Result: 41/50 mismatches → 0/50, FULL_MATCH, byte-identical.

### 16b. Forcing retail's per-iteration global reload in shift loops (opaque byte-offset arithmetic)

`UnkClass_8045F564::~UnkClass_8045F564` (`__dt__17UnkClass_8045F564Fv`, 0x100):
retail's array-removal shift loop reloads the `lbl_eu_80665710` singleton from
sbss **every iteration** (`lwz r6, lbl_eu_80665710@sda21(r0)` inside the check),
because its raw-pointer stores (`stw r0, 0x238(r3)` where r3 is derived from
the reloaded pointer) alias with the global in MWCC's view.

**Symptom:** writing the loop with struct-field access
(`layout->instanceArray[j] = layout->instanceArray[j+1]` and
`j < layout->instanceCount - 1`) lets MWCC prove field non-aliasing and cache
`instanceCount` in a register → the loop gets strength-reduced/unrolled and
the code differs massively (6% match, 0x1CC bytes under `-O4,p`).

**Fix:** express the same semantics with opaque byte-offset arithmetic so MWCC
cannot disambiguate the store from the global reload:

```cpp
u8* base = (u8*)lbl_eu_80665710;
u32 cnt = *(u32*)(base + 0x2B8);
u32 i = 0;
u8* it = base;
while (i < cnt) {
    if (*(u32*)(it + 0x238) == (u32)this) {
        u32 bo = i * 4;
        u32 curCnt;
        while (i < (curCnt = *(u32*)((u8*)lbl_eu_80665710 + 0x2B8)) - 1) {
            u8* cur = (u8*)lbl_eu_80665710 + bo;
            bo += 4;
            u32 next = *(u32*)(cur + 0x23C);
            i++;
            *(u32*)(cur + 0x238) = next;
        }
        *(u32*)((u8*)lbl_eu_80665710 + 0x2B8) = curCnt - 1;
        break;
    }
    it += 4;
    i++;
}
```

Key points:
- Assigning the reloaded count into a named `curCnt` **in the loop condition**
  lets MWCC reuse the last `subi r0, rX, 1` result for the post-loop store
  (`stw r0, …`) instead of emitting a second `subi` (removes 1 instruction).
- Drop any `if (cnt != 0)` wrapper around `while (i < cnt)`: the wrapper makes
  MWCC emit a redundant second zero-check before `mtctr`; without it MWCC
  emits retail's single `mtctr`/`cmplwi`/`bdnz` counted loop.
- Combine with `-O4,p` (drop `-O4,s`) so the prologue uses individual
  `stw r31, …`/`stw r30, …` instead of `stmw r30` — see §16.

Result: 6% → **99.5% match** (CODE_MATCH, size exact 0x100); the last 5 diffs
are pure reg-swaps in the reload (Chaitin coloring artifact — 6 source
variants tried: named layout, named count, reassignment, scope moves — none
flip the r3/r6 assignment). `EQUIVALENT_MATCH` additionally blocked by
unaccepted external callees (`us-80435c98`, `us-804375c4`).



---

### `__declspec(novtable)` + `extern "C"` constructor + C++ virtual destructor

**Symptom:** Retail constructor uses short-form symbol name (`__ct__CScnFrame` instead of
`__ct__9CScnFrameFUl`), and derived destructor does not call base destructor.

**Root cause:** The retail class may not use C++ inheritance but instead flatten the struct.
The constructor is a plain `extern "C"` function, while the destructor is a genuine C++ virtual
destructor with the full mangled name. The vtable/RTTI data lives in a separate data TU.

**Fix pattern:**

```cpp
// Header: novtable + virtual dtor (no C++ constructor declaration)
struct __declspec(novtable) CMyClass {
    virtual ~CMyClass();
    // fields...
};

// Source: extern "C" constructor sets vtable manually;
// C++ virtual destructor handles delete check automatically

extern char lbl_eu_ADDRESS[];  // vtable from data TU

extern "C" void __ct__CMyClass(CMyClass* self) {
    *(void**)self = (void*)lbl_eu_ADDRESS;  // manual vtable
    // field inits...
}

CMyClass::~CMyClass() {}
```

**Constraints:**
- `__declspec(novtable)` prevents MWCC from generating vtable/RTTI data in the TU
- `extern "C"` constructor gives short-form symbol name matching retail
- C++ virtual destructor gives full-form mangled name (`__dt__NClassNameFv`) matching retail
- No base destructor call in generated code (no C++ base class relationship)
- For "base class" init, call the init function explicitly: `other_init(self);`
- Works with `-RTTI on -Cpp_exceptions on`
- Tested on: CScnFrame (standalone), CVirtualLightAmb (flattened)

### Early-load scheduling via pre-computed locals (CriWare sfd_buf)

**Symptom:** Function is semantically correct but hexdiff shows pervasive register
swaps and prologue scheduling mismatches. MWCC schedules array loads near their
first *use* in source order, but retail scheduled them at the *top* of the
function for latency hiding.

**Fix:** Compute derived values (nonzero flags, pointer offsets) into named locals
*before* any stores. This forces the compiler to schedule the loads early,
matching retail's instruction order and Chaitin register allocation.

```c
// BAD: compiler delays ptrs[idx] load until the p[4] store
*(u32 *)(p + 0x00) = 2;
*(u32 *)(p + 0x04) = (u32)(ptrs[idx] != 0);  // load scheduled here

// GOOD: local forces early load, matching retail scheduling
u32 valid = (u32)(ptrs[idx] != 0);  // load scheduled at top
*(u32 *)(p + 0x00) = 2;
*(u32 *)(p + 0x04) = valid;
```

**Key details:**
- The local must be used *soon* after definition (within ~2 instructions) for the
  compiler to schedule the load at the definition point. If the use is far away
  (20+ instructions), the compiler delays the computation regardless.
- For pointer fields that are later re-dereferenced (e.g. `p[0x20]` stored then
  read back for indirect zeroing), use the local for the *first* indirect access
  and re-dereference through the struct field for subsequent ones. The compiler
  uses the live register for the first access, then reloads from memory (aliasing
  prevents CSE), matching retail's `stw r3, off(r10); stw r7, 0(r3); lwz r3, off(r10); ...`
  pattern.
- Do NOT cache array values that retail reloads (e.g. `ptrs[idx]` used for both
  the nonzero check and a later field store). Without a local, the stores to `p`
  between the two accesses prevent CSE (void* aliasing), forcing the reload.

**Results:** sfbuf_InitAringBuf went from 76.8% (HIGH_MATCH, SMT blocked by
Chaitin rotation) to **100% FULL_MATCH** with this pattern. sfbuf_InitVfrmBuf
reached **91.2% EQUIVALENT_MATCH** (remaining gap: prologue scheduling of a
late-used `addi` that the compiler won't hoist).

**Files:** `libs/CriWare/src/sofdec/sfdcore/sfd/sfd_buf.c`



---

### Jump-table SMT equivalence: MWCC `lis; slwi; addi; lwzx` shape + linked retry (sfh_ver1)

**Symptoms:** A clean `switch` (e.g. `criware_803D2C98`, pic-rate code → rate×1000
jump table) compiles byte-identical (hexdiff 0 mismatches, fuzzy 99.7%) yet
`cycle` remains at `CODE_MATCH`: objdiff 99.7%, SMT `not_equivalent` with
`exit.target: 0x0000bc00 != 0x00000000`.

**Root cause chain (three independent gaps):**
1. `tools/ppc_equivalence/jump_table.py` `_match_jump_table_tail` only accepted
   the self-shift `slwi` **immediately before** `lwzx` (`addi; slwi; lwzx`).
   MWCC also emits `lis; slwi; addi; lwzx` (shift between the base `lis` and the
   low-half `addi`) — recognised only as `partial` → no auto jump-table context.
   Fix: accept a left-shift up to two slots before the load when only
   `addi`/`addis` sits between it and the load; also corrected
   `_parse_left_shift` to return `(source, dest, scale)` (it returned
   `(dest, source, …)`; harmless for the old self-shift check).
   Corpus probe now tracks `resolved_base` separately from `auto_context_ok`
   (register-relative tables like `addi rX, r25, off` can never hydrate).
2. `tools/ppc_equivalence/engine.py` jump-table obligation block **overwrote**
   `early.proof_features` with `["readonly-image","indirect-target-closure"]`,
   dropping an already-appended `memory-bus` entry when a hardware profile is
   configured → feature gate demoted EQUIVALENT to INCONCLUSIVE_UNSUPPORTED
   ("obligation block 'memory_bus' present without a matching proof_features
   entry"). Fix: append instead of assign (matches the virtual-call block).
3. `tools/coop/lib/equivalence_check.py` `prove_unit_symbol(linked=True)` only
   retried with DOL/ELF linked bytes on **raised exceptions**. An unlinked
   proof that *completes* NOT_EQUIVALENT (TU-local reloc names `@N` vs retail
   `jumptable_eu_*` leave object-relative exit targets incomparable) never
   reached the linked retry. Fix: also retry when status is NOT_EQUIVALENT and
   either side carries unresolved relocations.

**Result:** `us-803d5358 criware_803D2C98` accepted `EQUIVALENT_MATCH` (fuzzy
99.7%, size PASS). Requires `cycle --linked` (and `ninja build/us/main.elf`).
Engine hash re-blessed in `coop.json` (`allowed_engine_sha256` =
`317dcc5b…`); full ppc_equivalence suite (1908 tests) + coop tests green apart
from pre-existing failures. **Files:** `tools/ppc_equivalence/jump_table.py`,
`engine.py`, `jump_table_corpus_probe.py`, `tools/coop/lib/equivalence_check.py`,
`libs/CriWare/src/sofdec/sfdcore/sfh/sfh_ver1.c`.


---

### Symbol-name resolution: retail symbol mangling ↔ source linkage (US, Wii/1.1)

The equivalence probe resolves the decomp side **by symbol name only** (`_resolve_candidates` in `tools/ppc_equivalence/elf_symbols.py`). A byte-identical body under a different name reads as `inconclusive_unsupported (0 candidates)` — the sweep's 24 blocked targets were mostly this. Rule:

- **Mangled retail symbol** (`func_800B07E8__Fv`, `func_80133324__12CUICfManagerFiii`) → the source must be a **C++ function** (member or free, MWCC emits the mangling). `extern "C"` emits the bare name → 0-candidate probe failure even at 100% bytes.
- **Bare retail symbol** (`func_8004350C`, `func_800B1A5C`, `lbl_eu_80663D18`) → `extern "C"` free function (explicit `self` param for member-like bodies).
- Verify the retail symbol's mangled signature against retail asm (arg registers r4-r10/f1-f8) before trusting it — `func_80133324__12CUICfManagerFv` was a wrong `Fv`; retail used r4/r5/r6 → corrected to `Fiii` in symbols.txt + target record.

---

### MWCC `-inline auto` inlines same-TU helpers — use `__declspec(noinline)`

`-inline auto` (kyoshin TUs) inlines any small function defined in the same TU, even when retail made a real `bl` (e.g. a 0x64 sinit inlined into a caller, or a 0x30 list-clear inlined at 10 call sites — body ballooned 0x108 → 0x278). Established fix: `extern "C" __declspec(noinline) void f(...)`. Also apply to the forward decl so callers before the definition see it.

---

### Struct layout verification via retail field stores

A named member's offset can be wrong even when its comment is right. `FixStr<N>` in code_800B06A4.hpp had `u8 _pad[8]` (size N+12) shifting `field_0xCF4`/`field_0xCF8` to 0xCFC/0xD00 — retail stored u32/f32 at 0xCF4/0xCF8 (inside the pad region; writing real string data there would be nonsensical). Check the retail store offsets against the member comment before trusting a struct. Fixing the struct lifted func_800B6800 34.9% → 46.5% with zero regressions to FULL_MATCH neighbors.

---

### Mask↔rlwinm encoding (empirical, -O4,p)

MWCC's `rlwinm` encoding for AND-masks is inverted vs naive bit math:
- `x & ~0x00000100` (clear bit 8) → `rlwinm rX,rX,0,24,22`
- `x & ~0x00800000` (clear bit 23) → `rlwinm rX,rX,0,9,7`
Verify with `.scratch/` probes (compile a tiny TU with the unit's exact flags) before hand-decoding masks.

---

### Byte-identical rename path to FULL_MATCH

For a target whose decomp body exists under another name: find the byte-identical counterpart (compare `FunctionBytes.code` across the decomp unit), rename source to the retail symbol (linkage per the rule above), rebuild, hexdiff 100%, cycle → FULL_MATCH. Worked on: `isUnk68Bit13Set`→`func_800404F0`, `actCallVt90/94/30`→`func_800560E4/F4/118`, `callStubReturnZero_800436A8`→`func_8004368C`, plus symbols.txt mangling fixes (`UnkVirtualFunc29 Fv→Ff`, `getRsrc Fv→CFv`).

---

### `extern "C"` is redundant on global-scope data labels — centralize with `tools/coop/lbls_gen.py`

Verified with the repo's MWCC Wii/1.1 (`build/compilers/Wii/1.1/mwcceppc.exe`):

1. **MWCC never mangles global-scope *data* names.** Plain `extern u32 lbl_eu_8065FC18;`
   emits the exact retail reloc (`R_PPC_EMB_SDA21` / `ADDR16_HA/LO` + `lbl_eu_XXXX`),
   byte-identical to `extern "C"` — for declarations *and* definitions. The `extern "C"`
   on lbl data is only semantically required inside `namespace { }` blocks (plain
   extern mangles there: `lbl_ns_plain__Q24[...]`). Mixed-linkage decl/definition
   (header C++-linkage decl + TU `extern "C"` definition inside a namespace) compiles
   and links cleanly on MWCC.
2. **`extern` declarations are inert.** A TU compiled with per-TU decls vs an
   equivalent header include produces byte-identical `.text` and zero data/bss —
   verified object-level across the whole build (2,337 objects) after a full
   codemod: only concurrently-agent-edited files differed.
3. **Bare declarations are definitions.** `CProcess* lbl_eu_80664054;` (no `extern`)
   inside `extern "C" { }` emits a `B`-symbol (TU's own `.bss` storage) — do NOT
   centralize those; only explicit-`extern` decl lines are inert.
4. **Workflow** (`tools/coop/lbls_gen.py`, per-area headers `include/lbls_<area>.hpp`):
   `generate` (pristine tree; writes headers + `lbls_manifest.json` +
   `lbls_exclusions.json`) → `apply` (strips per-TU decls, adds includes; **never
   rewrites the headers** — regenerating from the post-strip corpus silently shrinks
   them; skips git-dirty files so it never collides with concurrent agents) → `check`
   (CI gate: header/manifest freshness, no stray decls for centralized addresses,
   every remaining decl listed in the exclusions) → verify with
   `tools/coop/snapshot_objects.py` before/after a `ninja` build (compare `.text`
   sha + section sizes + relocs). Addresses with type conflicts across TUs or
   class/typedef types stay per-TU (listed in the exclusions) — forcing one type
   would change instruction selection (struct-vs-pointer: `lbl.f` is one `lwz`,
   `lbl->f` is two).

---

### 3-word struct copy: typed source pointer required for grouped load-all/store-all (kyoshin cf, FULL_MATCH)

`cf::CfObject::CfObject_UnkVirtualFunc27(void*)` (us-8003f6d8, ocUnit.cpp) is a
7-instruction leaf: `lwz r6,0(r4); lwz r5,4(r4); lwz r0,8(r4); stw r6,0x48(r3);
stw r5,0x4c(r3); stw r0,0x50(r3); blr`. 56+ prior shapes (3 u32 locals, u64
extract, CVec3 assign, u64 direct store, struct temp, store-order permutations,
pragma combos) all failed on register colors or spilled. Root causes:

1. **Grouped 3-word copy needs a TYPED source pointer parameter.** A `void*`
   param (even cast to a typed local) makes MWCC emit interleaved
   `lwz r0,N(r4); stw r0,N(r3)` pairs reusing r0. Declaring the param as
   `const struct { u32 a,b,c; }*` (or `const ml::CVec3*`) emits the retail
   batched loads `lwz r6/r5/r0` + in-order stores. Verified with direct
   `mwcceppc` probes (.scratch/copy12_probe{3,4,5}.cpp, Wii/1.1 -O4,p):
   `void*` src ⇒ interleaved; typed src ⇒ grouped; dest-side cast irrelevant.
2. **The retail `...FPv` name is a decompiler guess.** The mangled symbol
   `CfObject_UnkVirtualFunc27__Q22cf8CfObjectFPv` claims `void*` but retail
   codegen proves the original source had a typed pointer (same scheme as
   CfObjectEff's `func_800ACDA0__Q22cf11CfObjectEffFv` and CfObjectModel's
   `CfObject_UnkVirtualFunc27__Q22cf13CfObjectModelFPv`, both FULL_MATCH).
   Define the function as `extern "C" void <mangled-name>(...)` with typed
   params — `extern "C"` emits the name VERBATIM (no `__FP<params>` re-mangle);
   a plain global with the mangled-looking name gets re-mangled.
3. **Stale `#pragma optimize_for_size on / peephole off / scheduling off`
   blocks poison nearby leaves.** Func27 sat inside one (left over from another
   function's experiment): with the pragmas active, the struct-member copy
   emitted the grouped loads + an unnecessary `stwu sp,-32` temp spill; after
   removing the stale block the spill vanished. When a leaf refuses to match,
   check the pragma state above it before probing shapes.
4. **Working shape** (FULL_MATCH, ocUnit.cpp):
   ```cpp
   struct CfObjCopy12 { u32 a; u32 b; u32 c; };
   struct CfObjVec48View { u8 _pad[0x48]; CfObjCopy12 vec48; };
   extern "C" void CfObject_UnkVirtualFunc27__Q22cf8CfObjectFPv(CfObjVec48View* self, const CfObjCopy12* src) {
       self->vec48 = *src;
   }
   ```
   The vtable slot reloc resolves because the C symbol name equals the retail
   mangled name. Do NOT use a function-pointer cast at call sites to work
   around a conflicting 4-arg declaration: `((void(*)(void*,int,int,int,int))&f)(...)`
   emits byte-identical code to a direct call (probe-verified) but is less
   readable — prefer the direct call with the correct declaration.

---

### ABI-boundary register-reuse class: arg register reused for a VALUE — witness has no bijection

Several kyoshin/CriWare leaves share a class where MWCC reuses an incoming
ARGUMENT register (r3 this/self, r4 src, r5 idx) for a computed value once the
arg dies, while retail keeps the value in the *other* freed register. The
register-renaming witness rejects all of them ("rho | no consistent bijection
in region [lo, hi)") because the arg register maps to both itself (entry) and
the value register (after reuse) — a non-injective register mapping. FULL_MATCH
is the only route; probe every compiler version first (GC 2.7/3.0a5.2, Wii
1.0/1.1/1.5 all behave identically for these shapes).

Members (all recorded open items, witness-gate rho):
- **`func_801CB9D8` (us-801cd42c, CItemBoxGrid) — SOLVED FULL_MATCH:**
  12-byte-stride entry copy; retail base in r5 (reused idx reg), MWCC always
  bases in r4 (reused src reg). Fix: declare the **value locals before the
  base pointer** — `u32 v24, v28; u8* e = (u8*)src + idx * 12;` then load into
  v24/v28. MWCC colors in source order, so the value locals claim r4 (freed
  src arg) first and the base lands in r5 (freed idx arg): retail's exact
  `add r5,r4,r0; lwz r4,24(r5)`. Old shape (base assigned first, values
  inline) invariantly bases in r4 across decl-order, word-indexed,
  inline-offset, struct-typed, 5 MWCC versions. 100% byte-identical,
  semantic-certified, no witness needed.
- `func_8018B130` (us-8018c6e4, CMenuShopSell): 0x800-byte struct-copy loop;
  retail saves `or r6=src` before `or r7=dst`, MWCC reverses. Invariant:
  local/order variants, memcpy form (emits a call).
  **2026-08 sweep (~55 shapes total):** src-first moves ARE reachable, but
  only when the SOURCE is the `this` pointer of a member `copyTo(dst)` form
  (`d->x = this->x` with this=src=r3 emits `mr r6,r3; mr r7,r4`) — MWCC
  always moves the `this`/arg1 pointer first, so a free `func(dst=r3,
  src=r4)` can never emit the retail `or r6,r4,r4` first (r3's move
  invariantly precedes). ABI registers (r3/r4) rule out witness
  certification. Remaining angle: an original TU where the copy helper
  receives (src, dst) in that order.
- `CfObjectMove::CfObject_UnkVirtualFunc14/16` (us-800beb30/800beb80) — SOLVED
  FULL_MATCH (both, 100% byte-identical): three null-checked `stfs` to
  +0x388/+0x38C of the C4/C8/CC targets; the LAST block's value reuses r3
  (this) in MWCC vs retail's r4 (this dies at the third load). Invariant
  before: separated locals, inline helper, `(void)this`, `#pragma
  optimize_for_size on`. **Fix: make the function RETURN `this`** (change the
  virtual's return type to `void*`/`CfObjectMove*` — MWCC does not encode the
  return type in the mangled symbol, so the name is unchanged). Returning
  `this` keeps r3 (this) live through the third load, so MWCC must use r4 for
  the third target — exactly retail's allocation. Both targets 100%
  FULL_MATCH, semantic-certified. Reuse: for "last block reuses a dead
  argument register" near-misses, keep the argument register live by making
  the function return it (return type is not part of the MWCC mangled name).

Approved policy note: do NOT "fix" these with register/stack tricks — record
the open item with the exact region and invariant shapes, keep the source
natural, and move on. Any future tooling improvement (witness v2 with
per-value mapping, or a decomp.me scratch with a different MWCC build) should
revisit this class.

---

### Dead-branch goto-chain reproduces retail's duplicated-condition beq target (btm_sec, FULL_MATCH)

`btm_sec_l2cap_access_req` (us-802eed3c, btm_sec.c, 0x464 bytes) had one residual:
a 4-byte branch-distance diff on a DEAD branch. Retail emits
`cmpi is_originator,0; beq L_body; beq L_skip; lwz cur; cmpi cur; bne L_skip;
L_body: stw` — a duplicated `is_originator == FALSE` test whose SECOND beq
targets the SKIP (dead: the first beq already takes the body path on the same
condition). The `||` form `if (A || A || B) S` makes MWCC emit the second beq
to the BODY. Fix: a goto-chain with the exact retail layout:

```c
if (is_originator == FALSE) goto set_service;          // beq L_body
if (is_originator == FALSE) goto service_done;         // beq L_skip (dead, kept)
if (p_dev_rec->p_cur_service != NULL) goto service_done; // lwz; cmpi; bne L_skip
set_service:
    p_dev_rec->p_cur_service = p_srec;
service_done:
```

Result: 99.6% → 100% (FULL_MATCH). Lesson: when a duplicated/merged condition
shows a branch-TARGET diff on a provably-dead branch, the original source was
a goto-chain with the dead branch explicitly targeting the join label — the
`||` form silently retargets it to the body.

---

### Struct member-size mismatch shifts a field (CMainMenu, FULL_MATCH)

`func_80101A88` (us-80102570, CMainMenu) read `player->field_0x3F60` at
compiled offset 0x4024 (retail: 0x3F60). Cause: the embedded
`CMainMenuPlayerSpot` member is 0xC8 bytes (vtable + 0xC0 pad + field_0xC4),
but the parent padded `_3EA0[0x3F60 - 0x3EA0]` assuming a 4-byte spot —
the pad math `0x3F60 - 0x3EA0` silently absorbed the 0xC4-byte overflow,
pushing field_0x3F60 to 0x4024. Fix: drop the parent's duplicated
field_0x3F60 (it aliases `spot.field_0xC4` == player+0x3F60) and the bogus
pad; access the sub-object via `player->spot.field_0xC4`. Rule: when a
member-typed field's compiled offset disagrees with its name, verify the
EMBEDDED struct's sizeof — a size mismatch in the embedded type shifts every
following field even when the pad arithmetic looks self-consistent.

---

### -RTTI on pushes the first declared virtual to vtable+8 — use minimal cast-only interfaces (FULL_MATCH)

`CWorkSystemPack::wkStandbyLogout` (us-804e26bc, CWorkSystemPack.cpp) called a
vtable+8 destroy hook: retail `lwz r12,0(r3); lwz r12,8(r12); li r4,1; bcctrl`,
but the local mirror class `{virtual ~CPackItem(); virtual func_0x4();
virtual func_0x8(int);}` emitted slot 16. Cause: with `-RTTI on` (game flags),
MWCC inserts TWO hidden RTTI slots before the first declared virtual, so the
3rd declared virtual lands at +16. The retail slot +8 is a destroy hook, not
the class's own vtable layout. Fix: a minimal cast-only interface with the
hook as its ONLY virtual (lands at +8):

```cpp
struct CPackItemDestroyHook {
    virtual bool destroy(int flag);    // vtable+8 (first virtual under -RTTI on)
};
// call site:
((CPackItemDestroyHook*)item)->destroy(1);
```

Do NOT drop the dtor/func_0x4 declarations from the mirror class itself —
other functions in the TU call `delete` on it and regress (ctor/reslist dtors
depend on the full layout). Rule: when a virtual dispatch's slot offset is
wrong, compute the offset as `8 + 4*(declared-virtual-index)` under -RTTI on
(hidden RTTI pair at 0/4) and use a dedicated one-virtual interface for the
target slot instead of reordering the shared class.

---

### reslist-style objects carry a vtable POINTER at +0x00 even when methods are non-virtual (FULL_MATCH)

`func_801BC590` (us-801bde88, CSuddenCommu.cpp) read the enum list's
`mPtrCount` at compiled offset 0x61C vs retail 0x620. The `cf::CfObjEnumList`
class declares its methods as plain (non-virtual) members — retail calls
`func_800F6EAC` DIRECTLY (reloc confirmed) — yet the retail layout has
`mPtrArray` at +0x20 / `mPtrCount` at +0x620. Cause: the reslist BASE object
carries a vtable POINTER field at +0x00 (set by the reslist ctor), so the
padding after it is `[0x20 - 0x04]` starting at 0x04. The header had dropped
the vtable pointer, making the array start at 0x1C and the count at 0x61C.
Fix: add `void* m_vtable; // 0x00` and KEEP the `u8 _pad_04[0x20 - 0x04]`
length (0x1C bytes — do not "fix" it to `0x20 - 0x08`; 0x04+0x18=0x1C ≠ 0x20).
Verified: retail ctor `__ct__cf_CfObjEnumList` does `stw r7, 0x620(r3)` and the
sudden-commu loop reads `lwz r0, 1568(r3)` — both 0x620. Rule: when a
member-typed offset is 4 bytes low, check for a dropped vtable/pointer field
at the object base, and verify pad arithmetic against the target offset
(offset + pad-length must equal the next field's offset).

---

### `-ipa off` reverses the TU function/pool emission order — pooled-string immediates shift (FULL_MATCH)

`rfc_port_closed` (us-80305588, rfc_utils.c) was 96.5% with 3 offset diffs:
`addi r4, r31, 0x68/0x54/0x10` (retail) vs `0x18/0x28/0x3c` (decomp). The string
REFERENCES were correct (each side points at its own "rfc_port_closed" etc.) —
the .data layout differed because the STRING EMISSION ORDER follows the
FUNCTION EMISSION ORDER, and with `-ipa off` MWCC emits the TU's functions in
REVERSE source order (object table verified: check_send_cmd@0x0 ... calc_fcs
last). Retail emits forward. Fix: `-ipa file` (extra_cflags) restores the
forward emission order — rfc_port_closed 96.5% → 100%, zero unit regressions
(13/13 functions stay 100%), split exact. configure.py's port_rfc.c note
already documented the same fix ("-ipa off reversed the TU function/pool
emission order ... unit needs -ipa file (forward pool)"). Rule: when pooled
string/constant immediates differ by whole-pool layout, check the unit's
`-ipa` setting and the TU's function emission order in the .o (`list_text_functions`
order) before touching per-function source.

### `u32` counter `<= 0` folds to `== 0` (bne) — use `s32` for signed post-decrement guards
CScnTexWorkMan dtor (us-804941ec): the live-instance counter global was
declared `u32`; `counter--; if (counter <= 0)` folded the unsigned `<= 0` to
`== 0`, emitting `bne` (branch-if-not-eq skips the body) — semantically wrong
for counter==0 (0 <= 0 must run the body) AND byte-different from retail's
`addic.; bgt` (signed post-decrement check). Declaring the global `s32`
reproduced the retail `bgt` exactly (91.7% → 100%). Symptom to look for:
retail `bgt`/`ble` after `addic.` where the decomp emits `bne`/`beq`.

### Struct-assignment copy loops returning the dst pointer
func_80227994 (CQstLogList): a 0x22-byte entry copy compiled by retail as 2
byte copies + a 4x8-byte `lwzu/stwu` update-form counted loop (`mtctr`/`bdnz`).
The hand-rolled word loop emits plain `lwz/stw + addi`; the natural source
`*pDst = *pSrc; return pDst;` (full struct assignment) reproduces the retail
exactly. The `return pDst` is load-bearing: it keeps r3 (= dst) live through
the loop, forcing the loop word into r4 (retail's allocation). Needs
`optimize_for_size` to suppress the -O4,p unroll.

### `#pragma scheduling off` reproduces -O3 interleaved copy shapes
func_80227660 (CMenuQstCnt): retail emits the -O3 "load r0; store r0"
interleaved per-field copy under an -O4,s unit. Per-function
`optimization_level` cannot downgrade codegen (MWCC_CASES 1302), but
`#pragma scheduling off` around the function reproduces the interleaved
single-register copy byte-for-byte under -O4,s (verified Wii/1.1).

### 2026-08 highmatch sweep — reusable fixes (agent-highmatch session)

- **Mask-path volatile re-read (CfObjectMove func_800BC3B0, 0x28, FULL_MATCH):** retail
  reloads `mFlags68` after an early-return branch; MWCC CSEs a plain member re-read into
  the check register. `(*(volatile u32*)&self->mFlags68 & ~0x800u) | 0x100400u` forces the
  fresh `lwz` byte-identically. Also: retail `rlwinm 21,19` clears **0x800** (not 0x100000),
  and `oris 0x10` sets **0x100000** (the 16-bit immediate is shifted left 16).

- **Goto-gate for the branch-over-branch layout (CTitle func_802B64AC / func_802B6B08,
  0x30, FULL_MATCH):** retail's `bc 4,2` over a `blr` (if-body placed AFTER the return)
  requires `if (A && B) goto body; return; body: ...;` — plain nested ifs emit `bclr 12,2`
  instead. Same family as the documented sjrbf_PutChunk gate.

- **`void*` intermediate flips callee-saved allocation (nw4r snd_RemoteSpeaker
  IntervalAlarmHandler, 0x70, FULL_MATCH):** `RemoteSpeaker* p = (RemoteSpeaker*)
  OSGetAlarmUserData(pAlarm);` allocates pAlarm→r31/old→r30; the two-statement
  `void* ud = OSGetAlarmUserData(pAlarm); p = (RemoteSpeaker*)ud;` flips it to the retail's
  r30/r31 and byte-identity. (Contrast: the same trick does NOT move func_80145AA8's
  base/s roles.)

- **`__declspec(noinline)` on a trivial ctor (CMenuVision func_801ACCE0, 0x7C,
  FULL_MATCH):** an empty `__ct__CMenuVision { return self; }` gets INLINED by -inline auto,
  dropping the retail's ctor call and its null-check (and mangling the args — the size
  leaks into r3). `extern "C" __declspec(noinline)` restores the call + `cmpi/bc` pair.

- **Ternary vs if/else for a f32 selection (CTaskGameEff func_80044FBC, 0x7C,
  FULL_MATCH):** `f32 time; if (enable) time = const; else time = g->field;` lets MWCC
  hoist the following call's `lis/addi` base ABOVE the if; the single-expression
  `f32 time = (enable) ? const : g->field;` pins the base after the selection — byte-
  identical.

- **`(s8)(u8)` double cast reproduces the dead `rlwinm`+`extsb` pair (CSortMenu
  func_801D377C, 0x78, 86.7%):** `s8 sp5s = (s8)(u8)sp5;` emits retail's dead
  `rlwinm rX,r0,0,24,31; extsb rX,r0` (the rlwinm result is overwritten); a single `(s8)`
  cast emits only the extsb and the function is 1 instruction short.

- **Per-use inline casts instead of a base local (CScnItemModel func_80485CE8, 0x7C,
  FULL_MATCH):** keeping `act = (Act*)self->field_0x1F8` in a local makes MWCC hoist
  `addi rX, self, 0x1F8` into a saved register (+1 instruction, shifted allocation).
  Repeating `((Act*)self->field_0x1F8)->...` per use makes MWCC recompute the base inline
  like retail (0 structural → 100%). Also: the flag bit was **0x80000000** (rlwinm 0,0),
  not `& 1`.

- **`#pragma optimize_for_size on` for stmw/lmw frames (CTitleAHelp `__ct__CTitleAHelp`,
  0x80, FULL_MATCH):** retail ctor saves r29-r31 via `stmw`/`lmw`; the function-scoped
  pragma reproduces the frame byte-identically (same lever as the documented kyoshin
  dtor stmw cases).

- **Wrong-field zeroing (ahx_sjd AHXSJD_Start, 0x84, FULL_MATCH):** retail zeroes
  `chanInfo[2..7]` (0x1C-0x30, six words) — NOT decCallback/decCallbackPrm (0x4C/0x50).
  Always map each `stw` offset against the struct before assuming which field the retail
  initializes.

- **Loop-exit branch polarity (CGXCache func_8044CEF8/CF74, 0x7c, OPEN):** retail emits
  `[cmpl; bne → loop-continue; b → found]` (found OUT of line, +1 instr); MWCC emits
  `[cmpl; beq → found]` for every shape tried (for/do-while/goto/break/continue,
  -O4,p/s/3/2, GC/3.0a5.2, Wii/1.1). Open item: the found path stays the branch target.

- **sprintf format from a named string-table extern at a +offset (kyoshin
  code_80135FDC func_80136C98, 0x54, FULL_MATCH):** the retail format arg is
  `lbl_eu_80500664 + 7` — a NAMED .rodata string-table object (0x27C bytes),
  not the TU-local `%d` pool literal. Writing `"%d"` emits `addi r4,r6,@l`
  with an `@stringBase0` TU-local reloc; writing `&lbl_eu_80500664[7]` emits
  the retail `lis @ha; addi r6,@l; addi r4,r6,7` (named `lbl_eu_80500664`
  relocs, correct +7 slot, call-site alignment) — 0 structural, 100%.
  Reusable: a sprintf/strcmp/strcpy argument that lands at a nonzero offset
  of a large pooled string is usually a NAMED extern slice (`&lbl_eu_XXXX[7]`)
  — check sibling functions in the TU for existing `lbl_eu_80500664[...]`
  uses before assuming a standalone literal.

- **Duplicated-guard dead branch (CKizunagram func_80257D90 / func_80257F44,
  0x58 each, FULL_MATCH ×2):** retail `cmpwi r0,0; beq epilogue; beq zero;
  call; zero; epilogue` — TWO beq's on the same condition, and the field
  zeroing sits INSIDE the outer if. Single `if (p) { call; } p = 0;` emits
  one beq with the zeroing outside. The matching shape is the doubly-nested
  guard `if (p != 0) { if (p != 0) { call; } p = 0; }` — MWCC CSEs the
  duplicated test to one cmpwi + two beq's (same family as the btm_sec
  duplicated-operand dead branch). Reusable: when retail shows a dead
  second beq testing the same condition, duplicate the guard expression
  in a nested if rather than assuming an || chain.

- **Counted 1x loop vs -O4,p unroll: whole-function `optimize_for_size` +
  `-use_lmw_stmw off` (CriWare mpv_get MPV_GetPicAtr, 0x7c):** retail
  `li r0,16; mtctr; [8-byte copy]; bdnz` (1x counted loop). At GC/3.0a5.2
  -O4,p the `for (n=16;n!=0;n--)` form unrolls 8x, the do-while emits
  addic./bne, and -O4,s (native counted) breaks the sibling functions.
  The counted 1x loop requires the WHOLE-FUNCTION `#pragma optimize_for_size
  on` (a mid-function pragma does NOT suppress the unroll; the pragma also
  suppresses unrolling per the CArtsList stmw note). The pragma alone forces
  an stmw frame; per-object `extra_cflags=["-use_lmw_stmw off"]` (appended
  AFTER the lib-level `-use_lmw_stmw on`) restores the retail's individual
  stw's. Also: a struct member typed `u32 arr[32]` at the copy base makes
  MWCC see a u32-array copy and unroll regardless of the pragma — keep the
  member as a byte buffer. Result: 0 structural, 8 pure reg_swap (98.2%
  objdiff); witness blocked by the ABI rho (out->r31 vs retail handle->r31).

- **Unit compiler flip GC/3.0a5.2 reproduces a duplicated-guard dead branch
  that Wii/1.1 merges (monolib code_804DEDA8 func_804DFB88, 0x6c, FULL_MATCH
  + func_804DF7A4 19.2%→100%):** retail `cmpwi r3,0; blt end; blt end; lwz
  count; cmpw; ble end` — the TWO `handle < 0` blt's to the same exit are
  reproduced ONLY by GC/3.0a5.2 (and a3.4) with THREE separate goto guards:
  `if (handle<0) goto end; if (handle<0) goto end; if (count<=handle) goto
  end;` — Wii/1.1 merges the duplicates into `cmpwi r3,-1; ble` and inverts
  the count branch to `bgt body; b end`. The whole unit was mis-configured:
  flipping `Object(NonMatching, "monolib/src/core/code_804DEDA8.cpp",
  mw_version="GC/3.0a5.2")` (was default Wii/1.1) kept all previously
  matched functions at 100% and lifted two stubs to FULL_MATCH. Reusable:
  when a duplicated-condition dead branch won't reproduce at Wii/1.1, check
  whether the sibling functions in the TU match better under GC/3.0a5.2 —
  the retail unit may simply be GC-compiled (monolib core units are).

- **Member-dtor declarations + `optimize_for_size` stmw frame (kyoshin
  CMenuArtsSet __dt__9CArtsListFv, 0x78, FULL_MATCH):** retail dtor = stmw/lmw
  frame + four direct member-dtor calls (+0xE8 CSysWin, +0x74 CArtsInfo,
  +0x34 CScrollBar, +0x4 UnkClass_8045F564) with `li r4,-1` then
  `if (flag > 0) delete`. With opaque layout-view members (u8 data[0x74]
  buffers, no ~ declared) MWCC INLINES the member destruction (calls the
  sub-member dtor at +4, drops the +0x74 call) and emits individual stw's.
  Fix: (1) declare `~CSysWin();` in CSysWin.hpp and `~CArtsInfo();` in the
  TU's hpp so the auto-generated dtor emits the four retail bl's; (2) wrap
  the empty body in `#pragma push / #pragma optimize_for_size on /
  CArtsList::~CArtsList() {} / #pragma pop` for the stmw r30,8(sp)/lmw
  frame. Declarations only — verified no regressions in the CSysWin/CCol6
  consumers. Reusable: opaque member views hide the member dtor -> auto-dtor
  inlines the sub-object destruction; declare the member dtor (even without
  a body in that TU) to get the retail bl.

- **sprintf vararg vs format role swap + buf-size frame trim (kyoshin
  code_80135FDC func_80136B4C, 0x68, FULL_MATCH):** retail `sprintf(buf,
  lbl_eu_80500664, fmt)` — the r5 PARAM is the sprintf VARARG while the
  FORMAT is the constant extern (fmt is never used as a format); the 4th
  func_80136A1C arg is the value param, not 0. After fixing both, the only
  residual was a 16-byte frame overrun from `char buf[528]` — the retail
  frame (0x220, saved regs starting at 0x214) implies buf[520] (0x208):
  `char buf[520]` gives byte-identity (100%). Reusable: when the decomp
  frame is 16 bytes larger with all instructions identical, trim the
  buffer/array size until the saved-reg offsets line up — MWCC pads the
  frame to 16 but the retail layout has no pad.

- **`#pragma optimization_level 1` for base-add-before-store pool allocators
  (CScnItemModelNw4r func_80488954, 0x30, FULL_MATCH):** retail
  `lwz r0,0x860(r3); add r5,r3,r0; add r0,r0,r4; stw; addi r3,r5,0x864` computes the
  return base (`this+used`) BEFORE the counter store; -O4,p sinks the base-add after the
  stw (`add r0,r5,r4; stw; add r3,r3,r5; addi`) for every source shape (used-local,
  array-index, ptr-add, ret-intermediate, separate defs, inline accessor) and every
  compiler (Wii/1.0→1.7, GC/3.0a3.4→a5.2, -O2/-O3/-O4,p/s, -schedule off, -ipa off).
  Global `-opt level=1` reproduces the retail order byte-for-byte but breaks sibling
  thunks (tail-call folds disappear → frames). The per-function
  `#pragma push / #pragma optimization_level 1 / #pragma pop` around ONLY this function
  reproduces retail exactly with zero sibling regressions: 0 structural, 0 reg_swap,
  100%. Volatile counter (`volatile u32 used` member) is still required for the retail
  double-read in the success path (goto-gate layout gives the `blt`/inline-return-0
  polarity). Reusable: when the retail schedules a dependent add of a volatile reload
  BEFORE a store and -O4,p sinks it, try per-function `#pragma optimization_level 1`
  (also see sibling func_8048BD1C, same pattern, same fix applies).

---

### u8-return vs full-word compare + cast-only virtual interface for r12 dispatch (FULL_MATCH ×2 fixes)

`func_801CE974` (us-801d03c8, CItemBoxGrid.cpp) had two residuals, both fixed
to 100%:

1. **Return-type mask:** retail `cmpi r3,0` (full word) vs decomp
   `rlwinm r0,r3,0,24,31` (byte mask) after a `getField27()` call. The header
   declared `u8 getField27()`; MWCC's `!u8val` inserts the rlwinm mask. Retail
   compares the full word, so the recovered return type is `int` (the body
   still returns a byte — zero-extends into r3 with no extra codegen). Rule:
   when a caller-side `cmpi` vs `rlwinm` mask appears on a method result, the
   declared return type is too narrow; fix the header to `int`/`u32`.
2. **Manual-cast dispatch colors the vtable base r5; real virtual dispatch
   colors r12.** `((void(*)(void*,void*))(*(void***)(p+0xa0))[4])(...)` emits
   `lwz r5,160(r31); lwz r12,16(r5)`; retail (and a real virtual call) emits
   `lwz r12,160(r31); lwz r12,16(r12)`. Fix: a minimal cast-only interface
   with the target as the 3rd declared virtual (lands at +0x10 under -RTTI on,
   which inserts two hidden slots at 0/4):
   ```cpp
   struct CItemBoxObjA0Vt { virtual void _v08(); virtual void _v0C(); virtual void _v10(void*); };
   reinterpret_cast<CItemBoxObjA0Vt*>(p + 0xa0)->_v10(temp);
   ```
   The witness's abi-boundary gate (r5→r12) is avoided because MWCC's virtual
   dispatch uses r12 for the vtable base.

---

### u16 flag clear — wrap mask rlwinm 17,15 via bitfield clear (func_804CE140, FULL_MATCH)

`monolib/src/effect/code_804CC2B8.cpp` func_804CE140 (us-804d22b4): retail `lhz r0,6(r3); rlwinm r0,r0,0,17,15; sth r0,6(r3)` — clears u16 bit 0x8000 with the WRAP mask 0xFFFF7FFF. The prior SOFT-CAP comment claimed MWCC "optimizes the wrapping mask to non-wrapping 0x7FFF (rlwinm 17,31)" and called it semantically equivalent — true, but the mask is **not** provably-from-`& ~0x8000`: a 17-compiler sweep (GC/1.3..Wii/1.7) × `u16 &= ~0x8000`, `& 0xFFFF7FFF`, `DECOMP_PPC_RLWINM`/`__rlwinm`, volatile reads, tmp-vs-direct, and cast variants ALL normalize to `rlwinm 17,31` (MWCC knows the lhz value's upper bits are zero and canonicalizes the mask).

**The working shape is a u16 bitfield clear** (same as `CScheduleFlags::b15` in code_804E36DC.cpp): `struct EffectSceneFlags { u16 b15 : 1; u16 : 15; };` then `((EffectSceneFlags*)&self->field_0x06)->b15 = 0;` → byte-exact `rlwinm r0,r0,0,17,15`. Bitfield clears bypass the mask canonicalization. 100.0% FULL_MATCH, no policy exception needed (no DECOMP_PPC_RLWINM).

---

### register_mapping.md levers applied to near-miss batches (2026-08, Wii/1.1) — three proven shapes

Batch session matching pure-reg-swap CODE_MATCH targets with
`docs/register_mapping.md`; every target below reached FULL_MATCH with a
`full-instruction-match` certificate. Three reusable shape levers:

1. **3-float struct copy: FPR coloring follows the float LOCAL declaration
   order; load order follows the assignment order.** Retail pattern
   (`monolib/src/lod/code_80468434.cpp` func_8046A280/A318, us-8046e250/
   us-8046e2e8):
   ```
   lfs f2, 8(rX); lfs f1, 4(rX); lfs f0, 0(rX);   ; loads DESCENDING
   stfs f0, 0(rY); stfs f1, 4(rY); stfs f2, 8(rY)  ; stores ASCENDING
   ```
   `*self = *src` (Vec) or forward member-wise emit FORWARD loads with x→f2
   (4 reg-swaps); reverse member-wise (`self->z…; self->y…; self->x…`) emits
   retail's FPRs (z→f2, x→f0) but REVERSED stores (2 reg-swaps). The exact
   shape: declare `float x, y, z;` **first** (drives FPR allocation x→f0,
   y→f1, z→f2 per Rule C low→high), then **assign in reverse** `z = src->z;
   y = src->y; x = src->x;`, then **store forward** `self->x = x; self->y =
   y; self->z = z;` → 0 diffs. (nw4r `VEC3` class copy = same as plain Vec.)

2. **Address-add operand order: parenthesise `(scaled + const)` as a unit on
   the RIGHT of the base add.** Retail `add rD, scaled, base` (scaled FIRST)
   vs decomp `add rD, base, scaled` — one-byte diff, witness-ineligible, and
   plain `idx*0x2c + (u32)self + 0x1c` emits base-first. `(u32)self +
   (self->…field_0x2 * 0x2c + 0x1c)` (or `base + (mid * 4 + 8)` for a load
   address with the +8 folded into the lwz) emits scaled-first. Applies to
   `getFP__FPCc` (ocBdat, us-8003ae8c) and func_8046A11C/A1A0 (us-8046e0ec/
   us-8046e170).

3. **Tail pointer arithmetic: build incrementally with `+=`.** `char* dataPtr
   = base + dataOff + rowBytes + colDataOff;` gave a 7-way scratch rotation
   in getBdatStringColumnValue (us-8003b148); `char* dataPtr = base +
   dataOff; dataPtr += rowBytes; dataPtr += colDataOff;` aligned the volatile
   pool (rowBytes r4, colDataOff r0, ptr r7) exactly → 0 diffs.

Also from this batch: CMenuKizunaTalkList::Init tail reads were off by the
tempList frame base (tempList+0x148C not +0x14EC — the +0x60 base was double-
counted); Move's `(x & mask) != 0` vs `& mask` changes rlwinm normalization
(bit-31 form SH=31-k vs in-place SH=0,k,k); COption func_8029C4F4's switch
case set was 1,2,4,5,6,7,8,10 (0/3/9 empty) — Wii/1.1 truncates trailing empty
cases so the full 0..10 table needs the real case set; ocBdat func_8003B748
guard was `(elemType-6) <= 1 → shared *4 tail` (bc 4,1 = branch if NOT GT),
expressed as `if ((u32)(elemType-6) > 1) {switch} else {scale4: *4}` with
`goto` from case 3 (a flat `if (…) goto scale4; switch…; scale4:` lets switch
`default` fall through into ×4 — wrong).

---

### Tail-call thunk into an adjacent getter: split the merged retail symbol (CScnVirtualLight func_80493C08/func_80493C10, FULL_MATCH ×2)

Symptom: retail `func_80493C08` was annotated 0x10 bytes as
`addi r3,r3,0x54; b .+4; addi r3,r3,0xB8; blr` — the `b .+4` looks like a
"scheduler barrier" but is actually a **tail call into an adjacent second
function** (`func_80493C10 = addi r3,r3,0xB8; blr`). The annotation merged
two 0x8-byte functions into one 0x10 symbol. ~80 single-function probe shapes
all fold to `addi r3,r3,0x10C; blr` (documented plateau) because the two adds
are in ONE function.

Fix (monolib, Wii/1.1, `-ipa file -inline auto`):
1. Split the symbol: `func_80493C08 = .text:0x80497C7C; size:0x8` +
   `func_80493C10 = .text:0x80497C84; size:0x8` in config/us/symbols.txt
   (retail .o regenerates via the `split` ninja rule; depfile tracks symbols.txt).
2. Reconstruct as two `extern "C" __declspec(noinline)` functions in source
   order (getter first or thunk first both work — MWCC emits the thunk's
   `b` as a REL24 reloc resolved to +4 when the getter lands right after):
   ```cpp
   extern "C" __declspec(noinline) void* func_80493C10(void* self) {
       return (void*)((u8*)self + 0xB8);
   }
   extern "C" __declspec(noinline) void* func_80493C08(void* self) {
       return func_80493C10((void*)((u8*)self + 0x54));
   }
   ```
3. `extern "C"` is required: without it MWCC emits the mangled
   `func_80493C10__FPv` reloc name (reloc name drift, §1 fix).

Both functions reached 100% (FULL_MATCH). Reusable pattern: a `b` whose
target is the immediately-following instruction = tail call into an adjacent
function — check symbols.txt for a merged-symbol annotation before treating
it as a compiler artifact. The noinline leaf + tail call forces MWCC to keep
the `b` instead of folding the two adds.

**2026-08 RESOLVED (agent-structural):** the `b .+4` in `sinit_804DAF58` (us-804df214) is **NOT a scheduler barrier — it is a cross-function tail call into an adjacent helper** that the annotation merged into one 0x18 symbol. The DOL bytes `li r3,dest@sda21; b +4; lis/addi src@ha/l; stw r4,0(r3); blr` are TWO functions: the sinit thunk `li r3,&dest@sda21; b func_804DAF60` (0x8) + the helper `lis/addi r4,src@ha/l; stw r4,0(r3); blr` (0x10) at 0x804DF21C. The 2026-08 re-analysis saw "no reloc at +0x4" because dtk bakes intra-symbol branch displacements — with the merged annotation the branch looks baked, but splitting the symbol (`sinit_804DAF58` size 0x8 + new `func_804DAF60` size 0x10 in symbols.txt, re-run `dtk dol split`) turns it into a proper REL24 reloc and the two-function C reproduces it exactly:

```cpp
extern "C" __declspec(noinline) void func_804DAF60(void* dest) {
    *(void**)dest = (void*)lbl_eu_8056FD68;   // vtable as char[] keeps lis/addi address constant
}
extern "C" __declspec(noinline) void sinit_804DAF58() {
    func_804DAF60(&lbl_eu_806659E0);          // void body -> tail call `b`, no frame
}
```
Key: the sinit must be **void** (a trailing `return` kills the tail call and forces a frame), the helper is `__declspec(noinline)` (keeps the `b` instead of inlining/folding), and the vtable source is `char[]` (address constant via lis/addi — a pointer-typed extern emits an SDA21 `lwz` load instead). Both functions reached FULL_MATCH. The other merged-symbol sinits (`sinit_804DB0D8` etc., `sinit_eu_804F9FA4`) should re-split the same way — check for a `b .+4` tail-call boundary at +0x8 before treating them as ceilings. See also CScnVirtualLight func_80493C08/C10 (same merged-thunk resolution).

---

### u16 param + `(s16)value >= 2` emits the retail extsh (LODMemMan func_8046F024, FULL_MATCH)

Symptom: `if (mode) { field_0xBE = value; flag |= 0x100; if (value >= 2) field_0xBE = 1; }` with an `s16 value` param — MWCC folds the sign extension and compares the raw register (`cmpi r5,2`), 24 insns vs retail 25.

Retail emits `extsh r0,r5; cmpi r0,2` (sign-extend for the compare) while storing the RAW register (`sth r5,0xBE`). The `s16` param type is already "sign-extended" per ABI so MWCC omits the extsh.

Fix: declare the param as **`u16 value`** (raw register, matches the `sth` of the full value) and write the comparison as **`(s16)value >= 2`** — the explicit cast forces MWCC to emit `extsh r0,r5` for the compare. This is the general rule: when retail sign-extends a short for a comparison but stores it raw, the param is `u16` + the compare uses a `(s16)` cast.

---

### Doubled-beq family: `if (old) { if (old) { call } store0; }` near-miss class

func_80471794 (99.8%), func_804719FC (99.8%), func_804717FC (92.9%): all reconstruct with the `int old = lbl; if (old) { if (old) { deallocate(old) } lbl = 0; }` + an `old` local (the local is REQUIRED — direct global reads fold the double-beq). Residual is always exactly 1 reg_swap: the retail's dead second `beq` skips BOTH the call and the trailing `lbl = 0` store (targets past it); MWCC keeps the store inside the second beq's skip region. The witness cannot certify the branch-displacement field (slot 10 non-register bits).

**2026-08 correction (agent-codematch, both 100% FULL_MATCH):** nesting the trailing store INSIDE the inner redundant if is the winning shape, NOT the flat `{ if { call } store }` form. Both conditions are the identical `old != 0` test, so the nested shape is semantically unchanged, but the inner beq's skip region now includes the store — retail's branch target past it. `if (old) { if (old) { call; store; } }` → byte-identical on func_80471794 (0x68) and func_804719FC (0x74). The earlier "14 structural" report for the nested form must have used a different shape (e.g. an `else` or a re-read instead of the `old` local).

---

### u64 copy form fixes two-s32 store-pair register swap

`SFMPS_Seek` (us-803c55d4, sfd_mps.c): the 8-byte pairs
`self[0xee8..0xeec] = raw_hdr[0x18..0x1c]` and `mps_sub[0x10..0x14] =
raw_hdr[0x20..0x24]` plateaued at 89.3% (8 pure reg_swap, 0 structural) with
every two-s32 temporary shape (v0/v1 reuse, fresh vars, top/bottom
declarations, assignment order, store order, direct store expressions):
MWCC always emits `lwz r4,0x18; lwz r0,0x1c; stw r0,0xeec; stw r4,0xee8`
(first-loaded → r4, first-stored → r0) while retail emits
`lwz r0,0x18; lwz r4,0x1c; stw r4,0xeec; stw r0,0xee8` (first-loaded → r0).
The winning shape is a **single u64 load-store**
`*(u64*)((u8*)self + 0xee8) = *(u64*)((u8*)raw_hdr + 0x18);` — MWCC colors the
pair exactly like retail (loads 0x18→r0, 0x1c→r4; stores r4→high, r0→low) and
100% byte-identical. Mechanism: the u64 access makes MWCC allocate the two
words as one 64-bit value (low word born first → r0), whereas separate s32
loads allocate the second-defined temp to r0. Boundary: the mps_sub+0x2c/0x30
pair (same 8-byte span) must stay TWO s32 stores with a reused v0 temp — the
u64 form there breaks the earlier `li r3,0` scheduling (96.0%). Rule: for
8-byte store pairs whose low word is loaded first and stored second, try the
u64 copy form; if the surrounding code has an interleaved constant store,
keep the two-s32 form for that pair.

---

### Class/typedef-typed labels: include/lbls_typed.hpp + type-home gates

Real-typed class/typedef labels (kyoshin/monolib/nw4r classes, `OSThread*`,
`nw4r::ut::detail::RuntimeTypeInfo`, typedefs) live in `include/lbls_typed.hpp`
as plain externs in BOTH builds; their PC definitions stay raw bytes in
data_defs.cpp (byte fidelity -- a C++ initializer cannot reproduce arbitrary
retail bytes for a class object). The header is deliberately NOT included by
data_defs.cpp: its type providers (SDK headers like gx.h) declare globals
data_defs.cpp also defines (`__GXData`, `__files`) -- same-TU redefinition.

Three gates decide what may migrate (all enforced in `lbls_gen.py generate`):
1. find_type_home resolves the declaring header (definitions over forward
   decls, include-tree preference, `} Name;`/`typedef`/`(*Name)(` forms,
   ambiguity -> stay per-TU).
2. The home must be self-contained (`clang++ -include <home>` parses alone).
   kyoshin/monolib headers are NOT self-contained by convention (they rely on
   TU include order, e.g. reslist.hpp's mStartNodePtr template only parses
   with prior includes) -- so most kyoshin/monolib class-typed labels stay
   per-TU. This is correct: including them from a header would break every TU.
3. Per-entry extern-form test (catches namespace-relative `detail::X` and
   homes that declare the label itself with a different type) + a full-header
   parse. data_defs.cpp's own __MWERKS__-guarded body keeps the rest.

Type conflicts: 43 of 53 canonicalized via tools/coop/lbls_canonical.json
(a per-address canonical type chosen from site-level usage analysis --
address-of / cast / decay usage is type-agnostic, so one type serves every
site with zero codegen change; validated by a usage-form checker that rejects
scalar-canonical-with-indexing and object-canonical-with-member-access). 8
stayed per-TU (genuine scalar-read-vs-index / struct-view-vs-word-index
conflicts, e.g. lbl_eu_8065FC18 allocator global). 2 are FIXABLE_SITE
(lbl_eu_8065D138: code_804B2FF0.hpp pointer decl vs object usage;
lbl_eu_80661A40: scalar-vs-array decl changes the SDA addressing mode) --
documented in .scratch/type_conflict_report.json, source edits not applied.

Class-typed labels now get REAL PC definitions where possible (dual-mode):
port/lbls_typed_data.cpp (#define LBLS_DEFINE_DATA + #include <lbls_typed.hpp>)
defines them in a TU separate from data_defs.cpp (whose type providers would
clash with the SDK globals data_defs defines). Trivially-copyable object
labels use C++20 `__builtin_bit_cast(T, (unsigned char[N]){retail bytes})`
(byte-exact; verified sizeof-equality on the host, else demoted to
extern_only); pointer labels use `(T*)0xADDR` (real 8-byte PC pointers).
ctor-class labels (RTTI objects) cannot bit_cast and stay extern-only (raw
bytes in data_defs.cpp). The typed-header gate bisects cross-home conflicts
(e.g. a kyoshin home redefining nw4r types) so only the offending entries
drop to per-TU.

Migration result is deliberately small (32 labels) -- the gates trade coverage
for safety; the machinery is ready to pick up more labels as headers become
self-contained.

---

### Dual-mode area headers: `extern` on MWCC, data definitions on the PC port

`tools/coop/lbls_gen.py generate` emits each `include/lbls_<area>.hpp` as a
flat list of `LBLS_ENTRY(ext, dfn, init)` lines. The macro is defined ONCE in
`include/types.h`, next to the u32/s32 world-guard:

```c
#if defined(__MWERKS__) && !defined(NONMATCHING)
#  define LBLS_ENTRY(ext, dfn, init) extern ext;          // matching build
#elif defined(LBLS_DEFINE_DATA)
#  define LBLS_ENTRY(ext, dfn, init) dfn = LBLS_UNWRAP init;  // port data TU
#else
#  define LBLS_ENTRY(ext, dfn, init) extern ext;          // port, other TUs
#endif
#define LBLS_UNWRAP(...) __VA_ARGS__   // strips the paren-wrapping of init
```

- The MWCC matching build sees exactly the original extern declarations
  (textually identical modulo const/float-spelling/array-bound, all
  codegen-neutral — no `sizeof(extern)` exists in the corpus). `apply` +
  rebuild still yields byte-identical objects; the macro is inert unless a
  TU includes a lbls header, which the matching build does not.
- Non-MWCC builds get real definitions: `port/data_defs.cpp` sets
  `LBLS_DEFINE_DATA` BEFORE `#include "types.h"` (the macro picks its mode at
  definition time, so the flag must precede the types.h include), includes
  all five headers, then emits only the non-manifest remainder. Every other
  port TU sees `extern` — storage exists exactly once.
- Initializers carry RETAIL BYTES from `orig/<region>/sys/main.dol` decoded
  per type (u8/u16/u32 hex, s8/s16/s32/int and char as signed decimals to
  avoid narrowing, f32/f64 as finite literals, pointers as `(T*)0xADDR`),
  typed zeros for bss-family. Array bounds come from `symbols.txt` sizes.
- Macro-arg rules learned the hard way: declarators need NO parens (a
  parenthesized declarator `(u8 x[88]) = {...}` is invalid), initializers
  DO need paren-wrapping (`({0x0A, 0x41, ...})`) because brace contents
  contain commas; `void*` arrays need the PPC pointer width (4) for
  element count/bound — the scalar type-unit map has no entry for pointers.
- Source decls that are invalid for variables (`extern void lbl_x;`) are
  emitted as `u8`; `extern "C" void* x[]` with space collapses to `void*`.
- Verified end-to-end on the host: clang links data_defs.cpp (define) +
  an extern-mode TU + main; the retail value of `lbl_eu_804FA4C0[2]` (3)
  reads back correctly.

---

### Data-only TU matching: verify data sections like functions (`run.py data diff`)

Retail split objects carry the unit's data sections (`.data`/`.rodata`/`.sdata`/
`.sdata2`/`.bss`/`.sbss`/`.sbss2`). A data TU (typed or generated C) compiled
with MWCC should reproduce them; `tools/coop/lib/data_match.py` +
`run.py data diff <unit>` compares per section (bytes for file-backed, size +
alignment for bss-family, relocs where extractable) and exits non-zero on
mismatch — the data analog of the hexdiff loop, and the CI gate for data-only
TUs. `run.py data diff --all` lists every unit that defines data but mismatches.

### MWCC small-data section placement (verified Wii/1.1, `-O4,p`)

Where an object lands is decided by const-ness, initializer presence, and
size — NOT by the declaration alone:

| Declaration shape | Section |
|---|---|
| `int x;` / `struct s3 v;` (tentative, non-const, ≤ small-data threshold) | `.sbss` |
| `const struct s3 c = {0,0,0};` (const **with** initializer) | `.sbss2` (NOBITS) |
| `const struct s3 t;` (const, **no** initializer) | `.sbss` (NOT sbss2!) |
| `unsigned long long u;` (8-byte tentative) | `.sbss` |
| `#pragma sdata2` … `#pragma sdata` | forces the block into `.sbss2` / `.sbss` |
| non-const initialized small data | `.sdata` |
| const initialized small data (`f32`, ≤8B) | `.sdata2` |

Key gotchas:
- **`.sbss2` requires const + initializer together** — a const *tentative*
  still lands in `.sbss`. Fixing a `.sbss`→`.sbss2` residual (e.g. `bta_dm_cfg.c`
  `bta_dm_pm_cfg`: 3B `.sbss` vs retail 8B `.sbss2`) is: pad the struct to the
  retail size (retail was 8 bytes, align 8 — `UINT8 pad[5];`) **and** add
  `const` + a zero initializer. Consumers that only take the address
  (`p_bta_dm_pm_cfg = (T*)&bta_dm_pm_cfg;`) are unaffected by the const change.
- **`.sbss`/`.sbss2` are NOBITS** — the data-diff compares size + alignment
  only; the retail `.o`'s "bytes" for these sections are uninitialized DOL
  memory and must never be byte-compared (a naive diff reports false FAILs).
- **Symbol alignment matters for the final link** (bss addresses shift if
  `sh_addralign` differs) — data-diff compares section alignment too; pad to
  the retail alignment with a type that carries it (e.g. `f64`/`u64` array)
  or `alignas`.
- **MWCC emits `.sbss` symbols in REVERSE declaration order** (verified:
  decomp object layout is exactly the reverse of the source declaration list);
  `.sdata`/`.data` are forward. Declare globals in reverse of retail address
  order to reproduce the layout.
- Retail `.sbss`/`.sbss2` symbols are GLOBAL — drop `static` (static emits
  LOCAL bindings, flagged as data mismatch).

---

### Linker-GC'd class methods: remove from the TU, inline helpers in the header (agent 8, 2026)

When a retail split object contains only a few surviving functions from a
source file (the rest of the class implementation was linker-GC'd because the
game never references it), the decompiled TU must contain ONLY the survivors:
any reconstructed class code overshoots the .text budget AND emits
vtables/type-names the DOL-extracted retail object does not carry.

- Verified: `nw4r/src/g3d/g3d_anmvis.cpp` — retail split is 0x148 (two free
  `ApplyVisAnmResult` functions only); the AnmObjVis/AnmObjVisNode/AnmObjVisOR/
  AnmObjVisRes classes are absent from the retail DOL. The decompiled file had
  the full SDK implementation (0x1150 .text, .data/.rodata/.sdata2 vtables +
  type names). Fix: delete the class implementations; move the small helpers
  the surviving functions inline (TestExistence/TestDefined) INTO the header
  so `ApplyVisAnmResult` still inlines the binding check; the class stays
  abstract (pure virtuals) so no vtable is emitted. Result: .text 0x148 exact,
  all data sections empty, 2/2 functions byte-identical.
- Check first whether OTHER decompiled TUs reference the removed methods
  (virtual calls through the header declarations are fine; direct calls are
  not). In this case the only consumers call the free functions / use the
  pointer, so removal is link-safe.

---

### Reconfigure re-split: retail data ownership follows named decomp symbols (agent 8, 2026)

`configure.py --map` re-runs the splitter against the current decomp object
symbol table. Data addresses whose symbols the decomp defines (e.g. the named
`CCol3::white/gray/...` statics in `monolib/src/math/CCol3.cpp`) get
RE-ASSIGNED to that unit's split — `data diff` then compares against a retail
object that owns the .bss and the unit passes with NO source change (CCol3:
.bss 0x84 MATCH after re-split). Anonymous pool labels (`@N` float/double
magic in CScnFrame) and vtables the decomp emits under compiler-generated
names are NOT re-assigned — those stay mismatches. If a unit's data diff fails
only on named statics the source already defines, re-running the splitter may
be the intended fix rather than externing the data.

---

### const-self lever closes the load-hoist family — plus three sibling patterns (Wii/1.1 `-O4,p`)

**1. `const` on the self parameter hoists the first member load above the LR-save store.** Retail prologues like `stwu; mflr; lwz r3,0x6c0(r3); stw r0,0x14(r1)` (load BEFORE the LR save) were a documented "not reproducible" plateau (CfObjectMove_UnkVirtualFunc9 us-800bec0c, CfObjectModel vfunc52/53/56, CScnIdMan func_8049E51C). The lever: declare the parameter `const T* self` — MWCC then hoists the load above the compiler-generated store (the const object can't be modified by it). Verified: CScnEffectActNw4r getters (us-8049fff0/804a0018/804a0040/804a0068), CfObjectMove_UnkVirtualFunc9 (us-800bec0c), CfObjectModel vfunc56 (us-800bbc84) all → 100% FULL_MATCH. For MEMBER functions, constness re-mangles the symbol (Fv → CFv), so use the forced-name free-function form: `extern "C" int Name__Q22cf12CfObjectMoveFv(const cf::CfObjectMove* self)` — the vtable slot keeps referencing the same unmangled name. Non-const members, `-O4,s`, all GC/Wii versions, and scheduling pragmas do NOT reproduce the hoist.

**2. Dead second beq on a duplicated test: `&&`-form on a LOCAL keeps both beq; the global folds.** `__dt__8009D72C` (us-8009e104, 0x38) retail: `cmpwi r3,0; beq epi; beq epi; bl __dl__FPv; li r0,0; stw` — TWO beq to the same join. `if (g != 0 && g != 0)` on a GLOBAL folds to one beq (probe-verified across all compilers); `u32 p = g; if (p != 0 && p != 0)` on a LOCAL keeps `cmpwi + two beq` (one cmpwi, CSE'd test, dead second branch). Doubly-nested `if (p) { if (p) { … } }` folds for globals too; the CKizunagram member case keeps two beq only when the two targets differ (beq epi / beq zero-block).

**3. u8 truncation via memory round-trip = deref-of-storage.** `func_800A32C4` (us-800a3b8c): retail `bl getBdatStringColumnValue; stw r3,8(sp); lbz r3,8(sp)` (0x40). The `(u8)` cast folds to `rlwinm r3,r3,0,24,31` (0x3c). Write `u32 v = call(...); return *(const u8*)&v;` — the stw/lbz round-trip (CfObjectNpc MWCC_CASES rule; a bare `&call()` is "not an lvalue", so use the local).

**4. `#pragma optimize_for_size on` merges r30/r31 saves into stmw/lmw + fixes the copy order** — confirmed again on the CTagProcessor tag-writer family (func_801289B4 us-80129480, func_80128A70 us-8012953c, func_80128BB0 us-8012967c, all 0x48 → 100% FULL_MATCH). Retail prologue `stwu; mflr; stw r0; stmw r30; or r30,r4,r4; or r31,r6,r6`; plain `-O4,p -use_lmw_stmw on` emits `or r31,r6,r6` first + reversed `stw r31,28(sp); stw r30,24(sp)` pairs. The pragma (scoped on/off around the function) makes MWCC copy r3→r30 first and merge the saves (CPartyState func_801FD0A0 pattern).

**5. MWCC rejects `extern const float` vs `extern float` redeclarations of the same symbol** — a new non-const `extern float lbl_eu_8066A20C` in CActParamAnim.hpp (added 2026-08-14) silently broke the whole CfObjectModel TU build (stale .o since Aug 13, "identifier redeclared as 'float'" at the include site). Align the redeclaration qualifier with the first declaration (`extern const float`).

---

### Deleting a polymorphic layout: `delete` through the TYPED pointer reproduces the virtual dispatch + dead second beq (Wii/1.1 `-O4,p`)

`func_80285ABC` (us-80287f40, 0x68) and `CArrow3D::Term` (us-80122980, 0x70) both release an `nw4r::lyt::Layout*`. Retail: `lwz r3, 0x70(r31); cmpi r3,0; beq skip; beq zero; lwz r12,0(r3); lwz r12,8(r12); li r4,1; bcctrl; zero: li r0,0; stw r0,0x70(r31); skip:` — virtual slot-2 dtor with deleting flag 1, TWO beq's on the same test. The matching source is a plain `if (layout) { delete layout; mLayout = 0; }`:
- `delete` on a `Layout*` (polymorphic) dispatches the vtable dtor with flag 1 AND carries its own null-check — that null-check IS the retail's dead second beq (targeting the `mLayout = 0` block).
- `layout->~Layout()` emits the flag-0 call; `delete` on a `u8*` emits a DIRECT `bl __dl__FPv` (no virtual dispatch); a manual `((void(*)(void*,int))vt[2])` cast emits scratch `lwz r5` loads instead of the clean `lwz r12,0(r3); lwz r12,8(r12)`.
- The zero constant stays in r0 (re-`li`'d after the call) with a single r31 save when the source doesn't hold it in a local across the call.

---

### Leading-constant expression shape flips scratch regalloc (CPassiveSkill func_80266930, FULL_MATCH 100%)

`(u8)(slot + col*5 + (row-1)*25 + 1)` tail-calling a void setter left a fixed 5-instruction scratch rotation (37.5%: retail colors the row-1 chain r0, col*5 reuses r5, accumulator starts r4; MWCC colored row-1→r4 and col*5→r0 regardless of parenthesization/statement order). Preceding the whole sum with the constant term — `(u8)(1 + slot + col*5 + (row-1)*25)` — reorders MWCC's virtual-register birth so the coloring matches retail byte-for-byte: `addi r0,r4,-1; mulli r5,r5,5; mulli r0,r0,25; add r4,r6,r5; add r4,r4,r0; addi r0,r4,1; clrlwi r4,r0,24; b`. Same opcode graph, different birth order → 100% FULL_MATCH, no pragmas/macros. Lesson: when a pure scratch `reg_swap` rotation resists parenthesization and statement reordering, try moving a literal constant to the head of the left-associative sum.
