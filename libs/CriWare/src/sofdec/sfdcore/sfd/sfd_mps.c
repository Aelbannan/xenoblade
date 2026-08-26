// Decompiled CriWare sfd_mps TU - high-level C reconstruction
// Matches retail assembly at 0x803C3798..0x803C570C

#include <harness_catalog.h>

int SFD_SetElementOutSj(void* self, s32 idx, s32 data, s32 arg3, s32 arg4);
int SFMPS_Init(void);int SFMPS_Finish(void);
s32 SFMPS_ExecServer(void* self);
int sfmps_DecodeSomeUnit(void* self);
int criware_803C1490(void* self, s32* out_a, s32* out_b, s32 unused, s32* out_c);
int sfmps_DecodeOneUnit(void* self, s32 buf, s32 size, s32* out_size, s32* out_flag, s32 a5);
void sfmps_pesfn(void* self, u8 stream_kind, s32 arg3, s32 arg4);
void sfmps_SkipNext(void* self, s32 buf, s32 size, s32* out_size);
int sfmps_CopyPketData(void* self, s32 buf, s32 size, s32* out_size, s32* out_flag);
int sfmps_CopyAudio(void* self, s32 stream_kind, s32 buf, s32 size, s64 pts);
int sfmps_CopyVideo(void* self, s32 stream_kind, s32 buf, s32 size, s64 pts);
int sfmps_CopyPrvate(void* self, s32 kind, s32 buf, s32 size);
int sfmps_CopyUsrSj(void* self, s32 buf, s32 size, s32 out_kind);
int sfmps_CopyPadding(void);
int sfmps_CopyDstBuft(void* self, s32 stream_kind, s32 buf, s32 size, s64 pts);
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
// 4-byte wrapper thunk at .text:0x803C34F8 that SFMPS_Init's error path
// calls (retail symbol fn_803C34F8; SFLIB_SetErr proper sits at +4).
extern s32 fn_803C34F8(s32 val, u32 err_code);
void SFSET_SetCond(void* self, u32 idx, u32 val);
u32 SFSET_GetCond(void* self, u32 idx);
void SFBUF_SetPrepFlg(void* self, u32 idx, u32 val);
int SFBUF_GetPrepFlg(void* self, int idx);
void SFBUF_SetTermFlg(void* buf, s32 idx, u32 flg);
int SFBUF_GetTermFlg(void* self, int idx);
u32 SFBUF_GetRTot(void* self, u32 idx);
s32 SFBUF_GetWTot(void* self, u32 idx);
s32 SFBUF_RingGetRead(void* self, u32 idx, s32* out);
s32 SFBUF_RingGetWrite(void* self, u32 idx, void* out);
s32 SFBUF_RingAddRead(void* self, u32 idx, u32 size);
s32 SFBUF_RingAddWrite(void* self, u32 idx, u32 size, void* extra);
void SFBUF_GetFlowCnt(void* self, u32* readCnt, u32* writeCnt);
u64 SFBUF_UpdateFlowCnt(u32 hi, u32 lo, u32 x);
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
        return fn_803C34F8(0, 0xff000d01);
    }
    lbl_eu_80607AF0 = 0;
    return 0;
}

int SFMPS_Finish(void) {
    MPS_Finish();
    return 0;
}

