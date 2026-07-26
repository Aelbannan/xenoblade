// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/srcwii/adx_mwii
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADXM_WaitVsync(void) {}

void ADXM_ExecMain(void) {}

void ADXM_Lock(void) { SVM_Lock(); }

void ADXM_Unlock(void) { SVM_Unlock(); }

void adxm_lock() {}

void adxm_unlock() {}

void adxm_goto_mwidle_border() {}

void adxm_safe_proc(void) {
    extern int lbl_eu_805F3A50[];
    int *base = (int *)lbl_eu_805F3A50;
    while (*(int *)((char *)base + 0x9d0) == 1) {
        (*(int *)((char *)base + 0x48))++;
    }
    *(int *)((char *)base + 0x9d4) = 1;
}

void adxm_vsync_proc() {}

void adxm_fs_proc() {}

void adxm_mwidle_proc() {}

void ADXM_SetCbErr(void) { SVM_SetCbErr(); }

void adxm_create_base_thread() {}

void ADXM_SetupThrd() {}

extern u32 lbl_eu_805F3A54;
u32 ADXM_IsSetupThrd(void) {
    return (lbl_eu_805F3A54 != 0) ? 1 : 0;
}

void ADXM_ShutdownThrd() {}

void ADXM_SetupFramework() {}

void ADXM_ShutdownFramework() {}
