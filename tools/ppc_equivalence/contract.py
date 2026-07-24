from __future__ import annotations

from dataclasses import dataclass, replace

from .abi_shape import AbiShape
from .spr import AUX_SPR_NAME_INDEX, AUX_SPR_OBSERVABLES


@dataclass(frozen=True, slots=True)
class Observable:
    kind: str
    name: str
    index: int | None = None


@dataclass(frozen=True, slots=True)
class EquivalenceContract:
    observables: tuple[Observable, ...]
    timeout_ms: int = 10_000
    name: str = "manual"
    base_name: str | None = None
    auto_added: tuple[str, ...] = ()
    auto_reasons: tuple[str, ...] = ()
    abi_shape: AbiShape | None = None

    def __post_init__(self) -> None:
        if not self.observables and self.name != "live-out":
            raise ValueError("at least one observable is required")
        if not 1 <= self.timeout_ms <= 1_800_000:
            raise ValueError("solver timeout must be between 1 and 1800000 ms")

    def resolution_dict(self) -> dict[str, object] | None:
        if self.base_name is None and self.abi_shape is None:
            return None
        result: dict[str, object] = {}
        if self.base_name is not None:
            result["base"] = self.base_name
            result["added"] = list(self.auto_added)
            result["reasons"] = list(self.auto_reasons)
        if self.abi_shape is not None:
            result["abi_shape"] = self.abi_shape.to_dict()
        return result


CONTRACT_PRESETS = ("auto", "ppc-eabi", "ppc-eabi-fp", "strict", "live-out")

# State that persists beyond a normal function call but is not part of the
# compiler's ordinary EABI register contract.  Auto adds a component only when
# either implementation writes it.  Volatile compiler scratch (GPR/FPR, CR,
# XER, LR, and CTR) deliberately remains governed by ppc-eabi.
AUTO_PERSISTENT_OBSERVABLES = (
    "fpscr",
    *(f"gqr{index}" for index in range(8)),
    *(f"sr{index}" for index in range(16)),
    "msr",
    "time_base",
    "srr0",
    "srr1",
    *AUX_SPR_OBSERVABLES,
)

# Historical: an exit-kind filter once omitted ``r4`` / ``f1`` / ``f1.ps1`` on
# ``indirect-branch`` terminals, treating them as dead return halves after a
# tail transfer.  That was unsound — at a ``bctr`` tail call those registers
# are live outgoing arguments to the callee.  Kept as an empty frozenset so
# older imports/tests that mention the name keep resolving.
INDIRECT_BRANCH_OMITTED_OBSERVABLES: frozenset[str] = frozenset()

# PowerPC EABI outgoing argument registers at a tail call / virtual thunk.
# ``ppc-eabi`` / ``auto`` base contracts only list ``r3``/``r4`` and ``f1`` among
# volatiles (return halves). At ``indirect-branch`` / ``call-indirect`` those
# same slots are live outgoing args, and so are ``r5``–``r10`` / ``f2``–``f8``.
EABI_OUTGOING_GPR_ARGS: tuple[str, ...] = tuple(f"r{index}" for index in range(3, 11))
EABI_OUTGOING_FPR_ARGS: tuple[str, ...] = tuple(f"f{index}" for index in range(1, 9))
EABI_OUTGOING_PS1_ARGS: tuple[str, ...] = tuple(f"f{index}.ps1" for index in range(1, 9))


def observables_for_exit(
    contract: EquivalenceContract,
    exit_kind: str,
) -> tuple[Observable, ...]:
    """Select observables for a matched terminal pair.

    For ``indirect-branch`` / ``call-indirect``, the compared set is the contract
    observables **union** the EABI outgoing argument registers (``r3``–``r10``,
    ``f1``–``f8`` and ``.ps1`` lanes), truncated by an attached :class:`AbiShape`
    when present. Without a shape, all eight integer and eight float outgoing
    args are live (fail closed).

    When an explicit :class:`AbiShape` is attached, ``r4`` / ``f1`` / ``f1.ps1``
    may also be omitted on return/fallthrough according to return-width flags.
    ``r3``, memory, nonvolatiles, and other non-arg observables are never
    dropped here.
    """
    if exit_kind in ("indirect-branch", "call-indirect"):
        return _observables_for_indirect_exit(contract)

    if contract.abi_shape is None:
        return contract.observables

    omit: set[str] = set()
    shape = contract.abi_shape
    if exit_kind in ("return", "fallthrough"):
        if not shape.returns_i64:
            omit.add("r4")
        if not shape.returns_float:
            omit.update({"f1", "f1.ps1"})
    return tuple(o for o in contract.observables if o.name not in omit)


