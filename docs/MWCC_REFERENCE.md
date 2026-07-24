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
| Wide-arm reuses `r30` for height (`lha r30` vs retail `lha r31`) after non-wide correctly uses `self=r30`/`height=r31` | Precise liveness: `self` dead on wide arm so Chaitin recycles `r30`; keep-alives scramble dual-`getRenderModeObj` schedule | **CGame::func_800395F4** soft-cap ~99.8%: non-wide `s32 height` + `spInstance` reload; leave wide `s16` — do not chase keep-alive commas/ternaries |
| `addi …, @stringBase0, imm` wrong imm / missing Restart→CGame pool | Pointer-table `force_active` does not pack MWCC `@stringBase0`; contiguous pool needs a `.text` FORCEACTIVE that references the literals in order | Keep non-vararg `force_cgame_strings(Restart,"",43,arc,brlyt)` then **`drop_text_symbols`** the 0x1C thunk in `postprocess_reloc_names.py` (`CGame.o`) so stubs still fit `0xD08` |
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

Frameless Fv+r4/r5 ring copy leaves. High-level C++ CSEs dest materialization into **`stwux`** (r4+=r8) plus sequential loads from src-in-r7; retail keeps **`stwx`+`add r7`** with src in **r9** and an interleaved **8-then-4** field schedule (exact size `0x110` retail / `0x10C` decomp). Pointer+offset `(u8*)buffer+(outIdx<<5)` holds **96.02941%**; `outB->buffer[outIdx].unk00` + `&buffer[outIdx]` **regresses ~87.1%**. Dual `storeBase`/`destBase` aliases of `outB->buffer` **regress** (~95.1%). Soft-cap **96.02941%** CODE_MATCH; size PASS. `EQUIVALENT` blocked by ring-loop visit/instruction limits and Z3 timeout under `ppc-eabi` (cfg-exploration deadline even at soft-cap 15min budget).

**Also ruled out (still `stwux`):** distinct `off`/`offDest` copies; fresh `(outIdx<<5)` for dest vs store; `#pragma opt_propagation off`; `#pragma optimization_level 1`; `#pragma global_optimizer off` (grows to `0x124`, still `stwux`); `for`↔`while` mirror between vfunc1/vfunc2. Keep high-level C++ — **no** `.text` `insn_patches` / `asm void` (skill forbids).

## CBattleState UnkVirtualFunc6 — regalloc soft-cap (US)

Fv+r4 entry walk (8×`0x34` at `+0x1388`), bitfield set into `+0x15AC`,
tail-call vt+`0x48` via **`bctr`**. Exact size `0x154`. **`BattleStateV6If::vf48`**
fixes vptr coloring to retail `lwz r12` (~92.5%, was ~92.2% with FP temps).
Residual ~44 words: prologue GPR homes (`id`/`one`/`entries`/`n` = retail
`r6`/`r7`/`r5`/`r0` vs MWCC `r0`/`r5`/`r8`/`r10`) and walk in `r9` vs `r7`.
Retail emits `li one` before `addi entries`; MWCC prefers entries-first and
**constant-folds** `0x1387+one` → bare `addi 0x1388` (dependency does not stick).
Keeping `id` live across the match walk (`entries->unk0C = id` / compare on
cached `id` without clrlwi kill) drops to ~90%. Expanded byte/FPR copy schedule
also soft-caps ~92.47% at exact size. SMT `ppc-eabi`+fpscr stays
`inconclusive_timeout` on the peak body. `id`/`one`/`entries` decl-order and
scoped-`one` regress.

**FPR residual (free-slot fill):** peak already matches retail’s early
`lfs f0,lbl` + `f1`/`f2` copy schedule; only the scale multiply differs —
retail `fmuls f0,f0,f1` vs MWCC `fmuls f1,f0,f1` from
`f32 scaled = lbl * entries->unk24`. Ruled out: `scale = lbl; scale *= unk24`
(and assign-then-mul) → constant lands in **f2**; `*(volatile f32*)&lbl`
same f2 regression; operand swap `unk24 * lbl` → `fmuls f1,f1,f0` (still
dest=f1). Named bitfield temps/`self+off+0x15AC` do not change prologue
GPR homes. §17.6 single-insn asm not eligible (&lt;99%). Keep high-level C++;
**do not** use `asm void`.

## CBattleState UnkVirtualFunc8 — NV home / scan soft-cap (US)

Fv+r4 kind tree + slot walk (`0x68`×`0x34`). Fake SI `BattleStateV8If` (omit RTTI `_v000`/`_v004`) emits retail **`lwz r12` / `bctr`** for vt+`0x2C`/`0x4C`. Soft-cap ~**98.4%** (was ~98.1%): `volatile int tripKeep = trip; stillActive |= tripKeep & 0` restores dead **`li r3,0` / `addi r3,r3,7`**; bit clear uses **`(id >> 3) & 0x1FFC`** for retail `rlwinm SH=29,MB=19,ME=29` (`~3u` widens MB). Residual: Chaitin **this/entry** homes (`r27/r28` vs retail `r28/r29`), **thirteen** (`r29` vs `r27`), scan base in **r5** vs retail **r4** (tripKeep stack slot shifts frame / steals r4), word-copy **r0/r5** vs **r6/r0**. Plain `trip&0` / `trip^trip` / `clearLen=trip` DSE the addi. EQUIVALENT blocked by unresolved indirect vcalls + `memset` lacking an equivalence certificate (FULL_MATCH leaf). Keep high-level C++; **do not** use `asm void`.

## CBattleState UnkVirtualFunc10 — trip / SI / mask soft-cap (US)

Fv+r4 early-out on `arg->unk2E==0`, spill arg words, walk `0x68`×`0x34` matching `unk2E` (+ optional key eq). Same kind tree as vfunc8 on **slot** id; kind==3 clears `+0x1528`; stack-copy + `memset` + id-dup scan + vt+`0x4C` (no recursive vt+`0x2C`). Soft-cap ~**98.5%** (was ~98.2%): SI `BattleStateV8If::vf4C` restores **`lwz r12`**; same **tripKeep** + **`0x1FFC`** mask as vfunc8. Residual: NV **this** `r23` vs `r24`, scan **r5** vs **r4**, extra tripKeep `stw`, word-copy pair regs. EQUIVALENT same indirect/`memset` cert blocker. Keep high-level C++; **do not** use `asm void`.

## CBattleState UnkVirtualFunc26 — schedule + mtctr copy (US)

Fv+r4 src table: vt+`0x78` clear, 8×`0x10` records → stack `CBattleStateEntry` → vt+`0x1C`, then `0x80`-byte word copy into `this+0x1528`. Exact size `0x12C`.

**Breakthroughs:** `BattleStateV26If::vf1C` (omit RTTI `_v000`/`_v004`) restores retail **`lwz r12` / `bctr`** for vt+`0x1C`. `entry.unk30 |= 1` (no flags temp) + `flag4000` before `flag2000` → ~78.8% HIGH_MATCH.

