// Auto-scaffolded catalog TU for kyoshin/CMainMenu
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <string.h>
#include "kyoshin/CMainMenu.hpp"
#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceVI.hpp"          // waitForDrawDone
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/math/math_types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
// Real player base/spot types are in CfGimmick.hpp (CfPlayerBase::getHP at 0x128,
// CfPlayerSpot::partyOpen at 0x9C). Define minimal forward views here to avoid
// pulling the full CfGimmick header which conflicts on several extern "C" globals
// (func_8009CF8C/func_80124B78/getUnk80664658) already declared with different
// view structs in the menu headers.
class CfPlayerSpotReal {
public:
    virtual ~CfPlayerSpotReal();
    virtual void d01(); virtual void d02(); virtual void d03(); virtual void d04();
    virtual void d05(); virtual void d06(); virtual void d07(); virtual void d08();
    virtual void d09(); virtual void d10(); virtual void d11(); virtual void d12();
    virtual void d13(); virtual void d14(); virtual void d15(); virtual void d16();
    virtual void d17(); virtual void d18(); virtual void d19(); virtual void d20();
    virtual void d21(); virtual void d22(); virtual void d23(); virtual void d24();
    virtual void d25(); virtual void d26(); virtual void d27(); virtual void d28();
    virtual void d29(); virtual void d30(); virtual void d31(); virtual void d32();
    virtual void d33(); virtual void d34(); virtual void d35(); virtual void d36();
    virtual void partyOpen(u8* arg); // 0x9C
};
class CfPlayerBaseReal {
public:
    virtual ~CfPlayerBaseReal();
    virtual void d01(); virtual void d02(); virtual void d03(); virtual void d04();
    virtual void d05(); virtual void d06(); virtual void d07(); virtual void d08();
    virtual void d09(); virtual void d10(); virtual void d11(); virtual void d12();
    virtual void d13(); virtual void d14(); virtual void d15(); virtual void d16();
    virtual void d17(); virtual void d18(); virtual void d19(); virtual void d20();
    virtual void d21(); virtual void d22(); virtual void d23(); virtual void d24();
    virtual void d25(); virtual void d26(); virtual void d27(); virtual void d28();
    virtual void d29(); virtual void d30(); virtual void d31(); virtual void d32();
    virtual void d33(); virtual void d34(); virtual void d35(); virtual void d36();
    virtual void d37(); virtual void d38(); virtual void d39(); virtual void d40();
    virtual void d41(); virtual void d42(); virtual void d43(); virtual void d44();
    virtual void d45(); virtual void d46(); virtual void d47(); virtual void d48();
    virtual void d49(); virtual void d50(); virtual void d51(); virtual void d52();
    virtual void d53(); virtual void d54(); virtual void d55(); virtual void d56();
    virtual void d57(); virtual void d58(); virtual void d59(); virtual void d60();
    virtual void d61(); virtual void d62(); virtual void d63(); virtual void d64();
    virtual void d65(); virtual void d66(); virtual void d67(); virtual void d68();
    virtual void d69(); virtual void d70(); virtual void d71();
    virtual float getHP(); // 0x128
};

// C-linkage import (retail unmangled symbol).
extern "C" void func_8013676C(void* node, u32 font);
extern "C" void func_801390E0(CFileHandle** handle);
void releaseArcResourceAccessor(nw4r::lyt::ArcResourceAccessor* acc);

// CBaseCur shared helpers (defined in CCur.cpp)
extern "C" void func_801D21CC(CBaseCur* cur);

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

void* func_800FEDF8(void) {
    extern u32 lbl_eu_80663F18;
    return (void*)(uintptr_t)lbl_eu_80663F18;
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

// CMainMenu::~CMainMenu() - virtual destructor (D1/D2 merged).
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
    u32 v0;
    u32 *p = (u32 *)lbl_eu_8052BDF4;
    v0 = p[0];
    self->field_0x40 = p[1];
    self->field_0x3C = v0;
    self->field_0x44 = p[2];
}

extern "C" void func_800FEF4C(CMainMenu* self) {
    // Main-menu frame update: gate on the system window, refresh the menu
    // availability flags, then dispatch on the current state.
    if (func_8013BE50() == 0) {
        return;
    }
    if (lbl_eu_80663E24 & 0xAFE40000) {
        self->field_0x54 = 1;
        return;
    }
    if (func_8027EA64() != 0) return;
    if (getInstance__11CSysWinBuffFv() != 0) return;
    if (func_80192BD0() != 0) return;
    if (func_80167A18() != 0) return;
    if (func_8011CD5C() != 0) return;
    if (func_80212480() != 0) return;
    if (func_8022F530() != 0) return;
    if (func_80242354() != 0) return;
    if (func_80252CD4() != 0) return;
    if (func_80257308() != 0) return;
    if (func_80263944() != 0) return;
    if (func_8027037C() != 0) return;
    if (func_80272488() != 0) return;
    if (func_8028E440() != 0) return;
    if (func_8029BBA0() != 0) return;
    if (func_8029EE58() != 0) return;
    if (func_802AC510() != 0) return;

    // Refresh cursor/sub-menu availability flags.
    func_80101BF8(self);

    switch ((u32)self->field_0xE0) {
    case 0:
        // Enter: open sound + cursor-select state.
        playUISound__FUl(8);
        self->field_0xE0 = 1;
        break;
    case 1:
        // Cursor-select: once the intro animation finishes, set the current
        // cursor number (or its placeholder) and activate the cursor.
        if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(self->field_0x80, lbl_eu_80666F18) == 0) {
            break;
        }
        // Strings hoisted above the branch like retail; MWCC colors the
        // temps across both arms.
        {
            char* s1;
            char* s2;
            int n = self->field_0xC0 + 1;
            if (n > 0) {
                s1 = func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x67, n);
                s2 = func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x6c, n);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71, s1, 0);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c, s2, 0);
            } else {
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71,
                              lbl_eu_804FCEBC + 0x89, 0);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c,
                              lbl_eu_804FCEBC + 0x89, 0);
            }
            func_801D216C(&self->_90[0], 1);
            self->field_0xE0 = 2;
        }
        break;
    case 2:
        // Cursor-active: run the input handler for the selected menu.
        func_800FF920(self);
        break;
    case 3:
        // Exit: once the close animation finishes, mark the menu idle.
        if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(self->field_0x84, lbl_eu_80666F18) == 0) {
            break;
        }
        self->field_0x54 = 1;
        self->field_0xE0 = 0;
        break;
    case 4:
        // Screen-open dispatch: act on the cursor while no other window is up.
        if (func_80101A88(self) == 0 && func_8029A658() == 0) {
            switch (self->field_0xC0) {
            case 0:
                func_80134460();
                break;
            case 3:
                func_801341D8();
                break;
            }
        }
        self->field_0xE0 = 2;
        break;
    case 5:
        func_80100E14(self);
        break;
    case 6:
        func_801010B8(self);
        break;
    case 7:
        // Cursor-exit: like case 1 but driven by the exit animation.
        if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(self->field_0x8C, lbl_eu_80666F18) == 0) {
            break;
        }
        // Strings hoisted above the branch like case 1.
        {
            char* s1;
            char* s2;
            int n = self->field_0xC0 + 1;
            if (n > 0) {
                s1 = func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x67, n);
                s2 = func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x6c, n);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71, s1, 0);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c, s2, 0);
            } else {
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71,
                              lbl_eu_804FCEBC + 0x89, 0);
                func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c,
                              lbl_eu_804FCEBC + 0x89, 0);
            }
            self->field_0xE0 = 2;
        }
        break;
    case 8:
        func_801018F4(self);
        break;
    }

    // Epilogue: animate the layout and advance both cursors.
    self->field_0x7C->Animate(0);
    func_801D202C(&self->_90[0]);
    func_801D202C(&self->subCur);
}

