// kyoshin/CSysWinSelect - system window "select" process (singleton).

#include "kyoshin/CSysWinSelect.hpp"

#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

extern "C" void playUISound__FUl(u32 op);

extern char lbl_eu_804FEF98[];
extern char lbl_eu_8052D338[];
__declspec(section ".rodata") __attribute__((used, aligned(8))) char lbl_eu_804FEF98[0x10] = {0x43,0x53,0x79,0x73,0x57,0x69,0x6e,0x53,0x65,0x6c,0x65,0x63,0x74,0x00,0x00,0x00};
__declspec(section ".data") __attribute__((used, aligned(8))) char lbl_eu_8052D278[0xc0] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x94,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xff,0xff,0xff,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
__declspec(section ".data") __attribute__((used, aligned(8))) char lbl_eu_8052D338[0x40] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6c,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
__declspec(section ".sdata") __attribute__((used, aligned(8))) const char* const lbl_eu_80661ED8[2] = { lbl_eu_804FEF98, lbl_eu_8052D338 };
CSysWinSelect* lbl_eu_80663FDC = nullptr;

// VTable-compatible thunks adjacent to the CSysWinSelect subobjects (retail
// emits these as offset-adjusted dispatch stubs). C-linkage: retail symbols
// are the bare func_ names.
extern "C" void SysWinSelectBaseDtorThunk6C(void* self) { ((void(*)(void*))__dt__13CSystemWindowFv)((char*)self - 0x6c); }
extern "C" void SysWinSelectBaseRenderThunk70(void* self) { ((void(*)(void*))cbRenderBefore__13CSystemWindowFv)((char*)self - 0x70); }
extern "C" void SysWinSelectBaseDtorThunk70(void* self) { ((void(*)(void*))__dt__13CSystemWindowFv)((char*)self - 0x70); }
extern "C" void SysWinSelectDtorThunk6C(void* self) { ((void(*)(void*))__dt__13CSysWinSelectFv)((char*)self - 0x6c); }
extern "C" void SysWinSelectRenderThunk70(void* self) { ((void(*)(void*))cbRenderBefore__13CSysWinSelectFv)((char*)self - 0x70); }
extern "C" void SysWinSelectDtorThunk70(void* self) { ((void(*)(void*))__dt__13CSysWinSelectFv)((char*)self - 0x70); }

// SysWinSelectCreate (us-80125b4c)
// Allocate the CSysWinSelect singleton (0x250 bytes) from work memory, run its
// constructor, store it in the .sbss global, and register it as a CProcess
// child of `parent`. Returns 0 when the singleton already exists.
extern "C" CSysWinSelect* SysWinSelectCreate(CProcess* parent, void* a2, void* a3, void* a4,
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
        sysWinAdvancePhase3(&self->mSysWin[0]);
        self->field_60 = (s8)self->mCursorSel + 1;
        Cur_SetVisible(&self->mCur18[0], 0);
        playUISound__FUl(3);
    } else if (up != 0) {
        // Up: move the cursor up one row, wrapping below 0 to 1.
        self->mCursorSel = (u8)(self->mCursorSel - 1);
        if ((s8)self->mCursorSel < 0) self->mCursorSel = 1;
        u8 tmp[0xC];
        sysWinGetPaneScreenPos(tmp, &self->mSysWin[0], self->mCursorSel);
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(tmp);
        playUISound__FUl(1);
    } else if (down != 0) {
        // Down: move the cursor down one row, wrapping over 1 to 0.
        self->mCursorSel = (u8)(self->mCursorSel + 1);
        if ((s8)self->mCursorSel > 1) self->mCursorSel = 0;
        u8 tmp[0xC];
        sysWinGetPaneScreenPos(tmp, &self->mSysWin[0], self->mCursorSel);
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(tmp);
        playUISound__FUl(1);
    }
}
