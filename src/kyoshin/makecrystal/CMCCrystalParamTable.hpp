#pragma once

#include <types.h>

// 8-byte crystal-box param: crystal id, quantity, a spare word and two flag
// bytes. Element of the 32-slot tables (CMCCrystalParamTable and the sort
// table in CMCCrystalBox::rebuildCrystalBoxSelTable). Retail emits the inline
// members out of line after their first user (weak-emission order).
class CMCCrystalBoxParam {
public:
    CMCCrystalBoxParam(u16 id = 0, u16 count = 0) {
        m0 = id;
        m2 = count;
        m4 = 0;
        m6 = 0;
        m7 = 0;
    }
    CMCCrystalBoxParam(const CMCCrystalBoxParam& other) {
        m0 = other.m0;
        m2 = other.m2;
        m4 = other.m4;
        m6 = other.m6;
        m7 = other.m7;
    }
    void set(const CMCCrystalBoxParam& other) {
        m0 = other.m0;
        m2 = other.m2;
        m4 = other.m4;
        m6 = other.m6;
        m7 = other.m7;
    }

    u16 m0;   // +0x00 crystal id
    u16 m2;   // +0x02 quantity
    u16 m4;   // +0x04 spare
    u8  m6;   // +0x06 flag
    u8  m7;   // +0x07 flag
};

// Selected-crystal table at CMCCrystalBox+0x2D6 (retail ctor 0x80215BCC,
// dtor 0x80215CA4). The model display keeps a pointer to it (+0xE1C) and
// edits it through clearSlot/refreshCount/sortSlots (CModelDispMakeCrystal.cpp).
class CMCCrystalParamTable {
public:
    CMCCrystalParamTable() {
        mCount = 0;
        mItemId = 0;
        for (u8 i = 0; i < 32; i++) {
            mSlots[i].set(CMCCrystalBoxParam());
        }
    }
    ~CMCCrystalParamTable() {}

    void clearSlot(u16 idx);
    void refreshCount();
    void sortSlots();

    u8 mCount;                      // +0x00 used slots
    u8 mItemId;                     // +0x01 current item id byte
    CMCCrystalBoxParam mSlots[32];  // +0x02
};
