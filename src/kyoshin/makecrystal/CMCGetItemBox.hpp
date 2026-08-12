#pragma once

#include <types.h>
#include <cstring>
#include "kyoshin/CItemBoxInfo.hpp"
#include "monolib/util/FixStr.hpp"

class CFileHandle;

namespace nw4r { namespace lyt {
class AnimTransform;
class ArcResourceAccessor;
class Layout;
class Pane;
} }

/* 52-byte entry referenced by the item-box index table.
   Only the first word is known (read by func_80296D54). */
struct CMCItemBoxEntry {
    u32 field_00;                   // 0x00
    u8 bytes[0x34 - 0x04];          // 0x04-0x33
};

/* Small sub-object passed to the C-linkage menu helpers
   (func_801D216C). 0x18 bytes; stores a vtable pointer at +0x00. */
struct CMCItemBoxSubObj {
    u8 bytes[0x18];
};

/* Echo/placeholder for the CSysWin sub-object range. */
struct CMCGetItemBoxSysWin {
    u8 bytes[0x3C];                 // 0x00-0x3B
};

/* Item-box index structure at +0x314 of CMCGetItemBox.
   An s16 offset table, count, counter and backing entry array.
   Note: region +0x108..+0x14C of this object also doubles as a FixStr<64>
   name buffer in retail (accessed via object offset 0x41C). */
struct CMCItemBoxSub {
    s16 table[0x80];                // 0x000-0x0FF: offset table
    u16 count;                      // 0x100: number of entries
    u8  pad_102;                    // 0x102 (also read as an index source)
    u8  limit;                      // 0x103
    u8  counter;                    // 0x104
    u8  pad_105[0x108 - 0x105];     // 0x105-0x107
    u8  field_108;                  // 0x108 (FixStr<64> byte 0)
    u8  pad_109[0x148 - 0x109];     // 0x109-0x147 (FixStr<64> body)
    u32 field_148;                  // 0x148 (FixStr<64> mLength)
    u8  field_14C;                  // 0x14C
    u8  pad_14D[0x1CC - 0x14D];     // 0x14D-0x1CB
    u32 field_1CC;                  // 0x1CC
    CMCItemBoxEntry* listBase;      // 0x1D0: 52-byte Entry array base
    u8  field_1D4;                  // 0x1D4
    u8  pad_1D5[0x1D8 - 0x1D5];     // 0x1D5-0x1D7
};

class CMCGetItemBox {
public:
    CMCGetItemBox();
    virtual ~CMCGetItemBox();
    void OnFileEvent();

    u8 func_80297D1C();
    u8 func_80297D24();