// File-event handler (retail symbol __ct__800FF300): fires when the menu
// layout arc finished loading. Same shape as COption::OnFileEvent: open a
// scratch heap region (RAII Class_8045F858 guard), detach the file handle,
// attach it to a fresh ArcResourceAccessor, build the layout + four
// animations, bind the device font, reset text panes and animation state,
// then build both cursors on the stack and copy them into their slots.
// CMainMenu constructor (retail symbol __ct__CMainMenu: the split symbol
// carries no class-length mangling, so it stays a plain C-linkage global
// function rather than a member ctor - same idiom as __ct__CSimpleEveTalkWin).
//
// Runs CProcess's ctor then fills the IWorkEvent base region by hand (base
// ctor is out-of-line in retail, its effects are emitted inline here): the
// vtable at +0x10 is written twice (temp vtable lbl_eu_8052BF70, then the
// composite vtable lbl_eu_8052BE24 whose +0x24/+0xAC sub-tables back the
// +0x58/+0x5C interface pointers), the two __ptmf_null callback slots are
// copied, the embedded UnkClass_8045F564 region and both cursors are
// constructed, and finally the move-callback hook lbl_eu_8052BDE8 is installed.
extern "C" CMainMenu* __ct__CMainMenu(CMainMenu* _this, CScn* scene) {
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(_this));
    _this->mVtable = (void*)lbl_eu_8052BF70;

    // Composite vtable base and its two interface sub-table pointers,
    // materialized before the callback copy so MWCC schedules their lis/addi
    // ahead of the __ptmf_null base (same shape as __ct__CSysWinSelect).
    char* compVt = lbl_eu_8052BE24;
    u32 evtVt = (u32)(compVt + 0x24);
    u32 scnVt = (u32)(compVt + 0xac);

    // Copy the null member-function pointer into both callback slots (store
    // order 0x40,0x3C,0x44 then 0x4C,0x48,0x50). Post-increment derefs of a
    // local pointer make MWCC fold the first access into `lwzu` instead of an
    // extra addi-materialised pointer - cf. CTaskGameEvt / CSysWinSelect.
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    _this->field_0x40 = w1;
    _this->field_0x3C = w0;
    u32 w2 = *src++;
    _this->field_0x44 = w2;
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    _this->field_0x4C = w0;
    _this->field_0x48 = w1;
    w2 = *src++;
    _this->field_0x50 = w2;

    _this->field_0x54 = 0;
    _this->field_0x55 = 0;
    _this->mVtable = (void*)compVt;
    _this->mIWorkEventVtbl = evtVt;
    _this->field_0x5C = scnVt;

    __ct__17UnkClass_8045F564Fv(reinterpret_cast<u8*>(_this) + 0x60);

    CBaseCur* baseCur = reinterpret_cast<CBaseCur*>(reinterpret_cast<u8*>(_this) + 0x90);
    _this->field_0x70 = scene;
    _this->field_0x74 = NULL;
    _this->field_0x78 = NULL;
    _this->field_0x7C = NULL;
    _this->field_0x80 = NULL;
    _this->field_0x84 = NULL;
    _this->field_0x88 = NULL;
    _this->field_0x8C = NULL;
    __ct__8CBaseCurFv(baseCur, NULL);
    baseCur->mVtable = (void*)lbl_eu_8052BF28;
    __ct__CSubCur(reinterpret_cast<CBaseCur*>(reinterpret_cast<u8*>(_this) + 0xA8), NULL);

    _this->field_0xC0 = 3;

    // Install the move-callback hook. Retail folds the symbol's low half into
    // an update-form first load, then reads words 1/2 relative to the advanced
    // pointer; store order is 0x40, 0x3C, 0x44.
    // v0-before-pointer declaration order: the lwzu result claims r4 and the
    // base pointer takes r5 (cf. func_802A97A0).
    u32 h0;
    u32* hook = lbl_eu_8052BDE8;
    _this->field_0xC4 = 0;
    _this->field_0xE0 = 0;
    h0 = *hook++;
    _this->field_0x40 = hook[0];
    _this->field_0x3C = h0;
    _this->field_0x44 = hook[1];
    return _this;
}

