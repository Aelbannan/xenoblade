// Decompiled from RVL_SDK/src/revolution/bte/stack/btu/btu_hcif
// HCI event handlers that route connection-related events to BTM / L2CAP.

#include <harness_catalog.h>
#include <string.h>

/* Minimal BTE type definitions (mirroring bt_types.h / btm_api.h). */
typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned int   UINT32;
typedef int            INT32;

#define BD_ADDR_LEN   6
typedef UINT8 BD_ADDR[BD_ADDR_LEN];     /* Device address */
#define BD_NAME_LEN 248
typedef UINT8 BD_NAME[BD_NAME_LEN];       /* Device name */

#define LINK_KEY_LEN  16
typedef UINT8 LINK_KEY[LINK_KEY_LEN];   /* Link key */

#define DEV_CLASS_LEN 3
typedef UINT8 DEV_CLASS[DEV_CLASS_LEN]; /* Device class */

/* HCI link types (hcidefs.h). */
#define HCI_LINK_TYPE_SCO  0x00
#define HCI_LINK_TYPE_ACL  0x01

/* HCI command opcodes / BTU timer types referenced by the handlers
   (hcidefs.h / btu.h). Kept as local mirrors because the public headers
   carry a different btu_cb layout than the retail image. */
#define HCI_RESET                  0x0C03
#define HCI_HOST_NUM_PACKETS_DONE  0x0C35
#define HCI_COMMAND_NONE           0x0000
#define BTU_TTYPE_BTU_CMD_CMPL     60

/* Connection handle mask (hcimsgs.h): keep only the 12-bit handle field. */
#define HCI_DATA_HANDLE_MASK 0x0FFF
#define HCID_GET_HANDLE(u16) ((UINT16)((u16) & HCI_DATA_HANDLE_MASK))

/* HCI event stream extraction macros (bt_types.h). The BD_ADDR / DEV_CLASS
   copies are stored with the source byte order reversed into the local
   struct so the internal representation matches the HCI big-endian layout. */
#define STREAM_TO_UINT8(u8, p)   { (u8) = (UINT8)(*(p)); (p) += 1; }
#define STREAM_TO_UINT16(u16, p) { (u16) = (UINT16)((UINT16)(*(p)) + (((UINT16)(*((p) + 1))) << 8)); (p) += 2; }
#define STREAM_TO_BDADDR(a, p)   \
    { register int ijk; register UINT8 *pbda = (UINT8 *)(a) + BD_ADDR_LEN - 1; for (ijk = 0; ijk < BD_ADDR_LEN; ijk++) *pbda-- = *p++; }
#define STREAM_TO_DEVCLASS(a, p) \
    { register int ijk; register UINT8 *_pa = (UINT8 *)(a) + DEV_CLASS_LEN - 1; for (ijk = 0; ijk < DEV_CLASS_LEN; ijk++) *_pa-- = *p++; }
/* 16-byte link key extraction: stored with the source byte order reversed,
   matching the big-endian layout used for BD_ADDR. */
#define STREAM_TO_ARRAY16(a, p) \
    { register int ijk; register UINT8 *_pa = (UINT8 *)(a) + 15; for (ijk = 0; ijk < 16; ijk++) *_pa-- = *p++; }

/* Little-endian 32-bit extraction from the HCI event stream. */
#define STREAM_TO_UINT32(u32, p) { (u32) = (UINT32)((UINT32)(*(p)) + ((((UINT32)(*((p) + 1)))) << 8) + ((((UINT32)(*((p) + 2)))) << 16) + ((((UINT32)(*((p) + 3)))) << 24)); (p) += 4; }

/* SCO/ESCO connection-complete data (btm_api.h). bd_addr sits at offset 4,
   giving the struct a total size of 14 bytes. */
