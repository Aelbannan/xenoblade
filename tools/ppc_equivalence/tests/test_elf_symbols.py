"""Tests for ELF32 BE function extraction and check-objects wiring."""

from __future__ import annotations

import io
import importlib.util
import json
import struct
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from types import SimpleNamespace

from tools.coop.run import _equivalence_args_with_default_contract
from tools.coop.lib.equivalence_check import prove_unit_symbol
from tools.ppc_equivalence import cli
from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.elf_symbols import (
    ElfSymbolError,
    extract_function,
    extract_function_pair,
    list_text_functions,
)
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import ExecutionInconclusive, UnsupportedInstruction
from tools.ppc_equivalence.result import ProofResult, ProofStatus


# addi r3,r3,4 ; blr
_EQ_LEFT = bytes.fromhex("38630004 4e800020")
# addi r3,r3,5 ; blr
_NEQ = bytes.fromhex("38630005 4e800020")
_FP_FUNCTION = bytes.fromhex("fce1102a 4e800020")

_HAS_Z3 = importlib.util.find_spec("z3") is not None


def build_reloc_elf(
    functions: dict[str, bytes],
    *,
    text_addr: int = 0,
) -> bytes:
    """Build a minimal big-endian ELF32 ET_REL with .text/.symtab/.strtab."""
    text = b"".join(functions.values())
    if len(text) % 4:
        raise ValueError("function bytes must be word-aligned in aggregate")

    shstr = b"\x00.text\x00.strtab\x00.shstrtab\x00.symtab\x00"
    strtab = bytearray(b"\x00")
    name_offsets: dict[str, int] = {}
    for name in functions:
        name_offsets[name] = len(strtab)
        strtab.extend(name.encode("ascii") + b"\x00")

    symtab = bytearray(b"\x00" * 16)
    cursor = 0
    for name, code in functions.items():
        st_info = (1 << 4) | 2  # STB_GLOBAL | STT_FUNC
        symtab.extend(
            struct.pack(
                ">IIIBBH",
                name_offsets[name],
                cursor,
                len(code),
                st_info,
                0,
                1,  # .text section index
            )
        )
        cursor += len(code)

    e_shnum = 5
    e_shentsize = 40
    e_shoff = 52
    payload_off = e_shoff + e_shnum * e_shentsize

    text_off = payload_off
    strtab_off = text_off + len(text)
    shstr_off = strtab_off + len(strtab)
    symtab_off = shstr_off + len(shstr)

    def shdr(
        name_off: int,
        sh_type: int,
        flags: int,
        addr: int,
        offset: int,
        size: int,
        link: int = 0,
        info: int = 0,
        align: int = 4,
        entsize: int = 0,
    ) -> bytes:
        return struct.pack(
            ">IIIIIIIIII",
            name_off,
            sh_type,
            flags,
            addr,
            offset,
            size,
            link,
            info,
            align,
            entsize,
        )

    text_name = shstr.index(b".text\x00")
    strtab_name = shstr.index(b".strtab\x00")
    shstr_name = shstr.index(b".shstrtab\x00")
    symtab_name = shstr.index(b".symtab\x00")

    headers = (
        shdr(0, 0, 0, 0, 0, 0)
        + shdr(text_name, 1, 6, text_addr, text_off, len(text), align=4)
        + shdr(strtab_name, 3, 0, 0, strtab_off, len(strtab), align=1)
        + shdr(shstr_name, 3, 0, 0, shstr_off, len(shstr), align=1)
        + shdr(symtab_name, 2, 0, 0, symtab_off, len(symtab), link=2, info=1, align=4, entsize=16)
    )

    ident = bytearray(16)
    ident[0:4] = b"\x7fELF"
    ident[4] = 1
    ident[5] = 2
    ident[6] = 1

    header = bytes(ident) + struct.pack(
        ">HHIIIIIHHHHHH",
        1,
        20,
        1,
        0,
        0,
        e_shoff,
        0,
        52,
        0,
        0,
        e_shentsize,
        e_shnum,
        3,
    )
    assert len(header) == 52
    return header + headers + text + bytes(strtab) + shstr + bytes(symtab)


