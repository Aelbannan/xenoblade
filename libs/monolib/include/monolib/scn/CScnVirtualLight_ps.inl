/**
 * monolib CScnVirtualLight — isolated Gekko paired-single backend for the
 * func_80493BCC Vec3 subtract kernel.
 *
 * Isolated PS backend under PLAN.md §17.6 ("Isolated MWCC Gekko paired-single
 * backend").  Included ONLY by CScnVirtualLight.cpp on the Wii/MWCC build;
 * non-MWCC / PC builds compile the readable scalar fallback in that TU.
 *
 * func_80493BCC (out = a - b) is a retail psq_l/ps_sub/psq_st kernel with the
 * odd `b .+4` scheduler barrier and a register shuffle (a/out/b arrive in
 * r3/r4/r5, then are moved so r3=out, r4=a, r5=b).  MWCC's -O4,p
 * auto-vectorizer only pairs FMA/lerp shapes, never this pure subtract
 * (verified: field ops, paired-member struct, and loop forms all emit scalar
 * lfs/fsubs), so the whole retail body is shipped as `asm` with nofralloc,
 * exactly like the nw4r math_types_ps.inl / g3d_transform_ps.inl backends.
 *
 * Opcode set: mr, b, psq_l (pair W=0 and single W=1), ps_sub, psq_st.
 * Guard: __MWERKS__ && !NONMATCHING (Wii/MWCC matching build only).
 * Fallback: the scalar field subtract in CScnVirtualLight.cpp (validated for
 * numerical equivalence, not PS bit identity).
 */

#ifndef MONOLIB_SCN_CSCNVIRTUALLIGHT_PS_INL
#define MONOLIB_SCN_CSCNVIRTUALLIGHT_PS_INL

#if defined(__MWERKS__) && !defined(NONMATCHING)

// clang-format off

// 3-component subtract: out = a - b.  a.xy/b.xy as one pair, a.z/b.z as
// 32-bit singles.  The `b .+4` is the retail scheduler barrier (branches to
// the very next instruction; kept for byte identity).
asm void func_80493BCC(register void* a, register void* out,
                       register const void* b) {
    nofralloc
    mr         r0, r3
    mr         r3, r4
    mr         r4, r0
    b          lbl_97C50
lbl_97C50:
    psq_l      f0, 0x0(r4), 0, 0
    psq_l      f1, 0x0(r5), 0, 0
    ps_sub     f2, f0, f1
    psq_l      f0, 0x8(r4), 1, 0
    psq_l      f1, 0x8(r5), 1, 0
    psq_st     f2, 0x0(r3), 0, 0
    ps_sub     f2, f0, f1
    psq_st     f2, 0x8(r3), 1, 0
    blr
}

// clang-format on

#else

// Scalar fallback (PC / NONMATCHING builds).
void func_80493BCC(void* a, void* out, const void* b) {
    const f32* av = (const f32*)a;
    const f32* bv = (const f32*)b;
    f32* ov = (f32*)out;
    ov[0] = av[0] - bv[0];
    ov[1] = av[1] - bv[1];
    ov[2] = av[2] - bv[2];
}

#endif

#endif // MONOLIB_SCN_CSCNVIRTUALLIGHT_PS_INL
