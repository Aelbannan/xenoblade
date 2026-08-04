#!/usr/bin/env python3
"""
Read-only pre-classifier for no-SMT routing (triage).

For a unit, classify every non-accepted function target
(NOT_STARTED / COMPILES / CODE_MATCH / HIGH_MATCH / STRUCTURAL) into:

  regswap_only  — the target's retail instruction stream, compared
                  position-aligned against its nearest matched sibling
                  (same unit, FULL_MATCH / EQUIVALENT_MATCH), differs only
                  in register operands. This is the register-renaming
                  witness's exact precondition (docs/ppc_equiv_work/
                  31-reg-swap-witness.md), so the harness can route these
                  straight to a witness-only `run.py cycle` with ZERO LLM
                  rounds: if the witness certifies, the target flips to
                  EQUIVALENT_MATCH; if not, it fails cheaply (one build +
                  witness attempt, no tokens) and falls back to the batch.
  strict        — byte-identical to a sibling, or trivially template-shaped
                  (constant-return / field load-store / getter pattern:
                  size <= 0x20 with only li/lis/lwz/stw/blr). Highest LLM
                  hit rate — the harness routes these first in the batch.
  structural    — real opcode/immediate/branch/reloc differences vs the
                  nearest sibling, or a stream the witness can never
                  certify (reject-list opcodes, indirect dispatch, ...).
  unknown       — no retail ASM, no matched sibling, or unparseable.

The witness (tools/coop/lib/renaming_witness.py) is the source of truth —
it runs inside `cycle`. Triage is a PREDICTOR: a false `regswap_only`
routes to a witness run that fails cheaply and falls back to the normal
path; a false `structural` only wastes a similarity pointer. Bias toward
`regswap_only` only when the mnemonic sequences match position-aligned AND
every diff is register-only per the witness's own role table.

Read-only: never writes targets.json, no git ops, no SMT.

Usage:
  .venv/bin/python3 tools/coop/triage.py --unit kyoshin/cf/CfRes --json
  .venv/bin/python3 tools/coop/triage.py --unit kyoshin/cf/CfRes --limit 10 --json
"""

import argparse
import json
import os
import re
import sys
from collections import Counter

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TARGETS_JSON = os.path.join(REPO_ROOT, "tools", "coop", "targets.json")

# Ensure the repo root is on sys.path (same pattern as hexdiff.py) so the
# witness / ppc_equivalence imports below resolve when run as a script.
sys.path.insert(0, REPO_ROOT)

ACCEPTED = frozenset({"FULL_MATCH", "EQUIVALENT_MATCH"})
CANDIDATE = frozenset({"NOT_STARTED", "COMPILES", "CODE_MATCH", "HIGH_MATCH", "STRUCTURAL"})

# Trivially template-shaped mnemonics for the `strict` heuristic (spec:
# "size <= 0x20 with only li/lwz/stw/blr pattern"). `lis` is the obvious
# sibling of `li`; `mr` covers `mr r3,r4; blr` wrappers. Everything else
# (lhz/lbz/addi/...) requires an actual look at the target -> not strict.
STRICT_MNEMONICS = frozenset({"li", "lis", "lwz", "stw", "blr", "mr", "nop"})
STRICT_MAX_SIZE = 0x20

# How many same-size siblings we try before giving up on a reg-swap
# prediction (each is a candidate template the witness could certify).
MAX_SIBLING_ATTEMPTS = 16

# ---------------------------------------------------------------------------
# Assembly parsing — copied (with attribution) from tools/coop/sim_schedule.py
# (itself adapted from tools/batch_cluster.py::analyze_instrs and
# tools/batch_getters.py::parse_instructions). We deliberately do NOT import
# those pipelines: sim_schedule is a CLI script and the legacy batch tools
# carry target-mutation paths (load_targets / save_targets / cycle_one).
# The only deviation: split_asm_blocks keeps a LIST of bodies per symbol so
# duplicate `.fn` blocks (thunks/aliases) do not silently clobber each other.
# ---------------------------------------------------------------------------

