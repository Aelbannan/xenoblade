"""Adversarial soundness-regression suite (P1-01).

Thin re-exports of existing negative/adversarial TestCase classes so CI can
discover them under ``tests/soundness/`` without duplicating test bodies.

Primary copies remain at ``tools/ppc_equivalence/tests/test_*.py``. Nested
discovery::

    python -m unittest discover -s tools/ppc_equivalence/tests/soundness -p "test_*.py"
"""
