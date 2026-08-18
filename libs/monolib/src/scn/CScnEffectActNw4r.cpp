// Auto-scaffolded catalog TU for monolib/src/scn/CScnEffectActNw4r
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>
#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include "libs/monolib/src/scn/CScnEffectActNw4r.hpp"

// Constructor: installs the vtable and clears the act-table count.
extern "C" u32 lbl_eu_8056EAD8[];
CScnEffectActNw4r::CScnEffectActNw4r() : mCount(0) { *(void**)this = (void*)lbl_eu_8056EAD8; }

CScnEffectActNw4r::~CScnEffectActNw4r() { *(void**)this = (void*)lbl_eu_8056EAD8; }

// vtable stub thunks (retail: lwz r3,0x4(r3); b callee) - callees declared
// extern "C" so the relocs carry the retail unmangled names.
extern "C" bool func_80495E60(u8* self);
extern "C" void func_80484E5C(u8* self);
extern "C" void func_804838DC(u8* self);
extern "C" void func_80482AB8(u8* self);
extern "C" void func_8049BEA4(u8* self) { func_80495E60(*(u8**)(self + 4)); }

extern "C" void func_8049BEAC(u8* self) { func_80484E5C(*(u8**)(self + 4)); }

extern "C" void func_8049BEB4(u8* self) { func_804838DC(*(u8**)(self + 4)); }

extern "C" u8* func_8049BEE4(CScnEffectActNw4r* self) { return func_8048315C(self->mpMgr); }

extern "C" void func_8049BEEC(u8* self, const void* other) {
    u8* dst = *(u8**)((u8*)self + 4);
    *(u32*)(dst + 0x304) = *(u32*)((u8*)other + 0);
    *(u32*)(dst + 0x308) = *(u32*)((u8*)other + 4);
    *(u32*)(dst + 0x30C) = *(u32*)((u8*)other + 8);
}

// Manager's effect-act getter, act slot at +0x48.
// const self: MWCC hoists the mpMgr load above the LR-save store (retail
// prologue order lwz r3,4(r3) BEFORE stw r0,20(sp)); non-const emits stw-first.
extern "C" u8* func_8049BF0C(const CScnEffectActNw4r* self) {
    return func_8048315C(self->mpMgr) + 0x48;
}

// Manager's effect-act getter, act slot at +0x78.
extern "C" u8* func_8049BF34(const CScnEffectActNw4r* self) {
    return func_8048315C(self->mpMgr) + 0x78;
}

// Manager's effect-act getter, act slot at +0xb8.
extern "C" u8* func_8049BF5C(const CScnEffectActNw4r* self) {
    return func_8048315C(self->mpMgr) + 0xb8;
}

// Manager's effect-act getter, act slot at +0xc4.
extern "C" u8* func_8049BF84(const CScnEffectActNw4r* self) {
    return func_8048315C(self->mpMgr) + 0xc4;
}

