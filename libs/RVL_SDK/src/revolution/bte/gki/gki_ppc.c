// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/gki/gki_ppc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void GKI_init() {}

void GKI_shutdown() {}

void GKI_run(void) {
}

void GKI_sched_lock(void) {
}

void GKI_sched_unlock(void) {
}

void GKI_delay(void) {
}

void GKI_send_event() {}

int GKI_get_taskid(void) { return 0x2; }

void GKI_enable() {}

void GKI_disable() {}

void GKI_exception(void) {}

void* GKI_os_malloc(size_t size) {
    extern void* App_MEMalloc(size_t);
    void* result = App_MEMalloc(size);
    if (result == 0) return 0;
    return result;
}

void GKI_os_free(void) { App_MEMfree(); }
