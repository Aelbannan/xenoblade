// svm.c - CRI ADX Server Manager
// Reconstructed from retail svm.s (0x80399B0C..0x8039AD88).
//
// The retail build inlines the lock/unlock/error-callback bodies into every
// caller (there are no standalone helper functions in the retail .text), so
// these bodies are expressed as macros here. The lock/unlock/err callback
// pairs are part of the single ctrl struct at lbl_eu_805F26F0; the error
// callback pair and message buffer are separate globals next to it.

#include <string.h>

#include <harness_catalog.h>

typedef struct SvmCbPair {
    void (*func)(void*);
    void* object;
} SvmCbPair;

typedef struct SvmErrCb {
    void (*func)(void* object, const void* msg);
    void* object;
} SvmErrCb;

typedef struct SvmSvrEntry {
    u32 (*func)(void*);
    void* object;
    const char* name;
} SvmSvrEntry;

typedef struct SvmExecArea {
    u32 flags[8];              /* 0x3A8 */
    u32 counts[8];             /* 0x3C8 */
} SvmExecArea;

typedef struct SvmCbArea {
    SvmCbPair lock;            /* 0x010 */
    SvmCbPair unlock;          /* 0x018 */
} SvmCbArea;

typedef struct SvmCtrl {
    volatile u32 init_count;   /* 0x000 */
    volatile u32 lock_count;   /* 0x004 */
    volatile s32 lock_flag;    /* 0x008 */
    u32 field_0x0C;            /* 0x00C */
    SvmCbArea cb_area;         /* 0x010 */
    char err_msg[0x100];       /* 0x020 */
    SvmErrCb err_cb;           /* 0x120 */
    SvmSvrEntry svr_tbl[6][8]; /* 0x128 - [exec row][slot]; DelCbSvr indexes [idx][svrId] */
    SvmCbPair bdr_tbl[8];      /* 0x368 */
    SvmExecArea exec;          /* 0x3A8 */
    u64 field_0x3E8;           /* 0x3E8 */
    u64 field_0x3F0;           /* 0x3F0 */
    void* testandset_fn;       /* 0x3F8 */
} SvmCtrl;

SvmCtrl lbl_eu_805F26F0;
unsigned char lbl_eu_805F2AF0[8];

