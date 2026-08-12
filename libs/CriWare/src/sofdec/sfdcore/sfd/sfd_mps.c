// Decompiled CriWare sfd_mps TU - high-level C reconstruction
// Matches retail assembly at 0x803C3798..0x803C570C

#include <harness_catalog.h>

int SFD_SetElementOutSj(void* self, s32 idx, s32 data, s32 arg3, s32 arg4);
int SFMPS_Init(void);int SFMPS_Finish(void);
void SFMPS_ExecServer(void* self);
int sfmps_DecodeSomeUnit(void* self);
int criware_803C1490(void* self, s32* out_a, s32* out_b, s32 unused, s32* out_c);
int sfmps_DecodeOneUnit(void* self, s32 buf, s32 size, s32* out_size, s32* out_flag, s32 a5);
void sfmps_pesfn(void* self, u8 stream_kind, s32 arg3, s32 arg4);
void sfmps_SkipNext(void* self, s32 buf, s32 size, s32* out_size);
int sfmps_CopyPketData(void* self, s32 buf, s32 size, s32* out_size, s32* out_flag);
int sfmps_CopyAudio(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_hi, s32 pts_lo);
int sfmps_CopyVideo(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_hi, s32 pts_lo);
int sfmps_CopyPrvate(void* self, s32 kind, s32 buf, s32 size);
int sfmps_CopyUsrSj(void* self, s32 buf, s32 size, s32 out_kind);
int sfmps_CopyPadding(void);
int sfmps_CopyDstBuft(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_hi, s32 pts_lo);
int sfmps_ChkSupply(void* self, s32 buf, s32 size, s32 a5);
void sfmps_GetStmNum(void* self, s32* out_a, s32* out_b);
void sfmps_SetMvInf(void* self);
void sfmps_SetMpsHd(void* self);
void sfmps_SetMpsRaw(void* self, void* mps_work, s32 buf, s32 size);
void* SFMPS_Create(void* self);
void sfmps_InitInf(void* inf);
s32 sfmps_ErrFn(void* h, u32 err_code);
int SFMPS_Destroy(void* self);
int SFMPS_Seek(void* self);

// External symbols
extern void* lbl_eu_80606E34;
extern u32 lbl_eu_805687F4[];
extern u32 lbl_eu_80607AF0;
extern u8 lbl_eu_80607160[];
extern u32 lbl_eu_8051C930[];
extern s32 (*lbl_eu_80619BAC)(void*, void*);

// External function declarations
s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(s32 val, u32 err_code);
void SFSET_SetCond(void* self, u32 idx, u32 val);
u32 SFSET_GetCond(void* self, u32 idx);
void SFBUF_SetPrepFlg(void* self, u32 idx, u32 val);
int SFBUF_GetPrepFlg(void* self, int idx);
void SFBUF_SetTermFlg(void* buf, s32 idx, u32 flg);
int SFBUF_GetTermFlg(void* self, int idx);
u32 SFBUF_GetRTot(void* self, u32 idx);
s32 SFBUF_GetWTot(void* self, u32 idx);
s32 SFBUF_RingGetRead(void* self, u32 idx, void* out);
s32 SFBUF_RingGetWrite(void* self, u32 idx, void* out);
s32 SFBUF_RingAddRead(void* self, u32 idx, u32 size);
s32 SFBUF_RingAddWrite(void* self, u32 idx, u32 size, void* extra);
void SFBUF_GetFlowCnt(void* self, u32* readCnt, u32* writeCnt);
u64 SFBUF_UpdateFlowCnt(u64 v, u32 x);
void SFBUF_GetUoch(void* self, int idx, int sub_idx, u32* dst);
u32 SFTRN_GetPrepFlg(void* self, u32 idx);
void SFTRN_SetTermFlg(void* self, u32 idx, u32 val);
s32 SFCON_IsEndcodeSkip(void* h);
s32 SFCON_IsSystemEndcodeSkip(void* h);
s32 SFHDS_SetHdr(void* self, s32 kind, void* buf, s32 size, void* out);
void SFHDS_ReprocessHdr(void* self);
s32 SFPTS_IsPtsQueFull(void* self, int idx);
s32 SFPTS_WritePtsQue(void* self, int idx, void* data, void* out);
void MEM_Copy(void* dst, const void* src, u32 size);

void* MPS_Create(void);
int MPS_Destroy(void* handle);
int MPS_Init(int max_streams, void* work_buf);
void MPS_Finish(void);
void MPS_SetSystemFn(void* mps, void* fn, void* arg);
void MPS_SetPsMapFn(void* mps, void* fn, void* arg);
void MPS_SetPesFn(void* mps, void* pes_fn, void* pes_obj);
int MPS_DecHd(void* mps, void* buf, int size, int* out_size, int* out_flag);
int MPS_CheckDelim(const u8* buf);
int MPS_GetPketHd(void* mps, void* out);
int MPS_GetSysHd(void* mps, void* out, int idx);
int MPS_GetPackHd(void* mps, void* out);
int MPS_GetLastSysHd(void* mps, void* out);
int MPS_GetErrInf(void* handle, void* cb, void* arg);

int SFD_SetElementOutSj(void* self, s32 idx, s32 data, s32 arg3, s32 arg4) {
    void* mps;
    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000171);
    }
    if ((u32)(idx - 0xbc) > 0x43) {
        return 0;
    }
    mps = *(void**)((u8*)self + 0x2024);
    *(s32*)((u8*)mps + 0x150) = arg3;
    *(s32*)((u8*)mps + 0x154) = arg4;
    *(s32*)((u8*)mps + idx * 4 - 0x2b0) = data;
    return 0;
}

int SFMPS_Init(void) {
    int ret = MPS_Init(8, lbl_eu_80607160);
    if (ret) {
        return SFLIB_SetErr(0, 0xff000d01);
    }
    lbl_eu_80607AF0 = 0;
    return 0;
}

