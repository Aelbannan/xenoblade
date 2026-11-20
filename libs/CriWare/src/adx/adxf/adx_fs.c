#include <harness_catalog.h>

#include "libs/CriWare/src/adx/adxf/adxf_mes.h"

#define ADX_FS_WORK  adxf_workarea
#define ADX_FS_FLAG  adxf_pt_stat

extern void ADXCRS_Enter(void);
extern void ADXCRS_Leave(void);
extern void ADXCRS_Lock(void);
extern void ADXCRS_Unlock(void);
extern void ADXERR_CallErrFunc1_(const char *);
extern int ADXSTM_IsOpened(void *);
extern s32 ADXSTM_IsOpenReq(void *);
extern u32 ADXSTM_GetFileLen(void *);

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
extern s64 ADXSTM_GetFileLen64(void *);
extern void ADXSTM_SetPause(void *, u32);
extern void ADXSTM_SetSj(void *, void *);
extern void ADXF_Ocbi(const void *, u32);
extern void *SJRBF_Create(void *pool_mem, u32 buf_size, u32 xtr_size);
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


/* ADX FS request-queue entry (16 slots). */
struct AdxFsReq {
    s8    flag;   /* 0x00 request type */
    s8    status; /* 0x01 */
    u16   seq;    /* 0x02 */
    void *work;   /* 0x04 */
    s32   p1;     /* 0x08 */
    s32   p2;     /* 0x0C */
};  /* size 0x10 */


/* ADX partition (pointed to by adxf_pttbl[ptid]). Shares the 0x10-byte
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
__attribute__((aligned(8))) const ADXF_MES adxf_mes = {
    "E9040801:partition ID is range outside.(adxf_ChkPrmPt)",
    "E9040802:'ptinfo' is NULL.(adxf_ChkPrmPt)",
    "E20040917 : It is impossible to load AFS files in parallel.",
    "E2005121504 : NULL pointer is passed.",
    "E1110701:'tmpbuf' is null.(load partition)",
    "E1110702:'tbsize' <= 0.(load partition)",
    "E0042401:multi-load partition.(load partition)",
    "E9040804:can't open file.(load partition)",
    "E9040832:'adxf' is NULL.(adxf_GetStat)",
    "E0041303:illigal parameter 'ptid'.(adxf_GetPtStat)",
    "E0120401:'buf' isn't 32byte alignment.(adxf_ReadNw)",
    "AFS",
    "E0040701:Unacceptable format(not AFS).(adxf_GetPtStat)",
    "E0040702:Unacceptable number of file.(adxf_GetPtStat)",
    "E2122501:AFS file has 128MB or more of inside file.(adxf_GetPtStat)",
    "E04041201:not enough ADXF handle (adxf_CreateAdxFs)",
    "E02111001:can't create stm handle (adxf_CreateAdxFs)",
    "E9081901:illigal parameter fname=null.(adxf_SetFileInfoEx)",
    "E9040828:'adxf' is NULL.(adxf_GetFsizeSct)",
    "E2005121501 : NULL pointer is passed.",
    "E2005121502 : NULL pointer is passed.",
    "E2005121503 : NULL pointer is passed.",
    "E9040811:'adxf' is NULL.(adxf_ReadSj32)",
    "E9040812:'nsct'is negative.(adxf_ReadSj32)",
    "E9040813:'sj'is NULL.(adxf_ReadSj32)",
    "E9040816:'adxf' is NULL.(adxf_ReadNw32)",
    "E9040817:'nsct' is negative.(adxf_ReadNw32)",
    "E2006080701:Buffer size must be less than 2GB.",
    "E9040818:'buf' is NULL.(adxf_ReadNw32)",
    "E9040821:'sj' must be NULL.(adxf_ReadNw32)",
    "E9040822:'adxf' is NULL.(adxf_Stop)",
    "E9040823:'adxf->stm' is NULL.(adxf_Stop)",
    "E2092301:'adxf' is NULL.(adxf_StopNw)",
    "E2092302:'adxf->stm' is NULL.(adxf_StopNw)",
    "E9040825:'adxf' is NULL.(adxf_Seek)",
    "E9040826:'type' is illigal.(adxf_Seek)",
    "E9040827:'adxf' is NULL.(ADXF_Tell)",
    "E2005101910:'adxf' is NULL.(adxf_GetFsizeByte)",
    "E2005101911:'adxf' is NULL.(adxf_GetFsizeByte64)",
    "E9040830:'adxf' is NULL.(adxf_GetNumReqSct)",
    "E9040831:'adxf' is NULL.(adxf_GetNumReadSct)",
    "E9040828:Partition ID is out of range.",
    "E2008110501:Loading of partition is not completed.",
    "E9040828:File ID is out of range.",
    "E0041201:state is reading(ADXF_SetReqRdSct)",
    "E2005072901: NULL pointer is passed.",
};


/* The named error strings live in the ADXF_MES object above. */
/* Retail .bss objects (defined at the end of the file). */
extern unsigned char adxf_ptwork[0x340];        /* 0x000 partition work areas (16 x AdxFsWork) */
extern u32 adxf_pttbl[256];                    /* 0x340 partition table */
extern struct AdxFsReq adxf_reqslot[16];         /* 0x740 request slots */
extern u16 adxf_reqseq[16];                     /* 0x840 request sequence state */
extern s32 adxf_reqidx;                         /* 0x860 signed request index (retail uses a signed %16) */
extern s32 adxf_obci_flag;                         /* 0x864 */
extern u32 adxf_ftbl_idx;                         /* 0x868 */
extern struct AdxFsPt* adxf_curpt;             /* 0x86C current partition */
extern u32 adxf_pt_id;                         /* 0x870 */
extern u32 adxf_pt_stat;                         /* 0x874 */
extern u32 lbl_eu_805E0628;                         /* 0x878 */
extern u32 lbl_eu_805E062C;                         /* 0x87C */
extern unsigned char adxf_workarea[0x2080];       /* 0x880 work area */

