// Auto-scaffolded catalog TU for CriWare/src/adx/svm/svm
// Replace stubs with high-level C/C++ during decomp.

#include <string.h>

#include <harness_catalog.h>

extern u32 lbl_eu_805F26F0[];
extern const char lbl_eu_80518F50[];

typedef u32 (*SvmExecCb)(void*);

void SVM_Lock(void) {
    u32 fn;
    u32 ctx;
    u32* b = lbl_eu_805F26F0;
    fn = b[4];
    if (fn == 0) return;
    ctx = b[5];
    ((void (*)(u32))fn)(ctx);
    if (b[1] == 0) b[2] = 1;
    b[1]++;
}

void SVM_CallErr(const char*, u32, ...);
void SVM_Unlock(void) {
    u32* b = lbl_eu_805F26F0;
    u32 fn = b[6];
    if (fn == 0) return;
    {
        u32 cnt = b[1];
        b[1] = cnt - 1;
        if (b[1] == 0) {
            if (b[2] != 1)
                SVM_CallErr(lbl_eu_80518F50, b[2], 1);
            b[2] = 0;
        }
    }
    ((void (*)(u32))b[6])(b[7]);
}

void SVM_CallErr(const char*, u32, ...) {}

extern char* CRICRW_Strncpy(char* dst, void* ignored, const char* src, size_t n);

void SVM_CallErr1(const char* msg) {
    u32* base = lbl_eu_805F26F0;
    u32 errFn = base[0x120 / 4];
    void* errCtx = (void*)base[0x124 / 4];
    u32 isNull = (msg == NULL) ? 1 : 0;
    if (msg != NULL) {
        char* buf = (char*)&base[0x20 / 4];
        CRICRW_Strncpy(buf, (void*)0x100, msg, 0xFF);
    }
    if (errFn != 0) {
        if (isNull)
            ((void (*)(void*, u32))errFn)(errCtx, 0);
        else
            ((void (*)(void*, const char*))errFn)(errCtx, (const char*)&base[0x20 / 4]);
    }
}

/* Forward declarations */
void svm_SetCbSvr(u32 svrId, void* fn, void* ctx, const char* name);
void svm_SetCbSvrId(u32 svrId, u32 idx, void* fn, void* ctx, const char* name);

void SVM_SetCbSvrWithString(u32 svrId, u32 fn, u32 ctx, u32 nameOrNull) {
    SVM_Lock();
    svm_SetCbSvr(svrId, (void*)fn, (void*)ctx, nameOrNull ? (const char*)nameOrNull : NULL);
    SVM_Unlock();
}

void svm_SetCbSvr(u32 svrId, void* fn, void* ctx, const char* name) {
    u32* base = lbl_eu_805F26F0;
    u32 errFn;
    u32 i;
    if (svrId > 7) {
        errFn = base[0x120 / 4];
        if (errFn != 0)
            ((void (*)(void*, u32))base[0x124 / 4])((void*)base[0x124 / 4], 0);
        return;
    }
    {
        u32* entryBase = &base[(0x128 / 4) + svrId * (0x48 / 4)];
        for (i = 0; i < 6; i++) {
            if (entryBase[i * 3] == 0) {
                entryBase[i * 3] = (u32)fn;
                entryBase[i * 3 + 1] = (u32)ctx;
                entryBase[i * 3 + 2] = name ? (u32)name : (u32)(lbl_eu_80518F50 + 0x6d);
                return;
            }
        }
    }
    errFn = base[0x120 / 4];
    if (errFn != 0)
        ((void (*)(void*, u32))base[0x124 / 4])((void*)base[0x124 / 4], 0);
}

void SVM_DelCbSvr(u32 svrId, u32 idx) {
    u32* base = lbl_eu_805F26F0;
    SVM_Lock();
    if (idx > 5) { SVM_Unlock(); return; }
    if (svrId > 7) { SVM_Unlock(); return; }
    {
        u32* entry = &base[(0x128 / 4) + svrId * (0x48 / 4) + idx * 3];
        entry[0] = 0;
        entry[1] = 0;
        entry[2] = 0;
    }
    SVM_Unlock();
}

void SVM_SetCbSvrIdWithString(u32 svrId, u32 idx, u32 fn, u32 ctx, u32 nameOrNull) {
    SVM_Lock();
    svm_SetCbSvrId(svrId, idx, (void*)fn, (void*)ctx,
                   nameOrNull ? (const char*)nameOrNull : NULL);
    SVM_Unlock();
}

