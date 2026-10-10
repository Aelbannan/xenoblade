# Category E — hybrid-link pool labels (undefined symbols)

**Date:** 2026-09-29. Owner of record: session agent (E handoff).
Prerequisite docs: `docs/handoff/unit_rules/e_replace_placeholder_units.md`,
`docs/evidence/decomp/unit_rules_category_b.md` §7.8–7.10.

Scope: the `lbl_eu_80669Exx` / `lbl_eu_806656FC` undefined symbols reported by
`ninja build/us/main.elf` (handoff §1(a)), plus the placeholder-unit flips.
The §1(b) list (`cf::CfObjectActor::~CfObjectActor()`, 12 × `KyoshinHeap_*`,
`UnkClass_80460308::processData`) is the separate-ticket list; the dtor half is
included here because it was already parked as a one-line fix and blocked the
same link.

## 1. Frontier before

`ninja build/us/main.elf` (`.scratch/e_units/hybrid_link.log`): 17 undefined
reports / 11 unique names, then `internal linker error: ELF_linker.c:2295`:

```
cf::CfObjectActor::~CfObjectActor()          CfObjectPc.o (@etb_80009FF4)
lbl_eu_806656FC                              CLibHbm::setHbmActiveFlag(bool)
lbl_eu_80669E14 / E18                        nw4r::math::FrSqrt(float)         [math_arithmetic.o]
lbl_eu_80669E20..E38                         SinFIdx/CosFIdx/Atan2FIdx         [math_triangular.o]
```

`EEC`/`EF0` (snd_BasicSound) and `E00..E10` (math_arithmetic FExp/FLog) are in
the same family but their referencing functions are dead in the current link,
so the linker does not report them yet.

## 2. §3 decision per referencing object

| referencing object | state | decision |
|---|---|---|
| `math_arithmetic.o` (retail placeholder) | refs E00..E18 as UNDEF | owner `g3d_resmat.o` must define |
| `math_triangular.o` (retail placeholder) | refs E20..E38 as UNDEF | owner `g3d_resmat.o` must define |
| `snd_BasicSound.o` (retail placeholder) | refs EEC/EF0 as UNDEF | owner `g3d_resmat.o` must define |
| `CLibHbm.reloc.o` (ours) | refs E656FC as UNDEF | `CLibG3d.o` owns the byte |

`nw4r/src/g3d/res/g3d_resmat.cpp` owns the absorbed `nw4r_data.s` `.sdata2`
slice 0x80669A68–0x8066A178 as one opaque
`unsigned char sdata2_pool_g3d_resmat[1808]`. The referenced words are present
in the blob bytes, but the raw object has **no `@N` symbol** at those offsets
(the TU's code never references them), so:

* `rename_pool_symbols` preconditions fail (no `@…` candidate → no rename), and
* `add_symbols` has no name to redefine.

The two zero words (0x80669E24 = 0.0f, 0x80669EEC = 0.0f) cannot be their own
source objects: MWCC routes every all-zero read-only initializer to `.sbss2`
(verified: `const unsigned char x[4] = {0,0,0,0}` with
`__declspec(section ".sdata2")` still lands in `.sbss2`; `.rodata` is the only
section that keeps zero bytes), and `.sbss2` would fail the data gate (retail
has none).

## 3. Changes

* **`libs/nw4r/src/g3d/res/g3d_resmat.cpp`** — the blob is split into 3 filler
  arrays plus 14 real definitions (`lbl_eu_80669E00..E38`, `lbl_eu_80669EF0`);
  `lbl_eu_80669E20` is an 8-byte `{65536.0f, 0.0f}` object so the E24 zero word
  keeps its retail position, and the two zero words' names ride on the two
  filler arrays (E24 on the 0xB0-byte `[0x3D8,0x488)` array, EEC on the
  0x280-byte tail) whose symbols the rule re-points. The definitions are
  `extern const unsigned char …` at global scope: `extern` gives external
  linkage to an otherwise-internal-linkage `const` while keeping the bare
  (unmangled) variable name, without adding `extern "C"` smell debt.
* **`tools/postprocess_reloc_names.py`** `g3d_resmat.o` — `add_symbols` extended
  (existing field, value change) with
  `("lbl_eu_80669E24", ".sdata2", 0x3D4, 4)` and
  `("lbl_eu_80669EEC", ".sdata2", 0x49C, 4)` (retail 0x3BC/0x484 + the 0x18
  front pool dropped by the existing `drop_data_range`). Recorded with
  `check_unit_rules_frozen.py --ratchet` (413 → 413, one changed payload).
* **`libs/monolib/src/lib/CLibG3d.cpp`** — `u32 lbl_eu_806656F8[2]` split into
  `u32 lbl_eu_806656F8;` + `SbssActiveFlag lbl_eu_806656FC;` (bool + 3B pad).
  Retail `.sbss` 0x806656F0–0x80665700 is F0(8B)/F8(4B)/FC(1B) + 3B pad.
* **`src/kyoshin/cf/object/CfObjectEne.cpp`** — the parked CfObjectActor dtor
  fix, done source-side instead of adding a new `exact_renames` field: the free
  function is now `extern "C"`, so MWCC emits the verbatim retail
  `__dt__Q22cf13CfObjectActorFv` instead of the C++-linkage
  `__dt__Q22cf13CfObjectActorFv__FPQ22cf13CfObjectActorl`. Body byte-identical.
  This is a conscious +1 `extC-def` smell for that TU (5 → 6);
  `docs/CODE_SMELLS.md` is regenerated accordingly. The alternative (the parked
  `exact_renames`) adds a new field to the frozen table and needs
  `--write --force` sign-off.
* **`libs/nw4r/include/nw4r/math/math_arithmetic.h`** — the four fastcast
  wrappers now use the raw OSFastCast intrinsics:
  `U16ToF32(register u16 arg) { return __OSu16tof32(&arg); }`, `F32ToU16` /
  `S16ToF32` / `F32ToS16` likewise. The old by-value + `volatile`-out wrapper
  form made GC/3.0a5.2 emit an extra by-value copy (`sth`/`frsp`) and a
  volatile `stfs` per conversion; the raw form reproduces retail's
  `psq_st`/`psq_l`-straight-from-slot sequence.
* **`libs/nw4r/src/math/math_arithmetic.cpp`** — FExp tail add order is
  `(expx + (k << 23))` (74.3% → 77.1% hexdiff; best of six tried shapes).
* **`configure.py`** — `nw4r/src/math/math_triangular.cpp` flipped
  `NonMatching` → `Matching` (4/4 FULL_MATCH, raw data gate MATCH, size PASS).

No new rule keys/fields; no asm; no postprocess beyond the one extended
`add_symbols` value.

## 4. Verification

| check | result |
|---|---|
| `ninja build/us/main.elf` | every pool-label undefined gone; remaining 14 unique = exactly the §1(b) tickets |
| `hexdiff nw4r/src/g3d/res/g3d_resmat --all` | 54/56 (2 "not written yet"; same as before) |
| `data diff --postprocess main/nw4r/src/g3d/res/g3d_resmat` | **MATCH** — `.sdata2` 0x710 bytes identical; postprocessed symbols E14@0x3AC, E24@0x3BC, EEC@0x484, EF0@0x488 (all global) |
| raw `data diff` g3d_resmat | still the pre-existing `0x728 vs 0x710` FAIL (0x18 front pool); unchanged by this work |
| `hexdiff monolib/src/lib/CLibG3d --all` | 6/6; `data diff` **MATCH** (`.sbss` 0x10, align 8) |
| `hexdiff kyoshin/cf/object/CfObjectEne --all` | 129/137; both dtors 100%; other 8 mismatches pre-existing |
| `hexdiff nw4r/src/math/math_triangular --all` | **4/4 FULL_MATCH** (was 1/4); raw `data diff` MATCH; `size` PASS (0x2D4 exact) |
| `run.py cycle` us-8040d090 / us-8040d0f8 / us-8040d1bc | PASS, status FULL_MATCH (registry updated) |
| math_triangular flip | `build/us/main.elf.rsp` uses `build/us/src/nw4r/src/math/math_triangular.o`; link frontier unchanged |
| `data diff --postprocess main/nw4r/src/math/math_arithmetic` | **MATCH** (rule side ready) |
| fastcast-header sweep | direct wrapper users = math_arithmetic, math_triangular, g3d_resanm/chr/clr/texpat, snd_Lfo; the four resanm objects differ only by the harness `.note.split` (`.text` identical), snd_Lfo 4/4 with both forms. Only indirect user (`SinIdx`) is snd_Lfo. |
| `check_unit_rules_frozen.py --check` | OK (baseline 413; changed `add_symbols` ratcheted) |
| `smell_report.py --write` | regenerated; delta is exactly the conscious CfObjectEne `extC-def` 5 → 6 (`extern "C"` total 17544 → 17545). `--check` still fails on the pre-existing `src/**/*.tmp` litter + that regression (see §5) |
| `python3 -m unittest discover -s tools/coop/tests -t .` | 457 OK |

Before → after undefined sets (unique names):

```
BEFORE (11): cf::CfObjectActor::~CfObjectActor(), lbl_eu_806656FC,
             lbl_eu_80669E14, E18, E20, E24, E28, E2C, E30, E34, E38
AFTER  (14): 12 x KyoshinHeap_*, UnkClass_80460308::processData(),
             lbl_eu_80529690
```

The after-set is the handoff's §1(b) list; those entries were already in the
nm-level frontier, they were simply not reached while the pool labels blocked
the link. `ELF_linker.c:2295` still follows the undefined report, so it is not
(only) a consequence of the pool labels.

## 5. Residuals / next angles

* **math_arithmetic.cpp is not flippable yet.** `FExp`/`FLog` are CODE_MATCH
  (hexdiff 77.1% / 88.6%, `objdiff` fuzzy 98.5%, **0 structural / 8+4
  reg_swap**, `.text` 0x13C exact). The register witness refuses:
  `witness-gate: abi-boundary | rho perm maps gpr r4 -> r6; ABI registers must
  be fixed`, so only FULL_MATCH can accept them. Current residual (FLog, 4
  insns): retail loads the `FGetExpPart` raw word into `r3` and
  `FGetMantPart`'s into `r0`; ours loads them into `r0`/`r4` (`rlwinm` targets
  agree). FExp's residual is the `k << 23` / table-pointer / `expx` scratch
  coloring. Helper-shape and add-order variants tried (six for FExp, five for
  FLog): best 77.1% / 88.6%, all still reg_swap-only. Next angle: declaration-
  order / coalescing experiments per `docs/register_mapping.md`, or record as a
  near-miss open item; the unit rule (`extern_data_sections` + `retarget_relocs`)
  is already `data diff --postprocess` MATCH.
