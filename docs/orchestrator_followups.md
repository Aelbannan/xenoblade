# Orchestrator follow-ups (tracked; /tmp/xb is wiped on reboot)

Reviewers append here; the orchestrator never edits source for these.

## Mechanical / data
- 113 arity-drift candidates from `tools/coop/census_fixes.py sigs`: triage as `callee-signature` tasks.
- Forced-section padding (data-reconstruction debt): CPartsChange .sdata/.sdata2 tails, code_800C17DC gap_11, LODMemMan gap_09.
- gpCullingFrustum .sbss: US gives it to g3d_scnroot, JP to g3d_scnobj. Ambiguous; needs evidence before moving.
- Split UNIT_RULES keys by path (nw4r/nw4hbm share basename keys).
- Pre-existing debt under the new ban counters: scalar_ptr_alias 84, cast_addr_alias 77, addr_extern 25, single_use_inline 109.

## Tasks parked from earlier waves
- NW4HBM-LINKLIST-INIT, CAMMAN-VIEWFALLOFF, OPEN-CFTFILE-FMTSTR.
- CtrlPc: regain after the approved revert (faithful 17 -> 26).
- code_804BC9EC.hpp label 1 -> 2.

## Questions for the human (milestone)
- bdat column first-byte read idiom: which spelling is the original?
- nw4r g3d assert-inline layout: what evidence is needed to restore the original layout?
