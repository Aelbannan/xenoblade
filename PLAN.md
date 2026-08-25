# Xenoblade Chronicles Wii — decomp / matching fork plan

**Purpose:** policy and workflow reference for this **private/downstream** byte-matching
decompilation fork of *Xenoblade Chronicles* (Wii). The former split-screen co-op feature
effort is parked; the co-op plan sections were removed. This document now retains only the
sections agents still need: legal boundaries (§2), architecture invariants kept for
reference (§3), and the feature-driven decompilation / byte-matching loop (§17).

> Section numbering below preserves the original PLAN.md numbers so cross-references
> (`PLAN.md §17`, `PLAN.md §17.6`, `PLAN.md §§2/3`) stay valid.

**Companion documents:**

- `tools/coop/targets.json` — canonical function registry and current target state (equivalence certificates live in the gzip JSONL sidecar `tools/coop/targets.certs.jsonl.gz`; the runner merges it transparently on load)
- `AGENTS.md` / `.agents/skills/xenoblade-decomp/SKILL.md` — agent entry + decomp loop

---

## 2. Legal, repository, and contribution boundaries

Use a legally obtained game extraction. The public decompilation requires the user's own `main.dol` and REL files and does not distribute original game assets or assembly.

Do not commit or distribute:

- disc images;
- `main.dol` or REL binaries;
- extracted proprietary assets;
- complete rebuilt proprietary executables;
- large binary excerpts that reconstruct copyrighted code.

Distribute only original tooling, source modifications, documentation, symbol notes, and legally appropriate patch/delta data.

The public `xbret/xenoblade` repository currently states that LLM-assisted contributions are not accepted upstream. All agent-generated work must remain in a clearly labeled private or downstream fork. Do not submit AI-assisted reconstruction to the upstream repository.

Cloud agents should receive minimal function dossiers where practical: relevant disassembly, decompiler output, types, symbols, traces, and diffs. Prefer local agents for full-binary access.

---

## 3. Non-negotiable architecture invariants

Every agent must preserve these rules.

### 3.1 One simulation, two presentations

```text
Controller 1 -> PlayerState[0] -> party actor 0 --+
                                                  +-> one authoritative simulation update
Controller 2 -> PlayerState[1] -> party actor 1 --+

simulation result -> camera/HUD context 0 -> viewport 0
                  -> camera/HUD context 1 -> viewport 1
```

Gameplay updates once. Presentation renders twice.

### 3.2 Never call the complete frame update twice

Do not call any of these twice merely to obtain a second view:

- actor/AI update;
- battle update;
- task movement/update traversal;
- animation time advancement;
- particle simulation;
- UI state update or animation advancement;
- audio event generation;
- RNG-consuming logic;
- streaming decisions;
- save/quest/event logic.

### 3.3 Do not call the current top-level render path twice until proven safe

The recovered `CGame::wkRender()` advances a layout animation and calls the task draw path. The recovered process draw manager performs draw traversal and then tail traversal. Calling the entire path twice risks:

- UI animation advancing twice;
- frame-finalization or tail callbacks running twice;
- effect queues being flushed twice;
- cleanup occurring between views;
- audio or event side effects;
- global renderer scratch being reused incorrectly.

The render agent must identify a lower, scene-only and HUD-render-only boundary.

### 3.4 Sequential rendering only

NW4R G3D uses global temporary work memory. Treat the renderer as non-reentrant.

```text
render view 0 completely
restore/prepare global render state
render view 1 completely
finalize/present once
```

Do not attempt concurrent render threads.

### 3.5 Player identity is not presentation identity

The global game leader/current player must remain authoritative and stable unless gameplay code explicitly requires otherwise.

Do not implement split-screen by globally changing the current-player actor around the whole frame.

Allowed scoped differences:

- camera subject;
- view rectangle and projection;
- culling frustum;
- HUD subject;
- HUD cursor and displayed target;
- visible action prompt;
- local screen-space effects;
- optional local audio emphasis.

