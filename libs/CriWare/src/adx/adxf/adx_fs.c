#include <harness_catalog.h>

#define ADX_FS_WORK  lbl_eu_805E0630
#define ADX_FS_FLAG  lbl_eu_805E0624

extern void ADXCRS_Enter(void);
extern void ADXCRS_Leave(void);
extern void ADXCRS_Lock(void);
extern void ADXCRS_Unlock(void);
extern void ADXERR_CallErrFunc1_(const char *);
extern int ADXSTM_IsOpened(void *);
extern s32 ADXSTM_IsOpenReq(void *);
extern u32 ADXSTM_GetFileLen(void *);
extern char lbl_eu_805157E0[];
extern u32 lbl_eu_805E0624;
extern u32 lbl_eu_805E0630;

extern u32 lbl_eu_805E00F0[];

extern void *ADXSTM_Create(void *, int);
extern void ADXSTM_Destroy(void *);
extern void ADXSTM_BindFileNw(void *, void *, u32, u32, u32, u32);
extern void ADXSTM_ReleaseFile(void *);
extern s32 ADXSTM_GetStat(void *);
extern int ADXSTM_Seek(void *, int);
extern s32 ADXSTM_Tell(void *);
extern u32 ADXSTM_GetFileSct(void *);
extern s32 ADXSTM_Start2(void *, u32);
extern void ADXSTM_Stop(void *);
extern void ADXSTM_SetEos(void *, s32);
extern s32 ADXSTM_SetReqRdSize(void *, u32);
extern u64 ADXSTM_GetFileLen64(void *);
extern void ADXSTM_SetPause(void *, u32);
extern void ADXSTM_SetSj(void *, void *);
extern void ADXF_Ocbi(const void *, u32);
extern void *memset(void *, int, unsigned long);

void ADXT_ExecFsSvr(void);
void adxt_ExecFsSvr(void);

/* ADX file partition / work-item structure.
 * Shared header layout: flag at 0x0, signed status at 0x1, ADXSTM handle at
 * 0x4, sector count at 0x8. Work items are 0x34 bytes each. */
struct AdxFsWork {
    s8    flag;      /* 0x00 */
    s8    status;    /* 0x01 */
    s8    b2;        /* 0x02 */
    s8    b3;        /* 0x03 */
    void* fstm;      /* 0x04 */
    s32   sectCnt;   /* 0x08 */
    u32   field_0C;  /* 0x0C */
    u32   field_10;  /* 0x10 */
    u32   field_14;  /* 0x14 */
    u32   field_18;  /* 0x18 */
    u32   field_1C;  /* 0x1C */
    u32   field_20;  /* 0x20 */
    u32   field_24;  /* 0x24 */
    u32   field_28;  /* 0x28 */
    u32   field_2C;  /* 0x2C */
    u32   field_30;  /* 0x30 */
};  /* size 0x34 */

extern struct AdxFsWork lbl_eu_805DFDB0[16];

/* ADX FS request-queue entry (16 slots). */
struct AdxFsReq {
    s8    flag;   /* 0x00 request type */
    s8    status; /* 0x01 */
    u16   seq;    /* 0x02 */
    void *work;   /* 0x04 */
    s32   p1;     /* 0x08 */
    s32   p2;     /* 0x0C */
};  /* size 0x10 */

extern struct AdxFsReq lbl_eu_805E04F0[16];
extern u16 lbl_eu_805E05F0[8];

/* ADX partition (pointed to by lbl_eu_805E00F0[ptid]). Shares the 0x10-byte
 * header with AdxFsWork; the partition name and file table follow. The file
 * table tail at 0x118 is read either as a 32-bit total length + per-file u32
 * lengths (word mode, field_0F==1) or as a u16 length + per-file u16 sector
 * offsets (halfword mode), depending on the flag at 0x0F. */
