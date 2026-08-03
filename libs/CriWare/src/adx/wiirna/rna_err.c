// Auto-scaffolded catalog TU for CriWare/src/adx/wiirna/rna_err
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef struct {
    u32 errfunc;
    u32 errarg;
    u8 buf[255];
} RNAERR_Globals;
extern RNAERR_Globals lbl_eu_805F2AF8;
void RNAERR_EntryErrFunc(void* a, void* b) {
    lbl_eu_805F2AF8.errfunc = (u32)a;
    lbl_eu_805F2AF8.errarg = (u32)b;
}

char* strncpy(char* dst, const char* src, size_t n);
void RNAERR_CallErrFunc(const char* msg) {
    register u32* g = (u32*)&lbl_eu_805F2AF8;
    void (*cb)(u32, char*);
    strncpy((char*)&lbl_eu_805F2AF8 + 8, msg, 0xFF);
    cb = (void (*)(u32, char*))g[0];
    if (cb) {
        cb(g[1], (char*)(g + 2));
    }
}
