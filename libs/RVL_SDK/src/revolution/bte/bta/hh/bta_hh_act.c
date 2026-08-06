// bta_hh_act.c -- BTA HID Host action functions.
// High-level C reconstruction matching
// build/us/asm/RVL_SDK/src/revolution/bte/bta/hh/bta_hh_act.s
// (retail object: build/us/obj/RVL_SDK/src/revolution/bte/bta/hh/bta_hh_act.o)

#include <string.h>

#include <harness_catalog.h>

#include <revolution/BTE/gki/platform/data_types.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/bta/include/bd.h>
#include <revolution/BTE/bta/include/bta_hh_api.h>
#include <revolution/BTE/bta/sys/bta_sys.h>
#include <revolution/BTE/stack/include/hiddefs.h>
#include <revolution/BTE/stack/include/hidh_api.h>
#include <revolution/BTE/stack/include/sdp_api.h>
#include <revolution/BTE/include/bt_trace.h>
#include <revolution/BTE/gki/common/gki.h>

/* ------------------------------------------------------------------ */
/*  Local constants                                                    */
/* ------------------------------------------------------------------ */

/* HID device states (bta_hh_int.h tBTA_HH_STATE). */
enum { BTA_HH_NULL_ST, BTA_HH_IDLE_ST, BTA_HH_W4_CONN_ST, BTA_HH_CONN_ST };

/* The retail binary was compiled with BTA_HH_MAX_KNOWN = 16 (the
 * header default HID_HOST_MAX_DEVICES is 7).  Override here to match. */
#undef BTA_HH_MAX_KNOWN
#define BTA_HH_MAX_KNOWN    16

#define BTA_HH_MAX_RPT_CHARS        8

#define BTA_HH_FST_TRANS_CB_EVT     BTA_HH_GET_RPT_EVT
#define BTA_HH_FST_BTE_TRANS_EVT    HID_TRANS_GET_REPORT

/* BTA HH internal (state-machine) events -- BTA_SYS_EVT_START(BTA_ID_HH). */
#define BTA_HH_API_OPEN_EVT     (BTA_SYS_EVT_START(BTA_ID_HH) + 0)
#define BTA_HH_API_CLOSE_EVT    (BTA_SYS_EVT_START(BTA_ID_HH) + 1)
#define BTA_HH_INT_OPEN_EVT     (BTA_SYS_EVT_START(BTA_ID_HH) + 2)
#define BTA_HH_INT_CLOSE_EVT    (BTA_SYS_EVT_START(BTA_ID_HH) + 3)
#define BTA_HH_INT_DATA_EVT     (BTA_SYS_EVT_START(BTA_ID_HH) + 4)
#define BTA_HH_INT_CTRL_DATA    (BTA_SYS_EVT_START(BTA_ID_HH) + 5)
#define BTA_HH_INT_HANDSK_EVT   (BTA_SYS_EVT_START(BTA_ID_HH) + 6)
#define BTA_HH_SDP_CMPL_EVT     (BTA_SYS_EVT_START(BTA_ID_HH) + 7)
#define BTA_HH_API_WRITE_DEV_EVT (BTA_SYS_EVT_START(BTA_ID_HH) + 8)
#define BTA_HH_API_GET_DSCP_EVT (BTA_SYS_EVT_START(BTA_ID_HH) + 9)
#define BTA_HH_API_MAINT_DEV_EVT (BTA_SYS_EVT_START(BTA_ID_HH) + 10)
#define BTA_HH_OPEN_CMPL_EVT    (BTA_SYS_EVT_START(BTA_ID_HH) + 11)
#define BTA_HH_API_ENABLE_EVT   (BTA_SYS_EVT_START(BTA_ID_HH) + 12)
#define BTA_HH_API_DISABLE_EVT  (BTA_SYS_EVT_START(BTA_ID_HH) + 13)
#define BTA_HH_API_GET_ACL_Q_EVT (BTA_SYS_EVT_START(BTA_ID_HH) + 14)
#define BTA_HH_DISC_CMPL_EVT    (BTA_SYS_EVT_START(BTA_ID_HH) + 15)
#define BTA_HH_INVALID_EVT      (BTA_SYS_EVT_START(BTA_ID_HH) + 16)

/* BTA_HH_GET_ACL_Q_EVT is not in the public header (bta_hh_int.h enum). */
#ifndef BTA_HH_GET_ACL_Q_EVT
#define BTA_HH_GET_ACL_Q_EVT    0x0f
#endif

/* This build's tBTA_HH_STATUS diverges from the public header enum:
 * BTA_HH_ERR_TOD_UNSPT is 0xFF here (see bta_hh_sdp_cback / sdp_cmpl),
 * and the initial SDP status used by bta_hh_sdp_cback is 0x10.
 * BTA_HH_ERR_DB_FULL is 0x0A (an entry was dropped from the enum). */
#undef BTA_HH_ERR_TOD_UNSPT
#define BTA_HH_ERR_TOD_UNSPT    0xFF
#undef BTA_HH_ERR_DB_FULL
#define BTA_HH_ERR_DB_FULL      0x0A
#define BTA_HH_ERR_16           0x10

/* ------------------------------------------------------------------ */
/*  Internal types -- not in the public SDK headers                     */
/* ------------------------------------------------------------------ */

/* HID device control block (kdev[] element, 0x20 bytes). */
typedef struct
{
    tHID_DEV_DSCP_INFO  dscp_info;       /* 0x00: dl_len(2) + pad(2) + dsc_list(4) */
    BD_ADDR             addr;            /* 0x08 */
    UINT16              attr_mask;       /* 0x0E */
    UINT16              w4_evt;          /* 0x10 */
    UINT8               index;           /* 0x12 */
    UINT8               sub_class;       /* 0x13 */
    UINT8               sec_mask;        /* 0x14 */
    UINT8               app_id;          /* 0x15 */
    UINT8               hid_handle;      /* 0x16 */
    BOOLEAN             vp;              /* 0x17 */
    BOOLEAN             in_use;          /* 0x18 */
    BOOLEAN             incoming_conn;   /* 0x19 */
    BOOLEAN             opened;          /* 0x1A */
    tBTA_HH_PROTO_MODE  mode;            /* 0x1B */
    UINT8               state;           /* 0x1C */
    UINT8               pad1[3];         /* 0x1D-0x1F */
} tBTA_HH_DEV_CB;                        /* 0x20 */

