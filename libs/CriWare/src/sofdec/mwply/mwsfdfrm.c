// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdfrm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 MWSFD_GetUsePicUsr();

void mwPlyGetCurFrm() {}

/* ---- Frame-info conversion (mwl_convFrmInfFromSFD) ----
 * The player's frame table at +0xE0 holds 0x38-byte entries describing each
 * multiplexed frame; a frame index from the SFD stream header selects one. */

typedef struct MwlSfdFrame {
    s32 field_0x00;          /* 0x00 */
    u8 _04[0x0c];            /* 0x04..0x10 */
    s32 field_0x10;          /* 0x10 */
    s32 field_0x14;          /* 0x14 */
    u8 _18[0x20];            /* 0x18..0x38 */
} MwlSfdFrame;               /* 0x38 */

typedef struct MwlSfdSelf {
    u8 _00[0x58];            /* 0x00..0x58 */
    void* field_0x58;        /* 0x58: SFD stream handle */
    u8 _5c[0xd8 - 0x5c];     /* 0x5c..0xd8 */
    s32 field_0xd8;          /* 0xd8: frame index */
    u8 _dc[0x04];            /* 0xdc..0xe0 */
    MwlSfdFrame frames[8];   /* 0xe0, stride 0x38 */
    u8 _2a0[0x684 - 0x2a0];  /* 0x2a0..0x684 */
    s32 field_0x684;
    s32 field_0x688;
    s32 field_0x68c;
} MwlSfdSelf;

typedef struct MwlSfdInfSub {
    u8 _00[0x10];            /* 0x48..0x58 */
    s32 field_0x58;
    u8 _5c[0x10];            /* 0x5c..0x6c */
    s8 field_0x6c;
    u8 _6d[0x13];            /* 0x6d..0x80 */
} MwlSfdInfSub;              /* 0x38 */

typedef struct MwlSfdInfo {
    s32 field_0x00;
    s32 field_0x04;
    s32 field_0x08;
    s32 field_0x0c;
    s32 field_0x10;
    u8 _14[0x04];
    s32 field_0x18;
    s32 field_0x1c;
    s32 field_0x20;
    s32 field_0x24;
    s32 field_0x28;
    s32 field_0x2c;
    s32 field_0x30;
    s32 field_0x34;
    u8 _38[0x10];
    MwlSfdInfSub sub;        /* 0x48..0x80 */
} MwlSfdInfo;

typedef struct MwlSfdFrmInf {
    s32 field_0x00;
    s32 field_0x04;
    s32 field_0x08;
    s32 field_0x0c;
    s32 field_0x10;
    s32 field_0x14;
    s32 field_0x18;
    s32 field_0x1c;
    s32 field_0x20;
    s32 field_0x24;
    s32 field_0x28;
    s32 field_0x2c;
    s32 field_0x30;
    s32 field_0x34;
    s32 field_0x38;
    s32 field_0x3c;
    s32 field_0x40;
    s32 field_0x44;
    s32 field_0x48;
    s32 field_0x4c;
    s32 field_0x50;
    s32 field_0x54;
    u8 _58[0x10];
    u8 field_0x68[0x38];     /* 0x68..0xa0 */
    s32 field_0xa0;
    s32 field_0xa4;
} MwlSfdFrmInf;

extern const char lbl_eu_80519870[];
extern s32 SFD_GetFps(void* self, s32* out);
extern int UTY_MulDiv(int a, int b, int c);
extern u32 mwPlyFxGetCompoMode(void* self);
extern int MWSFD_GetFxType(void* self, int idx);
extern void MWSFSVM_Error(const char* fmt, ...);
extern s32 SUD_AnalyTypeDivField(const char* buf, s32 len);

void mwsffrm_SetPicUsrInf(void* a, void* b, void* out);
void mwsffrm_SetSudDatInf(void* a, void* b, void* out);