    // +0x00 vtable
    UnkClass_8045F564 memRegion1;    // 0x04-0x13
    UnkClass_8045F564 memRegion2;    // 0x14-0x23
    CFileHandle* fileHandle1;               // 0x24
    CFileHandle* fileHandle2;               // 0x28
    CFileHandle* fileHandle3;               // 0x2C
    CFileHandle* fileHandle4;               // 0x30
    u8* memManagerPtr;             // 0x34 (deallocated via mtl::MemManager)
    nw4r::lyt::ArcResourceAccessor* arcAcc1;                   // 0x38 (nw4r::lyt::ArcResourceAccessor)
    nw4r::lyt::ArcResourceAccessor* arcAcc2;                   // 0x3C
    nw4r::lyt::Layout* layout40;     // 0x40 (root pane at +0x10)
    nw4r::lyt::AnimTransform* animTrans1;                // 0x44 (nw4r::lyt::AnimTransform)
    nw4r::lyt::AnimTransform* animTrans2;                // 0x48
    u8 field_4C;                     // 0x4C
    u8 field_4D;                     // 0x4D
    u8 _pad_4E[0x50 - 0x4E];         // 0x4E-0x4F
    u8* objAt50;                   // 0x50 (deallocated; has vtable)
    u8 field_54;                     // 0x54
    u8 mField55;                     // 0x55
    u8 _pad_56[0x58 - 0x56];         // 0x56-0x57
    CMCItemBoxSubObj subObj_58;      // 0x58-0x6F (CCur07)
    CMCItemBoxSubObj subObj_70;      // 0x70-0x87 (CCur09)
    CMCItemBoxSubObj subObj_88;      // 0x88-0x9F (CCur16)
    CMCItemBoxSubObj subObj_A0;      // 0xA0-0xB7 (CCur18)
    CMCGetItemBoxSysWin sysWin_B8;   // 0xB8-0xF3
    u8 itemBox[0x1A4 - 0xF4];        // 0xF4-0x1A3 (a CItemBoxInfo, 0xB0 bytes)
    u8 arr_1A4[0x300 - 0x1A4];       // 0x1A4-0x2FF (cursor index table)
    u8 field_300;                    // 0x300
    u8 field_301;                    // 0x301 (also sort/index)
    u8 field_302;                    // 0x302
    u8 mField303;                    // 0x303
    u8 field_304;                    // 0x304 (row)
    u8 field_305;                    // 0x305 (column)
    u8 _pad_306[0x308 - 0x306];      // 0x306-0x307
    u32 field_308;                   // 0x308
    u32 field_30C;                   // 0x30C
    u32 field_310;                   // 0x310
    CMCItemBoxSub sub_314;           // 0x314
};


// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" void* CItem_initItemImplInstances(void*);
extern "C" char* func_80138F78(u32);
extern "C" int   func_801C6E90(void*);
extern "C" u32   func_80136254(void*, const char*, u16);
extern "C" void func_801599D4(CMCItemBoxEntry*, int);
extern "C" u32 CSysWin_isReady(CMCGetItemBoxSysWin*);
extern "C" u32 func_80157CD0(u8);
extern "C" void advanceItemBoxState__FP12CItemBoxInfo(CItemBoxInfo*);
extern "C" u16 ArrayGet12(const unsigned short*, unsigned char);
extern "C" void func_801CB9D8(u32*, void*, u32);
extern "C" void func_80137924(void*, void*, void*, void*);
extern "C" char* func_80136190(const void*, const void*, int);
extern "C" void func_80124270(void*, u32);
extern "C" void func_801D4174(void*);
extern "C" void func_801D4260(void*, u16);
extern "C" void func_8022B7F4(void*);
extern "C" void* func_801D3C74(void*, u32);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
// Retail mangled names kept as source names so the reloc matches exactly.
extern "C" void func_8003AA8C__5CBdatFUl(u32);
extern "C" void func_801390E0__FPP11CFileHandle(void*);
extern "C" void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(nw4r::lyt::ArcResourceAccessor*);
extern "C" char lbl_eu_8050FF8C[];
extern "C" void* lbl_eu_806640EC;  // declared u32 in CCollepedia.hpp, void* in CItemBoxLine.hpp -> kept void*
extern "C" void* lbl_eu_806640D8;  // declared void* in CMCEffStart.hpp/CItemBoxLine.hpp -> kept void*
extern "C" void* lbl_eu_80664100;  // unused address anchor - kept void*
extern "C" void* lbl_eu_80664A18;  // unused address anchor - kept void*
extern "C" void* lbl_eu_80664A1C;  // unused address anchor - kept void*
extern "C" void* lbl_eu_80664A20;  // unused address anchor - kept void*
extern "C" const float lbl_eu_80668BF0;
extern "C" void func_802999B0(CMCGetItemBox*);
extern "C" void func_80298AC8(CMCGetItemBox*, u32, CMCItemBoxEntry*, u8);
extern "C" void func_80298FB4(CMCGetItemBox*, u32, CMCItemBoxEntry*, u8);
extern "C" void __dl__FPv(void*);
extern "C" CMCItemBoxEntry* func_80296DB0(CMCItemBoxSub*, u32);