// kyoshin/CSysWinSelect - system window "select" process (singleton).

#include "kyoshin/CSysWinSelect.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <string.h>
#include <revolution/gx/GXPixel.h>

// Forward decls used by the catalog thunks below (retail sibling symbols).
// __dt__13CSysWinSelectFv / cbRenderBefore__13CSysWinSelectFv are declared in
// CSysWinSelect.hpp; the CSystemWindow ones are imported here for the thunks.
void __dt__13CSystemWindowFv(void* self);
void cbRenderBefore__13CSystemWindowFv(void* self);
// Global-view declaration so the catalog thunks can address the member method
// by its retail symbol name (the definition below emits that symbol).
void cbRenderBefore__13CSysWinSelectFv(void* self);

// VTable-compatible thunks adjacent to the CSysWinSelect subobjects (retail
// emits these as offset-adjusted dispatch stubs). Preserved from the scaffold.
void func_801252A0(void* self) { ((void(*)(void*))__dt__13CSystemWindowFv)((char*)self - 0x6c); }

void func_801252A8(void* self) { ((void(*)(void*))cbRenderBefore__13CSystemWindowFv)((char*)self - 0x70); }

void func_801252B0(void* self) { ((void(*)(void*))__dt__13CSystemWindowFv)((char*)self - 0x70); }

void func_801252B8(void* self) { ((void(*)(void*))__dt__13CSysWinSelectFv)((char*)self - 0x6c); }

void func_801252C0(void* self) { ((void(*)(void*))cbRenderBefore__13CSysWinSelectFv)((char*)self - 0x70); }

void func_801252C8(void* self) { ((void(*)(void*))__dt__13CSysWinSelectFv)((char*)self - 0x70); }

void func_801250FC(CSysWinSelect* self);  // forward decl (defined below)

void CSysWinSelect::Init() {
    // Register the IScnRender subobject (+0x70) with the owning scene. The
    // `if (this)` is the MWCC idiom that splits `mr r4,r31 / beq / addi r4,+0x70`.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->addRenderCB(render, 0xd, 0);

    // CSysWin virtual: load the layout (vtable slot 0x22 = +0x88).
    reinterpret_cast<CSysWinView*>(&mSysWin[0])->v20();

    // Rebuild the embedded CCur18 cursor: construct on stack with the shared
    // arc-resource accessor, copy the non-vtable fields into +0xB4 (the vtable
    // pointer there is retained from construction), then destroy the temp.
    u8 temp[0x18];
    __ct__CCur18(temp, (void*)func_801355F4());
    *(u32*)(mCur18 + 4) = *(u32*)(temp + 4);
    *(u32*)(mCur18 + 8) = *(u32*)(temp + 8);
    *(u32*)(mCur18 + 0xC) = *(u32*)(temp + 0xC);
    *(u32*)(mCur18 + 0x10) = *(u32*)(temp + 0x10);
    mCur18[0x14] = temp[0x14];
    mCur18[0x15] = temp[0x15];
    __dt__6CCur18Fv(temp, -1);

    // CCur18 cursor virtual (vtable slot 2 = +0x08).
    reinterpret_cast<CCur18View*>(&mCur18[0])->vf02();
}

