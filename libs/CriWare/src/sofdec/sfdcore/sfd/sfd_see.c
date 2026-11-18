// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_see
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

s32 SFLIB_CheckHn(SfdHandle* h);
s32 SFLIB_SetErr(void* h, u32 err_code);
u32 SFTRN_IsSetup(void* self, u32 idx);
u32 SFSET_GetCond(void* self, u32 idx);
s32 SFCON_IsEndcodeSkip(void* self);
s32 SFHDS_GetMuxVerNum(void* self);
s32 UTY_MulDiv(s32 a, s32 b, s32 c);

void sfsee_UpdateEByteRate(SfdHandle* self);

void SFSEE_InitHn(SfdSee* see) {
    see->avplay = NULL;
    see->field_0x004 = 0;
    see->seekTime = -3;
    see->seekTimeUnit = 1;
}

s32 SFD_EntrySeek(SfdHandle* self, void* seekData) {
    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(NULL, 0xFF000151);
    }
    self->see.avplay = seekData;
    return 0;
}

void SFSEE_FixAvPlay(SfdHandle* self, int arg2, int arg3) {
    SfdAvp* avplay = self->see.avplay;

    if (avplay == NULL) return;
    if (avplay->fixTime < 0) {
        avplay->fixTime = arg2;
    }
    if (avplay->fixSize < 0) {
        avplay->fixSize = arg3;
    }
}

void sfsee_ExecHeadAnaly(SfdHandle* self);
void sfsee_ExecFinAnaly(SfdHandle* self);

void SFSEE_ExecServer(SfdHandle* self) {
    if (self->see.avplay != NULL) {
        sfsee_ExecHeadAnaly(self);
        sfsee_ExecFinAnaly(self);
    }
}

void sfsee_ExecHeadAnaly(SfdHandle* self) {
    s32 setup3_flag;
    s32 setup2_flag;
    s32 ok1;
    s32 ok2;
    s32 st;
    SfdAvp* avplay = self->see.avplay;
    s32 r3, r4;

    if (avplay->state != 0) return;

    if (SFTRN_IsSetup(self, 3) != 0 && (s32)SFSET_GetCond(self, 6) == 1) {
        setup3_flag = 1;
        if (avplay->vidSeek == 0)
            ok1 = 1;
        else
            ok1 = 0;
    } else {
        setup3_flag = 0;
        ok1 = 0;
    }
    if (ok1 != 0) return;

    if (SFTRN_IsSetup(self, 2) != 0 && (s32)SFSET_GetCond(self, 5) == 1) {
        setup2_flag = 1;
        if (avplay->audSeek == 0)
            ok2 = 1;
        else
            ok2 = 0;
    } else {
        setup2_flag = 0;
        ok2 = 0;
    }
    if (ok2 != 0) return;

    st = SFTRN_IsSetup(self, 1);
    if ((u32)(-st | st) >> 31) {
        avplay->mpsHd.state = 1;
        {
            if (avplay->hdrImage != 0 && avplay->hdrByteRate > 0) {
                s32 fileSize = avplay->fileSize;
                s32 totalSize = avplay->hdrTotalSize;
                if (fileSize > 0 && totalSize > 0) {
                    r3 = UTY_MulDiv(fileSize, 1000, totalSize);
                } else {
                    r3 = avplay->hdrByteRate;
                }
            } else if (avplay->hdrImage != 0) {
                s32 muxVer = SFHDS_GetMuxVerNum(self);
                if (muxVer < 0x6C) {
                    /* fixed-point conversion via reciprocal multiply */
                    u32 m = avplay->mpsHd.picRate << 11;
                    s32 hi = __mulhw((s32)0x81E722C3, (s32)m);
                    s32 x = (hi + (s32)m) >> 10;
                    r3 = x + ((u32)x >> 31);
                } else {
                    r3 = avplay->mpsHd.picRate;
                }
            } else {
                r3 = avplay->mpsHd.picRate;
            }
        }
        r4 = avplay->mpsHd.timeUnit;
    } else if (setup2_flag) {
        r3 = avplay->audByteRate;
        r4 = avplay->audTimeUnit;
    } else if (setup3_flag) {
        r3 = avplay->vidByteRate;
        r4 = avplay->vidTimeUnit;
    } else {
        return;
    }

    avplay->byteRate = r3;
    avplay->timeUnit = r4;
    avplay->state = 1;
    sfsee_UpdateEByteRate(self);
}

