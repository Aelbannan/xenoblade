from __future__ import annotations
import json, sys, unittest
from pathlib import Path
from typing import Any, Dict, List, Optional

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.llm_harness.dossier import (
    TargetDossier, RetailProgramContext, SourceContext, CallContext,
    TypeContext, SymbolInventory, PromptConstraints,
    AttemptCluster, TargetIdentity, SignatureContext, SignatureParameter,
    SignatureReturn, ImplicitThis, DecodedInstruction, CFGBlock,
    ControlFlowGraph, CFGTerminator, DataFlowSummary, MemoryAccess,
    CallerSnippet, DeclarationEntry, SymbolMember, SymbolFunction,
    SymbolGlobal, SymbolEnum, build_target_identity, parse_signature,
    build_target_dossier, build_cfg, build_data_flow_summary,
    build_declaration_context, validate_dossier, dossier_to_dict,
    decode_instructions, _classify_instruction, _find_class_name,
    _format_operand, _split_params, demangled_has_explicit_return,
    compact_model_facing_dossier,
)
from tools.llm_harness.structural import compare_structural
from tools.ppc_equivalence.elf_symbols import FunctionBytes, FunctionRelocation
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.ir import Instruction, Opcode, R_PPC_REL24


SIMPLE_FN_CODE = bytes.fromhex(
    '9421fff0' '7c0802a6' '93e1000c' '90010014'
    '48000001' '80010014' '83e1000c' '7c0803a6'
    '38210010' '4e800020'
)

SIMPLE_FN = FunctionBytes(name='test', path=Path('.'), code=SIMPLE_FN_CODE,
    base=0x80000000, value=0x80000000, size=40,
    section_index=0, section_name='.text', symbol_type=0,
)

BRANCH_FN_CODE = bytes.fromhex(
    '2f800000'
    '409e0008'
    '38600001'
    '4e800020'
    '38600000'
    '4e800020'
)

BRANCH_FN = FunctionBytes(name='branch_test', path=Path('.'),
    code=BRANCH_FN_CODE,
    base=0x80000000, value=0x80000000, size=24,
    section_index=0, section_name='.text', symbol_type=0,
)


def _make_reloc_fn() -> FunctionBytes:
    import struct
    bl_insn = struct.pack('>I', 0x48000001)
    code = bytes.fromhex('9421fff0') + bl_insn + bytes.fromhex('4e800020')
    reloc = FunctionRelocation(
        offset=4, relocation_type=R_PPC_REL24, symbol="targetFunc", addend=0,
    )
    return FunctionBytes(name='reloc_test', path=Path('.'), code=code,
        base=0x80000000, value=0x80000000, size=12,
        section_index=0, section_name='.text', symbol_type=0,
        relocations=(reloc,),
    )


class TestDecodedInstructions(unittest.TestCase):

    def test_instructions_present(self):
        decoded = decode_instructions(SIMPLE_FN)
        self.assertTrue(len(decoded) > 0)

    def test_raw_word_matches_address(self):
        decoded = decode_instructions(SIMPLE_FN)
        for di in decoded:
            self.assertTrue(di.address.startswith("0x"))
            self.assertIsInstance(di.raw_word, str)
            self.assertEqual(len(di.raw_word), 8)

    def test_branch_target_in_decoded(self):
        decoded = decode_instructions(BRANCH_FN)
        targets = [di for di in decoded if di.branch_target is not None]
        self.assertTrue(len(targets) > 0)

    def test_relocation_attached(self):
        fn = _make_reloc_fn()
        decoded = decode_instructions(fn)
        relocated = [di for di in decoded if di.relocation is not None]
        self.assertTrue(len(relocated) >= 1)
        r = relocated[0].relocation
        self.assertIn("symbol", r)
        self.assertEqual(r["symbol"], "targetFunc")


class TestSignatureParsing(unittest.TestCase):

    def test_exact_signature(self):
        sig = parse_signature("void foo(int x)")
        self.assertTrue(sig.declaration.endswith("foo(int x)"))

    def test_signature_authoritative(self):
        sig = parse_signature("Class::method(int)")
        self.assertTrue(sig.authoritative)