__declspec(section ".rodata") __attribute__((aligned(8))) char lbl_eu_80518F50[0x1B8] = {
    0x32, 0x31, 0x30, 0x33, 0x31, 0x30, 0x32, 0x3A, 0x53, 0x56, 0x4D, 0x3A, 0x73, 0x76, 0x6D, 0x5F, 0x75, 0x6E, 0x6C, 0x6F, 0x63, 0x6B, 0x3A, 0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x74, 0x79, 0x70, 0x65, 0x20, 0x6D, 0x69, 0x73, 0x73, 0x20, 0x6D, 0x61, 0x74, 0x63, 0x68, 0x2E, 0x28, 0x74, 0x79, 0x70, 0x65, 0x20, 0x6F, 0x72, 0x67, 0x3D, 0x25, 0x64, 0x2C, 0x20, 0x74, 0x79, 0x70, 0x65, 0x20, 0x6E, 0x6F, 0x77, 0x3D, 0x25, 0x64, 0x29, 0x00, 0x31, 0x30, 0x37, 0x31, 0x32, 0x30, 0x35, 0x3A, 0x53, 0x56, 0x4D, 0x5F, 0x53, 0x65, 0x74, 0x43, 0x62, 0x53, 0x76, 0x72, 0x49, 0x64, 0x3A, 0x69, 0x6C, 0x6C, 0x65, 0x67, 0x61, 0x6C, 0x20, 0x73, 0x76, 0x74, 0x79, 0x70, 0x65, 0x00, 0x55, 0x6E, 0x6B, 0x6E, 0x6F, 0x77, 0x6E, 0x00, 0x31, 0x30, 0x35, 0x31, 0x30, 0x30, 0x31, 0x3A, 0x53, 0x56, 0x4D, 0x5F, 0x53, 0x65, 0x74, 0x43, 0x62, 0x53, 0x76, 0x72, 0x3A, 0x74, 0x6F, 0x6F, 0x20, 0x6D, 0x61, 0x6E, 0x79, 0x20, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x00, 0x31, 0x30, 0x35, 0x31, 0x30, 0x30, 0x32, 0x3A, 0x53, 0x56, 0x4D, 0x5F, 0x44, 0x65, 0x6C, 0x43, 0x62, 0x53, 0x76, 0x72, 0x3A, 0x69, 0x6C, 0x6C, 0x65, 0x67, 0x61, 0x6C, 0x20, 0x69, 0x64, 0x00, 0x31, 0x30, 0x37, 0x31, 0x32, 0x30, 0x36, 0x3A, 0x53, 0x56, 0x4D, 0x5F, 0x53, 0x65, 0x74, 0x43, 0x62, 0x53, 0x76, 0x72, 0x49, 0x64, 0x3A, 0x69, 0x6C, 0x6C, 0x65, 0x67, 0x61, 0x6C, 0x20, 0x73, 0x76, 0x74, 0x79, 0x70, 0x65, 0x00, 0x31, 0x30, 0x37, 0x31, 0x32, 0x30, 0x31, 0x3A, 0x53, 0x56, 0x4D, 0x5F, 0x53, 0x65, 0x74, 0x43, 0x62, 0x53, 0x76, 0x72, 0x49, 0x64, 0x3A, 0x69, 0x6C, 0x6C, 0x65, 0x67, 0x61, 0x6C, 0x20, 0x69, 0x64, 0x00, 0x31, 0x30, 0x37, 0x31, 0x32, 0x30, 0x32, 0x3A, 0x53, 0x56, 0x4D, 0x5F, 0x53, 0x65, 0x74, 0x43, 0x62, 0x53, 0x76, 0x72, 0x49, 0x64, 0x3A, 0x69, 0x6C, 0x6C, 0x65, 0x67, 0x61, 0x6C, 0x20, 0x73, 0x76, 0x74, 0x79, 0x70, 0x65, 0x00, 0x32, 0x31, 0x30, 0x30, 0x38, 0x30, 0x31, 0x3A, 0x53, 0x56, 0x4D, 0x5F, 0x53, 0x65, 0x74, 0x43, 0x62, 0x53, 0x76, 0x72, 0x49, 0x64, 0x3A, 0x6F, 0x76, 0x65, 0x72, 0x20, 0x77, 0x72, 0x69, 0x74, 0x65, 0x20, 0x63, 0x61, 0x6C, 0x6C, 0x62, 0x61, 0x63, 0x6B, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x2E, 0x00, 0x31, 0x30, 0x37, 0x31, 0x33, 0x30, 0x31, 0x3A, 0x53, 0x56, 0x4D, 0x5F, 0x45, 0x78, 0x65, 0x63, 0x53, 0x76, 0x72, 0x46, 0x75, 0x6E, 0x63, 0x49, 0x64, 0x3A, 0x69, 0x6C, 0x6C, 0x65, 0x67, 0x61, 0x6C, 0x20, 0x69, 0x64, 0x00, 0x31, 0x30, 0x37, 0x31, 0x33, 0x30, 0x32, 0x3A, 0x53, 0x56, 0x4D, 0x5F, 0x45, 0x78, 0x65, 0x63, 0x53, 0x76, 0x72, 0x46, 0x75, 0x6E, 0x63, 0x49, 0x64, 0x3A, 0x69, 0x6C, 0x6C, 0x65, 0x67, 0x61, 0x6C, 0x20, 0x73, 0x76, 0x74, 0x79, 0x70, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define lbl_eu_805F2710 (lbl_eu_805F26F0.err_msg)
#define lbl_eu_805F2810 (lbl_eu_805F26F0.err_cb)
#define lbl_eu_805F2A58 ((u32*)lbl_eu_805F26F0.bdr_tbl)
#define lbl_eu_805F2700 (lbl_eu_805F26F0.cb_area.lock)
#define lbl_eu_805F2708 (lbl_eu_805F26F0.cb_area.unlock)


extern char* CRICRW_Strncpy(char* dst, void* ignored, const char* src, size_t n);
extern int CRICRW_Vsprintf(char* s, void* ignored, const char* fmt, va_list ap);

void SVM_CallErr(const char* fmt, ...);

/* Report an error message through the error callback; NULL means no message.
   (Retail inlines this into svm_SetCbSvr / svm_SetCbSvrId / SVM_DelCbSvr;
   see those functions for the inline form.) */

/* Lock the server manager (retail inlines this body into every caller). */
#define SVM_LOCK()                                                            \
    do {                                                                      \
        SvmCtrl* _c = &lbl_eu_805F26F0;                                       \
        if (_c->cb_area.lock.func != NULL) {                                       \
            void (*_fn)(void*) = _c->cb_area.lock.func;                            \
            void* _obj = _c->cb_area.lock.object;                                  \
            _fn(_obj);                                                        \
            if (_c->lock_count == 0)                                          \
                _c->lock_flag = 1;                                            \
            _c->lock_count++;                                                 \
        }                                                                     \
    } while (0)

/* Unlock the server manager (retail inlines this body into every caller). */
#define SVM_UNLOCK()                                                          \
    do {                                                                      \
        SvmCtrl* _c = &lbl_eu_805F26F0;                                       \
        if (_c->cb_area.unlock.func != NULL) {                                     \
            _c->lock_count = _c->lock_count - 1;                              \
            if (_c->lock_count == 0) {                                        \
                if (_c->lock_flag != 1)                                       \
                    SVM_CallErr(lbl_eu_80518F50, _c->lock_flag, 1);           \
                _c->lock_flag = 0;                                            \
            }                                                                 \
            {                                                                 \
                void (*_fn)(void*) = _c->cb_area.unlock.func; \
                void* _obj = _c->cb_area.unlock.object; \
                _fn(_obj);                                                    \
            }                                                                 \
        }                                                                     \
    } while (0)

void SVM_Lock(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (ctrl->cb_area.lock.func != NULL) {
        /* Object fetched through the global directly; MWCC keeps the
           intermediate pair address (addi+lwz) instead of folding. */
        ctrl->cb_area.lock.func(lbl_eu_805F26F0.cb_area.lock.object);
        if (ctrl->lock_count == 0)
            ctrl->lock_flag = 1;
        ctrl->lock_count++;
    }
}

void SVM_Unlock(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    /* Pair pointer live across SVM_CallErr forces MWCC to rematerialize
       the address at the use instead of folding the displacement. */
    SvmCbPair* cb = &ctrl->cb_area.unlock;
    if (ctrl->cb_area.unlock.func != NULL) {
        ctrl->lock_count--;
        if (ctrl->lock_count == 0) {
            if (ctrl->lock_flag != 1)
                SVM_CallErr(lbl_eu_80518F50, ctrl->lock_flag, 1);
            ctrl->lock_flag = 0;
        }
        cb->func(cb->object);
    }
}

void SVM_CallErr(const char* fmt, ...) {
    va_list ap;
    /* NB: retail schedules the va_start struct build after the memset call
       (the GPR/FP register spill is hoisted to entry; the va_info struct is
       sunk to the Vsprintf call site). va_start must therefore come after
       memset in source to reproduce the retail layout. */
    memset(lbl_eu_805F2710, 0, 0x100);
    va_start(ap, fmt);
    CRICRW_Vsprintf(lbl_eu_805F2710, (void*)0x100, fmt, ap);
    if (lbl_eu_805F2810.func != NULL) {
        void (*fn)(void*, const void*) = lbl_eu_805F2810.func;
        void* obj = lbl_eu_805F2810.object;
        fn(obj, lbl_eu_805F2710);
    }
}

void SVM_CallErr1(const char* msg) {
    if (msg == NULL) {
        void (*fn)(void*, const void*) = lbl_eu_805F2810.func;
        void* obj = lbl_eu_805F2810.object;
        fn(obj, NULL);
    } else {
        CRICRW_Strncpy(lbl_eu_805F2710, (void*)0x100, msg, 0xFF);
        if (lbl_eu_805F2810.func != NULL) {
            void (*fn)(void*, const void*) = lbl_eu_805F2810.func;
            void* obj = lbl_eu_805F2810.object;
            fn(obj, lbl_eu_805F2710);
        }
    }
}

s32 svm_SetCbSvr(u32 svrId, void* fn, void* ctx, const char* name);
void svm_SetCbSvrId(u32 svrId, u32 idx, void* fn, void* ctx, const char* name);

s32 SVM_SetCbSvrWithString(u32 svrId, void* fn, void* ctx, const char* name) {
    s32 ret;
    SVM_LOCK();
    ret = svm_SetCbSvr(svrId, fn, ctx, name);
    SVM_UNLOCK();
    return ret;
}

/* Inlined error-report body; retail duplicates this into every caller.
   The message is formed as base + offset from a runtime base value so the
   NULL check survives as `addic. r5, r3, off` instead of being folded away.
   Func fetches stay folded off the ctrl register while the object fetch is
   issued through the global name (mixed shape mirrors SVM_Lock/SVM_Unlock,
   giving the unfolded addi + lwz pair load). */
/* Inline (not macro): base must arrive as an opaque r3 parameter so the
   NULL test survives as addic. r5, r3, off with msg live for Strncpy.
   ecb live across the Strncpy call makes MWCC rematerialize the pair
   address (addi + lwz) for the object fetch, matching retail. */
static inline void svm_ReportErr(const char* base, s32 off) {
    /* Direct-global accesses throughout: with no ctrl local in the helper,
       MWCC can share the caller's hoisted address register instead of
       rematerializing the global address inside the inlined body. */
    SvmErrCb* ecb = &lbl_eu_805F26F0.err_cb;
    const char* msg = &base[off];
    if (msg == NULL) {
        lbl_eu_805F26F0.err_cb.func(ecb->object, NULL);
    } else {
        CRICRW_Strncpy(lbl_eu_805F26F0.err_msg, (void*)0x100, msg, 0xFF);
        if (lbl_eu_805F26F0.err_cb.func != NULL)
            lbl_eu_805F26F0.err_cb.func(ecb->object, lbl_eu_805F26F0.err_msg);
    }
}

s32 svm_SetCbSvr(u32 svrId, void* fn, void* ctx, const char* name) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    s32 i;
    if (svrId > 7) {
        svm_ReportErr(lbl_eu_80518F50, 0x47);
        return -1;
    }
    /* First free slot; func/object are stored before the name branch. */
    for (i = 0; i < 6; i++) {
        SvmSvrEntry* entry = &ctrl->svr_tbl[svrId][i];
        if (entry->func == NULL) {
            entry->func = (u32 (*)(void*))fn;
            entry->object = ctx;
            entry->name = name != NULL ? name : &lbl_eu_80518F50[0x6D];
            break;
        }
    }
    if (i != 6)
        return i;
    svm_ReportErr(lbl_eu_80518F50, 0x75);
    return -1;
}

