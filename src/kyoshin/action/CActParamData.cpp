// Auto-scaffolded catalog TU for kyoshin/action/CActParamData
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/action/CActParamData.hpp"
#include "monolib/util/FixStr.hpp"
#include <string.h>

extern "C" {
extern char lbl_eu_80526338[];
extern void __dl__FPv(void*);
}

// Retail (unmangled) stream-reset entry; distinct from the TU-local
// C++-linkage func_80055700.
extern "C" int func_80055700(void* table, void* flag, void* stream);

// ============================================================
// Constructor
// ============================================================
CActParamData::CActParamData() {
    void* self = this;
    *(void**)self = &lbl_eu_80526338;
    *(u32*)((u32)self + 0x260) = 0;
    *(u32*)((u32)self + 0x264) = 0;
    *(u32*)((u32)self + 0x2E0) = 0;
    *(u32*)((u32)self + 0x2E4) = 0;
    *(u8*)((u32)self + 0x215) = 0;
    func_80053B24(this, 0, 0, 0, 0);
}

// ============================================================
// Destructor
// ============================================================
CActParamData::~CActParamData() {}

// Cast-only SI ifaces for anonymous vtable tail-call thunks
// (same RTTI-omit pattern as BattleStateV8If in MWCC_CASES).
struct If0x38 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void vf38();
};
struct If0x3C {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038();
    virtual void vf3C();
};
struct If0x40 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C();
    virtual void vf40();
};

extern "C" void func_80056128(If0x40* self) { self->vf40(); }
extern "C" void func_80056138(If0x38* self) { self->vf38(); }
extern "C" void func_80056148(If0x3C* self) { self->vf3C(); }

// Shared cast-only SI iface for the anonymous vtable tail-call thunks.
// First user virtual sits at vt+0x08 (RTTI entries at 0x00/0x04), each +0x04.
struct ActParamThunkIf {
    virtual void v08(); virtual void v0C(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1C(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2C(); virtual void v30(); virtual void v34();
    virtual void v38(); virtual void v3C(); virtual void v40(); virtual void v44();
    virtual void v48(); virtual void v4C(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5C(); virtual void v60(); virtual void v64();
    virtual void v68(); virtual void v6C(); virtual void v70(); virtual void v74();
    virtual void v78(); virtual void v7C(); virtual void v80(); virtual void v84();
    virtual void v88(); virtual void v8C(); virtual void v90(); virtual void v94();
    virtual void v98(); virtual void v9C(); virtual void vA0(); virtual void vA4();
    virtual void vA8();
    virtual void vAC(); virtual void vB0(); virtual void vB4(); virtual void vB8();
    virtual void vBC(); virtual void vC0(); virtual void vC4(); virtual void vC8();
    virtual void vCC(); virtual void vD0(); virtual void vD4(); virtual void vD8();
    virtual void vDC();
};

extern "C" void func_80056158(ActParamThunkIf* self) { self->v44(); }
extern "C" void func_80056168(ActParamThunkIf* self) { self->v48(); }
extern "C" void func_80056178(ActParamThunkIf* self) { self->v4C(); }
extern "C" void func_80056188(ActParamThunkIf* self) { self->v50(); }
extern "C" void func_80056198(ActParamThunkIf* self) { self->v58(); }
extern "C" void func_800561A8(ActParamThunkIf* self) { self->v20(); }
extern "C" void func_800561B8(ActParamThunkIf* self) { self->v24(); }
extern "C" void func_800561C8(ActParamThunkIf* self) { self->v28(); }
extern "C" void func_800561D8(ActParamThunkIf* self) { self->v2C(); }
extern "C" void func_800561E8(ActParamThunkIf* self) { self->v5C(); }
extern "C" void func_800561F8(ActParamThunkIf* self) { self->v60(); }
extern "C" void func_80056208(ActParamThunkIf* self) { self->v64(); }
extern "C" void func_80056218(ActParamThunkIf* self) { self->v68(); }
extern "C" void func_80056228(ActParamThunkIf* self) { self->v6C(); }
extern "C" void func_80056238(ActParamThunkIf* self) { self->v70(); }
extern "C" void func_80056248(ActParamThunkIf* self) { self->v74(); }
extern "C" void func_80056258(ActParamThunkIf* self) { self->v78(); }
extern "C" void func_80056614(ActParamThunkIf* self) { self->v9C(); }
extern "C" void func_80056660(ActParamThunkIf* self) { self->vA4(); }
extern "C" void func_80056670(ActParamThunkIf* self) { self->vA8(); }
extern "C" void func_800566F8(ActParamThunkIf* self) { self->vB8(); }
extern "C" void func_80056790(ActParamThunkIf* self) { self->vC8(); }
extern "C" void func_800567A0(ActParamThunkIf* self) { self->vCC(); }
extern "C" void func_800567B0(ActParamThunkIf* self) { self->vD0(); }
extern "C" void func_800567C0(ActParamThunkIf* self) { self->vD4(); }
extern "C" void func_800567D0(ActParamThunkIf* self) { self->vD8(); }
extern "C" void func_800567E0(ActParamThunkIf* self) { self->vDC(); }
// Param-arg thunks: set r5 (2nd arg) to a constant, tail-call a vtable slot.
struct ActParamThunkIfP {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078();
    virtual void vf7C(u32 a, u32 b);
    virtual void _v080();
    virtual void vf84(u32 a, u32 b);
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C();
    virtual void vfA0(u32 a, u32 b);
    virtual void _v0A4(); virtual void _v0A8();
    virtual void vfAC(u32 a, u32 b);
    virtual void vfB0(u32 a, u32 b);
    virtual void vfB4(u32 a, u32 b);
};

extern "C" void func_80056104(ActParamThunkIfP* self, u32 a, u32 b) { self->vf84(a, (u16)b); }
extern "C" void func_80056268(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 1); }
extern "C" void func_8005627C(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 2); }
extern "C" void func_800564D4(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x20); }
extern "C" void func_800564E8(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x21); }
extern "C" void func_800564FC(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x22); }
extern "C" void func_80056510(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x23); }
extern "C" void func_80056524(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x24); }
extern "C" void func_80056538(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x25); }
extern "C" void func_8005654C(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x26); }
extern "C" void func_80056560(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x27); }
extern "C" void func_80056574(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x28); }
extern "C" void func_80056588(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x29); }
extern "C" void func_8005659C(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x2a); }
extern "C" void func_800565B0(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x2b); }
extern "C" void func_800565C4(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x2c); }
extern "C" void func_800565D8(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x2d); }
extern "C" void func_800565EC(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x2e); }
extern "C" void func_80056600(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x2f); }
extern "C" void func_80056624(ActParamThunkIfP* self, u32 a) { self->vfA0(a, 0); }
extern "C" void func_80056638(ActParamThunkIfP* self, u32 a) { self->vfA0(a, 1); }
extern "C" void func_8005664C(ActParamThunkIfP* self, u32 a) { self->vfA0(a, 2); }
extern "C" void func_80056680(ActParamThunkIfP* self, u32 a) { self->vfB0(a, 0); }
extern "C" void func_80056694(ActParamThunkIfP* self, u32 a) { self->vfB0(a, 1); }
extern "C" void func_800566A8(ActParamThunkIfP* self, u32 a) { self->vfB0(a, 2); }
extern "C" void func_800566BC(ActParamThunkIfP* self, u32 a) { self->vfB0(a, 3); }
extern "C" void func_800566D0(ActParamThunkIfP* self, u32 a) { self->vfB4(a, 0); }
extern "C" void func_800566E4(ActParamThunkIfP* self, u32 a) { self->vfB4(a, 1); }
extern "C" void func_80056708(ActParamThunkIfP* self, u32 a) { self->vfAC(a, 0); }
extern "C" void func_8005671C(ActParamThunkIfP* self, u32 a) { self->vfAC(a, 1); }


void initParamDefaults(){}
void setupParamData(){}
void calcFloatField(){}
void findByteInArray(){}
void updateFloatInterp(){}
void setupWithFlag(){}
void processDataBlock(){}
void setupWithFlagAlt(){}
void copyParamBlock(){}
void iterLinkedList(){}