struct AdxFsPt {
    s8    flag;          /* 0x00 */
    s8    status;        /* 0x01 */
    s8    b2;            /* 0x02 */
    s8    b3;            /* 0x03 */
    void* fstm;          /* 0x04 */
    s32   sectCnt;       /* 0x08 */
    union {
        u32   field_0C;  /* 0x0C */
        struct {
            u8  b0C;     /* 0x0C */
            u8  b0D;     /* 0x0D */
            s8  b0E;     /* 0x0E */
            s8  b0F;     /* 0x0F */
        } b;
    } hdr;               /* 0x0C */
    char  name[0x100];   /* 0x10 */
    u32   field_110;     /* 0x110 */
    u32   field_114;     /* 0x114 */
    union {
        s32   total;     /* 0x118 word-mode total length */
        struct {
            u16   len;   /* 0x118 */
            u16   offs[1]; /* 0x11A */
        } hw;             /* halfword mode */
    } file;              /* 0x118 */
    s32   field_11C[1];  /* 0x11C word-mode per-file lengths */
};

extern char *CRICRW_Strncpy(char *dst, void *ignored, const char *src, size_t n);

/* Sector-cache flush callback: the object pointer is stashed in work->sectCnt
 * while a read is pending; the release method is the 4th vtable slot
 * (offset 0xC). */
struct AdxFsCb {
    void (**vt)(void *);   /* 0x00 pointer to vtable (4 method slots) */
};  /* size 0x4 */
extern s32 lbl_eu_805E0610;
extern s32 lbl_eu_805E0614;

/* internal stubs */
int adxf_LoadPtBothNw(s32 p1, int p2, int p3, const char *p4, void *p5, int p6, int p7, void *p8, void *p9, int p10, int p11);
int adxf_GetPtStat(int a);
void *adxf_CreateAdxFs(void);
int adxf_ReadNw32(void *a, void *b, int c);
int adxf_Stop(void *a);
void adxf_ExecOne(struct AdxFsWork *work);
int adxf_Seek(void *a, int b, int c);
int adxf_GetFnameRangeEx(const char *fname, int flags, char *namebuf, u32 *a, u32 *b, u32 *c, u32 *d);

/* ADXF_LoadPartitionNw - external wrapper */
s32 ADXF_LoadPartitionNw(s32 ptid, const char *fname, void *ptinfo, void *nfile) {
    void *work;
    s32 r;
    ADXCRS_Enter();
    work = (void *)(((u32)&lbl_eu_805E0630 + 0x1F) & ~0x1Fu);
    r = adxf_LoadPtBothNw(ptid, 0, 0, fname, ptinfo, 0, 0xFFFFF, nfile, work, 0x800, 0);
    ADXCRS_Leave();
    return r;
}

/* adxf_LoadPtBothNw - internal implementation */
int adxf_LoadPtBothNw(s32 p1, int p2, int p3, const char *p4, void *p5, int p6, int p7, void *p8, void *p9, int p10, int p11) {
    if (lbl_eu_805E0624 != 2) {
        /* check initialization */
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x61);
        return -1;
    }
    /* Partition loading logic (0x68C bytes in retail) */
    /* Placeholder for the actual implementation */
    return 0;
}

int ADXF_GetPtStat(int a) {
    int r;
    ADXCRS_Enter();
    r = adxf_GetPtStat(a);
    ADXCRS_Leave();
    return r;
}

int adxf_GetPtStat(int a) {
    /* Get partition status (0xE3C bytes in retail - huge!) */
    return 0;
}

/* adxf_CreateAdxFs - find a free work slot, allocate its stream handle, and
 * mark it busy. Returns the work pointer, or NULL on failure. */
