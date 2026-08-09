// Translation unit: monolib/src/coli/code_804A6C60
// Collision subsystem - method dispatch table handlers and utilities.
// Each stub below is a catalog placeholder pending decompilation.

#include <harness_catalog.h>
#include <monolib/coli/code_804A6C60.hpp>
#include <monolib/coli/coli_types.hpp>
#include <nw4r/math.h>

using ml::coli::CColiObject;
using nw4r::math::VEC3;

static inline f32 clamp01(f32 value) {
    if (value < lbl_eu_8066AE44) return lbl_eu_8066AE44;
    if (value > lbl_eu_8066AE3C) return lbl_eu_8066AE3C;
    return value;
}

void func_804A6C60(void){}

void func_804A6D90(void){}

void func_804A6DC0(void){}

void func_804A6E20(void){}

void func_804A70F8(void){}

void func_804A732C(void){}

void func_804A73A0(void){}

void func_804A763C(void){}

void func_804A7834(void){}

void func_804A7878(void){}

void func_804A790C(){}

void func_804A79B4(){}

void func_804A7ACC(){}

void func_804A7BDC(){}

void func_804A7C64(){}

void func_804A7D1C(){}

void func_804A7E18(){}

void func_804A7E7C(){}

extern u32 lbl_eu_80665938;
u32 func_804A7EC8(u32 unused, u32 val) { lbl_eu_80665938 = val; return unused; }

void func_804A7ED0(){}

void func_804A7F0C(){}

void func_804A7F50(){}

// No-op handler. Used as a default stub in collision dispatch tables for
// method slots that require no action (e.g. optional callbacks).
void func_804A822C() {}

void func_804A8230(){}

void func_804A8850(){}

void func_804A8BE0(){}

void func_804A8CB0(){}

void func_804A98C4(){}

void func_804AA4F4(){}

void func_804AA504(){}

void func_804AA870(){}

void func_804AAA98(){}

void func_804AAD90(){}

void func_804AB524(){}

void func_804AB538(){}

void func_804ABA08(){}

void func_804ABA68(){}

void func_804ABAF0(){}

void func_804ABBF4(){}

void func_804ABCA4(){}

void func_804ABD0C(){}

void func_804ABDD4(){}

void func_804ABE84(){}

void func_804ABF08(){}

void func_804AC020(){}

void func_804AC198(){}

void func_804AC3B0(){}

void func_804AC4E4(){}

void func_804AC4F4(){}

void func_804AC57C(){}

void func_804AC5D8(){}

// Default false-return handler. Used as a placeholder in collision method
// dispatch tables for boolean-returning virtual method slots that are
// not overridden (always returns 0 / false). The self pointer is unused.
int func_804AC61C(CColiObject* /*self*/) { return 0; }

void func_804AC624(){}

void func_804AC9F4(){}

void func_804ACD9C(){}

void func_804AD1E0(){}

void func_804AD410(){}

void func_804AD8FC(){}

void func_804ADD3C(){}

void func_804AE0D0(){}

void func_804AE11C(){}

void func_804AE388(){}

void func_804AE9A4(){}

void func_804AEC8C(){}

void func_804AF07C(){}

void func_804AF09C(){}

void func_804AF2F0(){}

void func_804AF310(){}

void func_804AF32C(){}

void func_804AF808(){}

void func_804AF98C(){}

void func_804AFA08(){}