int SFMPS_Finish(void) {
    MPS_Finish();
    return 0;
}

void SFMPS_ExecServer(void* self) {
    void (*fn)(void*, void*);
    int endflg;
    int ret;
    s32* mps_sub;
    s32 prepflg;
    s32 cond_val;

    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_805687F4[3] = (u32)self;
        fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
        fn(lbl_eu_80606E34, &lbl_eu_805687F4[1]);
    }

    if (*(s32*)((u8*)self + 0x2034) != 8) {
        endflg = SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2034)) & 1;
    } else {
        endflg = 1;
    }
    if (*(s32*)((u8*)self + 0x2030) != 8) {
        endflg = endflg & SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2030));
    }
    if (*(s32*)((u8*)self + 0x2038) != 8) {
        endflg = endflg & SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2038));
    }

    if (endflg == 1) {
        ret = 0;
    } else {
        void* mps_work;
        mps_sub = *(s32**)((u8*)self + 0x2024);
        mps_work = (void*)mps_sub[0];
        MPS_SetSystemFn(mps_work,
                        *(void**)((u8*)self + 0xd44),
                        *(void**)((u8*)self + 0xd48));
        ret = sfmps_DecodeSomeUnit(self);

        if (*(s32*)((u8*)self + 0x54) == 2) {
            s32 num_a, num_b;
            sfmps_GetStmNum(self, &num_a, &num_b);

            if (*(s32*)((u8*)self + 0x2034) != 8) {
                prepflg = SFBUF_GetPrepFlg(self, *(s32*)((u8*)self + 0x2034));
            } else {
                prepflg = 0;
            }
            if (*(s32*)((u8*)self + 0x2030) != 8) {
                prepflg |= SFBUF_GetPrepFlg(self, *(s32*)((u8*)self + 0x2030));
            }
            if (*(s32*)((u8*)self + 0x2038) != 8) {
                prepflg |= SFBUF_GetPrepFlg(self, *(s32*)((u8*)self + 0x2038));
            }

            if (prepflg != 1) {
                s32 idx = *(s32*)((u8*)self + 0x202c);
                if (SFBUF_GetPrepFlg(self, idx) == 1) {
                    s32* sub = (s32*)((u8*)self + idx * 0x74);
                    s32 f8 = *(s32*)((u8*)self + 0x8);
                    s32 max_buf = *(s32*)((u8*)self + 0xa74);
                    s32 f13d4 = sub[0x13d4 / 4];
                    s32 limit;
                    s32 wt;

                    if (f8 <= 0) f8 = f13d4;
                    if (f8 <= 0) f8 = max_buf;
                    if (f8 < max_buf) max_buf = f8;

                    wt = SFBUF_GetWTot(self, 0);
                    if (wt >= max_buf) {
                        if (*(s32*)((u8*)self + 0x2034) != 8)
                            SFBUF_SetPrepFlg(self, *(s32*)((u8*)self + 0x2034), 1);
                        if (*(s32*)((u8*)self + 0x2030) != 8)
                            SFBUF_SetPrepFlg(self, *(s32*)((u8*)self + 0x2030), 1);
                        if (*(s32*)((u8*)self + 0x2038) != 8)
                            SFBUF_SetPrepFlg(self, *(s32*)((u8*)self + 0x2038), 1);
                    }
                }
            }

            sfmps_SetMvInf(self);

            mps_sub = *(s32**)((u8*)self + 0x2024);
            cond_val = SFSET_GetCond(self, 6);
            if (cond_val != 0) {
                if (SFSET_GetCond(self, 0x50) != 0) {
                    if (SFBUF_GetWTot(self, 2) == 0) {
                        if (SFTRN_GetPrepFlg(self, 6) != 0) {
                            SFSET_SetCond(self, 6, 0);
                        }
                    }
                }
            }

            if (SFSET_GetCond(self, 5) != 0) {
                if (SFSET_GetCond(self, 0x4f) != 0) {
                    if (SFBUF_GetWTot(self, 1) == 0) {
                        if (mps_sub[1] == 0) {
                            if (SFTRN_GetPrepFlg(self, 7) != 0) {
                                SFSET_SetCond(self, 5, 0);
                            }
                        }
                    }
                }
            }

            sfmps_SetMpsHd(self);
        }
    }

    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_805687F4[0x74 / 4] = (u32)((u8*)self + 0x998);
        lbl_eu_805687F4[0x80 / 4] = (u32)((u8*)self + 0x9a0);
        lbl_eu_805687F4[0x8c / 4] = (u32)((u8*)self + 0x9a8);
        fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
        fn(lbl_eu_80606E34, &lbl_eu_805687F4[0x6c / 4]);
    }
}

int sfmps_DecodeSomeUnit(void* self) {
    int ret = 0;
    s32 limit = *(s32*)((u8*)self + 0x2c);
    s32 cond_val = SFSET_GetCond(self, 0x4b);
    s32 total = 0;
    s32 read_size, data_size, out_size, dummy, out_flag;
    u32 flow_b, flow_a;

    for (;;) {
        s32 err;

        if (*(s32*)((u8*)self + 0x70) != 0)
            break;

        ret = criware_803C1490(self, &read_size, &data_size, limit, &dummy);
        if (ret != 0) break;

        ret = sfmps_DecodeOneUnit(self, read_size, data_size, &out_size, &out_flag, dummy);
        if (ret != 0) break;

        *(s64*)((u8*)self + 0x9a0) += out_size;
        *(s64*)((u8*)self + 0x9a8) += out_flag;

        if (out_size == 0)
            break;

        s32 add_size = out_size;
        err = SFBUF_RingAddRead(self, *(s32*)((u8*)self + 0x202c), add_size);
        if (err == 0) {
            *(s32*)((u8*)self + 0x39a8) += add_size;
            err = 0;
        }
        ret = err;
        if (err != 0)
            break;
        total += out_size + out_flag;
        if (cond_val != -1 && total >= cond_val)
            break;
    }

    SFBUF_GetFlowCnt(*(void**)((u8*)self + 0x13cc), &flow_a, &flow_b);
    {
        u32 hi = *(u32*)((u8*)self + 0x998);
        u32 lo = *(u32*)((u8*)self + 0x99c);
        u64 v = SFBUF_UpdateFlowCnt(((u64)hi << 32) | lo, flow_a);
        *(u32*)((u8*)self + 0x998) = (u32)(v >> 32);
        *(u32*)((u8*)self + 0x99c) = (u32)v;
    }

    return ret;
}

