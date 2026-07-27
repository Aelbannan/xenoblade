#pragma once

#include <types.h>

// Full object layout for CModelDispMakeCrystal (used by C-linkage accessors)
struct CModelDispMakeCrystalFull {
    u8 _00[0xBE8];
    u8 field_BE8;
    u8 _BE9[0x2DC3 - 0xBE9];
    u8 field_2DC3;
    u8 _2DC4[0x2DD2 - 0x2DC4];
    u8 field_2DD2;
};

class CModelDispMakeCrystal {
public:
    CModelDispMakeCrystal();
    virtual ~CModelDispMakeCrystal();
    void OnFileEvent();

    void initCrystalSubStruct();
    u8 getCrystalStateA();
    u8 getCrystalStateB();
    u8 getCrystalStateC();
    void setCrystalPosEntry(unsigned short index, short a, short b);
    void setCrystalCursor(u32 val);
    void destroyCrystalDispThunk4();
    void releaseCrystalDispThunk();
    void destroyCrystalDispThunk8();

    // TODO: add fields
};

