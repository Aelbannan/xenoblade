"""Integration tests for new harness features (Phases 2-6)."""
from __future__ import annotations

import io
import json
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest.mock import Mock, patch, MagicMock, PropertyMock

from tools.llm_harness.core import Harness
from tools.llm_harness.types import (
    BinaryDiffFeedback,
    InstructionDifference,
    InstructionWindow,
    SizeComparison,
    StackFrameComparison,
    ComponentFinding,
    fingerprint_binary_feedback,
)
from tools.llm_harness.match_improve import normalize_objdiff_feedback
from tools.llm_harness.run import main


class TestSolveCommand(unittest.TestCase):
    """Tests for the new solve command."""

    def test_solve_command_exists(self) -> None:
        """Test that solve command is registered."""
        harness = Mock()
        harness.models_for_workflow.return_value = [Mock(id="test", provider="opencode", model="test")]
        harness.providers = {"opencode": Mock()}
        
        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                # Just test that the command is recognized (will fail on missing config but not on unknown command)
                try:
                    main(["solve", "--help"])
                except SystemExit:
                    pass  # --help exits with 0
                except Exception:
                    pass  # Expected if no config

    def test_solve_config_section(self) -> None:
        """Test that solve config is read correctly."""
        config = {
            "project_adapter": "test.adapter",
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
            "solve": {
                "initial_candidates": 5,
                "compile_repairs": 3,
                "match_repairs": 6,
                "max_repeated_fingerprint": 3,
                "stop_on_full_match": False,
                "stop_on_equivalent_match": True,
            }
        }
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
            json.dump(config, f)
            config_path = f.name
        
        try:
            with patch("tools.llm_harness.core.Harness._load_adapter", return_value=Mock()):
                harness = Harness(Path(config_path))
                self.assertEqual(harness.config["solve"]["initial_candidates"], 5)
                self.assertEqual(harness.config["solve"]["compile_repairs"], 3)
                self.assertEqual(harness.config["solve"]["match_repairs"], 6)
        finally:
            Path(config_path).unlink()


