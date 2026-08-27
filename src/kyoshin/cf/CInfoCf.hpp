#pragma once

/**
 * cf::CInfoCf - Information/settings singleton.
 *
 * One global instance is created at boot; the constructor stores the
 * instance pointer in the global lbl_eu_80664250 so any code in this TU
 * can reach it without passing ``this`` around.  Used by the menu / info
 * stack to query and toggle UI state flags.
 *
 * Field layout (MWCC big-endian 32-bit):
 *   0x00  void* vtable     - set to externally-defined lbl_eu_805309B0
 *   0x04  u32   mFlags     - bitfield of state flags
 *   0x08  u32   mField08   - cached setting value (set by func_801667AC)
 *   0x0C  u32   mField0C   - runtime value 1 (set by func_801667AC)
 *   0x10  u32   mField10   - runtime value 2 (set by func_801667AC)
 */

#include <types.h>
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/CProcess.hpp"

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CItemBoxGrid.hpp"

namespace cf {

// The retail ctor symbol __ct__cf_CInfoCf is a C-linkage name (no C++
// mangling markers), so the constructor is emitted as a global function in
// the cpp (same convention as __ct__cf_CfTFile). The dtor keeps its regular
// member mangling (__dt__Q22cf7CInfoCfFv).
struct CInfoCf {
    ~CInfoCf();

    void* vtable;   // 0x00 - externally-defined vtable (lbl_eu_805309B0)
    u32 mFlags;     // 0x04
    u32 mField08;   // 0x08
    u32 mField0C;   // 0x0C
    u32 mField10;   // 0x10
};

} // namespace cf

// Minimal CScn declaration (only the render-callback member Term calls).
// Guarded so the kyoshin task headers can be included together in one TU
// (same declaration appears in CTaskGameEff.hpp / CTaskGameEvt.hpp).
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// Minimal CTaskGame decl (retail global-namespace class, symbols
// getInstance__9CTaskGameFv / isFlag01Set__9CTaskGameFv). The full class
// header pulls monolib/scn.hpp whose real CScn clashes with the guarded
// minimal CScn above - same pattern as CSystemWindow.hpp / CSysWinSelect.hpp.
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool isFlag01Set();
};

/*
 * Menu-screen process shared by the in-game menu / info stack.
 *
 * Layout (Term + sibling menu classes):
 *   0x00: vptr            -- CProcess-compatible vptr (m_vptr; dtor drives the
 *                             library D1 CProcess dtor by hand, see the cpp)
 *   0x04: opaque task area (0x54 bytes)
 *   0x58: IScnRender      -- render-callback subobject (member, vptr)
 *   0x5C: CScn*           -- owning scene (removeRenderCB target in Term)
 *   0x60: CBgTex          -- background layout widget
 *   0x80: CTitleAHelp     -- title/help bar
 *   0xB8: CItemBoxGrid    -- core item-box grid (vptr subobject + storage)
 *  0x4AC4: u8 mState      -- phase/state indicator
 *  0x4AC5: u8 mField4AC5  -- flag read by Term (skip CfGameManager handoff)
 */
class CMenuItem {
public:
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    void* m_vptr;              // 0x00: CProcess-compatible vptr (never stored here)
    u8 mPrefix[0x54];          // 0x04: opaque base/task-system area
    IScnRender mIScnRender;    // 0x58: render-callback subobject (vptr)
    CScn* mScene;              // 0x5C: owning scene
    CBgTex mBgTex;             // 0x60
    CTitleAHelp mTitleAHelp;   // 0x80
    CItemBoxGrid mItemBoxGrid; // 0xB8 (vptr-only subobject; storage padded below)
    u8 mGridPad[0x4AC4 - 0xBC]; // 0xBC-0x4AC3
    u8 mState;                 // 0x4AC4
    u8 mField4AC5;             // 0x4AC5
    u8 mField4AC6;             // 0x4AC6 (read by Init for the grid ctor flag)
};

// C-linkage imports (retail-unmangled symbols: US strips the member
// manglings for the func_ helpers, and the CfGameManager statics carry their
// C++ mangling inside the identifier). Declared extern "C" so MWCC emits the
// retail reloc names verbatim (same scheme as CfTFile.hpp / code_800A75FC.hpp).
extern "C" {
void setPresentationFlag__Q22cf13CfGameManagerFv(u32 enable);
void func_801C3D9C(CBgTex* self);
void func_801C40A0(CTitleAHelp* self);
void func_801CAE9C(CItemBoxGrid* self);
void func_80062F60();
int func_800A8CD4();
u32 func_800A8DA4();
u32 func_80166830();
void __dt__8CProcessFv(CProcess* self, int flags);
// CMenuItem dtor (retail D1, manual member/base destruction - see the cpp)
// and the embedded widget dtors it drives, all with the retail CW names.
CMenuItem* __dt__9CMenuItemFv(CMenuItem* self, int flags);
void __dt__12CItemBoxGridFv(CItemBoxGrid* self, int flags);
void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
void __dt__6CBgTexFv(CBgTex* self, int flags);
// Retail-unmangled ctors/helpers used by CMenuItem::Init (US strips the
// member manglings; the widget headers' C++ members would mangle the relocs).
void __ct__CBgTex(CBgTex* self, u8 arg);
void __ct__CTitleAHelp(CTitleAHelp* self, char* arg1, u8 arg2);
void __ct__CItemBoxGrid(CItemBoxGrid* self, u32 type, u32 unk, u32 scene, u32 flag);
int func_801C3C14(CBgTex* self);
void func_801C3A24(CBgTex* self);
void CTitleAHelp_load(CTitleAHelp* self);
void func_801CB480(CItemBoxGrid* self);
void func_801CAA6C(CItemBoxGrid* self);
void PushToList(CItemBoxGrid* self, u8 val);
}

