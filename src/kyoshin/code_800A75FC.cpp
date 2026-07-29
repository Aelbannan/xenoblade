// Auto-scaffolded catalog TU for kyoshin/code_800A75FC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// Forward declarations for external functions in other TUs
void func_800A37CC(void*, void*);
int func_800A3594(void*, int, int);
int func_800A36A4(void*, int, int);
u32 func_800A3998(void*);
int func_800A39E8(void*, void*, void*, void*);

// Structure for range data at lbl_eu_80572B38 (offset 0 = start, offset 0xC = size)
struct RangeData {
    u32 field_0x00;
    u8 _pad_0x04[8];
    u32 field_0x0C;
};

// BSS symbols for the global singleton at lbl_eu_80572B94
extern char lbl_eu_80572B94[];
extern char lbl_eu_80572B88[];
extern s8 lbl_eu_80663E98;


void __dt__800A75FC(){}

void func_800A76EC(){}

void func_800A7CDC(){}

void func_800A7D9C(){}

void func_800A7EFC(){}

void func_800A7FBC(){}

void func_800A807C(){}

void func_800A813C(){}

void func_800A81FC(){}

void func_800A82BC(){}

void func_800A837C(){}

void func_800A843C(){}

void func_800A84FC(){}

void func_800A85D8(){}

void* func_800A86AC(unsigned int param1, unsigned int param2) {
    extern void* lbl_eu_805282A0[];
    void* r4 = lbl_eu_805282A0[param2];
    if (param1 >= 0xe) {
        return *(void**)r4;
    }
    return ((void**)r4)[param1];
}

void* func_800A86D8(unsigned int param1, unsigned int param2) {
    extern void* lbl_eu_80528398[];
    void** r4 = static_cast<void**>(lbl_eu_80528398[param2]);
    if (param1 >= 0xe) {
        return r4[0];
    }
    return r4[param1];
}

void func_800A8704(){}

void func_800A87E0(){}

void func_800A88C8(){}

void func_800A89C0(){}

void func_800A8AAC(){}

int func_800A8B98(int param) {
    if (param == 0) return 0;
    if (param > 0) {
        extern char lbl_eu_80572B38[];
        return func_800A3594((void*)lbl_eu_80572B38, param, 0);
    } else {
        extern char lbl_eu_80572B38[];
        return func_800A36A4((void*)lbl_eu_80572B38, -param, 0);
    }
}

int func_800A8BD8(u32 param) {
    if (param == 0) return 0;
    extern char lbl_eu_80572B38[];
    u32 start = *(u32*)(lbl_eu_80572B38);
    u32 size = *(u32*)(lbl_eu_80572B38 + 0xC);
    u32 end = start + size;
    int result = 0;
    if (start <= param && param < end) {
        result = 1;
    }
    return result;
}

int func_800A8C1C(void* p1, void* p2, void* p3) {
    if (p1 == 0) return 0;
    extern char lbl_eu_80572B38[];
    func_800A39E8((void*)lbl_eu_80572B38, p1, p2, p3);
    return 1;
}

void func_800A8C68(void* p) {
    if (p == 0) return;
    extern char lbl_eu_80572B38[];
    func_800A37CC((void*)lbl_eu_80572B38, p);
}

void func_800A8C84(void) {
    extern void func_800A3940(void*);
    extern char lbl_eu_80572B38[];
    func_800A3940((void*)lbl_eu_80572B38);
}

u32 func_800A8C90() {
    extern char lbl_eu_80572B38[];
    return func_800A3998((void*)lbl_eu_80572B38) / 774144;
}

void func_800A8CD4(){}

u32 func_800A8DA4(){ return 0x500000; }

void func_800A8DAC(){}

void func_800A8E6C(){}

int func_800A9024(u32 param) {
    if (param == 0) return 0;
    extern char lbl_eu_80572B78[];
    u32 start = *(u32*)(lbl_eu_80572B78);
    u32 size = *(u32*)(lbl_eu_80572B78 + 0xC);
    u32 end = start + size;
    int result = 0;
    if (start <= param && param < end) {
        result = 1;
    }
    return result;
}

void func_800A9068(){}

void func_800A9134(){}

