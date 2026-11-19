// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_cmc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include "libs/CriWare/src/sofdec/sfdcore/mpv/mpv_types.h"

void MPVMC08_Init(u32* dst);
void MPVMC16_Init(void* this_ptr);
void mpvcmc_InitMcOiTa(MpvHn* self);

void MPVCMC_InitObj(void* self) {
    u8* base = (u8*)self;
    MPVMC08_Init((u32*)(base + 0xA4C));
    MPVMC16_Init(base + 0xA4C);
    mpvcmc_InitMcOiTa(self);
}

void mpvcmc_InitMcOiTa(MpvHn* hn) {
    s32 flags = 4;
    MpvMcRow* rows = hn->mcOiTa;
    u8* work = hn->mcWork;
    int i;
    if (hn->cfgImg[5] == 0) {
        flags = -1;
    }
    hn->mcOiTaCount = flags;
    for (i = 0; i < 6; i++) {
        rows[i].base = work;
    }
    for (i = 0; i < 6; i++) {
        rows[i].stride = 8;
    }
}

void MPVCMC_InitMcOiRt(MpvHn* hn) {
    s32 i;
    s32* e = hn->mcRows;
    hn->mcRowCount = (hn->cfgImg[5] == 0) ? -1 : 4;
    for (i = 0; i < 2; i++) { e[i * 2 + 1] = hn->outStride; }
    for (i = 2; i < 6; i++) { e[i * 2 + 1] = hn->outChromaStride; }
}

void MPVCMC_SetCcnt(MpvHn* hn) {
    s32 r4 = 4;
    if (hn->cfgImg[5] == 0) {
        r4 = -1;
    }
    hn->mcOiTaCount = r4;
    hn->mcRowCount = r4;
}