extern "C" bool __ct__800FF300(CMainMenu* self, CEventFile* pEventFile) {
    if (self->field_0x74 == pEventFile->mFileHandle) {
        // Long-lived rodata base: retail caches lbl_eu_804FCEBC in a
        // callee-saved register and offsets every resource name from it.
        char* base = lbl_eu_804FCEBC;
        reinterpret_cast<UnkClass_8045F564*>(&self->_60[0])->createRegion(
            mtl::MemManager::getHandleMEM2(), 0xCC00, &base[0x8a], 0);
        Class_8045F858 memHost(reinterpret_cast<UnkClass_8045F564*>(&self->_60[0]));

        // Take ownership of the loaded file data (getData clears the handle's
        // data pointer) and release the loader's lock.
        void* fileData = self->field_0x74->getData();
        mtl::MemManager::setMemInitFlag(false);

    self->field_0x78 = createArcResourceAccessor__10CLibLayoutFv();
            self->field_0x78->Attach(fileData, &base[0x94]);
            buildLayout(&self->field_0x7C, self->field_0x78, &base[0x98]);
            bindLayoutAnimTransform(self->field_0x7C, &self->field_0x80, self->field_0x78,
                          &base[0xaf]);
            bindLayoutAnimTransform(self->field_0x7C, &self->field_0x84, self->field_0x78,
                          &base[0xc9]);
            bindLayoutAnimTransform(self->field_0x7C, &self->field_0x88, self->field_0x78,
                          &base[0xe4]);
            bindLayoutAnimTransform(self->field_0x7C, &self->field_0x8C, self->field_0x78,
                          &base[0x102]);

            // Bind the device font: ask the font object for its pane (vtable+0x24)
            // and push it onto the layout root.
            nw4r::lyt::Pane* rootPane = *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10);
            func_8013676C(
                rootPane,
                reinterpret_cast<CMainMenuFontView*>(
                    getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->field_0x7C))
                    ->sf9());

            func_80136B4C(self->field_0x7C, &base[0x71], &base[0x89], 0);
            func_80136B4C(self->field_0x7C, &base[0x7c], &base[0x89], 0);

            // Park all animations except the intro (anim0), which stays enabled.
            self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x80, true);
            self->field_0x80->SetFrame(lbl_eu_80666F1C);
            self->field_0x7C->Animate(0);

            // Build the base cursor on the stack and copy its members into +0x90
            // (implicit copy-assign skips the vptr), then run its init virtual.
            {
                u8 temp[0x18];
                __ct__8CBaseCurFv((CBaseCur*)temp, self->field_0x78);
                ((CBaseCur*)temp)->mVtable = (void*)lbl_eu_8052BF28;
                ((CBaseCur*)&self->_90[0])->mArcResAcc = ((CBaseCur*)temp)->mArcResAcc;
                ((CBaseCur*)&self->_90[0])->mpLayout = ((CBaseCur*)temp)->mpLayout;
                ((CBaseCur*)&self->_90[0])->mpAnimTrans0 = ((CBaseCur*)temp)->mpAnimTrans0;
                ((CBaseCur*)&self->_90[0])->mpAnimTrans1 = ((CBaseCur*)temp)->mpAnimTrans1;
                ((CBaseCur*)&self->_90[0])->mActive = ((CBaseCur*)temp)->mActive;
                ((CBaseCur*)&self->_90[0])->mVisible = ((CBaseCur*)temp)->mVisible;
                __dt__8CBaseCurFv(temp, 0);
                ((CBaseCurVtIf*)&self->_90[0])->VUpdate();
            }
            // Build the sub cursor on the stack and copy its members into +0xA8.
            {
                u8 temp[0x18];
                __ct__CSubCur((CBaseCur*)temp, self->field_0x78);
                ((CMainMenuSubCurView*)&self->subCur)->mArcResAcc =
                    ((CMainMenuSubCurView*)temp)->mArcResAcc;
                ((CMainMenuSubCurView*)&self->subCur)->mpLayout =
                    ((CMainMenuSubCurView*)temp)->mpLayout;
                ((CMainMenuSubCurView*)&self->subCur)->mpAnimTrans0 =
                    ((CMainMenuSubCurView*)temp)->mpAnimTrans0;
                ((CMainMenuSubCurView*)&self->subCur)->mpAnimTrans1 =
                    ((CMainMenuSubCurView*)temp)->mpAnimTrans1;
                ((CMainMenuSubCurView*)&self->subCur)->mActive =
                    ((CMainMenuSubCurView*)temp)->mActive;
                ((CMainMenuSubCurView*)&self->subCur)->mVisible =
                    ((CMainMenuSubCurView*)temp)->mVisible;
                __dt__7CSubCurFv((CBaseCur*)temp, -1);
                ((CBaseCurVtIf*)&self->subCur)->VUpdate();
            }

            func_80101BF8(self);

            // Register the render callback slot (+0x5C) with the owning scene; the
            // null-check branch is the standard cross-cast idiom.
            IScnRender* render = reinterpret_cast<IScnRender*>(self);
            if (self != NULL) {
                render = reinterpret_cast<IScnRender*>((char*)self + 0x5C);
            }
            self->field_0x70->addRenderCB(render, 0xa, 0);

            self->field_0x74 = NULL;
            reinterpret_cast<UnkClass_8045F564*>(&self->_60[0])->func_8045F810();
            return true;
    }
    return false;
}

// rodata block containing resource names:
// offset 0x00: brlyt name (e.g. "mf00_menu.brlyt")
// offset 0x19: brlan name (e.g. "mf00_menu_in.brlan")
// offset 0x37: brlan name (e.g. "mf00_menu_out.brlan")
// offset 0x53: pane name "Param"

// VUpdate() override for the CBaseCur-derived class embedded at CMainMenu+0x90.
// Loads the layout and two animation transforms, then initializes via func_801D21CC.
extern "C" void func_800FEA88(CBaseCur* self) {
    buildLayout(&self->mpLayout, self->mArcResAcc, &lbl_eu_804FCEBC[0]);
    bindLayoutAnimTransform(self->mpLayout, &self->mpAnimTrans0, self->mArcResAcc, &lbl_eu_804FCEBC[0x19]);
    bindLayoutAnimTransform(self->mpLayout, &self->mpAnimTrans1, self->mArcResAcc, &lbl_eu_804FCEBC[0x37]);
    self->mpLayout->UnbindAllAnimation();
    func_801D21CC(self);
}