void SVM_DelCbSvr(u32 svrId, u32 idx) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (ctrl->cb_area.lock.func != NULL) {
        ctrl->cb_area.lock.func(lbl_eu_805F26F0.cb_area.lock.object);
        if (ctrl->lock_count == 0)
            ctrl->lock_flag = 1;
        ctrl->lock_count++;
    }
    if (idx > 5) {
        svm_ReportErr(lbl_eu_80518F50, 0xA3);
    } else if (svrId > 7) {
        svm_ReportErr(lbl_eu_80518F50, 0xC3);
    } else {
        /* Slot clearing: row is the exec-server index, column the slot id. */
        SvmSvrEntry* entry = &ctrl->svr_tbl[idx][svrId];
        entry->func = NULL;
        entry->object = NULL;
        entry->name = NULL;
    }
    {
        SvmCbPair* cb = &ctrl->cb_area.unlock;
        if (ctrl->cb_area.unlock.func != NULL) {
            ctrl->lock_count--;
            if (ctrl->lock_count == 0) {
                if (ctrl->lock_flag != 1)
                    SVM_CallErr(lbl_eu_80518F50, ctrl->lock_flag, 1);
                ctrl->lock_flag = 0;
            }
            cb->func(cb->object);
        }
    }
}

void SVM_SetCbSvrIdWithString(u32 svrId, u32 idx, void* fn, void* ctx, const char* name) {
    SVM_LOCK();
    svm_SetCbSvrId(svrId, idx, fn, ctx, name);
    SVM_UNLOCK();
}

