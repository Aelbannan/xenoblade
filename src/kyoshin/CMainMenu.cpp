// Auto-scaffolded catalog TU for kyoshin/CMainMenu
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMainMenu.hpp"
#include "kyoshin/CArtsInfo.hpp"
extern "C" void __dt__9CMainMenuFv();
extern "C" void cbRenderBefore__9CMainMenuFv();
extern "C" void __ct__800FF300();
u32 func_800FEDF8(void) {
    extern u32 lbl_eu_80663F18;
    return lbl_eu_80663F18;
}









extern u32 lbl_eu_8052BDF4[];

// Copies 3 words from static data lbl_eu_8052BDF4 to fields at 0x3C/0x40/0x44,
// guarded by field_0x74 to ensure one-shot initialization.
extern "C" void func_800FEF20(CMainMenu* self) {
    if (self->field_0x74 != 0) {
        return;
    }
    u32 v0 = lbl_eu_8052BDF4[0];
    self->field_0x40 = lbl_eu_8052BDF4[1];
    self->field_0x3C = v0;
    self->field_0x44 = lbl_eu_8052BDF4[2];
}

void func_800FEF4C(){}

void __ct__800FF300(void* self){}

void CMainMenu::cbRenderBefore() {}

void func_800FF6BC(){}

extern u32 lbl_eu_80663F18;

// Declare the mangled symbol to prevent IPA from inlining the stub.
extern "C" int func_800FF778__9CMainMenuFv();

// Returns 1 if func_800FF778() is non-zero, else booleanizes lbl_eu_80663F18.
extern "C" u32 func_800FF738(CMainMenu* self) {
    if (func_800FF778__9CMainMenuFv() != 0) {
        return 1;
    }
    return lbl_eu_80663F18 != 0 ? 1 : 0;
}

int CMainMenu::func_800FF778() {
    // TODO: decompile
    return 0;
}

void func_800FF8B0(){}

extern "C" void func_800FF914(CArtsInfo* self) {
    self->field_0x54 = 1;
}

void func_800FF920(){}

void func_80100E14(){}

void func_801010B8(){}

void func_801018F4(){}

void func_80101A88(){}

void func_80101BF8(){}

void OnFileEvent__9CMainMenuFP10CEventFile(void* self) { ((void(*)(void*))__ct__800FF300)((char*)self - 0x58); }

void func_80102008(void* self) { ((void(*)(void*))__dt__9CMainMenuFv)((char*)self - 0x58); }

void func_80102010(void* self) { ((void(*)(void*))cbRenderBefore__9CMainMenuFv)((char*)self - 0x5c); }

extern "C" void func_80102018(void* self) { ((void(*)(void*))__dt__9CMainMenuFv)((char*)self - 0x5c); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide.
// Layout: inherits CProcess (0x00-0x3C), adds mMoveFunc at 0x3C and mDrawFunc at 0x48.
// Size: 0x54.

#include "monolib/work/CProcess.hpp"

class IUICf;

template <typename T>
class CTTask : public CProcess {
public:
    typedef void (CProcess::*MoveFunc)();
    typedef void (CProcess::*DrawFunc)();

    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();

protected:
    MoveFunc mMoveFunc;  // 0x3C — pointer-to-member-function (12 bytes)
    DrawFunc mDrawFunc;  // 0x48 — pointer-to-member-function (12 bytes)
};

// CTTask<IUICf>::Move — test PTMF at +0x3C, call if non-null
template<>
void CTTask<IUICf>::Move() {
    if (mMoveFunc) {
        (this->*mMoveFunc)();
    }
}

// CTTask<IUICf>::Draw — test PTMF at +0x48, call if non-null
template<>
void CTTask<IUICf>::Draw() {
    if (mDrawFunc) {
        (this->*mDrawFunc)();
    }
}
