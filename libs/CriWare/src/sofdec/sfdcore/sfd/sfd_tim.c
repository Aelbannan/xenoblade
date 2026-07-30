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

void SFTIM_InitTtu(void* self, int val) {
    int zero = 0;
    int one = 1;
    *(int*)((u8*)self + 0x00) = zero;
    *(int*)((u8*)self + 0x04) = zero;
    *(int*)((u8*)self + 0x08) = zero;
    *(int*)((u8*)self + 0x0C) = zero;
    *(int*)((u8*)self + 0x10) = zero;
    *(int*)((u8*)self + 0x14) = zero;
    *(int*)((u8*)self + 0x18) = zero;
    *(int*)((u8*)self + 0x1C) = zero;
    *(u16*)((u8*)self + 0x20) = 0;
    *(u16*)((u8*)self + 0x22) = 0;
    *(int*)((u8*)self + 0x24) = val;
    *(int*)((u8*)self + 0x28) = one;
}

void SFTIM_UpdateItime() {}

int SFTIM_GetNextItime(void* self, int time) {
    int a = *(int*)((u8*)self + 0x298);
    int b = *(int*)((u8*)self + 0x29c);
    int c = *(int*)((u8*)self + 0x2a0);
    int sum1 = a + b;
    int sum2 = a + c;
    if (time < sum1) return sum1;
    if (time >= sum2) return 0x7FFFFFFF;
    return sum2;
}

void SFTIM_VbIn() {}

void SFTIM_IsStagnant() {}

void SFTIM_GetAudioStartSample() {}

void SFTIM_GetVideoStartSample() {}

void SFTIM_SetStartTime(void* self, u32 a, u32 b) { *(u32*)((u8*)self + 0x144) = a; *(u32*)((u8*)self + 0x148) = b; }

void SFD_GetTime() {}

void SFTIM_GetTimeSub() {}

u32 SFTIM_GetTime(void* self, u32* out1, u32* out2) {
    *out1 = *(u32*)((u8*)self + 0x1028);
    *out2 = *(u32*)((u8*)self + 0x102c);
    return 0;
}

int sftim_GetTimeNone(void *self, int *out1, int *out2) {
    int val = *(int*)((char*)self + 0x54);
    int tmp;
    if (val == 4) goto good;
    if (val == -4) goto good;
    if (val == 6) goto good;
    if (val == -6) goto good;
    tmp = -1;
    *out1 = tmp;
    tmp = 1;
    *out2 = tmp;
    tmp = 0;
    goto check;
good:
    tmp = 1;
check:
    if (!tmp) return 0;
    tmp = -2;
    *out1 = tmp;
    tmp = 1;
    *out2 = tmp;
    return 0;
}

void sftim_GetTimeVsync() {}

int sftim_GetTimeUfrm(void* self, int* out1, int* out2) {
    int val = *(int*)((u8*)self + 0x54);
    if (!(val == 4 || val == -4 || val == 6 || val == -6)) {
        *out1 = -1;
        *out2 = 1;
    }
    return 0;
}

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

extern u32 lbl_eu_8051CBF8[];

void SFTIM_GetTimeOneFrmVideo(void* self, int* out1, int* out2) {
    int fps = *(int*)((u8*)self + 0x930);
    if (fps == 0) {
        *out1 = fps;
        *out2 = 0x7512;
    } else {
        *out1 = 1000;
        *out2 = lbl_eu_8051CBF8[fps];
    }
}

void SFD_GetFps() {}

void SFTIM_IsGetFrmTime() {}

void SFTIM_IsGetFrmTimeTunit() {}

void SFTIM_IsExecTime() {}

void sftim_IsGrExecTime() {}

void SFTIM_IsVideoTerm() {}

void SFTIM_SetSpeed(void* self, u32 a, u32 b) { *(u32*)((u8*)self + 0x1048) = a; *(u32*)((u8*)self + 0x104C) = b; }

void SFTIM_GetSpeed(void* self, u32* out1, u32* out2) {
    *out1 = *(u32*)((u8*)self + 0x1048);
    *out2 = *(u32*)((u8*)self + 0x104c);
}

void SFD_SetCyclicFrameOutput() {}

void SFTIM_ExecCyclicFrameOutput() {}

void SFD_CalcCycleFromFps() {}

void SFD_SetLimitTime(void* self, u32 val) { *(u32*)((u8*)self + 0x1390) = val; }

void SFD_GetLimitTime() {}
