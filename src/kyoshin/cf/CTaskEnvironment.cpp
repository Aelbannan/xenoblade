// Auto-scaffolded catalog TU for kyoshin/cf/CTaskEnvironment
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "functions.hpp"

extern "C" void nopFunc();  // retail no-op helper defined below
extern "C" float scaleByGlobal(float val);
extern "C" u32 checkBitFlag(u8* self);
extern "C" void func_80059920(void* dst, const void* src);
extern "C" void func_80059944(void* self, const void* src);
extern "C" void func_80059974(ml::CVec4* out, const ml::CVec4* a,
                               const ml::CVec4* b);
extern "C" void copyWord4Offset(u32* dst, const u32* src);

// Blend endpoint aliases; expanded by the preprocessor so MWCC reloads the
// sdata2 constants at each use like retail (no cross-call register lifetime).
#define FAC lbl_eu_80665FAC
#define FC0 lbl_eu_80665FC0
#include "kyoshin/cf/CTaskEnvironment.hpp"
#include "monolib/math/CVec4.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "nw4r/math/math_triangular.h"
#include <new>

// Retail-symbol import: sdata2 s32->f32 conversion magic
// (0x4330000080000000); MWCC's inline int->float cast pulls its pool double
// from here (CREvtModelMap.cpp convention).
extern double lbl_eu_80665FB8;
// Retail-symbol import: sdata2 u32->f32 conversion magic double used by
// func_800599E0's seconds cast.
extern double lbl_eu_80665FF0;

// Retail constructs CTaskEnvironment through a C-linkage out-of-line ctor
// entry point taking (self, arg) and returning self (CSysWinSave /
// CREvtModelMap precedent). noinline keeps the create-site call a real bl.
__declspec(noinline) CTaskEnvironment* __ct__CTaskEnvironment(
    CTaskEnvironment* self, CProcess* pArg) {
    // The scene process argument doubles as the CTaskEnvScene provider; its
    // +0x78 world object supplies the clock fields copied below. Declaring it
    // first makes MWCC bind it to r31 like retail.
    CTaskEnvScene* scene = reinterpret_cast<CTaskEnvScene*>(pArg);
    __ct__8CProcessFv(self);
    u32* p = reinterpret_cast<u32*>(self);

    // Interim CTTask<CTaskEnvironment> vtable (overwritten later).
    p[4] = reinterpret_cast<u32>(lbl_eu_805263D8);

    // NULL PTMF -> mMoveFunc (0x3C) / mDrawFunc (0x48), matching the retail
    // load/store order.
    p[0x10] = __ptmf_null[1];        // 0x40 mMoveFunc[1]
    p[0xF] = __ptmf_null[0];         // 0x3C mMoveFunc[0]
    p[0x11] = __ptmf_null[2];        // 0x44 mMoveFunc[2]
    p[0x12] = __ptmf_null[0];        // 0x48 mDrawFunc[0]
    p[0x13] = __ptmf_null[1];        // 0x4C mDrawFunc[1]
    p[0x14] = __ptmf_null[2];        // 0x50 mDrawFunc[2]

    // Final CTaskEnvironment vtable, then scalar member init.
    p[4] = reinterpret_cast<u32>(lbl_eu_80526390);
    self->mScene = scene;
    self->field_0x58 = 0;
    self->field_0x5C = 2000;
    self->field_0x60 = 0;
    self->field_0x64 = 0;
    self->field_0x68 = 0x10;
    self->field_0x6C = 0;
    self->field_0x70 = lbl_eu_80665FA0;
    self->field_0x74 = lbl_eu_80665FA4;
    self->field_0x78 = lbl_eu_80665FA8;
    self->field_0x7C = lbl_eu_80665FA0;
    self->field_0x80 = lbl_eu_80665FA0;
    self->field_0x84 = lbl_eu_80665FAC;
    self->field_0x88 = lbl_eu_80665FA0;
    self->field_0x8C = lbl_eu_80665FA0;
    self->field_0xA8 = 0;
    p[0x2B] = 0;   // 0xAC
    p[0x2C] = 0;   // 0xB0
    p[0x2D] = 0;   // 0xB4
    p[0x2E] = 0;   // 0xB8
    p[0x2F] = 0;   // 0xBC
    p[0x30] = 0;   // 0xC0
    p[0x31] = 0;   // 0xC4
    p[0x32] = 0;   // 0xC8
    p[0x33] = 0;   // 0xCC
    p[0x34] = 0;   // 0xD0
    p[0x35] = 0;   // 0xD4
    p[0x36] = 0;   // 0xD8
    p[0x37] = 0;   // 0xDC
    p[0x38] = 0;   // 0xE0
    p[0x39] = 0;   // 0xE4
    self->field_0xE8 = 0;
    self->field_0xE9 = 0;
    self->field_0xEA = 0;

    // Register as the global environment manager and snapshot the world clock.
    lbl_eu_80663D58 = reinterpret_cast<CTaskEnvGlobal*>(self);
    CTaskEnvWorld* world = scene->field_0x78;
    self->field_0x94 = world->field_0x68;
    self->field_0x90 = world->field_0x64;
    self->field_0x9C = world->field_0x70;
    self->field_0x98 = world->field_0x6C;
    self->field_0xA0 = world->field_0x54;
    self->field_0xA4 = world->field_0x58;
    return self;
}