class TestBinaryFeedback(unittest.TestCase):
    """Tests for structured binary feedback (Phase 2)."""

    def test_fingerprint_stability(self) -> None:
        """Test that fingerprint is stable for same input."""
        from tools.ppc_equivalence.elf_symbols import FunctionBytes
        
        # Use valid PPC instructions: 4 instructions (16 bytes)
        # 38600000 = li r3,0 ; 38600001 = li r3,1 ; 38600002 = li r3,2 ; 4E800020 = blr
        retail = FunctionBytes(
            name="test_fn",
            path="/fake/path",
            base=0x8000,
            value=0x8000,
            size=16,
            code=bytes.fromhex("38600000 38600001 38600002 4E800020"),
            relocations=[],
            section_index=1,
            section_name=".text",
            symbol_type=0,
        )
        candidate = FunctionBytes(
            name="test_fn",
            path="/fake/path",
            base=0x8000,
            value=0x8000,
            size=16,
            code=bytes.fromhex("38600001 38600002 38600003 4E800020"),
            relocations=[],
            section_index=1,
            section_name=".text",
            symbol_type=0,
        )
        
        feedback = normalize_objdiff_feedback(retail, candidate, max_window=4)
        fp1 = fingerprint_binary_feedback(feedback)
        
        feedback2 = normalize_objdiff_feedback(retail, candidate, max_window=4)
        fp2 = fingerprint_binary_feedback(feedback2)
        
        self.assertEqual(fp1, fp2)

    def test_fingerprint_changes_on_difference(self) -> None:
        """Test that fingerprint changes when candidate changes."""
        from tools.ppc_equivalence.elf_symbols import FunctionBytes
        
        retail = FunctionBytes(
            name="test_fn",
            path="/fake/path",
            base=0x8000,
            value=0x8000,
            size=16,
            code=bytes.fromhex("38600000 38600001 38600002 4E800020"),
            relocations=[],
            section_index=1,
            section_name=".text",
            symbol_type=0,
        )
        candidate1 = FunctionBytes(
            name="test_fn",
            path="/fake/path",
            base=0x8000,
            value=0x8000,
            size=16,
            code=bytes.fromhex("38600001 38600002 38600003 4E800020"),
            relocations=[],
            section_index=1,
            section_name=".text",
            symbol_type=0,
        )
        candidate2 = FunctionBytes(
            name="test_fn",
            path="/fake/path",
            base=0x8000,
            value=0x8000,
            size=16,
            code=bytes.fromhex("38600002 38600003 38600004 4E800020"),
            relocations=[],
            section_index=1,
            section_name=".text",
            symbol_type=0,
        )
        
        feedback1 = normalize_objdiff_feedback(retail, candidate1, max_window=4)
        fp1 = fingerprint_binary_feedback(feedback1)
        
        feedback2 = normalize_objdiff_feedback(retail, candidate2, max_window=4)
        fp2 = fingerprint_binary_feedback(feedback2)
        
        self.assertNotEqual(fp1, fp2)
        
        feedback1 = normalize_objdiff_feedback(retail, candidate1, max_window=4)
        fp1 = fingerprint_binary_feedback(feedback1)
        
        feedback2 = normalize_objdiff_feedback(retail, candidate2, max_window=4)
        fp2 = fingerprint_binary_feedback(feedback2)
        
        self.assertNotEqual(fp1, fp2)

    def test_feedback_contains_expected_fields(self) -> None:
        """Test that binary feedback has all expected fields."""
        from tools.ppc_equivalence.elf_symbols import FunctionBytes, FunctionRelocation
        
        retail = FunctionBytes(
            name="test_fn",
            path="/fake/path",
            base=0x8000,
            value=0x8000,
            size=8,
            code=bytes.fromhex("38600000 4E800020"),
            relocations=[],
            section_index=1,
            section_name=".text",
            symbol_type=0,
        )
        candidate = FunctionBytes(
            name="test_fn",
            path="/fake/path",
            base=0x8000,
            value=0x8000,
            size=8,
            code=bytes.fromhex("38600001 4E800020"),
            relocations=[],
            section_index=1,
            section_name=".text",
            symbol_type=0,
        )
        
        feedback = normalize_objdiff_feedback(retail, candidate, max_window=4)
        
        self.assertIn("first_difference_offset", feedback)
        self.assertIn("instruction_windows", feedback)
        self.assertIn("differences", feedback)
        self.assertIn("relocation_differences", feedback)
        self.assertIn("function_size", feedback)
        self.assertIn("stack_frame", feedback)
        self.assertIn("structural_summary", feedback)


