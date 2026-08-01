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
    /* 0x84 */ u8 reserved84[0xB0 - 0x84];
    /* 0xB0 */ f32 pos_play_radius;
    /* 0xB4 */ f32 pos_sensitivity;
    /* 0xB8 */ f32 hor_play_radius;
    /* 0xBC */ f32 hor_sensitivity;
    /* 0xC0 */ f32 dist_play_radius;
    /* 0xC4 */ f32 dist_sensitivity;
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
f32 kp_rm_acc_max = 3.4f;
f32 kp_fs_acc_max = 2.1f;
f32 kp_fs_revise_deg = 24.0f;
u8 kp_initialized;

f64 kp_wbc_ave_sample[4];
f64 kp_wbc_weight_ave[4];

extern void WPADSetCallbackByKPAD(s32 callback);
void KPADiSamplingCallback(s32 chan);
void KPADiConnectCallback(s32 chan, s32 result);

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

    vx = kp->unk_d4 * ax + kp->unk_d8 * ay;
    vy = kp->unk_d8 * ax - kp->unk_d4 * ay;
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
    ax /= f1;
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

void read_kpad_acc() {
    kp_fs_rot.m[0][0] = 0.0f;
}

s32 select_2obj_first(KPADInternal* kp) {
    KPADObject* op1;
    KPADObject* op2;
    KPADObject* best1;
    KPADObject* best2;
    f32 best = kp_err_first_inpr;
    f32 d;
    f32 dist;
    f32 inv;
    f32 nx, ny;
    f32 s;
    f32 rx, ry;

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
            inv = 1.0f / dist;
            nx *= inv;
            ny *= inv;
            s = kp->unk_544 * inv;
            if (s <= kp->unk_548 || s >= kp_err_dist_max) {
                continue;
            }

            rx = kp->sec_nrm_hori.x * nx + kp->sec_nrm_hori.y * ny;
            ry = kp->sec_nrm_hori.y * nx - kp->sec_nrm_hori.x * ny;
            d = kp->acc_horizon.x * rx + kp->acc_horizon.y * ry;
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

s32 select_2obj_continue(KPADInternal* kp) {
    KPADObject* op1;
    KPADObject* op2;
    KPADObject* best1;
    KPADObject* best2;
    f32 best = 2.0f;
    f32 d;
    f32 dist;
    f32 inv;
    f32 nx, ny;
    f32 s;
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

            nx = op2->center.x - op1->center.x;
            ny = op2->center.y - op1->center.y;
            dist = (f32)sqrt(nx * nx + ny * ny);
            inv = 1.0f / dist;
            nx *= inv;
            ny *= inv;
            s = kp->unk_544 * inv;
            if (s <= kp->unk_548 || s >= kp_err_dist_max) {
                continue;
            }

            s -= kp->sec_dist;
            if (s < 0.0f) {
                s *= kp->dist_speedM_1;
            } else {
                s *= kp->dist_speed_1;
            }
            if (s >= 1.0f) {
                continue;
            }

            d = kp->sec_nrm.x * nx + kp->sec_nrm.y * ny;
            if (d < 0.0f) {
                d = -d;
                neg = 1;
            } else {
                neg = 0;
            }
            if (d <= kp_err_next_inpr) {
                continue;
            }

            s += (1.0f - d) / (1.0f - kp_err_next_inpr);
            if (s < best) {
                best = s;
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

    if (best == 2.0f) {
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
        sp->vec = Vec2_0;
        sp->speed = 0.0f;
    } else {
        vec.x = pos.x - sp->horizon.x;
        vec.y = pos.y - sp->horizon.y;
        f1 = (f32)sqrt(vec.x * vec.x + vec.y * vec.y);

        if (kp->hor_mode == 0) {
            if (f1 >= kp->hor_play_radius) {
                f1 = 1.0f;
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

            sp->vec.x = vec.x - sp->horizon.x;
            sp->vec.y = vec.y - sp->horizon.y;
            sp->speed = (f32)sqrt(sp->vec.x * sp->vec.x + sp->vec.y * sp->vec.y);

            sp->horizon = vec;
        } else {
            if (f1 > kp->hor_play_radius) {
                f1 = (f1 - kp->hor_play_radius) / f1 * kp->hor_sensitivity;
                vec.x = vec.x * f1 + sp->horizon.x;
                vec.y = vec.y * f1 + sp->horizon.y;
                f1 = (f32)sqrt(vec.x * vec.x + vec.y * vec.y);
                vec.x /= f1;
                vec.y /= f1;

                sp->vec.x = vec.x - sp->horizon.x;
                sp->vec.y = vec.y - sp->horizon.y;
                sp->speed = (f32)sqrt(sp->vec.x * sp->vec.x + sp->vec.y * sp->vec.y);

                sp->horizon = vec;
            } else {
                sp->vec = Vec2_0;
                sp->speed = 0.0f;
            }
        }
    }

    dist = kp->unk_544 / kp->sec_length;

    if (sp->dpd_valid_fg == 0) {
        sp->dist = dist;
        sp->dist_vec = 0.0f;
        sp->dist_speed = 0.0f;
    } else {
        f2 = dist - sp->dist;
        if (f2 < 0.0f) {
            f1 = -f2;
        } else {
            f1 = f2;
        }

        if (kp->dist_mode == 0) {
            if (f1 >= kp->dist_play_radius) {
                f1 = 1.0f;
            } else {
                f1 /= kp->dist_play_radius;
                f1 *= f1;
                f1 *= f1;
            }
            f1 *= kp->dist_sensitivity;

            sp->dist_vec = f1 * f2;
            if (sp->dist_vec < 0.0f) {
                sp->dist_speed = -sp->dist_vec;
            } else {
                sp->dist_speed = sp->dist_vec;
            }

            sp->dist += sp->dist_vec;
        } else {
            if (f1 > kp->dist_play_radius) {
                f1 = (f1 - kp->dist_play_radius) / f1 * kp->dist_sensitivity;
                sp->dist_vec = f1 * f2;
                if (sp->dist_vec < 0.0f) {
                    sp->dist_speed = -sp->dist_vec;
                } else {
                    sp->dist_speed = sp->dist_vec;
                }

                sp->dist += sp->dist_vec;
            } else {
                sp->dist_vec = 0.0f;
                sp->dist_speed = 0.0f;
            }
        }
    }

    pos.x = (kp->kobj_regular[0].center.x + kp->kobj_regular[1].center.x) * 0.5f;
    pos.y = (kp->kobj_regular[0].center.y + kp->kobj_regular[1].center.y) * 0.5f;

    f1 = kp->sec_nrm.x * kp->sec_nrm_hori.x + kp->sec_nrm.y * kp->sec_nrm_hori.y;
    f2 = -kp->sec_nrm.y * kp->sec_nrm_hori.x + kp->sec_nrm.x * kp->sec_nrm_hori.y;
    vec.x = f1 * pos.x - f2 * pos.y;
    vec.y = f2 * pos.x + f1 * pos.y;

    vec.x = (kp->unk_e4 - vec.y) * kp->unk_ec;
    vec.y = (kp->unk_e8 - vec.x) * kp->unk_ec;

    pos.x = -kp->unk_d8 * vec.x + kp->unk_d4 * vec.y;
    pos.y = -kp->unk_d4 * vec.x - kp->unk_d8 * vec.y;
    if (sp->dpd_valid_fg == 0) {
        sp->pos = pos;
        sp->vec = Vec2_0;
        sp->speed = 0.0f;
    } else {
        vec.x = pos.x - sp->pos.x;
        vec.y = pos.y - sp->pos.y;
        f1 = (f32)sqrt(vec.x * vec.x + vec.y * vec.y);

        if (kp->pos_mode == 0) {
            if (f1 >= kp->pos_play_radius) {
                f1 = 1.0f;
            } else {
                f1 /= kp->pos_play_radius;
                f1 *= f1;
                f1 *= f1;
            }
            f1 *= kp->pos_sensitivity;

            sp->vec.x = f1 * vec.x;
            sp->vec.y = f1 * vec.y;
            sp->speed = (f32)sqrt(sp->vec.x * sp->vec.x + sp->vec.y * sp->vec.y);

            sp->pos.x += sp->vec.x;
            sp->pos.y += sp->vec.y;
        } else {
            if (f1 > kp->pos_play_radius) {
                f1 = (f1 - kp->pos_play_radius) / f1 * kp->pos_sensitivity;
                sp->vec.x = f1 * vec.x;
                sp->vec.y = f1 * vec.y;
                sp->speed = (f32)sqrt(sp->vec.x * sp->vec.x + sp->vec.y * sp->vec.y);

                sp->pos.x += sp->vec.x;
                sp->pos.y += sp->vec.y;
            } else {
                sp->vec = Vec2_0;
                sp->speed = 0.0f;
            }
        }
    }

    sp->dpd_valid_fg = valid_fg;
}

void read_kpad_dpd() {}

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

void read_kpad_ext() {
    kp_wbc_ave_sample[0] = 0.0;
    kp_wbc_weight_ave[0] = 0.0;
}

s32 KPADReadEx(s32 chan, KPADStatus* status, s32 count, KPADResult* result) {
    return 0;
}

void KPADiRead() {}

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
        kp->unk_d4 = iaccXY_nrm_hori.x;
        kp->unk_d8 = iaccXY_nrm_hori.y;
        kp->sec_nrm_hori = isec_nrm_hori;
        kp->unk_e4 = icenter_org.x;
        kp->unk_e8 = icenter_org.y;

        f27 = 1.0f;
        f28 = 0.75f;
        f1 = (f32)sqrt(f27 * f27 + f28 * f28);
        if (icenter_org.x < 0.0f) {
            f27 += icenter_org.x;
        } else {
            f27 -= icenter_org.x;
        }
        if (icenter_org.y < 0.0f) {
            f28 += icenter_org.y;
        } else {
            f28 -= icenter_org.y;
        }
        if (f28 < f27) {
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
        kp->pos_mode = 0;
        kp->hor_mode = 0;
        kp->dist_mode = 0;
        kp->acc_mode = 0;
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
            kp->kp_ex_ptr = &uwStatus[i * (length / 4)];
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

void KPADiSamplingCallback(s32 chan) {}
