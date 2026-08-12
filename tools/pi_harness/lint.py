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
# Unknown-name placeholders: offset-style `unkN` locals/fields (unk0, unk4,
# this->unk10, unkC …) and generated unknown types/members (UnkClass_8045F564,
# UnkVirtualFunc3, CActorParam_UnkStruct2 …). These come from the generated
# ctx and mass-imported headers; NEW uses in added lines mean the model
# failed to name the entity it is writing against.
_RE_UNK_NUM = re.compile(r"\bunk[0-9A-Fa-f]+\b")
# Unk* generated names may appear as a bare type or as a member/prefix inside
# a longer identifier (CActorParam_UnkStruct2, CCBattleManager_UnkVirtualFunc9),
# so allow any word-boundary start with optional prior identifier chars.
_RE_UNK_GEN = re.compile(r"Unk(?:Class_[0-9A-Fa-f]+|VirtualFunc[0-9]+|Struct[0-9A-Fa-f]+)")
# Binary-patching escapes (PLAN.md §17.6): forbidden in source. The narrow
# linker-ADDR16 bake (bake_linker_addrs / force_symbol_relocs) is allowed.
_RE_BINPATCH = re.compile(
    r"insn_patches|insert_insns|reloc_offset_moves|postprocess_reloc_names"
)
# Fakematch-candidate patterns (source reproduces retail codegen, not retail
# source): single-instruction asm shims and the init-list side-effect trick
# (assignment inside a cast used as a value, e.g.
#   mTempSpecialHandle(reinterpret_cast<SeqSoundHandle*>(
#       mPreparedFlag = mLoadingFlag = false))
# which forces member-store ordering to match retail bytes).
_RE_ASM_INSN_MARKER = re.compile(r"\bDECOMP_ASM_INSN_(?:BEGIN|END)\b")
_RE_INIT_CAST_ONE_LINE = re.compile(
    r"(?:reinterpret_cast|static_cast)\s*<[^>]+>\s*\([^()]*\w\s*=(?!=)")
_RE_INIT_CAST_OPEN = re.compile(
    r"(?:reinterpret_cast|static_cast)\s*<[^>]+>\s*\(\s*$")
_RE_ASSIGN_OP = re.compile(r"\w\s*=(?!=)")
_RE_PRAGMA = re.compile(r"^\s*#\s*pragma\b")
_RE_IF0 = re.compile(r"^\s*#\s*if\s+0\b")
_RE_SECTION = re.compile(r"__declspec\s*\(\s*section|__attribute__\s*\(\s*\(\s*section")
_RE_ANGLE_INC = re.compile(r'^\s*#\s*include\s*<([^>]+)>')


def is_cpp(path: str) -> bool:
    """True for C++ source/header files."""
    return path.endswith((".cpp", ".cc", ".cxx", ".hpp", ".hh"))


_RE_SELF_PARAM = re.compile(r"\*\s*self\s*(?:[,)]|$)")

# SJIS-safe check: the build runs sjiswrap + mwcceppc which reject characters
# with no Shift-JIS encoding (e.g. U+2014 em-dash, U+2018/2019 curly quotes).
# Genuine Japanese (し, く, 。…) DOES have SJIS encodings and is fine. A UTF-8
# char that cannot round-trip through shift_jis will fail the build.
def _sjis_unsafe_chars(text: str) -> list[str]:
    out: list[str] = []
    for ch in text:
        if ord(ch) > 0x7F:
            try:
                ch.encode("shift_jis")
            except UnicodeEncodeError:
                if ch not in out:
                    out.append(ch)
    return out


