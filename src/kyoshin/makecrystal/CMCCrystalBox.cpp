// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/makecrystal/CMCCrystalBox.hpp"

#include <nw4r/lyt.h>

extern float lbl_eu_80668470;  // anim sentinel constant used by completion checks
extern float lbl_eu_8066845C;

// Retail symbols: func_80137444 keeps the mangled C++ name, but func_80137510
// is the unmangled C name (per retail relocs).
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);

void func_801D216C(void*, u8);
void func_80138078(u32);

// These stub callees are leaf subroutines of this unit. Their bodies call a
// genuinely external function (func_801D216C) so MWCC cannot inline them and
// matched callers emit a real `bl`. Their own bytes are not acceptance targets.

void func_80218018(CMCCrystalBox* self);
void func_80215B78(CMCCrystalBox* self);
void func_8021852C(CMCCrystalBox* self);
void func_8021900C(CMCCrystalBox* self);
void func_8021911C(CMCCrystalBox* self);
void func_802191A4(CMCCrystalBox* self);
void func_8021922C(CMCCrystalBox* self);
void func_80219994(CMCCrystalBox* self, int);
void func_802138B8(){}

void func_80213964(int unused, void* a, void* b) {
    short b_lo = *(short*)b;
    unsigned char b_hi = ((unsigned char*)b)[2];
    short a_lo = *(short*)a;
    unsigned char a_hi = ((unsigned char*)a)[2];
    *(short*)a = b_lo;
    ((unsigned char*)a)[2] = b_hi;
    *(short*)b = a_lo;
    ((unsigned char*)b)[2] = a_hi;
}

void func_80213988(){}

void func_80213B1C(){}

void __ct__CMCCrystalBox(){}

void func_80213D74(){}

struct CMCCrystalBoxParam {
    s16 m0;
    s16 m2;
    s16 m4;
    u8 m6;
    u8 m7;

    void initCrystalBoxParam_80213E04(s16 a, s16 b)
    {
        m0 = a;
        m2 = b;
        m4 = 0;
        m6 = 0;
        m7 = 0;
    }
};

void copyCrystalBoxParam_80213E20(CMCCrystalBoxParam *dst, const CMCCrystalBoxParam *src) {
    unsigned short *d16 = (unsigned short*)dst;
    const unsigned short *s16 = (const unsigned short*)src;
    d16[0] = s16[0];
    d16[1] = s16[1];
    d16[2] = s16[2];
    unsigned char *d8 = (unsigned char*)dst;
    const unsigned char *s8 = (const unsigned char*)src;
    d8[6] = s8[6];
    d8[7] = s8[7];
}

void __dt__80213E4C(){}

void func_80213E8C(CMCCrystalBox* self) {
    u32 i;
    self->unk20 = 0;
    ((u8*)self)[0x29] = 0;
    ((u8*)self)[0x6A] = 0;
    for (i = 0; i < 8; i++) {
        self->subObjPtrs[i] = 0;
        ((u8*)self)[0x21 + i] = 0xFF;
    }
}

void __dt__80213ECC(){}

CMCCrystalBox::~CMCCrystalBox() {}

void func_80213FE4(){}

void func_802142C4(){}

void func_80214408(){}

void func_802144F4(){}

void func_80214634(){}

u8 getByte_69_802146C0(void* self) { return static_cast<CMCCrystalBox*>(self)->unk69; }

u8 func_802146C8(CMCCrystalBox* self) {
    if (self->unk2D1 != 0) return 1;
    if (self->unk1500 != 0) return 1;
    return self->unk2CE;
}

u8 CSysWin_getUnk34(void* self);
void syswinGetUnk34_802146F8(void* self) { CSysWin_getUnk34((char*)self + 0x290); }

void func_80214700(){}

void func_802147F4(){}

void func_8021488C(){}

void func_8021492C(){}

void func_802149C4(){}

void func_80214A54(){}

void func_80214C7C(){}

void func_80214EBC(){}

void func_80215144(){}

void func_80215408(){}

void func_80215490(){}

void func_80215518(){}

void func_802156C0(){}

struct CMCCrystalBoxLookup {
    int values[8];
    s8 valueIndex[0x49 - 0x20];
};

int lookupIndexedValue_80215AE8(void* self) {
    CMCCrystalBox* obj = static_cast<CMCCrystalBox*>(self);
    if (obj->unk20 == 0) return 0;
    CMCCrystalBoxLookup* lookup = (CMCCrystalBoxLookup*)obj;
    return lookup->values[lookup->valueIndex[((u8*)obj)[0x29]]];
}

void func_80215B18(CMCCrystalBox* self) {
    // Free-slot bookkeeping: unclaim the slot owned at [0x20+slotIdx] and
    // decrement the header counts.
    u8 count = self->unk20;
    if (count == 0) return;

    u8 slotIdx = ((u8*)self)[0x29];
    int* table = (int*)self;
    s8 owner = (s8)((u8*)self)[0x20 + slotIdx];
    if (table[owner] == 0) return;

    table[owner] = 0;
    self->unk20 = count - 1;
    ((u8*)self)[0x21 + slotIdx] = 0xFF;
    ((u8*)self)[0x29] = slotIdx - 1;
    func_80215B78(self);
}

void func_80215B78(CMCCrystalBox* self){ (void)self; func_801D216C((void*)0, 0); }

void func_80215D98(){}

u8 getByte_2D4_8021624C(void* self) { return static_cast<CMCCrystalBox*>(self)->unk2D4; }

