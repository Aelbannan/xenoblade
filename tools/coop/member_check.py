#!/usr/bin/env python3
"""ABI-level member-vs-free classification from retail asm.

Implements the tiered audit rules verified by independent agents
(kimi-k3 / glm-5.2, 2026-08-08) on the CfGameManager mis-annotation set:

  N1  any call site passes a NON-ZERO, NON-ADDRESS constant in r3
      (li/lis immediate)                -> NOT a non-static member  [sound, decisive]
  N2  callee never dereferences r3 and never forwards it in r3
      position (only integer ops / r4+ args) -> NOT a non-static member
  N3  callee's max r3-relative access offset >= class size (anchor)
                                        -> NOT a member of that class  [sound]
  P1  (hint) symbol address found as a 4-byte pointer in retail .data
                                        -> candidate vtable / function-pointer slot
  Tier B  zero instance-anchored call sites + stack-heavy provenance
                                        -> flag for review (heuristic)
  Tier C  callee derefs r3 in-bounds + ambiguous provenance
                                        -> UNDECIDABLE — never auto-claim

Deliberately NOT used as evidence (refuted in-repo):
  - stack address in r3 (a member can be called on a stack-allocated object)
  - this-source clustering (a member can pass `this` to free helpers)
"""
from __future__ import annotations

import re
import struct
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
ASM_DIR = ROOT / "build" / "us" / "asm"
OBJ_DIR = ROOT / "build" / "us" / "obj"

INS = re.compile(
    r"/\* [0-9A-F]+ [0-9A-F]+  ([0-9A-F]{2}(?: [0-9A-F]{2}){3}) \*/\s*(.+)"
)
MNEM = re.compile(r"^([a-z0-9_.]+)(?:\s+(.+))?$")

# mnemonics that READ r3 but do not write it
READERS = ("cmp", "b", "bl", "blr", "bctrl", "st", "mtctr", "mtlr", "mfspr", "mftb", "bc", "bcctr")
# mnemonics that treat r3 as a value (integer arithmetic), not a base pointer
INTEGER_OPS = ("rlwinm", "slwi", "srwi", "srawi", "rlwimi", "or", "orc", "and", "andc",
               "xor", "add", "sub", "subf", "mulli", "mullw", "divw", "neg", "extsb",
               "extsh", "cntlzw", "clrlwi", "clrrwi", "rotlwi")

DEREF_LOAD = re.compile(r"(?:lwz|lbz|lhz|lha|lfs|lfd)\s+r\d+,\s*([0-9A-Fa-fxX]+)\(r3\)")
DEREF_STORE = re.compile(r"(?:stw|stb|sth|stfs|stfd)\s+r\d+,\s*([0-9A-Fa-fxX]+)\(r3\)")


def _parse_file(path: Path) -> list[tuple[str, str]]:
    """All instructions in a dtk asm file, in order."""
    out = []
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        m = INS.match(line)
        if not m:
            continue
        b = m.group(2).strip()
        b = re.sub(r"\s+//.*$", "", b).split("  ")[0].strip()
        m2 = MNEM.match(b)
        if m2:
            out.append((m2.group(1), m2.group(2) or ""))
    return out


class AsmIndex:
    """Parsed retail asm: instruction streams per file + function ranges + calls."""

    def __init__(self, asm_dir: Path = ASM_DIR):
        self.files: dict[str, list[tuple[str, str]]] = {}
        self.fn_range: dict[str, tuple[str, int, int]] = {}
        self.calls: dict[str, list[tuple[str, str, int]]] = {}  # target -> (file, caller, idx)
        if not asm_dir.is_dir():
            return
        for path in sorted(asm_dir.rglob("*.s")):
            rel = str(path.relative_to(asm_dir))
            insns = _parse_file(path)
            if not insns:
                continue
            self.files[rel] = insns
            self._index_functions(path, rel, insns)

    def _index_functions(self, path: Path, rel: str, insns: list[tuple[str, str]]) -> None:
        cur = None
        start = 0
        idx = 0
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            s = line.strip()
            if s.startswith(".fn "):
                cur = s[4:].split(",")[0].strip()
                start = idx
            elif s.startswith(".endfn") and cur:
                self.fn_range[cur] = (rel, start, idx)
                cur = None
            else:
                if INS.match(line):
                    idx += 1
        if cur:
            self.fn_range[cur] = (rel, start, idx)
        # index call sites
        for i, (mnem, ops) in enumerate(insns):
            if mnem == "bl" and ops.strip() and not ops.strip().startswith((".", "@")):
                target = ops.strip().split(",")[0].strip()
                caller = None
                for fn, (frel, fs, fe) in self.fn_range.items():
                    if frel == rel and fs <= i < fe:
                        caller = fn
                        break
                self.calls.setdefault(target, []).append((rel, caller or "?", i))


