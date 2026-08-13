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
extern int memcmp(const void *, const void *, unsigned long);

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
        u16   hw_0C;     /* 0x0C */
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

/* SJ object vtable - the first word of every SJRBF/SJMEM object points to
 * this function table (retail data at lbl_eu_80565C30 / lbl_eu_80565C00).
 * Slot names come from the retail vtable relocations: slot 3 = Destroy,
 * 4 = GetUuid, 5 = Reset, 6 = GetChunk, 7 = UngetChunk, 8 = PutChunk,
 * 9 = get-avail (SJRBF fn_80397A74 / SJMEM GetNumData), 10 = IsGetChunk,
 * 11 = EntryErrFunc. Slots 0-2 are NULL in retail (reserved base-object
 * methods). */
struct SjObjVtbl {
    void (*reserved0)(void *self);                               /* 0x00 NULL in retail */
    void (*reserved1)(void *self);                               /* 0x04 NULL in retail */
    void (*reserved2)(void *self);                               /* 0x08 NULL in retail */
    void (*destroy)(void *self);                                 /* 0x0C SJRBF_Destroy */
    void *(*getUuid)(void *self);                                /* 0x10 SJRBF_GetUuid */
    void (*reset)(void *self);                                   /* 0x14 SJRBF_Reset */
    int (*getChunk)(void *self, int mode, int size, void *out);  /* 0x18 SJRBF_GetChunk */
    int (*ungetChunk)(void *self, int mode, void *chunk);        /* 0x1C SJRBF_UngetChunk */
    int (*putChunk)(void *self, int mode, void *chunk);          /* 0x20 SJRBF_PutChunk */
    int (*getAvail)(void *self, int mode);                       /* 0x24 SJRBF fn_80397A74 */
    int (*isGetChunk)(void *self, int mode, int size, int *out); /* 0x28 SJRBF_IsGetChunk */
    void (*entryErrFunc)(void *self, void *cb, void *arg);       /* 0x2C SJRBF_EntryErrFunc */
};

/* Sector-cache flush callback: while a read is pending, work->sectCnt
 * stashes the SJRBF used as the read cache (retail adxf_ReadNw32 calls
 * SJRBF_Create and stores the handle at work+0x08); flushing the cache
 * calls its destroy slot. */
struct AdxFsCb {
    struct SjObjVtbl *vt;   /* 0x00 */
};  /* size 0x4 */
extern s32 lbl_eu_805E0610;
extern s32 lbl_eu_805E0614;
extern s32 lbl_eu_805E0618;
extern struct AdxFsPt *lbl_eu_805E061C;
extern s32 lbl_eu_805E0620;
extern u32 lbl_eu_805E0628;
extern s32 lbl_eu_805E062C;

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

/* 32-bit field read used by the ADX partition table parser (byte 0 of the
 * field ends up in the low byte of the result, matching retail codegen). */
#define ADX_RD32(p) ((u32)(p)[0] | ((u32)(p)[1] << 8) | ((u32)(p)[2] << 16) | ((u32)(p)[3] << 24))

/* adxf_GetPtStat - report/refresh the status of the current ADX partition.
 * ptid must equal the registered partition count (lbl_eu_805E0620). For an
 * open partition (status==1) this queues a 32-sector-aligned buffered read of
 * the file table; for a loaded partition (status==3) it (re)builds the
 * in-memory file table from the read buffer - a word table when the mode flag
 * at 0x0F is 1, a halfword sector table otherwise. */
