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
| sibling [`mwcc-wii-1.1`](../../mwcc-wii-1.1) (`~/projects/mwcc-wii-1.1`) | Wii/1.1 compiler mechanism findings — start at `docs/WII_1_1_AGENT_GUIDE.md`; prefer over GC/1.2.5 `mwcc-decomp` transfers when they conflict |

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

## `*=` double keeps fmul dest=FRC (`fmul f0,f1,f0`) — `x * lbl` commutes (Wii/1.1 -O4)

- Symptom:   `lfs f1, x; lfd f0, 1.5; fmul f0,f0,f1; frsp` vs retail `fmul f0,f1,f0` (same loads)
- Cause:     `x = (f32)(x * lbl_double)` / named left/`f64` locals commute FRA/FRC so dest==FRA
- Fix:       write `x *= lbl_double` (same shape as a matching `x *= lbl_float` `fmuls`)
- Result:    operand order matches; lfd+frsp stay
- Confidence: repo_proven
- Applies to/a.k.a.: sdata2 doubles, dest-as-FRC fmul, commute peephole

## Member-init list stores emit in *declaration* order, not written order (Wii/1.1 -O4,p)

- Symptom:   a ctor whose member-init list is written flags-first still emits the flag stores in
  declaration order (after the pointer/word stores); retail has them first
- Cause:     MWCC Wii/1.1 ignores written list order for the scalar stores (probe:
  `struct S { int a,b,c,d; S(int x) : d(g),c(3),b(2),a(x) {} }` -> stores a,b,c,d)
- Fix:       when retail's store order differs from declaration order, the order must be forced
  through expression evaluation (e.g. a side-effecting initializer expression that assigns the
  earlier-stored members); reordering the written list alone is a no-op
- Result:    explains the SeqSound ctor case (natural list 87.2% / 4 structural vs side-effect
  initializer 100%); see MWCC_CASES `__ct__...SeqSound`
- Confidence: repo_proven
- Applies to/a.k.a.: member-init order, store scheduling, side-effect-in-initializer fakematch candidates


### Never declare `__RTTI__*` symbols

Declaring `extern u32 __RTTI__10IWorkEvent[];` (any type, inside or outside `extern "C"`) in a
TU that (a) forward-declares classes with `__declspec(novtable)` and (b) defines
anonymous-namespace classes with virtual functions, under `-ipa file`, defers an
**"illegal name overloading" (10322) error onto the LAST declaration/function at EOF** - the
diagnostic lands on whatever is compiled last, masking the real cause. The compiler's internal
RTTI name-lookup for the in-scope classes collides with the user spelling. Minimal repro:
novtable-predeclared class + anon-ns class with out-of-line virtual dtor + one `__RTTI__*`
extern decl + any trailing function definition.

**Fix: never spell `__RTTI__*` in such TUs.** Reference the typeinfo objects through legal
stand-in names (`rtti_10IWorkEvent[]`) in hand-built vtables and retarget those slots via
UNIT_RULES `retarget_relocs` to the retail `__RTTI__*` names (§17.6 reloc naming). The same
mechanism covers anonymous-namespace method symbols whose `@unnamed@` manglings are not legal
C++ identifiers. Verified: CDesktop data dissolution reached raw data-diff MATCH this way.
Confidence: repo_proven. Applies to: monolibdata2 dissolves of TUs owning class data;
`-ipa file` builds; DECOMP_FORCEACTIVE varargs are NOT the trigger.

---
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

### Compound load-mask-store (`g &= ~M` + reload) splits the web and flips scratch colors (CfGameManager func_80086490, FULL_MATCH)

`func_80086490` (us-80086e58, 0x158) sat at 83.7% with 0 structural + 14 pure reg-swaps:
a clean r5<->r6 exchange between the `pad` pointer web and the `enabledFlags`
mask web across the whole flag-clearing block. Invariant to: swapping the two
locals' declaration/statement order, and eliminating the pointer local via
direct global access. The witness cannot certify it (rho maps r5->r6; r3-r12 are
ABI-fixed lanes), so FULL_MATCH was required.

- Symptom:   retail colors mask=r5/pad=r6; decomp colors mask=r6/pad=r5 — every `and`/`stw`/`lwz` in the block swapped
- Cause:     `u32 enabledFlags = g & ~M; g = enabledFlags;` keeps load+mask+store as ONE high-degree web born after the pointer load, so the allocator colored the pointer first (low scratch r5); retail's web structure colors the mask first
- Fix:       write `lbl &= ~M;` (compound assignment) then reload into the named local: `lbl_eu_80663DF8 &= ~0x600230; u32 enabledFlags = lbl_eu_80663DF8;` — the store-back no longer shares the mask web, birth/degree order flips → 100% FULL_MATCH
- Result:    FULL_MATCH (was 83.7%, 14 reg-swap)
- Confidence: repo_proven
- Applies to/a.k.a.: any global read-mask-writeback feeding later uses; same family as Rule A/C levers in register_mapping.md but for scratch (volatile-pool) registers where declaration order is a no-op

---
---

### Swapped-operand range guards (`if (0 >= v) continue; if (N < v) continue;`) block MWCC's range-check fusion (CfGameManager func_8007E0D0 FULL_MATCH, func_8008360C residual)

Retail frequently guards a 1..N range with TWO separate signed compares
(`cmpi rX,0 / ble→skip; cmpi rX,N / bgt→skip`), but MWCC Wii/1.1 -O4,p fuses every
normal C spelling of that range check into the unsigned idiom
`subi r0,rX,1; cmplwi r0,N-1; bgt→skip` (1 instruction shorter, shifts the whole
rest of the function). Probed ~15 source shapes via .scratch mwcceppc probes
(rangefusion.c/rangefusion2.c):

- FUSES (all of these): nested ifs `if (v>0) { if (v<=13) }`; flat continues
  `if (v<1) continue; if (v>13) continue;`; single `||` condition; `&&` with body;
  goto form; reversed guard order; guards followed by more equality branches;
  volatile load; increment in loop body.
- `(u32)v > 13u` on the second guard blocks fusion but emits UNSIGNED `cmplwi`
  (retail uses signed `cmpi`) — wrong opcode.
- **WORKS: swap the operands — `if (0 >= v) continue; if (13 < v) continue;`**
  MWCC's fusion recognizer only matches `v OP const` operand order; `const OP v`
  keeps two separate SIGNED `cmpi` + `ble`/`bgt` branches, byte-identical to retail.
- Verified end-to-end: func_8007E0D0 went 52.6% → 100% FULL_MATCH with only this
  change (+ latch increment order `++entry, ++i`).
- Related unfixed residual: `(off>>2)<<2` index recombination NEVER folds to `off`
  in Wii/1.1 -O4,p — `p[off >> 2]` with a 4-stepping offset always emits
  `clrrwi/rlwinm r0,off,2` before `lwzx` (probed: signed/unsigned off, /4 form,
  byte-pointer cast → becomes pointer-WALK `lwz 0(rX); addi rX,4` instead,
  `optimize_for_size` pragma → no change). Retail bare `lwzx rD,rBase,rOff` with a
  4-stepping index register: PARTIAL solution found — `volatile s32* volatile p`
  (pointer itself volatile) + byte-pointer load blocks the pointer-walk transform
  and emits exactly bare `lwzx r3,r3,offIV` with a stride-4 IV (probe p15/p17),
  but the volatile stack-slot round-trip costs +8 bytes and perturbs prologue/
  call order; only viable when retail also pays that reload. Also ruled out:
  CSE'd double-load and keeping-read of off (still recompute clrrwi — MWCC never
  creates a masked IV), const pointer (does not block the walk), pointer-to-array
  typing (keeps mask). Open on func_8008360C.
- Confidence: repo_proven (probe-verified against Wii/1.1 mwcceppc).

---

### Small wrapper definitions need `__declspec(noinline)` or -inline auto inlines them into callers (CfGameManager func_80084F50, FULL_MATCH)

When a TU defines a tiny wrapper function under its RETAIL name while other code
in the same TU is being matched, `-inline auto` will inline the wrapper body into
every caller — the caller's `bl` then targets the wrapper's callee instead of the
wrapper symbol, breaking an otherwise-100% function.

- Symptom:   caller hexdiff shows one reloc drift: decomp calls the callee where retail calls the wrapper; everything else byte-identical
- Cause:     retail-named definition added without inline protection; MWCC inlined it at the call site
- Fix:       mark small standalone definitions `extern "C" __declspec(noinline)` (repo convention, cf. cfCam_zeroVec3) so they stay callable symbols
- Result:    func_80084F50 96.7% -> 100% FULL_MATCH (drift was self-inflicted by an unprotected definition)
- Confidence: repo_proven (Wii/1.1 -O4,p)
- Applies to/a.k.a.: any single-call wrapper/thunk added to a matched TU; also explains "caller regressed after I added a helper" reports
- Applies to/a.k.a.: any `lo < v && v <= hi` / `v < lo || v > hi` guard; loop-latch
  comma-increment order also matters (`++ptr, ++i` vs `++i, ++ptr` selects which
  `addi` the scheduler emits first).



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
r31`) byte-for-byte. Verified on kyoshin/cf/CtrlRemote CtrlRemote_FreeBuffer (0.0% →
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

### monolib effect code_804DB938 — CEVec3LerpPS / CEVec4LerpPS lerp kernels (Wii/1.1)

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
  variants all fail with error 33135 `<string not found>`. Bare-symbol forms
  (`li r0, sym`, `addi r0, r0, sym`, `li r0, sym(r0)`) all fail with error
  33023, so the no-modifier `lwz/lfs sym(rA)` SDA21 default does NOT transfer
  to addi-immediate operands — no inline-asm spelling emits SDA21 `addi`.
  The register-renaming witness also gates on the reloc slot (same symbol +
  addend, type 109 vs 4), and linked values genuinely differ (sda-offset vs
  low16), so witness/SMT certification would be unsound; the function is
  retail-dead SDK code in any case. The compiler *does*
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

`UnkClass_8047D2AC::{solveWalkStep,solveWalkPair}` use retail `mtspr GQR5` with `0x03070307` before the signed-fixed-point scene-manager calls and `0x00070007` afterward. The normal high-level candidate preserves the solver calls and state updates, but no source-level `OSSetGQR*` or scalar initialization reproduced the orphaned GQR writes; the bounded candidates stayed at 69.2%/76.1% and split-size PASS. Keep the candidate readable and do not add inline assembly or register tricks; revisit after the `UnkClass_8047E110` solver callee frontier is accepted.

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
| Text bytes identical, `code%` still low, witness gate `slot N: non-register bits differ` on a `switch` fn | Retail keeps the jump table in `.data`; decomp table routes different case indexes (mapping shifted) or a different bound immediate | **§8 jumptable routing** — match per-index table routing (keep no-op cases), same bound, rename `@NNN` cookie via `exact_renames`; see CItemBoxGrid GetCatLabelMsg |
| Two identical patterns, opposite regalloc | Shared locals forced one color | **§4** — block-scope a fresh pair |
| Wrong register for same opcode | Declaration order | **§4** — reorder locals |
| Wide-arm reuses `r30` for height (`lha r30` vs retail `lha r31`) after non-wide correctly uses `self=r30`/`height=r31` | Precise liveness: `self` dead on wide arm so Chaitin recycles `r30`; keep-alives scramble dual-`getRenderModeObj` schedule | **CGame::func_800395F4** soft-cap ~99.8%: non-wide `s32 height` + `spInstance` reload; leave wide `s16` — do not chase keep-alive commas/ternaries |
| `addi …, @stringBase0, imm` wrong imm / missing Restart→CGame pool | Pointer-table `force_active` does not pack MWCC `@stringBase0`; contiguous pool needs a `.text` FORCEACTIVE that references the literals in order | Keep non-vararg `force_cgame_strings(Restart,"",43,arc,brlyt)` then **`drop_text_symbols`** the 0x1C thunk in `postprocess_reloc_names.py` (`CGame.o`) so stubs still fit `0xD08` |
| Stack slot at wrong offset | MWCC alignment / padding | **§5** — use multiple smaller types (`CPnt16` × N not `CRect16` × 1) |
| Zeros/sign-extension: `extsh` vs none | `s16` local vs `int` | Use `int` unless retail has `extsh` |
| Struct offset off by 4/8 | Missing pad or wrong base layout | Fix header layout before tuning C++ |
| Bitfield `extrwi` pos off by 2 (e.g. bit 19 vs 17) | Adjacent bitfields swapped in MWCC MSB-first layout | Reorder fields in the bitfield struct to match retail extract (nw4r `TexMap::mBits`: `paletteFormat` before `anisotropy`) |
| `unk64`-style flag test: retail `rlwinm. r0,r0,0,28,28` but decomp `rlwinm.,0,27,27` (or branch direction inverted) | **PPC bit numbering is MSB-first**: `rlwinm mb=m,me=m` masks PPC bit `m` whose *value* is `0x80000000>>m` (PPC bit 0 = MSB/0x80000000, PPC bit 31 = LSB/0x1). A C source `(x & 0x8)` maps to PPC bit 28, while `(x >> 30)&1` extracts C-bit 30 = PPC bit 1 (value 0x40000000). Decomp agents repeatedly confused the two (wrote `>> 30` for the value-0x2 flag). Bit-test polarity also flips: retail `beq skip` (continue when bit set) vs `bne skip` (continue when clear) means the ternary is `(flag) ? 1 : 2` not `? 2 : 1` | Translate `extrwi rD,rS,1,b` → `(x >> (31-b)) & 1`; `rlwinm. rD,rS,0,m,m` (test) → `x & (0x80000000>>m)`. Match the exact `beq`/`bne` skip direction (it encodes true/false polarity). `isPC`/`isNPC`/`isENE` (`ocUnit.cpp`): retail `extrwi r0,r0,1,30; neg r5,r0; addi r0,r5,2` ↔ source `retVal.type = ((obj->unk64 >> 1) & 1) ? 1 : 2` (bit-extract cond → MWCC keeps the branchless `neg/addi` idiom) |
| VM plugin funcs call `ArtsSelect_GetContainer` with a stale `r3`; decomp's `ArtsSelect_GetContainer(pThread)` emits an extra `mr r3,r30` → +4 bytes structural | `ArtsSelect_GetContainer` (`us-80187748`, FULL_MATCH) **ignores r3**; callers never need to set it. When a `vmArg*` precedes the call, retail leaves r3 stale while decomp reloads `mr r3,r30` | **RESOLVED**: `ArtsSelect_GetContainer` is now `void* ArtsSelect_GetContainer(void)` in `code_801862C0.hpp`+`.cpp`; all ~61 callsites in `ocUnit`/`pluginCfs`/`pluginCam`/`pluginBtl`/`CfObjectPoint` call `ArtsSelect_GetContainer()`. C-linkage symbol unchanged → impl FULL_MATCH preserved; ~40 ocUnit VM funcs jumped 50%→90%+. (Note: `pluginBtl`/`pluginCfs` have *pre-existing* unrelated compile errors — `cf::CfGameManager::getInstance` / `CtrlObjectParam_ResolveEquipItem` overloading — not caused by this change.) |
| `__dynamic_cast` RTTI args: decomp passes the global's **value** `(void*)lbl_eu_806618D8` → MWCC dereferences (`lwz`) | retail passes the **address** (`li r5, lbl_eu_806618D8@sda21`) | Pass `&lbl_eu_806618D8` (address-of), matching `CBattleManager.cpp`/`CVision.cpp`. Fixed across ocUnit.cpp |
| Same RTTI address (0x806618D8) has **two reloc names** across functions | retail uses `__RTTI__Q22cf13CfObjectActor` in `invin`/`lookAt`/`sendBattleEvent` but `lbl_eu_806618D8` in `setColi`/`delBuff` — **per-function** | Match the exact name each function uses (check `hexdiff --symbol X --brief` “Reloc drift”); declare `extern "C" void* __RTTI__Q22cf13CfObjectActor;` (from `CVision.hpp`) where needed |
| CfObject talk sub-object: source used `obj->mSubObj38` (offset 0x38) | retail reads `obj+0x74` for the `UIWin_CreateTalkWin`/`UIWin_CreateEveTalkWin`/vtable[0x50] sub-object arg | `mSubObj38` (header offset 0x38) is a **different** field used by other TUs — don't retarget the header. In ocUnit.cpp use `*(void**)((u8*)obj + 0x74)` for the talk sub-object (winTalk/talkMsg/sendNotify/getPartyHandle) |
| SDA vs far addressing | Different insn (`lwz@sda21` vs `lis`/`addi`) | Correct section/size via linker script |
| Wrong callee-save depth | `_savegpr_29` vs `_savegpr_28` | Shrink live ranges across large functions |
| Inlined vs outlined | Extra/missing `bl` | Match retail inline boundaries; IPA pragmas |
| Same body duplicated → ~99.3% regswap vs retail | Manual paste of callee body keeps callee's RA | Prefer `callee();` + `-ipa file` so IPA inlines in *caller* context (different Chaitin colors) |
| Wrong float constant pool | `lfs` from wrong `.sdata2` slot | `extern "C" const float lbl_eu_*` |
| Ternary vs `if/else` codegen | Extra `b` or `sel` | Toggle form |
| Ghidra `r13` SDA | Misleading decompilation | Set SDA bases in Ghidra |
| Retail materialises a struct base (`addi r3,rX,0x3e`) for a long run of stores; all pointer/volatile/field-store forms fold back to direct offsets | MWCC keeps a **walked pointer** in a base register but folds constant-index/field accesses | Declare `u16* q = &obj->sub.vDelta;` and advance with `*q++` per store; start one field before the run so MWCC materialises at the retail base after the first folded store (HBMMIXInitChannel tail, 594/594; **__MIXRmtUpdateSettings phase-3**: walk `u16* q = (u16*)((u8*)out + 0x102)` so the folded cur0 store lands at `258(r30)` and the run materialises `addi r3,r30,260` with `sth 0..28(r3)` — 167 structural → 0, FULL_MATCH us-8034f910) |
| 3-op load-order reg-swaps in a top-level sum (`lwz` order differs, adds identical) | MWCC rotates a top-level sum chain `[s0,s1,s2]` into loads `[s2,s0,s1]` (tree `((s2+s0)+s1)`) | Write the source in rotated order: retail loads `[panFrontL, fader, X]` require `fader + X + panFrontL`; sums nested in a larger tree (`(a+b+c)-30`) are NOT rotated (HBMMIXInitChannel) |
| 0 structural, pure reg-swaps, but `cycle` witness never certifies | **Gate 6 reject-list: any prologue that saves FPRs emits `stfd`+`psq_st` pairs (MWCC always does this for f14–f31 saves), and the register-renaming witness unconditionally rejects `psq_*`** — witness-ineligible no matter how clean the body diff. Also: commutative `add` operand-order swaps break rho (r6 maps to both r6 and r0) and the region-sliced fallback refuses. **Confirmed 2026-08-03: the witness also never applies to any function containing a `bl` call** — 0/11 witness-certified certs in the sidecar contain `bl`; opaque-EABI callee contracts make the terminal-state comparison diverge on callee effects. For ANY call-containing target, the only no-SMT acceptance is FULL_MATCH (byte-identical) **— 2026-08 REFINEMENT (nw4r campaign, 5 probes): bl does NOT categorically block the witness.** Four bl-containing pure-regswap/near-pure targets were cycled: the witness ran past calls and refused on real divergences — observed gate sequence: (1) mnemonic (`or` vs `rlwinm`/`stw`), (2) fields (frame-slot displacement differing by 0x4), (3) abi-boundary for BOTH FPR (`fpr f0 -> f1`) and GPR (`gpr r30 -> r31`) permutations that cross ABI-fixed positions, (4) deadline/cfg-exploration on loop-heavy bodies. Practical rule: a bl-containing target IS certifiable at EQUIVALENT_MATCH iff its residual reduces to a color permutation that never touches ABI-fixed registers AND its CFG is shallow enough to explore. The 0/11 sidecar sample likely reflected mnemonic/fields divergences hiding upstream of the bl question | Record stall with `next_change: accept via --smt out-of-band` (SMT is out-of-band per orchestrator). Skip further regalloc attempts — no source lever flips MWCC's callee-save colors for FPR-saving functions (hbm/seq.c `__HBMSEQInitTracks` 12 swaps, `HBMSEQRunAudioFrame` 16 swaps; `__HBMSEQReadHeader` 1 swap via rho conflict); for bl-containing targets the reg-swap levers are equally unlikely to pay off unless a FULL_MATCH shape is reachable (e.g. wpad `WPADiExcludeButton` r5↔r6 pointer color: 3 source variants no-op, 17 swaps — recorded for SMT acceptance). For abi-boundary rejections: the flipped colors feed ABI-fixed positions, so only original-source statement order resolves them (see attempts.jsonl us-803dd448/us-803e41e8/us-803f4600 packets) |
| **Branchy pure reg-swap pairs reject as "terminal pair diverges structurally — gpr r0"** | The cheap disjointness simplify (`_z3_simplify`) is a local rewrite: cross-path pairs of branchy functions build `And(X, Not(X))` from per-side byte-read memory equalities that are NOT structurally identical across sides, so `is_false` returns False and the impossible pair is compared (diverges on a lane live on only one path). **FIXED 2026-08 in `run_structural_witness`**: bounded QF_BV `unsat` fallback (`_path_conditions_disjoint_sat`, 10 s timeout, fail-closed on sat/unknown/timeout) proves the cross paths cannot co-occur and skips the pair. Sound: `unsat` for the conjunction is a proof of infeasibility. 142 tests green incl. `test_branchy_cross_pair_disjointness_sat_fallback`. Unblocked `CMMTex::OnFileEvent` (us-801184b8, rho r5↔r6, EQUIVALENT_MATCH witness-certified). Apply to any branchy reg-swap target whose witness reason is `structural … gpr r0` |



### 8. Retail `.data` switch jumptables — routing, bound, cookie, commuted add (CItemBoxGrid GetCatLabelMsg → FULL_MATCH, Wii/1.1 -O4,p)

When retail lowers a dense `switch` through a named `.data` jump table (`jumptable_eu_8053xxxx`, entries are `.4byte func+off`), four things must line up; text bytes alone can look identical while `code%`/data stay low and the witness gate reports `slot N: non-register bits differ (0x…)` on the `cmpli` bound:

1. **Per-index routing:** every case index must map to the same target as retail's table row — including no-op rows (retail often routes leading/trailing indexes to the merge point). Empty `case N:` fallthroughs fold away in MWCC and shrink the table; give no-op cases their own indexes with bodies moved to retail positions instead.
2. **Bound immediate:** the table covers `[0, highest routed case]`; a shifted mapping shrinks the `cmpli` bound (e.g. 11 vs 13) which the witness rejects as non-register bits (`XOR 0x6`).
3. **Cookie name:** MWCC emits its own `@NNN` table that cannot be named from source; rename via `UNIT_RULES["<TU>.o"].exact_renames = (("@NNN", "jumptable_eu_8053xxxx"),)` in `tools/postprocess_reloc_names.py`. The id **drifts with TU growth** (observed @16378→@16376 from one edit) — re-check after any TU change.
4. **Commutated `add` on base+index:** `p[idx + 0x62]` can emit `add r3, r0, r3` where retail has `add r3, r3, r0`; shape as a named pointer (`u8* entry = p + (s8)p[0x6f]; cat = entry[0x62];`) to keep the base register first.

Diagnose routing/bound diffs with the raw asm (`hexdiff --symbol X --asm`) rather than the byte-table: byte-compare hides `.data` content and shows relocated immediates as equal zeros.

5. **Dominator hoist vs per-case rematerialization (negative result, code_804BC9EC func_804BC9F4):** when retail rematerializes the record pointer inside every case block (`add r3, r27, r24` × 13 cases) but decomp hoists a single `add` before the `bctr`, do NOT try to defeat it from source — inlining the base+index expression into every case (removing the shared `u8* entry` local) does NOT help: MWCC GVN re-creates one value at the dispatch dominator and all cases reuse it (verified: identical 0x1e8 body, hoist merely moved). With GPRs plentiful under `stmw r14-r31` there is no register-pressure lever either. Treat as a §17.6 code-length residual: normalize the `.data` jumptable case-label addends via `UNIT_RULES[...].addend_patches` (CGXCache recipe) until the function matches by other means; re-derive deltas from a FRESH RAW build whenever anything perturbs the TU (-ipa file scheduling flips the variant), and never trust the shared `build/` object for measurements (hexdiff leaves it postprocessed in place).

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

## Implicit template-dtor co-emission under user-defined ctor+dtor — <when it applies>
- Symptom:   decomp `.o` carries unreferenced weak template dtors (e.g. `__dt__ LinkList<T,N>`, `LinkListNode`, TU-local `NonCopyable`, `AutoLock`) that retail never emitted anywhere in the DOL; zero relocs reference them; all call sites inline identically. Raw `run.py size` FAILs by the extras' bytes even though every function is 100%.
- Cause:     MWCC `-ipa file` emits standalone weak copies of member template dtors when the holder has a user-defined ctor **and** user-defined dtor (either alone does not trigger it), or when a complete holder object is constructed+destroyed in-TU (even fully inlined), or when a function-local static needs the dtor address (`__destroy_arr`). Verified by bisection: implicit-ctor probe clean; adding a user ctor emits `<244>`+`<PlayerHeap,24>`; adding in-TU construction emits the same pair. The bodies are offset-independent (call base-impl dtor + `operator delete`), so co-emitted spellings (`244`/`252`) are byte-identical to each other.
- Fix:       no source fix exists while the required ctor/dtor bodies stay (their 100% codegen is the trigger). Accept the raw-size FAIL and rely on the grandfathered `tools/postprocess_reloc_names.py` `UnitRules` (`exact_renames` + `drop_text_symbols`) at link time — same precedent as Matching `lyt_window` (raw +0x29C) and other Matching units with drops (`lyt_animation`, `lyt_group`, `CDeviceGX`, `dvd_broadway`). Never add new `UnitRules` (frozen/shrink-only); never chase the emission with source hacks.
- Applies to/a.k.a.: every `nw4r/snd` TU using `AutoLock`+`LinkList` members (`snd_SoundPlayer`, `snd_SoundThread`, `snd_ExternalSoundPlayer`, ...); `run.py size` raw FAIL vs link-time drops.
- Confidence: repo_proven
- Example:   us-80422840 (`snd_SoundPlayer` LinkList dtor; body 100%, unresolvable below)

## `cycle` digit-strip resolution needs a UNIQUE decomp candidate (template-Ofs fossils)
- Symptom:   `cycle` fails with `ambiguous decomp symbol '<...>' (0 candidates)` while `hexdiff` shows the same pair at 100%. The target's ctor sibling accepted fine (`us-80422820` FULL_MATCH) but the dtor will not.
- Cause:     `elf_symbols._resolve_candidates` digit-strip fallback only accepts a *unique* hit. Retail fossil names (e.g. `LinkList<BasicSound,256>` from a TU compiled against an older header where the node sat at `0x100`) strip to the same key as every live spelling (`244`, `252`), so two unavoidable co-emissions (see above) collide 2-ways and the witness pair never forms. The single-ctor case resolves because only one `LinkList` ctor exists in the object.
- Fix:       none at source level (any second `LinkList<BasicSound,N>` dtor collides after digit-strip; removing the needed ones breaks 100% bodies). Record the 100% hexdiff evidence in `attempts.jsonl`, keep the target ACTIVE, promote the TU via the existing `UnitRules` rename path when tooling allows. Do NOT rename registry symbols or add decoy instantiations to force uniqueness.
- Applies to/a.k.a.: `tools/ppc_equivalence/elf_symbols.py::_resolve_candidates`; `prove_unit_symbol` fallback path.
- Confidence: repo_proven
- Example:   us-80422840 vs us-80422820 (`snd_SoundPlayer`)

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

**novtable + member subobject: emit the ctor as extern "C" `__ct__…` (us-8004b700):** when retail stores the vptr *before* an embedded member ctor (`CActParamAnim`: `lbl_eu_805261C8`, zero +0x0C, then `__ct__13CActParamDataFv` at +0x10), a C++ member ctor cannot match — `novtable` drops the implicit vptr and C++ still constructs members before the body. Define `extern "C" T* __ct__…(T* self)` with the same mangling and write the stores/calls in retail order (offset-based if fields are private). Verified FULL_MATCH `0x48` on `__ct__13CActParamAnimFv`.

**Inline-empty base dtor elides the call in derived dtors (CTaskCulling dtor, us-801a4278):** MWCC only elides a base-class dtor call in a derived dtor when the base dtor's empty body is visible in the same TU. Retail's `IWorkEvent` header was `virtual ~IWorkEvent(){}` (inline-empty) — `~CTaskCulling` then calls only `~CProcess` (+ member `~COccCulling`), no `~IWorkEvent`, and the dtor is 0x78. When the dtor was moved out-of-line (IWorkEvent.cpp), the call reappeared (+0xC, dtor 0x84, unit 8 bytes over split budget). Fix: keep `virtual ~IWorkEvent(){}` inline-empty in the header, and keep a **strong copy** in the key-function TU (`src/kyoshin/CTaskGame.cpp` had `IWorkEvent::~IWorkEvent() {}` matching retail's strong symbol placement — remove it only if the header body replaces it; an out-of-line redefinition of an inline member errors with `(10333) object redefined`). Verified: derived `~Der` with a secondary inline-empty base emits only the offset-0 base call + delete; the decomp `~CTaskCulling` returned to byte-identity and the unit to 0x708 ≤ 0x70C.

**Pool-cookie reloc drift certified by the register-renaming witness (no SMT/linked DOL):** for byte-identical functions whose only diff is a TU-local pool reloc (`@N` vs `lbl_eu_*`), the mined reloc map (`reloc_map.py mine` → `retail_reloc_map.json`) canonicalizes both names via the decoder's `canonical_symbols` hook, so the **pre-SMT witness certifies directly** (`register-renaming-witness: N terminal pair(s) structurally equal under rho`) — no Z3, no `main.elf`. Verified: `getLevel__16CMCCylinderGaugeFv` (us-80224098, `@6134`→`lbl_eu_80668520`) and `__ct__16CMCCylinderGaugeF…` (us-80223cac, `@6092`→`lbl_eu_80668518`) both accepted EQUIVALENT_MATCH with 99.7% static and exact 0x8D4 size. **Re-mine after any edit that shifts pool numbering** (removing a `.data` vtable via novtable renumbers every `@N` in the TU — the stale map entry then silently un-canonicalizes). `__vt__`-named drift (global symbols, not `unit@` keys) is NOT canonicalized; fix those in source with `__declspec(novtable)` + explicit retail-label assignment (`__vt__6CToken`→`lbl_eu_8056B52C` via `*(void**)this = (void*)lbl_eu_8056B52C;` as the first ctor statement — works byte-identically when the member-init list is empty; with a non-empty init list the manual store is scheduled at the end, see COccCulling above).

When a vtable / data table already references the shortened `…Fv` name (common for help/switch helpers), keep the retail symbol via `extern "C"` and take the extra args on that entry point, e.g. `trySetHelpFlag__Q22cf11CHelpSwitchFv(self, u32 flag)`.

**LOD Fv entry-point verification:** `libs/monolib/src/lod/code_804645CC.cpp` confirms that a high-level `extern "C"` definition with explicit ABI parameters can retain a shortened Fv linker name; `func_80465704__Q23LOD17UnkClass_804645CCFv(s32)` reaches 100% static match (0x14 bytes). Do not use `asm("...")` symbol-label syntax with MWCC Wii/1.1 build 151: it fails at compile time with error 33106 (`<string not found>`), including on free functions. Use the explicit `extern "C"` Fv entry-point form instead.

### Fake vtables -> real classes (subagent flow; CHelp is the worked example)

**Goal of every fake-vtable TU:** delete the pad/If/View and recover the real
class tree with real methods. Do not move pads into a header, do not combine
them into one shared If, do not leave `_v008()` dummies. End state is
`this->method(...)` (or `this->Base::method(...)`) on the type that owns the
slot. CHelp (below) is the proven instance of this flow; replicate it on the
next pad cluster (`CfObjectEne` / `CActorParam` / `PcSub4*` / `CHelp_Talk`
sub-object pads / ...), not only on help.

**Fix all blockers (hard):** do not stop with "still blocked" / "tiny If kept"
/ "TODO owner". Every `*If` / `*Vt` / `_vNNN` / `fn_0xNN` pad in the assigned
files must be deleted (folded onto a real owning-class virtual with retail
arity). Foreign `*(obj+N)` sub-objects: recover the owner (table/RTTI), edit
that hot header if needed, call `obj->method(...)`, delete the iface. If one
angle regresses, revert and try the next — do not park the pad. Non-pad
matching residuals (PS/scheduling) may remain; pad leftovers may not.

**Flow (do in order, hexdiff `--brief` after each step, revert if the mismatch
count rises):**

1. **Dump the retail table.** JP `__vt__Q22cf...` / US `lbl_eu_*` in
   `build/<region>/asm/split1.s`. Words: RTTI at +0x00, 0 at +0x04, then the
   method list. That list **is** the original virtuals. Note size: a 0x1C table
   is five virtuals; 0x24 is seven. Compare sibling tables to see which slots
   a subclass adds.
2. **Name the owning class from the linker symbol**, not from the TU you are
   in. `trySetHelpFlag__Q22cf11CHelpSwitchFv` is `cf::CHelpSwitch`, even if the
   call site is `CHelp_ArtsAttack`. JP `__RTTI__` / `*_typestr` / `*_hierarchy`
   confirm. Hierarchy size: 0xC = one parent, 0x14 = two, 0x1C = three.
   A missing `__vt__` for an intermediate (Switch) is normal if nothing
   constructs it as most-derived; leaf tables still hold its slots.
3. **Put each slot on that class** as a real virtual with the retail signature
   (recover args from `hexdiff --asm` / `symbols xref`; `_Fv` is not evidence).
   `__declspec(novtable)` if the TU has no `.data`. Ctor writes the symbols.txt
   label (`this->vtbl() = &lbl_eu_...`), same as `CToken`. Do not emit a
   compiler `__vt__`.
4. **Inherit the real tree.** Switch-family leaves inherit `CHelpSwitch`;
   direct leaves inherit `CHelp`. A leaf evaluate (`func_802B7D00`) **is** the
   +0x10 override, not a wrapper around a view. A foreign sub-object
   (`*(obj+4)`) stays a tiny named iface on the **owning object** (`PcSub4VtIf`),
   never a pad named after the caller.
5. **Call `this->method(...)`.** Delete `CHelpXxxIf` / `*VtblView` / `_vNNN`
   / `reinterpret_cast<Pad*>(this)`. Then hexdiff the TU, every TU that
   included the header, and one sibling that must **not** have grown a slot
   (CHelp: Target / Sp / ArtsSet).
6. **Rename last.** `UnkVirtualFunc*` is a placeholder, not a slot index
   (`CHelp_UnkVirtualFunc3` is Target's +0x10). `symbols rename-plan` /
   `rename-all` after the tree is right; prefer same-length names.

**Do not:**

- Declare a differently-named virtual hoping it overrides a base slot (it
  **appends**). `func_802B7CB0` stays a non-virtual on Switch; retail leaf
  tables still store it at +0x08, and we do not emit those tables.
- Put a subclass slot on the base (`bool f1C(u32)` on `CHelp` forced +0x1C
  onto Target).
- Inherit a view and redeclare a slot with a new signature (appends +0x20,
  ~97.4%).
- `mVtbl->mSlots[N]` as a function pointer (colors `r4` instead of `r12`).
- Return `bool` from a caller when the virtual returns `u32`: MWCC inserts
  `neg`/`or`/`srwi` and grows the caller ~0xC. ArtsAttack evaluate is `u32`
  `func_802B7D00` calling `u32 func_802B7CBC`.
- Keep a pad "until we know the name". The vtable word **is** the name
  (`trySetHelpFlag__Q22cf11CHelpSwitchFv`).

Worked example follows.

### cf::CHelp layout (real classes, retail __vt__ from symbols.txt)

Retail `__ct__Q22cf5CHelpFv` stores `owner@0`, `param@4`, `lbl_eu_8053B3A0@8`. JP symbols.txt
names that 0x20 object `__vt__Q22cf5CHelp` (US/EU still `lbl_eu_8053B3A0`). It is a C++
vtable (RTTI, per-derived `__vt__` / `__RTTI__`), not a homemade function-pointer table.
The vptr sits at +8 because an 8-byte non-virtual prefix (`CHelpPrefix`) precedes the
virtuals. `CHelp.cpp` has **no .data** -- do not emit `__vt__` from that TU.

`CHelpDispatchIface` / `CHelpTblView` / `CHelpTIPS` / `CHelpXxxIf` / `CHelpF1CView` /
`CHelpTalkVtblView` were the pads; they are gone. Slots live on `CHelp` /
`CHelpSwitch` / the leaf.

`CHelpSwitch` is a real class, not a helper. Linker names are
`func_*__Q22cf11CHelpSwitchFv`. JP still has `cf_CHelpSwitch_typestr`,
`cf_CHelpSwitch_hierarchy`, and `__RTTI__Q22cf11CHelpSwitch`. There is no
`__vt__Q22cf11CHelpSwitch` because nothing constructs a most-derived Switch;
leaf tables reuse its slots. Do not fold `mFlag` / `func_802B7CB0` / `CBC` / `CE4`
into `CHelp` (that re-mangles the symbols to `__Q22cf5CHelp` and grows every
direct `CHelp` child).

Two families, read from the US split1 objects (JP `__vt__Q22cf16CHelp_ArtsAttack`
= `lbl_eu_8053B3C0`, size 0x24):

| Slot | Direct `CHelp` child (Target, Sp, ArtsSet, size 0x1C) | `CHelpSwitch` child (ArtsAttack, Close*, CkKizuna, EndEvent, Talk) |
|---|---|---|
| +0x08 | `CHelp_UnkVirtualFunc1` (or leaf override) | `CHelpSwitch::func_802B7CB0` (Talk overrides with `resetTalkHelp`) |
| +0x0C | `CHelp_UnkVirtualFunc2` | same |
| +0x10 | leaf **evaluate** (`CHelp_Target::CHelp_UnkVirtualFunc3`, `isSpThresholdMet`, ...) | leaf **evaluate** (`func_802B7D00`, `func_802B8534`, `func_802B86F0`, ...) |
| +0x14 / +0x18 | `CHelp_UnkVirtualFunc4` / `5` (`return 0`) | same |
| +0x1C | not present | `CHelpSwitch::func_802B7CBC` |
| +0x20 | not present | `CHelpSwitch::func_802B7CE4` |
| +0x24 | not present | Talk / EndEvent extra (null in those tables) |

Switch-family leaves inherit `CHelpSwitch` and call `this->func_802B7CBC(...)`
(slot +0x1C). Direct-family inherit `CHelp` only. Do not put `f1C` on `CHelp`.

## CAttackParam/CArtsParam record tables -> prefix-vptr real classes (novtable)

- Symptom: TU-local `CAttackParamTailIf` / `*VtblRec` pure-virtual views plus a
  manual `((Slot*)unk84)[2](this)` function-pointer dispatch; ctor stuck at
  81% with a pure `r12 -> r4` reg_swap (virtual calls use `r12`, hand-rolled
  function pointers color `r4`).
- Cause: the record vptr lives at +0x84 (0x84 data bytes first). MWCC puts the
  derived vptr after a non-polymorphic prefix base (same as `CHelpPrefix` ->
  vptr at +8, `_sArtsSet` union -> vptr at +0x34), so the real shape is a
  prefix struct + `__declspec(novtable)` class with four real virtuals.
- Fix: `CAttackParamPrefix` (0x84 bytes of named fields) +
  `class __declspec(novtable) CAttackParam : public CAttackParamPrefix` with
  the four table slots as real virtuals (retail `lbl_eu_8052F610`, RTTI
  `lbl_eu_80662280`); ctor installs the blob label via `vtbl()` and calls
  the base virtual. Differently-named derived functions (`CArtsParam_*`,
  `vtableFunc3`) stay NON-virtual members (a same-signature rename would
  append, and the mangled retail names must be kept); calls go through the
  base name so dispatch stays on slot +0x08. Return-type-only fixes
  (`void` -> `u8`/`float`) do not re-mangle (`Fv` suffix kept).
- Result: `CArtsSet.cpp` 15/15 held while deleting `TailIf`/`*VtblRec`/
  `CArtsRecord`; `CArtsParam.cpp` 13/18 -> 15/18 with both record ctors at
  100% (MWCC inlines the base ctor into the derived one, matching retail's
  store-call-store-call with no `bl` to the base ctor).
- Confidence: repo_proven
- Applies to/a.k.a.: any vptr-at-offset record (`CfEneAtkVtblRec` in
  `CfObjectEne.cpp` is the same slot +0x08 dispatch); the `mVtbl->mSlots[N]`
  ban; `_Fv` is not signature evidence.

## Foreign sub-object dtor slot -> real `delete` (single `if` + delete guard)

- Symptom: `CfTboxSubB0Vt` pad (`u8 pad[0x10]` + `_v008(u32)`) wrapping a
  `lwz r12,0x10(r3)` / `lwz r12,0x8(r12)` / `li r4,1` release call on the
  +0xB0 sub-object.
- Cause: the table at slot +0x08 is the owning class's deleting destructor
  (`__dt__Q22cf13CfResTboxImplFv` in `lbl_eu_80535204`; its body frees on
  flag != 0). The owner (`CfResTboxImpl`, vptr at +0x10 via its own prefix)
  already declares `virtual ~CfResTboxImpl()` at that slot.
- Fix: `delete (CfResTboxImpl*)mSubObjB0;` inside a SINGLE `if (p != 0)`
  (complete type via the hot-header include in the `.cpp`). The explicit
  `if` is retail's first `beq`, `delete`'s own null guard is the second
  `beq` (one `cmpwi`, two `beq`s, then the virtual call with `r4 = 1`). A
  doubled `if` + `delete` regresses (three branches); a doubled `if` +
  manual view call was the old 100% shape this replaces.
- Result: `CfObjectTbox` ctor held at 100% with the pad deleted.
- Confidence: repo_proven
- Applies to/a.k.a.: any `flag = 1` slot +0x08 call on a foreign object
  whose table head is `__dt__`; `delete` vs manual-dispatch branch-count
  rule.

The evaluate function **is** the +0x10 virtual, not a regular method that
casts `this` through a view. `CHelp_ArtsAttack::func_802B7D00` is slot +0x10
on `__vt__Q22cf16CHelp_ArtsAttack`; its body then calls Switch +0x1C. Same
for ClosePartyMenu `func_802B8534`, Talk `func_802B86F0`, Target
`CHelp_UnkVirtualFunc3`. Talk `resetTalkHelp` is the +0x08 override (it
already calls `CHelpSwitch::func_802B7CB0()`). Talk's extra slot is +0x24,
not +0x20 -- +0x20 is already `func_802B7CE4`.

**UnkVirtualFunc\* names are decomp placeholders**, already in symbols.txt.
They are not original identifiers and not slot indices (`UnkVirtualFunc3` is
Target's +0x10 evaluate). Retail has RTTI class strings, not method names.
Recover a better name from the vtable + body, then `symbols rename-plan` /
`rename-all` (prefer same-length). Recipe:

1. Dump the leaf `__vt__` (JP) / `lbl_eu_*` (US) in `build/<region>/asm/split1.s`.
   That list **is** the method list (RTTI at +0x00, 0 at +0x04, virtuals at +0x08).
2. `python3 tools/coop/run.py symbols xref <addr>` and
   `python3 tools/coop/hexdiff.py <unit> --symbol <mangled> --asm` -- recover
   the signature from how r4 / f1 are used (`_Fv` is not evidence).
3. Read overrides: a `blr` base + a leaf that clearly checks X is the evaluate
   slot; Switch `func_802B7CB0` (`mFlag = 1`) is the enable/reset override of
   Unk1; Unk2 calls Unk5 then Unk4, then `CUICfManager_queueTutorialMenu` / `CtrlRemote_SetSharedBit`.
4. Rename only after the class tree is right. Do not invent a pad named after
   the leaf to "hold" an unrecovered slot.

End-state shape (novtable, ctor writes the symbols.txt label, no compiler
`__vt__` in help .cpp TUs):

```cpp
struct CHelpPrefix { void* mOwner; u32 mParam; }; // vptr at +8
class __declspec(novtable) CHelp : public CHelpPrefix {
    virtual void CHelp_UnkVirtualFunc1();           // +0x08, blr
    virtual void CHelp_UnkVirtualFunc2();           // +0x0C, notify
    virtual UNKWORD f10();                          // +0x10, null in base
    virtual UNKWORD CHelp_UnkVirtualFunc4();        // +0x14, return 0
    virtual UNKWORD CHelp_UnkVirtualFunc5();        // +0x18, return 0
};
class __declspec(novtable) CHelpSwitch : public CHelp {
    // func_802B7CB0 stays non-virtual (a new virtual would append, not override Unk1)
    void func_802B7CB0();
    virtual u32 func_802B7CBC(u32 flag);            // +0x1C
    virtual u32 func_802B7CE4(u8 flag);             // +0x20
    u8 mFlag; // +0xC
};
class __declspec(novtable) CHelp_ArtsAttack : public CHelpSwitch {
    virtual UNKWORD f10(); // = func_802B7D00; body calls func_802B7CBC
};
class __declspec(novtable) CHelp_Target : public CHelp {
    virtual UNKWORD f10(); // = CHelp_UnkVirtualFunc3
};
```

Ctor still assigns the retail label (`this->vtbl() = &lbl_eu_8053B3A0` or
`__vt__Q22cf5CHelp` after a three-region rename + re-split), same as `CToken`.
Never construct a view type so MWCC emits a compiler vtable.

Recipe:

1. `__declspec(novtable)` on `CHelp`, `CHelpSwitch`, every `CHelp_*`, and the
   static-table shapes (`CHelpWordC` / `CHelpFloatC` / ...).
2. Virtuals on the class that owns the slot (above). Extra slots on the leaf
   (`CHelp_Talk` +0x24), never a sibling view type.
3. Switch-family leaves inherit `CHelpSwitch`. Direct-family inherit `CHelp`.
   Hierarchy size in JP symbols.txt is the check: 0xC = one parent (`CHelp`),
   0x14 = two (`Switch` then `CHelp`), TIPS 0x1C = three.
4. Call sites: `this->f10()` / `this->func_802B7CBC(flag)`, not
   `reinterpret_cast<CHelpXxxIf*>(this)->_v01C(...)`. Manager lists and
   `lbl_eu_80576D08` entries are `CHelp*`: `item->f10()`,
   `item->CHelp_UnkVirtualFunc1()`, `tbl->mHelp1.CHelp_UnkVirtualFunc2()`.
5. Foreign sub-objects that are not `this` (e.g. `*(obj+4)`) stay a tiny named
   iface on the owning object (`PcSub4VtIf`, `CHelp_EnemyEnableSub` at +0x3E9C),
   never a pad named after the help class.

Ruled out:

- `bool f1C(u32)` on `CHelp` (forces +0x1C onto Target / Sp / ArtsSet).
- Inheriting a view and redeclaring `f1C` with a new signature (appends slot
  0x20, ~97.4%).
- A TU-local `CHelpArtsAttackIf` / `CHelpF1CView` / `CHelpDispatchIface` /
  `CHelpTblView` / `CHelpTIPS` / 150-slot dummy list.
- Making `CHelp_X` inherit a view instead of `CHelp` / `CHelpSwitch`.
- `mVtbl->mSlots[N]` function-pointer calls (colors `r4` instead of `r12`).
- Folding pads into one shared If/View type (that is moving the fake, not
  killing it).
- Treating `func_802B7D00` / `func_802B86F0` as non-virtual wrappers around a
  pad call; they are the +0x10 overrides.

**Sub-object pads:** grep the owning object header before declaring a pad.
`CfObjectPc`+4 slot 0x30 is `PcSub4VtIf` (vptr at +0) for a loaded pointer; do
not use `PcSub4Fake`.

**Gotcha:** ASCII comments only (sjiswrap). Derived help TUs must stay novtable
so they do not grow a `.data` vtable (retail help .cpp splits are .text-only).
`CHelp_UnkVirtualFunc1` is defined in `CHelpManager.cpp`; calls in that TU must
stay indirect (`lwz r12`). If MWCC de-virtualizes to a direct `bl`, do not bring
back a pad If -- move the stub out of the TU or call through a `CHelp*` whose
static type MWCC cannot bind to the defining class. `func_802B7CBC` /
`func_802B7CE4` keep their shortened `Fv` linker names via `extern "C"` if a
header signature would re-mangle them. A `bool` caller of a `u32` virtual
grows ~0xC (`neg`/`or`/`srwi`); match the virtual's return type at the call
site (`u32 func_802B7D00` -> `func_802B7CBC`). Talk's +0x20 call is
`func_802B7CE4`, not a Talk-only `vf20` (that would land at +0x24).

Subagents: copy the **Fake vtables -> real classes** flow above, using this
CHelp tree as the template. Hexdiff `--brief` per step. **NEVER TOUCH GIT** —
no git commands; leave the tree dirty for the human/harness.

**Owning-class / hot headers are in scope.** If retail puts a slot on
`CfObjectEne` / `CActorParam` / `CfObjectPc` / `CfObject` / etc., edit that
header: dump the table, put the real virtual with retail arity, call
`obj->method(...)`, delete the TU-local pad. Hexdiff the TU **and** other
units that include the header; revert if mismatch counts rise. Still forbidden:
growing `_vNNN` dummy lists, no-arg passthrough views of already-widened slots,
and inventing a differently-named virtual that appends instead of overriding.

**Actor/move deep vtable slots (r12):**

**reslist::size() regalloc:** retail often wants `length` in r4 / `cur` in r3. Declaring `length` first in a *local* inlined helper can match without changing shared `reslist::size()` (that reorder regressed `menu-enemy-cbrender`).

**CETrail reslist destructor loop:** The derived `reslist<CETrail::POINT>` deleting destructor is an inline template body, not a call to the base destructor. Directly spelling the sentinel traversal and ownership guard produces the exact `0xC0` function size and 98.85% static match; using a shared `trailClearListDerived` helper outlines the loop and regresses to `0xB8`/~77%. The remaining seven words are pure r4/r5 sentinel-loop coloring. Keep the readable inline loop and accept only after the `__dl__`/`__dla__` callee certificates are available.

**CViewRoot::create reslist reserve loop (~99.41%):** express the 128-node history clear naturally as `for (int i = 0; i < 128; ++i) mList[i].mNext = nullptr`. MWCC emits retail's eight-iteration CTR loop with 16 stores per iteration exactly; the prior hand-unrolled two-half form left 49 mismatches (four structural). Residual 14 words are one constructor-initialization Chaitin cycle: retail `(histVtTemp, capacity, pool1, pool2, historySentinel) = (r6,r9,r8,r7,r5)`, MWCC `(r5,r8,r7,r6,r9)`. Constructor-shaped helpers, placement construction, ABI-ordered inline parameters, declaration/assignment permutations, signedness, and `-O4,s` did not rotate it without structural regressions. Keep the natural loop and do not restore `.text` instruction patches.

**menu-bps-move Chaitin ceiling (~99.76%):** after r12 virtual ifaces + local size helper, residual is (1) party-fill GPR permutation `party/shift/actors` = retail `r21/r22/r23` vs MWCC `r22/r23/r21`, and (2) loop-hoisted FPR homes for `FB8/FA8/FC0/F90/F94` (+1 rotation on `{f27…f31}`). Ruled out: named FPR NVs, bias helpers, actorsBase-before-party, block-scoped party, IPA `fillActors`, fcmpu swap, `fullHpCount` move. Keep high-level C++; **no** `.text` `insn_patches`.

**menu-enemy-cbrender sort Chaitin (~99.172%):** keep exact size `0x274` with draw counter as `u32 i` and `order[static_cast<u8>(i)]` / `while (i < 0x18)` (u8 counter emits terminal `clrlwi`, +4). Sort: pass-before-order + block-scoped `s32 limit`, outer `for (left=0x17; left!=0; left--)` CTR, inner bottom-tested goto, `&indices[j]` rematerialize, depthB before depthA, XOR store-reload with `swapped=1` after the first store. That locks `order=r9`, `j=r12`, `swapped=r11`. Residual: `pass/limit/pair` = MWCC `r8/r10/r30` vs retail `r10/r5/r8` (pair should reuse vacant `r8`). Hoisting `j` before `order` gets `pass=r10` but pins `j` in `r8` and spills pair to `r30`. Dead `gap` before the loop is DCE'd and does not reserve `r8`. Equivalence blocked by unvalidated callees.

**menu-arts-cbrender bitfield-loop Chaitin (~99.3125%):** exact size `0x3C0`; gates + `unk1B8`/`unk80`/`unk98` draws are byte-identical. Residual is NV homes on the 8-slot (`unk318`/`unkA4`) and 9-slot (`unk310`/`unk314`/`unk104`/`unk170`) bitfield passes only: retail `i8=r28` / `i9=r27` with `bitI=r28`/`bit18=r29`, MWCC coalesces both counters into `r29` (`bitI=r27`/`bit18=r28`). `one=r31` and pointer `r30` already match. Ruled out: distinct `Layout**` walks (~94.8%), predeclared `i8`/`i9` (flat), early `i8=i9=0` overlap (~98.5%), `#pragma scheduling off` (~75%). Keep high-level C++; **no** `.text` `insn_patches`. EQUIVALENT blocked by unvalidated callees (`getInstance`, `isFlag01Set`, layout draw helpers, `DrawInfo` ctor/dtor, etc.).

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
  FULL_MATCH pure tails (`b` to `CDeviceFont::func_80452CF8` / `resetGXFull`);
  keep callee undefined or `#pragma dont_inline` so MWCC does not inline empty
  same-TU stubs into `blr`. `getScissorRect1/2` soft-cap (~68%/66%): declare `split` before `offset` so MWCC
  places split@sp+0x10 / offset@sp+0x8 (first local = higher addr). Exact retail
  size; residual is post-`bl` lha/sth interleave + overlap/clip Chaitin. Dead-result
  `getFrame2ViewOffset` required.

**`hasSplitViews` / `func_8043FD10` / `func_8043E58C`:** CAFC is the dual-view
  probe shared with `getSplitLine`/`setSplitLine` (return hasView2); high-level C
  soft-caps on the same prologue interleave. The former `getSplitLine`
  whole-function asm was removed 2026-09-17 (not sanctioned by PLAN.md §17.6);
  keep high-level C++ and record the near-miss. FD10 is the render() client-rect + border expand helper —
  peak ~99.5% + SMT `EQUIVALENT` (plain field stores; `volatile` lha regresses).
  Cycle may label that `CODE_MATCH` even when `equivalence=equivalent`; promote
  to `EQUIVALENT_MATCH` when the cert is issued. E58C walks `mParent` via
  `convertToView` / `getInstance` accumulating `getFrame2ViewOffset`, then FD10
  for size (~87% schedule); avoid long `cycle` SMT on it.

**`getSplitFrameFlag` / frame draw / GX scissor ring:** CE90 is `lbz` of
  `unk45C[8]` (FULL via high-level `extern "C"` C++). `drawFrameBorder` draws border
  quads (`begin(9,1)`/`add`/`end`) with `mFrameColor`×`lbl_eu_8066A318` then
  ×`lbl_eu_8066A2F4` (~54% first pass). `cacheScissor` copies optional
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
  `mUsed`, `mCapacity`) then `CGXCache::cacheScissor`. Peak ~68%/81%/93% before
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

**CUIBattleManager::Move soft-cap (~95.57%):** unit size PASS; function `0xB2C` vs retail `0xB00`. Peak keeps indirect `UIBattleCreatePlayerState` call (same-TU empty stub is IPA'd away on direct `bl`), pad0C chain, capacity-first slot loads, `framePad[8]`. Residual: decomp `-0x210`/`stmw r24` vs retail `-0x220`/`stmw r25` because `battleWorkEvent` `this+0x54` is CSE'd into an NV across the asset `readFile` loop (4th NV). Volatile `this` reload restores `stmw r25`/`-0x220` but fuzzy drops to ~94.4%. Slot `r8`/`r7` order and pathBuf homes (`0x178`/`0xF8` vs `0x158`/`0xD8`) ride the frame skew. `EQUIVALENT` blocked by unresolved indirect + unvalidated callees. Keep high-level C++; no `asm void`.

`CHelp_Pg::isThresholdMet` int→float uses retail `lbl_eu_80669000@sda21`; MWCC pools `@N` with the signed magic double — rename via `CHelp_Pg.o` `pool_patterns` `(MAGIC_HI, MAGIC_LO) → lbl_eu_80669000` once `.text` already matches.



---

### Core patterns — the 5 things that fix 90% of gaps

### 1. Relocation name drift (99.3–99.9%, instructions identical)

**SUPERSEDED (2026-10-10):** PLAN.md §17.6 no longer approves relocation-name spellings. Do not add `extern "C" lbl_eu_*` (or pool renames) to line up reloc names; the comparator ignores names where it can (`functionRelocDiffs=data_value`, `tools/coop/lib/reloc_canon.py`). Read this section for diagnosis only; existing spellings are cleanup debt. Object post-processing to rename symbols remains forbidden.

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

**Billboard layered-renderer application:** `MPFDrawBillLayTex`'s billboard quad paths use the NW4R `VEC3Add` high-level helper to reproduce retail paired-single `psq_l`/`ps_add`/`psq_st` sequences. Scalar `Vec.x/y/z` additions compiled to separate `lfs`/`fadds` instructions and reduced `drawBillIndexed` to 22.9% fuzzy; replacing only the four corner sums with `nw4r::math::VEC3Add` raised it to 59.3% and kept the split within budget. Use the SDK/NW4R vector helper for PS-heavy geometry rather than scalar C++ arithmetic.

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
- The mined map keys named symbols globally (`spInstance__9CDeviceGX` → `lbl_eu_806656A0`, 29×) and TU-local labels (`@N` pools, `...bss.0` section relocs) per-unit (`{objdiff_unit}@{symbol}`, e.g. `main/kyoshin/plugin/ocUnit@12270`). `_canonical_symbols_for_unit` accepts both that objdiff name and the registry unit (`kyoshin/plugin/ocUnit`); a registry-only prefix used to miss every `main/…` key and over-reject pool/jumptable sites as reloc-dest mismatches. It also loads those **global** named entries (vtables `__vt__…` → `lbl_eu_*`, SDA/`spInstance` globals). `_byte_identical_with_relocs` treats a MWCC linkage tail as the same dest (`foo` vs `foo__Fv`, `lbl_eu_X` vs `lbl_eu_X__2cf`). Call-reloc leftovers (`func_*` vs a recovered name whose hex is not that function's address) are **not** in the data-only map — token-replace the leftover identifier in `src/`/`include/`/`libs/` (not generated `.ctx.c`) and drop leftover decls that would duplicate the recovered method. `symbols rename-all` is type-oriented and is the wrong tool here. Do not remap a leftover C++ `bool func_X(int)` onto an existing `extern "C" Recovered(...)` with a different signature: C has no overloads (MWCC 10197/10505). Remine `--all-kinds` is also wrong (`ensure_fresh` is data-only and would wipe call entries). Large addend deltas (> `MAX_MAP_ADDEND_DELTA`) are dropped as misalignment noise. Auto-mined entries reproduce the *legacy* hand-written `exact_renames` rules (e.g. `s_nandUserAreaCallbackFmt` → `lbl_80551294`, `s_nanderrPath` → `lbl_805512B8`), confirming the map is correct — but **do not add new postprocess rules**: object/reloc postprocessing is deprecated (PLAN.md §17.6), so the tool only points at the approved source-level `extern "C"` fix.
- `hexdiff` embeds the same analysis: the terminal output ends with a **Reloc drift** section listing each diverging reloc site with its source fix (the `extern "C"` declaration for name drift; inline/emit-the-symbol guidance for presence drift; builtin/expression guidance for type drift — and an EQUIVALENT_MATCH fallback note when the symbol is an implicit pool that can't be named in source); `--json` adds `reloc_drift` + `reloc_suggestions` keys.

#### 1j. Section-symbol anchors (`...bss.0` / `...rodata.0` / `...data.0`) — layout-driven, not source-renamable

MWCC emits a **section-symbol reloc** (e.g. `...bss.0`) for the base computation of a folded address group instead of the named symbol. Observed triggers:

- **Multiple same-section symbols fold into one base** (WUD `__wudSyncStoredDevInfoToNand`/`__wudSyncTryConnect`/`__wudDeleteHandler`/`__wudInitDevInfo`/…: `p = &_wcb` + `&_scArray.regist` both anchor off one `lis/addi`, retail names `__rvl_wudcb`, decomp emits `...bss.0`). A function that references exactly ONE bss symbol of the section gets the named symbol (`__wudSyncFlushCallback`).
- **The anchored symbol is NOT the first in the section** (SIBios `SITransfer`/`SIInterruptHandler`/`GetTypeCallback`: decomp `.bss` order is XferTime(0), TypeTime(0x20), **Packet(0x40)** — retail has **Packet(0x0)** first — so `&Packet[chan]` becomes `...bss.0 + 0x40` with the offset **baked into the instruction** (`addi r0, r31, 64` vs retail `addi r0, r31, 0`). The witness then fails at gate 3 `fields` (non-register bits), not gate 2.
- **`.bss`/`.rodata` emission order is NOT declaration order** — verified on SIBios: source declares `Packet` before `XferTime` yet the object emits `XferTime` first; `Alarm` is declared last but sits 4th. The order follows first-reference/use order across the TU's functions, so reordering declarations is a no-op; only reordering the function bodies (matching retail `.text` order) would flip it, which risks every sibling FULL_MATCH function in the unit (their named-symbol relocs are order-independent — only the section-anchored functions change). **Do not chase**; the mined map canonicalizes `...bss.0`/`...rodata.0` → retail name for gate 2, and the static match is unaffected (values equal). Same cap for rodata anchors (`HBMAxSound SetVolumeAllSeq`: retail anchors the float pool to `WT_FILENAME__24@unnamed@HBMAxSound_cpp@` at `.rodata` offset 0, decomp emits `...rodata.0`).

#### 1k. Float-pool ordering is TU-wide — per-function literals are not fixable

`HBMAxSound SetVolumeAllSeq` (us-803252a0): the three float constants pool at the SAME offsets (40/44/48) on both sides but the **values are permuted** (retail `[0.0f,10.0f,20.0f]`, decomp `[10.0f,20.0f,0.0f]`), so every `lfs fX, imm(r31)` at the same slot loads a different constant → witness gate 3 `fields`. The pool is TU-wide (other functions' constants interleave), so §1b's `extern "C" const float` route is wrong here (it switches to SDA21 `lfs @sda21`, changing the instruction shape). Record as blocked.

#### 1k2. `.sdata2` leading-pair order swap is source-unfixable when the reorder would break code or split-size (AXFXReverbHiExp, Wii/1.1 `-O4,p`)

`AXFXReverbHiExp` (us-802d8c10 `__InitParams` + 6 siblings): all 7/7 functions FULL_MATCH (hexdiff `--all` 100%, 0 structural, 0 reg_swap, split PASS with 0xC spare) but `data diff` FAILs on 4 bytes — decomp pool `[0.0, 32000.0, 1.0, 0.6, …]` vs retail `[32000.0, 0.0, 1.0, 0.6, …]` (leading f32 pair swapped; code still matches because SDA21 relocs are compared by resolved value).
- Symptom:   unit-level `.sdata2` order mismatch with per-function code at 100%.
- Cause:     MWCC pools float constants in TU source order; the decomp's `Init` hits the `< 0.0f` guard textually before the `* (f32)32000` multiply, so 0.0 takes slot 0. Retail pooled 32000.0 first. Contrast with sibling `AXFXReverbStdExp`, whose leading `GetMemSize` references `32000.0f` before any `0.0f` and data-matches exactly.
- Fix:       none source-side — (a) `(f32)32000` vs `32000.0f` spelling cannot reorder (same TU source position); (b) moving the multiply above the guard emits store-before-branch and breaks the 100% code match; (c) rate-local hoist / FORCELITERAL+FORCEACTIVE blow the 0xC split spare; (d) a `swap_sdata2_leading_f32_pair` postprocess rule is barred by the frozen UNIT_RULES (shrink-only). Record as blocked data near-miss.
- Result:    functions accepted (FULL_MATCH via `cycle`); unit stays code-100% + data-86.7%.
- Confidence: negative_result
- Applies to/a.k.a.: §1k (TU-wide float-pool permutation); any TU where the only pool-order lever sits on the wrong side of a guard branch or the size budget.

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

**Member/offset variant (CItemBoxGrid HandleSysWinOK, FULL_MATCH):** for a byte field at a raw offset, the *value* cast `!(s8)p[0x540]` still emits `lbz` + `cmpi` — the cast must sit on the **lvalue read**, not the result: `if (!((s8*)p)[0x540])` emits the retail `lbz; extsb.; bne`. Same rule as the global guard above: MWCC only takes the record-sign-extend path when the loaded operand is typed `s8` before the test.

**Truncation placement (CItemBoxGrid FindKindPoolIdx, FULL_MATCH):** if retail copies a helper's result to a saved reg **untruncated** (`or r30,r3,r3`) and masks at the *use* site (`rlwinm r5,r30,0,16,31` as a call arg), the helper must be declared with a **full-width return type** (`u32`, not `u16`) — a `u16` decl forces the mask onto the def site. But an `& 0xFFFF` mask at the single use gets copy-propagated back to the def anyway; write the use as an explicit **`(u16)` cast** so the conversion node stays at the arg. Same family: testing a `u8`-returning helper through a wrong `u32` decl needs `(u8)call(...)` at the test to reproduce `rlwinm r0,r3,0,24,31; beq` instead of `cmpwi`.

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
r6 — same ABI-boundary class as reslistCount/801CB9D8). Rule: when a
CriWare/retail pool double is accessed, prefer the named extern over a raw
literal address — raw addresses inline and drop the reloc.

#### 7j. Duplicate loads retail keeps un-CSE'd — force with a `volatile` lvalue read (repo_proven)

Retail sometimes loads the same field twice back-to-back (e.g. `cur` and `orig`
both read from the counter before modification; `lha r5` / `lha r31` adjacent).
MWCC value-numbers by ADDRESS, not TBAA: reading through a second view struct
type or a raw `*(s16*)((u8*)self + off)` cast does NOT block the CSE. A
`volatile`-qualified lvalue read does:

```c
s16 cur = v->field_0x335A;
s16 orig = *(volatile s16*)((u8*)self + 0x335A);  // forces the second lha
```

Verified: `CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv` (us-8017f858,
Wii/1.1 -O4,p) went 17.1% -> 100% static -> FULL_MATCH with no other change.
Related: volatile reads also act as scheduling pins for constant-pool load
order (see attempts.jsonl us-8017f0dc round 3).

Negative result (probed, `.scratch/cse_probe.cpp`): when retail instead RECOMPUTES a base-pointer adjustment per call (`addi r3,r30,8` before each helper/virtual call) while our draft caches it in a callee-saved reg, NO expression form reproduces the rematerialization - same-expression caching is value-numbering-driven, and a `volatile` stack local yields sp-relative reloads (`lwz r3,off(sp)`) instead of `addi`. Retail's per-call form implies its allocator rematerialized under different pressure, i.e., the original expression graph kept different values live. Treat affected functions (CActorParam vf10/vf12/func_801765A4 family) as allocation-grind redrafts, not expression-shape fixes.

#### 7j-2. Dead constant-compare branches retail keeps — preserve with a `static` local (repo_proven)

When a draft stores a literal into a field and then range-checks the literal
(`s16 sv = 1; field = sv; if (sv < 0){...} else if (sv > 4){...}`), MWCC folds
the compares and drops the branches — but retail keeps them (`cmpi r0,0` /
`cmpi r0,4` against the store register, dead at runtime). Declaring the
variable `static` defeats constant propagation into the compares while
generating identical store code:

```c
static s16 sv;
sv = 1;
field = (u16)sv;
if (sv < 0) { field = 0; } else if (sv > 4) { field = 4; }   // branches survive
```

Verified: CActorParam_UnkVirtualFunc153 (us-8017f410, Wii/1.1 -O4,p) went
59.2% -> 100% static -> FULL_MATCH solely from this change; the same block in
vf159's arts-hit path picked up its missing clamp sequence identically. Pairs
with §7j (volatile for duplicate loads): both abuse storage duration/qualification
to block front-end simplification, not scheduling.

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

For `monolib/src/core/code_804E36DC.cpp`, `ml::CVec3` subtraction through its high-level `operator-`/`CVec3::sub` path reproduces retail's paired-single `psq_l`/`ps_sub` sequence and the temporary-to-result copy before `PSVECMag`. Scalar component arithmetic does not. The retail `schedFeedTargetDist` distance helper also uses a same-TU `schedFeedDistance` call; marking that helper `DECOMP_DONT_INLINE` keeps the TU within its exact `0xC58` split budget. The residual distance-function mismatch is external virtual/callee register scheduling, not a semantic difference.

`schedClearFlag15Update`/`schedStepEntries` clear a `u16` flag with the retail wrap mask `rlwinm ...,17,15`; ordinary `u16 &= ~0x8000` emits the semantically equivalent `rlwinm ...,17,31`. Use the approved `DECOMP_PPC_RLWINM` intrinsic only when exact opcode selection is required, and log the policy exception.

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
- `setFontPath__17CDeviceFontLoaderFv` (takes `void* arg1, const char* path`)
- `getFloatParam__7CEvent1Fv` (takes `int index` as r4)
- `getPtrParam__7CEvent1Fv` (takes `int index` as r4)

**Fix:** define the function as `extern "C"` with an explicit `self`
pointer as first parameter. This produces the exact `Fv` symbol name
(`extern "C"` strips C++ mangling), and the parameters are passed by C
calling convention (r3=self, r4=arg1, r5=arg2):

```cpp
extern "C" {
void setFontPath__17CDeviceFontLoaderFv(
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

`CSchedule::Sched_RebuildHandles` (0xC8): retail recomputes `&mHandles[count]` per
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

- **Mangled retail symbol** (`getInstance__Fv`, `func_80133324__12CUICfManagerFiii`) → the source must be a **C++ function** (member or free, MWCC emits the mangling). `extern "C"` emits the bare name → 0-candidate probe failure even at 100% bytes.
- **Bare retail symbol** (`CTaskGame_setFlag800000`, `clearReslistLinks`, `lbl_eu_80663D18`) → `extern "C"` free function (explicit `self` param for member-like bodies).
- Verify the retail symbol's mangled signature against retail asm (arg registers r4-r10/f1-f8) before trusting it — `func_80133324__12CUICfManagerFv` was a wrong `Fv`; retail used r4/r5/r6 → corrected to `Fiii` in symbols.txt + target record.

---

### MWCC `-inline auto` inlines same-TU helpers — use `__declspec(noinline)`

`-inline auto` (kyoshin TUs) inlines any small function defined in the same TU, even when retail made a real `bl` (e.g. a 0x64 sinit inlined into a caller, or a 0x30 list-clear inlined at 10 call sites — body ballooned 0x108 → 0x278). Established fix: `extern "C" __declspec(noinline) void f(...)`. Also apply to the forward decl so callers before the definition see it.

---

### Struct layout verification via retail field stores

A named member's offset can be wrong even when its comment is right. `FixStr<N>` in code_800B06A4.hpp had `u8 _pad[8]` (size N+12) shifting `field_0xCF4`/`field_0xCF8` to 0xCFC/0xD00 — retail stored u32/f32 at 0xCF4/0xCF8 (inside the pad region; writing real string data there would be nonsensical). Check the retail store offsets against the member comment before trusting a struct. Fixing the struct lifted setTrackedTarget 34.9% → 46.5% with zero regressions to FULL_MATCH neighbors.

---

### Mask↔rlwinm encoding (empirical, -O4,p)

MWCC's `rlwinm` encoding for AND-masks is inverted vs naive bit math:
- `x & ~0x00000100` (clear bit 8) → `rlwinm rX,rX,0,24,22`
- `x & ~0x00800000` (clear bit 23) → `rlwinm rX,rX,0,9,7`
Verify with `.scratch/` probes (compile a tiny TU with the unit's exact flags) before hand-decoding masks.

---

### Byte-identical rename path to FULL_MATCH

For a target whose decomp body exists under another name: find the byte-identical counterpart (compare `FunctionBytes.code` across the decomp unit), rename source to the retail symbol (linkage per the rule above), rebuild, hexdiff 100%, cycle → FULL_MATCH. Worked on: `isUnk68Bit13Set`→`isFlag2000Set`, `actCallVt90/94/30`→`actAnimVt90/F4/118`, `callStubReturnZero_800436A8`→`func_8004368C`, plus symbols.txt mangling fixes (`UnkVirtualFunc29 Fv→Ff`, `getRsrc Fv→CFv`).

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
   CfObjectEff's `setEffRotVec___Q22cf11CfObjectEffFv` and CfObjectModel's
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
- **`CopyTabSlotVec` (us-801cd42c, CItemBoxGrid) — SOLVED FULL_MATCH:**
  12-byte-stride entry copy; retail base in r5 (reused idx reg), MWCC always
  bases in r4 (reused src reg). Fix: declare the **value locals before the
  base pointer** — `u32 v24, v28; u8* e = (u8*)src + idx * 12;` then load into
  v24/v28. MWCC colors in source order, so the value locals claim r4 (freed
  src arg) first and the base lands in r5 (freed idx arg): retail's exact
  `add r5,r4,r0; lwz r4,24(r5)`. Old shape (base assigned first, values
  inline) invariantly bases in r4 across decl-order, word-indexed,
  inline-offset, struct-typed, 5 MWCC versions. 100% byte-identical,
  semantic-certified, no witness needed.
- `MenuShopSellCopyData` (us-8018c6e4, CMenuShopSell): 0x800-byte struct-copy loop;
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

`CMainMenu_IsInputBlocked` (us-80102570, CMainMenu) read `player->field_0x3F60` at
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

`SuddenCommuClearCommuVoices` (us-801bde88, CSuddenCommu.cpp) read the enum list's
`mPtrCount` at compiled offset 0x61C vs retail 0x620. The `cf::CfObjEnumList`
class declares its methods as plain (non-virtual) members — retail calls
`getObjectAt` DIRECTLY (reloc confirmed) — yet the retail layout has
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
QstLogList_CopyEntry (CQstLogList): a 0x22-byte entry copy compiled by retail as 2
byte copies + a 4x8-byte `lwzu/stwu` update-form counted loop (`mtctr`/`bdnz`).
The hand-rolled word loop emits plain `lwz/stw + addi`; the natural source
`*pDst = *pSrc; return pDst;` (full struct assignment) reproduces the retail
exactly. The `return pDst` is load-bearing: it keeps r3 (= dst) live through
the loop, forcing the loop word into r4 (retail's allocation). Needs
`optimize_for_size` to suppress the -O4,p unroll.

### `#pragma scheduling off` reproduces -O3 interleaved copy shapes
QstCnt_CopyInfoA (CMenuQstCnt): retail emits the -O3 "load r0; store r0"
interleaved per-field copy under an -O4,s unit. Per-function
`optimization_level` cannot downgrade codegen (MWCC_CASES 1302), but
`#pragma scheduling off` around the function reproduces the interleaved
single-register copy byte-for-byte under -O4,s (verified Wii/1.1).

### 2026-08 highmatch sweep — reusable fixes (agent-highmatch session)

- **Mask-path volatile re-read (CfObjectMove CfObjectMove_setMoveSpeedGated, 0x28, FULL_MATCH):** retail
  reloads `mFlags68` after an early-return branch; MWCC CSEs a plain member re-read into
  the check register. `(*(volatile u32*)&self->mFlags68 & ~0x800u) | 0x100400u` forces the
  fresh `lwz` byte-identically. Also: retail `rlwinm 21,19` clears **0x800** (not 0x100000),
  and `oris 0x10` sets **0x100000** (the 16-bit immediate is shifted left 16).

- **Goto-gate for the branch-over-branch layout (CTitle CTitleLogo_draw / CTitleMenu_draw,
  0x30, FULL_MATCH):** retail's `bc 4,2` over a `blr` (if-body placed AFTER the return)
  requires `if (A && B) goto body; return; body: ...;` — plain nested ifs emit `bclr 12,2`
  instead. Same family as the documented sjrbf_PutChunk gate.

- **`void*` intermediate flips callee-saved allocation (nw4r snd_RemoteSpeaker
  IntervalAlarmHandler, 0x70, FULL_MATCH):** `RemoteSpeaker* p = (RemoteSpeaker*)
  OSGetAlarmUserData(pAlarm);` allocates pAlarm→r31/old→r30; the two-statement
  `void* ud = OSGetAlarmUserData(pAlarm); p = (RemoteSpeaker*)ud;` flips it to the retail's
  r30/r31 and byte-identity. (Contrast: the same trick does NOT move getTelopString's
  base/s roles.)

- **`__declspec(noinline)` on a trivial ctor (CMenuVision createVisionMenu, 0x7C,
  FULL_MATCH):** an empty `__ct__CMenuVision { return self; }` gets INLINED by -inline auto,
  dropping the retail's ctor call and its null-check (and mangling the args — the size
  leaks into r3). `extern "C" __declspec(noinline)` restores the call + `cmpi/bc` pair.

- **Ternary vs if/else for a f32 selection (CTaskGameEff setEffectEnabled, 0x7C,
  FULL_MATCH):** `f32 time; if (enable) time = const; else time = g->field;` lets MWCC
  hoist the following call's `lis/addi` base ABOVE the if; the single-expression
  `f32 time = (enable) ? const : g->field;` pins the base after the selection — byte-
  identical.

- **`(s8)(u8)` double cast reproduces the dead `rlwinm`+`extsb` pair (CSortMenu
  sortMenuPageDownStep, 0x78, 86.7%):** `s8 sp5s = (s8)(u8)sp5;` emits retail's dead
  `rlwinm rX,r0,0,24,31; extsb rX,r0` (the rlwinm result is overwritten); a single `(s8)`
  cast emits only the extsb and the function is 1 instruction short.

- **Per-use inline casts instead of a base local (CScnItemModel simRefreshActDist, 0x7C,
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
  code_80135FDC LayoutSetTextBoxInt, 0x54, FULL_MATCH):** the retail format arg is
  `lbl_eu_80500664 + 7` — a NAMED .rodata string-table object (0x27C bytes),
  not the TU-local `%d` pool literal. Writing `"%d"` emits `addi r4,r6,@l`
  with an `@stringBase0` TU-local reloc; writing `&lbl_eu_80500664[7]` emits
  the retail `lis @ha; addi r6,@l; addi r4,r6,7` (named `lbl_eu_80500664`
  relocs, correct +7 slot, call-site alignment) — 0 structural, 100%.
  Reusable: a sprintf/strcmp/strcpy argument that lands at a nonzero offset
  of a large pooled string is usually a NAMED extern slice (`&lbl_eu_XXXX[7]`)
  — check sibling functions in the TU for existing `lbl_eu_80500664[...]`
  uses before assuming a standalone literal.

- **Duplicated-guard dead branch (CKizunagram releaseRadarChild / releaseCurChild,
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
  that Wii/1.1 merges (monolib code_804DEDA8 SchedItem_ReleaseHandle, 0x6c, FULL_MATCH
  + SchedRes_ReinitTables 19.2%→100%):** retail `cmpwi r3,0; blt end; blt end; lwz
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
  code_80135FDC LayoutSetTextBoxFmtValue, 0x68, FULL_MATCH):** retail `sprintf(buf,
  lbl_eu_80500664, fmt)` — the r5 PARAM is the sprintf VARARG while the
  FORMAT is the constant extern (fmt is never used as a format); the 4th
  LayoutSetTextBoxText arg is the value param, not 0. After fixing both, the only
  residual was a 16-byte frame overrun from `char buf[528]` — the retail
  frame (0x220, saved regs starting at 0x214) implies buf[520] (0x208):
  `char buf[520]` gives byte-identity (100%). Reusable: when the decomp
  frame is 16 bytes larger with all instructions identical, trim the
  buffer/array size until the saved-reg offsets line up — MWCC pads the
  frame to 16 but the retail layout has no pad.

- **`#pragma optimization_level 1` for base-add-before-store pool allocators
  (CScnItemModelNw4r scnImN4PoolAlloc, 0x30, FULL_MATCH):** retail
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
  (also see sibling scnImN4AllocFromBuf, same pattern, same fix applies).

---

### u8-return vs full-word compare + cast-only virtual interface for r12 dispatch (FULL_MATCH ×2 fixes)

`ShowExchangeCur` (us-801d03c8, CItemBoxGrid.cpp) had two residuals, both fixed
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
(bit-31 form SH=31-k vs in-place SH=0,k,k); COption COptionTickState's switch
case set was 1,2,4,5,6,7,8,10 (0/3/9 empty) — Wii/1.1 truncates trailing empty
cases so the full 0..10 table needs the real case set; ocBdat func_8003B748
guard was `(elemType-6) <= 1 → shared *4 tail` (bc 4,1 = branch if NOT GT),
expressed as `if ((u32)(elemType-6) > 1) {switch} else {scale4: *4}` with
`goto` from case 3 (a flat `if (…) goto scale4; switch…; scale4:` lets switch
`default` fall through into ×4 — wrong).

---

### Tail-call thunk into an adjacent getter: split the merged retail symbol (CScnVirtualLight scnVlGetOff10C/func_80493C10, FULL_MATCH ×2)

Symptom: retail `scnVlGetOff10C` was annotated 0x10 bytes as
`addi r3,r3,0x54; b .+4; addi r3,r3,0xB8; blr` — the `b .+4` looks like a
"scheduler barrier" but is actually a **tail call into an adjacent second
function** (`func_80493C10 = addi r3,r3,0xB8; blr`). The annotation merged
two 0x8-byte functions into one 0x10 symbol. ~80 single-function probe shapes
all fold to `addi r3,r3,0x10C; blr` (documented plateau) because the two adds
are in ONE function.

Fix (monolib, Wii/1.1, `-ipa file -inline auto`):
1. Split the symbol: `scnVlGetOff10C = .text:0x80497C7C; size:0x8` +
   `func_80493C10 = .text:0x80497C84; size:0x8` in config/us/symbols.txt
   (retail .o regenerates via the `split` ninja rule; depfile tracks symbols.txt).
2. Reconstruct as two `extern "C" __declspec(noinline)` functions in source
   order (getter first or thunk first both work — MWCC emits the thunk's
   `b` as a REL24 reloc resolved to +4 when the getter lands right after):
   ```cpp
   extern "C" __declspec(noinline) void* func_80493C10(void* self) {
       return (void*)((u8*)self + 0xB8);
   }
   extern "C" __declspec(noinline) void* scnVlGetOff10C(void* self) {
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
Key: the sinit must be **void** (a trailing `return` kills the tail call and forces a frame), the helper is `__declspec(noinline)` (keeps the `b` instead of inlining/folding), and the vtable source is `char[]` (address constant via lis/addi — a pointer-typed extern emits an SDA21 `lwz` load instead). Both functions reached FULL_MATCH. The other merged-symbol sinits (`sinit_804DB0D8` etc., `sinit_eu_804F9FA4`) should re-split the same way — check for a `b .+4` tail-call boundary at +0x8 before treating them as ceilings. See also CScnVirtualLight scnVlGetOff10C/C10 (same merged-thunk resolution).

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

**3. u8 truncation via memory round-trip = deref-of-storage.** `CtrlObjectParam_GetRowColumnByte` (us-800a3b8c): retail `bl getBdatStringColumnValue; stw r3,8(sp); lbz r3,8(sp)` (0x40). The `(u8)` cast folds to `rlwinm r3,r3,0,24,31` (0x3c). Write `u32 v = call(...); return *(const u8*)&v;` — the stw/lbz round-trip (CfObjectNpc MWCC_CASES rule; a bare `&call()` is "not an lvalue", so use the local).

**4. `#pragma optimize_for_size on` merges r30/r31 saves into stmw/lmw + fixes the copy order** — confirmed again on the CTagProcessor tag-writer family (TagWriterCode7 us-80129480, TagWriterCode5 us-8012953c, TagWriterCode3 us-8012967c, all 0x48 → 100% FULL_MATCH). Retail prologue `stwu; mflr; stw r0; stmw r30; or r30,r4,r4; or r31,r6,r6`; plain `-O4,p -use_lmw_stmw on` emits `or r31,r6,r6` first + reversed `stw r31,28(sp); stw r30,24(sp)` pairs. The pragma (scoped on/off around the function) makes MWCC copy r3→r30 first and merge the saves (CPartyState drawPartyState pattern).

**5. MWCC rejects `extern const float` vs `extern float` redeclarations of the same symbol** — a new non-const `extern float lbl_eu_8066A20C` in CActParamAnim.hpp (added 2026-08-14) silently broke the whole CfObjectModel TU build (stale .o since Aug 13, "identifier redeclared as 'float'" at the include site). Align the redeclaration qualifier with the first declaration (`extern const float`).

---

### Deleting a polymorphic layout: `delete` through the TYPED pointer reproduces the virtual dispatch + dead second beq (Wii/1.1 `-O4,p`)

`resetEIBCur` (us-80287f40, 0x68) and `CArrow3D::Term` (us-80122980, 0x70) both release an `nw4r::lyt::Layout*`. Retail: `lwz r3, 0x70(r31); cmpi r3,0; beq skip; beq zero; lwz r12,0(r3); lwz r12,8(r12); li r4,1; bcctrl; zero: li r0,0; stw r0,0x70(r31); skip:` — virtual slot-2 dtor with deleting flag 1, TWO beq's on the same test. The matching source is a plain `if (layout) { delete layout; mLayout = 0; }`:
- `delete` on a `Layout*` (polymorphic) dispatches the vtable dtor with flag 1 AND carries its own null-check — that null-check IS the retail's dead second beq (targeting the `mLayout = 0` block).
- `layout->~Layout()` emits the flag-0 call; `delete` on a `u8*` emits a DIRECT `bl __dl__FPv` (no virtual dispatch); a manual `((void(*)(void*,int))vt[2])` cast emits scratch `lwz r5` loads instead of the clean `lwz r12,0(r3); lwz r12,8(r12)`.
- The zero constant stays in r0 (re-`li`'d after the call) with a single r31 save when the source doesn't hold it in a local across the call.

---

### Leading-constant expression shape flips scratch regalloc (CPassiveSkill CPassiveSkill_linearizeGridSlot, FULL_MATCH 100%)

`(u8)(slot + col*5 + (row-1)*25 + 1)` tail-calling a void setter left a fixed 5-instruction scratch rotation (37.5%: retail colors the row-1 chain r0, col*5 reuses r5, accumulator starts r4; MWCC colored row-1→r4 and col*5→r0 regardless of parenthesization/statement order). Preceding the whole sum with the constant term — `(u8)(1 + slot + col*5 + (row-1)*25)` — reorders MWCC's virtual-register birth so the coloring matches retail byte-for-byte: `addi r0,r4,-1; mulli r5,r5,5; mulli r0,r0,25; add r4,r6,r5; add r4,r4,r0; addi r0,r4,1; clrlwi r4,r0,24; b`. Same opcode graph, different birth order → 100% FULL_MATCH, no pragmas/macros. Lesson: when a pure scratch `reg_swap` rotation resists parenthesization and statement reordering, try moving a literal constant to the head of the left-associative sum.

---

### Chained 3-copy swap temporaries: declare tC,tB,tA (reverse use order) for retail stack slots (CEquipItemBox sortEIBByFlags/sortEIBByCount, FULL_MATCH 100%)
- Symptom:   bubble-sort swap block (tA=copy(a); copy(a,tB=b); copy(b,tC=tA)) at ~95%: only the addi rX,sp,N slot pair for two of the three 8-byte temps swapped (retail sp+8/sp+24 vs decomp sp+24/sp+8), 0 structural.
- Cause:     MWCC assigns address-taken locals' stack slots by virtual-register birth order; declaring the temps in use order (tA,tB,tC) births them in an order that reverses one adjacent slot pair vs retail.
- Fix:       declare the three temps in reverse use order - CEquipItemData tC; CEquipItemData tB; CEquipItemData tA; - matching the already-matched sibling sort sortEIBByVf08. Both affected sorts went to 100% with no other change.
- Result:    FULL_MATCH x2
- Confidence: repo_proven
- Applies to/a.k.a.: any chained-copy swap/rotate block; pairs with register_mapping.md Rule A (declaration order steers both saved regs and stack slots).

### The `(v | ~C) - ((v - C) >> 1)` idiom IS MWCC's unsigned `v >= C` compare (CFloorMap/CChain/CMainMenu, Wii/1.1 `-O4,p`)
- Symptom:   retail shows `li rB,C; subi rX,v,C; orc rY,v,rB; rlwinm rZ,rX,31,1,31; subf` feeding a bit test - looks like obfuscated bit-twiddling; resist decompiling it literally.
- Cause:     it is the canonical MWCC expansion of the unsigned comparison `v >= C` (C a constant). Confirmed by CChain.cpp comments and by scanning the whole retail corpus: every `li+orc` site sits at an unsigned `>=`/`<` against a constant (CSysWinScenarioLog thresholds, CTaskREvtSequence, CFloorMap rand gates).
- Fix:       write the plain comparison (`pane->SetVisible(CtrlRemote_TouchBitByArg(0x20) >= 0x171)`). Do NOT write the expanded form - the frontend folds `~C` into `li -(C+1); or` giving the wrong opcode (or vs orc). Do NOT use a named local for C (folds too), volatile (adds stack traffic), or mw_version GC/3.0a5.2 (all Wii/GC versions fold it - probed every compiler in build/compilers).
- Result:    FloorMap_SetRowTexPalette 88.3% to 90.3% (opcode selection byte-exact; remaining: hi/lo chain schedule + loop colors, open item).
- Confidence: repo_proven
- Applies to/a.k.a.: `v < C` negation folds to the same form; the sibling range-test idiom `(C << cntlzw(v ^ C)) & 0x80000000` for `v < 2^k` matches retail verbatim when written literally.

### sinit small-data init: complete-type extern arrays + SDA21 + 0xFF constants + descending stores (CFloorMap sinit_80250CB4, Wii/1.1)
- Symptom:   decomp emits `lis/addi` (HA/LO relocs) for `extern u16 lbl[]` writes where retail uses `li rX, sym@sda21` (R_PPC_EMB_SDA21); size mismatch.
- Cause:     (1) incomplete array type `extern u16 lbl[]` is not SDA-eligible - use a complete type (`extern u16 lbl[4]`) at file scope; (2) the constant was misread from the data image: retail stores `0xFF` (`li r12,255`), not `0xFFFF` (`lis+subi`) - trust the asm, not a guessed data image; (3) store order is descending ([3],[2],[1],[0]) per group.
- Fix:       complete-type file-scope externs + plain descending element stores. Pointer-init locals and chained assignments compile identically; interleaved pair statements regress store order.
- Result:    sinit_80250CB4 0.0% (0xac bytes) to FULL_MATCH 100% (exact 0x70/0x70, all reloc types/symbols aligned). An intermediate 3-li schedule permutation seen mid-session was an artifact of a mixed-state TU build (concurrent configure.py clobber) and vanished under clean Wii/1.1 - re-hexdiff residuals after TU-wide state changes before chasing them.
- Confidence: repo_proven
- Applies to/a.k.a.: any .sbss/.sdata init inside sinit functions; R_PPC_EMB_SDA21 witness reloc-site gate.
- UPDATE (CMenuEnemyState sinit_801134E8, 0xE0, FULL_MATCH): ascending groups + descending indices ([3],[2],[1],[0]) reproduces the full retail template with no extra work - per-group emission +6,+4,+2 reg-based then +0 absolute SDA21 last, 8 address `li` split 3 pre-stmw (volatile r11/r8/r7) + 5 post-stmw, shared-value `li` first (0, 255) then per-group values in descending-index order. First attempt (ascending indices) gave 16.1% / 0 structural / pure reg-swap mirror; flipping index order closed it to 100% / 16-of-16 relocs in one step. Same-recipe instance, no new mechanism.

### Loop address arithmetic association: `(u8*)p + i*N + OFF` prevents base-register hoisting (CFloorMap FloorMap_Draw, Wii/1.1 `-O4,p`)
- Symptom:   decomp prologue saves one extra callee-saved register and holds `p + 0x150` (array base) in it across a call-containing loop; retail keeps only the counter in a saved reg and computes addresses per iteration as `add r3, r30, r0` (p + i*8) followed by `lwz rX, 0x150(r3)`. Every subsequent stack displacement shifts, inflating the structural count.
- Cause:     writing the element address as `(u8*)p + 0x150 + i * 8` associates `(p + 0x150)` first, so MWCC strength-reduces the loop-invariant base into its own register (extra saved reg, cascading displacement drift).
- Fix:       reassociate to put the induction-variable term FIRST: `*(void**)((u8*)p + i * 8 + 0x150)`. MWCC then folds `i*8` via rlwinm into an add with p and keeps 0x150 as the load displacement. Also use a bottom-checked for-form (`for` with the condition re-testing each iteration), matching retail's [init][b->cond][body][inc][cond][blt] layout - a do-while places the body unconditionally first.
- Result:    FloorMap_Draw 60.1% -> 85.6% in one change (structural 56 -> 19, size exact 0x29c/0x29c); prologue save count now matches retail.
- Confidence: repo_proven
- Applies to/a.k.a.: any loop indexing a member array at a fixed offset through a pointer; pairs with register_mapping.md Rule C. Ruled out in the same chase: struct member-array access `p->arr[i].field` (loses strength reduction entirely, 23.4 percent), u8 vs u32 counter type (identical codegen).

### Call-site narrowing to `u8` params: only direct-call provenance skips the mask (CPassiveSkill CPassiveSkillLine_setSlotNamePanes, Wii/1.1 `-O4,p`)
- Symptom:   caller of a `...PcUc` (u8 last param) callee emits an extra `rlwinm rX,rY,0,24,31` immediately before the `bl` where retail passes the register bare; the 16-bit record-form test mask (`rlwinm. r5,r3,0,16,31`) is correct but every arg shape re-adds the byte mask.
- Cause:     MWCC narrows any argument whose type is wider than the declared param. The narrowing is skipped ONLY when the argument is the direct narrow-return call result (`u16 v = CALL(); f(v)`) - provenance tracking on the variable. Any arithmetic touching the variable anywhere in the function (`v & 0xFFFF`, `v & 0xFF`, `(u16)v`, `&=`) resets provenance and forces the conversion mask at every call site taking that value.
- Fix:       none found for the fused retail shape (mask+record-test+unmasked arg reuse). Ruled out: header return widened to int/u32 (regresses sibling matched callers that rely on trusted u16 returns - CPassiveSkillInfo_setNameTex 100->79%, CPassiveSkillLine_refreshSP 100->31%); explicit u16 casts; assignment-in-condition; separate u16 temp. Best residual: keep the 8-bit fused form (`(id & 0xFF)` test + `(u8)(id & 0xFF)` arg = single `rlwinm.`), accept the mask-width delta as a recorded near-miss.
- Result:    CPassiveSkillLine_setSlotNamePanes stable at 98.2% (1 instr diff); open-item packet in attempts.jsonl.
- Confidence: negative_result
- Applies to/a.k.a.: any TU calling PcUc/Us-caliber narrow-param helpers with computed values; pairs with the "return-type too narrow" rule earlier in this file (that rule fixes the compare side, not the call-arg side).

## Named vtable-pointer local pins the vptr across large copies: use interface-cast dispatch (ScnCamLoadMtx fix, Wii/1.1 -O4,p)
- Symptom:   struct-copy-heavy function is +8 bytes with one extra callee-saved register (r28) and every word color shifted by one slot
- Cause:     `CScnItemCameraVtbl* vtbl = *(...**)self; vtbl->slot9(self);` keeps the vptr live ACROSS the preceding 12-word copy, adding a 14th simultaneous value that forces a 4th saved register; retail re-reads the vptr after the copy. Removing the named local does NOT help - the scheduler still hoists the load
- Fix:       dispatch through a cast-interface type instead: `((CScnItemCameraRefreshIf*)self)->refresh();` (virtual declared at the right slot index) - MWCC then loads the vptr at its use point, late
- Result:    FULL_MATCH 100% on ScnCamLoadMtx in one change (0xb0/0xb0)
- Confidence: repo_proven
- Applies to/a.k.a.: any function ending in a virtual refresh/callback call after bulk data movement; prefer interface-cast over explicit-vtable-struct member calls everywhere (ctor + F8E4 already matched this way)

## Stray frsp around a single-precision op = retail negated through double: write (f32)(-(f64)x) (ScnCamDepthUp fix, Wii/1.1 -O4,p)
- Symptom:   function is -4 bytes vs retail; retail shows an apparently pointless `frsp f0,f0` between two single-precision stores/muls; decomp lacks it
- Cause:     retail's source negated the value through the DOUBLE domain for one of the downstream terms (`-(f64)near`), so MWCC emits fneg + frsp(round to single) + fmuls; writing plain `-near` folds both negations into one and drops the frsp
- Fix:       write the affected term as `(f32)(-(f64)near) * aspect` - reproduces fneg(f0,f1) + frsp(f0,f0) + fmuls(f0,f0,f2) exactly, and the full FPR coloring falls in line
- Result:    FULL_MATCH 100% on ScnCamDepthUp in one change after three failed scheduling-angle attempts (50% -> 100%)
- Confidence: repo_proven
- Applies to/a.k.a.: any unmatched function with an unexplained frsp adjacent to fmuls/fadds on a value that is also negated elsewhere; check WHICH downstream term the frsp result feeds before placing the cast

## Float `>` compare: write the negated le form to get retail's cror merge (func_804D401C fix, Wii/1.1 -O4,p)
- Symptom:   `if (f > lim)` on floats emitted a direct `fcmpu; bc 4,1` (ble skip) while retail emits `fcmpu; cror cr0[eq], cr0[lt], cr0[eq]; bc 12,2` (materialized le then beq)
- Cause:     retail wrote the negation explicitly; MWCC only builds the cror merge when the expression is the negated form
- Fix:       write the condition as NOT(f le lim) instead of f > lim — 35.1% to 74.3% in one change on func_804D401C (monolib/src/effect/code_804CC2B8), size went exact
- Result:    74.3% HIGH_MATCH (remaining residual is unrelated phi-copy placement)
- Confidence: repo_proven
- Applies to/a.k.a.: any float compare used as a branch condition; same family as the != 0 -O4,p/-O4,s lowering note. Try the De-Morgan/negated form first whenever a float branch shows cror on the compare result. SCOPE NOTE (CScnItemCamera/EFF8 negative result): the merge only appears when the compare runs on the plain value - if retail's branch follows a fneg of the compared value, the cror comes from elsewhere and negated-lt/ge rewrites are codegen-neutral

## Narrow-return declarations force mask-at-store: declare lookup helpers returning int when retail defers narrowing (getEIBProgress discovery, Wii/1.1 -O4,p)
- Symptom:   `u16 v = BdatGetU16Direct(...);` emits `rlwinm r3, r3, 0, 16, 31` after every call, shifting all downstream register allocations vs retail which has no post-call mask
- Cause:     MWCC trusts the declared return type: u16 return → upper 16 bits of r3 assumed garbage → narrowing mask emitted after every call. Retail source likely declared the helper returning int/wider, deferring all narrowing to explicit use-site casts
- Fix:       REMOVE the declaration from the shared header entirely; declare it as `extern "C" int func(...)` directly in each .cpp that needs wide-return codegen. Each TU compiles independently so there's no cross-TU redefinition conflict. Add `(u16)` casts at use sites that need the narrowed value.
- Result:    getEIBProgress went 94.8% → 100% (FULL MATCH); CPartyState.cpp compiles clean with its own u16 declaration
- Confidence: repo_proven (CEquipItemBox getEIBProgress + cross-TU break in CPartyState confirmed)
- Applies to/a.k.a.: any extern-C lookup/table function returning u8/u16/u32 where retail's codegen lacks post-call masks; grep for functions called by both matched and unmatched functions

## stmw save-area observability blocks witness certification for regswap-only diffs (sortEIBByNameKey/sortEIBByNameKey69 finding, Wii/1.1 -O4,p)
- Symptom:   functions with 0 structural mismatches and only pure register swaps fail the equivalence witness with "rho perm maps gpr rN -> rM; ABI registers must be fixed"
- Cause:     `stmw rN, off(sp)` / `lmw rN, off(sp)` write callee-saved register values to the caller-visible stack frame. If decomp assigns a different saved-register SET (e.g. stmw r25 vs retail's stmw r26), the save-area bytes observably differ even though program semantics are identical
- Fix:       none via source reshaping — requires exact saved-register allocation match (i.e. FULL byte match) or extending the witness/SMT tooling to model save areas as implementation-private
- Result:    sortEIBByNameKey, sortEIBByNameKey69, refreshEIBItemList permanently classified as needing FULL match; witness path closed for all stmw-using near-misses
- Confidence: repo_proven (confirmed on 3 independent functions)
- Applies to/a.k.a.: any function using stmw/lmw with register allocation differences; check prologue `stmw` range first before attempting source-level fixes

## Vtable offset decode convention: vtable offset_bytes / 4 - 2 = declared virtual index (CEquipItemBox closeEIBBox discovery, Wii/1.1 -RTTI on)
- Symptom:   source calls the wrong virtual method through a view class; decomp emits `lwz r12, 0x14(r12)` where retail has `lwz r12, 0xC(r12)`
- Cause:     MWCC with -RTTI on prepends offset-to-top and typeinfo pointers before the virtual function table, so declared index N maps to vtable offset (N+2)*4 bytes
- Fix:       decode: offset_bytes / 4 - 2 = declared index. E.g. offset 0x0C → index 1 → v01(); offset 0x88 → index 32 → v32(). Verify against known-good matched functions using the same view class
- Result:    closeEIBBox 26.8% → 100% FULL MATCH by fixing v03() → v01()
- Confidence: repo_proven (CEquipItemBox closeEIBBox + loadEIBFiles slot-34 confirmation)
- Applies to/a.k.a.: any virtual call through a view class; grep for `lwz r12,` followed by `mtctr/bctrl` in unmatched functions

## Unsigned `-(a<b)` `li r0,-1; subfe` loses the schedule hole to an independent `field=1` store (CItemBoxGrid func_801CCAF0, Wii/1.1 -O4,p + optimize_for_size)
- Symptom:   Retail `subfc; …; li r0,-1; subfe r3,r0,r0; stb flag; li r0,1; stb other`. Decomp either signed-`rlwinm` (size-exact) or unsigned with `li r0,1; stb other` in the hole and short `subfe rD,rD,rD` (8 bytes short).
- Cause:     Both the compare’s `-1` and the later `= 1` store want `r0`. The independent store is ready at `subfc` and steals the hole. A following call is a barrier, so writing the `=1` store after the call cannot match retail’s pre-call store. Comma/`leftover` locals do not create a real dep; `(under&0)+1` folds.
- Fix:       Keep the `=1` store data-dependent on the compare result without folding, or otherwise occupy the hole with `item`/`count-1` so `li r0,-1` wins. Not yet a closed source recipe — see CASES `func_801CCAF0`.
- Result:    80.1% near-miss when signed casts pad size; unsigned+store-before-call regresses to ~69%
- Confidence: hypothesis
- Applies to/a.k.a.: `-(unsigned)(a<b)` vs later `p[off]=1`; subfe vs rlwinm sign-bit

## Diff-1 return pairs: `if (c) return K; return K+1;` lowers to branchless addic/subfe — route through a named local (eibHudPrompt fix, Wii/1.1 -O4,p)
- Symptom:   state-code dispatchers returning adjacent constants (`if (vis) return 0x1d; return 0x1c;`) emitted branchless `addic r0,c,-1 / subfe / addi r3,r3,K` selects where retail has explicit `cmpi / li FALSE / beq / li TRUE / b`
- Cause:     MWCC's integer-select formation fires only when the two result constants differ by exactly 1 (carry trick yields 0/1 offset); diff>=2 pairs keep branches
- Fix:       route the pair through a named local: `int ret = K; if (c) ret = K+1; return ret;` — breaks the select pattern and reproduces retail's li/beq/li block
- Result:    eibHudPrompt 59.7% → 100% FULL MATCH; also exposed two pairs where the inherited source had the manyParty semantics REVERSED vs retail bytes (cat2-nonvis many→0x1e, final-nonvis many→0x18) — byte-matching caught a real behavior bug
- Confidence: repo_proven (CEquipItemBox eibHudPrompt)
- Applies to/a.k.a.: any dispatcher returning small adjacent codes gated on booleans; grep source for `return 0x` pairs differing by 1 inside one block

## Magic-constant conversion helper: union store order sets the scratch colors (s16ToF_b0f0, Wii/1.1 -O4,p)
- Symptom:   inline `(union){double,u32[2]}` helpers (0x43300000/exponent-trick s16-to-f32) emitted the magic-constant temp and the sign-xor temp with swapped scratch colors vs retail (lis r7/xoris r6 instead of lis r6/xoris r7)
- Cause:     the helper wrote `c.w[0] = magic; c.w[1] = value;` — birth order of the two temps inside the inline expansion drives volatile coloring; the retail statement order is reversed
- Fix:       write `c.w[1] = (u32)(s32)v ^ 0x80000000u;` FIRST, then `c.w[0] = 0x43300000u;` — cleared the swap and improved func_804CD9EC 24.1% to 28.9% (monolib/src/effect/code_804CC2B8)
- Result:    28.9% and counting; residual is local stack-slot layout (union temps vs destination struct mirrored)
- Confidence: repo_proven
- Applies to/a.k.a.: all 0x43300000/2^52 conversion helpers (s16ToF/u16ToF/u8ToF/s32ToF families); check each helper's store order against retail's stw sequence before blaming declaration order at the call site

## s32→f32 conversion: drop the union helper entirely — `(f32)(int)x` makes MWCC emit its own magic + pool constants (getEIBProgress fix, Wii/1.1 -O4,p)
- Symptom:   hand-rolled `CEquipItemBoxF64Conv c; c.w[0]=0x43300000; c.w[1]=(u32)m^0x80000000;` + explicit subtraction of the retail pool constants (`c.d - lbl_eu_80668B18`) emitted DOUBLE-precision ops (`fsub`/`fmul`, FC-opcode) where retail has SINGLE (`fsubs`/`fmuls`, EC-opcode); 19 mismatches in the FP web alone
- Cause:     the retail source never touched those constants — `lbl_eu_80668B18` (43300000_80000000 = 2^52+2^31) and `lbl_eu_80668B10` (43300000_00000000 = 2^52) are MWCC's OWN implicit s32→f32/s32→f64 correction constants. The lfd-pool + fsubs sequence IS the compiler's inline expansion of a plain float cast
- Fix:       delete the union and the constant references; write the cast directly — `(f32)(int)((u32)v1 * (u32)n)` for signed (emits xoris+B18 path), `(f32)(u32)x` for unsigned (plain store+B10 path). Return-site conversions likewise: `return (f32)(u16)result;`
- Result:    getEIBProgress 67.2% → 94.8% (0 structural, 3 role-clean reg-swaps); reloc drift cleared TU-wide by postprocess pool_patterns renaming @N→lbl_eu_80668B18/B10 + extern_data_sections
- Confidence: repo_proven (CEquipItemBox getEIBProgress)
- Applies to/a.k.a.: every site that builds 0x43300000 unions then subtracts lbl_eu_80668B10/B18/B1B0-style constants — that whole dance is re-implementing `(f32)(int)` / `(f32)(u32)`; grep for `- lbl_eu_80668B10`/`- lbl_eu_80668B18` to find candidates

## Dead pooled .sdata2 constant: pool_patterns cannot fire without a live reloc — use drop_data_tail (CScnItemCamera fix, Wii/1.1 -O4,p)
- Symptom:   `data diff` shows `.sdata2` retail 0x0 vs decomp 0x8 containing the int->double magic pair 43300000_80000000; no relocation in the .o references it
- Cause:     MWCC still emits the pooled copy of a magic constant even after every use is rewritten to build the double on-stack (union store + lfd); with zero live refs, UnitRules `pool_patterns` + `extern_data_sections` match nothing (they rename via relocations) and the blob survives postprocess
- Fix:       add `drop_data_tail=((".sdata2", 0x0),)` to the unit's UnitRules in tools/postprocess_reloc_names.py; keep the existing `pool_patterns` entry as a safety net for live-reference variants (see CScnItemCamera.o / CVirtualLightObj.o rules)
- Result:    VERDICT MATCH on main/monolib/src/scn/CScnItemCamera (.sdata2 empty both)
- Confidence: repo_proven
- Applies to/a.k.a.: any unit where a lone float/double pool literal becomes unreferenced after source-side dissolving; first check the .o for sdata2-referencing relocations — none means drop, some means rename+strip

## Dead @LOCAL float-zero pool + FORCEACTIVE emitter trim: dissolve both via UnitRules, never source surgery (CWorkSystemCache fix, Wii/1.1 -O4,p)
- Symptom:   `data diff` shows `.sdata2` retail 0x0 vs decomp 0x4 holding a bare 00000000; separately hexdiff shows every function at 100% but split FAIL with decomp .text over budget by exactly the size of N contiguous `FORCEACTIVE<unit>_cppNNN__Fv` stubs packed after the last real function
- Cause:     two independent artifacts of dissolving data at source level: (1) MWCC allocates an `@LOCAL@<func>@zero` pool constant during compilation even when the finished code has zero float instructions — the word is emitted and referenced by nothing; (2) DECOMP_FORCEACTIVE emitters exist only to keep const blobs alive at compile time, but once compiled the blobs are global definitions in their own sections and do NOT need the .text refs to survive linking — so the stub bytes are pure split-budget waste
- Fix:       in the unit's UnitRules in tools/postprocess_reloc_names.py: `drop_data_tail=((".sdata2", 0x0),)` for the dead pool, plus `drop_text_symbols` naming each `FORCEACTIVE...__Fv` emitter (find exact names/sizes by listing local FUNC syms in the built .o's .text tail); keep the DECOMP_FORCEACTIVE macros in the cpp — they still do their compile-time job, the rules just shave the orphan bytes afterward
- Result:    VERDICT MATCH + split PASS (0x0 spare) on main/monolib/src/work/CWorkSystemCache, 14/14 functions untouched
- Confidence: repo_proven
- Applies to/a.k.a.: any dissolved unit where retail has empty .sdata2 or a tight .text budget; contrast with MWCC_CASES "FORCEACTIVE trim" which removes redundant stubs at source when some blobs have other live anchors — this rule drops ALL of a unit's stubs post-process and is safe because section-defined globals don't rely on text relocs to be kept

## Absolute value: use the float-width builtin (__fabsf), never double __fabs, on f32 expressions (Wii/1.1 -O4,p)
- Symptom:   `__fabs(f32expr)` emitted width-conversion sequences and wrong shapes; function was 12 bytes over with 121 structural mismatches
- Cause:     `__fabs` is the double builtin — MWCC widens the f32 operand, applies the 64-bit fabs pattern, and narrows back; retail used single-precision absolute value
- Fix:       use `__fabsf(expr)` for every f32 operand — func_804CDF20 went 2.2% to 22.8% in one change (monolib/src/effect/code_804CC2B8)
- Result:    22.8% STRUCTURAL (size delta shrank from exact to -12 pending PS-vector reconstruction of the distance block)
- Confidence: repo_proven
- Applies to/a.k.a.: any abs/fabs/negative-absolute on float math; audit all `__fabs(` uses when a function shows width-conversion noise around dominance/compare chains

---

### Do not name single-use intermediate loads - direct member reads let MWCC CSE into scratch like retail (CEquipItemBox eibApplySelect, FULL_MATCH 100%)
- Symptom:   after fixing field extractions and switch shape, 17 pure reg-swaps remained: retail held the halfword load in scratch r0 with lo/hi extracts in r4/r6; decomp put the load in r4, lo in r0, hi reusing r4 - every web permuted.
- Cause:     the decomp declared a named local for the intermediate load (u32 w = self->unk_1fc). A named local births its own register web that steals a color and shifts all downstream volatile assignments; retail never materialized it - the two reads were CSEd into one scratch load.
- Fix:       delete the named intermediate and write both member reads directly (lo = self->unk_1fc & 0xF; hi = (self->unk_1fc >> 4) & 0xF). MWCC CSEs them into a single r0 load exactly like retail. Went 69.1% -> 100% instantly.
- Result:    FULL_MATCH 100%
- Confidence: repo_proven
- Applies to/a.k.a.: any function with reg-swap plateaus where a short-lived load was hoisted into a named local "for readability"; pairs with the u32-local + (u8)-cast-at-call idiom for u8-returning noinline calls (rebuildEIBPage, FULL_MATCH).

### Witness extension spec: symmetric-compare operand-swap tolerance (DESIGNED, NOT YET IMPLEMENTED)

Blocker it removes: CActorParam vf9 (us-8017e6b8, 99.8 pct static) - retail
fcmpu cr0,f0,f31 vs ours fcmpu cr0,f31,f0; both sides load identical values into
identical registers, only the compare A/B operand order differs.

Why the witness refuses: gate 4 rho bijection - the swapped pair demands
retail-f0 to our-f31 AND retail-f31 to our-f0 while every other slot demands
identity - a many-to-one conflict. Gate 3 passes (both differing bits are
renameable FPR fields). The refusal is CORRECT for the general case: swapping
fcmpu operands flips CR lt/gt, so unconditional tolerance would be unsound for
any function that branches on lt/gt/unordered of that field.

Sound design (follows the audited Round-9 extension conventions in
tools/coop/lib/renaming_witness.py):

1. Pre-pass cr_field_non_eq_reads(instructions) returning the set of crfD
   indexes whose LT/GT/UN bits are consumed: scan BC/BCLR/BCCTR words; the BI
   field encodes crfD*4+bit, with bit in {0 LT,1 GT,3 UN} marking a non-EQ
   read of that crfD; also flag MCRF and CR-logic-op reads of the whole field.
2. In check_gates per-slot loop, when both sides are FCMPU/FCMPO with equal
   crfD and swapped register-field assignment but equal register SETS, and
   the crfD is absent from both sides non-EQ read sets: accept via the
   existing swap retry path (bits 16-20 exchange 11-15 already matches B-form
   A/B layout). Rho contribution becomes identity for both lanes because the
   exchange realigns the swapped operands.
3. Terminal-state safety: with operands realigned, symbolic execution of both
   sides produces identical CR trees; the EQ-only scan guarantees no divergent
   lt/gt consumption exists anywhere in either function.
4. Tests: vf9 (us-8017e6b8) must certify FULL_MATCH; add a synthetic negative
   test where a bgt consumes the swapped field and the witness must still
   refuse.

Estimated scope: about 60 lines plus tests in renaming_witness.py. Unblocks
vf9 (99.8 pct) immediately.

CORRECTION (post architecture study - implementer MUST read):

The witness shares ALL non-register state verbatim between sides - including
a single symbolic witness.cr 32-bit lane (see _initial_states in
renaming_witness.py: 'All non-register state (CR, XER, LR, CTR, GQR, SR,
MSR...) is shared verbatim'). Both sides execute their ACTUAL fcmpu writes
into that one lane: retail stores lt=f0<f31 while ours stores lt=f31<f0 -
different symbolic trees in the same shared bits. Even with a correct
gate-level swap tolerance, the terminal-state z3.eq over the full CR would
fail on the divergent lt/gt lanes.

Therefore the real implementation must ALSO canonicalize the compare write in
SymbolicOps/semantics.py: model FCMPU/FCMPO CR results with operands put into
a canonical order before building lt/gt trees. This touches soundness-
critical FP compare modeling and needs the synthetic negative test plus a
review pass. Estimated true scope covers gate pre-scan + rho retry +
SymbolicOps canonicalization + terminal comparison audit - budget
accordingly; not a quick patch.

IMPLEMENTATION ENTRY POINTS (verified by code read):
- tools/ppc_equivalence/semantics.py::_execute_fp_compare (~line 2603) is the
  single choke point: it computes cr_nibble via _fp_compare_nibble(left,
  right, ops), writes it into both the CR field (_set_cr_field) and
  FPSCR.FPRF bits 15..12, and raises FPSCR exceptions. A canonical-operand
  normalization belongs here: reorder (left_bits, right_bits) by a
  deterministic structural key over the symbolic trees before the nibble is
  built, keeping FPSCR/snan handling aligned with the reordered pair.
- The scalar_fp_exact_v2 concrete fast path inside the same function must
  apply the identical reordering before calling exact_fcmpu/exact_fcmpo.
- Witness-side gate pre-scan per the design above lives in
  tools/coop/lib/renaming_witness.py::check_gates.
Note: the nibble feeds FPSCR.FPRF as well as CR - canonicalization must keep
FPRF consistent with the reordered pair (it is value-symmetric, so
reordering the pair before nibble construction handles both uniformly).

### Cached local forcing extra callee-saved register (CPassiveSkill CPassiveSkillLine_syswinAdvance, Wii/1.1 `-O4,p` + optimize_for_size)
- Symptom:   decomp saves one more callee-saved register than retail (`stw r30` + `stw r31` vs retail's single `stw r31`), making the function 4 bytes larger.
- Cause:     a local pointer/variable cached from a member access keeps a register live across calls, forcing MWCC to save an additional callee-saved register. Removing the local and accessing the member directly at each use site reduces live-across-call pressure by exactly one register.
- Fix:       replace cached local with direct member access at every use site. E.g. `CSysWin* syswin = self->mInfo.field_54; ... sysWinAdvancePhase3(syswin);` → `... sysWinAdvancePhase3(self->mInfo.field_54);`
- Result:    CPassiveSkillLine_syswinAdvance 91.1% to FULL_MATCH (exact size 0x70/0x70).
- Confidence: repo_proven
- Applies to/a.k.a.: any function 4 bytes over size with an extra stw/lwz pair in the prologue/epilogue; pairs with register_mapping.md Rule C (liveness steering).

---

### Retail byte-packed .sbss tails: MWCC force-aligns every >=2-byte .sbss object to 4 — model tail spans as separate 1-byte globals (code_804F0258 fix, Wii/1.1 -O4,p)
- Symptom:   `data diff` .sbss short by the size of a retail object's anonymous tail bytes (e.g. retail `lbl_eu_80665A8A` is a 6-byte flag+pad object at +0x22; decomp .sbss 0x1C vs retail 0x28). Declaring the object as `u8[6]`, a u8-only struct, or wrapping in `__attribute__((aligned(1)))` / `__declspec(align(1))` all land it at +0x24 (section 0x2A) instead of +0x22
- Cause:     under `-align powerpc` MWCC packs only true 1-byte scalars contiguously in .sbss; ANY object of 2+ bytes (arrays, structs, even u8-only aggregates) gets forced 4-byte alignment, and align(1) directives are ignored on section data (they can raise alignment, never lower it)
- Fix:       declare the leading flag as a plain `extern "C" u8` scalar at its retail address and model the anonymous tail bytes as separate named 1-byte globals (`lbl_eu_..._pad`), kept alive via DECOMP_FORCEACTIVE (CLODCacheManagerS retail-tail recipe). The data gate compares NOBITS section size + alignment only, so extra pad symbol names are invisible to it; code relocs are unaffected since nothing references the pads
- Result:    VERDICT MATCH on main/monolib/src/core/code_804F0258 (.sbss exactly 0x28, A80 8-byte align-8 via `u8[8] __attribute__((aligned(8)))`, A8A..A8F packed at +0x22)
- Confidence: repo_proven
- Applies to/a.k.a.: any TU whose retail .sbss/.bss slice has byte-granular objects or anonymous gaps after an aligned object; for FILE-BACKED sections the equivalent trick is folding the gap into the previous array's declared size (CDeviceFontLayer recipe); contrast drop_nobits_range which shrinks decomp-side padding when MWCC OVER-aligns relative to retail

## Phantom .sdata2 FAIL from a stale build object: rebuild via hexdiff before writing pool rules (CMdlDynamics diagnosis, Wii/1.1 -O4,p)
- Symptom:   `data diff` shows `.sdata2` retail 0x0 vs decomp 0x8 (e.g. 3F800000 BF800000 ±1.0f pools), but the cpp already references only extern pool symbols — no local float literals anywhere in the source
- Cause:     the `build/us/src/**.o` on disk predates the source's pool-symbol conversion; MWCC was never re-run after the last source edit, so the data-diff compares retail against an orphaned blob
- Fix:       run any rebuilding command first (`hexdiff <unit> --all` or `--symbol <mangled>`) and re-run `data diff`; if the FAIL persists on the fresh object, then reach for UnitRules pool_patterns/extern_data_sections/drop_data_tail
- Result:    VERDICT MATCH on main/monolib/src/scn/CMdlDynamics with zero source/rule edits
- Confidence: repo_proven
- Applies to/a.k.a.: any "retail empty vs decomp N-byte" data-section FAIL whose source looks already dissolved; diagnose by parsing the fresh .o for `.sdata2` size + sdata2-referencing relocations before touching tools/postprocess_reloc_names.py

---

### Extern-data type change flips call-site codegen: keep the original decl type, rename the definition via exact_renames (code_804B2FF0 "ref" needle fix, Wii/1.1 -O4,p)
- Symptom:   after re-typing an extern data symbol for a dissolved data definition (`extern char lbl_eu_80663AA8[8]` → `const char[4]`), three unrelated 100%-matched functions in the same TU (Coli_FindNodeByName/B31EC/B33C8 — exactly the ones passing the symbol as an nw4r va-arg needle) regress to ~17% with +0x18/+0x30 size growth: each `if (!handle.IsValid())` site swaps the retail `cmpi/bc` fold for a materialized bool (`neg r0,r3; or r0,r0,r3; rlwinm r0,r0,1,31,31; bc`)
- Cause:     under `-str reuse,pool,readonly` + `-ipa file`, MWCC's readonly-object handling keys off the DECLARATION TYPE of the extern at the call site; changing non-const char[] to const char[] changes how the address is classified and downstream inline-bool folding in every function that passes it — the dissolved definition itself is innocent
- Fix:       keep the original declaration type untouched for code references; emit the retail bytes/size/section under a legal stand-in spelling (`const char sdata_ref_needle[4] = "ref"` with `__declspec(section ".sdata")` + aligned) and rename it to the retail symbol via UNIT_RULES `exact_renames=(("sdata_ref_needle","lbl_eu_80663AA8"),)`. The data gate only sees section bytes + reloc names, so the stand-in name is invisible once renamed
- Result:    VERDICT MATCH on main/monolib/src/coli/code_804B2FF0 (all data sections) with .text restored to the pre-change baseline (17/38 FULL_MATCH)
- Confidence: repo_proven
- Applies to/a.k.a.: any dissolve that needs a different type/size than the existing extern decl (char[N] vs const char[M], sized vs unsized arrays); bisect order when a data dissolve regresses matched functions: (1) novtable decl, (2) extern "C" protos, (3) dissolved-block halves, (4) per-symbol decl types — here only step 4 fired

### novtable + hand-built blocks scale to multi-table TUs: dispatch tables need extern "C" linkage, template slots need stand-ins (code_804B2FF0 dissolve, Wii/1.1 -O4,p)
- Symptom:   retail object carries shape-dispatch tables of free-function pointers (flat `func_XXXXXXXX` reloc names), class vtables whose RTTI slot points at a monolib `{name, table}` descriptor instead of `__RTTI__*`, and vtables referencing template-specialization members whose mangled names contain `<>`
- Cause:     MWCC auto-emits vtable + __RTTI__ + base-name strings + hierarchy tables for a key-method class; none of these exist in retail (monolib ships custom descriptors), and C++ mangles both the definitions and any spellable reference
- Fix:       `class __declspec(novtable) X;` forward-declared before all includes (kills all auto-emissions; safe when the dtor has no vptr store — verify with hexdiff first); declare the dispatch participants `extern "C"` BEFORE their definitions so the definitions adopt flat retail names (no codegen change); build vtables/descriptors as `u32[]` arrays with `(u32)&symbol` initializers in retail offset order; for unspellable `<...>` member names use extern "C" stand-ins + `retarget_relocs`; pad NOBITS totals with `_pad` byte arrays; force section aligns via `set_data_align` (.data 4 / .bss 8 / .sbss 4 typical)
- Result:    VERDICT MATCH on main/monolib/src/coli/code_804B2FF0 (.data 0x10C / .rodata 0x40 / .sdata 0x20 / .bss 0x68 / .sbss 0x1C, all bytes+relocs identical); extends the CDesktop.o recipe
- Confidence: repo_proven
- Applies to/a.k.a.: monolib coli/task-family TUs owning their own .data slices (contrast CfPadTask/CDeviceRemotePad where retail carries no data and extern_data_sections suffices); watch for the string-literal trap: retail `"CTTask<16CTaskColiManager>"`-style RTTI names are spelled with the IN-TU class name ("CTTask<CTaskColiManager>", 24 chars), read them from build/us/asm/*.s `.string` lines, not from memory

## `__declspec(align(N>4))` on a .rodata string re-orders/re-pads the section (CLibLayout fix, Wii/1.1 -O4,p)
- Symptom:   decomp .rodata larger than retail and objects in the wrong order (e.g. CLibLayout: 0x24 vs 0x20, name string emitted last at +0x18 instead of first at +0x0).
- Cause:     MWCC buckets const data by alignment; an 8-aligned string is emitted after/away from the 4-aligned strings, changing both order and padding. Retail's layout implies no special alignment.
- Fix:       drop the `__declspec(align(...))` from the string definition so all TU-local .rodata shares one alignment class and emits in declaration order; verify with a fresh hexdiff build + symbol dump before adding UNIT_RULES drops.
- Result:    VERDICT MATCH (.rodata 0x20 byte-identical)
- Confidence: repo_proven
- Applies to/a.k.a.: RTTI name strings, pooled literals, any "rodata size off by a few bytes + wrong order" residual; pairs with the stale-build trap (rebuild via hexdiff first).

### Placement-new intrinsic null guard: doubled-bc and guarded-member-address cases (CTaskLOD, FULL_MATCH ×2)
MWCC's `new (p) T()` always emits an intrinsic null guard (`cmpli/cmpi + beq` around the ctor call, address formed with `addic.` when the guard consumes CR0):
- **Doubled bc:** writing an explicit `if (lod) { lod = new (lod) LODMemMan(); }` duplicates the guard — MWCC keeps BOTH branches. When retail shows a single beq before the ctor call, drop the source-level `if` and just assign `lod = new (lod) T();` (the re-assignment also keeps `lod` re-derived from the ctor return, matching retail's r0 tracking). func_80462A08: 48.9% → 100% byte-identical.
- **Guard on a provably-nonnull member address:** for `new (&t->member) T()` where `t != 0` was checked earlier, MWCC still cannot prove `&t->member` non-null and emits the guard + `addic.`; retail (which proved it) has neither. Fix: replace placement-new with a direct extern-"C" ctor-symbol call (`extern "C" void __ct__Q23LOD9LODMemManFv(void* self); __ct__Q23LOD9LODMemManFv(&t->mLODMemMan);`) — same recipe as explicit `__dt__*` dtor calls. create__8CTaskLODFv: 57.6% → 100%.
- Keep placement-new where retail *has* the guard (heap pointers from allocate()).

### Undefined-but-declared virtual adds a phantom split function
Defining an empty override `void CTaskLOD::Draw() {}` whose vtable slot targets a base implementation (`Draw__17CTask<8CTaskLOD>Fv`) emits a 4-byte `Draw__8CTaskLODFv` that retail's split does not contain — invisible in per-function hexdiff (not in the unit function list) but it busts the .text split budget by exactly its size. If nothing odr-uses it, delete the definition and keep only the class declaration. Also check the retail symbol list for functions your TU defines that retail doesn't.


## novtable ctor vptr store: init-list sinks it, body-assignment + early-born locals restore retail schedule (CLibHbm fix, Wii/1.1 -O4,p)
- Symptom:   under `__declspec(novtable)`, a manual `*(void**)this = &vtbl;` written as the first ctor statement is SUNK below all init-list member stores (retail keeps the vptr store immediately after the base-ctor call); with member stores moved to the body, the store lands correctly but the two scratch `li` constants (0 / -1) swap colors (r4/r5 reg_swap, 0 structural).
- Cause:     MWCC list-schedules plain stores freely within the block, so an init list evaluates before any body statement; and scratch `li` colors follow virtual-register BIRTH order, which follows local/temp creation order — not the final store or emission order.
- Fix:       write member inits as BODY assignments after the vptr store (CLibLayout pattern), then force birth order with explicit locals declared before use: `u32 zeroBits = 0; const u32 invalidHandle = (u32)mtl::INVALID_HANDLE;` and assign members from them. The value that retail colors r4 (lowest scratch) must be created first.
- Result:    FULL_MATCH (ctor 52.5% pure-reg_swap -> 100%); whole unit data gate MATCH.
- Confidence: repo_proven
- Applies to/a.k.a.: any `__declspec(novtable)` class whose ctor has non-trivial member initialization + manual vptr install; pairs with "user .data emits before compiler-generated vtables" (declaration order IS .data emission order for novtable TUs — declare vtable, base-list, jumptable in retail offset order).

## novtable vptr store with non-trivial members: comma-expression side effect pins it into the member-init phase (ArcResourceAccessor fix, Wii/1.1 -O4,p)
- Symptom:   ctor byte-identical except the vptr store schedules LAST (after all member stores) while retail has it mid-group (standard ABI order: base call -> vptr -> member ctors). Body-assignment move is impossible when a member has a non-trivial implicit ctor (e.g. `ut::LinkList` zeroing + self-looping its root node) — its stores always emit before any body statement.
- Cause:     program order drives MWCC's store scheduling window; a body-statement vptr store can only be scheduled within/after the member-store group, never before it.
- Fix:       attach the vptr install to a member initializer as a comma-expression side effect: `ArcResourceAccessor::ArcResourceAccessor() : mArcBuf((InitVptr(this), (void*)NULL)) {}` with `inline void InitVptr(void* obj) { *(void**)obj = (void*)lbl_eu_...; }`. The store now competes for schedule position inside the member-init phase and lands exactly where retail puts it.
- Result:    FULL_MATCH (5 displaced-store reg-swaps -> 0 mismatches, 0x5C/0x5C); whole unit 7/7 functions 100%.
- Confidence: repo_proven
- Applies to/a.k.a.: complement to the CLibHbm body-assignment recipe above — use body-assignment when members are POD, comma-expression init when a member's implicit ctor must stay put.

## Folded template default-ctor never emits a standalone body: early explicit instantiation + exact_renames pairing (CScnItemPool fix, Wii/1.1 -O4,p -ipa file)
- Symptom:   retail symbol (`__ct__reslist_CScnItem`, 0x3C bytes) has NO counterpart in the decomp object — hexdiff pairs an unrelated function positionally; cycle reports "ambiguous decomp symbol ... (0 candidates)".
- Cause:     `-inline auto` folds the trivial template default-ctor into every call site and never emits a global body, even under `template class reslist<T>;` explicit instantiation.
- Fix:       (1) BEFORE any use of the type, force emission: `#pragma push / #pragma auto_inline off / template reslist<CScnItem*>::reslist(); / #pragma pop` (CWorkRoot precedent); (2) pair the name for objdiff via UNIT_RULES exact_renames: `("__ct__19reslist<P8CScnItem>Fv", "__ct__reslist_CScnItem")` — the emitted mangled template name differs from the retail label.
- Result:    FULL_MATCH (58% artifact row -> 100.0%, certificate issued); unit split PASS.
- Confidence: repo_proven
- Applies to/a.k.a.: monolib reslist family and any retail-labeled template instantiation; watch function EMISSION ORDER — the forced instantiation emits where written, so place it to match the retail object's function order or positional pairing shifts.

## Variant-proof jumptable addends: addend_sets pins absolute retail values (code_804BC9EC fix, Wii/1.1 -O4,p)
- Symptom:   unit `.data` carries a switch jumptable whose case-label addends differ from retail even though raw bytes match; worse, the decomp-side values CHANGE across rebuilds with no source edit (observed first-word variants 544 / 388 / 440 on code_804BC9EC func_804BC9F4).
- Cause:     ambient `-ipa file` scheduling flips the residual function's dispatch block layout per build, so case-label offsets are non-deterministic; any relative `addend_patches` delta set is valid for exactly one build.
- Fix:       new UnitRules field `addend_sets=((section, offset, ABSOLUTE_value), ...)` (backed by `set_reloc_addend`) writes the retail addend regardless of which variant produced the object — no re-derivation loop needed. Supersedes the re-measure advice in the func_804BC9F4 entry above whenever the gate only needs reloc equality (documented §17.6 code residual; behavior tests cover semantics).
- Result:    VERDICT MATCH on main/monolib/src/effect/... n/a — main/monolib/src/scn/code_804BC9EC data gate MATCH under three consecutive variant flips.
- Confidence: repo_proven
- Applies to/a.k.a.: any jumptable-in-.data unit whose dispatch function is a documented COMPILES residual; pairs with the pool-coupled dissolve recipes (CScnBloom.o family) and the stale-object rebuild rule below.

## Incomplete-type forward declarations break .data address commoning in ctors (CDeviceFont fix, Wii/1.1 -O4,p)
- Symptom:   a ctor taking addresses of three+ adjacent TU-defined .data arrays (vtable + sub-vtables) emits ONE `lis/addi` pair against an internal section-base symbol (`...data.0`) plus `addi rX, base, offset` for each use; retail materializes each label with its own independent HA/LO pair. Cascades into frame/saved-reg differences (base register repurposes `this`). hexdiff shows reloc drift "decomp-side only ...data.0" + "retail-side only lbl_eu_*" at every site.
- Cause:     MWCC's address-constant commoning: when complete types make adjacency provable (sized array definitions visible before the ctor), all same-section addresses fold to one base anchor. The commoning survives volatile barriers, intervening calls, and access-spelling changes (all probed negative — `.scratch/probe/probe_slot.c`).
- Fix:       give the ctor INCOMPLETE declarations (`extern "C" u32 lbl_eu_8056C734[];` — unknown bound) and place the sized definitions AFTER the ctor function body. With sizes unknowable at the ctor's compile point, commoning cannot prove adjacency and each address materializes independently, byte-for-byte like retail. .data layout is unchanged (declaration order within the section is preserved).
- Result:    FULL_MATCH (ctor 23.7% / 12 structural / 33 reg_swap -> 100% / 0 / 0, exact 0xE8; semantic-certified). Data gate unaffected.
- Confidence: repo_proven
- Applies to/a.k.a.: any ctor/function taking addresses of 2+ adjacent same-section arrays it also defines in-TU; probe recipe in `.scratch/probe/probe_slot.c` (V1 grouped vs V3 incomplete = retail pattern); pairs with the novtable vptr-store entry above (same CDeviceFont-style manual vptr installs).

## Volatile word-view of singleton globals: forces anti-CSE reloads and blocks load sinking (CLibLayout isInitialized/func_8045F438, Wii/1.1 -O4,p -ipa file)
- Symptom:   retail reloads a global's fields multiple times (loop-head reloads, double loads across branches) or preloads a field BEFORE a branch merging both paths; our compile CSEs the reloads into one named-local read and/or sinks the preload into one arm — wrong instruction count/order even when every other shape matches.
- Cause:     `-ipa file` proves the plain pointer/global reads redundant and merges them; plain member reads through a cached local get sunk below branches when the value is dead on one path.
- Fix:       read the object through `const volatile u32*` (word view): `static_cast<const volatile u32*>(static_cast<const volatile void*>(inst))` (MWCC rejects the direct CLibLayout*→volatile u32* static_cast — go through void*). Each spelled read is an access the compiler must emit, so double reads survive CSE, per-iteration reloads stay inside loops, and preloaded values dominate branches. Spell the SAME offset twice where retail loads it twice; use signed (`int`) casts when retail emits `divw` rather than `divwu`.
- Result:    isInitialized reached FULL_MATCH (co-owner finished from this base); func_8045F438 went 30.2%→69.8% with 0 structural diffs and exact size (parked near-miss only on ABI-boundary witness rule)
- Confidence: repo_proven (shape reproduction verified byte-level by hexdiff in two functions)
- Applies to/a.k.a.: any singleton/global with multi-read patterns (msgQueue scans, hash lookups, alloc callbacks); complements ref:8303d15428 (EH sp-save frame coupling) — same anti-CSE need, different mechanism

## VALIDATE-style range-check macros: decode retail's flag COUNT from the expansion before writing the macro (CDeviceFont, Wii/1.1 -O4,p)
- Symptom:   TU .text exceeds split budget by hundreds of bytes with no single large cause; hexdiff shows the pointer-validation macro expansions preset N+2 flag registers where retail presets N, and each macro instance carries 1-2 extra compare/branch pairs.
- Cause:     the hand-written boolean expression had MORE range clauses than retail's inlined assert: we merged two adjacent ranges (0x80/0x81) into one flag and appended extra tails (top5==0xD0, (addr&0xFFFFC000)==0xE0000000) that retail's version does not check.
- Fix:       disassemble ONE retail macro instance and count the pre-set flag registers (`li rX,1` chain) — that count IS the number of range conditions. Rewrite the source macro to exactly that many separate flags (retail kept 0x80 and 0x81 as SEPARATE flags). ~430B recovered TU-wide across 14+ instances.
- Result:    SPLIT GATE PASS (decomp .text 0x23A0 -> 0x2218 vs budget 0x2370); wkRender structural count down.
- Confidence: repo_proven
- Applies to/a.k.a.: any panic/assert-style macro inlined many times; also a general lesson — when split budget overflows without an obvious cause, count `li rX,1` runs inside repeated inline-macro expansions.

## `==` operand order steers cmpl register colors on (loaded value, param) pairs (btm_is_sco_active, Wii/1.1 GC/3.0a5.2)
- Symptom:   hexdiff shows 0 structural, N reg_swaps consisting ONLY of `cmpl cr0,0,rA,rB` vs `cmpl cr0,0,rB,rA`, where one operand is a memory load and the other is the function parameter
- Cause:     source wrote `*(T*)(expr) == param`; MWCC colors the load into the first cmpl operand and the param into the second — retail's expression had the parameter on the LEFT
- Fix:       flip the equality to `param == *(T*)(expr)` (plain high-level C); no declaration reordering needed when one side is an anonymous load
- Result:    btm_is_sco_active 89.3%+3 reg_swap -> 100% byte-identical; cycle upgraded EQUIVALENT_MATCH(witness) -> FULL_MATCH
- Confidence: repo_proven
- Applies to/a.k.a.: scan re-gate candidates for reg_swap-only verdicts whose swapped pairs are cmpl with a load on one side; try operand flip FIRST (one-line change) before declaration-order steering. Complements docs/register_mapping.md declaration-order contract — operand order acts on anonymous temporaries that declaration order cannot reach.

## RTTI stand-in names resolve to native __RTTI__* relocs under -RTTI on — no retarget rules needed (CLibLayout fix, Wii/1.1 -O4,p -ipa file)
- Symptom:   hand-built vtables/RTTI base lists need base-class typeinfo relocs, but spelling `__RTTI__X` in source trips the deferred 10322 ICE (see CDesktop pattern above); stand-in names look like they would need UNIT_RULES retarget_relocs.
- Cause:     under `-RTTI on` the compiler already knows the typeinfo symbols for every in-scope class; a legal stand-in extern (`extern u32 rtti_10IWorkEvent[];`) used as `(u32)&rtti_10IWorkEvent` gets unified with the real typeinfo symbol during emission.
- Fix:       declare stand-ins (`extern u32 rtti_X[];`), reference them in manual tables, and SKIP the retarget_relocs rule entirely — the raw object emits retail-named relocs. Verify with `run.py data diff` ("raw object already data-matched").
- Result:    CLibLayout data MATCH raw; no §17.6 postprocess needed.
- Confidence: repo_proven
- Applies to/a.k.a.: monolibdata dissolves of TUs owning class data; complements the CDesktop/CProcRoot 10322 stand-in pattern.

## DECOMP_FORCEACTIVE is unnecessary for zero-init .sbss/.bss tail globals under -ipa file (CLibLayout fix, Wii/1.1 -O4,p -ipa file)
- Symptom:   dissolved-TU recipes reflexively add DECOMP_FORCEACTIVE for unreferenced sbss/bss tail symbols; each entry costs real .text (vararg emitter function grows per symbol) and can break an exactly-full split budget.
- Cause:     assumption that `-ipa file` GCs unreferenced zero-init definitions. It does not, at least for scalar/array POD tails in small-data sections.
- Fix:       delete the FORCEACTIVE stub, rebuild via hexdiff, re-check the owning TU's data gate (.sbss/.bss size must stay at retail). Only re-add for symbols proven to vanish.
- Result:    CLibLayout .sbss stayed 0x10 with the stub deleted; split budget went OVER(0x18) → exact PASS.
- Confidence: repo_proven
- Applies to/a.k.a.: any dissolve TU flirting with its split budget; check before trimming elsewhere.

## FORCEACTIVE is LOAD-BEARING for zero-ref file-scope STATIC objects — only external-linkage defs survive deletion (code_804BC9EC fix, Wii/1.1 -O4,p -ipa file)
- Symptom:   deleting a dissolved-TU's FORCEACTIVE stubs shrank `.bss` below retail (0xEC → 0xA4, exactly −0x48 = the two guarded pads); data gate FAIL because following tail objects shifted up.
- Cause:     MWCC eliminates unreferenced `static` file-scope objects even under `-ipa file`; only definitions with EXTERNAL linkage are assumed reachable from other TUs and always emitted. The CLibLayout rule above holds for globals only.
- Fix:       per-symbol triage: grep linkage first — delete stubs for `extern "C"`/non-static defs, KEEP stubs for `static` defs (or promote static→extern if retail has no such symbol and the symbol table doesn't matter). Always re-verify with `run.py data diff` + hexdiff; revert kept-stub decisions individually, not whole-file.
- Result:    code_804BC9EC: 5/7 stubs deleted (split spare 0x28→0x6C), 2 static-pad stubs kept, data MATCH restored; CScnItemLight: 9/9 deleted flipped split FAIL(+0x90) → exact PASS.
- Confidence: repo_proven
- Applies to/a.k.a.: complement to the CLibLayout rule above and the CLODCacheManagerS pad recipe; also explains why blob-surgery pad arrays (`*_bss_pad_*`) are declared `static u8[]` + FORCEACTIVE as a pair.
- Round-2 corroboration (2026-08): the third option beats both keep-stub and promote-to-extern when the static pad trails a modeled object: FOLD the padding into the owning object's type (code_804BC9EC: grew header ScnResData with `u8 blockTail[0xC4 - 0x7C]` so the definition spans the full retail block). No separate statics -> nothing to guard -> both FORCEACTIVE emitters deleted, .bss shape now exactly retail (one 0xC4 object), data MATCH, split spare 0x6C->0x8C. Requires the struct's owner header to be TU-exclusive (or the grown tail harmless to sibling users) and a size that is NOT a multiple of 8 (avoids MWCC's 8-align-bss-array shift the old two-filler scheme worked around).

### Comments must be pure ASCII under `-enc SJIS` (sjiswrap gate)
The build preprocesses every source through `tools/sjiswrap.exe`; non-ASCII comment characters trip it:
- Sequences invalid as Shift-JIS (e.g. UTF-8 em-dash `—`, curly quotes) produce a hard
  `sjiswrap: File ... contains Shift JIS encoding errors` build error.
- Worse, *some* non-ASCII bytes decode as valid-but-wrong Shift-JIS pairs silently (e.g. `§`
  = UTF-8 `C2 A7` parses as a lead+trail pair), so the build passes with a mojibake comment — no error,
  no warning. Grep for `[^\\x00-\\x7F]` before committing; write comments in ASCII only.
  (Found when documenting CTaskLOD's intentional uninitialized-out-arg idiom.)

## Inverted branch polarity (`bc 12,x` vs `bc 4,x`) — negate the source condition
- Symptom:   hexdiff shows exactly one mismatch: retail `bc 12,0` branching OVER the if-body vs decomp `bc 4,0` branching over the same body; everything else identical.
- Cause:     retail was compiled from `if (!(a < b)) body` / equivalent negated form, so MWCC emits a branch-if-true that jumps past the body; writing `if (a < b) body` yields branch-if-false.
- Fix:       wrap the condition in `!(...)` instead of rewriting it as `>=` — a rewritten comparison (e.g. `(s8)a >= (u8)b`) changes operand/cast shape and breaks expression order + reloc placement. Negation preserves operand order byte-for-byte.
- Result:    FULL_MATCH on PrevGridPage + NextGridPage (kyoshin/CItemBoxGrid), 100%, semantic-certified.
- Confidence: repo_proven
- Applies to/a.k.a.: any single-instruction bc-polarity residual; combine with ternary-vs-two-statement local init shape for scratch-color fixes in the same function tail.

## Compound-|| guards and if/else-if dispatch: split early-returns + real switch (CPassiveSkill CPassiveSkillInfo_draw + CPassiveSkill_update, Wii/1.1 -O4,p)
- Symptom:   (a) guarded draw/update function has its FIRST branch targeting the post-call join instead of the shared epilogue (one bc displacement differs, everything else identical); (b) state-byte dispatch emits inverted `bne`-over-body branches plus an extra unconditional jump where retail shows a clean compare chain of `beq` into case bodies.
- Cause:     (a) `if (a != 0) { call; } if (b && c) { call2; }` nests the second block under the first's join; retail source used sequential `if (x == 0) return;` guards that all funnel to one epilogue. (b) MWCC lowers if/else-if chains to bne-over style; a small `switch` lowers to cmpi/beq chains with break-jumps to the tail - byte-identical to retail.
- Fix:       rewrite guards as separate `if (field == 0) { return; }` statements (never compound `||` when retail shares one exit), and transcribe multi-case byte dispatch as an actual `switch` with `break`s. Verified combo took CPassiveSkillInfo_draw 96.6%->100% in one change and CPassiveSkill_update 73.1%->100% in two changes.
- Result:    FULL_MATCH x2, semantic-certified (us-80267054, us-8026fd18).
- Confidence: repo_proven
- Applies to/a.k.a.: any visibility-gated update/draw helper with flag guards + state machine dispatch; check bc polarity at each guard before touching anything else.

## getBdatStringColumnValue signature: spell param 3 `s32` everywhere, never `int` (repo-wide header rule)
- Symptom:   `(10197) illegal function overloading` at whichever header declares it SECOND, after a sibling agent touches any copy.
- Cause:     defining TU is ocBdat.cpp: `extern "C" u32 getBdatStringColumnValue(void*, const char*, s32 index)`. Under MWCC `s32` = `signed long`, a distinct type from `int`; mixed spellings across headers are an extern-"C" overload error. Both directions have been tried; only all-s32 builds.
- Fix:       every declaration (code_80135FDC.hpp, code_801862C0.hpp, CUIWindowManager.hpp, CPcKizunagram.hpp, cfsys/CfObjectImplWalker.hpp, plugin/ocUnit.hpp, include/kyoshin/cf/CItem.hpp, cf/object/CfObjectPoint.hpp) must read `s32`. No ABI effect for extern "C" calls; int-expression args convert implicitly with zero codegen impact.
- Result:    resolves the recurring cross-agent build break permanently.
- Confidence: repo_proven
- Applies to/a.k.a.: any helper whose definition spells narrow-int params as s32/s64 typedefs - declarations must copy the DEFINITION'S spelling verbatim, not the "natural" type.

## Table[b*8+c] lookups: hoist the scaled term into an explicit temp (CPassiveSkill CPassiveSkill_learnAllSkills, Wii/1.1 -O4,p)
- Symptom:   row-lookup block loads both out-param locals in the "wrong" order and folds the base pointer with the UNSCALED term (`add base+b0` before `rlwinm b1*8`), where retail loads multiplier-first and associates base with the scaled term; residual shows as swapped lbz pairs + one add-operand swap.
- Cause:     bare `tbl[b1 * 8 + b0 - 9]` gives MWCC freedom to commute the adds and schedule either load first. Retail source associated `base + (b1*8)` first - reproduced only when the scaled term is a named lvalue.
- Fix:       write `u32 idxN = bN * 8; u8 row = tbl[idxN + c - K];` - the temp forces lbz(multiplier local first)/rlwinm/add-base/add-other byte-for-byte. Took CPassiveSkill_learnAllSkills 87.9% -> 100% in two changes (one per mirrored block).
- Result:    FULL_MATCH, semantic-certified (us-80270208). NOTE: does NOT transfer to CPassiveSkill_loadSkillData, whose residual is whole-loop-body coloring, not association.
- Confidence: repo_proven
- Applies to/a.k.a.: any stride-8/stride-N table lookup fed by paired out-param locals after a call boundary.

## Single-bit test decode: emitted rlwinm rotate = 32 − C shift (evalParamChain, Wii/1.1 -O4,p)
- Symptom:   retail `rlwinm r0,r0,17,31,31` vs decomp `rlwinm r0,r0,18,31,31` after writing what looks like the same bit test; every shift/mask variant lands one rotation off.
- Cause:     for single-bit extractions feeding a bool normalize (`neg`/`or`/`rlwinm 1,31,31` chain), MWCC emits rotate = 32 − (C right-shift count). Naively decoding "rotate N selects bit (31−N)" sends you to the WRONG source bit: rotate 17 corresponds to C `(x & 0x8000) >> 15`, i.e. bit15 — not bit14. Verified empirically: shift 13→rot 19, shift 14→rot 18, shift 15→rot 17 on Wii/1.1 -O4,p.
- Fix:       to match an emitted rotate R, write `(x & (1 << (32-R))) >> (32-R)` (mask+shift form; pure-mask and shift-mask forms also worked for rotl0 mask-pair sites like `rlwinm …,0,21,21` ⇔ `& 0x400`). Cross-check polarity of the surrounding branch (`bc 12,x` vs `bc 4,x`) before assuming the condition is inverted — a beq-to-continuation reads backwards from early-return style.
- Result:    evalParamChain field260 bit-test matched exactly (was the last instruction-class mismatch besides saved-reg colors).
- Confidence: repo_proven
- Applies to/a.k.a.: any flag-merge idiom `byte | ((word >> n) & 1)` normalized through neg/or; also explains "off-by-one" rlwinm residuals in guard-heavy state machines.

## Missing retail-named stubs: declared-but-undefined externs poison symbol resolution TU-wide (CActParamAnim/CActParamData, Wii/1.1)
- Symptom:   hexdiff shows a target at 0% with decomp side = a 4-byte unrelated function or wrong body; objdiff cycle fails with `ambiguous decomp symbol 'func_XXXXXXXX' (N candidates)`.
- Cause:     header declares `extern "C" func_XXXXXXXX(...)` but no TU defines it, so `_resolve_candidates` falls through exact → substring → digit-strip fuzzy matching, which matches several unrelated mangled names.
- Fix:       reconstruct the tiny getter/setter/copy from its retail asm (usually 1–7 instructions) and define it under the EXACT retail name in the owning TU. For blr-only stubs previously given invented names (e.g. `initSub1`), rename to the retail symbol — ambiguous fuzzy resolution then disappears. Watch signature spelling against the header (`void*` vs typed params → 10197 illegal overloading if they diverge).
- Result:    getTurnTarget/B7B8/B3F0/51BC4/51BDC + B0B0/B0B4 renames → six FULL_MATCH accepts in one session.
- Confidence: repo_proven
- Applies to/a.k.a.: any unit where targets.json shows FULL_MATCH 0.0% ACCEPTED rows alongside ACTIVE COMPILES rows — audit `nm` for undefined `U func_*` symbols before believing the registry.

## sret helpers consumed as by-value args: use the value-returning spelling + nested call (CPassiveSkill CPassiveSkillLine_stepOpenTo5, Wii/1.1 -O4,p)
- Symptom:   function is exact-size but a pair of stack slots is mirrored - local VEC3 at sp+20 / by-value argument copy at sp+8, where retail has them the other way round; every load/store between the two calls differs only in displacement.
- Cause:     retail source nested the sret call directly into the consumer (`g(f(a,b,c))`), so MWCC allocated the hidden return buffer at sp+8 and the outgoing struct copy above it. A named local `VEC3 pos; f(&pos,...); g(pos)` allocates pos AFTER the arg area, mirroring the layout.
- Fix:       declare the helper TU-locally with the value-returning spelling - `extern "C" VEC3 f(Pane*,Pane*,Pane*);` - which is ABI-identical on PPC EABI (hidden pointer in r3 either way) but lets you write `g(f(...))`. If a shared header declares it sret-style, `#define`-rename it away BEFORE the include chain that pulls it (watch transitive includes!), #undef after, then declare locally. Sites that reuse the result twice keep the named-local form via direct-init `VEC3 pos = f(...)`; NEVER plain assignment `pos = f(...)` - that materializes an anonymous temp plus a copy-assign and grows the function (+0x50 across three sites).
- Result:    FULL_MATCH on CPassiveSkillLine_stepOpenTo5, 91.1% -> 100%, semantic-certified (us-8026b474).
- Confidence: repo_proven
- Applies to/a.k.a.: any GetPanePosition-style helper whose result feeds one by-value struct parameter; check which of the two stack objects sits lower before picking the spelling.

## Memberwise pointer copies batch unless accesses are volatile-qualified (CPassiveSkill CPassiveSkillInfo_copy, Wii/1.1 -O4,p -ipa file)
- Symptom:   dst/src member-copy helper compiles to ALL loads into separate registers followed by ALL stores, while retail interleaves load/store through one scratch register per member; dozens of structural mismatches at exact size.
- Cause:     retail could not prove dst/src disjoint, so every store ordered before the next load (conservative aliasing). With `-ipa file`, MWCC proves the in-TU call sites pass disjoint objects and reorders freely.
- Fix:       qualify both sides volatile for the copies (`((volatile T*)dst)->m = ((volatile const T*)src)->m;`) - volatile ordering forbids hoisting/batching and reproduces the strict interleave byte-for-byte. Named volatile pointer locals vs inline casts produce identical code.
- Result:    CPassiveSkillInfo_copy 32.4% -> 91.2% in one change (exact size). Residual: first volatile load hoists above the r31 prologue save (scheduler-internal; named-local vs cast spellings identical).
- Confidence: repo_proven
- Applies to/a.k.a.: any small dst/src struct copy helper whose retail body shows single-register interleaved pairs.
- Boundary:   does NOT help when the copies are already interleaved in decomp but surrounding scheduling drifts (OnFileEvent) - volatile member copies there were byte-identical to plain ones; MWCC still floats independent loop setup across the sequence. Only cures load-BATCHING.

## Union word-field write order pins the int->double conversion register flow (CPassiveSkill CPassiveSkillLine_refreshGridAlpha + CPassiveSkill_learnAllSkills, Wii/1.1 -O4,p)
- Symptom:   u8-to-float conversion via the {hi=0x43300000, lo=value} stack union emits lbz into a different register than retail and shifts every subsequent fp-operation coloring.
- Cause:     writing conv.w.hi first forces MWCC to move the GetAlpha byte out of r3 (its call-return register) before the magic constant load; writing conv.w.lo first uses r3 directly for the first store and lets lis take the dead register instead.
- Fix:       order union member writes value-first (lo), magic-second (hi) whenever the value comes from a call return. Proven in CPassiveSkillLine_refreshGridAlpha (13.4->17%, byte in r3 matching retail) and consistent with CPassiveSkill_learnAllSkills's learn-loop shape.
- Result:    improved match + aligned the whole downstream fp section's register coloring.
- Confidence: repo_proven
- Applies to/a.k.a.: any 0x43300000-magic int-to-double conversion idiom; pairs with the lbl_eu_80668910 named-pool rule earlier in this file.

## 6. Composite downcast offset = base-class sizes (CfPlayerExtraData fix)

**Symptom:** `subi r3, r3, <K>` after a getter differs from retail by a power-of-2-ish
constant (here exactly 0x80) while every other instruction matches.

**Cause:** the class used for a `static_cast` downcast has a byte-array stub base whose
size is wrong; the compiler computes `offsetof(Composite, ThirdBase)` from it and folds
it into one immediate.

**Diagnosis:** compute expected offset = sizeof(base1)+sizeof(base2); compare against
retail's immediate. The delta tells you which stub to resize.

**Fix:** resize the stub (`u8 field[0xA9C]` → `u8 field[0xB1C]`). Safe when the composite
type is referenced only by the affected functions and the stub has no named fields.

**Tool note:** hexdiff columns are **LEFT=retail, RIGHT=decomp** — verify by editing the
source once and confirming the RIGHT side changes.

## 7. %360-then-float-cast of the same int: temp birth order is CALLER-CONTEXT dependent — give conflicting callers private helper copies (CERot_AxisToMatrix fix, Wii/1.1 -O4,p)

**Symptom:** in table-interpolated trig (`deg = 57.29578f*rad; n = (int)deg; frac = deg - (f32)n;
idx = n % 360;`) some inline sites show the pure-reg-swap quartet: retail `mulhw r3,r0,r4` /
`xoris r0,r4,0x8000` vs decomp `mulhw r0` / `xoris r3` (plus the dependent stw/add) — while other
sites inlining the SAME static helper match with the original declaration order.

**Cause:** the two temps color by birth order (scratch claims low→high), but MWCC's vreg birth
order for the pair depends on the CALLER's register context once the helper is inlined — one
textual order cannot satisfy every site. CERot_AxisToMatrix needed idx-first; the cardinal-axis
builders (CERotXMatrix/44F8/4F8 family) need frac-first.

**Fix:** keep the shared helper in the order most sites need and add a private idx-first variant
(`CERotTrigIdxFirst`, byte-identical body) used only by the conflicting caller. Verify per-site
with hexdiff --all after each move.

**Result:** FULL_MATCH on CERot_AxisToMatrix (12 reg_swaps -> 0). Also fixed in the same unit: the
§17.6 pool_patterns target was a first-match mistake — verify against the retail split asm which
byte-identical pool copy (here lbl_eu_8066B268 vs lbl_eu_8066A388) the unit actually references
before pinning a content rule (`grep -c 'lbl_eu_8066XXXX@' build/us/asm/<unit>.s`).

**Confidence:** repo_proven (hexdiff byte-verified; frac-first confirmed required by the
cardinal builders — an earlier blanket reorder regressed them to +8 structural).

**Applies to/a.k.a.:** any `n % const` + `(float)n` pair from one integer (rotation tables,
angle normalization); pairs with the magic-conversion union-store-order pattern above.

## 7. Impure predicates: verify purity before declaring branches dead

**Trap:** an `if (f()) / else if (!f())` ladder looks like a decompilation bug
(the second arm unreachable) when `f()` is assumed pure.

**Reality check:** trace the callee to its leaf. `func_80086D9C -> CfT_FrameInWindow`
reads a live frame counter (fmod of play-clock); consecutive calls differ as time
advances, so re-checking is meaningful and retail's five call sites are genuine.

**Rule:** before flagging "unreachable branch / wrong second condition", confirm
the predicate has no side effects and reads no mutable state. Extern C thunks
hiding time/input-dependent logic are common in this codebase.

## setPaneVisible else-path argument correction (CPassiveSkill CPassiveSkillInit_triggerPane14/4470)
- Symptom:   0% match on small if/else helper functions that call a shared tail with different bool args.
- Cause:     source incorrectly passed `1` in both branches; retail passes `0` in the else path. The semantic error masked the real codegen difference (non-tail bl + stack frame vs tail b).
- Fix:       correct the argument. The codegen gap persists independently: retail's if-path emits `bl` with a stack frame while the else-path uses a tail `b`, plus an orphaned dead `bl` between the paths that cannot be reproduced from clean C++.
- Result:    semantic correctness improved; hexdiff score unchanged at 0% due to the irreducible frame/dead-code delta.
- Confidence: repo_proven
- Applies to/a.k.a.: any paired enable/disable helpers calling setPaneVisible(anim, bool) — verify per-branch arguments against retail asm before assuming symmetry.

## Duplicate extern "C" wrappers shadow member bodies under the same mangled name (repo_proven, CLibCriMoviePlay)
- Symptom:   a function whose retail symbol equals a member's mangled name (e.g. wkUpdate__16CLibCriMoviePlayFv) compiles to a 0x10-byte tail-call instead of the full body; hexdiff shows size 0x10/0x1C8.
- Cause:     defining BOTH the member `void wkUpdate()` AND `extern "C" void wkUpdate__16CLibCriMoviePlayFv(CLibCriMoviePlay*)` in one TU collides on the linker symbol; MWCC keeps ONE definition (the wrapper), silently discarding the member body.
- Fix:       keep only ONE provider. For members whose mangled name IS the retail symbol: delete the extern "C" wrapper and add a plain declaration for &-references in vtables. For retail names that are NOT member manglings (startMovie etc.): move the body into the extern "C" free function and delete the member (update internal call sites).
- Result:    wkUpdate/wkStandbyLogout went 0% -> 22.8%/12.2% from deletion alone; bodies land under retail names directly.
- Confidence: repo_proven
- Applies to/a.k.a.: novtable classes with hand-spelled vtables; also applies to statics referenced from free functions (call as Class::member()).

## postprocess strtab appends must pad to 4 bytes — unaligned .rela* kills objdiff (repo_proven, tooling)
- Symptom:   after retarget_relocs/exact_renames runs, objdiff-cli rejects the object with "Invalid ELF section header offset/size/alignment" while lenient parsers accept it.
- Cause:     retarget_reloc_to_symbol appends name+NUL to .strtab and shifts every later section by the raw length; odd append lengths leave .rela* sections at non-4-aligned offsets.
- Fix:       pad the appended bytes to a 4-byte multiple before shifting (strtab consumers only need NUL termination).
- Result:    objects pass objdiff again (CLibCriMoviePlay cycle unblocked).
- Confidence: repo_proven
- Applies to/a.k.a.: any rule that grows .strtab by manual section surgery.

## 8. Trivial accessors need noinline when retail calls them out-of-line

**Symptom:** decomp contains inline load instructions (`lwz`/`lha`/`lbz` + optional
`rlwinm` bit-extract) where retail has a `bl` to a small function.

**Cause:** MWCC inlines single-load accessor functions even at -O4,p with default
inline settings, because the body is cheaper than the call overhead. Retail's
compiler (same version but different TU context / IPO state) kept them out-of-line.

**Diagnosis:** look for `lwz rN, offset(rM)` followed by optional bit-extract
where `offset` matches a known struct field referenced by an extern "C" accessor.
Cross-reference: if a function matching that description exists and its address
is called by retail but the loads appear directly in your output → it was inlined.

**Fix:** wrap the definition in `#pragma dont_inline on` / `#pragma dont_inline reset`.
Do NOT use `__declspec(noinline)` — it can interfere with other MWCC optimizations.

**Example pair:** func_80083290 (`*(s16*)(data+0x532)`) and func_80083284
(`(*(u32*)(data+0x4EC) >> 20) & 1`) — both were inlined into func_80083118,
adding 16 bytes of inline loads where retail had two 4-byte bl instructions.

## 8. Late-definition inlining defeats `#pragma auto_inline off`
MWCC may inline a TU-local function into callers that appear EARLIER in the file,
even when `#pragma auto_inline off` precedes the callee's definition — the pragma
only affects code generated after it. Symptom: caller contains the callee's body
(syswin gates, page-wrap logic, sound call) and is oversized by exactly the
callee's size. Fix: `__declspec(noinline)` on the callee definition itself
(pragma-order immune). Confirmed: eibSortPageNext→D7C, eibPagePrev+eibPageNext→B94.

## 9. Raw-u8 address arithmetic vs spurious `(s8)` casts
For grid addressing like `(u8)(unk_377 + idx*4)`, retail emits NO extsb on the
byte load — write the cast exactly as `(u8)(self->unk_377 + ...)` without
`(s8)` on the byte field. Each extra `(s8)` costs one `extsb` instruction and
breaks linear alignment for all following code.

## 10. Zero-test on a byte field: record-form extsb vs cmpi
Retail tests `(s8)b == 0` via `lbz rN / extsb. rM, rN` (Rc=1 folds the compare).
To reproduce: keep the variable `u8` and cast only inside the test:
`u8 b = self->field; if ((s8)b == 0) {...}`. Declaring the variable itself as
`s8` swaps register allocation (load lands in the temp register instead).

## 9. Sequential early-return guards should be `||` chains

**Symptom:** decomp emits TWO branch instructions (conditional + unconditional
`b exit`) where retail has ONE for the same early-return predicate; cascading
offset drift through the rest of the function; decomp LARGER than retail.

**Cause:** retail's compiler merges consecutive `if (...) return;` guards into
a single branch chain (short-circuit evaluation). Expressing them as separate
`if` blocks in source prevents MWCC from seeing them as mergeable.

**Fix:** rewrite sequential guards that share the same return target as a single
`||` chain:
```cpp
if ((flags & 0x4) != 0 || checkFunc() || (flags & 0x80) == 0) {
    return;
}
```

**Measured impact:** func_8007CF64 8.7% -> 34.4% match, structural diffs
127 -> 73, from two guard merges alone.

## 10. `bool` params generate 12-byte booleanization — use `u8` when retail passes raw

**Symptom:** decomp emits `neg r0, rN` / `or r0, r0, rN` / `rlwinm rD, r0, 1, 31, 31`
(12 bytes) before a call where retail passes the byte directly.

**Cause:** MWCC booleanizes `bool` arguments to normalize any nonzero value to 1
(`(v | -v) >> 31`). Retail's source used `u8`/`int`, passing the raw value.

**Fix:** change the extern declaration from `bool` to `u8`.

**Measured impact:** func_8007CF64 34.4% -> 82.0% match (structural 73 -> 19,
size converged to exact 0x22c/0x22c) — the function calls MenuSnd_RestartBGM_88D34 four
times, each site dropping 12 bytes of booleanization code.

## `-ipa off` string pool order follows .text first-use — force declaration order with a data-side anchor (btu_hcif, GC/3.0a5.2 -ipa off)

**Symptom:** .data string literals emit in use order (function at the END of
.text pools first) instead of source declaration order; retail .data has them
in declaration order.

**Cause:** under `-ipa off` MWCC appends each literal to the pool at its first
.text reference while emitting functions, so pool order = text first-use order.

**Fix:** reference the literals that must pool early from a file-scope data
initializer placed before all functions (`char *const unit_pool_anchor[] = {
str_a, str_b };`). Data initializers create their literals at parse time,
ahead of every function. The pointer table itself was eliminated by the retail
linker (btm_sec orphan-anchor pattern): strip it via UNIT_RULES
`extern_data_sections`. Bonus: the strings stop being folded anon `@N` and
become named globals, which makes §17.6 exact_renames onto the retail split's
local labels trivial.

- Result:    btu_hcif `.data` gate MATCH; .text byte-identical pre/post
- Confidence: repo_proven

## const → non-const moves pooled strings/tables from .rodata to .data with zero codegen change (GXFifo / GXTev, Wii/1.1)

**Symptom:** data gate fails with `.rodata: retail 0x0 != decomp N` and
`.data: retail M != decomp 0` — same bytes, wrong section.

**Cause:** retail's original source declared these objects without `const`;
MWCC files `static const char[]` / function-local `static const u32[]` into
.rodata instead.

**Fix:** drop `const` from the declaration (keep explicit array sizes to pin
the split tail, e.g. `[0x18]` + `[0x16]`). Function codegen is unchanged —
only the section home moves. Verified GXSetTevOrder byte-diff identical before/
after (GXTev.c c2r[10]).

- Result:    GXFifo + GXTev data gates MATCH raw (no rules needed)
- Confidence: repo_proven

## Unmatched-text literal pools: dedupe compiler-duplicated words, then permute_sdata2_words into retail order (KPAD, Wii/1.1 -ipa file)

**Symptom:** .sdata2 is a permutation (+duplicates) of the retail pool; every
attempt to reorder declarations is ignored because MWCC folds `static const`
scalars into anonymous per-use literals.

**Cause:** pool order follows decomp .text first-use order — meaningless until
the functions themselves are matched.

**Fix (UNIT_RULES, no source churn):**
1. `drop_data_range` the compiler-duplicated literal words (same constant
   pooled once per use-site cluster).
2. New `permute_sdata2_words=(j→i, ...)`: position j receives the word
   currently at index i; symbols and their st_values follow their VALUES, so
   every existing .text load keeps reading the same constant.
Compute the permutation by greedy value-matching against the retail .sdata
asm dump. When the text is later matched, revisit: the permuted layout may
diverge from what matched codegen naturally pools.

- Result:    KPAD data gate MATCH (.data/.sdata/.sbss/.bss also realigned by
             declaration reorder alone — MWCC emits named globals in source
             order; only folded literals need the permutation)
- Confidence: repo_proven

## DECOMP_FORCEACTIVE on a data symbol is redundant when real code already references it (CLibG3d, Wii -ipa file)
- Symptom:   TU had `DECOMP_FORCEACTIVE(CLibG3d_cpp, lbl_eu_8056D0F0)` guarding the
             manual vtable array; split-size FAILed by exactly the stub's size
             (decomp .text 0x214 vs retail 0x204).
- Cause:     FORCEACTIVE exists to keep **unused** globals alive against dead-stripping.
             When the symbol is referenced from matched code in the same TU (here the
             ctor does `*(void**)this = (void*)lbl_eu_8056D0F0;`), the stub is pure
             .text overhead and its removal changes nothing else.
- Fix:       Delete the `DECOMP_FORCEACTIVE(...)` call. Recipe: (1) grep the TU for a
             genuine code/data reference to every symbol listed in the macro — if all
             are referenced, just delete it; (2) rebuild via hexdiff, confirm per-function
             FULL_MATCH unchanged, data diff MATCH, and split-size improves by ~0x10/stub.
             Only zero-init tails with NO other reference still need the macro (or a
             natural keep-alive: own the definition / real reference).
- Result:    6/6 FULL_MATCH preserved, data gate MATCH, split-size PASS 0x204/0x204.
             Corroborated (H4 pilot): UnkClass_80460308.cpp `lbl_eu_805231F8` string pool
             referenced only via `lbl + 0x00/0x1E/0x34` msg-pointer arithmetic in three
             `strm->msg =` stores — offset addends still emit live HA/LO relocs, stub
             deleted, functions unchanged, split improved by the stub's 0x10.
- Confidence: repo_proven
- Applies to/a.k.a.: DECOMP_FORCEACTIVE / DECOMP_FORCEACTIVE_DTOR / DECOMP_FORCELITERAL;
             monolibdata2-dissolve units (CScnBloom/CScnFrame/... pattern); any TU where
             the guarded symbol has an in-TU code reference. Batch-applied 2026-08:
             CScnTexWorkMan (80524010 externally anchored by CScnItemCamera sd table,
             80524020 live MemManager::create arg), CScnIdMan (vtable ctor/dtor refs;
             RTTI name defined-in-TU), CScnFogMan (assert blob + RTTI name defined-in-TU),
             CScnEnvLgtCtrl (both blob-dispatch tables live-ref'd), code_8047BB54
             (80658500 returned by func_8047C034; 806658B8pad .sbss tail per CLibLayout).
             All FULL_MATCH counts preserved, splits PASS (CScnIdMan exactly 0x184/0x184).
             Batch-applied 2026-08 (round 3): CWorkSystemCache (all 6 stubs live-ref'd:
             RTTI names via .sdata locator inits, locators via vtable init, vtable via
             manual ctor store, FD10 via 3B40 init, FD3C via ctor+dtor; retired the
             whole drop_text_symbols FORCEACTIVE rule from UNIT_RULES, split now
             exactly 0x4E4/0x4E4 raw), CNReqtaskRemove (5 live-ref'd via sinit/helper/
             initializer chain; 806659F8pad zero-ref .sbss tail kept alive by its own
             definition alone per CLibLayout, data gate MATCH incl .sbss 0x8;
             split exactly 0x114/0x114). Batch-applied 2026-08 (round 4):
             CLODCacheManagerS (8056D700 live call ref; 4 zero-ref .sbss pads
             8066575A/5C/64/68 survive via own definitions, .sbss span 0x34
             exact), CDesktop (all rodata/sdata/vtable/rtti symbols live-ref'd
             by code or in-TU data initializers; gap_20 sbss tail survives own
             definition; split PASS 0xAC spare), CDeviceFontInfoExt (6 data-only
             symbols chained by initializer refs, split PASS 0x1EC spare),
             code_804EE558 (5 zero-ref .bss arrays + jumptable_eu_805702B8 whose
             ADDR32 relocs come from its own initializer; all present in fresh
             .o symtab). No drop_text_symbols rules existed for these units.
             Note: hexdiff --all 'decomp .text' includes non-function @etb/@eti
             locals, so listed function sizes can sum exactly to budget while
             the split line still FAILs — compare against pre-edit state before
             blaming a stub removal.
             Batch-applied 2026-08 (round 5): CVirtualLightAmb (vtable live-ref'd
             by manual ctor store; 2 .rodata RTTI name strings survive as
             external-linkage definitions referenced by foreign-TU typeinfo —
             split FAIL 0x30 over -> PASS exact), UnkClass_80466348 (3 data
             arrays with zero TU refs survive via own definitions; split
             overflow reclaimed exactly the stub bytes, match count unchanged).
             COUNTER-RESULT (dvd_broadway.c, both stubs KEPT): a FORCEACTIVE can be
             a positional anchor, not just keep-alive. (a) The dvdContexts stub's
             early text reference fixes MWCC .bss first-reference ORDER — deleting
             it shifted sda-offset selection (addi r9,0 vs retail +64) and
             regressed DVDLowOpenPartition/TicketView even though the array has
             live refs everywhere. (b) The tmd-strings stub pools shared string
             literals at their retail .data offsets — deleting it re-pooled ~19
             functions' literals (addi pool-offset drift + anon-local relocs
             becoming section-symbol relocs). Recipe addition: after deleting a
             'redundant' stub, diff the WHOLE unit (hexdiff --all), not just the
            owning function; if unrelated functions regress, revert — the stub is
             load-bearing for literal pooling or small-data ordering.
             Batch-applied 2026-08 (round 5, FORCELITERAL / FORCEACTIVE_DTOR
             variants): nw4r math_geometry DECOMP_FORCELITERAL(math_geometry_cpp,
             0.5f, 0.0f) deleted — its fake function was a bare 4-byte blr stub;
             the TU's shared-pool constants are extern lbl_eu_* decls and
             FRUSTUM::Set references 0.5f/0.0f naturally, and UnitRules
             pool_patterns remaps local .sdata2 values by pattern (not order),
             so pool order needs no text anchor. 8/8 FULL_MATCH preserved,
             .text −0x4. nw4r snd_AnimSound.cpp (DECOMP_FORCEACTIVE_DTOR keep-
             alive for SoundHandle's dtor) was a DEAD TU — not in configure.py,
             never compiled, zero effect on any gate — file deleted instead of
             wiring it into the build just to host the macro. Recipe for both:
             delete → hexdiff --all (per-function FULL_MATCH + split delta =
             stub size) → retire any drop_text_symbols rule naming the stub.
             Batch-applied 2026-08 (round 6): CMdlDynamics (all 11 stubs:
             vtable lbl_eu_805701FC live-ref'd via ctor `vtbl =` store; RTTI
             name/anim-param/assert strings 80524830/40/70/88 — 40/70/88 have
             ZERO refs of any kind yet survive -ipa file as external-linkage
             const arrays, confirmed in fresh .o symtab + data gate MATCH;
             sdata locator/ref strings CB0–CC8 live via Panic args + locator
             init), CMdlLook (all 10 stubs: pool lbl_eu_805701E0 live-ref'd by
             ctor `field_00 = &`; 8052481C rodata + CA8/CAC sdata zero-ref
             externals survive own definitions; B3B8/C0/C8 anchored by
             805701F0's initializer). Function counts unchanged (48/66,
             5/8), both data gates MATCH byte-exact, splits PASS with large
             spare (.text −0x90 / −0x28 of stub waste). No drop_text_symbols
             rules existed for either unit.
             Batch-applied 2026-08 (round 7, 14 single-stub monolib device/core/
             scn/mpfsys/lod TUs): CGXCache (4 vtable lbls live via ctor/
             IStateCache/CMsgParam stores; rodata/sdata/sdata2_pool survive as
             externals; split overflow reclaimed 0x50→0x20), CDeviceVI (11 syms
             all live: vtbl/RTTI arrays + ctor/dtor stores), CDeviceFont (4 sbss
             labels heavily live), CDeviceFileDvd (80665670pad zero-ref external
             pad kept — it anchors .sbss span; only stub deleted), CDeviceFileCri
             (jumptable def-only external survived; blob-dispatch unit green),
             CDevice (def-only 8056C0B8/80522AD0 survived; initDevices unchanged),
             code_804F0258 (f32 labels live stores; 6 pad bytes external defs),
             CException (80522F88 rodata blob def-only survived), CScnItemModelNw4r
             (vtable live loads ×2), CScnFilterMan (blob def-only survived),
             CScnFadeMan (vtable live ctor+dtor; pre-existing split FAIL −0x60 not
             attributable to stub removal), CScnCameraMan (vtable live store),
             UnkClass_80471EC8 (RTTI locator def-only survived), code_8046A530
             (0.005f label live reads). No regressions anywhere; no
             drop_text_symbols rules existed for any of the 14 units.
             Batch-applied 2026-08 (round 8, 10 monolib core/scn/mpfsys/lib/
             lod/device TUs, 34 stubs): CView (80522630 live via line-1475
             msg-store; 225E0/225F0/22608 internal-linkage const arrays with
             ZERO refs survive file-scope emission — confirmed present in the
             fresh .o bytes, UNIT_RULES retargets intact), CPackItem (FF58
             vtable def-only external survived; 246FC/BF8 chained by locator
             initializers), CArcItem (FFE0 def-only external survived;
             24708/C00 initializer-chained), LODMemMan (3 multi-sym blocks:
             23D80 def-only rodata external survived; rest initializer/live
             chained; sbss gap tails survive own definitions per CLibLayout),
             CScnRootNw4r (E768 23-word blob + E7C4 def-only externals
             survived; 23FD0/23FE0 name strings def-only survived),
             UnkClass_8047E110 (DC74 live at line 1632; DC68 live at 1612;
             DC90/DCB8 def-only externals survived), MPFDrawDisplayList
             (DBA0 live protoAddr store; D98/68/70 initializer-chained),
             CLibHbmControl (all 4 def-only/initializer-chained externals
             survived), CDeviceFontLayer + CDeviceClock (rodata/sdata/data/
             rtti/vtable symbols all cross-chained by their own initializer
             graph; sbss labels heavily live). Function match counts
             unchanged everywhere; splits: all PASS except pre-existing FAILs
             at CPackItem and CDeviceClock that shrank strictly (A/B measured:
             −0x30 and −0x60 of stub waste respectively). No drop_text_symbols
             rules existed for any of these units. NOTE: a concurrent agent's
             write restored UnkClass_8047E110.cpp mid-campaign — re-deleted
             file-end FA lines only and re-verified (split spare grew
             0x36C→0x3D8) per the conflict protocol.

### 4b. Uninitialized function-scope decl births the vreg at the declaration point

Case-local `char* pFmt2 = pMsg + 0x428;` (`__wudDeleteHandler`, FULL_MATCH) was colored into the
lowest saved reg (r28) because its vreg was born at the late assignment. Hoisting an **uninitialized**
declaration to the FIRST line of the function and assigning inside the case births the vreg early;
the allocator then recycles the same high register retail uses (r31) once the early constant dies.
Use when a case-local pointer shows a pure reg_swap against a register that is dead on that path.
Note: this only moved colors when the hoisted decl was placed *before all initialized decls* —
reordering among uninitialized decls alone had no effect (birth follows first use for those).
- Result:    FULL_MATCH (98.6% → 100%)
- Confidence: repo_proven

## Pointer-walk loop: `<` vs `!=` decides unroll template (Wii/1.1, -O4,p)

- Symptom:   decomp of a zero-fill pointer loop over a struct-member array emits MWCC's unknown-count memset expansion WITH an 8x-unroll fast path (`subi rX,end,96`, `/96` blocking, magic signed division) — retail has only the plain compare/back-branch loop; +0xF8 size bloat.
- Cause:     a `for (p = first; p < end; p++)` walk is lowered through the count-computing memset-style template (with unroll); an `!=` walk lowers to a plain do-while compare loop.
- Fix:       write the walk as `for (p = first; p != end; p++)`. Verified both directions on __ct__Q22cf7CVisionFv: `<` = 19.8% (unrolled), `!=` = 26.0% (plain loop, decomp 0x2c4 vs retail 0x2d4).
- Result:    26.0% near-miss (loop-shape residual fixed; remaining diff is scheduling/regcolor)
- Confidence: repo_proven
- Applies to/a.k.a.: member-array init loops, "unroll fast path", divwu/mulhw count templates

## FORCEACTIVE is LOAD-BEARING for anon-namespace FUNCTIONS referenced only via extern retail dispatch tables (g3d dcc, Wii GC/3.0a5.2 -O4,p)

- Symptom:   TU keeps `DECOMP_FORCEACTIVE(unit_cpp, FnA, FnB, ...)` listing anonymous-namespace helper functions whose only "refs" are `extern "C" const FnPtr tbl[]` dispatch tables defined as raw retail data (lbls), so the compiler sees zero in-TU references. Deleting the stub compiles fine but hexdiff collapses: every listed function DCE'd (unit .text 0xD00→0x26C; matched 3/16→1/16).
- Cause:     dead-stripping works on functions too. The §3129 rule (static *objects*) and the §3439 rule (redundant when live refs exist) both concern data; internal-linkage function definitions with no visible call-site refs vanish just the same, even though retail calls them through pointer tables.
- Fix:       keep the DECOMP_FORCEACTIVE line. If a split gate overflows by roughly the emitter's size (vararg pointer-args grow it ~4-8 bytes/symbol), the clean escape is a zero-.text anchor (e.g. define the dispatch tables in-TU so real relocs keep the fns alive) — NOT deleting the guard. Verified empirically on g3d_xsi (delete→DCE→revert restored exact baseline); applied by identity to g3d_maya / g3d_3dsmax.
- Result:    kept with green per-function states (no regression vs baseline)
- Confidence: repo_proven
- Applies to/a.k.a.: DECOMP_FORCEACTIVE on function-name lists; complement to MWCC_PATTERNS §"unnecessary for zero-init .sbss/.bss tail globals" and §"redundant when real code already references it"; also seen anchoring split-space string literals mid-dissolve (WUD.c WUD_c/WUD_c_1/WUD_c_2 — deletion displaced downstream symbol↔retail-address mapping).
- Round-2 corroboration (2026-08): `__attribute__((used))` is NOT a zero-.text anchor on this MWCC — annotating all 14 g3d_maya builders and deleting the emitter dead-stripped 13/14 fns (survivor mis-sized/misplaced); reverted to exact baseline. External linkage is also closed: nw4r_data.s relocates the dispatch tables against the anon-namespace mangled names (...22@unnamed@g3d_<file>_cpp@...), so leaving the anonymous namespace breaks the relocs. The remaining escape (in-TU table definition) moves data ownership out of the TU owner's file list. Treat these emitters as required.

## Renaming a mangled C++ member method (`func_XXXXXX__Q22cfNClassFv` → real name): recipe (cf::CChainTime::func_8027CE30 → resetChainTime, Wii/1.1 -O4,p)
- Symptom:   accepted FULL_MATCH function still carries a placeholder member name; call sites read `obj.func_8027CE30()`.
- Cause:     name recovered late; the mangled symbol appears in source decl/def/call sites, region symbol maps, and the registry.
- Fix:       (1) grep repo-wide for the address (e.g. `8027CE30`) — there are almost always MORE call sites than the task lists (here: a dtor in CBattleManager.cpp); `.ctx.c` files are generated reference copies, skip them. (2) Rename decl/def + all call sites (method name only; signature untouched so bytes can't change). (3) Update the mangled LHS in **all three** regions' `config/{us,jp,eu}/symbols.txt` (`<new>__Q22...Fv`, same address/comment). (4) `run.py targets sync-symbols` updates the registry's function/symbol fields. (5) Verify with `hexdiff <unit> --all --brief` (100% expected — bl encoding is address-based, only reloc names change), then `cycle`. Same-length names are cosmetic; any rename remangles anyway.
- Result:    FULL_MATCH preserved, new semantic certificate issued, zero byte drift.
- Confidence: repo_proven
- Applies to/a.k.a.: member-method renames only — type renames should still use `symbols rename-all`; do NOT confuse with `extern "C"` retail-symbol aliases.

## Float multiply commutation is NOT source-order steerable via operand swap (negative result)
- Symptom:   retail `fmuls f1, f0, f1` (const * arg) vs decomp `fmuls f1, f1, f0` (arg * const) with identical load order; seen in CosFIdx/SinFIdx angle math (cf::CtrlAct testHeightGap / isMoveBlocked).
- Cause:     MWCC's scheduler/peephole picks the multiply operand order independently of the C source order; swapping `a*b` to `b*a` in source only permutes the sda21 const-load placement (reloc-site drift), never the emitted multiply order.
- Fix:       none found at source level. Do not chase fmuls/fmadd operand-order diffs by swapping operands — it made both targets worse (reg_swap 12→15, 13→17).
- Result:    negative_result (testHeightGap 89.1%→87.4% under GC/3.0a5.2; retested post-Wii/1.1-flip: 96.6%→95.4% — holds on both compilers; both reverted)
- Confidence: negative_result
- Applies to/a.k.a.: paired-single math helpers (CosFIdx/SinFIdx/Atan2FIdx callers), fp_contract on, -O4,p

## Narrowed virtual-call result compared `> 0` — declare the receiver `int` (<compiler flags>, FULL_MATCH-grade idiom)
- Symptom:   retail emits `rlwinm. rX,r3,0,16,31` (record-form u16 narrow) followed by `bng`-family branch; decomp emits either `rlwinm.` + `beq` (receiver declared `u16`) or `extsh` + bng (`s16` receiver).
- Cause:     MWCC canonicalizes an unsigned-typed comparison value (`u16 n ... if (n > 0)`) to an eq-test, and sign-extends an `s16` receiver; the retail source held the zero-extended value in a *signed* (int-width) local, so the narrowing stays fusable into the record form and the branch tests GT.
- Fix:       declare the receiving local `int` (or `s32`): `int n = obj->Vfn(...); if (n > 0)` — u16 return narrows via fused `rlwinm.` and the signed compare branches on GT.
- Result:    buildItemBox2SlotRec +0xb0/+0xb4 byte-exact (was 2 structural per site).
- Confidence: repo_proven
- Applies to/a.k.a.: any vtable/C-call returning u16/u8 compared `> 0`; record-form rlwinm.; beq/bng branch-condition mismatch category.

## Signed `%` by power-of-two constant: MWCC emits an alternate algorithm; reconstruct manually

## signed % 2^k — retail srawi/addze/slwi/subf unreachable via `%` operator → manual div/reconstruct (Wii/1.1 + GC/3.0a5.2, -O4,p)
- Symptom:   retail emits `srawi rQ,x,k; addze rQ,rQ; slwi rQ,k; subf m = x - q*2^k` for signed `%`; decomp `%` always lowers to the rlwinm-pair floor-mod (`rlwinm rotl(32-k-1); rlwinm sign-bit; subf; rlwinm; add; subf`) regardless of operand signedness
- Cause:     MWCC's `%` operator has one lowering path (the rlwinm-pair); the srawi/addze sequence only appears when the source performs the division and remainder reconstruction as separate statements
- Fix:       write `int q = x / 128; ... (s8)(x - q * 128) ...` instead of `x % 128`. Verified in .scratch probes on Wii/1.1 AND GC/3.0a5.2: opaque param / call result / named local / volatile reload all give the rlwinm-pair via `%`; the manual form reproduces srawi+addze+slwi+subf exactly
- Result:    MakeCrystal_FinishRound (CModelDispMakeCrystal) RNG gate went from permanent mismatch to byte-region match (43% -> 56.2% overall, % block fully green)
- Confidence: repo_proven
- Applies to/a.k.a.: any signed `%`/`/` by 2^k constant where retail shows srawi/addze; also explains "unsigned magic" mismatches near rand31()%N call sites (u32 return of MTRand::rand31 is NOT the cause — cast/local signedness does not change the selection)

## Ellipsis (`...`) prototype on a shared extern "C" import: MWCC emits `crxor cr1` before every call (negative result, Scn_QueryUnk80State, Wii/1.1 -O4,p)
- Symptom:   after retyping a shared import as `extern "C" void* f(...)`, every caller gains an extra `crxor 6,6,6` (cr1 clear) immediately before the `bl`, shifting all later offsets (+4 bytes/call site); a previously FULL_MATCH no-arg caller dropped to 25%.
- Cause:     for variadic-prototyped calls MWCC clears CR1 at the call site per the SysV varargs contract — even when zero arguments are passed and none are float. A fixed-arity prototype emits a plain `bl`.
- Fix:       never use an ellipsis prototype to unify mixed-arity retail call sites. Give the shared owner header one fixed-arity declaration, and give genuine stale-r3 (no-source-arg) retail call sites a documented TU-local fixed-arity declaration instead (they must not include the owner header).
- Result:    Scn_QueryUnk80State callers verified back at 100% (CMenuUpdate MenuUpdate_QueryUnk80Float, CfCam cfCam_updateFrame, CfSoundMan func_801BFC38, CMenuQstCnt Move/cbRenderBefore, CfGimmick CfGimmick_UpdatePartyAnchorState).
- Confidence: repo_proven
- Applies to/a.k.a.: single-winning-declaration (H3) cleanups; "too few arguments" vs stale-register call sites; any symbol called both with and without source-level arguments.

## fcmpu symmetric-operand swap on float equality vs extern const — named-local birth-order fix
- Symptom:   single-instruction residual `fcmpu cr0, fB, fA` (decomp) vs `fcmpu cr0, fA, fB` (retail); loads/div identical. Typical on `x != CONST` where x comes from a call and CONST is an extern const f32 (e.g. 0.0f).
- Cause:     with an *anonymous* compare operand, MWCC Wii/1.1 normalizes the fcmpu operand order to a canonical (persistent, scratch)-style order that does not match retail — expression shape cannot change it (see prior note in attempts: attempt:us-8017e6b8). That invariant only holds for anonymous temps.
- Fix:       hoist the compared value into a **named local declared before any other float local** (`f32 scale = ...; f32 result; if (scale != CONST) {...}`), keeping natural comparison order. Birth-order scratch coloring then assigns the value f0 / const f1 AND emits the compare in source order = retail's `(value, const)`.
- Result:    FULL_MATCH ×8 in kyoshin/cf/CtrlMoveEne (us-8009369c/750/804/8b8/9bc/9a98/b74/c50)
- Confidence: repo_proven (8 independent certifications)
- Applies to/a.k.a.: supersedes "operand order invariant to expression shape" for the named-local case; also fixed the 0xDC mtRand-gated variants; do NOT hoist the const itself into a local (it becomes call-persistent → f31 + early load, regresses).

## CItemBoxInfo rotation family: witness path CLOSED (confirms existing stmw save-area entry above); NEW: hexdiff can mask real store-displacement diffs as 'reg_swap'
- Symptom:   0-structural pure-reg-swap near-misses in stmw-framed functions fail cycle with abi-boundary (`rho perm maps gpr rN -> rM` for N inside the function's own stmw mask) — confirms the sortEIBByNameKey/sortEIBByNameKey69 stmw finding. Separately, hexdiff classified rows as pure `reg_swap` even when the store DISPLACEMENT also differed (e.g. `sth r28,18(sp)` vs `sth r30,16(sp)`), hiding real instruction-stream divergence from the diff view; the witness caught it at its fields gate (`slot 76 ... 0x00000002` = displacement low bit).
- Cause:     (a) stmw/lmw save-mask registers are pinned ABI for the rho permutation (existing rule); (b) hexdiff's role-clean classification keys on register-field mapping and does not surface displacement-bit deltas on those rows.
- Fix:       decode the raw words directly (XOR both sides' instruction streams, mask GPR fields) to expose displacement/immediate deltas that hexdiff hides. In resetItemBox2RowPanes this revealed retail stores each all-shade s16 pair in DESCENDING element order (`{z,z}` pairs via explicit `arr[1]=z; arr[0]=z;`) and copies the four colour words to args[] in ASCENDING slot order — both now matched in source.
- Result:    resetItemBox2RowPanes stream fully order-matched; still witness-closed by the stmw pin (needs FULL_MATCH). getItemBoxEffect same.
- Confidence: repo_proven
- Applies to/a.k.a.: setupItemBoxSlotPanes/D8C0C/D8930/E9190/E9224/DF4E0/DF578/E3918/E4194 rotation family; any stmw-framed reg-swap target; hexdiff-vs-witness granularity gap.

## Derived dtor must chain through a base whose retail dtor is a forced-name free function: declare the base dtor undefined + exact_renames the UNDEF (CfObjectTbox fix, Wii/1.1 -O4,p)
- Symptom:   derived `~Derived()` hexdiff shows an extra `cmpi this,0 / beq` guard around a direct `bl __dt__<GrandBase>Fv` call; retail instead calls an intermediate base's dtor unconditionally via a flat retail name (`__dt__800BFA14`), size +8.
- Cause:     retail's C++ source had a user-defined virtual dtor on the intermediate base; the fork models that dtor as a forced-name free function (retail emits no mangled member symbol). With NO declared base dtor, MWCC skips the intermediate and synthesizes a guarded grandbase call — different shape AND different callee.
- Fix:       declare `~IntermediateBase();` in the header WITHOUT defining it anywhere; MWCC then synthesizes the standard unguarded `bl __dt__Q2..IntermediateBaseFv(this, 0)` in every derived dtor. Add `exact_renames=(("__dt__Q2..Fv", "__dt__<flat>"),)` to the DERIVED object's UnitRules so the UNDEF resolves to the existing retail-named definition at link. Do not restructure the TU that owns the free function.
- Result:    FULL_MATCH us-801faa18 (__dt__Q22cf12CfObjectTboxFv) 0x74/0x74.
- Confidence: repo_proven
- Applies to/a.k.a.: CfObjectObj/__dt__800BFA14 chain (CfObjectTbox); any hierarchy where retail merges ctor/dtor pairs under flat cf_ names.

## `obj->~T()` in a helper: compiler dtor-call emits `li r4,-1` flags arg + local mangled reloc; call the extern "C" retail __dt__ name directly (rebuildTboxPool fix, Wii/1.1 -O4,p)
- Symptom:   decomp has extra `li r4,-1` before the dtor call and the reloc targets the TU-local `__dt__23reslist<Q...>Fv` instead of retail's `__dt__800B183C`; size +4.
- Cause:     spelling the destructor through C++ (`obj->~reslist()`) makes MWCC generate its own flags-arg calling convention and bind to its local out-of-line instance; retail's source called the (retail-flat-named) destructor directly with no flags setup.
- Fix:       declare/lookup the retail linker name (`extern "C" void __dt__800B183C(void*);` — often already in the TU header) and call it as a normal function: `__dt__800B183C(obj);`.
- Result:    FULL_MATCH us-800b7bfc 0x44/0x44.
- Confidence: repo_proven
- Applies to/a.k.a.: reslist/container teardown helpers; same family as the forced-name dtor pattern above but for plain calls inside function bodies.

## @N exact_renames indices flicker across TU edits: prefer CONTENT-keyed pool_patterns for conversion-magic doubles (code_80135FDC u32->f32 fixes, Wii/1.1 -O4,p)
- Symptom:   a working `("@11349", "lbl_eu_806672F8")` rename stops matching after unrelated edits to the same TU; hexdiff reports a NEW index (`@11677`, then `@12644`) needing the same blob.
- Cause:     MWCC anon pool labels are ordinal (@N depends on total pool count), so ANY source change in the TU renumbers them — exact_renames keyed on @N rot silently.
- Fix:       key on bytes instead: `pool_patterns=((struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_<unsigned-blob>"), ((struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_<signed-blob>"),))`. Verify blob contents in split1.s first (0x43300000_00000000 = unsigned 2^52; ..._80000000 = signed correction). Keep old @N entries only if other sites still use them.
- Result:    FULL_MATCH us-8013a65c (BlendFloatAvgScale) and us-8013c6e8 (AnimJumpToLast).
- Confidence: repo_proven
- Applies to/a.k.a.: all int→float/double conversion magic drifts; completes MWCC_CASES §7i with a maintenance-proof recipe.

## NULL-result guard with shared tail: guard SPELLING is per-function — probe both forms (MakeIconTplName vs BuildItemNameStr, Wii/1.1 -O4,p)
- Symptom:   bdat-string getter family (`fp = getFP(...); if (fp) result = getBdat(...); sprintf(tail)`): one spelling mismatches badly, the other matches; which one differs PER FUNCTION even between siblings.
- Cause:     `T r = NULL; if (cond) {...}` (pre-init single guard) fixed MakeIconTplName 45.7%→84.4% (MWCC keeps the string-base lis/addi adjacent and emits one `li r3,0` for the NULL path), but the SAME rewrite on sibling BuildItemNameStr regressed 36.1%→3.1% — retail there uses branch-over-null-assign (`if (x == 0) { v = 0; } else {...}`) per guard.
- Fix:       when a NULL-guard getter plateaus, try converting every guard to pre-init + single positive `if`; if that regresses, revert — the retail source likely used explicit if/else null branches. Do NOT use ternary-with-comma (45.7%) or redundant-init if/else (50%). In MakeIconTplName `col` must be declared before `fp`.
- Result:    MakeIconTplName 84.4%; BuildItemNameStr stays on if/else form (36.1%, different residuals).
- Confidence: repo_proven (negative result documented)
- Applies to/a.k.a.: getBdatStringColumnValue wrapper getters with shared sprintf/format tails in code_80135FDC.

## pool_patterns cross-section theft: rename_pool_symbols matches ANY @N symbol against .sdata2 bytes at st_value=0 (kyoshin data-dissolve: CQuestWindow / CfGimmickObject / CMCCrystalBox / CMCCrystalList)
- Symptom:   data gate reports MATCH (strip empties both sides), but the hexdiff-postprocessed build object is semantically wrong: a switch jumptable's lis/addi relocs resolve to a FLOAT blob label (e.g. dispatch site -> lbl_eu_80667160), nm shows one jumptable UNDEF missing and no `U @N` leftover. Invisible to every gate; only reloc-level parsing catches it.
- Cause:     tools/postprocess_reloc_names.py rename_pool_symbols iterates ALL nm-visible @N symbols regardless of home section and reads match-content from .sdata2 AT THE SYMBOL'S st_value. A .data/.rodata table placed at offset 0 therefore "contains" sdata2[0:8] (e.g. the 2^52 double 43300000_00000000) and binds a float pattern before exact_renames can claim it. Tables at nonzero offsets escape by accident (value >= sdata2 size).
- Fix:       in units that mix pooled floats with a val-0 .data table, express float mappings section-aware: data_pool_patterns=((".sdata2", struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_<blob>"),) instead of pool_patterns. Keep one pattern per pool symbol (multi-symbol pools need each mapping listed explicitly - dropping one strands its lfd sites as dangling `U @N`). Table mappings stay content-keyed (bytes(0xNN)) or exact_renames.
- Result:    CQuestWindow/CfGimmickObject/CMCCrystalBox/CMCCrystalList rebuilt objects verified: every HA/LO site on jumptable/base labels, all SDA21 conversion-double loads on their lbl_eu_8066xxxx slots, zero leftover anonymous UNDEFs; all four units VERDICT: MATCH.
- Confidence: repo_proven
- Applies to/a.k.a.: any UNIT_RULES entry combining pool_patterns with file-backed tables whose symbols sit below sdata2 size; audit recipe = parse .rela.text HA/LO targets post-hexdiff and assert no float label receives a table-site reloc.
- UPDATE:    tools/postprocess_reloc_names.py rename_pool_symbols now enforces st_shndx == .sdata2 for candidates (upstream fix, 2026-08-26) - new rules can use pool_patterns for genuine .sdata2 pools without the theft hazard; keep section-aware data_pool_patterns for anything living outside .sdata2.
- Caveats:   rename_data_pool_symbols silently SKIPS ambiguous patterns (both .data tables all-zero => bytes(0xNN) prefix matches both), so coexisting zero tables need exact_renames with fresh @N. A table with NO retail counterpart (retail compiled the switch as compare chains) stays an anonymous UNDEF by design - document it in the rule ('KNOWN-DANGLING') so audits expect exactly that one.

---

## ScheduleList (monolib/src/core) — direct calls where retail uses virtual dispatch through dissolved vtables → abstract pure-virtual interface cast (Wii/1.1, -O4,p, 10x FULL_MATCH)
- Symptom:   hexdiff shows `bl <direct callee>` + decomp-side-only R_PPC_REL24 where retail has `lwz r12,N(r3); lwz r12,K(r12); mtspr ctr; bctrl` (virtual call); forwarders also inline their callee body (decomp .text >> retail). After switching to function-pointer-table loads (`((fn)((void**)obj->mSelfVtbl)[i])(args)`), residual = vtable loads scheduled BEFORE argument setup (scratch reg computed from pre-copy pointer) — volatile pointee does NOT pin them.
- Cause:     the scaffold expressed virtual entry points as plain extern "C" functions; retail reaches them via each object's own vtable (SLList vptr at 0x00, CSchedule vptr at 0xEC `mTablePtr`). Function-pointer expressions are scheduler-independent values, so MWCC hoists them; only a genuine virtual call forces the load-after-arg-setup ordering.
- Fix:       declare local abstract interface types mirroring the retail table and call through a cast — `struct SLDispatch { virtual void destroy(int)=0; virtual void step(f32)=0; ... }; ((SLDispatch*)list)->step(x);`. For a vptr at nonzero offset, derive from an opaque primary base sized to the offset (`struct B { u8 opaque[0xec]; }; struct D : B { virtual void destroy(int)=0; };`). Nothing is constructed/deleted in the TU → MWCC emits NO vtables/typeinfo. Slot map: MWCC's vptr points AT the RTTI cell, so the FIRST declared virtual lands at +0x08 (do NOT add leading dummy slots). CSchedule deleting-dtor slot = first virtual of the 0xEC-offset interface.
- Result:    all 10 demoted targets FULL_MATCH (100% static), sizes aligned exactly.
- Confidence: repo_proven
- Applies to/a.k.a.: any TU whose object layouts keep raw `(u32)&fn` vtable arrays (dissolved-data monolib splices); pairs with the SchedFlagBits bitfield recipe below.

## u16 bitfield overlay for single-bit flag tests — record-form rlwinm extraction (ScheduleList SchedList_StepAll, Wii/1.1, FULL_MATCH)
- Symptom:   retail tests schedule flags with record-form rotates `rlwinm. r0,r4,22,31,31` (+beq) and `rlwinm. r0,r4,17,31,31` (+bne); mask form `rlwinm rx,ry,0,22,22`, shift forms `(x>>9)&1` (SH=23), and `(x<<22)<0` (MB=0..1 + srawi) all mismatch.
- Cause:     retail source read packed bitfields, not masks: MWCC compiles a 1-bit unsigned bitfield read as `lhz` + record-form rotate extracting that single bit to bit31 (SH = position_of_bit − preceding_bits_in_unit − 1... empirically for a u16-unit bitfield struct: extracted LSB-position p = 14 − bits_declared_before_member).
- Fix:       overlay `struct SchedFlagBits { u16 clearOnMatch : 1; u16 midPad : 4; u16 destroy : 1; };` on the s16 flags word (`((SchedFlagBits*)&item->field_0x00)->destroy`) — clearOnMatch extracts at bit 14 (SH=17), destroy at bit 9 (SH=22), matching retail exactly. Members MUST be unsigned (u16): s16 members emit `lha` + srawi sign-extraction chains instead.
- Result:    SchedList_StepAll 100% static (was 45.2%).
- Confidence: repo_proven (positions verified empirically on Wii/1.1; recalibrate the p = 14 − preceding rule per compiler with a .scratch probe before reuse)
- Applies to/a.k.a.: any lhz-loaded packed flag word tested one bit at a time; complements MWCC_PATTERNS "Mask↔rlwinm encoding".

## Float-compare bool return: mfcr-materialization vs branch-form is controlled by shared-exit structure (UnkClass_8047E110::func_8047EEB0 fix, Wii/1.1 -O4,p)
- Symptom:   `if (thr >= diff) return true; return false;` inside both arms of an if/else lowered to `fcmpu; cror cr0[eq],cr0[gt],cr0[eq]; mfcr r3; rlwinm r3,r3,N,31,31; blr` (value-form) while retail uses `fcmpu; cror; bc <shared fail block>` with inline `li r3,1; blr` per path.
- Cause:     MWCC canonicalizes adjacent if/return pairs into a computed bool. Retail's branch form appears only when each arm's failure path exits through ONE SHARED label (tail-merged `li r3,0` block), which blocks the canonicalization.
- Fix:       restructure to `if (!(thr >= diff)) goto fail; return true;` in each arm + single `fail: return false;`. Result: mfcr eliminated, size matched exactly (0x10c/0x10c). Note: rewriting the TAIL flag-gate (`return (f8 & 4) == 0;`) into branch form regresses — leave expression-form returns alone when retail has them.
- Result:    26.9% -> 29.9% static, cycle fuzzy 80.3%; witness now gated only on branch-displacement drift from remaining GPR colors.
- Confidence: repo_proven
- Applies to/a.k.a.: any leaf bool function with two+ float threshold tests converging on shared pass/fail exits.

## Volatile FPR birth order for named float constants follows declaration order — declare in the order retail colors them (UnkClass_8047E110::func_8047F658 fix, Wii/1.1 -O4,p)
- Symptom:   init block loading three SDA constants (flat/depth/half) colored f1/f2/f0 in decomp vs f2/f1/f0 in retail; ~20 pure reg-swaps.
- Cause:     volatile FPRs claim low->high by virtual birth order = first-declaration order among the named const locals.
- Fix:       reorder ONLY the declarations so the constant retail colors lowest (f1) is declared first. Do NOT also move the first-use statement or introduce named pointer temporaries for struct copies — `*out = arr[sel]`, named-src-pointer, and explicit dst[i]=src[sel*3+i] all canonicalize to identical code (negative result x3), so copy-shape steering is not source-reachable.
- Result:    51.9% -> 55.6% static, cycle fuzzy 88.0% -> 89.1%.
- Confidence: repo_proven
- Applies to/a.k.a.: extends docs/register_mapping.md Rule A to volatile-FPR constants; pairs with the existing fcmpu symmetric-operand entry.

## Named `sum = lhs + rhs` with an earlier FPR still live in f0 pins fadds dest and operand order (dispatchPassiveSkillInput, Wii/1.1 -O4,p)
- Symptom:   `acc += addend` matches dest/colors (`fadds f1,f1,f2`) but retail wants `fadds f1,f2,f1`. Writing `acc = addend + acc` or a same-scope `next = lhs + rhs` after `lhs,rhs,occ` decl recolors all three FPRs (~96.9%).
- Cause:     FPR scratch is low→high by declaration order. `f32 occ, acc, addend` pins f0/f1/f2. `acc += addend` already has dest==FRA so MWCC will not emit FRA=addend. A *new* named sum with `occ` still live occupies f0, so dest reuses `acc` (f1) while source order `addend + acc` is preserved as FRA/FRB.
- Fix:       Declare the occupier first (`cap,step,cur` → f0,f1,f2), assign in retail load order, then `f32 sum = cur + step` (use `sum` for the store and the compare so `cap` stays live across the add).
- Result:    FULL_MATCH on us-80265ea4
- Confidence: repo_proven
- Applies to/a.k.a.: commutative fadds/fmuls last-1% operand-order swaps; MWCC_CASES dispatchPassiveSkillInput; pairs with monolib const-first fadds notes

## Compound `|=` on a tail store can hoist LR restore above CSRs (SuddenCommuMarkCommuVoices, Wii/1.1 -O4,p)
- Symptom:   Pure epilogue `lwz` order swap: decomp `lwz r0,LR` then r31/r30; retail restores CSRs first, LR last. Body is otherwise identical.
- Cause:     `field |= bit` is a read-modify-write. MWCC treats the RMW as extending the last-use of LR setup so `lwz r0,xx(sp)` is scheduled first.
- Fix:       Split to `u32 t = field; t |= bit; *(u32*)&field = t;` (plain stw). Same TU already used this for ctor/reset epilogues (volatile last store does the opposite — hoists LR earlier).
- Result:    FULL_MATCH on us-801bdd6c
- Confidence: repo_proven
- Applies to/a.k.a.: last-instruction `|=` / `+=` on a member before blr; CSuddenCommu ctor comments; epilogue-only 3-swap residuals

## rlwinm rotate-mask decode recipe for bitmap-index mismatches (UnkClass_8047E110::func_8047EFBC fix, Wii/1.1 -O4,p)
- Symptom:   `bitmap[i >> N]` structural mismatch where decomp emits one rlwinm encoding and retail another (e.g. sh28/mb20/me29 vs sh29/mb19/me29).
- Cause:     mask endpoints encode the effective word index: convert via lsb math — kept_lsb_range = [32-1-ME .. 32-1-MB], orig_bit(pos) = (pos - SH) mod 32; result = byte offset of `(x >> s)*4` etc.
- Fix:       decode BOTH sides before touching source. Here retail sh29,mb3,me30 = u32 word index `nEnt>>5`; source had `nEnt>>6` (a real semantic bug invisible without decoding).
- Result:    index instruction matched exactly after source fix.
- Confidence: repo_proven
- Applies to/a.k.a.: any psq/bitmap/stride lookup where hexdiff shows two different rlwinm immediate fields; also proves negative results for `/8`, `/4>>1` refolding (MWCC canonicalizes back to me31).

## CTR countdown loops with a dead counter decrement alongside bdnz are a repo-wide wall (func_80480140 / MPV_DecodeFrmSj / func_8047FE48, Wii/1.1 -O4,p)
- Symptom:   retail inner loop = `lhzx cnt; mtspr ctr; cmpi; ble exit; body; addi ptr; bdnz` with NO counter decrement anywhere; every C shape (u16 do-while, s32 do-while, s32 for-with-pretest) emits mtctr+bdnz PLUS a dead `subi/addic.` counter update, or drops CTR entirely.
- Cause:     unknown internal trip-count handling; not reachable from high-level source (confirmed across three TUs and multiple shapes).
- Fix:       none yet — recorded as open wall; do not burn attempts re-probing basic shapes.
- Result:    documented near-misses (11.8%, 27.6%, 32.9%).
- Confidence: repo_proven (negative result)
- Applies to/a.k.a.: MWCC_CASES "CTR-vs-addic" entries; scan-neighbour loops with break-on-match bodies.

## `%` by non-power-of-two constant: divw-vs-magic-multiply selection (negative result)

- Symptom:   retail emits `li rN,K; divw; mullw; subf` for `%K` (K=5/10); decomp always emits magic-multiply (`lis/addi 0x6667; mulhw; srawi; mulli K; subf`)
- Cause:     retail's divisor was NOT a compile-time constant when lowering ran (likely an inlined-late helper parameter under `-ipa file` without value-prop); every constant source shape folds to the multiply idiom
- Fix:       none found at high level. Probed Wii/1.1 -O4,p: cast/local-signedness/opaque-param/volatile-reload all give magic-multiply for `%`; `x % noinline_ident(K)` gives `li+divw` but inserts a call retail lacks; branch-dependent divisor gives divw plus branch code; `/K` alone gives mulhw magic
- Result:    negative_result (MakeCrystal_PlaceCrystals %5, MakeCrystal_NopD3E4 %10 sites remain mismatched)
- Confidence: negative_result
- Applies to/a.k.a.: rand31()%N call sites; if a future engine change models IPA value-prop boundaries this class closes

## Address-taken local store before call-arg setup is a Wii/1.1-vs-GC schedule split
- Symptom:   retail emits `stw rX, N(sp)` BEFORE the `addi r4, sp, N` / `li r5` argument setup of the consuming call; GC/3.0a5.2 always sinks the store after the arg-setup `li`s. Seen in gate idioms like `u32 local = *obj->vf30(); testActorStatus(obj, &local, 0x807)`.
- Cause:     final-scheduling tie-break differs by compiler version, not source shape. Probes (.scratch/ctrlact_stwsink_probe.c: decl-at-init, separate assign, volatile, copy-through-temp, dead-local variants) all sink under GC/3.0a5.2 (-lang c and c++); Wii/1.1 hoists the store above the arg setup in every variant.
- Fix:       compile the TU with mw_version="Wii/1.1" when this pattern dominates; verify no other function regresses (some GC-specific schedules like stfs-between-stb-pairs flip the other way — check net unit --all before keeping).
- Result:    kyoshin/cf/CtrlAct flipped a5.2->Wii/1.1: 9->12 FULL_MATCH, ctrlActHandleCase4Move/ctrlActState4Handler/CAttackParam_UnkVirtualFunc4 100%, ctrlActDispatchEntry 98.5% pure-regswap.
- Confidence: repo_proven
- Applies to/a.k.a.: address-taken locals passed to calls; gate idioms; store sinking; mw_version selection

## Untyped vtable-slot calls load the vptr into a scratch reg; typed virtual member calls use r12 (CFloorMap FloorMap_SetPaneFlag/FloorMap_Draw fixes, Wii/1.1 -O4,p)
- Symptom:   retail `lwz r12,0(r3); lwz r12,60(r12); mtspr 19,r12` vs decomp `lwz r6,0(r3); lwz r12,60(r6)` — vptr lands in a low scratch reg instead of r12.
- Cause:     MWCC only emits its canonical virtual-dispatch sequence (r12 idiom) when the call is spelled as a real C++ virtual member call on a declared polymorphic type. A hand-rolled `(*(void(**)(...))vtbl[i])(obj, ...)` or `VFuncPtr* vt = *(VFuncPtr**)obj; vt[15](obj,...)` shape compiles as an indirect function-pointer call with different coloring.
- Fix:       identify the real owning class and spell it as a virtual call: cast obj to the class pointer and invoke the declared virtual (`((nw4r::lyt::Pane*)obj)->FindPaneByName(name, 1)`, `accessor->GetResource(type, name, NULL)`). Slot index must match the class's declared virtual order.
- Result:    FloorMap_SetPaneFlag 88.9% -> 100% FULL_MATCH; FloorMap_Draw removed its last structural diffs.
- Confidence: repo_proven
- Applies to/a.k.a.: FindPaneByName-family, ArcResourceAccessor::GetResource, any slot-N hand dispatch

## Word-wise VEC3 copy: ((u32*)&dst)[i] = ((u32*)&src)[i] keeps integer-register copies into inlined SetTranslate (CFloorMap FloorMap_SetCursorState + FloorMap_CursorPageDown fix, Wii/1.1 -O4,p)
- Symptom:   retail copies pos.y/z via lwz/stw word moves; decomp forwards floats (lfs/fmuls/stfs) straight from src into the store.
- Cause:     a struct-assign (`dest = pos;`) lets MWCC forward the float values directly into the inlined SetTranslate argument stores.
- Fix:       copy the three words through u32 pointer casts before the call (three statements). Second independent confirmation of the existing CMiniMap/Cursor idiom.
- Result:    FloorMap_CursorPageDown 64.6% -> 92.9% (all 23 structural gone); FloorMap_SetCursorState previously matched.
- Confidence: repo_proven
- Applies to/a.k.a.: VEC3 copy before SetTranslate; pane position refresh blocks

## Embedded widget members are passed by ADDRESS (&p->field), not by loaded value (CFloorMap FloorMap_Draw fix, Wii/1.1 -O4,p)
- Symptom:   retail `addi r3, r30, 0xB8` (address-of member) vs decomp `lwz r3, 0xB8(r30)` (load).
- Cause:     source dereferenced a scalar-typed member view where retail passes the sub-object base pointer to widget helpers (CSysWin/CScrollBar/CCur helpers take the widget object pointer).
- Fix:       pass `&p->member` (or declare the member as an array so it decays). Also flips arg-eval order to match retail's `or r4` (arg2) before `addi r3` (this).
- Result:    FloorMap_Draw 89.8% -> 98.8%, 11 structural -> 0.
- Confidence: repo_proven
- Applies to/a.k.a.: CFloorMapFull::mSysWinB8/F4/mScrollBar/mCursor family; any struct-view mismatch between scalar field and embedded object

## Parenthetical prose inside a symbols.txt comment is parsed as a symbol attribute — dtk fails TU-wide (infra, config/us/symbols.txt)
- Symptom:   `dtk dol split: Unknown symbol attribute '(vtable'` — every hexdiff/ninja build fails at the SPLIT step, not just the edited TU.
- Cause:     dtk reads attributes out of the trailing comment; free-form text like `(vtable referenced by X, addend +2)` breaks the parser. A second hazard: adding an odd-address symbol entry that overlaps the preceding entry's declared size trips "Unaligned symbol entry" / overlap failures.
- Fix:       keep comments to `type:/size:` tokens only (move notes to their own leading `//` line or drop them); never add entries whose [addr, addr+size) intersects a neighbor — check both neighbors' sizes when inserting mid-run symbols.
- Result:    unblocked all-TU builds after repair.
- Confidence: repo_proven
- Applies to/a.k.a.: symbols.txt maintenance, data-label insertion, cross-agent concurrent edits

## Dead direct-indexed array stores scatter across the stack; walk the array through a pointer to keep it contiguous (CfCamEvent_1 sinit_8007BE74 fix, GC/3.0a5.2 -O4,p)
- Symptom:   static-init writing a dead local array element-wise (`local[23].x = c;` front-to-back) compiles to stfs with unique, wildly scattered displacements (~292-byte strides, 20 KB frame) vs retail's tight contiguous block.
- Cause:     MWCC splits each dead scalar store of a non-address-taken local array into its own stack "string" and never coalesces them. Verified with isolated probes: plain POD struct and union views behave identically.
- Fix:       access through a pointer: `P* p = &local[23]; p->x = ...; p = &local[22]; ...`. Address-taken arrays stay contiguous; frame and all displacements then match retail exactly.
- Result:    sinit_8007BE74 54% -> 91.3% (frame -304 matches, all stack stores byte-match).
- Confidence: repo_proven (probe-reproduced in .scratch/sinit_probe*.cpp)
- Applies to/a.k.a.: any static-init or bulk-dead-store pattern over local struct arrays

## Uncalled functions get ABI-violating parameter registers — check caller count before chasing FP color diffs (CfCamEvent_1 CamEvtDeadStp finding, GC/3.0a5.2 -O4,p)
- Symptom:   retail consumes an incoming float param from f0 instead of f1; every source shape keeps it in f1; the equivalence witness rejects the rename with "abi-boundary | rho perm maps fpr f0 -> f1; ABI registers must be fixed".
- Cause:     the function has ZERO callers anywhere in linked retail — IPA freed the ABI constraint and the parameter coloring placed it arbitrarily. Not expressible in C++.
- Fix:       none. Confirm by grepping all asm for `bl <symbol>`; if no callers exist, record as permanent near-miss immediately.
- Result:    CamEvtDeadStp documented permanent 60% near-miss (body otherwise identical).
- Confidence: repo_proven
- Applies to/a.k.a.: any never-called helper with param-register color residuals; saves hours of declaration-order experiments

## Negative sentinel stores to u8 fields: assign through a signed lvalue to get retail's `li rN,-N` (CItemBoxGrid GridMoveCursorRight, Wii/1.1 -O4,p + optimize_for_size)
- Symptom:   `p[0x525] = -2;` / `(u8)-2` both emit `li r0,254` where retail has `li r0,-2` (0x3800FFFE vs 0x380000FE); same for -1/255
- Cause:     storing through a `u8*` lvalue makes MWCC canonicalize the constant to the unsigned domain before materializing; retail's source evidently used a signed char lvalue so the wraparound kept the negative form
- Fix:       store through a signed view of the field: `*(s8*)&p[0x525] = -2;` — MWCC then emits the negative `li`. Plain casts and index arithmetic (`lbl+2`-style) do NOT fold; the lvalue type is the lever
- Result:    GridMoveCursorRight 88.4% -> 89.2% (reg_swap 11 -> 9, both li mismatches cleared)
- Confidence: repo_proven

## `const T*` pointer params let `-str reuse,readonly` CSE away retail's reloads — drop const when retail re-reads (regSeenListCb/setSeenBitLst fix, Wii/1.1 -O4,p)
- Symptom:   loop reads `list[i]` early and again later; decomp emits ONE lhz keeping the value in a register across intervening stores, retail reloads with a second lhz. Structural + color diffs throughout the loop.
- Cause:     with a `const u16*` param, MWCC's readonly-string/reuse path treats the pointee as invariant across the bit-array stores (different type/qual) and CSEs the reloads; with plain `u16*` it must assume aliasing and reload.
- Fix:       change the parameter to non-const (`u16* list`) in both prototype and definition. regSeenListCb went 21.3%→76.6% (0 structural), setSeenBitLst 10%→40% with size now exact.
- Result:    repo_proven on both targets
- Confidence: repo_proven
- Applies to/a.k.a.: any walked-pointer loop that re-reads through the pointer after stores through another global/array; check for missing second `lhz` before touching declaration order.

## Data-gate diagnosis: measure addends/offsets ONLY on a fresh bake — the data-diff temp-copy path applies UNIT_RULES a second time (CGXCache/CERand, Wii/1.1 -O4,p -ipa file)
- Symptom:   jumptable `addend_patches` deltas that "should" fix .data reloc drift keep oscillating (+4/+8/-4 across consecutive verifications) even with unchanged source; patched values overshoot or undershoot retail by exactly one rule application.
- Cause:     `run.py data diff` compares the RAW shared object first, and only on FAIL re-applies UNIT_RULES to a TEMP COPY — but hexdiff/run.py builds already baked the rules into `build/us/src/**/*.o` in place. Diagnosing against an already-baked object therefore counts each delta TWICE (bake + temp-copy), so back-computing "raw" from baked values yields wrong signs/magnitudes. Marker experiment (set `.data@12 += 100`, rebuild, read): bake applies exactly once.
- Fix:       to derive drift deltas: (1) rebuild the TU via `hexdiff <unit> --all` (fresh ninja + single bake with the CURRENT rule), (2) read the baked object's reloc values directly and subtract only the CURRENT rule's deltas — or temporarily set the candidate rule, rebuild, and compare baked-vs-retail, iterating until baked == retail exactly; (3) never diagnose from an object another session may have baked with older rules. For units under concurrent rewrite, re-derive at verification time and say so in the rule comment.
- Result:    CGXCache jumptable (14 addends) restored to exact retail in one iteration after three mis-derived rounds; same procedure settled CERand's tail-drop sizing.
- Confidence: repo_proven
- Applies to/a.k.a.: any UNIT_RULES delta field (`addend_patches`, `patch_reloc_addend`, `move_reloc_offsets`) on a unit whose shared build object is also written by hexdiff/cycle; pairs with the CGXCache recipe in MWCC_CASES §jumptable.

## Hand-building vtables whose slots reference own-class members trips MWCC 10322 — zero the slot, inject_relocs the reloc (CNBanner, Wii/1.1 -O4,p -ipa file -sdata 0)
- Symptom:   `extern "C" void __dt__8CNBannerFv();` (or the OnFileEvent mangled name, in ANY form incl. `extern char x[]`) inside the TU that DECLARES the class member → `(10322) illegal name overloading`, error attributed to a later unrelated extern "C" block.
- Cause:     under `-ipa file` MWCC rejects source-level declarations whose spelling equals a compiler-generated symbol of a class defined in the same TU. Foreign-class mangled names (WorkEvent1__10IWorkEvent*, other TUs' __RTTI__) are fine.
- Fix:       spell those vtable words as literal 0 and let UNIT_RULES `inject_relocs=((section, byte_offset, "symbol"), ...)` append the R_PPC_ADDR32 entries post-build (slot bytes stay 0 as link placeholders — matches the CChildListNode precedent). Also: under `-sdata 0`, small aggregates land in .data/.bss regardless of size — force placement with `__declspec(section ".sdata"/".sbss")`; and a `u8 flag; u8 pad[7];` pair gets the pad 4-aligned by MWCC (pushing later symbols +4) — use one sized symbol (`u8 flag[8]`) when retail sizes it as a block.
- Result:    CNBanner all data sections MATCH raw (.rodata 0x23 / .data 0xB4 / .sdata 0x10 / .sbss 0x4).
- Confidence: repo_proven
- Applies to/a.k.a.: hand-built vtables for non-novtable classes; any TU compiled with `-sdata 0` needing .sdata/.sbss placement.

## pad_data_section_func ELF rebuild: carry the section-header table + use a real write cursor (monolib data-gate regression sweep, Wii/1.1)
- Symptom:   every UNIT_RULES entry using `pad_data_section` failed at once (13 monolib units); direct invocation raised `struct.error: unpack_from requires a buffer of ...` past EOF, or silently produced objects whose post-.data section headers pointed mid-content (garbage sizes in the data gate).
- Cause:     two stacked bugs in the tail rebuild: (1) the section-header table was never copied into the rebuilt body while the fixup loop still read it at the OLD e_shoff; (2) per-section new offsets were derived arithmetically (`old_off + delta`) while bytes were appended by a separate cursor, so any pre-existing inter-section slack desynced recorded vs actual layout.
- Fix:       rebuild the tail with an explicit cursor preserving each section's sh_addralign residue (`new_off = len(body) + ((old_off % align) - len(body)) % align`), then append the old shdr table verbatim at `new_e_shoff = (len(body)+3) & ~3`, patch e_shoff@32, and update moved sections' sh_offset. See pad_data_section_func.
- Result:    CProc/CView/etc. pad users back to MATCH; verified on CProc.o (.data 0xC4→0xC8 with 8-aligned later sections).
- Confidence: repo_proven
- Applies to/a.k.a.: any rule touching file-layout surgery (pad/drop/move); if a gate regressed across MANY units simultaneously, suspect shared tooling first and call the rule function directly on a scratch copy to surface hidden exceptions.

## Order-only .data/.rodata drift: remap .rela r_offsets instead of moving bytes (CView/CNBanner, Wii/1.1 -O4,p -ipa file)
- Symptom:   data gate reports ".data: N bytes identical" AND "reloc drift" in the same run — MWCC emitted the same content words in a different block order than retail.
- Cause:     all content words are link-time relocs or zero, so byte comparison passes regardless of order; only the reloc positions/names differ.
- Fix:       (a) same-section permutation with equal-size blocks → swap_data_blocks (symbols+relocs ride along); (b) arbitrary reordering where bytes stay zero → new `data_reloc_offset_moves` field ((section, old_off, new_off), ...) applied before retarget_relocs, then retarget_relocs/rename for names; (c) rodata string pools → block swaps home each string (pure bytes), zero_data_range only where goal==0, drop_data_tail to retail size.
- Result:    CView (.data 0x120 / .rodata 0x70 / .sdata 0x28) and CNBanner (.data 0xB4 / .sdata 0x10 / .sbss 0x4) MATCH.
- Confidence: repo_proven
- Applies to/a.k.a.: `data_reloc_offset_moves` is NOT idempotent and hexdiff re-bakes rules into build/us/src on EVERY run (even ninja no-op) — always touch the cpp / force a fresh build before evaluating, and evaluate via `run.py data diff --no-postprocess` (raw-first path).

## Retail-owned .sbss word in a TU compiled with -sdata 0: flip the unit to -sdata 8 (CNBanner lbl_eu_80665A98)
- Symptom:   tentative/zero-init 4-byte global lands in .bss (or is GC'd entirely under -ipa when unreferenced); retail split owns it as .sbss 0x4. `__declspec(section ".sbss")` rejected (33044: uninitialized-section name).
- Cause:     -sdata 0 disables small-data placement; DECOMP_FORCEACTIVE-style references keep the symbol alive but cannot choose the section.
- Fix:       set the unit's extra_cflags to ["-sdata 8"] and keep a FORCEACTIVE stub (drop_text_symbols removes it). Verify text safety first: diff raw -sdata 0 vs -sdata 8 .text — only the stub's own words may differ (true for TUs whose matched functions never touch small data).
- Result:    CNBanner .sbss 0x4 MATCH; all 9 functions still byte-exact.
- Confidence: repo_proven
- Applies to/a.k.a.: any monolib TU with `-sdata 0` whose retail split carries a small .sbss object; pairs with the inject_relocs recipe above.

## Cross-header extern "C" conflicts surface only when a new include leaks into an existing chain (infra, CfCamEvent_1 TU blocked by CTaskGameEff.hpp edit)
- Symptom:   TU that compiled cleanly for days suddenly fails with `(10505) illegal overloading` / `(10563) identifier redeclared` / `(10197) illegal function overloading` in headers it includes — but none of those headers were touched by this agent.
- Cause:     another agent's edit to a transitively-included header (e.g., adding `#include "CfObjectImplMove.hpp"` to CTaskGameEff.hpp) introduces a second, divergent extern "C" declaration of a symbol your hpp also declares. MWCC errors lazily at the first USE site, not at the declaration.
- Diagnosis: run the compiler directly with `-maxerrors 20` (not via hexdiff/ninja) to get the full error list; trace which newly-visible header brings each conflicting decl; grep all declarations of each name across include/ and src/.
- Fix:       per conflict, (1) align return types to the canonical DEFINITION (grep ocBdat.cpp etc.), (2) make const-ness consistent (sdata2 constants are const), (3) remove divergent prototypes entirely and cast through local function-pointer typedefs at call sites (see interface-cast pattern above). Verify zero blast radius: grep all other includers of the changed header for uses of the changed names.
- Result:    CfCamEvent_1 TU unblocked; 16/39 FULL_MATCH preserved; zero regressions.
- Confidence: repo_proven
- Applies to/a.k.a.: multi-agent concurrent edits; shared bdat/math/collision headers; any TU whose build breaks without its own sources changing

## Direct extern "C" calls vs static inline wrappers: bare-symbol call sites need the direct form (CamEvtPickWeightedRow fix, GC/3.0a5.2 -O4,p)
- Symptom:   reloc drift shows mangled callee (`getCol8__FPvPCci`, `CamEvtMatchTypeFlag__Fii`) where retail has bare names (`getBdatStringColumnValue`, `CamEvtMatchTypeFlag`); plus extra instructions from the wrapper layer.
- Cause:     `static inline` helper wrappers around extern "C" functions emit their own frame + mangled call when MWCC declines to inline them under -ipa file.
- Fix:       replace wrapper calls with direct `(u8)extern_c_function(...)` calls; declare the callees' definitions as `extern "C"` if retail uses bare symbols. Note: extern "C" linkage change requires re-verification of the changed function's own match status.
- Result:    CamEvtPickWeightedRow 17.2% -> 31.1%; all callee reloc names now match retail.
- Confidence: repo_proven
- Applies to/a.k.a.: getCol8/getCol16-style column-read helpers; any static inline wrapping an extern "C" API

## Sub-100 instruction_match on byte-identical functions = objdiff unlinked-object scoring artifact (midband batch 01 finding, Wii/1.1)
- Symptom:   registry row labelled FULL_MATCH with persisted instruction_match stuck below 100 (tiny functions across different units all reporting the identical 97.5 floor; larger ones 98.x-99.9x), while `hexdiff --json` shows mismatch_count=0, reg_swap=0, structural=0, reloc_drift=[] and `cycle` issues a full-match equivalence certificate.
- Cause:     objdiff scores raw instruction bytes on UNLINKED .o pairs; REL24/SDA21 displacement fields that only resolve at DOL link cannot match, and tiny functions get diluted by section-level weighting. Not reachable by any high-level C++ change.
- Fix:       none required — certify via hexdiff --json (0/0/0, drift empty, size ok) plus the cycle full-match certificate; the registry `status: FULL_MATCH` is the acceptance authority. Do NOT chase the percentage with source churn (each speculative edit risks regressing real matches).
- Result:    19/63 midband rows closed as FULL_MATCH with documented artifact residual; 44 rows measure a true 100.0 after a fresh cycle refresh.
- Confidence: repo_proven
- Applies to/a.k.a.: any batch whose mandate reads "instruction_match in (0,100) is a real residual" — triage with hexdiff FIRST; most such rows are stale or artifact, not real residuals.

## #pragma force_active does NOT stop -ipa file compile-time dead-stripping (g3d dcc trio, GC/3.0a5.2 -ipa file)
- Symptom:   anon-namespace (internal-linkage) functions referenced only by retail data-blob tables are stripped at COMPILE time ("not written yet" in hexdiff) even when wrapped in `#pragma force_active on ... off`.
- Cause:     force_active only sets the linker no-deadstrip symbol attribute; the `-ipa file` pass runs earlier in the compiler and strips unreferenced internal-linkage entities regardless. Same failure mode as `__attribute__((used))` (this MWCC ignores it too).
- Fix:       best fix: define the referencing data in-source. If the references are dispatch TABLES owned by a blob, define them in the TU with exact retail names + contents (real initializer references anchor the functions), then UNIT_RULES `extern_data_sections` strips the local copies and converts the symbols to UNDEF so they resolve to the blob at link (g3d dcc trio, 2026-08 -- DECOMP_FORCEACTIVE fully retired there). Fallback when the data cannot move in-source: keep the DECOMP_FORCEACTIVE emitter and drop it from the linked object with UNIT_RULES `drop_text_symbols` using the TRAILING-* PREFIX form ("FORCEACTIVE<unit>_cpp*") -- stub symbols embed a __LINE__ that drifts with every edit above the macro, and an exact name silently disarms the drop.
- Result:    g3d_maya/xsi/3dsmax: stubs kept, drops applied; split overflow +0xFC/+0x140/+0x100 -> +0x50/+0x4/+0x68 (residuals are genuine code-size overshoot of unmatched functions — verify tails are real epilogue code with a byte dump before ever considering patch_symbol_sizes).
- Confidence: repo_proven
- Applies to/a.k.a.: any TU whose internal-linkage functions are reachable only from data.s tables; pairs with the drop_text_symbols/repack_after_drop recipes.
## Retire DECOMP_FORCEACTIVE emitters by restoring the real GC'd functions they were faking (dvd_broadway.c, Wii/1.1 -O4,p)
- Symptom:   a unit carries `DECOMP_FORCEACTIVE` emitters (retail-absent variadic fake functions) held alive only to (a) pool string literals at retail .data offsets, (b) pin .bss first-reference order / keep unused .bss symbols alive; each emitter costs retail-absent .text until dropped and reads as fake in source.
- Cause:     the retail SOURCE defined real functions there that the retail linker GC'd from the image; the literals/.bss pins they produced while compiling are still visible in the DOL-extracted retail .o. The emitters approximated their compile-time side effects.
- Fix:       restore the REAL functions at their retail source positions (bodies templated from surviving siblings; only their literal-emission order matters since the text is dropped) and list them in `drop_text_symbols` exactly like the emitters were. Three concrete levers: (1) string-literal pooling — a restored function pools its own __FUNCTION__ + OSReport literals in first-use order at its position; shared literals (`"@@@ (...) IOS_IoctlvAsync..."`) reuse earlier pools via `-str reuse`, adding no .data. (2) .bss symbol liveness — a reference inside any restored function keeps the variable in .bss at its first-reference slot. (3) .bss FIRST-REFERENCE pin without a fake — make an early real helper non-static: static+single-call helpers are fully inlined away and pin nothing, but a non-static definition forces an outline copy at its early source position whose references pin the order (drop that outline copy via drop_text_symbols). After larger drops, `repack_after_drop=<func_align>` re-lays survivors so split-size still fits.
- Result:    dvd_broadway.c: both emitters removed, 3 fake UnusedStr byte arrays deleted, 32/32 hexdiff green, split exact fit 0x26B0, .bss/.sbss layouts identical to retail; pre-existing section-symbol reloc-name drift unchanged.
- Confidence: repo_proven
- Applies to/a.k.a.: any FORCEACTIVE kept only for data-layout side effects; pairs with drop_text_symbols/repack_after_drop; KB cross-ref "WPAD bss order = first-reference order; only a real .text reference pins it".

## extern "C" prototyped calls with >=6 lvalue args fail type-check (10248) — cast one arg to an integer rvalue (monolib/src/scn code_804BD8E8, Wii/1.1 -O4,p)
- Symptom:   `(10248) function call 'f(void *, {lval} const void *, ...)' does not match 'f(void *, const void *, ...)'` on a call whose argument types exactly match the extern "C" prototype. Reproduces in a 6-line scratch TU; fires at 6 lvalue args (5 lvalues + 1 literal is fine).
- Cause:     MWCC Wii/1.1 C++ front-end bug in the overload/candidate check when every argument is an lvalue and the candidate list has one extern "C" function. Casts to POINTER types still count as lvalues (`(const void*)a` fails); only integer-cast/literal rvalues clear it.
- Fix:       cast one argument to its integer type at the callsite, e.g. pass `(u32)a3` for a `u32` param. Unprototyped `extern "C" void f();` decls are NOT an escape — `-lang=c++` treats them as zero-arg strict.
- Result:    ScnRes_SegQueryForward_E348/ScnRes_VertRayForward_E398 wrappers compile and match 100%.
- Confidence: repo_proven
- Applies to/a.k.a.: any TU forwarding many word args through extern "C" cross-TU entry points.

## Countdown loops: only the `for (; n > 0; --n)` spelling gets mtctr/bdnz; do-while(--n) emits addic./bc (code_804BD8E8 ScnRes_AnyEntryValued_E5C8 fix, Wii/1.1 -O4,p)
- Symptom:   retail scan loop is `mtctr rN; cmpi; ble .Lskip; body; addi; bdnz`; decomp do-while with `while (--count)` lowers to `addic. rN,rN,-1; bc 4,2` (+2 instructions).
- Cause:     MWCC's CTR-form recognition keys on the canonical counted-for shape (bound tested >0 before entry, decrement in the for-expression). A do-while with an explicit pre-guard keeps the counter in a GPR.
- Fix:       write `for (; count > 0; --count) { ... }` (guard folds into the same cr0 as any preceding compare). Single-exit wrapper `if (n != 0) { for... } return K;` makes both exit paths share one tail `li/bclr`.
- Result:    ScnRes_AnyEntryValued_E5C8 and ScnRes_ScanFlaggedEntry_EE54 FULL_MATCH.
- Confidence: repo_proven
- Applies to/a.k.a.: pairs with the reslist::size() countdown note in MWCC_CASES (CDeviceFileCri).

## Plain-name export of scaffolded funcs: extern "C" declaration BEFORE the definition; matched zero-arg wrappers keep their bare `b` via fn-pointer cast call (code_804BD8E8 linkage sweep, Wii/1.1)
- Symptom:   decomp object exports `func_XXXX__Fv`-style mangled symbols while retail relocs (and sibling wrappers) reference plain `func_XXXX`; nm shows `U func_XXXX` + `T func_XXXX__Fv` in the same object.
- Cause:     auto-scaffold declared stubs without extern "C"; MWCC then treats a later mismatching declaration as a distinct entity instead of erroring.
- Fix:       declare `extern "C" <real signature> func_XXXX(<params>);` above the definition (the definition inherits C linkage, no keyword needed). Wrappers that must stay byte-exact bare `b func_XXXX` tail-calls but now face a prototyped callee compile as `((ret (*)())&func_XXXX)();` — zero args materialized, direct branch preserved.
- Result:    ScnRes_ProjectTracked/ScnRes_ValidateCrossing plain-name exports fixed; ScnRes_ProjThunk_E538/BE5A4/BE5A8/BE628 stayed 100%.
- Confidence: repo_proven
- Applies to/a.k.a.: catalog TUs with mixed scaffolding; check nm on build/us/src/**/*.o when cycle reports unresolved/plain-symbol drift.

## Dense switch dispatching same-TU helpers: noinline every case target

- Symptom:   dispatcher function explodes to many times retail size; hexdiff shows hundreds of structural diffs and decomp-side relocs vanish (case bodies present inline instead of `bl` per case)
- Cause:     `-ipa file` inlines every same-TU callee into the switch, destroying the jump-table form
- Fix:       add `__declspec(noinline)` to each case-target DEFINITION (bodies unchanged), plus an `extern "C"` declaration block BEFORE the definitions so call sites resolve to the unmangled retail symbols. Verify the state->function mapping against the retail asm table entry-by-entry (a one-slot shift cascades through all subsequent relocs). Existing postprocess `data_pool_patterns` rules name the auto-emitted jumptable once the shape matches
- Result:    MakeCrystal_DispatchState (CModelDispMakeCrystal) 0.7% -> 100.0% FULL_MATCH; unit .text overage 3640B -> 324B
- Confidence: repo_proven
- Applies to/a.k.a.: any state-machine/opcode dispatcher over same-TU runners; also check the cmpli bounds constant (table size) and inline-case bodies against retail before assuming deeper problems

### Weak-definition anti-folding for code references to locally-defined data labels (ScheduleList ctors)

When a dissolved-data splice defines its vtable/label arrays **in the same TU** that references them
from code (`self->vtable = (void*)&lbl_eu_XXXX`), MWCC folds multiple nearby-label references into ONE
`lis/addi` anchor register plus constant-offset `addi`s (`addi r0,r27,+84`), while retail - whose ctor
TU saw those tables as extern - emits one named `R_PPC_ADDR16_HA/LO` reloc pair PER reference. Byte
shapes then diverge structurally and no extern "C" rename applies (symbols are already correctly
named).

**Fix:** mark the table *definitions* `__attribute__((weak))`. A weak definition can be overridden at
link time, so MWCC must not fold references to it and emits individual named HA/LO relocs; the section
bytes and layout are unchanged, and no other definition exists so addresses resolve identically.
Verified on Wii/1.1 -O4,p: `extern "C" u32 lbl_eu_80570078[12] __attribute__((weak)) = {...};`
converted 3 folded refs back into 3 named reloc pairs (structural mismatches 112 -> 99, frame delta
-0x7c -> -0x60 in __ct__804E4B64) with zero regressions across all other unit functions.

Applies to/a.k.a.: any TU where code must take named-reloc references to data labels it also defines
(vptr stores in ctors/dtors of dissolved-data splices); complement to the "extern incomplete-array
keeps sda21 away" trick (MWCC_CASES §1a) which only covers symbols defined in OTHER TUs.

## Retail-inlined factory: spell it as a helper call over a STRING LITERAL so -ipa folds params through the inline (CWorkRoot::initialize fix, Wii/1.1 -O4,p -ipa file)
- Symptom:   `initialize` hand-inlined the CWorkRootThread construction body (new + ctor + entryWork + spInstance store) and landed pure r30/r31 color inversions; swapping declaration order changed NOTHING (MWCC canonicalized), and passing the retail-named label `&lbl_eu_80522744[0]` instead of a literal BROKE the schedule entirely (getWorkMem hoisted above the string lis; prologue shrank to one saved reg).
- Cause:     retail's shape is `create(pName, parent)` inlined by IPA with the call-site LITERAL constant-propagated through the inline — the parameter web is born as a known constant, which drives both scheduling and scratch coloring. An opaque named-array argument kills the constant fold, and IPA makes different inline/schedule decisions.
- Fix:       keep the static factory (`CWorkRootThread::create(...)`) and call it with the string LITERAL: `create("CWorkRoot", nullptr)`. Residual: the literal pools as anon `@stringBase0` instead of the retail pool label (0.2% name-only reloc drift) — pairing it via UNIT_RULES would collide with the already-defined lbl_eu_80522744 array symbol, so accept at FULL_MATCH-with-certificate.
- Result:    initialize FULL_MATCH 99.8% (from CODE_MATCH 98.5% with 13+ reg_swaps / 36 structural variants tried); standbyWork(thread,bool) refreshed to 100.0%.
- Confidence: repo_proven
- Applies to/a.k.a.: any TU where retail inlined a create/init factory into a caller — prefer helper-call-plus-literal over hand-inlining the body; register_mapping.md Rule A does not apply when an IPA-folded constant is the first-born web.

### Literal try{...}catch(...){throw;} reproduces monolib reslist exception-frame codegen (ScheduleList func_804E4830, Wii/1.1 -O4,p -Cpp_exceptions on)

MWCC_CASES ref:8303d15428 diagnosed that retail's dead `stw r1,N(r31)` saves and backchain-restoring
epilogues come from `_reslist_node::setItem`'s `try{...}catch(...){throw;}` under `-Cpp_exceptions on`,
but left "no source change needed" as the fix note (frame-size coupling). Follow-up proof from
ScheduleList `func_804E4830`: writing the construct LITERALLY at the inlined store site

```cpp
if (temp != 0) {
    if (temp != 0) {
        try {
            temp->mItem = p;
        } catch (...) {
            throw;
        }
    }
}
```

reproduces from plain high-level C++: the exception sp-save (`stw sp,28(r31)`), the doubled
`addic./beq` null-guards, AND the backchain epilogue (`or r10,r31,r31; lmw ...; lwz r10,0(sp);
lwz r0,4(r10); or sp,r10,r10`) — function jumped 3.4% -> 37.9% with size gap -0x10 -> -0x4, zero
regressions TU-wide. The unit must compile with `-Cpp_exceptions on` (cflags_game already sets it;
the base list's `off` is overridden).

Applies to/a.k.a.: any monolib reslist push_back/setItem/insert inlined into matched code
(reslist-heavy TUs: effect, coli, scn); whenever a retail function shows a dead sp-store plus
backchain-restore epilogue, suspect an inlined throwing member and write the try/catch literally.

## Fake-vtable kill on menu TUs: fold pads onto named virtuals, but preserve the CALLER's conversion flavor (CMenuBattlePlayerState, Wii/1.1 -O4,p -RTTI on)

Killing TU-local `*If` pad structs (filler virtuals + `vfNNN()`) and retyping the receiver to the real
class is codegen-neutral ONLY if every folded slot keeps the retail ABI. Three traps found while
de-padding `kyoshin/menu/CMenuBattlePlayerState.cpp` (Move held 94% before and after; all seven Ifs
deleted):

1. **Signed/unsigned conversion flavor beats the shared header's return type.** The pad declared the
   +0x1E8/+0x1F0 getters `int`; the hot header (`cf::CActorParam.hpp`) types both `u32`. Retail
   converts +0x1E8 as SIGNED (no xoris before the biased lfd) in one caller and via explicit
   `(u32)` in another. Fix: call the named u32 virtual through `static_cast<f32>(static_cast<int>(
   actor->Func85()))` at signed sites — MWCC composes u32→int→f32 into the plain signed stw/lfd/
   fsubs sequence. Getting this wrong regressed Move 94% → 4.6% (489 structural).
2. **Missing arg setup ≠ no-arg call.** Retail `Animate()`-shaped dispatches (+0x38) that appear to
   pass garbage r4 are actually `Animate(0)` with the `li r4,0` SCHEDULED EARLY across intervening
   stores. Check a ±10-instruction window before concluding the prototype had no parameter.
3. **Pad slot arithmetic**: first declared virtual lands at vtable+0x8 under `-RTTI on`; N fillers put
   the next virtual at 0x8+4N. `range(0x008, 0x1FC, 4)` style off-by-ones shift the dispatch -0x4 and
   show up as a single `lwz r12, K(r12)` diff.

Also reusable: when retail chains a virtual's RETURN as the next lvalue base (`bcctrl; lbz r0,
187(r3)` after Pane::FindPaneByName), spell it chained — `reinterpret_cast<V*>(root->FindPaneByName(
n, true))->flagsBB &= ...` — not via a pre-loaded pointer temp.

Applies to/a.k.a.: "Fake vtables -> real classes" playbook (CHelp flow); any TU-local dispatch pad over
cf::/nw4r:: classes with named slots; conversion-flavor preservation when folding.

## Same-name non-virtual shadow of a base virtual APPENDS a vtable slot (Wii/1.1 -O4,p -RTTI on)
- Symptom:   virtual calls through a derived type emit `lwz r12, K+0xC` (3 slots late) vs retail;
  every downstream slot in the primary table shifts by the shadow count. Isolated with .scratch
  replica hierarchies: a derived class redeclaring a SECONDARY-base virtual's name appends +1 slot
  per same-signature redeclaration, with or without the `virtual` keyword; different-signature
  redeclarations and the ctor add nothing. Concretely `void* g18(); int g03(); void g02();` (same
  signatures as base, no bodies) shifted a probe call 0x5B0 -> 0x5BC (+3); adding `virtual` did NOT
  fix it; only deletion restored the layout.
- Cause:     MWCC 1.1 does not merge same-name redeclarations of secondary-base virtuals into the
  base slot (no override matching without an exact override declaration path); each becomes a new
  primary-table entry. Real case: `CActorParam` redeclared `CBattleState` UVF18/UVF3/UVF2 with
  identical signatures and no bodies, shifting `CfObjectActor`-own virtuals (UVF6 0x5B4 -> 0x5C0)
  and breaking every primary-table fold onto them. Verified against the retail vtable group
  (DOL .data dump: `CfObjectActor_getActorHeading` sits at group +0x5B4).
- Fix:       delete the bodyless same-signature shadows (they were uncalled and undefined); keep
  different-signature shadows (harmless, hidden) and inline-defined same-signature ones (proper
  overrides/definitions, no append). If retail owns a same-named thunk (e.g. battle -8-adjust
  thunks `CBattleState_UnkVirtualFunc3__Q22cf11CActorParamFv`: `subi r3,-8; b CActorParam UVF1`),
  define it as an `extern "C"` free function with the mangled name (forward-declare the target
  `extern "C"` too, or the reference drifts to `...__FP...`).
- Result:    FULL_MATCH restored on all affected callers; dependent TUs held or improved
  (CfObjectPc 23/36 -> 29/36, CAIAction 10/23 -> 11/23).
- Confidence: repo_proven
- Applies to/a.k.a.: any hierarchy where a derived header redeclares secondary-base virtual names
  (the CHelpSwitch `func_802B7CB0 stayed non-virtual` rule is the same quirk from the other side);
  vtable-offset forensics via DOL-group dump + .scratch replica bisection.

## Phantom __vt__ materialize from pre-call float locals in a hand-vtable ctor (Wii/1.1)
- Symptom:   ctor gains lis/addi + stw of __vt__Q2... (compiler-generated vtable symbol the TU never emits) in the prologue: +12 bytes and an UNDEF link break; goes away when the float locals are removed
- Cause:     hand-written vtable stores (lbl_eu_* casts) + float locals whose scope spans most of phase 1 before a (proxy or real) virtual call: MWCC -ipa synthesizes its own __vt__ reference (bisected: phase-1 scoped {one, zero} triggers it, phase-2-only post-call locals do not; real this->virtual call is NOT required - proxy m5C build also showed it)
- Fix:       keep pre-call float usage to direct pool refs (accept f0-vs-f1 regswap, size-neutral); float locals are safe ONLY when defined after the last call of their span (phase-2 one2/zero2 pattern). Same reason CScnItemModel uses a fragment ctor (header comment there)
- Result:    CfObjectModel ctor: phantom-free, exact size 0x118
- Confidence: repo_proven (bisect in-TU + CScnItemModel corroboration)
- Applies to/a.k.a.: any TU with hand-written .data vtables + explicit vtable stores in ctor/dtor; -ipa file devirtualization analysis

## u8 base-local alias reproduces View-pointer codegen (Wii/1.1, absolute-field TUs)
- Symptom:   replacing `View* v = (View*)self; ... v->field` with raw `(T*)((u8*)self + OFF)` at two
  use sites (call arg + later store) CSEs the address tree into a saved reg (extra `addi` + save/
  restore, +12 bytes) instead of folding base+displacement per use like the View version did
- Cause:     `(u8*)self + OFF` is a materializable computation MWCC hoists across calls/loops;
  `v + OFF` (register + const) folds at each use. Deleting the local changes allocation coloring too
- Fix:       keep one plain `u8* sb = (u8*)self;` local and spell uses `*(T*)(sb + OFF)` /
  `(T*)(sb + OFF)`; same register+const trees as the View version, zero extra regs. Proven on a
  FULL function (accumulateTension 0x1e0: +105 struct regressed to raw-direct, back to FULL with sb)
- Result:    FULL_MATCH preserved while deleting the View struct and all its uses
- Confidence: repo_proven
- Applies to/a.k.a.: any TU replacing absolute-offset Views with raw casts where an address is used
  twice across a call/loop; tail-region fields past a layout shift (members address wrong bytes)

## Same-TU direct call inlines tiny callee, breaking subi+b tail thunks (Wii/1.1)
- Symptom:   rewriting a thunk's `((CAST)target)(adjusted)` as a direct `target(adjusted)` call
  absorbs the callee body (0x8 thunk ballooned to 0xa0 / 0x14) instead of `subi r3,-8; b target`
- Cause:     MWCC auto-inlines same-TU direct calls when decl/def agree (fires at least up to 0x94
  body size; 0x144 body was left alone); the indirect-call spelling is opaque to the inliner, so the
  tail branch survives and r4 passes through untouched (retail shape)
- Fix:       keep a precise-prototype indirect call `((RET(*)(ARGS))target)(adjusted, fwd)` with the
  exact retail arity (document why: inline-block + r4-forward); direct spelling only when the callee
  is too big to inline or decl/def disagree. Verified subi+b FULL on all five CActorParam
  CBattleState thunks (0x8/0x8)
- Result:    FULL_MATCH on adjust-this tail thunks with honest arity, no generic pad typedef
- Confidence: repo_proven
- Applies to/a.k.a.: any adjust-this/forwarding thunk TU (CBattleState/CActorParam/CfObjectActor
  families); alternative is `#pragma auto_inline` games (unverified, TU-wide blast radius)

## Dual-branch pointer locals can need opposite declaration order (Wii/1.1)
- Symptom:   one `if`/`else` copy of the same root/base walk matches; the other has those two saved colors swapped
- Cause:     the branches are separate vregs. Rule A is per-block; the order that colors the first block correctly can invert the second
- Fix:       keep the matching branch's declaration order; flip only the mismatched branch (`root;` then `base;` vs `base;` then `root=...`)
- Applies to/a.k.a.: Rule A on duplicated if/else pointer walks
- Confidence: repo_proven
- Example:   us-802343bc (`ArtsTable_bumpUsage`)

## Implicit array-member ctor walk: declare end first, assign start first (Wii/1.1 -O4,p)
- Symptom:   inlined `T[N]` member construction walks with start/end colors swapped (`addi r31,this+start` / `addi r30,this+end` vs retail r30/r31); 0 structural
- Cause:     implicit per-element ctor creates start before end, so Rule A colors start=r31. A source `while` that declares `end` first gets the colors but emits end's `addi` first and a forward `b` to the test (size +4, structural collapse)
- Fix:       make the element POD; write a `do`-`while` in the enclosing ctor; declare `end` first (Rule A → r31) but assign `start` first so the `addi` order matches retail; keep the later bulk `clear()`/`memset` if retail has both
- Applies to/a.k.a.: Rule A start/end array walks; CBattleManager unk94[32] 8-byte memset loop
- Confidence: repo_proven
- Example:   us-800d8fe4 (`__ct__Q22cf14CBattleManagerFv`)

## MWCC treats `f32`/`fN` as FPR names in some struct members (Wii/1.1)
- Symptom:   `undefined identifier 'f32'` on a later member such as `f32 f40`, even though earlier
  `f32` fields in the same TU compiled
- Cause:     MetroWerks lexes `f0`–`f31` as floating-point registers. A member named `f40` (or the
  type token `f32` next to another `fNN` name) can be parsed as a register rather than a type/field
- Fix:       spell the type as `float` and avoid `fNN` member names in that struct (`p40`, `field_40`)
- Applies to/a.k.a.: any large overlay struct with `f32 f00`… style field names
- Confidence: repo_proven
- Example:   us-8023f51c (`NandApplySave` battle-param overlay)

## Signed byte call-arg wants `optimize_for_size` + named `s8` (Wii/1.1)
- Symptom:   Retail `lbz r4,off; extsb r4,r4; clrlwi r4,r4,16` into arg2; decomp `lbz r0` / `extsb r0` / `rlwinm r4,r0`. Inlining `(u16)(s8)field` or `int val=(s8)field` at `-O4,p` does not move the load into r4.
- Cause:     `-O4,p` births the sign-extend temp in r0. `-O4,s` (`#pragma optimize_for_size`) plus a named `s8` local performs the extend in-place in the ABI arg register.
- Fix:       `#pragma optimize_for_size on` around the function; `s8 page = obj->byte; fn(ptr, (u16)page);`
- Applies to/a.k.a.: register_mapping.md Rule B (call-arg liveness); sibling CMenuArtsSet `ArtsTable_scrollUp` / `ArtsTable_pageUp` already use `-O4,s`
- Confidence: repo_proven
- Example:   us-80233ef8 (`ArtsTable_pageDown`)

## `(u8)func(...)` vs `func(...) & 0xFF` changes VR birth (Wii/1.1 -O4,p)
- Symptom:   Pure saved-reg swap of a call result and a truncated helper return (`rlwinm …,0,24,31` already matches). Decl-order swaps and `int` vs `u8` are no-ops. Inlining the truncated value flips call order.
- Cause:     `& 0xFF` after the call creates an extra VR that colors before the earlier named local. A `(u8)` result cast is the same mask instruction but a different birth, so Rule A/C colors the first call result r31.
- Fix:       Prefer `T x = (u8)func(...)` when the mask is already at assignment time and a later-named local is stealing r31.
- Applies to/a.k.a.: register_mapping.md Rule A/C; any `bl` + immediate `rlwinm` truncate into a saved reg
- Confidence: repo_proven
- Example:   us-801ffd88 (`swapPartyMembers`)

## Counted loop unrolls to CTR + dead `+(checks-1)` induction counter (Wii/1.1 -O4,p)
- Symptom:   Retail has `li rX,0` before a 2-trip `mtctr` loop and `addi rX,rX,<checks-1>`
  inside the body, with no compare on the counter; a plain `for (i = 0; i < N; i++)` over
  fixed-offset checks compiles without the counter.
- Cause:     A counted loop whose index is also the search position (e.g.
  `for (i = 0; i < 16; i++) if (self[i].id) return 1;` over 8-byte entries) gets unrolled 8x;
  the optimizer converts the trip count to `mtctr` but keeps the induction update as a dead
  `i += checks-1` (7) because the web was live at DCE time. MWCC removes a counter that only
  feeds the loop test, so a manually-unrolled/`!= 14; += 7` rewrite loses it.
- Fix:       Write the natural indexed scan (`struct Entry { u32 id; u32 data; }; Entry* e =
  (Entry*)self; for (s32 i = 0; i < 16; i++) if (e[i].id) return 1;`). The pointer advance and
  the dead counter then fall out of the unroller.
- Result:    FULL_MATCH (CPartsChange_HasAnyEntry 0xc0/0xc0)
- Confidence: repo_proven
- Follow-up (findFreeBattleSlot us-800d880c FULL_MATCH 0x17c/0x17c): when the scan
  compares against a key word (not zero), three refinements stack on the base recipe:
  (1) keep the key base (`kp`) and the advancing byte cursor (`p += 4`) as
  DISTINCT locals — the extra live temp shifts numbering to retail colors
  (idx r4 / scan r5 / base r6) instead of scan-steals-r4; (2) read the scan word
  into a per-iteration temp FIRST (`u32 s = *(p+8); if (s == kp[1])`) — the temp
  forces the scan side to load before the invariant key side (else MWCC loads
  the key first and flips cmpl order, which rho cannot rename); (3) exit the
  unrolled loop with `goto out` + a post-loop `found = 0` (no pre-init: the
  flag then lives in r0) and return via a shared `goto done` exit (else the
  mid/loop2 `return result` const-folds to `li r3,0` or emits an inline `or`).
  Goto-gate chains are the §17.6-allowed escape; log with policy_exception.
- Applies to/a.k.a.: inlined-search family (BTE find_first_serv / find_mx_serv "dead
  +(checks-1) counter"); counted scans over fixed-stride records; any `li rX,0 ... mtctr ...
  addi rX,rX,N-1` shape

## Retail shares out-of-line CResLookup-style vcall thunks; MWCC inlines the dispatch (Wii/1.1 -O4,p -ipa file)
- Symptom:   Every resolve* helper in a unit has `lwz r3,0x2C(rX); lwz r12,0(r3); lwz r12,slot(r12); mtctr; bcctrl`
  inline where retail has `mr r3,rX; li r4,arg; bl <Sym>_vcallNN` plus a 0x18-0x28-byte thunk at a fixed
  address doing the dispatch (and often `bctr`/`bclr` instead of `bcctrl`/`blr`).
- Cause:     The original source used plain member calls (`entry->field_0x2C->method(entry, arg)`); MWCC
  extracted one shared dispatcher per vtable slot and calls it from every site. Our build inlines because the
  call-site shapes/implementations differ enough that MWCC does not dedupe.
- Fix:       Define the thunk as a real high-level function and mark it `__declspec(noinline)` (otherwise
  MWCC re-inlines it), then call it at every site:
  `extern "C" __declspec(noinline) int Sym_vcallNN(void* self, u32 val) { CResLookup* l =
  ((ResInfoEntry*)self)->field_0x2C; return l->cmpField4Eq(self, val); }`
  Null-guarded tail-call thunks are `void` with `if (l == 0) return; l->method(self);` (yields `bctr`).
  Watch the return type: a value-returning thunk forces a stack frame + `bcctrl` + epilogue; retail's
  `bctr` proves the original thunk returns void.
- Result:    CfRes cluster 15/15 FULL_MATCH (0x80064154..0x80065F48); unit 189 -> 213/224 functions.
- Confidence: repo_proven
- Applies to/a.k.a.: any TU with `_vcallNN`-style shared dispatchers (CfRes, CModelDisp, IResInfo callers);
  MWCC_CASES "CScnFilterMan" noinline-callee recipe extended to vtable dispatch.

## Sentinel-first declaration order fixes a whole GPR colour set (Wii/1.1 -O4,p)
- Symptom:   A list-walk helper is byte-perfect except every register is off by one position
  (`sentinel r7 / e r3 / node r4 / result r5 / id r6` vs retail `sentinel r3 / e r4 / node r5 / result r6 / id r7`),
  i.e. one value takes the `r3` the others each want, pushing the rest up.
- Cause:     MWCC colours locals in declaration order, but a pointer loaded from a parameter register
  *coalesces* onto the dying parameter only when it is the first-declared local. Declaring the walk
  cursor/entry/result before `sentinel` makes the sentinel take the next free colour instead of `r3`.
- Fix:       Declare the loop's sentinel pointer first, then the remaining locals in retail's colour order:
  `Node* sentinel; Entry* e; Node* node; void* result = 0; u32 id = *(u16*)(obj + 0x45C0);`
  (`result`/`id` initialisers may still sit later in the body; only the declaration order matters).
- Result:    FULL_MATCH (CPartsChange_FindActorByObj 0x78/0x78, 0 structural after the reorder)
- Confidence: repo_proven
- Applies to/a.k.a.: any "walk a sentinel list, compare a key, return a pointer" helper; pairs with the
  CScnFilterMan/`next`-before-`node` declaration-order case.

## Interface-class vtable offset convention: add a 2-word header (Wii/1.1, -RTTI on)
- Symptom:   Calling the Nth virtual of a TU-local interface class emits `lwz r12, 0x28(r12)` where retail
  reads `lwz r12, 0x20(r12)` (and `0x2c` vs `0x24`), with the class's first dummy virtual otherwise unused.
- Cause:     MWCC vtable layout reserves two words of header before the first declared virtual, so the
  first `virtual void v0()` lives at object+0x8. Retail's slot 8 (`0x20`) is therefore the *6th* declared
  virtual in our spelling.
- Fix:       Declare `dummies = slot_index - 2` placeholder virtuals before the method you need
  (`for slot 0x20: v0..v5, then getCount(), getName(u32)`), and document the arithmetic near the class.
- Result:    CPartsChange_ResolveSlotNames dispatches at retail's 0x20/0x24 (was 0x28/0x2c).
- Confidence: repo_proven
- Applies to/a.k.a.: every opaque interface class in the catalog TUs; CScnEnvLgtCtrlListItem's
  "v24 slot (0x68)" comment is the same convention.

## Byte-view of a returned u32 gives retail's big-endian `stw`+`lbz` (Wii/1.1)
- Symptom:   Retail stores a call result to a stack slot and immediately `lbz`es it back
  (`stw r3,8(r1); lbz r0,8(r1); cmplwi r0,3`) where a `(u8)` cast gives a single `rlwinm r0,r3,0,24,31`.
- Cause:     `stw`+`lbz` reads the *most significant* byte (lowest address, big-endian); `(u8)` takes the
  least significant. Only a memory byte view can produce the store.
- Fix:       Keep the u32 in a named local and read its first byte: `u32 val = fn(...); u8 v = *(u8*)&val;`
  Then a u16 mask (`flags &= ~0x4;`) emits `andi.` rather than `rlwinm`, and a `<=` float test
  (`if (f <= 0.0f)`) emits `fcmpo` + `cror eq,lt,eq` + `bne` (the `cror` is absent for `<`).
- Result:    FULL_MATCH (CPartsChange_CheckNameEntry 0x64/0x64, CPartsChange_FireIdEffect 0x98/0x98)
- Confidence: repo_proven
- Applies to/a.k.a.: BDAT column reads, any "compare the first byte of a u32 result" guard.

## Search-result fall-through: assign the miss value AFTER the loop (Wii/1.1 -O4,p)
- Symptom:   A "find an element, else NULL" helper is instruction-identical to retail except one
  `li rX,0` sits in the prologue instead of in the not-found block (all branch displacements shift).
- Cause:     `T* found = 0; if (key) { for (...) { if (hit) { found = p; break; } } }` lets MWCC hoist the
  initialiser to the top; retail's copy is in the fall-through block after the loop.
- Fix:       Leave the local uninitialised, `goto done` out of the loop on a hit, and assign the miss value
  once after the `if`:
  `T* found; if (key != 0) { ... if (hit) { found = p; goto done; } ... } found = 0; done:;`
  The hit path then branches *past* retail's single `li rX,0`, and the key==0 path shares that block.
- Result:    FULL_MATCH (CPartsChange_SpawnById 0xD0/0xD0, 0 structural)
- Confidence: repo_proven

## 12-byte aggregate copy is four word moves in a fixed order (Wii/1.1)
- Symptom:   `*dst = *src;` on a 0x20-byte record copies every field (0x104 bytes instead of 0xDC).
- Cause:     Retail only copies the 12-byte position member (matching CPartsChange_CopyHeaderAndLoad).
- Fix:       Spell the copy on u32 words: `u32 w0 = s[0]; u32 w1 = s[1]; d[1] = w1; d[0] = w0; d[2] = s[2];`
  (second word stored before the first, then the third) -- MWCC's own 3-word copy shape.
- Result:    ComputeSpawnPose A/B/C/D all FULL_MATCH
- Confidence: repo_proven

## FPR scratch colouring follows declaration order, newest-highest (Wii/1.1 -O4,p)
- Symptom:   Two function-scope float locals live across `bl` calls with identical code shape but swapped
  nonvolatile FPRs (angle in f30/radius f31 vs the reverse).
- Cause:     MWCC colours the first-declared of the pair HIGHER (f31) and the second f30, independently of
  the order the arithmetic blocks are emitted.
- Fix:       Declare the value retail keeps in f31 first (`f32 radius = ...; f32 angle = ...;`).
- Result:    FULL_MATCH (ComputeSpawnPoseB 0xDC/0xDC; 89.1% -> 100% from the reorder alone)
- Confidence: repo_proven

## Class destructors: define the raw D0 symbol so no second vtable is emitted (Wii/1.1)
- Symptom:   A TU cannot implement `~Class()` normally because MWCC then emits `__vt__Q22...` next to the
  injected retail vtable data object (duplicate/foreign data in the unit).
- Cause:     The class's key function is the destructor, so defining it in the TU makes the compiler emit
  the vtable there; the retail vtable is supplied as a data blob (`lbl_eu_8053xxxx`).
- Fix:       Define the destructor through its raw retail symbol instead:
  `extern "C" void* __dt__Q22cf12CPartsChangeFv(void* self, int flags) { if (self != 0) { if (flags > 0)
  __dl__FPv(self); } return self; }` -- the class keeps its declared `virtual ~CPartsChange();` and the
  blob provides the vtable slot.
- Result:    FULL_MATCH (__dt__Q22cf12CPartsChangeFv 0x40/0x40)
- Confidence: repo_proven

## Arbitrary vtable slots need a pure-virtual slab view class (Wii/1.1, -RTTI on)
- Symptom:   A call site loads a high slot (`lwz r12,0x2bc(r12)`), and no declared method can reach it.
- Cause:     With -RTTI the two-word header puts declared index N at (N+2)*4, so 0x2bc is index 173.
- Fix:       Add a view class with index+1 pure virtuals (last one being the target slot) to the unit
  header; pure-virtual-only classes emit no vtable. `class CPartsChangeDevView { virtual void v000() = 0;
  ... 173 of them ...; virtual int mAt2BC() = 0; };` then `((CPartsChangeDevView*)self)->mAt2BC()`.
  (Same recipe as CSysWinScenarioLog.hpp's CSysWinDevView.)
- Result:    CPartsChange_ResetBattleEntry dispatches at retail's 0x2bc
- Confidence: repo_proven

## Materialised bool vs the `||` initialiser (Wii/1.1 -O4,p)
- Symptom:   Retail stores a 0/1 flag through a saved register (`li r31,0` ... `li r31,1` ... `cmpwi r31,0`)
  where the decomp folds the condition into a `neg/or/srwi` bool.
- Cause:     A `bool flag = a || b;` initialiser is folded; a separate `flag = false;` plus
  `if (a || b) flag = true;` materialises the value in a register.
- Fix:       `bool flag = false; if (a || b) { flag = true; } if (flag) { ... }`
- Result:    FULL_MATCH (CPartsChange_ResetBattleEntry 0xC8/0xC8, 14% -> 100%)
- Confidence: repo_proven

## Re-load a repeatedly used pointer; do not cache it in a local (Wii/1.1 -O4,p)
- Symptom:   A helper that calls `*(Iface**)(self+8)->method(...)` several times keeps the interface
  pointer in an extra saved register (4 saved GPRs, +8 bytes) where retail re-loads it before every call.
- Cause:     A local `Iface* src = *(Iface**)(self+8);` makes the value call-crossing, so MWCC gives it a
  nonvolatile register and an extra prologue save; the original dereferenced the member inline each time.
- Fix:       Write the load at every use site (`((Iface*)*(u32*)(self + 8))->method(...)`), keeping only the
  cheap guard test (`if (*(u32*)(self + 8) == 0) return;`).
- Result:    FULL_MATCH (CPartsChange_NotifySlotStates 0x10C/0x10C; 10.1% -> 100%)
- Confidence: repo_proven

## Dead CR re-tests around a `&&` guard (Wii/1.1 -O4,p)
- Symptom:   Retail keeps two *unreachable* `beq cr6/cr1` branches (re-testing an already-passed
  conjunction) between the last conjunct's branch and the loop body; omitting them costs 8 bytes.
- Cause:     MWCC evaluates the conjuncts into CR fields and re-materialises the tests when the block is
  entered from the later conjunct's fall-through.
- Note:      The single-`if (A && B) { if (C == 0) { loop } ... }` form and the split `if (A && B) {...}
  if (A && B) {...}` form bracket the retail size (0x128 / 0x13C vs 0x130) but neither emits the dead pair.
  Parked for CPartsChange_SyncSlotState; treat as a codegen artifact, not a source-shape error.
- Confidence: repo_proven (negative result)

## Early exit + fall-through must share one return block: use `goto` (Wii/1.1 -O4,p)
- Symptom:   A search helper is 2 instructions (8 bytes) too big: the `-1`/NULL return is emitted twice --
  once as the early guard's inline `li rX,-1; blr` and once as the loop's fall-through tail.
- Cause:     `if (key == 0) return -1;` and `return -1;` are separate return statements; MWCC blocks them
  separately when the guard is a plain early return.
- Fix:       Route the guard through the same label: `if (key == 0) goto notfound; ... notfound: return -1;`
  (a single block, matching retail's `beq` straight to the tail).
- Result:    FULL_MATCH (CPartsChange_FindVoiceIndex 0xE4/0xE4; 1.7% -> 100%)
- Confidence: repo_proven

## Narrow return types cost an extra mask; widen the prototype (Wii/1.1 -O4,p)
- Symptom:   Retail masks a call result per *comparison* (`clrlwi r0,r3,16` before each
  `cmplwi r0,N`) where the decomp masks once at the assignment (`rlwinm r3,r3,0,16,31` in place).
- Cause:     Declaring the callee as returning `u16` makes MWCC narrow r3 at the assignment; the original
  declaration was wider and each use site cast to u16.
- Fix:       Declare the callee `extern "C" u32 f();` (same linker symbol) and cast the value at each
  comparison, e.g. `if ((u16)v == 0)`. Likewise a signed-valued first parameter must be typed `s32` to get
  `cmpi` instead of `cmpli`.
- Result:    FULL_MATCH (CPartsChange_CheckReloadGate 0x11C/0x11C; 21.1% -> 94.4% -> 100%)
- Confidence: repo_proven

## Fused vs split multiply-add: a named temp defeats fmadds (Wii/1.1 -O4,p)
- Symptom:   `a * b + c` in one expression emits `fmadds`; retail has `fmuls` + `fadds` (two ops).
- Cause:     MWCC fuses at -O4,p; the original computed the product in its own statement, so the
  value died before the add and no fusion was possible.
- Fix:       `f32 off = a * b; f32 out = c + off;` (the temp gets a volatile FPR; it is not
  live across a call so it does not perturb the nonvolatile allocation).
- Result:    CPartsChange_ComputeSpawnPoseModulo 16.0% -> 66.7%, size-exact
- Confidence: repo_proven

## Function-scope float pairs are colored per function, not by a global rule
- `dx`-before-`dz` gives dx=f30 in CPartsChange_ComputeSpawnPoseStaggered (`f30` first = LOWER), while ComputeSpawnPose
  needs the opposite declaration order for the same physical layout. Check the retail pair order
  per function instead of assuming the rule; a two-line swap is the cheapest probe.
- Confidence: repo_proven (CPartsChange_ComputeSpawnPoseStaggered 82.3% -> 89.9%)

## Div-heavy helpers: put the div source statement FIRST, the copy second
- CPartsChange_ComputeSpawnPoseModulo 16% -> 50.6% just by moving the 12-byte copy below the `v = field_14` / mod
  computations; retail schedules the `lwz r9, 0x14(r3)` before any copy store. This mirrors
  ComputeSpawnPoseA/B where the copy is first and the divs are absent -- i.e. match the retail
  load order, not a house style.
- Confidence: repo_proven

## Indexed stores: write `slots[i]` / `slots[i+1]`, not `*(p + i)` (Wii/1.1 -O4,p)
- Symptom:   Two adjacent word stores; retail emits `stwx r5,r3,r0` (indexed) and only THEN
  `add r4,r3,r0; stw r5,4(r4)`, while the pointer-arithmetic source hoists the `add` before the
  first store (2-instruction transpose, 94.3% forever, no witness possible).
- Cause:     `*(u32*)(actor + i*8)` and `*(u32*)(actor + i*8 + 4)` are two independent address
  computations the scheduler is free to hoist; the array form keeps the slot clear as one
  indexed store plus one base+offset store in source order.
- Fix:       introduce `u32* slots = (u32*)actor;` and write `slots[i * 2] = 0; slots[i * 2 + 1] = 0;`
- Result:    CPartsChange_UnregisterEntry 94.3% -> 100.0% (0x8C/0x8C)
- Confidence: repo_proven

## Re-read array counts instead of caching them in a local (Wii/1.1 -O4,p)
- Symptom:   A growable-array helper ends 2 instructions long and saves extra nonvolatile GPRs
  (r28/r29/r30 instead of r30/r31): MWCC hoisted `u32 n = *(u32*)(self+0x9800)` out of reach and
  kept it live across the memset calls, spilling into nonvolatiles.
- Cause:     A named local is a *snapshot*, so MWCC may keep it in a register across calls. Retail
  re-loads the count in the tail (one load feeding mulli + addi) and uses a scratch register in the
  loop prologue, i.e. the original never cached it outside the loop.
- Fix:       Use the member expression at each use -- `u8* end = self + *(u32*)(self+0x9800)*0x4C;`
  for the loop, then in the tail `u32 n = *(u32*)(self+0x9800); *(u32*)(self+0x9800) = n + 1;
  memcpy(self + n*0x4C, &rec, 0x4C);` (a short-lived local whose uses all precede the call).
- Result:    CPartsChange_InitChangeRecord 1.4% -> 100.0% (0x10C/0x10C) after the frame/back-end
  regs converged.
- Confidence: repo_proven

## The same indexed-array rule also kills the `stwux` fusion
- Symptom:   Retail has `stwx r0,r3,r5` + `add r5,r3,r5` (store, then materialise the element
  address) and one MORE instruction; the pointer version emits the fused `stwux r0,r5,r3`
  (store-with-update) and gets a different tail layout (`add r6,r3,r0; stw r5,0(r6)` instead of
  `stwx`).
- Cause:     `*(u32*)(self + idx) = v; u8* q = self + idx;` lets MWCC fold the address computation
  into the store; the array form keeps the two stores as subscript operations on a `u32*`.
- Fix:       `u32* slots = (u32*)self; slots[idx] = slots[0]; slot = 0; slots[idx+1] = slots[1];`
  and for the record fill `slots[idx2] = id; slots[idx2+1] = 0;` with `idx = slot * 2` in u32 units.
- Result:    CPartsChange_FindFreeSlot 68.7% -> 100.0% (0x14C/0x14C); the 16-slot scan itself was
  already exact (a `for (i = 0; i < 16; i++)` unrolled 8x with a 2-iteration ctr block).
- Confidence: repo_proven (2nd independent hit after UnregisterEntry)

## Inlined 2-word struct swap reproduces the MWCC spill shape exactly
- The 4 inlined copies in CPartsChange_CopyObjFields use distinct stack temp slots (0x08, 0x10,
  0x18, 0x20) with a `stw` spill + `lfs` reload for the float half -- that is MWCC's inlined
  `static inline void Swap(T* x, T* y) { T t = *x; *x = *y; *y = t; }` with `struct T { u32; f32; }`.
  Writing the swap out by hand or typing the second member as `u32` does not give the `lfs`/`stfs`
  mix; a function-pointer member (`int (*fp)(T*, T*)` at +0) plus two `== 0` bools gave 100% on the
  first build (0x170/0x170).
- Confidence: repo_proven

## Small-data (SDA21) vs ADDR16_HA/LO: size the array in the declaration
- Symptom:   `extern const u16 table[];` compiles to `lis/addi` (R_PPC_ADDR16_HA + _LO) where retail has
  `li r3, table@sda21` + `lhzx` (R_PPC_EMB_SDA21).
- Cause:     An unsized array is not a candidate for .sdata; MWCC emits a full address.
- Fix:       Give the declaration a small size (`extern const u16 table[4];`) so the symbol lands in
  .sdata and the access uses the SDA21 reloc.
- Result:    CPartsChange_SpawnItemDrop 40.6% -> 58.9%, size-exact (0x17C/0x17C), 12 fewer structural.
- Confidence: repo_proven

## `(ptr + 0)` pointer-deref forces a register-relative store AND preserves the reload MWCC would CSE (__ct__80193270 reslist sentinel, Wii/1.1 -O4,p)
- Symptom:   sentinel init `head->next = head; head->prev = head;` decompiled as two displacement stores off the base (`stw r8, -0x57f0(r9)` / `stw r8, 4(r8)`, 4 bytes short) while retail has `stw r8, 0(r8)` then `lwz r8, -0x57f0(r9)` (reload the head FIELD) then `stw r8, 4(r8)`.
- Cause:     writing the first store as a plain *address expression* (`*(u32*)(self + 0xA810) = ...`) lets MWCC emit a displacement store and prove it does not alias the head field at self+0xA80C, so the later `*(u32**)(self + 0xA80C)` load is CSE'd away. Retail's store address is a pointer VALUE (r8), so the aliasing store forces the reload.
- Fix:       write the self-link through a pointer deref with an explicit `+0`: `*(u32*)(*(u32**)(self + 0xA80C) + 0) = (u32)(self + 0xA810);` — this emits the `0(r8)` form *and* keeps the head-field reload. Result: 31% -> **FULL_MATCH** (0x15c/0x15c, 0 structural).
- Confidence: repo_proven
- Applies to/a.k.a.: any inlined doubly-linked-list sentinel init; also the general trick when retail reloads a field that decomp CSEs.

## 0xC-stride u32 slot stores: index through a byte pointer or MWCC scales by 4 (__ct__80193270 actor table, Wii/1.1 -O4,p)
- Symptom:   16-slot wipe of a `u32*` member emitted stores at +0xC0/+0xF0/+0x120… (stride 0x30) instead of retail's +0xC/+0x18/+0x24 (stride 0xC).
- Cause:     `*(u32*)(*(u32**)(self + 0xA81C) + i * 0xC)` — pointer arithmetic on `u32*` multiplies the index by 4.
- Fix:       `*(u32*)((u8*)*(u32**)(self + 0xA81C) + i * 0xC) = 0;` (byte pointer, MWCC still emits the unrolled displacement stores and reloads the member each time).
- Confidence: repo_proven
- Applies to/a.k.a.: any element stride that is not a multiple of the pointee size; pairs with the +4/-byte-cast rule for link-field stores.

## Nested duplicate null check is the inlined-base-destructor shape — keep both tests (__dt__80193538, Wii/1.1 -O4,p)
- Symptom:   4 bytes short: decomp emits ONE `bc 12, 2` for `if (list != 0) { …unlink+free… }` where retail emits TWO consecutive `bc 12, 2` to the same target (`addic. r29, …` then both branches test CR0).
- Cause:     retail's source has the call-site null check plus the inlined callee's own `if (this != 0)` check; both read the same `addic.` CR0, so MWCC keeps both branches (no merge).
- Fix:       write the duplicate explicitly — `if (list != 0) { if (list != 0) { …body… } }` (indentation kept for readability). Result: 50% -> **FULL_MATCH** (0x138/0x138).
- Confidence: repo_proven
- Applies to/a.k.a.: any inlined member that begins with a `this != 0` guard (reslist base teardowns, `delete`-style helpers).

## Inline the loop bound into the `for` header to colour the induction variable low (CPartsChange_FindPartsElem, Wii/1.1 -O4,p)
- Symptom:   0x6c/0x6c, 0 structural, 8 reg_swap: retail colours the counter r5 and the loop bound r0; decomp had the bound in r5 and the counter in r7.
- Cause:     the bind variable `u32 n = *(u32*)(self + 0x9800);` was hoisted above the loop, so its virtual register is born first and claims the low register.
- Fix:       inline the bound — `for (u32 i = 0; i < *(u32*)(self + 0x9800); i++)` — the induction variable is then defined first (r5) and MWCC hoists the bound load itself into r0. Result: 70.4% -> **FULL_MATCH**.
- Confidence: repo_proven
- Applies to/a.k.a.: compare-counted loops where the bound is a member load; also explains retail's `mtspr 19, r0` colour in CTR loops.

## Duplicate the inline helper call in the else-if arm; hoist table/base; hoist single-use locals to steer colours (CPartsChange_ResolveSlotReplayIds, Wii/1.1 -O4,p)
- Symptom:   stub -> 43% after the natural decode; retail re-calls findObjectById in the else arm and re-reads the id from the slot each time.
- Cause:     the source's helper (`findObjectById(id); if (p) p -= 0x3E9C;`) is inlined at each mention, so an `else if` arm re-emits the whole sequence; a named `id` local instead produces an extra `or r3, r27, r27` per arm.
- Fix:       inline `*(u32*)(self + i * 8)` at every helper call site (no named local), hoist the BDAT table + column base above the loop (`void* bdat = lbl_eu_806640CC; const char* col = lbl_eu_80503C48;`), and hoist the third loop's `u16 replay = 0;` to function scope. Result: 0% -> 82.1%, 0 structural, size-exact; residual is a pure GPR colour permutation.
- Confidence: repo_proven
- Applies to/a.k.a.: inline accessor helpers (`GetXxx(id)` with a null-adjust) used in both arms of `if/else if`; 16-slot table sweeps.

## All-zero aggregate in `.data`: needs `#pragma section data_type` *and* the section declspec (Wii/1.1, big TU)
- Symptom:   a retail `.data` object whose bytes are all zero (e.g. a zero-filled pointer
             table or vtable slice) lands in `.bss`, so the unit's `.data` is short by
             exactly its size — and `ldscript.lcf` concatenates sections positionally, so
             every later unit's data shifts.  `run.py data diff` reports
             `[FAIL] .data: retail size 0x228 != decomp 0x1F8` while the chips it *does*
             define are byte-identical.
- Cause:     MWCC routes a zero-initialised aggregate to `.bss` no matter what the source
             says: `__declspec(section ".data")` alone is ignored, `__attribute__((used))`,
             `nocommon` and `aligned` do not change it, and in a large TU
             `#pragma section data_type ".data" ".data"` alone is ignored too (it works in a
             small probe, and works in the real TU only once the declspec is also present).
- Fix:       put `#pragma section data_type ".data" ".data"` in front of the block **and**
             keep `__declspec(section ".data")` on each all-zero definition; restore with
             `#pragma section data_type ".data" ".bss"` before the next `.bss`/`.sbss`
             definitions.  Result: `.data` 0x228 with identical bytes, `.bss`/`.sbss`
             back at their retail sizes.  (`kyoshin/code_800A75FC.cpp`; the same pairing is
             what `CfNandManager.cpp` uses.)
- Confidence: repo_proven
- Applies to/a.k.a.: retail label reconstruction / absorb-free data typification; any
             "section size off by a multiple of the zero object" data-gate FAIL;
             `#pragma push`/`pop` around the block if the file already uses section pragmas.

## Zero-init `.sbss` slot wider than the object: `aligned(n)` on the *next* symbol reproduces retail offsets (code_800A75FC)
- Symptom:   retail has `lbl_eu_80663E98` as an 8-byte `.sbss` symbol (byte flag) and the
             next label at +0x08; a plain `s8` definition puts the next label at +0x01, so
             five `(s8,u32)` stage pairs land 0x2F early and `.sbss` (0x30) comes up 1 byte.
- Cause:     the flag's users do byte loads/stores (`lbz`/`stb`), so its *type* must stay
             `s8`; the width in the retail map is slot padding, not an access width.
- Fix:       keep `s8 lbl_eu_80663E98;` and declare the *next* object
             `__attribute__((aligned(8))) s8 lbl_eu_80663EA0;` — the 8-byte slot and every
             following retail offset then reproduce exactly (`.sbss` 0x30, zero-fill).
- Confidence: repo_proven
- Applies to/a.k.a.: `.sbss`/`.bss` label groups whose retail offsets are 8-aligned but
             whose access width is 1; works without touching the flag's 14 call sites.

### Chained `a = b = c = 0` stores emit right-to-left; split for ascending slot order (MWSFCRE_SetSupplySj, Wii/1.1 `-O4,p`)
- Symptom:   arg-struct fill `arg.a = 2; arg.b = sj; arg.c = arg.d = arg.e = 0; arg.f = 0;` at 95.3% (0 structural, 4 reg_swap): the three chained zero-stores emit `stw 24(sp); stw 20(sp); stw 16(sp)` (descending) where retail emits `stw 16/20/24(sp)` (ascending); the `a`/`b` stores already match.
- Cause:     chained assignment evaluates right-to-left (`e = 0` first), and MWCC preserves that store order into emission; retail's ascending order means its source used separate statements.
- Fix:       split the chain into separate statements (`arg.c = 0; arg.d = 0; arg.e = 0;`) — 95.3% to 97.7%, zero new instructions. Statement-order swaps of the `a`/`b` pair are no-ops (frontend normalizes), and the remaining `stw 8(sp)`/`stw 12(sp)` emission-order pair is a scheduler tie-break, not an IR-order effect.
- Result:    MWSFCRE_SetSupplySj 95.3% to 97.7% (residual: one store-order pair, witness-gated by CODE_MATCH callee)
- Confidence: repo_proven
- Applies to/a.k.a.: any `x = y = C` zero-fill chain to adjacent struct slots; pairs with the sinit descending-stores note (same mechanism, opposite direction).

## `extern "C"` on data/array definitions is a no-op (FloatUtils, Wii/1.1 `-O4,p`)
- Symptom:   `docs/CODE_SMELLS.md` reports ~19k `extern "C"` lines; its generated note used
             to claim ~80% were "retail-reloc-mandated" and therefore untouchable.
- Cause:     wrong.  The retail `main.dol` is stripped — `grep -a` finds none of
             `wkRender` / `__ct__cf_CfCamFollow` / `func_8004B0B0` / `lbl_eu_*` — so no name
             is retail-mandated.  Every name comes from `config/<region>/symbols.txt` (the
             table `dtk dol split` also uses to name the synthetic objdiff target objects),
             and `extern "C"` merely forces MWCC to emit that literal string.  For *data* it
             is not even needed: MWCC does not mangle namespace-scope variables, so
             `extern "C" const float lbl_eu_x` and `extern const float lbl_eu_x` emit the
             same symbol.  PLAN.md §17.6 already classes the spelling as cleanup debt.
- Fix:       delete `extern "C" ` from data/array declarations in place — line-neutral, so
             no `-ipa file` line-number shift.  A/B on `libs/monolib/src/math/FloatUtils.cpp`:
             93 `extern "C" const …` definitions became `extern const …` (215-line file
             unchanged); `hexdiff --all` still 1/1 FULL_MATCH with a byte-identical reloc
             list, and `run.py data diff` still `VERDICT: MATCH` (`.sdata2` 488 bytes
             identical).
- Result:    that TU's `extern "C" (total lines)` 93 → 0, zero `.text`/data/reloc effect.
- Confidence: repo_proven
- Applies to/a.k.a.: `lbl_eu_*` / `data_*` / float-pool constants.  Do **not** apply blindly
             to functions: a real C++ function (or member) mangles, so converting one
             changes the emitted name and needs `symbols rename-plan` + `rename-all`.
             Cross-ref: `docs/CODE_SMELLS.md` Notes; PLAN.md §17.6.

## `__attribute__((aligned(8)))` on a data definition flips a *code* function's register allocation (CTaskGameEffAfter, Wii/1.1 -O4,p)
- Symptom:   after moving a vtable/RTTI table into source as a definition referenced by
             the TU's own ctor, that ctor drops 100.0% -> 76.7% (0 structural, 10 reg_swap,
             same 0xac size).  The only diff is the color of the __ptmf_null base
             (retail r5, decomp r4) and of the first loaded word (retail r4, decomp r5) -
             pure Chaitin coloring, no opcode/immediate change.
- Cause:     the definition carried `__attribute__((aligned(8)))`, which raises the
             `.data` section alignment; MWCC then re-associates/load-schedules the
             lis/addi address materialization of that symbol differently, changing the
             allocator's tie-breaks for every other live value in the function.
- Fix:       drop the alignment attribute from the definition (MWCC's default .data
             alignment is already 8 here, so the section align and the bytes are unchanged);
             hexdiff the *referencing* function, not just the data gate.
- Result:    CTaskGameEffAfter ctor back to 100.0% (8/8) with the hand-built .data/
             .rodata/.sdata slice byte-identical (data gate MATCH, labels placed).
- Confidence: repo_proven
- Applies to/a.k.a.: any dissolve that introduces a source definition whose address a
             matched function materializes; paired with the "Extern-data type change flips
             call-site codegen" record above (same family: data-side attributes leak into
             code allocation).  Note the *declaration* type change (const u8[] -> u32[9])
             was proven innocent here - the definition's presence/alignment was the trigger.

## void* -> typed pointer in matched code — retype is codegen-neutral, but MWCC 1.1 C++ needs explicit casts (Wii/1.1 -O4,p)
- Symptom:   a legacy TU passes `void*` where callers/headers prove a concrete type (`CItemBoxGrid*`,
             `CItemBoxInfo*`); leaving it costs smell count but changing the type looks risky.
- Cause:     object-pointer types are erased at codegen, so the emitted loads/stores/registers are
             identical. BUT MWCC 1.1 is a strict C++ front end: it rejects an implicit `void*`->`T*`
             argument (probed: even `int*` fails), so every call site needs an explicit `(T*)` cast once
             the callee parameter is retyped. The cast itself is free in codegen.
- Fix:       retype the parameter/local to the true type and add same-line explicit casts at the call
             sites (no new locals — see the temp-view record below). SAFE subset: locals, internal
             casts, and parameters of C-linkage functions (`extern "C"` / `func_*`, whose emitted symbol
             carries no type). UNSAFE without the rename ceremony: parameters of mangled C++ functions,
             where the type is part of the mangled name (`...FPv`). Header-owned declarations are a
             separate change (blast radius across every including TU).
- Result:    CItemBoxGrid: 18 C-linkage fns retyped (41 `void*` lines removed, 19 casts added); all 18
             still 100%, unit 106/144 unchanged. CItemBoxInfo: 4 leading params retyped, still 100%.
- Confidence: repo_proven
- Applies to/a.k.a.: matching-neutral cleanup of the `void*` smell family; classify each hit first
             (C-linkage param / local / mangled-param / header-owned / id-as-pointer) before editing.

## Pointer-arith -> typed member view — per-statement casts, never a named temp-view local (Wii/1.1 -O4,p)
- Symptom:   a matched "copy fields out of a stack temp" block written as
             `*(u32*)((u8*)this + 0x74) = *(u32*)(tempBgTex + 0x14);` — byte-exact, but ~100 raw-offset
             lines in one function (CMenuSkipTimer::Init).
- Cause:     the temp buffer is a `u8[]` because the retail ctor symbol is a C-linkage free function
             (`__ct__CBgTex`, so there is no C++ ctor to construct a real object). Types are erased for
             object pointers, so a typed view of the same bytes is codegen-identical; however, hoisting
             that view into a named local (`CBgTex* t = reinterpret_cast<CBgTex*>(buf)`) keeps `t` live
             across the whole straight-line block and costs one extra callee-saved register (`stmw`
             prologue instead of `stw`) -> Init regresses to 5.9%.
- Fix:       replace the destination with the real declared member (`mBgTex.mFileHandle = ...`) and view
             the temp per statement: `reinterpret_cast<CBgTex*>(tempBgTex)->mFileHandle`. Keep the
             retail free-function ctor/dtor calls verbatim. Keep field widths exact: a field retail loads
             with `lha` must be `s16`, not `u16` (which emits `lhz`).
- Result:    CMenuSkipTimer::Init 58 raw-offset lines -> 0, 100% (0x298/0x298); unit 14/14 unchanged.
- Confidence: repo_proven
- Applies to/a.k.a.: any "temp widget rebuild" block (CMenuSave/CMenuCollepedia convention,
             `__ct__UnkClass_8011C974` mem-region copy); the named-local register cost generalizes to
             any view pointer that stays live across a long straight-line region.

## Raw-offset -> typed-field cleanup: byte-identical EXCEPT six load-bearing shapes (Wii/1.1 -O4,p, verified)
- Symptom:    replacing `*(T*)((u8*)base + N)` / `((char*)self + N)` with typed member/struct-field access
             normally leaves the object byte-identical (types erase at codegen), but a minority of shapes
             change instruction selection/scheduling and regress the `hexdiff --all` table.
- Cause:      MWCC lowers "running byte base, then deref" differently from "index*stride, then add"; and a
             pointer `static_cast` across a nonzero base offset emits a null-guard the retail `subi`/`addi`
             does not.
- Fix:        classify each site first, and KEEP the raw form (with a comment recording why) for these five:
                1. running-`byteOff` loops (`base = start + i*stride; ... base += stride;`) - any `arr[i]`
                   form adds a per-iteration `mulli` (100% -> 86%).
                2. `&arr[index]` / hoisted single-load reuse where retail performs two loads (no CSE) -
                   100% -> 23-43%.
                3. this-adjustment thunks / MI subobject recovery (`(char*)self - 0xNN`, `container_of`) -
                   `static_cast` emits a null-guard or changes size 0x8 -> 0x10; the raw subtract is the
                   function's purpose.
                4. integer-pointer-add operand order `(T*)((u32)roff + (u32)base)` - any pointer spelling
                   canonicalizes base-first and flips the add.
                5. probing a TU-local view block as plain source can shift a later compiler-warning line
                   citation in the `hexdiff --all` table -> declare views in the TU-local header instead.
                6. folding `&mFieldN + K` into a `field_0x(N+K)` member can split an `addi` that retail
                   shares with the next call (CfCam `CleanupVoiceEnd`, 100% -> 97.5%) - keep the raw form.
             A cast-at-use (`((View*)self)->field`) is NOT a new `self` param, but note the smell detector
             counts it: `tools/coop/smell_report.py` `self`/`void*` rows are line-based and move with any
             reformatting, so a byte-identical cleanup can *raise* those rows without adding real debt.
- Result:     one wave converted ~465 accepted-function sites across 30 TUs; every unit's `hexdiff --all`
             table stayed byte-identical. The residual raw-offset sites in accepted functions are mostly
             shapes 1-4 above.
- Confidence: repo_proven
- Applies to/a.k.a.: docs/CODE_SMELLS.md "Actionable pointer arithmetic" burn-down. `tools/coop/ptr_sites.py`
             is now class-aware (`AcceptedIndex`: qualified/chain-suffix match, unqualified last-component
             only for free functions) - the old bare-last-name lookup counted `CUICfManager::Move` (HIGH_MATCH)
             as accepted via `CTTask<12CUICfManager>::Move`.

## renderCircle-class trig loops: duplicate the angle expression per call instead of a shared idx local (CDrawGX FULL_MATCH, Wii/1.1 -O4,p)
- Symptom:   single `float idx = (float)i * (TAU/(float)sides) * K;` local feeding both `SinFIdx(idx)` and `CosFIdx(idx)` keeps idx live across the first call in a nonvolatile FPR (+`fmr` to the call reg): +1 saved FPR, bigger frame (208 vs 192), ~1.6% with only reloc-name drift left; retail instead recomputes idx before EACH trig call (two `fdivs/fmuls` sequences, cos first = right-to-left ctor-arg evaluation)
- Cause:     a named local's live range spans the call by construction so the allocator parks it nonvolatile; two textually-identical inline expressions cannot CSE-merge because the first call clobbers the volatile result reg, forcing recompute — exactly retail's shape
- Fix:       inline the angle expression into each trig argument (`SinFIdx((float)i * (TAU/(float)sides) * K)` ... `CosFIdx((float)i * (TAU/(float)sides) * K)`); do NOT factor through a helper/local (re-merges into the nonvolatile home). Pair with builtin `(float)` casts (no `frsp`, matches retail's `fsubs`-into-`fdivs` fold) and spell 2*pi as the shared-pool extern (`lbl_eu_8066A1FC`), letting the pre-existing `pool_patterns` UNIT_RULES rename the locally-pooled conversion magic to the shared slot
- Result:    CDrawGX::renderCircle 19.4% -> 100.0% FULL_MATCH (0 structural, size exact), TU 20/20 promoted to Matching
- Confidence: repo_proven
- Applies to/a.k.a.: any loop calling two trig/table functions of the same angle value where retail shows doubled angle-computation sequences; also confirms the s32ToF-union-helper deletion rule above (helper's `(float)(double)` return adds `frsp` + 2 FPRs vs the builtin fold)

## subf result lands in the minuend's web (a-b result reuses b's register)
- Symptom:   `d = a - b` (lowered `subf d,a,b`) colors `d` with the minuend `b`'s register across every
  spelling tried (compound `t -= m`, reassigned `m = t - m`, single-def `m = t - (x&k)`, anonymous shift
  `m = (x>>1) - m`): 6 shapes on OcMsgRingProbeOverflow, dest always the shift-web's reg. Getting dest
  into the subtrahend's reg requires the mirror prefix (whole chain recolors r0<->r5).
- Cause:     MWCC delivers the subtraction result into the minuend's web (dest reuses b's register), so
  dest follows whichever web holds the minuend; flipping dest without flipping the minuend's color is
  not source-steerable. The reversed source (`(x&k)-(x>>1)`) reproduces the byte stream but negates
  the value (verified numerically: v=0 differs) - a semantic trap, not a fix.
- Fix:       choose which web holds the minuend to place the subf dest (here: minuend must be the web
  that ends in the subtrahend's dead register is impossible - accept the dilemma and bank the
  prefix-correct draft); do not burn probes on reassignment-vs-compound - they color identically.
- Result:    CODE_MATCH 88.9% near-miss banked (us-8003aa6c); EQUIVALENT_MATCH unreachable (witness
  abi-boundary + bl gates), FULL_MATCH needs an allocator tie-break flip or version/flags angle.
- Confidence: hypothesis
- Applies to/a.k.a.: pure reg_swap residuals on `subf`/`subfc` where dest must reuse the dead operand.

## MWCC emits weak __dt__ in every TU that defines storage for the class (nw4r LightObj, g3d_light)
- Symptom:   TU at 23/23 100% code with exact sizes yet split-size over by 0x40: one unreferenced
  STB_WEAK `__dt__Q34nw4r3g3d8LightObjFv` in the decomp .o; retail split has room for exactly the
  23 functions. g3d_state.o (defines default ctor + a stack copy-local + a global + member arrays)
  also emits it; g3d_scnroot.o / g3d_anmscn.o (pointer params only, no LightObj storage) do not.
- Cause:     MWCC vague-linkage analog: any TU that defines storage for an object of a class with a
  user-declared (non-trivial) dtor gets a weak out-of-line `__dt__` copy, even when every
  destruction inlines away and nothing references it. Named locals, globals, member arrays, and
  union members with a nontrivial member all trigger it; references/pointers/casts do not (no
  owned destruction). Removing the TU-local `inline LightObj::LightObj()` does NOT remove it,
  but does regress codegen (4 ctor calls appear) — the inline def is load-bearing, keep it.
- Fix:       none in-source that preserves codegen (see companion entry): a named `LightObj&`/ptr
  alias stays live in a register and perturbs allocation; inline-forwarding mirrors hoist the
  derived pointer across calls (TBAA: `LightScratch*`-derived values are not invalidated by inner
  `LightObj*` calls); a union member reproduces retail's call sequence byte-for-byte but adds a
  second weak dtor (union's own, 10402 warning). Minimal residual is the pristine shape + one
  weak `__dt__` (+0x40, link-merged, unreferenced). Do NOT add extern retail-name spellings.
- Result:    g3d_light LightSetting ctor stays CODE_MATCH 100% (witness-blocked, see below);
  TU unpromotable (+0x40 split). Banked as us-803ffbb0 attempt history.
- Confidence: repo_proven
- Applies to/a.k.a.: any class with user-declared inline dtor + by-value locals where the retail
  split budget has no room for the weak dtor; sibling pattern at CTaskCulling/IWorkEvent (keep
  inline-empty base dtors in header, key-function TU owns the strong copy).

## Witness gate-2 compares (symbol, addend) pairs: byte-labeled retail pools vs TU-local pool words
- Symptom:   function 100% code, exact size, only SDA21 reloc-name drift, yet witness-gate fails
  `reloc | slot 43: lbl_eu_80669D0D@0 vs lbl_eu_80669D0C@1`. Raw relocs show why: decomp refs one
  pool word four times (`@9824+0..+3`, aggregate `{255,255,255,255}` pools one word); retail refs
  four byte-labels (`lbl_eu_80669D0C..D0F+0`) — the splitter byte-labels the shared pool's two
  words, and retail's TU owns no pool at all (all 17 pool symbols SHN_UNDEF, 0x2C pool owned by a
  data TU). Same bytes, different (symbol, addend) pairs; the map (`@9824 -> D0C`) cannot express
  1 word -> 4 byte labels, and canonicalization is symbol-only (no absolute-address compare).
- Cause:     aggregate templates pool per occurrence (white's and ambWhite's identical 0xFFFFFFFF
  words stay separate) while scalar literals merge globally; the witness needs 8 distinct decomp
  symbols to map 1:1, but any 8-symbol source shape (volatile bytes, single-byte const boxes)
  defeats pooling into immediates or re-times the loads (11-15% regressions, frame changes).
  Named 8-symbol shapes and identical codegen are mutually exclusive here.
- Fix:       none in-scope: extern pool-name spellings are forbidden (§17.6), tools/ edits are
  forbidden, --smt/--linked are forbidden paths. Tools-side fix would be absolute-address
  comparison in gate-2 (resolve both sides through the link layout: D0C+1 == D0D+0). Until then,
  record as residual and accept the CODE_MATCH ceiling.
- Result:    us-803ffbb0 CODE_MATCH ceiling documented; TU left NonMatching.
- Confidence: repo_proven
- Applies to/a.k.a.: any function whose literals live in a shared external pool that the splitter
  byte-labels (sdata2-heavy ctors); check raw retail .o rela (build/us/obj/...) for SHN_UNDEF
  pool symbols before burning probes on pool rescheduling.

## TU-local inline getters steer MWCC tails scheduling + saved-reg colors (GC/3.0a5.2, -O4,p)
- Symptom:   big function at ~88% with a scrambled 15-op tails window (loads/ALU/stores
  permuted, all ops present) plus a saved-GPR color rotation, all statement-order tweaks
  neutral (scheduler attractor). Bare-decl order fixes FPR colors (Rule A) but not the
  interleave; GPR decl/assign/type/position sweeps all neutral or worse.
- Cause:     retail reads several members through inline accessor boundaries (extra inline
  PCode nodes change vreg numbering/graph → different schedule + colors), while the decomp
  reads them direct. Direct reads fold to identical loads, so the shapes look the same but
  schedule/color differently.
- Fix:       mirror the hypothesized accessors as TU-local inline getters (subclass or view
  methods; inlined away, no extra calls/mrs) and call them at retail's positions. Proven
  via .scratch MWCC probes (GC/3.0a5.2, exact TU flags, full-fidelity replica incl. real
  headers): WsdPlayer::UpdateChannel 87.8% (10 ST + 26 RS) -> 99.3% (0 ST + 2 RS); the two
  remaining lbz values are emission-order-swapped with identical regs (all call-order,
  decl, type, separation variants exhausted).
- Result:    us-8042d158 HIGH_MATCH 99.3% near-miss (witness-blocked by bl/psq, FULL only).
- Confidence: repo_proven
- Applies to/a.k.a.: any function whose tails schedule resists statement surgery — check
  whether member reads go through getters retail-side (compare inline-boundary PCode via
  probe); vol-first-assigns + scheduler-hoists-pitch-above-vol also general (instruction
  order != source order for independent chains); MWCC reciprocal-opts `/64.0f` to
  `*0.015625` (pool) but keeps `/127.0f` as fdivs (GC/3.0a5.2).

## `!!(x & mask)` single-mention booleanization fixes scratch-color rotation vs duplicated `(x|-x)>>31` (Wii/1.1)
- Symptom:   pure reg_swap residual (0 structural), a cyclic rotation of 3 scratch regs
  (retail r4/r5/r3 vs decomp r3/r4/r5) across a duplicated extraction block, while
  trigger/branch/call/final-bool regs all match. Declaration-order swaps neutral,
  assignment-order swap worse (+4 swaps).
- Cause:     hand-written `(u32)(-(s32)(x) | (x)) >> 31` mentions the mask twice per
  statement; CSE merges them but the duplicate mention changes virtual-register birth
  order vs retail, rotating scratch colors (Rule C: temp-count/birth, not Rule A decl
  order). Retail's `!!` lowering emits the identical neg/or/rlwinm(>>31) sequence from
  a single mention, with retail's birth order.
- Fix:       write the booleanization as `dst = !!(word & MASK);` (single mention).
  Keeps the exact neg/or/shift instruction stream and restores retail scratch colors.
- Result:    TitleMenuHandleInput (kyoshin/menu/CMenuTitle) 71.4% + 22 reg_swap -> FULL_MATCH 100%
- Confidence: repo_proven
- Applies to/a.k.a.: any `!=0`-to-`u32` conversion showing neg+or+rlwinm(1,31,31) with
  rotated scratch colors; prefer the `!!` idiom over manual `(-x|x)>>31` whenever the
  residual is pure reg_swap.

## op31_922 is extsh with a phantom third operand; each (s16) cast is one extsh (Wii/1.1)
- Symptom:   hexdiff prints `op31_922 rD, rA, r0` inside an `(s16)(... % 8)`-style
  computation; reads look like an undefined register (e.g. retail reads r6 though only
  r5 was loaded). Single-cast rewrite drops one op31_922 and goes structural + smaller.
- Cause:     op31_922 XO 922 = `extsh`; the printer dumps it as 3-operand
  (src, dest, phantom r0), so `op31_922 r4, r6, r0` is really `extsh r6, r4`
  (extend r4 into r6) and the dataflow is sound. Each explicit `(s16)` cast in
  `(s16)((s16)(tail + 1) % 8)` emits one extsh (inner narrowing + outer narrowing);
  the sibling pump function matches 100% with the same double-cast idiom.
- Fix:       keep the double `(s16)((s16)(...) % 8)` cast to preserve both extsh
  instructions; do not "simplify" to a single cast. A leftover pure r5<->r6 scratch
  swap between the tail web and the extsh-dest web (0 structural, same size) is
  witness-eligible in a bl-free leaf — cycle certifies EQUIVALENT_MATCH.
- Result:    monolib CNand func_804DA47C 64.7% (0 ST + 6 RSW) -> EQUIVALENT_MATCH
  (register-renaming witness, rho = r5<->r6 swap), size-exact 0x44/0x44.
- Confidence: repo_proven
- Applies to/a.k.a.: any `(s16)` arithmetic followed by `%`/`/` power-of-two lowering
  (srawi/addze/rlwinm/subf/extsh); when hexdiff shows an op31_* reading a
  never-loaded register, re-read it as (src, dest) before theorizing about args.

## Factory create() with placement-new: r29<->r31 name/obj swap is witness-closed (abi-boundary) — do not chase, record near-miss (monolib CWorkSystem::create, Wii/1.1 -O4,s)
- Symptom:   factory `create()` (getInstance -> getWorkMem -> allocate -> placement-new -> entryWork) sits at 80-99% with 0 structural / 6 reg_swap, size-exact: retail holds (parent=r30, name=r31, obj=r29), decomp holds (parent=r30, name=r29, obj=r31). Pure color swap of the string-name web vs the object web.
- Cause:     the r29<->r31 permutation touches callee-saved registers saved/restored by the stmw/lmw r29 prologue/epilogue, so the register-renaming witness closes with `witness-gate: abi-boundary | rho perm maps gpr r29 -> r31; ABI registers must be fixed` (equivalence=inconclusive_smt_disabled). Degree theory fits the decomp coloring exactly (use-count obj=4 > parent=2 > pName=1 colors first-claimed-first to r31/r30/r29) but retail colored the single-use name web FIRST — unreachable from declaration order (Rule A), statement order, or keeping-uses under the size-mode pragma.
- Fix:       none from source (negative result). Ruled out: nested single-statement placement-new (neutral), trailing (void)name keeping-use (DCE'd, neutral), decl+statement reorder (frame/schedule collapse), split raw/obj webs a la CWorkSystemPack registerArcFile (neutral here), pName-init-first (right colors, constant hoists above the getInstance bl = structural), uninit/null-init/dead-def reassign (obj stuck r31), explicit if-guard (duplicate beq vs placement-new auto-guard). Remaining angle is TU-wide flag negotiation (-use_lmw_stmw at -O4,p rematerializes the name after the call, plausibly yielding 31/30/29) — owner decision, risks rescheduling matched bodies.
- Result:    CWorkSystem::create (us-80447194) near-miss at 80.0% code / 99.0% instruction, size 0x78==0x78, CODE_MATCH; stall_note in attempts.jsonl.
- Confidence: negative_result
- Applies to/a.k.a.: any create-style factory whose residual is a pure saved-reg swap involving a stmw/lmw-saved register — check the witness gate FIRST (cycle once); if abi-boundary, stop source experiments and record. Companion wall: -func_align 16 inter-function padding (~100B over 14 gaps here) can fail the TU split-size gate even when every function is size-exact — verify with per-function offsets, retail used func_align 4-or-less layout.

## lyt_common DrawQuadUc — inline-dtor/inlining fixed point + weak-split catch-22 (Wii/1.1 -O4,p -inline auto -ipa file)
- Symptom: `DrawQuad(VEC2,Size,u8,TexCoord,Color,u8)` (us-8032f920) sits at 57.9% / 0 structural / 32 reg_swap / size-exact: a 3-value rotation (colorWork-ptr/pColors-copy/loop-index across r31/r12/r11) plus mirrored spill slots (8<->12, 11<->15). Hoisted `int i`, explicit `Color()` init temps, `++i` are all byte-identical neutral; the loop index is dead (li/addi, never read) yet occupies r11 retail vs r31 decomp.
- Cause: two self-consistent fixed points. (a) With TU-local `inline Color::~Color(){}`, the inlined MultipleAlpha body stays small so `-inline auto` fires, the Color prvalue constructs in place into the array (byte stores), and all destruction vanishes — but MWCC emits weak `__dt__Color` (+0x40 .text) which overflows the split (0xE80->0xEC0) and blocks both promotion and EQUIVALENT gates. (b) Without the visible dtor (Color or u32 buffer), the call site exceeds the auto-inline budget: MultipleAlpha goes out-of-line (real bl) + temp/array `__dt__` calls appear (1.1-1.3%, ~70 structural). Per-element assignment init is likewise 88-structural. Callee-side named locals/params need no dtor calls when the dtor is invisible (DrawLine by-value Color param matches 100% with no dtor), but caller-side temps and array elements do.
- Fix: none in-scope (recorded soft-cap). Retail lyt_common.o carries no `__dt__Color` symbol or reference, so its buffer is POD-shaped, yet no POD spelling reproduces the in-place byte construction honestly; do not fake it with manual byte stores. Retail function order (EqualsPane first, DrawLine after DrawQuadUc) restored with zero regressions to the other 15/16.
- Result: ACTIVE near-miss 57.9% (us-8032f920); TU stays NonMatching on split-size.
- Confidence: negative_result
- Applies to/a.k.a.: any TU where a visible-empty inline dtor is load-bearing for inlining (lyt_pane DrawSelf wall is the companion case); witness Gate-3 rejects spill-offset differences, so mirrored stack temps are FULL-only. Next angles if revisited: u8/unsigned index type, while-loop form.

## OSFont Decode (Yay0) — volatile-birth positional rule + abi-boundary witness wall (RVL_SDK os, Wii/1.1 -O4,p -ipa file)
- Symptom:   static Yay0 `Decode` sits at 44-55% with 0 structural / 48-59 reg_swap, size-exact: retail holds chunk->r29/maskPtr->r30 (saved) with j/linkOfs/offset in r6/r7 (volatile); naive decomp holds chunk->r6/maskPtr->r7 (volatile, declaration-locked births 1-2).
- Cause:     (a) Volatile coloring is positional: first 7 non-expandSize declaration births take r6-r12 in order (expandSize always r0; short-lived 8th birth skipped in favor of the next long-lived); verified by NullA (linkOfs/count swap identical), Exp1 (j+count to top shifts all), P1/P2 (7th-birth takes r11, r12 goes to most-deserving remainder). Init-statement order must stay (scheduler follows source order for prologue loads), so births 1-6 (chunk/mp/exp/i/linkTbl/mskBits) are locked and FULL is unreachable: any init-birth move (F1/S2) scrambles colors globally AND flips prologue scheduling (2 structural, vnum tie-breaks). (b) Witness Gate 5 fixes r26-r31 here (live-in via _savegpr_26 helper save; no inline-stw spill carve-out) and retail r6/r7 host 2+ webs each (bytes/chunkPos/j; linkP/linkOfs/offset), so no global rho exists; region-slicing cannot rebind loop-carried lanes across unbounded data-dependent outer+ctr loops. Cycle fails `witness-gate: abi-boundary` (first r11->r10, then r26->r30).
- Fix:       best-known shape: declare `s32 count;` before `u32 maskBits = 0;` (pure-decl move, statements untouched) -> count->r10, maskBits->r11, mask->r12 (54.7%, 0 structural). Check the witness gate FIRST (one cycle run): if abi-boundary on a locked birth, stop - no declaration/expression/type lever can move init-locked births without flipping scheduling.
- Result:    Decode (us-8035a4d0) ACTIVE near-miss 54.7% (48 reg_swap, 0 structural, size 0x1a8==0x1a8, split PASS); TU stays 12/13 (configure already Matching, left as-is).
- Confidence: negative_result
 TU flag negotiation (breaks 12 FULL).

## MWCC `entry` labels are size-0 NOTYPE symbols: cycle cannot pair them despite byte-identical units (RVL_SDK OSInterrupt RAS markers, Wii/1.1)
- Symptom:   `cycle` FAILs with `equivalence: inconclusive_unsupported (ambiguous decomp symbol '<label>' ... (0 candidates))` while reporting code 100% / data 100% / fuzzy 100%, hexdiff `--all` 11/11 green, and split-size PASS. Target is an `__RAS_*_begin/end` (or `__OSDBINTSTART`/`__OSEVStart`-style) `entry` label inside an `asm` function body.
- Cause:     MWCC emits `entry` labels as STB_GLOBAL/STT_NOTYPE/size-0 symbols (verified via ELF symtab: `__RAS_OSDisableInterrupts_begin` GLOBAL/NOTYPE val 0x0 size 0 at .text+0x0, `_end` at +0xc). `tools/ppc_equivalence/elf_symbols.py:list_text_functions` skips `st_size == 0`, so `extract_function` finds 0 decomp candidates and the witness gate never runs. Retail reconstruction sizes the label from symbols.txt ranges (here 0x14 = exactly the 5-insn `OSDisableInterrupts` body), so the asymmetry is inherent to raw-compiler-output vs reconstructed-retail, not to source shape.
- Fix:       none from source (negative result) — do NOT touch the asm block, add stubs/padding, or split functions to chase symbol sizes. Run ONE `cycle` to confirm the `0 candidates` gate, verify the label address/size coincides with its enclosing function body, then record a stall_note and move on. The real fix is tooling-side: resolve size-0 global labels via enclosing sized-function bounds (or alias to the co-addressed sized function) in `extract_function_pair`, then re-cycle with no source change.
- Result:    `__RAS_OSDisableInterrupts_begin` (us-8035b720) near-miss at 100% instruction / 0 structural / 0 reg_swap, size-exact; TU stays `Object(Matching)` (already promoted, 11/11 FULL bodies + size PASS).
- Confidence: repo_proven
- Applies to/a.k.a.: any `entry`-label registry target (`__RAS_*`, OS boot-vector `entry` labels `__OSDBINTSTART`/`__OSDBJUMPSTART`/`__OSEVStart`, `OS.c` `__DBVECTOR` family); symptom key is `(0 candidates)` paired with 100% code.

## CTTask factory ctor: vtFinal-between-loads + volatile-pointer PTMF base (CfTaskMain, Wii/1.1 -O4,p)
- Symptom:   self-allocating factory ctor (`__ct__800697E8`, CTTask<CfTaskMain> + secondary IWorkEvent vtable) at 68.6% (6 structural, 10 reg_swap): decomp materialized the final vtable early (`lis r5` before any PTMF load, holding it in r5) where retail interleaves `lwz r4,4(r6) / lis r3 / lwz r5,0(r6) / addi r3` (vtable reuses r3 after the interim CTTask-vtable store) and computes `evt = vt+36` after the first callback store.
- Cause:     (a) a `u32 vt` local declared before the interim-vtable store stays live across that store, so the allocator gives it a fresh register (r5) and hoists its lis above the PTMF loads; retail's reuse of r3 proves the final vtable is first named after the interim store. (b) direct `__ptmf_null[i]` indexing lets MWCC fold the `@l` part into the offset-0 loads (`lwz rX,sym@l(rHI)`) and split the base across two registers (extra decomp-side-only `R_PPC_ADDR16_LO __ptmf_null` relocs); retail issues one `addi r6,r6,@l` plus plain loads off a single base, which is the shape pointer-based reads produce.
- Fix:       mirror CUICfManager::Init (CUICfInitProcess): typed ctor shim (`void* vtable; u32 moveCallbacks[3]; u32 drawCallbacks[3]; void* evtVtable; u32 field58`), interim `s->vtable = lbl_temp;` store first, PTMF read through `volatile u32* ptmf = (volatile u32*)(u32)__ptmf_null`, `u32 ptmfWord1 = ptmf[1];` then `char* vtFinal = lbl_final;` then `u32 ptmfWord0 = ptmf[0];` (vtFinal named between the [1]/[0] loads), per-slot named temps (`ptmfWord2 = ptmf[2]` before each `[2]` store, reload both words for the second slot), finish with `s->vtable = vtFinal; s->evtVtable = vtFinal + 0x24;`. Labels declared `char lbl[]` so the assignments need no casts.
- Result:    FULL_MATCH 100.0% (0 structural, 0 reg_swap), size-exact; TU 11/11 promoted to Matching.
- Confidence: repo_proven
- Applies to/a.k.a.: any CTTask/CProcess self-allocating factory with null-PTMF callback triples (CUICfManager, CCol6CheckBat, CTaskGameEffAfter family); diagnostic keys are `lis` before vs between the PTMF `lwz` pair and decomp-side-only `@l __ptmf_null` relocs.

## -O4,p 8x-unrolled byte-copy tail: loop-local shapes cannot flip tail temp colors; r5/r6 ABI swaps are witness-uncertifiable (RVL_SDK bte sdp_db, GC/3.0a3.4 -O4,p -func_align 4 -ipa off)
- Symptom:   SDP_DeleteAttribute (us-80307104) stalls at 95.8% / 0 structural / 6 pure reg_swap, size-exact (0x23c/0x23c, split PASS): the -O4,p 8x-unrolled memmove tail preheader+loop swaps exactly two temps retail idx16->r6/src->r5 vs decomp idx16->r5/src->r6 (`rlwinm/add/subf/cmpl/lbz/addi`); everything else in the TU (9/10) is FULL.
- Cause:     (a) The loop shape alone pins the colors: a 16-variant .scratch probe sweep (for/while/do-while/empty-update/update-swap/prefix/init-decl/named-byte-temp/deref spellings/cond-direction, exact TU flags) compiles byte-identical 8x-head+CTR-tail with idx-lower/src-higher every time, so no loop-local source lever (declaration/expression/statement order, Rule A/B/C) reaches the unroller's internal tail-temp creation order. (b) The swap lives on call-argument registers (r5/r6 = LogMsg_2 arg1/arg2 at the in-function trace call, whose setup matches), so no global rho exists: `cycle` fails `witness-gate: abi-boundary | rho perm maps gpr r5 -> r6`, SMT is disabled, and EQUIVALENT_MATCH is closed -- FULL_MATCH is the only acceptance path. Named advancing src IV and do-while both destroy the unroll (structural/size blowup); int index type destroys the CTR/cmpl tail idiom (u16 index pinned by the rlwinm masks).
- Fix:       none found in-scope (recorded soft-cap with a concrete next angle). Check the witness gate FIRST with one `cycle`: if abi-boundary on r5/r6-class regs, stop color-chasing and go FULL-only. Next angle if revisited: full-context probe bisection -- replicate the whole function (global sdp_cb struct, 664-stride record walk, attr-find with break, 12B struct shift loop, u16 cnt computation with casts, writeback) in .scratch and add context features one at a time until the tail flips to src-lower, then port that feature back (probes: .scratch/probe_tail.py, .scratch/probe_ctx.py).
- Result:    SDP_DeleteAttribute ACTIVE near-miss 95.8% (0 structural, 6 reg_swap, size-exact, split PASS); TU 9/10, configure.py already Matching (left as-is).
- Confidence: negative_result
- Applies to/a.k.a.: any -O4 loop-unrolled copy tail whose residual is a 2-temp volatile swap overlapping call-arg regs (witness Gate abi-boundary); diagnostic keys are `witness-gate: abi-boundary` + probe-sweep invariance. Contrast OSFont Decode (init-locked volatile births) and CTTask (visible-dtor inlining) walls.

## MWCC struct-assign lowering is member-type-driven: FogState (f32 members) goes member-wise (FPRs), u32/union view goes word-wise (GPRs) (monolib effect, Wii/1.1 -O4,p)
- Symptom:   `resolveEffectFog` (us-804dc6c0) initial work-image copies: retail moves 2x0x30 bytes with grouped 12x `lwz` + 12x `stw` via GPRs only (r16-r27, r28-r31 + scratch, `_savegpr_14`); naive `FogState work[2]; work[0] = saved; work[1] = tmp;` lowers member-wise (`lfs`/`stfs` for the 4 float words through FPRs, `lwz`/`stw`/`lbz`/`stb`/`lhz` for the rest), pinning floats live and cascading into saved-FPR prologue (`stfd`+`psq_st` f27-f31), `_savegpr_17`, and a shifted frame.
- Cause:     MWCC lowers struct assignment by member type, not as a blind block copy: any struct with `f32` members emits `lfs`/`stfs` for those words (verified by minimal probes `copyA` vs `copyB` under exact TU flags, plus the matched `Trail_RestoreFog` reference which is member-wise for the same reason). A `union { FogState fog; u32 w[12]; }` (or plain u32-struct) view lowers the same bytes word-wise (`lwzu`-led interleaved `lwz`/`stw`, GPRs only, no FPRs) in isolation and under float pressure. Same result under GC/3.0a5.2, so it is not a version lever.
- Fix:       for retail word-wise copies, copy through a `u32` view of the struct. Caveats from this target (all reproduced): (a) raw `u32*` pointer copies alias-block the scheduler and regress (0.3%); (b) a function-local `union FogWork` with `.fog` member accesses scatters stack slots and blows size (OVER budget) -- the union member access appears to duplicate/spill the image; (c) element-wise `.w[i]` copies shrink the frame (lost spills) and still sit at 0.3% because the 8 downstream normalized colors stay in regs instead of spilling to stack. The word-wise copy is necessary but not sufficient: it must be paired with a formulation that keeps the downstream float live-ranges stack-spilled (still open; scheduler hoists norm/work-float reloads far above uses regardless of source order -- tried select-before/after norms, dst-before/after color, per-channel scalars, volatile arrays).
- Result:    ACTIVE near-miss 3.5% (us-804dc6c0); copies still member-wise in the kept shape. Companion lever that DID pay: inverted float select (`if (blend < C) ... else ...` gives `fcmpo`+`bge`; naive `>=` gives `fcmpo`+`cror`+`bne` with different NaN semantics).
- Confidence: repo_proven (probe + matched-reference verified; full-function word-wise still open)
- Applies to/a.k.a.: any struct with float members whose retail copies are word-wise (fog/fade images, matrix/quaternion work areas); diagnostic keys are `lwz`/`stw`-only copy windows vs `lfs`/`stfs` mix plus `_savegpr_N` one-step shifts. Contrast Trail_RestoreFog (member-wise is correct there).

## AXFX Dpl2 IIR-mix idiom: `*inBus[ch]++` in-subscript pins hoist order + temp colors (AXFXDelayExpDpl2 callback, Wii/1.1 -O4,p)
- Symptom:   `AXFXDelayExpCallbackDpl2` (us-802d9f00) stalled at 88.8% with 15 reg_swap and 0 structural: decomp hoisted `&input[0]` into r6 before `&inBus[0]` into r7, retail the reverse (`addi r6,sp,24=inBus` first), cascading into an r24/r25/r26 rotation through the whole busIn path; the witness gate refuses (`fields | slot 48: non-register bits differ`) because the `addi` pair differs in immediates, so only FULL_MATCH can accept.
- Cause:     three transcription inversions vs the family idiom (confirmed against FULL_MATCH siblings `AXFXDelayExp.c`: `mixedL = invCoef * (*inputL + *inL++) + coef * fx->last[0]` and `AXFXChorusExpDpl2.c`: `data = (f32)(*input[ch] + *inBus[ch]++)`): (a) add operands written `*inBus + *input` instead of `*input + *inBus` -- fixing this alone moved 88.8% to 94.0% and fixed the r6/r7 hoist order; (b) a named `s32 in` temp + separate `inBus[ch] += 1` statement instead of post-increment inside the subscript -- folding the temp changed nothing, but moving the bump into the subscript (`*inBus[ch]++`) renumbered the IR temps to retail's coloring with zero instruction movement. Expression-order-only levers (mult operand swap `coef*last + invCoef*in` even though the else-branch is invCoef-first) were fully neutral: MWCC's scheduler is availability-driven and emission order is sticky, so color follows IR node shape, not operand order.
- Fix:       write the busIn mix exactly as `mixed[ch] = invCoef * (*input[ch] + *inBus[ch]++) + coef * fx->last[ch];` (invCoef-first, input-first, `++` inside). Do NOT use a named intermediate or a separate bump statement.
- Result:    FULL_MATCH 100.0% (0 structural, 0 reg_swap, 0x218/0x218); TU 7/7, size PASS, configure.py already Matching.
- Confidence: repo_proven
- Applies to/a.k.a.: any AXFX Exp/Dpl2 callback with the coef/invCoef IIR busIn mix (Delay/Chorus/Reverb families); diagnostic keys are hoist-order `addi` pairs with swapped immediates + temp-rotation-only residuals in one branch.

## Ctor 3-temp scratch rotation with base-ctor bl: init-list spellings are color-neutral, witness abi-boundary closed (nw4hbm lyt_arcResourceAccessor, Wii/1.0a)
- Symptom:   ctor `__ct__Q36nw4hbm3lyt19ArcResourceAccessorFv` (us-8032ebc0) stalls at 57.1% with 0 structural / 9 pure reg_swap, size-exact 0x54/0x54: a fixed 3-way volatile rotation on {vtable ptr, &mFontList.mNode, NULL-zero} -- retail vtable->r4/node->r5/null->r0 vs decomp vtable->r5/node->r0/null->r4 (lis/addi/li/addi/stw x4 window after the base-ctor `bl`).
- Cause:     (a) the three webs pairwise interfere (live ranges overlap across the store window) and are colored low->high from the scratch mask in reverse virtual-register birth order, so the residual is a birth-order difference: retail births &mNode first, decomp births vtable first. Member-init codegen order is fixed by declaration order regardless of init-list spelling, and the scheduler pins the emission order, so no .cpp spelling renumbers the births. (b) The base-ctor `bl` keeps call-argument registers live across the window, so no global rho exists: `cycle` fails `witness-gate: abi-boundary | rho perm maps gpr r4 -> r5`, SMT is disabled, and EQUIVALENT_MATCH is closed -- FULL_MATCH is the only acceptance path.
- Fix:       none found in-scope (recorded soft-cap). Run ONE `cycle` first: if the gate reports abi-boundary on call-arg regs, stop color-chasing and go FULL-only. Do NOT repeat init-list order / placement-value / base-call-form / implicit-member / value-init probes (all byte-identical here). Off-limits next angles: member decl reorder (breaks offsets in matched sibs), shared-header Initialize_ reorder (perturbs every LinkList TU), per-TU flag/mw_version change (perturbs matched sibs), comma-vptr manual store (fake decl + doubled store).
- Result:    ACTIVE near-miss 57.1% (0 structural, 9 reg_swap, size-exact); TU 5/6, raw split OVER covered by grandfathered UNIT_RULES drop_text_symbols + repack16; configure.py already Matching left as-is.
- Confidence: negative_result
- Applies to/a.k.a.: any ctor whose residual is a volatile 3-temp rotation behind a base-ctor `bl` (vtable/member-addr/zero triples); diagnostic keys are `witness-gate: abi-boundary` + init-spelling i

## Search-and-shift-down dtor loop: 3-way volatile rotation frozen by birth-order tie-break; inline-array header + global refs + align-4 are the structural levers (monolib UnkClass_8045F564 dtor, Wii/1.1)
- Symptom:   `~UnkClass_8045F564` (us-80463550) stalls at 84.4% / 0 structural / 10 pure reg_swap, size-exact (0x100/0x100, split PASS 0x2F4): a fixed cyclic rotation retail {shared index->r4, byteoff induction->r5, inner lib reload->r6} vs decomp {->r6, ->r4, ->r5}; everything else byte-identical (CTR outer search loop, hoisted `count-1` bound, add-form inner addr, second SDA `lbl` reload, trailing `__dl__FPv` delete-this). The `cycle` witness refuses (`abi-boundary | rho perm maps gpr r4 -> r6`; r4 is a call-arg reg), so EQUIVALENT_MATCH is closed -- FULL_MATCH only.
- Cause:     (a) the three webs pairwise interfere through the inner loop, and MWCC colors them in an order no source lever in-scope perturbs: 12+ variants (local/global lib anchor fully CSE to identical bytes; inner for/while; separate inner index coalesces; decl order; int-vs-u32 index; named swap tmp; named/for-init/top count bounds) are either byte-identical or structural/bloat. (b) Three REAL structural levers were found and kept: the TU must see the inline `instanceArray[32]` layout (local `src/lib/CLibLayout.hpp`, not the public pointer-typed header -- pointer semantics emit indexed `lwzx`, 40.9%); the TU must drop its anomalous `-func_align 16` (default align 4; 16 inserts pre-loop `ori r0,r0,0` NOPs and inter-function padding that hid the true diff and broke the exactly-full split); the inner shift must be a `for` (bound hoists to the preheader with the lib reload; `while` reloads `count` per iteration). (c) Global lib refs in the inner loop are REQUIRED: the outer induction consumes the base register, so the inner lib must rematerialize via a second SDA load (reloc-set parity needs exactly 2 `lbl` loads).
- Fix:       keep nested `for` loops with all-global inner refs + `lbl->instanceCount--` + `break` under the local inline-array header and default `-func_align 4`. Do NOT repeat the ruled-out color variants above (all byte-identical or worse); do NOT add named bound locals (stack bloat to 0x1CC / +1 insn breaks the 0-spare budget).
- Result:    ACTIVE near-miss 84.4% (0 structural, 10 reg_swap, size-exact, split PASS); TU 5/6, configure.py left NonMatching with the per-unit align fix kept (retail TU is packed). Next angle if revisited: unit mw_version sweep or human-sanctioned -O4,s probe with 5-function regression check.
- Confidence: negative_result
- Applies to/a.k.a.: any nested search-and-shift loop whose residual is a volatile 3-temp cyclic rotation overlapping call-arg regs (witness gate abi-boundary); diagnostic keys are `witness-gate: abi-boundary` + variant-sweep invariance + a matched same-idiom sibling with the common coloring (here `CLibHbm::removeCallback`, FULL_MATCH with index->r6, proving this retail's index->r4 is the odd one). Contrast ctor 3-temp rotation (same gate, init-order lever class) and unrolled-copy-tail swaps (same gate, full-context bisection lever).

## MWCC `A | B` evaluates RHS-first with home=B; narrow loop counter needs explicit (s8) casts for int-width tail use (nw4r GDSetIndTexMtx, GC/3.0a5.2 -O4,p -ipa file)
- Symptom:   bitfield word-build `word = slice | idshift` roots at the id-shift (`slwi` + `rlwimi`-inserted slice) while retail roots at the slice (`rlwinm` + `rlwimi`-inserted id) with a final `or`; separately, an `s8` loop counter emits `extsb` on every promotion so `u32 exp = scale+17` mismatches a retail plain `addi`, yet `s32` scale breaks the loop's own `extsb+cmpwi` bound checks and scale's register.
- Cause:     (a) MWCC evaluates `A | B` RHS-first: B is materialized into a fresh reg (the OR home) and A is folded in via `rlwimi`-insert (proven over 4 scratch probes: `(E|I)`->id-root, `(I|E)`->exp-root, multi-stmt `X=I;X|=E`->exp-root, `X=E;X|=I`->id-root; same rule picks the payload inner home, so `(Wlo|Wshi)` roots at Wshi with Wlo inserted). (b) `s8` promotion always emits `extsb` (u32/s32 target, return, masked-inline+CSE all keep it), while loop `scale>=46`/`scale>-17` REQUIRE `extsb+cmpwi` (retail has them) and the tail `addi` requires none -- one variable cannot satisfy both unless the type is split.
- Fix:       (1) spell bases as `((id+N)<<24) | (slice)` (id-part FIRST in source) and payloads as `base | ((lo-part) | (hi-part))` (inner lo-FIRST in source) to land exp-root + Wshi-home + final-`or` exactly. (2) declare the counter `s32` and cast at the loop checks -- `if ((s8)scale >= 46)`, `} while (... && (s8)scale > -17);` -- so checks keep `extsb+cmpwi` (+identical regs) while `u32 exp = scale+17` is a plain `addi`. (3) when float colors are right but two loads emit swapped, split declaration (allocation order) from assignment (emission order): `f32 m00,m01,m10,...;` then assigns in retail emission order -- fixes emission with zero color drift. Fused `(s32)(scl*m)` conversions must stay fused (named float temps get fresh regs and break `fmuls` colors; scheduler separates fused fmuls/fctiwz on its own).
- Result:    GDSetIndTexMtx us-803e98ec 72.9% -> 80.1% (13 structural + 20 reg_swap left, size-exact 0x298/0x298, split PASS); head/loops/word0 byte-identical. Residual is a spill-scheduling tie (stfd-w11 vs fctiwz-w12) insensitive to statement order/node naming -- see stall_note in attempts.jsonl.
- Confidence: repo_proven
- Applies to/a.k.a.: any `((id)<<24)|((exp)&3)<<22`-style XF/GP fifo word builder; any s8 counter feeding both compared and widened uses; any load-order swap with correct colors. Example: us-803e98ec.

## `-(x != 0)` branchless 0/-1 return: neg/or/srawi peephole (CriWare mpv_deli, GC/3.0a5.2)
- Symptom:   `criware_803A59B0` (us-803a914c) at 52.6% with 9 structural, size OVER by 8: retail tail is `neg r0,r3; or r0,r0,r3; srawi r3,r0,31` (branchless `(x|-x)>>31` = 0 if x==0 else -1) while `if (memcmp(...) != 0) return -1; return 0;` emits `cmpi; bc; li -1; b; li 0`.
- Cause:     MWCC peephole-lowers `-(x != 0)` directly to the neg/or/srawi triple without materializing the boolean; any if/else or ternary spelling takes the branch form.
- Fix:       write the return as `return -(memcmp(table, lbl_eu_8051C090, 256) != 0);`.
- Result:    FULL_MATCH 100.0% (0 structural, size 0x44/0x44); TU 5/5, size PASS, promoted to Matching.
- Confidence: repo_proven
- Applies to/a.k.a.: any 0-or-(-1) status return derived from a nonzero test (memcmp/strcmp-style self-check stubs); diagnostic key is a retail `neg; or; srawi *,*,31` tail vs decomp `cmpi/bc/li` branch.

## Extern-owned static-init: empty init-guard struct regenerates the auto-`__sinit_` backslash form with no storage and no UNIT_RULES (ut_IOStream, GC/3.0a5.2)
- Symptom:   `__sinit_\ut_IOStream_cpp` (us-8042e154) showed hexdiff 100% (0 structural, 0 reg_swap, 0xC/0xC, identical SDA21 relocs) via a hand-written `extern "C" void __sinit_ut_IOStream_cpp()` (underscore name, global linkage, no `.ctors`), yet `cycle` failed `COMPILES` with `ambiguous decomp symbol (0 candidates)` -- objdiff pairs symbols exactly, so the hand-written underscore name can never pair with the retail backslash name, and no postprocess `exact_renames` key may be added (UNIT_RULES freeze is shrink-only).
- Cause:     MWCC emits the `__sinit_\<tu>_cpp` backslash form (local linkage + `.ctors` entry) ONLY for compiler-generated static init; a hand-written function keeps its C spelling and creates no `.ctors`. Retail `ut_IOStream.o` is `.text` + `.ctors` with the init storing NULL to the UND `lbl_eu_80665540` (storage owned by `ut_FileStream.o`'s `.sbss`, which is already a promoted Matching TU -- moving the definition is off-limits). The init therefore must come from a static initializer with no emitted storage of its own.
- Fix:       declare the label `extern "C"` and add a TU-local empty guard struct with an inline side-effect ctor plus one static instance: `namespace { struct IOStreamTypeInfoInit { IOStreamTypeInfoInit() { lbl_eu_80665540.mParentTypeInfo = NULL; } }; static IOStreamTypeInfoInit s_ioStreamTypeInfoInit; }`. MWCC inlines the one-store ctor into the auto-`__sinit_` (`li r0,0; stw r0,X@sda21; blr`), drops the 1-byte empty-class storage (no `.bss`/`.sbss`/`.data` emitted), adds the `.ctors` entry, and leaves the `lbl_eu_80665540` reloc UND -- the decomp symbol table then mirrors retail exactly. Keep the edit line-neutral (same line count above the matched functions): `-ipa file` codegen is line-sensitive and shifts can regress accepted siblings.
- Result:    FULL_MATCH 100.0% (0 structural, 0 reg_swap, 0xC/0xC, no reloc drift), semantic-certified; TU 5/5, code+data 100%, split PASS, promoted to Matching. First-attempt fix, no revert needed.
- Confidence: repo_proven
- Applies to/a.k.a.: any TU whose retail `__sinit_` stores through an UND extern with no TU-owned data sections (splitter-assigned or sibling-owned storage); supersedes the old "hand-write the sinit + exact_renames safety net" advice (hand-written names never pair; new rename keys violate the freeze). Diagnostic keys are hexdiff-100%-but-cycle-`0 candidates` + retail `.o` having `.ctors` with no data sections + decomp `.o` missing `.ctors`.

## Param-save rotation via a late-declared walk-pointer copy: `char* p = data` declared after the other locals (CriWare sud_lib SUD_SearchSudDat, Wii/1.1)
- Symptom:   pure `reg_swap` residual (0 structural, exact size) where the four param-save colors are rotated one slot: decomp `(data,id,outOffset,outSize)->(r25,r26,r27,r28)` vs retail `(id,outOffset,outSize,data)->(r25,r26,r27,r28)`. Statement/guard/store/increment reorderings and while/for loop-form swaps are all color-neutral (verified: 6 scratch variants, identical saves) -- the rotation is NOT order-driven and the witness refuses it (abi-boundary: the 4-cycle rho touches call-argument flow into `bl`, so only FULL_MATCH can accept).
- Cause:     MWCC colors first-declared locals into the highest saved registers (Rule A) and params fill the remaining slots in signature order. A fresh `char* p = data` copy (data dead after the copy + guard, so the webs coalesce to one 7-web/7-reg frame) declared AFTER `table`/`i` takes the lowest local slot (r28), pushing nothing else: table=r30, i=r29, p=r28, id/outOffset/outSize=r25-r27. Declaring the copy first (or walking the param directly) puts the walked pointer at r25/r30 instead.
- Fix:       recipe: introduce `char* p = data;` as the LAST local declaration (`const char* table; s32 i; char* p = data;`), use `p` for the guard, loop, memcmp args, casts, and increment; keep everything else identical. Diagnostic: if id/outOffset/outSize already sit at retail's regs but the walked pointer is one slot off, add the late-declared copy rather than reordering statements.
- Confidence: repo_proven
- Applies to/a.k.a.: loop-carried walked-pointer params live across calls (CriWare scan/search loops); companion to the Rule A declaration-order and SFD_SetCond 4-variable rotation notes. Example: us-803d97c4 (SUD_SearchSudDat) 77.1% pure reg_swap -> 100.0% FULL_MATCH, TU 4/4, split PASS.

## Single-symbol multi-base init: MWCC folds all constant-offset derivations to 2 GPR bases (GC/3.0a5.2 -O4,p); 3+ bases are witness-closed (sinit_804C86C0, monolib code_804C8684)
- Symptom:   retail static-init writes N words through 1 table base + K per-block derived bases (here 4: r6=table, r5=r6+0, r4=r6+16, r3=r6+32; heads via table, tails via per-block bases; 22 insns, 7 relocs, no calls/frame). Every C++ shape emits at most 2 GPR bases (symbol base + one completed copy, plus one partial+LO store): hexdiff 0.0% (11 structural + 11 reg_swap, 0x58/0x50), objdiff fuzzy ~70%, witness-gate SIZE 22v20.
- Cause:     MWCC GVN/propagation folds every same-symbol constant-offset address into (partial base + completed copy); the first +0-offset store takes the partial+LO form and everything else shares the copy. Proven over 30+ direct-compiler probes (TU-exact flags): pointers (&arr[i]/base+i/chained/walking), subobject pointers, casts, inlined helpers, unrolled loops, decl/def/use orderings, volatile (pins store order only), pragmas (opt_propagation/peephole/optimize_for_size), ipa file/off, opt levels O0-O4s, versions GC 1.x-3.x + Wii (old ones use update-form single base). Two distinct symbols give 2 bases each (per-symbol rule); one symbol can never yield 3+. Emission is semantically identical (same addresses + values), so only the base allocation differs -- but the witness needs same count + same immediates + same reloc slots, and no-SMT policy leaves no other equivalence path: FULL-only.
- Fix:       none from straight-line C++ (recorded wall). Closest reachable shape: all accesses through one table pointer variable (heads via p[i], tails via p->), extern "C" linkage (see below); exact values/addresses, 2 bases, 9 relocs (extra partial+LO table reloc). Next angles: unroll-pragma + per-iter-select loop nests; scheduling permutations for the lfs/store interleave. ALSO: a missing `extern "C"` on the defined function mangles the decomp symbol (sinit_804C86C0__Fv vs retail sinit_804C86C0); objdiff then mispaired it as a phantom 100% FULL_MATCH twice -- always check `objdump -t` pairing (names AND sizes) when cycle reports 100% against a hexdiff 0%.
- Result:    near-miss STRUCTURAL (hexdiff 0.0%, objdiff 69.8% fuzzy, size-exact split PASS, 4/5 TU fns still 100%); stall_note in attempts.jsonl.
- Confidence: negative_result (wall) + repo_proven (extern-C pairing check)
- Applies to/a.k.a.: any single-symbol multi-base static-init (sinit_8049FC60 sibling has the same wall); any cycle-100%-vs-hexdiff-0% discrepancy (check symbol mangling first). Example: us-804cc81c.

## Hidden third call arg between stores + O4 scheduler canonicalization wall (CScnLightMan ctor, Wii/1.1)
- Symptom:   ctor prologue differs in 11 instructions: retail `stw vt,0(r3); mr r5,r28; stw param,4(r3); mr r3,r4; li r4,0; bl` vs decomp `stw param; li; stw vt; mr r3` (+spill). Loop/epilogue/size identical.
- Cause:     retail passes a hidden 3rd call arg (here `ths`/owner to `constructItemLightNw4r(param, 0, ths)`); the `mr r5,r28` + `li r4,0` scheduled between two stores is the diagnostic (same as rfc fcon/fcoff hidden-arg3). Under Wii/1.1 O4 the scheduler hoists the ready critical-path arg copy above the addi-delayed vtable store, so the vtable goes to r6, param spills, and the store sinks -- every source-order/type/cast/comma/tmp-reuse spelling converges to the same canonical output (verified over ~25 direct-compiler probes + in-TU builds); copyprop coalesces any early owner copy.
- Fix:       none from honest source (recorded wall). Closest: 3-arg TU-local decl reproduces the `mr` in position but keeps r6/+4 bytes. `#pragma scheduling off` (AXFX-style, honored function-wide) restores natural order but breaks the O4-scheduled loop (kind-`li`-first, sentinel-`r4`, `lha`/`addi` interleave) and still materializes the copy at the call; loop retune is resisted by const-fold/copyprop merging. `#pragma optimization_level N` is ignored by Wii/1.1 MWCC; mid-function scheduling toggles are ignored (function-granular).
- Result:    near-miss HIGH_MATCH 77.1% (8 structural + 3 reg_swap, size-exact, split PASS); stall_note in attempts.jsonl.
- Confidence: repo_proven (hidden-arg3 diagnostic, 2nd TU) + negative_result (scheduler wall)
- Applies to/a.k.a.: any `stw; mr; stw` + call prologue with an unaccounted `mr` (check for hidden args first); O4 prologues where the scheduler canonically hoists arg setup above constant stores.

## Unrolled channel-init loop: hoisted-table-value vs loop-base-pointer scratch swap resists decl/assign reorder (HBM synctrl ResetAllControllers, Wii/1.1)
- Symptom:   pure `reg_swap` (0 structural, exact size 0xdc/0xdc) where exactly two volatile webs are swapped: hoisted `__HBMSYNVolumeAttenuation[100]` value (retail r6, decomp r8) vs per-iter unrolled-loop base pointer (retail r8, decomp r6); the other hoisted value stays put (r7 both). Objdiff fuzzy 96.9%, witness-certified EQUIVALENT_MATCH; hexdiff strict ~52.7%.
- Cause:     both webs are short-lived scratch-pool values colored low->high; the loop pointer's higher use-degree gets it colored first in the decomp graph, while retail colored the hoisted value first. Declaration/assignment order of the two hoisted locals does not move the coloring.
- Fix:       none from ordering alone (recorded negative result). Ruled out, each reverted after a single hexdiff: (1) assign hoisted-high-index first -> 2 structural + 27 reg_swap (load order must stay low-index-first to match the retail `lwz` sequence); (2) declare high-index local first, same assignment order -> 27 reg_swap; (3) move the `u8` loop counter declaration first -> 31 reg_swap (counter spills into r9). Next angles: degree levers (shorten loop-pointer live range / lengthen hoisted web) or loop-form variants preserving the 8x-unrolled store sequence.
- Result:    EQUIVALENT_MATCH (witness-certified), split-size PASS, TU already Matching; FULL_MATCH residual recorded in attempts.jsonl.
- Confidence: negative_result
- Applies to/a.k.a.: unrolled init loops with hoisted table constants + recomputed base pointers in volatile regs (Rule C scratch-pool swaps); companion to the Rule A declaration-order recipe (which covers saved-reg swaps, not this shape). Example: us-803439f0.

## Prologue param-copy interleave + param-above-locals coloring wall (HBM synvoice ClearVoiceReferences, Wii/1.1)
- Symptom:   2 structural + 4 reg_swap (hexdiff 85.4%, size-exact 0xa4/0xa4) where retail saves r31, copies the param (`or r31,r3`), then saves r30/r29, while decomp saves r31/r30/r29 first and copies the param last (`or r29,r3`); remaining body is a global r29<->r31 rename (retail vpb->r31 synth->r30 voiceIndex->r29 voice-reuses-r31; decomp voiceIndex->r31 synth->r30 vpb->r29 voice-reuses-r31). Cycle HIGH_MATCH 94.8% with witness-gate `mnemonic | slot 4: or vs stw`, so no EQUIVALENT_MATCH despite a valid global rename existing for the rest.
- Cause:     the copy is emitted immediately after its target register's save, so copy position and color are one claim-order cause: whichever web claims the highest saved register gets its copy hoisted between the saves. Retail's single-param web claims r31 ABOVE two locals, contradicting the usual params-fill-lowest pattern -- declaration-order levers (Rule A) only reorder locals among themselves and cannot lift a param web above them.
- Fix:       recorded wall. Ruled out, each reverted after a single hexdiff: (1) release-call-before-voice-compute stmt reorder -> 65.9% (voice drops to volatile r4; retail voice is saved r31, so voice must be born before the last call); (2) voice/voiceIndex decl swap -> byte-identical no-op (voice transparent via slot reuse); (3) init-at-decl for synth/voiceIndex -> 63.4% full rotation; (4) drop row/col locals for direct `voiceTable[voice->voiceIndex][voice->voiceColumn]` index -> byte-identical no-op (row/col webs transparent). Next angles: synth-first declaration order; braced voiceIndex scope to change web degree/birth; voiceIndex narrower type.
- Result:    near-miss HIGH_MATCH (hexdiff 85.4%, cycle 94.8% fuzzy, split-size PASS, 2/3 TU fns still 100%); stall_note in attempts.jsonl.
- Confidence: negative_result
- Applies to/a.k.a.: any prologue where retail interleaves a param copy between callee-saved stores (diagnostic: copy target == highest saved reg; fixing color fixes both schedule and witness gate); param-vs-local claim-priority questions under Rule A/Rule B. Example: us-80344ab0.

## Branch-hoisted multi-table base anchors to section symbol, not first symbol (HBM synpitch __HBMSYNGetRelativePitch, all Wii MWCCs)
- Symptom:   a function indexing several adjacent same-TU data arrays from both sides of a branch gets ONE hoisted base (`lis`/`addi` at function top + per-table `addi` offsets + `lfsx`) whose reloc names the section (`...data.0`/`...bss.0` + 0) where retail names the first array symbol (`__HBMSYNCentsTable` + 0, same address). Witness-gate refuses the slot (`reloc | slot 1`); the objdiff/hexdiff comparators ignore the name (data_value), so the drift is FULL_MATCH-neutral (cf. dvd_broadway pre-existing `...data.0` anchors in an accepted unit) but EQUIVALENT-blocking while structural diffs remain.
- Cause:     MWCC's data-address folder merges the per-table addresses into one section-relative anchor when the base is hoisted across branches. Proveshape, not version: reproduced on Wii/1.0/1.1/1.3/1.5/1.6/1.7 + GC/3.0a5.2, robust across -O4,s / -ipa off / -inline changes (probes `.scratch/synpitch/anchor_probe*.c`). Straight-line multi-table access does NOT merge (per-symbol named relocs); single-symbol constant-biased indexing (`tbl[100+i]`) names the symbol but loses the hoist (per-branch base materialization + `addi`+`slwi` index bias instead of scale-only + folded base - worse).
- Fix:       none from honest source while the merge is active: keep the natural multi-symbol indexing (matches retail shape/size/values exactly) and treat the anchor as a tooling-side residual. Do NOT rewrite to single-symbol-relative indices (adds instructions/relocs) or add `extern "C"` rename spellings (reloc names are comparator-ignored; §17.6 no longer approves name spellings). If the witness must certify, the gate needs a value-identical section-vs-symbol tolerance (splitter-canonicalization hypothesis: the retail .o splitter may rewrite `...data.0`+0 to the coincident global symbol, i.e. the original compiler output may be section-anchored too).
- Result:    near-miss HIGH_MATCH (hexdiff 26.5% / 6 structural + 44 reg_swap; objdiff 81.6% / fuzzy 92%; size-exact 0x110; split PASS); stall_note in attempts.jsonl.
- Confidence: repo_proven (anchor_probe2 vs probe3 + 8-compiler/3-flag sweep) + negative_result (no source formulation yields hoisted+named)
- Applies to/a.k.a.: any branch-hoisted base shared by 2+ adjacent arrays (diagnostic: one `lis`/`addi` at function top + `...data.0`/`...bss.0` reloc; confirm with a 3-table branchy scratch probe); sibling pattern "Unrolled channel-init loop" (same TU family, scratch-pool coloring wall).

## Branch-over-branch (bne+block+b-end) tail guard is unreachable from C++ under every tested MWCC (CPathUtil getNoPathExtName, all Wii/GC MWCCs)
- Symptom:   retail tail guard `lwz r0,72(sp); cmpwi r0,0; bne B; b END; B: <fixup+strncpy+NUL+strlen>; END:` (106 insns) vs decomp `cmpwi; beq END; B` (105 insns); everything after is identical modulo the 1-slot shift (strncpy/strlen reloc presence diffs are pure shift artifacts) plus a len/needle r28-r30 color swap. Hexdiff 71.7% / fuzzy 98.4%, size 0x1A8 vs 0x1A4.
- Cause:     Wii/1.1 (and every other tested MWCC) folds ANY empty-then/empty-else arm to a single beq at IR level, before/at lowering: the unfilled `b END` can only be produced with a surviving instruction in the empty arm, and every surviving instruction stays visible (li/mr/bl/stores never merge or delete post-lowering). So `bne B; b END` with nothing between is not expressible from honest C++.
- Fix:       none from honest source (recorded wall, reverted to baseline `if (temp.mLength != 0)`). Ruled out via ~35 direct-compiler probes (`.scratch/branchshape/`, wibo + real mwcceppc, GNU-objdump verified): empty-then `{}`/`;`, !=-with-empty-else (3 forms), 2-case switch, goto-to-end (3 forms), early return, break-in-do-while(0), self-assign, discarded reads, dead writes to live/dead vars, duplicate stores in both arms (kept, no cross-arm merge), empty inline call, dead label, empty switch, goto-to-next, opaque-call then-arm, decl-swap, full-context replication with real FixStr loop+goto+tail (reproduces decomp beq byte-structure exactly). Compiler sweep Wii/1.0/1.0a/1.1/1.3/1.5/1.6/1.7 + GC/2.7/3.0a3.4/3.0a5.2 folds identically, so per-unit mw_version cannot fix it; -O4,s folds too (cf. WUD note). Probe rig recipe: minimal `if (m==0) {} else { body(); }` + opaque callees, `build/tools/wibo build/compilers/<ver>/mwcceppc.exe -c -O4,p -use_lmw_stmw on -func_align 4`, disassemble with `build/binutils/powerpc-eabi-objdump -d`.
- Result:    near-miss CODE_MATCH (71.7% exact / 98.4% fuzzy, split-size PASS with 0x4 spare); witness hard-refuses on its size gate (106 vs 105 positional slots) so EQUIVALENT_MATCH is unreachable and FULL_MATCH needs the unemittable `b`. stall_note in attempts.jsonl for us-80437a10.
- Confidence: repo_proven (35-probe sweep + 10-compiler sweep + full-context replication) + negative_result (no source formulation emits the shape)
- Applies to/a.k.a.: any retail `bne X; b COMMON` / `beq`-foldable branch-over-branch pair (diagnostic: single-`b` delta, all downstream diffs are shift artifacts); companion to the MWCC_CASES WUD note (same pattern accepted at EQUIVALENT_MATCH under SMT — under no-SMT policy it needs a witness size-gate relaxation for provably-identical branch-over-branch pairs, else stays a near-miss). Example: us-80437a10.

## Auto-unrolled loop: iter1 load order is recency-driven, iters 2+ follow source order (WPADEncrypt WPADiDecode, Wii/1.1 -O4,p)
- Symptom:   8x auto-unrolled loop where iter1 matches retail (`lbz rX,0(r3)` buf-first) but iters 2+ swap two independent loads (decomp xor-table-then-buf vs retail buf-then-xor); 26 lbz / 1 lbzx / 8 stb / 1 stbx opcode-identical both sides, counts 178 vs 180 (missing one callee-saved spill pair), cycle HIGH_MATCH ~91-94% with witness-gate `size | 180 vs 178`, then `fields` on the swapped immediates (0x92C vs 0x001).
- Cause:     in iter1 the buf address is freshly computed so the scheduler emits its load immediately (recency wins regardless of source order); in iters 2+ both addresses are carried, so the scheduler follows source evaluation order. An xor-first source (`x ^ buf[i]`) therefore matches iter1 by accident but loses iters 2+.
- Fix:       write the loop body buf-first (`v = buf[i] ^ xor[m]`) AND lengthen one table byte's live range with a named local (`u8 a; ... a = add[m]; ... buf[i] = a + v;`) — the extra live range spills exactly one more callee-saved reg (r28, individual stw/lwz, no _savegpr), which also flips iter1's addressing from `lbzx` to retail's `lbz 0(r3)`. Lvalue-view casts (`*(u8*)&buf[i]`) overshoot to 6 saved regs (`_savegpr_26` bl, witness-dead); declaration-order swaps are no-ops (temps dominate birth order); buf-first without the named local loses r28 again (178).
- Result:    FULL_MATCH 100.0% (0 structural / 0 reg_swap, size-exact 0x2d0/0x2d0), TU promoted to Matching.
- Confidence: repo_proven
- Applies to/a.k.a.: any MWCC -O4 auto-unrolled byte loop where iter1 matches but later iters swap independent loads (diagnostic: iter1 load immediately follows its address computation; iters 2+ use carried addresses); pressure lever "name the longest-lived table temp" as an alternative to lvalue views when the view overshoots into _savegpr convention. Example: us-80378540.
## Div-chain address: Mul-node `q * 16` vs Shift-node `q << 4` colors the folded `(T<<1)&~F` temps differently (HBM synsample SetupAdpcm, Wii/1.1 -O4,p)
- Symptom:   two interleaved unsigned div-by-14 chains (`q = n/14; m = n%14; a = (m+base) + ((q<<4)+2)`) at 0 structural / 28 pure reg-swaps (per-chain r8<->r9 / r6<->r7 rotations); declaration order, statement order, inline-vs-named q/m/a/t, store interleaving, parens, signedness, and named base-pointers are all absorbed byte-identically (20+ no-op probes).
- Cause:     MWCC folds `(T>>3)<<4` back into the div magic intermediate as `rlwinm y,T,1,0,27` regardless of spelling, but a Shift node (`<<4`) and a Mul node (`*16`) number the div-chain virtual regs differently, so the Chaitin coloring lands in a different basin. The original used `* 16`.
- Fix:       write the frame-scale as a multiply: `u32 a2 = (m2 + basePtr) + ((q2 * 16) + 2);` — then-branch goes 14 diffs -> 0 (byte-identical); the single-chain else-branch is unaffected by `*16` either way. Companion lever for the same function: the else dividend must be an unnamed CSE temp (`u32 q = (totalSamp-1)/14; u32 m = (totalSamp-1)%14;`) — a named `t` (or manual `m = t-q*14`, or folding `-1` into the local) flips the pRegion temp r9->r8 and costs 3 diffs.
- Result:    28 -> 11 reg-swaps (0 structural, size-exact 0x214/0x214); then-branch FULL_MATCH, else-branch still witness-closed (active0/t-1 r7/r8 swap + qt fresh-vs-reuse cascade).
- Confidence: repo_proven
- Applies to/a.k.a.: any MWCC -O4 div-by-constant chain feeding `(q<<k)` address math where decl/statement reorder probes are all absorbed — try the Mul spelling of the shift first; pairs with "m = n % 14 (not n-q*14)" and "unnamed shared dividend temp" levers. Example: us-80344630.

## Walking-pointer loop unrolls with re-based addi cadence; indexed loop folds to flat offsets (bte bdcmp, Wii/1.1 -func_align 4)
- Symptom:   6-byte compare loop written indexed (`p1[i] != p2[i]`, u8 index) fully unrolls to flat `lbz rX,N(r3)/N(r4)` (0x98, 20.0% / 31 structural) vs retail's re-based cadence: bytes 0-1 off r3/r4, then `addi r6,r3,2` + `addi r3,r4,2` hoisted between the byte-1 loads, bytes 2-5 as 0-3 off r6/r3 (0xA0).
- Cause:     MWCC's auto-unroller emits addressing in the loop's own idiom: a walking-pointer body (`*p1++`) keeps bumped bases (addi + small offsets), while an indexed body folds everything to constant offsets off the original bases. Same semantics, different unroll idiom.
- Fix:       write the body walking-pointer: `int i; for (i = 0; i < 6; i++) { if (*p1++ != *p2++) return -1; } return 0;` — MWCC unrolls x6 itself with the exact retail addi placement/scheduling.
- Result:    FULL_MATCH 100.0% (0 structural / 0 reg_swap, size-exact 0xA0/0xA0), TU promoted to Matching.
- Confidence: repo_proven
- Applies to/a.k.a.: any small-count unrolled byte loop where retail shows mid-body `addi base,+N` re-basing and an indexed source gives flat offsets (diagnostic: decomp shorter than retail by exactly the missing addis; cf. WUD walking-CRC-loop note ref:73604dc434). Example: us-802e0fb8.
## LICM-bound vs SR-bump tie-break is source-immovable (nw4r snd_SeqFile ReadOffsetByLabel, GC/3.0a5.2 -O4,p)
- Symptom:   indexed label-table loop (`for (i=0; i<blk->count; i++) e=(Ent*)(blk+blk->off[i])`, 0 structural) with pure r29<->r30 swap: retail bound(entryCount-hoist)->r30 / bump-cursor->r29, decomp bound->r29 / cursor->r30; witness-gate abi-boundary (rho permutes call-crossing saved regs) closes EQUIVALENT_MATCH, FULL_MATCH only.
- Cause:     The two loop-invariant temps are both optimizer-born (LICM hoist vs strength-reduction bump-copy), so frontend declaration/statement order cannot renumber them; ~25 probes show a conservation law: with 3 frontend temps {base,nameLen,i} the SR bump-copy always wins r30, and every shape that promotes the bound (explicit count/cursor locals, two-iterator explicit-cursor loop) adds a 4th frontend temp that rotates base/name/cursor instead, or breaks structure (plain-body cascade, flipped add operands, 9-reg pressure).
- Fix:       none found from source (near-miss catalogued); true-member conversion (drop extern "C"+layout-view hack, declare the member in snd_SeqFile.h — u32 is unsigned long under MWCC so `PUl` pairs naturally) is codegen-neutral and kept; increment comma order (`pCur += 4, i++`) does control bump-vs-inc emission order for future two-iterator shapes; bound-load attaches to the add.-defined primary temp (diagnostic for primacy reasoning).
- Result:    near-miss 89.1% (0 structural / 5 reg_swap, size-exact 0xB8/0xB8, split-size PASS); TU stays 2/3 FULL so promotion is blocked on this function.
- Confidence: negative_result
Example: us-8041b9b4.
## Size-0 `entry`-label target is witness-closed: accept via address-resolved byte identity, not source (RVL_SDK OSSync SystemCallVector)
- Symptom:   target symbol (e.g. `__OSSystemCallVectorStart`, 0x20 per map range) is a size-0 STT_NOTYPE `entry` label inside an MWCC `asm` function, not a sized function. `cycle` fails at the witness gate with `ambiguous decomp symbol '<sym>' (0 candidates)` because `list_text_functions` skips `st_size == 0` symbols on BOTH sides (retail carries the identical size-0 label — verify with `powerpc-eabi-objdump -t` on `build/us/obj/...`). hexdiff is 2/2 100% and size PASS at the same time.
- Cause:     the witness/equivalence path resolves the pair BY NAME, so no sized symbol with that name exists to extract; the cycle fallback already resolves the retail side by address (`text_base + value == addr`, i.e. the enclosing `SystemCallVector` FUNC) but then still requires a decomp-side name hit. No high-level C++ can manufacture a second sized FUNC symbol over the same bytes without breaking byte-identity, split budget, or the no-stub rule.
- Fix:       keep the honest asm body; match the original linkage instead — retail `SystemCallVector` is a GLOBAL func, so drop any `static` the decomp added (codegen-neutral for `asm` bodies, makes the decomp symtab fully identical to retail: global FUNC 0x20 + two size-0 entry labels). Do NOT rename the asm function to the label name (orphans the retail `SystemCallVector` pairing, hexdiff drops to 1/2) and do NOT add an alias function (budget-exact units have 0 spare; also a stub under PLAN.md §17).
- Result:    tooling wall, not a source wall: code 100% / 0 structural / 0 reg_swap / size-exact, decomp symtab == retail symtab, TU already `Object(Matching)`. The target needs an address-keyed FULL_MATCH acceptance for size-0 entry labels (retail side already resolves; only the decomp name lookup fails) — until then it stays ACTIVE with a stall_note. stall_note in attempts.jsonl for us-8035e020.
- Confidence: repo_proven (retail+decomp symtab dumps compared field-by-field, 2x cycle gate confirmation pre/post static-removal)
- Applies to/a.k.a.: any RVL/boot-vector `entry`-label target (`__OSSystemCallVectorStart/End`, interrupt vectors, `__start`-adjacent labels) where bind-correct `asm` output is byte-identical but the registry name is a size-0 label (diagnostic: `nm` shows `T <label>` at the same offset as a sized `t/T <Func>` + `cycle` says 0 candidates while hexdiff says 100%). Example: us-8035e020.
## Dead reload after store diagnoses volatile + explicit re-read (CriWare sfd counter, default MWCC -O4)
- Symptom:   retail does load/inc/store then a dead `lwz r0,0(r4)` reload of the same global before blr (6 instr) while natural `return g++` emits 5 instr (66.7%, 2 structural); return register still holds the first (old) load, so the function returns the pre-increment value.
- Cause:     a non-volatile double-read would be CSE/DCE'd at -O4, so a preserved dead reload proves the object is volatile-qualified in the original; MWCC keeps the observable second read but colors it r0 (dead at return).
- Fix:       qualify the global `volatile u32` and express both reads: `u32 ret = g++; (void)g; return ret;` — the `(void)g` statement is a real observable volatile read, not padding.
- Result:    FULL_MATCH 100.0% (0 structural / 0 reg_swap, 4/4 relocs, size-exact 0x18/0x18), TU promoted to Matching.
- Confidence: repo_proven
- Applies to/a.k.a.: any retail load/store/load-same-symbol tail where blr returns the first load (diagnostic: hexdiff shows exactly one trailing dead load; `volatile` alone without the second read is a no-op probe). Example: us-803c25c0.

## Linear-beq multi-exit dispatch needs int-temp + goto-gate + inverted final gate (CriWare mps_del MPS_CheckDelim, default MWCC -O4,p)
- Symptom:   retail checks one byte against 3 consecutive constants as `cmpi; beq retN` x3 with all return blocks outlined after the dispatch plus a trailing `b` to the range check (MPEG start-code guard 0xB9/0xBA/0xBB/>=0xBC). Plain if/else-if on `u8` emits `cmpli; bne`-inline returns (24.1%); a switch normalizes to `cmpi` but binary-searches the 3 consecutive cases (BA pivot, 0x7c OVER size).
- Cause:     (1) MWCC picks `cmpli` for `u8 == small-const` but `cmpi` for `int == const` and for switch cases (Tutorial_Move quirk); (2) -O4,p inlines tiny if-returns but outlines switch-case bodies; (3) a trailing `if (c) goto A; goto B;` is canonicalized to `bne B; b A` (branch-to-branch inversion on the last gate only).
- Fix:       `int v = buf[3];` temp + §17.6 goto-gate (`if (buf[i] != ..) goto ret0;` prefix, `if (v == ..) goto retN;` gates, outlined `retN:` return labels) with the FINAL gate source-inverted (`if (v != 0xBB) goto checkBC; goto retBB;`) so MWCC's inversion lands on the retail polarity; `(u32)v >= bound` keeps the unsigned tail `cmpli`.
- Result:    FULL_MATCH 100.0% (0 structural / 0 reg_swap, 29/29 insns, size-exact 0x74/0x74), TU promoted to Matching.
- Confidence: repo_proven
- Applies to/a.k.a.: any retail linear `cmp; beq body` chain over consecutive small constants with outlined constant-return blocks; diagnostic: switch attempt shows pivot reorder + OVER size while if-chain shows cmpli. Example: us-803bf200.
## Dead reload overwritten by sign-extend means the compared value is loop-live, not the reloaded field (CXReadUncompLH offset_bits, Wii/1.1 -O4,s)
- Symptom:   retail `lbz r12,off(r24)` immediately overwritten by `extsb r12,r8` (dead load), then `cmpi r8,1; bgt BODY; b SKIP`, while natural `switch((u8)field)` gives `lbz r11; cmpi r11,0; blt(dead); cmpi r11,1; ble SKIP` (4 structural, witness-gate mnemonic extsh-vs-cmpwi) and `if((s8)field>1)` gives single-`ble` skip layout (9 structural + alignment nop).
- Cause:     the sign-extended dest (r12) feeds a LATER use (`subi r8,r12,1`), but the compare consumes a DIFFERENT register (r8) holding the same value from the preceding table loop — i.e. the original compares a loop-carried local, while the field reload + extend were hoisted/scheduled for the later arithmetic. A switch on the reloaded field can never emit this shape.
- Fix:       none found yet (near-miss catalogued); candidate not yet tried: cache the table value in a local and compare the local while keeping the field reload for the later use, or shrink a live range so the local survives in scratch. Do NOT fake it with asm-shaped gotos (branch-over-branch `bgt BODY; b SKIP` is a lowering artifact, not writable control flow).
- Result:    near-miss 87.9% hexdiff (4 structural / 58 reg_swap, size-exact 0x800/0x800, split PASS); witness-closed (mnemonic gate), TU stays NonMatching. Example: us-8030aec0.
- Confidence: negative_result
- Applies to/a.k.a.: any `lbz`-then-`extsb`-of-another-register + single-`cmpi`-compare window (diagnostic: hexdiff shows exactly one dead load feeding an extend whose dest differs from the compared register).
## Anchor-carrying multi-product statement splits into fresh accumulator + fadds merge; chained tail doubles as FPR-pressure valve (Wii/1.1 -O4,p float dot-products)
- Symptom:   32-tap `fmadds` dot product matches frame/prologue (8 saved FPRs, 0x90) but is +8 bytes over budget: exactly one extra `fadds f0,f0,f1` per loop pass, with the tail products evaluated in a fresh accumulator (`fmuls f3` + `fmadds f3` chain + fold). All merge-free single-accumulator forms (all-32-separate `sum +=`, single 32-product chain, comma-joined, anchor-free chain first) blow the frame to 0xB0 (10 saved FPRs f22-f31, loads hoisted ~6 taps deep into saved regs).
- Cause:     (1) MWCC evaluates `sum = sum + P+Q+...` (any multi-product tail, even 2 products; parens/anchor-at-end do not help) as temp-chain-then-fold: fresh-accumulator `fmuls`/`fmadds` sequence plus one merging `fadds`. (2) That chained tail statement acts as a scheduling valve: with >=4 chained taps MWCC keeps hoisting shallow (8 saved regs); with a <=2-tap tail or all-separate statements it pipelines ~6 taps deep (10 saved regs). So 0x90-frame shapes always merge and merge-free shapes are always 0xB0 — mutually exclusive under straight-line C.
- Fix:       no FULL_MATCH fix known (near-miss catalogued); best-known is 24 separate taps + 8-product chained tail (frame-exact 0x90, +8B). Do NOT chase 3/5/6/7-tap tails (merge persists; coloring lotto only) or chain-first forms (0xB0). Re-attempt only with a novel valve primitive (anchor-free multi-product that stays late in program order without redefining the accumulator) or evidence of the original's exact subexpression from sibling-asm comparison.
- Result:    near-miss 11.2% hexdiff (122 structural / 162 reg_swap, body +2 insns, 0x500/0x4F8 OVER 8); cycle HIGH_MATCH 84.5% fuzzy, witness-gate size-refusal (318 vs 320 insns) so FULL_MATCH-only. Example: us-8038dc50.
- Confidence: negative_result
- Applies to/a.k.a.: any long unrolled single-precision `fmadds`-chain dot product where retail claims fewer saved FPRs than the natural all-separate C shape; diagnostic: decomp tail shows `fmuls fX` + `fmadds fX` self-chain + `fadds` fold while retail shows a pure `fmadds f0` chain, plus a +4 reloc slide downstream of the merge.
## Overlapping scratch-FPR pair flipped with 0 structural: assignment order is forced, declaration is DCE-immune, naming is count-neutral (CQuat::getRotXYZ asin-angle vs twoX2, Wii/1.1)
- Symptom:   0 structural with a pure two-register FPR swap (`frsp`/`fmuls`/`stfs`/`fcmpo`/`fadds` all `f4<->f5`, 8 per inlined expansion x3 sites) between a call-derived float (`angle = frsp(asin_result)`) and an overlapping `fmuls` product (`twoX2`); witness-gate `abi-boundary` (rho maps volatile/arg-range `fpr f4 -> f5`, live near a variadic `Warning` call) refuses certification, so only FULL_MATCH can accept. Split-size PASS and size-exact.
- Cause:     the two webs interfere (ranges overlap) with identical statements/order/asm on both sides, so the color split comes from virtual-register birth/numbering inside a shared-header inline body, not from any visible statement. The births are pinned: `frsp` must read `f1` immediately after `bl asin` (f1 dies at the next load), and the products must stay textually after the `Warning`+`asin` calls or they go saved (+1 save, +16 frame, structural blowup). Moving the computation earlier crosses the calls; splitting declaration from assignment is neutral (vregs are born at assignment — the bare declaration web is DCE'd); renaming/adding named temps is count-neutral (expression temps get webs regardless; a pass-through temp coalesces to zero net change); folding one clamp layer into the `asin` call removes a whole compare layer (structural); `?:`-form clamps change branch emission (structural).
- Fix:       none found (near-miss catalogued). Do NOT move computations across calls, do NOT fold clamp layers, do NOT ternarize branchy clamps. A TU-local member-semantics duplicate (`struct` mirror calling the same helpers) reproduces the baseline byte-for-byte and is the right experiment/landing vehicle where headers are additive-only — but only a formulation that births the product webs before the call-result web without moving computation across calls and without changing emitted ops can fix it; none found. Revisit only via (a) shadow-formulations of the shared call-chain layers (`AsinRad` `||`-form), (b) a `.scratch` probe mapping MWCC scratch-FPR color choice for call-result-vs-`fmuls` overlap, or (c) a TU `mw_version`/flags audit.
- Result:    near-miss 95.9% hexdiff (0 structural / 24 reg_swap = 8x3 blocks, size-exact 0x93c/0x93c, split-size PASS); `cycle` CODE_MATCH, witness-gate abi-boundary, TU stays 1/2 (NonMatching).
- Confidence: negative_result
- Applies to/a.k.a.: any 0-structural pure FPR `reg_swap` between a call-result float and an overlapping arithmetic product where both orders are pinned by call-clobber/save constraints (diagnostic: identical swap repeats at every inlining site regardless of call-site context, and split-decl/rename/nest variants are all color-neutral while any computation move is structural). Example: us-80435010.

## CXUncompressHuffman (RVL_SDK cx) — u32 node + bit-first add narrow 42-regswap rotation to 23; dead-reg-reuse gap is the wall (Wii/1.1, -O4,p, CODE_MATCH near-miss)
- Symptom:   CXUncompressHuffman 52.3% / 0 structural / 42 reg_swap / size-exact; loop-carried words/tree/bitsLeft/bits/bit already color-matched, residual is a setup volatile rotation plus inner node/slw temps. Witness first rho-refuses (region [21,26)), later pinpoints `abi-boundary | rho perm maps gpr r3 -> r10`.
- Cause:     two independent Birth-order defects plus one systematic policy gap. (1) `u8 node` narrows the inner temp web; retail keeps it 32-bit (lbz zero-extends either way, stream identical). (2) `((tree&~1) + bit + ...)` evaluates base-first while retail adds bit-first (`add r27,r11` order). (3) WALL: retail reuses dead regs for the two late lbz sites (flags lbz reuses dead header-word r6; node lbz reuses dead src-param r3) while MWCC takes fresh lowest-free (r0/r10); the r3->r10 perm touches the ABI-fixed param reg so the witness can never certify — only FULL_MATCH (original value lifetimes) accepts.
- Fix:       `u32 node` (fn or inner scope, identical); tree advance as `bit + ((u32)tree & ~1) + 2 * ((node & 0x3F) + 1)`; endian helper in mask-first house style `(val & MASK) >> n` (byte-identical, matches sibling CXStreamingUncompression idiom). Neutral-verified (revert to keep clean): decl-order block moves, stmt-order moves, shift/bit/out/flags/size/bitsLeft/bits/countMax/byteCount/bitBuffer u32/s32/int/unsigned spellings, u8*/u32-address/param-const alias axis, 1- vs 2-level helper inlining, single-mention flags local, const head/tail and /256 canonicalizations, dual-var reload (coalesced), inline-out, boundary forms (`>=1` regresses 23->25; `>-1`/`<1` structural+size).
- Result:    42 -> 24 (u32 node) -> 23 (bit-first add) pure reg_swap, 0 structural, size-exact, sibling CXGetUncompressedSize stays 100%; stall packet in attempts.jsonl for us-8030b700.
- Confidence: repo_proven (improvements) / negative_result (wall)
- Applies to/a.k.a.: any byte-load-then-mask setup (`lbz` + rlwinm extracts) plateauing as a volatile rotation — try widening the load temp to u32 first; check add-operand order on accumulate chains; if retail reuses a dead param/raw reg for a later load while MWCC takes fresh, record the abi-boundary wall rather than churning orders.

## HBM synenv envelope — builtin (f32) casts + literal-pool anchor + inlined complex conversions (Wii/1.1, -O4,p, CODE_MATCH near-miss)
- Symptom:   hand-rolled `0x43300000` union + explicit `double` subtraction emitted `fsub`+`frsp` pairs (+1 insn per site, +5 total, size OVER) where retail has lone single-rounded `fsubs`; after the cast fix, residual was pure color (FPR f3/f4 + GPR r3/r4 swaps) then a 7-swap r5/r6 ABI wall at 96.1%/0-structural/size-exact/data-100%.
- Cause:     retail never touches conversion constants — `lfd-pool + fsubs` IS MWCC's inline expansion of a plain `(f32)(s32)` builtin cast (xoris + stw-pair + lfd-magic + fsubs, magic = MWCC's own B18 literal pooled with the user literals). A `const f64 x = 2.0` qualifier (or function-scope hoist) breaks literal dedup (pools 2.0 twice) and/or frame shape (f31 save); a `f64` variable at the pow site hoists the `lfd` above the div-load (+4 structural); named float temps in complex arms rotate FPR colors.
- Fix:       (1) replace every manual union with plain `(f32)(s32)int` builtin casts; (2) pool 2.0 first with a bare `(void)2.0;` statement at function top — pools first, emits no code, numbers no vreg (a named variable instead costs a web/frame; position top-vs-block is neutral); use literal `2.0` at simple-arm pow sites (variable hoists the load early) and literal `2.0` in complex arms (variable hoists it above the div-load); (3) INLINE the complex-arm conversions (`(f32)a + (f32)e*n128[ch]`, no named attackT/extraT temps) — named temps rotate f3/f4; simple arms keep one named temp each; (4) attack-rate store order must be ticks, rate(div), vol, phase (retail order; rate-first or vol-first add a lis-hoist structural or regress colors); (5) decay block: declare `s32 decay; s32 extra2;` BEFORE `pp/ch` with split assignment (loads stay params,note,decay,extra2) — birth-before-pointer flips decay r3/r4 to retail.
- Result:    15.2% (129 structural) → 69.8% (0 structural after literal-2.0) → 79.9% → 91.6% (inlined complex) → 96.1% (0 structural / 7 reg_swap, size-exact 0x2cc, data 100%, split PASS); RunVolumeEnvelope stays 100%. Remaining 7 are one r5<->r6 flip (params vs ch) that resists decl/split/hoist/pp-local/signedness/const/anchor/direct-access levers — witness-gate abi-boundary, FULL_MATCH-only wall (see stall packet us-80343e90).
- Confidence: repo_proven (cast/anchor/pool/inline/store-order/early-birth fixes) / negative_result (r5/r6 wall)
- Applies to/a.k.a.: any 0x43300000-union site showing `fsub`+`frsp` vs retail `fsubs` (rule: retail `fsubs` ⇒ builtin cast, retail `fsub`+blob ⇒ manual union); any TU whose literal pool must start with a specific double (use a bare `(void)lit;` anchor, never a named const/variable); any pow-with-division complex arm (literal base at site, never a variable); any divw-result color mismatch (try retail store order first). Example: us-80343e90.

## UTY_CmpTime — s64 compare operand order drives mulhw/mullw evaluation order (Wii/1.1, FULL_MATCH)
- Symptom:   pure reg_swap (0 structural, 11 reg_swap, size-exact 0x30): decomp emitted `mulhw cb` first while retail emitted `mulhw ad` first, cascading into different scratch colors (ad-lo in r3 vs r6, hi-diff temp r4 vs r7). Witness-gate abi-boundary (rho perm maps gpr r3 -> r5, ABI-fixed) refused certification, so only FULL_MATCH could accept.
- Cause:     comparison operand order sets which 64-bit product MWCC evaluates first. `return cb >= ad` births the cb product webs first (cb-hi mulhw first); `return ad <= cb` births ad first (ad-hi mulhw first, retail order). Declaration order of the two s64 locals was color-neutral (both orders emitted identical bytes); the `+ ad - ad` keep-alive variant was fully value-numbered away (byte-identical to the plain form). Same semantics, mirrored birth order.
- Fix:       write the comparison LHS-first for the product retail evaluates first: `s64 ad = (s64)a * (s64)d; s64 cb = (s64)c * (s64)b; return ad <= cb ? 1 : 0;`. Try the mirrored comparison direction before any declaration/expression reshaping.
- Result:    FULL_MATCH (hexdiff 100%, 0 structural / 0 reg_swap, size PASS; cycle semantic-certified). Example: us-803d6aa4.
- Confidence: repo_proven
- Applies to/a.k.a.: any pure reg_swap between two s64 multiply-compare webs where decomp evaluates the products in the opposite order from retail (diagnostic: first two mulhw instructions swapped); also candidate for other commutative-pair births (add/sub operand order) in straight-line integer code.

## UTY_MulDiv — INT_MIN-saturate clamp: literal `+0x80000000` always folds to addis, E-form keeps lis but renders subfic+subf (GC+Wii all versions, HIGH_MATCH near-miss)
- Symptom:   c==0 saturate path needs `xor r3,r3,r4; lis r0,-32768; srwi r3,r3,31; subi r3,r3,1; add r3,r3,r0` ((x-1)+INT_MIN with x=(a^b)>>31). Every literal-`+INT_MIN` spelling (`(x-1)+K`, `K+(x-1)`, temps, compound-assign, comma, ternary, K-first, unsigned variants) compiles to `xor r0,...; srwi r3,r0; addis r3,r3,-32768; subi r3,r3,-1` on ALL 25 available compilers (GC 1.0-Wii 1.7) at ALL opt levels (-O0..-O4, -O4,p, exact retail cflags): MWCC canonicalizes `(x+C1)+C2` to `(x+(C1+C2))-1` at the IR level, so statement splitting cannot block it.
- Cause:     `add` has an immediate form (addis) but `subf` does not, so only an outer-SUBTRACTION keeps INT_MIN in a register: `K-(1-x)` (`(int)0x80000000 - (1 - (int)((u32)(a^b)>>31))`) emits the retail `xor r3/lis r0/srwi r3` prefix byte-identically (GC/1.3+ and all Wii) but renders the tail as `subfic r3,r3,1; subf r4,r3,r0` instead of `subi r3,r3,1; add r3,r3,r0`. Same values (verified by host execution over all sign combos: both equal `(a^b)<0?INT_MIN:INT_MAX`), different mnemonics. The exhaustive depth-2 arithmetic sweep (all 12 value-correct `(x,1,K)` (+/-) trees) shows no literal-K form emits `lis+subi+add`; a runtime-K would need a second reaching def (SSA folds single-path constants) whose materialization has no room on the tight 6-insn `mullw/or/srawi/mulhw/bl/or` divide path.
- Fix:       E-form `return (int)0x80000000 - (1 - (int)((u32)(a ^ b) >> 31));` for the c==0 path (value-exact near-miss, 85.5% HIGH_MATCH, size-exact 0x58/0x58, split PASS). Do NOT chase versions/opts (uniform across all), statement splits (expression-level fold), or `x+0x7FFFFFFF` (rewrites to the same addis form).
- Result:    27.3% (13 structural, addis-fold) -> 85.5% HIGH_MATCH (subfic+subf + or/lwz epilogue-order consequence, size-exact); `cycle` witness-gate `mnemonic | slot 9: addi vs subfic`, so only FULL_MATCH can accept. Example: us-803d6c10.
- Confidence: repo_proven (fold uniformity, E-form prefix) / negative_result (subi+add wall)
- Applies to/a.k.a.: any `(x-C)+0x80000000` saturate/clamp idiom where retail shows `lis; ...; subi; add` — check E-form first for the lis-prefix, then record the mnemonic wall rather than churning spellings/versions.

## CMenuBattlePlayerState ctor — unmangled extern "C" factory + bit30 /12 fixup + -O4,s divwu fills (Wii/1.1, HIGH_MATCH 82.4%)
- Symptom:   `__ct__CMenuBattlePlayerState` (retail symbol has NO mangled suffix) compiled as a C++ ctor mangles to `__ct__22CMenuBattlePlayerStateFP4CScn`, adds implicit base/member init calls (extra CProcess + 2x UnkClass bls), and parks this in r31; retail keeps this in r27 with exactly 4 calls. Separately, retail s32/12 lowers to lis 0x2AAB/mulhw/srawi/`rlwinm r,_,1,31,31`-fixup/add (bit30, dead for this magic but emitted), while every tested MWCC (Wii 1.0a/1.0RC1/0x4201_127/1.0/1.1/1.3/1.5/1.6/1.7, GC 2.6/2.7/3.0a5/3.0a5.2, -O0..-O4,s) uses the sign-bit `srwi` fixup, and -O4,s lowers plain /12 to divw.
- Cause:     (1) Retail ctor is a hand-written unmangled `extern "C"` free function (same idiom as the FULL_MATCH `__ct__CMenuBattleCommu` factory: temp vtable, post-increment ptmf triple via `*p++`/`p[-3]` idiom for lwzu, dispatch slots, UnkClass ctor) — not a C++ ctor, so no implicit init calls. (2) The bit30 fixup is not produced by any available MWCC lowering: it must be hand-built in the original source (or an unknown compiler). (3) Fill counts divide by strides: under -O4,s both literal and variable divisors keep divwu+mtctr, but under -O4,p even variable strides propagate (`s60=0x60` folds) and strength-reduce to mulhwu — so the TU must stay -O4,s and plain /12 stays divw (count 352 vs 348 blocks the witness size gate).
- Fix:       define `extern "C" void* __ct__CMenuBattlePlayerState(CMenuBattlePlayerState*, CScn*)` (unmangled; single `me`/`self` park, commu-style ptmf block); keep TU `-O4,s` with literal /0x60//0x0C (divwu+mtctr, no stride vars needed); for the div, `__mulhw` (real Wii/1.1 builtin, verified bare-`mulhw` emission for two opaque s32 via `(s64)a*(s64)b>>32`) + `__rlwinm(q,1,31,31)` reproduce the exact opcodes, but split statements invite scheduler hoists (padLim/counts jump the gate) that cost more than the div — prefer plain /12 until the hoists are pinned. Reverted a `DECOMP_PPC_MULHW` decomp.h addition as unused; re-add if the hand-split path resumes (log policy_exception).
- Result:    3.7% (C++ ctor, implicit calls, OVER size) -> 81-82.4% HIGH_MATCH, size PASS 0x570/0x580, prelude 0x0-0xE0 reg-swap-only; witness-gate `size | 352 vs 348` (div idiom length), rho untested. Stall packet in attempts.jsonl for menu-bps-ctor.
- Confidence: repo_proven (extern-C/symbol/calls, -O4,s fills, builtin emissions, version sweep) / negative_result (bit30 compiler, homes r27, hoist pinning)

## Hoisted global base + per-case constant offset: MWCC folds into displacements, retail keeps per-site addi (vi3in1 __VISetMacrovision, witness size-gate)
- Symptom:   nested-switch dispatch with N inlined copy sites off one hoisted global base shows objdiff HIGH_MATCH ~88% but hexdiff ~3% (hundreds structural): retail does `addi rT,rBase,off` per site + small-offset `lbz rX,0..25(rT)`, MWCC emits `lbz rX,off+i(rBase)` folded displacements, leaving decomp exactly N instructions short; `cycle` witness-gate fires on `size | retail vs decomp` before any renaming is attempted.
- Cause:     with the base referenced once (single HA/LO reloc pair, addend 0) and every site offset a compile-time constant, folding is strictly smaller (saves 1 addi/site) so MWCC 43_151 `-O4,p` always folds — independent of loop form (indexed vs pointer-increment), `static __inline` + `inline_max_size` pragmas, local-ptr/direct-expr/`&arr[i]`/casts/`volatile`/`memcpy`/struct-copy spellings, `-ipa off`, or compiler version. Any source rewrite that makes the offset runtime (offset table, 2D index, pointer table) destroys the switch dispatch or adds pointer loads, contradicting retail branches/relocs.
- Fix:       none known from high-level C — do NOT churn spellings (15+ forms ruled out across 4 sessions); record a stall packet and move on. Diagnostic shortcuts: (1) reloc check first — a single HA/LO pair with addend 0 plus per-site pure immediates proves the reference pattern already matches; (2) instruction-count delta == number of folded addis confirms the mechanism; (3) objdiff fuzzy (~88%) vs hexdiff static (~3%) gap is the signature of an addressing-mode-only diff.
- Result:    STALLED near-miss (hexdiff 2.8%, 487 structural + 351 reg_swap; objdiff 88.27% HIGH_MATCH; witness size-gated 862 vs 850; SMT disabled so only FULL_MATCH could accept, which needs the unsteerable addi). Example: us-8036a2b0.
- Confidence: negative_result (4 sessions, 15+ forms, version/flag sweeps) / repo_proven (reloc + count-delta diagnostics)
- Applies to/a.k.a.: any hoisted-base + per-case-constant-offset copy fan (ACP/gamma-style tables, multi-site I2C/DVD table sends); the `lis; lbz rX,lo(rB)` single-symbol OFF-site shape (no addi needed) vs the `lis; addi; addi-per-site` gammaSet shape in the same function.
- Applies to/a.k.a.: any unmangled `__ct__` retail symbol (check for missing C++ suffix first — a C++ definition mangles, adds implicit calls, and shifts homes); any retail `rlwinm _,_,1,31,31` fixup after mulhw/srawi (no MWCC emits it: treat as hand-built, verify numerically, do not chase versions); any divwu+mtctr fill under -O4,s (literals are safe; stride vars only help under -O4,p where they propagate anyway).

## CLibLayout::layoutAllocCallback hash probe — saved-reg decl order + volatile load order vs unreachable preload-coalescing (Wii/1.1 -O4,p, CODE_MATCH 76.7% stall)
- Symptom:   bl-containing allocator callback at 65.1% pure reg_swap (0 structural, size-exact): saved elem/pBuf colors swapped (r31/r30 vs r30/r31) plus 5 volatile temps rotated (count r0->r7, accum r6->r0, divisor r7->r6, table r8->r5, idx-1 r5->r7). Witness sequence: `fields | slot 10` (load-order) -> fixed by count-first; `abi-boundary | rho r6->r7` -> `abi-boundary | rho r0->r7` after saved fix. r0 is ABI-fixed (prologue mfspr/stw), so the witness can never certify while count sits in r7; bl-containing => FULL_MATCH only.
- Cause:     (1) Saved regs claim high->low in declaration order (Rule A): elem declared before pBuf took r31, but retail (pBuf r31, elem r30) declares the return buffer first. (2) The witness pairs instructions positionally, so the two body loads must come in retail order (count, accum) even though colors differ. (3) Retail preloads mAllocHandle into r6 AND reuses r6 for accum (path-sensitive coalescing of non-interfering ranges); MWCC re-derives a fresh color whenever val is born before the temps, so accum->r6 is unreachable from any named-local shape with a pre-branch val.
- Fix:       declare the cross-call return buffer before the element (`u8* pBuf;` then `elem = table[idx]`); compute the probe count-first in split statements (`s32 idx = Vcount; idx += Vaccum; idx = (idx-1) % divisor;`) with the branch test the sole volatile read (CSE never merges a volatile branch with a plain body read, so the double hashCount load survives without volatile casts in the body).
- Result:    65.1% -> 76.7% (15 -> 10 reg_swap, 0 structural, size PASS, split PASS, line-neutral so the sibling layoutFreeCallback near-miss is undisturbed). Stall packet in attempts.jsonl (us-80463408).
- Confidence: repo_proven (saved decl-order fix, load-order gate fix) / negative_result (single-expression fusion displaces inst r5->r7 at 72.1%; val-declared-inside + duplicated tails goes 8.9% with +20 structural/+8 size - no tail-merge and an extra or-param-save; plain-vs-volatile body reads are byte-identical)
lookup compensates the registration post-store hashCount++ with - 1 (signed divw).

## nw4r TypeObj ==-chain bool-materialization poison + redundant-load CSE wall (GC/3.0a5.2 -O4,p -ipa file; ScnGroup::IsDerivedFrom STALLED one-load near-miss)
- Symptom:   multi-link `IsDerivedFrom` (`other == TypeObj(lbl)` chains, nw4r g3d RTTI) compiles to whole-function `subf/cntlzw/srwi.` bool materialization plus a clobbered base (`lwz r4,0(r4)`) instead of retail's `cmpl; bne`-per-link + trailing `subf/cntlzw/rlwinm` with `lwz r0,0(r4)` reloads. hexdiff shows large structural counts (8-12%) with 0 reg_swap even though the source looks branchy.
- Cause:     two stacked effects. (1) POISON: `operator==` on a temporary bound to a TU-visible rodata definition (same-entity initializer anywhere in the TU, position-independent) flips the lowering of EVERY `==` in the function to bool materialization - proven by TU-context probes (ternary+operator== with one local-def link materializes all links; all-extern variant lowers to cmpl). A static-helper boundary does the same after inlining. Spelling the comparisons as explicit `GetTypeID() == TypeObj(lbl).GetTypeID()` u32 ternaries dodges it completely (cmpl restored, label position irrelevant). (2) CSE WALL: helper-free, the `other.mName` load CSEs to a single `lwz` reused across links while retail reloads per link (except the trailing link, which reuses) - one redundant load, provably unobtainable from high-level C++ (copy/address-taken/volatile/call barriers all add instructions retail lacks).
- Fix:       write the chain as a flat ternary over explicit GetTypeID values, no operator==, no helper: `return o.GetTypeID() == TypeObj(A).GetTypeID() ? true : o.GetTypeID() == TypeObj(B).GetTypeID() ? true : (o.GetTypeID() == TypeObj(C).GetTypeID());`. Result is 19/20 instructions byte-identical (same regs, same mnemonics, same reloc symbols). Do NOT churn if/return vs ternary vs helper once this shape is reached, and do NOT move rodata definitions (tested, no effect).
- Result:    STALLED near-miss (ScnGroup::IsDerivedFrom us-803f7ea4: cycle CODE_MATCH 95.0%, witness-gate `size | 20 vs 19`; hexdiff linear 40.0% from the 4-byte cascade). Witness (renaming-only) cannot certify an instruction-count delta and SMT is disabled, so only FULL_MATCH could accept - unreachable (see Cause 2).
- Confidence: repo_proven (poison trigger + GetTypeID dodge + CSE wall, isolated and TU-context probes with exact TU flags) / negative_result (helper/if/ternary-operator== spellings, label EOF move, reload-forcing barriers, version/flag changes out of scope)
- Applies to/a.k.a.: every nw4r multi-link IsDerivedFrom against a TU-owned TYPE_NAME (sibling ScnObj::IsDerivedFrom us-803f7ef4 should adopt the same GetTypeID-ternary recipe - expect the same one-load residual); the anmclr AnmObjMatClr ternary (all-extern labels, CSE-only gap) is the reference shape; KB ref:6d18c6f7b7 (pure redundant-load CSE was SMT-certified EQUIVALENT pre-policy).

## do-while pointer fill peels head + bulk(8x)/remainder with double guard (nw4r BasicPlayer ctor, Wii/1.1 `-O4,p`)
- Symptom:   Constructor filling a float array with a `while (it < end) { 6 stores; it += 6; }` pointer loop plus explicit head stores sticks at ~11% (87 structural): decomp emits a single boolean bulk-entry guard while retail validates both the byte-diff and the trip/trip+1 signs (extra `rlwinm`/`cmp` block), and the bulk limit derives as `end-192` (`subi`) instead of `this+16` (`addi`), plus a missing end reload.
- Cause:     MWCC splits a runtime-trip pointer loop into an 8x-unrolled bulk CTR loop (stride 192) + a remainder CTR loop (stride 24). A `while` loop's trip is ceil-based (single guard); a full-range `do { ...; it += 6; } while (it < BOUND)` over the whole array lets MWCC peel the head iteration itself (6 direct stores) and compute the trip floor+1 style (double guard: diff signs AND trip/trip+1 signs). An explicit `end` local forces `subi`-from-end derivation and pointer-preserving bool colors (no reload); anINLINE bound expression folds to `this+16` (`addi`) and spills the pointer regs into the bools (forcing the retail reload). Explicit head stores also force the prologue float loads one-first; a pure loop peeling its own head loads zero-first (matches retail).
- Fix:       Write the fill as a pure full-range `do-while` with the bound inline (`it < &arr[N]`), no `end` local, no explicit head stores, no float locals — let MWCC peel the head. Pointer stride must equal the store-group width (here 6 floats = 24 B; bulk stride 192 = 8 groups).
- Result:    11.4% (87 structural) → 40.9%, 0 structural, exact size, all float/bl relocs positional-match; residual pure GPR `reg_swap` (CODE_MATCH 96.6–96.8% fuzzy). Witness then gates on ABI (`r4`) colors, not structure.
- Confidence: repo_proven (nw4r `snd_BasicPlayer` `__ct__BasicPlayerFv`, US, 0x254/0x254)
- Applies to/a.k.a.: runtime-trip pointer fills with peeled heads; `-O4,p` bulk+remainder splitting; `while` vs `do-while` trip-guard shapes; inline-bound vs end-local `addi`/`subi` derivation.

## GetMemSize leaf — `#pragma peephole off` restores stwu-first prologue + retail load window (Wii/1.1 -O4,p)
- Symptom:   Small table+float-mul leaf emits lfs/lis before stwu, float-const lfs lands late, tail matches but prologue/load windows are order-swapped vs retail; STRUCTURAL ~45% with peephole on.
- Cause:     MWCC peephole merges/sinks the prologue store and reorders base materialization vs pool loads; with peephole on the scheduler fills latency differently than the original build.
Wrap ONLY the leaf in `#pragma peephole off` ... `#pragma peephole on`, keep plain six-local shape
- Result:    45.7% -> 69.04% objdiff (62.5% raw); stwu 1st, const lfs at retail offset, tail from mid-function byte-identical. Remaining order-only swaps (param-lfs hoist, lis/addi reg merge, load-window distribution) are scheduler-invariant and witness-closed.
- Confidence: repo_proven (RVL AXFXReverbStdExp GetMemSize + Dpl2 twin us-802db010, Wii/1.1)
- Applies to/a.k.a.: peephole prologue sinking; stwu-first prologues; scheduler load-window fingerprints; GetMemSize leaves.

## Loop-inner chunk locals hoisted to function scope fix saved-reg birth order (RVL hbm seq, Wii/1.1)
- Symptom:   pure `reg_swap` (0 structural, size-exact 0x11c/0x11c) where the loop counter (`n`, decomp r27) and the per-iteration chunk length (`len`, decomp r26) are swapped vs retail (`n` r26, `len` r27); both live across the float-conversion `bl`, so bl-containing witness path is closed and only FULL_MATCH accepts.
- Cause:     saved regs claim high->low in virtual-register birth order (Rule A); `len` declared inside the loop body is born after the function-scope counter, so it claims the lower register. Retail births `len` before the counter.
- Fix:       hoist the loop-inner locals to function scope declared BEFORE the counter (`u32 tag; u32 len;` after `p`, then `int n = count;`) and assign (not init) them in the loop body. Declaration position alone flips the two colors with zero instruction change.
- Result:    90.1% (7 reg_swap) -> FULL_MATCH 100.0% (`__HBMSEQInitTracks__FP15_HBMSEQSEQUENCEPUci`, us-80344c60).
- Confidence: repo_proven
- Applies to/a.k.a.: any pure-swap residual between a loop counter and a loop-inner local both live across a call; prefer hoisting the inner local above the counter over swapping counter/pointer order (the latter rotated `p` out of its correct register here: 90.1% -> 83.1%, ruled out).

## Float-to-u32 needs the `(u32)(s32)` double cast — direct `(u32)f` emits an out-of-line helper call (Wii/1.1)
- Symptom:   Rewriting `(u32)(s32)(K * x)` as `(u32)(x * (f32)K)` collapses match (~62% -> ~23%), grows size (0x60 -> 0x68 OVER), and emits `mfspr/stw` FPSCR save/restore plus a `bl` to a float->uint helper; retail has inline `fmuls/fctiwz/stfd/lwz` with no call.
- Cause:     MWCC lowers a direct float->u32 conversion via an out-of-line helper (FPSCR manipulation + bl); the `(s32)` intermediate keeps the conversion on the inline fctiwz path. Same-file Init idiom `(u32)(x * (f32)32000)` is NOT transferable to leaves whose retail is call-free.
- Fix:       Keep `(u32)(s32)(32000.0f * x)` (or equivalent double cast) whenever retail shows inline fctiwz/stfd/lwz and no bl.
- Result:    Restores 62.5% size-exact state on AXFXReverbStdExpGetMemSize (us-802da2e0); direct-cast variant ruled out.
- Confidence: repo_proven (RVL AXFXReverbStdExp us-802da2e0, Wii/1.1 -O4,p)
- Applies to/a.k.a.: float-to-unsigned conversions; fctiwz vs helper-call codegen; bl-free retail leaves.

## GC/3.0a call-containing loop: two live-across-call pointers resist all declaration/scope levers (sdp_api SDP_FindServiceUUIDInDb, negative result)
- Symptom:   0 structural / 18 pure reg_swap, size-exact; retail `p_rec`->r31 / `p_attr`->r30, MWCC emits `p_rec`->r30 / `p_attr`->r31; `p_uuid`->r29 both. Witness-gate abi-boundary (rho gpr r30->r31) closes EQUIVALENT_MATCH and SMT is disabled, so only FULL_MATCH accepts.
- Cause:     Both pointers are born at function scope and stay live across the same `bl`; unlike the Wii/1.1 hbm-seq hoisting case, GC/3.0a3.4 (this TU: `-func_align 4 -ipa off`) does not recolor them for any declaration-position or scope change tried.
- Fix:       none found — ruled out, each hexdiff-verified as a byte-identical no-op and reverted: (1) declaration-order swap both ways, (2) moving `p_attr` decl into the outer loop body, (3) outer `while` -> `for` with advance in post-expr, (4) `tBT_UUID` param-copy local used at both call sites (coalesced away), (5) scoped named temp for the sub-attr list head. Prior work additionally ruled out p_attr2-vs-reuse, ternary init, initialized decls, flat-vs-nested ifs.
- Result:    near-miss CODE_MATCH 72.7% instr / 98.3% fuzzy; recorded with stall_note (us-80305d38). Sibling SDP_FindServiceInDb reaches FULL_MATCH only because it contains no calls (volatile regs, no saved-color pressure).
- Confidence: hypothesis (single-TU negative result; allocator soft-cap, not proven irreducible)
- Applies to/a.k.a.: GC/3.0a BTE call-containing db-scan loops; callee-saved pointer-pair swaps where both values cross the same call; witness-closed abi-boundary residuals.

## Placement-new LinkList keeps LinkListNode NULLs; MWCC merges them away (hbm lyt_layout Build, negative result)
- Symptom:   retail `Layout::NewObj<GroupContainer>()` inlines to zero+init (5 stores: `stw 0,4(r3); addi r4,r3,4; stw 0,8(r3); stw 0,0(r3); stw r4,4(r3); stw r4,8(r3)`); decomp emits init-only (3 stores, 8 bytes short, all downstream branch displacements shift by 8 and every reloc drifts by 8). Witness-gate SIZE (193 vs 191 instructions) => witness-closed, FULL_MATCH-only.
- Cause:     the zeros are `LinkListNode::LinkListNode()` NULLs (`mNext/mPrev = NULL`) preserved before `LinkListImpl::Initialize_()` self-links the sentinel (`mSize = 0; mNode.mNext/Prev = &mNode`). MWCC's constructor merging dead-store-eliminates the NULLs as overwritten-before-load. Proved via direct `mwcceppc` probes: (a) POD-node (no ctor) => init-only everywhere; (b) NULLing-node => NULLs kept in placement-new context, dropped in mem-init context; (c) adding `private NonCopyable` base to `LinkListNode` defeats the merge in tiny TUs (exact retail bytes incl. `stw r4,4(r3)` addressing); (d) NO Wii version (1.0-1.7) nor GC/3.0a5.2 emits NULLs with current headers; `-O4,s` and `-ipa off` also drop them. Mem-init sites (`Layout` ctor `: mAnimTransList()`) drop NULLs in BOTH retail and decomp (3-store `Initialize_`, byte-identical), so the merge is placement-new-specific and retail preserves NULLs only there.
- Fix:       none found in-scope — the N1 (Node base, with and without `Init()`-call form) restores NULLs in tiny-TU probes but NOT in the real TU (verified via forced rebuild and shadow-header real-TU compiles; `-ipa off` and minimized-Build probes also drop). Ruled out: implicit-`GroupContainer` value-init (MWCC does not zero placement-new of implicit types on any tested compiler), per-unit `mw_version` change (would perturb 13 matched sibs; no version keeps NULLs anyway), `memset`/explicit word-zero (extra `bl` or duplicate-store, wrong shape), `NewArray`/branch-shape variants. Shared-header surgery (`ut_LinkList.h`) left reverted: blast radius is every inline placement-new LinkList site program-wide.
- Result:    near-miss CODE_MATCH 62.2% (39 structural = pure 8-byte displacement fallout, 34 reg_swap fallout); recorded with stall_note (us-80330450). All header edits reverted; tree at baseline.
- Confidence: hypothesis (single-TU negative result; mechanism proven by probe, trigger conditions not fully characterized — N1 defeats merge in tiny TUs but not in the real TU for unknown contextual reasons)
- Applies to/a.k.a.: placement-new of LinkList-containing types with inline ctor chains (`NewObj<GroupContainer>`); LinkListNode NULL-init vs Initialize_ merging; next angles are original HBM/NW4R header comparison and MWCC version/flag discovery for NULL-preserving placement-new under full headers.

## Named char* local fuses lis/addi into one saved reg (+ mr copies, src-first strcpy order) — ctor __ct__CCharVoice (Wii/1.1)
- Symptom:   22.2% (21 structural / 7 reg_swap) ctor; retail `lis r30,@ha; addi r30,r30,@l` once then `or`/mr copies to r3/r4 (strlen/strcpy), zero in r31; decomp holds only `lis @ha` in r31 with per-use `addi rX,@l`, zero in r30, and strcpy dst-arg materialized before src-arg.
- Cause:     inline `(const char*)lbl` address expressions at each call site never form a single fully-materialized pointer web, so MWCC keeps just the high part common and re-adds the low part per use (extra LO reloc, addi instead of mr, dst-first arg order); the unnamed zero constant loses the Rule-A claim race to the string.
- Fix:       named locals declared zero-first: `int zero = 0;` (assigned to both zero fields, incl. a `(CVoiceOwnerIntf*)` cast for the pointer field) then `char *name = (char*)lbl;` used at both call sites. Zero->r31, name->r30 per Rule A; single HA+LO pair; mr copies; retail src-first strcpy order falls out.
- Result:    22.2% -> FULL_MATCH 100.0% size-exact 0x90/0x90 (`__ct__CCharVoice`, us-802a3230).
- Confidence: repo_proven
- Applies to/a.k.a.: ctors/init functions materializing one array address for strlen+strcpy pairs; inline cast-at-use vs named-pointer-local codegen; zero-vs-pointer saved-reg claim order.

## Interleaved multi-byte accumulation flips volatile pointer-chain colors (hbm seq HBMSEQRunAudioFrame, partial + witness-closed volatile abi-boundary)
- Symptom:   Tempo-path 3-byte big-endian read: 0 structural / 16 pure reg-swaps (retail pointer/borrow chain in r5/r4/r3/r0, decomp rotated), size-exact. Single-expression form tempo32 = ((((u32)b1 << 8) + b2) << 8) + b3.
- Cause:     One nested expression births all three pointer temps before any combine, so MWCC colors the chain in one rotation; splitting the accumulation into interleaved statements (tempo32 = (u32)b1 << 8; tempo32 = (tempo32 + b2) << 8; tempo32 += b3;) re-births the middle temps after the first combine and flips 6 of the swaps away. The remaining middle-pair swap (retail t2/t3 in r4/r3, decomp in r3/r4) is invariant across declaration orders, load-form variants (cur[-1] normalizes to identical codegen), block-scope locals (regresses), length-byte liveness (b vs b1), and accumulator vreg (v coalesces with tempo32).
- Fix:       Interleave the accumulation with the byte reads as above (16 -> 10 swaps, 94.5pct -> 96.6pct hexdiff, still 0 structural/size-exact). Ruled-out no-ops (each hexdiff-verified, reverted): b1/b2/b3 declaration orders, track->cur[-1] form, case-block-scope tempo locals, b1-as-length-loop-byte, v accumulator, interleave position.
- Result:    near-miss CODE_MATCH 96.6pct / 0 structural / 10 reg_swap / 0x490 exact (us-80344e60); witness-gate abi-boundary (rho perm maps gpr r3 -> r4; ABI registers must be fixed) - volatile arg registers count as ABI-fixed, so the witness can never certify even a clean 2-register bijection here. Only FULL_MATCH or out-of-band SMT accepts.
- Confidence: repo_proven (lever: 16->10 on us-80344e60, Wii/1.0a-or-1.1 hbm -O4,p; closure: witness abi-boundary on volatile r3/r4)
- Applies to/a.k.a.: multi-byte big-endian assembly (tempo/midi/varint tails); volatile pointer-chain rotations; witness-closed abi-boundary residuals on r3/r4.

## EyeAnm_RegisterMats twin-scan — GetResMat(int)/(u32) overload trap + j-before-i ordering (Wii/1.1 -O4,p)
- Symptom:   Twin material-scan loops (EyeAnm_RegisterMats, CMdlAnmEye) stuck at pure reg_swap (54) with a uniform +1 saved-reg shift, second-loop counter in the opposite free slot (retail j=r30 vs decomp j=r23), and an r4/r5 id-vs-zero scratch swap; 0 structural, size-exact.
- Cause:     (a) ResMdl exposes GetResMat(int) AND GetResMat(u32) overloads: s32 counters do not compile (ambiguous 10199), int counters silently bind the INT overload (reloc drift vs retail, invisible to hexdiff instruction compare which normalizes bl targets) — retail rela.text proves the ULONG overload, so counters must be u32. (b) Second-loop counter declared before first-loop counter (u32 j; ... u32 i;) shaves 54->50 swaps; all other decl positions (flag top/mid/last, n/nm hoists, for-init) are neutral — uninit decls only matter via relative order, not presence.
- Fix:       Counters u32 (matches ULONG callee); declare j before i. Diagnose literal-vs-variable from asm position: a pre-loop hoisted li (flag=1) proves a variable, an in-block li (zero) proves a literal — an in-block zero-var is neutral, a dropped flag-var goes structural.
- Result:    near-miss CODE_MATCH 61.2pct / 0 structural / 50 reg_swap / 0x204 exact (us-804eba54); witness-gate abi-boundary (rho r28->r29) so only FULL_MATCH accepts. Ruled-out no-ops (each hexdiff-verified, reverted): n-after-stores, n-first, stores-swapped (all structural — match-block store/load order is load-bearing for the 2-load CSE); u8-flag, int/s32-num, void*-field, named per-iter ResMdl (regresses to 56), (void)j keep-alive (DCEd), per-iter named temps.
- Confidence: hypothesis (lever: 54->50 on us-804eba54, Wii/1.1 -O4,p; overload trap repo-verified via rela.text)
- Applies to/a.k.a.: twin-scan loops (EyeAnm/MouthAnm/UVAnm families); overloaded nw4r ResMdl/ResMat accessors; hoisted-literal asymmetry diagnostic; witness-closed abi-boundary residuals.

## Fog assert-block shaping: hoist loads across Panic + single-base stores + split decl/assign (CScnFogMan FogManApplyFog 31%→89%, Wii/1.1 -O4,p)
- Symptom:   Fog-setter blocks (`if (ptr==NULL) Panic; if (ptr!=NULL) stores;`) compiled stores after the Panic branch (volatile f0 reloads) while retail hoists the value loads before the branch into saved regs (126 structural, 31.3%); loop color store-back pipelined with 3 extra base reloads (+3 size); hoisted f32 pairs swapped f28<->f29.
- Cause:     (a) Values used after a call must be born before it to claim saved regs — inline member loads in the guarded store block are born after the Panic call, so they stay volatile. (b) Repeated `fogSlot.ref().color.X` store expressions defeat base CSE (scheduler pipelines with clobbered base). (c) Load order follows assignment order while reg color follows birth order — coupled in `f32 x = init` form.
- Fix:       (a) Hoist values to named locals before the NULL check (`f32 endz = ...; f32 startz = ...; if NULL Panic; if nonNULL stores`) — loads sink before the branch into f28/f29. (b) Cache one `FogData* pfd = &fogSlot.ref();` and store through it — single base load, pipeline collapses, size-exact. (c) Split declaration from assignment (`f32 startz; f32 endz; endz = ...; startz = ...;`) — loads follow assignment order, colors follow birth order; both match retail. Same lever as HBMMIX declare/assign split. (d) Byte-channel color packs: `u8 tmp[4]` + `GXColor c = *(GXColor*)tmp;` reproduces retail stb/lwz/stw; chained `r=g=b=v` assignment emits descending store order — use three separate statements for ascending.
- Result:    31.3% (126 structural) → 89.0% (0 structural, size-exact 0x48c) on FogManApplyFog (us-804a2020); residual is a 7-slot stack-layout permutation + SDA name drift (see stall_note in attempts.jsonl). Siblings unaffected (7/7 stay 100%).
- Confidence: repo_proven (levers a–d each hexdiff-verified, reversions verified clean)
- Applies to/a.k.a.: NW4R-assert-guarded setter blocks (g3d_fog.h Panic sites); store-block CSE via cached pointer/reference; decl/assign split for load-vs-color decoupling; u8-channel GXColor packing.

## Placement-new factory constant-name/result r30<->r31 swap is source-immovable; witness abi-boundary covers callee-saved regs (monolib CWorkControl::create)
- Symptom:   Factory `create` with early two-local shape (`const char* name = pool+off; T* p = new (getMem()) T(name, parent); entryWork(p, parent, false); return p;`) sits at 0 structural / 6 pure reg-swaps (retail name=r31/result=r30, decomp reversed), size-exact 0x74/0x74, relocs clean.
- Cause:     MWCC births the rematerializable constant-name web independently of source declaration/assignment order, so the name/result callee-saved claim order cannot be steered from source; only shapes that destroy the frame move coloring. The register-renaming witness refuses any rho perm touching r30/r31 (abi-boundary: ABI registers must be fixed), so EQUIVALENT_MATCH via witness is impossible even for a clean 2-register bijection on non-volatile regs.
- Fix:       none — all hexdiff-verified no-ops/regressions, reverted: decl order both ways, declare-then-assign (result-first), `&pool[0x6C]` array-index form, inline name into ctor arg (22 structural: 16B frame, 2 saved regs, name sinks post-null-check), removing `#pragma optimize_for_size` (23 structural, spills, 0x84 OVER — pragma required). Prior: base-type result, WORK_ID mem local (name sinks past allocate bl), explicit allocate+placement-new (124B), mw_version GC/3.0a5.2, -O4,s.
- Result:    near-miss CODE_MATCH 79.3% (0 structural / 6 reg_swap / 0x74 exact, us-804462f0); recorded with stall_note. Only FULL_MATCH can accept; next angle is type-driven IR difference (getWorkMem/allocate declared types shifting web birth order).
- Confidence: repo_proven (levers each hexdiff-verified on us-804462f0, Wii/1.1 monolib -O4,p + per-function optimize_for_size; closure: cycle witness-gate abi-boundary on r30->r31)
- Applies to/a.k.a.: placement-new factory functions with a constant string-pool name local; callee-saved r30/r31 claim-order walls; witness-closed abi-boundary on non-volatile regs.

## Configure-subtask u8*-param + typed cast-local + hoisted scalar args fixes saved-reg rotation across strcpy (monolib nand CNReqtask, FULL_MATCH)
- Symptom:   Configure helper (strcpy path + scalar stores + vtable-pointer return) stuck at 0 structural / 8 pure reg-swaps, size-exact: retail data->r31/arg2->r28/arg3->r29/arg4->r30 vs decomp data->r28/arg2->r29/arg3->r30/arg4->r31 — a full 4-slot rotation of the values saved across the strcpy call.
- Cause:     With a struct-typed first param used directly, MWCC births the param webs in ABI order so the first param claims the lowest saved reg (Rule B). Declaring the first param as opaque `u8*` plus a typed cast-local (`T* d = (T*)data`) and hoisting the scalar args to named locals before the strcpy births the data web last, so it claims r31 first and the args ascend beneath it — matching retail. No-ops ruled out (each hexdiff-verified, reverted): dropping the `d` alias, scalar-stores-before-strcpy, plain named arg-locals with struct-typed param, struct-typed param + late `d` copy with dual use (all pin the 8-swap rotation; coalescing normalizes same-type spellings).
- Fix:       `Ret* F(u8* data, const char* path, u32 a2, u32 a3, u8 a4) { T* d = (T*)data; u32 buf = a2; u32 size = a3; u8 flag = a4; strcpy(d->path, path); d->... = buf/...; ...; return vtable; }` — also aligns the definition with the existing `u8*` declarations at call sites (kills declaration drift instead of adding it).
- Result:    FULL_MATCH 100% / 0 structural / 0 reg_swap / size-exact on NandLoadConfigure (us-804df22c, monolib -O4,s); sibling NandRmConfigure (us-804df54c) is the repo-proven prior art with the identical recipe.
- Confidence: repo_proven (FULL_MATCH on two sibling configure helpers, monolib nand)
- Applies to/a.k.a.: NAND/CNRequest configure/init subtask helpers (Load/Remove/Readdir/Save families); any strcpy-then-stores configure with a rotated saved-reg set; u8*-vs-typed-pointer declaration drift.

## `.init`-resident EABI boot code: inline-SPR + frameless-r31 ⇒ C++-unreachable, witness-closed (record near-miss, do not churn)

- Symptom:   target lives in retail `.init` (e.g. `__init_hardware` 0x24: `mfmsr r0 / ori r0,r0,0x2000 / mtmsr r0 / mflr r31 / 3x bl / mtlr r31 / blr`). `hexdiff --symbol` errors `symbol not found` and lists only `.text` symbols — hexdiff reads `.text` only, so `.init` functions are invisible to it (confirm presence with a symtab dump, e.g. pyelftools: expect the symbol in the `.init` section index). `cycle` reports `STRUCTURAL` ~1% with `equivalence=inconclusive_unsupported (ambiguous decomp symbol, 0 candidates)` because the witness resolves decomp symbols by name in `.text` only.
- Cause:     the only high-level spellings of the SPR ops (`PPCMfmsr()`/`PPCMtmsr()` from `revolution/base/PPCArch.h`) are real cross-TU `asm` calls — retail precedent (`OSError.c` usage) compiles to `bl PPCMfmsr / ori / bl PPCMtmsr`, never inline `mfmsr/mtmsr`. Retail's inline SPR + zero-frame body with 3 live calls (LR parked in un-saved r31, no `stwu`) can only come from `asm`/`nofralloc` original source, which §17.6 does not cover (boot-vector exception is `InitMetroTRK*` only; PS-backend exception is paired-single only). C++ additionally always draws a frame for a non-leaf, so size can never fit (experiment: 0x34 vs 0x24).
- Fix:       none at source level — one C++ experiment suffices to prove the wall (frame + 2 extra `bl`s, witness 0-candidates), then REVERT (do not leave non-matching C++ in the tree: the linked DOL's boot path depends on the byte-exact `.init`). Do NOT `cycle` a byte-identical `asm` baseline either — a pass would auto-persist an un-acceptable acceptance. Record the stall_note and move on; a future §17.6-class exception for EABI `.init` boot code, or witness support for `.init` symbols, is the only acceptance path.
- Result:    near-miss (STRUCTURAL 1.1%, witness-closed) — `__init_hardware` us-800065c0 kept at pre-existing `asm` baseline, `.init` 0x64 exact, `.text` siblings untouched at 100%.
- Confidence: repo_proven (single decisive experiment + revert, verified byte-identical restore)
- Applies to/a.k.a.: any retail-`.init` function with inline privileged SPR (`mfmsr`/`mtmsr`/`mfspr`) or frameless callee-saved-LR borrow; `__flush_cache` (same TU, sibling scope) shares the section invisibility but was not evaluated here.

## (void*) round-trip cast-local recolors call-surviving spills with zero code change (monolib core func_eu_804F9E98, FULL_MATCH)
- Symptom:   Tiny strcpy-then-stores helper stuck at 0 structural / 6 pure reg-swaps, size-exact 0x48: retail req->r31/flagD->r29/flagE->r30 vs decomp req->r29/flagD->r30/flagE->r31 — a 3-slot rotation of the values spilled across the strcpy call. Witness-gate abi-boundary (rho maps gpr r29->r30), so only FULL_MATCH accepts.
- Cause:     With the struct-typed param used directly, MWCC births the request web first so it claims the lowest saved reg (r29); the sibling single-save function in the same TU (-O4,s) takes r31, confirming top-down claim order. Same-type spellings coalesce, so plain named locals in any declaration order are no-ops (hexdiff-verified: identical output).
- Fix:       Keep the retail signature verbatim and add one round-trip cast-local: `MonoRequestState* rs = (MonoRequestState*)(void*)req;` then use `rs` throughout. The extra IR node births the request web last, so it claims r31 first and the byte flags ascend beneath it (r29/r30) — byte-identical, relocs unchanged. Ruled-out no-ops (each hexdiff-verified, reverted): char-vs-u8 flag types, plain d/e/r named locals, per-function `#pragma optimize_for_size off` (emits separate stw saves, 0x58 OVER).
- Result:    FULL_MATCH 100% / 0 structural / 0 reg_swap / size-exact (func_eu_804F9E98, us-804f9e98, monolib -O4,s); TU promoted (4/4 symbols 100%, split PASS, data bypass:ok extern-only).
- Confidence: repo_proven
- Applies to/a.k.a.: the u8*-param + cast-local configure-helper recipe (prior entry, same lever with signature change); any strcpy-then-stores helper with a rotated saved-reg set where the signature must stay verbatim.

## Same-base field-load batch sorts ascending under Wii/1.1 scheduling; descending retail order is toolchain-closed, fields-gate witness-closed (CriWare SJ_SplitChunk)
- Symptom:   8-byte struct split: retail lwz r0,4(r3) / lwz r3,0(r3) (size-first, ptr load kills dead src) then ptr/size/size stores; decomp lwz r7,0(r3) / lwz r0,4(r3) (ptr-first) with identical stores+tail. hexdiff 86.4pct (0 structural / 3 reg_swap / size-exact), cycle symbol 99.2pct.
- Cause:     Wii/1.1 -O4 scheduler sorts same-base hoisted loads ascending regardless of source statement order (ptr-first, size-first, temp-hoisted all emit ptr,size). pragma schedule off/on bracketing the function IS load-bearing (emission flips to program/descending order) but the sched-off allocator assigns the first load r7 instead of r0 (5 reg_swap). Allocation (r0-first + r3-reuse) only occurs under scheduling, which always sorts ascending -- mutually exclusive, so no source shape reaches retail bytes with this toolchain. Witness fields-gate (slot 0 non-register bits differ) refuses the order swap, so only FULL_MATCH can accept.
- Fix:       none at source level under Wii/1.1 -- ruled out hexdiff-verified (reverted): size-first assigns, dst2-first, chained-size-first, both-locals, single-temp reuse, schedule-off bracket in both shapes. Next angle is TU-level mw_version archaeology (retail descending batch suggests CRI built sj_utl with a different scheduler) or order-tolerant witness.
- Result:    near-miss ACTIVE us-803997d4 (SJ_SplitChunk, CriWare sj_utl); source reverted to scaffold; TU not promoted (sibling SJ_SearchTag unmatched).
- Confidence: repo_proven (all six variants hexdiff-verified on us-803997d4; closure: cycle witness-gate fields|slot0)
- Applies to/a.k.a.: any same-struct two-field load batch where retail order is descending (offset 4 before offset 0) with dead-base reuse; pragma schedule off/on per-function bracket as a diagnostic lever (order flips but allocation shifts r0->r7).

## Named-vs-anonymous integer temp globally flips MWCC Chaitin homes; add-operand order controls RA/RB roles only (nw4r math_arithmetic FExp)
- Symptom:   FExp (detail::FExp, GC/3.0a5.2 -ipa file) stuck at 0 structural / 8 pure reg_swap / size-exact 0x8c: retail table triple (lis r5, idx r6, addr r4) uniformly one reg above decomp (r4, r5, r3), plus kshift/bits homes swapped (retail r3/r0, decomp r0/r3); tail `add r0,r3,r0` byte-identical by commutativity coincidence (retail RA=kshift, decomp RA=bits). Witness-gate abi-boundary (rho r4->r6) closes EQUIVALENT_MATCH; FULL_MATCH-only.
- Cause:     MWCC Chaitin coloring is virtual-numbering-sensitive, not forward-greedy: giving the shift result a name (`u32 kpart`, IDENTICAL live range [0x4c,0x74], line-neutral edit) flips the FIRST virtuals globally (k lha r0->r3, whole lhz r3->r0) with zero instruction-schedule change (12 swaps). Separately, add-operand source order selects RA/RB roles (`(k<<23)+expx` flips add to r0,r0,r3) but never the operand homes. s16->s32 widening is byte- and color-neutral when the loads already extend (lha/lhz). A +1 comment line above the TU changes nothing, delimiting the CMenuPTGauge -ipa line-sensitivity precedent to inline/scheduling decisions, not coloring.
- Fix:       none found in 5 probes + 2 diagnostics (all hexdiff-verified, reverted, file left unchanged at 77.1%/8 baseline): both add orders, early/late line-neutral kpart (early: scheduler keeps slwi @0x18, structural; late: global flip), s32 k, +1 line shift. The retail pattern needs a third integer live across [0x44,0x58] occupying r4 ( freeing by addr-time) with zero code change; no semantics-preserving source produces it. Record stall_note, keep baseline, move on.
- Result:    near-miss ACTIVE us-8040cf54 (FExp 77.1% hexdiff / 97.86% cycle, split PASS); TU not promoted (FLog sibling unmatched, tables locally defined).
- Confidence: repo_proven
- Applies to/a.k.a.: any 0-structural pure-regswap plateau where the table/index/shift triple sits exactly one reg off plus a swapped pair; probe add-order FIRST (roles vs homes disambiguation in one build); never repeat naming probes without a new lever.

## Goto-gate multi-exit dispatch with outlined R0 stubs (CriWare mpv_emp MPV_IsEmptyBpic, GC/3.0a5.2)
- Symptom:   B-picture macroblock_type dispatch (peek-6 range check + 11-check) at 27.3% with beq-to-inline-R0 vs retail beq-to-P6-forward plus `b`-to-outlined-R0; plain if/else (any sense: ==/!=, explicit-else/early-return, >/<=) always emits `cmpli; beq/beq->P6` with an INLINE `li r3,0; bclr`, and switch lowering emits cmpi-shape dispatch instead of retail subi+cmpli.
- Cause:     MWCC GC/3.0a5.2 inlines `return 0` in if/else-THEN position (same family as mps_del MPS_CheckDelim: plain if/else-if inlines the returns); retail outlines both dispatch returns as `b`-to-shared-trail blocks, which only the §17.6 goto-gate reproduces (labels in source order -> blocks in retail order).
- Fix:       `if (d22 <= 1u) goto p5; if (v6 == 11u) goto p6; goto ret0; p5: <5-bit consume>; goto join; p6: <6-bit consume>; goto join; ret0: return 0; join: ;` — ble->P5fwd / beq->P6fwd / b->R0 byte-exact, both P5/P6 keep trailing `b join` over the outlined return. Applies per dispatch site (Bpic needed it twice: first + trailing macroblock_type).
- Result:    27.3% -> 77.6% / 0 structural / size-exact 0x2dc (MPV_IsEmptyBpic us-803a9288, ACTIVE near-miss: residual is pure reg_swap, witness rho-blocked on prologue temp-merge, see stall_note).
- Confidence: repo_proven (if/else senses, switch bodies, optimize_for_size pragma all hexdiff-verified negative results first).
- Applies to/a.k.a.: any linear ble/beq multi-exit dispatch with outlined return stubs in CriWare sofdec (mpv/mps family); extends the MPS_CheckDelim goto-gate entry (that one has all-return exits, this one adds the join-over-outlined-return shape for consume-then-continue arms).

## YUV table init: builtin (float)(int) casts give retail fsubs+fmuls but MWCC pools the B18 magic (CFT_Ycc420plnToArgb8888Init, GC/3.0a5.2 -O4,p)
- Symptom:   hand-rolled `0x43300000` union + `(float)(u.d - bias)` emits `fsub`+`frsp`+`fmuls` per site (+1 insn/site, spills, size OVER) where retail has lone `fsubs`+`fmuls` (10 sites, no `frsp`, no pool, 90 instr exact). Plain `(float)(int-center)` casts flip MWCC to its own 0x4330 idiom (`subi`/`xoris` + `stw`-pair + `lfd`-magic + `fsubs`, NO `frsp`) feeding `fmuls` directly — code shape matches, size-exact 0x168/0x168, data 100% (decomp @-pool ignored by data comparison).
- Cause:     retail `fsubs` ⇒ builtin cast (rule); the union-double cast never folds under GC/3.0a5.2 or Wii/1.1 (6 shapes tested: k-first/k-last, named/unnamed, direct/indirect — all `frsp`). But the builtin idiom needs pooled B18 (`0x4330000080000000`) → TU-local `@N` (8B .rodata) + prologue `lis`/`addi`/`lfd` + 2 extra relocs; retail has NO pool (rodata exactly 104B). MWCC will not reuse a user bias variable for the idiom (different address, value unknown at compile time); deleting the bias var does not stop pooling.
- Fix:       write all 10 centers as builtin casts (`k * (float)(i-128)` etc., Y scale/add split across statements to defeat `fmadds` per the named-temp rule); fifth table reuses bumped base (`tblBase += 0x400` → `addi rX,rX,4096`); two counters (`i += 2` once + `j++` twice → single byte-IV bumped `+=4` twice). Result 1.1% → 40.1% (cycle), size-exact, data-100%, split PASS; witness gate moves from size to mnemonic (slot 0).
- Residual:   `@N` pool shifts prologue slots (witness mnemonic gate) + 64 structural loop-scheduling diffs + 19 reg-swaps; pool elimination vs prologue reorder are the next angles (small-data pool placement risks field-gate fail on offsets; postprocess rename forbidden).
- Confidence: repo_proven (fold/no-fold behavior, size-exact, data-100%) / open (pool + scheduling residuals). Example: us-8039eb5c.

## GXAdjustForOverscan ALU-before-memory pairs are scheduler tie-breaks; copy pointer type and temp decl order are color-neutral (RVL_SDK GXFrameBuf, Wii/1.1)
- Symptom:   GXAdjustForOverscan stuck at 4 structural / 56 reg_swap / size-exact 0x15c: retail subf-then-lwz @0xB4/0xB8 (fbW-h2 before viTVmode load) and sth-then-add @0x13C/0x140 (viYOrigin store before viXOrigin add); decomp emits each pair swapped (early-memory schedule). Copy region structurally clean but load order differs (retail word0 first into r22; decomp w1..w14 then w0 last) plus fbW/xfbH lhz order swapped (offsets 4/8) - all non-pure reg_swap, witness fields-gate closed (memory-contract SMT path closed under no-SMT policy).
- Cause:     Wii/1.1 -O4 list scheduler hoists independent memory ops above ALU ops; both orders are dependency-legal so no statement reorder pins retail's (fbWidth-first store lands pre-div; tv inner-scope delay reschedules badly; tail direct statements over-complete Y with no interleave). Copy load order and the w5..w14 callee partition do not follow temp declaration order.
- Fix:       none at source level in 7 hexdiff-verified probes (all reverted, file left as-found): fbWidth-first, fbWadj hoist temp, tv inner-scope, tail direct/split statements, tail decl swap, u8*-byte-offset vs u32*-index copy (byte-identical codegen - pointer type is neutral), temp decl-order reversal (partition unchanged). Next angles: register_mapping.md declaration-order contract for w0..w9->r22..r31 with word0-first loads; named prod/quot temps to reshape live ranges; WII_1_1_SCHED_CORE tie-break docs for the exact IR order.
- Result:    near-miss HIGH_MATCH us-8031d240 (31.0pct hexdiff / 91.0pct cycle, split PASS 0x8 spare); TU not promoted (GXCopyDisp/GXCopyTex siblings unmatched).
- Confidence: repo_proven (negative results) / open (tie-break levers)
- Applies to/a.k.a.: any ALU-before-memory independent pair where retail puts arithmetic first under Wii/1.1 -O4; sibling of the SJ_SplitChunk ascending-sort wall.

## Entry-store stwx scheduling + alias-temp recipe; r11 is witness abi-fixed (CriWare sfd_pts SFPTS_WritePtsQue, Wii/1.1)
- Symptom:   ring-queue entry write shows stwx+add vs add+stw pair plus interleaved vs hoisted pos/size loads (4 structural), collapsing to 0 structural / pure reg_swap once scheduled right, but the witness still refuses with `abi-boundary | rho perm maps gpr r7 -> r11`.
- Cause:     (a) When the entry pointer `e` takes a FRESH reg (not clobbering the base), MWCC keeps base+index live and can emit the first word store as indexed `stwx` with the `add` after it (retail order); when `e` clobbers the base reg, only D-form `stw` pair is possible. Declaration order `uc, e, next` (e before next) produces the fresh-reg shape. (b) After a store through `e`, a later `d->field` load of the sibling half is alias-killed and reloads -- pre-loading it into a temp (`u32 lo = d[1]`) before any store avoids the reload. (c) Size-first named temps force hoisted load-load-store-store instead of interleaved load-store pairs. (d) r11 is in the witness ABI-fixed set, so any residual pairing a scratch value with r11 is witness-closed (FULL_MATCH only).
- Fix:       `s32 uc = q->unk0C; SfdPtsEntry* e = &q->entries[uc]; s32 next = uc+1; u32 lo = d[1]; u32 size; u32 pos; e->u.hw[0] = d[0]; e->u.hw[1] = lo; size = d[3]; pos = d[2]; e->pos = pos; e->size = size;` + ternary wrap `(next < max) ? next : next-max` -- 0 structural, size-exact. Ruled-out negatives: indexed-first-store (base reload), halves without lo temp (alias reload), split decl/assign (stwux), drop-next (stwux fuse), d[] vs struct casts (identical).
- Result:    near-miss us-803ce260 (97.8% func / 73.0% code, 0 structural / 17 reg_swap, size PASS); entries->r11 color wall stands.
- Confidence: repo_proven (each shape hexdiff-verified, 16+ builds) / open (r11-forcing lever).
- Applies to/a.k.a.: any multi-word entry write through a fresh pointer where retail uses stwx+add; the lo-temp alias rule generalizes to any sibling-field load after a store through the same pointer.

## inflate_fast table-entry access: struct-copy spills, pointer + eager val-carriers; lbzux fusion and cross-jump walls (monolib zlib, Wii/1.1 -O4,p)
- Symptom:   ZlibInflateFastCore at 0.0pct / 309 structural / 0x568 OVER(92): decomp spills Huffman entry (stb/sth to stack + reloads) where retail loads fields straight from table (lbzux/lbz/lhz); lazy here->val reloads at every use (+1 lhz).
- Cause:     InfCode here; + here = lcode[hold & lmask] forces a 4B stack temporary under Wii/1.1 (no scalar replacement across goto-dispatch); each here->val use reloads because no local carries the value.
- Fix:       const InfCode* here + here = lcode + (hold & lmask); assign len/dist EAGERLY right after DROPBITS (len = here->val before op-dispatch) and reuse len/dist at literal store, length/dist base, and 2nd-level index (lcode + (len + ...)) instead of re-reading here->val. Result 0.0pct/0x568 -> 2.2pct positional / 81.5pct cycle / 0x504 PASS, spills gone, lhz 5->2, sibling Adler-32 100pct intact, unit split PASS.
- Residual:   retail 323 vs decomp 321: retail has FOUR independent (and/slwi/lbzux/lbz/lhz) triples; MWCC cross-jumps explicit 2nd-level triples into shared tails (327 with redundant bits loads when forced - reverted); add+lbz never fuses to lbzux for named-pointer locals, nor does u32-index table[hidx].field form (317 - reverted). Global recoloring (stmw r19 vs r18). witness-gate: size only, fuzzy 89.8pct.
- Confidence: repo_proven (pointer + eager-carrier improvements, both negative results hexdiff-verified) / open (fusion trigger, cross-jump inhibition, Rule-A color sweep)
- Applies to/a.k.a.: any table-driven decoder with goto-shared lookup (inftrees/inflate); us-804646f8.

## Singleton-factory saved-reg rotation: 2nd-half webs recolor entry; bistable 32/38 landscape; witness abi-boundary (CCol6Invite ctor, Wii/1.1 -O4,p)
- Symptom:   self-allocating singleton factory (singleton check, work-mem alloc, base-ctor bl, vtable + __ptmf_null double-triple init, field block, singleton store + Regist) shows 0 structural / 32 reg_swap: saved regs rotated one slot (obj->r27 + params->r28-r31 vs retail params->r27-r30 + obj->r31) plus an r7/r8 scratch swap on the 2nd ptmf reload pair. cycle witness-gate: `abi-boundary | rho perm maps gpr r27 -> r28` -- bl-containing factory with callee-saved rotation is witness-closed (FULL_MATCH only).
- Cause:     the 2nd callback triple's webs (multi-def word reloads + 3 stores) perturb Chaitin's simplify/pop order for the whole function: deleting the 2nd half (B3) makes the entry byte-exact; restoring ANY 2nd-half form re-rotates (reassign=+1 shift; late fresh webs=+1 shift plus ptmf r9->r8; early fresh webs=same as reassign). Same-asm streams color differently by vreg birth numbering (post-scheduling PCode graph), so the rotation is a scheduler-driven numbering wall, not a statement-order problem -- and the scheduler keeps source order (Hint precedent), forcing loads-[1],[0]/stores-[0],[1] per triple.
- Fix:       none found (near-miss). Best reachable is the 32-state: extern "C" unmangled def, single shim pointer, split workMem temp, init-decl word locals with multi-def reassign for the 2nd triple, explicit casts, plain reload tail. Biggest prior win was fixing a symbol-absent mispair (TU defined mangled Create; retail wants unmangled __ct__ factory) -- always nm-check the decomp object when hexdiff pairs a same-size sibling.
- Result:    near-miss us-80165088 (54.3% / 0 structural / 32 reg_swap / size PASS, ACTIVE). Ruled-out levers (each hexdiff-verified, ~30 builds): single-vs-dual ptr, w2 local vs direct, split vs init decls, nested vs split workMem, u32 vs unsigned-long ptmf view, CCol6Invite* vs CProcess* self, C vs C++ linkage, uintptr tail dance, field6C cast form, implicit void* cast, top-declared ptmf, real-class obj with implicit upcasts, triple-swap, early/late fresh 2nd-triple webs, store-swaps (scheduler keeps).
- Confidence: repo_proven (bisection + bistability + witness-gate, all hexdiff/cycle-verified) / open (numbering wall needs mwcc-wii-1.1 Coloring probe; -O4,s scheduling variant out of scope)
- Applies to/a.k.a.: any inline-init singleton factory with 2+ live params (1-param CheckBat sibling is clean at 93.9% with only the pair-2 swap left); all repo FULL_MATCH factories delegate init to a __ct__ call -- when retail inlines, expect this wall.

## Flat-named template ctor (__ct__reslist_X): novtable subclass pairs digit-strip and reproduces the template schedule; hand stores cannot (monolib CScnItemPool, Wii/1.1 -O4,p)
- Symptom:   retail flat `__ct__reslist_CScnItem` (0x3C, double vtable-store schedule) vs decomp weak `__ct__19reslist<P8CScnItem>Fv`: byte-identical bodies but the flat name never pairs (0 candidates), so the target sits at 0% while the code already matches. Hand-written flat-named C++ def (`__ct__reslist_CScnItem__FP...`, pairs by substring) stalls at 73.3% (li r0@+10 vs retail +4; addi r5@+4 vs retail +10; knock-on lis/stw swap) across all variants.
- Cause:     (1) pairing: substring needs contiguous `reslist_cscnitem`; template mangling inserts the length prefix (`19reslist<...>`), digit-strip still mismatches (`<p...>` vs `_c...`). (2) schedule: the template's vtable stores are IMPLICIT (compiler-generated, DSE-immune, scheduler-pinned); hand-written explicit stores DSE-fuse unless exactly one is volatile, and the li/addi tie-break differs structurally (proven insensitive to volatile/zero-local/decl-order/split-function/placement-new variants over 7 builds -- placement-new additionally emits a null-check branch). The template explicit-instantiation context is the only proven source of the retail order.
- Fix:       real subclass `struct __declspec(novtable) reslist_CScnItem : public reslist<CScnItem*>` with out-of-line `reslist_CScnItem() : reslist<CScnItem*>() {}`: ctor mangles to `__ct__16reslist_CScnItemFv` (pairs via digit-strip fallback, unique), base init inlines with the template phase structure (retail schedule, 100% insn), novtable suppresses the third vtable store, no weak standalone emitted (size-neutral), dtors/vtables via dtor-only explicit instantiation (`template reslist<CScnItem*>::~reslist();`). Relocs name the weak vtables; the TU's UNIT_RULES renames them to lbl_eu_* at link.
- Result:    hexdiff 100% (15/15, 0x3c/0x3c, size PASS) but cycle fail-closed: objdiff under-scores the name-mismatched pair (0.0%) and the FULL_MATCH cert + witness both refuse the two derived-vtable reloc names (`__vt__19reslist<P8CScnItem>` vs `lbl_eu_8056E540`; the base pair IS canonicalized via the mined map). The miner can never learn the derived mapping (chicken-and-egg: the only exact-name pair exhibiting it is the unaccepted target) and the linker layout cannot resolve weak names -- needs map seeding from the TU's own UNIT_RULES exact_renames or template-aware pairing, then acceptance with zero source change.
- Confidence: repo_proven (pairing gap, scheduler wall, DSE/volatile behavior, novtable suppression, size neutrality -- all hexdiff/cycle-verified) / open (map-seeding acceptance path not yet executed)
- Applies to/a.k.a.: any flat-named template ctor/dtor (`__ct__reslist_*`, `__dt__*` flat names); CScnEnvLgtCtrl `__ct__reslist_IScnEnvCtl` precedent (same spelling trick, foreign-owned labels); COccCulling novtable precedent; CBattleManager FULL_MATCH-with-name-drift precedent. Evidence us-8048ffb8.

## CriWare DCT 8x8 IDCT double loop: pair-unrolled fmadd chains need base+cursor spelling; scheduler interleave vs unroll-copy sequentiality wall (dct_ac, Wii/1.0 default)
- Symptom:   DCT_AcIdctDouble (0x22C, 139 insns): plain triple loops give 0.0% / 0x2E4 (k x4-unrolled, indexed slwi addressing). Explicit-8 sums give rolled ctr-8 loops (0x244). Step-1+peel+trip-7 nest unrolls jx2 sequential BUT duplicates ip loads across copies (0x238). Best: j+=2 pair loops + per-output cursor + peeled trip-7 k-loops + peeled-4 explicit seed terms: 0x220/88 structural (seed-4) or exact 0x22C two-loop variant (91 structural, 60.3% fuzzy, size+budget PASS).
- Cause:     (a) Retail uses pointer+byte-cursor addressing throughout (lfdx base,cursor + add recompute + const offsets, zero per-output loads, tp/op bumped per pair) -- plain indexing emits slwi+lfdx and hoisted offset regs. Spell as `cp[c]` + `(cp+c)[k*8]` with `c++` per output, `tp+=2`/`op` bumps per pair, tmp column hoisted to scalars via a column pointer (`colp[0,8,...,56]` gives single-base+const offsets, lfdux). (b) MWCC interleaves two independent output chains in one scheduling region (fmr WAR-copies, shared-then-reused regs) while retail is sequential; the only sequential mechanism is unroll-after-schedule of a single-output region (nest), which then misses post-unroll CSE (dup loads). Explicit pair form shares (CSE) but interleaves -- fundamental tension, last mile is scheduler/allocator (fmr->nothing, zero-split, f29-f31 pressure for saves, `lis/addi/b.+4/stwu` prologue).
- Fix:       row: `ip=&in[i*8]`, `tp=&tmp[0][0]+i*8`, `c=0`, `for j+=2`: peel seed `s=zero+cp[c]*ip[0]` + 3 explicit terms + trip-4 k-loop (k=4..7), store, `c++` per output, `tp+=2`; col: hoisted `t0..t7` via `colp=&tmp[0][i]`, `cp`+`op=&out[i]` cursors `cb++`/`ob+=8` per output with recompute+advance (stfdx+addi-64). Exact class composition needed: 3x fmr->lfd + 2x stfd + 1x b, nothing else.
- Result:    near-miss us-803bd348 (seed-4: 0x220/136 vs 0x22C/139, 88 structural/51 reg_swap, budget PASS; two-loop variant: exact size, 91 structural). Ruled out: triple (x4), explicit-8 (rolled), step-1 straight (rolled over budget), seed-5/6 (trip-3/2 worse), operand swap (neutral).
- Confidence: repo_proven (pair structure, cursor addressing, peel-lfdx, seed/dup scaling, class-composition accounting -- all hexdiff/cycle-verified, 15+ builds) / open (sequential-schedule lever, zero-split lever, pressure lever).
- Applies to/a.k.a.: any paired-output FP kernel with fully-unrolled inner sums (IDCT/DCT/FFT butterflies); the base+cursor + peel-seed idiom generalizes to column/row-strided table access.

## Kept address-only mid-base (stw rX,N(rBase) with addi rBase,r3,const): straight-line constant-index stores always fold; only -O4,s loop-IV form keeps the base (mpv_cmc, GC/3.0a5.2 + Wii/1.1)
- Symptom:   retail `mpvcmc_InitMcOiTa` (0x58, 22 insn) materializes `addi r5,r3,0xAD8` + `addi r6,r3,0x380` pre-branch, then 6x `stw r6,0..40(r5)` + 6x `stw r0,4..44(r5)` (flags `stw r4,0xAD4(r3)` via r3). Every decomp spelling folds the 0xAD8 base into r3-relative stores (21 insn, 0x54): array `e[i]`, named members `e->pN`, flags-via-`e[-1]`, `for` loops (unrolled), const-first `(u32)base+0xAD8`, `#pragma opt_propagation off`, `#pragma global_optimizer off` -- hexdiff 4.5%, cycle HIGH_MATCH 87.4% fail-closed on witness-gate: size (22 vs 21).
- Cause:     MWCC address-mode selection folds an address-only (never stored/compared/called) base with constant offsets into the cheapest live base (r3); loop-IV form under -O4,s is the only shape that keeps it (`addi r6,r3,0xAD8` + `addi r4,r3,0x380` hoisted pre-branch, `stwx`/`add` ctr loops) -- but adds branches retail does not have. Version- and opt-independent: GC/3.0a5.2, Wii/1.1, GC/2.5-2.7, GC/1.3.2, Wii/1.0 x -O0..-O4,p/-O4,s all fold straight-line; -O2/-O3 additionally sink the lwz below the addis (retail: lwz first => -O4,p schedule).
- Fix:       none in-scope (near-miss). Next angles in order: (1) TU-level `mw_version` sweep for mpv_cmc (Sofdec family is retail Wii/1.1 per 2026-08 sweep; sibling mpv_cdec/mpv_mcy already carry `mw_version="Wii/1.1"`) -- needs configure.py edit + full-TU re-baseline incl. accepted MPVCMC_InitObj; (2) per-function unroll pragma under -O4,s if MWCC exposes one; (3) TU-split if (1) fixes Ta/Rt but regresses InitObj. Do NOT retry source spellings -- the fold is exhaustive.
- Result:    near-miss us-803a7de8 (HIGH_MATCH via cycle, hexdiff 4.5% / 7 structural / 14 regswap / size 0x58 vs 0x54 PASS, split PASS 0x8 spare). Sibling MPVCMC_InitMcOiRt shares the identical wall (base 0xAA4, 6.2%).
- Confidence: repo_proven (10+ scratch MWCC probes across 5 versions x 5 opt levels + 2 pragmas, all objdump-verified) / open (mw_version sweep, unroll-pragma lever).
- Applies to/a.k.a.: any branchless straight-line function whose retail keeps an address-only mid-base (mpv_cmc Ta/Rt pair); the -O4,s loop-IV base-keep is the recipe when retail itself loops.

## Allocation-root-cause clustering: prologue order + indexed-load/mull swap + saved<->volatile home swap are ONE coloring divergence (ADX_DecodeMono4, Wii/1.1 -O4,p)
- Symptom:   hexdiff shows a few structural slots (here 7: 5-slot prologue li-vs-stmw/lis/lha/addi reorder + 2-slot lwzx-vs-mullw swap) plus a large pure reg_swap rotation (here 30: block<->prev_lo and table/byte/nib/index rotation), with saved-set COUNT exact (stmw r25 both) and size exact. cycle witness-gate reports mnemonic at slot 1.
- Cause:     A single Chaitin coloring divergence: the thin long-lived web (loop counter) landed in volatile (r11) instead of saved (r31). Volatile init floats above stmw (prologue order + reloc drift for free); the indexed-load reuse victim changes with it (lwzx/mullw order follows the reuse, scheduler just obeys). Declaration order, loop-scope-vs-function-scope birth, and statement hoists were all inert or regressive; -O4,p vs -O4,s schedule identically (no unroll either side), so flags cannot fix it.
- Fix:       Fix the HOME of the displaced web and all three symptoms clear together; do not chase the prologue order, reloc drift, or load/mull order independently (they are consequences). If order levers are inert, it is a degree/liveness wall: reshape a web live range without changing the saved-set count (adding/removing a live web flips stmw and explodes the diff). Witness is likely abi-closed next on the saved-reg permutation, so this class needs FULL_MATCH.
- Result:    Near-miss documented (53.2% strict / 91.8% fuzzy); ruled out: decl-swap (null), temp-scope promotion (null), load hoist (53.2->22.8% regress, reverted).
- Confidence: hypothesis (single-function evidence, mechanism consistent with register_mapping.md degree-driven reuse + scheduling.md p/s identity)
- Applies to/a.k.a.: any CTR-loop kernel where a prologue-order structural co-occurs with a saved<->volatile home swap; check the saved-SET match first (stmw range) to confirm count-exactness before any lever.

## LOD lerpDistEntry: u16/s32-lerp recipe (for-CTR + spanF-first slots + leftP madd) + two walls (Wii/1.1 -O4,p, us-804670e8)
- Symptom:   linear u16-far-bin lerp over stride-8 entries: do-while counter source gives addic loop + 36 structural (43.8%); union-double scratch conversions emit fsub+frsp (+size OVER).
- Cause:     retail loop is mtctr/bdnz pointer walk with two persistent 0x4330 scratch slots (B unsigned in slotB 16(sp), signed span in slotA 8(sp)); retail fsubs sequence IS the builtin-cast idiom (union-double never folds).
- Fix:       (1) for-shape loop (`for (s32 j = 1; j < cnt; j++)` + `e++` in body) reproduces mtctr/bdnz + pointer walk exactly (findCutBinIdx precedent, same TU); (2) plain builtin casts `(f32)u16`/`(f32)s32` only; (3) span conversion textually FIRST (`f32 spanF=(f32)span` before any B conversion -- MWCC evaluates div left-first, first conversion takes slotA); (4) lerp tail as `f32 leftP = e[-1].near * (unit - t); return leftP + e->near * t;` -- forces prev-load-first order and prev*oneT-first fmuls/fmadds assignment (inline return swaps both); (5) keep the region line-count neutral (pads) so -ipa file siblings do not shift.
- Result:    near-miss 87.7% hexdiff / 96.0% objdiff, size-exact 0x124/0x124, all operation positions + slots + loop/GPRs exact; residual is 5 STRUCTURAL prologue loads (retail lhz/stwH/lfdM/stwL/lfsT vs lfdM-first -- MWCC hoists the pool-magic lfd above lhz in Wii/1.1+Wii/1.0+GC/3.0a5.2 x -O4,p/-O4,s x fp_contract on/off, direct/temp/helper spellings; GC matches high-first stores but would flip findCutBinIdx low-first) + 4 pureReg num/unit FPR swap (retail num f5/unit f2; spanF-first/bF/named-num/oneT/prevN/curN/unit-before-after-t variants all rotate elsewhere). Witness mnemonic-gate closed, needs FULL_MATCH.
- Confidence: repo_proven (CTR recipe, slot-order rule, leftP order rule, fsubs=>builtin-cast rule, hoist universality probes -- 15+ builds) / open (lhz-first scheduler lever, num/unit graph lever)
- Applies to/a.k.a.: any u16->f32 conversion prologue + lfsx/lfs indexed early-outs (findCutBinIdx same-TU twin wall at 92.1%); catmullRomBin/fetchSampleXY same family.

## initEvtMemArena (CREvtMem): volatile-ABI pure-reg_swap wall + res-dest reuse rule (Wii/1.1, CODE_MATCH 78.2% near-miss)
- Symptom:   `initEvtMemArena` (0x1b8, 110 insns): 0 structural, 24 pure reg_swap, size-exact. Retail (m:singleton r6, pos r5, pad r4, res r4) vs decomp (m r4, pos r5, pad r6, res r5). `cycle` witness-gate: `abi-boundary | rho perm maps gpr r4 -> r6` -- volatile/call-arg regs must be fixed, so EQUIVALENT_MATCH is closed; FULL_MATCH only.
- Cause:     The 4 long webs (m/pos/pad/res) permute over r4/r5/r6 while every temp (bound/check/add in r3, arenaSize/end/low/chunk+pad sums in r0, reloads in r3) matches. res-add `add rD,rA,rB` reuses a source reg, never fresh r0/r3: dest = the reg of the LOWER-declared of {pos,pad} (baseline decl [..,pos,..,pad] -> res uses pos r5; pad-first decl -> res uses pad). m is first-claimed (r4) in every variant except check-via-direct-temp (temp takes r4, m->r5); pad-first decl moves pos (r5->r6) instead of m. So m<->pad never swap while pos stays fixed: births [pad,pos,m] (pad first) would be needed, but a live early pad-assign is unconstructible (hoisted `pad = 0` is NOT sunk by the scheduler -- emits early `li`, +11 structural; loads cannot sink across the may-alias field stores; bare hoisted decls birth late at assignment).
- Fix:       none in-scope (near-miss). Reuse rule (actionable): to flip an `add`-result into the other's reg, swap the DECLARATION order of the two source locals (lower-decl# wins the dest), not the operand order (rA is fixed by the source expression and mismatching it goes structural). Do NOT chase: hoists (li/loads emit early), shared locals across the MEM2-call boundary (would force saved regs + prologue change -- verified absent: prologue saves only r30/r31), check-direct temps (shift m r4->r5 only), end-local (same swap), operand swaps (rA/rB fields change => structural), overload spellings (relocs already clean: `allocate_head FUlUli`).
- Result:    near-miss us-80168f1c (78.2% hexdiff / 98.8% cycle-fuzzy, 24 reg_swap / 0 structural / size-exact / relocs clean / split PASS with 0x0 spare). Ruled out (~18 builds, all reverted to baseline): res/pos swap (null), pad-first (28, worse), end-local (m<->pos swap), check-direct (m->r5), global-cond+m-inside (CSE merges to baseline), all-direct (same), single shared m (null, no prologue change), shared pos/pad (null), m1-init hoist (+5 structural), hoisted pad=0 (+11 structural, li not sunk), hoisted bare decls (late birth), inline-& (CSE no-op).
- Confidence: hypothesis (single-function evidence, res-rule 6/6 variants) / negative_result (hoist/sink, shared-local, temp-shift levers)
- Applies to/a.k.a.: any arena-carve/align idiom with a cached struct pointer + pos/pad/res webs over volatile regs where the witness gate reports abi-boundary; the res-dest/decl# rule generalizes to any 3-address `add` whose dest reuses a dying source.

## CriWare Sofdec row-pipelined unrolled copy — `#pragma scheduling off` kills chain-hoist; straight-line alternating temps reproduce retail order (US, Wii/1.1)

- Symptom:   hand-unrolled 16-row straight-line copy (MPVMC16_OneRef1p_TuneC us-803b15dc, 0x650) shows a hoisted pointer-chain advance (`add` for G1 pulled above G0 loads/stores) + spilled data temp + frame under default `-O4,p`, even when source order matches retail exactly.
- Cause:     MWCC's scheduler eagerly computes the pointer chain (`q`, `q+step`, ...) ahead of loads/stores (critical-path bias) and sinks late-born temps; loops cannot reproduce the shape (8-trip simple loops fully unroll but a pipelined single-row loop stays CTR-rolled, and same-name pipelined loops coalesce the cur/next rotation so no even/odd register alternation emerges).
- Fix:       (1) write the pipeline straight-line with alternating temp sets (even rows one set, odd rows another) in exact retail op order (quirky load/store sequence verbatim, `*(p + step)` indexed form alongside a separate `q = p + step` — MWCC keeps both the `lwzx` and the plain `add`); (2) bracket the function with `#pragma scheduling off/on` (function-granular, honored; AXFX precedent) — this pins advances to program order and stops the hoist, while loop-optimizer hoists (`cmpi`/`dcbt` to loop top) and `lwzux` fusion survive; (3) declare temps just-in-time to shorten ranges (moved prolog `src` r4→r5 in isolation).
- Result:    order-identical output (260 structural + 148 regswap collapse to prolog/frame pairing once aligned); residual is one r12-reserved spill (`b3`→saved `r31` + frame) that resists all source steering — recorded as near-miss.
- Confidence: repo_proven (order/hoist parts verified by hexdiff before/after pragma; spill-wall is a negative_result after ~60 variants)
- Applies to/a.k.a.: hand-unrolled copy/interleave loops, chain-hoist, program-order pinning, `#pragma scheduling off`, CriWare Sofdec mpv family

## Two-address-const store lis-interleave wall — value-chain-first is scheduler tie-break, not source order (Wii/1.1, MPVABDEC_Init us-803b276c)
- Symptom:   6-insn init `dst[2] = (u32)src` with both sides absolute addresses: 0 structural, 4 pure reg_swap, size-exact. Retail interleave base-first (`lis r3=dst, lis r4=src, addi r3, addi r4, stw r4,8(r3)`); decomp value-first (`lis r4, lis r3, addi r4, addi r3`, same `stw`). Register allocation already identical (Rule C scratch low→high: base=r3, value=r4 both sides). Witness-gate reloc refusal (slot0 symbol order) closes EQUIVALENT_MATCH — FULL_MATCH only.
- Cause:     MWCC IR evaluates the stored value before the store address for this pattern, and the list scheduler emits the value chain first regardless of source statement/declaration order. Diagnostic proof: `#pragma scheduling off` yields sequential value-first (`lis r4, addi r4, lis r3, addi r3`), i.e. IR order itself is value-first — so retail's interleaved base-first order is a scheduler tie-break difference (likely mw_version), unreachable from source under this compiler.
- Fix:       none in-scope (near-miss). Do NOT chase: ptr-temp with base-first statement order (scheduler reorders across statements), extern declaration-order swap (null), struct-member store with identical reloc names (null), integer base+offset deref (null), byte-array +8 deref (null), `#pragma scheduling off` (regressive: 2 structural + 2 reg_swap). Next angle: configure-level mw_version probe for the CriWare TU (which scheduler orders the base chain first).
- Result:    near-miss us-803b276c (cycle HIGH_MATCH 91.7% / hexdiff strict 33.3%, 0 structural / 4 reg_swap / size-exact); 7 variants ruled out, source reverted to single-statement baseline.
- Confidence: negative_result (7-build evidence + pragma diagnostic; mechanism consistent with register_mapping.md Rule C + scheduling.md scheduler-before-coloring)
- Applies to/a.k.a.: any single store of one absolute address into another (`*p = (u32)q`, table-pointer init idiom) where stw/regs match but the two lis/addi chains interleave oppositely; check allocation-identity first (same stw regs) to confirm scheduler wall vs color wall.

## MPV_GetVbvBufSiz (CriWare mpv_get): same-graph color cascade + color-preserving operand flips (Wii/1.1, CODE_MATCH 80% near-miss)
- Symptom:   `MPV_GetVbvBufSiz` (0xC8, 50 insns): 0 structural, 10 pure reg_swap, size-exact. `cycle` witness-gate: `abi-boundary | rho perm maps gpr r4 -> r5` -- volatile/call-arg reg conflict closes EQUIVALENT_MATCH; FULL_MATCH only.
- Cause:     A single first-divergence (bitRate temp retail r4 vs decomp r5) cascades self-consistently through the `__mulhw` magic-divide cluster (frameRate r0/r4, magic r3/r0, prod r0/r3, hi r3/r0, x r0/r3, sign r3/r0; sum+final r0 both sides). Both sides are valid colorings of the same interference graph, so the only lever is vreg numbering/birth order -- but the scheduler preserves source order here (store-swap emits in source order, +3 structural), pinning numbering to the fixed emission order.
- Fix:       none in-scope (near-miss). Reusable rules: (1) trace a pure-reg_swap diff to its FIRST divergence and check cascade-consistency before steering -- if both colorings are self-consistent, only numbering levers apply; (2) `a+b` vs `b+a` (and mullw/mulhw arg order) with identical births/ranges is COLOR-PRESERVING -- it flips only the ra/rb slots of that one insn, so operand flips can never fix a color wall alone (but pair one with a color fix, since the flip itself is predictable); (3) ra/rb slot evidence (mullw ra=frameRate, mulhw ra=magic, adds ra=hi/ra=x) confirms expression operand order without a build -- do not flip what the slots already confirm. Do NOT chase: cast add/remove (null), split declaration+init (birth is assignment-driven, null), store/load reordering across independent addresses (scheduler keeps program order here), unsigned prod with signed conversions (null), branch-polarity flips (layout matches), hoisted-range-shortening locals (contradicted by temp ownership: addis-temp owns r0 in retail), sum/x web merges (add rd differs, would need a visible mr).
- Result:    near-miss us-803a9d4c (80% hexdiff, 10 reg_swap / 0 structural / size-exact / split PASS). Next angles: hand-`__mulhw` may number differently than compiler-expanded signed division (solve divisor D from magic 0x91A2B3C5/shift-10, try `prod/D` source); remaining split-init declaration permutations of prod/hi/x.
- Confidence: negative_result (4 builds + contradiction analysis; cascade-consistency and color-preserving-flip rules verified against both asm dumps) / hypothesis (division-expansion numbering angle, untested)
- Applies to/a.k.a.: any magic-divide (`__mulhw` + add + srawi + rlwinm-sign + add) cluster with an abi-boundary r4/r5/r6 witness refusal; first-divergence cascade tracing for same-graph color walls.

## const-pointer load hoisting across struct-copy stores — mutable param pins retail schedule (Wii/1.1, CERotEulerOrder us-804dfc98 FULL_MATCH)
- Symptom:   6 identical per-case regions (struct-copy 12 pairs + inlined table-sincos): decomp hoists the angle load + const load + fmul above/into the copy stores (`lfs f1,8(r31)` first, pairs split, idx in r6); retail does copy pairs first with only const/magic-double loads interleaved and the angle load after the copy (`lfs f2,8(r31)` last, pairs intact, idx in r3). 198 structural + 72 reg_swap; witness-gate mnemonic refusal (slot 108 lwz vs lfs) closes EQUIVALENT_MATCH. Copy#2 (no following trig) and trigs #1/#2 (call-bounded) match byte-for-byte.
- Cause:     MWCC treats loads through a `const`-qualified pointer as unmodifiable and hoists them above unrelated stores (critical-path scheduling); through a mutable pointer the same load may-alias the stores and stays pinned after them. The TU's header/recovered decl said `const Vec*`, but retail codegen matches the mutable-pointer schedule (matches the `Mtx* out, const Vec* p, s32 field` header everywhere except this constness). Ruled out (all byte-identical no-ops): trig-helper variant swap (IdxFirst vs Third/commuted multiply canonicalized), angle-spill-local removal (`f32 az` vs direct `ang->z` — same IR), `memcpy` copies (MWCC emits real `bl`, 9.4%), 12x explicit float assigns (float pipeline `lfs/stfs` + `FC00181E` fctiwz variant, 63.7%).
- Fix:       declare the angle pointer mutable in the DEFINING TU only: `void CERotEulerOrder(ml::CMat34* mtx, Vec* ang, int order)`. One word, line-neutral, no header/caller/configure.py change (extern "C" symbol unchanged; other TUs still see the header's `const Vec*`; no writes through `ang` in the body so runtime-safe). Scheduler then pins all 6 angle loads after their copy stores; colors fall out identical (72 reg_swap -> 0).
- Result:    FULL_MATCH us-804dfc98 (hexdiff 100.0%, 0 structural / 0 reg_swap / size 0x1108 exact / split PASS 0x0 spare; cycle semantic-certified). Sibling CERot_AxisToMatrix still 100%; XYZ/ZXY tails (same disease, separate targets) untouched.
- Confidence: repo_proven (single-target FULL_MATCH; mechanism explains all 6 regions + why copy#2/trigs#1/#2 already matched)
- Applies to/a.k.a.: any `*dst_struct = tmp` followed by float computation loading through a `const T*` param where decomp hoists the load above the copy stores but retail does the copy first (sdata/absolute loads still hoist in both — only the pointer-relative load is pinned); check `const` on pointer params before chasing declaration-order/expression-order levers. Sibling instances: CERotEulerXYZ / CERotEulerZXY tails (same TU, same fix expected).

## LightCtlCopyObj (monolib CLight): stub-mispair linkage + void-vs-pointer return tail (Wii/1.1, FULL_MATCH)
- Symptom:   hexdiff 0.0%, 45 structural, retail 0xB4 vs decomp 0x8; decomp symbol reads `LightCtlCopyObj__FPUci` (C++-mangled) while retail is the flat `LightCtlCopyObj`. After the linkage fix, 86.7% with decomp 4 bytes short and only the epilogue differing (missing trailing `or r3,rdst,rdst` before `bl _restgpr_27`).
- Cause:     (1) The TU stub was declared `(u8*, int)` without `extern "C"`, so the decomp object held a mangled symbol and hexdiff paired it against a 2-insn stub. (2) Retail copies all plain fields, calls `LightObj::operator=` through the 0x2C pointer, copies the trailing field, and returns dst -- the `void` return dropped the load-bearing `or r3,rdst,rdst`.
- Fix:       `extern "C" CLight* LightCtlCopyObj(CLight* dst, const CLight* src)` with field copies (`CVec3` assigns for the vectors, raw `u32`-word copy for the four scalar floats at 0x1C..0x28 per the LightCtlSetPos idiom so retail `lwz/stw` reproduces instead of `lfs/stfs`), `*dst->mpLightObj = *src->mpLightObj;`, trailing `dst->unk3C = src->unk3C; return dst;`. `const` src lets MWCC hoist the field loads above the stores; vtable (offset 0) and the LightObj pointer itself are never copied. Callers passing `u32` addresses (`CScnVirtualLight`) are ABI-unaffected.
- Result:    FULL_MATCH us-804c4440 (hexdiff 100.0%, 0 structural / 0 reg_swap / size 0xB4 exact; cycle semantic-certified). No sibling movement (6/13 -> 7/13).
- Confidence: repo_proven (single-target FULL_MATCH)
- Applies to/a.k.a.: any 0%-stub target whose decomp symbol carries a `__F...` mangling suffix vs a flat retail name (check linkage before any source experiment); any copy helper whose decomp is exactly one instruction / 4 bytes short at the `_restgpr` tail (check for a missing pointer return).

## CriWare Sofdec 8x8 1-pel motion-comp kernel — unrolled (src&7) dispatch with update-load chase (mpv_mc MPVMC08_OneRef1p_TuneC us-803ac1b0 near-miss)
- Symptom:   8x8 byte-copy kernel dispatching on `(u32)src & 7` with 5 straight-line bodies (no branches; each ends `blr`): case0 = 8 FPR doubles (`mr` + 7x `lfdux` + `lfd` + 8x `stfd`), case4 = 8 word rows (`mr` + 7x `lwzux`-led pairs + 16x `stw`), case2/6 = halfword recombine (`clrrwi` stride mask + `srwi`/`rlwimi` + plain `add` stepping), cases1/5 and 3/7 = byte-shift recombine with per-row `dcbtct`. Looped high-level source gives ~11% (148 structural, loop branches, missing dcbts); fully unrolled source reaches instruction-count parity in cases 0/4 with reg color as the main residual.
- Cause:     (1) Update-load cases (0/4) advance the ORIGINAL `src` pointer (`src += stride; x = *src` fuses to `lfdux`/`lwzux` in place, no `add`); a separate cursor with `*(p += stride)` only partially fuses (`add` + 5/7 `lfdux` + `lfdx` tail) and a function-top chase local coalesces with `src` (forces `add`+`lwzx`/`lhzux` instead of `mr`+update). The row-0 copy (`u8 *p = src`) must stay live across the advances (row-0/final load from `p`) to justify retail's `mr`. (2) Add-stepped cases (2/6, 1/5, 3/7) use a separate cursor (`p = src + stride`, plain `add`s, const-offset `dst` stores, `dst` never bumped). (3) Byte cases execute 7 `dcbt(src,stride)` touches, none before the final row. (4) Recombines are bare shift-or (`(w1>>24)|(w0<<8)`, `(w1>>8)|(w0<<24)`, `(w>>16)|(h0<<16)`) so MWCC emits `srwi`+`rlwimi` with no `andi`; `stride &= ~1u` reproduces the 2/6 `clrrwi`. (5) Jump-table relocs prove case grouping case0=doubles, case1/5=shift-1, case2/6=halfword, case3/7=shift-3, case4=words -- an earlier "1/5<->3/7 swapped bodies" hypothesis is disproven.
- Fix:       per-case cursor locals (never a function-top `q`); `u8 *p = src` + in-place `src += stride` + `*src` loads for update cases with the row-0/final load from `p`; separate-cursor + const-offset stores + 7 dcbts for byte cases; `stride &= ~1u` for the halfword case. Twin template: `MPVMC16_OneRef1p_TuneC` in `mpv_mcy.c` (q-chase, `step` masking, out-of-order stores) is the 16x16 sibling idiom source.
- Result:    near-miss us-803ac1b0 (3.2%, 263 structural / 39 reg_swap with 17 pure / size 0x4CC vs retail 0x4E0, TU OVER 52). Residual micro-gaps: case0 7th `lfdux` dead-update elided to `lfdx` + missing `mr` (entry color r7 vs r6); case4 first-row p-load hoisted above the advance; case2/6 advance fused to `lhzux` vs retail plain `add` (~5-instr shortfall: decomp case 0x118 vs retail 0x12C) + `slwi`-base vs `srwi`-base combine + mask position; byte-case load order (`-1` first vs `+3` first) + combine-base choice. Witness-gate SIZE (312 vs ~307) closes EQUIVALENT_MATCH; suspect Wii/1.1-vs-default MWCC version wall (whole Sofdec family is Wii/1.1 retail per 2026-08 sweep; `mpv_mc.c` builds at default version; configure.py untouchable in that task).
- Confidence: hypothesis (4 builds: looped 11.5% baseline -> unrolled kind-correct near-miss; scheduler/version micro-gaps untested) / negative_result (function-top `q`, compound `*(p += stride)`, 8-dcbt rows, masked-recombine `&` forms all ruled out)
- Applies to/a.k.a.: any `dcbtct`+`bctr` alignment-dispatch kernel with `lfdux`/`lwzux` chains and no per-case branches (Sofdec mpv family, Wii/1.1); update-vs-add cursor selection for `mr`-led vs `add`-led case heads; dead-update preservation limits; `mw_version` sweep requires the configure.py owner.

## nw4r MakeTexSrtMtx_R (maya) FPR color rotation unresponsive to numbering levers (Wii/1.1, near-miss us-803f4478)
- Symptom:   81.1% hexdiff, 0 structural, 7 pure reg_swap, size-exact 0x94, relocs identical; `cycle` witness-gate `abi-boundary | rho perm maps fpr f0 -> f4` closes EQUIVALENT_MATCH (FULL_MATCH only). The residual is a CYCLIC rotation of three absolute colors: retail (HALF-const f4, sinR-load f3, t-product f0, h-product f3-reuse, base f4-reuse) vs decomp (const f3, sinR-load f0, t f4, h f5-fresh, base f3-reuse); cosR-load f1, ZERO f2, m03/m13 reuses match on both sides.
- Cause:     unknown variable-set difference vs retail. The two post-call loads (const, sinR) NEVER move under any numbering lever, and the two product temps only bounce between f4/f5; everything downstream is consistent relative reuse. Since the absolute anchors are immovable, declaration/birth-order steering cannot reach retail's coloring from the current variable set.
- Fix:       none found. Ruled out (each built and measured, zero movement unless noted): decl-order perms (t,h,b)/(h,t,b)/(t,b,h)/(b,h,t) split and init forms, temps-before-sinRcosR, sinRcosR-before-r, explicit fidx temp (transparent), named `half` temp (transparent), store-interleaved assignment (transparent), dropping `r` local (WORSE: prologue f0/f1 swap -- prologue pins the `r` local), dropping `base` via CSE (WORSE: 4 structural, MWCC does not CSE). Sibling RT carries the identical chain bug; twin-TU 3dsmax R is 100% with a chain-free `0.5*(cos+sin-1)` formula.
- Result:    near-miss us-803f4478 (ACTIVE, stall noted). Reusable rules: (1) when two loads never move across all numbering perms, the variable SET differs -- stop permuting and look for a missing/extra named temp or a different call/prologue shape; (2) a cyclic 3-color rotation among (const-load, stack-load, first-product) with stable downstream reuse is the signature of this wall; (3) transparent-vs-live test: fidx/half/store-interleave no-ops prove those entities are numbering-invisible, narrowing the search.
- Confidence: negative_result (~13 builds)
- Applies to/a.k.a.: sibling ProductTexSrtMtx_RT/MakeTexSrtMtx_RT same-TU chain color walls; any 0-structural pure-reg_swap function where the absolute load colors resist all declaration-order steering.

## reslist deleting-dtor doubled dead-guard loop color wall (kyoshin, us-8005a798 family near-miss)
- Symptom:   reslist deleting dtor 87.0%, 0 structural, 6 pure r4/r5 reg_swap, size-exact 0xB8; retail loop reuses the dead start reg for the walker (lwz r4,0(r4); or r5,r4,r4; lwz r4,0(r4); stw r0,0(r5); lwz r5,4(r3); cmpl r4,r5; stw r5,0(r5)) while decomp keeps start live and walks in r5 (exact pairwise r4<->r5 swap). Single-guard sibling in the same TU is 100% with the opposite colors. Witness-gate rho-refusal in the loop region + bl calls closes EQUIVALENT_MATCH (FULL_MATCH only).
- Cause:     unknown web birth-order difference vs retail. The retail-only delta is the doubled dead null guard (one cmpi + two beqs, targets end/tail), but guard-value/position variants are color-neutral in our builds, and declaration/expression/type/path/qualifier levers (cur hoist/rename, self-advance, self-vs-base guards, cond order, clearList inline, next-temp, volatile, byte-path init, sentinel local, derived/base mix, delete spellings, void* item type, relink-via-node, per-function optimize_for_size) are all byte-neutral null-ops or worse -- the absolute load colors resist all numbering steering, the signature of a variable-set/ordering wall (cf. MakeTexSrtMtx_R entry).
- Fix:       none found. Decisive negatives worth reusing: (1) single guard deletes the 2nd beq (35 structural) -- the doubled guard must be written explicitly, MWCC emits no auto dtor guard on free functions; (2) cond operand order mechanically drives cmpl bytes (sentinel-first keeps cmpl, walker-first flips it); (3) relink-next via node (provably equal at exit) moves only the relink store, never the loop; (4) optimize_for_size gives an stmw prologue -- retail is default-mode. Keep the skill bans: no raw-offset casts, fake volatility, or dead ops to steer registers.
- Result:    near-miss us-8005a798 (ACTIVE, stall noted, source left at baseline). Family-wide: __dt__reslist_IUIBattle (CUIBattleManager) and __dt__reslist_IUICf (CUICfManager) carry the identical 87.0%/6 wall.
- Confidence: negative_result (~16 hexdiff-measured probes this session + 1 cycle rho-refusal)
- Applies to/a.k.a.: any doubled-dead-guard reslist/__dt__ loop with pure r4/r5 walker-vs-sentinel swap and a 100% opposite-color single-guard sibling; coordinate across twin owners before repeating probes.

## nw4r MakeTexSrtMtx_SR (3dsmax) decl-colors / assign-loads decoupling (Wii/1.1, FULL_MATCH us-803f5dec)
- Symptom:   71.1% hexdiff, 0 structural, 13 reg_swap (11 pure), size-exact 0xB4; `cycle` witness-gate `fields | slot 13: non-register bits differ` closes EQUIVALENT_MATCH (FULL_MATCH only). Residual is FPR coloring: retail (svs=f9,sus=f7,suc=f6,svc=f8; loads sin=f1,Sv=f0,Su=f3,cos=f2) vs decomp (svs=f6,sus=f7,suc=f8,svc=f9; loads Sv,sin,Su,cos with Sv=f3,Su=f2,cos=f0). The `.4byte` diff is just `fneg` on a differently-colored temp, not a real mnemonic wall.
- Cause:     MWCC numbers copy-temps sequentially by DECLARATION order but schedules their loads by ASSIGNMENT (first-use) order. Combined decl+assign (`f32 svs = sv*sinR; ...`) couples the two, so no single statement order satisfies both retail's colors and retail's load order; retail's sin-first load order with `fmuls ra=Sv` additionally implies retail references `sinR` through an early alias temp.
- Fix:       split declaration from assignment and order each independently, plus `sinR`/`cosR` aliases: `f32 sv, s, c, su; f32 suc, sus, svc, svs;` then assigns `s=sinR; sv=Sv; su=Su; c=cosR;` (loads sin,Sv,Su,cos with colors Sv=f0,s=f1,c=f2,su=f3) then products `svs=sv*s; sus=su*s; suc=su*c; svc=sv*c;` (emission Sv*sin,Su*sin,Su*cos,Sv*cos, colors f9,f7,f6,f8). Path: combined (13 RS) -> product-decl split `suc,sus,svc,svs` (8 RS, products match) -> sv/su decl swap (6 RS, su+sin match) -> alias bundle (regresses: 6 structural, extra live temps perturb store scheduling -- revert) -> decoupled decl/assign + aliases (FULL_MATCH). Same mantra as the maya sibling's split decl/assign and this file's `ProductTexSrtMtx_SRT` (`f32 s = sinR; ... f32 c = cosR;`).
- Result:    FULL_MATCH us-803f5dec (hexdiff 100.0%, 0 structural / 0 reg_swap / size 0xB4 exact; cycle semantic-certified). Unit-level size FAIL (OVER 104) is other functions' bloat, not this target.
- Confidence: repo_proven (single-target FULL_MATCH; stepwise RS reduction 13->8->6->0)
- Applies to/a.k.a.: any 0-structural pure-FPR-reg_swap function where load ORDER and load COLORS cannot be satisfied simultaneously by statement reordering (decouple the two: decl order steers colors, assign order steers loads); sin/cos-alias temps (`s`/`c`) when retail loads a stack out-param before its first arithmetic use.

## RVL_SDK hcicmds per_inq_mode GPR color rotation unresponsive to naming levers (GC/3.0a5.2 -O4, near-miss us-802f3914)
- Symptom:   58.3% hexdiff, 0 structural, 20 pure reg_swap, size-exact 0xC0, relocs identical; `cycle` witness-gate `abi-boundary | rho perm maps gpr r0 -> r4` closes EQUIVALENT_MATCH (FULL_MATCH only). Cyclic rotation over the same 6-volatile set {r0,r4,r5,r6,r7,r8}: retail {12:r0, 0:r5, 3:r4, 4:r0-reuse, 9:r8, maxhi:r7-fresh, minhi:r6-fresh, b2:r5-reuse, b1:r4-reuse, b0:r0-reuse} vs decomp {12:r4, 0:r0, 3:r7, 4:r6-fresh, 9:r5-fresh, maxhi:r4-reuse, minhi:r0-reuse, b2:r8-fresh, b1:r7-reuse, b0:r6-reuse}. Schedule (incl. hoisted li/srawi/rlwinm block and interleaved lbz/stb bd loads) is byte-exact.
- Cause:     unknown variable-set difference vs retail. All naming/cast/qualifier levers coalesce to identical output, so the interference graph never changes; only a genuinely different live-range shape could move colors.
- Fix:       none found. Ruled out (each built, zero color movement unless noted): b decl-order perm, BT_HDR struct stores + pp=(p+1) indexed form (pp folds, identical), named shift temps max_hi/min_hi (coalesced), `const unsigned char *bd_addr` (inquiry-style, no-op), first-use rename b0<-bd[2] (no-op), pp++ stream-chain (WORSE: 6 structural -- mutable may-alias pins bd loads after shift stores, sinking lbz late; the interleaved b-temp cache is the only skeleton giving retail order). Prior art also ruled out: -ipa off, GC/3.0a3.4 vs a5.2, store orderings, byte-pair len stores, param signedness (s16 max_delay srawi / u16 min_delay rlwinm confirmed). Sibling btsnd_hcic_inquiry (fewer temps, direct bd reads) is 100%.
- Result:    near-miss us-802f3914 (ACTIVE, stall noted, source left at indexed+interleave baseline).
- Confidence: negative_result (6 builds this session + prior-art batch)
- Applies to/a.k.a.: any 0-structural pure-GPR-reg_swap builder where li/shift/lbz colors resist all rename/cast/qualifier steering (GPR analog of the MakeTexSrtMtx_R wall above); test a lever by whether ANY color moves -- full immobility means the graph is unchanged (coalescing), change live ranges not names.

## monolib LightCtlSetDir VEC3Scale work-reg f0/f1 wall (Wii/1.1, near-miss us-804c45e0)
- Symptom:   89.8% hexdiff, 0 structural, 6 reg_swap, size-exact 0xEC, relocs identical; residual is ONLY the two `nw4r::math::VEC3Scale` inline-ASM work regs swapped (retail `psq_l f0`/`ps_muls0 f1,f0,f2` vs decomp `psq_l f1`/`ps_muls0 f0,f1,f2`). `cycle` witness-gate `reject-list | slot 20: non-byte-identical PSQ` closes EQUIVALENT_MATCH (PS_MULS0/PSQ reg fields are categorically uncertifiable; SMT forbidden) -- FULL_MATCH only.
- Cause:     unknown allocator preference vs retail. Factor-in-f2, stack slots (scale-tmp@sp+20, pos@sp+8), lfs/stfs trio, lwz/stw struct copy, switch chain, and all three LightObj calls are byte-exact, so the interference graph matches except for the symmetric work0/work1 choice, which is immobile across all tested context perturbations (signature of a numbering/ordering wall, cf. MakeTexSrtMtx_R entry).
- Fix:       partial (0% stub -> 89.8%). Load-bearing decisions: (1) `extern "C"` + `(CLight*, const ml::CVec3*)` signature (stub was mangled `__FPUci`, retail-size 0xEC vs decomp 0x08); (2) word-copy top (`u32* dst` / `const u32* s`, LightCtlSetPos idiom); (3) NAMED `const f32 dist` local pins the scale-factor load to f2 (literal/unnamed use gives f0); (4) direct `VEC3Scale` into a first-declared tmp (operator*/static-wrapper add temps and flip slots); (5) memberwise `pos.x/y/z = tmp.x/y/z` fill (set()/ctor give y,z,x load order, memberwise gives retail x,y,z). Ruled out (zero work-reg movement): call-path perms, tmp CVec3-vs-VEC3 type, const/non-const vec cast, named VEC3 ref (folds cleanly, no-op), dist init-vs-split and decl-order perms, inverted memberwise (WORSE 78%), static-scale wrapper (WORSE 40%, extra hop), op*-into-tmp (WORSE 30%, 3 temps).
- Result:    near-miss us-804c45e0 (ACTIVE, stall noted, source left at best shape; siblings LightCtlSetPos/SetAttnMode/CopyObj still 100%). Next angles: case-label textual reorder (tests MWCC label sorting vs CFG-allocation coupling), if/else-if chain, header archaeology on VEC3Scale work-reg decl order vs accepted repo users.
- Confidence: negative_result (~12 builds) with reusable positive idiom (named-dist + direct-VEC3Scale + memberwise-pos).
- Applies to/a.k.a.: LightCtlSetDirFromAngle case-1 (identical retail scale-block shape, same TU); any inlined paired-single `register work0/work1` template whose symmetric allocation resists context steering.

## nw4r MakeTexSrtMtx_SRT (3dsmax) m13sum-dest f0/f1 reuse-choice wall (GC/3.0a5.2, near-miss us-803f5fc4)
- Symptom:   90.2% hexdiff, 0 structural, 5 reg_swap, size-exact 0xCC, relocs identical; residual is ONE 5-word cluster (m13sum@90 f0/f1, fneg2@98 f1/f0, m13@a0 f0/f1, m10@a4 f1/f0, m13st@ac f0/f1 -- a consistent f0<->f1 swap of the m13sum/m13/m13st/fneg2/m10 chain). `cycle` witness-gate `rho | no consistent bijection in region [36, 39)` closes EQUIVALENT_MATCH (the swap breaks rho: f0 maps to both f0 (Tv/svc-t2 uses) and f1 (m13sum-dest)) -- FULL_MATCH only. Started at 23.0% STRUCTURAL (recomputing subproducts, 0xF4 size).
- Cause:     MWCC reuses the FIRST-listed sum operand's reg for the m13 sum (f1 = svs*t1-result); retail reuses the SECOND (f0 = svc*t2-result). The choice is decl-invariant (7-way decl reorder/position sweep: identical cluster) and extra-temp-invariant (zero/half/m13s/m13a/m13b temps shift global colors but keep m13sum->f1). Flipping the sum order (svc-term-first) breaks t1@60-before-m00@64 timing in every context tried (temp/inline/m03a-early/m13a-named), and t1-timing requires svs-term-first. Mutually exclusive levers = soft cap.
- Fix:       partial (23% STRUCTURAL -> 90.2% CODE_MATCH, 46/51 exact words). Load-bearing decisions, each probe-verified: (1) named product temps suc/sus/svc/svs + DIRECT struct reads (no su/sv/tu/tv/c/s copies -- any copy-temp flips front loads to Su,cos,Sv,sin; direct gives retail Su,cos,sin,Sv + su-pair muls); (2) products strictly suc,sus,svc,svs (all 5 other suc-first perms break first-mul or front); (3) t1 = half+Tu (NOT Tu+half -- commuted form flips front to cos-pair); t2 = Tv-half; t1-stmt after products, t2-stmt after t1-stmt (else front breaks); (4) named m03a=-suc*t1/m03b=sus*t2 temps pin translation-mul/drain schedule (without them: zero/m00 swap + m03-pair inversion + store-interleave diffs); (5) m10-store LAST (else fneg2 displaces sus*t2); (6) t1/t2 INLINE with CSE-sharing (named t1/t2 take fresh f9/f10; inline reuses dead f3/f2 -- matches retail t1->f3, t2->f2, zero->f6, half->f4, suc->f5); (7) m02/m12 use CC0 direct (zero-temp shifts sin/half colors). Ruled out (~80 /tmp probes w/ exact TU flags + 9 repo builds): t1/t2-statement positions (14-way sweep invariant), simple-store orders (m10-first/m00-last/retail-order/zero-first: no schedule effect), m03-term/def/sum swaps (each fixes one axis, breaks another: m03-first+F3-terms=Tu-late; m03-swapped=m03-pair-inverted; m03b-def-first=front-reshuffle), associations (half-first normalized to sum-first, no-op), mul-operand commutation (would break words), m13a/m13b named (same cluster, worse colors), m13s named sum (same cluster), m03-sum swap (breaks matching m03sum word), SinCos/prologue/call variations. Scheduler model learned: output = f(value-DAG) with textual tie-breaks (A/B/C/D identical programs -> identical output; E/E2/E3 same-DAG-different-order -> different output); store/mul TEXTUAL order steers load timing backward (m13-before-m03 stmt order <=> Tu-before-Tv loads); t2-stmt before t1-stmt catastrophically hoists Tv/zero loads to front. XO correction for hand-decoding: fmadds=29 (not 21), fadds=21, fsubs=20, fmuls=25.
- Result:    near-miss us-803f5fc4 (ACTIVE, 90.2% CODE_MATCH, stall noted, source left at X2-shape). Next angles: (a) m13 = (svc*t2+svs*t1)+half WITH a compensating t1-early pin holding t1@60 (m03a-def repositioning alone insufficient per X9); (b) a numbering SET (not order) flipping sum-dest without extra-temp color damage; (c) TU-budget size FAIL (0x40 over) is sibling functions' bloat, not this target (0xCC exact).
- Confidence: negative_result (~80 probes + 9 builds) with reusable positive idiom (direct-reads + su-pair products + inline-CSE t1/t2 + named m03a/m03b + m10-last).
- Applies to/a.k.a.: any 0-structural pure-FPR-reg_swap function with a sum-dest reuse-choice cluster (f0<->f1 swap + consistent followers, rho-refusal region); the MakeTexSrtMtx_R/SR entries in this file (same TU/family); use /tmp probes with the exact TU compile command for lock-free schedule iteration, but verify colors in-repo (probe colors transfer for fixed schedules -- validated here).

## Unit-wide pool_patterns vs single-site divergent pool copy (Wii/1.1, near-miss us-804e1a6c CEBuildSinCos)
- Symptom:   hexdiff 100.0% (0 structural / 0 reg_swap / size 0x148 exact / split PASS) yet cycle FAILs: witness-gate `reloc | slot 8: lbl_eu_8066B280 vs lbl_eu_8066B268`. Retail loads the 0x4330000080000000 int->float magic from sdata2 slot B280 at exactly one site (+0x0020); the decomp pools the same bytes once TU-wide as anon @2564, which the frozen unit-wide `pool_patterns` rule (and the mined reloc map, 32x) renames to B268 -- correct for the 31 sibling sites, wrong for this one.
- Cause:     MWCC's builtin `(f32)(s32)` lowering hardcodes the single-rounded `fsubs`-against-magic idiom with a compiler-pooled literal; no source spelling names that literal (manual union-subtract gives `fsub`+`frsp`, burned in CGXCache/CActorParam; repo rule-of-thumb retail-`fsubs`=>builtin-cast is already in the source). The only lever is the object-level pool rename, but UNIT_RULES is shrink-frozen and unit-scoped, so one rule cannot express B268-almost-everywhere + B280-once. Contributing trap: the rule's census comment cited a function sub-range (func_804DB980..func_804DD5B0) that excluded this site, so re-mining keeps re-learning B268-only.
- Fix:       none in source (body left byte-perfect, no edit). Census the FULL retail `.rela.text` unit-wide (not a sub-range) before pinning a content rule -- here `lbl_eu_8066B268 x31 sites` vs `lbl_eu_8066B280 x1 site` would have been visible. Acceptance needs a tooling-level per-function pool override for the divergent site, or a witness upgrade certifying value-equal same-section reloc pairs (both are .sdata2 doubles with identical bytes).
- Result:    near-miss us-804e1a6c (ACTIVE CODE_MATCH, stall noted, source untouched).
- Confidence: repo_proven (diagnosis: hexdiff json + retail/decomp symtab + rela census; negative source results cited from CGXCache s32ToF/CActorParam UnkVirtualFunc11/CItemBoxInfo entries in MWCC_CASES.md)
- Applies to/a.k.a.: any TU whose conversion-magic/pool bytes exist at 2+ retail slots (B268/B280, A388/B298 families) with a unit-wide pool_patterns rule; check `reloc_map.py diff -s <sym>` per function, not just the unit, when the map disagrees with one retail site. Example: us-804e1a6c.

## nw4r XSI MakeTexSrtMtx_SRT FPR base-color +1 wall (Wii/1.1, near-miss us-803f53b0)
- Symptom:   46% hexdiff, 2 structural, 25 pure reg_swap, size-exact 0xC8; `cycle` witness-gate `fields` closes EQUIVALENT_MATCH (FULL_MATCH only). Decomp base products f6-f9 vs retail f7-f10 (uniform +1); copies fully recolored but 1.0 stays f0 both sides; loads cos,Sv,sin,Su vs retail Sv,cos swap is the only load diff.
- Cause:     unknown live-range-structure difference vs retail. Pure reg-reg copies (c/s trig locals, Product_SRT house style) coalesce and are invisible to FPR numbering; only loads/computations consume vregs, and counts match, so the +1 is interference-graph shape, not count.
- Fix:       partial (COMPILES baseline -> best clean-schedule shape, init-form su-first products). Load-bearing: four locals su/sv/tu/tv loaded once early (inline Sv/Su/Tu/Tv reads sink loads and scramble schedule); copies must stay init-form in su,sv,tu,tv order (split-copy decls or su-last assignment sink the Su load late: 17 structural); MWCC births sv-pair first even from su-first declaration (as in matched SR). Ruled out (each built): c/s copies alone (neutral 46/2/25) and c/s+split (WORSE 44/16 structural + reloc drift); product declare/assign split (safe, moves WHO gets f6, base stays f6); named early nsv_cos (pins fneg, WORSE 9 structural); SR-mirror inline-Sv (WORSE 17 structural); copy order sv,tu,tv,su init or split (WORSE 17 structural); su-first vs sv-first product init order (identical colors).
- Result:    near-miss us-803f53b0 (ACTIVE, stall noted, source left at su-first init-form baseline).
- Confidence: negative_result (8 builds this session)
- Applies to/a.k.a.: any FPR wall where base-product colors sit uniformly +1/-1 across all decl/assign permutations while pure-copy temps coalesce invisibly -- change overlapping live ranges (loads/computations), not names; next angle: tu/tv assigned between sv-pair and su-pair statements, or tu taking fresh (not Sv-reuse) home implying Sv live longer in retail.

## kyoshin initMCItemSub prologue hoist-vs-copy scheduling wall (Wii/1.1 -O4,s, near-miss us-8029917c)
- Symptom:   92.6% hexdiff (25/27), 2 structural, 0 reg_swap, size-exact 0x6C; residual is ONLY prologue order: retail `addi r0,r3,256` (hoisted loop bound) before `or r5,r3,r3` (param copy), decomp emits the copy first with otherwise identical colors (ptr r5 / bound r0 / store-val r4). `cycle` witness-gate `mnemonic | slot 0: addi vs or` closes EQUIVALENT_MATCH (FULL_MATCH only).
- Cause:     MWCC list-scheduler emits the param copy before the CSE-hoisted bound whenever both derive from the same base register, regardless of source order; the bound must stay an inline expression (a named `end` local allocates to a non-volatile r5 instead of retail's CSE-temp r0), and every inline-bound shape keeps copy-first.
- Fix:       none found. Ruled out (each built): split declare/assign with end-first assignment (WORSE 77.8%, loses r0 coloring); `&x->table[0]`/`&x->table[0x80]` subscript (no-op); `(s16*)x` casts (no-op); top-level `* const` param (no-op); flipped `bound > p` comparison (MWCC normalizes to identical cmpl/blt, no-op); for-loop with separate `p++` increment (WORSE 0%, adds while-guard prebranch +0x14 size). Prior art ruled out decl order, dependent derivation, alias, comma-seq, while-guard.
- Result:    near-miss us-8029917c (ACTIVE HIGH_MATCH, stall noted, source left at inline-bound do-while baseline).
- Confidence: negative_result (6 builds this session + prior-art batch)
- Applies to/a.k.a.: any 0-reg_swap function whose only residual is hoisted-addi vs param-copy prologue order with exact colors; next angle is a single-expression bound-first IR trick without naming the bound (cf. the array-push postfix fix in kyoshin leaf-function patterns) -- test whether ANY shape moves the order, else treat as scheduler-fixed.

## kyoshin isEIBCatMatch mulli-dest + saved-home rotation wall (Wii/1.1 -O4,s, near-miss us-8028adcc)
- Symptom:   70.0% hexdiff (21/30), 0 structural, 9 reg_swap, size-exact 0x78/0x78; residual is a global rotation: retail `mulli r4,r4,5` / `lbz r5` / `lhz r0` / cat in r30 / result in r31 vs decomp `mulli r0,r4,5` / `lbz r4` / `lhz r5` / cat in r31 / result in r30. Single shared `lbz r4,501(r3)` feeds both the `(s8)==-1` early-out (`extsb r0,r4; cmpwi`) and the `mulli`. `cycle` witness-gate `rho | no consistent bijection in region [10,16)` closes EQUIVALENT_MATCH (FULL_MATCH only); the in-place retail mask `rlwinm r4,r4` vs decomp copy `rlwinm r4,r0` breaks every bijection, so no consistent-rename shape exists short of byte-identity.
- Cause:     MWCC allocates the check-reused load-temp to a fresh scratch (r0) while retail destructively reuses the check register (r4); the saved-home swap (r30<->r31) follows from the same coloring pass. Insensitive to every surface lever (each built, byte-identical unless noted): declaration-order perms (use-order, cat/a swap), expression flip `unk_1f5*5+unk_1f4`, u8 vs int idx, u32 vs u8 cat, grid local `&self->_pad37D[1]` vs inline `(u8*)self+0x37E` (addi never hoists either way), inline idx into the call (load order `mulli,lbz,lhz` preserved -- scheduler is need-driven -- colors unchanged), extra named `f5` temp (transparent), early-homed `int idx` (WORSE: homes r0, 11 swaps), `const`+fused initializers, inner `(u8)` cast (folded), `#pragma optimize_for_size` removal (byte-inert). u32 a/b REQUIRED (u8 breaks the tail structurally); `return b == a` REQUIRED (matches retail `subf` a-minus-b direction; `a == b` flips it). Family proof: eibRowMatches FULL_MATCH uses the same idiom with grid hoisted (different context, r0-pattern on both sides); findEIBEquipSlot FULL_MATCH uses flipped+inner-cast spelling (also r0-pattern).
- Fix:       subf direction via `return b == a` (real fix, kept). mulli-dest rotation: none found -- stall noted, source left at family-idiom initializer baseline.
- Result:    near-miss us-8028adcc (ACTIVE CODE_MATCH 98.0%, stall noted).
- Confidence: negative_result (10 builds this session + prior-art batch of 2)
- Applies to/a.k.a.: any check-then-reuse `lbz`/`extsb`/`cmpwi` + `mulli rX,rX` cluster where decomp picks a fresh scratch; next angle is the unknown pass-level numbering lever for mulli dest (cf. clpGetRow* FULL_MATCH where 2-address mulli follows an extsb-reg producer, never a bare load), else SMT/out-of-band acceptance.

## kyoshin CBattleState_addStagingEntry prologue copy/schedule wall (Wii/1.1, near-miss us-80148210)
- Symptom:   85 insns, 44 mismatch (39 pure reg_swap + 5 structural), size-exact 0x154/0x154; `cycle` witness-gate `fields` closes EQUIVALENT_MATCH (FULL_MATCH only). Retail prologue `lhz; li one; addi entries; li trip; rlwinm; rlwinm; add ptr; lwz; slw; or-copy p; or; stw; mtspr` vs decomp `lhz; addi entries; li one; li trip; rlwinm; rlwinm; add ptr; or-copy p; lwz; slw; or; stw; mtspr` (copy p=entries before lwz, addi before li). Tail FPR color swap: retail `fmuls f0,f0,f1` + `lfs f1,40(r4)`/`stfs f1,40` vs decomp `fmuls f1,f0,f1` + `lfs f0,40(r4)`/`stfs f0,40` (named `scaled` temp f1 vs f0, anon unk28 temp the other).
- Cause:     MWCC list-scheduler + Chaitin coloring inert to source order here: decomp output byte-identical across declaration permutations, assignment permutations (one-before-entries to match retail li-before-addi), float-temp hoist (`f32 scaled` top vs mid), and variable-set change (named `u8* wordPtr` sibling idiom + split bitfield store). Early p-copy forces p/one live-range overlap (fresh r9 vs retail reuse of one's r7), cascading the global GPR rotation (entries r5<->r8, id r6<->r0, one r7<->r5, trip r0<->r10).
- Fix:       none found. Ruled out (each built, all byte-identical no-ops): (A) decl entries,p,n,one,id -> id,one,entries,p,n + f32 scaled to top + assign one-before-entries; (B) decl one-before-entries alone; (C) named wordPtr temp. Positive control (`one = 2` -> `li r5,2` observed) proves the pipeline reflects edits. Source reverted to baseline.
- Result:    near-miss us-80148210 (ACTIVE HIGH_MATCH 92.5%, stall noted, source untouched at baseline).
- Confidence: negative_result (5 builds this session: 3 shape probes + positive control + revert-verify)
- Applies to/a.k.a.: any function whose only structural residual is li-vs-addi prologue order + param-copy placement with an otherwise global reg rotation; next angles are for-init birth (`for (p = entries, n = 8; ...)`), init-at-declaration, or a load-bearing extra live range that forces p after slw -- cf. initMCItemSub hoist-vs-copy wall above.

## kyoshin CfResReloadImpl isReloadTypeOk — missing flat symbol + `>=` booleanizes to subi/orc value-first idiom (Wii/1.1, FULL_MATCH us-8016e198)
- Symptom:   target absent from decomp object (`hexdiff --symbol` ERROR, registry stale at CODE_MATCH 88.6%); decomp held an invented dead helper `checkTypeIsValid` (mangled C++ name, no retail counterpart, zero callers) carrying the same `(v|0xFFFFFFFC)-((v-3)>>1)` arithmetic. Writing the arithmetic as extern "C" converges to `li r0,-4; subi r3; or r0; rlwinm r3; subf r0,r3,r0` (37.5%, 1 structural + 4 reg_swap) vs retail `li r3,3; subi r0; orc r3,r4,r3; rlwinm r0; subf r0,r0,r3`; `cycle` witness-gate `fields | slot 1: non-register bits differ` closes EQUIVALENT_MATCH (FULL_MATCH only).
- Cause:     twofold. (1) The decomp targeted the wrong symbol: retail's flat `isReloadTypeOk` (vslot 0x14 body) was never defined; the invented helper occupied its place. (2) The arithmetic spelling can never select `orc`: the folded constant 0xFFFFFFFC fits a simm (`li r0,-4` + `or`), while retail's `li r3,3` + `orc` is MWCC's *comparison* booleanization — the value-first `x >= c` form of the WaveArchiveReader `subfic/orc/subf/srwi` rule (`orc(v|~c)` with the bound kept live in the return register).
- Fix:       delete the invented helper, define `extern "C" int isReloadTypeOk(cf::CfResReloadImpl* self) { return self->field_08 >= 3; }` (u16 field promotes; `>=` gives value-first, `<=`/swapped operands would give constant-first `subfic` form). 8/8 byte-identical, 0x20/0x20, FULL_MATCH. The same arithmetic inlined in a sibling tail (isReloadInUse) is a different context — do NOT "fix" the sibling to `>=`; its residual is a genuine inlining-shape item.
- Result:    FULL_MATCH us-8016e198 (100.0%, split PASS).
- Confidence: repo_proven (this target + WaveArchiveReader us-8042bbb0 precedent)
- Applies to/a.k.a.: any `subi; li; orc; srwi; subf; srwi(31)` cluster — write `x >= c`, never the equivalent arithmetic; absent-symbol targets where a same-semantics invented helper exists in the decomp (check decomp symbol list vs retail `--list` before shaping expressions).

## RVL_SDK GXTexture GXGetTexObjLODAll — SDA FP-const hoist + fmuls fixed point (Wii/1.1 default, near-miss us-8031ebf0)
- Symptom:   69.1% (code 86.4%, data 100%, size-exact 0xdc/0xdc); 12 structural + 5 reg_swap over 55 insns with a 100%-identical instruction multiset. Decomp hoists the three SDA FP-const loads (lfd 2^52 magic, lfs 0.0625, lfd signed-magic) to the prologue top; retail interleaves them with the mode1/mode0 lwz + byte extracts (extsh/extsb/xoris conversion chain positions match otherwise). 3x fmuls emit const-in-frA (decomp `fmuls fD,f3,f1`) vs retail var-in-frA (`fmuls fD,f1,f3`); `cycle` witness-gate refuses with `mnemonic | slot 2: lwz vs lfd` (witness-closed, FULL_MATCH only).
- Cause:     MWCC list-scheduler priority wall under frozen unit flags (31 FULL_MATCH siblings): dependency-free SDA const loads are hoisted above the integer chain regardless of source position, and fmuls operand colors are inert to source order (matches the ocUnit `fmuls f1,f1,f0` vs `fmuls f1,f0,f1` fixed-point record in MWCC_CASES).
- Fix:       none found. Ruled out (each built): (A) scoped `#pragma push` / `#pragma scheduling off` / `#pragma pop` => alien 1.8% codegen (48 structural, 0xcc size, args reloaded from stack), reverted — scheduling-off is NOT a lever for this TU/family; (B) const-first float mults (`float_8066C038 * (f32)(u32)x`, the TU sibling idiom from GXInitTexObjLOD) => byte-identical no-op; (C) mutable `u8* filtConv` may-alias pinning => byte-identical no-op. Prior sessions ruled out 5+ shapes (inline/direct-access/hoisted extracts/value-locals/hoisted lodBiasRaw) and retail-extract reorder (regressed to 45%). Source reverted to canonical KB shape (var-first mults, `const u8*` table).
- Result:    near-miss us-8031ebf0 (ACTIVE HIGH_MATCH 69.1%, stall noted, source untouched at baseline; TU split PASS 0xC spare, 30/32 still 100%).
- Confidence: negative_result (4 builds this session + prior-art batch of 6+ shapes + witness-gate probe)
- Applies to/a.k.a.: any int->float-scale function whose only structural residual is SDA const-load hoisting with identical multiset; next angles are exact-original SDK statement order (unknown IR-delaying spelling), an unknown per-function load-hoist pragma (`scheduling off` proven alien here), else FULL_MATCH when (a) is found — cf. CMenuQstCnt `#pragma scheduling off` (works there, fails here).

## kyoshin unrolled null-walk: alternating-cursor flat gotos, not nested if/else (Wii/1.1 -O4,s, FULL_MATCH us-80261260)
- Symptom:   nested-if/else 10-deep prev-chain walk sat at 56.6% hexdiff (+16 bytes, 4 extra lone `b` islands after the deeper or-pads); identical doubling reproduced in .scratch probes across Wii/1.0, 1.1, 1.3 and GC/3.0a5.2, and persisted through loop-context, do-while(0)+break, and call-vs-assign innermost variants. Inverted (`== 0`/else-deeper), standalone-if pairs, and sequential-break shapes all flipped branch senses to `bc 4,2`, moved loads out of line, or added dup tests / double moves.
- Cause:     MWCC emits a join `b` per nesting level even when the level's else is an empty (coalesced no-op) assignment, so fully-nested unrolled walks always carry dead join branches; retail instead alternates the cursor between two variables with flat forward gotos (same idiom as IdTable_SumValues/IdTable_MaxValueC in code_8025FB10.cpp): odd levels load the scratch `t` (null falls straight to the shared tail, zero overhead) and even levels load `head` (null needs its own `head = t` copy block: single `or` + single `b`). The first level's null case is its own `head = pos` copy block (falls through to the tail, no `b`).
- Fix:       `E* head; E* t = pos->pField1C; if (t == 0) goto C0; head = t->pField1C; if (head == 0) goto C2; t = head->pField1C; if (t == 0) goto done; ... head = findHead(head); goto done;` with labels innermost-first (`C10..C2: head = t; goto done;` then `C0: head = pos;` falling into `done:` + shared stores). Uninitialized `head` builds warning-clean (same as the `result` precedent).
- Result:    FULL_MATCH us-80261260 (100.0%, 0 structural, 0 reg_swap, size-exact 0x17c/0x17c).
- Confidence: repo_proven (this target + IdTable_SumValues/MaxValueC family precedent)
- Applies to/a.k.a.: any unrolled multi-level null-check walk whose residual is extra lone-`b` islands after or-pads with otherwise exact loads/senses; `goto done` to a shared tail is the established family spelling (code_8025FB10.cpp), log `policy_exception: true` per PLAN §17.6 goto-gate rule.

## In-compare assignment operands: load emission + cmpl order + scratch colors (Wii/1.1, FULL_MATCH us-80435c98)
- Symptom:   pure reg_swap on two ABI scratch loads (e.g. SDA-global vs member, r4/r5) where declaration/assignment permutations never move the colors, !=-operand swaps canonicalize to identical output, and hoisting either load to a named init flips the emission order but keeps the wrong colors. Witness-gate `abi-boundary | rho perm maps gpr rX -> rY` closes EQUIVALENT_MATCH: FULL_MATCH only.
- Cause:     three coupled MWCC behaviors: (1) among straight-line plain inits the this-based load always emits first, so a member-copy init schedules before an SDA init regardless of init order; (2) a member load inside the branchy compare sinks below an SDA-direct init (emission SDA-first); (3) cmpl lists the impure (call/assignment) operand LAST (pure LHS first, both-pure in source order), so a lone assignment on the LHS flips the cmpl operand order vs retail.
- Fix:       split declare (colors) from assign (loads): declare both temps WITHOUT initializers, then assign BOTH inside the != operands — `if ((handle = mHandle) != (hMEM1 = LBL_SDA) && ...)`. Both loads stay branchy (SDA keeps first-emission slot and r4), the extra handle temp fixes the allocator graph (member r5), and the both-impure tie lists source order (member, SDA) like retail's cmpl. A lone LHS assignment flips the cmpl; a lone member init flips the emission; only both-assign satisfies all three.
- Applies to/a.k.a.: any two-load compare (SDA/global vs member/param) with an abi-boundary witness refusal and a both-pure residual that will not move; direct-SDA-slot read (same reloc the getter inlines to) plus in-compare assignment-exprs; no asm/register/policy_exception needed.
- Confidence: repo_proven
- Example:   us-80435c98 (`__dt__Q33mtl10MemManager9MemRegionFv`, monolib/src/util/MemManager, 98.0%/5-swap -> 100.0% FULL_MATCH semantic-certified)

## kyoshin CSuddenCommu sinit 42-word table-copy live-range cliff (Wii/1.1, stalled us-801be164)
- Symptom:   sinit_801BC86C 0.0% (84 structural + 38 reg_swap, 0x1cc/0x1e8, frame 144 vs 96). Retail: clean straight-line 42-word copy (14x12B PMF entries, vtable+0x30 -> .bss): src full lis+addi in r4, dst HA in r3 with first store `stwu r16,dst@l(r3)`, 9 src entry-address windows (entries 0-8, recomputed for 9, on-demand for 10-13), two waves (entries 0-9 with 4 stack spills, then 10-13), ~26 values in saved regs. `cycle` witness-gate `size` (115 vs 122 insns) closes EQUIVALENT_MATCH: FULL_MATCH only.
- Cause:     MWCC live-range cliff at ~27 simultaneously-live words: with 42 hoisted loads the allocator abandons multi-reg coloring and stages everything through the stack with a single temp (frame 144), regardless of source shape. The retail schedule (graceful 26-reg + 4-spill, entry-address addis feeding exactly 2 loads each, hard 30/12 wave split with no barrier instruction) is not reproducible: MWCC folds ALL constant entry addresses unconditionally (struct/member/u32/PMF/nested/u64 spellings, explicit `&src[i]` temps, pointer chains, `t+K` row pointers, named value temps — all fold), and no high-level construct splits the scheduling region (14x copies in load/store/reverse orders, 10+4 and struct+u32 type splits, labels, same-symbol may-alias pointers all hoist identically; volatile pins order but still degenerates past 26 values). Verified across MWCC Wii/1.0/1.0RC1/1.0a/1.1/1.3/1.5/1.6/1.7 (all fold; -O0/-O4,s emit windows but alien shapes) and per-function pragmas (peephole/opt-size/scheduling/cse) via a standalone micro-harness with identical flags. Clean multi-reg only at <=8-9 copies (24-27 words).
- Fix:       none found. Parked at the honest 14x PMF-copy spelling (PMF-typed locals, entry order). Positive controls: 4-copy probe gives the exact retail idiom (stwu trick, folded offsets, multireg, no spill); volatile-src probe pins 26 ordered multireg loads then degenerates, bounding the cliff.
- Result:    stalled us-801be164 (ACTIVE COMPILES 0.0%, stall noted, source at honest baseline; TU split PASS).
- Confidence: negative_result (~40 micro builds + 8 TU builds: 2 shape probes + const/loop/member/global/10+4/twotype/PMF variants + positive controls + revert-verify)
- Applies to/a.k.a.: any straight-line bulk copy over ~27 words whose retail shows entry-address windows + graceful spills; next angles are an unknown IR shape that materializes entry addresses (runtime index with no visible loads is impossible in a void leaf, so the original may have used a construct outside the tested space) — cf. CBattleState_addStagingEntry scheduling wall above.

## Post-call anonymous-global tail: volatile rematerialization + mirror-image sibling residuals (GC/3.0a5.2 -ipa off, stalled us-802eb330)
- Symptom:   97.0% hexdiff, 0 structural / 2 pure reg_swap, size-exact 0x108/0x108: a function with one early `&btm_cb` base (named `p_inq`, r30) and one late `&btm_cb` base (anonymous, r31, born just before a `p_bd_db` load) matches everywhere except the final two `sth` stores, which decomp (spelled `p_inq->`) binds to r30 while retail binds to r31. `cycle` witness-gate `abi-boundary | rho perm maps gpr r30 -> r31` closes EQUIVALENT_MATCH (callee-saved): FULL_MATCH only.
- Cause:     the tail binding is not per-store choosable — it follows the value the expression is bound to — but every re-binding attempt fails distinctly: (a) spelling the tail `btm_cb.num/max_bd_entries` (anonymous global) does NOT reuse r31: after the intervening `GKI_freebuf` call MWCC rematerializes `&btm_cb` into volatile r3 (`lis r3; ...; sth 0(r3)` x2, +8 bytes, 44.1%); (b) swapping the two locals' declaration order only rotates global colors (69.7%); (c) a block-scoped late `tBTM_INQ_CB *p_db = &btm_cb` also rotates global colors with `p_inq` moving to r31 (62.1%). The same-TU sibling `btm_event_filter_complete` carries the MIRROR residual for the identical source pattern (retail r30 vs decomp r31, 98.5%): the allocator picks opposite bases per function context, so neither spelling is universally right.
- Fix:       none found. Source left at the honest baseline (`p_inq->` tail, 97.0%, split PASS). Do NOT retry (a)/(b)/(c) without a new reason.
- Result:    stalled us-802eb330 (ACTIVE 97.0%, stall noted, source untouched at baseline; TU split PASS 0x1A98).
- Confidence: negative_result (3 TU builds + full-asm mechanism analysis + witness-gate probe + positive controls proving pipeline sensitivity)
- Applies to/a.k.a.: any two-base (early named + late anonymous global) function whose only residual is the tail binding after a call; next angle is the exact-original SDK statement order for the post-call tail or an unknown IR shape that extends the late base's live range — cf. the in-compare assignment-operands record for another abi-boundary color wall

## aligned(8) on a small rodata blob flips the unroller's addressing template (Wii/1.1 -O4,p, FULL_MATCH us-803975d4 sjrbf_Create)
- Symptom:   8x-CTR-unrolled counted scan over a 72-byte-record BSS pool comes out bump-pointer (`add r7,base,off; lwz r0,4(r7)` per check + `li off,0`, +64 bytes) while retail uses fixed-base displacements (`lwz r0,4/76/148/...(r6)`, base bumped 576/chunk). Loop/index/bound/post-test spellings (local pointer vs inline cast vs typed-global array, break vs goto, `==0x100` post-test) all keep the bump form.
- Cause:     the `__attribute__((aligned(8)))` the scaffold put on the 16-byte rodata UUID blob (consumed in the found path as `self->uuid = blob`) flips MWCC's unroll addressing template for the whole function. Bisected with standalone probes under the exact TU flags: no-attr/displacement vs attr/bump, and per-symbol: uuid-attr alone flips, err-blob/vtable attrs do not.
- Fix:       drop `__attribute__((aligned(8)))` from the UUID blob decl only (keep `__declspec(section ".rodata")`). Layout-neutral here (blob is first in .rodata at offset 0; the following blob keeps its own attr), data stays 100%, and the loop falls out as retail's displacement unroll with identical prologue/found path (0 mismatches, 0 reg_swap, size-exact 0x134/0x134).
- Result:    FULL_MATCH us-803975d4 (6.5%/78-structural -> 100.0% semantic-certified).
- Confidence: repo_proven (probe matrix: base/local/goto/typed-global x harness/standalone x per-symbol attr).
- Applies to/a.k.a.: any unrolled scan whose residual is an extra per-check `add` + offset IV with otherwise identical unroll factor/exits; check scaffold-added aligned() attrs on small rodata/data blobs referenced in the function before rewriting the loop.

## Derived reslist dtor loop-temp color wall + sentinel-first cmpl fix (Wii/1.1 -O4,p, stalled us-8045514c)
- Symptom:   inlined double-guard derived reslist dtors (`if (t != 0) { if (t != 0) { walk; free; } if (deleting > 0) delete t; }`) sit at 84.8% hexdiff, 0 structural / 7 pure reg_swap, size-exact: the null-walk loop compiles node=r5/prev=r4 while retail has node=r4/prev=r5 (init `lwz r4,4(r3); lwz r5,0(r4)` vs retail coalesced `lwz r4,4(r3); lwz r4,0(r4)`). The same-TU/base dtor (single guard) with the IDENTICAL loop body is 100%. `cycle` witness-gate `rho | no consistent bijection` closes EQUIVALENT_MATCH: FULL_MATCH only.
- Cause:     MWCC's temp-coalescing decision for the `node = sentinel->mNext` init chain plus the loop-carried copy temp is insensitive to every high-level permutation: declaration order/scope (inner, outer, function-top, multi-declarator, prev-first), init form (single, two-step, prev-born-first), loop construct (while vs for), advance spelling (`node->mNext` vs `prev->mNext`), and an extra named sentinel temp all produce byte-identical output. Separately, the loop-condition operand order selects the `cmpl` operand order, so `sentinel != node` fixes that line even with wrong colors (84.8% -> 87.0%, compensatory, not the true spelling).
- Fix:       partial: spell the condition sentinel-first (`while (t->mStartNodePtr != node)`) to fix the `cmpl` line (87.0%, 6 reg_swap left). The true original is almost certainly `node != sentinel` (all siblings/base agree) PLUS an unknown color flip. Next angles: extend `node`'s live range past the loop (retail's first post-loop sentinel reload prefers r5, hinting node still occupies r4 there); crack one member of the wall family and transfer.
- Result:    stalled us-8045514c (ACTIVE 87.0%, 0 structural, 6 reg_swap, size PASS, line-neutral, no TU regression).
- Confidence: negative_result (1 cycle + 8 TU builds: pipeline sensitivity proven by the condition-flip delta; family evidence across CDeviceFont/CDeviceFile/CDeviceFontLayer/ScheduleList derived dtors, all base-100/derived-stuck identically)
- Applies to/a.k.a.: every inlined derived reslist dtor with the double null-guard shape (`__dt__reslist_*` vs `__dt___reslist_base_*`); do NOT retry declaration/birth/loop permutations without a new reason — see also the in-compare assignment-operands record for another color wall with an abi-boundary refusal.

## No-local CSE-temp loop element — declared element local pins wrong scratch color (Wii/1.1, FULL_MATCH us-804634b4)
- Symptom:   pure reg_swap (walker r5<->r3, element r3<->r5), 0 structural, witness-gate abi-boundary (r3 perm) so EQUIVALENT_MATCH impossible; decl hoists/anchor locals never move colors.
- Cause:     A declared `T* inst = arr[i];` local gets its virtual-register number at declaration time (older), while the compiler-fused singleton-copy walker temp is created later (younger); youngest-first scratch coloring then gives the walker the lowest free reg. Retail has no element variable at all.
- Fix:       Delete the element local; spell `arr[i]->field` inline at each use and let CSE merge the loads into one anonymous temp (younger than the walker), flipping walker->r5 / element->r3 with identical bytes.
- Result:    FULL_MATCH layoutFreeCallback__10CLibLayoutFv (81.2% -> 100.0%, 0x80/0x80)
- Confidence: repo_proven
- Applies to/a.k.a.: scratch (r3/r5-class) color walls immune to declaration/birth-order permutations; variable-set differs, not ordering; likely same fix for sibling layoutAllocCallback__10CLibLayoutFv.

## mwsfsfx signed-halving idiom — `v/2` vs hand-rolled `(v+(v>>31))>>1` (Wii default flags, 83% near-miss)
- Symptom:   hand-rolled unsigned `(s32)(v + (v >> 31)) >> 1` emits retail's `rlwinm/srawi` but the `add` operands come out swapped (`add r0,r31,r0` vs retail `add r0,r0,r31`); flipping source order does not move it.
- Cause:     MWCC canonicalizes the general unsigned-add operands (register-sorted) while the signed-division idiom has fixed lowering: `rlwinm r0,v,1,31,31` (srwi-31 sign extract) + `add r0,r0,v` + `srawi r0,r0,1`.
- Fix:       declare the halved value `s32` and write `w0 = v / 2;` — add order fixed byte-for-byte, no other bytes disturbed (lwz/stw identical for u32/s32).
- Result:    mwsfsfx_SetYcc420plnInfToSfx 80.9% → 83.0% (fixed 1 structural + add colors); residual is an unrelated one-slot `stw`/`srawi` store-sinking scheduler wall.
- Confidence: repo_proven (single-target, byte-verified delta)
- Applies to/a.k.a.: any `rlwinm rX,rY,1,31,31 / add / srawi` round-toward-zero halving; prefer the `/2` idiom over manual sign-fixup spells.

## sjuni_Reset tail fresh-zero/scratch-index color wall (Wii/1.1 -O4,p, stalled us-80398cf4)
- Symptom:   chunk-pool reset tail sits at 85.7% hexdiff, 0 structural / 10 reg_swap, size-exact (0x118/0x118): retail `rlwinm r4,r7,4 / li r0,0 / stwx r0,r8,r4 / add r4,r8,r4 / stw r0,...` (index in named r4, fresh scratch zero r0) while every source spelling yields `rlwinm r0,r7,4 / li r5,0 / stwx r5,r8,r0 / add r4,r8,r0 / stw r5,...` (index in scratch r0, zero reuses loop-zero r5). Loop body, prologue, and error branches are byte-identical; only the 10-instruction tail differs. `cycle` witness-gate `abi-boundary | rho perm maps gpr r0 -> r5` closes EQUIVALENT_MATCH (r0 is ABI-fixed): FULL_MATCH only.
- Cause:     Same-asm streams color differently by vreg birth numbering in the post-scheduling PCode graph (cf. CCol6Invite scheduler-numbering precedent): retail's tail value-web pops/colors before the index-web (fresh r0 first pick, index takes r4, address reuses r4), while every reconstruction births/colors index-first (r0) with the value reusing dead loop-zero (r5). Insensitive to all high-level levers: declaration orders (p,i,chunks=85.7 / i,chunks=56.3 offset-codegen / chunks,i=50.7), split declare/assign, pointer-cursor vs pure-index loop, NULL vs 0 vs (T*)0 cast literals (always merge), chunk-first vs queue-first tails (scheduler keeps source order), (chunks+i)-> / &chunks[i]-> / byte-offset address forms (identical codegen), int j=i copy (coalesced away), int zero var + casts (rotates named colors, tail unchanged), int off=i*16 (absorbed), per-function opt_propagation pragma on/off (no effect), loop-body reorder (structural), comma RHS (MWCC error 10209). MWCC_CASES "signed loop / i-first" recipe for this function is stale (reproduces 56.3%, not FULL_MATCH).
- Fix:       none found (near-miss). Best reachable is the 85.7% shape: `SJUNI_CHUNK *p; int i; SJUNI_CHUNK *chunks;` (p-first decl) + `p = chunks` cursor loop (`p->next = &chunks[i+1]; p->ptr = NULL; p->size = 0; p++;`) + index-form tail (`chunks[i].next/ptr/size = NULL` then 4 queue NULLs) (+56.3%->85.7% from the cursor loop alone). Next angles: MWCC Wii/1.1 coloring/vreg-numbering probe to pop the tail value-web before the index-web; a live-across-tail web that raises the value-web degree without emitting code.
- Result:    stalled us-80398cf4 (ACTIVE 85.7%, 0 structural, 10 reg_swap, size-exact 0x118/0x118, 15/15 TU siblings intact).
- Confidence: negative_result (18+ hexdiff builds + 2 cycles: web-insensitivity proven across literal/cast/address/order axes; witness abi-boundary verified twice)
- Applies to/a.k.a.: stwx+add entry tails where retail pairs a fresh scratch zero with a named index (cf. SFPTS_WritePtsQue r11 wall, CCol6Invite numbering wall); do NOT retry literal/cast/address/decl-order permutations without a new reason.

## Nonvolatile-pool color wall — MWCC never touches retail's lowest nonvolatile anywhere (Wii/1.1, stalled us-8036b350)
- Symptom:   united call-crossing scalar range colors r29 (decomp) vs r28 (retail) with zero other diffs except downstream homing (stale-constant reuse register, held-across-copy constant in r0 vs r28, copy interleave shift, epilogue save/restore pair missing = 1-insn size delta). MWCC never emits r28 anywhere in the function in ANY variant, while retail uses it for three disjoint ranges (early scalar, zero constants, held copy constant). `cycle` witness-gate `size` (299 vs 298) closes EQUIVALENT_MATCH: FULL_MATCH only.
- Cause:     MWCC fixes the nonvolatile pool from max-concurrent demand (here 3: copy base + 2 unroll temps) and coalesces the early scalar into the pool (r29, shared with the late base); retail pooled 4 (early scalar kept separate in r28, late constant-holder coalesced into it). Pool-affecting levers are demand, not spelling: insensitive to scalar declaration order (first/swapped/mid-buffer), init form, assignment birth order, extra volatile-homed scalar temps, OR-operand order, statement order, and buffer-count (dead arrays are layout-neutral). Graph-sensitivity probe: adding a def on the fallthrough switch path (`region = 1` in the otherwise undef `case 2:`) splits the web (early r30 + switch-range r4-volatile, score drops) — proving the coloring responds to web structure while showing the split is the wrong shape.
- Fix:       none found (near-miss). Diagnostic shortcut: if a fresh literal rematerialization (`li r30,1` for a value provably live in r29 across calls) appears, the allocator is refusing cross-call reuse — the web, not the spelling, differs; do not chase the remat site, chase pool demand. Next angles: a mechanism raising concurrent nonvolatile demand to 4, or a vreg-coloring-order probe; do NOT retry ordering/typing/temp permutations.
- Result:    stalled us-8036b350 (ACTIVE 82.9% hexdiff, 14 structural + 37 reg_swap, size 0x4AC/0x4A8 PASS with 0x34 split spare, 12 reverted single-variable builds + positive-control literal flip).
- Confidence: negative_result (12 hexdiff builds this session + 15+ prior variants + 2 witness cycles: size-gate verified, sibling __VISetMacrovision verified unregressed at 487 structural)
- Applies to/a.k.a.: united-range scalar color walls where one side never touches the lowest nonvolatile (cf. sjuni_Reset tail wall, reslist base-100/derived-stuck wall); shared-inline-helper caution — copy-schedule probes (buf[0] store placement) regressed the sibling's 13 sites, always check both functions.

## Stack arrays sorted by size descending (stable) — declaration order only breaks same-size ties (Wii/1.1, observed vi3in1)
- Symptom:   twelve `u8 buf[2..27]` locals land high-to-low as 27,5,3,3,3,2,2,2,2,2,2,2 with ties in exact declaration order — neither declaration nor assignment nor reverse order predicts the layout.
- Cause:     MWCC sorts stack array allocations by size descending (stable): reordering declarations only moves offsets within a size class, never across classes. Same-size swaps still shift every store (stable sort preserves the new tie order), so buffer declaration order is NOT a free lever.
- Fix:       when reconstructing a multi-buffer function, match sizes first (offsets follow), then permute only same-size declarations to align offsets. Dead (unreferenced) arrays are layout-neutral only if the same bytes are allocated anyway (e.g. an inlined helper's buffer coalescing into the dead slot — verify by hexdiff, then prefer dropping the false declaration).
- Result:    layout diagnostic used on __VISetRevolutionModeSimple (us-8036b350): confirmed all 12 buffer offsets without touching declarations.
- Confidence: hypothesis (single-function observation, byte-verified offsets; needs a second function to promote)
- Applies to/a.k.a.: multi-stack-buffer Wii/1.1 functions; companion to the lbzu-HA/LO base-finalize idiom (`lis rX,HA(sym); lbzu rY,LO(sym)` loads byte 0 AND completes the base for small-offset indexed loads — seen for VIZeroACPType .bss access, pre-link display `lbzu rY,0(rX)`).

## Flat extern "C" __ct__ must return Class* to preserve this (Wii/1.1, proven us-802b0b74)
- Symptom:   __ct__CLoad sat at 40.0% hexdiff, 8 structural + 10 reg_swap, size 0x78/0x74: retail `li r4,0; li r0,1; stw r4,...; or r3,r30,r30; stw r4,...` (zeros via r4, this restored into r3 mid-sequence) while decomp emitted `li r3,0; ...; stw r3,...` (zeros via r3, no restore).
- Cause:     a `void`-returning flat ctor leaves `this` (r3) dead after the member-ctor call, so MWCC reuses r3 as the zero temp; retail keeps `this` live for return, forcing zeros into r4 and emitting `or r3,r30,r30` to restore the return value.
- Fix:       declare `extern "C" CLoad* __ct__CLoad(CLoad* self, u8 arg)` and end the body with `return self;`. No other source change; store order already matched.
- Result:    us-802b0b74 40.0% -> 100% FULL_MATCH, size 0x78/0x78.
- Confidence: repo_proven (single-target, byte-verified delta)
- Applies to/a.k.a.: any flat `extern "C" __ct__Class` whose retail shows `or r3,rX,rX` after the member-ctor call plus zero stores via r4; companion to the "flat extern C avoids duplicate vtable store" idiom.

## Declared self-rooted walk pointer + decl-order coin flip for two-loop color pairing (Wii/1.1, proven us-804e7e84)
- Symptom:   schedStepEntries sat at pure reg_swap (13 swaps, 0 structural, size PASS): retail walks both 32-entry s16 loops with ptr=r29/counter=r30, decomp had them swapped. Hoisting `int i` + `item` only reduced it to a clean r29<->r30 swap; `++i`, `i=0;for(;)`, decl swaps of item/i, and `s16* handles = self->mHandles` (materializes `addi rX,r28,152` + 0-offset walk = 36 structural) all failed. Witness-gate abi-boundary (r29->r30 perm) closed EQUIVALENT_MATCH: FULL_MATCH only.
- Cause:     retail's walk is self-rooted (`lha 152(r29)`, base starts at self, +2/iter) AND older than the counter. A declared mHandles-rooted pointer can never produce the 152 form (MWCC folds it to 0-offset), so the walk must be a declared self-valued `s16*` advanced per-iter and read at a fixed displacement: `s16* h = (s16*)self; ... h[76] ...; h++` (76 = offsetof(mHandles)/2). Declare-before-assign split puts the `or` copy after the flag store (retail schedule). A single shared `h` matches loop 1 but its loop-2 reassignment (live-in from loop 1) births a younger web and loop 2 stays swapped; a second pointer `h2` for loop 2 fixes loop 2 but breaks loop 1 (valid-3-coloring see-saw). Declaring `h2` BEFORE `h` flips the heuristic pairing to retail's on both loops.
- Fix:       `s16 *h2, *h; int i;` at function top (h2 first), `h = (s16*)self; i = 0; for (; i < 32; i++, h++)` using `h[76]`, loop 2 same shape with `h2[76]`. Counter shared.
- Result:    us-804e7e84 81.9% (13 reg_swap) -> 100.0% FULL_MATCH (0 structural, 0 reg_swap), semantic-certified.
- Confidence: repo_proven (single-target, byte-verified; ~8 hexdiff builds converging: hoist, pre-incr, handles-materialize, h-walk, assign-split, counter-split, two-pointer, decl-flip)
- Applies to/a.k.a.: any self-rooted indexed table walk (`disp(base)` + small-stride back-edge) whose counter sits one nonvolatile above the walk reg; when two same-shape loops see-saw one-match/one-swap, try per-loop walk pointers + declaration-order permutation before touching assignments (decl order never moves instructions).

## __ptmf_null triple: reinterpret struct-view single-base + split top-bare decls (Wii/1.1, 89.6% us-80276dcc)
- Symptom:   singleton-factory `__ptmf_null[1],[0],[2]` load-both/store-both copy sits at 85.7% (11 reg_swap) with TWO extra `__ptmf_null` LO relocs and a dual base (`addi r10,r11,0` + loads off both r10/r11, [0]-loads folded to `lwz rX,LO(rHA)`); any explicit pointer conversion fixes relocs but globally rotates callee-saved colors (params r28-30/obj r31 -> r29-31/r27, 53.2%). Witness-gate `reloc` then `abi-boundary r0->r3` refuses EQUIVALENT (r3 unconditionally fixed; r0/r9/r10 renamable).
- Cause:     Implicit array decay lets MWCC fold `@l` into the zero-displacement [0] loads while completing a second full base for [1]/[2]; any mid-block conversion makes the pointer an opaque long-lived temp and re-rolls callee-saved coloring. Declaration position (not order: both orders inert, renames inert) drives the callee-saved assignment: bare declarations at function top (before the allocate) number into the retail slots.
- Fix:       `const PtmfNullWords* ptmf; u32 ptmfSecond; u32 ptmfFirst; u32 ptmfWord2;` bare at function top (Second/First order irrelevant), then `ptmf = reinterpret_cast<const PtmfNullWords*>(__ptmf_null); ptmfFirst = ptmf->w1; ptmfSecond = ptmf->w0;` stores per slot, reassigned per triple (CCol6CheckBat shape, u32 members fine). Yields single r11 base, exact reloc lists, exact callee-saved colors, 0 structural. Residual wall: the [1]/[0] temp pair keeps swapped colors (r3/r0 vs r0/r3 both triples) across every axis — needs triple1 byte-exact for the witness.
- Result:    stalled us-80276dcc (ACTIVE 89.6%, 0 structural, 8 reg_swap, size-exact, relocs-exact; 85.7% -> 53.2% -> 89.6% across ~15 builds, unit 11/14 intact).
- Confidence: negative_result (plateau proven across decay/direct/cast-kind/interleave/fresh-local/inline/reorder axes; rotation fix + fold fix each byte-verified)
- Applies to/a.k.a.: every null-ptmf singleton factory (CSysWinSave/CCol6CheckBat/CMenuPTState/CUICfManager share the idiom; CCol6CheckBat 93.9% shows the same triple1-swap pattern); when an explicit conversion rotates a whole function, hoist bare decls to function top before touching anything else.

## Witness reloc-gate fires alone on FPR-only residuals (volatile FPR renames not abi-boundary)
- Symptom:   CfT_PlayClockUpdate (90.5%, 6 FPR swaps) and CfT_PlayClockAdvanceInt (62.2%, 14 FPR swaps) cycle with `witness-gate: reloc | slot N: lbl_eu_80666220 vs @433x` and NO abi-boundary/rho-refusal line, while a GPR r3-involving swap (CfTFile_FirstEntry3, r3<->r5) gates `abi-boundary` on the same runner. Instruction bodies are otherwise identical or pure-FPR-rotated.
- Cause:     The gate pins volatile GPRs r3-r10 and callee-saved r14-r31 (plus PS/PSQ renames), but volatile-FPR (f0-f13) renames do not trip it; the single reported reason is the 2^52 magic pool slot (retail named sdata2 double vs decomp anonymous pool, same value) which the gate treats as a reloc mismatch.
- Fix:       None in high-level C this batch: plain `(float)u16/u32` casts synthesize the correct fsubs sequence but always reference an anonymous pool; explicit union stack-double + named-sub gets the name but lowers to double fsub+frsp (+1 insn). The residual is therefore magic-only once FPR colors are right; do not chase FPR declaration order to satisfy the gate.
- Result:    Stalled us-8006a944 / us-8006abb8 (ACTIVE; gate reloc-only); us-8006a8fc (100% insn, same slot) same wall. Cf. §7i / §3201 (i2f magic pools as genuine gate-2 blockers).
- Confidence: hypothesis (two-target gate evidence; short-circuit-on-first-reason not ruled out — needs a both-gates function to promote).
- Applies to/a.k.a.: any function whose only witness complaint is a pool-name reloc slot; kyoshin/cf/CfTFile clock family (PlayTimeGet/PlayClockUpdate/AdvanceFrame/AdvanceInt/Accumulate share slot lbl_eu_80666220).

## Split-local declaration order is inert; vregs number by first assignment (SSA) — g3d_maya SRT proof (Wii/1.1, -O4,p)
- Symptom:   0-structural residual with pure register-color diffs; reordering split `f32 a, b, ...;` declarations changes nothing (byte-identical output).
- Cause:     Uninitialized declarations do not number vregs; vregs are born at first assignment (SSA). Only assignment order / temp set affect allocation.
- Fix:       Don't permute bare decls. To move colors, change first-assignment order (moves loads too — usually structural) or the named-temp set (schedule-fragile). If neither preserves schedule, record the wall.
- Result:    MakeTexSrtMtx_SRT products-first decl perm was byte-identical (58.2%/0/23 held); the same TU's 3dsmax "declaration order" recipes worked only because they used initializers (decl+assign coupled).
- Confidence: repo_proven
- Applies to/a.k.a.: split decl/assign idiom; brief §5 "numbers copy-temps by declaration order" applies to initialized/compiler temps, not split locals.

## hexdiff/objdiff mislabels paired-single arithmetic as ps_cmpo0; use powerpc-eabi-objdump (Wii/1.1, all TUs)
- Symptom:   A region shows runs of `ps_cmpo0` (or `.4byte` fcmpo/fmr/frsp) with no visible arithmetic, suggesting missing VEC3Dot/VEC3Add/ps_add/ps_mul/ps_madd/ps_sum0/ps_sub output; "no NaN checks / no lenSq computation" theories follow.
- Cause:     The objdiff PS decoder maps paired-arithmetic XOs to `ps_cmpo0` (and leaves fcmpo/fmr/frsp as `.4byte`). Byte comparison is unaffected (hex compared), only the displayed mnemonic misleads. `build/binutils/powerpc-eabi-objdump -d` knows classic `ps_mul`/`ps_madd`/`fcmpo`/`frsp` (but mislabels other PS ops as VSX `vsel`/`xsaddsp`/`xsmaddasp`/`lq`/`mtvsrbmi`); cross-reading both decoders against raw words (`op31_535` = lfsx, `op31_663` = stfsx) recovers ground truth.
- Fix:       Never conclude "arithmetic missing" from hexdiff mnemonics. Verify with objdump histograms/word dumps first. Probe compiles (`VEC3Add` micro-TU) confirm ASM blocks emit correctly.
- Result:    MdlDyn_AddVecAll/PushSphere/PushPole "missing add/dot" mysteries dissolved (VEC3LenSq vs VEC3Dot load-shape question was real and decidable from the W-bit + load pattern, not from arithmetic absence).
- Confidence: repo_proven (probe + objdump cross-check, CMdlDynamics batch).
- Applies to/a.k.a.: any paired-single-heavy function where hexdiff shows cmpo runs; VEC3LenSq (`psq_l xy W0` + `lfs z` + `ps_mul`/`ps_madd`/`ps_sum0`) vs VEC3Dot(p,p) (`psq_l yz` + `psq_l yz` + ...) disambiguation by load shape.

## Shared miss-epilogue via inverted-`<` + nested body (Wii/1.1, -O4,p)
- Symptom:   Two `return 0` miss paths (NaN/lenSq/radius checks) each emit `li r3,0 + b` (+2/+4 bytes); naive `>=`-to-`goto fail` gives `fcmpo`+`cror`+`beq` (NaN-safe but byte-wrong vs retail `fcmpo`+`bge`).
- Cause:     Retail shares ONE `li r3,0` miss epilogue: miss branches target it directly. `if (x >= C) goto fail` renders as cror+beq; inverted `if (x < C) { body; return 1; } fail: return 0` (or nested-`if` with body, or `continue` for loop misses) renders miss-taken branches as plain `bge`/`beq` to the shared pad, with the hit `return 1` contributing the single jump-over-`li0` `b`.
- Fix:       Prefer `if (x < C) { ...; return 1; } return 0` / nested-`if` bodies / `continue` over `>=`-goto-fail when retail shows cror-free `bge`+shared-`li0`. (`==`-miss can stay `if (x == C) {} else` or `==`-`continue`; `==` needs no cror either way.)
- Result:    MdlDyn_PushSphere `bge` landed, `li`/`b` extras gone (0x2e8 -> 0x2dc); same shape copied to MdlDyn_PushPole radSq checks.
- Confidence: hypothesis (two-function evidence; NaN semantics differ from naive `>=` and were not audited beyond byte-match).
- Applies to/a.k.a.: sphere/capsule push-out helpers and any dual-miss early-out with a shared `li r3,0` pad; cf. inverted-float-select entry (`<`-with-body-first gives `fcmpo`+`bge`).

## Cached loop bound becomes ctr/bdnz; reloaded field stays lha/cmp/bc (GC/3.0a5.2, -O4,p)
- Symptom:   Retail loop shows `b COND; BODY; addi ptr; addi idx; COND: lha cnt; cmp idx,cnt; bc BODY` but decomp emits `mtspr ctr` + `bdnz` with the bound hoisted.
- Cause:     Caching the bound in a local (`s16 count = obj->field; for (i=1; i<count; ...)`) lets MWCC prove loop invariance and select the counted-loop template. Retail re-reads the field every iteration.
- Fix:       Compare against the field in the loop condition (`for (; i < obj->field; ...)` or `for (i=1; ...)` with the field spelling). Bonus color rule from the same functions: declare the index before the element pointer (`int i = 1; Elem* e = base+1;`) to land index=r4/ptr=r5 instead of the reverse.
- Result:    CamEvtStepShk/PushCamEntry shake loops: ctr/bdnz eliminated, loop skeleton (b/COND/addi/addi/lha/cmp/bc) and r4/r5 colors now match retail; function % flat (other residuals dominate) but skeleton verified byte-aligned in --asm.
- Confidence: repo_proven (before/after --asm on kyoshin/cf/CfCamEvent_1, GC/3.0a5.2).
- Applies to/a.k.a.: any `for (i=1; i<cached; i++, ptr++)` over an object field where retail shows per-iteration lha; cf. declaration-order allocator contract (register_mapping.md).

## TU-wide C++-mangled vs retail-plain symbols silently disables reloc-name canonicalization (Wii/1.1, all TUs)
- Symptom:   Every function in a TU shows 0-insn-diff but `cycle` reports function 0.0% + `witness-gate: reloc | slot N: lbl_eu_X vs @NNNN`, or pool-name drift that no source edit fixes. Decomp `nm` shows `Foo__Fv`/`Foo__FP7Bar` while retail `nm` shows plain `Foo`.
- Cause:     The reloc canonicalizer pairs functions by exact symbol name; mangled-vs-plain never pairs, so value-equal pool relocs (e.g. compiler-synthesized 2^52 conversion magic vs the TU's named .sdata2 double) are never canonicalized and the witness reloc-gate refuses. The comparator itself ignores reloc NAMES (functionRelocDiffs=data_value), so pairing is the only blocker.
- Fix:       Declare the affected functions `extern "C"` (header extern "C" block, following the CfT_SelectDisplayMode precedent) so decomp emits the plain retail names; verify with `nm` on both objects first. Callers in other TUs already treating them as `extern "C"` confirms the direction. No `configure.py`/UNIT_RULES/postprocess change needed.
- Result:    kyoshin/cf/CfTFile: CfT_PlayTimeGet (0 mismatches) went straight to FULL_MATCH on the next cycle; PlayClockUpdate 90.5%->99.3% (reloc gate cleared, pure FPR-rotation residual exposed); PlayClockAdvanceFrame/FirstEntry3/FindEntryByKey pair correctly for the first time.
- Confidence: repo_proven (before/after cycle on 5 targets, same batch; no codegen change, names only).
- Applies to/a.k.a.: any TU whose decomp symbols carry `__F` suffixes while retail's are plain; the "7 functions went 0-mismatch FULL_MATCH" recipe (extern "C" + in-TU magic definition). Check FIRST before chasing pool/conversion spellings.

## Jump-table body mapping via .rela.data — cascade collapse → per-body fixes (Wii/1.1, FULL_MATCH path)
- Symptom:   3000-insn switch function at ~55% with ~1500 structural; offset-aligned hexdiff shows a single early insertion cascading thousands of rows, masking the true (small) diff count.
- Cause:     Alignment cascade from 1-2 extra/missing insns per case body; the real diffs are a handful of same-size bodies.
- Fix:       Parse retail+decomp .rela.data dense R_PPC_ADDR32 runs per function symbol: sorted unique addends = case-body layout; consecutive differences = body sizes. Diff the size sequences rank-by-rank — only truly-differing bodies show up (here: exactly 2/86). Reverse-map target addend -> table index -> source case, then fix each flagged body from its own retail rows. Same technique maps any body address to its switch idx.
- Result:    mpvabdec IntraBlock/DC11/NintraBlock 54-76% -> 91-97% / 20% by fixing only flagged bodies (escape e-sharing, sign swaps); all sizes exact.
- Confidence: repo_proven
- Applies to/a.k.a.: large switch/decoder TUs (mpv, VLC, Huffman); jump-table base via .rela.text table symbol; entry[i] == idx i.

## Escape-head value sharing (s32 e) + f00 &0xFFFFFF mask (Wii/1.1, FULL_MATCH path)
- Symptom:   Escape-coded VLC body +1 insn vs retail: decomp recomputes (s8)((bits>>13)&0xFF) inside the else while retail hoists one extsb above the branch and reuses it for both test and recompute; f00 computed via srawi instead of rlwinm.
- Cause:     Test uses &0x7F while else uses (s8)&0xFF (no CSE across branch); f00 (s8)(v>>8) lowers to srawi+extsb instead of rlwinm+extsb.
- Fix:       Keep the &0x7F test (retail emits the dead extract too); introduce `s32 e = (s8)((bits >> 13) & 0xFF);` before the test and use `(e << 1)` in the else (forces the hoist+reuse, -1 insn, positions match); write f00 as `(u32)(s8)(((v >> 8) & 0xFFFFFF))` (value-identical, forces rlwinm extract). Do NOT use &0xFF on f00 (adds a redundant mask insn).
- Result:    Escape bodies 0x138/0xf0 -> exact 0x134/0xec, cascade collapsed (~1400 structural gone per function).
- Confidence: repo_proven
- Applies to/a.k.a.: MPEG/VLC escape paths with (x-1)|1 / -((x-1)|1) sign forms; Wii/1.1 trace-scheduling hoists.

## ON/OFF string ternary → default-then-overwrite sharing one pool base (Wii/1.1, FULL_MATCH path)
- Symptom:   Error call `Error(fmt, pause==1 ? ON : OFF)` with string-table offsets compiles to a branchy ternary: false-value first, then true-value, then the base address recomputed a second time for the format arg (+2 insns, +8 bytes, base in two regs). Retail instead keeps ONE pool base in a scratch reg, derives fmt then default-OFF from it, and conditionally overwrites with ON.
- Cause:     MWCC lowers the `?:` with the false value as default in a branch; the later format-arg computation cannot reuse the dead base and rematerializes `lis/addi`. Splitting into adjacent assignments lets CSE share one base.
- Fix:       Replace the ternary with two locals assigned adjacently after any preceding call: `fmt = lbl+0x106; onoff = lbl+0x12c; if (pause == 1) onoff = lbl+0x129; Error(fmt, onoff);`. Retail order is fmt first, OFF default, conditional ON overwrite.
- Result:    CriWare mwsfdply mwPlyPause 61.4%->100% FULL_MATCH (size 0x14c->0x138 exact, split PASS); sibling mw_sfd_start_ex 36.8%->77%+ with the identical change (same family, shared root cause).
- Confidence: repo_proven (before/after hexdiff --all on 2 same-TU siblings + cycle accept on one).
- Applies to/a.k.a.: ON/OFF, TRUE/FALSE, enum-string error args of the form `Error(base+K1, c ? base+K2 : base+K3)`; the fmt-first/default-second/overwrite-third schedule.

## Stack-slot order = reverse declaration, use-order independent (Wii/1.1, probe-proven)
- Symptom:   Function with N address-taken word locals places them at unexpected sp offsets; guessing declaration order from use order fails.
- Cause:     MWCC assigns stack slots strictly by declaration order, last-declared = lowest offset (sp+8 upward), regardless of first-use order. Probe: `int a,b,c,d,e` used in order a..e vs e..a both yield a@sp+0x18 ... e@sp+8.
- Fix:       Declare locals so that reverse-declaration matches the retail slot map (lowest-offset local declared LAST). For slots t1@8,t2@12,...,fps@48 declare `fps, cond, ..., t2, t1`.
- Result:    mwsfdsl mwPlyExecInfiniteLoopHandle 11-word frame slots exact on first reorder (304 structural -> ~0 from slots alone).
- Confidence: repo_proven (wibo probe pq1/pq2 + same-TU application).
- Applies to/a.k.a.: any function with 3+ address-taken locals; frame-layout reconstruction.

## `if (x>=0) {S} else {return}` emits trailing dead branch (Wii/1.1, probe-proven)
- Symptom:   Retail has `[cmpi; blt FAR-epilogue; S; b NEXT; b FAR-epilogue]` with the last branch unreachable; writing `if (x<0) return; S;` gives `[bge; b FAR; S]` instead (extra branch, wrong polarity).
- Cause:     MWCC lays out `if (C) {S} else {return}` as branch-on-C-true directly to the far return, S inline, `b JOIN`, then the else-return block out of line. Probe `qa` reproduces the exact shape incl. the dead `blr`.
- Fix:       Write the test in `>=` form with the work in the then-block and a bare `else { return; }`, e.g. `if (r >= 0) { self->f = 2; } else { return; }`.
- Result:    mwsfdsl infinite-loop calc tail byte-exact (eliminated phantom +1 branch).
- Confidence: repo_proven (wibo probe qa vs qb + same-TU application).
- Applies to/a.k.a.: early-exit tails with a trailing dead branch; dual is `if (C) return; REST;` -> single branch-on-true to return (qb shape).