// Render callback: draws the menu layout and its two embedded cursors.
// Gated on task-busy state, the global mode bitfield (bit 10), the
// system-window-active gate, and the presentation flag word.
void CMainMenu::cbRenderBefore() {
    // Single OR so MWCC emits the short-circuit branches: A -> bne exit,
    // B -> beq continue / b exit (same shape as CMiniMap::cbRenderBefore).
    if (CTaskGame::getInstance()->isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (!func_8013BE50())
        return;
    if (lbl_eu_80663E24 & 0xAFE40000)
        return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Scope-exit dtor is auto-emitted as the retail direct call; an explicit
    // dtor call would emit a virtual dispatch.
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250(&drawInfo);
    drawLayout(field_0x7C, &drawInfo, 0, 1);
    func_801D20B0((char*)this + 0x90, &drawInfo);
    func_801D20B0((char*)this + 0xA8, &drawInfo);
}

// Finds the "Param" pane in the layout's root pane and sets its translate
// to the given position (3 floats: x, y, z).
void CMainMenu::func_800FEB14(float* pos) {
    nw4r::lyt::Pane* pane =
        field_0x08->GetRootPane()->FindPaneByName(lbl_eu_804FCEBC + 0x53, true);
    if (pane != NULL) {
        pane->SetSRTElement(0, pos[0]);
        pane->SetSRTElement(1, pos[1]);
        pane->SetSRTElement(2, pos[2]);
    }
}

extern u32 lbl_eu_80663F18;

// Creates the CMainMenu singleton: allocates from work memory,
// constructs via the unmangled retail ctor fragment __ct__CMainMenu,
// stores in lbl_eu_80663F18, and registers as a child of the given CProcess.
// Returns NULL if already created.
extern "C" void* func_800FF6BC(void* parent, void* param) {
    if (lbl_eu_80663F18) {
        return NULL;
    }
    u32 workMem = CWorkThreadSystem::getWorkMem();
    CMainMenu* menu = (CMainMenu*)mtl::MemManager::allocate(0xE4, workMem);
    if (menu != NULL) {
        // __ct__CMainMenu returns _this; keep the value in the return reg
        // so MWCC needs no extra spill across the call.
        menu = __ct__CMainMenu(menu, (CScn*)param);
    }
    lbl_eu_80663F18 = (u32)menu;
    Regist__8CProcessFP8CProcessb(menu, parent, false);
    return (void*)lbl_eu_80663F18;
}

extern u32 lbl_eu_80663F18;

// Returns 1 if isMenuOpen() is non-zero, else booleanizes lbl_eu_80663F18.
extern "C" u32 func_800FF738(CMainMenu* self) {
    if (isAnyMenuOpen__9CMainMenuFv() != 0) {
        return 1;
    }
    return lbl_eu_80663F18 != 0 ? 1 : 0;
}

int CMainMenu::isAnyMenuOpen() {
    // Menu-open gate: returns 1 while any menu screen is active (each callee
    // is a singleton/state guard), otherwise the tutorial-list state.
    if (func_80192BD0()) return 1;
    if (func_80167A18()) return 1;
    if (func_8011CD5C()) return 1;
    if (func_80212480()) return 1;
    if (func_8022F530()) return 1;
    if (func_80242354()) return 1;
    if (func_80252CD4()) return 1;
    if (func_80257308()) return 1;
    if (func_80263944()) return 1;
    if (func_8027037C()) return 1;
    if (func_80272488()) return 1;
    if (func_8028E440()) return 1;
    if (func_8029BBA0()) return 1;
    if (func_8029EE58()) return 1;
    return func_802AC510();
}

// Returns 1 if isMenuOpen() is non-zero, else checks the global CMainMenu
// singleton's field_0xE0 for states 4 or 8 (active/invite).
extern "C" u32 func_800FF8B0() {
    if (isAnyMenuOpen__9CMainMenuFv() != 0) {
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

// CMainMenu::Init - loads the menu layout file via CDeviceFile::readFile.
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

// Pane-flag setter used by the sub-menu open blocks: find the named pane
// through a fresh layout-root load (retail reloads it every lookup) and
// rewrite its enable bit (mFlag bit0). All lookups recurse except the last.
#define MENU_SET_PANE_FLAG(self_, name_, on_, rec_)                              \
    do {                                                                         \
        nw4r::lyt::Pane* pane_ =                                                 \
            (*(nw4r::lyt::Pane**)((u8*)(self_)->field_0x7C + 0x10))              \
                ->FindPaneByName((name_), (rec_));                               \
        if (on_) {                                            \
            pane_->SetVisible(true);                          \
        } else {                                              \
            pane_->SetVisible(false);                         \
        }                                                     \
    } while (0)

// Per-frame main-menu input handler (state 2): scroll the main cursor,
// open the selected sub-screen, or cancel back to the exit state.
void func_800FF920(CMainMenu* self) {
    if (func_80101A88(self) != 0) {
        // Input locked: stop the open animations, run the close animation,
        // clear both number panes and drop to the exit state.
        self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, true);
        // Retail materializes the rodata base only here, after the anim calls.
        char* base = lbl_eu_804FCEBC;
        func_80136B4C(self->field_0x7C, base + 0x71, base + 0x89, 0);
        func_80136B4C(self->field_0x7C, base + 0x7c, base + 0x89, 0);
        func_801D216C(&self->_90[0], 0);
        playUISound__FUl(9);
        self->field_0xE0 = 3;
        return;
    }
    if (func_8029A658() != 0) {
        return;
    }
    CMainMenuPad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
    // Held-button early-outs per input style.
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        if (pad->mHeldButtonFlags & 0x08000000) {
            return;
        }
    } else {
        if (pad->mHeldButtonFlags & 0x800) {
            return;
        }
    }

    u32 down, up, aPressed, bPressed, confirm;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        u32 p = pad->mPressedButtonFlags;
        u32 t = pad->mTurboPressButtonFlags;
        u32 cHi = p & 0x400000;
        aPressed = (p >> 21) & 1;
        u32 cLo = p & 0x400;
        down = t & 1;
        confirm = (cHi | cLo) != 0;
        up = (t >> 1) & 1;
        bPressed = (p >> 26) & 1;
    } else {
        u32 p = pad->mPressedButtonFlags;
        u32 t = pad->mTurboPressButtonFlags;
        u32 c = p & 0x420;
        aPressed = (p >> 4) & 1;
        down = t & 1;
        up = (t >> 1) & 1;
        bPressed = (p >> 12) & 1;
        confirm = c != 0;
    }

    if (down != 0) {
        // Down: wrap the main cursor backward and reposition it.
        s32 idx = self->field_0xC0 - 1;
        self->field_0xC0 = idx;
        if (idx < 0) {
            self->field_0xC0 = 6;
        }
        // "Param" anchor pane gives y/z; x comes from the cursor-x table.
        nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10);
        nw4r::lyt::Pane* pane = root->FindPaneByName(lbl_eu_804FCEBC + 0x53, true);
        nw4r::math::VEC3 vec;
        if (pane != NULL) {
            vec = pane->GetTranslate();
        }
        vec.x = lbl_eu_804FCD60[self->field_0xC0];
        ((CBaseCurVtIf*)&self->_90[0])->setRootPaneTranslate(&vec);
        // Refresh the "N" counter panes for the new cursor index.
        int n = self->field_0xC0 + 1;
        if (n > 0) {
            char* base = lbl_eu_804FCEBC;
            char* s1 = func_80136190(base + 0x5e, base + 0x67, n);
            char* s2 = func_80136190(base + 0x5e, base + 0x6c, n);
            func_80136B4C(self->field_0x7C, base + 0x71, s1, 0);
            func_80136B4C(self->field_0x7C, base + 0x7c, s2, 0);
        } else {
            char* base = lbl_eu_804FCEBC;
            func_80136B4C(self->field_0x7C, base + 0x71, base + 0x89, 0);
            func_80136B4C(self->field_0x7C, base + 0x7c, base + 0x89, 0);
        }
        playUISound__FUl(0x6a);
        return;
    }
    if (up != 0) {
        // Up: wrap the main cursor forward and reposition it.
        if (++self->field_0xC0 > 6) {
            self->field_0xC0 = 0;
        }
        nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10);
        nw4r::lyt::Pane* pane = root->FindPaneByName(lbl_eu_804FCEBC + 0x53, true);
        nw4r::math::VEC3 vec;
        if (pane != NULL) {
            vec = pane->GetTranslate();
        }
        vec.x = lbl_eu_804FCD60[self->field_0xC0];
        ((CBaseCurVtIf*)&self->_90[0])->setRootPaneTranslate(&vec);
        int n = self->field_0xC0 + 1;
        if (n > 0) {
            char* base = lbl_eu_804FCEBC;
            char* s1 = func_80136190(base + 0x5e, base + 0x67, n);
            char* s2 = func_80136190(base + 0x5e, base + 0x6c, n);
            func_80136B4C(self->field_0x7C, base + 0x71, s1, 0);
            func_80136B4C(self->field_0x7C, base + 0x7c, s2, 0);
        } else {
            char* base = lbl_eu_804FCEBC;
            func_80136B4C(self->field_0x7C, base + 0x71, base + 0x89, 0);
            func_80136B4C(self->field_0x7C, base + 0x7c, base + 0x89, 0);
        }
        playUISound__FUl(0x6a);
        return;
    }
    if (aPressed == 0) {
        if (confirm == 0) {
            if (bPressed != 0) {
                func_8013D8A0();
            }
            return;
        }
        // Cancel: same tail as the input-lock branch above.
        self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, true);
        char* base = lbl_eu_804FCEBC;
        func_80136B4C(self->field_0x7C, base + 0x71, base + 0x89, 0);
        func_80136B4C(self->field_0x7C, base + 0x7c, base + 0x89, 0);
        func_801D216C(&self->_90[0], 0);
        playUISound__FUl(9);
        self->field_0xE0 = 3;
        return;
    }

    // Confirm: reject unavailable entries, otherwise open the selected menu.
    u32 sel = self->field_0xC0;
    if (self->field_0xC8[sel] != 0) {
        playUISound__FUl(5);
        return;
    }
    // Sub-menu open: reveal the four entries of the selected page (14 panes
    // total), swap to the sub-cursor open animation and park the state at 5.
    // Retail duplicates the anim tail inside every case, so each arm carries
    // its own copy here too.
    switch (sel) {
    case 0:
    case 3:
        // Header entries: activate the main cursor and enter screen-open state.
        func_801D2174((CBaseCur*)&self->_90[0]);
        self->field_0xE0 = 4;
        playUISound__FUl(0x6b);
        return;
    case 1: {
        char* base = lbl_eu_804FCEBC;
        MENU_SET_PANE_FLAG(self, base + 0x142, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x150, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x15e, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x16c, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x17a, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x188, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x196, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1a4, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1b2, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1c0, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1ce, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1dc, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1ea, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1f8, 0, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, true);
        self->field_0x88->SetFrame(lbl_eu_80666F1C);
        self->field_0xE0 = 5;
        playUISound__FUl(0x6c);
        return;
    }
    case 2: {
        char* base = lbl_eu_804FCEBC;
        MENU_SET_PANE_FLAG(self, base + 0x142, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x150, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x15e, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x16c, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x17a, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x188, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x196, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1a4, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1b2, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1c0, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1ce, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1dc, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1ea, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1f8, 0, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, true);
        self->field_0x88->SetFrame(lbl_eu_80666F1C);
        self->field_0xE0 = 5;
        playUISound__FUl(0x6c);
        return;
    }
    case 4: {
        char* base = lbl_eu_804FCEBC;
        MENU_SET_PANE_FLAG(self, base + 0x142, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x150, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x15e, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x16c, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x17a, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x188, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x196, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x1a4, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x1b2, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x1c0, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x1ce, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1dc, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1ea, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1f8, 0, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, true);
        self->field_0x88->SetFrame(lbl_eu_80666F1C);
        self->field_0xE0 = 5;
        playUISound__FUl(0x6c);
        return;
    }
    case 5: {
        char* base = lbl_eu_804FCEBC;
        MENU_SET_PANE_FLAG(self, base + 0x142, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x150, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x15e, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x16c, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x17a, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x188, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x196, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1a4, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1b2, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1c0, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1ce, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x1dc, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x1ea, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1f8, 0, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, true);
        self->field_0x88->SetFrame(lbl_eu_80666F1C);
        self->field_0xE0 = 5;
        playUISound__FUl(0x6c);
        return;
    }
    case 6: {
        char* base = lbl_eu_804FCEBC;
        MENU_SET_PANE_FLAG(self, base + 0x142, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x150, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x15e, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x16c, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x17a, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x188, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x196, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1a4, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1b2, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1c0, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1ce, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1dc, 0, true);
        MENU_SET_PANE_FLAG(self, base + 0x1ea, 1, true);
        MENU_SET_PANE_FLAG(self, base + 0x1f8, 1, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, true);
        self->field_0x88->SetFrame(lbl_eu_80666F1C);
        self->field_0xE0 = 5;
        playUISound__FUl(0x6c);
        return;
    }
    default:
        return;
    }
}