void svm_SetCbSvrId(u32 svrId, u32 idx, void* fn, void* ctx, const char* name) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (idx > 5) {
        svm_ReportErr(lbl_eu_80518F50, 0xE9);
        return;
    }
    if (svrId > 7) {
        svm_ReportErr(lbl_eu_80518F50, 0x10B);
        return;
    }
    {
        SvmSvrEntry* entry = &ctrl->svr_tbl[idx][svrId];
        if (entry->func != NULL) {
            svm_ReportErr(lbl_eu_80518F50, 0x131);
        }
        entry->func = (u32 (*)(void*))fn;
        entry->object = ctx;
        if (name != NULL)
            entry->name = name;
        else
            entry->name = &lbl_eu_80518F50[0x6D];
    }
}

void SVM_SetCbBdr(s32 idx, void* fn, void* ctx) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    SVM_LOCK();
    ctrl->bdr_tbl[idx].func = (void (*)(void*))fn;
    ctrl->bdr_tbl[idx].object = ctx;
    SVM_UNLOCK();
}

void SVM_GotoSvrBorder(s32 idx) {
    u32* base = lbl_eu_805F2A58;
    u32 fn = base[idx * 2];
    if (fn == 0) return;
    ((void (*)(void*))fn)((void*)base[idx * 2 + 1]);
}

