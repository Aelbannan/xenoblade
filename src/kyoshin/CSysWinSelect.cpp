// kyoshin/CSysWinSelect - system window "select" process (singleton).

#include "kyoshin/CSysWinSelect.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <string.h>
#include <revolution/gx/GXPixel.h>

// VTable-compatible thunks adjacent to the CSysWinSelect subobjects (retail
// emits these as offset-adjusted dispatch stubs). Preserved from the scaffold.
void func_801252A0(void* self) { ((void(*)(void*))__dt__13CSystemWindowFv)((char*)self - 0x6c); }

void func_801252A8(void* self) { ((void(*)(void*))cbRenderBefore__13CSystemWindowFv)((char*)self - 0x70); }

void func_801252B0(void* self) { ((void(*)(void*))__dt__13CSystemWindowFv)((char*)self - 0x70); }

void func_801252B8(void* self) { ((void(*)(void*))__dt__13CSysWinSelectFv)((char*)self - 0x6c); }

void func_801252C0(void* self) { ((void(*)(void*))cbRenderBefore__13CSysWinSelectFv)((char*)self - 0x70); }

void func_801252C8(void* self) { ((void(*)(void*))__dt__13CSysWinSelectFv)((char*)self - 0x70); }

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
    if (CTaskGame::getInstance()->isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
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
            u8 tmp[0xC];
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
// CSysWinSelect::~CSysWinSelect (us-801257ac)
// Complete-object destructor. Subobjects destroyed in reverse construction
// order: CCur18@0xB4, then CSysWin@0x78, then the CProcess base. Written as a
// C-linkage definition carrying the retail mangled-member symbol so the flags
// parameter and MWCC's D2-inlined-into-D1 artifacts (nested double null-check
// around the base dtor) can be expressed exactly; a real member definition
// makes MWCC emit an extra vtable store and reorders the subobject calls.
// The member declaration in CSysWinSelect.hpp is satisfied at link time
// (CMapSel idiom).
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
// CSysWinSelect::Term (us-801258e8)
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
// func_80125070 (us-80125b4c)
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
// func_801250FC (us-80125bd8)
// The state==2 input handling for the select dialog. Reads the current cf pad
// data; the confirm button flag is read from a different bit in co-op vs
// single-player, and the D-pad up/down bits select the cursor row (wrapping).
// C-linkage (retail symbol is unmangled func_801250FC). The pad fields are
// read inside each mode branch - retail emits the loads in both arms, so keep
// them out of shared locals or MWCC hoists them above the branch and the
// function grows an extra saved register.
// ---------------------------------------------------------------------------
extern "C" void func_801250FC(CSysWinSelect* self) {
    CSysPadData* pad = (CSysPadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int confirm, up, down;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        // Co-op: D-pad rows read from the turbo-press flags, confirm from bit 21.
        up = (pad->field_104 & 0x8004) != 0;
        down = (pad->field_104 & 0x10008) != 0;
        confirm = (pad->field_04 >> 21) & 1;
    } else {
        // Single-player: confirm is bit 4.
        up = (pad->field_104 & 0x8004) != 0;
        down = (pad->field_104 & 0x10008) != 0;
        confirm = (pad->field_04 >> 4) & 1;
    }

    if (confirm != 0) {
        // Confirm: close the window and record the chosen row index.
        self->mState = 3;
        func_8022B8E4(&self->mSysWin[0]);
        self->field_60 = (s8)self->mCursorSel + 1;
        func_801D216C(&self->mCur18[0], 0);
        playUISound(3);
    } else if (up != 0) {
        // Up: move the cursor up one row, wrapping below 0 to 1.
        self->mCursorSel = (u8)(self->mCursorSel - 1);
        if ((s8)self->mCursorSel < 0) self->mCursorSel = 1;
        u8 tmp[0xC];
        func_8022C1B4(tmp, &self->mSysWin[0], self->mCursorSel);
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(tmp);
        playUISound(1);
    } else if (down != 0) {
        // Down: move the cursor down one row, wrapping over 1 to 0.
        self->mCursorSel = (u8)(self->mCursorSel + 1);
        if ((s8)self->mCursorSel > 1) self->mCursorSel = 0;
        u8 tmp[0xC];
        func_8022C1B4(tmp, &self->mSysWin[0], self->mCursorSel);
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(tmp);
        playUISound(1);
    }
}

// ---------------------------------------------------------------------------
// nw4r::ut::Color dtor (us-80125dac)
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
// CSysWinSelect::cbRenderBefore (us-80125ab4)
// Gate prefix (CTaskGame busy / global bit21 / scene-active) then draw the
// embedded CSysWin and CCur18 cursor through a stack DrawInfo.
// ---------------------------------------------------------------------------
void CSysWinSelect::cbRenderBefore() {
    CTaskGame::getInstance();
    // Gate: when the task game is not ready or the global mode bit (0x200000)
    // is set, skip the render. The if-&&-goto body / goto end / end: return
    // chain (exit label before the body label) keeps the body off the
    // fallthrough so MWCC emits retail's branch-over-branch: `bne end` for
    // the first disjunct, `beq body; b end` for the second (same scheme as
    // CPartyStateWin::cbRenderBefore).
    if (CTaskGame::isFlag01Set() == 0 && (lbl_eu_80663E28 & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    if (func_8013BE50() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // to match the retail direct calls (a C++ local would virtual-dispatch
    // its scope-exit destructor and bloat the body).
    u8 drawInfo[0x60];
    __ct__Q34nw4r3lyt8DrawInfoFv(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
    func_80137250(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
    func_8022B7C8(&mSysWin[0], reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
    func_801D20B0(&mCur18[0], reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
    __dt__Q34nw4r3lyt8DrawInfoFv(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]), -1);
}

// Constructor (retail emits it under an unmangled global symbol name, hence
// the C-linkage definition; declared in CSysWinSelect.hpp).
// Defined at the bottom of the file so MWCC cannot inline it into
// func_80125070 - the retail creator emits a real `bl __ct__CSysWinSelect`.
extern "C" CSysWinSelect* __ct__CSysWinSelect(CSysWinSelect* self, void* scene,
                                               void* strA, void* strB,
                                               void* strC) {
    // Base CProcess subobject.
    __ct__8CProcessFv((CProcess*)self);

    // Temporarily install the plain IUIWindow vtable at +0x10.
    reinterpret_cast<CSysWinSelectHeaderView*>(self)->vtable = (u32)lbl_eu_8052D238;

    // Materialize the composite vtable address + offset values before the
    // callback copy so MWCC schedules their lis/addi ahead of the __ptmf_null
    // base (retail interleaves the final-vtable lis between the temp-vtable
    // store and the ptmf lwzu).
    char* finalVt = lbl_eu_8052D278;
    u32 evtVt = (u32)(finalVt + 0x24);
    u32 rndVt = (u32)(finalVt + 0xac);

    // Copy the null member-function pointer into both callback slots (retail
    // store order 0x40,0x3C,0x44 then 0x4C,0x48,0x50). Post-increment derefs
    // of a local pointer make MWCC fold the first access into `lwzu` instead
    // of an extra `addi`-materialised pointer - cf. CTaskGameEvt / MWCC_CASES
    // btm_sco_init lwzu shape.
    const u32* src = __ptmf_null;
    u32 ptmfWord0 = *src++;
    u32 ptmfWord1 = *src++;
    self->ptmf0[1] = ptmfWord1;
    self->ptmf0[0] = ptmfWord0;
    u32 ptmfWord2 = *src++;
    self->ptmf0[2] = ptmfWord2;
    src = __ptmf_null;
    ptmfWord1 = *src++;
    ptmfWord0 = *src++;
    self->ptmf1[1] = ptmfWord0;
    self->ptmf1[0] = ptmfWord1;
    ptmfWord2 = *src++;
    self->ptmf1[2] = ptmfWord2;

    self->field_54 = 0;
    self->field_58 = 0;
    self->field_5C = 0;
    self->field_60 = -1;
    self->field_64 = 0;
    self->field_65 = 0;
    self->field_66 = 0;
    self->field_67 = 1;
    self->field_68 = 0;

    // Overwrite +0x10 with the composite CSysWinSelect vtable.
    reinterpret_cast<CSysWinSelectHeaderView*>(self)->vtable = (u32)finalVt;
    self->mWorkEvent = evtVt;
    self->mScnRender = rndVt;
    self->mScene = (CScn*)scene;

    // Embedded window and cursor subobjects.
    __ct__CSysWin(&self->mSysWin[0], 0);
    __ct__CCur18(&self->mCur18[0], 0);

    self->mState = 0;
    self->mCursorSel = 1;
    self->field_24E = 0;

    // Copy each optional string, or terminate it when absent.
    if (strA != NULL)
        strcpy(&self->mStrA[0], (const char*)strA);
    else
        self->mStrA[0] = '\0';
    if (strB != NULL)
        strcpy(&self->mStrB[0], (const char*)strB);
    else
        self->mStrB[0] = '\0';
    if (strC != NULL)
        strcpy(&self->mStrC[0], (const char*)strC);
    else
        self->mStrC[0] = '\0';

    return self;
}