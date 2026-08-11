// Auto-scaffolded catalog TU for kyoshin/cf/CActParamAnimGame
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CActParamAnimGame.hpp"
#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_triangular.h>
#include <revolution/mtx/vec.h>
#include "monolib/math/CVec3.hpp"

void __ct__8005A3FC(){}

cf::CActParamAnimGame::CActParamAnimGame() {}

cf::CActParamAnimGame::~CActParamAnimGame() {}

void cf::CActParamAnimGame::func_8005A524() {}

void func_8005A594(){}

void func_8005A5B0(){}

void func_8005B820(){}

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
void func_8004B9D4(void*, u32, u32, s32, u32);
u32 func_804BE348(void*, void*, u32, u32, u32);
u32 func_804BE398(void*, u32, u32, u32);
u32 func_804BE5A4(u32, u32);
void func_804BE4B4(void*, u32);
void func_804BE4E0(void*, u32);
void* func_804BE50C(u32);
u32 func_804BD94C(void*, void*, u32, u32, u32, u32);
void func_804B0B54(void*, const ml::CVec3*);
void func_804B1130(void*, void*, void*, void*, void*);
s8 lbl_eu_80663D64;
s8 lbl_eu_80663D6C;
s8 lbl_eu_80663D74;
f32 lbl_eu_80663D60;
f32 lbl_eu_80663D68;
f32 lbl_eu_80663D70;
}

struct CActParamAnimGameView {
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
    u8 _4F0[0x24];
    f32 f514;               // 0x514
    f32 f518;               // 0x518
    f32 f51C;               // 0x51C
    u8 _520[0x0C];
    u32 state52C;           // 0x52C
    u16 flags530;           // 0x530
};

static float callVfn(void* self, u32 off) {
    return ((float (*)(void*))(*(void**)((u8*)*(void**)self + off)))(self);
}
static int callVfnI(void* self, u32 off) {
    return ((int (*)(void*))(*(void**)((u8*)*(void**)self + off)))(self);
}

