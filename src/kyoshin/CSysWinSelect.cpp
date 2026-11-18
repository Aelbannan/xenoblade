// kyoshin/CSysWinSelect - system window "select" process (singleton).

#define IWORK_EVENT_INLINE_DTOR
#define CTTASK_INLINE_DTOR
#define IUIWINDOW_INLINE_DTOR
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CSysWinSelect.hpp"
#include "kyoshin/CSysWin.hpp"

#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/code_80135FDC_State.hpp"


// __vt__13CSysWinSelect, its RTTI base list/record and the "CSysWinSelect"
// type-name string are compiler-emitted.
CSysWinSelect* lbl_eu_80663FDC = nullptr;

// VTable-compatible thunks adjacent to the CSysWinSelect subobjects (retail
// emits these as offset-adjusted dispatch stubs). C-linkage: retail symbols
// are the bare func_ names.
extern "C" void SysWinSelectBaseDtorThunk6C(void* self) { ((void(*)(void*))__dt__13CSystemWindowFv)((char*)self - 0x6c); }
extern "C" void SysWinSelectBaseRenderThunk70(void* self) { ((void(*)(void*))cbRenderBefore__13CSystemWindowFv)((char*)self - 0x70); }
extern "C" void SysWinSelectBaseDtorThunk70(void* self) { ((void(*)(void*))__dt__13CSystemWindowFv)((char*)self - 0x70); }


// CSysWinSelect::~CSysWinSelect (us-80282058-style D1/D0 dtor): the compiler
// destroys mCur18@0xB4 and the embedded CSysWin@0x78, then the CProcess base
// (through the inlined CTTask<IUIWindow>/IUIWindow dtors) and the conditional
// operator delete.
CSysWinSelect::~CSysWinSelect() {}

// CSysWinSelect::CSysWinSelect (us-8012565c)
// CProcess ctor, inlined CTTask<IUIWindow>/IUIWindow state + vptrs, the embedded
// CSysWin / CCur18, the three scalar fields, then the three optional strings.
CSysWinSelect::CSysWinSelect(void* a2, void* a3, void* a4, void* a5)
    : mScene((CScn*)a2), mSysWin(0), mCur18(NULL), mState(0), mCursorSel(1), field_24E(0) {
    if (a3 != 0) {
        strcpy(mStrA, (char*)a3);
    } else {
        mStrA[0] = 0;
    }
    if (a4 != 0) {
        strcpy(mStrB, (char*)a4);
    } else {
        mStrB[0] = 0;
    }
    if (a5 != 0) {
        strcpy(mStrC, (char*)a5);
    } else {
        mStrC[0] = 0;
    }
}

// SysWinSelectCreate (us-80125b4c)
// Allocate the CSysWinSelect singleton (0x250 bytes) from work memory, run its
// constructor, store it in the .sbss global, and register it as a CProcess
// child of `parent`. Returns 0 when the singleton already exists.
extern "C" CSysWinSelect* SysWinSelectCreate(CProcess* parent, void* a2, void* a3, void* a4,
                                        void* a5) {
    if (lbl_eu_80663FDC != 0)
        return 0;

    CSysWinSelect* obj = new (mtl::MemManager::allocate(0x250, CWorkThreadSystem::getWorkMem()))
        CSysWinSelect(a2, a3, a4, a5);
    lbl_eu_80663FDC = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return lbl_eu_80663FDC;
}

// SysWinSelectHandleInput (us-80125bd8)
// The state==2 input handling for the select dialog. Reads the current cf pad
// data; the confirm button flag is read from a different bit in co-op vs
// single-player, and the D-pad up/down bits select the cursor row (wrapping).
// The pad fields are read inside each mode branch - retail emits the loads in
// both arms, so keep them out of shared locals or MWCC hoists them above the
// branch and the function grows an extra saved register.
extern "C" void SysWinSelectHandleInput(CSysWinSelect* self) {
    CSysPadData* pad = (CSysPadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int confirm, up, down;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
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
        self->mSysWin.advancePhase3();
        self->mField60 = (s8)self->mCursorSel + 1;
        self->mCur18.SetVisible(0);
        playUISound(3);
    } else if (up != 0) {
        // Up: move the cursor up one row, wrapping below 0 to 1.
        self->mCursorSel = (u8)(self->mCursorSel - 1);
        if ((s8)self->mCursorSel < 0) self->mCursorSel = 1;
        nw4r::math::VEC3 tmp;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&tmp), &self->mSysWin, self->mCursorSel);
        self->mCur18.setRootPaneTranslate(&tmp);
        playUISound(1);
    } else if (down != 0) {
        // Down: move the cursor down one row, wrapping over 1 to 0.
        self->mCursorSel = (u8)(self->mCursorSel + 1);
        if ((s8)self->mCursorSel > 1) self->mCursorSel = 0;
        nw4r::math::VEC3 tmp;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&tmp), &self->mSysWin, self->mCursorSel);
        self->mCur18.setRootPaneTranslate(&tmp);
        playUISound(1);
    }
}

