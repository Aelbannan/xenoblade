#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CEquipItemBox.hpp"
#include "kyoshin/plugin/ocBdat.hpp"  // getBdatStringColumnValue
/* CItemBoxInfo.hpp declares func_801393CC(void*) while CEquipItemBox.hpp
   (included above) already declares the real u32(u32) - MWCC rejects the
   overload. This unit never calls it, so rename the stale declaration away
   for this inclusion only. */
#define func_801393CC func_801393CC_stale_voidp
#define GetCollectedFlagByte GetCollectedFlagByte_stale_int_u8
#define func_80136A1C func_80136A1C_stale_constcharp
#define func_801397AC func_801397AC_stale_gxcolor
#include "kyoshin/CItemBoxInfo.hpp"
#undef func_801393CC
#undef GetCollectedFlagByte
#undef func_80136A1C
#undef func_801397AC

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
    CEquipChange();   // not defined here - retail's "ctor" is the unmangled
                      // free function __ct__CEquipChange below
    ~CEquipChange();
    bool OnFileEvent(CEventFile*);
    u8 EquipChange_IsActiveFlag();
    void EquipChange_CheckBoxOpen();
    void EquipChange_IsMenuBusy();
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
extern "C" int EquipChange_MapCursorToCat(CEquipChange* self);
extern "C" int EquipChange_MapCursorToSlot(CEquipChange* self);

// C++ linkage so MWCC mangles to the retail symbol advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf.
u32 advanceAnimTransform(nw4r::lyt::AnimTransform*, float);

struct CEqChCursorRec {
    u16 field_00;
    u32 field_04;
};

// Per-slot bdat message-table handles read indexed by func_802042C0
// (lbl_eu_806640D8 is an array of handles; the scalar extern comes from
// CEquipItemBox.hpp).
#define lbl_eu_806640D8_arr ((void**)(void*)&lbl_eu_806640D8)

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

// 8-byte colour pair copied from lbl_eu_80508120 by EquipChange_RefreshCursorPos's palette
// copy loop (the loop writes 8 pairs into the 7-pair local - retail shape).
struct CEquipColorPair {
    u32 a;   // 0x0
    u32 b;   // 0x4
};

// 0x38-byte palette block used by EquipChange_RefreshCursorPos's copy (block-copy path
// emits the retail lwzu/stwu counted loop - 7 pairs of 8 bytes).
struct CEquipPaletteBlock {
    CEquipColorPair pair[7];   // 0x38
};

// Cast-only view of nw4r::lyt::Pane exposing the mScale VEC2 at +0x44 as raw
// words (EquipChange_RefreshCursorPos copies it via the GPR struct-copy path - retail shape).
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
extern "C" void __ct__UnkClass_8011C974(void* dst, void* src);
extern "C" void func_8018BE74(void* dst, void* src);
extern "C" void func_8018B0FC(void* dst, void* src);
extern "C" void func_8016742C(void* dst, void* src);

// Manual vtable for the ctor store (retail lis/addi lbl_eu_80535688).
extern char lbl_eu_80535688[];

// Local copy of the CEquipChangeCopyView layout from CPartyStateWin.hpp
// (that header is not includable here - conflicting decls).
struct CEqChStateView {
    u8 _00[4];             // 0x00 vtable word
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
    u8 _pad9a[0xE];        // 0x9A..0xA7 (keeps fa8 at 0xA8)
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
    u8 f622[0xc];          // 0x622..0x62D
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
    u32 _00;               // 0x00 (vtable-slot word never copied; f04 sits at +4)
    u8 f04[0x10];          // 0x04 (__ct__UnkClass_8011C974)
    u8 f14[0x10];          // 0x14 (__ct__UnkClass_8011C974)
    u32 f24[7];            // 0x24..0x40
    u8 f2f0[4];            // 0x40
    u8 f2f4[0x18];         // 0x44 (PartyStateWin_CopySlotRec)
    u8 f30c[0x18];         // 0x5c (PartyStateWin_CopySlotRec)
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
    u8 pairs16[0x80];      // 0xf8..0x177 (16 x 8-byte records)
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
    u8 f2634[0x1c];        // 0x2384..0x239f (4 records)
    u8 f2650[4];           // 0x23a0 (loop base for f2654)
    u8 f2654[0x400];       // 0x23a4..0x27a3 (0x80 records)
    u32 f27a4;             // 0x27a4 (temp ends here - retail boxTmp starts at sp+0x27b0)
};

