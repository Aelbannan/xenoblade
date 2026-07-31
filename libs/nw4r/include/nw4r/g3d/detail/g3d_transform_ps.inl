/**
 * nw4r::g3d::detail view-matrix kernels — isolated Gekko paired-single backend.
 *
 * Isolated PS backend under PLAN.md §17.6 ("Isolated MWCC Gekko paired-single
 * backend").  This file is included ONLY by g3d_transform.cpp on the Wii/MWCC
 * build; non-MWCC / PC builds compile the readable scalar fallbacks in that TU.
 *
 * The retail nw4r SDK compiled these kernels as whole `asm` function bodies
 * (the SDK's own shipping form; identical to zeldaret/ss's nw4r port).  MWCC
 * reschedules register-operand `ASM()` blocks and recolors FPRs, so the
 * retail byte stream (psq_l/ps_merge10/ps_mul/ps_msub/ps_madd/ps_abs/
 * ps_cmpo0/fres/ps_nmsub/ps_muls0/psq_st plus the minimal lis/addi address
 * setup, bge branch, li/blr returns) is only reproducible as a named PS
 * kernel body.  No numbered-GPR choreography beyond the retail kernel: the
 * FPR/GPR usage below is exactly the retail instruction stream.
 *
 * Opcode set (all three kernels): psq_l, psq_st, ps_merge10, ps_mul, ps_msub,
 * ps_madd, ps_abs, ps_cmpo0, fres, ps_add, ps_sub, ps_nmsub, ps_muls0, and
 * the minimal memory/branch support (lis/addi address materialization, bge,
 * cmplw/beq for the alias check, lfs translation loads, li/blr returns).
 *
 * Guard: __MWERKS__ && !NONMATCHING (Wii/MWCC matching build only).
 * Fallback: the scalar C++ CalcAdjugate/IsInvertible/FastReciprocal paths in
 * g3d_transform.cpp (validated for numerical equivalence, not PS bit identity).
 */

#ifndef NW4R_G3D_DETAIL_G3D_TRANSFORM_PS_INL
#define NW4R_G3D_DETAIL_G3D_TRANSFORM_PS_INL

#if defined(__MWERKS__) && !defined(NONMATCHING)

// Singularity threshold shared by all three kernels.  Retail loads this via
// lis/addi + psq_l from lbl_eu_80669B40 (= 1.0000000359391298e-36f).
// C linkage keeps a plain symbol name for the @ha/@l asm references.
extern "C" const f32 g3dTransformEpsilon = 1.0e-36f;

// Included inside namespace nw4r::g3d::detail (by g3d_transform.cpp), so this
// file must NOT reopen the namespaces — doing so double-mangles the symbols.

// clang-format off

asm bool CalcViewNrmMtx(register math::MTX33* pOut, register const math::MTX34* pMtx) {
    nofralloc
    psq_l      f0,  0x0(pMtx), 1, 0
    psq_l      f1,  0x4(pMtx), 0, 0
    psq_l      f2,  0x10(pMtx), 1, 0
    ps_merge10 f6,  f1,  f0
    psq_l      f3,  0x14(pMtx), 0, 0
    psq_l      f4,  0x20(pMtx), 1, 0
    ps_merge10 f7,  f3,  f2
    psq_l      f5,  0x24(pMtx), 0, 0
    ps_mul     f11, f3,  f6
    ps_merge10 f8,  f5,  f4
    ps_mul     f13, f5,  f7
    ps_msub    f11, f1,  f7,  f11
    ps_mul     f12, f1,  f8
    ps_msub    f13, f3,  f8,  f13
    lis        r12, g3dTransformEpsilon@ha
    ps_msub    f12, f5,  f6,  f12
    ps_mul     f7,  f0,  f13
    addi       r12, r12, g3dTransformEpsilon@l
    ps_mul     f10, f3,  f4
    ps_madd    f7,  f2,  f12, f7
    psq_l      f6,  0x0(r12), 1, 0
    ps_mul     f9,  f0,  f5
    ps_madd    f7,  f4,  f11, f7
    ps_msub    f10, f2,  f5,  f10
    ps_msub    f9,  f1,  f4,  f9
    ps_abs     f8,  f7
    ps_cmpo0   cr0, f8,  f6
    bge        nrm_invertible
    psq_st     f0,  0x0(pOut), 1, 0
    psq_st     f2,  0xc(pOut), 1, 0
    psq_st     f4,  0x18(pOut), 1, 0
    psq_st     f1,  0x4(pOut), 0, 0
    psq_st     f3,  0x10(pOut), 0, 0
    psq_st     f5,  0x1c(pOut), 0, 0
    li         r3,  0x0
    blr

nrm_invertible:
    fres       f5,  f7
    ps_mul     f8,  f1,  f2
    ps_sub     f6,  f6,  f6
    ps_add     f1,  f5,  f5
    ps_msub    f8,  f0,  f3,  f8
    ps_mul     f2,  f7,  f5
    ps_nmsub   f0,  f5,  f2,  f1
    ps_muls0   f13, f13, f0
    ps_muls0   f12, f12, f0
    psq_st     f13, 0x0(pOut), 0, 0
    ps_muls0   f11, f11, f0
    psq_st     f12, 0xc(pOut), 0, 0
    ps_muls0   f10, f10, f0
    psq_st     f11, 0x18(pOut), 0, 0
    ps_muls0   f9,  f9,  f0
    psq_st     f10, 0x8(pOut), 1, 0
    ps_muls0   f8,  f8,  f0
    psq_st     f9,  0x14(pOut), 1, 0
    psq_st     f8,  0x20(pOut), 1, 0
    li         r3,  0x1
    blr
}

