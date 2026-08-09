// Auto-scaffolded catalog TU for kyoshin/menu/CMenuArtsSet
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuArtsSet.hpp"
#include <cstddef>
#include <nw4r/lyt.h>

// C++-linkage draw helper (mangles to the retail symbol).
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// Arts archive path string (accessed via sda21 small-data relocation).
extern char lbl_eu_8050AC70[];

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

// Switch-case handlers used by func_8022FDF4 (same TU, defined below).
void func_8022FF74();
void func_80230070();

void func_8022FD9C(SArts2FDF4* self) {
    if (self->field_0x8) {
        self->field_0x8->v2(1);
        self->field_0x8 = 0;
    }
}

extern "C" __declspec(noinline) void func_8022FDF4(SArts2FDF4* self) {
    switch (self->field_0x18) {
        case 0: func_8022FF74(); break;
        case 1: func_80230070(); break;
    }
    self->field_0x8->v14(0);
}

// 2-arg C++ overload of func_80124270 (separate mangled symbol), used by func_80230D18.
void func_80124270(void*, u32);

// AnimTransform frame-check helper (defined in COption.cpp / CArtsInfo.cpp).
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
// State-progression byte table (sdata).
extern u8 lbl_eu_806628A8[];
// Switch-case handlers used by func_8022FDF4 (same TU, defined below).
void func_8022FF74();
void func_80230070();

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

void func_8022FF00(SArtsSub8022FA58* self, int arg2) {
    SArtsManagerRoot* root =
        (SArtsManagerRoot*)func_8009EC9C(func_8023040C(self, self->field_0x14));
    SArtsManager* obj = &root->mObj17C;
    s32 prev = obj->v128();               // vtable +0x200
    obj->v126(prev - arg2);               // vtable +0x1F8
    func_80230160(self);
}

void func_8022FF74(){}

void func_80230070(){}



void func_80230374(){}

extern "C" __declspec(noinline) u8 func_8023040C(SArtsSub8022FA58* self, u32 idx) {
    if (idx < self->field_0x21) {
        return self->field_0x19[idx];
    } else {
        return 0;
    }
}

void __ct__8023042C(SArts3042C* self, u32 a, u32 b) {
    self->field_0x00 = a;
    self->field_0x04 = b;
    self->field_0x08 = 0;
    self->field_0x0C = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1C = 0;
    self->field_0x20 = 4;
    self->field_0x21 = 4;
    self->field_0x22 = 0;
    self->field_0x23 = 1;
    self->field_0x24 = 0;
    self->field_0x25 = 2;
    self->field_0x26 = 0;
    self->field_0x27 = 0;
    self->field_0x28 = 1;
}

extern "C" void* __dt__80230484(void* self, int flags) {
    if (self && flags > 0) {
        ::operator delete(self);
    }
    return self;
}

void func_802304C4(){}

void func_8023066C(){}

void func_802306F0(){}

void func_802307A4(SArtsDrawBox* self, nw4r::lyt::DrawInfo* info) {
    if (self->field_0x22 != 0 && self->field_0x28 != 0) {
        func_80137038(self->mLayout08, info, 0, 1);
        func_80137038(self->mLayout14, info, 0, 1);
    }
}

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

void func_80230D18(SArts30D18* self) {
    if (self->field_0x23 != 0) {
        self->field_0x24 = 0;
        func_80124270(self->field_0x08->field_0x10, 1);
        func_80124270(self->field_0x14->field_0x10, 0);
    }
}

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

void __ct__802319AC(SArts319AC* self, u32 a, u32 b, u32 c) {
    self->field_0x00 = a;
    self->field_0x04 = b;
    self->field_0x08 = 0;
    self->field_0x0C = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1C = 0;
    self->field_0x20 = 0;
    self->field_0x21 = 0;
    self->field_0x22 = 0;
    self->field_0x23 = 1;
    self->field_0x24 = 0;
    self->field_0x25 = 2;
    self->field_0x26 = 0;
    self->field_0x28 = c;
    self->field_0x12C = 0;
    self->field_0x12D = 0;
    self->field_0x12E = 1;
}

extern "C" void* __dt__80231A08(void* self, int flags) {
    if (self && flags > 0) {
        ::operator delete(self);
    }
    return self;
}

void func_80231A48(){}

void func_80231C30(){}

void func_80231CB4(){}

void func_80231D68(SArtsDrawBox* self, nw4r::lyt::DrawInfo* info) {
    if (self->field_0x22 != 0 && self->field_0x12E != 0) {
        func_80137038(self->mLayout08, info, 0, 1);
        func_80137038(self->mLayout14, info, 0, 1);
    }
}

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

u8 func_802322F4(SArts322BC* self, int key) {
    u8 count = self->field_0x12C;
    if (count == 0) return 0;
    if (key == -1) {
        int o = (((int)self->field_0x21 + (int)self->field_0x20) << 4);
        return *(self->mTable + o + 2);
    }
    for (u8 i = 0; i < count; i++) {
        if ((int)self->mTable[i * 16] == key) {
            return self->mTable[i * 16 + 2];
        }
    }
    return 0;
}

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

