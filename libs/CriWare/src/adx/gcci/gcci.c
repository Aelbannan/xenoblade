// CriWare ADX GCI (GameCube Interface) streaming layer.
// High-level C reconstruction of libs/CriWare/src/adx/gcci/gcci.c

#include <harness_catalog.h>
#include <string.h>

#include <revolution/DVD.h>
#include <revolution/OS.h>

// ── Globals (bss) ──────────────────────────────────────────────────────────

// Control block at 0x805E6B70: [0x00] self-refresh word, [0x04] error
// callback, [0x08] error callback arg, [0x0C] status block (0xC bytes).
typedef struct {
    u32 unk0;                          // +0x00
    void (*errfunc)(u32, const char *, s32);  // +0x04
    u32 errarg;                        // +0x08
    u8 status[0xC];                    // +0x0C
} GciGlobals;

// BSS single blob 0x10C0
__attribute__((aligned(8))) unsigned char gcci_bss[0x10C0];
#define lbl_eu_805E6B70 (*(GciGlobals*)(gcci_bss+0x0))
#define lbl_eu_805E6B74 (*(void (**)(u32, const char *, s32))(gcci_bss+0x4))
#define lbl_eu_805E6B78 (*(u32*)(gcci_bss+0x8))
#define lbl_eu_805E6B7C (*(GciStatus*)(gcci_bss+0xC))
#define lbl_eu_805E6B88 (*(GciHndl (*)[40])(gcci_bss+0x18))
#define lbl_eu_805E7B28 (*(u32*)(gcci_bss+0xFB8))
#define lbl_eu_805E7B30 (gcci_bss+0xFC0)

// Keep GciGlobals and GciStatus definitions for type, but remove externs
typedef struct {
    s32 status;
    u8 flag;
    u8 pad[7];
} GciStatus;
         // GCI interface table
