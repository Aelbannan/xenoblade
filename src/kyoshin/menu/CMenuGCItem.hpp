#pragma once

#include <types.h>
#include <monolib/scn/IScnRender.hpp>
#include <monolib/work/CProcess.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CItemBoxGrid.hpp"

// Forward declaration for member type (not yet decompiled)
class COption;

/*
 * GC Item menu screen process.
 *
 * Known layout (constructor/destructor):
 *   0x00: CProcess        -- task-system base
 *   0x3C: ptmf0           -- vtable pointer-to-member-function data (Move hook)
 *   0x48: ptmf1           -- vtable pointer-to-member-function data (Draw hook)
 *   0x54: u8 mField54     -- phase flag
 *   0x55: u8 mField55
 *   0x58: IScnRender      -- render-callback subobject (member, vptr)
 *   0x5C: CProcess*       -- parent process reference
 *   0x60: CBgTex          -- background layout widget
 *   0x80: CTitleAHelp     -- title/help bar
 *   0xB8: CItemBoxGrid    -- core item-box grid logic (vptr subobject + storage)
 *  0x4AC4: u8 mState       -- phase/state indicator (0..3)
 *  0x4AC5: u8 mArg         -- constructor argument
 */
class CMenuGCItem : public CProcess {
public:
    CMenuGCItem(CProcess* parent, u32 arg);
    virtual ~CMenuGCItem();

    // CProcess overrides
    virtual void Init();
    virtual void Term();
    virtual void Move();

    // IScnRender callback (dispatched through the +0x58 subobject vtable)
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_802B0F08();
    void func_802B0F10();

    // Fields (partial -- enough for known accessors)
    // Inherited: CProcess (0x00-0x3B)
    u32 ptmf0[3];                 // 0x3C-0x47: vtable pointer-to-member-function data
    u32 ptmf1[3];                 // 0x48-0x53: vtable pointer-to-member-function data
    u8 mField54;                  // 0x54: phase flag
    u8 mField55;                  // 0x55
    u8 _pad56[2];                 // 0x56-0x57
    IScnRender mIScnRender;       // 0x58 (vptr; render-callback subobject)
    CProcess* mParentRef;         // 0x5C
    CBgTex mBgTex;                // 0x60
    CTitleAHelp mTitleAHelp;      // 0x80
    CItemBoxGrid mItemBoxGrid;    // 0xB8 (vptr-only subobject; storage padded below)
    u8 _padGrid[0x4AC4 - 0xBC];   // 0xBC-0x4AC3
    u8 mState;                    // 0x4AC4
    u8 mArg;                      // 0x4AC5
    CMenuGCItem();
};

// Data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_80663E28;            // render-gate mode bitfield (.sbss; bit 21 = busy)
extern unsigned long lbl_eu_80664C00;  // singleton instance flag (cleared by Term)
extern u8 lbl_eu_8052BF70[];           // temporary (CProcess) vtable, pre-PMF copy
extern u8 lbl_eu_8053AD08[];           // composite vtable; IScnRender sub-vtable at +0x24
// Null pointer-to-member-function constant (12-byte CW PMF triple).
struct Ptmf { u32 adjust; u32 func; u32 vtAdjust; };
extern u32 __ptmf_null[3];             // null pointer-to-member-function constant

// Retail-unmangled C-linkage callees (US strips the member manglings for the
// func_ helpers; isIdle keeps its C++ mangling). int returns keep the caller's
// `!= 0` as a plain cmpwi (no u8 mask) like retail.
extern "C" int func_801C3E34(CBgTex* self);
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* self);
extern "C" int func_801CB038(CItemBoxGrid* self);
extern "C" void func_801CB28C(CItemBoxGrid* self);
extern "C" void func_801CABC8(CItemBoxGrid* self);
extern "C" void func_801CAD8C(CItemBoxGrid* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801CAE9C(CItemBoxGrid* self);
extern "C" int GetField61(CItemBoxGrid* self);
extern "C" void func_80138078__FUl(u32 op);
extern "C" int func_8013BE50();

// CW-mangled ctors/dtors referenced at the retail call sites (extern "C" stops
// C++ re-mangling of the retail CW names).
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 id);
extern "C" void __ct__CItemBoxGrid(CItemBoxGrid* self, u32 a, u32 b, u32 c, u32 d);
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" CMenuGCItem* __dt__11CMenuGCItemFv(CMenuGCItem* self, int flags);
extern "C" void __dt__12CItemBoxGridFv(CItemBoxGrid* self, int flags);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int flags);
extern "C" void __dt__8CProcessFv(CProcess* self, int flags);

// nw4r DrawInfo C-ABI ct/dt (retail emits the direct calls; a C++ local would
// virtual-dispatch its scope-exit destructor and bloat the body).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(void* self);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(void* self, int dealloc);

// DrawInfo helper - retail emits the mangled nw4r form
// (func_80137250__FPQ34nw4r3lyt8DrawInfo), so it is declared as a normal C++
// global function to let MWCC mangle it - NOT inside the extern "C" block.
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
