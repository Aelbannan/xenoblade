// Auto-scaffolded catalog TU for kyoshin/cf/chain/CChainEffect
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/chain/CChainEffect.hpp"

namespace cf {}

cf::CChainEffect::CChainEffect() : unk4(0), unk8(0) {}
cf::CChainEffect::~CChainEffect() {}

// Look up an effect linkage object by (class-id, member pointer). The member
// pointer stores the object offset as a run-time pointer; when non-null it is
// rebased by 0x3E9C before the lookup (target 1).
void* func_802A07F4(u32 cls, int mem) {
    if (mem != 0) {
        mem += 0x3E9C;
    }
    return func_800451D8(cls, mem);
}

// Same as func_802A07F4, but the member pointer is loaded from src->field_00
// first (target 2).
void* func_802A0804(u32 cls, u32* src) {
    int mem = *src;
    if (mem != 0) {
        mem += 0x3E9C;
    }
    return func_800451D8(cls, mem);
}

// Bind a chain effect (target 8). When `a` is non-zero, unlink the previous
// owner (if any) and link `self` to the resolved object; when `a` is zero,
// tear down the current link and clear.
extern "C" void func_802A0950(cf::CChainEffect* self, int a, int b, int c, int d, int e) {
    if (a != 0) {
        if (self->unk8 != (u32)d) {
            func_802A0950(self, 0, 0, 0, 0, 0);
        }
        if (self->unk4 != 0) {
            return;
        }
        // Resolve the member pointer encoded in c (0x3E9C rebase).
        u32 member = (c != 0) ? *(u32*)c : 0;
        if (member != 0) {
            member += 0x3E9C;
        }
        CChainObj* obj = (CChainObj*)func_800451D8((u32)b, (int)member);
        self->unk4 = (u32)obj;
        if (obj != 0) {
            obj->field_b0 = self;
        }
        if (d != 0) {
            u32 m2 = *(u32*)d;
            if (m2 != 0) {
                m2 += 0x3E9C;
            }
            func_800ACFD8((void*)self->unk4, (void*)m2);
        }
        self->unk8 = (u32)d;
        if (e != 0) {
            func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, (u32)e, 0, 0, lbl_eu_80668C50);
        }
        return;
    }

    // Release: unbind the linked object and clear the effect fields.
    CChainObj* obj = (CChainObj*)self->unk4;
    if (obj != 0) {
        obj->field_b0 = 0;
        if (obj->field_98 != 0) {
            func_800ACFD8(obj, 0);
            func_800ACC14((void*)self->unk4, 1);
        } else {
            func_800B3A88(func_800B07E8(), obj);
        }
        self->unk4 = 0;
        self->unk8 = 0;
    }
}

// Release helper wrappers (targets 4-6).
void func_802A0AE0(cf::CChainEffect* self, u32 obj) {
    if (obj != self->unk4) {
        return;
    }
    self->unk4 = 0;
    self->unk8 = 0;
}

void func_802A0AA0(cf::CChainEffect* self, u32 obj) {
    if (self->unk4 == 0) {
        return;
    }
    if (self->unk8 == 0) {
        return;
    }
    if (*(u32*)(self->unk8) != obj) {
        return;
    }
    func_802A0950(self, 0, 0, 0, 0, 0);
}

void func_802A0904(cf::CChainEffect* self) {
    func_802A0950(self, 0, 0, 0, 0, 0);
    self->unk4 = 0;
    self->unk8 = 0;
}

void func_802A08F4(void* self) {
    *(unsigned long*)((char*)self + 4) = 0;
    *(unsigned long*)((char*)self + 8) = 0;
}

// Scan the manager's circular object list and bind-remove every object matching
// (id, p2). The scan repeats until a pass finds nothing; the return flags
// whether any object was matched (target 7).
int func_802A0818(s32 id, u32 p2) {
    int found = 0;
    int matched;
    do {
        CChainManager* mgr = func_800B6C34();
        CChainNode* node = mgr->field_04->field_00;
        matched = 0;
        do {
            CChainItem* item = (CChainItem*)func_800AC610(node->field_08);
            if (item != 0 && item->field_8C == id
                && (p2 == 0 || p2 == (item->field_9C != 0 ? item->field_9C - 0x3E9C : 0))) {
                func_800B3A88(func_800B07E8(), item);
                matched = 1;
                break;
            }
            node = node->field_00;
        } while (node != mgr->field_04);
        if (matched) {
            found = 1;
        }
    } while (matched);
    return found;
}