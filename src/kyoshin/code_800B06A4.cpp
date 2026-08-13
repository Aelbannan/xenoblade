// Auto-scaffolded catalog TU for kyoshin/code_800B06A4
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/code_800B06A4.hpp"
#include <string.h>

// C-ABI import for func_800B3A88
// Forward declaration (defined elsewhere in this unit; unmangled name)
extern "C" void func_800B3A88(UnkClass_805764CC* self, void* target);

void UnkClass_800B0AD8::clearCounters() {
    unkB00 = 0;
    unkAFC = 0;
}
DECOMP_DONT_INLINE UnkClass_805764CC* func_800B07E8();
#pragma inline
u32 func_800AA2BC(u32 a, u32 b);
extern "C" void func_80193810(unsigned long a, void* b);
extern "C" void func_801F3CCC(unsigned long a, void* b);
extern "C" void func_801F45B4(unsigned long a, void* b);
extern "C" void func_802074F0(unsigned long a, void* b);
extern "C" void func_8019397C(unsigned long a, void* b);
extern "C" void func_80193D48(unsigned long a, void* b);
extern "C" void func_80195E5C(unsigned long a, float b);

// Forward declarations for callees used by my targets
struct CfMapMineManager;
extern void func_80206BD4(CfMapMineManager* self);
extern void func_800B44A0(UnkClass_805764CC* self, void* arg);
extern void* func_800B61FFC();
extern void func_800B137C(void* self, void* arg, int count);
extern void func_800B8524(void* singleton, void* self, void* other);
void* func_800B47A8(void* a, void* b, void* c, void* d, void* e, void* f, void* g);

void func_80081258(void* self);
void func_80081264(void* self, cf::CfObject* obj);

// Target 4: us-800b0f70 - Store float and its square to several globals.
// Retail emits frsp f2,f1 for `float sq` — the float->double->float round-trip
// (double d = a; float sq = (float)d;) keeps the explicit conversion; a plain
// `float sq = a` lets MWCC drop it.
extern "C" void func_800B06A4(float a) {
    extern float lbl_eu_80661CCC, lbl_eu_80661CD0;
    extern float lbl_eu_80663EC8, lbl_eu_80663ECC, lbl_eu_80663ED0, lbl_eu_80663ED4;
    double d = (double)a;
    float sq = (float)d;
    float c = lbl_eu_80661CD0;
    lbl_eu_80661CCC = a;
    sq = sq * sq;
    lbl_eu_80663EC8 = a;
    lbl_eu_80663ECC = sq;
    lbl_eu_80663ED0 = c;
    lbl_eu_80663ED4 = sq;
}

void init_0A90(void){}
// Target 1: us-800b1118 - func_800B084C
// Calls func_80061FFC() to get a handle, then passes it along with `count` to func_800B0894.
void func_800B084C(UnkClass_805764CC* self, unsigned long count) {
    func_800B0894(self, func_80061FFC(), count);
}

// Target 1: us-800b15d8 - reslist<cf::IFactoryEvent*>::reslist() constructor
// (flattened: base init with the _reslist_base vtable, then the derived
// vtable install overwrites +0). Array-typed externs force absolute lis/addi
// addressing; the base vtable store is volatile so MWCC keeps both stores.
// Local reslist view (mirror of the _reslist_base layout: vtable@0,
// mStartNodePtr@4, sentinel node@8 (mNext/mPrev/mItem, 0xC stride), mList@0x14,
// mCapacity@0x18, ownership flag@0x1C). The struct-typed sentinel access
// reproduces the retail scheduling (MWCC keeps li r0 first + the v2 lis/addi
// pair adjacent).
struct CfReslistNode {
    void* mNext;  // 0x0
    void* mPrev;  // 0x4
    void* mItem;  // 0x8 (node stride 0xC)
};
struct CfReslistLayout {
    void* mVtable;           // 0x00
    CfReslistNode* mStartNodePtr;  // 0x04
    CfReslistNode mStartNode;      // 0x08 (0xC bytes)
    void* mList;             // 0x14
    int mCapacity;           // 0x18
    bool field_0x1C;         // 0x1C
};

void __ct__reslist_cf_IFactoryEvent(void* self) {
    extern void* lbl_eu_805290B8[];
    extern void* lbl_eu_805290A0[];
    CfReslistLayout* obj = (CfReslistLayout*)self;
    *(volatile u32*)((u8*)self) = (u32)lbl_eu_805290B8;
    obj->mList = 0;
    obj->mCapacity = 0;
    obj->field_0x1C = false;
    obj->mStartNodePtr = &obj->mStartNode;
    obj->mStartNodePtr->mNext = &obj->mStartNode;
    obj->mStartNodePtr->mPrev = &obj->mStartNode;
    obj->mVtable = (void*)lbl_eu_805290A0;
}

// Target 1: us-800b1368 - reslist<cf::CfObject*>::reslist() constructor
// (flattened: base init with the _reslist_base vtable, then the derived
// vtable install overwrites +0). Array-typed externs force absolute lis/addi
// addressing instead of SDA21. The base vtable store is volatile so MWCC's
// dead-store elimination keeps it (the derived install provably overwrites
// +0, but retail emits both stores).
void __ct__reslist_cf_CfObject(void* self) {
    extern void* lbl_eu_8052585C[];
    extern void* lbl_eu_805290E8[];
    CfReslistLayout* obj = (CfReslistLayout*)self;
    *(volatile u32*)((u8*)self) = (u32)lbl_eu_8052585C;
    obj->mList = 0;
    obj->mCapacity = 0;
    obj->field_0x1C = false;
    obj->mStartNodePtr = &obj->mStartNode;
    obj->mStartNodePtr->mNext = &obj->mStartNode;
    obj->mStartNodePtr->mPrev = &obj->mStartNode;
    obj->mVtable = (void*)lbl_eu_805290E8;
}
// Target 3: us-800b186c - func_800B0FA0
extern "C" DECOMP_DONT_INLINE void func_800B0FA0(UnkClass_805764CC* self) {
    if (func_800B0FEC(&self->field_0xC80) == 0) {
        func_800B0FF4(&self->field_0xC80, func_80061FFC(), 4);
    }
}
#pragma inline

