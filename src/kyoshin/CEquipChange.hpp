#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CEquipItemBox.hpp"
#include "kyoshin/CItemBoxInfo.hpp"

namespace nw4r {
namespace lyt {
    class AnimTransform;
    class DrawInfo;
    class Pane;
}
namespace math {
    struct VEC3;
}
}
class CBaseCur;
struct CItemBoxInfo;
class UnkClass_8045F564;

/* Sets mVtbl before members are constructed (retail ctor order).
   Manual vtable base (non-polymorphic) so MWCC does not emit a vptr re-seat
   store in ~CEquipChange (retail shape) - same trick as CBatteryVtblBase. */
struct CEquipChangeVtblBase {
    void* mVtbl; // 0x0
};

/* Layout verified against extab destroy-member order:
   0x00: IWorkEvent base (vtable ptr)
   0x04: UnkClass_8045F564 (0x10 bytes)
   0x14: UnkClass_8045F564 (0x3C bytes)
   0x50: CCur14           (0x18 bytes)
   0x68: CCur15           (0x18 bytes)
   0x80: CSubCur          (0x24 bytes)
   0xA4: CItemBoxInfo     (0x20C bytes)
   0x2B0: CEquipItemBox */
class CEquipChange : public CEquipChangeVtblBase {
public:
    CEquipChange();
    ~CEquipChange();
    bool OnFileEvent(CEventFile*);
    u8 func_802023C0();
    void func_802023C8();
    void func_8020247C();
    void func_8020397C();
    void func_80203984();
    void func_8020398C();

    // 0x04: UnkClass_8045F564 (size 0x10)
    u8 _pad04[0x10];
    // 0x14: UnkClass_8045F564 (size 0x3C)
    u8 _pad14[0x10];                       // 0x14..0x24
    u32 field_24;                          // 0x24 - file handle 1
    u32 field_28;                          // 0x28 - file handle 2
    nw4r::lyt::ArcResourceAccessor* field_2C; // 0x2C - arc resource accessor
    u32 field_30;                          // 0x30
    u32 field_34;                          // 0x34
    nw4r::lyt::AnimTransform* field_38;    // 0x38
    nw4r::lyt::AnimTransform* field_3C;    // 0x3C
    nw4r::lyt::AnimTransform* field_40;    // 0x40
    u8 field_44;                           // 0x44
    u8 _pad45[0x03];                       // 0x45..0x48
    u32 field_48;                          // 0x48
    u8 field_4C;                           // 0x4C
    u8 field_4D;             // 0x4D
    u8 _pad4E[0x02];
    // 0x50: CCur14 (size 0x18)
    u8 _pad50[0x18];
    // 0x68: CCur15 (size 0x18)
    u8 _pad68[0x18];
    // 0x80: CSubCur (size 0x24)
    u8 field_80[0x18];       // 0x80
    s8 field_98;              // 0x98
    u8 field_99;              // 0x99
    u8 _pad9A[0x0A];          // 0x9A..0xA4
    // 0xA4: CItemBoxInfo (size 0x20C)
    u8 _padA4[0x20C];
    // 0x2B0: CEquipItemBox
    CEquipItemBox mEquipItemBox;  // 0x2B0
};

// Retail symbols are unmangled free functions (not class members); MWCC would
// mangle plain C++ declarations, so they carry C linkage to emit the exact
// retail symbols.
extern "C" int func_80203138(CEquipChange* self);
extern "C" int func_802031A0(CEquipChange* self);

// C++ linkage so MWCC mangles to the retail symbol func_80137444__FPQ34nw4r3lyt13AnimTransformf.
u32 func_80137444(nw4r::lyt::AnimTransform*, float);

// Cast-only view of the layout object at field_34 (vtable slot 11 = +0x2C,
// method (arg, mode)). A real virtual call makes MWCC load the vtable into
// r12 (retail's `lwz r12,0(r3); lwz r12,0x2c(r12); bctrl`), where a
// function-pointer temp would allocate it to a scratch register. MWCC reserves
// 2 leading vtable slots, so v9 lands at +0x2C.
struct CLayoutVtbl11 {
    virtual void v0(int arg);      // +0x08 (slot 0 after 2 reserved)
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9(void* arg, int mode);
    virtual void v10();            // +0x30
    virtual void v11();            // +0x34
    virtual void v12(int arg);     // +0x38
};

// Cast-only view of the sub-cursor at self+0x80 (vtable slot 4 = +0x10,
// method (arg)). Real virtual call emits the retail r12 dispatch chain;
// MWCC reserves 2 leading vtable slots, so v2 lands at +0x10.
struct CSubCurVtblView {
    virtual void v0();
    virtual void v1();
    virtual void v2(void* arg);
};

