// Decompiled CriWare Sofdec sfd_mpv module.
#include <harness_catalog.h>

// ---------------------------------------------------------------------------
// Local view of the SFMPV handle tail (pic-user-buffer output area).
// Used so the store below emits a plain stw displacement (no address CSE).
// ---------------------------------------------------------------------------
typedef struct SFMPVDecView {
    u8 field_0x0[0x3908];
    u32 field_0x3908;
} SFMPVDecView;

// ---------------------------------------------------------------------------
// Local view of the 11-word tc-save block at self+0xe58 (copy source/target).
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Local view of the tc-save region: cur block at +0xe58, prev block at +0xe84,
// deadline at +0xed4, shc pointer at +0x2068.
// ---------------------------------------------------------------------------
typedef struct TCBlockView {
    u32 w[9];
    s32 tm;
    u32 fx;
} TCBlockView;

typedef struct SfMpvTcView {
    u8 pad_0x0[0xe58];
    TCBlockView cur_tc;
    TCBlockView prv_tc;
    u8 pad_0xeb0[0x24];
    s32 lim;
    u8 pad_0xed8[0x2068 - 0xed8];
    void* shc;
} SfMpvTcView;

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
void sfmpv_ProcessAuxShc(void* self);
s32 sfmpv_IsVbvEnough(void* self);
u32 sfmpv_SearchDelim(void* buf, u32 mask, u32* out);
u32 sfmpv_BsearchDelim(void* buf, u32 mask, u32* out);
s32 sfmpv_NeedSafeDlmRefresh(void* buf, u32 dlm, u32 dlm_new);
s32 sfmpv_GetActiveSize(void* self, s32* out1, s32* out2, s32* out3);
s32 sfmpv_DecodeOneUnit(void* self, s32 ch, s32 pat, s32 dlm, s32* out);
s32 sfmpv_ConcatSub(u8* self);
s32 sfmpv_DecodePicAtr(void* self, u32* pic, void* sj, s32 pat, s32* out);
s64 sfmpv_ComplementPts(void* self, u32* dst, void* frm, u32* pts, s32 mode, s32* out1, s32 rate);
void sfmpv_CalcRepeatField(void* self, void* frm, s32 mode);
void sfmpv_DoReformTc(void* self, void* frm, s64 pts, s32 rep);
void sfmpv_Pts2Tc(s64 a, s32 v, s32 t, s32 u, u32* out);
void sfmpv_NextTc(void* in, void* out);
s32 sfmpv_FirstPicAtr(void* self, void* mpv, void* frm, void* pic);
void sfmpv_SetMpvHd(void* self, s32 bitrate, u32* pic);
s32 sfmpv_ChkBufSiz(void* self, void* para);
s32 sfmpv_IsSkip(void* self, void* bpic);
s32 sfmpv_IsEmptyBpic(void* self, s32 type, void* bpic);
void sfmpv_UpdateDefect(void* self, void* frm, s32 mode);
s32 sfmpv_IsLate(void* self, s32 type);
s32 sfmpv_SkipFrm(void* self, void* sj);
s32 sfmpv_DecodeFrm(void* self, void* sj);
s32 sfmpv_SetFrmPara(void* self, void* frm, void* para, void** out);
void fn_803C9948(void* a, void* b, s32* out1, s32* out2);
void fn_803C99C8(void* self);
s32 sfmpv_GoDdelim(u8* self, u32 unused, u32 mask);
s32 sfmpv_InitInf(u8* self, u8* shc);
void sfmpv_InitFrmObj(void* frm, const u32* src, s32 count);
s32 sfmpv_ReprocessShc(void* self, void* shc, s32* out);
void sfmpv_SetFrmInf(void* self, void* frm, void* info);
s32 SFMPV_Seek(void* self);

void sfmpv_ErrFn(s32 val, u32 err);
s32 fn_803C34F8(s32 val, u32 err_code);

// Global data
extern u32 lbl_eu_80607AF8;
extern u32 lbl_eu_80619B18;
extern void* lbl_eu_80606E34;
extern u32 lbl_eu_8051C4E0;
extern u32 lbl_eu_8051C940[];
extern u32 lbl_eu_8051C97C[];
extern u32 lbl_eu_8051CBF8[];
extern u8 lbl_eu_8051C9E0[];
extern u32 lbl_eu_805688C8[];
extern u32 lbl_eu_80568A70[];
extern u32 lbl_eu_80568B44[];
extern u32 lbl_eu_80568C18[];
extern u32 lbl_eu_80568CF0[];
extern u32 lbl_eu_80568D5C[];
extern u32 lbl_eu_80568DC8[];
extern u32 lbl_eu_80568E34[];

// ---------------------------------------------------------------------------
// SFD_SetMpvCond
// ---------------------------------------------------------------------------
s32 SFD_SetMpvCond(void* h, s32 cond, s32 val) {
    void* mpv;
    if (h == NULL) {
        mpv = NULL;
    } else {
        if (SFLIB_CheckHn(h) != 0) {
            return SFLIB_SetErr(0, 0xff000181);
        }
        mpv = **(void***)((u8*)h + 0x2068);
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
s32 SFMPV_SaveCond(void* h, s32* conds, u32 count) {
    void* mpv;
    s32 i;
    s32 max;
    u32 n;
    mpv = **(void***)((u8*)h + 0x2068);
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
void SFMPV_RestoreCond(void* h, s32* conds, s32 count) {
    void* mpv = **(void***)((u8*)h + 0x2068);
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
void SFD_SetMbCb(void* self, u32 a, u32 b, u32 c) {
    void* arg1 = *(void**)(*(void**)((u8*)self + 0x2068));
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
    r = MPV_Init(8, &lbl_eu_80607AF8);
    if (r != 0) {
        return fn_803C34F8(0, ((u32)r + 0xfd0000 == 0xff05) ? 0xff000f13 : 0xff000f01);
    }
    SFMPVF_InitPool();
    lbl_eu_80619B18 = 0;
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
s32 SFMPV_ExecServer(void* self) {
    s32 ret;
    s32 idx;
    s32 ch;
    s32 v1;
    s32 v2;
    s32 v3;
    void* sj;
    s32 out;

    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_805688C8[3] = (u32)self;
        (*(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24))(lbl_eu_80606E34, &lbl_eu_805688C8[1]);
    }

    if (SFSET_GetCond(self, 5) == 0) {
        return 0;
    }
    if (SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2074)) == 1) {
        return 0;
    }
    if (SFSET_GetCond(self, 0x1c) != 0 && SFHDS_GetColType(self) != -1) {
        if (self == NULL) {
            if (MPV_SetCond(0, 5, 0) != 0) {
                SFLIB_SetErr(self, 0xff000f12);
            }
        } else if (SFLIB_CheckHn(self) != 0) {
            SFLIB_SetErr(0, 0xff000181);
        } else {
            if (MPV_SetCond(**(void***)((u8*)self + 0x2068), 5, 0) != 0) {
                SFLIB_SetErr(self, 0xff000f12);
            }
        }
    }
    if (*(s32*)((u8*)self + 0x54) == 2) {
        sfmpv_ProcessAuxShc(self);
    }
    ret = 0;
    for (;;) {
        if (*(s32*)((u8*)self + 0x70) != 0) break;
        if (*(s32*)((u8*)self + 0x74) != 0) break;
        ret = sfmpv_GetActiveSize(self, &v1, &v2, &v3);
        if (ret != 0) break;
        ret = sfmpv_DecodeOneUnit(self, v1, v2, v3, &out);
        if (ret != 0) break;
        if (out == 0) break;
    }
    {
        u32 fcnt[2];
        /* idx doubles as ch2 here, then the prep-frame limit, then numfrm */
        idx = *(s32*)((u8*)self + 0x2070);
        ch = *(s32*)((u8*)self + 0x2074);
        SFBUF_RingGetSj(self, idx, &sj);
        SFBUF_GetFlowCnt(sj, &fcnt[0], &fcnt[1]);
        *(u64*)((u8*)self + 0x9b0) = SFBUF_UpdateFlowCnt(*(u64*)((u8*)self + 0x9b0), fcnt[0]);
        if (SFBUF_GetPrepFlg(self, ch) != 1 && SFBUF_GetPrepFlg(self, idx) != 1) {
            s32 r = 0;
            if (SFMPVF_IsTermDec(self) != 0) {
                r = 1;
            } else {
                s32 n = *(s32*)((u8*)self + 0x30);
                idx = *(s32*)((u8*)self + 0xa78);
                if (idx == -1) {
                    idx = n + 2;
                }
                if (n + 2 < idx) {
                    idx = n + 2;
                }
                out = SFMPVF_GetNumFrm(self);
                if (*(s32*)((u8*)self + 0x68) == 2) {
                    out += *(s32*)((u8*)self + 0x978);
                }
                if (out >= idx && sfmpv_IsVbvEnough(self) != 0) {
                    r = 1;
                }
            }
            if (r != 0) {
                SFBUF_SetPrepFlg(self, ch, 1);
                if (*(s32*)((u8*)self + 0xed4) != 0x7fff0000 + 0xffff) {
                    *(s32*)((u8*)self + 0xeb0) = 1;
                }
            }
        }
    }
    ch = SFMPVF_GetNumFrm(self);
    if (ch != -1) {
        s32 r = 0;
        if (SFMPVF_IsTermDec(self) != 0) {
            if (ch == 0) {
                r = 1;
            } else if (*(s32*)((u8*)self + 0x68) == 1 && ch == 1) {
                if (*(s32*)((u8*)self + 0x978) > *(s32*)((u8*)self + 0x97c)) {
                    r = 1;
                }
            }
        }
        if (r != 0) {
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2074), 1);
            if (*(s32*)((u8*)self + 0x960) == 0) {
                SFSET_SetCond(self, 5, 0);
            }
        }
    }
    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_805688C8[0x74 / 4] = (u32)((u8*)self + 0x9b0);
        lbl_eu_805688C8[0x80 / 4] = (u32)((u8*)self + 0x9b8);
        lbl_eu_805688C8[0x8c / 4] = (u32)((u8*)self + 0x9c0);
        (*(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24))(lbl_eu_80606E34, &lbl_eu_805688C8[0x6c / 4]);
    }
    return ret;
}

// ---------------------------------------------------------------------------
// sfmpv_ProcessAuxShc
// ---------------------------------------------------------------------------
void sfmpv_ProcessAuxShc(void* self) {
    void* e = *(void**)((u8*)self + 0x2068);
    void* p = *(void**)((u8*)e + 0);
    s32 buf[2];
    s32 c;
    buf[0] = *(s32*)((u8*)self + 0xd90);
    buf[1] = *(s32*)((u8*)self + 0xd94);
    if (buf[0] == 0 || buf[1] == 0)
        return;
    if ((s32)*(u32*)((u8*)e + 8) != 0xc0)
        return;
    if (MPV_DecodePicAtr(p, buf, &c) == 0) {
        *(u32*)((u8*)e + 4) = 2;
        *(u32*)((u8*)e + 8) = 0xc8;
    }
}