* **§1(b) tickets:**
  * `kyoshin/code_800A75FC.cpp` KyoshinHeap_*: the source defines the functions
    with C++ linkage (`KyoshinHeap_GetField10__Fv`) while retail and
    `targets.json` use the bare C names; the 12 undefineds are that name
    mismatch. Adding `extern "C"` to the 45 definitions clears all 12, but then
    the unit's own unreconstructed data surfaces (`lbl_eu_80572B38/B58/B78/B88/
    B94`, `lbl_eu_80663E98`; raw data gate already FAILs .data 0x228, .rodata
    0x18, .bss 0x1B8, .sbss 0x30 missing). Reverted here to keep the ticket
    whole; run the linkage fix together with the data reconstruction.
  * `kyoshin/cf/code_800C17DC.cpp` (`UnkClass_80460308::processData`,
    `lbl_eu_80529690`): the unit is marked `Matching` but its source is a
    284-line stub (our `.o` 4.5 KB vs retail 57.7 KB); the referenced symbols
    are simply not reconstructed.
* **Working-tree hygiene (not this change):** `src/kyoshin/CGame.hpp.tmp`,
  `src/kyoshin/cf/CBattleManager.hpp.tmp`, `src/kyoshin/cf/object/CAIAction.hpp.tmp`
  and `src/kyoshin/cf/object/CfObjectPoint.cpp.tmp` are stray backups that make
  `smell_report.py --check` fail on the freshness gate; whoever owns them
  should delete/move them. `src/kyoshin/cf/CBattleManager.cpp:3885` also has a
  duplicated `__declspec(section ".data") __attribute__((used))` prefix that no
  longer compiles (the stale object still links because no depfile forces a
  rebuild); it needs a one-token fix before that TU is built again.

## Session update — 2026-09-29 (hygiene items + §1(b) KyoshinHeap linkage/data)

* **Hygiene (previous note's two open items) — both closed:**
  * The four `src/**/*.tmp` backups are gone: moved to
    `.scratch/e_units/backups/` (not deleted), so the `smell_report.py --check`
    freshness gate is green again.
  * `src/kyoshin/cf/CBattleManager.cpp` compiles again.  Three defects, all
    pre-existing: (a) the duplicated `__declspec`/`__attribute__` prefix at
    line 3885; (b) 13 comment lines carrying UTF-8 `->`/`--` glyphs in a
    `-enc SJIS` TU (`sjiswrap` warned, and the SJIS decoder swallowed the
    newline after one of them, merging line 3923 into the previous line);
    (c) four missing closing braces — two inside the outer `switch`'s
    `case 260:` (the `switch (rand() % 4)` and `switch (chosen)` closers; the
    Aug-25 scaffold backup `.scratch/f3970/CBattleManager.cpp.new` still had
    both, proving the intent) and the CRIT-block `else` closer before the
    `} else {` type-gate (5848).  An inserted closer at the end of
    `func_800DCB54` was tried first and removed again: the unit balances
    either way (EOF depth 0), but only the corrected set compiles.  Note the
    TU is `NonMatching`, so none of this reaches `main.elf` — it is a
    compile-only fix; the CRC/line numbers of that TU's *matched* functions
    were not touched (no lines added in any function).
* **§1(b) `kyoshin/code_800A75FC.cpp` — link barrier cleared.**  Fix is the
  linkage + data pair the previous note predicted, now measured:
  * `extern "C"` on **48** definitions (45 `KyoshinHeap_*`, plus
    `func_800A8E6C`, `func_800A9134`, `sinit_800A9F40`), prefix-only so line
    numbers (and `-ipa file` output) are unchanged.  `sinit_800A9F40` also
    silences the `FORCEACTIVE symbol 'sinit_800A9F40' … does not exist`
    warning.  Verified with `nm` set-difference against the retail split
    object: no retail function symbol is missing from our object any more,
    and `hexdiff --all` still reports the same 46 FULL_MATCH.
  * Data reconstructed from the retail split object (sizes *and* contents):
    `.rodata` 0x18 = the three `MemManager::create` heap names
    (`"CF_MEM1\0CF_MEM2\0CF_BDAT"`); `.data` 0x228 = 9 × `u32[14]` per-stage
    offset tables + 2 × `LblPtrTable*[6]`; `.bss` 0x1B8 (the six objects at
    retail offsets AC8/B38/B58/B78/B88/B94 plus `lbl_eu_80572C48[0x38]`);
    `.sbss` 0x30 (`s8` flag + five `(s8,u32)` stage pairs, retail offsets
    E98/EA0…EC4 via an `aligned(8)` on EA0 to reproduce the flag's 8-byte
    retail slot).  `run.py data diff` is now `ok` on `.data`/`.rodata`/
    `.bss`/`.sbss`, bytes identical for `.data`/`.rodata`.
  * **MWCC gotcha (reusable):** an all-zero aggregate goes to `.bss` even with
    `__declspec(section ".data")`; in a large TU the `#pragma section
    data_type ".data" ".data"` alone was also ignored (it works in a small
    probe) — the two `LblPtrTable*[6]` arrays need **both** the pragma *and*
    the declspec to stay in `.data`.  `nocommon`/`aligned` attributes do not
    substitute (probed: all land in `.bss`).
  * Residual for this unit: `.sdata2` is 0x4 (retail 0x0) — one
    `@LOCAL@func_800A76EC…@zero` float-0 pool word emitted by the still-open
    `func_800A76EC` (46.2%); retail's function produces its zero without a
    pool entry, so this belongs to that function's matching work, not to the
    data reconstruction.  The unit's code residual is unchanged (4 open
    functions: `func_800A76EC` 46.2%, `func_800A8E6C` 84.5%,
    `func_800A9134` 0.9%, `sinit_800A9F40` 94.0%).
* **Link state after this change:** `build/us/main.elf` no longer reports any
  `KyoshinHeap_*` or KyoshinHeap-data undefineds; the whole remaining frontier
  is two symbols: `UnkClass_80460308::processData()` (undefined in
  `UnkClass_80460C34.o`; owner split is `monolib/src/lib/UnkClass_80460308.cpp`,
  whose object has 2 functions and a split FAIL, i.e. the unit needs its own
  reconstruction) and `lbl_eu_80529690` (owner `kyoshin/cf/code_800C17DC.cpp`,
  .data 0x228 at retail; that unit is `Matching` but its data is still
  unreconstructed).  The `ELF_linker.c:2295` internal error persists after the
  undefined list shrinks, so it is not (only) error-count related.
* **Data-reconstruction recipe used (also for the tickets above):**
  `objcopy -O binary --only-section=.X` the retail split object, parse the
  words, emit typed arrays in retail offset order; `objdump -t` gives the
  retail per-symbol sizes; `ldscript.lcf` concatenates sections positionally,
  so short sections move every later unit's data — sizes are load-bearing even
  when the contents are zeros.

### Remaining-frontier analysis (for the next session)

* **`kyoshin/cf/code_800C17DC.cpp` — data, reloc-driven.**  Retail split data:
  `.data` 0xEC8 (9 labels), `.bss` 0x38 (`lbl_eu_805739E8` 0x10,
  `lbl_eu_805739F8` 0x28 — both already typed in
  `include/kyoshin/cf/code_800C17DC.hpp`), `.sdata2` 0x28 (5 string labels:
  `"chr"`, `"anm"`, `"obj"`, `"effect"`, `"mapse"`, `"pack"` plus alignment
  gaps at 0x13/0x1A/0x24).  Our object defines none of them.
  The blocking symbol `lbl_eu_80529690` (0x228) is **not** plain data: the
  retail object carries **909 `R_PPC_ADDR32` relocations** in `.data`
  (starting at offset 0 with the RTTI name + `CObjectState_*` vtable slots,
  e.g. `CObjectState_setStateBitMask__Q22cf12CObjectStateFv`), i.e. it is the
  `CObjectState`/`CfObjectMove` vtable block, which is why `objcopy` shows all
  zero bytes.  Reconstructing it means spelling the slots as pointer
  initializers (`objdump -r` gives every offset→symbol pair) — the
  `CSysWinSave.cpp` `struct SaveDataA` precedent — which also needs the
  `extern u8 lbl_eu_80529690[]` declaration in
  `include/kyoshin/cf/object/CfObjectMove.hpp` to become a pointer array type
  (it is referenced by `CfObjectMove.o`, `CfObjectMap.o`, …).  The remaining
  8 `.data` labels are zero/tiny (only `lbl_eu_80529DA0` 0x610 and
  `lbl_eu_8052A528` 0x30 carry non-zero words; the latter is the typed
  `TextKeyword lbl_eu_8052A528[6]` dispatch table already declared in the
  unit header, ids 1..5 at +0x0C/14/1C/24/2C).  Use the same
  `#pragma section data_type ".data" ".data"` + `__declspec(section ".data")`
  pair as above for the all-zero parts.
* **`monolib/src/lib/UnkClass_80460308.cpp` (owner of `processData`).**  Split
  `.text` 0x92C, our object 0x988 (split FAIL) and only 2 functions defined
  (`adler32`, `ZlibInflateFastCore` 0.0%); `processData__17UnkClass_80460308Fv`
  (.text:0x804642D8, size 0x420) is absent.  This is a real zlib
  reconstruction, not a data gap.
* **Smell gate note:** the 48 `extern "C"` prefixes on `code_800A75FC.cpp`
  definitions are a deliberate `docs/CODE_SMELLS.md` re-baseline (that TU's
  extC-def count 0 → 49, repo definitions +47).  They are retail linker names
  (§17.6 reloc-name drift), not new ABI drift; the gate's per-TU regression
  check passes after `smell_report.py --write`.

## Session update 2 — 2026-09-29 (linker internal error fixed, real frontier measured)

* **`ELF_linker.c:2295` root cause found and fixed (item 6).**  The error was
  *not* a consequence of undefined symbols (the earlier note assumed that).  It
  was a bug in the §17.6 postprocess: the `.sdata2` swap rules
  (`swap_sdata2_leading_f32_pair`, `swap_sdata2_leading_f32_words`,
  `permute_sdata2_words`, the window remap, the `.text` remap and two
  `sec_idx` loops) rewrote `st_value` for **every** symbol in the section,
  including the **section symbol** (`STT_SECTION`), whose value must stay 0.
  `AXFXReverbHiExp.reloc.o` and `mtx.reloc.o` ended up with `.sdata2`'s section
  symbol at 4, which is what mwldeppc trips on.
  * Bisection: relink with all `.reloc.o` replaced by their raw objects →
    0 internal errors (but 102 undefined error blocks); binary search over the
    85 reloc inputs → a single culprit (`index 445`,
    `RVL_SDK/src/revolution/axfx/AXFXReverbHiExp.reloc.o`).
  * Fix: `_is_section_symbol()` helper + a guard in the six st_value-rewriting
    loops (`tools/postprocess_reloc_names.py`).  The frozen *table* is
    untouched, so `check_unit_rules_frozen.py --check` stays green.
  * Verified: audit of all 129 `.reloc.o` files → 0 section symbols with a
    non-zero value; `build/us/main.elf` no longer reports the internal error.
* **The link's undefined list is capped.**  mwldeppc stops after ~100 undefined
  error blocks, so the reported frontier (14 names at the start of this ticket
  family; 39 after §1(a)) was never the real one.  New tool
  **`tools/coop/link_undefs.py`** computes it offline from
  `build/us/main.elf.rsp`: 1161 objects, 50275 defined names, **1235 missing**
  (referenced, defined nowhere), 85 of them local `@N`/`@LOCAL@` artefacts.
  Top owners: `g3d_resmat` 304, `CfObjectEne` 124, `CMenuBattleChain` 84,
  `CVision` 68, `CfGimmick` 44, `CChainActorList` 37, `UnkClass_80466348` 36,
  `CfScript` 33.  Almost all are *data* labels of Matching units that retail
  (NonMatching) objects reference — the same pattern as the KyoshinHeap pool:
  the owner unit has the bytes but not the symbols.
* **`processData` (the §1(b) ticket) is done: it was the adler32 body under an
  invented name.**  `monolib/src/lib/UnkClass_80460308.cpp` defined it as
  `adler32__17UnkClass_80460308Fv`; the retail symbol is
  `processData__17UnkClass_80460308Fv` (0x420).  Renamed → `hexdiff --all`
  reports **100.0% FULL_MATCH** for it.  Unit residual: `ZlibInflateFastCore`
  0.0% (0x50C vs retail 0x568) and a split FAIL, so the unit still needs its
  inflate reconstruction.
* **`g3d_resmat` pool labels (first batch of the 1235).**  The unit rule only
  aliases the labels *its own* code references; other units reference the rest.
  25 labels are now real source definitions: 24 split out of the `.sdata2` head
  filler (`AA0…CE8`) and `E40`/`E50` out of the `E24` filler, byte-for-byte
  identical sections, with the declared `f32`/`f32[2]` types where a header
  declares them (bit-exact literals) and byte arrays elsewhere; the all-zero
  ones use the `#pragma section data_type ".sdata2" ".sdata2"` +
  `__declspec(section ".sdata2")` pair.  `hexdiff --all` 54/56 (the two
  `ResTexPlttInfo` accessors are still unwritten — unchanged), split PASS,
  `.sdata2` bytes unchanged.  The remaining 304 are the rest of that pool
  (343 retail labels) and the same split recipe applies.
* **Still open from the original list:** items 1 and 2 (`math_arithmetic`
  FExp/FLog need FULL_MATCH; `snd_BasicSound` 28/35 with a split overrun) and
  item 3 (DOL verification) — the latter is gated on the 1235 missing symbols,
  which is the §1(b) placeholder-unit burn-down at large scale rather than a
  handful of tickets.

## Session update 3 — frontier classification + first linkage batch (2026-09-29)

The 1235 missing symbols are not 1235 missing reconstructions.  Measured
breakdown (`tools/coop/link_undefs.py` + `config/us/{symbols,splits}.txt` +
`UNIT_RULES`):

* **472 not in the source at all** — 85 are local `@N`/`@LOCAL@` artefacts of
  the §17.6 pipeline (definition dropped with the reference left behind); the
  rest are real bodies/data.
* **768 appear in source identifiers**, i.e. mostly *linkage or naming* bugs,
  not missing work.  Of those, 156 are defined with `extern "C"` and still
  missing (postprocess/strip class), 607 are plain C++-linkage definitions that
  MWCC mangles while retail wants the literal name.
* 1069 of 1235 need our objects to define them; 166 are `extern_data_sections`
  "dissolved" data (the rule is the debt: reconstruct the data in source, then
  remove the field).

**Stale `@N` renames.**  The table's `exact_renames` entries drift when MWCC
renumbers pools.  An automatic repair exists: our object's reloc offset and the
*retail* object's reloc at the same offset name the correct label.  Applied to
the CMenuQuestLog jumptable: the entry said `@13609` (no longer emitted), the
live symbol is `@10951`, both at `.text+0x726` where retail has
`jumptable_eu_8052CC10`.  19 of the 37 `@N` danglers resolve uniquely this way;
18 need content matching.

**`extern "C"` linkage batch — `CfObjectEne.cpp` (−113 symbols).**  Its 124
missing symbols were mostly definitions MWCC mangled (`...Fv__FPQ22cf11CActorParam`)
while retail wants the literal name.  Adding `extern "C"` to the *definition*
(not to names that a header already declares, and not to definitions whose
natural C++ mangling already equals the retail name) fixes them:
`124 -> 10` missing, **hexdiff unchanged at 129/137**, clean build, no new
missing symbols except two header-declared names
(`__dt__Q22cf13CfObjectActorFv`, `getUnk80664658__Fv`) that need a
header/declaration-level fix instead.  Frontier: **1235 -> 1122**.

Same-pattern candidates for the next batches (plain-linkage definitions whose
retail name is literal): `CMenuBattleChain.cpp` (84), `CVision.cpp` (68),
`CfGimmick.cpp` (44), `CfScript.cpp` (33), `CScrollBar.cpp` (32), plus the
`.ctx.c` name pools (`code_802B9064.ctx.c` 58, `CfSoundMan.ctx.c` 26, ...).

## Session update 4 — linkage batch sweep (2026-09-29)

`tools/coop/fix_linkage.py` (promoted from the session scratch) adds
`extern "C"` to a unit's definition (plus any in-file declaration of the same
name) when the retail symbol's name is a literal that MWCC would otherwise
mangle.  Guards, all learned the hard way:

