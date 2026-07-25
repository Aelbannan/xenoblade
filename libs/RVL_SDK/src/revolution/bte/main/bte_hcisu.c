// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/main/bte_hcisu
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-802e0730
extern void GKI_freebuf(void*);

void bte_hcisu_send(void* buffer, uint16_t value) {
    *(uint16_t*)buffer = value;
    if (p_hcisu_if) {
        ((void (*)(void*, void*))(((uint32_t*)p_hcisu_if)[3]))(buffer, p_hcisu_if);
    } else {
        GKI_freebuf(buffer);
    }
}
// LLM-HARNESS-END: us-802e0730

// LLM-HARNESS-BEGIN: us-802e0750
void bte_hcisu_task() {}
// LLM-HARNESS-END: us-802e0750

// LLM-HARNESS-BEGIN: us-802e07c0
extern void* p_hcisu_if;
void bte_hcisu_close(void) {
    if (p_hcisu_if) {
        void (*close_func)(void) = *(void (**)(void))((char*)p_hcisu_if + 8);
        if (close_func) {
            close_func();
        }
    }
}
// LLM-HARNESS-END: us-802e07c0

// LLM-HARNESS-BEGIN: us-802e07f0
void bta_ci_hci_msg_handler() {}
// LLM-HARNESS-END: us-802e07f0
