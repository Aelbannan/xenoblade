#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuItem.hpp"

#include "kyoshin/menu/CMenuGCItem.hpp"
#include "kyoshin/cf/CInfoCf.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include "kyoshin/CTaskGame.hpp"
#include <revolution/gx/GXPixel.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"

// ---- Declarations private to this unit (C-style imports and the copy views
// used by Init until CItemBoxGrid and its members are real classes). ----

// C-linkage imports (retail-unmangled symbols: US strips the member
// manglings for the func_ helpers, and the CfGameManager statics carry their
// C++ mangling inside the identifier). Declared extern "C" so MWCC emits the
// retail reloc names verbatim (same scheme as CfTFile.hpp / code_800A75FC.hpp).
extern "C" {
void setPresentationFlag__Q22cf13CfGameManagerFv(u32 enable);
void BgTex_Release_3D9C(CBgTex* self);
void teardown(CTitleAHelp* self);
void CfRes_getInstPtrBC();
int KyoshinHeap_GetActive54();
u32 KyoshinHeap_GetSize500000();
void __dt__8CProcessFv(CProcess* self, int flags);
// CMenuItem dtor (retail D1, manual member/base destruction - see the cpp)
// and the embedded widget dtors it drives, all with the retail CW names.
void __dt__12CItemBoxGridFv(CItemBoxGrid* self, int flags);
int BgTex_Acquire_3C14(CBgTex* self);
void BgTex_SetupRegion_3A24(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
}

// CInfoCf vtable (retail .data:0x805309B0), the UI-state singleton pointer
// (sbss; instance stored by __ct__cf_CInfoCf, cleared by the dtor), and the
// Menu singleton pointer (sbss), cleared by CMenuItem::Term.

// Title/help name-string base used by CMenuItem::Init (retail rodata).

/*
 * Body-copy view for the C-ABI copy helpers in this TU. Each helper copies a
 * fixed slice of a larger object, skipping the member vtable at +0x00.
 * Layouts recovered from the retail copy ASM (InfoCfCopyObj60 / 801672E4 /
 * 801671D4 / 8016742C all emit load-all-then-store-all word/byte copies).
 * The container structs let the copy be written as a member struct assignment
 * (dst->body = src->body), which reproduces the retail register allocation;
 * a plain `*dst = *src` over a padded struct would copy the vtable slot
 * (and, for the SysWin body, the 0x29-0x2B alignment gap).
 */
struct CInfoCfCopy60 {
    u32 field_04;
    u32 field_08;
    u32 field_0C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u8 field_20;
    u8 field_21;
    u8 field_22;
    u8 field_23;
    u8 field_24;
};

struct CInfoCfObj60 {
    void* vtable;       // 0x00 (never copied)
    CInfoCfCopy60 body; // 0x04
};

struct CInfoCfCopyE4 {
    u32 field_04;
    u32 field_08;
    u32 field_0C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u8 field_24;
    u8 field_25;
    u8 field_26;
    u8 field_27;
};

struct CInfoCfObjE4 {
    void* vtable;       // 0x00 (never copied)
    CInfoCfCopyE4 body; // 0x04
};

struct CInfoCfCopyD4 {
    u32 field_04;
    u32 field_08;
    u32 field_0C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u8 field_2C;
    u8 field_2D;
    u8 field_2E;
    u8 field_2F;
    u8 field_30;
};

struct CInfoCfObjD4 {
    void* vtable;       // 0x00 (never copied)
    CInfoCfCopyD4 body; // 0x04
};

// CSysWin body copied by InfoCfCopySysWin (0x04..0x39). The 0x29-0x2B gap is
// compiler-inserted alignment padding between field_28 (u8) and field_2C
// (u32), which MWCC struct assignment does not copy - matching retail.
struct CInfoCfSysWinBody {
    u32 field_04;  // UnkClass_8045F564 mMemRegion.unk0
    u32 field_08;  // mMemRegion.unk4
    u32 field_0C;  // mMemRegion.unk8
    u32 field_10;  // mMemRegion.unkC
    u32 field_14;  // CFileHandle* mFileHandle
    u32 field_18;  // CTagProcessor* mTagProcessor
    u32 field_1C;  // ArcResourceAccessor* mArcAccessor
    u32 field_20;  // nw4r::lyt::Layout* mLayout
    u32 field_24;  // nw4r::lyt::AnimTransform* mAnimTrans
    u8 field_28;
    u32 field_2C;
    u32 field_30;
    u8 field_34;
    u8 field_35;
    u8 field_36;
    u8 field_37;
    u8 field_38;
    u8 field_39;
};

struct CInfoCfObjSysWin {
    void* vtable;           // 0x00 (never copied)
    CInfoCfSysWinBody body; // 0x04
};

// 8-byte table entry. The original layout is packed (u64 elements at a
// 4-mod-8 offset), so the wrapper forces 4-byte alignment; MWCC still copies
// the u64 member with the base-4 lwzu/stwu loop convention.
// Two u32s (not u64): a u64 member 8-aligns the copy view and pads +4
// after the vtable. A 4-aligned pair keeps the object body at +0x04.
__declspec(align(4)) struct CInfoCfE48Entry {
    u32 lo;
    u32 hi;
};

/*
 * Body-copy view for InfoCfCopyObjE48 (0x04..0xEF). Split around the 0x2C..0x2F
 * hole retail does not copy. Head is +0x04..+0x2B; tail is +0x30..+0xEE
 * with a 16-entry 8-byte table at +0x6C (lwzu/stwu base at 0x68 = 0x6C-4).
 */
struct CInfoCfCopyE48Head {
    u32 field_04;
    u32 field_08;
    u32 field_0C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u8 field_28;
    u8 field_29;
    u8 field_2A;
    u8 field_2B;
};

struct CInfoCfCopyE48Tail {
    u32 field_30;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
    u32 field_40;
    u32 field_44;
    u32 field_48;
    u32 field_4C;
    u8 field_50;
    u8 field_51;
    u8 field_52;
    u8 field_53;
    f32 field_54;
    f32 field_58;
    f32 field_5C;
    f32 field_60;
    f32 field_64;
    u8 field_68;
    CInfoCfE48Entry field_6C[16];
    u8 field_EC;
    u8 field_ED;
    u8 field_EE;
};

struct CInfoCfObjE48 {
    void* vtable;              // 0x00 (never copied)
    CInfoCfCopyE48Head head;   // 0x04 .. 0x2B
    u32 gap_2C;                // 0x2C .. 0x2F (not assigned)
    CInfoCfCopyE48Tail tail;   // 0x30 .. 0xEE
};

/*
 * Body-copy view for InfoCfCopyObjF80 (0x04..0x208). The retail copy is two
 * aggregate statements: a field-by-field head (0x04..0xC4, 35 streamed words +
 * bytes + five 8-byte pair chunks) and a middle block (0xC8..0x208) that
 * starts with the 18-entry u64 counted loop, continues with paired words and
 * the 0x188 byte, and ends with the 16-entry u64 loop at the odd +0x189.
 * Splitting the object this way reproduces the retail prologue scheduling:
 * loop 1's li/addi/addi/mtctr sits AFTER the head copy, while loop 2's
 * prologue is hoisted into the 0x158 pair - a single-statement copy hoists
 * both, which does not match.
 */
// 8-byte chunk type for the paired word regions in InfoCfCopyObjF80's blob.
// Retail copies each 8-byte run as a 2-load/2-reverse-store pair (lwz r5 / lwz r0
// / stw r0 / stw r5); a 4-aligned 8-byte struct reproduces both the offset and
// the r5-first allocation (u64 allocates r0-first; u8[8] is 1-aligned).
struct CInfoCfPair {
    u32 lo;
    u32 hi;
};

// Tail u64 table of InfoCfCopyObjF80's blob: retail packs it at the odd +0x189
// (loop base +0x185), so the type must be 1-aligned. Declared at file scope
// (MWCC ignores #pragma pack inside a struct body) and used as a member so the
// struct assignment still copies it with the counted lwzu/stwu loop.
#pragma pack(push, 1)
struct CInfoCfF80Tail {
    u64 field_189[16];
};
#pragma pack(pop)

// The head (0x04..0xC4) is naturally 4-aligned; the mid block (0xC8..0x208)
// starts with the 18-entry u64 table, so it is naturally 8-aligned and still
// lands at +0xC8. Natural alignment keeps every field gap (0x91-0x93 etc. are
// NOT copied) without packing.
#pragma pack(push, 4)
struct CInfoCfHeadF80 {
    u32 field_04;
    u32 field_08;
    u32 field_0C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 field_2C;
    u32 field_30;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
    u32 field_40;
    u32 field_44;
    u32 field_48;
    u32 field_4C;
    u32 field_50;
    u32 field_54;
    u32 field_58;
    u32 field_5C;
    u32 field_60;
    u32 field_64;
    u32 field_68;
    u32 field_6C;
    u32 field_70;
    u32 field_74;
    u32 field_78;
    u32 field_7C;
    u32 field_80;
    u32 field_84;
    u32 field_88;
    u32 field_8C;
    u8 field_90;
    u32 field_94;
    u8 field_98;
    u8 field_99;
    u8 field_9A;
    CInfoCfPair field_9C;  // 8-byte chunk (retail 2-load/2-reverse-store pair)
    CInfoCfPair field_A4;
    u16 field_AC;
    u8 field_AE;
    CInfoCfPair field_B0;
    CInfoCfPair field_B8;
    u64 field_C0;   // last word pair before loop 1
};

struct CInfoCfMidF80 {
    u64 field_C8[18];   // +0xC8..+0x157 (retail counted loop, li r0, 0x12)
    CInfoCfPair field_158;
    u32 field_160;
    CInfoCfPair field_164;
    u32 field_16C;
    CInfoCfPair field_170;
    CInfoCfPair field_178;
    CInfoCfPair field_180;
    u8 field_188;
    CInfoCfF80Tail tail;  // 1-aligned pack(1) type -> sits at +0x189 (odd)
};
#pragma pack(pop)

struct CInfoCfObjF80 {
    void* vtable;         // 0x00 (never copied)
    CInfoCfHeadF80 head;  // 0x04
    CInfoCfMidF80 mid;    // 0xC8
};

/*
 * Body-copy view for InfoCfCopyObj368 (0x04..0x42). The two words at +0x39 /
 * +0x3D are packed at odd offsets (retail reads them with plain lwz), so the
 * aligned head is a normal struct and the packed tail is a raw byte slice on
 * the object (copied with word casts by the helper, not by struct assignment).
 */
struct CInfoCfCopy368 {
    u32 field_04;
    u32 field_08;
    u32 field_0C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 field_2C;
    u8 field_30;
    u8 field_31;
    u8 field_32;
    u8 field_33;
    u16 field_34;
    u8 field_36;
    u8 field_37;
    u8 field_38;
};

struct CInfoCfObj368 {
    void* vtable;       // 0x00 (never copied)
    CInfoCfCopy368 body; // 0x04..0x39
    u8 tail[9];         // 0x39..0x42 packed tail (words + final byte)
};

/*
 * Raw body view for InfoCfCopyObj4D0 (0x00..0x44C0, vtable slot included).
 * The object is a packed blob: aligned u64 tables start at +0x00, +0x2825,
 * +0x28A5, +0x2CAC, +0x34B4, +0x3CBC (the last three are 4-mod-8, so no
 * natural struct can express them) with packed string/data runs in between.
 * InfoCfCopyObj4D0 copies it region-by-region with counted lwzu/stwu loops.
 */
struct CInfoCfObj4D0 {
    void* vtable;       // 0x00 (copied by the bulk loop in this helper)
    u8 body[0x44C0];    // 0x00..0x44C0 raw packed view
};

/*
 * CItemBoxGrid body-copy view used by CMenuItem::Init. The retail Init
 * constructs a full CItemBoxGrid temporary on the stack, copies it into the
 * embedded member region-by-region (inline head/tail copies + the func_80166xxx
 * copy helpers), then destroys the temporary. The view maps the body (+0x04..
 * +0x4A0B, vtable slot at +0 skipped) onto the helper regions.
 */
// Packed tail of the grid head: 2-load/2-reverse-store pair + lone word + bytes.
#pragma pack(push, 1)
struct CItemBoxGridHeadTail {
    CInfoCfPair pair62;  // 0x62..0x69
    u32 field_6A;        // 0x6A
    u8 field_6E;         // 0x6E
    u8 field_6F;         // 0x6F
};
#pragma pack(pop)

// Grid head (body +0x04..+0x6F): 20 streamed words, byte, 2 words, 2 bytes,
// then the 1-aligned packed tail (retail copies it inline).
struct CItemBoxGridHead {
    u32 field_04;
    u32 field_08;
    u32 field_0C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 field_2C;
    u32 field_30;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
    u32 field_40;
    u32 field_44;
    u32 field_48;
    u32 field_4C;
    u32 field_50;
    u8 field_54;
    u32 field_58;
    u32 field_5C;
    u8 field_60;
    u8 field_61;
    CItemBoxGridHeadTail tail;  // +0x62 (1-aligned)
};

// Grid tail region (body +0x524..+0x549): byte/halfword run + pair + word +
// bytes, copied inline by Init.
struct CItemBoxGridTail524 {
    u8 field_524;
    u8 field_525;
    u8 field_526;
    u8 field_527;
    u8 field_528;
    u8 field_529;
    u16 field_52A;
    u8 field_52C;
    u8 field_52D;
    u16 field_52E;
    u8 field_530;
    CInfoCfPair pair534;  // +0x534 (4-aligned)
    u32 field_53C;
    u8 field_540;
    u8 field_541;
    u8 field_542;
    u8 field_543;
    u8 field_544;
    u8 field_545;
    u8 field_546;
    u8 field_547;
    u8 field_548;
    u8 field_549;
};

// Whole-body view: head/tail inline regions plus the func_80166xxx helper
// regions (each helper takes an object view whose +4 body slice it copies).
struct CItemBoxGridBodyView {
    CItemBoxGridHead head;        // 0x04..0x6F (inline)
    u8 _70[0xE8 - 0x70];          // 0x70..0xE7 (not copied directly)
    CInfoCfObjE48 objE8;          // 0xE8  InfoCfCopyObjE48
    CInfoCfObjF80 obj1D8;         // 0x1D8 InfoCfCopyObjF80
    CInfoCfObjD4 obj3E4;          // 0x3E4 InfoCfCopyObjD4
    CInfoCfObj60 obj418;          // 0x418 InfoCfCopyObj60
    CInfoCfObjE4 obj440;          // 0x440 InfoCfCopyObjE4
    CInfoCfObj368 obj468;         // 0x468 InfoCfCopyObj368
    CInfoCfObjSysWin obj4AC;      // 0x4AC InfoCfCopySysWin
    CInfoCfObjSysWin obj4E8;      // 0x4E8 InfoCfCopySysWin
    CItemBoxGridTail524 tail524;  // 0x524..0x549 (inline)
    u8 _54A[0x54C - 0x54A];       // 0x54A..0x54B
    CInfoCfObj4D0 obj54C;         // 0x54C InfoCfCopyObj4D0 (0x44C0 bytes)
};

// Same-unit import: pad-input/advance helper called from CMenuItem::Move
// case 2 (body lives in kyoshin/menu/CMenuItem.cpp).
extern "C" void CMenuItem_HandleInput(CMenuItem* self);

// CItemBoxGrid C-ABI helper (body lives in kyoshin/CItemBoxGrid.cpp); no
// header declares the retail-unmangled name, so declare it here.

// Render-gate mode bitfield (.sbss; bit 21 = busy), shared with the other
// menu-screen TUs.
extern u32 lbl_eu_80663E28;

// Menu/task-flow imports previously pulled in via CSkipTimer.hpp (that header
// carries a conflicting __ct__7CSysWinFUc declaration, so they are declared here).
extern u32 CMainMenu_GetInstancePtr();
extern void ArtsInfo_SetReadyFlag();
extern void playUISound(u32);


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
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
extern "C" void beginClose(CTitleAHelp* self);
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
void SetWideLocAdjust(nw4r::lyt::DrawInfo* drawInfo);
extern "C" int IsMenuState621F0();
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);