/* Main BTA HH control block (global bta_hh_cb, 0x230 bytes, .bss). */
typedef struct
{
    UINT8               pad0[0x10];      /* kb_cb + pad: 0x00-0x0F */
    tBTA_HH_DEV_CB      kdev[BTA_HH_MAX_KNOWN];   /* 0x010-0x20F */
    tBTA_HH_DEV_CB     *p_cur;           /* 0x210 */
    UINT8               cb_index[BTA_HH_MAX_KNOWN]; /* 0x214-0x223 */
    tBTA_HH_CBACK      *p_cback;         /* 0x224 */
    void               *p_disc_db;       /* 0x228 */
    UINT8               trace_level;     /* 0x22C */
    UINT8               cnt_num;         /* 0x22D */
    BOOLEAN             w4_disable;      /* 0x22E */
} tBTA_HH_CB;                            /* 0x230 */

/* BTA HH API connection request data (tBTA_HH_API_CONN). */
typedef struct
{
    BT_HDR              hdr;             /* 0x00 */
    BD_ADDR             bd_addr;         /* 0x08 */
    UINT8               sec_mask;        /* 0x0E */
    tBTA_HH_PROTO_MODE  mode;            /* 0x0F */
} tBTA_HH_API_CONN;                      /* 0x10 */

/* BTA HH API enable request (tBTA_HH_API_ENABLE) -- this build's caller
 * (BTA_HhEnable) keeps the callback pointer at 0x30. */
typedef struct
{
    BT_HDR              hdr;             /* 0x00 */
    UINT8               sec_mask;        /* 0x08 */
    UINT8               pad[0x27];       /* 0x09-0x2F */
    tBTA_HH_CBACK      *p_cback;         /* 0x30 */
} tBTA_HH_API_ENABLE_RETAIL;             /* 0x34 */

/* BTA HH add/remove-device request (tBTA_HH_MAINT_DEV).  The 16-bit
 * sub_event at 0x10 selects ADD (0x000B) vs REMOVE (0x000C). */
typedef struct
{
    BT_HDR              hdr;             /* 0x00 */
    BD_ADDR             bda;             /* 0x08 */
    UINT16              attr_mask;       /* 0x0E */
    UINT16              sub_event;       /* 0x10 */
    UINT8               sub_class;       /* 0x12 */
    UINT8               app_id;          /* 0x13 */
    tHID_DEV_DSCP_INFO  dscp_info;       /* 0x14 */
} tBTA_HH_MAINT_DEV;                     /* 0x1C */

/* BTA HH command data sent to the device (tBTA_HH_CMD_DATA). */
typedef struct
{
    BT_HDR              hdr;             /* 0x00 */
    UINT8               t_type;          /* 0x08 */
    UINT8               param;           /* 0x09 */
    UINT8               rpt_id;          /* 0x0A */
    UINT8               pad1;            /* 0x0B */
    UINT16              data;            /* 0x0C */
    UINT16              pad2;            /* 0x0E */
    BT_HDR             *p_data;          /* 0x10 */
} tBTA_HH_CMD_DATA;                      /* 0x14 */

/* BTA HH callback data from the HID host (tBTA_HH_CBACK_DATA). */
typedef struct
{
    BT_HDR              hdr;             /* 0x00 */
    UINT32              data;            /* 0x08 */
    BT_HDR             *p_data;          /* 0x0C */
} tBTA_HH_CBACK_DATA;                    /* 0x10 */

/* Internal message union fed to the BTA HH state machine. */
typedef union
{
    BT_HDR              hdr;
    tBTA_HH_API_CONN    api_conn;
    tBTA_HH_API_ENABLE_RETAIL  api_enable;
    tBTA_HH_MAINT_DEV   maint_dev;
    tBTA_HH_CMD_DATA    api_sndcmd;
    tBTA_HH_CBACK_DATA  hid_cback;
    tBTA_HH_STATUS      status;
} tBTA_HH_DATA;

/* Handshake data -- this build's rsp_data carries a full BT_HDR (8 bytes),
 * unlike the public header's pointer variant. */
typedef struct
{
    tBTA_HH_STATUS  status;              /* 0x00 */
    UINT8           handle;              /* 0x01 */
    union
    {
        tBTA_HH_PROTO_MODE  proto_mode;  /* 0x02 */
        BT_HDR              rpt_data;    /* 0x02 */
        UINT8               idle_rate;   /* 0x02 */
    } rsp_data;                          /* 0x02, 8 bytes */
} tBTA_HH_HSDATA_RETAIL;                 /* 0x0A */

/* Per-link queue info (tBTA_HH_QUEUE_INFO). */
typedef struct
{
    UINT8   handle;                      /* 0x00 */
    UINT16  num_queued;                  /* 0x02 */
    UINT16  num_not_acked;               /* 0x04 */
} tBTA_HH_QUEUE_INFO;                    /* 0x06 */

/* ACL queue info reported to the app (tBTA_HH_ACL_QUEUE_INFO). */
typedef struct
{
    UINT16              num_avail_buffs; /* 0x00 */
    UINT16              num_buffs;       /* 0x02 */
    UINT16              num_links;       /* 0x04 */
    tBTA_HH_QUEUE_INFO  queue_info[4];   /* 0x06 */
} tBTA_HH_ACL_QUEUE_INFO;                /* 0x1E */

/* HID device SDP record -- this build's layout (sub_class at 0x64). */
typedef struct
{
    char     svc_name[0x20];             /* 0x00 */
    char     svc_descr[0x20];            /* 0x20 */
    char     prov_name[0x20];            /* 0x40 */
    UINT16   rel_num;                    /* 0x60 */
    UINT16   hpars_ver;                  /* 0x62 */
    UINT8    sub_class;                  /* 0x64 */
    UINT8    ctry_code;                  /* 0x65 */
    UINT16   sup_timeout;                /* 0x66 */
    tHID_DEV_DSCP_INFO dscp_info;        /* 0x68 */
    void    *p_sdp_layer_rec;            /* 0x70 */
} tHID_DEV_SDP_INFO_RETAIL;              /* 0x74 */