// Color/sound palette entries initialised by sinit_802059E8 (sdata2).
extern void* lbl_eu_80664668;
extern void* lbl_eu_80664670;
extern void* lbl_eu_80664678;
extern void* lbl_eu_80664680;
extern void* lbl_eu_80664688;
extern void* lbl_eu_80664690;

// Global equip-page flag (sdata), cleared by EquipChange_CleanupFiles.
extern u32 lbl_eu_80664698;

// Palette source (16 u32 colours) and string pool used by EquipChange_RefreshCursorPos and
// the bind-file loader EquipChange_LoadBindFiles.
extern u32 lbl_eu_80508120[];
extern char lbl_eu_80508168[];

// Sub-cursor equip lookup tables (func_80202EB4): a u32 category word at
// byte offset 2*i plus a byte at 2*i+4 per row i. Fixed sizes keep the
// symbols sdata-eligible so MWCC emits the retail sda21 addressing.
extern u8 lbl_eu_806682A0[8];
extern u8 lbl_eu_806682A4[8];

// Result of the item-impl fetch hook
// func_802052A8: packed word at +0 and a halfword at +4 whose low bit picks
// between the direct-equip and bdat-rebuild paths.
struct CEquipV9Result {
    u32 field_00;
    u16 field_04;
};

// Scratch CItem record rebuilt from bdat by func_802052A8's fallback path
// (cleared once, guarded by lbl_eu_8066469C).
struct CEquipWorkItem {
    u32 field_00;
    u16 field_04;
    u8 _pad06[0xF8];
};
extern CEquipWorkItem lbl_eu_80576568;
extern s8 lbl_eu_8066469C;

// bdat helpers used by func_802052A8's rebuild path.
extern "C" u32 Bdat_GetMaxRow_B1EC(void* file);
extern "C" void func_80159F6C(void* self, u32 family, u32 row, u16 kind);


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail emits these as C-style (unmangled) symbols, so reference them with C
// linkage. Cur_BothSubPanesOn/CEquipItemBox gates take the object and return status.
extern "C" u32 AnimRewindFrame(nw4r::lyt::AnimTransform*, float);

// CItemBoxInfo helpers used by the equip-change screen (C-ABI retail names).
extern "C" u32 startItemBoxOpen(CItemBoxInfo* info);
extern "C" u32 func_801D4260(CItemBoxInfo* info, u8 arg2);
extern "C" void func_801D47D4(CItemBoxInfo* info, u16 arg2, void* arg3, u16 arg4);
extern "C" void func_801D202C(void* cur);
extern "C" void updateItemBoxAnims(CItemBoxInfo* info);
extern "C" void func_801D4174(CItemBoxInfo* info);

// Free-function form (retail: advanceItemBoxState__FP12CItemBoxInfo).
void advanceItemBoxState(CItemBoxInfo* info);

