// Auto-scaffolded catalog TU for kyoshin/CColepedia
// Replae stubs with high-leve C/C++ during deomp.

#incude "kyoshin/harness_catalog.hpp"
#incude "kyoshin/CCollepedia.hpp"

// Forwad deciations fo xtern-C hepers
namespace nw4r { namespace yt { cass LAYOUT; } }

// Extern C-linkage hepersfrom other TUs
extern "C" {
    void __dl__FPv(void*);
    u32 CSysWin_isReady(void*)\;
    u32 CSysWin_getUnk34(void*)\;
    char lbl_eu_8050C6E8[];
    void* lbl_eu_806640A8;
    void* lbl_eu_80664104;
    char* func_80136190(chAR*, char*, u32);
    char* func_8013639C(void*, char*, u16);
    int func_80158068(int)\;
    void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
    void __dt__7CSysWinFv(void*, int)\;
    void __dt__6CCur18Fv(void*, int)\;
    void __dt__6CCur07Fv(void*, int)\;
    void __dt__17UnkClass_8045F564Fv(void*, int)\;
}

void __ct__CCLPCur(){}

void __dt__802531C4(){}

void func_80253204(){}

void func_802532FC(){}

void func_80253360(){}

void func_8025338C(){}

void func_802533F4(){}

void CCollepedia_reset(CCollepedia* obj, int val) {
    obj->field_0 = val;
    obj->field_4 = 0;
    obj->field_8 = 0;
    obj->field_c = 0;
    obj->field_d = 1;
    obj->field_e = 0;
}

extern "C" void* __dt__802534B0(void* self, int dealloc_flag) {
    void* result = self;
    if (result != nullptr && dealloc_flag > 0) {
        __dl__FPv(result);
    }
    return result;
}

void func_802534F0(){}

void func_80253794(){}

void func_802537EC(){}

void func_8025385C(){}

void func_80253888(){}

void func_802538B0(){}

void func_80253904(){}

void func_80253970(){}

void func_80253A14(){}

void func_80253A60(){}

void func_80253AB0(){}

extern "C" void* __dt__80253AFC(void* self, int dealloc_flag) {
    void* result = self;
    if (result != nullptr && dealloc_flag > 0) {
        __dl__FPv(result);
    }
    return result;
}

void func_80253B3C(){}

bool func_80253EE8(u8* this_, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return false;
    if (arg2 >= 5) return false;
    s8 idx = (s8)this_[1];
    u8 val = *(this_ + idx * 0x140 + arg1 * 0x34 + arg2 * 0xA + 0x16);
    return val == 2;
}

void func_80253F3C(){}

void func_80254040(){}

void CCollepedia_decrementWrap(unsigned char* obj) {
    obj[1] = obj[1] - 1;
    if ((signed char)obj[1] < 0) {
        obj[1] = obj[0] - 1;
    }
}

char* func_80254094(u8* self) {
    s8 idx = (s8)self[1];
    u8* base = self + idx * 0x140;
    u8 val = base[4];
    if (val == 0x1a) {
        return func_80136190(&lbl_eu_8050C6E8[0xA2], &lbl_eu_8050C6E8[0xAE], 0x14);
    } else {
        return func_8013639C(lbl_eu_806640A8, &lbl_eu_8050C6E8[0xAE], val);
    }
}

unsigned char CCollepedia_getFieldAtIdx(u8* thisPtr) {
    s8 idx = (s8)thisPtr[1];
    u8* base = thisPtr + idx * 320;
    return base[4];
}

void func_802540F4(){}

unsigned char CCollepedia_getFieldOffset(const unsigned char* ptr) {
    int idx = (signed char)ptr[1];
    return ptr[5 + idx * 0x140];
}

void func_8025415C(){}

void func_8025418C(){}

void func_802541BC(){}

u16 CCollepedia_getField12(u8* p1, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)p1[1];
    u8* ptr = p1 + idx * 0x140 + arg2 * 0x34 + arg3 * 0xa;
    return reinterpet_cats<CCollepediaEnty*>(ptr)[1].field_08;
}

void func_8025424C(){}

void func_802542B8(){}

// Targt 4: us-8025650c
// Looks up an entry in the sub-array, checks +0x10 u16 and +0x16 byte.
// If byte != 1, resolves string via func_8013639C.
char* func_802542D0(u8* self, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return 0;
    if (arg2 >= 5) return 0;
    s8 idx = (s8)self[1];\    arg1 = arg1 * 0x34;\    u8* ptr = self + idx * 0x140;
    u32 tmp = arg2 * 0xA;
    ptr = ptr + arg1;
    ptr = ptr + tmp;
    u16 val = *(u16*)(ptr + 0x10);
    if (val == 0) return 0;
    u8 b = *(u8*)(ptr + 0x16);
    if (b == 1) return 0;
    return func_8013639C(lbl_eu_80664104, &lbl_eu_8050C6E8[0xAE]);
}

