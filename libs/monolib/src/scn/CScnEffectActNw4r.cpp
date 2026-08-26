// CScnEffectActNw4r: nw4r-model-backed effect-act table for scenes.
//
// The class keeps a fixed table of 64 effect acts (plus a parallel u16 id
// table) harvested from named nodes of the scene model, and forwards most
// lookups to the shared CScnEffectActMgr (its own local table first, then the
// manager's act chain). The .data vtable below binds these functions to the
// retail slots.

#include <harness_catalog.h>
#include <string.h>
#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include "libs/monolib/src/scn/CScnEffectActNw4r.hpp"

// This TU's .data vtable (defined at the bottom of this file); the constructor
// installs it. Global-scope data names are not mangled by MWCC.
extern u32 lbl_eu_8056EAD8[];

// Constructor: installs the vtable and clears the act-table count.
// Retail order: vtable store first, then mCount = 0 (body, not init list).
CScnEffectActNw4r::CScnEffectActNw4r() { *(void**)this = (void*)lbl_eu_8056EAD8; mCount = 0; }

// Dtor leaves the vtable untouched; as a virtual dtor MWCC generates the
// deleting form (null-this guard + operator delete on flag bit 0).
CScnEffectActNw4r::~CScnEffectActNw4r() {}

// ===== Vtable slot implementations =====
//
// Retail names are unrecovered plain func_* symbols, so these definitions keep
// C linkage to emit the exact linker names referenced by other TUs and by the
// vtable relocs below.

// Most slots are one-line forwarders: load mpMgr and tail-call into the
// manager's implementation.
extern "C" void func_8049BEA4(CScnEffectActNw4r* self) { func_80495E60(self->mpMgr); }

extern "C" void func_8049BEAC(CScnEffectActNw4r* self) { func_80484E5C(self->mpMgr); }

extern "C" void func_8049BEB4(CScnEffectActNw4r* self) { func_804838DC(self->mpMgr); }

extern "C" u8* func_8049BEE4(CScnEffectActNw4r* self) { return func_8048315C(self->mpMgr); }

