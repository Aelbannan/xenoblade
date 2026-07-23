"""Explicit ABI / live-out shape for exit-kind observable narrowing.

Fail-closed defaults observe the full PowerPC EABI outgoing-argument set
(``r3``–``r10``, ``f1``–``f8``) at ``indirect-branch`` / ``call-indirect``
exits. Narrowing is opt-in via an attached ``AbiShape`` (or conservative
inference when evidence is strong).
"""

from __future__ import annotations

from dataclasses import asdict, dataclass
from typing import Any, Mapping


@dataclass(frozen=True, slots=True)
class AbiShape:
    """Signature / live-out hints that may omit half of a dual return or an
    unused outgoing argument register at selected exit kinds.

    Defaults are fail-closed: observe all eight integer and eight float
    outgoing EABI argument registers at tail transfers.
    """

    returns_i64: bool = True  # if False, omit r4 on return/fallthrough
    returns_float: bool = True  # if False, omit f1/f1.ps1 on return/fallthrough
    outgoing_gpr_args: int = 8  # count of live r3.. among r3–r10 at indirect exits
    outgoing_fpr_args: int = 8  # count of live f1.. among f1–f8 at indirect exits
    source: str = "default-conservative"

    def __post_init__(self) -> None:
        if not 0 <= self.outgoing_gpr_args <= 8:
            raise ValueError("outgoing_gpr_args must be in 0..8")
        if not 0 <= self.outgoing_fpr_args <= 8:
            raise ValueError("outgoing_fpr_args must be in 0..8")

    def to_dict(self) -> dict[str, object]:
        return asdict(self)

    @classmethod
    def from_dict(cls, data: Mapping[str, Any] | None) -> AbiShape:
        if data is None:
            return cls.conservative()
        return cls(
            returns_i64=bool(data.get("returns_i64", True)),
            returns_float=bool(data.get("returns_float", True)),
            outgoing_gpr_args=int(data.get("outgoing_gpr_args", 8)),
            outgoing_fpr_args=int(data.get("outgoing_fpr_args", 8)),
            source=str(data.get("source", "from-dict")),
        )

    @classmethod
    def conservative(cls) -> AbiShape:
        """Fail-closed: observe all EABI outgoing args at indirect exits."""
        return cls()
