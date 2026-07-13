# MWCC reference — reaching FULL_MATCH

Practical reference for reaching **`FULL_MATCH`** (100% instruction + relocation match) on Xenoblade Chronicles Wii using **high-level C/C++ only**. Retail assembly, Ghidra, and `build/us/asm/` are **reference only** — do not ship asm, `register rN`, fake stack buffers, or register-named parameters in `src/**` or `libs/**`.

**Policy sources:** `.cursor/skills/xenoblade-decomp/SKILL.md`, `PLAN.md` §17, `coop.json` (`match_policy: full`, `functionRelocDiffs=data_value`).

**Fork constraint:** many community projects use inline asm, `register` variables, or asm labels for the last 1–5%. This fork requires **high-level C/C++ only** — techniques marked ⚠️ asm/register are listed for **diagnosis**, not as approved fixes.

**Living document:** agents must **append new patterns and breakthroughs** to this file when they discover reusable MWCC matching knowledge. Do not leave discoveries only in `docs/evidence/decomp/attempts.jsonl` or chat. See **Contributing patterns** below.

---

## How objdiff matching works here

objdiff compares **relocatable `.o` files**: the **target** (split from retail `main.dol`) vs the **base** (built from your C++ source).

| Layer | What must match | Tooling |
|-------|------------------|---------|
| Instructions | Every opcode/operand in the function body | objdiff function diff |
| Relocations | Symbol names, SDA bases, ADDR16/EMB_SDA21 entries | objdiff with `functionRelocDiffs=data_value` |
| TU linkage | Whole object 100% before `Object(Matching, …)` | objdiff object view + `ninja` |

**Statuses** (`PLAN.md` §17.2; thresholds in `tools/coop/lib/objdiff_report.py`):

| Status | Symbol-scoped rule |
|--------|-------------------|
| `FULL_MATCH` | `function_match >= 100.0` |
| `CODE_MATCH` | `>= 95.0` (instructions; relocs may still differ) |
| `HIGH_MATCH` | `>= 70.0` |

Only **`FULL_MATCH`** closes a target. Treat **`coop run cycle` PASS** + `docs/evidence/decomp/attempts.jsonl` as ground truth over stale `TASKS.md` checkboxes.

**Important distinction:** instructions can match in the linked ELF while objdiff still reports &lt;100% because **relocation records** differ (e.g. `spInstance__9CDeviceGX` vs `lbl_eu_806656A0@sda21`, or TU-local `@2856` vs `lbl_eu_8066A1D8`). See **§11** when `extern "C" lbl_eu_*` is not enough.

### Commands

```bash
python tools/coop/run.py build monolib/src/core/CView
python tools/coop/run.py diff monolib/src/core/CView --symbol setCurrent__5CViewFv
python tools/coop/run.py cycle view-set-current --hypothesis "..." --next-change "..."
```

| Generic workflow step | This repo |
|-----------------------|-----------|
| objdiff function diff | `python tools/coop/run.py diff <unit> --symbol <sym>` |
| Strict relocs | `coop.json` → `functionRelocDiffs=data_value` |
| Context for scratch | `python tools/coop/run.py ctx <path>` |
| Match policy | `FULL_MATCH` only; log in `docs/evidence/decomp/attempts.jsonl` |
| Behaviour below 100% | `python tools/coop/run.py behaviour compare <id>` + `behaviour audit` — see `tools/test/compare_behaviour/README.md` |
| No asm matching | See `SKILL.md` “Low-level techniques — do not use” |

`coop.json` passes `--config functionRelocDiffs=data_value` to objdiff — stricter than upstream default (`functionRelocDiffs=none`).

---

## Behaviour comparison (non-`FULL_MATCH`)

When static objdiff match is **below 100%**, byte identity is not yet proven. Use **host dual-oracle tests** (`tools/test/compare_behaviour/`):

```bash
python tools/coop/run.py behaviour audit
python tools/coop/run.py behaviour compare view-rect-data-clamp
```

Each test file defines `retail_*` (oracle from asm) and `decomp_*` (from source) with the same inputs. Minimum scenario counts:

| Match % | Min `run_scenario` calls |
|---------|--------------------------|
| 95–99.9 | 8 |
| 90–94.9 | 12 |
| 80–89.9 | 20 |
| < 80 | 30 |

Do not treat semantics as settled until `behaviour audit` passes. Log `runtime_test: behaviour:<test-id>` in `attempts.jsonl`.

---

## Contributing patterns

This reference is maintained by agents during decompilation. **Update it in the same session** when you learn something reusable.

| Discovery type | Add under |
|----------------|-----------|
| Proven high-level fix that reached or improved match % | **Patterns that work in this repo** — new numbered subsection or extend an existing one |
| Symptom → cause → fix (even if not 100% yet) | **Pitfalls and failure modes** — repo-specific table |
| Compiler flag, pragma, or codegen behavior | **MWCC compiler behavior** or **decomp.me workflow** |
| Confirmed limit under high-level-only policy | **When FULL_MATCH may be unrealistic** |

**Entry format** (keep each addition short):

```markdown
### N. Short title — `Class::method` (`path/to/File.cpp`)

- **Symptom:** what objdiff showed (e.g. missing `mr r3, r30`, reloc `lbl_eu_…`)
- **Fix:** the high-level C++ change
- **Result:** match % or `FULL_MATCH`
```

Also log the attempt in `docs/evidence/decomp/attempts.jsonl`. The reference is for **reusable** knowledge; one-off typos belong only in the log.

---

## MWCC compiler behavior

MWCC for PowerPC (`mwcceppc`) targets the **EABI** with **two small-data areas**:

| Register | Section | Typical use |
|----------|---------|-------------|
| **r13** | `.sdata` / `.sbss` | Mutable small globals |
| **r2** | `.sdata2` / `.sbss2` | Read-only small data, float constants |

Access is **base + 16-bit offset** (`lwz r3, lbl@sda21(r13)`). Relocations must name the same symbol retail used.

### Optimization pipeline (relevant passes)

