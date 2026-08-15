#pragma once

#include <types.h>
#include <monolib/work/CEventFile.hpp>  // CEventFile (OnFileEvent param)

// CEquipItemBox vtable (lbl_eu_80538658)
extern "C" void* lbl_eu_80538658[];

namespace nw4r {
namespace math {
    struct VEC3;
}
namespace lyt {
    class Layout;
    class DrawInfo;
    class AnimTransform;
    class ArcResourceAccessor;
    class Pane;
}
}

/* Layout cursor used by CEquipItemBox (CBaseCur-style), own vtable.
   Ctor is C-ABI: retail symbol __ct__CEIBCur carries no class-length
   mangling, so it is declared/defined with C linkage and called explicitly
   from derived ctors. */
struct CEIBCur {
    void func_80285A18();
    void func_80285B24();
    void* mVtable;          // 0x00
    void* mArcResAcc;       // 0x04
    void* mpLayout;         // 0x08
    void* mpAnimTrans0;     // 0x0C
    void* mpAnimTrans1;     // 0x10
    u8 mActive;             // 0x14
    u8 mVisible;            // 0x15
    u8 _pad16[2];           // 0x16
};

/* CEIBCur ctor (C-ABI, returns this). */
extern "C" CEIBCur* __ct__CEIBCur(CEIBCur* self, void* arcResAcc);

/* CEIBCur vtable / CEIBPageCur vtable (in this unit's .data). */
extern "C" void* lbl_eu_80538704[];
extern "C" void* lbl_eu_805386EC[];

/* Layout draw w/ visible flag. Retail symbol IS the mangled name; declare
   with C++ linkage so MWCC mangles func_80137038 to the retail symbol. */
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

/* Bind a named pane to a resource (code_80135FDC unit, 3-arg form). */
extern "C" void func_80137E7C(nw4r::lyt::Layout*, const char*, u32);

/* Layout/anim builders (code_80135FDC unit). C++ linkage so MWCC mangles
   the unmangled identifiers to the retail names. */
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);

/* CEIBPageCur - page-cursor subclass of CEIBCur, overrides vtable.
   Ctor is C-ABI like __ct__CEIBCur (retail symbol __ct__CEIBPageCur, no
   class-length mangling); called explicitly by OnFileEvent. */
struct CEIBPageCur : CEIBCur {
};
extern "C" CEIBPageCur* __ct__CEIBPageCur(CEIBPageCur* self, void* arcResAcc);

/* 8-byte item data struct copied by copyEquipItemData */
struct CEquipItemData {
    s16 unk0;   // 0x0
    u8 unk2;    // 0x2
    u8 unk3;    // 0x3
    u8 unk4;    // 0x4
    u8 unk5;    // 0x5
    u8 unk6;    // 0x6
    u8 unk7;    // 0x7
};

/* Item-list region of the CItemBoxInfo object at unk_20c + 0xB0, written by
   func_80289AA4's list refresh: u16 ids, 12 VEC3 records (i*0xC), two per-item
   byte tables (+0xA8 / +0xB4) and a per-item s16 table (+0xC0). */
struct CEquipItemBoxItemListView {
    u16 field_00[0xC];            // 0x00..0x17 u16 item ids
    u8 field_18[0x90];            // 0x18..0xA7 12 x 0xC records (VEC3 copies)
    u8 field_A8[0x20];            // 0xA8..0xC7 per-item bytes (+0xA8 / +0xB4)
    s16 field_C0[0x20];           // 0xC0..0xFF per-item shorts
};

/* Overlay for the per-page u16 item array at +0x210 with the per-index u8
   table at +0x2C4 and s16 table at +0x2D0 (read by func_80289AA4). */
struct CEquipItemBoxPageDataView {
    u8 pad00[0x210];
    u16 field_210[0x20];    // 0x210..0x24F per-page u16 ids
    u8 pad250[0x2C4 - 0x250];
    u8 field_2C4[0xC];      // 0x2C4..0x2CF per-index bytes
    s16 field_2D0[0xC];     // 0x2D0..0x2DF per-index shorts
};

/* Item instance (retail CItemBase-ish). Only the id word at +0x0, the
   flag halfword at +0x4 and the byte at +0x7 are read by this TU's helpers. */
struct CItemInstance {
    u32 word;   // 0x0 id/kind word
    u16 flags;  // 0x4
    u8 unk6;    // 0x6
    u8 unk7;    // 0x7
};

/* Grid of 8-byte items used by the equip-box list helpers. Header fields
   (count / category / cursor row) sit right after the element array. */
