// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_tst
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u8 lbl_eu_80619BD0[];
extern char* strcpy(char*, const char*);
extern unsigned long strlen(const char*);
extern u8 lbl_eu_8051CD18[];

void SFTST_Create(void* self) {
    s32* w = (s32*)self;
    char local[274];

    memset(self, 0, 0x1D0);
    w[0] = 1;
    w[1] = 0;
    w[2] = 0;
    w[3] = 1;
    w[4] = 10;
    memset((u8*)self + 0x18, 0, 0xF0);
    w[5] = 0;
    w[67] = 0; w[66] = 0;
    w[69] = 1; w[68] = 0;
    w[71] = 0; w[70] = 0;
    w[73] = 1; w[72] = 0;
    w[75] = 0; w[74] = 0;
    w[77] = 1; w[76] = 0;
    w[79] = 16683; w[78] = 0;
    w[81] = 1000000; w[80] = 0;
    w[83] = 200000; w[82] = 0;
    w[85] = 1000000; w[84] = 0;
    w[87] = 0; w[86] = 0;
    w[89] = 1000000; w[88] = 0;
    w[91] = 0; w[90] = 0;
    w[93] = 1000000; w[92] = 0;
    w[95] = -1; w[94] = -1;
    w[97] = -1; w[96] = -1;
    w[99] = 0; w[98] = 0;
    w[101] = 0; w[100] = 0;
    w[102] = 1; w[103] = 1;
    memset(w + 104, 0, 0x2C);
    memcpy(local, lbl_eu_8051CD18, 274);
    if (*(void**)(lbl_eu_80619BD0 + 4) != NULL) {
        memset(*(void**)(lbl_eu_80619BD0 + 4), 0, *(u32*)lbl_eu_80619BD0);
        *(char**)(lbl_eu_80619BD0 + 8) = strcpy((char*)(*(u32*)(lbl_eu_80619BD0 + 4)), local + 4);
        *(u32*)(lbl_eu_80619BD0 + 8) += strlen(local + 4);
        *(u32*)(lbl_eu_80619BD0 + 0xC) = *(u32*)(lbl_eu_80619BD0 + 8);
    }
}

void SFTST_SetTstFlg(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }

void SFTST_SetTolerance(void* self, s32* param) {
    s32 v0 = param[0];
    s32 v1 = param[1];
    *(u32*)((u8*)self + 0x13c) = v1;
    *(u32*)((u8*)self + 0x138) = v0;
    s32 v2 = param[2];
    s32 v3 = param[3];
    *(u32*)((u8*)self + 0x144) = v3;
    *(u32*)((u8*)self + 0x140) = v2;
}

void SFTST_SetExcessErr(void* self, s32* param) {
    s32 v0 = param[0];
    s32 v1 = param[1];
    *(u32*)((u8*)self + 0x14c) = v1;
    *(u32*)((u8*)self + 0x148) = v0;
    s32 v2 = param[2];
    s32 v3 = param[3];
    *(u32*)((u8*)self + 0x154) = v3;
    *(u32*)((u8*)self + 0x150) = v2;
}

void SFTST_SetAdjStart(void* self, s32* param) {
    s32 v0 = param[0];
    s32 v1 = param[1];
    *(u32*)((u8*)self + 0x15c) = v1;
    *(u32*)((u8*)self + 0x158) = v0;
    s32 v2 = param[2];
    s32 v3 = param[3];
    *(u32*)((u8*)self + 0x164) = v3;
    *(u32*)((u8*)self + 0x160) = v2;
}

void SFTST_SetAdjPoff(void* self, s32* param) {
    s32 v0 = param[0];
    s32 v1 = param[1];
    *(u32*)((u8*)self + 0x16c) = v1;
    *(u32*)((u8*)self + 0x168) = v0;
    s32 v2 = param[2];
    s32 v3 = param[3];
    *(u32*)((u8*)self + 0x174) = v3;
    *(u32*)((u8*)self + 0x170) = v2;
}

void SFTST_SetMovaveRange(void* self, s32 val) {
    if (val > 0) {
        *(u32*)((u8*)self + 0x10) = val;
    }
}

void SFTST_Pause(void* self, u32 val) { *(u32*)((u8*)self + 0x4) = val; }

void SFTST_SetAdjFlg(void* self, u32 val) { *(u32*)((u8*)self + 0xc) = val; }

void SFTST_GoNextFrame(void* self, s32* param) {
    if (*(u32*)((u8*)self + 0xc) == 0) {
        s32 a_hi = *(s32*)((u8*)self + 0x134);
        s32 a_lo = *(s32*)((u8*)self + 0x130);
        s32 b_hi = param[1];
        s32 b_lo = param[0];
        s32 d_hi = param[3];
        s32 d_lo = param[2];

        s64 val_a = ((s64)a_hi << 32) | (u32)a_lo;
        s64 val_b = ((s64)b_hi << 32) | (u32)b_lo;
        s64 val_d = ((s64)d_hi << 32) | (u32)d_lo;
        s64 result = (val_a * val_b) / val_d;

        *(u32*)((u8*)self + 0x12c) += (s32)(u32)result;
        *(u32*)((u8*)self + 0x128) += (s32)(u32)(result >> 32);
    }
}

void SFTST_SetSpeed(void* self, u32 a, u32 b) {
    *(u32*)((u8*)self + 0x198) = a;
    *(u32*)((u8*)self + 0x19c) = b;
}

static void sftst_CalcSub(void* self, s32* param, void* arg5, s32* out);

void SFTST_Calc(void* self, s32* param, void* arg5, s32* out) {
    s32 speed_a = *(s32*)((u8*)self + 0x198);
    s32 speed_b = *(s32*)((u8*)self + 0x19c);

    if (speed_a == speed_b) {
        sftst_CalcSub(self, param, arg5, out);
    } else {
        s32 local_param[4];
        s32 result[4];

        local_param[0] = param[0];
        local_param[1] = param[1];

        s64 val = ((s64)param[3] << 32) | (u32)param[2];
        s64 div = ((s64)(speed_b >> 31) << 32) | (u32)speed_b;
        s64 quot = (val * (s64)(s32)speed_a) / div;

        local_param[2] = (s32)(u32)(quot >> 32);
        local_param[3] = (s32)(u32)quot;

        sftst_CalcSub(self, local_param, arg5, result);

        out[0] = result[0];
        out[1] = result[1];
        out[2] = param[2];
        out[3] = param[3];
    }
}

void sftst_CalcSub(void* self, s32* param, void* arg5, s32* out) {}
