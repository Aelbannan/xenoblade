#include <string.h>

#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/include/bt_trace.h>
#include <revolution/BTE/stack/include/l2c_api.h>
#include <revolution/BTE/stack/include/hiddefs.h>

/* HID Device control block - defined in hidd_mgmt.c */
/* Layout reconstructed from retail ASM offsets */
typedef struct
{
    UINT8   pad0[0x2C];             /* +0x00 */
    UINT32  timer_param;            /* +0x2C: conn.timer_entry.param */
    UINT8   pad1[0x9A];             /* +0x30 */
    UINT8   ctrl_mtu_present;       /* +0xCA: l2cap_ctrl_cfg.mtu_present */
    UINT8   pad2;                   /* +0xCB */
    UINT16  ctrl_mtu;               /* +0xCC: l2cap_ctrl_cfg.mtu */
    UINT8   pad3[0x38];             /* +0xCE */
    UINT8   intr_mtu_present;       /* +0x106: l2cap_int_cfg.mtu_present */
    UINT8   pad4;                   /* +0x107 */
    UINT16  intr_mtu;               /* +0x108: l2cap_int_cfg.mtu */
    UINT8   pad5[0x37];             /* +0x10A */
    UINT8   trace_level;            /* +0x141 */
    UINT8   pad6[0x02];             /* +0x142 */
} tHIDD_CB;

extern tHIDD_CB hd_cb;
extern void hidd_proc_repage_timeout(void);

/*******************************************************************************
**
** Function         HID_DevInit
**
** Description      Initialize the HID device control block
**
** Returns          void
**
*******************************************************************************/
void HID_DevInit(void)
{
    memset(&hd_cb, 0, sizeof(hd_cb));

    hd_cb.ctrl_mtu_present = TRUE;
    hd_cb.ctrl_mtu = 64;

    hd_cb.intr_mtu_present = TRUE;
    hd_cb.intr_mtu = 64;

    hd_cb.timer_param = (UINT32)&hidd_proc_repage_timeout;

    hd_cb.trace_level = BT_TRACE_LEVEL_NONE;
}
