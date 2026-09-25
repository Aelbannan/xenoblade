// Auto-scaffolded catalog TU for kyoshin/cf/CPartsChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CPartsChange.hpp"

using cf::CfPartyInfo;
using cf::CfPartyInfoSortKey;
using cf::CfActorAccessors;
using cf::CfObjectPcExt;

void __ct__cf_CPartsChange(){}

// removed destructor to suppress vtable duplication

void CPartsChange_ResolveSlotNames(){}

void CPartsChange_NotifySlotStates(){}

void CPartsChange_SyncSlotState(){}

void func_801931D0(){}

void __ct__80193270(){}

void __dt___reslist_base_cf_CfPartyInfo(){}

void __dt__reslist_cf_CfPartyInfo(){}

void __dt__80193538(){}

extern "C" u32 lbl_eu_8066430C;
extern "C" int lbl_eu_80664308;
extern "C" void func_80193810(u8* self);

extern "C" u32 CPartsChange_GetActorTable(void) { return lbl_eu_8066430C; }

void CPartsChange_FireIdEffect(){}

void func_80193710(){}

extern "C" int CPartsChange_GetLandmarkTable(void) { return lbl_eu_80664308; }

extern "C" void CPartsChange_DispatchChangeList(u8* self) { func_80193810(self); }

extern void* findObjectById(int);
extern "C" int lookupWorkAtAddr(void* addr);
extern "C" void gmFileObject(void* addr);
extern "C" s16 lbl_eu_80664314;

// Walk the party-change list at this+0xA80C: for each node whose slot
// table has bit0 set, resolve up to 16 object ids through findObjectById,
// keep those that pass lookupWorkAtAddr, then notify each via gmFileObject
// and reset the list / 0xA40-byte work buffer.
extern "C" void func_80193810(u8* self) {
    struct Slot {
        int id;
        u32 pad;
    };
    struct Node {
        Node* next;
        u32 pad04;
        Slot* slots;
    };

    Node* sentinel = *(Node**)(self + 0xA80C);
    Node* node = sentinel->next;
    void* found[96];
    u32 count = 0;

    while (node != sentinel) {
        Slot* slots = node->slots;
        if ((*(u16*)((u8*)slots + 0xA0) & 1) != 0) {
            int i;
            for (i = 0; i < 16; i++, slots++) {
                void* p = findObjectById(slots->id);
                void* q = p != 0 ? (u8*)p - 0x3E9C : p;
                if (q != 0) {
                    void* r = q != 0 ? (u8*)q + 0x3E9C : q;
                    if (lookupWorkAtAddr(r) != 0) {
                        if (q != 0) q = (u8*)q + 0x3E9C;
                        found[count++] = q;
                    }
                }
            }
        }
        node = node->next;
    }

    {
        u32 i;
        for (i = 0; i < count; i++) {
            gmFileObject(found[i]);
        }
    }

    *(u32*)(self + 0x9800) = 0;
    *(u32*)(self + 0xA804) = 0;
    {
        Node* n = sentinel->next;
        while (n != sentinel) {
            Node* cur = n;
            n = n->next;
            cur->next = 0;
        }
        sentinel->next = sentinel;
        ((u32*)sentinel)[1] = (u32)sentinel;
    }
    memset(self + 0xA828, 0, 0xA40);
    *(s16*)(self + 0xB272) = 5;
    *(s16*)(self + 0xB270) = 1;
    *(s16*)(self + 0xB274) = 0;
    lbl_eu_80664314 = 0;
}

void CPartsChange_InitChangeRecord(){}

void CfActorAccessors::SetFlag400(int enable) { if (enable) mFlags1E |= 0x400; else mFlags1E &= ~0x400; }

u32 CfActorAccessors::GetField94() { return mField94; }

// Scan 16 actor slots at self+0xA828 (stride 0xA4); match word at +0xA8BC.
extern "C" void* CPartsChange_FindActorById(u8* self, u32 id) {
    // Shared null epilogue (retail or r3,r7). Pure reg_swap vs retail p/result
    // colours is accepted via witness when FULL is not reached.
    void* result = 0;
    if (id != 0) {
        u8* p = self;
        for (u32 i = 0; i < 16; i++) {
            if (id == *(u32*)(p + 0xA8BC)) {
                result = self + 0xA828 + i * 0xA4;
                break;
            }
            p += 0xA4;
        }
    }
    return result;
}

