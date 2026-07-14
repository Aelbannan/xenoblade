# Broadway / PowerPC semantic references

The checker uses a documented source hierarchy. Nintendo has not published a
public Broadway programming manual, so “Broadway semantics” here means the
32-bit big-endian user instruction subset inherited from the 750-family
PowerPC architecture, checked against a mature Wii implementation.

1. [Freescale/NXP EREF 2.0 programmer's reference](https://community.nxp.com/pwmxy87654/attachments/pwmxy87654/mpc5xxx/3777/1/EREFRM.pdf)
   is the normative instruction-pseudocode reference for integer arithmetic,
   XER/CR effects, storage access, and branch BO/BI behavior. Its architecture
   overview identifies the original PowerPC ISA 1.10 desktop environment used
   by G2/e300/e600-family cores and distinguishes it from newer extensions.
2. [NXP PowerPC e500 Core Family Reference Manual](https://www.nxp.com/docs/en/reference-manual/E500CORERM.pdf)
   is a second official reference for common Book I encodings and exceptional
   cases. e500-only facilities are not imported into the Broadway model.
3. [PowerPC 750CL machine-readable ISA](https://github.com/riptl/ppc750cl/blob/master/isa.yaml)
   supplies 750CL opcode masks and reserved-field constraints. The checker
   decodes raw fields itself and uses Capstone only as an independent boundary
   and mnemonic cross-check.
4. Dolphin's Wii/GC PowerPC interpreter is the implementation cross-reference:
   [integer semantics](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_Integer.cpp),
   [integer storage semantics](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_LoadStore.cpp),
   and the [PowerPC core sources](https://github.com/dolphin-emu/dolphin/tree/master/Source/Core/Core/PowerPC).

## Deliberate model choices

- Storage is one shared array of 8-bit cells indexed by 32-bit effective
  address. Halfword and word accesses compose bytes in big-endian order;
  `*brx` forms reverse that byte order.
- The defined proof domain requires naturally aligned accesses to mapped,
  ordinary RAM. MMU/protection/cache/MMIO behavior is not modeled. This is
  explicit in every proof result.
- `XER.CA`, `XER.OV`, sticky `XER.SO`, record-form CR0, comparison SO copying,
  LR link updates, and CTR decrement-before-test are architectural state, not
  decoder annotations.
- Architecturally undefined divide quotients are excluded from the defined
  input domain. A proof cannot use a made-up quotient value.
- Branches are explored only through an acyclic supplied block. A back-edge,
  path bound, instruction bound, or unknown continuation is inconclusive.
- External direct/indirect calls are terminal exits. Both sides must agree on
  exit kind and target; no unsound “call is a no-op” summary is used.
