// Decompiled btm_sec.c -- high-level C reconstruction of the BTM security manager.
// Retail layout notes: the Wii binary's btm_cb differs from btm_int.h's tBTM_CB
// (24 service records of 0x3C, 16 device records of 0x88). The security manager
// state between 0x193C-0x198F and 0x27B0-0x27C0 mirrors the retail binary
// exactly (timers, pin-code request state, bond flags, acl_disc_reason, ...).
// Local structs below mirror the retail offsets.

#include <string.h>

/* ------------------------------------------------------------------ */
/*  Local constants (Wii build values; bt_target.h defaults differ).  */
/*  Defined before includes so the #ifndef guards in bt_target.h      */
/*  pick up the retail-correct values.                                */
/* ------------------------------------------------------------------ */
#define BTM_SEC_MAX_SERVICE_RECORDS     24
#define BTM_SEC_MAX_RMT_NAME_CALLBACKS  2
#define BTM_SEC_SERVICE_NAME_LEN        21
#define BTM_PIN_CODE_LEN                6
#define BTM_SEC_IN_USE                  0x80
#define BTM_SEC_AUTHORIZED              0x01
#define BTM_SEC_AUTHENTICATED           0x02
#define BTM_SEC_ENCRYPTED               0x04
#define BTM_SEC_NAME_KNOWN              0x08
#define BTM_SEC_LINK_KEY_KNOWN          0x10
#define BTM_SEC_BOND                    0x40
#define BTM_CLOCK_OFFSET_VALID          0x8000
#define BTM_PAIR_STATE_IDLE             0
#define BTM_PAIR_STATE_GET_REM_NAME     1
#define BTM_SEC_STATE_IDLE              0
#define BTM_SEC_STATE_AUTHENTICATING    1
#define BTM_SEC_STATE_ENCRYPTING        2
#define BTM_SEC_STATE_GETTING_NAME      3
#define BTM_SEC_STATE_AUTHORIZING       4
#define BTM_SEC_STATE_SWITCHING_ROLE    5
#define BTM_SUCCESS                     0
#define BTM_CMD_STARTED                 1
#define BTM_NO_RESOURCES                3
#define BTM_MODE_UNSUPPORTED            4
#define BTM_ILLEGAL_VALUE               5
#define BTM_ERR_PROCESSING              10
#define BTM_RMT_NAME_SEC                2
#define HCI_ERR_HOST_REJECT_DEVICE      0x0F
#define HCI_ERR_LMP_ERR_TRANS_COLLISION 0x23

#include "revolution/BTE/include/bt_target.h"
#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/include/bt_trace.h"

/* ------------------------------------------------------------------ */
/*  Local constants (Wii build values; bt_target.h defaults differ)   */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Security service record: 24 entries of 0x3C bytes at 0x1990       */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT32  mx_proto_id;                     /* 0x00 */
    UINT32  orig_mx_chan_id;                 /* 0x04 */
    UINT32  term_mx_chan_id;                 /* 0x08 */
    UINT16  psm;                             /* 0x0C */
    UINT8   security_flags;                  /* 0x0E */
    UINT8   service_id;                      /* 0x0F */
    UINT8   orig_service_name[BTM_SEC_SERVICE_NAME_LEN + 1]; /* 0x10-0x25 */
    UINT8   term_service_name[BTM_SEC_SERVICE_NAME_LEN + 1]; /* 0x26-0x3B */
} BtmSecServRec;                             /* total 0x3C */

/* ------------------------------------------------------------------ */
/*  Device record (mirrors btm_dev.c): 16 entries of 0x88 at 0x1F30   */
/* ------------------------------------------------------------------ */
/* Callback types used by the security manager */
typedef UINT8 (*BtmAuthorizeCb)(UINT8 *bd_addr, UINT8 *dev_class, char *bd_name,
                                UINT8 *service_name, UINT8 service_id,
                                BOOLEAN is_originator);
typedef UINT8 (*BtmLinkKeyReqCb)(UINT8 *bd_addr, UINT8 *link_key);
typedef UINT8 (*BtmAuthCompleteCb)(UINT8 *bd_addr, UINT8 *dev_class,
                                   char *bd_name, int result);
