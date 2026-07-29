// Auto-scaffolded catalog TU for kyoshin/CMainMenu
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMainMenu.hpp"
#include "kyoshin/CArtsInfo.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/math/math_types.h>
extern "C" void cbRenderBefore__9CMainMenuFv();
extern "C" void __ct__800FF300();

// CBaseCur shared helpers (defined in CCur.cpp)
extern void func_801D21CC(CBaseCur* cur);

// CBaseCur base destructor (defined in kyoshin/CCur.cpp)
extern "C" void __dt__8CBaseCurFv(void*, int);
// CProcess base destructor
extern "C" void __dt__8CProcessFv(void*, int);
// CSubCur destructor (defined in kyoshin/CCur.cpp)
extern "C" void* __dt__7CSubCurFv(CBaseCur*, int);
// UnkClass_8045F564 destructor
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);

// Destructor for CBaseCur-derived class at vtable 0x800FEA30.
// Standard MWCC virtual dtor: null-check, call base dtor with flag 0,
// conditionally operator delete, return this.
void* __dt__800FEA30(void* _this, int flags) {
    if (_this) {
        __dt__8CBaseCurFv(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

u32 func_800FEDF8(void) {
    extern u32 lbl_eu_80663F18;
    return lbl_eu_80663F18;
}

// Destructor for CProcess-derived class at vtable 0x800FED0C.
// The nested null-check reproduces MWCC's D2-inlined-into-D1 pattern
// (D2 has its own null guard, preserved when inlined).
void* __dt__800FED0C(void* _this, int flags) {
    if (_this) {
        if (_this) {
            __dt__8CProcessFv(_this, 0);
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// CMainMenu::~CMainMenu() — virtual destructor (D1/D2 merged).
// Destroys subobjects in reverse order: CSubCur at +0xA8, CBaseCur at +0x90,
// UnkClass_8045F564 at +0x60, CProcess at 0x00, then conditionally frees memory.
// The addic. for CBaseCur and the nested CProcess null-checks are MWCC
// D2-inlined-into-D1 artifacts.
extern "C" void* __dt__9CMainMenuFv(CMainMenu* _this, int flags) {
    if (_this) {
        CBaseCur* subCur = (CBaseCur*)((char*)_this + 0xA8);
        __dt__7CSubCurFv(subCur, -1);
        CBaseCur* baseCur = (CBaseCur*)((char*)_this + 0x90);
        if (baseCur) {
            __dt__8CBaseCurFv(baseCur, 0);
        }
        __dt__17UnkClass_8045F564Fv((char*)_this + 0x60, -1);
        if (_this) {
            if (_this) {
                __dt__8CProcessFv(_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
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

// rodata block containing resource names:
// offset 0x00: brlyt name (e.g. "mf00_menu.brlyt")
// offset 0x19: brlan name (e.g. "mf00_menu_in.brlan")
// offset 0x37: brlan name (e.g. "mf00_menu_out.brlan")
// offset 0x53: pane name "Param"
extern "C" char lbl_eu_804FCEBC[];

// VUpdate() override for the CBaseCur-derived class embedded at CMainMenu+0x90.
// Loads the layout and two animation transforms, then initializes via func_801D21CC.
extern "C" void func_800FEA88(CBaseCur* self) {
    func_80136E84(&self->mpLayout, self->mArcResAcc, &lbl_eu_804FCEBC[0]);
    func_80136F08(self->mpLayout, &self->mpAnimTrans0, self->mArcResAcc, &lbl_eu_804FCEBC[0x19]);
    func_80136F08(self->mpLayout, &self->mpAnimTrans1, self->mArcResAcc, &lbl_eu_804FCEBC[0x37]);
    self->mpLayout->UnbindAllAnimation();
    func_801D21CC(self);
}

void CMainMenu::cbRenderBefore() {}

// Finds the "Param" pane in the layout's root pane and sets its translate
// to the given position (3 floats: x, y, z).
void CMainMenu::func_800FEB14(float* pos) {
    nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(lbl_eu_804FCEBC + 0x53, true);
    if (pane != NULL) {
        pane->SetSRTElement(0, pos[0]);
        pane->SetSRTElement(1, pos[1]);
        pane->SetSRTElement(2, pos[2]);
    }
}

extern u32 lbl_eu_80663F18;

extern "C" void Regist__8CProcessFP8CProcessb(void* _this, void* parent, bool insertTop);

// Creates the CMainMenu singleton: allocates from work memory via placement new,
// stores in lbl_eu_80663F18, and registers as a child of the given CProcess.
// Returns NULL if already created.
extern "C" void* func_800FF6BC(void* parent, void* param) {
    if (lbl_eu_80663F18) {
        return NULL;
    }
    CMainMenu* menu = new (CWorkThreadSystem::getWorkMem()) CMainMenu(param);
    lbl_eu_80663F18 = (u32)menu;
    Regist__8CProcessFP8CProcessb(menu, parent, false);
    return (void*)lbl_eu_80663F18;
}

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

// Returns 1 if func_800FF778() is non-zero, else checks the global CMainMenu
// singleton's field_0xE0 for states 4 or 8 (active/invite).
extern "C" u32 func_800FF8B0() {
    if (func_800FF778__9CMainMenuFv() != 0) {
        return 1;
    }
    CMainMenu* menu = (CMainMenu*)(uintptr_t)lbl_eu_80663F18;
    if (menu == NULL) {
        return 0;
    }
    if (menu->field_0xE0 == 4) {
        return 1;
    }
    // cntlzw trick: returns 1 if field_0xE0 == 8, else 0
    return (menu->field_0xE0 == 8) ? 1 : 0;
}

// CMainMenu::Init — loads the menu layout file via CDeviceFile::readFile.
// The IWorkEvent at offset 0x58 receives OnFileEvent when the load completes.
extern u32 lbl_eu_80661DC0;

void CMainMenu::Init() {
    // Get IWorkEvent pointer at offset 0x58 (NULL-safe: if this is NULL, workEvent stays NULL)
    void* workEvent = this;
    if (this) {
        workEvent = (char*)this + 0x58;
    }
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    field_0x74 = CDeviceFile::readFile(handle, (const char*)lbl_eu_80661DC0, (IWorkEvent*)workEvent, 0, 0);
}

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

// Standard MWCC virtual dtor: empty body, compiler emits base dtor call + D1 wrapper
template<>
CTTask<IUICf>::~CTTask() {
}