struct CEquipItemGrid {
    CEquipItemData data[0x400];  // 0x0000..0x2000 (8-byte elements)
    u16 count;                   // 0x2000 (index limit)
    u8 cat;                      // 0x2002 (category passed to item lookup)
    u8 _pad2003;                 // 0x2003 (count/range)
    s8 idx;                      // 0x2004 (current cursor row)
    u8 _pad2005;                 // 0x2005
    char field_2006[0x20];       // 0x2006 name buffer (sprintf dst)
    u8 field_2026[0x400];        // 0x2026..0x2425 per-cell state bytes
};

/* CEquipItemBox - equip item box UI widget.
   Layout reconstructed from __ct__CEquipItemBox. */
struct CEquipItemBox {
    CEquipItemBox();
    ~CEquipItemBox();
    int OnFileEvent(CEventFile* ev);

    u8 func_802865A0();
    u8 func_802865A8();
    int func_80286650();
    int func_802866A0();
    void func_802866E8();
    u8 func_80286698();
    u8 func_80287EE8();
    void func_80287EFC(u32 val);
    void func_80288A1C();
    void func_80288A6C();
    void func_80282DF8();
    void func_80282E24();
    void func_80287D58();
    u8 func_802832B4();
    int func_802865B0();
    void func_80286740();
    int func_8028847C();
    char* func_8028D0EC();
    void func_80286F6C();
    void func_80289754();
    void func_80289AA4();

    void* mVtbl;            // 0x00

    // +0x04: UnkClass_8045F564 mMemRegion1 (0x10 bytes)
    // +0x14: UnkClass_8045F564 mMemRegion2 (0x10 bytes)
    u8 _pad04[0x20];        // 0x04..0x23 (memregions at 0x04 / 0x14)
    void* field_24;         // 0x24 file handle 1
    void* field_28;         // 0x28 file handle 2
    void* field_2C;         // 0x2C file handle 3
    nw4r::lyt::ArcResourceAccessor* field_30; // 0x30 arc resource accessor
    nw4r::lyt::ArcResourceAccessor* field_34; // 0x34 arc resource accessor
    nw4r::lyt::Layout* field_38;              // 0x38 layout object
    void* field_3C;         // 0x3C entry animation transform

    u8 unk_40;              // 0x40
    u8 unk_41;              // 0x41
    u8 unk_42;              // 0x42
    u8 unk_43;              // 0x43

    // +0x44: CEIBCur   (0x18 bytes)
    // +0x5C: CEIBPageCur (0x18 bytes)
    // +0x74: CCur18     (0x18 bytes)
    u8 _pad44[0x14];    // 0x44..0x57
    u8 unk_58;          // 0x58 (cursor0 mActive)
    u8 _pad59[3];       // 0x59..0x5B (mVisible + pad)
    u8 pagecur[0x18];   // 0x5C..0x73 (CEIBPageCur region)
    u8 ccur18[0x18];    // 0x74..0x8B

    // +0x8C: CSortMenu (0xF0 bytes)
    u8 _padSortMenu[0xF0];

    // +0x17C: CSysWin (0x3C bytes)
    u8 _padSysWin1[0x3C];

    // +0x1B8: CSysWin (0x3C bytes)
    u8 _padSysWin2[0x3C];

    u8 unk_1f4;             // 0x1F4
    s8 unk_1f5;             // 0x1F5
    u8 unk_1f6;             // 0x1F6
    u8 _pad1F7[3];          // 0x1F7
    u16 unk_1fa;            // 0x1FA
    u16 unk_1fc;            // 0x1FC
    u8 unk_1fe;             // 0x1FE
    u8 _pad1FF[0xD];       // 0x1FF
    u32 unk_20c;            // 0x20C

    // ... many more fields ...
    u8 _pad210[0x15C];

    u8 unk_36c[6];          // 0x36C
    u8 unk_372;             // 0x372
    u8 unk_373;             // 0x373
    u8 unk_374;             // 0x374
    u8 unk_375;             // 0x375
    u8 unk_376;             // 0x376
    u8 unk_377;             // 0x377
    u8 unk_378;             // 0x378
    u8 unk_379;             // 0x379
    u8 unk_37a;             // 0x37A
    u8 unk_37b;             // 0x37B
    u8 unk_37c;             // 0x37C

    // ---- region used by grid/cursor helpers (large array area) ----
    u8 _pad37D[0x2003 - 0x37D];  // 0x37D..0x2002
    u8 field_2003;          // 0x2003 (count/range)
    s8 field_2004;          // 0x2004 (current index)
    u8 _pad2005[0x2026 - 0x2005]; // 0x2005..0x2025
    u8 field_2026[0x400];   // 0x2026..0x2425
    u8 _pad2426[0x27A4 - 0x2426]; // 0x2426..0x27A3
    u32 field_27A4;         // 0x27A4
};

/* CEventFile event object: the +0x4 word is the file handle compared against
   field_24/field_28/field_2C (the three bind files). */