def r3_provenance(insns: list[tuple[str, str]], i: int) -> tuple[str, str]:
    """Classify the last r3 WRITE before instruction i. mr chains chased one level.
    A bl (call) clobbers r3, so the scan stops there."""
    for j in range(i - 1, max(-1, i - 9), -1):
        mnem, ops = insns[j]
        if mnem == "bl":
            return "CALL_RESULT", ops  # r3 = previous call's result (or stale garbage)
        if not re.search(r"\br3\b", ops):
            continue
        if mnem.startswith(READERS):
            continue
        op = [x.strip().rstrip(",") for x in ops.replace("  ", " ").split(" ") if x.strip()]
        if mnem in ("li", "lis"):
            imm = op[1] if len(op) > 1 else "?"
            return "CONSTANT", f"{mnem} {ops}"
        if mnem == "mr" and op and op[0] == "r3":
            src = op[1] if len(op) > 1 else "?"
            for k in range(j - 1, max(-1, j - 9), -1):
                m2, o2 = insns[k]
                if not re.search(r"\b" + src + r"\b", o2):
                    continue
                if m2.startswith(READERS):
                    continue
                o = [x.strip().rstrip(",") for x in o2.replace("  ", " ").split(" ") if x.strip()]
                if m2 in ("li", "lis"):
                    return "CONSTANT", f"{m2} {o2}"
                if m2 in ("lwz", "lbz", "lhz", "lfs", "lfd") and len(o) >= 2:
                    return "LOAD_FIELD", o2
                if m2 == "addi" and len(o) >= 3:
                    if "@l" in o2 or "@ha" in o2:
                        return "ADDR_LABEL", o2
                    if o[1].rstrip(",") == "r1":
                        return "ADDR_STACK", o2
                    return "ADDI", o2
                if m2 == "bl":
                    return "CALL_RESULT", o2
                if m2 == "mr":
                    return "COPY", o2
                return f"{m2.upper()}", o2
            return "COPY", ops
        if mnem in ("lwz", "lbz", "lhz", "lfs", "lfd") and len(op) >= 3 and op[0] == "r3":
            return "LOAD_FIELD", ops
        if mnem == "addi" and op and op[0] == "r3":
            if "@l" in ops or "@ha" in ops:
                return "ADDR_LABEL", ops
            if len(op) >= 3 and op[1].rstrip(",") == "r1":
                return "ADDR_STACK", ops
            return "ADDI", ops
        if mnem == "bl":
            return "CALL_RESULT", ops
        return f"{mnem.upper()}", ops
    return "UNKNOWN", ""


def callee_r3_usage(body: list[tuple[str, str]], class_size: int = 0) -> dict:
    """Analyze how the callee uses r3."""
    deref = False
    deref_max_off = 0
    stores = False
    vtable_dispatch = False
    integer_only = False
    r3_ops = 0
    derefs = 0
    forwarded_r3 = False
    r12_from_r3 = False
    has_bctrl = False
    for mnem, ops in body:
        full = f"{mnem} {ops}"
        if mnem == "lwz" and re.search(r"r12\b", ops) and re.search(r"\(r3\)", ops):
            r12_from_r3 = True  # method pointer loaded via this's vtable
        if mnem == "bctrl":
            has_bctrl = True
        if "r3" in full:
            r3_ops += 1
            m = DEREF_LOAD.search(full) or DEREF_STORE.search(full)
            if m:
                derefs += 1
                deref = True
                try:
                    deref_max_off = max(deref_max_off, int(m.group(1), 16))
                except ValueError:
                    pass
                if mnem in ("stw", "stb", "sth", "stfs", "stfd"):
                    stores = True
                continue
            if mnem in INTEGER_OPS:
                integer_only = True
            if mnem in ("mr", "bl", "lwz", "stw") and re.search(r"r3\b", ops):
                forwarded_r3 = True
    vtable_dispatch = r12_from_r3 and has_bctrl
    # integer_only: r3 used but never deref'd, and never forwarded as object
    return {
        "deref": deref,
        "deref_max_offset": deref_max_off,
        "stores": stores,
        "vtable_dispatch": vtable_dispatch,
        "integer_only": derefs == 0 and r3_ops > 0 and (integer_only or not forwarded_r3),
        "r3_ops": r3_ops,
        "derefs": derefs,
    }


