from __future__ import annotations

import subprocess
from dataclasses import dataclass
from pathlib import Path

from tools.test.compare_behaviour.lib.manifest import BehaviourTest
from tools.test.compare_behaviour.lib.paths import BUILD_TEST_DIR, HOST_DIR, repo_root

@dataclass
class HostCompareResult:
    binary: Path
    ok: bool
    output: str


def _compile_flags(include_root: Path) -> list[str]:
    return [
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-Wno-unused-parameter",
        "-DNONMATCHING",
        "-DCOMPAT_ANY",
        "-DNDEBUG=1",
        "-DBUILD_VERSION=2",
        "-DVERSION_US",
        f"-I{include_root / 'include'}",
        f"-I{include_root / 'libs' / 'monolib' / 'include'}",
        f"-I{include_root / 'src'}",
    ]


def build_host_binary(name: str) -> Path:
    root = repo_root()
    BUILD_TEST_DIR.mkdir(parents=True, exist_ok=True)
    source = HOST_DIR / f"{name}.cpp"
    if not source.is_file():
        raise FileNotFoundError(f"Host test source missing: {source}")

    out = BUILD_TEST_DIR / name
    cmd = ["clang++", *_compile_flags(root), str(source), "-o", str(out)]
    subprocess.run(cmd, cwd=root, check=True)
    return out


def run_host_compare(test: BehaviourTest) -> HostCompareResult | None:
    if not test.host_binary:
        return None

    binary_name = test.host_binary
    if not binary_name:
        return None
    binary = build_host_binary(binary_name)
    proc = subprocess.run([str(binary)], cwd=repo_root(), check=False, capture_output=True, text=True)
    output = proc.stdout + proc.stderr
    return HostCompareResult(binary=binary, ok=proc.returncode == 0, output=output.strip())