_FN_RE = re.compile(r'^\.fn\s+"?([^",]+)"?\s*(?:,.*)?$')
_ENDFN_RE = re.compile(r"^\.endfn(?:\s+.*)?$")

# One function-header comment in an objdiff listing, immediately preceding
# the .fn line:   # .text:0x770 | 0x800626E0 | size: 0x8
# Used to resolve a target's retail stream by ADDRESS when its symbol name
# is not in the listing (accepted targets often carry legacy `func_`
# placeholder symbols while the listing was regenerated with recovered
# names; both spellings share the same retail bytes at the same address).
_HEADER_RE = re.compile(r"^#\s+\.text:0x[0-9A-Fa-f]+\s+\|\s+0x([0-9A-Fa-f]{8})")

# One instruction line in an objdiff listing:
#   /* 80061F70 0002B1B0  94 21 FF E0 */	stwu r1, -0x20(r1)
# addr(8 hex) + offset(8 hex) + 2..4 byte groups.  The raw word is the
# ground truth for the witness-style bit comparison.
_INSN_RE = re.compile(
    r"/\*\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s+((?:[0-9A-Fa-f]{2}\s+){2,4})\*/"
)


def parse_instructions(body):
    """Extract (mnemonic, operands) tuples from an assembly body (copied
    verbatim from sim_schedule.py; labels/directives skipped)."""
    instrs = []
    for line in body.split("\n"):
        line = line.strip()
        if not line:
            continue
        # Strip the objdiff bytecode comment prefix: /* VA OFFSET BYTES */
        idx = line.find("*/")
        if idx >= 0:
            line = line[idx + 2 :].strip()
            if not line:
                continue
        # Skip labels (.L_xxx:), directives (.4byte) and comment lines (# ...)
        if line.startswith((".", "#")):
            continue
        parts = line.split()
        if parts:
            instrs.append((parts[0], " ".join(parts[1:])))
    return instrs


def split_asm_blocks(asm_text):
    """Split a `.s` listing into ({symbol: [body-text, ...]}, {addr: symbol})
    in one pass.  A symbol may appear in several `.fn` blocks
    (aliases/thunks); consumers pick the longest body.  The address map
    associates each .fn with the preceding `# .text:... | 0xADDR | ...`
    header so targets with legacy `func_` placeholder symbols can still
    resolve their retail stream (and their siblings') by address."""
    blocks = {}
    addr_to_sym = {}
    current = None
    current_addr = None
    body = []
    for raw in asm_text.split("\n"):
        line = raw.strip()
        fn = _FN_RE.match(line)
        if fn:
            if current is not None:
                blocks.setdefault(current, []).append("\n".join(body))
            current = fn.group(1)
            body = []
            # The `# .text:... | 0xADDR | size: ...` header always precedes
            # its .fn; setdefault keeps the first symbol per address.
            if current_addr is not None:
                addr_to_sym.setdefault(current_addr, current)
        elif _ENDFN_RE.match(line):
            if current is not None:
                blocks.setdefault(current, []).append("\n".join(body))
            current = None
            body = []
        elif current is None:
            hdr = _HEADER_RE.match(line)
            if hdr:
                current_addr = hdr.group(1).upper()
        else:
            body.append(line)
    if current is not None:
        blocks.setdefault(current, []).append("\n".join(body))
    return blocks, addr_to_sym


def mnemonic_counts(instrs):
    """Multiset of mnemonics for a function body (copied from
    sim_schedule.py; cached per symbol)."""
    counts = Counter()
    for mnem, _ops in instrs:
        counts[mnem] += 1
    return counts


def overlap_ratio(a, b):
    """Multiset Jaccard (Tanimoto) over mnemonic counts (copied from
    sim_schedule.py): sum(min)/sum(max). 1.0 = identical opcode profile."""
    if not a or not b:
        return 0.0
    inter = union = 0
    for mnem in set(a) | set(b):
        ca = a.get(mnem, 0)
        cb = b.get(mnem, 0)
        inter += min(ca, cb)
        union += max(ca, cb)
    return inter / union if union else 0.0


