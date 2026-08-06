// Auto-scaffolded catalog TU for kyoshin/menu/CMenuArtsSet
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuArtsSet.hpp"
#include <cstddef>
void __ct__CMenuArtsSet(){}

extern "C" unsigned long func_8022F530() {
    extern unsigned long lbl_eu_80664740;
    return lbl_eu_80664740 != 0;
}

void func_8022F544(){}

void cbRenderBefore__12CMenuArtsSetFv(void* self);
void func_8022FA48(void* self) {
    // Thunk: adjust this down from sub-object at offset 0x58
    CMenuArtsSet* obj = (CMenuArtsSet*)((char*)self - offsetof(CMenuArtsSet, mSubObj58_start));
    cbRenderBefore__12CMenuArtsSetFv(obj);
}

void __dt__12CMenuArtsSetFv(void* self);
void func_8022FA50(void* self) {
    // Thunk: adjust this down from sub-object at offset 0x58
    CMenuArtsSet* obj = (CMenuArtsSet*)((char*)self - offsetof(CMenuArtsSet, mSubObj58_start));
    __dt__12CMenuArtsSetFv(obj);
}

void __ct__8022FA58(SArtsSub8022FA58* self, u32 a, u32 b) {
    self->field_0x00 = a;
    self->field_0x04 = b;
    self->field_0x08 = 0;
    self->field_0x0C = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x15 = 0;
    self->field_0x16 = 1;
    self->field_0x17 = 0;
    self->field_0x18 = 0;
    self->field_0x21 = 0;
}

extern "C" void* __dt__8022FA90(void* self, int flags) {
    if (self && flags > 0) {
        ::operator delete(self);
    }
    return self;
}

void func_8022FAD0(){}

void func_8022FD9C(){}

void func_8022FDF4(){}

extern "C" void func_80230160(SArtsSub8022FA58* self);
extern "C" void func_802316F8(SArtsSub8022FA58* self);
extern "C" void func_80124270(void* p);

void func_8022FE58(SArtsSub8022FA58* self) {
    self->field_0x16 = 0;
    self->field_0x17 = 1;
    u8 next = self->field_0x14 + 1;
    self->field_0x14 = next;
    if ((s8)next >= self->field_0x21) {
        self->field_0x14 = 0;
    }
    return func_80230160(self);
}

void func_8022FE90(SArtsSub8022FA58* self) {
    self->field_0x16 = 0;
    self->field_0x17 = 0;
    int v = (int)self->field_0x14 - 1;
    self->field_0x14 = (u8)v;
    if ((s8)v < 0) {
        self->field_0x14 = (u8)(self->field_0x21 - 1);
    }
    return func_80230160(self);
}

void func_8022FEC4(){}

void func_8022FF00(){}

void func_8022FF74(){}

void func_80230070(){}



void func_80230374(){}

u8 func_8023040C(SArtsSub8022FA58* self, u32 idx) {
    if (idx < self->field_0x21) {
        return self->field_0x19[idx];
    } else {
        return 0;
    }
}

void __ct__8023042C(){}

extern "C" void* __dt__80230484(void* self, int flags) {
    if (self && flags > 0) {
        ::operator delete(self);
    }
    return self;
}

void func_802304C4(){}

void func_8023066C(){}

void func_802306F0(){}

void func_802307A4(){}

void func_8023080C(SArtsSub8022FA58* self, u8 val) {
    if (self->field_0x27 != 0) {
        func_80124270(((SArts080C*)(self->field_0x08))->field_0x10);
    } else {
        self->field_0x22 = val;
        return func_802316F8(self);
    }
}

void func_8023082C(SArtsSub8022FA58* self, int a) {
    self->field_0x20 = (s8)(self->field_0x20 + 1);
    if ((s8)self->field_0x20 >= 9) {
        self->field_0x20 = 0;
    }
    if (a != 0 && self->field_0x20 == 4) {
        self->field_0x20 = 5;
    }
    return func_802316F8(self);
}

void func_80230870(SArtsSub8022FA58* self, int a) {
    self->field_0x20 = (s8)(self->field_0x20 - 1);
    if ((s8)self->field_0x20 < 0) {
        self->field_0x20 = 8;
    }
    if (a != 0 && self->field_0x20 == 4) {
        self->field_0x20 = 3;
    }
    return func_802316F8(self);
}

void func_802308B0(){}

void func_80230D18(){}

void func_80230D74(){}