/* HID host control block (global hh_cb) -- devices[] element only. */
typedef struct
{
    UINT8   in_use;                      /* 0x00 */
    BD_ADDR addr;                        /* 0x01 */
    UINT8   pad1;                        /* 0x07 */
    UINT16  attr_mask;                   /* 0x08 */
    UINT8   state;                       /* 0x0A */
    UINT8   conn_substate;               /* 0x0B */
    UINT8   conn_tries;                  /* 0x0C */
    UINT8   pad2[3];                     /* 0x0D */
    struct
    {
        UINT8   conn_state;              /* 0x10 */
        UINT8   conn_flags;              /* 0x11 */
        UINT8   ctrl_id;                 /* 0x12 */
        UINT8   pad3;                    /* 0x13 */
        UINT16  ctrl_cid;                /* 0x14 */
        UINT16  intr_cid;                /* 0x16 */
        UINT8   pad4[0x1C];              /* 0x18-0x33 */
    } conn;                              /* 0x10, 0x24 bytes */
} tHID_HOST_DEV_CTB;                     /* 0x34 */

/* L2CAP link control block -- fields touched by this TU. */
typedef struct
{
    UINT8   in_use;                      /* 0x00 */
    UINT8   pad0[0x37];                  /* 0x01 */
    UINT16  sent_not_acked;              /* 0x38 */
    UINT8   pad1[0x0A];                  /* 0x3A */
    BUFFER_Q link_xmit_data_q;           /* 0x44 */
} tL2C_LCB;

/* L2CAP channel control block -- only p_lcb is touched here. */
typedef struct
{
    UINT8   pad0[0x10];
    tL2C_LCB *p_lcb;                     /* 0x10 */
} tL2C_CCB;

/* L2CAP global control block (global l2cb, 0x7E8 bytes) -- first fields
 * only; declared at the full retail size so MWCC keeps full 32-bit
 * addressing (small extern types would pick @sda21). */
typedef struct
{
    UINT8   l2cap_trace_level;           /* 0x00 */
    UINT8   is_register;                 /* 0x01 */
    UINT16  num_lm_acl_bufs;             /* 0x02 */
    UINT16  controller_xmit_window;      /* 0x04 */
    UINT8   pad[0x7E8 - 8];
} tL2C_CB_RETAIL;

/* ------------------------------------------------------------------ */
/*  External globals and functions                                     */
/* ------------------------------------------------------------------ */

extern tBTA_HH_CB     bta_hh_cb;
extern UINT8          appl_trace_level;
extern tBTA_HH_CFG   *p_bta_hh_cfg;

extern void utl_freebuf(void **p);
extern void bta_hh_clean_up_kdev(tBTA_HH_DEV_CB *p_cb);

/* HID host event callback (defined in this TU). */
void bta_hh_cback(UINT8 dev_handle, UINT8 event, UINT32 data, BT_HDR *p_data);

extern void bta_hh_sm_execute(tBTA_HH_DEV_CB *p_cb, UINT16 event,
                              tBTA_HH_DATA *p_data);
extern void bta_hh_trace_dev_db(void);
extern BOOLEAN bta_hh_tod_spt(tBTA_HH_DEV_CB *p_cb, UINT8 sub_class);
extern void bta_hh_add_device_to_list(tBTA_HH_DEV_CB *p_cb, UINT8 handle,
                                      UINT16 attr_mask,
                                      tHID_DEV_DSCP_INFO *p_dscp_info,
                                      UINT8 sub_class, UINT8 app_id);
extern void bta_hh_start_sdp(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);

extern void bta_hh_co_open(UINT8 dev_handle, UINT8 sub_class,
                           UINT16 attr_mask, UINT8 app_id);
extern void bta_hh_co_close(UINT8 dev_handle, UINT8 app_id);

extern tHID_HOST_DEV_CTB hh_cb[];
extern tL2C_CB_RETAIL    l2cb;
extern tL2C_CCB *l2cu_find_ccb_by_cid(tL2C_LCB *p_lcb, UINT16 local_cid);

/* ------------------------------------------------------------------ */
/*  Local helpers                                                      */
/* ------------------------------------------------------------------ */

/* Name of the event a device control block is waiting for.  MWCC
 * auto-inlines this at every trace call site (no standalone copy). */
static char const *bta_hh_get_w4_event(UINT16 event)
{
    switch (event) {
    case BTA_HH_GET_RPT_EVT:
        return "BTA_HH_GET_RPT_EVT";

    case BTA_HH_SET_RPT_EVT:
        return "BTA_HH_SET_RPT_EVT";

    case BTA_HH_GET_PROTO_EVT:
        return "BTA_HH_GET_PROTO_EVT";

    case BTA_HH_SET_PROTO_EVT:
        return "BTA_HH_SET_PROTO_EVT";

    case BTA_HH_GET_IDLE_EVT:
        return "BTA_HH_GET_IDLE_EVT";

    case BTA_HH_SET_IDLE_EVT:
        return "BTA_HH_SET_IDLE_EVT";

    case BTA_HH_OPEN_EVT:
        return "BTA_HH_OPEN_EVT";

    default:
    case BTA_HH_ENABLE_EVT:
    case BTA_HH_DISABLE_EVT:
    case BTA_HH_CLOSE_EVT:
        return "Unknown event";
    }
}

/* ------------------------------------------------------------------ */
/*  Action functions                                                   */
/* ------------------------------------------------------------------ */

/* Enable the HID host: initialize the BTE HID host, zero the control
 * block, register the host callback, and seed the device table. */
void bta_hh_api_enable(tBTA_HH_DATA *p_data)
{
    tBTA_HH_STATUS status = BTA_HH_ERR;
    UINT8 xx;

    HID_HostInit();

    memset(&bta_hh_cb, 0, sizeof bta_hh_cb);

    HID_HostSetSecurityLevel("\0\0\0\0\0\0\0",
                             p_data->api_enable.sec_mask);

    if (HID_HostRegister(bta_hh_cback) == HID_SUCCESS) {
        bta_hh_cb.p_cback = p_data->api_enable.p_cback;
        status = BTA_HH_OK;

        for (xx = 0; xx < BTA_HH_MAX_KNOWN; xx++) {
            bta_hh_cb.kdev[xx].state = BTA_HH_IDLE_ST;
            bta_hh_cb.kdev[xx].hid_handle = BTA_HH_INVALID_HANDLE;
            bta_hh_cb.kdev[xx].index = xx;
            bta_hh_cb.cb_index[xx] = BTA_HH_MAX_KNOWN;
        }
    }

    (*bta_hh_cb.p_cback)(BTA_HH_ENABLE_EVT, (tBTA_HH *)&status);
}

