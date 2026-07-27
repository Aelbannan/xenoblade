// Auto-scaffolded catalog TU for kyoshin/action/CActParamData
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/action/CActParamData.hpp"

CActParamData::CActParamData() {}

CActParamData::~CActParamData() {}

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
void callBlockAt2E0(void* self) { processBlockCore(reinterpret_cast<char*>(self) + 0x2e0); }

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
    void* val = *(void**)(reinterpret_cast<char*>(param) + 0x2f4);
    if (!val) return 0;
    return reinterpret_cast<char*>(val) + 0x14;
}

void hasAnyFieldSet(){}

void saveStateFields(){}

void clearStateFields(){}

int getShortValue_A(void* self) {
    char* base = reinterpret_cast<char*>(self) + 0x260;
    if (*reinterpret_cast<int*>(reinterpret_cast<char*>(self) + 0x2d4) == 0)
        base = reinterpret_cast<char*>(self) + 0x2e0;
    void* ptr = *reinterpret_cast<void**>(base + 0x74);
    if (ptr == NULL)
        return -1;
    return *reinterpret_cast<short*>(reinterpret_cast<char*>(ptr) + 8);
}

s16 getShortValue_B(void* self) {
    u8* base = reinterpret_cast<u8*>(self) + 0x260;
    if (*reinterpret_cast<s32*>(reinterpret_cast<u8*>(self) + 0x2d4) == 0)
        base = reinterpret_cast<u8*>(self) + 0x2e0;
    void* ptr = *reinterpret_cast<void**>(base + 0x74);
    if (!ptr) return -1;
    return *reinterpret_cast<s16*>(reinterpret_cast<u8*>(ptr) + 0xa);
}

void clearTwoFields(void* self) {
    *reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(self) + 0x2d4) = 0;
    *reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(self) + 0x354) = 0;
}

void getDataFromOffset48(){}

void* getNonNullPtr(void* param) {
    if (*reinterpret_cast<int*>(reinterpret_cast<char*>(param) + 0x274) != 0) {
        void* ptr = *reinterpret_cast<void**>(reinterpret_cast<char*>(param) + 0x2d8);
        if (ptr != 0) return ptr;
    }
    if (*reinterpret_cast<int*>(reinterpret_cast<char*>(param) + 0x2f4) != 0) {
        void* result = *reinterpret_cast<void**>(reinterpret_cast<char*>(param) + 0x358);
        if (result != 0) return result;
    }
    return 0;
}

bool alwaysFalse_1() { return false; }

void actCallVt34(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x34 / 4];
    func(self);
}

void actCallVt98(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x98 / 4];
    func(self);
}

void actCallVt88(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x88 / 4];
    func(self);
}

void actCallVt8C(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x8c / 4];
    func(self);
}

void actCallVt90(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x90 / 4];
    func(self);
}

void actCallVt94(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[148 / 4];
    func(self);
}

void actCallVt84_u16(){}

void actCallVt30(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[48 / 4];
    func(self);
}

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

int actParamThunk_3(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 3);
}

int actParamThunk_4(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 4);
}

int actParamThunk_5(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 5);
}

int actParamThunk_6(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 6);
}

int actParamThunk_7(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 7);
}

int actParamThunk_8(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 8);
}

int actParamThunk_9(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 9);
}

int actParamThunk_10(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 10);
}

int actParamThunk_11(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 11);
}

int actParamThunk_12(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 12);
}

int actParamThunk_13(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 13);
}

int actParamThunk_14(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 14);
}

int actParamThunk_15(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 15);
}

int actParamThunk_16(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 16);
}

int actParamThunk_17(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 17);
}

int actParamThunk_18(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 18);
}

int actParamThunk_19(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 19);
}

int actParamThunk_20(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 20);
}

int actParamThunk_21(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 21);
}

int actParamThunk_22(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 22);
}

int actParamThunk_23(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 23);
}

int actParamThunk_24(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 24);
}

int actParamThunk_25(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 25);
}

int actParamThunk_26(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 26);
}

int actParamThunk_27(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 27);
}

int actParamThunk_28(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 28);
}

int actParamThunk_29(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 29);
}

int actParamThunk_30(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 30);
}

int actParamThunk_31(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 31);
}

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
