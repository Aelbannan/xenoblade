#include <harness_catalog.h>
#include <string.h>

extern const char lbl_eu_80518810[];

__attribute__((aligned(8))) const void * const lbl_eu_80518840[1] = {
    lbl_eu_80518810
};

extern void mfCiExecServer();
extern void mfCiEntryErrFunc();
extern int mfCiGetFileSize();
extern void *mfCiOpen();
extern void mfCiClose();
extern int mfCiSeek();
extern int mfCiTell();
extern int mfCiReqRd();
extern void mfCiStopTr();
extern int mfCiGetStat();
extern int mfCiGetSctLen();
extern void mfCiSetSctLen();
extern int mfCiGetNumTr();
extern int mfCiOptFn1();

__attribute__((aligned(8))) void *lbl_eu_80565B98[26] = {
    mfCiExecServer,
    mfCiEntryErrFunc,
    mfCiGetFileSize,
    0,
    mfCiOpen,
    mfCiClose,
    mfCiSeek,
    mfCiTell,
    mfCiReqRd,
    0,
    mfCiStopTr,
    mfCiGetStat,
    mfCiGetSctLen,
    mfCiSetSctLen,
    mfCiGetNumTr,
    0,0,0,0,0,0,0,0,0,
    mfCiOptFn1,
    0
};

void (*mfci_errfn)(u32, const char*, int); // name-guess: adjacent CriWare gcci_errfn; sole mfci.c use is the error callback
u32 mfci_errarg; // name-guess: adjacent CriWare gcci_errarg; passed as errfn arg
unsigned char mfci_work[0x9F0]; // name-guess: adjacent CriWare gcci_work; remainder of the 0x9F8 error/work BSS block

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
        value = value * base;
        value = value + d;
        str++;
    }
    *endptr = (char*)str;
    return value;
}

void mfCiExecServer() {}

void mfCiEntryErrFunc(u32 a, u32 b) {
    mfci_errfn = (void (*)(u32, const char*, int))a;
    mfci_errarg = b;
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
    MfCiGlobals* g = (MfCiGlobals*)&mfci_errfn;
    int size = 0;
    const char* p;
    if (strlen(name) >= 0x12) {
        CRICRW_Sprintf(g->errbuf, 0x12C, "E01100308:length of '%s' is over %d bytes.(mfci_get_adr_size)", name, 0x12);
        if (g->errfn != NULL)
            g->errfn(g->errarg, g->errbuf, 0);
    }
    p = name;
    mfci_str_to_uint_ptr(p, (char**)&p, 0x10);
    if (*p != 0) p++;
    size = (int)strtoul(p, (char**)&p, 0x10);
    return size;
}

void* mfCiOpen(const char* path, int mode) {
    MfCiFile* sct;
    MfCiGlobals* g = (MfCiGlobals*)&mfci_errfn;
    int i;

    if (path == NULL) {
        const char* msg = "E01100301:fname is null.(mfCiOpen)";
        if (g->errfn != NULL)
            g->errfn(g->errarg, msg, 0);
        return NULL;
    }
    if (mode != 0) {
        const char* msg = "E01100302:rw is illigal.(mfCiOpen)";
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
        const char* msg = "E01100303:not enough handle resource.(mfCiOpen)";
        if (g->errfn != NULL)
            g->errfn(g->errarg, msg, 0);
        return NULL;
    }
    CRICRW_Strcpy(sct->name, 0x12, path);
    sct->sctsize = 0x800;
    if (strlen(sct->name) >= 0x12) {
        CRICRW_Sprintf(g->errbuf, 0x12C, "E01100308:length of '%s' is over %d bytes.(mfci_get_adr_size)", sct->name, 0x12);
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
        const char* msg = "E0092912:handl is null.";
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, msg, 0);
        return;
    }
    SVM_Lock();
    sct->status = 0;
    SVM_Unlock();
    if (sct->flag == 1) {
        sct->flag = (s8)0;
        memset(sct, 0, 0x38);
    }
}

int mfCiSeek(MfCiFile* sct, int offset, int whence) {
    if (sct == NULL) {
        const char* msg = "E01100305:handl is null.";
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, msg, 0);
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
        const char* msg = "E01100306:handl is null.";
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, msg, 0);
        return 0;
    }
    return sct->pos;
}

/* Inlined parse tail: advance past separator, then read the hex size part.
   The outSize pointer guard reproduces the retail addic./beq record-test. */
static void mfciParseSizeTail(char** pp, unsigned long* outSize) {
    if (**pp != '\0') (*pp)++;
    if (outSize != NULL)
        *outSize = strtoul(*pp, pp, 0x10);
}