void func_80193B0C(){}

void CPartsChange_InitActorEntry(){}

u16 CfActorAccessors::GetField9E() { return mField9E; }

void CPartsChange_FindActorByObj(){}
void func_80193D48(){}

void func_80194264(){}

void func_801943E4(){}

void func_80194610(){}

void func_801949E0(){}

void func_80194AFC(){}

void func_80194D5C(){}

// 0x4C-stride party-change element + manager view for CPartsChange_UpdateElemSpeeds
// (layout from CPartsChange.ctx.c).
struct CfPartsElem4C {
    u8 pad_00[0x10];
    f32 field_10;
    f32 field_14;
    f32 field_18;
    u16 field_1C;
    u16 field_1E;
    u8 pad_20[0x10];
    u8 field_30[9];
    u8 pad_39[0x13];
};
struct CfPartsElemArray {
    CfPartsElem4C mElems[0x200];
    u32 mCount;
};
struct CfPartsManager {
    CfPartsElemArray mElems;
};
extern "C" int CfRes_getD80Flag();
extern "C" f32 Scn_GetFrameDelta(void);
extern const f32 lbl_eu_80667AC0;
extern const f32 lbl_eu_80667AD4;

// Per-frame speed decay across the party-change element array.
extern "C" void CPartsChange_UpdateElemSpeeds(CfPartsManager* self) {
    CfRes_getD80Flag();
    f32 step = Scn_GetFrameDelta();
    CfPartsElemArray* arr = &self->mElems;
    for (CfPartsElem4C* e = arr->mElems; e != arr->mElems + arr->mCount; e++) {
        if (e->field_1E & 0x400) {
            e->field_14 = lbl_eu_80667AC0;
        } else if (e->field_14 > lbl_eu_80667AD4) {
            e->field_14 -= step;
            if (e->field_14 < lbl_eu_80667AD4) e->field_14 = lbl_eu_80667AD4;
        }
        if ((e->field_1E & 0x4) != 0) continue;
        if (!(e->field_10 > lbl_eu_80667AD4)) continue;
        volatile u16& curFlags = e->field_1E;
        if ((curFlags & 0x800) != 0) continue;
        e->field_10 -= step;
        if (!(e->field_10 > lbl_eu_80667AD4)) {
            e->field_10 = lbl_eu_80667AD4;
            if ((e->field_1E & 0x200) != 0 && (e->field_1E & 0x80) != 0) {
                e->field_1E |= 0x20;
            }
            memset(&e->field_30[0], 0, 9);
        }
    }
}

u32 CfActorAccessors::TestFlag400() { return (mFlags1E >> 10) & 0x1u; }

extern "C" u32 CtrlRemote_TouchBitByArg(u32 arg);

// Retail bl's CtrlRemote_TouchBitByArg(self+0x1D44), then cntlzw/rlwinm bool coerce.
extern "C" int CPartsChange_IsSubStateClear(u8* self) {
    if (self == 0) return 0;
    return CtrlRemote_TouchBitByArg((u32)(self + 0x1D44)) == 0;
}

void CPartsChange_UpdateCollectionState(){}

void CPartsChange_CheckNameEntry(){}

void func_801953E8(){}

u32 CfActorAccessors::TestFlag8() { return (mFlags1E >> 3) & 0x1u; }

void CfActorAccessors::SetField8C(float val) { mField8C = val; }

void CfObjectPcExt::SetField45C4(u16 val) { mField45C4 = val; }

void CfObjectPcExt::SetField45C8(u16 val) { mField45C8 = val; }

u16 CfObjectPcExt::GetField45C6() { return mField45C6; }

void* CfActorAccessors::GetField30() { return &mField30; }

void* CfObjectPcExt::GetField60C() { return (void*)((u8*)this + 0x60c); }

void CfPartyInfo::SetField2D(u8 val) { field_2D = val; }

void CPartsChange_SpawnById(){}

void func_80195BD4(){}

void func_80195E5C(){}