typedef struct {
    UINT16  rx_pkt_len;
    UINT16  tx_pkt_len;
    BD_ADDR bd_addr;
    UINT8   link_type;  /* BTM_LINK_TYPE_SCO or BTM_LINK_TYPE_ESCO */
    UINT8   tx_interval;
    UINT8   retrans_window;
    UINT8   air_mode;
} tBTM_ESCO_DATA;

/* QoS flow specification (bt_types.h). Note the two 8-bit fields are
   followed by 2 bytes of struct padding before the first 32-bit field,
   matching the HCI QoS Setup Complete event layout. */
typedef struct {
    UINT8   qos_flags;
    UINT8   service_type;
    UINT32  token_rate;
    UINT32  token_bucket_size;
    UINT32  peak_bandwidth;
    UINT32  latency;
    UINT32  delay_variation;
} FLOW_SPEC;

/* Header of each buffer used in the Bluetooth stack (bt_types.h). */
typedef struct {
    UINT16  event;
    UINT16  len;
    UINT16  offset;
    UINT16  layer_specific;
} BT_HDR;

/* ------------------------------------------------------------------
** Local mirrors of the BTU control block / buffer types.
**
** The retail image uses a FLAT btu_cb (no per-controller command
** queue array), so the member offsets below are taken from the
** Wii-era layout rather than the hci_cmd_cb[] struct in the public
** btu.h. The offsets are verified against this TU's HCI handlers
** (cmd_cmpl_timer @ 0x4c, cmd_cmpl_q @ 0x70, controller_cmd_window
** @ 0x80).
** ------------------------------------------------------------------ */
typedef UINT8 BOOLEAN;

typedef struct _tle {
    struct _tle  *p_next;
    struct _tle  *p_prev;
    void         *p_cback;
    INT32         ticks;
    void         *param;
    UINT16        event;
    UINT8         in_use;
} TIMER_LIST_ENT;

typedef struct {
    TIMER_LIST_ENT  *p_first;
    TIMER_LIST_ENT  *p_last;
    INT32            last_ticks;
} TIMER_LIST_Q;

typedef struct {
    void    *p_first;
    void    *p_last;
    UINT16   count;
} BUFFER_Q;

typedef struct {
    TIMER_LIST_ENT  *p_tle;
    void            *timer_cb;
} tBTU_TIMER_REG;

typedef struct {
    UINT16  event_range;
    void   *event_cb;
} tBTU_EVENT_REG;

typedef struct {
    tBTU_TIMER_REG   timer_reg[2];
    tBTU_EVENT_REG   event_reg[6];
    TIMER_LIST_Q     timer_queue;
    TIMER_LIST_ENT   cmd_cmpl_timer;         /* offset 0x4c */
    BUFFER_Q         cmd_xmit_q;
    BUFFER_Q         cmd_cmpl_q;             /* offset 0x70 */
    UINT16           hcit_acl_data_size;
    UINT16           hcit_acl_pkt_size;
    UINT16           controller_cmd_window;  /* offset 0x80 */
    UINT8            reset_complete;
} tBTU_CB;

extern tBTU_CB  btu_cb;

/* Local mirror of the BTM control block (retail layout). Only the device
   state byte at offset 0x64E is referenced by this TU (the startup
   "waiting for reset complete" check before priming the command window). */
typedef struct {
    UINT8  reserved[0x64E];
    UINT8  devcb_state;      /* 0x64E: btm_cb.devcb.state */
} tBTM_CB;

extern tBTM_CB  btm_cb;

/* GKI buffer-queue helpers (gki.h). */
void   *GKI_dequeue (BUFFER_Q *);
void    GKI_enqueue_head (BUFFER_Q *, void *);
void    GKI_enqueue (BUFFER_Q *, void *);
void   *GKI_getpoolbuf (UINT8 pool_id);
BOOLEAN GKI_queue_is_empty (BUFFER_Q *);
void    GKI_freebuf (void *);

/* BTU timer helpers (btu.h). */
void btu_start_timer (TIMER_LIST_ENT *p_tle, UINT16 type, UINT32 timeout);
void btu_stop_timer  (TIMER_LIST_ENT *p_tle);

