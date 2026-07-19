"""Tests for retail asm listing helpers used in harness prompts."""
from __future__ import annotations

import unittest

from tools.llm_harness.asm_listings import (
    assembly_function_block,
    compact_type_snippets,
    format_instruction_listing,
    select_similar_siblings,
    strip_listing_bytecode_comments,
)


class TestAssemblyListings(unittest.TestCase):
    def test_assembly_function_block(self) -> None:
        source = (
            '.fn "foo__Fv", global\n'
            "/* 80000000 00000000 */ lwz r3, 0(r0)\n"
            '.endfn "foo__Fv"\n'
        )
        block = assembly_function_block(source, "foo__Fv")
        self.assertIn("lwz r3", block)
        self.assertIn('.endfn "foo__Fv"', block)

    def test_strip_listing_bytecode_comments(self) -> None:
        raw = (
            ".fn getInstance__5CGameFv, global\n"
            "/* 800396C0 00002900  80 6D A4 80 */	lwz r3, spInstance__5CGame@sda21(r0)\n"
            "/* 800396C4 00002904  4E 80 00 20 */	blr\n"
            ".endfn getInstance__5CGameFv\n"
        )
        cleaned = strip_listing_bytecode_comments(raw)
        self.assertEqual(
            cleaned,
            ".fn getInstance__5CGameFv, global\n"
            "lwz r3, spInstance__5CGame@sda21(r0)\n"
            "blr\n"
            ".endfn getInstance__5CGameFv\n",
        )
        self.assertNotIn("800396C0", cleaned)
        self.assertNotIn("80 6D A4 80", cleaned)

    def test_format_instruction_listing_from_dicts(self) -> None:
        listing = format_instruction_listing(
            [
                {
                    "address": "0x80000000",
                    "mnemonic": "lwz",
                    "operands": "r3, 0(r0)",
                    "relocation": {"symbol": "spInstance__5CGame"},
                },
                {
                    "address": "0x80000004",
                    "mnemonic": "blr",
                    "operands": "",
                    "relocation": None,
                },
            ]
        )
        self.assertIn("lwz r3, 0(r0)  ; spInstance__5CGame", listing)
        self.assertIn("blr", listing)
        self.assertNotIn("instruction_class", listing)

    def test_compact_type_snippets_prefers_method_and_static(self) -> None:
        header = """
class CGame : public CProc {
public:
    static CGame* getInstance();
    virtual void wkRender();
    static CGame* spInstance;
    void OnPauseTrigger(bool paused);
};
"""
        snippets = compact_type_snippets(
            header_text=header,
            source_text="",
            demangled="CGame::getInstance()",
            reloc_symbols=["spInstance__5CGame"],
            max_chars=1000,
        )
        joined = "\n".join(snippets)
        self.assertIn("getInstance()", joined)
        self.assertIn("spInstance", joined)
        self.assertNotIn("OnPauseTrigger", joined)
        self.assertNotIn("CTaskGame::getInstance()", joined)
        self.assertLess(len(joined), 400)

    def test_select_similar_siblings_filters_large_unrelated(self) -> None:
        siblings = [
            {
                "id": "a",
                "function": "CGame::wkRender",
                "status": "FULL_MATCH",
                "body": "void CGame::wkRender() {\n" + ("  doSomethingLong();\n" * 60) + "}",
            },
            {
                "id": "b",
                "function": "CGame::tiny",
                "status": "FULL_MATCH",
                "body": "void CGame::tiny() { return; }",
            },
            {
                "id": "c",
                "function": "Other::foo",
                "status": "FULL_MATCH",
                "body": "void Other::foo() { return; }",
            },
        ]
        selected = select_similar_siblings(
            siblings,
            demangled="CGame::getInstance()",
            max_bodies=2,
            max_chars_each=400,
        )
        self.assertEqual(len(selected), 1)
        self.assertEqual(selected[0]["id"], "b")


if __name__ == "__main__":
    unittest.main()