def _observables_for_indirect_exit(
    contract: EquivalenceContract,
) -> tuple[Observable, ...]:
    """Union contract observables with live EABI outgoing args at a tail transfer."""
    if contract.abi_shape is None:
        gpr_n = 8
        fpr_n = 8
    else:
        gpr_n = int(contract.abi_shape.outgoing_gpr_args)
        fpr_n = int(contract.abi_shape.outgoing_fpr_args)

    live_args = (
        EABI_OUTGOING_GPR_ARGS[:gpr_n]
        + EABI_OUTGOING_FPR_ARGS[:fpr_n]
        + EABI_OUTGOING_PS1_ARGS[:fpr_n]
    )
    # Drop outgoing-arg registers beyond the declared count that may still be
    # present in the base ``ppc-eabi`` contract (e.g. ``r4`` / ``f1`` when
    # ``outgoing_gpr_args=1``).
    omit = set(EABI_OUTGOING_GPR_ARGS[gpr_n:])
    omit.update(EABI_OUTGOING_FPR_ARGS[fpr_n:])
    omit.update(EABI_OUTGOING_PS1_ARGS[fpr_n:])

    merged: list[Observable] = []
    seen: set[str] = set()
    for item in contract.observables:
        if item.name in omit:
            continue
        if item.name not in seen:
            merged.append(item)
            seen.add(item.name)
    for name in live_args:
        if name in seen or name in omit:
            continue
        merged.extend(parse_observables((name,)))
        seen.add(name)
    return tuple(merged)


def with_abi_shape(
    contract: EquivalenceContract,
    shape: AbiShape,
) -> EquivalenceContract:
    """Return a copy of ``contract`` with ``abi_shape`` attached."""
    return replace(contract, abi_shape=shape)


def preset_observable_names(name: str) -> tuple[str, ...]:
    if name == "ppc-eabi":
        # Function-boundary contract for the 32-bit PowerPC EABI used by MWCC:
        # stack/small-data bases, integer and FP returns, nonvolatile GPRs/FPRs,
        # and the nonvolatile CR fields. Volatile scratch state is omitted.
        return (
            "r1",
            "r2",
            "r3",
            "r4",
            "r13",
            "r14",
            "r15",
            "r16",
            "r17",
            "r18",
            "r19",
            "r20",
            "r21",
            "r22",
            "r23",
            "r24",
            "r25",
            "r26",
            "r27",
            "r28",
            "r29",
            "r30",
            "r31",
            "f1",
            "f1.ps1",
            "f14",
            "f15",
            "f16",
            "f17",
            "f18",
            "f19",
            "f20",
            "f21",
            "f22",
            "f23",
            "f24",
            "f25",
            "f26",
            "f27",
            "f28",
            "f29",
            "f30",
            "f31",
            "f14.ps1", "f15.ps1", "f16.ps1", "f17.ps1", "f18.ps1", "f19.ps1",
            "f20.ps1", "f21.ps1", "f22.ps1", "f23.ps1", "f24.ps1", "f25.ps1",
            "f26.ps1", "f27.ps1", "f28.ps1", "f29.ps1", "f30.ps1", "f31.ps1",
            "cr2",
            "cr3",
            "cr4",
            "memory",
        )
    if name == "ppc-eabi-fp":
        return preset_observable_names("ppc-eabi") + ("fpscr",)
    if name == "strict":
        return tuple(f"r{index}" for index in range(32)) + tuple(f"f{index}" for index in range(32)) + tuple(
            f"f{index}.ps1" for index in range(32)
        ) + tuple(f"gqr{index}" for index in range(8)) + (
            *(f"sr{index}" for index in range(16)),
            "cr",
            "fpscr",
            "xer.ca",
            "xer.ov",
            "xer.so",
            "lr",
            "ctr",
            "msr",
            "time_base",
            "srr0",
            "srr1",
            *AUX_SPR_OBSERVABLES,
            "memory",
        )
    if name == "live-out":
        raise ValueError("live-out observables require decoded instructions")
    if name == "auto":
        raise ValueError("auto observables require decoded instructions")
    raise ValueError(f"unknown contract preset '{name}'")


def _auto_contract(
    original_live_out: tuple[str, ...],
    candidate_live_out: tuple[str, ...],
    timeout_ms: int,
    abi_shape: AbiShape | None = None,
) -> EquivalenceContract:
    base_names = preset_observable_names("ppc-eabi")
    base_set = set(base_names)
    original_writes = set(original_live_out)
    candidate_writes = set(candidate_live_out)
    added = tuple(
        name
        for name in AUTO_PERSISTENT_OBSERVABLES
        if name not in base_set and (name in original_writes or name in candidate_writes)
    )
    reasons: list[str] = []
    for name in added:
        sides = []
        if name in original_writes:
            sides.append("original")
        if name in candidate_writes:
            sides.append("candidate")
        reasons.append(f"{name} written by {' and '.join(sides)}")
    return EquivalenceContract(
        parse_observables(base_names + added),
        timeout_ms,
        "auto",
        base_name="ppc-eabi",
        auto_added=added,
        auto_reasons=tuple(reasons),
        abi_shape=abi_shape,
    )