/* QoS setup completion (btm_int.h). */
void btm_qos_setup_complete (UINT8 status, UINT16 handle, FLOW_SPEC *p_flow);

/* Callees defined in other BTE translation units. */
void btm_sec_conn_req(BD_ADDR bda, DEV_CLASS dc);
void btm_sco_conn_req(BD_ADDR bda, DEV_CLASS dc, UINT8 link_type);
void btm_sec_connected(BD_ADDR bda, UINT16 handle, UINT8 status, UINT8 enc_mode);
void btm_sco_connected(UINT8 hci_status, BD_ADDR bda, UINT16 hci_handle, tBTM_ESCO_DATA *p_esco_data);
void btm_sec_link_key_notification(BD_ADDR bda, LINK_KEY key, UINT8 key_type);
void l2c_link_hci_conn_comp(UINT8 status, UINT16 handle, BD_ADDR bda);
void btm_pm_proc_cmd_status(UINT8 status);
void btm_process_remote_name(BD_ADDR bda, BD_NAME name, UINT16 evt_len, UINT8 hci_status);
void btm_sec_rmt_name_request_complete(UINT8* bd_addr, UINT8* bd_name, UINT8 status);
void btm_acl_role_changed(UINT8 hci_status, BD_ADDR bd_addr, UINT8 new_role);
BOOLEAN btm_is_sco_active(UINT16 handle);
void btm_esco_proc_conn_chg(UINT8 status, UINT16 handle, UINT8 tx_interval,
                            UINT8 retrans_window, UINT16 rx_pkt_len,
                            UINT16 tx_pkt_len);
void l2c_link_role_change_failed(void);

/* HCI command sent to the lower layer (bte_hcisu.c). */
void bte_hcisu_send (BT_HDR *p_msg, UINT16 event);
#define BT_EVT_TO_BTU_HCI_CMD  0x2000  /* HCI Command */

/* Remaining catalog stubs (handlers not yet decompiled). */
void btu_hcif_process_event() {}

