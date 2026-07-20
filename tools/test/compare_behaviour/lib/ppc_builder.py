from __future__ import annotations

import os
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable

from tools.test.compare_behaviour.lib.manifest import BehaviourTest
from tools.test.compare_behaviour.lib.paths import repo_root

PPC_DIR = Path(__file__).resolve().parents[1] / "ppc"
LINKER_VERSION = "Wii/1.1"
BEHAVIOUR_RESULT_MAGIC = 0xBEEFCAFE
CVIEW_UNIT = "monolib/src/core/CView"
CVIEWROOT_UNIT = "monolib/src/core/CViewRoot"
CVIEW_RETAIL_SLICE_SOURCE = PPC_DIR / "cview_ppc_retail.c"
CVIEWROOT_RETAIL_SLICE_SOURCE = PPC_DIR / "cviewroot_ppc_retail.c"
CVIEWROOT_DECOMP_SLICE_SOURCE = PPC_DIR / "cviewroot_ppc_decomp.c"
CVIEW_THUNKS_SOURCE = PPC_DIR / "cview_ppc_thunks.c"


@dataclass
class PpcBuildResult:
    dol: Path
    elf: Path
    map_path: Path | None
    ok: bool
    output: str


def _tool_paths(root: Path) -> tuple[Path, Path, Path, Path, Path, Path]:
    wibo = root / "build" / "tools" / "wibo"
    mwcc = root / "build" / "compilers" / LINKER_VERSION / "mwcceppc.exe"
    mwld = root / "build" / "compilers" / LINKER_VERSION / "mwldeppc.exe"
    objcopy = root / "build" / "binutils" / "powerpc-eabi-objcopy"
    dtk = root / "build" / "tools" / "dtk"
    ldscript = PPC_DIR / "ldscript.lcf"
    for path in (wibo, mwcc, mwld, objcopy, dtk, ldscript):
        if not path.exists():
            raise FileNotFoundError(f"PPC build prerequisite missing: {path}")
    return wibo, mwcc, mwld, objcopy, dtk, ldscript


def _mwcc_flags(root: Path, region: str) -> list[str]:
    return [
        "-nodefaults",
        "-proc",
        "gekko",
        "-align",
        "powerpc",
        "-enum",
        "int",
        "-fp",
        "hardware",
        "-Cpp_exceptions",
        "on",
        "-O4,p",
        "-inline",
        "auto",
        '-pragma',
        '"cats off"',
        "-maxerrors",
        "1",
        "-nosyspath",
        "-RTTI",
        "on",
        "-fp_contract",
        "on",
        "-str",
        "reuse",
        "-enc",
        "SJIS",
        "-ipa",
        "file",
        "-use_lmw_stmw",
        "on",
        "-str",
        "reuse,pool,readonly",
        "-func_align",
        "4",
        "-i",
        str(root / "include"),
        "-i",
        str(root / "libs" / "PowerPC_EABI_Support" / "include" / "stl"),
        "-i",
        str(root / "libs" / "PowerPC_EABI_Support" / "include"),
        "-i",
        str(root / f"build/{region}/include"),
        "-i",
        str(root / "src"),
        "-i",
        str(root / "libs" / "monolib" / "include"),
        "-i",
        str(root / "libs" / "nw4r" / "include"),
        "-i",
        str(root / "libs" / "RVL_SDK" / "include"),
        "-i",
        str(root / "libs" / "CriWare" / "include"),
        "-i",
        str(PPC_DIR),
        f"-DBUILD_VERSION=2",
        f"-DVERSION_{region.upper()}",
        "-DNDEBUG=1",
    ]


def _mwld_flags(ldscript: Path) -> list[str]:
    return [
        "-fp",
        "hardware",
        "-nodefaults",
        "-lcf",
        str(ldscript),
        "-m",
        "__start",
    ]


