from __future__ import annotations

import hashlib
import json
import re
import sqlite3
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Iterator, Optional, Sequence


SCHEMA_VERSION = 1

TAG_PATTERNS: dict[str, re.Pattern[str]] = {
    "abi": re.compile(r"\bABI\b|calling convention|fake[- ]`?Fv|extra arguments", re.I),
    "compiler_flags": re.compile(r"pragma|(?:^|\s)-O4|compiler flag|optimization level", re.I),
    "control_flow": re.compile(r"control flow|branch (?:layout|shape|order)|fallthrough|if/else|goto", re.I),
    "inlining": re.compile(r"\binline(?:d|s|ing)?\b|\bIPA\b|helper boundar", re.I),
    "literal_pool": re.compile(r"literal pool|string pool|float pool|\.sdata2|@stringBase|@N\b", re.I),
    "loop_codegen": re.compile(r"\bloop\b|\bmtctr\b|\bbdnz\b|unroll|do-while|induction", re.I),
    "policy_exception": re.compile(r"policy exception|§17\.6|insn_patches|inline asm|postprocess", re.I),
    "regalloc": re.compile(r"regalloc|register alloc|register color|Chaitin|regswap|callee-save|GPR|FPR", re.I),
    "relocation": re.compile(r"reloc|R_PPC_|@sda21|SDA\b|symbol name drift|extern \"C\"", re.I),
    "semantics": re.compile(r"semantic|behaviou?r|logic bug|correctness", re.I),
    "size": re.compile(r"split budget|size PASS|\.text|function size|over budget|exact size", re.I),
    "stack_frame": re.compile(r"stack (?:frame|home|slot)|frame size|prologue|epilogue|spill", re.I),
    "struct_layout": re.compile(r"struct layout|class layout|field offset|padding|\bstride\b|subobject", re.I),
    "switch_codegen": re.compile(r"\bswitch\b|jump table|case order|decision tree", re.I),
    "vtable": re.compile(r"vtable|virtual (?:call|slot|dispatch)|__vt__|\bvt\+", re.I),
}

STATUS_RE = re.compile(
    r"\b(EQUIVALENT_MATCH|FULL_MATCH|CODE_MATCH|HIGH_MATCH|STRUCTURAL|COMPILES|NOT_STARTED)\b"
)
PERCENT_RE = re.compile(r"(?<![\w.])(~?\d{1,3}(?:\.\d+)?)%")
HEADING_RE = re.compile(r"^(#{2,3})\s+(.+?)\s*$")
CODE_TOKEN_RE = re.compile(r"`([^`\n]+)`")


@dataclass(frozen=True)
class KnowledgeEntry:
    id: str
    source_kind: str
    source_path: str
    line_start: int
    line_end: int
    section: str
    title: str
    body: str
    functions: str = ""
    status: str = ""
    match_percent: Optional[float] = None
    target_id: str = ""
    symbol: str = ""
    region: str = ""
    unit: str = ""
    tags: tuple[str, ...] = ()


def _stable_id(prefix: str, value: str, occurrence: int) -> str:
    digest = hashlib.sha1(value.encode("utf-8")).hexdigest()[:10]
    suffix = f":{occurrence}" if occurrence > 1 else ""
    return f"{prefix}:{digest}{suffix}"


def _plain_heading(value: str) -> str:
    value = re.sub(r"\*+", "", value)
    value = value.replace("`", "")
    return value.strip()


def infer_tags(text: str) -> tuple[str, ...]:
    return tuple(name for name, pattern in TAG_PATTERNS.items() if pattern.search(text))


def _detect_status(text: str) -> str:
    matches = STATUS_RE.findall(text)
    if not matches:
        return ""
    priority = {
        "EQUIVALENT_MATCH": 7,
        "FULL_MATCH": 6,
        "CODE_MATCH": 5,
        "HIGH_MATCH": 4,
        "STRUCTURAL": 3,
        "COMPILES": 2,
        "NOT_STARTED": 1,
    }
    return max(matches, key=priority.__getitem__)


def _detect_percent(text: str) -> Optional[float]:
    values = [float(value.lstrip("~")) for value in PERCENT_RE.findall(text)]
    return values[-1] if values else None


def _reference_evidence(raw_title: str, body: str, *, level: int) -> tuple[str, Optional[float]]:
    title_status = _detect_status(raw_title)
    title_percent = _detect_percent(raw_title)
    if level == 2:
        return title_status, title_percent
    result_lines = [
        line
        for line in body.splitlines()
        if re.match(r"^\s*\*\*(?:Result|Status)[^:]*:\*\*", line, re.I)
    ]
    result = result_lines[-1] if result_lines else ""
    first_paragraph = body.split("\n\n", 1)[0]
    status = title_status or _detect_status(result) or _detect_status(first_paragraph)
    match_percent = title_percent
    if match_percent is None:
        match_percent = _detect_percent(result) if result else _detect_percent(first_paragraph)
    if not status and title_percent is None:
        match_percent = None
    if status == "FULL_MATCH":
        match_percent = 100.0
    return status, match_percent