u8 getByte_2D5_80216254(void* self) { return static_cast<CMCCrystalBox*>(self)->unk2D5; }

void func_8021625C(){}

unsigned long tableGet_802165CC(unsigned long* table, unsigned int idx) {
    if (idx >= 8) {
        return 0;
    }
    return table[(unsigned char)idx];
}

void copyCrystalBoxParam_802165E8(CMCCrystalBoxParam *dest, const CMCCrystalBoxParam *src) {
    unsigned short *d = (unsigned short *)dest;
    const unsigned short *s = (const unsigned short *)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    unsigned char *db = (unsigned char *)dest;
    const unsigned char *sb = (const unsigned char *)src;
    db[6] = sb[6];
    db[7] = sb[7];
}

void func_80216614(){}

void func_80216698(CMCCrystalBox* self) {
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;

    // Countdown timer at +0x14F2; when it wraps sub-range, reload from +0x14F1.
    u8 v = self->field_14F2 - 1;
    self->field_14F2 = v;
    if ((s8)v < 0) {
        self->field_14F2 = self->field_14F1 - 1;
    }

    func_80219994(self, 1);
    func_80218018(self);
    func_80138078(0x70);
}

void func_80216718(){}

void func_80216850(){}

void func_80216A20(){}

void func_80216AEC(){}

void func_80216B7C(){}

void func_80216BC8(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[7], lbl_eu_80668470) == 0) return;
    self->unk64 = 3;
    func_8021911C(self);
    self->unk69 = 1;
    func_801D216C((u8*)self + 0x6c, 1);
    func_801D216C((u8*)self + 0x84, 1);
    func_8021852C(self);
}

void func_80216C3C(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[7], 1.0f) != 0) {
        func_8021900C(self);
        self->unk64 = 5;
    }
}

void func_80216C88(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[6], lbl_eu_80668470) == 0) return;
    self->unk69 = 1;
    self->unk64 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
}

void func_80216CE0(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[14], lbl_eu_80668470) == 0) return;
    self->unk69 = 1;
    self->unk64 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
}

void func_80216D38(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[9], lbl_eu_80668470) == 0) return;
    self->unk64 = 9;
    func_8021922C(self);
}

void func_80216D84(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[10], lbl_eu_80668470) == 0) return;
    self->unk69 = 1;
    func_8021852C(self);
    self->unk64 = 3;
}

void func_80216DD8(){}

void func_80216E1C(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[12], lbl_eu_80668470) == 0) return;
    self->unk64 = 3;
    self->subObjPtrs[12]->SetFrame(lbl_eu_8066845C);
}

void func_80216E6C(){}

void func_80216EB0(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[10], lbl_eu_80668470) == 0) return;
    self->unk64 = 0xe;
    func_802191A4(self);
}

void func_80216EFC(){}

void func_80216F8C(){}

void func_80217098(){}

void func_80217434(){}

void func_802177D0(){}

u8 func_80217BDC(void* self) {
    u32 vtable = *(u32*)self;
    u32 kind = (vtable >> 16) & 0xF;
    u32 flag = ((u8*)self)[7] & 3;
    if (kind != 9) return 0;
    if (flag != 1) return 0;
    return 1;
}

void func_80217C0C(){}

void func_80218018(CMCCrystalBox* self){ (void)self; func_801D216C((void*)0, 0); }

void func_802180B4(){}

void func_80218460(){}

void func_8021852C(CMCCrystalBox* self){ (void)self; func_801D216C((void*)0, 0); }

void func_8021899C(){}

void func_80218A80(){}

void func_80218B10(){}

struct CMCCrystalRec5 { unsigned short m0; unsigned short m2; unsigned char m4; };
void copyCrystalRec5_80218FD4(CMCCrystalRec5* dst, const CMCCrystalRec5* src) {
    *dst = *src;
}

struct CMCCrystalRec5Ex {
    unsigned short a;
    unsigned short b;
    unsigned char c;
};

void copyCrystalRec5Ex_80218FF0(CMCCrystalRec5Ex* dst, CMCCrystalRec5Ex* src) {
    unsigned short a = src->a;
    unsigned short b = src->b;
    unsigned char c = src->c;
    dst->a = a;
    dst->b = b;
    dst->c = c;
}

void func_8021900C(CMCCrystalBox* self){ (void)self; func_801D216C((void*)0, 0); }

void func_80219094(){}

void func_8021911C(CMCCrystalBox* self){ (void)self; func_801D216C((void*)0, 0); }

void func_802191A4(CMCCrystalBox* self){ (void)self; func_801D216C((void*)0, 0); }

void func_8021922C(CMCCrystalBox* self){ (void)self; func_801D216C((void*)0, 0); }

void func_802192B4(){}

void func_80219348(){}

void func_802193D0(){}

void func_80219464(){}

void func_802194EC(){}

void func_80219994(CMCCrystalBox* self, int a){ (void)self; (void)a; func_801D216C((void*)0, 0); }

void func_80219AF0(){}

void copyShortPair_80219D10(void* dst, void* src){
    *(unsigned short*)dst = *(unsigned short*)src;
    *(unsigned short*)((char*)dst + 2) = *(unsigned short*)((char*)src + 2);
}

void CMCCrystalBox::OnFileEvent() {}

void func_80219D10(CMCCrystalBoxParam* dst, const CMCCrystalBoxParam* src) {
    dst->m0 = (unsigned short)src->m0;
    dst->m2 = (unsigned short)src->m2;
}