__declspec(section ".rodata") __attribute__((aligned(8))) const unsigned char gcci_rodata[0x228] = {0x45,0x30,0x30,0x39,0x32,0x39,0x30,0x31,0x3a,0x66,0x6e,0x61,0x6d,0x65,0x20,0x69,0x73,0x20,0x6e,0x75,0x6c,0x6c,0x2e,0x28,0x67,0x63,0x43,0x69,0x47,0x65,0x74,0x46,0x69,0x6c,0x65,0x53,0x69,0x7a,0x65,0x29,0x00,0x45,0x30,0x30,0x34,0x30,0x32,0x30,0x31,0x3a,0x63,0x61,0x6e,0x27,0x74,0x20,0x6f,0x70,0x65,0x6e,0x20,0x61,0x20,0x66,0x69,0x6c,0x65,0x2e,0x28,0x67,0x63,0x43,0x69,0x47,0x65,0x74,0x46,0x69,0x6c,0x65,0x53,0x69,0x7a,0x65,0x29,0x00,0x45,0x30,0x30,0x34,0x30,0x32,0x30,0x32,0x3a,0x63,0x61,0x6e,0x27,0x74,0x20,0x63,0x6c,0x6f,0x73,0x65,0x20,0x61,0x20,0x66,0x69,0x6c,0x65,0x2e,0x28,0x67,0x63,0x43,0x69,0x47,0x65,0x74,0x46,0x69,0x6c,0x65,0x53,0x69,0x7a,0x65,0x29,0x00,0x45,0x30,0x30,0x39,0x32,0x39,0x30,0x38,0x3a,0x66,0x6e,0x61,0x6d,0x65,0x20,0x69,0x73,0x20,0x6e,0x75,0x6c,0x6c,0x2e,0x28,0x67,0x63,0x43,0x69,0x4f,0x70,0x65,0x6e,0x29,0x00,0x45,0x30,0x30,0x39,0x32,0x39,0x30,0x39,0x3a,0x72,0x77,0x20,0x69,0x73,0x20,0x69,0x6c,0x6c,0x69,0x67,0x61,0x6c,0x2e,0x28,0x67,0x63,0x43,0x69,0x4f,0x70,0x65,0x6e,0x29,0x00,0x45,0x30,0x30,0x39,0x32,0x39,0x31,0x30,0x3a,0x6e,0x6f,0x74,0x20,0x65,0x6e,0x6f,0x75,0x67,0x68,0x20,0x68,0x61,0x6e,0x64,0x6c,0x65,0x20,0x72,0x65,0x73,0x6f,0x75,0x72,0x63,0x65,0x2e,0x28,0x67,0x63,0x43,0x69,0x4f,0x70,0x65,0x6e,0x29,0x00,0x45,0x30,0x30,0x39,0x32,0x39,0x31,0x31,0x3a,0x44,0x56,0x44,0x4f,0x70,0x65,0x6e,0x20,0x66,0x61,0x69,0x6c,0x2e,0x28,0x67,0x63,0x43,0x69,0x4f,0x70,0x65,0x6e,0x29,0x00,0x45,0x30,0x30,0x39,0x32,0x39,0x31,0x32,0x3a,0x68,0x61,0x6e,0x64,0x6c,0x20,0x69,0x73,0x20,0x6e,0x75,0x6c,0x6c,0x2e,0x00,0x45,0x30,0x30,0x39,0x32,0x39,0x31,0x33,0x3a,0x6e,0x73,0x63,0x74,0x20,0x3c,0x20,0x30,0x2e,0x28,0x67,0x63,0x43,0x69,0x52,0x65,0x71,0x52,0x64,0x29,0x00,0x45,0x30,0x30,0x39,0x32,0x39,0x31,0x34,0x3a,0x62,0x75,0x66,0x20,0x69,0x73,0x20,0x6e,0x75,0x6c,0x6c,0x2e,0x28,0x67,0x63,0x43,0x69,0x52,0x65,0x71,0x52,0x64,0x29,0x00,0x45,0x30,0x30,0x39,0x32,0x39,0x31,0x37,0x3a,0x44,0x56,0x44,0x43,0x61,0x6e,0x63,0x65,0x6c,0x20,0x66,0x61,0x69,0x6c,0x65,0x64,0x2e,0x00,0x45,0x30,0x30,0x39,0x32,0x39,0x31,0x38,0x3a,0x44,0x56,0x44,0x43,0x61,0x6e,0x63,0x65,0x6c,0x20,0x74,0x69,0x6d,0x65,0x20,0x6f,0x75,0x74,0x2e,0x00,0x45,0x30,0x30,0x34,0x30,0x33,0x30,0x31,0x3a,0x68,0x61,0x6e,0x64,0x6c,0x20,0x69,0x73,0x20,0x6e,0x75,0x6c,0x6c,0x2e,0x00,0x45,0x30,0x30,0x34,0x30,0x33,0x30,0x32,0x3a,0x68,0x61,0x6e,0x64,0x6c,0x20,0x69,0x73,0x20,0x6e,0x75,0x6c,0x6c,0x2e,0x00,0x45,0x30,0x30,0x34,0x30,0x33,0x30,0x33,0x3a,0x69,0x6e,0x76,0x61,0x6c,0x69,0x64,0x61,0x74,0x65,0x20,0x73,0x69,0x7a,0x65,0x2e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0x4c,0x53,0x43,0x2f,0x57,0x49,0x49,0x20,0x56,0x65,0x72,0x2e,0x32,0x2e,0x33,0x30,0x20,0x42,0x75,0x69,0x6c,0x64,0x3a,0x4f,0x63,0x74,0x20,0x32,0x38,0x20,0x32,0x30,0x30,0x39,0x20,0x32,0x31,0x3a,0x31,0x31,0x3a,0x32,0x34,0x0a,0x00,0x00,0x00};
#define lbl_eu_805181F0 ((const char*)gcci_rodata)
#define lbl_eu_805183E8 ((const char*)(gcci_rodata+0x1F8))         // error message strings