// Copy the position triplet into the manager's scale/pos fields at 0x304.
// Kept as integer word copies: retail moves the bits without float ops.
extern "C" void func_8049BEEC(CScnEffectActNw4r* self, const void* other) {
    CScnEffectActMgr* mgr = self->mpMgr;
    *(u32*)&mgr->field_0x304 = *(u32*)other;
    *(u32*)&mgr->field_0x308 = *(u32*)((const char*)other + 4);
    *(u32*)&mgr->field_0x30c = *(u32*)((const char*)other + 8);
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
            // Named in z,y,x order so MWCC's load scheduler matches retail.
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

extern "C" void func_8049C394(CScnEffectActNw4r* self) { func_80482AB8(self->mpMgr); }

void func_8049C39C(CScnEffectActNw4r* self) {
    func_80482AD4(self->mpMgr);
}

void func_8049C3A4(CScnEffectActNw4r* self) {
    func_8048856C(self->mpMgr);
}

// Effect-range gate: the act is usable while the manager's range value sits
// inside [AB74 - A208, AB74 + A208], unless the manager's act flag disables
// the whole check (then always usable). Returns 1 usable / 0 outside range.
u32 func_8049C3AC(CScnEffectActNw4r* self) {
    u32 result;
    u32 ok;
    CScnEffectActMgr* mgr;
    mgr = self->mpMgr;
    result = 1;
    ok = 1;
    if ((mgr->field_0x7a4 & 0x20000000) == 0) {
        // Upper gate fails when val < AB74 + A208.
        if (!(mgr->field_0x304 >= lbl_eu_8066AB74 + lbl_eu_8066A208)) {
            ok = 0;
        }
    }
    if (ok == 0) {
        // Below the low bound the act stays usable; only the gap band clears.
        if (!(mgr->field_0x304 <= lbl_eu_8066AB74 - lbl_eu_8066A208)) {
            result = 0;
        }
    }
    return result;
}

// Bit 30 of the manager's flag word: the manager has a direct act ready.
extern "C" u32 func_8049C420(CScnEffectActNw4r* self) { return (self->mpMgr->field_0x7a4 >> 30) & 1; }

extern "C" u32 func_8049C430(CScnEffectActNw4r* self) { return self->mCount; }

// Virtual dispatch thunks on the manager (slots 4 / 43 / 16).
extern "C" void func_8049BA44(CScnEffectActNw4r* self) { self->mpMgr->v4(); }
extern "C" void func_8049BEBC(CScnEffectActNw4r* self) { self->mpMgr->v43(); }
extern "C" void func_8049BED0(CScnEffectActNw4r* self) { self->mpMgr->v16(); }

// Recursively register every node whose name is "<tag><NNN>" (three digits at
// offsets 2..4) into the effect-act table: the node's mtx id selects the act
// slot (0x30-byte entries from mgr->field_0x147c->field_0xec), and the digits
// parse into the u16 id stored next to the act pointer. Then descend into the
// node's child and next sibling.
// C linkage: the retail symbol (and the recursive self-calls) carry the plain
// unmangled name func_8049BA58.
extern "C" void func_8049BA58(CScnEffectActNw4r* self, nw4r::g3d::ResNode* node);
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
    // Link pointer for the tail recursion; declared before the gotos so its
    // live range spans the end: merge like retail.
    u8* lp;
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
    // NOTE: retail reads the 0x64 link first, then 0x60 - the reverse of the
    // header's toChildNode/toNextSibling field order. On a misaligned link the
    // retail build asserts but recurses with the pointer as-is (no reset).
    if (node->ptr()->toNextSibling != 0) {
        lp = (u8*)node->ptr() + node->ptr()->toNextSibling;
        if (((u32)lp & 3) != 0) {
            nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        }
    } else {
        lp = NULL;
    }
    nw4r::g3d::ResNode child(lp);
    func_8049BA58(self, &child);
    if (!node->IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E850, 0x2c, lbl_eu_8056E834,
                        lbl_eu_80663910, lbl_eu_80663A30);
    }
    if (node->ptr()->toChildNode != 0) {
        lp = (u8*)node->ptr() + node->ptr()->toChildNode;
        if (((u32)lp & 3) != 0) {
            nw4r::db::Panic(lbl_eu_8056E820, 0x2c, lbl_eu_8056E7F8);
        }
    } else {
        lp = NULL;
    }
    nw4r::g3d::ResNode next(lp);
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

// ===== [.data] 0x8056EAD8-0x8056EB50: CScnEffectActNw4r vtable + sub-view =====

// The compiler-generated deleting-dtor symbol has no C++ spelling; declare the
// mangled name for the vtable slot (repo-wide recipe, cf. CDeviceFont.cpp).
extern "C" void __dt__17CScnEffectActNw4rFv();

// Foreign .sdata words (typeinfo headers for the two views).
extern u32 lbl_eu_80663A20;
extern u32 lbl_eu_80663A28;

extern "C" u32 lbl_eu_8056EAD8[26] = {
    (u32)&lbl_eu_80663A20, 0x00000000,
    (u32)&__dt__17CScnEffectActNw4rFv,
    (u32)&func_8049BEA4, (u32)&func_8049BA44, (u32)&func_8049BEAC,
    (u32)&func_8049BEB4, (u32)&func_8049BEBC, (u32)&func_8049BED0,
    (u32)&func_8049BEE4, (u32)&func_8049BEEC, (u32)&func_8049BF0C,
    (u32)&func_8049BF34, (u32)&func_8049BF5C, (u32)&func_8049BF84,
    (u32)&func_8049BFAC, (u32)&func_8049C060, (u32)&func_8049C18C,
    (u32)&func_8049C244, (u32)&func_8049C430, (u32)&func_8049C314,
    (u32)&func_8049C394, (u32)&func_8049C39C, (u32)&func_8049C3A4,
    (u32)&func_8049C3AC, (u32)&func_8049C420,
};
extern "C" u32 lbl_eu_8056EB40[4] = {
    (u32)&lbl_eu_80663A28, 0x00000000, 0x00000000, 0x00000000,
};
