#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CItemBoxGrid.hpp"

class CScn;

/* 8-byte copy unit used by the sell-screen data blob. func_8018B130 copies
 * the 256-item array as one struct assignment, which MWCC lowers to the
 * retail `mtctr` + `lwzu/stwu` counted loop (same shape as CMenuShopBuy's
 * ShopBuyDataItem blob copies). */
struct ShopSellDataItem {
    u32 lo;  // +0x0
    u32 hi;  // +0x4
};

/* 0x800-byte array wrapper (whole-struct assignment emits the counted copy
 * loop; C arrays are not assignable directly). */
struct ShopSellDataItems {
    ShopSellDataItem v[256];  // 0x800 bytes
};

/* 0x806-byte sell data blob: the item array lives at +0x4 (offset +0x0 is
 * skipped by func_8018B130) with a trailing u16 at +0x804. */
struct ShopSellData {
    u32 mCount;                   // +0x0 (not copied)
    ShopSellDataItems mItems;     // +0x4..+0x803
    u16 mField804;                // +0x804
};

/* 0x18-byte cursor object. func_8018B0FC copies the +0x4..+0x15 region
 * (4 words + 2 bytes); callers pass the object base, so member offsets are
 * absolute within the object. */
struct ShopSellCursor18 {
    u32 field_0x0;    // +0x0 (not copied)
    u32 field_0x4;    // +0x4
    u32 field_0x8;    // +0x8
    u32 field_0xC;    // +0xC
    u32 field_0x10;   // +0x10
    u8 field_0x14;    // +0x14
    u8 field_0x15;    // +0x15
};

/*
 * Shop sell menu screen process.
 *
 * Layout mirrors the CMenuGCItem skeleton (retail offsets):
 *   0x00: CProcess -- task-system base (0x3C) + vtable PMF data
 *   0x54: u8 mField54 -- closing-state flag (func_8018B658)
 *   0x58: IScnRender -- render-callback subobject (member, vptr)
 *   0x5C: CScn* mScene -- owning scene (removeRenderCB target in Term)
 *   0x60: CBgTex -- background layout widget (0x20 bytes)
 *   0x80: CTitleAHelp -- title/help bar (0x38 bytes)
 *   0xB8: CItemBoxGrid -- item grid (vptr subobject; storage padded below)
 *  0x4AC4: u8 mState -- phase/state indicator (func_8018B420)
 */
class CMenuShopSell : public CProcess {
public:
    CMenuShopSell();
    virtual ~CMenuShopSell();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_8018B6A8();
    void func_8018B6B0();

    // CProcess base occupies 0x00-0x3B; vtable pointer-to-member data follows.
    u32 ptmf0[3];               // 0x3C-0x47: vtable pointer-to-member data
    u32 ptmf1[3];               // 0x48-0x53: vtable pointer-to-member data
    u8 mField54;                // 0x54: closing-state flag
    u8 mField55;                // 0x55
    u8 _pad56[2];               // 0x56-0x57
    IScnRender mIScnRender;     // 0x58: render-callback subobject (vptr)
    CScn* mScene;               // 0x5C: owning scene
    CBgTex mBgTex;              // 0x60
    CTitleAHelp mTitleAHelp;    // 0x80
    CItemBoxGrid mItemBoxGrid;  // 0xB8 (vptr-only subobject; storage padded below)
    u8 _padGrid[0x4AC4 - 0xBC]; // 0xBC-0x4AC3
    u8 mState;                  // 0x4AC4
};

extern "C" __declspec(noinline) CMenuShopSell* __ct__CMenuShopSell(CMenuShopSell* self, CScn* scene);
extern "C" CMenuShopSell* __dt__13CMenuShopSellFv(CMenuShopSell* self, int flags);
extern "C" void Init__13CMenuShopSellFv();
extern "C" void Term__13CMenuShopSellFv();
extern "C" void Move__13CMenuShopSellFv();
extern "C" void cbRenderBefore__13CMenuShopSellFv(void* self);
extern "C" void func_8018B0FC(void*, void*);
extern "C" void func_8018B130(void*, void*);
extern "C" CMenuShopSell* func_8018B324(CProcess* parent, CScn* scene);
extern "C" bool func_8018B398();
extern "C" __declspec(noinline) void func_8018B3A8(CMenuShopSell* self);
extern "C" __declspec(noinline) void func_8018B420(CMenuShopSell* self);
extern "C" __declspec(noinline) void func_8018B470(CMenuShopSell* self);
extern "C" __declspec(noinline) void func_8018B658(CMenuShopSell* self);