// Global: UI state flag read by InfoCfGetUiFlags.

// Body-copy helpers defined below in this TU (called by CMenuItem::Init).
// extern "C": the retail reloc names are unmangled (same convention as
// ItemMenu_IsPresent in kyoshin/menu/CMenuItem.cpp).
extern "C" void InfoCfCopyObjE48(CInfoCfObjE48* dst, const CInfoCfObjE48* src);
extern "C" void InfoCfCopyObjF80(CInfoCfObjF80* dst, const CInfoCfObjF80* src);
extern "C" void InfoCfCopyObjD4(CInfoCfObjD4* dst, const CInfoCfObjD4* src);
extern "C" void InfoCfCopyObj60(CInfoCfObj60* dst, const CInfoCfObj60* src);
extern "C" void InfoCfCopyObjE4(CInfoCfObjE4* dst, const CInfoCfObjE4* src);
extern "C" void InfoCfCopyObj368(CInfoCfObj368* dst, const CInfoCfObj368* src);
extern "C" void InfoCfCopyObj4D0(CInfoCfObj4D0* dst, const CInfoCfObj4D0* src);

// The grid is opaque storage (see CMenuItem.hpp), so its destructor is called
// by name; the other members and the bases are destroyed by the compiler.
CMenuItem::~CMenuItem() {
    __dt__12CItemBoxGridFv(grid(), -1);
}

