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
   [floating-point semantics](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_FloatingPoint.cpp),
   [Broadway FP helpers](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_FPUtils.h),
   [FPSCR system-register semantics](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_SystemRegisters.cpp),
   [quantized paired load/store semantics](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_LoadStorePaired.cpp),
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
- `fmuls` applies Broadway's `Force25Bit` rounding to `frC` before the
  multiply, then rounds the product to binary32. The shared corpus contains
  normal and subnormal cases that differ from a plain IEEE multiply/cast.
- `fcmpu` and `fcmpo` classify signaling NaNs from the original binary64 FPR
  bits so payload conversion cannot quiet them first. Ordered compare models
  Dolphin's `VXSNAN`/`VXVC` split, `VE` interaction, exception summaries, and
  unconditional CR/FPCC update; trap delivery remains outside the model.
- Scalar add/subtract/multiply/divide follow Dolphin's `NI_add`, `NI_sub`,
  `NI_mul`, and `NI_div` cause precedence. The model propagates and quiets the
  first NaN operand, distinguishes `VXISI`/`VXIMZ`/`VXZDZ`/`VXIDI`, raises
  `ZX` for nonzero division by zero, and suppresses FPR/FPRF writes under
  `VE`/`ZE`. Overflow, underflow, inexact flags, and trap delivery are separate.
- `fctiw`/`fctiwz` follow Dolphin's `ConvertToInteger`: rounding occurs before
  the signed-32-bit range check, results use the `0xFFF80000xxxxxxxx` FPR
  format, negative values rounded to zero set the marker bit, and
  `VXSNAN`/`VXCVI`/`XX`/`FI`/`FR` plus `VE` suppression are modeled.
- `fmadds` follows Dolphin's `NI_madd<true>` including `frC` Force25 rounding,
  one-round binary32 behavior, NaN priority `frA` then `frB` then `frC`,
  `VXSNAN`/`VXIMZ`/`VXISI`, `VE` suppression, and direct `FI`/`FR` updates.
  ConcreteOps includes Dolphin's tie correction; the symbolic contract requires
  finite operands to originate as binary32 values so proofs remain decidable.
- `fmsubs` and `fnmsubs` reuse `NI_msub<true>`. Subtraction reverses the
  infinity-sign condition for `VXISI`; unlike `fmadds`, these instructions do
  not assign `FI`/`FR` on finite writeback. `fnmsubs` negates the rounded result
  only when it is not NaN, preserving NaN sign and payload.
- `fnmadds` uses `NI_madd<true>` with the same Force25 and exact binary32
  result path, then negates only a non-NaN result. Like the subtract forms, it
  preserves `FI`/`FR` on finite writeback.
- Double `fmadd`, `fmsub`, `fnmadd`, and `fnmsub` use one binary64 fused
  operation, retain full double NaN payloads, implement the add/subtract
  `VXISI` sign rules, and suppress writeback when an invalid exception is
  enabled. Negative forms never negate NaNs.
- FPSCR writes apply the Broadway writable mask, recompute the derived VX/FEX
  summaries, preserve FX stickiness where required, and copy final FX/FEX/VX/OX
  into CR1 for record forms. `mcrfs` clears only exception bits in the selected
  source field.
- PSQ operations model PS0 and PS1 separately. GQR load/store type and scale
  fields select float, unsigned/signed 8-bit, or unsigned/signed 16-bit
  conversion. Integer stores round through binary32, truncate toward zero, and
  saturate; float stores flush binary32 subnormals to signed zero. Invalid GQR
  type encodings and non-finite integer stores are outside the defined domain.