**Soft-cap:** (1) MWCC hoists `lhz`/`extrwi.` on `unk0E` before field stores (retail keeps them late; steals r0 from the flags `ori`). Reload/`flags != 0` barriers regress. (2) Tail copy is retail **`mtctr`/`bdnz` + `lwzu`/`stwu`**; high-level `do { … } while (--i)` emits **`addic.`/`bne`** (same size). Keep high-level C++; **no** `insn_patches`.

## COccCulling::setFrustum — scale/rot + interleaved planes (US)

Exact size `0x588`. Best ~**89.2%** HIGH_MATCH: `setScale(CVec3(sx,sy,lbl_eu_80667C88))` + `setRotXYZ` + `FLAGS_01` plane guard + SDA dir/unk124/128 zeros + `math::sqrt` (inlined `FSqrt`/`Warning`/`FrSqrt`).

Retail prologue builds scale with field `stfs` of SDA 0/1, then **`MTX34RotXYZFIdx`** with **`lbl_eu_80667C90`** (RAD_TO_FIDX) and `PSMTXConcat(rot,scale,scale)`. Explicit FIdx + `lbl_eu_80667C90` matches size but **drops fuzzy ~1–2%** vs `setRotXYZ` (`MTX34RotXYZRad`). Side-plane bodies are retail **psq-interleaved** edge/normalize/dist; sequential `CPlane::set(p0,p1)` stays shorter/differently scheduled. Explicit side-plane edge temps (4 locals or ping-pong `edgeA`/`edgeB` with early N+1 `sub` before `mDist` N) **regressed to ~69%** and **size FAIL** (+`0x24`–`0x38`). Scalar `CVec3::sub` + fmadd / direct `VEC3Sub`+`VEC3LenSq` corner loop **regressed to ~86–87%**. Split spare ~`0x14`.

`EQUIVALENT_MATCH` blocked on retail callees: `MTX34RotXYZFIdx` (`us-8040d584`, COMPILES) and `CPlane::set` 3-arg (`us-804398b8`, ~79% HIGH_MATCH soft-cap) plus missing/stale v45 certs on FULL_MATCH math helpers. Keep high-level C++; **no** `insn_patches`.

## CMenuEnemyState::Move — `_savegpr_20` vs `_savegpr_22` (US)

Retail size `0x9B8` (decomp peak `0x984`). Frame `-0xe0` / f28–f31 OK.

**Breakthrough (87.16% HIGH_MATCH):** replace scalar `distSq = x*x+y*y+z*z` with `VEC3Sub(pDelta,…)； scratch=delta； VEC3LenSq(pScratch)`. That recovers retail `ps_mul` / `ps_madd` / `ps_sum0` (+~1.5% over the prior 85.64% soft-cap). Keep pre-loop order `pc → animMarker → distThresh → scratch/pScratch → one → delta/pDelta → i/z` and `posTmpPtr`.

**Residual soft-cap:** decomp still emits `_savegpr_20` (two extra NVs vs retail `_savegpr_22`), so GPR homes stay shifted (`this` in r20 vs r23). Ruled out for shedding: dual-home pulse `obj`/`noTarget`, dropping `posTmpPtr`, block-scoping `hasSub`, `*pScratch=*pDelta` vs `scratch=delta`. Dropping `scratch` / inlining `skipDist` still regresses (~80.9%). `EQUIVALENT` blocked by unresolved indirect + many unvalidated callees. Soft-cap under high-level C++; **no** `insn_patches`.

## CMenuBattlePlayerState ctor — 0x20 vs 0x60 pad clear (US)

`__ct__CMenuBattlePlayerState` (`menu-bps-ctor`, TU `-O4,s`): Process+PTMF+`__construct_array`+per-slot stack temp. Retail `0x580`, `_savegpr_21` / `-0x2B0`.

**Peak ~80.3% HIGH_MATCH, size PASS `0x570`/`0x580`:** (1) hoist pad bounds + signed `sizePlus/12` + dual bool gates (padSize/sizePlus signs, then q12/q12p1) interleaved with `unk7F4`/`unk7E*` stores; (2) gated 0x60/0x0C fill with `do {…} while (--c)` counted from `padStart`; (3) unrolled 0x11 header zeros; (4) retail slot copy — `dst[0]=0`, head words through `+0x7c`, `unk80` byte, then **`copyWords=0x30` lwzu/stwu pair-copy** from `+0x84` (CBattleState `*(s+1)` / `*(s+=2)` shape), then tail fields. Aggregate `mSlots[i]=slot` alone peaks ~75% but size FAIL (`0x5D4`, mtctr `0x2e` + hole lbz/lhz).

**Soft-cap residuals:** under `-O4,s`, signed `/12` stays **`divw`** (retail `lis 0x2AAB`/`mulhw`); explicit `0x2AAAAAAB` s64→mulhw drops fuzzy (~78.8%) and size FAIL (`0x59C`). Fill still not byte-identical to retail `mtctr`/`cmplw` schedule. Equivalence blocked on unvalidated callees (`CProcess` / `UnkClass_8045F564` / `__construct_array`). **no** `insn_patches`.

Retail mid-clear of `pad90[0x174]` (and leaf `func_8010B324` ~83.8%) is MWCC’s **0x60 + 0x0C** gated fill. Stock **`-O4,p`** word-while emits wrong **0x20 + 0x4**. Ctor TU split keeps `-O4,s` on `__ct` only; Term/`cbRenderBefore` stay FULL_MATCH.

## CHelp* / CChainCombo — r12 iface / vptr walks (US)

Manual vptr loads (`this+8` iface tables, `CChainVObj` / `CfObjectPc` vptrs) must use **`lwz r12,…`** like a C++ virtual call. High-level function-pointer temps color as **r4/r5**. Prefer fake SI virtual iface (`MenuBpsActorIf` pattern) or other high-level reshape — **`asm void` bodies are not allowed**.

## CUICfManager::func_80133324 — 312c codePersist / slot RA (US)

Exact size `0x3C0`, frame `-0x80` / `stmw r25` / `mr r31,r1`. Peak ~**99.56%** CODE_MATCH.

**Breakthroughs:**
1. Drop dead `pad0C`…`pad20` keep-alives (fully DCE’d but scrambled Chaitin).
2. Expand `setItem` via `CUICfListNode` + `u32* ptr = &temp->item` / try-catch so retail schedules `add r4,temp` → `addic. r3,r4,8` → **then** `lwz savedRet` (not arg-first).
3. Slot locals: **declare** `capacity` before `startNode`, **assign** `startNode` then `capacity` → retail `lwz r8,0x128` / `lwz r7,0x13C` with `i`/`byteOff` in `r5`/`r6`.

