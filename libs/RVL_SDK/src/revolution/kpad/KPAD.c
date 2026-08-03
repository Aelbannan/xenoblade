#include <revolution/OS.h>
#include <revolution/KPAD.h>

#include <math.h>
#include <string.h>

/* Internal pad state (stride 0x578, 4 pads in inside_kpads). */
typedef struct KPADObject {
    Vec2 center;  // at 0x0
    s8 error_fg;   // at 0x8
    s8 state_fg;  // at 0x9
    u8 pad[2]; // at 0xA
} KPADObject;

typedef struct KPADInternal {
    /* 0x00 */ KPADStatus status;
    /* 0xB0 */ f32 pos_play_radius;
    /* 0xB4 */ f32 pos_sensitivity;
    /* 0xB8 */ f32 hor_play_radius;
    /* 0xBC */ f32 hor_sensitivity;
    /* 0xC0 */ f32 dist_play_radius;
    /* 0xC4 */ f32 dist_sensitivity;
    /* 0xC8 */ f32 acc_play_radius;
    /* 0xCC */ f32 acc_sensitivity;
    /* 0xD0 */ f32 dist_init;
    /* 0xD4 */ Vec2 unk_d4;
    /* 0xDC */ Vec2 sec_nrm_hori;
    /* 0xE4 */ Vec2 unk_e4;
    /* 0xEC */ f32 unk_ec;
    /* 0xF0 */ KPADObject kobj_sample[4];
    /* 0x120 */ KPADObject kobj_regular[2];
    /* 0x138 */ s16 valid_objs;
    /* 0x13A */ u8 state_a;
    /* 0x13B */ u8 state_b;
    /* 0x13C */ KPADUnifiedWpadStatus kp_ex[16];
    /* 0x4BC */ KPADUnifiedWpadStatus* kp_ex_ptr;
    /* 0x4C0 */ u32 kp_ex_count;
    /* 0x4C4 */ f32 sec_length;
    /* 0x4C8 */ Vec2 sec_nrm;
    /* 0x4D0 */ f32 sec_dist;
    /* 0x4D4 */ f32 trust_sec_length;
    /* 0x4D8 */ Vec hard_acc;
    /* 0x4E4 */ Vec2 obj_horizon;
    /* 0x4EC */ Vec2 acc_horizon;
    /* 0x4F4 */ Vec2 ah_circle_pos;
    /* 0x4FC */ u16 ah_circle_ct_pad;
    /* 0x4FE */ u8 unk_4fe;
    /* 0x4FF */ u8 unk_4ff;
    /* 0x500 */ u16 btn_repeat_time;
    /* 0x502 */ u16 btn_repeat_next;
    /* 0x504 */ u16 btn_repeat_delay;
    /* 0x506 */ u16 btn_repeat_pulse;
    /* 0x508 */ u16 btn_cl_repeat_time;
    /* 0x50A */ u16 btn_cl_repeat_next;
    /* 0x50C */ WPADCallback dpd_callback;
    /* 0x510 */ f32 acc_scale_x;
    /* 0x514 */ f32 acc_scale_y;
    /* 0x518 */ f32 acc_scale_z;
    /* 0x51C */ f32 fs_acc_scale_x;
    /* 0x520 */ f32 fs_acc_scale_y;
    /* 0x524 */ f32 fs_acc_scale_z;
    /* 0x528 */ f32 kobj_frame_min_x;
    /* 0x52C */ f32 kobj_frame_min_y;
    /* 0x530 */ f32 kobj_frame_max_x;
    /* 0x534 */ f32 kobj_frame_max_y;
    /* 0x538 */ f32 dist_speed_1;
    /* 0x53C */ f32 dist_speedM_1;
    /* 0x540 */ f32 ah_circle_radius2;
    /* 0x544 */ f32 unk_544;
    /* 0x548 */ f32 unk_548;
    /* 0x54C */ WPADSamplingCallback sampling_cb;
    /* 0x550 */ KPADConnectCallback connect_cb;
    /* 0x554 */ u8 enable;
    /* 0x555 */ u8 unk_555;
    /* 0x556 */ u8 dpd_enabled;
    /* 0x557 */ u8 dpd_status;
    /* 0x558 */ u8 unk_558;
    /* 0x559 */ u8 unk_559;
    /* 0x55A */ u8 unk_55a;
    /* 0x55B */ u8 dpd_cb_state;
    /* 0x55C */ u8 aiming_x;
    /* 0x55D */ u8 aiming_y;
    /* 0x55E */ u8 unk_55e;
    /* 0x55F */ u8 unk_55f;
    /* 0x560 */ u32 pos_mode;
    /* 0x564 */ u32 hor_mode;
    /* 0x568 */ u32 dist_mode;
    /* 0x56C */ u32 acc_mode;
    /* 0x570 */ u32 unk_570;
    /* 0x574 */ u8 unk_574;
} KPADInternal;

static const f32 float_8066C0B0 = 0.0f;
static const f32 float_8066C0B8 = -1.0f;
static const f32 float_8066C0BC = 1.0f;
static const f32 float_8066C0C0 = -0.75f;
static const f32 float_8066C0C4 = 0.75f;
static const f32 float_8066C100 = 0.38386398553848267f;

f32 kp_obj_interval = 0.2f;
f32 kp_ah_circle_radius = 0.07f;
u16 kp_ah_circle_ct = 100;
f32 kp_err_outside_frame = 0.05f;
f32 kp_err_dist_speed = 0.04f;
f32 kp_err_near_pos = 0.1f;
f32 kp_dist_vv1;
f32 kp_err_dist_min;
u8 kp_wbc_zero_point_done;
u8 kp_wbc_enabled;
u8 kp_wbc_issued;
u8 kp_wbc_tgc_weight_issued;
u8 kp_wbc_setup;
u16 kp_wbc_ave_sample_count;

Vec2 Vec2_0;
Vec2 icenter_org;

KPADInternal inside_kpads[4];
typedef union {
    f32 m[3][4];
    f64 align_hint;
} KpFsRot;
KpFsRot kp_fs_rot;

static const f32 float_8066C0B4 = 0.5f;
static const f32 float_8066C0C8 = 2.0f;
static const f64 double_8066C0D0 = 4503601774854144.0;
static const f64 double_8066C0E8 = 0.0;
static const f64 double_8066C0F8 = 4503599627370496.0;
static const f32 float_8066C104 = 0.017453292f;

static const f32 float_8066C0D8 = 0.001953125f;
static const f32 float_8066C0DC = 0.99902344f;
static const f32 float_8066C0E0 = 0.74902344f;
static const f64 double_8066C0F0 = -0.5;
static const f32 float_8066C110 = 0.01f;
static const f32 float_8066C114 = 0.005f;
static const f32 float_8066C118 = 0.2f;
static const f32 float_8066C11C = -0.2f;

static const char kpad_version_str[] =
    "<< RVL_SDK - KPAD \trelease build: Jun 22 2009 18:32:13 (0x4302_145) >>";
const char* __KPADVersion = kpad_version_str;

f32 idist_org = 1.0f;
Vec2 iaccXY_nrm_hori = {0.0f, -1.0f};
Vec2 isec_nrm_hori = {1.0f, 0.0f};
f32 kp_acc_horizon_pw = 0.05f;
f32 kp_ah_circle_pw = 0.06f;
f32 kp_err_dist_max = 3.0f;
f32 kp_err_first_inpr = 0.9f;
f32 kp_err_next_inpr = 0.9f;
f32 kp_err_acc_inpr = 0.9f;
f32 kp_err_up_inpr = 0.7f;
s32 kp_fs_fstick_min = 15;
s32 kp_fs_fstick_max = 71;
s32 kp_cl_stick_min = 60;
s32 kp_cl_stick_max = 308;
s32 kp_cl_trigger_min = 30;
s32 kp_cl_trigger_max = 180;
f32 kp_rm_acc_max = 3.4f;
f32 kp_fs_acc_max = 2.1f;
s32 kp_ex_trigger_max = 0x100;
s32 kp_ex_analog_max = 0x400;
u8 kp_wbc_wait_count = 0x32;
f32 kp_wbc_ave_count = 400.0f;
f32 kp_fs_revise_deg = 24.0f;
u8 kp_initialized;

s32 kp_stick_clamp_cross;
s32 kp_ex_trigger_min;
s32 kp_ex_analog_min;
f64 kp_wbc_tgc_weight;

f64 kp_wbc_ave_sample[4];
f64 kp_wbc_weight_ave[4];

extern void WPADSetCallbackByKPAD(s32 callback);
extern s32 WPADControlBLC(s32 chan, u8 command, WPADCallback callback);
extern s32 WBCSetupCalibration(void);
extern s32 WBCGetCalibrationStatus(void);
extern s32 WBCSetZEROPointDummy(void* samples, s32 count);
extern s32 WBCGetBatteryLevel(s32 battery);
extern s32 WBCReadDummy(KPADUnifiedWpadStatus* status, f64* pWeight, s32 count);
extern s32 WBCGetTGCWeightDummy(f64* pTgcWeight, KPADUnifiedWpadStatus* status);
void KPADiSamplingCallback(s32 chan);
void KPADiConnectCallback(s32 chan, s32 result);
s32 KPADiRead(s32 chan, KPADStatus* status, s32 count, s32* result, s32 param5);

