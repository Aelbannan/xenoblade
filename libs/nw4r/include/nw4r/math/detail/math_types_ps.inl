/**
 * nw4r::math::MTX34RotXYZFIdx — isolated Gekko paired-single backend.
 *
 * Isolated PS backend under PLAN.md §17.6 ("Isolated MWCC Gekko paired-single
 * backend").  Included ONLY by math_types.cpp on the Wii/MWCC build;
 * non-MWCC / PC builds compile the readable scalar fallback in that TU.
 *
 * The retail nw4r SDK shipped this function as a whole `asm` function body:
 * paired-single reduction loops (ps_abs/ps_cmpu0/ps_cmpu1/ps_sum0/
 * ps_merge10/ps_neg/ps_sub), u16 quantized stores to the stack (psq_st/psq_l
 * with qr3 = OSInitFastCast U16), the sin/cos lookup-table indexing
 * (lwz/lhz/rlwinm/add/psq_l/ps_madds0/ps_madds1), and the final rotation
 * matrix assembly (ps_muls0/ps_muls1/ps_madds0/ps_merge00/ps_merge10/
 * ps_neg/psq_st).  GPR/stack support is exactly the retail kernel's: the
 * stwu/stack spill for the u16 conversions, lis/addi address setup, the
 * lhz/lwz/rlwinm index math, and the li/blr returns.  None of this can be
 * expressed through MWCC high-level C++ (MWCC never emits psq/ps_* from
 * scalar C++ and reschedules register-operand ASM() blocks).
 *
 * Opcode set: psq_lx, psq_l, psq_st, ps_merge00, ps_merge10, ps_abs, ps_neg,
 * ps_sub, ps_cmpu0, ps_cmpu1, ps_sum0, ps_madds0, ps_madds1, ps_muls0,
 * ps_muls1, plus the minimal scalar/GPR/branch support (fabs, fcmpu, fsubs,
 * lhz, lwz, rlwinm, add, lis/addi, stwu/addi r1, li, bge/ble/bgt, blr).
 *
 * Guard: __MWERKS__ && !NONMATCHING (Wii/MWCC matching build only).
 * Fallback: the scalar SinCosFIdx-based C++ in math_types.cpp (validated for
 * numerical equivalence, not PS bit identity).
 *
 * Notes:
 *  - Retail addresses the (65536.0f, 65536.0f) pair with `li r0, X@sda21`
 *    (R_PPC_EMB_SDA21).  MWCC 3.0a5.2's assembler only accepts @h/@ha/@l/
 *    @loword/@hiword, so the decomp uses `li r0, X@l` (R_PPC_ADDR16_LO) --
 *    identical instruction bytes, reloc type differs (unresolved=yellow in
 *    hexdiff; objdiff functionRelocDiffs=data_value compares resolved values,
 *    which link to the same retail data-pool symbol either way).
 *  - The retail never calls MTX34RotXYZFIdx (nor SinCosFIdx) in the Xenoblade
 *    binary; it is SDK dead code.  Its lookup-index quirk (table pointer +
 *    index<<20) is therefore inert at runtime; the fallback below implements
 *    the intended rotation instead of the inert dead-code behaviour.
 */

#ifndef NW4R_MATH_DETAIL_MATH_TYPES_PS_INL
#define NW4R_MATH_DETAIL_MATH_TYPES_PS_INL

#if defined(__MWERKS__) && !defined(NONMATCHING)

extern "C" {
// Paired (65536.0f, 65536.0f) loaded by psq_lx.  Retail loads it from
// lbl_eu_80669E50 (8 bytes in the retail nw4r data pool); we reference the
// same external symbol so relocations (and objdiff fuzzy) match exactly.
extern const f32 lbl_eu_80669E50[2];
// The sin/cos table (256 entries of {sin_val, cos_val, sin_delta,
// cos_delta} = 16 bytes each).  Retail: lbl_eu_8051D7F8 in the nw4r data
// pool; the decomp port defines the same data as detail::gSinCosTbl in
// math_triangular.cpp.  Referencing the retail name keeps relocs identical.
extern const f32 lbl_eu_8051D7F8[];
}

// clang-format off

