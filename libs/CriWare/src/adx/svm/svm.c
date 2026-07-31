// svm.c - CRI ADX Server Manager
// Reconstructed from retail svm.s (0x80399B0C..0x8039AD88).

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
    u32 init_count;            /* 0x000 */
    u32 lock_count;            /* 0x004 */
    u32 lock_flag;             /* 0x008 */
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

/* Report an error message through the error callback; NULL means no message. */
static void svm_err_cb_msg(const char* msg) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (msg == NULL) {
        void (*fn)(void*, const void*) = ctrl->err_cb.func;
        void* obj = ctrl->err_cb.object;
        fn(obj, NULL);
    } else {
        CRICRW_Strncpy(ctrl->err_msg, (void*)0x100, msg, 0xFF);
        if (ctrl->err_cb.func != NULL) {
            void (*fn)(void*, const void*) = ctrl->err_cb.func;
            void* obj = ctrl->err_cb.object;
            fn(obj, ctrl->err_msg);
        }
    }
}

static void svm_lock(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (ctrl->lock_cb.func != NULL) {
        void (*fn)(void*) = ctrl->lock_cb.func;
        void* obj = ctrl->lock_cb.object;
        fn(obj);
        if (ctrl->lock_count == 0)
            ctrl->lock_flag = 1;
        ctrl->lock_count++;
    }
}

static void svm_unlock(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (ctrl->unlock_cb.func != NULL) {
        ctrl->lock_count--;
        if (ctrl->lock_count == 0) {
            if (ctrl->lock_flag != 1)
                SVM_CallErr(lbl_eu_80518F50, ctrl->lock_flag, 1);
            ctrl->lock_flag = 0;
        }
        {
            void (*fn)(void*) = ctrl->unlock_cb.func;
            void* obj = ctrl->unlock_cb.object;
            fn(obj);
        }
    }
}

void SVM_Lock(void) {
    svm_lock();
}

void SVM_Unlock(void) {
    svm_unlock();
}

void SVM_CallErr(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    memset(lbl_eu_805F2710, 0, 0x100);
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
    svm_lock();
    ret = svm_SetCbSvr(svrId, fn, ctx, name);
    svm_unlock();
    return ret;
}

s32 svm_SetCbSvr(u32 svrId, void* fn, void* ctx, const char* name) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    s32 i;
    if (svrId > 7) {
        svm_err_cb_msg(&lbl_eu_80518F50[0x47]);
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
    svm_err_cb_msg(&lbl_eu_80518F50[0x75]);
    return -1;
}

void SVM_DelCbSvr(u32 svrId, u32 idx) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    svm_lock();
    if (idx > 5) {
        svm_err_cb_msg(&lbl_eu_80518F50[0xA3]);
    } else if (svrId > 7) {
        svm_err_cb_msg(&lbl_eu_80518F50[0xC3]);
    } else {
        ctrl->svr_tbl[svrId][idx].func = NULL;
        ctrl->svr_tbl[svrId][idx].object = NULL;
        ctrl->svr_tbl[svrId][idx].name = NULL;
    }
    svm_unlock();
}

void SVM_SetCbSvrIdWithString(u32 svrId, u32 idx, void* fn, void* ctx, const char* name) {
    svm_lock();
    svm_SetCbSvrId(svrId, idx, fn, ctx, name);
    svm_unlock();
}

void svm_SetCbSvrId(u32 svrId, u32 idx, void* fn, void* ctx, const char* name) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (idx > 5) {
        svm_err_cb_msg(&lbl_eu_80518F50[0xE9]);
        return;
    }
    if (svrId > 7) {
        svm_err_cb_msg(&lbl_eu_80518F50[0x10B]);
        return;
    }
    {
        SvmSvrEntry* entry = &ctrl->svr_tbl[svrId][idx];
        if (entry->func != NULL)
            svm_err_cb_msg(&lbl_eu_80518F50[0x131]);
        entry->func = (u32 (*)(void*))fn;
        entry->object = ctx;
        entry->name = name != NULL ? name : &lbl_eu_80518F50[0x6D];
    }
}