void reset_kpad(KPADInternal* kp) {
    KPADObject* op;
    KPADStatus* sp = &kp->status;
    f32 vv1;

    kp->enable = 0;
    kp->kobj_frame_min_x = -1.0f + kp_err_outside_frame;
    kp->kobj_frame_max_x = 1.0f - kp_err_outside_frame;
    kp->kobj_frame_min_y = -0.75f + kp_err_outside_frame;
    kp->kobj_frame_max_y = 0.75f - kp_err_outside_frame;
    kp->dist_speed_1 = 1.0f / kp_err_dist_speed;
    kp->dist_speedM_1 = -1.0f / kp_err_dist_speed;
    kp->ah_circle_radius2 = kp_ah_circle_radius * kp_ah_circle_radius;
    kp->unk_548 = kp_err_dist_min;
    vv1 = kp_dist_vv1;
    kp->unk_544 = vv1;

    sp->hold = sp->trig = sp->release = 0;
    kp->btn_repeat_time = 0;
    kp->btn_repeat_next = kp->btn_repeat_delay;
    kp->status.dpd_valid_fg = 0;
    kp->unk_4fe = 0;

    sp->pos = sp->vec = Vec2_0;
    sp->speed = 0.0f;

    sp->horizon.x = kp->acc_horizon.x = kp->obj_horizon.x = 1.0f;
    sp->horizon.y = kp->acc_horizon.y = kp->obj_horizon.y = 0.0f;
    sp->hori_vec = Vec2_0;
    sp->hori_speed = 0.0f;

    sp->acc_vertical.x = 1.0f;
    sp->acc_vertical.y = 0.0f;

    sp->dist = kp->dist_init;
    sp->dist_vec = sp->dist_speed = 0.0f;

    kp->sec_dist = sp->dist;
    kp->sec_length = kp->trust_sec_length = vv1 / kp->dist_init;
    kp->sec_nrm = kp->sec_nrm_hori;

    sp->acc.x = sp->acc.z = 0.0f;
    sp->acc.y = -1.0f;
    sp->acc_value = 1.0f;
    sp->acc_speed = 0.0f;
    kp->hard_acc = sp->acc;

    kp->ah_circle_pos = kp->acc_horizon;
    kp->ah_circle_ct_pad = kp_ah_circle_ct;

    kp->valid_objs = 0;

    op = &kp->kobj_sample[3];
    do {
        op->error_fg = -1;
    } while (--op >= kp->kobj_sample);

    op = &kp->kobj_regular[1];
    do {
        op->error_fg = -1;
    } while (--op >= kp->kobj_regular);

    kp->state_b = 0;
    kp->unk_555 = 1;
}

void calc_button_repeat(KPADInternal* pad, u32 mode, u16 delta) {
    if (pad->status.trig != 0 || pad->status.release != 0) {
        pad->btn_repeat_time = 0;
        pad->btn_repeat_next = pad->btn_repeat_delay;
        if (pad->status.trig != 0) {
            if (pad->btn_repeat_pulse != 0) {
                pad->status.hold |= 0x80000000;
            }
        }
    } else if (pad->status.hold != 0) {
        pad->btn_repeat_time += delta;
        if (pad->btn_repeat_time >= 0x9C40) {
            pad->btn_repeat_time -= 0x9C40;
        }
        if (pad->btn_repeat_time >= pad->btn_repeat_next) {
            pad->status.hold |= 0x80000000;
            pad->btn_repeat_next += pad->btn_repeat_pulse;
            if (pad->btn_repeat_time >= 0x4E20) {
                pad->btn_repeat_time -= 0x4E20;
                pad->btn_repeat_next -= 0x4E20;
            }
        }
    }

    if (mode == 2 || (mode - 0x10) <= 1) {
        if (pad->status.ex_status.cl.trig != 0 || pad->status.ex_status.cl.release != 0) {
            pad->btn_cl_repeat_time = 0;
            pad->btn_cl_repeat_next = pad->btn_repeat_delay;
            if (pad->status.ex_status.cl.trig != 0) {
                if (pad->btn_repeat_pulse != 0) {
                    pad->status.ex_status.cl.hold |= 0x80000000;
                }
            }
        } else if (pad->status.ex_status.cl.hold != 0) {
            pad->btn_cl_repeat_time += delta;
            if (pad->btn_cl_repeat_time >= 0x9C40) {
                pad->btn_cl_repeat_time -= 0x9C40;
            }
            if (pad->btn_cl_repeat_time >= pad->btn_cl_repeat_next) {
                pad->status.ex_status.cl.hold |= 0x80000000;
                pad->btn_cl_repeat_next += pad->btn_repeat_pulse;
                if (pad->btn_cl_repeat_time >= 0x4E20) {
                    pad->btn_cl_repeat_time -= 0x4E20;
                    pad->btn_cl_repeat_next -= 0x4E20;
                }
            }
        }
    }
}

void calc_acc(KPADInternal* kp, f32* acc, f32 acc2) {
    f32 f1, f2, f3;

    f3 = *acc;
    f2 = acc2 - f3;

    if (kp->acc_mode == 0) {
        if (f2 < 0.0f) {
            f1 = -f2;
        } else {
            f1 = f2;
        }

        if (f1 >= kp->acc_play_radius) {
            f1 = 1.0f;
        } else {
            f1 /= kp->acc_play_radius;
            f1 *= f1;
            f1 *= f1;
        }
        f1 *= kp->acc_sensitivity;
        *acc += f1 * f2;
    } else {
        if (f2 < -kp->acc_play_radius) {
            *acc = f3 + kp->acc_sensitivity * (f2 + kp->acc_play_radius);
        } else if (f2 > kp->acc_play_radius) {
            *acc = f3 + kp->acc_sensitivity * (f2 - kp->acc_play_radius);
        }
    }
}

s8 select_1obj_first(KPADInternal* kp) {
    KPADObject* op1;
    f32 vx, vy;
    Vec2 p1, p2;

    vx = kp->sec_nrm_hori.x * kp->acc_horizon.x + kp->sec_nrm_hori.y * kp->acc_horizon.y;
    vy = kp->sec_nrm_hori.y * kp->acc_horizon.x - kp->sec_nrm_hori.x * kp->acc_horizon.y;
    vx *= kp->trust_sec_length;
    vy *= kp->trust_sec_length;

    op1 = kp->kobj_sample;
    do {
        if (op1->error_fg != 0) {
            continue;
        }

        p1.x = op1->center.x - vx;
        p1.y = op1->center.y - vy;
        p2.x = op1->center.x + vx;
        p2.y = op1->center.y + vy;

        if (p1.x <= kp->kobj_frame_min_x || p1.x >= kp->kobj_frame_max_x ||
            p1.y <= kp->kobj_frame_min_y || p1.y >= kp->kobj_frame_max_y) {
            if (p2.x > kp->kobj_frame_min_x && p2.x < kp->kobj_frame_max_x &&
                p2.y > kp->kobj_frame_min_y && p2.y < kp->kobj_frame_max_y) {
                kp->kobj_regular[1] = *op1;
                kp->kobj_regular[0].center = p1;
                kp->kobj_regular[0].error_fg = 0;
                kp->kobj_regular[0].state_fg = -1;
                return -1;
            }
        } else {
            if (p2.x <= kp->kobj_frame_min_x || p2.x >= kp->kobj_frame_max_x ||
                p2.y <= kp->kobj_frame_min_y || p2.y >= kp->kobj_frame_max_y) {
                kp->kobj_regular[0] = *op1;
                kp->kobj_regular[1].center = p2;
                kp->kobj_regular[1].error_fg = 0;
                kp->kobj_regular[1].state_fg = -1;
                return -1;
            }
        }
    } while (++op1 < &kp->kobj_sample[4]);

    return 0;
}

s8 select_1obj_continue(KPADInternal* kp) {
    KPADObject* op1;
    KPADObject* op2;
    KPADObject* rp1;
    KPADObject* rp2;
    f32 f1, vx, vy;
    f32 min = kp_err_near_pos * kp_err_near_pos;

    op1 = kp->kobj_regular;
    do {
        if (op1->error_fg != 0) {
            continue;
        }
        if (op1->state_fg != 0) {
            continue;
        }

        op2 = kp->kobj_sample;
        do {
            if (op2->error_fg != 0) {
                continue;
            }

            vx = op1->center.x - op2->center.x;
            vy = op1->center.y - op2->center.y;
            f1 = vx * vx + vy * vy;
            if (f1 < min) {
                min = f1;
                rp1 = op1;
                rp2 = op2;
            }
        } while (++op2 < &kp->kobj_sample[4]);
    } while (++op1 < &kp->kobj_regular[2]);

    if (min == kp_err_near_pos * kp_err_near_pos) {
        return 0;
    }

    *rp1 = *rp2;
    kp->sec_nrm.x = kp->sec_nrm_hori.x * kp->acc_horizon.x + kp->sec_nrm_hori.y * kp->acc_horizon.y;
    kp->sec_nrm.y = kp->sec_nrm_hori.y * kp->acc_horizon.x - kp->sec_nrm_hori.x * kp->acc_horizon.y;

    vx = kp->sec_length * kp->sec_nrm.x;
    vy = kp->sec_length * kp->sec_nrm.y;
    if (rp1 == &kp->kobj_regular[0]) {
        kp->kobj_regular[1].center.x = rp1->center.x + vx;
        kp->kobj_regular[1].center.y = rp1->center.y + vy;
        kp->kobj_regular[1].error_fg = 0;
        kp->kobj_regular[1].state_fg = -1;
    } else {
        kp->kobj_regular[0].center.x = rp1->center.x - vx;
        kp->kobj_regular[0].center.y = rp1->center.y - vy;
        kp->kobj_regular[0].error_fg = 0;
        kp->kobj_regular[0].state_fg = -1;
    }

    if (kp->status.dpd_valid_fg < 0) {
        return -1;
    }
    return 1;
}

void KPADReset(void) {
    f32 interval;
    BOOL enabled;
    s32 i;

    interval = kp_obj_interval;
    enabled = OSDisableInterrupts();
    kp_obj_interval = interval;
    kp_err_dist_min = interval / float_8066C100;
    kp_dist_vv1 = interval / float_8066C100;
    OSRestoreInterrupts(enabled);

    for (i = 3; i >= 0; i--) {
        if (WPADGetStatus() == WPAD_LIB_STATUS_3) {
            WPADControlMotor(i, 0);
        }
        inside_kpads[i].enable = 1;
    }
}

void KPADiControlWbcCallback(s32 chan, s32 result) {
    kp_wbc_enabled = result == 0;
    kp_wbc_issued = 0;
}

void KPADiUpdateTempWbcCallback(s32 chan, s32 result) {
    kp_wbc_zero_point_done = result == 0;
    kp_wbc_issued = 0;
}

void KPADDisableDPD(s32 chan) {
    inside_kpads[chan].dpd_enabled = 0;
}

void KPADEnableDPD(s32 chan) {
    inside_kpads[chan].dpd_enabled = 1;
}