void CMenuItem::Init() {
    // Gate the game-manager handoff until the menu flag is clear (retail
    // reads mField4AC5 before any widget setup).
    if (mField4AC5 == 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(1);
    }

    // --- Re-initialise the embedded CBgTex via a temporary ---
    mBgTex = CBgTex(0);

    if (BgTex_Acquire_3C14(&mBgTex) != 0) {
        BgTex_SetupRegion_3A24(&mBgTex);
    }

    // --- Re-initialise the embedded CTitleAHelp via a temporary ---
    char* name = BdatTouchStringCell("MNU_item", "name", 1);
    mTitleAHelp = CTitleAHelp(name, 1);
    mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded CItemBoxGrid via a temporary ---
    u8 tempGrid[0x4a0c];
    __ct__12CItemBoxGridFUcUsUlUc(reinterpret_cast<CItemBoxGrid*>(tempGrid),
                       mField4AC5 ? 3 : 0, 0, (u32)mScene, mField4AC6);

    // Copy the whole temporary into the member, region by region (retail
    // split schedule: inline head/tail + the func_80166xxx copy helpers).
    CItemBoxGridBodyView* dstBody =
        reinterpret_cast<CItemBoxGridBodyView*>(grid());
    const CItemBoxGridBodyView* srcBody =
        reinterpret_cast<const CItemBoxGridBodyView*>(tempGrid);
    // Head: 20 streamed words + byte + 2 words + 2 bytes + packed tail
    // (member-wise so MWCC inlines them; a whole-struct assignment would
    // emit an __as__ call).
    CItemBoxGridHead* dh = &dstBody->head;
    const CItemBoxGridHead* sh = &srcBody->head;
    dh->field_04 = sh->field_04;
    dh->field_08 = sh->field_08;
    dh->field_0C = sh->field_0C;
    dh->field_10 = sh->field_10;
    dh->field_14 = sh->field_14;
    dh->field_18 = sh->field_18;
    dh->field_1C = sh->field_1C;
    dh->field_20 = sh->field_20;
    dh->field_24 = sh->field_24;
    dh->field_28 = sh->field_28;
    dh->field_2C = sh->field_2C;
    dh->field_30 = sh->field_30;
    dh->field_34 = sh->field_34;
    dh->field_38 = sh->field_38;
    dh->field_3C = sh->field_3C;
    dh->field_40 = sh->field_40;
    dh->field_44 = sh->field_44;
    dh->field_48 = sh->field_48;
    dh->field_4C = sh->field_4C;
    dh->field_50 = sh->field_50;
    dh->field_54 = sh->field_54;
    dh->field_58 = sh->field_58;
    dh->field_5C = sh->field_5C;
    dh->field_60 = sh->field_60;
    dh->field_61 = sh->field_61;
    dh->tail.pair62 = sh->tail.pair62;
    dh->tail.field_6A = sh->tail.field_6A;
    dh->tail.field_6E = sh->tail.field_6E;
    dh->tail.field_6F = sh->tail.field_6F;
    InfoCfCopyObjE48(&dstBody->objE8, &srcBody->objE8);
    InfoCfCopyObjF80(&dstBody->obj1D8, &srcBody->obj1D8);
    InfoCfCopyObjD4(&dstBody->obj3E4, &srcBody->obj3E4);
    InfoCfCopyObj60(&dstBody->obj418, &srcBody->obj418);
    InfoCfCopyObjE4(&dstBody->obj440, &srcBody->obj440);
    InfoCfCopyObj368(&dstBody->obj468, &srcBody->obj468);
    *reinterpret_cast<CSysWin*>(&dstBody->obj4AC) = *reinterpret_cast<const CSysWin*>((void*)&srcBody->obj4AC);
    *reinterpret_cast<CSysWin*>(&dstBody->obj4E8) = *reinterpret_cast<const CSysWin*>((void*)&srcBody->obj4E8);
    dstBody->tail524.field_524 = srcBody->tail524.field_524;
    dstBody->tail524.field_525 = srcBody->tail524.field_525;
    dstBody->tail524.field_526 = srcBody->tail524.field_526;
    dstBody->tail524.field_527 = srcBody->tail524.field_527;
    dstBody->tail524.field_528 = srcBody->tail524.field_528;
    dstBody->tail524.field_529 = srcBody->tail524.field_529;
    dstBody->tail524.field_52A = srcBody->tail524.field_52A;
    dstBody->tail524.field_52C = srcBody->tail524.field_52C;
    dstBody->tail524.field_52D = srcBody->tail524.field_52D;
    dstBody->tail524.field_52E = srcBody->tail524.field_52E;
    dstBody->tail524.field_530 = srcBody->tail524.field_530;
    dstBody->tail524.pair534 = srcBody->tail524.pair534;
    dstBody->tail524.field_53C = srcBody->tail524.field_53C;
    dstBody->tail524.field_540 = srcBody->tail524.field_540;
    dstBody->tail524.field_541 = srcBody->tail524.field_541;
    dstBody->tail524.field_542 = srcBody->tail524.field_542;
    dstBody->tail524.field_543 = srcBody->tail524.field_543;
    dstBody->tail524.field_544 = srcBody->tail524.field_544;
    dstBody->tail524.field_545 = srcBody->tail524.field_545;
    dstBody->tail524.field_546 = srcBody->tail524.field_546;
    dstBody->tail524.field_547 = srcBody->tail524.field_547;
    dstBody->tail524.field_548 = srcBody->tail524.field_548;
    dstBody->tail524.field_549 = srcBody->tail524.field_549;
    InfoCfCopyObj4D0(&dstBody->obj54C, &srcBody->obj54C);
    __dt__12CItemBoxGridFv(reinterpret_cast<CItemBoxGrid*>(tempGrid), -1);

    ((CItemBoxGrid*)(grid()))->ClearListSlots();

    if (mField4AC5 == 0) {
        // Single-player item list ordering.
        ((CItemBoxGrid*)(grid()))->PushToList(2);
        ((CItemBoxGrid*)(grid()))->PushToList(4);
        ((CItemBoxGrid*)(grid()))->PushToList(5);
        ((CItemBoxGrid*)(grid()))->PushToList(6);
        ((CItemBoxGrid*)(grid()))->PushToList(7);
        ((CItemBoxGrid*)(grid()))->PushToList(8);
        ((CItemBoxGrid*)(grid()))->PushToList(0xb);
        ((CItemBoxGrid*)(grid()))->PushToList(0xd);
        ((CItemBoxGrid*)(grid()))->PushToList(0xc);
        ((CItemBoxGrid*)(grid()))->PushToList(3);
        ((CItemBoxGrid*)(grid()))->PushToList(9);
        ((CItemBoxGrid*)(grid()))->PushToList(0xa);
    } else {
        ((CItemBoxGrid*)(grid()))->PushToList(mField4AC5);
    }
    ((CItemBoxGrid*)(grid()))->LoadItemBoxFiles();

    mScene->addRenderCB(this, 0xd, 0);
}