void mwl_convFrmInfFromSFD(MwlSfdSelf* self, MwlSfdInfo* info, MwlSfdFrmInf* out) {
    /* Local declaration order drives MWCC's callee-saved register colours:
     * first colorable local -> r14, then r18..r31 (params keep r15-r17).
     * Retail: f0c/r14, sfd/r18, picH/r19, picW/r20, frmIdx/r21, f28/r22,
     * f24/r23, f30/r24, frmCnt/r25, f34/r26, picH0/r27, picW0/r28,
     * frmKind/r29, picKind/r30, res1/r31.  The four stack locals keep
     * slots 0x08/0x0c/0x10/0x14 in declaration order. */
    s32 fps;      /* 0x08: SFD_GetFps output */
    s32 f08;      /* 0x0c: info->field_0x08 */
    s32 f20;      /* 0x10: info->field_0x20 */
    s32 res2;     /* 0x14 */
    s32 f0c;      /* r14: info->field_0x0c */
    s32 sfd;      /* r18: self->field_0x58 */
    s32 picKind;  /* r19: info->field_0x1c (colored in reverse decl order) */
    s32 frmKind;  /* r20: info->field_0x10 */
    s32 picW0;    /* r21: info->field_0x00 */
    s32 picH0;    /* r22: info->field_0x04 */
    s32 f34;      /* r23: info->field_0x34 */
    s32 frmCnt;   /* r24: info->field_0x18 */
    s32 f30;      /* r25: info->field_0x30 */
    s32 f24;      /* r26: info->field_0x24 */
    s32 f28;      /* r27: info->field_0x28 */
    s32 frmIdx;   /* r28: info->field_0x2c */
    s32 picW;     /* r29 */
    s32 picH;     /* r30 */
    s32 res1;     /* r31 */
    s32 useFrm;
    s32 fps2;
    s32 mul;
    s32 v58;
    s32 sud0;
    s32 sud1;
    s32 fx;

    f20 = info->field_0x20;
    sfd = (s32)self->field_0x58;
    switch (info->field_0x1c) {
    case 1: picKind = 1; break;
    case 2: picKind = 2; break;
    case 3: picKind = 3; break;
    default: picKind = 3; break;
    }
    picW0 = info->field_0x00;
    picH0 = info->field_0x04;
    f08 = info->field_0x08;
    f0c = info->field_0x0c;
    switch (info->field_0x10) {
    case 1: frmKind = 1; break;
    case 2: frmKind = 2; break;
    case 3: frmKind = 3; break;
    case 4: frmKind = 4; break;
    default:
        MWSFSVM_Error(lbl_eu_80519870 + 0x11b);
        frmKind = 1;
        break;
    }
    f34 = info->field_0x34;
    frmCnt = info->field_0x18;
    f30 = info->field_0x30;
    frmIdx = info->field_0x2c;
    f24 = info->field_0x24;
    f28 = info->field_0x28;

    if (SFD_GetFps((void*)sfd, &fps) != 0) {
        MWSFSVM_Error(lbl_eu_80519870 + 0x14a);
    }

    /* 29.97 fps is stored as 30*999 = 29970, i.e. a multiple of 999; the
     * drop-frame conversion then uses a 1001-frame multiplier.  T3 re-tests
     * fps2 (a distinct IR value) so MWCC does not CSE it with T1's test. */
    fps2 = fps;
    if (fps % 999 == 0 && frmCnt % 999 != 0) {
        if (fps2 % 999 == 0) {
            mul = 1001;
            fps2 = (fps / 999) * 1000;
        } else {
            mul = 1000;
        }
    } else {
        mul = 1000;
    }
    res1 = UTY_MulDiv(f34, fps2, frmCnt * mul);

    fps2 = fps;
    if (fps % 999 == 0 && frmCnt % 999 != 0) {
        if (fps2 % 999 == 0) {
            mul = 1001;
            fps2 = (fps / 999) * 1000;
        } else {
            mul = 1000;
        }
    } else {
        mul = 1000;
    }
    res2 = UTY_MulDiv(f30, fps2, frmCnt * mul);

    useFrm = 0;
    if (self->frames[frmIdx % 8].field_0x00 == 1) {
        picW = self->frames[frmIdx % 8].field_0x10;
        picH = self->frames[frmIdx % 8].field_0x14;
        if (picW != 0 && picH != 0) {
            useFrm = 1;
        } else {
            picW = picW0;
            picH = picH0;
        }
    } else {
        picW = picW0;
        picH = picH0;
    }

    /* Composition-mode dependent picture-size fixups. */
    if (mwPlyFxGetCompoMode(self) == 0x11) {
        fx = MWSFD_GetFxType(self, self->field_0xd8);
        if (fx == 0x51 || fx == 0x61) fx = 0x41;
        if (fx == 0x21 && useFrm != 0) {
            picH = picH * 2;
        }
    } else if (mwPlyFxGetCompoMode(self) == 0x21) {
        if (useFrm == 0) {
            picH = picH / 2;
        }
    } else if (mwPlyFxGetCompoMode(self) == 0) {
        fx = MWSFD_GetFxType(self, self->field_0xd8);
        if (fx == 0x51 || fx == 0x61) fx = 0x41;
        if (fx == 0x21 && useFrm == 0) {
            picH = picH / 2;
        }
    }

    out->field_0x00 = f20;
    out->field_0x08 = picKind;
    out->field_0x0c = picW0;
    out->field_0x10 = picH0;
    out->field_0x14 = picW;
    out->field_0x18 = picH;
    out->field_0x1c = f08;
    out->field_0x20 = f0c;
    out->field_0x24 = frmKind;
    out->field_0x28 = fps;
    out->field_0x2c = res1;
    out->field_0x30 = f34;
    out->field_0x34 = frmCnt;
    out->field_0x38 = frmIdx;
    out->field_0x3c = res2;
    out->field_0x40 = f30;
    out->field_0x44 = f24;
    out->field_0x48 = f28;
    self->field_0x684 = f34;
    self->field_0x688 = frmCnt;
    self->field_0x68c = res1;
    mwsffrm_SetPicUsrInf(self, info, out);
    mwsffrm_SetSudDatInf(self, info, out);

    v58 = info->sub.field_0x58;
    useFrm = 0;
    sud1 = out->field_0xa4;
    sud0 = out->field_0xa0;
    if ((u32)(v58 - 1) <= 1) {
        useFrm = 2;
    } else if (v58 == 3) {
        if (info->sub.field_0x6c == 0) {
            useFrm = 2;
        }
    } else {
        MWSFSVM_Error(lbl_eu_80519870 + 0x168);
    }
    if (MWSFD_GetUsePicUsr() == 1 && SUD_AnalyTypeDivField((const char*)sud0, sud1) == 1) {
        useFrm = 2;
    }
    out->field_0x54 = useFrm;
    memcpy(out->field_0x68, &info->sub, 0x38);
}