* skip names declared in any real header (`include/`, `libs/*/include/`,
  `src/**/*.hpp`) — the header declaration would conflict (linkage mismatch);
* skip if a *retail* symbol disappears from the object after the patch.  The
  first version reverted whenever any symbol disappeared, which rejected every
  legitimate patch (the old mangled name is expected to go away).  Only lost
  *retail* names signal breakage;
* units without `UNIT_RULES` link their raw object, so `ninja
  build/us/src/<unit>.reloc.o` reports "unknown target" — that is not a build
  failure.

Results: `CfObjectEne.cpp` 124 -> 10 missing (−113 including the earlier
batch), then 21 more across `CtrlMoveNpc`, `CVision`, `CfGimmick`, `CfScript`,
`CfCollCylinderImpl`, `CfResTboxImpl`, `code_80213488`, `code_800C17DC`,
`CfGimmickItem`.  Frontier **1235 -> 1101**, no new missing symbols, hexdiff
unchanged for every touched unit, smell/unit-rules/tests green.

What is left in the "name present in source" class (as of 1101): 211
header-declared definitions (need `extern "C"` on the header declaration, with
class-scope care), 280 declaration/comment-only (the body is genuinely
missing), 101 absent, 413 whose owner path has no `src/` file (units not
started), 86 pipeline artefacts (`@N`).

