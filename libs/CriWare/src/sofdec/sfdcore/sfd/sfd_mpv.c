// Decompiled CriWare Sofdec sfd_mpv module.
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

// Absorbed from criware_data (retail bytes) - generated
__attribute__((aligned(8))) unsigned char __bss_fill_0[0xCD8]; // filler gap at 0x0 for .bss
u32 sMpvWorkArea; // bss off 0xCD8 (scalar, spec 0x12020)
unsigned char __bss_fill_1[0x1201C]; // filler gap at 0xCDC for .bss
u32 sMpvInitCounter; // bss off 0x12CF8 (scalar, spec 0x8)
unsigned char __bss_fill_2[0x94]; // trailing filler for .bss

// ---------------------------------------------------------------------------
// External declarations
// ---------------------------------------------------------------------------

s32 MPV_SetCond(void* mpv, s32 cond, s32 val);
s32 MPV_GetCond(void* mpv, s32 cond, s32* out);
s32 MPV_SetErrFunc(void* mpv, void* cb, void* arg);
s32 MPV_Init(u32 a, void* b);
void MPV_Finish(void);
u32 MPV_Create(void);
s32 MPV_Destroy(void* mpv);
s32 MPV_DecodePicAtr(void* handle, void* in, void* out);
s32 MPV_GetLinkFlg(void* handle, u32* out_prev, u32* out_next);
s32 MPV_GetBitRate(void* handle, u32* out);
s32 MPV_GetVbvBufSiz(void* handle, u32* out_size, u32* out_avg, u32* out_max);
s32 MPV_CheckDelim(const u8* buf);
u32 MPV_SearchDelim(const u8* buf, u32 size, u32 mask);
u32 MPV_BsearchDelim(const u8* buf, u32 size, u32 mask);
void MPV_SetPicUsrBuf(void* self, void* a, void* b);
s32 MPV_DecodePicAtrSj(void* mpv, void* sj);
s32 MPV_GetPicAtr(void* handle, void* out);
void MPV_GetPicUsr(void* self, u32* out_top, u32* out_bot);
s32 MPV_SkipFrmSj(void* mpv, void* sj);
s32 MPV_DecodeFrmSj(void* mpv, void* sj, void* out);
void MPV_GetDctCnt(void* self, u32* out1, u32* out2);

s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFSET_GetCond(void* self, u32 idx);
void SFSET_SetCond(void* self, u32 idx, u32 val);
s32 SFHDS_GetColType(void* obj);
s32 SFBUF_GetTermFlg(void* self, s32 idx);
void SFBUF_SetTermFlg(void* self, s32 idx, u32 val);
s32 SFBUF_GetPrepFlg(void* self, int idx);
void SFBUF_SetPrepFlg(void* self, u32 idx, u32 val);
s32 SFBUF_RingGetRead(void* self, int idx, u32* out);
void SFBUF_RingGetDlm(void* self, int idx, u32* dlm, u32* size);
void SFBUF_RingSetDlm(void* self, int idx, u32 dlm, u32 size);
u32 SFBUF_GetRingBufSiz(void* self, u32 idx);
s32 SFBUF_GetWTot(void* self, int idx);
s32 SFBUF_RingGetSj(void* self, s32 idx, void** out);
void SFBUF_RingAddRead(void* self, int idx, u32 size);
void SFBUF_GetFlowCnt(void* self, u32* readCnt, u32* writeCnt);
u64 SFBUF_UpdateFlowCnt(u64 v, u32 x);
void SFBUF_AddRtotSj(void* self, s32 idx, s32 size);
u32 SFBUF_GetRTot(void* self, s32 idx);
u32 SFBUF_RingGetDataSiz(void* self, s32 idx);
s32 SJRBF_GetFlowCnt(void* self, u32 a, u32 b);
s32 MPV_RequestStop(void* mpv);

s32 SFCON_IsEndcodeSkip(void* h);
s32 SFCON_IsVideoEndcodeSkip(void* h);
void SFCON_UpdateConcatTime(void* h, s32 delta);
s32 SFCON_ReadTotSmplQue(void* h, s32* lastSmpl, s32* value);
s32 SFSEEKI_IsSkip(void* hn, u32* outSkipCount);
u32 SFTRN_IsSetup(void* self, u32 idx);
u32 SFPLY_GetResetFlg(void);
void SFPLY_AddSkipPic(void* self, u32 a, u32 b);
void SFPLY_AddDecPic(void* self, u32 a, u32 b);

void SFMPVF_InitPool(void);
s32 SFMPVF_ReadGlobalMpvPara(void* self);
s32 SFMPVF_WriteGlobalMpvPara(void* self);
void SFMPVF_ResetPicUsrBuf(void* self);
void SFMPVF_InitPicUsr(void* self);
void SFMPVF_SetGopStat(void* self);
void SFMPVF_TermDec(void* self);
u32 SFMPVF_IsTermDec(void* self);
s32 SFMPVF_GetNumFrm(void* self);
s32 SFMPVF_ChkImageSize(void* self, u32 w, u32 h);
void* SFMPVF_AllocFrm(void* self);
void SFMPVF_FreeFrm(void* frm);
void SFMPVF_RefStbyFrm(void* frm);
void SFMPVF_StbyFrm(void* frm);
void SFMPVF_EndRefFrm(void* frm);
void SFTIM_InitTtu(void* self, int val);
void SFTIM_Tc2Time(void* tcdata, s32* out1, s32* out2);
void SFTIM_UpdateItime(void* self, s32 val);
s32 SFTIM_GetNextItime(void* self, s32 val);
void SFTIM_GetSpeed(void* self, s32* out1, s32* out2);
void SFTIM_GetTime(void* self, s32* out1, s32* out2);
s64 SFTMR_GetTmr(void* self);
void SFTMR_AddTsum(void* self, s64 val);
s32 SFPTS_ReadPtsQue(void* self, s32 idx, u32 delim, void* out);
int UTY_MulDiv(int a, int b, int c);
s64 UTY_MulDivRound64(s64 a, s64 b, s64 c);
s32 UTY_CmpTime(s32 a, s32 b, s32 c, s32 d);
int UTY_MemsetDword(u32* dst, u32 val, int count);
int MEM_Copy(void* dst, void* src, u32 size);
void* memcpy(void* dst, const void* src, u32 size);
int memcmp(const void* s1, const void* s2, u32 size);

// Internal functions
void sfmpv_ProcessAuxShc(SfdHandle* self);
s32 sfmpv_IsVbvEnough(SfdHandle* self);
u32 sfmpv_SearchDelim(u32* buf, u32 mask, u32* out);
u32 sfmpv_BsearchDelim(u32* buf, u32 mask, u32* out);
s32 sfmpv_NeedSafeDlmRefresh(u32* buf, u32 dlm, u32 dlm_new);
s32 sfmpv_GetActiveSize(SfdHandle* self, s32* out1, s32* out2, s32* out3);
s32 sfmpv_DecodeOneUnit(SfdHandle* self, s32 ch, s32 pat, s32 dlm, s32* out);
s32 sfmpv_ConcatSub(SfdHandle* self);
s32 sfmpv_DecodePicAtr(SfdHandle* self, u32* pic, void* sj, s32 pat, s32* out);
s64 sfmpv_ComplementPts(SfdTim* self, u32* dst, SfdMpvPicAtr* frm, u32* pts, s32 mode, s32* out1, s32 rate);
void sfmpv_CalcRepeatField(SfdHandle* self, SfdMpvPicAtr* frm, s32 mode);
void sfmpv_DoReformTc(SfdHandle* self, SfdMpvPicAtr* frm, s64 pts, s32 rep);
void sfmpv_Pts2Tc(s64 a, s32 v, s32 t, s32 u, u32* out);
void sfmpv_NextTc(s32* in, s32* out);
s32 sfmpv_FirstPicAtr(SfdHandle* self, void* mpv, SfdMpvPicAtr* frm, u32* pic);
void sfmpv_SetMpvHd(SfdHandle* self, s32 bitrate, u32* pic);
s32 sfmpv_ChkBufSiz(SfdHandle* self, void* para);
s32 sfmpv_IsSkip(SfdHandle* self, void* bpic);
s32 sfmpv_IsEmptyBpic(SfdHandle* self, s32 type, void* bpic);
void sfmpv_UpdateDefect(SfdHandle* self, SfdMpvPicAtr* frm, s32 mode);
s32 sfmpv_IsLate(SfdHandle* self, s32 type);
s32 sfmpv_SkipFrm(SfdHandle* self, void* sj);
s32 sfmpv_DecodeFrm(SfdHandle* self, void* sj);
s32 sfmpv_SetFrmPara(SfdHandle* self, SfdMpvPicAtr* frm, SfdMpvFrmPara* para, SfdFrame** out);
void fn_803C9948(SfdHandle* a, SfdMpvSession* b, s32* out1, s32* out2);
void fn_803C99C8(SfdHandle* self);
s32 sfmpv_GoDdelim(SfdHandle* self, u32 unused, u32 mask);
s32 sfmpv_InitInf(SfdHandle* self, SfdMpvSession* shc);
void sfmpv_InitFrmObj(SfdFrame* frm, const u32* src, s32 count);
s32 sfmpv_ReprocessShc(SfdHandle* self, SfdMpvSession* shc, s32* out);
void sfmpv_SetFrmInf(SfdHandle* self, SfdFrame* frm, SfdFrmInfo* info);
s32 SFMPV_Seek(SfdHandle* self);

void sfmpv_ErrFn(s32 val, u32 err);
s32 fn_803C34F8(s32 val, u32 err_code);

// Global data
extern void* gpSfdCoreTraceCb;
extern u32 lbl_eu_8051C4E0;
int SFMPV_RequestStop(SfdHandle* self); int SFMPV_Start(void); int SFMPV_Stop(void); int SFMPV_Pause(void);
s32 SFMPV_GetWrite(void* h); s32 SFMPV_AddWrite(void* h); void SFMPV_GetRead(void* self, void* a, void* b);
void SFMPV_AddRead(void); int fn_803CA368(void); int SFMPV_Finish(void); s32 SFMPV_Init(void);
s32 SFMPV_ExecServer(SfdHandle* self); s32 SFMPV_Create(SfdHandle* self); s32 SFMPV_Destroy(SfdHandle* self);
/* JP map: SFD_tr_vd_mpv (MPEG video decode transfer module interface) */
const SfdTrIf SFD_tr_vd_mpv = {
    SFMPV_Init, SFMPV_Finish, SFMPV_ExecServer, SFMPV_Create,
    SFMPV_Destroy, SFMPV_RequestStop, SFMPV_Start, SFMPV_Stop,
    SFMPV_Pause, SFMPV_GetWrite, SFMPV_AddWrite, SFMPV_GetRead,
    SFMPV_AddRead, SFMPV_Seek, fn_803CA368,
};
const s32 sfmpv_fps_round[9] = {0, 24, 24, 25, 30, 30, 50, 60, 60};
const s32 sfmpv_conv_29_97[8] = {107892, 17982, 1800, 1798, 28, 30, 10, 2};
const s32 sfmpv_conv_59_94[8] = {215784, 35964, 3600, 3596, 56, 60, 10, 4};
extern const s32 SFTIM_prate[];
extern u32 sTraceSfmpvExecServer[];
extern u32 sTraceDecodePicAtr[];
extern u32 sTraceDecodeFrm[];
extern u32 sTraceDecodePicAtrRet[];
extern SfdTrcRecIn sTraceSfdIsSkip;
extern u32 sTraceSfdWaitSupply[];
extern u32 sTraceSfdWaitRelFrm[];
extern u32 sTraceSfmpvConcatSub[];

// ---------------------------------------------------------------------------
// SFD_SetMpvCond
// ---------------------------------------------------------------------------
s32 SFD_SetMpvCond(SfdHandle* h, s32 cond, s32 val) {
    void* mpv;
    if (h == NULL) {
        mpv = NULL;
    } else {
        if (SFLIB_CheckHn(h) != 0) {
            return SFLIB_SetErr(0, 0xff000181);
        }
        mpv = *(void**)h->trn[2].hdr;
    }
    if (cond == 5) {
        val = 0;
    }
    if (MPV_SetCond(mpv, cond, val) != 0) {
        return SFLIB_SetErr(h, 0xff000f12);
    }
    return 0;
}

// ---------------------------------------------------------------------------
// SFMPV_SaveCond
// ---------------------------------------------------------------------------
s32 SFMPV_SaveCond(SfdHandle* h, s32* conds, u32 count) {
    void* mpv;
    s32 i;
    s32 max;
    u32 n;
    mpv = *(void**)h->trn[2].hdr;
    if (mpv == NULL) {
        return 0;
    }
    n = count >> 2;
    max = 0x10;
    if (n <= 0x10) {
        max = n;
    }
    for (i = 0; i < max; i++) {
        MPV_GetCond(mpv, i, conds + i);
    }
    return max;
}

// ---------------------------------------------------------------------------
// SFMPV_RestoreCond
// ---------------------------------------------------------------------------
void SFMPV_RestoreCond(SfdHandle* h, s32* conds, s32 count) {
    void* mpv = *(void**)h->trn[2].hdr;
    if (mpv != NULL) {
        s32 i;
        for (i = 0; i < count; i++) {
            MPV_SetCond(mpv, i, conds[i]);
        }
    }
}