/*******************************************************************************
**
** Function         btu_hcif_send_cmd
**
** Description      This function is called to check if it can send commands
**                  to the Host Controller. It may be passed the address of
**                  a packet to send.
**
** Returns          void
**
*******************************************************************************/
void btu_hcif_send_cmd(BT_HDR *p_buf)
{
    UINT16  opcode;
    BT_HDR *p_buf2;
    UINT8  *pp;

    /* If there are already commands in the queue, then enqueue this command */
    if ((p_buf) && (btu_cb.cmd_xmit_q.count))
    {
        GKI_enqueue (&btu_cb.cmd_xmit_q, p_buf);
        p_buf = NULL;
    }

    /* Allow for startup case, where no acks may be received */
    if ((!btu_cb.controller_cmd_window) && (!btm_cb.devcb_state))
    {
        btu_cb.controller_cmd_window = btu_cb.cmd_xmit_q.count + 1;
    }

    /* See if we can send anything */
    while (btu_cb.controller_cmd_window != 0)
    {
        if (!p_buf)
            p_buf = (BT_HDR *)GKI_dequeue (&btu_cb.cmd_xmit_q);

        if (!p_buf)
            break;

        /* Check if the command does not get a response */
        pp = (UINT8 *)(p_buf + 1) + p_buf->offset;
        STREAM_TO_UINT16 (opcode, pp);

        if ((opcode != HCI_RESET) && (opcode != HCI_HOST_NUM_PACKETS_DONE))
        {
            /* Save the command so we can issue a command complete event */
            if ((p_buf2 = (BT_HDR *)GKI_getpoolbuf (2)) != NULL)
            {
                memcpy (p_buf2, p_buf, sizeof (BT_HDR));

                memcpy ((UINT8 *)(p_buf2 + 1) + p_buf2->offset,
                        (UINT8 *)(p_buf + 1) + p_buf->offset,
                        p_buf->len);

                GKI_enqueue (&btu_cb.cmd_cmpl_q, p_buf2);
                btu_start_timer (&btu_cb.cmd_cmpl_timer, BTU_TTYPE_BTU_CMD_CMPL, 8);
            }
        }

        btu_cb.controller_cmd_window--;
        bte_hcisu_send (p_buf, BT_EVT_TO_BTU_HCI_CMD);
        p_buf = NULL;
    }

    if (p_buf)
        GKI_enqueue (&btu_cb.cmd_xmit_q, p_buf);
}
void btu_hcif_qos_setup_comp_evt(UINT8 *p, UINT16 evt_len)
{
    UINT8     status;
    UINT16    handle;
    FLOW_SPEC flow;

    /* status (p[0]), the connection handle (p[1..2], little-endian) and the
       five 32-bit flow-spec fields follow the two 8-bit qualifiers. The
       FLOW_SPEC struct carries 2 bytes of padding after service_type, so the
       32-bit fields line up with the event layout. */
    STREAM_TO_UINT8  (status, p);
    STREAM_TO_UINT16 (handle, p);
    STREAM_TO_UINT8  (flow.qos_flags, p);
    STREAM_TO_UINT8  (flow.service_type, p);
    STREAM_TO_UINT32 (flow.token_rate, p);
    STREAM_TO_UINT32 (flow.peak_bandwidth, p);
    STREAM_TO_UINT32 (flow.latency, p);
    STREAM_TO_UINT32 (flow.delay_variation, p);

    btm_qos_setup_complete (status, handle, &flow);
}
/*******************************************************************************
**
** Function         btu_hcif_esco_connection_comp_evt
**
** Description      Process HCI_ESCO_CONNECTION_COMP_EVT. An eSCO/SCO connection
**                  has completed; route the result (including the eSCO
**                  parameters) to the SCO manager.
**
** Returns          void
**
*******************************************************************************/
void btu_hcif_esco_connection_comp_evt(UINT8 *p, UINT16 evt_len)
{
    tBTM_ESCO_DATA  esco_data;
    UINT16          handle;
    BD_ADDR         bda;
    UINT8           status;

    /* status (p[0]), connection handle (p[1..2], little-endian) and the
       device address (p[3..8]) are extracted straight from the event. The
       bd_addr is stored with its byte order reversed (big-endian) to match
       the internal BD_ADDR representation. */
    STREAM_TO_UINT8  (status, p);
    STREAM_TO_UINT16 (handle, p);
    STREAM_TO_BDADDR (bda, p);

    /* Remaining eSCO parameters follow the link key in the event layout. */
    STREAM_TO_UINT8  (esco_data.link_type, p);
    STREAM_TO_UINT8  (esco_data.tx_interval, p);
    STREAM_TO_UINT8  (esco_data.retrans_window, p);
    STREAM_TO_UINT16 (esco_data.rx_pkt_len, p);
    STREAM_TO_UINT16 (esco_data.tx_pkt_len, p);
    STREAM_TO_UINT8  (esco_data.air_mode, p);

    /* Copy the reversed BD_ADDR into the eSCO data block before notifying. */
    memcpy (esco_data.bd_addr, bda, BD_ADDR_LEN);

    btm_sco_connected (status, bda, handle, &esco_data);
}
/* Command-completion handlers in BTM (btm_acl / btm_inq / btm_devctl / btm_dev). */
extern void btm_reset_complete(void);
extern void btm_process_inq_complete(UINT8 status);
extern void btm_event_filter_complete(UINT8* p);
extern void btm_read_stored_link_key_complete(UINT8* p);
extern void btm_write_stored_link_key_complete(UINT8* p);
extern void btm_delete_stored_link_key_complete(UINT8* p);
extern void btm_read_link_policy_complete(UINT8* p);
extern void btm_read_hci_buf_size_complete(UINT8* p, UINT16 evt_len);
extern void btm_read_local_version_complete(UINT8* p, UINT16 evt_len);
extern void btm_read_local_features_complete(UINT8* p, UINT16 evt_len);
extern void btm_read_local_name_complete(UINT8* p, UINT16 evt_len);
extern void btm_read_local_addr_complete(UINT8* p, UINT16 evt_len);
extern void btm_read_link_quality_complete(UINT8* p);
extern void btm_read_rssi_complete(UINT8* p);
extern void btm_vsc_complete(UINT8* p, UINT16 cc_opcode, UINT16 evt_len);

