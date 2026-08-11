// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemModel
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnItemModel.hpp"

void func_804970D0(u8* self);
void func_8048472C(u8* self) { ((void(*)(void*))func_804970D0)((char*)self + 0xc); }

void func_80482DF4(){}

void func_804830AC(){}

void func_804830BC(){}

void func_804830E4(){}

void func_8048310C(){}

void func_80483134(){}

void func_8048315C(){}

void func_804831C4(){}

void func_80483448(){}

void func_804838DC(){}

void func_804839D4(){}

void func_80484164(){}

void func_804842B0(){}

void func_804844D0(){}

void func_80484734(){}

void func_80484838(){}

float func_8048490C(u8* self) { return ((CScnItemModel*)self)->value858; }

void func_80484914(){}

void func_804849E4(){}

void func_80484AB4(){}

void func_80484BB4(){}

void func_80484C84(){}

void func_80484D3C(){}

void func_80484E04(u8* self, u32 val) { ((CScnItemModel*)self)->value7E8 = val; }

void func_80484E0C(void) {}

extern "C" void func_80484E10(u8* self, u32 a, u32 b) {
    volatile u32* f = (volatile u32*)((u8*)self + 0x7A8);
    if (a != 0) {
        *f |= 0x1;
    } else {
        *f &= ~0x1;
    }
    if (b != 0) {
        *f |= 0x2;
    } else {
        *f &= ~0x2;
    }
}

void func_80484E5C(){}

void func_80484EB0(){}

void func_80484F18(){}

void func_80484F80(){}

void func_804850A4(){}

void func_8048510C(){}

void func_80485174(){}

void func_804851DC(){}

void func_80485244(){}

void func_80485464(){}

void func_80485684(){}

void func_80485774(){}

extern "C" void func_804857DC(u8* self, u32 param) {
    *(u32*)((u8*)self + 0x7A4) |= 0x20;
    *(u32*)((u8*)self + 0x7C8) = param;
}

extern "C" void func_804857F0(u8* self, u32 param) {
    *(u32*)((u8*)self + 0x7A4) |= 0x100;
    *(u32*)((u8*)self + 0x7C8) = param;
}

void func_80485804(){}

void func_804858C8(){}

void func_80485994(){}

void func_804859E8(){}

void func_80485A48(){}

void func_80485AD8(){}

void func_80485B98(){}

void func_80485C28(){}

void func_80485CE8(){}

void func_80485D64(){}

extern "C" void func_80489A60(void* a, void* b, void* c, void* d, void* e, int f);
extern "C" void func_80486068(u8* self, void* b, void* c, void* d, void* e) { func_80489A60(self, b, c, d, e, 102); }

extern "C" void func_80489C94(void* self);
extern "C" void func_80486070(void* self) { func_80489C94(self); }

void func_eu_8048A084(){}

extern "C" int func_80486074(u8* self) { return 0; }

extern "C" int func_8048607C(u8* self) { return 1; }

extern "C" void func_80486084(void) {}

extern "C" int func_80486088(u8* self) { return 0; }

extern "C" u32 func_80486090(u8* self) { return ((CScnItemModel*)self)->value7E4; }

extern "C" void func_804828F0(u8* self, u32 flags, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0) |= flags;
    } else {
        *(u32*)((u8*)self + 0) &= ~flags;
    }
}

extern "C" void func_804830BC(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A4) |= 0x200000;
    } else {
        *(u32*)((u8*)self + 0x7A4) &= ~0x200000;
    }
}

extern "C" void func_804830E4(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A8) |= 0x100;
    } else {
        *(u32*)((u8*)self + 0x7A8) &= ~0x100;
    }
}

extern "C" void func_8048310C(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A4) |= 0x08000000;
    } else {
        *(u32*)((u8*)self + 0x7A4) &= ~0x08000000;
    }
}

extern "C" void func_80483134(u8* self, u32 enable) {
    if (enable != 0) {
        *(u32*)((u8*)self + 0x7A4) |= 0x02000000;
    } else {
        *(u32*)((u8*)self + 0x7A4) &= ~0x02000000;
    }
}

extern "C" void func_804827DC() {}
extern "C" void func_80482918() {}
extern "C" void func_804829E8() {}
extern "C" void func_80482AD4() {}
extern "C" void func_80482B3C() {}