asm bool CalcViewTexMtx(register math::MTX34* pOut, register const math::MTX34* pMtx) {
    nofralloc
    psq_l      f0,  0x0(pMtx), 1, 0
    psq_l      f1,  0x4(pMtx), 0, 0
    psq_l      f2,  0x10(pMtx), 1, 0
    ps_merge10 f6,  f1,  f0
    psq_l      f3,  0x14(pMtx), 0, 0
    psq_l      f4,  0x20(pMtx), 1, 0
    ps_merge10 f7,  f3,  f2
    psq_l      f5,  0x24(pMtx), 0, 0
    ps_mul     f11, f3,  f6
    ps_merge10 f8,  f5,  f4
    ps_mul     f13, f5,  f7
    ps_msub    f11, f1,  f7,  f11
    ps_mul     f12, f1,  f8
    ps_msub    f13, f3,  f8,  f13
    lis        r12, g3dTransformEpsilon@ha
    ps_msub    f12, f5,  f6,  f12
    ps_mul     f7,  f0,  f13
    addi       r12, r12, g3dTransformEpsilon@l
    ps_mul     f10, f3,  f4
    ps_madd    f7,  f2,  f12, f7
    psq_l      f6,  0x0(r12), 1, 0
    ps_mul     f9,  f0,  f5
    ps_madd    f7,  f4,  f11, f7
    ps_msub    f10, f2,  f5,  f10
    ps_msub    f9,  f1,  f4,  f9
    ps_abs     f8,  f7
    ps_cmpo0   cr0, f8,  f6
    bge        tex_invertible
    cmplw      pOut, pMtx
    beq        tex_alias
    psq_st     f0,  0x0(pOut), 1, 0
    psq_st     f2,  0x10(pOut), 1, 0
    psq_st     f4,  0x20(pOut), 1, 0
    ps_sub     f6,  f6,  f6
    psq_st     f1,  0x4(pOut), 0, 0
    psq_st     f3,  0x14(pOut), 0, 0
    psq_st     f5,  0x24(pOut), 0, 0
    psq_st     f6,  0xc(pOut), 1, 0
    psq_st     f6,  0x1c(pOut), 1, 0
    psq_st     f6,  0x2c(pOut), 1, 0
tex_alias:
    li         r3, 0x0
    blr

tex_invertible:
    fres       f5,  f7
    ps_mul     f8,  f1,  f2
    ps_sub     f6,  f6,  f6
    ps_add     f1,  f5,  f5
    ps_msub    f8,  f0,  f3,  f8
    psq_st     f6,  0xc(pOut), 1, 0
    ps_mul     f2,  f7,  f5
    psq_st     f6,  0x1c(pOut), 1, 0
    psq_st     f6,  0x2c(pOut), 1, 0
    ps_nmsub   f0,  f5,  f2,  f1
    ps_muls0   f13, f13, f0
    ps_muls0   f12, f12, f0
    psq_st     f13, 0x0(pOut), 0, 0
    ps_muls0   f11, f11, f0
    psq_st     f12, 0x10(pOut), 0, 0
    ps_muls0   f10, f10, f0
    psq_st     f11, 0x20(pOut), 0, 0
    ps_muls0   f9,  f9,  f0
    psq_st     f10, 0x8(pOut), 1, 0
    ps_muls0   f8,  f8,  f0
    psq_st     f9,  0x18(pOut), 1, 0
    psq_st     f8,  0x28(pOut), 1, 0
    li         r3,  0x1
    blr
}

