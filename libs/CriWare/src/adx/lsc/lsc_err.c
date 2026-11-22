// Auto-scaffolded catalog TU for CriWare/src/adx/lsc/lsc_err
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void (*lsc_err_func)(void*, char*);
void* lsc_err_obj;
char lsc_err_msg[0x100];

void LSC_EntryErrFunc(void (*func)(void*, char*), void* obj) {
    if (func == NULL) {
        lsc_err_func = NULL;
        lsc_err_obj = NULL;
    } else {
        lsc_err_func = func;
        lsc_err_obj = obj;
    }
}

extern void CRICRW_Vsprintf(char* dst, s32 size, const char* fmt, va_list ap);

void LSC_CallErrFunc_(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    CRICRW_Vsprintf(lsc_err_msg, 256, fmt, ap);
    va_end(ap);
    if (lsc_err_func != NULL) {
        lsc_err_func(lsc_err_obj, lsc_err_msg);
    }
}