class CEventFile;
struct CEquipItemBoxEventFileView {
    void* vtable;   // 0x0
    void* handle;   // 0x4
};

/* CFileHandle object read by OnFileEvent (r3 passed to func_8003AA34). */
struct CEquipItemBoxFileHandleView {
    void* vtable;   // 0x0
    void* field_4;  // 0x4
};

/* 8-byte colour-vector pair returned in r3:r4 by func_80139658 / func_801397AC
   and copied to the .sbss colour tables by CopyVec4s. */
struct CEquipItemBoxFourShorts {
    s16 a, b, c, d;
};

/* Font object returned by CDeviceFont::func_80452C10: vtable slot 9
   (offset 0x24) yields the pane data bound via func_8013676C. */
struct CEquipItemBoxFontView {
    virtual void f2() = 0;
    virtual void f3() = 0;
    virtual void f4() = 0;
    virtual void f5() = 0;
    virtual void f6() = 0;
    virtual void f7() = 0;
    virtual void f8() = 0;
    virtual u32 f9() = 0;  // vtable +0x24
};

/* Texture object returned by ArcResourceAccessor::GetResource for 'timg':
   mChain (+0x08) -> dims (+0x00) carries the 2D dimension header. */
struct CEquipItemBoxTexDimsView {
    u16 field_0;    // 0x0
    u16 field_2;    // 0x2
};
struct CEquipItemBoxTexChainView {
    CEquipItemBoxTexDimsView* mDims;  // 0x0
};
struct CEquipItemBoxTexObjView {
    void* mVtable;                 // 0x0
    void* _pad_04;                 // 0x4
    CEquipItemBoxTexChainView* mChain;  // 0x8
};

/* Pane size words at +0x4C / +0x50 (retail writes the texture dims here). */
struct CEquipItemBoxPaneSizeView {
    u8 pad00[0x4C];
    float mW;   // 0x4C
    float mH;   // 0x50
};

/* Cursor-member vtable view: the first declared virtual lands at vtable
   +0x08 (MWCC RTTI prefix), the slot OnFileEvent dispatches after copying
   the temp cursor into the member. */
class CEquipItemBoxCurMemberView {
public:
    virtual void vfSlot8() = 0;  // vtable +0x08
};

/* Non-vtable CCur18 field block copied from the stack temp into the member
   (4 words + 2 bytes, skipping the vtable word at +0x00). */
struct CEquipItemBoxCCur18CopyView {
    u32 word0;  // +0x04
    u32 word1;  // +0x08
    u32 word2;  // +0x0C
    u32 word3;  // +0x10
    u8 b0;      // +0x14
    u8 b1;      // +0x15
};

/* Per-page store tables written by func_8028C280: byte table at +0x2B8,
   byte table at +0x2C4 and s16 table at +0x2D0 (read by func_80289AA4). */
struct CEquipItemBoxPageStoreView {
    u8 pad00[0x2B8];
    u8 field_2B8[0xC];  // 0x2B8..0x2C3
    u8 field_2C4[0xC];  // 0x2C4..0x2CF
    s16 field_2D0[0xC]; // 0x2D0..0x2DF
};

// OnFileEvent helper imports (retail C-ABI names).
extern "C" void createRegion__17UnkClass_8045F564FiiPCci(void*, int, int, const char*, int);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void __dt__14Class_8045F858Fv(void*, int);
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor*, void*, const char*);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void func_8013676C(nw4r::lyt::Pane*, void*);
// Font/line text providers: func_801355A0 is the retail MANGLED name (C++
// linkage), the other two are plain C-ABI names.
char* func_801355A0();
extern "C" char* func_801355BC();
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
void func_801368C0(nw4r::lyt::Layout*, char*, u32);
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int);
extern "C" u16 func_8013606C(const void*, const void*, u16);
extern "C" CEquipItemBoxFourShorts func_80139658(void*, void*, u32);
extern "C" CEquipItemBoxFourShorts func_801397AC(void*, u32);
extern "C" void CopyVec4s(void*, const void*);
extern "C" void func_80137F88(void*, void*);
extern "C" char* func_8013639C(u32, const char*, u32);
extern "C" void func_801FA220(u8*, const u8*);
extern "C" void __ct__CCur18(void*, void*);
extern "C" void func_8003AA34(void*);
extern "C" void* getFP__FPCc(const char*);
extern "C" void func_8003AA78__5CBdatFUlPv(u32, void*);
extern "C" void func_8045F810__17UnkClass_8045F564Fv(void*);
// Bdat table slot written by the field_2C file handler.
extern void* lbl_eu_806649E0;