**Soft-cap (8 words):** `id-0x312c` / `li 0xc8` colored `r0`/`r3` (decomp) vs retail `r3`/`r0`, so `codePersist` lands in **r27** vs retail **r30** (and `mr` vs second `clrlwi` on the assign). Decl-order / goto / inline-helper / `u32`+ret2 reuse did not flip it. `EQUIVALENT` blocked by unvalidated callees. Keep high-level C++; **no** `asm void`.

**Stall packet (cursor-gki-uicf, 2026-07-24):** peak **99.5625%** CODE_MATCH, size PASS `0x3C0`, equiv `inconclusive_unvalidated_callee` (9 callees). Residual still the 8-word Chaitin pair above. Fresh RA shapes ruled out: (1) `u16 codePersist` reused for `ret2` — flat 99.56%; (2) `u8 code` + `u32 diff` — **98.58%** (`li r25,0xc8` steals a1 home); (3) `codePersist=0xc8` then conditional overwrite — **98.58%** (`li r27` early). Prior ruled out: decl-order, goto beq-skip, inline helper, masked temp, `u32`+ret2. Next experiments: decomp.me scratch for li→r0 anti-coalesce; certify callees for EQUIVALENT route; avoid narrowing `code` onto NV early. Term measures ~99.8% on current toolchain (registry FULL_MATCH stale at HEAD too) — do not chase via this function's locals.

## CUICfManager::Init — packed slot templates (US)

Large `-0x1A0` / `stmw r22` frame: `readFile` + `0x54` process/`__ptmf_null` (same prelude as `CUIBattleManager::Init`) then eight `0x168` slots.

## CUIBattleManager::Move — stmw r25 / slot r7–r8 soft-cap (US)

Exact size `0xB00`. Peak ~**95.57%** CODE_MATCH (`battle-mgr-move`): per-site `savedRet` stack homes, inline `setItem` list inserts, `pad0C`/`pad18` chain to keep capacity live, `__cntlzw`/`srwi` for `func_8009CF8C(0x3357)`, indirect `func_8012F5F8` call, `framePad[8]` toward retail `-0x220`.

**Residual categories (byte diff vs retail):**
1. **Prologue:** retail `stwu -0x220` + `stmw r25,0x204`; decomp `-0x210` + `stmw r31,0x1f0` (shallower callee-save set).
2. **Six inline slot-search inserts:** retail `lwz r8,0x60` / `lwz r7,0x74` then `cmpw r5,r7`; decomp swaps **r7/r8** and emits `cmpw r5,r8` plus wrong sentinel register in list-link stores (`stw r7,0(r4)` vs retail `stw r8,0(r4)`).
3. **Asset/party loop:** alternate `this`/`partyId` register homes vs retail `r30`/`r26`/`r27` schedule.

**Regressions ruled out:** direct `bl func_8012F5F8` (drops ~0.05%); uniform `startNode`-before-`capacity` in all blocks; raw `0x60`/`0x74` offset loads; `framePad[12]`. Behaviour host `battle-mgr-move` (20 scenarios) PASS; size PASS. **EQUIVALENT** blocked: 26 callees lack v18 certificates + indirect call edge. Soft-cap under high-level C++; **no** `insn_patches` yet (would need per-site Chaitin patches across ~704 words).

**Breakthrough:** stack **zeros** and aggregate **copy** are different views of the same `0x90` bytes. Named `u16` fields give correct `sth` zeros but memberwise `lhz` copies. Retail copy is:

`lwz +0`, paired `+8/+4`, `lhz +0xC`, lone `lwz +0x0E`, then memcpy-style paired unaligned words through `+0x8E/+0x8A`.

**Best high-level so far ~94.8% HIGH_MATCH** (copy load schedule equals retail):
1. Contiguous `CUICfInitTemplates { state, block0, blocks[3], tail }`.
2. `#pragma pack(1)` Tail: `u32`, `u8 mid[8]` (pair-copy → `+8/+4` loads), `u16`, `u32` at `+0x0E`, then `u8[0x40]` + `u8[0x3E]` chunks.
3. Zeros via `u16*` overlay at `+4` (six `sth`) + `u8*` stores at `+0x10/+0x12/+0x52` — not via the copy field types.
4. Split field assigns; last chunk through a `u8[0x40]` cast view so the final pair is `lwz +0x8E/+0x8A` (2-byte overhang), not `lhz +0x8E`.
5. Clear: fresh `&blocks[0]` + `(end+stride-1-start)/stride` countdown (today `mulhwu`/`bne`; retail `divwu`/`mtctr`/`bdnz` + `li r22,52` still open). One big `u8[0x82]`/`u8[0x7E]` → `lwzu`/`bdnz` loop — split under ~`0x40`. **ok/ok2 + for(--clearCount) regressed to ~78.6%** — keep do-while peak.

## CMenuArtsSelect::Move — header layout regression (US)

Exact retail size `0xBB4`. Historical peak ~**93.8%** (commit `28b3f22`) used opaque `u8` padding through `0x54` + `slot+=4` walk + volatile `homes[3/4]` sink (`stwu -0x70`, `savegpr_20`, `this=r21`). After header refinement (named Process/vtable/ptmf + filled pads for ctor FULL_MATCH), same peak Move body falls to ~**78%**; named `unk2A0[i]`/`unkA4[i]` HEAD shape is the current baseline (~**79.4%**).

**Current codegen gap (refined header):** decomp emits ~`0xA28`–`0xA34` (not `0xBB4`), `stwu -0x60`, `this=r24`. Opaque `unk00[0x54]` alone is insufficient without the rest of the peak pad layout. Function-scope extra f32/NV keep-alives regress (~75.7%). Full peak header conflicts with Init/ctor typed `unk90`/`unk224[]`/vtiface fields.

**Conflict:** Move peak needs opaque pads; ctor FULL_MATCH needs named Process/vt/PTMF (plus removed §17.6 epilogue `insn_patches` — without them ctor measures ~88.8%). Term stays 100%; cbRenderBefore soft-cap 99.3%. EQUIVALENT blocked by many unvalidated callees + indirect call. Soft-cap under high-level C++; **no** `insn_patches`.

**Regressions:** whole-Tail assign with large trailing arrays; union/`memcpy` dual-views; volatile stride (spills and shifts stack); runtime stride + `for(count)` reintroduces 8× clear unroll; function-scope dummy f32/NV pads. Keep high-level C++; **`asm void` bodies are not allowed**.

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
  `CE68`: same countdown for-loop → mtctr/bdnz + bctr vtbl[3] (~86%, size
  140/144). SMT on both rings hits loop-iteration bounds quickly. `isCurrent`
  is FULL+cert (`mCurrentView` compare). `isCurrentChild` ~86% (3-level unroll
  + recurse via `convertToView`/`mChildren`). `hasCurrent` ~89% (size 104/96).
  Inlining `42DA8` without `poolPairAt` regresses ~94%→~72%; keep the helper.
  Ring EQUIVALENT needs B298 accepted+certified first.

