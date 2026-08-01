/* HID host connection management (hidh_conn.c) -- RVL_SDK bte stack.
 * Reconstructed from the retail binary; the hh_cb control block lives in
 * hidh_api.c.  All L2CAP/HID-layer callbacks and connection state machine
 * code for the HID-Host role. */

/* bt_target.h defaults to 7; retail binary uses 16 */
#define HID_HOST_MAX_DEVICES 16

#include <harness_catalog.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/stack/include/hiddefs.h>
#include <revolution/BTE/stack/include/hcidefs.h>

/* ------------------------------------------------------------------ */
/*  BTE type helpers.                                                  */
/* ------------------------------------------------------------------ */

/* Trace level thresholds (bt_trace.h) */
#define BT_TRACE_LEVEL_ERROR   1
#define BT_TRACE_LEVEL_WARNING 2
#define BT_TRACE_LEVEL_EVENT   4

/* BTM security (btm_api.h / btm_int.h) */
#define BTM_SUCCESS        0
#define BTM_SEC_PROTO_HID  6

/* L2CAP connection response result codes (l2c_api.h) */
#define L2CAP_CONN_NO_RESOURCES     3
#define L2CAP_CONN_NO_PSM_ALLOWED   4

/* HID attribute mask (hidh_api.h) */
#define HID_VIRTUAL_CABLE           0x0001
#define HID_SEC_REQUIRED            0x8000

/* HID-Host events delivered via dev_cback (hidh_api.h) */
enum
{
    HID_HDEV_EVT_OPEN,
    HID_HDEV_EVT_CLOSE,
    HID_HDEV_EVT_RETRYING,
    HID_HDEV_EVT_INTR_DATA,
    HID_HDEV_EVT_INTR_DATC,
    HID_HDEV_EVT_CTRL_DATA,
    HID_HDEV_EVT_CTRL_DATC,
    HID_HDEV_EVT_HANDSHAKE,
    HID_HDEV_EVT_VC_UNPLUG
};

/* Device state (dev->state) */
enum
{
    HID_DEV_NO_CONN,
    HID_DEV_CONNECTED
};

/* ------------------------------------------------------------------ */
/*  External functions (declared locally -- bt_trace.h / l2c_api.h /   */
/*  btm_int.h / gki.h have unmet dependencies in this TU).            */
/* ------------------------------------------------------------------ */

extern void LogMsg_0(UINT32 trace_set_mask, const char *p_str);
extern void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void LogMsg_2(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1,
                     UINT32 p2);

extern int memcmp(const void *s1, const void *s2, size_t n);
extern void *memcpy(void *dst, const void *src, size_t n);

extern UINT8 L2CA_Register(UINT16 psm, void *p_cb_info);
extern void L2CA_Deregister(UINT16 psm);
extern UINT16 L2CA_ConnectReq(UINT16 psm, BD_ADDR p_bd_addr);
extern UINT8 L2CA_ConnectRsp(BD_ADDR p_bd_addr, UINT8 id, UINT16 lcid,
                             UINT16 result, UINT16 status);
extern UINT8 L2CA_ConfigReq(UINT16 cid, void *p_cfg);
extern UINT8 L2CA_ConfigRsp(UINT16 cid, void *p_cfg);
extern void L2CA_DisconnectReq(UINT16 cid);
extern UINT8 L2CA_DisconnectRsp(UINT16 cid);
extern UINT8 L2CA_DataWrite(UINT16 cid, void *p_data);

extern int btm_sec_mx_access_request(BD_ADDR bd_addr, UINT16 psm,
                                     UINT8 is_originator, UINT32 mx_proto_id,
                                     UINT32 mx_chan_id, void *p_callback,
                                     void *p_ref_data);
extern UINT16 btm_get_acl_disc_reason_code(void);

extern void *GKI_getpoolbuf(UINT8 pool_id);
extern void GKI_freebuf(void *p_buf);

/* ------------------------------------------------------------------ */
/*  HID-Host connection structures (mirror hidh_int.h / hidh_api.c).  */
/* ------------------------------------------------------------------ */

/* Timer list entry (gki.h) */
typedef struct _tle
{
    struct _tle *p_next;   /* +0x00 */
    struct _tle *p_prev;   /* +0x04 */
    void *p_cback;         /* +0x08 */
    INT32 ticks;           /* +0x0C */
    UINT32 param;          /* +0x10 */
    UINT16 event;          /* +0x14 */
    UINT8 in_use;          /* +0x16 */
} TIMER_LIST_ENT;

/* Connection state for one L2CAP channel pair */
typedef struct
{
    UINT8       conn_state;      /* +0x00 */
    UINT8       conn_flags;      /* +0x01 */
    UINT8       ctrl_id;         /* +0x02 */
    UINT8       pad_03;          /* +0x03 */
    UINT16      ctrl_cid;        /* +0x04 */
    UINT16      intr_cid;        /* +0x06 */
    UINT16      rem_mtu_size;    /* +0x08 */
    UINT16      disc_reason;     /* +0x0A */
    UINT8       pad_0C[0x18];    /* +0x0C (timer entry) */
} tHID_CONN;

/* conn_state values */
enum
{
    HID_CONN_STATE_UNUSED = 0,
    HID_CONN_STATE_CONNECTING,
    HID_CONN_STATE_CONNECTED,    /* control channel established */
    HID_CONN_STATE_CONFIG,       /* configuring the interrupt channel */
    HID_CONN_STATE_OPENED,       /* fully configured */
    HID_CONN_STATE_DISCONNECTING,
    HID_CONN_STATE_SECURITY      /* waiting on security procedures */
};