s32 gcCiGetFileSize(const char* path) {
    char buf[0x100];
    DVDFileInfo fi;
    u32 n;
    s32 size;
    if (path == NULL) {
        const char* msg = &lbl_eu_805181F0[0];
        if (lbl_eu_805E6B74 != NULL)
            lbl_eu_805E6B74(lbl_eu_805E6B78, msg, 0);
        return 0;
    }
    strcpy(buf, (const char*)lbl_eu_805E7B30);
    strcat(buf, path);
    n = strlen(buf);
    {
        u32 i;
        for (i = 0; i < n; i++) {
            if (buf[i] == '\\') buf[i] = '/';
        }
    }
    if (!DVDOpen(buf, &fi)) {
        const char* msg = &lbl_eu_805181F0[41];
        if (lbl_eu_805E6B74 != NULL)
            lbl_eu_805E6B74(lbl_eu_805E6B78, msg, 0);
        return 0;
    }
    size = fi.size;
    if (size & 0x80000000) size = 0x7FFFFFFF;
    if (!DVDClose(&fi)) {
        const char* msg = &lbl_eu_805181F0[86];
        if (lbl_eu_805E6B74 != NULL)
            lbl_eu_805E6B74(lbl_eu_805E6B78, msg, 0);
        return 0;
    }
    return size;
}


// GCI transfer handle (0x64 bytes), 40 entries at 0x805E6B88.
typedef struct {
    s8 use;                            // +0x00
    s8 unk01;                          // +0x01
    s8 state;                          // +0x02
    s8 unk03;                          // +0x03
    u32 unk04;                         // +0x04
    u8 *buf;                           // +0x08
    s32 dvdStatus;                     // +0x0C
    s32 sctLen;                        // +0x10
    s32 fileSize;                      // +0x14
    s32 numSct;                        // +0x18
    s32 pos;                           // +0x1C
    s32 transferred;                   // +0x20
    s32 length;                        // +0x24
    DVDFileInfo fi;                    // +0x28
} GciHndl;

void gcCiExecServer(void);
void gcCiEntryErrFunc(u32 a, u32 b);
s32 gcCiGetFileSize(const char* p);
void* gcCiOpen(const char* a, s32 b, s32 c);
void gcCiClose(GciHndl *h);
s32 gcCiSeek(GciHndl *h, s32 b, s32 c);
s32 gcCiTell(GciHndl *h);
s32 gcCiReqRd(GciHndl *h, s32 length, u8 *buf);
void gcCiStopTr(GciHndl *h);
s32 gcCiGetStat(GciHndl *h);
s32 gcCiGetSctLen(GciHndl *h);
void gcCiSetSctLen(GciHndl *h, s32 b);
s32 gcCiGetNumTr(GciHndl *h);
s32 gcCiIsExistFile(const char* a);
__declspec(section ".data") __attribute__((aligned(8))) const void* lbl_eu_80565B30[26] = {
    (const void*)gcCiExecServer,
    (const void*)gcCiEntryErrFunc,
    (const void*)gcCiGetFileSize,
    0,
    (const void*)gcCiOpen,
    (const void*)gcCiClose,
    (const void*)gcCiSeek,
    (const void*)gcCiTell,
    (const void*)gcCiReqRd,
    0,
    (const void*)gcCiStopTr,
    (const void*)gcCiGetStat,
    (const void*)gcCiGetSctLen,
    (const void*)gcCiSetSctLen,
    (const void*)gcCiGetNumTr,
    0,
    (const void*)gcCiIsExistFile,
    0,0,0,0,0,0,0,0,0
};