// ---------------------------------------------------------------------------
// SFD_SetMbCb
// ---------------------------------------------------------------------------
void MPV_SetMbCb(void* p, u32 a, u32 b, u32 c);
void SFD_SetMbCb(SfdHandle* self, u32 a, u32 b, u32 c) {
    void* arg1 = *(void**)self->trn[2].hdr;
    ((u32*)self)[0xd8c / 4] = a;
    ((u32*)self)[0xd88 / 4] = c;
    ((u32*)self)[0xd84 / 4] = b;
    MPV_SetMbCb(arg1, a, b, c);
}

// ---------------------------------------------------------------------------
// SFMPV_Init
// ---------------------------------------------------------------------------
s32 SFMPV_Init(void) {
    s32 r;
    if (MPV_SetErrFunc(0, sfmpv_ErrFn, 0) != 0) {
        return SFLIB_SetErr(0, 0xff000f0b);
    }
    r = MPV_Init(8, &sMpvWorkArea);
    if (r != 0) {
        return fn_803C34F8(0, ((u32)r + 0xfd0000 == 0xff05) ? 0xff000f13 : 0xff000f01);
    }
    SFMPVF_InitPool();
    sMpvInitCounter = 0;
    return 0;
}

// ---------------------------------------------------------------------------
// SFMPV_Finish
// ---------------------------------------------------------------------------
int SFMPV_Finish(void) {
    MPV_Finish();
    return 0;
}

// ---------------------------------------------------------------------------
// SFMPV_ExecServer
// ---------------------------------------------------------------------------
s32 SFMPV_ExecServer(SfdHandle* self) {
    s32 ret;
    s32 idx;
    s32 ch;
    s32 v1;
    s32 out;
    s32 v2;
    s32 v3;
    u32 writeCnt;
    u32 readCnt;
    void* sj;

    if (gpSfdCoreTraceCb != NULL) {
        sTraceSfmpvExecServer[3] = (u32)self;
        (*(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24))(gpSfdCoreTraceCb, &sTraceSfmpvExecServer[1]);
    }

    do {
    if (SFSET_GetCond(self, 5) == 0) {
        ret = 0;
        break;
    }
    if (SFBUF_GetTermFlg(self, self->trn[2].termIdx) == 1) {
        ret = 0;
        break;
    }
    if (SFSET_GetCond(self, 0x1c) != 0 && SFHDS_GetColType(self) != -1) {
        SFD_SetMpvCond(self, 5, 0);
    }
    if (self->errorState == 2) {
        sfmpv_ProcessAuxShc(self);
    }
    ret = 0;
    for (;;) {
        if (self->field_0x0070 != 0) break;
        if (self->field_0x0074 != 0) break;
        ret = sfmpv_GetActiveSize(self, &v1, &v2, &v3);
        if (ret != 0) break;
        ret = sfmpv_DecodeOneUnit(self, v1, v2, v3, &out);
        if (ret != 0) break;
        if (out == 0) break;
    }
    {
        SFBUF_RingGetSj(self, self->trn[2].bufIdx, &sj);
        SFBUF_GetFlowCnt(sj, &readCnt, &writeCnt);
        self->field_0x09b0 = SFBUF_UpdateFlowCnt(self->field_0x09b0, readCnt);
        ch = self->trn[2].termIdx;
        idx = self->trn[2].bufIdx;
        if (SFBUF_GetPrepFlg(self, ch) != 1 && SFBUF_GetPrepFlg(self, idx) == 1) {
            s32 r;
            if (SFMPVF_IsTermDec(self) != 0) {
                r = 1;
            } else {
                s32 n = self->field_0x0030;
                idx = self->cond[0x17];
                if (idx == -1) {
                    idx = n + 2;
                }
                if (n + 2 < idx) {
                    idx = n + 2;
                }
                s32 numFrm = SFMPVF_GetNumFrm(self);
                if (self->field_0x0068 == 2) {
                    numFrm += self->field_0x0978;
                }
                if (numFrm >= idx && sfmpv_IsVbvEnough(self) != 0) {
                    r = 1;
                } else {
                    r = 0;
                }
            }
            if (r != 0) {
                SFBUF_SetPrepFlg(self, ch, 1);
                if (self->tim.limit != 0x7fff0000 + 0xffff) {
                    self->tim.field_0x118[0] = 1;
                }
            }
        }
    }
    idx = SFMPVF_GetNumFrm(self);
    if (idx != -1) {
        s32 ended;
        if (SFMPVF_IsTermDec(self) != 0) {
            if (idx == 0) {
                ended = 1;
            } else if (self->field_0x0068 == 1 && idx == 1 &&
                       self->field_0x0978 > self->field_0x097c) {
                ended = 1;
            } else {
                ended = 0;
            }
        } else {
            ended = 0;
        }
        if (ended == 0)
            break;
    }
    SFBUF_SetTermFlg(self, self->trn[2].termIdx, 1);
    if (self->field_0x0960 == 0) {
        SFSET_SetCond(self, 5, 0);
    }
    } while (0);
    if (gpSfdCoreTraceCb != NULL) {
        sTraceSfmpvExecServer[0x74 / 4] = (u32)&self->field_0x09b0;
        sTraceSfmpvExecServer[0x80 / 4] = (u32)&self->field_0x09b8;
        sTraceSfmpvExecServer[0x8c / 4] = (u32)&self->field_0x09c0;
        (*(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24))(gpSfdCoreTraceCb, &sTraceSfmpvExecServer[0x6c / 4]);
    }
    return ret;
}

// ---------------------------------------------------------------------------
// sfmpv_ProcessAuxShc
// ---------------------------------------------------------------------------
void sfmpv_ProcessAuxShc(SfdHandle* self) {
    SfdMpvSession* e = self->trn[2].hdr;
    void* p = e->mpv;
    s32 buf[2];
    s32 c;
    buf[0] = (s32)self->field_0x0d90;
    buf[1] = (s32)self->field_0x0d94;
    if (buf[0] == 0 || buf[1] == 0)
        return;
    if (e->pattern != 0xc0)
        return;
    if (MPV_DecodePicAtr(p, buf, &c) == 0) {
        e->state = 2;
        e->pattern = 0xc8;
    }
}

