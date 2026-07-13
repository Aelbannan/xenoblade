from __future__ import annotations

import re
from pathlib import Path


def lookup_symbol_address(map_path: Path, symbol: str) -> int | None:
  if not map_path.is_file():
    return None

  pattern = re.compile(
      rf"^\s*[0-9a-fA-F]+\s+[0-9a-fA-F]+\s+([0-9a-fA-F]+)\s+[0-9a-fA-F]+\s+\d+\s+{re.escape(symbol)}\s",
  )
  for line in map_path.read_text(encoding="utf-8", errors="replace").splitlines():
    match = pattern.match(line)
    if match:
      return int(match.group(1), 16)
  return None
