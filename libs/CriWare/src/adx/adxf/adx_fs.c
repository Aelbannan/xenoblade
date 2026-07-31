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

void ADXT_ExecFsSvr(void);
void adxt_ExecFsSvr(void);

/* ADX file partition / work-item structure.
 * Shared header layout: flag at 0x0, signed status at 0x1, ADXSTM handle at
 * 0x4, sector count at 0x8. Work items are 0x34 bytes each. */
struct AdxFsWork {
    s8    flag;      /* 0x00 */
    s8    status;    /* 0x01 */
    u16   seq;       /* 0x02 */
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

/* internal stubs */
int adxf_LoadPtBothNw(s32 p1, int p2, int p3, const char *p4, void *p5, int p6, int p7, void *p8, void *p9, int p10, int p11);
int adxf_GetPtStat(int a);
int adxf_CreateAdxFs(void);
int adxf_ReadNw32(void *a, void *b, int c);
int adxf_Stop(int a);
int adxf_ExecOne(void *a);
int adxf_Seek(void *a, int b, int c);
int adxf_GetFnameRangeEx(const char *a, int b, void *c, u32 *d, u32 *e, u32 *f, u32 *g);

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

int adxf_CreateAdxFs(void) {
    return 0;
}

int ADXF_OpenNw(int a, int b, int c, int d) {
    int r;
    ADXCRS_Enter();
    r = 0;
    ADXCRS_Leave();
    return r;
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

int ADXF_Close(int a) {
    int r;
    ADXCRS_Enter();
    r = 0;
    ADXCRS_Leave();
    return r;
}

int ADXF_CloseAll(void) {
    int r = 0;
    /* close all handles */
    return r;
}

int adxf_read_sj32(int a, int b) { return 0; }
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

int ADXF_Stop(int a) { int r; ADXCRS_Enter(); r = adxf_Stop(a); ADXCRS_Leave(); return r; }
#pragma push
#pragma auto_inline off
int adxf_Stop(int a) { return 0; }
#pragma pop

int adxf_ExecOne(void *a);
#pragma push
#pragma auto_inline off
int adxf_ExecOne(void *a) { return 0; }
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

int ADXF_GetFsizeSct(int a) { int r; ADXCRS_Enter(); r = 0; ADXCRS_Leave(); return r; }

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

int adxf_GetFnameRangeEx(const char *a, int b, void *c, u32 *d, u32 *e, u32 *f, u32 *g) { return 0; }

char *ADXF_GetFnameFromPt(int idx) {
    char *p;
    ADXCRS_Enter();
    p = (char *)lbl_eu_805E00F0[idx] + 0x10;
    ADXCRS_Leave();
    return p;
}
