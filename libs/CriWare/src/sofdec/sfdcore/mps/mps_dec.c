// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mps/mps_dec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPSDEC_Init() {}

void MPSDEC_Finish() {}

typedef struct MPS_WORK MPS_WORK;
struct MPS_WORK {
    unsigned char pad[0xf4];
    void *pes_fn;
    void *pes_obj;
};

int MPSLIB_CheckHn(MPS_WORK*);

void MPS_SetSystemFn(MPS_WORK* hn, void* fn, void* obj) {
    if (MPSLIB_CheckHn(hn) == 0) {
        *(void**)((u8*)hn + 0xE4) = fn;
        *(void**)((u8*)hn + 0xE8) = obj;
    }
}

void MPS_SetPsMapFn(MPS_WORK* hn, void* fn, void* obj) {
    if (MPSLIB_CheckHn(hn) == 0) {
        *(void**)((u8*)hn + 0xEC) = fn;
        *(void**)((u8*)hn + 0xF0) = obj;
    }
}

void MPS_SetPesFn(MPS_WORK *hn, void *pes_fn, void *pes_obj) {
    if (MPSLIB_CheckHn(hn) == 0) {
        hn->pes_fn = pes_fn;
        hn->pes_obj = pes_obj;
    }
}

extern int MPSLIB_SetErr(void *, int);

typedef void (*mps_dechd_fn)(void *, void *, void *, void *, void *);

void MPS_DecHd(void *handle, void *arg1, void *arg2, void *out1, void *out2) {
    mps_dechd_fn fn;

    *(int *)out1 = 0;
    *(int *)out2 = 0;

    if (MPSLIB_CheckHn((MPS_WORK*)handle)) {
        MPSLIB_SetErr(NULL, 0xFF020301);
        return;
    }

    fn = *(mps_dechd_fn *)((u8 *)handle + 0xD4);
    fn(handle, arg1, arg2, out1, out2);
}

void MPSDEC_DecHdMpeg1() {}

void mpsdec_DecOneHd() {}

void mpsdec_DecPackHd() {}

void mpsdec_DecSysHd() {}

void mpsdec_DecPketHd() {}