void nop_80054A20() {}

unsigned char getActParamByteBySel(unsigned char* p, int sel) {
    if (sel != 0) {
        return p[0x344];
    }
    return p[0x2c4];
}

void checkBothBlocks(){}
void validateBlockData(){}
void processBlockCore(void* self);
void callBlockAt2E0(void* self) { processBlockCore((u8*)static_cast<CActParamData*>(self)->getResetBlock2E0()); }

void nop_800550D8() {}
void nop_800550DC() {}
void nop_800550E0() {}
void nop_800550E4() {}

void processWithFlag24(){}

void nop_800554D0() {}
void nop_800554D4() {}
void nop_800554D8() {}

void checkBlockBitFlag(){}
void checkType3Data(){}
void initDataEntries(){}
void findEntryByIndex(){}
void processCode29(){}
void processCode29Ext(){}
void storeResult(){}
void processDataArray(){}
void getFloatFieldA(){}
void getFloatFieldB(){}
void resetBlockFields(){}

extern "C" void* func_80055EA0(void* param) {
    void* val = *(void**)((u32)param + 0x2F4);
    return val ? (void*)((u32)val + 0x14) : 0;
}

void hasAnyFieldSet(){}
void saveStateFields(){}
void clearStateFields(){}

int getShortValue_A(void* self) {
    u32* p = (u32*)self;
    void* ptr = p[0x2D4/4] ? (void*)p[0x2D4/4] : (void*)p[0x354/4];
    return ptr ? *(s16*)((u32)ptr + 0x08) : -1;
}

s16 getShortValue_B(void* self) {
    u32* p = (u32*)self;
    void* ptr = p[0x2D4/4] ? (void*)p[0x2D4/4] : (void*)p[0x354/4];
    return ptr ? *(s16*)((u32)ptr + 0x0A) : -1;
}

void clearTwoFields(void* self) {
    u32* p = (u32*)self;
    p[0x2D4/4] = 0;
    p[0x354/4] = 0;
}

void getDataFromOffset48(){}

void* getNonNullPtr(void* param) {
    u32* p = (u32*)param;
    if (p[0x274/4]) {
        void* ptr = (void*)p[0x2D8/4];
        if (ptr) return ptr;
    }
    if (p[0x2F4/4]) {
        void* ptr = (void*)p[0x358/4];
        if (ptr) return ptr;
    }
    return 0;
}

bool alwaysFalse_1() { return false; }

// Vtable thunk calls
void actCallVt34(void* self) { void** v = *(void***)self; ((void (*)(void*))v[0x34/4])(self); }
void actCallVt98(void* self) { void** v = *(void***)self; ((void (*)(void*))v[0x98/4])(self); }
void actCallVt88(void* self) { void** v = *(void***)self; ((void (*)(void*))v[0x88/4])(self); }
void actCallVt8C(void* self) { void** v = *(void***)self; ((void (*)(void*))v[0x8c/4])(self); }
void actCallVt84_u16(){}

void actCallVt40(){}
void actCallVt38(){}
void actCallVt3C(){}
void actCallVt44(){}
void actCallVt48(){}
void actCallVt4C(){}
void actCallVt50(){}
void actCallVt58(){}
void actCallVt20(){}
void actCallVt24(){}
void actCallVt28(){}
void actCallVt2C(){}
void actCallVt5C(){}
void actCallVt60(){}
void actCallVt64(){}
void actCallVt68(){}
void actCallVt6C(){}
void actCallVt70(){}
void actCallVt74(){}
void actCallVt78(){}
void actParamThunk_1(){}
void actParamThunk_2(){}

struct CActParamDataVTableIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual int vf7C(int a, int b);
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
};

extern "C" void func_800560E4(void* self) { ((CActParamDataVTableIf*)self)->_v090(); }
extern "C" void func_800560F4(void* self) { ((CActParamDataVTableIf*)self)->_v094(); }
extern "C" void func_80056118(void* self) { ((CActParamDataVTableIf*)self)->_v030(); }

int actParamThunk_3(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 3); }
int actParamThunk_4(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 4); }
int actParamThunk_5(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 5); }
extern "C" int func_800562CC(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 6); }
int actParamThunk_7(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 7); }
int actParamThunk_8(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 8); }
int actParamThunk_9(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 9); }
int actParamThunk_10(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 10); }
int actParamThunk_11(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 11); }
int actParamThunk_12(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 12); }
int actParamThunk_13(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 13); }
int actParamThunk_14(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 14); }
int actParamThunk_15(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 15); }
int actParamThunk_16(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 16); }
int actParamThunk_17(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 17); }
int actParamThunk_18(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 18); }
int actParamThunk_19(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 19); }
int actParamThunk_20(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 20); }
int actParamThunk_21(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 21); }
int actParamThunk_22(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 22); }
int actParamThunk_23(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 23); }
int actParamThunk_24(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 24); }
int actParamThunk_25(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 25); }
int actParamThunk_26(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 26); }
int actParamThunk_27(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 27); }
int actParamThunk_28(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 28); }
int actParamThunk_29(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 29); }
extern "C" int func_800564AC(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 30); }
int actParamThunk_31(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 31); }

void actParamThunk_32(){}
void actParamThunk_33(){}
void actParamThunk_34(){}
void actParamThunk_35(){}
void actParamThunk_36(){}
void actParamThunk_37(){}
void actParamThunk_38(){}
void actParamThunk_39(){}
void actParamThunk_40(){}
void actParamThunk_41(){}
void actParamThunk_42(){}
void actParamThunk_43(){}
void actParamThunk_44(){}
void actParamThunk_45(){}
void actParamThunk_46(){}
void actParamThunk_47(){}

void actCallVt9C(){}
void actThunkA0_0(){}
void actThunkA0_1(){}
void actThunkA0_2(){}
void actCallVtA4(){}
void actCallVtA8(){}
void actThunkB0_0(){}
void actThunkB0_1(){}
void actThunkB0_2(){}
void actThunkB0_3(){}
void actThunkB4_0(){}
void actThunkB4_1(){}
void actCallVtB8(){}
void actThunkAC_0(){}
void actThunkAC_1(){}
void actBoundCallVtBC(){}
void actBoundCallVtC0(){}
void actCallVtC8(){}
void actCallVtCC(){}
void actCallVtD0(){}
void actCallVtD4(){}
void actCallVtD8(){}
void actCallVtDC(){}

bool alwaysFalse_2() { return false; }
bool alwaysFalse_3() { return false; }
bool alwaysFalse_4() { return false; }

void setterBit29_A(){}
void setterBit29_B(){}
void setterBit29_C(){}
void setterBit29_D(){}
void setterBit29_E(){}
void setterBit28_A(){}
void setterBit28_B(){}
void setterBit29_F(){}
void setterBit29_G(){}
void setterBit29_H(){}
void setterBit29_I(){}
void setterBit29_J(){}
void setterBit28_C(){}
void setterBit29_K(){}
void setterBit28_D(){}
void setterBit28_E(){}
void setterBit29_L(){}
void setterBit29_M(){}
void setterBit28_F(){}
void setterBit28_G(){}
void setterBit29_N(){}
void setterBit28_H(){}
void setterBit29_O(){}
void setterBit28_I(){}
void setterBit29_Find(){}
void setterBit28_J(){}
void setterBit0_ShortPair(){}
void setterBit1_ShortPair(){}

// Flag-gated setters: when bit 2 of `flags` is set, store `val` into the
// destination field; always return 0.
int func_80056CE4(u32 flags, CActParamSet25C* dst, void* unused, u32 val) {
    if ((flags & 4) == 0) return 0;
    dst->field_0x25C = val;
    return 0;
}

int func_80056EAC(u32 flags, CActParamSet18* dst, void* unused, u32 val) {
    if ((flags & 4) == 0) return 0;
    dst->field_0x18 = val;
    return 0;
}

int func_80057264(u32 flags, void* unused, CActParamSet74* dst, u32 val) {
    if ((flags & 4) == 0) return 0;
    dst->field_0x74 = val;
    return 0;
}

int func_80057654(u32 flags, void* unused, CActParamSet78* dst, u32 val) {
    if ((flags & 4) == 0) return 0;
    dst->field_0x78 = val;
    return 0;
}

