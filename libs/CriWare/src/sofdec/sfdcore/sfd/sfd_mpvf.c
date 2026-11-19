// Decompiled CriWare Sofdec SFMPVF (MPV video frame hand-off).
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

u32 SFSET_GetCond(void* self, u32 idx);
s32 SFTIM_IsGetFrmTime(void* self, void* frm);
s32 SFTIM_ExecCyclicFrameOutput(void* self);

extern SfdMpvParaGlobal lbl_eu_80619B20;
void* memset(void* dst, int val, size_t n);
void* memcpy(void* dst, const void* src, size_t n);

void SFMPVF_InitPool(void) {
    SfdMpvParaGlobal* tbl = &lbl_eu_80619B20;
    u8* pool = (u8*)tbl;

    /* clear the 9 parameter words, the two strides and the frame sizes */
    memset(pool + 0, 0, 0x24);
    memset(pool + 0x28, 0, 8);
    memset(pool + 0x30, 0, 0x40);
}

void SFD_SetMpvParaTbl(u32* src, u32* strides, u32* tbl) {
    SfdMpvParaGlobal* g = &lbl_eu_80619B20;
    u32* p = (u32*)g->para;
    u32* stride = (u32*)g->stride;
    u32* sizes = (u32*)g->frameSize;
    s32 i = 0;
    s32 j, k;

    /* the parameter words are copied as four doublewords */
    *(u64*)&p[0] = *(u64*)&src[0];
    *(u64*)&p[2] = *(u64*)&src[2];
    *(u64*)&p[4] = *(u64*)&src[4];
    *(u64*)&p[6] = *(u64*)&src[6];
    p[4] = 0;
    p[8] = 0;
    stride[0] = (strides[0] + 0x1F) & ~0x1F;
    stride[1] = (strides[1] + 0x1F) & ~0x1F;

    for (j = 0; j < 4; j++) {
        for (k = 0; k < 4; k++) {
            if (i < src[7]) {
                sizes[j * 4 + k] = (tbl[k] + 0x1F) & ~0x1F;
            } else {
                sizes[j * 4 + k] = 0;
            }
            i++;
        }
        tbl += 4;
    }
}

