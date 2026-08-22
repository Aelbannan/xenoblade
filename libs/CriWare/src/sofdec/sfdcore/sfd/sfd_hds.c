// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_hds
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

/* SFH runtime context - 0x18 bytes (created over a header image) */
typedef struct SFHContext {
    s32 active;       /* 0x00 */
    u32 field_0x04;   /* 0x04: header base */
    u32 field_0x08;   /* 0x08: header size */
    u32 field_0x0C;   /* 0x0C */
    u32 field_0x10;   /* 0x10 */
    u32 field_0x14;   /* 0x14 */
} SFHContext;

/* Parsed SFHD header information block (also used as the in-object
 * header work area at handle+0x88). Video sub-results live at 0x5C. */
typedef struct SfhdsInfo {
    s32 active;        /* 0x00 */
    s32 toolVerMajor;  /* 0x04 */
    s32 toolVerMinor;  /* 0x08 */
    s32 byteRate;      /* 0x0C */
    s32 hdrSize;       /* 0x10 */
    s32 packType;      /* 0x14 */
    s32 pketSizLen;    /* 0x18 */
    s32 packSiz;       /* 0x1C */
    s32 numElemTot;    /* 0x20 */
    s32 numElemAud;    /* 0x24 */
    s32 numElemVid;    /* 0x28 */
    s32 numElemPrv;    /* 0x2C */
    s32 maxPlyLenAud;  /* 0x30 */
    s32 maxPlyLenVid;  /* 0x34 */
    s32 maxFrmNum;     /* 0x38 */
    s32 audStmId;      /* 0x3C */
    s32 vidStmId;      /* 0x40 */
    s32 audElemId;     /* 0x44 */
    s32 vidElemId;     /* 0x48 */
    s32 elemCodecAud;  /* 0x4C */
    s32 elemLayer;     /* 0x50 */
    s32 elemChNum;     /* 0x54 */
    s32 elemSmpHz;     /* 0x58 */
    s32 video[13];     /* 0x5C..0x8F: sfhds_AnlyVideo results */
} SfhdsInfo;

/* SofDec player handle (fields touched by the header module) */
typedef struct SfdHdsHandle {
    u8 pad00[0x88];
    SfhdsInfo info;                /* 0x88: header work/analysis area */
    s32 hdrSize;                   /* 0x118 */
    u8 hdrBuf[0x800];              /* 0x11C */
    u8 pad91C[0x940 - 0x91C];
    s32 field_0x940;               /* cached info.numElemVid */
    s32 field_0x944;               /* cached info.numElemPrv */
    s32 field_0x948;               /* cached info.maxPlyLenAud */
    u8 pad94C[0xD3C - 0x94C];
    s32 cbCount;                   /* 0xD3C */
    void (*cbFunc)(void*, void*);  /* 0xD40 */
    u8 padD44[0x2670 - 0xD44];
    u8* concatHdr;                 /* 0x2670 */
} SfdHdsHandle;

