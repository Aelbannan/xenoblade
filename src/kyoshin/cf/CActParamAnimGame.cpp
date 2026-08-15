// Auto-scaffolded catalog TU for kyoshin/cf/CActParamAnimGame
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CActParamAnimGame.hpp"
#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_triangular.h>
#include <revolution/mtx/vec.h>
#include "monolib/math/CVec3.hpp"

// Retail constructor(C) at 0x8005AA64 (unmangled symbol): base-constructs
// the ::CActParamAnim subobject, stores the retail vtable manually
// (novtable class), zeroes the flag fields, invokes the vtable+0xE0 virtual,
// then parks the link argument at +0x4E8 and returns this.
cf::CActParamAnimGame* __ct__8005A3FC(cf::CActParamAnimGame* self, cf::CActParamAnimGameLink* arg) {
    __ct__13CActParamAnimFv(self);
    *(void**)self = (void*)lbl_eu_80526458;
    self->field_4EC = 0;
    self->field_4F0 = 0;
    self->field_530 = 0;
    ((cf::CActParamAnimGameVtE0*)self)->vE0();
    self->field_4E8 = arg;
    return self;
}

cf::CActParamAnimGame::CActParamAnimGame() {
    // Base ctor (__ct__13CActParamAnimFv) runs first; the retail vtable is
    // stored manually because the class is __declspec(novtable).
    *(void**)this = (void*)lbl_eu_80526458;
    field_4EC = 0;
    field_4F0 = 0;
    field_530 = 0;
    ((CActParamAnimGameVtE0*)this)->vE0();
}

cf::CActParamAnimGame::~CActParamAnimGame() {}

void cf::CActParamAnimGame::func_8005A524() {
    func_8004B114();
    // sdata2 constants written inline (const-float globals hoist the loads at
    // retail's position and allocate retail's FPR coloring, MWCC_REFERENCE
    // SDA-const hoist / FPR-coloring rules).
    field_4E8 = 0;
    field_4F4 = 0;
    field_4F8 = lbl_eu_80666040;
    field_500 = lbl_eu_80666044;
    field_508 = lbl_eu_80666044;
    field_50C = lbl_eu_80666048;
    field_52C = 0;
    field_504 = lbl_eu_80666048;
    field_532 = 0;
    field_534 = 0;
    field_536 = 2;
    field_538 = 0;
}

// Stubs for retail C-linkage functions that func_8005D2C4 tail-calls; kept
// noinline (repo pattern, see CQstLogList.cpp) so -ipa does not fold the
// call sites away, and extern "C" so the call-site reloc keeps the retail
// unmangled name (same precedent as func_8005BC14 in this file).
// func_8005A5B0 is defined below (after the collision-query typedefs).
extern "C" void func_8005A5B0(cf::CActParamAnimGame* self);

extern "C" void __declspec(noinline) func_8005B820(cf::CActParamAnimGame* self){}

typedef unsigned char u8;
typedef signed char s8;

extern "C" {
f32 SinFIdx__Q24nw4r4mathFf(f32);
f32 CosFIdx__Q24nw4r4mathFf(f32);
f32 Atan2FIdx__Q24nw4r4mathFff(f32, f32);
f32 FrSqrt__Q24nw4r4mathFf(f32);
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
void PSVECCrossProduct(const Vec*, const Vec*, Vec*);
void PSVECNormalize(const Vec*, Vec*);
f32 PSVECMag(const Vec*);
bool isTvFormatPal__9CDeviceVIFv();
f32 getSecPerFrame__9CDeviceVIFv();
f32 func_eu_8048A084();
void func_8004B7C0(void*, const ml::CVec3*);
int func_80051C40(void*);
int func_80051BF4(void*);
void func_80051CAC(void*);
f32 func_8004B34C(void*);
f32 func_80053958();
bool func_8004B354(void*, const ml::CVec3*);
bool func_8004B52C(void*, f32);
u32 func_804BE348(void*, void*, u32, u32, u32);
u32 func_804BE5A4(u32, u32);
u32 func_804BE4AC(void);
int func_804BE470(void*, void*, void*, void*, void*, f32, f32);
int func_804BE53C(void*, u32);
int func_804BE5A8(void*, u32, u32, u32);
int func_804BE604(u32);
void* func_804BE520(int);
void* func_804BE50C(u32);
u32 getTargetFramerate__9CDeviceVIFv(void);
void func_804876E4(void*, void*);
int func_804B1164(void*, ml::CVec3*, ml::CVec3*, ml::CVec3*);
int func_804B5658(void*, ml::CVec3*, ml::CVec3*, ml::CVec3*, ml::CVec3*);
void func_8004B840(void*, f32);
void func_804BE628(int);
int func_804BE5B8(void);
int func_804BE5C0(void);
u32 func_800822F4__Q22cf13CfGameManagerFv(void);
int func_80082694__Q22cf13CfGameManagerFv(int);
void func_8008269C__Q22cf13CfGameManagerFv(int, int);
void func_800826F0__Q22cf13CfGameManagerFv(int);
void func_804B0B54(void*, const ml::CVec3*);
void func_804B1130(void*, void*, void*, void*, void*);
s8 lbl_eu_80663D64;
s8 lbl_eu_80663D6C;
s8 lbl_eu_80663D74;
f32 lbl_eu_80663D60;
f32 lbl_eu_80663D68;
f32 lbl_eu_80663D70;
f32 lbl_eu_8066A1F8;
const char lbl_eu_80526324[];
const char lbl_eu_80526300[];
ml::CVec3 zero__Q22ml5CVec3;
}

// func_804BE398's retail ABI passes two FP args (f1, f2) after the four GPR
// args; the header now owns the extern "C" decl (6 GPR+FP args), and the
// already-matched func_8005BC14 4-arg call site casts to a 4-arg fn type.
typedef int (*BE398Fn)(void*, u32, u32, u32, f32, f32);
typedef int (*BE398Fn4)(void*, u32, u32, u32);
// func_804BD94C (11-arg; header-owned extern "C") cast helpers.
typedef u32 (*BD94CFn)(void*, void*, u32, u32, u32, u32, f32, f32, f32, f32, f32);
typedef u32 (*BD94CFn6)(void*, void*, u32, u32, u32, u32);

// Collision-query result object returned by func_804BE520 / func_804BE50C:
// position triple with the height at +4.
struct CActParamAnimGameBeObj {
    f32 x, y, z;
};