// Cast-only view of the cursors at self+0x50/0x68/0x80 (vtable slot 3 =
// +0x0C, no-arg method). v1 lands at +0x0C (2 reserved leading slots); v2
// lands at +0x10 (position/colour setter used by func_802040FC).
struct CCurVtblView {
    virtual void v0();
    virtual void v1();            // +0x0C
    virtual void v2(void* arg);   // +0x10
};

// Cast-only view of the layout object at field_34: the pane-finder
// sub-object pointer sits at +0x10 (its vtable carries the +0x3C pane
// method - see CLayoutSubVtbl13).
struct CLayoutView {
    u8 _pad[0x10];
    void* field_10;   // 0x10
};

// Cast-only view of the layout sub-object at *(field_34 + 0x10) (vtable slot
// 15 = +0x3C, method (arg, mode) returning a Pane*). A real virtual call
// makes MWCC load the vtable into r12 (retail dispatch), same rationale as
// CLayoutVtbl11. MWCC reserves 2 leading vtable slots, so v13 lands at +0x3C.
struct CLayoutSubVtbl13 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual nw4r::lyt::Pane* v13(u32 arg, int mode);   // +0x3C
};

// Character-data table returned by func_8009EC9C; +0x176C holds a state word
// that is 1 when the character is busy (same object as CPartyCharData). The
// +0x1C..+0x26 s16 slots hold the per-category equipped item ids (weapon/
// armour rows) read by func_80203210 / func_802042C0.
struct CBdatCharData {
    u8 _pad[0x1C];
    s16 field_1C;   // 0x1C
    s16 field_1E;   // 0x1E
    s16 field_20;   // 0x20
    s16 field_22;   // 0x22
    s16 field_24;   // 0x24
    s16 field_26;   // 0x26
    u8 _pad2[0x176C - 0x28];
    s32 field_176C;   // 0x176C - busy flag
};

// 8-byte colour pair copied from lbl_eu_80508120 by func_802040FC's palette
// copy loop (the loop writes 8 pairs into the 7-pair local - retail shape).
struct CEquipColorPair {
    u32 a;   // 0x0
    u32 b;   // 0x4
};

// 0x38-byte palette block used by func_802040FC's copy (block-copy path
// emits the retail lwzu/stwu counted loop - 7 pairs of 8 bytes).
struct CEquipPaletteBlock {
    CEquipColorPair pair[7];   // 0x38
};

// Cast-only view of nw4r::lyt::Pane exposing the mScale VEC2 at +0x44 as raw
// words (func_802040FC copies it via the GPR struct-copy path - retail shape).
struct CPaneScaleView {
    u8 _pad[0x44];
    CEquipColorPair mScale;    // 0x44 - {x, y} raw words
};

// Retail-unmangled sub-object constructors called by ~CEquipChange/ctor.
extern "C" void __ct__CCur14(void* self, nw4r::lyt::ArcResourceAccessor* arc);
extern "C" void __ct__CCur15(void* self, nw4r::lyt::ArcResourceAccessor* arc);
extern "C" void __ct__CSubCur(void* self, nw4r::lyt::ArcResourceAccessor* arc);
extern "C" void __ct__CItemBoxInfo(void* self, int a, int b);
extern "C" void __ct__CEquipItemBox(void* self);
extern "C" void __ct__UnkClass_8011C974(void* dst, const void* src);
extern "C" void func_8018BE74(void* dst, const void* src);
extern "C" void func_8018B0FC(void* dst, const void* src);
extern "C" void func_8016742C(void* dst, const void* src);

// Manual vtable for the ctor store (retail lis/addi lbl_eu_80535688).
extern char lbl_eu_80535688[];