asm MTX34* MTX34RotXYZFIdx(register MTX34* pMtx, register f32 fx, register f32 fy, register f32 fz) {
    nofralloc
    lis        r4, lbl_eu_8051D7F8@ha
    li         r0, lbl_eu_80669E50@l
    stwu       r1, -0x10(r1)
    addi       r4, r4, lbl_eu_8051D7F8@l
    psq_lx     f0, r0, r0, 0, 0
    ps_merge00 f6, f1, f2
    ps_merge00 f0, f0, f0
    ps_abs     f4, f6
    ps_neg     f1, f0
    ps_sub     f2, f0, f0
    ps_cmpu0   cr0, f4, f0
    ble        fx_reduced
fx_loop:
    ps_sum0    f4, f4, f4, f1
    ps_cmpu0   cr0, f4, f0
    bgt        fx_loop
fx_reduced:
    ps_cmpu1   cr0, f4, f0
    ble        fy_reduced
    ps_merge10 f4, f4, f4
fy_loop:
    ps_sum0    f4, f4, f4, f1
    ps_cmpu0   cr0, f4, f0
    bgt        fy_loop
    ps_merge10 f4, f4, f4
fy_reduced:
    psq_st     f4, 0x8(r1), 0, 3
    psq_l      f7, 0x8(r1), 0, 3
    fabs       f5, f3
    lwz        r0, 0x8(r1)
    fcmpu      cr0, f5, f0
    ble        fz_reduced
fz_loop:
    fsubs      f5, f5, f0
    fcmpu      cr0, f5, f0
    bgt        fz_loop
fz_reduced:
    psq_st     f5, 0x8(r1), 1, 3
    rlwinm     r5, r0, 20, 20, 27
    add        r5, r4, r5
    ps_sub     f7, f4, f7
    psq_l      f4, 0x0(r5), 0, 0
    rlwinm     r6, r0, 4, 20, 27
    psq_l      f8, 0x8(r5), 0, 0
    ps_cmpu0   cr0, f6, f2
    add        r6, r4, r6
    ps_madds0  f0, f8, f7, f4
    psq_l      f4, 0x0(r6), 0, 0
    psq_l      f8, 0x8(r6), 0, 0
    lhz        r0, 0x8(r1)
    bge        fx_positive
    ps_neg     f9, f0
    ps_merge01 f0, f9, f0
fx_positive:
    ps_madds1  f1, f8, f7, f4
    psq_l      f7, 0x8(r1), 1, 3
    rlwinm     r0, r0, 4, 20, 27
    ps_cmpu1   cr0, f6, f2
    add        r5, r4, r0
    fsubs      f7, f5, f7
    psq_l      f4, 0x0(r5), 0, 0
    psq_l      f8, 0x8(r5), 0, 0
    bge        fy_positive
    ps_neg     f9, f1
    ps_merge01 f1, f9, f1
fy_positive:
    fcmpu      cr0, f3, f2
    ps_madds0  f2, f8, f7, f4
    bge        fz_positive
    ps_neg     f9, f2
    ps_merge01 f2, f9, f2
fz_positive:
    ps_neg     f3, f0
    ps_muls1   f5, f2, f1
    ps_sub     f7, f0, f0
    ps_merge10 f3, f3, f0
    ps_merge10 f6, f5, f5
    ps_muls0   f4, f0, f2
    psq_st     f7, 0x2c(r3), 1, 0
    ps_muls0   f8, f3, f2
    psq_st     f6, 0x0(r3), 1, 0
    ps_muls1   f6, f0, f2
    ps_muls1   f2, f3, f2
    ps_madds0  f6, f6, f1, f8
    ps_neg     f2, f2
    psq_st     f6, 0x4(r3), 0, 0
    ps_merge00 f6, f7, f5
    psq_st     f6, 0xc(r3), 0, 0
    ps_madds0  f6, f4, f1, f2
    psq_st     f6, 0x14(r3), 0, 0
    ps_neg     f6, f1
    ps_merge00 f6, f7, f6
    psq_st     f6, 0x1c(r3), 0, 0
    ps_muls1   f6, f0, f1
    psq_st     f6, 0x24(r3), 0, 0
    addi       r1, r1, 0x10
    blr
}

// clang-format on

#endif // __MWERKS__ && !NONMATCHING

#endif // NW4R_MATH_DETAIL_MATH_TYPES_PS_INL