// Main per-frame actor update: resolves the ground/obstacle probes, runs the
// obstacle-scan loop (nearest/tallest hits), refreshes the facing vector, and
// feeds the move vec into the +0x3A0 chain. C-linkage retail symbol; called
// by func_8005D2C4.
extern "C" void __declspec(noinline) func_8005A5B0(cf::CActParamAnimGame* self) {
    self->field_47C = lbl_eu_8066604C;
    self->field_0C &= ~0x800;
    self->field_4AC = 0;
    self->field_520 = zero__Q22ml5CVec3;

    if (self->field_4EC & 0x40) {
        self->field_4EC &= ~0x40;
        goto shared;
    }

    if (self->field_270 & 0x20) {
        // Ground contact already tracked: probe straight down, then commit.
        ml::CVec3 vecA(lbl_eu_80666040, lbl_eu_80666050, lbl_eu_80666040);
        ml::CVec3 vec220 = self->field_3A8 + vecA;
        if (func_804BE470(&vec220, &self->field_520, 0, 0, 0, lbl_eu_80666054, lbl_eu_80666040)) {
            CActParamAnimGameBeObj* obj = (CActParamAnimGameBeObj*)func_804BE520(0);
            if (obj->y < 0.0f) {
                self->field_4EC |= 0x20000000;
            } else {
                ml::CVec3 vec88 = self->field_3A8 + self->field_520;
                self->field_488 += self->field_520.y;
                func_8004B354(self, &vec88);
                if (self->field_3A0) func_804876E4(self->field_3A0, &self->field_520);
            }
        }
        if ((self->field_0C & 0x80) == 0) {
            if (self->field_488 - self->field_3A8.y > lbl_eu_80666058) {
                self->field_4EC |= 0x80;
            }
        }
        goto shared;
    }

    {
        // Probe down; the f2-height path re-aims the actor when the probe
        // point lies beyond the facing half-plane.
        ml::CVec3 vecA(lbl_eu_80666040, lbl_eu_80666050, lbl_eu_80666040);
        ml::CVec3 vec214 = self->field_3A8 + vecA;
        f32 localF = 0.0f;
        if (self->field_3CC.x == 0.0f && self->field_3CC.z == 0.0f) {
            if (func_804BE470(&vec214, &self->field_520, &localF, 0, 0, lbl_eu_80666054, lbl_eu_80666040)) {
                CActParamAnimGameBeObj* obj = (CActParamAnimGameBeObj*)func_804BE520(0);
                if (obj->y < 0.0f) {
                    self->field_4EC |= 0x20000000;
                } else {
                    ml::CVec3 vec = self->field_3A8 + self->field_520;
                    self->field_488 += self->field_520.y;
                    func_8004B354(self, &vec);
                    if (localF != 0.0f) func_8004B52C(self, localF + self->field_444);
                    if (self->field_3A0) func_804876E4(self->field_3A0, &self->field_520);
                }
            }
        } else {
            if (func_804BE470(&vec214, &self->field_520, 0, 0, 0, lbl_eu_80666054, lbl_eu_80666040)) {
                CActParamAnimGameBeObj* obj = (CActParamAnimGameBeObj*)func_804BE520(0);
                if (obj->y < 0.0f) {
                    self->field_4EC |= 0x20000000;
                } else {
                    ml::CVec3 vec = self->field_3A8 + self->field_520;
                    self->field_488 += self->field_520.y;
                    func_8004B354(self, &vec);
                    if (self->field_3A0) func_804876E4(self->field_3A0, &self->field_520);
                }
            }
        }
        self->field_4EC &= ~0x40000;
        goto shared;
    }

shared:
    {
        ml::CVec3 vecA(lbl_eu_80666040, lbl_eu_80666060, lbl_eu_80666040);
        ml::CVec3 vec58 = self->field_3A8 + vecA;
        *(u32*)&self->field_3E4.x = *(u32*)&vec58.x;
        *(u32*)&self->field_3E4.y = *(u32*)&vec58.y;
        *(u32*)&self->field_3E4.z = *(u32*)&vec58.z;
        f32 f31v = lbl_eu_8066605C;
        ml::CVec3 vecB(lbl_eu_80666040, lbl_eu_80666064, lbl_eu_80666040);
        ml::CVec3 vec4C = self->field_3A8 + vecB;
        ml::CVec3 vec124 = vec4C;
        int r31v = 1;
        if (((BE398Fn)func_804BE398)(&vec124, 0xa04, 0, 1, lbl_eu_80666060, lbl_eu_80666040)) {
            int count = func_804BE4AC();
            f32 f30 = lbl_eu_80666068 + self->field_3A8.y;
            int r30v = -1;
            f32 f29 = lbl_eu_8066606C + self->field_3A8.y;
            int r29v = -1;
            f32 f28 = f30 - lbl_eu_80666070;
            f32 f24 = lbl_eu_80666050;
            int r28v = -1;
            f32 f27 = lbl_eu_80666074 + f30;
            int r27v = -1;
            f32 f26 = f30 - f24;
            int r26v = -1;
            f32 f23 = f28;
            int r25v;
            for (r25v = 0; r25v < count; r25v++) {
                ml::CVec3 vec208;
                func_804BE4B4(&vec208, r25v);
                if (func_804BE5A4(0x40000, r25v)) {
                    f32 y = vec208.y;
                    if (y >= f29) {
                        if (f27 > y) { f27 = y; r29v = r25v; }
                    } else {
                        if (f24 + self->field_3A8.y > y) {
                            if (f26 < y) { f26 = y; r27v = r25v; }
                            if (f23 < y) { f23 = y; r26v = r25v; }
                        }
                    }
                } else {
                    CActParamAnimGameBeObj* obj = (CActParamAnimGameBeObj*)func_804BE520(r25v);
                    f32 y = vec208.y;
                    if (obj->y > 0.0f) {
                        if (f30 > y) {
                            if (f28 < y) { f28 = y; r30v = r25v; }
                            if (f23 < y) { f23 = y; r26v = -1; }
                        }
                    } else {
                        if (f29 > y) {
                            if (f28 < y) { f28 = y; r30v = r25v; }
                            if (f23 < y) { f23 = y; r26v = -1; }
                        }
                    }
                }
            }
            if (r30v != -1) {
                r28v = r30v;
                if (self->field_3C0.y <= 0.0f) {
                    if (ml::math::abs(self->field_3A8.y - f28) <= lbl_eu_8066606C) {
                        if ((self->field_4EC & 4) == 0) {
                            ml::CVec3 vec1fc;
                            if (func_804BE53C(&vec1fc, r30v)) {
                                f32 fr = (f32)getTargetFramerate__9CDeviceVIFv();
                                vec1fc *= fr;
                                *(u32*)&self->field_3F0.x = *(u32*)&vec1fc.x;
                                *(u32*)&self->field_3F0.y = *(u32*)&vec1fc.y;
                                *(u32*)&self->field_3F0.z = *(u32*)&vec1fc.z;
                                r31v = 0;
                            }
                        }
                    }
                }
                self->field_47C = f28;
            }
            if (r29v != -1) {
                r25v = 0;
                self->field_4FC = f27;
                if (r30v != -1) self->field_4F8 = f27 - f28;
                r28v = r29v;
                if ((self->field_4EC & 4) == 0) {
                    ml::CVec3 vec1f0;
                    if (func_804BE53C(&vec1f0, r29v)) {
                        f32 fr = (f32)getTargetFramerate__9CDeviceVIFv();
                        vec1f0 *= fr;
                        *(u32*)&self->field_3F0.x = *(u32*)&vec1f0.x;
                        *(u32*)&self->field_3F0.y = *(u32*)&vec1f0.y;
                        *(u32*)&self->field_3F0.z = *(u32*)&vec1f0.z;
                        r31v = 0;
                    }
                }
                if (self->field_4F8 > self->field_508) {
                    f23 = lbl_eu_80666044 + self->field_3A8.y;
                    int r22v = -1;
                    f27 = lbl_eu_8066607C;
                    f29 = lbl_eu_80666040;
                    f24 = lbl_eu_80666078 + f23;
                    int r21v;
                    for (r21v = 0; r21v < count; r21v++) {
                        if (((CActParamAnimGameBeObj*)func_804BE520(r21v))->y < 0.0f
                            && ((CActParamAnimGameBeObj*)func_804BE520(r21v))->y >= f27) {
                            f32 g = ((CActParamAnimGameBeObj*)func_804BE50C(r21v))->y;
                            if (f23 < g && f24 > g) { f24 = g; r22v = r21v; }
                        }
                    }
                    if (r22v != -1) {
                        ml::CVec3 vec1e4;
                        func_804BE4E0(&vec1e4, r22v);
                        if (vec1e4.x * self->field_3CC.x + vec1e4.z * self->field_3CC.z <= 0.0f) {
                            ml::CVec3 vec1d8;
                            vec1d8.x = vec1e4.x;
                            vec1d8.y = 0.0f;
                            vec1d8.z = vec1e4.z;
                            f32 len2 = vec1d8.y * vec1d8.y + vec1d8.x * vec1d8.x + vec1d8.z * vec1d8.z;
                            if (len2 == 0.0f) {
                                vec1d8 = zero__Q22ml5CVec3;
                            } else {
                                PSVECNormalize((const Vec*)&vec1d8, (Vec*)&vec1d8);
                            }
                            ml::CVec3 up(0.0f, 1.0f, 0.0f);
                            ml::CVec3 cross;
                            PSVECCrossProduct((const Vec*)&vec1d8, (const Vec*)&up, (Vec*)&cross);
                            f32 clen2 = cross.y * cross.y + cross.x * cross.x + cross.z * cross.z;
                            if (clen2 == 0.0f) {
                                cross = zero__Q22ml5CVec3;
                            } else {
                                PSVECNormalize((const Vec*)&cross, (Vec*)&cross);
                            }
                            f32 dot = cross.x * self->field_3CC.x + cross.z * self->field_3CC.z;
                            f32 fz = 0.0f;
                            f32 fx = cross.x * dot;
                            f32 fz2 = cross.z * dot;
                            *(u32*)&self->field_3CC.y = *(u32*)&fz;
                            *(u32*)&self->field_3CC.x = *(u32*)&fx;
                            *(u32*)&self->field_3CC.z = *(u32*)&fz2;
                            *(u32*)&self->field_3F0.x = *(u32*)&vec1d8.x;
                            *(u32*)&self->field_3F0.y = *(u32*)&vec1d8.y;
                            *(u32*)&self->field_3F0.z = *(u32*)&vec1d8.z;
                            r25v = 1;
                            r31v = 0;
                        }
                    }
                }
            }
            if (r28v != -1) {
                CActParamAnimGameBeObj* o = (CActParamAnimGameBeObj*)func_804BE50C(r28v);
                *(u32*)&self->field_3E4.x = *(u32*)&o->x;
                *(u32*)&self->field_3E4.y = *(u32*)&o->y;
                *(u32*)&self->field_3E4.z = *(u32*)&o->z;
                CActParamAnimGameBeObj* o2 = (CActParamAnimGameBeObj*)func_804BE520(r28v);
                *(u32*)&self->field_3D8.x = *(u32*)&o2->x;
                *(u32*)&self->field_3D8.y = *(u32*)&o2->y;
                *(u32*)&self->field_3D8.z = *(u32*)&o2->z;
                void* sub = ((cf::CActParamAnimGameObj3A0*)self->field_3A0)->sub7EC;
                if (sub != 0) {
                    ml::CVec3 vec;
                    vec.x = self->field_3E4.x;
                    vec.y = self->field_3E4.y;
                    vec.z = self->field_3E4.z;
                    ((cf::CActParamAnimGameObj7EC*)sub)->v14(&vec);
                }
                if ((self->field_4EC & 0x20000000) && r25v == 0) {
                    f32 dot = self->field_3D8.x * self->field_3CC.x + self->field_3E4.z * self->field_3CC.z;
                    if (dot <= 0.0f) {
                        ml::CVec3 v1c0(self->field_3D8.x, 0.0f, self->field_3E4.z);
                        ml::CVec3 up2(0.0f, 1.0f, 0.0f);
                        ml::CVec3 cross2;
                        PSVECCrossProduct((const Vec*)&v1c0, (const Vec*)&up2, (Vec*)&cross2);
                        f32 clen2b = cross2.y * cross2.y + cross2.x * cross2.x + cross2.z * cross2.z;
                        if (clen2b == 0.0f) {
                            cross2 = zero__Q22ml5CVec3;
                        } else {
                            PSVECNormalize((const Vec*)&cross2, (Vec*)&cross2);
                        }
                        f32 dot2 = cross2.x * self->field_3CC.x + cross2.z * self->field_3CC.z;
                        f32 fz = 0.0f;
                        f32 fx = cross2.x * dot2;
                        f32 fz2 = cross2.z * dot2;
                        *(u32*)&self->field_3CC.y = *(u32*)&fz;
                        *(u32*)&self->field_3CC.x = *(u32*)&fx;
                        *(u32*)&self->field_3CC.z = *(u32*)&fz2;
                    }
                }
                u32 local8 = 0;
                if (r30v != -1) {
                    if (func_804BE5A4(0x10000, r30v)) {
                        func_804BE5A8(&local8, 0x10000, r30v, 0);
                    }
                } else if (r29v != -1) {
                    if (func_804BE5A4(0x10000, r29v)) {
                        func_804BE5A8(&local8, 0x10000, r29v, 0);
                    }
                }
                self->field_4AC = local8;
                if (func_804BE604(r28v)) {
                    self->field_4EC |= 0x1000000;
                }
                if (func_804BE5A4(0x4000, r28v)) {
                    if (ml::math::abs(self->field_3A8.y - f28) <= lbl_eu_80666050) {
                        self->field_4EC |= 0x80;
                    }
                }
                if (r27v != -1 && r29v == -1) {
                    if (f26 < f28) r27v = r28v;
                } else {
                    r27v = r28v;
                }
                if (func_804BE5A4(0x20000, r27v)) {
                    f32 objY = ((CActParamAnimGameBeObj*)func_804BE50C(r27v))->y;
                    if (self->field_3A8.y - objY <= lbl_eu_80666044) {
                        self->field_4EC |= 0x1000000;
                        func_804BE5A8(&local8, 0x20000, r27v, 1);
                        self->field_4B0 = local8;
                    }
                }
                if (func_804BE5A4(0x8000, r28v)) {
                    self->field_4EC |= 0x800000;
                } else {
                    self->field_4EC &= ~0x800000;
                }
                if (func_804BE5A4(0x400, r28v)) {
                    if ((self->field_0C & 2) != 0 && (self->field_270 & 0x20) == 0) {
                        ml::CVec3 vec1a8;
                        func_804BE4E0(&vec1a8, r28v);
                        ml::CVec3 vec19c = self->field_3F0;
                        f32 f5 = self->field_390 * getSecPerFrame__9CDeviceVIFv();
                        ml::CVec3 tmp = self->field_3C0 * f5;
                        ml::CVec3 vec190 = tmp;
                        vec190 += self->field_3CC;
                        if (vec190.x == 0.0f && vec190.z == 0.0f) {
                            f31v = lbl_eu_80666090;
                            if (vec1a8.y <= f31v) {
                                vec19c.x += vec1a8.x * lbl_eu_80666098;
                                vec19c.z += vec1a8.z * lbl_eu_80666098;
                            }
                        } else {
                            if (f5 != 0.0f) f5 = 1.0f / f5;
                            if (self->field_0C & 0x200000) {
                                vec19c += vec190 * (lbl_eu_80666084 * f5);
                            } else {
                                vec19c += vec190 * (lbl_eu_80666080 * f5);
                            }
                            f32 len2 = vec19c.x * vec19c.x + vec19c.z * vec19c.z;
                            if (len2 >= lbl_eu_80666088) {
                                if (!(len2 >= 0.0f)) {
                                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                                }
                                f32 len = len2 <= 0.0f ? 0.0f : len2 * FrSqrt__Q24nw4r4mathFf(len2);
                                f32 inv = 1.0f / len;
                                vec19c.x *= lbl_eu_8066608C * inv;
                                vec19c.z *= lbl_eu_8066608C * inv;
                            }
                            if (vec1a8.y <= lbl_eu_80666090) {
                                f32 len2b = vec190.x * vec190.x + vec190.z * vec190.z;
                                if (!(len2b >= 0.0f)) {
                                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                                }
                                f32 lenb = len2b <= 0.0f ? 0.0f : len2b * FrSqrt__Q24nw4r4mathFf(len2b);
                                f32 invb = 1.0f / lenb;
                                f32 nvx = vec190.x * invb;
                                f32 nvz = vec190.z * invb;
                                f32 len2c = vec1a8.x * vec1a8.x + vec1a8.z * vec1a8.z;
                                if (!(len2c >= 0.0f)) {
                                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                                }
                                f32 lenc = len2c <= 0.0f ? 0.0f : len2c * FrSqrt__Q24nw4r4mathFf(len2c);
                                f32 invc = 1.0f / lenc;
                                f32 dot = nvz * (vec1a8.z * invc) + nvx * (vec1a8.x * invc);
                                if (dot < 0.0f) dot = -dot;
                                dot *= lbl_eu_80666098;
                                vec19c += ml::CVec3(vec1a8.x * dot, 0.0f, vec1a8.z * dot);
                            }
                            f31v = lbl_eu_8066609C;
                        }
                        *(u32*)&self->field_3F0.x = *(u32*)&vec19c.x;
                        *(u32*)&self->field_3F0.y = *(u32*)&vec19c.y;
                        *(u32*)&self->field_3F0.z = *(u32*)&vec19c.z;
                        self->field_0C |= 0x30;
                    } else {
                        self->field_0C &= ~0x40000;
                    }
                } else {
                    self->field_0C &= ~0x40000;
                }
                if (r26v != -1) {
                    self->field_4EC |= 0x8000000;
                }
                if (self->field_530 & 0x400) {
                    if (self->field_270 & 0x20) {
                        ml::CVec3 diff = self->field_3A8 - self->field_3B4;
                        f32 len2 = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
                        if (len2 <= lbl_eu_806660A0) {
                            if (func_804BE348(&self->field_3A8, &self->field_3B4, 0, 0x4000, 0)) {
                                self->field_4EC |= 0x80;
                            }
                        }
                    }
                }
                if (r31v != 0) {
                    ml::CVec3 v = self->field_3F0 * f31v;
                    *(u32*)&self->field_3F0.x = *(u32*)&v.x;
                    *(u32*)&self->field_3F0.y = *(u32*)&v.y;
                    *(u32*)&self->field_3F0.z = *(u32*)&v.z;
                }
            }
        }
    }
}