## Session update 5 — math_arithmetic FExp/FLog diagnosis (2026-09-29)

`math_arithmetic` is `split PASS` exactly (0x13C), `FAtan` 100%, and the two
residuals are **pure allocator tie-breaks, not structural**:

* `FExp` 77.1% — 0 structural, 8 reg_swap, 0x8C/0x8C
* `FLog` 88.6% — 0 structural, 4 reg_swap, 0x8C/0x8C

The instruction stream is byte-for-byte the retail opcode sequence; only the
GPR assignment differs, and `--brief` shows it is a rotation of
{r3,r0} plus a one-register shift (retail keeps the `k<<23` result in r3 and the
reloaded spill in r0; ours does the reverse).

Two source-shape variants were tried and both are *worse*, so the current text
is the best known form and the next attempt must come from the MWCC
register-allocation playbook (`docs/register_mapping.md`, the register skill):

* named `u32 kbits = k << 23;` temporary — 77.1% -> **65.7%** (12 swaps)
* commutative swap `((k << 23) + expx)` — 77.1% -> **74.3%** (9 swaps)

Not a blocker: the unit's data gate is MATCH and the remaining work is
allocator-shape archaeology on an otherwise byte-identical function.

## Session update 6 — why the g3d_resmat pool labels cannot be named (investigated, reverted)

