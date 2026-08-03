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

typedef struct SvmCtrl {
    volatile u32 init_count;   /* 0x000 */
    volatile u32 lock_count;   /* 0x004 */
    volatile s32 lock_flag;    /* 0x008 */
    u32 field_0x0C;            /* 0x00C */
    SvmCbPair lock_cb;         /* 0x010 */
    SvmCbPair unlock_cb;       /* 0x018 */
    char err_msg[0x100];       /* 0x020 */
    SvmErrCb err_cb;           /* 0x120 */
    SvmSvrEntry svr_tbl[8][6]; /* 0x128 */
    SvmCbPair bdr_tbl[8];      /* 0x368 */
    u32 exec_flags[8];         /* 0x3A8 */
    u32 exec_counts[8];        /* 0x3C8 */
    u64 field_0x3E8;           /* 0x3E8 */
    u64 field_0x3F0;           /* 0x3F0 */
    void* testandset_fn;       /* 0x3F8 */
} SvmCtrl;

extern SvmCtrl lbl_eu_805F26F0;
extern const char lbl_eu_80518F50[];

extern char lbl_eu_805F2710[];    /* error message buffer (ctrl + 0x20) */
extern SvmErrCb lbl_eu_805F2810;  /* error callback pair (ctrl + 0x120) */

extern char* CRICRW_Strncpy(char* dst, void* ignored, const char* src, size_t n);
extern int CRICRW_Vsprintf(char* s, void* ignored, const char* fmt, va_list ap);

void SVM_CallErr(const char* fmt, ...);

/* Report an error message through the error callback; NULL means no message.
   (Retail inlines this into svm_SetCbSvr / svm_SetCbSvrId / SVM_DelCbSvr;
   the error callback is reached through the ctrl struct there.) */
#define SVM_ERR_CB_MSG(m)                                                     \
    do {                                                                      \
        SvmCtrl* _e = &lbl_eu_805F26F0;                                       \
        if ((m) == NULL) {                                                    \
            void (*_fn)(void*, const void*) = _e->err_cb.func;                \
            void* _obj = _e->err_cb.object;                                   \
            _fn(_obj, NULL);                                                  \
        } else {                                                              \
            CRICRW_Strncpy(_e->err_msg, (void*)0x100, (m), 0xFF);             \
            if (_e->err_cb.func != NULL) {                                    \
                void (*_fn)(void*, const void*) = _e->err_cb.func;            \
                void* _obj = _e->err_cb.object;                               \
                _fn(_obj, _e->err_msg);                                       \
            }                                                                 \
        }                                                                     \
    } while (0)

/* Lock the server manager (retail inlines this body into every caller). */
#define SVM_LOCK()                                                            \
    do {                                                                      \
        SvmCtrl* _c = &lbl_eu_805F26F0;                                       \
        if (_c->lock_cb.func != NULL) {                                       \
            void (*_fn)(void*) = _c->lock_cb.func;                            \
            void* _obj = _c->lock_cb.object;                                  \
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
        if (_c->unlock_cb.func != NULL) {                                     \
            _c->lock_count = _c->lock_count - 1;                              \
            if (_c->lock_count == 0) {                                        \
                if (_c->lock_flag != 1)                                       \
                    SVM_CallErr(lbl_eu_80518F50, _c->lock_flag, 1);           \
                _c->lock_flag = 0;                                            \
            }                                                                 \
            {                                                                 \
                void (*_fn)(void*) = _c->unlock_cb.func;                      \
                void* _obj = _c->unlock_cb.object;                            \
                _fn(_obj);                                                    \
            }                                                                 \
        }                                                                     \
    } while (0)

void SVM_Lock(void) {
    SVM_LOCK();
}

void SVM_Unlock(void) {
    SVM_UNLOCK();
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

s32 svm_SetCbSvr(u32 svrId, void* fn, void* ctx, const char* name) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    s32 i;
    if (svrId > 7) {
        SVM_ERR_CB_MSG(&lbl_eu_80518F50[0x47]);
        return -1;
    }
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
    SVM_ERR_CB_MSG(&lbl_eu_80518F50[0x75]);
    return -1;
}

void SVM_DelCbSvr(u32 svrId, u32 idx) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    SVM_LOCK();
    if (idx > 5) {
        SVM_ERR_CB_MSG(&lbl_eu_80518F50[0xA3]);
    } else if (svrId > 7) {
        SVM_ERR_CB_MSG(&lbl_eu_80518F50[0xC3]);
    } else {
        SvmSvrEntry* entry = &ctrl->svr_tbl[svrId][idx];
        entry->func = NULL;
        entry->object = NULL;
        entry->name = NULL;
    }
    SVM_UNLOCK();
}

void SVM_SetCbSvrIdWithString(u32 svrId, u32 idx, void* fn, void* ctx, const char* name) {
    SVM_LOCK();
    svm_SetCbSvrId(svrId, idx, fn, ctx, name);
    SVM_UNLOCK();
}

