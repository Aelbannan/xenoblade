# e — Replace the hybrid-link placeholder units (link undefined symbols)

**Status (2026-09-29):** §1(a) is done and the first placeholder unit is
flipped. See `docs/evidence/decomp/unit_rules_category_e_pool_labels.md` for
the full record.

* Every `lbl_eu_80669Exx` / `lbl_eu_806656FC` undefined is cleared: the
g3d_resmat `.sdata2` blob is split in source so those words are real global
definitions (`add_symbols` value extended + ratcheted for the two 0.0f words),
and `CLibG3d.cpp` splits its `.sbss` so `lbl_eu_806656FC` exists.
* The parked CfObjectActor dtor fix landed **source-side** (`extern "C"` on the
definition in `CfObjectEne.cpp`), so no new `exact_renames` field/sign-off is
needed.
* The nw4r fastcast wrappers (`nw4r/math/math_arithmetic.h`) now use the raw
`__OSu16tof32`/`__OSf32tou16`/`__OSs16tof32`/`__OSf32tos16` forms — the old
by-value + volatile-out shape made GC/3.0a5.2 emit extra copies. This is the
missing piece for §4 matching: `math_triangular.cpp` is **4/4 FULL_MATCH** and
flipped to `Matching` (raw data gate MATCH, size PASS, registry accepted).
* `math_arithmetic.cpp` is not flippable yet: `FExp`/`FLog` are CODE_MATCH
(hexdiff 77.1%/88.6%, 0 structural, reg_swap only) and the witness refuses on
an ABI-boundary register permutation, so only FULL_MATCH can accept. Its
`data diff --postprocess` is already MATCH.
* `ninja build/us/main.elf` now stops on exactly the §1(b) tickets (12 ×
`KyoshinHeap_*`, `UnkClass_80460308::processData`, `lbl_eu_80529690`).
`ELF_linker.c:2295` still follows the undefined report.

**Update (2026-09-29, same day):** the working-tree hygiene items above are
closed and the first §1(b) ticket is done.

* Stray `src/**/*.tmp` backups moved to `.scratch/e_units/backups/` (smell
freshness gate green again) and `CBattleManager.cpp` compiles again (duplicated
`__declspec`, 13 UTF-8 glyphs in an SJIS TU, four missing closing braces —
recorded in the evidence doc; that TU is `NonMatching`, so the link never saw
it).
* `kyoshin/code_800A75FC.cpp` is fixed as a pair: `extern "C"` on 48
definitions (45 `KyoshinHeap_*`, `func_800A8E6C`, `func_800A9134`,
`sinit_800A9F40`) **and** the unit's `.rodata`/`.data`/`.bss`/`.sbss`
reconstructed from the retail split (contents and sizes; `run.py data diff`
is ok on all four, `.data`/`.rodata` byte-identical). The 12 `KyoshinHeap_*`
undefineds are gone. Residual for that unit: `.sdata2` 0x4 vs retail 0x0 (one
zero-float pool word owned by the still-open `func_800A76EC`) plus the four
open functions.
* Remaining frontier is now two symbols: `UnkClass_80460308::processData()`
(owner `monolib/src/lib/UnkClass_80460308.cpp`: 2 functions, split FAIL — the
unit itself needs reconstruction) and `lbl_eu_80529690` (owner
`kyoshin/cf/code_800C17DC.cpp`: `Matching`, `.data` 0x228 at retail, data still
unreconstructed). `ELF_linker.c:2295` persists, so it is not purely an
error-count artefact.

**Update 2 (2026-09-29): all three of those are closed, and the frontier is
much larger than this ticket's error list showed.**

