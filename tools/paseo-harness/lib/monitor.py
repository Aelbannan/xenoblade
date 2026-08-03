"""Sweep loop: keeps up to N paseo agents running over a batch plan, each in
its own worktree workspace, and merges finished batches back into main.

Cost controls implemented here (see README.md):
- lower default concurrency (--running, default 6)
- provider-epidemic backoff: if >=N agents error/hang in one sweep, pause
  launches for a few sweeps instead of relaunching into the same jam
- per-batch relaunch budget + global relaunch budget; relaunches continue the
  SAME branch (checkout-branch), seeded with the previous attempt's report,
  with thinking lowered one notch (and optional fallback model)
- agent-hour budget gate (--max-agent-hours) stops launching new batches
- merge drivers make shared-registry writes conflict-free on merge-back
"""
import json
import os
import time
from datetime import datetime, timezone

from . import gitutil, paseo
import shlex

from .common import (ROOT, TARGETS, VENV_PY, now_iso, save_json, sh, slug,
                   state_dir, utc_age_minutes)
from .prompt import build_prompt

MODEL_DEFAULT = "openrouter/deepseek/deepseek-v4-flash-0731"
THINKING_LADDER = ["max", "high", "medium", "medium"]
GRACE_MIN = 12           # error grace period before treating an agent as dead
FRESH_MIN = 5            # activity younger than this => alive regardless of status
TITLE_FMT = "{section} {bid}: {label} ({total} fns)"