void func_80230FF0(SArtsSubDElem* self, u8 val, u32 idx, u32 sub, u32 off) {
    SArtsSubDElem* e = &self[idx];
    e->data[off + sub * 8] = val;
    e->field_0x30 = 1;
}

void func_80231014(){}

void func_80231220(){}

void func_80231320(void* self){}

void func_802313E0(){}

void func_80231464(SArtsSub8022FA58* self) {
    if (self->field_0x20 != 4) {
        return;
    }
    self->field_0x20 = 3;
    return func_802316F8(self);
}

void func_80231480(){}

void func_8023150C(){}

void func_802315BC(){}

void func_80231648(){}

void func_80231848(CMenuArtsSet* self, const SArtsVec2* src) {
    self->mField44 = src->x;
    self->mField48 = src->y;
}

void func_8023185C(){}

void __ct__802319AC(){}

extern "C" void* __dt__80231A08(void* self, int flags) {
    if (self && flags > 0) {
        ::operator delete(self);
    }
    return self;
}

void func_80231A48(){}

void func_80231C30(){}

void func_80231CB4(){}

void func_80231D68(){}

void func_80231DD0(){}

void func_80231E8C(){}

void func_80231F60(){}

void func_80232000(){}

void func_802320C0(){}

u8 func_802322BC(SArts322BC* self) {
    if (self->field_0x12C != 0) {
        s32 off = (self->field_0x21 + self->field_0x20) << 4;
        return self->mTable[off];
    }
    return 0;
}

void func_802322F4(){}

void func_80232370(){}

void func_802324C4(){}

void func_80232638(){}

u8 func_8023270C(SArts3270C* self) {
    u8 r = 0;
    if (self->field_0x20 == 0 && self->field_0x21 == 0) {
        r = 1;
    }
    return r;
}

void func_80232B88(SArts327B0* self);
void func_80232C78(SArts327B0* self);

void func_80232734(){}

void func_802327B0(SArts327B0* self) {
    extern void func_801F3850(void*, u32);
    self->field_0x20 = 0;
    self->field_0x21 = 0;
    func_801F3850(self->field_0x28, 0);
    func_80232B88(self);
    func_80232C78(self);
}

void func_80232800(){}

void func_80232888(){}

void func_80232910(){}

void func_8023299C(){}

void func_80232A4C(){}

void func_80232AD8(){}

void func_80232B88(SArts327B0* self) { func_80124270(self); }

void func_80232C78(SArts327B0* self) { func_80124270(self); }

void __ct__CArtsList(){}

CArtsList::~CArtsList() {}

void func_8023352C(){}

void func_8023359C(){}

void func_80233674(){}

void func_80233760(){}

void func_8023380C(){}

u8 CMenuArtsSet::func_80233880() { return mField31; }

void func_80233888(){}

void func_8023390C(){}

void func_80235F50(void* self);
void CMenuArtsSet::func_80233968() { func_80235F50(mSubObj74); }

void func_80233970(){}

void func_802339D4(){}

void func_80233A50(){}

void func_80233C08(){}

void func_80233DC0(){}

void func_80233E9C(){}

void func_80233F78(){}

void func_802340C4(){}

void func_802346BC(){}

void func_80234780(){}

void func_80234844(){}

void func_80234928(){}

void CMenuArtsSet::func_802349F8(u8 val) { mField139 = val; }

void CMenuArtsSet::func_80234A00() { func_80231320(mSubObj148); }

void func_80234A08(){}

void func_80234A94(){}

u8 func_80234C84(SArts34C84* self) {
    if (self->field_0x16F != 0) {
        return 1;
    }
    return self->field_0x2A1;
}

void func_80234CA0(){}

void func_80234D14(){}

void func_80234D68(){}

void func_80234EB8(){}

void func_80234F7C(){}

void func_80234FDC(){}

// func_80137510 is a flat C symbol (anim-frame check helper).
extern "C" u32 func_80137510(void*, float);

void func_8023506C(SArts3506C* self) {
    extern float lbl_eu_80668648;
    if (func_80137510(self->mAnim20, lbl_eu_80668648) != 0) {
        self->field_0x31 = 1;
        self->field_0x2C = 0;
    }
}

void func_802350B8(){}

void func_80235108(SArts35108* self) {
    if (self->field_0x1C != 0) {
        self->field_0x30 = 1;
        self->field_0x28 = 1;
    }
}

void func_80235124(){}

void CArtsList::OnFileEvent() {}
