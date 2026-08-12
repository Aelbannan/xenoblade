/**
 * monolib code_804DB938 — isolated Gekko paired-single backend for the
 * func_804DD89C / func_804DD8C8 vector lerps.
 *
 * Isolated PS backend under PLAN.md §17.6 ("Isolated MWCC Gekko paired-single
 * backend").  Included ONLY by code_804DB938.cpp on the Wii/MWCC build;
 * non-MWCC / PC builds compile the readable scalar fallback in that TU.
 *
 * The retail functions are straight paired-single lerp kernels
 * (out = a + (b - a) * t): psq_l both pairs, ps_sub, ps_madds0 with the
 * scalar t in f1, psq_st both halves.  MWCC never emits psq/ps instructions
 * from scalar C++ for this shape (the -O4,p auto-vectorizer keeps
 * lfs/fsubs/fmadds/stfs), and the nw4r VEC3Lerp inline-asm helper
 * interleaves XY and Z instead of the retail load-all-first schedule — so
 * the whole retail kernel body is shipped as `asm` with nofralloc, exactly
 * like the nw4r math_types_ps.inl / g3d_transform_ps.inl backends.
 *
 * Opcode set: psq_l (pair W=0 and single W=1), ps_sub, ps_madds0, psq_st.
 * Guard: __MWERKS__ && !NONMATCHING (Wii/MWCC matching build only).
 * Fallback: the scalar field lerp in code_804DB938.cpp (validated for
 * numerical equivalence, not PS bit identity).
 */

#ifndef MONOLIB_EFFECT_CODE_804DB938_PS_INL
#define MONOLIB_EFFECT_CODE_804DB938_PS_INL

#if defined(__MWERKS__) && !defined(NONMATCHING)

// clang-format off

// 3-component lerp: a.xy/b.xy as one pair, a.z/b.z as 32-bit singles.
asm void func_804DD89C(register void* out, register const void* a,
                       register const void* b, register f32 t) {
    nofralloc
    psq_l      f0, 0x0(r4), 0, 0
    psq_l      f3, 0x0(r5), 0, 0
    psq_l      f2, 0x8(r4), 1, 0
    psq_l      f4, 0x8(r5), 1, 0
    ps_sub     f3, f3, f0
    ps_sub     f4, f4, f2
    ps_madds0  f0, f3, f1, f0
    ps_madds0  f1, f4, f1, f2
    psq_st     f0, 0x0(r3), 0, 0
    psq_st     f1, 0x8(r3), 1, 0
    blr
}

// 4-component lerp: two full pairs (all W=0 loads/stores).
asm void func_804DD8C8(register void* out, register const void* a,
                       register const void* b, register f32 t) {
    nofralloc
    psq_l      f0, 0x0(r4), 0, 0
    psq_l      f3, 0x0(r5), 0, 0
    psq_l      f2, 0x8(r4), 0, 0
    psq_l      f4, 0x8(r5), 0, 0
    ps_sub     f3, f3, f0
    ps_sub     f4, f4, f2
    ps_madds0  f0, f3, f1, f0
    ps_madds0  f1, f4, f1, f2
    psq_st     f0, 0x0(r3), 0, 0
    psq_st     f1, 0x8(r3), 0, 0
    blr
}

// clang-format on

#else

// Scalar fallbacks (PC / NONMATCHING builds).
void func_804DD89C(void* out, const void* a, const void* b, f32 t) {
    const f32* av = (const f32*)a;
    const f32* bv = (const f32*)b;
    f32* ov = (f32*)out;
    ov[0] = av[0] + (bv[0] - av[0]) * t;
    ov[1] = av[1] + (bv[1] - av[1]) * t;
    ov[2] = av[2] + (bv[2] - av[2]) * t;
}

void func_804DD8C8(void* out, const void* a, const void* b, f32 t) {
    const f32* av = (const f32*)a;
    const f32* bv = (const f32*)b;
    f32* ov = (f32*)out;
    ov[0] = av[0] + (bv[0] - av[0]) * t;
    ov[1] = av[1] + (bv[1] - av[1]) * t;
    ov[2] = av[2] + (bv[2] - av[2]) * t;
    ov[3] = av[3] + (bv[3] - av[3]) * t;
}

#endif

#endif // MONOLIB_EFFECT_CODE_804DB938_PS_INL