// CInfoCf vtable (retail .data:0x805309B0), the UI-state singleton pointer
// (sbss; instance stored by __ct__cf_CInfoCf, cleared by the dtor), and the
// menu singleton flag cleared by CMenuItem::Term.
extern u8 lbl_eu_805309B0[];
extern cf::CInfoCf* lbl_eu_80664250;
extern u32 lbl_eu_80664258;

// Title/help name-string base used by CMenuItem::Init (retail rodata).
extern char lbl_eu_8050303C[];

/*
 * Body-copy view for the C-ABI copy helpers in this TU. Each helper copies a
 * fixed slice of a larger object, skipping the member vtable at +0x00.
 * Layouts recovered from the retail copy ASM (func_80167260 / 801672E4 /
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

// CSysWin body copied by func_8016742C (0x04..0x39). The 0x29-0x2B gap is
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
__declspec(align(4)) struct CInfoCfE48Entry {
    u64 q;
};

/*
 * Body-copy view for func_80166E48 (0x04..0xEF). Field-by-field head/tail
 * with a 16-entry 8-byte table at +0x6C that retail copies with an
 * lwzu/stwu counted loop (element base registers start at 0x68 = 0x6C-4).
 */
struct CInfoCfCopyE48 {
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
    CInfoCfE48Entry field_6C[16]; // align(4) entries -> table at +0x6C like retail
    u8 field_EC;
    u8 field_ED;
    u8 field_EE;
};

struct CInfoCfObjE48 {
    void* vtable;       // 0x00 (never copied)
    CInfoCfCopyE48 body; // 0x04
};

/*
 * Body-copy view for func_80166F80 (0x04..0x208). The retail copy is two
 * aggregate statements: a field-by-field head (0x04..0xC4, 35 streamed words +
 * bytes + five 8-byte pair chunks) and a middle block (0xC8..0x208) that
 * starts with the 18-entry u64 counted loop, continues with paired words and
 * the 0x188 byte, and ends with the 16-entry u64 loop at the odd +0x189.
 * Splitting the object this way reproduces the retail prologue scheduling:
 * loop 1's li/addi/addi/mtctr sits AFTER the head copy, while loop 2's
 * prologue is hoisted into the 0x158 pair - a single-statement copy hoists
 * both, which does not match.
 */
// 8-byte chunk type for the paired word regions in func_80166F80's blob.
// Retail copies each 8-byte run as a 2-load/2-reverse-store pair (lwz r5 / lwz r0
// / stw r0 / stw r5); a 4-aligned 8-byte struct reproduces both the offset and
// the r5-first allocation (u64 allocates r0-first; u8[8] is 1-aligned).
struct CInfoCfPair {
    u32 lo;
    u32 hi;
};

// Tail u64 table of func_80166F80's blob: retail packs it at the odd +0x189
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
 * Body-copy view for func_80167368 (0x04..0x42). The two words at +0x39 /
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
 * Raw body view for func_801674D0 (0x00..0x44C0, vtable slot included).
 * The object is a packed blob: aligned u64 tables start at +0x00, +0x2825,
 * +0x28A5, +0x2CAC, +0x34B4, +0x3CBC (the last three are 4-mod-8, so no
 * natural struct can express them) with packed string/data runs in between.
 * func_801674D0 copies it region-by-region with counted lwzu/stwu loops.
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
    CInfoCfObjE48 objE8;          // 0xE8  func_80166E48
    CInfoCfObjF80 obj1D8;         // 0x1D8 func_80166F80
    CInfoCfObjD4 obj3E4;          // 0x3E4 func_801671D4
    CInfoCfObj60 obj418;          // 0x418 func_80167260
    CInfoCfObjE4 obj440;          // 0x440 func_801672E4
    CInfoCfObj368 obj468;         // 0x468 func_80167368
    CInfoCfObjSysWin obj4AC;      // 0x4AC func_8016742C
    CInfoCfObjSysWin obj4E8;      // 0x4E8 func_8016742C
    CItemBoxGridTail524 tail524;  // 0x524..0x549 (inline)
    u8 _54A[0x54C - 0x54A];       // 0x54A..0x54B
    CInfoCfObj4D0 obj54C;         // 0x54C func_801674D0 (0x44C0 bytes)
};

// Same-unit import: pad-input/advance helper called from CMenuItem::Move
// case 2 (body lives in kyoshin/menu/CMenuItem.cpp).
extern "C" void func_80167A2C(CMenuItem* self);

// CItemBoxGrid C-ABI helper (body lives in kyoshin/CItemBoxGrid.cpp); no
// header declares the retail-unmangled name, so declare it here.
extern "C" u32 func_801CB1E4(CItemBoxGrid* self);

// Render-gate mode bitfield (.sbss; bit 21 = busy), shared with the other
// menu-screen TUs.
extern u32 lbl_eu_80663E28;

// Menu/task-flow imports previously pulled in via CSkipTimer.hpp (that header
// carries a conflicting __ct__CSysWin declaration, so they are declared here).
extern u32 func_800FEDF8();
extern void func_800FF914();
extern void playUISound__FUl(u32);