def lint_delta(path: str, old_text: str | None,
               new_text: str) -> list[LintViolation]:
    violations: list[LintViolation] = []

    # C vs C++: this decomp fork writes free functions with `X* self` first
    # params in BOTH languages (retail `func_*` symbols are unmangled free
    # functions, so `Class::method` is impossible for them) — self params are
    # NOT a C++-vs-C discriminator here. The real differences:
    #  - `extern "C"` is C++-only syntax; it is a compile error in .c files.
    #  - Member-function style (`Class::method`) only applies to mangled
    #    member symbols, which the codebase already handles.
    is_c = path.endswith((".c", ".h")) and not is_cpp(path)

    def add(rule: str, line_no: int, line: str, why: str) -> None:
        violations.append(LintViolation(
            rule=rule, line=line_no,
            detail=f"{why}: `{line.strip()[:100]}`"))

    cast_pending = False
    for line_no, raw in _added_lines(old_text, new_text):
        code = _strip_line_comment(raw)

        # SJIS safety: characters with no Shift-JIS encoding break the build
        # (sjiswrap rejects them). Check the raw line INCLUDING comments.
        bad = _sjis_unsafe_chars(raw)
        if bad:
            add("non_sjis_char", line_no, raw,
                f"character(s) with no Shift-JIS encoding (will fail the "
                f"build): {''.join(bad)} — replace with ASCII or valid SJIS")

        # C-only: `extern "C"` is C++ syntax — illegal in C source.
        if is_c and _RE_EXTERN_C.search(code):
            add("extern_c_in_c", line_no, raw,
                '`extern "C"` is C++-only syntax and does not compile in C '
                "files — remove it")

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
        if _RE_BINPATCH.search(raw):
            add("no_binary_patching", line_no, raw,
                "binary-patching escapes (insn_patches / insert_insns / "
                "reloc_offset_moves / postprocess_reloc_names) are forbidden — "
                "chase EQUIVALENT_MATCH, not byte-identity patches")
        if _RE_EXTERN_C.search(raw) and "lbl_" not in raw:
            add("no_extern_c", line_no, raw,
                'extern "C" is only allowed for lbl_* reloc names')

        # C++-only: constructors/destructors must be member functions, never
        # C-style free functions taking `* self` (extern "C" or not). The
        # retail symbols are mangled members (__ct__/__dt__), so writing them
        # as `Class::Class(...)` is always possible.
        if is_cpp(path) and re.search(r"__[cd]t__", code) and "::" not in code \
           and _RE_SELF_PARAM.search(code):
            add("cpp_free_ctor", line_no, raw,
                "constructor/destructor written as a C-style free function "
                "taking `* self` — use a member constructor/destructor "
                "(Class::Class / Class::~Class) instead")
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
        if _RE_ASM_INSN_MARKER.search(code):
            add("no_asm_insn_shim", line_no, raw,
                "DECOMP_ASM_INSN single-instruction asm shims are forbidden in "
                "new code — write high-level C++ and chase a natural match")
        # Init-list side-effect trick: assignment inside a cast used as a
        # value. One-line form matches directly; a cast opened at end-of-line
        # may close on a later added line, so track it across added lines.
        if _RE_INIT_CAST_ONE_LINE.search(code):
            add("no_init_side_effect", line_no, raw,
                "assignment inside a cast (init-list store-ordering trick) is "
                "forbidden — write the plain value")
        if not cast_pending and _RE_INIT_CAST_OPEN.search(code):
            cast_pending = True
        if cast_pending:
            if _RE_ASSIGN_OP.search(code):
                add("no_init_side_effect", line_no, raw,
                    "assignment inside a multi-line cast / member-initializer "
                    "is forbidden — write the plain value")
            if ")" in code:
                cast_pending = False
        if _RE_REGISTER_KW.search(code):
            add("no_register_keyword", line_no, raw,
                "register keyword / register bindings are forbidden")
        if _RE_REG_NAME.search(code) and _RE_DECL_LIKE.search(code):
            add("no_register_names", line_no, raw,
                "identifiers named after GPRs (r0-r31) are forbidden")
        if _RE_VOID_PTR.search(code):
            add("no_void_ptr", line_no, raw,
                "void* is forbidden; use a proper struct/class type")
        if _RE_UNK_NUM.search(code):
            add("no_unk_name", line_no, raw,
                "unknown-name placeholder `unkN` is forbidden in new code; "
                "name the variable/field from the retail symbol or struct "
                "layout")
        if _RE_UNK_GEN.search(code):
            add("no_unk_generated", line_no, raw,
                "generated unknown type/member (`UnkClass_*`, `UnkVirtualFunc*`, "
                "`UnkStruct*`) is forbidden in new code; use a real struct/class "
                "name from the headers or declare it properly")
        if _RE_CAST.search(code) and _RE_HEX_OFF.search(code):
            add("no_offset_arithmetic", line_no, raw,
                "raw pointer offset arithmetic is forbidden; use struct "
                "fields")

    return violations
