"""Certified-calls-v2 refinement obligations (Wave 2).

Each summarized callee must eventually discharge
``ordinary semantics ≡ summary semantics``. Wave 2 lands the obligation schema,
strict validation, Tier-A rejection gates, and a trivial-leaf SMT discharge
helper. Opaque EABI and general callees stay incomplete / non-promotion-grade.
"""

from __future__ import annotations

import hashlib
from dataclasses import dataclass
from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    STATUS_SCOPED_ASSUMPTION,
    CapabilityAssurance,
    CapabilityAttestation,
    build_attestation,
)
from tools.ppc_equivalence.provenance import canonical_json_sha256
from tools.ppc_equivalence.vtable_obligations import detect_callee_cycle

CERTIFIED_CALLS_CAPABILITY = "certified-calls"
CERTIFIED_CALLS_MODEL_VERSION = "certified-calls-v2"
CERTIFIED_CALLS_ALGORITHM = "certified-calls-refinement-v1"
TRIVIAL_LEAF_REFINEMENT_ALGORITHM = "certified-calls-trivial-leaf-v1"

_SHA256_LEN = 64
_HEX64 = frozenset("0123456789abcdef")

KNOWN_OBLIGATION_KEYS = frozenset(
    {"capability", "model_version", "callees", "closure_sha256"}
)
KNOWN_CALLEE_ENTRY_KEYS = frozenset(
    {
        "target_id",
        "certificate_sha256",
        "body_sha256",
        "summary_sha256",
        "refinement",
    }
)
KNOWN_REFINEMENT_KEYS = frozenset({"result", "query_sha256"})
KNOWN_REFINEMENT_RESULTS = frozenset(
    {"unsat", "sat", "unknown", "incomplete", "timeout"}
)

# Rejection codes that block promotion-grade (Tier A).
REJECTION_OPAQUE_EABI = "opaque-eabi-summary"
REJECTION_AUTO_ASSUMED_RELOCATED = "auto-assumed-relocated-callee"
REJECTION_UNRESOLVED_DIRECT = "unresolved-direct-call"
REJECTION_INDIRECT_NO_CLOSURE = "indirect-call-without-closed-target-set"
REJECTION_STALE_CHILD = "stale-child-certificate"
REJECTION_MISSING_DEFINEDNESS = "missing-invalid-reasons"
REJECTION_CIRCULAR_SCC = "circular-scc-without-induction"
REJECTION_SYMBOL_TOKEN = "wrong-callee-symbol-sharing-token"
REJECTION_SUMMARY_MISMATCH = "summary-sha256-mismatch"
REJECTION_BODY_MISMATCH = "body-sha256-mismatch"
REJECTION_ENGINE_BOUND = "certificate-not-engine-bound"
REJECTION_REFINEMENT_INCOMPLETE = "refinement-not-unsat"

OPAQUE_SOURCES = frozenset(
    {
        "opaque-eabi",
        "nested-call-opaque-eabi",
    }
)


def _is_sha256(value: Any) -> bool:
    return (
        isinstance(value, str)
        and len(value) == _SHA256_LEN
        and all(ch in _HEX64 for ch in value)
    )


def compute_summary_sha256(
    *,
    target_id: str,
    symbol: str,
    summary: Mapping[str, Any],
) -> str:
    """Digest binding callee identity to its effect summary (sharing-token gate)."""
    reads = summary.get("reads", [])
    writes = summary.get("writes", [])
    invalid = summary.get("invalid_reasons", [])
    payload = {
        "target_id": str(target_id),
        "symbol": str(symbol),
        "summary": {
            "reads": sorted(str(item) for item in reads),
            "writes": sorted(str(item) for item in writes),
            "invalid_reasons": sorted(int(item) for item in invalid),
            "return_behavior": str(summary.get("return_behavior", "normal")),
        },
    }
    return canonical_json_sha256(payload)


def compute_body_sha256(
    *,
    retail_sha256: str,
    candidate_sha256: str,
) -> str:
    """Pair retail/candidate body digests into a single body identity."""
    return canonical_json_sha256(
        {
            "retail_sha256": str(retail_sha256),
            "candidate_sha256": str(candidate_sha256),
        }
    )