void init_0FA0(){}
u32 UnkClass_805764CC::get_u32_18(){return *(u32*)((u8*)this + 0x18);}
// func_800B0A90: zero the u32 at self (retail: li r0,0; stw r0,0x0(r3))
extern "C" void func_800B0A90(void* self) { *(u32*)self = 0; }
void init_dispatchTarget_1(){}
// func_800B1808: zero the global flag word (retail: li r0,0; stw r0,lbl_eu_80663EE0@sda21)
extern "C" void func_800B1808() { extern u32 lbl_eu_80663EE0; lbl_eu_80663EE0 = 0; }
void gflag_setBits(unsigned long flags){extern unsigned long lbl_eu_80663EE0;lbl_eu_80663EE0 |= flags;}
void FactoryEvent3__Q22cf13IFactoryEventFv(){}
void init_137C(){}
void node_copyNextPtr(void* dst, void* src){void* ptr = *(void**)((char*)src + 4); *(unsigned long*)dst = *(unsigned long*)ptr;}
void* node_getDataPtr(void* self){return (char*)(*(void**)self) + 8;}
void init_14E0(){}
void node_copyNextU32(void* dst, void* src){*(unsigned long*)dst = *(unsigned long*)((char*)src + 4);}
void init_14FC(){}
void __dt__800B151C();
extern "C" void func_800B1518(){void(*dtor)() = __dt__800B151C; dtor();}
void __dt__800B151C(){}
void init_dispatchTarget_2(){}
extern u32 lbl_eu_80663F24;
void gvar_clearF24(){lbl_eu_80663F24 = 0;}
void FactoryEvent4__Q22cf13IFactoryEventFv(){}
void UnkClass_805764CC::set_u32_00(u32 val){*(u32*)((u8*)this + 0x0) = val;}
void copy_int_ptr(int* dst, int* src){*dst = *src;}
void init_182C(){}
void __dt__800B183C(){}
void __dt__800B18CC(){}
void init_dispatchTarget_5(){}
// Target 1: func_800B93AC
extern "C" void func_800B93AC() {
    func_800B1B2C(func_800B07E8());
}

// Target 4: us-800b9df0 - func_800B94D4
void func_800B94D4(cf::CfObject* obj) {
    u8 stackBuf[0x28];
    func_80081258(stackBuf);
    func_80081264(stackBuf, obj);
    func_800B20B4(func_800B07E8(), 0x200, stackBuf, 0);
}

// Target 2: func_800B1A5C
extern "C" DECOMP_DONT_INLINE void func_800B1A5C(void* list) {
    u32 sentinel, cur, next, zero;
    u32* p;
    sentinel = *(u32*)((u32*)list + 1);
    cur = *(u32*)sentinel;
    zero = 0;
    goto check;
loop:
    p = (u32*)cur;
    cur = *p;
    *p = zero;
check:
    sentinel = *(u32*)((u32*)list + 1);
    if (cur != sentinel) goto loop;
    *(u32*)sentinel = sentinel;
}
#pragma inline

// Target: us-800b23c0 - func_800B1AF4: run the list/state init via
// func_800B72DC, then clear the 0x100 mask bit via func_800B4278.
extern "C" void func_800B72DC(void* self);
extern "C" void func_800B4278(void* object, u32 mask);

extern "C" void func_800B1AF4(void* self) {
    func_800B72DC(self);
    func_800B4278(self, 256);
}

// Target: us-800b1bf8 - cf::CfValueItemManager ctor: base ctor call then
// derived vtable install (retail lbl_eu_805316D0); returns this (retail
// emits the mr r3, r31 return-this after the vtable load).
extern "C" void __ct__cf_CfMapItemManager(void* self);
void* __ct__cf_CfValueItemManager(void* self) {
    extern void* lbl_eu_805316D0[];
    __ct__cf_CfMapItemManager(self);
    *(u32*)((u8*)self + 0) = (u32)lbl_eu_805316D0;
    return self;
}

void init_1A8C(){}
void* node_getDataPtr_alt(void* self){return (char*)(*(void**)self) + 8;}
void init_1AA8(){}
void copy_int_ptr_alt(int* dst, int* src){*dst = *src;}
void node_copyNextU32_alt(void* dst, void* src){*(unsigned long*)dst = *(unsigned long*)((char*)src + 4);}
void init_1AD8(){}
void init_1AF4(){}
void init_dispatchTarget_6(){}
void init_1BBC(){}
// us-800b2488: if the flag bit-6 test is set, null the self arg, then call
// func_800B1C24(8, self-or-0) (declared in code_800B06A4.hpp).
extern "C" void func_800B1BBC(void* self) {
    if (func_800B1C00()) {
        self = 0;
    }
    func_800B1C24(8, self);
}
// func_800B1C00: bit 6 of the global flag word (retail: lwz r0,lbl_eu_80663EE0; extrwi r3,r0,1,25 = (x>>6)&1)
extern "C" DECOMP_DONT_INLINE u32 func_800B1C00(){ extern u32 lbl_eu_80663EE0; return (lbl_eu_80663EE0 >> 6) & 1; }
void init_1C0C(){}
void init_1C24(){}
s32 func_800B1C40() {
    if (func_800B1C00()) {
        return 0;
    }
    return func_800B1C0C(8);
}
void init_1C78(){}
void init_1CDC(){}
void init_1E18(){}
// field_0xCA0; if nonzero tail-call with (field, r4-passthrough) (retail lwz;cmpwi;beqlr;b)
extern "C" void func_800B1E18(UnkClass_805764CC* self, void* obj){if (self->field_0xCA0){func_8019397C(self->field_0xCA0, obj);}}
void init_1E2C(){}
void init_1EB8(){}
void init_1EC8(){}
void init_1F2C(){}
extern "C" void func_800B1F2C(UnkClass_805764CC* self, void* obj){if (self->field_0xCA0){func_80193D48(self->field_0xCA0, obj);}}
extern "C" void func_800B1F40(UnkClass_805764CC* self, void* obj){if (self->field_0xCA0){func_80193810(self->field_0xCA0, obj);}}