void bta_hh_api_disable(void)
{
    UINT8 xx;
    tBTA_HH_STATUS status;

    if (!bta_hh_cb.p_cback)
        return;

    if (bta_hh_cb.cnt_num == 0) {
        status = BTA_HH_OK;

        if (HID_HostDeregister() != HID_SUCCESS)
            status = BTA_HH_ERR;

        for (xx = 0; xx < BTA_HH_MAX_KNOWN; xx++)
            utl_freebuf((void **)&bta_hh_cb.kdev[xx].dscp_info.dsc_list);

        utl_freebuf((void **)&bta_hh_cb.p_disc_db);

        (*bta_hh_cb.p_cback)(BTA_HH_DISABLE_EVT, (tBTA_HH *)&status);

        memset(&bta_hh_cb, 0, sizeof bta_hh_cb);
    } else {
        bta_hh_cb.w4_disable = TRUE;

        for (xx = 0; xx < BTA_HH_MAX_KNOWN; xx++) {
            if (bta_hh_cb.kdev[xx].state == BTA_HH_CONN_ST)
                bta_hh_sm_execute(&bta_hh_cb.kdev[xx], BTA_HH_API_CLOSE_EVT,
                                  NULL);
        }
    }
}

void bta_hh_disc_cmpl(void)
{
    UINT8 xx;
    tBTA_HH_STATUS status = BTA_HH_OK;

    if (HID_HostDeregister() != HID_SUCCESS)
        status = BTA_HH_ERR;

    for (xx = 0; xx < BTA_HH_MAX_KNOWN; xx++)
        utl_freebuf((void **)&bta_hh_cb.kdev[xx].dscp_info.dsc_list);

    utl_freebuf((void **)&bta_hh_cb.p_disc_db);

    (*bta_hh_cb.p_cback)(BTA_HH_DISABLE_EVT, (tBTA_HH *)&status);

    memset(&bta_hh_cb, 0, sizeof bta_hh_cb);
}

/* SDP discovery callback (global in this build's symbol table). */
void bta_hh_sdp_cback(UINT16 result, UINT16 attr_mask,
                      tHID_DEV_SDP_INFO_RETAIL *sdp_rec)
{
    tBTA_HH_DEV_CB *p_cb = bta_hh_cb.p_cur;
    UINT8 hdl;
    tBTA_HH_STATUS status = BTA_HH_ERR_16;

    if (result == SDP_SUCCESS) {
        /* security is required for the connection, add attr_mask bit */
        if (p_cb->sec_mask)
            attr_mask |= HID_SEC_REQUIRED;

        if (appl_trace_level >= 4)
            LogMsg_3(0x503, "bta_hh_sdp_cback: p_cb: %d result 0x%02x,                             attr_mask 0x%02x",
                     (UINT32)p_cb, result, attr_mask);

        /* check to see if the type of device is supported */
        if (bta_hh_tod_spt(p_cb, sdp_rec->sub_class)) {
            /* if not added before */
            if (p_cb->hid_handle == BTA_HH_INVALID_HANDLE) {
                /* add device/update attr_mask information */
                if ((status = HID_HostAddDev(p_cb->addr, attr_mask, &hdl)) ==
                    HID_SUCCESS) {
                    /* update cb_index[] map */
                    bta_hh_cb.cb_index[hdl] = p_cb->index;

                    /* update cb information with attr_mask, dscp_info etc. */
                    bta_hh_add_device_to_list(p_cb, hdl, attr_mask,
                                              &sdp_rec->dscp_info,
                                              sdp_rec->sub_class,
                                              p_cb->app_id);
                } else {
                    p_cb->app_id = 0;
                }
            }
        } else {
            /* type of device is not supported */
            status = BTA_HH_ERR_TOD_UNSPT;
        }
    }

    /* free disc_db when SDP is completed */
    utl_freebuf((void **)&bta_hh_cb.p_disc_db);

    /* send SDP_CMPL_EVT into the state machine */
    bta_hh_sm_execute(p_cb, BTA_HH_SDP_CMPL_EVT, (tBTA_HH_DATA *)&status);
}

/* Start the SDP discovery for a device.  Known devices (app_id != 0)
 * skip SDP and are added straight to the HID host; unknown devices get
 * a discovery database and async SDP record fetch. */
void bta_hh_start_sdp(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    tBTA_HH_STATUS status = BTA_HH_ERR_16;
    UINT8 hdl;

    p_cb->sec_mask = p_data->api_conn.sec_mask;
    p_cb->mode = p_data->api_conn.mode;

    if (p_cb->app_id != 0) {
        /* known device: skip SDP */
        status = BTA_HH_OK;

        if (appl_trace_level >= 5)
            LogMsg_0(0x504,
                     "bta_hh_start_sdp:: skip SDP for known devices");

        if (p_cb->hid_handle == BTA_HH_INVALID_HANDLE) {
            if ((status = HID_HostAddDev(p_cb->addr, p_cb->attr_mask,
                                         &hdl)) == HID_SUCCESS) {
                bta_hh_add_device_to_list(p_cb, hdl, p_cb->attr_mask, NULL,
                                          p_cb->sub_class, p_cb->app_id);
                bta_hh_cb.cb_index[hdl] = p_cb->index;
            }
        }

        bta_hh_sm_execute(p_cb, BTA_HH_SDP_CMPL_EVT,
                          (tBTA_HH_DATA *)&status);
    } else {
        /* unknown device: query SDP record */
        if (bta_hh_cb.p_disc_db == NULL) {
            bta_hh_cb.p_disc_db =
                GKI_getbuf(p_bta_hh_cfg->sdp_db_size);
            bta_hh_cb.p_cur = p_cb;

            if ((status = HID_HostGetSDPRecord(
                     p_data->api_conn.bd_addr, bta_hh_cb.p_disc_db,
                     p_bta_hh_cfg->sdp_db_size,
                     (tHID_HOST_SDP_CALLBACK *)bta_hh_sdp_cback)) !=
                HID_SUCCESS) {
                if (appl_trace_level >= 5)
                    LogMsg_1(0x504,
                             "bta_hh_start_sdp:  HID_HostGetSDPRecord failed:                 Status 0x%2X",
                             status);

                status = BTA_HH_ERR_SDP;
                utl_freebuf((void **)&bta_hh_cb.p_disc_db);
            }
        }

        if (status != BTA_HH_OK)
            bta_hh_sm_execute(p_cb, BTA_HH_SDP_CMPL_EVT,
                              (tBTA_HH_DATA *)&status);
    }
}

/* SDP completion handler: open the device when the record was found,
 * otherwise report the failure to the application. */