// Copy a 0xEB-byte body slice (+0x04..+0xEF). The 16-entry 8-byte table at
// +0x6C is copied by a counted lwzu/stwu loop (element base registers walk
// from 0x68 = 0x6C-4).
extern "C" void InfoCfCopyObjE48(CInfoCfObjE48* dst, const CInfoCfObjE48* src) {
    dst->head = src->head;
    dst->tail = src->tail;
}

extern "C" void InfoCfCopyObjF80(CInfoCfObjF80* dst, const CInfoCfObjF80* src) {
    dst->head = src->head;
    dst->mid = src->mid;
}

// Copy a 0x21-byte body slice (7 words + 5 bytes at +0x04..+0x24), skipping
// the member vtable at +0x00. Member struct assignment reproduces the retail
// load-all-then-store-all shape and register allocation.
extern "C" void InfoCfCopyObj60(CInfoCfObj60* dst, const CInfoCfObj60* src) {
    dst->body = src->body;
}

// Copy an 8-word + 4-byte body slice (+0x04..+0x27).
extern "C" void InfoCfCopyObjE4(CInfoCfObjE4* dst, const CInfoCfObjE4* src) {
    dst->body = src->body;
}

// Copy a 10-word + 5-byte body slice (+0x04..+0x30).
extern "C" void InfoCfCopyObjD4(CInfoCfObjD4* dst, const CInfoCfObjD4* src) {
    dst->body = src->body;
}

