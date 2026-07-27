// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/main/bte_hcisu
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void GKI_freebuf(void*);

void bte_hcisu_send(void* buffer, uint16_t value) {
    *(uint16_t*)buffer = value;
    if (p_hcisu_if) {
        uint32_t* p_if = (uint32_t*)p_hcisu_if;
        void (*func)(void*, void*) = (void (*)(void*, void*))p_if[3];
        func(buffer, p_hcisu_if);
    } else {
        GKI_freebuf(buffer);
    }
}

void bte_hcisu_task() {}

extern void* p_hcisu_if;
void bte_hcisu_close(void) {
    if (p_hcisu_if) {
        void (*close_func)(void) = *(void (**)(void))((char*)p_hcisu_if + 8);
        if (close_func) {
            close_func();
        }
    }
}

void bta_ci_hci_msg_handler() {}
