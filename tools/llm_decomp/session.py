"""Session loop — the conversational state machine.

Design reference: docs/llm_decomp_design.md §2 (lifecycle), §4 (tools),
§6 (verification), §9 (budgets).

One Session = one target (match) or one TU task (other session types).
The provider never touches the filesystem; all writes go through the
patch engine + lint gate, all acceptance through verify.py.
"""
from __future__ import annotations

import json
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

from . import paths, state, verify
from .brief import build_brief
from .config import Config
from .contracts import (Baseline, SessionMeta, SessionStatus, ToolCall,
                        ToolResult, Verdict, VerdictKind)
from .lint import lint_delta
from .patcher import FilePatch, SearchReplace, apply_patch
from .policies import SessionPolicy
from .providers.base import Provider
from .rename_tools import RENAME_TOOL_SCHEMAS, symbols_tool, rename_symbol_tool
from .tools import TOOL_SCHEMAS, ToolContext, dispatch
from .transcript import Transcript
from .providers.base import compute_cost

_SYSTEM_PROMPT = """\
You are a decompilation agent working on a Wii game (PowerPC, MWCC \
compiler). Your job: reconstruct high-level C/C++ whose compiled bytes \
match the retail binary. You have tools to read project files, apply \
patches, build, and diff. Follow the brief exactly. Acceptance is decided \
by the harness, never by you.

**Build early.** Call `build` on your first or second turn to see the \
current mismatch state. Analysis alone wastes turns — compile the \
baseline, read the diff, then patch and iterate.

**Patch after the first diff.** After the first `diff` call, apply \
your best-guess patch immediately. Don't read more files — iterate \
with patch → build → diff cycles. Analysis without editing burns turns."""


@dataclass
class SessionOutcome:
    accepted: bool
    reason: str
    session_id: str
    verdict_rule: str = ""