def compute_closure_sha256(
    callees: Sequence[Mapping[str, Any]],
) -> str:
    """Transitive closure digest over ordered callee certificate identities."""
    entries = []
    for item in callees:
        entries.append(
            {
                "target_id": str(item.get("target_id", "")),
                "certificate_sha256": str(item.get("certificate_sha256", "")),
                "body_sha256": str(item.get("body_sha256", "")),
                "summary_sha256": str(item.get("summary_sha256", "")),
            }
        )
    entries.sort(key=lambda row: row["target_id"])
    return canonical_json_sha256({"callees": entries})


def validate_certified_calls_obligation(obligation: Mapping[str, Any] | None) -> str | None:
    """Strict schema check; unknown fields/algorithms fail closed. None = ok."""
    if obligation is None:
        return "certified-calls obligation is missing"
    if not isinstance(obligation, Mapping):
        return "certified-calls obligation must be an object"

    unknown = sorted(set(obligation.keys()) - KNOWN_OBLIGATION_KEYS)
    if unknown:
        return f"certified-calls unknown fields: {', '.join(unknown)}"
    missing = sorted(KNOWN_OBLIGATION_KEYS - set(obligation.keys()))
    if missing:
        return f"certified-calls missing fields: {', '.join(missing)}"

    if obligation.get("capability") != CERTIFIED_CALLS_CAPABILITY:
        return (
            f"certified-calls.capability must be {CERTIFIED_CALLS_CAPABILITY!r}"
        )
    if obligation.get("model_version") != CERTIFIED_CALLS_MODEL_VERSION:
        return (
            f"certified-calls.model_version must be "
            f"{CERTIFIED_CALLS_MODEL_VERSION!r}"
        )

    callees = obligation.get("callees")
    if not isinstance(callees, list):
        return "certified-calls.callees must be a list"
    if not callees:
        return "certified-calls.callees must be nonempty"

    seen_ids: set[str] = set()
    for index, entry in enumerate(callees):
        prefix = f"certified-calls.callees[{index}]"
        if not isinstance(entry, Mapping):
            return f"{prefix} must be an object"
        unknown_entry = sorted(set(entry.keys()) - KNOWN_CALLEE_ENTRY_KEYS)
        if unknown_entry:
            return f"{prefix} unknown fields: {', '.join(unknown_entry)}"
        missing_entry = sorted(KNOWN_CALLEE_ENTRY_KEYS - set(entry.keys()))
        if missing_entry:
            return f"{prefix} missing fields: {', '.join(missing_entry)}"

        target_id = entry.get("target_id")
        if not isinstance(target_id, str) or not target_id:
            return f"{prefix}.target_id must be a nonempty string"
        if target_id in seen_ids:
            return f"{prefix}.target_id duplicate {target_id!r}"
        seen_ids.add(target_id)

        for digest_key in ("certificate_sha256", "body_sha256", "summary_sha256"):
            if not _is_sha256(entry.get(digest_key)):
                return f"{prefix}.{digest_key} must be a 64-hex digest"

        refinement = entry.get("refinement")
        if not isinstance(refinement, Mapping):
            return f"{prefix}.refinement must be an object"
        unknown_ref = sorted(set(refinement.keys()) - KNOWN_REFINEMENT_KEYS)
        if unknown_ref:
            return f"{prefix}.refinement unknown fields: {', '.join(unknown_ref)}"
        missing_ref = sorted(KNOWN_REFINEMENT_KEYS - set(refinement.keys()))
        if missing_ref:
            return f"{prefix}.refinement missing fields: {', '.join(missing_ref)}"
        result = refinement.get("result")
        if result not in KNOWN_REFINEMENT_RESULTS:
            return (
                f"{prefix}.refinement.result must be one of "
                f"{sorted(KNOWN_REFINEMENT_RESULTS)}"
            )
        if not _is_sha256(refinement.get("query_sha256")):
            return f"{prefix}.refinement.query_sha256 must be a 64-hex digest"

    if not _is_sha256(obligation.get("closure_sha256")):
        return "certified-calls.closure_sha256 must be a 64-hex digest"
    expected_closure = compute_closure_sha256(callees)
    if obligation.get("closure_sha256") != expected_closure:
        return "certified-calls.closure_sha256 does not match callees"

    return None


