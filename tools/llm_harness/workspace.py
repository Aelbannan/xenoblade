from __future__ import annotations

import shutil
import subprocess
import tempfile
import threading
from pathlib import Path


class GitWorktreeManager:
    """Create detached worktrees overlaid with the caller's current dirty snapshot."""

    def __init__(self, root: Path, *, copy_dirty: bool = True) -> None:
        self.root = root.resolve()
        self.copy_dirty = copy_dirty
        self._git_lock = threading.Lock()

    def create(self, label: str) -> Path:
        path = Path(tempfile.mkdtemp(prefix=f"llm-harness-{_safe(label)}-"))
        path.rmdir()
        with self._git_lock:
            added = False
            try:
                subprocess.run(
                    ["git", "worktree", "add", "--detach", str(path), "HEAD"],
                    cwd=self.root,
                    check=True,
                    capture_output=True,
                    text=True,
                )
                added = True
                if self.copy_dirty:
                    self._overlay_dirty(path)
            except Exception:
                if added:
                    self._remove_locked(path)
                elif path.exists():
                    shutil.rmtree(path)
                raise
        return path

    def remove(self, path: Path) -> None:
        with self._git_lock:
            self._remove_locked(path)

    def _remove_locked(self, path: Path) -> None:
        completed = subprocess.run(
            ["git", "worktree", "remove", "--force", str(path)],
            cwd=self.root,
            check=False,
            capture_output=True,
            text=True,
        )
        if completed.returncode and path.exists():
            shutil.rmtree(path)
        if completed.returncode:
            subprocess.run(
                ["git", "worktree", "prune"],
                cwd=self.root,
                check=False,
                capture_output=True,
                text=True,
            )

    def _overlay_dirty(self, destination: Path) -> None:
        paths: set[str] = set()
        for command in (
            ["git", "diff", "--name-only", "-z", "HEAD"],
            ["git", "ls-files", "--others", "--exclude-standard", "-z"],
        ):
            result = subprocess.run(
                command,
                cwd=self.root,
                check=True,
                capture_output=True,
            )
            paths.update(value.decode("utf-8") for value in result.stdout.split(b"\0") if value)
        for relative in sorted(paths):
            source = self.root / relative
            target = destination / relative
            if not source.exists() and not source.is_symlink():
                if target.is_dir():
                    shutil.rmtree(target)
                else:
                    target.unlink(missing_ok=True)
                continue
            target.parent.mkdir(parents=True, exist_ok=True)
            if source.is_symlink():
                target.unlink(missing_ok=True)
                target.symlink_to(source.readlink())
            elif source.is_dir():
                shutil.copytree(source, target, dirs_exist_ok=True)
            else:
                shutil.copy2(source, target)


def _safe(value: str) -> str:
    return "".join(char if char.isalnum() else "-" for char in value)[:40]
