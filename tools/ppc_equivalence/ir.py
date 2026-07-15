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

    # Floating-point load/store (D-form).
    LFS = "lfs"
    LFSU = "lfsu"
    LFD = "lfd"
    LFDU = "lfdu"
    STFS = "stfs"
    STFSU = "stfsu"
    STFD = "stfd"
    STFDU = "stfdu"

    # Floating-point load/store indexed (X-form).
    LFSX = "lfsx"
    LFSUX = "lfsux"
    LFDX = "lfdx"
    LFDUX = "lfdux"
    STFSX = "stfsx"
    STFSUX = "stfsux"
    STFDX = "stfdx"
    STFDUX = "stfdux"
    STFIWX = "stfiwx"

    # Single-precision floating-point arithmetic.
    FADDS = "fadds"
    FSUBS = "fsubs"
    FMULS = "fmuls"
    FDIVS = "fdivs"
    FSQRTS = "fsqrts"
    FRES = "fres"
    FMADDS = "fmadds"
    FMSUBS = "fmsubs"
    FNMADDS = "fnmadds"
    FNMSUBS = "fnmsubs"

    # Double-precision floating-point arithmetic.
    FADD = "fadd"
    FSUB = "fsub"
    FMUL = "fmul"
    FDIV = "fdiv"
    FSQRT = "fsqrt"
    FRSQRTE = "frsqrte"
    FSEL = "fsel"
    FMADD = "fmadd"
    FMSUB = "fmsub"
    FNMADD = "fnmadd"
    FNMSUB = "fnmsub"

    # Floating-point compare / convert / move.
    FCMPU = "fcmpu"
    FCMPO = "fcmpo"
    FRSP = "frsp"
    FCTIW = "fctiw"
    FCTIWZ = "fctiwz"
    FNEG = "fneg"
    FMR = "fmr"
    FNABS = "fnabs"
    FABS = "fabs"

    # FPSCR access.
    MTFSB0 = "mtfsb0"
    MTFSB1 = "mtfsb1"
    MTFSFI = "mtfsfi"
    MFFS = "mffs"
    MTFSF = "mtfsf"
    MCRFS = "mcrfs"

    # Paired-single arithmetic.
    PS_DIV = "ps_div"
    PS_SUB = "ps_sub"
    PS_ADD = "ps_add"
    PS_SEL = "ps_sel"
    PS_RES = "ps_res"
    PS_MUL = "ps_mul"
    PS_RSQRTE = "ps_rsqrte"
    PS_MSUB = "ps_msub"
    PS_MADD = "ps_madd"
    PS_NMSUB = "ps_nmsub"
    PS_NMADD = "ps_nmadd"
    PS_SUM0 = "ps_sum0"
    PS_SUM1 = "ps_sum1"
    PS_MULS0 = "ps_muls0"
    PS_MULS1 = "ps_muls1"
    PS_MADDS0 = "ps_madds0"
    PS_MADDS1 = "ps_madds1"
    PS_NEG = "ps_neg"
    PS_MR = "ps_mr"
    PS_NABS = "ps_nabs"
    PS_ABS = "ps_abs"

    # Paired-single compare.
    PS_CMPU0 = "ps_cmpu0"
    PS_CMPO0 = "ps_cmpo0"
    PS_CMPU1 = "ps_cmpu1"
    PS_CMPO1 = "ps_cmpo1"

    # Paired-single merge.
    PS_MERGE00 = "ps_merge00"
    PS_MERGE01 = "ps_merge01"
    PS_MERGE10 = "ps_merge10"
    PS_MERGE11 = "ps_merge11"

    # Paired-single quantized load/store (D-form).
    PSQ_L = "psq_l"
    PSQ_LU = "psq_lu"
    PSQ_ST = "psq_st"
    PSQ_STU = "psq_stu"

    # Paired-single quantized load/store indexed (X-form).
    PSQ_LX = "psq_lx"
    PSQ_LUX = "psq_lux"
    PSQ_STX = "psq_stx"
    PSQ_STUX = "psq_stux"


# Scalar FP instructions whose value semantics are implemented by both
# ConcreteOps and SymbolicOps.  Compare invalid exceptions are modeled;
# arithmetic exception-enable suppression and architectural trap delivery
# remain outside the proof model (see README.md).
SUPPORTED_FP_OPCODES = frozenset({
    Opcode.LFS, Opcode.LFSU, Opcode.LFD, Opcode.LFDU,
    Opcode.STFS, Opcode.STFSU, Opcode.STFD, Opcode.STFDU,
    Opcode.LFSX, Opcode.LFSUX, Opcode.LFDX, Opcode.LFDUX,
    Opcode.STFSX, Opcode.STFSUX, Opcode.STFDX, Opcode.STFDUX, Opcode.STFIWX,
    Opcode.FADDS, Opcode.FSUBS, Opcode.FMULS, Opcode.FDIVS,
    Opcode.FADD, Opcode.FSUB, Opcode.FMUL, Opcode.FDIV,
    Opcode.FSEL, Opcode.FCMPU, Opcode.FCMPO, Opcode.FRSP,
    Opcode.FNEG, Opcode.FMR, Opcode.FNABS, Opcode.FABS,
})


SUPPORTED_OPCODES = frozenset(
    opcode
    for opcode in Opcode
    if (
        not opcode.name.startswith(("F", "LF", "STF", "PS"))
        and opcode not in {
            Opcode.MCRFS, Opcode.MFFS, Opcode.MTFSB0, Opcode.MTFSB1,
            Opcode.MTFSFI, Opcode.MTFSF,
        }
    )
) | SUPPORTED_FP_OPCODES


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