void* gcCiOpen(const char* path, s32 mode, s32 a3) {
    GciGlobals* g = &lbl_eu_805E6B70;
    char buf[0x100];
    s32 i;
    GciHndl* h;
    if (path == NULL) {
        const char* msg = &lbl_eu_805181F0[132];
        if (g->errfunc != NULL)
            g->errfunc(g->errarg, msg, 0);
        return NULL;
    }
    if (a3 != 0) {
        const char* msg = &lbl_eu_805181F0[166];
        if (g->errfunc != NULL)
            g->errfunc(g->errarg, msg, 0);
        return NULL;
    }
    h = NULL;
    for (i = 0; i < 40; i++) {
        if (((GciHndl*)&g->status[0xC])[i].use == 0) {
            h = (GciHndl*)((u8*)g + 0x18 + i * 100);
            break;
        }
    }
    if (h == NULL) {
        const char* msg = &lbl_eu_805181F0[200];
        if (g->errfunc != NULL)
            g->errfunc(g->errarg, msg, 0);
        return NULL;
    }
    strcpy(buf, (const char*)lbl_eu_805E7B30);
    strcat(buf, path);
    {
        u32 n = strlen(buf);
        s32 j;
        for (j = 0; j < n; j++) {
            if (buf[j] == '\\') buf[j] = '/';
        }
    }
    if (!DVDOpen(buf, &h->fi)) {
        const char* msg = &lbl_eu_805181F0[247];
        if (g->errfunc != NULL)
            g->errfunc(g->errarg, msg, 0);
        memset(h, 0, 100);
        return NULL;
    }
    h->sctLen = 2048;
    {
        s32 sz = h->fi.size;
        if (sz & 0x80000000) sz = 0x7FFFFFFF;
        h->fileSize = sz;
    }
    h->numSct = (h->sctLen + h->fileSize - 1) / h->sctLen;
    h->pos = 0;
    h->buf = NULL;
    h->length = 0;
    h->transferred = 0;
    h->state = 0;
    h->use = 1;
    return h;
}


s32 DVDGetTransferredSize(DVDFileInfo *info);
void gcCiStopTr(GciHndl *h);
void gcci_rd_cbfn();

// ── Functions ──────────────────────────────────────────────────────────────

void *gcCiGetInterface(void) {
    GciGlobals *g = (GciGlobals *)&lbl_eu_805E6B70;
    volatile u32 t = *(volatile u32 *)&g->unk0;
    g->unk0 = t;
    memset(lbl_eu_805E7B30, 0, 0x100);
    g->errfunc = NULL;
    g->errarg = 0;
    memset(&g->status, 0, 0xC);
    return lbl_eu_80565B30;
}

void gcCiExecHndl(GciHndl *h) {
    if (h->state == 2) {
        s32 st = DVDGetCommandBlockStatus(&h->fi.block);
        h->dvdStatus = st;
        lbl_eu_805E6B7C.status = st;
        if (st == -1)
            goto case_fatal;
        if (st == 0)
            goto case_done;
        if (st == 10)
            goto case_err;
        goto done;
    case_fatal:
        h->state = 3;
        lbl_eu_805E6B7C.flag = 3;
        goto done;
    case_done: {
        s32 tr = h->length * h->sctLen;
        DCInvalidateRange(h->buf, tr);
        s32 np = h->pos + h->length;
        s32 nt = np * h->sctLen;
        s32 fs = h->fileSize;
        h->transferred = tr;
        h->pos = np;
        if (nt > fs) {
            s32 over = nt - fs;
            u8 *dst = h->buf + tr - over;
            memset(dst, 0, over);
            DCStoreRange(dst, over);
        }
        h->state = 1;
        lbl_eu_805E6B7C.flag = 1;
    } goto done;
    case_err: {
        s32 ts = DVDGetTransferredSize(&h->fi);
        DCInvalidateRange(h->buf, ts);
        s32 nsct = ts / h->sctLen;
        h->transferred = h->sctLen * nsct;
        h->pos += nsct;
        h->state = 0;
        lbl_eu_805E6B7C.flag = 0;
    } goto done;
    done:;
    }
}

void gcCiExecServer(void) {
    GciHndl *h = &lbl_eu_805E6B88[0];
    s32 i;
    for (i = 0; i < 40; i++, h++) {
        if (h->use == 1)
            gcCiExecHndl(h);
    }
}

void gcCiEntryErrFunc(u32 a, u32 b) {
    lbl_eu_805E6B74 = (void (*)(u32, const char *, s32))a;
    lbl_eu_805E6B78 = b;
}


void gcCiClose(GciHndl *h) {
    if (h == NULL)
        return;
    gcCiStopTr(h);
    DVDClose(&h->fi);
    h->use = 0;
    memset(h, 0, 0x64);
}

