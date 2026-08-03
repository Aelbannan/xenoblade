// Auto-scaffolded catalog TU for CriWare/src/adx/lsc/lsc_err
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void LSC_EntryErrFunc(void *param1, void *param2) {
    extern void *lbl_eu_805E7C30;
    extern void *lbl_eu_805E7C34;
    if (param1 == NULL) {
        lbl_eu_805E7C30 = NULL;
        lbl_eu_805E7C34 = NULL;
    } else {
        lbl_eu_805E7C30 = param1;
        lbl_eu_805E7C34 = param2;
    }
}

extern void CRICRW_Vsprintf(char* dst, s32 size, const char* fmt, va_list ap);
extern void* lbl_eu_805E7C30;

void LSC_CallErrFunc_(const char* fmt, ...) {
    char *base = (char *)&lbl_eu_805E7C30;
    va_list ap;
    va_start(ap, fmt);
    CRICRW_Vsprintf(base + 8, 256, fmt, ap);
    va_end(ap);
    if (lbl_eu_805E7C30 != NULL) {
        ((void (*)(void *, char *))lbl_eu_805E7C30)(*(void **)(base + 4), base + 8);
    }
}
