from __future__ import annotations

from dataclasses import dataclass
from enum import Enum


class Opcode(str, Enum):
    # Immediate integer arithmetic/logical.
    ADDI = "addi"
    ADDIS = "addis"
    ADDIC = "addic"
    ADDIC_DOT = "addic."
    SUBFIC = "subfic"
    MULLI = "mulli"
    ORI = "ori"
    ORIS = "oris"
    XORI = "xori"
    XORIS = "xoris"
    ANDI_DOT = "andi."
    ANDIS_DOT = "andis."

    # Rotate, logical, shift, arithmetic, and compare.
    RLWINM = "rlwinm"
    RLWIMI = "rlwimi"
    RLWNM = "rlwnm"
    AND = "and"
    ANDC = "andc"
    EQV = "eqv"
    NAND = "nand"
    NOR = "nor"
    OR = "or"
    ORC = "orc"
    XOR = "xor"
    SLW = "slw"
    SRW = "srw"
    SRAW = "sraw"
    SRAWI = "srawi"
    ADD = "add"
    ADDC = "addc"
    ADDE = "adde"
    ADDME = "addme"
    ADDZE = "addze"
    SUBF = "subf"
    SUBFC = "subfc"
    SUBFE = "subfe"
    SUBFME = "subfme"
    SUBFZE = "subfze"
    NEG = "neg"
    MULHW = "mulhw"
    MULHWU = "mulhwu"
    MULLW = "mullw"
    DIVW = "divw"
    DIVWU = "divwu"
    CMPWI = "cmpwi"
    CMPLWI = "cmplwi"
    CMPW = "cmpw"
    CMPLW = "cmplw"
    EXTSB = "extsb"
    EXTSH = "extsh"
    CNTLZW = "cntlzw"

    # Condition-register and special-register transfers.
    CRAND = "crand"
    CRANDC = "crandc"
    CREQV = "creqv"
    CRNAND = "crnand"
    CRNOR = "crnor"
    CROR = "cror"
    CRORC = "crorc"
    CRXOR = "crxor"
    MFCR = "mfcr"
    MTCRF = "mtcrf"
    MFSPR = "mfspr"
    MTSPR = "mtspr"

    # Integer storage access (big-endian unless BRX).
    LBZ = "lbz"
    LBZU = "lbzu"
    LHZ = "lhz"
    LHZU = "lhzu"
    LHA = "lha"
    LHAU = "lhau"
    LWZ = "lwz"
    LWZU = "lwzu"
    STB = "stb"
    STBU = "stbu"
    STH = "sth"
    STHU = "sthu"
    STW = "stw"
    STWU = "stwu"
    LMW = "lmw"
    STMW = "stmw"
    LBZX = "lbzx"
    LBZUX = "lbzux"
    LHZX = "lhzx"
    LHZUX = "lhzux"
    LHAX = "lhax"
    LHAUX = "lhaux"
    LWZX = "lwzx"
    LWZUX = "lwzux"
    STBX = "stbx"
    STBUX = "stbux"
    STHX = "sthx"
    STHUX = "sthux"
    STWX = "stwx"
    STWUX = "stwux"
    LHBRX = "lhbrx"
    LWBRX = "lwbrx"
    STHBRX = "sthbrx"
    STWBRX = "stwbrx"

    # User-level control flow.
    B = "b"
    BC = "bc"
    BCLR = "bclr"
    BCCTR = "bcctr"


@dataclass(frozen=True, slots=True)
class Instruction:
    address: int
    raw: int
    opcode: Opcode
    operands: tuple[int, ...]
    record: bool = False
    overflow: bool = False
    link: bool = False
    display_mnemonic: str | None = None

    def to_dict(self) -> dict[str, object]:
        return {
            "address": f"0x{self.address:08x}",
            "bytes": self.raw.to_bytes(4, "big").hex(" "),
            "opcode": self.opcode.value,
            "operands": list(self.operands),
            "record": self.record,
            "overflow": self.overflow,
            "link": self.link,
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


class ExecutionInconclusive(RuntimeError):
    """The decoded program is valid but outside the bounded execution model."""
