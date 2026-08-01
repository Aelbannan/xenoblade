// Decompiled from RVL_SDK/src/revolution/bte/stack/hid/hidd_pm
// Power-management functions for the HID Device role.

#include <string.h>
#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/stack/include/btm_api.h>
#include <revolution/BTE/stack/include/btu.h>

// ------------------------------------------------------------------
// hd_cb - HID device control block (BSS, size 0x144).
// Partial layout covering only the power-management fields.
// ------------------------------------------------------------------
typedef void (*tHIDD_APP_CBACK)(UINT32 event, UINT32 data, UINT32 param);

typedef struct
{
    UINT8           reserved_bda[6];      // +0x00: BD_ADDR of peer
    UINT8           _pad0[0x2E];          // +0x06
    TIMER_LIST_ENT  inact_timer;           // +0x34: inactivity timer entry
    UINT8           saved_pm[10];          // +0x4C: saved PM params (copy)
    UINT8           inact_pm[10];          // +0x56: inact timeout PM params
    UINT8           _pad1[0x6A - 0x60];   // +0x60
    UINT8           current_mode;          // +0x6A: current PM mode
    UINT8           _pad2;                 // +0x6B
    UINT16          current_interval;      // +0x6C: current PM interval
    UINT8           pm_set;                // +0x6E: power-mode-set flag
    UINT8           inact_flag;            // +0x6F: inactivity flag
    tBTM_PM_PWR_MD  pm_config;             // +0x70: desired PM config (10B)
    UINT8           _pad3[0xC4 - 0x7A];   // +0x7A
    tHIDD_APP_CBACK app_cback;             // +0xC4: application callback
    UINT8           _pad4[0x140 - 0xC8];  // +0xC8
    UINT8           registered;            // +0x140: HID registered flag
    UINT8           trace_level;           // +0x141
    UINT8           _pad5[0x02];           // +0x142..0x143
} tHIDD_CB;

extern tHIDD_CB hd_cb;
extern void hidd_pm_inact_timeout(void *p_tle);

static int hidd_pm_set_now(tBTM_PM_PWR_MD *p_params);

/******************************************************************************
 * hidd_pm_set_now
 ******************************************************************************/
static int hidd_pm_set_now(tBTM_PM_PWR_MD *p_params)
{
    UINT8 status;
    UINT8 new_mode;
    UINT8 cur_mode;
    UINT16 cur_interval;
    tBTM_PM_PWR_MD zero_md = {0};

    status = 0;

    if (hd_cb.pm_set != 0)
        goto accept;

    cur_mode = hd_cb.current_mode;
    new_mode = p_params->mode;

    if (new_mode != cur_mode)
        goto process;

    if (new_mode == 0)
        goto accept;

    cur_interval = hd_cb.current_interval;
    if (cur_interval < p_params->min)
        goto process;
    if (cur_interval > p_params->max)
        goto process;

accept:
    hd_cb.pm_config.mode = 0xFF;
    return 1;

process:
    switch (new_mode)
    {
    case 0:
        if (cur_mode == 2)
        {
            status = BTM_SetPowerMode(BTM_PM_SET_ONLY_ID, hd_cb.reserved_bda, &zero_md);
            hd_cb.pm_set = 1;
        }
        else if (cur_mode == 3)
        {
            status = BTM_SetPowerMode(BTM_PM_SET_ONLY_ID, hd_cb.reserved_bda, &zero_md);
            hd_cb.pm_set = 1;
        }
        break;

    case 2:
        if (cur_mode != 0)
        {
            hidd_pm_set_now(&zero_md);
        }
        else
        {
            status = BTM_SetPowerMode(BTM_PM_SET_ONLY_ID, hd_cb.reserved_bda, p_params);
            hd_cb.pm_set = 1;
        }
        break;

    case 3:
        if (cur_mode != 0)
        {
            hidd_pm_set_now(&zero_md);
        }
        else
        {
            status = BTM_SetPowerMode(BTM_PM_SET_ONLY_ID, hd_cb.reserved_bda, p_params);
            hd_cb.pm_set = 1;
        }
        break;
    }

    if (status <= 1)
        return 1;

    status += 0x37;
    if (hd_cb.app_cback != NULL)
        hd_cb.app_cback(4, hd_cb.inact_flag, (UINT32)&status);

    return 0;
}

/******************************************************************************
 * hidd_pm_proc_mode_change
 ******************************************************************************/
void hidd_pm_proc_mode_change(UINT8 hci_status, UINT8 mode, UINT16 interval)
{
    if (hd_cb.registered == 0)
        return;

    hd_cb.pm_set = 0;

    if (hci_status != 0)
    {
        if (hd_cb.app_cback != NULL)
            hd_cb.app_cback(4, hd_cb.inact_flag, (UINT32)&hci_status);
        return;
    }

    hd_cb.current_mode    = mode;
    hd_cb.current_interval = interval;

    if (hd_cb.pm_config.mode != 0xFF)
    {
        if (hd_cb.pm_config.mode != mode)
            goto process;
        if (hd_cb.pm_config.mode == 0)
            goto accept;
        if (interval < hd_cb.pm_config.min)
            goto process;
        if (interval <= hd_cb.pm_config.max)
            goto accept;

    process:
        hidd_pm_set_now(&hd_cb.pm_config);
        goto done;

    accept:
        hd_cb.pm_config.mode = 0xFF;
        goto done;
    }
    else
    {
        if (mode == 0)
        {
            memcpy(&hd_cb.pm_config, hd_cb.saved_pm, sizeof(hd_cb.pm_config));
            hidd_pm_set_now((tBTM_PM_PWR_MD *)hd_cb.saved_pm);
            hd_cb.inact_flag = 0;
            hd_cb.inact_timer.param = (TIMER_PARAM_TYPE)&hidd_pm_inact_timeout;
            btu_start_timer(&hd_cb.inact_timer, 0x16, 0x3C);
        }
    }

done:
    if (hd_cb.app_cback != NULL)
        hd_cb.app_cback(3, mode, (UINT32)&interval);
}

/******************************************************************************
 * hidd_pm_inact_timeout
 ******************************************************************************/
void hidd_pm_inact_timeout(void *p_tle)
{
    (void)p_tle;

    memcpy(&hd_cb.pm_config, hd_cb.inact_pm, sizeof(hd_cb.pm_config));
    hidd_pm_set_now((tBTM_PM_PWR_MD *)hd_cb.inact_pm);
    hd_cb.inact_flag = 1;
}
