// Auto-scaffolded catalog TU for kyoshin/code_800B06A4
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/code_800B06A4.hpp"
#include <string.h>

// Forward declaration for func_800B3A88 (defined in this TU)
void func_800B3A88(UnkClass_805764CC* self, void* target);

void UnkClass_800B0AD8::clearCounters() {
    unkB00 = 0;
    unkAFC = 0;
}
UnkClass_805764CC* func_800B07E8();
u32 func_800AA2BC(u32 a, u32 b);
extern void func_80193810(u32);
extern void func_801F3CCC(u32);
extern void func_801F45B4(u32);
extern void func_802074F0(u32);
extern void func_80195E5C(float);

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
void func_800B06A4(float a) {
    extern float lbl_eu_80661CCC, lbl_eu_80661CD0;
    extern float lbl_eu_80663EC8, lbl_eu_80663ECC, lbl_eu_80663ED0, lbl_eu_80663ED4;
    float sq = (float)a;
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
void __ct__reslist_cf_IFactoryEvent(void* self) {
    extern void* lbl_eu_805290B8;
    extern void* lbl_eu_805290A0;
    u32* base = (u32*)self;
    u32 sentinelAddr = (u32)((u8*)self + 8);
    base[0] = (u32)&lbl_eu_805290B8;
    base[5] = 0;
    base[6] = 0;
    ((u8*)self)[0x1c] = 0;
    base[1] = sentinelAddr;
    ((u32*)self)[2] = sentinelAddr;
    ((u32*)self)[3] = sentinelAddr;
    base[0] = (u32)&lbl_eu_805290A0;
}
// Target 3: us-800b186c - func_800B0FA0
void func_800B0FA0(UnkClass_805764CC* self) {
    if (func_800B0FEC(&self->field_0xC80) == 0) {
        func_800B0FF4(&self->field_0xC80, func_80061FFC(), 4);
    }
}

void init_0FA0(){}
u32 UnkClass_805764CC::get_u32_18(){return *(u32*)((u8*)this + 0x18);}
void init_0FF4(){}
void init_dispatchTarget_1(){}
void __ct__cf_CfValueItemManager(){}
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
void func_800B93AC() {
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
void func_800B1A5C(void* list) {
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

void init_1A8C(){}
void* node_getDataPtr_alt(void* self){return (char*)(*(void**)self) + 8;}
void init_1AA8(){}
void copy_int_ptr_alt(int* dst, int* src){*dst = *src;}
void node_copyNextU32_alt(void* dst, void* src){*(unsigned long*)dst = *(unsigned long*)((char*)src + 4);}
void init_1AD8(){}
void init_1AF4(){}
void init_dispatchTarget_6(){}
void init_1BBC(){}
unsigned long gflag_getBit25(){extern unsigned long lbl_eu_80663EE0;return (lbl_eu_80663EE0 >> 25) & 1;}
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
void init_1E2C(){}
void init_1EB8(){}
void init_1EC8(){}
void init_1F2C(){}
void func_800B1F40(UnkClass_805764CC* self){if (self->field_0xCA0){func_80193810(self->field_0xCA0);}}

// Target 1: us-800b2820 - Check field_0xCA0; if nonzero, call func_80195E5C with float constant.
void func_800B1F54(UnkClass_805764CC* self) {
    if (self->field_0xCA0 == 0) return;
    extern float lbl_eu_80663EC8;
    func_80195E5C(lbl_eu_80663EC8);
}
void init_1F6C(){}
void init_1FD8(){}
void func_800B2034(UnkClass_805764CC* self){if (self->field_0xCFC){func_801F3CCC(self->field_0xCFC);}}
void func_800B2048(UnkClass_805764CC* self){if (self->field_0xCFC){func_801F45B4(self->field_0xCFC);}}
// TEST_FUNC_205C
void func_800B20A0(UnkClass_805764CC* self){if (self->field_0xCAC){func_802074F0(self->field_0xCAC);}}
void init_20B4(){}
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
    while (idx < count) {
        u32* entry = (u32*)(entryBase + byteOff);
        if (entry[0] == 0) {
            break;
        }
        idx++;
        byteOff += 0xC;
    }

    // Calculate entry pointer
    u32* newEntry = (u32*)(entryBase + idx * 0xC);

    // Copy data word into entry[8]
    *(u32*)((u8*)newEntry + 8) = *(u32*)data;

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
extern "C" void func_800B4368(UnkClass_805764CC* self, const char* name) {
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
            func_800B2D88((u8*)self + 0xC28);
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
void func_800B5978(UnkClass_805764CC* self, u32* val) {
    u32 idx = *(u32*)((u8*)self + 0x380);
    ((u32*)self)[idx] = *val;
    *(u32*)((u8*)self + 0x380) = idx + 1;
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
int func_800B67CC(void* self) {
    u8 val = *(u8*)((u8*)self + 2);
    return (val >= 1 && val <= 24) ? 1 : 0;
}
void* UnkClass_805764CC::getPtr_1A8(){return (void*)((u8*)this + 0x1a8);}
void UnkClass_805764CC::clear_700(){*(u32*)((u8*)this + 1792) = 0;}
void init_6800(){}
void init_68A8(){}
void init_6AF4(){}
extern "C" void func_800B6BA0(){}
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
int func_800B7680(void* self) {
    void* head = *(void**)((u8*)self + 4);
    void* cur = *(void**)head;
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
// Remove nodes matching a given ID from a linked list at offset 0xC84
void func_800B88E0(u8* self, u32 targetId) {
    u32* head = *(u32**)(self + 0xC84);
    u32* sentinel = head;
    u32* node = (u32*)*head;
    while (node != sentinel) {
        u32* next = (u32*)*node;
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
}
// Target: us-800b10b4 - func_800B07E8 (singleton sinit: init once, then return &singleton)
UnkClass_805764CC* func_800B07E8() {
    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }
    return (UnkClass_805764CC*)lbl_eu_80572CD4;
}
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
        func_80193CD0(ca0, self);
        result = ca0;
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
void sinit_800B9A40() {
    extern float lbl_eu_80661CC8, lbl_eu_80661CCC, lbl_eu_80661CD0;
    extern float lbl_eu_80663EC8, lbl_eu_80663ECC, lbl_eu_80663ED0, lbl_eu_80663ED4;
    extern u32 lbl_eu_80663EE0;
    float f3 = lbl_eu_80661CC8;
    float f0 = lbl_eu_80661CCC;
    float f1 = lbl_eu_80661CD0;
    float f2 = f3 * f3;
    float f0_sq = f0 * f0;
    lbl_eu_80663EC8 = f3;
    lbl_eu_80663ECC = f2;
    lbl_eu_80663ED0 = f1;
    lbl_eu_80663ED4 = f0_sq;
    lbl_eu_80663EE0 = 0;
}

extern "C" void func_800B14D4() {}
extern "C" void func_800B14FC() {}
extern "C" bool func_800B2D28() { return false; }
extern "C" void func_800B31D4() {}
extern "C" void func_800B3D34() {}
extern "C" void func_800B3D40() {}
extern "C" void func_800B4A24() {}
extern "C" void func_800B4F6C() {}
extern "C" bool func_800B4FA4() { return false; }
extern "C" void func_800B64D0() {}
extern "C" void func_800B7058() {}
extern "C" void func_800B7320() {}
extern "C" void func_800B7A18() {}
extern "C" void func_800B87FC() {}
extern "C" void func_800B8804__FPvPQ22cf13IFactoryEvent() {}
extern "C" void func_800B92D8() {}
extern "C" void func_800B93D0() {}
extern "C" void func_800B9404() {}
extern "C" void func_800B9438() {}
extern "C" void func_800B946C() {}
extern "C" void func_800B94A0() {}
extern "C" void func_800B9548() {}
extern "C" bool func_800B96D8() { return false; }
