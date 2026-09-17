#pragma once

#include <types.h>
#include "kyoshin/plugin/ocBdat.hpp"

#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include "revolution/gx/GXTypes.h"
#include <revolution/mtx/vec.h>

#include <nw4r/lyt.h>
#include <nw4r/math/math_types.h>
#include <nw4r/ut/ut_Font.h>

#include "monolib/math/CVec3.hpp"
#include "monolib/math/CMat34.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)
#include "kyoshin/cf/object/CfObject.hpp"
#include <nw4r/lyt/lyt_material.h>

class CScn;

// All definitions in code_80135FDC.cpp are C-linkage (extern "C"), so the
// declarations MUST match -- otherwise callers emit Itanium-mangled relocs
// (e.g. BdatGetPtrDirect__FPvPcUs) while retail has the unmangled name, and the
// witness rejects the pair at the reloc gate (us-8025658c / us-8025650c were
// stuck at 99.4-99.6% on exactly this).
// Params use const void* so both const char* (CArtsInfo, CExchangeWin) and
// const void* (CCollepedia) call sites bind without conversions.
extern "C" {
u16 BdatGetU16ByTableKey(const void*, const void*, u32);
u8 BdatGetU8ByTableKey(const void*, const void*, u32);
s16 BdatGetS16ByTableKey(const void*, const void*, u32);
s8 BdatGetS8ByTableKey(const void*, const void*, u32);
#ifndef CODE_80135FDC_CPP
// The definitions of these in code_80135FDC.cpp use different (not-yet-)
// matched signatures/linkage; code_80135FDC.cpp defines CODE_80135FDC_CPP
// before including so these caller-tuned exports don't clash with them.
extern "C" char* BdatTouchStringCell(const void*, const void*, int);
u8 BdatGetU8Direct(u32, const char*, u32);
char* BdatGetPtrDirect(const void*, const void*, int);
void func_8013676C(void*, u32);
#endif
void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);
void LayoutSetTextBoxFmtValue(nw4r::lyt::Layout*, char*, char*, u32);
// Retail symbols for these ARE the Itanium-mangled names; declare them under
// C linkage so calls bind to the literal mangled identifier (declaring them
// as C++ functions would re-derive the same mangling, but the extern "C" +
// mangled-identifier form is what the definitions in code_80135FDC.cpp use).
void setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(nw4r::lyt::Layout*, char*, u32);
void setLayoutTextBoxNumber__FPQ34nw4r3lyt6LayoutPcUc(nw4r::lyt::Layout*, char*, u8);
void buildLayout__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
}

