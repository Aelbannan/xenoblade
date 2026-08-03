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

extern GciGlobals lbl_eu_805E6B70;

// Standalone aliases for the error callback globals (some functions
// reference them as separate symbols, matching the retail relocations).
extern void (*lbl_eu_805E6B74)(u32, const char *, s32);
extern u32 lbl_eu_805E6B78;

// Status block at 0x805E6B7C (aliases GciGlobals.status).
typedef struct {
    s32 status;                        // +0x00
    u8 flag;                           // +0x04
    u8 pad[7];
} GciStatus;

extern GciStatus lbl_eu_805E6B7C;

extern u8 lbl_eu_805E7B30[];           // sector buffer (0x100)
extern char lbl_eu_80565B30[];         // GCI interface table
extern char lbl_eu_805181F0[];         // error message strings

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

extern GciHndl lbl_eu_805E6B88[];

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
        ((u8 *)&lbl_eu_805E6B7C)[4] = 3;
        goto done;
    case_done: {
        s32 tr = h->length * h->sctLen;
        DCInvalidateRange(h->buf, tr);
        s32 np = h->pos + h->length;
        s32 fs = h->fileSize;
        s32 nt = np * h->sctLen;
        h->transferred = tr;
        h->pos = np;
        if (nt > fs) {
            s32 over = nt - fs;
            u8 *dst = h->buf + tr - over;
            memset(dst, 0, over);
            DCStoreRange(dst, over);
        }
        h->state = 1;
        ((u8 *)&lbl_eu_805E6B7C)[4] = 1;
    } goto done;
    case_err: {
        s32 ts = DVDGetTransferredSize(&h->fi);
        DCInvalidateRange(h->buf, ts);
        s32 nsct = ts / h->sctLen;
        h->pos += nsct;
        h->transferred = nsct * h->sctLen;
        h->state = 0;
        ((u8 *)&lbl_eu_805E6B7C)[4] = 0;
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

void gcCiReqRd() {}

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
    if (DVDCancel(&h->fi.block) < 0) {
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
        u32 elapsed = now - start - 1;
        if (now >= start)
            elapsed = now - start;
        if (elapsed > 2000) {
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