// Flag-gated float copy: when bit 2 of `flags` is set, copy the float from
// src+0x08 to dst+0x14; always return 0.
int func_80056808(u32 flags, CActParamFloatDst14* dst, void* unused, const CActParamFloatSrc08* src) {
    if ((flags & 4) == 0) return 0;
    dst->mFloat14 = src->mFloat08;
    return 0;
}

void buildVTable() {}

// ml::FixStr<N> is defined in monolib/util/FixStr.hpp; emit only the retail
// specialization owned by this split (format__Q22ml10FixStr<16>FPCce).
template <>
void ml::FixStr<16>::format(const char* fmt, ...) {}

// Reset the 0x218 short table to -1 and clear the flag word at 0x258.
void func_80053A90(CActParamData* self) {
    self->mField258 = 0;
    for (int i = 0; i < 32; i++) {
        self->mShorts218[i] = 0xFFFF;
    }
}
// True when any inline byte entry equals `value`.
bool func_80053F40(CActParamByteList* list, u8 value) {
    int count = list->mCount2C;
    for (int i = 0; i < count; i++) {
        if (value == list->mEntries[i]) {
            return true;
        }
    }
    return false;
}
// Stamp the halfword of the node whose extent covers `target` into the next
// slot of the 0x218 short table; the index-or-zero trick yields 0x8000 for
// any nonzero search index (retail neg/or/srawi sequence).
// Duplicated inline at both call sites to match retail codegen.
#define ACTPARAM_STAMP_SHORT(selfP, targetP)                                   \
    do {                                                                       \
        u32 cnt = (selfP)->mCount210;                                          \
        u32 idx = 0;                                                           \
        ActParamT1Node* hit = reinterpret_cast<ActParamT1Node*>(targetP);      \
        while (cnt > 0) {                                                      \
            ActParamT1Node* nd =                                               \
                static_cast<ActParamT1Node*>((selfP)->mNodes1F0[idx]);         \
            hit = nd;                                                          \
            if (!((u32)nd > (u32)(targetP)) &&                                 \
                (u32)nd + nd->mSize04 > (u32)(targetP)) {                      \
                break;                                                         \
            }                                                                  \
            idx++;                                                             \
            cnt--;                                                             \
        }                                                                      \
        u16 val = *reinterpret_cast<u16*>(reinterpret_cast<u8*>(hit) + 4);     \
        if (idx != 0)                                                          \
            val |= 0x8000;                                                     \
        (selfP)->mShorts218[(selfP)->mField258] = val;                         \
        u32 next = (selfP)->mField258 + 1;                                     \
        (selfP)->mField258 = next;                                             \
        if (next >= 0x20)                                                      \
            (selfP)->mField258 = 0;                                            \
    } while (0)

void func_80053F7C(CActParamData* self, float arg1, float arg2);
void func_80054170() {}
int func_80054438(CActParamData* self, void* data, int sel, u32 gate) {
    // sel picks which sub-object receives the record walk.
    u8* sub = (sel != 0) ? reinterpret_cast<u8*>(self->getResetBlock2E0())
                         : reinterpret_cast<u8*>(&self->mA260);
    u16 off = *reinterpret_cast<u16*>(data);
    // Register the sub-object under the record block's base offset.
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&lbl_eu_80663D50) + off) =
        reinterpret_cast<u32>(sub);
    u8* cur = reinterpret_cast<u8*>(data) + off;
    while (*reinterpret_cast<u16*>(cur + 2) > 1) {
        u16 type = *reinterpret_cast<u16*>(cur + 2);
        bool matched = false;
        // gate==0 or a type-2/0x29 record allows handler dispatch.
        if (gate == 0 || type == 2 || type == 0x29) {
            if (type == 0x29 && lbl_eu_80570788[type](2, self, sub, cur) != 0) {
                matched = true;
            }
        }
        if (matched) {
            ACTPARAM_STAMP_SHORT(self, cur);
            ACTPARAM_STAMP_SHORT(self, data);
            return 1;
        }
        cur += *reinterpret_cast<u16*>(cur);
    }
    return 0;
}
void func_80054614() {}

// Per-frame update of both sub-object float blocks: shift current into the
// shadow slot, accumulate the entry's threshold float, then recompute the
// crossed-threshold flag and streak counters.
void func_80053F7C(CActParamData* self, float arg1, float arg2) {
    ActParamData388* entry = static_cast<ActParamData388*>(self->mEntry04);
    // Sub-object A
    self->mA2B0 = self->mFloat2B4;
    u32 countA = self->mA2BC + 1;
    self->mFloat2B4 = arg1;
    self->mA2BC = countA;
    self->mA2C8 = self->mA2C8 + entry->mFloat388;
    if (static_cast<ActParamObj10*>(self->mObj10)->check80()) {
        self->mA2C0 = self->mA2C0 + 1;
    }
    u8 fireA;
    if (self->mA260 & 0x10000) {
        fireA = (self->mFloat2B4 < self->mA2B0 && self->mA2C8 > lbl_eu_80665F88)
                    ? 1
                    : 0;
    } else {
        fireA = 0;
    }
    self->mA2C4 = fireA;
    if (self->mFloat2B4 < self->mA2B0) {
        self->mA2B0 = self->mFloat2B4 - lbl_eu_80665F88;
    }
    if (self->mFloat2B4 == self->mA2B0) {
        self->mA2B8 = self->mA2B8 + 1;
    } else {
        self->mA2B8 = 0;
    }
    // Sub-object B (mirror of A)
    self->mFloat330 = self->mFloat334;
    u32 countB = self->mField33C + 1;
    self->mFloat334 = arg2;
    self->mField33C = countB;
    self->mFloat348 = self->mFloat348 + entry->mFloat388;
    if (static_cast<ActParamObj10*>(self->mObj10)->check80()) {
        self->mField340 = self->mField340 + 1;
    }
    u8 fireB;
    if (self->mField2E0 & 0x10000) {
        fireB = (self->mFloat334 < self->mFloat330 &&
                 self->mFloat348 > lbl_eu_80665F88)
                    ? 1
                    : 0;
    } else {
        fireB = 0;
    }
    self->mByte344 = fireB;
    if (self->mFloat334 < self->mFloat330) {
        self->mFloat330 = self->mFloat334 - lbl_eu_80665F88;
    }
    if (self->mFloat334 == self->mFloat330) {
        self->mField338 = self->mField338 + 1;
    } else {
        self->mField338 = 0;
    }
    if (self->mA260 & 0x1000000) {
        func_eu_80053FDC();
    }
}