/* conn_flags bits */
#define HID_CONN_FLAG_IS_ORIG       0x01
#define HID_CONN_FLAG_CTL_CFG_IND   0x02
#define HID_CONN_FLAG_CTL_CFG_CFM   0x04
#define HID_CONN_FLAG_INTR_CFG_IND  0x08
#define HID_CONN_FLAG_INTR_CFG_CFM  0x10
#define HID_CONN_FLAG_CONGESTED     0x20
#define HID_CONN_FLAG_ALL_CONFIG    0x1E

typedef void (tHID_HOST_DEV_CALLBACK)(UINT8 dev_handle, UINT8 event,
                                      UINT32 data, BT_HDR *p_buf);

/* Per-device control block (0x34 bytes) */
typedef struct
{
    UINT8       in_use;          /* +0x00 */
    BD_ADDR     addr;            /* +0x01 */
    UINT8       pad_07;          /* +0x07 */
    UINT16      attr_mask;       /* +0x08 */
    UINT8       state;           /* +0x0A */
    UINT8       conn_substate;   /* +0x0B */
    UINT8       conn_tries;      /* +0x0C */
    UINT8       pad_0D[3];       /* +0x0D */
    tHID_CONN   conn;            /* +0x10 */
} tHID_HOST_DEV_CTB;

/* L2CAP configuration parameters (tL2CAP_CFG_INFO layout) */
typedef struct
{
    UINT16      result;              /* +0x00 */
    UINT8       mtu_present;         /* +0x02 */
    UINT16      mtu;                 /* +0x04 */
    UINT8       qos_present;         /* +0x06 */
    FLOW_SPEC   qos;                 /* +0x08 */
    UINT8       flush_to_present;    /* +0x20 */
    UINT16      flush_to;            /* +0x22 */
    UINT8       fcr_present;         /* +0x24 */
    UINT8       fcr[10];             /* +0x26 */
    UINT8       fcs_present;         /* +0x30 */
    UINT8       fcs;                 /* +0x31 */
    UINT8       ext_flow_spec_present; /* +0x32 */
    UINT8       ext_flow_spec[16];   /* +0x34 */
    UINT16      flags;               /* +0x44 */
    UINT8       pad_46[0xBC - 0x46]; /* +0x46 .. 0xBC */
} tL2CAP_CFG_INFO;

/* HID host control block -- mirrors hidh_api.c, 0x404 bytes total */
typedef struct
{
    tHID_HOST_DEV_CTB devices[HID_HOST_MAX_DEVICES]; /* +0x000 */
    tHID_HOST_DEV_CALLBACK *dev_cback;               /* +0x340 */
    tL2CAP_CFG_INFO l2cap_cfg;                       /* +0x344 */
    UINT8       reg_flag;                            /* +0x400 */
    UINT8       trace_level;                         /* +0x401 */
    UINT8       pad_402[2];                          /* +0x402 */
} tHID_HOST_CTB;

extern tHID_HOST_CTB hh_cb;

/* L2CAP registration info (tL2CAP_APPL_INFO, 10 entries) */
typedef struct
{
    void *pL2CA_ConnectInd_Cb;
    void *pL2CA_ConnectCfm_Cb;
    void *pL2CA_ConnectPnd_Cb;
    void *pL2CA_ConfigInd_Cb;
    void *pL2CA_ConfigCfm_Cb;
    void *pL2CA_DisconnectInd_Cb;
    void *pL2CA_DisconnectCfm_Cb;
    void *pL2CA_QoSViolationInd_Cb;
    void *pL2CA_DataInd_Cb;
    void *pL2CA_CongestionStatus_Cb;
} tL2CAP_APPL_INFO;

/* HID data buffer access (BT_HDR has no data[] member in bt_types.h) */
#define HID_BUF_DATA(p) ((UINT8 *)((BT_HDR *)(p) + 1))

/* ------------------------------------------------------------------ */
/*  Forward declarations.                                             */
/* ------------------------------------------------------------------ */

void hidh_l2cif_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm,
                            UINT8 l2cap_id);
void hidh_l2cif_connect_cfm(UINT16 l2cap_cid, UINT16 result);
void hidh_l2cif_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO *p_cfg);
void hidh_l2cif_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO *p_cfg);
void hidh_l2cif_disconnect_ind(UINT16 l2cap_cid, UINT8 ack_needed);
void hidh_l2cif_disconnect_cfm(UINT16 l2cap_cid, UINT16 result);
void hidh_l2cif_cong_ind(UINT16 l2cap_cid, UINT8 is_congested);
void hidh_l2cif_data_ind(UINT16 l2cap_cid, BT_HDR *p_buf);
void hidh_sec_check_complete_orig(BD_ADDR addr, void *p_ref_data, UINT8 result);
void hidh_sec_check_complete_term(BD_ADDR addr, void *p_ref_data, UINT8 result);
void hidh_proc_repage_timeout(TIMER_LIST_ENT *p_tle);
tHID_STATUS hidh_conn_disconnect(UINT8 dhandle);
tHID_STATUS hidh_conn_initiate(UINT8 dhandle);
tHID_STATUS hidh_conn_snd_data(UINT8 dhandle, UINT8 trans_type, UINT8 param,
                               UINT16 data, UINT8 report_id, BT_HDR *buf);