int mfCiReqRd(MfCiFile* sct, int size, void* buf) {
    MfCiGlobals* g = (MfCiGlobals*)&mfci_errfn;
    char* p;
    int parsed;
    int n;
    int rdSize;
    int rdOff;
    int size2;
    int avail;
    int n2;

    if (sct == NULL) {
        const char* msg = "E01100307:handl is null.";
        if (g->errfn != NULL)
            g->errfn(g->errarg, msg, 0);
        return 0;
    }
    if (size < 0) {
        const char* msg = "E01100308:nsct < 0.(mfCiReqRd)";
        if (g->errfn != NULL)
            g->errfn(g->errarg, msg, (u32)sct);
        return 0;
    }
    if (buf == NULL) {
        const char* msg = "E01100309:buf is null.(mfCiReqRd)";
        if (g->errfn != NULL)
            g->errfn(g->errarg, msg, (u32)sct);
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
    /* clamp read to remaining sectors; retail keeps numTr=0 first */
    sct->numTr = 0;
    n = sct->totalSct - sct->pos;
    if (size < n) n = size;
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
        CRICRW_Sprintf(g->errbuf, 0x12C, "E01100308:length of '%s' is over %d bytes.(mfci_get_adr_size)", sct->name, 0x12);
        if (g->errfn != NULL)
            g->errfn(g->errarg, g->errbuf, 0);
    }
    p = sct->name;
    parsed = mfci_str_to_uint_ptr(p, (char**)&p, 0x10);
    mfciParseSizeTail(&p, (unsigned long*)&size2);
    n2 = sct->rdSize;
    avail = size2 - sct->rdOff;
    if (n2 > avail) n2 = avail;
    SVM_Unlock();
    memcpy(buf, (u8*)parsed + sct->rdOff, n2);
    if (sct->rdSize - n2 > 0)
        memset((u8*)buf + n2, 0, sct->rdSize - n2);
    SVM_Lock();
    sct->status = 1;
    sct->pos = sct->pos + sct->trSize;
    sct->numTr = sct->trSize * sct->sctsize;
    SVM_Unlock();
    return sct->trSize;
}

void mfCiStopTr(MfCiFile* sct) {
    if (sct == NULL) {
        const char* msg = "E0092912:handl is null.";
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, msg, 0);
        return;
    }
    SVM_Lock();
    sct->status = 0;
    SVM_Unlock();
}

int mfCiGetStat(MfCiFile* sct) {
    if (sct == NULL) {
        const char* msg = "E0092912:handl is null.";
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, msg, 0);
        return 0;
    }
    return (s8)sct->status;
}

int mfCiGetSctLen(MfCiFile* sct) {
    if (sct == NULL) {
        const char* msg = "E0040301:handl is null.";
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, msg, 0);
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
        const char* msg = "E0040302:handl is null.";
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, msg, 0);
        return;
    }
    s32 len = sct->len;
    s32 old = sct->sctsize;
    sct->sctsize = sctsize;
    sct->sctcnt = (len + sctsize - 1) / sctsize;
    sct->sctcnt2 = sct->sctcnt2 * old / sctsize;
    sct->unk14 = sct->unk18 * sctsize;}

int mfCiGetNumTr(MfCiFile* sct) {
    if (sct == NULL) {
        const char* msg = "E0092912:handl is null.";
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, msg, 0);
        return 0;
    }
    return sct->numTr;
}

int mfCiOptFn1(MfCiFile* sct, int op) {
    MfCiGlobals* g = (MfCiGlobals*)&mfci_errfn;
    const char* p;
    int result;

    if (sct == NULL) {
        return 0;
    }
    switch (op) {
    case 0xC8:
        return 0;
    case 0xC9:
        if (sct == NULL) {
            const char* msg = "E0092912:handl is null.";
            if (g->errfn != NULL)
                g->errfn(g->errarg, msg, 0);
            return 0;
        }
        return sct->numTr;
    case 0xCA:
        return 0;
    case 0xCB:
        if (strlen(sct->name) >= 0x12) {
            CRICRW_Sprintf(g->errbuf, 0x12C, "E01100308:length of '%s' is over %d bytes.(mfci_get_adr_size)", sct->name, 0x12);
            if (g->errfn != NULL)
                g->errfn(g->errarg, g->errbuf, 0);
        }
        p = sct->name;
        mfci_str_to_uint_ptr(p, (char**)&p, 0x10);
        if (*p != 0) p++;
        result = (int)strtoul(p, (char**)&p, 0x10);
        return result;
    case 0xCC:
        return 0;
    case 0xCD:
        if (strlen(sct->name) >= 0x12) {
            CRICRW_Sprintf(g->errbuf, 0x12C, "E01100308:length of '%s' is over %d bytes.(mfci_get_adr_size)", sct->name, 0x12);
            if (g->errfn != NULL)
                g->errfn(g->errarg, g->errbuf, 0);
        }
        p = sct->name;
        mfci_str_to_uint_ptr(p, (char**)&p, 0x10);
        if (*p != 0) p++;
        result = (int)strtoul(p, (char**)&p, 0x10);
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

// The three trailing retail error strings name a link-dead mfCiOpenEntry path:
// no symbol exists for it in config/*/symbols.txt and no retained TU calls it.
int mfCiOpenEntry(int entry, int rw, void** out) {
    if ((unsigned int)entry > 0x20) {
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, "E1041001:invalid entry number.(mfCiOpenEntry)", 0);
        return 0;
    }
    if (rw != 0) {
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, "E1041002:rw is illigal.(mfCiOpenEntry)", 0);
        return 0;
    }
    if (out == NULL) {
        if (mfci_errfn != NULL)
            mfci_errfn(mfci_errarg, "E1041002:not enough handle resource.(mfCiOpenEntry)", 0);
        return 0;
    }
    return 1;
}
