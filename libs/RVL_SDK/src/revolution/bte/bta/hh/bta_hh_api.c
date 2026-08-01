/*
 * BTA HID Host API functions.
 *
 * These functions allocate a GKI message buffer, populate it with the
 * appropriate event code and parameters, and send it via bta_sys_sendmsg
 * to the BTA HH task for processing.
 */

#include "revolution/BTE/bta/include/bta_hh_api.h"
#include "revolution/BTE/bta/sys/bta_sys.h"
#include "revolution/BTE/gki/common/gki.h"
#include "revolution/BTE/bta/include/bd.h"
#include "revolution/BTE/include/bt_trace.h"
#include <string.h>

/* BTA HH internal event codes (top byte = BTA_ID_HH = 0x17) */
#define BTA_HH_API_ENABLE_EVT      0x170c
#define BTA_HH_API_DISABLE_EVT     0x170d
#define BTA_HH_API_OPEN_EVT        0x1700
#define BTA_HH_API_CLOSE_EVT       0x1701
#define BTA_HH_API_SEND_DATA_EVT   0x1708
#define BTA_HH_API_ADD_DEV_EVT     0x170a
#define BTA_HH_API_GET_ACL_QUEUE_INFO_EVT 0x170e

/* Sub-events for BTA_HH_API_ADD_DEV_EVT */
#define BTA_HH_ADD_DEV_SUB_EVT     0x000B
#define BTA_HH_REMOVE_DEV_SUB_EVT  0x000C

/* Registration table for bta_sys_register */
extern BOOLEAN bta_hh_hdl_event(BT_HDR *p_msg);

/* Registration descriptor */
static const tBTA_SYS_REG bta_hh_reg = {
    bta_hh_hdl_event,
    BTA_HhDisable
};

/*******************************************************************************
**
** Function         BTA_HhEnable
**
** Description      Enable HID host and register HID-Host with lower layers.
**
** Parameters       sec_mask  - security mask
**                  p_cback   - HH callback function
**
** Returns          void
**
*******************************************************************************/
void BTA_HhEnable(tBTA_SEC sec_mask, tBTA_HH_CBACK *p_cback)
{
    BT_HDR *p_buf;

    GKI_sched_lock();

    bta_sys_register(BTA_ID_HH, &bta_hh_reg);

    GKI_sched_unlock();

    p_buf = (BT_HDR *)GKI_getbuf(0x34);
    if (p_buf != NULL) {
        UINT8 *p;

        memset(p_buf, 0, 0x34);

        p_buf->event = BTA_HH_API_ENABLE_EVT;
        p = (UINT8 *)p_buf;

        /* offset 0x30: p_cback callback pointer */
        *(tBTA_HH_CBACK **)(p + 0x30) = p_cback;
        /* offset 0x08: sec_mask */
        *(p + 0x08) = sec_mask;

        bta_sys_sendmsg(p_buf);
    }
}

/*******************************************************************************
**
** Function         BTA_HhDisable
**
** Description      Deregister HID host (called when the host is about to
**                  power down).
**
** Returns          void
**
*******************************************************************************/
void BTA_HhDisable(void)
{
    BT_HDR *p_buf;

    p_buf = (BT_HDR *)GKI_getbuf(sizeof(BT_HDR));
    if (p_buf != NULL) {
        p_buf->event = BTA_HH_API_DISABLE_EVT;
        bta_sys_sendmsg(p_buf);
    }
}

/*******************************************************************************
**
** Function         BTA_HhClose
**
** Description      Disconnect the device.
**
** Parameters       dev_handle - device handle
**
** Returns          void
**
*******************************************************************************/
void BTA_HhClose(UINT8 dev_handle)
{
    BT_HDR *p_buf;

    p_buf = (BT_HDR *)GKI_getbuf(sizeof(BT_HDR));
    if (p_buf != NULL) {
        memset(p_buf, 0, sizeof(BT_HDR));

        p_buf->event = BTA_HH_API_CLOSE_EVT;
        p_buf->layer_specific = dev_handle;

        bta_sys_sendmsg(p_buf);
    }
}

/*******************************************************************************
**
** Function         BTA_HhOpen
**
** Description      Open a connection to a HID device.
**
** Parameters       dev_bda  - BD address of the device
**                  mode     - protocol mode (report or boot)
**                  sec_mask - security mask
**
** Returns          void
**
*******************************************************************************/
void BTA_HhOpen(BD_ADDR dev_bda, tBTA_HH_PROTO_MODE mode, tBTA_SEC sec_mask)
{
    BT_HDR *p_buf;

    p_buf = (BT_HDR *)GKI_getbuf(0x10);
    if (p_buf != NULL) {
        UINT8 *p;

        memset(p_buf, 0, 0x10);

        p_buf->event = BTA_HH_API_OPEN_EVT;
        p = (UINT8 *)p_buf;

        /* offset 6: layer_specific = invalid handle */
        *(UINT16 *)(p + 6) = BTA_HH_INVALID_HANDLE;
        /* offset 0x0E: sec_mask */
        *(p + 0x0E) = sec_mask;
        /* offset 0x0F: mode */
        *(p + 0x0F) = mode;

        bdcpy(p + 8, dev_bda);

        bta_sys_sendmsg(p_buf);
    } else {
        if (appl_trace_level >= BT_TRACE_LEVEL_ERROR) {
            LogMsg_0(TRACE_CTRL_GENERAL | TRACE_LAYER_NONE | TRACE_ORG_APPL | TRACE_TYPE_ERROR,
                     "No resource to send HID host Connect request.");
        }
    }
}