// Copy a 0x3E-byte body slice (+0x04..+0x42). Retail load-all-then-store-all
// with one temp per field (22 temps fit the register file); the packed tail
// words at +0x39/+0x3D (odd offsets) are read with word casts from the raw
// tail slice.
extern "C" void InfoCfCopyObj368(CInfoCfObj368* dst, const CInfoCfObj368* src) {
    u32 v04 = src->body.field_04;
    u32 v08 = src->body.field_08;
    u32 v0C = src->body.field_0C;
    u32 v10 = src->body.field_10;
    u32 v14 = src->body.field_14;
    u32 v18 = src->body.field_18;
    u32 v1C = src->body.field_1C;
    u32 v20 = src->body.field_20;
    u32 v24 = src->body.field_24;
    u32 v28 = src->body.field_28;
    u32 v2C = src->body.field_2C;
    u8 v30 = src->body.field_30;
    u8 v31 = src->body.field_31;
    u8 v32 = src->body.field_32;
    u8 v33 = src->body.field_33;
    u16 v34 = src->body.field_34;
    u8 v36 = src->body.field_36;
    u8 v37 = src->body.field_37;
    u8 v38 = src->body.field_38;
    u32 v39 = *(u32*)&src->tail[0];
    u32 v3D = *(u32*)&src->tail[4];
    u8 v41 = src->tail[8];
    dst->body.field_04 = v04;
    dst->body.field_08 = v08;
    dst->body.field_0C = v0C;
    dst->body.field_10 = v10;
    dst->body.field_14 = v14;
    dst->body.field_18 = v18;
    dst->body.field_1C = v1C;
    dst->body.field_20 = v20;
    dst->body.field_24 = v24;
    dst->body.field_28 = v28;
    dst->body.field_2C = v2C;
    dst->body.field_30 = v30;
    dst->body.field_31 = v31;
    dst->body.field_32 = v32;
    dst->body.field_33 = v33;
    dst->body.field_34 = v34;
    dst->body.field_36 = v36;
    dst->body.field_37 = v37;
    dst->body.field_38 = v38;
    *(u32*)&dst->tail[0] = v39;
    *(u32*)&dst->tail[4] = v3D;
    dst->tail[8] = v41;
}

