// Auto-scaffolded catalog TU for kyoshin/cf/CTaskEnvironment
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CTaskEnvironment.hpp"
#include "monolib/math/CVec4.hpp"
void __ct__CTaskEnvironment(){}

void copyWord4(u32* dst, const u32* src) {
    const u32* s = src;
    u32* d = dst;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

extern "C" void* func_8005870C(void* self) { return (char*)self + 0x64; }

u32 getSubField78(void* self) { return *(u32*)((u8*)self + 0x78); }

extern "C" float func_8005871C(void* self) { return *(float*)((char*)self + 0x54); }

extern "C" float func_80058724(void* self) { return *(float*)((char*)self + 0x58); }

CTaskEnvironment::~CTaskEnvironment() {}

extern "C" void* getGlobalSda() { return lbl_eu_80663D58; }

extern "C" u32 getField68(u8* self) { return *(u32*)((u8*)self + 0x68); }

extern "C" u32 getSubField7C(u8* self) { return *(u32*)((u8*)self + 0x7C); }

void func_800587F8(){}

extern "C" u32 getField5C(u8* self) { return *(u32*)((u8*)self + 0x5C); }

// Retail func_80058844 is a 0x394-byte out-of-line helper; extern "C" +
// noinline keeps callers (Init) emitting a real unmangled `bl func_80058844`
// instead of inlining the empty stub body (reloc name must match retail).
extern "C" __declspec(noinline) void func_80058844() {}

void func_80058BD8(){}

void CTaskEnvironment::Init() {
    func_80058844();
    func_804C1094(mScene->mEnvLgtCtrl, 1);
    mScene->mEnvLgtCtrl->field_0 |= 0x80;
}

void CTaskEnvironment::Term() {}

void CTaskEnvironment::Move() {}

void nopFunc() {}

void func_80059610(){}

extern float lbl_eu_8066A210;
extern "C" float scaleByGlobal(float val) { return val * lbl_eu_8066A210; }

void func_800596B8(){}

void func_800598A8(){}

extern "C" u32 checkBitFlag(u8* self) { return (*(u32*)((u8*)self + 0) >> 1) & 0x1u; }

// Converted to inline member function in header

void func_80059920(){}

void func_80059944(){}

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

void copyWord4Offset(u32* dst, const u32* src) {
    dst[1] = src[0];
    dst[2] = src[1];
    dst[3] = src[2];
    dst[4] = src[3];
}


void func_800599E0(){}

void func_80059A48(){}

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

void func_80059C58(){}

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
void sinit_80059D60(){}

extern "C" void func_800586E8() {}