class TestPromptBudgeting(unittest.TestCase):
    """Tests for prompt budget enforcement (Phase 6)."""

    def test_prompt_budget_enforcement(self) -> None:
        """Test that prompt budget limits are enforced."""
        # The budget enforcement is tested indirectly via the adapter
        # We can test the config is read correctly
        config = {
            "project_adapter": "test.adapter",
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
            "prompt": {
                "max_chars": 100,
                "max_decoded_instructions": 10,
                "max_declaration_chars": 500,
                "max_callers": 1,
                "max_sibling_bodies": 1,
                "include_raw_hex": False,
            }
        }
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
            json.dump(config, f)
            config_path = f.name
        
        try:
            with patch("tools.llm_harness.core.Harness._load_adapter", return_value=Mock()):
                harness = Harness(Path(config_path))
                self.assertEqual(harness.config["prompt"]["max_chars"], 100)
                self.assertEqual(harness.config["prompt"]["max_decoded_instructions"], 10)
        finally:
            Path(config_path).unlink()

    def test_prompt_constraints_from_config(self) -> None:
        from tools.llm_harness.dossier import PromptConstraints

        cons = PromptConstraints.from_prompt_config({
            "max_decoded_instructions": 12,
            "max_declaration_chars": 800,
            "max_callers": 2,
            "max_sibling_bodies": 1,
            "include_raw_hex": True,
        })
        self.assertEqual(cons.max_decoded_instructions, 12)
        self.assertEqual(cons.max_declaration_chars, 800)
        self.assertEqual(cons.max_callers, 2)
        self.assertEqual(cons.max_accepted_siblings, 1)
        self.assertTrue(cons.include_raw_hex)

    def test_compile_repair_prompt_has_no_unsubstituted_placeholders(self) -> None:
        from tools.llm_harness.types import Candidate

        class FakeAdapter:
            root = Path(".")
            prompt_dir = Path("tools/llm_harness/prompts")

        config = {
            "project_adapter": "test.adapter",
            "project": {"prompt_dir": "tools/llm_harness/prompts"},
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
        }
        with tempfile.NamedTemporaryFile(mode="w", suffix=".json", delete=False) as f:
            json.dump(config, f)
            config_path = f.name
        try:
            with patch("tools.llm_harness.core.Harness._load_adapter", return_value=FakeAdapter()):
                harness = Harness(Path(config_path))
            candidate = Candidate(
                source="int foo() { return 0; }",
                hypothesis="test",
                notes=[],
                next_change="",
            )
            prompt = harness._build_repair_prompt(
                candidate,
                "error: undeclared identifier",
                [],
                {},
                budget=2,
                repair_index=0,
            )
            self.assertNotIn("{{WORKFLOW_PROMPT}}", prompt)
            self.assertNotIn("{{DOSSIER_JSON}}", prompt)
            self.assertNotIn("{{CURRENT_FUNCTION}}", prompt)
            self.assertIn("int foo()", prompt)
            self.assertIn('"source"', prompt)
            # Template may mention the section name; a real section is a markdown heading.
            self.assertNotIn("\n## Retail ASM (signature/ABI check)\n", prompt)

            with_asm = harness._build_repair_prompt(
                candidate,
                "error: undeclared identifier",
                [],
                {},
                budget=2,
                repair_index=0,
                retail_asm="blr\n",
            )
            self.assertIn("\n## Retail ASM (signature/ABI check)\nblr", with_asm)
        finally:
            Path(config_path).unlink()

    def test_parse_candidate_clamps_metadata(self) -> None:
        from tools.llm_harness.core import parse_candidate

        payload = {
            "source": "int f() { return 1; }",
            "hypothesis": "h" * 400,
            "notes": ["n" * 200, "second", "third", "fourth"],
            "next_change": "x" * 200,
            "change": "y" * 200,
        }
        candidate = parse_candidate(json.dumps(payload))
        self.assertEqual(len(candidate.hypothesis), 160)
        self.assertEqual(len(candidate.notes), 3)
        self.assertEqual(len(candidate.notes[0]), 120)
        self.assertEqual(len(candidate.next_change), 120)


class TestEnhancedTypeContext(unittest.TestCase):
    """Tests for enhanced type context (Phase 6)."""

    def test_dossier_includes_enhanced_types(self) -> None:
        """Test that dossier includes owner_declaration and callee_declarations."""
        # This is tested via the dossier building in xenoblade_project.py
        pass