int adxf_GetPtStat(int a) {
    int status;
    struct AdxFsPt *pt;
    struct AdxFsReq *req;
    s32 idx;
    u16 seq;
    void *stm;
    u8 *buf;
    int st;
    int cnt;
    int r;
    int val;
    int sct;
    int nfiles;
    int i;
    u32 *wtot = NULL;
    u32 *wtbl = NULL;
    u16 *htot = NULL;
    u16 *htbl = NULL;

    if (a != lbl_eu_805E0620) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x196);
        return -3;
    }
    if (lbl_eu_805E061C == 0 && lbl_eu_805E0620 != -1) {
        return (int)lbl_eu_805E0624;
    }
    pt = lbl_eu_805E061C;
    if (pt == 0) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x16f);
        status = -3;
    } else {
        status = pt->status;
    }
    if (status == 4) {
        lbl_eu_805E0624 = 4;
        return 4;
    }

    pt = lbl_eu_805E061C;
    ADXCRS_Enter();
    if (pt == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
        r = 0;
    } else {
        r = ADXSTM_IsOpened(pt->fstm);
    }
    ADXCRS_Leave();
    if (r == 0) {
        return (int)lbl_eu_805E0624;
    }

    if (status == 1) {
        /* partition open: issue a buffered read of the file table */
        {
            u32 rdsz = (u32)lbl_eu_805E0628;
            void *rdst = (void *)lbl_eu_805E062C;
            void *rsrc = (void *)lbl_eu_805E061C;
            if (rdsz & 0x1F) {
                ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x1c9);
                r = -3;
            } else {
                r = adxf_ReadNw32(rsrc, rdst, (int)rdsz);
            }
        }
        if (r < 0) {
            /* read failed: enqueue a close request for the partition */
            idx = lbl_eu_805E0610 % 16;
            pt = lbl_eu_805E061C;
            req = &lbl_eu_805E04F0[idx];
            seq = lbl_eu_805E05F0[3] + 1;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = pt;
            req->p1 = -1;
            lbl_eu_805E05F0[3] = seq;
            req->p2 = -1;
            lbl_eu_805E0610 = idx + 1;
            if (pt != NULL) {
                /* wait for the stream to drain, then tear the partition down */
                while (1) {
                    int opened;
                    if (pt == NULL) {
                        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
                        opened = 0;
                    } else {
                        opened = ADXSTM_IsOpened(pt->fstm);
                    }
                    if (opened == 1) {
                        break;
                    }
                    if (ADXSTM_IsOpenReq(pt->fstm) == 0) {
                        break;
                    }
                    ADXT_ExecFsSvr();
                }
                if (pt->status == 2) {
                    adxf_Stop(pt);
                }
                if (pt->fstm != NULL) {
                    pt->flag = 0;
                    stm = pt->fstm;
                    pt->fstm = NULL;
                    ADXSTM_ReleaseFile(stm);
                    ADXSTM_Destroy(stm);
                }
                memset(pt, 0, 0x34);

                idx = lbl_eu_805E0610 % 16;
                req = &lbl_eu_805E04F0[idx];
                req->flag = 3;
                req->status = 1;
                seq = lbl_eu_805E05F0[3];
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                req->p2 = -1;
                lbl_eu_805E0610 = idx + 1;
            }
            lbl_eu_805E0624 = 4;
            return 4;
        }
        return (int)lbl_eu_805E0624;
    }

    /* partition not open: refresh the cached status */
    lbl_eu_805E0624 = status;
    if (status != 3) {
        return status;
    }

    /* status == 3: (re)build the in-memory file table */
    pt = (struct AdxFsPt *)lbl_eu_805E00F0[a];
    if (pt->hdr.b.b0F == 1) {
        wtot = (u32 *)((char *)pt + 0x118);
        wtbl = wtot + 1;
    } else {
        htot = (u16 *)((char *)pt + 0x118);
        htbl = htot + 1;
    }
    if (pt->sectCnt == 0) {
        buf = (u8 *)lbl_eu_805E0628;
        if (memcmp(buf, lbl_eu_805157E0 + 0x1fd, 3) != 0) {
            ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x201);
            /* bad header: close the partition and reset */
            idx = lbl_eu_805E0610 % 16;
            pt = lbl_eu_805E061C;
            req = &lbl_eu_805E04F0[idx];
            seq = lbl_eu_805E05F0[3] + 1;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = pt;
            req->p1 = -1;
            lbl_eu_805E0624 = 4;
            lbl_eu_805E05F0[3] = seq;
            req->p2 = -1;
            lbl_eu_805E0610 = idx + 1;
            if (pt != NULL) {
                while (1) {
                    int opened;
                    if (pt == NULL) {
                        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
                        opened = 0;
                    } else {
                        opened = ADXSTM_IsOpened(pt->fstm);
                    }
                    if (opened == 1) {
                        break;
                    }
                    if (ADXSTM_IsOpenReq(pt->fstm) == 0) {
                        break;
                    }
                    ADXT_ExecFsSvr();
                }
                if (pt->status == 2) {
                    adxf_Stop(pt);
                }
                if (pt->fstm != NULL) {
                    pt->flag = 0;
                    stm = pt->fstm;
                    pt->fstm = NULL;
                    ADXSTM_ReleaseFile(stm);
                    ADXSTM_Destroy(stm);
                }
                memset(pt, 0, 0x34);

                idx = lbl_eu_805E0610 % 16;
                req = &lbl_eu_805E04F0[idx];
                req->flag = 3;
                req->status = 1;
                seq = lbl_eu_805E05F0[3];
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                req->p2 = -1;
                lbl_eu_805E0610 = idx + 1;
            }
            lbl_eu_805E061C = 0;
            lbl_eu_805E0618 = 0;
            lbl_eu_805E062C = 0;
            return (int)lbl_eu_805E0624;
        }
        nfiles = (int)ADX_RD32(buf + 4);
        if (nfiles > 0x10000) {
            ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x238);
            /* too many files: close the partition and reset */
            idx = lbl_eu_805E0610 % 16;
            pt = lbl_eu_805E061C;
            req = &lbl_eu_805E04F0[idx];
            seq = lbl_eu_805E05F0[3] + 1;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = pt;
            req->p1 = -1;
            lbl_eu_805E0624 = 4;
            lbl_eu_805E05F0[3] = seq;
            req->p2 = -1;
            lbl_eu_805E0610 = idx + 1;
            if (pt != NULL) {
                while (1) {
                    int opened;
                    if (pt == NULL) {
                        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
                        opened = 0;
                    } else {
                        opened = ADXSTM_IsOpened(pt->fstm);
                    }
                    if (opened == 1) {
                        break;
                    }
                    if (ADXSTM_IsOpenReq(pt->fstm) == 0) {
                        break;
                    }
                    ADXT_ExecFsSvr();
                }
                if (pt->status == 2) {
                    adxf_Stop(pt);
                }
                if (pt->fstm != NULL) {
                    pt->flag = 0;
                    stm = pt->fstm;
                    pt->fstm = NULL;
                    ADXSTM_ReleaseFile(stm);
                    ADXSTM_Destroy(stm);
                }
                memset(pt, 0, 0x34);

                idx = lbl_eu_805E0610 % 16;
                req = &lbl_eu_805E04F0[idx];
                req->flag = 3;
                req->status = 1;
                seq = lbl_eu_805E05F0[3];
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                req->p2 = -1;
                lbl_eu_805E0610 = idx + 1;
            }
            lbl_eu_805E061C = 0;
            lbl_eu_805E0618 = 0;
            lbl_eu_805E062C = 0;
            return (int)lbl_eu_805E0624;
        }
        pt->hdr.hw_0C = (u16)nfiles;
        pt->sectCnt = nfiles & 0xFFFF;
        if (pt->hdr.b.b0F == 1) {
            pt->fstm = (void *)((((u32)pt->sectCnt + 1) * 4 + 0x11C) & ~3u);
        } else {
            pt->fstm = (void *)((((u32)pt->sectCnt + 1) * 2 + 0x11A) & ~3u);
        }
        if (pt->hdr.b.b0F == 1) {
            *wtot = ADX_RD32((u8 *)lbl_eu_805E0628 + 8);
        } else {
            *htot = (u16)(((int)ADX_RD32((u8 *)lbl_eu_805E0628 + 8) + 0x400) >> 11);
        }
        i = 3;
    } else {
        i = 1;
    }

    cnt = ((lbl_eu_805E062C << 11) + 2) >> 2;
    idx = lbl_eu_805E0618;
    for (; i < cnt; i += 2) {
        if (pt->hdr.b.b0F == 1) {
            /* word table: one 32-bit length per file */
            wtbl[idx] = ADX_RD32((u8 *)lbl_eu_805E0628 + i * 4);
            idx++;
            lbl_eu_805E0618 = idx;
        } else {
            /* halfword table: 16-bit sector counts */
            val = (int)ADX_RD32((u8 *)lbl_eu_805E0628 + i * 4);
            sct = (val + 0x400) >> 11;
            if (val % 2048 > 0) {
                sct++;
            }
            if ((sct & 0xFFFF0000) != 0) {
                ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x26e);
                /* sector count overflow: close the partition and reset */
                idx = lbl_eu_805E0610 % 16;
                pt = lbl_eu_805E061C;
                req = &lbl_eu_805E04F0[idx];
                seq = lbl_eu_805E05F0[3] + 1;
                req->flag = 3;
                req->status = 0;
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                lbl_eu_805E0624 = 4;
                lbl_eu_805E05F0[3] = seq;
                req->p2 = -1;
                lbl_eu_805E0610 = idx + 1;
                if (pt != NULL) {
                    while (1) {
                        int opened;
                        if (pt == NULL) {
                            ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
                            opened = 0;
                        } else {
                            opened = ADXSTM_IsOpened(pt->fstm);
                        }
                        if (opened == 1) {
                            break;
                        }
                        if (ADXSTM_IsOpenReq(pt->fstm) == 0) {
                            break;
                        }
                        ADXT_ExecFsSvr();
                    }
                    if (pt->status == 2) {
                        adxf_Stop(pt);
                    }
                    if (pt->fstm != NULL) {
                        pt->flag = 0;
                        stm = pt->fstm;
                        pt->fstm = NULL;
                        ADXSTM_ReleaseFile(stm);
                        ADXSTM_Destroy(stm);
                    }
                    memset(pt, 0, 0x34);

                    idx = lbl_eu_805E0610 % 16;
                    req = &lbl_eu_805E04F0[idx];
                    req->flag = 3;
                    req->status = 1;
                    seq = lbl_eu_805E05F0[3];
                    req->seq = (u16)seq;
                    req->work = pt;
                    req->p1 = -1;
                    req->p2 = -1;
                    lbl_eu_805E0610 = idx + 1;
                }
                lbl_eu_805E061C = 0;
                lbl_eu_805E0618 = 0;
                lbl_eu_805E062C = 0;
                return (int)lbl_eu_805E0624;
            }
            htbl[idx] = (u16)sct;
            idx++;
            lbl_eu_805E0618 = idx;
        }
        if (idx >= pt->sectCnt) {
            /* table fully copied: close the partition and stop */
            idx = lbl_eu_805E0610 % 16;
            pt = lbl_eu_805E061C;
            req = &lbl_eu_805E04F0[idx];
            seq = lbl_eu_805E05F0[3] + 1;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = pt;
            req->p1 = -1;
            lbl_eu_805E0624 = 3;
            lbl_eu_805E05F0[3] = seq;
            req->p2 = -1;
            lbl_eu_805E0610 = idx + 1;
            if (pt != NULL) {
                while (1) {
                    int opened;
                    if (pt == NULL) {
                        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
                        opened = 0;
                    } else {
                        opened = ADXSTM_IsOpened(pt->fstm);
                    }
                    if (opened == 1) {
                        break;
                    }
                    if (ADXSTM_IsOpenReq(pt->fstm) == 0) {
                        break;
                    }
                    ADXT_ExecFsSvr();
                }
                if (pt->status == 2) {
                    adxf_Stop(pt);
                }
                if (pt->fstm != NULL) {
                    pt->flag = 0;
                    stm = pt->fstm;
                    pt->fstm = NULL;
                    ADXSTM_ReleaseFile(stm);
                    ADXSTM_Destroy(stm);
                }
                memset(pt, 0, 0x34);

                idx = lbl_eu_805E0610 % 16;
                req = &lbl_eu_805E04F0[idx];
                req->flag = 3;
                req->status = 1;
                seq = lbl_eu_805E05F0[3];
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                req->p2 = -1;
                lbl_eu_805E0610 = idx + 1;
            }
            lbl_eu_805E061C = 0;
            lbl_eu_805E0618 = 0;
            lbl_eu_805E062C = 0;
            break;
        }
    }
    {
        u32 rdsz = (u32)lbl_eu_805E0628;
        void *rdst = (void *)lbl_eu_805E062C;
        void *rsrc = (void *)lbl_eu_805E061C;
        if (i >= cnt) {
            /* read the next chunk of the file table */
            if (rdsz & 0x1F) {
                ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x1c9);
                r = -3;
            } else {
                r = adxf_ReadNw32(rsrc, rdst, (int)rdsz);
            }
            if (r < 0) {
            /* read failed: close the partition and reset */
            idx = lbl_eu_805E0610 % 16;
            pt = lbl_eu_805E061C;
            req = &lbl_eu_805E04F0[idx];
            seq = lbl_eu_805E05F0[3] + 1;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = pt;
            req->p1 = -1;
            lbl_eu_805E0624 = 4;
            lbl_eu_805E05F0[3] = seq;
            req->p2 = -1;
            lbl_eu_805E0610 = idx + 1;
            if (pt != NULL) {
                while (1) {
                    int opened;
                    if (pt == NULL) {
                        ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x9d);
                        opened = 0;
                    } else {
                        opened = ADXSTM_IsOpened(pt->fstm);
                    }
                    if (opened == 1) {
                        break;
                    }
                    if (ADXSTM_IsOpenReq(pt->fstm) == 0) {
                        break;
                    }
                    ADXT_ExecFsSvr();
                }
                if (pt->status == 2) {
                    adxf_Stop(pt);
                }
                if (pt->fstm != NULL) {
                    pt->flag = 0;
                    stm = pt->fstm;
                    pt->fstm = NULL;
                    ADXSTM_ReleaseFile(stm);
                    ADXSTM_Destroy(stm);
                }
                memset(pt, 0, 0x34);

                idx = lbl_eu_805E0610 % 16;
                req = &lbl_eu_805E04F0[idx];
                req->flag = 3;
                req->status = 1;
                seq = lbl_eu_805E05F0[3];
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                req->p2 = -1;
                lbl_eu_805E0610 = idx + 1;
            }
            lbl_eu_805E061C = 0;
            lbl_eu_805E0618 = 0;
            lbl_eu_805E062C = 0;
        } else {
            if (lbl_eu_805E061C == 0) {
                ADXERR_CallErrFunc1_(lbl_eu_805157E0 + 0x16f);
                st = -3;
            } else {
                st = lbl_eu_805E061C->status;
            }
            lbl_eu_805E0624 = st;
        }
    }
    }
    return (int)lbl_eu_805E0624;
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
        ((struct AdxFsCb *)obj)->vt->destroy(obj);
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
            cb->vt->destroy(cb);
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
                cb->vt->destroy(cb);
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
            cb->vt->destroy(cb);
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
                cb->vt->destroy(cb);
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