extern "C" void copyWord4(u32* dst, const u32* src) {
    const u32* s = src;
    u32* d = dst;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

extern "C" void* func_8005870C(void* self) { return (char*)self + 0x64; }
extern "C" u32 getSubField78(void* self) { return *(u32*)((u8*)self + 0x78); }

extern "C" float func_8005871C(void* self) { return *(float*)((char*)self + 0x54); }

extern "C" float func_80058724(void* self) { return *(float*)((char*)self + 0x58); }

// CTaskEnvironment::~CTaskEnvironment() - extern "C" free-function form
// (CCol6CheckBat/CCol6Invite precedent): outer null-check covers the delete;
// clears the global manager pointer; single inner null-guard (retail has one
// beq, unlike the double-nest in the CCol6 family) around the CProcess base
// dtor; default -O4,p (retail keeps separate stw r31/stw r30 saves).
extern "C" void __dt__8CProcessFv(void* self, int flags);
extern "C" void* __dt__16CTaskEnvironmentFv(CTaskEnvironment* self, int flags) {
    if (self != 0) {
        lbl_eu_80663D58 = 0;
        if (self != 0) {
            __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

extern "C" void* getGlobalSda() { return lbl_eu_80663D58; }

extern "C" u32 getField68(u8* self) { return *(u32*)((u8*)self + 0x68); }

extern "C" u32 getSubField7C(u8* self) { return *(u32*)((u8*)self + 0x7C); }

// Clock helper: hours = minute/c + second/c/c (c = sdata2 rate constant,
// 60.0f; the int->float conversion uses the retail double trick).
// Clock helper: hours = second/c/c + minute/c (c = sdata2 rate constant,
// 60.0f). The int->float conversion builds the 0x43300000 pattern manually
// so the magic-double reloc matches retail.
extern "C" float func_800587F8(CTaskEnvTime* time) {
    float c = lbl_eu_80665FB0;
    float s = time->second / c;
    float m = (float)time->minute;
    float r = s / c;
    return m / c + r;
}

extern "C" u32 getField5C(u8* self) { return *(u32*)((u8*)self + 0x5C); }

// Build the environment lights: two CTimeLightGrp groups from the work heap,
// then colour/ambient/directional lights registered into scene-manager slots,
// and finally copy the world clock into +0x90..+0xA4. (Retail keeps this as an
// unmangled C entry point taking the task.)
extern "C" __declspec(noinline) void func_80058844(CTaskEnvironment* self) {
    func_804923F8(self->mScene->field_0x5C, 0);
    u32 heap = (u32)func_80496004(self->mScene);

    self->mUnkE0 = mtl::MemManager::allocate(0x3C, heap);
    if (self->mUnkE0 != NULL) {
        __ct__CTimeLightGrp((CTimeLightGrp*)self->mUnkE0, (void*)heap);
    }
    self->mUnkE4 = mtl::MemManager::allocate(0x3C, heap);
    if (self->mUnkE4 != NULL) {
        __ct__CTimeLightGrp((CTimeLightGrp*)self->mUnkE4, (void*)heap);
    }

    ml::CVec4 colBlack(lbl_eu_80665FC0, lbl_eu_80665FC0, lbl_eu_80665FC0,
                       lbl_eu_80665FAC);
    self->mLgtDC =
        func_804930BC(self->mScene->field_0x5C, self->mScene->field_0x5C->field_0x6C,
                      &colBlack);
    self->mScene->field_0x5C->field_0x19 = 1;
    ml::CVec4 colWhite(lbl_eu_80665FAC, lbl_eu_80665FAC, lbl_eu_80665FAC,
                       lbl_eu_80665FAC);
    self->mLgtAC =
        func_804930BC(self->mScene->field_0x5C, self->mScene->field_0x5C->field_0x0C,
                      &colWhite);
    ml::CVec3 dirDown(lbl_eu_80665FC4, lbl_eu_80665FC8, lbl_eu_80665FC4);
    ml::CVec4 colAmbient(lbl_eu_80665FC0, lbl_eu_80665FC0, lbl_eu_80665FC0,
                         lbl_eu_80665FAC);
    self->mLgtB0 =
        func_804933AC(self->mScene->field_0x5C, self->mScene->field_0x5C->field_0x0C,
                      &colAmbient, &dirDown);
    self->mLgtB8 =
        func_804930BC(self->mScene->field_0x5C, self->mScene->field_0x5C->field_0x4C,
                      lbl_eu_805708C0);
    ml::CVec4 colWhite2(lbl_eu_80665FAC, lbl_eu_80665FAC, lbl_eu_80665FAC,
                        lbl_eu_80665FAC);
    self->mLgtBC =
        func_804930BC(self->mScene->field_0x5C, self->mScene->field_0x5C->field_0x2C,
                      &colWhite2);

    // Sun colour/direction pair built from the sdata table scaled by FC0.
    const float* sunTable = lbl_eu_80570860;
    float sunScale = lbl_eu_80665FC0;
    ml::CVec3 dirSun(lbl_eu_80665FC4, lbl_eu_80665FC8, lbl_eu_80665FC4);
    ml::CVec4 colSun(sunTable[0] * sunScale, sunTable[1] * sunScale,
                     sunTable[2] * sunScale, sunTable[3]);
    self->mLgtC0 =
        func_804933AC(self->mScene->field_0x5C, self->mScene->field_0x5C->field_0x2C,
                      &colSun, &dirSun);
    ml::CVec4 colWhite3(lbl_eu_80665FAC, lbl_eu_80665FAC, lbl_eu_80665FAC,
                        lbl_eu_80665FAC);
    self->mLgtC4 =
        func_804930BC(self->mScene->field_0x5C, self->mScene->field_0x5C->field_0x0C,
                      &colWhite3);
    ml::CVec3 dirDown2(lbl_eu_80665FC4, lbl_eu_80665FC8, lbl_eu_80665FC4);
    ml::CVec4 colAmbient2(lbl_eu_80665FC0, lbl_eu_80665FC0, lbl_eu_80665FC0,
                          lbl_eu_80665FAC);
    self->mLgtC8 =
        func_804933AC(self->mScene->field_0x5C, self->mScene->field_0x5C->field_0x0C,
                      &colAmbient2, &dirDown2);
    ml::CVec4 colWhite4(lbl_eu_80665FAC, lbl_eu_80665FAC, lbl_eu_80665FAC,
                        lbl_eu_80665FAC);
    self->mLgtD4 =
        func_804930BC(self->mScene->field_0x5C, self->mScene->field_0x5C->field_0x2C,
                      &colWhite4);
    ml::CVec3 dirSun2(lbl_eu_80665FC4, lbl_eu_80665FC8, lbl_eu_80665FC4);
    ml::CVec4 colSun2(sunTable[0] * sunScale, sunTable[1] * sunScale,
                      sunTable[2] * sunScale, sunTable[3]);
    self->mLgtD8 =
        func_804933AC(self->mScene->field_0x5C, self->mScene->field_0x5C->field_0x2C,
                      &colSun2, &dirSun2);

    func_8005A2F0((CTimeLightGrp*)self->mUnkE0, self->mLgtAC);
    func_8005A2F0((CTimeLightGrp*)self->mUnkE0, self->mLgtB0);
    func_8005A2F0((CTimeLightGrp*)self->mUnkE0, self->mLgtB8);
    func_8005A2F0((CTimeLightGrp*)self->mUnkE0, self->mLgtBC);
    func_8005A2F0((CTimeLightGrp*)self->mUnkE0, self->mLgtC0);
    func_8005A2F0((CTimeLightGrp*)self->mUnkE4, self->mLgtC4);
    func_8005A2F0((CTimeLightGrp*)self->mUnkE4, self->mLgtC8);
    func_8005A2F0((CTimeLightGrp*)self->mUnkE4, self->mLgtD4);
    func_8005A2F0((CTimeLightGrp*)self->mUnkE4, self->mLgtD8);

    CTaskEnvWorld* world = self->mScene->field_0x78;
    self->field_0x94 = world->field_0x68;
    self->field_0x90 = world->field_0x64;
    self->field_0x9C = world->field_0x70;
    self->field_0x98 = world->field_0x6C;
}

extern "C" void func_80058BD8(ml::CVec4* out, const ml::CVec4* in, float s) {
    float z = in->z * s;
    float y = in->y * s;
    float x = in->x * s;
    float w = in->w;
    out->z = z;
    out->x = x;
    out->y = y;
    out->w = w;
}

void CTaskEnvironment::Init() {
    func_80058844(this);
    func_804C1094(mScene->mEnvLgtCtrl, 1);
    mScene->mEnvLgtCtrl->field_0 |= 0x80;
}

void CTaskEnvironment::Term() {
    func_804C1094(mScene->mEnvLgtCtrl, 0);
    if (mUnkE4 != 0) {
        if (mUnkE4 != 0) {
            static_cast<CTaskEnvUnk*>(mUnkE4)->vfUnk1(1);
        }
        mUnkE4 = 0;
    }
    if (mUnkE0 != 0) {
        if (mUnkE0 != 0) {
            static_cast<CTaskEnvUnk*>(mUnkE0)->vfUnk1(1);
        }
        mUnkE0 = 0;
    }
}

// Day-cycle light update. Reads the world clock (or CfGameManager time when
// available), advances the sun-angle/colour factors and blends the per-hour
// light presets from the sdata table into the two CTimeLightGrp groups.
void CTaskEnvironment::Move() {
    CTaskEnvTime* time = reinterpret_cast<CTaskEnvTime*>(this);
    u8* tbl = reinterpret_cast<u8*>(lbl_eu_80570830);
    u16 hourArg, minuteArg;
    if (func_8007E1B4__Q22cf13CfGameManagerFv() != 0) {
        // Live clock from the game manager: (hour, minute) out-params plus a
        // truncated seconds word.
        func_80086D98__Q22cf13CfGameManagerFv(&hourArg, &minuteArg);
        u32 secArg = func_80086DA8__Q22cf13CfGameManagerFv();
        func_800599E0(time, hourArg, minuteArg, secArg & 0xFFFF);
    } else if (field_0x58 == 0) {
        float delta = lbl_eu_80665FCC * func_80496288(mScene);
        func_80059A48(time, delta);
    }

    func_804C123C(reinterpret_cast<CTaskEnvLgtCtrlAccess*>(
                      getSubField7C(reinterpret_cast<u8*>(mScene))),
                  1);
    func_804C1270(reinterpret_cast<CTaskEnvLgtCtrlAccess*>(
                      getSubField7C(reinterpret_cast<u8*>(mScene))),
                  1);

    // Sun elevation factor: month/day/hour progress through the year scaled
    // by the .data rate constant. Integer fields are converted to float via
    // the 0x43300000 bit-pattern trick so the pool reloc names the retail
    // sdata2 magic lbl_eu_80665FB8.
    float c1 = lbl_eu_80665FB4;
    float c2 = lbl_eu_80665FA8;
    float c3 = lbl_eu_80665FD0;
    F64Conv conv;
    conv.w[0] = 0x43300000;
    conv.w[1] = (u32)(int)this->field_0x68 ^ 0x80000000;
    float fh = (float)(conv.d - lbl_eu_80665FB8);
    conv.w[1] = (u32)(int)this->field_0x64 ^ 0x80000000;
    float fd = (float)(conv.d - lbl_eu_80665FB8);
    conv.w[1] = (u32)(int)this->field_0x60 ^ 0x80000000;
    float fm = (float)(conv.d - lbl_eu_80665FB8);
    float t1 = fm / c3 + fd / c1 / c2 / c3 + fh / c1 / c2 / c3;
    f32 sunAngle = lbl_eu_8066A1FC * t1;
    nopFunc();

    // Time-of-day factor: second/minute/hour progress through the day.
    conv.w[1] = (u32)(int)this->field_0x6C ^ 0x80000000;
    float fmin = (float)(conv.d - lbl_eu_80665FB8);
    conv.w[1] = (u32)(int)this->field_0x68 ^ 0x80000000;
    float fhour = (float)(conv.d - lbl_eu_80665FB8);
    float dayFactor =
        lbl_eu_80665FD4 *
        (fhour / lbl_eu_80665FB4 + fmin / lbl_eu_80665FB0 / lbl_eu_80665FB4 +
         (field_0x70 / lbl_eu_80665FB0) / lbl_eu_80665FB0 /
             lbl_eu_80665FB4);
    field_0x88 = dayFactor;
    field_0x8C =
        lbl_eu_80665FD8 + field_0x78 + field_0x74 * func_8004CC74(sunAngle);

    CTaskEnvRotMtx mtx;
    func_80059610(&mtx, scaleByGlobal(field_0x88));
    func_800596B8(&mtx, scaleByGlobal(field_0x8C));

    // Sun direction vector: unit X/Z basis rotated by the matrix above.
    float dirSrc[3];   // sp+0x230 in retail
    float dirOut[3];   // sp+0x23C
    ml::CVec3* v = reinterpret_cast<ml::CVec3*>(
        func_8004B60C((ml::CVec3*)dirSrc, lbl_eu_80665FA0, lbl_eu_80665FA0,
                      lbl_eu_80665FDC));
    func_800598A8(reinterpret_cast<ml::CVec3*>(dirOut), &mtx, v);
    func_8004B3F0(reinterpret_cast<void*>(&field_0x7C),
                  reinterpret_cast<void*>(dirOut));

    // Push the total seconds-of-day into the env-light controller.
    int secsOfDay =
        ((int)this->field_0x6C + (int)this->field_0x68 * 60) * 60;
    conv.w[1] = (u32)secsOfDay ^ 0x80000000;
    float fsod = (float)(conv.d - lbl_eu_80665FB8);
    func_804C1600(
        reinterpret_cast<CTaskEnvLgtCtrlAccess*>(
            getSubField7C(reinterpret_cast<u8*>(mScene))),
        (void*)(int)(fsod + field_0x70));

    if (field_0xE8 != 0 ||
        checkBitFlag(
            reinterpret_cast<u8*>(getSubField7C(reinterpret_cast<u8*>(mScene)))) !=
        0) {
        goto postUpdate;
    }

    {
        u32 hr = getField68(reinterpret_cast<u8*>(this));
        if (hr >= 6 && getField68(reinterpret_cast<u8*>(this)) <= 0x10) {
            // Full daytime: copy preset rows directly.
            func_80059920(mUnkE0, tbl + 0x20);
            func_80059920(mUnkE4, tbl + 0x50);
            copyWord4(&this->field_0x90,
                      reinterpret_cast<const u32*>(func_8005870C(
                          (void*)getSubField78(mScene))));
            field_0xA0 = func_8005871C((void*)getSubField78(mScene));
            field_0xA4 = func_80058724((void*)getSubField78(mScene));
            func_8049347C(
                static_cast<CVirtualLightObj*>(
                    (void*)getField5C(reinterpret_cast<u8*>(mScene))),
                reinterpret_cast<const ml::CVec4*>(tbl + 0x90), FAC);
            func_80059944(mLgtB8, tbl + 0x20);
            goto postUpdate;
        }
        if (getField68(reinterpret_cast<u8*>(this)) < 0x11 ||
            getField68(reinterpret_cast<u8*>(this)) > 0x12) {
            goto nightBlock;
        }
        if (getField68(reinterpret_cast<u8*>(this)) == 0x11 &&
            !(func_800587F8(reinterpret_cast<CTaskEnvTime*>(this)) >= FC0)) {
            // Hour 17 before the threshold: dusk blend begins.
            // Reuse the sun-angle slot so MWCC pins k to f31 like retail.
            float& k = sunAngle;
            k = lbl_eu_80665FCC * func_800587F8(reinterpret_cast<CTaskEnvTime*>(this));
            float v200[4], v210[4], v220[4];
            func_80058BD8((ml::CVec4*)v200,
                          reinterpret_cast<const ml::CVec4*>(tbl + 0x00), k);
            func_80058BD8((ml::CVec4*)v210,
                          reinterpret_cast<const ml::CVec4*>(tbl + 0x20),
                          FAC - k);
            func_80059974((ml::CVec4*)v220, (const ml::CVec4*)v210,
                          (const ml::CVec4*)v200);
            func_80059920(mUnkE0, v220);
            func_80059920(mUnkE4, tbl + 0x50);
            float v1d0[4], v1e0[4], v1f0[4];
            func_80058BD8((ml::CVec4*)v1d0,
                          reinterpret_cast<const ml::CVec4*>(tbl + 0x60), k);
            func_80058BD8((ml::CVec4*)v1e0,
                          reinterpret_cast<const ml::CVec4*>(func_8005870C(
                              (void*)getSubField78(mScene))),
                          FAC - k);
            func_80059974((ml::CVec4*)v1f0, (const ml::CVec4*)v1e0,
                          (const ml::CVec4*)v1d0);
            copyWord4(&this->field_0x90, reinterpret_cast<const u32*>(v1f0));
            field_0xA0 = lbl_eu_80665FE0 * k +
                         (FAC - k) * func_8005871C((void*)getSubField78(mScene));
            field_0xA4 = lbl_eu_80665FE4 * k +
                         (FAC - k) * func_80058724((void*)getSubField78(mScene));
            float va[4], vb[4], vc[4];
            func_80058BD8((ml::CVec4*)va, reinterpret_cast<const ml::CVec4*>(tbl + 0x00),
                          k);
            func_80058BD8((ml::CVec4*)vb, reinterpret_cast<const ml::CVec4*>(tbl + 0x90),
                          FAC - k);
            func_80059974((ml::CVec4*)vc, (const ml::CVec4*)vb,
                          (const ml::CVec4*)va);
            copyWord4Offset(reinterpret_cast<u32*>(mLgtB8),
                            reinterpret_cast<const u32*>(vc));
            func_80059944(mLgtB8, tbl + 0x20);
            goto postUpdate;
        }
        // L_597D0: noon hold / evening transition selection for hours 17-18.
        if (getField68(reinterpret_cast<u8*>(this)) == 0x11 &&
            func_800587F8(reinterpret_cast<CTaskEnvTime*>(this)) >= FC0) {
            goto noonBlock;
        }
        if (getField68(reinterpret_cast<u8*>(this)) == 0x12 &&
            !(func_800587F8(reinterpret_cast<CTaskEnvTime*>(this)) >= FC0)) {
        noonBlock:
            func_80059920(mUnkE0, tbl + 0x00);
            func_80059920(mUnkE4, tbl + 0x50);
            copyWord4(&this->field_0x90,
                      reinterpret_cast<const u32*>(tbl + 0x60));
            field_0xA0 = lbl_eu_80665FE0;
            field_0xA4 = lbl_eu_80665FE4;
            copyWord4Offset(reinterpret_cast<u32*>(mLgtB8),
                            reinterpret_cast<const u32*>(tbl + 0x00));
            func_80059944(mLgtB8, tbl + 0x20);
            goto postUpdate;
        }
        if (getField68(reinterpret_cast<u8*>(this)) != 0x12 ||
            !(func_800587F8(reinterpret_cast<CTaskEnvTime*>(this)) >= FC0)) {
            goto postUpdate;
        }
        // Evening: fade the day presets out toward the night rows.
        {
        // Reuse the sun-angle slot so MWCC pins k to f31 like retail.
        float& k = sunAngle;
        k = lbl_eu_80665FCC * (func_800587F8(reinterpret_cast<CTaskEnvTime*>(this)) - FC0);
        float v170[4], v180[4], v190[4];
        func_80058BD8((ml::CVec4*)v170, reinterpret_cast<const ml::CVec4*>(tbl + 0x50), k);
        func_80058BD8((ml::CVec4*)v180, reinterpret_cast<const ml::CVec4*>(tbl + 0x00),
                      FAC - k);
        func_80059974((ml::CVec4*)v190, (const ml::CVec4*)v180,
                      (const ml::CVec4*)v170);
        func_80059920(mUnkE0, v190);
        float v140[4], v150[4], v160[4];
        func_80058BD8((ml::CVec4*)v140, reinterpret_cast<const ml::CVec4*>(tbl + 0x40), k);
        func_80058BD8((ml::CVec4*)v150, reinterpret_cast<const ml::CVec4*>(tbl + 0x50),
                      FAC - k);
        func_80059974((ml::CVec4*)v160, (const ml::CVec4*)v150,
                      (const ml::CVec4*)v140);
        func_80059920(mUnkE4, v160);
        float v110[4], v120[4], v130[4];
        func_80058BD8((ml::CVec4*)v110, reinterpret_cast<const ml::CVec4*>(tbl + 0x80), k);
        func_80058BD8((ml::CVec4*)v120, reinterpret_cast<const ml::CVec4*>(tbl + 0x60),
                      FAC - k);
        func_80059974((ml::CVec4*)v130, (const ml::CVec4*)v120,
                      (const ml::CVec4*)v110);
        copyWord4(&this->field_0x90, reinterpret_cast<const u32*>(v130));
        field_0xA0 = lbl_eu_80665FE0 * k + lbl_eu_80665FE0 * (FAC - k);
        field_0xA4 = lbl_eu_80665FE8 * k + lbl_eu_80665FE4 * (FAC - k);
        float ve0[4], vf0[4], v100[4];
        func_80058BD8((ml::CVec4*)ve0, reinterpret_cast<const ml::CVec4*>(tbl + 0x40), k);
        func_80058BD8((ml::CVec4*)vf0, reinterpret_cast<const ml::CVec4*>(tbl + 0x00),
                      FAC - k);
        func_80059974((ml::CVec4*)v100, (const ml::CVec4*)vf0,
                      (const ml::CVec4*)ve0);
        copyWord4Offset(reinterpret_cast<u32*>(mLgtB8),
                        reinterpret_cast<const u32*>(v100));
        func_80059944(mLgtB8, tbl + 0x20);
        }
        goto postUpdate;

    nightBlock:
        if (getField68(reinterpret_cast<u8*>(this)) >= 0x13 ||
            getField68(reinterpret_cast<u8*>(this)) <= 4) {
            // Night: direct night rows.
            func_80059920(mUnkE0, tbl + 0x50);
            func_80059920(mUnkE4, tbl + 0x40);
            copyWord4(&this->field_0x90,
                      reinterpret_cast<const u32*>(tbl + 0x80));
            field_0xA0 = lbl_eu_80665FE0;
            field_0xA4 = lbl_eu_80665FE8;
            copyWord4Offset(reinterpret_cast<u32*>(mLgtB8),
                            reinterpret_cast<const u32*>(tbl + 0x40));
            func_80059944(mLgtB8, tbl + 0x20);
        } else if (getField68(reinterpret_cast<u8*>(this)) == 5) {
            // Dawn blend at hour 5 (no FCC scaling on the factor).
            float& k = sunAngle;
            k = func_800587F8(reinterpret_cast<CTaskEnvTime*>(this));
            float vb0[4], vc0[4], vd0[4];
            func_80058BD8((ml::CVec4*)vb0,
                          reinterpret_cast<const ml::CVec4*>(tbl + 0x20), k);
            func_80058BD8((ml::CVec4*)vc0,
                          reinterpret_cast<const ml::CVec4*>(tbl + 0x50),
                          FAC - k);
            func_80059974((ml::CVec4*)vd0, (const ml::CVec4*)vc0,
                          (const ml::CVec4*)vb0);
            func_80059920(mUnkE0, vd0);
            float v80[4], v90[4], va0[4];
            func_80058BD8((ml::CVec4*)v80,
                          reinterpret_cast<const ml::CVec4*>(tbl + 0x50), k);
            func_80058BD8((ml::CVec4*)v90,
                          reinterpret_cast<const ml::CVec4*>(tbl + 0x40),
                          FAC - k);
            func_80059974((ml::CVec4*)va0, (const ml::CVec4*)v90,
                          (const ml::CVec4*)v80);
            func_80059920(mUnkE4, va0);
            float v50[4], v60[4], v70[4];
            func_80058BD8((ml::CVec4*)v50,
                          reinterpret_cast<const ml::CVec4*>(func_8005870C(
                              (void*)getSubField78(mScene))),
                          k);
            func_80058BD8((ml::CVec4*)v60, reinterpret_cast<const ml::CVec4*>(tbl + 0x80),
                          FAC - k);
            func_80059974((ml::CVec4*)v70, (const ml::CVec4*)v60,
                          (const ml::CVec4*)v50);
            copyWord4(&this->field_0x90, reinterpret_cast<const u32*>(v70));
            field_0xA0 = lbl_eu_80665FE0 * (FAC - k) +
                         k * func_8005871C((void*)getSubField78(mScene));
            field_0xA4 = lbl_eu_80665FE8 * (FAC - k) +
                         k * func_80058724((void*)getSubField78(mScene));
            float v20b[4], v30b[4], v40b[4];
            func_80058BD8((ml::CVec4*)v20b,
                          reinterpret_cast<const ml::CVec4*>(tbl + 0x90), k);
            func_80058BD8((ml::CVec4*)v30b,
                          reinterpret_cast<const ml::CVec4*>(tbl + 0x40),
                          FAC - k);
            func_80059974((ml::CVec4*)v40b, (const ml::CVec4*)v30b,
                          (const ml::CVec4*)v20b);
            copyWord4Offset(reinterpret_cast<u32*>(mLgtB8),
                            reinterpret_cast<const u32*>(v40b));
            func_80059944(mLgtB8, tbl + 0x20);
        }
    }

postUpdate:
    func_8005A374(static_cast<CTimeLightGrp*>(mUnkE0));
    func_8005A374(static_cast<CTimeLightGrp*>(mUnkE4));
    func_8049E374(reinterpret_cast<u8*>((void*)getSubField78(mScene)),
                  field_0xA0, field_0xA4);

    if (checkBitFlag(reinterpret_cast<u8*>(
            getSubField7C(reinterpret_cast<u8*>(mScene)))) == 0) {
        if (field_0xA8 != 0) {
            func_800407C8_tmp tmp;
            void* fogColour = func_800407C8(
                &tmp, field_0x90, field_0x90, field_0x90, field_0x9C);
            func_8049E350(reinterpret_cast<u8*>((void*)getSubField78(mScene)),
                          fogColour);
        } else {
            func_8049E350(reinterpret_cast<u8*>((void*)getSubField78(mScene)),
                          &this->field_0x90);
        }
    }
}

#undef FAC
#undef FC0

extern "C" void nopFunc() {}

// Y-axis rotation matrix: [cos,0,sin; 0,FAC,0; -sin,0,cos]. Angles are
// scaled into fidx units by FEC before SinFIdx/CosFIdx.
extern "C" CTaskEnvRotMtx* func_80059610(CTaskEnvRotMtx* mtx, float angle) {
    float s = nw4r::math::SinFIdx(lbl_eu_80665FEC * angle);
    float c = nw4r::math::CosFIdx(lbl_eu_80665FEC * angle);
    mtx->m00 = c;
    mtx->m01 = lbl_eu_80665FA0;
    mtx->m02 = s;
    mtx->m10 = lbl_eu_80665FA0;
    mtx->m11 = lbl_eu_80665FAC;
    mtx->m12 = lbl_eu_80665FA0;
    mtx->m20 = -s;
    mtx->m21 = lbl_eu_80665FA0;
    mtx->m22 = c;
    return mtx;
}

extern const float lbl_eu_8066A210;
extern "C" float scaleByGlobal(float val) { return val * lbl_eu_8066A210; }

// Rotate the row vectors of 'mtx' about the X axis: the first row is scaled
// by the identity constants, the second/third mix cos/-sin and sin/cos of the
// fidx-scaled angle. The product lands in a local and is written back.
extern "C" void func_800596B8(CTaskEnvRotMtx* mtx, float angle) {
    float s = nw4r::math::SinFIdx(lbl_eu_80665FEC * angle);
    float c = nw4r::math::CosFIdx(lbl_eu_80665FEC * angle);
    float ns = -s;
    CTaskEnvRotMtx out;
    out.m00 = lbl_eu_80665FAC * mtx->m00 + lbl_eu_80665FA0 * mtx->m10 +
              lbl_eu_80665FA0 * mtx->m20;
    out.m01 = lbl_eu_80665FAC * mtx->m01 + lbl_eu_80665FA0 * mtx->m11 +
              lbl_eu_80665FA0 * mtx->m21;
    out.m02 = lbl_eu_80665FAC * mtx->m02 + lbl_eu_80665FA0 * mtx->m12 +
              lbl_eu_80665FA0 * mtx->m22;
    out.m10 = lbl_eu_80665FA0 * mtx->m00 + c * mtx->m10 + ns * mtx->m20;
    out.m11 = lbl_eu_80665FA0 * mtx->m01 + c * mtx->m11 + ns * mtx->m21;
    out.m12 = lbl_eu_80665FA0 * mtx->m02 + c * mtx->m12 + ns * mtx->m22;
    out.m20 = lbl_eu_80665FA0 * mtx->m00 + s * mtx->m10 + c * mtx->m20;
    out.m21 = lbl_eu_80665FA0 * mtx->m01 + s * mtx->m11 + c * mtx->m21;
    out.m22 = lbl_eu_80665FA0 * mtx->m02 + s * mtx->m12 + c * mtx->m22;
    *mtx = out;
}

// Row-major 3x3 matrix * vector, sequenced for the retail codegen order.
extern "C" void func_800598A8(ml::CVec3* out, const CTaskEnvRotMtx* m,
                              const ml::CVec3* v) {
    out->z = v->y * m->m21 + v->x * m->m20 + v->z * m->m22;
    out->y = v->y * m->m11 + v->x * m->m10 + v->z * m->m12;
    out->x = v->y * m->m01 + v->x * m->m00 + v->z * m->m02;
}

extern "C" u32 checkBitFlag(u8* self) { return (*(u32*)((u8*)self + 0) >> 1) & 0x1u; }

// Converted to inline member function in header

struct _S16 { u32 a, b, c, d; };

extern "C" void func_80059920(void* dst, const void* src) {
    *(_S16*)((u8*)dst + 0x28) = *(const _S16*)((const u8*)src);
}

struct _S16b { u32 a, b, c, d; };

extern "C" void func_80059944(void* self, const void* src) {
    if (((u8*)self)[0x19] != 0) return;
    *(_S16b*)((u8*)self + 0x1C) = *(const _S16b*)src;
}

// Vector add: out = a + b (component-wise). The four sums are computed in
// w,z,y,x order but stored z,y,x,w; temps are declared z,w,y,x so MWCC
// allocates z to f4 and w to f5 like retail.
void func_80059974(ml::CVec4* out, const ml::CVec4* a, const ml::CVec4* b) {
    float z, w, y, x;
    w = a->w + b->w;
    z = a->z + b->z;
    y = a->y + b->y;
    x = a->x + b->x;
    out->z = z;
    out->y = y;
    out->x = x;
    out->w = w;
}

extern "C" void copyWord4Offset(u32* dst, const u32* src) {
    dst[1] = src[0];
    dst[2] = src[1];
    dst[3] = src[2];
    dst[4] = src[3];
}


// Set clock time. Hour >= 24 and minute >= 60 reset to 0; seconds arrive as
// an int, get converted to float via the double trick, then clamp to FA0
// when they reach the FB0 limit.
extern "C" void func_800599E0(CTaskEnvTime* time, int hour, int minute, int second) {
    time->hour = hour;
    if (hour >= 0x18) {
        time->hour = 0;
    }
    time->minute = minute;
    if (minute >= 0x3C) {
        time->minute = 0;
    }
    time->second = (float)(u32)second;
    if (time->second >= lbl_eu_80665FB0) {
        time->second = lbl_eu_80665FA0;
    }
}

// Advance the clock by 'delta' seconds. Every field wraps with upward modulo
// (negative deltas borrow): seconds wrap at 60 into minutes, minutes into
// hours (24), hours into days (30), days into months (12); month carries
// accumulate in 'total'. The truncated integer part is rebuilt as a double
// through the 0x43300000 bit-pattern trick so the subtraction reloc names the
// retail sdata2 magic lbl_eu_80665FB8 (CfTFile.cpp convention).
extern "C" void func_80059A48(CTaskEnvTime* time, float delta) {
    F64Conv conv;
    conv.w[0] = 0x43300000;
    float s = time->second + delta;
    time->second = s;
    int secs = (int)s;
    int addMin = secs / 60;
    int rem = secs % 60;
    if (rem < 0) {
        rem += 60;
    }
    conv.w[1] = (u32)(int)s ^ 0x80000000;
    float whole = s - (conv.d - lbl_eu_80665FB8);

    int minute = time->minute + addMin;
    int addHour = minute / 60;
    time->minute = minute % 60;
    if (time->minute < 0) {
        time->minute += 60;
    }
    int hour = time->hour + addHour;
    int addDay = hour / 24;
    time->hour = hour % 24;
    if (time->hour < 0) {
        time->hour += 24;
    }
    int day = time->day + addDay;
    int addMonth = day / 30;
    time->day = day % 30;
    if ((s32)time->day < 0) {
        time->day += 30;
    }
    int month = time->month + addMonth;
    int addTotal = month / 12;
    time->month = month % 12;
    if ((s32)time->month < 0) {
        time->month += 12;
    }
    time->total += addTotal;

    conv.w[1] = (u32)rem ^ 0x80000000;
    time->second = whole + (float)(conv.d - lbl_eu_80665FB8);
}

// Returns a 0/1/2 region code for the record at (global->field_6C +
// global->field_68 * 60): 0 for records inside [0x10E, 0x41A), otherwise
// 1 (within 60 of the 0x41A boundary) or 2.
u32 func_80059C14() {
    u32 x = (u32)(lbl_eu_80663D58->field_0x6C + lbl_eu_80663D58->field_0x68 * 60);
    if ((u32)(x - 270) <= 779) {
        return 0;
    }
    return ((x - 1050) <= 59) ? 1 : 2;
}

// Allocate from the work-thread heap, construct (skipped when allocation
// fails) and register under 'parent'; the object is returned even when null.
CTaskEnvironment* func_80059C58(CProcess* parent, CProcess* arg) {
    CTaskEnvironment* obj =
        (CTaskEnvironment*)mtl::MemManager::allocate(0xEC, CWorkThreadSystem::getWorkMem());
    if (obj != NULL) {
        obj = __ct__CTaskEnvironment(obj, arg);
    }
    obj->Regist(parent, false);
    return obj;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<T> is declared in kyoshin/CTaskGameEff.hpp (pulled in via
// harness_catalog.hpp); the out-of-line specializations below emit the retail
// Move/Draw/dtor symbols. Do not define a local CTTask here - it would clash
// with the template from CTaskGameEff.hpp.
template<> CTTask<CTaskEnvironment>::~CTTask() {}
template<> void CTTask<CTaskEnvironment>::Move() {
    if (mMoveFunc) {
        (static_cast<CTaskEnvironment*>(this)->*mMoveFunc)();
    }
}

template<> void CTTask<CTaskEnvironment>::Draw() {
    if (mDrawFunc) {
        (static_cast<CTaskEnvironment*>(this)->*mDrawFunc)();
    }
}
// Static initializer: fills the 10 four-float rows of the sdata table at
// lbl_eu_80570830 with sdata2 constants (colour/light preset values). MWCC
// hoists each distinct constant into a live register across the whole body.
void sinit_80059D60() {
    float* t = lbl_eu_80570830;
    t[0x00] = lbl_eu_80665FF8;
    t[0x01] = lbl_eu_80665FFC;
    t[0x02] = lbl_eu_80666000;
    t[0x03] = lbl_eu_80665FAC;
    t[0x04] = lbl_eu_80665FF8;
    t[0x05] = lbl_eu_80665FFC;
    t[0x06] = lbl_eu_80666000;
    t[0x07] = lbl_eu_80665FAC;
    t[0x08] = lbl_eu_80665FAC;
    t[0x09] = lbl_eu_80665FAC;
    t[0x0A] = lbl_eu_80665FAC;
    t[0x0B] = lbl_eu_80665FAC;
    t[0x0C] = lbl_eu_80666004;
    t[0x0D] = lbl_eu_80666008;
    t[0x0E] = lbl_eu_8066600C;
    t[0x0F] = lbl_eu_80665FAC;
    t[0x10] = lbl_eu_80666010;
    t[0x11] = lbl_eu_80666014;
    t[0x12] = lbl_eu_80666018;
    t[0x13] = lbl_eu_80665FAC;
    t[0x14] = lbl_eu_80665FA0;
    t[0x15] = lbl_eu_80665FA0;
    t[0x16] = lbl_eu_80665FA0;
    t[0x17] = lbl_eu_80665FAC;
    t[0x18] = lbl_eu_8066601C;
    t[0x19] = lbl_eu_80666020;
    t[0x1A] = lbl_eu_80666020;
    t[0x1B] = lbl_eu_80665FAC;
    t[0x1C] = lbl_eu_8066601C;
    t[0x1D] = lbl_eu_80666020;
    t[0x1E] = lbl_eu_80666020;
    t[0x1F] = lbl_eu_80665FAC;
    t[0x20] = lbl_eu_80666024;
    t[0x21] = lbl_eu_80666028;
    t[0x22] = lbl_eu_8066602C;
    t[0x23] = lbl_eu_80665FAC;
    t[0x24] = lbl_eu_80666030;
    t[0x25] = lbl_eu_80666030;
    t[0x26] = lbl_eu_80666030;
    t[0x27] = lbl_eu_80665FAC;
}

extern "C" void func_800586E8() {}
