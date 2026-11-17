#pragma once

#include "kyoshin/CItemBoxLine.hpp"

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"

// Forward declarations: the owning scene is kept opaque (same scheme as
// CMenuBattleEnd); the CItemBoxLine storage member is an opaque byte region
// because CItemBoxLine's declared sizeof differs from its retail extent.
class CScn;

// 8-byte pane-colour vector: CItemBoxLine.hpp references this type but relies
// on includers having seen it first (CEquipItemBox.hpp owns the canonical
// definition); defined here because that header cannot be edited.
struct CEquipBoxFourShorts {
    s16 a, b, c, d;
};

/* 8-byte copy unit used by the shop-buy data blob. ShopBuy_CopyData copies the
 * blob's sub-struct members; MWCC turns each struct assignment into the
 * retail's `mtctr` + `lwzu/stwu` counted loop. */
struct ShopBuyDataItem {
    u32 lo;  // +0x0
    u32 hi;  // +0x4
};

struct ShopBuyData24 {
    ShopBuyDataItem v[3];  // 24 bytes
};

struct ShopBuyData144 {
    ShopBuyDataItem v[18];  // 144 bytes
};

struct ShopBuyData128 {
    ShopBuyDataItem v[16];  // 128 bytes
};

/* Shop buy screen data blob (0x159 bytes), copied field-by-field by
 * ShopBuy_CopyData (and filled from Init's stack scratch copies). */
struct ShopBuyData {
    ShopBuyData24 m00;      // 0x00..0x18  (3 x 8B copy loop)
    ShopBuyData144 m18;     // 0x18..0xA8  (18 x 8B copy loop)
    u32 mA8[6];             // 0xA8..0xC0  (6 unrolled words)
    ShopBuyData24 mC0;      // 0xC0..0xD8  (3 x 8B copy loop)
    u8 mD8;                 // 0xD8        (single byte)
    ShopBuyData128 mD9;     // 0xD9..0x159 (16 x 8B copy loop)
};

/* 10-byte / 8-byte byte-packed chunks of the item-box line data region.
 * Init copies these as aggregates so MWCC emits the retail's word-pair loads
 * (unaligned lwz for the odd-offset tab chunk) instead of byte copies. */
struct ShopBuyIBL10 {
    u8 v[10];
};

struct ShopBuyIBL8 {
    u8 v[8];
};

/* 0x10-byte mem-region slot (4-aligned, no ctors so embedding it in the
 * mirror/temp does not generate implicit UnkClass_8045F564 construction). */
struct ShopBuyIBLRegion {
    u32 v[4];
};

/* Mirror of the CItemBoxLine data region (base = vtable + 4), used by Init
 * to copy the stack-temporary layout into the embedded member. Offsets are
 * relative to the first data byte (the vtable at +0 is never copied); the
 * named fields match CItemBoxLine.hpp where the member maps 1:1. */