* `ELF_linker.c:2295` was a bug in the frozen §17.6 postprocess (the `.sdata2`
swap rules rewrote the *section* symbol's `st_value`); fixed with a
`STT_SECTION` guard — no table change. The link now reports its real errors.
* mwldeppc caps its undefined list (~100 error blocks), so the "12/14/39
symbols" figures seen here were never the frontier. `tools/coop/link_undefs.py`
(replaces the capped output) measures the real one: **1235 missing symbols**
across the link inputs, dominated by data labels of Matching units that retail
objects reference (`g3d_resmat` 304, `CfObjectEne` 124, `CMenuBattleChain` 84,
`CVision` 68, ...). Burning that down is this ticket's `§1(b)` work at scale.
* `processData` = the unit's adler32 body under an invented name → renamed to
the retail symbol, now 100% FULL_MATCH. `lbl_eu_80529690` (the CfObjectMove
vtable, 909 relocs) and that unit's `.data`/`.bss`/`.sdata2` are reconstructed;
`run.py data diff` is MATCH there. 25 `g3d_resmat` pool labels are now real
source definitions (recipe in the evidence doc).
* Still open: `math_arithmetic` FExp/FLog (FULL_MATCH needed) and
`snd_BasicSound` (28/35), plus the 1235-symbol burn-down that gates
`ninja build/us/main.dol`.

You are working in the Xenoblade Chronicles decomp fork. **Read the repo rules first**
(`AGENTS.md`), then load `.agents/skills/xenoblade-decomp/SKILL.md` (this *is* a
decomp/matching task: you will edit `src/**`/`libs/**`, run `hexdiff`, the data gate
and the link). Non-negotiables from `AGENTS.md`: use **`.venv/bin/python3`**, **never
run `git`**, prefer `tools/coop/hexdiff.py` over raw `ninja`, keep the repo root clean
(`/tmp` or `.scratch/` for scratch).

Background you need: `docs/evidence/decomp/unit_rules_category_b.md`
(§7.8 = the mwldeppc SIGSEGV fix, §7.9 = this undefined-symbol frontier, §7.10 = A1
probe results) and `docs/handoff/unit_rules/README.md` (state + policies).
`PLAN.md` §17 governs what counts as legitimate matching; **§17.6 object reshape rules
are frozen** and must not be extended without human sign-off (§“Frozen manifest” below).

## 1. The symptom

`ninja build/us/main.elf` no longer SIGSEGVs (§7.8) but the link stops with roughly
25 `undefined:` errors plus `internal linker error: ELF_linker.c:2295`. Two classes:

**(a) pool-label references (this prompt's scope).** Undefined symbols of the form
`lbl_eu_80669Exx` (and `lbl_eu_806656FC`, `lbl_eu_80669EEC/EF0`, …), referenced e.g.
by `nw4r::math::FrSqrt(float)` in `math_arithmetic.o`,
`nw4r::math::Atan2FIdx/SinFIdx/CosFIdx` in `math_triangular.o`,
`nw4r::snd::detail::BasicSound::Stop` etc. Regenerate the current list with:

```bash
ninja build/us/main.elf 2>&1 | tee .scratch/hybrid_link.log | grep -B1 "Referenced from"
# referencing unit histogram:
grep -oE "in [A-Za-z0-9_]+\.o" .scratch/hybrid_link.log | sort | uniq -c | sort -rn
# name -> retail owner unit (address -> splits.txt lookup):
.venv/bin/python3 .scratch/unitrules_b/owner_of_undef.py .scratch/hybrid_link.log
```

**(b) real missing definitions (do the same workflow, separate tickets).**
`cf::CfObjectActor::~CfObjectActor()` (`CfObjectEne.cpp` writes the dtor as a free
function, so MWCC mangles it with a parameter list; the one-line `exact_renames` fix is
parked in the table awaiting sign-off), 12 × `KyoshinHeap_*` owned by
`kyoshin/code_800A75FC.cpp`, and `UnkClass_80460308::processData` owned by
`kyoshin/cf/code_800C17DC.cpp`. The `ELF_linker.c:2295` internal error is believed to be
a *consequence* of the undefineds — re-check once the list is empty.

## 2. Why the pool labels are undefined (verified 2026-09)

* **The link is a hybrid.** `build/us/main.elf.rsp` has 1161 inputs; **681 come from
  `build/us/obj/**`** (dtk's retail split objects) and 480 from `build/us/src/**` (our
  decompiled objects). The choice is the `Object(Matching, …)` vs
  `Object(NonMatching, …)` marker in `configure.py` (grep the two math units to see it):
  `NonMatching` units link the **retail placeholder** copy, so the DOL stays valid while
  the unit is incomplete. `math_arithmetic.cpp` / `math_triangular.cpp` are currently
  `NonMatching` and therefore linked from `build/us/obj/`.
* dtk's split object for the owner of those pool words —
  `nw4r/src/g3d/res/g3d_resmat.cpp` — defines them **globally** (`nm` shows
  `R lbl_eu_80669E14`), which is how the placeholders can reference them. Retail's
  `.sdata2` slice for that unit is 0x710 bytes; the labels start at 0x398
  (`lbl_eu_80669E00`), `lbl_eu_80669E14` = `0x3f000000` at object offset 0x3AC.
* **Our** `build/us/src/nw4r/src/g3d/res/g3d_resmat.o` has the same **bytes** (its
  `.sdata2` is 0x728 bytes and the unit rule drops the leading 0x18:
  `drop_data_range=((".sdata2", 0, 24),)`), verified by
  `ours[0x18:] == retail` byte-for-byte. But it does **not** define the labels, and the
  mechanism that names pool words —
  `rename_pool_symbols` in `tools/postprocess_reloc_names.py` — only *renames* existing
  `@N` symbols in `.sdata2` that are **reloc-referenced inside the same object**; these
  words are referenced only from *other* objects. `add_symbols` adds **local** symbols
  and therefore cannot satisfy external references.
* Our `math_arithmetic.o` **defines its own copies** of the constants locally
  (`.sdata2` 0x1C bytes, symbols `@1494`, `@1495`, `@1496`, `@1514`, …), while our
  `math_triangular.o` **references the retail names as UNDEF** (`U lbl_eu_80669E20`, …)
  and has a big `.rodata` (0x1010). The unit table already carries
  `math_arithmetic.o: extern_data_sections=('.sdata2',)`.

## 3. The open question you must answer first

For each referencing unit, decide which of these is true and act accordingly:

1. **Our object references the retail labels as UNDEF** (the `math_triangular.o` case).
   Flipping `NonMatching -> Matching` is not enough by itself: the labels must be
   defined by their owner (`g3d_resmat.o`) in the link.
2. **Our object defines its own local pool words** (the `math_arithmetic.o` case).
   Then the reference is local, but the *bytes* land in the wrong slice versus retail
   (retail keeps them in `g3d_resmat.cpp`'s slice), so a plain flip can change the
   linked layout. The project's mechanism for this is
   `extern_data_sections` (make the local definition external) **plus** `exact_renames`
   (`@1494 -> lbl_eu_80669E14`, …) so the reference carries the retail name and resolves
   against the owner's definition.

Then make the owner define them. Check exactly what `g3d_resmat.o` has:

```bash
nm -S build/us/src/nw4r/src/g3d/res/g3d_resmat.o | grep '@'      # local pool symbols?
nm -S build/us/obj/nw4r/src/g3d/res/g3d_resmat.o | grep lbl_eu_80669E   # retail: R symbols
```

* If the object has `@N` words at the shifted offsets (retail 0x398+0x18 = 0x3B0, …),
  extend the unit's existing `pool_patterns` value so the content-based renamer gives
  them the retail names (a **changed value** on an existing field, which the frozen gate
  permits — record it with `tools/check_unit_rules_frozen.py --ratchet`, which prints the
  payload diff for review). Watch the renamer's preconditions (symbol must be `@…`,
  defined in `.sdata2`, and referenced by a reloc in the same object — if it is *not*
  referenced, the renamer cannot touch it and you need option 2 of §4 below).
* If it has no symbols there, the words are unlabeled; do **not** invent source constants.

Also note the compile flags include `-str reuse,pool,readonly`, i.e. the original build
used MWCC's pool merging, which is exactly why a constant used by the math units can be
*owned* by `g3d_resmat.cpp` in retail. A legitimate outcome of your investigation is
"the ownership split must be reproduced via the pool-rename mechanism / link order",
not "add a global alias".

## 4. What you may change

* **Source/matching (preferred):** edit the referencing units' sources so they match
  (`hexdiff --all` all functions matched, `run.py data diff --postprocess <unit>`
  MATCH, raw `.text`/`.data` within the split budget). Then change the unit's marker in
  `configure.py` from `NonMatching` to `Matching` (this is the normal progression; it
  swaps the link input from the retail placeholder to our object).
* **Existing rule values** on units that already have the field (`exact_renames`,
  `extern_data_sections`, `pool_patterns`, …) may be extended — value changes are
  reviewable and ratcheted.
* **Not allowed without human sign-off:** adding a *new* field to a unit
  (`--write --force` on the frozen manifest), fabricating source symbols that retail's
  own object did not have, `add_symbols` for cross-object references (local-only), any
  asm-side workaround, or touching `git`.

## 5. Verification (all of it)

```bash
.venv/bin/python3 tools/coop/hexdiff.py nw4r/src/math/math_arithmetic --all      # all funcs matched
.venv/bin/python3 tools/coop/run.py data diff --postprocess main/nw4r/src/math/math_arithmetic   # MATCH
ninja build/us/main.elf 2>&1 | tee .scratch/hybrid_link_after.log               # undefined list shrinks
diff <(grep undefined: .scratch/hybrid_link.log | sort -u) \
     <(grep undefined: .scratch/hybrid_link_after.log | sort -u)                # only your labels disappear
.venv/bin/python3 tools/check_unit_rules_frozen.py --check                      # still OK
.venv/bin/python3 -m unittest discover -s tools/coop/tests -t .                 # 457 tests green
```

When the undefined list is empty: `ninja build/us/main.dol && ninja dol_diff`
(`build/us/main.elf` -> `.dol` -> compare against retail). If the pool words end up at
different addresses than retail (`config/us/symbols.txt`: `lbl_eu_80669E14` =
`0x80669E14`), the ownership/link-order work in §3 is still pending — a link that
resolves can still produce a wrong DOL.

Useful helpers already in the tree: `.scratch/unitrules_b/owner_of_undef.py`
(undefined name -> retail owner unit), `.scratch/unitrules_b/tail_diag.py`
(object symbol/section diffs), `docs/handoff/unit_rules/*.md` (the a–d prompts for the
per-unit rule classes you may encounter).

**Update 3 — measured completion plan (2026-09-29).**

`tools/coop/link_undefs.py` is the acceptance instrument from here on (mwldeppc's
own list is capped at ~100 error blocks).  Current frontier **1235**, and the
breakdown that defines the work:

1. 85 local `@N`/`@LOCAL@` artifacts — §17.6 rule work, no source changes (~1 session).
2. 727 data symbols, 14.4 KB total: 323 whose bytes are already emitted (split +
   name only, recipe proven on `g3d_resmat`), 404 whose bytes are absent (one
   data pass per owner unit, KyoshinHeap/`code_800C17DC` recipe; 20 units own them).
3. 421 function bodies, 57.7 KB / ~14 k instructions: 130 are <=0x10 B generated
   stubs (`CActorParam_UnkVirtualFunc*` 70, `CfObjectModel_UnkVirtualFunc*` 6,
   `EffCtl_EmptyStub*`, accessors), ~0x5BEC B are `func_*`/voice-thread scripted
   code, and ~0xDCE8 B are real bodies concentrated in `CfGimmick` (41),
   `UnkClass_80466348` (36), `CChainActorList` (25), `CfObjectEne` (15),
   `CVision` (10).  The last bucket is the long pole; the units also carry more
   unwritten functions than the link-blocking globals (e.g. `CfGimmick` is
   37/51 on hexdiff).
4. Relink + `main.dol` + DOL verification last (1 session, needs 1-3 at 100%).

Order: items 1-2 of the ticket (math_arithmetic FExp/FLog register swaps;
snd_BasicSound 5 residuals + the +0x1E0 split overrun), then 1. and the 130
stubs (17% of the frontier for ~3 sessions), then the data passes, then the
real bodies in parallel.

**Update 4 (frontier burn-down started).** `tools/coop/link_undefs.py` is the
measuring instrument; the frontier was 1235 and is now **1003** (linkage batches: CfObjectEne + 11 units at the definition level, then 24 headers at the declaration level).  First batch:
`CfObjectEne.cpp` (124 -> 10) by adding `extern "C"` to definitions whose MWCC
mangling differs from the retail literal (skip names a header declares, and
skip definitions whose natural mangling already matches).  Recipe, residual
cases (2 header-declared names) and the stale-`@N` repair method are in
`docs/evidence/decomp/unit_rules_category_e_pool_labels.md`.

**Update 5 (pool-label class investigated).**  The `g3d_resmat` `.sdata2` pool
(293 labels) cannot be named today: MWCC 8-aligns every `.sdata2` object, so
retail's 4-aligned labels cannot be individual source objects without shifting
the layout, and the §17.6 `add_symbols` mechanism runs out of string-table space
after ~51 new names.  Measurements, the failed experiment (frontier 1022 -> 780
with a +8-byte layout drift) and the two viable tool-level fixes are in
`docs/evidence/decomp/unit_rules_category_e_pool_labels.md` (update 6).