static const tL2CAP_APPL_INFO hst_reg_info = {
    (void *)hidh_l2cif_connect_ind,
    (void *)hidh_l2cif_connect_cfm,
    NULL,
    (void *)hidh_l2cif_config_ind,
    (void *)hidh_l2cif_config_cfm,
    (void *)hidh_l2cif_disconnect_ind,
    (void *)hidh_l2cif_disconnect_cfm,
    NULL,
    (void *)hidh_l2cif_data_ind,
    (void *)hidh_l2cif_cong_ind
};

/*******************************************************************************
**
** Function         find_conn_by_cid
**
** Description      This function finds a connection control block based on CID
**
** Returns          index of the control block, or HID_HOST_MAX_DEVICES if not
**                  found
**
*******************************************************************************/
static UINT8 find_conn_by_cid(UINT16 cid)
{
    UINT8 xx;

    for (xx = 0; xx < HID_HOST_MAX_DEVICES; xx++)
    {
        if (hh_cb.devices[xx].in_use &&
            hh_cb.devices[xx].conn.conn_state != HID_CONN_STATE_UNUSED &&
            ((hh_cb.devices[xx].conn.ctrl_cid == cid) ||
             (hh_cb.devices[xx].conn.intr_cid == cid)))
        {
            break;
        }
    }

    return (xx);
}

/*******************************************************************************
**
** Function         hidh_conn_reg
**
** Description      Registers HID Host with L2CAP
**
** Returns          tHID_STATUS
**
*******************************************************************************/
tHID_STATUS hidh_conn_reg(void)
{
    int xx;

    hh_cb.l2cap_cfg.mtu_present = TRUE;
    hh_cb.l2cap_cfg.mtu = HID_HOST_MTU;
    hh_cb.l2cap_cfg.flush_to_present = TRUE;
    hh_cb.l2cap_cfg.flush_to = HID_HOST_FLUSH_TO;

    if (!L2CA_Register(HID_PSM_CONTROL, (void *)&hst_reg_info))
    {
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_ERROR)
        {
            LogMsg_0(0x001e0000, "HID Control Registration failed");
        }
        return (HID_ERR_L2CAP_FAILED);
    }

    if (!L2CA_Register(HID_PSM_INTERRUPT, (void *)&hst_reg_info))
    {
        L2CA_Deregister(HID_PSM_CONTROL);
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_ERROR)
        {
            LogMsg_0(0x001e0000, "HID Interrupt Registration failed");
        }
        return (HID_ERR_L2CAP_FAILED);
    }

    for (xx = 0; xx < HID_HOST_MAX_DEVICES; xx++)
    {
        hh_cb.devices[xx].in_use = FALSE;
        hh_cb.devices[xx].conn.conn_state = HID_CONN_STATE_UNUSED;
    }

    return (HID_SUCCESS);
}

/*******************************************************************************
**
** Function         hidh_conn_disconnect
**
** Description      This function disconnects the device.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
tHID_STATUS hidh_conn_disconnect(UINT8 dhandle)
{
    tHID_CONN *p_hcon = &hh_cb.devices[dhandle].conn;

    if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_0(0x001e0003, "HID - disconnect");
    }

    if ((p_hcon->ctrl_cid != 0) || (p_hcon->intr_cid != 0))
    {
        p_hcon->conn_state = HID_CONN_STATE_DISCONNECTING;

        if (p_hcon->intr_cid)
        {
            L2CA_DisconnectReq(p_hcon->intr_cid);
        }

        if (p_hcon->ctrl_cid)
        {
            L2CA_DisconnectReq(p_hcon->ctrl_cid);
        }
    }
    else
    {
        p_hcon->conn_state = HID_CONN_STATE_UNUSED;
    }

    return (HID_SUCCESS);
}

/*******************************************************************************
**
** Function         hidh_sec_check_complete_term
**
** Description      This function is called when security check for
**                  termination is complete.
**
*******************************************************************************/
void hidh_sec_check_complete_term(BD_ADDR addr, void *p_ref_data, UINT8 res)
{
    tHID_HOST_DEV_CTB *p_dev = (tHID_HOST_DEV_CTB *)p_ref_data;
    tHID_HOST_DEV_CTB *p_dev2;
    UINT8 i = (UINT8)(p_dev - hh_cb.devices);

    if (res == BTM_SUCCESS && p_dev->conn.conn_state == HID_CONN_STATE_SECURITY)
    {
        p_dev->conn.disc_reason = 0;
        p_dev->conn.conn_state = HID_CONN_STATE_CONNECTED;
        L2CA_ConnectRsp(p_dev->addr, p_dev->conn.ctrl_id, p_dev->conn.ctrl_cid,
                        0, 0);
        L2CA_ConfigReq(p_dev->conn.ctrl_cid, (void *)&hh_cb.l2cap_cfg);
    }
    else if (res != BTM_SUCCESS)
    {
        p_dev->conn.disc_reason = HID_ERR_AUTH_FAILED;
        p_dev2 = &hh_cb.devices[i];
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        {
            LogMsg_0(0x001e0003, "HID - disconnect");
        }
        if ((p_dev2->conn.ctrl_cid != 0) || (p_dev2->conn.intr_cid != 0))
        {
            p_dev2->conn.conn_state = HID_CONN_STATE_DISCONNECTING;
            if (p_dev2->conn.intr_cid)
            {
                L2CA_DisconnectReq(p_dev2->conn.intr_cid);
            }
            if (p_dev2->conn.ctrl_cid)
            {
                L2CA_DisconnectReq(p_dev2->conn.ctrl_cid);
            }
        }
        else
        {
            p_dev2->conn.conn_state = HID_CONN_STATE_UNUSED;
        }
    }
}

