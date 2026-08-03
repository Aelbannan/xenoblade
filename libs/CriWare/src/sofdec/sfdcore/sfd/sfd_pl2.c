#include <harness_catalog.h>

extern int SFLIB_CheckHn(void *);
extern int SFLIB_SetErr(void *, int);
extern int SFTIM_Pause(void *, int);
extern int SFTIM_SetSpeed(void *, int, int);
extern int SFAOAP_SetSpeed(void *, int, int);
extern int SFTRN_CallTrtTrif(void *, int, int, int *, int);
extern int fn_803CD484(void *);

/* struct offsets for SFD player handle */
#define P_FLAG     0x50
#define P_STATE    0x54
#define P_SUBSTATE 0x58
#define P_PAUSE    0x5C
#define P_PAUSECT  0x60

/* Pause the player.
 * pause_flag=0: resume, pause_flag!=0: pause
 * Returns 0 on success, error code on failure */
int SFD_Pause(void *handle, int pause_flag) {
    int ret = 0;
    u32 current_pause;
    s32 action;

    if (SFLIB_CheckHn(handle)) {
        return SFLIB_SetErr(NULL, 0xFF000142);
    }

    current_pause = *(u32 *)((u8 *)handle + P_PAUSE);

    if (pause_flag == 0) {
        if (current_pause == 0)
            return 0;
        action = 0;
    } else {
        action = 2;
        if (current_pause == 0)
            action = 1;
    }

    *(u32 *)((u8 *)handle + P_PAUSE) = pause_flag;

    if (action == 2)
        goto act2;
    if (action == 1)
        goto act1;
    if (action == 0)
        goto act0;
    goto done;

act2:
    if (*(s32 *)((u8 *)handle + P_STATE) != 4)
        goto done;
    if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 3)
        goto body2;
    if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 4)
        goto body2;
    ret = 0;
    goto done;
body2:
    SFTIM_Pause(handle, 2);
    {
        int data = 2;
        int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
        if (result) ret = result;
    }
    goto done;

act1:
    {
        s32 ct = *(s32 *)((u8 *)handle + P_PAUSECT);
        *(s32 *)((u8 *)handle + P_PAUSECT) = ct + 1;
        if (ct != 0)
            goto done;
        if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 3)
            goto body1;
        if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 4)
            goto body1;
        ret = 0;
        goto done;
    }
body1:
    SFTIM_Pause(handle, 1);
    {
        int data = 1;
        int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
        if (result) ret = result;
    }
    goto done;

act0:
    {
        s32 ct = *(s32 *)((u8 *)handle + P_PAUSECT);
        *(s32 *)((u8 *)handle + P_PAUSECT) = ct - 1;
        if (ct - 1 != 0)
            goto done;
        if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 3)
            goto body0;
        if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 4)
            goto body0;
        ret = 0;
        goto done;
    }
body0:
    SFTIM_Pause(handle, 0);
    {
        int data = 0;
        int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
        if (result) ret = result;
    }

done:
    *(u32 *)((u8 *)handle + P_FLAG) = 1;
    return ret;
}

int SFPL2_Pause(void *handle, int op) {
    int ret = 0;

    if (op == 2)
        goto case2;
    else if (op == 1)
        goto case1;
    else if (op == 0)
        goto case0;
    else
        goto done;

case2:
    if (*(s32 *)((u8 *)handle + P_STATE) != 4)
        goto done;
    if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 3)
        goto body2;
    if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 4)
        goto body2;
    ret = 0;
    goto done;
body2:
    SFTIM_Pause(handle, 2);
    {
        int data = 2;
        int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
        if (result) ret = result;
    }
    goto done;

case1:
    {
        s32 ct = *(s32 *)((u8 *)handle + P_PAUSECT);
        *(s32 *)((u8 *)handle + P_PAUSECT) = ct + 1;
        if (ct != 0)
            goto done;
        if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 3)
            goto body1;
        if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 4)
            goto body1;
        ret = 0;
        goto done;
    }
body1:
    SFTIM_Pause(handle, 1);
    {
        int data = 1;
        int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
        if (result) ret = result;
    }
    goto done;

case0:
    {
        s32 ct = *(s32 *)((u8 *)handle + P_PAUSECT);
        *(s32 *)((u8 *)handle + P_PAUSECT) = ct - 1;
        if (ct - 1 != 0)
            goto done;
        if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 3)
            goto body0;
        if (*(s32 *)((u8 *)handle + P_SUBSTATE) == 4)
            goto body0;
        ret = 0;
        goto done;
    }
body0:
    SFTIM_Pause(handle, 0);
    {
        int data = 0;
        int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
        if (result) ret = result;
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