def _check_extracted(left_code: bytes, right_code: bytes, *, base: int = 0):
    original = decode_block(left_code, base, validate_with_capstone=False)
    candidate = decode_block(right_code, base, validate_with_capstone=False)
    contract = make_contract(preset=None, observe=["r3"], timeout_ms=5_000)
    try:
        return check_equivalence(
            original,
            candidate,
            contract,
            original_hex=left_code.hex(),
            candidate_hex=right_code.hex(),
        )
    except (UnsupportedInstruction, ExecutionInconclusive) as exc:
        return ProofResult(
            status=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            contract=contract.name,
            observables=[item.name for item in contract.observables],
            unsupported=[str(exc)],
        )


class ElfBuilderTests(unittest.TestCase):
    def test_extract_named_function(self) -> None:
        elf = build_reloc_elf({"foo": _EQ_LEFT, "bar": _NEQ})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.o"
            path.write_bytes(elf)
            foo = extract_function(path, "foo")
            self.assertEqual(foo.code, _EQ_LEFT)
            self.assertEqual(foo.size, len(_EQ_LEFT))
            self.assertEqual(foo.base, 0)
            self.assertEqual(foo.value, 0)
            bar = extract_function(path, "bar")
            self.assertEqual(bar.code, _NEQ)
            self.assertEqual(bar.value, len(_EQ_LEFT))
            self.assertEqual(bar.base, len(_EQ_LEFT))
            names = [item.name for item in list_text_functions(path)]
            self.assertEqual(names, ["foo", "bar"])

    def test_text_addr_shifts_base(self) -> None:
        elf = build_reloc_elf({"f": _EQ_LEFT}, text_addr=0x80001000)
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.o"
            path.write_bytes(elf)
            fn = extract_function(path, "f")
            self.assertEqual(fn.base, 0x80001000)

    def test_substring_and_ambiguity(self) -> None:
        elf = build_reloc_elf(
            {
                "add__3FooFv": _EQ_LEFT,
                "add__3BarFv": _NEQ,
                "unique_token_xyz": _EQ_LEFT,
            }
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.o"
            path.write_bytes(elf)
            found = extract_function(path, "unique_token")
            self.assertEqual(found.name, "unique_token_xyz")
            with self.assertRaises(ElfSymbolError):
                extract_function(path, "add__")
            with self.assertRaises(ElfSymbolError):
                extract_function(path, "missing")

    def test_rejects_non_elf(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.o"
            path.write_bytes(b"not elf")
            with self.assertRaises(ElfSymbolError):
                list_text_functions(path)


class CheckObjectsApiTests(unittest.TestCase):
    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_equivalent_objects_under_r3(self) -> None:
        left = build_reloc_elf({"f": _EQ_LEFT})
        right = build_reloc_elf({"f": _EQ_LEFT})
        with tempfile.TemporaryDirectory() as tmp:
            a = Path(tmp) / "a.o"
            b = Path(tmp) / "b.o"
            a.write_bytes(left)
            b.write_bytes(right)
            pair = extract_function_pair(a, b, "f")
            result = _check_extracted(pair[0].code, pair[1].code, base=pair[0].base)
            self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_check_objects_defaults_to_auditable_auto_contract(self) -> None:
        for code_bytes, expected_added in (
            (_EQ_LEFT, []),
            (_FP_FUNCTION, ["fpscr"]),
        ):
            with self.subTest(expected_added=expected_added), tempfile.TemporaryDirectory() as tmp:
                left = Path(tmp) / "a.o"
                right = Path(tmp) / "b.o"
                left.write_bytes(build_reloc_elf({"f": code_bytes}))
                right.write_bytes(build_reloc_elf({"f": code_bytes}))
                output = io.StringIO()
                with redirect_stdout(output):
                    exit_code = cli.main([
                        "check-objects",
                        "--original", str(left),
                        "--candidate", str(right),
                        "--symbol", "f",
                        "--json",
                    ])
                self.assertEqual(exit_code, 0)
                payload = json.loads(output.getvalue())
                self.assertEqual(payload["format"], 3)
                self.assertEqual(payload["contract"], "auto")
                self.assertEqual(payload["contract_resolution"]["base"], "ppc-eabi")
                self.assertEqual(payload["contract_resolution"]["added"], expected_added)
                self.assertIn("memory", payload["observables"])

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_inequivalent_objects(self) -> None:
        left = build_reloc_elf({"f": _EQ_LEFT})
        right = build_reloc_elf({"f": _NEQ})
        with tempfile.TemporaryDirectory() as tmp:
            a = Path(tmp) / "a.o"
            b = Path(tmp) / "b.o"
            a.write_bytes(left)
            b.write_bytes(right)
            pair = extract_function_pair(a, b, "f")
            result = _check_extracted(pair[0].code, pair[1].code)
            self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_candidate_symbol_alias(self) -> None:
        left = build_reloc_elf({"retail_f": _EQ_LEFT})
        right = build_reloc_elf({"decomp_f": _EQ_LEFT})
        with tempfile.TemporaryDirectory() as tmp:
            a = Path(tmp) / "a.o"
            b = Path(tmp) / "b.o"
            a.write_bytes(left)
            b.write_bytes(right)
            pair = extract_function_pair(a, b, "retail_f", candidate_symbol="decomp_f")
            self.assertEqual(pair[0].code, pair[1].code)
            result = _check_extracted(pair[0].code, pair[1].code)
            self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_extract_cli_list_and_out(self) -> None:
        elf = build_reloc_elf({"alpha": _EQ_LEFT, "beta": _NEQ})
        with tempfile.TemporaryDirectory() as tmp:
            obj = Path(tmp) / "t.o"
            out = Path(tmp) / "alpha.bin"
            obj.write_bytes(elf)
            self.assertEqual(cli.main(["extract", "--object", str(obj), "--list"]), 0)
            self.assertEqual(
                cli.main(["extract", "--object", str(obj), "--symbol", "alpha", "--out", str(out)]),
                0,
            )
            self.assertEqual(out.read_bytes(), _EQ_LEFT)

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_check_objects_cli(self) -> None:
        left = build_reloc_elf({"f": _EQ_LEFT})
        right = build_reloc_elf({"f": _NEQ})
        with tempfile.TemporaryDirectory() as tmp:
            a = Path(tmp) / "a.o"
            b = Path(tmp) / "b.o"
            a.write_bytes(left)
            b.write_bytes(right)
            code = cli.main(
                [
                    "check-objects",
                    "--original",
                    str(a),
                    "--candidate",
                    str(b),
                    "--symbol",
                    "f",
                    "--observe",
                    "r3",
                ]
            )
            self.assertEqual(code, 1)


class CoopDefaultContractTests(unittest.TestCase):
    def test_check_objects_defaults_to_auto(self) -> None:
        args = _equivalence_args_with_default_contract(
            ["check-objects", "--original", "a.o", "--candidate", "b.o", "--symbol", "f"]
        )
        self.assertEqual(args[1:3], ["--contract", "auto"])
        self.assertEqual(
            _equivalence_args_with_default_contract(["check-unit", "unit", "--symbol", "f"]),
            ["check-unit", "unit", "--symbol", "f"],
        )

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_automatic_equivalent_match_probe_uses_auto(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            retail = Path(tmp) / "retail.o"
            decomp = Path(tmp) / "decomp.o"
            retail.write_bytes(build_reloc_elf({"f": _FP_FUNCTION}))
            decomp.write_bytes(build_reloc_elf({"f": _FP_FUNCTION}))
            unit = SimpleNamespace(target_path=retail, base_path=decomp)
            probe = prove_unit_symbol(None, unit, "f")  # type: ignore[arg-type]
            self.assertEqual(probe.status, ProofStatus.EQUIVALENT)
            self.assertEqual(probe.detail, "auto contract: ppc-eabi + fpscr")


class CoopCheckUnitTests(unittest.TestCase):
    def test_check_unit_forwards_to_check_objects(self) -> None:
        from tools.coop.lib.config import CoopConfig
        from tools.coop.lib.project import ObjdiffUnit
        from tools.coop import run as coop_run

        left = build_reloc_elf({"sym": _EQ_LEFT})
        right = build_reloc_elf({"sym": _EQ_LEFT})
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            retail = root / "retail.o"
            decomp = root / "decomp.o"
            retail.write_bytes(left)
            decomp.write_bytes(right)
            config = CoopConfig(project_root=root, region="us")
            unit = ObjdiffUnit(
                name="fake/unit",
                target_path=retail,
                base_path=decomp,
                source_path=None,
            )

            class FakeProject:
                def __init__(self) -> None:
                    self.root = root
                    self.config = config

                def resolve_unit(self, hint: str) -> ObjdiffUnit:
                    return unit

                def ninja_build(self, target: str) -> None:
                    raise AssertionError("ninja_build should not run with --no-build")

            captured: list[list[str]] = []

            def fake_cmd_equivalence(project, cfg, args):
                captured.append(list(args))
                return 0

            original = coop_run.cmd_equivalence
            original_report = coop_run.report_unit
            try:
                coop_run.cmd_equivalence = fake_cmd_equivalence  # type: ignore[assignment]
                coop_run.report_unit = lambda *_a, **_k: (_ for _ in ()).throw(  # type: ignore[assignment]
                    FileNotFoundError("skip report")
                )
                code = coop_run._cmd_equivalence_check_unit(
                    FakeProject(),  # type: ignore[arg-type]
                    config,
                    ["fake/unit", "--symbol", "sym", "--no-build", "--observe", "r3"],
                )
            finally:
                coop_run.cmd_equivalence = original  # type: ignore[assignment]
                coop_run.report_unit = original_report  # type: ignore[assignment]

            self.assertEqual(code, 0)
            self.assertEqual(len(captured), 1)
            self.assertEqual(captured[0][0], "check-objects")
            self.assertIn(str(retail), captured[0])
            self.assertIn(str(decomp), captured[0])
            self.assertIn("--symbol", captured[0])
            self.assertIn("sym", captured[0])
            self.assertIn("--observe", captured[0])
            self.assertIn("r3", captured[0])


@unittest.skipUnless(
    Path("build/us/obj/kyoshin/CGame.o").is_file() and Path("build/us/src/kyoshin/CGame.o").is_file(),
    "requires built CGame retail/decomp objects",
)
class RealObjectSmokeTests(unittest.TestCase):
    def test_extract_from_cgames_objects(self) -> None:
        retail = Path("build/us/obj/kyoshin/CGame.o")
        decomp = Path("build/us/src/kyoshin/CGame.o")
        functions = list_text_functions(retail)
        self.assertGreater(len(functions), 0)
        sample = functions[0]
        left = extract_function(retail, sample.name)
        right = extract_function(decomp, sample.name)
        self.assertEqual(left.size % 4, 0)
        self.assertEqual(right.size % 4, 0)
        self.assertEqual(len(left.code), left.size)
        self.assertEqual(left.name, right.name)
        if _HAS_Z3 and left.code == right.code:
            result = _check_extracted(left.code, right.code, base=left.base)
            self.assertIn(
                result.status,
                {
                    ProofStatus.EQUIVALENT,
                    ProofStatus.INCONCLUSIVE_UNSUPPORTED,
                    ProofStatus.INCONCLUSIVE_TIMEOUT,
                    ProofStatus.INCONCLUSIVE_UNKNOWN,
                },
            )


if __name__ == "__main__":
    unittest.main()
