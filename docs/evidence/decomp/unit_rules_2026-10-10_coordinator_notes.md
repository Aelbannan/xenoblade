# UNIT_RULES 2026-10-10 batch — coordinator notes

<!-- Coordinator: main-checkout session. Companion to:
     docs/evidence/decomp/unit_rules_category_a_residual_2026-10-10.md,
     docs/evidence/decomp/unit_rules_category_b_run_2026-10-10.md,
     docs/evidence/decomp/unit_rules_category_c_run_2026-10-10.md,
     docs/evidence/decomp/unit_rules_category_d_decision.md (+ _inventory.md). -->

## Landed in the main checkout (this coordinator)

- **Policy:** `PLAN.md` §17.6 "Relocation name drift" row removed (replaced by a note); `SKILL.md`
  angles/exceptions/routing updated; `docs/MWCC_PATTERNS.md` §1 marked SUPERSEDED;
  `tools/pi_harness/lint.py` no longer permits `extern "C" lbl_*`; `tools/pi_harness/src/witness-hints.ts`
  reloc hint rewritten; `tools/coop/reloc_map.py` drift hints no longer propose `extern "C"`
  retail-name spellings (33 reloc-map tests pass).
- **A-residual:** 9 inert pure-`exact_renames` keys retired; evidence + 4 kept keys in
  `docs/evidence/decomp/unit_rules_category_a_residual_2026-10-10.md`.
- **B batch applied + build-verified:** 29 keys deleted + 74 fields pruned; link inputs and
  `main.elf.undefs.json` byte-identical; ratcheted (396 → 367 at the time; a sibling batch later
  moved the table to 366 / baseline 366 / 149 retired).
- **B ported:** run doc `unit_rules_category_b_run_2026-10-10.md` copied to main (the packets doc
  was already identical). **Prose deliberately not ported** — main's table comments are partly
  *newer* than B's (sibling updates on 2026-10-10, e.g. the `wave DL r2: retired 2026-10-10`
  comment); porting B's stale prose would regress them. B's verdicts live in the run doc.
- **C ported:** run doc + `.scratch/catc/` scripts; `coop.json` (`./ninja`) and build artifacts
  excluded; addendum records the live-gate `rfc_*` disproval.
- **D ported:** `unit_rules_category_d_decision.md` + `unit_rules_category_d_inventory.md`
  (decision packet: Option C recommended, 279-row inventory, 5-batch plan; 0 deletions).
- **Claims + metadata:** B/C/D claims applied centrally (B groups: 29 deleted / 38 pruned / 4 align /
  3 WIP; D: all 279 inventory rows that are still EDS — 5 rows no longer EDS in main:
  `CCur.o`, `CMenuBattleChain.o`, `CMenuQuestLog.o`, `pluginEve.o`, `snd_AxfxImpl.o`; C: 28 keys).
  Class/owner normalized to the derived triage (`content` > `layout` > `rename`; owner
  `category-c` for content, `category-d` for EDS, `category-b` for layout-only), reasons normalized
  (content → C doc §2 pointer, EDS → D bucket reason). Final `--report`: content 28, layout 294,
  rename 43, other 1; owners a=10, b=59, c=28, d=269; **0 unassigned**; `--check` green.

## C cross-check: `rfc_mx_fsm.o` / `rfc_port_fsm.o` are NOT stale ACCEPTs

C's static packet flagged `us-80301fbc` (`rfc_mx_sm_state_disc_wait_ua`) and `us-80302874`
(`rfc_port_sm_orig_wait_sec_check`) as likely stale FULL_MATCHes. Live hexdiff in main says
otherwise:

```
hexdiff RVL_SDK/.../rfc_mx_fsm  --symbol rfc_mx_sm_state_disc_wait_ua   → 100.0% | 0 structural | 0 reg_swap | size PASS
hexdiff RVL_SDK/.../rfc_port_fsm --symbol rfc_port_sm_orig_wait_sec_check → 100.0% | 0 structural | 0 reg_swap | size PASS
```

