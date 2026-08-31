// kyoshin/CSysWinBuff - system window "buff" process (singleton).

#include "kyoshin/CSysWinBuff.hpp"
extern char lbl_eu_806629E8[];
extern char lbl_eu_806629E0[];
extern char lbl_eu_80537EC0[];
extern char lbl_eu_80537E84[];
extern char lbl_eu_80537DC0[];
extern char lbl_eu_80537DB0[];
extern char lbl_eu_80537D28[];
extern char lbl_eu_8050EBC4[];
extern char lbl_eu_8050EBB8[];
extern "C" {
    void func_8003AA78__5CBdatFUlPv(int, void*);
    void func_8003AA8C__5CBdatFUl(int);
    void func_8008294C__Q22cf13CfGameManagerFv(bool);
}
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CTaskGame.hpp"

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
// Target 1: CSysWinBuff::~CSysWinBuff (us-80276958)
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
// Target 2: func_80274A84 (us-80276f08) - IWorkEvent::OnFileEvent body.
// When the loaded arc matches mFileHandle: hand its data buffer to CBdat
// (releases the archive), re-store the loaded file-pointer (getFP) in the
// global, clear the handle and return true; otherwise false.
// ---------------------------------------------------------------------------
extern "C" bool func_80274A84(CSysWinBuff* self, CEventFile* pEventFile) {
    if (self->mFileHandle == pEventFile->mFileHandle) {
        void* data = self->mFileHandle->getData();
        func_8003AA78__5CBdatFUlPv(2, data);
        func_8003AA34();
        lbl_eu_806648E4 = (u32)getFP__FPCc(&lbl_eu_8050EBC4[0x22]);
        self->mFileHandle = 0;
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Target 3: CSysWinBuff::Term (us-80276b08)
// Wait for the VI, release the arc file handle and tear down the embedded
// CSysWin, release the BDAT archive, clear the two globals, deregister the
// render callback (this-adjusting IScnRender view at +0x70), then re-enable
// the cf load once if the boot-time byte flag is still clear.
// ---------------------------------------------------------------------------
void CSysWinBuff::Term() {
    CDeviceVI::waitForDrawDone();
    func_801390E0(&mFileHandle);
    func_8022B7F4(&mSysWin[0]);
    func_8003AA8C__5CBdatFUl(2);
    lbl_eu_806648E4 = 0;
    lbl_eu_806648E0 = 0;
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->removeRenderCB(render);
    func_8013B980();
    if (code80135FDC_getByte_64080() == 0)
        func_8008294C__Q22cf13CfGameManagerFv(false);
}

// ---------------------------------------------------------------------------
// Target 4: CSysWinBuff::cbRenderBefore (us-80276d34)
// Gate prefix (task busy / global render flag / scene active / cf busy) then
// draw the embedded CSysWin through a stack DrawInfo.
// ---------------------------------------------------------------------------
void CSysWinBuff::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
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
// Target 5: create__11CSysWinBuffFv (us-80276dcc)
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
        // (retail loads __ptmf_null[1],[0],[2] per slot - load-both/store-both,
        // so use intermediate locals to force the retail ordering).
        const u32* ptmf = __ptmf_null;
        u32 ptmfWord1 = ptmf[1];
        u32 ptmfWord0 = ptmf[0];
        obj->callbacks[0] = ptmfWord0;
        obj->callbacks[1] = ptmfWord1;
        u32 ptmfWord2 = ptmf[2];
        obj->callbacks[2] = ptmfWord2;
        ptmfWord1 = ptmf[1];
        ptmfWord0 = ptmf[0];
        obj->callbacks[3] = ptmfWord0;
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
        obj->field70 = (u32)(lbl_eu_80537DC0 + 0xac);        obj->fileHandle = 0;
        obj->scene = scene;

        __ct__CSysWin(&obj->mSysWin[0], 0);

        obj->flagB8 = 0;
        obj->argBA = arg;
        func_8008294C__Q22cf13CfGameManagerFv(true);
        code80135FDC_postIncByte_64080();
    }
    lbl_eu_806648E0 = (CSysWinBuff*)obj;
    ((CProcess*)obj)->Regist(registerParent, false);
    return lbl_eu_806648E0;
}

// Non-target members: CProcess overrides (stubs) and the singleton accessor.
void CSysWinBuff::Init() {}
void CSysWinBuff::Move() {}
CSysWinBuff* CSysWinBuff::getInstance() { return lbl_eu_806648E0; }

// absorb: retail data (generated)
__declspec(section ".rodata") char lbl_eu_8050EBB8[0xc] = {0x43, 0x53, 0x79, 0x73, 0x57, 0x69, 0x6e, 0x42, 0x75, 0x66, 0x66, 0x00};
__declspec(section ".rodata") char lbl_eu_8050EBC4[0x2c] = {0x2f, 0x63, 0x6f, 0x6d, 0x6d, 0x6f, 0x6e, 0x2f, 0x6a, 0x70, 0x2f, 0x62, 0x64, 0x61, 0x74, 0x5f, 0x6d, 0x65, 0x6e, 0x75, 0x5f, 0x6d, 0x65, 0x73, 0x2e, 0x62, 0x69, 0x6e, 0x00, 0x68, 0x65, 0x6c, 0x70, 0x00, 0x4d, 0x4e, 0x55, 0x5f, 0x62, 0x75, 0x66, 0x66, 0x00, 0x00};
__declspec(section ".data") char lbl_eu_80537D28[0x88] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__declspec(section ".data") char lbl_eu_80537DB0[0x10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__declspec(section ".data") char lbl_eu_80537DC0[0xc4] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__declspec(section ".data") char lbl_eu_80537E84[0x3c] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__declspec(section ".data") char lbl_eu_80537EC0[0x10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__declspec(section ".sdata") char lbl_eu_806629E0[0x8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__declspec(section ".sdata") char lbl_eu_806629E8[0x8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
CSysWinBuff* lbl_eu_806648E0;
u32 lbl_eu_806648E4;
__declspec(section ".sbss") char lbl_eu_806648E8[0x1];
__declspec(section ".sbss") char lbl_eu_806648EC[0x4];
__declspec(section ".sbss") char lbl_eu_806648F0[0x1];
__declspec(section ".sbss") char lbl_eu_806648F4[0x4];
__declspec(section ".sbss") char lbl_eu_806648F8[0x1];
__declspec(section ".sbss") char lbl_eu_806648FC[0x4];
