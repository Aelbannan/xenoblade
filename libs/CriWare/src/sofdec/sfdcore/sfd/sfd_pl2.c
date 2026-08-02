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

    if (SFLIB_CheckHn(handle)) {
        return SFLIB_SetErr(NULL, 0xFF000142);
    }

    {
        u32 *h = (u32 *)handle;
        u32 current_pause = h[P_PAUSE / 4];
        u32 new_pause = pause_flag;
        u32 action;

        if (pause_flag == 0) {
            action = (current_pause != 0) ? 1 : 0;
        } else {
            action = (current_pause == 0) ? 2 : 0;
        }

        h[P_PAUSE / 4] = new_pause;

        switch (action) {
        case 0: /* no state change needed */
            break;

        case 2: /* pause */
            if (h[P_STATE / 4] == 4) {
                if (h[P_SUBSTATE / 4] == 3 || h[P_SUBSTATE / 4] == 4) {
                    SFTIM_Pause(handle, 2);
                    {
                        int data = 2;
                        int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
                        if (result) ret = result;
                    }
                }
            }
            break;

        case 1: /* unpause */
            if (h[P_STATE / 4] == 4) {
                if (h[P_SUBSTATE / 4] == 3 || h[P_SUBSTATE / 4] == 4) {
                    u32 ct = h[P_PAUSECT / 4];
                    if (ct == 0) {
                        SFTIM_Pause(handle, 1);
                        {
                            int data = 1;
                            int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
                            if (result) ret = result;
                        }
                    }
                }
            }
            break;
        }

        h[P_PAUSECT / 4] = 0;
        if (action == 1) {
            h[P_PAUSECT / 4] = 1;
        }
    }

    return ret;
}

/* Player pause v2 - returns error code directly */
int SFPL2_Pause(void *handle, int op) {
    int ret = 0;

    if (op == 2) {
        if (*(u32 *)((u8 *)handle + P_STATE) == 4) {
            u32 substate = *(u32 *)((u8 *)handle + P_SUBSTATE);
            if (substate == 3 || substate == 4) {
                SFTIM_Pause(handle, 2);
                {
                    int data = 2;
                    int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
                    if (result) ret = result;
                    else ret = 0;
                }
            }
        }
    } else if (op == 1) {
        u32 ct = *(u32 *)((u8 *)handle + P_PAUSECT);
        if (ct == 0) {
            *(u32 *)((u8 *)handle + P_PAUSECT) = ct + 1;
            if (*(u32 *)((u8 *)handle + P_STATE) == 4) {
                u32 substate = *(u32 *)((u8 *)handle + P_SUBSTATE);
                if (substate == 3 || substate == 4) {
                    SFTIM_Pause(handle, 1);
                    {
                        int data = 1;
                        int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
                        if (result) ret = result;
                        else ret = 0;
                    }
                }
            }
        }
    } else if (op == 0) {
        u32 ct = *(u32 *)((u8 *)handle + P_PAUSECT);
        if (ct != 0) {
            *(u32 *)((u8 *)handle + P_PAUSECT) = ct - 1;
            if (*(u32 *)((u8 *)handle + P_STATE) == 4) {
                u32 substate = *(u32 *)((u8 *)handle + P_SUBSTATE);
                if (substate == 3 || substate == 4) {
                    SFTIM_Pause(handle, 0);
                    {
                        int data = 0;
                        int result = SFTRN_CallTrtTrif(handle, 7, 8, &data, 0);
                        if (result) ret = result;
                        else ret = 0;
                    }
                }
            }
        }
    }

    return ret;
}

/* Enter standby state */
int SFD_Standby(void *handle) {
    int ret = 0;

    if (SFLIB_CheckHn(handle))
        return SFLIB_SetErr(NULL, 0xFF000143);

    fn_803CD484(handle);
    ((u32 *)handle)[P_SUBSTATE / 4] = 3;
    return ret;
}

/* Standby sub-operation */
int SFPL2_Standby(void *handle) {
    fn_803CD484(handle);
    *(u32 *)((u8 *)handle + P_SUBSTATE) = 3;
    return 0;
}

/* Set playback speed (rational numerator/denominator) */
int SFD_SetSpeedRational(void *handle, int num, int den) {
    if (SFLIB_CheckHn(handle))
        return SFLIB_SetErr(NULL, 0xFF000144);

    SFTIM_SetSpeed(handle, num, den);
    SFAOAP_SetSpeed(handle, num, den);
    return 0;
}