def parse_reference(path: Path, *, source_path: Optional[str] = None) -> Iterator[KnowledgeEntry]:
    lines = path.read_text(encoding="utf-8").splitlines()
    headings: list[tuple[int, int, str]] = []
    for index, line in enumerate(lines, 1):
        match = HEADING_RE.match(line)
        if match:
            headings.append((index, len(match.group(1)), match.group(2)))

    current_section = ""
    title_occurrences: dict[str, int] = {}
    display_path = source_path or path.as_posix()
    for pos, (line_start, level, raw_title) in enumerate(headings):
        line_end = headings[pos + 1][0] - 1 if pos + 1 < len(headings) else len(lines)
        title = _plain_heading(raw_title)
        title_occurrences[title] = title_occurrences.get(title, 0) + 1
        if level == 2:
            current_section = title
        body = "\n".join(lines[line_start:line_end]).strip()
        combined = f"{raw_title}\n{body}"
        code_tokens = [token for token in CODE_TOKEN_RE.findall(combined) if len(token) <= 160]
        status, match_percent = _reference_evidence(raw_title, body, level=level)
        yield KnowledgeEntry(
            id=_stable_id("ref", title, title_occurrences[title]),
            source_kind="reference",
            source_path=display_path,
            line_start=line_start,
            line_end=line_end,
            section=current_section,
            title=title,
            body=body,
            functions=" ".join(dict.fromkeys(code_tokens)),
            status=status,
            match_percent=match_percent,
            tags=infer_tags(combined),
        )


def parse_attempts(path: Path, *, source_path: Optional[str] = None) -> Iterator[KnowledgeEntry]:
    display_path = source_path or path.as_posix()
    for line_number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        if not line.strip():
            continue
        try:
            row = json.loads(line)
        except json.JSONDecodeError:
            continue
        target_id = str(row.get("target_id", ""))
        function = str(row.get("function", ""))
        hypothesis = str(row.get("hypothesis", ""))
        next_change = str(row.get("next_change", ""))
        notes = str(row.get("notes", ""))
        body_parts = [
            f"Hypothesis: {hypothesis}" if hypothesis else "",
            f"Next change: {next_change}" if next_change else "",
            f"Notes: {notes}" if notes else "",
            f"Runtime test: {row.get('runtime_test')}" if row.get("runtime_test") else "",
        ]
        body = "\n".join(part for part in body_parts if part)
        combined = f"{function}\n{body}"
        percent = row.get("instruction_match")
        try:
            match_percent = float(percent) if percent is not None else None
        except (TypeError, ValueError):
            match_percent = None
        yield KnowledgeEntry(
            id=f"attempt:{target_id or 'unknown'}:{line_number}",
            source_kind="attempt",
            source_path=display_path,
            line_start=line_number,
            line_end=line_number,
            section="Attempt history",
            title=function or target_id or f"Attempt {line_number}",
            body=body,
            functions=function,
            status=str(row.get("status", "")),
            match_percent=match_percent,
            target_id=target_id,
            symbol=str(row.get("symbol", "") or ""),
            region=str(row.get("region", "") or ""),
            unit=str(row.get("unit", "") or ""),
            tags=infer_tags(combined),
        )


def connect(path: Path) -> sqlite3.Connection:
    connection = sqlite3.connect(path)
    connection.row_factory = sqlite3.Row
    return connection


