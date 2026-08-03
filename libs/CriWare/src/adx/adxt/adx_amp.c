// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_amp
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADXAMP_Destroy(void* p) {
    if (!p) return;
    ADXCRS_Lock();
    memset(p, 0, 0x30);
    ADXCRS_Unlock();
}

void ADXAMP_Start(void* self) {
    u8* p;
    s32 i;
    for (i = 0; i < (s8)*(u8*)((u8*)self + 2); i++) {
        *(u32*)((u8*)self + 0x14 + i * 4) = 0;
    }
    *(u32*)((u8*)self + 0x2c) = 0;
    p = (u8*)self;
    for (i = 0; i < (s8)*(u8*)((u8*)self + 2); i++) {
        void* x;
        void* y;
        s32 sp[2];
        x = *(void**)((u8*)p + 4);
        y = *(void**)((u8*)x + 0);
        ((void (*)(void*))*(void**)((u8*)y + 0x14))(x);
        ((void (*)(void*, s32))*(void**)((u8*)y + 0x24))(x, 0);
        ((void (*)(void*, s32, s32*))*(void**)((u8*)y + 0x18))(x, 0, sp);
        memset((void*)sp[0], 0, sp[1]);
        ((void (*)(void*, s32, s32*))*(void**)((u8*)y + 0x1c))(x, 0, sp);
        p += 4;
    }
    *(u8*)((u8*)self + 1) = 2;
}

void ADXAMP_Stop(void *self) { *(u8*)((u8*)self + 1) = 0; }

void ADXAMP_SetSfreq(void* self, u32 val) { *(u32*)((u8*)self + 0x20) = val; }