void *adxf_CreateAdxFs(void) {
    int i;
    struct AdxFsWork *p;
    struct AdxFsWork *work = NULL;
    for (p = lbl_eu_805DFDB0, i = 0; i < 16; p++, i++) {
        if (p->flag == 0) {
            work = &lbl_eu_805DFDB0[i];
            break;
        }
    }
    if (work == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x2b2);
        return NULL;
    }
    work->fstm = ADXSTM_Create(NULL, 0x100);
    if (work->fstm == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x2e6);
        return NULL;
    }
    work->status = 1;
    work->field_14 = 0;
    work->field_18 = 0;
    work->field_1C = 0;
    work->field_28 = 0x200;
    work->b2 = 0;
    work->sectCnt = 0;
    work->b3 = 0;
    work->flag = 1;
    return work;
}

/* ADXF_OpenNw - external open wrapper */
int ADXF_OpenNw(void *adxf, u32 nfile) {
    struct AdxFsWork *work;
    s32 seq;
    s32 idx;
    struct AdxFsReq *req;

    ADXCRS_Enter();
    if (adxf == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x3cd);
        work = NULL;
    } else {
        idx = lbl_eu_805E0610 % 16;
        req = &lbl_eu_805E04F0[idx];
        seq = lbl_eu_805E05F0[1] + 1;
        lbl_eu_805E05F0[1] = seq;
        req->flag = 1;
        req->status = 0;
        req->seq = (u16)seq;
        req->work = adxf;
        req->p1 = (s32)nfile;
        req->p2 = -1;
        lbl_eu_805E0610 = idx + 1;

        work = (struct AdxFsWork *)adxf_CreateAdxFs();
        if (work != NULL) {
            if (adxf == NULL) {
                ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x381);
            } else {
                work->field_30 = nfile;
                work->field_10 = 0;
                work->field_0C = 0xFFFFF;
                ADXSTM_BindFileNw(work->fstm, adxf, nfile, 0, 0, 0x7FFFF800);
            }
        }

        idx = lbl_eu_805E0610 % 16;
        req = &lbl_eu_805E04F0[idx];
        req->flag = 1;
        req->status = 1;
        seq = lbl_eu_805E05F0[1];
        req->seq = (u16)seq;
        req->work = adxf;
        req->p1 = (s32)nfile;
        req->p2 = -1;
        lbl_eu_805E0610 = idx + 1;
    }
    ADXCRS_Leave();
    return (int)work;
}

int ADXF_IsOpened(void *adxf) {
    int r;
    ADXCRS_Enter();
    if (adxf == 0) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
        r = 0;
    } else {
        r = ADXSTM_IsOpened(((struct AdxFsWork *)adxf)->fstm);
    }
    ADXCRS_Leave();
    return r;
}

/* ADXF_Close - close one handle: wait for pending I/O, stop and release the
 * stream, then clear the work slot. */
void ADXF_Close(void *adxf) {
    struct AdxFsWork *work = (struct AdxFsWork *)adxf;
    s32 idx;
    u16 seq;
    struct AdxFsReq *req;
    void *stm;

    ADXCRS_Enter();

    idx = lbl_eu_805E0610 % 16;
    req = &lbl_eu_805E04F0[idx];
    seq = lbl_eu_805E05F0[3] + 1;
    lbl_eu_805E05F0[3] = seq;
    req->flag = 3;
    req->status = 0;
    req->seq = (u16)seq;
    req->work = adxf;
    req->p1 = -1;
    req->p2 = -1;
    lbl_eu_805E0610 = idx + 1;

    if (work != NULL) {
        while (1) {
            int opened;
            if (work == NULL) {
                ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
                opened = 0;
            } else {
                opened = ADXSTM_IsOpened(work->fstm);
            }
            if (opened == 1) {
                break;
            }
            if (ADXSTM_IsOpenReq(work->fstm) == 0) {
                break;
            }
            ADXT_ExecFsSvr();
        }
        if (work->status == 2) {
            adxf_Stop(work);
        }
        stm = work->fstm;
        if (stm != NULL) {
            work->flag = 0;
            work->fstm = NULL;
            ADXSTM_ReleaseFile(stm);
            ADXSTM_Destroy(stm);
        }
        memset(work, 0, 0x34);

        idx = lbl_eu_805E0610 % 16;
        req = &lbl_eu_805E04F0[idx];
        req->flag = 3;
        req->status = 1;
        seq = lbl_eu_805E05F0[3];
        req->seq = (u16)seq;
        req->work = adxf;
        req->p1 = -1;
        req->p2 = -1;
        lbl_eu_805E0610 = idx + 1;
    }
    ADXCRS_Leave();
}