struct AdxFsCb {
    struct SjObjVtbl *vt;   /* 0x00 */
};  /* size 0x4 */

/* internal stubs */
int adxf_LoadPtBothNw(s32 p1, int p2, int p3, const char *p4, void *p5, int p6, int p7, void *p8, void *p9, int p10, int p11);
int adxf_GetPtStat(int a);
void *adxf_CreateAdxFs(void);
int adxf_ReadNw32(void *a, int b, void *c);
int adxf_Stop(void *a);
void adxf_ExecOne(struct AdxFsWork *work);
int adxf_Seek(void *a, int b, int c);
int adxf_GetFnameRangeEx(const char *fname, int flags, char *namebuf, u32 *a, u32 *b, u32 *c, u32 *d);

/* ADXF_LoadPartitionNw - external wrapper */
s32 ADXF_LoadPartitionNw(s32 ptid, const char *fname, void *ptinfo, void *nfile) {
    void *work;
    s32 r;
    ADXCRS_Enter();
    work = (void *)(((u32)&adxf_workarea + 0x1F) & ~0x1Fu);
    r = adxf_LoadPtBothNw(ptid, 0, 0, fname, ptinfo, 0, 0xFFFFF, nfile, work, 0x800, 0);
    ADXCRS_Leave();
    return r;
}