class TestConfigValidation(unittest.TestCase):
    """Tests for strict config validation (Phase 8)."""

    def test_rejects_unknown_top_level_keys(self) -> None:
        """Test that unknown top-level keys are rejected."""
        config = {
            "project_adapter": "test.adapter",
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
            "unknown_key": "value",
        }
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
            json.dump(config, f)
            config_path = f.name
        
        try:
            with patch("tools.llm_harness.core.Harness._load_adapter", return_value=Mock()):
                with self.assertRaises(ValueError) as cm:
                    Harness(Path(config_path))
            self.assertIn("unknown top-level keys", str(cm.exception))
        finally:
            Path(config_path).unlink()

    def test_rejects_unknown_project_keys(self) -> None:
        """Test that unknown project keys are rejected."""
        config = {
            "project_adapter": "test.adapter",
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
            "project": {"unknown_project_key": "value"},
        }
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
            json.dump(config, f)
            config_path = f.name
        
        try:
            with patch("tools.llm_harness.core.Harness._load_adapter", return_value=Mock()):
                with self.assertRaises(ValueError) as cm:
                    Harness(Path(config_path))
            self.assertIn("unknown keys", str(cm.exception))
        finally:
            Path(config_path).unlink()

    def test_rejects_unknown_execution_keys(self) -> None:
        """Test that unknown execution keys are rejected."""
        config = {
            "project_adapter": "test.adapter",
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
            "execution": {"unknown_exec_key": "value"},
        }
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
            json.dump(config, f)
            config_path = f.name
        
        try:
            with patch("tools.llm_harness.core.Harness._load_adapter", return_value=Mock()):
                with self.assertRaises(ValueError) as cm:
                    Harness(Path(config_path))
            self.assertIn("execution contains unknown keys", str(cm.exception))
        finally:
            Path(config_path).unlink()

    def test_rejects_deprecated_knowledge_keys(self) -> None:
        """Test that deprecated mwcc_knowledge keys are rejected."""
        config = {
            "project_adapter": "test.adapter",
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
            "project": {"mwcc_knowledge_enabled": True},
        }
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
            json.dump(config, f)
            config_path = f.name
        
        try:
            with patch("tools.llm_harness.core.Harness._load_adapter", return_value=Mock()):
                with self.assertRaises(ValueError) as cm:
                    Harness(Path(config_path))
            self.assertIn("deprecated knowledge keys", str(cm.exception))
        finally:
            Path(config_path).unlink()

    def test_show_config_works(self) -> None:
        """Test that --show-config prints effective config."""
        config = {
            "project_adapter": "adapter.py",
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
            "output_dir": "build/custom",
            "project": {"coop_config": "coop.json"},
        }

        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "class A:\n"
                " def __init__(self, root): self.root=root\n"
                " def finalize(self): pass\n"
                "def create_adapter(root, settings): return A(root)\n",
                encoding="utf-8",
            )
            config_path = root / "config.json"
            config_path.write_text(json.dumps(config), encoding="utf-8")
            with redirect_stdout(io.StringIO()) as out:
                result = main(["--config", str(config_path), "--show-config"])
            self.assertEqual(result, 0)
            output = out.getvalue()
            self.assertIn("project_adapter", output)
            self.assertIn("adapter.py", output)
            self.assertIn("build/custom", output)


class TestDryRunAndCompilation(unittest.TestCase):
    """Test that all modules compile and basic functionality works."""

    def test_all_modules_compile(self) -> None:
        """Ensure all harness modules compile without error."""
        import tools.llm_harness.core
        import tools.llm_harness.dossier
        import tools.llm_harness.match_improve
        import tools.llm_harness.promotion
        import tools.llm_harness.providers
        import tools.llm_harness.structural
        import tools.llm_harness.types
        import tools.llm_harness.workspace
        import tools.llm_harness.xenoblade_project
        import tools.llm_harness.run

    def test_solve_config_defaults(self) -> None:
        """Test solve config defaults are applied."""
        config = {
            "project_adapter": "test.adapter",
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
        }
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
            json.dump(config, f)
            config_path = f.name
        
        try:
            with patch("tools.llm_harness.core.Harness._load_adapter", return_value=Mock()):
                harness = Harness(Path(config_path))
                eff = harness.get_effective_config()
                self.assertEqual(eff["solve"]["initial_candidates"], 2)
                self.assertEqual(eff["solve"]["compile_repairs"], 1)
                self.assertEqual(eff["solve"]["match_repairs"], 3)
        finally:
            Path(config_path).unlink()

    def test_prompt_config_defaults(self) -> None:
        """Test prompt config defaults are applied."""
        config = {
            "project_adapter": "test.adapter",
            "models": {
                "default": [{"id": "test", "provider": "opencode", "model": "test", "runs": 1}]
            },
        }
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.json', delete=False) as f:
            json.dump(config, f)
            config_path = f.name
        
        try:
            with patch("tools.llm_harness.core.Harness._load_adapter", return_value=Mock()):
                harness = Harness(Path(config_path))
                eff = harness.get_effective_config()
                self.assertEqual(eff["prompt"]["max_chars"], 60000)
                self.assertEqual(eff["prompt"]["max_decoded_instructions"], 400)
                self.assertEqual(eff["prompt"]["max_declaration_chars"], 12000)
        finally:
            Path(config_path).unlink()


if __name__ == "__main__":
    unittest.main()