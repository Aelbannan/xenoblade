#include <revolution/MTX.h>

asm void PSMTXMultVec(const register Mtx mtx, const register Vec* vec,
                      register Vec* out) {
    // clang-format off
    nofralloc

    psq_l f0, 0x0(vec), 0, 0
    psq_l f2, 0x0(mtx), 0, 0
    psq_l f1, 0x8(vec), 1, 0
    ps_mul f4, f2, f0
    psq_l f3, 0x8(mtx), 0, 0
    ps_madd f5, f3, f1, f4
    psq_l f8, 0x10(mtx), 0, 0
    ps_sum0 f6, f5, f6, f5
    psq_l f9, 0x18(mtx), 0, 0
    ps_mul f10, f8, f0
    psq_st f6, 0x0(out), 1, 0
    ps_madd f11, f9, f1, f10
    psq_l f2, 0x20(mtx), 0, 0
    ps_sum0 f12, f11, f12, f11
    psq_l f3, 0x28(mtx), 0, 0
    ps_mul f4, f2, f0
    psq_st f12, 0x4(out), 1, 0
    ps_madd f5, f3, f1, f4
    ps_sum0 f6, f5, f6, f5
    psq_st f6, 0x8(out), 1, 0

    blr
    // clang-format on
}