void svm_SetCbSvrId(u32 svrId, u32 idx, void* fn, void* ctx, const char* name) {
    u32* base = lbl_eu_805F26F0;
    u32* entry;
    if (svrId > 7) return;
    if (idx > 5) return;
    entry = &base[(0x128 / 4) + svrId * (0x48 / 4) + idx * 3];
    if (entry[0] != 0) {
        if (base[0x120 / 4] != 0)
            ((void (*)(void*, u32))base[0x124 / 4])((void*)base[0x124 / 4], 0);
    }
    entry[0] = (u32)fn;
    entry[1] = (u32)ctx;
    entry[2] = name ? (u32)name : (u32)(lbl_eu_80518F50 + 0x6d);
}

void SVM_SetCbBdr(s32 idx, void* fn, void* ctx) {
    u32* base = lbl_eu_805F26F0;
    SVM_Lock();
    base[0x368 / 4 + idx * 2] = (u32)fn;
    base[0x368 / 4 + idx * 2 + 1] = (u32)ctx;
    SVM_Unlock();
}

extern u32 lbl_eu_805F2A58[];
void SVM_GotoSvrBorder(s32 idx) {
    u32* base = lbl_eu_805F2A58;
    u32 fn = base[idx * 2];
    if (fn == 0) return;
    ((void (*)(void*))fn)((void*)base[idx * 2 + 1]);
}

void SVM_SetCbErr(void* fn, void* ctx) {
    u32* base = lbl_eu_805F26F0;
    SVM_Lock();
    base[0x120 / 4] = (u32)fn;
    base[0x124 / 4] = (u32)ctx;
    SVM_Unlock();
}

typedef struct SvmCbPair {
    void* cb;
    void* ctx;
} SvmCbPair;
extern SvmCbPair lbl_eu_805F2700;
void SVM_SetCbLock(void* cb, void* ctx) {
    SvmCbPair* p = &lbl_eu_805F2700;
    p->cb = cb;
    p->ctx = ctx;
}

extern SvmCbPair lbl_eu_805F2708;
void SVM_SetCbUnlock(void* cb, void* ctx) {
    SvmCbPair* p = &lbl_eu_805F2708;
    p->cb = cb;
    p->ctx = ctx;
}

u32 SVM_ExecSvrVint(void) {
    u32* base = lbl_eu_805F26F0;
    u32* tbl = &base[0x128 / 4];
    u32 result = 0;
    s32 i;
    for (i = 0; i < 6; i++) {
        u32 fn = tbl[i * 3];
        void* ctx = (void*)tbl[i * 3 + 1];
        if (fn != 0) {
            base[0x3a8 / 4] = 1;
            result |= ((SvmExecCb)fn)(ctx);
            base[0x3a8 / 4] = 0;
        }
    }
    base[0x3c8 / 4] += 1;
    return result;
}

u32 SVM_ExecSvrUsrVsync(void) {
    u32* base = lbl_eu_805F26F0;
    u32* tbl = &base[(0x128 + 0x48) / 4];
    u32 result = 0;
    s32 i;
    for (i = 0; i < 6; i++) {
        u32 fn = tbl[i * 3];
        void* ctx = (void*)tbl[i * 3 + 1];
        if (fn != 0) {
            base[0x3ac / 4] = 1;
            result |= ((SvmExecCb)fn)(ctx);
            base[0x3ac / 4] = 0;
        }
    }
    base[0x3cc / 4] += 1;
    return result;
}

u32 SVM_ExecSvrVsync(void) {
    u32* base = lbl_eu_805F26F0;
    u32* tbl = &base[(0x128 + 0x90) / 4];
    u32 result = 0;
    s32 i;
    for (i = 0; i < 6; i++) {
        u32 fn = tbl[i * 3];
        void* ctx = (void*)tbl[i * 3 + 1];
        if (fn != 0) {
            base[0x3b0 / 4] = 1;
            result |= ((SvmExecCb)fn)(ctx);
            base[0x3b0 / 4] = 0;
        }
    }
    base[0x3d0 / 4] += 1;
    return result;
}

u32 SVM_ExecSvrUhigh(void) {
    u32* base = lbl_eu_805F26F0;
    u32* tbl = &base[(0x128 + 0xd8) / 4];
    u32 result = 0;
    s32 i;
    for (i = 0; i < 6; i++) {
        u32 fn = tbl[i * 3];
        void* ctx = (void*)tbl[i * 3 + 1];
        if (fn != 0) {
            base[0x3b4 / 4] = 1;
            result |= ((SvmExecCb)fn)(ctx);
            base[0x3b4 / 4] = 0;
        }
    }
    base[0x3d4 / 4] += 1;
    return result;
}