void CSysWinSelect::Move() {
    // Bail if the task is busy or the global render flag is set.
    if (CTaskGame::getInstance()->func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;

    switch (mState) {
    case 0:
        // Window just became ready: load strings, start the animation.
        if (CSysWin_isReady(&mSysWin[0])) {
            mState = 1;
            func_8022B90C(&mSysWin[0], 2);
            func_8022B9B4(&mSysWin[0], &mStrA[0], 0);
            func_8022BF6C(&mSysWin[0], &mStrB[0], &mStrC[0]);
            func_8022BFC8(&mSysWin[0], 0);
            func_8022B8B8(&mSysWin[0]);
        }
        break;
    case 1:
        // Window active: snapshot the cursor index and update the cursor.
        if (CSysWin_isActive(&mSysWin[0])) {
            mState = 2;
            u8 tmp[0x18];
            func_8022C1B4(tmp, &mSysWin[0], mCursorSel);
            reinterpret_cast<CCur18View*>(&mCur18[0])->vf04(tmp);
            func_801D216C(&mCur18[0], 1);
        }
        break;
    case 2:
        func_801250FC(this);
        break;
    case 3:
        // Closing transition finished - hand control back to the UI.
        if (CSysWin_isActive(&mSysWin[0])) {
            field_67 = 0;
            field_64 = 1;
        }
        break;
    }

    func_8022B748(&mSysWin[0]);
    func_801D202C(&mCur18[0]);
}

// ---------------------------------------------------------------------------
// Target 3: CSysWinSelect::~CSysWinSelect (us-801257ac)
// Complete-object destructor. Subobjects destroyed in reverse construction
// order: CCur18@0xB4, then CSysWin@0x78, then the CProcess base. The nested
// double null-check guards the base-class dtor (MWCC D2-inlined-into-D1
// artifact), then a conditional operator delete when flags > 0.
// ---------------------------------------------------------------------------
extern "C" CSysWinSelect* __dt__13CSysWinSelectFv(CSysWinSelect* _this,
                                                   int flags) {
    if (_this != 0) {
        __dt__6CCur18Fv(&_this->mCur18[0], -1);
        __dt__7CSysWinFv(&_this->mSysWin[0], -1);
        if (_this != 0) {
            if (_this != 0) {
                __dt__8CProcessFv(_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// ---------------------------------------------------------------------------
// Target 2: CSysWinSelect::Term (us-801258e8)
// Wait for draw-done, detach the render callback from the owning scene, release
// the embedded CSysWin, run the CCur18 cursor update virtual, clear the
// singleton global. The `if (this)` prepending on the render-cb arg is the MWCC
// idiom that reproduces the retail `mr r4,r31 / beq / addi r4,r31,0x70` split.
// ---------------------------------------------------------------------------
void CSysWinSelect::Term() {
    CDeviceVI::waitForDrawDone();

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->removeRenderCB(render);

    func_8022B7F4(&mSysWin[0]);
    reinterpret_cast<CCursor18*>(&mCur18[0])->vf3();
    lbl_eu_80663FDC = 0;
}

// ---------------------------------------------------------------------------
// Target 4: func_80125070 (us-80125b4c)
// Allocate the CSysWinSelect singleton (0x250 bytes) from work memory, run its
// constructor, store it in the .sbss global, and register it as a CProcess
// child of `parent`. Returns 0 when the singleton already exists. Regist is
// called even if the allocation/ctor failed, matching retail.
// ---------------------------------------------------------------------------
CSysWinSelect* func_80125070(CProcess* parent, void* a2, void* a3, void* a4,
                             void* a5) {
    if (lbl_eu_80663FDC != 0)
        return 0;

    u32 workMem = CWorkThreadSystem::getWorkMem();
    CSysWinSelect* obj =
        (CSysWinSelect*)mtl::MemManager::allocate(0x250, workMem);
    if (obj != 0) {
        obj = __ct__CSysWinSelect(obj, a2, a3, a4, a5);
    }
    lbl_eu_80663FDC = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return lbl_eu_80663FDC;
}

// ---------------------------------------------------------------------------
// Target 4: func_801250FC (us-80125bd8)
// The state==2 input handling for the select dialog. Reads the current cf pad
// data; the confirm button flag is read from a different bit in co-op vs
// single-player, and the D-pad up/down bits select the cursor row (wrapping).
// ---------------------------------------------------------------------------
void func_801250FC(CSysWinSelect* self) {
    CSysPadData* pad = (CSysPadData*)getCfPadData__Q22cf13CfGameManagerFv();
    u32 btnFlags = pad->field_04;
    u32 dueFlags = pad->field_104;
    u32 confirm, up, down;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        confirm = (btnFlags >> 21) & 1;
        up = (dueFlags & 0x8004) != 0;
        down = (dueFlags & 0x10008) != 0;
    } else {
        confirm = (btnFlags >> 4) & 1;
        up = (dueFlags & 0x8004) != 0;
        down = (dueFlags & 0x10008) != 0;
    }

    if (confirm != 0) {
        // Confirm: close the window and record the chosen row index.
        self->mState = 3;
        func_8022B8E4(&self->mSysWin[0]);
        self->field_60 = (s8)self->mCursorSel + 1;
        func_801D216C(&self->mCur18[0], 0);
        func_80138078__FUl(3);
    } else if (up != 0) {
        // Up: move the cursor up one row, wrapping below 0 to 1.
        self->mCursorSel = (u8)(self->mCursorSel - 1);
        if ((s8)self->mCursorSel < 0) self->mCursorSel = 1;
        u8 tmp[0x18];
        func_8022C1B4(tmp, &self->mSysWin[0], self->mCursorSel);
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(tmp);
        func_80138078__FUl(1);
    } else if (down != 0) {
        // Down: move the cursor down one row, wrapping over 1 to 0.
        self->mCursorSel = (u8)(self->mCursorSel + 1);
        if ((s8)self->mCursorSel > 1) self->mCursorSel = 0;
        u8 tmp[0x18];
        func_8022C1B4(tmp, &self->mSysWin[0], self->mCursorSel);
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(tmp);
        func_80138078__FUl(1);
    }
}

// ---------------------------------------------------------------------------
// Target 1: nw4r::ut::Color dtor (us-80125dac)
// Scalar-deleting destructor with the flag > 0 delete gate. Emitted into this
// TU by MWCC for a deleted nw4r::ut::Color subobject.
// ---------------------------------------------------------------------------
void* __dt__Q34nw4r2ut5ColorFv(void* self, int flags) {
    if (self != 0 && flags > 0) {
        operator delete(self);
    }
    return self;
}

// ---------------------------------------------------------------------------
// Target 5: CSysWinSelect::cbRenderBefore (us-80125ab4)
// Gate prefix (CTaskGame busy / global bit21 / scene-active) then draw the
// embedded CSysWin and CCur18 cursor through a stack DrawInfo.
// ---------------------------------------------------------------------------
void CSysWinSelect::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() != 0) {
        return;
    }
    if (lbl_eu_80663E28 & 0x200000) {
        return;
    }
    if (!func_8013BE50()) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250(&drawInfo);
    func_8022B7C8(&mSysWin[0], &drawInfo);
    func_801D20B0(&mCur18[0], &drawInfo);
    drawInfo.~DrawInfo();
}

// Constructor (not a matching target - retail emits the ctor with an unmangled
// C-linkage name). Defined at the bottom of the file so MWCC cannot inline it
// into func_80125070 (its body is not visible at that call site) - the retail
// creator emits a real `bl __ct__CSysWinSelect`. This scaffolding body just
// initializes the CProcess base and clears the Move region so the object is
// usable; only its symbol matters for linking.
extern "C" CSysWinSelect* __ct__CSysWinSelect(CSysWinSelect* self, void* a2,
                                               void* a3, void* a4, void* a5) {
    CScn* scene = (CScn*)a2;
    const char* str1 = (const char*)a3;
    const char* str2 = (const char*)a4;
    const char* str3 = (const char*)a5;
    __ct__8CProcessFv((CProcess*)self);

    // IUIWindow vtable at +0x10 is written twice (temp vtable, then the
    // CSysWinSelect composite vtable), the two __ptmf_null callback slots are
    // copied, then the scalar fields and the embedded CSysWin/CCur18 are
    // constructed and the three text strings copied.
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052D238;
    self->ptmf0[0] = __ptmf_null[0];
    self->ptmf0[1] = __ptmf_null[1];
    self->ptmf0[2] = __ptmf_null[2];
    self->ptmf1[0] = __ptmf_null[0];
    self->ptmf1[1] = __ptmf_null[1];
    self->ptmf1[2] = __ptmf_null[2];

    self->field_54 = 0;
    self->field_58 = 0;
    self->field_5C = 0;
    self->field_60 = -1;
    self->field_64 = 0;
    self->field_65 = 0;
    self->field_66 = 0;
    self->field_67 = 1;
    self->field_68 = 0;

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052D278;
    self->mWorkEvent = (u32)lbl_eu_8052D278 + 0x24;
    self->mScnRender = (u32)lbl_eu_8052D278 + 0xac;
    self->mScene = scene;

    __ct__CSysWin(&self->mSysWin[0], 0);
    __ct__CCur18(&self->mCur18[0], 0);

    self->mState = 0;
    self->mCursorSel = 1;
    self->field_24E = 0;
    if (str1)
        strcpy(&self->mStrA[0], str1);
    else
        self->mStrA[0] = 0;
    if (str2)
        strcpy(&self->mStrB[0], str2);
    else
        self->mStrB[0] = 0;
    if (str3)
        strcpy(&self->mStrC[0], str3);
    else
        self->mStrC[0] = 0;

    return self;
}