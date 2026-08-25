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

#include "monolib/math/CMat34.hpp"

class CScn;

// All definitions in code_80135FDC.cpp are C-linkage (extern "C"), so the
// declarations MUST match -- otherwise callers emit Itanium-mangled relocs
// (e.g. func_8013639C__FPvPcUs) while retail has the unmangled name, and the
// witness rejects the pair at the reloc gate (us-8025658c / us-8025650c were
// stuck at 99.4-99.6% on exactly this).
// Params use const void* so both const char* (CArtsInfo, CExchangeWin) and
// const void* (CCollepedia) call sites bind without conversions.
extern "C" {
u16 func_8013606C(const void*, const void*, u32);
u8 func_8013600C(const void*, const void*, u32);
s16 func_80136130(const void*, const void*, u32);
s8 func_801360CC(const void*, const void*, u32);
#ifndef CODE_80135FDC_CPP
// The definitions of these in code_80135FDC.cpp use different (not-yet-)
// matched signatures/linkage; code_80135FDC.cpp defines CODE_80135FDC_CPP
// before including so these caller-tuned exports don't clash with them.
extern "C" char* func_80136190(const void*, const void*, int);
u8 func_801361E8(u32, const char*, u32);
char* func_8013639C(const void*, const void*, int);
void func_8013676C(void*, u32);
#endif
void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);
void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
// Retail symbols for these ARE the Itanium-mangled names; declare them under
// C linkage so calls bind to the literal mangled identifier (declaring them
// as C++ functions would re-derive the same mangling, but the extern "C" +
// mangled-identifier form is what the definitions in code_80135FDC.cpp use).
void func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(nw4r::lyt::Layout*, char*, u32);
void func_80136910__FPQ34nw4r3lyt6LayoutPcUc(nw4r::lyt::Layout*, char*, u8);
void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
}

// Retail symbols that ARE the mangled C++ names stay C++-linkage. Declare the
// UNMANGLED identifier so MWCC mangles func_80137038 to the retail symbol
// (func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii); declaring the
// pre-mangled name here would double-mangle it (see CEquipItemBox.hpp).
// The extern "C" + mangled-identifier declarations above serve the pre-mangled
// call sites (CMenuUpdate, CMCCrystalSupport, CMCEffStart, CPcKizunagram); the
// unmangled forms below serve the (majority) unmangled call sites. Both bind
// to the same retail symbols.
void func_801368C0(nw4r::lyt::Layout*, char*, u32);
void func_80136910(nw4r::lyt::Layout*, char*, u8);
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_80137250(nw4r::lyt::DrawInfo* pDrawInfo);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);

// Retail symbols are unmangled (C linkage) -- keep them extern "C" so calls
// bind to the unmangled name, not an Itanium-mangled re-derivation.
extern "C" {
#ifndef CODE_80135FDC_CPP
u8 func_801372B4(u32);
#endif
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
#ifndef CODE_80135FDC_CPP
void func_80137E7C(void*, void*, void*);
char* func_80138F78(u32);
void func_80139A18(void*, void*, void*, void*);
#endif
}
extern "C" int func_8013BE50();
extern "C" u8 code80135FDC_getByte_64077();

// Frame object returned by func_8049626C as consumed by func_80137038: the
// view matrix lives at +0xCC (3x4) and the projection matrix at +0x194.
// Struct assignment of the matrix member makes MWCC emit the retail lwz/stw
// pair copy inline (a u32 loop would degrade to sequential loads/stores).
// Tag processor used by func_80136A1C/func_80136D74: vtable slot 0x14
// rewrites a UTF-16 string in place.
class CTagProcIf36D74 {
public:
    virtual void _v04();
    virtual void _v08();
    virtual void _v10();
    virtual const wchar_t* Proc(wchar_t* text, int param, float a, float b);  // 0x14
};

// String-setter interface called by func_80136D74: vtable slot 0x7C sets a
// UTF-16 string entry.
class CLytSetStrIf36D74 {
public:
    virtual void _v004();
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v078();
    virtual void SetString(u16* text, int index);  // 0x7C
};

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
extern "C" u8 func_801392B4(u32);
#endif