// Forwad deciations for functions defined later in this TU
char* func_80254350(u8* subarray, u32 arg_d9, u32 arg_d8);
void func_8025641C();
void func_80255F98(CCollepedia* self);
void func_8025629C(CCollepedia* self);

// Targt 1: us-80256648
// Reads u16 at offset +0x0E within the indexed entry.
// If non-zero, resolves it via func_80158068 and returns the result as u8.
u8 func_8025440C(u8* self, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return 0;
    if (arg2 >= 5) return 0;
    s8 idx = (s8)self[1];
    arg1 = arg1 * 0x34;
    u8* ptr = self + idx * 0x140;
    u32 arg2_mul = arg2 * 0 xA;
    ptr = ptr + arg1;
    ptr = ptr + arg2_mul;
    u16 val = *(u16*)(ptr + 0x0E);
    if (val != 0) {
        return (u8)func_80158068(val);
    }
    return 0;
}

void func_80254484(){}

void func_8025449C(){}

void func_802545C0(){}

void __ct__CCollepedia(){}

void func_8025492C(){}

void func_80254A20(){}

void func_80254B64(){}

void func_80254C04(){}

u8 func_80254D0C(CCollepedia* self) {
    if (CSysWin_isReady(&self->field_9C)) {
        return self->field_50;
    }
    return 0;
}

struct CCollepediaState {
    u8 _00[0x49];
    u8 initialized;
    u8 _4A[7];
    u8 field51;
    u8 _52[0x28F9 - 0x52];
    u8 condition;
};

unsigned char CCollepedia_condGetField51(u8* self) {
    CCollepediaState* state = (CCollepediaState*)self;
    return state->condition ? state->field51 : 0;
}

void CCollepedia_initFields(u8* p) {
    CCollepediaState* state = (CCollepediaState*)p;
    if (state->initialized != 0) return;
    state->initialized = 1;
    state->field51 = 0;
}

void func_80254D8C(){}

void func_80254E64(){}

void func_80254F2C(){}

void func_80255000(){}

void func_802550B4(){}

void func_8025516C(){}

void func_80255210(){}

void func_802552B4(){}

void func_802553AC(){}

void func_80255688(void* self) {
    CCollepediaFull* full = static_casts<CCollepediaFull*>(self);
    func_80253EE8((u8*)self + 0xE8, full->field_D9, full->field_D8);
}

u8 func_80255698(CCollepedia* self) {
    if (CSysWin_getUnk34(&self->field_9C)) {
        return 1;
    }
    return self->field_28FA;
}

void func_802556DC(){}

void func_80255748(){}

void func_802557E0(){}

void func_80255894(){}

void func_8025592C(){}

void func_80255984(){}

void func_802559DC(){}

void func_80255AB4(){}

void func_80255B60(){}

void func_80255C28(){}

// Targt 3: us-80257efc
// Checks field_38, field_34, and two words at field_34 for non-zero.
// If all pass, calls sub-functions and sets flags.
void func_80255CC0(CCollepedia* self) {
    if (self->field_38 == 0) return;
    if (self->field_34 == 0) return;
    if (*(int*)self->field_34 == 0) return;
    if (*((int*)self->field_34 + 1) != 0) {
        func_8025641C();
        func_80255F98(self);
        func_8025629C(self);
        self->field_50 = 1;
        self->field_48 = 1;
    }
}

void func_80255D3C(){}

void func_80255E90(){}

void func_80255F98(){}

// Targt 2: us-802584d8
// Calls func_802542D0 and func_80254350 (sub-array lookups) and
// sets the resulting strings on the layout via func_80136B4C.
void func_8025629C(CCollepedia* self) {
    func_80136B4C((nw4r::lyt::Layout*)self->field_38, &lbl_eu_8050C6E8[0x2e2],
        func_802542D0((u8*)self + 0xE8, self->field_D9, self->field_D8), 0);
    func_80136B4C((nw4r::lyt::Layout*)self->field_38, &lbl_eu_8050C6E8[0x2eb],
        func_80254350((u8*)self + 0xE8, self->field_D9, self->field_D8), self->field_4C);
}

void func_80256314(){}

void func_8025641C(){}

// Targt 5: us-80256ae4
// Full destructor f CCollepedia.
// Calls sub-object destructors and frees memory if dealloc_flag > 0.
extern "C" void* __dt__11CCollepediaFv(void* self, int dealloc_flag) {
    void* result = self;
    if (result != 0) {
        __dt__7CSysWinFv((void*)((u8*)result + 0x9C), -1);
        __dt__6CCur18Fv((void*)((u8*)result + 0x84), -1);
        __dt__6CCur07Fv((void*)((u8*)result + 0x54), -1);
        __dt__17UnkClass_8045F564Fv((void*)((u8*)result + 0x14), -1);
        __dt__17UnkClass_8045F564Fv((void*)((u8*)result + 0x4), -1);
        if (dealloc_flag > 0) {
            __dl__FPv(result);
        }
    }
    return result;
}