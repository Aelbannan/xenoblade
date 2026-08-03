// Auto-scaffolded catalog TU for CriWare/src/adx/mfci/mfci
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_80518840;
extern char lbl_eu_80565B98[];
extern char lbl_eu_80518844[];
extern u32 lbl_eu_805EC454;
extern void (*lbl_eu_805EC450)(u32, const char*, int);

extern void CRICRW_Sprintf(char* buf, int size, const char* fmt, ...);
extern void CRICRW_Strcpy(char* dst, int maxlen, const char* src);
extern void SVM_Lock(void);
extern void SVM_Unlock(void);
extern unsigned long strlen(const char* s);
extern unsigned long strtoul(const char* nptr, char** endptr, int base);

void* mfCiGetInterface(void) {
    *(volatile u32*)&lbl_eu_80518840;
    return lbl_eu_80565B98;
}

int mfci_str_to_uint_ptr(const char* str, char** endptr, int base) {
    int value = 0;
    for (;;) {
        u8 c = (u8)*str;
        int d;
        if ((unsigned char)(c - '0') <= 9) {
            d = (s8)c - '0';
        } else if ((unsigned char)(c - 'a') <= 5) {
            d = (s8)c - 'a' + 10;
        } else if ((unsigned char)(c - 'A') <= 5) {
            d = (s8)c - 'A' + 10;
        } else {
            break;
        }
        value = value * base + d;
        str++;
    }
    *endptr = (char*)str;
    return value;
}

void mfCiExecServer() {}

void mfCiEntryErrFunc(u32 a, u32 b) {
    lbl_eu_805EC450 = (void (*)(u32, const char*, int))a;
    lbl_eu_805EC454 = b;
}

typedef struct MfCiFile {
    s8 flag;          // +0x00
    s8 status;        // +0x01
    u8 pad02[2];      // +0x02
    s32 sctsize;      // +0x04
    s32 fileSize;     // +0x08
    s32 totalSct;     // +0x0C
    s32 pos;          // +0x10
    s32 numTr;        // +0x14
    s32 trSize;       // +0x18
    char name[0x14];  // +0x1C
    s32 rdOff;        // +0x30
    s32 rdSize;       // +0x34
} MfCiFile;           // 0x38 bytes

typedef struct MfCiGlobals {
    void (*errfn)(u32, const char*, int);  // +0x00
    u32 errarg;                            // +0x04
    char errbuf[0x130];                    // +0x08
    MfCiFile files[0x28];                  // +0x138
} MfCiGlobals;

int mfCiGetFileSize(const char* name) {
    MfCiGlobals* g = (MfCiGlobals*)&lbl_eu_805EC450;
    int size = 0;
    const char* p;
    int* sizePtr = &size;
    if (strlen(name) >= 0x12) {
        CRICRW_Sprintf(g->errbuf, 0x12C, lbl_eu_80518844, name, 0x12);
        if (g->errfn != NULL)
            g->errfn(g->errarg, g->errbuf, 0);
    }
    p = name;
    mfci_str_to_uint_ptr(p, (char**)&p, 0x10);
    if (*p != 0) p++;
    *sizePtr = (int)strtoul(p, (char**)&p, 0x10);
    return size;
}

void* mfCiOpen(const char* path, int mode) {
    MfCiFile* sct;
    MfCiGlobals* g = (MfCiGlobals*)&lbl_eu_805EC450;
    int i;

    if (path == NULL) {
        const char* msg = lbl_eu_80518844 + 0x3E;
        if (g->errfn != NULL)
            g->errfn(g->errarg, msg, 0);
        return NULL;
    }
    if (mode != 0) {
        const char* msg = lbl_eu_80518844 + 0x61;
        if (g->errfn != NULL)
            g->errfn(g->errarg, msg, 0);
        return NULL;
    }
    sct = NULL;
    {
        MfCiFile* p = g->files;
        for (i = 0; i < 0x28; i++) {
            if (p->flag == 0) {
                sct = &g->files[i];
                break;
            }
            p++;
        }
    }
    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0x84;
        if (g->errfn != NULL)
            g->errfn(g->errarg, msg, 0);
        return NULL;
    }
    CRICRW_Strcpy(sct->name, 0x12, path);
    sct->sctsize = 0x800;
    if (strlen(sct->name) >= 0x12) {
        CRICRW_Sprintf(g->errbuf, 0x12C, lbl_eu_80518844, sct->name, 0x12);
        if (g->errfn != NULL)
            g->errfn(g->errarg, g->errbuf, 0);
    }
    {
        char* p = sct->name;
        mfci_str_to_uint_ptr(p, &p, 0x10);
        if (*p != 0) p++;
        sct->fileSize = (int)strtoul(p, &p, 0x10);
    }
    sct->totalSct = (sct->fileSize + sct->sctsize - 1) / sct->sctsize;
    sct->pos = 0;
    sct->trSize = 0;
    sct->numTr = 0;
    sct->status = 0;
    sct->flag = 1;
    return sct;
}

void mfCiClose(MfCiFile* sct) {
    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0xB4;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, 0);
        return;
    }
    SVM_Lock();
    sct->status = 0;
    SVM_Unlock();
    if (sct->flag == 1) {
        sct->flag = 0;
        memset(sct, 0, 0x38);
    }
}