void KPADiControlDpdCallback(s32 chan, s32 result) {
    KPADInternal* pad = &inside_kpads[chan];

    if (result == 0 && pad->dpd_callback != NULL && pad->dpd_cb_state == 0) {
        pad->dpd_cb_state = 1;
        pad->dpd_callback(chan, 1);
        pad->unk_55a = 0;
    }
    pad->dpd_status = WPADIsDpdEnabled(chan);
    pad->unk_558 = 0;
}

void KPADEnableAimingMode(s32 chan) {
    inside_kpads[chan].aiming_x = 1;
    inside_kpads[chan].aiming_y = 1;
}

KPADConnectCallback KPADSetConnectCallback(s32 chan, KPADConnectCallback cb) {
    BOOL enabled;
    KPADConnectCallback old;

    enabled = OSDisableInterrupts();
    old = inside_kpads[chan].connect_cb;
    inside_kpads[chan].connect_cb = cb;
    OSRestoreInterrupts(enabled);
    return old;
}

/* --- remaining catalog stubs (not yet matched) --- */

void read_kpad_button(KPADInternal* kp, u32 mode, u16 delta, u32 buttons, u32 hold,
                      u32 cl_hold) {
    u32 flag;
    u32 old;
    u32 old16;
    u32 diff;

    old = kp->status.hold;
    kp->status.hold = buttons & 0x9FFF;
    old16 = old & 0xFFFF;

    if (mode == 1) {
        flag = (kp->unk_555 != 0) ? -1 : 0;
        kp->status.hold |= (hold & ~flag) & 0x6000;
    }

    diff = kp->status.hold ^ old16;
    kp->status.trig = diff & kp->status.hold;
    kp->status.release = diff & old16;

    if (mode == 2 || (mode - 0x10) <= 1) {
        u32 cl_old16;
        u32 cl_flag;
        u32 cl_diff;

        cl_old16 = kp->status.ex_status.cl.hold & 0xFFFF;
        cl_flag = (kp->unk_555 != 0) ? -1 : 0;
        kp->status.ex_status.cl.hold = (cl_hold & ~cl_flag) & 0xFFFF;
        cl_diff = kp->status.ex_status.cl.hold ^ cl_old16;
        kp->status.ex_status.cl.trig = cl_diff & kp->status.ex_status.cl.hold;
        kp->status.ex_status.cl.release = cl_diff & cl_old16;
    }

    calc_button_repeat(kp, mode, delta);
}

void calc_acc_horizon(KPADInternal* kp) {
    f32 f1, vx, vy, ax, ay;

    f1 = (f32)sqrt(kp->hard_acc.x * kp->hard_acc.x + kp->hard_acc.y * kp->hard_acc.y);
    if (f1 == 0.0f || f1 >= 2.0f) {
        return;
    }
    ax = kp->hard_acc.x / f1;
    ay = kp->hard_acc.y / f1;

    if (f1 > 1.0f) {
        f1 = 2.0f - f1;
    }
    f1 *= f1 * kp_acc_horizon_pw;

    vx = kp->unk_d4.x * ax + kp->unk_d4.y * ay;
    vy = kp->unk_d4.y * ax - kp->unk_d4.x * ay;
    ax = (vx - kp->acc_horizon.x) * f1 + kp->acc_horizon.x;
    ay = (vy - kp->acc_horizon.y) * f1 + kp->acc_horizon.y;

    f1 = (f32)sqrt(ax * ax + ay * ay);
    if (f1 == 0.0f) {
        return;
    }
    kp->acc_horizon.x = ax / f1;
    kp->acc_horizon.y = ay / f1;

    kp->ah_circle_pos.x += (kp->acc_horizon.x - kp->ah_circle_pos.x) * kp_ah_circle_pw;
    kp->ah_circle_pos.y += (kp->acc_horizon.y - kp->ah_circle_pos.y) * kp_ah_circle_pw;

    vx = kp->acc_horizon.x - kp->ah_circle_pos.x;
    vy = kp->acc_horizon.y - kp->ah_circle_pos.y;
    if (vx * vx + vy * vy <= kp->ah_circle_radius2) {
        if (kp->ah_circle_ct_pad != 0) {
            --kp->ah_circle_ct_pad;
        }
    } else {
        kp->ah_circle_ct_pad = kp_ah_circle_ct;
    }
}

void calc_acc_vertical(KPADInternal* kp) {
    KPADStatus* sp = &kp->status;
    f32 f1, f2, ax, ay;

    ax = (f32)sqrt(f2 = kp->hard_acc.x * kp->hard_acc.x + kp->hard_acc.y * kp->hard_acc.y);
    ay = -kp->hard_acc.z;
    f1 = (f32)sqrt(f2 + ay * ay);
    if (f1 == 0.0f || f1 >= 2.0f) {
        return;
    }
    ax = ax / f1;
    ay /= f1;

    if (f1 > 1.0f) {
        f1 = 2.0f - f1;
    }
    f1 *= f1 * kp_acc_horizon_pw;

    ax = (ax - sp->acc_vertical.x) * f1 + sp->acc_vertical.x;
    ay = (ay - sp->acc_vertical.y) * f1 + sp->acc_vertical.y;

    f1 = (f32)sqrt(ax * ax + ay * ay);
    if (f1 == 0.0f) {
        return;
    }
    sp->acc_vertical.x = ax / f1;
    sp->acc_vertical.y = ay / f1;
}

static f32 clamp_acc(f32 acc, f32 clamp) {
    if (acc < 0.0f) {
        clamp = -clamp;
        if (acc < clamp) {
            return clamp;
        }
    } else if (acc > clamp) {
        return clamp;
    }
    return acc;
}

void read_kpad_acc(KPADInternal* kp, KPADUnifiedWpadStatus* uwp) {
    KPADStatus* sp = &kp->status;
    Vec t;
    Vec vec;

    switch (uwp->fmt) {
    case 1:
    case 2:
    case 4:
    case 5:
    case 7:
    case 8:
    case 0xB: {
        kp->hard_acc.x = clamp_acc((f32)(s32)(-uwp->u.core.accX) * kp->acc_scale_x, kp_rm_acc_max);
        kp->hard_acc.y = clamp_acc((f32)(s32)(-uwp->u.core.accZ) * kp->acc_scale_z, kp_rm_acc_max);
        kp->hard_acc.z = clamp_acc((f32)(s32)uwp->u.core.accY * kp->acc_scale_y, kp_rm_acc_max);

        vec = sp->acc;
        calc_acc(kp, &sp->acc.x, kp->hard_acc.x);
        calc_acc(kp, &sp->acc.y, kp->hard_acc.y);
        calc_acc(kp, &sp->acc.z, kp->hard_acc.z);
        sp->acc_value =
            (f32)sqrt(sp->acc.x * sp->acc.x + sp->acc.y * sp->acc.y + sp->acc.z * sp->acc.z);

        vec.x -= sp->acc.x;
        vec.y -= sp->acc.y;
        vec.z -= sp->acc.z;
        sp->acc_speed = (f32)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        calc_acc_horizon(kp);
        calc_acc_vertical(kp);

        if (uwp->u.core.err == 0 && uwp->u.core.dev == 1) {
            if (uwp->fmt != WPAD_FMT_FS_BTN_ACC && uwp->fmt != WPAD_FMT_FS_BTN_ACC_DPD) {
                break;
            }
            t.x = clamp_acc((f32)(s32)(-uwp->u.fs.fsAccX) * kp->fs_acc_scale_x, kp_fs_acc_max);
            t.y = clamp_acc((f32)(s32)(-uwp->u.fs.fsAccZ) * kp->fs_acc_scale_z, kp_fs_acc_max);
            t.z = clamp_acc((f32)(s32)uwp->u.fs.fsAccY * kp->fs_acc_scale_y, kp_fs_acc_max);

            if (kp->unk_55e != 0) {
                PSMTXMultVec(kp_fs_rot.m, &t, &t);
            }

            vec = sp->ex_status.fs.acc;
            calc_acc(kp, &sp->ex_status.fs.acc.x, t.x);
            calc_acc(kp, &sp->ex_status.fs.acc.y, t.y);
            calc_acc(kp, &sp->ex_status.fs.acc.z, t.z);
            sp->ex_status.fs.acc_value =
                (f32)sqrt(sp->ex_status.fs.acc.x * sp->ex_status.fs.acc.x +
                          sp->ex_status.fs.acc.y * sp->ex_status.fs.acc.y +
                          sp->ex_status.fs.acc.z * sp->ex_status.fs.acc.z);

            vec.x -= sp->ex_status.fs.acc.x;
            vec.y -= sp->ex_status.fs.acc.y;
            vec.z -= sp->ex_status.fs.acc.z;
            sp->ex_status.fs.acc_speed =
                (f32)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        }
        break;
    }
    }
}

s8 select_2obj_first(KPADInternal* kp) {
    KPADObject* op1;
    KPADObject* op2;
    KPADObject* best1;
    KPADObject* best2;
    f32 one = 1.0f;
    f32 best = kp_err_first_inpr;
    f32 d;
    f32 dist;
    f32 inv;
    f32 ny, nx;
    f32 s;
    Vec2 n;

    op1 = kp->kobj_sample;
    do {
        if (op1->error_fg != 0) {
            continue;
        }

        op2 = op1 + 1;
        do {
            if (op2->error_fg != 0) {
                continue;
            }

            nx = op2->center.x - op1->center.x;
            ny = op2->center.y - op1->center.y;
            dist = (f32)sqrt(nx * nx + ny * ny);
            inv = one / dist;
            nx *= inv;
            ny *= inv;
            n.x = kp->sec_nrm_hori.x * nx + kp->sec_nrm_hori.y * ny;
            n.y = kp->sec_nrm_hori.y * nx - kp->sec_nrm_hori.x * ny;
            s = kp->unk_544 * inv;
            if (s <= kp->unk_548 || s >= kp_err_dist_max) {
                continue;
            }

            d = kp->acc_horizon.x * n.x + kp->acc_horizon.y * n.y;
            if (d < 0.0f) {
                d = -d;
                if (d > best) {
                    best = d;
                    best1 = op2;
                    best2 = op1;
                }
            } else {
                if (d > best) {
                    best = d;
                    best1 = op1;
                    best2 = op2;
                }
            }
        } while (++op2 <= &kp->kobj_sample[3]);
    } while (++op1 < &kp->kobj_sample[3]);

    if (best == kp_err_first_inpr) {
        return 0;
    }

    kp->kobj_regular[0] = *best1;
    kp->kobj_regular[1] = *best2;
    return 2;
}

