// CriWare ADX GCI (GameCube/Wii DVD) device driver for cvFs.
//
// gcCiGetInterface() hands cvFs a table of device entry points (the GCI I/F);
// every open file gets a GciHndl out of the gcci_hndl[] pool and all transfers
// are driven through the DVD library.

#include <harness_catalog.h>
#include <string.h>

#include <revolution/DVD.h>
#include <revolution/OS.h>

// Not exported by the RVL_SDK headers this unit sees.
extern s32 DVDGetTransferredSize(DVDFileInfo *info);

#define GCI_MAX_HNDL 40
#define GCI_NAME_MAX 0x100
#define GCI_SCT_LEN 0x800
#define GCI_TIMEOUT_MS 2000

typedef void (*GCCI_ERRFN)(void *arg, const char *msg, s32 level);

// Shared cvFs drive status.
typedef struct {
    s32 status;   // +0x00 last DVD command block status
    s8 flag;      // +0x04
    u8 pad05[3];  // +0x05
    s32 busy;     // +0x08 cancel in progress
} GciStatus;

// One open file (0x64 bytes).
typedef struct {
    s8 use;           // +0x00
    s8 unk01;         // +0x01
    s8 state;         // +0x02
    s8 unk03;         // +0x03
    s32 unk04;        // +0x04
    u8 *buf;          // +0x08
    s32 dvdStatus;    // +0x0C
    s32 sctLen;       // +0x10
    s32 fileSize;     // +0x14
    s32 numSct;       // +0x18
    s32 pos;          // +0x1C
    s32 transferred;  // +0x20
    s32 length;       // +0x24
    DVDFileInfo fi;   // +0x28
} GciHndl;

// cvFs device I/F table (only the entries the GCI driver provides are set).
typedef struct {
    void (*ExecServer)(void);                              // +0x00
    void (*EntryErrFunc)(GCCI_ERRFN fn, void *arg);        // +0x04
    s32 (*GetFileSize)(const char *fname);                 // +0x08
    void *unk0C;                                           // +0x0C
    void *(*Open)(const char *fname, s32 size, s32 mode);  // +0x10
    void (*Close)(GciHndl *hndl);                          // +0x14
    s32 (*Seek)(GciHndl *hndl, s32 offset, s32 origin);    // +0x18
    s32 (*Tell)(GciHndl *hndl);                            // +0x1C
    s32 (*ReqRd)(GciHndl *hndl, s32 size, u8 *buf);        // +0x20
    void *unk24;                                           // +0x24
    void (*StopTr)(GciHndl *hndl);                         // +0x28
    s32 (*GetStat)(GciHndl *hndl);                         // +0x2C
    s32 (*GetSctLen)(GciHndl *hndl);                       // +0x30
    void (*SetSctLen)(GciHndl *hndl, s32 sctlen);          // +0x34
    s32 (*GetNumTr)(GciHndl *hndl);                        // +0x38
    void *unk3C;                                           // +0x3C
    s32 (*IsExistFile)(const char *fname);                 // +0x40
    void *unk44[7];                                        // +0x44 .. +0x5C
    void *unk60;                                           // +0x60
    void *unk64;                                           // +0x64
} GCCI_IF;

static u32 gcci_unk0;                      // +0x000
static GCCI_ERRFN gcci_errfn;              // +0x004
static void *gcci_errarg;                  // +0x008
static GciStatus gcci_status;              // +0x00C
static GciHndl gcci_hndl[GCI_MAX_HNDL];    // +0x018
static u32 gcci_rdMode;                    // +0xFB8
static char gcci_prefix[GCI_NAME_MAX];     // +0xFC0

// Error message pool of the GCI driver (one C string per failure).
static const char gcci_errmsg[] =
    "E0092901:fname is null.(gcCiGetFileSize)\0"
    "E0040201:can't open a file.(gcCiGetFileSize)\0"
    "E0040202:can't close a file.(gcCiGetFileSize)\0"
    "E0092908:fname is null.(gcCiOpen)\0"
    "E0092909:rw is illigal.(gcCiOpen)\0"
    "E0092910:not enough handle resource.(gcCiOpen)\0"
    "E0092911:DVDOpen fail.(gcCiOpen)\0"
    "E0092912:handl is null.\0"
    "E0092913:nsct < 0.(gcCiReqRd)\0"
    "E0092914:buf is null.(gcCiReqRd)\0"
    "E0092917:DVDCancel failed.\0"
    "E0092918:DVDCancel time out.\0"
    "E0040301:handl is null.\0"
    "E0040302:handl is null.\0"
    "E0040303:invalidate size.\0";