def build_data_hits(addresses: set[int]) -> dict[int, list[tuple[str, int]]]:
    """One-pass scan: for each given address, where does it appear as a 4-byte
    big-endian pointer in retail .o files (candidate vtable / func-table slots)?"""
    packed = {a: struct.pack(">I", a & 0xFFFFFFFF) for a in addresses}
    hits: dict[int, list[tuple[str, int]]] = {}
    if not OBJ_DIR.is_dir():
        return hits
    for f in OBJ_DIR.rglob("*.o"):
        data = f.read_bytes()
        rel = str(f.relative_to(OBJ_DIR))
        for a, p in packed.items():
            n = data.count(p)
            if n:
                hits.setdefault(a, []).append((rel, n))
    return hits


def data_pointer_hits(symbol_addr: int) -> list[tuple[str, int]]:
    """4-byte big-endian pointers to the address in retail .o files (candidate vtable/func-table slots)."""
    hits = []
    if not OBJ_DIR.is_dir():
        return hits
    packed = struct.pack(">I", symbol_addr & 0xFFFFFFFF)
    for f in OBJ_DIR.rglob("*.o"):
        data = f.read_bytes()
        n = data.count(packed)
        if n:
            hits.append((str(f.relative_to(OBJ_DIR)), n))
    return hits


def parse_qualifier_class(mangled: str) -> str | None:
    """Extract the last class token from a Q-prefixed mangled qualifier, e.g.
    func_8007DCA8__Q22cf13CfGameManagerFv -> CfGameManager."""
    m = re.search(r"__Q(?:\d+[A-Za-z_]+)+\w*(?=F)", mangled)
    if not m:
        return None
    q = m.group(0)[3:]
    # q is like "22cf13CfGameManager" -> tokens: 2+cf, 13+CfGameManager
    tokens = []
    i = 0
    while i < len(q):
        dm = re.match(r"(\d+)([A-Za-z_]+)", q[i:])
        if not dm:
            break
        tokens.append(dm.group(2))
        i += len(dm.group(0))
    return tokens[-1] if tokens else None


def first_mention_is_read(body: list[tuple[str, str]], reg: str) -> bool | None:
    """Is the register's first use in the function a READ (i.e. a consumed param)?
    Returns None if the register never appears."""
    READ_ONLY = ("stw", "stb", "sth", "stfs", "stfd", "stwx", "cmpwi", "cmpw",
                 "cmplwi", "cmplw", "cmp", "mtctr", "mtlr", "bctrl", "bl", "bc")
    for mnem, ops in body:
        if not re.search(rf"\b{reg}\b", ops):
            continue
        if mnem in READ_ONLY:
            return True  # store/cmp/ctrl: register is read
        op = [x.strip().rstrip(",") for x in ops.replace("  ", " ").split(" ") if x.strip()]
        if op and op[0] == reg:
            # write to reg — but read-modify-write (reg also a source, e.g.
            # `slwi r3, r3, N`, `add r3, r3, r0`) still consumes the input value
            if len(op) > 1 and any(o == reg for o in op[1:]):
                return True
            return False
        return True
    return None