def parse_size(raw):
    """Registry sizes are hex strings ("0x210"); tolerate decimal ints
    (copied from sim_schedule.py)."""
    if raw is None:
        return None
    s = str(raw)
    try:
        if s.lower().startswith("0x"):
            return int(s, 16)
        return int(s, 10)
    except ValueError:
        return None


# ---------------------------------------------------------------------------
# Byte-accurate instruction extraction (triage-specific).
# ---------------------------------------------------------------------------

def parse_instruction_words(body):
    """Extract [(addr:int, word:int, mnem:str, ops:str)] per instruction
    using the listing's byte comment.  Lines without a 4-byte comment
    (labels, `.4byte` data, directives) are skipped — position alignment is
    preserved as long as both sides skip the same lines; a mismatch
    degrades to `structural` (conservative)."""
    out = []
    for line in body.split("\n"):
        line = line.strip()
        if not line:
            continue
        m = _INSN_RE.match(line)
        if not m:
            continue
        try:
            addr = int(m.group(1), 16)
            byte_groups = m.group(2).split()
            word = int.from_bytes(bytes.fromhex("".join(byte_groups)), "big")
        except ValueError:
            continue
        if word >> 26 > 0x3F:  # not a plausible PPC primary opcode
            continue
        rest = line[m.end():].strip()
        parts = rest.split()
        mnem = parts[0] if parts else ""
        ops = " ".join(parts[1:])
        out.append((addr, word, mnem, ops))
    return out


# ---------------------------------------------------------------------------
# Register-renaming checks — the witness (tools/coop/lib/renaming_witness.py,
# docs/ppc_equiv_work/31-reg-swap-witness.md) is the source of truth.
# hexdiff.py's `_pure_reg_swap` (frozen pre-A2 classifier copy) gave the
# idea; we implement the bit comparison directly against the witness's OWN
# role table (`_gpr_fpr_masks` + `_ra_field_is_register`, doc 32 A2 rev 5)
# so the predictor tracks the certifier exactly, including the
# value-dependent r0-in-RA rule (a literal can never rename with a
# register) and the gate-6 reject list.
# ---------------------------------------------------------------------------

from tools.coop.lib.renaming_witness import (  # noqa: E402  (import order fine)
    REJECT_OPCODES,
    _WITNESS_ALLOWED_SPRS,
    _gpr_fpr_masks,
    _has_indirect_dispatch,
    _ra_field_is_register,
)
from tools.ppc_equivalence.decoder import _decode_word  # noqa: E402
from tools.ppc_equivalence.ir import Opcode  # noqa: E402

_WORD_DECODE_CACHE = {}


def _decode(word: int, addr: int = 0):
    key = (word, addr)
    insn = _WORD_DECODE_CACHE.get(key)
    if insn is None:
        try:
            insn = _decode_word(word, addr, allow_broadway_lmw_overlap=True)
        except Exception:
            return None
        _WORD_DECODE_CACHE[key] = insn
    return insn


def _unmodeled_absolute_branch(words):
    """Local mirror of the witness's ``_has_unmodeled_absolute_branch`` for
    retail-only streams.  The witness skips branches with a relocation (gate
    2 binds offset/type/symbol/addend); ``_decode_word`` never attaches
    relocations, so we use the listing operand text: a non-``.L_`` target
    (function name, ``lbl_*`` symbol, absolute) is a reloc site and is
    skipped.  Only a non-link ``b``/``bc`` to a LOCAL label whose decoded
    target falls outside the function is flagged (the unmodeled absolute
    tail-call form the witness rejects)."""
    addrs = [a for a, _w, _m, _o in words]
    if not addrs:
        return False
    by_index = {a: i for i, a in enumerate(addrs)}
    end_pc = addrs[-1] + 4
    for addr, word, mnem, ops in words:
        insn = _decode(word, addr)
        if insn is None:
            return True
        if insn.opcode not in (Opcode.B, Opcode.BC) or insn.link:
            continue
        target_text = ops.split(",")[0].strip() if ops else ""
        if not target_text.startswith(".L_"):
            continue  # reloc-bearing branch — the witness's gate 2 handles it
        target = insn.operands[0] if insn.opcode == Opcode.B else insn.operands[2]
        if target not in by_index and target != end_pc:
            return True
    return False