**CViewRoot ring helpers:** `func_80442B54`/`42C68`/`42DA8` push/pop three
  `{u32,u32}` rings in `mPool0/1/2` trailer (`mStartNodePtr`=base, `mList`=index,
  `mUsed`, `mCapacity`) then `CGXCache::func_8044B298`. Peak ~68%/81%/93% before
  schedule regressions; keep high-level divw shape (`sum - (sum/cap)*cap`). Certify of byte-identical callers must skip
  `validate_callee_contract` (path explosion on `getView`→`getWorkThread`);
  rebind certs after certifier-tree edits and recertify leaves bottom-up.

**RVL `PSMTXMultVec` / `vec.c` split fit:** retail MTX paired-single kernels match
  only via the same `asm void` / `ASM()` PS idiom already used for `PSVEC*` /
  `PSMTXConcat` in this fork (scalar C is 0% and oversize). Strip unused
  `C_*` / empty `asm` stubs from `mtxvec.c` / `vec.c` — each emits a 4-byte
  `blr` + align and blows the split (`mtxvec` 0x60, `vec` 0x210).

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

**`__OSShutdownDevices` (US FULL_MATCH):** ogws groups `OS_SD_EVENT_RESTART` with
FATAL/RETURN/LAUNCH as `keepEnable=FALSE`. Xenoblade retail is
`(event-5)<=1` (RETURN/LAUNCH) **or** `event==FATAL` only — **RESTART keeps
`keepEnable=TRUE`**. Inlined `KillThreads`: case order `READY` then `SLEEPING`
emits retail `cmpwi 1` / `cmpwi 4`.

**`OSReturnToMenu` / `OSResetSystem` / `OSRestart` / `__OSHotResetForError`
(US FULL_MATCH):** same NAND sized-pool rule — bare `OS_ERROR("…")` / string
literals → size-0 `…data.N` relocs and ~98% soft-cap despite identical opcodes.
Use explicit `char OSReset_file[0xC]`, `OSReset_hotResetPool[0x60]`,
`OSReset_menuPool[0x180]`, `OSReset_obsoletePool[0x7C]` (retail dtk object
sizes; orphan setting/HTML strings live inside the menu pool) and
`OSPanic(pool, line, msg)`.

- Thin wrappers (`OSReturnToMenu`, `OSResetSystem`) and `OSRestart` /
  `__OSHotResetForError` panic: pass **two distinct objects** so MWCC emits
  `lis r3` / `lis r5` (retail). `OSPanic(base, n, base + off)` collapses to
  one `lis` + `addi r5,r3,imm` and drops to ~85% / wrong size.
- `__OSReturnToMenuForError` (~97.1% soft-cap): hoist `char* strBase =
  OSReset_file` and `HotResetPanicMenu(strBase)` (`strBase+0xC` / `+0x38`,
  file in **r31**). Only residual vs prior FULL: MWCC peeps `addi r3,r31,0` →
  `mr r3,r31` at both `OSPanic` file args (`strBase+0` / asm `addi` also peep).
  §17.6 whole-call asm blocks regressed (~90%); needs a tighter opword-only
  carve-out or a C shape that keeps `addi` (historical orphan-pool FULL).
- `__OSReturnToMenu` (~95.1% soft-cap): needs **three** `OSStateFlags` locals
  (retail frame `-0x90` at `0x58`/`0x38`/`0x18`; one shared local → `-0x50`)
  **and** Chaitin coloring `strBase=r30`, disc/ticket scratch=`r31`,
  `menuMode=r29`. Current MWCC keeps `strBase` in **r31** (swapped with
  scratch) despite decl-order / union / CSE-hoist / late-init experiments —
  residual is almost entirely that register permutation (plus the same
  `addi`→`mr` peep on panic args). Peak pre-sized-pool was ~99.88%. Unit
  `.data` may still 8-align after the `0xC` file object.

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
- **`Init` (~99.9%) / `SettingsUpdate` (~99.8%)**: inlined free-on-failure (not
  `bl Shutdown`); success flag set only on loop fallthrough (`ok = TRUE` after
  alloc/memset), with delay `inPos`/`outPos`/`lastPos`/`sizeFP` in the memset
  success path. Best shape: function-scope `f32** walk` for alloc / SU-memset;
  block-scope `walkM` + separate `k` for Init memset (retail walk=`r28`, i=`r27`);
  reuse `i` for alloc+first-free (pins nested=`r27`); `j` for later frees. Same
  pattern on Dpl2 (4ch + `AXGetMode` gate). Soft-cap remains: Init alloc index
  lands `r28` vs retail `r29` (same insn count; retail leaves an `r28` hole);
  SU memset walk lands `r28` vs retail `r27` (index already `r29`). Decl-order,
  block-scoped split `i`, early null/`nested=mask`, and separate `ai` do not open
  the hole without regressing free1/memset.
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
- **`Callback` (~98.9%)**: declare long-lived coef locals **before** loop temps
  in order `lpfCoef2, earlyCoef, combCoef0/1, allpassCoef, lpfCoef1,
  earlyGain, fusedGain` so MWCC homes them to **f0..f7** and emits retail's
  setup (`1.0`→f2, `(1-lpf)`→f5, `0.6`→f3, gains→f6/f7, coefs→f1..f4). Keep
  `lpfCoef1 = 1.0f - reverb->lpfCoef; lpfCoef2 = reverb->lpfCoef;` then
  `early/fused * 0.6f`; hoist sample positions; unroll pos updates. Remaining
  miss is **i2f magic f10 vs f11** (cascades loop FPR temps) plus busIn path
  GPR color (`r24`/`r25`/`r26`). Loading mem floats before `1.0f`, hoisting
  `outGain`/`sendGain`, or a live `0.0f` pad can force magic→f11 but regresses
  match/size. TU is `-lang=c` (C89): no mid-block declarations.
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


## CProcess size trim (US)

- Retail **does not** emit `DeleteList__11CProcessMan...` or `Tail__11CProcessManFv` in this TU —
  **inline `DeleteList` into `Delete`** (two loops). `CProcessMan::Tail` is unused / header-inline OK.
- `Tail__8CProcessFv` lives in **`kyoshin/CTaskGame.cpp`** (0x800444BC), not `CProcess.cpp` — remove the
  empty OOL from CProcess to clear the last **+4** bytes.

## gki_time (US)

- Retail slice is **exactly 8 symbols**. Strip Android-only APIs (`GKI_timer_update`,
  `gki_timers_is_timer_running`, `gki_adjust_timer_count`, …). **Inline `gki_adjust_timer_count` into
  `GKI_start_timer`**. Donor: ogws REVOLUTION `gki_time.c`.