bool CompareSortKey(const CfPartyInfoSortKey* a, const CfPartyInfoSortKey* b) {
    return a->sortKey < b->sortKey;
}

void func_80196434(){}

void func_80196864(){}

void CPartsChange_CopyObjFields(){}

void func_80196E04(){}

extern "C" void* CPartsChange_FindEntryById(u8* self, u32 id) {
    // Ascending i < n → mtctr + cmpli/ble (PATTERNS); shared null epilogue.
    if (id != 0) {
        u32 n = *(u32*)(self + 0x9800);
        u8* p = self;
        for (u32 i = 0; i < n; i++) {
            if (id == *(u16*)(p + 0x1C)) {
                return self + i * 0x4C;
            }
            p += 0x4C;
        }
    }
    return 0;
}

extern "C" void* CPartsChange_FindPartsElem(u8* self, u8* obj) {
    if ((*(u32*)(obj + 0x3F00) & 0x4) != 0) {
        u16 raw = *(u16*)(obj + 0x456C);
        int shifted = (int)raw >> 4;
        if (shifted != 0) {
            u32 n = *(u32*)(self + 0x9800);
            u8* p = self;
            u32 id = (u32)(u16)shifted;
            for (u32 i = 0; i < n; i++) {
                if (id == *(u16*)(p + 0x1C)) {
                    return (void*)((u8*)self + i * 0x4C);
                }
                p += 0x4C;
            }
        }
        return 0;
    }
    return 0;
}

void func_80197538(){}

void CPartsChange_ResolveActorEntry(){}

extern "C" int CPartsChange_CountListNodes(u8* self) {
    // Retail colours: end=r5, cur=r4, n=r3. Declare cur before end so the
    // allocator gives end the later volatile (r5) after cur claims r4.
    void* cur;
    void* end;
    int n;
    end = *(void**)(self + 4);
    n = 0;
    cur = *(void**)end;
    goto check;
loop:
    cur = *(void**)cur;
    n++;
check:
    if (cur != end) goto loop;
    return n;
}

void CPartsChange_TeardownContainer(){}

void CPartsChange_ResetBattleEntry(){}

void CPartsChange_SpawnItemDrop(){}

void func_80197DE8(){}

int lbl_eu_80664308;
u32 lbl_eu_8066430C;
unsigned char lbl_eu_80664310;
short lbl_eu_80664312;
void CPartsChange_SetLoadFlag(int arg) {
    if (lbl_eu_8066430C == 0) return;
    unsigned char* p = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p - 0x4d88) = (unsigned short)arg;
    if (arg == 0) return;
    unsigned char* p2 = (unsigned char*)lbl_eu_8066430C + 0x10000;
    *(unsigned short*)(p2 - 0x4d8a) = 0;
}

void CPartsChange_FindFreeSlot(){}

void CPartsChange_UnregisterEntry(){}

extern "C" void* CPartsChange_ResolveLinkedObj(int* self) {
    void* p = findObjectById(*self);
    if (p != 0) {
        p = (u8*)p - 0x3E9C;
    }
    return p;
}

extern "C" int CPartsChange_HasAnyEntry(u8* self) {
    // Scan 16 eight-byte {id, data} records for a live id. The 16-count loop
    // unrolls 8x into the retail two-trip CTR form and keeps the dead
    // per-iteration induction update (li r4,0 / addi r4,r4,7).
    struct Entry {
        u32 id;
        u32 data;
    };
    Entry* e = (Entry*)self;
    for (s32 i = 0; i < 16; i++) {
        if (e[i].id != 0) return 1;
    }
    return 0;
}

void CPartsChange_FindVoiceIndex(){}

void* CPartsChange_GetSlotEntryAt(void* self, unsigned long idx) {
    return *(void**)((char*)self + (idx << 3));
}

extern "C" void* CPartsChange_GetEnemySlotAt(u8* table, u32 idx) {
    void* p = findObjectById(*(int*)(table + (idx << 3)));
    if (p != 0) {
        p = (u8*)p - 0x3E9C;
    }
    return p;
}

void func_80198524(){}