Disallowed local-only differences:

- whether an Art executes;
- actor movement;
- target ownership in authoritative battle state;
- AI ownership;
- damage, cooldowns, buffs, aggro;
- RNG;
- quest or event state;
- loaded/active world objects.

### 3.6 Action prompts are gameplay, not decoration

Blossom Dance-style follow-up presses must update the actor's native action state once during simulation. Rendering the prompt can differ by viewport, but accepting the input cannot happen only inside a HUD draw pass.

### 3.7 Extra memory is opt-in and isolated

Dolphin may provide expanded MEM2, but the original Xenoblade heaps must remain unchanged initially. New co-op allocations go to a dedicated expanded-memory heap until the original allocator and every affected pool are understood.

---

## 17. Feature-driven decompilation and byte-matching loop

Every missing function should be approached as a bounded question, not “decompile the renderer.”

### 17.1 Standard loop

```text
select one function/question
-> import symbols and types
-> export assembly, relocations, decompiler output, callers, callees (**reference only — not shipped source**)
-> produce conservative candidate **high-level C or C++** (semantics and types, not register-level transcription)
-> compile with the pinned original toolchain
-> compare with objdiff
-> classify mismatches
-> revise source shape/types/control flow in readable C/C++ only
-> repeat toward exact code and relocation match
-> instrument original binary in Dolphin
-> verify semantic behavior
-> document evidence and hook safety
```

### 17.2 Match statuses

| Status | Meaning |
|---|---|
| `NOT_STARTED` | No reconstruction attempt. |
| `COMPILES` | Candidate compiles; no correctness claim. |
| `STRUCTURAL` | Control-flow and calls broadly align. |
| `HIGH_MATCH` | Most instructions/relocations match (≥ ~70% fuzzy). |
| `CODE_MATCH` | Instruction bytes largely match (≥ ~95% fuzzy); data/relocations may remain. |
| `EQUIVALENT_MATCH` | Fuzzy ≥ **50%** and the register-renaming witness (`cycle`/`batch-cycle`, runs by default) certifies equivalence, plus split-size fit. (Z3/SMT probe is disabled — see No-SMT policy below.) |
| `FULL_MATCH` | Code, relocations, expected stack/function size match (100% static), and split-size fit. |
| `BEHAVIOR_VERIFIED` | Runtime tests confirm the interpretation. |

A byte match proves faithful code generation, not the semantic name of every unknown field or hook safety.

**Current project policy:** the required acceptance bar for every decompilation target is **`EQUIVALENT_MATCH`** or **`FULL_MATCH`** — both are equal-tier outcomes (prefer `FULL_MATCH` when reachable: a 100% static match is cheaper to certify and needs no semantic proof). `EQUIVALENT_MATCH` guarantees semantic correctness via the **register-renaming witness**, which runs inside `cycle`/`batch-cycle` by default; `FULL_MATCH` guarantees byte-level identity and receives an automatic `full-instruction-match` certificate without a solver. Intermediate statuses remain useful for logging progress. `coop run cycle` exits non-zero until `EQUIVALENT_MATCH` or `FULL_MATCH` (plus split-size fit).

**No-SMT policy (hard):** the full Z3 SMT probe is **disabled** in this repo. Do not run `--smt`/`--linked`, or a plain `ppc_equivalence`-driven `run.py diff` (it defaults the probe on), for acceptance evidence. The only equivalence path is the **register-renaming witness** inside `cycle`/`batch-cycle`. A function the witness cannot certify (scheduling/immediate/instruction-selection diffs, any `psq_*`, or any `bl` call) must reach `FULL_MATCH` or stay recorded as a near-miss — the SMT probe is never the answer.

### 17.2.1 Behaviour comparison (static + optional PPC)

When static objdiff match is below 100%, optional evidence lives in `tools/test/compare_behaviour/`:

