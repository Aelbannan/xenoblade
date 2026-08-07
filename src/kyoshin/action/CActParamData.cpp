// Auto-scaffolded catalog TU for kyoshin/action/CActParamData
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/action/CActParamData.hpp"

extern "C" {
extern void* lbl_eu_80526338;
extern void func_80053B24(void*, void*, void*, void*, void*);
extern void __dl__FPv(void*);
}

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
    func_80053B24(self, 0, 0, 0, 0);
}

// ============================================================
// Destructor
// ============================================================
CActParamData::~CActParamData() {}

// Cast-only SI ifaces for anonymous vtable tail-call thunks
// (same RTTI-omit pattern as BattleStateV8If in MWCC_REFERENCE).
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

extern "C" void func_80056128(void* self) { reinterpret_cast<If0x40*>(self)->vf40(); }
extern "C" void func_80056138(void* self) { reinterpret_cast<If0x38*>(self)->vf38(); }
extern "C" void func_80056148(void* self) { reinterpret_cast<If0x3C*>(self)->vf3C(); }

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

extern "C" void func_80056158(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v44(); }
extern "C" void func_80056168(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v48(); }
extern "C" void func_80056178(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v4C(); }
extern "C" void func_80056188(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v50(); }
extern "C" void func_80056198(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v58(); }
extern "C" void func_800561A8(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v20(); }
extern "C" void func_800561B8(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v24(); }
extern "C" void func_800561C8(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v28(); }
extern "C" void func_800561D8(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v2C(); }
extern "C" void func_800561E8(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v5C(); }
extern "C" void func_800561F8(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v60(); }
extern "C" void func_80056208(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v64(); }
extern "C" void func_80056218(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v68(); }
extern "C" void func_80056228(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v6C(); }
extern "C" void func_80056238(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v70(); }
extern "C" void func_80056248(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v74(); }
extern "C" void func_80056258(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v78(); }
extern "C" void func_80056614(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->v9C(); }
extern "C" void func_80056660(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->vA4(); }
extern "C" void func_80056670(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->vA8(); }
extern "C" void func_800566F8(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->vB8(); }
extern "C" void func_80056790(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->vC8(); }
extern "C" void func_800567A0(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->vCC(); }
extern "C" void func_800567B0(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->vD0(); }
extern "C" void func_800567C0(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->vD4(); }
extern "C" void func_800567D0(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->vD8(); }
extern "C" void func_800567E0(void* self) { reinterpret_cast<ActParamThunkIf*>(self)->vDC(); }
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

extern "C" void func_80056104(void* self, u32 a, u32 b) { reinterpret_cast<ActParamThunkIfP*>(self)->vf84(a, (u16)b); }
extern "C" void func_80056268(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 1); }
extern "C" void func_8005627C(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 2); }
extern "C" void func_800564D4(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x20); }
extern "C" void func_800564E8(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x21); }
extern "C" void func_800564FC(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x22); }
extern "C" void func_80056510(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x23); }
extern "C" void func_80056524(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x24); }
extern "C" void func_80056538(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x25); }
extern "C" void func_8005654C(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x26); }
extern "C" void func_80056560(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x27); }
extern "C" void func_80056574(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x28); }
extern "C" void func_80056588(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x29); }
extern "C" void func_8005659C(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x2a); }
extern "C" void func_800565B0(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x2b); }
extern "C" void func_800565C4(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x2c); }
extern "C" void func_800565D8(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x2d); }
extern "C" void func_800565EC(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x2e); }
extern "C" void func_80056600(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vf7C(a, 0x2f); }
extern "C" void func_80056624(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfA0(a, 0); }
extern "C" void func_80056638(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfA0(a, 1); }
extern "C" void func_8005664C(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfA0(a, 2); }
extern "C" void func_80056680(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfB0(a, 0); }
extern "C" void func_80056694(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfB0(a, 1); }
extern "C" void func_800566A8(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfB0(a, 2); }
extern "C" void func_800566BC(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfB0(a, 3); }
extern "C" void func_800566D0(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfB4(a, 0); }
extern "C" void func_800566E4(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfB4(a, 1); }
extern "C" void func_80056708(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfAC(a, 0); }
extern "C" void func_8005671C(void* self, u32 a) { reinterpret_cast<ActParamThunkIfP*>(self)->vfAC(a, 1); }


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
void callBlockAt2E0(void* self) { processBlockCore(static_cast<CActParamData*>(self)->mSubObj2E0); }

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

void* getSubObjPtr(void* param) {
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
void actCallVt90(void* self) { void** v = *(void***)self; ((void (*)(void*))v[0x90/4])(self); }
void actCallVt94(void* self) { void** v = *(void***)self; ((void (*)(void*))v[0x94/4])(self); }
void actCallVt84_u16(){}
void actCallVt30(void* self) { void** v = *(void***)self; ((void (*)(void*))v[0x30/4])(self); }

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
};

int actParamThunk_3(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 3); }
int actParamThunk_4(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 4); }
int actParamThunk_5(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 5); }
int actParamThunk_6(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 6); }
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
int actParamThunk_30(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 30); }
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

void buildVTable() {}

namespace ml {
template <int N> class FixStr { public: void format(const char*, ...); };
template <> void FixStr<16>::format(const char*, ...) {}
}

extern "C" void func_80053A90() {}
extern "C" void func_80053F40() {}
extern "C" void func_80053F7C() {}
extern "C" void func_80054170() {}
extern "C" void func_80054438() {}
extern "C" void func_80054614() {}
extern "C" void func_80054980() {}
extern "C" void func_80054A94() {}
extern "C" void func_80054D3C() {}
extern "C" void func_800550E8() {}
extern "C" void func_80055700() {}
extern "C" void func_80055AC4() {}
extern "C" void func_80055B88() {}
extern "C" void func_80055DB8() {}
extern "C" void func_80055DD4() {}
extern "C" void func_80055DF0() {}
extern "C" void func_80055EBC() {}
extern "C" void func_80055EE4() {}
extern "C" void func_80055F08() {}
extern "C" void func_800563BC() {}
extern "C" void func_80056730() {}
extern "C" void func_80056760() {}
extern "C" void func_80056808() {}
extern "C" void func_80056888() {}
extern "C" void func_800568A8() {}
extern "C" void func_800568C8() {}
extern "C" void func_80056C34() {}
extern "C" void func_80056C54() {}
extern "C" void func_80056CC8() {}
extern "C" void func_80056CE4() {}
extern "C" void func_80056EAC() {}
extern "C" void func_80056EC8() {}
extern "C" void func_80057084() {}
extern "C" void func_80057244() {}
extern "C" void func_80057264() {}
extern "C" void func_80057470() {}
extern "C" void func_80057490() {}
extern "C" void func_80057654() {}
extern "C" void func_8005789C() {}

extern "C" void func_80057CDC__13CActParamDataFv() {}