s8 select_2obj_continue(KPADInternal* kp) {
    KPADObject* op1;
    KPADObject* op2;
    KPADObject* best1;
    KPADObject* best2;
    f32 best = float_8066C0C8;
    f32 d;
    f32 dist;
    f32 inv;
    f32 vx, vy;
    Vec2 nrm;
    s32 neg;

    op1 = kp->kobj_sample;
    do {
        if (op1->error_fg != 0) {
            continue;
        }

        op2 = op1 + 1;
        do {
            if (op2->error_fg != 0) {
                continue;
            }

            vx = op2->center.x - op1->center.x;
            vy = op2->center.y - op1->center.y;
            dist = (f32)sqrt(vx * vx + vy * vy);
            inv = 1.0f / dist;
            nrm.x = vx * inv;
            nrm.y = vy * inv;
            inv *= kp->unk_544;
            if (inv <= kp->unk_548 || inv >= kp_err_dist_max) {
                continue;
            }

            inv -= kp->sec_dist;
            if (inv < 0.0f) {
                inv *= kp->dist_speedM_1;
            } else {
                inv *= kp->dist_speed_1;
            }
            if (inv >= 1.0f) {
                continue;
            }

            d = kp->sec_nrm.x * nrm.x + kp->sec_nrm.y * nrm.y;
            if (d < 0.0f) {
                d = -d;
                neg = 1;
            } else {
                neg = 0;
            }
            if (d <= kp_err_next_inpr) {
                continue;
            }

            inv += (1.0f - d) / (1.0f - kp_err_next_inpr);
            if (inv < best) {
                best = inv;
                if (neg != 0) {
                    best1 = op2;
                    best2 = op1;
                } else {
                    best1 = op1;
                    best2 = op2;
                }
            }
        } while (++op2 <= &kp->kobj_sample[3]);
    } while (++op1 < &kp->kobj_sample[3]);

    if (best == float_8066C0C8) {
        return 0;
    }

    kp->kobj_regular[0] = *best1;
    kp->kobj_regular[1] = *best2;
    return 2;
}

void calc_dpd_variable(KPADInternal* kp, s8 valid_fg) {
    KPADStatus* sp = &kp->status;
    f32 f1, f2, dist;
    Vec2 pos, vec;

    if (valid_fg == 0) {
        sp->dpd_valid_fg = 0;
        return;
    }

    pos.x = kp->sec_nrm_hori.x * kp->sec_nrm.x + kp->sec_nrm_hori.y * kp->sec_nrm.y;
    pos.y = kp->sec_nrm_hori.y * kp->sec_nrm.x - kp->sec_nrm_hori.x * kp->sec_nrm.y;

    if (sp->dpd_valid_fg == 0) {
        sp->horizon = pos;
        sp->hori_vec = Vec2_0;
        sp->hori_speed = float_8066C0B0;
    } else {
        vec.x = pos.x - sp->horizon.x;
        vec.y = pos.y - sp->horizon.y;
        f1 = (f32)sqrt(vec.x * vec.x + vec.y * vec.y);

        if (kp->hor_mode == 0) {
            if (f1 >= kp->hor_play_radius) {
                f1 = float_8066C0BC;
            } else {
                f1 /= kp->hor_play_radius;
                f1 *= f1;
                f1 *= f1;
            }
            f1 *= kp->hor_sensitivity;
            vec.x = f1 * vec.x + sp->horizon.x;
            vec.y = f1 * vec.y + sp->horizon.y;
            f1 = (f32)sqrt(vec.x * vec.x + vec.y * vec.y);
            vec.x /= f1;
            vec.y /= f1;

            sp->hori_vec.x = vec.x - sp->horizon.x;
            sp->hori_vec.y = vec.y - sp->horizon.y;
            sp->hori_speed = (f32)sqrt(sp->hori_vec.x * sp->hori_vec.x + sp->hori_vec.y * sp->hori_vec.y);

            sp->horizon = vec;
        } else {
            if (f1 > kp->hor_play_radius) {
                f1 = (f1 - kp->hor_play_radius) / f1 * kp->hor_sensitivity;
                vec.x = vec.x * f1 + sp->horizon.x;
                vec.y = vec.y * f1 + sp->horizon.y;
                f1 = (f32)sqrt(vec.x * vec.x + vec.y * vec.y);
                vec.x /= f1;
                vec.y /= f1;

                sp->hori_vec.x = vec.x - sp->horizon.x;
                sp->hori_vec.y = vec.y - sp->horizon.y;
                sp->hori_speed = (f32)sqrt(sp->hori_vec.x * sp->hori_vec.x + sp->hori_vec.y * sp->hori_vec.y);

                sp->horizon = vec;
            } else {
                sp->hori_vec = Vec2_0;
                sp->hori_speed = float_8066C0B0;
            }
        }
    }

    dist = kp->unk_544 / kp->sec_length;

    if (sp->dpd_valid_fg == 0) {
        sp->dist = dist;
        sp->dist_vec = float_8066C0B0;
        sp->dist_speed = float_8066C0B0;
    } else {
        f2 = dist - sp->dist;
        if (f2 < float_8066C0B0) {
            f1 = -f2;
        } else {
            f1 = f2;
        }

        if (kp->dist_mode == 0) {
            if (f1 >= kp->dist_play_radius) {
                f1 = float_8066C0BC;
            } else {
                f1 /= kp->dist_play_radius;
                f1 *= f1;
                f1 *= f1;
            }
            f1 *= kp->dist_sensitivity;

            sp->dist_vec = f1 * f2;
            if (sp->dist_vec < float_8066C0B0) {
                sp->dist_speed = -sp->dist_vec;
            } else {
                sp->dist_speed = sp->dist_vec;
            }

            sp->dist += sp->dist_vec;
        } else {
            if (f1 > kp->dist_play_radius) {
                f1 = (f1 - kp->dist_play_radius) / f1 * kp->dist_sensitivity;
                sp->dist_vec = f1 * f2;
                if (sp->dist_vec < float_8066C0B0) {
                    sp->dist_speed = -sp->dist_vec;
                } else {
                    sp->dist_speed = sp->dist_vec;
                }

                sp->dist += sp->dist_vec;
            } else {
                sp->dist_vec = float_8066C0B0;
                sp->dist_speed = float_8066C0B0;
            }
        }
    }

    f1 = kp->sec_nrm.x * kp->sec_nrm_hori.x + kp->sec_nrm.y * kp->sec_nrm_hori.y;
    f2 = -kp->sec_nrm.y * kp->sec_nrm_hori.x + kp->sec_nrm.x * kp->sec_nrm_hori.y;
    pos.x = (kp->kobj_regular[0].center.x + kp->kobj_regular[1].center.x) * float_8066C0B4;
    pos.y = (kp->kobj_regular[0].center.y + kp->kobj_regular[1].center.y) * float_8066C0B4;
    vec.y = f2 * pos.x + f1 * pos.y;
    vec.x = f1 * pos.x - f2 * pos.y;

    vec.y = (kp->unk_e4.y - vec.y) * kp->unk_ec;
    vec.x = (kp->unk_e4.x - vec.x) * kp->unk_ec;
    pos.x = -kp->unk_d4.y * vec.x + kp->unk_d4.x * vec.y;
    pos.y = -kp->unk_d4.x * vec.x - kp->unk_d4.y * vec.y;
    if (sp->dpd_valid_fg == 0) {
        sp->pos = pos;
        sp->vec = Vec2_0;
        sp->speed = float_8066C0B0;
    } else {
        vec.x = pos.x - sp->pos.x;
        vec.y = pos.y - sp->pos.y;
        f1 = (f32)sqrt(vec.x * vec.x + vec.y * vec.y);

        if (kp->pos_mode == 0) {
            if (f1 >= kp->pos_play_radius) {
                f1 = float_8066C0BC;
            } else {
                f1 /= kp->pos_play_radius;
                f1 *= f1;
                f1 *= f1;
            }
            f1 *= kp->pos_sensitivity;

            sp->vec.x = f1 * vec.x;
            sp->vec.y = f1 * vec.y;
            sp->speed = (f32)sqrt(sp->vec.y * sp->vec.y + sp->vec.x * sp->vec.x);

            sp->pos.x += sp->vec.x;
            sp->pos.y += sp->vec.y;
        } else {
            if (f1 > kp->pos_play_radius) {
                f1 = (f1 - kp->pos_play_radius) / f1 * kp->pos_sensitivity;
                sp->vec.x = f1 * vec.x;
                sp->vec.y = f1 * vec.y;
                sp->speed = (f32)sqrt(sp->vec.y * sp->vec.y + sp->vec.x * sp->vec.x);

                sp->pos.x += sp->vec.x;
                sp->pos.y += sp->vec.y;
            } else {
                sp->vec = Vec2_0;
                sp->speed = float_8066C0B0;
            }
        }
    }

    sp->dpd_valid_fg = valid_fg;
}