def _run(cmd: list[str], *, cwd: Path) -> subprocess.CompletedProcess[str]:
    return subprocess.run(cmd, cwd=cwd, check=False, capture_output=True, text=True)


def _prefix_object(objcopy: Path, src: Path, dst: Path, prefix: str) -> None:
    subprocess.run([str(objcopy), f"--prefix-symbols={prefix}", str(src), str(dst)], check=True)


def _trim_object_to_symbol(objcopy: Path, src: Path, dst: Path, symbol: str) -> None:
    shutil.copy2(src, dst)
    subprocess.run(
        [str(objcopy), f"-G{symbol}", "--strip-unneeded", str(dst)],
        check=True,
    )


def _compile_retail_slice(
    wibo: Path,
    mwcc: Path,
    objcopy: Path,
    flags: list[str],
    retail_obj: Path,
    symbol: str,
    out_obj: Path,
    *,
    root: Path,
) -> None:
    if symbol == "getCurrentView__5CViewFv":
        proc = _compile_c_source(wibo, mwcc, flags, CVIEW_RETAIL_SLICE_SOURCE, out_obj, root=root)
        if proc.returncode != 0:
            raise RuntimeError((proc.stdout + proc.stderr).strip())
        return

    prefixed = out_obj.parent / "retail_prefixed_full.o"
    _prefix_object(objcopy, retail_obj, prefixed, "rb_")
    _trim_object_to_symbol(objcopy, prefixed, out_obj, f"rb_{symbol}")


def _unit_stub_path(unit: str) -> Path:
    return PPC_DIR / "stubs" / f"{unit.replace('/', '_')}.c"


def _compile_c_source(
    wibo: Path,
    mwcc: Path,
    flags: list[str],
    src: Path,
    out_obj: Path,
    *,
    root: Path,
) -> subprocess.CompletedProcess[str]:
    compile_flags = list(flags)
    if src.suffix == ".c":
        compile_flags.extend(["-lang", "c99", "-gccinc"])
    elif src.suffix == ".cpp":
        compile_flags.extend(["-Cpp_exceptions", "off", "-RTTI", "off"])
    return _run([str(wibo), str(mwcc), *compile_flags, "-c", str(src), "-o", str(out_obj)], cwd=root)


def _postprocess_retail_object(objcopy: Path, retail_prefixed: Path) -> None:
    subprocess.run(
        [
            str(objcopy),
            "--redefine-sym",
            "rb_@LOCAL@getInstance__Q22ml6MTRandFv@instance_806561E0=rb_mtrand_singleton",
            str(retail_prefixed),
        ],
        check=True,
    )


def _postprocess_stub_object(
    objcopy: Path,
    src_obj: Path,
    dst_obj: Path,
    *,
    stub_src: Path | None = None,
) -> None:
    shutil.copy2(src_obj, dst_obj)


def _blr_pool_object(
    wibo: Path,
    mwcc: Path,
    objcopy: Path,
    flags: list[str],
    obj_dir: Path,
    *,
    root: Path,
) -> Path | None:
    pool_src = PPC_DIR / "cview_blr_pool.c"
    if not pool_src.is_file():
        return None
    raw_obj = obj_dir / "cview_blr_pool.raw.o"
    alias_obj = obj_dir / "cview_blr_pool.o"
    proc = _compile_c_source(wibo, mwcc, flags, pool_src, raw_obj, root=root)
    if proc.returncode != 0:
        return None
    subprocess.run(
        [
            str(objcopy),
            "--add-symbol",
            "rb___dt__11reslist<Ul>Fv=cview_blr_pool,global",
            "--add-symbol",
            "dc___dt__11reslist<Ul>Fv=cview_blr_pool,global",
            str(raw_obj),
            str(alias_obj),
        ],
        check=True,
    )
    return alias_obj


def _uses_cview_semantic_slices(test: BehaviourTest) -> bool:
    """CView tests that link hand-written rb_/dc_ slices instead of trimmed CView.o."""
    return test.unit == CVIEW_UNIT and bool(test.ppc_stubs)