extern "C" void CPartsChange_RemoveListNode(u8* self, void** key) {
    // Same shape as matched unlinkNodesByKey (code_800B06A4): next before
    // cur, next loaded first in the loop body → cur r8 / next r7.
    u32 sentinel = *(u32*)(self + 4);
    u32 next;
    u32 cur = *(u32*)sentinel;
    u32 zero = 0;
    while (cur != sentinel) {
        next = *(u32*)cur;
        if (*(u32*)(cur + 8) == *(u32*)key) {
            u32 prev = *(u32*)(cur + 4);
            *(u32*)prev = next;
            *(u32*)(next + 4) = prev;
            *(u32*)cur = zero;
        }
        cur = next;
    }
}

void CfPartyInfo::func_80198710(void* r4, float f1, int r5, int r6, float f2, float f3) {
    int r8 = *(int*)((char*)r4 + 0);
    int r7 = *(int*)((char*)r4 + 4);
    int r0 = *(int*)((char*)r4 + 8);
    field_00 = r8;
    field_04 = r7;
    field_08 = r0;
    field_18 = f1;
    field_0C = r5;
    field_14 = r6;
    field_1C = f2;
    field_20 = f3;
    if (r6 <= 0) {
        field_14 = 1;
    }
    extern float lbl_eu_80667B28;
    field_28 = lbl_eu_80667B28;
    field_2C = 0;
    field_2D = 1;
    field_2E = 0;
}

void CPartsChange_ProcessPartyInfo(){}

extern "C" void CPartsChange_ProbePartyCollisions(u32* src, u32* dst);

extern "C" void CPartsChange_CopyHeaderAndLoad(u32* src, u32* dst) {
    u32 w0 = src[0];
    u32 w1 = src[1];
    dst[1] = w1;
    dst[0] = w0;
    dst[2] = src[2];
    CPartsChange_ProbePartyCollisions(src, dst);
}

void func_80198AE0(){}

void CPartsChange_ComputeSpawnPoseA(){}

void func_80198D44(){}

void CPartsChange_ComputeSpawnPoseB(){}

void CPartsChange_ComputeSpawnPoseC(){}

void CPartsChange_ComputeSpawnPoseD(){}

void func_801990F0(){}

void CPartsChange_ComputeSpawnPoseFull(){}


// absorb tails - generated to match retail ET_REL bytes
__declspec(section ".data") __attribute__((aligned(8))) __attribute__((used)) unsigned char __tail_kyoshin_cf_CPartsChange_data[0xB8] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x40, 0x00, 0x00,
    0x3F, 0x00, 0x00, 0x00, 0x3E, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