// ---------------------------------------------------------------------------
// sfmpv_IsVbvEnough
// ---------------------------------------------------------------------------
s32 sfmpv_IsVbvEnough(SfdHandle* self) {
    SfdMpvSession* shc = self->trn[2].hdr;
    void* mpv = shc->mpv;
    u32 bitrate;
    s32 v;
    s32 i;
    if (SFBUF_GetTermFlg(self, self->trn[2].bufIdx) == 1) {
        return 1;
    }
    if (self->fhd.info.active != 0 && self->fhd.info.video[8] == 0) {
        return 1;
    }
    MPV_GetBitRate(mpv, &bitrate);
    if ((bitrate - 0x30000) == 0xffff) {
        return 1;
    }
    if (SFBUF_GetWTot(self, 1) >= shc->vbvLimit) {
        return 1;
    }
    i = SFTRN_IsSetup(self, 1) ? 0 : 1;
    v = SFBUF_GetRingBufSiz(self, i);
    if (SFBUF_GetWTot(self, i) >= v) {
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_GetActiveSize
// ---------------------------------------------------------------------------
s32 sfmpv_GetActiveSize(SfdHandle* self, s32* out1, s32* out2, s32* out3) {
    u32 info[7];  /* sp+0x18: ring read info */
    u32 dlm;      /* sp+0x14 */
    u32 dlm_size; /* sp+0x10 */
    u32 type;     /* sp+0x0c: delimiter type */
    u32 chk;      /* sp+0x08: BsearchDelim out / MPV_CheckDelim result */
    u32 r;
    s32 tmp = 0;
    s32 ch;

    ch = self->trn[2].bufIdx;
    *out1 = 0;
    *out2 = 0;
    *out3 = 0;
    r = SFBUF_RingGetRead(self, ch, info);
    if (r != 0) {
        return r;
    }
    if (info[1] == 0) {
        return 0;
    }
    r = sfmpv_SearchDelim(info, 0xce, &type);
    if (r != info[0]) {
        if (r != 0) {
            u32 v;
            if (info[0] <= r && r < info[0] + info[1]) {
                v = r - info[0];
            } else if (info[2] <= r && r < info[2] + info[3]) {
                v = info[1] + (r - info[2]);
            } else {
                v = 0;
            }
            *out1 = v;
        } else {
            s32 n = (s32)(info[1] + info[3] - 3);
            *out1 = n < 0 ? 0 : n;
        }
        if (*out1 > 0) {
            *out3 = 1;
        }
        return 0;
    }
    *out2 = type;
    *out1 = 4;
        if ((type & 0x80) != 0) {
            return 0;
        }
        SFBUF_RingGetDlm(self, ch, &dlm, &dlm_size);
        if (sfmpv_NeedSafeDlmRefresh(info, type, dlm) != 0) {
            u32 end;
            dlm = tmp;
            if (info[3] == 0) {
                end = info[0] + info[1];
            } else {
                end = info[2] + info[3];
            }
            if (dlm_size != end) {
                dlm_size = end;
                dlm = sfmpv_BsearchDelim(info, 0xcc, &chk);
                SFBUF_RingSetDlm(self, ch, dlm, dlm_size);
            }
        }
        if (dlm == 0) {
            tmp = self->trn[2].bufIdx;
            s32 siz = SFBUF_GetRingBufSiz(self, tmp);
            s32 n = siz - SFBUF_RingGetDataSiz(self, tmp);
            if (n < self->field_0x002c) {
                return SFLIB_SetErr(self, 0xff000f1c);
            }
            return 0;
        }
        chk = MPV_CheckDelim((u8*)dlm);
        switch ((s32)chk) {
        case 8:
            if ((type & 0x40) != 0) {
                u32 dr = sfmpv_SearchDelim(info, 8, &type);
                if (dr == 0 || dr == dlm) {
                    tmp = self->trn[2].bufIdx;
                    s32 n = SFBUF_GetRingBufSiz(self, tmp) - SFBUF_RingGetDataSiz(self, tmp);
                    if (n < self->field_0x002c) {
                        return SFLIB_SetErr(self, 0xff000f1c);
                    }
                    return 0;
                }
            }
            break;
        case 4:
            if ((type & 0x48) != 0) {
                u32 dr = sfmpv_SearchDelim(info, 4, &type);
                if (dr == 0 || dr == dlm) {
                    tmp = self->trn[2].bufIdx;
                    s32 n = SFBUF_GetRingBufSiz(self, tmp) - SFBUF_RingGetDataSiz(self, tmp);
                    if (n < self->field_0x002c) {
                        return SFLIB_SetErr(self, 0xff000f1c);
                    }
                    return 0;
                }
            }
        }
        {
            u32 val;
            if (info[0] <= dlm && dlm < info[0] + info[1]) {
                val = dlm - info[0];
            } else if (info[2] <= dlm && dlm < info[2] + info[3]) {
                val = info[1] + (dlm - info[2]);
            } else {
                val = 0;
            }
            *out1 = val;
        }
        return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_NeedSafeDlmRefresh
// ---------------------------------------------------------------------------
s32 sfmpv_NeedSafeDlmRefresh(u32* buf, u32 dlm, u32 dlm_new) {
    u8 tmp[4];
    u32 r;
    if (dlm_new == 0) {
        return 1;
    }
    if (dlm_new == buf[0]) {
        return 1;
    }
    if (dlm_new > buf[0] && (s32)(dlm_new - buf[0]) <= 3) {
        return 1;
    }
    if (dlm_new >= buf[0] && dlm_new < buf[0] + buf[1]) {
        /* first-half window: delimiter tail may wrap across the ring segments */
        s32 n = dlm_new + 4 - (buf[0] + buf[1]);
        if (n > 0) {
            if (n > (s32)buf[3]) {
                return 1;
            }
            memcpy(tmp, (void*)dlm_new, 4 - n);
            memcpy(tmp + 4 - n, (void*)buf[2], n);
        } else {
            memcpy(tmp, (void*)dlm_new, 4);
        }
    } else {
        /* second-half window */
        if (dlm_new >= buf[2] && dlm_new < buf[2] + buf[3]) {
            s32 n = dlm_new + 4 - (buf[2] + buf[3]);
            if (n > 0) {
                return 1;
            }
            memcpy(tmp, (void*)dlm_new, 4);
        } else {
            return 1;
        }
    }
    {
        s32 v = MPV_CheckDelim(tmp);
        if (v == 0x40 || v == 0x80) {
            /* no refresh needed */
        } else {
            switch (v) {
            case 8:
                if ((dlm & 0x40) != 0) {
                    r = sfmpv_SearchDelim(buf, 8, &dlm);
                    if (r == 0 || r == dlm_new) {
                        return 1;
                    }
                }
                break;
            case 4:
                if ((dlm & 0x48) != 0) {
                    r = sfmpv_SearchDelim(buf, 4, &dlm);
                    if (r == 0 || r == dlm_new) {
                        return 1;
                    }
                }
                break;
            default:
                return 1;
            }
        }
        return 0;
    }
}

// ---------------------------------------------------------------------------
// sfmpv_SearchDelim
// ---------------------------------------------------------------------------
u32 sfmpv_SearchDelim(u32* buf, u32 mask, u32* out) {
    u8* p;
    s32 bound;
    u32 r2;
    s32 i;
    s32 n1;
    s32 n2;
    u32 r;
    u8 tmp[8];
    *out = 0;
    r = MPV_SearchDelim((void*)buf[0], buf[1], mask);
    if (r != 0) {
        *out = MPV_CheckDelim((u8*)r);
        return r;
    }
    if ((s32)buf[3] == 0) {
        return 0;
    }
    n1 = (s32)buf[1];
    n1 = (n1 < 3) ? n1 : 3;
    n2 = (s32)buf[3];
    n2 = (n2 < 3) ? n2 : 3;
    memcpy(tmp, (u8*)buf[0] + (s32)buf[1] - n1, n1);
    memcpy(tmp + n1, (void*)buf[2], n2);
    bound = n1 + n2 - 3;
    p = tmp;
    for (i = 0; i < bound; i++) {
        u32 v = MPV_CheckDelim(p);
        if ((v & mask) != 0) {
            *out = v;
            {
                s32 off = buf[0] + (s32)buf[1] - n1;
                return off + i;
            }
        }
        p++;
    }
    r2 = MPV_SearchDelim((void*)buf[2], buf[3], mask);
    if (r2 != 0) {
        *out = MPV_CheckDelim((u8*)r2);
        return r2;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_BsearchDelim
// ---------------------------------------------------------------------------
u32 sfmpv_BsearchDelim(u32* buf, u32 mask, u32* out) {
    u8* p;
    s32 bound;
    u32 r2;
    s32 i;
    s32 n1;
    s32 n2;
    u32 r;
    u8 tmp[8];
    *out = 0;
    if ((s32)buf[3] != 0) {
        r = MPV_BsearchDelim((u8*)buf[2] + buf[3], buf[3], mask);
        if (r != 0) {
            *out = MPV_CheckDelim((u8*)r);
            return r;
        }
        n1 = (s32)buf[1];
        n1 = (n1 < 3) ? n1 : 3;
        n2 = (s32)buf[3];
        n2 = (n2 < 3) ? n2 : 3;
        memcpy(tmp, (u8*)buf[0] + (s32)buf[1] - n1, n1);
        memcpy(tmp + n1, (void*)buf[2], n2);
        bound = n1 + n2 - 3;
        p = tmp;
        for (i = 0; i < bound; i++) {
            u32 v = MPV_CheckDelim(p);
            if ((v & mask) != 0) {
                *out = v;
                {
                    s32 off = buf[0] + (s32)buf[1] - n1;
                    return off + i;
                }
            }
            p++;
        }
    }
    r2 = MPV_BsearchDelim((u8*)buf[0] + buf[1], buf[1], mask);
    if (r2 != 0) {
        *out = MPV_CheckDelim((u8*)r2);
        return r2;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_DecodeOneUnit
// ---------------------------------------------------------------------------
s32 sfmpv_DecodeOneUnit(SfdHandle* self, s32 ch, s32 pat, s32 dlm, s32* out) {
    void (*fn)(void*, void*);
    s32 out2;
    void* sj;
    void* p;
    SfdMpvSession* shc;
    SfdMpvSession* shc2;
    u32 d_vdo[2];
    u32 d_end[2];
    u32 pic[2];
    s32 ret;
    s32 c0;

    *out = 0;
    self->field_0x0988 = 0;
    shc = self->trn[2].hdr;
    if (shc->pattern != 0xcc || shc->field_0x0f8 != 0) {
        pat &= 0xcc;
    }
    ret = SFBUF_RingGetSj(self, self->trn[2].bufIdx, &sj);
    if (ret != 0) {
        return 0;
    }
    if ((pat & 0xc8) != 0) {
        SFMPVF_SetGopStat(self);
    }
    if (pat == 0x80) {
        if (self->trn[2].field_0x20 < 0) {
            self->trn[2].field_0x20 = SFBUF_GetRTot(self, self->trn[2].bufIdx) + 4;
        }
        if (self->tim.tcB.ttime < 0) {
            /* refresh the 11-word tc-save block at +0xe2c from +0xe00 */
            self->tim.tcB = self->tim.tcA;
        }
        if (SFCON_IsEndcodeSkip(self) != 0) {
            /* drain concat stream: pull 4-byte units until a non-GOP delimiter */
            if (sfmpv_ConcatSub(self) != -1) {
                p = sj;
                for (;;) {
                    void* v = *(void**)p;
                    ((void (*)(void*, int, int, u32*))((void**)v)[6])(p, 1, 4, d_end);
                    if (d_end[1] == 4 && MPV_CheckDelim((u8*)(u32)d_end[0]) == 0x80) {
                        ((void (*)(void*, int, u32*))((void**)v)[8])(p, 0, d_end);
                        SFBUF_AddRtotSj(self, self->trn[2].bufIdx, 4);
                        self->field_0x09b8 += 4;
                        self->field_0x0050 = 1;
                    } else {
                        ((void (*)(void*, int, u32*))((void**)v)[7])(p, 1, d_end);
                        break;
                    }
                }
                *out = 1;
            }
            goto done;
        }
        if (SFCON_IsVideoEndcodeSkip(self) != 0) {
            p = sj;
            for (;;) {
                void* v = *(void**)p;
                ((void (*)(void*, int, int, u32*))((void**)v)[6])(p, 1, 4, d_vdo);
                if (d_vdo[1] == 4 && MPV_CheckDelim((u8*)(u32)d_vdo[0]) == 0x80) {
                    ((void (*)(void*, int, u32*))((void**)v)[8])(p, 0, d_vdo);
                    SFBUF_AddRtotSj(self, self->trn[2].bufIdx, 4);
                    self->field_0x09b8 += 4;
                    self->field_0x0050 = 1;
                } else {
                    ((void (*)(void*, int, u32*))((void**)v)[7])(p, 1, d_vdo);
                    break;
                }
            }
            *out = 1;
            goto done;
        }
    }
    if (dlm == 0) {
        s32 r;
        if (pat == 0x80) {
            r = 1;
        } else if (ch <= 4 && SFBUF_GetTermFlg(self, self->trn[2].bufIdx) == 1) {
            r = 1;
        } else {
            r = 0;
        }
        if (r == 0) {
            /* terminate decode once either playback-limit condition holds */
            c0 = SFSET_GetCond(self, 0x4c);
            s32 c1 = SFSET_GetCond(self, 0x4d);
            shc2 = self->trn[2].hdr;
            if ((c0 != -1 && self->field_0x0960 >= c0) ||
                (c1 != -1 && shc2->pts > c1)) {
                r = 1;
            }
        }
        if (r != 0) {
            SFMPVF_TermDec(self);
            goto done;
        }
    }
    if (dlm == 0 && ch <= 4) {
        self->field_0x0988 = 1;
        if (gpSfdCoreTraceCb != NULL) {
            sTraceSfdWaitSupply[3] = (u32)self;
            fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
            fn(gpSfdCoreTraceCb, &sTraceSfdWaitSupply[1]);
        }
        goto done;
    }
        if ((pat & 0x4c) != 0) {
        u32 info[7];
        if (SFBUF_RingGetRead(self, self->trn[2].bufIdx, info) != 0) {
            pic[0] = 0;
            pic[1] = 0;
        } else {
            pic[0] = info[0];
            pic[1] = info[1];
        }
        ret = sfmpv_DecodePicAtr(self, pic, sj, pat, &out2);
        if (ret != 0) {
            return ret;
        }
        if (out2 == 0) {
            if ((pat & shc->pattern) != 0) {
                shc->pattern = 0xcc;
            }
            shc->field_0x0f8 = 1;
        }
        if (pat == 0x40 && out2 == -2) {
            shc->pattern = 0xc0;
        }
        *out = 1;
        goto done;
    }
    if ((pat & 2) != 0) {
        u32 info[7];
        if (SFBUF_RingGetRead(self, self->trn[2].bufIdx, info) != 0) {
            pic[0] = 0;
            pic[1] = 0;
        } else {
            pic[0] = info[0];
            pic[1] = info[1];
        }
        if (sfmpv_IsSkip(self, pic) != 0) {
            ret = sfmpv_SkipFrm(self, sj);
            if (ret == 0) {
                *out = 1;
            }
        } else {
            ret = sfmpv_DecodeFrm(self, sj);
        }
        goto done;
    }
    if (pat != 0x80) {
        if (sfmpv_GoDdelim(self, (u32)sj, 0xcc) > 0) {
            *out = 1;
        }
    }
done:
    return ret;
}

// ---------------------------------------------------------------------------
// sfmpv_ConcatSub
// ---------------------------------------------------------------------------
s32 sfmpv_ConcatSub(SfdHandle* self) {
    u8* shc;
    s32 dlm;
    shc = (u8*)self->trn[2].hdr;
    if (SFSET_GetCond(self, 6) == 0) {
        SfdTc* e00 = &self->tim.tcA;
        if (e00->flag == 0) {
            dlm = 0;
        } else {
            s32 tc[8];
            s32 t2;
            s32 t1;
            sfmpv_NextTc(&e00->field_0x04, tc);
            tc[6] = 0;
            SFTIM_Tc2Time(tc, &t1, &t2);
            dlm = t1 - (s32)self->tim.hdInfo.w[9];
        }
    } else {
        u32 p = *(u32*)self;
        s32 lastSmpl;
        s32 val;
        /* sample-rate mismatch: synthesize a 44100-based conversion rate */
        if (*(u32*)(p + 0xc) != (u32)&lbl_eu_8051C4E0) {
            lastSmpl = 0;
            val = 0x10000 - 0x53bc;
        } else if (SFCON_ReadTotSmplQue(self, &lastSmpl, &val) == 0) {
            dlm = -1;
            goto check;
        }
        self->tim.concat.field_0x90 += lastSmpl;
        if ((dlm = UTY_MulDiv(self->tim.concat.field_0x90, (s32)self->tim.hdInfo.w[10], val) - self->tim.concat.time) < 0) {
            dlm = 0;
        }
    check:
        if (dlm < 0) {
            return -1;
        }
    }
    if (dlm > 0) {
        SFCON_UpdateConcatTime(self, dlm);
        *(s32*)(shc + 0x10) += 1;
        if (gpSfdCoreTraceCb != NULL) {
            void (*fn)(void*, void*);
            sTraceSfmpvConcatSub[3] = (u32)self;
            sTraceSfmpvConcatSub[0x18 / 4] = (u32)(shc + 0x10);
            sTraceSfmpvConcatSub[0x24 / 4] = (u32)&dlm;
            sTraceSfmpvConcatSub[0x30 / 4] = (u32)&self->tim.concat.time;
            sTraceSfmpvConcatSub[0x3c / 4] = (u32)&self->tim.field_0x160;
            fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
            fn(gpSfdCoreTraceCb, &sTraceSfmpvConcatSub[1]);
        }
    }
    SFTIM_InitTtu(&self->tim.hdInfo, 0x7fffffff);
    SFTIM_InitTtu(&self->tim.tcA, -1);
    *(s32*)(shc + 8) = 0xc0;
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_DecodePicAtr
// ---------------------------------------------------------------------------
s32 sfmpv_DecodePicAtr(SfdHandle* self, u32* pic, void* sj, s32 pat, s32* out) {
    SfdMpvSession* shc;
    void* mpv;
    SfdMpvPicAtr* frm;
    s32 ret;
    s32 fc;
    s32 t1;
    s32 rate;
    s32 c0;
    s64 pts;
    s32 f0;
    s32 pts0;
    s32 pts1;
    u32 pque[4];
    u32 tc[8];

    shc = self->trn[2].hdr;
    mpv = shc->mpv;
    frm = &shc->picAtr;
    self->picUsr.field_0x10 = 0;
    MPV_SetPicUsrBuf(mpv, (void*)self->picUsr.curWidth, (void*)self->picUsr.stride);
    fc = SJRBF_GetFlowCnt(sj, 0, 1);
    if (gpSfdCoreTraceCb != NULL) {
        sTraceDecodePicAtr[3] = (u32)mpv;
        sTraceDecodePicAtr[0x18 / 4] = (u32)&fc;
        (*(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24))(gpSfdCoreTraceCb, &sTraceDecodePicAtr[1]);
    }
    *out = MPV_DecodePicAtrSj(mpv, sj);
    t1 = SJRBF_GetFlowCnt(sj, 0, 1) - fc;
    {
        s32 v = *out;
        if (v == 0) {
            ret = 0;
        } else if (v == -2) {
            if (t1 > 0) {
                ret = 0;
            } else {
                ret = SFLIB_SetErr(self, -2);
            }
        } else if (v == -3) {
            if (t1 > 0) {
                ret = 0;
            } else {
                ret = SFLIB_SetErr(self, -3);
            }
        } else {
            ret = SFLIB_SetErr(self, 0xff000f04);
        }
    }
    if (gpSfdCoreTraceCb != NULL) {
        sTraceDecodePicAtr[0x74 / 4] = (u32)&t1;
        sTraceDecodePicAtr[0x80 / 4] = (u32)out;
        sTraceDecodePicAtr[0x8c / 4] = (u32)&ret;
        (*(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24))(gpSfdCoreTraceCb, &sTraceDecodePicAtr[0x6c / 4]);
    }
    SFBUF_AddRtotSj(self, self->trn[2].bufIdx, t1);
    self->field_0x09b8 += t1;
    if (t1 > 0) {
        self->field_0x0050 = 1;
    }
    if (ret != 0) {
        return ret;
    }
    if (*out == -2) {
        return 0;
    }
    ret = MPV_GetPicAtr(mpv, frm);
    *out = ret;
    if (ret != 0) {
        return SFLIB_SetErr(self, 0xff000f05);
    }
    if ((pat & 0x40) != 0) {
        if (SFMPVF_ChkImageSize(self, frm->width, frm->height) != 0) {
            *out = -2;
            return 0;
        }
    }
    if (frm->type == 1) {
        shc->field_0x0fc = 0;
    } else if (self->field_0x0040 == 3) {
        SfdFrame* prev = shc->refPrev;
        if (prev != 0 && frm->type == 2) {
            if (frm->time < prev->picAtr.time && prev->picAtr.time < 0x200) {
                shc->field_0x0fc = 1;
            }
        } else if (prev != 0 && frm->type == 3) {
            if (frm->time >= prev->picAtr.time) {
                shc->field_0x0fc = 1;
            }
        }
    }
    MPV_GetPicUsr(mpv, 0, &self->picUsr.field_0x10);
    if (frm->field_0x30 != shc->pts) {
        shc->pts = frm->field_0x30;
        shc->repFld = 1;
    } else {
        shc->repFld = 0;
    }
    if ((pat & 0x40) != 0) {
        void* cb = self->delimCb;
        void* arg = self->delimCbArg;
        if (cb != NULL) {
            u32 r = MPV_SearchDelim((u8*)pic[0], pic[1], 1);
            if (r != 0) {
                /* cb is the callback fn; arg is its first parameter */
                ((void (*)(void*, void*, u32))cb)(arg, (void*)pic[0], r + 4 - pic[0]);
            }
        }
    }
    {
        u32 r = MPV_SearchDelim((u8*)pic[0], pic[1], 4);
        f0 = shc->repFld;
        pts0 = -1;
        pts1 = -1;
        if (r != 0) {
            s32 rate2;
            SFPTS_ReadPtsQue(self, self->trn[2].bufIdx, r, pque);
            rate2 = SFTIM_prate[frm->rateIndex];
            if (self->fhd.info.active != 0 && self->fhd.info.video[4] != -1) {
                rate2 = self->fhd.info.video[4];
            }
            pts = sfmpv_ComplementPts(&self->tim, shc->ptsBase, frm, pque, f0, &pts0, rate2);
        }
        shc->ptsLo = pts1;
        shc->ptsHi = pts0;
        if ((pat & shc->pattern) == 0) {
            return 0;
        }
    }
    sfmpv_CalcRepeatField(self, frm, shc->repFld);
    rate = shc->repFld;
    c0 = SFSET_GetCond(self, 0x34);
    if (c0 == 0) {
        s32 set = 0;
        if (pts >= 0 || frm->field_0x30 == 0 || frm->field_0x57 != 0) {
            set = 1;
        } else if (rate != 0 && self->tim.tcA.flag != 0) {
            s32 a;
            s32 b;
            s32 c;
            tc[0] = self->tim.tc[0];
            tc[1] = self->tim.tc[1];
            tc[2] = self->tim.tc[2];
            tc[3] = self->tim.tc[3];
            tc[4] = self->tim.tc[4];
            tc[5] = self->tim.tc[5];
            tc[6] = self->tim.tc[6];
            tc[7] = self->tim.tc[7];
            /* first call: out1->c, out2 shared; second: out1->a, out2->b.
             * set when c > b and c < b + a * cond(0x35) */
            SFTIM_Tc2Time(tc, &c, &b);
            SFTIM_Tc2Time(&self->tim.tcA.field_0x04, &a, &b);
            if (c > b && c < b + a * SFSET_GetCond(self, 0x35)) {
                set = 1;
            }
        }
        if (set) {
            SFSET_SetCond(self, 0x34, 1);
            c0 = 1;
        }
    }
    if (c0 == 1) {
        sfmpv_DoReformTc(self, frm, pts, rate);
    }
    if ((s32)self->tim.hdInfo.w[0] == 0) {
        s32 o1;
        s32 o2;
        tc[0] = self->tim.tc[0];
        tc[1] = self->tim.tc[1];
        tc[2] = self->tim.tc[2];
        tc[3] = self->tim.tc[3];
        tc[4] = self->tim.tc[4];
        tc[5] = self->tim.tc[5];
        tc[6] = 0;
        tc[7] = self->tim.tc[7];
        SFTIM_Tc2Time(tc, &o1, &o2);
        /* dd4 = valid flag, dd8..df4 = tc[0..7], df8/dfc = converted time.
         * Store order mirrors retail (pairs swapped). */
        self->tim.hdInfo.w[2] = tc[1];
        self->tim.hdInfo.w[1] = tc[0];
        self->tim.hdInfo.w[4] = tc[3];
        self->tim.hdInfo.w[3] = tc[2];
        self->tim.hdInfo.w[6] = tc[5];
        self->tim.hdInfo.w[5] = tc[4];
        self->tim.hdInfo.w[8] = tc[7];
        self->tim.hdInfo.w[7] = tc[6];
        self->tim.hdInfo.w[9] = o1;
        self->tim.hdInfo.w[10] = o2;
        self->tim.hdInfo.w[0] = 1;
    }
    {
        s32 o1;
        s32 o2;
        s32 delta;
        tc[0] = self->tim.tc[0];
        tc[1] = self->tim.tc[1];
        tc[2] = self->tim.tc[2];
        tc[3] = self->tim.tc[3];
        tc[4] = self->tim.tc[4];
        tc[5] = self->tim.tc[5];
        tc[6] = self->tim.tc[6];
        tc[7] = self->tim.tc[7];
        SFTIM_Tc2Time(tc, &o1, &o2);
        /* e58 = valid flag, e5c..e78 = tc[0..7], e7c/e80 = converted time */
        self->tim.tcC.hour = tc[1];
        self->tim.tcC.field_0x04 = tc[0];
        self->tim.tcC.second = tc[3];
        self->tim.tcC.minute = tc[2];
        self->tim.tcC.frame2 = tc[5];
        self->tim.tcC.frame = tc[4];
        self->tim.tcC.field_0x20 = tc[7];
        *(s32*)&self->tim.tcC.field_0x1c = tc[6];
        delta = o1 - (s32)self->tim.hdInfo.w[9];
        self->tim.tcC.ttime = delta;
        self->tim.tcC.ftime = o2;
        self->tim.tcC.flag = 1;
        if (self->tim.tcA.ttime <= delta) {
            self->tim.tcA.flag = 1;
            self->tim.tcA.field_0x04 = tc[0];
            self->tim.tcA.hour = tc[1];
            self->tim.tcA.minute = tc[2];
            self->tim.tcA.second = tc[3];
            self->tim.tcA.frame = tc[4];
            self->tim.tcA.frame2 = tc[5];
            *(s32*)&self->tim.tcA.field_0x1c = tc[6];
            self->tim.tcA.field_0x20 = tc[7];
            self->tim.tcA.ttime = delta;
            self->tim.tcA.ftime = o2;
        }
    }
    ret = sfmpv_FirstPicAtr(self, mpv, frm, pic);
    if (gpSfdCoreTraceCb != NULL) {
        sTraceDecodePicAtrRet[3] = (u32)&frm->type;
        sTraceDecodePicAtrRet[0x18 / 4] = (u32)&frm->field_0x30;
        sTraceDecodePicAtrRet[0x24 / 4] = (u32)&frm->field_0x68;
        sTraceDecodePicAtrRet[0x30 / 4] = (u32)&self->tim.tcC.ttime;
        (*(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24))(gpSfdCoreTraceCb, &sTraceDecodePicAtrRet[1]);
    }
    return ret;
}

// ---------------------------------------------------------------------------
// sfmpv_ComplementPts
// ---------------------------------------------------------------------------
s64 sfmpv_ComplementPts(SfdTim* self, u32* dst, SfdMpvPicAtr* frm, u32* pts, s32 mode, s32* out1, s32 rate) {
    s64 ptsv;
    s64 d;
    s32 frm14 = frm->time;

    /* negative accumulated PTS: re-seed it from the frame duration */
    if (*(s64*)&self->streamPosHi < 0) {
        ptsv = *(s64*)pts;
        if (ptsv < 0) {
            return -1;
        }
        {
            /* frame duration scaled back to PTS units */
            d = ptsv - ((s64)frm14 * 0x55d4a80) / rate;
            /* clamp negative delta to zero */
            if (d < 0) {
                d = 0;
            }
        }
        self->streamPosLo = (u32)d;
        self->streamPosHi = (u32)(d >> 32);
    }

    {
        ptsv = *(s64*)pts;
        if (ptsv >= 0 && memcmp(dst + 4, pts, 4) == 0) {
            /* same PTS as the saved stamp: restart the run */
            s64 p = ptsv - *(s64*)&self->streamPosHi;
            if (p < 0) {
                p = 0;
            }
            dst[4] = pts[0];
        dst[5] = pts[1];
        dst[6] = pts[2];
        dst[7] = pts[3];
        dst[1] = 0;
        dst[0] = frm14;
        dst[2] = (frm->type == 3) ? 1 : 0;
            out1[0] = pts[0];
            out1[1] = pts[1];
            return p;
        } else {
            /* continuation: advance by the frame count scaled back to PTS units */
            s64 p = *(s64*)(dst + 4) - *(s64*)&self->streamPosHi;
        p = (p < 0) ? 0 : p;
        if (mode != 0) {
            dst[0] = 0;
            dst[1] = dst[2] + dst[1] + 1;
            dst[2] = 0;
        }
        {
            s32 w = frm14 - dst[0];
            dst[2] = (dst[2] <= w) ? w : dst[2];
            p += ((s64)(dst[1] + w) * 0x55d4a80) / rate;
        }
        p = (p < 0) ? 0 : p;
            return p;
        }
    }
}

// ---------------------------------------------------------------------------
// sfmpv_CalcRepeatField
// ---------------------------------------------------------------------------
void sfmpv_CalcRepeatField(SfdHandle* self, SfdMpvPicAtr* frm, s32 mode) {
    s16* tbl = self->tim.table;
    s32 t1;
    s32 t2;
    s32 i;
    s32 w;
    s32 h;
    s32 n;
    s32 sv;
    s16* slot;
    SfdFrame* p;
    SfdMpvSession* shc;
    self->tim.tc[0] = frm->rateIndex;
    shc = self->trn[2].hdr;
    self->tim.tc[1] = frm->field_0x1c;
    self->tim.tc[2] = frm->field_0x20;
    self->tim.tc[3] = frm->field_0x24;
    self->tim.tc[4] = frm->field_0x28;
    self->tim.tc[5] = frm->field_0x2c;
    self->tim.tc[6] = frm->time;
    *(s16*)&self->tim.tc[7] = frm->field_0x54;
    ((s16*)&self->tim.tc[7])[1] = 0;
    if (mode != 0) {
        for (i = 0; i < 0x80; i++) {
            tbl[i] = -1;
        }
        tbl[1] = -1;
        return;
    }
    if ((u32)(frm->type - 1) <= 1) {
        p = shc->refPrev;
        if (p != NULL) {
            w = frm->time;
            h = p->picAtr.time;
            if (w < h) {
                w += 0x400;
            }
            for (n = h + 1; n < w; n++) {
                tbl[(n % 64) * 2] = -1;
            }
        }
    }
    slot = &tbl[(frm->time % 64) * 2];
    slot[0] = *(s16*)&self->tim.tc[7];
    if (mode != 0) {
        slot[1] = 0;
    } else if (frm->time == 0) {
        if (tbl[1] == -1) {
            tbl[1] = 0;
        }
    } else {
        /* walk back up to 16 slots; on failure the slot is left untouched */
        for (i = 0; i < 0x10; i++) {
            s16* q = &tbl[((frm->time - i + 0x3f) % 64) * 2];
            if (q[0] != -1) {
                slot[1] = q[0] + q[1];
                break;
            }
        }
    }
    ((s16*)&self->tim.tc[7])[1] = slot[1];
    if (frm->type == 3) {
        if (slot[0] != 0) {
            if (shc->refPrev != NULL) {
                w = shc->refPrev->picAtr.time;
                sv = slot[0] + slot[1];
                tbl[(w % 64) * 2 + 1] = sv;
                /* NOTE: deliberately re-reads the prev-frame pointer around the
                 * time conversion to match retail load placement */
                p = shc->refPrev;
                p->ttu.field_0x22 = sv;
                p = shc->refPrev;
                SFTIM_Tc2Time(&p->ttu.field_0x04, &t1, &t2);
                p = shc->refPrev;
                p->ttu.field_0x24 = t1 - (s32)self->tim.hdInfo.w[9];
                p->ttu.field_0x28 = t2;
                p->ttu.field_0x00 = 1;
                if (self->tim.tcA.ttime <= p->ttu.field_0x24) {
                    self->tim.tcA.flag = p->ttu.field_0x00;
                    self->tim.tcA.field_0x04 = p->ttu.field_0x04;
                    self->tim.tcA.hour = p->ttu.field_0x08;
                    self->tim.tcA.minute = p->ttu.field_0x0c;
                    self->tim.tcA.second = p->ttu.field_0x10;
                    self->tim.tcA.frame = p->ttu.field_0x14;
                    self->tim.tcA.frame2 = p->ttu.field_0x18;
                    *(s32*)&self->tim.tcA.field_0x1c = p->ttu.field_0x1c;
                    *(s32*)&self->tim.tcA.field_0x20 = *(s32*)&p->ttu.field_0x20;
                    self->tim.tcA.ttime = p->ttu.field_0x24;
                    self->tim.tcA.ftime = p->ttu.field_0x28;
                }
                p = shc->refPrev;
                p->timeUnit = p->ttu.field_0x28;
                n = self->tim.concat.time + (p->ttu.field_0x24 - self->tim.limit);
                p->time = n;
                p->field_0x58 = p->ttu.field_0x24;
                p->field_0x5c = p->ttu.field_0x24 + self->tim.concat.time;
                if (self->tim.maxTime < n) {
                    self->tim.maxTime = n;
                    self->tim.maxTimeUnit = p->timeUnit;
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// sfmpv_DoReformTc
// ---------------------------------------------------------------------------
void sfmpv_DoReformTc(SfdHandle* self, SfdMpvPicAtr* frm, s64 pts, s32 rep) {
    SfdTim* tc = &self->tim;
    s32 v;
    s32 t;
    s32 u;
    s16 sv;
    v = frm->rateIndex;
    t = frm->field_0x1c;
    u = frm->time;
    if (pts >= 0) {
        sfmpv_Pts2Tc(pts, v, t, u, (u32*)tc->tc);
    } else if (tc->tcA.flag == 0) {
        if ((s32)self->see.avplay == 0) {
            tc->tc[0] = (u32)v;
            tc->tc[1] = 0;
            tc->tc[2] = 0;
            tc->tc[3] = 0;
            tc->tc[4] = 0;
            tc->tc[5] = 0;
        } else {
            return;
        }
    } else if (rep != 0) {
        sfmpv_NextTc(&tc->tcA.field_0x04, tc->tc);
        sv = ((s16*)tc->tc)[0x0f];
        tc->table[1] = sv;
        tc->table[(u % 64) * 2 + 1] = sv;
    } else {
        tc->tc[0] = (u32)tc->tcA.field_0x04;
        tc->tc[1] = (u32)tc->tcA.hour;
        tc->tc[2] = (u32)tc->tcA.minute;
        tc->tc[3] = (u32)tc->tcA.second;
        tc->tc[4] = (u32)tc->tcA.frame;
        tc->tc[5] = (u32)tc->tcA.frame2;
    }
}

// ---------------------------------------------------------------------------
// sfmpv_Pts2Tc
// ---------------------------------------------------------------------------
// Output timecode record written by sfmpv_Pts2Tc (view over the tc block).
typedef struct SFMPVPts2TcOut {
    u32 fld_0x00;
    u32 fld_0x04;
    u32 fld_0x08;
    u32 fld_0x0c;
    s32 fld_0x10;
    s32 fld_0x14;
    u8 pad_0x18[0x1E - 0x18];
    s16 fld_0x1E;
} SFMPVPts2TcOut;

// pts -> timecode. Multiplies by twice the per-v rate, then splits the half-
// second count either through a drop-frame correction table (rate 0x7512 or
// low-half 0xea24) or straight base-60 decomposition.
void sfmpv_Pts2Tc(s64 a, s32 v, s32 t, s32 u, u32* out) {
    SFMPVPts2TcOut* tc = (SFMPVPts2TcOut*)out;
    const s32* rateTbl = SFTIM_prate;
    const s32* p;
    s32 v0;
    s32 t0;
    s32 d;
    s32 m;
    s32 q0;
    s32 q1;
    s32 q2;
    s32 rem;
    s32 acc;
    s64 prod;

    v0 = rateTbl[v];
    t0 = sfmpv_fps_round[v];
    prod = UTY_MulDivRound64(a, (s64)(v0 * 2), 0x55d4a80);
    m = (s32)prod;
    tc->fld_0x1E = (s16)(m & 1);
    tc->fld_0x00 = (u32)v;
    tc->fld_0x04 = (u32)t;
    /* elapsed half-frames minus offset, clamped at zero */
    d = (m >> 1) - u;
    if (d < 0) {
        d = 0;
    }
    if (t != 0 && (v0 == 0x7512 || (v0 & 0xFFFF) == 0xEA24)) {
        /* drop-frame path: correct via table pairs */
        s32 w0;
        s32 w1;
        s32 lim;
        s32 w3;
        s32 w4;
        s32 w5;
        s32 w6;
        s32 w7;
        s32 rem0;
        s32 x;
        s32 y;
        if (v0 == 0x7512) {
            p = sfmpv_conv_29_97;
        } else {
            p = sfmpv_conv_59_94;
        }
        w0 = p[0];
        w1 = p[1];
        q0 = d / w0;
        rem0 = d % w0;
        q1 = rem0 / w1;
        rem = rem0 % w1;
        lim = p[2];
        if (rem >= lim) {
            /* correction window: rem deliberately goes negative here */
            x = rem - lim;
            w3 = p[3];
            q2 = x / w3;
            acc = q2 + 1;
            x = x % w3;
            w4 = p[4];
            if (x >= w4) {
                y = x - w4;
                w5 = p[5];
                q2 = y / w5 + 1;
                rem = y % w5;
            } else {
                w7 = p[7];
                rem = x + w7;
                q2 = 0;
            }
        } else {
            w5 = p[5];
            q2 = rem / lim;
            rem = rem - q2 * w5;
            acc = 0;
        }
        w6 = p[6];
        tc->fld_0x08 = (u32)q0;
        tc->fld_0x0c = (u32)(w6 * q1 + acc);
        tc->fld_0x10 = q2;
        tc->fld_0x14 = rem;
    } else {
        /* generic base-60 decomposition */
        s32 mm = d / t0;
        s64 hr = (s64)(mm / 60 / 60);
        tc->fld_0x08 = (u32)((u64)hr >> 32);
        tc->fld_0x0c = (u32)hr;
        tc->fld_0x10 = (u32)(mm % 60);
        tc->fld_0x14 = (u32)(d % t0);
    }
}

// ---------------------------------------------------------------------------
// sfmpv_NextTc
// ---------------------------------------------------------------------------
void sfmpv_NextTc(s32* in, s32* out) {
    s32 t8 = (s32)((s16*)in)[0x0f];
    s32 t7 = (s32)((s16*)in)[0x0e];
    s32 t0 = in[0];
    s32 sum = t7 + t8;
    s32 t5 = in[5];
    s32 t6 = in[6];
    s32 rate = sfmpv_fps_round[t0];
    s32 a = t5 + t6 + 1 + sum / 2;
    s32 s = sum % 2;
    s32 t1 = in[1];
    s32 d = a / rate;
    s32 e = in[4] + d;
    s32 f = in[3] + e / 60;
    s32 q2 = f / 60;
    s32 t2 = in[2];
    s32 t3rem = a - d * rate;
    s32 frem = f % 60;
    s32 erem = e % 60;
    /* drop-frame guard: branch chain skips the +2 when any carry term hits */
    if (t1 != 0 && erem == 0 && frem % 10 != 0 && (u32)t3rem <= 1) {
        t3rem = 2;
    }
    out[0] = t0;
    out[1] = in[1];
    out[2] = t2 + q2;
    out[3] = frem;
    out[4] = erem;
    out[5] = t3rem;
    ((s16*)out)[0x0f] = (s16)s;
}

// ---------------------------------------------------------------------------
// sfmpv_FirstPicAtr
// ---------------------------------------------------------------------------
// NOTE: residual 5-instruction rotation vs retail (addi r30 dst before the four
// mr param copies in the prologue; decomp emits the copies first). Exact size /
// zero reloc drift; resists declaration/assignment order, C99 mixed decls, temp
// locals. br/vb temps below are required to reproduce retail's load hoisting
// (lwz vbv/bitrate + ChkBufSiz arg setup before the 5-word copy loop).
s32 sfmpv_FirstPicAtr(SfdHandle* self, void* mpv, SfdMpvPicAtr* frm, u32* pic) {
    SfdMpvSession* shc = self->trn[2].hdr;
    u32* dst = (u32*)&self->imageWidth;
    s32 bitrate;
    s32 vbv;
    s32 avg;
    s32 max;
    if (dst[4] != 0) {
        return 0;
    }
    if (MPV_GetBitRate(mpv, (u32*)&bitrate) != 0) {
        return SFLIB_SetErr(self, 0xff000f16);
    }
    MPV_GetVbvBufSiz(mpv, (u32*)&vbv, (u32*)&avg, (u32*)&max);
    if (SFSET_GetCond(self, 0x3c) == 0) {
        shc->vbvLimit = 0;
    } else {
        s32 ring = (s32)SFBUF_GetRingBufSiz(self, 1);
        if (max == -1) {
            max = vbv;
        }
        shc->vbvLimit = (max < ring) ? max : ring;
    }
    sfmpv_SetMpvHd(self, bitrate, pic);
    {
        s32 br;
        s32 vb;
        vb = vbv;
        br = bitrate;
        dst[0] = frm->width;
        dst[1] = frm->height;
        dst[2] = frm->field_0x08;
        dst[3] = frm->field_0x0c;
        dst[5] = frm->rateIndex;
        dst[4] = (u32)br;
        dst[8] = (u32)vb;
        return sfmpv_ChkBufSiz(self, dst);
    }
}

// ---------------------------------------------------------------------------
// sfmpv_SetMpvHd
// ---------------------------------------------------------------------------
// OPEN ITEM (fixed-codegen stall, 33 mismatch / 26 structural / 7 reg_swap,
// size exact, reloc drift 0): MWCC if-converts the header-slot diamond -
// decomp emits `addi r31,r6,0xad0` speculatively ABOVE the shc+0x10 compare
// (ble joins past the `li r31,0` override), while retail keeps the addi in a
// forward branch-target block after `li r31,0; b`. Invariant across 6 source
// shapes: else-if chain, inverted-polarity nested if, ternary (flat and inner
// select), p=NULL-init nested if (300B variant), goto form, u32* pic typing,
// decl-order permutations - all byte-identical output. Downstream clamp
// coloring (pic[4] -> r3 vs retail r4) and MEM_Copy arg setup follow the same
// shift. Not retried here: unit-level flag changes (-O4,s etc.) - other fns
// in this TU match under Wii/1.1 defaults.
/* 0x2c-byte picture-header info block (SfdMpvHdInfo, see sfd_types.h) is
 * copied verbatim into the header slot. */

void sfmpv_SetMpvHd(SfdHandle* self, s32 bitrate, u32* pic) {
    void* v;
    SfdMpvSession* shc;
    SfdAvpHdCopy* p;
    u8* dst;
    s32 sz;
    s32 n;

    v = self->see.avplay;
    shc = self->trn[2].hdr;
    /* header slot: usable only when a decode session exists and the frame
     * count is still 0 */
    if (v == 0) {
        p = NULL;
    } else if (shc->frmSeq > 0) {
        p = NULL;
    } else {
        p = &self->see.avplay->audHd;
    }
    if (p != NULL && p->state == 0) {
        dst = p->image;
        sz = (s32)pic[1];
        n = (sz < 0x200) ? sz : 0x200;
        *(u32*)(dst + 0x200) = n;
        MEM_Copy(dst, (void*)pic[0], n);
        if ((u32)(bitrate - 0x30000) == 0xffff) {
            p->byteRate = 0;
            p->timeUnit = 0;
        } else {
            p->byteRate = bitrate * 0x32;
            p->timeUnit = 1;
        }
        p->hdInfo = self->tim.hdInfo;
        p->state = 1;
    }
}

// ---------------------------------------------------------------------------
// sfmpv_ChkBufSiz
// ---------------------------------------------------------------------------
// Validates/recomputes the frame-memory layout for the requested picture size.
// Macroblock dims come from self+0x2788/0x278c, the new request from para.
// size1/size2 are per-frame luma/chroma byte budgets; the search loop finds how
// many frames of the new budget fit into n old budgets (capped at 16).
s32 sfmpv_ChkBufSiz(SfdHandle* self, void* para) {
    s32 a = (self->para.para[2] + 15) / 16;
    /* frame-object array base, kept live across the whole call */
    SfdFrame* frm = self->frames;
    s32 b = (self->para.para[3] + 15) / 16;
    s32 va = a * 16;
    s32 vb = b * 16;
    s32 ta = (a >> 4) & 1;
    s32 tb = (b >> 4) & 1;
    s32 sb = (vb + tb) >> 1;
    s32 n = self->para.para[7];
    s32 sa = (va + ta) >> 1;
    SfdMpvSession* shc = self->trn[2].hdr;
    s32 ca = (sa + 31) / 32;
    s32 cwa = va / 32;
    s32 c = (*(s32*)para + 15) / 16;
    s32 d = (*((s32*)para + 1) + 15) / 16;
    s32 vp = c * 16;
    s32 vq = d * 16;
    s32 tc = (c >> 4) & 1;
    s32 tq = (d >> 4) & 1;
    s32 sc = (vp + tc) >> 1;
    s32 sq = (vq + tq) >> 1;
    s32 cc = (sc + 31) / 32;
    s32 cvp = vp / 32;
    /* per-frame byte budgets for the old (1) and new (2) layouts */
    s32 size1 = sb * (ca * 32) * 2 + vb * (cwa * 32) + 0x20;
    s32 size2 = sq * (cc * 32) * 2 + vq * (cvp * 32) + 0x20;
    s32 cnt;
    s32 acc;
    s32 lim;
    s32 i;
    s32 m1;
    s32 m2;
    u32 base;
    u32 v;
    u32 e0;

    if (size2 * 2 > size1 * 2) {
        return SFLIB_SetErr(self, 0xff000f17);
    }
    base = self->para.para[8];
    if (base == 0) {
        cnt = n;
    } else {
        lim = n * size1;
        acc = size2;
        /* how many whole new-budget frames fit in n old ones (cap 16) */
        for (cnt = 0; cnt < 16; cnt++) {
            if (acc > lim) {
                break;
            }
            acc += size2;
        }
        if (cnt < n) {
            return SFLIB_SetErr(self, 0xff000f17);
        }
        self->para.stride[0] = self->para.para[4];
        self->para.stride[1] = self->para.para[4] + size2;
        if (cnt > 0) {
            u32* q = (u32*)self->para.frameSize;
            for (i = 0; i < cnt; i++) {
                q[i] = base + i * size2;
            }
        }
    }
    {
        s32 ww = (vp + 31) / 32;
        s32 half = vp / 2;
        s32 hh = (half + 31) / 32;
        v = self->para.stride[0];
        m1 = vq * (ww * 32);
        m2 = (vq / 2) * (hh * 32);
        shc->plane[0].lineSiz = ww * 32;
        shc->plane[0].uvLineSiz = hh * 32;
        shc->plane[0].base = v;
        shc->plane[0].field_0x00 = v + m1;
        shc->plane[0].field_0x04 = v + m1 + m2;
        shc->plane[1].lineSiz = ww * 32;
        shc->plane[1].uvLineSiz = hh * 32;
        e0 = self->para.stride[1];
        shc->plane[1].base = e0;
        shc->plane[1].field_0x00 = e0 + m1;
        shc->plane[1].field_0x04 = e0 + m1 + m2;
    }
    if (self->field_0x0040 == 3) {
        s32 m = 14;
        if (n < 14) {
            m = n;
        }
        self->frameCount = m + 2;
        sfmpv_InitFrmObj(frm, (u32*)self->para.stride, 2);
        sfmpv_InitFrmObj(frm + 2, (u32*)self->para.frameSize, m);
    } else {
        s32 m = 16;
        if (n < 16) {
            m = n;
        }
        self->frameCount = m;
        sfmpv_InitFrmObj(frm, (u32*)self->para.frameSize, m);
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_IsSkip
// ---------------------------------------------------------------------------
s32 sfmpv_IsSkip(SfdHandle* self, void* bpic) {
    /* Single work slot: holds the shc pointer, then the pic type, then the verdict.
       Retail keeps all three in one register (r29). */
    SfdMpvSession* shc = self->trn[2].hdr;
    SfdMpvPicAtr* frm = &shc->picAtr;
    void* wk;
    u32 skip;
    wk = shc;
    if (SFSET_GetCond(self, 0x2f) == 1) {
        return 1;
    }
    /* the seek-skip call's own result is ignored; only the count matters */
    SFSEEKI_IsSkip(self, &skip);
    if (skip != 0) {
        return 1;
    }
    if (SFSET_GetCond(self, 0x27) == 1) {
        return 0;
    }
    if (frm->field_0x58) {
        return shc->field_0x0f4;
    }
    /* deadline check: current tc valid, no limiter active, and tc not past deadline */
    {
        s32 tc = self->see.seekTime;
        s32 r = 0;
        wk = (void*)(uintptr_t)frm->type;
        if (tc >= 0 && self->tim.field_0x118[0] == 0 &&
            UTY_CmpTime(tc, self->see.seekTimeUnit, self->tim.tcC.ttime, self->tim.tcC.ftime) == 0) {
            r = 1;
        }
        if (r != 0) {
            void (*fn)(void*, void*);
            if (gpSfdCoreTraceCb != NULL) {
                sTraceSfdIsSkip.entry.args[0].value = self;
                sTraceSfdIsSkip.entry.args[1].value = "before seek time";
                fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
                fn(gpSfdCoreTraceCb, &sTraceSfdIsSkip.entry);
            }
            wk = (void*)1;
            goto out;
        }
    }
    /* unknown picture types are treated as skippable */
    {
        s32 cond;
        switch ((s32)wk) {
        case 1:
            cond = (self->cond[2] == 0);
            break;
        case 2:
            cond = (self->cond[3] == 0);
            break;
        case 3:
            cond = (self->cond[4] == 0);
            break;
        default:
            cond = 1;
            break;
        }
        if (cond) {
            void (*fn)(void*, void*);
            if (gpSfdCoreTraceCb != NULL) {
                sTraceSfdIsSkip.entry.args[0].value = self;
                sTraceSfdIsSkip.entry.args[1].value = "ptype";
                fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
                fn(gpSfdCoreTraceCb, &sTraceSfdIsSkip.entry);
            }
            wk = (void*)1;
            goto out;
        }
    }
    if (sfmpv_IsEmptyBpic(self, (s32)wk, bpic) != 0) {
        void (*fn)(void*, void*);
        if (gpSfdCoreTraceCb != NULL) {
            sTraceSfdIsSkip.entry.args[0].value = self;
            sTraceSfdIsSkip.entry.args[1].value = "empty B";
            fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
            fn(gpSfdCoreTraceCb, &sTraceSfdIsSkip.entry);
        }
        wk = (void*)1;
        goto out;
    }
    {
        s32 t = shc->state;
        s32 flg = 0;
        if (t == 2) {
            if ((u32)((s32)wk - 2) <= 1) {
                flg = 1;
            }
        } else if (t == 3) {
            if ((s32)wk == 3) {
                flg = 1;
            }
        }
        if (flg != 0) {
            void (*fn)(void*, void*);
            if (gpSfdCoreTraceCb != NULL) {
                sTraceSfdIsSkip.entry.args[0].value = self;
                sTraceSfdIsSkip.entry.args[1].value = "defect";
                fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
                fn(gpSfdCoreTraceCb, &sTraceSfdIsSkip.entry);
            }
            wk = (void*)1;
            goto out;
        }
        if (sfmpv_IsLate(self, (s32)wk) != 0) {
            void (*fn)(void*, void*);
            if (gpSfdCoreTraceCb != NULL) {
                sTraceSfdIsSkip.entry.args[0].value = self;
                sTraceSfdIsSkip.entry.args[1].value = "late";
                fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
                fn(gpSfdCoreTraceCb, &sTraceSfdIsSkip.entry);
            }
            wk = (void*)1;
            goto out;
        }
        wk = (void*)0;
    }
out:
    sfmpv_UpdateDefect(self, frm, (s32)wk);
    return (s32)wk;
}

// ---------------------------------------------------------------------------
// sfmpv_UpdateDefect
// ---------------------------------------------------------------------------
void sfmpv_UpdateDefect(SfdHandle* self, SfdMpvPicAtr* frm, s32 mode) {
    SfdMpvSession* shc = self->trn[2].hdr;
    void* mpv = shc->mpv;
    s32 flg = shc->state;
    if (shc->repFld != 0) {
        s32 a;
        s32 b;
        MPV_GetLinkFlg(mpv, (u32*)&a, (u32*)&b);
        if (a == 1) {
            flg = 5;
        } else {
            if (self->tim.field_0x118[0] == 0 && SFSET_GetCond(self, 0x49) == 1) {
                b = 1;
            }
            if (b == 1) {
                flg = 2;
            }
        }
    }
    if (mode == 1) {
        u32 v = (u32)frm->type;
        if (v - 1 <= 1) {
            flg = 2;
        }
    }
    shc->state = flg;
}

// ---------------------------------------------------------------------------
// sfmpv_IsEmptyBpic
// ---------------------------------------------------------------------------
s32 sfmpv_IsEmptyBpic(SfdHandle* self, s32 type, void* bpic) {
    if (SFSET_GetCond(self, 7) != 0) {
        return 0;
    }
    if (type == 3) {
        s32 r = MPV_IsEmptyBpic(*(u32*)bpic, *(u32*)((u8*)bpic + 4),
                                self->field_0x0924 * self->field_0x0928);
        if (r != 0) {
            self->field_0x0970 += 1;
        }
        return r;
    }
    if (type == 2) {
        s32 r = MPV_IsEmptyPpic(*(u32*)bpic, *(u32*)((u8*)bpic + 4),
                                self->field_0x0924 * self->field_0x0928);
        if (r != 0) {
            self->field_0x0974 += 1;
        }
        return r;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_IsLate
// ---------------------------------------------------------------------------
s32 sfmpv_IsLate(SfdHandle* self, s32 type) {
    s32 t;
    s32 e8v;
    SfdTim* tc;
    SfdMpvSession* shc = self->trn[2].hdr;
    void* fn;
    s32 r;
    s32 c;
    s32 d;
    s32 a;
    s32 b;
    tc = &self->tim;
    if (tc->field_0x118[0] == 0) {
        t = 0;
    } else {
        t = (tc->tcC.ttime - tc->limit) + tc->concat.time;
    }
    fn = tc->field_0x018;
    e8v = tc->tcC.ftime;
    if (fn != 0) {
        return ((s32 (*)(void*, s32, s32, s32))fn)(self, type, t, e8v);
    }
    if (type == 1) {
        SFTIM_UpdateItime(tc, t);
    }
    if ((u32)(type - 1) <= 1) {
        t = SFTIM_GetNextItime(tc, t);
    }
    SFTIM_GetSpeed(self, &a, &b);
    if (a <= b && shc->field_0x00c >= (s32)self->cond[38]) {
        return 0;
    }
    SFTIM_GetTime(self, &c, &d);
    if (c < 0) {
        return 0;
    }
    r = UTY_CmpTime(c, d, t - (e8v * (s32)self->cond[42]) / (s32)self->cond[43], e8v);
    if (r != 0) {
        return 0;
    }
    shc->field_0x00c += 1;
    return 1;
}

// ---------------------------------------------------------------------------
// sfmpv_SkipFrm
// ---------------------------------------------------------------------------
s32 sfmpv_SkipFrm(SfdHandle* self, void* sj) {
    /* tcC/tcD hold identical 11-word tc-save blocks */
    SfdMpvSession* shc;
    void* mpv;
    s32 cur;
    s32 lim;
    SfdTc* dst;
    SfdTc* src;
    shc = self->trn[2].hdr;
    mpv = shc->mpv;
    cur = self->tim.tcC.ttime;
    lim = self->tim.limit;
    dst = &self->tim.tcD;
    src = &self->tim.tcC;
    u32 a;
    u32 b;
    s32 r;
    s32 n;
    if (cur < lim) {
        /* staged save: retail hoists every load above every store */
        dst->flag = src->flag;
        dst->field_0x04 = src->field_0x04;
        dst->hour = src->hour;
        dst->minute = src->minute;
        dst->second = src->second;
        dst->frame = src->frame;
        dst->frame2 = src->frame2;
        *(s32*)&dst->field_0x1c = *(s32*)&src->field_0x1c;
        dst->field_0x20 = src->field_0x20;
        dst->ttime = cur;
        dst->ftime = src->ftime;
    }
    a = SJRBF_GetFlowCnt(sj, 0, 1);
    r = MPV_SkipFrmSj(mpv, sj);
    b = SJRBF_GetFlowCnt(sj, 0, 1);
    n = b - a;
    if (r == 0) {
        r = 0;
    } else if (r == -2) {
        if (n > 0) {
            r = 0;
        } else {
            r = SFLIB_SetErr(self, -2);
        }
    } else if (r == -3) {
        if (n > 0) {
            r = 0;
        } else {
            r = SFLIB_SetErr(self, -3);
        }
    } else {
        r = SFLIB_SetErr(self, 0xff000f07);
    }
    SFBUF_AddRtotSj(self, self->trn[2].bufIdx, n);
    self->field_0x09b8 += n;
    if (n > 0) {
        self->field_0x0050 = 1;
    }
    if (r != 0) {
        return r;
    }
    if (shc->picAtr.field_0x58 == 0) {
        shc->field_0x0f4 = 1;
    }
    SFPLY_AddSkipPic(self, 1, shc->picAtr.type);
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_DecodeFrm
// ---------------------------------------------------------------------------
s32 sfmpv_DecodeFrm(SfdHandle* self, void* sj) {
    void (*fn)(void*, void*);
    SfdMpvSession* shc = self->trn[2].hdr;
    void* mpv = shc->mpv;
    SfdMpvPicAtr* frm = &shc->picAtr;
    SfdFrame* out;
    SfdMpvFrmPara fcnt;
    s32 r;
    s32 n;
    s32 ret = 0;
    s32 a;
    s32 b;
    s64 t1;
    s64 t2;
    if (sfmpv_SetFrmPara(self, frm, &fcnt, &out) != 0) {
        return 0;
    }
    fn_803C9948(self, shc, &a, &b);
    {
        u32* band = (u32*)out->field_0x60;
        memcpy((void*)band[0], (void*)self->picUsr.curWidth, self->picUsr.field_0x10);
        band[1] = self->picUsr.field_0x10;
    }
    fn_803C99C8(self);
    t1 = SFTMR_GetTmr(self);
    a = SJRBF_GetFlowCnt(sj, 0, 1);
    if (gpSfdCoreTraceCb != NULL) {
        sTraceDecodeFrm[3] = (u32)mpv;
        sTraceDecodeFrm[0x18 / 4] = (u32)&frm->type;
        sTraceDecodeFrm[0x24 / 4] = (u32)&frm->field_0x30;
        sTraceDecodeFrm[0x30 / 4] = (u32)&frm->field_0x68;
        fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
        fn(gpSfdCoreTraceCb, &sTraceDecodeFrm[1]);
    }
    r = MPV_DecodeFrmSj(mpv, sj, &fcnt);
    b = SJRBF_GetFlowCnt(sj, 0, 1);
    n = b - a;
    if (gpSfdCoreTraceCb != NULL) {
        sTraceDecodeFrm[0x74 / 4] = (u32)&n;
        sTraceDecodeFrm[0x80 / 4] = (u32)&fcnt.decoded;
        sTraceDecodeFrm[0x8c / 4] = (u32)&fcnt.field_0x3c;
        sTraceDecodeFrm[0x98 / 4] = (u32)&fcnt.field_0x40;
        fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
        fn(gpSfdCoreTraceCb, &sTraceDecodeFrm[0x6c / 4]);
    }
    t2 = SFTMR_GetTmr(self);
    SFTMR_AddTsum((u8*)&self->field_0x26a0 + frm->type * 0x20, t2 - t1);
    self->field_0x0a14 += fcnt.decoded;
    self->field_0x0a18 += fcnt.field_0x3c;
    if (r == 0) {
        ret = 0;
    } else if (r == -2) {
        if (n > 0) {
            ret = 0;
        } else {
            ret = SFLIB_SetErr(self, -2);
        }
    } else if (r == -3) {
        if (n > 0) {
            ret = 0;
        } else {
            ret = SFLIB_SetErr(self, -3);
        }
    } else {
        ret = SFLIB_SetErr(self, 0xff000f06);
    }
    SFBUF_AddRtotSj(self, self->trn[2].bufIdx, n);
    self->field_0x09b8 += n;
    if (n > 0) {
        self->field_0x0050 = 1;
    }
    if (ret != 0) {
        SFMPVF_FreeFrm(out);
        return ret;
    }
    if (n > 0) {
        out->ttu.field_0x00 = self->tim.tcC.flag;
        out->ttu.field_0x04 = self->tim.tcC.field_0x04;
        out->ttu.field_0x08 = self->tim.tcC.hour;
        out->ttu.field_0x0c = self->tim.tcC.minute;
        out->ttu.field_0x10 = self->tim.tcC.second;
        out->ttu.field_0x14 = self->tim.tcC.frame;
        out->ttu.field_0x18 = self->tim.tcC.frame2;
        out->ttu.field_0x1c = *(s32*)&self->tim.tcC.field_0x1c;
        *(s32*)&out->ttu.field_0x20 = self->tim.tcC.field_0x20;
        out->ttu.field_0x24 = self->tim.tcC.ttime;
        out->ttu.field_0x28 = self->tim.tcC.ftime;
        out->timeUnit = self->tim.tcC.ftime;
        a = self->tim.concat.time + (self->tim.tcC.ttime - self->tim.limit);
        out->time = a;
        out->field_0x58 = self->tim.tcC.ttime;
        out->field_0x5c = self->tim.tcC.ttime + self->tim.concat.time;
        if (self->tim.maxTime < a) {
            self->tim.maxTime = a;
            self->tim.maxTimeUnit = out->timeUnit;
        }
        out->order = shc->frmSeq;
        out->field_0x4c = a + shc->field_0x0fc + shc->frmSeq;
        out->field_0x50 = n + shc->frmSeq;
        out->field_0x100 = (s16)fcnt.decoded;
        if (shc->cur == NULL) {
            shc->state = (shc->state == 2) ? 3 : 5;
        }
        if (frm->field_0x38 == 3 || shc->cur != NULL) {
            shc->cur = NULL;
        } else {
            shc->cur = out;
        }
        shc->field_0x0f4 = 0;
        shc->field_0x0f8 = 0;
        if (shc->cur == NULL) {
            if (self->field_0x0040 == 3) {
                s32 t = frm->type;
                if (t >= 2 && t <= 3) {
                    SFMPVF_RefStbyFrm(out);
                } else {
                    SFMPVF_StbyFrm(out);
                }
            } else {
                SFMPVF_StbyFrm(out);
            }
            MPV_GetDctCnt(mpv, (u32*)&self->field_0x0968, (u32*)&self->field_0x096c);
            shc->field_0x00c = 0;
        }
        SFPLY_AddDecPic(self, 1, frm->type);
        self->field_0x0050 = 1;
    } else {
        if (shc->cur == NULL) {
            SFMPVF_FreeFrm(out);
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_SetFrmPara
// ---------------------------------------------------------------------------
s32 sfmpv_SetFrmPara(SfdHandle* self, SfdMpvPicAtr* frm, SfdMpvFrmPara* para, SfdFrame** out) {
    SfdMpvSession* shc = self->trn[2].hdr;
    if (shc->cur != NULL) {
        *out = shc->cur;
    } else {
        *out = SFMPVF_AllocFrm(self);
        if (*out == NULL) {
            self->field_0x098c = 1;
            /* notify error callback through its vtable slot 0x24 */
            if (gpSfdCoreTraceCb != NULL) {
                sTraceSfdWaitRelFrm[3] = (u32)self;
                ((void (*)(void*, void*))(*(void**)((u8*)*(void**)gpSfdCoreTraceCb + 0x24)))
                    (gpSfdCoreTraceCb, &sTraceSfdWaitRelFrm[1]);
            }
            return -1;
        }
    }
    {
        /* macroblock parameter words; 16 pairs copied with walking pointers */
        u32* src = (u32*)frm - 1;
        (*out)->field_0x0c = (u16)frm->width;
        (*out)->field_0x0e = (u16)frm->height;
        (*out)->field_0x10 = (u16)frm->field_0x08;
        (*out)->field_0x12 = (u16)frm->field_0x0c;
        (*out)->field_0x14 = frm->type;
        {
            s32 n = 0x10;
            u32* dst = (u32*)&(*out)->picAtr - 1;
            do {
                dst[1] = src[1];
                src += 2;
                dst += 2;
                *dst = *src;
            } while (--n != 0);
        }
    }
    ((u32*)&(*out)->field_0xe8)[1] = shc->ptsLo;
    ((u32*)&(*out)->field_0xe8)[0] = shc->ptsHi;
    (*out)->field_0xF0 = frm->field_0x34;
    (*out)->field_0xF4 = frm->field_0x30;
    (*out)->field_0xF8 = frm->field_0x68;
    (*out)->field_0xFC = (frm->field_0x5f - 1 == 0);
    if (self->fhd.info.active != 0 && self->fhd.info.video[5] != 0 && self->fhd.info.video[12] == 1) {
        (*out)->field_0xFC = 1;
    }
    if (self->field_0x0040 == 3) {
        if (frm->type >= 2 && frm->type <= 3) {
            if (shc->cur == NULL) {
                if (shc->refTerm != NULL) {
                    SFMPVF_EndRefFrm(shc->refTerm);
                }
                shc->refTerm = shc->refPrev;
                shc->refPrev = *out;
            }
        }
        {
            /* derive luma/chroma plane sizes and fill both reference-plane
             * descriptor halves; strides come from the frame width */
            SfdFrame* r2 = shc->refPrev;
            SfdFrame* r1 = shc->refTerm;
            s32 w = (s32)frm->width;
            s32 h = (s32)frm->height;
            s32 w16;
            s32 wBytes;
            s32 hB32;
            s32 chromaW;
            s32 luma16;
            s32 chromaH;
            s32 planeSize;
            s32 chromaSize;
            u32 y1;
            u32 y2;
            if (r2 == NULL) {
                r2 = *out;
            }
            if (r1 == NULL) {
                r1 = r2;
            }
            w16 = (w + 0xf) / 0x10;
            wBytes = w16 * 0x10;
            hB32 = (wBytes + 0x1f) / 0x20;
            chromaW = ((((wBytes + (w16 >> 4 & 1)) >> 1) + 0x1f) / 0x20) * 0x20;
            luma16 = (h + 0xf) / 0x10;
            chromaH = (luma16 * 0x10 + (luma16 >> 4 & 1)) >> 1;
            planeSize = wBytes * (hB32 * 0x20);
            chromaSize = chromaH * chromaW;
            y1 = (u32)r1->picBuf;
            y2 = (u32)r2->picBuf;
            para->plane[0].lineSiz = hB32 * 0x20;
            para->plane[0].uvLineSiz = chromaW;
            para->plane[0].base = y1;
            para->plane[0].field_0x00 = y1 + planeSize;
            para->plane[0].field_0x04 = y1 + planeSize + chromaSize;
            para->plane[1].base = y2;
            para->plane[1].field_0x00 = y2 + planeSize;
            para->plane[1].field_0x04 = y2 + planeSize + chromaSize;
            para->plane[1].uvLineSiz = chromaW;
            para->plane[1].lineSiz = hB32 * 0x20;
        }
    } else {
        if (frm->type >= 2 && frm->type <= 3) {
            shc->planeSelA ^= 1;
            shc->planeSelB ^= 1;
            shc->refPrev = *out;
        }
        {
            u32* d = (u32*)para;
            u32* t = (u32*)&shc->plane[shc->planeSelA];
            d[0] = t[0];
            d[1] = t[1];
            d[2] = t[2];
            d[3] = t[3];
            t = (u32*)&shc->plane[shc->planeSelB];
            d[4] = t[0];
            d[5] = t[1];
            d[6] = t[2];
            d[7] = t[3];
        }
    }
    para->picBuf = (u32)(*out)->picBuf;
    para->picAtr = (u32)&(*out)->picAtr;
    para->decoded = 0;
    para->field_0x3c = 0;
    para->field_0x40 = 0;
    self->field_0x098c = 0;
    return 0;
}

// ---------------------------------------------------------------------------
// fn_803C9948
// ---------------------------------------------------------------------------
void fn_803C9948(SfdHandle* a, SfdMpvSession* b, s32* out1, s32* out2) {
    s32 v1 = 0;
    s32 v2 = 0;
    if (a->field_0x0040 == 3) {
        switch (b->picAtr.type) {
        case 2: {
            SfdFrame* p = b->refTerm;
            if (p != NULL) {
                v1 = p->field_0x4c;
                v2 = p->field_0x50;
            }
            break;
        }
        case 3: {
            SfdFrame* p = b->refTerm;
            if (p != NULL) {
                v1 = p->field_0x4c;
                v2 = p->field_0x50;
            }
            SfdFrame* q = b->refPrev;
            if (q != NULL) {
                v1 += q->field_0x4c;
                v2 += q->field_0x50;
            }
            break;
        }
        }
    }
    *out1 = v1;
    *out2 = v2;
}

// ---------------------------------------------------------------------------
// fn_803C99C8
// ---------------------------------------------------------------------------
/* eight-word timecode block (runs / fields) */
typedef struct SfdTcBlock {
    u32 w[8];
} SfdTcBlock;

typedef struct {
    u8 pad0[0xa2c];          /* 0x000 */
    s32 a2c;                 /* 0xa2c */
    u8 pad1[0xdd4 - 0xa30];  /* 0xa30 */
    u32 tc[11];              /* 0xdd4 */
    u8 pad2[0xe58 - 0xe00];  /* 0xe00 */
    u32 tflag;               /* 0xe58 */
    u32 t[8];                /* 0xe5c */
    u32 t9;                  /* 0xe80 */
    u8 pad3[0xeb0 - 0xe84];  /* 0xe84 */
    u32 dst[11];             /* 0xeb0 */
    u8 pad4[0x2068 - 0xedc]; /* 0xedc */
    void* shc;               /* 0x2068 */
} SFMPV99;

void fn_803C99C8(SfdHandle* self) {
    SFMPV99* o = (SFMPV99*)self;
    SfdTcBlock local;
    s32* base;
    u32* dst;
    s32 flg;
    s32 t1;
    s32 t2;
    void* shc;

    base = (s32*)&o->tc[0];
    dst = &o->dst[0];
    if (o->dst[0] != 0) {
        return;
    }
    flg = 0;
    local = *(SfdTcBlock*)&o->t[0];
    shc = o->shc;
    if (*(s32*)((u8*)shc + 4) == 2) goto case2;
    if (*(s32*)((u8*)shc + 4) == 3) goto case3;
    goto done;
case2:
    flg = 1;
    goto done;
case3:
    flg = 1;
done:
    if (flg == 0 && o->a2c != 0) {
        local.w[6] = 0;
    }
    SFTIM_Tc2Time(local.w, &t1, &t2);
    *(SfdTcBlock*)&o->dst[1] = local;
    o->dst[9] = t1 - o->tc[9];
    o->dst[10] = t2;
    o->dst[0] = 1;
}

// ---------------------------------------------------------------------------
// sfmpv_GoDdelim
// ---------------------------------------------------------------------------
// NOTE: residual 24-instruction reg swap vs retail: retail homes param3 (mask)
// into r31 and param1 (self) into r30; this MWCC always homes param1 first.
// Declaration-order/copy/coalescing levers all exhausted (see session notes).
s32 sfmpv_GoDdelim(SfdHandle* self, u32 unused, u32 mask) {
    s32 n;
    u8* seg0;
    u32 siz0;
    u8* seg1;
    s32 found;
    s32 i;
    u32 info[7];
    u32 r;
    u32 type;
    u32 m;

    m = mask;
    if (SFBUF_RingGetRead(self, self->trn[2].bufIdx, info) != 0) {
        return 0;
    }
    if (info[1] == 0) {
        return 0;
    }
    r = sfmpv_SearchDelim(info, m, &type);
    if (r == 0) {
        n = (s32)(info[1] + info[3] - 3);
        n = (n > 0) ? n : 0;
    } else {
        if (info[0] <= r && r < info[0] + info[1]) {
            n = (s32)(r - info[0]);
        } else if (info[2] <= r && r < info[2] + info[3]) {
            n = (s32)(info[1] + (r - info[2]));
        } else {
            n = 0;
        }
    }
    SFBUF_RingAddRead(self, self->trn[2].bufIdx, n);
    /* scan up to min(n,3) bytes of the (possibly two-segment) ring for a
     * non-zero byte; a nonzero byte means the skipped chunk carries data */
    seg0 = (u8*)info[0];
    siz0 = info[1];
    seg1 = (u8*)info[2];
    found = 0;
    for (i = 0; i < (n < 3 ? n : 3); i++) {
        u8* p2;
        if (i < (s32)siz0) {
            p2 = seg0;
        } else {
            p2 = seg1 + (i - (s32)siz0);
        }
        if (*(s8*)p2 != 0) {
            found = 1;
            break;
        }
        seg0++;
    }
    if (found != 0) {
        self->field_0x09c0 += n;
    }
    self->field_0x09b8 += n;
    return n;
}

// ---------------------------------------------------------------------------
// SFMPV_Create
// ---------------------------------------------------------------------------
s32 SFMPV_Create(SfdHandle* self) {
    SfdMpvSession* shc;
    void* mpv;
    s32 r;
    if (SFSET_GetCond(self, 5) == 0) {
        return 0;
    }
    shc = &self->mpvSession;
    self->trn[2].hdr = shc;
    r = sfmpv_InitInf(self, shc);
    if (r != 0) {
        return r;
    }
    mpv = (void*)MPV_Create();
    if (mpv == NULL) {
        return SFLIB_SetErr(0, 0xff000f0a);
    }
    if (MPV_SetErrFunc(mpv, sfmpv_ErrFn, self) != 0) {
        MPV_Destroy(mpv);
        return SFLIB_SetErr(0, 0xff000f0b);
    }
    MPV_SetCond(mpv, 1, SFSET_GetCond(self, 0));
    MPV_SetCond(mpv, 2, SFSET_GetCond(self, 1));
    MPV_SetCond(mpv, 6, self->field_0x0040);
    shc->mpv = mpv;
    if (SFPLY_GetResetFlg() != 0) {
        SFMPVF_ResetPicUsrBuf(self);
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_InitInf
// ---------------------------------------------------------------------------
s32 sfmpv_InitInf(SfdHandle* self, SfdMpvSession* shc) {
    s32 r;
    s32 i;
    r = SFMPVF_ReadGlobalMpvPara(self);
    if (r != 0) {
        return r;
    }
    shc->mpv = NULL;
    self->curFrame = 0;
    shc->state = 5;
    shc->pattern = 0xc0;
    shc->planeSelB = 1;
    shc->planeSelA = 0;
    self->termFlag = 0;
    shc->refTerm = NULL;
    shc->refPrev = NULL;
    shc->cur = NULL;
    shc->field_0x0f4 = 0;
    shc->field_0x0f8 = 0;
    shc->field_0x0fc = 0;
    sfmpv_InitFrmObj(&self->frames[0], (u32*)&self->para.frameSize[0], 0x10);
    shc->field_0x00c = 0;
    shc->frmSeq = 0;
    UTY_MemsetDword((u32*)&shc->picAtr, -1, 0x20);
    shc->pts = -1;
    shc->repFld = 0;
    shc->vbvLimit = 0x7fffffff;
    shc->ptsBase[0] = 0;
    shc->ptsBase[1] = 0;
    shc->field_0x0a8 = 0;
    shc->field_0x0b4 = -1;
    shc->field_0x0b0 = -1;
    shc->field_0x0b8 = 0;
    shc->field_0x0bc = -1;
    SFMPVF_InitPicUsr(&self->picUsr);
    for (i = 0; i < 16; i++) {
        self->frames[i].field_0x60 = (u32)&self->picUsr.band[i].top;
    }
    shc->ptsLo = -1;
    shc->ptsHi = -1;
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_InitFrmObj
// ---------------------------------------------------------------------------
void sfmpv_InitFrmObj(SfdFrame* frm, const u32* src, s32 count) {
    const u32* q = src;
    s32 i;
    for (i = 0; i < count; i++) {
        frm->state = 0;
        frm->field_0x04 = 0;
        frm->field_0x0c = 0;
        frm->field_0x0e = 0;
        frm->field_0x10 = 0;
        frm->field_0x12 = 0;
        frm->field_0x14 = 5;
        SFTIM_InitTtu(&frm->ttu, 0);
        frm->picBuf = (u8*)*q;
        frm->time = 0;
        frm->timeUnit = 1;
        frm->field_0x4c = 0;
        frm->field_0x50 = 0;
        frm->order = 0;
        frm->field_0x58 = 0;
        frm->field_0x5c = 0;
        frm->dispId = -1;
        UTY_MemsetDword((u32*)&frm->picAtr, -1, 0x20);
        frm->field_0xe8 = -1;
        frm->field_0xF0 = 0;
        frm->field_0xF4 = 0;
        frm->field_0xF8 = 0;
        frm->field_0xFC = 0;
        frm->field_0x100 = 0;
        frm++;
        q += 1;
    }
}

// ---------------------------------------------------------------------------
// sfmpv_ErrFn
// ---------------------------------------------------------------------------
void sfmpv_ErrFn(s32 val, u32 err) {
    u32 tmp = err + 3;
    if (tmp <= 1 || err == 0) {
        return;
    }
    if ((err & 0xFFFF0000) == 0xFF040000)
        fn_803C34F8(val, err);
    else
        SFLIB_SetErr((void*)val, err);
}

// ---------------------------------------------------------------------------
// SFMPV_Destroy
// ---------------------------------------------------------------------------
s32 SFMPV_Destroy(SfdHandle* self) {
    SfdMpvSession* shc = self->trn[2].hdr;
    void* mpv = shc->mpv;
    if (mpv == NULL) {
        return 0;
    }
    SFMPVF_WriteGlobalMpvPara(self);
    if (MPV_Destroy(mpv) != 0) {
        return SFLIB_SetErr(self, 0xff000f0c);
    }
    shc->mpv = NULL;
    return 0;
}

// ---------------------------------------------------------------------------
// SFMPV_RequestStop
// ---------------------------------------------------------------------------
int SFMPV_RequestStop(SfdHandle* self) {
    void* p;
    int result;

    p = ((SfdMpvSession*)self->trn[2].hdr)->mpv;
    if (p == NULL) {
        result = 0;
    } else {
        MPV_RequestStop(p);
        result = 0;
    }
    return result;
}

int SFMPV_Start(void) { return 0x0; }

int SFMPV_Stop(void) { return 0x0; }

int SFMPV_Pause(void) { return 0x0; }

s32 SFMPV_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000f0d);
}

s32 SFMPV_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000f0d);
}

void SFMPVF_GetRead(void* self, void* a, void* b, void* cb);
void SFMPV_GetRead(void* self, void* a, void* b) {
    SFMPVF_GetRead(self, a, b, sfmpv_SetFrmInf);
}

// ---------------------------------------------------------------------------
// sfmpv_SetFrmInf
// ---------------------------------------------------------------------------
void sfmpv_SetFrmInf(SfdHandle* self, SfdFrame* frm, SfdFrmInfo* info) {
    info->field_0x00 = frm->picAtr.width;
    info->field_0x04 = frm->picAtr.height;
    info->field_0x08 = frm->picAtr.field_0x08;
    info->field_0x0c = frm->picAtr.field_0x0c;
    info->field_0x10 = (u32)frm->picAtr.type;
    info->time = frm->time;
    info->timeUnit = frm->timeUnit;
    info->codecId = self->field_0x0040;
    info->field_0x20 = (u32)frm->picBuf;
    info->field_0x24 = frm->field_0x4c;
    info->field_0x28 = frm->field_0x50;
    info->field_0x2c = frm->order;
    info->field_0x30 = frm->field_0x58;
    info->field_0x34 = frm->field_0x5c;
    info->field_0x38 = frm->field_0x60;
    info->field_0x3c = frm->picAtr.field_0x40;
    info->field_0x40 = frm->picAtr.field_0x44;
    if (frm->picAtr.field_0x40 == 0) {
        info->field_0x48 = 2;
    } else {
        info->field_0x48 = 1;
    }
    info->field_0x50 = frm->field_0xe8;
    info->field_0x58 = frm->picAtr.field_0x38;
    info->field_0x5c = frm->picAtr.field_0x3c;
    info->field_0x60 = frm->picAtr.field_0x48;
    info->field_0x64 = frm->picAtr.field_0x4c;
    info->field_0x68 = frm->picAtr.field_0x50;
    info->field_0x6a = frm->picAtr.field_0x52;
    info->field_0x6c = frm->picAtr.field_0x55;
    info->field_0x6d = frm->picAtr.field_0x56;
    info->field_0x6e = frm->picAtr.field_0x57;
    info->field_0x6f = frm->picAtr.field_0x59;
    info->field_0x70 = frm->picAtr.field_0x5a;
    info->field_0x71 = frm->picAtr.field_0x5b;
    info->field_0x72 = frm->picAtr.field_0x5c;
    info->field_0x73 = frm->picAtr.field_0x5d;
    info->field_0x74 = frm->picAtr.field_0x5e;
    info->field_0x75 = frm->picAtr.field_0x5f;
    info->field_0x76 = frm->picAtr.field_0x60;
    info->field_0x77 = frm->picAtr.field_0x61;
    info->field_0x78 = frm->picAtr.field_0x62;
    info->field_0x79 = frm->picAtr.field_0x63;
    info->field_0x7a = frm->picAtr.field_0x64;
    info->field_0x7c = frm->field_0x100;
}

void SFMPV_AddRead(void) { SFMPVF_AddRead(); }

// ---------------------------------------------------------------------------
// SFMPV_Seek
// ---------------------------------------------------------------------------
s32 SFMPV_Seek(SfdHandle* self) {
    SfdMpvSession* shc = self->trn[2].hdr;
    s32 out;
    s32 r = sfmpv_ReprocessShc(self, shc, &out);
    if (r != 0) {
        return r;
    }
    shc->state = 2;
    if (!(out != 0 && SFSET_GetCond(self, 0x30) != 0)) {
        shc->pattern = 0xc0;
    } else {
        shc->pattern = 0xc8;
    }
    return 0;
}

int fn_803CA368(void) { return 0x0; }

// ---------------------------------------------------------------------------
// sfmpv_ReprocessShc
// ---------------------------------------------------------------------------
s32 sfmpv_ReprocessShc(SfdHandle* self, SfdMpvSession* shc, s32* out) {
    s32 v;
    SfdAvpHdCopy* p;
    SfdMpvSession* hd;
    void* mpv;
    u32 out2;
    u32 pic[2];

    *out = 0;
    v = (s32)self->see.avplay;
    hd = self->trn[2].hdr;
    if (v == 0) {
        p = NULL;
    } else if (hd->frmSeq > 0) {
        p = &self->see.avplay->audHd;
    } else {
        p = NULL;
    }
    if (p == NULL) {
        return 0;
    }
    if (p->state == 0) {
        return 0;
    }
    mpv = shc->mpv;
    self->tim.hdInfo = p->hdInfo;
    pic[0] = (u32)p->image;
    pic[1] = (u32)p->size;
    if (MPV_DecodePicAtr(mpv, pic, &out2) != 0) {
        return SFLIB_SetErr(self, 0xff000f1b);
    }
    *out = 1;
    return 0;
}
