// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_tst
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

extern void *memset(void *, int, unsigned long);
extern void *memcpy(void *, const void *, unsigned long);
extern char* strcpy(char*, const char*);
extern unsigned long strlen(const char*);

u32 sftst_debout_siz;
char* sftst_debout_buf;
char* sftst_debout_write;
char* sftst_debout_round;
char* sftst_last;

void SFTST_Create(SfdTst* tst) {
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
    {
        char local[274] = "tst, help_time_sec, help_time_msec, help_time_64, help_time, mt_max, master_time, out_time,  mt_ot, mtmax_ot,  diff_l_max, diff_l_min, diff_a_max, tst->diff_a_min, pastat, adjmode, resethist, excesserr, adj_limit, adj_front, adj_rear,  movave_1st, movave_2nd,  adxt_stat \n\n";
        u32 siz = sftst_debout_siz;

        if (sftst_debout_buf != NULL) {
            memset(sftst_debout_buf, 0, siz);
            sftst_debout_write = sftst_debout_buf;
            strcpy(sftst_debout_write, local);
            sftst_debout_write += strlen(local);
            sftst_debout_round = sftst_debout_write;
        }
    }
}

const char lbl_eu_8051CE2C[148] = "%p, %ld, %ld, 0x%08lX%08lX, %ld, %ld, %ld, %ld,   %ld, %ld,   %ld, %ld, %ld, %ld,   %ld, %ld,   %ld, %ld, %ld, %ld, %ld,   %ld, %ld,   %ld \n";

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