// Target 1: us-800b2820 - field_0xCA0; if nonzero tail-call func_80195E5C(field, const)
extern "C" void func_800B1F54(UnkClass_805764CC* self) {
    if (self->field_0xCA0 == 0) return;
    extern float lbl_eu_80663EC8;
    func_80195E5C(self->field_0xCA0, lbl_eu_80663EC8);
}
void init_1F6C(){}
void init_1FD8(){}
extern "C" void func_800B2034(UnkClass_805764CC* self, void* obj){if (self->field_0xCFC){func_801F3CCC(self->field_0xCFC, obj);}}
extern "C" void func_800B2048(UnkClass_805764CC* self, void* obj){if (self->field_0xCFC){func_801F45B4(self->field_0xCFC, obj);}}
// TEST_FUNC_205C
extern "C" void func_800B20A0(UnkClass_805764CC* self, void* obj){if (self->field_0xCAC){func_802074F0(self->field_0xCAC, obj);}}
void init_20B4(){}
// us-800b2928: if the game-manager getter is nonzero and self->field_0xCAC
// is set, drive func_80206BD4 with it.
extern "C" u32 func_80082900__Q22cf13CfGameManagerFv();
extern "C" void func_800B205C(UnkClass_805764CC* self) {
    if (func_80082900__Q22cf13CfGameManagerFv() && self->field_0xCAC) {
        func_80206BD4((CfMapMineManager*)self->field_0xCAC);
    }
}
u32 UnkClass_805764CC::get_u32_04(){return *(u32*)((u8*)this + 0x4);}
// Target 5: us-800b35fc - __dt__800B2D30
// Destructor that calls subobject destructor, then frees memory if flags > 0.
// Returns self (standard MWCC destructor convention).
void* __dt__800B2D30(void* self, int flags) {
    void* ret = self;
    if (self != NULL) {
        __dt__8047BDA8(self);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return ret;
}
void FactoryEvent1__Q22cf13IFactoryEventFv(){}
void UnkClass_805764CC::fwdTo_2DB0(int param){extern void fwd_2DB0_body(void*, void*); fwd_2DB0_body(this, &param);}
void fwd_2DB0_body(){}
// Target 2: us-800b3704 - func_800B2E38
// Insert a node into a doubly-linked list with a fixed-size entry pool.
// Finds the first empty slot (entry[0] == 0) in the pool at list+0x14 (count at list+0x18),
// copies data from r6 into entry[8], copies the node template from r5[0] into entry[0..7],
// and links it into the list before the node pointed to by r5[0].
// Returns the new entry pointer via *r3.
extern "C" void func_800B2E38(void** out, void* list, void* templ, void* data) {
    u32 count = *(u32*)((u8*)list + 0x18);
    u32 entryBase = *(u32*)((u8*)list + 0x14);
    u32 idx = 0;
    u32 byteOff = 0;

    // Find first empty slot (entry[0] == 0)
    u32* base = (u32*)entryBase;
    for (; idx < count; idx++) {
        if (base[byteOff / 4] == 0) {
            break;
        }
        byteOff += 0xC;
    }

    // Calculate entry pointer
    u32* newEntry = (u32*)(entryBase + idx * 0xC);

    // Copy data word into entry[8] (retail guards the computed address:
    // addic. r4,r7,8; beq — kept from a source-level pointer null check).
    u32* p8 = (u32*)((u8*)newEntry + 8);
    if (p8 != 0) {
        *p8 = *(u32*)data;
    }

    // Insert before the node pointed to by templ[0]
    u32* targetNode = *(u32**)templ;
    newEntry[0] = (u32)targetNode;    // entry->next = target
    newEntry[1] = targetNode[1];       // entry->prev = target->prev
    // Link forward: target->prev->next = entry
    ((u32*)targetNode[1])[0] = (u32)newEntry;
    // Link backward: target->prev = entry
    targetNode[1] = (u32)newEntry;

    *out = newEntry;
}
void init_2ED0(){}
int obj_testBit_64_v0(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v1(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v2(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v3(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v4(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v5(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v6(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
u32 shift_u32_hi8(u32 val){return (val >> 16) & 0xFF;}
// Sorted circular buffer insertion (by pointer address, field_74 fast path).
extern "C" void func_800B3210(UnkClass_800B0AD8* self, UnkClass_805764CC** item_ptr) {
    u32 count = self->unkB00;

    if (count == 0) {
        ((UnkClass_805764CC**)self->unkAF8)[self->unkAFC] = *item_ptr;
        self->unkB00 = 1;
        return;
    }

    // Fast path: append at end if item's sort key >= last element's sort key
    {
        u32 head = self->unkAFC;
        u32 cap = self->unkB04;
        UnkClass_805764CC** buf = (UnkClass_805764CC**)self->unkAF8;
        u32 lastIdx = (head + count - 1) % cap;
        if (*(u32*)((u8*)*item_ptr + 0x74) >= *(u32*)((u8*)buf[lastIdx] + 0x74)) {
            buf[(head + count) % cap] = *item_ptr;
            self->unkB00 = count + 1;
            return;
        }
    }

    // Binary search for insertion point (unsigned pointer comparison)
    u32 insIdx;
    {
        u32 head = self->unkAFC;
        u32 cap = self->unkB04;
        UnkClass_805764CC** buf = (UnkClass_805764CC**)self->unkAF8;
        u32 lo = 0;
        u32 range = count;
        while (range > 0) {
            u32 mid = range / 2;
            u32 probe = lo + mid;
            u32 probeIdx = (head + probe) % cap;
            if (buf[probeIdx] < *item_ptr) {
                lo = probe + 1;
                range -= mid + 1;
            } else {
                range = mid;
            }
        }
        insIdx = lo;
    }

    // Shift elements to make room, choosing the shorter direction
    if (insIdx < count / 2) {
        // Shift elements [0, insIdx) toward head, then decrement head
        u32 i = 0;
        for (; i < insIdx; i++) {
            u32 h = self->unkAFC;
            u32 c = self->unkB04;
            UnkClass_805764CC** b = (UnkClass_805764CC**)self->unkAF8;
            u32 src = (h + i) % c;
            u32 dst = (src - 1) % c;
            b[dst] = b[src];
        }
        self->unkAFC = (self->unkAFC - 1) % self->unkB04;
    } else {
        // Shift elements [insIdx+1, count-1] toward tail
        u32 i = count - 1;
        for (; i > insIdx; i--) {
            u32 h = self->unkAFC;
            u32 c = self->unkB04;
            UnkClass_805764CC** b = (UnkClass_805764CC**)self->unkAF8;
            u32 src = (h + i) % c;
            u32 dst = (src + 1) % c;
            b[dst] = b[src];
        }
    }

    // Insert item and increment count
    {
        u32 head = self->unkAFC;
        u32 cap = self->unkB04;
        UnkClass_805764CC** buf = (UnkClass_805764CC**)self->unkAF8;
        buf[(head + insIdx) % cap] = *item_ptr;
    }
    self->unkB00 = self->unkB00 + 1;
}
u32 UnkClass_805764CC::get_u32_74(){return *(u32*)((u8*)this + 0x74);}
void init_39C8(){}
void copy_int_ptr_alt2(int* dst, int* src){*dst = *src;}
void UnkClass_805764CC::fwdTo_99EC(int param){extern void fwd_99EC_body(void*, void*); fwd_99EC_body(this, &param);}
void init_3A88(){}
u32 shift_u32_byte3(u32 val){return val >> 24;}
u32 shift_u32_hi8_alt1(u32 val){return (val >> 16) & 0xFF;}
void init_3D4C(){}
void init_3D64(){}
void init_4120(){}
void init_4278(){}
void init_42E8(){}
// Target 3: us-800b4c64 - func_800B4368
// Walk a linked list of CfObject nodes starting from self->field_0xBCC,
// find entries where data->field_0x94 == 2, and if name is NULL or
// strcmp(name, data->field_0x120) == 0, call func_800B3A88(self, data).
// The list has sentinel at *(self+0xBCC), nodes are [0]=next, [8]=data_ptr.
extern "C" DECOMP_DONT_INLINE void func_800B4368(UnkClass_805764CC* self, const char* name) {
    u8* sentinel = *(u8**)((u8*)self + 0xBCC);
    u8* cur = *(u8**)sentinel;

    while (cur != sentinel) {
        u8* data = *(u8**)(cur + 8);
        cur = *(u8**)cur;

        if (*(u32*)(data + 0x94) == 2) {
            if (name == NULL || strcmp(name, (const char*)(data + 0x120)) == 0) {
                func_800B3A88(self, (cf::CfObject*)data);
            }
        }
    }
}
#pragma inline
// List-walk search: start at *headPtr and skip nodes until the cursor equals
// *valA or its +8 link equals *valB, advancing *headPtr past each skipped
// node; store the found node to *out (retail func_800B4554).
extern "C" void func_800B4554(void** out, void** headPtr, void** valA, void** valB) {
    void* node;
    while ((node = *headPtr) != *valA && *(void**)((u8*)node + 8) != *valB) {
        *headPtr = *(void**)node;
    }
    *out = node;
}

// Null-guarded triple dispatch: fetch a handle, reset, then pass the reset
// result + handle to the forwarder (retail: handle in r4).
extern "C" void* func_800B720C();
extern "C" void* func_800B6CA0();
extern "C" void func_800B7214(void* a, void* b);

extern "C" void func_800B71CC(void* arg) {
    if (arg != 0) {
        void* v = func_800B720C();
        func_800B7214(func_800B6CA0(), v);
    }
}

// Target 2: us-800b4cfc - func_800B4400
// Clears all nodes from reslist at field_0xC28, then reinitializes its sentinel.
// Then iterates reslist at field_0xBC8, and for each node whose data's field_0x94
// is 1 or 6, calls func_800B2D88(&this->field_0xC28).
extern "C" void func_800B4400(UnkClass_805764CC* self) {
    // Clear all nodes from field_0xC28's list
    u32* headPtr = (u32*)((u8*)self + 0xC2C); // &field_0xC28.field_0x04
    u32* sentinel = (u32*)*headPtr;
    u32* node = (u32*)*sentinel;
    while (node != sentinel) {
        u32* next = (u32*)*node;
        *node = 0;
        node = next;
    }
    // Reinitialize sentinel: prev = next = sentinel
    *sentinel = (u32)sentinel;
    *(sentinel + 1) = (u32)sentinel;

    // Iterate field_0xBC8's list and call func_800B2D88 for matching nodes
    u32* headPtr2 = (u32*)((u8*)self + 0xBCC); // &field_0xBC8.field_0x04
    u32* sentinel2 = (u32*)*headPtr2;
    u32* node2 = (u32*)*sentinel2;
    while (node2 != sentinel2) {
        u32* data = (u32*)*(node2 + 2); // node->field_0x08 = data pointer
        u32 val = *(data + 0x94 / 4);   // data->field_0x94
        if (val == 1 || val == 6) {
            func_800B2D88((u8*)self + 0xC28, data);
        }
        node2 = (u32*)*node2;
    }
}
void init_44A0(){}
void init_4554(){}
void init_4588(u32 val){}
u32 shift_u32_hi8_alt2(u32 val){return (val >> 16) & 0xFF;}
void init_45A0(){}
void init_47A8(){}
void init_4A24(){}
int CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv(void* self){return 0;}
void init_4B0C(){}
int func_800B4B74(UnkClass_805764CC* self, u32 val){return (self->field_0x15F0 == val) ? 1 : 0;}
void init_4B88(){}
void init_4CA0(){}
void init_4D84(){}
u32 shift_u32_hi8_alt3(u32 val){return (val >> 16) & 0xFF;}
extern u16 lbl_eu_8066408C;
unsigned short gvar_get408C(){return lbl_eu_8066408C;}
extern "C" u16 func_800B4F64(){extern u16 lbl_eu_80664314; return lbl_eu_80664314;}
void init_4F6C(){}
void init_4F80(){}
void init_4F90(){}
u32 UnkClass_805764CC::get_u32_70(){return *(u32*)((u8*)this + 0x70);}
void init_4FAC(){}
void init_535C(){}
void init_570C(){}
void init_587C(){}
void init_5944(){}
void init_5948(){}
void UnkClass_805764CC::clear_380(){*(u32*)((u8*)this + 896) = 0;}
void* UnkClass_805764CC::getNull(){return 0;}
u32 UnkClass_805764CC::get_u32_380(){return *(u32*)((u8*)this + 0x380);}

// Target 2: us-800b13c0 - __dt__800B0AF4
// Destructor for UnkClass_800B0AD8. Clears counters and frees memory if flags > 0.
// Returns self (standard MWCC destructor convention).
extern "C"
UnkClass_800B0AD8* __dt__800B0AF4(UnkClass_800B0AD8* self, int flags) {
    if (self != NULL) {
        self->unkB00 = 0;
        self->unkAFC = 0;
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Target 2: us-800b6274 - Store value at array index, increment counter.
// Single-expression postfix form `arr[(*cnt)++] = *val` is required: it puts
// the rlwinm addr in r0 and the addi next in r4 (dead param reg) exactly like
// retail; the two-statement form always emits the swapped colors (r0/r4).
extern "C" void func_800B5978(UnkClass_805764CC* self, const u32* val) {
    u32* cnt = (u32*)((u8*)self + 0x380);
    ((u32*)self)[(*cnt)++] = *val;
}
void init_5994(){}
void init_6484(){}
void init_6494(){}
u32 shift_u32_hi8_alt4(u32 val){return (val >> 16) & 0xFF;}
void init_64B8(){}
u32 shift_u32_and(u32 a, u32 b){return a & b;}
u32 UnkClass_805764CC::get_u32_98(){return *(u32*)((u8*)this + 0x98);}
u16 UnkClass_805764CC::get_u16_45C0(){return *(u16*)((u8*)this + 0x45C0);}
u32 shift_u32_hi8_alt5(u32 val){return (val >> 16) & 0xFF;}
void init_64F8(){}
void init_6508(){}
void UnkClass_805764CC::appendToBuffer(void* other){
    unsigned int count = *(unsigned int*)((unsigned char*)this + 0x700);
    unsigned int val = *(unsigned int*)other;
    unsigned int newCount = count + 1;
    *(unsigned int*)((unsigned char*)this + 0x700) = newCount;
    float fval = *(float*)((unsigned char*)other + 4);
    void* dest = (unsigned char*)this + count * 8;
    *(unsigned int*)dest = val;
    *(float*)((unsigned char*)dest + 4) = fval;
}
void init_6544(){}
void init_655C(){}
void init_66AC(){}
void init_66BC(){}

// Target 3: us-800b70c8 - Return 1 if byte at offset 2 is in [1, 24].
// (retail codegen: lbz; li r3,0; cmplwi r0,1; bltlr; cmplwi r0,24; bgtlr; li r3,1; blr —
//  matches under GC/3.0a5.2; Wii/1.1 folds the range into (u8)(val-1)<=23)
extern "C" DECOMP_DONT_INLINE int func_800B67CC(void* self) {
    u8 val = *(u8*)((u8*)self + 2);
    int result = 0;
    if (val >= 1) {
        if (val <= 24) {
            result = 1;
        }
    }
    return result;
}
#pragma inline
void* UnkClass_805764CC::getPtr_1A8(){return (void*)((u8*)this + 0x1a8);}
void UnkClass_805764CC::clear_700(){*(u32*)((u8*)this + 1792) = 0;}
void init_6800(){}
void init_68A8(){}
void init_6AF4(){}
extern "C" UnkClass_805764CC* func_800B6BA0() { return func_800B07E8(); }
void* sub_getReslist_B28(){return &UnkClass_805764CC::func_800B07E8()->field_0xB28;}
void* sub_getReslist_B48(){return &UnkClass_805764CC::func_800B07E8()->field_0xB48;}
void* sub_getReslist_B68(){return &UnkClass_805764CC::func_800B07E8()->field_0xB68;}
void* sub_getReslist_B88(){return &UnkClass_805764CC::func_800B07E8()->field_0xB88;}
void* sub_getReslist_BE8(){return &UnkClass_805764CC::func_800B07E8()->field_0xBE8;}
void* sub_getReslist_BC8(){return &UnkClass_805764CC::func_800B07E8()->field_0xBC8;}
void* sub_getReslist_C08(){return &UnkClass_805764CC::func_800B07E8()->field_0xC08;}
void* sub_getReslist_C48(){return &UnkClass_805764CC::func_800B07E8()->field_0xC48;}
extern "C" reslist<cf::CfObject>* func_800B6CC4() {
    UnkClass_805764CC* obj = func_800B07E8();
    func_800B4400(obj);
    return &obj->field_0xC28;
}
void init_6CF8(){}
void* func_800B6D3C(void*);
void fwd_6DD0_body(){}
void init_6EC0(){}
u32 UnkClass_800B0AD8::getCount(){return *(u32*)((u8*)this + 0xB00);}
u32 UnkClass_800B0AD8::getSize(){return *(u32*)((u8*)this + 0xB04);}
void* UnkClass_800B0AD8::getRingElem(u32 index){
    u32 start = *(u32*)((u8*)this + 0xAFC); u32 count = *(u32*)((u8*)this + 0xB04);
    u32* base = *(u32**)((u8*)this + 0xAF8); return &base[(start + index) % count];
}
void sub_mainReset(){func_800B6D3C(func_800B07E8());}
void init_7058(){}
void init_708C(){}
void init_70FC(){}
u32 gvar_get40F4(void){extern u32 lbl_eu_806640F4; return lbl_eu_806640F4;}
void init_71CC(){}
void* UnkClass_805764CC::getPtr_720(){return (void*)((u8*)this + 0x720);}
void init_7214(){}
void init_72DC(){}
void init_7320(){}
void list_removeNode(void* out_prev, void* unused, void* list_ptr){
    (void)unused; void* entry = *(void**)list_ptr; void* prev = *(void**)entry;
    void* next = *(void**)((char*)entry + 4); *(void**)next = prev;
    *(void**)((char*)prev + 4) = next; entry = *(void**)list_ptr;
    *(void**)entry = NULL; *(void**)out_prev = prev;
}
void init_7410(){}
unsigned short gvar_get408C_alt1(){return lbl_eu_8066408C;}
unsigned short gvar_get408C_alt2(){return lbl_eu_8066408C;}
void UnkClass_805764CC::maskField_6C(u32 mask, int enable){
    u32* field = (u32*)((u8*)this + 0x6C);
    if (enable) { *field |= mask; } else { *field &= ~mask; }
}
extern "C" u16 func_800B75B4(){extern u16 lbl_eu_80663E42; return lbl_eu_80663E42;}
extern "C" u16 func_800B75BC(){extern u16 lbl_eu_80663E44; return lbl_eu_80663E44;}
void init_75EC(){}

// Target 5: us-800b7f9c - Count nodes in a circular linked list.
// Retail assigns head=r5/cur=r4. Winning shape: declare cur BEFORE head
// (uninitialized) so head's vreg is born after cur's, giving head the
// higher register r5 (retail lwz r5,4(r3); lwz r4,0(r5); cmpl r4,r5).
extern "C" int func_800B7680(void* self) {
    void* cur;
    void* head = *(void**)((u8*)self + 4);
    cur = *(void**)head;
    int count = 0;
    while (cur != head) { cur = *(void**)cur; count++; }
    return count;
}
void sub_resetReslist_B28(void* self){func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB28);}
void sub_resetReslist_B48(void* self){func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB48);}
void sub_resetReslist_B68(void* self){func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB68);}
void* sub_resetReslist_B88(void* self){return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB88);}
void* sub_resetReslist_BE8(void* self){return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBE8);}
void* sub_resetReslist_BC8(void* self){return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBC8);}
void* sub_resetReslist_C08(void* self){return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xC08);}
void* sub_resetReslist_BA8(void* self){return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBA8);}
void func_800B77E4(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xB28, obj);
}

void func_800B781C(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xB48, obj);
}

