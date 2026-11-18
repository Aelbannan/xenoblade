// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_tst
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

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


void SFTST_Create(SfdTst* tst) {
    char local[274];

    memset(tst, 0, 0x1D0);
    tst->tstFlg = 1;
    tst->pause = 0;
    tst->field_0x008 = 0;
    tst->adjFlg = 1;
    tst->movaveRange = 10;
    memset(tst->table, 0, 0xF0);
    tst->field_0x014 = 0;
    tst->field_0x10c = 0; tst->field_0x108 = 0;
    tst->field_0x114 = 1; tst->field_0x110 = 0;
    tst->field_0x11c = 0; tst->field_0x118 = 0;
    tst->field_0x124 = 1; tst->field_0x120 = 0;
    tst->position[1] = 0; tst->position[0] = 0;
    tst->step[1] = 1; tst->step[0] = 0;
    tst->tolerance[1] = 16683; tst->tolerance[0] = 0;
    tst->tolerance[3] = 1000000; tst->tolerance[2] = 0;
    tst->excessErr[1] = 200000; tst->excessErr[0] = 0;
    tst->excessErr[3] = 1000000; tst->excessErr[2] = 0;
    tst->adjStart[1] = 0; tst->adjStart[0] = 0;
    tst->adjStart[3] = 1000000; tst->adjStart[2] = 0;
    tst->adjPoff[1] = 0; tst->adjPoff[0] = 0;
    tst->adjPoff[3] = 1000000; tst->adjPoff[2] = 0;
    tst->prevTime[1] = -1; tst->prevTime[0] = -1;
    tst->field_0x180[1] = -1; tst->field_0x180[0] = -1;
    tst->field_0x188[1] = 0; tst->field_0x188[0] = 0;
    tst->baseTime[1] = 0; tst->baseTime[0] = 0;
    tst->speedNum = 1; tst->speedDen = 1;
    memset(tst->field_0x1a0, 0, 0x2C);
    memcpy(local, lbl_eu_8051CD18, 274);
    if (*(void**)(lbl_eu_80619BD0 + 4) != NULL) {
        memset(*(void**)(lbl_eu_80619BD0 + 4), 0, *(u32*)lbl_eu_80619BD0);
        *(char**)(lbl_eu_80619BD0 + 8) = strcpy((char*)(*(u32*)(lbl_eu_80619BD0 + 4)), local + 4);
        *(u32*)(lbl_eu_80619BD0 + 8) += strlen(local + 4);
        *(u32*)(lbl_eu_80619BD0 + 0xC) = *(u32*)(lbl_eu_80619BD0 + 8);
    }
}

void SFTST_SetTstFlg(SfdTst* tst, u32 val) { tst->tstFlg = val; }

void SFTST_SetTolerance(SfdTst* tst, s32* param) {
    s32 v0 = param[0];
    s32 v1 = param[1];
    tst->tolerance[1] = v1;
    tst->tolerance[0] = v0;
    s32 v2 = param[2];
    s32 v3 = param[3];
    tst->tolerance[3] = v3;
    tst->tolerance[2] = v2;
}

void SFTST_SetExcessErr(SfdTst* tst, s32* param) {
    s32 v0 = param[0];
    s32 v1 = param[1];
    tst->excessErr[1] = v1;
    tst->excessErr[0] = v0;
    s32 v2 = param[2];
    s32 v3 = param[3];
    tst->excessErr[3] = v3;
    tst->excessErr[2] = v2;
}

void SFTST_SetAdjStart(SfdTst* tst, s32* param) {
    s32 v0 = param[0];
    s32 v1 = param[1];
    tst->adjStart[1] = v1;
    tst->adjStart[0] = v0;
    s32 v2 = param[2];
    s32 v3 = param[3];
    tst->adjStart[3] = v3;
    tst->adjStart[2] = v2;
}

void SFTST_SetAdjPoff(SfdTst* tst, s32* param) {
    s32 v0 = param[0];
    s32 v1 = param[1];
    tst->adjPoff[1] = v1;
    tst->adjPoff[0] = v0;
    s32 v2 = param[2];
    s32 v3 = param[3];
    tst->adjPoff[3] = v3;
    tst->adjPoff[2] = v2;
}

void SFTST_SetMovaveRange(SfdTst* tst, s32 val) {
    if (val > 0) {
        tst->movaveRange = val;
    }
}

void SFTST_Pause(SfdTst* tst, u32 val) { tst->pause = val; }

void SFTST_SetAdjFlg(SfdTst* tst, u32 val) { tst->adjFlg = val; }

void SFTST_GoNextFrame(SfdTst* tst, s32* param) {
    if (tst->adjFlg == 0) {
        /* 64-bit fixed-point accumulate: accum += (control * param) / param_d */
        s64 val_a = *(s64*)&tst->step[0];
        s64 val_b = *(s64*)param;
        s64 val_d = *(s64*)(param + 2);

        *(s64*)&tst->position[0] += val_a * val_b / val_d;
    }
}

void SFTST_SetSpeed(SfdTst* tst, u32 a, u32 b) {
    tst->speedNum = a;
    tst->speedDen = b;
}

static void sftst_CalcSub(SfdTst* tst, s32* param, s32* arg5, s32* out);

void SFTST_Calc(SfdTst* tst, s32* param, s32* arg5, s32* out) {
    s32 speed_a = tst->speedNum;
    s32 speed_b = tst->speedDen;

    if (speed_a == speed_b) {
        /* No speed change: pass through directly */
        sftst_CalcSub(tst, param, arg5, out);
    } else {
        /* Rescale the position by the speed ratio before adjusting */
        s32 local_param[4];
        s32 result[4];

        *(s64*)&local_param[0] = *(s64*)&param[0];
        *(s64*)&local_param[2] = *(s64*)(param + 2) * speed_a / (s64)speed_b;

        sftst_CalcSub(tst, local_param, arg5, result);

        *(s64*)&out[0] = *(s64*)&result[0];
        *(s64*)&out[2] = *(s64*)&param[2];
    }
}

static void sftst_CalcSub(SfdTst* tst, s32* param, s32* arg5, s32* out) {
    if (((arg5[3] ^ 1) | arg5[2]) != 0 && tst->tstFlg != 0) {
        s64 d1;
        s64 p01;

        if (*(s64*)&tst->prevTime[0] == -1)
            *(s64*)&tst->prevTime[0] = *(s64*)arg5;

        d1 = (s64)(((s64)arg5[1] << 32) | (u32)arg5[0]) -
             (s64)(((s64)tst->prevTime[1] << 32) |
                   (u32)tst->prevTime[0]);
        p01 = (s64)(((s64)param[1] << 32) | (u32)param[0]) -
              (s64)(((s64)tst->baseTime[1] << 32) |
                    (u32)tst->baseTime[0]);

        if (p01 >= 0) {
            param[0] = tst->baseTime[0];
            param[1] = tst->baseTime[1];
        }
        (void)d1;
    }
    out[0] = param[0];
    out[1] = param[1];
    out[2] = param[2];
    out[3] = param[3];
}