def callee_params(body: list[tuple[str, str]]) -> dict:
    """Binary param evidence: which GPR/FPR arguments the callee consumes.
    Returns {'gprs': [(reg, class)], 'fprs': [reg...]} for consumed registers.
    Class: 'ptr' (deref base), 'int' (arithmetic/cmp), 'bool' (only 0/1 cmp),
    'opaque' (only moved/passed on)."""
    if not body:
        return {"gprs": [], "fprs": []}
    gprs, fprs = [], []
    for n in range(3, 11):
        reg = f"r{n}"
        if first_mention_is_read(body, reg) is True:
            is_ptr = any(re.search(rf"\(({reg})\)", f"{m} {o}") for m, o in body)
            is_int = any(
                m in INTEGER_OPS or m.startswith("cmp")
                and re.search(rf"\b{reg}\b", o)
                for m, o in body
            )
            # bool: compared only against 0/1
            cmps = [
                o for m, o in body
                if m.startswith("cmp") and re.search(rf"\b{reg}\b", o)
            ]
            if is_ptr:
                cls = "ptr"
            elif cmps and all(re.search(r"0x[01]$", o) for o in cmps):
                cls = "bool"
            elif is_int:
                cls = "int"
            else:
                cls = "opaque"
            gprs.append((reg, cls))
    for n in range(1, 5):
        reg = f"f{n}"
        if first_mention_is_read(body, reg) is True:
            fprs.append(reg)
    return {"gprs": gprs, "fprs": fprs}


def classify_symbol(symbol: str, idx: AsmIndex, class_size: int = 0,
                    symbol_addr: int | None = None,
                    data_hits: dict[int, list[tuple[str, int]]] | None = None) -> dict:
    """Tiered verdict for one retail symbol."""
    calls = idx.calls.get(symbol, [])
    prov = [r3_provenance(idx.files[rel], ci) for rel, caller, ci in calls]
    kinds = Counter(k for k, _ in prov)

    body = None
    fr = idx.fn_range.get(symbol)
    if fr:
        rel, fs, fe = fr
        body = idx.files[rel][fs:fe]
    usage = callee_r3_usage(body) if body else {}
    params = callee_params(body) if body else {"gprs": [], "fprs": []}

    # ---- verdict rules (verified tiered classifier) ----
    # N1: constant in r3 (NON-ZERO, non-address) at any call site — zero is
    # ambiguous (null-this UB vs null pointer arg) and excluded per audit rules
    def _const_imm(detail: str) -> int | None:
        m = re.search(r"(?:li|lis)\s+r3,\s*(0x[0-9A-Fa-f]+|\d+)", detail)
        if not m:
            return None
        return int(m.group(1), 16) if m.group(1).startswith("0x") else int(m.group(1))

    n1 = any(k == "CONSTANT" and _const_imm(d) not in (None, 0) for k, d in prov)
    # N2: callee never derefs r3 (and uses it at all)
    n2 = bool(body is not None and not usage.get("deref") and usage.get("r3_ops", 0) > 0)
    # N3: max r3-relative offset >= class size
    n3 = bool(class_size and usage.get("deref_max_offset", 0) >= class_size)
    # integer-only: r3 used solely as an integer (never deref'd, arithmetic ops)
    int_only = bool(usage.get("integer_only"))

    stack_heavy = kinds.get("ADDR_STACK", 0) == len(calls) and len(calls) > 0
    instance_anchored = kinds.get("ADDR_LABEL", 0) + kinds.get("LOAD_FIELD", 0)
    vtable_hints = []
    if symbol_addr:
        if data_hits is not None:
            vtable_hints = data_hits.get(symbol_addr, [])
        else:
            vtable_hints = data_pointer_hits(symbol_addr)

    if n1:
        verdict = "NOT non-static member (N1: constant r3 at call site)"
    elif int_only:
        verdict = "NOT non-static member (r3 used as integer, never deref'd)"
    elif n2:
        verdict = "NOT non-static member (N2: r3 never deref'd/forwarded)"
    elif n3:
        verdict = f"NOT member of class (N3: r3 offset {usage.get('deref_max_offset'):#x} >= size {class_size:#x})"
    elif usage.get("vtable_dispatch"):
        verdict = "possible virtual member (vtable dispatch via r3; confirm vtable membership)"
    elif usage.get("deref"):
        if stack_heavy:
            verdict = "Tier B: flag for review (derefs r3 but all-stack provenance)"
        elif instance_anchored > 0:
            verdict = "possible member (instance-anchored r3 calls; not exclusive)"
        else:
            verdict = "Tier B: flag for review (derefs r3; no instance-anchored call sites)"
    else:
        verdict = "NOT non-static member (r3 never used as object base)"

    return {
        "symbol": symbol,
        "call_sites": len(calls),
        "r3_provenance": dict(kinds),
        "callee": usage,
        "binary_params": params,
        "class_size": class_size,
        "verdict": verdict,
        "vtable_hints": vtable_hints,
        "body_present": body is not None,
    }