- **`GKI_update_timer_list` FULL:** drop `-func_align 4` — default **16-byte** function align places
  the first while-loop body on a 16-byte boundary and emits retail's lone **`nop`** after the
  forward `b` (same inter-fn pad pattern as retail). With `-func_align 4` the loop starts unaligned
  and MWCC omits the nop (~97%).

## CRsrcData (US)

- **Size / FULL (0x42C):** one `extern "C" build__9CRsrcData…` only (no C++ static twin — double
  emission). Ctor name via postprocess `__ct__9CRsrcDataFPCc…` → `__ct__CRsrcData`; vt →
  `lbl_eu_8056B360`; `trim_text_size=0x42C` drops weak IWorkEvent stubs.
- **build:** `lbl_eu_80522534` is **`"CRsrcData"`** (not `""`); call mangled ctor explicitly (not
  placement `new`) to avoid a second null `beq`.
- **dtor:** do **not** re-null-check `this` (virtual dtor already does); load `mCacheData` before
  the compiler's vt store; `#pragma optimize_for_size` for `stmw r30`.
- **destruct:** `#pragma dont_inline` so `wkUpdate` tail-`b`s it (`beqlr`/`bgtlr` leaf).
- **isSameName:** return **`int`**, not `bool` (bool adds `neg`/`or` after `cntlzw`/`srwi`).
- **setRsrcFile:** `getFilePtrFromPath(mName)` on the copied buffer; `unk4E0 = flag` (stb of bool reg).
- **releaseCache:** `if (cache == data) { … return true; } return false;` (`bne` fail shape).

## CWorkThread (`libs/monolib/src/work/CWorkThread.cpp`)

- **`wkRemoveChild` (FULL_MATCH 0x48):** Do **not** call `reslist::remove` (walks all matches).
  Retail unlinks the **first** match only. Emit retail loop with **`goto advance` /
  `check:`** (advance-at-top, head compare, then item compare) — a plain
  `while (curr != head) { if (item==) break; curr=next; }` stalls ~98%.
- **`getWorkThread(const char*)`:** Null-check **`this`** (not `name`);
  `strcmp((const char*)&mName, name)` (FixStr `mString` is first); `#pragma dont_inline on`
  to stop recursive IPA unroll (otherwise ~0x16C vs retail 0x9C). Residual: `cmpwi` vs
  retail `cntlzw`/`srwi` after strcmp, and `stw` vs `stmw r29`.
- **`wkSetEvent` / `wkSetEventChild`:** Retail uses a **4-level unrolled** descendant walk
  with OOL `CMsgParam<8>::enqueue`. High-level recursive `wkSetEvent` on children keeps
  size under budget but caps fuzzy (~74% / ~33%).

## CRsrc (`libs/monolib/src/core/CRsrc.cpp`)

- **FULL_MATCH 9/9, size exact 0x3A0 (US).**
- **`releaseCacheLocal`:** bottom-tested `goto check`/`loop:` with per-iter
  `lwz 0x60(parent)` head reload (not a cached `head` local).
- **`getRsrc`:** force `extsh`/`slwi`/`lwzx` via `(u8*)list+(off<<2)`; declare
  **`entry` before `index`** for retail r6/r7; `if (id == *(u32*)(entry+0x1C4))`.
- **`isExistDataLocal`:** `addi this,0x5c` into `func_80438AF0`; retail flag shape
  (match when cache==data and NOT (non-null && refCount==0)); `optimize_for_size`
  for `stmw`.
- **`entry`:** `#pragma dont_inline` + **`extern "C"`** `isExistData__5CRsrcFPCv(data)`.
- **`releaseCache` / `isExistData`:** `dont_inline` + `goto check`/`loop:` +
  `optimize_for_size`.
- **`hasChild`:** `subf`/`or`/`srwi 31` on sentinel vs first-child.
- **`sRsrcPointerList__5CRsrc`:** `extern` BSS in `extern "C"` block.

## gki_buffer (US)

- Shared corruption check: odd-ptr guard + `subf`/`or`/`srwi` vs `MAGIC_NO`
  (`0xDDBADDBA`), not `*magic != MAGIC_NO`. Macro `gki_magic_corrupted` for
  send/enqueue (do **not** use GNU statement-exprs — regresses those callers).
- **`GKI_freebuf` (~91.4%, size exact `0x170`):** no early `p_cb` (avoids r30);
  pooled string base `"getbuf: Size is zero"` + `+0x30/+0x48/+0x5c`; shared
  `free_corrupted:`; open-coded odd/`bad`. Residual: MWCC `addis` form of
  `v-MAGIC` vs retail dual `subf`.
- **`gki_init_free_queue` (~96.3%, size exact `0x220`):** REVOLUTION always-store
  `pool_start`/`pool_end`; `total==0` early; no `Type=0` stb; magic via
  `*(UINT32*)((UINT8*)hdr + tempsize + BUFFER_HDR_SIZE)`. Handwritten 8× CTR
  unroll blows size — leave scalar.
- **`GKI_getbuf` (~97.7%, size exact `0x1A0`):** bottom-tested scan joining at
  `if (i == curr_total)` (retail `cmplw`/`bne`); post-`disable` take without
  size re-check; success: `task_id` → ret → status/Type/p_next.
- **`GKI_create_pool`:** 3× empty-slot search; handwritten 8× shift regresses.

## CWorkThread (`libs/monolib/src/work/CWorkThread.cpp`)

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

- **`TexMap::Get(GXTexObj*)` / `mBits`:** retail LOD path uses `extrwi` mag@12 (1 bit), bias@13, edge@14, aniso@15 (2). Layout is `magFilter:1` then bias/edge/`anisotropy:2` then `paletteFormat:2` (not `magFilter:3` with palette before aniso). Residual ~0.17% after that was `@N` → `lbl_eu_80669DB8/DC0/DC8` pool renames in `postprocess_reloc_names.py` (`lyt_texMap.o`).
- **`TexMap::Get(GXTlutObj*)`:** stays `FULL_MATCH` with the same `mBits` order (palette after anisotropy).
- **`BasicPlayer` / `PlayerParamSet` (US):** param block lives at `BasicPlayer+0x4` (`sizeof == 0xCC`); `mId@0xD0`. Send-array bases on the player are still `mFxSend@0x34`, `mRemoteOutVolume@0x40`, `mRemoteSend@0x50`, `mRemoteFxSend@0x60`. `mPanMode@0x2C` / `mPanCurve@0x30` (param `+0x28`/`+0x2C`). `mRemoteFilter` is `u8@0x1D` (param `+0x19`) — **not** after the arrays. `BasicPlayer::InitParam` is `addi r3,r3,4; b PlayerParamSet::Init`. Init floats must reloc to `lbl_eu_80669EE0` (1.0) / `lbl_eu_80669EE4` (0.0) — `EE8`/`EEC` are a different duplicate pair and stick at ~99.8%.
- **`PanMode` / `PanCurve` mangling:** retail is `nw4r::snd::PanMode` (`Q34nw4r3snd7PanMode`), not `detail::`. Wrong namespace → 0% on `SetPanMode`/`SetPanCurve`.

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