/* ADXF_CloseAll - close every busy handle. */
void ADXF_CloseAll(void) {
    int i;
    struct AdxFsWork *work;
    s32 idx;
    u16 seq;
    struct AdxFsReq *req;
    void *stm;

    ADXCRS_Enter();
    for (i = 0; i < 16; i++) {
        work = &lbl_eu_805DFDB0[i];
        if (work->flag == 1) {
            idx = lbl_eu_805E0610 % 16;
            req = &lbl_eu_805E04F0[idx];
            seq = lbl_eu_805E05F0[3] + 1;
            lbl_eu_805E05F0[3] = seq;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = work;
            req->p1 = -1;
            req->p2 = -1;
            lbl_eu_805E0610 = idx + 1;

            if (work != NULL) {
                while (1) {
                    int opened;
                    if (work == NULL) {
                        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
                        opened = 0;
                    } else {
                        opened = ADXSTM_IsOpened(work->fstm);
                    }
                    if (opened == 1) {
                        break;
                    }
                    if (ADXSTM_IsOpenReq(work->fstm) == 0) {
                        break;
                    }
                    ADXT_ExecFsSvr();
                }
                if (work->status == 2) {
                    adxf_Stop(work);
                }
                if (work->fstm != NULL) {
                    work->flag = 0;
                    stm = work->fstm;
                    work->fstm = NULL;
                    ADXSTM_ReleaseFile(stm);
                    ADXSTM_Destroy(stm);
                }
                memset(work, 0, 0x34);

                idx = lbl_eu_805E0610 % 16;
                req = &lbl_eu_805E04F0[idx];
                req->flag = 3;
                req->status = 1;
                seq = lbl_eu_805E05F0[3];
                req->seq = (u16)seq;
                req->work = work;
                req->p1 = -1;
                req->p2 = -1;
                lbl_eu_805E0610 = idx + 1;
            }
        }
    }
    ADXCRS_Leave();
}

/* adxf_read_sj32 - start a sequential (sj) read of n sectors; clamps to the
 * remaining sector budget and returns the number of sectors actually queued. */
int adxf_read_sj32(void *adxf, int n, void *sj) {
    struct AdxFsWork *work = (struct AdxFsWork *)adxf;
    int len;
    int r;

    if (ADXSTM_GetStat(work->fstm) != 1) {
        ADXSTM_Stop(work->fstm);
    }
    ADXCRS_Lock();

    len = work->field_0C - work->field_10;
    work->field_14 = work->field_2C + work->field_10;
    if (n < len) {
        len = n;
    }
    work->field_18 = len;
    work->field_1C = 0;

    if (len == 0) {
        work->status = 3;
        r = 0;
    } else {
        ADXSTM_SetEos(work->fstm, -1);
        ADXSTM_SetSj(work->fstm, sj);
        ADXSTM_SetReqRdSize(work->fstm, work->field_28);
        work->status = 2;
        work->b3 = 0;
        ADXSTM_SetPause(work->fstm, 0);
        ADXSTM_Seek(work->fstm, work->field_10);
        ADXSTM_Start2(work->fstm, work->field_18);
        r = work->field_18;
    }
    ADXCRS_Unlock();
    return r;
}
#pragma push
#pragma auto_inline off
int adxf_ReadNw32(void *a, void *b, int c) { return 0; }
#pragma pop