/*******************************************************************************
**
** Function         hidh_l2cif_connect_ind
**
** Description      This function handles an L2CAP connect indication event.
**
*******************************************************************************/
void hidh_l2cif_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm,
                            UINT8 l2cap_id)
{
    tHID_CONN *p_hcon;
    BOOLEAN bAccept = TRUE;
    int i;
    tHID_HOST_DEV_CTB *p_dev;

    if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_2(0x001e0003, "HID - Rcvd L2CAP conn ind, PSM: 0x%04x  CID 0x%x",
                 psm, l2cap_cid);
    }

    for (i = 0; i < HID_HOST_MAX_DEVICES; i++)
    {
        if (hh_cb.devices[i].in_use &&
            (!memcmp(bd_addr, hh_cb.devices[i].addr, sizeof(BD_ADDR))))
        {
            break;
        }
    }

    if (i == HID_HOST_MAX_DEVICES)
    {
        L2CA_ConnectRsp(bd_addr, l2cap_id, l2cap_cid,
                        L2CAP_CONN_NO_RESOURCES, 0);
        return;
    }

    p_hcon = &hh_cb.devices[i].conn;
    p_dev = &hh_cb.devices[i];

    if (psm == HID_PSM_INTERRUPT)
    {
        if (p_hcon->ctrl_cid == 0)
        {
            if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
            {
                LogMsg_0(0x001e0001,
                         "HID - Rcvd INTR L2CAP conn ind, but no CTL channel");
            }
            bAccept = FALSE;
        }
        if (p_hcon->conn_state != HID_CONN_STATE_CONNECTED)
        {
            if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
            {
                LogMsg_1(0x001e0001,
                         "HID - Rcvd INTR L2CAP conn ind, wrong state: %d",
                         p_hcon->conn_state);
            }
            bAccept = FALSE;
        }
    }
    else
    {
        if (p_hcon->conn_state != HID_CONN_STATE_UNUSED)
        {
            if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
            {
                LogMsg_1(0x001e0001,
                         "HID - Rcvd CTL L2CAP conn ind, wrong state: %d",
                         p_hcon->conn_state);
            }
            bAccept = FALSE;
        }
    }

    if (!bAccept)
    {
        L2CA_ConnectRsp(bd_addr, l2cap_id, l2cap_cid,
                        L2CAP_CONN_NO_PSM_ALLOWED, 0);
        return;
    }

    if (psm == HID_PSM_CONTROL)
    {
        UINT32 mx_chan_id;

        p_hcon->conn_flags = 0;
        p_hcon->ctrl_cid = l2cap_cid;
        p_hcon->ctrl_id = l2cap_id;
        p_hcon->disc_reason = HID_L2CAP_CONN_FAIL;
        p_hcon->conn_state = HID_CONN_STATE_SECURITY;
        mx_chan_id = -((p_dev->attr_mask & HID_SEC_REQUIRED) >> 15);
        btm_sec_mx_access_request(p_dev->addr, HID_PSM_CONTROL, FALSE,
                                  BTM_SEC_PROTO_HID, mx_chan_id + 2,
                                  (void *)hidh_sec_check_complete_term, p_dev);
        return;
    }

    p_hcon->conn_state = HID_CONN_STATE_CONFIG;
    p_hcon->intr_cid = l2cap_cid;

    L2CA_ConnectRsp(bd_addr, l2cap_id, l2cap_cid, 0, 0);

    L2CA_ConfigReq(l2cap_cid, (void *)&hh_cb.l2cap_cfg);

    if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_2(0x001e0003,
                 "HID - Rcvd L2CAP conn ind, sent config req, PSM: 0x%04x  CID 0x%x",
                 psm, l2cap_cid);
    }
}

/*******************************************************************************
**
** Function         hidh_proc_repage_timeout
**
** Description      This function is called when the repage timer expires.
**
*******************************************************************************/
void hidh_proc_repage_timeout(TIMER_LIST_ENT *p_tle)
{
    hidh_conn_initiate((UINT8)p_tle->param);
    hh_cb.devices[p_tle->param].conn_tries++;
    hh_cb.dev_cback((UINT8)p_tle->param, HID_HDEV_EVT_RETRYING,
                    hh_cb.devices[p_tle->param].conn_tries, NULL);
}