```bash
python3 tools/coop/run.py behaviour audit              # size budget for registered tests
python3 tools/coop/run.py behaviour compare <test-id>  # static + ppc if present
python3 tools/coop/run.py behaviour ppc <test-id>      # headless Dolphin when ppc_source set
```

Host dual-oracle `host/*.cpp` tests were **removed** (they were mostly tautological). Do not add them back. Prefer continuing toward `EQUIVALENT_MATCH` / `FULL_MATCH` / §17.6, or a real PPC harness when the unit links. See `tools/test/compare_behaviour/README.md`.

### 17.2.2 Split object size (`.text` budget)

Each translation unit occupies a fixed `.text` range in `config/<region>/splits.txt` (`start`/`end`). Before promoting a unit to `Matching` in `configure.py` or claiming `EQUIVALENT_MATCH` / `FULL_MATCH`, verify the decompiled object’s **`.text` section** does not exceed that budget:

```bash
python3 tools/coop/run.py size <unit>
python3 tools/coop/run.py size --all
```

`coop run diff`, `cycle`, and `behaviour compare` print a `size:` line but do **not** fail on a unit split-size overrun — function acceptance is per-function (user policy 2026-08). Unit split size gates only **unit promotion**: `run.py size` must PASS before the unit's configure.py `NonMatching` → `Matching` flip (TU-final `sizeOk`). Behaviour tests may pass while size fails — semantics can be correct before codegen fits the retail slot. Implementation: `tools/coop/lib/object_size.py`.

**Source language:** matched functions must be expressed as **high-level C or C++** in `src/**` and `libs/**`, except where the isolated Gekko paired-single backend exception in §17.6 applies. Express recovered semantics — fields, locals, control flow, and normal function calls — rather than register-level or stack-level implementation detail. Use asm/disassembly only to recover semantics for readable C++ outside that exception.

See **§17.6** for narrow, logged exceptions when C++ and decomp.me cannot close the last instruction(s).

### 17.6 Policy exceptions (closing the gap to FULL_MATCH or EQUIVALENT_MATCH)

Use only after normal C++ and decomp.me fail, and **log every use** in `docs/evidence/decomp/attempts.jsonl` with `"policy_exception": true` and a one-line justification.

