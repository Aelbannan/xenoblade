#include <nw4r/math.h>


namespace nw4r {
namespace math {

MTX33* MTX33Identity(register MTX33* pMtx) {
    register f32 c_00 = 0.0f, c_10 = 1.0f;
    register f32 c_11;

    ASM (
        ps_merge00 c_11, c_10, c_00

        psq_st c_00, MTX33._02(pMtx), 0, 0 // _02=0, _10=0
        psq_st c_00, MTX33._20(pMtx), 0, 0 // _20=0, _21=0
        psq_st c_11, MTX33._00(pMtx), 0, 0 // _00=1, _01=0
        psq_st c_11, MTX33._11(pMtx), 0, 0 // _11=1, _12=0

        stfs c_10,   MTX33._22(pMtx)       // _22=1
    )

    return pMtx;
}

MTX33* MTX34ToMTX33(register MTX33* pOut, register const MTX34* pIn) {
    register f32 row0a, row0b, row1a, row1b, row2a, row2b;

    ASM (
        psq_l row0a, MTX34._00(pIn), 0, 0
        psq_l row0b, MTX34._02(pIn), 0, 0
        psq_l row1a, MTX34._10(pIn), 0, 0
        psq_l row1b, MTX34._12(pIn), 0, 0
        psq_l row2a, MTX34._20(pIn), 0, 0
        psq_l row2b, MTX34._22(pIn), 0, 0

        psq_st row0a, MTX33._00(pOut), 0, 0
        psq_st row0b, MTX33._02(pOut), 1, 0
        psq_st row1a, MTX33._10(pOut), 0, 0
        psq_st row1b, MTX33._12(pOut), 1, 0
        psq_st row2a, MTX33._20(pOut), 0, 0
        psq_st row2b, MTX33._22(pOut), 1, 0
    )

    return pOut;
}

#define nofralloc

MTX34* MTX34Zero(register MTX34* pMtx) {
    register f32 c_zero = 0.0f;

    ASM (
        psq_st c_zero, MTX34._00(pMtx), 0, 0
        psq_st c_zero, MTX34._02(pMtx), 0, 0
        psq_st c_zero, MTX34._10(pMtx), 0, 0
        psq_st c_zero, MTX34._12(pMtx), 0, 0
        psq_st c_zero, MTX34._20(pMtx), 0, 0
        psq_st c_zero, MTX34._22(pMtx), 0, 0
    )

    return pMtx;
}

MTX34* MTX34Scale(register MTX34* pOut, register const MTX34* pIn,
                  register const VEC3* pScale) {
    register f32 xy, z;
    register f32 row0a, row0b;
    register f32 row1a, row1b;
    register f32 row2a, row2b;

    ASM (
        psq_l xy, VEC3.x(pScale), 0, 0 // (XXXX, YYYY)
        psq_l z,  VEC3.z(pScale), 1, 0 // (ZZZZ, 1111)

        psq_l row0a, MTX34._00(pIn), 0, 0
        psq_l row0b, MTX34._02(pIn), 0, 0
        psq_l row1a, MTX34._10(pIn), 0, 0
        psq_l row1b, MTX34._12(pIn), 0, 0
        psq_l row2a, MTX34._20(pIn), 0, 0
        psq_l row2b, MTX34._22(pIn), 0, 0
        
        ps_mul row0a, row0a, xy
        ps_mul row0b, row0b, z
        ps_mul row1a, row1a, xy
        ps_mul row1b, row1b, z
        ps_mul row2a, row2a, xy
        ps_mul row2b, row2b, z
    
        psq_st row0a, MTX34._00(pOut), 0, 0
        psq_st row0b, MTX34._02(pOut), 0, 0
        psq_st row1a, MTX34._10(pOut), 0, 0
        psq_st row1b, MTX34._12(pOut), 0, 0
        psq_st row2a, MTX34._20(pOut), 0, 0
        psq_st row2b, MTX34._22(pOut), 0, 0
    )

    return pOut;
}

MTX34* MTX34Trans(register MTX34* pOut, register const MTX34* pIn,
                  register const VEC3* pTrans) {
    register f32 xy, z;
    register f32 row0a, row0b;
    register f32 row1a, row1b;
    register f32 row2a, row2b;
    register f32 work0, work1, work2;

    ASM (
        psq_l xy, VEC3.x(pTrans), 0, 0 // (XXXX, YYYY)
        psq_l z,  VEC3.z(pTrans), 1, 0 // (ZZZZ, 1111)

        /**
         * Copy inner 3x3 matrix
         */

        psq_l row0a, MTX34._00(pIn), 0, 0
        psq_l row0b, MTX34._02(pIn), 0, 0
        psq_l row1a, MTX34._10(pIn), 0, 0
        psq_l row1b, MTX34._12(pIn), 0, 0
        psq_l row2a, MTX34._20(pIn), 0, 0
        psq_l row2b, MTX34._22(pIn), 0, 0

        psq_st row0a, MTX34._00(pOut), 0, 0
        psq_st row0b, MTX34._02(pOut), 0, 0
        psq_st row1a, MTX34._10(pOut), 0, 0
        psq_st row1b, MTX34._12(pOut), 0, 0
        psq_st row2a, MTX34._20(pOut), 0, 0
        psq_st row2b, MTX34._22(pOut), 0, 0

        /**
         * Perform translation
         */

        ps_mul  work0, row0a, xy            // (_00*x, _01*y)
        ps_madd work1, row0b, z, work0      // (_02*z + _00*x, _03 + _01*y)
        ps_sum0 work2, work1, work2, work1
        psq_st  work2, MTX34._03(pOut), 1, 0

        ps_mul  work0, row1a, xy            // (_10*x, _11*y)
        ps_madd work1, row1b, z, work0      // (_12*z + _10*x, _13 + _11*y)
        ps_sum0 work2, work1, work2, work1
        psq_st  work2, MTX34._13(pOut), 1, 0
        
        ps_mul  work0, row2a, xy            // (_20*x, _21*y)
        ps_madd work1, row2b, z, work0      // (_22*z + _20*x, _23 + _21*y)
        ps_sum0 work2, work1, work2, work1
        psq_st  work2, MTX34._23(pOut), 1, 0
    )

    return pOut;
}

MTX34* MTX34RotAxisFIdx(MTX34* pMtx, const VEC3* pAxis, f32 fidx) {
    PSMTXRotAxisRad(*pMtx, *pAxis, NW4R_MATH_FIDX_TO_RAD(fidx));
    return pMtx;
}

#if defined(__MWERKS__) && !defined(NONMATCHING)

// Isolated Gekko paired-single backend (PLAN.md §17.6).  Wii/MWCC matching
// build: the retail nw4r SDK shipped MTX34RotXYZFIdx as a whole `asm`
// function body (paired-single reduction loops, u16 quantized stack
// conversion, sin/cos table lookups, rotation-matrix assembly).  MWCC
// reschedules register-operand ASM() blocks and recolors FPRs, so only the
// SDK kernel body reproduces the retail byte stream.  See
// libs/nw4r/include/nw4r/math/detail/math_types_ps.inl for the full
// exception record (opcode set, guard, fallback).
//
// Known residual (MWCC wall #3): retail addresses the (65536.0f, 65536.0f)
// pair with `li r0, X@sda21` (R_PPC_EMB_SDA21); MWCC's assembler rejects
// every spelling that would emit it -- `@sda21` (err 33135), bare symbol
// (33023), `sym - _SDA_BASE_` (33024) -- so the kernel uses `X@l`
// (R_PPC_ADDR16_LO): byte-identical instructions, reloc type differs.
// Hybrid probe record (2026 session): computing pTable/pConst as real C++
// pointers around a raw ASM_VOLATILE kernel block reproduces retail's exact
// instruction schedule (structural: 0), and MWCC emits the EMB_SDA21 reloc --
// but its allocator never colors an asm-block-referenced variable into r0
// (probed declaration/assignment orders and binding the mid-kernel r0 chain
// to a named var; all regress), so `li r0` stays unreachable from compiled
// code.  Revisit with a newer MWCC or a linked-DOL prove.
#include "nw4r/math/detail/math_types_ps.inl"

#else

MTX34* MTX34RotXYZFIdx(MTX34* pMtx, f32 fx, f32 fy, f32 fz) {
    f32 sx, cx;
    SinCosFIdx(&sx, &cx, fx);
    f32 sy, cy;
    SinCosFIdx(&sy, &cy, fy);
    f32 sz, cz;
    SinCosFIdx(&sz, &cz, fz);

    pMtx->_20 = -sy;
    pMtx->_00 = cz * cy;
    pMtx->_10 = sz * cy;
    pMtx->_21 = cy * sx;
    pMtx->_22 = cy * cx;

    f32 cx_sz = cx * sz;
    f32 sx_cz = sx * cz;
    pMtx->_01 = sx_cz * sy - cx_sz;
    pMtx->_12 = cx_sz * sy - sx_cz;

    f32 sx_sz = sx * sz;
    f32 cx_cz = cx * cz;
    pMtx->_02 = cx_cz * sy + sx_sz;
    pMtx->_11 = sx_sz * sy + cx_cz;

    pMtx->_03 = 0.0f;
    pMtx->_13 = 0.0f;
    pMtx->_23 = 0.0f;

    return pMtx;
}

#endif // __MWERKS__ && !NONMATCHING

VEC3* VEC3TransformNormal(VEC3* pOut, const MTX34* pMtx, const VEC3* pVec) {
    VEC3 tmp;
    tmp.x = pMtx->_00 * pVec->x + pMtx->_01 * pVec->y + pMtx->_02 * pVec->z;
    tmp.y = pMtx->_10 * pVec->x + pMtx->_11 * pVec->y + pMtx->_12 * pVec->z;
    tmp.z = pMtx->_20 * pVec->x + pMtx->_21 * pVec->y + pMtx->_22 * pVec->z;

    pOut->x = tmp.x;
    pOut->y = tmp.y;
    pOut->z = tmp.z;

    return pOut;
}

MTX44* MTX44Identity(register MTX44* pMtx) {
    register f32 c_zero = 0.0f, c_one = 1.0f;
    register f32 c_01, c_10;

    ASM (
        ps_merge01 c_01, c_zero, c_one
        ps_merge10 c_10, c_one, c_zero

        psq_st c_zero, MTX44._02(pMtx), 0, 0  // _02=0, _03=0
        psq_st c_zero, MTX44._12(pMtx), 0, 0  // _12=0, _13=0
        psq_st c_zero, MTX44._20(pMtx), 0, 0  // _20=0, _21=0
        psq_st c_01,   MTX44._10(pMtx), 0, 0  // _10=0, _11=1
        psq_st c_10,   MTX44._00(pMtx), 0, 0  // _00=1, _01=0
        psq_st c_10,   MTX44._22(pMtx), 0, 0  // _22=1, _23=0
        psq_st c_zero, MTX44._30(pMtx), 0, 0  // _30=0, _31=0
        psq_st c_01,   MTX44._32(pMtx), 0, 0  // _32=0, _33=1
    )

    return pMtx;
}

MTX44* MTX44Copy(register MTX44* pDst, register const MTX44* pSrc) {
    register f32 row0a, row0b;
    register f32 row1a, row1b;
    register f32 row2a, row2b;
    register f32 row3a, row3b;

    ASM (
        psq_l row0a, MTX44._00(pSrc), 0, 0
        psq_l row0b, MTX44._02(pSrc), 0, 0
        psq_l row1a, MTX44._10(pSrc), 0, 0
        psq_l row1b, MTX44._12(pSrc), 0, 0
        psq_l row2a, MTX44._20(pSrc), 0, 0
        psq_l row2b, MTX44._22(pSrc), 0, 0
        psq_l row3a, MTX44._30(pSrc), 0, 0
        psq_l row3b, MTX44._32(pSrc), 0, 0

        psq_st row0a, MTX44._00(pDst), 0, 0
        psq_st row0b, MTX44._02(pDst), 0, 0
        psq_st row1a, MTX44._10(pDst), 0, 0
        psq_st row1b, MTX44._12(pDst), 0, 0
        psq_st row2a, MTX44._20(pDst), 0, 0
        psq_st row2b, MTX44._22(pDst), 0, 0
        psq_st row3a, MTX44._30(pDst), 0, 0
        psq_st row3b, MTX44._32(pDst), 0, 0
    )

    return pDst;
}

} // namespace math
} // namespace nw4r

