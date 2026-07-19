"""Deterministic randomized PPC32 program generator for differential testing.

This package adds a seeded, deterministic generator that emits *valid* instruction
blocks (not uniform random words) using the existing ``encode`` helpers, executes
them with ``ConcreteOps``, and compares bit-exact observables against
``SymbolicOps`` with the concrete initial state bound. Dolphin is optional and
reported as skipped when unavailable.

Only this package and ``cli.py`` are modified for P2-01; the shared semantics,
decoder, engine, and corpus are untouched.
"""

from .random_gen import (
    differential_check,
    generate_program,
)
from .shrink import shrink_program

__all__ = ["differential_check", "generate_program", "shrink_program"]
