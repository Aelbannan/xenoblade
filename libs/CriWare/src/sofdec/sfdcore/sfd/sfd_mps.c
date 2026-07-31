// Decompiled CriWare sfd_mps TU — high-level C reconstruction
// Matches retail assembly at 0x803C3798..0x803C570C

#include <harness_catalog.h>

extern void* MPS_Create(void);
extern int MPS_Destroy(void* handle);
extern int MPS_Init(int max_streams, void* work_buf);
extern void MPS_Finish(void);
extern void MPS_SetSystemFn(void* mps, void* fn, void* arg);
extern void MPS_SetPsMapFn(void* mps, void* fn, void* arg);
extern void MPS_SetPesFn(void* mps, void* pes_fn, void* pes_obj);
extern int MPS_DecHd(void* mps, void* buf, int size, int* out_size, int* out_flag);
extern int MPS_CheckDelim(const u8* buf);
extern int MPS_GetPketHd(void* mps, void* out);
extern int MPS_GetSysHd(void* mps, void* out, int idx);
extern int MPS_GetPackHd(void* mps, void* out);
extern int MPS_GetLastSysHd(void* mps, void* out);
extern int MPS_GetErrInf(void* handle, void* cb, void* arg);

extern s32 SFLIB_CheckHn(void* h);
extern s32 SFLIB_SetErr(void* h, u32 err_code);

extern void SFSET_SetCond(void* self, u32 idx, u32 val);
extern u32 SFSET_GetCond(void* self, u32 idx);

extern void SFBUF_SetPrepFlg(void* self, u32 idx, u32 val);
extern int SFBUF_GetPrepFlg(void* self, int idx);
extern void SFBUF_SetTermFlg(void* buf, s32 idx, u32 flg);
extern int SFBUF_GetTermFlg(void* self, int idx);
extern u32 SFBUF_GetRTot(void* self, u32 idx);
extern void SFBUF_GetWTot(void);
extern s32 SFBUF_RingGetRead(void* self, u32 idx, void* out);
extern s32 SFBUF_RingGetWrite(void* self, u32 idx, void* out);
extern s32 SFBUF_RingAddRead(void* self, u32 idx, u32 size);
extern s32 SFBUF_RingAddWrite(void* self, u32 idx, u32 size, void* extra);
extern void SFBUF_GetFlowCnt(void* self, int* a, int* b);
extern int SFBUF_UpdateFlowCnt(int count, int new_val, int old_val);
extern void SFBUF_GetUoch(void* self, int idx, int sub_idx, u32* dst);

extern u32 SFTRN_GetPrepFlg(void* self, u32 idx);
extern void SFTRN_SetTermFlg(void* self, u32 idx, u32 val);

extern s32 SFCON_IsEndcodeSkip(void* h);
extern s32 SFCON_IsSystemEndcodeSkip(void* h);

extern s32 SFHDS_SetHdr(void* self, void* buf, void* size, void* out);
extern void SFHDS_ReprocessHdr(void* self);

extern s32 SFPTS_IsPtsQueFull(void* self, int idx);
extern s32 SFPTS_WritePtsQue(void* self, int idx, void* data, void* out);

extern void MEM_Copy(void* dst, const void* src, u32 size);

extern void SFBUF_GetFlowCnt(void* self, int* a, int* b);

typedef struct {
    u8 _000[0x9A0];
    u32 stm_id;
    u32 stm_inf;
    u8 _9A8[0x3B4];
    u32 pes_fn;
    u32 pes_obj;
} SFMPS_HN;

void SFD_SetElementOutSj(void* self, s32 idx, s32 data, s32 arg3, s32 arg4) {
    void* mps;
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(NULL, 0xff000171);
        return;
    }
    if ((u32)(idx - 0xbc) > 0x43) {
        return;
    }
    mps = *(void**)((u8*)self + 0x2024);
    *(s32*)((u8*)mps + 0x150) = arg3;
    *(s32*)((u8*)mps + 0x154) = arg4;
    *(s32*)((u8*)mps + idx * 4 - 0x2b0) = data;
}

void SFMPS_Init(void) {
    if (MPS_Init(8, lbl_eu_80607160)) {
        SFLIB_SetErr(NULL, 0xff000d01);
    } else {
        lbl_eu_80607AF0 = 0;
    }
}

int SFMPS_Finish(void) {
    MPS_Finish();
    return 0;
}