/* adxf_LoadPtBothNw - internal implementation */
int adxf_LoadPtBothNw(s32 p1, int p2, int p3, const char *p4, void *p5, int p6, int p7, void *p8, void *p9, int p10, int p11) {
    if (adxf_pt_stat != 2) {
        /* check initialization */
        ADXERR_CallErrFunc1_(adxf_mes.e20040917);
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
 * ptid must equal the registered partition count (adxf_pt_id). For an
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

    if (a != adxf_pt_id) {
        ADXERR_CallErrFunc1_(adxf_mes.e0041303);
        return -3;
    }
    if (adxf_curpt == 0 && adxf_pt_id != -1) {
        return (int)adxf_pt_stat;
    }
    pt = adxf_curpt;
    if (pt == 0) {
        ADXERR_CallErrFunc1_(adxf_mes.e9040832);
        status = -3;
    } else {
        status = pt->status;
    }
    if (status == 4) {
        adxf_pt_stat = 4;
        return 4;
    }

    pt = adxf_curpt;
    ADXCRS_Enter();
    if (pt == NULL) {
        ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
        r = 0;
    } else {
        r = ADXSTM_IsOpened(pt->fstm);
    }
    ADXCRS_Leave();
    if (r == 0) {
        return (int)adxf_pt_stat;
    }

    if (status == 1) {
        /* partition open: issue a buffered read of the file table */
        {
            u32 rdsz = (u32)lbl_eu_805E0628;
            void *rdst = (void *)lbl_eu_805E062C;
            void *rsrc = (void *)adxf_curpt;
            if (rdsz & 0x1F) {
                ADXERR_CallErrFunc1_(adxf_mes.e0120401);
                r = -3;
            } else {
                r = adxf_ReadNw32(rsrc, (int)rdsz, rdst);
            }
        }
        if (r < 0) {
            /* read failed: enqueue a close request for the partition */
            idx = adxf_reqidx % 16;
            pt = adxf_curpt;
            req = &adxf_reqslot[idx];
            seq = adxf_reqseq[3] + 1;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = pt;
            req->p1 = -1;
            adxf_reqseq[3] = seq;
            req->p2 = -1;
            adxf_reqidx = idx + 1;
            if (pt != NULL) {
                /* wait for the stream to drain, then tear the partition down */
                while (1) {
                    int opened;
                    if (pt == NULL) {
                        ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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

                idx = adxf_reqidx % 16;
                req = &adxf_reqslot[idx];
                req->flag = 3;
                req->status = 1;
                seq = adxf_reqseq[3];
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                req->p2 = -1;
                adxf_reqidx = idx + 1;
            }
            adxf_pt_stat = 4;
            return 4;
        }
        return (int)adxf_pt_stat;
    }

    /* partition not open: refresh the cached status */
    adxf_pt_stat = status;
    if (status != 3) {
        return status;
    }

    /* status == 3: (re)build the in-memory file table */
    pt = (struct AdxFsPt *)((u32*)adxf_pttbl)[a];
    if (pt->hdr.b.b0F == 1) {
        wtot = (u32 *)&pt->file;
        wtbl = wtot + 1;
    } else {
        htot = (u16 *)&pt->file;
        htbl = htot + 1;
    }
    if (pt->sectCnt == 0) {
        buf = (u8 *)lbl_eu_805E0628;
        if (memcmp(buf, adxf_mes.afs, 3) != 0) {
            ADXERR_CallErrFunc1_(adxf_mes.e0040701);
            /* bad header: close the partition and reset */
            idx = adxf_reqidx % 16;
            pt = adxf_curpt;
            req = &adxf_reqslot[idx];
            seq = adxf_reqseq[3] + 1;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = pt;
            req->p1 = -1;
            adxf_pt_stat = 4;
            adxf_reqseq[3] = seq;
            req->p2 = -1;
            adxf_reqidx = idx + 1;
            if (pt != NULL) {
                while (1) {
                    int opened;
                    if (pt == NULL) {
                        ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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

                idx = adxf_reqidx % 16;
                req = &adxf_reqslot[idx];
                req->flag = 3;
                req->status = 1;
                seq = adxf_reqseq[3];
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                req->p2 = -1;
                adxf_reqidx = idx + 1;
            }
            adxf_curpt = 0;
            adxf_ftbl_idx = 0;
            lbl_eu_805E062C = 0;
            return (int)adxf_pt_stat;
        }
        nfiles = (int)ADX_RD32(buf + 4);
        if (nfiles > 0x10000) {
            ADXERR_CallErrFunc1_(adxf_mes.e0040702);
            /* too many files: close the partition and reset */
            idx = adxf_reqidx % 16;
            pt = adxf_curpt;
            req = &adxf_reqslot[idx];
            seq = adxf_reqseq[3] + 1;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = pt;
            req->p1 = -1;
            adxf_pt_stat = 4;
            adxf_reqseq[3] = seq;
            req->p2 = -1;
            adxf_reqidx = idx + 1;
            if (pt != NULL) {
                while (1) {
                    int opened;
                    if (pt == NULL) {
                        ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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

                idx = adxf_reqidx % 16;
                req = &adxf_reqslot[idx];
                req->flag = 3;
                req->status = 1;
                seq = adxf_reqseq[3];
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                req->p2 = -1;
                adxf_reqidx = idx + 1;
            }
            adxf_curpt = 0;
            adxf_ftbl_idx = 0;
            lbl_eu_805E062C = 0;
            return (int)adxf_pt_stat;
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
    idx = adxf_ftbl_idx;
    for (; i < cnt; i += 2) {
        if (pt->hdr.b.b0F == 1) {
            /* word table: one 32-bit length per file */
            wtbl[idx] = ADX_RD32((u8 *)lbl_eu_805E0628 + i * 4);
            idx++;
            adxf_ftbl_idx = idx;
        } else {
            /* halfword table: 16-bit sector counts */
            val = (int)ADX_RD32((u8 *)lbl_eu_805E0628 + i * 4);
            sct = (val + 0x400) >> 11;
            if (val % 2048 > 0) {
                sct++;
            }
            if ((sct & 0xFFFF0000) != 0) {
                ADXERR_CallErrFunc1_(adxf_mes.e2122501);
                /* sector count overflow: close the partition and reset */
                idx = adxf_reqidx % 16;
                pt = adxf_curpt;
                req = &adxf_reqslot[idx];
                seq = adxf_reqseq[3] + 1;
                req->flag = 3;
                req->status = 0;
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                adxf_pt_stat = 4;
                adxf_reqseq[3] = seq;
                req->p2 = -1;
                adxf_reqidx = idx + 1;
                if (pt != NULL) {
                    while (1) {
                        int opened;
                        if (pt == NULL) {
                            ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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

                    idx = adxf_reqidx % 16;
                    req = &adxf_reqslot[idx];
                    req->flag = 3;
                    req->status = 1;
                    seq = adxf_reqseq[3];
                    req->seq = (u16)seq;
                    req->work = pt;
                    req->p1 = -1;
                    req->p2 = -1;
                    adxf_reqidx = idx + 1;
                }
                adxf_curpt = 0;
                adxf_ftbl_idx = 0;
                lbl_eu_805E062C = 0;
                return (int)adxf_pt_stat;
            }
            htbl[idx] = (u16)sct;
            idx++;
            adxf_ftbl_idx = idx;
        }
        if (idx >= pt->sectCnt) {
            /* table fully copied: close the partition and stop */
            idx = adxf_reqidx % 16;
            pt = adxf_curpt;
            req = &adxf_reqslot[idx];
            seq = adxf_reqseq[3] + 1;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = pt;
            req->p1 = -1;
            adxf_pt_stat = 3;
            adxf_reqseq[3] = seq;
            req->p2 = -1;
            adxf_reqidx = idx + 1;
            if (pt != NULL) {
                while (1) {
                    int opened;
                    if (pt == NULL) {
                        ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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

                idx = adxf_reqidx % 16;
                req = &adxf_reqslot[idx];
                req->flag = 3;
                req->status = 1;
                seq = adxf_reqseq[3];
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                req->p2 = -1;
                adxf_reqidx = idx + 1;
            }
            adxf_curpt = 0;
            adxf_ftbl_idx = 0;
            lbl_eu_805E062C = 0;
            break;
        }
    }
    {
        u32 rdsz = (u32)lbl_eu_805E0628;
        void *rdst = (void *)lbl_eu_805E062C;
        void *rsrc = (void *)adxf_curpt;
        if (i >= cnt) {
            /* read the next chunk of the file table */
            if (rdsz & 0x1F) {
                ADXERR_CallErrFunc1_(adxf_mes.e0120401);
                r = -3;
            } else {
                r = adxf_ReadNw32(rsrc, (int)rdsz, rdst);
            }
            if (r < 0) {
            /* read failed: close the partition and reset */
            idx = adxf_reqidx % 16;
            pt = adxf_curpt;
            req = &adxf_reqslot[idx];
            seq = adxf_reqseq[3] + 1;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = pt;
            req->p1 = -1;
            adxf_pt_stat = 4;
            adxf_reqseq[3] = seq;
            req->p2 = -1;
            adxf_reqidx = idx + 1;
            if (pt != NULL) {
                while (1) {
                    int opened;
                    if (pt == NULL) {
                        ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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

                idx = adxf_reqidx % 16;
                req = &adxf_reqslot[idx];
                req->flag = 3;
                req->status = 1;
                seq = adxf_reqseq[3];
                req->seq = (u16)seq;
                req->work = pt;
                req->p1 = -1;
                req->p2 = -1;
                adxf_reqidx = idx + 1;
            }
            adxf_curpt = 0;
            adxf_ftbl_idx = 0;
            lbl_eu_805E062C = 0;
        } else {
            if (adxf_curpt == 0) {
                ADXERR_CallErrFunc1_(adxf_mes.e9040832);
                st = -3;
            } else {
                st = adxf_curpt->status;
            }
            adxf_pt_stat = st;
        }
    }
    }
    return (int)adxf_pt_stat;
}

/* adxf_CreateAdxFs - find a free work slot, allocate its stream handle, and
 * mark it busy. Returns the work pointer, or NULL on failure. */
void *adxf_CreateAdxFs(void) {
    int i;
    struct AdxFsWork *p;
    struct AdxFsWork *work = NULL;
    for (p = (struct AdxFsWork*)adxf_ptwork, i = 0; i < 16; p++, i++) {
        if (p->flag == 0) {
            work = (struct AdxFsWork*)(adxf_ptwork + (i)*52);
            break;
        }
    }
    if (work == NULL) {
        ADXERR_CallErrFunc1_(adxf_mes.e04041201);
        return NULL;
    }
    work->fstm = ADXSTM_Create(NULL, 0x100);
    if (work->fstm == NULL) {
        ADXERR_CallErrFunc1_(adxf_mes.e02111001);
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
    u16 seq;
    s32 idx;
    struct AdxFsReq *req;
    s32 idx2;

    ADXCRS_Enter();
    if (adxf == NULL) {
        ADXERR_CallErrFunc1_(adxf_mes.e2005121503);
        work = NULL;
    } else {
        idx = adxf_reqidx % 16;
        req = &adxf_reqslot[idx];
        seq = adxf_reqseq[1] + 1;
        adxf_reqseq[1] = seq;
        req->flag = 1;
        req->status = 0;
        req->seq = (u16)seq;
        req->work = adxf;
        req->p1 = (s32)nfile;
        req->p2 = -1;
        adxf_reqidx = idx + 1;

        work = (struct AdxFsWork *)adxf_CreateAdxFs();
        if (work != NULL) {
            if (adxf == NULL) {
                ADXERR_CallErrFunc1_(adxf_mes.e2005121501);
            } else {
                work->field_30 = nfile;
                work->field_10 = 0;
                work->field_0C = 0xFFFFF;
                ADXSTM_BindFileNw(work->fstm, adxf, nfile, 0, 0, 0x7FFFF800);
            }
        }

        idx2 = adxf_reqidx % 16;
        req = &adxf_reqslot[idx2];
        req->flag = 1;
        req->status = 1;
        req->seq = adxf_reqseq[1];
        req->work = adxf;
        req->p1 = (s32)nfile;
        req->p2 = -1;
        adxf_reqidx = idx2 + 1;
    }
    ADXCRS_Leave();
    return (int)work;
}

int ADXF_IsOpened(void *adxf) {
    int r;
    ADXCRS_Enter();
    if (adxf == 0) {
        ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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
    u16 seq;
    void *stm;
    struct AdxFsWork *work = (struct AdxFsWork *)adxf;

    ADXCRS_Enter();

    struct AdxFsReq *req = &adxf_reqslot[adxf_reqidx % 16];
    seq = adxf_reqseq[3] + 1;
    adxf_reqseq[3] = seq;
    req->flag = 3;
    req->status = 0;
    req->seq = (u16)seq;
    req->work = adxf;
    req->p1 = -1;
    req->p2 = -1;
    adxf_reqidx = (adxf_reqidx % 16) + 1;

    if (work != NULL) {
        while (1) {
            int opened;
            if (work == NULL) {
                ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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

        req = &adxf_reqslot[adxf_reqidx % 16];
        req->flag = 3;
        req->status = 1;
        req->seq = adxf_reqseq[3];
        req->work = adxf;
        req->p1 = -1;
        req->p2 = -1;
        adxf_reqidx = (adxf_reqidx % 16) + 1;
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
    for (i = 0, work = (struct AdxFsWork*)adxf_ptwork; i < 16; i++, work++) {
        if (work->flag == 1) {
            req = &adxf_reqslot[adxf_reqidx % 16];
            seq = adxf_reqseq[3] + 1;
            adxf_reqseq[3] = seq;
            req->flag = 3;
            req->status = 0;
            req->seq = (u16)seq;
            req->work = work;
            req->p1 = -1;
            req->p2 = -1;
            adxf_reqidx = (adxf_reqidx % 16) + 1;

            if (work != NULL) {
                while (1) {
                    int opened;
                    if (work == NULL) {
                        ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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

                idx = adxf_reqidx % 16;
                req = &adxf_reqslot[idx];
                req->flag = 3;
                req->status = 1;
                seq = adxf_reqseq[3];
                req->seq = (u16)seq;
                req->work = work;
                req->p1 = -1;
                req->p2 = -1;
                adxf_reqidx = (adxf_reqidx % 16) + 1;
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

    s32 cur;

    work->field_14 = work->field_2C + work->field_10;
    len = work->field_0C - work->field_10;
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
/* adxf_ReadNw32 - queue a non-waiting sector read. Allocates an SJRBF ring
 * buffer over the caller's memory, binds it as the sector cache, and kicks
 * off adxf_read_sj32 under lock. Note the request is enqueued before the
 * parameter checks (matching retail). */
int adxf_ReadNw32(void *a, int b, void *c) {
    s32 idx;
    struct AdxFsReq *req;
    u16 seq;
    struct AdxFsWork *work;
    s32 size;
    void *sj;
    int r;
    s32 openreq;
    s32 opened;

    idx = adxf_reqidx % 16;
    req = &adxf_reqslot[idx];
    seq = adxf_reqseq[4] + 1;
    adxf_reqseq[4] = seq;
    work = (struct AdxFsWork *)a;
    req->flag = 4;
    req->status = 0;
    req->seq = seq;
    req->work = a;
    req->p1 = b;
    req->p2 = (s32)c;
    adxf_reqidx = idx + 1;

    if (a == NULL) {
        ADXERR_CallErrFunc1_(adxf_mes.e9040816);
        return -3;
    }
    if (b < 0) {
        ADXERR_CallErrFunc1_(adxf_mes.e9040817);
        return -3;
    }
    if (b >= 0x100000) {
        ADXERR_CallErrFunc1_(adxf_mes.e2006080701);
        return -3;
    }
    if (c == 0) {
        ADXERR_CallErrFunc1_(adxf_mes.e9040818);
        return -3;
    }
    if (work->status == 2) {
        return 0;
    }

    ADXCRS_Lock();
    openreq = ADXSTM_IsOpenReq(work->fstm);
    if (a == NULL) {
        ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
        opened = 0;
    } else {
        opened = ADXSTM_IsOpened(work->fstm);
    }
    ADXCRS_Unlock();
    if (openreq == 0 && opened == 0) {
        work->status = 4;
        return -1;
    }

    ADXCRS_Lock();
    if (work->sectCnt != 0) {
        ADXERR_CallErrFunc1_(adxf_mes.e9040821);
        ADXCRS_Unlock();
        return -1;
    }
    ADXCRS_Unlock();

    size = b << 11;
    sj = SJRBF_Create(c, size, 0);
    if (sj == NULL) {
        return -2;
    }

    ADXCRS_Lock();
    work->field_20 = (u32)c;
    work->field_24 = (u32)size;
    work->sectCnt = (u32)sj;
    if (adxf_obci_flag == 1) {
        ADXF_Ocbi(c, size);
    }
    /* retail reloads the cache handle from the work item for the call */
    r = adxf_read_sj32(work, b, (void *)work->sectCnt);
    if (r <= 0) {
        /* read failed: flush the just-created sector cache */
        ADXCRS_Lock();
        if (work->sectCnt != 0) {
            ((struct AdxFsCb *)work->sectCnt)->vt->destroy((void *)work->sectCnt);
            work->sectCnt = 0;
        }
        ADXCRS_Unlock();
    }
    work->b2 = 0;
    ADXCRS_Unlock();

    idx = adxf_reqidx % 16;
    req = &adxf_reqslot[idx];
    seq = adxf_reqseq[4];
    req->flag = 4;
    req->status = 1;
    req->seq = seq;
    req->work = a;
    req->p1 = b;
    req->p2 = (s32)c;
    adxf_reqidx = idx + 1;
    return r;
}
#pragma pop

int ADXF_ReadNw(void *adxf, void *buffer, int sectors) {
    int r;
    ADXCRS_Enter();
    if (sectors & 0x1F) {
        ADXERR_CallErrFunc1_(adxf_mes.e0120401);
        r = -3;
    } else {
        /* retail passes (file, buf, len) positionally even though this TU's
         * adxf_ReadNw32 declares its 2nd param as the length slot */
        r = adxf_ReadNw32(adxf, (int)buffer, (void *)sectors);
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
int adxf_Stop(void *adxf) {
    s32 idx;
    u16 seq;
    struct AdxFsReq *req;
    struct AdxFsReq *req2;
    void *obj;
    struct AdxFsWork *work = (struct AdxFsWork *)adxf;

    req = &adxf_reqslot[adxf_reqidx % 16];
    seq = adxf_reqseq[5] + 1;
    adxf_reqseq[5] = seq;
    req->flag = 5;
    req->status = 0;
    req->seq = (u16)seq;
    req->work = adxf;
    req->p1 = -1;
    req->p2 = -1;
    adxf_reqidx = (adxf_reqidx % 16) + 1;

    if (work == NULL) {
        ADXERR_CallErrFunc1_(adxf_mes.e9040822);
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
        ADXERR_CallErrFunc1_(adxf_mes.e9040823);
        return -1;
    }

    ADXSTM_Stop(work->fstm);
    ADXCRS_Lock();
    work->field_1C = ADXSTM_Tell(work->fstm) - work->field_10;
    if (work->sectCnt != 0 && work->b2 == 0) {
        if (adxf_obci_flag == 1) {
            ADXF_Ocbi((const void *)work->field_20, work->field_24);
        }
        obj = (void *)work->sectCnt;
        work->sectCnt = 0;
        ((struct AdxFsCb *)obj)->vt->destroy(obj);
    }
    work->status = 1;
    ADXCRS_Unlock();

    idx = adxf_reqidx % 16;
    req2 = &adxf_reqslot[idx];
    req2->flag = 5;
    req2->status = 1;
    req2->seq = adxf_reqseq[5];
    req2->work = adxf;
    req2->p1 = -1;
    req2->p2 = -1;
    adxf_reqidx = idx + 1;

    return (int)work->field_10;
}
#pragma pop

/* adxf_ExecOne - service one busy work item: flush the sector-cache callback
 * when the stream errors out (stat==4), run end-of-file / position
 * bookkeeping while reading (status==2), and rearm after a finished
 * sequential read (b3 set once the last block was queued). */
#pragma push
void adxf_ExecOne(struct AdxFsWork *work) {
    struct AdxFsCb *cb;
    s32 base;
    s32 tell;
    if (ADXSTM_GetStat(work->fstm) == 4) {
        if (work->sectCnt != 0 && work->b2 == 0) {
            if (adxf_obci_flag == 1) {
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
                if (adxf_obci_flag == 1) {
                    ADXF_Ocbi((const void *)work->field_20, work->field_24);
                }
                cb = (struct AdxFsCb *)work->sectCnt;
                work->sectCnt = 0;
                cb->vt->destroy(cb);
            }
            return;
        }
        work->status = ADXSTM_GetStat(work->fstm);
        {
            u8 sdiff;
            tell = ADXSTM_Tell(work->fstm);
            /* materialize the range-check temp before the base load: MWCC
             * allocates temps last-created-first, so this lands in r5 and
             * field_10 in r4, matching retail */
            sdiff = (u8)((u8)work->status - 3);
            base = work->field_10;
            work->field_1C = tell - base;
            if (sdiff > 1) {
                goto l70skip;
            }
            work->field_10 += work->field_1C;
                if (work->sectCnt != 0 && work->b2 == 0) {
                    if (adxf_obci_flag == 1) {
                        ADXF_Ocbi((const void *)work->field_20, work->field_24);
                    }
                    cb = (struct AdxFsCb *)work->sectCnt;
                    work->sectCnt = 0;
                    cb->vt->destroy(cb);
                }
            l70skip:;
        }
    }
    if (work->b3 == 1) {
        if (ADXSTM_GetStat(work->fstm) == 1) {
            s32 tell = ADXSTM_Tell(work->fstm);
            work->field_1C = tell - work->field_10;
            if (work->sectCnt != 0 && work->b2 == 0) {
                if (adxf_obci_flag == 1) {
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
        struct AdxFsWork *work = (struct AdxFsWork *)adxf_ptwork;
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
/* adxf_Seek - enqueue a seek request and update the cached position.
 * Resolves an unknown file length (0xFFFFF) once by querying the stream,
 * then applies the seek relative to the requested origin and clamps to
 * [0, file length in sectors]. */
int adxf_Seek(void *a, int b, int c) {
    struct AdxFsReq *req;
    s32 idx;
    u16 seq;
    s32 size;

    if (a == NULL) {
        ADXERR_CallErrFunc1_(adxf_mes.e9040825);
        return -3;
    }

    req = &adxf_reqslot[adxf_reqidx % 16];
    seq = adxf_reqseq[6] + 1;
    adxf_reqseq[6] = seq;
    req->flag = 6;
    req->status = 0;
    req->seq = (u16)seq;
    req->work = a;
    req->p1 = b;
    req->p2 = c;
    adxf_reqidx = (adxf_reqidx % 16) + 1;

    {
        size = (s32)((struct AdxFsWork *)a)->field_0C;
        if (size == 0xFFFFF) {
            /* file size unknown: drain pending I/O, then fetch it once */
            if (a == NULL) {
                ADXERR_CallErrFunc1_(adxf_mes.e9040828);
                size = -3;
            } else if (((struct AdxFsWork *)a)->field_0C == 0xFFFFF) {
            const ADXF_MES *msg = &adxf_mes;
            /* re-checked: the drain loop's opaque calls may change it */
            while (1) {
                int opened;
                if (a == NULL) {
                    ADXERR_CallErrFunc1_(msg->e2005121504);
                    opened = 0;
                } else {
                    opened = ADXSTM_IsOpened(((struct AdxFsWork *)a)->fstm);
                }
                if (opened == 1) {
                    break;
                }
                if (ADXSTM_IsOpenReq(((struct AdxFsWork *)a)->fstm) == 0) {
                    break;
                }
                ADXT_ExecFsSvr();
            }
            {
                s64 len = ADXSTM_GetFileLen64(((struct AdxFsWork *)a)->fstm);
                len += 0x7FF;
                ((struct AdxFsWork *)a)->field_0C = (u32)(len / 0x800);
            }
            size = (s32)((struct AdxFsWork *)a)->field_0C;
            if (size >= 0xFFFFF) {
                size = -1;
            }
        }
        ((struct AdxFsWork *)a)->field_0C = (u32)size;
        }
    }
    {
        struct AdxFsWork *w = (struct AdxFsWork *)a;
        if (w->status == 2) {
            adxf_Stop(a);
        }
        if (c == 0) {
            w->field_10 = (u32)b;
        } else if (c == 1) {
            w->field_10 = w->field_10 + b;
        } else if (c == 2) {
            w->field_10 = w->field_0C + b;
        } else {
            ADXERR_CallErrFunc1_(adxf_mes.e9040826);
            return -3;
        }
        if ((s32)w->field_10 < 0) {
            w->field_10 = 0;
        } else if ((s32)w->field_10 > (s32)w->field_0C) {
            w->field_10 = w->field_0C;
        }
    }

    idx = adxf_reqidx % 16;
    req = &adxf_reqslot[idx];
    req->flag = 6;
    req->status = 1;
    seq = adxf_reqseq[6];
    req->seq = (u16)seq;
    req->work = a;
    req->p1 = b;
    req->p2 = c;
    adxf_reqidx = idx + 1;
    return (int)((struct AdxFsWork *)a)->field_10;
}
#pragma pop

/* ADXF_GetFsizeSct - cached file size in sectors (0xFFFFF = not yet known). */
int ADXF_GetFsizeSct(void *adxf) {
    struct AdxFsWork *work = (struct AdxFsWork *)adxf;
    int r;

    ADXCRS_Enter();
    if (work == NULL) {
        ADXERR_CallErrFunc1_(adxf_mes.e9040828);
        r = -3;
    } else {
        r = (int)work->field_0C;
        if ((u32)r == 0xFFFFFu) {
            while (1) {
                int opened;
                if (work == NULL) {
                    ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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
                /* round up to sectors; MWCC emits an inline 64-bit div here */
                work->field_0C = (u32)((ADXSTM_GetFileLen64(work->fstm) + 0x7FF) / 0x800);
                r = (int)work->field_0C;
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
        ADXERR_CallErrFunc1_(adxf_mes.e2005101910);
        r = -3;
    } else {
        while (1) {
            int opened;
            if (work == 0) {
                ADXERR_CallErrFunc1_(adxf_mes.e2005121504);
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
        ADXERR_CallErrFunc1_(adxf_mes.e9040832);
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
        ADXERR_CallErrFunc1_(adxf_mes.e9040828_2);
        r = -3;
    } else {
        pt = (struct AdxFsWork *)adxf_pttbl[prm];
        if (pt == 0) {
            ADXERR_CallErrFunc1_(adxf_mes.e9040828_2);
            r = -3;
        } else if (pt->sectCnt <= 0) {
            ADXERR_CallErrFunc1_(adxf_mes.e2008110501);
            r = -3;
        } else if (sect < 0 || sect >= (int)pt->sectCnt) {
            ADXERR_CallErrFunc1_(adxf_mes.e9040828_3);
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
/* Round a byte length up to sectors (2048-byte units). */
static s32 adxf_SctRoundW(s32 v) {
    s32 q = v / 0x800;
    if (v % 0x800 > 0) {
        q++;
    }
    return q;
}

int adxf_GetFnameRangeEx(const char *fname, int flags, char *namebuf, u32 *a, u32 *b, u32 *c, u32 *d) {
    struct AdxFsPt *pt;
    s32 sum;
    int i;
    s32 *tbl;
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
    pt = (struct AdxFsPt *)((u32*)adxf_pttbl)[(int)fname];
    if (pt->hdr.b.b0F == 1) {
        /* word table: per-file byte lengths, rounded up to sectors */
        sum = pt->file.total / 0x800;
        if (pt->file.total % 0x800 > 0) {
            sum++;
        }
        {
            /* word table: per-file byte lengths, each rounded up to sectors */
            tbl = pt->field_11C;
            for (i = 0; i < flags; i++) {
                sum += adxf_SctRoundW(tbl[i]);
            }
            *c = adxf_SctRoundW(tbl[flags]);
            *d = tbl[flags];
        }
    } else {
        /* halfword table: sector offsets */
        sum = pt->file.hw.len;
        {
            u16 *offs = pt->file.hw.offs;
            for (i = 0; i < flags; i++) {
                sum += offs[i];
            }
            *c = offs[flags];
            *d = (u32)offs[flags] << 11;
        }
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
    p = (char *)((u32*)adxf_pttbl)[idx] + 0x10;
    ADXCRS_Leave();
    return p;
}

/* .bss: defined after the code, so every function addresses these through
 * their own @ha/@l symbol (no section anchor), as retail does. MWCC emits
 * .bss objects in reverse definition order. */
unsigned char adxf_workarea[0x2080];       /* 0x880 work area */
u32 lbl_eu_805E062C;                         /* 0x87C */
u32 lbl_eu_805E0628;                         /* 0x878 */