u32 SVM_ExecSvrFs(void) {
    u32* base = lbl_eu_805F26F0;
    u32* tbl = &base[(0x128 + 0x120) / 4];
    u32 result = 0;
    s32 i;
    for (i = 0; i < 6; i++) {
        u32 fn = tbl[i * 3];
        void* ctx = (void*)tbl[i * 3 + 1];
        if (fn != 0) {
            base[0x3b8 / 4] = 1;
            result |= ((SvmExecCb)fn)(ctx);
            base[0x3b8 / 4] = 0;
        }
    }
    base[0x3d8 / 4] += 1;
    return result;
}

u32 SVM_ExecSvrMain(void) {
    u32* base = lbl_eu_805F26F0;
    u32* tbl = &base[(0x128 + 0x168) / 4];
    u32 result = 0;
    s32 i;
    for (i = 0; i < 6; i++) {
        u32 fn = tbl[i * 3];
        void* ctx = (void*)tbl[i * 3 + 1];
        if (fn != 0) {
            base[0x3bc / 4] = 1;
            result |= ((SvmExecCb)fn)(ctx);
            base[0x3bc / 4] = 0;
        }
    }
    base[0x3dc / 4] += 1;
    return result;
}

u32 SVM_ExecSvrMwIdle(void) {
    u32* base = lbl_eu_805F26F0;
    u32* tbl = &base[(0x128 + 0x1b0) / 4];
    u32 result = 0;
    s32 i;
    for (i = 0; i < 6; i++) {
        u32 fn = tbl[i * 3];
        void* ctx = (void*)tbl[i * 3 + 1];
        if (fn != 0) {
            base[0x3c0 / 4] = 1;
            result |= ((SvmExecCb)fn)(ctx);
            base[0x3c0 / 4] = 0;
        }
    }
    base[0x3e0 / 4] += 1;
    return result;
}

u32 SVM_ExecSvrUsrIdle(void) {
    u32* base = lbl_eu_805F26F0;
    u32* tbl = &base[(0x128 + 0x1f8) / 4];
    u32 result = 0;
    s32 i;
    for (i = 0; i < 6; i++) {
        u32 fn = tbl[i * 3];
        void* ctx = (void*)tbl[i * 3 + 1];
        if (fn != 0) {
            base[0x3c4 / 4] = 1;
            result |= ((SvmExecCb)fn)(ctx);
            base[0x3c4 / 4] = 0;
        }
    }
    base[0x3e4 / 4] += 1;
    return result;
}

void SVM_Init(void) {
    u32* base = lbl_eu_805F26F0;
    if (base[0] != 0) goto inc;
    memset(&base[0x3a8 / 4], 0, 0x20);
    memset(&base[0x10 / 4], 0, 8);
    memset(&base[0x18 / 4], 0, 8);
    memset(&base[0x3e8 / 4], 0, 8);
    memset(&base[0x3f0 / 4], 0, 8);
    base[0x3c8 / 4] = 0;
    base[0x3cc / 4] = 0;
    base[0x3d0 / 4] = 0;
    base[0x3d4 / 4] = 0;
    base[0x3d8 / 4] = 0;
    base[0x3dc / 4] = 0;
    base[0x3f8 / 4] = 0;
inc:
    base[0] += 1;
}

void SVM_Finish(void) {
    u32* base = lbl_eu_805F26F0;
    base[0] -= 1;
    if (base[0] != 0) return;
    memset(&base[0x3a8 / 4], 0, 0x20);
    memset(&base[0x10 / 4], 0, 8);
    memset(&base[0x18 / 4], 0, 8);
    memset(&base[0x3e8 / 4], 0, 8);
    memset(&base[0x3f0 / 4], 0, 8);
    base[0x3c8 / 4] = 0;
    base[0x3cc / 4] = 0;
    base[0x3d0 / 4] = 0;
    base[0x3d4 / 4] = 0;
    base[0x3d8 / 4] = 0;
    base[0x3dc / 4] = 0;
    base[0x3f8 / 4] = 0;
    memset(&base[0x120 / 4], 0, 8);
}

s32 SVM_TestAndSet(u32* p) {
    u32* base = lbl_eu_805F26F0;
    u32 fn = base[0x3f8 / 4];
    if (fn != 0)
        return ((s32(*)(u32*))fn)(p);
    SVM_Lock();
    {
        u32 old = *p;
        *p = 1;
        {
            s32 result = (old == 0) ? 0 : 1;
            SVM_Unlock();
            return result;
        }
    }
}
