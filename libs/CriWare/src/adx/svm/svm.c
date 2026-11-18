// svm.c - CRI ADX server manager (server callback table, lock/unlock and
// error-callback plumbing).
//
// Retail keeps the work areas as separate .bss objects (init/lock counters,
// the two lock callbacks, the message buffer, the error callback + server
// table, and the exec/border area); references to them are merged onto one
// base register per function by MWCC's address folder.

#include <harness_catalog.h>
#include <string.h>

// Cross-TU imports (C linkage): the CRI runtime string helpers.
extern char* CRICRW_Strncpy(char* dst, void* ignored, const char* src, size_t n);
extern int CRICRW_Vsprintf(char* s, void* ignored, const char* fmt, va_list ap);

void SVM_CallErr(const char* fmt, ...);

// Message pool. Entry offsets are the retail byte offsets into this array.
const char svm_mes[] =
    "2103102:SVM:svm_unlock:lock type miss match.(type org=%d, type now=%d)\0"
    "1071205:SVM_SetCbSvrId:illegal svtype\0"
    "Unknown\0"
    "1051001:SVM_SetCbSvr:too many server function\0"
    "1051002:SVM_DelCbSvr:illegal id\0"
    "1071206:SVM_SetCbSvrId:illegal svtype\0"
    "1071201:SVM_SetCbSvrId:illegal id\0"
    "1071202:SVM_SetCbSvrId:illegal svtype\0"
    "2100801:SVM_SetCbSvrId:over write callback function.\0"
    "1071301:SVM_ExecSvrFuncId:illegal id\0"
    "1071302:SVM_ExecSvrFuncId:illegal svtype\0"
    "\0\0\0";

#define SVM_MES_NONE        0x000 // "2103102:SVM:svm_unlock:..."
#define SVM_MES_SVRTYPE1    0x047 // "1071205:SVM_SetCbSvrId:illegal svtype"
#define SVM_MES_UNKNOWN     0x06d // "Unknown"
#define SVM_MES_TOOMANY     0x075 // "1051001:SVM_SetCbSvr:too many server function"
#define SVM_MES_ILLEGAL_ID  0x0a3 // "1051002:SVM_DelCbSvr:illegal id"
#define SVM_MES_SVRTYPE2    0x0c3 // "1071206:SVM_SetCbSvrId:illegal svtype"
#define SVM_MES_ID1         0x0e9 // "1071201:SVM_SetCbSvrId:illegal id"
#define SVM_MES_SVRTYPE3    0x10b // "1071202:SVM_SetCbSvrId:illegal svtype"
#define SVM_MES_OVERWRITE   0x131 // "2100801:SVM_SetCbSvrId:over write callback function."

// User callback pair (function + its context pointer).
typedef struct SvmCb {
    void (*func)(void*);
    void* object;
} SvmCb;

// Counters shared by the lock/unlock pair.
typedef struct SvmCtrl {
    u32 init_count; // 0x00 init nesting
    u32 lock_count; // 0x04 lock nesting
    s32 lock_flag;  // 0x08 current lock type
    u32 work_flag;  // 0x0C
} SvmCtrl;

// Error callback pair.
typedef struct SvmErrCb {
    void (*func)(void*, const char*);
    void* object;
} SvmErrCb;

// One server entry: the server function, its context and its (optional) name.
typedef struct SvmSvrEntry {
    void* func;
    void* object;
    const char* name;
} SvmSvrEntry;

SvmCtrl svm_ctrl;                   // 0x000 lock counters
SvmCb svm_lock_cb;                  // 0x010 user lock callback
SvmCb svm_unlock_cb;                // 0x018 user unlock callback
char svm_err_msg[0x100];            // 0x020 error message buffer
SvmErrCb svm_err_cb;                // 0x120 user error callback
static SvmSvrEntry svm_svr[8][6];   // 0x128 server callback table
SvmCb svm_bdr_cb[8];                // 0x368 border callbacks
static u32 svm_flags[8];            // 0x3A8 per-server "executing" flags
static u32 svm_count[8];            // 0x3C8 per-server execution counters
static u32 svm_lock_work[2];        // 0x3E8
static u32 svm_unlock_work[2];      // 0x3F0
static void* svm_testandset;        // 0x3F8 hardware test-and-set hook
u64 lbl_eu_805F2AF0;                // 0x400

typedef u32 (*SvmSvrFunc)(void*);

