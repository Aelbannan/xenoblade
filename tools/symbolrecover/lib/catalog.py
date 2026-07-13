from __future__ import annotations

from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path

from .mwcc import demangle_symbol
from .parser import (
    SymbolEntry,
    is_placeholder_type,
    iter_placeholder_types,
    load_symbols,
    placeholder_address,
)


@dataclass
class PlaceholderType:
    name: str
    address: str
    kind: str
    symbols: list[SymbolEntry] = field(default_factory=list)
    namespaces: set[str] = field(default_factory=set)
    methods: set[str] = field(default_factory=set)

    @property
    def symbol_count(self) -> int:
        return len(self.symbols)

    def add(self, entry: SymbolEntry) -> None:
        self.symbols.append(entry)
        info = demangle_symbol(entry.name)
        if info.namespace:
            self.namespaces.add(info.namespace)
        if info.is_ctor:
            self.methods.add("constructor")
        elif info.is_dtor:
            self.methods.add("destructor")
        elif not info.function.startswith("func_"):
            self.methods.add(info.function)


def build_placeholder_catalog(entries: list[SymbolEntry]) -> dict[str, PlaceholderType]:
    catalog: dict[str, PlaceholderType] = {}
    for entry in entries:
        seen: set[str] = set()
        for token in iter_placeholder_types(entry.name):
            if token in seen:
                continue
            seen.add(token)
            if token not in catalog:
                addr = placeholder_address(token) or ""
                kind = token.split("_", 1)[0]
                catalog[token] = PlaceholderType(name=token, address=addr, kind=kind)
            catalog[token].add(entry)
        if is_placeholder_type(entry.name):
            token = entry.name
            if token not in catalog:
                catalog[token] = PlaceholderType(
                    name=token,
                    address=placeholder_address(token) or "",
                    kind=token.split("_", 1)[0],
                )
            catalog[token].add(entry)
    return catalog


def load_catalog(symbols_path: Path) -> dict[str, PlaceholderType]:
    return build_placeholder_catalog(load_symbols(symbols_path))


def find_type(catalog: dict[str, PlaceholderType], query: str) -> PlaceholderType | None:
    query = query.strip()
    if query in catalog:
        return catalog[query]
    upper = query.upper().replace("0X", "")
    if not upper.startswith("804") and not upper.startswith("800") and not upper.startswith("801") and not upper.startswith("802"):
        upper = None
    for name, item in catalog.items():
        if query.lower() in name.lower():
            return item
        if upper and item.address.upper() == upper:
            return item
        if name.endswith("_" + query.upper()):
            return item
    return None


def list_rtti_symbols(entries: list[SymbolEntry], query: str = "") -> list[SymbolEntry]:
    rows = [e for e in entries if "__RTTI__" in e.name]
    if query:
        q = query.lower()
        rows = [e for e in rows if q in e.name.lower()]
    return rows


def group_by_translation_unit(
    catalog: dict[str, PlaceholderType],
) -> dict[str, list[PlaceholderType]]:
    grouped: dict[str, list[PlaceholderType]] = defaultdict(list)
    for item in sorted(catalog.values(), key=lambda x: x.name):
        grouped[item.address[:4] if item.address else "????"].append(item)
    return grouped