/* HCI command opcodes with completion handlers in this dispatch (hcidefs.h). */
#define HCI_INQUIRY_CANCEL          0x0402
#define HCI_READ_LINK_POLICY        0x080C
#define HCI_SET_EVENT_FILTER        0x0C05
#define HCI_READ_STORED_LINK_KEY    0x0C0D
#define HCI_WRITE_STORED_LINK_KEY   0x0C11
#define HCI_DELETE_STORED_LINK_KEY  0x0C12
#define HCI_READ_LOCAL_NAME         0x0C14
#define HCI_READ_LOCAL_VERSION      0x1001
#define HCI_READ_LOCAL_FEATURES     0x1003
#define HCI_READ_BUFFER_SIZE        0x1005
#define HCI_READ_BD_ADDR            0x1009
#define HCI_READ_LINK_QUALITY       0x1403
#define HCI_READ_RSSI               0x1405
#define HCI_GRP_VENDOR_SPECIFIC     0xFC00

void btu_hcif_hdl_command_complete(UINT16 opcode, UINT8 *p, UINT16 evt_len)
{
    switch (opcode)
    {
    case HCI_RESET:
        btm_reset_complete();
        break;

    case HCI_INQUIRY_CANCEL:
        btm_process_inq_complete(0);
        break;

    case HCI_SET_EVENT_FILTER:
        btm_event_filter_complete(p);
        break;

    case HCI_READ_STORED_LINK_KEY:
        btm_read_stored_link_key_complete(p);
        break;

    case HCI_WRITE_STORED_LINK_KEY:
        btm_write_stored_link_key_complete(p);
        break;

    case HCI_DELETE_STORED_LINK_KEY:
        btm_delete_stored_link_key_complete(p);
        break;

    case HCI_READ_LOCAL_VERSION:
        btm_read_local_version_complete(p, evt_len);
        break;

    case HCI_READ_LINK_POLICY:
        btm_read_link_policy_complete(p);
        break;

    case HCI_READ_BUFFER_SIZE:
        btm_read_hci_buf_size_complete(p, evt_len);
        break;

    case HCI_READ_LOCAL_FEATURES:
        btm_read_local_features_complete(p, evt_len);
        break;

    case HCI_READ_LOCAL_NAME:
        btm_read_local_name_complete(p, evt_len);
        break;

    case HCI_READ_BD_ADDR:
        btm_read_local_addr_complete(p, evt_len);
        break;

    case HCI_READ_LINK_QUALITY:
        btm_read_link_quality_complete(p);
        break;

    case HCI_READ_RSSI:
        btm_read_rssi_complete(p);
        break;

    default:
        if ((opcode & HCI_GRP_VENDOR_SPECIFIC) == HCI_GRP_VENDOR_SPECIFIC)
            btm_vsc_complete(p, opcode, evt_len);
        break;
    }
}