// Full initializer for the parameter data object: resets every sub-object
// field to its default constant, clears the short table to -1, zeroes the
// inline record area, then optionally registers the first key/node pair.
void func_80053B24(CActParamData* self, void* entry, void* obj, void* nodeA,
                   void* nodeB) {
    self->mEntry04 = entry;
    func_80055700(self, nodeA, nodeB);

    // Sub-object A defaults
    self->mA260 = 0;
    self->mA264 = 0;
    self->mA268 = 0;
    self->mA26C = 0;
    self->mA270 = 0;
    self->mField274 = 0;
    self->mA278 = 0;
    self->mA27C = 0;
    self->mA280 = 0;
    self->mFloat284 = lbl_eu_80665F78;
    self->mA288 = lbl_eu_80665F7C;
    self->mFloat28C = lbl_eu_80665F7C;
    self->_pad290 = 0;
    self->mByte291 = 0;
    self->mA2A0 = lbl_eu_80665F80;
    self->mA2A4 = lbl_eu_80665F80;
    self->mA2A8 = 0;
    self->mA294 = 0;
    self->mA298 = 0;
    self->mStr29C = 0;
    self->mA2AC = 0;
    self->mA2B0 = lbl_eu_80665F80;
    self->mFloat2B4 = lbl_eu_80665F80;
    self->mA2B8 = 0;
    self->mA2BC = 0;
    self->mA2C0 = 0;
    self->mA2C4 = 0;
    self->mA2C8 = lbl_eu_80665F80;
    self->mField2CC = 0;
    self->mFloat2D0 = lbl_eu_80665F7C;
    self->mField2D4 = 0;
    self->mPtr2D8 = 0;
    self->mShort2DC = -1;
    // Sub-object B defaults
    self->mField2E0 = 0;
    self->mField2E4 = 0;
    self->mField2E8 = 0;
    self->mField2EC = 0;
    self->mField2F0 = 0;
    self->mPtr2F4 = 0;
    self->mField2F8 = 0;
    self->mField2FC = 0;
    self->mField300 = 0;
    self->mFloat304 = lbl_eu_80665F78;
    self->mFloat308 = lbl_eu_80665F7C;
    self->mFloat30C = lbl_eu_80665F7C;
    self->mByte310 = 0;
    self->mByte311 = 0;
    self->mFloat320 = lbl_eu_80665F80;
    self->mFloat324 = lbl_eu_80665F80;
    self->mField328 = 0;
    self->mField314 = 0;
    self->mField318 = 0;
    self->mField31C = 0;
    self->mField32C = 0;
    self->mFloat330 = lbl_eu_80665F80;
    self->mFloat334 = lbl_eu_80665F80;
    self->mField338 = 0;
    self->mField33C = 0;
    self->mField340 = 0;
    self->mByte344 = 0;
    self->mFloat348 = lbl_eu_80665F80;
    self->mField34C = 0;
    self->mFloat350 = lbl_eu_80665F7C;
    self->mField354 = 0;
    self->mPtr358 = 0;
    self->mShort35C = -1;
    // Top-level fields
    self->mFloat14 = lbl_eu_80665F84;
    self->mField1C = -1;
    self->mField20 = -1;
    self->mField24 = 0;
    self->mCount210 = 0;
    self->mNode0C = 0;
    self->mNode08 = 0;
    self->mPtr18 = 0;
    self->mByte214 = 0;
    self->mByte216 = 0;
    self->mField2C = 0;
    self->mField25C = 0;
    self->mField258 = 0;
    for (int i = 0; i < 32; i++) {
        self->mShorts218[i] = 0xFFFF;
    }
    // Zero the inline record area in 0x20-byte chunks.
    u32* blk = reinterpret_cast<u32*>(reinterpret_cast<u8*>(self) + 0x30);
    for (int i = 0; i < 13; i++, blk += 8) {
        blk[0] = 0;
        blk[1] = 0;
        blk[2] = 0;
        blk[3] = 0;
        blk[4] = 0;
        blk[5] = 0;
        blk[6] = 0;
        blk[7] = 0;
    }
    // Optional first node registration.
    self->mObj10 = obj;
    if (nodeA != 0 && nodeB != 0) {
        self->mNode0C = nodeA;
        self->mNode08 = reinterpret_cast<u8*>(nodeB) + 0x20;
        func_800555EC(self, nodeA, nodeB);
        u32 cnt = self->mCount210;
        self->mKeys1D0[cnt] = nodeA;
        self->mNodes1F0[cnt] = nodeB;
        self->mCount210 = cnt + 1;
    }
}
// Step to the next record in the chain.
static inline CActParamWalkRec* advanceWalkRec(CActParamWalkRec* rec) {
    return (CActParamWalkRec*)((u8*)rec + rec->mOffset0);
}