class TestCallersAndSiblings(unittest.TestCase):

    def test_max_callers_constraint(self):
        cons = PromptConstraints(max_callers=5)
        self.assertEqual(cons.max_callers, 5)

    def test_from_prompt_config(self):
        cons = PromptConstraints.from_prompt_config({
            "max_decoded_instructions": 9,
            "max_sibling_bodies": 2,
        })
        self.assertEqual(cons.max_decoded_instructions, 9)
        self.assertEqual(cons.max_accepted_siblings, 2)
        defaults = PromptConstraints.from_prompt_config({})
        self.assertEqual(defaults.max_decoded_instructions, 400)
        self.assertEqual(defaults.max_declaration_chars, 12000)

    def test_accepted_examples_included(self):
        dossier = TargetDossier(
            accepted_examples=[{"source": "int x = 1;"}],
        )
        d = dossier_to_dict(dossier)
        self.assertEqual(len(d["accepted_examples"]), 1)


class TestTypeContext(unittest.TestCase):

    def test_relevant_declarations_included(self):
        source = "struct Foo { int a; int b; };\n"
        sig = "Foo::method()"
        ctx = build_declaration_context(source, "", sig, [])
        self.assertTrue(len(ctx.declarations) >= 1)

    def test_unrelated_large_declarations_excluded(self):
        source = "struct Foo { int data; };\n"
        sig = "Foo::method()"
        ctx = build_declaration_context(source, "", sig, [], max_chars=0)
        self.assertEqual(len(ctx.declarations), 0)


class TestSymbolInventory(unittest.TestCase):

    def test_symbol_inventory_deterministic(self):
        fn = SIMPLE_FN
        d1 = build_target_dossier(
            "test", "_Z3foov", "foo()",
            "src/a.cpp", "src/a.o", "src/a.cpp",
            0x80000000, 40, fn, "", "", [],
        )
        d2 = build_target_dossier(
            "test", "_Z3foov", "foo()",
            "src/a.cpp", "src/a.o", "src/a.cpp",
            0x80000000, 40, fn, "", "", [],
        )
        self.assertEqual(
            dossier_to_dict(d1)["symbols"],
            dossier_to_dict(d2)["symbols"],
        )

    def test_missing_retail_returns_error(self):
        d = TargetDossier(signature=SignatureContext(declaration="void f()"))
        errs = validate_dossier(d)
        self.assertTrue(any("retail" in e for e in errs))


class TestDossierStability(unittest.TestCase):

    def test_dossier_json_stable(self):
        fn = SIMPLE_FN
        d = build_target_dossier(
            "test", "_Z3foov", "foo()",
            "src/a.cpp", "src/a.o", "src/a.cpp",
            0x80000000, 40, fn, "", "", [],
        )
        d1 = dossier_to_dict(d)
        d2 = dossier_to_dict(d)
        self.assertEqual(d1, d2)

    def test_dossier_structure_contains_sections(self):
        fn = SIMPLE_FN
        d = build_target_dossier(
            "test", "_Z3foov", "foo()",
            "src/a.cpp", "src/a.o", "src/a.cpp",
            0x80000000, 40, fn, "", "", [],
        )
        dd = dossier_to_dict(d)
        for key in ("target", "signature", "retail", "source", "calls", "types", "symbols"):
            self.assertIn(key, dd)

    def test_truncation_explicitly_marked(self):
        cons = PromptConstraints(max_decoded_instructions=2)
        fn = SIMPLE_FN
        d = build_target_dossier(
            "test", "_Z3foov", "foo()",
            "src/a.cpp", "src/a.o", "src/a.cpp",
            0x80000000, 40, fn, "", "", [], constraints=cons,
        )
        self.assertTrue(d.retail.truncated)