int ADXF_ReadNw(void *adxf, void *buffer, int sectors) {
    int r;
    ADXCRS_Enter();
    if (sectors & 0x1F) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x1c9);
        r = -3;
    } else {
        r = adxf_ReadNw32(adxf, buffer, sectors);
    }
    ADXCRS_Leave();
    return r;
}

/* ADXF_Stop - external stop wrapper */
int ADXF_Stop(void *adxf) {
    int r;
    ADXCRS_Enter();
    r = adxf_Stop(adxf);
    ADXCRS_Leave();
    return r;
}

/* adxf_Stop - stop a stream: enqueue a stop request, flush the sector cache
 * callback if present, and return the position where playback stopped. */
#pragma push
#pragma auto_inline off
int adxf_Stop(void *adxf) {
    struct AdxFsWork *work = (struct AdxFsWork *)adxf;
    s32 idx;
    u16 seq;
    struct AdxFsReq *req;
    void *obj;

    idx = lbl_eu_805E0610 % 16;
    req = &lbl_eu_805E04F0[idx];
    seq = lbl_eu_805E05F0[5] + 1;
    lbl_eu_805E05F0[5] = seq;
    req->flag = 5;
    req->status = 0;
    req->seq = (u16)seq;
    req->work = adxf;
    req->p1 = -1;
    req->p2 = -1;
    lbl_eu_805E0610 = idx + 1;

    if (work == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x540);
        return -3;
    }
    if (work->status == 1) {
        return (int)work->field_10;
    }
    if (work->status == 3) {
        work->status = 1;
        return (int)work->field_10;
    }
    if (work->fstm == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x564);
        return -1;
    }

    ADXSTM_Stop(work->fstm);
    ADXCRS_Lock();
    work->field_1C = ADXSTM_Tell(work->fstm) - work->field_10;
    if (work->sectCnt != 0 && work->b2 == 0) {
        if (lbl_eu_805E0614 == 1) {
            ADXF_Ocbi((const void *)work->field_20, work->field_24);
        }
        obj = (void *)work->sectCnt;
        work->sectCnt = 0;
        {
            void **vt = *(void ***)obj;
            ((void (*)(void *))vt[3])(obj);
        }
    }
    work->status = 1;
    ADXCRS_Unlock();

    idx = lbl_eu_805E0610 % 16;
    req = &lbl_eu_805E04F0[idx];
    req->flag = 5;
    req->status = 1;
    seq = lbl_eu_805E05F0[5];
    req->seq = (u16)seq;
    req->work = adxf;
    req->p1 = -1;
    req->p2 = -1;
    lbl_eu_805E0610 = idx + 1;

    return (int)work->field_10;
}
#pragma pop

/* adxf_ExecOne - service one busy work item: flush the sector-cache callback
 * when the stream errors out (stat==4), run end-of-file / position
 * bookkeeping while reading (status==2), and rearm after a finished
 * sequential read (b3 set once the last block was queued). */