struct CActParamAnimGameViewBC14 {
    u8 _00[0x08];
    void* owner;            // 0x08
    u32 flags0C;            // 0x0C
    u8 _10[0x260];
    u32 flags270;           // 0x270
    u8 _274[0x11C];
    f32 f390;               // 0x390
    f32 f394;               // 0x394
    f32 f398;               // 0x398
    u8 _39C[0x4];
    void* obj3A0;           // 0x3A0
    u8 _3A4[0x4];
    f32 posX3A8;            // 0x3A8
    f32 posY3AC;            // 0x3AC
    f32 posZ3B0;            // 0x3B0
    u8 _3B4[0x0C];
    f32 vec3C0[3];          // 0x3C0
    u8 _3CC[0x24];
    f32 vec3F0[3];          // 0x3F0
    u8 _3FC[0x34];
    f32 f430;               // 0x430
    f32 f434;               // 0x434
    u8 _438[0x04];
    f32 f43C;               // 0x43C
    f32 f440;               // 0x440
    f32 f444;               // 0x444
    u8 _448[0x34];
    f32 f47C;               // 0x47C
    u8 _480[0x08];
    f32 f488;               // 0x488
    u8 _48C[0x1C];
    u32 filter4A8;          // 0x4A8
    u8 _4AC[0x3C];
    void* region4E8;        // 0x4E8
    u32 flags4EC;           // 0x4EC
    u8 _4F0[0x8];           // 0x4F0
    f32 f4F8;               // 0x4F8
    u8 _4FC[0xC];           // 0x4FC
    f32 f508;               // 0x508
    u8 _50C[0x8];           // 0x50C
    f32 f514;               // 0x514
    f32 f518;               // 0x518
    f32 f51C;               // 0x51C
    u8 _520[0x0C];
    u32 state52C;           // 0x52C
    u16 flags530;           // 0x530
};

// Sets flag bits on the actor object: 0x160 in the +0xC word and bit 0 in
// the +0x4EC word (retail: two independent read-modify-write pairs).
void func_8005A594(CActParamAnimGameViewBC14* self) {
    self->flags0C |= 0x160;
    self->flags4EC |= 1;
}

static float bc14CallVfn(void* self, u32 off) {
    return ((float (*)(void*))(*(void**)((u8*)*(void**)self + off)))(self);
}
static int bc14CallVfnI(void* self, u32 off) {
    return ((int (*)(void*))(*(void**)((u8*)*(void**)self + off)))(self);
}

extern "C" void func_8005BC14(void* selfV) {
    CActParamAnimGameViewBC14* self = (CActParamAnimGameViewBC14*)selfV;
    s32 moved = 0;
    ml::CVec3 pos;
    ml::CVec3 up;
    ml::CVec3 dir;
    ml::CVec3 up2;
    ml::CVec3 dir2;
    ml::CVec3 accel;
    ml::CVec3 target;
    ml::CVec3 dirM;

    self->flags530 &= 0xFF3F;
    self->state52C = 0;
    self->flags4EC &= ~0xC0000;
    self->f394 = 1.0f;
    self->f398 = 1.0f;
    *(ml::CVec3*)((u8*)self + 0x3F0) = zero__Q22ml5CVec3;
    func_8004B7C0(self, &zero__Q22ml5CVec3);
    func_80051C40(self);

    if ((self->flags0C & 0x100) != 0) goto exit;
    if (self->obj3A0 == 0) goto exit;

    moved = 1;
    u32 flags = self->flags530 | 0x800;
    self->flags530 = (u16)flags;
    pos.x = self->posX3A8;
    pos.y = self->posY3AC;
    pos.z = self->posZ3B0;
    if ((flags & 0xC) == 0) goto mode0;

    self->flags530 |= 0x80;
    if ((self->flags530 & 2) == 0) goto bit1clear;

    if (self->flags530 & 4) pos.y += 0.5f;
    if (self->flags530 & 8) {
        pos.y -= 2.0f;
        pos.x = self->f514 - 2.0f * SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444);
        pos.z = self->f51C - 2.0f * CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444);
    }
    self->flags530 &= 0xFFF1;
    goto commit;