extern "C" void updateEIBBox(CEquipItemBox* box);
// 2-arg subcur visibility setter (retail unmangled symbol; see CCur.cpp).
extern "C" void Cur_ShowTwoSubPanes(void*, u32);
extern "C" void closeEIBBox(CEquipItemBox* box);
extern "C" void eibNavLeft(CEquipItemBox* box);
extern "C" int Cur_BothSubPanesOn(CBaseCur*);
extern "C" int getEIBOpenFlag(CEquipItemBox* box);
extern "C" int getEIBActiveMark(CEquipItemBox* box);
extern "C" int eibInputBlocked(CEquipItemBox* box);
extern "C" void finishEIBEntry(CEquipItemBox* box);
extern "C" void eibHandleSubPage(CEquipItemBox* box, int arg);
extern "C" void calcItemBoxPaneVec(void*, CItemBoxInfo*, u32);
extern "C" int eibMenuBusy(CEquipItemBox* box);
extern "C" int getEIBNamePane(CEquipItemBox* box);
extern "C" int eibSysWinBusy(CEquipItemBox* box);
extern "C" int eibWindowsReady(CEquipItemBox* box);
extern "C" int takeEIBAction(CEquipItemBox* box);
extern "C" void eibConfirmSort(CEquipItemBox* box);
extern "C" void EquipChange_RefreshCursorPos(CEquipChange* self);
extern "C" void loadItemBoxFiles(void* info);
extern "C" void loadEIBFiles(CEquipItemBox* box);
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void func_802042C0(CEquipChange* self);
extern "C" void CtrlObjectParam_SyncParamFromActorEx(void* charObj, u32 flag);
extern "C" void CtrlObjectParam_GetArtsStatsRow(void* rows, u32 count);
extern "C" void* func_8009EC9C(u32);
extern "C" void* func_80157C4C(u32 index, s16 value);
extern "C" void setEIBBoxInfo(CEquipItemBox* box, u32 val);
extern "C" void eibOpenPages(CEquipItemBox* box, u16 packed, void* arg3, u16 arg4);
extern "C" void openEIBBox(CEquipItemBox* box);
extern "C" int eibHudPrompt(CEquipItemBox* box);
extern "C" void drawEIBBox(CEquipItemBox* box, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void* func_802052A8(CEquipChange* self);
extern "C" void Cur_DrawLayout(void*, void*);
extern "C" void renderItemBox(CItemBoxInfo* info, nw4r::lyt::DrawInfo* drawInfo);

// CEquipItemBox gate/handler imports used by the equip-change handlers.
extern "C" void func_802869B4(CEquipItemBox* box);
extern "C" void eibNavUp(CEquipItemBox* box);
extern "C" void eibNavRight(CEquipItemBox* box);
extern "C" int func_80288948(CEquipItemBox* box);
extern "C" int getEIBSelCat(CEquipItemBox* box);
extern "C" int eibRowMatches(CEquipItemBox* box);
extern "C" int findEIBEquipSlot(CEquipItemBox* box);

// bdat item-name table (sdata pointer) and message-count lookup used by
// func_80203210's category equip checks.
extern u32 lbl_eu_806640EC;
extern "C" u32 CtrlRemote_TouchBitByArg(u32 resourceId);

// Per-category equipped-item row setters (func_80203210 case tails).
extern "C" void CtrlObjectParam_SetEquipSlot5(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot0(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot1(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot2(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot3(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot4(void*, int);
extern "C" void CtrlObjectParam_SyncParamFromActor(void*);

// 3-word scene-name table read by func_80203210's refresh block (.rodata).
extern u32 lbl_eu_80508068[3];

// CfObjEnumList helper family (opaque void* params repo-wide - see
// CPartyStateWin.hpp); struct views for the stack holder / list / slot used
// by func_80203210's refresh block.
struct CEquipEnumHolder { void* field_0x0; u32 field_0x4; };
struct CEquipEnumList { u8 _00[0x620]; u32 field_0x620; };
struct CEquipEnumListSlot { u8 _00[0x4]; void* field_0x4; };
extern "C" void CTaskGame_enumListCtor(void*);
extern "C" void* CTaskGame_enumListGet(void*);
extern "C" void startEnumObjects(void*, u32, u32);
extern "C" void* getEntryAt(void*, u32);
extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void* objMove);
extern "C" void func_800BFDE0(void* obj, u32 flag);
extern "C" void __dt__80043E88(void*, int);
extern "C" int stepEIBSysWin2(CEquipItemBox* box);
extern "C" int func_802882A4(CEquipItemBox* box);
extern "C" void func_802873D8(CEquipItemBox* box);
extern "C" void eibApplySelect(CEquipItemBox* box, u16 arg2, void* arg3);
extern "C" void func_80202EB4(CEquipChange* self, u8 cat);

// Sub-cursor activate (defined in CCur.cpp).
extern "C" void Cur_SetActive(CBaseCur* cur);

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
