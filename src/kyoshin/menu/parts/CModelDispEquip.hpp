#pragma once

#include <types.h>

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

    // TODO: add fields
    u8 _pad1014[0x1014];
    u8 field_1014; // 0x1014
    u8 _pad1015[0x0B];
    u8 field_1020; // 0x1020
    u8 field_1021; // 0x1021
};

