// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsvm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>
#include <stdarg.h>

/* ---- SVM server API (src/adx/svm/svm.c) ---- */

void SVM_Init(void);
s32 SVM_SetCbSvrWithString(u32 svrId, void* fn, void* ctx, const char* name);
void SVM_SetCbSvrIdWithString(u32 svrId, u32 idx, void* fn, void* ctx, const char* name);
s32 SVM_TestAndSet(u32* p);
void SVM_GotoSvrBorder(s32 idx);
void SVM_CallErr1(const char* msg);

/* Registered callback slots shared with mwsfdlib.c, zeroed by
 * MWSFSVM_Init: +0x00 spare, +0x04 id-callback function,
 * +0x08 idle callback id, +0x0C main callback id. */
extern u32 lbl_eu_805FF1D0;
extern u32 lbl_eu_805FF1D4;
extern u32 lbl_eu_805FF1D8;
extern u32 lbl_eu_805FF1DC;

void MWSFSVM_Init(void) {
    u32* base;
    base = (u32*)&lbl_eu_805FF1D0;
    SVM_Init();
    base[0] = 0;
    base[1] = 0;
    base[2] = 0;
    base[3] = 0;
}

/* Register the movie-server id callback: hand it to SVM and remember it
 * in the callback table (+0x04). */
void MWSFSVM_EntryIdVfunc(void* self, void* a, void* b, void* c) {
    SVM_SetCbSvrIdWithString(2, (u32)self, a, b, c);
    lbl_eu_805FF1D4 = (u32)self;
}

/* Register the idle server callback; SVM returns its assigned svr id,
 * stored in the callback table (+0x08). */
void MWSFSVM_EntryIdleFunc(void* p1, void* p2, void* p3) {
    lbl_eu_805FF1D8 = SVM_SetCbSvrWithString(6, p1, p2, p3);
}

/* Register the main server callback; SVM returns its assigned svr id,
 * stored in the callback table (+0x0C). */
void MWSFSVM_EntryMainFunc(int arg1, int arg2, int arg3) {
    int result = SVM_SetCbSvrWithString(5, (void*)arg1, (void*)arg2, (void*)arg3);
    lbl_eu_805FF1DC = result;
}
/* Test-and-set passthrough on an SVM flag word (used by mwsfdsvr.c to
 * claim per-worker state). */
s32 MWSFSVM_TestAndSet(void* p) { return SVM_TestAndSet(p); }

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

/* Error data record. Entry sub-record at +0x04, exit at +0x6c; the
 * formatted message pointer is stashed at +0x0c. */
typedef struct TraceRec TraceRec;
struct TraceRec {
    u8 pad_0x00[0x4];
    u32 field_0x04;                            /* 0x04 (entry sub-record) */
    u8 pad_0x08[0x4];
    u32 field_0x0c;                            /* 0x0c (message pointer) */
    u8 pad_0x10[0x5c];                         /* 0x10..0x6b */
    u32 field_0x6c;                            /* 0x6c (exit sub-record) */
};

extern TraceCb* lbl_eu_805FF3A0;
extern TraceRec lbl_eu_80566AC8;

extern char lbl_eu_805FF1E0[256];

/* Format a message into the shared scratch buffer and report it:
 * enter-trace -> SVM error hook -> exit-trace.
 * NB: residual 2 reg-swaps (va_info stw 0x68/0x6c order) are compiler-version
 * intrinsic: retail CriWare is Wii/1.1-built; this TU still builds under the
 * criwareLib GC/3.0a5.2 default which swaps the two va_list field stores.
 * OPEN-ITEM packet: status BLOCKED-at-99.8% (structural 0, 2 reg-swaps, relocs
 * clean, size PASS). Ruled out: va_start before memset (frame +0x10/r29),
 * vsprintf prototype visibility, char* vs const char* fmt, va_end, fmt local
 * copy. Next experiments: (1) mw_version="Wii/1.1" on the configure.py Object
 * + unit-wide regression scan; (2) alternate stdarg builtin if one exists;
 * (3) re-test after any MWCC toolchain change. See MWCC_CASES svm.c entry
 * fact 0 for the version-intrinsic store-order evidence. */
void MWSFSVM_Error(const char* fmt, ...) {
    va_list ap;

    memset(lbl_eu_805FF1E0, 0, 256);
    va_start(ap, fmt);
    vsprintf(lbl_eu_805FF1E0, fmt, ap);
    if (lbl_eu_805FF3A0 != NULL) {
        lbl_eu_80566AC8.field_0x0c = (u32)lbl_eu_805FF1E0;
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0,
            &lbl_eu_80566AC8.field_0x04);
    }
    SVM_CallErr1(lbl_eu_805FF1E0);
    if (lbl_eu_805FF3A0 != NULL) {
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0,
            &lbl_eu_80566AC8.field_0x6c);
    }
}

extern TraceRec lbl_eu_80566B9C;

/* Jump the server thread to its idle border, bracketing the transition
 * with enter/exit trace records like MWSFSVM_Error. */
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