// Base/subobject ctors (retail keeps the unmangled names at these call sites).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 id);
extern "C" void __ct__CItemBoxGrid(CItemBoxGrid* self, u32 a, u32 b, u32 c, u32 d);

// vtable / PMF shared data (MWCC does not mangle global-scope data names).
extern u32 __ptmf_null[3];            // null pointer-to-member-function constant
// Temporary (CProcess) vtable, pre-PMF copy; overwritten by the composite vtable.
extern char lbl_eu_8052BF70[];
// CMenuShopSell composite vtable; IScnRender sub-vtable at +0x24.
extern char lbl_eu_80532650[];

/* Local shim for cf::CfPadData - only the flag words func_8018B470 reads
 * (CfPadData.hpp owns the full type but pulls CPad/monolib umbrella headers
 * into this TU). 0x00/0x04 are CPad held/pressed flags, 0x104 turbo flags. */
struct ShopSellPadData {
    u32 mHeldFlags;                   // +0x00 (CPad::mHeldButtonFlags)
    u32 mPressedFlags;                // +0x04 (CPad::mPressedButtonFlags)
    u8 _pad08[0x104 - 0x08];          // 0x08..0x103
    u32 mTurboFlags;                  // +0x104 (CfPadData::mTurboPressButtonFlags)
};

// func_8018B470 pad/flag callees (retail unmangled names). func_801CB0FC is
// declared int so the caller compares with cmpwi directly; func_801CDFB4 is
// declared int so the (u8) cast at the tail emits the retail clrlwi.
extern "C" int func_8029A658();
extern "C" ShopSellPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" void func_801CCAF0(CItemBoxGrid* self);
extern "C" int func_801CB0FC(CItemBoxGrid* self);
extern "C" void func_801CC7B0(CItemBoxGrid* self, u32 arg);
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" void func_801CB38C(CItemBoxGrid* self);
extern "C" void func_801CB5F0(CItemBoxGrid* self);
extern "C" void func_801CBA04(CItemBoxGrid* self);
extern "C" void func_801CBDE8(CItemBoxGrid* self);
extern "C" void func_801CC0EC(CItemBoxGrid* self);
extern "C" void func_801CC5DC(CItemBoxGrid* self);
extern "C" void func_801CDC40(CItemBoxGrid* self);
extern "C" void func_801CDEE8(CItemBoxGrid* self);
extern "C" int func_801CDFB4(CItemBoxGrid* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);

// Retail-unmangled callee names (US strips the member manglings for these
// func_ helpers; GetField61 keeps its bare name too). int returns keep the
// caller's `!= 0` as a plain cmpwi (no u8 mask), like retail.
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_801CAE9C(CItemBoxGrid* self);
extern "C" int GetField61(CItemBoxGrid* self);
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8 enable);
extern "C" int func_801C3E34(CBgTex* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" int func_801CB038(CItemBoxGrid* self);
extern "C" void func_801CB28C(CItemBoxGrid* self);
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801CABC8(CItemBoxGrid* self);
extern "C" void func_801CAD8C(CItemBoxGrid* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" int func_8013BE50();
extern "C" void func_80138078__FUl(u32 op);

// Subobject dtors (retail keeps the C++ mangled names) and the game-side
// CProcess dtor wrapper (defined in CMainMenu.cpp).
extern "C" void __dt__12CItemBoxGridFv(CItemBoxGrid* self, int flags);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int flags);
extern "C" void __dt__800FED0C(void* self, int flags);

// nw4r DrawInfo C-ABI ct/dt (retail emits the direct calls; a C++ local would
// virtual-dispatch its scope-exit destructor). func_80137250 keeps its mangled
// nw4r form, so it is declared outside extern "C" for MWCC to re-mangle it.
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(void* self);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(void* self, int dealloc);
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Shop-sell singleton flag (.sbss; cleared by Term).
extern u32 lbl_eu_806642F0;
// Render-gate mode bitfield (.sbss; bit 21 = busy).
extern u32 lbl_eu_80663E28;