struct ShopBuyIBLData {
    ShopBuyIBLRegion mRegion04;       // 0x00..0x0F mem region
    ShopBuyIBLRegion mRegion14;       // 0x10..0x1F mem region
    u32 field_0x20[10];               // 0x20..0x47 (10 words)
    u8 field_0x48;                    // 0x48
    u32 field_0x4C;                   // 0x4C (CItemBoxLine::field50)
    u32 field_0x50;                   // 0x50 (pad_54[0..3])
    u8 field_0x54;                    // 0x54 (pad_54[4])
    u8 unk59;                         // 0x55 (CItemBoxLine::unk59)
    ShopBuyIBL10 tabEntries10;        // 0x56..0x5F (tabEntries[9] + tabCount)
    ShopBuyIBL10 pad6410;             // 0x60..0x69 (pad_64[9] + field6D)
    u8 cursors[4][0x18];              // 0x6C..0xCB (4 x 0x18 cursor objects)
    ShopBuyIBLRegion mRegionD4;       // 0xD0..0xDF mem region
    ShopBuyIBLRegion mRegionE4;       // 0xE0..0xEF mem region
    u32 field_0xF0[27];               // 0xF0..0x15B (27 words)
    u8 field_0x15C;                   // 0x15C
    u32 field_0x160;                  // 0x160
    u8 field_0x164;                   // 0x164
    u8 field_0x165;                   // 0x165
    u8 field_0x166;                   // 0x166
    ShopBuyIBL8 chunk168;             // 0x168..0x16F (2-word chunk)
    ShopBuyIBL8 chunk170;             // 0x170..0x177 (2-word chunk)
    u16 field_0x178;                  // 0x178
    ShopBuyData blob17C;              // 0x17C..0x2D4 (ShopBuy_CopyData)
    ShopBuyIBLRegion mRegion2DC;      // 0x2DC..0x2EB mem region
    u32 field_0x2EC[6];               // 0x2EC..0x303 (6 words)
    u8 field_0x304[5];                // 0x304..0x308
    ShopBuyIBLRegion mRegion310;      // 0x310..0x31F mem region
    u32 field_0x320[4];               // 0x320..0x32F (4 words)
    u8 field_0x330[4];                // 0x330..0x333
    f32 field_0x334[5];               // 0x334..0x347 (5 floats)
    u8 field_0x348;                   // 0x348
    ShopBuyIBLRegion mRegion350;      // 0x350..0x35F mem region
    u32 field_0x360[5];               // 0x360..0x373 (5 words)
    u8 field_0x374[18];               // 0x374..0x385
    u8 _pad386[2];                    // 0x386..0x387
    s16 unk38C;                       // 0x388 (CItemBoxLine::unk38C)
    s16 unk38E;                       // 0x38A (CItemBoxLine::unk38E)
    u8 field_0x38C;                   // 0x38C (pad_390[0])
    s16 field_0x38E;                  // 0x38E (pad_390[2..3])
    u16 field_0x390;                  // 0x390 (pad_390[4..5])
    ShopBuyIBL8 chunk392;             // 0x392..0x399 (pad_390[6..13])
    u8 field_0x39A[6];                // 0x39A..0x39F (unk39E..field3A3)
    ShopBuyData144 data3A0;           // 0x3A0..0x42F (CIBLTab entries, 18x8 loop)
    u16 tabCount;                     // 0x430 (CIBLTab.count)
    u8 field_0x432;                   // 0x432
    u8 field_0x433;                   // 0x433
    char str434[0x20];                // 0x434..0x453 (FixStr<32>)
    u32 len454;                       // 0x454
    char str458[0x20];                // 0x458..0x477 (FixStr<32>)
    u32 len4D8;                       // 0x4D8
    u8 _pad478[0x4D8 - 0x478];        // 0x478..0x4D7
    ShopBuyData blob4DC;              // 0x4DC..0x634 (ShopBuy_CopyData)
};

/* Local shim for cf::CfPadData - only the flag words ShopBuy_HandleInput reads
 * (CfPadData.hpp owns the full type but pulls CPad/monolib umbrella headers
 * into this TU). 0x00/0x04 are CPad held/pressed flags, 0x104 turbo flags. */
struct ShopBuyPadData {
    u32 mHeldFlags;                   // +0x00 (CPad::mHeldButtonFlags)
    u32 mPressedFlags;                // +0x04 (CPad::mPressedButtonFlags)
    u8 _pad08[0x104 - 0x08];          // 0x08..0x103
    u32 mTurboFlags;                  // +0x104 (CfPadData::mTurboPressButtonFlags)
};

/* Stack temporary for Init: the CItemBoxLine blob (vtable + data mirror).
 * Declared as a typed object so MWCC folds field accesses to sp-relative
 * loads (retail keeps no base register for the temp). */
struct ShopBuyItemBoxLine {
    u32 mItemBoxVtbl;                 // 0x00..0x03 (vtable)
    ShopBuyIBLData mData;             // 0x04..0x638 data region
};

/*
 * Shop buy menu screen process.
 *
 * Layout (Term / dtor / phase handlers):
 *   0x00: IUICf           -- task base (CTTask<IUICf>) + mField54/mField55
 *   0x54: u8 mField54     -- closing-state flag (written by ShopBuy_MarkClosing)
 *   0x58: IScnRender      -- render-callback subobject (base, vptr)
 *   0x5C: CScn*           -- owning scene (removeRenderCB target in Term)
 *   0x60: u32 mField60    -- ctor arg 3
 *   0x64: CBgTex          -- background layout widget (0x20 bytes)
 *   0x84: CTitleAHelp     -- title/help bar (0x38 bytes)
 *   0xBC: CItemBoxLine    -- item box line storage (0x63C bytes, opaque)
 *  0x6F8: u8 mState       -- phase/state indicator
 *  0x6FC: f32 mFloat6FC   -- float init (lbl_eu_80667A20 pool constant)
 *  total: 0x700
 */
