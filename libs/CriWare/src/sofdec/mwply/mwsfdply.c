#include <harness_catalog.h>

extern int SFD_Start(void *);
extern int SFD_TermSupply(void *);
extern int SFD_Stop(void *);
extern int SFD_Pause(void *, int);
extern int SFD_RecordFname(void *, const char *);
extern int MWSFSVM_Error(const char *, ...);
extern int MWSFLIB_SetErrCode(int);
extern int mw_sfd_start_ex(void *, void *);
extern void MWSFD_SetFlowLimit(void *, u32);
extern int MWSTM_GetStat(void *);
extern size_t strlen(const char *);
extern char *strncpy(char *, const char *, size_t);
extern char *strcpy(char *, const char *);
extern int MWSFD_IsFsBdr(void *);
extern int MWSFSVR_IsSvrBdrHndl(void *);

extern char lbl_eu_8051B1A0[];
extern double lbl_eu_8051B190;

typedef struct LscHandle LscHandle;

/* Movie player handle (MWSFDPLY). Partial layout covering the fields touched by
 * the decode/stop/pause helpers in this TU; unknown gaps are opaque byte runs. */
typedef struct MWSFDPLY {
    s32  field_0x0;         /* 0x00 */
    s32  field_0x4;         /* 0x04 decode status / error */
    s32  field_0x8;         /* 0x08 mode */
    u8   field_0x0c[0x4c];  /* 0x0c..0x57 */
    void*    field_0x58;    /* 0x58 SFD decode handle */
    void*    field_0x5c;    /* 0x5c STM handle */
    u8   field_0x60[0x4];   /* 0x60..0x63 */
    LscHandle* field_0x64;  /* 0x64 LSC handle */
    u8   field_0x68[0x2a];  /* 0x68..0x91 */
    s8   field_0x92;        /* 0x92 pause/seek flag */
    u8   field_0x93;        /* 0x93 */
    s32  field_0x94;        /* 0x94 */
    u8   field_0x98[0x468]; /* 0x98..0x4ff */
    s32  field_0x500;       /* 0x500 */
    u8   field_0x504[0x10]; /* 0x504..0x513 */
    s32  field_0x514;       /* 0x514 */
    s32  field_0x518;       /* 0x518 */
    s32  field_0x51c;       /* 0x51c */
    s32  field_0x520;       /* 0x520 */
    void*    field_0x524;   /* 0x524 callback object */
    u8   field_0x528[0x8];  /* 0x528..0x52f */
    s32  field_0x530;       /* 0x530 */
    s32  field_0x534;       /* 0x534 */
    s32  field_0x538;       /* 0x538 */
    s32  field_0x53c;       /* 0x53c */
    u8   field_0x540[0x98]; /* 0x540..0x5d7 */
    u8   field_0x5d8[0x20]; /* 0x5d8 stream-set A (SstHn) */
    u8   field_0x5f8[0x8];  /* 0x5f8..0x5ff */
    u8   field_0x600[0x20]; /* 0x600 stream-set B (SstHn) */
    u8   field_0x620[0x10]; /* 0x620..0x62f */
    s32  field_0x630;       /* 0x630 */
    u8   field_0x634[0xc];  /* 0x634..0x63f */
    s32  field_0x640;       /* 0x640 */
    s32  field_0x644;       /* 0x644 */
    s32  field_0x648;       /* 0x648 */
    s32  field_0x64c;       /* 0x64c */
    u8   field_0x650[0x10]; /* 0x650..0x65f */
    s32  field_0x660;       /* 0x660 */
    s32  field_0x664;       /* 0x664 */
    u8   field_0x668[0x10]; /* 0x668..0x677 */
    s32  field_0x678;       /* 0x678 */
} MWSFDPLY;

/* SVM trace-callback infrastructure: lbl_eu_805FF3A0 is an optional trace
 * object whose vtable exposes a method at +0x24; each instrumented helper
 * stores its args into a per-function record (lbl_eu_80566*) and fires the
 * callback on entry (sub-record at +0x04) and exit (+0x6c). */
typedef struct SfdTraceVtable {
    u8  pad_0x00[0x24];
    void (*trace)(void* self, void* rec);
} SfdTraceVtable;
typedef struct SfdTraceObj {
    const SfdTraceVtable* vtable;
} SfdTraceObj;
typedef struct SfdTraceRec {
    u32 name;           /* 0x00 */
    u32 entry;          /* 0x04 */
    u32 field_0x08;     /* 0x08 */
    u32 self;           /* 0x0c */
    u8  pad_0x10[0x5c]; /* 0x10..0x6b */
    u32 exit;           /* 0x6c */
} SfdTraceRec;