class TestCompactModelFacingDossier(unittest.TestCase):

    def test_explicit_return_detection(self) -> None:
        self.assertFalse(demangled_has_explicit_return("CGame::wkRender"))
        self.assertFalse(demangled_has_explicit_return("func_8006BA80"))
        self.assertTrue(demangled_has_explicit_return("int func(int a)"))
        self.assertTrue(demangled_has_explicit_return("void Class::method()"))
        self.assertTrue(demangled_has_explicit_return("float ns::Class::method(int)"))

    def test_keeps_distinct_mangled_demangled(self) -> None:
        compacted = compact_model_facing_dossier({
            "target": {
                "target_id": "t1",
                "mangled_name": "foo__6CClassFv",
                "demangled_name": "CClass::foo(void)",
                "source_file": "src/a.cpp",
                "object_file": "build/a.o",
                "translation_unit": "a",
                "retail_address": "0x00000010",
                "retail_size": 32,
            },
            "signature": {
                "declaration": "CClass::foo(void)",
                "authoritative": True,
                "implicit_this": {"type": "CClass*", "register": "r3"},
                "parameters": [],
                "return_info": {"type": "void", "register": ""},
            },
            "retail": {"base": "0x00000010", "size": 32, "truncated": False},
            "source": {
                "kind": "detected-function-definition",
                "begin_marker": "// LLM-HARNESS-BEGIN: t1",
                "end_marker": "// LLM-HARNESS-END: t1",
                "source_path": "src/a.cpp",
                "current_source": "",
            },
            "types": {"snippets": [], "total_chars": 0},
            "schema_version": 3,
            "repository": "xenoblade-wii-us",
            "workflow": "new",
            "retail_asm": "blr",
        })
        self.assertEqual(compacted["target"]["mangled_name"], "foo__6CClassFv")
        self.assertEqual(compacted["target"]["demangled_name"], "CClass::foo(void)")
        self.assertNotIn("object_file", compacted["target"])
        self.assertNotIn("retail_address", compacted["target"])
        self.assertNotIn("retail_size", compacted["target"])
        self.assertEqual(
            compacted["retail"],
            {"base": "0x00000010", "size": 32, "truncated": False},
        )
        self.assertNotIn("parameters", compacted["signature"])
        self.assertNotIn("return_info", compacted["signature"])
        self.assertNotIn("authoritative", compacted["signature"])
        self.assertIn("implicit_this", compacted["signature"])
        self.assertNotIn("source", compacted)
        self.assertNotIn("types", compacted)
        self.assertNotIn("schema_version", compacted)

    def test_drops_duplicate_name_when_identical(self) -> None:
        compacted = compact_model_facing_dossier({
            "target": {
                "target_id": "t2",
                "mangled_name": "func_8006BA80",
                "demangled_name": "func_8006BA80",
                "source_file": "src/a.cpp",
            },
            "signature": {
                "declaration": "func_8006BA80",
                "authoritative": True,
                "implicit_this": None,
                "parameters": [],
                "return_info": {"type": "void", "register": ""},
            },
            "retail": {"base": "0x1", "size": 4, "truncated": False},
        })
        self.assertNotIn("mangled_name", compacted["target"])
        self.assertEqual(compacted["target"]["demangled_name"], "func_8006BA80")
        self.assertNotIn("return_info", compacted["signature"])
        self.assertNotIn("implicit_this", compacted["signature"])

    def test_keeps_recovered_return_and_parameters(self) -> None:
        compacted = compact_model_facing_dossier({
            "target": {
                "target_id": "t3",
                "mangled_name": "bar__Fi",
                "demangled_name": "int bar(int a)",
                "source_file": "src/a.cpp",
            },
            "signature": {
                "declaration": "int bar(int a)",
                "authoritative": True,
                "implicit_this": None,
                "parameters": [{"name": "a", "type": "int"}],
                "return_info": {"type": "int", "register": ""},
            },
            "types": {"snippets": ["struct S {};"], "total_chars": 12},
            "retail": {"base": "0x1", "size": 4, "truncated": True},
        })
        self.assertEqual(compacted["signature"]["return_info"], {"type": "int"})
        self.assertEqual(
            compacted["signature"]["parameters"],
            [{"name": "a", "type": "int"}],
        )
        self.assertEqual(compacted["types"], {"snippets": ["struct S {};"]})


if __name__ == "__main__":
    raise SystemExit(unittest.main())