// Copy the 0x44C0-byte info-screen object (vtable slot included) region by
// region, mirroring retail's split schedule: u64 tables via counted lwzu/stwu
// loops (pointer walks keep MWCC from unrolling), then the packed string/data
// run as word/byte copies.
extern "C" void InfoCfCopyObj4D0(CInfoCfObj4D0* dst, const CInfoCfObj4D0* src) {
    u8* d = (u8*)dst;
    const u8* s = (const u8*)src;
    u64* dq = (u64*)d;
    const u64* sq = (const u64*)s;
    for (int i = 0; i < 0x500; i++) {
        *dq++ = *sq++;
    }
    *(u16*)(d + 0x2800) = *(u16*)(s + 0x2800);
    d[0x2802] = s[0x2802];
    d[0x2803] = s[0x2803];
    d[0x2804] = s[0x2804];
    *(u32*)(d + 0x2805) = *(u32*)(s + 0x2805);
    *(u32*)(d + 0x2809) = *(u32*)(s + 0x2809);
    *(u32*)(d + 0x280d) = *(u32*)(s + 0x280d);
    *(u32*)(d + 0x2811) = *(u32*)(s + 0x2811);
    *(u32*)(d + 0x2815) = *(u32*)(s + 0x2815);
    *(u32*)(d + 0x2819) = *(u32*)(s + 0x2819);
    *(u32*)(d + 0x281d) = *(u32*)(s + 0x281d);
    *(u32*)(d + 0x2821) = *(u32*)(s + 0x2821);
    dq = (u64*)(d + 0x2825);
    sq = (const u64*)(s + 0x2825);
    for (int i = 0; i < 0x10; i++) {
        *dq++ = *sq++;
    }
    dq = (u64*)(d + 0x28a5);
    sq = (const u64*)(s + 0x28a5);
    for (int i = 0; i < 0x80; i++) {
        *dq++ = *sq++;
    }
    dq = (u64*)(d + 0x2cac);
    sq = (const u64*)(s + 0x2cac);
    for (int i = 0; i < 0x100; i++) {
        *dq++ = *sq++;
    }
    *(u16*)(d + 0x34ac) = *(u16*)(s + 0x34ac);
    dq = (u64*)(d + 0x34b4);
    sq = (const u64*)(s + 0x34b4);
    for (int i = 0; i < 0x100; i++) {
        *dq++ = *sq++;
    }
    *(u16*)(d + 0x3cb4) = *(u16*)(s + 0x3cb4);
    dq = (u64*)(d + 0x3cbc);
    sq = (const u64*)(s + 0x3cbc);
    for (int i = 0; i < 0x100; i++) {
        *dq++ = *sq++;
    }
    *(u16*)(d + 0x44bc) = *(u16*)(s + 0x44bc);
}

