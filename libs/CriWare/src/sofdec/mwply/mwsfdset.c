// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdset
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

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

void mwPlyGetRareStat();
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

void mwPlyGetRareStat() {}

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

void mwPlyGetTime(void* self, s32* out1, s32* out2) {
    s32 state = (self == NULL) ? 0 : *(s32*)self;
    *out1 = 0;
    *out2 = 1;
    if (state != 1) {
        MWSFSVM_Error(lbl_eu_8051B7B0 + 0x40D);
        return;
    }
    if (*(s32*)((u8*)self + 0x58) != 0) {
        s32 ret = SFD_GetTime(self, out1, out2);
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

s32 mwPlyGetOutVol(void* self) {
    s32 state = (self == NULL) ? 0 : *(s32*)self;
    if (state != 1) {
        MWSFSVM_Error(lbl_eu_8051B7B0 + 0x488);
        return 0;
    }
    if (*(s32*)((u8*)self + 0x58) == 0) return 0;
    {
        s32 a = MWSFRNA_GetOutVol(self);
        s32 b = MWSST_GetOutVol((u8*)self + 0x5D8);
        s32 c = MWSST_GetOutVol((u8*)self + 0x600);
        s32 ret;
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
}

int criware_803A2258(void *h) {
    if ((s32)*(u32 *)((u8 *)h + 0x5D8) == 1 && MWSST_GetStat((u8 *)h + 0x5D8) == 4)
        return 1;
    if ((s32)*(u32 *)((u8 *)h + 0x600) == 1 && MWSST_GetStat((u8 *)h + 0x600) == 4)
        return 1;
    return 0;
}