/*******************************************************************************
**
** Function         hidh_sec_check_complete_orig
**
** Description      This function is called when security check for originator
**                  is complete.
**
*******************************************************************************/
void hidh_sec_check_complete_orig(BD_ADDR addr, void *p_ref_data, UINT8 result)
{
    tHID_HOST_DEV_CTB *p_dev = (tHID_HOST_DEV_CTB *)p_ref_data;
    tHID_HOST_DEV_CTB *p_dev2;
    UINT8 i = (UINT8)(p_dev - hh_cb.devices);

    if (result == BTM_SUCCESS && p_dev->conn.conn_state == HID_CONN_STATE_SECURITY)
    {
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        {
            LogMsg_0(0x001e0003, "HID - Originator security pass.");
        }
        p_dev->conn.disc_reason = 0;
        p_dev2 = &hh_cb.devices[i];
        p_dev->conn.intr_cid = L2CA_ConnectReq(HID_PSM_INTERRUPT, p_dev2->addr);
        if (p_dev->conn.intr_cid == 0)
        {
            if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
            {
                LogMsg_0(0x001e0001, "HID - INTR Originate failed");
            }
            if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
            {
                LogMsg_0(0x001e0003, "HID - disconnect");
            }
            if ((p_dev2->conn.ctrl_cid != 0) || (p_dev2->conn.intr_cid != 0))
            {
                p_dev2->conn.conn_state = HID_CONN_STATE_DISCONNECTING;
                if (p_dev2->conn.intr_cid)
                {
                    L2CA_DisconnectReq(p_dev2->conn.intr_cid);
                }
                if (p_dev2->conn.ctrl_cid)
                {
                    L2CA_DisconnectReq(p_dev2->conn.ctrl_cid);
                }
            }
            else
            {
                p_dev2->conn.conn_state = HID_CONN_STATE_UNUSED;
            }
            hh_cb.dev_cback(i, HID_HDEV_EVT_CLOSE, HID_L2CAP_REQ_FAIL, NULL);
            return;
        }
        else
        {
            p_dev->conn.conn_state = HID_CONN_STATE_CONNECTED;
        }
    }
    if (result != BTM_SUCCESS && p_dev->conn.conn_state == HID_CONN_STATE_SECURITY)
    {
        p_dev->conn.disc_reason = HID_ERR_AUTH_FAILED;
        p_dev2 = &hh_cb.devices[i];
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        {
            LogMsg_0(0x001e0003, "HID - disconnect");
        }
        if ((p_dev2->conn.ctrl_cid != 0) || (p_dev2->conn.intr_cid != 0))
        {
            p_dev2->conn.conn_state = HID_CONN_STATE_DISCONNECTING;
            if (p_dev2->conn.intr_cid)
            {
                L2CA_DisconnectReq(p_dev2->conn.intr_cid);
            }
            if (p_dev2->conn.ctrl_cid)
            {
                L2CA_DisconnectReq(p_dev2->conn.ctrl_cid);
            }
        }
        else
        {
            p_dev2->conn.conn_state = HID_CONN_STATE_UNUSED;
        }
    }
}

/*******************************************************************************
**
** Function         hidh_l2cif_connect_cfm
**
** Description      This function handles an L2CAP connect confirm event.
**
*******************************************************************************/
void hidh_l2cif_connect_cfm(UINT16 l2cap_cid, UINT16 result)
{
    UINT8 dhandle;
    tHID_CONN *p_hcon = NULL;
    tHID_HOST_DEV_CTB *p_dev = NULL;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
    {
        p_dev = &hh_cb.devices[dhandle];
        p_hcon = &hh_cb.devices[dhandle].conn;
    }

    if ((p_hcon == NULL)
        || (!(p_hcon->conn_flags & HID_CONN_FLAG_IS_ORIG))
        || ((l2cap_cid == p_hcon->ctrl_cid) &&
            (p_hcon->conn_state != HID_CONN_STATE_CONNECTING))
        || ((l2cap_cid == p_hcon->intr_cid) &&
            (p_hcon->conn_state != HID_CONN_STATE_CONNECTED)))
    {
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
        {
            LogMsg_1(0x001e0001, "HID - Rcvd unexpected conn cnf, CID 0x%x ",
                     l2cap_cid);
        }
        return;
    }

    if (result != 0)
    {
        if (l2cap_cid == p_hcon->ctrl_cid)
        {
            p_hcon->ctrl_cid = 0;
        }
        else
        {
            p_hcon->intr_cid = 0;
        }

        hidh_conn_disconnect(dhandle);
        hh_cb.dev_cback(dhandle, HID_HDEV_EVT_CLOSE,
                        HID_L2CAP_CONN_FAIL | result, NULL);
        return;
    }

    if (l2cap_cid == p_hcon->ctrl_cid)
    {
        UINT32 mx_chan_id;

        p_hcon->conn_state = HID_CONN_STATE_SECURITY;
        p_hcon->disc_reason = HID_L2CAP_CONN_FAIL;
        mx_chan_id = -((p_dev->attr_mask & HID_SEC_REQUIRED) >> 15);
        btm_sec_mx_access_request(p_dev->addr, HID_PSM_CONTROL, TRUE,
                                  BTM_SEC_PROTO_HID, mx_chan_id + 2,
                                  (void *)hidh_sec_check_complete_orig, p_dev);
    }
    else
    {
        p_hcon->conn_state = HID_CONN_STATE_CONFIG;
    }

    L2CA_ConfigReq(l2cap_cid, (void *)&hh_cb.l2cap_cfg);
    if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_1(0x001e0003, "HID - got CTRL conn cnf, sent cfg req, CID: 0x%x",
                 l2cap_cid);
    }
}

/*******************************************************************************
**
** Function         hidh_l2cif_config_ind
**
** Description      This function handles an L2CAP config indication event.
**
*******************************************************************************/
void hidh_l2cif_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO *p_cfg)
{
    UINT8 dhandle;
    tHID_CONN *p_hcon = NULL;
    tHID_HOST_DEV_CTB *p_dev = NULL;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
    {
        p_dev = &hh_cb.devices[dhandle];
        p_hcon = &hh_cb.devices[dhandle].conn;
    }

    if (p_hcon == NULL)
    {
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
        {
            LogMsg_1(0x001e0001, "HID - Rcvd L2CAP cfg ind, unknown CID: 0x%x",
                     l2cap_cid);
        }
        return;
    }

    if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_1(0x001e0003, "HID - Rcvd cfg ind, sent cfg cfm, CID: 0x%x",
                 l2cap_cid);
    }

    if ((!p_cfg->mtu_present) || (p_cfg->mtu > HID_HOST_MTU))
    {
        p_hcon->rem_mtu_size = HID_HOST_MTU;
    }
    else
    {
        p_hcon->rem_mtu_size = p_cfg->mtu;
    }

    p_cfg->flush_to_present = FALSE;
    p_cfg->mtu_present = FALSE;
    p_cfg->result = 0;

    L2CA_ConfigRsp(l2cap_cid, (void *)p_cfg);

    if (l2cap_cid == p_hcon->ctrl_cid)
    {
        p_hcon->conn_flags |= HID_CONN_FLAG_CTL_CFG_IND;
    }
    else
    {
        p_hcon->conn_flags |= HID_CONN_FLAG_INTR_CFG_IND;
    }

    if (((p_hcon->conn_flags & HID_CONN_FLAG_ALL_CONFIG) ==
         HID_CONN_FLAG_ALL_CONFIG) &&
        (p_hcon->conn_state == HID_CONN_STATE_CONFIG))
    {
        p_hcon->conn_state = HID_CONN_STATE_OPENED;
        hh_cb.devices[dhandle].state = HID_DEV_CONNECTED;
        hh_cb.dev_cback(dhandle, HID_HDEV_EVT_OPEN, 0, NULL);
    }
}