void SVM_SetCbErr(void* fn, void* ctx) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    SVM_LOCK();
    ctrl->err_cb.func = (void (*)(void*, const void*))fn;
    ctrl->err_cb.object = ctx;
    SVM_UNLOCK();
}

void SVM_SetCbLock(void* cb, void* ctx) {
    SvmCbPair* p = &lbl_eu_805F2700;
    p->func = (void (*)(void*))cb;
    p->object = ctx;
}

void SVM_SetCbUnlock(void* cb, void* ctx) {
    SvmCbPair* p = &lbl_eu_805F2708;
    p->func = (void (*)(void*))cb;
    p->object = ctx;
}

/* Shared server-runner body; retail inlines this into each SVM_ExecSvr*
   wrapper with the table pointer and server index folded in at the call
   sites. */
static inline u32 svm_ExecSvr(SvmSvrEntry* tbl, s32 id) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    u32 one = 1;
    SvmSvrEntry* p = &tbl[6 * id];
    u32 zero = 0;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            ctrl->exec.flags[id] = one;
            result |= fn(obj);
            ctrl->exec.flags[id] = zero;
        }
    }
    ctrl->exec.counts[id] += 1;
    return result;
}

u32 SVM_ExecSvrVint(void) {
    return svm_ExecSvr(lbl_eu_805F26F0.svr_tbl[0], 0);
}

u32 SVM_ExecSvrUsrVsync(void) {
    return svm_ExecSvr(lbl_eu_805F26F0.svr_tbl[0], 1);
}