void bta_hh_sdp_cmpl(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    tBTA_HH_CONN conn;
    tBTA_HH_STATUS status;

    if (appl_trace_level >= 5)
        LogMsg_1(0x504, "bta_hh_sdp_cmpl:  status 0x%2X",
                 p_data->status);

    memset(&conn, 0, sizeof conn);
    conn.handle = p_cb->hid_handle;
    bdcpy(conn.bda, p_cb->addr);

    status = p_data->status;

    if (status == BTA_HH_OK) {
        if (!p_cb->incoming_conn) {
            /* set security level and open the connection */
            HID_HostSetSecurityLevel("\0\0\0\0\0\0\0", p_cb->sec_mask);

            if ((status = HID_HostOpenDev(p_cb->hid_handle)) !=
                HID_SUCCESS) {
                if (appl_trace_level >= 5)
                    LogMsg_1(0x504,
                             "bta_hh_sdp_cmpl:  HID_HostOpenDev failed:                     Status 0x%2X",
                             status);

                HID_HostRemoveDev(p_cb->hid_handle);
            }
        } else {
            /* open_cmpl_act will handle the connection */
            bta_hh_sm_execute(p_cb, BTA_HH_OPEN_CMPL_EVT, NULL);
        }
    }

    if (status != BTA_HH_OK) {
        /* SDP failed: report and tear down */
        if (p_data->status == BTA_HH_ERR_TOD_UNSPT)
            conn.status = BTA_HH_ERR_DB_FULL;
        else
            conn.status = BTA_HH_ERR_SDP;

        (*bta_hh_cb.p_cback)(BTA_HH_OPEN_EVT, (tBTA_HH *)&conn);

        bta_hh_sm_execute(p_cb, BTA_HH_API_CLOSE_EVT, NULL);

        if (p_cb->app_id == 0 && !p_cb->incoming_conn)
            bta_hh_clean_up_kdev(p_cb);

        bta_hh_trace_dev_db();
    }
}

void bta_hh_api_disc_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    tBTA_HH_CBDATA disc_dat;
    tHID_STATUS status;

    disc_dat.handle = p_data ? (UINT8)p_data->hdr.layer_specific
                             : p_cb->hid_handle;
    disc_dat.status = BTA_HH_ERR;

    status = HID_HostCloseDev(disc_dat.handle);

    if (status)
        (*bta_hh_cb.p_cback)(BTA_HH_CLOSE_EVT, (tBTA_HH *)&disc_dat);
}

void bta_hh_open_cmpl_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    tBTA_HH_CONN conn;
    UINT8 dev_handle = p_data ? (UINT8)p_data->hid_cback.hdr.layer_specific
                              : p_cb->hid_handle;

    memset(&conn, 0, sizeof conn);
    conn.handle = dev_handle;
    bdcpy(conn.bda, p_cb->addr);

    ++bta_hh_cb.cnt_num;

    bta_hh_co_open(p_cb->hid_handle, p_cb->sub_class, p_cb->attr_mask,
                   p_cb->app_id);

    bta_sys_conn_open(BTA_ID_HH, p_cb->app_id, p_cb->addr);

    if (p_cb->mode != BTA_HH_PROTO_RPT_MODE) {
        if ((conn.status = HID_HostWriteDev(dev_handle, HID_TRANS_SET_PROTOCOL,
                                            p_cb->mode, 0, 0, NULL)) !=
            HID_SUCCESS) {
            conn.status = BTA_HH_ERR_PROTO;

            (*bta_hh_cb.p_cback)(BTA_HH_OPEN_EVT, (tBTA_HH *)&conn);
        } else {
            p_cb->w4_evt = BTA_HH_OPEN_EVT;
        }
    } else {
        p_cb->opened = TRUE;

        (*bta_hh_cb.p_cback)(BTA_HH_OPEN_EVT, (tBTA_HH *)&conn);
    }

    p_cb->incoming_conn = FALSE;
}

void bta_hh_open_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    tBTA_HH_API_CONN conn_data;

    UINT8 dev_handle = p_data ? (UINT8)p_data->hid_cback.hdr.layer_specific
                              : p_cb->hid_handle;

    if (appl_trace_level >= 4)
        LogMsg_1(0x503, "bta_hh_open_act:  Device[%d] connected",
                 dev_handle);

    if (p_cb->app_id != 0) {
        bta_hh_sm_execute(p_cb, BTA_HH_OPEN_CMPL_EVT, p_data);
    } else {
        p_cb->incoming_conn = TRUE;

        memset(&conn_data, 0, sizeof conn_data);
        bdcpy(conn_data.bd_addr, p_cb->addr);

        bta_hh_start_sdp(p_cb, (tBTA_HH_DATA *)&conn_data);
    }
}

/* Map a HID handshake result code to a BTA HH status.  MWCC auto-inlines
 * this at every use site (no standalone copy). */
static tBTA_HH_STATUS bta_hh_get_trans_status(UINT32 result)
{
    switch (result) {
    case HID_PAR_HANDSHAKE_RSP_SUCCESS:
        return BTA_HH_OK;

    case HID_PAR_HANDSHAKE_RSP_NOT_READY:
    case HID_PAR_HANDSHAKE_RSP_ERR_INVALID_REP_ID:
    case HID_PAR_HANDSHAKE_RSP_ERR_UNSUPPORTED_REQ:
    case HID_PAR_HANDSHAKE_RSP_ERR_INVALID_PARAM:
        return (tBTA_HH_STATUS)result;

    default:
    case HID_PAR_HANDSHAKE_RSP_ERR_UNKNOWN:
    case HID_PAR_HANDSHAKE_RSP_ERR_FATAL:
        return BTA_HH_HS_ERROR;
    }
}

/* Handshake event from the HID host.  (Not part of the active batch; the
 * full body keeps this TU's string pool and jump tables in the retail
 * order so the sibling functions match byte-for-byte.) */