// Local copy of the CEquipChangeCopyView layout from CPartyStateWin.hpp
// (that header is not includable here - conflicting decls).
struct CEqChStateView {
    u8 f04[0x10];
    u8 f14[0x10];
    u32 f24[8];
    u8 f44;
    u8 _pad45[3];
    u32 f48;
    u8 f4c;
    u8 f4d;
    u8 _pad4e[2];
    u8 f50[0x18];
    u8 f68[0x18];
    u8 f80[0x18];
    u8 f98;
    u8 f99;
    u32 f9a;
    u32 f9e;
    u8 _pada2[6];
    u8 fa8[0x10];
    u8 fb8[0x10];
    u32 fc8[27];
    u8 f134;
    u8 _pad135[3];
    u32 f138;
    u8 f13c;
    u8 f13d;
    u8 f13e;
    u8 _pad13f;
    u32 f140;
    u32 f144;
    u32 f148;
    u32 f14c;
    u16 f150;
    u8 f152;
    u8 _pad153;
    u8 f154[0x160];
    u8 f2b4[0x10];
    u8 f2c4[0x10];
    u32 f2d4[7];
    u8 f2f0[4];
    u8 f2f4[0x18];
    u8 f30c[0x18];
    u8 f324[0x1c];
    u8 f340[0x10];
    u32 f350[5];
    u8 f364[4];
    u8 _pad368[4];
    u8 f36c[0x10];
    u32 f37c[4];
    u8 f38c[4];
    f32 f390[5];
    u8 f3a4;
    u8 _pad3a5[3];
    u8 pairs16[0x80];
    u8 f428[3];
    u8 _pad42b;
    u8 f42c[0x3c];
    u8 f468[0x3c];
    u8 f4a4[3];
    u8 _pad4a7;
    u16 f4a8;
    u16 f4aa;
    u16 f4ac;
    u8 f4ae;
    u8 _pad4af;
    u32 f4b0;
    u32 f4b4;
    u32 f4b8;
    u32 f4bc;
    u8 f4c0[0x15c];
    u32 f61c;
    u16 f620;
    u8 f622[0xe];
    u8 f62e[0x2000];
    u16 f262e;
    u8 f2630[4];
    u8 f2634[0x20];
    u8 f2650[4];
    u8 f2654[0x400];
    u32 f2a54;
};

/* Stack copy sources for the ctor: fresh CItemBoxInfo(4,0) and CEquipItemBox
   temporaries whose config regions are copied into the members field-by-field.
   Offsets mirror CEquipChangeCopyView shifted by the member bases (-0 for the
   box info temp, -0x2B0 for the equip item box temp). */
struct CEqChBoxTemp {
    u32 _00;               // 0x00
    u8 f04[0x10];          // 0x04 (__ct__UnkClass_8011C974)
    u8 f14[0x10];          // 0x14 (__ct__UnkClass_8011C974)
    u32 f24[27];           // 0x24..0x90
    u8 f134;               // 0x90
    u8 _pad91[3];
    u32 f138;              // 0x94
    u8 f13c;               // 0x98
    u8 f13d;               // 0x99
    u8 f13e;               // 0x9a
    u8 _pad13f;
    u32 f140;              // 0x9c
    u32 f144;              // 0xa0
    u32 f148;              // 0xa4
    u32 f14c;              // 0xa8
    u16 f150;              // 0xac
    u8 f152;               // 0xae
    u8 _pad153;
    u8 f154[0x160];        // 0xb0..0x210 (func_8018BE74)
};

struct CEqChEquipTemp {
    u8 f04[0x10];          // 0x04 (__ct__UnkClass_8011C974)
    u8 f14[0x10];          // 0x14 (__ct__UnkClass_8011C974)
    u32 f24[7];            // 0x24..0x40
    u8 f2f0[4];            // 0x40
    u8 f2f4[0x18];         // 0x44 (func_801FA220)
    u8 f30c[0x18];         // 0x5c (func_801FA220)
    u8 f324[0x1c];         // 0x74 (func_8018B0FC)
    u8 f340[0x10];         // 0x90 (__ct__UnkClass_8011C974)
    u32 f350[5];           // 0xa0..0xb4
    u8 f364[4];            // 0xb4..0xb8
    u8 _padb8[4];
    u8 f36c[0x10];         // 0xbc (__ct__UnkClass_8011C974)
    u32 f37c[4];           // 0xcc..0xdc
    u8 f38c[4];            // 0xdc..0xe0
    f32 f390[5];           // 0xe0..0xf4
    u8 f3a4;               // 0xf4 (counted-pair loop base)
    u8 _pad3a5[3];
    u8 pairs16[0x78];      // 0xf8..0x170 (16 records)
    u8 f428[3];            // 0x178..0x17b
    u8 _pad42b;
    u8 f42c[0x3c];         // 0x17c (func_8016742C)
    u8 f468[0x3c];         // 0x1b8 (func_8016742C)
    u8 f4a4[3];            // 0x1f4..0x1f7
    u8 _pad4a7;
    u16 f4a8;              // 0x1f8
    u16 f4aa;              // 0x1fa
    u16 f4ac;              // 0x1fc
    u8 f4ae;               // 0x1fe
    u8 _pad4af;
    u32 f4b0;              // 0x200
    u32 f4b4;              // 0x204
    u32 f4b8;              // 0x208
    u32 f4bc;              // 0x20c
    u8 f4c0[0x15c];        // 0x210..0x36c (func_8018BE74)
    u32 f61c;              // 0x36c
    u16 f620;              // 0x370
    u8 f622[0xe];          // 0x372..0x380 (&f622[8] is the loop base at 0x37a)
    u8 _pad380[0x1ffe];    // 0x380..0x237e
    u16 f262e;             // 0x237e
    u8 f2630[4];           // 0x2380 (loop base for f2634)
    u8 f2634[0x1c];        // 0x2384..0x23a0 (4 records)
    u8 f2650[4];           // 0x23a0 (loop base for f2654)
    u8 f2654[0x3f8];       // 0x23a4..0x279c (0x80 records)
    u32 f27a4;             // 0x27a4
    u8 _padTail[0x10];     // frame pad - retail reserves 0x27b8 for this temp
};

