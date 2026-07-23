# MWCC reference — closing the last few percent

Practical reference for reaching **`FULL_MATCH`** (100% byte match) or **`EQUIVALENT_MATCH`** on Xenoblade Chronicles Wii using **high-level C/C++ only**. Retail assembly, Ghidra, and `build/us/asm/` are **reference only** — do not ship asm, `register rN`, fake stack buffers, or register-named parameters in `src/**` or `libs/**`.

**Policy sources:** `.claude/skills/xenoblade-decomp/SKILL.md`, `PLAN.md` §17, `coop.json` (`functionRelocDiffs=data_value`).

**Living document:** agents must **append new patterns** here when they discover reusable MWCC matching knowledge. Do not leave discoveries only in `docs/evidence/decomp/attempts.jsonl` or chat.

---

## Quick diagnostic: I'm stuck at 97–99.9% — what do I check?

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
| Stack slot at wrong offset | MWCC alignment / padding | **§5** — use multiple smaller types (`CPnt16` × N not `CRect16` × 1) |
| Zeros/sign-extension: `extsh` vs none | `s16` local vs `int` | Use `int` unless retail has `extsh` |
| Struct offset off by 4/8 | Missing pad or wrong base layout | Fix header layout before tuning C++ |
| Bitfield `extrwi` pos off by 2 (e.g. bit 19 vs 17) | Adjacent bitfields swapped in MWCC MSB-first layout | Reorder fields in the bitfield struct to match retail extract (nw4r `TexMap::mBits`: `paletteFormat` before `anisotropy`) |
| SDA vs far addressing | Different insn (`lwz@sda21` vs `lis`/`addi`) | Correct section/size via linker script |
| Wrong callee-save depth | `_savegpr_29` vs `_savegpr_28` | Shrink live ranges across large functions |
| Inlined vs outlined | Extra/missing `bl` | Match retail inline boundaries; IPA pragmas |
| Same body duplicated → ~99.3% regswap vs retail | Manual paste of callee body keeps callee's RA | Prefer `callee();` + `-ipa file` so IPA inlines in *caller* context (different Chaitin colors) |
| Wrong float constant pool | `lfs` from wrong `.sdata2` slot | `extern "C" const float lbl_eu_*` |
| Ternary vs `if/else` codegen | Extra `b` or `sel` | Toggle form |
| Ghidra `r13` SDA | Misleading decompilation | Set SDA bases in Ghidra |

---



## kyoshin/main (US) — early init + contiguous .data base

US `main` is **not** the JP-shaped “copy ErrMesData strings then initialize” path. Retail:

1. Clears `lbl_eu_8065FD00[0x125]`, calls `CBdat::func_8003AA50`, then `CErrMes` / `CNandData` setup.
2. Registers path pointers via `func_eu_804520B0` (three string getters + `sLanguageFolderPaths` + `languageFolderPaths`).
3. Wires disc/NAND error strings from `func_eu_802B133C`…`802B1414` into `CDeviceFileCri` / `func_804DAA90`….
4. `setRegionMaxSize(0x686000, 0)` (not `MemRegion::initialize()`’s `0x680000`), `setUseStaticHandle(true)`, `CDeviceGX::setValues(GX_PF_RGB8_Z24, 0x180000)`.
5. Ends with `func_eu_80457318(func_eu_802B136C())`, `CWorkRoot::run()`, `func_eu_802B1334()`.

**.data layout:** keep `sGameMainIcon`, non-`const` `sLanguageFolderPaths[8]`, `languageFolderPaths[8]`, then `sStaticArcFiles` contiguous so MWCC holds `r30 = &sGameMainIcon` and emits `addi r3,r30,0x10/0x30/0x50`. `const` path arrays fall into `.rodata` and break that.

**Flags:** `-O4,s` + `-func_align 4` — size opt yields `stmw`/`lmw` for r30/r31; without explicit `-func_align 4`, `-O4,s` pads thin wrappers to 16 and blows the split.

**Locals / entryTable soft-cap:** keep `char* dataBase = (char*)&sGameMainIcon` for r30-relative `+0x10/+0x30/+0x50`. MWCC peeps C `dataBase+0` **and** asm `addi r3,r30,0` to `mr r3,r30`. Close with §17.6 inline asm that emits `opword 0x387E0000` (`addi r3,r30,0`) then `li r4,1` / `bl entryTable__…` (declare the mangled symbol in `extern "C"`). Do **not** keep a live `int clear` next to that asm — it reshuffles r30/r31; use literal `0` for the early stores. Do **not** post-process `.text` (`insn_patches` forbidden).

## CAIAction UnkVirtualFunc1/2 — `stwx` vs `stwux` (US)

Frameless Fv+r4/r5 ring copy leaves. High-level C++ CSEs dest materialization into **`stwux`** (r4+=r8) plus sequential loads from src-in-r7; retail keeps **`stwx`+`add r7`** with src in **r9** and an interleaved **8-then-4** field schedule (exact size `0x110`). Indexed/`buf[i]` / separate `dstBytes` stores still fuse under MWCC. Soft-cap ~96%; keep high-level C++ — **no** `.text` `insn_patches` (skill forbids).

## CBattleState UnkVirtualFunc6 — regalloc soft-cap (US)

Fv+r4 entry walk (8×`0x34` at `+0x1388`), bitfield set into `+0x15AC`,
tail-call vt+`0x48` via **`bctr`**. Exact size `0x154`. **`BattleStateV6If::vf48`**
fixes vptr coloring to retail `lwz r12` (~92.5%, was ~92.2% with FP temps).
Residual ~44 words: prologue GPR homes (`id`/`one`/`entries`/`n` = retail
`r6`/`r7`/`r5`/`r0` vs MWCC `r0`/`r5`/`r8`/`r10`) and walk in `r9` vs `r7`.
`id`/`one`/`entries` decl-order and scoped-`one` regress. Keep high-level C++;
**do not** use `asm void`.

## CBattleState UnkVirtualFunc8 — NV home / scan soft-cap (US)

Fv+r4 kind tree + slot walk (`0x68`×`0x34`). Fake SI `BattleStateV8If` (omit RTTI `_v000`/`_v004`) emits retail **`lwz r12` / `bctr`** for vt+`0x2C`/`0x4C`. Soft-cap ~98%: Chaitin parks **this/entry in r27/r28** (retail **r28/r29**) and **thirteen in r29** (retail **r27**); scan loop drops dead **`addi r3,r3,7`** (decomp `0x420` vs retail `0x428`); word-copy uses r0/r5 vs retail r6/r0. Function-scope `slot`/`i`, late `one`/`thirteen`, and trip keep-alives do not move the NV homes. EQUIVALENT blocked by indirect vcalls + unaccepted `memset`. Keep high-level C++; **do not** use `asm void`.