The 293 remaining labels of `g3d_resmat`'s `.sdata2` pool look like the easiest
frontier class (bytes already emitted, only names missing).  They are not, and
the reason is worth recording:

* **MWCC 8-aligns every object placed in `.sdata2`.**  Retail's neighbouring
  labels sit at 4-aligned offsets (e.g. `lbl_eu_80669D0D` at range+0x2A5 in a
  byte-granular run).  Emitting each label as its own object (section
  `__declspec` + the `.sdata2` pragma) inserts padding: measured drift +0x18
  (front-piece duplicate) then +8 (alignment), with `__attribute__((aligned(1)))`
  and `aligned(4)` both ignored.  The byte-exact layout therefore requires the
  labels to live *inside* larger blobs, where C++ cannot give them symbols.
* **`add_symbols` can define at arbitrary offsets, but has no space.**  The tool
  does create global definitions (`st_info` 0x11) and reuses the "redefine an
  existing UNDEF" path; appending a new name is limited by the object's string
  table.  A 344-entry batch produced **51** successes and 293 silent `False`
  returns.  The mechanism works for names the object already references as
  UNDEF; it cannot introduce hundreds of new names.
* Batch experiments: with the generated layout the unit gained ~285 definitions
  (frontier 1022 -> 780) but `.sdata2` grew +8 bytes, which shifts every
  following symbol -> not acceptable for byte-exactness.  With the byte-exact
  source, the added symbol entries did not materialise at all.  Everything was
  reverted; the table is back inside the frozen baseline.

To land this class: (a) grow `.strtab`/`.symtab` in `add_local_symbol` (re-emit
the ELF) and then batch the labels through `add_symbols`, or (b) find a per-TU
pragma/flag that keeps `.sdata2` objects 4-aligned so the labels can be real
source definitions.  Either one is a tool change, not a source change.

## Session update 7 — header-level `extern "C"` batch landed (1235 -> 1005)

`extern "C"` on **free-function declarations in headers** (24 headers, 101
names) plus the owner-unit rebuilds: frontier 1022 -> **1005**, 0 new missing
symbols, 0 build failures, hexdiff unchanged for every affected unit
(`CfGimmick` 38/51, `CFade` 13/14, `CArtsSet` 15/15, `CfScript` 38/40,
`CMenuBattleChain` 22/23).

Two bugs made the earlier attempts fail and are now fixed:

* **Encoding**: patching a file with `read_text`/`write_text` re-encodes SJIS
  TUs as UTF-8 (`sjiswrap: contains Shift JIS encoding errors`).  All patch
  tooling now uses `latin-1` byte-preserving round-trips.