def _uses_semantic_slices(test: BehaviourTest) -> bool:
    """Hand-written rb_/dc_ slices in ppc_stubs — do not link full unit .o."""
    return bool(test.ppc_stubs)


def _extra_stub_sources(test: BehaviourTest) -> list[Path]:
    paths: list[Path] = []
    if test.ppc_stubs:
        for name in test.ppc_stubs:
            path = PPC_DIR / name
            if path.is_file():
                paths.append(path)
    # getCurrentView uses a minimal stub set (no unit stub). Other CView slice tests
    # still need monolib_src_core_CView.c for CSplitFrame / CViewRoot mocks.
    if test.unit == CVIEW_UNIT and test.id == "cview-get-current-view":
        return paths
    unit_stub = _unit_stub_path(test.unit)
    if unit_stub.is_file() and unit_stub not in paths:
        paths.append(unit_stub)
    return paths


def ppc_source_for_test(test: BehaviourTest) -> Path | None:
    if not test.ppc_source:
        return None
    path = PPC_DIR / test.ppc_source
    return path if path.is_file() else None


def ppc_build_dir(region: str, test_id: str) -> Path:
    return repo_root() / "build" / region / "test" / "compare_behaviour" / "ppc" / test_id


def build_ppc_test(test: BehaviourTest) -> PpcBuildResult:
    root = repo_root()
    wibo, mwcc, mwld, objcopy, dtk, ldscript = _tool_paths(root)
    source = ppc_source_for_test(test)
    if source is None:
        raise FileNotFoundError(f"No PPC harness source for test '{test.id}'")

    out_dir = ppc_build_dir(test.region, test.id)
    obj_dir = out_dir / "obj"
    if out_dir.exists():
        shutil.rmtree(out_dir)
    obj_dir.mkdir(parents=True)

    retail_obj = root / "build" / test.region / "obj" / f"{test.unit}.o"
    decomp_obj = root / "build" / test.region / "src" / f"{test.unit}.o"
    for path in (retail_obj, decomp_obj):
        if not path.is_file():
            raise FileNotFoundError(f"Missing object for PPC link: {path}")

    obj_name = Path(f"{test.unit}.o").name
    if obj_name in {"MTRand.o", "CfPadTask.o", "CDeviceGX.o"}:
        postprocess = root / "tools" / "postprocess_reloc_names.py"
        if postprocess.is_file():
            subprocess.run([sys.executable, str(postprocess), str(decomp_obj)], cwd=root, check=False)

    retail_prefixed = obj_dir / "retail_prefixed.o"
    decomp_prefixed = obj_dir / "decomp_prefixed.o"
    _prefix_object(objcopy, retail_obj, retail_prefixed, "rb_")
    _prefix_object(objcopy, decomp_obj, decomp_prefixed, "dc_")
    if test.unit == "monolib/src/math/MTRand":
        _postprocess_retail_object(objcopy, retail_prefixed)

    flags = _mwcc_flags(root, test.region)
    sources = [
        PPC_DIR / "crt0.c",
        PPC_DIR / "stubs.c",
        PPC_DIR / "behaviour_report.c",
        source,
    ]
    if test.unit == CVIEW_UNIT and test.id != "cview-get-current-view":
        sources.append(CVIEW_THUNKS_SOURCE)
    if test.unit == CVIEWROOT_UNIT:
        sources.append(CVIEWROOT_RETAIL_SLICE_SOURCE)
        sources.append(CVIEWROOT_DECOMP_SLICE_SOURCE)
    objects: list[Path] = []
    for src in sources:
        out_obj = obj_dir / f"{src.stem}.o"
        proc = _compile_c_source(wibo, mwcc, flags, src, out_obj, root=root)
        if proc.returncode != 0:
            return PpcBuildResult(
                dol=out_dir / "test.dol",
                elf=out_dir / "test.elf",
                map_path=None,
                ok=False,
                output=(proc.stdout + proc.stderr).strip(),
            )
        objects.append(out_obj)

    for stub_src in _extra_stub_sources(test):
        raw_obj = obj_dir / f"{stub_src.stem}.raw.o"
        final_obj = obj_dir / f"{stub_src.stem}.o"
        proc = _compile_c_source(wibo, mwcc, flags, stub_src, raw_obj, root=root)
        if proc.returncode != 0:
            return PpcBuildResult(
                dol=out_dir / "test.dol",
                elf=out_dir / "test.elf",
                map_path=None,
                ok=False,
                output=(proc.stdout + proc.stderr).strip(),
            )
        _postprocess_stub_object(objcopy, raw_obj, final_obj, stub_src=stub_src)
        objects.append(final_obj)

    if test.unit == CVIEWROOT_UNIT:
        pass
    elif test.unit == CVIEW_UNIT:
        if not _uses_cview_semantic_slices(test):
            decomp_slice = obj_dir / "decomp_slice.o"
            _trim_object_to_symbol(objcopy, decomp_prefixed, decomp_slice, f"dc_{test.symbol}")
            objects.append(decomp_slice)
            retail_slice = obj_dir / "retail_slice.o"
            try:
                _compile_retail_slice(
                    wibo, mwcc, objcopy, flags, retail_obj, test.symbol, retail_slice, root=root
                )
            except RuntimeError as exc:
                return PpcBuildResult(
                    dol=out_dir / "test.dol",
                    elf=out_dir / "test.elf",
                    map_path=None,
                    ok=False,
                    output=str(exc),
                )
            objects.append(retail_slice)
    elif _uses_semantic_slices(test):
        # CViewFrame / other units: ppc_stubs supply rb_/dc_ — skip full prefixed .o.
        pass
    else:
        objects.extend([retail_prefixed, decomp_prefixed])

    link_fixes = root / "tools" / "postprocess_link_fixes.py"
    if link_fixes.is_file():
        subprocess.run([sys.executable, str(link_fixes)], cwd=root, check=False)

    runtime_dir = root / "build" / test.region / "src" / "PowerPC_EABI_Support" / "src" / "Runtime"
    runtime_objects = sorted(runtime_dir.glob("*.o")) if runtime_dir.is_dir() else []
    runtime_objects.append(
        root / "build" / test.region / "src" / "PowerPC_EABI_Support" / "src" / "MSL_C" / "MSL_Common" / "mem.o"
    )
    for extra in runtime_objects:
        if extra.is_file():
            objects.append(extra)

    elf_path = out_dir / "test.elf"
    dol_path = out_dir / "test.dol"
    map_path = out_dir / "test.map"
    rsp_path = out_dir / "test.rsp"
    rsp_path.write_text("\n".join(str(o) for o in objects) + "\n", encoding="utf-8")

    link_cmd = [str(wibo), str(mwld), *_mwld_flags(ldscript), "-map", str(map_path), "-o", str(elf_path), f"@{rsp_path}"]
    link_proc = _run(link_cmd, cwd=root)
    if link_proc.returncode != 0:
        return PpcBuildResult(
            dol=dol_path,
            elf=elf_path,
            map_path=map_path if map_path.is_file() else None,
            ok=False,
            output=(link_proc.stdout + link_proc.stderr).strip(),
        )

    dol_proc = _run([str(dtk), "elf2dol", str(elf_path), str(dol_path)], cwd=root)
    output = (link_proc.stdout + link_proc.stderr + dol_proc.stdout + dol_proc.stderr).strip()
    ok = dol_proc.returncode == 0 and dol_path.is_file()
    return PpcBuildResult(
        dol=dol_path,
        elf=elf_path,
        map_path=map_path if map_path.is_file() else None,
        ok=ok,
        output=output,
    )
