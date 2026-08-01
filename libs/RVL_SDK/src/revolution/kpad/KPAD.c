#include <revolution/OS.h>
#include <revolution/KPAD.h>

/* Internal pad state (stride 0x578, 4 pads in inside_kpads). */
typedef struct KPADObject {
    Vec2 center;  // at 0x0
    s8 error_fg;   // at 0x8
    s8 state_fg;  // at 0x9
    u8 pad[2]; // at 0xA
} KPADObject;

typedef struct KPADInternal {
    /* 0x00 */ KPADStatus status;
    /* 0x84 */ u8 reserved84[0xC8 - 0x84];
    /* 0xC8 */ f32 acc_play_radius;
    /* 0xCC */ f32 acc_sensitivity;
    /* 0xD0 */ f32 dist_init;
    /* 0xD4 */ f32 unk_d4;
    /* 0xD8 */ f32 unk_d8;
    /* 0xDC */ Vec2 sec_nrm_hori;
    /* 0xE4 */ f32 unk_e4;
    /* 0xE8 */ f32 unk_e8;
    /* 0xEC */ f32 unk_ec;
    /* 0xF0 */ KPADObject kobj_sample[4];
    /* 0x120 */ KPADObject kobj_regular[2];
    /* 0x138 */ u16 valid_objs;
    /* 0x13A */ u8 state_a;
    /* 0x13B */ u8 unk_13b;
    /* 0x13C */ u8 reserved13c[0x4C4 - 0x13C];
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
    /* 0x510 */ f32 unk_510;
    /* 0x514 */ f32 unk_514;
    /* 0x518 */ f32 unk_518;
    /* 0x51C */ u8 reserved51c[0x528 - 0x51C];
    /* 0x528 */ f32 kobj_frame_min_x;
    /* 0x52C */ f32 kobj_frame_min_y;
    /* 0x530 */ f32 kobj_frame_max_x;
    /* 0x534 */ f32 kobj_frame_max_y;
    /* 0x538 */ f32 dist_speed_1;
    /* 0x53C */ f32 dist_speedM_1;
    /* 0x540 */ f32 ah_circle_radius2;
    /* 0x544 */ f32 unk_544;
    /* 0x548 */ f32 unk_548;
    /* 0x54C */ u32 unk_54c;
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
    /* 0x560 */ u8 reserved560[0x56C - 0x560];
    /* 0x56C */ u32 acc_mode;
    /* 0x570 */ u8 reserved570[0x578 - 0x570];
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

Vec2 Vec2_0;

KPADInternal inside_kpads[4];

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

    kp->unk_13b = 0;
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

s32 select_1obj_first(KPADInternal* kp) {
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

s32 select_1obj_continue(KPADInternal* kp) {
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

void read_kpad_button() {}

void calc_acc_horizon() {}

void calc_acc_vertical() {}

void read_kpad_acc() {}

void select_2obj_first() {}

void select_2obj_continue() {}

void calc_dpd_variable() {}

void read_kpad_dpd() {}

void clamp_stick_circle() {}

void clamp_stick_cross() {}

void read_kpad_ext() {}

s32 KPADReadEx(s32 chan, KPADStatus* status, s32 count, KPADResult* result) {
    return 0;
}

void KPADiRead() {}

void KPADInitEx(KPADUnifiedWpadStatus* uwStatus, u32 length) {}

void KPADiConnectCallback() {}

void KPADiSamplingCallback() {}