From the [CodeWarrior Build Tools Reference](https://www.nxp.com/docs/en/reference-manual/CWMCUPABTR.pdf) and community practice:

- **Interprocedural analysis (IPA)** — `-ipa file` / `#pragma ipa file` inlines across translation units; changes call boundaries and register pressure.
- **Dead store elimination (DSE)** — removes stores whose values are never read → missing `stw` in match.
- **Copy propagation / coalescing** — merges `mr rA, rB` when safe → missing **dead** `mr` moves (common 96–99% blocker).
- **Register allocation** — Chaitin-style graph coloring; prefers r3–r12, then spills to r31…r28, then stack ([mwcc-debugger](https://github.com/cadmic/mwcc-debugger)).

### Codegen preferences

- **`-O4,p`** vs **`-O4,s`** — speed vs size; changes inlining and branch layout.
- **`-inline auto`** / **`-inline on`** — aggressive inlining; wrong helper → wrong entire function shape.
- **`-use_lmw_stmw on`** — paired load/store multiple in prologue/epilogue.
- **`-fp_contract on`** — may fuse multiply-add; use `#pragma fp_contract off` on sensitive float TUs if retail differed.
- **`-func_align 4`** — function entry alignment; missing on game code → systematic offset drift.

### `configure.py` flags in this repo

| Unit | Flags | Notes |
|------|-------|-------|
| `kyoshin/CGame.cpp` | `-O4,s`, `-func_align 4` | Space optimization vs `-O4,p` base |
| `monolib/*` (game) | `cflags_game`: `-ipa file`, `-inline auto`, `-use_lmw_stmw on`, `-RTTI on`, `-Cpp_exceptions on` | IPA affects inlining across TUs |
| Base | `-O4,p`, `-fp_contract on`, `-enc SJIS` | Wrong `-O` or missing `-func_align 4` → systematic drift |

**Global (`cflags_base`):**

```python
"-O4,p", "-inline auto", '-pragma "cats off"', '-pragma "warn_notinlined off"',
"-fp_contract on", "-enc SJIS", "-Cpp_exceptions off"  # base; game libs override
```

**Per-object `extra_cflags`:**

```python
Object(MatchingFor("jp"), "kyoshin/CGame.cpp", extra_cflags=["-O4,s", "-func_align 4"]),
```

**Pragmas seen in tree** (use sparingly, match retail TU):

| Pragma | Where | Effect |
|--------|-------|--------|
| `#pragma ipa file` | `cflags_game`, some nw4r | File-level IPA — inlines across file boundary |
| `#pragma optimization_level 0` | `NdevExi2A/DebuggerDriver.c` | Debug-only; not for game TUs |
| `#pragma unused(x)` | nw4r, SDK | Silences warnings; no codegen change |
| `#pragma fp_contract on/off` | per-file | Affects FMA contraction |
| `-pragma "dont_inline on"` | commented in `configure.py` debug | Emergency: stop inlining |

**Matching flip** — only after **entire TU** is 100% in objdiff:

```python
Object(Matching, "monolib/src/core/CViewRectDataCore.cpp"),  # was NonMatching
```

---

## C/C++ → MWCC translation patterns

| C/C++ idiom | Typical MWCC output | Matching tip |
|-------------|---------------------|--------------|
| `if (a) x=1; else x=2;` | Compare + branch tree | If wrong, swap blocks or invert condition ([TWW guide](https://github.com/zeldaret/tww/blob/main/docs/decompiling.md)) |
| `return cond ? a : b;` | Different branch than if/else | Toggle ternary ↔ if/else |
| `static inline` / header inline | May disappear entirely | Match retail inline boundaries; debug maps help in other projects |
| `extern "C" void f();` | `bl f` with C symbol | Required for correct `bl` reloc |
| `Class::method()` C++ call | `bl` with mangled name | Wrong mangling → reloc miss |
| `float` literal in IPA file | Floats land in `.sdata2` order | ⚠️ `.sdata2` order vs other data — see [ppcdis TOOLS.md](https://github.com/SeekyCt/ppcdis/blob/main/TOOLS.md) |
| Global `static` in TU | May use SDA or far abs | Retail name (`lbl_eu_*`) may differ from decomp name |
| Member access `obj->field` | `lwz` with struct offset | Wrong layout in header → everything wrong |
| Virtual call | `lwz` vtable + `mtctr`/`bctrl` | Check vtable order and `this` adjustment |
| `Fv` method, arg in r4 | Implicit register param | Use proper C++ ref/param type, not `register u32* r4` (fork policy) |
| Named helper `static void helper()` | Separate `bl` | Splitting changes frame; inline by making helper `static` in same flow or match call count |
| `volatile` | Extra loads/stores | Fork discourages fake volatiles for stack; real MMIO only |
| `#pragma unused(x)` | No codegen change | Silence warnings only |

### Fv ABI note (MWCC C++)

Metrowerks often passes **extra arguments in registers** even on legacy `…Fv` symbol names when the retail compiler re-used parameter slots. Recovery path here: express as **normal C++ parameters** (`const ml::CPnt16&`, etc.) — proven on `func_804592F0` / `func_80459384` at 100%.

**`symbols.txt` must use the mangling MWCC actually emits** (e.g. `…FRCQ22ml6CPnt16`), not shortened `…Fv` placeholders — otherwise per-unit objdiff can pass while **ELF link fails** (retail split objects reference `Fv`, recompiled objects export full mangling).

---

## Patterns that work in this repo

### 1. Simple control flow + real struct members → easy FULL_MATCH

**`CView::setDisp`** (`libs/monolib/src/core/CView.cpp`) — reference pattern:

- Normal `if`/`else` on `bool` parameters
- Bit twiddle on `unk278`
- One `extern "C"` call chain through `CViewRoot`
- No stack temporaries mirroring caller frames
- **Result:** 100% at high-level C++ (unchanged through asm→C++ rewrite)

**`CView::setDefaultFrameColor`**, **`detachRenderWork`**, **`CGame::wkUpdate` / `wkRender` / `setViewRect`** — same class: thin wrappers, correct types, upstream-matched.

### 2. `extern "C"` on `bl` targets with retail mangling

MWCC emits `bl` to **exact linker symbols**. C++-mangled names on callees cause wrong relocs or dual symbols.

```cpp
extern "C" {
    CViewRoot* getInstance__9CViewRootFv();
    void invalidCurrent__9CViewRootFP5CView(CView* view);
    GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
}
```

Used in `CView.cpp` (split-line helpers) and `CViewRectDataCore.cpp` (`getRenderModeObj__9CDeviceVIFv`).

### 3. Retail SDA global names for reloc match

**`CDeviceGX::viBeginFrame`** (`libs/monolib/src/device/CDeviceGX.cpp`):

```cpp
extern "C" CDeviceGX* lbl_eu_806656A0;

void CDeviceGX::viBeginFrame() {
    if (lbl_eu_806656A0->mDevicesInitialized == 1) { … }
}
```

Retail uses `lbl_eu_806656A0@sda21`, not `spInstance__9CDeviceGX`. High-level field access (`->mDevicesInitialized`) is fine; the **global declaration name** must match retail for `FULL_MATCH`.

**`CDeviceGX::drawFrame`** — same `lbl_eu_806656A0` for all `spInstance` field stores/loads; else-branch stopwatch uses `lbl_eu_80663754` (retail sdata slot for `someString`), not `someString__9CDeviceGX`:

```cpp
extern "C" {
    CDeviceGX* lbl_eu_806656A0;
    const char* lbl_eu_80663754;
}

void CDeviceGX::drawFrame() {
    if (lbl_eu_806656A0->mDevicesInitialized == 1) { … }
    else { CStopwatchUtil::entry(lbl_eu_80663754); }
}
```

Compare `mDevicesInitialized` with `== 1` and `unk274` with `== 0` so MWCC emits `cmpwi` / `lbz` like retail.

### 3b. `vmPluginRegist` plugin tables — retail `lbl_eu_*` string + data symbols

**`pluginMathRegist`**, **`pluginDebRegist`**, **`pluginPadRegist`**, etc. (`src/kyoshin/plugin/plugin*.cpp`) — split units contain **`.text` only**; plugin name strings and `PluginFuncData` tables live elsewhere in the DOL. Retail `plugin*Regist` loads `lbl_eu_<rodata>` and `lbl_eu_<data>` via `@ha`/`@l` relocs, not `@stringBase0` + local `sPlugin*Funcs`.

```cpp
extern "C" {
    extern char lbl_eu_804FB050[];      // "math"
    extern PluginFuncData lbl_eu_805261B8[];
}

void pluginMathRegist() {
    vmPluginRegist(lbl_eu_804FB050, lbl_eu_805261B8);
}
```

Remove the TU-local `static PluginFuncData sPlugin…[]` array so the compiled `.o` does not emit extra `.data`/`.rodata` sections. Same pattern for `lbl_eu_80663E28` (`pad_enable` flags), `lbl_eu_80665588` (`CRect16` static init), and sdata2 float pools (`lbl_eu_80669008` / `lbl_eu_8066900C` in `pluginVoice.cpp`).

### 3c. `nw4r::ut::IOStream` RTTI — `lbl_eu_80665540` + out-of-line align stubs

**`ut_IOStream.cpp`** / **`snd_McsSoundArchive.cpp`** — retail puts `GetOffsetAlign` / `GetSizeAlign` / `GetBufferAlign` (`li r3,1; blr`) in the **McsSoundArchive** split (`.text` only, 24 bytes), not in `ut_IOStream` (44 bytes). Declare the three align methods in `ut_IOStream.h` without inline bodies; define them in `snd_McsSoundArchive.cpp`.

For `__sinit_\ut_IOStream_cpp`, retail stores `0` to **`lbl_eu_80665540@sda21`** — not `typeInfo__Q34nw4r2ut8IOStream`. Do **not** use `NW4R_UT_RTTI_DEF_BASE(IOStream)` (that emits a separate `typeInfo` symbol and a constructor helper). Instead:

```cpp
extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665540(NULL);
```

Header: `GetRuntimeTypeInfo()` returns `&lbl_eu_80665540` (declare `extern "C"` in the header). **`ut_FileStream.cpp`**: `extern "C" lbl_eu_80665548(&lbl_eu_80665540)` instead of `NW4R_UT_RTTI_DEF_DERIVED`. The **split unit is `.text` only** (24 bytes): keep only `Cancel`, `CancelAsync`, and RTTI init in `ut_FileStream.cpp`; move `FilePosition::Skip` / `Append` / `Seek` to a separate non-matching TU (e.g. `ut_FilePosition.cpp`).

### 3d. `ml::MTRand::getInstance` — `@GUARD@` vs `lbl_eu_80665580`

US retail `getInstance` checks **`lbl_eu_80665580@sda21`** (not `@GUARD@getInstance__Q22ml6MTRandFv@instance`). MWCC emits `@GUARD@` when using `static MTRand instance` (C++ object with ctor).

**FIXED (99.7% → 99.8%, 142/142 insn byte-identical, guard reloc matches retail):** remove the non-trivial `MTRand()` constructor from `MTRand.hpp` (confirm via grep that `getInstance()` is the *only* place the class is constructed — true here). With no user ctor, `MTRand` is trivially default-constructible, so `static MTRand instance;` needs **no compiler-synthesized guard at all**. Do the one-time init by hand, guarded by the retail global, declared as **`s8` (signed char), not `u8`**:

```cpp
extern "C" s8 lbl_eu_80665580;

MTRand* MTRand::getInstance() {
    static MTRand instance;
    if (!lbl_eu_80665580) {
        instance.srand(0x012BD6AA);
        instance.pNext = instance.state;
        lbl_eu_80665580 = 1;
    }
    return &instance;
}
```

- `s8` reproduces retail's `lbz r0,guard@sda21; extsb. r0,r0; bne …` (record-bit compare-and-branch on the sign-extended byte). `u8` compiles the same `!x` test to `lbz`+`cmpwi r0,0` — a 1-insn diff, only 99.3%.
- Both `R_PPC_EMB_SDA21` relocs (offsets `0x8` and `0x150`) now target `lbl_eu_80665580`, matching retail exactly.
- `srand__Q22ml6MTRandFUl` / `nextMt__Q22ml6MTRandFv` stay `FULL_MATCH` — removing the ctor from the header doesn't touch either function's body.

**Singleton suffix (FIXED via §11 postprocess):** base emits `@LOCAL@getInstance__Q22ml6MTRandFv@instance` (WEAK, `.bss`); target has `@LOCAL@…@instance_806561E0` (UND + decomp-toolkit address suffix on a `.text`-only split). Exact objcopy redefine in `postprocess_reloc_names.py` closes `getInstance` to **FULL_MATCH**. Not reachable from high-level source alone.

**Superseded guard reloc fix attempt (do not use):** `static volatile char instance[sizeof(MTRand)]` + `reinterpret_cast` removes `@GUARD@` and uses `lbl_eu_80665580`, but drops instruction match (~81%) because retail inlines the `srand` loop into `getInstance` (0x238 bytes) instead of calling `srand` / using a C++ ctor guard. The ctor-removal + manual `s8` guard approach above keeps the inlined-`srand`-loop shape while still avoiding `@GUARD@`.

**`randFloat` / `randFloat1`:** retail uses `0x43300000` int-to-`double` stack trick + `lbl_eu_8066A1D8` (`lfd`/`fsubs`) and `lbl_eu_8066A1D0` (`fmuls`); `randFloat1` also uses `lbl_eu_8066A1E0` (`fadds`).

**FIXED (FULL_MATCH):** use **extern-only** SDA pool symbols in `MTRand.cpp` (no TU-local `const f32` definitions — those duplicate `@2854` and block reloc rename). Keep `return (f32)rand32() * lbl_eu_8066A1D0` so MWCC inlines `rand32()`. Run `tools/postprocess_mtrand_object.py` after compile (wired in `coop run build/diff/cycle`). General pattern: **§11 Relocation name drift**.

- `patch_sdata2_magic`: MWCC emits `43300000 00000000` for the int-to-double subtract constant; patch low word to `80000000`.
- `rename_pool_symbols`: rename only `@` symbols **referenced in `.rela.text`** (e.g. `@2856` → `lbl_eu_8066A1D8`) via `.sdata2` content match — avoids renaming unrelated `@` pools at the same offset. Do **not** rely on `objdiff.json` `symbol_mappings` for reports (objdiff #279).

```cpp
extern "C" {
    extern const f64 lbl_eu_8066A1D8;
    extern const f32 lbl_eu_8066A1D0;
    extern const f32 lbl_eu_8066A1E0;
    s8 lbl_eu_80665580;
}

float MTRand::randFloat() {
    return (f32)rand32() * lbl_eu_8066A1D0;
}
```

- All **39/39** instruction words byte-identical; all three relocs match retail under `functionRelocDiffs=data_value`.
- `randFloat1` also reaches **FULL_MATCH** with the same TU/postprocess.
- **Do not** use explicit inlined twist + `lbl_eu_8066A1D8` subtract (~70%) or `DECOMP_FORCELITERAL` (spurious `blr` TU).

**Superseded (~89.6% / 99.7%):** TU-local `const f32 lbl_eu_8066A1D0/E0` fixed scheduling but left `@2857`/`@2854` relocs; `objdiff.json` `symbol_mappings` alone did not reach 100% under strict reloc diff.

**decomp.me:** `python tools/coop/run.py ctx libs/monolib/src/math/MTRand.cpp` — iterate `randFloat` / `getInstance` in scratch (move single function from Context → Source).

**`getInstance` bytes:** compiled `.text` for `getInstance` is **byte-identical** to retail (0x238). Guard relocs match via `s8 lbl_eu_80665580`. The remaining `@LOCAL@…@instance` vs `…@instance_806561E0` suffix is closed by `postprocess_reloc_names.py` exact rename → **FULL_MATCH** (decomp-toolkit address suffix; not reachable from source alone).

**US-only:** `wait_frame` (`pluginWait.cpp`) calls `CDeviceVI::isTvFormatPal()` and subtracts `0x1000` (NTSC) vs `0x1333` (PAL) from the VM work counter — branch order must be `if (!isTvFormatPal())` first to match retail codegen.

### 4. Proper C++ parameter types (Fv ABI quirks)

**`CViewRectDataCore::func_804592F0` / `func_80459384`** — retail passes rect/size in **r4**; symbols are `…FRCQ22ml6CPnt16` (not `…Fv`):

- High-level fix: `const ml::CPnt16&` parameters (or equivalent struct refs)
- **`func_80459384`:** 100% with `*(ml::CPnt16*)&unk4 = maxSize`
- **`func_804592F0`:** clamp semantics at **~88.5%** with comma-defer (see pitfall table); not yet 100% under high-level-only policy

**`func_80459270`** — four chained `getRenderModeObj()` calls; **`return this`** (`CViewRectDataCore*`) keeps `r3` live so MWCC emits retail's dead `mr r3, r30` after the `unk4` store → **100%** (mangling stays `…Fv`; retail leaves `this` in r3 at `blr`).

### 5. Semantics-first rewrites that preserve logic but lose frame shape

When retail reads **caller stack** (`sp+0xC`…), express the **same data** via struct members:

- **`CView::setCurrent`**: snapshot `unk1C8`, `unk1DC`, ring index into `mContextRingBase` slots — correct semantics, **74.5%** match (frame `-0x40`, spill interleaving differs from retail)

This is the preferred high-level approach even when match % drops.

### 6. String / literal pool forcing — `wkStandbyLogin`

`include/decomp.h` macros (disabled under `NONMATCHING` / `COMPAT_ANY`):

| Macro | Purpose |
|-------|---------|
| `DECOMP_FORCEACTIVE(module, …)` | Force rodata/string pool references |
| `DECOMP_FORCELITERAL(module, …)` | Force `.sdata2` float literal ordering |
| `DECOMP_DONT_INLINE` | `__attribute__((never_inline))` — split codegen boundary |

`src/kyoshin/CGame.cpp` uses these for `wkStandbyLogin` string pool relocs (`@stringBase0`, layout offsets). Works with `cflags_game` `-str reuse,pool,readonly`. Still **88.8%** high-level vs **98.9%** asm-era `CODE_MATCH` — remaining gap is frame/scheduling, not missing strings.

### 7. Struct layout before logic — `CView.hpp`

Fix offsets in `libs/monolib/include/monolib/core/CView.hpp` before tuning C++ shape:

- `unk1C8` → `CViewRectDataCore`
- `mContextRingBase` / `mContextRingWriteIndex` / `mContextRingCapacity` at `0x3EC` / `0x3F4` / `0x3F8`
- `unk45C` → `CSplitFrame*`

Wrong layout causes branch/frame divergence even when control flow looks right.

### 8. Local declaration order for regalloc

**`setSplitLine` (100%):** declare `hasView2` before `hasView1` so MWCC maps r31/r30 like retail.

**`getSplitLine` (100%):** load `splitFrame = unk45C` first; gate with goto chain; epilogue uses `if (hasView2 != 0) goto call; return 0;` / `getSplitLine_call:` so retail `beq`→`mr`/`bl`→`b`→`li` order matches (inverted `hasView2 == 0` goto regressed).

### 8b. Per-block-scoped temporaries when the SAME pair repeats with opposite regalloc order — `CView::wkUpdate`

**`wkUpdate` (100%):** the retail function has *two* independent "has view1/has view2" goto-chain gates (one before `apply()`, one before `wkSetEvent(EVT_NONE)`), each checking `getView1`/`getView2` on the same `unk45C` split frame. Naively reusing one pair of function-scope `hasView1`/`hasView2` locals for both gates forces MWCC to pick a *single* r30/r31 zero-init order for both — but retail uses the **opposite** `li` order in each gate (first gate: r30 then r31; second gate: r31 then r30). No single top-level declaration/assignment order for a shared pair satisfies both simultaneously (best achievable: 99.9%, one swapped `li` pair).

Fix: give the **first** gate its own block-scoped locals (`{ u32 hasView1a; u32 hasView2a; ... }`), leaving the second gate's `if (mChildren.empty() && ...)` block using the function-scope `hasView1`/`hasView2`. Each pair's zero-assignment order can then be tuned independently (first gate: `hasView2a = 0; hasView1a = 0;`; second gate: `hasView2 = 0; hasView1 = 0;`) to reach a full byte match. General lesson: when the *same logical pattern* repeats with a *different* regalloc order in retail, don't reuse one variable pair across both — scope a fresh pair to whichever occurrence needs the non-default order.

### 8c. Function-scope stack homes — `CView::setRect`

**`setRect` (100%):** declare `ml::CRect16 frameOffset`, `ml::CPnt16 stackPos`, `CWorkThread* parentSnap`, `CWorkThread* sourceParent` at function scope (not nested blocks). Split path: `stackPos` neg staging at `sp+0x10` after `getFrame2ViewOffset` at `sp+0xC`; interleave `parentSnap = mParent` between neg x/y. Normal path: reuse `stackPos` for `deltaPos` pack. Nested scopes or in-place `frameOffset.mPos` negation shifted stack slots (`sp+0x10` vs `sp+0xC`) and regressed match.

### 8c2. Direct extern SDA float in a compare — `CfPadTask::updateCfPadData`

**`updateCfPadData__Q22cf9CfPadTaskFPQ22cf9CfPadDataPC4CPad`** (`src/kyoshin/cf/CfPadTask.cpp`) — retail's two stick-deadzone compares (`ml::math::abs(...) < 0.5f`) both load the constant via `lfs f0, lbl_eu_80667EB0@sda21(r0)`. Writing the constant as a **local TU literal** (`static const float CFPAD_STICK_DEADZONE = 0.5f;` in `CfPadData.hpp`, matching a plain `0.5f` in source) compiles to **byte-identical instructions** (217/217, 0 diff) but MWCC pools it into a **TU-local anonymous `.sdata2` slot** (`@N`, renumbered per build) instead of the shared, project-wide-merged retail symbol — `CODE_MATCH` ~99.95% (2 reloc-name mismatches at the two `lfs` sites), not `FULL_MATCH`.

**Fix:** declare the retail symbol directly and use it verbatim in both compares — no helper, no header constant:

```cpp
extern "C" {
    extern const float lbl_eu_80667EB0;
}
...
if (ml::math::abs(r4->mPad.mLStickXRaw) < lbl_eu_80667EB0) { ... }
if (ml::math::abs(r4->mPad.mLStickYRaw) < lbl_eu_80667EB0) { ... }
```

**Result:** `FULL_MATCH` — **0 byte diff** across all 868 bytes of the function; both `@sda21` relocs now name-match `lbl_eu_80667EB0`, and the `fabs`/`frsp`/`fcmpo` schedule around the preceding struct-copy block is **unchanged** from the local-literal version.

**Note:** an earlier session reported this same direct-extern substitution regressing to ~98.1% (reordering the float schedule vs the pad struct copy). That regression did **not** reproduce in this build — the plain top-level `extern "C" const float` declaration (no `static`, no wrapper/helper, no `volatile`) used unmodified at both compare sites reached `FULL_MATCH` cleanly. If a future MWCC/objdiff version regresses this again, retry with the *exact* declaration shape above before falling back to the TU-local literal + policy-exception reloc alias.

### 8c2b. Direct extern SDA flag word — `CfPadTask::checkForControllerError`

**`checkForControllerError__Q22cf9CfPadTaskFb`** (`src/kyoshin/cf/CfPadTask.cpp`, 99.8% → **100%** `FULL_MATCH`). Retail tests co-op pad-enable flag bit 28 via `lwz r0, lbl_eu_80663E28@sda21(r0)` + `rlwinm. r0, r0, 0, 3, 3` (two call sites). Calling `CfGameManager::checkUnkFlag(28)` inlines a `sUnkFlags` load path with a different mangled reloc — ~99.8% `CODE_MATCH`, same frame size `0xB4`.

**Fix:** reuse the existing TU `extern "C" u32 lbl_eu_80663E28;` and test the bit inline (same pattern as `Move()` / `update()`):

```cpp
if (lbl_eu_80663E28 & (1u << 28)) { ... }
```

Keep the original `(timer && extension) || gc` gate structure; restructuring to mirror retail's fall-through branch layout (goto / extra bool) **regressed** to 61–70% match. No behaviour-test harness yet (function is small and pure on statics); semantics unchanged vs baseline.

### 8c4. Inlined ctor vtable relocs — `CfPadTask::create`

**`create__Q22cf9CfPadTaskFP8CProcess`** (`src/kyoshin/cf/CfPadTask.cpp`, **FULL_MATCH** via §11 postprocess). Retail has no separate `__ct__Q22cf9CfPadTaskFv`; `new (getWorkMem()) CfPadTask()` inlines placement `allocate` + `__ct__8CProcessFv` + manual MI vtable/`__ptmf_null` setup + `addCallback` + timer init + `Regist`. Instructions are **byte-identical** at `0xEC`.

**FIXED:** `postprocess_reloc_names.py` renames `__vt__23CTTask<Q22cf9CfPadTask>` → `lbl_eu_80533D08`, `__vt__Q22cf9CfPadTask` → `lbl_eu_80533C90`, and the inlined-ctor `0.0f` pool `@N` → `lbl_eu_80667EA8`. Prefer source `extern "C" const float lbl_eu_80667EA8` where MWCC already emits that name; keep postprocess for vtable labels that have no high-level equivalent.

**Superseded:** CViewRoot-style manual rewrite (`allocate` + pointer stores from `lbl_eu_*`) fixed one vtable name but **regressed** to 72–84% on `lis`/`addi`/`lwz` schedule.

| Offset | Retail | Decomp (placement new) |
|--------|--------|------------------------|
| +0x03a/+0x042 | `lbl_eu_80533D08` HA/LO | `__vt__23CTTask<Q22cf9CfPadTask>` |
| +0x04e/+0x056 | `lbl_eu_80533C90` HA/LO | `__vt__Q22cf9CfPadTask` |
| +0x0b4 (fixed) | `lbl_eu_80667EA8` | was anonymous `@pool` for `0.0f` |

**Safe fix (+0.1%):** in `CfPadTask::CfPadTask()` use `sInputDisableTimer = lbl_eu_80667EA8` / `sButtonDisableTimer = lbl_eu_80667EA8` with `extern "C" const float lbl_eu_80667EA8;` (same pattern as §8c2/§8c3).

**Vtable / float pool relocs:** see FIXED note above and §11 (`create` / `Move` / `checkForControllerError` / `updateCfPadData`).

### 8c5. Cross-TU vtable slot offset — `CProc::pssDetachView`

**`pssDetachView__5CProcFUl`** (`libs/monolib/src/core/CProc.cpp`, 99.97% → **100%** `FULL_MATCH`). Retail virtual dispatch for `view->detachRenderWork(this)` uses `lwz r12, 0xB4(r12)`; decomp emitted `0xB0` — one immediate byte at `+0x54`, relocs for `getView__9CViewRootFUl` already matched. Not an `lbl_eu_*` SDA issue; MWCC computed `detachRenderWork` one vtable slot too early in the `CProc` TU.

**Fix:** add a compile-time-only padding virtual before the existing entries in `CView.hpp`:

```cpp
virtual void CView_UnkVirtualFunc0();
virtual void CView_UnkVirtualFunc1();
virtual void detachRenderWork(CWorkThread* pThread);
```

No retail symbol for `UnkVirtualFunc0` — header-only slot to shift the offset MWCC uses when compiling other TUs. Verify with aligned prologue compare (`818c00b4` at `+0x54`). `CView::wkUpdate` remained **FULL_MATCH** after this change.

### 8c3. Reloc naming + unclosable reload gap — `CfPadTask::update`

**`update__Q22cf9CfPadTaskFv`** (`src/kyoshin/cf/CfPadTask.cpp`, 98.1% → 99.6% `CODE_MATCH`). Fixed several genuine logic bugs found by comparing retail vs decomp disassembly line-by-line:

- `mainPadChannel != -WPAD_CHAN_INVALID` → `!= WPAD_CHAN_INVALID` (spurious negation).
- `CfGameManager::setCurrentPadPtr(CfGameManager::getPad(i), 0)` → pass `i`, not `0`.
- `u32 padType = pad->mPadType;` → must read from the local copy `newPad.mPadType` (retail re-reads the copy, not the source pointer).
- `result` gating: `if (padType == sMainPadType || sMainPadType == PAD_TYPE_CORE || (padType == PAD_TYPE_CORE && CfGameManager::checkUnkFlag(28)))` → the `checkUnkFlag(28)` arm is inverted (`!CfGameManager::checkUnkFlag(28)`); `if (newPad.mConnected) result = 0;` → inverted, must be `if (!newPad.mConnected)`.
- `sMainPadIsGCController = true;` on the "became a GC-mapped controller" path → retail sets `false`.

**Reloc naming:** applied the same `extern "C" lbl_eu_*` pattern as `Move()`/`updateCfPadData()`:
- `CfGameManager::checkUnkFlag(21|24|28)` → direct `lbl_eu_80663E28 & (1u << N)` (avoids a `sUnkFlags` mangled-name reloc that retail doesn't have).
- Error message getters → `func_eu_802B1384/9C/B4/CC()`.
- `sInputDisableTimer`/`sButtonDisableTimer` decrement-to-zero block: `> 0` / `-= 1` / `<= 0` / `= 0` all rewritten against `extern "C" const float lbl_eu_80667EA8` (0.0f) and `lbl_eu_80667EB4` (1.0f) — **safe**, +0.1% with no frame-size change, confirmed via `objdump -r` that retail's `update()` references these two exact symbols at this call site (offsets `0x814`/`0x824`/`0x848`/`0x854` relative to the object file, i.e. `+0x70`/`+0x80`/`+0xa4`/`+0xb0` into the function).
- Right-stick deadzone divide (`(x - 0.3f) / 0.7f`, both X/Y, both branches): rewritten against `lbl_eu_80667EAC` (0.3f) / `lbl_eu_80667EB8` (0.7f) — **safe**, reloc-name only (retail hoists a single `f30`/`f31` load reused by all 4 call sites; decomp already did the same, so this is a pure rename, 0 instruction-byte change).

**Regression found and reverted:** additionally substituting `extern "C" const float` for *every* `0.0f`/`0.3f`/`0.7f`/`1.0f` literal in the function (the `newPad.m*Raw = 0` resets, both `CPadManager::setRightStickDeadzone(0.3f)` calls, and the deadzone-branch `>= 0` comparisons) grows the prologue from `stwu r1,-368(r1)` to `-384(r1)` (extra callee-saved float spilled) and reorders/renumbers every subsequent branch target — match **drops to 97.5%**. Root cause: too many simultaneous named-extern float live ranges raises register pressure across the whole function body, unlike the narrow, isolated substitutions above. **Do not** blanket-replace every float literal with an extern; only replace the literals retail's own relocations confirm are read at that exact call site (verify via `objdump -r` address ranges before committing to a substitution).

**Best baseline (99.6%):** `stickX`/`stickY` locals loaded once, `>= 0.0f` compare, negative arm `-(ml::math::abs(stickX) - lbl_eu_80667EAC)/lbl_eu_80667EB8`.

**Remaining gap (0.4%, unclosed):** decomp's negative-deadzone branches **omit** the `lfs f0, off(r1)` reload that retail emits immediately before `fabs f0,f0` (2 occurrences at function offsets `+0x604` / `+0x654`, 8 bytes total). Retail reloads from the `newPad` stack copy (`0x70`/`0x74`) even though `f0` is still live from the preceding `fcmpo`; MWCC elides the redundant reload when `ml::math::abs(stickX)` reuses the compare register. Decomp function size is `0x7C0` vs retail `0x7C8`. Tried and rejected this session: explicit `stickX = newPad.mRStickXRaw` in else (optimized away), member-compare / branch-scoped locals, comma/ternary reload, `std::memcpy` reload (340→458 byte diffs), empty/`DECOMP_DONT_INLINE` barrier helper (98.4%), single-instruction `asm { lfs f0, 0x70(r1) }` (regalloc drift to f21), full negative-path asm block (30% match). Prior rejects still stand: algebraic `(x+0.3)/0.7` (98.1%), `nw4r::math::FAbs` on member (99.2%), inverted branch (97.2%). Closing likely needs decomp.me on the negative stick branch or a §17.6 **two-instruction** `lfs` carve-out (user-approved) without disturbing the existing `fabs`/`frsp`/`fsubs`/`fneg`/`fdivs` schedule.

### 8b. Comma-defer compare — `func_804592F0` width clamp

Retail emits `cmpw unk8,widthLimit` then `subf maxHeight` then `ble` (CR from compare survives the intervening `subf`). A straight `if (unk8 > widthLimit)` computes `maxHeight` before the compare.

**Partial fix (~88.5%, up from ~86.9%):** stash the compare in a `BOOL`, finish `maxHeight` in the comma middle operand, branch on the saved flag:

```cpp
partialH = (s16)(unk2 - unkE);
if ((overW = unk8 > widthLimit, maxHeight = (s16)(partialH - unk12), overW)) {
    unk8 = widthLimit;
}
```

Same pattern on height clamp with `overH`. **Regresses** if you split `maxWidth` into chained `-=` (drops to ~81%) or use `volatile BOOL` (~40%). Remaining gap: maxWidth chain regalloc (retail keeps `r0`/`r8`) and tail `bgelr` shape.

**PPC behaviour (`min_s16_size`):** retail compares `unk8`/`unkA` against **32-bit** promoted limits (`(s32)unk4 - (s32)maxWidth`), not the truncated `s16 widthLimit`. Using `(s32)unk8 > (s32)unk4 - (s32)maxWidth` in the comma-defer compare fixes Dolphin harness divergence while keeping `s16` locals for stores (~83% static vs ~88% before).

---

## Pitfalls and failure modes

### General pitfalls

| Pitfall | Symptom in objdiff | High-level remedy |
|---------|-------------------|-------------------|
| Wrong global symbol name | Reloc mismatch at `lwz`/`lfs` | `extern "C"` retail label (`lbl_eu_806656A0`) |
| SDA vs far addressing | Different insn (`lwz@sda21` vs `lis`/`addi`) | Correct section/size of global; linker script |
| Regswap (live-range order) | Same ops, different registers | Reorder locals, split/combine expressions, add helper ([TWW regalloc guide](https://github.com/zeldaret/tww/blob/main/docs/regalloc.md)) |
| Missing dead `mr` | 1 insn, ~97–99% | Extra use of temp; **`return this`** if retail ends with `this` in r3; decomp.me; ⚠️ asm `mr` elsewhere |
| Wrong callee-save depth | `_savegpr_29` vs `_savegpr_28` | Shrink live ranges across large functions |
| Stack frame size | `stwu r1,-0xN` differs | Fewer spills or different helper boundaries |
| Inlined vs outlined | Extra/missing `bl` | IPA/inline pragmas; match retail inline usage |
| if/else inversion | Branch to wrong block | Swap per asm |
| Ternary vs branch | Extra `b` or `sel` | Toggle form |
| `switch` shape wrong | Jump table vs compare tree; case order differs | Match **asm** case order, not Ghidra; duplicate “useless” cases may be required ([TWW decompiling.md](https://github.com/zeldaret/tww/blob/main/docs/decompiling.md)) |
| Wrong float constant pool | `lfs` from wrong `.sdata2` slot | Data order / `-ipa file` interaction |
| C++ mangling on `bl` | Wrong or duplicate symbol | `extern "C"` declaration |
| `functionRelocDiffs=none` vs `data_value` | “100%” in lenient mode, fail in coop | This fork uses **strict** reloc diff |
| Ghidra r13 SDA | Misleading decompilation | Set SDA bases in Ghidra ([issue #325](https://github.com/NationalSecurityAgency/ghidra/issues/325)) |

### Repo-specific failures (from `docs/evidence/decomp/attempts.jsonl`)

| Symptom in objdiff | Likely cause | High-level fix to try |
|--------------------|--------------|------------------------|
| 99–99.9%, instructions look identical | Relocation name mismatch (`spInstance__…` vs `lbl_eu_…`, `@stringBase0`, `@N` float/double pool) | Prefer `extern "C"` retail symbol (§3 / §8c2). If MWCC still emits `@N` for an implicit pool and codegen must stay put: `objcopy --redefine-sym` via content+reloc match — **not** `objdiff.json` `symbol_mappings` (CLI-ignored, objdiff #279). See §11 / `tools/postprocess_mtrand_object.py` |
| `CODE_MATCH` ~97%, one missing `mr r3, rN` | MWCC dead-store / coalescing elided preserve move when `this`/r3 is dead | **`return this`** on non-void member if retail ends with `this` in r3; else reorder locals / extend temp live range; decomp.me; **do not** use `register r30` |
| Frame size wrong (`-0x30` vs `-0x28`, `-0x40` vs smaller) | Different spill count / callee-save set (`_savegpr_28` vs `_savegpr_29`) | Reduce live ranges: fewer locals in scope; split into `static` helper; match helper boundaries retail used |
| Prologue order differs (spills interleaved with loads) | High-level C++ schedules all spills up front | Interleave by splitting into helpers called mid-prologue (only if helpers themselves match) |
| `bl` target symbol wrong | C++ linkage on callee | `extern "C" void foo__1AFv();` at call site |
| Branch layout differs | Ghidra inverted if/else vs retail | Swap blocks / invert condition (see TWW decomp guide) |
| Ternary vs if/else codegen | MWCC optimizes differently | Swap ternary ↔ explicit if/else |
| Split path stack slots (`sp+0xC`, `sp+0x14`) | Retail reused stack for outgoing args | Prefer member temps; if stuck, retail may have used stack homes — high-level policy may cap match % |
| Virtual / adjusted-this call wrong | Wrong `this` adjustment or vtable index | Fix class layout in header; use explicit base cast only if type-correct |
| `pssCreateView` ~73.5% | Retail monolithic `-0xF0` frame inlines FixStr@sp+0x68, reslist slot push, `CProc_UnkStruct1` walks, `getFrame2ViewOffset`, `lbl_eu_8066A278`/`8066A280` fctiwz path; high-level `CView::create`+`pssAttachView`+`pssMakeClientRect` IPA-inlines to `-0xC0` with wrong stack slots | **`wkReplaceHasChild((s16)param3)`** for retail `extsh` before call; explicit monolithic rewrite regresses to ~60%; **`u8 framePad[0x30]`** regresses to ~72%; next: decomp.me on single function with `PssCreateViewStack` layout matching sp+0x48 rect / sp+0x68 name / sp+0xB0 spills |
| `pssSetFocus` ~91.8% | MWCC inlines `pssGetView(INVALID_WORK_ID)` to correct walk+type-check semantics but allocates walk to **r3** (`mr r3,r6`), clobbering **`this`**, and fetches first WORK_ID via **`lwz r3,8(r6)`** instead of retail's **3× `lwz` reload from `this+0x1C8`**; explicit manual walk / `CProc_UnkStruct1` / `proc=this` regress to 68–80% | Keep thin `pssSetFocus` + inlined `pssGetView`; tune regalloc so walk uses **r4**, head **r6**, count **r5**, preserve **r3=this** through reload — decomp.me candidate |
| `setRect` ~84–91% | Frame `-0x40`, `getFrame2ViewOffset` at wrong `sp` slot | Function-scope `stackPos`/`frameOffset`; neg via `stackPos` not in-place `frameOffset.mPos`; parentSnap between neg x/y |
| `setRect` ~91% | Nested block locals → `-0x40` frame; function-scope `frameOffset`+`stackPos` → `-0x30` | Declare **`ml::CRect16 frameOffset` + `ml::CPnt16 stackPos` at function scope** (not per-branch); reuse across split/normal via `goto setRect_tail`; **remaining gap:** MWCC homes frameOffset at `sp+0x10` vs retail `sp+0xC`, parent gate uses `bge` chain not retail `blt` ladder |
| `func_804592F0` ~87% | Retail interleaves `cmpw` → `subf` (maxHeight) → `ble` while reusing r0/r5; high-level `if (unk8 > limit)` schedules compare after maxHeight | Semantics correct (`CPnt16&`, `unk6 - (unk2-unkE-unk12)`); **comma-defer** `if ((overW = unk8 > widthLimit, maxHeight = partialH - unk12, overW))` reaches **~88.5%**; remaining gap is maxWidth regalloc (r0/r8) + BOOL spill extsh + tail `bgelr` vs `bge` |
| Decomp `.text` exceeds split budget | Extra instructions / frame growth in a translation unit | `python tools/coop/run.py size <unit>` — budget from `config/<region>/splits.txt`; behaviour tests may still pass; blocks `FULL_MATCH` and `Matching` promotion until `.text` fits |
| `wkStandbyLogin` ~88–99.5% | FixStr `.empty()` vs `size()==0`; float pool `@4566`/`@4482`; frame/spill order | `mName.size() == 0`; `DECOMP_FORCEACTIVE` + `"CGameRestart"+N`; **`StaticDataHandle` at function entry**; **scoped `{ view … }` block** before aspect setup; **`float spills[4]`** vec4 u32 stores at `sp+0x10`–`0x1c`; **narrow height:** `efb - ((u32)(u16)unk230 << 1)` — omit `(u16)` on the doubled band so MWCC emits `slwi` not `rlwinm 16,30` from `(u16)unk230 * 2`; drop stale `DECOMP_FORCELITERAL` once spills hold `0.0f`/`1.0f` (fixes 4-byte `.text` overrun) |

### Asm era vs high-level era (lesson)

Several CView / UnkClass functions reached **100% with inline asm** then **regressed** when rewritten to high-level C++ (`setCurrent` 100%→74.5%, `setRect` 100%→84.3%, `wkStandbyLogin` 98.9%→88.8%). The gap is almost always **frame shape and register scheduling**, not wrong logic.

When community docs recommend asm or `register` for the last percent, treat that as **diagnosis** — then seek high-level alternatives (helpers, types, decomp.me) or document the policy gap in the attempt log.

---

## decomp.me workflow

### Tooling

| Tool | Role |
|------|------|
| [encounter/objdiff](https://github.com/encounter/objdiff) | Compares `.o` files, rebuild on save, demangles CodeWarrior symbols |
| [decomp.dev](https://decomp.dev) | Progress tracking powered by objdiff |
| [decomp-toolkit](https://github.com/encounter/decomp-toolkit) | Split DOL, reloc analysis, `dol diff` |
| [m2c](https://simonsoftware.se/other/m2c.html) | PPC MWCC C++ decompile hint (not final source) |
| [mwcc-debugger](https://github.com/cadmic/mwcc-debugger) | Dump MWCC register allocation state |
| [Decomp Academy](https://decomp-academy.dev) | Interactive MWCC matching lessons |
| [ppcdis](https://github.com/SeekyCt/ppcdis) | Disasm + `.sdata2` manual range overrides |

Config: `objdiff.json` maps `target_path` (retail split) ↔ `base_path` (your build).

### Steps (this repo)

1. **Build context**
   ```bash
   python tools/coop/run.py ctx libs/monolib/src/core/CView.cpp
   # Or after configure + ninja (uses same -I/-D as the object):
   ninja build/us/src/libs/monolib/src/core/CView.ctx.c
   ```
   Wraps `tools/decompctx.py` (includes from `libs/monolib/include`, `src/`, SDK paths). Compiler on decomp.me: **MWCC Wii/1.1** (`config.linker_version` in `configure.py`).

2. **Open in objdiff** — load `objdiff.json`, select object, open failing function.

3. **Create scratch** — objdiff → **decomp.me** button; move **only the target function** from Context → Source tab; trim trailing context if compile fails.

4. **Iterate in scratch** — edit high-level C++; MWCC compiles server-side; aim for byte match on that function.

5. **Paste back** — copy matched function into the real `.cpp`; preserve `extern "C"` blocks and includes.

6. **Verify locally**
   ```bash
   python tools/coop/run.py diff monolib/src/core/CView --symbol <mangled-symbol>
   ```

7. **Log attempt** — append to `docs/evidence/decomp/attempts.jsonl`.

**Best for:** small stubborn gaps on functions not yet at 100%. **Poor for:** whole-TU frame/layout issues (`setCurrent`, `wkStandbyLogin`) unless you decompose into leaf helpers first.

**Known tooling issue:** `coop run cycle` may fail if `decompctx.py` gets no include dirs — use `build` + `diff` directly.

---

## Policy exceptions (`PLAN.md` §17.6`)

When **one or few instructions** block `FULL_MATCH` after C++ + decomp.me:

### 9. MWCC PPC intrinsics — `DECOMP_PPC_*` (`include/decomp.h`)

Approved high-level (same as SDK `__rlwimi` / `__rlwinm`):

```cpp
#include "decomp.h"
u32 doubled = DECOMP_PPC_SHL1_U32((u32)(u16)unk230);  // slwi expansion mask 0,30
```

Log `"policy_exception": true` in `attempts.jsonl` on first use per function. If intrinsics regress frame %, try decomp.me placement or inline callee body instead of extra locals.

### 10. Single-instruction inline asm

Allowed when **exactly one** insn differs (e.g. `wkStandbyLogin` `slwi` vs `rlwinm`). Wrap with `DECOMP_ASM_INSN_BEGIN` / `END`, mirror retail operands, no `register` variables.

### 11. Relocation name drift — `@N` pools vs retail `lbl_eu_*`

`coop.json` uses `functionRelocDiffs=data_value`. Instructions can be byte-identical while objdiff still reports **99.7–99.9%** because relocation **symbol names** differ: MWCC pools a float/double into a TU-local `@2856` / `@N`, while retail’s split object references a shared SDA label (`lbl_eu_8066A1D8`).

**Prefer source first (no tooling):**

1. `extern "C" const f32/f64 lbl_eu_…;` (declaration only — **do not** define the constant in this TU).
2. Use the symbol in the expression so MWCC emits `R_PPC_EMB_SDA21` to that name (see §3 / §8c2 `updateCfPadData`).

**When source cannot name the pool** (common for MWCC’s implicit int-to-`double` subtract constant `0x43300000…`):

| Approach | Result |
|----------|--------|
| More C++ variants / `DECOMP_FORCELITERAL` | Often regresses scheduling or adds spurious `blr` stubs |
| `objdiff.json` `symbol_mappings` (`@2856` → `lbl_eu_…`) | **Does not affect CLI `report generate`** — UI-only ([objdiff #279](https://github.com/encounter/objdiff/issues/279)) |
| `powerpc-eabi-objcopy --redefine-sym` after compile | **Closes FULL_MATCH** when insn words + section bytes already match |

**Approved post-process** (`PLAN.md` §17.6 Relocation name drift; log `"policy_exception": true`):

```bash
# Wired into coop run build/diff/cycle/size for MTRand; reusable pattern:
powerpc-eabi-objcopy --redefine-sym=@2856=lbl_eu_8066A1D8 path/to/Unit.o
```

Reference implementation: `tools/postprocess_reloc_names.py` (wired into `coop run build/diff/cycle/size`; `postprocess_mtrand_object.py` is a thin wrapper).

1. **Patch bytes if needed** — e.g. MTRand: MWCC emits `43300000 00000000`; retail wants `43300000 80000000`. **Do not** apply this patch on units that keep *both* unsigned and signed magic doubles (CDeviceGX).
2. **Rename only reloc-referenced `@` symbols** — match `.sdata2` content at the symbol’s value; skip non-`@LOCAL@` pools that are not in `.rela.text`.
3. Prefer **content + reloc discovery** over hard-coded `@` IDs — numeric pool IDs drift when the TU changes.
4. **Exact renames** for non-pool symbols (e.g. `__vt__*` → `lbl_eu_*`, `@LOCAL@…@instance` → address-suffixed retail name).

Worked examples (`tools/postprocess_reloc_names.py`, wired via `coop run build/diff`):

| Unit | Reloc gap | Fix |
|------|-----------|-----|
| `MTRand.o` | `@N` → `lbl_eu_8066A1D8` (int→double); `@LOCAL@…@instance` → `…@instance_806561E0` | Magic byte patch + pool rename + exact LOCAL rename → `getInstance` / `randFloat` / `randFloat1` **FULL_MATCH** |
| `CfPadTask.o` | `@N` float pools → `lbl_eu_80667EA8`/`EB0`/…; `__vt__*` → `lbl_eu_80533D08`/`80533C90` | Pool + vtable rename → `create` / `updateCfPadData` / `Move` / `checkForControllerError` **FULL_MATCH** |
| `CDeviceGX.o` | `@N` → `lbl_eu_8066A440` / `8066A448` (both magic doubles) | Pool rename only (**do not** patch unsigned `…00000000` → signed — retail keeps both) → `viAfterDrawDone` / `copyEfb` **FULL_MATCH** |

---

## When FULL_MATCH may be unrealistic (high-level-only)

Be explicit in attempt logs when blocked:

1. **Caller-stack ABI** — Retail loads arguments from **caller's stack frame** (`setCurrent` retail `lwz` at `sp+0xC`). High-level member snapshots are semantically correct but may never reproduce spill/interleave order.

2. **Dead `mr` preservation** — MWCC omits moves with no semantic effect when the source reg is dead. **`return this`** fixed `func_80459270`; other cases may still need asm/register tricks forbidden here.

3. **Forced register homes** — Retail kept `this` in r28 vs r29 (`wkStandbyLogin`) based on live-range pressure across a large function. Readable C++ may pick different callee-saves.

4. **IPA / inline boundaries** — Retail may have inlined across a boundary your high-level factoring exposes as `bl`. Fix: match inline usage (hard without debug maps) or accept helper boundary drift.

5. **SDA float / string pool order** — Constants like `@4566`, `@4482`, `@stringBase0` require correct **data section** ordering; `-ipa file` and `.sdata2` layout matter.

**Recommended stance:** land **correct high-level C++** + log `HIGH_MATCH`/`CODE_MATCH` with concrete `next_change`. Escalate to user/policy only after decomp.me + pragma/flag sweep. Do not silently revert to asm.

**Known hard caps under policy:** `CView::setCurrent` (member asm), `CViewRoot::setCurrent` (frame-pointer iterator).

### 8d. `CViewRoot::setCurrent` — `mViewHistory` @0x4F4 + frame-pointer walk (~71.8%)

**`setCurrent__9CViewRootFP5CView`** (`libs/monolib/src/core/CViewRoot.cpp`) — retail uses **`-0x50` frame with `mr r31,r1`** and manual iterator state at `sp+0xC`…`0x18` while walking `view->unk238` (`mStartNodePtr` at `0x23C`). High-level C++ with function-scope locals + goto gates recovers semantics at **~71.8%** (500-byte body).

**Recovered layout:** `reslist<WORK_ID> mViewHistory` at **`0x4F4`** (ends at `mCurrentView@0x514`). Retail `push_back` into this list when the view's `mWorkID` is not already present.

**Semantics (verified from asm):**
- Early exit: `getInstance()==nullptr`, `view->mFlags & THREAD_FLAG_NO_EVENT`, empty `unk238`, `mCurrentView==view`.
- Resolve `CProc*` from **first** `unk238` node's `mItem` (passed to `getWorkThread__9CWorkUtilFUl`), type range `0x17`–`0x2F`, then `pssGetRoot__5CProcFP5CProc`.
- Skip full update only when `mCurrentView/mAttachedProc0/mAttachedProc1` match and **`proc->unk1E8 != 0`** (other attached-proc fields must match).
- Update `mCurrentView`, `mAttachedProc0`, `mAttachedProc1` via **`getInstance()` reload** after each store (same pattern as `destroyProc`).
- `extern "C"` on `getWorkThread__9CWorkUtilFUl` and `pssGetRoot__5CProcFP5CProc` for reloc match.

**Remaining gap:** ~28% is **frame shape** (`-0x50` + stack-homed reslist iterator), not wrong logic. Next: decomp.me scratch on this function or dedicated behaviour host test (≥30 scenarios below 80%).

### `CViewRoot::getView` — reslist walk + post-loop type check (FULL_MATCH)

Retail walks `mChildren.mStartNodePtr` (offset `0x60`), calling `getWorkThread__9CWorkUtilFUl(id)` each iteration until non-null or sentinel; **type check runs after the loop**, not inside it. Wrong pattern (type check inside loop on first hit) capped match at ~51%.

**Loop (100% match):** `-0x10` frame; `id` in r30; node in r31; `thread` stays in r3; loop condition reloads `getInstance()->mChildren.mStartNodePtr` each iteration.

**Type check (FULL_MATCH):** delegate post-loop filter to **`CView::convertToView(thread)`** (inline from header). MWCC inlines its `if (pThread == nullptr) return nullptr` plus `if (THREAD_CVIEW > type || type >= THREAD_CVIEW_MAX) return nullptr` as the retail nested `cmpwi`/`blt` epilogue with **one shared `li r3,0` block** — the manual inverted nested `if (type >= 48) { if (type >= 53) thread = nullptr; } else { thread = nullptr; }` + `return static_cast<CView*>(thread)` duplicated null stores (~94.9%). Do **not** add a redundant pre-check `if (thread == nullptr) return nullptr` before `convertToView` (92.5% — double null gate). Avoid standalone `||` range checks and goto chains on this function.

### `CViewRoot::getFullScreenView` — desktop child walk + fullscreen gate (~92.5%)

**Semantics (from asm):** `lbl_eu_806655D0` early out → `getView__8CDesktopFv()` → walk `desktop->mChildren` (`mStartNodePtr` @0x60). Per child: inline CVIEW type gate (0x30–0x35); qualify if `THREAD_FLAG_EXCEPTION`, **`mMsgQueue` contains `command==2` (`EVT_EXCEPTION`)** via ring at `child+0x1A4`…`0x1B0`, or skip when `mState` is LOGIN/RUN; reject `THREAD_FLAG_NO_EVENT`; accept if `unk278&1`, else `getFrame2ViewOffset` + `(unk278&4)==0` + zero offset sums + `unk1C8` size matches `getRenderModeObj` fb/efb. Fallback: return desktop view.

**Match notes (~92.5%):** `-0x30` / `stmw r27`; declare **`viewWidth`/`viewHeight` (r28/r27) before `childView`/`walkNode`/`desktopView`** for **r29=desktop / r31=child**; `lbl_eu_806655D0` direct null gate; `for (msgIndex…)` → **mtctr/bdnz** msg ring scan; exception flag sets `msgQualified=1` before shared LOGIN/RUN gate (`loginRunKeep` + `skipChild`); type gate via sequential `goto getFullScreenView_after_convert`; `extern "C"` on helper calls; offset-cast `mMsgQueue` @0x1A4–0x1B0.

**Remaining gap (~7.5%):** `viewFlags` in **r4** vs retail **r7**; type null branch **beq/bge** vs **bne/blt**; post-`getFrame2ViewOffset` **lha/extsh.** schedule; LOGIN **`r5` gate** vs retail **`li r3`/`r4` pattern**; SDA **`lbl_eu_806655D0`** reloc on prologue load. Next: decomp.me `-0x30` scratch on frame-check `lha` interleave + `viewFlags` r7 pin.

### `CViewRoot::create` — placement alloc + inlined pool/history init (~83.4%)

**`create__9CViewRootFP11CWorkThread`** (`libs/monolib/src/core/CViewRoot.cpp`) — retail has **no `__ct__9CViewRoot` symbol**; `create` allocates `0x520`, calls **`__ct__11CWorkThread` only**, sets **`lbl_eu_8056B710`** vtable, inlines init for three **`CViewRootPool`** subobjects @`0x1C4`/`0x2D4`/`0x3E4` (dtor `__dt__80442084`, spacing `0x110`) plus **`reslist<WORK_ID> mViewHistory`** @`0x4F4`, stores singleton **`lbl_eu_806655D0`**, `allocate_array(0x600)` into `mViewHistory.mList@0x508`, **8-iteration `bdnz` clear** (offset stride `0x60`, reload `mList` each store), `mCapacity=0x80@0x50C`, **`lbl_eu_806655D4=0`**, then **`entryWork` + `func_804385CC(0)`** even when alloc fails.

**Recovered layout:** `CViewRootPool` = embedded sentinel + `0x100` pad + reslist header fields (`mStartNodePtr/mList/mUsed/mCapacity`). Total object `0x520`.

**Match notes (~83.4%):** declare **`name`/`parent`/`root` for r31/r29/r30**; load string via **`lbl_eu_8052266C`** before `mr r29,r3`; **`extern "C"`** on `getWorkMem`, `allocate`, `allocate_array`, `__ct__11CWorkThread`, `entryWork`; direct **`*(void**)root = lbl_eu_8056B710`** (avoid vtable locals → `lwz`); **`goto create_clear_loop`** with **`mViewHistory.mList` reload per store** for `bdnz` body; history sentinel circular init **reload `mStartNodePtr` between `mNext`/`mPrev` stores**. Remaining ~17%: **`lis/addi` vtable scheduling** vs temp loads, loop **offset home r3 vs r4**, **`li r9,32` vs `li r6,32`** across pool stores.

### `CViewRoot::renderView` — root child walk + render dispatch (~83.5%)

**Semantics (from asm @0x80445A5C):** Early out: `getInstance()==nullptr`, **`lbl_eu_806655D4!=0`** (cleared in `create`), **`root->mFlags & THREAD_FLAG_NO_EVENT`**, **`getInstance__8CDesktopFv()==nullptr`**. Walk **`CViewRoot::mChildren`** (`mStartNodePtr` @0x60; node in r31, child in r3). Per child: inverted CVIEW type gate (0x30–0x35); **skip render when `THREAD_FLAG_EXCEPTION` or msg ring has `command==2`** — retail routes both through **`.L_80445B44` qualified block** (`cmpwi`/`li r0,0`/`bne` then LOGIN/RUN state check sets r0=1); call **`renderView__5CViewFv`** when r0≠0 after state gate.

**Match notes:** `-0x10` frame / r31=walkNode; **`extern "C"`** on `getInstance__8CDesktopFv`, `renderView__5CViewFv`, `lbl_eu_806655D4`; bottom-checked loop (`goto renderView_loop_check` + `getInstance()` reload on sentinel compare); replicate B44 qualified gate (do **not** early-goto skip on exception — set `shouldRender=1` and fall into shared block). High-level C++ reaches **~83.5%** at retail size `0x150`; remaining ~16% is **mtctr/bdnz msg loop** and **r3=child / r31=node reg pinning**.

---

## Quick checklist before claiming FULL_MATCH

- [ ] `python tools/coop/run.py diff <unit> --symbol <sym>` → 100%
- [ ] Relocations tab clean (not just instructions) — 99.7–99.9% with identical insn words → check `@N` vs `lbl_eu_*` (§11)
- [ ] Same `extra_cflags` as `configure.py` for that object
- [ ] No asm / `register rN` / fake `sp[]` in source
- [ ] Attempt logged in `docs/evidence/decomp/attempts.jsonl` (`policy_exception` if using objcopy rename)
- [ ] `TASKS.md` / `configure.py` `Matching` updated for whole TU

---

## Further reading

- [TWW decompiling.md](https://github.com/zeldaret/tww/blob/main/docs/decompiling.md) — inline functions, minor fixes, decomp.me
- [TWW regalloc.md](https://github.com/zeldaret/tww/blob/main/docs/regalloc.md) — regswap causes
- [objdiff README](https://github.com/encounter/objdiff/blob/main/README.md)
- [decomp-toolkit README](https://github.com/encounter/decomp-toolkit/blob/main/README.md)
- [Decompedia — MWCC](https://decomp.wiki/compilers/MWCC)
- [NXP AN4094](https://www.nxp.com/docs/en/application-note/AN4094.pdf) — SDA linker options
- [CodeWarrior pragma / section reference](https://docs.nxp.com/bundle/GUID-9FAC1C79-3809-474F-B8DF-82BEB5B88419/page/GUID-0DE5F557-13B4-41E3-8BB6-51B99A563439.html)
- [ppcdis TOOLS.md — sdata2 / IPA floats](https://github.com/SeekyCt/ppcdis/blob/main/TOOLS.md)