// Build report string of the library.
static const char gcci_version[] =
    "\nLSC/WII Ver.2.30 Build:Oct 28 2009 21:11:24\n";

#define GCCI_ERR(off)                                                        \
    do {                                                                     \
        const char *msg = &gcci_errmsg[off];                                  \
        if (gcci_errfn != NULL) gcci_errfn(gcci_errarg, msg, 0);              \
    } while (0)

#define GCCI_ERR_H(off, hndl)                                                \
    do {                                                                     \
        const char *msg = &gcci_errmsg[off];                                  \
        if (gcci_errfn != NULL)                                              \
            gcci_errfn(gcci_errarg, msg, (s32)(u32)(hndl));                   \
    } while (0)

#define GCCI_TICKS_TO_MS(ticks)                                              \
    ((ticks) / (__mulhwu(*(volatile u32 *)0x800000F8 >> 2, 0x10624DD3) >> 6))

void gcCiExecServer(void);
void gcCiEntryErrFunc(GCCI_ERRFN fn, void *arg);
s32 gcCiGetFileSize(const char *fname);
void *gcCiOpen(const char *fname, s32 size, s32 mode);
void gcCiClose(GciHndl *hndl);
s32 gcCiSeek(GciHndl *hndl, s32 offset, s32 origin);
s32 gcCiTell(GciHndl *hndl);
s32 gcCiReqRd(GciHndl *hndl, s32 size, u8 *buf);
void gcCiStopTr(GciHndl *hndl);
s32 gcCiGetStat(GciHndl *hndl);
s32 gcCiGetSctLen(GciHndl *hndl);
void gcCiSetSctLen(GciHndl *hndl, s32 sctlen);
s32 gcCiGetNumTr(GciHndl *hndl);
s32 gcCiIsExistFile(const char *fname);
void gcCiExecHndl(GciHndl *hndl);
void gcci_rd_cbfn(s32 result, DVDFileInfo *info);

static const GCCI_IF gcci_iface = {
    gcCiExecServer,
    gcCiEntryErrFunc,
    gcCiGetFileSize,
    NULL,
    gcCiOpen,
    gcCiClose,
    gcCiSeek,
    gcCiTell,
    gcCiReqRd,
    NULL,
    gcCiStopTr,
    gcCiGetStat,
    gcCiGetSctLen,
    gcCiSetSctLen,
    gcCiGetNumTr,
    NULL,
    gcCiIsExistFile,
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    NULL,
    NULL};

// Prefix the game path with the working directory and fix the separators.
static inline void gcci_makePath(char *buf, const char *fname) {
    u32 i;
    u32 len;

    strcpy(buf, gcci_prefix);
    strcat(buf, fname);
    len = strlen(buf);
    for (i = 0; i < len; i++) {
        if (buf[i] == '\\') buf[i] = '/';
    }
}

void *gcCiGetInterface(void) {
    volatile u32 t;

    memset(gcci_prefix, 0, GCI_NAME_MAX);
    t = gcci_unk0;
    gcci_unk0 = t;
    gcci_errfn = NULL;
    gcci_errarg = NULL;
    memset(&gcci_status, 0, sizeof(GciStatus));
    return (void *)&gcci_iface;
}

void gcCiExecHndl(GciHndl *hndl) {
    s32 status;
    s32 tr;
    s32 np;
    s32 nt;
    s32 ts;
    s32 nsct;
    u8 *dst;

    if (hndl->state != 2) return;

    status = DVDGetCommandBlockStatus(&hndl->fi.block);
    hndl->dvdStatus = status;
    gcci_status.status = status;

    switch (status) {
    case -1:
        hndl->state = 3;
        gcci_status.flag = 3;
        break;

    case 0:
        tr = hndl->length * hndl->sctLen;
        DCInvalidateRange(hndl->buf, tr);
        np = hndl->pos + hndl->length;
        nt = np * hndl->sctLen;
        hndl->transferred = tr;
        hndl->pos = np;
        if (nt > hndl->fileSize) {
            s32 over = nt - hndl->fileSize;
            dst = hndl->buf + tr - over;
            memset(dst, 0, over);
            DCStoreRange(dst, over);
        }
        hndl->state = 1;
        gcci_status.flag = 1;
        break;

    case 10:
        ts = DVDGetTransferredSize(&hndl->fi);
        DCInvalidateRange(hndl->buf, ts);
        nsct = ts / hndl->sctLen;
        hndl->state = 0;
        hndl->transferred = hndl->sctLen * nsct;
        hndl->pos += nsct;
        gcci_status.flag = 0;
        break;

    default:
        break;
    }
}