extern SFHContext* SFH_Create(u32 base, s32 size, SFHContext* ctx);
extern s32 SFH_Destroy(SFHContext* ctx);
extern s32 SFH_IsSfdHeader(SFHContext* hdr, s32* outValid);
extern s32 SFH_AnlyHdrToolVer(SFHContext* hdr, s32* outMajor, s32* outMinor);
extern s32 SFH_AnlyByteRate(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyHdrSiz(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyPackType(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyPketSizLen(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyPackSiz(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyNumElemTot(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyNumElemAud(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyNumElemVid(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyNumElemPrv(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyMaxPlyLenAud(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyMaxPlyLenVid(SFHContext* hdr, s32* out);
extern s32 SFH_AnlyMaxFrmNum(SFHContext* hdr, s32* out);
extern s32 SFH_IsExistStmId(SFHContext* hdr, u8 id, s32* out);
extern s32 SFH_AnlyElemCodecAud(SFHContext* hdr, u8 id, s32* out);
extern s32 SFH_AnlyElemLayer(SFHContext* hdr, u8 id, s32* out);
extern s32 SFH_AnlyElemChNum(SFHContext* hdr, u8 id, s32* out);
extern s32 SFH_AnlyElemSmpHz(SFHContext* hdr, u8 id, s32* out);
extern void MEM_Copy(void* dst, void* src, u32 size);
extern void SFH_Init(void);
extern u32 SFMPS_GetConcatCnt(SfdHdsHandle* self);
extern s32 SFLIB_SetErr(s32 val, u32 err_code);

void sfhds_DoProcessHdr(SFHContext* hdr, SfhdsInfo* out);
void sfhds_AnlyVideo(void* self, u32 a, s32* out);

void SFHDS_Init(void) { SFH_Init(); }


void SFHDS_InitFhd(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x90) = 0;
}

void SFHDS_FinishFhd(void* self) {
    *(u32*)((u8*)self + 0x00) = 0;
    *(u32*)((u8*)self + 0x0c) = 0;
    *(u32*)((u8*)self + 0x90) = 0;
}

/* Re-parse the concatenated-file header: copy 0x898 bytes of header image
 * from concatHdr+0xC into the in-object work area, then run the analyzer. */
s32 SFHDS_ReprocessHdr(SfdHdsHandle* self) {
    SFHContext ctx;
    SFHContext* sfh;
    u32* d;
    u32* s;
    s32 n;
    s32 ret;

    if (self->concatHdr == NULL)
        return 0;
    if ((s32)SFMPS_GetConcatCnt(self) > 0)
        return 0;
    s = (u32*)(self->concatHdr + 0xC);
    if (s == NULL)
        return 0;

    d = (u32*)&self->info;
    n = 274;
    do {
        *d++ = *s++;
        *d++ = *s++;
    } while (--n != 0);
    *d = *s;

    sfh = SFH_Create((u32)self->hdrBuf, self->hdrSize, &ctx);
    if (sfh == NULL) {
        ret = SFLIB_SetErr(0, 0xFF000232);
    } else {
        sfhds_DoProcessHdr(sfh, &self->info);
        ret = SFH_Destroy(sfh);
    }

    self->field_0x940 = self->info.numElemVid;
    self->field_0x944 = self->info.numElemPrv;
    self->field_0x948 = self->info.maxPlyLenAud;
    return ret;
}

/* Install a new header image. Scans back from buf for the 0x1BF sync
 * marker (at buf-6 or buf-8), validates it via a throwaway SFHContext,
 * notifies the registered callback, copies up to 0x800 bytes into the
 * in-object header buffer and runs the analyzer over it. */
s32 SFHDS_SetHdr(SfdHdsHandle* self, s32 type, u8* buf, u8* end, s32* outFlag) {
    SFHContext probeCtx;
    SFHContext ctx;
    SFHContext* sfh;
    u8* start;
    u8* stop;
    u32 sync;
    s32 valid;
    s32 copySize;
    s32 n;
    u32* d;
    u32* s;
    u8 c0;
    u8 c1;
    u8 c2;
    u8 c3;

    *outFlag = 0;
    if (type != 2)
        return 0;

    start = buf - 6;
    stop = end + 6;
    c0 = start[0];
    c1 = start[1];
    c2 = start[2];
    c3 = start[3];
    sync = (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
    if (sync != 0x1BF) {
        /* retry two bytes further back */
        c0 = start[-2];
        c1 = start[-1];
        start -= 2;
        stop += 2;
        sync = (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
        if (sync != 0x1BF)
            return 0;
    }

    sfh = SFH_Create((u32)(start - 0xC), (s32)(stop + 0xC), &probeCtx);
    valid = 0;
    if (sfh != NULL) {
        if (SFH_IsSfdHeader(sfh, &valid) == 0)
            valid = 0;
        SFH_Destroy(sfh);
    }
    if (valid == 0)
        return 0;

    if (self->cbCount != 0)
        self->cbFunc(start - 0xC, stop + 0xC);

    if (self->info.active != 0) {
        *outFlag = 0;
        return 1;
    }

    copySize = 0x800;
    if ((s32)(stop + 0xC) < 0x800)
        copySize = (s32)(stop + 0xC);
    MEM_Copy(self->hdrBuf, start - 0xC, copySize);
    self->hdrSize = copySize;

    sfh = SFH_Create((u32)self->hdrBuf, self->hdrSize, &ctx);
    if (sfh == NULL) {
        SFLIB_SetErr(0, 0xFF000232);
    } else {
        sfhds_DoProcessHdr(sfh, &self->info);
        SFH_Destroy(sfh);
    }

    self->field_0x940 = self->info.numElemVid;
    self->field_0x944 = self->info.numElemPrv;
    self->field_0x948 = self->info.maxPlyLenAud;

    if (self->concatHdr != NULL && (s32)SFMPS_GetConcatCnt(self) <= 0) {
        s = (u32*)(self->concatHdr + 0xC);
        if (s != NULL) {
            d = (u32*)&self->info;
            n = 274;
            do {
                *d++ = *s++;
                *d++ = *s++;
            } while (--n != 0);
            *d = *s;
        }
    }

    *outFlag = 1;
    return 1;
}

/* Run the full header analysis over an SFHContext, filling out. */
void sfhds_DoProcessHdr(SFHContext* hdr, SfhdsInfo* out) {
    s32 toolMaj;
    s32 toolMin;
    s32 byteRate;
    s32 tmp;
    s32 ver;
    s32 valid;
    s32 id;
    s32 i;

    if (SFH_IsSfdHeader(hdr, &valid) == 0)
        valid = 0;
    if (valid == 0)
        return;

    if (SFH_AnlyHdrToolVer(hdr, &toolMaj, &toolMin) == 0) {
        toolMaj = 0;
        toolMin = 0;
    }
    out->toolVerMajor = toolMaj;
    out->toolVerMinor = toolMin;
    ver = toolMin + toolMaj * 100;

    if (SFH_AnlyByteRate(hdr, &byteRate) == 0)
        byteRate = 0;
    /* old tool versions stored the byte rate negated */
    if (ver < 0x6E)
        byteRate = -byteRate;
    out->byteRate = byteRate;

    if (SFH_AnlyHdrSiz(hdr, &tmp) == 0)
        tmp = -1;
    out->hdrSize = tmp;

    if (SFH_AnlyPackType(hdr, &tmp) == 0)
        tmp = -1;
    out->packType = tmp;

    if (SFH_AnlyPketSizLen(hdr, &tmp) == 0)
        tmp = -1;
    if (tmp == -1)
        tmp = 2;
    out->pketSizLen = tmp;

    if (SFH_AnlyPackSiz(hdr, &tmp) == 0)
        tmp = -1;
    out->packSiz = tmp;

    if (SFH_AnlyNumElemTot(hdr, &tmp) == 0)
        tmp = -1;
    out->numElemTot = tmp;

    if (SFH_AnlyNumElemAud(hdr, &tmp) == 0)
        tmp = -1;
    out->numElemAud = tmp;

    if (SFH_AnlyNumElemVid(hdr, &tmp) == 0)
        tmp = -1;
    out->numElemVid = tmp;

    if (SFH_AnlyNumElemPrv(hdr, &tmp) == 0)
        tmp = -1;
    out->numElemPrv = tmp;

    if (SFH_AnlyMaxPlyLenAud(hdr, &tmp) == 0)
        tmp = -1;
    out->maxPlyLenAud = tmp;

    if (SFH_AnlyMaxPlyLenVid(hdr, &tmp) == 0)
        tmp = -1;
    out->maxPlyLenVid = tmp;

    if (SFH_AnlyMaxFrmNum(hdr, &tmp) == 0)
        tmp = -1;
    out->maxFrmNum = tmp;

    if (SFH_IsExistStmId(hdr, 0xBD, &tmp) != 0 && tmp != 0)
        out->audStmId = 0xBD;
    else
        out->audStmId = 0;

    if (SFH_IsExistStmId(hdr, 0xBF, &tmp) != 0 && tmp != 0)
        out->vidStmId = 0xBF;
    else
        out->vidStmId = 0;

    /* find the first private-stream audio element id in 0xC0..0xDF */
    out->audElemId = 0;
    for (i = 0xC0; i <= 0xDF; i++) {
        if (SFH_IsExistStmId(hdr, (u8)i, &tmp) != 0 && tmp != 0) {
            out->audElemId = i;
            break;
        }
    }

    /* find the first private-stream video element id in 0xE0..0xEF */
    out->vidElemId = 0;
    for (i = 0xE0; i <= 0xEF; i++) {
        if (SFH_IsExistStmId(hdr, (u8)i, &tmp) != 0 && tmp != 0) {
            out->vidElemId = i;
            break;
        }
    }

    if (out->audElemId != 0) {
        id = out->audElemId;
        if (SFH_AnlyElemCodecAud(hdr, (u8)id, &tmp) == 0)
            tmp = -1;
        out->elemCodecAud = tmp;

        if (SFH_AnlyElemLayer(hdr, (u8)id, &tmp) == 0)
            tmp = -1;
        out->elemLayer = tmp;

        if (SFH_AnlyElemChNum(hdr, (u8)id, &tmp) == 0)
            tmp = -1;
        out->elemChNum = tmp;

        if (SFH_AnlyElemSmpHz(hdr, (u8)id, &tmp) == 0)
            tmp = -1;
        out->elemSmpHz = tmp;
    }

    sfhds_AnlyVideo(hdr, out->vidElemId, out->video);
    out->active = 1;
}

extern s32 SFH_AnlyElemCodecVid(void*, u8, s32*);
extern s32 SFH_AnlyElemBitRate(void*, u8, s32*);
extern s32 SFH_AnlyElemPicSz(void*, u8, s32*, s32*);
extern s32 SFH_AnlyElemPicRate(void*, u8, s32*);
extern s32 SFH_IsEffFtrInf(void*, u8, s32*);
extern s32 SFH_AnlyFtrColType(void*, u8, s32*);
extern s32 SFH_AnlyFtrPicType(void*, u8, s32*);
extern s32 SFH_AnlyFtrFixFlg(void*, u8, s32*);
extern s32 SFH_AnlyFtrShcFixFlg(void*, u8, s32*);
extern s32 SFH_AnlyFtrExpand(void*, u8, s32*);
extern s32 SFH_AnlyFtrGopN(void*, u8, s32*);
extern s32 SFH_AnlyFtrGopM(void*, u8, s32*);

void sfhds_AnlyVideo(void* self, u32 a, s32* out) {
    s32 tEff, tCodec, tBitRate, tPicRate, tColType, tPicType, tFixFlg, tShcFixFlg, tExpand, tGopN, tGopM;
    s32 v;

    if (SFH_AnlyElemCodecVid(self, (u8)a, &tCodec) == 0)
        v = -1;
    else
        v = tCodec;
    out[0] = v;

    if (SFH_AnlyElemBitRate(self, (u8)a, &tBitRate) == 0)
        v = -1;
    else
        v = tBitRate;
    out[1] = v;

    if (SFH_AnlyElemPicSz(self, (u8)a, &out[2], &out[3]) == 0) {
        out[2] = -1;
        out[3] = -1;
    }

    if (SFH_AnlyElemPicRate(self, (u8)a, &tPicRate) == 0)
        v = -1;
    else
        v = tPicRate;
    out[4] = v;

    if (SFH_IsEffFtrInf(self, (u8)a, &tEff) == 0)
        tEff = 0;
    out[5] = (tEff != 0);
    if (tEff == 0)
        return;

    if (SFH_AnlyFtrColType(self, (u8)a, &tColType) == 0)
        v = -1;
    else
        v = tColType;
    out[6] = v;

    if (SFH_AnlyFtrPicType(self, (u8)a, &tPicType) == 0)
        v = -1;
    else
        v = tPicType;
    out[7] = v;

    if (SFH_AnlyFtrFixFlg(self, (u8)a, &tFixFlg) == 0)
        v = -1;
    else
        v = tFixFlg;
    out[8] = v;

    if (SFH_AnlyFtrShcFixFlg(self, (u8)a, &tShcFixFlg) == 0)
        v = -1;
    else
        v = tShcFixFlg;
    out[9] = v;

    if (SFH_AnlyFtrExpand(self, (u8)a, &tExpand) == 0)
        v = -1;
    else
        v = tExpand;
    out[10] = v;

    if (SFH_AnlyFtrGopN(self, (u8)a, &tGopN) == 0)
        v = -1;
    else
        v = tGopN;
    out[11] = v;

    if (SFH_AnlyFtrGopM(self, (u8)a, &tGopM) == 0)
        v = -1;
    else
        v = tGopM;
    out[12] = v;
}

typedef struct {
    unsigned char pad0[0x88];
    int field_0x88;
    int field_0x8c;
    int field_0x90;
} SfdHdsStruct;

int SFHDS_GetMuxVerNum(SfdHdsStruct* this) {
    if (this->field_0x88 == 0) {
        return 0;
    }
    return this->field_0x8c * 100 + this->field_0x90;
}

int SFHDS_GetColType(void* obj) {
    struct SubStruct {
        unsigned char pad[0x14];
        int field_14;
        int field_18;
    };
    struct MainStruct {
        unsigned char pad0[0x88];
        int flag;
        unsigned char pad1[0x58];
        struct SubStruct sub;
    };
    struct MainStruct* p = (struct MainStruct*)obj;
    if (p->flag == 0) return -1;
    struct SubStruct* s = &p->sub;
    if (s->field_14 == 0) return -1;
    return s->field_18;
}

extern char lbl_eu_80606E38[];
void* fn_803C3300(void) {
    return lbl_eu_80606E38 + 0x21c;
}
