// kyoshin/CSysWinBuff - system window "buff" process (singleton).

#include "kyoshin/CSysWinBuff.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <new>
#include <revolution/gx/GXPixel.h>

// Global-view declarations so the catalog thunks can address the retail-named
// functions by their verbatim symbols (the definitions below emit them).
extern "C" bool func_80274A84(CSysWinBuff* self, CEventFile* pEventFile);
extern "C" void* __dt__11CSysWinBuffFv(CSysWinBuff* self, int flags);
extern "C" void cbRenderBefore__11CSysWinBuffFv(void* self);

// ---------------------------------------------------------------------------
// VTable-compatible thunks adjacent to the CSysWinBuff subobjects (retail
// emits these as offset-adjusted dispatch stubs: only r3 is re-based, then a
// tail jump into the right subobject trampoline). Preserved from the scaffold.
// ---------------------------------------------------------------------------
void OnFileEvent__11CSysWinBuffFP10CEventFile(void* self) { ((void(*)(void*))func_80274A84)((char*)self - 0x6c); }

void func_80274B08(void* self) { ((void(*)(void*))__dt__11CSysWinBuffFv)((char*)self - 0x6c); }

void func_80274B10(void* self) { ((void(*)(void*))cbRenderBefore__11CSysWinBuffFv)((char*)self - 0x70); }

extern "C" void func_80274B18(void* self) { ((void(*)(void*))__dt__11CSysWinBuffFv)((char*)self - 0x70); }

extern "C" void func_80274B20() {}

extern "C" void func_80274B24() {}