s32 gcCiSeek(GciHndl *h, s32 offset, s32 mode) {
    if (h == NULL) {
        const char *msg = &lbl_eu_805181F0[0x118];
        if (lbl_eu_805E6B74 != NULL)
            lbl_eu_805E6B74(lbl_eu_805E6B78, msg, 0);
        return 0;
    }
    if (mode == 0) {
        h->pos = offset;
    } else if (mode == 2) {
        h->pos = h->numSct + offset;
    } else if (mode == 1) {
        h->pos = h->pos + offset;
    }
    s32 p = h->numSct;
    s32 q = h->pos;
    if (q < p)
        p = q;
    p = (p > 0) ? p : 0;
    h->pos = p;
    return p;
}

s32 gcCiTell(GciHndl *h) {
    if (h == NULL) {
        const char *msg = &lbl_eu_805181F0[0x118];
        if (lbl_eu_805E6B74 != NULL)
            lbl_eu_805E6B74(lbl_eu_805E6B78, msg, 0);
        return 0;
    }
    return h->pos;
}

s32 gcCiReqRd(GciHndl *h, s32 length, u8 *buf) {
    GciGlobals *g = (GciGlobals *)&lbl_eu_805E6B70;
    GciStatus *st = (GciStatus *)&g->status;
    s32 i;
    s32 busy;
    s32 offset;
    s32 size;
    s32 rc;

    if (h == NULL) {
        const char *msg = &lbl_eu_805181F0[0x118];
        if (g->errfunc != NULL)
            g->errfunc(g->errarg, msg, 0);
        return 0;
    }
    if (length < 0) {
        const char *msg = &lbl_eu_805181F0[0x130];
        if (g->errfunc != NULL)
            g->errfunc(g->errarg, msg, (s32)(u32)h);
        return 0;
    }
    if (buf == NULL) {
        const char *msg = &lbl_eu_805181F0[0x14e];
        if (g->errfunc != NULL)
            g->errfunc(g->errarg, msg, (s32)(u32)h);
        return 0;
    }
    // A request is only accepted while the handle is idle (0) or has data (1).
    s32 st_ok = (h->state == 1 || h->state == 0);
    if (!st_ok)
        return 0;
    s32 dvd_ok = (h->dvdStatus == 0 || h->dvdStatus == 10);
    if (!dvd_ok)
        return 0;
    // Only one DVD transfer may be in flight: reject if any handle is active.
    busy = 0;
    for (i = 0; i < 40; i++) {
        if (((GciHndl *)&g->status[0xC])[i].use == 1 &&
            ((GciHndl *)&g->status[0xC])[i].state == 2) {
            busy = 1;
            break;
        }
    }
    if (busy)
        return 0;
    if (length == 0) {
        h->state = 1;
        st->flag = 1;
        return 0;
    }
    h->transferred = 0;
    h->buf = buf;
    h->length = length;
    // Service any pending transfers so only this one is queued next.
    for (i = 0; i < 40; i++) {
        if (((GciHndl *)&g->status[0xC])[i].use == 1)
            gcCiExecHndl(&((GciHndl *)&g->status[0xC])[i]);
    }
    offset = h->pos * h->sctLen;
    size = h->length * h->sctLen;
    if (offset + size > h->fileSize) {
        size = h->fileSize - offset;
        if (size < 0) {
            // Request lies past EOF: pretend the transfer is already done.
            h->state = 1;
            st->flag = 1;
            return length;
        }
    }
    size = (size + 0x1f) & ~0x1f;
    DCInvalidateRange(buf, size);
    if (lbl_eu_805E7B28 == 0)
        rc = DVDReadAsyncPrio(&h->fi, buf, size, offset, gcci_rd_cbfn, 2);
    else
        rc = DVDReadPrio(&h->fi, buf, size, offset, 2);
    if (rc == 0)
        return 0;
    h->state = 2;
    st->flag = 2;
    return h->length;
}