asm bool CalcInvWorldMtx(register math::MTX34* pOut, register const math::MTX34* pMtx) {
    nofralloc
    psq_l      f0,  0x0(pMtx), 1, 0
    psq_l      f1,  0x4(pMtx), 0, 0
    psq_l      f2,  0x10(pMtx), 1, 0
    ps_merge10 f6,  f1,  f0
    psq_l      f3,  0x14(pMtx), 0, 0
    psq_l      f4,  0x20(pMtx), 1, 0
    ps_merge10 f7,  f3,  f2
    psq_l      f5,  0x24(pMtx), 0, 0
    ps_mul     f11, f3,  f6
    ps_merge10 f8,  f5,  f4
    ps_mul     f13, f5,  f7
    ps_msub    f11, f1,  f7,  f11
    ps_mul     f12, f1,  f8
    ps_msub    f13, f3,  f8,  f13
    lis        r12, g3dTransformEpsilon@ha
    ps_msub    f12, f5,  f6,  f12
    ps_mul     f7,  f0,  f13
    addi       r12, r12, g3dTransformEpsilon@l
    ps_mul     f10, f3,  f4
    ps_madd    f7,  f2,  f12, f7
    psq_l      f6,  0x0(r12), 1, 0
    ps_mul     f9,  f0,  f5
    ps_madd    f7,  f4,  f11, f7
    ps_msub    f10, f2,  f5,  f10
    ps_msub    f9,  f1,  f4,  f9
    ps_abs     f8,  f7
    ps_cmpo0   cr0, f8,  f6
    bge        world_invertible
    li         r3, 0x0
    blr

world_invertible:
    fres       f6,  f7
    ps_mul     f8,  f1,  f2
    ps_add     f5,  f6,  f6
    ps_mul     f4,  f7,  f6
    ps_msub    f8,  f0,  f3,  f8
    ps_nmsub   f0,  f6,  f4,  f5
    lfs        f1,  0xc(pMtx)
    ps_muls0   f13, f13, f0
    lfs        f2,  0x1c(pMtx)
    ps_muls0   f12, f12, f0
    lfs        f3,  0x2c(pMtx)
    ps_muls0   f11, f11, f0
    ps_merge00 f5,  f13, f12
    ps_merge11 f4,  f13, f12
    ps_mul     f6,  f13, f1
    psq_st     f5,  0x0(pOut), 0, 0
    psq_st     f4,  0x10(pOut), 0, 0
    ps_muls0   f10, f10, f0
    ps_muls0   f9,  f9,  f0
    ps_madd    f6,  f12, f2,  f6
    psq_st     f10, 0x20(pOut), 1, 0
    ps_muls0   f8,  f8,  f0
    ps_nmadd   f6,  f11, f3,  f6
    psq_st     f9,  0x24(pOut), 1, 0
    ps_mul     f7,  f10, f1
    ps_merge00 f5,  f11, f6
    psq_st     f8,  0x28(pOut), 1, 0
    ps_madd    f7,  f9,  f2,  f7
    ps_merge11 f4,  f11, f6
    psq_st     f5,  0x8(pOut), 0, 0
    ps_nmadd   f7,  f8,  f3,  f7
    psq_st     f4,  0x18(pOut), 0, 0
    psq_st     f7,  0x2c(pOut), 1, 0
    li         r3,  0x1
    blr
}

// clang-format on

#endif // __MWERKS__ && !NONMATCHING

#endif // NW4R_G3D_DETAIL_G3D_TRANSFORM_PS_INL