// Color/sound palette entries initialised by sinit_802059E8 (sdata2).
extern void* lbl_eu_80664668;
extern void* lbl_eu_80664670;
extern void* lbl_eu_80664678;
extern void* lbl_eu_80664680;
extern void* lbl_eu_80664688;
extern void* lbl_eu_80664690;

// Global equip-page flag (sdata), cleared by func_8020228C.
extern u32 lbl_eu_80664698;

// Palette source (16 u32 colours) and string pool used by func_802040FC and
// the bind-file loader func_80202090.
extern u32 lbl_eu_80508120[];
extern char lbl_eu_80508168[];

// Sub-cursor equip lookup tables (func_80202EB4): a u32 category word at
// byte offset 2*i plus a byte at 2*i+4 per row i. Fixed sizes keep the
// symbols sdata-eligible so MWCC emits the retail sda21 addressing.
extern u8 lbl_eu_806682A0[8];
extern u8 lbl_eu_806682A4[8];

// Cast-only view of the object returned by CItem_initItemImplInstances
// (vtable slot 0x30 = +0x30, method (item) returning the equipped count as
// u8). MWCC reserves 2 leading vtable slots, so the +0x30 method is the
// 11th declared (v10). v9 at +0x2C is the item-impl fetch used by
// func_802052A8; v15 at +0x44 is the equip/unequip hook used by
// func_80203210 (args item, mode, -1).
struct CItemImplVtblView {
    virtual void v0();                    // +0x08
    virtual void v1();                    // +0x0C
    virtual void v2();                    // +0x10
    virtual void v3();                    // +0x14
    virtual void v4();                    // +0x18
    virtual void v5();                    // +0x1C
    virtual void v6();                    // +0x20
    virtual void v7();                    // +0x24
    virtual void v8();                    // +0x28
    virtual void* v9(void* item, u32 flag);  // +0x2C
    virtual u8 v10(void* item);              // +0x30 equipped count
    virtual void v11();                      // +0x34
    virtual void v12();                      // +0x38
    virtual void v13();                      // +0x3C
    virtual void v14();                      // +0x40
    virtual void v15(void* item, u8 flag, int arg);  // +0x44
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail emits these as C-style (unmangled) symbols, so reference them with C
// linkage. func_801D2ED8/CEquipItemBox gates take the object and return status.
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);

// CItemBoxInfo helpers used by the equip-change screen (C-ABI retail names).
extern "C" u32 func_801D421C(CItemBoxInfo* info);
extern "C" u32 func_801D4260(CItemBoxInfo* info, u8 arg2);
extern "C" void func_801D47D4(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4);
extern "C" void func_801D202C(void* cur);
extern "C" void func_801D40C4(CItemBoxInfo* info);
extern "C" void func_801D4174(CItemBoxInfo* info);

// Free-function form (retail: advanceItemBoxState__FP12CItemBoxInfo).
void advanceItemBoxState(CItemBoxInfo* info);