| Exception | Allowed when | Requirements |
|-----------|----------------|--------------|
| **MWCC PPC intrinsics** | Opcode selection (`slwi` vs `rlwinm`, bitfield inserts) | Use `DECOMP_PPC_*` macros from `include/decomp.h` (same family as SDK `__rlwimi` / `__rlwinm`). Document in `MWCC_CASES.md` if a new pattern is reusable. |
| **Isolated MWCC Gekko paired-single backend** | A named Wii/MWCC target contains retail `psq_*`, `ps_*`, or related paired-single operations that cannot be expressed through approved high-level MWCC C++ (`__vec2x32float__`, scalar builtins, and normal C++), after the ordinary C++ path has been exhausted | Keep the implementation in a designated C/C++ PS backend file or `.inl` included by the owning TU, or in an explicitly marked PS region. Guard it so non-MWCC/PC builds select a complete high-level fallback. `ASM`/`asm void` may be used only for the documented PS kernel and its minimal memory/branch support; do not hand-write a prologue/epilogue or unrelated control-flow, GPR, or stack choreography. Record the opcode set, target, guard, fallback, and validation evidence. |
| **Goto gate chains** | CSplitFrame / multi-exit guards (see `setSplitLine` 100%) | Gotos for control-flow gates are OK; not for prologue spill ordering alone. |
| **Wii boot-entry vectors (MetroTRK `InitMetroTRK*`)** | A named Wii/MWCC target is a hardware boot-entry vector entered with a non-standard ABI (no valid stack frame, hardware ID in `r5`), so MWCC's mandatory frame prologue cannot reproduce the retail body, after the ordinary C++ path has been exhausted and the target is otherwise blocked | Guard `asm void` + `nofralloc` to the MWCC build; provide a complete readable C fallback for non-MWCC/PC builds. Transcription is limited to the named boot-vector body (GPR save/restore via `stmw`/`lmw`, MSR/SRR1 + IABR/DABR SPR setup, fixed debug-stack switch, comm-table init, tail `b TRK_main`). No unrelated control-flow or prologue/epilogue hacks. Log every use with `"policy_exception": true`. |
| **Relocation name drift** | `functionRelocDiffs=data_value` already compares values; TU-local `@N` vs retail `lbl_eu_*` at same offset | Prefer `extern "C" lbl_eu_*` when it does not regress codegen. Symbol names do not affect EQUIVALENT_MATCH; `objdiff.json` `symbol_mappings` does not affect CLI reports (objdiff #279). Do not post-process objects to rename symbols. |

#### Isolated Gekko paired-single backend requirements

This is a narrow hardware-backend exception, not a general assembly allowance:

- **Scope:** only Gekko/Broadway paired-single instructions and the minimum loads, stores, scalar operations, comparisons, and branches needed to implement that named PS kernel. Typical examples include `psq_l`/`psq_st`, `ps_merge*`, `ps_mul`, `ps_madd`/`ps_msub`/`ps_nmsub`, `ps_muls*`, `ps_sum*`, `ps_cmp*`, `ps_abs`, and `fres`.
- **Isolation:** place the code in a designated PS backend `.inl`/C/C++ file included by the owning translation unit, or in a clearly marked backend region. Do not create a general-purpose asm utility or a standalone `.s` implementation. Keep the symbol and split ownership in the normal C/C++ TU unless a target record explicitly approves another layout.
- **Platform split:** the PS path must be excluded from host/non-MWCC parsing and execution. Every PS backend must have a complete readable scalar/high-level fallback for the PC port; the fallback is validated for numerical/gameplay equivalence, not paired-single bit identity.
- **Assembly discipline:** `ASM`/`asm void` is allowed only inside the isolated PS backend and only for the documented kernel. No hand-written prologue/epilogue, fake stack frame, numbered GPR binding, register-color tuning outside the PS operands, binary patching, or unrelated retail control-flow transcription.
- **Evidence:** before acceptance, record why high-level C++/MWCC builtins were insufficient, the exact target and opcode set, the compile guard, the fallback, static/size results, and PPC plus host numerical/gameplay validation. Log each use with `"policy_exception": true`.

#### Wii boot-entry vector exception requirements

This is a narrow hardware-ABI exception, not a general assembly allowance:

- **Scope:** only the named MetroTRK boot-entry functions (`InitMetroTRK`, `InitMetroTRK_BBA`) whose retail bodies cannot be produced by MWCC's standard function ABI (entry with no valid stack frame and no return address; hardware ID passed in `r5`; hand-rolled GPR save/restore via `stmw`/`lmw`; `mfmsr`/`mtmsr`/`mtsrr1` and `mtiabr`/`mtdabr` SPR manipulation; fixed debug-stack switch; tail `b TRK_main`).
- **Isolation:** keep the `asm void` bodies in the owning TU (`MetroTRK/dolphin_trk.c`) inside a clearly marked boot-vector region guarded to MWCC builds. Do not create a standalone `.s` unit or a general-purpose asm utility.
- **Platform split:** non-MWCC / PC builds must select a complete readable C fallback (TRK is dead code on PC; the fallback keeps the port linkable and functionally sane — comm-table init then `TRK_main`).
- **Assembly discipline:** `asm void` + `nofralloc` only for the named boot vectors; transcribe only the retail body. No unrelated control-flow, fake frames, or prologue/epilogue hacks beyond the transcribed body.
- **Evidence:** record why C++ failed (MWCC unconditional prologue vs non-standard entry ABI), the exact targets and opcode set, the compile guard, the fallback, static/size results, and log each use with `"policy_exception": true`.

**Not approved outside the isolated PS-backend exception:** `register rN`, fake `sp[]` buffers, arbitrary inline `asm { }`, arbitrary **`asm void` / whole-function asm bodies**, standalone `.s` units, or transcribed retail asm blocks. Matching targets must remain **high-level C/C++** except for the narrowly defined PS kernel above. **Object-file post-processing** to patch instruction bytes, reorder data pools, rename symbols, or manipulate section sizes (`postprocess_reloc_names.py` or equivalent) is not approved — EQUIVALENT_MATCH with SMT proof is the acceptance bar; do not chase byte-identity through binary patching.

**Escalation:** frame-size / caller-stack ABI gaps (`setCurrent`, `setRect` prologue) may combine intrinsics, leaf helpers, and decomp.me — not asm of any kind outside the isolated PS-backend exception.

#### Original-SDK assembly retained as-is (`libs/PowerPC_EABI_Support`)

The Metrowerks runtime and the MetroTRK debugger shipped as assembly in the original SDK, and retail contains that assembly verbatim. The `asm void` / `.s` content below is **original SDK source transcribed for matching — not decompiled output** — and is intentionally kept as-is (this is a documentation note, not a new exception; pre-existing content needs no `policy_exception` log, but do not re-transcribe or convert it):

- `Runtime/runtime.c` — compiler runtime helpers (`__save_fpr`/`__restore_fpr`, `__save_gpr`/`__restore_gpr`, `__div2u`/`__div2i`/`__mod2u`/`__mod2i`/`__shl2i`/`__shr2u`/`__shr2i`, `__cvt_*` conversions)
- `Runtime/ptmf.c`, `Runtime/__mem.c`, `Runtime/Gecko_ExceptionPPC.cp` — pointer-to-member thunks, memory kernels, exception-handling glue
- `MetroTRK/flush_cache.c` (`TRK_flush_cache`), `MetroTRK/mpc_7xx_603e.c`, `MetroTRK/targimpl.c` (interrupt handler, MSR helpers), `MetroTRK/dolphin_trk.c` (boot vectors, see the Wii boot-entry exception above), `MetroTRK/dolphin_trk_glue.c` (context save/restore, `TRKLoadContext`)
- `MetroTRK/__exception.s`, `MetroTRK/targsupp.s` — standalone SDK assembly units (interrupt vector table, target support)

Rules: do not run clang-format on these files (it mangles MWCC `asm` text and changes generated code); keep the `asm void` bodies byte-stable; do not "fix" their style. Any **new** assembly added for matching remains subject to the exceptions table above.

### 17.3 Common mismatch categories

- signed versus unsigned comparisons;
- wrong field type/width;
- branch shape;
- switch lowering;
- local-variable lifetime and register allocation;
- expression order;
- inlining;
- virtual or adjusted-this call;
- stack layout;
- constant-pool use;
- missing pragma/compiler context;
- incorrect relocation target.

### 17.4 Agent attempt record

```json
{"function":"CMenuArtsSelect::Move","region":"us","attempt":17,"status":"HIGH_MATCH","instruction_match":94.2,"relocation_match":88.9,"hypothesis":"wrong signedness for selected slot","next_change":"change candidate slot to s16","runtime_test":"HUD-03"}
```

Store records as JSONL for automated review.

### 17.5 Large functions

For large functions under the current **`EQUIVALENT_MATCH` or `FULL_MATCH` policy**:

- match leaf helpers first, but do not mark the parent target complete until the parent function also reaches `EQUIVALENT_MATCH` or `FULL_MATCH`;
- divide by state-machine cases;
- identify exact call sites;
- use narrow reversible hooks only after the hooked leaf or enclosing function is at `EQUIVALENT_MATCH` or `FULL_MATCH`;
- decompose work into smaller symbols/units, but do not accept `STRUCTURAL` or `CODE_MATCH` as the final state for any target.

---

