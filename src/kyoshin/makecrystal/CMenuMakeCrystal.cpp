// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMenuMakeCrystal
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMenuMakeCrystal.hpp"
#include "kyoshin/CBgTex.hpp"
#include "revolution/gx/GXPixel.h"
#include <nw4r/lyt/lyt_drawInfo.h>

// CTaskGame::func_800426F0 (declared here to avoid pulling CScn headers).
extern "C" void CTaskGame_getInstance();
extern "C" int func_800426F0__9CTaskGameFv();
extern "C" int func_8013BE50();
extern "C" void func_80137250__FPQ34nw4r3lyt8DrawInfo(nw4r::lyt::DrawInfo* di);
extern u32 lbl_eu_80663E28;

extern "C" void cbRenderBefore__16CMenuMakeCrystalFv(void*);
extern "C" void* __dt__15CMakeCrystalWinFv(void*, int);
extern "C" void __dt__6CBgTexFv(void*, int);
extern "C" void __dt__800FED0C(void*, int);


void __ct__CMenuMakeCrystal(){}

// extern "C" free-function form (CCol6CheckBat precedent): retail calls the
// sub-object dtors at +0x80 (CMakeCrystalWin), +0x60 (CBgTex), then the
// +0x00 embedded dtor (flags 0), then the flags-based delete; stmw/lmw frame.
#pragma push
#pragma optimize_for_size on
extern "C" void* __dt__16CMenuMakeCrystalFv(void* self, int flags) {
    if (self != 0) {
        __dt__15CMakeCrystalWinFv((u8*)self + 0x80, -1);
        __dt__6CBgTexFv((u8*)self + 0x60, -1);
        __dt__800FED0C(self, 0);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}
#pragma pop

void CMenuMakeCrystal::Init() {}

void func_80210E9C(){}

void func_802116D4(){}

// Copy fields +0x4..+0x14 from src to dst (byte/word/byte copy).
void func_80211CB8(CMakeCrystalCopyBlock* dst, const CMakeCrystalCopyBlock* src) {
    dst->field_0x4 = src->field_0x4;
    dst->field_0x5 = src->field_0x5;
    dst->field_0x8 = src->field_0x8;
    dst->field_0xC = src->field_0xC;
    dst->field_0x10 = src->field_0x10;
    dst->field_0x14 = src->field_0x14;
}

void func_80211CEC(){}

void func_80212158(){}

// CScn view exposing the render-callback removal (retail mangled name).
class CScn {
public:
    void removeRenderCB(void* cb);
};

extern void* lbl_eu_806646C8;
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void func_801338C8();
extern "C" void func_804962A0(void*, int);
extern "C" void func_801C3D9C(void*);
extern "C" void func_8021299C(void*);
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(int);

void CMenuMakeCrystal::Term() {
    waitForDrawDone__9CDeviceVIFv();
    func_801338C8();
    func_804962A0(*(void**)((u8*)this + 0x5C), 1);
    void* render = this;
    if (this) render = (u8*)this + 0x58;
    ((CScn*)*(void**)((u8*)this + 0x5C))->removeRenderCB(render);
    func_801C3D9C((u8*)this + 0x60);
    func_8021299C((u8*)this + 0x80);
    lbl_eu_806646C8 = 0;
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

void CMenuMakeCrystal::Move() {}

void CMenuMakeCrystal::cbRenderBefore() {
    CTaskGame_getInstance();
    if (func_800426F0__9CTaskGameFv() == 0) {
        if ((lbl_eu_80663E28 & 0x200000) == 0) {
            if (func_8013BE50() == 0) {
                return;
            }
            GXSetZMode(GX_DISABLE, GX_NEVER, GX_DISABLE);
            nw4r::lyt::DrawInfo drawInfo;
            func_80137250__FPQ34nw4r3lyt8DrawInfo(&drawInfo);
            reinterpret_cast<CBgTex*>((u8*)this + 0x60)->func_801C3D7C(&drawInfo);
        }
    }
}

void func_8021240C(){}

void* lbl_eu_806646C8;

// (lbl_eu_806646C8 != 0)
extern "C" bool func_80212480() { return lbl_eu_806646C8 != 0; }
void* func_80212BE0(void*);

void* func_80212490() {
    unsigned char* p = static_cast<unsigned char*>(lbl_eu_806646C8);
    if (p != 0) {
        return func_80212BE0(p + 0x80);
    }
    return 0;
}

extern "C" int func_801C3E34(void*);
extern "C" void func_8021260C(void*);

// Init the +0x60 sub-object when it reports active, then set the +0x43E4 flag.
void func_802124AC(void* self) {
    if (func_801C3E34((u8*)self + 0x60) != 0) {
        func_8021260C((u8*)self + 0x80);
        *(u8*)((u8*)self + 0x43E4) = 1;
    }
}

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMakeCrystal.
extern "C" void func_802124F4(void* self) {
    ((void(*)(void*))cbRenderBefore__16CMenuMakeCrystalFv)((char*)self - 0x58);
}

// IScnRender vtable this-adjusting thunk for destructor.
extern "C" void func_802124FC(void* self) {
    ((void(*)(void*))__dt__16CMenuMakeCrystalFv)((char*)self - 0x58);
}
