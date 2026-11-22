"""Mechanical land gate: the only path by which source/config changes land.

Every change (agent plan or orchestrator hand edit) is judged by measured,
whole-tree before/after snapshots, never by a plan's own claims:

* ``snapshot``  - per unit: per-function hexdiff marks, identical data bytes
  per section, extab status, faithful hard categories + extra bans.
* ``compare``   - rejects any ✓ lost, any whole-tree drop of identical data
  bytes in a section (moves between units are conserved), any unit whose
  extab goes from passing to failing, and any per-unit rise of a hard
  faithful category or ban (unless an explicit, reasoned allowance is given).
* ``ledger``    - files that passed the gate are recorded by content hash; the
  pre-commit hook refuses staged gated files whose content never passed.

CLI: ``.venv/bin/python3 -m tools.coop.land {snapshot,compare,check,apply,adopt,verify-staged}``.
"""