void gcCiExecServer(void) {
    s32 i;

    for (i = 0; i < GCI_MAX_HNDL; i++) {
        if (gcci_hndl[i].use == 1) gcCiExecHndl(&gcci_hndl[i]);
    }
}

void gcCiEntryErrFunc(GCCI_ERRFN fn, void *arg) {
    gcci_errfn = fn;
    gcci_errarg = arg;
}

s32 gcCiGetFileSize(const char *fname) {
    DVDFileInfo fi;
    char buf[GCI_NAME_MAX];
    s32 size;

    if (fname == NULL) {
        GCCI_ERR(0x0);
        return 0;
    }
    gcci_makePath(buf, fname);
    if (!DVDOpen(buf, &fi)) {
        GCCI_ERR(0x29);
        return 0;
    }
    size = fi.size;
    if (size & 0x80000000) size = 0x7fffffff;
    if (!DVDClose(&fi)) {
        GCCI_ERR(0x56);
        return 0;
    }
    return size;
}

void *gcCiOpen(const char *fname, s32 size, s32 mode) {
    char buf[GCI_NAME_MAX];
    GciHndl *hndl;
    s32 i;

    if (fname == NULL) {
        GCCI_ERR(0x84);
        return NULL;
    }
    if (mode != 0) {
        GCCI_ERR(0xa6);
        return NULL;
    }
    hndl = NULL;
    for (i = 0; i < GCI_MAX_HNDL; i++) {
        if (gcci_hndl[i].use == 0) {
            hndl = &gcci_hndl[i];
            break;
        }
    }
    if (hndl == NULL) {
        GCCI_ERR(0xc8);
        return NULL;
    }
    gcci_makePath(buf, fname);
    if (!DVDOpen(buf, &hndl->fi)) {
        GCCI_ERR(0xf7);
        memset(hndl, 0, sizeof(GciHndl));
        return NULL;
    }
    hndl->sctLen = GCI_SCT_LEN;
    size = hndl->fi.size;
    if (size & 0x80000000) size = 0x7fffffff;
    hndl->fileSize = size;
    hndl->numSct = (hndl->sctLen + hndl->fileSize - 1) / hndl->sctLen;
    hndl->pos = 0;
    hndl->buf = NULL;
    hndl->length = 0;
    hndl->transferred = 0;
    hndl->state = 0;
    hndl->use = 1;
    return hndl;
}

void gcCiClose(GciHndl *hndl) {
    if (hndl == NULL) return;
    gcCiStopTr(hndl);
    DVDClose(&hndl->fi);
    hndl->use = 0;
    memset(hndl, 0, sizeof(GciHndl));
}

s32 gcCiSeek(GciHndl *hndl, s32 offset, s32 origin) {
    s32 pos;
    s32 max;

    if (hndl == NULL) {
        GCCI_ERR(0x118);
        return 0;
    }
    if (origin == 0) {
        hndl->pos = offset;
    } else if (origin == 2) {
        hndl->pos = hndl->numSct + offset;
    } else if (origin == 1) {
        hndl->pos = hndl->pos + offset;
    }
    max = hndl->numSct;
    pos = hndl->pos;
    if (pos < max) max = pos;
    pos = (max > 0) ? max : 0;
    hndl->pos = pos;
    return pos;
}

s32 gcCiTell(GciHndl *hndl) {
    if (hndl == NULL) {
        GCCI_ERR(0x118);
        return 0;
    }
    return hndl->pos;
}