// Clip the segment (pA, pB) against the frame spanned by (pC, pD): writes the
// entry/exit points of the portion of the segment that lies within the strip
// between the two parallel lines through pC and pD into pOutA/pOutB.
extern "C" void func_804AFB28(VEC3* pOutA, VEC3* pOutB, const VEC3* pA,
                              const VEC3* pB, const VEC3* pC, const VEC3* pD) {
    VEC3 v3, v1, v4;
    VEC3 scratch;
    VEC3 v2;
    VEC3Sub(&v1, pD, pC);
    VEC3Scale(&v2, &v1, lbl_eu_8066AE50);
    VEC3Sub(&v3, pB, pA);
    VEC3Sub(&v4, pC, pA);

    f32 a = VEC3Dot(&v1, &v1);
    f32 b = VEC3Dot(&v3, &v3);
    f32 c = VEC3Dot(&v3, &v2);
    f32 d = VEC3Dot(&v3, &v4);
    f32 e = VEC3Dot(&v2, &v4);

    f32 t = (b * e - c * d) / (a * b - c * c);
    f32 u = (d - c * t) / b;

    if ((u < lbl_eu_8066AE44 || u > lbl_eu_8066AE3C) &&
        (t < lbl_eu_8066AE44 || t > lbl_eu_8066AE3C)) {
        // both out of range
        f32 uc = clamp01(u);
        VEC3Scale(&scratch, &v3, uc);
        VEC3Add(pOutA, pA, &scratch);
        VEC3Sub(&v4, pOutA, pC);
        f32 s = VEC3Dot(&v1, &v4) / a;
        if (s < lbl_eu_8066AE44 || s > lbl_eu_8066AE3C) {
            f32 sc = clamp01(s);
            VEC3Scale(&scratch, &v1, sc);
            VEC3Add(pOutB, pC, &scratch);
            VEC3Sub(&v4, pOutB, pA);
            f32 s2 = VEC3Dot(&v3, &v4) / b;
            f32 sc2 = clamp01(s2);
            VEC3Scale(&scratch, &v3, sc2);
            VEC3Add(pOutA, pA, &scratch);
        } else {
            VEC3Scale(&scratch, &v1, s);
            VEC3Add(pOutB, pC, &scratch);
        }
    } else {
        if (u < lbl_eu_8066AE44 || u > lbl_eu_8066AE3C) {
            // u out, t in
            f32 uc = clamp01(u);
            VEC3Scale(&scratch, &v3, uc);
            VEC3Add(pOutA, pA, &scratch);
            VEC3Sub(&v4, pOutA, pC);
            f32 s = VEC3Dot(&v1, &v4) / a;
            f32 sc = clamp01(s);
            VEC3Scale(&scratch, &v1, sc);
            VEC3Add(pOutB, pC, &scratch);
        } else {
            if (t < lbl_eu_8066AE44 || t > lbl_eu_8066AE3C) {
                // t out, u in
                f32 tc = clamp01(t);
                VEC3Scale(&scratch, &v1, tc);
                VEC3Add(pOutB, pC, &scratch);
                VEC3Sub(&v4, pOutB, pA);
                f32 s2 = VEC3Dot(&v3, &v4) / b;
                f32 sc2 = clamp01(s2);
                VEC3Scale(&scratch, &v3, sc2);
                VEC3Add(pOutA, pA, &scratch);
            } else {
                // both in range
                VEC3Scale(&scratch, &v3, u);
                VEC3Add(pOutA, pA, &scratch);
                VEC3Scale(&scratch, &v1, t);
                VEC3Add(pOutB, pC, &scratch);
            }
        }
    }
}

void func_804B028C(){}

void func_804B06FC(){}

void func_804B073C(){}

void func_804B077C(){}

void func_804B07F0(){}

void func_804B0818(){}

void func_804B08A0(){}

void func_804B0924(){}

void __dt__804B095C(){}

void func_804B09C8(){}

void func_804B0A6C(void){}

void func_804B0A74(void){}

void func_804B0A7C(){}

void func_804B0AD4(){}

void func_804B0B0C(){}

void func_804B0B54(){}

void func_804B0C0C(){}

void func_804B0CE8(){}

void func_804B0DF4(){}

void func_804B0EA0(){}

void func_804B102C(){}

void func_804B1130(){}

void func_804B1164(){}

void func_804B192C(){}

void func_804B19CC(){}

void func_804B1AD8(){}

void func_804B1BDC(){}

void func_804B1C9C(){}

void func_804B1DC0(u8* self, int arg) {
    int* flags = (int*)((char*)self + 0xa8);
    if (arg != 0) {
        *flags &= ~0x100;
    } else {
        *flags = (*flags | 0x100) & ~0x0E;
    }
}

void func_804B1DEC(){}

void func_804B204C(){}

void func_804B21A8(){}

void func_804B236C(){}

void func_804B24A4(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804B2524(){}