int criware_803C1490(void* self, s32* out_a, s32* out_b, s32 unused, s32* out_c) {
    s32 ret;
    s32 idx;
    s32 tmp[10];

    idx = *(s32*)((u8*)self + 0x202c);
    *out_a = 0;
    *out_b = 0;
    *out_c = 0;

    ret = SFBUF_RingGetRead(self, idx, tmp);
    if (ret != 0)
        return ret;

    *out_a = tmp[0];
    *out_b = tmp[1];
    *out_c = tmp[1] + tmp[3];

    if (*out_b >= 0x800) {
        u32 addr = *out_a;
        if (addr != *(u32*)((u8*)self + 0x39ac)) {
            u32 accum = *(u32*)((u8*)self + 0x39a8);
            if ((accum & 0x7ff) == 0) {
                void (*cb)(s32, s32) = (void (*)(s32, s32))(*(u32*)((u8*)self + 0x39a0));
                s32 arg = *(s32*)((u8*)self + 0x39a4);
                if (cb != NULL)
                    cb(arg, addr);
                *(u32*)((u8*)self + 0x39ac) = *out_a;
            }
        }
    }

    return 0;
}

void criware_803C1570(void* self, u32 a, u32 b) {
    if (self == NULL) return;
    *(u32*)((u8*)self + 0x39a0) = a;
    *(u32*)((u8*)self + 0x39a4) = b;
}

int sfmps_DecodeOneUnit(void* self, s32 buf, s32 size, s32* out_size, s32* out_flag, s32 a5) {
    int ret = 0;
    int has_delim;
    int cond_val;
    void* mps_sub;
    void* mps_work;
    int flags = 0;
    int flags2 = 0;

    *out_size = 0;
    *out_flag = 0;

    mps_sub = *(void**)((u8*)self + 0x2024);
    mps_work = *(void**)mps_sub;

    if (!sfmps_ChkSupply(self, buf, size, a5))
        return 0;

    has_delim = (size >= 4) ? MPS_CheckDelim((const u8*)buf) : 0;

    MPS_SetPsMapFn(mps_work, *(void**)((u8*)self + 0xd4c), *(void**)((u8*)self + 0xd50));

    if (*(s32*)((u8*)self + 0xd5c) != 0) {
        MPS_SetPesFn(mps_work, sfmps_pesfn, self);
    } else {
        MPS_SetPesFn(mps_work, NULL, NULL);
    }

    if (MPS_DecHd(mps_work, (void*)buf, size, &flags, &flags2)) {
        ret = SFLIB_SetErr((s32)self, 0xff000d03);
    }

    if (flags2 & 0x20000) {
        sfmps_SetMpsRaw(self, mps_work, buf, size);
    }

    cond_val = SFSET_GetCond(self, 0x56);
    if (*(s32*)((u8*)self + 0x39a0) != 0)
        cond_val = 0x800;

    if ((flags2 & 0x08000000) && SFCON_IsEndcodeSkip(self)) {
        void* sub = *(void**)((u8*)self + 0x2024);
        *(s32*)((u8*)sub + 0x20) += 1;
        *out_size = cond_val;
        *(s32*)((u8*)mps_sub + 0x158) = cond_val;
        return ret;
    }

    if ((flags2 & 0x08000000) && SFCON_IsSystemEndcodeSkip(self)) {
        *out_size = cond_val;
        *(s32*)((u8*)mps_sub + 0x158) = cond_val;
        return ret;
    }

    if (!has_delim) {
        s32 skip_cnt;
        sfmps_SkipNext(self, buf, size, out_flag);
        skip_cnt = *out_flag;
        *out_size = skip_cnt;
        if (skip_cnt > 0) {
            s32 cur = *(s32*)((u8*)mps_sub + 0x158);
            s32 hdr_size = *(s32*)((u8*)self + 0x2c);
            if (cur >= 0) {
                if (cur >= hdr_size) {
                    *(s32*)((u8*)mps_sub + 0x158) = cur + skip_cnt;
                } else if (cur + skip_cnt > hdr_size) {
                    s32 overlap = hdr_size - cur;
                    *out_flag = skip_cnt - overlap;
                    *(s32*)((u8*)mps_sub + 0x158) = hdr_size + (skip_cnt - overlap);
                } else {
                    *(s32*)((u8*)mps_sub + 0x158) = cur + skip_cnt;
                    *out_flag = 0;
                }
            }
        }
        return ret;
    }

    if (flags2 & 0x40000) {
        s32 pket_out;
        sfmps_CopyPketData(self, buf + flags, size - flags, out_size, &pket_out);
        ret = pket_out;
        if (pket_out == 1) {
            *out_size = flags + *out_size;
        }
        *(s32*)((u8*)mps_sub + 0x158) = -1;
        return ret;
    }

    if (SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x202c)) == 1) {
        SFTRN_SetTermFlg(self, 1, 1);
        if (*(s32*)((u8*)self + 0x2034) != 8)
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
        if (*(s32*)((u8*)self + 0x2030) != 8)
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
        if (*(s32*)((u8*)self + 0x2038) != 8)
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
        return ret;
    }

    if (*(s32*)((u8*)self + 0x2c) < size) {
        s32 f10 = flags;
        if (f10 > 0) {
            *out_size = f10;
            *out_flag = f10;
        } else {
            *out_size = 1;
            *out_flag = 1;
        }
    }

    return ret;
}

