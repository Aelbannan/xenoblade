#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

class CProcess;
class CScn;
namespace nw4r {
namespace lyt {
    class Layout;
    class AnimTransform;
    class ArcResourceAccessor;
}
}

// IUIWindow region (0x6C): CProcess base (0x00-0x3B) + vtable + window fields.
// The dtor is declared (defined by the retail __dt__9IUIWindowFv) so the
// derived dtor emits the base-destruction call; it is deliberately
// NON-virtual so MWCC keeps the vtable pointer at +0x10 (retail layout).
class IUIWindow {
public:
    ~IUIWindow();
    u8 _00[0x10];                  // 0x00 - CProcess link-list region
    u32 mField10;                  // 0x10 - vtable
    u8 _14[0x54 - 0x14];           // 0x14 - ptmf slots
    nw4r::lyt::Layout* mLayout;    // 0x54
    u32 mField58;                  // 0x58
    u32 mField5C;                  // 0x5C
    s32 mField60;                  // 0x60
    u8 mField64;                   // 0x64
    u8 mField65;                   // 0x65
    u8 mField66;                   // 0x66
    u8 mField67;                   // 0x67
    u32 mField68;                  // 0x68
};

// KEEP (header-consolidation round 2, 2026-10-23): the ctor stores both
// subobject vptrs by hand, so these stay PODs instead of the monolib classes.
// IWorkEvent / KizunaScnRender base subobjects: 4-byte vtable slots into the retail
// composite vtable (+0x24 / +0xAC, see the ctor). Plain PODs - the retail
// subobject vtables are stored by the ctor, not by MWCC.
struct KizunaWorkEvent {
    u32 mVtable;
};
struct KizunaScnRender {
    u32 mVtable;
};

// KEEP (header-consolidation pilot 2026-10-23): this TU models IWorkEvent as a
// manual-vtable struct above, which conflicts with the real IWorkEvent that the
// canonical monolib/scn/CScn.hpp derives from, so it cannot include it.
// Minimal CScn view: only the render-callback members are used by this TU
// (retail symbols addRenderCB__4CScnFP10IScnRenderUlUl /
// removeRenderCB__4CScnFP10IScnRender). Defined OOL in monolib/src/scn/CScn.
// Render-callback argument type: the retail mangling is
// addRenderCB__4CScnFP10IScnRenderUlUl, so the parameter is the canonical
// IScnRender class (forward-declared; the subobject vtable slot is written by
// hand through the KizunaScnRender POD above).
class IScnRender;
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};

// Layout vtable view: MWCC inserts an offset-to-top + RTTI prefix (2 entries)
// so the first declared virtual sits at vtable + 0x8. All-pure so MWCC emits
// no vtable for the view itself.
class CLayoutView {
public:
    virtual void vf02(int flag) = 0;   // vtable + 0x8 - release/unbind
};

// kickoffKizunaWindow stack scratch laid out like the retail frame:
// cursor @sp+0x08, case-2 CSysWin @sp+0x20, case-0 CSysWin @sp+0x5c.
struct KizunaScratch {
    u8 mCur[0x18];  // raw cursor frame slot (no ctor runs on the scratch)
    u8 mWin2[0x3C];
    u8 mWin0[0x3C];
};

/*
 * Kizuna (friendship) talk screen - singleton, created by createKizunaTalkSingleton.
 *
 * Retail layout (ctor / dtor / Term):
 *   0x00  IUIWindow base (0x6C)
 *   0x6C  IWorkEvent base (composite vtable + 0x24)
 *   0x70  KizunaScnRender base (composite vtable + 0xAC)
 *   0x74  CScn* owning scene (render-callback target)
 *   0x78  UnkClass_8045F564 storage (0x10 bytes)
 *   0x88/0x8C  CFileHandle* (talk-list file / shared archive)
 *   0x90  nw4r::lyt::ArcResourceAccessor*
 *   0x94  nw4r::lyt::AnimTransform*
 *   0x98  character id (BDAT affinity index)
 *   0x9C  page byte (signed; clamps in driveKizunaTalkPad)
 *   0xA0  move state
 *   0xA4  window state (switch in pollKizunaTalkReady)
 *   0xA8  CCur18 cursor storage (0x18 bytes)
 *   0xC0  CSysWin panel storage (0x3C bytes)
 *   sizeof = 0xFC
 *
 * The class is deliberately NON-polymorphic: the retail ctor stores the
 * composite / subobject vtables by hand and the retail dtor emits no vtable
 * store, so the dtor is non-virtual and the bases carry no virtuals.
 */
class CEventFile;

class CMenuKizunaTalk : public IUIWindow, public KizunaWorkEvent, public KizunaScnRender {
public:
    static CMenuKizunaTalk* sInstance;

    CScn* mScene;                        // 0x74
    UnkClass_8045F564 mMemRegion;        // 0x78
    CFileHandle* mFile1;                 // 0x88
    CFileHandle* mFile2;                 // 0x8C
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // 0x90
    nw4r::lyt::AnimTransform* mAnim;     // 0x94
    u32 mCharId;                         // 0x98
    s8 mField9C;                         // 0x9C
    u32 mFieldA0;                        // 0xA0
    u8 mFieldA4;                         // 0xA4
    u8 _padA5[3];                        // 0xA5 - padding before mCur
    CCur18 mCur;                         // 0xA8
    u8 mSysWin[0x3C];                    // 0xC0 - CSysWin panel storage

    ~CMenuKizunaTalk();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore(CScn* scene);
    void OnFileEvent(CEventFile* pEvent);
};

// C-linkage imports
// (the CSysWin / UnkClass_8045F564 dtors and the CDeviceVI / CProcess base
// ctor); the rest come from CItemBoxGrid.hpp / functions.hpp.
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __ct__8CProcessFv(void*);

// Flat cf pad-data view for the kizuna talk input handling (offsets match
// retail, same as CSysPadData / KizunaPadData views elsewhere).
struct KizunaTalkPadData {
    u8 _00[0x4];
    u32 mPressedFlags;          // +0x04 CPad::mPressedButtonFlags
    u8 _08[0x104 - 0x8];
    u32 mTurboFlags;            // +0x104 CfPadData::mTurboPressButtonFlags
};

// C-ABI helper imports for the kizuna talk input / file-event handlers. The
// retail symbols are unmangled, so they must stay C-linkage (the C++-linkage
// declarations in CSysWinSelect.hpp/CSysWinSave.hpp would mangle them).
extern "C" KizunaTalkPadData* getCfPadData__Q22cf13CfGameManagerFv();


// Layout animation frame-target check (CSysWin.cpp / CCol6System.hpp decl).

// C-linkage helpers formerly pulled in through CItemBoxGrid.hpp (which now
// defines a real IWorkEvent-derived class and so cannot be included here).
// BdatGetU8Direct keeps the wide-view (u32) decl: the callee's lbz already
// zero-extends and callers keep the raw result (mr), masking only at use.
extern "C" int KyoshinHeap_GetField44();
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void __ct__7CSysWinFUc(void*, int);
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
#include "kyoshin/cf/CtrlRemoteApi.hpp"
//@@KIZUNA_DECLS@@

// Global data imports (MWCC does not mangle global-scope data names).
extern char lbl_eu_80505118[];            // shared-archive path string (.rodata)
extern u8* sKizunaBdatTable;               // BDAT character table pointer (.sbss)
extern char lbl_eu_8052D238[];            // CTTask_IUIWindow vtable (.data)
extern char lbl_eu_80533A98[];
extern u32 __ptmf_null[3];                // null pointer-to-member-function