s32 svm_SetCbSvr(u32 svrId, void* func, void* object, const char* name);
void svm_SetCbSvrId(u32 svrId, u32 slot, void* func, void* object, const char* name);

// Lock the server manager: notify the user lock callback and bump the nesting
// counter (the innermost lock records the lock type).
void SVM_Lock(void) {
    SvmCtrl* ctrl = &svm_ctrl;
    if (svm_lock_cb.func != 0) {
        svm_lock_cb.func(svm_lock_cb.object);
        if (ctrl->lock_count == 0)
            ctrl->lock_flag = 1;
        ctrl->lock_count++;
    }
}

// Unlock the server manager (checking the recorded lock type on the outermost
// unlock) and notify the user unlock callback.
void SVM_Unlock(void) {
    if (svm_unlock_cb.func != 0) {
        svm_ctrl.lock_count--;
        if (svm_ctrl.lock_count == 0) {
            if (svm_ctrl.lock_flag != 1)
                SVM_CallErr(svm_mes, svm_ctrl.lock_flag, 1);
            svm_ctrl.lock_flag = 0;
        }
        svm_unlock_cb.func(svm_unlock_cb.object);
    }
}

// Report a formatted error message to the user error callback.
void SVM_CallErr(const char* fmt, ...) {
    va_list ap;
    memset(svm_err_msg, 0, 0x100);
    va_start(ap, fmt);
    CRICRW_Vsprintf(svm_err_msg, (void*)0x100, fmt, ap);
    if (svm_err_cb.func != 0)
        svm_err_cb.func(svm_err_cb.object, svm_err_msg);
}

// Report one fixed error message to the user error callback (a NULL message is
// passed through as-is).
void SVM_CallErr1(const char* msg) {
    if (msg == 0) {
        svm_err_cb.func(svm_err_cb.object, 0);
    } else {
        CRICRW_Strncpy(svm_err_msg, (void*)0x100, msg, 0xFF);
        if (svm_err_cb.func != 0)
            svm_err_cb.func(svm_err_cb.object, svm_err_msg);
    }
}

// Register a server callback (locking the manager around the update).
s32 SVM_SetCbSvrWithString(u32 svrId, void* func, void* object, const char* name) {
    s32 ret;
    SVM_Lock();
    ret = svm_SetCbSvr(svrId, func, object, name);
    SVM_Unlock();
    return ret;
}

// Register a server callback.
s32 svm_SetCbSvr(u32 svrId, void* func, void* object, const char* name) {
    s32 i;
    if (svrId > 7) {
        SVM_CallErr1(svm_mes + SVM_MES_SVRTYPE1);
        return -1;
    }
    for (i = 0; i < 6; i++) {
        SvmSvrEntry* ent = &svm_svr[svrId][i];
        if (ent->func == 0) {
            ent->func = func;
            ent->object = object;
            if (name != 0)
                ent->name = name;
            else
                ent->name = svm_mes + SVM_MES_UNKNOWN;
            break;
        }
    }
    if (i == 6) {
        SVM_CallErr1(svm_mes + SVM_MES_TOOMANY);
        return -1;
    }
    return i;
}

// Unregister a server callback.
void SVM_DelCbSvr(u32 svrId, u32 slot) {
    SVM_Lock();
    if (slot > 5) {
        SVM_CallErr1(svm_mes + SVM_MES_ILLEGAL_ID);
    } else if (svrId > 7) {
        SVM_CallErr1(svm_mes + SVM_MES_SVRTYPE2);
    } else {
        SvmSvrEntry* ent = &svm_svr[svrId][slot];
        ent->func = 0;
        ent->object = 0;
        ent->name = 0;
    }
    SVM_Unlock();
}

// Register a server callback at an explicit slot (locking the manager).
void SVM_SetCbSvrIdWithString(u32 svrId, u32 slot, void* func, void* object, const char* name) {
    SVM_Lock();
    svm_SetCbSvrId(svrId, slot, func, object, name);
    SVM_Unlock();
}

// Register a server callback at an explicit slot.
void svm_SetCbSvrId(u32 svrId, u32 slot, void* func, void* object, const char* name) {
    SvmSvrEntry* ent;
    if (slot > 5) {
        SVM_CallErr1(svm_mes + SVM_MES_ID1);
        return;
    }
    if (svrId > 7) {
        SVM_CallErr1(svm_mes + SVM_MES_SVRTYPE3);
        return;
    }
    ent = &svm_svr[svrId][slot];
    if (ent->func != 0)
        SVM_CallErr1(svm_mes + SVM_MES_OVERWRITE);
    ent->func = func;
    ent->object = object;
    if (name != 0)
        ent->name = name;
    else
        ent->name = svm_mes + SVM_MES_UNKNOWN;
}

