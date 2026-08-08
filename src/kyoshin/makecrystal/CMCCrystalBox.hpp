#pragma once

#include <types.h>

namespace nw4r { namespace lyt { class AnimTransform; class Layout; } }

// 8-byte crystal box param: two s16 counters (selected id, quantity), a
// spare s16, and two flag bytes. Inlined low-level value type for the
// 32-slot sub-tables at CMCCrystalBox+0x2D8 and the transient sort table.
struct CMCCrystalBoxParam {
    s16 m0;   // +0x00 selected id
    s16 m2;   // +0x02 quantity
    s16 m4;   // +0x04 spare
    u8  m6;   // +0x06 flag
    u8  m7;   // +0x07 flag

    void initCrystalBoxParam_80213E04(s16 a, s16 b);
};

// Entry in the crystal-state table rooted at CMCCrystalBox+0x3D8.
struct CMCCrystalDataEntry {
    s16  id;     // +0x00
    u8   flag;   // +0x02
    u8   pad3;   // +0x03
};

// Crystal-state table rooted at CMCCrystalBox+0x3D8: 1024 entries (4 bytes
// each), then a small state header, then the name buffer for sprintf.
struct CMCCrystalData {
    CMCCrystalDataEntry entries[0x400]; // +0x000
    u16  count;                         // +0x1000
    u8   byte_1002;                     // +0x1002 (category)
    u8   limit;                         // +0x1003
    s8   current;                       // +0x1004
    char name[0x10C8 - 0x1005];         // +0x1005 (buffer passed to sprintf)
};

class CMCCrystalBox {
public:
    CMCCrystalBox(u8 parentType);
    virtual ~CMCCrystalBox();
    void OnFileEvent();

    // Fields - vtable at +0x00 (implicit), user fields start at +0x04
    u8 pad_00[0x1C];                    // +0x04 to +0x1F
    u8 unk20;                           // +0x20: entry count (0 = no entries)
    u8 pad_21[0x24 - 0x21];
    nw4r::lyt::AnimTransform* subObjPtrs[0x0F]; // +0x24 to +0x5F: array of 15 sub-object pointers
    u8 unk60;                           // +0x60
    u8 pad_61_pad_63[3];
    u32 unk64;                          // +0x64: state/status
    u8 unk68;                           // +0x68
    u8 unk69;                           // +0x69: active flag
    u8 pad_6A[0x6C - 0x6A];
    // +0x6C: CCur07
    // +0x84: CCur09
    // +0x9C: CCur18
    // +0xB4: CCur11
    // +0xCC: CCur16
    // +0xE4: CCur18
    // +0xFC: CSortMenu
    // +0x1EC: CMCCrystalInfo
    // +0x250: CScrollBar
    // +0x290: CSysWin
    u8 pad_6C[0x2CC - 0x6C];
    u8 unk2CC;                          // +0x2CC
    u8 unk2CD;                          // +0x2CD
    u8 unk2CE;                          // +0x2CE
    u8 unk2CF;                          // +0x2CF
    u8 unk2D0;                          // +0x2D0: parent type
    u8 unk2D1;                          // +0x2D1
    u8 unk2D2;                          // +0x2D2
    u8 unk2D3;                          // +0x2D3
    u8 unk2D4;                          // +0x2D4: read by getByte_2D4_8021624C
    u8 unk2D5;                          // +0x2D5: read by getByte_2D5_80216254
    u8 unk2D6;                          // +0x2D6: selected-crystal table cursor (written/returned by func_8021625C)
    u8 unk2D7;                          // +0x2D7: current item id byte
    CMCCrystalBoxParam subTable[32];    // +0x2D8 to +0x3D8: selected-crystal table (32 slots)
    CMCCrystalData data;                // +0x3D8: crystal-state table (ends at +0x14A0)
    u8 field_14A0;                      // +0x14A0
    u8 pad_14A1[0x14EC - 0x14A1];       // +0x14A1..0x14EB
    u8 field_14EC[5];                   // +0x14EC..0x14F0 (indexed by field_14F2)
    u8 field_14F1;                      // +0x14F1
    u8 field_14F2;                      // +0x14F2: countdown ticker / index
    u8 pad_14F3[0x1500 - 0x14F3];
    u8 unk1500;                         // +0x1500
    u8 pad_1501[0x1504 - 0x1501];
    u16 unk1504;                        // +0x1504
    u8 field_1506;                      // +0x1506
    u8 field_1507;                      // +0x1507
};