def _witness_reject_reason(words):
    """Gate-6 mirror: a stream the witness can NEVER certify (reject-list
    opcodes, indirect dispatch, disallowed SPRs, unmodeled absolute
    tail-calls). Returns a short reason string or None.  Only the CANDIDATE
    stream is checked: if the sibling carries a reject opcode at a slot the
    candidate does not, that slot is a non-register diff -> `structural`
    anyway, and if the candidate carries it too, this check catches it."""
    insns = []
    for addr, word, _mnem, _ops in words:
        insn = _decode(word, addr)
        if insn is None:
            return "decode-error"
        insns.append(insn)
    if _has_indirect_dispatch(insns):
        return "indirect-dispatch"
    if _unmodeled_absolute_branch(words):
        return "unmodeled-absolute-branch"
    for insn in insns:
        op = insn.opcode
        if op in REJECT_OPCODES:
            return f"reject-list:{op.value}"
        if op in (Opcode.MFSPR, Opcode.MTSPR):
            if insn.operands[1] not in _WITNESS_ALLOWED_SPRS:
                return f"spr:{insn.operands[1]}"
    return None


_LABEL_TOKEN = re.compile(r"\.L_[0-9A-Fa-f]+")


def _normalize_ops(ops):
    """Replace branch label names so identical-byte instructions with
    different label TEXTS compare equal.  Two retail functions at different
    addresses name their labels by address (.L_80062804 vs .L_80062794)
    while the raw words (and therefore the relative displacements) are
    identical — the witness compares bytes, so such a slot is byte-identical
    and must not count as a structural difference.  Any other operand drift
    (reloc symbol, immediate) survives normalization."""
    return _LABEL_TOKEN.sub("@L", ops)


def _slot_kind(r_word, d_word, r_mnem, d_mnem, r_ops, d_ops):
    """Classify one position-aligned slot: "same" | "reg_swap" | "structural".

    Mirrors witness gates 2/3 between two RETAIL streams:
    - identical raw word -> same, unless the operand text drifted in a way
      that is NOT just a branch label name (a reloc site whose symbol
      differs shows identical placeholder bytes but a different lbl_*/func
      operand -> structural, because gate 2 binds offset/type/symbol/addend
      and triage cannot see decomp relocs).
    - different words, different mnemonic -> structural.
    - different words, same mnemonic: every bit outside the witness's
      renameable GPR/FPR field mask (plus the value-dependent RA rename
      mask) must be bit-equal.
    """
    if r_word == d_word:
        if r_ops == d_ops or _normalize_ops(r_ops) == _normalize_ops(d_ops):
            return "same"
        return "structural"
    if not r_mnem or not d_mnem or r_mnem != d_mnem:
        return "structural"
    r_insn = _decode(r_word)
    d_insn = _decode(d_word)
    if r_insn is None or d_insn is None:
        return "structural"
    if r_insn.opcode != d_insn.opcode:
        return "structural"
    gpr_mask, fpr_mask = _gpr_fpr_masks(r_insn.opcode)
    register_mask = gpr_mask | fpr_mask
    ra_rename_mask = (0x1F << 16) if _ra_field_is_register(r_insn, d_insn) else 0
    if (r_word ^ d_word) & ~(register_mask | ra_rename_mask):
        return "structural"
    return "reg_swap"


def compare_streams(cand, sib):
    """Position-aligned comparison of two instruction lists (each a list of
    (addr, word, mnem, ops)). Returns (structural_count, reg_swap_count);
    structural_count == -1 signals an instruction-count mismatch."""
    if len(cand) != len(sib):
        return -1, 0
    structural = 0
    reg_swap = 0
    for (_, rw, rm, ro), (_, dw, dm, do) in zip(cand, sib):
        kind = _slot_kind(rw, dw, rm, dm, ro, do)
        if kind == "structural":
            structural += 1
        elif kind == "reg_swap":
            reg_swap += 1
    return structural, reg_swap