@dataclass(frozen=True)
class CalleeObligationInput:
    """Inputs needed to build one callee entry in a certified-calls obligation."""

    target_id: str
    symbol: str
    certificate_sha256: str
    retail_sha256: str
    candidate_sha256: str
    summary: Mapping[str, Any]
    contract_source: str = "certified"
    engine_hash: str = ""
    expected_engine_hash: str = ""
    required_invalid_reasons: frozenset[int] = frozenset()
    refinement: Mapping[str, Any] | None = None


@dataclass(frozen=True)
class CertifiedCallsContext:
    """Caller-side premises that gate Tier-A / promotion-grade."""

    unresolved_direct_calls: bool = False
    has_indirect_calls: bool = False
    indirect_target_set_closed: bool = True
    auto_assumed_relocated: bool = False
    call_graph: Mapping[str, frozenset[str]] | None = None
    induction_certificate: Mapping[str, Any] | None = None
    root_target_id: str = ""


def build_callee_entry(
    item: CalleeObligationInput,
    *,
    refinement: Mapping[str, Any] | None = None,
) -> dict[str, Any]:
    """Build one callees[] entry with identity digests and refinement block."""
    body = compute_body_sha256(
        retail_sha256=item.retail_sha256,
        candidate_sha256=item.candidate_sha256,
    )
    summary_digest = compute_summary_sha256(
        target_id=item.target_id,
        symbol=item.symbol,
        summary=item.summary,
    )
    ref = dict(refinement or item.refinement or _incomplete_refinement(item.target_id))
    return {
        "target_id": item.target_id,
        "certificate_sha256": item.certificate_sha256,
        "body_sha256": body,
        "summary_sha256": summary_digest,
        "refinement": {
            "result": str(ref.get("result", "incomplete")),
            "query_sha256": str(ref.get("query_sha256", "0" * _SHA256_LEN)),
        },
    }


def build_certified_calls_obligation(
    callees: Sequence[CalleeObligationInput | Mapping[str, Any]],
    *,
    refinements: Mapping[str, Mapping[str, Any]] | None = None,
) -> dict[str, Any]:
    """Assemble a certified-calls-v2 obligation (strict schema)."""
    entries: list[dict[str, Any]] = []
    ref_map = dict(refinements or {})
    for item in callees:
        if isinstance(item, CalleeObligationInput):
            entry = build_callee_entry(
                item, refinement=ref_map.get(item.target_id),
            )
        else:
            entry = {
                "target_id": str(item["target_id"]),
                "certificate_sha256": str(item["certificate_sha256"]),
                "body_sha256": str(item["body_sha256"]),
                "summary_sha256": str(item["summary_sha256"]),
                "refinement": {
                    "result": str(item["refinement"]["result"]),
                    "query_sha256": str(item["refinement"]["query_sha256"]),
                },
            }
            if item["target_id"] in ref_map:
                entry["refinement"] = {
                    "result": str(ref_map[item["target_id"]].get("result", "incomplete")),
                    "query_sha256": str(
                        ref_map[item["target_id"]].get(
                            "query_sha256", "0" * _SHA256_LEN
                        )
                    ),
                }
        entries.append(entry)
    entries.sort(key=lambda row: row["target_id"])
    return {
        "capability": CERTIFIED_CALLS_CAPABILITY,
        "model_version": CERTIFIED_CALLS_MODEL_VERSION,
        "callees": entries,
        "closure_sha256": compute_closure_sha256(entries),
    }


def _incomplete_refinement(seed: str) -> dict[str, str]:
    digest = hashlib.sha256(
        f"incomplete:{seed}:{TRIVIAL_LEAF_REFINEMENT_ALGORITHM}".encode("utf-8")
    ).hexdigest()
    return {"result": "incomplete", "query_sha256": digest}


def _is_trivial_leaf_summary(summary: Mapping[str, Any]) -> bool:
    """Identity / no-op summaries: no memory, no architectural writes."""
    writes = {str(item) for item in summary.get("writes", [])}
    reads = {str(item) for item in summary.get("reads", [])}
    if "memory" in writes or "memory" in reads or "*" in reads:
        return False
    # Pure identity: nothing written (blr / empty body effect).
    return not writes


def _is_blr_only_body(*, retail_hex: str | None, candidate_hex: str | None) -> bool:
    """True when both bodies are a single ``blr`` (``4e800020``)."""
    if not retail_hex or not candidate_hex:
        return False
    retail = retail_hex.replace(" ", "").lower()
    candidate = candidate_hex.replace(" ", "").lower()
    return retail == "4e800020" and candidate == "4e800020"