def build_database(
    database: Path,
    reference: Path,
    attempts: Optional[Path] = None,
    *,
    root: Optional[Path] = None,
) -> int:
    database.parent.mkdir(parents=True, exist_ok=True)
    if database.exists():
        database.unlink()
    reference_name = reference.relative_to(root).as_posix() if root else reference.as_posix()
    entries = list(parse_reference(reference, source_path=reference_name))
    if attempts and attempts.is_file():
        attempts_name = attempts.relative_to(root).as_posix() if root else attempts.as_posix()
        entries.extend(parse_attempts(attempts, source_path=attempts_name))

    with connect(database) as connection:
        connection.executescript(
            """
            CREATE TABLE metadata (key TEXT PRIMARY KEY, value TEXT NOT NULL);
            CREATE TABLE entries (
                rowid INTEGER PRIMARY KEY,
                id TEXT NOT NULL UNIQUE,
                source_kind TEXT NOT NULL,
                source_path TEXT NOT NULL,
                line_start INTEGER NOT NULL,
                line_end INTEGER NOT NULL,
                section TEXT NOT NULL,
                title TEXT NOT NULL,
                body TEXT NOT NULL,
                functions TEXT NOT NULL,
                status TEXT NOT NULL,
                match_percent REAL,
                target_id TEXT NOT NULL,
                symbol TEXT NOT NULL,
                region TEXT NOT NULL,
                unit TEXT NOT NULL,
                tags TEXT NOT NULL
            );
            CREATE VIRTUAL TABLE entries_fts USING fts5(
                title, body, functions, symbol, tags,
                content='entries', content_rowid='rowid',
                tokenize='unicode61 tokenchars _'
            );
            """
        )
        connection.executemany(
            """
            INSERT INTO entries (
                id, source_kind, source_path, line_start, line_end, section,
                title, body, functions, status, match_percent, target_id,
                symbol, region, unit, tags
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
            """,
            [
                (
                    entry.id,
                    entry.source_kind,
                    entry.source_path,
                    entry.line_start,
                    entry.line_end,
                    entry.section,
                    entry.title,
                    entry.body,
                    entry.functions,
                    entry.status,
                    entry.match_percent,
                    entry.target_id,
                    entry.symbol,
                    entry.region,
                    entry.unit,
                    " ".join(entry.tags),
                )
                for entry in entries
            ],
        )
        connection.execute("INSERT INTO entries_fts(entries_fts) VALUES ('rebuild')")
        connection.executemany(
            "INSERT INTO metadata(key, value) VALUES (?, ?)",
            [
                ("schema_version", str(SCHEMA_VERSION)),
                ("reference_mtime_ns", str(reference.stat().st_mtime_ns)),
                (
                    "attempts_mtime_ns",
                    str(attempts.stat().st_mtime_ns) if attempts and attempts.is_file() else "0",
                ),
            ],
        )
    return len(entries)


def database_is_fresh(database: Path, sources: Sequence[Path]) -> bool:
    if not database.is_file():
        return False
    try:
        with connect(database) as connection:
            version = connection.execute(
                "SELECT value FROM metadata WHERE key='schema_version'"
            ).fetchone()
            if not version or int(version[0]) != SCHEMA_VERSION:
                return False
    except (sqlite3.Error, ValueError):
        return False
    return database.stat().st_mtime_ns >= max(
        (path.stat().st_mtime_ns for path in sources if path.is_file()), default=0
    )


def _fts_query(query: str, *, mode: str = "all") -> str:
    tokens = re.findall(r"[\w@.]+", query, flags=re.UNICODE)
    operator = " AND " if mode == "all" else " OR "
    return operator.join(f'"{token}"*' for token in tokens)


def search(
    connection: sqlite3.Connection,
    query: str,
    *,
    source_kind: str = "",
    status: str = "",
    tag: str = "",
    mode: str = "all",
    limit: int = 10,
) -> list[sqlite3.Row]:
    where = ["entries_fts MATCH ?"]
    params: list[object] = [_fts_query(query, mode=mode)]
    if source_kind:
        where.append("e.source_kind = ?")
        params.append(source_kind)
    if status:
        where.append("e.status = ?")
        params.append(status)
    if tag:
        where.append("(' ' || e.tags || ' ') LIKE ?")
        params.append(f"% {tag} %")
    params.append(limit)
    return connection.execute(
        f"""
        SELECT e.*, bm25(entries_fts, 3.0, 1.0, 2.0, 2.0, 1.5) AS rank,
               snippet(entries_fts, 1, '[', ']', ' … ', 24) AS snippet
        FROM entries_fts
        JOIN entries e ON e.rowid = entries_fts.rowid
        WHERE {' AND '.join(where)}
        ORDER BY rank, e.match_percent DESC
        LIMIT ?
        """,
        params,
    ).fetchall()


def get_entry(connection: sqlite3.Connection, entry_id: str) -> Optional[sqlite3.Row]:
    return connection.execute("SELECT * FROM entries WHERE id = ?", (entry_id,)).fetchone()


def stats(connection: sqlite3.Connection) -> dict[str, object]:
    by_kind = dict(
        connection.execute(
            "SELECT source_kind, COUNT(*) FROM entries GROUP BY source_kind ORDER BY source_kind"
        ).fetchall()
    )
    by_tag = dict(
        connection.execute(
            """
            WITH RECURSIVE split(rowid, tag, rest) AS (
              SELECT rowid, '', tags || ' ' FROM entries
              UNION ALL
              SELECT rowid, substr(rest, 0, instr(rest, ' ')), substr(rest, instr(rest, ' ') + 1)
              FROM split WHERE rest <> ''
            )
            SELECT tag, COUNT(*) FROM split WHERE tag <> '' GROUP BY tag ORDER BY COUNT(*) DESC, tag
            """
        ).fetchall()
    )
    return {"total": sum(by_kind.values()), "by_kind": by_kind, "by_tag": by_tag}
