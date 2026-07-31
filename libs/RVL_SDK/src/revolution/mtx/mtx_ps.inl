// Isolated Gekko paired-single backend for named RVL_SDK mtx kernels
// (PSMTXCopy, PSMTXScale, PSMTXScaleApply, PSMTXQuat).
//
// This file is included only by mtx.c. The retail builds of these kernels use
// paired-single (psq_*/ps_*) operations that are not expressible through
// approved high-level MWCC C; the __MWERKS__ branch below reproduces the
// retail instruction stream inside the documented PS backend exception
// (PLAN.md 17.6). The #else branch is a complete scalar fallback for
// NONMATCHING / PC / parser builds.
//
// Allowed opcode set: psq_l, psq_st, ps_add, ps_sub, ps_mul, ps_madd,
// ps_msub, ps_nmsub, ps_muls0, ps_sum0, ps_merge*, ps_cmp*, ps_abs, fres,
// plus the minimum scalar loads/stores/ops the kernels require (lfs, stfs,
// fsubs, fadds, fmuls, frsp).
#if defined(__MWERKS__) && !defined(NONMATCHING)

asm void PSMTXCopy(const register Mtx src, register Mtx dst) {
    nofralloc

    psq_l fp0, 0(src), 0, 0
    psq_st fp0, 0(dst), 0, 0
    psq_l fp1, 8(src), 0, 0
    psq_st fp1, 8(dst), 0, 0
    psq_l fp2, 16(src), 0, 0
    psq_st fp2, 16(dst), 0, 0
    psq_l fp3, 24(src), 0, 0
    psq_st fp3, 24(dst), 0, 0
    psq_l fp4, 32(src), 0, 0
    psq_st fp4, 32(dst), 0, 0
    psq_l fp5, 40(src), 0, 0
    psq_st fp5, 40(dst), 0, 0

    blr
}

void PSMTXScale(register Mtx m, register f32 xS, register f32 yS,
                register f32 zS) {
    register f32 c0 = 0.0f;

    ASM (
        stfs xS, 0(m)
        psq_st c0, 4(m), 0, 0
        psq_st c0, 12(m), 0, 0
        stfs yS, 20(m)
        psq_st c0, 24(m), 0, 0
        psq_st c0, 32(m), 0, 0
        stfs zS, 40(m)
        stfs c0, 44(m)
    )
}

asm void PSMTXScaleApply(const register Mtx src, register Mtx dst,
                         register f32 xS, register f32 yS, register f32 zS) {
    nofralloc

    frsp xS, xS
    psq_l fp4, 0(src), 0, 0
    frsp yS, yS
    psq_l fp5, 8(src), 0, 0
    frsp zS, zS
    ps_muls0 fp4, fp4, xS
    psq_l fp6, 16(src), 0, 0
    ps_muls0 fp5, fp5, xS
    psq_l fp7, 24(src), 0, 0
    ps_muls0 fp6, fp6, yS
    psq_l fp8, 32(src), 0, 0
    psq_st fp4, 0(dst), 0, 0
    ps_muls0 fp7, fp7, yS
    psq_l fp2, 40(src), 0, 0
    psq_st fp5, 8(dst), 0, 0
    ps_muls0 fp8, fp8, zS
    psq_st fp6, 16(dst), 0, 0
    ps_muls0 fp2, fp2, zS
    psq_st fp7, 24(dst), 0, 0
    psq_st fp8, 32(dst), 0, 0
    psq_st fp2, 40(dst), 0, 0

    blr
}