## CBattleState UnkVirtualFunc26 — schedule + mtctr copy (US)

Fv+r4 src table: vt+`0x78` clear, 8×`0x10` records → stack `CBattleStateEntry` → vt+`0x1C`, then `0x80`-byte word copy into `this+0x1528`. Exact size `0x12C`.

**Breakthroughs:** `BattleStateV26If::vf1C` (omit RTTI `_v000`/`_v004`) restores retail **`lwz r12` / `bctr`** for vt+`0x1C`. `entry.unk30 |= 1` (no flags temp) + `flag4000` before `flag2000` → ~78.8% HIGH_MATCH.

**Soft-cap:** (1) MWCC hoists `lhz`/`extrwi.` on `unk0E` before field stores (retail keeps them late; steals r0 from the flags `ori`). Reload/`flags != 0` barriers regress. (2) Tail copy is retail **`mtctr`/`bdnz` + `lwzu`/`stwu`**; high-level `do { … } while (--i)` emits **`addic.`/`bne`** (same size). Keep high-level C++; **no** `insn_patches`.

## COccCulling::setFrustum — scale/rot + interleaved planes (US)

Exact size `0x588`. Best ~**89.2%** HIGH_MATCH: `setScale(CVec3(sx,sy,lbl_eu_80667C88))` + `setRotXYZ` + `FLAGS_01` plane guard + SDA dir/unk124/128 zeros + `math::sqrt` (inlined `FSqrt`/`Warning`/`FrSqrt`).

Retail prologue builds scale with field `stfs` of SDA 0/1, then **`MTX34RotXYZFIdx`** with **`lbl_eu_80667C90`** (RAD_TO_FIDX) and `PSMTXConcat(rot,scale,scale)`. Explicit FIdx + `lbl_eu_80667C90` matches size but **drops fuzzy ~1–2%** vs `setRotXYZ` (`MTX34RotXYZRad`). Side-plane bodies are retail **psq-interleaved** edge/normalize/dist; sequential `CPlane::set(p0,p1)` stays shorter/differently scheduled. Split spare ~`0x14`. Keep high-level C++; **no** `insn_patches`.

## CMenuBattlePlayerState ctor — 0x20 vs 0x60 pad clear (US)

`__ct__CMenuBattlePlayerState`: Process+PTMF+`__construct_array`+per-slot stack temp. Peak ~**65.5%** STRUCTURAL; decomp ~`0x4C4` vs retail `0x580` (`_savegpr_21` / `-0x2B0`).

Retail mid-clear of `pad90[0x174]` (and standalone leaf `func_8010B324`) is MWCC’s **0x60 + 0x0C** gated fill (`li`/`divwu`/`mtctr`/`bdnz`, ctor also hoists `0x2AAB` size math). Same high-level `do-while` word clear under stock **`-O4,p`** emits a **0x20 + 0x4** fill (`mulhw`-style gates). Explicit 24-store `/0x60` under `-O4,p` strength-reduces the count to **`mulhwu`** (or blows the loop with `for`-countdown).

**Breakthrough (leaf `us-8010be00` ~83.8% HIGH_MATCH, reported size `0x120`):** `#pragma optimize_for_size on` (or TU `-O4,s`) around the gated form plus a retail-like bool preamble:

```c
if (start >= end) return;
lim = end - 0x60;
ok = 0; ok2 = 0;
if (!(start > end)) ok = 1;
if (ok != 0) ok2 = 1;
if (ok2 != 0) {
  c = (lim + 0x5f - start) / 0x60;
  for (; c; --c) { /* 24 u32 stores */ p += 0x60; }
}
c = (end + 0xb - p) / 0xc;
for (; c; --c) { /* 3 u32 stores */ p += 0xc; }
```

emits retail’s `li 0x60`/`divwu`/`mtctr`/`bdnz`/`addi …,0x60` family (same as `CMenuBattleDamage` `func_8010A940`). Whole-ctor `#pragma optimize_for_size` **regresses** fuzzy (rest of ctor wants speed opt); inlining the gated body into the ctor under `-O4,p` also regresses (~33%). Keep ctor on word-while + live `0x60`/`0x0C` NVs until a scoped size-opt path works. Remaining leaf gaps are mostly regalloc/schedule (e.g. end in `r8`, store-zero in `r5`).

**Best ctor ~65.5% STRUCTURAL:** hoist stack `slot` + `padStart`/`padEnd`/`padSize` + live `0x60`/`0x0C`/`size+0xb` NVs **after** unk7D0 floats, word-while mid, post-float `0x11` head + sparse tails, keep NVs live with cheap xor + `(nv&z)` sink. Still `-0x2A0`. Ruled out earlier: `memset`/`__fill_mem` → `bl`; byte-index; fixed-trip unroll; `bl` leaf; early-hoist before Process; volatile strides (frame + no `mtctr`). **no** `insn_patches`.

## CHelp* / CChainCombo — r12 iface / vptr walks (US)

Manual vptr loads (`this+8` iface tables, `CChainVObj` / `CfObjectPc` vptrs) must use **`lwz r12,…`** like a C++ virtual call. High-level function-pointer temps color as **r4/r5**. Prefer fake SI virtual iface (`MenuBpsActorIf` pattern) or other high-level reshape — **`asm void` bodies are not allowed**.

## CUICfManager::Init — packed slot templates (US)

Large `-0x1A0` / `stmw r22` frame: `readFile` + `0x54` process/`__ptmf_null` (same prelude as `CUIBattleManager::Init`) then eight `0x168` slots.

**Breakthrough:** stack **zeros** and aggregate **copy** are different views of the same `0x90` bytes. Named `u16` fields give correct `sth` zeros but memberwise `lhz` copies. Retail copy is:

`lwz +0`, paired `+8/+4`, `lhz +0xC`, lone `lwz +0x0E`, then memcpy-style paired unaligned words through `+0x8E/+0x8A`.

**Best high-level so far ~94.8% HIGH_MATCH** (copy load schedule equals retail):
1. Contiguous `CUICfInitTemplates { state, block0, blocks[3], tail }`.
2. `#pragma pack(1)` Tail: `u32`, `u8 mid[8]` (pair-copy → `+8/+4` loads), `u16`, `u32` at `+0x0E`, then `u8[0x40]` + `u8[0x3E]` chunks.
3. Zeros via `u16*` overlay at `+4` (six `sth`) + `u8*` stores at `+0x10/+0x12/+0x52` — not via the copy field types.
4. Split field assigns; last chunk through a `u8[0x40]` cast view so the final pair is `lwz +0x8E/+0x8A` (2-byte overhang), not `lhz +0x8E`.
5. Clear: fresh `&blocks[0]` + `(end+stride-1-start)/stride` countdown (today `mulhwu`/`bne`; retail `divwu`/`mtctr`/`bdnz` + `li r22,52` still open). One big `u8[0x82]`/`u8[0x7E]` → `lwzu`/`bdnz` loop — split under ~`0x40`.