extern "C" void func_80286264(CEquipItemBox* box);
extern "C" void func_80286454(CEquipItemBox* box);
extern "C" void func_802867E0(CEquipItemBox* box);
extern "C" int func_801D2ED8(CBaseCur*);
extern "C" int func_802865A0(CEquipItemBox* box);
extern "C" int func_802865A8(CEquipItemBox* box);
extern "C" int func_802865B0(CEquipItemBox* box);
extern "C" void func_80286740(CEquipItemBox* box);
extern "C" void func_80287250(CEquipItemBox* box, int arg);
extern "C" void func_801D4B3C(void*, CItemBoxInfo*, u32);
extern "C" int func_80286650(CEquipItemBox* box);
extern "C" int func_80286698(CEquipItemBox* box);
extern "C" int func_802866A0(CEquipItemBox* box);
extern "C" int func_8028652C(CEquipItemBox* box);
extern "C" int func_80287EE8(CEquipItemBox* box);
extern "C" void func_802870DC(CEquipItemBox* box);
extern "C" void func_802040FC(CEquipChange* self);
extern "C" void func_801D4054(void* info);
extern "C" void func_802861A8(CEquipItemBox* box);
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void func_802042C0(CEquipChange* self);
extern "C" void* func_8009EC9C(u32);
extern "C" void* func_80157C4C(u32 index, s16 value);
extern "C" void func_80287EFC(CEquipItemBox* box, u32 val);
extern "C" void func_80287DB4(CEquipItemBox* box, u16 packed, void* arg3, u16 arg4);
extern "C" void func_802866E8(CEquipItemBox* box);
extern "C" int func_8028876C(CEquipItemBox* box);
extern "C" void func_80286340(CEquipItemBox* box, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void* func_802052A8(CEquipChange* self);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D4154(CItemBoxInfo* info, nw4r::lyt::DrawInfo* drawInfo);

// CEquipItemBox gate/handler imports used by the equip-change handlers.
extern "C" void func_802869B4(CEquipItemBox* box);
extern "C" void func_80286B94(CEquipItemBox* box);
extern "C" void func_80286D7C(CEquipItemBox* box);
extern "C" int func_80288948(CEquipItemBox* box);
extern "C" int func_80288530(CEquipItemBox* box);
extern "C" int func_8028847C(CEquipItemBox* box);
extern "C" int func_80288544(CEquipItemBox* box);

// bdat item-name table (sdata pointer) and message-count lookup used by
// func_80203210's category equip checks.
extern u32 lbl_eu_806640EC;
extern "C" u32 func_8009CF8C(u32 resourceId);

// Per-category equipped-item row setters (func_80203210 case tails).
extern "C" void func_8009E0A8(void*, int);
extern "C" void func_8009E024(void*, int);
extern "C" void func_8009E030(void*, int);
extern "C" void func_8009E03C(void*, int);
extern "C" void func_8009E048(void*, int);
extern "C" void func_8009E054(void*, int);
extern "C" void func_800A1370(void*);

// 3-word scene-name table read by func_80203210's refresh block (.rodata).
extern u32 lbl_eu_80508068[3];

// CfObjEnumList helper family (opaque void* params repo-wide - see
// CPartyStateWin.hpp); struct views for the stack holder / list / slot used
// by func_80203210's refresh block.
struct CEquipEnumHolder { void* field_0x0; u32 field_0x4; };
struct CEquipEnumList { u8 _00[0x620]; u32 field_0x620; };
struct CEquipEnumListSlot { u8 _00[0x4]; void* field_0x4; };
extern "C" void func_80043D90(void*);
extern "C" void* func_80043F18(void*);
extern "C" void func_800F4A98(void*, u32, u32);
extern "C" void* func_800F6EC0(void*, u32);
extern "C" void* func_800BFC68__FPQ22cf12CfObjectMove(void* objMove);
extern "C" void func_800BFDE0(void* obj, u32 flag);
extern "C" void __dt__80043E88(void*, int);
extern "C" int func_80287D58(CEquipItemBox* box);
extern "C" int func_802882A4(CEquipItemBox* box);
extern "C" void func_802873D8(CEquipItemBox* box);
extern "C" void func_80287F04(CEquipItemBox* box, u16 arg2, void* arg3);
extern "C" void func_80202EB4(CEquipChange* self, u8 cat);

// Sub-cursor activate (defined in CCur.cpp).
extern "C" void func_801D2174(CBaseCur* cur);

// Accumulate a pane's translate into output (CPartyState.hpp also declares
// this C-ABI helper).
extern "C" void func_801375A0(nw4r::math::VEC3* output, nw4r::lyt::Pane* pane);

// Sub-object destructors referenced by ~CEquipChange. Defined in CCur.cpp /
// CItemBoxInfo.cpp / CEquipItemBox.cpp.
extern "C" void* __dt__7CSubCurFv(CBaseCur* _this, int flags);
extern "C" void* __dt__6CCur14Fv(CBaseCur* _this, int flags);
extern "C" void* __dt__6CCur15Fv(CBaseCur* _this, int flags);
extern "C" void __dt__17UnkClass_8045F564Fv(void* _this, int flags);
extern "C" void* __dt__12CItemBoxInfoFv(CItemBoxInfo* _this, int flags);
extern "C" void* __dt__13CEquipItemBoxFv(CEquipItemBox* _this, int flags);