extern "C" void func_8005BC14(void* selfV) {
    CActParamAnimGameView* self = (CActParamAnimGameView*)selfV;
    s32 moved = 0;

    self->flags530 &= 0xFF3F;
    self->state52C = 0;
    self->flags4EC &= ~0x3000;
    self->f394 = 1.0f;
    self->f398 = 1.0f;
    *(ml::CVec3*)((u8*)self + 0x3F0) = ml::CVec3::zero;
    func_8004B7C0(self, &ml::CVec3::zero);
    func_80051C40(self);

    if ((self->flags0C & 0x100) != 0) goto exit;
    if (self->obj3A0 == 0) goto exit;

    moved = 1;
    ml::CVec3 pos;
    u32 flags = self->flags530 | 0x800;
    self->flags530 = (u16)flags;
    pos.x = self->posX3A8;
    pos.y = self->posY3AC;
    pos.z = self->posZ3B0;
    if (((flags >> 2) & 3) == 0) goto mode0;

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
        f32 f30v = callVfn(self, 0x14) - f29v;
        f32 f29b = callVfn(self, 0x0C);
        if (callVfn(self, 0x10) * f31v + f29b < f30v) self->flags530 |= 2;
    }
    moved = 0;
    pos.x += 0.1f * (self->f514 - pos.x);
    pos.z += 0.1f * (self->f51C - pos.z);
    {
        ml::CVec3 dir = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
        ml::CVec3 up = pos + ((self->flags530 & 4) ? ml::CVec3(0.0f, 1.0f, 0.0f) : ml::CVec3(0.0f, -1.5f, 0.0f));
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
    if (((flags >> 4) & 3) != 0) {
        ml::CVec3 up2 = pos + ml::CVec3(0.0f, 1.0f, 0.0f);
        ml::CVec3 dir2 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
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
        if (callVfnI(self->owner, 0x10) != 0 || func_80051BF4(self) != 0) {
            self->flags530 &= 0xFDFF;
        } else {
            if (lbl_eu_80663D64 == 0) { lbl_eu_80663D60 = 60.0f * 0.017453292f; lbl_eu_80663D64 = 1; }
            if (lbl_eu_80663D6C == 0) { lbl_eu_80663D68 = -60.0f * 0.017453292f; lbl_eu_80663D6C = 1; }
            f32 ang = *(f32*)((u8*)self->owner + 0xC);
            if (ang > lbl_eu_80663D60 || ang <= lbl_eu_80663D68) {
                self->flags530 &= 0xFDFF;
            }
        }
    }
    if (moved == 0) goto exit;

    ml::CVec3 accel = ml::CVec3(0.0f, 0.0f, 0.0f);
    if ((self->flags530 & 0x200) == 0) {
        f32 f28v = 0.0f;
        f32 f29v = *(f32*)((u8*)self->owner + 0x14);
        if (f29v * self->f430 >= func_80053958()) {
            f28v = *(f32*)((u8*)self->owner + 0x14) * self->f394;
        }
        f32 f31v = f28v * (1000.0f * func_8004B34C(self) / 3600.0f) * (self->f390 * getSecPerFrame__9CDeviceVIFv());
        if (self->flags530 & 0x400) {
            f32 a = *(f32*)((u8*)self->owner + 0xC);
            f32 f30v = fabsf(f31v * SinFIdx__Q24nw4r4mathFf(40.743663f * a));
            accel.y = f31v * CosFIdx__Q24nw4r4mathFf(40.743663f * a);
            ml::CVec3 d2 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
            ml::CVec3 d3 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f440), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f440));
            ml::CVec3 upv = ml::CVec3(0.0f, 1.0f, 0.0f);
            ml::CVec3 cross;
            PSVECCrossProduct((const Vec*)&d2, (const Vec*)&upv, (Vec*)&cross);
            f32 len2 = cross.x * cross.x + cross.y * cross.y + cross.z * cross.z;
            if (len2 == 0.0f) {
                cross = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)&cross, (Vec*)&cross);
            }
            f32 dot = cross.x * d3.x + cross.y * d3.y + cross.z * d3.z;
            if (dot <= 0.0f) f30v = -f30v;
            accel.x = cross.x * f30v;
            accel.z = cross.z * f30v;
        } else {
            accel.x = f31v * SinFIdx__Q24nw4r4mathFf(40.743663f * self->f440);
            accel.z = f31v * CosFIdx__Q24nw4r4mathFf(40.743663f * self->f440);
            ml::CVec3 d4 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
            ml::CVec3 d5 = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f440), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f440));
            f32 dot = d4.x * d5.x + d4.y * d5.y + d4.z * d5.z;
            accel.y = f31v * dot;
            f32 scale = 1.0f - fabsf(dot);
            accel.x *= scale;
            accel.z *= scale;
        }
    }
    {
        ml::CVec3 target;
        target.x = self->posX3A8;
        target.y = self->posY3AC;
        target.z = self->posZ3B0;
        ml::CVec3 dir = ml::CVec3(SinFIdx__Q24nw4r4mathFf(40.743663f * self->f444), 0.0f, CosFIdx__Q24nw4r4mathFf(40.743663f * self->f444));
        moved = 1;
        u32 hit;
        if (accel.x == 0.0f && accel.y == 0.0f && accel.z == 0.0f) {
            ml::CVec3 tpd = target + dir * 2.0f;
            hit = func_804BE348(&target, &tpd, 0, 8192, 0) != 0;
        } else {
            ml::CVec3 va = target + accel;
            ml::CVec3 vb = dir * 3.0f;
            ml::CVec3 vc = va - dir;
            ml::CVec3 vd = vc + vb;
            hit = func_804BE348(&vc, &vd, 0, 8192, 0) != 0;
            if (hit == 0) {
                ml::CVec3 t2 = target + dir * 2.0f;
                hit = func_804BE348(&target, &t2, 0, 8192, 0) != 0;
            }
        }
        if (hit == 0) {
            target += ml::CVec3(0.01f, 0.01f, 0.01f);
            ml::CVec3 tpd = target + dir * 2.0f;
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
        if (len2 < 0.0f) {
            Warning__Q24nw4r2dbFPCciPCce("arithmetic.h", 627, "FSqrt: Input is out of the domain.");
        }
        f32 len = 0.0f;
        if (len2 > 0.0f) len = len2 * FrSqrt__Q24nw4r4mathFf(len2);
        if (len > 0.001f) {
            nx = nx / len;
            nz = nz / len;
            nx = 0.3f * (nx - dir.x) + dir.x;
            nz = 0.3f * (nz - dir.z) + dir.z;
            len2 = nx * nx + nz * nz;
            if (len2 < 0.0f) {
                Warning__Q24nw4r2dbFPCciPCce("arithmetic.h", 627, "FSqrt: Input is out of the domain.");
            }
            f32 lenb = 0.0f;
            if (len2 > 0.0f) lenb = len2 * FrSqrt__Q24nw4r4mathFf(len2);
            if (lenb != 0.0f) {
                nx = nx / lenb;
                nz = nz / lenb;
            } else {
                nx = -v.x;
                nz = -v.z;
            }
        }
        f32 f28v = 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(nx, nz);
        func_8004B52C(self, f28v);
        dir.x = SinFIdx__Q24nw4r4mathFf(40.743663f * f28v);
        dir.z = CosFIdx__Q24nw4r4mathFf(40.743663f * f28v);
        if (accel.x == 0.0f && accel.y == 0.0f && accel.z == 0.0f) goto ground;
        {
            f32 f2v = 0.024543693f * Atan2FIdx__Q24nw4r4mathFff(accel.z * dir.x - accel.x * dir.z, accel.y);
            if (lbl_eu_80663D74 == 0) { lbl_eu_80663D70 = 3.1415927f * 0.25f; lbl_eu_80663D74 = 1; }
            f32 p4 = lbl_eu_80663D70;
            if (f2v <= p4 && f2v >= -p4) {
                self->state52C = 1;
            } else if (f2v >= 3.0f * p4 || f2v <= -3.0f * p4) {
                self->state52C = 2;
            } else if (f2v > p4 && f2v < 3.0f * p4) {
                self->state52C = 4;
            } else {
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
                    func_804BD94C(&target2, &accel, self->filter4A8, 0, (self->flags0C & 0x100) != 0, 0);
                }
                target2.y += 0.9f;
                if (func_804BE398(&target2, self->filter4A8, 0, 0) != 0) {
                    void* e = func_804BE50C(0);
                    target2.y -= 1.4f - (*(f32*)((u8*)e + 0x4) - target2.y);
                }
                {
                    ml::CVec3 d2 = dir * 2.0f;
                    ml::CVec3 tmd = target2 - dir;
                    ml::CVec3 tpd = target2 + d2;
                    u32 hit2 = func_804BE348(&tmd, &tpd, 0, 8192, 0) != 0;
                    if (hit2 == 0) {
                        target2 += ml::CVec3(0.01f, 0.01f, 0.01f);
                        ml::CVec3 tmd2 = target2 - dir;
                        ml::CVec3 tpd2 = target2 + d2;
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
    func_8004B9D4(self, 6, 0, -1, 0);
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

void cf::CActParamAnimGame::func_8005D2C4() {}

void cf::CActParamAnimGame::func_8005D608() {}

void cf::CActParamAnimGame::func_8005D67C() {}

void cf::CActParamAnimGame::func_8005D6C0() {}

bool func_8004ECF4__13CActParamAnimFv(void* self);
bool func_8005D70C__Q22cf17CActParamAnimGame(void* self) {
    if (*(unsigned int*)((unsigned char*)self + 0x4ec) & 2) {
        return false;
    }
    return func_8004ECF4__13CActParamAnimFv(self);
}

void cf::CActParamAnimGame::func_8005D728() {}

void cf::CActParamAnimGame::func_8005D76C() {}

void cf::CActParamAnimGame::func_8005D84C() {}

void cf::CActParamAnimGame::func_8005D99C() {}

void cf::CActParamAnimGame::func_8005DA44() {}

void func_8004BC94(void*);
bool func_8005DAE4__Q22cf17CActParamAnimGame(void* self) {
    if (*(unsigned int*)((unsigned char*)self + 0x4ec) & 0x00040000) {
        func_8004BC94(self);
        return true;
    }
    return false;
}

void cf::CActParamAnimGame::func_8005DB1C() {}

float cf::CActorParam::CActorParam_UnkVirtualFunc38() { return *(float*)((u8*)this + 0x17F4); }

void cf::CActParamAnimGame::func_8005DC30() {}

// cf::CActParamAnimGame::func_8005DCA0 - trivial leaf, returns false
// Retail symbol lacks Fv suffix; preserves exact mangling for FULL_MATCH
bool func_8005DCA0__Q22cf17CActParamAnimGame() { return false; }

void cf::CActParamAnimGame::func_8005DCA8() {}

void func_8005DE68(){}

void func_8005E28C(){}

void func_8005E60C(){}

void func_8005E7C4(){}

void func_8005E990(){}

void cf::CActParamAnimGame::func_8005EEB4() {}

void cf::CActParamAnimGame::func_80060110() {}

void cf::CActParamAnimGame::func_80060268() {}

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