def discharge_trivial_leaf_refinement(
    *,
    target_id: str,
    symbol: str,
    summary: Mapping[str, Any],
    retail_hex: str | None = None,
    candidate_hex: str | None = None,
    deadline: Any | None = None,
) -> dict[str, str]:
    """SMT-discharge ordinary ≡ summary for trivial identity leaves.

    Only blr-only / empty-write leaves get a real UNSAT. Everything else is
    ``incomplete`` (fail closed for promotion-grade).
    """
    if not _is_trivial_leaf_summary(summary):
        return _incomplete_refinement(f"{target_id}:nontrivial")
    if not _is_blr_only_body(retail_hex=retail_hex, candidate_hex=candidate_hex):
        # Without a concrete blr body we refuse to claim UNSAT.
        return _incomplete_refinement(f"{target_id}:body-not-blr")

    try:
        import z3
    except ImportError:
        return _incomplete_refinement(f"{target_id}:no-z3")

    from tools.ppc_equivalence.deadline import Deadline
    from tools.ppc_equivalence.discharge import discharge_bad_conditions

    # Identity summary on blr: r3_out == r3_in on both ordinary and summary sides.
    # Bad behavior: summary result differs from ordinary (both identity → UNSAT).
    r3 = z3.BitVec(f"cc.{symbol}.r3", 32)
    ordinary_out = r3  # blr preserves GPRs
    summary_out = r3  # empty writes ⇒ identity
    bad = ordinary_out != summary_out
    resolved_deadline = deadline if deadline is not None else Deadline.after_ms(5_000)
    discharge = discharge_bad_conditions(
        premises=[],
        bad_conditions=[bad],
        deadline=resolved_deadline,
        algorithm=TRIVIAL_LEAF_REFINEMENT_ALGORITHM,
        z3_module=z3,
    )
    # Bind target identity into the query digest so token-sharing is fail-closed.
    query = hashlib.sha256(
        f"{discharge.query_sha256}:{target_id}:{symbol}".encode("utf-8")
    ).hexdigest()
    return {"result": discharge.status, "query_sha256": query}


def collect_rejection_reasons(
    callees: Sequence[CalleeObligationInput],
    *,
    context: CertifiedCallsContext | None = None,
    live_certificates: Mapping[str, Mapping[str, Any]] | None = None,
) -> list[str]:
    """Return ordered rejection codes that block promotion-grade."""
    reasons: list[str] = []
    ctx = context or CertifiedCallsContext()
    live = dict(live_certificates or {})

    if ctx.unresolved_direct_calls:
        reasons.append(REJECTION_UNRESOLVED_DIRECT)
    if ctx.has_indirect_calls and not ctx.indirect_target_set_closed:
        reasons.append(REJECTION_INDIRECT_NO_CLOSURE)
    if ctx.auto_assumed_relocated:
        reasons.append(REJECTION_AUTO_ASSUMED_RELOCATED)

    if ctx.call_graph is not None and ctx.root_target_id:
        cycle = detect_callee_cycle(ctx.call_graph, root=ctx.root_target_id)
        if cycle is not None:
            induction = ctx.induction_certificate
            if not isinstance(induction, Mapping) or induction.get("status") != "discharged":
                reasons.append(REJECTION_CIRCULAR_SCC)

    for item in callees:
        source = str(item.contract_source)
        if source in OPAQUE_SOURCES or source.startswith("opaque"):
            reasons.append(REJECTION_OPAQUE_EABI)
        if (
            item.expected_engine_hash
            and item.engine_hash
            and item.engine_hash != item.expected_engine_hash
        ):
            reasons.append(REJECTION_ENGINE_BOUND)
        live_cert = live.get(item.target_id)
        if live_cert is not None:
            live_hash = live_cert.get("certificate_sha256")
            if live_hash != item.certificate_sha256:
                reasons.append(REJECTION_STALE_CHILD)
            live_summary = live_cert.get("summary")
            if isinstance(live_summary, Mapping):
                expected = compute_summary_sha256(
                    target_id=item.target_id,
                    symbol=item.symbol,
                    summary=live_summary,
                )
                actual = compute_summary_sha256(
                    target_id=item.target_id,
                    symbol=item.symbol,
                    summary=item.summary,
                )
                if expected != actual:
                    reasons.append(REJECTION_SUMMARY_MISMATCH)
            live_symbol = live_cert.get("symbol")
            if isinstance(live_symbol, str) and live_symbol and live_symbol != item.symbol:
                # Same target_id / summary payload claimed under a different symbol.
                reasons.append(REJECTION_SYMBOL_TOKEN)
            claimed = live_cert.get("summary_sha256")
            if isinstance(claimed, str) and claimed:
                bound = compute_summary_sha256(
                    target_id=item.target_id,
                    symbol=item.symbol,
                    summary=item.summary,
                )
                if claimed != bound:
                    reasons.append(REJECTION_SYMBOL_TOKEN)
            live_body = compute_body_sha256(
                retail_sha256=str(live_cert.get("retail_sha256", "")),
                candidate_sha256=str(live_cert.get("candidate_sha256", "")),
            )
            expected_body = compute_body_sha256(
                retail_sha256=item.retail_sha256,
                candidate_sha256=item.candidate_sha256,
            )
            if live_body != expected_body and live_cert.get("retail_sha256"):
                reasons.append(REJECTION_BODY_MISMATCH)

        required = frozenset(int(x) for x in item.required_invalid_reasons)
        present = frozenset(
            int(x) for x in item.summary.get("invalid_reasons", [])
        )
        if required and not required.issubset(present):
            reasons.append(REJECTION_MISSING_DEFINEDNESS)

    # De-dupe preserving order.
    seen: set[str] = set()
    ordered: list[str] = []
    for reason in reasons:
        if reason not in seen:
            seen.add(reason)
            ordered.append(reason)
    return ordered