/*******************************************************************************
**
** Function         hidh_l2cif_config_cfm
**
** Description      This function handles an L2CAP config confirm event.
**
*******************************************************************************/
void hidh_l2cif_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO *p_cfg)
{
    UINT8 dhandle;
    tHID_CONN *p_hcon = NULL;

    if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_2(0x001e0003, "HID - Rcvd cfg cfm, CID: 0x%x  Result: %d",
                 l2cap_cid, p_cfg->result);
    }

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
    {
        p_hcon = &hh_cb.devices[dhandle].conn;
    }

    if (p_hcon == NULL)
    {
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
        {
            LogMsg_1(0x001e0001, "HID - Rcvd L2CAP cfg ind, unknown CID: 0x%x",
                     l2cap_cid);
        }
        return;
    }

    if (p_cfg->result != 0)
    {
        hidh_conn_disconnect(dhandle);
        hh_cb.dev_cback(dhandle, HID_HDEV_EVT_CLOSE,
                        HID_L2CAP_CFG_FAIL | p_cfg->result, NULL);
        return;
    }

    if (l2cap_cid == p_hcon->ctrl_cid)
    {
        p_hcon->conn_flags |= HID_CONN_FLAG_CTL_CFG_CFM;
    }
    else
    {
        p_hcon->conn_flags |= HID_CONN_FLAG_INTR_CFG_CFM;
    }

    if (((p_hcon->conn_flags & HID_CONN_FLAG_ALL_CONFIG) ==
         HID_CONN_FLAG_ALL_CONFIG) &&
        (p_hcon->conn_state == HID_CONN_STATE_CONFIG))
    {
        p_hcon->conn_state = HID_CONN_STATE_OPENED;
        hh_cb.devices[dhandle].state = HID_DEV_CONNECTED;
        hh_cb.dev_cback(dhandle, HID_HDEV_EVT_OPEN, 0, NULL);
    }
}

/*******************************************************************************
**
** Function         hidh_l2cif_disconnect_ind
**
** Description      This function handles an L2CAP disconnect indication event.
**
*******************************************************************************/
void hidh_l2cif_disconnect_ind(UINT16 l2cap_cid, UINT8 ack_needed)
{
    UINT8 dhandle;
    tHID_CONN *p_hcon = NULL;
    UINT16 disc_res = 0;
    UINT16 hid_close_evt_reason;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
    {
        p_hcon = &hh_cb.devices[dhandle].conn;
    }

    if (p_hcon == NULL)
    {
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
        {
            LogMsg_1(0x001e0001, "HID - Rcvd L2CAP disc, unknown CID: 0x%x",
                     l2cap_cid);
        }
        return;
    }

    if (ack_needed)
    {
        L2CA_DisconnectRsp(l2cap_cid);
    }

    if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_1(0x001e0003, "HID - Rcvd L2CAP disc, CID: 0x%x", l2cap_cid);
    }

    p_hcon->conn_state = HID_CONN_STATE_DISCONNECTING;

    if (l2cap_cid == p_hcon->ctrl_cid)
    {
        p_hcon->ctrl_cid = 0;
    }
    else
    {
        p_hcon->intr_cid = 0;
    }

    if ((p_hcon->ctrl_cid == 0) && (p_hcon->intr_cid == 0))
    {
        hh_cb.devices[dhandle].state = HID_DEV_NO_CONN;
        p_hcon->conn_state = HID_CONN_STATE_UNUSED;

        if (!ack_needed)
        {
            disc_res = btm_get_acl_disc_reason_code();
        }

        hid_close_evt_reason = p_hcon->disc_reason;

        if ((disc_res == HCI_ERR_AUTH_FAILURE) ||
            (disc_res == HCI_ERR_KEY_MISSING) ||
            (disc_res == HCI_ERR_HOST_REJECT_SECURITY) ||
            (disc_res == HCI_ERR_PAIRING_NOT_ALLOWED) ||
            (disc_res == HCI_ERR_UNIT_KEY_USED) ||
            (disc_res == HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED) ||
            (disc_res == HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE) ||
            (disc_res == HCI_ERR_REPEATED_ATTEMPTS))
        {
            hid_close_evt_reason = HID_ERR_AUTH_FAILED;
        }

        hh_cb.dev_cback(dhandle, HID_HDEV_EVT_CLOSE, hid_close_evt_reason,
                        NULL);
    }
}