bit1clear:
    {
        f32 f31v = isTvFormatPal__9CDeviceVIFv() ? 1.2f : 1.0f;
        f32 f29v = func_eu_8048A084();
        f32 f30v = bc14CallVfn(self, 0x14) - f29v;
        f32 f29b = bc14CallVfn(self, 0x0C);
        if (bc14CallVfn(self, 0x10) * f31v + f29b >= f30v) {
        } else {
            self->flags530 |= 2;
        }
    }
    moved = 0;
    pos.x += 0.1f * (self->f514 - pos.x);
    pos.z += 0.1f * (self->f51C - pos.z);
    {
        dir = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
        up = pos + ((self->flags530 & 4) ? ml::CVec3(0.0f, 1.0f, 0.0f) : ml::CVec3(0.0f, -1.5f, 0.0f));
        ml::CVec3 t3 = up + dir * 3.0f;
        if (func_804BE348(&up, &t3, self->filter4A8, 0, 0) != 0
            && func_804BE5A4(8192, 0) != 0) {
            ml::CVec3 v;
            func_804BE4E0(&v, 0);
            if (v.y <= 0.7071f) {
                func_8004B52C(self, 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(-v.x, -v.z));
            }
        }
    }
commit:
    if (self->region4E8 != 0) func_804B0B54((u8*)self->region4E8 + 0x60C, &pos);
    func_8004B354(self, &pos);
    goto next;

mode0:
    if ((flags & 0x30) != 0) {
        up2 = pos + ml::CVec3(0.0f, 1.0f, 0.0f);
        dir2 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
        ml::CVec3 t3b = up2 + dir2 * 2.0f;
        if (func_804BE348(&up2, &t3b, self->filter4A8, 0, 0) != 0
            && func_804BE5A4(8192, 0) != 0) {
            ml::CVec3 v;
            func_804BE4E0(&v, 0);
            if (v.y <= 0.7071f) {
                func_8004B52C(self, 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(-v.x, -v.z));
            }
        }
        if (self->flags530 & 0x20) {
            pos.y += 0.1f * (self->f518 - pos.y);
            func_8004B354(self, &pos);
        }
        moved = 0;
        goto next;
    }
    if ((self->flags530 & 0x100) != 0) {
        f32 dx = self->f514 - pos.x;
        f32 dz = self->f51C - pos.z;
        if (dx * dx + dz * dz < 0.0001f) {
            self->flags530 &= 0xFEFF;
        } else {
            pos.x += 0.3f * dx;
            pos.z += 0.3f * dz;
            func_8004B354(self, &pos);
            moved = 0;
        }
    }
next:
    if (self->flags530 & 0x200) {
        if (bc14CallVfnI(self->owner, 0x10) != 0 || func_80051BF4(self) != 0) {
            self->flags530 &= 0xFDFF;
        } else {
            if (lbl_eu_80663D64 == 0) { lbl_eu_80663D60 = 60.0f * 0.017453292f; lbl_eu_80663D64 = 1; }
            if (lbl_eu_80663D6C == 0) { lbl_eu_80663D68 = -60.0f * 0.017453292f; lbl_eu_80663D6C = 1; }
            f32 ang = *(f32*)((u8*)self->owner + 0xC);
            if (!(ang <= lbl_eu_80663D60) || ang <= lbl_eu_80663D68) {
                self->flags530 &= 0xFDFF;
            }
        }
    }
    if (moved == 0) goto exit;

    f32 f28v = 0.0f;
    f32 f29v;
    f32 f31v;
    f32 f30v;
    accel = ml::CVec3(0.0f, 0.0f, 0.0f);
    if ((self->flags530 & 0x200) == 0) {
        f29v = *(f32*)((u8*)self->owner + 0x14);
        if (f29v * self->f430 >= func_80053958()) {
            f28v = *(f32*)((u8*)self->owner + 0x14) * self->f394;
        }
        f31v = f28v * (1000.0f * func_8004B34C(self) / 3600.0f) * (self->f390 * getSecPerFrame__9CDeviceVIFv());
        if (self->flags530 & 0x400) {
            f32 a = *(f32*)((u8*)self->owner + 0xC);
            f30v = fabsf(f31v * SinFIdx__Q24nw4r4mathFf(40.743663f * a));
            accel.y = f31v * CosFIdx__Q24nw4r4mathFf(40.743663f * a);
            ml::CVec3 d2 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
            ml::CVec3 d3 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f440), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f440));
            ml::CVec3 upv = ml::CVec3(0.0f, 1.0f, 0.0f);
            ml::CVec3 cross;
            PSVECCrossProduct((const Vec*)&d2, (const Vec*)&upv, (Vec*)&cross);
            f32 len2 = cross.x * cross.x + cross.y * cross.y + cross.z * cross.z;
            if (len2 == 0.0f) {
                cross = zero__Q22ml5CVec3;
            } else {
                PSVECNormalize((const Vec*)&cross, (Vec*)&cross);
            }
            f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&cross, (const nw4r::math::VEC3*)&d3);
            if (dot <= 0.0f) f30v = -f30v;
            accel.x = cross.x * f30v;
            accel.z = cross.z * f30v;
        } else {
            accel.x = f31v * SinFIdx__Q24nw4r4mathFf(40.743663f * self->f440);
            accel.z = f31v * CosFIdx__Q24nw4r4mathFf(40.743663f * self->f440);
            ml::CVec3 d4 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
            ml::CVec3 d5 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f440), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f440));
            f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&d4, (const nw4r::math::VEC3*)&d5);
            accel.y = f31v * dot;
            f32 scale = 1.0f - fabsf(dot);
            accel.x *= scale;
            accel.z *= scale;
        }
    }
    {
        target.x = self->posX3A8;
        target.y = self->posY3AC;
        target.z = self->posZ3B0;
        dirM = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
        moved = 1;
        u32 hit;
        if (accel.x == 0.0f && accel.y == 0.0f && accel.z == 0.0f) {
            ml::CVec3 tpd = target + dirM * 2.0f;
            hit = func_804BE348(&target, &tpd, 0, 8192, 0) != 0;
        } else {
            ml::CVec3 va = target + accel;
            ml::CVec3 vb = dirM * 3.0f;
            ml::CVec3 vc = va - dirM;
            ml::CVec3 vd = vc + vb;
            hit = func_804BE348(&vc, &vd, 0, 8192, 0) != 0;
            if (hit == 0) {
                ml::CVec3 t2 = target + dirM * 2.0f;
                hit = func_804BE348(&target, &t2, 0, 8192, 0) != 0;
            }
        }
        if (hit == 0) {
            target += ml::CVec3(0.01f, 0.01f, 0.01f);
            ml::CVec3 tpd = target + dirM * 2.0f;
            hit = func_804BE348(&target, &tpd, 0, 8192, 0) != 0;
        }
        if (hit == 0) goto after_hit;
        f32 nx;
        f32 nz;
        ml::CVec3 v;
        func_804BE4E0(&v, 0);
        if (v.y > 0.7071f) goto after_hit;
        nx = -v.x;
        nz = -v.z;
        f32 len2 = nx * nx + nz * nz;
        if (!(len2 >= 0.0f)) {
            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 627, lbl_eu_80526300);
        }
        f32 len;
        if (len2 <= 0.0f) {
            len = 0.0f;
        } else {
            len = len2 * FrSqrt__Q24nw4r4mathFf(len2);
        }
        if (len > 0.001f) {
            nx *= 1.0f / len;
            nz *= 1.0f / len;
            nx = 0.3f * (nx - dirM.x) + dirM.x;
            nz = 0.3f * (nz - dirM.z) + dirM.z;
            len2 = nx * nx + nz * nz;
            if (!(len2 >= 0.0f)) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 627, lbl_eu_80526300);
            }
            f32 lenb;
            if (len2 <= 0.0f) {
                lenb = 0.0f;
            } else {
                lenb = len2 * FrSqrt__Q24nw4r4mathFf(len2);
            }
            if (lenb != 0.0f) {
                nx = nx / lenb;
                nz = nz / lenb;
            } else {
                nx = -v.x;
                nz = -v.z;
            }
        }
        f28v = 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(nx, nz);
        func_8004B52C(self, f28v);
        dirM.x = SinFIdx__Q24nw4r4mathFf(40.743663f * f28v);
        dirM.z = CosFIdx__Q24nw4r4mathFf(40.743663f * f28v);
        if (accel.x == 0.0f && accel.y == 0.0f && accel.z == 0.0f) goto ground;
        {
            f32 f2v = 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(accel.z * dirM.x - accel.x * dirM.z, accel.y);
            if (lbl_eu_80663D74 == 0) { lbl_eu_80663D70 = lbl_eu_8066A1F8 * 0.25f; lbl_eu_80663D74 = 1; }
            f32 p4 = lbl_eu_80663D70;
            if (p4 >= f2v && f2v >= -p4) {
                self->state52C = 1;
            } else if (lbl_eu_8066A1F8 - p4 <= f2v || f2v <= -(lbl_eu_8066A1F8 - p4)) {
                self->state52C = 2;
            } else if (p4 < f2v && f2v < 3.0f * p4) {
                self->state52C = 4;
            } else if (-p4 > f2v && f2v > 3.0f * -p4) {
                self->state52C = 3;
            }
        }