// Open item (best shape): inline stm_info stores. Retail hoists both loads
// (lwz r0,2464 / lwz r5,2468) before both stores; MWCC interleaves s1's load
// with its store (loads s1 first because its store precedes s0's). Residual:
// 2 structural (load-pair order s1,s0 vs retail s0,s1), 1 reg_swap, 0x54/0x54.
// 14+ shapes probed: locals (reverses both loads AND stores), u64/struct-copy
// (C spills the pair to the stack), named members, decl/store permutations,
// loads-first (breaks the args store order). The witness rejects the load-pair
// offset diff; FULL_MATCH needs the s1 load scheduled after s0's.
void sfmps_pesfn(void* self, u8 stream_kind, s32 arg3, s32 arg4) {
    void (*cb)(s32, void*, s32, s32);
    struct {
        u8 kind;         /* sp+8  */
        u32 pad;         /* sp+0xc */
        s32 args[2];     /* sp+0x10 */
        s32 stm_info[2]; /* sp+0x18 */
    } inf;
    cb = (void (*)(s32, void*, s32, s32))(*(u32*)((u8*)self + 0xd5c));
    if (cb == NULL) return;
    inf.kind = stream_kind;
    inf.args[1] = arg4;
    inf.args[0] = arg3;
    inf.stm_info[1] = *(s32*)((u8*)self + 0x9a4);
    inf.stm_info[0] = *(s32*)((u8*)self + 0x9a0);
    cb(*(s32*)((u8*)self + 0xd60), &inf, *(s32*)((u8*)self + 0x9a4), arg4);
}

void sfmps_SkipNext(void* self, s32 buf, s32 size, s32* out_size) {
    s32 skip_cnt;
    s32 hdr_size;
    int all_zero;
    int i;

    *out_size = 0;
    hdr_size = *(s32*)((u8*)self + 0x2c);

    if (size >= hdr_size + 3) {
        all_zero = 1;
        s32 p = buf;
        for (i = 0; i < hdr_size; i++) {
            if (*(s8*)p++ != 0) {
                all_zero = 0;
                break;
            }
        }
        if (all_zero) {
            *out_size = hdr_size;
            return;
        }
    }

    {
        s32 skip_cnt = 0;
        while (size >= 4) {
            int delim = MPS_CheckDelim((const u8*)buf);
            if (delim & 0x000d0000) {
                *out_size = skip_cnt;
                return;
            }
            skip_cnt++;
            buf++;
            size--;
        }

        if (size > 0 && size < 4) {
            s32 idx = *(s32*)((u8*)self + 0x202c);
            s32* sub = (s32*)((u8*)self + idx * 0x74);
            int is_wrap;

            if (sub[0x13c8 / 4] == 0 && (sub[0x13d8 / 4] != 0 || sub[0x13dc / 4] != 0)) {
                is_wrap = 0;
            } else {
                s32 start = sub[0x13d0 / 4];
                s32 end = sub[0x13d4 / 4];
                is_wrap = ((start + end) == (buf + size)) ? 1 : 0;
            }

            if (is_wrap) {
                skip_cnt += size;
                size = 0;
            }
        }

        if (size > 0 && size < 4) {
            if (SFSET_GetCond(self, 0x55) != 0) {
                skip_cnt += size;
            }
        }

        *out_size = skip_cnt;
    }
}