void mwsffrm_SetPicUsrInf(void* a, void* b, void* out) {
    void* inf = *(void**)((u8*)b + 0x38);
    s32 v = *(s32*)((u8*)inf + 0);
    s32 w = *(s32*)((u8*)inf + 4);
    u32 picadr = (u32)((u8*)a + 0x4A4);
    u32 pic = *(u32*)((u8*)a + 0x4bc);
    if (MWSFD_GetUsePicUsr(a) != 1) {
        *(s32*)((u8*)out + 0x4c) = 0;
        *(s32*)((u8*)out + 0x50) = 0;
        return;
    }
    if (pic == picadr) {
        *(s32*)((u8*)out + 0x4c) = 0;
        *(s32*)((u8*)out + 0x50) = 0;
        return;
    }
    if (v != 0 && w > 4) {
        v += 4;
        w -= 4;
    }
    *(s32*)((u8*)out + 0x4c) = v;
    *(s32*)((u8*)out + 0x50) = w;
}

extern void* SUD_SearchSudDat(void* a, void* b, void* c, void* d);

void mwsffrm_SetSudDatInf(void* a, void* b, void* out) {
    void* r1 = 0;
    void* r2 = 0;
    void* sud = *(void**)((u8*)b + 0x38);
    void* p = *(void**)((u8*)sud + 0);
    s32 q = *(s32*)((u8*)sud + 4);
    *(s32*)((u8*)out + 0xa0) = 0;
    *(s32*)((u8*)out + 0xa4) = 0;
    if (p != NULL && q > 4) {
        SUD_SearchSudDat((u8*)p + 4, (u8*)q - 4, &r1, &r2);
    }
    if (r1 != NULL && (s32)r2 > 0) {
        *(s32*)((u8*)out + 0xa0) = (s32)r1;
        *(s32*)((u8*)out + 0xa4) = (s32)r2;
    }
}