u32 SVM_ExecSvrVsync(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    SvmSvrEntry* tbl = ctrl->svr_tbl[0];
    u32 i;
    u32 one = 1;
    /* One-time opaque read keeps the flags base in its own register across
       the server calls (retail hoists it with an 8-byte displacement). */
    u32* volatile flagsv = ctrl->exec.flags;
    u32* flags = flagsv;
    u32 zero = 0;
    for (i = 0; i < 6; i++) {
        u32 (*fn)(void*) = tbl[6 * 2 + i].func;
        void* obj = tbl[6 * 2 + i].object;
        if (fn != NULL) {
            flags[2] = one;
            result |= fn(obj);
            flags[2] = zero;
        }
    }
    {
        u32* cnt = ctrl->exec.counts;
        cnt[2] += 1;
    }
    return result;
}

u32 SVM_ExecSvrUhigh(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    SvmSvrEntry* tbl = ctrl->svr_tbl[0];
    u32 i = 0;
    u32 one = 1;
    /* Opaque pointer read keeps the flags base in its own register across
       the server calls instead of folding into a ctrl displacement. */
    u32* volatile flagsv = ctrl->exec.flags;
    u32* flags = flagsv;
    SvmSvrEntry* p = &tbl[18];
    u32 zero = 0;
    for (; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flags[3] = one;
            result |= fn(obj);
            flags[3] = zero;
        }
    }
    ctrl->exec.counts[3] += 1;
    return result;
}

u32 SVM_ExecSvrFs(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    u32 i = 0;
    u32 one = 1;
    /* Opaque read keeps the flags base in its own register across the
       server calls instead of folding into a ctrl displacement. */
    u32* volatile flagsv = ctrl->exec.flags;
    u32* flags = flagsv;
    SvmSvrEntry* p = &ctrl->svr_tbl[4][0];
    u32 zero = 0;
    for (; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flags[4] = one;
            result |= fn(obj);
            flags[4] = zero;
        }
    }
    ctrl->exec.counts[4] += 1;
    return result;
}

u32 SVM_ExecSvrMain(void) {
    /* Retail inlines the server-runner with row 5 baked in; the volatile
       local keeps the flags base address materialized across the calls. */
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    u32 i = 0;
    u32 one = 1;
    u32* volatile flagsv = ctrl->exec.flags;
    u32* flags = flagsv;
    SvmSvrEntry* p = &ctrl->svr_tbl[5][0];
    u32 zero = 0;
    for (; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flags[5] = one;
            result |= fn(obj);
            flags[5] = zero;
        }
    }
    ctrl->exec.counts[5] += 1;
    return result;
}

/* Single-call-site copy of the server runner for SVM_ExecSvrMwIdle.
   The p/end pointer pair keeps both entry addresses derived from the
   table row (closer to retail's two-step address computation). */
static inline u32 svm_ExecSvrMwIdleSub(SvmSvrEntry (*tbl)[8]) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    u32 one = 1;
    u32* flags = ctrl->exec.flags;
    SvmSvrEntry* p = &tbl[6][0];
    SvmSvrEntry* end = &tbl[7][0];
    u32 zero = 0;
    for (; p < end;) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flags[6] = one;
            result |= fn(obj);
            flags[6] = zero;
        }
        p++;
    }
    {
        u32* cnt = ctrl->exec.counts;
        cnt[6] += 1;
    }
    return result;
}

u32 SVM_ExecSvrMwIdle(void) {
    return svm_ExecSvrMwIdleSub(lbl_eu_805F26F0.svr_tbl);
}