int sfmps_CopyPketData(void* self, s32 buf, s32 size, s32* out_size, s32* out_flag) {
    int ret = 0;
    void* mps_sub;
    s32 pket_buf[10];
    s32 pket_type;
    s32 pket_size;
    void* sj;
    s32 tmp_size;

    *out_size = 0;
    *out_flag = 0;

    mps_sub = *(void**)((u8*)self + 0x2024);

    if (MPS_GetPketHd(*(void**)mps_sub, pket_buf)) {
        ret = SFLIB_SetErr((s32)self, 0xff000d06);
    }

    pket_type = pket_buf[2];
    pket_size = pket_buf[3];

    if ((u32)(pket_type - 0xbc) > 0x43) {
        *out_flag = 1;
        SFLIB_SetErr((s32)self, 0xff000d0f);
        return ret;
    }
    if (pket_size > 3) {
        *out_flag = 1;
        return 0;
    }
    if (pket_size < 0) {
        SFLIB_SetErr((s32)self, 0xff000d0e);
        return ret;
    }
    if (pket_size == 0) {
        *out_size = 0;
        *out_flag = 1;
        return 0;
    }

    if (size < pket_size) {
        if (SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x202c)) == 1) {
            SFTRN_SetTermFlg(self, 1, 1);
            if (*(s32*)((u8*)self + 0x2034) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
            if (*(s32*)((u8*)self + 0x2030) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
            if (*(s32*)((u8*)self + 0x2038) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
        }
        return 0;
    }

    sj = *(void**)((u8*)mps_sub + pket_type * 4 + 0x40);
    if (sj != NULL) {
        void** vtbl = *(void***)sj;
        s32 avail;
        void (*notify_fn)(void*, s32) = *(void(**)(void*, s32))((u8*)mps_sub + 0x150);
        void* notify_arg = *(void**)((u8*)mps_sub + 0x154);
        s32 total_copied = 0;

        avail = ((s32(*)(void*, s32))vtbl[9])(sj, 0);
        if (avail < pket_size) {
            total_copied = 0;
        } else {
            void* tmp_ptr;
            s32 tmp_sz;
            ((void(*)(void*, s32, s32, void*))vtbl[6])(sj, 0, pket_size, &tmp_ptr);
            MEM_Copy((void*)buf, tmp_ptr, tmp_sz);
            ((void(*)(void*, s32, void*))vtbl[8])(sj, 1, &tmp_ptr);

            if (tmp_sz == 0) {
                total_copied = 0;
            } else {
                s32 remaining = pket_size - tmp_sz;
                buf += tmp_sz;

                if (remaining > 0) {
                    ((void(*)(void*, s32, s32, void*))vtbl[6])(sj, 0, remaining, &tmp_ptr);
                    MEM_Copy((void*)buf, tmp_ptr, tmp_sz);
                    ((void(*)(void*, s32, void*))vtbl[8])(sj, 1, &tmp_ptr);
                    if (tmp_sz != remaining) {
                        lbl_eu_80607AF0++;
                    }
                }

                if (notify_fn != NULL) {
                    notify_fn(notify_arg, pket_type);
                }
                total_copied = 1;
            }
        }
        *out_flag = total_copied;
    } else {
        typedef int (*copy_fn_t)(void*, s32, s32, s32*);
        copy_fn_t fn = ((copy_fn_t*)lbl_eu_8051C930)[pket_size];
        *out_flag = fn(self, buf, pket_size, &tmp_size);
    }

    if (*out_flag == 1) {
        *out_size = pket_size;
    } else if (*out_flag != 0) {
        ret = *out_flag;
    }

    return ret;
}

int sfmps_CopyAudio(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_hi, s32 pts_lo) {
    void* mps_sub;
    s32 split_val;
    int skip;

    if (SFSET_GetCond(self, 6) == 0)
        return 1;

    mps_sub = *(void**)((u8*)self + 0x2024);

    if (*(s32*)((u8*)mps_sub + 0x38) == -1)
        *(s32*)((u8*)mps_sub + 0x38) = stream_kind;
    if (*(s32*)((u8*)mps_sub + 0x30) == -1)
        *(s32*)((u8*)mps_sub + 0x30) = stream_kind;

    split_val = SFSET_GetCond(self, 0x1e);
    if (split_val != -1) {
        if (SFSET_GetCond(self, 0x37) != 0) {
            s32 prev = *(s32*)((u8*)mps_sub + 0x28);
            s32 xored = stream_kind ^ prev;
            skip = ((xored >> 1) - (xored & prev)) < 0;
        } else {
            skip = (*(s32*)((u8*)mps_sub + 0x30) == stream_kind) ? 1 : 0;
        }
        if (skip)
            *(s32*)((u8*)mps_sub + 0x38) = split_val;
    }

    *(s32*)((u8*)mps_sub + 0x28) = stream_kind;

    if (*(s32*)((u8*)mps_sub + 0x38) != stream_kind)
        return 1;

    {
        s64 pts = ((s64)pts_hi << 32) | (u32)pts_lo;
        if (pts < 0) {
            /* negative PTS: skip min/max update */
        } else {
            s64 min = *(s64*)((u8*)mps_sub + 0x10);
            s64 max = *(s64*)((u8*)mps_sub + 0x18);
            min = (pts < min) ? pts : min;
            *(s64*)((u8*)mps_sub + 0x10) = min;
            max = (max < pts) ? pts : max;
            *(s64*)((u8*)mps_sub + 0x18) = max;
        }
    }

    return sfmps_CopyDstBuft(self, *(s32*)((u8*)self + 0x2034), buf, size, pts_hi, pts_lo);
}

int sfmps_CopyVideo(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_hi, s32 pts_lo) {
    void* mps_sub;
    s32 split_val;

    if (SFSET_GetCond(self, 5) == 0)
        return 1;

    mps_sub = *(void**)((u8*)self + 0x2024);

    if (*(s32*)((u8*)mps_sub + 0x34) == -1) {
        s32 mode = SFSET_GetCond(self, 0x3b);
        s32 new_val;

        if (mode == 1) {
            new_val = stream_kind;
        } else if (mode == 2) {
            s32 na, nb;
            sfmps_GetStmNum(self, &na, &nb);
            new_val = (nb >= 2) ? 2 : stream_kind;
        } else {
            new_val = stream_kind;
        }
        *(s32*)((u8*)mps_sub + 0x34) = new_val;

        {
            s32 sys_buf[7];
            MPS_GetSysHd(*(void**)mps_sub, sys_buf, 1);
            if (sys_buf[3] == -1) {
                SFSET_SetCond(self, 0x49, 1);
            }
        }
    }

    if (*(s32*)((u8*)mps_sub + 0x2c) == -1)
        *(s32*)((u8*)mps_sub + 0x2c) = stream_kind;

    split_val = SFSET_GetCond(self, 0x1d);
    if (split_val != -1) {
        int skip = 0;
        if (SFSET_GetCond(self, 0x37) != 0) {
            s32 prev = *(s32*)((u8*)mps_sub + 0x24);
            s32 xored = stream_kind ^ prev;
            skip = (((xored >> 1) & ~(xored & prev)) >> 31) & 1;
        } else {
            skip = (*(s32*)((u8*)mps_sub + 0x2c) == stream_kind) ? 1 : 0;
        }
        if (skip && *(s32*)((u8*)mps_sub + 0x34) != split_val) {
            int is_video = 0;
            if (size >= 4) {
                u8* p = (u8*)buf;
                if (p[0] == 0 && p[1] == 0 && p[2] == 1) {
                    is_video = (p[3] == 0xb3 || p[3] == 0xb8) ? 1 : 0;
                }
            }
            if (is_video)
                *(s32*)((u8*)mps_sub + 0x34) = split_val;
        }
    }

    *(s32*)((u8*)mps_sub + 0x24) = stream_kind;

    if (*(s32*)((u8*)mps_sub + 0x34) != stream_kind)
        return 1;

    return sfmps_CopyDstBuft(self, *(s32*)((u8*)self + 0x2030), buf, size, pts_hi, pts_lo);
}

int sfmps_CopyPrvate(void* self, s32 kind, s32 buf, s32 size) {
    s32 hdr_out;

    if (SFHDS_SetHdr(self, kind, (void*)buf, size, &hdr_out)) {
        if (hdr_out != 0) {
            sfmps_CopyUsrSj(self, 0, buf - 0x12, size + 0x12);
        }
        return 1;
    }

    return sfmps_CopyUsrSj(self, kind, buf, size);
}

int sfmps_CopyUsrSj(void* self, s32 buf, s32 size, s32 out_kind) {
    void* sj;
    void** vtbl;
    s32 avail;
    u32 uoch[4];
    void (*fn1)(void*, s32);
    void (*fn2)(void*, s32);
    void* arg1;
    void* arg2;

    if (*(s32*)((u8*)self + 0x2038) == 8)
        return 1;

    SFBUF_GetUoch(self, *(s32*)((u8*)self + 0x2038), buf, uoch);

    sj = (void*)uoch[0];
    fn1 = (void(*)(void*, s32))uoch[1];
    arg1 = (void*)uoch[2];
    fn2 = (void(*)(void*, s32))uoch[3];
    arg2 = (void*)uoch[2];

    if (sj == NULL)
        return 1;

    vtbl = *(void***)sj;
    avail = ((s32(*)(void*, s32))vtbl[9])(sj, 0);

    if (avail < size) {
        out_kind = 0;
    } else {
        void* tmp_ptr;
        s32 tmp_sz;
        s32 total = 1;

        ((void(*)(void*, s32, s32, void*))vtbl[6])(sj, 0, size, &tmp_ptr);
        MEM_Copy((void*)buf, tmp_ptr, tmp_sz);
        ((void(*)(void*, s32, void*))vtbl[8])(sj, 1, &tmp_ptr);

        if (tmp_sz == 0) {
            out_kind = 0;
        } else {
            s32 remaining = size - tmp_sz;
            buf += tmp_sz;

            if (remaining > 0) {
                ((void(*)(void*, s32, s32, void*))vtbl[6])(sj, 0, remaining, &tmp_ptr);
                MEM_Copy((void*)buf, tmp_ptr, tmp_sz);
                ((void(*)(void*, s32, void*))vtbl[8])(sj, 1, &tmp_ptr);
                if (tmp_sz != remaining) {
                    lbl_eu_80607AF0++;
                }
            }

            if (total == 1) {
                if (fn1 != NULL)
                    fn1(arg1, buf);
                if (fn2 != NULL)
                    fn2(arg2, buf);
            }
            return total;
        }
    }

    if (out_kind == 1) {
        if (fn1 != NULL)
            fn1(arg1, buf);
        if (fn2 != NULL)
            fn2(arg2, buf);
    }

    return out_kind;
}

int sfmps_CopyPadding(void) {
    return 1;
}

int sfmps_CopyDstBuft(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_hi, s32 pts_lo) {
    s32 ring_buf[6];
    s32 write_pos;
    s32 total_size;
    s32 first_size;
    void* first_ptr;
    void* second_ptr;

    if (SFBUF_RingGetWrite(self, stream_kind, ring_buf))
        return 0;

    first_size = ring_buf[1];
    total_size = ring_buf[3];
    write_pos = ring_buf[2];
    first_ptr = (void*)ring_buf[0];
    second_ptr = (void*)ring_buf[5];

    if (size > first_size + total_size)
        return 0;

    if (stream_kind == 1) {
        if (lbl_eu_80619BAC != NULL) {
            s32 data[3];
            data[0] = pts_hi;
            data[1] = pts_lo;
            data[2] = size;
            if (lbl_eu_80619BAC((u8*)self + 0x1374, data) == -1)
                return 0;
        }

        if ((s64)pts_hi >= 0) {
            if (SFPTS_IsPtsQueFull(self, stream_kind)) {
                return 0;
            } else {
                s32 pts_data[4];
                s32 out;
                pts_data[0] = pts_hi;
                pts_data[1] = pts_lo;
                pts_data[2] = write_pos;
                pts_data[3] = size;
                if (SFPTS_WritePtsQue(self, stream_kind, pts_data, &out))
                    return 0;
            }
        }
    } else if (stream_kind == 2) {
        if (lbl_eu_80619BAC != NULL) {
            s32 data[3];
            data[0] = pts_hi;
            data[1] = pts_lo;
            data[2] = size;
            if (lbl_eu_80619BAC((u8*)self + 0x1368, data) == -1)
                return 0;
        }
    }

    if (size <= write_pos) {
        MEM_Copy(first_ptr, (void*)buf, size);
    } else {
        MEM_Copy(first_ptr, (void*)buf, write_pos);
        MEM_Copy(second_ptr, (void*)(buf + write_pos), size - write_pos);
    }

    if (SFBUF_RingAddWrite(self, stream_kind, size, second_ptr))
        return 0;

    return 1;
}

int sfmps_ChkSupply(void* self, s32 buf, s32 size, s32 a5) {
    void* mps_sub;
    s32 stream_idx;
    int delim = 0;
    int endflg;

    mps_sub = *(void**)((u8*)self + 0x2024);
    stream_idx = *(s32*)((u8*)self + 0x202c);

    if (size >= 4) {
        delim = MPS_CheckDelim((const u8*)buf);
        if ((delim & 0x08000000)) {
            if (*(s32*)((u8*)self + 0x203c) < 0) {
                *(s32*)((u8*)self + 0x203c) = SFBUF_GetRTot(self, stream_idx) + 4;
            }
            *(s32*)((u8*)mps_sub + 0x3c) = 1;
        } else if (delim != 0) {
            *(s32*)((u8*)mps_sub + 0x3c) = 0;
        }
    }

    {
        int check_end = 0;
        if ((delim & 0x08000000) != 0) {
            if (SFCON_IsEndcodeSkip(self) || SFCON_IsSystemEndcodeSkip(self)) {
                check_end = 0;
            } else {
                check_end = 1;
            }
        }

        if (check_end) {
            SFTRN_SetTermFlg(self, 1, 1);
            if (*(s32*)((u8*)self + 0x2034) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
            if (*(s32*)((u8*)self + 0x2030) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
            if (*(s32*)((u8*)self + 0x2038) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
            return 0;
        }
    }

    if (a5 < 4) {
        if (SFBUF_GetTermFlg(self, stream_idx) == 1) {
            SFTRN_SetTermFlg(self, 1, 1);
            if (*(s32*)((u8*)self + 0x2034) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
            if (*(s32*)((u8*)self + 0x2030) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
            if (*(s32*)((u8*)self + 0x2038) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
            return 0;
        }
    }

    if (size < 0x40) {
        if ((delim & 0x00010000) == 0 && (delim & 0x00040000) == 0) {
            return 1;
        }
        if (SFBUF_GetTermFlg(self, stream_idx) == 1) {
            SFTRN_SetTermFlg(self, 1, 1);
            if (*(s32*)((u8*)self + 0x2034) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
            if (*(s32*)((u8*)self + 0x2030) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
            if (*(s32*)((u8*)self + 0x2038) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
            return 0;
        }
        return 1;
    }

    return 1;
}

void sfmps_GetStmNum(void* self, s32* out_a, s32* out_b) {
    void* mps_sub;
    void* mps_work;
    s32 max_a = 0, max_b = 0;
    int i;
    s32 sys_buf[8];

    mps_sub = *(void**)((u8*)self + 0x2024);
    mps_work = *(void**)mps_sub;

    for (i = 0; i < 3; i++) {
        MPS_GetSysHd(mps_work, sys_buf, i);
        max_a = (max_a > sys_buf[2]) ? max_a : sys_buf[2];
        max_b = (max_b > sys_buf[3]) ? max_b : sys_buf[3];
    }

    *(s32*)((u8*)mps_sub + 0x8) = max_a;
    *(s32*)((u8*)mps_sub + 0x4) = max_b;
    *out_a = max_a;
    *out_b = *(s32*)((u8*)mps_sub + 0x4);
}

void sfmps_SetMvInf(void* self) {
    void* mps_sub;
    void* mps_work;
    s32 pack_buf[4];
    s32 sys_buf[8];

    mps_sub = *(void**)((u8*)self + 0x2024);
    mps_work = *(void**)mps_sub;

    MPS_GetPackHd(mps_work, pack_buf);
    if (pack_buf[3] >= 1)
        *(s32*)((u8*)self + 0x934) = pack_buf[3];

    MPS_GetSysHd(mps_work, sys_buf, 1);
    if (sys_buf[4] != -1)
        *(s32*)((u8*)self + 0x938) = sys_buf[4];

    if (*(s32*)((u8*)self + 0x940) == -1)
        *(s32*)((u8*)self + 0x940) = *(s32*)((u8*)mps_sub + 0x8);
    if (*(s32*)((u8*)self + 0x944) == -1)
        *(s32*)((u8*)self + 0x944) = *(s32*)((u8*)mps_sub + 0x4);
}

void sfmps_SetMpsHd(void* self) {
    s32* hdr;
    s32* sub;

    hdr = *(s32**)((u8*)self + 0x2670);
    if (hdr == NULL) {
        hdr = NULL;
    } else {
        if (*(s32*)((u8*)*(void**)((u8*)self + 0x2024) + 0x20) > 0) {
            hdr = NULL;
        } else {
            hdr = (s32*)((u8*)hdr + 0x8a0);
        }
    }
    if (hdr == NULL) return;

    sub = *(s32**)((u8*)self + 0x2024);
    {
        s32 hi = sub[6];
        s32 lo = sub[7];
        if ((((s64)(u32)hi << 32) | (u32)lo) == 0x7fffffffffffffffLL)
            return;

        {
            u64 a = ((u64)(u32)hi << 32) | (u32)lo;
            u64 b = ((u64)(u32)hdr[8] << 32) | (u32)hdr[9];
            u64 d = a - b;
            *(s32*)((u8*)self + 0xef4) = (s32)(u32)d;
            *(s32*)((u8*)self + 0xef0) = (s32)(u32)(d >> 32);
        }

        if (hdr[0] != 0) return;

        hdr[1] = *(s32*)((u8*)self + 0x934) * 50;
        hdr[2] = *(s32*)((u8*)self + 0x938);
        hdr[3] = sub[1];
        hdr[4] = sub[2];
        {
            s32 f_eec = *(s32*)((u8*)self + 0xeec);
            s32 f_ee8 = *(s32*)((u8*)self + 0xee8);
            hdr[7] = f_eec;
            hdr[6] = f_ee8;
        }
        {
            s32 s4 = sub[4];
            s32 s5 = sub[5];
            hdr[9] = s5;
            hdr[8] = s4;
        }
        hdr[10] = sub[11];
        hdr[11] = sub[12];
    }
}

void sfmps_SetMpsRaw(void* self, void* mps_work, s32 buf, s32 size) {
    s32* hdr;
    s32* raw_hdr;
    s32 sys_buf[8];
    s32 copy_size;

    hdr = *(s32**)((u8*)self + 0x2670);
    if (hdr == NULL) {
        hdr = NULL;
    } else {
        if (*(s32*)((u8*)*(void**)((u8*)self + 0x2024) + 0x20) > 0) {
            hdr = NULL;
        } else {
            hdr = (s32*)((u8*)hdr + 0x8a0);
        }
    }
    if (hdr == NULL) return;

    if (hdr[0] != 0) return;

    raw_hdr = (s32*)((u8*)hdr + 0x30);
    MPS_GetLastSysHd(mps_work, sys_buf);

    copy_size = (size >= 0xb0) ? 0xb0 : size;

    if (sys_buf[3] > 0) {
        *(s32*)((u8*)raw_hdr + 0x160) = copy_size;
    } else if (sys_buf[2] > 0) {
        *(s32*)((u8*)raw_hdr + 0x164) = copy_size;
        raw_hdr = (s32*)((u8*)raw_hdr + 0xb0);
    } else {
        return;
    }

    MEM_Copy(raw_hdr, (void*)buf, copy_size);
}

void* SFMPS_Create(void* self) {
    void* mps_sub;
    void* mps;

    mps_sub = (u8*)self + 0x2240;
    *(void**)((u8*)self + 0x2024) = mps_sub;

    sfmps_InitInf(mps_sub);

    mps = MPS_Create();
    if (mps == NULL) {
        return (void*)SFLIB_SetErr(0, 0xff000d08);
    }

    if (MPS_GetErrInf(mps, sfmps_ErrFn, self)) {
        MPS_Destroy(mps);
        return (void*)SFLIB_SetErr(0, 0xff000d09);
    }

    *(void**)mps_sub = mps;
    *(u32*)((u8*)self + 0x39a0) = 0;
    *(u32*)((u8*)self + 0x39a4) = 0;
    *(u32*)((u8*)self + 0x39a8) = 0;
    *(u32*)((u8*)self + 0x39ac) = 0;

    return NULL;
}

void sfmps_InitInf(void* inf) {
    s32* p = (s32*)inf;
    s32* q;
    s32 cnt = 2;
    s32 neg = -1;
    s32 zero = 0;
    s32 max = 0x7fffffff;
    int i, j;

    p[0] = zero;
    p[1] = zero;
    p[2] = zero;
    p[5] = neg;
    p[4] = max;
    p[7] = neg;
    p[6] = max;
    p[8] = zero;
    p[9] = max;
    p[10] = max;
    p[11] = neg;
    p[12] = neg;
    p[13] = neg;
    p[14] = neg;
    p[15] = zero;

    for (i = 0; i < cnt * 32; i++) {
        p[16 + i] = zero;
    }
    q = p + i;
    q[16] = 0;
    q[17] = 0;
    q[18] = 0;
    q[19] = 0;
    p[84] = 0;
    p[85] = 0;
    p[86] = -1;
}

s32 sfmps_ErrFn(void* h, u32 err_code) {
    return SFLIB_SetErr((s32)h, err_code);
}

int SFMPS_Destroy(void* self) {
    void* mps_sub = *(void**)((u8*)self + 0x2024);
    void* mps = *(void**)mps_sub;

    if (MPS_Destroy(mps)) {
        return SFLIB_SetErr((s32)self, 0xff000d0a);
    }
    return 0;
}

int SFMPS_RequestStop(void) { return 0; }
int SFMPS_Start(void) { return 0; }
int SFMPS_Stop(void) { return 0; }
int SFMPS_Pause(void) { return 0; }

s32 SFMPS_GetWrite(void* h) {
    return SFLIB_SetErr((s32)h, 0xff000d0b);
}

s32 SFMPS_AddWrite(void* h) {
    return SFLIB_SetErr((s32)h, 0xff000d0b);
}

s32 SFMPS_GetRead(void* h) {
    return SFLIB_SetErr((s32)h, 0xff000d0b);
}

s32 SFMPS_AddRead(void* h) {
    return SFLIB_SetErr((s32)h, 0xff000d0b);
}

int SFMPS_Seek(void* self) {
    s32* hdr = *(s32**)((u8*)self + 0x2670);
    s32* raw_hdr;
    u8* base;
    void* mps;
    void* mps_sub;
    int out1, out2;
    s32 ret1, ret2;
    s32 err;
    s32 v0, v1;

    if (hdr == NULL) {
        raw_hdr = NULL;
    } else if (*(s32*)((u8*)*(void**)((u8*)self + 0x2024) + 0x20) > 0) {
        raw_hdr = NULL;
    } else {
        raw_hdr = (s32*)((u8*)hdr + 0x8a0);
    }
    if (raw_hdr == NULL)
        return 0;
    if (raw_hdr[0] == 0)
        return 0;

    mps_sub = *(void**)((u8*)self + 0x2024);
    SFHDS_ReprocessHdr(self);
    mps = *(void**)mps_sub;
    base = (u8*)raw_hdr + 0x30;

    ret1 = MPS_DecHd(mps, base, *(s32*)((u8*)raw_hdr + 0x190), &out1, &out2);
    ret2 = MPS_DecHd(mps, base + 0xb0, *(s32*)(base + 0x164), &out1, &out2);

    if (ret1 != 0 || ret2 != 0) {
        err = SFLIB_SetErr((s32)self, 0xff000d0d);
    } else {
        err = 0;
    }
    if (err != 0)
        return err;

    v0 = *(s32*)((u8*)raw_hdr + 0x28);
    *(s32*)((u8*)mps_sub + 0x2c) = v0;
    v0 = *(s32*)((u8*)raw_hdr + 0x2c);
    *(s32*)((u8*)mps_sub + 0x30) = v0;
    v0 = *(s32*)((u8*)raw_hdr + 0x18);
    v1 = *(s32*)((u8*)raw_hdr + 0x1c);
    *(s32*)((u8*)self + 0xeec) = v1;
    *(s32*)((u8*)self + 0xee8) = v0;
    v0 = *(s32*)((u8*)raw_hdr + 0x20);
    v1 = *(s32*)((u8*)raw_hdr + 0x24);
    *(s32*)((u8*)mps_sub + 0x14) = v1;
    *(s32*)((u8*)mps_sub + 0x10) = v0;

    return 0;
}

u32 SFMPS_GetConcatCnt(void* self) {
    void* ptr = *(void**)((u8*)self + 0x2024);
    return *(u32*)((u8*)ptr + 0x20);
}
