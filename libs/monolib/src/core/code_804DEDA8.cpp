// Auto-scaffolded catalog TU for monolib/src/core/code_804DEDA8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include "monolib/core/CScheduleItem.hpp"
#include "monolib/core/code_804DEDA8.hpp"

void func_804DEDA8(void* r3) {
    char* base = (char*)r3;
    base[0] = 0;
    base[2] = 0;
    base[3] = 0;
    *(int*)(base + 4) = 0;
    *(int*)(base + 8) = 0;
    *(int*)(base + 0x10) = 0;
    *(int*)(base + 0xc) = 0;
}

void __dt__804DEDCC(){}

void __dt__804DF068(){}

void func_804DF118(){}

void func_804DF150(){}

void func_804DF164(){}

struct ResTableIndex {
    u32 offset;
    u32 size;
};

struct ResTable {
    u8 _00[8];
    u32 indexOffset;
};

void* func_804DF2A8(u8* self, int index) {
    ResTable* base = *(ResTable**)((u8*)self + 0x10);
    ResTableIndex* entries = (ResTableIndex*)((u8*)base + base->indexOffset);
    return (u8*)base + entries[index].offset;
}

void func_804DF2C4(){}

void func_804DF2F0(){}

void func_804DF344(){}

void func_804DF3D0(){}

void func_804DF4BC(){}

void func_804DF5F8(){}

void func_804DF690(){}

void __dla__FPv(void*);
void* lbl_eu_80665A30;
void* lbl_eu_80665A34;
void* lbl_eu_80665A38;
void* lbl_eu_80665A3C;

void __dt__804DF744() {
    void* p1 = lbl_eu_80665A34;
    lbl_eu_80665A30 = 0;
    if (p1) {
        __dla__FPv(p1);
        lbl_eu_80665A34 = 0;
    }
    void* p2 = lbl_eu_80665A38;
    if (p2) {
        __dla__FPv(p2);
        lbl_eu_80665A38 = 0;
    }
    lbl_eu_80665A3C = 0;
}

void func_804DF7A4(){}

void func_804DF7FC() { lbl_eu_80665A30 = 0; }

void func_804DF808(){}

void func_804DFA08(){}

void func_804DFA84(){}

// Release a schedule item by handle (idempotent for invalid handles).
extern "C" void func_804DFB88(s16 handle) {
    if (handle < 0 || lbl_eu_80661718.count <= handle) {
        return;
    }
    lbl_eu_80661718.lastHandle = handle;
    func_804E3E2C(&lbl_eu_80661718.base[handle]);
    if (lbl_eu_80661718.freeCount > 0) {
        lbl_eu_80661718.freeCount--;
    }
}

// Look up a schedule item by handle; returns NULL for invalid handles.
extern "C" CScheduleItem* func_804DFBF4(s16 handle) {
    if (handle < 0) {
        return NULL;
    }
    bool inRange = handle >= 0 && handle < lbl_eu_80661718.count;
    if (inRange) {
        return &lbl_eu_80661718.base[handle];
    }
    return NULL;
}

void func_804DFC48(){}

void func_804DFCC4(){}

void func_804DFE20(){}

extern "C" { extern char lbl_eu_80661728[]; }
extern "C" u32 func_804DFE8C(void) { return *(u32*)(lbl_eu_80661728 + 8); }

extern "C" u32 func_804DFE9C(void) { return *(u32*)(lbl_eu_80661728 + 4); }

void func_804DFEAC(){}

void func_804DFF00(){}

void func_804DFFA8(){}

void func_804E0098(){}

extern "C" { extern char lbl_eu_80661738[]; }
extern "C" u32 func_804E0104(void) { return *(u32*)(lbl_eu_80661738 + 4); }

void func_804E0114(){}

void func_804E0168(){}

void func_804E0188(){}

void func_804E0248(){}

void func_804E04D4(){}

void func_804E0580(){}

void func_804E06B4(){}

void func_804E0788(){}

void func_804E08BC(){}

void func_804E0990(){}

void func_804E0B94(){}

void func_804E0CF0(){}

void func_804E0E48(){}

void func_804E1044(){}

void func_804E1294(){}

void func_804E17A4(){}

void func_804E18CC(){}

void func_804E196C(){}

void func_804E1A44(){}

void func_804E1AA8(){}

void func_804E1C1C(){}

void func_804E1D50(){}

void func_804E2088(){}

void func_804E214C(){}

void func_804E24A8(){}

void func_804E26D8(){}

void func_804E2A5C(){}

void func_804E2B54(){}

void func_804E2D8C(){}

void func_804E2EAC(){}

void func_804E2F7C(){}

void func_804E30F0(){}