typedef struct {
    u32 a, b, c;
    s32 d, e, f;
} MwSfdYccPlane;

typedef struct {
    u32 a, b, c;
    s16 d, e;
} MwSfdYccPlaneTmp;

extern void SFD_CalcYccPlane(void *, void *, void *, void *);

void mwPlyCalcYccPlane(void *a, void *b, void *c, MwSfdYccPlane *out) {
    MwSfdYccPlaneTmp tmp;
    SFD_CalcYccPlane(a, b, c, &tmp);
    out->a = tmp.c;
    out->b = tmp.a;
    out->c = tmp.b;
    out->d = tmp.e;
    out->e = tmp.d;
    out->f = tmp.d;
}

void mwPlyRelCurFrm() {}

int mwPlyGetTotalFrmNum(void* self) {
    int index = *(s32*)((u8*)self + 0xd8) % 8;
    u32* entry = (u32*)((u8*)self + index * 0x38);
    if ((s32)entry[0xE0 / 4] != 1) return 0;
    return entry[0x100 / 4];
}

void MWSFFRM_InitSfhInfTable() {}

void SFD_SetSfdHeaderFn(void* a, void* cb, void* ctx);
void mwsffrm_CallbackAnalyzeSofdecHeader();
void MWSFFRM_SetShfCbFn(void* self) {
    SFD_SetSfdHeaderFn(*(void**)((u8*)self + 0x58), mwsffrm_CallbackAnalyzeSofdecHeader, self);
}

void mwsffrm_CallbackAnalyzeSofdecHeader() {}

extern s32 SFH_AnlyElemCodecAud(const void* sfd, s32 idx, s32* out);
extern s32 SFH_AnlyElemChNum(const void* sfd, s32 idx, s32* out);
extern s32 SFH_AnlyElemSmpHz(const void* sfd, s32 idx, s32* out);
extern s32 SFH_IsExistStmId(const void* sfd, s32 idx, s32* out);
extern s32 SFH_AnlyElemPicSz(const void* sfd, s32 idx, s32* sz, s32* rate);
extern s32 SFH_AnlyFtrNetWidth(const void* sfd, s32 idx, s32* out);
extern s32 SFH_AnlyFtrNetHeight(const void* sfd, s32 idx, s32* out);

void mwsffrm_AnalyAudioInfo(void* self, void* sj) {
    s32 i;
    s32 out;
    if (*(s32*)((u8*)self + 0x2A0) != 1) return;
    for (i = 0; i < 32; i++) {
        s32* ent = (s32*)((u8*)self + i * 16 + 0x2A4);
        s32 idx = (i + 0xC0) & 0xFF;
        ent[0] = 0;
        ent[1] = 0;
        ent[2] = 0;
        ent[3] = 0;
        if (SFH_AnlyElemCodecAud(sj, idx, &out) == 1 && out == 1) {
            ent[0] = 1;
            if (SFH_AnlyElemChNum(sj, idx, &out) == 1) {
                s32 ch = out;
                if ((u32)(ch - 2) > 3) {
                    ent[1] = 2;
                } else if (ch == 1) {
                    ent[1] = 1;
                } else if (ch == 6) {
                    ent[1] = 3;
                } else if (ch == 7) {
                    ent[1] = 4;
                } else if (ch == 8) {
                    ent[1] = 5;
                } else {
                    ent[1] = 0;
                }
            }
            if (SFH_AnlyElemSmpHz(sj, idx, &out) == 1) {
                ent[2] = out;
            }
            if (SFH_IsExistStmId(sj, idx, &out) == 1) {
                ent[3] = out;
            }
        }
    }
}

