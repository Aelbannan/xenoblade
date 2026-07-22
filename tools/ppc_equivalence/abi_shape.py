"""Explicit ABI / live-out shape for exit-kind observable narrowing.

Fail-closed defaults preserve today's full ``ppc-eabi`` observation of ``r4`` /
``f1`` / ``f1.ps1`` on every exit kind. Narrowing is opt-in via an attached
``AbiShape`` (or conservative inference when evidence is strong).
"""

from __future__ import annotations

from dataclasses import asdict, dataclass
from typing import Any, Mapping


@dataclass(frozen=True, slots=True)
class AbiShape:
    """Signature / live-out hints that may omit half of a dual return or an
    unused outgoing argument register at selected exit kinds.

    Defaults match today's conservative EABI observation (always compare
    ``r4`` / ``f1`` / ``f1.ps1``).
    """

    returns_i64: bool = True  # if False, omit r4 on return/fallthrough
    returns_float: bool = True  # if False, omit f1/f1.ps1 on return/fallthrough
    outgoing_gpr_args: int = 2  # if < 2, omit r4 on indirect-branch/call-indirect
    outgoing_fpr_args: int = 1  # if < 1, omit f1/f1.ps1 on indirect-branch/call-indirect
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
            outgoing_gpr_args=int(data.get("outgoing_gpr_args", 2)),
            outgoing_fpr_args=int(data.get("outgoing_fpr_args", 1)),
            source=str(data.get("source", "from-dict")),
        )

    @classmethod
    def conservative(cls) -> AbiShape:
        """Today's behavior: observe r4/f1 on every exit kind."""
        return cls()