#define GCI_TICKS_TO_MS(ticks) \
    ((ticks) / (__mulhwu(*(volatile u32 *)0x800000F8 >> 2, 0x10624DD3) >> 6))

void gcCiStopTr(GciHndl *h) {
    GciGlobals *g = (GciGlobals *)&lbl_eu_805E6B70;
    if (h == NULL) {
        const char *msg = &lbl_eu_805181F0[0x118];
        if (g->errfunc != NULL)
            g->errfunc(g->errarg, msg, NULL);
        return;
    }
    if ((u8)h->state <= 1)
        return;
    DVDGetCommandBlockStatus(&h->fi.block);
    DVDGetDriveStatus();
    *(u32 *)&g->status[8] = 1;
    s32 rc = DVDCancel(&h->fi.block);
    *(u32 *)&g->status[8] = 0;
    if (rc < 0) {
        const char *msg = &lbl_eu_805181F0[0x16F];
        if (g->errfunc != NULL)
            g->errfunc(g->errarg, msg, (s32)(u32)h);
        return;
    }
    u32 start = GCI_TICKS_TO_MS(OSGetTick());
    while (h->dvdStatus != 0 && h->dvdStatus != 10) {
        h->dvdStatus = DVDGetCommandBlockStatus(&h->fi.block);
        *(u32 *)&g->status = h->dvdStatus;
        u32 now = GCI_TICKS_TO_MS(OSGetTick());
        s32 elapsed = (s32)(now - start) - 1;
        if (now >= start)
            elapsed = (s32)(now - start);
        if ((u32)elapsed > 2000) {
            const char *msg = &lbl_eu_805181F0[0x18A];
            if (g->errfunc != NULL)
                g->errfunc(g->errarg, msg, (s32)(u32)h);
            break;
        }
    }
    h->state = 0;
    g->status[4] = 0;
    DVDGetCommandBlockStatus(&h->fi.block);
    DVDGetDriveStatus();
}

s32 gcCiGetStat(GciHndl *h) {
    if (h == NULL) {
        const char *msg = &lbl_eu_805181F0[0x118];
        if (lbl_eu_805E6B74 != NULL)
            lbl_eu_805E6B74(lbl_eu_805E6B78, msg, 0);
        return 0;
    }
    return (s8)h->state;
}

s32 gcCiGetSctLen(GciHndl *h) {
    if (h == NULL) {
        const char *msg = &lbl_eu_805181F0[0x1A7];
        if (lbl_eu_805E6B74 != NULL)
            lbl_eu_805E6B74(lbl_eu_805E6B78, msg, 0);
        return 0;
    }
    return h->sctLen;
}

void gcCiSetSctLen(GciHndl *h, s32 sctLen) {
    if (h == NULL) {
        const char *msg = &lbl_eu_805181F0[0x1BF];
        if (lbl_eu_805E6B74 != NULL)
            lbl_eu_805E6B74(lbl_eu_805E6B78, msg, 0);
        return;
    }
    s32 oldSct = h->sctLen;
    if (oldSct % 32 != 0) {
        const char *msg = &lbl_eu_805181F0[0x1D7];
        if (lbl_eu_805E6B74 != NULL)
            lbl_eu_805E6B74(lbl_eu_805E6B78, msg, 0);
        return;
    }
    h->sctLen = sctLen;
    s32 np = h->pos * oldSct;
    h->numSct = (sctLen + h->fileSize - 1) / sctLen;
    h->pos = np / sctLen;
    h->transferred = h->length * sctLen;
}

s32 gcCiGetNumTr(GciHndl *h) {
    if (h == NULL) {
        const char *msg = &lbl_eu_805181F0[0x118];
        if (lbl_eu_805E6B74 != NULL)
            lbl_eu_805E6B74(lbl_eu_805E6B78, msg, 0);
        return 0;
    }
    return h->transferred;
}

extern s32 DVDConvertPathToEntrynum(const char *);
s32 gcCiIsExistFile(const char *path) {
    s32 entry = DVDConvertPathToEntrynum(path);
    return (entry >= 0) ? 1 : 0;
}

void gcci_rd_cbfn() {}
