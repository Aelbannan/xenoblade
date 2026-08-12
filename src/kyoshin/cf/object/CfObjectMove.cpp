// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"

struct OMIfShift { char pad[0x10]; };
struct OMIf : OMIfShift {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void vf0014();
};

typedef void (*VFn)(void*);
// Cast-only SI iface for the +0xB0/+0x10 double-hop thunks.
// Shift base puts the vptr at object+0x10 (retail lwz r12, 0x10(r3));
// RTTI omit keeps slots at vtable+0x08, +0x04 each.
struct Shift { char pad[0x10]; };
struct ObjVtIf : Shift {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void vf14();
    virtual void vf18(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void vf28(); virtual void vf2C(); virtual void vf30(); virtual void vf34();
    virtual void _v038(); virtual void _v03C(); virtual void vf40(); virtual void _v044();
    virtual void vf48(); virtual void _v04C(); virtual void _v050(); virtual void vf54();
    virtual void vf58(); virtual void _v05C(); virtual void vf60(); virtual void _v064();
    virtual void vf68();
};


namespace cf {

CfObjectMove::CfObjectMove() {}

cf::CfObjectMove::~CfObjectMove() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc4() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc7() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc6() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc1() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc2() {}

void CfObjectMove_nullsub_1() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc2() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc1() {}

void CfObjectMove_nullsub_2() {}

void CfObjectMove_nullsub_3(){}

void cf::CfObjectMove::setMoveSpeed(float value)
{
    mMoveSpeed = value;
    unsigned int flags = mFlags68;
    flags = (flags & ~0x400u) | 0x800u;
    mFlags68 = flags;
}

void CfObjectMove_nullsub_4(){}

void CfObjectMove_nullsub_5(){}

f32 lbl_eu_80666A88;

void cf::CfObjectMove::resetMoveSpeed() {
    u32 flags = mFlags68;
    f32 value = lbl_eu_80666A88;
    mMoveSpeed = value;
    mFlags68 = flags & ~0xC00;
}

void CfObjectMove_nullsub_6(){}

void CfObjectMove_nullsub_7(){}

void CfObjectMove_nullsub_8(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc5() {}

void CfObjectMove_nullsub_9(){}

void CfObjectMove_nullsub_10(){}

void CfObjectMove_nullsub_11(){}

void CfObjectMove_nullsub_12(){}

void CfObjectMove_nullsub_13(){}

void CfObjectMove_nullsub_14(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc46() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc47() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc49() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc64(int flag) {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc65() {}

void func_804B0B54(void*, void*);

void cf::CfObjectMove::updatePos() {
    if ((unk64 & 8) != 0) {
        func_804B0B54(_60C_region, (void*)((u8*)this + 0x3C));
    }
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc19() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc22() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc25() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc26(u32 value, float amount) {}

u32 cf::CfObjectMove::CfObject_UnkVirtualFunc23() { return 0; }

void cf::CfObjectMove::CfObject_UnkVirtualFunc27(void* src) {}

void CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(CfObjectModel*);
void func_8004B4A4(void*, float);

void cf::CfObjectMove::CfObject_UnkVirtualFunc29(float value) {
    CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(this);
    void* target = mTargetC4;
    if (target != 0 && ((mFlags68 & 0x4) != 0)) {
        func_8004B4A4(target, value);
    }
    target = mTarget6C0;
    if (target != 0) {
        *(float*)((char*)target + 0xC) = value;
    }
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc30() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc32() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc33(float amount) {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc13() {}

extern "C" void* func_800BE0B0(CfObjectMove* self) { return &self->mSubObj54; }

void cf::CfObjectMove::CfObject_UnkVirtualFunc57() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc4() {}

void* cf::CfObjectMove::CObjectParam_UnkVirtualFunc2() { return cf::CfObjectModel::CObjectParam_UnkVirtualFunc2(); }

void CfObjectMove_nullsub_15(){}

void CfObjectMove_nullsub_16(){}

void CfObjectMove_nullsub_17(){}

// Open item: three null-checked stores of `value` at +0x388. MWCC reuses r3
// (this) for the THIRD target load/store (this is dead after); retail keeps r4
// for all three (3 reg_swap, 0 structural). Witness rejects the r4<->r3 ABI
// permutation. Separated locals + (void)this invariant.
void cf::CfObjectMove::CfObject_UnkVirtualFunc14(float value) {
    void* target = this->mTargetC4;
    if (target != 0) {
        *(float*)((char*)target + 0x388) = value;
    }
    target = this->mTargetC8;
    if (target != 0) {
        *(float*)((char*)target + 0x388) = value;
    }
    target = this->mTargetCC;
    if (target != 0) {
        *(float*)((char*)target + 0x388) = value;
    }
}

extern "C" void CfObjectMove_UnkVirtualFunc11__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, u16 v) {
    void* o = *(void**)((u8*)self + 0x6c0);
    if (o) *(u16*)((u8*)o + 0xde) = v;
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc7() {
    // Dispatch to the NPC movement-target handler (tail call, r3 only).
    cf::CtrlNpc* target = (cf::CtrlNpc*)mTarget6C0;
    if (target != 0) {
        func_800948F8(target);
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc8() {
    cf::CtrlNpc* target = (cf::CtrlNpc*)mTarget6C0;
    if (target != 0) {
        func_80094CE8(target);
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc12() {
    cf::CtrlNpc* target = (cf::CtrlNpc*)mTarget6C0;
    if (target != 0) {
        func_80094DF4(target);
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc13() {
    cf::CtrlNpc* target = (cf::CtrlNpc*)mTarget6C0;
    if (target != 0) {
        func_80094E44(target);
    }
}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc14() {}

void CfObjectMove_nullsub_19(){}

void CfObjectMove_nullsub_20(){}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc16() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc17() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc18() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc21() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc22() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc19() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc20() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc18() {}

void CfObjectMove_nullsub_21(){}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc23() {}

void cf::CfObject::func_800BE898(int, u32, float, float) {}

void CfObjectMove_nullsub_22(){}

int cf::CfObjectMove::getSubState() {
    struct SubObj { int dummy0[11]; int field_2c; int field_30; };
    SubObj* sub = (SubObj*)mSubObj38;
    if (!sub) return -1;
    if (sub->field_2c < 0) return -1;
    return sub->field_30;
}

void cf::CfObjectMove::freeSub()
{
    extern void func_802A1304(void*);
    char* sub = (char*)mSubObj38;
    if (sub != 0) {
        func_802A1304(sub + 0x28);
    }
}

void CfObjectMove_nullsub_23(){}

void cf::CfObjectMove::setSubFieldC(unsigned short val) {
    void* ptr = mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xc) = val;
}

int cf::CfObjectMove::getSubFieldA() { return *(s16*)((char*)mSubObjB0 + 10); }

void cf::CfObjectMove::setSubFieldA(unsigned short val) {
    void* ptr = mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xa) = val;
}

int cf::CfObjectMove::getSubFieldE() { return *(s16*)((char*)mSubObjB0 + 14); }

void cf::CfObjectMove::setSubFieldE(unsigned short val) {
    void* ptr = mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xe) = val;
}

extern "C" void CfObject_UnkVirtualFunc9__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf14();
}

extern "C" void CfObject_UnkVirtualFunc10__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf18();
}

void cf::CfObjectMove::virtCall10(){
    void** inner = (void**)mPtr10;
    void (*func)(void*) = (void (*)(void*))inner[0x14 / 4];
    func(this);
}

extern "C" void CfObject_UnkVirtualFunc61__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf30();
}

extern "C" void CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf34();
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc12() {}

void CfObjectMove_nullsub_24() {}

int cf::CfObjectMove::nullsub_25() { return 0; }

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc3() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc66(int) {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc19() {}

extern "C" void* CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf48();
    return NULL;
}

int cf::CfObjectMove::nullsub_26() { return 0; }

void cf::CfObjectMove::CfObject_UnkVirtualFunc37() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc38() {}

int cf::CfObjectMove::nullsub_27() { return 0; }

int cf::CfObjectMove::nullsub_28() { return 0; }

void CfObjectMove_nullsub_29(){}

void CfObjectMove_nullsub_30(){}

void CfObjectMove_nullsub_31(){}

void CfObjectMove_nullsub_32(){}

void CfObjectMove_nullsub_33(){}

void CfObjectMove_nullsub_34() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc39() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc40() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc42() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc43() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc45() {}

void CfObjectMove_nullsub_35(){}

void CfObjectMove_nullsub_36(){}

void CfObjectMove_nullsub_37() {}

void CfObjectMove_nullsub_38() {}

void CfObjectMove_nullsub_39(){}

void CfObjectMove_nullsub_40(){}

void CfObjectMove_nullsub_41() {}

void CfObjectMove_nullsub_42(){}

void func_eu_800BFC78() {}

extern "C" void func_800BEE1C(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf40();
}
extern "C" void func_800BF29C(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf54();
}
extern "C" void func_800BF2B0(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf58();
}
extern "C" void func_800BF2CC(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf60();
}
extern "C" void func_800BF2E0(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf28();
}
extern "C" void func_800BF2F8(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf2C();
}
extern "C" void func_eu_800BFC7C(void* self) {
    reinterpret_cast<ObjVtIf*>(*(void**)((u8*)self + 0xb0))->vf68();
}
extern "C" void func_800BC4B8(void* self, float v) {
    extern float lbl_eu_80666A90;
    void* o = *(void**)((u8*)self + 0xb0);
    *(float*)((u8*)o + 4) = lbl_eu_80666A90 * v;
}


int cf::CfObjectMove::isActive() { return 1; }

void cf::CfObjectMove::setBit6c9(unsigned long bit) {
    unsigned char val = mFlags6C9;
    mFlags6C9 = __rlwimi(val, bit, 0, 31, 31);
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc70(float value) {}

cf::CfObjectMove* cf::CfObjectMove::testFlag8() {
    if (this != NULL && (unk64 & 8) != 0) {
        return this;
    }
    return NULL;
}

} // namespace cf

extern "C" void func_800BE9AC(void* self) { reinterpret_cast<OMIf*>(self)->vf0014(); }

extern "C" void func_800BC3B0() {}
extern "C" void func_800BC3F0() {}
extern "C" void func_800BC458() {}
extern "C" void func_800BC4CC() {}
extern "C" void func_800BC68C() {}
extern "C" void func_800BC8D8() {}
extern "C" void func_800BC9EC() {}
extern "C" void func_800BCD04() {}
extern "C" void func_800BCFA0() {}
// retail: lhz r0,0x1678(r3); extrwi r3,r0,1,28 = ((u16 at +0x1678)>>3)&1
extern "C" u32 func_800BD638(void* self) { return (*(const u16*)((const char*)self + 0x1678) >> 3) & 1; }
extern "C" void func_800BD644() {}
extern "C" void func_800BE0F8() {}
extern "C" void func_800BE12C() {}

// retail: lwz r3,0xB0(r3); lhz r3,0xC(r3); blr
extern "C" u16 func_800BE93C(void* self) { return *(u16*)((u8*)*(void**)((u8*)self + 0xB0) + 0xC); }
extern "C" void func_800BE1A4() {}
void func_800BE28C(cf::CfObjectMove* self) {
    cf::CtrlNpc* target = (cf::CtrlNpc*)self->mTarget6C0;
    if (target != 0) {
        func_80094FC8(target);
    }
}
extern "C" void func_800BE33C() {}
extern "C" void func_800BE3E8() {}
extern "C" void func_800BE824() {}
extern "C" void func_800BE8B4() {}
extern "C" void func_800BEA34() {}
extern "C" bool func_800BEA38() { return false; }
extern "C" void func_800BED6C() {}
extern "C" void func_800BED80() {}
extern "C" void func_800BEDC4() {}
extern "C" void func_800BEE08() {}
extern "C" void func_800BEE30() {}
extern "C" void func_800BF2F4() {}
