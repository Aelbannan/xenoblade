#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "kyoshin/CBgTex.hpp"

class CScn;
class CTitleAHelp;
class CItemBoxGrid;

// MWCC pointer-to-member-function triple (copied around by value in ctors).
struct CMenuPtmf {
    u32 func;
    u32 delta;
    u32 vindex;
};

/*
 * Menu item-screen process (in-game item menu singleton).
 *
 * Layout mirrors cf::CInfoCf's CMenuItem notes / CMenuGetItem:
 *   0x00: CProcess head (CDoubleListNode), vptr at 0x10 written by
 *         __ct__8CProcessFv, then swapped temp -> composite vtable here.
 *   0x3C/0x48: Move/Draw callback pointer-to-member slots (__ptmf_null).
 *   0x58: IScnRender subobject vptr (composite vtable + 0x24).
 *   0x60: CBgTex, 0x80: CTitleAHelp, 0xB8: CItemBoxGrid.
 *   0x4AC4-0x4AC6: state / config flag bytes.
 */
class CMenuItem {
public:
    CMenuItem();
    ~CMenuItem();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    u8 mProcess[0x10];         // 0x00 CProcess storage head (CDoubleListNode)
    u32 mVtable10;             // 0x10 CProcess vtable slot (temp, then composite)
    u8 mProcRest[0x39 - 0x14]; // 0x14..0x38 CProcess tail (CChildListNode)
    u8 mIsRemove;              // 0x39 CProcess::mIsRemove
    u8 mIsDisableMove;         // 0x3A
    u8 mIsDisableDraw;         // 0x3B
    u32 ptmfCallbacks[6];      // 0x3C - Move (0-2) / Draw (3-5) callback pmfs
    u8 mField54;               // 0x54
    u8 mField55;               // 0x55
    u8 _pad56[2];              // 0x56
    u32 mIScnRenderVt;         // 0x58 - IScnRender subobject vtable slot
    CScn* mScene;              // 0x5C - owning scene
    CBgTex mBgTex;             // 0x60 - background layout widget
    u8 mTitleAHelp[0x38];      // 0x80 - title/help bar (opaque storage)
    u8 mItemBoxGrid[0x4AC4 - 0xB8]; // 0xB8..0x4AC3 - item grid (opaque storage)
    u8 mState;                 // 0x4AC4
    u8 mField4AC5;             // 0x4AC5
    u8 mField4AC6;             // 0x4AC6
};

// Singleton instance pointer (retail sbss; cleared by CInfoCf-side code).
extern u32 lbl_eu_80664258;

// cf::CfPadData view: only the flag words the menu input handler reads
// (+0x00 held, +0x04 pressed, +0x104 turbo/short-press).
struct CfMenuPadData {
    u32 mHeldFlags;                  // +0x00
    u32 mPressedFlags;               // +0x04
    u8 _pad08[0x104 - 0x08];         // 0x08..0x103
    u32 mTurboFlags;                 // +0x104
};

// Input-handler callees (retail unmangled names).
extern "C" CfMenuPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" void func_801CCAF0(CItemBoxGrid* self);
extern "C" u32 func_801CB0FC(CItemBoxGrid* self);
extern "C" void func_801CC7B0(CItemBoxGrid* self, int arg);
extern "C" void func_801CB38C(CItemBoxGrid* self);
extern "C" void func_801CB5F0(CItemBoxGrid* self);
extern "C" void func_801CBA04(CItemBoxGrid* self);
extern "C" void func_801CBDE8(CItemBoxGrid* self);
extern "C" void func_801CC0EC(CItemBoxGrid* self);
extern "C" void func_801CC5DC(CItemBoxGrid* self);
extern "C" void func_801CDC40(CItemBoxGrid* self);
extern "C" void func_801CDEE8(CItemBoxGrid* self);
extern "C" u8 func_801CDFB4(CItemBoxGrid* self);

// Temp (CProcess) vtable pre-PMF copy; composite vtable whose IScnRender
// sub-vtable sits at +0x24.
extern char lbl_eu_8052BF70[];
extern char lbl_eu_805309C0[];

// Null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// C-ABI imports: CProcess is abstract so its ctor is called by retail name,
// and the US build strips the widget ctor manglings (see CInfoCf.hpp).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* arg1, u8 arg2);
extern "C" void __ct__CItemBoxGrid(CItemBoxGrid* self, u32 arg1, u32 arg2,
                                   u32 arg3, u32 arg4);