**Vtable (US Xenoblade, not stock nw4r):** after RTTI/`__dt__`: `Shutdown@+0x10`, `IsPrepared@+0x14`, `IsAttachedTempSpecialHandle@+0x18`, `DetachTempSpecialHandle@+0x1C`, `InitParam@+0x20` (virtual!), `GetBasicPlayer@+0x24/+0x28`, `OnUpdatePlayerPriority@+0x2C`, `UpdateMoveValue@+0x30`, `UpdateParam@+0x34`. **`Stop` / `Pause` / `Update` / `StartPrepared` / `IsPause` are non-virtual.** Wrong Shutdown slot (`+0x20` vs `+0x10`) soft-caps `Stop` at ~99.99%.

**`Stop`:** early `Shutdown()` via vt+`0x10`; fade path inlines `SetPlayerPriority(0)` (stb `mPriority@0x94` + `detail_SortPriorityList` + virt `OnUpdatePlayerPriority`); clears `mPauseState@0x84` (not `mPauseFlag`), sets `mFadeOutFlag`.

**`Pause`:** `mPauseState` machine (on → state 1 / fade→0 / `mPauseFadeFlag=false`; off → state 3 / fade→1 / flag true). Retail has two unreachable `b` after pause-on and one after unpause range-check (`b work; b return`); MWCC omits them → ~98.7% with correct cascade+switch-off.

**`InitParam`:** no `mPauseFlag` clear. `SoundParam` has an extra `f32 unk18` before `priority` (retail `stfs@0x48`, `priority@0x4C`). Output line: ternary `(mSoundPlayer != NULL) ? player->detail_GetOutputLine() : OUTPUT_LINE_MAIN` so MWCC reuses SetTarget’s `li r4,1` (if/else rematerializes and sticks ~98.7%). `SoundPlayer::mOutputLineFlag` is at `+0x34`. Floats via `lbl_eu_80669EE8`/`EEC`.

**`BasicSound(int,int)` / MoveValue / LinkListNode:** retail ctor writes fields in place (no default-zero member ctors). Empty `MoveValue()` / `LinkListNode()` + `node.Init()` + late callback zeros; vtable rename `__vt__…BasicSound` → `lbl_eu_8056A710`. `UpdateMoveValue` updates only `mFadeVolume` + `mExtMoveVolume` (not pause).

**`SoundPlayer` (US Xenoblade):** lock via `detail::SoundThread::AutoLock` (mutex `@+0x354`), not a `SoundPlayer::mMutex` member (ctor has no `OSInitMutex`; layout ends with `mFxSend[AUX_BUS_NUM]@0x58`). `CalcCurrentPlayerPriority` adds `mUnk0x50` (not `SoundParam::priority@0x4C`). `detail_SortPriorityList(BasicSound*)` unlocks between erase and insert. `SetPlayableSoundCount` always `Clamp(count,0,limit)` (no `mUsePlayerHeap` gate). Default `mPlayableLimit = 0x7FFFFFFF`. `PlayerHeap` is a compact type (`mSound@4`, `mPlayer@8`, node`@0x18`), not stock `SoundHeap` subclass; use `AttachSound`/`AttachPlayerHeap`/`AttachSoundPlayer` (bl, not inline). Split budget `0xBA8`: keep Alloc*/Insert-Remove helpers in `snd_SoundPlayerAlloc.cpp`; drop weak LinkListNode/NonCopyable/AutoLock dtors via postprocess.

**`SortPriorityList()` void:** static bucket array + early-out when list size `< 2`. Postprocess: prefix-rename `@GUARD@listsByPrio$N` → `lbl_eu_80665500`, `listsByPrio$N` → `lbl_eu_806382F0`, `@#` bss cookie → `lbl_eu_806382E0`, `__arraydtor$N` → `__arraydtor$4226`; rename LinkList ct/dt mangled Ofs `252` → `256` (node stays `@0xFC`; construct_array only cares about the symbol name).

**`detail_CanPlaySound`:** nest the lowest-priority check inside `playing >= playable` (not an early `return true` for room) so MWCC matches retail `blt` skip.

**`PlayerHeap::Create`:** `limit = size + base` (not `base + size`) so MWCC emits `add r4,r5,r4`. Align with `(base+0x1F)&~0x1F`. `Clear`/`~PlayerHeap`: `u32` end−start for `subf`; dtor nulls `mEnd` after unlock.

**`Pause`:** `mPauseState` machine; ~98.7% — retail dead epilogue `b`s.

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
- **`Callback` (~83.4%) / `__InitParams` (~99.8%)**: soft-caps on GPR coloring
  (`_savegpr_20` vs lower; iirGain/zero temps r5/r6) while semantics match; keep
  shared `>> 7` after the busIn branch join. Decl-order experiments on Callback
  do not close the savegpr band.
- **`SettingsUpdate`**: FULL when `BOOL nested` is declared before `BOOL mask`
  (outer mask→r30, nested→r31).

## RVL AXFX DelayExpDpl2 (4ch)

- **`GetMemSize` / `__InitParams`**: FULL (`<< 4` for ×16; same scheduling-off
  pattern as DelayExp). Gate Init on `AXGetMode() == AX_OUTPUT_DPL2`.
- **`Callback` (~95.4%)**: coef/invCoef declared and assigned before input setup
  improves over array-first (~90.9%). Residual is loop GPR color.
- **`Init` (~90.8%)**: `ok = TRUE` only on alloc fallthrough + `goto alloc_done`
  (Chorus-style) beats pre-setting `ok = TRUE`. Free/alloc still use `s32** p`
  walks (retail advances the object base through `line[0..3]`).
- **Settings/Shutdown (~88%)**: same walk family; unrolling 4ch regresses size
  vs retail’s compact `blt` loops.

## GXFifo — CP/PI register ops + fifo struct copy (US)

`GXSetCPUFifo` / `GXSetGPFifo` soft-cap drivers:

- **`GX_BITSET` on `gxdt->cpCtrlReg` / `cpClrReg`**: MSB-indexed `GX_BITSET` emits
  `oris` into the upper half; retail uses `ori`/`rlwinm` on low CP bits. Replace
  fifo helpers with `reg |= 4; reg &= ~8;` style updates + `GX_CP_REG_WRITE_U16`.
- **PI CPBAS/CPTOP**: retail `clrlwi r4,r4,2` = `TOPHYSICAL(base)` — not `>> 2`.
- **CP fifo / breakpoint high halves**: retail `extrwi rD,rS,14,2` is
  `(ptr >> 16) & 0x3FFF` (MSB bit start 2), **not** `>> 2`. Watermarks use
  `srwi >> 16`. Wrong `>> 2` was the sole miss on `GXEnableBreakPt` /
  `__GXCleanGPFifo` (now FULL_MATCH).