/* Callback object stored at +0x524; vtable method at +0x14. */
typedef struct SfdCbVtable {
    u8  pad_0x00[0x14];
    void (*method)(void* self);
} SfdCbVtable;
typedef struct SfdCbObj {
    const SfdCbVtable* vtable;
} SfdCbObj;

/* --- Cross-TU helpers referenced by the targets below --- */
extern SfdTraceObj* lbl_eu_805FF3A0;
extern SfdTraceRec lbl_eu_805669F4;
extern SfdTraceRec lbl_eu_80566920;
extern s32 MWSFD_IsEnableHndl(void* self);
extern void SFD_RequestStop(void* sst);
extern void MWSFD_RequestStopRead(void* self);
extern void MWSFD_SetProhibitServer(int on);
extern void mwlSfdSleepDecSvr(void* self);
extern void MWSST_Stop(void* sst);
extern void MWSST_GetTime(void* sst, s32 mode);
extern void MWSTM_ReqStop(void* stm);
extern void LSC_Stop(LscHandle* handle);
extern void MWSFLSC_Pause(void* self, s32 flag);
extern s32 SFD_GetCond(void* self, u32 idx, s32* out);
extern s32 MWSFD_GetPauseBdr(void);
extern void mwPlyLinkStm(void* self, s32 linkStm);

/* Get SFD handle from player handle (+0x58) */
static void *sfd(void *h) { return *(void **)((u8 *)h + 0x58); }

int mwPlySfdStart(void *h) {
    if (SFD_Start(sfd(h))) {
        MWSFLIB_SetErrCode(-0x133);
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x1B);
        return -0x133;
    }
    return 0;
}

int mwPlyTermSupply(void *h) {
    if (SFD_TermSupply(sfd(h)))
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x62);
}

static u32 sfdStartExScratch[32];

/* Stub for the retail 0x298-byte mw_sfd_start_ex (not a move target in this
 * batch). A bulky body keeps MWCC from inlining it into mwPlyStartSj and
 * dropping the `bl` (small stubs get folded away). Computes only into a
 * private static buffer, so it is runtime-safe. */
int mw_sfd_start_ex(void *a, void *b) {
    u32 base = (u32)(u32)(long)a;
    u32 v = (u32)(long)b;
    u32 acc = v + 0x1234;
    int i, j;
    for (i = 0; i < 16; i++) {
        sfdStartExScratch[i] = acc;
        acc = acc * 0x1f + (u32)i;
    }
    for (j = 0; j < 16; j++) {
        u32 t = sfdStartExScratch[j];
        int k;
        for (k = 0; k < 4; k++)
            t = t * 3 + sfdStartExScratch[j];
        sfdStartExScratch[j] = t;
    }
    return (int)(acc + base + sfdStartExScratch[base & 0xf]);
}

int mwPlyStartFname(void *h, const char *fname) {
    return mw_sfd_start_ex(h, (void *)fname);
}


void mwSfdStopDec(MWSFDPLY* self) {
    void* sfdH = self->field_0x58;
    if (!sfdH)
        return;
    mwlSfdSleepDecSvr(self);
    self->field_0x4 = 0;
    self->field_0x58 = 0;
    if (SFD_Stop(sfdH)) {
        MWSFLIB_SetErrCode(-0x134);
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x361);
    }
    self->field_0x58 = sfdH;
    MWSST_Stop(self->field_0x5d8);
    MWSST_Stop(self->field_0x600);
    if (self->field_0x5c) {
        MWSTM_ReqStop(self->field_0x5c);
    }
    if (self->field_0x64) {
        LSC_Stop(self->field_0x64);
    }
    {
        void* cb = self->field_0x524;
        self->field_0x538 = 0;
        self->field_0x530 = 0;
        self->field_0x534 = 0;
        self->field_0x53c = 0;
        if (cb) {
            ((void (*)(void*))*(void**)((char*)*(void**)cb + 0x14))(cb);
        }
    }
    if (self->field_0x644 == 0) {
        self->field_0x64c = 0;
        self->field_0x648 = 0;
        self->field_0x640 = 0;
        self->field_0x664 = 0;
        self->field_0x660 = -1;
    } else {
        self->field_0x64c = 2;
    }
    self->field_0x678 = 0;
    MWSFLSC_Pause(self, 0);
}

