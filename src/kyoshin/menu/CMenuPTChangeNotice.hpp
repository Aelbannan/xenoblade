#pragma once
#include "kyoshin/cf/CtrlRemoteApi.hpp"
#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

namespace nw4r {
namespace lyt {
class Layout;
class AnimTransform;
class ArcResourceAccessor;
}
}
class CProcess;
class CEventFile;
class CScn;
class IWorkEvent;

/*
 * Party-member change notice screen task.
 *
 * IUIWindow-derived region at +0x00 with the vtable stored manually at +0x10
 * (CProcess-style layout), the IWorkEvent / IScnRender dispatch vtables at
 * +0x6c/+0x70, an embedded MEM2 memory region at +0x74 and the async
 * file-handle / active-animation pair at +0x88/+0x94.
 *
 *   0x00 IUIWindow region (link nodes + runtime state)
 *   0x10 u32 mVtab          (CMenuPTChangeNotice vtable)
 *   0x54 nw4r::lyt::Layout* mLayout
 *   0x64 u8  mField64       (party-change trigger flag)
 *   0x6C u32 mWorkEventVt   (IWorkEvent dispatch vtable ptr)
 *   0x70 u32 mScnRenderVt   (IScnRender dispatch vtable ptr)
 *   0x74 UnkClass_8045F564 mMemRegion
 *   0x84 CScn* mScene       (render-callback target)
 *   0x88 u32 mFileHandle    (CDeviceFile::readFile result, CFileHandle*)
 *   0x8C nw4r::lyt::ArcResourceAccessor* mAccessor
 *   0x90 nw4r::lyt::Layout* mLayout90
 *   0x94 nw4r::lyt::AnimTransform* mAnim94
 *   0x98 u8  mField98       (notice state)
 */
class CMenuPTChangeNotice {
public:
    u8  mPad0[0x10];                     // 0x00 link-list nodes
    u32 mVtab;                           // 0x10 vtable ptr
    u8  mPad14[0x54 - 0x14];             // 0x14 rest of IUIWindow region
    nw4r::lyt::Layout* mLayout;          // 0x54
    u32 mField58;                        // 0x58
    u32 mField5C;                        // 0x5C
    s32 mField60;                        // 0x60 (-1)
    u8  mField64;                        // 0x64
    u8  mField65;                        // 0x65
    u8  mField66;                        // 0x66
    u8  mField67;                        // 0x67
    u32 mField68;                        // 0x68
    u32 mWorkEventVt;                    // 0x6C IWorkEvent dispatch vtable ptr
    u32 mScnRenderVt;                    // 0x70 IScnRender dispatch vtable ptr
    UnkClass_8045F564 mMemRegion;        // 0x74 (0x10 bytes)
    CScn* mScene;                        // 0x84 render-callback target
    u32 mFileHandle;                     // 0x88 readFile result (CFileHandle*)
    nw4r::lyt::ArcResourceAccessor* mAccessor;  // 0x8C layout arc accessor
    nw4r::lyt::Layout* mLayout90;        // 0x90 layout built from the arc
    nw4r::lyt::AnimTransform* mAnim94;   // 0x94 active animation
    u8  mField98;                        // 0x98

    /*
     * "timg" texture object returned by the resource accessor. The dims
     * pointer is reached through a two-level chain: obj->mChain (at +8)
     * points to a struct whose first field is the dimension header.
     */
    struct CTextureDims {
        u16 mW;                          // 0x00
        u16 mH;                          // 0x02
    };
    struct CTextureChain {
        CTextureDims* mDims;             // 0x00
    };
    struct CTextureObj {
        u8 _00[8];
        CTextureChain* mChain;           // 0x08
    };

    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void OnFileEvent(CEventFile* pEvent);
};

// The retail symbols for this class's own lifecycle functions are the
// unmangled C-linkage names below (the ctor/dtor take an explicit receiver;
// the dtor carries the complete-object/delete flag). Definitions live in
// CMenuPTChangeNotice.cpp.
extern "C" {
CMenuPTChangeNotice* __ct__CMenuPTChangeNotice(CMenuPTChangeNotice* obj, void* arg);
CMenuPTChangeNotice* __dt__19CMenuPTChangeNoticeFv(CMenuPTChangeNotice* obj, int flags);
CMenuPTChangeNotice* PTNotice_Create_3B9C(CProcess* parent, void* arg);
void PTNotice_Advance_3C20(CMenuPTChangeNotice* obj);
void PTNotice_Rewind_3D2C(CMenuPTChangeNotice* obj);
int PTNotice_BuildLayout_348C(CMenuPTChangeNotice* obj, CEventFile* evt);
}

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
extern "C" {
extern u32 lbl_eu_80538888[];        // CMenuPTChangeNotice primary vtable
extern const char* lbl_eu_80662AF8;  // file path for the notice layout
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32 handle, const char* path,
                                                   IWorkEvent* evt, int, int);
void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent,
                                   bool insertTop);
void setInputDisableTime__Q22cf9CfPadTaskFf(float f1);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
void __dt__17UnkClass_8045F564Fv(UnkClass_8045F564* self, int flags);
void __dt__9IUIWindowFv(void* self, int flags);
// Unmangled in retail (unlike advanceAnimTransform's mangled form).
// Layout/message/input helpers used by the file-load completion handler.
void writePanePos(nw4r::lyt::Pane*, float*);

void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
}

// C++-linkage helpers (retail symbols are their MWCC-mangled forms).

// .sbss globals shared with the retail binary (global-scope plain externs keep
// the unmangled symbol; no extern "C" needed outside a namespace).
extern u32 lbl_eu_80664A00;  // CMenuPTChangeNotice singleton pointer