// Tear down the menu screen: wait for the draw callback to finish, remove the
// render callback from the owning scene, release each embedded widget, clear
// the singleton flag, and hand control back to the game manager when the
// menu-state byte is clear.
void CMenuItem::Term() {
    CDeviceVI::waitForDrawDone();

    mScene->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    ((CItemBoxGrid*)(grid()))->UnloadItemBox();

    sInstance = nullptr;
    if (mField4AC5 == 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(0);
    }
}

void CMenuItem::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;

    // Button input: the confirm trigger comes from a different pad bit in
    // co-op vs single-player mode.
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 pressed;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        pressed = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        pressed = (pad->mPressedButtonFlags >> 10) & 1;
    }
    if (pressed != 0) {
        // Close a pending item-box interaction, then mark the menu done.
        if (((CItemBoxGrid*)(grid()))->IsSubWinActive() == 0) {
            if (CMainMenu_GetInstancePtr() != 0) {
                ArtsInfo_SetReadyFlag();
            }
            playUISound(6);
            mState = 4;
            mField54 = 1;
        }
    }

    switch (mState) {
    case 0:
        // Once the bg texture, title bar and item grid are all ready,
        // start the panel intro animations and play the open cue.
        if (BgTex_IsLoaded_3E34(&mBgTex) != 0 && isInitialized(&mTitleAHelp) != 0 &&
            ((CItemBoxGrid*)(grid()))->IsItemBoxReady() != 0) {
            func_801C412C(&mTitleAHelp);
            ((CItemBoxGrid*)(grid()))->InitItemDisplay();
            mState = 1;
            playUISound(0x6d);
        }
        break;
    case 1:
        // Advance to phase 2 once the title bar is idle and the grid ready.
        if (mTitleAHelp.isIdle() != 0 && ((CItemBoxGrid*)(grid()))->GetField61() != 0) {
            mState = 2;
        }
        break;
    case 2:
        CMenuItem_HandleInput(this);
        break;
    case 3:
        // Same idle+ready check, but sets the phase flag at 0x54 instead.
        if (mTitleAHelp.isIdle() != 0 && ((CItemBoxGrid*)(grid()))->GetField61() != 0) {
            mField54 = 1;
        }
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    ((CItemBoxGrid*)(grid()))->UpdateItemBox();
    updateHelp(&mTitleAHelp);
}

