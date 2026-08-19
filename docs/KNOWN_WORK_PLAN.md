# Known work plan — CriWare EQUIVALENT_MATCH cleanup (agent handoff)

Context: after the 2026-07-26 EQUIVALENT_MATCH audit + ppc_equivalence
soundness fix (commits `3c3cbfb` and the preceding two-phase-prove commit),
30 targets remain at `CODE_MATCH` / `REVALIDATION_REQUIRED`. They fall into
two independent work items. Both end with `cycle` acceptance at
`EQUIVALENT_MATCH` + split-size PASS. Do not regress the green suites:
`python3 -m unittest discover -s tools/ppc_equivalence/tests -p "test_*.py"` (1906)
and `python3 -m unittest discover -s tools/coop/tests -p "test_*.py"` (184).

Read first: `.agents/skills/xenoblade-decomp/SKILL.md` (hexdiff rapid loop,
cycle acceptance, §17.6 exceptions). Source must stay high-level C — no
register/stack micro-management, no inline asm.

---

## Work item 1 — fix 4 genuinely `not_equivalent` CriWare functions

These were falsely accepted by a legacy promotion tool; the current engine
refutes them. All four are tiny (4–5 instruction) void functions where the
decomp **clobbers r3 with a leftover address value** and retail does not.
The engine compares r3 even for declared-void returns (deliberate fail-closed:
`contract.py` `observables_for_exit` never drops r3; only r4 is dropped when
`returns_i64=False`). The counterexamples are therefore on dead-but-compared
r3, e.g. `r3: 0x7effffff != 0x81000000`.

**Do NOT "fix" this by relaxing the engine contract** — that is a
soundness-relevant decision requiring adversarial review (see Work item 3,
not recommended). The intended fix is ordinary matching: restyle the C so
MWCC does not reuse the argument register for the address temporary.

### 1a. `us-8039a658` SVM_SetCbLock and 1b. `us-8039a66c` SVM_SetCbUnlock
- Source: `libs/CriWare/src/adx/svm/svm.c:84` and `:92` (identical pattern,
  different globals `lbl_eu_805F2700` / `lbl_eu_805F2708`).
- Unit: `CriWare/src/adx/svm/svm`. Fuzzy 54.0.
- Retail (`build/us/asm/CriWare/src/adx/svm/svm.s`):
  `lis r6,ha; addi r5,r6,lo; stw r3,0(r6); stw r4,4(r5); bclr`
  — address kept in r6/r5, r3/r4 (args) untouched.
- Decomp: `lis r5,ha; stw r3,0(r5); addi r3,r5,lo; stw r4,4(r3); bclr`
  — r3 clobbered by the second address form.
- Hypothesis to try first: express the global as a two-field struct
  (`struct { u32 cb; u32 ctx; }`) declared with the retail linker name via
  `extern "C"`, then `g.cb = (u32)cb; g.ctx = (u32)ctx;` — or compute the
  address once into a local pointer and store both fields through it without
  re-deriving. Iterate with the ~1s loop:
  `python3 tools/coop/run.py build CriWare/src/adx/svm/svm`
  `python3 tools/coop/hexdiff.py CriWare/src/adx/svm/svm --symbol SVM_SetCbLock --json`
  (drive `mismatch_count` to 0 or stall after 3 attempts per protocol).

### 1c. `us-8039e76c` criCrw_GetVersion and 1d. `us-803bdf28` DCT_GetVerStr
- Sources: `libs/CriWare/src/adx/std/cri_crw_std.c:10`,
  `libs/CriWare/src/sofdec/sfdcore/dct/dct_ver.c:10`. Fuzzy 94.0 both.
- Pattern: store a string-literal address into a global pointer.
  Retail uses r3 for the literal address, r4 for the global; decomp swaps
  (r3↔r4, 100% pure reg-swap) and leaves r3 = literal address.
  Retail ends with r3 = *input-preserved / literal addr in r3 stored through
  r4* — check `build/us/asm/...` for exact allocation before editing.
- Same approach: restyle the store expression (single pointer local,
  different expression order, named temp for the literal) until hexdiff
  agrees. These are 4-instruction functions; expect quick convergence.

### Acceptance (each of the 4)
```bash
python3 tools/coop/run.py cycle <target-id> \
  --hypothesis "<regalloc hypothesis>" --next-change "<next>"
```
Must print `status: EQUIVALENT_MATCH` and `size: PASS`. Then confirm the
proof is honest (no mem1 assumption needed):
`python3 tools/coop/run.py equivalence check-unit <unit> --symbol <sym> --no-build`
→ `status: equivalent`.

---

## Work item 2 — match + certify `SFLIB_SetErr` (unblocks 26 dependents)

The 26 remaining `REVALIDATION_REQUIRED` targets (SFADXT_*, SFAOAP_*,
SFMEM_*, SFMPS_*, SFMPV_*, SFMPVF_*, SFUO_*, SFVOM_*, SFXZ_SetTagInf,
ADXPD_Stop, MPV_SetMbCb — full list: query
`jq '[.targets[] | select(.workflow_status=="REVALIDATION_REQUIRED")]'
tools/coop/targets.json`) are all 3-instruction wrappers ending in
`b SFLIB_SetErr`. The engine correctly fails closed:
`unsupported: tail-call target 'SFLIB_SetErr' has no matched-callee lemma`
(verified by adversarial review — not a checker bug). Each is otherwise a
100% pure reg-swap. Once `SFLIB_SetErr` is accepted with a current
certificate, re-cycling them should prove `EQUIVALENT` immediately via the
certified-callee path (`equivalence_check._load_certified_callees`).