void func_800B7854(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xB68, obj);
}

void func_800B78C4(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xBE8, obj);
}

// Target 1: us-800b8218 - Add CfObject to reslist at field_0xBC8
void func_800B78FC(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xBC8, obj);
}

// Target 2: us-800b8250 - Add CfObject to reslist at field_0xBA8
void func_800B7934(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xBA8, obj);
}

// Target 3: us-800b8288 - Add CfObject to reslist at field_0xC08
void func_800B796C(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xC08, obj);
}
void init_79A4(){}
void init_7A18(){}
void init_7AF0(){}
void init_83AC(){}
void init_8524(){}
void init_dispatchTarget_3(){}
void init_dispatchTarget_4(){}
void init_8804(){}
// Target 4: us-800b91fc - func_800B88E0
// Remove nodes matching a given ID from a linked list at offset 0xC84.
// `next` is declared before `node` so MWCC colors node=r7/next=r6 like
// retail (the reverse declaration order swaps the registers).
void func_800B88E0(u8* self, u32 targetId) {
    u32* head = *(u32**)(self + 0xC84);
    u32* sentinel = head;
    u32* next;
    u32* node = (u32*)*head;
    while (node != sentinel) {
        next = (u32*)*node;
        if (node[2] == targetId) {
            u32* prev = (u32*)node[1];
            *prev = (u32)next;
            next[1] = (u32)prev;
            *node = 0;
        }
        node = next;
    }
}
extern "C" {
    extern s8 lbl_eu_80663EE8;
    extern u8 lbl_eu_80572CD4[];
    extern u8 lbl_eu_80572CC8[];
    extern void __dt__17UnkClass_805764CCFv(void*, int);
    extern void __ct__17UnkClass_805764CCFv(void*);
    extern void __register_global_object(void*, void*, void*);
    extern void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
    extern int func_800B64AC(void* p);
}
// Target: us-800b10b4 - func_800B07E8 (singleton sinit: init once, then return &singleton)
DECOMP_DONT_INLINE UnkClass_805764CC* func_800B07E8() {
    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }
    return (UnkClass_805764CC*)lbl_eu_80572CD4;
}
#pragma inline
// Target: us-800b1160 - func_800B0894 (allocate + zero-fill array of count*0xc, store at +0x14/+0x18)
extern "C" void func_800B0894(UnkClass_805764CC* self, unsigned long handle, unsigned long count) {
    u32* arr = (u32*)allocate_array__Q23mtl10MemManagerFUlUl(count * 0xc, handle);
    self->field_0x14 = (u32)arr;
    for (int i = 0; i < (int)count; i++) {
        arr[i * 3] = 0;
    }
    self->field_0x18 = (u32)count;
    // OPEN ITEM: retail unrolls this loop 8x with per-store reload of self->field_0x14
    // (aliasing pattern); 3 source shapes tried (local arr, ptr loop, member-cast),
    // best 23.4%/44 structural/0x134 vs 0x12c. Next: element-struct type or
    // reslist-method access form.
}
// Target: us-800b2220 - func_800B1954 (list cleanup)
extern "C" void func_800B1A8C(void* a, void* b);
extern "C" void* func_800B1A9C(void* a);
extern "C" void func_800B1AA8(void* a, void* b, int c);
extern "C" void func_800B1ACC(void* a, void* b);
extern "C" int func_800B1AD8(void* a, void* b);
extern "C" void func_800B0B40(void* a);
extern "C" void func_800B4278(void* object, u32 mask);
extern float lbl_eu_80663EDC;
extern u32 lbl_eu_80663EE4;
extern "C" void func_800B1954(UnkClass_805764CC* self) {
    func_800B4278(self, 0xFFFFFDFF); // retail: lis 1, subi 0x201 = 0x0000FDFF; li-form here aligns rest better (OPEN ITEM)
    func_800B4278(self, 0xFFFFFFFF);
    func_800B1A5C((void*)((u8*)self + 0xc28));
    func_800B1A5C((void*)((u8*)self + 0xc08));
    func_800B1A5C((void*)((u8*)self + 0xbc8));
    func_800B1A5C((void*)((u8*)self + 0xbe8));
    func_800B1A5C((void*)((u8*)self + 0xb88));
    func_800B1A5C((void*)((u8*)self + 0xb68));
    func_800B1A5C((void*)((u8*)self + 0xb48));
    func_800B1A5C((void*)((u8*)self + 0xb28));
    func_800B1A5C((void*)((u8*)self + 0xba8));
    func_800B1A5C(self);
    func_800B0B40((void*)((u8*)self + 0x20));
    self->field_0xD0E = 0;
    self->field_0xD10 = 0;
    self->field_0xD04 = 0;
    lbl_eu_80663EDC = 0.0f;
    lbl_eu_80663EE4 = 0;
    u32 sbA[4], sbB[4], sbC[4];
    func_800B1A8C(sbA, (void*)((u8*)self + 0xc48));
    for (;;) {
        func_800B1ACC(sbB, (void*)((u8*)self + 0xc48));
        if (func_800B1AD8(sbA, sbB) != 0) {
            void* item = func_800B1A9C(sbA);
            *(u32*)item = 0;
            func_800B1AA8(sbC, sbA, 0);
        } else {
            break;
        }
    }
}
// Target: us-800b6fb8 - func_800B66BC (guard-chain + event queue dispatch)
extern "C" void func_800B655C(void* self, void* list);
extern "C" void* func_800B67EC();
extern "C" void func_800B67F4(void* buf);
extern "C" void func_800B5994(void* self, void* arg, void* list, void* buf, float f);
extern "C" void func_800B4D84(void* self, void* buf);
extern "C" int CfRes_getE24Bit22();
extern "C" float func_80069EA0();
extern "C" int func_800829B8__Q22cf13CfGameManagerFv();
extern "C" void* getInstance__Q22cf14CBattleManagerFv();
extern float lbl_eu_80663ED8;
extern float lbl_eu_80663EC8;
extern float lbl_eu_80661CCC;
extern "C" void func_800B66BC(UnkClass_805764CC* self, void* arg) {
    if (arg == 0) return;
    if (func_800829B8__Q22cf13CfGameManagerFv() != 0) return;
    if (func_800B1C0C(2) == 0) {
        func_800B655C(self, (void*)((u8*)self + 0xb48));
    }
    if (lbl_eu_80663ED8 > 0.0f) {
        float dt = func_80069EA0();
        lbl_eu_80663ED8 -= dt;
        if (lbl_eu_80663ED8 < 0.0f) {
            lbl_eu_80663ED8 = 0.0f;
        }
    }
    int result = 0;
    if (CfRes_getE24Bit22() == 0 && getInstance__Q22cf14CBattleManagerFv() != 0) {
        void* tmp = func_800B67EC();
        result = func_800B67CC(tmp);
    }
    if (result == 0) {
        char buf[0x700];
        func_800B67F4(buf);
        func_800B5994(self, arg, (void*)((u8*)self + 0xb48), buf, lbl_eu_80663EC8);
        func_800B5994(self, arg, (void*)((u8*)self + 0xb68), buf, lbl_eu_80661CCC);
        func_800B4D84(self, buf);
    }
}
// Target: us-800b70fc - func_800B6800
extern "C" void func_800B6800(UnkClass_805764CC* self, void* arg, int flag, float value) {
    extern float lbl_eu_806669D8;
    if (flag) {
        self->field_0xCF4 = (u32)func_800B39C0(arg);
        self->field_0xCF8 = value;
    } else {
        self->field_0xCF8 = lbl_eu_806669D8;
        self->field_0xCF4 = 0;
        func_800B68A8(self, 0, (void*)&self->field_0xB48, flag, lbl_eu_806669D8);
        func_800B68A8(self, 0, (void*)&self->field_0xB68, flag, lbl_eu_806669D8);
        // OPEN ITEM: best 46.5%/19 structural; residual lfs/stfs f0-vs-f1 PS-float juggling
        // (retail: single lfs f1 hoisted to top, stfs f1; decomp: lfs f0 for stfs + lfs f1
        // per call). 7 source shapes tried; next: PS-float angle / expression order.
    }
    func_800B1C24(4, (void*)flag);
}
// Target: us-800b79ac - func_800B708C (singleton lookup + flag check)
extern "C" void* func_800B708C__Fi(int id) {
    UnkClass_805764CC* obj = func_800B07E8();
    void* result = func_800B6EC0(obj, id);
    bool valid = false;
    if (result != 0 && func_800B64AC(result) == 0) {
        valid = true;
    }
    return valid ? result : 0;
}
// Target 5: us-800b923c - func_800B8920
// Checks if an address is aligned and within a valid range [0x80000000, 0x93800000),
// then looks up the singleton and calls func_800B6EC0(&singleton, *(this+0x74)).
// Returns 1 if the check passes and func_800B6EC0 returns non-zero, else 0.
extern "C" int func_800B8920(u32 addr) {
    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }

    // Check alignment: low 2 bits must be 0, and address must be in [0x80000000, 0x93800000)
    if ((addr & 3) != 0 || addr < 0x80000000 || addr >= 0x93800000) {
        return 0;
    }

    u32 val = *(u32*)(addr + 0x74);
    int result = (int)func_800B6EC0((UnkClass_805764CC*)lbl_eu_80572CD4, val);
    return (result != 0) ? 1 : 0;
}
// Target 4: us-800b92e8 - func_800B89CC
// Singleton accessor for UnkClass_805764CC at lbl_eu_80572CD4.
// If the singleton hasn't been initialized (lbl_eu_80663EE8 == 0),
// construct it and register as a global object.
// Then access field_0xCA0 and call func_80193AB0(field_0xCA0, id).
// Returns the result of func_80193AB0, or 0 if field_0xCA0 is NULL.
extern "C" {
    extern s8 lbl_eu_80663EE8;
    extern u8 lbl_eu_80572CD4[];
    extern u8 lbl_eu_80572CC8[];
    extern void __dt__17UnkClass_805764CCFv(void*, int);
    extern void __ct__17UnkClass_805764CCFv(void*);
    extern void __register_global_object(void*, void*, void*);
    extern void* func_80193AB0(void*, u32);
}