extern "C" void func_80100E14(CMainMenu* self) {
    // Per-frame menu advance: update the sub-cursor pane names/positions from
    // the current (state, sub-state) pair, then park the state at 6 (idle).
    if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(self->field_0x88, lbl_eu_80666F18) == 0) {
        return;
    }
    self->field_0xC4 = 0;
    int idx = 0;
    switch (self->field_0xC0) {
    case 1: idx = 0x8; break;
    case 2: idx = 0xb; break;
    case 4: idx = 0xd; break;
    case 5: idx = 0x11; break;
    case 6: idx = 0x13; break;
    }
    if (idx == 0x13) {
        // All-states cursor: while any presentation/player condition holds the
        // cursor index is forced to 0x16 (no free roam).
        int cond;
        if (getUnk80664658() != 0 && (getUnk80664658()->field_214 & 0x8)) {
            cond = 1;
        } else if (lbl_eu_80663E28 & 0x40000000) {
            cond = 1;
        } else {
            CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
            if (spot == 0) goto math;
            if (getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4 == 0) goto math;
            u32 flags = getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4->field_4EC;
            if ((flags & 0x1000000) || (flags & 0x8000) == 0 || (flags & 0x100)) {
                cond = 1;
            } else {
            math:
                // Retail booleanizes this bound check arithmetically
                // (subi/orc/srwi/subf/srwi idiom for unsigned >=).
                cond = (func_8009CF8C(0x20) >= 0x18E);
            }
        }
        if (cond != 0) idx = 0x16;
    }
    // Hoisted rodata base: retail keeps it in r29 and offsets from it.
    char* base = lbl_eu_804FCEBC;
    char buf[0x20];
    sprintf(buf, base + 0x121, self->field_0xC0 + 1, self->field_0xC4 + 1);
    // First two lookups go through the root-pane virtual; the fourth argument
    // of func_80137924 is the layout's raw +0x10 root pointer (no dispatch).
    // Declaration order nudges MWCC's callee-saved coloring.
    nw4r::lyt::Pane* pane1;
    nw4r::lyt::Pane* pane2;
    nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10);
    pane1 = root->FindPaneByName(buf, true);
    pane2 = root->FindPaneByName(base + 0x133, true);
    nw4r::math::VEC3 vec;
    // Fourth argument reloads the raw root pointer (no dispatch).
    func_80137924(&vec, pane1, pane2,
                  *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10));
    ((CBaseCurVtIf*)&self->subCur)->setRootPaneTranslate(&vec);
    if (idx > 0) {
        char* s1 = func_80136190(base + 0x5e, base + 0x67, idx);
        char* s2 = func_80136190(base + 0x5e, base + 0x6c, idx);
        func_80136B4C(self->field_0x7C, base + 0x71, s1, 0);
        func_80136B4C(self->field_0x7C, base + 0x7c, s2, 0);
    } else {
        func_80136B4C(self->field_0x7C, base + 0x71, base + 0x89, 0);
        func_80136B4C(self->field_0x7C, base + 0x7c, base + 0x89, 0);
    }
    func_801D216C(&self->_90[0], 0);
    func_801D216C(&self->subCur, 1);
    self->field_0xE0 = 6;
}

