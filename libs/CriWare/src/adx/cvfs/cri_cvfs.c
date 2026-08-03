// Auto-scaffolded catalog TU for CriWare/src/adx/cvfs/cri_cvfs
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef void (*CvFsErrFn)(void *arg, const char *msg, int level);

extern CvFsErrFn lbl_eu_805E66E8;
extern void* lbl_eu_805E66EC;
extern char lbl_eu_80517640[];

typedef struct {
    void **vtbl; /* +0x00 */
    void *data;  /* +0x04 */
} CvFsHandle;

void cvFsStopTr(void *hndl) {
    CvFsHandle *h = (CvFsHandle *)hndl;
    void (*stopfn)(void *);

    if (h == NULL) {
        if (lbl_eu_805E66E8 != NULL) {
            lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0x283, 0);
        }
        return;
    }
    stopfn = (void (*)(void *))h->vtbl[0x28 / 4];
    if (stopfn != NULL) {
        stopfn(h->data);
        return;
    }
    if (lbl_eu_805E66E8 != NULL) {
        lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0x29E, 0);
    }
}

void cvFsCallUsrErrFn(void) {
    CvFsErrFn fn = lbl_eu_805E66E8;
    if (fn == NULL) return;
    ((void (*)(void *))fn)(lbl_eu_805E66EC);
}

void cvFsAddDev() {}

void cvFsSetDefDev(const char* name) {
    u8* g = (u8*)&lbl_eu_805E66E8;
    s32 len;
    s32 i;
    s32 found;
    if (name == NULL) {
        const char* msg = &lbl_eu_80517640[137];
        if (*(CvFsErrFn*)g != NULL)
            (*(CvFsErrFn*)g)(*(void**)(g + 4), msg, 0);
        return;
    }
    len = strlen(name);
    if (len == 0) {
        *(u8*)(g + 848) = 0;
        return;
    }
    for (i = 0; i <= len; i++) {
        if ((u8)(name[i] - 'a') <= 25) ((char*)name)[i] -= 32;
    }
    found = 0;
    for (i = 0; i < 32; i++) {
        if (strncmp(name, (const char*)(g + 336 + i * 16 + 4), len) == 0) {
            found = 1;
            break;
        }
    }
    if (found) {
        memcpy(g + 848, name, len + 1);
    } else {
        const char* msg = &lbl_eu_80517640[174];
        if (*(CvFsErrFn*)g != NULL)
            (*(CvFsErrFn*)g)(*(void**)(g + 4), msg, 0);
    }
}

void cvFsOpen() {}

void getDevName() {}

void cvFsClose(void *hndl) {
    CvFsHandle *h = (CvFsHandle *)hndl;
    if (h == NULL) {
        CvFsErrFn fn = (CvFsErrFn)lbl_eu_805E66E8;
        if (fn != NULL) {
            fn(lbl_eu_805E66EC, lbl_eu_80517640 + 0x18D, 0);
        }
    } else if (h->vtbl[0x14 / 4] != NULL) {
        ((void (*)(void *))h->vtbl[0x14 / 4])(h->data);
        h->data = NULL;
        h->vtbl = NULL;
    } else {
        CvFsErrFn fn = (CvFsErrFn)lbl_eu_805E66E8;
        if (fn != NULL) {
            fn(lbl_eu_805E66EC, lbl_eu_80517640 + 0x1A7, 0);
        }
    }
}

s32 cvFsTell(void *hndl) {
    CvFsHandle *h = (CvFsHandle *)hndl;
    s32 ret;
    if (h == NULL) {
        if (lbl_eu_805E66E8 != NULL) {
            lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0x1BF, 0);
        }
        return 0;
    }
    if (h->vtbl[0x1C / 4] != NULL) {
        ret = ((s32 (*)(void *))h->vtbl[0x1C / 4])(h->data);
    } else {
        ret = 0;
        if (lbl_eu_805E66E8 != NULL) {
            lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0x1D8, 0);
        }
    }
    return ret;
}

s32 cvFsSeek(void *hndl, s32 offset, s32 origin) {
    CvFsHandle *h = (CvFsHandle *)hndl;
    s32 ret;
    if (h == NULL) {
        if (lbl_eu_805E66E8 != NULL) {
            lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0x1EF, 0);
        }
        return 0;
    }
    if (h->vtbl[0x18 / 4] != NULL) {
        ret = ((s32 (*)(void *, s32, s32))h->vtbl[0x18 / 4])(h->data, offset, origin);
    } else {
        ret = 0;
        if (lbl_eu_805E66E8 != NULL) {
            lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0x208, 0);
        }
    }
    return ret;
}

s32 cvFsReqRd(void *hndl, void *buf, s32 size) {
    CvFsHandle *h = (CvFsHandle *)hndl;
    s32 ret;
    if (h == NULL) {
        if (lbl_eu_805E66E8 != NULL) {
            lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0x21F, 0);
        }
        return 0;
    }
    if (h->vtbl[0x20 / 4] != NULL) {
        ret = ((s32 (*)(void *, void *, s32))h->vtbl[0x20 / 4])(h->data, buf, size);
    } else {
        ret = 0;
        if (lbl_eu_805E66E8 != NULL) {
            lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0x239, 0);
        }
    }
    return ret;
}

extern u32 lbl_eu_805E6838[];

void cvFsExecServer(void) {
    s32 i;
    for (i = 0; i < 0x20; i++) {
        void *obj = (void *)lbl_eu_805E6838[i * 4];
        if (obj != NULL) {
            void (*fn)(void *) = *(void (**)(void *))obj;
            if (fn != NULL) {
                fn(obj);
            }
        }
    }
}

s32 cvFsGetStat(void *hndl) {
    CvFsHandle *h = (CvFsHandle *)hndl;
    s32 ret = 3;
    if (h == NULL) {
        if (lbl_eu_805E66E8 != NULL) {
            lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0x2B7, 0);
        }
        return 3;
    }
    if (h->vtbl[0x2C / 4] != NULL) {
        ret = ((s32 (*)(void *))h->vtbl[0x2C / 4])(h->data);
    } else {
        if (lbl_eu_805E66E8 != NULL) {
            lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0x2D3, 0);
        }
    }
    return ret;
}

void cvFsGetFileSize() {}

void cvFsEntryErrFunc(CvFsErrFn fn, void* arg) {
    if (fn == NULL) {
        lbl_eu_805E66E8 = NULL;
        lbl_eu_805E66EC = NULL;
    } else {
        lbl_eu_805E66E8 = fn;
        lbl_eu_805E66EC = arg;
    }
}

s32 cvFsIsAvailableRetry(void *hndl) {
    CvFsHandle *h = (CvFsHandle *)hndl;
    if (h == NULL) {
        if (lbl_eu_805E66E8 != NULL) {
            lbl_eu_805E66E8(lbl_eu_805E66EC, lbl_eu_80517640 + 0xB7D, 0);
        }
        return 0;
    }
    if (h->vtbl[0x60 / 4] != NULL) {
        return ((s32 (*)(void *, s32, s32, s32))h->vtbl[0x60 / 4])(h->data, 0x190, 0, 0);
    }
    return 1;
}