/*******************************************************************************
**
** Function         hidh_l2cif_disconnect_cfm
**
** Description      This function handles an L2CAP disconnect confirm event.
**
*******************************************************************************/
void hidh_l2cif_disconnect_cfm(UINT16 l2cap_cid, UINT16 result)
{
    UINT8 dhandle;
    tHID_CONN *p_hcon = NULL;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
    {
        p_hcon = &hh_cb.devices[dhandle].conn;
    }

    if (p_hcon == NULL)
    {
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
        {
            LogMsg_1(0x001e0001,
                     "HID - Rcvd L2CAP disc cfm, unknown CID: 0x%x",
                     l2cap_cid);
        }
        return;
    }

    if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_1(0x001e0003, "HID - Rcvd L2CAP disc cfm, CID: 0x%x",
                 l2cap_cid);
    }

    if (l2cap_cid == p_hcon->ctrl_cid)
    {
        p_hcon->ctrl_cid = 0;
    }
    else
    {
        p_hcon->intr_cid = 0;
    }

    if ((p_hcon->ctrl_cid == 0) && (p_hcon->intr_cid == 0))
    {
        hh_cb.devices[dhandle].state = HID_DEV_NO_CONN;
        p_hcon->conn_state = HID_CONN_STATE_UNUSED;
        hh_cb.dev_cback(dhandle, HID_HDEV_EVT_CLOSE, p_hcon->disc_reason,
                        NULL);
    }
}

/*******************************************************************************
**
** Function         hidh_l2cif_cong_ind
**
** Description      This function handles an L2CAP congestion indication event.
**
*******************************************************************************/
void hidh_l2cif_cong_ind(UINT16 l2cap_cid, UINT8 is_congested)
{
    UINT8 dhandle;
    tHID_CONN *p_hcon = NULL;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
    {
        p_hcon = &hh_cb.devices[dhandle].conn;
    }

    if (p_hcon == NULL)
    {
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
        {
            LogMsg_1(0x001e0001,
                     "HID - Rcvd L2CAP congestion status, unknown CID: 0x%x",
                     l2cap_cid);
        }
        return;
    }

    if (hh_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_2(0x001e0003,
                 "HID - Rcvd L2CAP congestion status, CID: 0x%x  Cong: %d",
                 l2cap_cid, is_congested);
    }

    if (is_congested)
    {
        p_hcon->conn_flags |= HID_CONN_FLAG_CONGESTED;
    }
    else
    {
        p_hcon->conn_flags &= ~HID_CONN_FLAG_CONGESTED;
    }
}

/*******************************************************************************
**
** Function         hidh_l2cif_data_ind
**
** Description      This function handles an L2CAP data indication event.
**
*******************************************************************************/
void hidh_l2cif_data_ind(UINT16 l2cap_cid, BT_HDR *p_buf)
{
    UINT8 *p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;
    UINT8 ttype;
    UINT8 param;
    UINT8 rep_type;
    UINT8 evt;
    UINT8 dhandle;
    tHID_CONN *p_hcon = NULL;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
    {
        p_hcon = &hh_cb.devices[dhandle].conn;
    }

    if (p_hcon == NULL)
    {
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
        {
            LogMsg_1(0x001e0001, "HID - Rcvd L2CAP data, unknown CID: 0x%x",
                     l2cap_cid);
        }
        GKI_freebuf(p_buf);
        return;
    }

    ttype = HID_GET_TRANS_FROM_HDR(*p_data);
    param = HID_GET_PARAM_FROM_HDR(*p_data);
    rep_type = param & HID_PAR_REP_TYPE_MASK;
    p_data++;

    p_buf->len--;
    p_buf->offset++;

    switch (ttype)
    {
    case HID_TRANS_HANDSHAKE:
        hh_cb.dev_cback(dhandle, HID_HDEV_EVT_HANDSHAKE, param, NULL);
        GKI_freebuf(p_buf);
        break;

    case HID_TRANS_CONTROL:
        switch (param)
        {
        case HID_PAR_CONTROL_VIRTUAL_CABLE_UNPLUG:
            hidh_conn_disconnect(dhandle);
            hh_cb.dev_cback(dhandle, HID_HDEV_EVT_VC_UNPLUG, 0, NULL);
            break;

        default:
            break;
        }
        GKI_freebuf(p_buf);
        break;

    case HID_TRANS_DATA:
        evt = (hh_cb.devices[dhandle].conn.intr_cid == l2cap_cid)
                  ? HID_HDEV_EVT_INTR_DATA
                  : HID_HDEV_EVT_CTRL_DATA;
        hh_cb.dev_cback(dhandle, evt, rep_type, p_buf);
        break;

    case HID_TRANS_DATAC:
        evt = (hh_cb.devices[dhandle].conn.intr_cid == l2cap_cid)
                  ? HID_HDEV_EVT_INTR_DATC
                  : HID_HDEV_EVT_CTRL_DATC;
        hh_cb.dev_cback(dhandle, evt, rep_type, p_buf);
        break;

    default:
        GKI_freebuf(p_buf);
        break;
    }
}

