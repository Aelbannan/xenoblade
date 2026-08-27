// Auto-scaffolded catalog TU for kyoshin/action/CActParamData
// Replace stubs with high-level C/C++ during decomp.

// func_800D9354 has a single shared decl on kyoshin/cf/CBattleManagerApi.hpp;
// CfGameManager.hpp carries no local copy.
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/action/CActParamData.hpp"

#include "libs/monolib/src/scn/CScnItemAnim.hpp"
#include "monolib/util/FixStr.hpp"
#include <stdarg.h>
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

int func_80055F24(CActParamData* self) {
    u8* block = reinterpret_cast<u8*>(self) + 0x260;
    if (*(u32*)(reinterpret_cast<u8*>(self) + 0x2D4) == 0)
        block = reinterpret_cast<u8*>(self) + 0x2E0;
    void* entry = *reinterpret_cast<void**>(block + 116);
    if (entry == NULL)
        return -1;
    return *reinterpret_cast<s16*>(static_cast<u8*>(entry) + 8);
}

int func_80055F54(CActParamData* self) {
    u8* block = reinterpret_cast<u8*>(self) + 0x260;
    if (*(u32*)(reinterpret_cast<u8*>(self) + 0x2D4) == 0)
        block = reinterpret_cast<u8*>(self) + 0x2E0;
    void* entry = *reinterpret_cast<void**>(block + 116);
    if (entry == NULL)
        return -1;
    return *reinterpret_cast<s16*>(static_cast<u8*>(entry) + 10);
}

u32 func_8005609C() {
    return 0;
}

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

void func_80054A20() {}

int func_80054A24(CActParamData* self, u32 sel) {
    if (sel != 0) {
        return *(u8*)(reinterpret_cast<u8*>(self) + 0x344);
    }
    return *(u8*)(reinterpret_cast<u8*>(self) + 0x2C4);
}

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

void func_800550D8() {}
void func_800550DC() {}
void func_800550E0() {}
void func_800550E4() {}

void processWithFlag24(){}

void func_800554D0() {}
void func_800554D4() {}
void func_800554D8() {}

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

extern "C" int func_80056290(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 3); }
extern "C" int func_800562A4(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 4); }
extern "C" int func_800562B8(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 5); }
extern "C" int func_800562CC(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 6); }
extern "C" int func_800562E0(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 7); }
extern "C" int func_800562F4(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 8); }
extern "C" int func_80056308(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 9); }
extern "C" int func_8005631C(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 10); }
extern "C" int func_80056330(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 11); }
extern "C" int func_80056344(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 12); }
extern "C" int func_80056358(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 13); }
extern "C" int func_8005636C(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 14); }
extern "C" int func_80056380(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 15); }
extern "C" int func_80056394(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 16); }
extern "C" int func_800563A8(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 17); }
extern "C" int func_800563D0(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 19); }
extern "C" int func_800563E4(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 20); }
extern "C" int func_800563F8(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 21); }
extern "C" int func_8005640C(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 22); }
extern "C" int func_80056420(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 23); }
extern "C" int func_80056434(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 24); }
extern "C" int func_80056448(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 25); }
extern "C" int func_8005645C(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 26); }
extern "C" int func_80056470(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 27); }
extern "C" int func_80056484(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 28); }
extern "C" int func_80056498(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 29); }
extern "C" int func_800564AC(void* self, int a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 30); }
extern "C" int func_800564C0(void* self, u32 a) { return ((CActParamDataVTableIf*)self)->vf7C(a, 31); }

// func_8005605C: sub-object selector: prefers the A-side pointer when the A
// gate byte and pointer are both live, otherwise falls back to the B side.
extern "C" void* func_8005605C(CActParamData* self) {
    if (self->mField274 != 0) {
        void* p = self->mPtr2D8;
        if (p != 0) {
            return p;
        }
    }
    if (self->mPtr2F4 != 0) {
        void* p = self->mPtr358;
        if (p != 0) {
            return p;
        }
    }
    return 0;
}

