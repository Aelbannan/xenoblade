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
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (ml::epsilon)
#include "kyoshin/cf/object/CfObject.hpp"
#include <nw4r/lyt/lyt_material.h>

class CScn;

// All definitions in code_80135FDC.cpp are C-linkage (extern "C"); their
// single canonical declarations live in the lean headers below.
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"


// Frame object returned by Scn_HasCamItem as consumed by drawLayout: the
// view matrix lives at +0xCC (3x4) and the projection matrix at +0x194.
// Struct assignment of the matrix member makes MWCC emit the retail lwz/stw
// pair copy inline (a u32 loop would degrade to sequential loads/stores).

// LayoutSetTextBoxText/SetLayoutSelfText dispatch through real classes (no local pads):
// - tagProc slot 0x14 is CTagProcessorBase::Proc (kyoshin/CTagProcessor.hpp,
//   retail TagProcPreprocessMessage); the TU-local decl lives in code_80135FDC.cpp
//   (CTagProcessor.hpp cannot be included here: its byte-wise ut::Color
//   clashes with the lyt headers above).
// - the slot-0x7C string setter is nw4r::lyt::TextBox::SetString.

struct CViewFrame37038 {
    /* 0x000 */ u8 pad00[0xCC];
    /* 0x0CC */ nw4r::math::MTX34 mtx;
    /* 0x0FC */ u8 padFC[0x98];
    /* 0x194 */ f32 projection[4][4];
};

// BDAT filename pointer tables copied to the stack by RevealMinimapArea. Retail
// copies them with inline mtctr/lwzu/stwu 8-byte loops; struct assignment is
// the only source shape that makes MWCC emit those loops (memcpy would emit
// bl memcpy).
struct XBMapTable1 { u32 w[35]; };   // 140 B -> 17x8 loop + tail word
struct XBMapTable2 { u32 w[31]; };   // 124 B -> 15x8 loop + tail word
struct XBMapTable3 { u32 w[34]; };   // 136 B -> 17x8 loop, no tail

// Pane colour / material dispatches (PaneSetVtxColorByName and siblings) go through
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
extern void deallocate__Q23mtl10MemManagerFPv(void*);
extern void Set__Q34nw4r3lyt12AnimResourceFPCv(void*, const void*);
extern void PSVECNormalize(const Vec*, Vec*);
extern void* getCurrentView__5CViewFv();
extern void* Scn_HasCamItem(void*, void*);
extern void SetFont__Q34nw4r3lyt7TextBoxFPCQ34nw4r2ut4Font(nw4r::lyt::TextBox*, const nw4r::ut::Font*);

extern u8 sNw4rMaterialFile[];
extern u8 sNw4rTexMapAssertMsg[];
extern u32 sThresholdLookup[28];
extern char gTplNameBuffer[];
extern char gIconNameBuffer[];

// globals (Block C)
extern u8 gSysMenuActive;
extern u8 gMenuStateMain;
extern u8 gMenuNotifyFlag;
extern u8 gMenuStateSecondary;
extern u8 gMenuStateReady;
extern u32 gMenuParamA;
extern u32 gMenuParamB;
extern u8 gMenuBlockedFlag;
extern u32 gSysFileListBdat;
extern u32 gSysIconListBdat;
// 7-byte indexed buffer (GetCollectedFlagByte indexes &lbl[idx]); was scalar u8.
extern u8 gCollectedFlags[7];
extern u8 gCollectedFlagCount;
extern u16 gSlotItemIds[3];
extern u8 gSlotItemCount;
extern u8 gMenuSubStateFlag;
extern u8 gMenuDepth[8];
// Defined as pointer globals in cf/CfBdat.cpp; keep the shared void* spelling
// so callers that also include the owning bdat/item headers do not redeclare them.
extern void* lbl_eu_806640F0;
extern u32 lbl_eu_80664184;
#ifndef CODE_80135FDC_CPP
#endif
extern u32 gSjisTablePtr;
extern const char lbl_eu_80500664[];

// Face pane names (defined in code_80135FDC.cpp .rodata; fwd here so the
// .sdata2 pointer table can precede them in TU layout experiments).
extern const char sFacePaneN01[0x0C];
extern const char sFacePaneM01[0x0C];
extern const char sFacePaneN02[0x0C];
extern const char sFacePaneM02[0x0C];

// string/rodata data (Block B)
extern u8 lbl_eu_8052CB40[];
extern u8 lbl_eu_8052CB1C[];
extern u8 sNw4rLinkListFile[];
extern u8 sNw4rNullPointerAssertMsg[];
extern u8 sNw4rTevColorAssertFile[];
extern u8 sNw4rTevColorAssertMsg[];
extern const char lbl_eu_80526324[]; // const: matches CActParamAnimGame.cpp definition + CfSoundMan.hpp chain
extern const char lbl_eu_80526300[];  // const char: matches CfSoundMan.hpp chain (u8/char variants broke CMiniMap TU)
extern const char lbl_eu_805262F0[];
extern const char lbl_eu_805262C8[];
extern u8 sSjisWordTable[];
extern u8 sLodCharMap[];
// Minimap pointer tables: heterogeneous (strings + NULLs); entries are relocs.
extern const void* sMinimapPtrTablePlain[32];
extern const void* sMinimapPtrTable34[34];
// 13-entry float table copied wholesale to the stack by GetFloatTableEntry.
struct FloatTable13 { f32 w[13]; };
extern FloatTable13 sRankFloatTable;
extern char gItemNameBuffer[];
extern ml::CVec3 zero__Q22ml5CVec3;

// helpers
// (Bdat_GetMaxRow_B1EC / CtrlObjectParam_GetSlotTableBase / CtrlRemote_SetSharedBit stay inline in
//  code_80135FDC.cpp - functions.hpp / CChainActorPc.hpp / CKizunaTalkList.hpp
//  declare them with different signatures and are co-included by callers)
extern "C" void* CtrlObjectParam_GetCharEntry(u32 idx);
extern "C" int CtrlObjectParam_GetItemBitFlag(u32 idx);

extern "C" void CItemBlock_setFlag120EC(u8, u32);

extern "C" void activateLOD__8CTaskLODFv(s8);
extern "C" void deactivateLOD__8CTaskLODFv(u8);
extern "C" int getLODData__8CTaskLODFv(u8);
extern "C" void ScnData_FwdB7D9C(void*, s8);
extern "C" void ScnData_FwdB7DD4(void*, u8);
extern "C" void forwardMpfCallB__17UnkClass_8047BB54Fv(void*, s8);
extern "C" void forwardMpfCallC__17UnkClass_8047BB54Fv(void*, u8);
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);

// standalone BDAT helpers
// getBdatStringColumnValue: canonical decl in plugin/ocBdat.hpp (included above).

// misc single decls
extern "C" void* getInstance__14Class_80296898Fv();
}

struct Table_80500188;
extern "C" struct Table_80500188 sThresholdPairTable;
