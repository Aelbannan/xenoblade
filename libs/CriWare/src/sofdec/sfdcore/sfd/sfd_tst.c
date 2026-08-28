// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_tst
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void *memset(void *, int, unsigned long);
extern void *memcpy(void *, const void *, unsigned long);
u8 lbl_eu_80619BD0[0x18]; /* .bss 0x80619BD0 size 0x18 */
extern char* strcpy(char*, const char*);
extern unsigned long strlen(const char*);
const unsigned int lbl_eu_8051CD18[69] = {
    0x7473742C,0x2068656C,0x705F7469,0x6D655F73,0x65632C20,0x68656C70,0x5F74696D,
    0x655F6D73,0x65632C20,0x68656C70,0x5F74696D,0x655F3634,0x2C206865,0x6C705F74,
    0x696D652C,0x206D745F,0x6D61782C,0x206D6173,0x7465725F,0x74696D65,0x2C206F75,
    0x745F7469,0x6D652C20,0x206D745F,0x6F742C20,0x6D746D61,0x785F6F74,0x2C202064,
    0x6966665F,0x6C5F6D61,0x782C2064,0x6966665F,0x6C5F6D69,0x6E2C2064,0x6966665F,
    0x615F6D61,0x782C2074,0x73742D3E,0x64696666,0x5F615F6D,0x696E2C20,0x70617374,
    0x61742C20,0x61646A6D,0x6F64652C,0x20726573,0x65746869,0x73742C20,0x65786365,
    0x73736572,0x722C2061,0x646A5F6C,0x696D6974,0x2C206164,0x6A5F6672,0x6F6E742C,
    0x2061646A,0x5F726561,0x722C2020,0x6D6F7661,0x76655F31,0x73742C20,0x6D6F7661,
    0x76655F32,0x6E642C20,0x20616478,0x745F7374,0x6174200A,0x0A000000
};
const char lbl_eu_8051CE2C[148] = "%p, %ld, %ld, 0x%08lX%08lX, %ld, %ld, %ld, %ld,   %ld, %ld,   %ld, %ld, %ld, %ld,   %ld, %ld,   %ld, %ld, %ld, %ld, %ld,   %ld, %ld,   %ld \n";


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
        /* 64-bit fixed-point accumulate: accum += (control * param) / param_d */
        s64 val_a = *(s64*)((u8*)self + 0x130);
        s64 val_b = *(s64*)param;
        s64 val_d = *(s64*)(param + 2);

        *(s64*)((u8*)self + 0x128) += val_a * val_b / val_d;
    }
}

void SFTST_SetSpeed(void* self, u32 a, u32 b) {
    *(u32*)((u8*)self + 0x198) = a;
    *(u32*)((u8*)self + 0x19c) = b;
}

static void sftst_CalcSub(void* self, s32* param, s32* arg5, s32* out);

void SFTST_Calc(void* self, s32* param, s32* arg5, s32* out) {
    s32 speed_a = *(s32*)((u8*)self + 0x198);
    s32 speed_b = *(s32*)((u8*)self + 0x19c);

    if (speed_a == speed_b) {
        /* No speed change: pass through directly */
        sftst_CalcSub(self, param, arg5, out);
    } else {
        /* Rescale the position by the speed ratio before adjusting */
        s32 local_param[4];
        s32 result[4];

        *(s64*)&local_param[0] = *(s64*)&param[0];
        *(s64*)&local_param[2] = *(s64*)(param + 2) * speed_a / (s64)speed_b;

        sftst_CalcSub(self, local_param, arg5, result);

        *(s64*)&out[0] = *(s64*)&result[0];
        *(s64*)&out[2] = *(s64*)&param[2];
    }
}

static void sftst_CalcSub(void* self, s32* param, s32* arg5, s32* out) {
    if (((arg5[3] ^ 1) | arg5[2]) != 0 && *(s32*)self != 0) {
        s64 d1;
        s64 p01;

        if (*(s64*)((u8*)self + 0x178) == -1)
            *(s64*)((u8*)self + 0x178) = *(s64*)arg5;

        d1 = (s64)(((s64)arg5[1] << 32) | (u32)arg5[0]) -
             (s64)(((s64)*(s32*)((u8*)self + 0x17C) << 32) |
                   (u32)*(s32*)((u8*)self + 0x178));
        p01 = (s64)(((s64)param[1] << 32) | (u32)param[0]) -
              (s64)(((s64)*(s32*)((u8*)self + 0x194) << 32) |
                    (u32)*(s32*)((u8*)self + 0x190));

        if (p01 >= 0) {
            param[0] = *(s32*)((u8*)self + 0x190);
            param[1] = *(s32*)((u8*)self + 0x194);
        }
        (void)d1;
    }
    out[0] = param[0];
    out[1] = param[1];
    out[2] = param[2];
    out[3] = param[3];
}