// ---------------------------------------------------------------------------
// CSysWinBuff::~CSysWinBuff (us-80276958)
// Complete-object destructor. The embedded CSysWin is destroyed (flags -1),
// then the CProcess base guarded by the nested double null-check (an MWCC
// D2-inlined-into-D1 artifact), then conditional operator delete when the
// deleting flag is set. Same shape as CSysWinSave's dtor.
// ---------------------------------------------------------------------------
extern "C" void* __dt__11CSysWinBuffFv(CSysWinBuff* _this, int flags) {
    if (_this != 0) {
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
// func_80274A84 (us-80276f08) - IWorkEvent::OnFileEvent body.
// When the loaded arc matches mFileHandle: hand its data buffer to CBdat
// (releases the archive), re-store the loaded file-pointer (getFP) in the
// global, clear the handle and return true; otherwise false.
// ---------------------------------------------------------------------------
extern "C" bool func_80274A84(CSysWinBuff* self, CEventFile* pEventFile) {
    if (self->mFileHandle == pEventFile->mFileHandle) {
        void* data = self->mFileHandle->getData();
        setBdatEntry__5CBdatFUlPv(2, data);
        func_8003AA34();
        lbl_eu_806648E4 = (u32)getFP__FPCc(&lbl_eu_8050EBC4[0x22]);
        self->mFileHandle = 0;
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// CSysWinBuff::Term (us-80276b08)
// Wait for the VI, release the arc file handle and tear down the embedded
// CSysWin, release the BDAT archive, clear the two globals, deregister the
// render callback (this-adjusting IScnRender view at +0x70), then re-enable
// the cf load once if the boot-time byte flag is still clear.
// ---------------------------------------------------------------------------
void CSysWinBuff::Term() {
    CDeviceVI::waitForDrawDone();
    func_801390E0(&mFileHandle);
    func_8022B7F4(&mSysWin[0]);
    getEntry__5CBdatFUl(2);
    lbl_eu_806648E4 = 0;
    lbl_eu_806648E0 = 0;
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->removeRenderCB(render);
    func_8013B980();
    if (code80135FDC_getByte_64080() == 0)
        setPresentationFlag__Q22cf13CfGameManagerFv(false);
}

// ---------------------------------------------------------------------------
// CSysWinBuff::cbRenderBefore (us-80276d34)
// Gate prefix (task busy / global render flag / scene active / cf busy) then
// draw the embedded CSysWin through a stack DrawInfo.
// ---------------------------------------------------------------------------
void CSysWinBuff::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (!func_8013BE50()) return;
    if (func_8029A658() != 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250(&drawInfo);
    func_8022B7C8(&mSysWin[0], &drawInfo);
    // Scope-exit dtor is auto-emitted as the retail direct
    // `bl __dt__Q34nw4r3lyt8DrawInfoFv(drawInfo, -1)`; an explicit
    // `drawInfo.~DrawInfo()` would ALSO emit a virtual-dispatched call.
}

// ---------------------------------------------------------------------------
// create__11CSysWinBuffFv (us-80276dcc)
// Allocate the CSysWinBuff singleton (0xBC bytes) from work memory, run the
// CProcess ctor, lay out the IUIWindow + Move region by hand, construct the
// embedded CSysWin and register as a CProcess child of `registerParent`.
// Returns 0 when the singleton already exists; Regist is called even when the
// allocation failed (r31 stays NULL), matching retail.
// ---------------------------------------------------------------------------
extern "C" CSysWinBuff* create__11CSysWinBuffFv(CProcess* registerParent, CScn* scene, u16 arg) {
    if (lbl_eu_806648E0 != 0)
        return 0;

    CSysWinBuffCtorShim* obj = (CSysWinBuffCtorShim*)mtl::MemManager::allocate(
        0xbc, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        __ct__8CProcessFv((CProcess*)obj);

        // IUIWindow vtable at +0x10 is written twice (temp vtable, then the
        // CSysWinBuff composite vtable); the two null ptmf callback slots are
        // copied, then the scalar fields are initialised.
        obj->vtable = (void*)lbl_eu_8052D238;

        // Copy the null member-function pointer into both callback slots
        // (retail loads __ptmf_null[1],[0],[2] then stores per slot, so use
        // intermediate locals to force the retail ordering). The uintptr_t
        // round-trip keeps MWCC from rematerializing the array address per
        // slot (same idiom as CSysWinSave).
        const u32* ptmf = __ptmf_null;
        u32 ptmfWord1 = ptmf[1];
        u32 ptmfWord0 = ptmf[0];
        obj->callbacks[0] = ptmfWord0;
        obj->callbacks[1] = ptmfWord1;
        u32 ptmfWord2 = ptmf[2];
        obj->callbacks[2] = ptmfWord2;
        ptmfWord1 = ptmf[1];
        ptmfWord0 = ptmf[0];        obj->callbacks[3] = ptmfWord0;
        obj->callbacks[4] = ptmfWord1;
        ptmfWord2 = ptmf[2];
        obj->callbacks[5] = ptmfWord2;

        obj->field54 = 0;
        obj->field58 = 0;
        obj->field5C = 0;
        obj->index = -1;
        obj->flag64 = 0;
        obj->flag65 = 0;
        obj->flag66 = 0;
        obj->active = 1;
        obj->field68 = 0;

        obj->vtable = (void*)lbl_eu_80537DC0;
        obj->field6C = (u32)(lbl_eu_80537DC0 + 0x24);
        obj->field70 = (u32)(lbl_eu_80537DC0 + 0xac);
        obj->fileHandle = 0;
        obj->scene = scene;

        __ct__CSysWin(&obj->mSysWin[0], 0);

        obj->flagB8 = 0;
        obj->argBA = arg;
        setPresentationFlag__Q22cf13CfGameManagerFv(true);
        code80135FDC_postIncByte_64080();
    }
    lbl_eu_806648E0 = (CSysWinBuff*)obj;
    ((CProcess*)obj)->Regist(registerParent, false);
    return lbl_eu_806648E0;
}

// ---------------------------------------------------------------------------
// CSysWinBuff::Init (us-802769cc)
// Register the render callback through the IScnRender subobject view at +0x70,
// request the common-archive System.arc through the IWorkEvent view at +0x6C,
// then rebuild the embedded CSysWin storage: construct a stack temp, copy its
// body fields (the vtable word at +0 and the 0x29-0x2B pad are retained),
// destroy the temp, and dispatch the layout-build virtual at vtable+0x88.
// ---------------------------------------------------------------------------
void CSysWinBuff::Init() {
    // IScnRender subobject at +0x70; keep the conditional this-adjust
    // (null this -> null render) as mr/beq/addi like the sibling units.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->addRenderCB(render, 0xd, 0);

    // IWorkEvent subobject at +0x6C, same conditional this-adjust.
    IWorkEvent* workEvent = reinterpret_cast<IWorkEvent*>(this);
    if (this) workEvent = reinterpret_cast<IWorkEvent*>(&mWorkEvent);
    mFileHandle = (CFileHandle*)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        func_800A9D90(), &lbl_eu_8050EBC4[0], workEvent, 0, 0);

    // Rebuild the embedded CSysWin from a fresh temp (ctor arg 0), copying
    // every body field except the vtable word and the 0x29-0x2B padding.
    u8 tempW[0x3C];
    __ct__CSysWin(tempW, 0);
    CSysWinDataBuff* sw = reinterpret_cast<CSysWinDataBuff*>(&mSysWin[0]);
    CSysWinDataBuff* tw = reinterpret_cast<CSysWinDataBuff*>(tempW);
    sw->f_04 = tw->f_04;
    sw->f_08 = tw->f_08;
    sw->f_0c = tw->f_0c;
    sw->f_10 = tw->f_10;
    sw->f_14 = tw->f_14;
    sw->f_18 = tw->f_18;
    sw->f_1c = tw->f_1c;
    sw->f_20 = tw->f_20;
    sw->f_24 = tw->f_24;
    sw->f_28 = tw->f_28;
    sw->f_2c = tw->f_2c;
    sw->f_30 = tw->f_30;
    sw->f_34 = tw->f_34;
    sw->f_35 = tw->f_35;
    sw->f_36 = tw->f_36;
    sw->f_37 = tw->f_37;
    sw->f_38 = tw->f_38;
    sw->f_39 = tw->f_39;
    __dt__7CSysWinFv(tempW, -1);

    // Dispatch the layout-build virtual at vtable+0x88 on the rebuilt CSysWin.
    reinterpret_cast<CSysWinView*>(&mSysWin[0])->v20();
}

// ---------------------------------------------------------------------------
// CSysWinBuff::Move (us-80276b88)
// Gate prefix (task busy / global render flag / scene active / cf busy / boot
// flag) then a 4-state machine driving the system window:
//   0 load the window label and ready the window; 1 wait active;
//   2 dispatch the confirm button; 3 closing transition hands control back.
// ---------------------------------------------------------------------------
void CSysWinBuff::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;
    if (func_8029A658() != 0) return;
    if (lbl_eu_80663E24 & 0x04000000) {
        field_64 = 1;
        return;
    }

    switch (mFlagB8) {
    case 0: {
        // Window just became ready: resolve the label message and start the
        // open animation. lbl_eu_806648E4 gates the arc-loaded state.
        if (CSysWin_isReady(&mSysWin[0]) != 0 && lbl_eu_806648E4 != 0) {
            mFlagB8 = 1;
            u32 msgId = func_80136254((void*)lbl_eu_806640E0,
                                      &lbl_eu_8050EBC4[0x1d], mArgBA);
            const char* label = func_8013639C((void*)lbl_eu_806648E4,
                                              &lbl_eu_8050EBC4[0x1d], (u16)msgId);
            func_8022B9B4(&mSysWin[0], (void*)label, 0);
            func_8022BFC8(reinterpret_cast<CSysWin*>(&mSysWin[0]), 1);
            func_8022B8B8(&mSysWin[0]);
        }
        break;
    }
    case 1:
        // Wait for the system window to become active.
        if (CSysWin_isActive(&mSysWin[0])) {
            mFlagB8 = 2;
        }
        break;
    case 2: {
        // Confirm button: player-specific (Classic A) in co-op, else Wiimote A.
        CPad* pad = cf::CfGameManager::getCurrentPad();
        int confirm;
        if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0)
            confirm = (pad->mPressedButtonFlags >> 21) & 1;
        else
            confirm = (pad->mPressedButtonFlags >> 4) & 1;
        if (confirm != 0) {
            mFlagB8 = 3;
            func_8022B8E4(&mSysWin[0]);
        }
        break;
    }
    case 3:
        // Closing transition finished - hand control back to the UI.
        if (CSysWin_isActive(&mSysWin[0])) {
            field_67 = 0;
            field_64 = 1;
        }
        break;
    }

    func_8022B748(&mSysWin[0]);
}

// Singleton accessor.
CSysWinBuff* CSysWinBuff::getInstance() { return lbl_eu_806648E0; }