### 2a. Match `us-803c34fc` SFLIB_SetErr
- Source (partial, poor): `libs/CriWare/src/sofdec/sfdcore/sfd/sfd_lib.c:39`
  — current object: **71 mismatches, 17.4% match, size 0x158 vs retail
  0x140**. This is real matching work, not a quick fix.
- Retail asm: `build/us/asm/CriWare/src/sofdec/sfdcore/sfd/sfd_lib.s:156-246`
  (0x140 bytes). Behavior:
  1. Standard frame (`stwu r1,-0x20`; saves r31, LR).
  2. `err_code == 0` → return 0.
  3. If error-handler object `lbl_eu_80606E34 != 0`: store `val` at
     `lbl_eu_80568720+0xc`, store `&err_code` (stack slot r1+8) at
     `lbl_eu_80568720+0x18`, then **virtual call**
     `handler->vtable[0x24](handler, &lbl_eu_80568720[1])` (`bctrl`).
  4. If `val == 0`: global ctx `lbl_eu_80606E20` — if `ctx[2]==0` store
     err_code; if `err_code!=0 && ctx[0]!=0` call `ctx[0](ctx[1])`.
  5. Else (`val != 0`): handle `h = val` — if `h->0xa10==0` store err_code;
     if `err_code!=0 && h->0xa08!=0` call `h->0xa08(h->0xa0c)`.
  6. Epilogue; check `.L_803C3628` in the asm for the exact return value.
- Complications:
  - **Two indirect `bctrl` calls** (`has_indirect_calls: true`). The proof
    will likely need the engine's virtual-call / opaque-callee machinery
    (`vtable_obligations.py`, `virtual_call` context in `_prove_bytes`) or
    will return inconclusive on the call paths. If the unlinked proof stays
    inconclusive, try `cycle --linked`.
  - Struct-field access through `val` as a pointer (offsets 0xa08/0xa0c/
    0xa10) — model as a struct, not raw arithmetic, per source-language
    policy; keep the retail global names via `extern "C"` decls as the
    existing source does (`lbl_eu_*` approved for reloc naming, §17.6).
- Workflow: `targets claim us-803c34fc --owner <agent>` → hexdiff loop →
  `cycle us-803c34fc --hypothesis "..." --next-change "..." [--linked]` →
  must end `status: EQUIVALENT_MATCH` + `size: PASS` → `targets release`.

### 2b. Re-accept the 26 dependents
```bash
python3 - <<'EOF' > /tmp/tailcall_ids.txt
import json
d = json.load(open('tools/coop/targets.json'))
ids = [t['id'] for t in d['targets']
       if t.get('workflow_status') == 'REVALIDATION_REQUIRED'
       and t.get('equivalence_status') == 'inconclusive_unvalidated_callee']
print(' '.join(ids))
EOF
python3 tools/coop/batch-cycle.py $(cat /tmp/tailcall_ids.txt) \
  --default-hypothesis "SFLIB_SetErr certified; tail-call lemma available" \
  --default-next-change "re-accept via certified-callee proof" \
  --summary /tmp/tailcall_reval.json
```
Expect all 26 PASS. Any FAIL means the callee lemma did not attach —
inspect with `check-unit <unit> --symbol <sym> --no-build` and check the
certificate's `callee_contracts` source is `certified:<sha>` (trusted) not
opaque (untrusted callees cap Tier C and block promotion).

---

## Work item 3 (optional, NOT recommended without review) — engine contract relaxation for declared-void

The Work-item-1 counterexamples exist because r3 is compared even for
declared-void returns. Dropping r3/r4 from the compared set when
`AbiShape.declared_return == "void"` is defensible (both are caller-saved)
but is a **contract soundness change**: it weakens refutation power for
every future proof. If pursued: same process as the previous engine fix —
concrete proposal, adversarial review by independent agents (kimi + flash),
regression tests, re-bless `allowed_engine_sha256` in `coop.json`, and
revalidate affected certificates. Only consider if Work item 1 stalls on
MWCC regalloc after the bounded-attempt protocol (3 non-improving attempts
→ stall packet, not acceptance).

---

## Final verification (after items 1 + 2)

```bash
python3 -c "
import json, collections
d = json.load(open('tools/coop/targets.json'))
print(collections.Counter(t.get('workflow_status') for t in d['targets']
      if t.get('status') == 'CODE_MATCH'))"
# expect zero REVALIDATION_REQUIRED
python3 tools/coop/run.py targets audit-promotion --dry-run   # expect 66→0 trend; no new affected
python3 -m unittest discover -s tools/ppc_equivalence/tests -p "test_*.py"  # 1906 OK
python3 -m unittest discover -s tools/coop/tests -p "test_*.py"             # 184 OK
python3 -m tools.ppc_equivalence differential                              # 362/362
```

Log all attempts (`attempts.jsonl` via `cycle`), and if a reusable MWCC
regalloc pattern emerges from Work item 1, append it to
`docs/MWCC_CASES.md` in the same session.