void mwPlyStop(MWSFDPLY* self) {
    if (lbl_eu_805FF3A0) {
        lbl_eu_805669F4.self = (u32)self;
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_805669F4.entry);
    }
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x380);
    } else {
        if (self->field_0x4 != 0 && self->field_0x678 != 1) {
            /* comma operator keeps each field store before its arg load */
            SFD_RequestStop((self->field_0x678 = 1, self->field_0x58));
            MWSFD_RequestStopRead(self);
        }
    }
    if (lbl_eu_805FF3A0) {
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_805669F4.exit);
    }
    if (lbl_eu_805FF3A0) {
        lbl_eu_80566920.self = (u32)self;
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566920.entry);
    }
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x3ae);
    } else {
        mwSfdStopDec(self);
        mwPlyLinkStm(self, 0);
        LSC_Stop((self->field_0x94 = 0, self->field_0x64));
    }
    if (lbl_eu_805FF3A0) {
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566920.exit);
    }
}

void mwPlyPause(MWSFDPLY* self, s32 pause) {
    s8 cur = self->field_0x92;
    void* sfdH;
    s32 cond;

    if (cur == 0 && pause == 0)
        return;
    if (cur == 1 && pause == 1)
        return;
    sfdH = self->field_0x58;
    if (MWSFD_GetPauseBdr() == 1 && self->field_0x8 == 1) {
        if (SFD_GetCond(sfdH, 6, &cond) == 0) {
            if (cond == 1)
                mwlSfdSleepDecSvr(self);
        } else {
            mwlSfdSleepDecSvr(self);
        }
    }
    if (SFD_Pause(sfdH, pause) != 0) {
        MWSFLIB_SetErrCode(-0x136);
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x106,
                      (pause == 1) ? (char*)(lbl_eu_8051B1A0 + 0x129)
                                   : (char*)(lbl_eu_8051B1A0 + 0x12c));
    }
    MWSST_GetTime(self->field_0x5d8, pause);
    MWSST_GetTime(self->field_0x600, pause);
    self->field_0x92 = (s8)pause;
}

int MWSFPLY_SetFlowLimit(void *h) {
    MWSFD_SetFlowLimit(h,
        (u32)(s32)(lbl_eu_8051B190 * (double)(s32)*(s32 *)((u8 *)h + 0x50C)));
}

int mwPlyChkSupply(void *h) {
    void *stm = *(void **)((u8 *)h + 0x5C);
    if (stm && MWSTM_GetStat(stm) == 3 && SFD_TermSupply(*(void **)((u8 *)h + 0x58)))
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x62);
}

int MWSFPLY_RecordFname(void *h, const char *fname) {
    if ((s32)strlen(fname) > *(s32 *)((u8 *)h + 0x4EC)) {
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x185);
        strncpy(*(char **)((u8 *)h + 0x4E8), fname, *(u32 *)((u8 *)h + 0x4EC));
    } else {
        strcpy(*(char **)((u8 *)h + 0x4E8), fname);
    }
}

void mwPlyStartSj(MWSFDPLY* self, s32 sj) {
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x336);
        return;
    }
    MWSFD_SetProhibitServer(1);
    mwSfdStopDec(self);
    self->field_0x500 = sj;
    self->field_0x518 = 0;
    self->field_0x51c = 0;
    self->field_0x520 = 0;
    /* code is the mw_sfd_start_ex arg (lives in r4); the field store reads the
     * same register so MWCC emits a single `li r4, 2` for both. */
    {
        void* code = (void*)2;
        self->field_0x514 = (s32)(long)code;
        mw_sfd_start_ex(self, code);
    }
    MWSFD_SetProhibitServer(0);
}

void fn_803A537C(MWSFDPLY* self) {
    if (lbl_eu_805FF3A0) {
        lbl_eu_805669F4.self = (u32)self;
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_805669F4.entry);
    }
    if (MWSFD_IsEnableHndl(self) != 1) {
        MWSFSVM_Error(lbl_eu_8051B1A0 + 0x380);
    } else {
        if (self->field_0x4 != 0 && self->field_0x678 != 1) {
            /* comma operator keeps the field store before the arg load */
            SFD_RequestStop((self->field_0x678 = 1, self->field_0x58));
            MWSFD_RequestStopRead(self);
        }
    }
    if (lbl_eu_805FF3A0) {
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_805669F4.exit);
    }
}

int MWSFD_IsEndPrepareStop(void *h) {
    if (MWSFD_IsFsBdr(h) == 0)
        return 0;
    return !!MWSFSVR_IsSvrBdrHndl(h);
}