* **Line-number drift**: the declaration scanner stripped `/*...*/` comments
  *including their newlines*, so the line numbers it recorded no longer matched
  the file.  It patched `include/decomp.h`'s `#else` into `extern "C" #else`,
  which broke `DECOMP_DONT_INLINE` for every TU that includes `monolib/vm/yvm2.h`
  (161 "new" build failures).  The scanner now preserves newlines, and the patch
  loop refuses `#`/comment lines.

With both fixed the batch is clean and repeatable.  The definition-level fixer
(`tools/coop/fix_linkage.py`) is now exhausted: a re-scan of the 1005-symbol
frontier found only 2 more candidates (`CtrlMoveNpc`, applied).

Remaining frontier classes: ~51 in-class (mangled member) declarations, the
`@N`/dissolved-data classes that need data reconstruction, and units whose
sources cannot rebuild in this checkout.

### Conscious smell re-baseline

The strict smell gate (`smell_report.py --check`) reports `extC-def` growth for
every unit this session converted to `extern "C"` (e.g. `CfObjectEne` 5 -> 123,
`CfGimmick` 3 -> 38, `CChainActorList` 4 -> 26, `code_800C17DC` extC-decl
0 -> 473).  That is the intended effect of the linkage batches (retail-named
free functions must carry C linkage), not a regression: freshness passes
(`--check --no-strict`: ok) and `docs/CODE_SMELLS.md` is regenerated.  The
strict gate compares against the committed baseline, so the human re-baselines
at commit time.

## Session update 8 — the g3d_resmat pool labels land in source (1003 -> 767)

Update 6 concluded the 293 `g3d_resmat` `.sdata2` labels needed a *tool* change
(grow `.strtab`/`.symtab`).  They do not: they can be **source objects**.

What was wrong with the earlier attempt is only alignment.  MWCC aligns every
`.sdata2` object to `min(size, 8)`, so a label is expressible in C++ exactly
when its retail offset already satisfies that alignment; everything else
(1-byte labels, 8-byte labels at 4-mod-8 offsets, labels a header declares with
another type, the words the rule already aliases) must stay inside a filler
piece and keep its symbol from the unit rule.

`g3d_resmat.cpp` now carries a regenerated pool block (`.scratch/e_units/gen_resmat_pool.py`):
one `extern __declspec(section ".sdata2") const unsigned char lbl_eu_80669XXX[N]`
per label, 4-byte-max filler pieces for the gaps, and the seven typed
`const f32` definitions plus two new ones (`lbl_eu_80669EEC` = 0f,
`lbl_eu_80669EF0` = 2^52 + 2^31 magic, both declared in
`nw4r/include/nw4r/snd/snd_BasicSound.h`) kept verbatim.

Verified:
* `.sdata2` **byte-identical** to the pre-edit (already byte-exact) object, and
  `.text` byte-identical; every named symbol sits at
  `0x18 + (addr - 0x80669A68)` (223 of 223 checked).
* unit hexdiff unchanged (54/56), `UNIT_RULES` untouched.
* frontier **1003 -> 767** (-236, zero new missing).

Residual for this unit: 43 labels that need the rule path (header-declared /
unaligned), and 25 genuinely missing function bodies
(`ResGenMode::CopyTo`, `ResMatChan::CopyTo`, ...).

Note: `run.py data diff` reports MISMATCH for this unit in this checkout because
the retail split object on disk is 0x6E0 where the unit's slice is 0x710; the
source reproduces the 0x710 slice exactly (verified against the pre-edit
object).

## Session update 9 — workstreams 1-3 (CRT symbols, vtables, @N leftovers)

The 86 "unattributed" symbols split into infrastructure, not decompilation, and
the two named classes turned out to be symptoms of the same data problem:

**1. CRT / link-config (10).**  `_stack_addr`, `_stack_end`, `_db_stack_end`,
`_SDA_BASE_`, `_SDA2_BASE_`, `__ArenaHi`, `__ArenaLo`, `__end__catch`, `__mftb`
are provided by the retail linker script (no `.lcf` in this checkout; the
proposal is a `bake_linker_addrs` entry, which is a table change needing human
sign-off).  `fake_function` was ours: it is the callee of the
`DECOMP_FORCEACTIVE` keep-alive stubs and had no definition anywhere.

**2. Vtables (7) + 3. `@N` leftovers (37) are not independent.**  Every `@N` is
a local whose section a rule externalised (`extern_data_sections`) because the
unit's data is not reconstructed: the strip zeroes the section and turns the
locals into UNDEFs, so the refs dangle.  They come free with the unit's data
pass (workstream 6), not with a symbol rename.

**Landed now:**

* `ut_Font.o` — its `.data` already reproduces the retail split byte-for-byte
  (0x30), so its `extern_data_sections=(".data",)` was turning four valid local
  table symbols into UNDEFs.  Dropped the field (shrink-only, allowed):
  frontier -4, `.data` still identical.
* `fake_function` — cleared.  The keep-alive stubs now either become explicit
  `.data` arrays (OSError, dsp_task: `.data` **and** `.init` byte-identical to
  retail, 0x0) or call the *defined* `OSReport` (gap_utils, OSPlayRecord:
  `.data` byte-identical).  This also removes the undefined reference and, for
  the two array conversions, the extra `.init` bytes.
* `usb.c` **parked** as `NonMatching` with a comment: its four original
  keep-alive stubs were rebuilt from the retail `.data`, but the emission order
  comes out 0x1C too long (all strings present, wrong placement).  The retail
  placeholder keeps the link byte-exact until the original 4-stub placement is
  re-derived.

Frontier **767 -> 762** (ut_Font -4, fake_function -1, zero new missing).

**Known residual:** keep-alive stubs that stay in `.init` add bytes versus
retail (`gap_utils` `.init` 0x30, `OSPlayRecord` 0x10); the array conversion
removes them but only when the unit's literal emission order survives the move.

## Session update 10 — the data-pass class is code-driven, not a data dump

Attempting the first data pass showed that the "404 labels whose bytes are
absent" do **not** have a source-side data home independent of the code:

* `code_80213488` (the one unit with reloc-free missing data): retail
  `.sdata2` 0x18 holds `30.0f`, `0.0f` and two 2^52 doubles ahead of the two
  pooled doubles we already emit.  Defining the two `f32`s in source as
  `extern "C" __declspec(section ".sdata2") const f32 ... = 30.0f/0.0f;` makes
  MWCC const-propagate them into `MakeCrystal_CollectByTarget`'s division and
  emit **extra pool words** (`.sdata2` 0x20 vs retail 0x18), so the two words
  must come from the code path that uses them, not from a data block.  Reverted
  byte-identically (0x10, 11/11 still matched).
