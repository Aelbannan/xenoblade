// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_link
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-8038d918
void ADXT_AttachAhx() {}
// LLM-HARNESS-END: us-8038d918

// LLM-HARNESS-BEGIN: us-8038da7c
extern void (*lbl_eu_805E4F20)(void);
void ADXT_DetachAhx(void) {
    if (lbl_eu_805E4F20 != NULL)
        (*lbl_eu_805E4F20)();
}
// LLM-HARNESS-END: us-8038da7c

// LLM-HARNESS-BEGIN: us-8038da98
void AHXLINK_DetachAhx() {}
// LLM-HARNESS-END: us-8038da98

// LLM-HARNESS-BEGIN: us-8038db20
void AHXLINK_SetDecSmpl(void) {}
// LLM-HARNESS-END: us-8038db20

// LLM-HARNESS-BEGIN: us-8038db24
void AHXLINK_TermSupply(void) {}
// LLM-HARNESS-END: us-8038db24

// LLM-HARNESS-BEGIN: us-8038db28
void AHXLINK_ExecOneAhx() {}
// LLM-HARNESS-END: us-8038db28