class Monitor:
    def __init__(self, section, plan, args):
        self.section = section
        self.secslug = slug(section)
        self.plan = plan
        self.args = args
        # monitor uses only a few args; report/merge subcommands don't define them all
        self.model = getattr(args, "model", None) or MODEL_DEFAULT
        self.fallback_model = getattr(args, "fallback_model", None)
        self.allow_smt = getattr(args, "allow_smt", False)
        self.running_max = getattr(args, "running", 6)
        self.max_idle_min = getattr(args, "max_idle_min", 60)
        self.max_relaunch = getattr(args, "max_relaunch", 3)
        self.backoff_threshold = getattr(args, "backoff_threshold", 3)
        self.backoff_sweeps = getattr(args, "backoff_sweeps", 2)
        self.max_agent_hours = getattr(args, "max_agent_hours", 0.0)
        self.thinking_override = getattr(args, "thinking", None)
        self.interval = getattr(args, "interval", 180)
        self.once = getattr(args, "once", False)
        self.base = getattr(args, "base", "main")
        self.prewarm = getattr(args, "prewarm", False)
        coop = {}
        try:
            coop = json.load(open(ROOT / "coop.json"))
        except Exception:
            pass
        self.region = coop.get("region", "us")
        self.ninja = coop.get("ninja") or "ninja"
        self.state_path = getattr(args, "state_dir", state_dir(section)) / "state.json"
        self.reports_dir = self.state_path.parent / "reports"
        self.log_path = self.state_path.parent / "monitor.log"
        self.st = self._load_state()
        self.by_id = {b["batch_id"]: b for b in plan["batches"]}
        self.labels = [f"paseo-harness={self.secslug}", f"section={self.secslug}"]
        self.errors_this_sweep = 0
        os.makedirs(self.reports_dir, exist_ok=True)

    # --- state -------------------------------------------------------------

    def _load_state(self):
        if self.state_path.exists():
            try:
                st = json.load(open(self.state_path))
            except Exception:
                st = None
        else:
            st = None
        if not st:
            st = {
                "section": self.section,
                "batches": {},
                "done": [],
                "gave_up": [],
                "relaunches_total": 0,
                "spent_hours": 0.0,
                "backoff_sweeps_left": 0,
                "started_at": now_iso(),
            }
        st.setdefault("done", [])
        st.setdefault("gave_up", [])
        st.setdefault("batches", {})
        return st

    def _save(self):
        save_json(self.state_path, self.st)

    def _log(self, msg):
        line = f"[{now_iso()}] {msg}"
        print(line, flush=True)
        with open(self.log_path, "a") as f:
            f.write(line + "\n")

    # --- helpers -----------------------------------------------------------

    def _report_for(self, bid, agent_id):
        tail = paseo.logs(agent_id, tail=20000)
        path = self.reports_dir / f"{bid}.txt"
        with open(path, "w") as f:
            f.write(tail or "(no log)")
        return path

    def _accepted_summary(self, bid, rec):
        """Count ACCEPTED batch targets on the batch branch (script-side)."""
        batch = self.by_id.get(bid)
        if not batch:
            return (0, 0, [])
        raw = gitutil.read_file_from_branch(rec["branch"], "tools/coop/targets.json")
        if not raw:
            return (None, batch["total"], [])
        try:
            d = json.loads(raw)
            by_id = {t["id"]: t for t in d.get("targets", [])}
        except Exception:
            return (None, batch["total"], [])
        acc = [tid for tid in batch["targets"] if by_id.get(tid, {}).get("workflow_status") == "ACCEPTED"]
        return (len(acc), batch["total"], acc)

    def _hours(self, rec):
        """Agent-hours for one batch: accumulated prior launches + current segment."""
        seg = rec.get("spent_hours", 0.0)
        start = rec.get("launched_at")
        if start:
            try:
                s = datetime.fromisoformat(start)
                end = datetime.fromisoformat(rec["finished_at"]) if rec.get("finished_at") else datetime.now(timezone.utc)
                seg += max(0.0, (end - s).total_seconds() / 3600.0)
            except Exception:
                pass
        return seg

    def _total_hours(self):
        return self.st.get("spent_hours", 0.0) + sum(self._hours(r) for r in self.st["batches"].values())

    # --- launching ---------------------------------------------------------

    def _title(self, batch, attempt):
        t = TITLE_FMT.format(section=self.secslug, bid=batch["batch_id"],
                             label=batch["label"], total=batch["total"])
        return t if attempt <= 1 else f"{t} [attempt {attempt}]"

    def _thinking_for(self, batch, attempt):
        base = self.thinking_override or batch.get("thinking") or "max"
        if attempt <= 1:
            return base
        try:
            i = THINKING_LADDER.index(base)
        except ValueError:
            return "medium"
        return THINKING_LADDER[min(len(THINKING_LADDER) - 1, i + attempt - 1)]

    def _model_for(self, attempt):
        if attempt >= 2 and self.fallback_model:
            return self.fallback_model
        return self.model

    def _launch(self, batch, attempt=1, seed_text=None):
        bid = batch["batch_id"]
        branch = f"paseo/{self.secslug}/{bid}"
        title = self._title(batch, attempt)
        thinking = self._thinking_for(batch, attempt)
        model = batch.get("model") or self._model_for(attempt)
        prompt = build_prompt(
            batch, section=self.section, worktree_path="(created by paseo)",
            branch=branch, launched_at=now_iso(), relaunch=attempt,
            seed_text=seed_text, allow_smt=self.allow_smt,
        )
        # create the worktree workspace FIRST so we can prepare it (orig symlink,
        # optional prewarm) before the agent's first hexdiff
        ws = paseo.create_worktree_workspace(
            branch, base=self.base, title=title,
            slug=f"paseo-{self.secslug}-{bid}-r{attempt}", path=str(ROOT),
        )
        if not ws or not ws.get("workspaceId"):
            self._log(f"launch {bid}: workspace create failed")
            return None
        wt = ws.get("cwd")
        if wt and not self._prepare_worktree(wt, batch):
            paseo.archive_workspace(ws["workspaceId"])
            self._log(f"launch {bid}: worktree prepare failed (not buildable)")
            return None
        rc, out = paseo.launch(
            prompt, model=model, thinking=thinking, title=title,
            workspace_id=ws["workspaceId"], labels=self.labels + [f"batch={bid}"],
        )
        if rc != 0:
            paseo.archive_workspace(ws["workspaceId"])
            self._log(f"launch {bid} FAILED rc={rc}: {out[-300:]}")
            return None
        aid = paseo.parse_agent_id(out)
        if not aid:
            paseo.archive_workspace(ws["workspaceId"])
            self._log(f"launch {bid}: could not parse agent id from: {out[-200:]}")
            return None
        return {
            "agent_id": aid,
            "branch": branch,
            "title": title,
            "launched_at": now_iso(),
            "status": "running",
            "launches": attempt,
            "error_since": None,
            "finished_at": None,
            "merged_at": None,
            "spent_hours": 0.0,
            "workspace_id": ws["workspaceId"],
            "worktree_path": wt,
            "model": model,
            "thinking": thinking,
            "accepted": None,
            "accepted_total": batch["total"],
        }

    def _prepare_worktree(self, wt, batch):
        """Fresh worktrees cannot build without the untracked retail files
        (orig/) or a build.ninja (configure.py): symlink orig/, quiet it in
        git, and run configure. Returns False if the worktree is not buildable
        (caller should treat the launch as failed)."""
        from pathlib import Path
        wt = Path(wt)
        if not wt.exists():
            self._log(f"prepare: worktree {wt} missing")
            return False
        orig = ROOT / "orig"
        wt_orig = wt / "orig"
        if orig.is_dir():
            if wt_orig.exists() and not wt_orig.is_symlink():
                # the worktree checked out the tracked orig/ skeleton (.gitkeep
                # dirs only); replace it with a symlink to the real retail files.
                # Safe: git tracks only .gitkeep under orig/, and orig/ is in the
                # worktree's info/exclude so git never notices.
                os.system(f"rm -rf {shlex.quote(str(wt_orig))}")
                self._log(f"prepare: replaced tracked orig/ skeleton with symlink target")
            if not wt_orig.exists():
                try:
                    os.symlink(orig, wt_orig, target_is_directory=True)
                    self._log(f"prepare: symlinked {orig} -> {wt_orig}")
                except Exception as e:
                    self._log(f"prepare: orig symlink failed: {e}")
                    return False
        # the venv is gitignored, so worktrees need it symlinked in for
        # .venv/bin/python3 (the agents' first command) to resolve
        wt_venv = wt / ".venv"
        if (ROOT / ".venv").is_dir() and not wt_venv.exists():
            try:
                os.symlink(ROOT / ".venv", wt_venv, target_is_directory=True)
                self._log(f"prepare: symlinked .venv -> {wt_venv}")
            except Exception as e:
                self._log(f"prepare: .venv symlink failed: {e}")
                return False
        r = sh(["git", "-C", str(wt), "rev-parse", "--git-path", "info/exclude"])
        if r.returncode == 0:
            excl = Path(r.stdout.strip())
            if not excl.is_absolute():
                excl = wt / excl
            try:
                txt = excl.read_text() if excl.exists() else ""
                add = [p for p in ("orig/",) if p not in txt]
                if add:
                    with open(excl, "a") as f:
                        f.write("\n" + "\n".join(add) + "\n")
            except Exception as e:
                self._log(f"prepare: exclude update failed: {e}")
        # mandatory: hexdiff/cycle run ninja directly and never configure.
        # Absolute script path — relative paths would resolve against the
        # harness's own cwd and configure the WRONG checkout.
        cfg = sh([str(VENV_PY), str(wt / "tools" / "coop" / "run.py"), "configure"],
                 cwd=wt, timeout=3600)
        if cfg.returncode != 0:
            self._log(f"prepare: configure failed rc={cfg.returncode}: {cfg.stderr[-300:]}")
            return False
        self._log(f"prepare: configured {wt}")
        if self.prewarm:
            self._prewarm(wt, batch)
        return True

    def _prewarm(self, wt, batch):
        """Build the batch's unit objects so the agent's first hexdiff is
        incremental. Uses ninja DIRECTLY: run.py build demands objdiff.json,
        which only appears after ninja's split -> reconfigure chain runs; the
        first ninja invocation on a fresh worktree does the split, regenerates
        build.ninja + objdiff.json, then builds the requested object."""
        for unit in batch["units"]:
            target = f"build/{self.region}/src/{unit}.o"
            r = sh([self.ninja, target], cwd=wt, timeout=3600)
            if r.returncode != 0:
                self._log(f"prewarm: ninja {target} rc={r.returncode} (continuing)")
        self._log(f"prewarm: built {len(batch['units'])} unit(s) in {wt}")

    def _relaunch(self, bid, rec, reason):
        self.st["spent_hours"] = self.st.get("spent_hours", 0.0) + self._hours(rec)
        rec["spent_hours"] = 0.0
        rec["launched_at"] = None
        attempt = rec.get("launches", 1) + 1
        if attempt > self.max_relaunch:
            self._log(f"{bid}: relaunch budget exceeded ({self.max_relaunch}); giving up")
            self._give_up(bid, rec, "relaunch budget")
            return
        # free the old worktree so checkout-branch can create a new one
        if rec.get("workspace_id"):
            paseo.archive_workspace(rec["workspace_id"])
        rec["workspace_id"] = None
        rec["worktree_path"] = None
        self.st["relaunches_total"] = self.st.get("relaunches_total", 0) + 1
        seed = None
        report = self.reports_dir / f"{bid}.txt"
        if report.exists():
            seed = report.read_text(errors="replace")
        batch = self.by_id.get(bid)
        if batch is None:
            self._log(f"{bid}: not in plan; dropping")
            self._give_up(bid, rec, "not in plan")
            return
        self._log(f"RELAUNCH {bid} (attempt {attempt}, {reason})")
        newrec = self._launch(batch, attempt=attempt, seed_text=seed)
        if not newrec:
            self._log(f"{bid}: relaunch failed to start; will retry next sweep")
            rec["status"] = "relaunch"
            rec["error_since"] = None
            self._save()
            return
        rec.update(newrec)
        rec["error_since"] = None
        rec["status"] = "running"
        self._save()

    def _give_up(self, bid, rec, reason):
        rec["status"] = "gave_up"
        rec["finished_at"] = now_iso()
        if bid not in self.st["gave_up"]:
            self.st["gave_up"].append(bid)
        self._save()

    def _finalize_done(self, bid, rec):
        """Batch agent finished: report, ensure committed, merge, archive."""
        try:
            self._report_for(bid, rec["agent_id"])
            n, total, ids = self._accepted_summary(bid, rec)
            rec["accepted"] = n
            rec["accepted_total"] = total
            rec["accepted_ids"] = ids
            with open(self.reports_dir / f"{bid}.txt", "a") as f:
                f.write("\n--- harness summary ---\n")
                f.write(f"accepted in branch: {n}/{total}\n")
                if ids:
                    f.write(f"  {', '.join(ids[:30])}\n")
        except Exception as e:
            self._log(f"{bid}: report write failed: {e}")
        if rec.get("worktree_path"):
            gitutil.commit_worktree_changes(rec["worktree_path"], f"paseo-harness: sweep {bid}")
        status, detail = gitutil.merge_batch(rec["branch"], f"paseo-harness: merge {bid}", base=self.base)
        paseo.archive_agent(rec["agent_id"])
        rec["finished_at"] = now_iso()
        if status == "merged":
            rec["status"] = "done"
            rec["merged_at"] = now_iso()
            if rec.get("workspace_id"):
                paseo.archive_workspace(rec["workspace_id"])
            if bid not in self.st["done"]:
                self.st["done"].append(bid)
            self._log(f"FINISHED+MERGED: {bid} -> {detail or 'ok'}")
        elif status == "conflict":
            rec["status"] = "merge_conflict"
            self._log(f"MERGE CONFLICT {bid} (branch kept: {rec['branch']}): {detail[-300:]}")
        else:
            rec["status"] = "merge_deferred"
            self._log(f"MERGE DEFERRED {bid} (main tree busy; will retry): {detail[-200:]}")
        self._save()

    def _retry_merge(self, bid, rec):
        status, detail = gitutil.merge_batch(rec["branch"], f"paseo-harness: merge {bid}", base=self.base)
        if status == "merged":
            rec["status"] = "done"
            rec["merged_at"] = now_iso()
            if rec.get("workspace_id"):
                paseo.archive_workspace(rec["workspace_id"])
            if bid not in self.st["done"]:
                self.st["done"].append(bid)
            self._log(f"MERGED (retry): {bid}")
            self._save()
            return True
        return False

    # --- sweep -------------------------------------------------------------

    def sweep(self):
        agents = paseo.ls_agents(global_=True, labels=self.labels)
        by_aid = {a["id"]: a for a in agents}
        self.errors_this_sweep = 0
        stalled = 0

        for bid, rec in list(self.st["batches"].items()):
            status = rec.get("status")
            if status == "merge_deferred":
                self._retry_merge(bid, rec)
                continue
            if status in ("done", "gave_up", "merge_conflict"):
                continue

            if not rec.get("agent_id"):
                # crashed earlier / launch pending: launch now if allowed
                if status == "relaunch":
                    self._relaunch(bid, rec, "relaunch slot")
                continue

            info = by_aid.get(rec["agent_id"])
            insp = paseo.inspect(rec["agent_id"]) if info is not None else None
            if info is None or insp is None:
                self._log(f"CRASHED: {bid} agent {rec['agent_id']} no longer listed — relaunching")
                self.errors_this_sweep += 1
                rec["status"] = "relaunch"
                rec["agent_id"] = None
                self._save()
                continue

            age = utc_age_minutes(insp.get("UpdatedAt"))
            if age is None:
                continue
            if age < FRESH_MIN:
                rec["status"] = "running"
                rec["error_since"] = None
                continue

            state = (insp.get("Status") or "").lower()
            if state in ("running", "pending"):
                if age > self.max_idle_min:
                    self._log(f"HANG: {bid} no activity {age:.0f}m — archiving and relaunching")
                    paseo.archive_agent(rec["agent_id"])
                    self.errors_this_sweep += 1
                    stalled += 1
                    rec["status"] = "relaunch"
                    rec["agent_id"] = None
                    self._save()
                continue
            if state in ("idle", "completed", "finished"):
                self._finalize_done(bid, rec)
                continue

            # error status + stale activity: grace period then relaunch
            if not rec.get("error_since"):
                rec["error_since"] = now_iso()
                self._save()
                continue
            grace = utc_age_minutes(rec["error_since"]) or 0.0
            if grace < GRACE_MIN:
                continue
            self._log(f"DEAD: {bid} error for {grace:.0f}m — archiving and relaunching")
            paseo.archive_agent(rec["agent_id"])
            self.errors_this_sweep += 1
            rec["status"] = "relaunch"
            rec["agent_id"] = None
            rec["error_since"] = None
            self._save()

        # relaunch slots
        for bid, rec in list(self.st["batches"].items()):
            if rec.get("status") == "relaunch" and not rec.get("agent_id"):
                self._relaunch(bid, rec, "relaunch slot")

        # provider-epidemic backoff
        if self.errors_this_sweep >= self.backoff_threshold:
            self.st["backoff_sweeps_left"] = self.backoff_sweeps
            self._log(f"BACKOFF: {self.errors_this_sweep} errors/hangs this sweep — "
                      f"pausing launches for {self.backoff_sweeps} sweeps")

        # launch new batches
        self._launch_pending(stalled)

    def _launch_pending(self, stalled=0):
        running = sum(1 for r in self.st["batches"].values() if r.get("agent_id") and r.get("status") == "running")
        pending = [b for b in self.plan["batches"] if b["batch_id"] not in self.st["batches"]]
        pending.sort(key=lambda b: b.get("launch_rank", 999))
        budget_hours = self.max_agent_hours
        total_hours = self._total_hours()

        if self.st.get("backoff_sweeps_left", 0) > 0:
            self.st["backoff_sweeps_left"] -= 1
            self._log(f"backoff active ({self.st['backoff_sweeps_left']} sweeps left) — skipping new launches")
            self._save()
            return

        for b in pending:
            if running >= self.running_max:
                break
            if budget_hours and total_hours >= budget_hours:
                self._log(f"AGENT-HOUR BUDGET reached ({total_hours:.1f}h >= {budget_hours}h) — not launching {b['batch_id']}")
                break
            rec = self._launch(b)
            if not rec:
                self._log(f"launch {b['batch_id']} FAILED — retrying next sweep")
                break
            self.st["batches"][b["batch_id"]] = rec
            running += 1
            total_hours += self._hours(rec)
            self._log(f"launched {b['batch_id']} as {rec['agent_id'][:8]} "
                      f"(thinking={rec['launches'] and self._thinking_for(b, rec['launches'])}, "
                      f"branch={rec['branch']})")
            self._save()

    def summary(self):
        running = sum(1 for r in self.st["batches"].values() if r.get("status") == "running")
        total = len(self.plan["batches"])
        done = len(self.st["done"])
        gave = len(self.st["gave_up"])
        pending = len([b for b in self.plan["batches"] if b["batch_id"] not in self.st["batches"]])
        conflicted = sum(1 for r in self.st["batches"].values() if r.get("status") == "merge_conflict")
        deferred = sum(1 for r in self.st["batches"].values() if r.get("status") == "merge_deferred")
        self._log(
            f"running={running} done={done}/{total} gave_up={gave} conflicted={conflicted} "
            f"deferred={deferred} pending={pending} relaunches={self.st.get('relaunches_total', 0)} "
            f"agent_hours={self._total_hours():.1f}"
        )

    def run(self):
        # single-instance guard: another monitor for this section would double-launch batches
        pidfile = self.state_path.parent / "monitor.pid"
        if pidfile.exists():
            try:
                old = int(pidfile.read_text().strip())
                os.kill(old, 0)
                self._log(f"another monitor is running (pid {old}); exiting")
                return
            except (ProcessLookupError, ValueError):
                pass
        pidfile.write_text(str(os.getpid()))
        try:
            while True:
                self.sweep()
                self.summary()
                if self.once:
                    break
                remaining = (len([b for b in self.plan["batches"] if b["batch_id"] not in self.st["batches"]])
                             + sum(1 for r in self.st["batches"].values()
                                   if r.get("status") in ("running", "relaunch", "merge_deferred")))
                if remaining == 0:
                    self._log("ALL BATCHES COMPLETE")
                    break
                time.sleep(self.interval)
        finally:
            try:
                pidfile.unlink()
            except Exception:
                pass

    def report(self):
        plan = self.plan
        print(f"section={self.section}  plan batches={len(plan['batches'])} "
              f"(ready={plan.get('ready_targets')} deferred={plan.get('deferred_targets')})")
        print(f"deferred reasons: {plan.get('defer_reasons', {})}")
        print(f"{'batch':<14} {'status':<15} {'acc/tot':<9} {'thinking':<7} {'hours':<6} {'acc/hr':<7} agent")
        by_tier = {}
        for bid, rec in sorted(self.st["batches"].items()):
            acc = rec.get("accepted")
            tot = rec.get("accepted_total")
            hrs = self._hours(rec)
            acc_str = f"{acc}/{tot}" if acc is not None else "?/?"
            ahr = f"{acc / hrs:.2f}" if (acc is not None and hrs > 0) else "-"
            print(f"{bid:<14} {rec.get('status','?'):<15} {acc_str:<9} "
                  f"{rec.get('thinking','?'):<7} {hrs:<6.1f} {ahr:<7} {(rec.get('agent_id') or '')[:8]}")
            tier = rec.get("thinking", "?")
            by_tier.setdefault(tier, {"batches": 0, "acc": 0, "hrs": 0.0})
            by_tier[tier]["batches"] += 1
            if acc is not None:
                by_tier[tier]["acc"] += acc
            by_tier[tier]["hrs"] += hrs
        print("--- by thinking tier ---")
        for tier, t in sorted(by_tier.items()):
            ahr = f"{t['acc'] / t['hrs']:.2f}" if t["hrs"] > 0 else "-"
            print(f"  {tier:<7} batches={t['batches']:<3} accepted={t['acc']:<4} "
                  f"hours={t['hrs']:<6.1f} accepts/hr={ahr}")
        print(f"done={len(self.st['done'])} gave_up={len(self.st['gave_up'])} "
              f"conflicted={sum(1 for r in self.st['batches'].values() if r.get('status') == 'merge_conflict')} "
              f"relaunches={self.st.get('relaunches_total', 0)} "
              f"agent_hours={self._total_hours():.1f}")