void bta_hh_handsk_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    tBTA_HH_CBDATA cback_data;
    tBTA_HH_HSDATA_RETAIL hs_data;
    tBTA_HH_CONN conn;

    if (appl_trace_level >= 5)
        LogMsg_2(0x504, "HANDSHAKE received for: event = %s data= %d",
                 (UINT32)bta_hh_get_w4_event(p_cb->w4_evt),
                 p_data->hid_cback.data);

    switch (p_cb->w4_evt) {
    case BTA_HH_GET_PROTO_EVT:
    case BTA_HH_GET_RPT_EVT:
    case BTA_HH_GET_IDLE_EVT:
        memset(&hs_data, 0, sizeof hs_data);
        hs_data.handle = p_cb->hid_handle;
        hs_data.status = bta_hh_get_trans_status(p_data->hid_cback.data);

        (*bta_hh_cb.p_cback)(p_cb->w4_evt, (tBTA_HH *)&hs_data);

        p_cb->w4_evt = 0;
        break;

    case BTA_HH_SET_RPT_EVT:
    case BTA_HH_SET_PROTO_EVT:
    case BTA_HH_SET_IDLE_EVT:
        cback_data.handle = p_cb->hid_handle;
        cback_data.status = bta_hh_get_trans_status(p_data->hid_cback.data);

        (*bta_hh_cb.p_cback)(p_cb->w4_evt, (tBTA_HH *)&cback_data);

        p_cb->w4_evt = 0;
        break;

    case BTA_HH_OPEN_EVT:
        conn.status = p_data->hid_cback.data ? BTA_HH_ERR_PROTO : BTA_HH_OK;
        conn.handle = p_cb->hid_handle;
        bdcpy(conn.bda, p_cb->addr);

        (*bta_hh_cb.p_cback)(p_cb->w4_evt, (tBTA_HH *)&conn);

        bta_hh_trace_dev_db();

        p_cb->w4_evt = 0;

        if (conn.status == BTA_HH_OK)
            p_cb->opened = TRUE;
        break;

    default:
        if (appl_trace_level >= 5)
            LogMsg_0(0x504, "unknown transaction type");
        break;
    }

    bta_sys_idle(BTA_ID_HH, p_cb->app_id, p_cb->addr);
}

void bta_hh_ctrl_dat_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    BT_HDR *pdata = p_data->hid_cback.p_data;
    UINT8 *data = (UINT8 *)(pdata + 1) + pdata->offset;
    tBTA_HH_HSDATA_RETAIL hs_data;

    if (appl_trace_level >= 5)
        LogMsg_1(0x504, "Ctrl DATA received w4: event[%s]",
                 (UINT32)bta_hh_get_w4_event(p_cb->w4_evt));

    hs_data.status = BTA_HH_OK;
    hs_data.handle = p_cb->hid_handle;

    switch (p_cb->w4_evt) {
    case BTA_HH_GET_IDLE_EVT:
        hs_data.rsp_data.idle_rate = *data;
        break;

    case BTA_HH_GET_RPT_EVT:
        memcpy(&hs_data.rsp_data.rpt_data, pdata,
               sizeof hs_data.rsp_data.rpt_data);
        break;

    case BTA_HH_GET_PROTO_EVT:
        hs_data.rsp_data.proto_mode = (UINT8)*data;
        break;

    /* should not expect control DATA for SET_ transactions */
    case BTA_HH_SET_PROTO_EVT:
    case BTA_HH_SET_RPT_EVT:
    case BTA_HH_SET_IDLE_EVT:
    default:
        if (appl_trace_level >= 5)
            LogMsg_1(0x504,
                     "invalid  transaction type for DATA payload: 4_evt[%s]",
                     (UINT32)bta_hh_get_w4_event(p_cb->w4_evt));
        break;
    }

    bta_sys_busy(BTA_ID_HH, p_cb->app_id, p_cb->addr);
    bta_sys_idle(BTA_ID_HH, p_cb->app_id, p_cb->addr);

    (*bta_hh_cb.p_cback)(p_cb->w4_evt, (tBTA_HH *)&hs_data);

    p_cb->w4_evt = 0;
    utl_freebuf((void **)&pdata);
}

/* Add/remove a virtually-cabled device (BTA_HhAddDev / BTA_HhRemoveDev).
 * The sub_event at p_data->maint_dev.sub_event selects the operation; the
 * result is reported to the application with the same sub-event code. */
void bta_hh_maint_dev_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    tBTA_HH_DEV_INFO dev_info;
    UINT8 hdl;

    dev_info.status = BTA_HH_ERR;
    dev_info.handle = BTA_HH_INVALID_HANDLE;

    switch (p_data->maint_dev.sub_event) {
    case BTA_HH_ADD_DEV_EVT:
        bdcpy(dev_info.bda, p_data->maint_dev.bda);

        if (p_cb->hid_handle == BTA_HH_INVALID_HANDLE) {
            if (HID_HostAddDev(p_data->maint_dev.bda,
                               p_data->maint_dev.attr_mask,
                               &hdl) == HID_SUCCESS) {
                dev_info.status = BTA_HH_OK;
                dev_info.handle = hdl;

                bta_hh_add_device_to_list(p_cb, hdl,
                                          p_data->maint_dev.attr_mask, NULL,
                                          p_data->maint_dev.sub_class,
                                          p_data->maint_dev.app_id);

                bta_hh_cb.cb_index[hdl] = p_cb->index;
            }
        } else {
            dev_info.handle = p_cb->hid_handle;
            dev_info.status = BTA_HH_OK;
        }

        bta_hh_trace_dev_db();
        break;

    case BTA_HH_RMV_DEV_EVT:
        dev_info.handle = (UINT8)p_data->hdr.layer_specific;
        bdcpy(dev_info.bda, p_cb->addr);

        if (p_cb->state != BTA_HH_CONN_ST) {
            if (HID_HostRemoveDev(dev_info.handle) == HID_SUCCESS) {
                dev_info.status = BTA_HH_OK;
                bta_hh_clean_up_kdev(p_cb);
            }
        }
        break;

    default:
        if (appl_trace_level >= 5)
            LogMsg_0(0x504, "invalid command");
        break;
    }

    (*bta_hh_cb.p_cback)(p_data->maint_dev.sub_event,
                         (tBTA_HH *)&dev_info);
}

void bta_hh_get_dscp_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    (*bta_hh_cb.p_cback)(BTA_HH_GET_DSCP_EVT, (tBTA_HH *)&p_cb->dscp_info);
}