extern "C" void func_801010B8(CMainMenu* self) {
    // Cursor-select sub-menu input: gate on the global input lock, then
    // handle sub-cursor movement / confirmation / cancel.

    if (func_80101A88(self) != 0) {
        // Input locked: stop the intro animations and move to the exit state.
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, true);
        self->field_0x8C->SetFrame(lbl_eu_80666F1C);
        func_801D216C(&self->_90[0], 1);
        func_801D216C(&self->subCur, 0);
        playUISound__FUl(6);
        self->field_0xE0 = 7;
        return;
    }
    if (func_8029A658() != 0) {
        return;
    }

    CMainMenuPad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
    // First mode query: held-button early-outs per input style.
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        if (pad->mHeldButtonFlags & 0x08000000) {
            return;
        }
    } else {
        if (pad->mHeldButtonFlags & 0x800) {
            return;
        }
    }
    u32 down, up, aPressed, bPressed, confirm;
    // Second mode query picks the button extraction.
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        u32 p = pad->mPressedButtonFlags;
        u32 t = pad->mTurboPressButtonFlags;
        int c = ((p >> 22) & 1) | ((p >> 10) & 1);
        aPressed = (p >> 21) & 1;
        down = t & 1;
        confirm = c != 0;
        up = (t >> 1) & 1;
        bPressed = (p >> 26) & 1;
    } else {
        u32 p = pad->mPressedButtonFlags;
        u32 t = pad->mTurboPressButtonFlags;
        int c = p & 0x420;
        aPressed = (p >> 4) & 1;
        down = t & 1;
        confirm = c != 0;
        up = (t >> 1) & 1;
        bPressed = (p >> 12) & 1;
    }

    if (down != 0) {
        // Down: wrap the sub-cursor backward, then move the cursor pane.
        switch (self->field_0xC0) {
        case 1:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 2;
            break;
        case 2:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 1;
            break;
        case 4:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 3;
            break;
        case 5:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 1;
            break;
        case 6:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 2;
            break;
        }
        // Hoisted rodata base like retail (cached in a callee-saved reg).
        char* base = lbl_eu_804FCEBC;
        char buf[0x20];
        sprintf(buf, base + 0x121, self->field_0xC0 + 1, self->field_0xC4 + 1);
        // Raw +0x10 root-pane loads like retail (no accessor call).
        nw4r::lyt::Pane* pane1 =
            (*(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10))->FindPaneByName(buf, true);
        nw4r::lyt::Pane* pane2 =
            (*(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10))
                ->FindPaneByName(base + 0x133, true);
        nw4r::math::VEC3 vec;
        func_80137924(&vec, pane1, pane2,
                      *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10));
        ((CBaseCurVtIf*)&self->subCur)->setRootPaneTranslate(&vec);
        playUISound__FUl(0x6a);
        goto tail;
    }
    if (up != 0) {
        // Up: wrap the sub-cursor forward, then move the cursor pane.
        switch (self->field_0xC0) {
        case 1:
            if (++self->field_0xC4 > 2) self->field_0xC4 = 0;
            break;
        case 2:
            if (++self->field_0xC4 > 1) self->field_0xC4 = 0;
            break;
        case 4:
            if (++self->field_0xC4 > 3) self->field_0xC4 = 0;
            break;
        case 5:
            if (++self->field_0xC4 > 1) self->field_0xC4 = 0;
            break;
        case 6:
            if (++self->field_0xC4 > 2) self->field_0xC4 = 0;
            break;
        }
        char* base = lbl_eu_804FCEBC;
        char buf[0x20];
        sprintf(buf, base + 0x121, self->field_0xC0 + 1, self->field_0xC4 + 1);
        nw4r::lyt::Pane* pane1 =
            (*(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10))->FindPaneByName(buf, true);
        nw4r::lyt::Pane* pane2 =
            (*(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10))
                ->FindPaneByName(base + 0x133, true);
        nw4r::math::VEC3 vec;
        func_80137924(&vec, pane1, pane2,
                      *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10));
        ((CBaseCurVtIf*)&self->subCur)->setRootPaneTranslate(&vec);
        playUISound__FUl(0x6a);
        goto tail;
    }
    if (aPressed != 0) {
        // Confirm: reject blocked items with a buzz, otherwise open the
        // selected sub-screen (party state for the special "free roam" item).
        u32 idx;
        switch (self->field_0xC0) {
        case 1: idx = (u8)self->field_0xC4; break;
        case 2: idx = (u8)(self->field_0xC4 + 3); break;
        case 4: idx = (u8)(self->field_0xC4 + 5); break;
        case 5: idx = (u8)(self->field_0xC4 + 9); break;
        case 6: idx = (u8)(self->field_0xC4 + 0xb); break;
        default: idx = 0; break;
        }
        if (self->field_0xC8[(u8)idx + 7] != 0) {
            playUISound__FUl(5);
            return;
        }
        if (self->field_0xC0 == 6 && self->field_0xC4 == 0) {
            // Free-roam entry: open the party screen via the spot object.
            // Retail biases the returned pointer back to the player base
            // before the null check and dispatches slot 0x9C with the address
            // of the sub-object's 0x3B4 region.
            setPresentationFlag__Q22cf13CfGameManagerFv(true);
            CMainMenuPlayer* player =
                (CMainMenuPlayer*)getPlayer__Q22cf13CfGameManagerFi(0);
            if (player != 0) {
                player = (CMainMenuPlayer*)((char*)player - 0x3E9C);
            }
            if (player != 0 && player->spot.field_0xC4 != 0) {
                ((CfPlayerSpotReal*)&player->spot)->partyOpen(&player->spot.field_0xC4->field_0x3B4[0]);
            }
        }
        playUISound__FUl(0x6b);
        self->field_0xE0 = 8;
        func_801D2174((CBaseCur*)&self->subCur);
        goto tail;
    }
    if (confirm != 0) {
        // Cancel: stop the animations and move to the exit state.
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, true);
        self->field_0x8C->SetFrame(lbl_eu_80666F1C);
        func_801D216C(&self->_90[0], 1);
        func_801D216C(&self->subCur, 0);
        playUISound__FUl(6);
        self->field_0xE0 = 7;
    } else if (bPressed != 0) {
        func_8013D8A0();
    }

