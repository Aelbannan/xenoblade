# Non-SJIS punctuation: a latent, repo-wide build landmine (2026-10-18)

## Symptom

Some units build; others fail with a message that looks unrelated:

```
sjiswrap: File Z:\...\src\kyoshin\CMiniMap.cpp contains Shift JIS encoding errors
```

It surfaced while bulk-verifying member-izations: `hexdiff --all --json` (which
forces a rebuild) failed for ~14 units with what looked like a SPLIT/config error,
then the real `sjiswrap` error appeared on a rerun.

## Root cause

`sjiswrap` reads each TU as UTF-8 and re-encodes to Shift JIS. **Any character
with no Shift-JIS mapping makes MWCC reject the whole TU.** In practice the
culprit is **U+2014 EM DASH** (`—`) written into comments by tooling/agents, plus
smaller numbers of U+2192 (`→`), U+2500 (`─`), U+2026 (`…`), U+2013/U+2011
dashes, and U+2248 (`≈`).

It is **latent**: a unit with an up-to-date `.o` keeps working, so the breakage is
invisible until that source file is touched — at which point the edit "randomly"
fails to build. That is why it masqueraded as a concurrency/SPLIT failure and why
a full-tree "compile is green" report was misleading (most objects were cached).

Confirmation: `CMiniMap.cpp` had exactly one em-dash at line 2444; replacing it
with `--` made the unit build (only benign warnings). Byte-level "is this valid
Shift-JIS?" scanning also misses some cases (a UTF-8 sequence can decode as valid
SJIS by coincidence: `CTaskLOD.hpp`), so the guard compares the way sjiswrap does
(UTF-8 -> SJIS encode), not by raw decode.

## Fix

- **64 source files** repaired: comment-only, line-neutral (line counts verified
  identical vs backup so `-ipa file` line numbers do not shift). Mappings:
  `—`/`–`/`‑` -> `-`/`--`, `→` -> `->`, `─` -> `-`, `…` -> `...`, `≈` -> `~`.
  881 replacements across 176 lines. No occurrence was in a string/char literal.
- Each fixed unit was force-rebuilt with `hexdiff --all`: 67 runs, **0 SJIS
  errors**. 63 compile; 4 then fail on pre-existing, unrelated C++ errors
  (`code_800B06A4.cpp:687`, `CMenuMakeCrystal.cpp:352`, `CMenuMapSelect.cpp:269`,
  `CTaskLOD.cpp:427`) — none on the edited lines.
- Two units are exempt by design and bypass sjiswrap with `-enc UTF8`:
  `libs/RVL_SDK/.../enc/encjapanese.c` (exotic kanji table) and
  `src/kyoshin/ErrMesData.cpp` (Japanese data). They are the entire allowlist.

## Guard

`tools/coop/check_sjis.py` — tree-wide UTF-8->SJIS gate in sjiswrap's direction,
skips generated `*.ctx.c`, exits non-zero with `file:line` + codepoints. It
complements the existing delta-level `non_sjis_char` check in
`tools/pi_harness/lint.py` (which only sees changed lines, so it would not have
caught the pre-existing backlog). Tests: `tools/coop/tests/test_check_sjis.py`
(9 tests, including a full-tree regression assertion).

## Why it matters beyond member-ization

Any agent editing one of those 64 files previously hit an unexplained build
failure. This is also a plausible contributor to "SPLIT config/us/config.yml"
churn seen in other waves. The guard means a new em-dash is caught at lint time
instead of at a future rebuild.