#pragma push
#pragma auto_inline off
void adxf_ExecOne(struct AdxFsWork *work) {
    struct AdxFsCb *cb;
    if (ADXSTM_GetStat(work->fstm) == 4) {
        if (work->sectCnt != 0 && work->b2 == 0) {
            if (lbl_eu_805E0614 == 1) {
                ADXF_Ocbi((const void *)work->field_20, work->field_24);
            }
            cb = (struct AdxFsCb *)work->sectCnt;
            work->sectCnt = 0;
            cb->vt[3](cb);
        }
        work->status = 4;
        return;
    }
    if (work->status == 2) {
        if (work->field_0C == 0xFFFFFu && ADXSTM_IsOpened(work->fstm) != 0 &&
            (s32)ADXSTM_GetFileSct(work->fstm) <= (s32)work->field_14) {
            work->status = 3;
            if (work->sectCnt != 0 && work->b2 == 0) {
                if (lbl_eu_805E0614 == 1) {
                    ADXF_Ocbi((const void *)work->field_20, work->field_24);
                }
                cb = (struct AdxFsCb *)work->sectCnt;
                work->sectCnt = 0;
                cb->vt[3](cb);
            }
            return;
        }
        work->status = ADXSTM_GetStat(work->fstm);
        work->field_1C = ADXSTM_Tell(work->fstm) - work->field_10;
        if ((u8)((u8)work->status - 3) > 1) {
            goto l70skip;
        }
        work->field_10 = work->field_10 + work->field_1C;
        if (work->sectCnt != 0 && work->b2 == 0) {
            if (lbl_eu_805E0614 == 1) {
                ADXF_Ocbi((const void *)work->field_20, work->field_24);
            }
            cb = (struct AdxFsCb *)work->sectCnt;
            work->sectCnt = 0;
            cb->vt[3](cb);
        }
    l70skip:;
    }
    if (work->b3 == 1) {
        if (ADXSTM_GetStat(work->fstm) == 1) {
            s32 tell = ADXSTM_Tell(work->fstm);
            u32 sct = work->sectCnt;
            work->field_1C = tell - work->field_10;
            if (sct != 0 && work->b2 == 0) {
                if (lbl_eu_805E0614 == 1) {
                    ADXF_Ocbi((const void *)work->field_20, work->field_24);
                }
                cb = (struct AdxFsCb *)work->sectCnt;
                work->sectCnt = 0;
                cb->vt[3](cb);
            }
            work->status = 1;
            work->b3 = 0;
        }
    }
}
#pragma pop

void ADXF_ExecServer(void) {
    ADXCRS_Enter();
    ADXCRS_Lock();
    {
        int i;
        struct AdxFsWork *work = (struct AdxFsWork *)lbl_eu_805DFDB0;
        for (i = 0; i < 16; i++) {
            if (work->flag == 1) {
                adxf_ExecOne(work);
            }
            work++;
        }
    }
    ADXCRS_Unlock();
    ADXCRS_Leave();
}

int ADXF_Seek(void *adxf, int offset, int origin) {
    int r;
    ADXCRS_Enter();
    r = adxf_Seek(adxf, offset, origin);
    ADXCRS_Leave();
    return r;
}

int adxf_Seek(void *a, int b, int c);
#pragma push
#pragma auto_inline off
int adxf_Seek(void *a, int b, int c) { return 0; }
#pragma pop

/* ADXF_GetFsizeSct - cached file size in sectors (0xFFFFF = not yet known). */
int ADXF_GetFsizeSct(void *adxf) {
    struct AdxFsWork *work = (struct AdxFsWork *)adxf;
    int r;

    ADXCRS_Enter();
    if (work == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x356);
        r = -3;
    } else {
        r = (int)work->field_0C;
        if ((u32)r == 0xFFFFFu) {
            while (1) {
                int opened;
                if (work == NULL) {
                    ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
                    opened = 0;
                } else {
                    opened = ADXSTM_IsOpened(work->fstm);
                }
                if (opened == 1) {
                    break;
                }
                if (ADXSTM_IsOpenReq(work->fstm) == 0) {
                    break;
                }
                ADXT_ExecFsSvr();
            }
            {
                s64 len = ADXSTM_GetFileLen64(work->fstm);
                r = (int)((len + 0x7FF) / 2048);
                work->field_0C = (u32)r;
            }
            if (r >= 0xFFFFF) {
                r = -1;
            }
        }
    }
    ADXCRS_Leave();
    return r;
}