void SVM_SetCbBdr(s32 idx, void* fn, void* ctx) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    svm_lock();
    ctrl->bdr_tbl[idx].func = (void (*)(void*))fn;
    ctrl->bdr_tbl[idx].object = ctx;
    svm_unlock();
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
    svm_lock();
    ctrl->err_cb.func = (void (*)(void*, const void*))fn;
    ctrl->err_cb.object = ctx;
    svm_unlock();
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
    u32* flag = &ctrl->exec_flags[0];
    u32* cnt = &ctrl->exec_counts[0];
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 6;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flag[1] = 1;
            result |= fn(obj);
            flag[1] = 0;
        }
    }
    cnt[1] += 1;
    return result;
}

u32 SVM_ExecSvrVsync(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    u32* flag = &ctrl->exec_flags[0];
    u32* cnt = &ctrl->exec_counts[0];
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 12;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flag[2] = 1;
            result |= fn(obj);
            flag[2] = 0;
        }
    }
    cnt[2] += 1;
    return result;
}

u32 SVM_ExecSvrUhigh(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    u32* flag = &ctrl->exec_flags[0];
    u32* cnt = &ctrl->exec_counts[0];
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 18;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flag[3] = 1;
            result |= fn(obj);
            flag[3] = 0;
        }
    }
    cnt[3] += 1;
    return result;
}

u32 SVM_ExecSvrFs(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    u32* flag = &ctrl->exec_flags[0];
    u32* cnt = &ctrl->exec_counts[0];
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 24;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flag[4] = 1;
            result |= fn(obj);
            flag[4] = 0;
        }
    }
    cnt[4] += 1;
    return result;
}

u32 SVM_ExecSvrMain(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    u32* flag = &ctrl->exec_flags[0];
    u32* cnt = &ctrl->exec_counts[0];
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 30;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flag[5] = 1;
            result |= fn(obj);
            flag[5] = 0;
        }
    }
    cnt[5] += 1;
    return result;
}

u32 SVM_ExecSvrMwIdle(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    u32* flag = &ctrl->exec_flags[0];
    u32* cnt = &ctrl->exec_counts[0];
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 36;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flag[6] = 1;
            result |= fn(obj);
            flag[6] = 0;
        }
    }
    cnt[6] += 1;
    return result;
}

u32 SVM_ExecSvrUsrIdle(void) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    u32 result = 0;
    s32 i;
    u32* flag = &ctrl->exec_flags[0];
    u32* cnt = &ctrl->exec_counts[0];
    SvmSvrEntry* p = ctrl->svr_tbl[0] + 42;
    for (i = 0; i < 6; i++, p++) {
        u32 (*fn)(void*) = p->func;
        void* obj = p->object;
        if (fn != NULL) {
            flag[7] = 1;
            result |= fn(obj);
            flag[7] = 0;
        }
    }
    cnt[7] += 1;
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
            cnt[0] = cnt[1] = cnt[2] = cnt[3] = cnt[4] = cnt[5] = 0;
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
    memset(&ctrl->exec_flags[0], 0, 0x20);
    memset(&ctrl->lock_cb, 0, 8);
    memset(&ctrl->unlock_cb, 0, 8);
    memset(&ctrl->field_0x3E8, 0, 8);
    memset(&ctrl->field_0x3F0, 0, 8);
    {
        u32* cnt = ctrl->exec_counts;
        cnt[0] = cnt[1] = cnt[2] = cnt[3] = cnt[4] = cnt[5] = 0;
    }
    ctrl->testandset_fn = NULL;
    memset(&ctrl->err_cb, 0, 8);
}

s32 SVM_TestAndSet(u32* p) {
    SvmCtrl* ctrl = &lbl_eu_805F26F0;
    if (ctrl->testandset_fn != NULL)
        return ((s32 (*)(u32*))ctrl->testandset_fn)(p);
    svm_lock();
    {
        u32 old = *p;
        s32 result;
        *p = 1;
        result = (old == 0) ? 0 : 1;
        svm_unlock();
        return result;
    }
}