ground:
        {
            ml::CVec3 v2;
            func_804BE4B4(&v2, 0);
            f32 ddx = pos.y - v2.y;
            f32 ddz = pos.x - v2.x;
            if (ddx * ddx + ddz * ddz < 0.0064f) {
                pos.x = v2.x - 0.08f * nx;
                pos.z = v2.z - 0.08f * nz;
            }
        }
        if (self->flags270 & 0x100) {
            accel *= 0.2f;
            ml::CVec3 v2;
            func_804BE4B4(&v2, 0);
            if (self->region4E8 != 0) {
                ml::CVec3 out;
                ml::CVec3 sum = v2 + accel;
                func_804B1130((u8*)self->region4E8 + 0x60C, &out, &sum, &sum, &v2);
                accel += out;
            }
            {
                ml::CVec3 target2;
                target2.x = pos.x;
                target2.y = pos.y + 0.1f;
                target2.z = pos.z;
                if (accel.x != 0.0f || accel.y != 0.0f || accel.z != 0.0f) {
                    if (accel.x != 0.0f || accel.z != 0.0f) {
                        ml::CVec3 v2c0 = ml::CVec3(accel.x, 0.0f, accel.z);
                        f32 mag = PSVECMag((const Vec*)&v2c0);
                        v2c0 *= 1.0f / mag * (0.4f + mag);
                        ml::CVec3 tp = target2 + v2c0;
                        if (func_804BE348(&target2, &tp, self->filter4A8, 0, 0) != 0) {
                            func_804BE4B4(&v2c0, 0);
                            accel.x = v2c0.x - target2.x;
                            accel.z = v2c0.z - target2.z;
                        }
                    }
                    ((BD94CFn6)func_804BD94C)(&target2, &accel, self->filter4A8, 0, (self->flags0C & 0x100) != 0, 0);
                }
                target2.y += 0.9f;
                if (((BE398Fn4)func_804BE398)(&target2, self->filter4A8, 0, 0) != 0) {
                    void* e = func_804BE50C(0);
                    target2.y -= 1.4f - (*(f32*)((u8*)e + 0x4) - target2.y);
                }
                {
                    ml::CVec3 d2 = dirM * 2.0f;
                    ml::CVec3 tmd = target2 - dirM;
                    ml::CVec3 tpd = target2 + d2;
                    u32 hit2 = func_804BE348(&tmd, &tpd, 0, 8192, 0) != 0;
                    if (hit2 == 0) {
                        target2 += ml::CVec3(0.01f, 0.01f, 0.01f);
                        ml::CVec3 tmd2 = target2 - dirM;
                        ml::CVec3 tpd2 = target2 + dirM * 2.0f;
                        hit2 = func_804BE348(&tmd2, &tpd2, 0, 8192, 0) != 0;
                    }
                    if (hit2 != 0 && v.y <= 0.7071f) {
                        ml::CVec3 v3;
                        func_804BE4B4(&v3, 0);
                        pos.x = v3.x - 0.08f * nx;
                        pos.y = v3.y - 1.0f;
                        pos.z = v3.z - 0.08f * nz;
                        moved = 0;
                    }
                }
            }
        }
    }
after_hit:
    func_8004B354(self, &pos);
    if (moved == 0) goto exit;
    self->flags530 &= 0xF7FF;
    self->flags4EC |= 0x40000;
    // func_8004B9D4 is declared in kyoshin/action/CActParamAnim.hpp
    // (CActParamAnim* first arg); cast the view pointer at the call site.
    func_8004B9D4((CActParamAnim*)self, 6, 0, -1, 0);
    func_8004B52C(self, self->f444);
    {
        ml::CVec3 zero2 = ml::CVec3(0.0f, 0.0f, 0.0f);
        func_8004B7C0(self, &zero2);
        ml::CVec3 d = ml::CVec3(0.2f * SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, 0.2f * CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
        pos = pos - d;
        func_8004B354(self, &pos);
    }
exit:
    self->f488 = self->f47C = self->posY3AC;
    func_80051CAC(self);
}

void cf::CActParamAnimGame::func_8005D2C4() {
    // Snapshot the flag word, clear it to the run mask, and reset the
    // air/height state; the +0x530 flags lose their low bit. The first
    // +0x4EC read is done through a volatile cast so MWCC keeps two loads
    // (retail feeds the copy and the mask from separate loads).
    field_4F0 = *(volatile u32*)&field_4EC;
    field_4F8 = lbl_eu_80666040;
    field_4FC = lbl_eu_80666040;
    field_4EC &= 0x14AE5A7D;
    field_530 &= 0xFFFE;

    // Child-data timers: keep the current value only when it is non-negative.
    f32 timerA = func_80055DD4(_10);
    if (timerA >= lbl_eu_80666040) field_500 = timerA;
    if (field_504 >= lbl_eu_80666040) field_500 = field_504;
    f32 timerB = func_80055DB8(_10);
    if (timerB >= lbl_eu_80666040) field_508 = timerB;
    if (field_50C >= lbl_eu_80666040) field_508 = field_50C;

    func_804BC9EC__Fv();
    if (func_804BCC10() != 0 && (field_4EC & 1) == 0) {
        func_8005A5B0(this);
    }

    if (field_374 == 2) {
        // In-battle mode: mark ready and run the full movement/step logic.
        // Load both flag words up front (retail interleaves the two RMWs;
        // the +0xC word is born first so it colors r0, the +0x530 word r4).
        u32 flags0C = field_0C;
        u16 flags530 = field_530;
        field_530 = flags530 | 1;
        field_0C = flags0C & ~0x80000;
        func_8005BC14(this);
    } else {
        bool over = field_4F8 > field_508;
        if (over && (field_0C & 0x10000) == 0) {
            // Air timer exceeded the fall timer: switch to falling state.
            field_4EC |= 2;
            field_0C &= ~0x80000;
            func_8005B820(this);
            goto d2c4_fall_done;
        }
        {
            u16 flags = field_530;
            if (flags & 0x30) {
                field_530 &= 0xF5CF;
                func_8004B354(this, (ml::CVec3*)&field_514);
            } else if (flags & 0x800) {
                // Small lateral pull toward the facing direction.
                ml::CVec3 v;
                v.x = field_3A8.x;
                f32 fidxScale = lbl_eu_806660CC;
                v.y = field_3A8.y;
                v.z = field_3A8.z;
                field_530 &= 0xF7FF;
                v.x -= lbl_eu_80666068 * nw4r::math::SinFIdx(fidxScale * field_444);
                v.z -= lbl_eu_80666068 * nw4r::math::CosFIdx(fidxScale * field_444);
                func_8004B354(this, &v);
            }
            if (field_4F8 > lbl_eu_80666040) {
                // Airborne: pick the ground-following scale pair from the
                // height delta below the actor.
                f32 drop = field_4FC - field_3A8.y;
                if (field_4EC & 0x100) {
                    field_394 = lbl_eu_80666080;
                    field_398 = lbl_eu_8066610C;
                } else if (drop < lbl_eu_806660D4) {
                    field_394 = lbl_eu_80666044;
                    field_398 = lbl_eu_80666044;
                } else if (drop < lbl_eu_80666110) {
                    field_394 = lbl_eu_80666044;
                    field_398 = lbl_eu_80666114;
                } else {
                    field_394 = lbl_eu_80666110;
                    field_398 = lbl_eu_80666118;
                }
            } else {
                field_394 = lbl_eu_80666044;
                field_398 = lbl_eu_80666044;
            }
            func_80052934();
        }
    d2c4_fall_done:;
    }

    // Notify the +0x3A0 chain's +0x7EC sub-object when the fall state and
    // distance from ground allow it.
    void* obj = field_3A0;
    if (obj != 0) {
        void* sub = ((CActParamAnimGameObj3A0*)obj)->sub7EC;
        if (sub != 0) {
            int arg = 0;
            if (field_4AC < 5 && field_4F8 < lbl_eu_806660D4 && (field_4EC & 0x10) == 0) {
                arg = 1;
            }
            ((CActParamAnimGameObj7EC*)sub)->v8(arg);
        }
    }

    // Link handoff: when the falling flag was newly set, detach the linked
    // region and reset its two virtual state slots.
    if ((field_4EC & 2) != 0 && (field_4F0 & 2) == 0) {
        void* link = func_8016FE34(field_4E8);
        if (link != 0) {
            ((CActParamAnimGameLinkVt*)link)->v194();
            ((CActParamAnimGameLinkVt*)link)->v17C();
        }
    }
}

void cf::CActParamAnimGame::func_8005D608() {}

// C-ABI sibling (retail unmangled name; defined in kyoshin/action/CActParamAnim.cpp)
extern "C" void func_8004BC94(void*);

// Returns true when the +0x4F8 float exceeds the +0x508 float; the linked
// region is then notified via func_8004BC94. The explicit bool local keeps
// MWCC's dead mfcr/rlwinm capture of the fcmpo result (retail `extrwi.` shape).
bool cf::CActParamAnimGame::func_8005D67C() {
    CActParamAnimGameViewBC14* self = reinterpret_cast<CActParamAnimGameViewBC14*>(this);
    bool cond = self->f4F8 > self->f508;
    if (cond) {
        func_8004BC94(this);
        return true;
    }
    return false;
}

// Mirror of func_8005D67C phrased with <= (retail `cntlzw`/`srwi.` shape).
bool cf::CActParamAnimGame::func_8005D6C0() {
    CActParamAnimGameViewBC14* self = reinterpret_cast<CActParamAnimGameViewBC14*>(this);
    if (self->f4F8 <= self->f508) {
        func_8004BC94(this);
        return true;
    }
    return false;
}

bool func_8004ECF4__13CActParamAnimFv(void* self);
bool func_8005D70C__Q22cf17CActParamAnimGame(void* self) {
    if (*(unsigned int*)((unsigned char*)self + 0x4ec) & 2) {
        return false;
    }
    return func_8004ECF4__13CActParamAnimFv(self);
}