def symbol_address(symbol: str, region: str = "us") -> int | None:
    """Look up a symbol's address in config/<region>/symbols.txt."""
    path = ROOT / "config" / region / "symbols.txt"
    if not path.is_file():
        return None
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        m = re.match(rf"^{re.escape(symbol)}\s*=\s*\S+:0x([0-9A-Fa-f]{{8}})", line)
        if m:
            return int(m.group(1), 16)
    return None


def find_class_tu(class_name: str) -> Path | None:
    """Locate the class's source TU (e.g. CfGameManager -> src/kyoshin/cf/CfGameManager.cpp)."""
    for base in (ROOT / "src", ROOT / "libs"):
        hits = sorted(base.rglob(f"{class_name}.cpp"))
        if hits:
            return hits[0]
    # fall back: file defining ClassName:: members
    for base in (ROOT / "src", ROOT / "libs"):
        for f in base.rglob("*.cpp"):
            try:
                if re.search(rf"{class_name}::\w+\s*\(", f.read_text(encoding="utf-8", errors="replace")[:40000]):
                    return f
            except OSError:
                continue
    return None


TRICK_RE = re.compile(
    r"this\s*(?:==|!=)\s*(?:nullptr|NULL|0)"
    r"|(?:r3|r4|r5|r6)\s*==\s*0"
)


def fake_members(class_name: str) -> list[tuple[str, int, str]]:
    """Source-side scan: member definitions using the 'this == nullptr' register-read
    trick (a recovered-as-member function that actually takes args in r3..)."""
    tu = find_class_tu(class_name)
    if tu is None:
        return []
    hits = []
    for i, line in enumerate(tu.read_text(encoding="utf-8", errors="replace").splitlines(), 1):
        if TRICK_RE.search(line) and re.search(rf"{class_name}::\w+\s*\(", line) is None:
            # trick inside a member body — find the enclosing member def line
            hits.append((str(tu.relative_to(ROOT)), i, line.strip()))
    return hits


def header_drift(class_name: str, idx: AsmIndex) -> list[dict]:
    """Header declarations contradicted by binary evidence (header-as-error-surface).
    For each header member decl, classify the retail symbol; report where the binary
    disagrees with the header's declared form (static-ness / param count)."""
    hdr = None
    for base in (ROOT / "include", ROOT / "src", ROOT / "libs"):
        hits = sorted(base.rglob(f"{class_name}.hpp"))
        if hits:
            hdr = hits[0]
            break
    if hdr is None:
        return []
    out = []
    for i, line in enumerate(hdr.read_text(encoding="utf-8", errors="replace").splitlines(), 1):
        m = re.search(r"(static\s+)?[\w:<>*&]+\s+(func_[0-9A-Fa-f]{8})\s*\(", line)
        if not m:
            continue
        is_static = bool(m.group(1))
        base = m.group(2)
        p_open = line.find("(")
        p_close = line.rfind(")")
        params_hdr = line[p_open + 1:p_close] if p_open >= 0 and p_close > p_open else ""
        nparams_hdr = 0 if params_hdr.strip() == "" else params_hdr.count(",") + 1
        # find the retail symbol by name prefix
        retail = None
        sym_path = ROOT / "config" / "us" / "symbols.txt"
        for sl in sym_path.read_text(encoding="utf-8", errors="replace").splitlines():
            if sl.startswith(base):
                retail = sl.split("=")[0].strip()
                break
        if retail is None:
            continue
        r = classify_symbol(retail, idx)
        bp = r.get("binary_params", {})
        nparams_bin = len(bp.get("gprs", [])) + len(bp.get("fprs", []))
        v = r["verdict"]
        confident_not_member = v.startswith("NOT non-static member")
        confident_member = "possible member" in v or "possible virtual" in v
        drift = []
        if is_static and confident_member:
            drift.append("header declares static but binary shows this-taking")
        if not is_static and confident_not_member:
            drift.append("header declares member but binary proves no-this")
        if nparams_bin and nparams_bin != nparams_hdr:
            drift.append(f"header {nparams_hdr} params, binary {nparams_bin}")
        if drift:
            out.append({
                "header": str(hdr.relative_to(ROOT)),
                "line": i,
                "decl": line.strip(),
                "retail": retail,
                "binary_params": bp,
                "verdict": v,
                "drift": drift,
            })
    return out