class Session:
    def __init__(self, repo_root: Path, cfg: Config, policy: SessionPolicy,
                 meta: SessionMeta, sdir: Path, provider: Provider, *,
                 demangled: str, signature: str, retail_asm: str,
                 target_id: str, carryover: str | None = None):
        self.repo_root = repo_root
        self.cfg = cfg
        self.policy = policy
        self.meta = meta
        self.sdir = sdir
        self.provider = provider
        self.demangled = demangled
        self.signature = signature
        self.retail_asm = retail_asm
        self.carryover = carryover

        self.budgets = cfg.budgets_for(policy.name)
        self.model = cfg.model_for(policy.name)
        self.baseline: Baseline | None = None
        self.transcript = Transcript(paths.transcript_path(sdir))

        # Counters
        self.turns = 0
        self.builds = 0
        self.agent_sweeps = 0
        self.patch_failures = 0  # consecutive anchoring failures
        self.compile_error_streak = 0
        self.tokens_in = 0
        self.cache_tokens_in = 0
        self.tokens_out = 0
        self.cost_total = 0.0
        self.cleanup_turns_left = 0
        self.best_mismatches: int | None = None
        self._built_this_session = False
        self._submitted = False
        self._submit_note = ""
        self._needs_cleanup_verify = False
        self._auto_verify = False  # auto-trigger verification after patch+build
        self._verdict: Verdict | None = None
        self._dirty = False  # files changed since last verification
        self._no_tool_turns = 0  # consecutive replies with no tool calls
        self._graph = None  # pinned IncludeGraph (v1.1)
        self._shared_headers: dict[str, tuple] = {}  # path -> (Tier, dependents)

    # ── tool wiring ──────────────────────────────────────────────────

    def _ctx(self) -> ToolContext:
        return ToolContext(
            repo_root=self.repo_root,
            writable=set(self.meta.writable),
            patch_fn=self._patch_fn if "patch" in self.policy.tools else None,
            build_fn=self._build_fn if "build" in self.policy.tools else None,
            diff_fn=self._diff_fn if "diff" in self.policy.tools else None,
            equivalence_fn=(self._equivalence_fn
                            if "equivalence" in self.policy.tools else None),
            symbols_fn=((lambda a: symbols_tool(self.repo_root, a))
                        if "symbols" in self.policy.tools else None),
            rename_symbol_fn=((lambda a: rename_symbol_tool(self.repo_root, a))
                              if "rename_symbol" in self.policy.tools else None),
        )

    def _init_snapshot_text(self, rel: str) -> str | None:
        snap = paths.init_snapshot_dir(self.sdir) / rel
        if snap.exists():
            return snap.read_text(encoding="utf-8")
        return None

    def _init_read_src(self) -> str | None:
        """Read the TU source file for the brief."""
        for w in self.meta.writable:
            if w.endswith(".cpp"):
                p = self.repo_root / w
                if p.is_file():
                    try:
                        return p.read_text(encoding="utf-8", errors="replace")
                    except Exception:
                        return None
        return None

    def _read_tu_header(self, unit: str) -> str | None:
        """Read the TU's header file content, if it exists."""
        for candidate in (f"include/{unit}.hpp", f"include/{unit}.h",
                          f"src/{unit}.hpp", f"src/{unit}.h"):
            p = self.repo_root / candidate
            if p.is_file():
                try:
                    return p.read_text(encoding="utf-8", errors="replace")
                except Exception:
                    return None
        return None

    def _patch_fn(self, files_raw: list[dict]) -> ToolResult:
        file_patches: list[FilePatch] = []
        for f in files_raw:
            blocks = [SearchReplace(search=b.get("search", ""),
                                    replace=b.get("replace", ""))
                      for b in f.get("blocks", [])]
            file_patches.append(FilePatch(
                path=str(f.get("path", "")), blocks=blocks,
                create=bool(f.get("create", False)),
                content=f.get("content")))

        # v1.1: headers outside the writable set need tier validation.
        from .cross_tu import Tier, dependent_units, tier_for
        allowed = set(self.meta.writable)
        new_shared: dict[str, tuple] = {}
        for fp in file_patches:
            norm = fp.path.replace("\\", "/").lstrip("./")
            if norm in allowed:
                continue
            if not (self.policy.scope.shared_headers and self._graph
                    and norm.startswith("include/")
                    and norm.endswith((".hpp", ".h"))
                    and ".." not in norm.split("/")):
                return ToolResult(
                    ok=False,
                    content=(f"**patch rejected:** `{fp.path}` is outside "
                             "this session's writable scope (shared-header "
                             "writes are limited to include/ headers)."))
            deps = set(self._graph.dependents(norm))
            deps -= {w for w in self.meta.writable
                     if w.endswith((".cpp", ".c"))}
            tier = tier_for(len(deps), self.cfg)
            if tier is Tier.REFUSE:
                return ToolResult(
                    ok=False,
                    content=(f"**patch rejected:** `{norm}` has "
                             f"{len(deps)} dependents (limit "
                             f"{self.cfg.cross_tu_refuse_over}). "
                             "Restructure: put the change in a TU-local "
                             "header instead."))
            new_shared[norm] = (tier, deps)
            allowed.add(norm)

        # Pre-patch contents for lint revert.
        pre: dict[str, str | None] = {}
        for fp in file_patches:
            p = self.repo_root / fp.path
            pre[fp.path] = (p.read_text(encoding="utf-8")
                            if p.is_file() else None)

        outcome = apply_patch(self.repo_root, file_patches, allowed)
        if not outcome.ok:
            self.patch_failures += 1
            return ToolResult(ok=False,
                              content=f"**patch rejected:** {outcome.error}",
                              data={"error": outcome.error})

        # Lint the delta for every touched file: vs the INIT snapshot for
        # writable files, vs pre-patch content for shared headers (their
        # whole content must not be re-linted — review #7).
        all_violations = []
        for fp in file_patches:
            new_text = (self.repo_root / fp.path).read_text(encoding="utf-8")
            if fp.path in set(self.meta.writable):
                old_text = self._init_snapshot_text(fp.path)
            else:
                old_text = pre[fp.path]
            all_violations += lint_delta(fp.path, old_text, new_text)

        # Separate hard violations (reject patch) from soft ones (warn only).
        # Soft violations are style issues the model should fix later but
        # shouldn't block matching.
        _SOFT_LINT_RULES = {"no_void_ptr", "no_offset_arithmetic",
                           "no_register_names", "no_register_keyword"}
        hard = [v for v in all_violations if v.rule not in _SOFT_LINT_RULES]
        soft = [v for v in all_violations if v.rule in _SOFT_LINT_RULES]

        if hard:
            for rel, old in pre.items():
                dest = self.repo_root / rel
                if old is None:
                    dest.unlink(missing_ok=True)
                else:
                    dest.write_text(old, encoding="utf-8")
            listing = "\n".join(
                f"- **{v.rule}** (line {v.line}): {v.detail}"
                for v in all_violations)
            return ToolResult(
                ok=False,
                content=("**patch reverted — lint violations:**\n" + listing),
                data={"violations": [v.rule for v in all_violations]})

        soft_warning = None
        if soft:
            soft_warning = "\n".join(
                f"- **{v.rule}** (line {v.line}): {v.detail}"
                for v in soft)
            self.transcript.log({
                "role": "harness", "kind": "lint_warning",
                "violations": [v.rule for v in soft]})

        self.patch_failures = 0
        self._dirty = True
        self._shared_headers.update(new_shared)
        if self.meta.status == SessionStatus.CLEANUP.value:
            self._needs_cleanup_verify = True
        ranges = {k: v for k, v in outcome.changed_ranges.items()}
        msg = f"**patch applied.** Changed ranges: `{json.dumps(ranges)}`"
        if soft_warning:
            msg += "\n\n**Style warnings (fix in a later patch):**\n" + soft_warning
        return ToolResult(ok=True, content=msg,
                          data={"changed_ranges": ranges, "warning": soft_warning})

    def _coerce_unit(self, unit: str) -> str:
        """The session's unit is fixed; the LLM sometimes invents names."""
        return self.meta.unit or unit

    def _build_fn(self, unit: str) -> ToolResult:
        unit = self._coerce_unit(unit)
        if self.builds >= self.budgets["max_builds"]:
            return ToolResult(ok=False,
                              content="**build budget exhausted.**")
        self.builds += 1
        ok, diagnostics = verify.build_unit(self.repo_root, unit)
        self._built_this_session = self._built_this_session or ok
        if ok:
            self.compile_error_streak = 0
            if self._dirty:
                self._auto_verify = True
            return ToolResult(ok=True, content="**build succeeded.**",
                              data={"unit": unit})
        # Compile-error attribution (review #11): foreign-caused failures
        # (diagnostics point only at files outside our write set) are
        # reported but not counted against the streak.
        foreign = (diagnostics and self.meta.writable and
                   not any(w in diagnostics for w in self.meta.writable))
        if not foreign:
            self.compile_error_streak += 1
        return ToolResult(ok=False,
                          content=("**build failed:**\n```\n" + diagnostics
                                   + "\n```"),
                          data={"foreign_caused": foreign})

    def _diff_fn(self, unit: str, symbol: str | None,
                 mode: str) -> ToolResult:
        unit = self._coerce_unit(unit)
        if not self._built_this_session:
            return ToolResult(
                ok=False,
                content="**diff:** run the build tool first this session.")
        if mode == "sweep":
            if self.agent_sweeps >= self.budgets["max_sweeps"]:
                return ToolResult(
                    ok=False,
                    content=("**sweep budget exhausted** — submit to get a "
                             "full harness check."))
            self.agent_sweeps += 1
            if self.baseline is None:
                return ToolResult(ok=False,
                                  content="**diff:** no baseline captured.")
            try:
                current = verify.current_object_path(self.repo_root, unit)
                regressions = verify.sweep(self.repo_root, unit,
                                           self.baseline, current)
            except Exception as exc:  # keep tool errors non-fatal
                return ToolResult(ok=False,
                                  content=f"**sweep failed:** {exc}")
            if not regressions:
                return ToolResult(ok=True,
                                  content="**sweep:** no sibling "
                                          "differences vs baseline.")
            lines = [f"- `{r.symbol}`: baseline {r.baseline_mismatches} → "
                     f"now {r.current_mismatches}"
                     + (" (WAS MATCHED)" if r.was_matched else "")
                     for r in regressions[:20]]
            return ToolResult(ok=True,
                              content="**sweep differences:**\n"
                                      + "\n".join(lines),
                              data={"regressions": len(regressions)})
        # target mode: instruction-level hexdiff for one symbol
        sym = symbol or self.meta.symbol
        proc = subprocess.run(
            [sys.executable, "tools/coop/hexdiff.py", unit,
             "--symbol", sym, "--json", "--no-build"],
            cwd=self.repo_root, capture_output=True, text=True, timeout=120)
        try:
            data = json.loads(proc.stdout)
        except json.JSONDecodeError:
            return ToolResult(ok=False,
                              content=("**diff failed:** "
                                       + (proc.stderr or proc.stdout)[-500:]))
        summary = {
            "mismatch_count": data.get("mismatch_count"),
            "reg_swap_count": data.get("reg_swap_count"),
            "structural_count": data.get("structural_count"),
            "reg_mapping": data.get("reg_mapping"),
        }
        return ToolResult(ok=True,
                          content="**diff:** ```json\n"
                                  + json.dumps(summary, indent=2) + "\n```",
                          data=summary)

    def _equivalence_fn(self, unit: str, symbol: str) -> ToolResult:
        result = verify.run_equivalence(self.repo_root,
                                        self._coerce_unit(unit),
                                        symbol or self.meta.symbol)
        return ToolResult(ok=True,
                          content=f"**equivalence:** {result}",
                          data={"result": result})

    # ── verification ─────────────────────────────────────────────────

    def _verify(self) -> Verdict:
        from .gates import gate_for
        gate = gate_for(self.policy.gate)
        if gate is not None:
            verdict = gate(self.repo_root, self.meta, self.baseline)
        else:
            verdict = verify.verdict_for_submit(
                self.repo_root, self.meta.unit, self.meta.symbol,
                self.baseline,
                equivalence_min_fuzzy=self.cfg.equivalence_min_fuzzy)
        verdict = self._cross_tu_check(verdict)
        self._verdict = verdict
        return verdict

    def _cross_tu_check(self, verdict: Verdict) -> Verdict:
        """v1.1: sweep dependent units of touched shared headers."""
        if not self._shared_headers:
            return verdict
        from .cross_tu import (dependent_units, has_hard_failure,
                               sweep_dependents)
        units: set[str] = set()
        tier = None
        for _path, (t, deps) in self._shared_headers.items():
            units |= set(dependent_units(deps))
            if tier is None or t.value > tier.value:
                tier = t
        units.discard(self.meta.unit)
        if not units:
            return verdict

        # Protect previously-accepted symbols in dependent units.
        from tools.coop.lib.config import load_config as load_coop_config
        from tools.coop.lib.targets import load_targets
        coop_cfg = load_coop_config(None, self.repo_root)
        accepted_baseline = {
            f"{t.unit}:{t.symbol}": 0
            for t in load_targets(coop_cfg)
            if t.unit in units
            and t.status in ("FULL_MATCH", "EQUIVALENT_MATCH")}

        results = sweep_dependents(self.repo_root, sorted(units), tier,
                                   accepted_baseline)
        self.transcript.log({"role": "harness", "kind": "cross_tu_sweep",
                             "units": sorted(units),
                             "tier": tier.name,
                             "hard_failure": has_hard_failure(
                                 results, accepted_baseline)})
        if not has_hard_failure(results, accepted_baseline):
            return verdict
        details = []
        for r in results:
            if not r.get("ok", True):
                details.append(f"- `{r['unit']}`: build failed\n```\n"
                               f"{r.get('detail', '')[-500:]}\n```")
            elif r.get("diffs"):
                for d in r["diffs"][:5]:
                    details.append(f"- `{r['unit']}` `{d.get('symbol')}`: "
                                   f"{d.get('mismatches')} mismatch(es)")
        return Verdict(
            kind=VerdictKind.HARD_REJECT, accepted=False,
            target_symbol=self.meta.symbol, rule="cross_tu_regression",
            text_size=verdict.text_size, text_budget=verdict.text_budget,
            diagnostics=("Shared-header edit regressed dependent units:\n"
                         + "\n".join(details)))

    def _restore(self, snapshot_dir: Path) -> None:
        for rel in self.meta.writable:
            snap = snapshot_dir / rel
            dest = self.repo_root / rel
            if snap.exists():
                dest.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(snap, dest)

    def _handle_verdict(self, verdict: Verdict) -> str:
        """Apply state transitions; returns markdown feedback for the agent."""
        if verdict.kind is VerdictKind.COMPILE_ERROR:
            return ("**verification: build failed.** Fix the compile "
                    "errors:\n```\n" + verdict.diagnostics[-2000:] + "\n```")

        if verdict.accepted:
            state.record_accepted(self.repo_root, self.sdir, self.meta)
            state.set_status(self.sdir, self.meta, SessionStatus.CLEANUP)
            self.cleanup_turns_left = self.budgets.get("cleanup_turns", 0)
            if self.cleanup_turns_left:
                return (f"**ACCEPTED ({verdict.rule}).** The harness has "
                        f"snapshotted the matching state. You have "
                        f"{self.cleanup_turns_left} cleanup turns for "
                        "optional tidy-up (comments, dead code) — every "
                        "patch is re-verified; breaking the match reverts "
                        "to the accepted snapshot. Submit when done.")
            return ""

        # Best-state checkpoint for carryover (review #6).
        if (verdict.target_mismatches is not None and
                (self.best_mismatches is None or
                 verdict.target_mismatches < self.best_mismatches)):
            self.best_mismatches = verdict.target_mismatches
            state.record_best(self.repo_root, self.sdir, self.meta)

        if verdict.kind is VerdictKind.HARD_REJECT:
            snap = (paths.accepted_snapshot_dir(self.sdir)
                    if self.meta.status == SessionStatus.CLEANUP.value
                    else paths.init_snapshot_dir(self.sdir))
            self._restore(snap)
            regs = "\n".join(f"- `{r.symbol}` ({r.baseline_mismatches} → "
                             f"{r.current_mismatches})"
                             for r in verdict.regressions[:10])
            if verdict.diagnostics:
                regs = (regs + "\n" if regs else "") + verdict.diagnostics
            return ("**HARD REJECT — matched sibling(s) regressed; your "
                    "change was reverted.**\n" + regs)
        # SOFT_REJECT
        parts = [f"**not accepted ({verdict.rule}).**"]
        if verdict.target_mismatches is not None:
            parts.append(f"Target mismatches: "
                         f"{verdict.target_mismatches}"
                         + (f" (fuzzy ~{verdict.target_fuzzy:.0f}%)"
                            if verdict.target_fuzzy is not None else ""))
        if verdict.regressions:
            regs = "\n".join(f"- `{r.symbol}` ({r.baseline_mismatches} → "
                             f"{r.current_mismatches})"
                             for r in verdict.regressions[:10])
            parts.append("Sibling(s) worse than baseline — fix or revert "
                         "your change:\n" + regs)
        parts.append(f".text size: {verdict.text_size}"
                     + (f" / budget {verdict.text_budget}"
                        if verdict.text_budget else ""))
        return "\n".join(parts)

    # ── main loop ────────────────────────────────────────────────────

    def _elide_if_needed(self, messages: list[dict]) -> None:
        approx = sum(len(str(m.get("content", ""))) for m in messages) // 4
        if approx < 0.8 * self.budgets["max_session_tokens"]:
            return
        for m in messages[2:]:  # keep system + brief
            if m.get("role") == "tool" and len(str(m.get("content", ""))) > 500:
                m["content"] = "[elided stale tool result]"

    def run(self) -> SessionOutcome:
        cfg, meta = self.cfg, self.meta
        try:
            self.baseline = verify.capture_baseline(
                self.repo_root, meta.unit, self.sdir)
        except Exception as exc:
            # No baseline = nothing downstream is meaningful (review: a
            # session without a baseline verifies against garbage).
            self.transcript.log({"role": "harness", "kind": "error",
                                 "stage": "baseline", "error": str(exc)})
            self._restore(paths.init_snapshot_dir(self.sdir))
            state.set_status(self.sdir, self.meta, SessionStatus.DONE)
            self.transcript.log({"role": "harness", "kind": "final",
                                 "accepted": False,
                                 "reason": "baseline_failed"})
            self.transcript.close()
            return SessionOutcome(accepted=False, reason="baseline_failed",
                                  session_id=self.meta.session_id)
        state.set_status(self.sdir, meta, SessionStatus.MATCHING)

        # Pin the include graph for cross-TU checks (v1.1, review #16).
        try:
            from .include_graph import IncludeGraph
            self._graph = IncludeGraph.load_or_build(self.repo_root)
            self.transcript.log({"role": "harness", "kind": "include_graph",
                                 "snapshot_id": self._graph.snapshot_id()})
        except Exception as exc:
            self.transcript.log({"role": "harness", "kind": "error",
                                 "stage": "include_graph",
                                 "error": str(exc)})

        brief = build_brief(
            target_id=meta.target_id, symbol=meta.symbol,
            demangled=self.demangled, signature=self.signature,
            unit=meta.unit, retail_asm=self.retail_asm,
            writable=meta.writable, baseline=self.baseline,
            carryover=self.carryover, session_type=self.policy.name,
            max_chars=cfg.prompt_max_chars,
            source_content=self._init_read_src(),
            header_content=self._read_tu_header(meta.unit))
        self.transcript.log({"role": "system", "config": self.model.model})
        self.transcript.log({"role": "user", "kind": "brief",
                             "chars": len(brief)})

        schemas = [s for s in TOOL_SCHEMAS + RENAME_TOOL_SCHEMAS
                   if s["function"]["name"] in self.policy.tools]
        messages: list[dict] = [
            {"role": "system", "content": _SYSTEM_PROMPT},
            {"role": "user", "content": brief},
        ]

        reason = "budget_exhausted"
        while True:
            if self.meta.status == SessionStatus.CLEANUP.value:
                if self.cleanup_turns_left <= 0:
                    reason = "accepted"
                    break
                self.cleanup_turns_left -= 1
            elif self.turns >= self.budgets["max_turns"]:
                reason = "budget_exhausted"
                break
            if self.patch_failures >= self.budgets["max_patch_failures"]:
                reason = "patch_failure_streak"
                break
            if self.compile_error_streak >= cfg.max_compile_error_streak:
                reason = "compile_error_streak"
                break

            self._elide_if_needed(messages)
            reply = self.provider.send(messages, schemas, self.model)
            self.turns += 1
            self.tokens_in += reply.usage.get("input_tokens", 0)
            self.cache_tokens_in += reply.usage.get("cache_input_tokens", 0) or 0
            self.tokens_out += reply.usage.get("output_tokens", 0) or 0
            cost = compute_cost(reply.usage)
            self.cost_total += cost.get("total_cost", 0) or 0
            self.transcript.log({
                "turn": self.turns, "role": "assistant",
                "text": reply.text[:2000],
                "tool_calls": [{"name": c.name, "args": c.args} for c in reply.tool_calls],
                "usage": reply.usage,
                "cost": cost,
                "finish_reason": reply.finish_reason})

            messages.append({
                "role": "assistant", "content": reply.text or None,
                "tool_calls": [
                    {"id": c.id or f"call_{self.turns}_{i}",
                     "type": "function",
                     "function": {"name": c.name,
                                  "arguments": json.dumps(c.args)}}
                    for i, c in enumerate(reply.tool_calls)] or None,
            })

            if not reply.tool_calls:
                self._no_tool_turns += 1
                if self.meta.status == SessionStatus.CLEANUP.value:
                    reason = "accepted"  # nothing left to clean up
                    break
                if self._no_tool_turns >= 2:
                    reason = "no_progress"
                    break
                messages.append({
                    "role": "user",
                    "content": ("No tool calls made. Use the tools to make "
                                "progress; call submit when the function "
                                "matches.")})
                continue
            self._no_tool_turns = 0

            for call in reply.tool_calls:
                result = dispatch(call, self._ctx())
                self.transcript.log({
                    "turn": self.turns, "role": "tool", "name": call.name,
                    "ok": result.ok, "data": result.data,
                    "content": result.content[:2000]})
                if result.data.get("submitted"):
                    self._submitted = True
                    self._submit_note = result.data.get("note", "")
                messages.append({
                    "role": "tool",
                    "tool_call_id": call.id or f"call_{self.turns}",
                    "content": result.content,
                })

            # Verification triggers: submit, cleanup patch, or auto-verify
            # (build after patch). Skip when nothing changed since last verdict.
            if (self._submitted or self._needs_cleanup_verify or self._auto_verify) and (
                    self._dirty or self._verdict is None):
                self._needs_cleanup_verify = False
                self._auto_verify = False
                self._dirty = False
                verdict = self._verify()
                self.transcript.log({
                    "turn": self.turns, "role": "harness", "kind": "verdict",
                    "accepted": verdict.accepted, "rule": verdict.rule,
                    "target_mismatches": verdict.target_mismatches,
                    "regressions": len(verdict.regressions)})
                feedback = self._handle_verdict(verdict)
                if self.meta.status == SessionStatus.CLEANUP.value \
                        and not feedback:
                    reason = "accepted"  # accepted, no cleanup turns
                    break
                if feedback:
                    messages.append({"role": "user", "content": feedback})
                if self._submitted:
                    self._submitted = False
                    if (verdict.accepted and
                            self.cleanup_turns_left <= 0):
                        reason = "accepted"
                        break
                # failed submit: conversation continues (review #8)
            elif self._submitted:
                # Re-submit with no changes: echo the last verdict.
                self._submitted = False
                if (self._verdict is not None and self._verdict.accepted
                        and self.cleanup_turns_left <= 0):
                    reason = "accepted"
                    break
                messages.append({
                    "role": "user",
                    "content": ("Nothing changed since the last "
                                "verification; the previous verdict still "
                                "stands.")})

        # ── DONE ──
        accepted = reason == "accepted"
        if accepted:
            self._restore(paths.accepted_snapshot_dir(self.sdir))
        else:
            self._restore(paths.init_snapshot_dir(self.sdir))
        state.set_status(self.sdir, self.meta, SessionStatus.DONE)
        self.transcript.log({
            "role": "harness", "kind": "final", "accepted": accepted,
            "reason": reason,
            "verdict_rule": self._verdict.rule if self._verdict else "",
            "submit_note": self._submit_note,
            "turns": self.turns, "builds": self.builds,
            "tokens_in": self.tokens_in,
            "cache_tokens_in": self.cache_tokens_in,
            "tokens_out": self.tokens_out,
            "cost_total": round(self.cost_total, 6)})
        self.transcript.close()
        return SessionOutcome(
            accepted=accepted, reason=reason, session_id=meta.session_id,
            verdict_rule=self._verdict.rule if self._verdict else "")
