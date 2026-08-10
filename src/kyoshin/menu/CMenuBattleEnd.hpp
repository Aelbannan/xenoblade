#pragma once

#include <types.h>

namespace nw4r {
namespace lyt {
class Layout;
class AnimTransform;
class ArcResourceAccessor;
class Pane;
}
}
class CScn;
class CProcess;
class IScnRender;

// Minimal CTaskGame decl (retail global-namespace class; symbols
// getInstance__9CTaskGameFv / func_800426F0__9CTaskGameFv). The real
// CTaskGame.hpp pulls broken monolib work.hpp-umbrella headers, so the static
// members are declared here (same scheme as CSystemWindow.hpp).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// One entry of the internal message/queue array (stride 8).
struct UnkBattleEntry {
    u8 flag;   // +0x00
    u8 _pad[3];
    u32 param; // +0x04
};

/*
 * Battle-end (result) screen task.
 *
 * Retail stores the vtable pointer at +0x10 (CProcess-style layout). The base
 * CProcess subobject is constructed OOL via __ct__8CProcessFv and the class's
 * own vtable + the CProcess run-time fields are filled by hand, so the struct
 * is laid out explicitly rather than inheriting (same scheme as CMenuQstCnt).
 *
 *   0x00 vtable region (CProcess: link nodes + child list + flags)
 *   0x10 u32 mVtab                     (CMenuBattleEnd vtable)
 *   0x3C ptmf (Move hook, __ptmf_null copy)
 *   0x48 ptmf (Draw hook, __ptmf_null copy)
 *   0x54 nw4r::lyt::Layout*  mLayout
 *   0x6C u32 mWorkEventVt              (IWorkEvent dispatch vtable ptr)
 *   0x70 u32 mScnRenderVt              (IScnRender dispatch vtable ptr)
 *   0x74 CScn* mScene
 *   0x78 UnkClass_8045F564 mMemRegion  (0x10)
 *   0x88/0x8C AnimTransform* mAnim88 / mAnim8C (two selectable anims)
 *   0x90 AnimTransform* mAnim90        (currently selected anim)
 *   0x94 UnkBattleEntry mEntries[4]
 *   0xB4 u8 mState, 0xB5 u8 mMode, 0xB8 u32 mParam
 */
struct CMenuBattleEnd {
    u8  mPad0[0x10];                        // 0x00 link-list nodes
    u32 mVtab;                              // 0x10 vtable ptr
    u8  mPad14[0x3C - 0x14];                // 0x14 rest of CProcess
    u32 mPtMf3C[3];                         // 0x3C Move ptmf (__ptmf_null)
    u32 mPtMf48[3];                         // 0x48 Draw ptmf (__ptmf_null)
    nw4r::lyt::Layout* mLayout;             // 0x54 (set up in Init)
    u32 mField58;                           // 0x58
    u32 mField5C;                           // 0x5C
    s32 mField60;                           // 0x60 (-1)
    u8  mField64;                           // 0x64
    u8  mField65;                           // 0x65
    u8  mField66;                           // 0x66
    u8  mActive;                            // 0x67
    u32 mField68;                           // 0x68
    u32 mWorkEventVt;                       // 0x6C IWorkEvent dispatch vtable ptr
    u32 mScnRenderVt;                       // 0x70 IScnRender dispatch vtable ptr
    CScn* mScene;                           // 0x74
    u8  mMemRegion[0x10];                   // 0x78 UnkClass_8045F564 storage
    nw4r::lyt::AnimTransform* mAnim88;      // 0x88
    nw4r::lyt::AnimTransform* mAnim8C;      // 0x8C
    nw4r::lyt::AnimTransform* mAnim90;      // 0x90
    UnkBattleEntry mEntries[4];             // 0x94 (0x20 bytes)
    s8  mState;                             // 0xB4 (retail compares signed)
    u8  mMode;                              // 0xB5
    u8  _padB6[2];
    u32 mParam;                             // 0xB8

    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
};

// The retail symbols for this class's own lifecycle functions are the
// unmangled C-linkage names below (the ctor/dtor/task entry take an explicit
// receiver; func_8026F95C / func_8026FB0C are the CProcess run-time hooks and
// the queue writer). Definitions live in CMenuBattleEnd.cpp.
extern "C" {
CMenuBattleEnd* __ct__CMenuBattleEnd(CMenuBattleEnd* obj, CScn* scene, u8 mode,
                                     u32 param);
CMenuBattleEnd* __dt__14CMenuBattleEndFv(CMenuBattleEnd* obj, int flags);
CMenuBattleEnd* func_8026F8B0(CProcess* parent, CScn* scene, u8 mode, u32 param);
void func_8026F95C(CMenuBattleEnd* obj);
void func_8026FB0C(CMenuBattleEnd* obj, u8 flag, u32 param);
void cbRenderBefore__14CMenuBattleEndFv(void* sub);
}

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
// NOTE: addRenderCB / func_801355F4 bypass the broken monolib work.hpp-umbrella
// headers (CScn.hpp / CUICfManager.hpp) which fail to compile repo-wide (CTTask
// redefinition); declared here directly with their literal retail names.
extern "C" {
extern u32 lbl_eu_8052D238[];       // CMenuBattleEnd primary vtable
extern u32 lbl_eu_80537AB0[];       // dispatch vtable cluster base
extern char lbl_eu_8050E6F8[];      // layout/animation/pane name string table
extern u32 __ptmf_null[];           // null member-function-pointer triple
extern f32 lbl_eu_80668990;         // anim frame target for func_80137444
extern f32 lbl_eu_80668994;         // anim reset frame for SetFrame
nw4r::lyt::ArcResourceAccessor* func_801355F4();
void addRenderCB__4CScnFP10IScnRenderUlUl(CScn*, IScnRender*, u32, u32);
void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);
void __ct__17UnkClass_8045F564Fv(void*);
void __dt__17UnkClass_8045F564Fv(void*, int);
void __dt__8CProcessFv(void*, int);
void __dl__FPv(void*);
void waitForDrawDone__9CDeviceVIFv();
void Regist__8CProcessFP8CProcessb(void*, void*, bool);
void __ct__Q34nw4r3lyt8DrawInfoFv(void*);
void __dt__Q34nw4r3lyt8DrawInfoFv(void*, int);
void func_8045F778__17UnkClass_8045F564Fv(void*);
}

// C++-linkage helpers (retail symbols are their Itanium-mangled forms).
u32 func_801355A0();
void func_80138078(u32);

// .sbss globals shared with the retail binary (global-scope plain externs keep
// the unmangled symbol; no extern "C" needed outside a namespace).
extern u32 lbl_eu_80663E28;
extern CMenuBattleEnd* lbl_eu_80664898;