u8 func_80232734(SArts322BC* self) {
    u8 count = self->field_0x12C;
    if (count > 5) {
        s32 r = 0;
        if (self->field_0x20 == 4 && (s8)self->field_0x21 == (s32)count - 5) {
            r = 1;
        }
        return r;
    }
    s32 last = (count > 0) ? ((s32)count - 1) : 0;
    s32 r = 0;
    if ((s8)self->field_0x20 == last && (s8)self->field_0x21 == 0) {
        r = 1;
    }
    return r;
}

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

void func_8023352C(CArtsList* self) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->field_0x14 = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)handle, lbl_eu_8050AC70 + 0x2ba, self, 0, 0);
    func_801F34F4(&self->mScrollBar);
    func_80235814(&self->mSubObj74);
    ((SArtsWinE8*)&self->mSysWinE8)->v32();
}

void func_8023359C(CMenuArtsSet* self) {
    if (self->field_0x28 != 0 && self->field_0x2C != 0) {
        switch (self->field_0x2C) {
            case 1: func_80234EB8(self); break;
            case 2: func_80234F7C(self); break;
            case 4: func_80234FDC(self); break;
            case 5: func_8023506C((SArts3506C*)self); break;
            case 6: func_802350B8(self); break;
        }
        self->field_0x1C->v14(0);
        func_801F3540(self->field_0x34);
        func_8023587C(&self->mSubObj74);
        func_8022B748(&self->mSubObjE8);
        func_8022FDF4(&self->field_0x124);
        func_802306F0(self->mSubObj148);
        func_80231CB4(&self->mSubObj148[0x2C]);
    }
}

void func_80233674(){}

void func_80233760(){}

u8 func_8023380C(CArtsList* self) {
    if (CScrollBar_isVisible(&self->mScrollBar) == 0) return 0;
    if (func_80235A90(&self->mSubObj74) == 0) return 0;
    if (CSysWin_isReady(&self->mSysWinE8) != 0) {
        return self->field_0x30;
    }
    return 0;
}

u8 CMenuArtsSet::func_80233880() { return mField31; }

void func_80233888(){}

u8 func_8023390C(CMenuArtsSet* self) {
    if (func_80235F50(&self->mSubObj74) != 0) return 1;
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return 1;
    return self->field_0x2A6;
}

void func_80235F50(void* self);
void CMenuArtsSet::func_80233968() { func_80235F50(&mSubObj74); }

void func_80233970(CMenuArtsSet* self) {
    if (self->field_0x2C != 0) return;
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return;
    self->field_0x2C = 1;
    self->mField31 = 0;
    func_80235AA0(&self->mSubObj74);
    func_80235124(self);
}

void func_802339D4(CMenuArtsSet* self) {
    if (self->field_0x2C != 3) return;
    if (CSysWin_getUnk34(&self->mSubObjE8) != 0) return;
    self->field_0x2C = 4;
    self->mField31 = 0;
    func_8023080C((SArtsSub8022FA58*)self->mSubObj148, 0);
    self->field_0x196 = 0;
    func_80232B88((SArts327B0*)&self->mSubObj148[0x2C]);
    func_80235AC0(&self->mSubObj74);
    func_80138078__FUl(6);
}

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

int func_80234CA0(SArts34D14* self) {
    if (self->field_0x16A == 0) return 0;
    if ((int)self->field_0x168 != 4) return 0;
    u8 v = func_8023040C(&self->mSubObj124, self->mSubObj124.field_0x14);
    if (v != 1) return 0;
    return func_800A32BC((void*)func_8009EC9C(v)) == 0;
}

u8 func_80234D14(SArts34D14* self) {
    u8 r = func_8023040C(&self->mSubObj124, self->mSubObj124.field_0x14);
    if (r == 1 && lbl_eu_806628A8[r] == 0) return 1;
    return 0;
}

void func_80234D68(){}

void func_80234EB8(){}

extern "C" __declspec(noinline) void func_80234F7C(CMenuArtsSet* self) {
    extern float lbl_eu_80668648;
    if (func_80137444(self->field_0x24, lbl_eu_80668648) != 0) {
        self->field_0x2C = 3;
        self->mField31 = 1;
        func_8023080C((SArtsSub8022FA58*)self->mSubObj148, 1);
        func_80235124(self);
    }
}

void func_80234FDC(){}

extern "C" __declspec(noinline) void func_8023506C(SArts3506C* self) {
    extern float lbl_eu_80668648;
    if (func_80137510(self->mAnim20, lbl_eu_80668648) != 0) {
        self->field_0x31 = 1;
        self->field_0x2C = 0;
    }
}

extern "C" __declspec(noinline) void func_802350B8(CMenuArtsSet* self) {
    if (func_80235A98(&self->mSubObj74) != 0) {
        self->mField31 = 1;
        self->field_0x2C = 3;
        func_80235124(self);
    }
}

void func_80235108(SArts35108* self) {
    if (self->field_0x1C != 0) {
        self->field_0x30 = 1;
        self->field_0x28 = 1;
    }
}

// Recursive stub for the large unmatched retail function func_80235124.
// Self-recursion prevents MWCC inlining; extern "C" gives the C reloc name.
extern "C" void func_80235124(CMenuArtsSet* self) { func_80235124(self); }

void CArtsList::OnFileEvent() {}