def refine_auto_contract_with_writes(
    contract: EquivalenceContract,
    *,
    symbolic_writes: set[str] | frozenset[str],
) -> EquivalenceContract:
    """Union auto-persistent observables discovered from symbolic terminal writes.

    The syntactic ``automatic_live_out`` table is a fast pre-filter; this
    refinement is the soundness backstop so a mis-tabled persistent write
    cannot silently drop an observable under ``auto``.
    """
    if contract.name != "auto":
        return contract
    base_names = preset_observable_names("ppc-eabi")
    base_set = set(base_names)
    already = set(contract.auto_added)
    extra = tuple(
        name
        for name in AUTO_PERSISTENT_OBSERVABLES
        if name not in base_set
        and name not in already
        and name in symbolic_writes
    )
    if not extra:
        return contract
    added = tuple(contract.auto_added) + extra
    reasons = tuple(contract.auto_reasons) + tuple(
        f"{name} written by symbolic terminal refinement" for name in extra
    )
    return EquivalenceContract(
        parse_observables(base_names + added),
        contract.timeout_ms,
        "auto",
        base_name="ppc-eabi",
        auto_added=added,
        auto_reasons=reasons,
        abi_shape=contract.abi_shape,
    )


def make_contract(
    *,
    preset: str | None,
    observe: list[str] | tuple[str, ...] | None,
    timeout_ms: int,
    live_out: tuple[str, ...] | None = None,
    original_live_out: tuple[str, ...] | None = None,
    candidate_live_out: tuple[str, ...] | None = None,
    abi_shape: AbiShape | None = None,
) -> EquivalenceContract:
    if preset is not None and observe:
        raise ValueError("--contract and --observe are mutually exclusive")
    if preset is not None:
        if preset == "auto":
            if original_live_out is None or candidate_live_out is None:
                raise ValueError("auto contract requires both decoded instruction sequences")
            return _auto_contract(
                original_live_out,
                candidate_live_out,
                timeout_ms,
                abi_shape=abi_shape,
            )
        names = live_out if preset == "live-out" else preset_observable_names(preset)
        return EquivalenceContract(
            parse_observables(names) if names else (),
            timeout_ms,
            preset,
            abi_shape=abi_shape,
        )
    if observe:
        return EquivalenceContract(
            parse_observables(observe),
            timeout_ms,
            "manual",
            abi_shape=abi_shape,
        )
    raise ValueError("select --contract or --observe")


def parse_observables(values: list[str] | tuple[str, ...]) -> tuple[Observable, ...]:
    tokens: list[str] = []
    for value in values:
        tokens.extend(part.strip().lower() for part in value.split(",") if part.strip())
    if not tokens:
        raise ValueError("at least one observable is required")
    result: list[Observable] = []
    seen: set[str] = set()
    for token in tokens:
        if token in seen:
            continue
        seen.add(token)
        if token.startswith("r") and token[1:].isdigit() and 0 <= int(token[1:]) < 32:
            result.append(Observable("gpr", token, int(token[1:])))
        elif token.startswith("f") and token[1:].isdigit() and 0 <= int(token[1:]) < 32:
            result.append(Observable("fpr", token, int(token[1:])))
        elif token.startswith("f") and token.endswith(".ps1") and token[1:-4].isdigit() and 0 <= int(token[1:-4]) < 32:
            result.append(Observable("ps1", token, int(token[1:-4])))
        elif token.startswith("gqr") and token[3:].isdigit() and 0 <= int(token[3:]) < 8:
            result.append(Observable("gqr", token, int(token[3:])))
        elif token.startswith("sr") and token[2:].isdigit() and 0 <= int(token[2:]) < 16:
            result.append(Observable("sr", token, int(token[2:])))
        elif token in AUX_SPR_NAME_INDEX:
            result.append(Observable("spr", token, AUX_SPR_NAME_INDEX[token]))
        elif token.startswith("cr") and token[2:].isdigit() and 0 <= int(token[2:]) < 8:
            result.append(Observable("cr_field", token, int(token[2:])))
        elif token in ("cr", "lr", "ctr", "msr", "time_base", "srr0", "srr1"):
            result.append(Observable(token, token))
        elif token == "memory":
            result.append(Observable("memory", "memory"))
        elif token in ("xer.ca", "xer.ov", "xer.so"):
            result.append(Observable("xer", token, None))
        elif token == "fpscr":
            result.append(Observable("fpscr", "fpscr"))
        else:
            raise ValueError(f"unsupported observable '{token}'")
    return tuple(result)