**Regressions:** whole-Tail assign with large trailing arrays; union/`memcpy` dual-views; volatile stride (spills and shifts stack); runtime stride + `for(count)` reintroduces 8× clear unroll. Keep high-level C++; **`asm void` bodies are not allowed**.

## CBattery / CBgTex IWorkEvent-compatible widgets (US)

Retail stores an **IWorkEvent vtable pointer at +0** but the TU often **does not emit** C++ `IWorkEvent` weak defaults or a local vtable (vtable lives in split1 `.data`).

**Do not** `class X : public IWorkEvent` in these TUs — inline/OOL defaults still tend to pull weak stubs or change dtor shape and blow the split `.text` budget.

Pattern that matched:

1. Plain struct with `void* mVtbl` set to `lbl_eu_*` (retail vtable label).
2. Shared string pool via `extern "C" char lbl_eu_*;` + byte offsets (not local string literals).
3. SDA globals by retail name (`lbl_eu_8066…`), not TU-local `static`.
4. US often uses **stripped symbol names** (`__ct__CBgTex`, `func_801C3A24`) — implement with `extern "C"`; keep truly mangled names (`__dt__`, `OnFileEvent__…`) as C++.
5. Add `-func_align 4` when default align-16 inserts pad gaps between functions.
6. Callee reloc names must match retail (`extern "C" void func_80124270(...)` when retail reloc is unmangled).

## How objdiff matching works here

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

## MWCC compiler behavior

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

---

## C/C++ → MWCC translation patterns

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

When a vtable / data table already references the shortened `…Fv` name (common for help/switch helpers), keep the retail symbol via `extern "C"` and take the extra args on that entry point, e.g. `func_802B7CBC__Q22cf11CHelpSwitchFv(self, u32 flag)`.

### cf::CHelp layout (manual iface at +0x8)

Retail `__ct__Q22cf5CHelpFv` stores `owner@0`, `param@4`, `lbl_eu_8053B3A0@8` — **not** a C++ vptr at +0. Derived helps add fields from `+0xC` (`s32` thresholds need `s32`/`cmpw`, not `u32`/`cmplw`). Calls through `this+0x8` are a manual interface table; MWCC function-pointer codegen often uses `r4` where retail virtual-style loads use `r12` (~99.3–99.6% near-miss).

High-level source should use `mVtbl->mSlots[N]` (or equivalent `this+0x8` reload between calls). Near-misses that are only vtbl temp register coloring (retail `r12` vs MWCC `r4`) need further high-level C reshaping or §17.6 single-instruction asm — do **not** post-process `.text` instruction words.

**Actor/move deep vtable slots (r12):** function-pointer `vslot` loads color the vptr as `r4`. Cast the object to a fake single-inheritance interface with `virtual` methods at the retail byte offsets and call through that — MWCC emits `lwz r12,0(obj)` / `lwz r12,off(r12)` / `bcctrl`. With kyoshin `-RTTI on`, omit the first **two** pad virtuals so slot `0xN` lands at offset `0xN` (hidden typeinfo slots). See `MenuBpsActorIf` / `MenuBpsMoveIf` in `CMenuBattlePlayerState.cpp` (`menu-bps-move` ~99.8%).

**reslist::size() regalloc:** retail often wants `length` in r4 / `cur` in r3. Declaring `length` first in a *local* inlined helper can match without changing shared `reslist::size()` (that reorder regressed `menu-enemy-cbrender`).

**menu-bps-move Chaitin ceiling (~99.76%):** after r12 virtual ifaces + local size helper, residual is (1) party-fill GPR permutation `party/shift/actors` = retail `r21/r22/r23` vs MWCC `r22/r23/r21`, and (2) loop-hoisted FPR homes for `FB8/FA8/FC0/F90/F94` (+1 rotation on `{f27…f31}`). Ruled out: named FPR NVs, bias helpers, actorsBase-before-party, block-scoped party, IPA `fillActors`, fcmpu swap, `fullHpCount` move. Keep high-level C++; **no** `.text` `insn_patches`.

**menu-enemy-cbrender sort Chaitin (~99.172%):** keep exact size `0x274` with draw counter as `u32 i` and `order[static_cast<u8>(i)]` / `while (i < 0x18)` (u8 counter emits terminal `clrlwi`, +4). Sort: pass-before-order + block-scoped `s32 limit`, outer `for (left=0x17; left!=0; left--)` CTR, inner bottom-tested goto, `&indices[j]` rematerialize, depthB before depthA, XOR store-reload with `swapped=1` after the first store. That locks `order=r9`, `j=r12`, `swapped=r11`. Residual: `pass/limit/pair` = MWCC `r8/r10/r30` vs retail `r10/r5/r8` (pair should reuse vacant `r8`). Hoisting `j` before `order` gets `pass=r10` but pins `j` in `r8` and spills pair to `r30`. Dead `gap` before the loop is DCE'd and does not reserve `r8`. Equivalence blocked by unvalidated callees.

**occ-cull-helper dir-vector Chaitin (~99.829%):** keep `CPlane::isOnPositiveSide`
  + `CVec3::dot(mDir, unk24->unk10C - mPos)` / `lbl_eu_80667C8C` (0.0f pool rename OK).
  Residual six words: retail `r3=&delta(sp+0x14)` / `r5=cam` vs MWCC `r5=&delta` /
  `r3=cam`. Ruled out: scratch/dir/cam decl order (size +8, ~92%), named
  `CVec3::sub` (~99.6%), cam-alias / `CFrustum&` (~98%), two-stage copy (+size).
  Keep high-level C++; **no** `.text` `insn_patches`. Equivalence blocked by
  unvalidated callees → needs FULL_MATCH or a new RA shape / multi-insn §17.6.

**CView CMsgParam soft-caps:** `attachRenderWork` (~85%) and `setCurrent` (~78%) share the dual-/single-inline enqueue snap-load schedule ceiling. High-level `CMsgParam<10>` already matches frame/`stmw`/`stwux`; prior FULL_MATCH used forbidden insn_patches or whole-function asm (rejected). Demote any ACCEPTED below EQUIVALENT/FULL.

**CView::updateMsg (~76%):** size already `0x798`. Gains: three uninit `volatile CtxSnap` only (no fan-copy layer; was 74.4%), plus case 0/1 child fan-out via `CMsgParam<10>` enqueue. Residual: prologue still `-0x140`/`stmw r17` vs retail `-0x150`/`stmw r14`, snap homes `0x80/0x60/0x40` vs `0x48/0x28/0x08`, `switch` tree vs `bctr` jumptable (MWCC rejects GCC `&&label` computed goto). Same caller-stack class as attach/setCurrent — park under high-level C++.

