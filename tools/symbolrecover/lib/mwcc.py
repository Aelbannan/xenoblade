from __future__ import annotations

import re
from dataclasses import dataclass
from typing import Optional

# Length-prefixed identifier in a CodeWarrior mangled name.
MWCC_IDENT_RE = re.compile(r"(?<!\d)(\d{1,3})([A-Za-z_][A-Za-z0-9_:]*)")

ARG_DECODE = {
    "v": "void",
    "b": "bool",
    "c": "char",
    "h": "unsigned char",
    "s": "short",
    "i": "int",
    "l": "long",
    "x": "long long",
    "f": "float",
    "d": "double",
    "w": "wchar_t",
    "Uc": "unsigned char",
    "Us": "unsigned short",
    "Ui": "unsigned int",
    "Ul": "unsigned long",
    "Ux": "unsigned long long",
    "Fv": "void",
    "Fb": "bool",
    "Fc": "char",
    "Fh": "unsigned char",
    "Fs": "short",
    "Fi": "int",
    "Fl": "long",
    "Fx": "long long",
    "Ff": "float",
    "Fd": "double",
    "Fw": "wchar_t",
}


@dataclass(frozen=True)
class DemangledSymbol:
    raw: str
    function: str
    class_name: Optional[str]
    namespace: Optional[str]
    args: Optional[str]
    is_ctor: bool
    is_dtor: bool


def decode_args(args_blob: str) -> str:
    if not args_blob:
        return ""
    if args_blob in ARG_DECODE:
        return ARG_DECODE[args_blob]
    if args_blob.startswith("F") and args_blob[1:] in ARG_DECODE:
        return ARG_DECODE[args_blob[1:]]
    parts: list[str] = []
    i = 0
    while i < len(args_blob):
        if args_blob.startswith("PC", i) or args_blob.startswith("P", i):
            if args_blob.startswith("PC", i):
                parts.append("const ...*")
                i += 2
                continue
            parts.append("...*")
            i += 1
            continue
        if args_blob[i] in "FPU":
            i += 1
            continue
        if args_blob[i] in "cisbhlxfdw":
            parts.append(ARG_DECODE.get(args_blob[i], args_blob[i]))
            i += 1
            continue
        parts.append(args_blob[i])
        i += 1
    return ", ".join(parts) if parts else args_blob


def read_length_prefixed(blob: str, pos: int) -> tuple[Optional[str], int]:
    if pos >= len(blob) or not blob[pos].isdigit():
        return None, pos
    num_start = pos
    while pos < len(blob) and blob[pos].isdigit():
        pos += 1
    digit_run = blob[num_start:pos]
    for end in range(1, len(digit_run) + 1):
        length = int(digit_run[:end])
        name_start = num_start + end
        if name_start + length <= len(blob):
            name = blob[name_start : name_start + length]
            if name and (name[0].isalpha() or name[0] == "_"):
                return name, name_start + length
    return None, num_start


KNOWN_NS_PREFIXES: list[tuple[str, str]] = [
    ("Q26mpfsys", "mpfsys"),
    ("Q23LOD", "LOD"),
    ("Q22ml", "ml"),
    ("Q22cf", "cf"),
]


def extract_namespace_and_class(blob: str) -> tuple[Optional[str], Optional[str]]:
    """Parse leading Q-encoded namespace + class from a mangled fragment."""
    for prefix, namespace in KNOWN_NS_PREFIXES:
        if blob.startswith(prefix):
            class_name, _pos = read_length_prefixed(blob, len(prefix))
            return namespace, class_name
    if blob.startswith("Q"):
        pos = 1
        chunks: list[str] = []
        while pos < len(blob):
            part, pos = read_length_prefixed(blob, pos)
            if part is None:
                break
            chunks.append(part)
        if not chunks:
            return None, None
        if len(chunks) == 1:
            return chunks[0], None
        return "::".join(chunks[:-1]), chunks[-1]
    class_name, _pos = read_length_prefixed(blob, 0)
    return None, class_name


def decode_qualified_type(blob: str) -> str:
    namespace, class_name = extract_namespace_and_class(blob)
    if class_name is None:
        return blob
    if namespace:
        return f"const {namespace}::{class_name}&"
    return f"const {class_name}&"


def decode_member_tail(tail: str) -> tuple[Optional[str], Optional[str], str]:
    """Parse <len>ClassName<args> after function__ prefix."""
    class_name, pos = read_length_prefixed(tail, 0)
    if class_name is None:
        return None, None, tail
    args_blob = tail[pos:]
    if args_blob.startswith("FRC"):
        return None, class_name, "(" + decode_qualified_type(args_blob[3:]) + ")"
    if args_blob == "Fv":
        return None, class_name, "()"
    if args_blob.startswith("Fbb"):
        return None, class_name, "(bool, bool)"
    if args_blob.startswith("Fs"):
        return None, class_name, "(short)"
    if args_blob.startswith("FP") and not args_blob.startswith("FPC"):
        # FP4CPad etc. — pointer param; keep simple decode
        return None, class_name, "(" + decode_args(args_blob[1:]) + ")"
    if args_blob.startswith("F"):
        return None, class_name, "(" + decode_args(args_blob[1:]) + ")"
    return None, class_name, ""


def demangle_symbol(name: str) -> DemangledSymbol:
    is_ctor = name.startswith("__ct__")
    is_dtor = name.startswith("__dt__")
    function = name
    class_name: Optional[str] = None
    namespace: Optional[str] = None
    args: Optional[str] = None

    if is_ctor or is_dtor:
        rest = name[6:]
        if rest.endswith("Fv"):
            rest = rest[:-2]
            args = "()"
        else:
            split_at = rest.rfind("F")
            if split_at > 0:
                args = "(" + decode_args(rest[split_at:]) + ")"
                rest = rest[:split_at]
        namespace, class_name = extract_namespace_and_class(rest)
        function = "constructor" if is_ctor else "destructor"
    elif "__" in name:
        func, _, tail = name.partition("__")
        function = func or name
        if tail and tail[0].isdigit():
            namespace, class_name, args = decode_member_tail(tail)
        else:
            if tail.endswith("Fv"):
                tail_body = tail[:-2]
                args = "()"
            else:
                split_at = tail.rfind("F")
                if split_at > 0:
                    tail_body = tail[:split_at]
                    args = "(" + decode_args(tail[split_at:]) + ")"
                else:
                    tail_body = tail
                    args = ""
            namespace, class_name = extract_namespace_and_class(tail_body)
    return DemangledSymbol(
        raw=name,
        function=function,
        class_name=class_name,
        namespace=namespace,
        args=args,
        is_ctor=is_ctor,
        is_dtor=is_dtor,
    )


def mangle_length_prefix(type_name: str) -> str:
    return f"{len(type_name)}{type_name}"


def replace_type_in_symbol(symbol: str, old_type: str, new_type: str) -> str:
    """Replace a placeholder type token, updating MWCC length prefixes."""
    old_prefixed = mangle_length_prefix(old_type)
    new_prefixed = mangle_length_prefix(new_type)
    return symbol.replace(old_prefixed, new_prefixed)


def rename_mangling_compatible(old_type: str, new_type: str) -> bool:
    return len(old_type) == len(new_type)


def suggest_same_length_names(old_type: str, hint: str) -> list[str]:
    """Pad or trim *hint* to the same length as *old_type* for mangling-safe renames."""
    target_len = len(old_type)
    if len(hint) == target_len:
        return [hint]
    if len(hint) < target_len:
        padded = hint + ("X" * (target_len - len(hint)))
        return [padded]
    if len(hint) > target_len:
        return [hint[:target_len]]
    return []
