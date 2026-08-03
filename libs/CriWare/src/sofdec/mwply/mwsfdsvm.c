// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsvm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>
#include <stdarg.h>

extern u32 lbl_eu_805FF1D0;
extern u32 lbl_eu_805FF1D8;
extern int lbl_eu_805FF1DC;

void MWSFSVM_Init(void) {
    u32* base;
    base = (u32*)&lbl_eu_805FF1D0;
    SVM_Init();
    base[0] = 0;
    base[1] = 0;
    base[2] = 0;
    base[3] = 0;
}

extern int SVM_SetCbSvrIdWithString(int, void*, void*, void*, void*);
extern u32 lbl_eu_805FF1D4;

void MWSFSVM_EntryIdVfunc(void* self, void* a, void* b, void* c) {
    SVM_SetCbSvrIdWithString(2, self, a, b, c);
    lbl_eu_805FF1D4 = (u32)self;
}

void MWSFSVM_EntryIdleFunc(void* p1, void* p2, void* p3) {
    lbl_eu_805FF1D8 = SVM_SetCbSvrWithString(6, p1, p2, p3);
}

void MWSFSVM_EntryMainFunc(int arg1, int arg2, int arg3)
{
    int result = SVM_SetCbSvrWithString(5, arg1, arg2, arg3);
    lbl_eu_805FF1DC = result;
}
void MWSFSVM_TestAndSet(void* p) { SVM_TestAndSet(p); }

/* ---- SVM trace callback infrastructure ----
 * lbl_eu_805FF3A0 is a global pointer to an optional trace object whose
 * vtable exposes a "trace" method at offset 0x24; lbl_eu_80566AC8 is the
 * error data record (sub-record at +0x6c). */

typedef struct TraceCb TraceCb;

typedef struct TraceCbVtable {
    u8 pad_0x00[0x24];
    void (*trace)(TraceCb* self, void* data);   /* 0x24 */
} TraceCbVtable;

struct TraceCb {
    const TraceCbVtable* vtable;
};

extern TraceCb* lbl_eu_805FF3A0;
extern u8 lbl_eu_80566AC8[];

extern void SVM_CallErr1(const char* msg);

extern char lbl_eu_805FF1E0[];

void MWSFSVM_Error(const char* fmt, ...) {
    va_list ap;
    char* msg = lbl_eu_805FF1E0;

    memset(msg, 0, 256);
    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
    va_end(ap);

    if (lbl_eu_805FF3A0 != NULL) {
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, msg);
    }
    SVM_CallErr1(msg);
    if (lbl_eu_805FF3A0 != NULL) {
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0,
            (void*)(lbl_eu_80566AC8 + 0x6C));
    }
}

typedef struct TraceRec TraceRec;
struct TraceRec {
    u8 pad_0x00[0x4];
    u32 field_0x04;                            /* 0x04 (entry sub-record) */
    u8 pad_0x08[0x64];                         /* 0x08..0x6b */
    u32 field_0x6c;                            /* 0x6c (exit sub-record) */
};

extern TraceRec lbl_eu_80566B9C;
extern int SVM_GotoSvrBorder(int svrId);

void MWSFSVM_GotoIdleBorder(void) {
    TraceCb* cb = lbl_eu_805FF3A0;
    if (cb != NULL) {
        cb->vtable->trace(cb, &lbl_eu_80566B9C.field_0x04);
    }
    SVM_GotoSvrBorder(6);
    cb = lbl_eu_805FF3A0;
    if (cb != NULL) {
        cb->vtable->trace(cb, &lbl_eu_80566B9C.field_0x6c);
    }
}