void read_kpad_dpd(KPADInternal* kp, KPADUnifiedWpadStatus* uwp) {
    s8 valid_fg;
    KPADObject* op;
    KPADObject* op2;
    KPADObject* end;
    f32 f1, f2, f3;
    u8 fmt;

    fmt = uwp->fmt;

    if (fmt == WPAD_FMT_CORE_BTN_ACC_DPD || fmt == WPAD_FMT_FS_BTN_ACC_DPD ||
        fmt == WPAD_FMT_CLASSIC_BTN_ACC_DPD ||
        (fmt == WPAD_FMT_BTN_ACC_DPD_EXTENDED && kp->dpd_status != 0)) {
        const f32 dpd_scale = float_8066C0D8;
        const f32 dpd_cx = float_8066C0DC;
        const f32 dpd_cy = float_8066C0E0;
        DPDObject* wobj_p = &uwp->u.core.obj[3];
        KPADObject* kobj_p = &kp->kobj_sample[3];

        do {
            if (wobj_p->size != 0) {
                kobj_p->center.x = (f32)(s32)wobj_p->x * dpd_scale - dpd_cx;
                kobj_p->center.y = (f32)(s32)wobj_p->y * dpd_scale - dpd_cy;
                kobj_p->error_fg = 0;
                kobj_p->state_fg = 0;
            } else {
                kobj_p->error_fg = -1;
            }
            --wobj_p;
        } while (--kobj_p >= kp->kobj_sample);
    } else {
        op = &kp->kobj_sample[3];
        do {
            op->error_fg = -1;
        } while (--op >= kp->kobj_sample);
    }

    end = &kp->kobj_sample[3];
    op = end;
    do {
        if (op->error_fg >= 0) {
            if (op->center.x <= kp->kobj_frame_min_x || op->center.x >= kp->kobj_frame_max_x ||
                op->center.y <= kp->kobj_frame_min_y || op->center.y >= kp->kobj_frame_max_y) {
                op->error_fg |= 1;
            }
        }
    } while (--op >= kp->kobj_sample);

    op = kp->kobj_sample;
    do {
        if (op->error_fg == 0) {
            op2 = op + 1;
            do {
                if (op2->error_fg == 0 && op->center.x == op2->center.x &&
                    op->center.y == op2->center.y) {
                    op2->error_fg |= 2;
                }
            } while (++op2 <= end);
        }
    } while (++op < end);

    kp->valid_objs = 0;
    op = &kp->kobj_sample[3];
    do {
        if (op->error_fg == 0) {
            kp->valid_objs++;
        }
    } while (--op >= kp->kobj_sample);

    if (!(kp->status.acc_vertical.x <= kp_err_up_inpr)) {
        s8 d = kp->status.dpd_valid_fg;
        if (d == 2 || d == -2) {
            if (kp->valid_objs >= 2) {
                if ((valid_fg = select_2obj_continue(kp)) != 0) {
                    goto select_ok;
                }
            }
            if (kp->valid_objs >= 1) {
                if ((valid_fg = select_1obj_continue(kp)) != 0) {
                    goto select_ok;
                }
            }
        } else if (d == 1 || d == -1) {
            if (kp->valid_objs >= 2) {
                if ((valid_fg = select_2obj_first(kp)) != 0) {
                    goto select_ok;
                }
            }
            if (kp->valid_objs >= 1) {
                if ((valid_fg = select_1obj_continue(kp)) != 0) {
                    goto select_ok;
                }
            }
        } else {
            if (kp->valid_objs >= 2) {
                if ((valid_fg = select_2obj_first(kp)) != 0) {
                    goto select_ok;
                }
            }
            if (kp->valid_objs == 1) {
                if ((valid_fg = select_1obj_first(kp)) != 0) {
                    goto select_ok;
                }
            }
        }
    }
    valid_fg = 0;

select_ok:
    if (valid_fg != 0) {
        f32 dx, dy, inv;

        dx = kp->kobj_regular[1].center.x - kp->kobj_regular[0].center.x;
        dy = kp->kobj_regular[1].center.y - kp->kobj_regular[0].center.y;
        f3 = (f32)sqrt(dx * dx + dy * dy);
        inv = 1.0f / f3;
        dx *= inv;
        dy *= inv;
        f1 = kp->unk_544 * inv;

        kp->sec_length = f3;
        kp->sec_nrm.x = dx;
        kp->sec_dist = f1;
        kp->sec_nrm.y = dy;
        kp->obj_horizon.x = kp->sec_nrm_hori.x * dx + kp->sec_nrm_hori.y * dy;
        kp->obj_horizon.y = kp->sec_nrm_hori.y * dx - kp->sec_nrm_hori.x * dy;

        if (kp->ah_circle_ct_pad == 0) {
            f1 = kp->obj_horizon.x * kp->acc_horizon.x + kp->obj_horizon.y * kp->acc_horizon.y;
            if (f1 <= kp_err_acc_inpr) {
                kp->kobj_regular[1].error_fg = 1;
                kp->kobj_regular[0].error_fg = 1;
                valid_fg = 0;
            }
        }

        if (kp->status.dpd_valid_fg == 2 && valid_fg == 2) {
            if (kp->unk_4fe == 200) {
                kp->trust_sec_length = kp->sec_length;
            } else {
                kp->unk_4fe++;
            }
        } else {
            kp->unk_4fe = 0;
        }
    } else {
        kp->unk_4fe = 0;
    }

    calc_dpd_variable(kp, valid_fg);
}

void clamp_stick_circle(Vec2* out, s32 x, s32 y, s32 min, s32 max) {
    f32 fx = (f32)x;
    f32 fy = (f32)y;
    f32 fmin = (f32)min;
    f32 fmax = (f32)max;
    f32 len = (f32)sqrt(fx * fx + fy * fy);
    f32 s;

    if (len <= fmin) {
        out->y = 0.0f;
        out->x = 0.0f;
    } else if (len >= fmax) {
        out->x = fx / len;
        out->y = fy / len;
    } else {
        s = (len - fmin) / (fmax - fmin);
        s = s / len;
        out->x = fx * s;
        out->y = fy * s;
    }
}

void clamp_stick_cross(Vec2* out, s32 x, s32 y, s32 min, s32 max) {
    f32 d2;
    f32 len;

    if (x < 0) {
        if (-x <= min) {
            out->x = 0.0f;
        } else if (-x >= max) {
            out->x = 1.0f;
        } else {
            out->x = (f32)(-(x + min)) / (f32)(max - min);
        }
        out->x = -out->x;
    } else {
        if (x <= min) {
            out->x = 0.0f;
        } else if (x >= max) {
            out->x = 1.0f;
        } else {
            out->x = (f32)(x - min) / (f32)(max - min);
        }
    }

    if (y < 0) {
        if (-y <= min) {
            out->y = 0.0f;
        } else if (-y >= max) {
            out->y = 1.0f;
        } else {
            out->y = (f32)(-(y + min)) / (f32)(max - min);
        }
        out->y = -out->y;
    } else {
        if (y <= min) {
            out->y = 0.0f;
        } else if (y >= max) {
            out->y = 1.0f;
        } else {
            out->y = (f32)(y - min) / (f32)(max - min);
        }
    }

    d2 = out->x * out->x + out->y * out->y;
    if (d2 > 1.0f) {
        len = (f32)sqrt(d2);
        out->x /= len;
        out->y /= len;
    }
}

/* Inline trigger clamp (no standalone retail symbol). */
static void clamp_trigger(f32* trigger, s32 tr, s32 min, s32 max) {
    if (tr <= min) {
        *trigger = 0.0f;
    } else if (tr >= max) {
        *trigger = 1.0f;
    } else {
        *trigger = (f32)(tr - min) / (f32)(max - min);
    }
}

