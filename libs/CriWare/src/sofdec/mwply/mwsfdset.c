// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdset
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

/* Movie player handle (MWSFDPLY). Partial layout covering the fields touched
 * by the helpers in this TU; unknown gaps are opaque byte runs. */
typedef struct MWSFDPLY {
    s32  field_0x00;        /* 0x00 signature */
    u8   field_0x04[0x54];  /* 0x04..0x57 */
    void* field_0x58;       /* 0x58 SFD decode handle */
    u8   field_0x5c[0x57c]; /* 0x5c..0x5d7 */
    u8   field_0x5d8[0x28]; /* 0x5d8 stream-set A (SstHn) */
    u8   field_0x600[0x20]; /* 0x600 stream-set B (SstHn) */
} MWSFDPLY;

void SFD_SetCond(void* a, u32 b, u32 c);
void MWSFD_SetAudioSw(void* self, u32 sw) {
    SFD_SetCond(*(void**)((u8*)self + 0x58), 6, sw);
}

void SFD_SetCond(void* a, u32 b, u32 c);
void MWSFD_SetVideoSw(void* self, u32 sw) {
    SFD_SetCond(*(void**)((u8*)self + 0x58), 5, sw);
}

extern void MWSFSVM_Error(const char*, ...);
extern char lbl_eu_8051B7B0[];
extern s32 MWSST_GetStat(void *);

void* mwPlyGetSfdHn(void* self) {
    s32 sig;
    if (self == NULL) {
        sig = 0;
    } else {
        sig = *(u32*)self;
    }
    if (sig != 1) {
        MWSFSVM_Error(lbl_eu_8051B7B0 + 0x5A);
        return NULL;
    }
    return *(void**)((u8*)self + 0x58);
}

void SFD_SetCond();
void MWSFD_SetCond(void* self, u32 sw) {
    void* obj = (self != NULL) ? *(void**)((u8*)self + 0x58) : NULL;
    SFD_SetCond(obj, sw);
}

u32 SFD_GetCond(void* a);
u32 MWSFD_GetCond(void* self) {
    return SFD_GetCond(self ? *(void**)((u8*)self + 0x58) : NULL);
}

u32 MWSFD_GetStmHn(void* self) { return *(u32*)((u8*)self + 0x5c); }

void MWSTM_SetFlowLimit(void* h, u32 limit);
void MWSFLSC_SetFlowLimit(void* self, u32 limit);
void MWSFD_SetFlowLimit(void* self, u32 limit) {
    void* h = *(void**)((u8*)self + 0x5c);
    MWSTM_SetFlowLimit(h, limit);
    MWSFLSC_SetFlowLimit(self, limit);
}

u32 MWSFD_IsEnableHndl(void* self) {
    if (self == NULL) return 0;
    return *(u32*)self;
}

s32 mwPlyGetRareStat(void* self);
int mwPlyGetStat(void *h) {
    int stat = ((int (*)(void *))mwPlyGetRareStat)(h);
    if ((s32)*(u32 *)((u8 *)h + 0x63C) == 1 &&
        (s32)*(u32 *)((u8 *)h + 0x640) == 1 &&
        *(u32 *)((u8 *)h + 0x64C) - 2 <= 1 &&
        (stat == 1 || stat == 3)) {
        return 2;
    }
    return stat;
}

extern int criware_803A2258(void* sst);
extern s32 SFD_GetHnStat(void* self);
extern void MWSFLIB_SetErrCode(s32 code);
extern char lbl_eu_8051B7B0[];
extern void MWSFSVM_Error(const char* fmt, ...);

s32 mwPlyGetRareStat(void* self) {
    s32 state = (self == NULL) ? 0 : *(s32*)self;
    if (state != 1) {
        MWSFLIB_SetErrCode(-12);
        MWSFSVM_Error(lbl_eu_8051B7B0 + 0x2B8);
        return 0;
    }
    {
        s32 r = SFD_GetHnStat(*(void**)((u8*)self + 0x58));
        if (r < 0) return 4;
        if (criware_803A2258(self) != 0) return 4;
        if (*(s32*)((u8*)self + 4) == 2) {
            if (r == 4 || r == 6) return 2;
            return 1;
        }
        return *(s32*)((u8*)self + 4);
    }
}

void MWSFSET_ExecSetCyclicFrameOutput(void *h) {
    void *sfd = *(void **)((u8 *)h + 0x58);
    if ((s32)*(u32 *)((u8 *)h + 0x66C) == 1) {
        s32 fps;
        SFD_GetFps(sfd, &fps);
        if (fps != -1) {
            s32 num, den;
            SFD_CalcCycleFromFps(fps, &num, &den);
            SFD_SetCyclicFrameOutput(sfd, num, den);
            *(u32 *)((u8 *)h + 0x66C) = 0;
        }
    }
}

extern s32 SFD_GetTime(void* self, s32* out1, s32* out2);
extern void MWSFLIB_SetErrCode(s32 code);

void mwPlyGetTime(MWSFDPLY* self, s32* out1, s32* out2) {
    s32 state = (self == NULL) ? 0 : self->field_0x00;
    *out1 = 0;
    *out2 = 1;
    if (state != 1) {
        MWSFSVM_Error(lbl_eu_8051B7B0 + 0x40D);
        return;
    }
    if (self->field_0x58 != NULL) {
        s32 ret = SFD_GetTime(self->field_0x58, out1, out2);
        if (ret != 0) {
            MWSFLIB_SetErrCode(-309);
            MWSFSVM_Error(lbl_eu_8051B7B0 + 0x437);
        }
        if (*out1 < 0) {
            *out1 = 0;
            *out2 = 1;
        }
    }
}

extern char lbl_eu_8051B7B0[];
extern void MWSFSVM_Error(const char* fmt, ...);
extern void MWSST_SetOutVol(void* sst, s32 vol);
extern void MWSFRNA_SetOutVol(void* rna, s32 vol);

void mwPlySetOutVol(void* self, s32 vol) {
    s32 state = (self == NULL) ? 0 : *(s32*)self;
    if (state != 1) {
        MWSFSVM_Error(lbl_eu_8051B7B0 + 0x45C);
    } else {
        MWSFRNA_SetOutVol(self, vol);
        MWSST_SetOutVol((u8*)self + 0x5D8, vol);
        MWSST_SetOutVol((u8*)self + 0x600, vol);
    }
}

extern s32 MWSFRNA_GetOutVol(void* rna);
extern s32 MWSST_GetOutVol(void* sst);

s32 mwPlyGetOutVol(MWSFDPLY* self) {
    s32 state = (self == NULL) ? 0 : self->field_0x00;
    if (state != 1) {
        MWSFSVM_Error(lbl_eu_8051B7B0 + 0x488);
        return 0;
    }
    if (self->field_0x58 == NULL) return 0;
    s32 ret;
    s32 a = MWSFRNA_GetOutVol(self);
    s32 b = MWSST_GetOutVol(&self->field_0x5d8);
    s32 c = MWSST_GetOutVol(&self->field_0x600);
    if (a == b || a == c) {
        ret = a;
    } else if (a != 0) {
        ret = a;
    } else if (b != 0) {
        ret = b;
    } else {
        ret = c;
    }
    return ret;
}

int criware_803A2258(void *h) {
    if ((s32)*(u32 *)((u8 *)h + 0x5D8) == 1 && MWSST_GetStat((u8 *)h + 0x5D8) == 4)
        return 1;
    if ((s32)*(u32 *)((u8 *)h + 0x600) == 1 && MWSST_GetStat((u8 *)h + 0x600) == 4)
        return 1;
    return 0;
}