/* Server tick: notify callback head/tail, terminate-flag gate, decode pass,
   then (once stream counts are known) buffer-prep bookkeeping and the two
   auto-clear cond conditions. */s32 SFMPS_ExecServer(void* self) {
    s32 flg;
    s32 ret;
    u32* sub;

    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_805687F4[3] = (u32)self;
        ((void (*)(void*, void*))(*(u32*)((u8*)*(u32*)lbl_eu_80606E34 + 0x24)))
            (lbl_eu_80606E34, &lbl_eu_805687F4[1]);
    }

    /* all three ring buffers terminated? */
    flg = 1;
    if (*(s32*)((u8*)self + 0x2034) != 8)
        flg = SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2034)) & 1;
    if (*(s32*)((u8*)self + 0x2030) != 8)
        flg = flg & SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2030));
    if (*(s32*)((u8*)self + 0x2038) != 8)
        flg = flg & SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2038));

    if (flg == 1) {
        ret = 0;
    } else {
        sub = *(u32**)((u8*)self + 0x2024);
        MPS_SetSystemFn((void*)sub[0],
                        *(void**)((u8*)self + 0xd44),
                        *(void**)((u8*)self + 0xd48));
        ret = sfmps_DecodeSomeUnit(self);

        if (*(s32*)((u8*)self + 0x54) == 2) {
            s32 num_a, num_b;
            sfmps_GetStmNum(self, &num_a, &num_b);

            /* any ring prepared? */
            flg = 0;
            if (*(s32*)((u8*)self + 0x2034) != 8)
                flg = SFBUF_GetPrepFlg(self, *(s32*)((u8*)self + 0x2034));
            if (*(s32*)((u8*)self + 0x2030) != 8)
                flg = flg | SFBUF_GetPrepFlg(self, *(s32*)((u8*)self + 0x2030));
            if (*(s32*)((u8*)self + 0x2038) != 8)
                flg = flg | SFBUF_GetPrepFlg(self, *(s32*)((u8*)self + 0x2038));

            if (flg != 1) {
                /* input ring caught up to the decode watermark -> mark all
                   rings prepared; watermark = clamp(f8, fallbacks, max).
                   The current-ring index is re-read after each call. */
                if (SFBUF_GetPrepFlg(self, *(s32*)((u8*)self + 0x202c)) == 1) {
                    u32* rec = (u32*)((u8*)self + *(s32*)((u8*)self + 0x202c) * 0x74);
                    s32 lim = *(s32*)((u8*)self + 0x8);
                    s32 maxb = *(s32*)((u8*)self + 0xa74);

                    if (lim <= 0) lim = *(s32*)((u8*)rec + 0x13d4);
                    if (lim <= 0) lim = maxb;
                    if (lim < maxb) maxb = lim;

                    if (SFBUF_GetWTot(self, 0) >= maxb) {
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

            sub = *(u32**)((u8*)self + 0x2024);

            /* video stream fully read and transferred -> clear cond 6 */
            if (SFSET_GetCond(self, 6) != 0) {
                if (SFSET_GetCond(self, 0x50) != 0) {
                    if (SFBUF_GetWTot(self, 2) == 0) {
                        if (SFTRN_GetPrepFlg(self, 6) != 0) {
                            SFSET_SetCond(self, 6, 0);
                        }
                    }
                }
            }

            /* audio stream fully read and transferred -> clear cond 5 */
            if (SFSET_GetCond(self, 5) != 0) {
                if (SFSET_GetCond(self, 0x4f) != 0) {
                    if (SFBUF_GetWTot(self, 1) == 0) {
                        if (sub[1] == 0) {
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
        ((void (*)(void*, void*))(*(u32*)((u8*)*(u32*)lbl_eu_80606E34 + 0x24)))
            (lbl_eu_80606E34, &lbl_eu_805687F4[0x6c / 4]);
    }

    return ret;
}

int sfmps_DecodeSomeUnit(void* self) {
    int ret = 0;
    s32 limit = *(s32*)((u8*)self + 0x2c);
    s32 cond_val = SFSET_GetCond(self, 0x4b);
    s32 total = 0;
    s32 read_size, data_size, out_size, dummy, out_flag;
    u32 flow_w, flow_r;
    s32 add_size;

    for (;;) {
        if (*(s32*)((u8*)self + 0x70) != 0)
            break;

        ret = criware_803C1490(self, &read_size, &data_size, limit, &dummy);
        if (ret != 0) break;

        ret = sfmps_DecodeOneUnit(self, read_size, data_size, &out_size, &out_flag, dummy);
        if (ret != 0) break;

        /* 64-byte-read and 64-byte-skip counters are hi/lo pairs */
        *(s64*)((u8*)self + 0x9a0) += out_size;
        *(s64*)((u8*)self + 0x9a8) += out_flag;

        if (out_size == 0)
            break;

        {
            add_size = out_size;
            ret = SFBUF_RingAddRead(self, *(s32*)((u8*)self + 0x202c), add_size);
            if (ret == 0) {
                *(s32*)((u8*)self + 0x39a8) += add_size;
                ret = 0;
            }
        }
        if (ret != 0)
            break;
        total += out_size + out_flag;
        if (cond_val != -1)
            if (total >= cond_val)
                break;
    }

    SFBUF_GetFlowCnt(*(void**)((u8*)self + 0x13cc), &flow_r, &flow_w);
    {
        *(u64*)((u8*)self + 0x998) = SFBUF_UpdateFlowCnt(*(u32*)((u8*)self + 0x998),
                                                         *(u32*)((u8*)self + 0x99c),
                                                         flow_r);
    }

    return ret;
}

int criware_803C1490(void* self, s32* out_a, s32* out_b, s32 unused, s32* out_c) {
    /* server notify bookkeeping block at self+0x39a0 */
    struct SfmpsNotify {
        u8 pad[0x202c];
        s32 field_0x202c;
        u8 pad2[0x39a0 - 0x2030];
        u32 field_0x39a0;
        u32 field_0x39a4;
        u32 field_0x39a8;
        u32 field_0x39ac;
    };
    struct SfmpsNotify* work = (struct SfmpsNotify*)self;
    s32 ret;
    s32 idx;
    u32 addr;
    s32 tmp[10];

    idx = work->field_0x202c;
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
        addr = *out_a;
        if (addr != work->field_0x39ac) {
            u32 accum = work->field_0x39a8;
            if ((accum & 0x7ff) == 0) {
                void (*cb)(s32, s32) = (void (*)(s32, s32))work->field_0x39a0;
                s32 arg = work->field_0x39a4;
                if (cb != NULL)
                    cb(arg, addr);
                work->field_0x39ac = *out_a;
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
    void* mps_sub;
    void* mps_work;
    int has_delim;
    int flags2;
    int flags;
    int cond_val;

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

    /* endcode flag check is an exact equality test against 0x80000;
       whole dispatch is one if/else chain so all exits share one tail */
    if ((flags2 == 0x80000) && SFCON_IsEndcodeSkip(self)) {
        /* counter bumped through a fresh re-read of the mps handle */
        *(s32*)((u8*)*(void**)((u8*)self + 0x2024) + 0x20) += 1;
        *out_size = cond_val;
        *(s32*)((u8*)mps_sub + 0x158) = cond_val;
    } else if ((flags2 == 0x80000) && SFCON_IsSystemEndcodeSkip(self)) {
        *out_size = cond_val;
        *(s32*)((u8*)mps_sub + 0x158) = cond_val;
    } else if (!has_delim) {
        s32 skip_cnt;
        s32 hdr_size;
        sfmps_SkipNext(self, buf, size, out_flag);
        *out_size = *out_flag;
        skip_cnt = *out_flag;
        if (skip_cnt > 0) {
            /* current packet offset kept as an anonymous CSE'd temp */
            if (*(s32*)((u8*)mps_sub + 0x158) >= 0) {
                hdr_size = *(s32*)((u8*)self + 0x2c);
                if (*(s32*)((u8*)mps_sub + 0x158) >= hdr_size) {
                    *(s32*)((u8*)mps_sub + 0x158) = *(s32*)((u8*)mps_sub + 0x158) + skip_cnt;
                } else if (*(s32*)((u8*)mps_sub + 0x158) + skip_cnt > hdr_size) {
                    *out_flag = skip_cnt - (hdr_size - *(s32*)((u8*)mps_sub + 0x158));
                    *(s32*)((u8*)mps_sub + 0x158) = *(s32*)((u8*)self + 0x2c) + *out_flag;
                } else {
                    *(s32*)((u8*)mps_sub + 0x158) = *(s32*)((u8*)mps_sub + 0x158) + skip_cnt;
                    *out_flag = 0;
                }
            }
        }
    } else if (!(flags2 & 0x40000)) {
        int term;
        if (SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x202c)) == 1) {
            SFTRN_SetTermFlg(self, 1, 1);
            if (*(s32*)((u8*)self + 0x2034) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
            if (*(s32*)((u8*)self + 0x2030) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
            if (*(s32*)((u8*)self + 0x2038) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
            term = 1;
        } else {
            term = 0;
        }
        if (term == 0 && size > *(s32*)((u8*)self + 0x2c)) {
            if (flags > 0) {
                *out_size = flags;
                *out_flag = flags;
            } else {
                *out_size = 1;
                *out_flag = 1;
            }
        }
    } else {
        /* packet copy: 4th arg gets header-consumed count, 5th arg a status */
        s32 pket_size;
        s32 pket_stat;
        ret = sfmps_CopyPketData(self, buf + flags, size - flags, &pket_size, &pket_stat);
        if (pket_stat == 1) {
            *out_size = flags + pket_size;
        }
        *(s32*)((u8*)mps_sub + 0x158) = -1;
    }

    return ret;
}

// FULL_MATCH 100% (Wii/1.1, c99). Retail hoists both loads (lwz r0,2464 /
// lwz r5,2468) before both stores, s0 first. 14+ shapes probed without
// volatile: locals reverse BOTH load order (s1,s0) AND store order ([0],[1]);
// inline stores interleave load-store-load-store. KEY LEVER: volatile on the
// pair of reads forces MWCC to keep BOTH loads in source order (s0 first) and
// hoist them ahead of the stores — byte-identical to retail. (Volatile is
// safe here: self+0x9a0/0x9a4 are state fields read once; no aliasing in scope.)
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
    s32 st0 = *(volatile s32*)((u8*)self + 0x9a0);
    s32 st1 = *(volatile s32*)((u8*)self + 0x9a4);
    inf.stm_info[1] = st1;
    inf.stm_info[0] = st0;
    cb(*(s32*)((u8*)self + 0xd60), &inf, st1, arg4);
}

void sfmps_SkipNext(void* self, s32 buf, s32 size, s32* out_size) {
    s32 skip_cnt;
    u8* p;
    s32 i;
    s32 hdr_size;
    s32 all_zero;
    s32 idx;
    s32* rec;
    s32 is_wrap;

    *out_size = 0;
    hdr_size = *(s32*)((u8*)self + 0x2c);

    if (size >= hdr_size + 3) {
        /* header-sized run of NUL bytes: skip it wholesale */
        p = (u8*)buf;
        for (i = 0; i < hdr_size; i++) {
            if (*(s8*)p++ != 0) {
                all_zero = 0;
                goto nonzero;
            }
        }
        all_zero = 1;
nonzero:
        if (all_zero) {
            *out_size = hdr_size;
            return;
        }
    }

    skip_cnt = 0;

    while (4 <= size) {
        if (MPS_CheckDelim((const u8*)buf) & 0x000d0000) {
            *out_size = skip_cnt;
            return;
        }
        skip_cnt++;
        buf++;
        size--;
    }

    /* trailing partial header: treated as wrapped if it abuts the ring wrap point */
    /* RESIDUAL (us-803c41ac): retail splits these guards into cmpwi r29,0 / ble
       + bge cr1 (cr1 CSE'd from the loop-head cmpwi cr1,r29,4, incl. one dead
       re-test). Every source spelling tried (&&, nested ifs, !(size>=4),
       reversed operands) still fuses into subi/cmpli range checks here.
       Best state: mismatch 32 / structural 18 / reg_swap 14 (banked was 50/29). */
    if (size > 0 && size < 4) {
            idx = *(s32*)((u8*)self + 0x202c);
            rec = (s32*)((u8*)self + idx * 0x74);

            if (rec[0x4f2] == 0 && (rec[0x4f6] != 0 || rec[0x4f7] != 0)) {
                is_wrap = 0;
            } else {
                is_wrap = (rec[0x4f4] + rec[0x4f5]) == (buf + size);
            }

            if (is_wrap) {
                skip_cnt += size;
                size = 0;
            }
    }

    if (size > 0) {
        if (!(size >= 4)) {
            if (SFSET_GetCond(self, 0x55) != 0) {
                skip_cnt += size;
            }
        }
    }

    *out_size = skip_cnt;
}

/* Packet-header record filled in by MPS_GetPketHd (sp+0x48 in retail) */
typedef struct SfmpsPketHd {
    s32 f00;         /* 0x00 */
    s32 f04;         /* 0x04 */
    u8 pad08[8];
    s32 f10;         /* 0x10: stream id (0xbc..0xff), indexes the SJ table */
    s32 f14;         /* 0x14: sub-stream kind (must be <= 3) */
    s32 f18;         /* 0x18 */
    u8 pad1c[8];
    s32 f24;         /* 0x24: packet payload size */
} SfmpsPketHd;

int sfmps_CopyPketData(void* self, s32 buf, s32 size, s32* out_size, s32* out_flag) {
    int ret = 0;
    /* ring-read result pairs (ptr,size); kept in distinct slots like retail */
    struct { void* p; s32 n; } t2;
    struct { void* p; s32 n; } t1;
    SfmpsPketHd hd;
    /* scratch block handed to the per-kind copy routines (sp+0x18 in retail) */
    s32 copy_blk[12];
    s32 copied;
    s32* mps_sub;
    s32 stm_id;
    s32 pkt_size;
    void* sj;

    *out_size = 0;
    *out_flag = 0;

    mps_sub = *(s32**)((u8*)self + 0x2024);

    if (MPS_GetPketHd(*(void**)mps_sub, &hd)) {
        ret = SFLIB_SetErr((s32)self, 0xff000d06);
    }

    stm_id = hd.f10;

    if ((u32)(stm_id - 0xbc) > 0x43) {
        *out_flag = 1;
        return SFLIB_SetErr((s32)self, 0xff000d0f);
    }

    pkt_size = hd.f24;

    if (hd.f14 > 3) {
        *out_flag = 1;
        return 0;
    }
    if (pkt_size < 0) {
        return SFLIB_SetErr((s32)self, 0xff000d0e);
    }
    if (pkt_size == 0) {
        *out_size = 0;
        *out_flag = 1;
        return 0;
    }

    if (size < pkt_size) {
        /* truncated packet: flush the pipeline if this stream already ended */
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

    sj = *(void**)((u8*)mps_sub + 0x40 + (stm_id - 0xbc) * 4);
    if (sj != NULL) {
        /* SJ present: copy the packet through the ring-buffer object.
           The vtable pointer is re-read before each indirect call. */
        /* cache the notify callback; reuses the by-then-dead self/size slots */
        self = *(void**)((u8*)mps_sub + 0x154);
        size = *(s32*)((u8*)mps_sub + 0x150);

        if ((*(s32 (**)(void*, s32))((u8*)*(void**)sj + 0x24))(sj, 0) < pkt_size) {
            copied = 0;
        } else {
            (*(void (**)(void*, s32, s32, void*))((u8*)*(void**)sj + 0x18))(sj, 0, pkt_size, &t1);
            MEM_Copy((void*)buf, t1.p, t1.n);
            (*(void (**)(void*, s32, void*))((u8*)*(void**)sj + 0x20))(sj, 1, &t1);

            if (t1.n == 0) {
                copied = 0;
            } else {
                /* wrapped copy: remainder comes from the ring start;
                   reuses the flag slot like retail */
                copied = pkt_size - t1.n;
                buf += t1.n;
                if (copied > 0) {
                    (*(void (**)(void*, s32, s32, void*))((u8*)*(void**)sj + 0x18))(sj, 0, copied, &t2);
                    MEM_Copy((void*)buf, t2.p, t2.n);
                    (*(void (**)(void*, s32, void*))((u8*)*(void**)sj + 0x20))(sj, 1, &t2);
                    if (t2.n != copied) {
                        lbl_eu_80607AF0++; /* global underrun counter */
                    }
                }
                copied = 1;
            }
        }
        *out_flag = copied;
        if (copied == 1 && size != 0) {
            ((void (*)(void*, s32))size)(self, stm_id);
        }
    } else {
        /* no SJ: route through the per-kind copy dispatch table */
        typedef s32 (*copy_fn_t)(void*, s32, s32, s32, s32, s32, s32*);
        copy_fn_t fn = ((copy_fn_t*)lbl_eu_8051C930)[hd.f14];
        *out_flag = fn(self, hd.f18, buf, pkt_size, hd.f00, hd.f04, copy_blk);
    }

    if (*out_flag == 1) {
        *out_size = pkt_size;
    } else if (*out_flag != 0) {
        ret = *out_flag;
    }

    return ret;
}

/* MPS sub-work layout (only fields touched by sfmps_CopyAudio) */
typedef struct SfmpsWork {
    u8 pad0[0x10];
    s64 ptsMin;    /* 0x10 */
    s64 ptsMax;    /* 0x18 */
    u8 pad20[0x08];
    s32 lastAudio; /* 0x28 */
    s32 lastVideo; /* 0x2c */
    s32 firstAudio;/* 0x30 */
    s32 f34;
    s32 audioCh;   /* 0x38 */
    s32 f3c;
} SfmpsWork;

int sfmps_CopyAudio(void* self, s32 stream_kind, s32 buf, s32 size, s64 pts) {
    SfmpsWork* work;
    s32 split_val;
    int skip;
    s32 v;

    if (SFSET_GetCond(self, 6) == 0)
        return 1;

    work = *(SfmpsWork**)((u8*)self + 0x2024);

    if (work->audioCh == -1)
        work->audioCh = stream_kind;
    if (work->firstAudio == -1)
        work->firstAudio = stream_kind;

    split_val = SFSET_GetCond(self, 0x1e);
    if (split_val != -1) {
        if (SFSET_GetCond(self, 0x37) != 0) {
            /* sign bit of ((v>>1)-(v&prev)); true iff new stream differs from
               the tracked one under the current split mode. Compound ops keep
               MWCC's schedule as shift-then-mask. Residual: pure reg-swap,
               decomp colors {load,v}={r0,r4}, retail {r4,r0} - allocator
               artifact, no source lever found (see session notes). */
            v = work->lastAudio ^ stream_kind;
            skip = v >> 1;
            v &= work->lastAudio;
            skip = (u32)(skip - v) >> 31;
        } else {
            v = (stream_kind == work->firstAudio) ? 1 : 0;
            skip = v;
        }
        if (skip)
            work->audioCh = split_val;
    }

    work->lastAudio = stream_kind;

    if (work->audioCh != stream_kind)
        return 1;

    /* running PTS min/max clamp (guarded by pts > 0, borrow-chain compare) */
    {
        if (pts >= 0) {
            s64 min = work->ptsMin;
            work->ptsMin = (pts < min) ? pts : min;
            s64 max = work->ptsMax;
            work->ptsMax = (pts < max) ? pts : max;
        }
    }

    return sfmps_CopyDstBuft(self, *(s32*)((u8*)self + 0x2034), buf, size, pts);
}

int sfmps_CopyVideo(void* self, s32 stream_kind, s32 buf, s32 size, s64 pts) {
    void* mps_sub;
    s32 split_val;

    if (SFSET_GetCond(self, 5) == 0)
        return 1;

    mps_sub = *(void**)((u8*)self + 0x2024);

    if (*(s32*)((u8*)mps_sub + 0x34) == -1) {
        s32 new_val;
        s32 nb, na;

        switch ((s32)SFSET_GetCond(self, 0x3b)) {
        case 1:
            new_val = stream_kind;
            break;
        case 2:
            sfmps_GetStmNum(self, &na, &nb);
            if (nb >= 2)
                new_val = 2;
            else
                new_val = stream_kind;
            break;
        default:
            new_val = stream_kind;
            break;
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
        int skip;
        if (SFSET_GetCond(self, 0x37) != 0) {
            /* sign bit of ((x >> 1) - (x & prev)), x = prev ^ stream_kind */
            s32 prev = *(s32*)((u8*)mps_sub + 0x24);
            s32 x = prev ^ stream_kind;
            skip = (u32)((x >> 1) - (x & prev)) >> 31;
        } else {
            skip = (*(s32*)((u8*)mps_sub + 0x2c) == stream_kind);
        }
        if (skip && *(s32*)((u8*)mps_sub + 0x34) != split_val) {
            /* only switch on a real GOP/sequence start code */
            u8* p = (u8*)buf;
            int is_video = size >= 4 && p[0] == 0 && p[1] == 0 && p[2] == 1 &&
                           (p[3] == 0xb3 || p[3] == 0xb8);
            if (is_video)
                *(s32*)((u8*)mps_sub + 0x34) = split_val;
        }
    }

    *(s32*)((u8*)mps_sub + 0x24) = stream_kind;

    if (*(s32*)((u8*)mps_sub + 0x34) != stream_kind)
        return 1;

    return sfmps_CopyDstBuft(self, *(s32*)((u8*)self + 0x2030), buf, size, pts);
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

int sfmps_CopyUsrSj(void* self, s32 src, s32 dst, s32 size) {
    /* uoch[] = { sj, notify_fn_a, notify_fn_b, notify_arg_b }; the running
     * destination pointer doubles as the success flag at the end. */
    u32 uoch[4];
    void* sj;
    void (*fn_a)(void*, s32);
    void (*fn_b)(void*, s32);
    void* fn_b_arg;
    /* two distinct ring-read result pairs (ptr,size); MWCC keeps them in
     * separate stack slots like retail */
    struct { void* p; s32 n; } t1;
    struct { void* p; s32 n; } t2;

    if (*(s32*)((u8*)self + 0x2038) == 8)
        return 1;

    SFBUF_GetUoch(self, *(s32*)((u8*)self + 0x2038), src, uoch);

    sj = (void*)uoch[0];
    fn_a = (void (*)(void*, s32))uoch[1];
    fn_b = (void (*)(void*, s32))uoch[2];
    fn_b_arg = (void*)uoch[3];

    if (sj == NULL)
        return 1;

    /* not enough free space in the SJ buffer */
    if ((*(s32 (**)(void*, s32))((u8*)*(void**)sj + 0x24))(sj, 0) < size) {
        dst = 0;
    } else {
        (*(void (**)(void*, s32, s32, void*))((u8*)*(void**)sj + 0x18))(sj, 0, size, &t1);
        /* NOTE: retail passes the read pointer as MEM_Copy's first argument */
        MEM_Copy(t1.p, (void*)dst, t1.n);
        (*(void (**)(void*, s32, void*))((u8*)*(void**)sj + 0x20))(sj, 1, &t1);

        if (t1.n == 0) {
            dst = 0;
        } else {
            size -= t1.n;
            dst += t1.n;

            /* wrapped read: fetch the remainder from the ring start */
            if (size > 0) {
                (*(void (**)(void*, s32, s32, void*))((u8*)*(void**)sj + 0x18))(sj, 0, size, &t2);
                MEM_Copy(t2.p, (void*)dst, t2.n);
                (*(void (**)(void*, s32, void*))((u8*)*(void**)sj + 0x20))(sj, 1, &t2);
                if (t2.n != size)
                    lbl_eu_80607AF0++; /* global underrun counter */
            }

            dst = 1;
        }
    }

    if (dst == 1) {
        if (fn_a != NULL)
            fn_a(self, src);
        if (fn_b != NULL)
            fn_b(fn_b_arg, src);
    }

    return dst;
}

int sfmps_CopyPadding(void) {
    return 1;
}

int sfmps_CopyDstBuft(void* self, s32 stream_kind, s32 buf, s32 size, s64 pts) {
    s32 ring_buf[10] __attribute__((aligned(8)));
    s32 res;
    void* first_ptr;
    s32 first_size;
    s32 total_size;
    void* write_ptr;
    void* second_ptr;

    res = SFBUF_RingGetWrite(self, stream_kind, ring_buf);
    if (res != 0)
        return res;

    first_ptr = (void*)ring_buf[0];
    first_size = ring_buf[1];
    write_ptr = (void*)ring_buf[2];
    second_ptr = (void*)ring_buf[5];
    total_size = ring_buf[3];

    if (size > first_size + total_size)
        return 0;

    if (stream_kind == 1) {
        s32 pts_data[4];
        if (lbl_eu_80619BAC != NULL) {
            s32 data[3] __attribute__((aligned(8)));
            data[1] = (s32)pts;
            data[0] = (s32)(pts >> 32);
            data[2] = size;
            if (lbl_eu_80619BAC((u8*)self + 0x1374, data) == -1)
                return 0;
        }

        if (pts >= 0) {
            s32 out;
            if (SFPTS_IsPtsQueFull(self, stream_kind))
                return 0;
            pts_data[1] = (s32)pts;
            pts_data[0] = (s32)(pts >> 32);
            pts_data[2] = (s32)first_ptr;
            pts_data[3] = size;
            res = SFPTS_WritePtsQue(self, stream_kind, pts_data, &out);
            if (res != 0)
                return res;
        }
    } else if (stream_kind == 2) {
        if (lbl_eu_80619BAC != NULL) {
            s32 data[3] __attribute__((aligned(8)));
            data[1] = (s32)pts;
            data[0] = (s32)(pts >> 32);
            data[2] = size;
            if (lbl_eu_80619BAC((u8*)self + 0x1368, data) == -1)
                return 0;
        }
    }

    if (size <= first_size) {
        MEM_Copy(first_ptr, (void*)buf, size);
    } else {
        MEM_Copy(first_ptr, (void*)buf, first_size);
        MEM_Copy(write_ptr, (void*)(buf + first_size), size - first_size);
    }

    res = SFBUF_RingAddWrite(self, stream_kind, size, second_ptr);
    if (res != 0)
        return res;

    return 1;
}
int sfmps_ChkSupply(void* self, s32 buf, s32 size, s32 a5) {
    void* mps_sub;
    s32 stream_idx;
    int delim;
    int endflg;

    mps_sub = *(void**)((u8*)self + 0x2024);
    stream_idx = *(s32*)((u8*)self + 0x202c);

    if (size >= 4) {
        /* CheckDelim returns bit flags (0x10000 video / 0x40000 audio /
           0x80000 endcode), hence the large-immediate comparisons */
        delim = MPS_CheckDelim((const u8*)buf);
        if (delim == 0x80000) {
            if (*(s32*)((u8*)self + 0x203c) < 0) {
                *(s32*)((u8*)self + 0x203c) = SFBUF_GetRTot(self, stream_idx) + 4;
            }
            *(s32*)((u8*)mps_sub + 0x3c) = 1;
        } else if (delim != 0) {
            *(s32*)((u8*)mps_sub + 0x3c) = 0;
        }
    } else {
        delim = 0;
    }

    /* exact retail shape: negated tests, innermost flag assigned as a
       negation (MWCC: beq->li 1 / fallthrough li 0) */
    if (delim == 0x80000) {
        if (!SFCON_IsEndcodeSkip(self)) {
            endflg = !SFCON_IsSystemEndcodeSkip(self);
        } else {
            endflg = 0;
        }
    } else {
        endflg = 0;
    }

    if (endflg) {
        SFTRN_SetTermFlg(self, 1, 1);
        if (*(s32*)((u8*)self + 0x2034) != 8)
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
        if (*(s32*)((u8*)self + 0x2030) != 8)
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
        if (*(s32*)((u8*)self + 0x2038) != 8)
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
        return 0;
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
        if (delim == 0x10000 || delim == 0x40000) {
            if (SFBUF_GetTermFlg(self, stream_idx) == 1) {
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
    s32 v0;
    s32* hdr = *(s32**)((u8*)self + 0x2670);
    s32* raw_hdr;
    u8* base;
    void* mps;
    void* mps_sub;
    int out1, out2;
    s32 ret1, ret2;
    s32 err;

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
    *(u64*)((u8*)self + 0xee8) = *(u64*)((u8*)raw_hdr + 0x18);
    *(u64*)((u8*)mps_sub + 0x10) = *(u64*)((u8*)raw_hdr + 0x20);

    return 0;
}

u32 SFMPS_GetConcatCnt(void* self) {
    void* ptr = *(void**)((u8*)self + 0x2024);
    return *(u32*)((u8*)ptr + 0x20);
}
