#pragma once

#include <types.h>

struct CEventFile;

// File slot: 12 bytes per entry
struct FileSlot {
    u32 handle;
    u32 data;
    u8 flag;
    u8 _pad[3];
};

class CModelDispEquip {
public:
    CModelDispEquip();
    virtual ~CModelDispEquip();
    void OnFileEvent();

    void resetBase();
    u8 getState20();
    u8 getState21();
    void setState14();
    void setState14_3();
    static void storeFloats(float* dest, float a, float b, float c, float d);

    void func_80200F9C();
    void func_80201298();
    void func_8020131C();
    void func_80201444();

    virtual void vfunc18();
    virtual void vfunc40();

    void thunk4_1298();
    void thunk4_1440();
    void thunk4_131C();
    void thunk4_1318();
    void thunk4_F9C();
    void thunk4_dtor();
    void thunk8_1444();
    void thunk8_dtor();

    // ---- Layout ----
    // The retail has 3 vtable pointers (multiple inheritance).
    // MWCC generates only 1, so we add 2 dummy words.
    u32 _vtable2; // 0x04 - second vtable
    u32 _vtable3; // 0x08 - third vtable
    u32 somePtr;  // 0x0C
    u8 _pad010[0x0C]; // 0x10-0x1B (3 sub-object ptrs)
    u8 actParamArea[0x53C]; // 0x1C-0x557
    s32 timer;    // 0x558
    u32 unk_55C;  // 0x55C
    u8 actParamArray[0xA78]; // 0x560-0xFD7
    u32 animModelPtrs[2]; // 0xFD8
    u32 equipPtrs[8]; // 0xFE0
    u32 currentModelPtr; // 0x1000
    u32 animPtrs[2]; // 0x1004
    u32 weaponId;  // 0x100C
    s32 equipSlot; // 0x1010
    u8 state;      // 0x1014
    u8 _pad1015[3];
    f32 alpha;     // 0x1018
    f32 alphaTimer; // 0x101C
    u8 state20;    // 0x1020
    u8 state21;    // 0x1021
    u8 _pad1022[2];
    FileSlot fileSlots[9]; // 0x1024-0x108F
    u32 modelFileHandle; // 0x1090
    u32 modelData;  // 0x1094
    f32 colorR;     // 0x1098
    f32 colorG;     // 0x109C
    f32 colorB;     // 0x10A0
    f32 colorA;     // 0x10A4
    f32 scale1[3];  // 0x10A8
    f32 scale2[3];  // 0x10B4
};