// Walk the type-9 record chain at +0x278; each type-9 record with a zero
// selector triggers the vt+0x0C callback on the host's interface pointer.
void func_80054980(ActParamWalkHost* host) {
    if (host->mRec278 == 0) {
        return;
    }
    CActParamWalkRec* rec = advanceWalkRec(host->mRec278);
    while (rec->mType2 > 1) {
        if (rec->mType2 != 9) {
            rec = advanceWalkRec(rec);
            continue;
        }
        if (rec->mSel08 == 0 && host->mIf24 != 0) {
            host->mIf24->vf0C(host->mField04, rec->mByte0A, rec->mByte0B);
        }
        rec = advanceWalkRec(rec);
    }
}
// Walk the record chain with handler id 9, then run the ctx gate callbacks.
int func_80054A94(CActParamData* self, void* ctx) {
    ActParamCtx94* rec = static_cast<ActParamCtx94*>(ctx);
    if (rec->mRec14 == 0 || self->mObj10 == 0) {
        return 0;
    }
    int sel = reinterpret_cast<u8*>(self->getResetBlock2E0()) !=
              reinterpret_cast<u8*>(ctx);
    if (func_80054438(self, rec->mRec14, sel, 0) != 0) {
        return 1;
    }

    u8* cur = reinterpret_cast<u8*>(rec->mRec14) +
              *reinterpret_cast<u16*>(rec->mRec14);
    while (*reinterpret_cast<u16*>(cur + 2) > 1) {
        u16 type = *reinterpret_cast<u16*>(cur + 2);
        bool matched = false;
        if (type == 0x29 && lbl_eu_80570788[type](9, self, ctx, cur) != 0) {
            matched = true;
        }
        if (matched) {
            ACTPARAM_STAMP_SHORT(self, cur);
            ACTPARAM_STAMP_SHORT(self, rec->mRec14);
            return 1;
        }
        cur += *reinterpret_cast<u16*>(cur);
    }

    ActParamObj10* iface = static_cast<ActParamObj10*>(self->mObj10);
    if (rec->mField1C != 0) {
        if (iface->invoke1C(rec->mField1C)) {
            return 1;
        }
    } else if (rec->mField20 != 0) {
        if (iface->invoke1C(rec->mField20)) {
            return 1;
        }
    }
    if (rec->mField74 != 0) {
        if (iface->invokeC4()) {
            return 1;
        }
    }
    return 0;
}
extern "C" int func_80054A3C(void* self) {
    int a = func_80054A94(static_cast<CActParamData*>(self),
                          reinterpret_cast<u8*>(self) + 0x2E0);
    int b = func_80054A94(static_cast<CActParamData*>(self),
                          reinterpret_cast<u8*>(self) + 0x260);
    return (b | a) != 0;
}
extern "C" __declspec(noinline) void func_80054D3C(void* self, void* member) {}
// func_80054D34: r4 = this+0x2E0, tail-branch to func_80054D3C (retail addi r4,r3,0x2e0; b)
extern "C" void func_80054D34(void* self) {
    func_80054D3C(self, (char*)self + 0x2E0);
}
// vtable-dispatch wrappers (retail: lwz r12,0(r3); lwz r12,N(r12); mtctr; bctr).
// A real C++ virtual call forces MWCC's r12 dispatch (fn-pointer form uses r4).
struct CActVt34 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11();
};
struct CActVt88 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11();
    virtual void m12(); virtual void m13(); virtual void m14(); virtual void m15();
    virtual void m16(); virtual void m17(); virtual void m18(); virtual void m19();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24(); virtual void m25(); virtual void m26(); virtual void m27();
    virtual void m28(); virtual void m29(); virtual void m30(); virtual void m31();
    virtual void m32();
};
struct CActVt8C {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11();
    virtual void m12(); virtual void m13(); virtual void m14(); virtual void m15();
    virtual void m16(); virtual void m17(); virtual void m18(); virtual void m19();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24(); virtual void m25(); virtual void m26(); virtual void m27();
    virtual void m28(); virtual void m29(); virtual void m30(); virtual void m31();
    virtual void m32(); virtual void m33();
};
struct CActVt98 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11();
    virtual void m12(); virtual void m13(); virtual void m14(); virtual void m15();
    virtual void m16(); virtual void m17(); virtual void m18(); virtual void m19();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24(); virtual void m25(); virtual void m26(); virtual void m27();
    virtual void m28(); virtual void m29(); virtual void m30(); virtual void m31();
    virtual void m32(); virtual void m33(); virtual void m34(); virtual void m35();
    virtual void m36();
};
extern "C" void func_800560A4(void* self) { ((CActVt34*)self)->m11(); }
extern "C" void func_800560B4(void* self) { ((CActVt98*)self)->m36(); }
extern "C" void func_800560C4(void* self) { ((CActVt88*)self)->m32(); }
extern "C" void func_800560D4(void* self) { ((CActVt8C*)self)->m33(); }
extern "C" void func_800550E8() {}
// us-80057a94: if (flags & 8) call func_800550E8(a, b, c, 0); always return 0.
// Goto-gate keeps the call block out-of-line with li r3, 0 on both paths.
extern "C" int func_8005742C(u32 flags, void* a, void* b, void* c) {
    extern int func_800550E8(void*, void*, void*, int);
    if (flags & 8) goto call;
    return 0;
call:
    func_800550E8(a, b, c, 0);
    return 0;
}
// Clear the stream's record chain: unlink each linkable record from its table
// slot and stop after the terminator record.
void func_80055700(CActParamLinkTable* table, int flag, CActParamRecStream* stream) {
    if (flag == 0) return;
    if (stream->mFlag14 == 0) return;
    u8* recBase = (u8*)&stream->mFirst;
    CActParamRec* rec = (CActParamRec*)recBase;
    for (;;) {
        u16 type = rec->mType2;
        if (type == 0) {
            rec->mField10 = -1;
            u32 sel = rec->mSel8;
            if (sel != 0) {
                u32 slotVal = table->mSlots[sel];
                if (slotVal == (u32)rec) {
                    table->mSlots[sel] = 0;
                }
            }
        } else if (type == 1) {
            break;
        }
        rec = (CActParamRec*)((u8*)rec + rec->mOffset0);
    }
    // Tail call with both args pointing at the inline first record.
    func_80055960(table, reinterpret_cast<ActParamStrRec*>(recBase),
                  reinterpret_cast<ActParamStrRec*>(recBase));
}
// Push a node pair onto the stack slots, then splice b into the list after
// the last node whose payload differs from b's.
void func_80055AC4(ActParamStack* self, ActParamStackNode* a, ActParamStackNode* b) {
    func_800555EC(self, a, b);
    self->mSlots1D0[self->mCount210] = a;
    u32 idx2 = self->mCount210;
    self->mSlots1F0[idx2] = b;
    self->mCount210 = idx2 + 1;
    if (b->mWord14 <= 1) {
        // Walk toward b: only the payload swaps are skipped once we reach it;
        // the append-at-tail step runs regardless.
        ActParamStackNode* node = self->mNode0C;
        for (;;) {
            if (node != b) {
                func_800557E8(self, reinterpret_cast<ActParamStrRec*>(&node->mBytes20),
                              reinterpret_cast<ActParamStrRec*>(&b->mBytes20));
                func_800557E8(self, reinterpret_cast<ActParamStrRec*>(&b->mBytes20),
                              reinterpret_cast<ActParamStrRec*>(&node->mBytes20));
            }
            if (node->mPtr0C == 0) {
                node->mPtr0C = b;
                b->mPtr08 = node;
                break;
            }
            node = node->mPtr0C;
        }
    }
}
// Unlink the node for `key` from the chain, splice the list back together,
// then re-register every remaining stream. Returns 1 when the tag matches.
int func_80055B88(CActParamData* self, void* key, u32 tag) {
    // Find the registered node whose key matches.
    ActParamNode94* node = 0;
    {
        u32 count = self->mCount210;
        u32 i = 0;
        while (count > 0) {
            if (self->mKeys1D0[i] == key) {
                node = static_cast<ActParamNode94*>(self->mNodes1F0[i]);
                break;
            }
            i++;
            count--;
        }
    }
    if (node->mType10 != 3) {
        return 1;
    }
    u32 left = node->mCount14 - 1;
    node->mCount14 = left;
    if (left != 0) {
        return 0;
    }

    // Unlink: walk from the head until the node is found.
    ActParamNode94* cur = static_cast<ActParamNode94*>(self->mNode0C);
    while (cur != 0) {
        if (cur == node) {
            if (cur->mPrev08 != 0) {
                cur->mPrev08->mNext0C = cur->mNext0C;
            }
            if (cur->mNext0C != 0) {
                cur->mNext0C->mPrev08 = cur->mPrev08;
            }
            cur->mPrev08 = 0;
            cur->mNext0C = 0;
            // Compact the key/node arrays over the removed slot.
            u32 count = self->mCount210;
            u32 j = 0;
            while (j < count) {
                if (self->mKeys1D0[j] == key) {
                    while (j < count - 1) {
                        self->mKeys1D0[j] = self->mKeys1D0[j + 1];
                        self->mNodes1F0[j] = self->mNodes1F0[j + 1];
                        j++;
                    }
                    self->mCount210 = count - 1;
                    break;
                }
                j++;
            }
            break;
        }
        cur = cur->mNext0C;
    }
    if (cur == 0) {
        return 0;
    }

    // Splice: swap payloads with every other node along the chain.
    if (self->mNode0C == node) {
        self->mNode0C = 0;
    }
    ActParamNode94* it = static_cast<ActParamNode94*>(self->mNode0C);
    while (it != 0) {
        if (it != node) {
            func_80055960(reinterpret_cast<CActParamLinkTable*>(self),
                          reinterpret_cast<ActParamStrRec*>(&it->mBytes20),
                          reinterpret_cast<ActParamStrRec*>(&node->mBytes20));
            func_80055960(reinterpret_cast<CActParamLinkTable*>(self),
                          reinterpret_cast<ActParamStrRec*>(&node->mBytes20),
                          reinterpret_cast<ActParamStrRec*>(&it->mBytes20));
        }
        ActParamNode94* nx = it->mNext0C;
        if (it == nx) {
            break;
        }
        it = nx;
    }

    func_80055700(self, reinterpret_cast<void*>(tag), node);
    // Re-register each remaining stream.
    ActParamNode94* it2 = static_cast<ActParamNode94*>(self->mNode0C);
    while (it2 != 0) {
        func_8005577C(reinterpret_cast<CActParamLinkTable*>(self),
                      reinterpret_cast<CActParamRecStream*>(it2));
        ActParamNode94* nx = it2->mNext0C;
        if (it2 == nx) {
            break;
        }
        it2 = nx;
    }

    if (self->mA294 == tag) {
        self->mField1C = 0;
        self->mA294 = 0;
        self->mA26C = 0;
        return 1;
    }
    return 0;
}
// Float getter: read the float at +0x08 of the optional mPtr18 entry,
// falling back to the default sdata2 constant when no entry is set.
float func_80055DB8(CActParamData* self) {
    CActParamDataEntry* entry = static_cast<CActParamDataEntry*>(self->mPtr18);
    if (entry == NULL) {
        return lbl_eu_80665F7C;
    }
    return entry->mFloat08;
}
// Same shape as func_80055DB8 but reads the +0x0C float.
float func_80055DD4(CActParamData* self) {
    CActParamDataEntry* entry = static_cast<CActParamDataEntry*>(self->mPtr18);
    if (entry == NULL) {
        return lbl_eu_80665F7C;
    }
    return entry->mFloat0C;
}

// Reset the SubObjB block: snapshot the tag word, wipe all fields to their
// defaults (three sdata2 float constants), then restore the saved tag.
void func_80055DF0(CActParamData* self) {
    u32 tag = self->mField2EC;
    if (self->mField2E8 == 2) tag = self->mField2E8;
    self->mField2E0 = 0;
    self->mField2E4 = 0;
    self->mField2E8 = 0;
    self->mField2EC = 0;
    self->mPtr2F4 = 0;
    self->mField2F8 = 0;
    self->mField2FC = 0;
    self->mField300 = 0;
    self->mFloat304 = lbl_eu_80665F78;
    self->mFloat308 = lbl_eu_80665F7C;
    self->mFloat30C = lbl_eu_80665F7C;
    self->mByte310 = 0;
    self->mByte311 = 0;
    self->mFloat320 = lbl_eu_80665F80;
    self->mFloat324 = lbl_eu_80665F80;
    self->mField328 = 0;
    self->mField314 = 0;
    self->mField318 = 0;
    self->mField31C = 0;
    self->mField32C = 0;
    self->mFloat330 = lbl_eu_80665F80;
    self->mFloat334 = lbl_eu_80665F80;
    self->mField338 = 0;
    self->mField33C = 0;
    self->mField340 = 0;
    self->mByte344 = 0;
    self->mFloat348 = lbl_eu_80665F80;
    self->mField34C = 0;
    self->mFloat350 = lbl_eu_80665F7C;
    self->mField354 = 0;
    self->mPtr358 = 0;
    self->mShort35C = -1;
    self->mField2F0 = tag;
}