void bta_hh_get_acl_q_info(void)
{
    tBTA_HH_ACL_QUEUE_INFO get_queue_info;
    tBTA_HH_DEV_CB *p_cb;
    UINT8 num_links = 0;
    UINT8 i;
    tL2C_CCB *p_l2c_ccb;
    tL2C_LCB *p_l2c_lcb;

    memset(&get_queue_info, 0, sizeof get_queue_info);

    for (i = 0; i < BTA_HH_MAX_KNOWN; ++i) {
        p_cb = &bta_hh_cb.kdev[i];

        if (p_cb->in_use && p_cb->state == BTA_HH_CONN_ST) {
            get_queue_info.queue_info[num_links].handle = p_cb->hid_handle;

            if ((p_l2c_ccb = l2cu_find_ccb_by_cid(
                     NULL, hh_cb[p_cb->hid_handle].conn.intr_cid)) != NULL) {
                if (((p_l2c_lcb = p_l2c_ccb->p_lcb) != NULL) &&
                    p_l2c_lcb->in_use) {
                    get_queue_info.queue_info[num_links].num_queued =
                        p_l2c_lcb->link_xmit_data_q.count;
                    get_queue_info.queue_info[num_links].num_not_acked =
                        p_l2c_lcb->sent_not_acked;
                }
            }

            ++num_links;
        }
    }

    get_queue_info.num_avail_buffs = l2cb.controller_xmit_window;
    get_queue_info.num_buffs = l2cb.num_lm_acl_bufs;
    get_queue_info.num_links = num_links;

    (*bta_hh_cb.p_cback)(BTA_HH_GET_ACL_Q_EVT, (tBTA_HH *)&get_queue_info);
}

void bta_hh_write_dev_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    tBTA_HH_CBDATA cbdata = {BTA_HH_OK, 0};
    UINT16 event = p_data->api_sndcmd.t_type - BTA_HH_FST_BTE_TRANS_EVT +
                   BTA_HH_FST_TRANS_CB_EVT;
    tHID_STATUS hid_status;

    cbdata.handle = p_cb->hid_handle;

    if ((hid_status = HID_HostWriteDev(p_cb->hid_handle,
                                       p_data->api_sndcmd.t_type,
                                       p_data->api_sndcmd.param,
                                       p_data->api_sndcmd.data,
                                       p_data->api_sndcmd.rpt_id,
                                       p_data->api_sndcmd.p_data)) !=
        HID_SUCCESS) {
        if (appl_trace_level >= 1)
            LogMsg_1(0x500, "HID_HostWriteDev Error %d", hid_status);

        cbdata.status = BTA_HH_ERR;

        if (p_data->api_sndcmd.t_type != HID_TRANS_CONTROL &&
            p_data->api_sndcmd.t_type != HID_TRANS_DATA) {
            (*bta_hh_cb.p_cback)(event, (tBTA_HH *)&cbdata);
        } else if (p_data->api_sndcmd.param ==
                   BTA_HH_CTRL_VIRTUAL_CABLE_UNPLUG) {
            (*bta_hh_cb.p_cback)(BTA_HH_VC_UNPLUG_EVT, (tBTA_HH *)&cbdata);
        }
    } else {
        switch (p_data->api_sndcmd.t_type) {
        case HID_TRANS_SET_PROTOCOL:
        case HID_TRANS_GET_REPORT:
        case HID_TRANS_SET_REPORT:
        case HID_TRANS_GET_PROTOCOL:
        case HID_TRANS_GET_IDLE:
        case HID_TRANS_SET_IDLE:
            p_cb->w4_evt = event;
            break;

        case HID_TRANS_DATA:
        case HID_TRANS_CONTROL:
            /* no handshake event will be generated */
            /* if VC_UNPLUG is issued, set flag */
            if (p_data->api_sndcmd.param == BTA_HH_CTRL_VIRTUAL_CABLE_UNPLUG)
                p_cb->vp = TRUE;

            break;

        /* currently not expected */
        case HID_TRANS_DATAC:
        default:
            if (appl_trace_level >= 5)
                LogMsg_1(0x504, "bta_hh_write_dev_act:: cmd type = %d",
                         p_data->api_sndcmd.t_type);
            break;
        }

        if (p_data->api_sndcmd.t_type != HID_TRANS_CONTROL) {
            bta_sys_busy(BTA_ID_HH, p_cb->app_id, p_cb->addr);
            bta_sys_idle(BTA_ID_HH, p_cb->app_id, p_cb->addr);
        } else if (p_data->api_sndcmd.param == BTA_HH_CTRL_SUSPEND) {
            bta_sys_sco_close(BTA_ID_HH, p_cb->app_id, p_cb->addr);
        } else if (p_data->api_sndcmd.param == BTA_HH_CTRL_EXIT_SUSPEND) {
            bta_sys_busy(BTA_ID_HH, p_cb->app_id, p_cb->addr);
        }
    }
}

/* Close the connection to a HID device (BTA_HH_API_CLOSE_EVT /
 * HID_HDEV_EVT_CLOSE).  L2CAP failure reasons or a device that was not
 * opened close the HID host side directly; an opened device is closed
 * on the application side first. */
