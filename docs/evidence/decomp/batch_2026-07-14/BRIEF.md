# Batch 2026-07-14 — five new decomp targets

Read and follow `.cursor/skills/xenoblade-decomp/SKILL.md`, `docs/MWCC_REFERENCE.md` (esp. §3 SDA / getInstance / `__register_global_object` patterns), and `tools/test/compare_behaviour/README.md` if static match &lt; 100%.

## Policy

- Goal: **FULL_MATCH** via `python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."`
- High-level C/C++ only in `src/**` / `libs/**` (no `register rN`, no whole-function asm)
- Below 100%: mandatory behaviour host tests + `behaviour audit`
- Run `coop run size <unit>` — must PASS (under budget OK)

## Targets

| id | symbol | unit | exclusive edit |
|----|--------|------|----------------|
| padmgr-get-instance | `getInstance__Q22cf13CfGameManagerFv` | kyoshin/cf/CfGameManager | `src/kyoshin/cf/CfGameManager.cpp` (+ hpp) |
| padmgr-get-current-pad | `getCurrentPad__Q22cf13CfGameManagerFv` | same | same |
| padmgr-func-80082D54 | `func_80082D54__Q22cf13CfGameManagerFi` | same | same |
| scn-add-render-cb | `addRenderCB__4CScnFP10IScnRenderUlUl` | monolib/src/scn/CScn | `libs/monolib/src/scn/CScn.cpp` (+ hpp) |
| scn-remove-render-cb | `removeRenderCB__4CScnFP10IScnRender` | same | same |

Retail asm for each symbol is in this directory (`asm_*.s`).

## Shared repo resources

- `python3 tools/coop/run.py targets show <id>`
- `python3 tools/coop/run.py cycle <id> --hypothesis "..." --next-change "..."`
- `python3 tools/coop/run.py diff <unit> --symbol <sym>`
- `python3 tools/coop/run.py size <unit>`
- `python3 tools/coop/run.py ctx <source.cpp>`
- Retail reference: `build/us/asm/...`
- Configure already lists both TUs (`NonMatching`)