extern "C" void* func_800B89CC(u32 id) {
    void* result = NULL;

    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }

    void* ca0 = *(void**)(lbl_eu_80572CD4 + 0xCA0);
    if (ca0 != NULL) {
        result = func_80193AB0(ca0, id);
    }

    return result;
}

// Target 5: us-800b9380 - func_800B8A64
// Same singleton pattern as func_800B89CC, but reads a u16 from
// this->field_0x45C0 and passes it as the id to func_80193AB0.
extern "C" void* func_800B8A64(void* self) {
    void* result = NULL;

    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }

    u16 id = *(u16*)((u8*)self + 0x45C0);
    void* ca0 = *(void**)(lbl_eu_80572CD4 + 0xCA0);
    if (ca0 != NULL) {
        result = func_80193AB0(ca0, id);
    }

    return result;
}

// Target 1: us-800b9418 - func_800B8AFC
// Singleton accessor + field_0xCA0 lookup, then calls func_80193CD0(field_0xCA0, this).
extern "C" void* func_800B8AFC(void* self) {
    void* result = NULL;

    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }

    void* ca0 = *(void**)(lbl_eu_80572CD4 + 0xCA0);
    if (ca0 != NULL) {
        result = func_80193CD0(ca0, self);
    }

    return result;
}
void init_8B94(){}
void init_8C78(){}
void init_8D5C(){}
void init_8FC4(){}
void sub_dispatchInit_1(){func_800B07E8(); ((void(*)())init_dispatchTarget_1)();}
void sub_dispatchInit_2(){func_800B07E8(); ((void(*)())init_dispatchTarget_2)();}
void init_92FC(){}
void sub_dispatchInit_3(){func_800B07E8(); ((void(*)())init_dispatchTarget_3)();}
void sub_dispatchInit_4(){func_800B07E8(); ((void(*)())init_dispatchTarget_4)();}
void sub_dispatchInit_5(){func_800B07E8(); ((void(*)())init_dispatchTarget_5)();}
void sub_dispatchInit_6(){func_800B07E8(); ((void(*)())init_dispatchTarget_6)();}
void init_93D0(){}
void init_9404(){}
void init_9438(){}
void init_946C(){}
void init_94A0(){}
void init_dispatchTarget_7(){}
void sub_dispatchWithArgs(u32 a, u32 b){func_800AA2BC(a, b); ((void(*)())init_dispatchTarget_7)();}
void init_9548(){}
void init_957C(){}
void UnkClass_805764CC::set_u32_734(u32 val){*(u32*)((u8*)this + 0x734) = val;}
void UnkClass_805764CC::set_u8_73A(u8 a, u8 b){this->field_0x20.unk0[0x71A] = a; this->field_0x20.unk0[0x71B] = b;}
void UnkClass_805764CC::set_u32_720(u32 val){*(u32*)((u8*)this + 0x720) = val;}
void UnkClass_805764CC::set_u16_738(u16 val){*(u16*)((u8*)this + 0x738) = val;}
void init_96E8(){}
void UnkClass_805764CC::set_float_730(float val){*(float*)((u8*)this + 0x730) = val;}
// Target 1: us-800ba028 - __ct__800B970C
// Constructor for a class with base vtable lbl_eu_8052AC98, final vtable lbl_eu_80537FB0,
// CCharVoice at offset 0x28, and fields at 0x6C/0x70.
// Size: 0x7C (allocated via allocate(0x7c, ...)).