void read_kpad_ext(KPADInternal* kp, KPADUnifiedWpadStatus* uwp) {
    void (*clamp_stick)(Vec2*, s32, s32, s32, s32);
    u8 fmt;
    s32 i;

    clamp_stick = clamp_stick_circle;
    if (kp_stick_clamp_cross != 0) {
        clamp_stick = clamp_stick_cross;
    }

    fmt = uwp->fmt;

    if (uwp->u.core.dev == WPAD_DEV_FREESTYLE) {
        if (fmt == WPAD_FMT_FS_BTN || fmt == WPAD_FMT_FS_BTN_ACC || fmt == WPAD_FMT_FS_BTN_ACC_DPD) {
            if (kp->unk_555 != 0) {
                kp->unk_555 = 0;
                kp->status.ex_status.fs.stick = Vec2_0;
                kp->status.ex_status.fs.acc.z = 0.0f;
                kp->status.ex_status.fs.acc.x = 0.0f;
                kp->status.ex_status.fs.acc.y = -1.0f;
                kp->status.ex_status.fs.acc_value = 1.0f;
                kp->status.ex_status.fs.acc_speed = 0.0f;
            }
            clamp_stick(&kp->status.ex_status.fs.stick, uwp->u.fs.fsStickX, uwp->u.fs.fsStickY,
                        kp_fs_fstick_min, kp_fs_fstick_max);
            return;
        }
    }

    if (uwp->u.core.dev == WPAD_DEV_CLASSIC) {
        if (fmt == WPAD_FMT_CLASSIC_BTN || fmt == WPAD_FMT_CLASSIC_BTN_ACC ||
            fmt == WPAD_FMT_CLASSIC_BTN_ACC_DPD) {
            if (kp->unk_555 != 0) {
                kp->unk_555 = 0;
                kp->status.ex_status.cl.hold = 0;
                kp->status.ex_status.cl.trig = 0;
                kp->status.ex_status.cl.release = 0;
                kp->status.ex_status.cl.lstick = Vec2_0;
                kp->status.ex_status.cl.rstick = Vec2_0;
                kp->status.ex_status.cl.ltrigger = 0.0f;
                kp->status.ex_status.cl.rtrigger = 0.0f;
                kp->btn_repeat_time = 0;
                kp->btn_repeat_next = kp->btn_repeat_delay;
            }
            clamp_stick(&kp->status.ex_status.cl.lstick, uwp->u.cl.clLStickX, uwp->u.cl.clLStickY,
                        kp_cl_stick_min, kp_cl_stick_max);
            clamp_stick(&kp->status.ex_status.cl.rstick, uwp->u.cl.clRStickX, uwp->u.cl.clRStickY,
                        kp_cl_stick_min, kp_cl_stick_max);
            clamp_trigger(&kp->status.ex_status.cl.ltrigger, uwp->u.cl.clTriggerL, kp_cl_trigger_min,
                          kp_cl_trigger_max);
            clamp_trigger(&kp->status.ex_status.cl.rtrigger, uwp->u.cl.clTriggerR, kp_cl_trigger_min,
                          kp_cl_trigger_max);
            return;
        }
    }

    if (uwp->u.core.dev == 0x11) { /* TGC */
        if (fmt == WPAD_FMT_BTN_ACC_DPD_EXTENDED) {
            if (kp->unk_555 != 0) {
                kp->unk_555 = 0;
                kp->status.ex_status.cl.hold = 0;
                kp->status.ex_status.cl.trig = 0;
                kp->status.ex_status.cl.release = 0;
                kp->status.ex_status.cl.lstick = Vec2_0;
                kp->status.ex_status.cl.rstick = Vec2_0;
                kp->status.ex_status.cl.ltrigger = 0.0f;
                kp->status.ex_status.cl.rtrigger = 0.0f;
                kp->btn_repeat_time = 0;
                kp->btn_repeat_next = kp->btn_repeat_delay;
            }
            clamp_stick(&kp->status.ex_status.cl.lstick, uwp->u.cl.clLStickX, uwp->u.cl.clLStickY,
                        kp_cl_stick_min, kp_cl_stick_max);
            clamp_trigger(&kp->status.ex_status.cl.rstick.x, uwp->u.cl.clRStickX, kp_ex_analog_min,
                          kp_ex_analog_max);
            clamp_trigger(&kp->status.ex_status.cl.rstick.y, uwp->u.cl.clRStickY, kp_ex_analog_min,
                          kp_ex_analog_max);
            clamp_trigger(&kp->status.ex_status.cl.ltrigger, uwp->u.cl.clTriggerL, kp_ex_trigger_min,
                          kp_ex_trigger_max);
            clamp_trigger(&kp->status.ex_status.cl.rtrigger, uwp->u.cl.clTriggerR, kp_ex_trigger_min,
                          kp_ex_trigger_max);
            return;
        }
    }

    if (uwp->u.core.dev == 0x10) { /* TR */
        if (fmt == 0x0A) {
            if (kp->unk_555 != 0) {
                kp->unk_555 = 0;
                kp->status.ex_status.cl.hold = 0;
                kp->status.ex_status.cl.trig = 0;
                kp->status.ex_status.cl.release = 0;
                kp->status.ex_status.cl.lstick = Vec2_0;
                kp->status.ex_status.cl.rstick = Vec2_0;
                kp->status.ex_status.cl.ltrigger = 0.0f;
                kp->status.ex_status.cl.rtrigger = 0.0f;
                kp->btn_repeat_time = 0;
                kp->btn_repeat_next = kp->btn_repeat_delay;
            }
            kp->status.ex_status.cl.lstick = Vec2_0;
            kp->status.ex_status.cl.rstick = Vec2_0;
            clamp_trigger(&kp->status.ex_status.cl.ltrigger, uwp->u.tr.brake, kp_ex_trigger_min,
                          kp_ex_trigger_max);
            clamp_trigger(&kp->status.ex_status.cl.rtrigger, uwp->u.tr.mascon, kp_ex_trigger_min,
                          kp_ex_trigger_max);
            return;
        }
    }

    if (uwp->u.core.dev == 3) { /* WBC */
        if (fmt == 0x0C) {
            s32 err;
            u16 count;

            if (kp->unk_555 != 0) {
                kp->unk_555 = 0;
                kp_wbc_zero_point_done = 0;
                kp_wbc_ave_sample_count = 0;
                kp_wbc_tgc_weight_issued = 0;
                kp_wbc_ave_sample[0] = 0.0;
                kp_wbc_weight_ave[0] = 0.0;
                kp_wbc_ave_sample[1] = 0.0;
                kp_wbc_weight_ave[1] = 0.0;
                kp_wbc_ave_sample[2] = 0.0;
                kp_wbc_weight_ave[2] = 0.0;
                kp_wbc_ave_sample[3] = 0.0;
                kp_wbc_weight_ave[3] = 0.0;
            }

            err = WBCGetBatteryLevel(uwp->u.bl.battery);
            if (err == 0) {
                kp->status.ex_status.wbc.err = -1;
                return;
            }
            if (kp_wbc_zero_point_done < 3) {
                kp->status.ex_status.wbc.err = -2;
                return;
            }
            if ((s8)uwp->u.bl.temp == 0x7F || (s8)uwp->u.bl.temp == -0x80) {
                kp->status.ex_status.wbc.err = -3;
                return;
            }

            err = WBCReadDummy(uwp, kp->status.ex_status.wbc.sample, 4);
            kp->status.ex_status.wbc.err = err;
            if (err >= 0) {
                kp_wbc_weight_ave[0] = (kp_wbc_weight_ave[0] * kp_wbc_ave_count +
                                        kp->status.ex_status.wbc.sample[0]) /
                                       (1.0f + kp_wbc_ave_count);
                kp_wbc_weight_ave[1] = (kp_wbc_weight_ave[1] * kp_wbc_ave_count +
                                        kp->status.ex_status.wbc.sample[1]) /
                                       (1.0f + kp_wbc_ave_count);
                kp_wbc_weight_ave[2] = (kp_wbc_weight_ave[2] * kp_wbc_ave_count +
                                        kp->status.ex_status.wbc.sample[2]) /
                                       (1.0f + kp_wbc_ave_count);
                kp_wbc_weight_ave[3] = (kp_wbc_weight_ave[3] * kp_wbc_ave_count +
                                        kp->status.ex_status.wbc.sample[3]) /
                                       (1.0f + kp_wbc_ave_count);

                if (kp_wbc_tgc_weight_issued != 0) {
                    count = kp_wbc_ave_sample_count + 1;
                    kp_wbc_ave_sample_count = count;
                    kp_wbc_ave_sample[0] = (kp_wbc_ave_sample[0] * (count - 1) +
                                            kp->status.ex_status.wbc.sample[0]) /
                                           count;
                    kp_wbc_ave_sample[1] = (kp_wbc_ave_sample[1] * (count - 1) +
                                            kp->status.ex_status.wbc.sample[1]) /
                                           count;
                    kp_wbc_ave_sample[2] = (kp_wbc_ave_sample[2] * (count - 1) +
                                            kp->status.ex_status.wbc.sample[2]) /
                                           count;
                    kp_wbc_ave_sample[3] = (kp_wbc_ave_sample[3] * (count - 1) +
                                            kp->status.ex_status.wbc.sample[3]) /
                                           count;
                    if ((f32)kp_wbc_ave_sample_count == kp_wbc_ave_count) {
                        kp_wbc_tgc_weight_issued = 0;
                        err = WBCGetTGCWeightDummy(&kp_wbc_tgc_weight, uwp);
                        kp->status.ex_status.wbc.err = err;
                        if (kp_wbc_tgc_weight < -0.5) {
                            kp->status.ex_status.wbc.err = -4;
                        }
                    }
                }
            }

            kp->status.ex_status.wbc.weight_ave[0] = kp_wbc_weight_ave[0];
            kp->status.ex_status.wbc.weight_ave[1] = kp_wbc_weight_ave[1];
            kp->status.ex_status.wbc.weight_ave[2] = kp_wbc_weight_ave[2];
            kp->status.ex_status.wbc.weight_ave[3] = kp_wbc_weight_ave[3];
            kp->status.ex_status.wbc.tgc_weight = kp_wbc_tgc_weight;
            kp->status.ex_status.wbc.tgc_weight_issued = kp_wbc_tgc_weight_issued;
        }
    }
}

s32 KPADReadEx(s32 chan, KPADStatus* status, s32 count, KPADResult* result) {
    return KPADiRead(chan, status, count, (s32*)result, 1);
}

