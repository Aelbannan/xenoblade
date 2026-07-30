/**
 * g3d_transform.cpp - nw4r::g3d::detail matrix transform functions
 *
 * All three functions compute the inverse of the upper 3x3 of a 3x4 matrix
 * using paired-single (PS) instructions for the Gekko/Broadway CPU.
 */

#include <nw4r/g3d.h>
#include <nw4r/math.h>

namespace nw4r {
namespace g3d {
namespace detail {

// Threshold for determinant comparison (~1.0e-36f, in .sdata2)
extern "C" const float lbl_eu_80669B40;

/******************************************************************************
 *
 * CalcViewNrmMtx - compute 3x3 normal matrix (inverse of upper 3x3)
 *
 ******************************************************************************/
asm bool CalcViewNrmMtx(register math::MTX33* pOut,
                        register const math::MTX34* pMtx) {
    nofralloc

    psq_l f0,  0(pMtx), 1, 0
    psq_l f1,  4(pMtx), 0, 0
    psq_l f2, 16(pMtx), 1, 0
    ps_merge10 f6, f1, f0
    psq_l f3, 20(pMtx), 0, 0
    psq_l f4, 32(pMtx), 1, 0
    ps_merge10 f7, f3, f2
    psq_l f5, 36(pMtx), 0, 0

    ps_mul     f11, f3, f6
    ps_merge10 f8,  f5, f4
    ps_mul     f13, f5, f7
    ps_msub    f11, f1, f7, f11
    ps_mul     f12, f1, f8
    ps_msub    f13, f3, f8, f13

    lis        r12, lbl_eu_80669B40@ha
    ps_msub    f12, f5, f6, f12

    ps_mul     f7,  f0, f13
    addi       r12, r12, lbl_eu_80669B40@l
    ps_mul     f10, f3, f4
    ps_madd    f7,  f2, f12, f7
    ps_mul     f9,  f0, f5
    ps_madd    f7,  f4, f11, f7
    psq_l      f6,  0(r12), 1, 0

    ps_mul     f8,  f1, f2
    ps_abs     f8, f7
    ps_msub    f10, f2, f5, f10
    ps_cmpo0   cr0, f8, f6
    ps_msub    f9,  f1, f4, f9
    ps_msub    f8,  f0, f3, f8
    bge        inverse_exists

    psq_st     f0,  0(pOut), 1, 0
    psq_st     f2,  12(pOut), 1, 0
    psq_st     f4,  24(pOut), 1, 0
    psq_st     f1,  4(pOut), 0, 0
    psq_st     f3,  16(pOut), 0, 0
    psq_st     f5,  28(pOut), 0, 0
    li         r3, 0
    blr

inverse_exists:
    fres       f5, f7
    ps_mul     f8,  f1, f2
    ps_sub     f6,  f6, f6
    ps_add     f1,  f5, f5
    ps_msub    f8,  f0, f3, f8
    ps_mul     f2,  f7, f5
    ps_nmsub   f0,  f5, f2, f1

    ps_muls0   f13, f13, f0
    ps_muls0   f12, f12, f0
    psq_st     f13, 0(pOut), 0, 0
    ps_muls0   f11, f11, f0
    psq_st     f12, 12(pOut), 0, 0
    ps_muls0   f10, f10, f0
    psq_st     f11, 24(pOut), 0, 0
    ps_muls0   f9,  f9, f0
    psq_st     f10, 8(pOut), 1, 0
    ps_muls0   f8,  f8, f0
    psq_st     f9,  20(pOut), 1, 0
    psq_st     f8,  32(pOut), 1, 0
    li         r3, 1
    blr
}

/******************************************************************************
 *
 * CalcViewTexMtx - compute 3x4 texture matrix (inverse of upper 3x3,
 *                  zero translation)
 *
 ******************************************************************************/
asm bool CalcViewTexMtx(register math::MTX34* pOut,
                        register const math::MTX34* pMtx) {
    nofralloc

    psq_l f0,  0(pMtx), 1, 0
    psq_l f1,  4(pMtx), 0, 0
    psq_l f2, 16(pMtx), 1, 0
    ps_merge10 f6, f1, f0
    psq_l f3, 20(pMtx), 0, 0
    psq_l f4, 32(pMtx), 1, 0
    ps_merge10 f7, f3, f2
    psq_l f5, 36(pMtx), 0, 0

    ps_mul     f11, f3, f6
    ps_merge10 f8,  f5, f4
    ps_mul     f13, f5, f7
    ps_msub    f11, f1, f7, f11
    ps_mul     f12, f1, f8
    ps_msub    f13, f3, f8, f13

    lis        r12, lbl_eu_80669B40@ha
    ps_msub    f12, f5, f6, f12

    ps_mul     f7,  f0, f13
    addi       r12, r12, lbl_eu_80669B40@l
    ps_mul     f10, f3, f4
    ps_madd    f7,  f2, f12, f7
    ps_mul     f9,  f0, f5
    ps_madd    f7,  f4, f11, f7
    psq_l      f6,  0(r12), 1, 0

    ps_mul     f8,  f1, f2
    ps_abs     f8, f7
    ps_msub    f10, f2, f5, f10
    ps_cmpo0   cr0, f8, f6
    ps_msub    f9,  f1, f4, f9
    ps_msub    f8,  f0, f3, f8
    bge        inverse_exists

    cmplw      cr0, pOut, pMtx
    beq        skip_copy
    psq_st     f0,  0(pOut), 1, 0
    psq_st     f2,  16(pOut), 1, 0
    psq_st     f4,  32(pOut), 1, 0
    psq_st     f1,  4(pOut), 0, 0
    psq_st     f3,  20(pOut), 0, 0
    psq_st     f5,  36(pOut), 0, 0

skip_copy:
    ps_sub     f6,  f6, f6
    psq_st     f6,  12(pOut), 1, 0
    psq_st     f6,  28(pOut), 1, 0
    psq_st     f6,  44(pOut), 1, 0
    li         r3, 0
    blr

inverse_exists:
    fres       f5, f7
    ps_mul     f8,  f1, f2
    ps_sub     f6,  f6, f6
    ps_add     f1,  f5, f5
    ps_msub    f8,  f0, f3, f8
    ps_mul     f2,  f7, f5
    ps_nmsub   f0,  f5, f2, f1

    psq_st     f6,  12(pOut), 1, 0
    psq_st     f6,  28(pOut), 1, 0
    psq_st     f6,  44(pOut), 1, 0

    ps_muls0   f13, f13, f0
    ps_muls0   f12, f12, f0
    psq_st     f13, 0(pOut), 0, 0
    ps_muls0   f11, f11, f0
    psq_st     f12, 16(pOut), 0, 0
    ps_muls0   f10, f10, f0
    psq_st     f11, 32(pOut), 0, 0
    ps_muls0   f9,  f9, f0
    psq_st     f10, 8(pOut), 1, 0
    ps_muls0   f8,  f8, f0
    psq_st     f9,  24(pOut), 1, 0
    psq_st     f8,  40(pOut), 1, 0
    li         r3, 1
    blr
}

/******************************************************************************
 *
 * CalcInvWorldMtx - compute 3x4 inverse world matrix
 *
 ******************************************************************************/
asm bool CalcInvWorldMtx(register math::MTX34* pOut,
                         register const math::MTX34* pMtx) {
    nofralloc

    psq_l f0,  0(pMtx), 1, 0
    psq_l f1,  4(pMtx), 0, 0
    psq_l f2, 16(pMtx), 1, 0
    ps_merge10 f6, f1, f0
    psq_l f3, 20(pMtx), 0, 0
    psq_l f4, 32(pMtx), 1, 0
    ps_merge10 f7, f3, f2
    psq_l f5, 36(pMtx), 0, 0

    ps_mul     f11, f3, f6
    ps_merge10 f8,  f5, f4
    ps_mul     f13, f5, f7
    ps_msub    f11, f1, f7, f11
    ps_mul     f12, f1, f8
    ps_msub    f13, f3, f8, f13

    lis        r12, lbl_eu_80669B40@ha
    ps_msub    f12, f5, f6, f12

    ps_mul     f7,  f0, f13
    addi       r12, r12, lbl_eu_80669B40@l
    ps_mul     f10, f3, f4
    ps_madd    f7,  f2, f12, f7
    ps_mul     f9,  f0, f5
    ps_madd    f7,  f4, f11, f7
    psq_l      f6,  0(r12), 1, 0

    ps_mul     f8,  f1, f2
    ps_abs     f8, f7
    ps_msub    f10, f2, f5, f10
    ps_cmpo0   cr0, f8, f6
    ps_msub    f9,  f1, f4, f9
    ps_msub    f8,  f0, f3, f8
    bge        inverse_exists

    li         r3, 0
    blr

inverse_exists:
    fres       f5, f7
    ps_mul     f8,  f1, f2
    ps_sub     f6,  f6, f6
    ps_add     f1,  f5, f5
    ps_msub    f8,  f0, f3, f8
    ps_mul     f2,  f7, f5
    ps_nmsub   f0,  f5, f2, f1

    ps_muls0   f13, f13, f0
    ps_muls0   f12, f12, f0
    ps_muls0   f11, f11, f0
    ps_muls0   f10, f10, f0
    ps_muls0   f9,  f9, f0
    ps_muls0   f8,  f8, f0

    ps_merge00 f5,  f13, f12
    ps_merge11 f4,  f13, f12

    lfs        f1, 12(pMtx)
    lfs        f2, 28(pMtx)
    lfs        f3, 44(pMtx)

    psq_st     f5,  0(pOut), 0, 0
    psq_st     f4,  16(pOut), 0, 0

    ps_mul     f6,  f13, f1
    ps_madd    f6,  f12, f2, f6
    ps_nmadd   f6,  f11, f3, f6

    psq_st     f10, 32(pOut), 1, 0
    psq_st     f9,  36(pOut), 1, 0
    psq_st     f8,  40(pOut), 1, 0

    ps_merge00 f5,  f11, f6
    ps_merge11 f4,  f11, f6
    psq_st     f5,  8(pOut), 0, 0
    psq_st     f4,  24(pOut), 0, 0

    ps_mul     f7,  f10, f1
    ps_madd    f7,  f9,  f2, f7
    ps_nmadd   f7,  f8,  f3, f7
    psq_st     f7,  44(pOut), 1, 0

    li         r3, 1
    blr
}

} // namespace detail
} // namespace g3d
} // namespace nw4r