// Forward declarations for symbols in this TU
struct CCharVoice;
extern "C" {
    extern u8 lbl_eu_8052AC98[];
    extern u8 lbl_eu_80537FB0[];
    extern void __ct__CCharVoice(CCharVoice* self);
}

extern "C" void* __ct__800B970C(void* self) {
    u8* s = (u8*)self;
    u8* baseVtab = lbl_eu_8052AC98;
    u8* finalVtab = lbl_eu_80537FB0;

    // Set base vtable and zero fields
    *(u32*)(s + 0x4) = 0;
    *(u32*)(s + 0x8) = 0;
    *(void**)s = baseVtab;
    *(void**)(s + 0xC) = baseVtab + 0xB4;
    *(void**)(s + 0x10) = baseVtab + 0xC4;
    *(u32*)(s + 0x14) = 0;
    *(u32*)(s + 0x18) = 0;
    *(u32*)(s + 0x1C) = 0;

    // Construct CCharVoice at offset 0x28
    __ct__CCharVoice((CCharVoice*)(s + 0x28));

    // Switch to final vtable and zero extra fields
    *(u32*)(s + 0x6C) = 0;
    *(void**)s = finalVtab;
    *(void**)(s + 0xC) = finalVtab + 0xB4;
    *(void**)(s + 0x10) = finalVtab + 0xC4;
    *(u32*)(s + 0x70) = 0;

    return self;
}
void init_97A0(){}
void init_985C(){}
void init_98C8(){}
u32 UnkClass_805764CC::get_u32_620(){return *(u32*)((u8*)this + 0x620);}
void init_998C(){}
void init_99BC(){}
void fwd_99EC_body(){}
void init_9A30(){}