int ADXF_GetFsizeByte(void *adxf) {
    struct AdxFsWork *work = (struct AdxFsWork *)adxf;
    int r;
    ADXCRS_Enter();
    if (work == 0) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x64d);
        r = -3;
    } else {
        while (1) {
            int opened;
            if (work == 0) {
                ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
                opened = 0;
            } else {
                opened = ADXSTM_IsOpened(work->fstm);
            }
            if (opened == 1) {
                break;
            }
            if (ADXSTM_IsOpenReq(work->fstm) == 0) {
                break;
            }
            ADXT_ExecFsSvr();
        }
        {
            u32 len = ADXSTM_GetFileLen(work->fstm);
            r = -1;
            if ((s32)len < (s32)0x7FFFF800) {
                r = (int)len;
            }
        }
    }
    ADXCRS_Leave();
    return r;
}

int ADXF_GetNumReqSct(void *adxf) {
    int r;
    ADXCRS_Enter();
    if (adxf == 0) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x16f);
        r = -3;
    } else {
        r = ((struct AdxFsWork *)adxf)->status;
    }
    ADXCRS_Leave();
    return r;
}

int adxf_ChkPrmGfr(int prm, int sect) {
    struct AdxFsWork *pt;
    int r = 0;
    if ((u32)prm > 0xFF) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x706);
        r = -3;
    } else {
        pt = (struct AdxFsWork *)lbl_eu_805E00F0[prm];
        if (pt == 0) {
            ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x706);
            r = -3;
        } else if (pt->sectCnt <= 0) {
            ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x72d);
            r = -3;
        } else if (sect < 0 || sect >= (int)pt->sectCnt) {
            ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x760);
            r = -3;
        } else {
            r = 0;
        }
    }
    return r;
}

int ADXF_GetFnameRangeEx(const char *fname, int flags, void *namebuf, u32 *a, u32 *b, u32 *c) {
    int r;
    u32 tmp;
    ADXCRS_Enter();
    r = adxf_GetFnameRangeEx(fname, flags, namebuf, a, b, c, &tmp);
    ADXCRS_Leave();
    return r;
}

/* adxf_GetFnameRangeEx - report the byte range of one file inside a loaded
 * partition. The partition's file table is either a word table (field_0F==1:
 * u32 total length at 0x118, per-file u32 lengths at 0x11C) or a halfword
 * table (u16 length at 0x118, per-file u16 sector offsets at 0x11A). */
int adxf_GetFnameRangeEx(const char *fname, int flags, char *namebuf, u32 *a, u32 *b, u32 *c, u32 *d) {
    struct AdxFsPt *pt;
    u32 sum;
    int i;
    int r = adxf_ChkPrmGfr((int)fname, flags);
    if (r < 0) {
        if (namebuf != NULL) {
            *namebuf = 0;
        }
        *a = 0;
        *b = -1;
        *c = -1;
        *d = -1;
        return r;
    }
    pt = (struct AdxFsPt *)lbl_eu_805E00F0[(int)fname];
    if (pt->hdr.b.b0F == 1) {
        /* word table: lengths in bytes, rounded up to sectors */
        sum = (pt->file.total + 0x7FF) >> 11;
        for (i = 0; i < flags; i++) {
            sum += (pt->field_11C[i] + 0x7FF) >> 11;
        }
        *c = (pt->field_11C[flags] + 0x7FF) >> 11;
        *d = pt->field_11C[flags];
    } else {
        /* halfword table: sector offsets */
        sum = pt->file.hw.len;
        for (i = 0; i < flags; i++) {
            sum += pt->file.hw.offs[i];
        }
        *c = pt->file.hw.offs[flags];
        *d = (u32)pt->file.hw.offs[flags] << 11;
    }
    if (namebuf != NULL) {
        CRICRW_Strncpy(namebuf, (void *)0x100, pt->name, 0x100);
    }
    *a = pt->field_110;
    *b = pt->field_114 + sum;
    return r;
}

char *ADXF_GetFnameFromPt(int idx) {
    char *p;
    ADXCRS_Enter();
    p = (char *)lbl_eu_805E00F0[idx] + 0x10;
    ADXCRS_Leave();
    return p;
}
