from __future__ import annotations

from dataclasses import dataclass
from enum import Enum


class Opcode(str, Enum):
    ADDI = "addi"
    ADDIS = "addis"
    MULLI = "mulli"
    ORI = "ori"
    ORIS = "oris"
    XORI = "xori"
    XORIS = "xoris"
    ANDI_DOT = "andi."
    ANDIS_DOT = "andis."
    RLWINM = "rlwinm"
    RLWIMI = "rlwimi"
    AND = "and"
    OR = "or"
    XOR = "xor"
    NOR = "nor"
    ADD = "add"
    SUBF = "subf"
    CMPWI = "cmpwi"
    CMPLWI = "cmplwi"
    CMPW = "cmpw"
    CMPLW = "cmplw"
    EXTSB = "extsb"
    EXTSH = "extsh"
    CNTLZW = "cntlzw"


@dataclass(frozen=True, slots=True)
class Instruction:
    address: int
    raw: int
    opcode: Opcode
    operands: tuple[int, ...]
    record: bool = False
    display_mnemonic: str | None = None

    def to_dict(self) -> dict[str, object]:
        return {
            "address": f"0x{self.address:08x}",
            "bytes": self.raw.to_bytes(4, "big").hex(" "),
            "opcode": self.opcode.value,
            "operands": list(self.operands),
            "record": self.record,
            "display_mnemonic": self.display_mnemonic or self.opcode.value,
        }


class DecodeError(ValueError):
    pass


class UnsupportedInstruction(DecodeError):
    def __init__(self, address: int, word: int, reason: str) -> None:
        self.address = address
        self.word = word
        self.reason = reason
        super().__init__(f"{reason} at 0x{address:08x} (0x{word:08x})")
