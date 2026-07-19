from __future__ import annotations

from dataclasses import dataclass
from enum import Enum


# ELF32 PowerPC relocation types used by the Xenoblade retail/decomp objects.
R_PPC_ADDR32 = 1
R_PPC_ADDR16_LO = 4
R_PPC_ADDR16_HI = 5
R_PPC_ADDR16_HA = 6
R_PPC_REL24 = 10
R_PPC_REL14 = 11
R_PPC_EMB_SDA21 = 109

SUPPORTED_TEXT_RELOCATIONS = frozenset({
    R_PPC_ADDR16_LO,
    R_PPC_ADDR16_HI,
    R_PPC_ADDR16_HA,
    R_PPC_REL24,
    R_PPC_REL14,
    R_PPC_EMB_SDA21,
})

# Data-section types observed for jump tables / vtables (not yet applied to text decode).
KNOWN_DATA_RELOCATION_TYPES = frozenset({
    R_PPC_ADDR32,
})


@dataclass(frozen=True, slots=True)
class RelocationRef:
    """Solver-independent logical relocation attached to one instruction field.

    ``symbol`` is the object-file spelling retained for diagnostics;
    ``canonical_symbol`` is the identity shared by the two implementations.
    Relocation expressions are interpreted by semantics rather than folded into
    placeholder instruction bytes by the decoder.
    """

    offset: int
    relocation_type: int
    symbol: str
    canonical_symbol: str
    addend: int


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
    MFMSR = "mfmsr"
    MTMSR = "mtmsr"
    MFSR = "mfsr"
    MTSR = "mtsr"
    MFTB = "mftb"

    # Cache, ordering, and system control.
    DCBF = "dcbf"
    DCBI = "dcbi"
    DCBST = "dcbst"
    DCBT = "dcbt"
    DCBZ = "dcbz"
    DCBZ_L = "dcbz_l"
    ICBI = "icbi"
    SYNC = "sync"
    ISYNC = "isync"
    TWI = "twi"
    SC = "sc"
    RFI = "rfi"

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
# ConcreteOps and SymbolicOps. Compare/scalar invalid causes, divide-by-zero,
# conversion exceptions, and enabled-result suppression are modeled. Arithmetic
# OX/UX/XX and architectural trap delivery remain outside (see README.md).
SUPPORTED_FP_OPCODES = frozenset({
    Opcode.LFS, Opcode.LFSU, Opcode.LFD, Opcode.LFDU,
    Opcode.STFS, Opcode.STFSU, Opcode.STFD, Opcode.STFDU,
    Opcode.LFSX, Opcode.LFSUX, Opcode.LFDX, Opcode.LFDUX,
    Opcode.STFSX, Opcode.STFSUX, Opcode.STFDX, Opcode.STFDUX, Opcode.STFIWX,
    Opcode.FADDS, Opcode.FSUBS, Opcode.FMULS, Opcode.FDIVS,
    Opcode.FRES, Opcode.FRSQRTE,
    Opcode.FMADDS, Opcode.FMSUBS, Opcode.FNMADDS, Opcode.FNMSUBS,
    Opcode.FMADD, Opcode.FMSUB, Opcode.FNMADD, Opcode.FNMSUB,
    Opcode.FADD, Opcode.FSUB, Opcode.FMUL, Opcode.FDIV,
    Opcode.FSEL, Opcode.FCMPU, Opcode.FCMPO, Opcode.FRSP,
    Opcode.FCTIW, Opcode.FCTIWZ,
    Opcode.FNEG, Opcode.FMR, Opcode.FNABS, Opcode.FABS,
    Opcode.MFFS, Opcode.MTFSF, Opcode.MTFSFI,
    Opcode.MTFSB0, Opcode.MTFSB1, Opcode.MCRFS,
    Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU,
    Opcode.PSQ_LX, Opcode.PSQ_LUX, Opcode.PSQ_STX, Opcode.PSQ_STUX,
    Opcode.PS_NEG, Opcode.PS_MR, Opcode.PS_NABS, Opcode.PS_ABS,
    Opcode.PS_MERGE00, Opcode.PS_MERGE01, Opcode.PS_MERGE10, Opcode.PS_MERGE11,
    Opcode.PS_CMPU0, Opcode.PS_CMPO0, Opcode.PS_CMPU1, Opcode.PS_CMPO1,
    Opcode.PS_ADD, Opcode.PS_SUB, Opcode.PS_MUL, Opcode.PS_MULS0, Opcode.PS_MULS1,
    Opcode.PS_MADD, Opcode.PS_MSUB, Opcode.PS_NMADD, Opcode.PS_NMSUB,
    Opcode.PS_MADDS0, Opcode.PS_MADDS1,
    Opcode.PS_SUM0, Opcode.PS_SUM1, Opcode.PS_SEL,
    Opcode.PS_DIV, Opcode.PS_RES, Opcode.PS_RSQRTE,
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
    relocation: RelocationRef | None = None

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
            "relocation": (
                {
                    "offset": self.relocation.offset,
                    "type": self.relocation.relocation_type,
                    "symbol": self.relocation.symbol,
                    "canonical_symbol": self.relocation.canonical_symbol,
                    "addend": self.relocation.addend,
                }
                if self.relocation is not None else None
            ),
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