typedef UINT8 (*BtmAbortCb)(UINT8 *bd_addr, UINT8 *dev_class, char *bd_name);
typedef UINT8 (*BtmConnFilterCb)(UINT8 *bd_addr, UINT8 *dev_class);
typedef void  (*BtmSecCallback)(UINT8 *bd_addr, void *p_ref_data, UINT8 result);
typedef void  (*BtmMkeyCallback)(UINT8 *bd_addr, UINT8 status, UINT8 key_flag);

typedef struct
{
    BtmSecServRec *p_cur_service;       /* 0x00 */
    BtmSecCallback p_callback;          /* 0x04 */
    void          *p_ref_data;          /* 0x08 */
    UINT32         timestamp;           /* 0x0C */
    UINT32         trusted_mask[2];     /* 0x10 */
    UINT16         hci_handle;          /* 0x18 */
    UINT16         clock_offset;        /* 0x1A */
    UINT8          bd_addr[6];          /* 0x1C */
    UINT8          dev_class[3];        /* 0x22 */
    UINT8          link_key[16];        /* 0x25 */
    char           sec_bd_name[65];     /* 0x35 */
    UINT8          sec_flags;           /* 0x76 */
    UINT8          features[8];         /* 0x77 */
    UINT8          sec_state;           /* 0x7F */
    UINT8          is_originator;       /* 0x80 */
    UINT8          security_required;   /* 0x81 */
    UINT8          link_key_not_sent;   /* 0x82 */
    UINT8          link_key_type;       /* 0x83 */
    UINT8          link_key_changed;    /* 0x84 */
    UINT8          sec_acl_connected;   /* 0x85 */
    UINT8          _pad[2];             /* 0x86-0x87 */
} BtmSecDevRec;                  /* total 0x88 */

/* ------------------------------------------------------------------ */
/*  Security application registration info (6 callback pointers)      */
/* ------------------------------------------------------------------ */
typedef struct
{
    BtmAuthorizeCb    p_authorize_callback;        /* 0x00 */
    BtmAuthorizeCb    p_pin_callback;              /* 0x04 */
    BtmAuthorizeCb    p_link_key_callback;         /* 0x08 */
    BtmLinkKeyReqCb   p_link_key_req_callback;     /* 0x0C */
    BtmAuthCompleteCb p_auth_complete_callback;    /* 0x10 */
    BtmAbortCb        p_abort_callback;            /* 0x14 */
} BtmApplInfo;                           /* total 0x18 */

/* ------------------------------------------------------------------ */
/*  Timer list entry as used by the btu layer in this build.          */
/* ------------------------------------------------------------------ */
typedef struct
{
    void   *p_next;      /* 0x00 */
    void   *p_prev;      /* 0x04 */
    void   *p_cback;     /* 0x08 */
    INT32   ticks;       /* 0x0C */
    void   *param;       /* 0x10 */
    UINT16  event;       /* 0x14 */
    UINT8   in_use;      /* 0x16 */
} BtmTimerListEnt;       /* total 0x18 */

/* ------------------------------------------------------------------ */
/*  Local btm_cb surrogate with retail offsets                         */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT8    _pad0[0x20];                        /* 0x00-0x1F */
    UINT8    pin_type;                           /* 0x20 cfg.pin_type */
    UINT8    cfg_pin_code_len;                   /* 0x21 cfg.pin_code_len */
    UINT8    cfg_pin_code[16];                   /* 0x22-0x31 cfg.pin_code */
    UINT8    _pad1[0x64E - 0x32];                /* 0x32-0x64D */
    UINT8    pairing_state;                      /* 0x64E */
    UINT8    _pad2[0x190C - 0x64F];              /* 0x64F-0x190B */
    BtmApplInfo api;                             /* 0x190C-0x1923 */
    void    *p_link_key_callback[BTM_SEC_MAX_RMT_NAME_CALLBACKS]; /* 0x1924-0x192B */
    void    *p_rmt_name_callback[BTM_SEC_MAX_RMT_NAME_CALLBACKS]; /* 0x192C-0x1933 */
    BtmConnFilterCb p_conn_filter_cb;            /* 0x1934-0x1937 */
    BtmSecDevRec *p_collided_dev_rec;            /* 0x1938-0x193B */
    BtmTimerListEnt sec_pin_code_req_tle;        /* 0x193C-0x1953 */
    UINT8    sec_pin_code_req_bd_addr[6];        /* 0x1954-0x1959 */
    UINT8    _pad6a[0x195C - 0x195A];            /* 0x195A-0x195B */
    BtmTimerListEnt sec_collision_tle;           /* 0x195C-0x1973 */
    UINT32   dev_rec_count;                      /* 0x1974-0x1977 */
    UINT8    security_mode;                      /* 0x1978 */
    UINT8    pairing_disabled;                   /* 0x1979 */
    UINT8    connect_only_paired;                /* 0x197A */
    UINT8    security_mode_changed;              /* 0x197B */
    UINT8    pin_type_changed;                   /* 0x197C */
    UINT8    bond_created_connection;            /* 0x197D */
    UINT8    disconnect_after_bond_reason;       /* 0x197E */
    UINT8    pin_code_len;                       /* 0x197F */
    UINT8    pin_code[16];                       /* 0x1980-0x198F */
    BtmSecServRec sec_serv_rec[BTM_SEC_MAX_SERVICE_RECORDS]; /* 0x1990-0x1F2F */
    BtmSecDevRec  sec_dev_rec[16];               /* 0x1F30-0x27AF */
    BtmMkeyCallback mkey_cback;                  /* 0x27B0-0x27B3 */
    UINT8    connecting_bda[6];                  /* 0x27B4-0x27B9 */
    UINT8    connecting_dc[3];                   /* 0x27BA-0x27BC */
    UINT8    first_disabled_channel;             /* 0x27BD */
    UINT8    last_disabled_channel;              /* 0x27BE */
    UINT8    acl_disc_reason;                    /* 0x27BF */
    UINT8    trace_level;                        /* 0x27C0 */
} BtmCb;