Both show only reloc-*name* drift (`@868`→`@540`, `@1465`→`@709`, section-symbol `...data.0`),
which the comparator ignores. **Do not demote; do not delete the `addend_sets` rules on that
basis.**

## B batch — build verification in main (2026-10-10)

B's worktree could not compile, so its delta was harvested independently (paseo terminal →
`/tmp/b_delta.json`), audited, applied to main, and verified with a real link:

1. **Routing audit:** all 29 deleted keys are `NonMatching` units with no `.reloc.o` in the link
   edge; 36/38 prune keys likewise, and the two `Matching` ones (`CProcess.o`, `HBMAxSound.o`) link
   via `.link.o`, which bypasses `link_reloc_postprocess`. So the pass never sees them.
2. **Applied:** 29 keys removed + 74 fields pruned, every prune re-verified empty by re-import;
   backups `.scratch/unitrules_b_verify/`.
3. **Link inputs unchanged:** after `run.py configure`, the `main.elf` link edge is identical to
   baseline — 1161 objects, 77 RELOCPOST copies, 0 added/removed.
4. **Link outcome unchanged:** `ninja build/us/main.elf` (red baseline, as known) produces a
   byte-identical `main.elf.undefs.json` (`json equal: True`, 1475 symbols, 0 added/removed).
5. **Ratcheted:** baseline 396 → 367, retired 148 at the time; `--check` green.

Merge note: B's branch still carries the base value `CMCCylinderGauge.o extern_data_sections =
('.sdata2', '.rodata')`; main has `('.sdata2',)` (sibling change at 16:27, ratcheted 16:50). A
3-way merge resolves to main (only one side changed), so no manual conflict is expected there.

## Category status / merge guidance (2026-10-10)

| cat | branch | result | merge? |
|---|---|---|---|
| A-residual | (main checkout) | 9 retired, 4 kept | n/a |
| B | `paseo/unitrules/b` | finished; 29 keys + 74 fields applied + build-verified; run doc ported | **do not merge** — the mechanical delta is already in main; a merge would replay B's older prose over newer main comments (keep main's table) |
| C | `paseo/unitrules/c` | finished; 0 deletions; run doc + `.scratch/catc/` ported | **do not merge** — `coop.json` `./ninja` tweak must not land; doc already ported |
| D | `paseo/unitrules/d` | finished; 0 deletions; decision + inventory docs ported | **do not merge** — docs already ported; branch has nothing else |
| E | `paseo/unitrules/e` | running (toolchain wired via paseo terminal) | port after it finishes, with the harvest → configure → link-A/B treatment |

Everything valuable from B/C/D is now in main; the branches are archival. The human still owns any
git operations (coordinator must not run git).

## Open items carried forward

- A-residual kept 4 (`CArtsSet.o`, `CtrlMoveNpc.o`, `CLibCriMoviePlay.o`, `ut_Font.o`) need a green
  tree for `ninja diff` / link proof.
- `ut_Font.o` data-ownership anomaly: `nw4r/ut/ut_CharStrmReader` (Matching) already defines
  `lbl_eu_8056AE30`, and the rule makes RVL `ut_Font` define the same label — resolve with E's
  pool-ownership work before touching the key.
- `CErrorWii.o` inherited item paused: it needs `extern "C" lbl_eu_*`-style declarations that the
  removed §17.6 row used to approve → owner sign-off required.
- C's strongest runnable next step (main has the toolchain): `OSNet.o` hoist
  (`(void)"NWC24iPrepareShutdown";` to the top of `NWC24iPrepareShutdown()`), then verify the 7 FULL
  targets + raw `.data` order and delete the rule.
- D decision pending: owner choice between keep-EDS / source-owned data / **Option C (code-only
  matching)** + the 5-batch ratchet plan in `unit_rules_category_d_decision.md` §2.