s32 gcCiReqRd(GciHndl *hndl, s32 size, u8 *buf) {
    s32 i;
    s32 busy;
    s32 offset;
    s32 length;
    s32 nsct;

    if (hndl == NULL) {
        GCCI_ERR(0x118);
        return 0;
    }
    if (size < 0) {
        GCCI_ERR_H(0x130, hndl);
        return 0;
    }
    if (buf == NULL) {
        GCCI_ERR_H(0x14e, hndl);
        return 0;
    }
    if (!(hndl->state == 1 || hndl->state == 0)) return 0;
    if (!(hndl->dvdStatus == 0 || hndl->dvdStatus == 10)) return 0;
    busy = 0;
    for (i = 0; i < GCI_MAX_HNDL; i++) {
        if (gcci_hndl[i].use == 1 && gcci_hndl[i].state == 2) {
            busy = 1;
            break;
        }
    }
    if (busy) return 0;
    if (size == 0) {
        hndl->state = 1;
        gcci_status.flag = 1;
        return 0;
    }
    hndl->transferred = 0;
    hndl->buf = buf;
    hndl->length = size;
    for (i = 0; i < GCI_MAX_HNDL; i++) {
        if (gcci_hndl[i].use == 1) gcCiExecHndl(&gcci_hndl[i]);
    }
    offset = hndl->pos * hndl->sctLen;
    length = hndl->length * hndl->sctLen;
    if (offset + length > hndl->fileSize) {
        length = hndl->fileSize - offset;
        if (length < 0) {
            hndl->state = 1;
            gcci_status.flag = 1;
            return size;
        }
    }
    nsct = (length + 0x1f) & ~0x1f;
    DCInvalidateRange(buf, nsct);
    if (gcci_rdMode == 0) {
        if (!DVDReadAsyncPrio(&hndl->fi, buf, nsct, offset, gcci_rd_cbfn, 2)) {
            return 0;
        }
    } else {
        if (!DVDReadPrio(&hndl->fi, buf, nsct, offset, 2)) return 0;
    }
    hndl->state = 2;
    gcci_status.flag = 2;
    return hndl->length;
}

void gcCiStopTr(GciHndl *hndl) {
    u32 start;
    u32 now;
    s32 elapsed;

    if (hndl == NULL) {
        GCCI_ERR(0x118);
        return;
    }
    if ((u8)hndl->state <= 1) return;
    DVDGetCommandBlockStatus(&hndl->fi.block);
    DVDGetDriveStatus();
    gcci_status.busy = 1;
    if (DVDCancel(&hndl->fi.block) < 0) {
        gcci_status.busy = 0;
        GCCI_ERR_H(0x16f, hndl);
        return;
    }
    gcci_status.busy = 0;
    start = GCCI_TICKS_TO_MS(OSGetTick());
    while (hndl->dvdStatus != 0 && hndl->dvdStatus != 10) {
        hndl->dvdStatus = DVDGetCommandBlockStatus(&hndl->fi.block);
        gcci_status.status = hndl->dvdStatus;
        now = GCCI_TICKS_TO_MS(OSGetTick());
        elapsed = (now >= start) ? (s32)(now - start) : (s32)(now - start - 1);
        if (elapsed > GCI_TIMEOUT_MS) {
            GCCI_ERR_H(0x18a, hndl);
            break;
        }
    }
    hndl->state = 0;
    gcci_status.flag = 0;
    DVDGetCommandBlockStatus(&hndl->fi.block);
    DVDGetDriveStatus();
}

s32 gcCiGetStat(GciHndl *hndl) {
    if (hndl == NULL) {
        GCCI_ERR(0x118);
        return 0;
    }
    return hndl->state;
}

s32 gcCiGetSctLen(GciHndl *hndl) {
    if (hndl == NULL) {
        GCCI_ERR(0x1a7);
        return 0;
    }
    return hndl->sctLen;
}

void gcCiSetSctLen(GciHndl *hndl, s32 sctlen) {
    s32 oldSct;

    if (hndl == NULL) {
        GCCI_ERR(0x1bf);
        return;
    }
    oldSct = hndl->sctLen;
    if (oldSct % 32 != 0) {
        GCCI_ERR(0x1d7);
        return;
    }
    hndl->sctLen = sctlen;
    hndl->numSct = (sctlen + hndl->fileSize - 1) / sctlen;
    hndl->pos = (hndl->pos * oldSct) / sctlen;
    hndl->transferred = hndl->length * sctlen;
}

s32 gcCiGetNumTr(GciHndl *hndl) {
    if (hndl == NULL) {
        GCCI_ERR(0x118);
        return 0;
    }
    return hndl->transferred;
}

s32 gcCiIsExistFile(const char *fname) {
    s32 entry = DVDConvertPathToEntrynum(fname);
    return (entry >= 0) ? 1 : 0;
}

void gcci_rd_cbfn(s32 result, DVDFileInfo *info) {}