// Effect-act lookup: the local id table first, then the manager's chain.
CScnEffectAct* func_8049C314(const CScnEffectActNw4r* self, u32 idx) {
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
CScnEffectAct* func_8049BFAC(const CScnEffectActNw4r* self, u32 idx) {
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
CScnEffectAct* func_8049C18C(const CScnEffectActNw4r* self, u32 idx) {
    if ((self->mpMgr->field_0x7a4 & 0x40000000) == 0) {
        return 0;
    }
    for (u32 i = 0; i < self->mCount; i++) {
        if (self->mIds[i] == idx) {
            return self->mActs[i];
        }
    }
    CScnEffectAct* chain = self->mpMgr->field_0x7c4;
    if (chain != 0) {
        return chain->v40()->v15(idx);
    }
    return 0;
}

// Id-based lookup that also copies the act's position triplet into out;
// returns 1 on success, 0 when the fallback chain reports none.
u32 func_8049C244(const CScnEffectActNw4r* self, u32 idx, ml::CVec3* out) {
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
void func_8049C060(ml::CVec3* out, const CScnEffectActNw4r* self, u32 idx) {
    if ((self->mpMgr->field_0x7a4 & 0x40000000) == 0) {
        out->x = ml::CVec3::zero.x;
        out->y = ml::CVec3::zero.y;
        out->z = ml::CVec3::zero.z;
        return;
    }
    if (func_80490AF4(self->mpMgr, idx)) {
        CScnEffectAct* act = self->mpMgr->v13(idx);
        float z = act->field_0x2c.x;
        float y = act->field_0x1c.x;
        float x = act->field_0xc.x;
        out->x = x;
        out->y = y;
        out->z = z;
        return;
    }
    CScnEffectAct* chain = self->mpMgr->field_0x7c4;
    if (chain != 0) {
        CScnEffectAct* act = chain->v40()->v13(idx);
        float z = act->field_0x2c.x;
        float y = act->field_0x1c.x;
        float x = act->field_0xc.x;
        out->x = x;
        out->y = y;
        out->z = z;
        return;
    }
    out->x = ml::CVec3::zero.x;
    out->y = ml::CVec3::zero.y;
    out->z = ml::CVec3::zero.z;
}

extern "C" void func_8049C394(u8* self) { func_80482AB8(*(u8**)(self + 4)); }

extern void func_80482AD4(CScnEffectActMgr* mgr);
void func_8049C39C(CScnEffectActNw4r* self) {
    func_80482AD4(self->mpMgr);
}

extern void func_8048856C(CScnEffectActMgr* mgr);
void func_8049C3A4(CScnEffectActNw4r* self) {
    func_8048856C(self->mpMgr);
}

// Effect-range gate: the act is usable while the manager's range value sits
// inside [AB74 - A208, AB74 + A208], unless the manager's act flag disables
// the whole check (then always usable). Returns 1 usable / 0 outside range.
u32 func_8049C3AC(CScnEffectActNw4r* self) {
    CScnEffectActMgr* mgr = self->mpMgr;
    u32 result = 1;
    u32 flag = 1;
    if ((mgr->field_0x7a4 & 0x20000000) == 0) {
        if (mgr->field_0x304 < lbl_eu_8066AB74 + lbl_eu_8066A208) {
            flag = 0;
        }
    }
    if (flag != 0) {
        return result;
    }
    if (mgr->field_0x304 <= lbl_eu_8066AB74 - lbl_eu_8066A208) {
        return result;
    }
    result = 0;
    return result;
}

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

// Node-name string of a ResNode with the retail header's validity assert:
// ptr + name offset (NULL when the offset is absent). Only used for the digit
// scan loop, where the two checks must share one fail tail (the `||` form);
// the other sites inline the assert so MWCC merges it with surrounding checks.
static inline const char* nodeName(const nw4r::g3d::ResNode* node) {
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_80663A34);
    }
    return node->ofs_to_ptr<char>(node->ref().name);
}

// Recursively register every node whose name is "<tag><NNN>" (three digits at
// offsets 2..4) into the effect-act table: the node's mtx id selects the act
// slot (0x30-byte entries from mgr->field_0x147c->field_0xec), and the digits
// parse into the u16 id stored next to the act pointer. Then descend into the
// node's child and next sibling.
void func_8049BA58(CScnEffectActNw4r* self, nw4r::g3d::ResNode* node) {
    if (node->ptr() == NULL) {
        return;
    }
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_80663A34);
    }
    const char* n1 = node->ofs_to_ptr<char>(node->ref().name);
    if (strlen(n1) < 5) {
        goto end;
    }
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_80663A34);
    }
    const char* n2 = node->ofs_to_ptr<char>(node->ref().name);
    if (memcmp(n2, lbl_eu_80663A18, strlen(lbl_eu_80663A18)) != 0) {
        goto end;
    }
    u32 flag = 1;
    for (u32 i = 2; i < 5; i++) {
        if (!node->IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_80663A34);
        }
        if (node->ofs_to_ptr<char>(node->ref().name)[i] < '0') {
            goto fail;
        }
        if (!node->IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_80663A34);
        }
        // Both digit checks share one fail tail: `<= '9'` continues, anything
        // else falls into the shared flag-clear block (retail merges them).
        if (node->ofs_to_ptr<char>(node->ref().name)[i] <= '9') {
            continue;
        }
    fail:
        flag = 0;
        break;
    }
    if (flag == 0) {
        goto end;
    }
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
    }
    u32 mtxId = node->GetMtxID();
    self->mActs[self->mCount] = (CScnEffectAct*)(self->mpMgr->field_0x147c->field_0xec + mtxId * 0x30);
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_80663A34);
    }
    const char* p = node->ofs_to_ptr<char>(node->ref().name);
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_80663A34);
    }
    const char* q = node->ofs_to_ptr<char>(node->ref().name);
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                        lbl_eu_80663910, lbl_eu_80663A34);
    }
    u16 v = (q[2] - '0') * 100 + (node->ofs_to_ptr<char>(node->ref().name)[3] - '0') * 10 + (p[4] - '0');
    self->mIds[self->mCount] = v;
    self->mCount++;