__declspec(section ".rodata") __attribute__((aligned(8))) __attribute__((used)) const unsigned char __tail_kyoshin_cf_CPartsChange_rodata[0x128] = {
    0x63, 0x66, 0x3A, 0x3A, 0x43, 0x50, 0x61, 0x72, 0x74, 0x73, 0x43, 0x68,
    0x61, 0x6E, 0x67, 0x65, 0x00, 0x00, 0x00, 0x00, 0x25, 0x73, 0x25, 0x73,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72, 0x65, 0x73, 0x6C,
    0x69, 0x73, 0x74, 0x3C, 0x63, 0x66, 0x3A, 0x3A, 0x43, 0x66, 0x50, 0x61,
    0x72, 0x74, 0x79, 0x49, 0x6E, 0x66, 0x6F, 0x20, 0x2A, 0x3E, 0x00, 0x00,
    0x5F, 0x72, 0x65, 0x73, 0x6C, 0x69, 0x73, 0x74, 0x5F, 0x62, 0x61, 0x73,
    0x65, 0x3C, 0x63, 0x66, 0x3A, 0x3A, 0x43, 0x66, 0x50, 0x61, 0x72, 0x74,
    0x79, 0x49, 0x6E, 0x66, 0x6F, 0x20, 0x2A, 0x3E, 0x00, 0x00, 0x00, 0x00,
    0x65, 0x5F, 0x72, 0x65, 0x70, 0x6F, 0x70, 0x74, 0x69, 0x6D, 0x65, 0x00,
    0x70, 0x6F, 0x73, 0x58, 0x00, 0x70, 0x6F, 0x73, 0x59, 0x00, 0x70, 0x6F,
    0x73, 0x5A, 0x00, 0x52, 0x61, 0x64, 0x69, 0x75, 0x73, 0x00, 0x70, 0x6F,
    0x70, 0x5F, 0x74, 0x79, 0x70, 0x65, 0x00, 0x65, 0x6E, 0x65, 0x5F, 0x74,
    0x79, 0x70, 0x65, 0x00, 0x66, 0x6F, 0x72, 0x6D, 0x00, 0x70, 0x6F, 0x73,
    0x41, 0x54, 0x52, 0x00, 0x6E, 0x61, 0x6D, 0x65, 0x64, 0x00, 0x72, 0x65,
    0x73, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x00, 0x73, 0x6E, 0x61, 0x70, 0x00,
    0x44, 0x69, 0x72, 0x00, 0x42, 0x54, 0x4C, 0x5F, 0x65, 0x6E, 0x65, 0x6C,
    0x69, 0x73, 0x74, 0x00, 0x72, 0x6F, 0x75, 0x74, 0x65, 0x49, 0x44, 0x00,
    0x71, 0x75, 0x65, 0x73, 0x74, 0x49, 0x44, 0x00, 0x71, 0x75, 0x65, 0x73,
    0x74, 0x5F, 0x53, 0x54, 0x46, 0x4C, 0x47, 0x00, 0x53, 0x5F, 0x46, 0x4C,
    0x47, 0x5F, 0x4D, 0x49, 0x4E, 0x00, 0x53, 0x5F, 0x46, 0x4C, 0x47, 0x5F,
    0x4D, 0x41, 0x58, 0x00, 0x50, 0x4F, 0x50, 0x5F, 0x54, 0x49, 0x4D, 0x45,
    0x00, 0x6D, 0x6F, 0x76, 0x65, 0x5F, 0x74, 0x79, 0x70, 0x65, 0x00, 0x66,
    0x6F, 0x72, 0x6D, 0x5F, 0x72, 0x61, 0x6E, 0x67, 0x65, 0x00, 0x4E, 0x41,
    0x4D, 0x45, 0x44, 0x5F, 0x46, 0x4C, 0x47, 0x00, 0x63, 0x68, 0x69, 0x6C,
    0x64, 0x5F, 0x49, 0x44, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) unsigned char __tail_kyoshin_cf_CPartsChange_sdata[0x58] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x65, 0x6E, 0x65, 0x3F, 0x49, 0x44, 0x00, 0x00, 0x65, 0x6E, 0x65, 0x3F,
    0x50, 0x65, 0x72, 0x00, 0x65, 0x6E, 0x65, 0x3F, 0x6E, 0x75, 0x6D, 0x00,
    0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFC, 0x65, 0x6E, 0x65, 0x3F,
    0x49, 0x44, 0x00, 0x00, 0x00, 0x00, 0x01, 0xCC, 0x01, 0xCD, 0x01, 0xCE,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata2") __attribute__((aligned(8))) __attribute__((used)) const unsigned char __tail_kyoshin_cf_CPartsChange_sdata2[0xB0] = {
    0x5F, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x42, 0x70, 0x00, 0x00,
    0x41, 0xF0, 0x00, 0x00, 0x44, 0xE1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x65, 0x6E, 0x65, 0x3F, 0x49, 0x44, 0x00, 0x00,
    0x65, 0x6E, 0x65, 0x3F, 0x6E, 0x75, 0x6D, 0x00, 0x3D, 0xCC, 0xCC, 0xCD,
    0x3C, 0x23, 0xD7, 0x0A, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x42, 0x22, 0xF9, 0x83, 0x42, 0x34, 0x00, 0x00, 0x42, 0x20, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x41, 0x20, 0x00, 0x00, 0x3F, 0x40, 0x00, 0x00,
    0x44, 0x7A, 0x00, 0x00, 0x40, 0xA0, 0x00, 0x00, 0x3E, 0xAA, 0xAA, 0xAB,
    0x42, 0x48, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x00,
    0x3F, 0x35, 0x04, 0x81, 0x46, 0x1C, 0x40, 0x00, 0x42, 0xC8, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x3D, 0xCC, 0xCC, 0xCD, 0x3C, 0x23, 0xD7, 0x0B,
    0x3C, 0x23, 0xD7, 0x0A, 0x42, 0x22, 0xF9, 0x83, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
};