// Register a border callback.
void SVM_SetCbBdr(u32 idx, void* func, void* object) {
    SVM_Lock();
    svm_bdr_cb[idx].func = func;
    svm_bdr_cb[idx].object = object;
    SVM_Unlock();
}

// Run the border callback of the given border.
void SVM_GotoSvrBorder(u32 idx) {
    if (svm_bdr_cb[idx].func != 0)
        svm_bdr_cb[idx].func(svm_bdr_cb[idx].object);
}

// Register the user error callback.
void SVM_SetCbErr(void* func, void* object) {
    SVM_Lock();
    svm_err_cb.func = (void (*)(void*, const char*))func;
    svm_err_cb.object = object;
    SVM_Unlock();
}

// Register the user lock callback.
void SVM_SetCbLock(void* func, void* object) {
    svm_lock_cb.func = (void (*)(void*))func;
    svm_lock_cb.object = object;
}

// Register the user unlock callback.
void SVM_SetCbUnlock(void* func, void* object) {
    svm_unlock_cb.func = (void (*)(void*))func;
    svm_unlock_cb.object = object;
}

// Run every server callback of one server: each callback is bracketed by the
// server's "executing" flag, and the results are OR-ed together.
static u32 svm_ExecSvr(s32 svrId) {
    SvmSvrEntry* ent = &svm_svr[svrId][0];
    s32 i;
    u32 result;
    result = 0;
    i = 0;
    for (; i < 6; i++, ent++) {
        SvmSvrFunc fn = (SvmSvrFunc)ent->func;
        void* obj = ent->object;
        if (fn != 0) {
            svm_flags[svrId] = 1;
            result |= fn(obj);
            svm_flags[svrId] = 0;
        }
    }
    svm_count[svrId]++;
    return result;
}

u32 SVM_ExecSvrVint(void) {
    return svm_ExecSvr(0);
}

u32 SVM_ExecSvrUsrVsync(void) {
    return svm_ExecSvr(1);
}

u32 SVM_ExecSvrVsync(void) {
    return svm_ExecSvr(2);
}

u32 SVM_ExecSvrUhigh(void) {
    return svm_ExecSvr(3);
}

u32 SVM_ExecSvrFs(void) {
    return svm_ExecSvr(4);
}

u32 SVM_ExecSvrMain(void) {
    return svm_ExecSvr(5);
}

u32 SVM_ExecSvrMwIdle(void) {
    return svm_ExecSvr(6);
}

u32 SVM_ExecSvrUsrIdle(void) {
    return svm_ExecSvr(7);
}

// Initialize the server manager (reference counted).
void SVM_Init(void) {
    if (svm_ctrl.init_count == 0) {
        memset(svm_flags, 0, 0x20);
        memset(&svm_lock_cb, 0, 8);
        memset(&svm_unlock_cb, 0, 8);
        memset(svm_lock_work, 0, 8);
        memset(svm_unlock_work, 0, 8);
        {
            u32* p = svm_count;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
        }
        svm_testandset = 0;
    }
    svm_ctrl.init_count++;
}

// Shut the server manager down (reference counted).
void SVM_Finish(void) {
    svm_ctrl.init_count = svm_ctrl.init_count - 1;
    if (svm_ctrl.init_count == 0) {
    memset(svm_flags, 0, 0x20);
    memset(&svm_lock_cb, 0, 8);
    memset(&svm_unlock_cb, 0, 8);
    memset(svm_lock_work, 0, 8);
    memset(svm_unlock_work, 0, 8);
    {
        u32* p = svm_count;
        *p++ = 0;
        *p++ = 0;
        *p++ = 0;
        *p++ = 0;
        *p++ = 0;
        *p++ = 0;
    }
    svm_testandset = 0;
    memset(&svm_err_cb, 0, 8);
    }
}

// Atomically set a word to 1 and report whether it already was 1; uses the
// hardware hook when the platform installed one.
s32 SVM_TestAndSet(u32* flag) {
    u32 old;
    s32 result;
    if (svm_testandset != 0) {
        result = ((s32 (*)(u32*))svm_testandset)(flag);
    } else {
        SVM_Lock();
        old = *flag;
        *flag = 1;
        result = (old != 1);
        SVM_Unlock();
    }
    return result;
}