void btu_hcif_command_complete_evt(UINT8 *p, UINT16 evt_len)
{
    UINT16  cc_opcode;
    BT_HDR *p_cmd;
    UINT8  *p_dequeued;
    UINT16  queued_opcode;

    /* The first event byte reports how many commands the controller can
       still accept; the next two bytes are the completed command opcode. */
    btu_cb.controller_cmd_window = p[0];
    cc_opcode = (UINT16)(p[1] + (p[2] << 8));

    /* Only a handful of commands leave a pending command queued that must be
       dequeued and freed when its completion arrives. */
    if ((cc_opcode != HCI_RESET) &&
        (cc_opcode != HCI_HOST_NUM_PACKETS_DONE) &&
        (cc_opcode != HCI_COMMAND_NONE))
    {
        p_cmd = (BT_HDR *)GKI_dequeue (&btu_cb.cmd_cmpl_q);
        if (p_cmd)
        {
            p_dequeued = (UINT8 *)(p_cmd + 1);
            queued_opcode = (UINT16)(p_dequeued[0] + (p_dequeued[1] << 8));

            /* A vendor-specific command whose opcode does not match the one
               just completed is re-queued rather than freed. */
            if ((cc_opcode == 0xFC4C) && (queued_opcode != cc_opcode))
            {
                GKI_enqueue_head (&btu_cb.cmd_cmpl_q, p_cmd);
                return;
            }

            GKI_freebuf (p_cmd);
        }

        /* Restart or stop the command-complete timeout timer depending on
           whether queued commands remain. */
        if (!GKI_queue_is_empty (&btu_cb.cmd_cmpl_q))
            btu_start_timer (&btu_cb.cmd_cmpl_timer, BTU_TTYPE_BTU_CMD_CMPL, 8);
        else
            btu_stop_timer (&btu_cb.cmd_cmpl_timer);
    }

    /* Dispatch the completion and try to send the next queued command. */
    btu_hcif_hdl_command_complete (cc_opcode, p + 3, evt_len - 3);
    btu_hcif_send_cmd (0);
}
extern void LogMsg_2(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2);