int mfCiSeek(MfCiFile* sct, int offset, int whence) {
    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0xCC;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, 0);
        return 0;
    }
    SVM_Lock();
    if (whence == 0) {
        sct->pos = offset;
    } else if (whence == 2) {
        sct->pos = sct->totalSct + offset;
    } else if (whence == 1) {
        sct->pos = sct->pos + offset;
    }
    {
        s32 p = sct->totalSct;
        s32 q = sct->pos;
        if (q < p) p = q;
        p = (p > 0) ? p : 0;
        sct->pos = p;
    }
    SVM_Unlock();
    return sct->pos;
}

int mfCiTell(MfCiFile* sct) {
    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0xE5;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, 0);
        return 0;
    }
    return sct->pos;
}

int mfCiReqRd(MfCiFile* sct, int size, void* buf) {
    int parsed;
    int n;
    int rdOff;
    int rdSize;

    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0xFE;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, 0);
        return 0;
    }
    if (size < 0) {
        const char* msg = lbl_eu_80518844 + 0x117;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, (u32)sct);
        return 0;
    }
    if (buf == NULL) {
        const char* msg = lbl_eu_80518844 + 0x136;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, (u32)sct);
        return 0;
    }
    if (size == 0) {
        sct->status = 1;
        return 0;
    }
    if (sct->status == 2) {
        return 0;
    }
    SVM_Lock();
    sct->trSize = 0;
    n = size;
    if (n > sct->totalSct - sct->pos) n = sct->totalSct - sct->pos;
    rdSize = n * sct->sctsize;
    sct->trSize = n;
    rdOff = sct->pos * sct->sctsize;
    if (rdSize == 0) {
        sct->status = 1;
        SVM_Unlock();
        return 0;
    }
    sct->status = 2;
    sct->rdOff = rdOff;
    sct->rdSize = rdSize;
    if (strlen(sct->name) >= 0x12) {
        CRICRW_Sprintf((char*)&lbl_eu_805EC450 + 8, 0x12C, lbl_eu_80518844, sct->name, 0x12);
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, (char*)&lbl_eu_805EC450 + 8, 0);
    }
    {
        char* p = sct->name;
        parsed = mfci_str_to_uint_ptr(p, &p, 0x10);
        if (*p != 0) p++;
        (void)strtoul(p, &p, 0x10);
    }
    n = rdSize;
    if (n > parsed + sct->rdOff) n = rdSize - sct->rdOff;
    SVM_Unlock();
    memcpy(buf, (const char*)parsed + rdOff, n);
    if (rdSize - n > 0) {
        memset((u8*)buf + n, 0, rdSize - n);
    }
    SVM_Lock();
    sct->status = 1;
    sct->pos = sct->pos + sct->trSize;
    sct->numTr = sct->trSize * sct->sctsize;
    SVM_Unlock();
    return sct->trSize;
}

void mfCiStopTr(MfCiFile* sct) {
    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0xB4;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, 0);
        return;
    }
    SVM_Lock();
    sct->status = 0;
    SVM_Unlock();
}

int mfCiGetStat(MfCiFile* sct) {
    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0xB4;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, 0);
        return 0;
    }
    return (s8)sct->status;
}

int mfCiGetSctLen(MfCiFile* sct) {
    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0x158;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, 0);
        return 0;
    }
    return sct->sctsize;
}

typedef struct MfCiSectInfo {
    u32 unk00;
    s32 sctsize;
    s32 len;
    s32 sctcnt;
    s32 sctcnt2;
    s32 unk14;
    s32 unk18;
} MfCiSectInfo;

void mfCiSetSctLen(MfCiSectInfo* sct, int sctsize)
{
    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0x170;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, 0);
        return;
    }
    s32 len = sct->len;
    s32 old = sct->sctsize;
    sct->sctsize = sctsize;
    sct->sctcnt = (len + sctsize - 1) / sctsize;
    sct->sctcnt2 = sct->sctcnt2 * old / sctsize;
    sct->unk14 = sct->unk18 * sctsize;
}

int mfCiGetNumTr(MfCiFile* sct) {
    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0xB4;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, 0);
        return 0;
    }
    return sct->numTr;
}

int mfCiOptFn1(MfCiFile* sct, int op) {
    int parsed;
    int result;

    if (sct == NULL) {
        return 0;
    }
    switch (op) {
    case 0xC8:
        return 0;
    case 0xC9:
        if (sct == NULL) {
            mfci_err(0, lbl_eu_80518844 + 0xB4);
            return 0;
        }
        return sct->numTr;
    case 0xCA:
        return 0;
    case 0xCB:
        if (strlen(sct->name) >= 0x12) {
            mfci_print_err(sct->name);
        }
        {
            char* p = sct->name;
            parsed = mfci_str_to_uint_ptr(p, &p, 0x10);
            if (*p != 0) p++;
            result = (int)strtoul(p, &p, 0x10);
        }
        return result;
    case 0xCC:
        return 0;
    case 0xCD:
        if (strlen(sct->name) >= 0x12) {
            mfci_print_err(sct->name);
        }
        {
            char* p = sct->name;
            parsed = mfci_str_to_uint_ptr(p, &p, 0x10);
            if (*p != 0) p++;
            result = (int)strtoul(p, &p, 0x10);
        }
        return result;
    case 0x12B:
        return 0;
    case 0x12C:
    case 0x12E:
        return *(s32*)((u8*)sct + 8);
    case 0x12D:
        return 0;
    case 0x190:
        return 0;
    case 0x258:
        return 1;
    default:
        return -1;
    }
}
