// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_tim
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef struct SFTIM {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
} SFTIM;

void SFTIM_Init(SFTIM *sftim, int val) {
    sftim->field_0 = 0;
    sftim->field_4 = 0;
    sftim->field_8 = val;
    if (val % 10 == 0) {
        sftim->field_C = val / 10;
        sftim->field_10 = 100;
    } else {
        sftim->field_C = val;
        sftim->field_10 = 1000;
    }
}

void SFTIM_InitHn() {}

void SFTIM_InitTtu() {}

void SFTIM_UpdateItime() {}

void SFTIM_GetNextItime() {}

void SFTIM_VbIn() {}

void SFTIM_IsStagnant() {}

void SFTIM_GetAudioStartSample() {}

void SFTIM_GetVideoStartSample() {}

void SFTIM_SetStartTime(void* self, u32 a, u32 b) {}

void SFD_GetTime() {}

void SFTIM_GetTimeSub() {}

u32 SFTIM_GetTime(void* self, u32* out1, u32* out2) {
    *out1 = *(u32*)((u8*)self + 0x1028);
    *out2 = *(u32*)((u8*)self + 0x102c);
    return 0;
}

void sftim_GetTimeNone() {}

void sftim_GetTimeVsync() {}

void sftim_GetTimeUfrm() {}

void sftim_GetTimeUtim() {}

void sftim_GetTimeExtClock() {}

int SFTIM_ChkRegularTime(const void *tim, int *a2, int *a3) {
    int val = *(const int *)((const char *)tim + 0x54);
    if (val == 4 || val == -4 || val == 6 || val == -6) {
        return 1;
    }
    *a2 = -1;
    *a3 = 1;
    return 0;
}

void SFD_SetUsrIsSkipFn() {}

void SFD_SetUsrTimeFn() {}

void SFD_SetExtClockFn() {}

void SFTIM_SetTimeFn(void* self, void* fn, u32 idx) {
    *(void**)((u8*)self + 0xd98 + idx * 4) = fn;
}

void SFTIM_Tc2Time() {}

void sftim_Tc2TimeN() {}

void sftim_Tc2Time23N() {}

void sftim_Tc2Time29N() {}

void sftim_Tc2Time59N() {}

void sftim_Tc2Time23D() {}

void sftim_Tc2Time29D() {}

void sftim_Tc2Time59D() {}

void SFTIM_Pause() {}

void SFTIM_GetTimeOneFrmVideo() {}

void SFD_GetFps() {}

void SFTIM_IsGetFrmTime() {}

void SFTIM_IsGetFrmTimeTunit() {}

void SFTIM_IsExecTime() {}

void sftim_IsGrExecTime() {}

void SFTIM_IsVideoTerm() {}

void SFTIM_SetSpeed(void* self, u32 a, u32 b) {}

void SFTIM_GetSpeed(void* self, u32* out1, u32* out2) {
    *out1 = *(u32*)((u8*)self + 0x1048);
    *out2 = *(u32*)((u8*)self + 0x104c);
}

void SFD_SetCyclicFrameOutput() {}

void SFTIM_ExecCyclicFrameOutput() {}

void SFD_CalcCycleFromFps() {}

void SFD_SetLimitTime(void* self, u32 val) { *(u32*)((u8*)self + 0x1390) = val; }

void SFD_GetLimitTime() {}