s32 KPADiRead(s32 chan, KPADStatus* sampling_bufs, s32 length, s32* result, s32 param5) {
    KPADInternal* kp = &inside_kpads[chan];
    KPADStatus* entry;
    KPADStatus saved;
    KPADUnifiedWpadStatus* raw;
    BOOL enabled;
    s32 err = 0;
    s32 num = 0;
    s32 idx;
    u32 count;
    s32 i;
    s8 e;

    if (kp_initialized == 0) {
        err = -5;
        goto out;
    }
    if (WPADGetStatus() != WPAD_LIB_STATUS_3) {
        err = -3;
        goto out;
    }

    enabled = OSDisableInterrupts();
    if (kp->unk_4ff != 0) {
        OSRestoreInterrupts(enabled);
        err = -4;
        goto out;
    }
    kp->unk_4ff = 1;

    if (WPADProbe(chan, NULL) == WPAD_ERR_NO_CONTROLLER) {
        reset_kpad(kp);
        if (kp->dpd_callback != NULL) {
            if (kp->unk_55a != 0) {
                if (kp->dpd_cb_state == 0) {
                    kp->dpd_cb_state = 1;
                    kp->dpd_callback(chan, 1);
                    kp->unk_55a = 0;
                }
            }
        }
        kp->dpd_status = WPADIsDpdEnabled(chan);
        kp->unk_558 = 0;
        kp->unk_4ff = 0;
        OSRestoreInterrupts(enabled);
        err = -2;
        goto out;
    }
    OSRestoreInterrupts(enabled);

    if (kp->enable == 0) {
        kp->status.wpad_err = -4;
        reset_kpad(kp);
    }

    saved = sampling_bufs[0];
    if (kp->state_b <= 1 || sampling_bufs == NULL || length == 0) {
        goto skip;
    }

    enabled = OSDisableInterrupts();
    count = kp->state_b;
    if (count > length) {
        count = length;
    }

    idx = (s32)kp->state_a - 1;
    if (idx < 0) {
        idx += kp->kp_ex_count + 0x10;
    }
    {
        s32 flag = 0;
        s32 changed = 0;
        s32 j = count;

        do {
            if (idx < 0x10) {
                raw = &kp->kp_ex[idx];
            } else {
                raw = kp->kp_ex_ptr + (idx - 0x10);
            }

            if (flag != 0) {
                if (raw->u.core.err == 0) {
                    raw->u.core.err = -7;
                    flag = 0;
                } else if (raw->u.core.err == -2) {
                    flag = 0;
                }
            } else {
                if (raw->u.core.err == -7) {
                    flag = 1;
                }
            }

            if (kp->status.dev_type != raw->u.core.dev) {
                changed = 1;
            }
            if (changed != 0) {
                raw->u.core.err = -4;
            }

            idx--;
            if (idx < 0) {
                idx += kp->kp_ex_count + 0x10;
            }
        } while (--j > 0);
    }

    num = 1;
    kp->state_b = 1;
    if (count > 1) {
        num = count - 1;
    }

    idx = (s32)kp->state_a - num - 1;
    if (idx < 0) {
        idx += kp->kp_ex_count + 0x10;
    }
    entry = sampling_bufs + num;
    for (i = num; i > 0; i--) {
        entry--;
        if (idx < 0x10) {
            raw = &kp->kp_ex[idx];
        } else {
            raw = kp->kp_ex_ptr + (idx - 0x10);
        }
        *(KPADUnifiedWpadStatus*)entry = *raw;
        idx++;
        if (idx >= kp->kp_ex_count + 0x10) {
            idx = 0;
        }
    }
    OSRestoreInterrupts(enabled);

    if (kp->unk_55f == 1) {
        u32 buttons, hold, cl_hold;
        u32 dev_type;
        KPADUnifiedWpadStatus* u;

        entry = sampling_bufs + num;
        for (i = num; i > 0; i--) {
            entry--;
            u = (KPADUnifiedWpadStatus*)entry;
            e = u->u.core.err;
            if (kp->status.dev_type != u->u.core.dev) {
                kp->status.dev_type = u->u.core.dev;
                kp->unk_555 = 1;
            }
            kp->status.wpad_err = e;
            kp->status.data_format = u->fmt;

            buttons = 0xFFFF;
            hold = 0xFFFF;
            cl_hold = 0xFFFF;
            dev_type = kp->status.dev_type;
            if (e == 0) {
                if (dev_type == WPAD_DEV_FREESTYLE) {
                    hold = u->u.core.button;
                    cl_hold = 0;
                } else if (dev_type == WPAD_DEV_CLASSIC || (dev_type - 0x10) <= 1) {
                    cl_hold = u->u.cl.clButton;
                    hold = 0;
                } else {
                    hold = 0;
                    cl_hold = 0;
                }
                buttons = u->u.core.button & 0x9F1F;
            } else if (e == WPAD_ERR_CORRUPTED) {
                hold = 0;
                cl_hold = 0;
                buttons = u->u.core.button & 0x9F1F;
            } else if (e == WPAD_ERR_COMMUNICATION_ERROR) {
                buttons = u->u.core.button & 0x9F1F;
            }
            if (buttons == 0xFFFF) {
                buttons = kp->status.hold & 0x9F1F;
            }
            if (hold == 0xFFFF) {
                hold = kp->status.hold;
            }
            if (cl_hold == 0xFFFF) {
                cl_hold = kp->status.ex_status.cl.hold;
            }
            read_kpad_button(kp, kp->status.dev_type, 1, buttons, hold, cl_hold);

            if (e == 0) {
                read_kpad_ext(kp, u);
                read_kpad_acc(kp, u);
                read_kpad_dpd(kp, u);
            } else if (e == WPAD_ERR_CORRUPTED) {
                read_kpad_acc(kp, u);
                read_kpad_dpd(kp, u);
            } else {
                if (param5 == 0) {
                    kp->status.dpd_valid_fg = 0;
                }
            }
            *entry = kp->status;
        }
    } else {
        u32 buttons, hold, cl_hold;
        u32 dev = 0;
        KPADUnifiedWpadStatus* u;

        buttons = 0xFFFF;
        hold = 0xFFFF;
        cl_hold = 0xFFFF;
        entry = sampling_bufs + num;
        for (i = num; i > 0; i--) {
            entry--;
            u = (KPADUnifiedWpadStatus*)entry;
            dev = u->u.core.dev;
            if (kp->status.dev_type != u->u.core.dev) {
                kp->status.dev_type = u->u.core.dev;
                kp->unk_555 = 1;
            }
            e = u->u.core.err;
            if (e == 0) {
                if (dev == WPAD_DEV_FREESTYLE) {
                    hold = u->u.core.button;
                    cl_hold = 0;
                } else if (dev == WPAD_DEV_CLASSIC || (dev - 0x10) <= 1) {
                    cl_hold = u->u.cl.clButton;
                    hold = 0;
                } else {
                    hold = 0;
                    cl_hold = 0;
                }
                buttons = u->u.core.button & 0x9F1F;
            } else if (e == WPAD_ERR_CORRUPTED || e == WPAD_ERR_COMMUNICATION_ERROR) {
                buttons = u->u.core.button & 0x9F1F;
            }
        }
        if (buttons == 0xFFFF) {
            buttons = kp->status.hold & 0x9F1F;
        }
        if (hold == 0xFFFF) {
            hold = kp->status.hold;
        }
        if (cl_hold == 0xFFFF) {
            cl_hold = kp->status.ex_status.cl.hold;
        }
        read_kpad_button(kp, dev, (u16)num, buttons, hold, cl_hold);

        entry = sampling_bufs + num;
        for (i = num; i > 0; i--) {
            entry--;
            u = (KPADUnifiedWpadStatus*)entry;
            e = u->u.core.err;
            kp->status.wpad_err = e;
            kp->status.data_format = u->fmt;
            if (e == 0) {
                read_kpad_ext(kp, u);
                read_kpad_acc(kp, u);
                read_kpad_dpd(kp, u);
            } else if (e == WPAD_ERR_CORRUPTED) {
                read_kpad_acc(kp, u);
                read_kpad_dpd(kp, u);
            } else {
                if (param5 == 0) {
                    kp->status.dpd_valid_fg = 0;
                }
            }
            *entry = kp->status;
        }
    }

skip:
    kp->unk_4ff = 0;

out:
    if (num == 0) {
        if (err == 0) {
            if (param5 != 0) {
                sampling_bufs[0] = saved;
            }
            err = -1;
        } else if (err == -2) {
            if (param5 != 0) {
                sampling_bufs[0].dev_type = 0xFD;
                sampling_bufs[0].data_format = 0;
                sampling_bufs[0].wpad_err = -1;
            }
        }
    }
    if (result != NULL) {
        *result = err;
    }
    return num;
}

void KPADInitEx(KPADUnifiedWpadStatus* uwStatus, u32 length) {
    s32 i, k;
    u8 sens;
    BOOL enabled;
    f32 interval;
    f32 f1, f2, f3;
    f32 f27, f28;
    KPADInternal* kp;

    if (kp_initialized != 0) {
        return;
    }

    WPADInit();
    memset(inside_kpads, 0, sizeof(inside_kpads));

    sens = WPADGetDpdSensitivity();
    kp_err_dist_max = 1.0f + (f32)sens;

    for (i = 0; i < 4; i++) {
        kp = &inside_kpads[i];
        kp->connect_cb = WPADSetConnectCallback(i, KPADiConnectCallback);
        kp->sampling_cb = WPADSetSamplingCallback(i, KPADiSamplingCallback);
        kp->dpd_status = 0;
        kp_wbc_issued = 0;
        kp_wbc_enabled = 0;
        kp_wbc_setup = 0;
        kp_wbc_zero_point_done = 0;
        kp_wbc_ave_sample_count = 0;
        kp_wbc_tgc_weight_issued = 0;
        kp_wbc_ave_sample[0] = 0.0;
        kp_wbc_weight_ave[0] = 0.0;
        kp_wbc_ave_sample[1] = 0.0;
        kp_wbc_weight_ave[1] = 0.0;
        kp_wbc_ave_sample[2] = 0.0;
        kp_wbc_weight_ave[2] = 0.0;
        kp_wbc_ave_sample[3] = 0.0;
        kp_wbc_weight_ave[3] = 0.0;
        kp->dpd_enabled = 1;
        kp->unk_559 = WPADGetDpdFormat();
        kp->status.dev_type = 0xFD;
        kp->status.data_format = 0;
        kp->dist_init = idist_org;
        kp->unk_d4 = iaccXY_nrm_hori;
        kp->sec_nrm_hori = isec_nrm_hori;
        kp->unk_e4 = icenter_org;

        f27 = 1.0f;
        f28 = 0.75f;
        f1 = (f32)sqrt(f27 * f27 + f28 * f28);
        if (kp->unk_e4.x < 0.0f) {
            f27 += kp->unk_e4.x;
        } else {
            f27 -= kp->unk_e4.x;
        }
        if (kp->unk_e4.y < 0.0f) {
            f28 += kp->unk_e4.y;
        } else {
            f28 -= kp->unk_e4.y;
        }
        if (f27 < f28) {
            f27 = f28;
        }
        kp->unk_ec = f1 / f27;

        kp->acc_play_radius = 0.0f;
        kp->dist_play_radius = 0.0f;
        kp->hor_play_radius = 0.0f;
        kp->pos_play_radius = 0.0f;
        kp->acc_sensitivity = 1.0f;
        kp->dist_sensitivity = 1.0f;
        kp->hor_sensitivity = 1.0f;
        kp->pos_sensitivity = 1.0f;
        kp->acc_mode = 0;
        kp->dist_mode = 0;
        kp->hor_mode = 0;
        kp->pos_mode = 0;
        kp->btn_repeat_delay = 40000;
        kp->btn_repeat_pulse = 0;
        kp->btn_repeat_time = 0;
        kp->btn_repeat_next = 40000;
        kp->btn_cl_repeat_time = 0;
        kp->btn_cl_repeat_next = 40000;
        kp->unk_55f = 0;
        kp->aiming_x = 1;
        kp->aiming_y = 1;
        kp->unk_574 = 1;
        kp->unk_55e = 0;
        if (length != 0 && uwStatus != NULL) {
            kp->kp_ex_count = length / 4;
            kp->kp_ex_ptr = &uwStatus[(length / 4) * i];
        } else {
            kp->kp_ex_count = 0;
            kp->kp_ex_ptr = NULL;
        }
        for (k = 0; k < 16; k++) {
            kp->kp_ex[k].u.core.err = -1;
        }
        for (k = 0; k < kp->kp_ex_count; k++) {
            kp->kp_ex_ptr[k].u.core.err = -1;
        }
    }

    WPADSetCallbackByKPAD(1);
    interval = kp_obj_interval;
    enabled = OSDisableInterrupts();
    kp_obj_interval = interval;
    kp_err_dist_min = interval / float_8066C100;
    kp_dist_vv1 = interval / float_8066C100;
    OSRestoreInterrupts(enabled);

    for (i = 3; i >= 0; i--) {
        if (WPADGetStatus() == WPAD_LIB_STATUS_3) {
            WPADControlMotor(i, 0);
        }
        inside_kpads[i].enable = 1;
    }

    f2 = 0.017453292f * kp_fs_revise_deg;
    kp_fs_rot.m[0][0] = 1.0f;
    kp_fs_rot.m[0][1] = 0.0f;
    kp_fs_rot.m[0][2] = 0.0f;
    kp_fs_rot.m[0][3] = 0.0f;
    kp_fs_rot.m[1][0] = 0.0f;
    kp_fs_rot.m[1][1] = (f32)cos(0.017453292f * kp_fs_revise_deg);
    kp_fs_rot.m[1][2] = -sin(0.017453292f * kp_fs_revise_deg);
    kp_fs_rot.m[1][3] = 0.0f;
    kp_fs_rot.m[2][0] = 0.0f;
    kp_fs_rot.m[2][1] = (f32)sin(0.017453292f * kp_fs_revise_deg);
    kp_fs_rot.m[2][2] = (f32)cos(0.017453292f * kp_fs_revise_deg);
    kp_fs_rot.m[2][3] = 0.0f;
    kp_initialized = 1;
    OSRegisterVersion(__KPADVersion);
}