class CMenuShopBuy : public IUICf, public IScnRender {
public:
    CMenuShopBuy(CScn* scene, u32 arg);
    virtual ~CMenuShopBuy();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    CScn* mScene;               // 0x5C: owning scene
    u32 mField60;               // 0x60: ctor arg 3
    CBgTex mBgTex;              // 0x64
    CTitleAHelp mTitleAHelp;    // 0x84
    // CItemBoxLine storage at 0xBC (0x63C bytes, opaque) overlapped with the
    // data-region mirror Init copies into (vtable at +0xBC excluded).
    union {
        u8 mItemBoxLine[0x63C];               // 0xBC..0x6F7 raw storage
        struct {
            u32 mItemBoxVtbl;                 // 0xBC..0xBF (vtable, not copied)
            ShopBuyIBLData mIBLData;          // 0xC0..0x6F5 data mirror
        } mItemBox;
    };
    u8 mState;                  // 0x6F8
    f32 mFloat6FC;              // 0x6FC: float init
};

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// isIdle/BgTex_IsLoaded_3E34/isInitialized/ItemBoxLine_IsReadyFlag/ItemBoxLine_GetSelectReady are declared
// int (not u8) so callers compare with cmpwi directly (no byte mask), matching
// retail.
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void teardown(CTitleAHelp* self);
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" u8 code80135FDC_getByte_64077();

// ShopBuy_HandleInput pad/flag callees (retail unmangled names). ItemBoxLine_IsBusy is
// declared int (not u8) so the caller compares with cmpwi directly.
extern "C" int MenuTutorialIsCreated();
extern "C" ShopBuyPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
extern "C" void MenuShopSellCopyCursor(void* dest, void* src);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dest, void* src);
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" void BgTex_Acquire_3C14(CBgTex* self);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(CScn*, IScnRender*, u32, u32);
extern char lbl_eu_80503B20[];  // split1 .rodata string pool (Init title name)
extern const f32 lbl_eu_80667A24;  // .sdata2 float pool constants (timer inc/clamp)
extern const f32 lbl_eu_80667A28;

// Subobject ctors/dtors (retail keeps the unmangled names).
extern "C" void __dt__12CItemBoxLineFv(CItemBoxLine*, int);

// Subobject draw/update hooks called by Move / cbRenderBefore (retail names
// are unmangled for these helpers too).
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);

// DrawInfo setup helper (C++ mangled name).
void SetWideLocAdjust(nw4r::lyt::DrawInfo* drawInfo);

// Phase handlers (same TU, extern "C" definitions in CMenuShopBuy.cpp) plus
// the factory (retail unmangled name). __declspec(noinline) stops
// -inline auto from inlining the small phase handlers into Move (retail keeps
// real bls; see MWCC_CASES -inline auto same-TU helpers).
extern "C" __declspec(noinline) void ShopBuy_OpenPhase(CMenuShopBuy* self);
extern "C" __declspec(noinline) void ShopBuy_AdvancePhase(CMenuShopBuy* self);
extern "C" __declspec(noinline) void ShopBuy_HandleInput(CMenuShopBuy* self);
extern "C" __declspec(noinline) void ShopBuy_MarkClosing(CMenuShopBuy* self);
extern "C" CMenuShopBuy* ShopBuy_CreateMenu(CProcess* parent, CScn* scene, u32 arg);

// Other retail-named helpers.
// unsigned int (not u32 = unsigned long): must match CItemBoxLine.hpp's
// declaration so the two extern "C" redeclarations merge.
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable);
// CScn.hpp pulls broken monolib umbrella headers (kyoshin headers already
// define an opaque CScn), so the render-callback removal is called through
// its retail C name (same scheme as CMenuBattleEnd / CMenuCollepedia).
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);

// Task-game gate helpers. Declared with C linkage because CTaskGame.hpp pulls
// broken monolib umbrella headers that redefine CScn (same scheme as
// CMenuUpdate). isFlag01Set takes the task-game instance in r3.

// Shared data imports (MWCC does not mangle global-scope data names).
extern "C" int IsMenuState621F0();
extern u32 lbl_eu_80663E28;       // render-gate mode bitfield (.sbss; bit 21 = busy)
extern u32 lbl_eu_806642F8;       // shop-buy singleton flag (cleared by Term)
extern const f32 lbl_eu_80667A20; // .sdata2 float pool constant (ctor mFloat6FC init)