/* CSysWin vtable view exposing the layout-build virtual at +0x88 (slot 34 =
   declared index 32 after the MWCC offset-to-top + RTTI prefix), dispatched by
   func_802861A8 after the file loads. Same shape as the CSysWinView classes in
   CSysWinSave.hpp / COption.hpp / CCollepedia.hpp. */
class CEquipItemBoxSysWinView {
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0; // vtable +0x88 (layout build)
};

/* Copy 8-byte CEquipItemData struct from src to dst */
void copyEquipItemData(CEquipItemData* dst, const CEquipItemData* src);

/* Item-impl vtable view exposing the slots the grid sorts dispatch: vf08
   (0x08), vf30 (0x30) and vf90 (0x90) carry the sort keys, plus the two
   slots func_80283350 reads (vf08 / vf20). MWCC auto-inserts RTTI + null
   at vtable slots 0-1, so the declared virtuals land at slots 2..n.
   Declarations are padded out to 0x90 so the sort-key slot offsets match
   retail's vtable. */
class CEquipItemBoxItemImplView {
public:
    virtual u32 vf08(CItemInstance* p);  // 0x08
    virtual void vf0C(CItemInstance* p); // 0x0C
    virtual void vf10(CItemInstance* p); // 0x10
    virtual void vf14();                 // 0x14
    virtual void vf18();                 // 0x18
    virtual void vf1C(CItemInstance* p); // 0x1C
    virtual u32 vf20(CItemInstance* p);  // 0x20
    virtual void vf24(CItemInstance* p); // 0x24
    virtual void vf28(CItemInstance* p); // 0x28
    virtual CItemInstance* vf2C(CItemInstance* p, u8 idx); // 0x2C (slot entry getter)
    virtual u32 vf30(CItemInstance* p);  // 0x30 (item-count sort key)
    virtual void vf34(CItemInstance* p); // 0x34
    virtual void vf38(CItemInstance* p); // 0x38
    virtual void vf3C(CItemInstance* p); // 0x3C
    virtual s16 vf40(CItemInstance* p, u8 idx);  // 0x40 (slot id, -1 = empty)
    virtual void vf44(CItemInstance* p, u8 idx, s32 val); // 0x44 (slot-id setter)
    virtual void vf48(CItemInstance* p); // 0x48
    virtual void vf4C(CItemInstance* p); // 0x4C
    virtual void vf50(CItemInstance* p); // 0x50
    virtual u32 vf54(CItemInstance* p);  // 0x54 (name-key sort)
    virtual void vf58(CItemInstance* p); // 0x58
    virtual void vf5C(CItemInstance* p); // 0x5C
    virtual void vf60(CItemInstance* p); // 0x60
    virtual void vf64(CItemInstance* p); // 0x64
    virtual void vf68(CItemInstance* p); // 0x68
    virtual void vf6C(CItemInstance* p); // 0x6C
    virtual void vf70(CItemInstance* p); // 0x70
    virtual void vf74(CItemInstance* p); // 0x74
    virtual void vf78(CItemInstance* p); // 0x78
    virtual void vf7C(CItemInstance* p); // 0x7C
    virtual void vf80(CItemInstance* p); // 0x80
    virtual void vf84(CItemInstance* p); // 0x84
    virtual void vf88(CItemInstance* p); // 0x88
    virtual void vf8C(CItemInstance* p); // 0x8C
    virtual u32 vf90(CItemInstance* p);  // 0x90 (name-key sort)
};

/* CCur18 vtable view (declared index 2 -> vtable +0x10 = Move/position-set
   virtual, dispatched by func_802870DC with a 16-byte state buffer). */
class CEquipItemBoxCur18View {
public:
    virtual void vf02() = 0;        // +0x08
    virtual void vf03(void*) = 0;   // +0x0C
    virtual void vf04(void*) = 0;   // +0x10 Move
};

/* Pane-layout view exposing the translate at +0x2C (Pane::mTranslate is
   protected in the nw4r header, so the equip-box helpers reach it through
   this fixed-offset mirror). */
struct CEquipItemBoxPaneView {
    u8 pad00[0x2C];
    float mTranslate[3];  // 0x2C
};

/* u32->float conversion scratch (CPartsChange.cpp convention): builds the
   0x43300000-prefixed bit pattern by hand so the magic subtraction below
   references the retail .sdata2 constants (lbl_eu_80668B18/80668B10)
   instead of MWCC's un-nameable pool entry. */
union CEquipItemBoxF64Conv {
    u32 w[2];
    double d;
};

/* Base position of the equip-box page cursor: the 3 words at +0x200 are
   copied by func_80289754 and shifted by the row offset. Read as u32 so the
   copy uses lwz/stw (retail shape). */
struct CEquipItemBoxPagePosView {
    u8 pad00[0x200];
    u32 pos[3];   // 0x200..0x20B
};