def obligation_promotion_eligible(
    obligation: Mapping[str, Any],
    *,
    rejection_reasons: Sequence[str] = (),
) -> bool:
    """True only when every callee refinement is real UNSAT and no rejections."""
    if validate_certified_calls_obligation(obligation) is not None:
        return False
    if rejection_reasons:
        return False
    for entry in obligation["callees"]:
        refinement = entry["refinement"]
        if refinement.get("result") != "unsat":
            return False
        if not _is_sha256(refinement.get("query_sha256")):
            return False
    return True


def evaluate_certified_calls_status(
    obligation: Mapping[str, Any] | None,
    *,
    rejection_reasons: Sequence[str] = (),
    assumptions: Sequence[str] = (),
) -> str:
    """Recompute attestation grade; opaque / incomplete never promotion-grade."""
    if obligation is None:
        return STATUS_INCOMPLETE
    if validate_certified_calls_obligation(obligation) is not None:
        return STATUS_INCOMPLETE
    if REJECTION_OPAQUE_EABI in rejection_reasons:
        return STATUS_INCOMPLETE
    if obligation_promotion_eligible(
        obligation, rejection_reasons=rejection_reasons
    ):
        return STATUS_PROMOTION_GRADE
    if assumptions and not rejection_reasons:
        return STATUS_SCOPED_ASSUMPTION
    return STATUS_INCOMPLETE


def build_certified_calls_attestation(
    obligation: Mapping[str, Any],
    *,
    rejection_reasons: Sequence[str] = (),
    assumptions: Sequence[str] = (),
    status: str | None = None,
) -> CapabilityAttestation:
    """Emit a certified-calls CapabilityAttestation wrapping the obligation."""
    error = validate_certified_calls_obligation(obligation)
    if error is not None:
        raise ValueError(error)
    recomputed = evaluate_certified_calls_status(
        obligation,
        rejection_reasons=rejection_reasons,
        assumptions=assumptions,
    )
    # Advisory only — evaluate_capability_assurance recomputes; never ship a
    # caller-forged promotion-grade when recomputed grade is weaker.
    advisory = status if status is not None else recomputed
    if (
        advisory == STATUS_PROMOTION_GRADE
        and recomputed != STATUS_PROMOTION_GRADE
    ):
        advisory = recomputed
    evidence: dict[str, Any] = {
        "obligation": dict(obligation),
        "rejection_reasons": list(rejection_reasons),
    }
    unsupported = tuple(
        reason for reason in rejection_reasons if reason == REJECTION_OPAQUE_EABI
    )
    return build_attestation(
        capability=CERTIFIED_CALLS_CAPABILITY,
        model_version=CERTIFIED_CALLS_MODEL_VERSION,
        algorithm=CERTIFIED_CALLS_ALGORITHM,
        status=advisory,
        assumptions=tuple(assumptions),
        unsupported=unsupported,
        evidence=evidence,
    )