s32 SFD_SetFileSize(SfdHandle* self, s32 fileSize) {
    SfdAvp* avplay;

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(NULL, 0xFF000159);
    }
    avplay = self->see.avplay;
    if (avplay == NULL) {
        return 0;
    }
    avplay->fileSize = fileSize;
    sfsee_UpdateEByteRate(self);
    return 0;
}

s32 SFD_SetTotTime(SfdHandle* self, s32 timeHi, s32 timeLo) {
    SfdAvp* avplay;

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(NULL, 0xFF00015A);
    }
    avplay = self->see.avplay;
    if (avplay == NULL) {
        return 0;
    }
    avplay->totalSize = timeHi;
    avplay->totalTime = timeLo;
    sfsee_UpdateEByteRate(self);
    return 0;
}

s32 SFD_SetByteRate(SfdHandle* self, s32 byteRate) {
    SfdAvp* avplay;

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(NULL, 0xFF00015B);
    }
    avplay = self->see.avplay;
    if (avplay == NULL) {
        return 0;
    }
    avplay->setByteRate = byteRate;
    sfsee_UpdateEByteRate(self);
    return 0;
}

s32 SFD_SetSeekPos(SfdHandle* self, s32 seekPos) {
    SfdAvp* avplay;

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(NULL, 0xFF00015C);
    }
    avplay = self->see.avplay;
    if (avplay == NULL) {
        return 0;
    }
    avplay->seekPos = seekPos;
    return 0;
}

// Fin analysis: once decoding is finishing up, accumulate the final playback
// position/time into the avplay handle from the seek target or stream header.
void sfsee_ExecFinAnaly(SfdHandle* self) {
    SfdAvp* p = self->see.avplay;
    s32 flag = 0;

    if (SFCON_IsEndcodeSkip(self) != 0)
        return;

    if (p->finTime <= 0) {
        s32 v6;

        if (self->see.seekTime == -3)
            v6 = 0;
        else
            v6 = p->seekPos;
        if (v6 >= 0) {
            // Look up the current channel's remaining duration; only apply it
            // when the header carried a valid (non-negative) value.
            s32 r0 = -1;
            SfdBuf* slot = &self->buf[self->trn[0].termIdx];
            SfdTrn* trn = &self->trn[slot->outTrn];
            s32 v = trn->field_0x20;
            if (v >= 0)
                r0 = v;
            if (r0 != -1) {
                p->finTime = v6 + r0;
                flag = 1;
            }
        }
    }
    if (p->finSize <= 0 && self->tim.tcB.ttime > 0) {
        p->finSize = self->tim.tcB.ttime;
        flag = 1;
        p->finTimeUnit = self->tim.tcB.ftime;
    }

    if (flag)
        sfsee_UpdateEByteRate(self);
}

void sfsee_UpdateEByteRate(SfdHandle* self) {
    SfdAvp* avplay = self->see.avplay;
    s32 byteRate = avplay->setByteRate;

    if (byteRate > 0) {
        avplay->eByteRate = byteRate;
        return;
    }

    {
        s32 fileSize = avplay->fileSize;
        s32 totalSize = avplay->totalSize;
        s32 totalTime = avplay->totalTime;

        if (fileSize > 0 && totalSize > 0) {
            avplay->eByteRate = UTY_MulDiv(fileSize, totalTime, totalSize);
            return;
        }

        {
            s32 headerRate = avplay->byteRate;
            if (headerRate > 0) {
                avplay->eByteRate = headerRate;
                return;
            }

            if (fileSize <= 0) {
                fileSize = avplay->finTime;
            }
            if (totalSize <= 0) {
                totalSize = avplay->finSize;
                totalTime = avplay->finTimeUnit;
            }

            if (fileSize > 0 && totalSize > 0) {
                avplay->eByteRate = UTY_MulDiv(fileSize, totalTime, totalSize);
            } else {
                avplay->eByteRate = headerRate;
            }
        }
    }
}