// func_800554DC: flush the pending tag/value pair of sub-object A (sel == 0)
// or B (sel != 0), then notify the callback object at 0x24 via its vt+0x28
// slot with the data object, the two vals words, and the func_80053960 tick.
extern "C" void func_800554DC(CActParamData* self, int sel) {
    // Word view of the selected 0x80-byte reset block (A at 0x260, B at 0x2E0).
    u32* blk = sel != 0 ? (u32*)&self->mField2E0 : (u32*)&self->mA260;
    if ((blk[0] & 0x80000) != 0) {
        return;
    }
    if (sel != 0) {
        // Rotate the shadowed value words at 0x268/0x26C.
        if (blk[3] == 0) {
            blk[3] = self->mA26C;
            self->mA26C = self->mA268;
        } else {
            blk[2] = blk[3];
            blk[3] = 0;
        }
        if (blk[3] == 0) {
            return;
        }
        if (blk[3] == 2 && blk[2] == 2) {
            blk[2] = 0;
        }
    } else {
        // Adopt the saved tag word at 0x2F0 once.
        if (self->mField2F0 != 0) {
            blk[2] = self->mField2F0;
            self->mField2F0 = 0;
        }
    }
    if (blk[5] == 0) {
        return;
    }
    if (self->mField24 == 0) {
        return;
    }
    u32 tick = func_80053960(static_cast<ActParamData388*>(self->mEntry04));
    reinterpret_cast<ActParamCb28*>(self->mField24)
        ->invoke28(static_cast<ActParamData388*>(self->mEntry04), blk[3], blk[5], blk[2],
                   tick);
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

// Flag-gated setters: when bit 2 of `flags` is set, store `val` into the
// destination field; always return 0.
int func_80056CE4(u32 flags, CActParamSet25C* dst, void* unused, u32 val) {
    if ((flags & 4) == 0) return 0;
    dst->field_0x25C = val;
    return 0;
}

u32 func_800567F0() {
    return 0;
}

u32 func_800567F8() {
    return 0;
}

u32 func_80056800() {
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
void ml::FixStr<16>::format(const char* fmt, ...) {
    char buffer[256];
    va_list va;
    va_start(va, fmt);
    vsnprintf(buffer, 256, fmt, va);
    va_end(va);
    mLength = std::strlen(buffer);
    std::strcpy(mString, buffer);
}

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
extern "C" int func_80054614(CActParamData* data, u32* out,
                               ActParamT1Src* rec, u8 initFlag, u32 gate);
u32 func_80054170(CActParamData* data, u32* out, u32 paramIdx, u8 initFlag,
                  u32 gate) {
    if (data->mByte216 == 0) {
        data->mByte216 = initFlag;
    }
    // Sticky enable byte OR bit15 of sub-object A's mode word.
    u32 modeWord = data->mA260;
    data->mByte216 = (data->mByte216 | ((modeWord >> 15) & 1)) != 0;

    // First three guards share one fail block in retail.
    if (data->mNode08 == NULL || paramIdx == 0 || paramIdx >= 0x68) {
        *out = 0xFFFFFFFF;
        return 0;
    }
    u32* entry = &data->mWords30[paramIdx];
    if (*entry == 0) {
        *out = 0xFFFFFFFF;
        return 0;
    }
    if ((modeWord & 0x400) != 0) {
        data->mByte216 = 1;
    }
    ActParamT1Src* rec = reinterpret_cast<ActParamT1Src*>(*entry);

    if ((rec->mField0C & 0x400) != 0 && gate == 0 && data->mByte216 == 0) {
        // Sub-object B pass.
        if (rec->mPtr08 != NULL) {
            data->mField2E8 = data->mField2EC;
            data->mField2EC = (u32)rec->mPtr08;
        }
        // Retail re-reads the entry for the walk call (the byte store
        // above blocks CSE of the load, but the slot address stays live).
        if (func_80054438(data, reinterpret_cast<ActParamT1Src*>(*entry),
                          1, 0) != 0) {
            data->mFloat330 = lbl_eu_80665F80;
            data->mFloat334 = lbl_eu_80665F80;
            data->mField338 = 0;
            *out = 0xFFFFFFFF;
            return 0;
        }
        // Locate the owning slot key whose node extent covers rec.
        // Retail walks a 4-byte cursor based at the object start, keeping
        // the 0x1F0 slot offset as a constant displacement.
        u32 res;
        ActParamT1Node* nd;
        s32 idx = 0;
        s32 cnt = data->mCount210;
        ActParamT1Node** cur = reinterpret_cast<ActParamT1Node**>(data);
        for (; idx < cnt; ++cur, ++idx) {
            nd = cur[0x7C];
            u32 sz = nd->mSize04;
            if ((u32)nd < (u32)rec && (u32)nd + sz > (u32)rec) {
                res = (u32)data->mKeys1D0[idx];
                goto foundB;
            }
        }
        res = 0;
    foundB:
        data->mField314 = res;
        func_80054614(data, out, rec, data->mByte216, gate);
        data->mFloat330 = lbl_eu_80665F80;
        data->mFloat334 = lbl_eu_80665F80;
        data->mField338 = 0;
        return 1;
    }
    // Sub-object A pass.
    if (rec->mPtr08 != NULL) {
        data->mA268 = data->mA26C;
        data->mA26C = (u32)rec->mPtr08;
    }
    if (func_80054438(data, reinterpret_cast<ActParamT1Src*>(*entry), 0,
                      0) != 0) {
        data->mA2B0 = lbl_eu_80665F80;
        data->mFloat2B4 = lbl_eu_80665F80;
        data->mA2B8 = 0;
        *out = 0xFFFFFFFF;
        return 0;
    }
    u32 res;
    ActParamT1Node* nd;
    s32 idx = 0;
    s32 cnt = data->mCount210;
    ActParamT1Node** cur = reinterpret_cast<ActParamT1Node**>(data);
    for (; idx < cnt; ++cur, ++idx) {
        nd = cur[0x7C];
        u32 sz = nd->mSize04;
        if ((u32)nd < (u32)rec && (u32)nd + sz > (u32)rec) {
            res = (u32)data->mKeys1D0[idx];
            goto foundA;
        }
    }
    res = 0;
foundA:
    data->mA294 = res;
    func_80054614(data, out, rec, data->mByte216, gate);
    data->mA2B0 = lbl_eu_80665F80;
    data->mFloat2B4 = lbl_eu_80665F80;
    data->mA2B8 = 0;
    return 0;
}

// Retail-shaped stamp variant used by func_80054438: loads the target's
// halfword before the scan, walks the node array through a running cursor,
// and derives the 0x8000 flag from a neg/or/srawi boolean of the scan index.
// Open-item packet (func_80054438 @ best 63 mismatches = 50 structural + 13
// reg_swap, size-exact 476B, 5 reloc-drift sites in the lis/stw/addi window):
// - Tried & ruled out this session: off-load hoisted above the global store
//   (scheduler still swaps stw/addi-r29); goto check/body/advance shared-tail
//   form (+8B, duplicated li r3,0 exits); while(true) early-return form (+8B);
//   handlers-local materialization before the store (reg swap r28/r29, worse);
//   indexed nodes[idx] do-while countdown (MWCC strength-reduces to pointer
//   walk, -16B); p-nodes idx derivation after loop (+8B); explicit
//   (-idx|idx)>>31 booleanize (-8B, MWCC folds it differently); down-counter
//   `left`/up-counter idx while-loop over mNodes1F0[idx] (reaches mtctr+guard
//   shape but MWCC still materializes base via addi r5,r30,0x1f0 then walks,
//   -16B); folded cur=data+*(u16*)data (identical output); if/else sub instead
//   of ternary (identical output); peeled do-while with trailing --cnt
//   condition (bdnz candidate, -16B/84).
// - Best ordering found: sub ternary -> off load -> cur -> global store LAST.
//   Store-last fixes the lhz placement (63 vs 64/66); remaining top-window diff
//   is only the stw/addi-r29 pair swap around the lbl_eu_80570788 LO reloc.
//   tbl-local hoisting between them swaps sub/table coloring (68) - ruled out.
// - Residual walls: (a) retail stamp scan uses mtctr/bdnz with separate idx
//   counter; unit-locked -O4 gives addic./bdnz pointer-walk only (wall #6);
//   (b) top-of-function scheduling of lhz/lis/stw/addi interleaving around the
//   lbl_eu_80663D50 sda21 store and lbl_eu_80570788 HA/LO split - reloc drift
//   sites 0x30-0x3e all live in this 4-instruction window; (c) mr-vs-or r5
//   forms and lwz r0 vs r3 count-load reg choice track the same coloring.
#define ACTPARAM_STAMP_SHORT_B(selfP, targetP)                                 \
    do {                                                                       \
        u32 cnt = (selfP)->mCount210;                                          \
        u32 idx = 0;                                                           \
        u16 oldVal = *reinterpret_cast<u16*>(                                  \
            reinterpret_cast<u8*>(targetP) + 4);                               \
        ActParamT1Node** nodes =                                               \
            reinterpret_cast<ActParamT1Node**>((selfP)->mNodes1F0);            \
        if (cnt > 0) {                                                         \
            ActParamT1Node* nd = nodes[0];                                     \
            while (!((u32)nd > (u32)(targetP)) &&                              \
                   !((u32)nd + nd->mSize04 > (u32)(targetP))) {                \
                ++nodes;                                                       \
                ++idx;                                                         \
                if (--cnt == 0) {                                              \
                    break;                                                     \
                }                                                              \
                nd = *nodes;                                                   \
            }                                                                  \
        }                                                                      \
        u32 msk = -(u32)(idx != 0);                                            \
        u32 bit = msk & 0x8000;                                                \
        u32 slot = (selfP)->mField258;                                         \
        (selfP)->mShorts218[slot] =                                            \
            (u16)(oldVal | (bit & 0xFFFF));                                    \
        u32 next = slot + 1;                                                   \
        (selfP)->mField258 = next;                                             \
        if (next >= 0x20)                                                      \
            (selfP)->mField258 = 0;                                            \
    } while (0)

int func_80054438(CActParamData* self, void* data, int sel, u32 gate) {
    // sel picks which sub-object receives the record walk.
    u8* sub;
    if (sel != 0) {
        sub = reinterpret_cast<u8*>(self->getResetBlock2E0());
    } else {
        sub = reinterpret_cast<u8*>(&self->mA260);
    }
    u16 off = *reinterpret_cast<u16*>(data);
    u8* cur = reinterpret_cast<u8*>(data) + off;
    // Register the sub-object in the global registration slot.
    *reinterpret_cast<u32*>(&lbl_eu_80663D50) = reinterpret_cast<u32>(sub);
    while (*reinterpret_cast<u16*>(cur + 2) > 1) {
        u16 type = *reinterpret_cast<u16*>(cur + 2);
        // gate==0 or a type-2/0x29 record avoids the early exit; only 0x29
        // records actually dispatch through the handler table.
        if (gate == 0 || type == 0x29 || type == 2) {
            if (type == 0x29 && lbl_eu_80570788[type](2, self, sub, cur) != 0) {
                ACTPARAM_STAMP_SHORT_B(self, cur);
                ACTPARAM_STAMP_SHORT_B(self, data);
                return 1;
            }
        }
        cur += *reinterpret_cast<u16*>(cur);
    }
    return 0;
}
extern "C" void func_80054D3C(CActParamData* self, ActParamD3CMember* member);
// Open-item packet (func_80054614 @ ~92%, size-exact 0x1c0, no reloc drift):
// - Fixed this session: proper types (CActParamData* / ActParamT1Src* params,
//   struct field access instead of raw u8* offsets) cut structural 112 -> 2.
// - Residual (35 = 33 reg_swap + 2 structural):
//   (a) Callee-saved color cycle rotated by one start position: retail ranks
//       sub=r28, data=r29, out=r30, rec=r31; ours rec=r28, sub=r29, data=r30,
//       out=r31 - SAME cycle (sub,data,out,rec), different assignment start.
//       Matches MWCC_CASES "Param-save birth order" OPEN wall (repo_proven,
//       no source lever). Tried & ruled out this session: decl order of sub
//       (top uninit / top init / mid), unified ret var, early short-lived
//       modeWord temp (no change), guard operand swap (regresses to 46),
//       single-shared-fail && chain (MWCC does NOT tail-dup here: 432B, 82
//       structural), single-exit if/else-if chain with unified result var
//       (compiles BYTE-IDENTICAL to the early-return form - branch shapes
//       are already canonical; do not retry either form).
//   (b) 2 branch-condition inversions (bc 4,2 vs bc 12,2) likely coupled to
//       the same coloring/layout pass.
// - Confirmed semantics: active==0 (NOT !=0) gates the bit10 sub-select;
//   retail fail blocks are DUPLICATED per guard (two separate early-return
//   guards, not one shared tail); sub uses li0/tests/li1 form, not ternary.
extern "C" int func_80054614(CActParamData* data, u32* out,
                               ActParamT1Src* rec, u8 initFlag, u32 gate) {
    if (data->mByte216 == 0) {
        data->mByte216 = initFlag;
    }
    // Effective enable state: sticky byte OR bit15 of the mode word at 0x260.
    int active = data->mByte216;
    active |= (data->mA260 & 0x8000) >> 15;
    active = (active != 0);
    data->mByte216 = (u8)active;

    if (rec == NULL || data->mNode08 == NULL) {
        *out = 0xFFFFFFFF;
        return 0;
    }
    if (rec->mType2 != 0) {
        *out = 0xFFFFFFFF;
        return 0;
    }
    // bit10 of the flags word selects the sub-object-B state block, but only
    // while the action is gated off and currently disabled.
    int sub = 0;
    if ((rec->mField0C & 0x400) != 0 && gate == 0 && active == 0) {
        sub = 1;
    }
    int result;
    if (func_80054438(data, rec, sub, 1) != 0) {
        if (sub != 0) {
            data->mFloat330 = lbl_eu_80665F80;
            data->mFloat334 = lbl_eu_80665F80;
            data->mField338 = 0;
        } else {
            data->mA2B0 = lbl_eu_80665F80;
            data->mFloat2B4 = lbl_eu_80665F80;
            data->mA2B8 = 0;
        }
        *out = 0xFFFFFFFF;
        result = 0;
    } else if (sub != 0) {
        // Sub-object B pass: a successful walk arms the byte214 flag.
        func_80054D3C(data, reinterpret_cast<ActParamD3CMember*>(
                                data->getResetBlock2E0()));
        int ret = (int)(intptr_t)func_800547D4(
            reinterpret_cast<ActParamT1Host*>(data),
            reinterpret_cast<ActParamT1Dst*>(data->getResetBlock2E0()), rec);
        *out = (u32)ret;
        if (ret >= 0) {
            data->mByte214 = 1;
        }
        result = 1;
    } else {
        // Sub-object A pass; on miss also reset B unless flag bit11 suppresses it.
        func_80054D3C(data, reinterpret_cast<ActParamD3CMember*>(&data->mA260));
        if ((rec->mField0C & 0x800) == 0) {
            func_80054D3C(data, reinterpret_cast<ActParamD3CMember*>(
                                    data->getResetBlock2E0()));
        }
        int ret2 = (int)(intptr_t)func_800547D4(
            reinterpret_cast<ActParamT1Host*>(data),
            reinterpret_cast<ActParamT1Dst*>(&data->mA260), rec);
        *out = (u32)ret2;
        data->mByte214 = 0;
        result = 0;
    }
    return result;
}

// Per-frame update of both sub-object float blocks: shift current into the
// shadow slot, accumulate the entry's threshold float, then recompute the
// crossed-threshold flag and streak counters.
void func_80053F7C(CActParamData* self, float arg1, float arg2) {
    // Sub-object A
    // References keep the store program-order (retail scheduling).
    float& prevA = self->mA2B0;
    float& curA = self->mFloat2B4;
    prevA = curA;
    self->mFloat2B4 = arg1;
    u32 countA = self->mA2BC + 1;
    ActParamData388* entryA = static_cast<ActParamData388*>(self->mEntry04);
    self->mA2BC = countA;
    float incA = entryA->mFloat388;
    self->mA2C8 = self->mA2C8 + incA;
    if (static_cast<ActParamObj10*>(self->mObj10)->check80(0)) {
        self->mA2C0 = self->mA2C0 + 1;
    }
    u8 fireA;
    if (self->mA260 & 0x4000) {
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
    float& prevB = self->mFloat330;
    float& curB = self->mFloat334;
    prevB = curB;
    self->mFloat334 = arg2;
    u32 countB = self->mField33C + 1;
    ActParamData388* entryB = static_cast<ActParamData388*>(self->mEntry04);
    self->mField33C = countB;
    float incB = entryB->mFloat388;
    self->mFloat348 = self->mFloat348 + incB;

    if (static_cast<ActParamObj10*>(self->mObj10)->check80(1)) {
        self->mField340 = self->mField340 + 1;
    }
    u8 fireB;
    if (self->mField2E0 & 0x4000) {
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
    // Zero the inline record area in 0x20-byte chunks. The cursor stays at
    // the object base with fixed offsets (retail shape).
    u32* cur = reinterpret_cast<u32*>(self);
    int n = 13;
    do {
        cur[0x0C] = 0;
        cur[0x0D] = 0;
        cur[0x0E] = 0;
        cur[0x0F] = 0;
        cur[0x10] = 0;
        cur[0x11] = 0;
        cur[0x12] = 0;
        cur[0x13] = 0;
        cur += 8;
    } while (--n);
    // Optional first node registration.
    self->mObj10 = obj;
    if (nodeA != 0 && nodeB != 0) {
        self->mNode0C = nodeB;
        self->mNode08 = reinterpret_cast<u8*>(nodeB) + 0x20;
        func_800555EC(reinterpret_cast<CActParamLinkTable*>(self),
                      reinterpret_cast<CScnItemAnim*>(nodeA),
                      static_cast<ActParamStackNode*>(nodeB));
        u32 cnt = self->mCount210;
        self->mKeys1D0[cnt] = nodeA;
        cnt = self->mCount210;
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
    // Retail shares one type-dispatch block between the loop back-edge and
    // the initial entry; express that shape directly.
    goto check;
body:
    if (rec->mSel08 == 0 && host->mIf24 != 0) {
        host->mIf24->vf0C(host->mField04, rec->mByte0A, rec->mByte0B);
    }
next:
    rec = advanceWalkRec(rec);
check:
    if (rec->mType2 > 1) {
        if (rec->mType2 == 9) {
            goto body;
        }
        goto next;
    }
}
// Stamp variant for func_80054A94: preloads the target's own halfword,
// scans the node array with a countdown counter plus a separate index
// (mtctr/bdnz + addi shape), and sets the 0x8000 flag iff the scan index
// is nonzero (-idx|idx)>>31 booleanize).
#define ACTPARAM_STAMP_SHORT_94(selfP, targetP)                                \
    do {                                                                       \
        u32 cnt = (selfP)->mCount210;                                          \
        s32 idx;                                                           \
        u16 oldVal =                                                           \
            *reinterpret_cast<u16*>(reinterpret_cast<u8*>(targetP) + 4);       \
        for (idx = 0; idx < cnt; idx++) {                                      \
            ActParamT1Node* nd =                                               \
                static_cast<ActParamT1Node*>((selfP)->mNodes1F0[idx]);       \
            if (!((u32)nd > (u32)(targetP)) &&                                 \
                (u32)nd + nd->mSize04 > (u32)(targetP)) {                      \
                break;                                                         \
            }                                                                  \
        }                                                                      \
        s32 sw = (s32)(-(s32)idx | (s32)idx) >> 31;                            \
        u32 slot = (selfP)->mField258;                                         \
        u32 next = slot + 1;                                                   \
        (selfP)->mShorts218[slot] =                                            \
            (u16)(oldVal | (sw & 0x8000));                                     \
        (selfP)->mField258 = next;                                             \
        if (next >= 0x20)                                                      \
            (selfP)->mField258 = 0;                                            \
    } while (0)

// sel flag: nonzero iff ctx IS this object's own sub-object-B reset block
// (retail tests with subf/cntlzw/srwi).
#define ACTPARAM_CTX_SEL(selfP, ctxP)                                          \
    (reinterpret_cast<u8*>(ctxP) == (selfP)->getResetBlock2E0())

// Walk the record chain with handler id 9, then run the ctx gate callbacks.
// Retail shares one type-check block between loop entry and back-edge (goto
// form) and keeps the type halfword live across the check into the dispatch.
int func_80054A94(CActParamData* self, void* ctx) {
    ActParamCtx94* rec = static_cast<ActParamCtx94*>(ctx);
    void* rec14 = rec->mRec14;
    if (rec14 == 0 || self->mObj10 == 0) {
        return 0;
    }
    // sel arg recomputed inline at each use (retail emits subf/cntlzw/srwi
    // at every site).
    if (func_80054438(self, rec14, ACTPARAM_CTX_SEL(self, ctx), 0) != 0) {
        return 1;
    }

    // Re-read mRec14 after the walk call (the opaque call blocks CSE, so
    // retail reloads the field here).
    u8* cur = reinterpret_cast<u8*>(rec->mRec14) +
              *reinterpret_cast<u16*>(rec->mRec14);
    u16 type;
    goto check;
body:
    // Every record type dispatches through the handler table; a nonzero
    // result registers the record and stops the walk.
    if (lbl_eu_80570788[type](9, self, ctx, cur) != 0) {
        ACTPARAM_STAMP_SHORT_94(self, cur);
        ACTPARAM_STAMP_SHORT_94(self, rec14);
        return 1;
    }
    cur += *reinterpret_cast<u16*>(cur);
check:
    type = *reinterpret_cast<u16*>(cur + 2);
    if (type > 1) {
        goto body;
    }

    // Gate callbacks: vt+0x1C with either context word (each site recomputes
    // the ctx-is-own-block flag), then vt+0xC4.
    if (rec->mField1C != 0) {
        if (static_cast<ActParamObj10*>(self->mObj10)->invoke1C(
                ACTPARAM_CTX_SEL(self, ctx), rec->mField1C)) {
            return 1;
        }
    } else if (rec->mField20 != 0) {
        if (static_cast<ActParamObj10*>(self->mObj10)->invoke1C(
                ACTPARAM_CTX_SEL(self, ctx), rec->mField20)) {
            return 1;
        }
    }
    if (rec->mField74 != 0) {
        if (static_cast<ActParamObj10*>(self->mObj10)->invokeC4()) {
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
// Walk the record chain hanging off member+0x6C. Dispatch by record type:
// 9 = threshold-crossing event (gated on bit 9 of the stream mode word) that
// fires vt+0x0C on the +0x24 callback object, 0xF = range-gated event firing
// vt+0x18, 0x14 = forwarded to func_800550E8. Retail shares one type-check
// block between loop entry and back-edge (goto form).
//
// Open-item packet (func_80054D3C @ best 200 mismatch / 155 structural /
// 45 reg_swap, 864B vs 924B):
// - Semantics fully reconstructed and verified against ASM (see handlers).
// - Fixed this session: union-based 0x4330 u16->double idiom with named
//   lbl_eu_80665F90 subtraction (plain (double)(u32) casts pool a TU-local
//   cookie and add f27/f28 saves); notify14 RETURNS the replacement time
//   (kept in f1 across the call) - fixing that dropped one callee-save fpr
//   and aligned the frame at 96B; func_800550E8 stub must be extern "C" +
//   noinline or MWCC inlines it away and the bl reloc vanishes.
// - Residual walls:
//   (a) Callee-save color rotation, BOTH banks by exactly one position:
//       gprs retail self=r28,member=r29,cursor=r31,const=r27 vs ours
//       self=r27,member=r28,cursor=r29,const=r31; floats retail
//       F80=f29,F78=f30,magic=f31 vs ours F80=f31,F78=f29,magic=f30.
//       Same func_80056A98-family allocator wall as func_80057490/
//       func_80054614 ("Param-save birth order"). Ruled out: explicit
//       long-lived convHi variable (identical output - MWCC rematerializes
//       lis), hoisting base out of the loop (needed, but does not fix
//       rotation), disjunction forms (a<b||a==b) for cror sites (+40B
//       regression), shared function-scope cvt union (frame regressed to
//       112B), exact retail temp topology (shared flag var across both
//       blocks + per-block hit temps + ok/count-flag var - matches li
//       pattern closely but does not shift the color start; 864B),
//       convHi declared as the FIRST statement of the function
//       (byte-identical output - constant color is invariant to birth
//       position; both sides keep it in a persistent reg, retail r27 vs
//       ours r31, so the ranking key is not source order).
//   (b) Conversion scratch lands at sp+16 vs retail sp+8 - an extra 8-byte
//       spill slot sits below it in ours; source lever unknown.
//   (c) ~92B size gap: retail is LARGER; likely per-site code retail
//      duplicates that MWCC CSEs in ours. Call to func_800550E8 lands at
//      +0x2dc vs retail +0x338 for the same reason (layout shift).
// - Next experiments: none known at this opt level; revisit with the
//   family-level allocator insight or finer-grained diff tooling.
extern "C" __declspec(noinline) void func_80054D3C(CActParamData* self,
                                        ActParamD3CMember* member) {
    // Magic high word for every u16->double conversion below; declared first
    // so its live range ranks ahead of the parameter spills.
    u32 convHi = 0x43300000;
    ActParamD3CStream* stream = member->mStream6C;
    if (stream == NULL || self->mObj10 == NULL) {
        return;
    }
    u32 mode = stream->mField0C;
    u16 off = stream->mHeadOff00;
    // 2^52 magic shared by every u16->double conversion below.
    double base = lbl_eu_80665F90;
    u32 bitSel = (mode >> 9) & 1;
    ActParamD3CRec* rec = reinterpret_cast<ActParamD3CRec*>(
        reinterpret_cast<u8*>(stream) + off);
    goto check;
body9:
    if (bitSel != 0) {
        // Convert the record's u16 to double via the 0x4330/2^52 idiom.
        union {
            u32 w[2];
            double d;
        } cvt;
        cvt.w[1] = rec->mShort08;
        cvt.w[0] = convHi;
        double val = cvt.d - base;
        if (lbl_eu_80665F80 == val) {
            // The callback returns the replacement time value.
            val = static_cast<ActParamObj5*>(self->mEntry04)->notify14();
        }
        int en = 1;
        int arm = 1;
        int t = 0;
        if (val <= member->mFloat54) {
            int h = 0;
            if (val > member->mFloat50) {
                if (static_cast<ActParamData388*>(self->mEntry04)->mFloat388 >
                    lbl_eu_80665F80) {
                    h = 1;
                }
            }
            if (h)
                t = 1;
        }
        if (t == 0) {
            int t2 = 0;
            if (val == member->mFloat54) {
                int h = 0;
                if (val == member->mFloat50) {
                    if (static_cast<ActParamData388*>(self->mEntry04)
                            ->mFloat388 > lbl_eu_80665F80) {
                        h = 1;
                    }
                }
                if (h)
                    t2 = 1;
            }
            int r6 = 0;
            if (t2 != 0) {
                if (member->mField58 < 1)
                    r6 = 1;
            }
            if (r6 == 0)
                arm = 0;
        }
        if (arm == 0) {
            if (!(val >= member->mFloat54))
                en = 0;
        }
        if (en == 0)
            goto advance;
        if (self->mField24 == 0)
            goto advance;
        reinterpret_cast<ActParamCbD3C*>(self->mField24)
            ->invoke0C(self->mEntry04, rec->mByte0A, rec->mByte0B);
    }
    goto advance;
bodyF:
    {
        union {
            u32 w[2];
            double d;
        } cvt;
        cvt.w[1] = rec->mShort08;
        cvt.w[0] = convHi;
        double val = cvt.d - base;
        if (lbl_eu_80665F80 == val) {
            // The callback returns the replacement time value.
            val = static_cast<ActParamObj5*>(self->mEntry04)->notify14();
        }
        int en;
        if (member->mFloat50 <= lbl_eu_80665F80 &&
            member->mFloat54 >= lbl_eu_80665F78) {
            en = 0;
        } else {
            en = 1;
            int flag = 0;
            int hit = 0;
            if (val <= member->mFloat54) {
                hit = 0;
                if (val > member->mFloat50) {
                    if (static_cast<ActParamData388*>(self->mEntry04)
                            ->mFloat388 > lbl_eu_80665F80) {
                        hit = 1;
                    }
                }
                if (hit)
                    flag = 1;
            }
            if (flag == 0) {
                flag = 0;
                int arm = 0;
                if (val == member->mFloat54) {
                    int h = 0;
                    if (val == member->mFloat50) {
                        if (static_cast<ActParamData388*>(self->mEntry04)
                                ->mFloat388 > lbl_eu_80665F80) {
                            h = 1;
                        }
                    }
                    if (h)
                        flag = 1;
                }
                if (flag != 0) {
                    if (member->mField58 < 1)
                        arm = 1;
                }
                if (arm == 0)
                    en = 0;
            }
        }
        if (en == 0) {
            // Retail reloads the short here for the secondary gate.
            u16 v = rec->mShort08;
            if (v != 0) {
                cvt.w[1] = v;
                cvt.w[0] = convHi;
                if (member->mFloat70 >= cvt.d - base)
                    goto advance;
            }
        }
        if (self->mField24 == 0)
            goto advance;
        reinterpret_cast<ActParamCbD3C*>(self->mField24)
            ->invoke18(self->mEntry04, rec);
    }
    goto advance;
body14:
    func_800550E8(self, member, reinterpret_cast<ActParamD3CTimeRec*>(rec), 1);
    goto advance;
advance:
    rec = reinterpret_cast<ActParamD3CRec*>(reinterpret_cast<u8*>(rec) +
                                            rec->mOffset0);
check:
    u16 type = rec->mType2;
    if (type <= 1)
        return;
    if (type == 9)
        goto body9;
    if (type == 0xF)
        goto bodyF;
    if (type == 0x14)
        goto body14;
    goto advance;
}
// func_80054D34: r4 = this+0x2E0, tail-branch to func_80054D3C (retail addi r4,r3,0x2e0; b)
void func_80054D34(void* self) {
    func_80054D3C(
        static_cast<CActParamData*>(self),
        reinterpret_cast<ActParamD3CMember*>(reinterpret_cast<u8*>(self) +
                                             0x2E0));
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
// Retail calls this out-of-line under an unmangled symbol; keep the stub
// opaque so callers emit the bl.
// func_800550E8: threshold-crossing dispatcher for the type-0x14 records.
// Phase A (gate == 0) converts the record's first time word and runs the
// range/edge tests; phase B always runs with the second time word and can
// additionally fire while the time is still past the +0x54 bound.
// Firing invokes the callback object at self+0x24 (vt+0x24) with the entry
// object and a phase selector; the function returns whether it fired.
//
// Open-item packet (func_800550E8 @ best 175 mismatch = 133 structural + 42
// reg_swap, frame-aligned 32B, no extra callee saves):
// - Fixed this session: proper typed signature (CActParamData* /
//   ActParamD3CMember* / ActParamD3CTimeRec*), notify14 result ASSIGNED to
//   the converted time value (t = notify14()) - that keeps t in f1 across
//   the bctrl like retail and kills the f31 spill; single shared cvt union
//   for both phases (frame 48 -> 32); goto noFire shared zero-return tail
//   for the both-zero fail path + phase-B fail (210 -> 175).
// - Ruled out: single-exit ret/goto form (adds _savegpr_27/_restgpr_27,
//   239/219); inverted (!(range)) phase-A shape (same regression);
//   hoisted `const double base = lbl_eu_80665F90` local (248 - shifts the
//   magic load above the both-zero block); split load/subtract
//   (`t = cvt.d; t = t - ...`) - byte-identical output, no effect.
// - Residual walls:
//   (a) Phase-A conversion register assignment: retail loads the 2^52 magic
//       into f2 and the stack value into f1 (fsubs f1,f1,f2); ours swaps the
//       two lfd assignments. MWCC-internal scheduling of the lis/stw/lfd
//       window around the union stores - wall #11 family.
//   (b) Retail rematerializes lfs lbl_eu_80665F80/F78 at every compare site;
//       ours CSEs the constant into a register across opaque calls.
//   (c) ~100B size gap, retail larger; shared-tail shapes partially closed
//       by goto noFire but branch-target offsets still shift downstream.
__declspec(noinline) int func_800550E8(CActParamData* self,
                                       ActParamD3CMember* member,
                                       ActParamD3CTimeRec* rec, int gate) {
    if (self->mField24 == 0) {
        return 0;
    }
    if (rec->mTime08 == 0 && rec->mTime0A == 0) {
        // No time data: choose between the two anim-side state queries.
        if (func_8004B8F8(self->mEntry04, 0)) {
            reinterpret_cast<ActParamCbSel24*>(self->mField24)->invoke24(
                self->mEntry04, 0);
            return 1;
        }
        if (func_8004B990(self->mEntry04, 0) == 0 && gate == 0) {
            // Shared no-fire exit with phase B below (retail .L_80055B14).
            goto noFire;
        }
        reinterpret_cast<ActParamCbSel24*>(self->mField24)->invoke24(
            self->mEntry04, 1);
        return 1;
    }

    // Shared 0x4330/2^52 conversion scratch reused by both phases.
    union {
        u32 w[2];
        double d;
    } cvt;
    int fire = 0;
    if (gate == 0) {
        // Phase A: convert time word 1.
        cvt.w[1] = rec->mTime08;
        cvt.w[0] = 0x43300000;
        double t = cvt.d - lbl_eu_80665F90;
        if (t == lbl_eu_80665F80) {
            // The callback returns the replacement time value.
            t = static_cast<ActParamObj5*>(self->mEntry04)->notify14();
        }
        if (member->mFloat50 <= lbl_eu_80665F80 &&
            member->mFloat54 >= lbl_eu_80665F78) {
            // Already inside the passive range: never fire phase A.
            fire = 0;
        } else {
            fire = 1;
            int hit = 0;
            if (t <= member->mFloat54) {
                if (t > member->mFloat50) {
                    if (static_cast<ActParamData388*>(self->mEntry04)
                            ->mFloat388 > lbl_eu_80665F80) {
                        hit = 1;
                    }
                }
            }
            if (hit == 0) {
                int g3 = 0;
                if (t == member->mFloat54) {
                    if (t == member->mFloat50) {
                        if (static_cast<ActParamData388*>(self->mEntry04)
                                ->mFloat388 > lbl_eu_80665F80) {
                            g3 = 1;
                        }
                    }
                }
                if (g3 != 0 && member->mField58 < 1) {
                    hit = 1;
                }
            }
            if (hit == 0) {
                fire = 0;
            }
        }
        if (fire != 0) {
            reinterpret_cast<ActParamCbSel24*>(self->mField24)->invoke24(
                self->mEntry04, 0);
            return 1;
        }
    }

    // Phase B: convert time word 2.
    cvt.w[1] = rec->mTime0A;
    cvt.w[0] = 0x43300000;
    double t2 = cvt.d - lbl_eu_80665F90;
    if (t2 == lbl_eu_80665F80) {
        // The callback returns the replacement time value.
        t2 = static_cast<ActParamObj5*>(self->mEntry04)->notify14();
    }
    fire = 1;
    int hit = 1;
    if (gate == 0 && member->mFloat50 <= lbl_eu_80665F80 &&
        member->mFloat54 >= lbl_eu_80665F78) {
        fire = 0;
    } else {
        int g2 = 0;
        if (t2 <= member->mFloat54) {
            if (t2 > member->mFloat50) {
                if (static_cast<ActParamData388*>(self->mEntry04)->mFloat388 >
                    lbl_eu_80665F80) {
                    g2 = 1;
                }
            }
        }
        if (g2 == 0) {
            int g3 = 0;
            if (t2 == member->mFloat54) {
                if (t2 == member->mFloat50) {
                    if (static_cast<ActParamData388*>(self->mEntry04)
                            ->mFloat388 > lbl_eu_80665F80) {
                        if (member->mField58 < 1) {
                            g3 = 1;
                        }
                    }
                }
            }
            if (g3 == 0) {
                hit = 0;
            }
        }
    }
    if (hit == 0) {
        // Extra gate: only an ungated pass still inside the upper bound fires.
        if (!(gate != 0 && t2 >= member->mFloat54)) {
            fire = 0;
        }
    }
    if (fire != 0) {
        reinterpret_cast<ActParamCbSel24*>(self->mField24)->invoke24(self->mEntry04,
                                                                    1);
        return 1;
    }
noFire:
    return 0;
}
// us-80057a94: if (flags & 8) call func_800550E8(a, b, c, 0); always return 0.
// Goto-gate keeps the call block out-of-line with li r3, 0 on both paths.
extern "C" int func_8005742C(u32 flags, void* a, void* b, void* c) {
    if (flags & 8) goto call;
    return 0;
call:
    func_800550E8(static_cast<CActParamData*>(a),
                  reinterpret_cast<ActParamD3CMember*>(b),
                  reinterpret_cast<ActParamD3CTimeRec*>(c), 0);
    return 0;
}
// Clear the stream's record chain: unlink each linkable record from its table
// slot and stop after the terminator record.
__declspec(noinline) void
func_80055700(CActParamLinkTable* table, int flag, CActParamRecStream* stream) {
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
// Register a stream node's record chain into the table slots. Each type-0
// record resolves its name tag against the anim resource, then claims its
// slot unless a type-2 record follows it. After the type-1 terminator, a
// first-time registration re-runs the payload swap over the fresh chain.
extern "C" __declspec(noinline) int
func_800555EC(CActParamLinkTable* table, CScnItemAnim* anim, ActParamStackNode* node) {
    if (node->mMode10 != 3) {
        return 0;
    }
    ActParamRegRec* recBase = reinterpret_cast<ActParamRegRec*>(&node->mBytes20);
    ActParamRegRec* rec = recBase;
    node->mWord14++;
    for (;;) {
        u16 type = rec->mType2;
        if (type == 0) {
            if ((s8)rec->mByte14 != 0) {
                rec->mField10 = func_8049E648(anim, reinterpret_cast<const char*>(&rec->mByte14));
            }
            u32 sel = rec->mSel8;
            if (sel != 0) {
                u32* slot = &table->mSlots[sel];
                ActParamRegRec* next =
                    reinterpret_cast<ActParamRegRec*>(reinterpret_cast<u8*>(rec) + rec->mOffset0);
                if (*slot == 0) {
                    *slot = reinterpret_cast<u32>(rec);
                } else if (next->mType2 != 2) {
                    *slot = reinterpret_cast<u32>(rec);
                }
            }
        } else if (type == 1) {
            break;
        }
        rec = reinterpret_cast<ActParamRegRec*>(reinterpret_cast<u8*>(rec) + rec->mOffset0);
    }
    if (node->mWord14 == 1) {
        node->mPtr08 = 0;
        node->mPtr0C = 0;
        func_800557E8(reinterpret_cast<ActParamStack*>(table),
                      reinterpret_cast<ActParamStrRec*>(recBase),
                      reinterpret_cast<ActParamStrRec*>(recBase));
    }
    if (node->mSelf18 == 0) {
        node->mSelf18 = node;
    }
    return 0;
}

// Push a node pair onto the stack slots, then splice b into the list after
// the last node whose payload differs from b's.
void func_80055AC4(ActParamStack* self, ActParamStackNode* a, ActParamStackNode* b) {
    func_800555EC(reinterpret_cast<CActParamLinkTable*>(self), reinterpret_cast<CScnItemAnim*>(a), b);
    self->mSlots1D0[self->mCount210] = a;
    // Post-increment inside the subscript: one load feeds both the address
    // and the deferred count store-back.
    self->mSlots1F0[self->mCount210++] = b;
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
// Open-item packet (func_80055B88 @ 122 mismatches = 96 structural + 26
// reg_swap, size 568 vs 560, +8B):
// - Fixed this session: retail only consumes the `key` arg (3rd param dead);
//   s32-countdown for-loops reproduce the retail mtctr/bdnz+guard shape for
//   BOTH counted loops (do-while `--count` gives bne-guard instead);
//   goto-based unlink walk removes the found-path null retest; explicit
//   headNext local is required (MWCC cannot hoist mNode0C->mNext0C across
//   the later calls); key used for both the mA294 compare and the
//   func_80055700 tag arg. Banked draft's 140 structural -> 96.
// - Residual: uniform volatile-color rotation. Retail colors {i:r7,
//   cursor:r6, count:r5(ctr), headNext:r0}; ours {i:r7, cursor:r5,
//   count:r0(ctr), headNext:r6}. Every scratch use shifts r5<->r0
//   accordingly. Decl-order variants tried (node,i,head,count,headNext /
//   head,count,headNext,node,i / split decl-assign / dropping the count
//   local / int-vs-u32 index / dropping the head local) only permute
//   i/headNext between r6/r7; cursor stays r5.
// - Also ruled out: while(cur && cur!=node)+post-loop unlink restructure
//   (92 structural but +16B, calls at +0x20); single-exit `result` merge of
//   the two zero-returns (over-merges, -24B, calls land BEFORE retail's);
//   the two separate early `return 0` blocks are required (retail shares one
//   li r3,0 tail block via bne from both the count-decrement miss and the
//   final tag-miss - MWCC will not reproduce that sharing from either form).
// - Ruled out: implicit-hoist form without headNext local (+20B, MWCC won't
//   hoist across calls); do-while countdown forms (bne guard, no mtctr);
//   post-loop unlink with `if (!cur)` test (extra found-path compare).
// - Next: find a source lever that delays headNext coloring until after
//   count (r0 assignment correlates with never-dereferenced values); or
//   locate the +8B pair (all front-section instruction counts verified
//   equal; bloat sits between the walk loop and the splice call block).
int func_80055B88(CActParamData* self, void* key, u32 /*tag*/) {
    // NOTE: retail only ever consumes the `key` argument (it doubles as the
    // stream tag); the third parameter is dead.
    ActParamNode94* head = static_cast<ActParamNode94*>(self->mNode0C);
    u32 count = self->mCount210;
    // Hoisted successor of the head, used to fix up the head slot after the
    // unlink (retail loads it before the search loop).
    ActParamNode94* headNext = head->mNext0C;
    ActParamNode94* node = 0;
    u32 i = 0;
    // Find the registered node whose key matches.
    for (s32 n = (s32)self->mCount210; n > 0; n--) {
        if (self->mKeys1D0[i] == key) {
            node = static_cast<ActParamNode94*>(self->mNodes1F0[i]);
            break;
        }
        i++;
    }
    if (node->mType10 != 3) {
        return 1;
    }
    if (--node->mCount14 != 0) {
        return 0;
    }

    // Unlink: walk from the head until the node is found.
    ActParamNode94* cur = head;
    do {
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
            u32 j = 0;
            for (s32 n = (s32)self->mCount210; n > 0; n--) {
                if (self->mKeys1D0[j] == key) {
                    // Condition re-reads the live count each iteration.
                    while (j < self->mCount210 - 1) {
                        self->mKeys1D0[j] = self->mKeys1D0[j + 1];
                        self->mNodes1F0[j] = self->mNodes1F0[j + 1];
                        j++;
                    }
                    self->mCount210 = self->mCount210 - 1;
                    break;
                }
                j++;
            }
            goto splice;
        }
        cur = cur->mNext0C;
    } while (cur != 0);
    return 0;

splice:


    // Splice: swap payloads with every other node along the chain.
    if (self->mNode0C == node) {
        self->mNode0C = headNext;
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

    func_80055700(reinterpret_cast<CActParamLinkTable*>(self),
                  (int)(uintptr_t)key,
                  reinterpret_cast<CActParamRecStream*>(node));
    // Re-register each remaining stream.
    it = static_cast<ActParamNode94*>(self->mNode0C);
    while (it != 0) {
        func_8005577C(reinterpret_cast<CActParamLinkTable*>(self),
                      reinterpret_cast<CActParamRecStream*>(it));
        ActParamNode94* nx = it->mNext0C;
        if (it == nx) {
            break;
        }
        it = nx;
    }

    if (self->mA294 == (u32)(uintptr_t)key) {
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

// Sub-object selector getter: pick block A (sel==0) or B, read its linked
// ref (+0x48) and current threshold float (+0x54), and gate the returned
// short on signed comparisons of the ref shorts against that threshold.
struct ActParamSelBlk {
    u8 _pad00[0x48];
    CActParamDataRef* mRef48;              // 0x48
    u8 _pad4C[0x54 - 0x4C];
    float mLimit54;                        // 0x54
};
void func_80055F84(CActParamData* self) {
    *(u32*)(reinterpret_cast<u8*>(self) + 0x2D4) = 0;
    *(u32*)(reinterpret_cast<u8*>(self) + 0x354) = 0;
}

int func_80055F94(CActParamData* self, int sel);
int func_80055F94(CActParamData* self, int sel) {
    ActParamSelBlk* blk;
    if (sel != 0) {
        blk = reinterpret_cast<ActParamSelBlk*>(self->getResetBlock2E0());
    } else {
        blk = reinterpret_cast<ActParamSelBlk*>(&self->mA260);
    }
    const CActParamDataRef* ref = blk->mRef48;
    if (ref == NULL) {
        return 0;
    }
    s16 v = ref->mShort08;
    if (v == 0) {
        return ref->mShort0C;
    }
    // Builtin s16 casts: MWCC emits the lis/xoris/stw/stw/lfd/fsubs magic
    // idiom per check (byte-identical to retail; see MWCC_CASES 7i). The
    // 2^52 magic constant pools to a TU-local cookie that the repo reloc
    // map canonicalizes to lbl_eu_80665F98.
    if ((float)v > blk->mLimit54) {
        return 0;
    }
    if (!((float)v <= blk->mLimit54)) {
        goto zero;
    }
    s16 w = ref->mShort0A;
    if (w != 0) {
        if (!((float)w > blk->mLimit54)) {
            goto zero;
        }
    } else {
        goto lb0;
    }
lb0:
    return ref->mShort0C;
zero:
    return 0;
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
// Bit-2-gated word store into dst+0x48; always returns 0.
int func_80056CC8(u32 flags, void* unused, CActParamBlock* dst, u32 val) {
    if ((flags & 4) == 0) return 0;
    dst->mField48 = val;
    return 0;
}
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
// ============================================================
// us-80057530: func_80056EC8
// Bit-3-gated threshold check (func_80056A98 variant): the range test here
// CLEARS the fire flag; a fired callback invokes the cb object's vt+0x18
// with the data object and the source record.
// ============================================================
int func_80056EC8(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src) {
    if ((flags & 8) == 0) {
        return 0;
    }
    int fire;
    float t = static_cast<float>(src->mShort08);
    if (lbl_eu_80665F80 == t) {
        // Retail keeps t live in f1 across this call: the callback RETURNS
        // the replacement time value.
        t = static_cast<ActParamObj5f*>(host->mObj04)->notify14();
    }
    if (vals->mFloat50 <= lbl_eu_80665F80) {
        if (vals->mFloat54 >= lbl_eu_80665F78) {
            // Range already satisfied: never fire.
            fire = 0;
            goto invoke;
        }
    }
    fire = 1;
    {
        int pass = 0;
        int ok = 0;
        if (t <= vals->mFloat54) {
            ok = (t > vals->mFloat50) &&
                 (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80);
            if (ok != 0) {
                pass = 1;
            }
        }
        if (pass != 0) {
            goto invoke;
        }
        // Edge case: t exactly on both bounds while the entry threshold
        // is crossed and the fire count is still zero.
        pass = 0;
        int edge = 0;
        if (t == vals->mFloat54) {
            int hit = (t == vals->mFloat50) &&
                      (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80);
            if (hit != 0) {
                pass = 1;
            }
        }
        if (pass != 0) {
            if (vals->mField58 < 1) {
                edge = 1;
            }
        }
        if (edge == 0) {
            fire = 0;
        }
    }
invoke:
    if (fire == 0) {
        // Retail re-reads src+0x08 through the volatile view here; the
        // load+compare survives even though both paths return 0.
        if (src->mShort08v == 0) {
            return 0;
        }
        return 0;
    } else {
        if (host->mCb24 != 0) {
            reinterpret_cast<ActParamCb18*>(host->mCb24)->invoke18(host->mObj04, src);
        }
    }
    return 0;
}

// ============================================================
// us-800576ec: func_80057084
// Bit-3-gated threshold check (func_80056A98 variant): on fire it does not
// invoke a callback object; instead the mask word at src+0x0C is OR-ed into
// (byte src+0x0A nonzero) or ANDC-ed out of the flag word at vals+0x00.
// ============================================================
int func_80057084(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src) {
    if ((flags & 8) == 0) {
        return 0;
    }
    if (vals->mField14 != 0) {
    float t = static_cast<float>(src->mShort08);
    if (lbl_eu_80665F80 == t) {
        // Retail keeps t live in f1 across this call: the callback RETURNS
        // the replacement time value.
        t = static_cast<ActParamObj5f*>(host->mObj04)->notify14();
    }
    int fire;
    if (vals->mFloat50 <= lbl_eu_80665F80 && vals->mFloat54 >= lbl_eu_80665F78) {
        // Range already satisfied: never fire.
        fire = 0;
        goto apply;
    }
    fire = 1;
    int pass = 0;
    if (t <= vals->mFloat54) {
        int ok = (t > vals->mFloat50) &&
                 (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80);
        if (ok != 0) {
            pass = 1;
        }
    }
    if (pass != 0) {
        goto apply;
    }
    // Edge case: t exactly on both bounds while the entry threshold
    // is crossed and the fire count is still zero.
    pass = 0;
    int edge = 0;
    if (t == vals->mFloat54) {
        int hit = (t == vals->mFloat50) &&
                  (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80);
        if (hit != 0) {
            pass = 1;
        }
    }
    if (pass != 0) {
        if (vals->mField58 < 1) {
            edge = 1;
        }
    }
    if (edge == 0) {
        fire = 0;
    }
apply:
    if (fire == 0) {
        return 0;
    }
    // Fired: apply src+0x0C as a set/clear mask on vals+0x00.
    u8 flagByte = src->mByte0A;
    u32 mask = src->mWord0C;
    if (flagByte != 0) {
        vals->mField00 |= mask;
    } else {
        vals->mField00 &= ~mask;
    }
    }
    return 0;
}

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
    // Declared first so it claims src's dead argument register (r6) at the
    // tail, matching retail's allocation.
    int cnt;
    if ((flags & 4) == 0) return 0;
    if (list->mCount2C >= 1) {
        return 0;
    }
    u32 value = src->mByte08;
    for (int i = 0; i < list->mCount2C; i++) {
        if (value == list->mEntries[i]) {
            return 0;
        }
    }
    cnt = static_cast<volatile CActParamByteList*>(list)->mCount2C;
    list->mEntries[cnt] = value;
    list->mCount2C = cnt + 1;
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
// us-80057af8: func_80057490
// Bit-3-gated threshold check (func_80056A98 variant): the vt+0x14 time
// callback fires when t==0 and RETURNS the replacement time value (kept in
// f1 across the call); on fire the cb object's vt+0x28 receives obj04, the
// src mask word, two vals words, and func_80053960(obj04) evaluated as the
// last argument. Goto form mirrors retail's shared li r3,0 fail tail.
//
// Plateau packet (best 22 mismatch / 6 structural / 16 reg-swap, size PASS
// 452B=452B, relocs clean 7=7): semantics fully reconstructed; the residual
// is the func_80056A98-family conversion-block wall shared with
// func_800568E8 (21 mism there) - MWCC assigns the 0x4330-magic f1/f2 pair
// and the fire/pass/edge int temps (r4/r5/r6/r0) one position rotated vs
// retail. Tried & ruled out this session: && bools (bool-normalize or-copies),
// nested ifs, per-block inner bool copied into pass, float t (adds frsp),
// interleaved named-const load (fixed lfd order; fsub-vs-fsubs remains),
// discarded-result notify14 (forces f31 spill). Next experiments: none known
// at this opt level; revisit only with a family-level allocator insight.
int func_80057490(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src) {
    int fire;
    if ((flags & 8) == 0) {
        return 0;
    }
    if (vals->mField14 == 0) {
        goto ret;
    }
    // The halfword time is widened once to double via the 0x4330
    // magic-high-word idiom; subtracting the NAMED 2^52 constant makes MWCC
    // reference retail's lbl_eu_80665F90 sdata2 pool entry.
    union {
        u32 w[2];
        double d;
    } cvt;
    // Halfword first, magic word second: retail's store order on the frame.
    // The named-const load is interleaved between the stores to mirror
    // retail's scheduling.
    cvt.w[1] = src->mShort08;
    double base = lbl_eu_80665F90;
    cvt.w[0] = 0x43300000;
    double t = cvt.d - base;
    if (lbl_eu_80665F80 == t) {
        // Retail keeps t live in f1 across this call: the callback RETURNS
        // the replacement time value.
        t = static_cast<ActParamObj5f*>(host->mObj04)->notify14();
    }
    if (vals->mFloat50 <= lbl_eu_80665F80 && vals->mFloat54 >= lbl_eu_80665F78) {
        // Range already satisfied: never fire.
        fire = 0;
        goto invoke;
    }
    fire = 1;
    {
        // Retail keeps a per-block inner bool and copies it into `pass`.
        int pass = 0;
        int inner = 0;
        if (t <= vals->mFloat54) {
            if (t > vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 >
                    lbl_eu_80665F80) {
                    inner = 1;
                }
            }
        }
        if (inner != 0) {
            pass = 1;
        }
        if (pass != 0) {
            goto invoke;
        }
        // Edge case: t exactly on both bounds while the entry threshold
        // is crossed and the fire count is still zero.
        pass = 0;
        int edge = 0;
        if (t == vals->mFloat54) {
            inner = 0;
            if (t == vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 >
                    lbl_eu_80665F80) {
                    inner = 1;
                }
            }
            if (inner != 0) {
                pass = 1;
            }
        }
        if (pass != 0) {
            if (vals->mField58 < 1) {
                edge = 1;
            }
        }
        if (edge == 0) {
            fire = 0;
        }
    }
invoke:
    if (fire == 0) {
        goto ret;
    }
    reinterpret_cast<ActParamCb28*>(host->mCb24)->invoke28(
        static_cast<ActParamData388*>(host->mObj04), src->mWord0C,
        vals->mField14, vals->mField0C,
        func_80053960(static_cast<ActParamData388*>(host->mObj04)));
ret:
    return 0;
}
// us-80057f04: func_8005789C
// Bit-3-gated threshold check (func_80056EC8 variant) additionally gated on
// vals+0x14: on fire it calls the callback object held at obj04+0x3a0 with a
// pointer to the src mask word (src+0x0C) and the normalized src byte flag.
//
// Open-item packet (best 68 mismatch / 57 structural / 440B vs 456B,
// reloc drift 5): semantics fully reconstructed. Residual shares the
// func_80056A98-family conversion-block wall (see func_80057490 packet):
// MWCC assigns the 0x4330-magic f1/f2 pair and the fire/pass/edge int temps
// one position rotated vs retail.
// Session findings: two separate early-return ifs made MWCC DROP the
// mField14 check entirely (-8B); merging via || recovers it (72->68 with
// retzero split). Retail shape = flags-fail inline li+b, mField14 beq to a
// shared tail also serving fire==0/cb==null (goto retzero form matches).
// Plain (float) cast pools 2^52 to a TU-local cookie (@118xx name drift);
// the named-const subtraction forces lbl_eu_80665F90 but adds frsp+fsub.
// Ruled out: && bool forms (+16B), union composite shared with callback arg
// (arg is &src+0x0C), cvt.w[1]-sourced cast (duplicate buffer).
int func_8005789C(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src) {
    if ((flags & 8) == 0) {
        // Flags-gate failure keeps its own inline zero-return.
        return 0;
    }
    if (vals->mField14 == 0) {
        goto retzero;
    }
    int fire;
    // The halfword is widened to a single-precision float via the standard
    // 0x4330-magic uint->float idiom.
    float t = static_cast<float>(src->mShort08);
    if (lbl_eu_80665F80 == t) {
        // Retail keeps t live in f1 across this call: the callback RETURNS
        // the replacement time value.
        t = static_cast<ActParamObj5f*>(host->mObj04)->notify14();
    }
    if (vals->mFloat50 <= lbl_eu_80665F80 && vals->mFloat54 >= lbl_eu_80665F78) {
        // Range already satisfied: never fire.
        fire = 0;
        goto invoke;
    }
    fire = 1;
    {
        int pass = 0;
        if (t <= vals->mFloat54) {
            if (t > vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 >
                    lbl_eu_80665F80) {
                    pass = 1;
                }
            }
        }
        if (pass != 0) {
            goto invoke;
        }
        // Edge case: t exactly on both bounds while the entry threshold
        // is crossed and the fire count is still zero.
        pass = 0;
        int edge = 0;
        if (t == vals->mFloat54) {
            int hit = 0;
            if (t == vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 >
                    lbl_eu_80665F80) {
                    hit = 1;
                }
            }
            if (hit != 0) {
                pass = 1;
            }
        }
        if (pass != 0) {
            if (vals->mField58 < 1) {
                edge = 1;
            }
        }
        if (edge == 0) {
            fire = 0;
        }
    }
invoke:
    if (fire != 0) {
        ActParamCb28Time* cb =
            static_cast<ActParamCb28Time*>(static_cast<ActParamData3A0*>(host->mObj04)->mCb3A0);
        if (cb != NULL) {
            cb->invoke28(&src->mWord0C, src->mByte0A != 0);
        }
    }
retzero:
    return 0;
}

// us-80058344: func_80057CDC
// One-time initialization of the two global handler dispatch tables used by
// the record-chain walkers. Unimplemented slot ranges default to the generic
// handlers func_80056800 (table1) / func_8005609C (table2).
// Defined further down in this TU.
int func_80056A98(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src);
int func_80056D00(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src);
int func_80057280(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src);
int func_800568E8(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src);

void CActParamData::func_80057CDC() {
    ActParamTbl1Fn* t1 = (ActParamTbl1Fn*)lbl_eu_80570788;
    t1[0] = func_800567F0;
    t1[1] = func_800567F8;
    t1[2] = func_80056800;
    t1[3] = (ActParamTbl1Fn)func_80056808;
    t1[4] = func_80056800;
    t1[5] = (ActParamTbl1Fn)func_80056828;
    t1[6] = (ActParamTbl1Fn)func_80056888;
    t1[7] = (ActParamTbl1Fn)func_800568A8;
    t1[8] = (ActParamTbl1Fn)func_800568C8;
    t1[9] = (ActParamTbl1Fn)func_800568E8;
    t1[10] = (ActParamTbl1Fn)func_80056A98;
    t1[11] = (ActParamTbl1Fn)func_80056C34;
    t1[12] = (ActParamTbl1Fn)func_80056C54;
    t1[13] = (ActParamTbl1Fn)func_80056D00;
    t1[14] = (ActParamTbl1Fn)func_80056EAC;
    t1[15] = (ActParamTbl1Fn)func_80056EC8;
    t1[16] = (ActParamTbl1Fn)func_80057084;
    t1[17] = (ActParamTbl1Fn)func_80057244;
    t1[18] = (ActParamTbl1Fn)func_80057264;
    t1[19] = (ActParamTbl1Fn)func_80057280;
    t1[20] = (ActParamTbl1Fn)func_8005742C;

    ActParamTbl19Fn* t2 = (ActParamTbl19Fn*)lbl_eu_805705F0;
    t2[0] = func_8005609C;
    t2[1] = (ActParamTbl19Fn)func_80056138;
    t2[2] = (ActParamTbl19Fn)func_80056128;
    t2[3] = (ActParamTbl19Fn)func_80056158;
    t2[4] = (ActParamTbl19Fn)func_80056188;
    t2[5] = (ActParamTbl19Fn)func_80056168;
    t2[6] = (ActParamTbl19Fn)func_80056178;
    t2[7] = (ActParamTbl19Fn)func_80056118;
    t2[8] = (ActParamTbl19Fn)func_80056148;
    t2[9] = func_8005609C;
    t2[10] = func_8005609C;
    t2[11] = (ActParamTbl19Fn)func_80056198;
    t2[12] = (ActParamTbl19Fn)func_800560A4;
    t2[13] = (ActParamTbl19Fn)func_800560C4;
    t2[14] = (ActParamTbl19Fn)func_800560D4;
    t2[15] = (ActParamTbl19Fn)func_800560E4;
    t2[16] = (ActParamTbl19Fn)func_800560F4;
    t2[17] = (ActParamTbl19Fn)func_800561A8;
    t2[18] = (ActParamTbl19Fn)func_800561D8;
    t2[19] = (ActParamTbl19Fn)func_800561C8;
    t2[20] = (ActParamTbl19Fn)func_800561B8;
    t2[21] = (ActParamTbl19Fn)func_800561E8;
    t2[22] = (ActParamTbl19Fn)func_80056238;
    t2[23] = (ActParamTbl19Fn)func_80056248;
    t2[24] = (ActParamTbl19Fn)func_80056268;
    t2[25] = (ActParamTbl19Fn)func_8005627C;
    t2[26] = (ActParamTbl19Fn)func_80056290;
    t2[27] = (ActParamTbl19Fn)func_800562A4;
    t2[28] = (ActParamTbl19Fn)func_800562B8;
    t2[29] = (ActParamTbl19Fn)func_800562CC;
    t2[30] = (ActParamTbl19Fn)func_800562E0;
    t2[31] = (ActParamTbl19Fn)func_800562F4;
    t2[32] = (ActParamTbl19Fn)func_80056308;
    t2[33] = (ActParamTbl19Fn)func_8005631C;
    t2[34] = (ActParamTbl19Fn)func_80056330;
    t2[35] = (ActParamTbl19Fn)func_80056344;
    t2[36] = (ActParamTbl19Fn)func_80056358;
    t2[37] = (ActParamTbl19Fn)func_8005636C;
    t2[38] = (ActParamTbl19Fn)func_80056380;
    t2[39] = (ActParamTbl19Fn)func_80056394;
    t2[40] = (ActParamTbl19Fn)func_800563A8;
    t2[41] = (ActParamTbl19Fn)func_800563BC;
    t2[42] = (ActParamTbl19Fn)func_800563D0;
    t2[43] = (ActParamTbl19Fn)func_800563E4;
    t2[44] = (ActParamTbl19Fn)func_800563F8;
    t2[45] = (ActParamTbl19Fn)func_8005640C;
    t2[46] = (ActParamTbl19Fn)func_80056420;
    t2[47] = (ActParamTbl19Fn)func_80056434;
    t2[48] = (ActParamTbl19Fn)func_80056448;
    t2[49] = (ActParamTbl19Fn)func_8005645C;
    t2[50] = (ActParamTbl19Fn)func_80056470;
    t2[51] = (ActParamTbl19Fn)func_80056484;
    t2[52] = (ActParamTbl19Fn)func_80056498;
    t2[53] = (ActParamTbl19Fn)func_800564AC;
    t2[54] = (ActParamTbl19Fn)func_800564C0;
    t2[55] = (ActParamTbl19Fn)func_800564D4;
    t2[56] = (ActParamTbl19Fn)func_800564E8;
    t2[57] = (ActParamTbl19Fn)func_800564FC;
    t2[58] = (ActParamTbl19Fn)func_80056510;
    t2[59] = (ActParamTbl19Fn)func_80056524;
    t2[60] = (ActParamTbl19Fn)func_80056538;
    t2[61] = (ActParamTbl19Fn)func_8005654C;
    t2[62] = (ActParamTbl19Fn)func_80056560;
    t2[63] = (ActParamTbl19Fn)func_80056574;
    t2[64] = (ActParamTbl19Fn)func_80056588;
    t2[65] = (ActParamTbl19Fn)func_8005659C;
    t2[66] = (ActParamTbl19Fn)func_800565B0;
    t2[67] = (ActParamTbl19Fn)func_800565C4;
    t2[68] = (ActParamTbl19Fn)func_800565D8;
    t2[69] = (ActParamTbl19Fn)func_800565EC;
    t2[70] = (ActParamTbl19Fn)func_80056600;
    t2[71] = (ActParamTbl19Fn)func_80056104;
    t2[72] = (ActParamTbl19Fn)func_800560B4;
    t2[73] = (ActParamTbl19Fn)func_80056614;
    t2[74] = (ActParamTbl19Fn)func_80056624;
    t2[75] = (ActParamTbl19Fn)func_80056638;
    t2[76] = (ActParamTbl19Fn)func_8005664C;
    t2[77] = (ActParamTbl19Fn)func_80056660;
    t2[78] = (ActParamTbl19Fn)func_80056670;
    t2[79] = (ActParamTbl19Fn)func_80056208;
    t2[80] = (ActParamTbl19Fn)func_80056218;
    t2[81] = (ActParamTbl19Fn)func_80056228;
    t2[82] = (ActParamTbl19Fn)func_800561F8;
    t2[83] = (ActParamTbl19Fn)func_80056708;
    t2[84] = (ActParamTbl19Fn)func_8005671C;
    t2[85] = (ActParamTbl19Fn)func_80056680;
    t2[86] = (ActParamTbl19Fn)func_80056694;
    t2[87] = (ActParamTbl19Fn)func_800566A8;
    t2[88] = (ActParamTbl19Fn)func_800566BC;
    t2[89] = (ActParamTbl19Fn)func_800566D0;
    t2[90] = (ActParamTbl19Fn)func_800566E4;
    t2[91] = (ActParamTbl19Fn)func_800566F8;
    t2[92] = (ActParamTbl19Fn)func_80056730;
    t2[93] = (ActParamTbl19Fn)func_80056760;
    t2[94] = (ActParamTbl19Fn)func_80056258;
    t2[95] = (ActParamTbl19Fn)func_80056790;
    t2[96] = (ActParamTbl19Fn)func_800567A0;
    t2[97] = (ActParamTbl19Fn)func_800567B0;
    t2[98] = (ActParamTbl19Fn)func_800567C0;
    t2[99] = (ActParamTbl19Fn)func_800567D0;
    t2[100] = (ActParamTbl19Fn)func_800567E0;
}


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
// dispatch through lbl_eu_805705F0[(u16)lo - 0x2A] with
// (a->mField10, c->mField24, hi). Semantics verified against retail asm;
// byte-match of the conversion blocks still open (see attempts.jsonl).
// ============================================================
int func_80057BA0(u32 flags, ActParamT19ArgA* a, ActParamT19ArgB* b, ActParamT19ArgC* c) {
    if ((flags & 2) == 0) {
        return 0;
    }
    u16 x = c->mShort28;
    if (x == 0) {
        if (c->mShort2A != 0) {
            // Upper index only: pass when t reaches it.
            if (b->mFloat54 >= c->mShort2A) {
                goto dispatch;
            }
            goto ret0;
        }
        goto dispatch;
    }
    // x != 0
    if (c->mShort2A != 0) {
        goto window;
    }
    // Lower index only: pass when t reaches it.
    if (b->mFloat54 >= x) {
        goto dispatch;
    }
    goto ret0;
window:
    // Both indices set: t must reach the lower index but stay below the
    // upper one.
    if (b->mFloat54 < x) {
        goto ret0;
    }
    if (b->mFloat54 >= c->mShort2A) {
        goto ret0;
    }
    goto dispatch;
ret0:
    return 0;
dispatch:
    u32 packed = c->mField08;
    return lbl_eu_805705F0[(u16)packed - 0x2A](a->mField10, c->mField24,
                                               packed >> 16)
               ? 1
               : 0;
}// us-80057d24: func_80057A64
// Window variant of func_80057BA0: bounds are the halfword pair at c+0x10 /
// c+0x12, flag bit 0 gates, and the dispatch passes (vals->mField10,
// c->mField0C, hi) into lbl_eu_805705F0[(u16)lo - 0x2A].
struct ActParamT19ArgD {
    u8 _pad00[8];
    u32 mWord08;
    u32 mField0C;
    u16 mShort10;
    u16 mShort12;
};
int func_80057A64(u32 flags, ActParamT19ArgA* a, ActParamT19ArgB* b,
                  ActParamT19ArgD* c) {
    if ((flags & 1) == 0) {
        return 0;
    }
    u16 x = c->mShort10;
    if (x == 0) {
        // x == 0
        if (c->mShort12 != 0) {
            // y only: pass when t reaches it.
            if (b->mFloat54 >= c->mShort12) {
                goto dispatch;
            }
            goto ret0;
        }
        goto dispatch;
    }
    // x != 0
    if (c->mShort12 != 0) {
        goto window;
    }
    // x only: pass when t reaches it.
    if (b->mFloat54 >= x) {
        goto dispatch;
    }
    goto ret0;
window:
    // Both indices set: t must reach the lower index but stay below the
    // upper one.
    if (b->mFloat54 < x) {
        goto ret0;
    }
    if (b->mFloat54 >= c->mShort12) {
        goto ret0;
    }
    goto dispatch;
ret0:
    return 0;
dispatch:
    u32 packed = c->mWord08;
    return lbl_eu_805705F0[(u16)packed - 0x2A](a->mField10, c->mField0C,
                                               packed >> 16)
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
    int fire;
    // Head: convert + float-compare before the call like retail; the value
    // dies at the compare so no nonvolatile spill is needed.
    if (actSrc5Time(src) == lbl_eu_80665F80) {
        static_cast<ActParamObj5*>(host->mObj04)->notify14();
    }
    // Volatile view blocks CSE with the head conversion, so this copy is
    // recomputed after the call and stays in a volatile FP register.
    float t = static_cast<float>(src->mShort08v);
    if (vals->mFloat50 <= lbl_eu_80665F80 && vals->mFloat54 >= lbl_eu_80665F78) {
        // Range already satisfied: never fire.
        fire = 0;
    } else {
        fire = 1;
        int g2;
        if (t > vals->mFloat54) {
            g2 = 1;
        } else {
            int r5 = 0;
            if (t > vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                    r5 = 1;
                }
            }
            g2 = r5;
        }
        if (g2 == 0) {
            int g3 = 0;
            if (t == vals->mFloat54) {
                int r6 = 0;
                if (t == vals->mFloat50) {
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
    }
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
    // Head check converts independently; the working copy below is computed
    // after the call so its lifetime never spans the bctrl (a local that
    // spans the call makes MWCC spill it to a nonvolatile FPR, unlike retail).
    if (actSrc5Time(src) == lbl_eu_80665F80) {
        static_cast<ActParamObj5*>(host->mObj04)->notify14();
    }
    float t = static_cast<float>(src->mShort08);
    // fire defaults to 1; only the range/edge tests below can clear it.
    int fire = 1;
    if (vals->mFloat50 <= lbl_eu_80665F80 && vals->mFloat54 >= lbl_eu_80665F78) {
        // Already inside the passive range: never fire.
        fire = 0;
    } else {
        int hit = 0;
        if (t <= vals->mFloat54) {
            if (t > vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 >
                    lbl_eu_80665F80) {
                    hit = 1;
                }
            }
        }
        if (hit == 0) {
            hit = 0;
            if (t == vals->mFloat54) {
                if (t == vals->mFloat50) {
                    if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 >
                        lbl_eu_80665F80) {
                        hit = 1;
                    }
                }
            }
            if (hit != 0) {
                if (vals->mField58 >= 1) {
                    fire = 0;
                }
            } else {
                fire = 0;
            }
        }
    }
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
    // The halfword time is widened once to double via the 0x4330
    // magic-high-word idiom; subtracting the NAMED 2^52 constant makes MWCC
    // reference retail's lbl_eu_80665F90 sdata2 pool entry.
    union {
        u32 w[2];
        double d;
    } cvt;
    // Halfword first, magic word second: retail's store order on the frame.
    cvt.w[1] = src->mShort08;
    cvt.w[0] = 0x43300000;
    double t = cvt.d - lbl_eu_80665F90;
    if (lbl_eu_80665F80 == t) {
        // notify14 returns the time as a double, so t stays live in f1
        // across the call (retail reuses the ABI return register).
        t = static_cast<ActParamObj5*>(host->mObj04)->notify14();
    }
    int fire;
    int flag;
    int both;
    int edge;
    if (vals->mFloat50 <= lbl_eu_80665F80) {
        if (vals->mFloat54 >= lbl_eu_80665F78) {
            // Range already satisfied: never fire.
            fire = 0;
            goto invoke;
        }
    }
    fire = 1;
    edge = 0;
    // Negated spelling: only this form lowers a mixed double/float compare
    // with retail's fcmpo + cror eq,lt,eq merge.
    if (!(t <= vals->mFloat54)) {
        flag = 0;
        if (t > vals->mFloat50) {
            if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                flag = 1;
            }
        }
        if (flag != 0) {
            edge = 1;
        }
    }
    if (edge != 0) {
        goto invoke;
    }
    {
        edge = 0;
        flag = 0;
        if (t == vals->mFloat54) {
            both = 0;
            if (t == vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 > lbl_eu_80665F80) {
                    both = 1;
                }
            }
            if (both != 0) {
                edge = 1;
            }
        }
        if (edge != 0) {
            if (vals->mField58 < 1) {
                flag = 1;
            }
        }
        if (flag == 0) {
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
        // Shared zero-return lives at the very end so MWCC branches forward
        // to it (retail beq-to-END shape) instead of duplicating a local
        // return block here.
        goto zeroret;
    }
    // Halfword widened to single-precision float via the standard
    // 0x4330-magic uint->float idiom.
    float t = static_cast<float>(src->mShort08);
    if (lbl_eu_80665F80 == t) {
        // notify14 returns the time as a float in f1, keeping t live across
        // the call (retail reuses the ABI return register).
        t = static_cast<ActParamObj5f*>(host->mObj04)->notify14();
    }
    int fire;
    if (vals->mFloat50 <= lbl_eu_80665F80) {
        if (vals->mFloat54 >= lbl_eu_80665F78) {
            // Already inside the passive range: never fire.
            fire = 0;
            goto invoke;
        }
    }
    fire = 1;
    {
        int edge = 0;
        int tmp;
        if (t <= vals->mFloat54) {
            // Inside the window: a dip below 50 with threshold crossed
            // fires immediately.
            tmp = 0;
            if (t > vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 >
                    lbl_eu_80665F80) {
                    tmp = 1;
                }
            }
        }
        if (tmp != 0) {
            edge = 1;
        }
        if (edge != 0) {
            goto invoke;
        }
    }
    {
        int keep = 0;
        int edge = 0;
        int hit = 0;
        if (t == vals->mFloat54) {
            if (t == vals->mFloat50) {
                if (static_cast<ActParamData388*>(host->mObj04)->mFloat388 >
                    lbl_eu_80665F80) {
                    hit = 1;
                }
            }
            if (hit != 0) {
                edge = 1;
            }
        }
        if (edge != 0) {
            if (vals->mField58 < 1) {
                keep = 1;
            }
        }
        if (keep == 0) {
            fire = 0;
        }
    }
invoke:
    if (fire != 0) {
        if (host->mCb24 != 0) {
            reinterpret_cast<ActParamCb20*>(host->mCb24)->invoke20(host->mObj04, src);
        }
    }
zeroret:
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


extern "C" __declspec(noinline) u32 func_80053960(ActParamData388* obj) {
    const u8* p = reinterpret_cast<const u8*>(obj);
    u32 v4C4 = *(const u32*)(p + 0x4C4);
    if (v4C4 != 0) {
        return v4C4;
    }
    const void* p08 = *(const void* const*)(p + 0x08);
    if (p08 != 0) {
        return *(const u32*)(reinterpret_cast<const u8*>(p08) + 0x18);
    }
    return *(const u32*)(p + 0x4B4);
}

extern "C" void* func_800547D4(ActParamT1Host* host, ActParamT1Dst* dst, ActParamT1Src* src) {
    if (src->mPtr08 != 0) {
        dst->mField0C = reinterpret_cast<u32>(src->mPtr08);
    }
    // Inherit fields from the previous entry in the chain.
    ActParamT1Src* prev = dst->mPtr14;
    if (prev != 0) {
        dst->mField04 = prev->mField0C;
    }
    u8 byte30 = dst->mByte30;
    u32 old38 = dst->mField38;
    float f28 = dst->mFloat28;
    const ActParamTbl1Fn* tbl = lbl_eu_80570788;
    float f40 = dst->mFloat40;
    u32 field0C = src->mField0C;
    dst->mPtr18 = dst->mPtr14;
    dst->mPtr14 = src;
    dst->mField1C = 0;
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
        tbl[rec->mType2](4, host, dst, rec);
        rec = nextT1Rec(rec);
    }

    // Find the slot whose node extent [p, p+size) contains src.
    int found = 0;
    u32 count = host->mCount210;
    for (u32 i = 0; i < count; i++) {
        ActParamT1Node* p = host->mSlots1F0[i];
        u32 size = p->mSize04;
        if (reinterpret_cast<u32>(p) >= reinterpret_cast<u32>(src)) {
            continue;
        }
        if (reinterpret_cast<u32>(p) + size <= reinterpret_cast<u32>(src)) {
            continue;
        }
        found = reinterpret_cast<u32>(host->mSlots1D0[i]);
        break;
    }
    dst->mField34 = found;
    host->mByte216 = 0;
    return host->mObj10;
}
