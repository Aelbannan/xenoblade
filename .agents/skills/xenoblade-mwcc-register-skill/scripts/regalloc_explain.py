#!/usr/bin/env python3
"""Print Wii 1.1 MWCC register-allocation facts and spill arithmetic.

Pools, bounds, and cuts are confirmed in Wii 1.1 mwcceppc.exe
(14f99570...fbe5) via the table fill at 0x005959B0; driver, simplify,
and select mechanics are inferred from control flow; the per-node cost
formula is open (the --uses/--defs/--weight convenience form is a
GC-era reference hypothesis, flagged where used).
"""

from __future__ import annotations

import argparse


CLASSES = {
    # volatile pool ascending is the normal preference (inferred);
    # claim pool descending is the fallback (inferred).
    "gpr": {
        "normal": [0, *range(3, 13)],  # confirmed pool contents
        "fallback": list(range(31, 13, -1)),  # inferred (31->14 saved span)
        "capacity": 29,  # 32 minus pre-marked r1, r2, r13 (confirmed)
        "unavailable": [1, 2, 13],
        "call_crossing": 18,  # inferred (EABI r14-r31)
        "prefix": "r",
    },
    "fpr": {
        "normal": list(range(14)),  # confirmed pool contents
        "fallback": list(range(31, 13, -1)),  # inferred
        "capacity": 32,
        "unavailable": [],
        "call_crossing": 18,  # inferred (EABI f14-f31)
        "prefix": "f",
    },
    "vr": {
        "normal": list(range(20)),  # confirmed pool contents
        "fallback": list(range(31, 19, -1)),  # inferred
        "capacity": 32,
        "unavailable": [],
        "call_crossing": 12,  # inferred (v20-v31)
        "prefix": "v",
    },
    "spr": {
        "normal": [0, 1, 2, 4, 5],  # confirmed pool contents, bound 6
        "fallback": list(range(3, -1, -1)),  # inferred shape
        "capacity": 6,
        "unavailable": [],
        "call_crossing": None,  # crossing semantics unrecovered
        "prefix": "spr",
    },
    "crfield": {
        "normal": [0, 1, 6, 7],  # confirmed pool contents, bound 8
        "fallback": list(range(5, -1, -1)),  # inferred shape
        "capacity": 8,
        "unavailable": [5],  # pre-marked used (confirmed)
        "call_crossing": None,  # crossing semantics unrecovered
        "prefix": "cr",
    },
}

DRIVER_ORDER = ["spr", "crfield", "vr", "fpr", "gpr"]  # inferred (0x005B8E20)
RETRY_CAP = 10  # confirmed assert Coloring.c:550


def format_regs(prefix: str, registers: list[int]) -> str:
    return ", ".join(f"{prefix}{register}" for register in registers)


def describe(register_class: str, crosses_call: bool) -> None:
    info = CLASSES[register_class]
    normal = info["normal"]
    fallback = [r for r in info["fallback"] if r not in info["unavailable"]]
    prefix = info["prefix"]
    print(f"class: {register_class.upper()}")
    print(f"driver order: {' -> '.join(c.upper() for c in DRIVER_ORDER)}")
    print(f"baseline colors: {info['capacity']}")
    if info["unavailable"]:
        print(f"unavailable: {format_regs(prefix, info['unavailable'])}")
    print(f"normal order: {format_regs(prefix, normal)}")
    print(f"fallback order: {format_regs(prefix, fallback)}")
    print(f"per-class retry cap: {RETRY_CAP} (Coloring.c:550)")
    if crosses_call:
        if info["call_crossing"] is None:
            print("call-crossing capacity: unrecovered for this class")
            return
        available = info["call_crossing"]
        print(f"call-crossing colors: {available} (inferred)")
        print(f"first clique spill pressure: {available + 1} values (inferred)")


def spill_cost(cost: float | None, uses: int | None, definitions: int | None,
               flow_weight: int | None, degree: int) -> None:
    # Confirmed: simplify ranks by cost/degree with x87 FP comparison;
    # protected flags 0x80/0x400 take fixed scores (values unrecovered).
    if cost is None:
        print("note: per-node cost formula is OPEN on Wii 1.1; "
              "weight*(2*uses+defs) is a GC-era reference hypothesis")
        cost = flow_weight * (2 * uses + definitions)
    print(f"spill cost: {cost:g}")
    print(f"selection score: {cost / degree:g} (minimize; ties: unrecovered)")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    order = subparsers.add_parser("order", help="show color order and capacity")
    order.add_argument("register_class", choices=CLASSES)
    order.add_argument("--crosses-call", action="store_true")

    cost = subparsers.add_parser("spill-cost", help="compute cost/degree score")
    cost.add_argument("--cost", type=float, default=None,
                      help="known spill cost (preferred; skips the hypothesis)")
    cost.add_argument("--uses", type=int, default=0)
    cost.add_argument("--definitions", type=int, default=0)
    cost.add_argument("--flow-weight", type=int, default=1)
    cost.add_argument("--degree", type=int, required=True)
    return parser


def main() -> None:
    args = build_parser().parse_args()
    if args.command == "order":
        describe(args.register_class, args.crosses_call)
        return
    if args.degree <= 0:
        raise SystemExit("degree must be positive")
    if args.cost is None and min(args.uses, args.definitions,
                                 args.flow_weight) < 0:
        raise SystemExit("counts and weight must be nonnegative")
    spill_cost(args.cost, args.uses, args.definitions,
               args.flow_weight, args.degree)


if __name__ == "__main__":
    main()
