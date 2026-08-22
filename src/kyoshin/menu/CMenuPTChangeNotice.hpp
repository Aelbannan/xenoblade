#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"

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
    void OnFileEvent();
};

// The retail symbols for this class's own lifecycle functions are the
// unmangled C-linkage names below (the ctor/dtor take an explicit receiver;
// the dtor carries the complete-object/delete flag). Definitions live in
// CMenuPTChangeNotice.cpp.
extern "C" {
CMenuPTChangeNotice* __ct__CMenuPTChangeNotice(CMenuPTChangeNotice* obj, void* arg);
CMenuPTChangeNotice* __dt__19CMenuPTChangeNoticeFv(CMenuPTChangeNotice* obj, int flags);
CMenuPTChangeNotice* func_80293B9C(CProcess* parent, void* arg);
void func_80293C20(CMenuPTChangeNotice* obj);
void func_80293D2C(CMenuPTChangeNotice* obj);
int func_8029348C(CMenuPTChangeNotice* obj, CEventFile* evt);
}

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
extern "C" {
extern u32 lbl_eu_80538888[];        // CMenuPTChangeNotice primary vtable
extern const f32 lbl_eu_80668B90;    // animation frame target
extern const f32 lbl_eu_80668B80;    // pad-input disable timer
extern const char* lbl_eu_80662AF8;  // file path for the notice layout
void func_8008294C__Q22cf13CfGameManagerFv(bool enable);
void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32 handle, const char* path,
                                                   IWorkEvent* evt, int, int);
void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent,
                                   bool insertTop);
void func_801C1B94__Q22cf9CfPadTaskFf(float f1);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
void __dt__17UnkClass_8045F564Fv(UnkClass_8045F564* self, int flags);
void __dt__9IUIWindowFv(void* self, int flags);
void __dl__FPv(void* p);
// Unmangled in retail (unlike func_80137444's mangled form).
u32 func_80137510(nw4r::lyt::AnimTransform* anim, float f1);
// Layout/message/input helpers used by the file-load completion handler.
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
u16 func_8013606C(const void*, const void*, u32);
char* func_80136190(const void*, const void*, int);
void func_8013676C(void*, u32);
void func_80137E7C(nw4r::lyt::Layout*, const char*, void*);
char* func_80138F78(u32);
nw4r::lyt::ArcResourceAccessor* func_801355F4();
u16 func_80136254(const void*, const void*, int);
void func_80124270(nw4r::lyt::Pane*, u32);
void func_80124288(nw4r::lyt::Pane*, float*);
u32 func_8009CF8C(u32);
void func_80139198(u32);
u8 func_801392B4(u32);
u8 func_801392C8(u32);
u8 code80135FDC_getByte_64077();
u8 code80135FDC_getByte_6407E();
void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
}

// C++-linkage helpers (retail symbols are their MWCC-mangled forms).
u32 func_80137444(nw4r::lyt::AnimTransform* anim, float f1);
void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**,
                   nw4r::lyt::ArcResourceAccessor*, char*);

// .sbss globals shared with the retail binary (global-scope plain externs keep
// the unmangled symbol; no extern "C" needed outside a namespace).
extern u32 lbl_eu_80664A00;  // CMenuPTChangeNotice singleton pointer

// Strings / constants for the notice layout (retail data symbols).
extern char lbl_eu_8050FC20[];     // notice layout path + pane-name table
// .sbss pointer globals holding the character/affinity icon message tables.
extern void* lbl_eu_80664090;
extern void* lbl_eu_80664098;
extern const f32 lbl_eu_80668B84;  // animation frame target (notice open)
// 2^52 double used by the u32->float conversion (lfd pool constant).
extern const f64 lbl_eu_80668B88;