def _is_simple_shape(words):
    """Trivially template-shaped: size <= 0x20 and only the strict
    mnemonic set (constant-return / field load-store / getter pattern)."""
    if not words or len(words) * 4 > STRICT_MAX_SIZE:
        return False
    return all(m in STRICT_MNEMONICS for _, _, m, _ in words)


def load_targets():
    with open(TARGETS_JSON) as f:
        data = json.load(f)
    return data.get("targets", []) if isinstance(data, dict) else []


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--unit", required=True, help="objdiff unit, e.g. kyoshin/cf/CfRes")
    ap.add_argument("--limit", type=int, default=None, help="emit at most N rows (default: all)")
    ap.add_argument("--region", default="us", help="retail asm region dir under build/ (default: us)")
    ap.add_argument("--json", action="store_true", help="emit one JSON object per line (JSONL)")
    args = ap.parse_args(argv)

    asm_path = os.path.join(REPO_ROOT, "build", args.region, "asm", args.unit + ".s")
    blocks = {}
    addr_to_sym = {}
    if os.path.exists(asm_path):
        with open(asm_path) as f:
            blocks, addr_to_sym = split_asm_blocks(f.read())
    else:
        sys.stderr.write(f"triage: WARNING no asm listing at {asm_path}\n")

    def normalize_addr(raw):
        """'0x80061F70' -> '80061F70' (listing headers are bare hex)."""
        if not raw:
            return None
        a = str(raw)
        if a.lower().startswith("0x"):
            a = a[2:]
        return a.upper()

    def resolve(sym, address):
        """Return (bodies, listing_symbol).  Falls back to the listing
        symbol at the target's ADDRESS when the target's own symbol is not
        present (legacy `func_` placeholder names on accepted targets)."""
        bodies = blocks.get(sym)
        if bodies:
            return bodies, sym
        addr = normalize_addr(address)
        alt = addr_to_sym.get(addr) if addr else None
        if alt is not None:
            return blocks.get(alt), alt
        return None, sym

    # Per-symbol parsed instruction cache (in-memory, per unit).
    words_cache = {}
    mnem_cache = {}

    def words_for(sym, address):
        key = (sym, normalize_addr(address))
        entry = words_cache.get(key)
        if entry is None:
            bodies, _alt = resolve(sym, address)
            entry = []
            if bodies:
                # Duplicate .fn blocks (aliases/thunks): take the longest —
                # the real body, not a 4-byte trampoline.
                entry = parse_instruction_words(max(bodies, key=len))
            words_cache[key] = entry
        return entry

    def counts_for(sym, address):
        key = (sym, normalize_addr(address))
        counts = mnem_cache.get(key)
        if counts is None:
            bodies, _alt = resolve(sym, address)
            counts = mnemonic_counts(parse_instructions("\n".join(bodies or [])))
            mnem_cache[key] = counts
        return counts

    siblings = []
    candidates = []
    for t in load_targets():
        if t.get("unit") != args.unit:
            continue
        if t.get("kind") is not None and t.get("kind") != "function":
            continue
        status = t.get("status", "NOT_STARTED")
        if status in ACCEPTED:
            siblings.append(t)
        elif status in CANDIDATE:
            candidates.append(t)
        # STALLED / ACTIVE / BLOCKED / ... are workflow-parked — never ranked.

    # Matched siblings must themselves have a parseable retail stream.
    sib_words = {}
    sibling_list = []
    for s in siblings:
        w = words_for(s["symbol"], s.get("address"))
        if w:
            sib_words[s["symbol"]] = w
            sibling_list.append(s)
    if not sibling_list:
        sys.stderr.write(f"triage: {args.unit} — no parseable matched sibling (FULL_MATCH/EQUIVALENT_MATCH); reg-swap prediction unavailable\n")

    rows = []
    for t in candidates:
        cand_words = words_for(t["symbol"], t.get("address"))
        row = {
            "targetId": t["id"],
            "symbol": t["symbol"],
            "size": (len(cand_words) * 4) if cand_words else parse_size(t.get("size")),
        }
        if not cand_words:
            row["cls"] = "unknown"  # no retail asm / unparseable
            rows.append(row)
            continue

        # Rank matched siblings: SAME ADDRESS first (a recovered-name
        # duplicate at the same address is byte-identical to the accepted
        # `func_`-placeholder target by construction), then same instruction
        # count (the witness's gate 1 precondition), then mnemonic overlap,
        # then symbol.
        _bodies, cand_listing_sym = resolve(t["symbol"], t.get("address"))
        cand_counts = mnemonic_counts(parse_instructions("\n".join(blocks.get(cand_listing_sym, []))))
        cand_len = len(cand_words)
        cand_addr = normalize_addr(t.get("address"))
        scored = []
        for s in sibling_list:
            sib_w = sib_words[s["symbol"]]
            scored.append(
                (
                    normalize_addr(s.get("address")) == cand_addr,
                    len(sib_w) == cand_len,
                    overlap_ratio(cand_counts, counts_for(s["symbol"], s.get("address"))),
                    s["symbol"],
                    s,
                )
            )
        scored.sort(key=lambda item: (-item[0], -item[1], -item[2], item[3]))
        if not scored:
            row["cls"] = "strict" if _is_simple_shape(cand_words) else "unknown"
            rows.append(row)
            continue

        # Gate-6 mirror: streams the witness can never certify.
        reject = _witness_reject_reason(cand_words)
        if reject is not None:
            best = scored[0]
            sib_w = sib_words[best[3]]
            st, _rs = compare_streams(cand_words, sib_w)
            row["nearestMatched"] = {"symbol": best[3], "score": best[2]}
            row["structuralCount"] = st
            row["cls"] = "structural"
            row["reject"] = reject
            rows.append(row)
            continue

        # Try up to MAX_SIBLING_ATTEMPTS same-count siblings as candidate
        # templates. A register-only match predicts the witness certifies
        # (cheap to be wrong: the witness run itself is the gate).
        for item in scored[:MAX_SIBLING_ATTEMPTS]:
            if item[1] == 0:
                continue  # count mismatch -> can never be witness-certified
            sib_w = sib_words[item[3]]
            st, rs = compare_streams(cand_words, sib_w)
            if st == 0 and rs > 0:
                row["cls"] = "regswap_only"
                row["nearestMatched"] = {"symbol": item[3], "score": item[2]}
                row["structuralCount"] = 0
                break
            if st == 0 and rs == 0:
                # Byte-identical twin (spec: byte-identical -> strict).
                row["cls"] = "strict"
                row["nearestMatched"] = {"symbol": item[3], "score": item[2]}
                row["structuralCount"] = 0
                break
        else:
            # No same-count register-only template: report the closest
            # sibling (same count preferred, else best overlap) and classify.
            best = scored[0]
            sib_w = sib_words[best[3]]
            st, rs = compare_streams(cand_words, sib_w)
            row["nearestMatched"] = {"symbol": best[3], "score": best[2]}
            row["structuralCount"] = st
            if st == 0 and rs > 0:
                row["cls"] = "regswap_only"  # best sibling matched count after all
            elif st == 0 and rs == 0:
                row["cls"] = "strict"
            elif _is_simple_shape(cand_words):
                row["cls"] = "strict"  # shape is intrinsic — wins over structural
            else:
                row["cls"] = "structural"
        rows.append(row)

    rows.sort(key=lambda r: (r["targetId"],))
    if args.limit is not None:
        rows = rows[: args.limit]

    if args.json:
        for r in rows:
            print(json.dumps(r))
        return 0

    dist = Counter(r["cls"] for r in rows)
    print(f"triage: {args.unit} — {len(rows)} candidate(s), "
          f"{len(sibling_list)} matched sibling(s); "
          f"distribution {dict(sorted(dist.items()))}")
    print(f"{'targetId':<16} {'symbol':<34} {'size':>6}  {'cls':<11}  sibling / structural")
    for r in rows:
        nm = r.get("nearestMatched")
        nm_str = f"{nm['symbol']} ({nm['score']:.2f})" if nm else "-"
        st = r.get("structuralCount")
        st_str = f"{st}" if st is not None else "-"
        print(f"{r['targetId']:<16} {r['symbol']:<34} {str(r['size']):>6}  "
              f"{r['cls']:<11}  {nm_str} / {st_str}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