// Scalar fallbacks for MTX34Add / MTX34Mult (the retail binary compiles these
// as pure paired-single kernels; the MWCC-only PS bodies live in
// math_types_ps.inl).  Used on PC / NONMATCHING builds only.
#if !defined(__MWERKS__) || defined(NONMATCHING)

namespace nw4r {
namespace math {

MTX34* MTX34Add(MTX34* pOut, const MTX34* pA, const MTX34* pB) {
    VEC3Add(reinterpret_cast<VEC3*>(&pOut->_00),
            reinterpret_cast<const VEC3*>(&pA->_00),
            reinterpret_cast<const VEC3*>(&pB->_00));
    pOut->_03 = pA->_03 + pB->_03;
    VEC3Add(reinterpret_cast<VEC3*>(&pOut->_10),
            reinterpret_cast<const VEC3*>(&pA->_10),
            reinterpret_cast<const VEC3*>(&pB->_10));
    pOut->_13 = pA->_13 + pB->_13;
    VEC3Add(reinterpret_cast<VEC3*>(&pOut->_20),
            reinterpret_cast<const VEC3*>(&pA->_20),
            reinterpret_cast<const VEC3*>(&pB->_20));
    pOut->_23 = pA->_23 + pB->_23;

    return pOut;
}

MTX34* MTX34Mult(MTX34* pOut, const MTX34* pIn, f32 factor) {
    VEC3Scale(reinterpret_cast<VEC3*>(&pOut->_00),
              reinterpret_cast<const VEC3*>(&pIn->_00), factor);
    pOut->_03 = pIn->_03 * factor;
    VEC3Scale(reinterpret_cast<VEC3*>(&pOut->_10),
              reinterpret_cast<const VEC3*>(&pIn->_10), factor);
    pOut->_13 = pIn->_13 * factor;
    VEC3Scale(reinterpret_cast<VEC3*>(&pOut->_20),
              reinterpret_cast<const VEC3*>(&pIn->_20), factor);
    pOut->_23 = pIn->_23 * factor;

    return pOut;
}

} // namespace math
} // namespace nw4r

#endif // !__MWERKS__ || NONMATCHING
