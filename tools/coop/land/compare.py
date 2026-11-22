"""Pure before/after snapshot comparison (the land gate's verdict)."""
from __future__ import annotations

from dataclasses import dataclass, field

PASS_MARK = "✓"


@dataclass(frozen=True)
class Violation:
    kind: str      # lost | data | extab | hard | missing | error
    unit: str
    detail: str


@dataclass
class Allowance:
    """An explicit, reasoned exception: ``unit kind:key +n -- reason``."""
    unit: str
    kind: str
    key: str
    amount: int
    reason: str

    @classmethod
    def parse(cls, text: str) -> "Allowance":
        head, _, reason = text.partition("--")
        if not reason.strip():
            raise ValueError(f"allowance needs a '-- reason': {text!r}")
        unit, spec, amt = head.split()
        kind, _, key = spec.partition(":")
        return cls(unit.removeprefix("main/"), kind, key, int(amt.lstrip("+")), reason.strip())


@dataclass
class Verdict:
    violations: list[Violation] = field(default_factory=list)
    gains: list[str] = field(default_factory=list)
    used_allowances: list[Allowance] = field(default_factory=list)

    @property
    def ok(self) -> bool:
        return not self.violations


def _norm(u: str) -> str:
    return u.removeprefix("main/")


def compare(before: dict, after: dict, renames: dict[str, str] | None = None,
            allowances: list[Allowance] | None = None) -> Verdict:
    """Reject any regression anywhere in the tree.

    * every function ✓ before must be ✓ after (keys mapped through *renames*)
    * per section, the whole-tree sum of identical bytes must not fall
      (bytes moving between units, e.g. a split move, are conserved)
    * a unit whose extab passed must still pass
    * no unit / unpaired header may gain a hard faithful category or ban
    * a unit present and measurable before must still be measurable after
    * the final link gains no missing / multiply-defined symbol
    """
    renames = renames or {}
    allow = {(a.unit, a.kind, a.key): a for a in (allowances or [])}
    v = Verdict()
    bu = {_norm(k): x for k, x in before.get("units", {}).items()}
    au = {_norm(k): x for k, x in after.get("units", {}).items()}
    berr = {_norm(k) for k in before.get("errors", {})}
    aerr = {_norm(k): e for k, e in after.get("errors", {}).items()}

    if before.get("build_ok", True) and not after.get("build_ok", True):
        v.violations.append(Violation("error", "*", "full build failed"))

    # Final link (link_gate result): no new missing / multiply-defined name,
    # no rise in any link error count.  Skipped when either side lacks it.
    if before.get("link") is not None and after.get("link") is not None:
        from tools.coop import link_gate
        regress, improve = link_gate.compare(before["link"], after["link"])
        v.violations += [Violation("link", "*", r) for r in regress]
        v.gains += [f"link {i}" for i in improve]

    # Split corrections: ``SRC merge:DST 0 -- reason`` folds unit SRC into DST
    # (evidence-backed TU merge).  SRC's functions are then checked in DST and
    # DST's hard baseline is the sum of both units.
    merge_al = [a for a in (allowances or []) if a.kind == "merge"]
    merges = {a.unit: _norm(a.key) for a in merge_al}
    v.used_allowances += [a for a in merge_al if a.unit in bu and a.unit not in au and merges[a.unit] in au]

    for u, b in bu.items():
        a = au.get(u)
        if a is None and u in merges and merges[u] in au:
            a = {"fns": au[merges[u]].get("fns", {}), "extab": None}
        if a is None:
            v.violations.append(Violation("missing", u, "unit no longer measured"))
            continue
        if u in aerr and u not in berr:
            v.violations.append(Violation("error", u, aerr[u][:200]))
        afns = a.get("fns", {})
        for sym, mark in b.get("fns", {}).items():
            new = renames.get(sym, sym)
            am = afns.get(new)
            if mark == PASS_MARK and am != PASS_MARK:
                v.violations.append(Violation("lost", u, f"{sym}{'' if new == sym else ' -> ' + new}: ✓ -> {am or 'absent'}"))
            elif mark != PASS_MARK and am == PASS_MARK:
                v.gains.append(f"{u}: {new} -> ✓")
        if b.get("extab") is True and a.get("extab") is False:
            v.violations.append(Violation("extab", u, "extab/extabindex now mismatch"))

    # whole-tree data conservation per section
    secs: set[str] = set()
    for x in list(bu.values()) + list(au.values()):
        secs |= set(x.get("data", {}))
    for s in sorted(secs):
        tb = sum(x.get("data", {}).get(s, 0) for x in bu.values())
        ta = sum(x.get("data", {}).get(s, 0) for x in au.values())
        if ta < tb:
            drops = sorted(
                ((u, bu[u].get("data", {}).get(s, 0) - au.get(u, {}).get("data", {}).get(s, 0)) for u in bu),
                key=lambda t: -t[1])
            who = ", ".join(f"{u} -{d}" for u, d in drops if d > 0)[:300]
            v.violations.append(Violation("data", "*", f"{s}: {tb} -> {ta} identical bytes ({who})"))
        elif ta > tb:
            v.gains.append(f"data {s}: +{ta - tb} identical bytes")

    # hard faithful categories + bans: per-unit and per-unpaired-header ratchet
    # ban counters the *baseline* measured (older snapshots predate some)
    known_bans = set(before.get("ban_keys") or [
        k[4:] for grp in ("units", "headers") for u in before.get(grp, {}).values()
        for k in u.get("hard", {}) if k.startswith("ban:")])

    def ratchet(bmap: dict, amap: dict):
        for name, a in amap.items():
            b = bmap.get(name, {})
            for cat, n in a.get("hard", {}).items():
                if cat.startswith("ban:") and cat[4:] not in known_bans:
                    continue  # counter the baseline never measured
                rise = n - b.get("hard", {}).get(cat, 0)
                if rise <= 0:
                    continue
                al = allow.get((name, "hard", cat))
                if al and rise <= al.amount:
                    v.used_allowances.append(al)
                    continue
                v.violations.append(Violation("hard", name, f"{cat} +{rise}"))
    hb = dict(bu)
    for src, dst in merges.items():
        if src in bu and dst in bu:
            tot = dict(bu[dst].get("hard", {}))
            for cat, n in bu[src].get("hard", {}).items():
                tot[cat] = tot.get(cat, 0) + n
            hb[dst] = {**bu[dst], "hard": tot}
    ratchet(hb, au)
    ratchet({_norm(k): x for k, x in before.get("headers", {}).items()},
            {_norm(k): x for k, x in after.get("headers", {}).items()})
    return v


def format_verdict(v: Verdict) -> str:
    lines = [f"LAND GATE: {'PASS' if v.ok else 'REJECT'}"]
    for x in v.violations:
        lines.append(f"  [{x.kind}] {x.unit}: {x.detail}")
    for a in v.used_allowances:
        lines.append(f"  [allowed] {a.unit} {a.kind}:{a.key} +{a.amount} -- {a.reason}")
    if v.gains:
        lines.append(f"  gains: {len(v.gains)}")
        lines += [f"    {g}" for g in v.gains[:40]]
    return "\n".join(lines)