// Target 5: us-800b141c - __ct__reslist_cf_TboxInfo
// Base reslist ctor: links the sentinel at this+8 to itself. Inlined into the
// derived ctor below, reproducing the retail store schedule.
template <typename T>
_reslist_base<T>::_reslist_base() {
    this->field_0x04 = (u32)((u8*)this + 8);
    this->sentinel_prev = (u32)((u8*)this + 8);
    this->sentinel_next = (u32)((u8*)this + 8);
}

// TboxInfo reslist ctor: zeroes the 0x2c/0x30/0x34 fields over the base.
template <>
reslist<cf::TboxInfo>::reslist() : _reslist_base<cf::TboxInfo>() {
    this->field_0x2c = 0;
    this->field_0x30 = 0;
    this->field_0x34 = 0;
}

_reslist_base<cf::TboxInfo>::~_reslist_base(){}
reslist<cf::TboxInfo>::~reslist(){}
_reslist_base<cf::IFactoryEvent*>::~_reslist_base(){}
reslist<cf::IFactoryEvent*>::~reslist(){}

// Target 3: us-800ba2a8 - func_800B998C
void* func_800B998C(void* self, void* a1, void* a2, void* a3, void* a4, void* a5) {
    return func_800B47A8((void*)1, self, a1, a2, a3, a4, a5);
}

// Target 4: us-800ba2d8 - func_800B99BC
void* func_800B99BC(void* self, void* a1, void* a2, void* a3, void* a4, void* a5) {
    return func_800B47A8((void*)0, self, a1, a2, a3, a4, a5);
}

// Target 5: us-800ba35c - sinit_800B9A40
extern "C" void sinit_800B9A40() {
    extern float lbl_eu_80661CC8, lbl_eu_80661CCC, lbl_eu_80661CD0;
    extern float lbl_eu_80663EC8, lbl_eu_80663ECC, lbl_eu_80663ED0, lbl_eu_80663ED4;
    extern u32 lbl_eu_80663EE0;
    float e = lbl_eu_80661CD0;
    float c = lbl_eu_80661CC8;
    float c2 = c * c;
    float d = lbl_eu_80661CCC;
    lbl_eu_80663EC8 = c;
    lbl_eu_80663ECC = c2;
    lbl_eu_80663ED0 = e;
    lbl_eu_80663ED4 = d * d;
    lbl_eu_80663EE0 = 0;
}