// Interface whose vtable slot 0x28 is the Set(idx, value*) dispatcher called
// by func_80137C1C (and siblings); ten placeholder virtuals put Set at 0x28.
class CAnimTargetIf37038 {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void Set(u32 idx, void* value);
};

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
extern void* func_8049626C(void*, void*);
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
extern u8 lbl_eu_80664070;
extern u8 lbl_eu_80664077;
extern u16 lbl_eu_80664078[3];
extern u8 lbl_eu_8066407E;
extern u8 lbl_eu_8066407F;
extern u8 lbl_eu_80664080;
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
extern f32 lbl_eu_806672F0;
extern f64 lbl_eu_806672F8;
extern f32 lbl_eu_80667300;
extern f32 lbl_eu_80667304;
extern f32 lbl_eu_80667308;
extern f32 lbl_eu_8066730C;
extern f32 lbl_eu_80667310;
extern f32 lbl_eu_80667314;
extern f32 lbl_eu_80667318;
extern f32 lbl_eu_8066731C;
extern f32 lbl_eu_80667320;
extern f32 lbl_eu_80667324;
extern f32 lbl_eu_80667328;
extern f32 lbl_eu_8066732C;
extern f32 lbl_eu_80667330;
extern f32 lbl_eu_80667334;
extern f32 lbl_eu_80667338;
extern f32 lbl_eu_8066733C;
extern f32 lbl_eu_80667340;
extern f32 lbl_eu_80667344;
extern f32 lbl_eu_80667348;
extern f64 lbl_eu_80667350;
extern f64 lbl_eu_80667358;
extern f64 lbl_eu_80667360;
extern u32 lbl_eu_8066A208;
extern const float lbl_eu_8066A20C;
extern u32 lbl_eu_806621F4;
extern CScn* lbl_eu_80663E14;
extern u32 lbl_eu_80663E24;
extern char lbl_eu_80500664[];

// string/rodata data (Block B)
extern u8 lbl_eu_8052CB40[];
extern u8 lbl_eu_8052CB1C[];
extern u8 lbl_eu_8052E4E4[];
extern u8 lbl_eu_8052E4C0[];
extern u8 lbl_eu_8052E558[];
extern u8 lbl_eu_8052E530[];
extern u8 lbl_eu_80526324[];
extern u8 lbl_eu_80526300[];
extern u8 lbl_eu_805262F0[];
extern u8 lbl_eu_805262C8[];
extern u8 lbl_eu_80500108[];
extern u8 lbl_eu_80500230[];
extern u8 lbl_eu_80500480[];
extern u8 lbl_eu_805005A8[];
// 13-entry float table copied wholesale to the stack by func_8013B380.
struct FloatTable13 { f32 w[13]; };
extern FloatTable13 lbl_eu_80500630;
extern char lbl_eu_80573C30[];
extern nw4r::math::VEC3 zero__Q22ml5CVec3;

// helpers
// (func_8003B1EC / func_8009ECB0 / func_8009D018 stay inline in
//  code_80135FDC.cpp - functions.hpp / CChainActorPc.hpp / CKizunaTalkList.hpp
//  declare them with different signatures and are co-included by callers)
extern "C" void* func_8009EC9C(u32 idx);
extern "C" int func_8009EBE8(u32 idx);
extern "C" void func_8009EB94(u32 idx, u32 value);
extern "C" void func_80157824(u8, u32);

extern "C" void func_80462D04__8CTaskLODFv(s8);
extern "C" void func_80462D5C__8CTaskLODFv(u8);
extern "C" int func_80462E1C__8CTaskLODFv(u8);
extern "C" void* func_804BC9EC__Fv(void);
extern "C" void func_804BCC30(void*, s8);
extern "C" void func_804BCC3C(void*, u8);
extern "C" void func_8047BD8C__17UnkClass_8047BB54Fv(void*, s8);
extern "C" void func_8047BD94__17UnkClass_8047BB54Fv(void*, u8);
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