extern void* mwPlyGetSfdHn(void* self);
extern void MWSFD_SetSstCh(void* self, s32 idx, s32 ch);
extern void criware_803A326C(void* self, s32 idx);
extern s32 SFD_GetCond(void* self, u32 idx, s32* out);
extern void MWSFD_SetAudioSw(void* self, u32 sw);
extern void MWSFD_SetCond(void* self, u32 sw, u32 unk);

void mwsffrm_ChangeSettingSyncPlayback(void* self) {
    void* hn;
    s32 mode1;
    s32 mode2;
    s32 ch1;
    s32 flag;
    s32 ch2;
    s32 found;
    s32 i;
    hn = mwPlyGetSfdHn(self);
    mode1 = 0;
    ch1 = -1;
    if (*(s32*)((u8*)self + 0x5D8) == 1) {
        if (*(s32*)((u8*)self + 0x5F0) == 1) {
            ch1 = *(s32*)((u8*)self + 0x5F4);
            if (*(s32*)((u8*)self + 0x2A4 + ch1 * 16) == 1) {
                if (*(s32*)((u8*)self + 0x2A8 + ch1 * 16) == 5) {
                    MWSFD_SetSstCh(self, 0, ch1);
                    mode1 = 1;
                }
            }
            if (mode1 != 1) {
                ch1 = -1;
                criware_803A326C(self, 0);
            }
        }
    }
    mode2 = 0;
    ch2 = -1;
    if (*(s32*)((u8*)self + 0x600) == 1) {
        if (*(s32*)((u8*)self + 0x618) == 1) {
            ch2 = *(s32*)((u8*)self + 0x61C);
            if (*(s32*)((u8*)self + 0x2A4 + ch2 * 16) == 1) {
                MWSFD_SetSstCh(self, 1, ch2);
                mode2 = 1;
            }
            if (mode2 != 1) {
                ch2 = -1;
                criware_803A326C(self, 1);
            }
        }
    }
    flag = 0;
    SFD_GetCond(hn, 30, &found);
    if (found == -1) {
        for (i = 0; i < 32; i++) {
            if (*(s32*)((u8*)self + 0x2A4 + i * 16) == 1) {
                if (i != ch1 && i != ch2) {
                    found = i;
                    break;
                }
            }
        }
    }
    if (found != -1 && found != ch1 && found != ch2) {
        s32 st = *(s32*)((u8*)self + 0x2A8 + found * 16);
        if (st == 1) {
            flag = 1;
        }
        if (st == 2) {
            if (*(s32*)((u8*)hn + 0x3990) == 1) {
                flag = 1;
            }
        }
        if (st == 0) {
            if (*(s32*)((u8*)hn + 0x3994) == 1) {
                flag = 1;
            }
        }
    }
    if (flag == 0) {
        MWSFD_SetAudioSw(self, 0);
    }
    if (mode1 == 1 || mode2 == 1) {
        MWSFD_SetCond(self, 67, 0);
    }
}

#pragma push
#pragma opt_propagation off
void criware_8039CD7C(void* a, s32* out1, s32* out2, s32* out3, s32* out4) {
    s32 stm;
    s32 sz;
    s32 rate;
    *out1 = 0;
    *out2 = 0;
    *out3 = 0;
    *out4 = 0;
    if (SFH_IsExistStmId(a, 0xE0, &stm) == 1 && stm == 1) goto body;
    goto exit;
exit:
    return;
body:
    if (SFH_AnlyElemPicSz(a, 0xE0, &sz, &rate) != 1) goto end;
    *out1 = sz;
    *out2 = rate;
    if (SFH_AnlyFtrNetWidth(a, 0xE0, &sz) != 0) {
        *out3 = sz;
    }
    if (SFH_AnlyFtrNetHeight(a, 0xE0, &rate) != 0) {
        *out4 = rate;
    }
end:
    return;
}
#pragma pop