void btu_hcif_hdl_command_status(UINT16 opcode, UINT8 num_hci_cmds, UINT8 *p_data)
{
    switch (opcode)
    {
    case 0x0801:   /* HCI_READ_LOCAL_VERSION_INFORMATION */
    case 0x0803:   /* HCI_READ_LOCAL_SUPPORTED_FEATURES */
    case 0x0804:   /* HCI_READ_LOCAL_EXTENDED_FEATURES */
    case 0x0805:   /* HCI_READ_BUFFER_SIZE */
    case 0x0806:   /* HCI_READ_BD_ADDR */
        btm_pm_proc_cmd_status(num_hci_cmds);
        return;
    default:
        break;
    }

    if (num_hci_cmds == 0)
        return;

    switch (opcode)
    {
    case 0x0401:   /* HCI_INQUIRY */
        btm_process_inq_complete(num_hci_cmds);
        break;

    case 0x0419:   /* HCI_REMOTE_NAME_REQUEST */
        btm_process_remote_name(0, 0, 0, num_hci_cmds);
        btm_sec_rmt_name_request_complete(0, 0, num_hci_cmds);
        break;

    case 0x000D:   /* HCI_QOS_SETUP */
        btm_qos_setup_complete(num_hci_cmds, 0, 0);
        break;

    case 0x080B:   /* HCI_READ_LINK_POLICY_SETTINGS */
        btm_acl_role_changed(num_hci_cmds, 0, 0xFF);
        l2c_link_role_change_failed();
        break;

    case 0x0405:   /* HCI_CREATE_CONNECTION */
        if (p_data != NULL)
        {
            BD_ADDR bd_addr;

            bd_addr[5] = p_data[1];
            bd_addr[4] = p_data[2];
            bd_addr[3] = p_data[3];
            bd_addr[2] = p_data[4];
            bd_addr[1] = p_data[5];
            bd_addr[0] = p_data[6];

            btm_sec_connected(bd_addr, 0xFFFF, num_hci_cmds, 0);
            l2c_link_hci_conn_comp(num_hci_cmds, 0xFFFF, bd_addr);
        }
        break;

    case 0x0428:   /* HCI_ADD_SCO_CONNECTION */
        if (p_data != NULL)
        {
            UINT16 handle = (UINT16)(p_data[1] + (p_data[2] << 8));

            if (btm_is_sco_active(handle))
                btm_esco_proc_conn_chg(num_hci_cmds, handle, 0, 0, 0, 0);
            else
            {
                tBTM_ESCO_DATA esco_data;
                btm_sco_connected(num_hci_cmds, 0, handle, &esco_data);
            }
        }
        break;

    default:
        if ((opcode & 0xFC00) == 0xFC00)
            btm_vsc_complete(p_data, opcode, 1);
        break;
    }
}
void btu_hcif_command_status_evt(UINT8 *p, UINT16 evt_len)
{
    UINT8   num_hci_cmds;
    UINT8   status;
    UINT16  opcode;
    UINT8  *p_data;
    BT_HDR *p_cmd;
    UINT16  queued_opcode;

    /* HCI Command Status: [0] = commands still allowed, [1] = status,
       [2..3] = opcode of the command the controller just accepted. */
    num_hci_cmds = p[0];
    status = p[1];

    p_cmd = NULL;
    p_data = NULL;

    btu_cb.controller_cmd_window = status;
    opcode = (UINT16)((UINT16)p[2] + ((UINT16)p[3] << 8));

    /* Commands that never produce a completion do not leave a queued copy. */
    if ((opcode != HCI_RESET) &&
        (opcode != HCI_HOST_NUM_PACKETS_DONE) &&
        (opcode != HCI_COMMAND_NONE))
    {
        p_cmd = (BT_HDR *)GKI_dequeue (&btu_cb.cmd_cmpl_q);
        if (p_cmd)
        {
            UINT8 *p_dequeued = (UINT8 *)(p_cmd + 1) + p_cmd->offset;
            queued_opcode = (UINT16)((UINT16)p_dequeued[0] + ((UINT16)p_dequeued[1] << 8));
            p_data = p_dequeued + 2;

            if (queued_opcode != opcode)
            {
                p_data = NULL;
                LogMsg_2(0x70001, "Event mismatch opcode=%X cmd opcode=%X",
                         opcode, queued_opcode);
            }
        }

        /* Restart or stop the command timeout timer depending on whether
           commands remain queued for completion. */
        if (!GKI_queue_is_empty (&btu_cb.cmd_cmpl_q))
            btu_start_timer (&btu_cb.cmd_cmpl_timer, BTU_TTYPE_BTU_CMD_CMPL, 8);
        else
            btu_stop_timer (&btu_cb.cmd_cmpl_timer);
    }

    /* Dispatch the status; the queued copy (if any) carried the command
       parameters that accompany the status payload. */
    btu_hcif_hdl_command_status (opcode, num_hci_cmds, p_data);
    if (p_cmd)
        GKI_freebuf (p_cmd);

    btu_hcif_send_cmd (0);
}
extern void LogMsg_0(UINT32 trace_set_mask, const char *fmt_str);
extern void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void btm_report_device_status(UINT8 status);

void btu_hcif_cmd_timeout(void)
{
    BT_HDR  *p_cmd;
    UINT8   *p;
    UINT16   opcode;

    btu_cb.controller_cmd_window = 1;

    p_cmd = (BT_HDR *)GKI_dequeue(&btu_cb.cmd_cmpl_q);
    if (p_cmd == NULL)
    {
        LogMsg_0(0x70001, "Cmd timeout; no cmd in queue");
        return;
    }

    if (!GKI_queue_is_empty(&btu_cb.cmd_cmpl_q))
        btu_start_timer(&btu_cb.cmd_cmpl_timer, BTU_TTYPE_BTU_CMD_CMPL, 8);

    p = (UINT8 *)(p_cmd + 1) + p_cmd->offset;

    opcode = (UINT16)(p[0] + (p[1] << 8));
    LogMsg_1(0x70001, "BTU HCI command timeout - cmd opcode = 0x%02x", opcode);

    switch (opcode)
    {
    case 0x000D:
    case 0x0401:
    case 0x0405:
    case 0x0419:
    case 0x0801:
    case 0x0803:
    case 0x0804:
    case 0x0805:
    case 0x0806:
        btu_hcif_hdl_command_status(opcode, 0x1F, p + 2);
        break;

    default:
        {
            UINT8 status = 0x1F;
            btu_hcif_hdl_command_complete(opcode, &status, 1);
        }
        break;
    }

    GKI_freebuf(p_cmd);
    btm_report_device_status(2);
    btu_hcif_send_cmd(0);
}

