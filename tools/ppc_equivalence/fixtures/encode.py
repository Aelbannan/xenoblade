from __future__ import annotations


def parse_int(value: object) -> int:
    if isinstance(value, str):
        return int(value, 0)
    return int(value)


def pack_xer(*, ca: int = 0, ov: int = 0, so: int = 0) -> int:
    return ((so & 1) << 31) | ((ov & 1) << 30) | ((ca & 1) << 29)


def unpack_xer(value: int) -> dict[str, int]:
    value &= 0xFFFFFFFF
    return {"so": (value >> 31) & 1, "ov": (value >> 30) & 1, "ca": (value >> 29) & 1}


def xo(opcd: int, rt: int, ra: int, rb: int, xo_bits: int, *, rc: int = 0, oe: int = 0) -> int:
    """Encode an X/XO-form word. `rt` is the bits-6..10 field (rD or rS)."""
    return (
        ((opcd & 0x3F) << 26)
        | ((rt & 0x1F) << 21)
        | ((ra & 0x1F) << 16)
        | ((rb & 0x1F) << 11)
        | ((oe & 1) << 10)
        | ((xo_bits & 0x3FF) << 1)
        | (rc & 1)
    )


def x_logical(ra: int, rs: int, rb: int, xo_bits: int, *, rc: int = 0) -> int:
    """Encode X-form logical/shift ops that write `ra` from `rs`/`rb`."""
    return xo(31, rs, ra, rb, xo_bits, rc=rc)


def dform(opcd: int, rd: int, ra: int, imm: int) -> int:
    return ((opcd & 0x3F) << 26) | ((rd & 0x1F) << 21) | ((ra & 0x1F) << 16) | (imm & 0xFFFF)


def mform(opcd: int, ra: int, rs: int, sh: int, mb: int, me: int, *, rc: int = 0) -> int:
    return (
        ((opcd & 0x3F) << 26)
        | ((rs & 0x1F) << 21)
        | ((ra & 0x1F) << 16)
        | ((sh & 0x1F) << 11)
        | ((mb & 0x1F) << 6)
        | ((me & 0x1F) << 1)
        | (rc & 1)
    )


def xl(opcd: int, bt: int, ba: int, bb: int, xo_bits: int, *, lk: int = 0) -> int:
    return (
        ((opcd & 0x3F) << 26)
        | ((bt & 0x1F) << 21)
        | ((ba & 0x1F) << 16)
        | ((bb & 0x1F) << 11)
        | ((xo_bits & 0x3FF) << 1)
        | (lk & 1)
    )


def bc(bo: int, bi: int, bd_words: int, *, aa: int = 0, lk: int = 0) -> int:
    """Encode `bc` with a signed word displacement from the current PC."""
    return (
        (16 << 26)
        | ((bo & 0x1F) << 21)
        | ((bi & 0x1F) << 16)
        | ((bd_words & 0x3FFF) << 2)
        | ((aa & 1) << 1)
        | (lk & 1)
    )


def words_to_hex(words: list[int]) -> str:
    return " ".join(f"{word & 0xFFFFFFFF:08x}" for word in words)


BLR = 0x4E800020
