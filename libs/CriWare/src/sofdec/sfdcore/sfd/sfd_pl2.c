#include <harness_catalog.h>

extern int SFLIB_CheckHn(void *);
extern int SFLIB_SetErr(void *, int);
extern int SFTIM_Pause(void *, int);
extern int SFTIM_SetSpeed(void *, int, int);
extern int SFAOAP_SetSpeed(void *, int, int);
extern int SFTRN_CallTrtTrif(void *, int, int, int *, int);
extern int fn_803CD484(void *);

/* SFD player handle layout */
typedef struct SfdPlayer {
    u8 field_0x00[0x50];
    s32 flag;      /* 0x50 */
    s32 state;     /* 0x54 */
    s32 substate;  /* 0x58 */
    s32 pause;     /* 0x5C */
    s32 pausect;   /* 0x60 */
} SfdPlayer;

/* struct offsets for SFD player handle */
#define P_FLAG     0x50
#define P_STATE    0x54
#define P_SUBSTATE 0x58
#define P_PAUSE    0x5C
#define P_PAUSECT  0x60

/* Put the player on standby (sub-state 3). */
#pragma scheduling off
int SFPL2_Standby(void *handle) {
    fn_803CD484(handle);
    *(u32 *)((u8 *)handle + P_SUBSTATE) = 3;
    return 0;
}
#pragma scheduling on

int SFD_Standby(void *handle) {
    if (SFLIB_CheckHn(handle))
        return SFLIB_SetErr(NULL, 0xFF000143);
    fn_803CD484(handle);
    *(u32 *)((u8 *)handle + P_SUBSTATE) = 3;
    return 0;
}

/* Pause the player.
 * pause_flag=0: resume, pause_flag!=0: pause
 * Returns 0 on success, transition error code on failure.
 *
 * The transition kind depends on both the requested state and the previous
 * one: pausing while already paused only forces (2), pausing from unpaused
 * nests (1), resuming unwinds the nesting count (0). Resuming with no pause
 * outstanding does nothing. Each transition runs only in sub-state 3 or 4. */
int SFD_Pause(SfdPlayer *player, int pause_flag) {
    s32 action;
    s32 ret;

    if (SFLIB_CheckHn(player))
        return SFLIB_SetErr(NULL, 0xFF000142);

    if (pause_flag == 0) {
        if (player->pause != 0)
            action = 0;
        else
            return 0;
    } else {
        action = 2;
        if (player->pause == 0)
            action = 1;
    }

    player->pause = pause_flag;

    if (action == 2) {
        ret = 0;
        if (player->state == 4) {
            if (player->substate == 3 || player->substate == 4) {
                int data = 2;
                int trt;
                SFTIM_Pause(player, 2);
                trt = SFTRN_CallTrtTrif(player, 7, 8, &data, 0);
                ret = 0;
                if (trt != 0)
                    ret = trt;
            } else {
                ret = 0;
            }
        }
    } else if (action == 1) {
        /* Only enter pause on the first nested pause request. */
        ret = 0;
        if (player->pausect++ == 0) {
            if (player->substate == 3 || player->substate == 4) {
                int data = 1;
                int trt;
                SFTIM_Pause(player, 1);
                trt = SFTRN_CallTrtTrif(player, 7, 8, &data, 0);
                ret = 0;
                if (trt != 0)
                    ret = trt;
            } else {
                ret = 0;
            }
        }
    } else if (action == 0) {
        /* Only resume when the last nested pause is released. */
        ret = 0;
        if (--player->pausect == 0) {
            if (player->substate == 3 || player->substate == 4) {
                int data = 0;
                int trt;
                SFTIM_Pause(player, 0);
                trt = SFTRN_CallTrtTrif(player, 7, 8, &data, 0);
                ret = 0;
                if (trt != 0)
                    ret = trt;
            } else {
                ret = 0;
            }
        }
    }

    player->flag = 1;
    return ret;
}

/* Pause/resume dispatch. op: 2=pause force, 1=pause (nested-count guarded),
 * 0=resume (nested-count guarded). Returns 0 or a transition error code. */
/* Run one pause/resume transition; returns 0 or the transition error code. */
int SFPL2_Pause(SfdPlayer *p, int op) {
    int ret = 0;

    switch (op) {
    case 2:
        if (p->state != 4)
            goto done;
        if (p->substate != 3 && p->substate != 4) {
            ret = 0;
            goto done;
        }
        SFTIM_Pause(p, 2);
        {
            int data = 2;
            int result = SFTRN_CallTrtTrif(p, 7, 8, &data, 0);
            if (result != 0)
                ret = result;
        }
        break;

    case 1:
        /* Only enter pause on the first nested pause request. */
        if (p->pausect++ != 0)
            goto done;
        if (p->substate != 3 && p->substate != 4) {
            ret = 0;
            goto done;
        }
        SFTIM_Pause(p, 1);
        {
            int data = 1;
            int result = SFTRN_CallTrtTrif(p, 7, 8, &data, 0);
            if (result != 0)
                ret = result;
        }
        break;

    case 0:
        /* Only resume when the last nested pause is released. */
        if (--p->pausect != 0)
            goto done;
        if (p->substate != 3 && p->substate != 4) {
            ret = 0;
            goto done;
        }
        SFTIM_Pause(p, 0);
        {
            int data = 0;
            int result = SFTRN_CallTrtTrif(p, 7, 8, &data, 0);
            if (result != 0)
                ret = result;
        }
        break;
    }

done:
    return ret;
}

/* Set playback speed (rational numerator/denominator) */
int SFD_SetSpeedRational(void *handle, int num, int den) {
    if (SFLIB_CheckHn(handle))
        return SFLIB_SetErr(NULL, 0xFF000144);

    SFTIM_SetSpeed(handle, num, den);
    SFAOAP_SetSpeed(handle, num, den);
    return 0;
}