/* 6-byte equipment-slot definition used by func_80288544's slot scan:
   category byte, item-id short and a per-category base offset byte. */
struct CEquipItemBoxSlotDef {
    u8 cat;     // 0x0
    u8 _pad1;   // 0x1
    s16 item;   // 0x2
    u8 extra;   // 0x4 (base offset added to the matched instance index)
    u8 _pad5;   // 0x5
};

/* 36-byte table of 6 slot definitions (data at lbl_eu_8050EF90). Copied to a
   stack local by func_80288544 before the item ids are patched in. */
struct CEquipItemBoxSlotTable {
    CEquipItemBoxSlotDef slots[6];
};

/* Item object returned by func_8009EC9C: 6 slot-id shorts at 0x1c..0x26
   supply the item ids patched into the slot table by func_80288544. */
struct CEquipItemBoxItemView {
    u8 _pad00[0x1c];
    s16 field_1c;   // 0x1c
    s16 field_1e;   // 0x1e
    s16 field_20;   // 0x20
    s16 field_22;   // 0x22
    s16 field_24;   // 0x24
    s16 field_26;   // 0x26
};

// 36-byte equipment-slot table (data import, same .data block as the string pool).
extern CEquipItemBoxSlotTable lbl_eu_8050EF90;

/* 24-byte equipment-category table (6 u32 entries; the low byte of each word
   is a category id). Copied to a stack local by func_80282610. */
struct CEquipItemBoxCatWordTable {
    u32 w[6];   // 0x00..0x17
};
extern CEquipItemBoxCatWordTable lbl_eu_8050EF50;

/* 12-byte u32 table copied by func_8028CBCC and indexed by the matched
   category slot (three CfMove search types). */
extern u32 lbl_eu_8050EFB4[3];

/* 6-byte category tables (.sdata) read by func_8028CBCC's cat-3 scan. */
struct CEquipItemBoxCat6 {
    u8 b[6];
};
extern CEquipItemBoxCat6 lbl_eu_80668B50;
extern CEquipItemBoxCat6 lbl_eu_80668B58;

/* Overlay for the per-page item-id table at +0x210 (u16) and the 12 page
   position VEC3 records at +0x228, reset by func_8028AA64. */
struct CEquipItemBoxPageStateView {
    u8 pad00[0x210];
    u16 field_210[0xC];       // 0x210..0x227
    float field_228[0xC][3];  // 0x228..0x2FF
};

/* Stack-allocated enum-list holder/list used by func_8028CBCC's CfMove
   search (func_80043D90 ctor / func_80043F18 getter / __dt__80043E88 dtor;
   the list's element count sits at +0x620). */