* Every other top candidate has the same shape: the source carries 9-54 string
  literals where retail's `.rodata` is 80-504 bytes, and the missing `.data` is
  reloc-bearing (70/68/176/46/... `.rela.data` entries) — vtables and pointer
  tables belonging to classes/functions that are not reconstructed yet.

So workstream 6 (data passes) is not separable from workstream 7 (function
bodies): each unit's data lands when its code lands.  The mechanical part that
*is* available — naming sub-objects inside data we already emit byte-exactly —
was done for `g3d_resmat` (223 labels) and `ut_Font` (4).

Workable next targets are therefore per-unit matching passes, biggest first:
`CMenuBattleChain` (77), `CVision` (63), `UnkClass_80466348` (36),
`CfScript` (30), `CScrollBar` (27), `CtrlMoveNpc` (17), `CChainActorList` (15),
`CFade` (13), `CCur` (13), `CCharVoice` (13).

## Session update 11 — linkage wave: 360 -> 248 undefineds, 0 new missing (2026-10-10)

Worktree `paseo-unitrules-e`. Instrument: `tools/coop/link_undefs.py`
(mwldeppc's own list is capped at ~100 error blocks). Frontier measured
**360 missing** at session start (docs said 762 on 2026-09-29; the worktree
snapshot postdates further burn-down elsewhere), **248 missing** at session
end. **112 cleared, 0 new missing symbols.** No `UNIT_RULES` changes
(`check_unit_rules_frozen.py --check` green, baseline 406 throughout), no
`configure.py` / `symbols.txt` / `splits.txt` edits, no new rule fields.
Every cleared symbol is a source-side linkage or data definition in
`src/**` / `libs/**`.

### The pattern (three sub-classes)

Almost all cleared symbols were *defined in our objects but under C++
linkage* while retail (and every referrer) wants the flat literal name.
The bodies were already byte-identical; only the emitted name differed:

* **(A) free function, retail-spelling identifier** — add `extern "C"` to the
  definition (and to any same-signature forward declaration in the TU).
  Zero codegen change by construction (same signature, same body); every
  converted unit's `hexdiff --all` table is unchanged. ~70 symbols.
* **(B) member function, short name** (CFade/CCharVoice/CfGimmick/CfObjectObj
  pilot class) — convert to `extern "C"` free function with explicit `self`
  param; `this` -> `self`, member decls removed from the class. `this` in r3
  and a first `self` param in r3 generate identically; verified 100% holds
  per function (CFade 8/8, CCharVoice 5/5 incl. the `(u32)this` idiom,
  CfGimmick 2/2, CfObjectObj 1/1).
* **(C) member/global ctor under a flat `__ct__` name** — same conversion;
  member/base-ctor calls that retail performs as plain `bl` need the
  retail-mangled-name shadow declaration (see the placement-new finding).

Callers overwhelmingly already reference the flat names (their headers carry
`extern "C"` blocks, or they are NonMatching retail placeholders), so the
definition-side conversion completes the circuit with no caller edits. Where
a same-TU or header declaration contradicted (10505/10197), the fix was to
align the declaration to the definition (authority = definition) or use the
documented `#define <sym>_<reason>_hidden` shield (one case:
`CChainActorList.cpp` vs `CChainTimer.hpp`'s `(void*,void*)` decls).

### Reusable findings (PROVEN this session)

* **MWCC placement new always emits a null guard** (`cmpi`/`addic.` + `bc`
  around the ctor `bl`), with `-Cpp_exceptions on` AND off (probe:
  `.scratch/probe/p_on.o` vs `p_off.o`, identical guards). Retail `__ct__`
  free functions never have the guard, so original sources cannot have used
  placement new here.
* **The guard-free spelling is an `extern "C"` shadow declaration of the
  member ctor by its mangled name**, e.g.
  `extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);`
  then a plain call. Probe `.scratch/probe/r.cpp`/`r.o` verifies a bare
  `addi`+`bl` with no guard; MWCC accepts the shadow alongside the member
  declaration. Used for CFade, CLoad, CLibCriStreamingPlay
  (base-ctor shadows), CfResObjImpl (member-ctor shadow). Per SKILL.md
  declaration-drift §(d) these stay TU-local with retail evidence in the
  comment until the owning headers are converged.
* **TU-local data definitions reschedule their users** (two independent
  cases): naming CFade's rodata pool base moves the `lis`/`addi` above the
  first `bl` in `OnFileEvent` (100% -> 94.3%, reverted); defining the
  `.sdata2` 1.0f moves the `lfs` late in StepFadeIn/Out (100% -> 73.7%,
  reverted — non-const decl AND volatile reads both tried). Mechanism:
  an `extern`-UNDEF symbol is call-clobbered-opaque, so its materialization
  stays put; a TU-local definition is freely schedulable/foldable. Same
  family as the `code_80213488` float ceiling (update 10).
* **Flat-name `bl` targets need no caller changes when the machine signature
  matches** (pointer-for-pointer, `(u8,int)` vs `(CTaskLOD*,int,int)` at the
  `bl` level); the drift is recorded, not "fixed", where call bytes already
  match.
* **`link_undefs.py` does not account for `configure.py link_transform`
  renames**: `CHelp_refreshPartyMenu` and `__ct__Q22cf5CHelpFv` are "missing"
  in its report but provided in the real link via CHelp.cpp's
  `objcopy --redefine-sym` copy (verified: the real `ninja` link never
  reports them). 2 phantoms; no other transform target is currently missing.

### Per-unit record (cleared counts; all hexdiff tables unchanged or better)

| unit | cleared | note |
|---|---|---|
| CFade.cpp | 9 | 8 member->free + `__ct__CFade`; 13/14 @100%, split PASS, data MATCH except pre-existing `.sdata2` gap |
| makecrystal/code_80213488.cpp | 8 | 11/11 FULL_MATCH holds, split PASS, data MATCH |
| CfResObjImpl.cpp | 8 | 7 free + `__ct__cf_CfResObjImpl` (returns self, still 100%); 14/14, split PASS, data MATCH |
| CfGimmick.cpp (+ CfResTboxImpl.cpp vtable slot) | 7 | 5 free + 2 member->free; 38/51 unchanged; Tbox vtable slot switched mangled->flat (data still MATCH) |
| CfScript.cpp | 6 | 38/40, split PASS |
| CVision.cpp | 5 | bodies open (14.9-88.1%, unchanged); names defined; one `(int,int)`->`(void*,void*)` decl unification + 2 cast-only call edits (callers still 100%) |
| CCharVoice.cpp | 5 | member->free incl. `(u32)this`; 7/8, split PASS |
| CTaskLOD.cpp | 5 | 38/38 holds, split PASS, data MATCH |
| bta_dm_act.c | 4 | 4 data defs from retail bytes (uuid u16[24], btm u8[96], security zero-rodata via non-const+declspec, compress_srvcs bss + recovered 9th struct byte from 0x2D BSS/memset-45 evidence); 13/56 unchanged; `.rodata`/`.bss` now byte-exact (section is WA-flagged, bytes identical) |
| CREvtLight.cpp | 4 | 3 free + `__ct__CREvtLight`; 5/7 unchanged |
| CChainActorList.cpp | 3 | 31/40 unchanged; `void_hidden` shield for CChainTimer decls |
| CfObjectEne.cpp (+ include/.../CfObjectEne.hpp decl) | 3 | 129/137 unchanged |
| CfObjectObj.cpp | 3 | 1 member->free + 2 free; 8/8, split PASS |
| CVS_THREAD_{BATTLE_BEGIN (3), CHAIN (3), REVIVE (3), PARTY_GAGE (2), VISION_BREAK (2), BATTLE_END/BATTLE_END_SP/DOWN/FAINT/SUDDEN (1 each)} | 14 | all build, tables unchanged |
| code_804C8684.cpp | 3 | 4/5 (sinit open, pre-existing) |
| CMdlAnmEye.cpp | 3 | 4/5 (RegisterMats reg-swap-only, pre-existing) |
| synenv.c | 2 | RunVolumeEnvelope 100%; SetupVolumeEnvelope open (pre-existing) |
| CNReqtaskSave.cpp | 2 | 33/34; `CNReqSaveConfigSave` NOT converted (see residuals) |
| CScnFogMan.cpp | 2 | 7/8 (ApplyFog open, pre-existing) |
| CLoad.cpp | 1 | `__ct__CLoad` 0% -> 40.0% (vtbl store + member bl added); 12/15 unchanged |
| CSystemWindow.cpp (+ hpp decl) | 1 | 8/8 |
| CtrlMoveNpc.cpp | 1 | refreshNpcRowFlags |
| CMenuQuestLog.cpp | 1 | 22/23; createQuestLogMenu |
| CREvtMem.cpp | 1 | 6/7; member->free |
| CDeviceSC.cpp | 1 | 10/10 (split +0x58 overrun pre-existing) |
| CLibCriMoviePlay.cpp | 1 | ctor 96.5% preserved (2 pre-existing displacement diffs = entry-stride layout drift) |
| CLibCriStreamingPlay.cpp | 1 | ctor still open (2.8%); name defined |
| CScnBlend.cpp | 1 | 5/5 |
| CScnFilter.cpp | 1 | 3/3 |
| snd_SeqFile.cpp | 1 | 2/3; ReadOffsetByLabel |

### Residuals (owner + next angle)

* **Linker-synthesized family (no object defines them; retail link gets them
  from the linker script/extab synthesis):** `_SDA_BASE_`, `_SDA2_BASE_`,
  `_stack_addr`, `_stack_end`, `_db_stack_end`, `__ArenaHi`, `__ArenaLo`
  (acknowledged linker-provided), plus `_eti_init_info` (extabindex:0x80039568
  size 0x20 per symbols.txt; retail `__init_cpp_exceptions.o` also refs it as
  UNDEF — needs the `.lcf`/extab route), `_ctors`/`_dtors`
  (`.ctors$00`/`.dtors$00` linker tables), `_bss_init_info`/`_rom_copy_info`
  (linker copy tables), `__end__catch`, `__mftb`. Route: `bake_linker_addrs`
  table entries — human sign-off (table change).
* **`__ct__CFade` (42.3%) / `__ct__CLoad` (40.0%) near-miss:** residual is the
  dead `or r3,r30` this-restore plus r4-vs-r3 zero coloring. Member-`this`
  codegen emits both; no free-function spelling found in 2-3 attempts each
  (placement new guards; direct stores recolor). Next angle: none obvious —
  genuine MWCC `this`-handling idiom; record as ceiling unless a member-shaped
  formulation with a flat name is discovered.
* **CFade `lbl_eu_8050B5A0` + `lbl_eu_80668750`:** defining either in-TU
  reschedules its users (measured, reverted twice). Needs the rule-side
  `add_symbols` route — CFade has no UNIT_RULES key, so this is a sign-off
  item, not a unilateral change. Proposed: extend `g3d_resmat`-style
  `add_symbols` for CFade (`lbl_eu_8050B5A0` @.rodata+0, `lbl_eu_80668750`
  @.sdata2+0) — coordinator decision.
* **`CNReqSaveConfigSave`:** 6-arg extern-"C" def overloads the deliberate
  1-arg in-TU decl (10197, probed). The file's own angle: full-signature
  in-TU decl with leftover-register call (the NandLoadConfigure pattern
  below it). Caller-matching work, not linkage.
* **`format__Q22ml10FixStr<128>FPCce`:** MWCC emits the generic-template
  mangling regardless (documented open item in CfScript.cpp). Needs a
  template/specialization angle.
* **CScrollBar 5 bodies, CHelp 2 bodies (split-exact blocks the 0xC
  setHelpFlag), `func_8045C700` 0x1B0 body, CTaskGamePic/CfGimmickItem/CCur/
  CtrlMoveNpc/CChainActorList/CVision/CfScript/g3d_resmat data labels:** real
  per-unit matching/data passes (update-10 class: data lands with code).
* **`__vt__*` (5) + `@N` (~25) + `cbRenderBefore__13CSysWinSelectFv`:**
  `extern_data_sections` residue of unreconstructed units (update-9
  analysis); land with those units' data passes.

### Hygiene / coordination notes

* Skipped-unit check: none of the touched units overlap the live-sibling WIP
  list (CItemBoxInfo/CActorParam/CBattleManager/snd_SoundArchivePlayer/
  snd_SeqSound). (`CActorParam_*` symbol names touched live in CfObjectEne.cpp
  and code_800C17DC.cpp, not CActorParam.o.)
* `orig/us/files` in this worktree was a broken symlink (sandbox); replaced
  with a local empty dir (gitignored `orig/*/*`) so `dtk dol split` proceeds
  DOL-only. No REL work was needed for `main.elf`.
* Toolchain symlinks (`build/tools`, `build/compilers`, `build/binutils`)
  were wired centrally mid-session; `run.py configure` re-ran clean.
* No attempts.jsonl appends made (single-writer rule); candidate lines are
  listed in the subagent final report for the coordinator.