void PSMTXQuat(register Mtx m, register const Quaternion *q) {
    // Register map (matches the retail FPR allocation of the SDK source):
    // c_zero=f0, c_one=f1, c_two=f2, scale=f3, tmp0=f4, tmp1=f5, tmp2=f6,
    // tmp3=f7, tmp4=f8, tmp5=f9, tmp6=f10, tmp7=f11, tmp8=f12, tmp9=f13.
    register f32 c_zero, c_one, c_two, scale;
    register f32 tmp0, tmp1, tmp2, tmp3, tmp4;
    register f32 tmp5, tmp6, tmp7, tmp8, tmp9;

    c_one = 1.0f;

    ASM (
        psq_l tmp0, 0(q), 0, 0
        psq_l tmp1, 8(q), 0, 0
        fsubs c_zero, c_one, c_one
        fadds c_two, c_one, c_one
        ps_mul tmp2, tmp0, tmp0
        ps_merge10 tmp5, tmp0, tmp0
        ps_madd tmp4, tmp1, tmp1, tmp2
        ps_mul tmp3, tmp1, tmp1
        ps_sum0 scale, tmp4, tmp4, tmp4
        ps_muls1 tmp7, tmp5, tmp1
        fres tmp9, scale
        ps_sum1 tmp4, tmp3, tmp4, tmp2
        ps_nmsub scale, scale, tmp9, c_two
        ps_muls1 tmp6, tmp1, tmp1
        ps_mul scale, tmp9, scale
        ps_sum0 tmp2, tmp2, tmp2, tmp2
        fmuls scale, scale, c_two
        ps_madd tmp8, tmp0, tmp5, tmp6
        ps_msub tmp6, tmp0, tmp5, tmp6
        psq_st c_zero, 12(m), 1, 0
        ps_nmsub tmp2, tmp2, scale, c_one
        ps_nmsub tmp4, tmp4, scale, c_one
        psq_st c_zero, 44(m), 1, 0
        ps_mul tmp8, tmp8, scale
        ps_mul tmp6, tmp6, scale
        psq_st tmp2, 40(m), 1, 0
        ps_madds0 tmp5, tmp0, tmp1, tmp7
        ps_merge00 tmp1, tmp8, tmp4
        ps_nmsub tmp7, tmp7, c_two, tmp5
        ps_merge10 tmp0, tmp4, tmp6
        psq_st tmp1, 16(m), 0, 0
        ps_mul tmp5, tmp5, scale
        ps_mul tmp7, tmp7, scale
        psq_st tmp0, 0(m), 0, 0
        psq_st tmp5, 8(m), 1, 0
        ps_merge10 tmp3, tmp7, c_zero
        ps_merge01 tmp9, tmp7, tmp5
        psq_st tmp3, 24(m), 0, 0
        psq_st tmp9, 32(m), 0, 0
    )
}

#else

void PSMTXCopy(const Mtx src, Mtx dst) {
    memcpy(dst, src, sizeof(Mtx));
}

void PSMTXScale(Mtx m, f32 xS, f32 yS, f32 zS) {
    m[0][0] = xS;
    m[0][1] = m[0][2] = m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = yS;
    m[1][2] = m[1][3] = 0.0f;
    m[2][0] = m[2][1] = 0.0f;
    m[2][2] = zS;
    m[2][3] = 0.0f;
}

void PSMTXScaleApply(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS) {
    dst[0][0] = src[0][0] * xS;
    dst[0][1] = src[0][1] * xS;
    dst[0][2] = src[0][2] * xS;
    dst[0][3] = src[0][3] * xS;
    dst[1][0] = src[1][0] * yS;
    dst[1][1] = src[1][1] * yS;
    dst[1][2] = src[1][2] * yS;
    dst[1][3] = src[1][3] * yS;
    dst[2][0] = src[2][0] * zS;
    dst[2][1] = src[2][1] * zS;
    dst[2][2] = src[2][2] * zS;
    dst[2][3] = src[2][3] * zS;
}

void PSMTXQuat(Mtx m, const Quaternion *q) {
    f32 x = q->x, y = q->y, z = q->z, w = q->w;
    f32 n = 1.0f / (x * x + y * y + z * z + w * w);
    f32 s = n + n;

    m[0][0] = 1.0f - s * x * x;
    m[0][1] = s * (x * y - z * w);
    m[0][2] = s * (x * z + y * w);
    m[0][3] = 0.0f;
    m[1][0] = s * (x * y + z * w);
    m[1][1] = 1.0f - s * (y * y + z * z + w * w);
    m[1][2] = s * (y * z - y * w);
    m[1][3] = 0.0f;
    m[2][0] = s * (y * z + y * w);
    m[2][1] = s * (x * z - y * w);
    m[2][2] = 1.0f - s * (x * x + y * y);
    m[2][3] = 0.0f;
}

#endif