struct CEquipItemBoxEnumHolder {
    void* list;   // 0x0
    u32 handle;   // 0x4
};
struct CEquipItemBoxEnumList {
    u8 _pad00[0x620];
    u32 field_620;  // 0x620
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dl__FPv(void*);
extern "C" int CSysWin_isActive(void*);
extern "C" void func_801D216C(void*, int);
extern "C" u32 func_801D32DC(u8*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" void func_80139198(void*);
extern "C" void func_80138078__FUl(u32);
extern "C" u8 func_801392B4(int);
extern "C" void func_8028A5D8(CEquipItemBox* self, int a);
extern "C" void func_80288AC0(CEquipItemBox* self);
extern "C" void func_8028AF98(CEquipItemBox* self, int a, int b);
extern "C" void func_8028B7CC(CEquipItemBox* self, int a, int b);
extern "C" void func_8028BE74(CEquipItemBox* self, int a, int b);
extern "C" u8 func_802832D8(CEquipItemGrid* grid, u16 idx);
extern "C" u8 func_80283280(CEquipItemGrid* grid, u32 param);
extern "C" int func_80288948(CEquipItemBox* box);
extern "C" void func_8028AA64(CEquipItemBox* self);
extern "C" void func_80289500(CEquipItemBox* self, int a);
extern "C" void func_8028A1DC(CEquipItemBox* self);
extern "C" void func_8028A07C(CEquipItemBox* self);
extern "C" void func_8028A0C0(CEquipItemBox* self, u8 val);
extern "C" void func_80289CC0(CEquipItemBox* self);
extern "C" void func_80285B70(CEIBCur* self);
extern "C" void func_8022B8E4(void*);
extern "C" void func_80289E70(CEquipItemBox*);
// Same-TU helpers (defined as extern "C" free functions in CEquipItemBox.cpp;
// the retail symbols are unmangled, so calls must reference the plain names).
extern "C" void func_80282DF8(CEquipItemBox* self);
extern "C" void func_80282E24(CEquipItemBox* self);
extern "C" u8 func_802832B4(CEquipItemBox* self);
extern "C" void func_80289754(CEquipItemBox* self);
extern "C" void func_80289AA4(CEquipItemBox* self);
extern "C" void func_80286D7C(CEquipItemBox* self);
extern "C" void func_80286F6C(CEquipItemBox* self);
extern "C" void func_8028A0E0(CEquipItemBox* self);
extern "C" char* func_8028D0EC(CEquipItemBox* self);
extern "C" void func_80288A1C(CEquipItemBox* self);
extern "C" void func_80288A6C(CEquipItemBox* self);
extern "C" void func_802889C0(CEquipItemBox* self);
extern "C" void func_80285A18(CEIBCur* self);
extern "C" void func_80285B24(CEIBCur* self);
extern "C" u32 func_802857F0(CEquipItemBox* self, CItemInstance* item);
extern "C" double func_80285708(CEquipItemBox* self, CItemInstance* item);
// Forward decls for the grid/array helpers (defined in CEquipItemBox.cpp).
extern "C" CEquipItemData* func_80282574(CEquipItemData* dst, s16 a0, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7);
extern "C" void func_80282594(CEquipItemData* dst, const CEquipItemData* src);
extern "C" void func_80282610(CEquipItemGrid* grid, u8 v, u8 b, u8 hi);
// String pool used by func_8028D0EC (offsets 0x2d / 0x36) and others.
extern "C" char lbl_eu_8050EFDC[];
extern "C" char* func_80136190(char*, char*, u32);
extern "C" u32 func_801361E8(u32, const char*, u32);
extern "C" void func_8003AA8C__5CBdatFUl(u32);
extern "C" void func_801390E0__FPP11CFileHandle(void*);
extern "C" void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" void func_801D3258(void*);
extern "C" void func_8022B7F4(void*);
extern "C" u16 func_80139358(u32);
extern "C" u16 func_80136254(const void*, const void*, int);
// Texture-name lookup (retail C-ABI; u16 arg, returns the resource name).
extern "C" char* func_80138F78(u16);
// Item-object validity/type check used by func_80288E14's texture pick.
extern "C" int func_801C6E90(void*);
extern "C" void* func_8009EC9C(u32);
// Item randomizer: reads the low 16 bits of the first word of the object
// returned by func_8009EC9C (cf/CtrlObjectParam TU; callers pass that obj).
extern "C" u16 func_800A082C(void*);
// Item drop-rate helpers (code_80135FDC TU): name-table float probe and the
// (rateA + rateB) clamp divisor shared by the equip-box/line stat displays.
extern "C" f32 func_8013B380(u32 idx);
extern "C" f32 func_80139C98(u32 a, u32 b, u32 c, f32 d);
extern "C" nw4r::math::VEC3* code80135FDC_setVec3(float*, float, float, float);
extern "C" void copyVEC3(float*, const float*);
extern "C" void* CItem_initItemImplInstances(void*);
extern u32 lbl_eu_806640D8;
extern u32 lbl_eu_806640F8;
extern "C" int CSysWin_getUnk34(void*);
extern "C" int func_801D3320(void*);
extern "C" int func_801D3328(void*);
extern "C" void func_801D377C(void*);
extern "C" void func_80124270(void*, u32);
// Pane-visibility query (C-ABI retail symbol; extern "C" so call sites emit
// the plain name, matching func_80287250's reloc site).
extern "C" bool func_801C4648(nw4r::lyt::Pane*);

// Item-box object type for the unk_20c pointer (forward decl; full layout in
// CItemBoxInfo.hpp, which is included by TUs that also include this header).
struct CItemBoxInfo;
// Item-box state query (retail mangled getItemBoxState__FP12CItemBoxInfo;
// u32 return avoids the u8 call-site mask; same decl as CItemBoxGrid.hpp).
extern "C" u32 getItemBoxState__FP12CItemBoxInfo(void*);
// Item-box list refresh helpers (CItemBoxInfo TU, retail plain names).
extern "C" void func_801D4AE0(CItemBoxInfo* info, int arg2, void* arg3);
extern "C" u8 func_801EF034(const u8*, unsigned int);
extern "C" u32 func_801D4260(CItemBoxInfo* info, u8 arg2);
// Layout text/visibility bind (retail mangled func_80136910__FPQ34nw4r3lyt6LayoutPcUc).
void func_80136910(nw4r::lyt::Layout*, char*, u8);
// Same-TU grid helper (retail plain name; defined in this .cpp).
extern "C" u8 func_80282D60(CEquipItemGrid* grid);
// Same-TU dispatch helpers used by func_8028345C (defined in this .cpp).
extern "C" void func_80283E64(CEquipItemGrid* grid, u32 cat);
extern "C" void func_80284C30(CEquipItemGrid* grid, u32 mode);
// Same-TU grid sort helpers dispatched by func_8028345C (defined below).
extern "C" void func_80284144(CEquipItemGrid* grid);
extern "C" void func_80284244(CEquipItemGrid* grid);
extern "C" void func_80284358(CEquipItemGrid* grid);
extern "C" void func_80284490(CEquipItemGrid* grid, int param);
extern "C" void func_8028461C(CEquipItemGrid* grid, int param);
extern "C" void func_802847A4(CEquipItemGrid* grid);
extern "C" void func_802848C4(CEquipItemGrid* grid);
extern "C" void func_80284A00(CEquipItemGrid* grid);
extern "C" void func_80284B18(CEquipItemGrid* grid);
extern "C" void func_80284DCC(CEquipItemGrid* grid);
extern "C" void func_80284F1C(CEquipItemGrid* grid);
extern "C" void func_8028506C(CEquipItemGrid* grid);
extern "C" void func_802851BC(CEquipItemGrid* grid);
extern "C" void func_8028530C(CEquipItemGrid* grid, int key);
extern "C" void func_80285478(CEquipItemGrid* grid);
extern "C" void func_802855C8(CEquipItemGrid* grid);
// Same-TU grid helpers used by the equip-list refresh (retail plain names).
extern "C" void* func_80282F34(CEquipItemGrid* grid, u16 idx);
extern "C" char* func_80283350(CEquipItemGrid* grid, u32 param);
extern "C" void func_8028A9CC(CEquipItemBox* self, int a, int b);
extern "C" void func_801D3330(void*);
extern "C" void func_801D3408(void*);
extern "C" void func_801D3430(void*, const nw4r::math::VEC3*);
extern "C" void func_801D3454(void*, void*);
extern "C" void func_801D353C(void*, u8);
// Sort-menu page-list helpers (CSortMenu TU, retail plain names).
// func_801D3818(menu, page, &outCount, &outRemain): writes func_8015780C(page)
// (clamped to >= 4) into outCount and (0 or count-4) into outRemain.
extern "C" void func_801D3818(void*, u8, u8*, u8*);
extern "C" void func_801D350C(void*);
extern "C" void func_801D3518(void*, void*);
extern "C" u16 func_8015780C(int);
extern "C" void func_80136B4C(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" void func_80137924(void*, void*, void*, void*);
// Per-frame update helpers (retail plain names, defined in sibling TUs).
extern "C" void func_801D3064(void*);
extern "C" int func_800A9D90(void);
extern "C" void func_801D202C(void*);
extern "C" void func_801D3160(void*);
extern "C" void func_8022B748(void*);
// Sub-window / cursor / sort-menu draw helpers (retail plain names).
extern "C" void func_801D31F8(void*, nw4r::lyt::DrawInfo*);
extern "C" void func_8022B7C8(void*, nw4r::lyt::DrawInfo*);
extern "C" void func_801D20B0(void*, nw4r::lyt::DrawInfo*);
// Layout text/colour setter used by the sort-menu page rebuild (func_8028A1DC);
// the 3rd/4th args are .sbss colour-table pairs referenced via sda21.
extern "C" void func_80139A18(nw4r::lyt::Layout*, char*, void*, void*);
// .sbss colour-table initialisers used by sinit_8028DAB0 (retail plain names).
extern "C" void func_801D1F9C(void*, u32);
extern "C" void func_801C4B60(void*, u32, u32, u32, u32);
extern s16 lbl_eu_80664920[4];
extern s16 lbl_eu_80664928[4];
extern s16 lbl_eu_80664930[4];
extern s16 lbl_eu_80664938[4];
extern s16 lbl_eu_80664940[4];
extern s16 lbl_eu_80664948[4];
extern s16 lbl_eu_80664950[4];
extern s16 lbl_eu_80664958[4];
extern s16 lbl_eu_80664960[4];
extern s16 lbl_eu_80664968[4];
extern s16 lbl_eu_80664970[4];
extern s16 lbl_eu_80664978[4];
extern s16 lbl_eu_80664980[4];
extern s16 lbl_eu_80664988[4];
extern s16 lbl_eu_80664990[4];
extern s16 lbl_eu_80664998[4];
extern s16 lbl_eu_806649A0[4];
extern s16 lbl_eu_806649A8[4];
extern s16 lbl_eu_806649B0[4];
extern s16 lbl_eu_806649B8[4];
extern s16 lbl_eu_806649C0[4];
extern s16 lbl_eu_806649C8[4];
extern s16 lbl_eu_806649D0[4];
extern s16 lbl_eu_806649D8[4];
// Sort-menu page handler called once per page by func_8028A1DC (this TU).
extern "C" void func_8028A374(CEquipItemBox* self, u8 v, u8 i);
// Kind-specific equip-box rebuild tail (this TU, retail plain name).
extern "C" void func_8028C280(CEquipItemBox* self, int kind, int item);

// Same-TU grid helpers used by the equip-slot checks (defined in this .cpp;
// retail plain names). Return u32 (lbz zero-extends; avoids call-site masks).
extern "C" u32 func_80283208(CEquipItemGrid* grid, u16 idx);
extern "C" u32 func_80282EC4(CEquipItemGrid* grid, u16 idx);

// Shared item/bdat table id passed as func_801361E8's group arg by the
// equip-slot checks (same .sbss block as lbl_eu_806640F4).
extern u32 lbl_eu_806640EC;
// Resource/flag helper: 0 when the named resource id is absent (CQuestLog).
extern "C" u32 func_8009CF8C(u32);
// Sort-menu scroll-down / page-up helpers (CSortMenu TU, retail plain names).
extern "C" void func_801D3698(void*);
extern "C" void func_801D3724(void*);
// Item-name string providers (CErrMes TU, current hint/error strings).
extern "C" char* func_eu_802B148C(void);
extern "C" char* func_eu_802B1474(void);
// Layout text bind (code_80135FDC unit): (layout, pane name, text, 0).
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);
// Page-slot word-table accessors used by the grid-cursor scans.
extern "C" u16 ArrayGet12(const u16*, u8);
extern "C" void func_801CB9D8(nw4r::math::VEC3*, const u16*, u32);
// Number-of-equip-pages byte (shared .sbss flag; > 1 means multi-page).
extern "C" u8 code80135FDC_getByte_64077();
// Sort-page colour table pairs (func_80139A18 3rd/4th args, .sbss).
extern s16 lbl_eu_806649B0[4];
extern s16 lbl_eu_806649B8[4];
extern s16 lbl_eu_806649C0[4];
extern s16 lbl_eu_806649C8[4];
extern s16 lbl_eu_806649D0[4];
extern s16 lbl_eu_806649D8[4];

// Small-data global read by func_80285890 (item-impl table id, .sbss).
extern u32 lbl_eu_806640F4;

// .sdata2 float constants used by the grid page-count math (func_80282610).
extern const float lbl_eu_80668B00;
extern const float lbl_eu_80668B08;

// CfMove enum-list holder helpers (retail plain names; sibling decls in
// CItemBoxGrid.hpp / CfObjectImplMove.hpp).
extern "C" void func_80043D90(CEquipItemBoxEnumHolder* holder);
extern "C" CEquipItemBoxEnumList* func_80043F18(CEquipItemBoxEnumHolder* holder);
extern "C" void func_800F4A98(CEquipItemBoxEnumList* list, u32 type, u32 filter);
extern "C" void* func_800F6EC0(CEquipItemBoxEnumList* list, u32 idx);
extern "C" void func_800BFDE0(void* obj, u32 arg);
extern "C" void* func_800BFC68__FPQ22cf12CfObjectMove(void* obj);
extern "C" void __dt__80043E88(CEquipItemBoxEnumHolder* holder, int flags);

// Equipment-slot clear helpers (retail plain names; declared here instead of
// pulling in the big cf/CtrlObjectParam headers).
extern "C" void func_8009E0A8(void*, int);
extern "C" void func_8009E024(void*, int);
extern "C" void func_8009E030(void*, int);
extern "C" void func_8009E03C(void*, int);
extern "C" void func_8009E048(void*, int);
extern "C" void func_8009E054(void*, int);
extern "C" void func_800A1370(void*);

// Item-count / category-base readers (cf/CItem TU, retail plain names).
extern "C" u32 func_80157C20(u8);
extern "C" u32 func_801576C8(u8);

// Char-data status lookup (code_8025FB10 TU): tests a flag at obj+0x3534.
extern "C" int func_8026178C(void*, u32);

// .sdata2 float/double constants used by the equip-box helpers (sda21
// relocs). 80668B10/80668B18 are the int->float conversion magics (0x4330
// trick); 80668B2C is the drop-rate scale; 80668B30/34/38 are pane offsets.
extern const float lbl_eu_80668B2C;
extern const float lbl_eu_80668B48;
extern const float lbl_eu_80668B4C;
extern const float lbl_eu_80668B30;
extern const float lbl_eu_80668B34;
extern const float lbl_eu_80668B38;
extern const float lbl_eu_80668B44;
extern const double lbl_eu_80668B10;
extern const double lbl_eu_80668B18;

// Item-impl accessor (retail CItemImpl vtable-indexed helper).
extern "C" u32 func_801392E4(u32);

// Sub-object destructors invoked by ~CEquipItemBox (retail C-ABI names; the
// member-class dtors live in their own TUs as extern "C" free functions).
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__9CSortMenuFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void* __dt__80285C44(void*, int);