int func_800A92F8(void* p1, void* p2, void* p3) {
    if (p1 == 0) return 0;
    extern char lbl_eu_80572B78[];
    func_800A39E8((void*)lbl_eu_80572B78, p1, p2, p3);
    return 1;
}

extern char lbl_eu_80572B78[];
void func_800A37CC(void*, void*);

void func_800A9344(void* p)
{
    if (p != 0) {
        func_800A37CC(lbl_eu_80572B78, p);
    }
}

void func_800A9360(){}

void func_800A9444(){}

void func_800A9534(){}

void func_800A965C(){}

void func_800A9784(){}

void func_800A98A8(){}

void func_800A99D0(){}

// --- Global singleton struct at lbl_eu_80572B94 (0xB4 bytes, BSS) ---
struct GlobalStruct_80572B94 {
    int field_0x00;   // -1
    int field_0x04;   // -1
    int field_0x08;   // -1
    u8 _0C[4];
    int field_0x10;   // 0
    int field_0x14;   // 0
    int field_0x18;   // 0
    int field_0x1C;   // 0
    int field_0x20;   // 0
    u8 _24[0x34 - 0x24];
    int field_0x34;   // 0
    int field_0x38;   // 0
    int field_0x3C;   // 0
    int field_0x40;   // 0 (BSS)
    int field_0x44;   // 0 (BSS)
    u8 _48[0x50 - 0x48];
    int field_0x50;   // 0
    u8 _54[0x5C - 0x54];
    u8 _5C[0x34];      // memset 0 (0x5C-0x90)
    u8 _90[0xA0 - 0x90];
    u8 _A0[0xC];       // memset 0 (0xA0-0xAC)
    u8 _AC[0xB0 - 0xAC];
    int field_0xB0;   // 0
};

void* memset(void*, int, unsigned long);
void* __register_global_object(void* object, void* destructor, void* registration);

int func_800A9A90() {
    if (lbl_eu_80663E98 == 0) {
        GlobalStruct_80572B94* g = (GlobalStruct_80572B94*)lbl_eu_80572B94;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(g->_A0, 0, 0xC);
        memset(g->_5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return ((GlobalStruct_80572B94*)lbl_eu_80572B94)->field_0x50;
}

int func_800A9B50() {
    if (lbl_eu_80663E98 == 0) {
        GlobalStruct_80572B94* g = (GlobalStruct_80572B94*)lbl_eu_80572B94;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(g->_A0, 0, 0xC);
        memset(g->_5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return ((GlobalStruct_80572B94*)lbl_eu_80572B94)->field_0x38;
}

int func_800A9C10() {
    if (lbl_eu_80663E98 == 0) {
        GlobalStruct_80572B94* g = (GlobalStruct_80572B94*)lbl_eu_80572B94;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(g->_A0, 0, 0xC);
        memset(g->_5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return ((GlobalStruct_80572B94*)lbl_eu_80572B94)->field_0x3C;
}

int func_800A9CD0() {
    if (lbl_eu_80663E98 == 0) {
        GlobalStruct_80572B94* g = (GlobalStruct_80572B94*)lbl_eu_80572B94;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(g->_A0, 0, 0xC);
        memset(g->_5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return ((GlobalStruct_80572B94*)lbl_eu_80572B94)->field_0x40;
}

int func_800A9D90() {
    if (lbl_eu_80663E98 == 0) {
        GlobalStruct_80572B94* g = (GlobalStruct_80572B94*)lbl_eu_80572B94;
        g->field_0x00 = -1;
        g->field_0x04 = -1;
        g->field_0x08 = -1;
        g->field_0x10 = 0;
        g->field_0x20 = 0;
        g->field_0x14 = 0;
        g->field_0x18 = 0;
        g->field_0x1C = 0;
        g->field_0x34 = 0;
        g->field_0xB0 = 0;
        g->field_0x38 = 0;
        g->field_0x3C = 0;
        g->field_0x50 = 0;
        memset(g->_A0, 0, 0xC);
        memset(g->_5C, 0, 0x34);
        __register_global_object(g, (void*)__dt__800A75FC, (void*)lbl_eu_80572B88);
        lbl_eu_80663E98 = 1;
    }
    return ((GlobalStruct_80572B94*)lbl_eu_80572B94)->field_0x44;
}

void func_800A9E50(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_800A9F40(){}