// Reset both sub-objects' saved flag/float shadow slots to defaults.
extern "C" void func_80055F08(void* data) {
    CActParamData* self = (CActParamData*)data;
    float defaultValue = lbl_eu_80665F7C;
    self->mField2CC = 0;
    self->mFloat2D0 = defaultValue;
    self->mField34C = 0;
    self->mFloat350 = defaultValue;
}
extern "C" void func_800563BC(ActParamThunkIfP* self, u32 a) { self->vf7C(a, 0x12); }
// Save the SubObjA flag/float and SubObjB tag/float into their shadow slots.
void func_80055EE4(CActParamData* self) {
    self->mField2CC = self->mField274;
    self->mFloat2D0 = self->mFloat2B4;
    self->mField34C = (u32)self->mPtr2F4;
    self->mFloat350 = self->mFloat334;
}

// True when SubObjB has a tag pointer, or its fallback word at +0x2F8 is set.
int func_80055EBC(CActParamData* self) {
    int result = 0;
    if (self->mPtr2F4 != 0 || self->mField2F8 != 0)
        result = 1;
    return result;
}

// Tail-call the vt+0xC0 lookup with a 16-bit-truncated index, guarded by the
// global table's entry count at +0x60.
int func_80056760(ActParamCallIf* self, void* unused, u32 index) {
    if (lbl_eu_80663D50->mCount60 < index) {
        return 0;
    }
    return self->findEntryByIndexWide(unused, (u16)index);
}

// Flag-gated copies: when bit 2 of `flags` is set, copy src field into dst;
// always return 0.
int func_80056888(u32 flags, void* unused, CActParamSet20* dst, const CActParamWordSrc20* src) {
    if ((flags & 4) == 0) return 0;
    dst->field_0x20 = src->mWord20;
    return 0;
}

int func_800568A8(u32 flags, void* unused, CActParamFloatDst24* dst, const CActParamFloatSrc08* src) {
    if ((flags & 4) == 0) return 0;
    dst->mFloat24 = src->mFloat08;
    return 0;
}

int func_800568C8(u32 flags, CActParamSet1C* dst, void* unused, const CActParamWordSrc08* src) {
    if ((flags & 4) == 0) return 0;
    dst->field_0x1C = src->mWord08;
    return 0;
}
// Byte source zero-extended to a full word on store.
int func_80056C34(u32 flags, void* unused, CActParamSet4C* dst, const CActParamByteSrc08* src) {
    if ((flags & 4) == 0) return 0;
    dst->field_0x4C = src->mByte08;
    return 0;
}
extern "C" void func_80056CC8() {}
// Tail-call virtual slot 0xBC with a 16-bit-truncated index, guarded by the
// global table's entry count.
int func_80056730(ActParamCallIf* self, void* unused, u32 index) {
    if (lbl_eu_80663D50->mCount5C < index) {
        return 0;
    }
    return self->findEntryByIndex(unused, (u16)index);
}
// Flag-gated s16 copy: sign-extended load from src+0x08 stored as a halfword
// at dst+0x7C; always return 0.
int func_80057470(u32 flags, void* unused, CActParamSet7C* dst, const CActParamDataRef* src) {
    if ((flags & 4) == 0) return 0;
    dst->field_0x7C = src->mShort08;
    return 0;
}
extern "C" void func_80056EC8() {}
extern "C" void func_80057084() {}

// Bit-2-gated: store the selector from src+0x08 into dst+0x1C, then walk the
// node chain table->mNodes[sel] until a type-1 terminator; a type-0 node's
// payload address lands in dst+0x38. Always returns 0.
int func_80056828(u32 flags, CActParamNodeTable* table, CActParamSet1C38* dst, const CActParamWordSrc08* src) {
    if ((flags & 4) == 0) return 0;
    u32 sel = src->mWord08;
    dst->mField1C = sel;
    CActParamNode* node = table->mNodes[sel];
    if (node != 0) {
        while (node->mType2 != 1) {
            if (node->mType2 == 0) {
                dst->mPayload38 = (u8*)&node->mPayload;
                break;
            }
            node = (CActParamNode*)((u8*)node + node->mOffset0);
        }
    }
    return 0;
}

// Walk the record stream at stream+0x20; each type-0 record with a nonzero
// selector claims its table slot unless the slot is already held and the next
// record is type 2. Stops after a type-1 record.
void func_8005577C(CActParamLinkTable* table, CActParamRecStream* stream) {
    CActParamRec* rec = &stream->mFirst;
    for (;;) {
        u16 type = rec->mType2;
        if (type == 0) {
            u32 sel = rec->mSel8;
            if (sel != 0) {
                u32* slot = &table->mSlots[sel];
                CActParamRec* next = (CActParamRec*)((u8*)rec + rec->mOffset0);
                if (*slot == 0) {
                    *slot = (u32)rec;
                } else if (next->mType2 != 2) {
                    *slot = (u32)rec;
                }
            }
        } else if (type == 1) {
            return;
        }
        rec = (CActParamRec*)((u8*)rec + rec->mOffset0);
    }
}

// Bit-2-gated: append src's byte to the list if the list is empty and the
// value is not already present. Always returns 0.
int func_80057828(u32 flags, CActParamByteList* list, u8* unused, const CActParamByteSrc08* src) {
    if ((flags & 4) == 0) return 0;
    int count = list->mCount2C;
    if (count >= 1) {
        return 0;
    }
    u8 value = src->mByte08;
    for (int i = 0; i < count; i++) {
        if (value == list->mEntries[i]) {
            return 0;
        }
    }
    list->mEntries[list->mCount2C] = value;
    list->mCount2C = list->mCount2C + 1;
    return 0;
}
int func_80057244(u32 flags, CActParamSet20* dst, void* unused, const CActParamWordSrc08* src) {
    if ((flags & 4) == 0) return 0;
    dst->field_0x20 = src->mWord08;
    return 0;
}
// Flag-gated float+byte copy from src+0x08/+0x0C to dst+0x28/+0x30.
int func_80056C54(u32 flags, void* unused, CActParamCopyDst2830* dst, const CActParamCopySrc080C* src) {
    if ((flags & 4) == 0) return 0;
    dst->mFloat28 = src->mFloat08;
    dst->mByte30 = src->mByte0C;
    return 0;
}
extern "C" void func_80057490() {}
extern "C" void func_8005789C() {}

extern "C" void func_80057CDC__13CActParamDataFv() {}


// When bit 2 of the flag is set, convert the s16 at src+8 to a float via the
// signed 2^52 double-magic and store it at dst+64. Always returns 0.
extern "C" u32 func_80056C7C(u32 flag, void* a2, void* dst, const void* src) {
    if ((flag & 4) != 0) goto convert;
    return 0;
convert:
    *(f32*)((u8*)dst + 0x40) =
        (f32)(s16)*(const s16*)((const u8*)src + 8) * lbl_eu_8066A210;
    return 0;
}

// ============================================================
// us-80058208: func_80057BA0
// Range test against the packed halfword pair at c+0x28/0x2A, then
// dispatch through a function table indexed by (lo halfword - 0x2A).
// ============================================================
int func_80057BA0(u32 flags, ActParamT19ArgA* a, ActParamT19ArgB* b, ActParamT19ArgC* c) {
    if ((flags & 2) == 0) {
        return 0;
    }
    u16 x = c->mShort28;
    u16 y;
    if (x != 0) {
        goto reloadY;
    }
    y = c->mShort2A;
    if (y == 0) {
        goto dispatch;
    }
    // Only an upper bound: pass when t < y.
    if (b->mFloat54 < (float)y) {
        goto dispatch;
    }
reloadY:
    y = c->mShort2A;
    if (y != 0) {
        goto bounds;
    }
    // Only a lower bound: pass when t >= x.
    if (x == 0) {
        goto bounds;
    }
    if (b->mFloat54 >= (float)x) {
        goto dispatch;
    }
bounds:
    if (x == 0) {
        return 0;
    }
    if (y == 0) {
        return 0;
    }
    if (b->mFloat54 >= (float)x) {
        return 0;
    }
    if (b->mFloat54 >= (float)y) {
        return 0;
    }
dispatch:
    u32 packed = c->mField08;
    return lbl_eu_805705F0[(u16)packed - 0x2A](a->mField10, (u16)packed, packed >> 16,
                                               c->mField24)
               ? 1
               : 0;
}