/* The real global (defined in another TU) */
extern BtmCb btm_cb;

/* Callback type for remote-name notifications */
typedef void (tBTM_RMT_NAME_CALLBACK) (BD_ADDR bd_addr, DEV_CLASS dc, BD_NAME bd_name);

/* ------------------------------------------------------------------ */
/*  External functions                                                 */
/* ------------------------------------------------------------------ */
BtmSecDevRec *btm_find_dev(BD_ADDR bd_addr);
BtmSecDevRec *btm_find_dev_by_handle(UINT16 handle);
BtmSecDevRec *btm_sec_alloc_dev(BD_ADDR bd_addr);
UINT8         btm_sec_execute_procedure(BtmSecDevRec *p_dev_rec);

void btm_sec_collision_timeout(void *p_tle);

UINT8  btm_initiate_rem_name(BD_ADDR remote_bda, void *p_cur, UINT8 origin,
                             UINT32 timeout, void *p_cb);
void   l2c_link_hci_conn_req(BD_ADDR bda);
void  *GKI_getpoolbuf(UINT8 pool_id);

void   btu_start_timer(void *p_tle, UINT16 type, UINT32 timeout);
void   btu_stop_timer(void *p_tle);

typedef struct
{
    UINT16 clock_offset;                     /* 0x00 tBTM_INQ_RESULTS.clock_offset */
} tBTM_INQ_INFO;

tBTM_INQ_INFO *BTM_InqDbRead(BD_ADDR p_bda);

void btsnd_hcic_write_pin_type(UINT8 pin_type);
void btsnd_hcic_pin_code_neg_reply(BD_ADDR bda);
BOOLEAN btsnd_hcic_pin_code_req_reply(BD_ADDR bda, UINT8 pin_len, UINT8 *p_pin);
void btsnd_hcic_write_auth_enable(UINT8 flag);
void btsnd_hcic_write_encr_mode(UINT8 mode);
BOOLEAN btsnd_hcic_link_key_req_reply(BD_ADDR bda, UINT8 *link_key);
BOOLEAN btsnd_hcic_link_key_neg_reply(BD_ADDR bda);
BOOLEAN btsnd_hcic_reject_conn(void *p_buf, BD_ADDR bda, UINT8 reason);
BOOLEAN btsnd_hcic_disconnect(UINT16 handle, UINT8 reason);

/* ------------------------------------------------------------------ */
/*  BTM_SecRegister -- register the application security callbacks.    */
/* ------------------------------------------------------------------ */
BOOLEAN BTM_SecRegister(BtmApplInfo *p_cb_info)
{
    btm_cb.api = *p_cb_info;
    BTM_TRACE_EVENT0("BTM_Sec: application registered");
    return TRUE;
}