/*******************************************************************************
**
** Function         BTA_HhSendData
**
** Description      Send a data report to a HID device.
**
** Parameters       dev_handle - device handle
**                  p_buf      - pointer to data buffer (BT_HDR)
**
** Returns          void
**
*******************************************************************************/
void BTA_HhSendData(UINT8 dev_handle, BT_HDR *p_buf)
{
    BT_HDR *p_msg;
    UINT8 *p;

    p_msg = (BT_HDR *)GKI_getbuf(0x14);
    if (p_msg != NULL) {
        memset(p_msg, 0, 0x14);

        p_msg->event = BTA_HH_API_SEND_DATA_EVT;
        p = (UINT8 *)p_msg;

        /* offset 6: layer_specific = dev_handle */
        *(UINT16 *)(p + 6) = dev_handle;
        /* offset 8: t_type = HID_TRANS_DATA (10) */
        *(p + 8) = HID_TRANS_DATA;
        /* offset 0x0C: hdr_param = 0 */
        *(UINT16 *)(p + 0x0C) = 0;
        /* offset 9: report_type = HID_PAR_REP_TYPE_OUTPUT (2) */
        *(p + 9) = HID_PAR_REP_TYPE_OUTPUT;
        /* offset 0x10: p_data = p_buf */
        *(BT_HDR **)(p + 0x10) = p_buf;
        /* offset 0x0A: report_id = 0 */
        *(p + 0x0A) = 0;

        bta_sys_sendmsg(p_msg);
    }
}

/*******************************************************************************
**
** Function         BTA_HhAddDev
**
** Description      Add a virtually cabled device into the HID host device list.
**
** Parameters       bda       - BD address
**                  attr_mask - attribute mask
**                  sub_class - device subclass
**                  app_id    - application ID
**                  dscp_info - device descriptor info
**
** Returns          void
**
*******************************************************************************/
void BTA_HhAddDev(BD_ADDR bda, tBTA_HH_ATTR_MASK attr_mask, UINT8 sub_class,
                  UINT8 app_id, tBTA_HH_DEV_DESCR dscp_info)
{
    BT_HDR *p_msg;
    UINT8 *p;

    p_msg = (BT_HDR *)GKI_getbuf(0x1C);
    if (p_msg != NULL) {
        memset(p_msg, 0, 0x1C);

        p_msg->event = BTA_HH_API_ADD_DEV_EVT;
        p = (UINT8 *)p_msg;

        /* offset 0x10: sub_event */
        *(UINT16 *)(p + 0x10) = BTA_HH_ADD_DEV_SUB_EVT;
        /* offset 6: layer_specific = invalid handle */
        *(UINT16 *)(p + 6) = BTA_HH_INVALID_HANDLE;
        /* offset 0x0E: attr_mask */
        *(UINT16 *)(p + 0x0E) = attr_mask;
        /* offset 0x12: sub_class */
        *(p + 0x12) = sub_class;
        /* offset 0x13: app_id */
        *(p + 0x13) = app_id;
        /* offset 0x14: dscp_info ({UINT16 dl_len, UINT8 *dsc_list}) */
        memcpy(p + 0x14, &dscp_info, sizeof(tBTA_HH_DEV_DESCR));

        bdcpy(p + 8, bda);

        bta_sys_sendmsg(p_msg);
    }
}

/*******************************************************************************
**
** Function         BTA_HhRemoveDev
**
** Description      Remove a device from the HID host devices list.
**
** Parameters       dev_handle - device handle
**
** Returns          void
**
*******************************************************************************/
void BTA_HhRemoveDev(UINT8 dev_handle)
{
    BT_HDR *p_buf;
    UINT8 *p;

    p_buf = (BT_HDR *)GKI_getbuf(0x1C);
    if (p_buf != NULL) {
        memset(p_buf, 0, 0x1C);

        p_buf->event = BTA_HH_API_ADD_DEV_EVT;
        p = (UINT8 *)p_buf;

        /* offset 0x10: sub_event = remove dev */
        *(UINT16 *)(p + 0x10) = BTA_HH_REMOVE_DEV_SUB_EVT;
        /* offset 6: layer_specific = dev_handle */
        *(UINT16 *)(p + 6) = dev_handle;

        bta_sys_sendmsg(p_buf);
    }
}

/*******************************************************************************
**
** Function         BTA_HhGetAclQueueInfo
**
** Description      Get ACL queue information.
**
** Returns          void
**
*******************************************************************************/
void BTA_HhGetAclQueueInfo(void)
{
    BT_HDR *p_buf;

    p_buf = (BT_HDR *)GKI_getbuf(sizeof(BT_HDR));
    if (p_buf != NULL) {
        memset(p_buf, 0, sizeof(BT_HDR));

        p_buf->event = BTA_HH_API_GET_ACL_QUEUE_INFO_EVT;

        bta_sys_sendmsg(p_buf);
    }
}