/*******************************************************************************
**
** Function         hidh_conn_snd_data
**
** Description      This function is called to send data to the device.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
tHID_STATUS hidh_conn_snd_data(UINT8 dhandle, UINT8 trans_type, UINT8 param,
                               UINT16 data, UINT8 report_id, BT_HDR *buf)
{
    tHID_HOST_DEV_CTB *p_dev;
    BT_HDR *p_buf;
    UINT8 *p_out;
    UINT16 bytes_copied;
    UINT8 seg_req;
    UINT16 data_size;
    UINT16 cid;
    UINT8 pool_id;
    UINT8 use_data = 0;
    UINT8 blank_datc = FALSE;

    p_dev = &hh_cb.devices[dhandle];

    if (p_dev->conn.conn_flags & HID_CONN_FLAG_CONGESTED)
    {
        if (buf)
        {
            GKI_freebuf(buf);
        }
        return (HID_ERR_CONGESTED);
    }

    switch (trans_type)
    {
    case HID_TRANS_CONTROL:
    case HID_TRANS_GET_REPORT:
    case HID_TRANS_SET_REPORT:
    case HID_TRANS_GET_PROTOCOL:
    case HID_TRANS_SET_PROTOCOL:
    case HID_TRANS_GET_IDLE:
    case HID_TRANS_SET_IDLE:
        cid = p_dev->conn.ctrl_cid;
        pool_id = 2;
        break;

    case HID_TRANS_DATA:
        cid = p_dev->conn.intr_cid;
        pool_id = 2;
        break;

    default:
        return (HID_ERR_INVALID_PARAM);
    }

    if (trans_type == HID_TRANS_SET_IDLE)
    {
        use_data = 1;
    }
    else if (trans_type == HID_TRANS_GET_REPORT && (param & 0x08))
    {
        use_data = 2;
    }

    do
    {
        if (buf == NULL || blank_datc)
        {
            if ((p_buf = (BT_HDR *)GKI_getpoolbuf(pool_id)) == NULL)
            {
                return (HID_ERR_NO_RESOURCES);
            }
            p_buf->offset = 9;
            seg_req = FALSE;
            data_size = 0;
            bytes_copied = 0;
            blank_datc = FALSE;
        }
        else if (buf->len > (p_dev->conn.rem_mtu_size - 1))
        {
            if ((p_buf = (BT_HDR *)GKI_getpoolbuf(pool_id)) == NULL)
            {
                return (HID_ERR_NO_RESOURCES);
            }
            p_buf->offset = 9;
            seg_req = TRUE;
            data_size = buf->len;
            bytes_copied = p_dev->conn.rem_mtu_size - 1;
        }
        else
        {
            p_buf = buf;
            p_buf->offset -= 1;
            seg_req = FALSE;
            data_size = buf->len;
            bytes_copied = buf->len;
        }

        p_out = (UINT8 *)(p_buf + 1) + p_buf->offset;
        *p_out++ = HID_BUILD_HDR(trans_type, param);

        if (trans_type == HID_TRANS_GET_REPORT && report_id != 0)
        {
            *p_out = report_id;
            data_size = bytes_copied = 1;
        }

        if (seg_req)
        {
            memcpy(p_out, ((UINT8 *)(buf + 1)) + buf->offset, bytes_copied);
            buf->offset += bytes_copied;
            buf->len -= bytes_copied;
        }
        else if (use_data == 1)
        {
            *(p_out + bytes_copied) = data & 0xff;
        }
        else if (use_data == 2)
        {
            *(p_out + bytes_copied) = data & 0xff;
            *(p_out + bytes_copied + 1) = (data >> 8) & 0xff;
        }

        p_buf->len = bytes_copied + 1 + use_data;
        data_size -= bytes_copied;

        if ((p_dev->conn.conn_flags & HID_CONN_FLAG_CONGESTED) ||
            (!L2CA_DataWrite(cid, p_buf)))
        {
            return (HID_ERR_CONGESTED);
        }

        if (data_size)
        {
            trans_type = HID_TRANS_DATAC;
        }
        else if (bytes_copied == (p_dev->conn.rem_mtu_size - 1))
        {
            trans_type = HID_TRANS_DATAC;
            blank_datc = TRUE;
        }
    } while ((data_size != 0) || blank_datc);

    return (HID_SUCCESS);
}

/*******************************************************************************
**
** Function         hidh_conn_initiate
**
** Description      This function is called to initiate a HID connection.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
tHID_STATUS hidh_conn_initiate(UINT8 dhandle)
{
    tHID_HOST_DEV_CTB *p_dev = &hh_cb.devices[dhandle];

    if (p_dev->conn.conn_state != HID_CONN_STATE_UNUSED)
    {
        return (HID_ERR_CONN_IN_PROCESS);
    }

    p_dev->conn.ctrl_cid = 0;
    p_dev->conn.intr_cid = 0;
    p_dev->conn.disc_reason = HID_L2CAP_CONN_FAIL;
    p_dev->conn.conn_flags = 1;
    p_dev->conn.ctrl_cid = L2CA_ConnectReq(HID_PSM_CONTROL, p_dev->addr);

    if (p_dev->conn.ctrl_cid == 0)
    {
        if (hh_cb.trace_level >= BT_TRACE_LEVEL_WARNING)
        {
            LogMsg_0(0x001e0001, "HID - Originate failed");
        }
        hh_cb.dev_cback((UINT8)((p_dev - hh_cb.devices) /
                                sizeof(tHID_HOST_DEV_CTB)),
                        HID_HDEV_EVT_CLOSE, HID_ERR_L2CAP_FAILED, NULL);
    }
    else
    {
        p_dev->conn.conn_state = HID_CONN_STATE_CONNECTING;
    }

    return (HID_SUCCESS);
}

/*******************************************************************************
**
** Function         hidh_conn_dereg
**
** Description      This function deregisters HID Host with L2CAP
**
** Returns          void
**
*******************************************************************************/
void hidh_conn_dereg(void)
{
    L2CA_Deregister(HID_PSM_CONTROL);
    L2CA_Deregister(HID_PSM_INTERRUPT);
}
