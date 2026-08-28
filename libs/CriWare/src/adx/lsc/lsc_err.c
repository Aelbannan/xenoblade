// Auto-scaffolded catalog TU for CriWare/src/adx/lsc/lsc_err
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef struct {
    void *cb;
    void *param;
    char buf[0x100];
} LSC_ErrBlob;

void* lbl_eu_805E7C30;
unsigned char lbl_eu_805E7C34[0x104];

void LSC_EntryErrFunc(void *param1, void *param2) {
    if (param1 == NULL) {
        lbl_eu_805E7C30 = NULL;
        *(void**)lbl_eu_805E7C34 = NULL;
    } else {
        lbl_eu_805E7C30 = param1;
        *(void**)lbl_eu_805E7C34 = param2;
    }
}

extern void CRICRW_Vsprintf(char* dst, s32 size, const char* fmt, va_list ap);

void LSC_CallErrFunc_(const char* fmt, ...) {
    LSC_ErrBlob *e = (LSC_ErrBlob *)&lbl_eu_805E7C30;
    va_list ap;
    va_start(ap, fmt);
    CRICRW_Vsprintf(e->buf, 256, fmt, ap);
    va_end(ap);
    if (e->cb != NULL) {
        ((void (*)(void *, char *))e->cb)(e->param, e->buf);
    }
}
