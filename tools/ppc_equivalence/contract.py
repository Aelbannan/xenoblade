from __future__ import annotations

from dataclasses import dataclass


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

    def __post_init__(self) -> None:
        if not self.observables:
            raise ValueError("at least one observable is required")
        if not 1 <= self.timeout_ms <= 600_000:
            raise ValueError("solver timeout must be between 1 and 600000 ms")


CONTRACT_PRESETS = ("ppc-eabi", "strict")


def preset_observable_names(name: str) -> tuple[str, ...]:
    if name == "ppc-eabi":
        # Function-boundary contract for the 32-bit PowerPC EABI used by MWCC:
        # stack/small-data bases, scalar return pair, nonvolatile GPRs, and the
        # nonvolatile CR fields. Volatile scratch state is intentionally omitted.
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
            "cr2",
            "cr3",
            "cr4",
        )
    if name == "strict":
        return tuple(f"r{index}" for index in range(32)) + (
            "cr",
            "xer.ca",
            "xer.ov",
            "xer.so",
            "lr",
            "ctr",
        )
    raise ValueError(f"unknown contract preset '{name}'")


def make_contract(
    *,
    preset: str | None,
    observe: list[str] | tuple[str, ...] | None,
    timeout_ms: int,
) -> EquivalenceContract:
    if preset is not None and observe:
        raise ValueError("--contract and --observe are mutually exclusive")
    if preset is not None:
        return EquivalenceContract(
            parse_observables(preset_observable_names(preset)),
            timeout_ms,
            preset,
        )
    if observe:
        return EquivalenceContract(parse_observables(observe), timeout_ms, "manual")
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
        elif token.startswith("cr") and token[2:].isdigit() and 0 <= int(token[2:]) < 8:
            result.append(Observable("cr_field", token, int(token[2:])))
        elif token in ("cr", "lr", "ctr"):
            result.append(Observable(token, token))
        elif token in ("xer.ca", "xer.ov", "xer.so"):
            result.append(Observable("xer", token, None))
        else:
            raise ValueError(f"unsupported observable '{token}'")
    return tuple(result)
