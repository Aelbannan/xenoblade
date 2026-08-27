#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"

// Forward declarations: the owning scene is kept opaque (same scheme as
// CMenuBattleEnd); the CItemBoxLine storage member is an opaque byte region
// because CItemBoxLine's declared sizeof differs from its retail extent.
class CScn;
class CItemBoxLine;

// 8-byte pane-colour vector: CItemBoxLine.hpp references this type but relies
// on includers having seen it first (CEquipItemBox.hpp owns the canonical
// definition); defined here because that header cannot be edited.
struct CEquipBoxFourShorts {
    s16 a, b, c, d;
};

/* 8-byte copy unit used by the shop-buy data blob. func_8018BE74 copies the
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
 * func_8018BE74 (and filled from Init's stack scratch copies). */
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
    ShopBuyData blob17C;              // 0x17C..0x2D4 (func_8018BE74)
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
    ShopBuyData blob4DC;              // 0x4DC..0x634 (func_8018BE74)
};

/* Local shim for cf::CfPadData - only the flag words func_8018C258 reads
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
    ShopBuyData24 m00;      // 0x00..0x18  (3 x 8B copy loop)
    ShopBuyData144 m18;     // 0x18..0xA8  (18 x 8B copy loop)
    u32 mA8[6];             // 0xA8..0xC0  (6 unrolled words)
    ShopBuyData24 mC0;      // 0xC0..0xD8  (3 x 8B copy loop)
    u8 mD8;                 // 0xD8        (single byte)
    ShopBuyData128 mD9;     // 0xD9..0x159 (16 x 8B copy loop)
};

/*
 * Shop buy menu screen process.
 *
 * Layout (Term / dtor / phase handlers):
 *   0x00: CProcess header -- task-system base (0x3C) + vtable PMF data
 *   0x54: u8 mField54     -- closing-state flag (written by func_8018C59C)
 *   0x58: IScnRender      -- render-callback subobject (member, vptr)
 *   0x5C: CScn*           -- owning scene (removeRenderCB target in Term)
 *   0x60: u32 mField60    -- ctor arg 3
 *   0x64: CBgTex          -- background layout widget (0x20 bytes)
 *   0x84: CTitleAHelp     -- title/help bar (0x38 bytes)
 *   0xBC: CItemBoxLine    -- item box line storage (0x63C bytes, opaque)
 *  0x6F8: u8 mState       -- phase/state indicator
 *  0x6FC: f32 mFloat6FC   -- float init (lbl_eu_80667A20 pool constant)
 *  total: 0x700
 */
class CMenuShopBuy : public CProcess {
public:
    virtual ~CMenuShopBuy();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // CProcess base occupies 0x00-0x3B; vtable pointer-to-member data follows.
    u32 ptmf0[3];               // 0x3C-0x47: vtable pointer-to-member data
    u32 ptmf1[3];               // 0x48-0x53: vtable pointer-to-member data
    u8 mField54;                // 0x54: closing-state flag
    u8 mField55;                // 0x55
    u8 _pad56[2];               // 0x56-0x57
    IScnRender mIScnRender;     // 0x58: render-callback subobject (vptr)
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
    u8 _pad6F9[3];              // 0x6F9-0x6FB
    f32 mFloat6FC;              // 0x6FC: float init
};

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// isIdle/func_801C3E34/func_801C4114/func_801ED800/func_801ED774 are declared
// int (not u8) so callers compare with cmpwi directly (no byte mask), matching
// retail.
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);
extern "C" int func_801C3E34(CBgTex* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" int func_801ED800(CItemBoxLine* self);
extern "C" u8 code80135FDC_getByte_64077();
extern "C" int func_801ED774(CItemBoxLine* self);
extern "C" void func_801ED618(CItemBoxLine* self);
extern "C" void func_801ED864(CItemBoxLine* self);

// func_8018C258 pad/flag callees (retail unmangled names). func_801ED808 is
// declared int (not u8) so the caller compares with cmpwi directly.
extern "C" int func_8029A658();
extern "C" ShopBuyPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
extern "C" int func_801ED808(CItemBoxLine* self);
extern "C" void func_801EECE0(CItemBoxLine* self, u32 arg);
extern "C" void func_801EECE8(CItemBoxLine* self);
extern "C" void func_801EED6C(CItemBoxLine* self);
extern "C" void func_801EE788(CItemBoxLine* self);
extern "C" void func_801EE684(CItemBoxLine* self);
extern "C" void func_801ED97C(CItemBoxLine* self);
extern "C" void func_801EDC94(CItemBoxLine* self);
extern "C" void func_801EDF40(CItemBoxLine* self);
extern "C" void func_801EE228(CItemBoxLine* self);
extern "C" void func_801EE448(CItemBoxLine* self);
extern "C" void func_801EEDF8(CItemBoxLine* self);
extern "C" void func_801EF050(CItemBoxLine* self);
extern "C" u8 func_801EF0EC(CItemBoxLine* self);
extern "C" void func_801EDA08(CItemBoxLine* self);
extern "C" void func_801EDA4C(CItemBoxLine* self, u8 val);
extern "C" void func_801ED31C(CItemBoxLine* self);
extern "C" void func_8018B0FC(void* dest, void* src);
extern "C" void __ct__UnkClass_8011C974(void* dest, void* src);
extern "C" char* func_80136190(const void* a, const void* b, int id);
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
extern "C" void func_801C3C14(CBgTex* self);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(CScn*, IScnRender*, u32, u32);
extern char lbl_eu_80503B20[];  // split1 .rodata string pool (Init title name)
extern const f32 lbl_eu_80667A24;  // .sdata2 float pool constants (timer inc/clamp)
extern const f32 lbl_eu_80667A28;

// Subobject ctors/dtors (retail keeps the unmangled names; __dt__800FED0C is
// the game-side CProcess dtor wrapper defined in CMainMenu.cpp).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __ct__CItemBoxLine(CItemBoxLine* self, u32 a, u32 b);
extern "C" void __dt__12CItemBoxLineFv(CItemBoxLine*, int);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp*, int);
extern "C" void __dt__6CBgTexFv(CBgTex*, int);
extern "C" void __dt__800FED0C(void*, int);