tail:
    // Common tail: pick the sub-cursor index for the selected menu and set
    // its text panes (free-roam gate identical to func_80100E14).
    int idx = 0;
    switch (self->field_0xC0) {
    case 1: idx = self->field_0xC4 + 0x8; break;
    case 2: idx = self->field_0xC4 + 0xb; break;
    case 4: idx = self->field_0xC4 + 0xd; break;
    case 5: idx = self->field_0xC4 + 0x11; break;
    case 6:
        idx = self->field_0xC4 + 0x13;
        if (idx == 0x13) {
            int cond;
            if (getUnk80664658() != 0 && (getUnk80664658()->field_214 & 0x8)) {
                cond = 1;
            } else if (lbl_eu_80663E28 & 0x40000000) {
                cond = 1;
            } else {
                CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
                if (spot == 0) goto math;
                if (getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4 == 0) goto math;
                u32 flags = getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4->field_4EC;
                if ((flags & 0x1000000) || (flags & 0x8000) == 0 || (flags & 0x100)) {
                    cond = 1;
                } else {
                math:
                    u32 x = func_8009CF8C(0x20);
                    u32 a = x | ~0x18E;
                    u32 b = (x - 0x18E) >> 1;
                    cond = (a - b) >> 31;
                }
            }
            if (cond != 0) idx = 0x16;
            if (lbl_eu_80663E28 & 0x40000000) idx = 0x17;
        } else if (idx == 0x14 && (lbl_eu_80663E28 & 0x40000000)) {
            idx = 0x18;
        }
        break;
    }
    if (idx > 0) {
        char* s1 = func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x67, idx);
        char* s2 = func_80136190(lbl_eu_804FCEBC + 0x5e, lbl_eu_804FCEBC + 0x6c, idx);
        func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71, s1, 0);
        func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c, s2, 0);
    } else {
        func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x71, lbl_eu_804FCEBC + 0x89, 0);
        func_80136B4C(self->field_0x7C, lbl_eu_804FCEBC + 0x7c, lbl_eu_804FCEBC + 0x89, 0);
    }
}

extern "C" void func_801018F4(CMainMenu* self) {
    // Per-frame menu-state dispatch: while no other screen is open, run the
    // handler for the current (state, sub-state) pair, then park the state
    // at 6 (idle) for the next frame.
    if (func_80101A88(self) == 0 && func_8029A658() == 0) {
        switch (self->field_0xC0) {
        case 1:
            switch (self->field_0xC4) {
            case 0: func_80133D78(); break;
            case 1: func_801342B0(); break;
            case 2: func_80134714(); break;
            }
            break;
        case 2:
            switch (self->field_0xC4) {
            case 0: func_80134388(); break;
            case 1: func_801348C8(); break;
            }
            break;
        case 4:
            switch (self->field_0xC4) {
            case 0: func_80133A08(0); break;
            case 1: func_801347EC(1); break;
            case 2: func_80134A78(); break;
            case 3: func_801349A0(); break;
            }
            break;
        case 5:
            switch (self->field_0xC4) {
            case 0: func_80133CA0(); break;
            case 1: func_80134F2C(0); break;
            }
            break;
        case 6:
            switch (self->field_0xC4) {
            case 0: func_80134B50(0, 1); break;
            case 1: func_80134C34(); break;
            case 2: func_80134E50(1); break;
            }
            break;
        }
    }
    self->field_0xE0 = 6;
}

extern "C" int func_80101A88(CMainMenu* self) {
    (void)self;
    // Gameplay-input gate: returns 1 while input should be blocked (dead
    // player, active battle list, closing menus, or presentation flags).
    CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
    CMainMenuPlayer* player = (CMainMenuPlayer*)spot;
    if (spot != 0) {
        // De-bias the embedded +0x3E9C spot object back to the player base.
        player = (CMainMenuPlayer*)((char*)spot - 0x3E9C);
    }
    if (player != 0) {
        float hp = ((CfPlayerBaseReal*)player)->getHP();
        if (lbl_eu_80666F1C == hp) return 1;
        CMainMenuPlayerSub* sub = player->spot.field_0xC4;
        if (sub != 0) {
            int cond = (sub->field_4EC >> 15) & 1;
            if (isSceneLoading__Q22cf13CfGameManagerFv() != 0) cond = 1;
            // Retail shares one return-1 tail (L1C) for the flag/action/arts
            // gates; goto anchors that shared block at its retail position.
            if (cond == 0 && (sub->field_4EC & 2) == 0) goto ret1;
            if (sub->field_530 & 1) goto ret1;
            if ((sub->field_4EC & 0x100) == 0) goto battle;
        ret1:
            return 1;
        }
    }
battle:
    // Active battle objects (non-empty manager list). Declaration order
    // mirrors the matched func_8027F0B8 loop so MWCC colors node r3 /
    // sentinel r5 like retail.
    CMainMenuBattleMgr* bm = (CMainMenuBattleMgr*)getInstance__Q22cf14CBattleManagerFv();
    CMainMenuBattleNode* node;
    int count;
    CMainMenuBattleNode* sentinel = bm->list;
    count = 0;
    node = sentinel->next;
    while (node != sentinel) {
        node = node->next;
        count++;
    }
    if (count != 0) return 1;
    if (func_80122450()) return 1;
    if (func_80124B78()) return 1;
    if (getUnk80664658()->field_214 & 0x80000) return 1;
    // Presentation-flag / close gates share one return-1 tail (L1BC); the
    // volatile reads defeat MWCC's load CSE and keep both loads hoisted
    // before the masks (retail lwz r0 + lwz r3).
    u32 ev0 = *(volatile u32*)&lbl_eu_80663E24;
    u32 ev1 = *(volatile u32*)&lbl_eu_80663E24;
    if ((ev0 & 0x4) != 0) goto ret2;
    if ((ev1 & 0x400000) != 0) goto ret2;
    if (isAnyFieldFlagSet__Q22cf13CfGameManagerFv() == 0) goto end;
ret2:
    return 1;
end:
    return func_80135898();
}

