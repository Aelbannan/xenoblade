"""Canonical fake-vtable -> real-class playbook for Paseo subagent prompts.

Also documented in docs/MWCC_PATTERNS.md ("Fake vtables -> real classes").
Keep this string and that section in sync when the flow changes.
"""

PLAYBOOK = r'''
PLAYBOOK (mandatory; also in docs/MWCC_PATTERNS.md "Fake vtables -> real classes"
and "cf::CHelp layout"):

GOAL: delete the pad/If/View and recover the real class tree with real methods.
Do NOT move pads into a header, do NOT combine them into one shared If, do NOT
leave _v008() dummies. End state is this->method(...) (or this->Base::method(...))
on the type that owns the slot. CHelp is the proven instance of this flow.

FLOW (hexdiff --brief after each step; revert if mismatch count rises):

1. Dump the retail table. JP __vt__Q22cf... / US lbl_eu_* in
   build/<region>/asm/split1.s. Words: RTTI at +0x00, 0 at +0x04, then the
   method list. That list IS the original virtuals. Note size: 0x1C = five
   virtuals; 0x24 = seven. Compare sibling tables for subclass slots.
2. Name the owning class from the linker symbol, not from the TU you are in.
   func_802B7CBC__Q22cf11CHelpSwitchFv is cf::CHelpSwitch even if the call site
   is CHelp_ArtsAttack. JP __RTTI__ / *_typestr / *_hierarchy confirm.
   Hierarchy size: 0xC = one parent, 0x14 = two, 0x1C = three. A missing
   __vt__ for an intermediate is normal if nothing constructs it as
   most-derived; leaf tables still hold its slots.
3. Put each slot on THAT class as a real virtual with the retail signature
   (hexdiff --asm / symbols xref; _Fv is not evidence). __declspec(novtable)
   if the TU has no .data. Ctor writes the symbols.txt label
   (this->vtbl() = &lbl_eu_...), same as CToken. Do not emit a compiler __vt__.
4. Inherit the real tree. A leaf evaluate IS the +0x10 override, not a wrapper
   around a view. A foreign sub-object (*(obj+4)) stays a tiny named iface on
   the OWNING object (PcSub4VtIf), never a pad named after the caller.
5. Call this->method(...). Delete *If / *VtblView / _vNNN /
   reinterpret_cast<Pad*>(this). Hexdiff the TU, every TU that included the
   header, and one sibling that must NOT have grown a slot.
6. Rename last. UnkVirtualFunc* is a placeholder, not a slot index.
   symbols rename-plan / rename-all after the tree is right; prefer same-length.

OWNING-CLASS / HOT HEADERS ARE IN SCOPE:
- If retail puts the slot on CfObjectEne / CActorParam / CfObjectPc / CfObject /
  CfObjectMove / etc., EDIT that header. Dump the table first, put the real
  virtual with retail arity, call obj->method(...), delete the TU-local pad.
- Hexdiff every TU that includes the header; revert if mismatch counts rise.
- Still forbidden: growing _vNNN dummy lists on those headers.

NO-ARG / PASSTHROUGH VIEWS ARE FORBIDDEN:
- Do NOT invent Passthrough9C / no-arg views of a slot that already takes args.
- Real shape is argument forwarding: UVF26(vec, scale) { this->UVF19(vec); ... }
  so MWCC keeps r4/f1 live into the callee (0x10-byte bctr thunk).
- CfObject already has widened arity: UVF19/22(const CVec3*), UVF25(CVec3*, float),
  UVF26(const CVec3*, float), UVF35(float), UVF36()->float. Prefer those.
- Forced-name extern "C" Fv wrappers may take hidden args when retail leaves
  r4/f1 live; recover the expression, don't invent a no-arg view.

DO NOT:
- Declare a differently-named virtual hoping it overrides a base slot (it
  APPENDS). CHelpSwitch::func_802B7CB0 stayed non-virtual for that reason.
- Put a subclass slot on the base.
- Inherit a view and redeclare a slot with a new signature (appends).
- mVtbl->mSlots[N] as a function pointer (colors r4 instead of r12).
- Return bool from a caller when the virtual returns u32: MWCC inserts
  neg/or/srwi and grows the caller ~0xC.
- Keep a pad "until we know the name". The vtable word IS the name.
- Grow dummy _vNNN lists. Prefer real named virtuals on the owning class.
- Commit. Do not run --smt / --linked / plain run.py diff.

CHelp worked example (already done; copy the shape, not the names):
- CHelp is novtable, prefix so vptr is at +8, ctor writes lbl_eu_8053B3A0
  (JP: __vt__Q22cf5CHelp). Five virtuals through +0x18.
- CHelpSwitch is a REAL class (Q22cf11CHelpSwitch, JP RTTI). CBC at +0x1C,
  CE4 at +0x20. Switch-family leaves inherit CHelpSwitch; direct leaves
  inherit CHelp.
- Call sites: this->func_802B7CBC(flag), not reinterpret_cast<If*>(this).

Use .venv/bin/python3. Prefer:
  python3 tools/coop/hexdiff.py <unit> --symbol <mangled> --brief
Identity/state: python3 tools/coop/run.py targets show <id>
'''.strip()


def build_fake_vt_prompt(*, cwd: str, files: str, unit: str, extra: str = "") -> str:
    """Assemble a full fake-vtable subagent prompt."""
    extra_block = f"\n{extra.strip()}\n" if extra and extra.strip() else "\n"
    return f"""You are in {cwd} (Xenoblade Wii decomp, private fork).
Load the xenoblade-decomp skill. Use .venv/bin/python3. Prefer hexdiff over ninja.

PRIMARY FILES (start here; owning-class headers are also in scope when a slot lives there):
{files}
hexdiff unit: {unit}

TASK: Kill fake vtables in these files and recover the real class tree.
{extra_block}
{PLAYBOOK}

When done, report: (1) retail __vt__ / lbl_eu_* you dumped and slot->symbol map,
(2) owning class per slot, (3) what you deleted, (4) hexdiff --brief before/after
for the functions you touched, (5) any slot still blocked and why. No commit.
"""