// C-linkage imports
// -----------------
// Retail symbols with C linkage (unmangled names, per retail relocs).
// Previously declared inline in each TU as pseudo-imports; kept here as real
// imports. The names ARE the retail symbol names - keep verbatim.
extern "C" void code80135FDC_setVec3(float*, float, float, float);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void func_801D216C(void*, u8);  // retail symbol is unmangled
extern "C" u32 func_801D32DC(void*);
extern "C" u8 func_8021A8EC(void*);
extern "C" u32 CScrollBar_isVisible(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" void func_8021A984(void*);
extern "C" void func_8021A918(void*);
extern "C" void func_8021A960(void*);
extern "C" void func_801F369C(void*);
extern "C" char lbl_eu_8050888C[];
extern "C" unsigned long lbl_eu_80508870[];
extern "C" void func_80213570(void*, u8);
extern "C" char* func_802138B8(CMCCrystalData*, int);
extern "C" void func_8021A93C(void*);
extern "C" void func_8021A8F4(void*);
extern "C" void func_8021A840(void*, void*);
extern "C" void func_8021A9A8(void*, unsigned short, unsigned char);
extern "C" int func_801D3320(void*);
extern "C" void func_801D31F8(void*, void*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D350C(void*);
extern "C" void func_801D3518(void*, void*);
extern "C" void func_801D353C(void*, unsigned char);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_8022B7C8(void*, void*);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F36BC(void*, unsigned long, unsigned long);
extern "C" void func_801F3850(void*, unsigned long);
extern "C" void func_801F367C(void*);
extern "C" void* CItem_initItemInstances();
extern "C" unsigned long func_8015780C(unsigned long);
extern "C" char* func_80136190(char*, char*, unsigned long);
extern "C" void func_80136B4C(nw4r::lyt::Layout*, char*, char*, unsigned long);
extern "C" void* func_80157C4C(unsigned char, short);
extern "C" int func_80213710(void*, unsigned char);
extern "C" unsigned char func_8021384C(void*, unsigned char);
extern "C" unsigned short func_802137DC(void*, unsigned char);
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__14CMCCrystalInfoFv(void*, int);
extern "C" void __dt__9CSortMenuFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__6CCur16Fv(void*, int);
extern "C" void __dt__6CCur11Fv(void*, int);
extern "C" void __dt__6CCur09Fv(void*, int);
extern "C" void __dt__6CCur07Fv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void __ct__CCur07(void*, int);
extern "C" void __ct__CCur09(void*, int);
extern "C" void __ct__CCur18(void*, int);
extern "C" void __ct__CCur11(void*, int);
extern "C" void __ct__CCur16(void*, int);
extern "C" void __ct__CSortMenu(void*);
extern "C" void __ct__CMCCrystalInfo(void*);
extern "C" void __ct__CScrollBar(void*, int);
extern "C" void __ct__CSysWin(void*, int);
extern "C" void func_8003AA8C__5CBdatFUl(u32);
extern "C" void func_801390E0__FPP11CFileHandle(void*);
extern "C" void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" void func_80139198(u32);
extern "C" void func_801D3258(void*);
extern "C" void func_8021A860(void*);
extern "C" void func_801F35DC(void*);
extern "C" void func_8022B7F4(void*);
extern "C" int  func_801D3328(void*);
extern "C" void func_801D3408(void*);
extern "C" void func_801D3330(void*);
extern "C" void func_801D3430(void*, void*);
extern "C" void func_801D3454(void*, void*);
extern "C" void func_80137924(void*, void*, void*, void*);
extern "C" void* CSysWin_getUnk34(void*);
extern "C" void  CSysWin_isActive(void*);
extern "C" void func_8022B8E4(void*);
extern "C" void func_8022B90C(void*, int);
extern "C" void func_8022B9B4(void*, void*, int);
extern "C" void func_8022BFC8(void*, int);
extern "C" void func_8022B8B8(void*);
extern "C" char* func_8013639C(void*, char*, int);
extern "C" void func_801D3160(void*);
extern "C" void func_8021A780(void*);
extern "C" void func_801D202C(void*);
extern "C" void func_801F3540(void*);
extern "C" void func_8022B748(void*);
extern "C" void func_80213488(void*);
extern "C" u32  func_801C6E90(void*);
extern "C" u16 func_80139358(u32);
extern "C" void func_80218018(CMCCrystalBox* self);
extern "C" void func_8021488C(CMCCrystalBox* self);
extern "C" int func_80216A20(CMCCrystalBox* self);
extern "C" void func_80219094(CMCCrystalBox* self);
extern "C" void func_80215B78(CMCCrystalBox* self);
extern "C" void func_8021852C(CMCCrystalBox* self);
extern "C" void func_8021900C(CMCCrystalBox* self);
extern "C" void func_8021911C(CMCCrystalBox* self);
extern "C" void func_802191A4(CMCCrystalBox* self);
extern "C" void func_8021922C(CMCCrystalBox* self);
extern "C" void func_80219994(CMCCrystalBox* self, int);
extern "C" void func_80213788(void* self);
extern "C" void func_802137B4(void* self);
extern "C" void func_802180B4(CMCCrystalBox* self);
extern "C" void func_80218460(CMCCrystalBox* self);
extern "C" void func_80218B10(CMCCrystalBox* self);
extern "C" void func_8021899C(CMCCrystalBox* self);
