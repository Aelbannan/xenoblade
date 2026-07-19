from __future__ import annotations
import json, sys, unittest
from pathlib import Path
from typing import Any, Dict, List, Optional

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.llm_harness.dossier import (
    TargetDossier, RetailProgramContext, SourceContext, CallContext,
    TypeContext, SymbolInventory, PromptConstraints, KnowledgeRecord,
    AttemptCluster, TargetIdentity, SignatureContext, SignatureParameter,
    SignatureReturn, ImplicitThis, DecodedInstruction, CFGBlock,
    ControlFlowGraph, CFGTerminator, DataFlowSummary, MemoryAccess,
    CallerSnippet, DeclarationEntry, SymbolMember, SymbolFunction,
    SymbolGlobal, SymbolEnum, build_target_identity, parse_signature,
    build_target_dossier, build_cfg, build_data_flow_summary,
    build_declaration_context, validate_dossier, dossier_to_dict,
    decode_instructions, _classify_instruction, _find_class_name,
    _format_operand, _split_params,
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

    def test_max_accepted_siblings_constraint(self):
        cons = PromptConstraints(max_accepted_siblings=2)
        self.assertEqual(cons.max_accepted_siblings, 2)

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


if __name__ == "__main__":
    raise SystemExit(unittest.main())