/*******************************************************************************
**
** Function         btu_hcif_link_key_notification_evt
**
** Description      Process HCI_LINK_KEY_NOTIFICATION_EVT. A link key has been
**                  generated/updated for a device; forward it to the security
**                  manager along with the key type.
**
** Returns          void
**
*******************************************************************************/
void btu_hcif_link_key_notification_evt(UINT8 *p, UINT16 evt_len)
{
    BD_ADDR   bda;
    LINK_KEY  key;
    UINT8     key_type;

    STREAM_TO_BDADDR (bda, p);            /* p[0..5]  */
    STREAM_TO_ARRAY16 (key, p);           /* p[6..21] (byte order reversed) */
    STREAM_TO_UINT8  (key_type, p);       /* p[22]    */

    btm_sec_link_key_notification (bda, key, key_type);
}

/*******************************************************************************
**
** Function         btu_hcif_connection_request_evt
**
** Description      Process HCI_CONNECTION_REQUEST_EVT. The remote device is
**                  asking to connect; route the request through the security
**                  manager (ACL) or SCO manager depending on the link type.
**
** Returns          void
**
*******************************************************************************/
void btu_hcif_connection_request_evt(UINT8 *p, UINT16 evt_len)
{
    BD_ADDR   bda;
    DEV_CLASS dc;
    UINT8     link_type;

    STREAM_TO_BDADDR (bda, p);
    STREAM_TO_DEVCLASS (dc, p);
    STREAM_TO_UINT8  (link_type, p);

    /* Pass the request to the security manager to check connect filters
       before forwarding to L2CAP. */
    if (link_type == HCI_LINK_TYPE_ACL)
    {
        btm_sec_conn_req (bda, dc);
    }
    else
    {
        btm_sco_conn_req (bda, dc, link_type);
    }
}

/*******************************************************************************
**
** Function         btu_hcif_connection_comp_evt
**
** Description      Process HCI_CONNECTION_COMP_EVT. A connection has been
**                  established (or failed); notify BTM and L2CAP (ACL) or the
**                  SCO manager.
**
** Returns          void
**
*******************************************************************************/
void btu_hcif_connection_comp_evt(UINT8 *p, UINT16 evt_len)
{
    UINT8           status;
    UINT16          handle;
    BD_ADDR         bda;
    UINT8           link_type;
    UINT8           enc_mode;
    tBTM_ESCO_DATA  esco_data;

    STREAM_TO_UINT8  (status, p);
    STREAM_TO_UINT16 (handle, p);
    STREAM_TO_BDADDR (bda, p);
    STREAM_TO_UINT8  (link_type, p);
    STREAM_TO_UINT8  (enc_mode, p);

    handle = HCID_GET_HANDLE (handle);

    if (link_type == HCI_LINK_TYPE_ACL)
    {
        btm_sec_connected (bda, handle, status, enc_mode);

        l2c_link_hci_conn_comp (status, handle, bda);
    }
    else
    {
        /* For SCO/ESCO the link type member is already 0 (SCO) after the
           zero-init; only the BD_ADDR needs copying in. */
        memset (&esco_data, 0, sizeof(tBTM_ESCO_DATA));
        memcpy (esco_data.bd_addr, bda, BD_ADDR_LEN);
        btm_sco_connected (status, bda, handle, &esco_data);
    }
}