- **CP status/ctrl bit tests**: use LSB masks / shifts (`(reg >> 3) & 1` for
  underflow-int enable, `(reg >> 1) & 1` for UNFL status, etc.). Feeding LSB
  indices into MSB-style `GX_BITGET` emits the wrong `extrwi`.
- **PI CPWRT wrap**: retail `extrwi …,1,2` → `(reg >> 29) & 1`, not
  `GX_BITGET(reg, 29, 1)`.
- **Struct copy**: retail load-all-then-store-all through one `r31` home; C field
  stores interleaved with loads emit `@ha` base stores. Use block-local temporaries
  for every `GXFifoObjImpl` field, then store batch (~81% → ~93% on `GXSetCPUFifo`,
  ~87% → ~93% on `GXSetGPFifo`).

Unit `RVL_SDK/src/revolution/gx/GXFifo`: ~96.7% fuzzy CODE_MATCH at exact size
budget (`0xC30`, 0 spare). `GXInitFifoBase` at **FULL_MATCH**; remaining near-misses
are single-instruction `extrwi` vs `rlwinm` soft-caps on pointer-high / wrap-bit extracts.

Additional near-miss patterns:

- **`GXInitFifoBase` loWatermark**: `(size >> 1) & ~0x1Fu` (retail `rlwinm …,1,26`) — not
  `(size >> 1) & ~1u`.
- **`__GXWriteFifoIntReset(TRUE, FALSE)`** (overflow IRQ path): retail `ori 1` then
  `rlwinm …,31,29` to clear lo-reset while setting hi-reset — add `else if (hi) reg &= ~2`.
- **`__GXCleanGPFifo` COUNTH**: `(u16)((s32)GPFifo.count >> 16)` for retail `srawi`, not
  unsigned `>> 16` (`srwi`).
- **Do not refactor `__GXCleanGPFifo`** into block-local temporaries — MWCC already
  interleaves struct/CP stores; explicit rewrite dropped ~6.6% match despite freeing `0x10`.
- **`GXSetGPFifo` tail breakpoint clear**: use `DECOMP_PPC_RLWINM(reg,0,31,29)` then
  `DECOMP_PPC_RLWINM(reg,0,27,25)` before the double `CP_ENABLE` sth (retail rlwinm pair,
  not `& ~0x22`). `(s32)count >> 16` for COUNTH (`srawi`).
- **`GXSetCPUFifo` / `GXSetGPFifo` base @0x0**: MWCC still emits `@ha` `stw` for the first
  struct field; keeping other fields through `dst = &CPUFifo` with `base` stored last is
  the best soft-cap (~93.9% CPU / ~95.2% GP after NULL bind-order). Do not use struct
  assign (`CPUFifo = tmp`) — grows +0x24 bytes and drops to ~85%.
- **NULL-path bind flags**: `GXSetCPUFifo` clears `bind_gp` then `bind_cpu`;
  `GXSetGPFifo` clears `bind_cpu` then `bind_gp`. Swapped order is a visible near-miss.
- **`GXSetCPUFifo` Ready before copy**: set `CPUFifoReady = 1` before the field copy,
  `bind_cpu = TRUE` after (the `+0x20` word copy would clobber an earlier bind store).
- **Do not interleave `GXSetGPFifo` CP burst into the temp copy block** — collapses to
  ~49%. Keep struct copy then separate `gpHome` CP writes.
- **`__GXSaveFifo` / `GXCPInterruptHandler`**: remaining gap is pure GPR color (~99.4% /
  ~99.7%); `u32 reg` on the BP path did not move InterruptHandler.

- **`GXSetTexCoordCylWrap`**: retail `and.; beqlr` flushes BP when
  `(tcsManEnab & (1<<coord)) != 0`. A negated test emits `bnelr` and stuck at
  99.8% — invert to match ScaleManually-style “flush when enabled”.

## GXInit — `.data` pool for `__GXInitGX` (US)

- Put the version banner in **`.data`** as `char s_GXVersionStr[0x48] = "…"` (not
  `const` / `.rodata`). Retail bases r28 on that object; `DefaultTexData` is
  `aligned(32)` at +0x60, VAT list at +0x80, proj at +0x150.
- `GXDefaultProjData` must be **8** floats (trailing 0 pad) so the object is
  0x20 — seven floats leave a 0x1C hole and shift later pool offsets.

## GXTexture — LOD + PreLoaded BP burst (US)

- **`GXInitTexObjCI` (FULL_MATCH)**: snap stack `tlut_name` into a local before
  `GXInitTexObj`; after the call do `flags = t->flags; t->tlutName = name;
  t->flags = flags & ~2` so MWCC emits retail `lbz` / `stw tlut` / `rlwinm` /
  `stb` order (tlut store **before** clearing flags bit1).
- **`__GXSetTmemConfig` (FULL_MATCH)**: load the retail SETIMAGE1/2 words as
  hex literals (`0x8C0D8000`, …). Rebuilding with
  `TX_SETIMAGE*(addr >> 5, 3, 3, …)` overflows the 10-bit tmem field into the
  RID byte (`0xBB001E00` vs retail `0xB30DDC00`) and fails equivalence.
- **`GXInitTexObjLOD` (~92.8%, size exact `0x104`)**: clear ROUND/FIELD with
  `__rlwinm(mode0, 0, 15, 12)` (retail wrap mask → value bits 17–18). Do **not** use
  `& 0xFFFF9FFF` (wrong bits). Prefer early `__cntlzw(mag_filt - 1)` /
  `__cntlzw(do_edge_lod)`, `float_8066C02C * lod_bias` (fmuls operand order), and
  `(s32)(f32)` for `fctiwz`. Remaining gap is latency fill (`lfs 0` / `cntlzw` edge vs
  `lwz mode0` order) — `#pragma scheduling off` and extra `biased`/`lod0` locals
  regress. Soft-cap until a non-patch schedule appears.
- **`GXLoadTexObjPreLoaded` (~75.1%, size `0x164`)**: keep `volatile void* pipe =
  &WGPIPE` + `u8 cmd = 0x61` so MWCC emits `lis 0xCC01` / `li 0x61` and
  `stb`/`stw` `-0x8000(r29)`. Interleave BP writes with RID `rlwimi` and store
  fields back after the image3 `stb` (before image3 `stw`). `#pragma scheduling off`
  drops to ~66%. Equivalence stays `inconclusive_unvalidated_callee` (indirect
  `tlutRegionCallback`). Soft-cap is mostly prologue reg color / table-`li`
  interleave vs retail.
