"""Thin wrapper around the paseo CLI used by the harness."""
import json
import time

from .common import sh, paseo_bin


def _pick(d, *names, default=None):
    """Case-insensitive-ish key lookup across paseo's inconsistent casing."""
    if not isinstance(d, dict):
        return default
    for n in names:
        if n in d:
            return d[n]
    low = {k.lower(): v for k, v in d.items()}
    for n in names:
        if n.lower() in low:
            return low[n.lower()]
    return default


def _run(args, cwd=None, timeout=120):
    return sh([paseo_bin()] + args, cwd=cwd, timeout=timeout)


def ls_agents(global_=True, labels=None):
    """List agents as JSON. Filters to harness-labelled agents when labels given."""
    cmd = ["ls", "--json"]
    if global_:
        cmd.append("--global")
    for kv in labels or []:
        cmd += ["--label", kv]
    r = _run(cmd)
    if r.returncode != 0:
        return []
    try:
        return json.loads(r.stdout)
    except Exception:
        return []


def inspect(agent_id):
    r = _run(["inspect", agent_id, "--json"])
    if r.returncode != 0:
        return None
    try:
        return json.loads(r.stdout)
    except Exception:
        return None


def status_of(agent):
    return (_pick(agent, "Status", "status") or "").lower()


def archive_agent(agent_id):
    r = _run(["archive", agent_id, "--force"])
    return r.returncode == 0


def logs(agent_id, tail=20000):
    r = _run(["logs", agent_id])
    return r.stdout or ""


def list_workspaces():
    r = _run(["workspace", "ls", "--json"])
    if r.returncode != 0:
        return []
    try:
        return json.loads(r.stdout)
    except Exception:
        return []


def find_workspace_by_name(name, isolation=None):
    for w in list_workspaces():
        if _pick(w, "name", "Name") == name:
            if isolation and (_pick(w, "isolation", "Isolation") or "").lower() != isolation.lower():
                continue
            return w
    return None


def archive_workspace(workspace_id):
    r = _run(["workspace", "archive", workspace_id])
    return r.returncode == 0


def _resolve_ref(ref, cwd=None):
    """Resolve a ref to a full SHA — paseo's --base can otherwise resolve to a
    stale cached commit, silently branching 6+ hours behind main."""
    r = sh(["git", "-C", str(cwd) if cwd else ".", "rev-parse", "--verify", ref])
    return r.stdout.strip() if r.returncode == 0 else ref


def create_worktree_workspace(branch, *, base="main", slug=None, title=None, path=None):
    """Create a paseo-managed worktree workspace on the given branch.

    branch exists  -> checkout-branch (relaunch: continue the same branch)
    otherwise      -> branch-off from base (first launch)
    Returns {"workspaceId", "cwd"} or None.
    """
    cmd = ["workspace", "create", "--isolation", "worktree", "--json"]
    if branch and _branch_exists_git(branch):
        cmd += ["--mode", "checkout-branch", "--branch", branch]
    else:
        cmd += ["--mode", "branch-off", "--new-branch", branch,
                "--base", _resolve_ref(base, cwd=path)]
    if slug:
        cmd += ["--worktree-slug", slug]
    if title:
        cmd += ["--title", title]
    if path:
        cmd += ["--path", path]
    r = _run(cmd, timeout=240)
    if r.returncode != 0:
        return None
    try:
        d = json.loads(r.stdout)
    except Exception:
        return None
    return {
        "workspaceId": d.get("workspaceId") or d.get("id"),
        "cwd": d.get("cwd") or d.get("path"),
    }


def _branch_exists_git(branch):
    from . import gitutil
    return gitutil.branch_exists(branch)


def launch(prompt, *, model, thinking, title, workspace_id=None, cwd=None, labels=None,
           wait_seconds=90):
    """Launch a background agent into an existing workspace (or a local cwd).

    workspace_id set -> agent is placed in that (worktree) workspace.
    Otherwise        -> plain local workspace on --cwd.
    Returns (agent_id, stdout).
    """
    cmd = ["run", "--background", "--provider", "pi",
           "--model", model, "--thinking", thinking, "--title", title]
    if workspace_id:
        cmd += ["--workspace", workspace_id]
    elif cwd:
        cmd += ["--cwd", cwd]
    for kv in labels or []:
        cmd += ["--label", kv]
    cmd.append(prompt)
    r = _run(cmd, timeout=240)
    return (r.returncode, r.stdout + r.stderr)


def parse_agent_id(stdout):
    import re
    m = re.search(r"([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12})", stdout)
    if m:
        return m.group(1)
    m = re.search(r"\b([0-9a-f]{7,8})\b", stdout)
    return m.group(1) if m else None


def wait_for_workspace(name, timeout_s=90):
    """Poll workspace ls until a workspace with the given name appears."""
    deadline = time.time() + timeout_s
    while time.time() < deadline:
        w = find_workspace_by_name(name)
        if w:
            return w
        time.sleep(2)
    return None
