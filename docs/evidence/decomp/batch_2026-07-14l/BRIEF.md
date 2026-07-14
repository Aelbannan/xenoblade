# Batch 2026-07-14l — five **new** decomp targets

Read and follow `.cursor/skills/xenoblade-decomp/SKILL.md`, `docs/MWCC_REFERENCE.md` (esp. **§3 Retail SDA global names**), and `tools/test/compare_behaviour/README.md` if static match &lt; 100%.

## Policy

- Goal: **FULL_MATCH** via `python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..."`
- High-level C/C++ only in `src/**` / `libs/**` (no `register rN`, no whole-function asm)
- Below 100%: mandatory behaviour host tests + `behaviour audit`
- Run `coop run size <unit>` — must PASS (under budget OK; trim TU extras if over)
- **Edit only your exclusive files / methods** — do not touch other batch targets' exclusive regions
- Prefer `StrReplace` scoped to your function body only (shared TUs!)

## Targets (one agent each)

| id | symbol | address | size | unit | exclusive edit |
|----|--------|---------|------|------|----------------|
| menu-ptgauge-init | `Init__12CMenuPTGaugeFv` | `0x801884E8` | `0x1AC` | `kyoshin/menu/CMenuPTGauge` | **only** `Init` — leave `Move` / `cbRenderBefore` alone |
| window-mgr-term | `Term__16CUIWindowManagerFv` | `0x8013D068` | `0x60` | `kyoshin/CUIWindowManager` | **only** `Term` — leave `Move` alone |
| menu-battlemode-init | `Init__15CMenuBattleModeFv` | `0x801A15E0` | `0x134` | `kyoshin/menu/CMenuBattleMode` | create `src/kyoshin/menu/CMenuBattleMode.cpp` (+ hpp) with **only** `Init` |
| arts-param-ctor | `__ct__Q22cf10CArtsParamFv` | `0x80155FD8` | `0x74` | `kyoshin/cf/CArtsParam` | **only** `CArtsParam` ctor — leave sibling vfuncs alone unless required for link |
| game-on-pause-trigger | `OnPauseTrigger__5CGameFb` | `0x80039FB4` | `0x100` | `kyoshin/CGame` | **only** `OnPauseTrigger` — leave `wkRender` / `wkUpdate` / `GameMain` alone |

Retail asm for each symbol is in this directory (`asm_*.s`).

## Hints from retail asm

### `CMenuPTGauge::Init` (`asm_Init_CMenuPTGauge.s`)
- Frame `-0x20`; `getHandleMEM2` → `createRegion(this+0x64, handle, 0x1200, lbl_eu_805039C8, 0)`
- Stack `Class_8045F858` over region; `func_801355F4` + `func_80136E84` into `this+0x74` with string `lbl_eu_805039C8+0xd`
- Five `func_80136F08` into `+0x78/+0x7C/+0x80/+0x84/+0x88` with string offsets `+0x29/+0x48/+0x6F/+0x96/+0xB7`
- Font path: layout `+0x10` root, `CDeviceFont::func_80452C10(1)` + vt+0x24 → `func_8013676C`; layout vt calls `+0x38(0)`, `+0x24`, `+0x1C(anim78)`, `+0x2C(anim78,1)`
- `addRenderCB(this->unk60, this ? this+0x5C : this, 0xA, 0)`; `func_8045F810(region)`; dtor stack helper
- Extend hpp: declare `Init()`; fields `+0x5C` IScnRender/this-adjust, `+0x60` CScn*, `+0x64` UnkClass_8045F564 region, anim slots through `+0x88`
- Mirror patterns from sibling menu Inits / Term; **do not** edit Move/cbRenderBefore

### `CUIWindowManager::Term` (`asm_Term_CUIWindowManager.s`)
- Tiny: `func_8009D0B4()`; `func_8009D514(this ? this+0x54 : this)` (IFlagEvent slice)
- `*(u8*)(this->unk9C + 0x39) = 1` (SetRemove on child); `lbl_eu_80664088 = 0` via SDA
- Use `extern "C" CUIWindowManager* lbl_eu_80664088;` (Move already does) — add `Term()` to hpp; field `unk9C` already present as char[] — widen to pointer type if needed

### `CMenuBattleMode::Init` (`asm_Init_CMenuBattleMode.s`)
- Same family as PTGauge Init but smaller: region at `+0x60` size `0xd00`, strings `lbl_eu_80503D80`
- Layout at `+0x74`; two anims `+0x78/+0x7C`; vt `+0x2C` false then true; `lfs lbl_eu_80667C80` → `stfs` at `anim78+0x10`; vt `+0x38(0)`
- `addRenderCB(*(this+0x70), this+0x5C, 7, 0)`; configure already has NonMatching `CMenuBattleMode.cpp` — create hpp/cpp with minimal class + Init only

### `cf::CArtsParam::CArtsParam` (`asm_ct_CArtsParam.s`)
- **Not** a simple call through CAttackParam ctor: writes Attack vt `lbl_eu_8052F610` to `this+0x84`, clears `unk0`/`unk20`/`unk78`, calls vt+8 (CAttackParam_UnkVirtualFunc1), then swaps vt to `lbl_eu_8052F5E8` and calls vt+8 again (CArtsParam_UnkVirtualFunc1)
- Prefer `extern "C"` vt labels + explicit clear/`(*vt)()` shape over relying on C++ base ctor codegen; existing draft in `CArtsParam.cpp` is wrong shape
- Leave `CArtsParam_UnkVirtualFunc1/2/3` bodies alone unless mangling forces a tiny shared fix

### `CGame::OnPauseTrigger` (`asm_OnPauseTrigger_CGame.s`)
- Existing C++ is close: gate `CfGameManager::func_8007E1B4()`; if paused and `unk228==0` do enter path else just `unk228++`
- Enter: `unk224 = func_801C0014()` (**stfs** — float!), `func_801BFFAC(0,0)` via SDA float `@4566` / `lbl_eu_*` zero, `func_801644BC(1)`, battle vision `getInstance()+0x219C` → `func_801A929C(1)`, `func_80044FBC(1)`
- Unpause when `unk228 <= 1` restore path; always `unk228--` with clamp to 0 via `subic.`
- Fix type of `unk224` to `f32` if currently wrong; use retail SDA zero float name (see MWCC_REFERENCE §3); do not touch other CGame methods

## Shared repo resources

```bash
python3 tools/coop/run.py targets show <id>
python3 tools/coop/run.py cycle <id> --hypothesis "..." --next-change "..."
python3 tools/coop/run.py diff <unit> --symbol <sym>
python3 tools/coop/run.py size <unit>
python3 tools/coop/run.py ctx <source.cpp>
```

- Retail reference: `build/us/asm/kyoshin/`
- Log attempts to `docs/evidence/decomp/attempts.jsonl`
- Append reusable breakthroughs to `docs/MWCC_REFERENCE.md`
- Claims recorded in `docs/ownership.csv`

## Done criteria

Report: final match %, `cycle` exit code, size PASS/FAIL, files changed, and whether FULL_MATCH was reached. If stuck below 100%, ship behaviour host coverage meeting the skill thresholds.