void bta_hh_close_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data)
{
    tBTA_HH_CONN conn;
    tBTA_HH_CBDATA cback_data = {BTA_HH_OK, BTA_HH_OK};
    tBTA_HH_EVT event = BTA_HH_CLOSE_EVT;
    UINT32 data = p_data->hid_cback.data;
    UINT8 xx;

    if (p_cb->vp)
        event = BTA_HH_VC_UNPLUG_EVT;

    cback_data.handle = p_cb->hid_handle;
    cback_data.status = (UINT8)p_data->hid_cback.data;

    if ((data & (HID_L2CAP_CONN_FAIL | HID_L2CAP_REQ_FAIL | HID_L2CAP_CFG_FAIL)) ||
        data == 0xf ||
        data == 0xe ||
        !p_cb->opened) {
        /* close the HID host connection and report OPEN_EVT */
        conn.handle = p_cb->hid_handle;
        conn.status = (data == 0xf) ? BTA_HH_ERR_AUTH_FAILED : BTA_HH_ERR;
        bdcpy(conn.bda, p_cb->addr);

        HID_HostCloseDev(p_cb->hid_handle);

        (*bta_hh_cb.p_cback)(BTA_HH_OPEN_EVT, (tBTA_HH *)&conn);
        bta_hh_trace_dev_db();
    } else {
        /* connection was opened: close the application side */
        bta_hh_co_close(p_cb->hid_handle, p_cb->app_id);
        bta_sys_conn_close(BTA_ID_HH, p_cb->app_id, p_cb->addr);

        --bta_hh_cb.cnt_num;
        p_cb->opened = FALSE;

        if (cback_data.status != BTA_HH_OK)
            cback_data.status = BTA_HH_ERR;

        (*bta_hh_cb.p_cback)(event, (tBTA_HH *)&cback_data);

        if (p_cb->vp) {
            HID_HostRemoveDev(p_cb->hid_handle);
            bta_hh_clean_up_kdev(p_cb);
        }

        bta_hh_trace_dev_db();

        p_cb->vp = FALSE;
        p_cb->w4_evt = 0;

        if (bta_hh_cb.cnt_num == 0 && bta_hh_cb.w4_disable) {
            tBTA_HH_STATUS status = BTA_HH_OK;

            if (HID_HostDeregister() != HID_SUCCESS)
                status = BTA_HH_ERR;

            for (xx = 0; xx < BTA_HH_MAX_KNOWN; xx++)
                utl_freebuf((void **)&bta_hh_cb.kdev[xx].dscp_info.dsc_list);

            utl_freebuf((void **)&bta_hh_cb.p_disc_db);

            (*bta_hh_cb.p_cback)(BTA_HH_DISABLE_EVT, (tBTA_HH *)&status);

            memset(&bta_hh_cb, 0, sizeof bta_hh_cb);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Data action (matched, retained for reference)                      */
/* ------------------------------------------------------------------ */

void bta_hh_data_act(void *p_cb, void *p_data)
{
    void *p_buf;
    unsigned short off;
    unsigned short len;
    unsigned char rpt;

    p_buf = *(void **)((char *)p_data + 0xc);
    off = ((BT_HDR *)p_buf)->offset;
    rpt = (unsigned char)(*(unsigned short *)((char *)p_data + 6));
    len = ((BT_HDR *)p_buf)->len;
    bta_hh_co_data(rpt, (char *)p_buf + off + 8, len,
                   *((unsigned char *)p_cb + 0x1b),
                   *((unsigned char *)p_cb + 0x13),
                   *((unsigned char *)p_cb + 0x15));
    utl_freebuf(&p_buf);
}

/* HID host event callback (registered via HID_HostRegister).  Maps the
 * HID host events onto BTA HH internal events and sends them to the BTA
 * HH state machine task. */
/* HID host event values as dispatched by bta_hh_cback.  The retail build
 * used this ordering, which differs from hidh_api.h's HID_HDEV_EVT_* enum
 * (there the open/close/data/handshake/ctrl/plugg events sit at other
 * indices).  Both the trace-name and the dispatch switch in the retail
 * function bound-check against 8, so the enum ramzes 0..8 below. */
enum {
    BTA_HH_CB_OPEN,          /* 0: connected */
    BTA_HH_CB_CLOSE,         /* 1: closed */
    BTA_HH_CB_INTR_DATA,     /* 2: interrupt data */
    BTA_HH_CB_HANDSHAKE,     /* 3: handshake */
    BTA_HH_CB_CTRL_DATA,     /* 4: control data */
    BTA_HH_CB_INTR_DATC,     /* 5: data channel tx complete (no event) */
    BTA_HH_CB_VC_UNPLUG,     /* 6: virtual cable unplug */
    BTA_HH_CB_RETRYING,      /* 7: reconnecting (no event) */
    BTA_HH_CB_CTRL_DATC,     /* 8: ctrl channel tx complete (no event) */
};

void bta_hh_cback(UINT8 dev_handle, UINT8 event, UINT32 data, BT_HDR *p_data)
{
    tBTA_HH_EVT bta_event = BTA_HH_INVALID_EVT;
    BT_HDR *p_buf;
    UINT8 xx;

    if (appl_trace_level >= 5) {
        const char *evt_str;

        switch (event) {
        case BTA_HH_CB_OPEN:
            evt_str = "HID_HDEV_EVT_OPEN";
            break;

        case BTA_HH_CB_CLOSE:
            evt_str = "HID_HDEV_EVT_CLOSE";
            break;

        case BTA_HH_CB_INTR_DATA:
            evt_str = "HID_HDEV_EVT_INTR_DATA";
            break;

        case BTA_HH_CB_HANDSHAKE:
            evt_str = "HID_HDEV_EVT_HANDSHAKE";
            break;

        case BTA_HH_CB_CTRL_DATA:
            evt_str = "HID_HDEV_EVT_CTRL_DATA";
            break;

        case BTA_HH_CB_INTR_DATC:
            evt_str = "HID_HDEV_EVT_INTR_DATC";
            break;

        case BTA_HH_CB_VC_UNPLUG:
            evt_str = "HID_HDEV_EVT_VC_UNPLUG";
            break;

        case BTA_HH_CB_RETRYING:
            evt_str = "HID_HDEV_EVT_RETRYING";
            break;

        case BTA_HH_CB_CTRL_DATC:
            evt_str = "HID_HDEV_EVT_CTRL_DATC";
            break;

        default:
            evt_str = "Unknown HID event";
            break;
        }

        LogMsg_1(0x504, "bta_hh_cback::HID_event [%s]", (UINT32)evt_str);
    }

    switch (event) {
    case BTA_HH_CB_OPEN:
        bta_event = BTA_HH_INT_OPEN_EVT;
        break;

    case BTA_HH_CB_CLOSE:
        bta_event = BTA_HH_INT_CLOSE_EVT;
        break;

    case BTA_HH_CB_INTR_DATA:
        bta_event = BTA_HH_INT_DATA_EVT;
        break;

    case BTA_HH_CB_HANDSHAKE:
        bta_event = BTA_HH_INT_HANDSK_EVT;
        break;

    case BTA_HH_CB_CTRL_DATA:
        bta_event = BTA_HH_INT_CTRL_DATA;
        break;

    case BTA_HH_CB_INTR_DATC:
        utl_freebuf((void **)&p_data);
        break;

    case BTA_HH_CB_VC_UNPLUG:
        for (xx = 0; xx < BTA_HH_MAX_KNOWN; xx++) {
            if (dev_handle == bta_hh_cb.kdev[xx].hid_handle) {
                bta_hh_cb.kdev[xx].vp = TRUE;
                break;
            }
        }
        break;

    case BTA_HH_CB_RETRYING:
    case BTA_HH_CB_CTRL_DATC:
    default:
        break;
    }

    if (bta_event != BTA_HH_INVALID_EVT) {
        if ((p_buf = (BT_HDR *)GKI_getbuf(0x18)) != NULL) {
            p_buf->event = bta_event;
            p_buf->layer_specific = dev_handle;
            ((tBTA_HH_CBACK_DATA *)p_buf)->data = data;
            ((tBTA_HH_CBACK_DATA *)p_buf)->p_data = p_data;
            bta_sys_sendmsg(p_buf);
        }
    }
}
