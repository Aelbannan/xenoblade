// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_link
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADXT_AttachAhx() {}

extern void (*lbl_eu_805E4F20)(void);
void ADXT_DetachAhx(void) {
    if (lbl_eu_805E4F20 != NULL)
        (*lbl_eu_805E4F20)();
}

void AHXLINK_DetachAhx() {}

void AHXLINK_SetDecSmpl(u32 val) { AHXSJD_SetDecSmpl(val); }

void AHXLINK_TermSupply(void) { AHXSJD_TermSupply(); }

void AHXLINK_ExecOneAhx() {}