// ============================================================
// us-80054440: func_80053DE8
// Pick sub-object A (sel==0) or B; default to its +0x10 float, fall back
// to the +0x18 float unless a name lookup at the linked ref matches.
// ============================================================
float func_80053DE8(CActParamData* self, int sel) {
    // Per-field selection: retail evaluates the ternary at every access.
    float res = sel != 0 ? self->mFloat304 : self->mFloat284;
    s32 byte = sel != 0 ? self->mByte311 : self->mByte291;
    if (byte == 0) {
        // Name lookup against the linked reference's title at +0x14.
        char* name = sel != 0 ? reinterpret_cast<char*>(self->mField31C) : self->mStr29C;
        if (name != 0) {
            void* p = sel != 0 ? self->mPtr2F4 : reinterpret_cast<void*>(self->mField274);
            if (p != 0) {
                char* p2 = sel != 0 ? reinterpret_cast<char*>(self->mPtr2F4)
                                    : reinterpret_cast<char*>(self->mField274);
                char* name2 = sel != 0 ? reinterpret_cast<char*>(self->mField31C) : self->mStr29C;
                if (strcmp(name2, p2 + 0x14) == 0) {
                    float alt = sel != 0 ? self->mFloat30C : self->mFloat28C;
                    if (alt < lbl_eu_80665F80) {
                        res = alt;
                    }
                }
            }
        }
    } else {
        res = sel != 0 ? self->mFloat30C : self->mFloat28C;
    }
    return res;
}

// ============================================================
// us-80055fc8: func_80055960 / us-80055e50: func_800557E8
// Walk a record chain; for type-0x29 and type-6 records search `list`
// (when the owner's gate word is set) for its end-of-chain sentinel and
// stamp the index into the record; type-1 stops and reports the misses.
// ============================================================
static inline ActParamStrRec* nextStrRec(ActParamStrRec* rec) {
    return reinterpret_cast<ActParamStrRec*>(reinterpret_cast<u8*>(rec) + rec->mOffset0);
}

int func_80055960(CActParamLinkTable* table, ActParamStrRec* list, ActParamStrRec* rec) {
    int count = 0;
    u32 idx = 0;
    for (;;) {
        u16 type = rec->mType2;
        if (type == 0x29) {
            ActParamStrRec* found = 0;
            ActParamStrRec* cur = list;
            if (table->mGate08 != 0) {
                for (;;) {
                    u16 t2 = cur->mType2;
                    if (t2 == 0) {
                        // Keep the last end-marker seen; keep scanning.
                        found = cur;
                    } else if (t2 == 2) {
                        if (strcmp(cur->mName08, rec->mName0C) == 0) {
                            break;
                        }
                    } else if (t2 == 1) {
                        found = 0;
                        break;
                    }
                    cur = nextStrRec(cur);
                }
            }
            if (found != 0) {
                rec->mField24 = idx;
            } else {
                count++;
            }
        } else if (type == 6) {
            ActParamStrRec* found = 0;
            ActParamStrRec* cur = list;
            if (table->mGate08 != 0) {
                for (;;) {
                    u16 t2 = cur->mType2;
                    if (t2 == 0) {
                        found = cur;
                    } else if (t2 == 2) {
                        if (strcmp(cur->mName08, rec->mName08) == 0) {
                            break;
                        }
                    } else if (t2 == 1) {
                        found = 0;
                        break;
                    }
                    cur = nextStrRec(cur);
                }
            }
            if (found != 0) {
                rec->mField20 = idx;
            } else {
                count++;
            }
        } else if (type == 1) {
            return count;
        }
        rec = nextStrRec(rec);
    }
}

int func_800557E8(ActParamStack* self, ActParamStrRec* dst, ActParamStrRec* src) {
    int count = 0;
    for (;;) {
        u16 type = src->mType2;
        if (type == 0x29) {
            ActParamStrRec* found = 0;
            ActParamStrRec* cur = dst;
            if (self->mGate08 != 0) {
                for (;;) {
                    u16 t2 = cur->mType2;
                    if (t2 == 0) {
                        found = cur;
                    } else if (t2 == 2) {
                        if (strcmp(cur->mName08, src->mName0C) == 0) {
                            break;
                        }
                    } else if (t2 == 1) {
                        found = 0;
                        break;
                    }
                    cur = nextStrRec(cur);
                }
            }
            // Stamp only when the slot is still empty.
            if (found != 0) {
                if (src->mField24 == 0) {
                    src->mField24 = reinterpret_cast<u32>(found);
                }
            } else {
                count++;
            }
        } else if (type == 6) {
            ActParamStrRec* found = 0;
            ActParamStrRec* cur = dst;
            if (self->mGate08 != 0) {
                for (;;) {
                    u16 t2 = cur->mType2;
                    if (t2 == 0) {
                        found = cur;
                    } else if (t2 == 2) {
                        if (strcmp(cur->mName08, src->mName08) == 0) {
                            break;
                        }
                    } else if (t2 == 1) {
                        found = 0;
                        break;
                    }
                    cur = nextStrRec(cur);
                }
            }
            if (found != 0) {
                if (src->mField20 == 0) {
                    src->mField20 = reinterpret_cast<u32>(found);
                }
            } else {
                count++;
            }
        } else if (type == 1) {
            return count;
        }
        src = nextStrRec(src);
    }
}

// ============================================================
// us-80057100: func_80056A98
// Bit-3-gated threshold check: notify the data object once, then decide
// via range/edge tests whether to fire the host's callback.
// ============================================================
int func_80056A98(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src) {
    if ((flags & 8) == 0) {
        return 0;
    }
    float t = static_cast<float>(src->mShort08);
    if (t == lbl_eu_80665F80) {
        static_cast<ActParamObj5*>(host->mObj04)->notify14();
    }
    int fire;
    if (vals->mFloat50 <= lbl_eu_80665F80) {
        if (vals->mFloat54 >= lbl_eu_80665F78) {
            fire = 1;
            goto invoke;
        }
    }
    fire = 0;
    if (t > vals->mFloat54) {
        if (t > vals->mFloat50) {
            if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                fire = 1;
            }
        }
    } else if (t == vals->mFloat54 && t == vals->mFloat50 &&
               static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
        if (vals->mField58 < 1) {
            fire = 1;
        }
    }
invoke:
    if (fire != 0) {
        if (host->mCb24 != 0) {
            host->mCb24->invoke10(host->mObj04);
        }
    }
    return 0;
}

// ============================================================
// func_80056D00 / func_80057280 / func_800568E8 / func_80057670
// Same body as func_80056A98; only the final callback differs:
// vt+0x14 (with src), vt+0x1C (with src), vt+0x0C (two bytes from src),
// and vt+0x20 (with src, gated on vals+0x14 being set).
// ============================================================
// The conversion is spelled out at every use (no local): MWCC keeps the
// CSE'd value in a volatile FP register like retail.
#define actSrc5Time(s) ((float)(s)->mShort08)

int func_80056D00(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src) {
    if ((flags & 8) == 0) {
        return 0;
    }
    if (actSrc5Time(src) == lbl_eu_80665F80) {
        static_cast<ActParamObj5*>(host->mObj04)->notify14();
    }
    // fire defaults to 1; only the range/edge tests below can clear it.
    int fire = 1;
    if (vals->mFloat50 <= lbl_eu_80665F80) {
        if (vals->mFloat54 >= lbl_eu_80665F78) {
            fire = 0;
            goto invoke;
        }
    }
    if (actSrc5Time(src) > vals->mFloat54) {
        int g2 = 0;
        if (actSrc5Time(src) > vals->mFloat50) {
            if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                g2 = 1;
            }
        }
        if (g2 != 0) {
            goto invoke;
        }
    }
    {
        int g3 = 0;
        if (actSrc5Time(src) == vals->mFloat54) {
            int r6 = 0;
            if (actSrc5Time(src) == vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                    r6 = 1;
                }
            }
            if (r6 != 0) {
                g3 = 1;
            }
        }
        if (g3 != 0) {
            if (vals->mField58 >= 1) {
                fire = 0;
            }
        } else {
            fire = 0;
        }
    }
