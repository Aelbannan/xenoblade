#!/usr/bin/env python3
"""
Grab the smallest NOT_STARTED targets in batches, claim them, show the
reference assembly and current stub, then let the agent match each one.
After matching, cycles and releases.  Repeats until no NOT_STARTED remain.

Usage:
  python3 tools/coop/claim-smallest.py                          # batch of 1
  python3 tools/coop/claim-smallest.py --batch 5                # batch of 5
  python3 tools/coop/claim-smallest.py --batch 3 --max-rounds 10
  python3 tools/coop/claim-smallest.py --dry-run                # preview
"""
from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config
from tools.coop.lib.targets import load_targets


def run_coop(*args: str) -> subprocess.CompletedProcess:
    cmd = [sys.executable, "tools/coop/run.py"] + list(args)
    return subprocess.run(cmd, capture_output=False, text=True, cwd=str(ROOT))


def get_smallest_not_started(config, num: int) -> list:
    targets = load_targets(config)
    candidates = [
        t for t in targets
        if t.status == "NOT_STARTED"
        and t.kind == "function"
        and t.buildable
        and not t.extra.get("claim", {}).get("owner")
    ]
    def size_val(t):
        raw = t.extra.get("size", "0x0")
        if isinstance(raw, str) and raw.startswith("0x"):
            return int(raw, 16)
        if isinstance(raw, (int, float)):
            return int(raw)
        return 0
    candidates.sort(key=size_val)
    return candidates[:num]


def show_assembly(unit: str, symbol: str) -> str:
    """Extract the retail .s block for a symbol."""
    asm = ROOT / "build" / "us" / "asm" / f"{unit}.s"
    if not asm.exists():
        return "(no assembly)"
    text = asm.read_text(encoding="utf-8")
    lines = text.split("\n")
    out = []
    capturing = False
    for line in lines:
        if f".fn {symbol}" in line or f".fn {symbol}_" in line:
            capturing = True
        if capturing:
            out.append(line)
            if ".endfn" in line:
                break
    return "\n".join(out) if out else "(symbol not found)"


def show_source_stub(src_path: Path, target_id: str) -> str:
    if not src_path.exists():
        return "(no source)"
    text = src_path.read_text(encoding="utf-8")
    start = text.find(f"LLM-HARNESS-BEGIN: {target_id}")
    if start == -1:
        return "(no harness block)"
    end = text.find("LLM-HARNESS-END:", start)
    if end == -1:
        return "(no harness end)"
    end = text.find("\n", end)
    return text[start:end].strip()


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--batch", type=int, default=1)
    parser.add_argument("--max-rounds", type=int, default=9999)
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--owner", default="pi")
    parser.add_argument("--continuous", action="store_true",
                        help="Keep claiming batch after batch until no NOT_STARTED remain (default: claim one batch then stop)")
    args = parser.parse_args(argv)

    config = load_config(None, ROOT)
    round_no = 0

    while round_no < args.max_rounds:
        batch = get_smallest_not_started(config, args.batch)
        if not batch:
            print("\n=== DONE: no NOT_STARTED targets remain ===", flush=True)
            break

        round_no += 1
        batch_ids = [t.id for t in batch]
        print(f"\n{'='*60}", flush=True)
        print(f"  ROUND {round_no} — {len(batch)} smallest NOT_STARTED target(s)", flush=True)
        print(f"{'='*60}", flush=True)

        if args.dry_run:
            for t in batch:
                src = str(t.source.relative_to(ROOT)) if t.source else "?"
                sz = t.extra.get("size", "?")
                print(f"  {t.id:30s}  size={sz:8s}  {t.function[:45]}  [{src}]", flush=True)
            continue

        # Claim each target
        for t in batch:
            src = str(t.source.relative_to(ROOT)) if t.source else ""
            run_coop("targets", "claim", t.id, "--owner", args.owner)
            print(f"  CLAIMED {t.id}  size={t.extra.get('size','?'):6s}  {t.function[:50]}", flush=True)

        # Show assembly + stub for each
        for t in batch:
            unit = t.unit or ""
            symbol = t.symbol or ""
            src_path = t.source

            print(f"\n--- {t.id} ({t.function}) ---", flush=True)
            print(f"  size={t.extra.get('size','?')}  source={src_path.relative_to(ROOT) if src_path else '?'}", flush=True)
            print(f"\n  RETAIL ASSEMBLY ({unit}):", flush=True)
            print(f"  {show_assembly(unit, symbol)}", flush=True)
            print(f"\n  CURRENT STUB:", flush=True)
            if src_path:
                print(f"  {show_source_stub(src_path, t.id)}", flush=True)
            print(f"\n  Edit the function at: {src_path.relative_to(ROOT) if src_path else '?'}", flush=True)
            print(f"  Then: python3 tools/coop/run.py build {unit}", flush=True)
            print(f"        python3 tools/coop/hexdiff.py {unit} --symbol {symbol}", flush=True)
            print(f"        python3 tools/coop/run.py cycle {t.id} --hypothesis ... --next-change ...", flush=True)
            print(f"        python3 tools/coop/run.py targets release {t.id} --owner {args.owner}", flush=True)

        print(f"\n--- End of round {round_no} ---", flush=True)
        print(f"Batch targets: {', '.join(batch_ids)}", flush=True)

        if not args.continuous:
            break  # Let the agent process this batch before continuing

    if not args.dry_run:
        remaining = get_smallest_not_started(config, 1)
        if remaining:
            print(f"\nNOT_STARTED targets still remain. Run again for next batch.", flush=True)
        else:
            print(f"\nAll NOT_STARTED targets have been claimed and processed!", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
