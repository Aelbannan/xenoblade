/**
 * monolib LOD code_80468434 — isolated Gekko paired-single backend for the
 * copySpawnPos spawn-position spread kernel.
 *
 * Isolated PS backend under PLAN.md §17.6 ("Isolated MWCC Gekko paired-single
 * backend").  Included ONLY by code_80468434.cpp on the Wii/MWCC build;
 * non-MWCC / PC builds compile the readable scalar fallback in that TU.
 *
 * copySpawnPos: nofralloc straight-line kernel that loads the spawn
 * bias/table/spread globals, computes scale = spread * in.w, and writes a
 * per-component select of in vs a dir*scale-derived value into out:
 *   out.c = (in.c < 0) ? in.c : sel(dir.c, scale)
 * (paired-single ps_sel selects per 32-bit half from the sign of FRB).
 *
 * Opcode set: lwz@sda21, add, lfs, rlwinm, mulli, fmuls, psq_l (pair W=0 and
 * single W=1), ps_sel, psq_st, blr.  The four ps_sel words use the spec
 * encoding (XO=12); every MWCC version assembles ps_sel with XO=23 (Wii
 * line) or a shifted field layout (GC line), so they are emitted as raw
 * dc.l words with the intended mnemonic in the comment.  ps_sel is not
 * expressible through approved high-level MWCC C++ (__vec2x32float__ /
 * scalar builtins / normal C++), matching the nw4r math_types_ps.inl /
 * g3d_transform_ps.inl precedent.
 *
 * Guard: __MWERKS__ && !NONMATCHING (Wii/MWCC matching build only).
 * Fallback: the scalar per-component select below (validated for numerical
 * equivalence, not PS bit identity; ps1 halves are undefined in retail).
 */

#ifndef MONOLIB_LOD_CODE_80468434_PS_INL
#define MONOLIB_LOD_CODE_80468434_PS_INL

#if defined(__MWERKS__) && !defined(NONMATCHING)

// r3=out, r4=scratch (overwritten by the table-base load), r5=in, r6=idx.
// The `idx` parameter is declared register s32 so MWCC passes it in r6.
// clang-format off

asm void copySpawnPos__Q23LOD17UnkClass_80468434Fv(register void* out,
                                                    register void* r4scratch,
                                                    register const void* in,
                                                    register s32 idx) {
    nofralloc
    lwz        r0, lbl_eu_806657A0(r0)      // bias
    lwz        r4, lbl_eu_806657AC(r0)      // table base
    add        r0, r6, r0
    lfs        f1, lbl_eu_8066A650(r0)      // spread
    rlwinm     r0, r0, 0, 29, 31              // (idx + bias) & 7
    lfs        f0, 12(r5)                     // in.w
    mulli      r0, r0, 36                     // entry stride 0x24
    fmuls      f0, f1, f0                     // scale = spread * in.w
    add        r4, r4, r0
    psq_l      f2, 12(r4), 0, 0               // dir.xy
    dc.l       0x10220018                     // ps_sel f1, f2, f0, f0 (XO=12 spec)
    psq_st     f1, 0(r3), 0, 0                // (dead pair store, overwritten)
    dc.l       0x10600890                     // ps_sel f3, f0, f2, f1 (XO=12 spec)
    psq_l      f2, 20(r4), 1, 0               // dir.z single
    dc.l       0x10220018                     // ps_sel f1, f2, f0, f0 (XO=12 spec)
    psq_st     f1, 8(r3), 1, 0                // (dead single store, overwritten)
    psq_l      f0, 0(r5), 0, 0                // in.xy pair
    dc.l       0x1003002A                     // ps_sel f0, f3, f0, f0 (XO=12 spec)
    psq_st     f0, 0(r3), 0, 0                // out.xy = select
    psq_l      f0, 8(r5), 1, 0                // in.z single
    dc.l       0x1001002A                     // ps_sel f0, f1, f0, f0 (XO=12 spec)
    psq_st     f0, 8(r3), 1, 0                // out.z = select
    blr
}

// clang-format on

#else

// Scalar fallback (PC / NONMATCHING builds).  Faithful transcription of the
// ps0-half data flow: scale = spread * in.w; per component c, the direction
// value d = dir[(idx + bias) & 7].c is folded through two sign selects and
// written only when the input component is >= 0 (negative inputs keep their
// value).  Retail's ps1 halves are undefined; this fallback treats the
// scalar `scale` sign as governing both halves, which is the intended
// single-scalar reading.
void copySpawnPos__Q23LOD17UnkClass_80468434Fv(void* out, void*,
                                                const void* in, s32 idx) {
    const f32* iv = (const f32*)in;
    f32* ov = (f32*)out;
    f32 scale = lbl_eu_8066A650 * iv[3];
    const LodDirEntry* entry = &lbl_eu_806657AC[(idx + lbl_eu_806657A0) & 7];
    f32 sx = (scale < 0.0f) ? scale : entry->dir.x;
    f32 sy = (scale < 0.0f) ? scale : entry->dir.y;
    f32 sz = (scale < 0.0f) ? scale : entry->dir.z;
    f32 tx = (sx < 0.0f) ? entry->dir.x : scale;
    f32 ty = (sy < 0.0f) ? entry->dir.y : scale;
    ov[0] = (iv[0] < 0.0f) ? iv[0] : tx;
    ov[1] = (iv[1] < 0.0f) ? iv[1] : ty;
    ov[2] = (iv[2] < 0.0f) ? iv[2] : sz;
}

#endif

#endif // MONOLIB_LOD_CODE_80468434_PS_INL