/* ------------------------------------------------------------------ */
/*  BTM_SecAddRmtNameNotifyCallback -- add a remote name callback.     */
/* ------------------------------------------------------------------ */
BOOLEAN BTM_SecAddRmtNameNotifyCallback(tBTM_RMT_NAME_CALLBACK *p_callback)
{
    int i;

    for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++)
    {
        if (btm_cb.p_rmt_name_callback[i] == NULL)
        {
            btm_cb.p_rmt_name_callback[i] = (void *)p_callback;
            return TRUE;
        }
    }
    return FALSE;
}

/* ------------------------------------------------------------------ */
/*  BTM_SecDeleteRmtNameNotifyCallback -- remove a remote name cb.     */
/* ------------------------------------------------------------------ */
BOOLEAN BTM_SecDeleteRmtNameNotifyCallback(tBTM_RMT_NAME_CALLBACK *p_callback)
{
    int i;

    for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++)
    {
        if (btm_cb.p_rmt_name_callback[i] == (void *)p_callback)
        {
            btm_cb.p_rmt_name_callback[i] = NULL;
            return TRUE;
        }
    }
    return FALSE;
}

/* ------------------------------------------------------------------ */
/*  BTM_SetPinType -- set the PIN type, code and length for pairing.   */
/* ------------------------------------------------------------------ */
void BTM_SetPinType(UINT8 pin_type, PIN_CODE pin_code, UINT8 pin_code_len)
{
    BTM_TRACE_API3("BTM_SetPinType: pin type %d [variable-0, fixed-1], code %s, length %d",
                   pin_type, (char *)pin_code, pin_code_len);

    /* If we are in pairing/bonding, save the pin type, code and length */
    if (btm_cb.pairing_state > BTM_PAIR_STATE_GET_REM_NAME)
    {
        btsnd_hcic_write_pin_type(pin_type);
    }

    btm_cb.pin_type = pin_type;
    btm_cb.cfg_pin_code_len = pin_code_len;
    memcpy(btm_cb.cfg_pin_code, pin_code, pin_code_len);
}

/* ------------------------------------------------------------------ */
/*  BTM_SetSecurityLevel -- register a service's security requirement. */
/* ------------------------------------------------------------------ */
BOOLEAN BTM_SetSecurityLevel(BOOLEAN is_originator, char *p_name, UINT8 service_id,
                             UINT8 sec_level, UINT16 psm, UINT32 mx_proto_id,
                             UINT32 mx_chan_id)
{
    BtmSecServRec *p_srec;
    UINT16 i;
    UINT16 last_id = 0xFFFF;
    BOOLEAN found = FALSE;

    /* See if a record already exists with this name and parameters */
    p_srec = &btm_cb.sec_serv_rec[0];

    for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++)
    {
        if (p_srec->security_flags & BTM_SEC_IN_USE)
        {
            if (p_srec->psm == psm && p_srec->mx_proto_id == mx_proto_id &&
                service_id == p_srec->service_id &&
                (!strncmp(p_name, (char *)p_srec->orig_service_name, BTM_SEC_SERVICE_NAME_LEN) ||
                 !strncmp(p_name, (char *)p_srec->term_service_name, BTM_SEC_SERVICE_NAME_LEN)))
            {
                found = TRUE;
                break;
            }
        }
        else if (!found)
        {
            found = TRUE;
            last_id = i;
        }
        p_srec++;
    }

    if (found)
    {
        if (i >= BTM_SEC_MAX_SERVICE_RECORDS)
        {
            i = last_id;
            p_srec = &btm_cb.sec_serv_rec[last_id];
        }

        /* Fill in the service record */
        p_srec->psm = psm;
        p_srec->service_id = service_id;
        p_srec->mx_proto_id = mx_proto_id;

        if (is_originator)
        {
            p_srec->orig_mx_chan_id = mx_chan_id;
            p_srec->security_flags = (UINT8)(p_srec->security_flags & ~0x38);
            strncpy((char *)p_srec->orig_service_name, p_name, BTM_SEC_SERVICE_NAME_LEN);
        }
        else
        {
            p_srec->term_mx_chan_id = mx_chan_id;
            p_srec->security_flags = (UINT8)(p_srec->security_flags & ~0x07);
            strncpy((char *)p_srec->term_service_name, p_name, BTM_SEC_SERVICE_NAME_LEN);
        }

        p_srec->security_flags |= (BTM_SEC_IN_USE | sec_level);

        BTM_TRACE_API6("BTM_SEC_REG[%d]: id %d, is_orig %d, psm 0x%04x, proto_id %d, chan_id %d",
                       i, service_id, is_originator, psm, mx_proto_id, mx_chan_id);
        BTM_TRACE_API3("               : sec: 0x%x, service name [%s] (up to %d chars saved)",
                       p_srec->security_flags, p_name, BTM_SEC_SERVICE_NAME_LEN);
    }
    else
    {
        BTM_TRACE_WARNING1("BTM_SEC_REG: Out of Service Records (%d)", BTM_SEC_MAX_SERVICE_RECORDS);
    }

    return found;
}