// Clears the +0x4EC bit-12 (0x80000) flag and notifies the linked region.
// Volatile re-read reproduces the retail's second lwz before the mask.
bool cf::CActParamAnimGame::func_8005D728() {
    CActParamAnimGameViewBC14* self = reinterpret_cast<CActParamAnimGameViewBC14*>(this);
    if ((self->flags4EC & 0x80000) != 0) {
        self->flags4EC = *(volatile u32*)&self->flags4EC & ~0x80000;
        func_8004BC94(this);
        return true;
    }
    return false;
}

void cf::CActParamAnimGame::func_8005D76C() {}

// Sets the +0x530 0x40 flag and notifies the linked region (func_8004BC94,
// forwarded the passed type) when the actor is actionable and the current
// +0x52C state matches the requested one. r4 = type forwarded to
// func_8004BC94, r5 = requested state (0..3).
bool cf::CActParamAnimGame::func_8005D84C(u32 type, u32 state) {
    if (field_530 & 0x1C0) return false;
    if (func_80051B38(this) != 0 || func_80051AD0(this) != 0) {
        if (state == 0) {
            if (field_52C == 1 && (field_530 & 0x200) == 0) {
                field_530 |= 0x40;
                ((void (*)(void*, u32))func_8004BC94)(this, type);
                return true;
            }
        } else if (state == 1) {
            if (field_52C == 2) {
                field_530 |= 0x40;
                ((void (*)(void*, u32))func_8004BC94)(this, type);
                return true;
            }
        } else if (state == 2) {
            if (field_52C == 4) {
                field_530 |= 0x40;
                ((void (*)(void*, u32))func_8004BC94)(this, type);
                return true;
            }
        } else if (state == 3) {
            if (field_52C == 3) {
                field_530 |= 0x40;
                ((void (*)(void*, u32))func_8004BC94)(this, type);
                return true;
            }
        }
    }
    return false;
}

void cf::CActParamAnimGame::func_8005D99C() {}

void cf::CActParamAnimGame::func_8005DA44() {}

bool func_8005DAE4__Q22cf17CActParamAnimGame(void* self) {
    if (*(unsigned int*)((unsigned char*)self + 0x4ec) & 0x00040000) {
        func_8004BC94(self);
        return true;
    }
    return false;
}

void cf::CActParamAnimGame::func_8005DB1C() {}

float cf::CActorParam::CActorParam_UnkVirtualFunc38() { return *(float*)((u8*)this + 0x17F4); }

// Returns true (and notifies the linked region) when the +0x4F4 link is
// active; otherwise falls back to the +0x4E8 link when +0x4F4 is null.
// func_8004BC94 takes (self, type) in retail; the type slot (r4) is a
// leftover register at these call sites. Declaring it as a (live) second
// parameter keeps r4 busy as a call-arg register so the CSE'd link pointer
// lands in r5 like retail (__wpadInfoCallback precedent).
bool cf::CActParamAnimGame::func_8005DC30(u32 type) {
    CActParamAnimGameLink* link = field_4F4;
    if (link != 0 && link->active != 0) {
        ((void (*)(void*, u32))func_8004BC94)(this, type);
        return true;
    }
    if (link == 0) {
        link = field_4E8;
        if (link != 0 && link->active != 0) {
            ((void (*)(void*, u32))func_8004BC94)(this, type);
            return true;
        }
    }
    return false;
}

// cf::CActParamAnimGame::func_8005DCA0 - trivial leaf, returns false
// Retail symbol lacks Fv suffix; preserves exact mangling for FULL_MATCH
bool func_8005DCA0__Q22cf17CActParamAnimGame() { return false; }

// Rotation-matrix facing update (state 2 only): builds a 4x3 matrix from the
// yaw (sin/cos of 40.743663f * field_444), rotates its first row by the yaw
// basis, re-probes the ground along the new direction, then sets the +0x4EC
// 0x40000 flag and clears the +0x0C 0x80 flag.
void cf::CActParamAnimGame::func_8005DCA8() {
    if (field_374 != 2) {
        func_8004FFBC();
        return;
    }
    f32 m[3][4];
    m[0][0] = lbl_eu_80666040;
    m[0][1] = lbl_eu_80666040;
    m[0][2] = lbl_eu_8066611C;
    f32 sinv = nw4r::math::SinFIdx(lbl_eu_806660CC * field_444);
    f32 cosv = nw4r::math::CosFIdx(lbl_eu_806660CC * field_444);
    m[0][3] = cosv;
    m[1][0] = lbl_eu_80666040;
    m[1][1] = sinv;
    m[1][2] = lbl_eu_80666040;
    m[1][3] = lbl_eu_80666044;
    m[2][0] = lbl_eu_80666040;
    m[2][1] = -sinv;
    m[2][2] = lbl_eu_80666040;
    m[2][3] = cosv;
    ml::CVec3 v;
    v.x = m[0][2] * sinv + m[0][0] * cosv + m[0][1] * lbl_eu_80666040;
    v.y = m[0][0] * lbl_eu_80666040 + m[0][1] * lbl_eu_80666044 + m[0][2] * lbl_eu_80666040;
    v.z = m[0][2] * cosv + m[0][0] * (-sinv) + m[0][1] * lbl_eu_80666040;
    func_8004B7C0(this, &v);
    func_8004CEF8(this, 0);
    ml::CVec3 w;
    w.x = lbl_eu_80666104 * nw4r::math::SinFIdx(lbl_eu_806660CC * field_444);
    w.y = lbl_eu_80666040;
    w.z = lbl_eu_80666104 * nw4r::math::CosFIdx(lbl_eu_806660CC * field_444);
    ml::CVec3 d(field_3A8.x - w.x, field_3A8.y - w.y, field_3A8.z - w.z);
    func_8004B354(this, &d);
    field_4EC |= 0x40000;
    field_0C &= ~0x80;
}

void func_8005DE68(){}

void func_8005E28C(){}