**CUIWindowManager::Move (~84.7%):** size `0x4DC` (8-wide unlink unroll present). Win: dual-cursor mark-all — search walks while mark stays at list head, then marks the *entire* list (retail r6/r7; was ~80.2% with iterator “mark from hit”). Keep poison-store `pending[i]=0` after `mNext=NULL` (dropping it → ~83.1%). Residual: unlink Chaitin + extra poison vs retail single `mNext` clear. Park unless IPA-outlined unlink helper.

`CHelp_Pg::func_802B85A4` int→float uses retail `lbl_eu_80669000@sda21`; MWCC pools `@N` with the signed magic double — rename via `CHelp_Pg.o` `pool_patterns` `(MAGIC_HI, MAGIC_LO) → lbl_eu_80669000` once `.text` already matches.

---

## Core patterns — the 5 things that fix 90% of gaps

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

#### 1d. Post-process rename (when source can't name the pool)

For MWCC's implicit pools (e.g. int-to-double `0x43300000` magic constant), `objcopy --redefine-sym` after compile:

```bash
powerpc-eabi-objcopy --redefine-sym=@2856=lbl_eu_8066A1D8 path/to/Unit.o
```

Reference implementation: `tools/postprocess_reloc_names.py` (wired into `coop run build/diff/cycle`). **Do not** rely on `objdiff.json` `symbol_mappings` — CLI-ignored (objdiff #279).

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

---

## Policy exceptions (`PLAN.md` §17.6)

When high-level C++ and decomp.me cannot close the last instruction(s):

| Tool | Use |
|------|-----|
| `DECOMP_PPC_*` intrinsics (`decomp.h`) | Opcode selection e.g. `slwi` vs `rlwinm` |
| Single-instruction `asm { }` | When exactly one insn differs |
| `extern "C" lbl_eu_*` | Reloc names when values match |
| `objcopy --redefine-sym` (§1d) | Implicit `@N` pools source can't name |

**Still forbidden:** `register rN`, fake `sp[]` buffers, **`asm void` / whole-function asm bodies**, standalone `.s` units, and post-process **instruction-word** patches on `.o` files.

Log all exceptions with `"policy_exception": true` in `attempts.jsonl`.

---

## RVL near-done TUs: strip unused + BSS visibility

- Empty `//unused` asm stubs and `DECOMP_FORCEACTIVE` helpers often push Matching
  TUs over the retail `.text` split. Delete them when they are absent from the
  retail object (`nm` EXTRA list).
- Scheduler / OSThread BSS objects (`DefaultThread`, `IdleThread`, `RunQueue`,
  `SwitchThreadCallback`, …) are **global** (`B`/`D`) in retail. Leaving them
  `static` makes them local (`b`/`d`) and tanks data match even when code is
  100%.
- Thin SPR wrappers called from C (e.g. `PPCMfhid0` / `PPCMthid0` used by
  `PPCDisableSpeculation`) must stay outlined: `#pragma dont_inline on` around
  the asm accessors, and prefer `asm void` `mthid0` over inline `mtspr`.
- Helpers that retail inlines into a larger function (e.g. former `__AXDumpVPB`
  inside `__AXSyncPBs`) should be open-coded at the call site, not left as
  separate exports.

## RVL OS / OSThread linker-ADDR16 bake

Xenoblade US retail `.o` splits bake some linker symbols into absolute
`lis`/`addi` immediates (no `R_PPC_ADDR16_*`), while MWCC emits relocatable
zeros + relocs for `_stack_addr` / `__ArenaLo` / `_db_stack_end`. Retail
`OSInit` also uses `memcpy@ha`/`@l` as `OS_BASE_CACHED` / the `0x80004000`
fallback.

**Keep high-level C with extern linker symbols** (right schedule). Close the
last immediates with `tools/postprocess_reloc_names.py`:

- `bake_linker_addrs` — fill HA/HI/LO from the **effective** baked address
  (`0x8067B560` / `0x8067D560`) and drop those relocs. Handles `R_PPC_ADDR16_HA`
  (addi) and `R_PPC_ADDR16_HI` (lis+ori, e.g. `__init_registers` in `.init`).
  Also works on `.rela.init` when `.text` is absent.
- `force_symbol_relocs` — rewrite `lis 0x8000` / `addi 0x4000` to zeros +
  `memcpy` ADDR16_HA/LO (dtk’s retail-split form).
- `__start.o`: bake **only** `_stack_addr` — retail keeps `_SDA*_BASE_` as
  ADDR16 relocs (zeros + reloc), not baked immediates.

Do **not** use `DECL_ADDRESS` / integer literals for these (reshuffles to
~98%). Do **not** reintroduce Chaitin `insn_patches`.

## RVL WUDHidHost (Matching)

- Stub `DEBUGPrint` strings need padded `char[N]` arrays with retail reloc
  sizes (`Invalid app_id` → 24, `bta_hh_co_open/close` → 20). Define them
  **after** `WUDiHidHostEventCallback` so `.data` stays
  callback strings → jumptable → stubs. Rename via
  `postprocess_reloc_names` (`s_*` → `lbl_805636C8/E0/F4`).
- Empty `BTA_HH_{SET,GET}_{RPT,PROTO,IDLE}_EVT` cases must appear as SET then
  GET in source so the string pool order matches retail (enum values remain
  GET-then-SET).

## RVL ipcclt near-misses

- **`IpcReplyHandler` / `IPCInterruptHandler` / unit**: **FULL_MATCH** on default
  **`Wii/1.1`** (no GC pin). Petari `switch (req->base.fd)` + `__relnchRpcSave ==
  req`. ACR IRQ clear: §17.6 **`opword 0x3c60cd00 / 0x3c004000 / 0x90030030`**
  (BASE_FIRST without r0 RA clobber; same on inlined Ack). Ack →
  **`__ipcSendRequestSubf`** (`queued - sent`, `if (waiting == 0)`); Reboot keeps
  Petari `~0` wrap on shared helpers. **Do not** pin the TU to `GC/3.0a5.2` —
  that compiler reorders the post-MSG `__mailboxAck` store ahead of
  sent/front updates (~7 word diffs on Interrupt) and also regresses Reboot.
- **`IOS_IoctlvReboot`**: **FULL_MATCH** on Wii/1.1 (see ipcclt section).

## RVL GXPerf (Petari donor)

- Retail TU is only `GXSetGPMetric` + `GXClearGPMetric` (budget `0x830`); omit Petari’s `GXReadXfRasMetric`.
- Map Petari `gx` → `gxdt`; `GX_WRITE_XF_REG`/`GX_WRITE_RA_REG`/`GX_WRITE_CP_STRM_REG` → `GX_XF_LOAD_REG` / `GX_BP_LOAD_REG` / `GX_CP_LOAD_REG(0x20, …)`.
- CP MMIO: `extern volatile void* __cpReg` + `*(volatile u16*)((volatile u16*)__cpReg + idx)`; clear metric writes index `2` (= byte `4`), memperf select index `3` (= byte `6`).
- Stallperf field: `GX_BITSET(perfSel, 24, 4, sel)` (same rlwimi as Petari `SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL`). End with `gxdt->lastWriteWasXF = GX_FALSE` (Petari `bpSentNot`).
- Name `GXData` fields at `0x5EC`/`0x5F0`/`0x5F4` (`perf0`/`perf1`/`perfSel`) so switches stay high-level.

## RVL GXPixel (ogws donor)

- **`GXSetFog`**: ogws fog math + `__cvt_fp2unsigned` on perspective path; load
  FOGPARAM1/2 only there; common path loads reg0/3/color. Keep WGPIPE base in
  one register across both perspective BP writes via
  `volatile void* pipe = (volatile void*)&WGPIPE` (not two `GX_BP_LOAD_REG`).
- **`GXInitFogAdjTable`**: use `sqrt` (double), not `sqrtf` — retail inserts
  `frsp` before `fmuls` and is `0x160` bytes; `sqrtf` drops 4 B / ~98.75%.

## RVL AXFX ReverbHiExp (Petari donor)

- Petari `AXFXReverbHiExp.c` is a clean FULL_MATCH for Xenoblade’s Exp TU
  (Init/Shutdown/Callback + `__AllocDelayLine` / `__BzeroDelayLines` /
  `__FreeDelayLine` / `__InitParams`). Size fits with ~0xC spare.
- Xenoblade retail `AXFXReverbHi.o` only exports Init/Shutdown/Callback — do
  **not** keep ogws `GetMemSize` / `Settings` wrappers in that thin TU (they
  blow the `0xB0` split). Those APIs are absent from this game’s Exp object too.
- Remaining Exp TUs (Delay/Std/Chorus ± Dpl2) lack full C donors in Petari/MKW/
  open_rvl; MKW only has coefficient tables for some.

## RVL AXFX ChorusExp (no Petari/MKW C donor)

- TU exports **SettingsUpdate**, not Settings (Settings stays header-only).
- **`GetMemSize` / `Shutdown`**: `return 0xc80 * 3 * sizeof(f32);` matches retail
  `lis`/`subi` → `0x9600`. Shutdown is a plain free-loop under interrupt mask.
- **`Init` / `SettingsUpdate` (~99%)**: inlined free-on-failure (not `bl Shutdown`);
  success flag set only on loop fallthrough (`ok = TRUE` after alloc/memset), with
  delay `inPos`/`outPos`/`lastPos`/`sizeFP` computed in the memset success path.
  Remaining miss is walk/index GPR color (`r27`/`r29` swap).
- **`__InitParams` (~96%)**: validate ranges (feedback is `[0,1)` via `>= 1.0f`);
  LFO setup `step = (32000/rate)*0.00390625`, `phaseAdd = (256*rate)/32000`, then
  `65536` scales. MWCC emits sdata2 tail `0.00390625,32000,256,65536`; retail is
  `65536,256,32000,0.00390625` — `reverse_sdata2_trailing_f32x4` on
  `AXFXChorusExp.o` (§17.6). Forcing first-ref with `scale=65536` locals restores
  pool order in-compiler but drops InitParams to ~68%.
- **`Callback` (~87%) / `__CalcLFO` (~64%)**: soft-cap. Callback history fill is
  retail `mtctr`/`bdnz` (+ dead `subi`) vs MWCC `subic.`/`bne`; `__CalcLFO` retail
  uses `mulhwu`+sign-fix and a `r31` stack frame for `(s64)a*b>>24`, while Wii/1.1
  `-O4,p` prefers a shorter `mulhw` path (~0x20 smaller). Keep high-level
  `((s64)…>>24)` / 96-sample LFO semantics.

## RVL AXFX ReverbStdExp schedule ceilings (Wii/1.1 `-O4,p`)

- **`GetMemSize` (~45.7%)**: best high-level keeps the retail add chain
  (`early[7] + (u32)(s32)(32000*preDelayTimeMax) + filter[6][0..3]) * 12` with a
  `filter0` local). Under `-O4,p`, MWCC always emits `lfs`/`lis` before `stwu`,
  fills `fmuls` latency with `early[7]` then `filter[0]` (never `filter[0]`
  *before* `fmuls`), and places `filter[3]` after `stfd` (retail: between
  `fctiwz` and `stfd`). `#pragma scheduling off` gets `stwu` first but destroys
  the multi-reg latency-fill pattern (~36%). Volatile/pointer/explicit
  sequencing does not force `filter[0]` before `fmuls`. Soft-cap until a
  non-patch approach appears; no `.text` `insn_patches`.
- **`Callback` (~98.4%)**: best setup is
  `lpfCoef1 = 1.0f - reverb->lpfCoef; lpfCoef2 = reverb->lpfCoef;` then
  `early/fused * 0.6f` and comb/allpass locals; hoist sample positions; unroll
  pos updates. That already keeps **lpf in f0**. Remaining miss is FPR color
  (`1.0`→f1 vs f2, `(1-lpf)`→f2 vs f5, comb/allpass perm, i2f magic f10 vs f11)
  plus loop GPR allocation. Loading mem floats before `1.0f` to pin `1.0`→f2
  regresses overall match. TU is `-lang=c` (C89): no mid-block declarations.
- **`.sdata2` pool order:** compute the `pow` exponent in a local first
  (`f32 exp = -3.0f * …; (f32)pow(10.0, exp);`) so MWCC emits `-3` (with pad),
  then `10.0`, then `0.95` (with pad) — matching retail’s 0x40 pool tail.
  Init still first-uses `0.0f` before `32000`, so the leading pair stays swapped
  vs retail; `tools/postprocess_reloc_names.py` (`swap_sdata2_leading_f32_pair`
  on `AXFXReverbHiExp.o`) swaps those two words + symbol `st_value`s (§17.6
  data-pool postprocess, same family as MTRand magic). Do **not** introduce a
  `f32 rate = 32000` local in Init — it hoists `lfs 32000` before `bge` and
  breaks the Init match.

## RVL NANDCore (Xenoblade-specific vs ogws)

- Xenoblade `NANDInit` calls external `ESP_InitLib` / `ESP_GetTitleId` /
  `ESP_GetDataDir` / `ESP_CloseLib`. Do **not** paste ogws local `_ES_*`
  IOS wrappers into this TU (they inflate `.text` and break Matching size).
- `nandConvertErrorCode` is **not** the empty-stub ogws shape: retail copies the
  local `errorMap` to a large aligned stack frame, logs selected ISFS codes via
  `sprintf` + `NANDLoggingAddMessageAsync(NULL, msg)`, and on unknown codes
  `OSReport`s then logs `"ISFS unexpected error code: %d"`.
- Error map must include Petari/`ISFS_ERROR_MAXDEPTH` entry
  `-110, NAND_RESULT_MAXDEPTH` between `MAXFILES` and `NOEXISTS` (41 pairs /
  `li r0, 0x29` copy loop). Omitting it shrinks `.rodata` and breaks convert.
- Log buffers are `char msg[128] ALIGN(64)` in nested scopes (forces `stwux`
  `-0x300` prologue). A single unaligned `char msg[128]` yields a small `stwu`
  frame and ~82% match.
- Inlined `nandReportErrorCode` is **not** an empty stub: keep the
  `cmpwi -23` / `bge` / `cmpwi -100` epilogue via

  ```c
  if (result < -23) {
      if (result < -100) {
          i = i; /* match path: delays errorMap[i+1] until after compares */
      }
  }
  return errorMap[i + 1];
  ```

  and on the unknown path `s32 mapped = NAND_RESULT_UNKNOWN;` with
  `mapped = mapped` inside the same nested ifs (constant store is scheduled
  after the compares). `#pragma scheduling off` on the whole function
  regresses badly.
- `.data` layout: `char __NANDVersion[0x60] = "...";` (not `const char*`), then
  `s_currentDir`, shutdown info, `s_shared2Prefix[12]`, `s_nandStringPool[0x94]`.
  Convert loads `char* strBase = __NANDVersion` first and formats with
  `strBase + 0xC8/0xDC/0x110` so relocs target `__NANDVersion` **size 96**.
  Bare string literals leave a `...data.0` reloc with size 0 → stuck at
  ~99.97% despite identical opcodes. `/shared2` must be a 12-byte array
  (reloc size 12); `/shared2/` lives at the start of the 0x94 pool.
  `nandConvertPath`'s `"%s/%s"` must be an 8-byte `.sdata` object
  (`char s_fmtJoinPath[8] = "%s/%s"`); a bare literal is size 6 → 99.9%.
- Retail `.sdata` (0x28) order: version*, `"/"`, `""`, `"."`, `".."`, `"/%s"`,
  `"%s/%s"`, `L""`, `L" "`. Keep each as its own SDA symbol with retail sizes
  (2 / 4 / 2 / 3 / 4 / 8 / 4 / 4). Zero words (`""`, `L""`) go to `.sbss` unless
  wrapped in `#pragma explicit_zero_data on` … `off`. Packing zeros into a
  neighbor struct forces `li`+`addi` field access and blows match/size.
- `nandChangeDir`: duplicate async/sync path arms; empty path is `strcpy(..., "")`
  (not `"/"` — same as `nandGenerateAbsPath`). Inline private check as
  `if (strncmp==0) isPrivate=TRUE; else FALSE;` (not `cntlzw` from `==0`).
  Coalesce `numFiles=0` with `MEMCLR(&stackPath)` at sync entry.
- `NANDInit`: keep `.data` base in `char* strBase = __NANDVersion` and use
  `strBase+0x60/0xa0/0x130` for currentDir / shutdown / OSReport so MWCC emits
  single `addi r3,r31,imm`. `OSRegisterVersion(s_nandVersion)` loads the sdata
  pointer (`lwz …@sda21`). Force-active `s_shutdownFuncInfo` if only addressed
  via `strBase+0xa0`.
- `__attribute__((never_inline))` / `DECOMP_DONT_INLINE` is ignored by this MWCC
  for stopping IPA inlining of `nandConvertErrorCode` into thin wrappers. Use:

  ```c
  #pragma dont_inline on
  s32 nandConvertErrorCode(s32 result) { ... }
  #pragma dont_inline off
  ```

  Without the pragma, Async/callback wrappers grow by ~convert size and the
  split goes over budget.
- `NANDOpenClose`: keep `nandOpen` outlined with `#pragma dont_inline on` around
  it — `-ipa file` otherwise inlines it into every wrapper and blows the
  `0x510` split. Retail OpenClose has only the open/close family (no safe-open).
- Version string for this game:
  `"<< RVL_SDK - NAND \trelease build: Feb 27 2009 10:05:16 (0x4302_145) >>"`.

## RVL NANDCheck / NANDLogging (Xenoblade retail slice)

- **`NANDCheck.c` retail is Async-only:** only `NANDCheckAsync` +
  `nandUserAreaCallback` (no sync `NANDCheck` / calc helpers). Sync donors
  (ogws/Petari/open_rvl) still help for the limit constants and flag ORs.
- **`USER_DIR_LIST` is a non-static `.data` pointer table**; `/meta` and
  `/ticket` are `.sdata` strings, title paths are `.data`. One `NULL`
  terminator (not two).
- **`nandUserAreaCallback` (FULL_MATCH):** prefer
  `if (result == OK || result == NOEXISTS) { … } else { callback(convert); }`.
  GetUsage last arg must be **`arg`** (not `block`) so MWCC emits
  `addi r4,r4,0xac` then `addi r5,r31,0xb0`. State `0x14` is best as an
  inlined/static **`nandCheck(req,reqInodes,home,homeInodes,user,userInodes)`**
  helper (loads color as retail r3/r8/r0/r7/r6/r5). State `0x15` uses
  `(used > limit) ? 0 : (limit - used)` (subfic/subfe/andc). OSReport strings:
  unsized `s_nandUserAreaCallbackName` / `s_nandUserAreaCallbackFmt` (fmt with
  trailing `\0` for size 0x24) so they pack at **0xAC / 0xC4**; sized
  `char[0x18]` 8-aligns to 0xB0 and breaks data. Rename + `st_size` via
  `tools/postprocess_reloc_names.py` (`NANDCheck.o` → `lbl_8055127C` /
  `lbl_80551294`).
- **`NANDLogging.c` retail symbols:** only `reserveFileDescriptor`,
  `NANDLoggingAddMessageAsync`, `asyncRoutine` (global). `prepareLine` /
  `callbackRoutine` are **inlined**; keeping them outlined oversizes the
  `0x5F0` split. `#pragma dont_inline on` around `reserveFileDescriptor` —
  IPA otherwise inlines it into AddMessage (~`0x158` vs retail `0xF8`).
- **Logging buffers:** contiguous `.bss` `s_message[256]`, then rBuf @+0x100 /
  wBuf @+0x200 (retail names them as `asyncRoutine` local statics). Globals:
  `s_message`, `s_fd[2]={-255,0}` (8-byte `.sdata`), `s_callback`, `s_stage`.
  Path/fmt: unsized padded strings → postprocess rename/size to
  `lbl_805512B8` / `lbl_805512D4`. Stage body holds `r31 = s_message`. Keep
  **only r30/r31** NVs: short-lived `s32 z = 0` for rBuf NUL (do not reuse
  incoming `result==0` — that forces r29). Reuse `result = atoi(...)` as line
  number. Stage 6: `n = atoi(...); fd = s_fd[0]; SeekAsync(fd, n*256, …)` for
  retail `mr r0,r3` before `slwi`. Do **not** hold `char* wbuf`/`line` across
  `snprintf`. Calendar field is `cal.month` in this tree.
- **`asyncRoutine` soft-caps (FULL_MATCH via postprocess):** MWCC peeps
  `base+0` msg to `stw r31,0x14(r1)` and folds null/CRLF stores through
  `r31+imm`. Close with `tools/postprocess_reloc_names.py` `NANDLogging.o`
  rules: null-store reorder + titleID `r4` homes, `insert_insns` for
  `addi r6,r31,0`, snprintf/CRLF schedule patches, reloc moves, and
  `pad_text_size=0x5F0`.

---

## mem_expHeap (`libs/RVL_SDK/src/revolution/mem/mem_expHeap.c`)

- **`AllocFromHead_` / `AllocFromTail_`:** MKW-style loop locals (`found`, `foundSize`, `foundMem`, `bAllocFirst`) with `if (!found) return NULL` beats ternary tail-call for MWCC (head/tail both reached **FULL_MATCH**).
- **`AllocUsedBlockFromFreeBlock_`:** Petari `MemRegion` + `RemoveMBlock_` / `InitFreeMBlock_` / `InsertMBlock_`. Retail skips left/right free fragments when `(allocDir==0|1) && !useMarginOfAlign` in addition to the `sizeof(MEMiExpHeapMBlock)+4` threshold (`MEMiExpHeapHead.useMarginOfAlign` at `0x12`). Do **not** cast the threshold to `(s32)` — that forces `cmpwi`; bare `sizeof(...) + 4` (unsigned) yields retail `cmplwi` (last ~0.9%).
- **`RecycleRegion_`:** Petari shape is required: `MemRegion freeRgn = *region` (stack copy), compare merges against the **original** `region` pointer, mutate `freeRgn`, return `BOOL`, then `InsertMBlock_(..., InitFreeMBlock_(&freeRgn), ...)`. Register-only extents (no stack copy) stall ~79% and shrink `.text` by `0x20`.
- **`MEMFreeToExpHeap`:** Petari order — `LockHeap` → `GetRegionOfMBlock_` → `RemoveMBlock_` (used list) → `RecycleRegion_` → `UnlockHeap`.
- **Avoid:** `__cntlzw`/`nor` alignment-mask rewrite for head/tail search — large regression vs `ROUND_UP_PTR`/`ROUND_DOWN_PTR` loops.

---

## ipcclt (`libs/RVL_SDK/src/revolution/ipc/ipcclt.c`)

- **`IpcReplyHandler` IOCTLV relaunch:** Xenoblade retail compares **`__relnchRpcSave == req`**, not `__relnchRpc` (ogws/Petari stub uses `__relnchRpc`). Required for correct relaunch semantics and ~98.5% match with switch on `req->base.fd`.
- **`IpcReplyHandler` (FULL_MATCH):** Default **`Wii/1.1`** + Petari `switch` +
  §17.6 `opword` ACR store; Ack → `__ipcSendRequestSubf`. Do **not** pin
  `GC/3.0a5.2` (see near-misses).
- **`IpcReplyHandler` / `IpcAckHandler` intr type:** Use **`s32 intr`** (not `s16`/`u8`). `s16` makes `IPCInterruptHandler` emit `extsh` into the `bl IpcReplyHandler` and drops that already-matched function to ~99.3%.
- **`IOS_IoctlvReboot` (FULL_MATCH):** Petari relaunch path: **`goto finish`** for busy (`ret=-10`), **`goto err`** cleanup, copy to **`__rpcBuf`**, **`__relnchRpc = (IPCRequestEx*)__rpcBuf`**, then **`req = &rpc->base`** before **`OSInitThreadQueue`** + **`DCFlushRange`**. Locals: **`s32 ret = 0`**, single **`u32 inten`**, trailing aliases **`IPCIOVector* vec = vectors; s32 nOut = outCount;`** (that order). Do **not** init **`rpc`** to `NULL`. Early **`return -10`** regresses.
- **`IOS_IoctlvReboot` queue wrap (subfic):** Shared `__ipcQueueRequest` / `__ipcSendRequest` true arm must be **`(s32)((u32)~0 - sent + queued + 1)`** (not `queued - sent` or `queued + (0 - sent)`). That emits retail **`subfic`/`add`** when inlined into Reboot; `__ios_Ipc2` still colors the same C as **`subf`**. Plain `(0 - sent)` becomes **`neg`** (~99.4%). Full Petari `diff()` macro on the helpers drops the TU.
- **`IOS_IoctlvReboot` send branch:** Call shared `__ipcQueueRequest`, but **open-code send** with **`if (waiting == 0) { … }`** (Petari wrap expr). Shared helper’s `if (waiting != 0) ret=BUSY; else body` becomes **`beq`+`b`** (+4 B) in Reboot; open-code yields retail **`bne`**-skip. `__ios_Ipc2` keeps the shared helper at FULL_MATCH.
- **`__ios_Ipc1`:** Keep as a **`static inline`** call; do **not** paste alloc setup into **`IOS_IoctlvReboot`** (~87.5%).
- **`__ipcQueueRequest`:** Take **`IPCRequest*`** (Petari); store **`(IPCRequestEx*)req`** into the queue; **`IPCiProfQueueReq((IPCRequestEx*)req, req->fd)`**.

---

## GXMisc (`libs/RVL_SDK/src/revolution/gx/GXMisc.c`)

- **Retail split is thinner than full SDK:** Xenoblade's `GXMisc` omits `GXResetWriteGatherPipe`, `GXWaitDrawDone` (as a symbol), `GXTexModeSync`, `GXPeekARGB`, `GXPeekZ`. Keep them out of this TU or `.text` overruns (~`0xA84` vs `0x9E0`).
- **PE/MEM via SDA pointers:** use `extern volatile u16* __peReg` / `__memReg` with `*(__peReg + i)` — not `DECL_ADDRESS` PE/MI maps (`lis`/`addi` vs `lwz@sda21`).
- **MEM PE request counter:** Petari high-half consistency read (`ctrH0`/`ctrH1`/`ctrL` loop, then `(ctrH0 << 16) | ctrL`) before `__GXAbortWait` compare. Index `0x27` / `0x28`.
- **`__GXIsGPFifoReady`:** declare as `GXBool` so the return check is `clrlwi.` (byte), not `cmpwi` on a full `int`.
- **BP immediates:** `(GX_BP_REG_DRAWDONE << 24) | 2` and `token | (GX_BP_REG_PETOKENINT << 24)` yield retail `lis`/`addi`/`oris`. `GX_BP_SET_OPCODE` from 0 forces extra `li`/`rlwimi`. For the second draw-sync write, `GX_BITSET(reg, 16, 16, token)` then `GX_BP_SET_OPCODE(..., PETOKEN)`.
- **`GXPokeAlphaRead`:** `GX_BITSET` for AFMT (bits 30–31) + ZFMT (bit 29) — bare `|= mode & 3` drops the leading `li r0,0` / `rlwimi` and shrinks by 4.
- **`GXPokeBlendMode`:** still set opcode/`RID` `0x41` via `GX_BP_SET_OPCODE` before the PE halfword store (matches retail even though `sth` only writes low 16).
- **IPA for AbortFrame / DrawDone:** write `GXAbortFrame` as `__GXAbort(); ...; GXFlush();` and `GXDrawDone` as `GXSetDrawDone();` + `static inline GXWaitDrawDone()`. Manual duplication of the callee body stalls at ~99.3–99.5% (wrong Chaitin colors); `-ipa file` inlines into the caller and matches retail RA. Do **not** emit a global `GXWaitDrawDone` symbol.

---

## Quick checklist before claiming FULL_MATCH

- [ ] `python3 tools/coop/run.py diff <unit> --symbol <sym>` → 100%
- [ ] Relocations tab clean — 99.7–99.9% with identical insn words → check `@N` vs `lbl_eu_*` (§1)
- [ ] Same `extra_cflags` as `configure.py` for that object
- [ ] No asm / `register rN` / fake `sp[]` in source
- [ ] `python3 tools/coop/run.py size <unit>` PASS (decomp `.text` ≤ split budget)
- [ ] Attempt logged in `docs/evidence/decomp/attempts.jsonl`
- [ ] `tools/coop/targets.json` updated; `configure.py` `Matching` updated for whole TU when appropriate

---

## nw4r `TexMap::mBits` / `BasicPlayer` array bases (US)

- **`TexMap::Get(GXTlutObj*)`:** retail `extrwi …, 2, 17` means `paletteFormat` sits at MSB bits 17–18 — **before** `anisotropy`. Swapping those two bitfields in `lyt_texMap.h` yields `FULL_MATCH` (and restores `b GXInitTlutObj` tail-call encoding).
- **`BasicPlayer` send arrays:** retail `stfs`/`lfs` bases are `mFxSend@0x34`, `mRemoteOutVolume@0x40`, `mRemoteSend@0x50`, `mRemoteFxSend@0x60`. Insert two `u32` words after `mMainSend@0x28` (PlayerParamSet zeros at +0x28/+0x2C). Wrong base → ~99.8% on the tiny indexed accessors.

## nw4r `BasicSound` layout (US Xenoblade)

Stock nw4r offsets are wrong for this DOL. Retail leaves:

| Field | Offset |
|-------|--------|
| `mFadeVolume` | `0x60` |
| `mStartFlag` / `mStartedFlag` | `0x80` / `0x81` |
| `mId` | `0x98` |
| `mExtMoveVolume` | `0x9C` |
| `mInitVolume` / `mExtPan` | `0xAC` / `0xB0` |
| linklist `Prio` node | `0xEC` |

Insert `u32` pad before `SoundParam@0x30` and before fade; flag block includes `mPauseState@0x84` (Xenoblade Pause state) with `mFadeOutFlag@0x83`. `SetInitialVolume`/`SetVolume` Clamp must use `lbl_eu_80669EE8` (1.0) / `lbl_eu_80669EEC` (0.0) — local `0.0f`/`1.0f` → `@N` pool → ~99%.

**`SetVolume` int→double:** inlined `MoveValue::GetValue` pools `@N`; add `snd_BasicSound.o` `pool_patterns` `(MAGIC_HI, MAGIC_LO) → lbl_eu_80669EF0`.

**Vtable:** retail `GetBasicPlayer` is at `+0x24` (not stock `+0x68`). Keep `Update`…`Shutdown` virtual, then `GetBasicPlayer`; demote volume/pan setters + `InitParam` + `SetAutoStopCounter`/`FadeIn` to non-virtual (they are direct symbols in this DOL).

**`BasicPlayer::mId`:** retail `0xD0` (not `0x4`). Keep a pad word at `0x4` so send-array bases stay at `0x34+`; `mRemoteFilter` follows arrays at `0x70`.

---

## Further reading

- [TWW decompiling.md](https://github.com/zeldaret/tww/blob/main/docs/decompiling.md) — inline functions, minor fixes, decomp.me
- [TWW regalloc.md](https://github.com/zeldaret/tww/blob/main/docs/regalloc.md) — regswap causes
- [objdiff README](https://github.com/encounter/objdiff/blob/main/README.md)
- [decomp-toolkit README](https://github.com/encounter/decomp-toolkit/blob/main/README.md)
- [Decompedia — MWCC](https://decomp.wiki/compilers/MWCC)
- [mwcc-debugger](https://github.com/cadmic/mwcc-debugger) — register allocation state
- [Decomp Academy](https://decomp-academy.dev) — interactive matching lessons
- [ppcdis TOOLS.md](https://github.com/SeekyCt/ppcdis/blob/main/TOOLS.md) — sdata2 / IPA floats

## RVL AXFX DelayExp (Wii/1.1 `-O4,p`)

- **`GetMemSize`**: `#pragma scheduling off` is required so `stwu` precedes the
  `lfs`/`fmuls`/`fctiwz` chain (same family as StdExp GetMemSize, but here the
  body is simple enough that scheduling-off reaches 100%).
- **`.sdata2`**: first-use emits `32,0,1,128,0.95` (0x14). Retail is 0x18 with a
  trailing zero pad on the `0.95` slot — `pad_sdata2_size=0x18` plus
  `symbol_sizes=(float_8066BE30, 0x8)` in `postprocess_reloc_names.py` (§17.6).
- **Init/Settings/Shutdown**: unroll the three-channel free/alloc paths (for-loops
  undersize `.text` by ~0x1A8). Init success sets `active |= 2; active &= ~1`.
- **`Callback` / `__InitParams` / `SettingsUpdate`**: soft-caps on GPR coloring
  (`_savegpr_20` vs lower, mask in r30 vs r31, iirGain temp r5/r6) while
  semantics match; keep shared `>> 7` after the busIn branch join.

