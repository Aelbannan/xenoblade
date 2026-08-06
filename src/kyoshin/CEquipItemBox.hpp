#pragma once

#include <types.h>

// CEquipItemBox vtable (lbl_eu_80538658)
extern "C" void* lbl_eu_80538658[];

namespace nw4r {
namespace lyt {
    class Layout;
    class DrawInfo;
    class AnimTransform;
}
}

/* Layout cursor used by CEquipItemBox (CBaseCur-style), own vtable. */
struct CEIBCur {
    CEIBCur(void* arcResAcc);
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

/* CEIBCur vtable / CEIBPageCur vtable (in this unit's .data). */
extern "C" void* lbl_eu_80538704[];
extern "C" void* lbl_eu_805386EC[];

/* Layout draw w/ visible flag. Retail symbol IS the mangled name; declare
   with C++ linkage so MWCC mangles func_80137038 to the retail symbol. */
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

/* CEIBPageCur - page-cursor subclass of CEIBCur, overrides vtable. */
struct CEIBPageCur : CEIBCur {
    CEIBPageCur(void* arcResAcc);
};

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

/* CEquipItemBox - equip item box UI widget.
   Layout reconstructed from __ct__CEquipItemBox. */
struct CEquipItemBox {
    CEquipItemBox();
    ~CEquipItemBox();
    void OnFileEvent();

    u8 func_802865A0();
    u8 func_802865A8();
    int func_80286650();
    int func_802866A0();
    void func_802866E8();
    u8 func_80286698();
    u8 func_80287EE8();
    void func_80287EFC(u32 val);
    void func_8028A07C();
    void func_8028A0C0(u8 val);
    void func_80288A1C();
    void func_80288A6C();
    void func_80288AC0();
    void func_80282DF8();
    void func_80282E24();
    void func_80287D58();
    u8 func_80283280(u32 param);
    u8 func_802832B4();

    void* mVtbl;            // 0x00

    // +0x04: UnkClass_8045F564 mMemRegion1 (0x10 bytes)
    // +0x14: UnkClass_8045F564 mMemRegion2 (0x10 bytes)
    // +0x24: u32[7]
    u8 _pad04[0x30];
    void* field_34;         // 0x34
    void* field_38;         // 0x38
    void* field_3C;         // 0x3C

    u8 unk_40;              // 0x40
    u8 unk_41;              // 0x41
    u8 unk_42;              // 0x42
    u8 unk_43;              // 0x43

    // +0x44: CEIBCur   (0x18 bytes)
    // +0x5C: CEIBPageCur (0x18 bytes)
    // +0x74: CCur18     (0x18 bytes)
    u8 _pad44[0x48];

    // +0x8C: CSortMenu (0xF0 bytes)
    u8 _padSortMenu[0xF0];

    // +0x17C: CSysWin (0x3C bytes)
    u8 _padSysWin1[0x3C];

    // +0x1B8: CSysWin (0x3C bytes)
    u8 _padSysWin2[0x3C];

    u8 unk_1f4;             // 0x1F4
    u8 unk_1f5;             // 0x1F5
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
    u8 field_2026[0x400];   // 0x2026
};

/* Copy 8-byte CEquipItemData struct from src to dst */
void copyEquipItemData(CEquipItemData* dst, const CEquipItemData* src);