// Subobject draw/update hooks called by Move / cbRenderBefore (retail names
// are unmangled for these helpers too).
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801ED3E8(CItemBoxLine* self);
extern "C" void func_801ED4FC(CItemBoxLine* self, nw4r::lyt::DrawInfo* drawInfo);

// DrawInfo raw-storage ct/dt (C-ABI so the scope-exit dtor is not
// virtual-dispatched) and the DrawInfo setup helper (C++ mangled name).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(void*);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(void*, int);
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Phase handlers (same TU, extern "C" definitions in CMenuShopBuy.cpp) plus
// the factory and ctor (retail unmangled names). __declspec(noinline) stops
// -inline auto from inlining the small phase handlers into Move (retail keeps
// real bls; see MWCC_CASES -inline auto same-TU helpers).
extern "C" __declspec(noinline) void func_8018C190(CMenuShopBuy* self);
extern "C" __declspec(noinline) void func_8018C208(CMenuShopBuy* self);
extern "C" __declspec(noinline) void func_8018C258(CMenuShopBuy* self);
extern "C" __declspec(noinline) void func_8018C59C(CMenuShopBuy* self);
extern "C" CMenuShopBuy* __ct__CMenuShopBuy(CMenuShopBuy* self, CScn* scene, u32 arg);
extern "C" CMenuShopBuy* func_8018C104(CProcess* parent, CScn* scene, u32 arg);

// Other retail-named helpers.
// unsigned int (not u32 = unsigned long): must match CItemBoxLine.hpp's
// declaration so the two extern "C" redeclarations merge.
extern "C" void playUISound__FUl(unsigned int op);
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable);
// CScn.hpp pulls broken monolib umbrella headers (kyoshin headers already
// define an opaque CScn), so the render-callback removal is called through
// its retail C name (same scheme as CMenuBattleEnd / CMenuCollepedia).
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);

// Task-game gate helpers. Declared with C linkage because CTaskGame.hpp pulls
// broken monolib umbrella headers that redefine CScn (same scheme as
// CMenuUpdate). isFlag01Set takes the task-game instance in r3.
extern "C" void* getInstance__9CTaskGameFv();
extern "C" bool isFlag01Set__9CTaskGameFv(void* taskGame);

// Shared data imports (MWCC does not mangle global-scope data names).
extern "C" int func_8013BE50();
extern u32 lbl_eu_80663E28;       // render-gate mode bitfield (.sbss; bit 21 = busy)
extern u32 lbl_eu_806642F8;       // shop-buy singleton flag (cleared by Term)
extern u32 __ptmf_null[3];        // null pointer-to-member-function constant
extern char lbl_eu_8052BF70[];    // temporary (CProcess) vtable, pre-PMF copy
extern char lbl_eu_805326C0[];    // CMenuShopBuy composite vtable; IScnRender sub-vtable at +0x24
extern const f32 lbl_eu_80667A20; // .sdata2 float pool constant (ctor mFloat6FC init)