void CMenuItem::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    ((CItemBoxGrid*)(grid()))->DrawItemBoxGrid(&drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

/*
 * Menu item-screen singleton factory. If the singleton already exists, returns NULL. Otherwise
 * constructs the process in work memory, registers it under `parent`, stores
 * the singleton pointer and returns it.
 */
CMenuItem* CMenuItem::sInstance;

CMenuItem* CMenuItem::create(CProcess* parent, CScn* scene, u8 flag5, u8 flag6) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuItem* obj = new (CWorkThreadSystem::getWorkMem()) CMenuItem(scene, flag5, flag6);

    sInstance = obj;
    obj->Regist(parent, false);

    return sInstance;
}

unsigned long ItemMenu_IsPresent(void) { return CMenuItem::sInstance != nullptr; }

/* Item-menu main input handler. Syncs the help bar to the grid mode, then
 * decodes the pad: turbo/pressed masks for cancel/page-turn/decide, and
 * pressed-bit positions that depend on the game-manager controller mode
 * (classic vs. others). Dispatches one grid action in priority order. */
void CMenuItem_HandleInput(CMenuItem* self) {
    // Help bar follows the grid's current mode.
    u8 mode = ((CItemBoxGrid*)(self->grid()))->GetPromptState();
    func_801C41E8(&self->mTitleAHelp, mode);

    CfMenuPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    bool up, down;
    int turboCancel, cancel, pageTurn, decide;
    bool leftTrigger, xHeld, yHeld;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        // Classic controller bit layout.
        turboCancel = (pad->mTurboFlags & 0x8004) != 0;
        pageTurn = (pad->mTurboFlags & 0x10008) != 0;
        up = (pad->mPressedFlags & 0x00200000) != 0;
        down = (pad->mPressedFlags & 0x00400000) != 0;
        cancel = (pad->mTurboFlags & 0x2001) != 0;
        decide = (pad->mTurboFlags & 0x4002) != 0;
        leftTrigger = (pad->mPressedFlags & 0x01000000) != 0;
        xHeld = (pad->mPressedFlags & 0x00000200) != 0;
        yHeld = (pad->mPressedFlags & 0x00000400) != 0;
    } else {
        turboCancel = (pad->mTurboFlags & 0x8004) != 0;
        pageTurn = (pad->mTurboFlags & 0x10008) != 0;
        up = (pad->mPressedFlags & 0x00000010) != 0;
        down = (pad->mPressedFlags & 0x00000020) != 0;
        cancel = (pad->mTurboFlags & 0x2001) != 0;
        decide = (pad->mTurboFlags & 0x4002) != 0;
        leftTrigger = (pad->mPressedFlags & 0x00000800) != 0;
        xHeld = (pad->mPressedFlags & 0x00000200) != 0;
        yHeld = (pad->mPressedFlags & 0x00000040) != 0;
    }

    if (up != 0) {
        ((CItemBoxGrid*)(self->grid()))->func_801CCAF0();
    } else if (down != 0) {
        if (((CItemBoxGrid*)(self->grid()))->IsItemBoxActive() != 0) {
            ((CItemBoxGrid*)(self->grid()))->HandleCancelBtn(0);
        } else {
            // Scroll failed: hide the help prompt, refresh the grid and move
            // to state 3.
            beginClose(&self->mTitleAHelp);
            ((CItemBoxGrid*)(self->grid()))->AdvanceBoxState();
            self->mState = 3;
        }
    } else if (turboCancel != 0) {
        ((CItemBoxGrid*)(self->grid()))->GridMoveCursorLeft();
    } else if (pageTurn != 0) {
        ((CItemBoxGrid*)(self->grid()))->GridMoveCursorRight();
    } else if (cancel != 0) {
        ((CItemBoxGrid*)(self->grid()))->GridMoveCursorDown();
    } else if (decide != 0) {
        ((CItemBoxGrid*)(self->grid()))->GridMoveCursorUp();
    } else if (leftTrigger != 0) {
        ((CItemBoxGrid*)(self->grid()))->OpenSortMenu();
    } else if (xHeld != 0) {
        ((CItemBoxGrid*)(self->grid()))->TickItemBoxGrid();
    } else if (yHeld != 0) {
        ((CItemBoxGrid*)(self->grid()))->SelectCatRow();
    }
}
