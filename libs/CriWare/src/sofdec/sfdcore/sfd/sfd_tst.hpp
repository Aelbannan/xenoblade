#pragma once

#include <harness_catalog.h>

typedef struct {
    /* 0x00 */ s32 flag;
    /* 0x04 */ s32 pause;
    /* 0x08 */ s32 mode;
    /* 0x0C */ s32 adj_flg;
    /* 0x10 */ s32 movave_range;
    /* 0x14 */ s32 sample_index;
    /* 0x18 */ s32 buf[60];   /* 0xF0 bytes */
    /* 0x108 */ s32 last_input_hi;
    /* 0x10C */ s32 last_input_lo;
    /* 0x110 */ s32 param_hi;
    /* 0x114 */ s32 param_lo;
    /* 0x118 */ s32 delta_hi;
    /* 0x11C */ s32 delta_lo;
    /* 0x120 */ s32 output_delta_hi;
    /* 0x124 */ s32 output_delta_lo;
    /* 0x128 */ s32 accum_lo;
    /* 0x12C */ s32 accum_hi;
    /* 0x130 */ s32 control_lo;
    /* 0x134 */ s32 control_hi;
    /* 0x138 */ s32 tolerance_lo;
    /* 0x13C */ s32 tolerance_hi;
    /* 0x140 */ s32 excess_err_lo;
    /* 0x144 */ s32 excess_err_hi;
    /* 0x148 */ s32 excess_err2_lo;
    /* 0x14C */ s32 excess_err2_hi;
    /* 0x150 */ s32 excess_err3_lo;
    /* 0x154 */ s32 excess_err3_hi;
    /* 0x158 */ s32 adj_start_lo;
    /* 0x15C */ s32 adj_start_hi;
    /* 0x160 */ s32 adj_start2_lo;
    /* 0x164 */ s32 adj_start2_hi;
    /* 0x168 */ s32 adj_poff_lo;
    /* 0x16C */ s32 adj_poff_hi;
    /* 0x170 */ s32 adj_poff2_lo;
    /* 0x174 */ s32 adj_poff2_hi;
    /* 0x178 */ s32 target_lo;
    /* 0x17C */ s32 target_hi;
    /* 0x180 */ s32 current_lo;
    /* 0x184 */ s32 current_hi;
    /* 0x188 */ s32 position_lo;
    /* 0x18C */ s32 position_hi;
    /* 0x190 */ s32 last_lo;
    /* 0x194 */ s32 last_hi;
    /* 0x198 */ s32 speed_a;
    /* 0x19C */ s32 speed_b;
    /* 0x1A0 */ s32 field_0x1a0;
    /* 0x1A4 */ s32 field_0x1a4;
    /* 0x1A8 */ s32 field_0x1a8;
    /* 0x1AC */ s32 field_0x1ac;
    /* 0x1B0 */ s32 field_0x1b0;
    /* 0x1B4 */ s32 moving_avg;
    /* 0x1B8 */ s32 moving_avg2;
    /* 0x1BC */ s32 min_pos_err;
    /* 0x1C0 */ s32 max_pos_err;
    /* 0x1C4 */ s32 min_neg_err;
    /* 0x1C8 */ s32 max_neg_err;
} SFTST;

void SFTST_Create(SFTST* self);
void SFTST_SetTstFlg(SFTST* self, u32 val);
void SFTST_SetTolerance(SFTST* self, s32* param);
void SFTST_SetExcessErr(SFTST* self, s32* param);
void SFTST_SetAdjStart(SFTST* self, s32* param);
void SFTST_SetAdjPoff(SFTST* self, s32* param);
void SFTST_SetMovaveRange(SFTST* self, s32 val);
void SFTST_Pause(SFTST* self, u32 val);
void SFTST_SetAdjFlg(SFTST* self, u32 val);
void SFTST_GoNextFrame(SFTST* self, s32* param);
void SFTST_SetSpeed(SFTST* self, u32 a, u32 b);
void SFTST_Calc(SFTST* self, s32* param, s32* arg5, s32* out);