invoke:
    if (fire != 0) {
        if (host->mCb24 != 0) {
            reinterpret_cast<ActParamCb14*>(host->mCb24)->invoke14(host->mObj04, src);
        }
    }
    return 0;
}

int func_80057280(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src) {
    if ((flags & 8) == 0) {
        return 0;
    }
    if (actSrc5Time(src) == lbl_eu_80665F80) {
        static_cast<ActParamObj5*>(host->mObj04)->notify14();
    }
    // fire defaults to 1; only the range/edge tests below can clear it.
    int fire = 1;
    if (vals->mFloat50 <= lbl_eu_80665F80) {
        if (vals->mFloat54 >= lbl_eu_80665F78) {
            fire = 0;
            goto invoke;
        }
    }
    if (actSrc5Time(src) > vals->mFloat54) {
        int g2 = 0;
        if (actSrc5Time(src) > vals->mFloat50) {
            if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                g2 = 1;
            }
        }
        if (g2 != 0) {
            goto invoke;
        }
    }
    {
        int g3 = 0;
        if (actSrc5Time(src) == vals->mFloat54) {
            int r6 = 0;
            if (actSrc5Time(src) == vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                    r6 = 1;
                }
            }
            if (r6 != 0) {
                g3 = 1;
            }
        }
        if (g3 != 0) {
            if (vals->mField58 >= 1) {
                fire = 0;
            }
        } else {
            fire = 0;
        }
    }
invoke:
    if (fire != 0) {
        if (host->mCb24 != 0) {
            reinterpret_cast<ActParamCb1C*>(host->mCb24)->invoke1C(host->mObj04, src);
        }
    }
    return 0;
}

int func_800568E8(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src) {
    if ((flags & 8) == 0) {
        return 0;
    }
    if (actSrc5Time(src) == lbl_eu_80665F80) {
        static_cast<ActParamObj5*>(host->mObj04)->notify14();
    }
    // fire defaults to 1; only the range/edge tests below can clear it.
    int fire = 1;
    if (vals->mFloat50 <= lbl_eu_80665F80) {
        if (vals->mFloat54 >= lbl_eu_80665F78) {
            fire = 0;
            goto invoke;
        }
    }
    if (actSrc5Time(src) > vals->mFloat54) {
        int g2 = 0;
        if (actSrc5Time(src) > vals->mFloat50) {
            if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                g2 = 1;
            }
        }
        if (g2 != 0) {
            goto invoke;
        }
    }
    {
        int g3 = 0;
        if (actSrc5Time(src) == vals->mFloat54) {
            int r6 = 0;
            if (actSrc5Time(src) == vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                    r6 = 1;
                }
            }
            if (r6 != 0) {
                g3 = 1;
            }
        }
        if (g3 != 0) {
            if (vals->mField58 >= 1) {
                fire = 0;
            }
        } else {
            fire = 0;
        }
    }
invoke:
    if (fire != 0) {
        if (host->mCb24 != 0) {
            // The two payload bytes ride along with the object pointer.
            reinterpret_cast<ActParamCb0C*>(host->mCb24)->invoke0C(host->mObj04, src->mByte0A,
                                                              src->mByte0B);
        }
    }
    return 0;
}

int func_80057670(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src) {
    if ((flags & 8) == 0) {
        return 0;
    }
    if (vals->mField14 == 0) {
        return 0;
    }
    if (actSrc5Time(src) == lbl_eu_80665F80) {
        static_cast<ActParamObj5*>(host->mObj04)->notify14();
    }
    // fire defaults to 1; only the range/edge tests below can clear it.
    int fire = 1;
    if (vals->mFloat50 <= lbl_eu_80665F80) {
        if (vals->mFloat54 >= lbl_eu_80665F78) {
            fire = 0;
            goto invoke;
        }
    }
    if (actSrc5Time(src) > vals->mFloat54) {
        int g2 = 0;
        if (actSrc5Time(src) > vals->mFloat50) {
            if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                g2 = 1;
            }
        }
        if (g2 != 0) {
            goto invoke;
        }
    }
    {
        int g3 = 0;
        if (actSrc5Time(src) == vals->mFloat54) {
            int r6 = 0;
            if (actSrc5Time(src) == vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                    r6 = 1;
                }
            }
            if (r6 != 0) {
                g3 = 1;
            }
        }
        if (g3 != 0) {
            if (vals->mField58 >= 1) {
                fire = 0;
            }
        } else {
            fire = 0;
        }
    }
invoke:
    if (fire != 0) {
        if (host->mCb24 != 0) {
            reinterpret_cast<ActParamCb20*>(host->mCb24)->invoke20(host->mObj04, src);
        }
    }
    return 0;
}

// ============================================================
// us-80054e3c: func_800547D4
// Initialize the destination block from defaults and the previous entry,
// walk the record chain dispatching through lbl_eu_80570788, then locate
// which host slot's node extent contains src.
// ============================================================
static inline ActParamT1Rec* nextT1Rec(ActParamT1Rec* rec) {
    return reinterpret_cast<ActParamT1Rec*>(reinterpret_cast<u8*>(rec) + rec->mOffset0);
}

void func_800547D4(ActParamT1Host* host, ActParamT1Dst* dst, ActParamT1Src* src) {
    if (src->mPtr08 != 0) {
        dst->mField0C = reinterpret_cast<u32>(src->mPtr08);
    }
    // Inherit fields from the previous entry in the chain.
    ActParamT1Src* prev = dst->mPtr14;
    if (prev != 0) {
        dst->mField04 = prev->mField0C;
    }
    ActParamT1Src* old = dst->mPtr14;
    u8 byte30 = dst->mByte30;
    u32 old38 = dst->mField38;
    float f28 = dst->mFloat28;
    float f40 = dst->mFloat40;
    u32 field0C = src->mField0C;
    dst->mPtr18 = old;
    dst->mPtr14 = src;
    dst->mFloat24 = lbl_eu_80665F78;
    dst->mField20 = 0;
    dst->mField4C = 9999;
    dst->mField00 = field0C;
    dst->mFloat2C = f28;
    dst->mByte31 = byte30;
    dst->mByte30 = 0;
    dst->mFloat28 = lbl_eu_80665F7C;
    dst->mFloat44 = f40;
    dst->mFloat40 = lbl_eu_80665F80;
    dst->mField48 = 0;
    dst->mFloat68 = lbl_eu_80665F80;
    dst->mField78 = 0;
    dst->mField3C = old38;
    dst->mField38 = 0;
    dst->mShort7C = -1;
    host->mField1C = -1;
    host->mField20 = -1;
    host->mField25C = 0;
    host->mField18 = 0;
    dst->mField5C = 0;
    dst->mField60 = 0;
    dst->mField74 = 0;
    host->mFloat14 = lbl_eu_80665F84;

    // Walk the record chain starting past the head; each live record is
    // dispatched through the handler table indexed by its type word.
    ActParamT1Rec* rec = nextT1Rec(reinterpret_cast<ActParamT1Rec*>(dst->mPtr14));
    while (rec->mType2 > 1) {
        lbl_eu_80570788[rec->mType2](4, host, dst, rec);
        rec = nextT1Rec(rec);
    }

    // Find the slot whose node extent [p, p+size) contains src.
    int found = 0;
    u32 count = host->mCount210;
    for (u32 i = 0; i < count; i++) {
        ActParamT1Node* p = host->mSlots1F0[i];
        if (reinterpret_cast<u32>(p) >= reinterpret_cast<u32>(src)) {
            continue;
        }
        if (reinterpret_cast<u32>(p) + p->mSize04 <= reinterpret_cast<u32>(src)) {
            continue;
        }
        found = reinterpret_cast<u32>(host->mSlots1D0[i]);
        break;
    }
    dst->mField34 = found;
    host->mByte216 = 0;
}
