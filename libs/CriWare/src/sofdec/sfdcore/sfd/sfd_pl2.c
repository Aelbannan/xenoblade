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
 * Returns 0 on success, error code on failure */
int SFD_Pause(SfdPlayer *p, int pause_flag) {
    int ret;
    s32 action;

    if (SFLIB_CheckHn(p))
        return SFLIB_SetErr(NULL, 0xFF000142);

    /* Decide which transition to run based on old/new pause state. */
    if (pause_flag == 0) {
        if (p->pause == 0)
            return 0;
        action = 0;
    } else {
        action = 2;
        if (p->pause == 0)
            action = 1;
    }

    p->pause = pause_flag;
    ret = 0;

    switch (action) {
    case 2:
        if (p->state != 4)
            break;
        if (p->substate == 3 || p->substate == 4) {
            SFTIM_Pause(p, 2);
            {
                int data = 2;
                int result = SFTRN_CallTrtTrif(p, 7, 8, &data, 0);
                if (result != 0)
                    ret = result;
            }
        } else {
            ret = 0;
        }
        break;

    case 1:
        /* Only enter pause on the first nested pause request. */
        {
            s32 ct = p->pausect++;
            if (ct != 0)
                break;
            if (p->substate == 3 || p->substate == 4) {
                SFTIM_Pause(p, 1);
                {
                    int data = 1;
                    int result = SFTRN_CallTrtTrif(p, 7, 8, &data, 0);
                    if (result != 0)
                        ret = result;
                }
            } else {
                ret = 0;
            }
        }
        break;

    case 0:
        /* Only resume when the last nested pause is released. */
        {
            s32 ct = --p->pausect;
            if (ct != 0)
                break;
            if (p->substate == 3 || p->substate == 4) {
                SFTIM_Pause(p, 0);
                {
                    int data = 0;
                    int result = SFTRN_CallTrtTrif(p, 7, 8, &data, 0);
                    if (result != 0)
                        ret = result;
                }
            } else {
                ret = 0;
            }
        }
        break;
    }

    p->flag = 1;
    return ret;
}

int SFPL2_Pause(SfdPlayer *p, int op) {
    int ret = 0;

    switch (op) {
    case 2:
        if (p->state != 4)
            return 0;
        if (p->substate == 3 || p->substate == 4) {
            SFTIM_Pause(p, 2);
            {
                int data = 2;
                int result = SFTRN_CallTrtTrif(p, 7, 8, &data, 0);
                if (result != 0)
                    ret = result;
            }
        } else {
            ret = 0;
        }
        break;

    case 1:
        /* Only enter pause on the first nested pause request. */
        {
            s32 ct = p->pausect++;
            if (ct != 0)
                break;
            if (p->substate == 3 || p->substate == 4) {
                SFTIM_Pause(p, 1);
                {
                    int data = 1;
                    int result = SFTRN_CallTrtTrif(p, 7, 8, &data, 0);
                    if (result != 0)
                        ret = result;
                }
            } else {
                ret = 0;
            }
        }
        break;

    case 0:
        /* Only resume when the last nested pause is released. */
        {
            s32 ct = --p->pausect;
            if (ct != 0)
                break;
            if (p->substate == 3 || p->substate == 4) {
                SFTIM_Pause(p, 0);
                {
                    int data = 0;
                    int result = SFTRN_CallTrtTrif(p, 7, 8, &data, 0);
                    if (result != 0)
                        ret = result;
                }
            } else {
                ret = 0;
            }
        }
        break;
    }

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