end:
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                        lbl_eu_80663910, lbl_eu_80663A30);
    }
    nw4r::g3d::ResNode child = node->GetChildNode();
    if ((u32)child.ptr() & 3) {
        nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        child = nw4r::g3d::ResNode(NULL);
    }
    func_8049BA58(self, &child);
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                        lbl_eu_80663910, lbl_eu_80663A30);
    }
    nw4r::g3d::ResNode next = node->GetNextSibling();
    if ((u32)next.ptr() & 3) {
        nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        next = nw4r::g3d::ResNode(NULL);
    }
    func_8049BA58(self, &next);
}

// Scene-model init: stashes the manager, clears the act table, then binds the
// model's root node into the act-manager (func_8049BA58).
void func_8049B9EC(CScnEffectActNw4r* self, CScnEffectActMgr* mgr) {
    self->mpMgr = mgr;
    self->mCount = 0;
    nw4r::g3d::ResMdl resMdl(mgr->field_0x146c);
    nw4r::g3d::ResNode node = resMdl.GetResNode(0);
    func_8049BA58(self, &node);
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace EABlob {
extern "C" void __dt__17CScnEffectActNw4rFv();
extern "C" void func_8049BEA4(); extern "C" void func_8049BA44();
extern "C" void func_8049BEAC(); extern "C" void func_8049BEB4();
extern "C" void func_8049BEBC(); extern "C" void func_8049BED0();
extern "C" void func_8049BEE4(); extern "C" void func_8049BEEC();
extern "C" void func_8049BF0C(); extern "C" void func_8049BF34();
extern "C" void func_8049BF5C(); extern "C" void func_8049BF84();
extern "C" void func_8049BFAC(); extern "C" void func_8049C060();
extern "C" void func_8049C18C(); extern "C" void func_8049C244();
extern "C" void func_8049C430(); extern "C" void func_8049C314();
extern "C" void func_8049C394(); extern "C" void func_8049C39C();
extern "C" void func_8049C3A4(); extern "C" void func_8049C3AC();
extern "C" void func_8049C420();
}
extern "C" u32 lbl_eu_80663A20;   // foreign .sdata
extern "C" u32 lbl_eu_80663A28;   // foreign .sdata

// [.data] 0x8056EAD8-0x8056EB50 (120B): CScnEffectActNw4r vtable + sub-view
extern "C" u32 lbl_eu_8056EAD8[26] = {
    (u32)&lbl_eu_80663A20, 0x00000000,
    (u32)&EABlob::__dt__17CScnEffectActNw4rFv,
    (u32)&EABlob::func_8049BEA4, (u32)&EABlob::func_8049BA44, (u32)&EABlob::func_8049BEAC,
    (u32)&EABlob::func_8049BEB4, (u32)&EABlob::func_8049BEBC, (u32)&EABlob::func_8049BED0,
    (u32)&EABlob::func_8049BEE4, (u32)&EABlob::func_8049BEEC, (u32)&EABlob::func_8049BF0C,
    (u32)&EABlob::func_8049BF34, (u32)&EABlob::func_8049BF5C, (u32)&EABlob::func_8049BF84,
    (u32)&EABlob::func_8049BFAC, (u32)&EABlob::func_8049C060, (u32)&EABlob::func_8049C18C,
    (u32)&EABlob::func_8049C244, (u32)&EABlob::func_8049C430, (u32)&EABlob::func_8049C314,
    (u32)&EABlob::func_8049C394, (u32)&EABlob::func_8049C39C, (u32)&EABlob::func_8049C3A4,
    (u32)&EABlob::func_8049C3AC, (u32)&EABlob::func_8049C420,
};
extern "C" u32 lbl_eu_8056EB40[4] = {
    (u32)&lbl_eu_80663A28, 0x00000000, 0x00000000, 0x00000000,
};