// ---------------------------------------------------------------------------
// sfmpv_IsVbvEnough
// ---------------------------------------------------------------------------
s32 sfmpv_IsVbvEnough(void* self) {
    void* shc = *(void**)((u8*)self + 0x2068);
    void* mpv = *(void**)shc;
    u32 bitrate;
    s32 v;
    s32 i;
    if (SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2070)) == 1) {
        return 1;
    }
    if (*(s32*)((u8*)self + 0x88) != 0 && *(s32*)((u8*)self + 0x104) == 0) {
        return 1;
    }
    MPV_GetBitRate(mpv, &bitrate);
    if ((bitrate - 0x30000) == 0xffff) {
        return 1;
    }
    if (SFBUF_GetWTot(self, 1) >= *(s32*)((u8*)shc + 0x9c)) {
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
s32 sfmpv_GetActiveSize(void* self, s32* out1, s32* out2, s32* out3) {
    u32 info[7];  /* sp+0x18: ring read info */
    u32 dlm;      /* sp+0x14 */
    u32 dlm_size; /* sp+0x10 */
    u32 type;     /* sp+0x0c: delimiter type */
    u32 chk;      /* sp+0x08: BsearchDelim out / MPV_CheckDelim result */
    u32 r;
    s32 tmp = 0;
    s32 ch;

    ch = *(s32*)((u8*)self + 0x2070);
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
            tmp = *(s32*)((u8*)self + 0x2070);
            s32 siz = SFBUF_GetRingBufSiz(self, tmp);
            s32 n = siz - SFBUF_RingGetDataSiz(self, tmp);
            if (n < *(s32*)((u8*)self + 0x2c)) {
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
                    tmp = *(s32*)((u8*)self + 0x2070);
                    s32 n = SFBUF_GetRingBufSiz(self, tmp) - SFBUF_RingGetDataSiz(self, tmp);
                    if (n < *(s32*)((u8*)self + 0x2c)) {
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
                    tmp = *(s32*)((u8*)self + 0x2070);
                    s32 n = SFBUF_GetRingBufSiz(self, tmp) - SFBUF_RingGetDataSiz(self, tmp);
                    if (n < *(s32*)((u8*)self + 0x2c)) {
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
s32 sfmpv_NeedSafeDlmRefresh(void* buf, u32 dlm, u32 dlm_new) {
    u8 tmp[4];
    u32 r;
    if (dlm_new == 0) {
        return 1;
    }
    if (dlm_new == *(u32*)buf) {
        return 1;
    }
    if (dlm_new > *(u32*)buf && (s32)(dlm_new - *(u32*)buf) <= 3) {
        return 1;
    }
    if (dlm_new >= *(u32*)buf && dlm_new < *(u32*)buf + *(u32*)((u8*)buf + 4)) {
        /* first-half window: delimiter tail may wrap across the ring segments */
        s32 n = dlm_new + 4 - (*(u32*)buf + *(u32*)((u8*)buf + 4));
        if (n > 0) {
            if (n > *(s32*)((u8*)buf + 0xc)) {
                return 1;
            }
            memcpy(tmp, (void*)dlm_new, 4 - n);
            memcpy(tmp + 4 - n, *(void**)((u8*)buf + 8), n);
        } else {
            memcpy(tmp, (void*)dlm_new, 4);
        }
    } else {
        /* second-half window */
        if (dlm_new >= *(u32*)((u8*)buf + 8) &&
            dlm_new < *(u32*)((u8*)buf + 8) + *(u32*)((u8*)buf + 0xc)) {
            s32 n = dlm_new + 4 - (*(u32*)((u8*)buf + 8) + *(u32*)((u8*)buf + 0xc));
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
u32 sfmpv_SearchDelim(void* buf, u32 mask, u32* out) {
    u8* p;
    s32 bound;
    u32 r2;
    s32 i;
    s32 n1;
    s32 n2;
    u32 r;
    u8 tmp[8];
    *out = 0;
    r = MPV_SearchDelim(*(void**)buf, *(u32*)((u8*)buf + 4), mask);
    if (r != 0) {
        *out = MPV_CheckDelim((u8*)r);
        return r;
    }
    if (*(s32*)((u8*)buf + 0xc) == 0) {
        return 0;
    }
    n1 = *(s32*)((u8*)buf + 4);
    n1 = (n1 < 3) ? n1 : 3;
    n2 = *(s32*)((u8*)buf + 0xc);
    n2 = (n2 < 3) ? n2 : 3;
    memcpy(tmp, *(u8**)buf + *(s32*)((u8*)buf + 4) - n1, n1);
    memcpy(tmp + n1, *(void**)((u8*)buf + 8), n2);
    bound = n1 + n2 - 3;
    p = tmp;
    for (i = 0; i < bound; i++) {
        u32 v = MPV_CheckDelim(p);
        if ((v & mask) != 0) {
            *out = v;
            {
                s32 off = *(u32*)((u8*)buf) + *(s32*)((u8*)buf + 4) - n1;
                return off + i;
            }
        }
        p++;
    }
    r2 = MPV_SearchDelim(*(void**)((u8*)buf + 8), *(u32*)((u8*)buf + 0xc), mask);
    if (r2 != 0) {
        *out = MPV_CheckDelim((u8*)r2);
        return r2;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_BsearchDelim
// ---------------------------------------------------------------------------
u32 sfmpv_BsearchDelim(void* buf, u32 mask, u32* out) {
    u8* p;
    s32 bound;
    u32 r2;
    s32 i;
    s32 n1;
    s32 n2;
    u32 r;
    u8 tmp[8];
    *out = 0;
    if (*(s32*)((u8*)buf + 0xc) != 0) {
        r = MPV_BsearchDelim((u8*)*(void**)((u8*)buf + 8) + *(u32*)((u8*)buf + 0xc), *(u32*)((u8*)buf + 0xc), mask);
        if (r != 0) {
            *out = MPV_CheckDelim((u8*)r);
            return r;
        }
        n1 = *(s32*)((u8*)buf + 4);
        n1 = (n1 < 3) ? n1 : 3;
        n2 = *(s32*)((u8*)buf + 0xc);
        n2 = (n2 < 3) ? n2 : 3;
        memcpy(tmp, *(u8**)buf + *(s32*)((u8*)buf + 4) - n1, n1);
        memcpy(tmp + n1, *(void**)((u8*)buf + 8), n2);
        bound = n1 + n2 - 3;
        p = tmp;
        for (i = 0; i < bound; i++) {
            u32 v = MPV_CheckDelim(p);
            if ((v & mask) != 0) {
                *out = v;
                {
                    s32 off = *(u32*)((u8*)buf) + *(s32*)((u8*)buf + 4) - n1;
                    return off + i;
                }
            }
            p++;
        }
    }
    r2 = MPV_BsearchDelim((u8*)*(void**)((u8*)buf) + *(u32*)((u8*)buf + 4), *(u32*)((u8*)buf + 4), mask);
    if (r2 != 0) {
        *out = MPV_CheckDelim((u8*)r2);
        return r2;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_DecodeOneUnit
// ---------------------------------------------------------------------------
s32 sfmpv_DecodeOneUnit(void* self, s32 ch, s32 pat, s32 dlm, s32* out) {
    void (*fn)(void*, void*);
    s32 out2;
    void* sj;
    void* p;
    void* shc2;
    u32 d_vdo[2];
    u32 d_end[2];
    u32 pic[2];
    s32 ret;
    s32 c0;

    *out = 0;
    *(s32*)((u8*)self + 0x988) = 0;
    p = *(void**)((u8*)self + 0x2068);
    if (*(s32*)((u8*)p + 8) != 0xcc || *(s32*)((u8*)p + 0xf8) != 0) {
        pat &= 0xcc;
    }
    ret = SFBUF_RingGetSj(self, *(s32*)((u8*)self + 0x2070), &sj);
    if (ret != 0) {
        return 0;
    }
    if ((pat & 0xc8) != 0) {
        SFMPVF_SetGopStat(self);
    }
    if (pat == 0x80) {
        if (*(s32*)((u8*)self + 0x2080) < 0) {
            *(s32*)((u8*)self + 0x2080) = SFBUF_GetRTot(self, *(s32*)((u8*)self + 0x2070)) + 4;
        }
        if (*(s32*)((u8*)self + 0xe50) < 0) {
            /* refresh the 11-word tc-save block at +0xe2c from +0xe00 */
            *(TCBlockView*)((u8*)self + 0xe2c) = *(TCBlockView*)((u8*)self + 0xe00);
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
                        SFBUF_AddRtotSj(self, *(s32*)((u8*)self + 0x2070), 4);
                        *(s64*)((u8*)self + 0x9b8) += 4;
                        *(s32*)((u8*)self + 0x50) = 1;
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
                    SFBUF_AddRtotSj(self, *(s32*)((u8*)self + 0x2070), 4);
                    *(s64*)((u8*)self + 0x9b8) += 4;
                    *(s32*)((u8*)self + 0x50) = 1;
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
        } else if (ch <= 4 && SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2070)) == 1) {
            r = 1;
        } else {
            r = 0;
        }
        if (r == 0) {
            /* terminate decode once either playback-limit condition holds */
            c0 = SFSET_GetCond(self, 0x4c);
            s32 c1 = SFSET_GetCond(self, 0x4d);
            shc2 = *(void**)((u8*)self + 0x2068);
            if ((c0 != -1 && *(s32*)((u8*)self + 0x960) >= c0) ||
                (c1 != -1 && *(s32*)((u8*)shc2 + 0x94) > c1)) {
                r = 1;
            }
        }
        if (r != 0) {
            SFMPVF_TermDec(self);
            goto done;
        }
    }
    if (dlm == 0 && ch <= 4) {
        *(s32*)((u8*)self + 0x988) = 1;
        if (lbl_eu_80606E34 != NULL) {
            lbl_eu_80568D5C[3] = (u32)self;
            fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
            fn(lbl_eu_80606E34, &lbl_eu_80568D5C[1]);
        }
        goto done;
    }
        if ((pat & 0x4c) != 0) {
        u32 info[7];
        if (SFBUF_RingGetRead(self, *(s32*)((u8*)self + 0x2070), info) != 0) {
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
            if ((pat & *(s32*)((u8*)p + 8)) != 0) {
                *(s32*)((u8*)p + 8) = 0xcc;
            }
            *(s32*)((u8*)p + 0xf8) = 1;
        }
        if (pat == 0x40 && out2 == -2) {
            *(s32*)((u8*)p + 8) = 0xc0;
        }
        *out = 1;
        goto done;
    }
    if ((pat & 2) != 0) {
        u32 info[7];
        if (SFBUF_RingGetRead(self, *(s32*)((u8*)self + 0x2070), info) != 0) {
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
        if (sfmpv_GoDdelim((u8*)self, (u32)sj, 0xcc) > 0) {
            *out = 1;
        }
    }
done:
    return ret;
}

// ---------------------------------------------------------------------------
// sfmpv_ConcatSub
// ---------------------------------------------------------------------------
s32 sfmpv_ConcatSub(u8* self) {
    u8* shc;
    s32 dlm;
    shc = *(u8**)(self + 0x2068);
    if (SFSET_GetCond(self, 6) == 0) {
        u8* e00 = self + 0xe00;
        u8* ttu = self + 0xdd4;
        if (*(s32*)e00 == 0) {
            dlm = 0;
        } else {
            u32 tc[8];
            s32 t1;
            s32 t2;
            sfmpv_NextTc(e00 + 4, tc);
            tc[6] = 0;
            SFTIM_Tc2Time(tc, &t1, &t2);
            dlm = t1 - *(s32*)(ttu + 0x24);
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
        *(s32*)(self + 0xf8c) += lastSmpl;
        if ((dlm = UTY_MulDiv(*(s32*)(self + 0xf8c), *(s32*)(self + 0xdfc), val) - *(s32*)(self + 0xefc)) < 0) {
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
        if (lbl_eu_80606E34 != NULL) {
            void (*fn)(void*, void*);
            lbl_eu_80568E34[3] = (u32)self;
            lbl_eu_80568E34[0x18 / 4] = (u32)(shc + 0x10);
            lbl_eu_80568E34[0x24 / 4] = (u32)&dlm;
            lbl_eu_80568E34[0x30 / 4] = (u32)(self + 0xefc);
            lbl_eu_80568E34[0x3c / 4] = (u32)(self + 0xef8);
            fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
            fn(lbl_eu_80606E34, &lbl_eu_80568E34[1]);
        }
    }
    SFTIM_InitTtu(self + 0xdd4, 0x7fffffff);
    SFTIM_InitTtu(self + 0xe00, -1);
    *(s32*)(shc + 8) = 0xc0;
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_DecodePicAtr
// ---------------------------------------------------------------------------
s32 sfmpv_DecodePicAtr(void* self, u32* pic, void* sj, s32 pat, s32* out) {
    u8* shc;
    void* mpv;
    u8* frm;
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

    shc = *(u8**)((u8*)self + 0x2068);
    mpv = *(void**)shc;
    frm = shc + 0x14;
    ((SFMPVDecView*)self)->field_0x3908 = 0;
    MPV_SetPicUsrBuf(mpv, *(void**)((u8*)self + 0x3904), *(void**)((u8*)self + 0x3900));
    fc = SJRBF_GetFlowCnt(sj, 0, 1);
    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_80568A70[3] = (u32)mpv;
        lbl_eu_80568A70[0x18 / 4] = (u32)&fc;
        (*(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24))(lbl_eu_80606E34, &lbl_eu_80568A70[1]);
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
    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_80568A70[0x74 / 4] = (u32)&t1;
        lbl_eu_80568A70[0x80 / 4] = (u32)out;
        lbl_eu_80568A70[0x8c / 4] = (u32)&ret;
        (*(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24))(lbl_eu_80606E34, &lbl_eu_80568A70[0x6c / 4]);
    }
    SFBUF_AddRtotSj(self, *(s32*)((u8*)self + 0x2070), t1);
    *(s64*)((u8*)self + 0x9b8) += t1;
    if (t1 > 0) {
        *(s32*)((u8*)self + 0x50) = 1;
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
        if (SFMPVF_ChkImageSize(self, *(u32*)((u8*)frm + 0), *(u32*)((u8*)frm + 4)) != 0) {
            *out = -2;
            return 0;
        }
    }
    if (*(s32*)((u8*)frm + 0x18) == 1) {
        *(s32*)((u8*)shc + 0xfc) = 0;
    } else if (*(s32*)((u8*)self + 0x40) == 3) {
        void* prev = *(void**)((u8*)shc + 0xec);
        if (prev != 0 && *(s32*)((u8*)frm + 0x18) == 2) {
            if (*(s32*)((u8*)frm + 0x14) < *(s32*)((u8*)prev + 0x7c) && *(s32*)((u8*)prev + 0x7c) < 0x200) {
                *(s32*)((u8*)shc + 0xfc) = 1;
            }
        } else if (prev != 0 && *(s32*)((u8*)frm + 0x18) == 3) {
            if (*(s32*)((u8*)frm + 0x14) >= *(s32*)((u8*)prev + 0x7c)) {
                *(s32*)((u8*)shc + 0xfc) = 1;
            }
        }
    }
    MPV_GetPicUsr(mpv, 0, (u32*)((u8*)self + 0x3908));
    if (*(s32*)((u8*)frm + 0x30) != *(s32*)((u8*)shc + 0x94)) {
        *(s32*)((u8*)shc + 0x94) = *(s32*)((u8*)frm + 0x30);
        *(s32*)((u8*)shc + 0x98) = 1;
    } else {
        *(s32*)((u8*)shc + 0x98) = 0;
    }
    if ((pat & 0x40) != 0) {
        void* cb = *(void**)((u8*)self + 0xd64);
        void* arg = *(void**)((u8*)self + 0xd68);
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
        f0 = *(s32*)((u8*)shc + 0x98);
        pts0 = -1;
        pts1 = -1;
        if (r != 0) {
            s32 rate2;
            SFPTS_ReadPtsQue(self, *(s32*)((u8*)self + 0x2070), r, pque);
            rate2 = lbl_eu_8051CBF8[*(s32*)((u8*)frm + 0x10)];
            if (*(s32*)((u8*)self + 0x88) != 0 && *(s32*)((u8*)self + 0xf4) != -1) {
                rate2 = *(s32*)((u8*)self + 0xf4);
            }
            pts = sfmpv_ComplementPts((u8*)self + 0xd98, (u32*)((u8*)shc + 0xa0), frm, pque, f0, &pts0, rate2);
        }
        *(s32*)((u8*)shc + 0x104) = pts1;
        *(s32*)((u8*)shc + 0x100) = pts0;
        if ((pat & *(s32*)((u8*)shc + 8)) == 0) {
            return 0;
        }
    }
    sfmpv_CalcRepeatField(self, frm, *(s32*)((u8*)shc + 0x98));
    rate = *(s32*)((u8*)shc + 0x98);
    c0 = SFSET_GetCond(self, 0x34);
    if (c0 == 0) {
        s32 set = 0;
        if (pts >= 0 || *(s32*)((u8*)frm + 0x30) == 0 || *(s8*)((u8*)frm + 0x57) != 0) {
            set = 1;
        } else if (rate != 0 && *(s32*)((u8*)self + 0xe00) != 0) {
            s32 a;
            s32 b;
            s32 c;
            tc[0] = *(s32*)((u8*)self + 0xdb4);
            tc[1] = *(s32*)((u8*)self + 0xdb8);
            tc[2] = *(s32*)((u8*)self + 0xdbc);
            tc[3] = *(s32*)((u8*)self + 0xdc0);
            tc[4] = *(s32*)((u8*)self + 0xdc4);
            tc[5] = *(s32*)((u8*)self + 0xdc8);
            tc[6] = *(s32*)((u8*)self + 0xdcc);
            tc[7] = *(s32*)((u8*)self + 0xdd0);
            /* first call: out1->c, out2 shared; second: out1->a, out2->b.
             * set when c > b and c < b + a * cond(0x35) */
            SFTIM_Tc2Time(tc, &c, &b);
            SFTIM_Tc2Time((u8*)self + 0xe04, &a, &b);
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
    if (*(s32*)((u8*)self + 0xdd4) == 0) {
        s32 o1;
        s32 o2;
        tc[0] = *(s32*)((u8*)self + 0xdb4);
        tc[1] = *(s32*)((u8*)self + 0xdb8);
        tc[2] = *(s32*)((u8*)self + 0xdbc);
        tc[3] = *(s32*)((u8*)self + 0xdc0);
        tc[4] = *(s32*)((u8*)self + 0xdc4);
        tc[5] = *(s32*)((u8*)self + 0xdc8);
        tc[6] = 0;
        tc[7] = *(s32*)((u8*)self + 0xdd0);
        SFTIM_Tc2Time(tc, &o1, &o2);
        /* dd4 = valid flag, dd8..df4 = tc[0..7], df8/dfc = converted time.
         * Store order mirrors retail (pairs swapped). */
        *(s32*)((u8*)self + 0xddc) = tc[1];
        *(s32*)((u8*)self + 0xdd8) = tc[0];
        *(s32*)((u8*)self + 0xde4) = tc[3];
        *(s32*)((u8*)self + 0xde0) = tc[2];
        *(s32*)((u8*)self + 0xdec) = tc[5];
        *(s32*)((u8*)self + 0xde8) = tc[4];
        *(s32*)((u8*)self + 0xdf4) = tc[7];
        *(s32*)((u8*)self + 0xdf0) = tc[6];
        *(s32*)((u8*)self + 0xdf8) = o1;
        *(s32*)((u8*)self + 0xdfc) = o2;
        *(s32*)((u8*)self + 0xdd4) = 1;
    }
    {
        s32 o1;
        s32 o2;
        s32 delta;
        tc[0] = *(s32*)((u8*)self + 0xdb4);
        tc[1] = *(s32*)((u8*)self + 0xdb8);
        tc[2] = *(s32*)((u8*)self + 0xdbc);
        tc[3] = *(s32*)((u8*)self + 0xdc0);
        tc[4] = *(s32*)((u8*)self + 0xdc4);
        tc[5] = *(s32*)((u8*)self + 0xdc8);
        tc[6] = *(s32*)((u8*)self + 0xdcc);
        tc[7] = *(s32*)((u8*)self + 0xdd0);
        SFTIM_Tc2Time(tc, &o1, &o2);
        /* e58 = valid flag, e5c..e78 = tc[0..7], e7c/e80 = converted time */
        *(s32*)((u8*)self + 0xe60) = tc[1];
        *(s32*)((u8*)self + 0xe5c) = tc[0];
        *(s32*)((u8*)self + 0xe68) = tc[3];
        *(s32*)((u8*)self + 0xe64) = tc[2];
        *(s32*)((u8*)self + 0xe70) = tc[5];
        *(s32*)((u8*)self + 0xe6c) = tc[4];
        *(s32*)((u8*)self + 0xe78) = tc[7];
        *(s32*)((u8*)self + 0xe74) = tc[6];
        delta = o1 - *(s32*)((u8*)self + 0xdf8);
        *(s32*)((u8*)self + 0xe7c) = delta;
        *(s32*)((u8*)self + 0xe80) = o2;
        *(s32*)((u8*)self + 0xe58) = 1;
        if (*(s32*)((u8*)self + 0xe24) <= delta) {
            *(s32*)((u8*)self + 0xe00) = 1;
            *(s32*)((u8*)self + 0xe04) = tc[0];
            *(s32*)((u8*)self + 0xe08) = tc[1];
            *(s32*)((u8*)self + 0xe0c) = tc[2];
            *(s32*)((u8*)self + 0xe10) = tc[3];
            *(s32*)((u8*)self + 0xe14) = tc[4];
            *(s32*)((u8*)self + 0xe18) = tc[5];
            *(s32*)((u8*)self + 0xe1c) = tc[6];
            *(s32*)((u8*)self + 0xe20) = tc[7];
            *(s32*)((u8*)self + 0xe24) = delta;
            *(s32*)((u8*)self + 0xe28) = o2;
        }
    }
    ret = sfmpv_FirstPicAtr(self, mpv, frm, pic);
    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_80568C18[3] = (u32)((u8*)frm + 0x18);
        lbl_eu_80568C18[0x18 / 4] = (u32)((u8*)frm + 0x30);
        lbl_eu_80568C18[0x24 / 4] = (u32)((u8*)frm + 0x68);
        lbl_eu_80568C18[0x30 / 4] = (u32)((u8*)self + 0xe7c);
        (*(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24))(lbl_eu_80606E34, &lbl_eu_80568C18[1]);
    }
    return ret;
}

// ---------------------------------------------------------------------------
// sfmpv_ComplementPts
// ---------------------------------------------------------------------------
s64 sfmpv_ComplementPts(void* self, u32* dst, void* frm, u32* pts, s32 mode, s32* out1, s32 rate) {
    s64 ptsv;
    s64 d;
    s32 frm14 = *(s32*)((u8*)frm + 0x14);

    /* negative accumulated PTS: re-seed it from the frame duration */
    if (*(s64*)((u8*)self + 0x150) < 0) {
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
        *(u32*)((u8*)self + 0x154) = (u32)d;
        *(u32*)((u8*)self + 0x150) = (u32)(d >> 32);
    }

    {
        ptsv = *(s64*)pts;
        if (ptsv >= 0 && memcmp((u8*)dst + 0x10, pts, 4) == 0) {
            /* same PTS as the saved stamp: restart the run */
            s64 p = ptsv - *(s64*)((u8*)self + 0x150);
            if (p < 0) {
                p = 0;
            }
            dst[4] = pts[0];
        dst[5] = pts[1];
        dst[6] = pts[2];
        dst[7] = pts[3];
        dst[1] = 0;
        dst[0] = frm14;
        dst[2] = (*(s32*)((u8*)frm + 0x18) == 3) ? 1 : 0;
            out1[0] = pts[0];
            out1[1] = pts[1];
            return p;
        } else {
            /* continuation: advance by the frame count scaled back to PTS units */
            s64 p = *(s64*)((u8*)dst + 0x10) - *(s64*)((u8*)self + 0x150);
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
            if (p < 0) {
                p = 0;
            }
            return p;
        }
    }
}

// ---------------------------------------------------------------------------
// sfmpv_CalcRepeatField
// ---------------------------------------------------------------------------
void sfmpv_CalcRepeatField(void* self, void* frm, s32 mode) {
    s16* tbl = (s16*)((u8*)self + 0x1268);
    s32 t1;
    s32 t2;
    s32 i;
    s32 w;
    s32 h;
    s32 n;
    s32 sv;
    s16* slot;
    void* p;
    void* shc;
    *(s32*)((u8*)self + 0xdb4) = *(s32*)((u8*)frm + 0x10);
    shc = *(void**)((u8*)self + 0x2068);
    *(s32*)((u8*)self + 0xdb8) = *(s32*)((u8*)frm + 0x1c);
    *(s32*)((u8*)self + 0xdbc) = *(s32*)((u8*)frm + 0x20);
    *(s32*)((u8*)self + 0xdc0) = *(s32*)((u8*)frm + 0x24);
    *(s32*)((u8*)self + 0xdc4) = *(s32*)((u8*)frm + 0x28);
    *(s32*)((u8*)self + 0xdc8) = *(s32*)((u8*)frm + 0x2c);
    *(s32*)((u8*)self + 0xdcc) = *(s32*)((u8*)frm + 0x14);
    *(s16*)((u8*)self + 0xdd0) = *(s8*)((u8*)frm + 0x54);
    *(s16*)((u8*)self + 0xdd2) = 0;
    if (mode != 0) {
        for (i = 0; i < 0x80; i++) {
            tbl[i] = -1;
        }
        tbl[1] = -1;
        return;
    }
    if ((u32)(*(s32*)((u8*)frm + 0x18) - 1) <= 1) {
        p = *(void**)((u8*)shc + 0xec);
        if (p != NULL) {
            w = *(s32*)((u8*)frm + 0x14);
            h = *(s32*)((u8*)p + 0x7c);
            if (w < h) {
                w += 0x400;
            }
            for (n = h + 1; n < w; n++) {
                tbl[(n % 64) * 2] = -1;
            }
        }
    }
    slot = &tbl[(*(s32*)((u8*)frm + 0x14) % 64) * 2];
    slot[0] = *(s16*)((u8*)self + 0xdd0);
    if (mode != 0) {
        slot[1] = 0;
    } else if (*(s32*)((u8*)frm + 0x14) == 0) {
        if (tbl[1] == -1) {
            tbl[1] = 0;
        }
    } else {
        /* walk back up to 16 slots; on failure the slot is left untouched */
        for (i = 0; i < 0x10; i++) {
            s16* q = &tbl[((*(s32*)((u8*)frm + 0x14) - i + 0x3f) % 64) * 2];
            if (q[0] != -1) {
                slot[1] = q[0] + q[1];
                break;
            }
        }
    }
    *(s16*)((u8*)self + 0xdd2) = slot[1];
    if (*(s32*)((u8*)frm + 0x18) == 3) {
        if (slot[0] != 0) {
            if (*(void**)((u8*)shc + 0xec) != NULL) {
                w = *(s32*)((u8*)(*(void**)((u8*)shc + 0xec)) + 0x7c);
                sv = slot[0] + slot[1];
                tbl[(w % 64) * 2 + 1] = sv;
                /* NOTE: deliberately re-reads the prev-frame pointer around the
                 * time conversion to match retail load placement */
                p = *(void**)((u8*)shc + 0xec);
                *(s16*)((u8*)p + 0x3a) = sv;
                p = *(void**)((u8*)shc + 0xec);
                SFTIM_Tc2Time((u8*)p + 0x1c, &t1, &t2);
                p = *(void**)((u8*)shc + 0xec);
                *(s32*)((u8*)p + 0x3c) = t1 - *(s32*)((u8*)self + 0xdf8);
                *(s32*)((u8*)p + 0x40) = t2;
                *(s32*)((u8*)p + 0x18) = 1;
                if (*(s32*)((u8*)self + 0xe24) <= *(s32*)((u8*)p + 0x3c)) {
                    *(s32*)((u8*)self + 0xe00) = *(s32*)((u8*)p + 0x18);
                    *(s32*)((u8*)self + 0xe04) = *(s32*)((u8*)p + 0x1c);
                    *(s32*)((u8*)self + 0xe08) = *(s32*)((u8*)p + 0x20);
                    *(s32*)((u8*)self + 0xe0c) = *(s32*)((u8*)p + 0x24);
                    *(s32*)((u8*)self + 0xe10) = *(s32*)((u8*)p + 0x28);
                    *(s32*)((u8*)self + 0xe14) = *(s32*)((u8*)p + 0x2c);
                    *(s32*)((u8*)self + 0xe18) = *(s32*)((u8*)p + 0x30);
                    *(s32*)((u8*)self + 0xe1c) = *(s32*)((u8*)p + 0x34);
                    *(s32*)((u8*)self + 0xe20) = *(s32*)((u8*)p + 0x38);
                    *(s32*)((u8*)self + 0xe24) = *(s32*)((u8*)p + 0x3c);
                    *(s32*)((u8*)self + 0xe28) = *(s32*)((u8*)p + 0x40);
                }
                p = *(void**)((u8*)shc + 0xec);
                *(s32*)((u8*)p + 0x48) = *(s32*)((u8*)p + 0x40);
                n = *(s32*)((u8*)self + 0xefc) + (*(s32*)((u8*)p + 0x3c) - *(s32*)((u8*)self + 0xed4));
                *(s32*)((u8*)p + 0x44) = n;
                *(s32*)((u8*)p + 0x58) = *(s32*)((u8*)p + 0x3c);
                *(s32*)((u8*)p + 0x5c) = *(s32*)((u8*)p + 0x3c) + *(s32*)((u8*)self + 0xefc);
                if (*(s32*)((u8*)self + 0x1020) < n) {
                    *(s32*)((u8*)self + 0x1020) = n;
                    *(s32*)((u8*)self + 0x1024) = *(s32*)((u8*)p + 0x48);
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// sfmpv_DoReformTc
// ---------------------------------------------------------------------------
void sfmpv_DoReformTc(void* self, void* frm, s64 pts, s32 rep) {
    u8* tc = (u8*)self + 0xd98;
    s32 v;
    s32 t;
    s32 u;
    s16 sv;
    v = *(s32*)((u8*)frm + 0x10);
    t = *(s32*)((u8*)frm + 0x1c);
    u = *(s32*)((u8*)frm + 0x14);
    if (pts >= 0) {
        sfmpv_Pts2Tc(pts, v, t, u, (u32*)(tc + 0x1c));
    } else if (*(s32*)(tc + 0x68) == 0) {
        if (*(s32*)((u8*)self + 0x2670) == 0) {
            *(u32*)(tc + 0x1c) = (u32)v;
            *(u32*)(tc + 0x20) = 0;
            *(u32*)(tc + 0x24) = 0;
            *(u32*)(tc + 0x28) = 0;
            *(u32*)(tc + 0x2c) = 0;
            *(u32*)(tc + 0x30) = 0;
        } else {
            return;
        }
    } else if (rep != 0) {
        sfmpv_NextTc(tc + 0x6c, tc + 0x1c);
        sv = *(s16*)(tc + 0x3a);
        *(s16*)(tc + 0x4d2) = sv;
        *(s16*)(tc + 0x4d2 + (u % 64) * 4) = sv;
    } else {
        *(u32*)(tc + 0x1c) = *(u32*)(tc + 0x6c);
        *(u32*)(tc + 0x20) = *(u32*)(tc + 0x70);
        *(u32*)(tc + 0x24) = *(u32*)(tc + 0x74);
        *(u32*)(tc + 0x28) = *(u32*)(tc + 0x78);
        *(u32*)(tc + 0x2c) = *(u32*)(tc + 0x7c);
        *(u32*)(tc + 0x30) = *(u32*)(tc + 0x80);
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
    const s32* rateTbl = (const s32*)lbl_eu_8051CBF8;
    const s32* tbl = (const s32*)lbl_eu_8051C940;
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
    t0 = tbl[15 + v];
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
            p = tbl + 24;
        } else {
            p = tbl + 32;
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
void sfmpv_NextTc(void* in, void* out) {
    s32 t8 = (s32)*(s16*)((u8*)in + 0x1e);
    s32 t7 = (s32)*(s16*)((u8*)in + 0x1c);
    s32 t0 = *(s32*)((u8*)in + 0x00);
    s32 sum = t8 + t7;
    s32 t5 = *(s32*)((u8*)in + 0x14);
    s32 t6 = *(s32*)((u8*)in + 0x18);
    s32 rate = lbl_eu_8051C97C[t0];
    s32 a = t5 + t6 + 1 + sum / 2;
    s32 s = sum % 2;
    s32 t1 = *(s32*)((u8*)in + 0x04);
    s32 d = a / rate;
    s32 e = *(s32*)((u8*)in + 0x10) + d;
    s32 f = *(s32*)((u8*)in + 0x0c) + e / 60;
    s32 q2 = f / 60;
    s32 t2 = *(s32*)((u8*)in + 0x08);
    s32 t3rem = a - d * rate;
    s32 frem = f % 60;
    s32 erem = e % 60;
    /* drop-frame guard: branch chain skips the +2 when any carry term hits */
    if (t1 != 0 && erem != 0 && frem % 10 != 0 && (u32)t3rem <= 1) {
        t3rem = 2;
    }
    *(s32*)((u8*)out + 0x00) = t0;
    *(s32*)((u8*)out + 0x04) = *(s32*)((u8*)in + 0x04);
    *(s32*)((u8*)out + 0x08) = t2 + q2;
    *(s32*)((u8*)out + 0x0c) = frem;
    *(s32*)((u8*)out + 0x10) = erem;
    *(s32*)((u8*)out + 0x14) = t3rem;
    *(s16*)((u8*)out + 0x1e) = (s16)s;
}

// ---------------------------------------------------------------------------
// sfmpv_FirstPicAtr
// ---------------------------------------------------------------------------
// NOTE: residual 5-instruction rotation vs retail (addi r30 dst before the four
// mr param copies in the prologue; decomp emits the copies first). Exact size /
// zero reloc drift; resists declaration/assignment order, C99 mixed decls, temp
// locals. br/vb temps below are required to reproduce retail's load hoisting
// (lwz vbv/bitrate + ChkBufSiz arg setup before the 5-word copy loop).
s32 sfmpv_FirstPicAtr(void* self, void* mpv, void* frm, void* pic) {
    void* shc = *(void**)((u8*)self + 0x2068);
    u32* dst = (u32*)((u8*)self + 0x91c);
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
        *(u32*)((u8*)shc + 0x9c) = 0;
    } else {
        s32 ring = (s32)SFBUF_GetRingBufSiz(self, 1);
        if (max == -1) {
            max = vbv;
        }
        *(u32*)((u8*)shc + 0x9c) = (u32)((max < ring) ? max : ring);
    }
    sfmpv_SetMpvHd(self, bitrate, pic);
    {
        s32 br;
        s32 vb;
        vb = vbv;
        br = bitrate;
        dst[0] = ((u32*)frm)[0];
        dst[1] = ((u32*)frm)[1];
        dst[2] = ((u32*)frm)[2];
        dst[3] = ((u32*)frm)[3];
        dst[5] = ((u32*)frm)[4];
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
void sfmpv_SetMpvHd(void* self, s32 bitrate, u32* pic) {
    void* p;
    void* shc;
    s32 v;
    s32 n;
    v = *(s32*)((u8*)self + 0x2670);
    shc = *(void**)((u8*)self + 0x2068);
    /* header slot: usable only when a decode session exists and frame count is
     * still 0 (retail branches: v==0 -> NULL, cnt>0 -> NULL, else v+0xad0) */
    if (v == 0) {
        p = NULL;
    } else if (*(s32*)((u8*)shc + 0x10) > 0) {
        p = NULL;
    } else {
        p = (u8*)v + 0xad0;
    }
    if (p == NULL || *(u32*)p != 0) {
        return;
    }
    {
        s32 sz = (s32)pic[1];
        n = 0x200;
        if (sz < 0x200) {
            n = sz;
        }
    }
    *(u32*)((u8*)p + 0x238) = n;
    MEM_Copy((u8*)p + 0x38, (void*)pic[0], n);
    if ((u32)(bitrate - 0x30000) == 0xffff) {
        *(s32*)((u8*)p + 4) = 0;
        *(s32*)((u8*)p + 8) = 0;
    } else {
        *(s32*)((u8*)p + 4) = bitrate * 0x32;
        *(s32*)((u8*)p + 8) = 1;
    }
    *(u32*)((u8*)p + 0xc) = *(u32*)((u8*)self + 0xdd4);
    *(u32*)((u8*)p + 0x10) = *(u32*)((u8*)self + 0xdd8);
    *(u32*)((u8*)p + 0x14) = *(u32*)((u8*)self + 0xddc);
    *(u32*)((u8*)p + 0x18) = *(u32*)((u8*)self + 0xde0);
    *(u32*)((u8*)p + 0x1c) = *(u32*)((u8*)self + 0xde4);
    *(u32*)((u8*)p + 0x20) = *(u32*)((u8*)self + 0xde8);
    *(u32*)((u8*)p + 0x24) = *(u32*)((u8*)self + 0xdec);
    *(u32*)((u8*)p + 0x28) = *(u32*)((u8*)self + 0xdf0);
    *(u32*)((u8*)p + 0x2c) = *(u32*)((u8*)self + 0xdf4);
    *(u32*)((u8*)p + 0x30) = *(u32*)((u8*)self + 0xdf8);
    *(u32*)((u8*)p + 0x34) = *(u32*)((u8*)self + 0xdfc);
    *(u32*)p = 1;
}

// ---------------------------------------------------------------------------
// sfmpv_ChkBufSiz
// ---------------------------------------------------------------------------
// Validates/recomputes the frame-memory layout for the requested picture size.
// Macroblock dims come from self+0x2788/0x278c, the new request from para.
// size1/size2 are per-frame luma/chroma byte budgets; the search loop finds how
// many frames of the new budget fit into n old budgets (capped at 16).
s32 sfmpv_ChkBufSiz(void* self, void* para) {
    s32 a = (*(s32*)((u8*)self + 0x2788) + 15) / 16;
    /* frame-object array base, kept live across the whole call */
    u8* frm = (u8*)self + 0x27f8;
    s32 b = (*(s32*)((u8*)self + 0x278c) + 15) / 16;
    s32 va = a * 16;
    s32 vb = b * 16;
    s32 ta = (a >> 4) & 1;
    s32 tb = (b >> 4) & 1;
    s32 sb = (vb + tb) >> 1;
    s32 n = *(s32*)((u8*)self + 0x279c);
    s32 sa = (va + ta) >> 1;
    void* shc = *(void**)((u8*)self + 0x2068);
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
    base = *(u32*)((u8*)self + 0x27a0);
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
        *(s32*)((u8*)self + 0x27a4) = *(s32*)((u8*)self + 0x2790);
        *(s32*)((u8*)self + 0x27a8) = *(s32*)((u8*)self + 0x2790) + size2;
        if (cnt > 0) {
            u32* q = (u32*)((u8*)self + 0x27ac);
            for (i = 0; i < cnt; i++) {
                q[i] = base + i * size2;
            }
        }
    }
    {
        s32 ww = (vp + 31) / 32;
        s32 half = vp / 2;
        s32 hh = (half + 31) / 32;
        v = *(u32*)((u8*)self + 0x27a4);
        m1 = vq * (ww * 32);
        m2 = (vq / 2) * (hh * 32);
        *(s16*)((u8*)shc + 0xd6) = ww * 32;
        *(s16*)((u8*)shc + 0xd4) = hh * 32;
        *(u32*)((u8*)shc + 0xd0) = v;
        *(u32*)((u8*)shc + 0xc8) = v + m1;
        *(u32*)((u8*)shc + 0xcc) = v + m1 + m2;
        *(s16*)((u8*)shc + 0xe6) = ww * 32;
        *(s16*)((u8*)shc + 0xe4) = hh * 32;
        e0 = *(u32*)((u8*)self + 0x27a8);
        *(u32*)((u8*)shc + 0xe0) = e0;
        *(u32*)((u8*)shc + 0xd8) = e0 + m1;
        *(u32*)((u8*)shc + 0xdc) = e0 + m1 + m2;
    }
    if (*(s32*)((u8*)self + 0x40) == 3) {
        s32 m = 14;
        if (n < 14) {
            m = n;
        }
        *(s32*)((u8*)self + 0x27ec) = m + 2;
        sfmpv_InitFrmObj(frm, (u32*)((u8*)self + 0x27a4), 2);
        sfmpv_InitFrmObj(frm + 0x220, (u32*)((u8*)self + 0x27ac), m);
    } else {
        s32 m = 16;
        if (n < 16) {
            m = n;
        }
        *(s32*)((u8*)self + 0x27ec) = m;
        sfmpv_InitFrmObj(frm, (u32*)((u8*)self + 0x27ac), m);
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_IsSkip
// ---------------------------------------------------------------------------
s32 sfmpv_IsSkip(void* self, void* bpic) {
    /* Single work slot: holds the shc pointer, then the pic type, then the verdict.
       Retail keeps all three in one register (r29). */
    void* wk = *(void**)((u8*)self + 0x2068);
    u8* frm = (u8*)wk + 0x14;
    u32 skip;
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
    if (*(s8*)(frm + 0x58)) {
        return *(s32*)((u8*)wk + 0xf4);
    }
    /* deadline check: current tc valid, no limiter active, and tc not past deadline */
    {
        s32 tc = *(s32*)((u8*)self + 0x2678);
        s32 r = 0;
        wk = (void*)(uintptr_t)*(s32*)(frm + 0x18);
        if (tc >= 0 && *(s32*)((u8*)self + 0xeb0) == 0 &&
            UTY_CmpTime(tc, *(s32*)((u8*)self + 0x267c), *(s32*)((u8*)self + 0xe7c), *(s32*)((u8*)self + 0xe80)) == 0) {
            r = 1;
        }
        if (r != 0) {
            void (*fn)(void*, void*);
            if (lbl_eu_80606E34 != NULL) {
                lbl_eu_80568CF0[3] = (u32)self;
                lbl_eu_80568CF0[0x18 / 4] = (u32)lbl_eu_8051C9E0;
                fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
                fn(lbl_eu_80606E34, &lbl_eu_80568CF0[1]);
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
            cond = (*(s32*)((u8*)self + 0xa24) == 0);
            break;
        case 2:
            cond = (*(s32*)((u8*)self + 0xa28) == 0);
            break;
        case 3:
            cond = (*(s32*)((u8*)self + 0xa2c) == 0);
            break;
        default:
            cond = 1;
            break;
        }
        if (cond) {
            void (*fn)(void*, void*);
            if (lbl_eu_80606E34 != NULL) {
                lbl_eu_80568CF0[3] = (u32)self;
                lbl_eu_80568CF0[0x18 / 4] = (u32)&lbl_eu_8051C9E0[0x11];
                fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
                fn(lbl_eu_80606E34, &lbl_eu_80568CF0[1]);
            }
            wk = (void*)1;
            goto out;
        }
    }
    if (sfmpv_IsEmptyBpic(self, (s32)wk, bpic) != 0) {
        void (*fn)(void*, void*);
        if (lbl_eu_80606E34 != NULL) {
            lbl_eu_80568CF0[3] = (u32)self;
            lbl_eu_80568CF0[0x18 / 4] = (u32)&lbl_eu_8051C9E0[0x17];
            fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
            fn(lbl_eu_80606E34, &lbl_eu_80568CF0[1]);
        }
        wk = (void*)1;
        goto out;
    }
    {
        s32 t = *(s32*)((u8*)((SfMpvTcView*)self)->shc + 4);
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
            if (lbl_eu_80606E34 != NULL) {
                lbl_eu_80568CF0[3] = (u32)self;
                lbl_eu_80568CF0[0x18 / 4] = (u32)&lbl_eu_8051C9E0[0x1f];
                fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
                fn(lbl_eu_80606E34, &lbl_eu_80568CF0[1]);
            }
            wk = (void*)1;
            goto out;
        }
        if (sfmpv_IsLate(self, (s32)wk) != 0) {
            void (*fn)(void*, void*);
            if (lbl_eu_80606E34 != NULL) {
                lbl_eu_80568CF0[3] = (u32)self;
                lbl_eu_80568CF0[0x18 / 4] = (u32)&lbl_eu_8051C9E0[0x26];
                fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
                fn(lbl_eu_80606E34, &lbl_eu_80568CF0[1]);
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
void sfmpv_UpdateDefect(void* self, void* frm, s32 mode) {
    void* shc = *(void**)((u8*)self + 0x2068);
    void* mpv = *(void**)shc;
    s32 flg = *(s32*)((u8*)shc + 4);
    if (*(s32*)((u8*)shc + 0x98) != 0) {
        s32 a;
        s32 b;
        MPV_GetLinkFlg(mpv, (u32*)&a, (u32*)&b);
        if (a == 1) {
            flg = 5;
        } else {
            if (*(s32*)((u8*)self + 0xeb0) == 0 && SFSET_GetCond(self, 0x49) == 1) {
                b = 1;
            }
            if (b == 1) {
                flg = 2;
            }
        }
    }
    if (mode == 1) {
        u32 v = *(u32*)((u8*)frm + 0x18);
        if (v - 1 <= 1) {
            flg = 2;
        }
    }
    *(s32*)((u8*)shc + 4) = flg;
}

// ---------------------------------------------------------------------------
// sfmpv_IsEmptyBpic
// ---------------------------------------------------------------------------
s32 sfmpv_IsEmptyBpic(void* self, s32 type, void* bpic) {
    if (SFSET_GetCond(self, 7) != 0) {
        return 0;
    }
    if (type == 3) {
        s32 r = MPV_IsEmptyBpic(*(u32*)bpic, *(u32*)((u8*)bpic + 4),
                                *(u32*)((u8*)self + 0x924) * *(u32*)((u8*)self + 0x928));
        if (r != 0) {
            *(u32*)((u8*)self + 0x970) += 1;
        }
        return r;
    }
    if (type == 2) {
        s32 r = MPV_IsEmptyPpic(*(u32*)bpic, *(u32*)((u8*)bpic + 4),
                                *(u32*)((u8*)self + 0x924) * *(u32*)((u8*)self + 0x928));
        if (r != 0) {
            *(u32*)((u8*)self + 0x974) += 1;
        }
        return r;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_IsLate
// ---------------------------------------------------------------------------
s32 sfmpv_IsLate(void* self, s32 type) {
    s32 t;
    s32 e8v;
    void* tc;
    void* shc = *(void**)((u8*)self + 0x2068);
    void* fn;
    s32 r;
    s32 c;
    s32 d;
    s32 a;
    s32 b;
    tc = (u8*)self + 0xd98;
    if (*(s32*)((u8*)tc + 0x118) == 0) {
        t = 0;
    } else {
        t = (*(s32*)((u8*)tc + 0xe4) - *(s32*)((u8*)tc + 0x13c)) + *(s32*)((u8*)tc + 0x164);
    }
    fn = *(void**)((u8*)tc + 0x18);
    e8v = *(s32*)((u8*)tc + 0xe8);
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
    if (a <= b && *(s32*)((u8*)shc + 0xc) >= *(s32*)((u8*)self + 0xab4)) {
        return 0;
    }
    SFTIM_GetTime(self, &c, &d);
    if (c < 0) {
        return 0;
    }
    r = UTY_CmpTime(c, d, t - (e8v * *(s32*)((u8*)self + 0xac4)) / *(s32*)((u8*)self + 0xac8), e8v);
    if (r != 0) {
        return 0;
    }
    *(s32*)((u8*)shc + 0xc) += 1;
    return 1;
}

// ---------------------------------------------------------------------------
// sfmpv_SkipFrm
// ---------------------------------------------------------------------------
s32 sfmpv_SkipFrm(void* self, void* sj) {
    /* e58/e84 hold identical 11-word tc-save blocks */
    SfMpvTcView* v = (SfMpvTcView*)self;
    void* shc;
    void* mpv;
    s32 cur;
    s32 lim;
    TCBlockView* dst;
    TCBlockView* src;
    shc = v->shc;
    mpv = *(void**)shc;
    cur = v->cur_tc.tm;
    lim = v->lim;
    dst = &v->prv_tc;
    src = &v->cur_tc;
    u32 a;
    u32 b;
    s32 r;
    s32 n;
    if (cur < lim) {
        /* staged save: retail hoists every load above every store */
        dst->w[0] = src->w[0];
        dst->w[1] = src->w[1];
        dst->w[2] = src->w[2];
        dst->w[3] = src->w[3];
        dst->w[4] = src->w[4];
        dst->w[5] = src->w[5];
        dst->w[6] = src->w[6];
        dst->w[7] = src->w[7];
        dst->w[8] = src->w[8];
        dst->tm = cur;
        dst->fx = src->fx;
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
    SFBUF_AddRtotSj(self, *(s32*)((u8*)self + 0x2070), n);
    *(s64*)((u8*)self + 0x9b8) += n;
    if (n > 0) {
        *(s32*)((u8*)self + 0x50) = 1;
    }
    if (r != 0) {
        return r;
    }
    if (*(s8*)((u8*)shc + 0x6c) == 0) {
        *(s32*)((u8*)shc + 0xf4) = 1;
    }
    SFPLY_AddSkipPic(self, 1, *(s32*)((u8*)shc + 0x2c));
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_DecodeFrm
// ---------------------------------------------------------------------------
s32 sfmpv_DecodeFrm(void* self, void* sj) {
    void (*fn)(void*, void*);
    void* shc = *(void**)((u8*)self + 0x2068);
    void* mpv = *(void**)shc;
    void* frm = (u8*)shc + 0x14;
    void* out;
    s32 r;
    s32 n;
    s32 ret = 0;
    u32 fcnt[2];
    s32 a;
    s32 b;
    s64 t1;
    s64 t2;
    if (sfmpv_SetFrmPara(self, frm, fcnt, &out) != 0) {
        return 0;
    }
    fn_803C9948(self, shc, &a, &b);
    memcpy(*(void**)((u8*)frm + 0x60), *(void**)((u8*)self + 0x3900), *(s32*)((u8*)self + 0x3904));
    *(s32*)((u8*)*(void**)((u8*)frm + 0x60) + 4) = *(s32*)((u8*)self + 0x3908);
    fn_803C99C8(self);
    t1 = SFTMR_GetTmr(self);
    a = SJRBF_GetFlowCnt(sj, 0, 1);
    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_80568B44[3] = (u32)mpv;
        lbl_eu_80568B44[0x18 / 4] = (u32)((u8*)frm + 0x18);
        lbl_eu_80568B44[0x24 / 4] = (u32)((u8*)frm + 0x30);
        lbl_eu_80568B44[0x30 / 4] = (u32)((u8*)frm + 0x68);
        fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
        fn(lbl_eu_80606E34, &lbl_eu_80568B44[1]);
    }
    r = MPV_DecodeFrmSj(mpv, sj, fcnt);
    b = SJRBF_GetFlowCnt(sj, 0, 1);
    n = b - a;
    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_80568B44[0x74 / 4] = (u32)&n;
        lbl_eu_80568B44[0x80 / 4] = (u32)&fcnt[0];
        lbl_eu_80568B44[0x8c / 4] = (u32)&fcnt[1];
        lbl_eu_80568B44[0x98 / 4] = (u32)&r;
        fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
        fn(lbl_eu_80606E34, &lbl_eu_80568B44[0x6c / 4]);
    }
    t2 = SFTMR_GetTmr(self);
    SFTMR_AddTsum((u8*)self + 0x26a0 + *(s32*)((u8*)frm + 0x18) * 0x20, t2 - t1);
    *(s32*)((u8*)self + 0xa14) += fcnt[0];
    *(s32*)((u8*)self + 0xa18) += fcnt[1];
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
    SFBUF_AddRtotSj(self, *(s32*)((u8*)self + 0x2070), n);
    *(s64*)((u8*)self + 0x9b8) += n;
    if (n > 0) {
        *(s32*)((u8*)self + 0x50) = 1;
    }
    if (ret != 0) {
        SFMPVF_FreeFrm(out);
        return ret;
    }
    if (n > 0) {
        *(s32*)((u8*)out + 0x18) = *(s32*)((u8*)self + 0xe58);
        *(s32*)((u8*)out + 0x1c) = *(s32*)((u8*)self + 0xe5c);
        *(s32*)((u8*)out + 0x20) = *(s32*)((u8*)self + 0xe60);
        *(s32*)((u8*)out + 0x24) = *(s32*)((u8*)self + 0xe64);
        *(s32*)((u8*)out + 0x28) = *(s32*)((u8*)self + 0xe68);
        *(s32*)((u8*)out + 0x2c) = *(s32*)((u8*)self + 0xe6c);
        *(s32*)((u8*)out + 0x30) = *(s32*)((u8*)self + 0xe70);
        *(s32*)((u8*)out + 0x34) = *(s32*)((u8*)self + 0xe74);
        *(s32*)((u8*)out + 0x38) = *(s32*)((u8*)self + 0xe78);
        *(s32*)((u8*)out + 0x3c) = *(s32*)((u8*)self + 0xe7c);
        *(s32*)((u8*)out + 0x40) = *(s32*)((u8*)self + 0xe80);
        *(s32*)((u8*)out + 0x48) = *(s32*)((u8*)self + 0xe80);
        a = *(s32*)((u8*)self + 0xefc) + (*(s32*)((u8*)self + 0xe7c) - *(s32*)((u8*)self + 0xed4));
        *(s32*)((u8*)out + 0x44) = a;
        *(s32*)((u8*)out + 0x58) = *(s32*)((u8*)self + 0xe7c);
        *(s32*)((u8*)out + 0x5c) = *(s32*)((u8*)self + 0xe7c) + *(s32*)((u8*)self + 0xefc);
        if (*(s32*)((u8*)self + 0x1020) < a) {
            *(s32*)((u8*)self + 0x1020) = a;
            *(s32*)((u8*)self + 0x1024) = *(s32*)((u8*)out + 0x48);
        }
        *(s32*)((u8*)out + 0x54) = *(s32*)((u8*)shc + 0x10);
        *(s32*)((u8*)out + 0x4c) = a + *(s32*)((u8*)shc + 0xfc) + *(s32*)((u8*)shc + 0x10);
        *(s32*)((u8*)out + 0x50) = n + *(s32*)((u8*)shc + 0x10);
        *(s16*)((u8*)out + 0x100) = (s16)fcnt[0];
        if (*(s32*)((u8*)shc + 0xf0) == 0) {
            *(s32*)((u8*)shc + 4) = (*(s32*)((u8*)shc + 4) == 2) ? 3 : 5;
        }
        if (*(s32*)((u8*)frm + 0x38) == 3 || *(s32*)((u8*)shc + 0xf0) != 0) {
            *(s32*)((u8*)shc + 0xf0) = 0;
        } else {
            *(s32*)((u8*)shc + 0xf0) = (u32)out;
        }
        *(s32*)((u8*)shc + 0xf4) = 0;
        *(s32*)((u8*)shc + 0xf8) = 0;
        if (*(s32*)((u8*)shc + 0xf0) == 0) {
            if (*(s32*)((u8*)self + 0x40) == 3) {
                s32 t = *(s32*)((u8*)frm + 0x18);
                if (t >= 2 && t <= 3) {
                    SFMPVF_RefStbyFrm(out);
                } else {
                    SFMPVF_StbyFrm(out);
                }
            } else {
                SFMPVF_StbyFrm(out);
            }
            MPV_GetDctCnt(mpv, (u32*)((u8*)self + 0x968), (u32*)((u8*)self + 0x96c));
            *(s32*)((u8*)shc + 0xc) = 0;
        }
        SFPLY_AddDecPic(self, 1, *(s32*)((u8*)frm + 0x18));
        *(s32*)((u8*)self + 0x50) = 1;
    } else {
        if (*(s32*)((u8*)shc + 0xf0) == 0) {
            SFMPVF_FreeFrm(out);
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_SetFrmPara
// ---------------------------------------------------------------------------
s32 sfmpv_SetFrmPara(void* self, void* frm, void* para, void** out) {
    void* shc = *(void**)((u8*)self + 0x2068);
    if (*(s32*)((u8*)shc + 0xf0) != 0) {
        *out = *(void**)((u8*)shc + 0xf0);
    } else {
        *out = SFMPVF_AllocFrm(self);
        if (*out == NULL) {
            *(s32*)((u8*)self + 0x98c) = 1;
            /* notify error callback through its vtable slot 0x24 */
            if (lbl_eu_80606E34 != NULL) {
                lbl_eu_80568DC8[3] = (u32)self;
                ((void (*)(void*, void*))(*(void**)((u8*)*(void**)lbl_eu_80606E34 + 0x24)))
                    (lbl_eu_80606E34, &lbl_eu_80568DC8[1]);
            }
            return -1;
        }
    }
    {
        /* macroblock parameter words; 16 pairs copied with walking pointers */
        u32* src = (u32*)((u8*)frm - 4);
        *(u16*)((u8*)*out + 0xc) = *(u32*)((u8*)frm + 0);
        *(u16*)((u8*)*out + 0xe) = *(u32*)((u8*)frm + 4);
        *(u16*)((u8*)*out + 0x10) = *(u32*)((u8*)frm + 8);
        *(u16*)((u8*)*out + 0x12) = *(u32*)((u8*)frm + 0xc);
        *(u32*)((u8*)*out + 0x14) = *(u32*)((u8*)frm + 0x18);
        {
            s32 n = 0x10;
            u32* dst = (u32*)((u8*)*out + 0x64);
            do {
                dst[1] = src[1];
                src += 2;
                dst += 2;
                *dst = *src;
            } while (--n != 0);
        }
    }
    *(u32*)((u8*)*out + 0xec) = *(u32*)((u8*)shc + 0x104);
    *(u32*)((u8*)*out + 0xe8) = *(u32*)((u8*)shc + 0x100);
    *(u32*)((u8*)*out + 0xf0) = *(u32*)((u8*)frm + 0x34);
    *(u32*)((u8*)*out + 0xf4) = *(u32*)((u8*)frm + 0x30);
    *(u32*)((u8*)*out + 0xf8) = *(u32*)((u8*)frm + 0x68);
    *(u32*)((u8*)*out + 0xfc) = (*(s8*)((u8*)frm + 0x5f) - 1 == 0);
    if (*(s32*)((u8*)self + 0x88) != 0 && *(s32*)((u8*)self + 0xf8) != 0 && *(s32*)((u8*)self + 0x114) == 1) {
        *(u32*)((u8*)*out + 0xfc) = 1;
    }
    if (*(s32*)((u8*)self + 0x40) == 3) {
        if (*(u32*)((u8*)frm + 0x18) >= 2 && *(u32*)((u8*)frm + 0x18) <= 3) {
            if (*(s32*)((u8*)shc + 0xf0) == 0) {
                if (*(void**)((u8*)shc + 0xe8) != NULL) {
                    SFMPVF_EndRefFrm(*(void**)((u8*)shc + 0xe8));
                }
                *(void**)((u8*)shc + 0xe8) = *(void**)((u8*)shc + 0xec);
                *(void**)((u8*)shc + 0xec) = *out;
            }
        }
        {
            /* derive luma/chroma plane sizes and fill both reference-plane
             * descriptor halves; strides come from the frame width */
            void* r2 = *(void**)((u8*)shc + 0xec);
            void* r1 = *(void**)((u8*)shc + 0xe8);
            s32 w = *(u32*)((u8*)frm + 0);
            s32 h = *(u32*)((u8*)frm + 4);
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
            y1 = *(u32*)((u8*)r1 + 8);
            y2 = *(u32*)((u8*)r2 + 8);
            *(u16*)((u8*)para + 0xe) = hB32 * 0x20;
            *(u16*)((u8*)para + 0xc) = chromaW;
            *(u32*)((u8*)para + 8) = y1;
            *(u32*)((u8*)para + 0) = y1 + planeSize;
            *(u32*)((u8*)para + 4) = y1 + planeSize + chromaSize;
            *(u32*)((u8*)para + 0x18) = y2;
            *(u32*)((u8*)para + 0x10) = y2 + planeSize;
            *(u32*)((u8*)para + 0x14) = y2 + planeSize + chromaSize;
            *(u16*)((u8*)para + 0x1c) = chromaW;
            *(u16*)((u8*)para + 0x1e) = hB32 * 0x20;
        }
    } else {
        if (*(u32*)((u8*)frm + 0x18) >= 2 && *(u32*)((u8*)frm + 0x18) <= 3) {
            *(s32*)((u8*)shc + 0xc0) ^= 1;
            *(s32*)((u8*)shc + 0xc4) ^= 1;
            *(void**)((u8*)shc + 0xec) = *out;
        }
        {
            void* p = (u8*)shc + 0xc8;
            u32* t = (u32*)((u8*)p + *(s32*)((u8*)shc + 0xc0) * 0x10);
            *(u32*)((u8*)para + 0) = t[0];
            *(u32*)((u8*)para + 4) = t[1];
            *(u32*)((u8*)para + 8) = t[2];
            *(u32*)((u8*)para + 0xc) = t[3];
            t = (u32*)((u8*)p + *(s32*)((u8*)shc + 0xc4) * 0x10);
            *(u32*)((u8*)para + 0x10) = t[0];
            *(u32*)((u8*)para + 0x14) = t[1];
            *(u32*)((u8*)para + 0x18) = t[2];
            *(u32*)((u8*)para + 0x1c) = t[3];
        }
    }
    *(u32*)((u8*)para + 0x30) = *(u32*)((u8*)*out + 8);
    *(u32*)((u8*)para + 0x34) = (u32)((u8*)*out + 0x68);
    *(u32*)((u8*)para + 0x38) = 0;
    *(u32*)((u8*)para + 0x3c) = 0;
    *(s16*)((u8*)para + 0x40) = 0;
    *(s32*)((u8*)self + 0x98c) = 0;
    return 0;
}

// ---------------------------------------------------------------------------
// fn_803C9948
// ---------------------------------------------------------------------------
void fn_803C9948(void* a, void* b, s32* out1, s32* out2) {
    s32 v1 = 0;
    s32 v2 = 0;
    if (*(s32*)((u8*)a + 0x40) == 3) {
        s32 f = *(s32*)((u8*)b + 0x2C);
        if (f == 2) {
            void* p = *(void**)((u8*)b + 0xE8);
            if (p != NULL) {
                v1 = *(s32*)((u8*)p + 0x4C);
                v2 = *(s32*)((u8*)p + 0x50);
            }
        } else if (f == 3) {
            void* p = *(void**)((u8*)b + 0xE8);
            if (p != NULL) {
                v1 = *(s32*)((u8*)p + 0x4C);
                v2 = *(s32*)((u8*)p + 0x50);
            }
            void* q = *(void**)((u8*)b + 0xEC);
            if (q != NULL) {
                v1 += *(s32*)((u8*)q + 0x4C);
                v2 += *(s32*)((u8*)q + 0x50);
            }
        }
    }
    *out1 = v1;
    *out2 = v2;
}

// ---------------------------------------------------------------------------
// fn_803C99C8
// ---------------------------------------------------------------------------
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

void fn_803C99C8(void* self) {
    SFMPV99* o = (SFMPV99*)self;
    u32 local[8];
    u32* t;
    s32* base;
    u32* dst;
    s32 flg;
    s32 t1;
    s32 t2;
    void* shc;
    t = &o->tflag;
    base = (s32*)&o->tc[0];
    dst = &o->dst[0];
    if (o->dst[0] != 0) {
        return;
    }
    flg = 0;
    *(u64*)&local[0] = *(u64*)&t[1];
    *(u64*)&local[2] = *(u64*)&t[3];
    *(u64*)&local[4] = *(u64*)&t[5];
    *(u64*)&local[6] = *(u64*)&t[7];
    /* shc->4 == 2/3 dispatch: bodies as branch targets (Wii/1.1 goto-chain) */
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
        local[6] = 0;
    }
    SFTIM_Tc2Time(local, &t1, &t2);
    *(u64*)&dst[1] = *(u64*)&local[0];
    *(u64*)&dst[3] = *(u64*)&local[2];
    *(u64*)&dst[5] = *(u64*)&local[4];
    *(u64*)&dst[7] = *(u64*)&local[6];
    dst[9] = t1 - base[9];
    dst[10] = t2;
    dst[0] = 1;
}

// ---------------------------------------------------------------------------
// sfmpv_GoDdelim
// ---------------------------------------------------------------------------
// NOTE: residual 24-instruction reg swap vs retail: retail homes param3 (mask)
// into r31 and param1 (self) into r30; this MWCC always homes param1 first.
// Declaration-order/copy/coalescing levers all exhausted (see session notes).
s32 sfmpv_GoDdelim(u8* self, u32 unused, u32 mask) {
    s32 n;
    u32 m;
    u8* obj;
    u8* seg0;
    u32 siz0;
    u8* seg1;
    s32 found;
    s32 i;
    u32 info[7];
    u32 r;
    u32 type;
    m = mask;
    obj = self;
    if (SFBUF_RingGetRead(obj, *(s32*)(obj + 0x2070), info) != 0) {
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
    SFBUF_RingAddRead(obj, *(s32*)(obj + 0x2070), n);
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
        *(s64*)(obj + 0x9c0) += n;
    }
    *(s64*)(obj + 0x9b8) += n;
    return n;
}

// ---------------------------------------------------------------------------
// SFMPV_Create
// ---------------------------------------------------------------------------
s32 SFMPV_Create(void* self) {
    void* shc;
    void* mpv;
    s32 r;
    if (SFSET_GetCond(self, 5) == 0) {
        return 0;
    }
    shc = (u8*)self + 0x2450;
    *(void**)((u8*)self + 0x2068) = shc;
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
    MPV_SetCond(mpv, 6, *(s32*)((u8*)self + 0x40));
    *(void**)shc = mpv;
    if (SFPLY_GetResetFlg() != 0) {
        SFMPVF_ResetPicUsrBuf(self);
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_InitInf
// ---------------------------------------------------------------------------
s32 sfmpv_InitInf(u8* self, u8* shc) {
    s32 r;
    s32 i;
    r = SFMPVF_ReadGlobalMpvPara(self);
    if (r != 0) {
        return r;
    }
    *(u32*)(shc + 0) = 0;
    *(u32*)(self + 0x27f4) = 0;
    *(u32*)(shc + 4) = 5;
    *(u32*)(shc + 8) = 0xc0;
    *(u32*)(shc + 0xc4) = 1;
    *(u32*)(shc + 0xc0) = 0;
    *(u32*)(self + 0x27f0) = 0;
    *(u32*)(shc + 0xe8) = 0;
    *(u32*)(shc + 0xec) = 0;
    *(u32*)(shc + 0xf0) = 0;
    *(u32*)(shc + 0xf4) = 0;
    *(u32*)(shc + 0xf8) = 0;
    *(u32*)(shc + 0xfc) = 0;
    sfmpv_InitFrmObj(self + 0x27f8, (u32*)(self + 0x27ac), 0x10);
    *(u32*)(shc + 0xc) = 0;
    *(u32*)(shc + 0x10) = 0;
    UTY_MemsetDword((u32*)(shc + 0x14), -1, 0x20);
    *(s32*)(shc + 0x94) = -1;
    *(u32*)(shc + 0x98) = 0;
    *(u32*)(shc + 0x9c) = 0x7fffffff;
    *(u32*)(shc + 0xa0) = 0;
    *(u32*)(shc + 0xa4) = 0;
    *(u32*)(shc + 0xa8) = 0;
    *(s32*)(shc + 0xb4) = -1;
    *(s32*)(shc + 0xb0) = -1;
    *(u32*)(shc + 0xb8) = 0;
    *(s32*)(shc + 0xbc) = -1;
    SFMPVF_InitPicUsr(self + 0x38f8);
    for (i = 0; i < 16; i++) {
        *(u32*)(self + 0x2858 + i * 0x110) = (u32)(self + 0x390c + i * 8);
    }
    *(s32*)(shc + 0x104) = -1;
    *(s32*)(shc + 0x100) = -1;
    return 0;
}

// ---------------------------------------------------------------------------
// sfmpv_InitFrmObj
// ---------------------------------------------------------------------------
void sfmpv_InitFrmObj(void* frm, const u32* src, s32 count) {
    const u32* q = src;
    s32 i;
    for (i = 0; i < count; i++) {
        *(u32*)((u8*)frm + 0x00) = 0;
        *(u32*)((u8*)frm + 0x04) = 0;
        *(u16*)((u8*)frm + 0x0c) = 0;
        *(u16*)((u8*)frm + 0x0e) = 0;
        *(u16*)((u8*)frm + 0x10) = 0;
        *(u16*)((u8*)frm + 0x12) = 0;
        *(u32*)((u8*)frm + 0x14) = 5;
        SFTIM_InitTtu((u8*)frm + 0x18, 0);
        *(u32*)((u8*)frm + 0x08) = *q;
        *(u32*)((u8*)frm + 0x44) = 0;
        *(u32*)((u8*)frm + 0x48) = 1;
        *(u32*)((u8*)frm + 0x4c) = 0;
        *(u32*)((u8*)frm + 0x50) = 0;
        *(u32*)((u8*)frm + 0x54) = 0;
        *(u32*)((u8*)frm + 0x58) = 0;
        *(u32*)((u8*)frm + 0x5c) = 0;
        *(u32*)((u8*)frm + 0x64) = -1;
        UTY_MemsetDword((u32*)((u8*)frm + 0x68), -1, 0x20);
        *(u32*)((u8*)frm + 0xec) = -1;
        *(u32*)((u8*)frm + 0xe8) = -1;
        *(u32*)((u8*)frm + 0xf0) = 0;
        *(u32*)((u8*)frm + 0xf4) = 0;
        *(u32*)((u8*)frm + 0xf8) = 0;
        *(u32*)((u8*)frm + 0xfc) = 0;
        *(u16*)((u8*)frm + 0x100) = 0;
        frm = (u8*)frm + 0x110;
        q += 1;
    }
}

// ---------------------------------------------------------------------------
// sfmpv_ErrFn
// ---------------------------------------------------------------------------
void sfmpv_ErrFn(s32 val, u32 err) {
    u32 tmp = err + 3;
    if (tmp <= 1) return;
    if (err == 0) return;
    if ((err & 0xFFFF0000) == 0xFF040000)
        fn_803C34F8(val, err);
    else
        SFLIB_SetErr((void*)val, err);
}

// ---------------------------------------------------------------------------
// SFMPV_Destroy
// ---------------------------------------------------------------------------
s32 SFMPV_Destroy(void* self) {
    void* shc = *(void**)((u8*)self + 0x2068);
    u32 mpv = *(u32*)shc;
    if (mpv == 0) {
        return 0;
    }
    SFMPVF_WriteGlobalMpvPara(self);
    if (MPV_Destroy((void*)mpv) != 0) {
        return SFLIB_SetErr(self, 0xff000f0c);
    }
    *(u32*)shc = 0;
    return 0;
}

// ---------------------------------------------------------------------------
// SFMPV_RequestStop
// ---------------------------------------------------------------------------
int SFMPV_RequestStop(void* self) {
    void* p;
    int result;
    
    p = *(void**)(*(void**)((u8*)self + 0x2068));
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
void sfmpv_SetFrmInf(void* self, void* frm, void* info) {
    *(u32*)((u8*)info + 0x00) = *(u32*)((u8*)frm + 0x68);
    *(u32*)((u8*)info + 0x04) = *(u32*)((u8*)frm + 0x6c);
    *(u32*)((u8*)info + 0x08) = *(u32*)((u8*)frm + 0x70);
    *(u32*)((u8*)info + 0x0c) = *(u32*)((u8*)frm + 0x74);
    *(u32*)((u8*)info + 0x10) = *(u32*)((u8*)frm + 0x80);
    *(u32*)((u8*)info + 0x14) = *(u32*)((u8*)frm + 0x44);
    *(u32*)((u8*)info + 0x18) = *(u32*)((u8*)frm + 0x48);
    *(u32*)((u8*)info + 0x1c) = *(u32*)((u8*)self + 0x40);
    *(u32*)((u8*)info + 0x20) = *(u32*)((u8*)frm + 0x08);
    *(u32*)((u8*)info + 0x24) = *(u32*)((u8*)frm + 0x4c);
    *(u32*)((u8*)info + 0x28) = *(u32*)((u8*)frm + 0x50);
    *(u32*)((u8*)info + 0x2c) = *(u32*)((u8*)frm + 0x54);
    *(u32*)((u8*)info + 0x30) = *(u32*)((u8*)frm + 0x58);
    *(u32*)((u8*)info + 0x34) = *(u32*)((u8*)frm + 0x5c);
    *(u32*)((u8*)info + 0x38) = *(u32*)((u8*)frm + 0x60);
    *(u32*)((u8*)info + 0x3c) = *(u32*)((u8*)frm + 0xa8);
    *(u32*)((u8*)info + 0x40) = *(u32*)((u8*)frm + 0xac);
    if (*(u32*)((u8*)frm + 0xa8) == 0) {
        *(u32*)((u8*)info + 0x48) = 2;
    } else {
        *(u32*)((u8*)info + 0x48) = 1;
    }
    *(u64*)((u8*)info + 0x50) = *(u64*)((u8*)frm + 0xe8);    *(u32*)((u8*)info + 0x58) = *(u32*)((u8*)frm + 0xa0);
    *(u32*)((u8*)info + 0x5c) = *(u32*)((u8*)frm + 0xa4);
    *(u32*)((u8*)info + 0x60) = *(u32*)((u8*)frm + 0xb0);
    *(u32*)((u8*)info + 0x64) = *(u32*)((u8*)frm + 0xb4);
    *(s16*)((u8*)info + 0x68) = *(s16*)((u8*)frm + 0xb8);
    *(s16*)((u8*)info + 0x6a) = *(s16*)((u8*)frm + 0xba);
    *(u8*)((u8*)info + 0x6c) = *(u8*)((u8*)frm + 0xbd);
    *(u8*)((u8*)info + 0x6d) = *(u8*)((u8*)frm + 0xbe);
    *(u8*)((u8*)info + 0x6e) = *(u8*)((u8*)frm + 0xbf);
    *(u8*)((u8*)info + 0x6f) = *(u8*)((u8*)frm + 0xc1);
    *(u8*)((u8*)info + 0x70) = *(u8*)((u8*)frm + 0xc2);
    *(u8*)((u8*)info + 0x71) = *(u8*)((u8*)frm + 0xc3);
    *(u8*)((u8*)info + 0x72) = *(u8*)((u8*)frm + 0xc4);
    *(u8*)((u8*)info + 0x73) = *(u8*)((u8*)frm + 0xc5);
    *(u8*)((u8*)info + 0x74) = *(u8*)((u8*)frm + 0xc6);
    *(u8*)((u8*)info + 0x75) = *(u8*)((u8*)frm + 0xc7);
    *(u8*)((u8*)info + 0x76) = *(u8*)((u8*)frm + 0xc8);
    *(u8*)((u8*)info + 0x77) = *(u8*)((u8*)frm + 0xc9);
    *(u8*)((u8*)info + 0x78) = *(u8*)((u8*)frm + 0xca);
    *(u8*)((u8*)info + 0x79) = *(u8*)((u8*)frm + 0xcb);
    *(u8*)((u8*)info + 0x7a) = *(u8*)((u8*)frm + 0xcc);
    *(s16*)((u8*)info + 0x7c) = *(s16*)((u8*)frm + 0x100);
}

void SFMPV_AddRead(void) { SFMPVF_AddRead(); }

// ---------------------------------------------------------------------------
// SFMPV_Seek
// ---------------------------------------------------------------------------
s32 SFMPV_Seek(void* self) {
    void* shc = *(void**)((u8*)self + 0x2068);
    s32 out;
    s32 r = sfmpv_ReprocessShc(self, shc, &out);
    if (r != 0) {
        return r;
    }
    *(s32*)((u8*)shc + 4) = 2;
    if (!(out != 0 && SFSET_GetCond(self, 0x30) != 0)) {
        *(s32*)((u8*)shc + 8) = 0xc0;
    } else {
        *(s32*)((u8*)shc + 8) = 0xc8;
    }
    return 0;
}

int fn_803CA368(void) { return 0x0; }

// ---------------------------------------------------------------------------
// sfmpv_ReprocessShc
// ---------------------------------------------------------------------------
s32 sfmpv_ReprocessShc(void* self, void* shc, s32* out) {
    s32 v;
    u8* p;
    void* hd;
    void* mpv;
    u32 out2;
    u32 pic[2];

    *out = 0;
    v = *(s32*)((u8*)self + 0x2670);
    hd = *(void**)((u8*)self + 0x2068);
    if (v != 0) {
        if (*(s32*)((u8*)hd + 0x10) <= 0) {
            p = (u8*)v + 0xad0;
            goto join_p;
        }
        p = NULL;
        goto join_p;
    }
    p = NULL;
join_p:
    if (p == NULL) {
        return 0;
    }
    if (*(u32*)p == 0) {
        return 0;
    }
    mpv = *(void**)shc;
    *(u32*)((u8*)self + 0xdd4) = *(u32*)((u8*)p + 0xc);
    *(u32*)((u8*)self + 0xdd8) = *(u32*)((u8*)p + 0x10);
    *(u32*)((u8*)self + 0xddc) = *(u32*)((u8*)p + 0x14);
    *(u32*)((u8*)self + 0xde0) = *(u32*)((u8*)p + 0x18);
    *(u32*)((u8*)self + 0xde4) = *(u32*)((u8*)p + 0x1c);
    *(u32*)((u8*)self + 0xde8) = *(u32*)((u8*)p + 0x20);
    *(u32*)((u8*)self + 0xdec) = *(u32*)((u8*)p + 0x24);
    *(u32*)((u8*)self + 0xdf0) = *(u32*)((u8*)p + 0x28);
    *(u32*)((u8*)self + 0xdf4) = *(u32*)((u8*)p + 0x2c);
    *(u32*)((u8*)self + 0xdf8) = *(u32*)((u8*)p + 0x30);
    *(u32*)((u8*)self + 0xdfc) = *(u32*)((u8*)p + 0x34);
    pic[0] = (u32)((u8*)p + 0x38);
    pic[1] = *(u32*)((u8*)p + 0x238);
    if (MPV_DecodePicAtr(mpv, pic, &out2) != 0) {
        return SFLIB_SetErr(self, 0xff000f1b);
    }
    *out = 1;
    return 0;
}