void svm_SetCbSvrId(u32 svrId, u32 idx, void* fn, void* ctx, const char* name) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (idx > 5) {
        SVM_ERR_CB_MSG(&lbl_eu_80518F50[0xE9]);
        return;
    }
    if (svrId > 7) {
        SVM_ERR_CB_MSG(&lbl_eu_80518F50[0x10B]);
        return;
    }
    {
        SvmSvrEntry* entry = &ctrl->svr_tbl[svrId][idx];
        if (entry->func != NULL)
            SVM_ERR_CB_MSG(&lbl_eu_80518F50[0x131]);
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

extern u32 lbl_eu_805F2A58[];
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

extern SvmCbPair lbl_eu_805F2700;
void SVM_SetCbLock(void* cb, void* ctx) {
    SvmCbPair* p = &lbl_eu_805F2700;
    p->func = (void (*)(void*))cb;
    p->object = ctx;
}

extern SvmCbPair lbl_eu_805F2708;
void SVM_SetCbUnlock(void* cb, void* ctx) {
    SvmCbPair* p = &lbl_eu_805F2708;
    p->func = (void (*)(void*))cb;
    p->object = ctx;
}

u32 SVM_ExecSvrVint(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    SvmSvrEntry* p = ctrl->svr_tbl[0];
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            ctrl->exec_flags[0] = 1;
            result |= fn(obj);
            ctrl->exec_flags[0] = 0;
        }
    }
    ctrl->exec_counts[0] += 1;
    return result;
}

u32 SVM_ExecSvrUsrVsync(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 6;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            ctrl->exec_flags[1] = 1;
            result |= fn(obj);
            ctrl->exec_flags[1] = 0;
        }
    }
    ctrl->exec_counts[1] += 1;
    return result;
}

u32 SVM_ExecSvrVsync(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    SvmSvrEntry* base = ctrl->svr_tbl[0];
    SvmSvrEntry* p = base + 12;
    u32* cnt = ctrl->exec_counts;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            ctrl->exec_flags[2] = 1;
            result |= fn(obj);
            ctrl->exec_flags[2] = 0;
        }
    }
    cnt[2] += 1;
    return result;
}

u32 SVM_ExecSvrUhigh(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 18;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            ctrl->exec_flags[3] = 1;
            result |= fn(obj);
            ctrl->exec_flags[3] = 0;
        }
    }
    ctrl->exec_counts[3] += 1;
    return result;
}

u32 SVM_ExecSvrFs(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 24;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            ctrl->exec_flags[4] = 1;
            result |= fn(obj);
            ctrl->exec_flags[4] = 0;
        }
    }
    ctrl->exec_counts[4] += 1;
    return result;
}

u32 SVM_ExecSvrMain(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 30;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            ctrl->exec_flags[5] = 1;
            result |= fn(obj);
            ctrl->exec_flags[5] = 0;
        }
    }
    ctrl->exec_counts[5] += 1;
    return result;
}

u32 SVM_ExecSvrMwIdle(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 36;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            ctrl->exec_flags[6] = 1;
            result |= fn(obj);
            ctrl->exec_flags[6] = 0;
        }
    }
    ctrl->exec_counts[6] += 1;
    return result;
}

u32 SVM_ExecSvrUsrIdle(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 42;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            ctrl->exec_flags[7] = 1;
            result |= fn(obj);
            ctrl->exec_flags[7] = 0;
        }
    }
    ctrl->exec_counts[7] += 1;
    return result;
}

void SVM_Init(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (ctrl->init_count == 0) {
        memset(&ctrl->exec_flags[0], 0, 0x20);
        memset(&ctrl->lock_cb, 0, 8);
        memset(&ctrl->unlock_cb, 0, 8);
        memset(&ctrl->field_0x3E8, 0, 8);
        memset(&ctrl->field_0x3F0, 0, 8);
        {
            u32* cnt = ctrl->exec_counts;
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
    ctrl->init_count = ctrl->init_count - 1;
    if (ctrl->init_count != 0)
        return;
    memset(&ctrl->exec_flags[0], 0, 0x20);
    memset(&ctrl->lock_cb, 0, 8);
    memset(&ctrl->unlock_cb, 0, 8);
    memset(&ctrl->field_0x3E8, 0, 8);
    memset(&ctrl->field_0x3F0, 0, 8);
    ctrl->exec_counts[0] = 0;
    ctrl->exec_counts[1] = 0;
    ctrl->exec_counts[2] = 0;
    ctrl->exec_counts[3] = 0;
    ctrl->exec_counts[4] = 0;
    ctrl->exec_counts[5] = 0;
    ctrl->testandset_fn = NULL;
    memset(&ctrl->err_cb, 0, 8);
}

s32 SVM_TestAndSet(u32* p) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (ctrl->testandset_fn != NULL)
        return ((s32 (*)(u32*))ctrl->testandset_fn)(p);
    SVM_LOCK();
    {
        u32 old = *p;
        s32 result;
        *p = 1;
        result = (old == 0) ? 0 : 1;
        SVM_UNLOCK();
        return result;
    }
}