// Movement-grounding probe used by the "waiting" anim states (1/3/4): lifts
// the position by lbl_806660BC, casts a probe along the facing dir (scaled/
// offset), and when a ground hit is found within lbl_80666050 of the current
// height, snaps the actor to it and records the ground point.
bool func_8005E60C(cf::CActParamAnimGame* self) {
    if (self->field_52C == 1 || (u32)(self->field_52C - 3) <= 1) {
        ml::CVec3 vec(self->field_3A8.x, self->field_3A8.y, self->field_3A8.z);
        if (((BE398Fn)func_804BE398)(&vec, self->field_4A8, 0, 0, lbl_eu_806660BC, lbl_eu_80666040) == 0) {
            vec.y += lbl_eu_806660BC;
            f32 sinv = nw4r::math::SinFIdx(lbl_eu_806660CC * self->field_444);
            ml::CVec3 dir;
            dir.x = sinv;
            f32 cosv = nw4r::math::CosFIdx(lbl_eu_806660CC * self->field_444);
            dir.y = lbl_eu_80666040;
            dir.z = cosv;
            dir.x = dir.x * lbl_eu_80666044 + vec.x;
            dir.y = dir.y * lbl_eu_80666044 + vec.y;
            dir.z = dir.z * lbl_eu_80666044 + vec.z;
            if (func_804BE348(&vec, &dir, self->field_4A8, 0, 0) == 0) {
                if (((BE398Fn)func_804BE398)(&dir, self->field_4A8, 0, 0, lbl_eu_80666124, lbl_eu_8066AF20) != 0) {
                    ml::CVec3 g;
                    func_804BE4B4(&g, 0);
                    if (g.y - self->field_3A8.y <= lbl_eu_80666050) {
                        ml::CVec3 t(self->field_3A8.x, self->field_3A8.y, self->field_3A8.z);
                        t.y = g.y - lbl_eu_80666050;
                        func_8004B354(self, &t);
                        self->field_514 = g.x;
                        self->field_518 = g.y;
                        self->field_51C = g.z;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// Ground-probe variant for states 2-4: offsets the position along the facing
// direction, probes down, and when a ground hit is found, records the ground
// point, re-probes, and snaps the actor to it.
bool func_8005E7C4(cf::CActParamAnimGame* self) {
    if ((u32)(self->field_52C - 2) > 2) return false;
    ml::CVec3 vec(self->field_3A8.x, self->field_3A8.y, self->field_3A8.z);
    f32 sinv = nw4r::math::SinFIdx(lbl_eu_806660CC * self->field_444);
    vec.x = vec.x + lbl_eu_806660D4 * (-sinv);
    f32 cosv = nw4r::math::CosFIdx(lbl_eu_806660CC * self->field_444);
    vec.z = vec.z + lbl_eu_806660D4 * (-cosv);
    vec.y = vec.y + lbl_eu_80666044;
    if (((BE398Fn)func_804BE398)(&vec, self->field_4A8, 0, 0, lbl_eu_80666048, lbl_eu_80666040) == 0) return false;
    ml::CVec3 g;
    func_804BE4B4(&g, 0);
    if (!(self->field_3A8.y - g.y <= lbl_eu_80666080)) return false;
    self->field_514 = self->field_3A8.x - nw4r::math::SinFIdx(lbl_eu_806660CC * self->field_444);
    self->field_51C = self->field_3A8.z - nw4r::math::CosFIdx(lbl_eu_806660CC * self->field_444);
    self->field_518 = g.y;
    ml::CVec3 t(self->field_514, self->field_518, self->field_51C);
    t.y = lbl_eu_80666044 + self->field_3A8.y;
    if (((BE398Fn)func_804BE398)(&t, self->field_4A8, 0, 0, lbl_eu_806660D8, lbl_eu_80666040) != 0) {
        func_804BE4B4(&g, 0);
        self->field_518 = g.y;
    }
    ml::CVec3 t2(self->field_3A8.x, self->field_3A8.y, self->field_3A8.z);
    t2.y = self->field_518;
    func_8004B354(self, &t2);
    return true;
}

void func_8005E990(){}

// Obstacle/facing update: adjusts the move vector against the linked
// region's probe results and refines it via the two scan loops, then runs
// the ground-follow / fallback logic. Returns a bool-ish status (r31).
int cf::CActParamAnimGame::func_8005EEB4(ml::CVec3* pos, ml::CVec3* move) {
    ml::CVec3 moveV(*move);
    ml::CVec3 v134(0.0f, 0.0f, 0.0f);
    int r31v = 0;

    if (field_4E8 != 0) {
        if (field_4EC & 0x8) {
            if (move->x != 0.0f || move->y != 0.0f) {
                ml::CVec3 v128(*pos);
                ml::CVec3 v11c(*move);
                ml::CVec3 v110(0.0f, 0.0f, 0.0f);
                v128.y = field_510;
                v11c.y = 0.0f;
                u16 saved = field_4E8->f6BE;
                field_4E8->f6BE = 0x1f4;
                func_804B1164(&field_4E8->f60C, &v128, &v11c, &v110);
                field_4E8->f6BE = saved;
                if (v110.x != 0.0f || v110.z != 0.0f) {
                    v128.x += v110.x;
                    v128.z += v110.z;
                    v128.x -= pos->x;
                    v128.z -= pos->z;
                    f32 len2 = v128.x * v128.x + v128.z * v128.z;
                    if (len2 != 0.0f) {
                        f32 len2b = move->x * move->x + move->z * move->z;
                        if (len2b != 0.0f) {
                            if (!(len2 >= 0.0f)) {
                                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                            }
                            f32 len = len2 <= 0.0f ? 0.0f : len2 * FrSqrt__Q24nw4r4mathFf(len2);
                            f32 inv = 1.0f / len;
                            f32 nx = v128.x * inv;
                            f32 nz = v128.z * inv;
                            if (!(len2b >= 0.0f)) {
                                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                            }
                            f32 lenb = len2b <= 0.0f ? 0.0f : len2b * FrSqrt__Q24nw4r4mathFf(len2b);
                            f32 invb = 1.0f / lenb;
                            f32 dot = nz * (move->z * invb) + nx * (move->x * invb);
                            if (dot > 0.0f) {
                                move->x = move->x * dot;
                                move->z = move->z * dot;
                            } else {
                                move->x = 0.0f;
                                move->z = 0.0f;
                            }
                        }
                    }
                }
            }
        }
        if (func_804B1164(&field_4E8->f60C, &v134, pos, move)) {
            if (v134.y > lbl_eu_8066606C) {
                field_47C = pos->y + v134.y;
                r31v = 1;
            }
        } else {
            if (v134.y > lbl_eu_8066606C) {
                if (field_3C0.y < 0.0f && field_3C0.y <= lbl_eu_80666128) {
                    func_8004B840(this, field_3C0.y * lbl_eu_8066612C);
                }
            }
        }
        if (v134.x != 0.0f || v134.y != 0.0f || v134.z != 0.0f) {
            field_4EC |= 0x20000000;
        }
        moveV += v134;
    }

    if (field_4EC & 0x28) {
        ml::CVec3 v38 = moveV * lbl_eu_80666098;
        ml::CVec3 v8c = v38;
        ml::CVec3 v2c = *pos + v8c;
        ml::CVec3 v98 = v2c;
        ml::CVec3 v80(0.0f, lbl_eu_806660BC, 0.0f);
        ml::CVec3 v20 = v98 + v80;
        ml::CVec3 va4 = v20;
        if (((BE398Fn)func_804BE398)(&va4, field_4A8 & ~0x2000, 0, 1, lbl_eu_80666130, lbl_eu_8066AF20)) {
            if (field_4EC & 0x8) {
                f32 f29v = lbl_eu_80666134;
                int r27v = -1;
                f32 f31v = 0.0f;
                int r26v;
                for (r26v = 0; r26v < func_804BE4AC(); r26v++) {
                    if (((CActParamAnimGameBeObj*)func_804BE520(r26v))->y >= f31v) {
                        if (f29v < ((CActParamAnimGameBeObj*)func_804BE50C(r26v))->y) {
                            f29v = ((CActParamAnimGameBeObj*)func_804BE50C(r26v))->y;
                            r27v = r26v;
                        }
                    }
                }
                if (r27v >= 0) {
                    if (func_804BE5A4(0x40000, r27v)) {
                        f32 f30v = f29v - lbl_eu_80666138;
                        int r26v2;
                        for (r26v2 = 0; r26v2 < func_804BE4AC(); r26v2++) {
                            if (r27v != r26v2) {
                                f32 g = ((CActParamAnimGameBeObj*)func_804BE50C(r26v2))->y;
                                if (f29v > g && f30v < g) {
                                    f30v = ((CActParamAnimGameBeObj*)func_804BE50C(r26v2))->y;
                                }
                            }
                        }
                        if (f29v - f30v > field_508) {
                            moveV = ml::CVec3(0.0f, 0.0f, 0.0f);
                        }
                    }
                }
            } else {
                if (field_4EC & 0x20) {
                    if (v134.x != 0.0f || v134.z != 0.0f) {
                        moveV = zero__Q22ml5CVec3;
                    }
                }
            }
        }
    }

    if (field_4EC & 0x8) {
        ml::CVec3 v104(*pos);
        ml::CVec3 vF8(moveV.x, 0.0f, moveV.z);
        v104.y = field_510;
        ((BD94CFn)func_804BD94C)(&v104, &vF8, field_4A8, 0, (field_0C & 0x10000) != 0, 1,
                                 field_494, field_490, lbl_eu_80666048, lbl_eu_8066AF20, field_484);
        field_510 = v104.y;
        v104.x -= pos->x;
        v104.z -= pos->z;
        f32 len2 = v104.x * v104.x + v104.z * v104.z;
        if (len2 != 0.0f) {
            f32 len2b = moveV.x * moveV.x + moveV.z * moveV.z;
            if (len2b != 0.0f) {
                if (!(len2 >= 0.0f)) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 len = len2 <= 0.0f ? 0.0f : len2 * FrSqrt__Q24nw4r4mathFf(len2);
                f32 inv = 1.0f / len;
                f32 nx = v104.x * inv;
                f32 nz = v104.z * inv;
                if (!(len2b >= 0.0f)) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 lenb = len2b <= 0.0f ? 0.0f : len2b * FrSqrt__Q24nw4r4mathFf(len2b);
                f32 invb = 1.0f / lenb;
                f32 dot = nz * (moveV.z * invb) + nx * (moveV.x * invb);
                if (dot > 0.0f) {
                    moveV.x = moveV.x * dot;
                    moveV.z = moveV.z * dot;
                } else {
                    moveV.x = 0.0f;
                    moveV.z = 0.0f;
                }
            }
        } else {
            moveV.x = 0.0f;
            moveV.z = 0.0f;
        }
    }

    if (func_800822F4__Q22cf13CfGameManagerFv() == 0x31) {
        if (field_4E8 != 0 && (field_4E8->f64 & 4)) {
            if (PSVECMag((const Vec*)&moveV) > lbl_eu_80666104) {
                f32 len2 = moveV.x * moveV.x + moveV.y * moveV.y + moveV.z * moveV.z;
                if (len2 == 0.0f) {
                    moveV = zero__Q22ml5CVec3;
                } else {
                    PSVECNormalize((const Vec*)&moveV, (Vec*)&moveV);
                }
                moveV *= lbl_eu_80666104;
            }
        }
    }

    if (field_4EC & 0x1000) {
        field_488 = pos->y;
        r31v = 1;
        field_4EC &= ~0x1000;
        ml::CVec3 zz(0.0f, 0.0f, 0.0f);
        func_8004B7C0(this, &zz);
        goto l60418;
    }

    if (field_270 & 0x20) {
        f32 f31v = pos->y + moveV.y;
        f32 f29v;
        if (field_3D8.y <= lbl_eu_806660C4) {
            f29v = lbl_eu_8066613C;
        } else if (field_3D8.y <= lbl_eu_8066605C) {
            f29v = lbl_eu_80666140;
        } else {
            f29v = lbl_eu_80666144;
        }
        if (v134.y > lbl_eu_8066606C) f29v = 0.0f;
        if (field_4EC & 0x20000000) {
            moveV.x = 0.0f;
            moveV.z = 0.0f;
        }
        if (field_530 & 0x400) {
            func_804BE628(1);
            r31v = (r31v | ((BD94CFn)func_804BD94C)(pos, &moveV, field_4A8, 0,
                        (field_0C & 0x10000) != 0, 1,
                        field_494, field_490, f29v, lbl_eu_8066AF20, field_484)) != 0;
            func_804BE628(0);
        } else {
            r31v = (r31v | ((BD94CFn)func_804BD94C)(pos, &moveV, field_4A8, 0,
                        (field_0C & 0x10000) != 0, 1,
                        field_494, field_490, f29v, lbl_eu_8066AF20, field_484)) != 0;
        }
        if (func_804BE5B8()) {
            field_4EC |= 0x2000000;
            ml::CVec3 v68 = field_3C0 * lbl_eu_8066609C;
            func_8004B7C0(this, &v68);
            if (lbl_eu_80666090 + field_464 < pos->y && lbl_eu_80666148 + f31v < pos->y) {
                if (move->x != 0.0f || move->z != 0.0f) {
                    ml::CVec3 vEC(move->x, 0.0f, move->z);
                    f32 clen2 = vEC.x * vEC.x + vEC.y * vEC.y + vEC.z * vEC.z;
                    if (clen2 == 0.0f) {
                        vEC = zero__Q22ml5CVec3;
                    } else {
                        PSVECNormalize((const Vec*)&vEC, (Vec*)&vEC);
                    }
                    field_0C |= 0x1000;
                    *(u32*)&field_3FC = *(u32*)&vEC.x;
                    *(u32*)&field_400 = *(u32*)&vEC.y;
                    *(u32*)&field_404 = *(u32*)&vEC.z;
                }
            }
        }
        if (moveV.y < 0.0f && pos->y - f31v <= lbl_eu_806660D4 && field_3C0.y <= lbl_eu_80666128) {
            func_8004B840(this, field_3C0.y * lbl_eu_8066612C);
        }
        if (pos->y - field_464 <= lbl_eu_80666098 && field_3C0.y > 0.0f && !(field_4EC & 0x1000000)) {
            r31v = 0;
        }
        field_534 = 0;
        if (field_4EC & 2) r31v = 1;
        goto l60418;
    }

    {
        f32 f29v = pos->y + moveV.y;
        f32 f30v;
        if (field_4EC & 2) {
            f30v = 0.0f;
            r31v = 1;
        } else if (field_0C & 0x10000) {
            f30v = 0.0f;
        } else {
            f32 len2 = move->x * move->x + move->z * move->z;
            if (len2 < lbl_eu_8066614C) {
                f30v = lbl_eu_80666150;
            } else {
                if (!(len2 >= 0.0f)) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 len = len2 <= 0.0f ? 0.0f : len2 * FrSqrt__Q24nw4r4mathFf(len2);
                f30v = -len;
                if (field_0C & 0x200000) f30v *= lbl_eu_80666050;
            }
        }
        if (v134.y > lbl_eu_8066606C) f30v = 0.0f;
        ml::CVec3 vE0 = *pos;
        if (field_530 & 0x400) {
            func_804BE628(1);
            r31v = (r31v | ((BD94CFn)func_804BD94C)(pos, &moveV, field_4A8, 0,
                        (field_0C & 0x10000) != 0, 1,
                        field_494, field_490, f30v, lbl_eu_8066AF20, field_484)) != 0;
            func_804BE628(0);
        } else {
            r31v = (r31v | ((BD94CFn)func_804BD94C)(pos, &moveV, field_4A8, 0,
                        (field_0C & 0x10000) != 0, 1,
                        field_494, field_490, f30v, lbl_eu_8066AF20, field_484)) != 0;
        }
        if (r31v != 0) {
            field_4EC |= 0x8000;
        } else {
            field_4EC &= ~0x8000;
        }
        if (func_804BE5B8()) {
            field_4EC |= 0x200000;
        } else {
            field_4EC &= ~0x200000;
        }
        if (moveV.y < 0.0f && pos->y - f29v <= lbl_eu_806660D4 && field_3C0.y <= lbl_eu_80666128) {
            func_8004B840(this, field_3C0.y * lbl_eu_8066612C);
        }
        if (r31v == 0) {
            if (field_4EC & 0x40000) {
                if (field_4EC & 0x1000) {
                    if (field_4EC & 0x800) {
                        if (v134.x == 0.0f && v134.z == 0.0f) {
                            if (vE0.y + moveV.y < pos->y) {
                                field_534 = 0;
                                r31v = 1;
                            } else {
                                s16 a = (s16)field_534;
                                s16 b = (s16)field_536;
                                field_534 = (u16)(a + 1);
                                if (a > b) {
                                    field_534 = (u16)(b + 1);
                                } else {
                                    r31v = 1;
                                }
                            }
                        } else {
                            *pos = vE0;
                            field_534 = 0;
                            field_4EC |= 0x400;
                            r31v = 1;
                        }
                    } else {
                        s16 a = (s16)field_534;
                        s16 b = (s16)field_536;
                        field_534 = (u16)(a + 1);
                        if (a > b) {
                            field_534 = (u16)(b + 1);
                        } else {
                            r31v = 1;
                        }
                    }
                } else {
                    field_534 = 0;
                }
            } else {
                field_534 = 0;
            }
        } else {
            field_534 = 0;
        }
        if (field_3C0.y > 0.0f) r31v = 0;
    }

l60418:
    if (func_804BE5C0()) {
        field_4EC |= 0x80000000;
    } else {
        field_4EC &= ~0x80000000;
    }
    if (r31v == 0) {
        if (!(field_0C & 0x10000)) {
            if (field_4EC & 0x40000) {
                field_4EC = (field_4EC & 0xFFF5FFFF) | 0x40000;
            }
            if (field_488 < pos->y || field_3B4.y - field_3A8.y > lbl_eu_806660C8) {
                field_488 = pos->y;
            } else {
                f32 d = field_488 - pos->y;
                if (d > lbl_eu_80666154 || (field_0C & 0x80)) {
                } else {
                    field_4EC |= 0x100000;
                    if (field_488 - pos->y <= lbl_eu_80666154) {
                        int n = (int)(field_488 - pos->y);
                        int q = n / 10;
                        int res = q * 20;
                        if (res >= 100) res = 100;
                        field_532 = (u16)res;
                    }
                }
            }
        }
    } else {
        if (field_4EC & 0x40000) {
            if (!(field_0C & 0x80) && field_488 - pos->y <= lbl_eu_80666154) {
                if (field_4EC & 2) {
                    if ((field_530 & 0x400) && field_488 - pos->y <= lbl_eu_80666158) {
                        if (func_80082694__Q22cf13CfGameManagerFv(0x83) == 0) {
                            func_8008269C__Q22cf13CfGameManagerFv(0x83, 1);
                            func_800826F0__Q22cf13CfGameManagerFv(0x83);
                        }
                    }
                } else {
                    int n = (int)(field_488 - pos->y);
                    int q = n / 10;
                    int res = q * 20;
                    if (res >= 100) res = 100;
                    field_532 = (u16)res;
                    field_538 = 0;
                    field_4EC |= 0x90000;
                }
            }
            field_4EC &= ~0x40000;
        } else if (field_4EC & 0x80000) {
            if (((CActParamAnimGameVt10*)this)->v10() > 0.0f) {
                s16 c = (s16)field_538;
                field_538 = (u16)(c + 1);
                if (c > 6) field_4EC &= ~0x80000;
            }
        }
    }
    field_488 = pos->y;
    field_4EC |= 0x20000;
    if (field_47C <= pos->y) field_47C = pos->y;
    if (field_4EC & 2) {
        field_4EC &= 0xFFE2FFFF;
    }
    if (field_4EC & 0x4000) {
        if (field_3A0 != 0) {
            void* sub = ((cf::CActParamAnimGameObj3A0*)field_3A0)->sub7EC;
            if (sub != 0) {
                ml::CVec3 v14 = *pos + ml::CVec3(0.0f, 1.0f, 0.0f);
                ml::CVec3 vBC = v14;
                ml::CVec3 v8 = *pos + ml::CVec3(0.0f, lbl_eu_8066615C, 0.0f);
                ml::CVec3 vB0 = v8;
                ml::CVec3 vD4;
                ml::CVec3 vC8;
                if (func_804B5658((void*)lbl_eu_80665958, &vD4, &vC8, &vBC, &vB0)) {
                    if (vD4.y > field_3E4.y - lbl_eu_80666160) {
                        ml::CVec3 v44 = vD4;
                        ((cf::CActParamAnimGameObj7EC*)sub)->v14(&v44);
                        ((cf::CActParamAnimGameObj7EC*)sub)->v18(1);
                    }
                }
            }
        }
    }
    return r31v;
}


// Actor-facing position/heading update: reads the +0x4E8 link's current
// position (vtable+0x4C), diffs it against this object's position, and when
// the delta is non-trivial, orients the link toward the actor (vtable+0xC4)
// with the heading from Atan2FIdx. vtable+0xAC returns the position vector.
void cf::CActParamAnimGame::func_80060110() {
    if (field_4E8 == 0) return;
    if (((CActParamAnimGameVt4C*)field_4E8)->v4C() == 0) return;
    void* src = func_800B708C((int)((CActParamAnimGameVt4C*)field_4E8)->v4C());
    if (src == 0) return;
    ml::CVec3* p = (ml::CVec3*)((CActParamAnimGameVt4C*)src)->vAC();
    ml::CVec3 d;
    nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(&d),
                         reinterpret_cast<const nw4r::math::VEC3*>(p),
                         reinterpret_cast<const nw4r::math::VEC3*>(&field_3A8));
    f32 dx = d.x;
    f32 dy = d.y;
    f32 dz = d.z;
    bool b1 = nw4r::math::FAbs(dx) <= lbl_eu_8066A208;
    bool b2 = nw4r::math::FAbs(dy) <= lbl_eu_8066A208;
    bool nearXY = b1 && b2;
    bool b3 = nw4r::math::FAbs(dz) <= lbl_eu_8066A208;
    bool nearAll = nearXY && b3;
    if (nearAll) return;
    field_0C |= 0x1000000;
    ((CActParamAnimGameVt4C*)field_4E8)->vC4(lbl_eu_806660E0 * nw4r::math::Atan2FIdx(dx, dz));
}

// Forwards the +0x444 scalar to the +0xC4 virtual slot of the object at
// +0x4E8 (tail call; retail keeps no stack frame). Real virtual dispatch
// so MWCC emits the r12 vtable load (the manual cast would use r4).
void cf::CActParamAnimGame::func_80060268() {
    CActParamAnimGameViewBC14* self = reinterpret_cast<CActParamAnimGameViewBC14*>(this);
    CActParamAnimGameVt4C* region = (CActParamAnimGameVt4C*)self->region4E8;
    if (region == 0) return;
    f32 value = self->f444;
    region->vC4(value);
}

extern "C" bool func_80060290(void* r3) {
    int r0 = *(int*)((char*)r3 + 0x270);
    int r4 = 0;
    if ((r0 & 0x800000) != 0) {
        r4 = 1;
    } else {
        r0 = *(int*)((char*)r3 + 0x2f0);
        if ((r0 & 0x800000) != 0) {
            r4 = 1;
        }
    }
    return r4 != 0;
}