void KPADiConnectCallback(s32 chan, s32 result) {
    s32 i;
    KPADInternal* kp = &inside_kpads[chan];
    KPADConnectCallback cb;

    if (result == 0) {
        kp->unk_570 = 0;
        kp->unk_574 = 1;
        WPADSetCallbackByKPAD(0);
        WPADSetSamplingCallback(chan, KPADiSamplingCallback);
        WPADSetCallbackByKPAD(1);
        kp->dpd_status = 0;
        kp->unk_559 = 0;
        if (chan == 3) {
            kp_wbc_issued = 0;
            kp_wbc_enabled = 0;
            kp_wbc_setup = 0;
            kp_wbc_zero_point_done = 0;
            kp_wbc_ave_sample_count = 0;
            kp_wbc_tgc_weight_issued = 0;
            kp_wbc_ave_sample[0] = 0.0;
            kp_wbc_weight_ave[0] = 0.0;
            kp_wbc_ave_sample[1] = 0.0;
            kp_wbc_weight_ave[1] = 0.0;
            kp_wbc_ave_sample[2] = 0.0;
            kp_wbc_weight_ave[2] = 0.0;
            kp_wbc_ave_sample[3] = 0.0;
            kp_wbc_weight_ave[3] = 0.0;
        }
    } else {
        for (i = 0; i < 16; i++) {
            kp->kp_ex[i].u.core.err = -1;
        }
        for (i = 0; i < kp->kp_ex_count; i++) {
            kp->kp_ex_ptr[i].u.core.err = -1;
        }
    }

    cb = kp->connect_cb;
    if (cb != NULL) {
        cb(chan, result);
    }
}

void KPADiSamplingCallback(s32 chan) {
    KPADInternal* kp = &inside_kpads[chan];
    KPADUnifiedWpadStatus* entry;
    WPADAccGravityUnit unit = {1, 1, 1};
    u32 dev_type;
    u32 idx;
    s32 i;
    static const u8 table[12][2] = {
        {0x00, 0x01}, {0x03, 0x02}, {0x00, 0x04}, {0x01, 0x05}, {0x00, 0x07},
        {0x01, 0x08}, {0x00, 0x0B}, {0x01, 0x0B}, {0x00, 0x0C}, {0x00, 0x0C},
        {0x00, 0x0A}, {0x00, 0x0A},
    };

    if (WPADProbe(chan, (s32*)&dev_type) == WPAD_ERR_NO_CONTROLLER) {
        goto out;
    }

    idx = kp->state_a;
    if (idx >= kp->kp_ex_count + 0x10) {
        idx = 0;
    }
    if (idx >= 0x10) {
        entry = kp->kp_ex_ptr + (idx - 0x10);
    } else {
        entry = &kp->kp_ex[idx];
    }

    WPADRead(chan, (WPADStatus*)entry);
    entry->fmt = WPADGetDataFormat(chan);
    kp->state_a = idx + 1;
    if (kp->state_b < kp->kp_ex_count + 0x10) {
        kp->state_b++;
    }

    if (dev_type != kp->unk_570 && dev_type == 1) {
        kp->unk_574 = 1;
    }

    if (kp->unk_574 != 0) {
        WPADGetAccGravityUnit(chan, WPAD_DEV_CORE, &unit);
        if (unit.x * unit.y * unit.z != 0) {
            kp->acc_scale_x = 1.0f / (f32)unit.x;
            kp->acc_scale_y = 1.0f / (f32)unit.y;
            kp->acc_scale_z = 1.0f / (f32)unit.z;
        } else {
            kp->acc_scale_z = kp->acc_scale_y = kp->acc_scale_x = 0.01f;
        }
        WPADGetAccGravityUnit(chan, WPAD_DEV_FREESTYLE, &unit);
        if (unit.x * unit.y * unit.z != 0) {
            kp->fs_acc_scale_x = 1.0f / (f32)unit.x;
            kp->fs_acc_scale_y = 1.0f / (f32)unit.y;
            kp->fs_acc_scale_z = 1.0f / (f32)unit.z;
        } else {
            kp->fs_acc_scale_z = kp->fs_acc_scale_y = kp->fs_acc_scale_x = 0.005f;
        }
        kp->unk_574 = 0;
    }

    if (kp->aiming_x != 0) {
        f32 f30;
        f32 f31;
        f32 center;
        f32 f3;

        if (kp->aiming_y != 0) {
            if (WPADGetSensorBarPosition() == WPAD_SENSOR_BAR_TOP) {
                center = 0.2f;
            } else {
                center = -0.2f;
            }
        } else {
            center = 0.0f;
        }
        kp->unk_e4.x = 0.0f;
        kp->unk_e4.y = -center;
        f31 = 1.0f;
        f30 = 0.75f;
        f3 = (f32)sqrt(f31 * f31 + f30 * f30);
        if (kp->unk_e4.x < 0.0f) {
            f31 += kp->unk_e4.x;
        } else {
            f31 -= kp->unk_e4.x;
        }
        if (kp->unk_e4.y < 0.0f) {
            f30 += kp->unk_e4.y;
        } else {
            f30 -= kp->unk_e4.y;
        }
        if (f31 < f30) {
            f31 = f31;
        } else {
            f31 = f30;
        }
        kp->unk_ec = f3 / f31;
        kp->aiming_x = 0;
    }

    {
        s32 slot;

        switch (dev_type) {
        case WPAD_DEV_CORE:
        case 0xFB:
        case 0xFC:
        case 0xFF:
            slot = 0;
            break;
        case WPAD_DEV_FREESTYLE:
            slot = 2;
            break;
        case WPAD_DEV_CLASSIC:
            slot = 4;
            break;
        case 0x11: /* TGC */
            slot = 6;
            break;
        case 3: /* WBC */
            slot = 8;
            break;
        case 0x10: /* TR */
            slot = 10;
            break;
        default:
            goto out;
        }

        if (kp->dpd_enabled != 0) {
            slot++;
        }
        {
            u32 dpd_val = (u32)(WPADIsDpdEnabled(chan) ? kp->unk_559 : 0);
            const u8* tbl = table[slot];
            if (dpd_val != tbl[0]) {
                if (kp->dpd_callback != NULL && kp->unk_55a == 0) {
                    kp->unk_55a = 1;
                    kp->dpd_callback(chan, 0);
                    kp->dpd_cb_state = 0;
                }
                if (kp->unk_558 == 0) {
                    kp->unk_558 = 1;
                    if (WPADControlDpd(chan, tbl[0], KPADiControlDpdCallback) == 0) {
                        kp->unk_559 = tbl[0];
                    }
                }
            } else {
                if (entry->fmt != tbl[1]) {
                    WPADSetDataFormat(chan, tbl[1]);
                }
            }
        }
    }

    if (dev_type == 3) { /* WBC */
        if (kp_wbc_issued == 0 && kp_wbc_enabled == 0) {
            if (WPADControlBLC(chan, 0xAA, KPADiControlWbcCallback) == 0) {
                kp_wbc_issued = 1;
            }
            goto out;
        }
        if (kp_wbc_enabled != 0) {
            if (kp_wbc_setup == 0) {
                if (kp_wbc_issued == 0) {
                    if (WBCSetupCalibration() == 0) {
                        kp_wbc_issued = 1;
                    }
                    goto out;
                }
                kp_wbc_setup = WBCGetCalibrationStatus();
                kp_wbc_issued = kp_wbc_setup == 0;
                goto out;
            }
        }
        if (kp_wbc_issued != 0 || kp_wbc_setup == 0 || kp_wbc_zero_point_done >= 3) {
            goto out;
        }

        switch (kp_wbc_zero_point_done) {
        case 0:
            if (WPADControlBLC(chan, 0, KPADiUpdateTempWbcCallback) == 0) {
                kp_wbc_issued = 1;
            }
            break;
        case 1:
            if ((s8)entry->u.bl.temp == 0x7F || (s8)entry->u.bl.temp == -0x80) {
                kp_wbc_zero_point_done = 0;
                kp_wbc_ave_sample_count = 0;
                kp_wbc_tgc_weight_issued = 0;
                kp_wbc_ave_sample[0] = 0.0;
                kp_wbc_weight_ave[0] = 0.0;
                kp_wbc_ave_sample[1] = 0.0;
                kp_wbc_weight_ave[1] = 0.0;
                kp_wbc_ave_sample[2] = 0.0;
                kp_wbc_weight_ave[2] = 0.0;
                kp_wbc_ave_sample[3] = 0.0;
                kp_wbc_weight_ave[3] = 0.0;
            } else {
                kp_wbc_ave_sample_count++;
                if (kp_wbc_ave_sample_count > kp_wbc_wait_count) {
                    kp_wbc_zero_point_done = 2;
                    kp_wbc_ave_sample_count = 0;
                }
            }
            break;
        case 2:
            if (entry->u.bl.err == 0) {
                u16 count;

                count = kp_wbc_ave_sample_count + 1;
                kp_wbc_ave_sample_count = count;
                for (i = 0; i < 4; i++) {
                    kp_wbc_ave_sample[i] =
                        (kp_wbc_ave_sample[i] * (count - 1) + entry->u.bl.press[i]) / count;
                }
                if ((f32)kp_wbc_ave_sample_count > kp_wbc_ave_count) {
                    kp_wbc_zero_point_done = 3;
                    WBCSetZEROPointDummy(kp_wbc_ave_sample, 4);
                }
            }
            break;
        }
    }

out:
    if (kp->sampling_cb != NULL) {
        kp->sampling_cb(chan);
    }
}