extern "C" void func_80101BF8(CMainMenu* self) {
    // Menu availability refresh: recompute cursor flags, sub-menu flags and
    // pane positions/colors from the current game state.
    self->field_0xC8[0] = 0;
    self->field_0xC8[1] = 0;
    self->field_0xC8[2] = 0;
    self->field_0xC8[3] = 0;
    self->field_0xC8[4] = 0;
    self->field_0xC8[5] = 0;
    self->field_0xC8[6] = 0;
    self->field_0xC8[7] = 0;

    // hasSave: a save-slot count other than 0xFE always enables the save
    // entry; the 0xFE case consults the shared save flag byte. The boolean is
    // materialized arithmetically (subfic/subi/or/srwi) and both branches
    // feed one store at the merge.
    u32 saveOk = func_8009CF8C(0x270) != 0xFE;
    u32 d0;
    if (saveOk != 0) {
        d0 = 1;
    } else {
        func_80139198(1);
        d0 = (u8)code80135FDC_getByte_64077() < 2;
    }
    self->field_0xD0 = d0;

    self->field_0xD1 = func_8009CF8C(0x337c) == 0 ? 1 : 0;
    self->field_0xD2 = func_8009CF8C(0x3355) == 0 ? 1 : 0;
    self->field_0xD3 = func_8009CF8C(0x3356) == 0 ? 1 : 0;
    self->field_0xD4 = func_8009CF8C(0x337f) == 0 ? 1 : 0;
    self->field_0xD5 = func_8009CF8C(0x337a) == 0 ? 1 : 0;
    self->field_0xD6 = func_8009CF8C(0x3380) == 0 ? 1 : 0;
    self->field_0xD7 = 0;
    self->field_0xD8 = 0;
    self->field_0xD9 = 0;

    // Free-roam gate (same expression shape as func_80100E14).
    int cond;
    if (getUnk80664658() != 0 && (getUnk80664658()->field_214 & 0x8)) {
        cond = 1;
    } else if (lbl_eu_80663E28 & 0x40000000) {
        cond = 1;
    } else {
        CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
        if (spot == 0) goto math;
        if (getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4 == 0) goto math;
        u32 flags = getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4->field_4EC;
        if ((flags & 0x1000000) || (flags & 0x8000) == 0 || (flags & 0x100)) {
            cond = 1;
        } else {
        math:
            // Odd free-roam gate computed from the player-state resource
            // (retail bit-twiddling reproduced verbatim: subi/orc/srwi/subf).
            u32 x = func_8009CF8C(0x20);
            cond = (x >= 0x18Eu);
        }
    }
    self->field_0xDA = cond;

    self->field_0xDB = (lbl_eu_80663E28 >> 30) & 1;
    self->field_0xDC = 0;

    // Main cursor items 0..6: pane name is sprintf'ed, position is bright
    // when available ({0x3c,0x32,0x1e}) else dim ({0xff,0xff,0xff}).
    char buf[0x20];
    for (u32 i = 0; i < 7; i++) {
        sprintf(buf, lbl_eu_804FCEBC + 0x206, (u8)i + 1);
        if (self->field_0xC8[(u8)i] != 0) {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0x1e;
            pos[1] = 0x32;
            pos[0] = 0x3c;
            func_801398A4(self->field_0x7C, buf, pos, 1);
        } else {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0xff;
            pos[1] = 0xff;
            pos[0] = 0xff;
            func_801398A4(self->field_0x7C, buf, pos, 1);
        }
    }

    // Sub-menu items 0..13: fixed pane names copied into a stack table;
    // availability flag bytes live at 0xCF+i (field_0xC8[7+i]).
    const char* names[14];
    *(CMainMenuNameTable*)names = *(CMainMenuNameTable*)lbl_eu_804FCE50;
    for (u32 i = 0; i < 14; i++) {
        if (self->field_0xC8[(u8)i + 7] != 0) {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0x1e;
            pos[1] = 0x32;
            pos[0] = 0x3c;
            func_801398A4(self->field_0x7C, names[(u8)i], pos, 1);
        } else {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0xff;
            pos[1] = 0xff;
            pos[0] = 0xff;
            func_801398A4(self->field_0x7C, names[(u8)i], pos, 1);
        }
    }
}

void OnFileEvent__9CMainMenuFP10CEventFile(void* self) { ((void(*)(void*))__ct__800FF300)((char*)self - 0x58); }

void func_80102008(void* self) { ((void(*)(void*))__dt__9CMainMenuFv)((char*)self - 0x58); }

void func_80102010(void* self) { ((void(*)(void*))cbRenderBefore__9CMainMenuFv)((char*)self - 0x5c); }

extern "C" void func_80102018(void* self) { ((void(*)(void*))__dt__9CMainMenuFv)((char*)self - 0x5c); }


// CTTask<IUICf>::Move - test PTMF at +0x3C, call if non-null
// (static_cast mirrors the canonical CTTask.hpp inline body).
template<>
void CTTask<IUICf>::Move() {
    if (mMoveFunc) {
        (static_cast<IUICf*>(this)->*mMoveFunc)();
    }
}

// CTTask<IUICf>::Draw - test PTMF at +0x48, call if non-null
template<>
void CTTask<IUICf>::Draw() {
    if (mDrawFunc) {
        (static_cast<IUICf*>(this)->*mDrawFunc)();
    }
}

// Standard MWCC virtual dtor: empty body, compiler emits base dtor call + D1 wrapper
template<>
CTTask<IUICf>::~CTTask() {
}


// CMainMenu::Term (us-800ff944): wait for the VI draw to complete, drop the
// render callback (+0x5C cross-cast) and the file handle, run both cursors'
// shutdown virtuals, delete the layout, release the arc accessor, tear down
// the +0x60 memory region, then clear the singleton.
void CMainMenu::Term() {
    CDeviceVI::waitForDrawDone();
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>((char*)this + 0x5C);
    }
    field_0x70->removeRenderCB(render);
    func_801390E0(&field_0x74);
    ((CBaseCurVtIf*)&_90[0])->cleanup();
    ((CBaseCurVtIf*)&subCur)->cleanup();
    if (field_0x7C != 0) {
        delete field_0x7C;
        field_0x7C = 0;
    }
    releaseArcResourceAccessor(field_0x78);
    field_0x78 = 0;
    reinterpret_cast<UnkClass_8045F564*>(&_60[0])->func_8045F778();
    lbl_eu_80663F18 = 0;
}