// Retail symbols that ARE the mangled C++ names stay C++-linkage. Declare the
// UNMANGLED identifier so MWCC mangles drawLayout to the retail symbol
// (drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii); declaring the
// pre-mangled name here would double-mangle it (see CEquipItemBox.hpp).
// The extern "C" + mangled-identifier declarations above serve the pre-mangled
// call sites (CMenuUpdate, CMCCrystalSupport, CMCEffStart, CPcKizunagram); the
// unmangled forms below serve the (majority) unmangled call sites. Both bind
// to the same retail symbols.
void setLayoutTextBoxFont(nw4r::lyt::Layout*, char*, u32);
void setLayoutTextBoxNumber(nw4r::lyt::Layout*, char*, u8);
void buildLayout(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void bindLayoutAnimTransform(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
void drawLayout(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_80137250(nw4r::lyt::DrawInfo* pDrawInfo);
u32 advanceAnimTransform(nw4r::lyt::AnimTransform*, float);
void closeFileHandle(CFileHandle**);
void releaseArcResourceAccessor(nw4r::lyt::ArcResourceAccessor*);

// Retail symbols are unmangled (C linkage) -- keep them extern "C" so calls
// bind to the unmangled name, not an Itanium-mangled re-derivation.
extern "C" {
#ifndef CODE_80135FDC_CPP
u8 MapValueToRank6(u32);
#endif
u32 AnimRewindFrame(nw4r::lyt::AnimTransform*, float);
#ifndef CODE_80135FDC_CPP
void PaneSetTexPaletteByName(void*, void*, void*);
char* MakeTplNameSysFile(u32);
void PaneMatSetTevColorsByName(void*, void*, void*, void*);
#endif
}
extern "C" int IsMenuState621F0();
extern "C" u8 code80135FDC_getByte_64077();

// Frame object returned by Scn_HasCamItem as consumed by drawLayout: the
// view matrix lives at +0xCC (3x4) and the projection matrix at +0x194.
// Struct assignment of the matrix member makes MWCC emit the retail lwz/stw
// pair copy inline (a u32 loop would degrade to sequential loads/stores).

// func_80136A1C/func_80136D74 dispatch through real classes (no local pads):
// - tagProc slot 0x14 is CTagProcessorBase::Proc (kyoshin/CTagProcessor.hpp,
//   retail func_80125B58); the TU-local decl lives in code_80135FDC.cpp
//   (CTagProcessor.hpp cannot be included here: its byte-wise ut::Color
//   clashes with the lyt headers above).
// - the slot-0x7C string setter is nw4r::lyt::TextBox::SetString.

struct CViewFrame37038 {
    /* 0x000 */ u8 pad00[0xCC];
    /* 0x0CC */ nw4r::math::MTX34 mtx;
    /* 0x0FC */ u8 padFC[0x98];
    /* 0x194 */ f32 projection[4][4];
};

// BDAT filename pointer tables copied to the stack by func_8013ACFC. Retail
// copies them with inline mtctr/lwzu/stwu 8-byte loops; struct assignment is
// the only source shape that makes MWCC emit those loops (memcpy would emit
// bl memcpy).
struct XBMapTable1 { u32 w[35]; };   // 140 B -> 17x8 loop + tail word
struct XBMapTable2 { u32 w[31]; };   // 124 B -> 15x8 loop + tail word
struct XBMapTable3 { u32 w[34]; };   // 136 B -> 17x8 loop, no tail
#ifndef CODE_80135FDC_CPP
extern "C" u8 GetCollectedFlagByte(u32);
#endif

// Pane colour / material dispatches (func_80137B44 and siblings) go through
// real nw4r virtuals: slot 0x28 is Pane::SetVtxColor, slot 0x3C is
// Pane::FindPaneByName, slot 0x68 is Pane::GetMaterial, slots 0x58/0x64/0x6C
// are Pane::FindAnimationLinkSelf/GetMaterialNum/GetMaterial, and slot 0x24
// is Material::FindAnimationLink. No local pads.

// ---------------------------------------------------------------------------
// C-linkage imports - moved verbatim from code_80135FDC.cpp. These are retail
// linker symbol names; keep linkage/signatures exactly as-is (do not "fix" the
// pre-mangled identifiers, they are the retail names).
// ---------------------------------------------------------------------------
extern "C" {
// Block A - imports

extern nw4r::lyt::Layout* createLayout__10CLibLayoutFv();
extern u32 getAllocHandle__10CLibLayoutFv();
extern void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
extern void deallocate__Q23mtl10MemManagerFPv(void*);
extern void Set__Q34nw4r3lyt12AnimResourceFPCv(void*, const void*);
extern void PSVECNormalize(const Vec*, Vec*);
extern void* getCurrentView__5CViewFv();
extern void* Scn_HasCamItem(void*, void*);
extern void SetFont__Q34nw4r3lyt7TextBoxFPCQ34nw4r2ut4Font(nw4r::lyt::TextBox*, const nw4r::ut::Font*);
extern void func_8006A234(u16*, u16*);
extern int func_8006A6D0();
extern u16 func_8016DF2C();
extern void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern u8 lbl_eu_8052E524[];
extern u8 lbl_eu_8052E4F0[];
extern u32 lbl_eu_805001C0[28];
extern char lbl_eu_80573B30[];
extern char lbl_eu_80573BB0[];

// globals (Block C)
extern u8 lbl_eu_806621F0;
extern u8 lbl_eu_80664058;
extern u8 lbl_eu_80664059;
extern u8 lbl_eu_8066405A;
extern u8 lbl_eu_8066405B;
extern u32 lbl_eu_8066405C;
extern u32 lbl_eu_80664060;
extern u8 lbl_eu_80664064;
extern u32 lbl_eu_80664068;
extern u32 lbl_eu_8066406C;
// 7-byte indexed buffer (GetCollectedFlagByte indexes &lbl[idx]); was scalar u8.
extern u8 lbl_eu_80664070[7];
extern u8 lbl_eu_80664077;
extern u16 lbl_eu_80664078[3];
extern u8 lbl_eu_8066407E;
extern u8 lbl_eu_8066407F;
extern u8 lbl_eu_80664080[8];
extern u32 lbl_eu_80664098;
extern u32 lbl_eu_806640F0;
extern u32 lbl_eu_806640F4;
extern u32 lbl_eu_806640F8;
extern u32 lbl_eu_806640FC;
extern u32 lbl_eu_80664108;
extern u32 lbl_eu_8066410C;
extern u32 lbl_eu_80664110;
extern u32 lbl_eu_80664184;
extern f32 lbl_eu_806672D8;
extern f32 lbl_eu_806672DC;
extern f32 lbl_eu_806672E0;
extern f32 lbl_eu_806672E4;
extern f32 lbl_eu_806672E8;
extern f32 lbl_eu_806672EC;
// 672F0 is const: MWCC routes a non-const 0.0f to .sbss2 despite the
// section attribute (all other pool floats survive as non-const).
extern const f32 lbl_eu_806672F0;
extern f64 lbl_eu_806672F8;
extern f32 lbl_eu_80667300;
extern f32 lbl_eu_80667304;
extern f32 lbl_eu_80667308;
extern f32 lbl_eu_8066730C;
// 67310-6733C hold face-string pointers (read as u32 bit patterns via the
// float externs below by func_8013A7D0/AC3C); typed as pointers here.
extern const char* lbl_eu_80667310;
extern const char* lbl_eu_80667314;
extern const char* lbl_eu_80667318;
extern const char* lbl_eu_8066731C;
extern const char* lbl_eu_80667320;
extern const char* lbl_eu_80667324;
extern const char* lbl_eu_80667328;
extern const char* lbl_eu_8066732C;
extern const char* lbl_eu_80667330;
extern const char* lbl_eu_80667334;
extern const char* lbl_eu_80667338;
extern const char* lbl_eu_8066733C;
extern f32 lbl_eu_80667340;
extern f32 lbl_eu_80667344;
extern f32 lbl_eu_80667348;
extern f64 lbl_eu_80667350;
extern f64 lbl_eu_80667358;
extern f64 lbl_eu_80667360;
#ifndef CODE_80135FDC_CPP
#endif
extern const float lbl_eu_8066A20C;
extern u32 lbl_eu_806621F4;
extern const char lbl_eu_80500664[];

// Face pane names (defined in code_80135FDC.cpp .rodata; fwd here so the
// .sdata2 pointer table can precede them in TU layout experiments).
extern const char lbl_eu_80500248[0x0C];
extern const char lbl_eu_80500254[0x0C];
extern const char lbl_eu_80500260[0x0C];
extern const char lbl_eu_8050026C[0x0C];

// string/rodata data (Block B)
extern u8 lbl_eu_8052CB40[];
extern u8 lbl_eu_8052CB1C[];
extern u8 lbl_eu_8052E4E4[];
extern u8 lbl_eu_8052E4C0[];
extern u8 lbl_eu_8052E558[];
extern u8 lbl_eu_8052E530[];
extern const char lbl_eu_80526324[]; // const: matches CActParamAnimGame.cpp definition + CfSoundMan.hpp chain
extern const char lbl_eu_80526300[];  // const char: matches CfSoundMan.hpp chain (u8/char variants broke CMiniMap TU)
extern u8 lbl_eu_805262F0[];
extern u8 lbl_eu_805262C8[];
extern u8 lbl_eu_80500108[];
extern u8 lbl_eu_80500230[];
// Minimap pointer tables: heterogeneous (strings + NULLs); entries are relocs.
extern const void* lbl_eu_80500480[32];
extern const void* lbl_eu_805005A8[34];
// 13-entry float table copied wholesale to the stack by GetFloatTableEntry.
struct FloatTable13 { f32 w[13]; };
extern FloatTable13 lbl_eu_80500630;
extern char lbl_eu_80573C30[];
extern ml::CVec3 zero__Q22ml5CVec3;

// helpers
// (func_8003B1EC / CtrlObjectParam_GetSlotTableBase / func_8009D018 stay inline in
//  code_80135FDC.cpp - functions.hpp / CChainActorPc.hpp / CKizunaTalkList.hpp
//  declare them with different signatures and are co-included by callers)
extern "C" void* func_8009EC9C(u32 idx);
extern "C" int CtrlObjectParam_GetItemBitFlag(u32 idx);
extern "C" void CtrlObjectParam_SetItemBitFlag(u32 idx, u32 value);
extern "C" void CItemBlock_setFlag120EC(u8, u32);

extern "C" void activateLOD__8CTaskLODFv(s8);
extern "C" void deactivateLOD__8CTaskLODFv(u8);
extern "C" int getLODData__8CTaskLODFv(u8);
extern "C" void* getScnHandle__Fv(void);
extern "C" void func_804BCC30(void*, s8);
extern "C" void func_804BCC3C(void*, u8);
extern "C" void forwardMpfCallB__17UnkClass_8047BB54Fv(void*, s8);
extern "C" void forwardMpfCallC__17UnkClass_8047BB54Fv(void*, u8);
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" f32 FrSqrt__Q24nw4r4mathFf(f32);

// standalone BDAT helpers
extern "C" void* getFP__FPCc(const char*);
// getBdatStringColumnValue: canonical decl in plugin/ocBdat.hpp (included above).

// misc single decls
extern "C" void func_80136D74(nw4r::lyt::Layout*, char*, u32);
extern "C" void* getInstance__14Class_80296898Fv();
}

struct Table_80500188;
extern "C" struct Table_80500188 lbl_eu_80500188;