s32 sfmpvf_CheckMpvPara(void* self) {
    s32* para = lbl_eu_80619B20.para;
    s32* sizes;
    s32 cnt = para[7];
    s32 i;

    if ((u32)(cnt - 1) > 0xF) {
        return -1;
    }
    if (para[4] == 0 || para[8] == 0) {
        if (lbl_eu_80619B20.stride[0] == 0) {
            return -1;
        }
        if (lbl_eu_80619B20.stride[1] == 0) {
            return -1;
        }
        sizes = lbl_eu_80619B20.frameSize;
        for (i = 0; i < cnt; i++) {
            if (*sizes == 0) {
                return -1;
            }
            sizes++;
        }
    }
    return 0;
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFLIB_CheckHn(void* h);
extern void SFLIB_LockCs(void* cs);
extern void SFLIB_UnlockCs(void* cs);
extern SfdFrame* SFMPVF_HoldFrm(SfdHandle* self);

s32 SFMPVF_ReadGlobalMpvPara(SfdHandle* self) {
    SfdMpvParaGlobal* tbl = &lbl_eu_80619B20;
    SfdMpvParaWork* dst = &self->para;
    u64* dstWords = (u64*)dst->para;
    u64* srcWords = (u64*)tbl->para;

    if (sfmpvf_CheckMpvPara(self) != 0) {
        return SFLIB_SetErr(0, 0xff000f15);
    }
    /* the parameter words are copied as four doublewords */
    dstWords[0] = srcWords[0];
    dstWords[1] = srcWords[1];
    dstWords[2] = srcWords[2];
    dstWords[3] = srcWords[3];
    dst->para[8] = tbl->para[8];
    memcpy(dst->stride, tbl->stride, sizeof(dst->stride));
    memcpy(dst->frameSize, tbl->frameSize, sizeof(dst->frameSize));
    return 0;
}

void SFMPVF_WriteGlobalMpvPara(SfdHandle* self) {
    SfdMpvParaGlobal* tbl = &lbl_eu_80619B20;
    SfdMpvParaWork* src = &self->para;
    u64* dstWords = (u64*)tbl->para;
    u64* srcWords = (u64*)src->para;

    /* the parameter words are copied as four doublewords */
    dstWords[0] = srcWords[0];
    dstWords[1] = srcWords[1];
    dstWords[2] = srcWords[2];
    dstWords[3] = srcWords[3];
    tbl->para[8] = src->para[8];
    memcpy(tbl->stride, src->stride, sizeof(src->stride));
    memcpy(tbl->frameSize, src->frameSize, sizeof(src->frameSize));
    tbl->usrWidth = self->picUsr.width;
    tbl->usrHeight = self->picUsr.height;
    tbl->usrStride = self->picUsr.stride;
}

void SFD_CalcYccPlane(s32 base, s32 w, s32 h, void* out) {
    s32 wa = (w + 15) / 16;
    s32 h15 = h + 15;
    *(s32*)((u8*)out + 8) = base;
    s32 w16 = wa * 16;
    s32 w32 = (w16 + 31) / 32;
    s32 t = (((((wa >> 27) & 1) + w16) >> 1) + 31) / 32;
    *(s16*)((u8*)out + 14) = (s16)(w32 * 32);
    s32 ha = h15 / 16;
    *(s16*)((u8*)out + 12) = (s16)(t * 32);
    s32 h16 = ha * 16;
    s32 u = (((ha >> 27) & 1) + h16) >> 1;
    *(s32*)((u8*)out + 0) = base + h16 * (w32 * 32);
    *(s32*)((u8*)out + 4) = base + h16 * (w32 * 32) + u * (t * 32);
}

s32 sfmpvf_SetPicUsrBuf(SfdHandle* self, u32 w, u32 h, u32 stride) {
    SfdPicUsrBuf* usr = &self->picUsr;
    s32 i;

    if (w == 0 || h == 0 || stride == 0) {
        for (i = 0; i < 37; i++) {
            ((u32*)usr)[i] = 0;
        }
        return 0;
    }
    if ((s32)h < self->field_0x0030 + 3) {
        return SFLIB_SetErr(self, 0xff000f1d);
    }
    {
        u32 cur = w + stride;
        usr->width = w;
        usr->height = h;
        usr->stride = stride;
        usr->curWidth = w;
        usr->field_0x10 = 0;
        for (i = 0; i < ((s32)(h - 1) >= 16 ? 16 : (s32)(h - 1)); i++) {
            usr->band[i].top = cur;
            usr->band[i].bottom = 0;
            cur += stride;
        }
    }
    return 0;
}

void SFMPVF_TermDec(SfdHandle* self) { self->termFlag = 1; }

u32 SFMPVF_IsTermDec(SfdHandle* self) { return self->termFlag; }

void SFMPVF_SetGopStat(SfdHandle* self) {
    SfdFrame* frm = self->frames;
    s32 cnt = self->frameCount;

    while (cnt-- > 0) {
        if (frm->state == 2 || frm->state == 4) {
            frm->field_0xFC = 1;
        }
        frm++;
    }
}

s32 SFMPVF_GetNumFrm(SfdHandle* self) {
    void* cs;
    s32 n;
    SfdFrame* frm;
    s32 cnt;

    SFLIB_LockCs(&cs);
    n = self->frameCount;
    frm = self->frames;
    cnt = 0;
    while (n-- > 0) {
        if ((frm->state == 2 || frm->state == 4) && frm->dispId == -1) {
            cnt++;
        }
        frm++;
    }
    if (self->termFlag == 1 && cnt == 0) {
        cnt = -1;
    }
    SFLIB_UnlockCs(&cs);
    return cnt;
}

SfdFrame* SFMPVF_AllocFrm(SfdHandle* self) {
    void* cs;
    SfdFrame* frm;
    s32 i;
    s32 n;

    SFLIB_LockCs(&cs);
    n = self->frameCount;
    frm = self->frames;
    for (i = 0; i < n; i++, frm++) {
        if (frm->state == 0 && frm->field_0x04 == 0) {
            frm->state = 1;
            break;
        }
    }
    if (i == n) {
        frm = NULL;
    }
    SFLIB_UnlockCs(&cs);
    return frm;
}

void SFMPVF_FreeFrm(SfdFrame* frm) {
    if (frm != NULL) {
        frm->state = 0;
    }
}

void SFMPVF_StbyFrm(SfdFrame* frm) {
    if (frm != NULL) {
        frm->state = 2;
    }
}

void SFMPVF_RefStbyFrm(SfdFrame* frm) {
    if (frm != NULL) {
        frm->state = 4;
    }
}

void SFMPVF_EndRefFrm(SfdFrame* frm) {
    if (frm == NULL) return;
    if (frm->state == 4) {
        frm->state = 2;
        return;
    }
    frm->state = 0;
}

s32 sfmpvf_IsChkFirst(SfdFrame* a, SfdFrame* b);

SfdFrame* SFMPVF_HoldFrm(SfdHandle* self) {
    void* cs;
    s32 i;
    s32 cnt;
    SfdFrame* frm;
    SfdFrame* best;
    s32 count;

    SFLIB_LockCs(&cs);
    count = self->frameCount;
    frm = self->frames;
    best = NULL;
    cnt = 0;
    for (i = 0; i < count; i++, frm++) {
        s32 st = frm->state;
        if ((st == 2 || st == 4) && frm->dispId == -1) {
            cnt++;
            if (sfmpvf_IsChkFirst(best, frm)) {
                best = frm;
            }
        }
    }
    if (cnt == 1 && self->termFlag == 0 && best->field_0xFC == 0) {
        best = NULL;
    }
    SFLIB_UnlockCs(&cs);
    return best;
}

void sfmpvf_SearchStbyFrm(SfdHandle* h, SfdFrame** outA, SfdFrame** outB);
u32 SFSET_GetCond(void* self, u32 idx);
s32 SFTIM_IsGetFrmTimeTunit(void* self, s32 cmpA, s32 cmpB);

s32 SFD_IsNextFrmReady(SfdHandle* h) {
    void* cs;
    SfdFrame* frm;
    SfdFrame* dummy;

    if (SFLIB_CheckHn(h) != 0) {
        SFLIB_SetErr(0, 0xff000183);
        return 0;
    }
    SFLIB_LockCs(&cs);
    if (h->errorState != 4) {
        frm = NULL;
    } else {
        sfmpvf_SearchStbyFrm(h, &dummy, &frm);
        if (frm != NULL && SFSET_GetCond(h, 0xf) != 0 &&
            SFTIM_IsGetFrmTimeTunit(h, frm->time, frm->timeUnit) == 0) {
            frm = NULL;
        }
    }
    SFLIB_UnlockCs(&cs);
    return frm != NULL;
}

void sfmpvf_SearchStbyFrm(SfdHandle* h, SfdFrame** outA, SfdFrame** outB) {
    SfdFrame* frm;
    s32 count;
    s32 cnt;
    s32 i;

    count = h->frameCount;
    *outA = NULL;
    *outB = NULL;
    frm = h->frames;
    cnt = 0;
    for (i = 0; i < count; i++, frm++) {
        s32 st = frm->state;
        if ((st == 2 || st == 4) && frm->dispId == -1) {
            cnt++;
            if (sfmpvf_IsChkFirst(*outA, frm)) {
                *outB = *outA;
                *outA = frm;
            } else if (sfmpvf_IsChkFirst(*outB, frm)) {
                *outB = frm;
            }
        }
    }
    if (h->termFlag == 0) {
        cnt--;
    }
    if (cnt <= 0) {
        *outA = NULL;
        *outB = NULL;
    } else if (cnt == 1) {
        *outB = NULL;
    }
}

s32 sfmpvf_IsChkFirst(SfdFrame* a, SfdFrame* b) {
    if (a == NULL) {
        return 1;
    }
    if (b->order < a->order) {
        return 1;
    }
    if (b->order > a->order) {
        return 0;
    }
    if (b->field_0xF0 < a->field_0xF0) {
        return 1;
    }
    if (b->field_0xF0 > a->field_0xF0) {
        return 0;
    }
    if (b->field_0xF4 < a->field_0xF4) {
        return 1;
    }
    if (b->field_0xF4 > a->field_0xF4) {
        return 0;
    }
    {
        s32 x = a->field_0xF8 ^ b->field_0xF8;
        return ((x >> 1) - (x & a->field_0xF8)) < 0;
    }
}

typedef void (*SFMPVF_ReadCb)(SfdHandle*, SfdFrame*);

s32 SFMPVF_GetRead(SfdHandle* self, SfdFrmInfo** out, u32* outVal, SFMPVF_ReadCb cb) {
    SfdFrame* frm;
    SfdVfrm* vfrm;
    SfdFrame* p;
    s32 cnt;
    s32 i;
    s32 v;

    if (SFSET_GetCond(self, 0xf) == 0 && self->field_0x005c != 1) {
        *out = NULL;
        return 0;
    }

    frm = SFMPVF_HoldFrm(self);
    if (frm == NULL) {
        *out = NULL;
        return 0;
    }

    /* Find the held frame's record index and its video frame slot. */
    cnt = self->frameCount;
    p = self->frames;
    vfrm = NULL;
    for (i = 0; i < cnt; i++, p++) {
        if (p == frm) {
            vfrm = &self->vfrm[i];
            break;
        }
    }

    vfrm->state = 1;
    self->curFrame = frm;
    *out = &vfrm->info;
    cb(self, frm);

    self->tim.curTime = (*out)->time;
    self->tim.curTimeUnit = (*out)->timeUnit;

    if (!SFTIM_IsGetFrmTime(self, *out)) {
        *out = NULL;
        return 0;
    }
    if (!SFTIM_ExecCyclicFrameOutput(self)) {
        *out = NULL;
        return 0;
    }

    if (self->field_0x0068 == 2) {
        v = self->displayCount;
        self->displayCount = v + 1;
        if (v + 1 < 0) {
            self->displayCount = 0;
        }
        frm->dispId = v;
        *outVal = v;
    }
    return 0;
}

s32 SFMPVF_AddRead(SfdHandle* self, SfdFrmInfo* info, s32* rd) {
    void* cs;
    SfdFrame* frm;
    SfdVfrm* rel;
    SfdVfrm* vfrm;
    SfdFrame* p;
    s32 n;
    s32 i;
    s32 ret;

    SFLIB_LockCs(&cs);
    frm = NULL;
    rel = NULL;
    if (self->field_0x0068 == 2) {
        /* Playing: locate the frame whose display slot matches *rd. */
        p = self->frames;
        for (i = 0; i < 16; i++, p++) {
            if (p->dispId == *rd) {
                frm = p;
                break;
            }
        }
        if (frm == NULL) {
            ret = SFLIB_SetErr(self, 0xff000f1f);
        } else {
            n = self->frameCount;
            p = self->frames;
            for (i = 0; i < n; i++, p++) {
                if (p == frm) {
                    rel = &self->vfrm[i];
                    break;
                }
            }
            ret = 0;
        }
    } else {
        /* Not playing: the info block sits just past the slot state word. */
        rel = (SfdVfrm*)((u8*)info - 8);
        if (rel->state != 1) {
            ret = SFLIB_SetErr(self, 0xff000f0e);
        } else {
            vfrm = self->vfrm;
            for (i = 0; i < 16; i++, vfrm++) {
                if (&vfrm->info == info) {
                    frm = &self->frames[i];
                    break;
                }
            }
            if (self->curFrame != frm) {
                ret = SFLIB_SetErr(self, 0xff000f0f);
            } else {
                ret = 0;
            }
        }
    }
    if (ret == 0) {
        rel->state = 0;
        if (frm != NULL) {
            if (frm->state == 4) {
                frm->state = 3;
            } else {
                frm->state = 0;
            }
            frm->dispId = -1;
        }
        ret = 0;
    }
    SFLIB_UnlockCs(&cs);
    return ret;
}

s32 SFMPVF_ChkImageSize(SfdHandle* self, u32 w, u32 h) {
    if (self->imageWidth > 0 &&
        (self->imageWidth != (s32)w || self->imageHeight != (s32)h)) {
        return -1;
    }
    {
        s32 (*cb)(void*) = self->chkImageSize;
        void* arg = self->chkImageSizeArg;
        if (cb != NULL && cb(arg) != 0) {
            return -1;
        }
    }
    return 0;
}

// ppf: user picture buffer setup entry points.
s32 SFD_SetPicUsrBuf(SfdHandle* self, u32 w, u32 h, u32 stride) {
    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xff000185);
    }
    return sfmpvf_SetPicUsrBuf(self, w, h, stride);
}

s32 SFMPVF_ResetPicUsrBuf(SfdHandle* self) {
    SfdMpvParaGlobal* tbl = &lbl_eu_80619B20;

    return sfmpvf_SetPicUsrBuf(self, tbl->usrWidth, tbl->usrHeight, tbl->usrStride);
}

void SFMPVF_InitPicUsr(SfdPicUsrBuf* usr) {
    s32 i;

    for (i = 0; i < 37; i++) ((u32*)usr)[i] = 0;
}
