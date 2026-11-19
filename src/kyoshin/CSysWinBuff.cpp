// kyoshin/CSysWinBuff - system window "buff" process (singleton).

#define IWORK_EVENT_INLINE_DTOR
#define CTTASK_INLINE_DTOR
#define IUIWINDOW_INLINE_DTOR
#include "kyoshin/CSysWinBuff.hpp"
#include "kyoshin/cf/CfGameManager.hpp"  // canonical setPresentationFlag decl
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
// Forward decls for retail data labels (defs at file end). Char blobs carry
// immediates; D28/DB0/E0/E8 are pointer tables anchored by clean refs (see below).
extern char lbl_eu_8050E980[];
// (decl was via CTaskGame.hpp; kept local after IWorkEvent decouple.)
extern u32 gmPauseFlags;
// Singleton/word single defs (pre-code so no forwards are needed).
CSysWinBuff* lbl_eu_806648E0 = 0;
u32 lbl_eu_806648E4 = 0;
// .sbss flag pool (MWCC rejects __declspec(section ".sbss") (33044), so
// plain tentatives auto-route; explicit pad bytes reproduce the retail gaps
// (EveTalkWin precedent). Unreferenced by code (size-only). Single defs
// (no forwards) to dodge the scalar tentative+extern 10322 fault.
u8 lbl_eu_806648E8 = 0;
u8 buff_sbss_padE9a = 0;
u8 buff_sbss_padE9b = 0;
u8 buff_sbss_padE9c = 0;
u32 camControlDefault = 0;
u8 lbl_eu_806648F0 = 0;
u8 buff_sbss_padF1a = 0;
u8 buff_sbss_padF1b = 0;
u8 buff_sbss_padF1c = 0;
u32 camControlClassic = 0;
u8 lbl_eu_806648F8 = 0;
u8 buff_sbss_padF9a = 0;
u8 buff_sbss_padF9b = 0;
u8 buff_sbss_padF9c = 0;
u32 camControlGc = 0;
// CBdat / CfGameManager callees: mangled names as extern "C" (declared in
// CSysWinBuff.hpp). MWCC-1.1 rejects true member-call spelling (10322).
#include "monolib/scn/CScn.hpp"
class CEventFile;
#include "kyoshin/CTaskGame.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <new>
#include <revolution/gx/GXPixel.h>

// ---------------------------------------------------------------------------
// CSysWinBuff::~CSysWinBuff (us-80276958)
// Complete-object destructor. The embedded CSysWin is destroyed (flags -1),
// then the CProcess base guarded by the nested double null-check (an MWCC
// D2-inlined-into-D1 artifact), then conditional operator delete when the
// deleting flag is set. Same shape as CSysWinSave's dtor.
// ---------------------------------------------------------------------------
CSysWinBuff::~CSysWinBuff() {}

// ---------------------------------------------------------------------------
// CSysWinBuff::OnFileEvent (us-80276f08) - IWorkEvent::OnFileEvent body.
// When the loaded arc matches mFileHandle: hand its data buffer to CBdat
// (releases the archive), re-store the loaded file-pointer (getFP) in the
// global, clear the handle and return true; otherwise false. Only reachable
// through the IWorkEvent sub-vtable, whose slot holds the compiler-emitted
// +0x6C adjusting thunk that tail-jumps here (retail 0x80276F84).
// ---------------------------------------------------------------------------
bool CSysWinBuff::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        void* data = mFileHandle->getData();
        CBdat::setBdatEntry(2, data);
        Bdat_GetTable_AA34();
        lbl_eu_806648E4 = (u32)getFP("/common/jp/bdat_menu_mes.bin\0help\0MNU_buff" + 0x22);
        mFileHandle = 0;
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
    closeFileHandle(&mFileHandle);
    mSysWin.termLayout();
    CBdat::getEntry(2);
    lbl_eu_806648E4 = 0;
    lbl_eu_806648E0 = 0;
    IScnRender* render = this;
    mScene->removeRenderCB(render);
    DecMenuCounter64080();
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
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (!IsMenuState621F0()) return;
    if (MenuTutorialIsCreated() != 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    mSysWin.drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo));
    // Scope-exit dtor is auto-emitted as the retail direct
    // `bl __dt__Q34nw4r3lyt8DrawInfoFv(drawInfo, -1)`; an explicit
    // `drawInfo.~DrawInfo()` would ALSO emit a virtual-dispatched call.
}

// ---------------------------------------------------------------------------
// create__11CSysWinBuffFv (us-80276dcc)
// Allocate the CSysWinBuff singleton (0xBC bytes) from work memory, run the
// (inlined) constructor and register as a CProcess child of `registerParent`.
// Returns 0 when the singleton already exists; Regist is called even when the
// allocation failed, matching retail.
// ---------------------------------------------------------------------------
extern "C" CSysWinBuff* create__11CSysWinBuffFv(CProcess* registerParent, CScn* scene, u16 arg) {
    if (lbl_eu_806648E0 != 0)
        return 0;
    CSysWinBuff* obj = new (mtl::MemManager::allocate(0xbc, CWorkThreadSystem::getWorkMem()))
        CSysWinBuff(scene, arg);
    lbl_eu_806648E0 = obj;
    ((CProcess*)obj)->Regist(registerParent, false);
    return lbl_eu_806648E0;
}

// Two empty 4-byte text stubs at the tail of the retail .text slice
// (0x80276FA4/0x80276FA8) - not yet identified, kept as the retail symbols.
extern "C" void SysWinBuff_EmptyStubB20() {}

extern "C" void SysWinBuff_EmptyStubB24() {}

// Non-target members: CProcess overrides (stubs) and the singleton accessor.
void CSysWinBuff::Init() {}
void CSysWinBuff::Move() {}
CSysWinBuff* CSysWinBuff::getInstance() { return lbl_eu_806648E0; }

// Retail data pool: the arc-path/help/label string pool is a code literal (so
// MWCC pools it after the compiler-generated RTTI type-name strings, matching
// retail's .rodata order); the table below points at the three .sbss words
// code_8027513C.cpp indexes.
const char lbl_eu_8050EBC4[0x2c] = {0x2f, 0x63, 0x6f, 0x6d, 0x6d, 0x6f, 0x6e, 0x2f, 0x6a, 0x70, 0x2f, 0x62, 0x64, 0x61, 0x74, 0x5f, 0x6d, 0x65, 0x6e, 0x75, 0x5f, 0x6d, 0x65, 0x73, 0x2e, 0x62, 0x69, 0x6e, 0x00, 0x68, 0x65, 0x6c, 0x70, 0x00, 0x4d, 0x4e, 0x55, 0x5f, 0x62, 0x75, 0x66, 0x66, 0x00, 0x00};
