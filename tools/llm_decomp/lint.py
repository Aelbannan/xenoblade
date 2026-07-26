"""Anti-cheat lint gate — operates on the patch DELTA, never whole files.

Design reference: docs/llm_decomp_design.md §5 (review #7: delta-only).

Added lines are computed with difflib opcodes; each rule is a regex/check
applied to those lines. Violated rule ids are returned so the session loop
can feed them back to the model as tool feedback.
"""
from __future__ import annotations

import difflib
import re
from dataclasses import dataclass

_ANGLE_INCLUDE_WHITELIST = {
    "macros.h", "types.h", "decomp.h", "string.h", "stddef.h", "stdarg.h",
    "math.h", "float.h", "limits.h", "ctype.h", "stdlib.h", "stdio.h", "new",
}


@dataclass
class LintViolation:
    rule: str  # stable snake_case rule id
    detail: str  # what was matched + line snippet
    line: int | None  # 1-indexed line in the NEW text, if known


def _added_lines(old_text: str | None,
                 new_text: str) -> list[tuple[int, str]]:
    """(1-indexed line in new_text, content) for inserted/replaced lines."""
    if old_text is None:
        return list(enumerate(new_text.splitlines(), 1))
    sm = difflib.SequenceMatcher(a=old_text.splitlines(),
                                 b=new_text.splitlines(),
                                 autojunk=False)
    out: list[tuple[int, str]] = []
    for tag, _a0, _a1, b0, b1 in sm.get_opcodes():
        if tag in ("insert", "replace"):
            out.extend((i + 1, new_text.splitlines()[i])
                       for i in range(b0, b1))
    return out


def _strip_line_comment(line: str) -> str:
    idx = line.find("//")
    return line if idx < 0 else line[:idx]


_RE_ASM = re.compile(r"\basm\b|__asm")
_RE_S_INCLUDE = re.compile(r'#\s*include\s*[<"][^>"]+\.s[>"]')
_RE_REGISTER_KW = re.compile(r"\bregister\b|asm\s*\(\s*\"r")
_RE_DECL_LIKE = re.compile(
    r"^\s*(?:u8|u16|u32|u64|s8|s16|s32|s64|f32|f64|int|char|short|long|"
    r"float|double|bool|void|unsigned|signed|const|static|struct|class|"
    r"[A-Z]\w*(?:\s*[*&])?)\s")
_RE_REG_NAME = re.compile(r"\br(?:3[01]|[12][0-9]|[0-9])\b")
_RE_EXTERN_C = re.compile(r'extern\s+"C"')
_RE_VOID_PTR = re.compile(r"\(\s*void\s*\*\s*\)|void\s*\*\s*\w+")
_RE_VOLATILE_ARR = re.compile(
    r"\bvolatile\b[^;]*\b(?:char|u8)\b[^;]*\[|"
    r"\b(?:u8|char)\s+(?:sp|stack)\d*\s*\[\s*(?:0x[0-9A-Fa-f]+|\d+)")
_RE_CAST = re.compile(r"\(\s*\w+\s*\*\s*\)")
_RE_HEX_OFF = re.compile(r"\+\s*0x[0-9A-Fa-f]+")
_RE_CODEGEN = re.compile(r"DECOMP_PPC_|DECOMP_FORCELITERAL|DECOMP_FORCEACTIVE")
_RE_PRAGMA = re.compile(r"^\s*#\s*pragma\b")
_RE_IF0 = re.compile(r"^\s*#\s*if\s+0\b")
_RE_SECTION = re.compile(r"__declspec\s*\(\s*section|__attribute__\s*\(\s*\(\s*section")
_RE_ANGLE_INC = re.compile(r'^\s*#\s*include\s*<([^>]+)>')


def lint_delta(path: str, old_text: str | None,
               new_text: str) -> list[LintViolation]:
    violations: list[LintViolation] = []

    def add(rule: str, line_no: int, line: str, why: str) -> None:
        violations.append(LintViolation(
            rule=rule, line=line_no,
            detail=f"{why}: `{line.strip()[:100]}`"))

    for line_no, raw in _added_lines(old_text, new_text):
        code = _strip_line_comment(raw)

        if _RE_PRAGMA.search(raw):
            add("no_pragmas", line_no, raw,
                "new preprocessor pragmas are forbidden (codegen steering)")
        if _RE_IF0.search(raw):
            add("no_if0", line_no, raw,
                "#if 0 wrapping is forbidden (can hide symbols from diffs)")
        if _RE_SECTION.search(raw):
            add("no_section_attr", line_no, raw,
                "section attributes are forbidden")
        if _RE_CODEGEN.search(raw):
            add("no_codegen_macros", line_no, raw,
                "DECOMP_* codegen-steering macros are forbidden")
        if _RE_EXTERN_C.search(raw) and "lbl_" not in raw:
            add("no_extern_c", line_no, raw,
                'extern "C" is only allowed for lbl_* reloc names')
        m = _RE_ANGLE_INC.search(raw)
        if m and m.group(1) not in _ANGLE_INCLUDE_WHITELIST:
            add("no_angle_include", line_no, raw,
                f"angle include <{m.group(1)}> is not whitelisted")
        if _RE_S_INCLUDE.search(raw):
            add("no_asm", line_no, raw, ".s includes are forbidden")
        if _RE_VOLATILE_ARR.search(raw):
            add("no_volatile_fake_stack", line_no, raw,
                "volatile/fake stack buffers are forbidden")

        if not code.strip():
            continue  # pure comment line
        if _RE_ASM.search(code):
            add("no_asm", line_no, raw,
                "assembly is forbidden, including single-instruction asm")
        if _RE_REGISTER_KW.search(code):
            add("no_register_keyword", line_no, raw,
                "register keyword / register bindings are forbidden")
        if _RE_REG_NAME.search(code) and _RE_DECL_LIKE.search(code):
            add("no_register_names", line_no, raw,
                "identifiers named after GPRs (r0-r31) are forbidden")
        if _RE_VOID_PTR.search(code):
            add("no_void_ptr", line_no, raw,
                "void* is forbidden; use a proper struct/class type")
        if _RE_CAST.search(code) and _RE_HEX_OFF.search(code):
            add("no_offset_arithmetic", line_no, raw,
                "raw pointer offset arithmetic is forbidden; use struct "
                "fields")

    return violations