u32 SVM_ExecSvrUsrIdle(void) {
    /* Dedicated copy: declaration order drives MWCC's register assignment
       (ctrl, result, table base, loop index, flag-one, flags base, cursor,
       flag-zero) to reproduce the retail allocation. */
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    SvmSvrEntry* tbl = ctrl->svr_tbl[0];
    s32 i = 0;
    u32 one = 1;
    /* Opaque read keeps the flags base in its own register across calls. */
    u32* volatile flagsv = ctrl->exec.flags;
    u32* flags = flagsv;
    SvmSvrEntry* p = &tbl[7 * 6];
    u32 zero = 0;
    for (; i < 6; i++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flags[7] = one;
            result |= fn(obj);
            flags[7] = zero;
        }
        p++;
    }
    ctrl->exec.counts[7] += 1;
    return result;
}

void SVM_Init(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (ctrl->init_count == 0) {
        memset(&ctrl->exec.flags[0], 0, 0x20);
        memset(&ctrl->cb_area.lock, 0, 8);
        memset(&ctrl->cb_area.unlock, 0, 8);
        memset(&ctrl->field_0x3E8, 0, 8);
        memset(&ctrl->field_0x3F0, 0, 8);
        {
            u32* cnt = ctrl->exec.counts;
            cnt[0] = 0;
            cnt[1] = 0;
            cnt[2] = 0;
            cnt[3] = 0;
            cnt[4] = 0;
            cnt[5] = 0;
        }
        ctrl->testandset_fn = NULL;
    }
    ctrl->init_count++;
}

void SVM_Finish(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    ctrl->init_count--;
    if (ctrl->init_count != 0)
        return;
    memset(ctrl->exec.flags, 0, 0x20);
    memset(&ctrl->cb_area.lock, 0, 8);
    memset(&ctrl->cb_area.unlock, 0, 8);
    memset(&ctrl->field_0x3E8, 0, 8);
    memset(&ctrl->field_0x3F0, 0, 8);
    {
        u32* cnt = ctrl->exec.counts;
        u32 zero = 0;
        cnt[0] = zero;
        cnt[1] = zero;
        cnt[2] = zero;
        cnt[3] = zero;
        cnt[4] = zero;
        cnt[5] = zero;
    }
    ctrl->testandset_fn = NULL;
    memset(&ctrl->err_cb, 0, 8);
}

/* NOTE: retail keeps the callback-pair address materialized (addi + lwz)
   instead of folding the load displacement off the ctrl base; peephole
   must stay off across this body or MWCC rewrites it to lwz disp(r31). */
/* NOTE: retail materializes the callback-pair address (addi + lwz) for each
   callback argument instead of folding a displacement off the ctrl base.
   No high-level source form tried (pair-pointer locals, volatile barriers,
   global-direct accesses, #pragma peephole off) reproduces the unfolded
   load under this unit's codegen; the folded lwz disp is the residual. */
s32 SVM_TestAndSet(u32* p) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    s32 result;
    u32 old;
    if (ctrl->testandset_fn != NULL) {
        result = ((s32 (*)(u32*))ctrl->testandset_fn)(p);
    } else {
        /* Inline lock */
        if (ctrl->cb_area.lock.func != NULL) {
            SvmCbPair* cb = &ctrl->cb_area.lock;
            ctrl->cb_area.lock.func(cb->object);
            if (ctrl->lock_count == 0)
                ctrl->lock_flag = 1;
            ctrl->lock_count++;
        }
        /* Swap in the locked value; result uses MWCC's compare idiom
           (((old - 1) | (1 - old)) >> 31). */
        old = *p;
        *p = 1;
        result = (old != 1);
        /* Inline unlock: callee is fetched at the call site (after the
           error path) so no pointer lives across SVM_CallErr. */
        if (ctrl->cb_area.unlock.func != NULL) {
            ctrl->lock_count--;
            if (ctrl->lock_count == 0) {
                if (ctrl->lock_flag != 1)
                    SVM_CallErr(lbl_eu_80518F50, ctrl->lock_flag, 1);
                ctrl->lock_flag = 0;
            }
            ctrl->cb_area.unlock.func(lbl_eu_805F26F0.cb_area.unlock.object);
        }
    }
    return result;
}
