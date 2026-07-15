from __future__ import annotations

from dataclasses import dataclass

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

    def __post_init__(self) -> None:
        if not self.observables and self.name != "live-out":
            raise ValueError("at least one observable is required")
        if not 1 <= self.timeout_ms <= 600_000:
            raise ValueError("solver timeout must be between 1 and 600000 ms")


CONTRACT_PRESETS = ("ppc-eabi", "strict", "live-out")


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
    raise ValueError(f"unknown contract preset '{name}'")


def make_contract(
    *,
    preset: str | None,
    observe: list[str] | tuple[str, ...] | None,
    timeout_ms: int,
    live_out: tuple[str, ...] | None = None,
) -> EquivalenceContract:
    if preset is not None and observe:
        raise ValueError("--contract and --observe are mutually exclusive")
    if preset is not None:
        names = live_out if preset == "live-out" else preset_observable_names(preset)
        return EquivalenceContract(
            parse_observables(names) if names else (),
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