def draft_certified_calls_assurance(
    result: Any,
    *,
    callees: Sequence[CalleeObligationInput] = (),
    context: CertifiedCallsContext | None = None,
    live_certificates: Mapping[str, Mapping[str, Any]] | None = None,
) -> CapabilityAssurance | None:
    """Build a certified-calls assurance block when ``assumed_callees`` is set.

    Status is advisory; ``evaluate_capability_assurance`` recomputes the grade.
    Opaque EABI / incomplete refinements stay non-promotion-grade.
    """
    assumed = getattr(result, "assumed_callees", None) or []
    if not assumed and not callees:
        return None

    inputs = list(callees)
    if not inputs:
        # Minimal placeholder entries from assumed_callees alone (incomplete).
        for name in sorted(assumed, key=str):
            symbol = str(name)
            summary = {"reads": [], "writes": [], "invalid_reasons": []}
            contracts = getattr(result, "callee_contracts", None) or {}
            contract = contracts.get(symbol) or contracts.get(name) or {}
            source = "opaque-eabi"
            if isinstance(contract, Mapping):
                source = str(contract.get("source", source))
                summary = {
                    "reads": list(contract.get("reads", [])),
                    "writes": list(contract.get("writes", [])),
                    "invalid_reasons": list(contract.get("invalid_reasons", [])),
                    "return_behavior": "normal",
                }
            inputs.append(
                CalleeObligationInput(
                    target_id=symbol,
                    symbol=symbol,
                    certificate_sha256="0" * _SHA256_LEN,
                    retail_sha256="0" * _SHA256_LEN,
                    candidate_sha256="0" * _SHA256_LEN,
                    summary=summary,
                    contract_source=source,
                )
            )

    refinements: dict[str, dict[str, str]] = {}
    for item in inputs:
        refinements[item.target_id] = dict(
            item.refinement
            if item.refinement is not None
            else _incomplete_refinement(item.target_id)
        )

    obligation = build_certified_calls_obligation(inputs, refinements=refinements)
    rejection_reasons = collect_rejection_reasons(
        inputs, context=context, live_certificates=live_certificates
    )
    attestation = build_certified_calls_attestation(
        obligation,
        rejection_reasons=rejection_reasons,
        status=STATUS_INCOMPLETE,
    )
    return CapabilityAssurance(capabilities=(attestation,))


def maybe_attach_certified_calls_draft(
    result: Any,
    *,
    callees: Sequence[CalleeObligationInput] = (),
    context: CertifiedCallsContext | None = None,
    live_certificates: Mapping[str, Mapping[str, Any]] | None = None,
) -> Any:
    """Attach / merge a certified-calls attestation when assumed_callees present."""
    from tools.ppc_equivalence.capability_assurance import _merge_assurance_attestation

    draft = draft_certified_calls_assurance(
        result,
        callees=callees,
        context=context,
        live_certificates=live_certificates,
    )
    if draft is None:
        return result
    merged = _merge_assurance_attestation(
        getattr(result, "capability_assurance", None),
        draft.capabilities[0],
    )
    result.capability_assurance = merged.to_dict()
    return result


def verify_certificate_sha256_binding(
    obligation: Mapping[str, Any],
    *,
    mutated_field: str = "certificate_sha256",
) -> str | None:
    """Fail closed when a callee certificate digest is mutated in-place."""
    error = validate_certified_calls_obligation(obligation)
    if error is not None:
        return error
    # Recompute closure after a hypothetical mutation of the first entry.
    callees = [dict(entry) for entry in obligation["callees"]]
    if not callees:
        return "certified-calls.callees must be nonempty"
    original = callees[0].get(mutated_field)
    if not isinstance(original, str) or len(original) != _SHA256_LEN:
        return f"cannot mutate {mutated_field}"
    # Flip first nibble.
    flipped = ("1" if original[0] == "0" else "0") + original[1:]
    callees[0][mutated_field] = flipped
    tampered = {
        "capability": obligation["capability"],
        "model_version": obligation["model_version"],
        "callees": callees,
        "closure_sha256": obligation["closure_sha256"],  # stale on purpose
    }
    return validate_certified_calls_obligation(tampered)