void SFMPS_ExecServer(void* self) {
    int endflg;
    int ret;
    int num_a, num_b;
    int prepflg;
    void* mps_sub;
    int cond_val;
    void* mps_work;

    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_805687F4.field_0C = self;
        ((void(*)(void*, ...))(*(void***)lbl_eu_80606E34))[9](
            lbl_eu_80606E34, &lbl_eu_805687F4.field_04);
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
        goto epilogue;
    }

    mps_work = *(void**)((u8*)self + 0x2024);
    MPS_SetSystemFn(*(void**)mps_work,
                    *(void**)((u8*)self + 0xd44),
                    *(void**)((u8*)self + 0xd48));
    ret = sfmps_DecodeSomeUnit(self);

    if (*(s32*)((u8*)self + 0x54) != 2) {
        goto epilogue;
    }

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
        s32 stream_idx = *(s32*)((u8*)self + 0x202c);
        if (SFBUF_GetPrepFlg(self, stream_idx) == 1) {
            s32 field_val = *(s32*)((u8*)self + 0x8);
            s32 wt;
            s32 limit;
            s32* sub = (s32*)((u8*)self + stream_idx * 0x74);
            s32 max_buf = *(s32*)((u8*)self + 0xa74);
            s32 field_13d4 = sub[0x13d4 / 4];

            if (field_val <= 0)
                field_val = field_13d4;
            if (field_val <= 0)
                field_val = max_buf;
            if (field_val < max_buf)
                max_buf = field_val;

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

    mps_sub = *(void**)((u8*)self + 0x2024);
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
                if (*(s32*)((u8*)mps_sub + 0x4) == 0) {
                    if (SFTRN_GetPrepFlg(self, 7) != 0) {
                        SFSET_SetCond(self, 5, 0);
                    }
                }
            }
        }
    }

    sfmps_SetMpsHd(self);

epilogue:
    if (lbl_eu_80606E34 != NULL) {
        void** vtbl = *(void***)lbl_eu_80606E34;
        lbl_eu_805687F4.field_74 = (u8*)self + 0x998;
        lbl_eu_805687F4.field_80 = (u8*)self + 0x9a0;
        lbl_eu_805687F4.field_8C = (u8*)self + 0x9a8;
        ((void(*)(void*, ...))vtbl[9])(
            lbl_eu_80606E34, &lbl_eu_805687F4.field_6C);
    }
}

int sfmps_DecodeSomeUnit(void* self) {
    int ret = 0;
    s32 limit;
    int total = 0;
    s32 cond_val;
    int a, b;

    limit = *(s32*)((u8*)self + 0x2c);
    cond_val = SFSET_GetCond(self, 0x4b);

    for (;;) {
        int err;
        s32 read_size = 0;
        s32 data_size = 0;
        s32 dummy;

        if (*(s32*)((u8*)self + 0x70) != 0)
            break;

        err = criware_803C1490(self, &read_size, &data_size, &dummy);
        if (err != 0) {
            ret = err;
            break;
        }

        err = sfmps_DecodeOneUnit(self, read_size, data_size, &data_size, dummy, &dummy);
        if (err != 0) {
            ret = err;
            break;
        }

        {
            s32 hi = *(s32*)((u8*)self + 0x9a4);
            s32 lo = *(s32*)((u8*)self + 0x9a0);
            s32 carry;
            *(s32*)((u8*)self + 0x9a4) = hi + data_size;
            carry = (hi + data_size < hi) ? 1 : 0;
            *(s32*)((u8*)self + 0x9a0) = lo + carry;

            hi = *(s32*)((u8*)self + 0x9ac);
            lo = *(s32*)((u8*)self + 0x9a8);
            *(s32*)((u8*)self + 0x9ac) = hi + dummy;
            carry = (hi + dummy < hi) ? 1 : 0;
            *(s32*)((u8*)self + 0x9a8) = lo + carry;
        }

        if (data_size == 0)
            break;

        ret = SFBUF_RingAddRead(self, *(s32*)((u8*)self + 0x202c), data_size);
        if (ret == 0) {
            *(s32*)((u8*)self + 0x39a8) += data_size;
        }
        if (ret != 0)
            break;

        total += data_size + dummy;
        if (cond_val == -1)
            continue;
        if (total < cond_val)
            continue;
        break;
    }

    SFBUF_GetFlowCnt(*(void**)((u8*)self + 0x13cc), &a, &b);
    {
        s32 lo = *(s32*)((u8*)self + 0x998);
        s32 hi = *(s32*)((u8*)self + 0x99c);
        s32 new_lo = SFBUF_UpdateFlowCnt(lo, hi, a);
        s32 new_hi;
        *(s32*)((u8*)self + 0x998) = new_lo;
        *(s32*)((u8*)self + 0x99c) = new_hi;
    }

    return ret;
}

