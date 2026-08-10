// Auto-scaffolded catalog TU for monolib/src/scn/CScnEffectActNw4r
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnEffectActNw4r.hpp"

extern void func_80495E60();
void func_8049BEA4(u8* self){ func_80495E60(); }

extern void func_80484E5C();
void func_8049BEAC(u8* self){ func_80484E5C(); }

extern void func_804838DC();
void func_8049BEB4(u8* self){ func_804838DC(); }

extern void func_8048315C();
void func_8049BEE4(u8* self){ func_8048315C(); }

extern "C" void func_8049BEEC(u8* self, const void* other) {
    u8* dst = *(u8**)((u8*)self + 4);
    *(u32*)(dst + 0x304) = *(u32*)((u8*)other + 0);
    *(u32*)(dst + 0x308) = *(u32*)((u8*)other + 4);
    *(u32*)(dst + 0x30C) = *(u32*)((u8*)other + 8);
}

void func_8049BF0C(){}

void func_8049BF34(){}

void func_8049BF5C(){}

void func_8049BF84(){}

// Effect-act lookup: the local id table first, then the manager's chain.
CScnEffectAct* func_8049C314(CScnEffectActNw4r* self, u32 idx) {
    if (self->mCount > idx) {
        return self->mActs[idx];
    }
    CScnEffectAct* chain = self->mpMgr->field_0x7c4;
    if (chain != 0) {
        return chain->v40()->v18(idx);
    }
    return 0;
}

// Same lookup as func_8049C314, but gated on the manager's act flag and
// preferring the manager's direct act slot before falling back to the chain.
CScnEffectAct* func_8049BFAC(CScnEffectActNw4r* self, u32 idx) {
    if ((self->mpMgr->field_0x7a4 & 0x40000000) == 0) {
        return 0;
    }
    if (func_80490AF4(self->mpMgr, idx)) {
        return self->mpMgr->v13(idx);
    }
    CScnEffectAct* chain = self->mpMgr->field_0x7c4;
    if (chain != 0) {
        return chain->v40()->v13(idx);
    }
    return 0;
}

// Id-based lookup that scans the u16 id table before falling back to the
// manager's chain.
CScnEffectAct* func_8049C18C(CScnEffectActNw4r* self, u32 idx) {
    CScnEffectActMgr* mgr = self->mpMgr;
    if ((mgr->field_0x7a4 & 0x40000000) == 0) {
        return 0;
    }
    for (u32 i = 0; i < self->mCount; i++) {
        if (self->mIds[i] == idx) {
            return self->mActs[i];
        }
    }
    CScnEffectAct* chain = mgr->field_0x7c4;
    if (chain != 0) {
        return chain->v40()->v15(idx);
    }
    return 0;
}

// Id-based lookup that also copies the act's position triplet into out;
// returns 1 on success, 0 when the fallback chain reports none.
u32 func_8049C244(CScnEffectActNw4r* self, u32 idx, ml::CVec3* out) {
    u32 count = self->mCount;
    for (u32 i = 0; i < count; i++) {
        if (self->mIds[i] == idx) {
            CScnEffectAct* act = self->mActs[i];
            float z = act->field_0x2c.x;
            float y = act->field_0x1c.x;
            float x = act->field_0xc.x;
            out->x = x;
            out->y = y;
            out->z = z;
            return 1;
        }
    }
    CScnEffectAct* chain = self->mpMgr->field_0x7c4;
    if (chain != 0) {
        return chain->v40()->v16(idx, out);
    }
    return 0;
}

// Fills out with the act's position triplet; zero when the effect acts are
// disabled or absent.
void func_8049C060(ml::CVec3* out, CScnEffectActNw4r* self, u32 idx) {
    CScnEffectActMgr* mgr = self->mpMgr;
    if ((mgr->field_0x7a4 & 0x40000000) == 0) {
        out->x = ml::CVec3::zero.x;
        out->y = ml::CVec3::zero.y;
        out->z = ml::CVec3::zero.z;
        return;
    }
    if (func_80490AF4(mgr, idx)) {
        CScnEffectAct* act = mgr->v13(idx);
        out->x = act->field_0xc.x;
        out->y = act->field_0x1c.x;
        out->z = act->field_0x2c.x;
        return;
    }
    CScnEffectAct* chain = mgr->field_0x7c4;
    if (chain != 0) {
        CScnEffectAct* act = chain->v40()->v13(idx);
        out->x = act->field_0xc.x;
        out->y = act->field_0x1c.x;
        out->z = act->field_0x2c.x;
        return;
    }
    out->x = ml::CVec3::zero.x;
    out->y = ml::CVec3::zero.y;
    out->z = ml::CVec3::zero.z;
}

extern void func_80482AB8();
void func_8049C394(u8* self){ func_80482AB8(); }

extern void func_80482AD4();
void func_8049C39C(u8* self){ func_80482AD4(); }

extern void func_8048856C();
void func_8049C3A4(u8* self){ func_8048856C(); }

void func_8049C3AC(){}

extern "C" u32 func_8049C420(u8* self) { return (*(u32*)((char*)*(void**)((u8*)self + 4) + 0x7A4) >> 30) & 1; }

extern "C" u32 func_8049C430(u8* self) { return *(u32*)((u8*)self + 0x188); }

// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).
struct VTarget {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0;
    virtual void v33() = 0;
    virtual void v34() = 0;
    virtual void v35() = 0;
    virtual void v36() = 0;
    virtual void v37() = 0;
    virtual void v38() = 0;
    virtual void v39() = 0;
    virtual void v40() = 0;
    virtual void v41() = 0;
    virtual void v42() = 0;
    virtual void v43() = 0;
    virtual ~VTarget() {}
};

extern "C" void func_8049BA44(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 4);
    obj->v4();
}
extern "C" void func_8049BEBC(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 4);
    obj->v43();
}
extern "C" void func_8049BED0(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 4);
    obj->v16();
}

extern "C" void func_8049B9EC() {}
extern "C" void func_8049BA58() {}