/* ------------------------------------------------------------------ */
/*  btm_sec_init -- initialise the security manager.                   */
/* ------------------------------------------------------------------ */
void btm_sec_init(UINT8 sec_mode)
{
    btm_cb.security_mode = sec_mode;
    memset(btm_cb.sec_pin_code_req_bd_addr, 0xff, BD_ADDR_LEN);
}

/* ------------------------------------------------------------------ */
/*  btm_sec_dev_reset -- re-enable authentication if security mode 3.  */
/* ------------------------------------------------------------------ */
void btm_sec_dev_reset(void)
{
    if (btm_cb.security_mode == 3)
    {
        btsnd_hcic_write_auth_enable(1);
        btsnd_hcic_write_encr_mode(1);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_sec_is_bonding -- check if the device is currently bonding.    */
/* ------------------------------------------------------------------ */
BOOLEAN btm_sec_is_bonding(UINT16 handle)
{
    BtmSecDevRec *p_dev_rec = btm_find_dev_by_handle(handle);

    if (p_dev_rec && (p_dev_rec->security_required & BTM_SEC_BOND))
        return TRUE;
    return FALSE;
}

/* ------------------------------------------------------------------ */
/*  btm_sec_update_clock_offset -- update clock offset of a device.    */
/* ------------------------------------------------------------------ */
void btm_sec_update_clock_offset(UINT16 handle, UINT16 clock_offset)
{
    BtmSecDevRec *p_dev_rec;
    tBTM_INQ_INFO *p_inq_info;

    p_dev_rec = btm_find_dev_by_handle(handle);
    if (p_dev_rec)
    {
        p_dev_rec->clock_offset = (UINT16)(clock_offset | BTM_CLOCK_OFFSET_VALID);

        p_inq_info = BTM_InqDbRead(p_dev_rec->bd_addr);
        if (p_inq_info)
            p_inq_info->clock_offset = (UINT16)(clock_offset | BTM_CLOCK_OFFSET_VALID);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_sec_pin_code_request_timeout -- PIN code request timed out.    */
/* ------------------------------------------------------------------ */
void btm_sec_pin_code_request_timeout(void *p_tle)
{
    BTM_TRACE_EVENT0("btm_sec_pin_code_request_timeout()");

    btm_cb.sec_pin_code_req_tle.param = 0;
    memset(btm_cb.sec_pin_code_req_bd_addr, 0xff, BD_ADDR_LEN);
    btsnd_hcic_pin_code_neg_reply(btm_cb.connecting_bda);
}

/* ------------------------------------------------------------------ */
/*  btm_read_trusted_mask -- return the trusted service mask.          */
/* ------------------------------------------------------------------ */
UINT32 *btm_read_trusted_mask(BD_ADDR bd_addr)
{
    BtmSecDevRec *p_dev_rec = btm_find_dev(bd_addr);

    if (p_dev_rec)
        return p_dev_rec->trusted_mask;

    return NULL;
}

/* ------------------------------------------------------------------ */
/*  BTM_PINCodeReply -- application response to a PIN code request.    */
/* ------------------------------------------------------------------ */
void BTM_PINCodeReply(BD_ADDR bd_addr, UINT8 res, UINT8 pin_len,
                      UINT8 *p_pin, UINT32 *trusted_mask)
{
    BtmSecDevRec *p_dev_rec;

    if (!btm_cb.sec_pin_code_req_tle.param)
        return;

    btu_stop_timer(&btm_cb.sec_pin_code_req_tle);

    btm_cb.sec_pin_code_req_tle.param = 0;

    memset(btm_cb.sec_pin_code_req_bd_addr, 0xff, BD_ADDR_LEN);

    if (res)
    {
        btm_cb.acl_disc_reason = 14;

        btsnd_hcic_pin_code_neg_reply(bd_addr);
    }
    else
    {
        btm_cb.acl_disc_reason = 0;

        btsnd_hcic_pin_code_req_reply(bd_addr, pin_len, p_pin);

        if ((p_dev_rec = btm_find_dev(bd_addr)) != NULL)
        {
            p_dev_rec->trusted_mask[0] = trusted_mask[0];
            p_dev_rec->trusted_mask[1] = trusted_mask[1];
        }
    }
}

void BTM_DeviceAuthorized() {}

void BTM_SecBond() {}

void btm_sec_l2cap_access_req() {}

void btm_sec_mx_access_request() {}

/* ------------------------------------------------------------------ */
/*  btm_sec_conn_req -- incoming HCI connection request.               */
/* ------------------------------------------------------------------ */
void btm_sec_conn_req(BD_ADDR bda, DEV_CLASS dc)
{
    void *p_buf;
    BtmSecDevRec *p_dev_rec;

    if (btm_cb.connect_only_paired)
    {
        p_dev_rec = btm_find_dev(bda);

        if (!p_dev_rec || !(p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN))
        {
            BTM_TRACE_EVENT0("Security Manager: connect request from not paired device");

            if ((p_buf = GKI_getpoolbuf(2)) != NULL)
                btsnd_hcic_reject_conn(p_buf, bda, HCI_ERR_HOST_REJECT_DEVICE);

            return;
        }
    }

    if (btm_cb.p_conn_filter_cb)
    {
        if (!(*btm_cb.p_conn_filter_cb)(bda, dc))
        {
            if ((p_buf = GKI_getpoolbuf(2)) != NULL)
                btsnd_hcic_reject_conn(p_buf, bda, HCI_ERR_HOST_REJECT_DEVICE);

            return;
        }
    }

    memcpy(btm_cb.connecting_bda, bda, BD_ADDR_LEN);
    memcpy(btm_cb.connecting_dc, dc, DEV_CLASS_LEN);

    l2c_link_hci_conn_req(bda);
}

/* ------------------------------------------------------------------ */
/*  btm_sec_abort_access_req -- abort an in-progress security access.  */
/* ------------------------------------------------------------------ */
void btm_sec_abort_access_req(BD_ADDR bd_addr)
{
    BtmSecDevRec *p_dev_rec = btm_find_dev(bd_addr);

    if (!p_dev_rec)
        return;

    if (btm_cb.api.p_abort_callback)
        (*btm_cb.api.p_abort_callback)(bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name);

    if (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHORIZING &&
        p_dev_rec->sec_state != BTM_SEC_STATE_AUTHENTICATING)
    {
        return;
    }

    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

    /* btm_clr_client_scn: clear the originator's SCN if a multiplexer */
    if (p_dev_rec->is_originator)
    {
        if (p_dev_rec->p_cur_service)
        {
            if (p_dev_rec->p_cur_service->mx_proto_id == 3)
                p_dev_rec->p_cur_service->orig_mx_chan_id = 0;
        }
    }

    p_dev_rec->p_callback = NULL;
}

void btm_sec_rmt_name_request_complete() {}

void btm_sec_auth_complete() {}

/* ------------------------------------------------------------------ */
/*  btm_sec_mkey_comp_event -- master key completion event.            */
/* ------------------------------------------------------------------ */
void btm_sec_mkey_comp_event(UINT16 handle, UINT8 status, UINT8 key_flg)
{
    BtmSecDevRec *p_dev_rec = btm_find_dev_by_handle(handle);
    UINT8 bd_addr[BD_ADDR_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    BTM_TRACE_EVENT2("Security Manager: mkey comp status:%d State:%d",
                     status, (p_dev_rec) ? p_dev_rec->sec_state : 0);

    if (btm_cb.mkey_cback)
    {
        if (!p_dev_rec)
            (*btm_cb.mkey_cback)(bd_addr, status, key_flg);
        else
            (*btm_cb.mkey_cback)(p_dev_rec->bd_addr, status, key_flg);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_sec_encrypt_change -- HCI encryption change event.             */
/* ------------------------------------------------------------------ */
void btm_sec_encrypt_change(UINT16 handle, UINT8 status, UINT8 encr_enable)
{
    BtmSecDevRec *p_dev_rec = btm_find_dev_by_handle(handle);

    BTM_TRACE_EVENT2("Security Manager: encrypt_change status:%d State:%d",
                     status, p_dev_rec ? p_dev_rec->sec_state : 0);

    if (!p_dev_rec)
        return;

    if (status == HCI_ERR_LMP_ERR_TRANS_COLLISION)
    {
        btm_cb.p_collided_dev_rec = p_dev_rec;
        btm_cb.sec_collision_tle.param = (void *)btm_sec_collision_timeout;
        btu_start_timer(&btm_cb.sec_collision_tle, 22, 1);

        return;
    }

    if (status != 0)
    {
        if (p_dev_rec->p_callback)
        {
            (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, BTM_ERR_PROCESSING);
            p_dev_rec->p_callback = NULL;
        }

        return;
    }

    p_dev_rec->sec_flags |= (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED);

    if (p_dev_rec->sec_state != BTM_SEC_STATE_ENCRYPTING)
        return;

    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

    status = btm_sec_execute_procedure(p_dev_rec);

    if (status != BTM_CMD_STARTED && p_dev_rec->p_callback)
    {
        (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, status);
        p_dev_rec->p_callback = NULL;
    }
}

void btm_sec_connected() {}

/* ------------------------------------------------------------------ */
/*  btm_sec_disconnect -- request to disconnect the ACL link.          */
/* ------------------------------------------------------------------ */
UINT8 btm_sec_disconnect(UINT16 handle, UINT8 reason)
{
    BtmSecDevRec *p_dev_rec = btm_find_dev_by_handle(handle);
    UINT8 old_state;

    if (!p_dev_rec)
        return BTM_ILLEGAL_VALUE;

    if (p_dev_rec->security_required & BTM_SEC_BOND)
    {
        btm_cb.disconnect_after_bond_reason = reason;
        return BTM_SUCCESS;
    }

    if (p_dev_rec->sec_state == BTM_SEC_STATE_SWITCHING_ROLE)
        return BTM_CMD_STARTED;

    old_state = p_dev_rec->sec_state;
    p_dev_rec->sec_state = BTM_SEC_STATE_SWITCHING_ROLE;

    if (btsnd_hcic_disconnect(handle, reason))
        return BTM_CMD_STARTED;

    p_dev_rec->sec_state = old_state;
    return BTM_NO_RESOURCES;
}

/* ------------------------------------------------------------------ */
/*  btm_sec_disconnected -- ACL link is gone; clean up security state. */
/* ------------------------------------------------------------------ */
void btm_sec_disconnected(UINT16 handle, UINT8 reason)
{
    BtmSecDevRec *p_dev_rec = btm_find_dev_by_handle(handle);

    if (!p_dev_rec)
        return;

    if (p_dev_rec->security_required & BTM_SEC_BOND && btm_cb.api.p_auth_complete_callback)
    {
        if (reason == 23)
        {
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, 23);
        }
        else
        {
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, 5);
        }
    }

    if (btm_cb.sec_pin_code_req_tle.param &&
        memcmp(btm_cb.sec_pin_code_req_bd_addr, p_dev_rec->bd_addr, 6) == 0)
    {
        btu_stop_timer(&btm_cb.sec_pin_code_req_tle);
        btm_cb.sec_pin_code_req_tle.param = 0;
        memset(btm_cb.sec_pin_code_req_bd_addr, 0xff, BD_ADDR_LEN);

        if (btm_cb.api.p_auth_complete_callback)
        {
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, 14);
        }
    }

    p_dev_rec->hci_handle = 0xffff;
    p_dev_rec->sec_state = 0;
    p_dev_rec->sec_flags &= ~0x07;
    p_dev_rec->security_required &= ~0x7f;
    p_dev_rec->p_callback = NULL;
}

void btm_sec_link_key_notification() {}

/* ------------------------------------------------------------------ */
/*  btm_sec_link_key_request -- HCI link key request event.            */
/* ------------------------------------------------------------------ */
void btm_sec_link_key_request(BD_ADDR p_bda)
{
    BtmSecDevRec *p_dev_rec = btm_find_dev(p_bda);
    UINT8 link_key[16];

    BTM_TRACE_EVENT6("btm_sec_link_key_request()  BDA: %02x:%02x:%02x:%02x:%02x:%02x",
                     p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);

    /* If we have a link key for the device, send it */
    if (p_dev_rec && (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN))
    {
        btsnd_hcic_link_key_req_reply(p_bda, p_dev_rec->link_key);
        return;
    }

    /* If we don't have a record for the device, create one */
    if (!p_dev_rec)
        p_dev_rec = btm_sec_alloc_dev(p_bda);

    if (!(p_dev_rec->security_required & BTM_SEC_BOND) && btm_cb.api.p_link_key_req_callback)
    {
        if (p_dev_rec)
        {
            if (!(*btm_cb.api.p_link_key_req_callback)(p_bda, p_dev_rec->link_key))
            {
                btsnd_hcic_link_key_req_reply(p_bda, p_dev_rec->link_key);
                return;
            }
        }
        else if (!(*btm_cb.api.p_link_key_req_callback)(p_bda, link_key))
        {
            btsnd_hcic_link_key_req_reply(p_bda, link_key);
            return;
        }
    }

    btsnd_hcic_link_key_neg_reply(p_bda);
}

void btm_sec_pin_code_request() {}

/* ------------------------------------------------------------------ */
/*  btm_sec_execute_procedure -- placeholder; recovered elsewhere.     */
/*  Kept non-inlinable: -ipa file would otherwise inline the empty    */
/*  body and drop the call sites in encrypt_change/collision_timeout. */
/* ------------------------------------------------------------------ */
#pragma auto_inline off
UINT8 btm_sec_execute_procedure(BtmSecDevRec *p_dev_rec)
{
    return 0;
}
#pragma auto_inline on

/* ------------------------------------------------------------------ */
/*  btm_sec_start_authorization -- start the authorization procedure.  */
/* ------------------------------------------------------------------ */
UINT8 btm_sec_start_authorization(BtmSecDevRec *p_dev_rec)
{
    UINT8 result;
    UINT8 *p_service_name = NULL;
    UINT8 service_id;
    UINT8 tempstate;

    if (p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN ||
        p_dev_rec->hci_handle == 0xffff)
    {
        if (!btm_cb.api.p_authorize_callback)
            return BTM_MODE_UNSUPPORTED;

        if (p_dev_rec->p_cur_service)
        {
            if (p_dev_rec->is_originator)
                p_service_name = p_dev_rec->p_cur_service->orig_service_name;
            else
                p_service_name = p_dev_rec->p_cur_service->term_service_name;

            service_id = p_dev_rec->p_cur_service->service_id;
        }
        else
        {
            service_id = 0;
        }

        p_dev_rec->sec_state = BTM_SEC_STATE_AUTHORIZING;
        result = (*btm_cb.api.p_authorize_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                                    p_dev_rec->sec_bd_name, p_service_name,
                                                    service_id, p_dev_rec->is_originator);

        if (result == BTM_SUCCESS)
        {
            p_dev_rec->sec_flags |= BTM_SEC_AUTHORIZED;
            p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;
        }

        return result;
    }

    /* btm_sec_start_get_name: request the remote name */
    tempstate = p_dev_rec->sec_state;
    p_dev_rec->sec_state = BTM_SEC_STATE_GETTING_NAME;

    if (btm_initiate_rem_name(p_dev_rec->bd_addr, NULL, BTM_RMT_NAME_SEC, 30, NULL) != BTM_CMD_STARTED)
        p_dev_rec->sec_state = tempstate;

    return BTM_CMD_STARTED;
}

/* ------------------------------------------------------------------ */
/*  btm_sec_collision_timeout -- retry the security procedure.         */
/* ------------------------------------------------------------------ */
void btm_sec_collision_timeout(void *p_tle)
{
    UINT8 status;

    BTM_TRACE_EVENT0("btm_sec_collision_timeout()");
    btm_cb.sec_collision_tle.param = 0;

    status = btm_sec_execute_procedure(btm_cb.p_collided_dev_rec);

    if (status == BTM_CMD_STARTED)
        return;

    if (btm_cb.p_collided_dev_rec->p_callback)
    {
        /* btm_clr_client_scn: clear the originator's SCN if a multiplexer */
        if (btm_cb.p_collided_dev_rec->is_originator)
        {
            if (btm_cb.p_collided_dev_rec->p_cur_service)
            {
                if (btm_cb.p_collided_dev_rec->p_cur_service->mx_proto_id == 3)
                    btm_cb.p_collided_dev_rec->p_cur_service->orig_mx_chan_id = 0;
            }
        }

        (*btm_cb.p_collided_dev_rec->p_callback)(btm_cb.p_collided_dev_rec->bd_addr,
                                                 btm_cb.p_collided_dev_rec->p_ref_data, status);
        btm_cb.p_collided_dev_rec->p_callback = NULL;
    }
}