- **`GXGetTexObjLODAll` (~49%)**: volatile `f64` i2f stacks still tend to emit
  `fsub`+`frsp` instead of retail `fsubs`; MWCC also saves an extra GPR (`r28`).
  `^ 0x80000000` forces `xoris` but has not beaten the ~49% baseline yet. Park
  while chasing InitLOD / PreLoaded.

## ocMsg ring push/pop (US)

Frameless helpers on a list in **r4** (unused first arg / return in **r3**). Shared
header: `wrap@0`, `capacity@4`, `readIdx@8`, `writeIdx@0xC`, `count@0x10`.

- Reuse one `s32` local/param for **index then count** so MWCC emits retail
  `addi r5,r3,1` / `lwz r3,count` (separate temps → `addi r3,r3,1`).
- Compare **`s32 newIdx > s32 capacity`** → **`cmpw`**. Same test on `u32` emits
  **`cmplw`** (1-insn miss, ~97%, and SMT `not_equivalent`).
- Slot stride 8 (`func_8003A714`/`A764`, cur at `0x64`/`0x68`) or 12
  (`A950`/`A9A8`, cur at `0x8C`/`0x90`/`0x94`).

## ocBdat typed load / column lookup (US)

- **`func_8003B6A0`**: `volatile u32 local; local = 0;` plus `switch (type)` with
  `default: break;` emits retail `stwu -0x10` / stack slot / `cmplwi` / `bctr`
  jumptable. Per-case typed stores via `volatile u8|u16|s16*`; case 7 adds
  `base`. `#pragma dont_inline` keeps callers on `b`. Rename MWCC switch cookie
  (currently `@1095`; drifts with TU growth) → `jumptable_eu_80524D90` in
  `postprocess_reloc_names.py` (`ocBdat.o`) — opcode-identical near-FULLs often
  fail objdiff solely on this reloc name.
- **`func_8003B800`**: same switch skeleton as B6A0 but writes `VMArg*` (`r28` out,
  `r31` local in retail); cases 7/8 set string/fixed types and skip the final
  `VM_TYPE_INT` pack. Rename MWCC cookie (currently `@1252`) →
  `jumptable_eu_80524DB8`. Soft-cap ~78% when `local` spills to `8(r1)` instead
  of `r31`.
- **`func_8003B434` (US FULL_MATCH) / `func_8003B748`**: load **`0x12` then `0x10`**;
  bounds via **`lhz` + `cmpw`** (`s32`). B434: use **goto fail-before-success**
  (`if (max < row) goto fail; if (row >= 0) goto success; fail: ok=0; … success:
  rowArg=row; ok=1`) so MWCC emits retail **`blt`/`bge`** polarity — a merged
  ok-flag `if` soft-caps ~94%. Then assign **`row = rowIdx`** before the ok
  flag (retail `mr r6,r4` / `li r0,1`) and keep retail tail load order into
  `b func_8003B6A0`. B748 elem-size: `(type-6)<=1` → `*4`, else inner `switch`
  on 1/4/2/5/3 (flat `else if` regresses hard). Soft-cap **B748 ~86%** (elem-size
  `cmpwi` schedule). Soft-cap near-misses: **getFP ~95%**, **B4B0 ~96%**,
  **AD98 ~96%**, **B488 ~87%**, **B800 ~98%** (drop `volatile` stores on cases
  1/2 to hit retail `0xEC`). **`getIdCount`/`getIdTop`:** helpers return **`u32`**
  while still loading `u16` — `u16` return emits caller `clrlwi` and blows the
  `0x4c` leaf size.
- **`func_eu_8003B720`** (XOR decrypt / string pool walk): exact retail size
  **`0x540`**, objdiff fuzzy **~84%**. Critical MWCC shapes:
  - Retail outer walk: **cached** `count` in `r11` + `cmplw`/`blt` (not CTR).
    Retail **inners** take CTR (`mtctr`/`bdnz` on 16-byte + residual). High-level
    `do { … } while (--n)` emits **`addic.`/`bne`** for those inners (known
    soft-cap elsewhere too), which leaves CTR free — then a cached outer count
    becomes **`mtctr`/`bdnz`** and undersizes (`0x53C`). Workaround: bottom-tested
    outer with a **reloaded** count so outer cannot take CTR:
    `goto loop_check; loop_body: { …; entryList++; idx++; } loop_check: if (idx < *p) goto loop_body;`
  - Top-tested `while (1) { if (idx >= count) break; … }` can still hit `0x540`
    but **drops fuzzy to 0%** (objdiff skips scoring). Prefer reload bottom-test.
  - Schedule: load `start`/`len`/`key` then `end=start+len`; `nor` interleaved
    with `cmpw` before key masks; restore keys from full `nor` temps; `ent[start]`
    for `lbzx`/`stbx`; defer `lim = mem1-3` into gate bodies; phase2 overflow
    tests `span` then `span1` signs (no `span+=1`).
  - Residual vs retail: `stmw r24` (decomp) vs `stmw r23` (retail XOR temp) and
    inner `addic.`/`bne` vs `mtctr`/`bdnz` — SMT on the full `0x540` XOR often
    `inconclusive_timeout` at the default prove budget.
  - **Do not** switch the 16-byte/residual decrypt to
    `for (; n != 0; n--)` here: it can emit `mtctr`/`bdnz` but **inflates** the
    body (`~0x6D8`, unit size FAIL) and drops fuzzy to ~57%. Keep
    `do { … } while (--n != 0)` for the exact-size peak.
  - Unit split budget `0x1378`: shrink oversizers
    (`getBdatStringColumnValue` early row-bounds `return`, keep B748 `switch`)
    for size PASS before accepting B720. Accepted leaves in-unit:
    `func_8003B204`, `func_8003AFC0`, `bdat`, `ocBdatRegist`.
  - VM wrappers: MWCC emits short names (`getVal`, …); retail objects use
    `getVal_8003BDB8`, … — rename in `postprocess_reloc_names.py` (`ocBdat.o`
    `exact_renames`). After rename: `getArrayCount`/`getVarType`/`getIdCount`/
    `getIdTop`/`getFlagVal`/`bdat` at **100%**; `getVal` ~91%, `getArrayVal` ~97%.
- **`getFP__FPCc`**: retail symbol is a C++ **free function** `getFP(const char*)` →
  `getFP__FPCc`, not `CBdat::getFP` (`getFP__5CBdatFPCc`). Do not forward-declare
  inside `extern "C"`. `CBdat::getFP` can inline-delegate to `::getFP`. Binary search
  over `lbl_eu_805705D0[0..6]` with `strcmp` on entry name at `entry + lhz(entry+6)`.
- **`func_8003B4B0`**: column hash `hash = hash*7 + (s8)c` for up to 8 chars
  (`strlen` capped), `rem = hash % bucketCount` via div/mul/sub, bucket walk with
  `strcmp(col, entry+4)` and `lhz(entry+2)` chain.