// Target 4: us-800b1da0 - deref field at +0x0 and add 8
u32 func_800B14D4(u8* self) {
    return *(u32*)self + 8;
}
// Target 5: us-800b1dc8 - compare two u32 for inequality (dual-subf/or/srwi idiom)
u32 func_800B14FC(int* a, int* b) {
    int va = *a;
    int vb = *b;
    return va != vb;
}
// Target 1: us-800b35f4 - accessor returning field at +0x4
u32 func_800B2D28(u8* self) {
    return *(u32*)(self + 0x4);
}
// Target 5: us-800b3aa0 - extract bit 7 of field at +0x64
u32 func_800B31D4(u8* self) {
    return (*(u32*)(self + 0x64) >> 7) & 1;
}
// Target 1: us-800b4630 - return bit 31 of field at +0x64
u32 func_800B3D34(u8* self) {
    return (*(u32*)(self + 0x64) >> 31) & 1;
}
// Target 2: us-800b463c - return bit 27 of field at +0x6C
u32 func_800B3D40(u8* self) {
    return (*(u32*)(self + 0x6C) >> 27) & 1;
}
// Target us-800b5320: func_800B4A24
// Rejects null / non-enabled args, then checks whether the parent container's
// field_0x15F0 type id lies in {4,5,6,7,8}.
extern "C" s32 func_800B4A24(CEvtTypeArg* arg) {
    if (arg == 0) {
        return 0;
    }
    int vres = arg->fnTable[0x80](arg);
    if (vres == 0) {
        return 0;
    }
    if ((arg->flags & 0x04000000) == 0) {
        return 0;
    }

    // Recover the enclosing object (arg sits at +0x3E9C within it) and read
    // its type/state id at +0x15F0.
    UnkClass_805764CC* container = (UnkClass_805764CC*)((u8*)arg - 0x3E9C);
    s32 value = (s32)container->field_0x15F0;

    if (value == 4 || value == 5 || value == 6 || value == 7 || value == 8) {
        return 1;
    }
    return 0;
}
// Target 4: us-800b5868 - clear bit 0 and set bit 1 of the field at +0x6C
void func_800B4F6C(u8* self) {
    *(u32*)(self + 0x6C) = (*(u32*)(self + 0x6C) & ~1u) | 2u;
}
// Target 2: us-800b58a0 - accessor returning field at +0x70
u32 func_800B4FA4(u8* self) {
    return *(u32*)(self + 0x70);
}
// Target 3: us-800b6dcc - return mask & field at +0x6C
u32 func_800B64D0(u8* self, u32 mask) {
    return mask & *(u32*)(self + 0x6C);
}
// Target 2: us-800b3654 - func_800B2D88
// Thunk: copy arg to a stack local, then forward its address to func_800B2DB0.
void func_800B2D88(void* self, void* arg) {
    func_800B2DB0(self, &arg);
}

// Target 1: us-800b9bf4 - func_800B92D8
// Wrapper: fetch the singleton and forward it to func_800B15A4.
void func_800B92D8() {
    func_800B15A4(func_800B07E8());
}

// Target 4: us-800b9cec - func_800B93D0
// Wrapper: fetch the singleton and forward (singleton, obj) to func_800B1E18.
void func_800B93D0(void* obj) {
    func_800B1E18(func_800B07E8(), obj);
}

// Target 5: us-800b9d20 - func_800B9404
// Wrapper: fetch the singleton and forward (singleton, obj) to func_800B3A88.
void func_800B9404(void* obj) {
    func_800B3A88(func_800B07E8(), obj);
}

// Target 3: us-800b7978 - func_800B7058
// Wrapper: fetch the singleton and insert obj into it via func_800B6DD0.
void func_800B7058(void* obj) {
    func_800B6DD0(func_800B07E8(), obj);
}
extern "C" void func_800B7320() {}
extern "C" void func_800B7A18() {}
extern "C" void func_800B87FC() {}
// Target us-800b9120: func_800B8804(self, event)
// Ensure the reslist pool @ field_0xC80 is set up, then look for an existing
// node whose data pointer equals `event`; if none, claim the first empty
// pool slot (entry[0]==0) and insert a new node holding `event` before the
// sentinel (head).
extern "C" void func_800B8804__FPvPQ22cf13IFactoryEvent(UnkClass_805764CC* self, cf::IFactoryEvent* event) {
    // Decl order drives Chaitin coloring: idx=r4, byteOff=r5, count=r6,
    // sentinel=r7 (retail).  Values are assigned in execution order below.
    int idx;
    u32 byteOff;
    int count;
    CFactoryEventPoolNode* sentinel;
    if (event == 0) {
        return;
    }
    func_800B0FA0(self);

    // Walk the node list at field_0xC80; node next/prev/data links.
    sentinel = (CFactoryEventPoolNode*)(*(u32*)((u8*)self + 0xc84));
    CFactoryEventPoolNode* node = sentinel->next;
    while (node != sentinel) {
        if (node->data == event) {
            return; // already registered
        }
        node = node->next;
    }

    // Claim the first empty slot in the fixed 0xc-byte-entry pool.  Retail
    // re-reads the pool base from self->[0xc94] every slot check (aliasing),
    // and keeps an explicit running byte-offset (r5) alongside the slot index
    // (r4): `lwzx r0,r3,r5` + `addi r5,r5,12` + `addi r4,r4,1`.
    count = *(int*)((u8*)self + 0xc98);
    idx = 0;
    byteOff = 0;
    while (idx < count) {
        if (*(u32*)(*(u8**)((u8*)self + 0xc94) + byteOff) == 0) {
            break;
        }
        byteOff += 0xc;
        idx++;
    }

    // Fill the new node and link it in just before the sentinel.  The data
    // write mirrors reslist::_reslist_node::setItem: the `&data` pointer is
    // null-checked and wrapped in try/catch, which is what forces retail's
    // frame pointer (mr r31,r1) + stw-r1 frame-anchor + the addic. null check.
    CFactoryEventPoolNode* newEntry = (CFactoryEventPoolNode*)(*(u8**)((u8*)self + 0xc94) + idx * 0xc);
    cf::IFactoryEvent** ptr = &newEntry->data;
    if (ptr != nullptr) {
        try {
            *ptr = event;
        } catch (...) {
            throw;
        }
    }
    newEntry->next = sentinel;
    newEntry->prev = sentinel->prev;
    sentinel->prev->next = newEntry;
    sentinel->prev = newEntry;
}
// Target 1: us-800b9d54 - func_800B9438
// Fetch the singleton and forward (singleton, arg) to func_800B4278.
void func_800B9438(void* arg) {
    func_800B4278(func_800B07E8(), (u32)arg);
}
// Target 2: us-800b9d88 - func_800B946C
// Fetch the singleton and forward (singleton, arg) to func_800B42E8.
void func_800B946C(void* arg) {
    func_800B42E8(func_800B07E8(), (u32)arg);
}
// Target 3: us-800b9dbc - func_800B94A0
// Fetch the singleton and forward (singleton, name) to func_800B4368.
void func_800B94A0(const char* name) {
    func_800B4368(func_800B07E8(), name);
}
// Target 4: us-800b9e64 - func_800B9548
// Fetch the singleton and forward (singleton, 0x8000, 0, 0) to func_800B20B4.
void func_800B9548() {
    func_800B20B4(func_800B07E8(), 0x8000, 0, 0);
}
// Target 3: us-800b9ff4 - setter storing val at +0x720
void func_800B96D8(u8* self, u32 val) {
    *(u32*)(self + 0x720) = val;
}