int criware_803C1490(void* self, s32* out_a, s32* out_b, s32* out_c) {
    s32 tmp[3];
    void (*cb)(void*, s32);

    *out_a = 0;
    *out_b = 0;
    *out_c = 0;

    if (SFBUF_RingGetRead(self, *(s32*)((u8*)self + 0x202c), tmp))
        return 0;

    *out_a = tmp[0];
    *out_b = tmp[1];
    *out_c = tmp[1] + tmp[2];

    if (tmp[1] >= 0x800) {
        u32 addr = *out_a;
        if (addr != *(u32*)((u8*)self + 0x39ac)) {
            u32 accum = *(u32*)((u8*)self + 0x39a8);
            if ((accum & 0x7ff) == 0) {
                cb = (void (*)(void*, s32))(*(u32*)((u8*)self + 0x39a0));
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

int sfmps_DecodeOneUnit(void* self, s32 buf, s32 size, s32* out_size, s32 a5, s32* out_flag) {
    int ret = 0;
    int has_delim;
    int cond_val;
    void* mps_sub;
    void* mps_work;
    s32 flags;

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

    if (MPS_DecHd(mps_work, buf, size, &flags, out_flag)) {
        ret = SFLIB_SetErr(self, 0xff000d03);
    }

    if (*out_flag & 0x2000) {
        sfmps_SetMpsRaw(self, mps_work, buf, size);
    }

    cond_val = SFSET_GetCond(self, 0x56);
    if (*(s32*)((u8*)self + 0x39a0) != 0)
        cond_val = 0x800;

    if ((*out_flag & 0x08000000) && SFCON_IsEndcodeSkip(self)) {
        void* sub = *(void**)((u8*)self + 0x2024);
        *(s32*)((u8*)sub + 0x20) += 1;
        *out_size = cond_val;
        *(s32*)((u8*)mps_sub + 0x158) = cond_val;
        return ret;
    }

    if ((*out_flag & 0x08000000) && SFCON_IsSystemEndcodeSkip(self)) {
        *out_size = cond_val;
        *(s32*)((u8*)mps_sub + 0x158) = cond_val;
        return ret;
    }

    if (!has_delim) {
        s32 skip_cnt;
        sfmps_SkipNext(self, buf, size, out_flag);
        *out_size = *out_flag;
        skip_cnt = *out_flag;
        if (skip_cnt <= 0) return ret;

        {
            s32 cur = *(s32*)((u8*)mps_sub + 0x158);
            if (cur < 0) return ret;
            if (cur >= *(s32*)((u8*)self + 0x2c)) {
                *(s32*)((u8*)mps_sub + 0x158) = cur + skip_cnt;
            } else if (cur + skip_cnt > *(s32*)((u8*)self + 0x2c)) {
                s32 overlap = *(s32*)((u8*)self + 0x2c) - cur;
                *out_flag = skip_cnt - overlap;
                *(s32*)((u8*)mps_sub + 0x158) = *(s32*)((u8*)self + 0x2c) + (skip_cnt - overlap);
            } else {
                *(s32*)((u8*)mps_sub + 0x158) = cur + skip_cnt;
                *out_flag = 0;
            }
        }
        return ret;
    }

    if (*out_flag & 0x1000) {
        sfmps_CopyPketData(self, buf + flags, size - flags, out_size, out_flag);
        ret = *out_flag;
        if (*out_flag == 1) {
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
        if (flags > 0) {
            *out_size = flags;
            *out_flag = flags;
        } else {
            *out_size = 1;
            *out_flag = 1;
        }
    }

    return ret;
}

void sfmps_pesfn(void* self, u8 stream_kind, s32 arg3, s32 arg4) {
    void (*cb)(s32, u8*, s32*, s32*, u32, s32*);
    s32 args[2];
    s32 stm_info[2];

    cb = (void (*)(s32, u8*, s32*, s32*, u32, s32*))(*(u32*)((u8*)self + 0xd5c));
    if (cb == NULL) return;

    args[0] = arg3;
    args[1] = arg4;
    stm_info[0] = *(s32*)((u8*)self + 0x9a0);
    stm_info[1] = *(s32*)((u8*)self + 0x9a4);

    cb(*(s32*)((u8*)self + 0xd60), &stream_kind, stm_info, args, 0, NULL);
}

void sfmps_SkipNext(void* self, s32 buf, s32 size, s32* out_size) {
    s32 skip_cnt = 0;
    s32 hdr_size = *(s32*)((u8*)self + 0x2c);

    *out_size = 0;

    if (size >= hdr_size + 3) {
        int all_zero = 1;
        s32 i;
        for (i = 0; i < hdr_size; i++) {
            if (*(s8*)(buf + i) != 0) {
                all_zero = 0;
                break;
            }
        }
        if (all_zero) {
            *out_size = hdr_size;
            return;
        }
    }

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
        s32 stream_idx = *(s32*)((u8*)self + 0x202c);
        s32* sub = (s32*)((u8*)self + stream_idx * 0x74);
        int is_wrap = 0;

        if (sub[0x13c8 / 4] == 0) {
            if (sub[0x13d8 / 4] == 0 && sub[0x13dc / 4] == 0) {
                is_wrap = 1;
            } else {
                is_wrap = 0;
            }
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

int sfmps_CopyPketData(void* self, s32 buf, s32 size, s32* out_size, s32* out_flag) {
    int ret = 0;
    void* mps_sub;
    s32 pket_buf[10];
    int pket_type;
    s32 pket_size;
    void* sj;

    *out_size = 0;
    *out_flag = 0;

    mps_sub = *(void**)((u8*)self + 0x2024);

    if (MPS_GetPketHd(*(void**)mps_sub, pket_buf)) {
        ret = SFLIB_SetErr(self, 0xff000d06);
    }

    pket_type = pket_buf[2];
    pket_size = pket_buf[3];

    if ((u32)(pket_type - 0xbc) > 0x43) {
        *out_flag = 1;
        SFLIB_SetErr(self, 0xff000d0f);
        return ret;
    }
    if (pket_size > 3) {
        *out_flag = 1;
        return 0;
    }
    if (pket_size < 0) {
        SFLIB_SetErr(self, 0xff000d0e);
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
        s32 (*get_avail)(void*, s32) = (s32(*)(void*, s32))vtbl[9];
        s32 (*read_fn)(void*, s32, s32, void*) = (s32(*)(void*, s32, s32, void*))vtbl[6];
        s32 (*commit_fn)(void*, s32, void*) = (s32(*)(void*, s32, void*))vtbl[8];
        s32 total_copied = 0;

        avail = get_avail(sj, 0);
        if (avail < pket_size) {
            *out_flag = 0;
        } else {
            void* tmp_buf;
            s32 tmp_size;
            void (*notify_fn)(void*, s32) = *(void(**)(void*, s32))((u8*)mps_sub + 0x150);
            void* notify_arg = *(void**)((u8*)mps_sub + 0x154);

            read_fn(sj, 0, pket_size, &tmp_buf);
            MEM_Copy((void*)buf, tmp_buf, tmp_size);
            commit_fn(sj, 1, &tmp_buf);

            if (tmp_size == 0) {
                *out_flag = 0;
            } else {
                s32 remaining = pket_size - tmp_size;
                buf += tmp_size;
                total_copied = 1;

                if (remaining > 0) {
                    read_fn(sj, 0, remaining, &tmp_buf);
                    MEM_Copy((void*)buf, tmp_buf, tmp_size);
                    commit_fn(sj, 1, &tmp_buf);
                    if (tmp_size != remaining) {
                        lbl_eu_80607AF0++;
                    }
                }

                if (total_copied == 1 && notify_fn != NULL) {
                    notify_fn(notify_arg, pket_type);
                }
                *out_flag = total_copied;
            }
        }
    } else {
        typedef int (*copy_func)(void*, s32, s32, s32*);
        extern copy_func lbl_eu_8051C930[];
        copy_func fn = lbl_eu_8051C930[pket_size];
        s32 result;
        s32 tmp[2];
        result = fn(self, buf, pket_size, tmp);
        *out_flag = result;
    }

    if (*out_flag == 1) {
        *out_size = pket_size;
    } else if (*out_flag != 0) {
        ret = *out_flag;
    }

    return ret;
}

int sfmps_CopyAudio(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_lo, s32 pts_hi) {
    void* mps_sub;
    s32 split_val;
    s32 cmp;

    if (SFSET_GetCond(self, 6) == 0)
        return 1;

    mps_sub = *(void**)((u8*)self + 0x2024);

    if (*(s32*)((u8*)mps_sub + 0x38) == -1)
        *(s32*)((u8*)mps_sub + 0x38) = stream_kind;
    if (*(s32*)((u8*)mps_sub + 0x30) == -1)
        *(s32*)((u8*)mps_sub + 0x30) = stream_kind;

    split_val = SFSET_GetCond(self, 0x1e);
    if (split_val != -1) {
        int skip = 0;
        if (SFSET_GetCond(self, 0x37) != 0) {
            s32 prev = *(s32*)((u8*)mps_sub + 0x28);
            s32 diff = stream_kind ^ prev;
            skip = ((diff >> 1) & ~(diff & prev)) >> 31;
        } else {
            skip = (*(s32*)((u8*)mps_sub + 0x30) == stream_kind) ? 1 : 0;
        }
        if (skip)
            *(s32*)((u8*)mps_sub + 0x38) = split_val;
    }

    *(s32*)((u8*)mps_sub + 0x28) = stream_kind;

    if (*(s32*)((u8*)mps_sub + 0x38) != stream_kind)
        return 1;

    if (pts_hi == 0 && pts_lo == 0) {
        /* PTS is zero; check if we should update min/max */
    } else {
        s32 min_lo = *(s32*)((u8*)mps_sub + 0x10);
        s32 min_hi = *(s32*)((u8*)mps_sub + 0x14);
        if (pts_hi < min_hi || (pts_hi == min_hi && pts_lo < min_lo)) {
            *(s32*)((u8*)mps_sub + 0x14) = pts_hi;
            *(s32*)((u8*)mps_sub + 0x10) = pts_lo;
        } else {
            *(s32*)((u8*)mps_sub + 0x14) = min_hi;
            *(s32*)((u8*)mps_sub + 0x10) = min_lo;
        }
        {
            s32 max_lo = *(s32*)((u8*)mps_sub + 0x18);
            s32 max_hi = *(s32*)((u8*)mps_sub + 0x1c);
            if (pts_hi > max_hi || (pts_hi == max_hi && pts_lo > max_lo)) {
                *(s32*)((u8*)mps_sub + 0x1c) = pts_hi;
                *(s32*)((u8*)mps_sub + 0x18) = pts_lo;
            } else {
                *(s32*)((u8*)mps_sub + 0x1c) = max_hi;
                *(s32*)((u8*)mps_sub + 0x18) = max_lo;
            }
        }
    }

    return sfmps_CopyDstBuft(self, *(s32*)((u8*)self + 0x2034), buf, size, pts_lo, pts_hi);
}

int sfmps_CopyVideo(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_lo, s32 pts_hi) {
    void* mps_sub;
    s32 split_val;

    if (SFSET_GetCond(self, 5) == 0)
        return 1;

    mps_sub = *(void**)((u8*)self + 0x2024);

    if (*(s32*)((u8*)mps_sub + 0x34) == -1) {
        s32 mode = SFSET_GetCond(self, 0x3b);
        s32 new_val;
        s32 sys_buf[6];

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

        MPS_GetSysHd(*(void**)mps_sub, sys_buf, 1);
        if (sys_buf[3] == -1) {
            SFSET_SetCond(self, 0x49, 1);
        }
    }

    if (*(s32*)((u8*)mps_sub + 0x2c) == -1)
        *(s32*)((u8*)mps_sub + 0x2c) = stream_kind;

    split_val = SFSET_GetCond(self, 0x1d);
    if (split_val != -1) {
        int skip = 0;
        if (SFSET_GetCond(self, 0x37) != 0) {
            s32 prev = *(s32*)((u8*)mps_sub + 0x24);
            s32 diff = stream_kind ^ prev;
            skip = ((diff >> 1) & ~(diff & prev)) >> 31;
        } else {
            skip = (*(s32*)((u8*)mps_sub + 0x2c) == stream_kind) ? 1 : 0;
        }
        if (skip && *(s32*)((u8*)mps_sub + 0x34) != split_val) {
            int is_video_start = 0;
            if (size >= 4) {
                u8* p = (u8*)buf;
                if (p[0] == 0 && p[1] == 0) {
                    if (p[2] == 1) {
                        is_video_start = (p[3] == 0xb3 || p[3] == 0xb8) ? 1 : 0;
                    }
                }
            }
            if (is_video_start)
                *(s32*)((u8*)mps_sub + 0x34) = split_val;
        }
    }

    *(s32*)((u8*)mps_sub + 0x24) = stream_kind;

    if (*(s32*)((u8*)mps_sub + 0x34) != stream_kind)
        return 1;

    return sfmps_CopyDstBuft(self, *(s32*)((u8*)self + 0x2030), buf, size, pts_lo, pts_hi);
}

int sfmps_CopyPrvate(void* self, s32 buf, s32 size, s32 a4) {
    s32 hdr_out;

    if (SFHDS_SetHdr(self, buf, &size, &hdr_out)) {
        if (hdr_out != 0) {
            sfmps_CopyUsrSj(self, 0, size - 0x12, a4 + 0x12);
        }
        return 1;
    }

    return sfmps_CopyUsrSj(self, buf, size, a4);
}

int sfmps_CopyUsrSj(void* self, s32 buf, s32 size, s32 out_kind) {
    void* sj;
    void* sj_vtbl;
    s32 avail;
    s32 tmp_buf, tmp_size;
    void (*notify_fn)(void*, s32);
    void (*notify2_fn)(void*, s32);
    void* notify_arg;
    void* notify2_arg;
    u32 uoch[4];

    if (*(s32*)((u8*)self + 0x2038) == 8)
        return 1;

    SFBUF_GetUoch(self, *(s32*)((u8*)self + 0x2038), buf, uoch);

    sj = (void*)uoch[0];
    notify_fn = (void(*)(void*, s32))uoch[1];
    notify_arg = (void*)uoch[2];
    notify2_fn = (void(*)(void*, s32))uoch[3];

    if (sj == NULL)
        return 1;

    sj_vtbl = *(void**)sj;
    avail = ((s32(*)(void*, s32))(*(void(**)(void*, s32))((u8*)sj_vtbl + 0x24)))(sj, 0);

    if (avail < size) {
        out_kind = 0;
    } else {
        void* tmp_ptr;
        s32 tmp_sz;
        s32 (*read_fn)(void*, s32, s32, void*) = (s32(*)(void*, s32, s32, void*))(*(void**)((u8*)sj_vtbl + 0x18));
        s32 (*commit_fn)(void*, s32, void*) = (s32(*)(void*, s32, void*))(*(void**)((u8*)sj_vtbl + 0x20));
        s32 total = 1;

        read_fn(sj, 0, size, &tmp_ptr);
        MEM_Copy((void*)buf, tmp_ptr, tmp_sz);
        commit_fn(sj, 1, &tmp_ptr);

        if (tmp_sz == 0) {
            out_kind = 0;
        } else {
            s32 remaining = size - tmp_sz;
            buf += tmp_sz;

            if (remaining > 0) {
                read_fn(sj, 0, remaining, &tmp_ptr);
                MEM_Copy((void*)buf, tmp_ptr, tmp_sz);
                commit_fn(sj, 1, &tmp_ptr);
                if (tmp_sz != remaining) {
                    lbl_eu_80607AF0++;
                }
            }

            if (total == 1) {
                if (notify_fn != NULL)
                    notify_fn(notify_arg, buf);
                if (notify2_fn != NULL)
                    notify2_fn(notify2_arg, buf);
            }
            return total;
        }
    }

    if (out_kind == 1) {
        if (notify_fn != NULL)
            notify_fn(notify_arg, buf);
        if (notify2_fn != NULL)
            notify2_fn(notify2_arg, buf);
    }

    return out_kind;
}

int sfmps_CopyPadding(void) {
    return 1;
}

int sfmps_CopyDstBuft(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_lo, s32 pts_hi) {
    s32 ring_buf[6];
    void* mps_sub;
    s32 write_pos;
    s32 total_size;
    s32 first_size;
    void* first_ptr;
    void* second_ptr;

    if (SFBUF_RingGetWrite(self, stream_kind, ring_buf))
        return 0;

    write_pos = ring_buf[1];
    total_size = ring_buf[3];
    first_size = ring_buf[2];
    first_ptr = (void*)ring_buf[0];
    second_ptr = (void*)ring_buf[4];

    if (size > write_pos + total_size)
        return 0;

    if (stream_kind == 1) {
        extern s32 (*lbl_eu_80619BAC)(void*, void*);
        if (lbl_eu_80619BAC != NULL) {
            s32 data[3];
            data[0] = pts_lo;
            data[1] = pts_hi;
            data[2] = size;
            if (lbl_eu_80619BAC((u8*)self + 0x1374, data) == -1)
                return 0;
        }

        if (pts_hi == 0 && pts_lo == 0) {
            /* no PTS check */
        } else {
            if (SFPTS_IsPtsQueFull(self, stream_kind)) {
                return 0;
            } else {
                s32 pts_data[4];
                pts_data[0] = pts_lo;
                pts_data[1] = pts_hi;
                pts_data[2] = write_pos;
                pts_data[3] = size;
                if (SFPTS_WritePtsQue(self, stream_kind, pts_data, ring_buf))
                    return 0;
            }
        }
    } else if (stream_kind == 2) {
        extern s32 (*lbl_eu_80619BAC)(void*, void*);
        if (lbl_eu_80619BAC != NULL) {
            s32 data[3];
            data[0] = pts_lo;
            data[1] = pts_hi;
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

    mps_sub = *(void**)((u8*)self + 0x2024);
    stream_idx = *(s32*)((u8*)self + 0x202c);

    if (size >= 4) {
        delim = MPS_CheckDelim((const u8*)buf);
        if ((delim & 0x08000000) && *(s32*)((u8*)self + 0x203c) < 0) {
            *(s32*)((u8*)self + 0x203c) = SFBUF_GetRTot(self, stream_idx) + 4;
        }
        if (delim & 0x08000000) {
            *(s32*)((u8*)mps_sub + 0x3c) = 1;
        } else if (delim != 0) {
            *(s32*)((u8*)mps_sub + 0x3c) = 0;
        }
    } else {
        delim = 0;
    }

    {
        int is_endcode = 0;
        if ((delim & 0x08000000) == 0) {
            is_endcode = 0;
        } else if (SFCON_IsEndcodeSkip(self) || SFCON_IsSystemEndcodeSkip(self)) {
            is_endcode = 0;
        } else {
            is_endcode = 1;
        }

        if (is_endcode) {
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
        int is_end;
        if ((delim & 0x00010000) == 0 && (delim & 0x00040000) == 0) {
            is_end = 0;
        } else {
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
    }

    return 1;
}

void sfmps_GetStmNum(void* self, s32* out_a, s32* out_b) {
    void* mps_sub;
    void* mps_work;
    s32 max_a = 0, max_b = 0;
    int i;
    s32 sys_buf[6];

    mps_sub = *(void**)((u8*)self + 0x2024);
    mps_work = *(void**)mps_sub;

    for (i = 0; i < 3; i++) {
        MPS_GetSysHd(mps_work, sys_buf, i);
        if (max_a < sys_buf[2])
            max_a = sys_buf[2];
        if (max_b < sys_buf[3])
            max_b = sys_buf[3];
    }

    *(s32*)((u8*)mps_sub + 0x8) = max_a;
    *(s32*)((u8*)mps_sub + 0x4) = max_b;
    *out_a = max_a;
    *out_b = *(s32*)((u8*)mps_sub + 0x4);
}

void sfmps_SetMvInf(void* self) {
    void* mps_sub;
    void* mps_work;
    s32 pack_buf[6];
    s32 sys_buf[6];

    mps_sub = *(void**)((u8*)self + 0x2024);
    mps_work = *(void**)mps_sub;

    MPS_GetPackHd(mps_work, pack_buf);
    if (pack_buf[3] >= 1)
        *(s32*)((u8*)self + 0x934) = pack_buf[3];

    MPS_GetSysHd(mps_work, sys_buf, 1);
    if (sys_buf[6] != -1)
        *(s32*)((u8*)self + 0x938) = sys_buf[6];

    if (*(s32*)((u8*)self + 0x940) == -1)
        *(s32*)((u8*)self + 0x940) = *(s32*)((u8*)mps_sub + 0x8);
    if (*(s32*)((u8*)self + 0x944) == -1)
        *(s32*)((u8*)self + 0x944) = *(s32*)((u8*)mps_sub + 0x4);
}

void sfmps_SetMpsHd(void* self) {
    s32* hdr;
    void* mps_sub;

    hdr = *(s32**)((u8*)self + 0x2670);
    if (hdr == NULL) {
        hdr = NULL;
    } else {
        mps_sub = *(void**)((u8*)self + 0x2024);
        if (*(s32*)((u8*)mps_sub + 0x20) > 0)
            hdr = NULL;
        else
            hdr = (s32*)((u8*)hdr + 0x8a0);
    }

    if (hdr == NULL) return;

    {
        s32* sub = *(s32**)((u8*)self + 0x2024);
        s32 hi = sub[4];
        s32 lo = sub[5];
        s32 mask_hi = 0x7fffffff;
        s32 mask_lo = -1;

        if ((hi ^ mask_hi) == 0 && (lo ^ mask_lo) == 0)
            return;

        {
            s32 diff_lo = lo - hdr[9];
            s32 diff_hi = hi - hdr[8];
            *(s32*)((u8*)self + 0xef4) = diff_lo;
            *(s32*)((u8*)self + 0xef0) = diff_hi;
        }

        if (hdr[0] != 0) return;

        hdr[1] = *(s32*)((u8*)self + 0x934) * 50;
        hdr[2] = *(s32*)((u8*)self + 0x938);
        hdr[3] = sub[1];
        hdr[4] = sub[2];
        hdr[6] = *(s32*)((u8*)self + 0xeec);
        hdr[7] = *(s32*)((u8*)self + 0xee8);
        hdr[8] = sub[4];
        hdr[9] = sub[5];
        hdr[10] = sub[11];
        hdr[11] = sub[12];
    }
}

void sfmps_SetMpsRaw(void* self, void* mps_work, s32 buf, s32 size) {
    s32* hdr;
    s32* sub;
    s32 sys_buf[6];
    s32* raw_hdr;
    s32 copy_size;

    hdr = *(s32**)((u8*)self + 0x2670);
    if (hdr == NULL) {
        raw_hdr = NULL;
    } else {
        sub = *(s32**)((u8*)self + 0x2024);
        if (sub[8] > 0)
            raw_hdr = NULL;
        else
            raw_hdr = (s32*)((u8*)hdr + 0x8a0);
    }

    if (raw_hdr == NULL) return;
    if (raw_hdr[0] != 0) return;

    raw_hdr = (s32*)((u8*)raw_hdr + 0x30);
    MPS_GetLastSysHd(mps_work, sys_buf);

    copy_size = (size >= 0xb0) ? 0xb0 : size;

    if (sys_buf[3] > 0) {
        raw_hdr[0x160 / 4] = copy_size;
    } else if (sys_buf[2] > 0) {
        raw_hdr[0x164 / 4] = copy_size;
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
        SFLIB_SetErr(NULL, 0xff000d08);
        return NULL;
    }

    if (MPS_GetErrInf(mps, sfmps_ErrFn, self)) {
        MPS_Destroy(mps);
        SFLIB_SetErr(NULL, 0xff000d09);
        return NULL;
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
    s32 max_val = 0x7fffffff;
    s32 neg1 = -1;
    int i;

    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = max_val;
    p[5] = neg1;
    p[6] = max_val;
    p[7] = neg1;
    p[8] = 0;
    p[9] = max_val;
    p[10] = max_val;
    p[11] = neg1;
    p[12] = neg1;
    p[13] = neg1;
    p[14] = neg1;
    p[15] = 0;

    for (i = 0; i < 2; i++) {
        s32* base = &p[16 + i * 32];
        int j;
        for (j = 0; j < 32; j++) {
            base[j] = 0;
        }
    }

    p[16 + 2 * 32] = 0;
    p[16 + 2 * 32 + 1] = 0;
    p[16 + 2 * 32 + 2] = 0;
    p[16 + 2 * 32 + 3] = 0;
    p[0x150 / 4] = 0;
    p[0x154 / 4] = 0;
    p[0x158 / 4] = -1;
}

s32 sfmps_ErrFn(void* h, u32 err_code) {
    return SFLIB_SetErr(h, err_code);
}

int SFMPS_Destroy(void* self) {
    void* mps_sub = *(void**)((u8*)self + 0x2024);
    void* mps = *(void**)mps_sub;

    if (MPS_Destroy(mps)) {
        return SFLIB_SetErr(self, 0xff000d0a);
    }
    return 0;
}

int SFMPS_RequestStop(void) { return 0; }
int SFMPS_Start(void) { return 0; }
int SFMPS_Stop(void) { return 0; }
int SFMPS_Pause(void) { return 0; }

s32 SFMPS_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000d0b);
}

s32 SFMPS_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000d0b);
}

s32 SFMPS_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000d0b);
}

s32 SFMPS_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000d0b);
}

int SFMPS_Seek(void* self) {
    s32* hdr;
    void* mps_sub;
    void* mps_work;
    s32* raw_hdr;
    int ret;

    hdr = *(s32**)((u8*)self + 0x2670);
    if (hdr == NULL)
        return 0;
    if (*(s32*)(*(s32**)((u8*)self + 0x2024) + 0x20) > 0)
        return 0;

    raw_hdr = (s32*)((u8*)hdr + 0x8a0);
    if (raw_hdr == NULL)
        return 0;
    if (raw_hdr[0] != 0)
        return 0;

    mps_sub = *(void**)((u8*)self + 0x2024);
    SFHDS_ReprocessHdr(self);
    mps_work = *(void**)mps_sub;

    ret = MPS_DecHd(mps_work, (u8*)raw_hdr + 0x30, raw_hdr[0x190 / 4], &ret, &ret);
    {
        int ret2 = MPS_DecHd(mps_work, (u8*)raw_hdr + 0x30 + 0xb0, raw_hdr[0x164 / 4], &ret, &ret);
        if (ret == 0 && ret2 == 0) {
            ret = 0;
        } else {
            ret = SFLIB_SetErr(self, 0xff000d0d);
        }
    }

    if (ret != 0)
        return ret;

    *(s32*)((u8*)mps_sub + 0x2c) = raw_hdr[10];
    *(s32*)((u8*)mps_sub + 0x30) = raw_hdr[11];
    *(s32*)((u8*)self + 0xeec) = raw_hdr[7];
    *(s32*)((u8*)self + 0xee8) = raw_hdr[6];
    *(s32*)((u8*)mps_sub + 0x14) = raw_hdr[9];
    *(s32*)((u8*)mps_sub + 0x10) = raw_hdr[8];

    return 0;
}

u32 SFMPS_GetConcatCnt(void* self) {
    void* ptr = *(void**)((u8*)self + 0x2024);
    return *(u32*)((u8*)ptr + 0x20);
}
