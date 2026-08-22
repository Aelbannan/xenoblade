#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r {
namespace lyt {
class Layout;
class AnimTransform;
class ArcResourceAccessor;
}
} // namespace nw4r

// Render-callback APIs on the owning scene (OOL in CScn, retail symbols
// addRenderCB__4CScnFP10IScnRenderUlUl / removeRenderCB__4CScnFP10IScnRender).
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};

// Minimal CTaskGame decl (retail global-namespace class, symbols
// getInstance__9CTaskGameFv / func_800426F0__9CTaskGameFv).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

/*
 * Zeal (collective) menu screen controller.
 *
 * Deliberately NON-polymorphic (the CMenuGetItem / CSystemWindow idiom): the
 * CProcess base region is embedded at +0x00 and the class's vtable lives at
 * +0x10 (written by the allocating factory ctor __ct__CMenuZeal). The dtor
 * drives the UnkClass_8045F564 region and the CProcess base destruction by
 * hand (via their retail dtor symbols); a C++ CProcess base would make MWCC
 * emit an extra vptr store / duplicate subobject destruction that the retail
 * dtor does not have.
 *
 * Layout (derived from the Init/Term/cbRenderBefore ASM):
 *   0x00  CProcess region (link nodes, vtable at 0x10, child list, flags)
 *   0x3C  ptmfMove[3] / 0x48 ptmfDraw[3] (__ptmf_null callback slots)
 *   0x54/0x55 state bytes (0x54 read by the func_8017FC88 singleton gate)
 *   0x58/0x5C IWorkEvent / IScnRender subobject vtable slots
 *   0x60  CScn* mScn (owning scene, removeRenderCB target)
 *   0x64/0x78/0x8C nw4r::lyt::Layout* (drawn in cbRenderBefore)
 *   0x9C  f32 (position constant written by func_8017FEF0)
 *   0xA0/0xA1 flags (0xA0 gates the 0x8C layout draw)
 *   0xA4  UnkClass_8045F564 scratch region (0x10 bytes)
 *   0xBD/0xBE/0xBF flags (0xBD gates the whole draw; 0xBF set by the
 *        func_8017FC88 singleton gate and cleared in Term)
 */
class CMenuZeal {
public:
    ~CMenuZeal();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    u8 mProcess[0x10];            // 0x00 CProcess storage head (CDoubleListNode)
    u32 mProcessVt;               // 0x10 vtable slot (written by factory ctor)
    u8 mProcRest[0x39 - 0x14];    // 0x14..0x38 CProcess tail (CChildListNode)
    u8 mIsRemove;                 // 0x39
    u8 mIsDisableMove;            // 0x3A
    u8 mIsDisableDraw;            // 0x3B
    u32 ptmfMove[3];              // 0x3C null pointer-to-member-function
    u32 ptmfDraw[3];              // 0x48 null pointer-to-member-function
    u8 mField_54;                 // 0x54
    u8 mField_55;                 // 0x55
    u8 _pad56[2];                 // 0x56-0x57
    u32 mIWorkEventVt;            // 0x58 IWorkEvent subobject vtable slot
    u32 mIScnRenderVt;            // 0x5C IScnRender subobject vtable slot
    CScn* mScn;                        // 0x60 owning scene
    nw4r::lyt::Layout* mLayout1;       // 0x64 first layout
    nw4r::lyt::AnimTransform* mAnim1;  // 0x68 first anim transform
    nw4r::lyt::AnimTransform* mAnim2;  // 0x6C
    nw4r::lyt::AnimTransform* mAnim3;  // 0x70
    nw4r::lyt::AnimTransform* mAnim4;  // 0x74
    nw4r::lyt::Layout* mLayout2;       // 0x78 second layout
    nw4r::lyt::AnimTransform* mAnim5;  // 0x7C
    nw4r::lyt::AnimTransform* mAnim6;  // 0x80
    nw4r::lyt::AnimTransform* mAnim7;  // 0x84
    nw4r::lyt::AnimTransform* mAnim8;  // 0x88
    nw4r::lyt::Layout* mLayout3;       // 0x8C third layout
    nw4r::lyt::AnimTransform* mAnim9;  // 0x90
    f32 mField_94;                     // 0x94
    f32 mField_98;                     // 0x98
    f32 mField_9C;                     // 0x9C
    u8 mField_A0;                      // 0xA0
    u8 mField_A1;                      // 0xA1
    u8 _padA2[2];                      // 0xA2-0xA3
    u8 mLayoutMem[0x10];               // 0xA4-0xB3 scratch region storage (manual dtor,
                                       //   raw bytes so MWCC does not auto-destroy it)
    f32 mField_B4;                     // 0xB4
    u32 mField_B8;                     // 0xB8
    u8 mField_BC;                      // 0xBC
    u8 mField_BD;                      // 0xBD
    u8 mField_BE;                      // 0xBE
    u8 mField_BF;                      // 0xBF
    f32 mField_C0;                     // 0xC0
    int mField_C4;                      // 0xC4 (menu state machine: 1/2/3/4/6)
};

// Zeal singleton instance (retail SDA symbol).
extern CMenuZeal* lbl_eu_806642A0;
// cbRenderBefore gate flag (bit 21 skips the draw).
extern u32 lbl_eu_80663E28;
// func_8017FEF0 positioning constant (sdata2 float pool).
extern const f32 lbl_eu_80667890;

// Temporary (CProcess) vtable, pre-PMF copy; composite vtable afterwards.
extern char lbl_eu_8052BF70[];
// Composite vtable: IWorkEvent sub-vtable at +0x24, IScnRender sub-vtable at +0xac.
extern char lbl_eu_80531BE8[];
// Null pointer-to-member-function constant (3 words, copied into +0x3C..0x53).
extern u32 __ptmf_null[3];
// sdata2 float pool constants (ctor initialisers / Move thresholds).
extern const f32 lbl_eu_80667870;
extern const f32 lbl_eu_80667874;
extern const f32 lbl_eu_80667878;
extern const f32 lbl_eu_8066788C;
// Zeal menu string table (layout/anim names, texture names, callbacks).
extern char lbl_eu_80503454[];

// C-ABI imports (retail pre-mangled names; these identifiers must keep C
// linkage so MWCC emits the literal reloc names -- see CSysWinSave.hpp).
extern "C" {
void __dt__8CProcessFv(CProcess* self, int flags);
void __ct__8CProcessFv(CProcess* obj);
void __ct__17UnkClass_8045F564Fv(u8* mem);
void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent,
                                   bool insertTop);
// Shared ARC resource accessor getter (retail unmangled name).
nw4r::lyt::ArcResourceAccessor* func_801355F4();
// Classic-controller detection; the retail callee ignores the arg, but the
// call site still materialises `li r3,-1` (the CfGameManager.hpp inline
// wrapper drops it when inlined, so bind the pre-mangled name directly).
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
}
// Effect/cue object factory (defined in pluginUi.cpp; declared here because
// pluginUi.hpp's extern "C" func_80136190 clashes with code_80135FDC.hpp's).
void* func_800451D8(u32 cls, void* param);

// UI sound effect (mangles to func_80138078__FUl).
void func_80138078(u32 op);
