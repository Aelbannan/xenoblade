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
#define BTM_SEC_MAX_DEVICE_RECORDS      16
#define BTM_SEC_MAX_SERVICES            46
#define BTM_PIN_CODE_LEN                6
#define BTM_SEC_IN_USE                  0x80
#define BTM_SEC_AUTHORIZED              0x01
#define BTM_SEC_AUTHENTICATED           0x02
#define BTM_SEC_ENCRYPTED               0x04
#define BTM_SEC_NAME_KNOWN              0x08
#define BTM_SEC_LINK_KEY_KNOWN          0x10
#define BTM_SEC_BOND                    0x40
#define BTM_SEC_IN_AUTHORIZE            0x01
#define BTM_SEC_IN_AUTHENTICATE         0x02
#define BTM_SEC_IN_ENCRYPT              0x04
#define BTM_SEC_OUT_AUTHORIZE           0x08
#define BTM_SEC_OUT_AUTHENTICATE        0x10
#define BTM_SEC_OUT_ENCRYPT             0x20
#define BTM_SEC_OUT_LEVEL4              (BTM_SEC_BOND | BTM_SEC_OUT_AUTHENTICATE) /* 0x50 */
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
#define BTM_DEVICE_TIMEOUT              8
#define BTM_ERR_PROCESSING              10
#define BTM_RMT_NAME_SEC                2
#define HCI_SUCCESS                     0x00
#define HCI_ERR_PAGE_TIMEOUT            0x04
#define HCI_ERR_AUTH_FAILURE            0x05
#define HCI_ERR_KEY_MISSING             0x06
#define HCI_ERR_CONNECTION_TOUT         0x08
#define HCI_ERR_COMMAND_DISALLOWED      0x0C
#define HCI_ERR_HOST_REJECT_SECURITY    0x0E
#define HCI_ERR_HOST_REJECT_DEVICE      0x0F
#define HCI_ERR_PEER_USER               0x13
#define HCI_ERR_REPEATED_ATTEMPTS       0x17
#define HCI_ERR_PAIRING_NOT_ALLOWED     0x18
#define HCI_ERR_UNSPECIFIED             0x1F
#define HCI_ERR_LMP_RESPONSE_TIMEOUT    0x22
#define HCI_ERR_LMP_ERR_TRANS_COLLISION 0x23
#define HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE 0x25
#define HCI_ERR_UNIT_KEY_USED           0x26
#define HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED 0x29
#define HCI_PAGE_SCAN_REP_MODE_R1       0x01
#define HCI_PAGE_SCAN_MODE_MANDATORY    0x00
#define HCI_PIN_TYPE_FIXED              1
#define HCI_INVALID_HANDLE              0xFFFF
#define HCI_VERSION_1_1                 0x01
#define BTU_TTYPE_USER_FUNC             22
#define BTM_PIN_CODE_REQUEST_TOUT       60
#define BTM_SEC_MODE_LINK               3
#define BTM_LKEY_TYPE_IGNORE            0xff

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
typedef void  (*BtmPinCb)(UINT8 *bd_addr, UINT8 *dev_class, char *bd_name);
typedef void  (*BtmLinkKeyCb)(UINT8 *bd_addr, UINT8 *dev_class, char *bd_name,
                              UINT8 *link_key, UINT8 key_type);
typedef UINT8 (*BtmLinkKeyReqCb)(UINT8 *bd_addr, UINT8 *link_key);
typedef void  (*BtmAuthCompleteCb)(UINT8 *bd_addr, UINT8 *dev_class,
                                   char *bd_name, int result);
typedef void  (*BtmAbortCb)(UINT8 *bd_addr, UINT8 *dev_class, char *bd_name);
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
    BtmPinCb          p_pin_callback;              /* 0x04 */
    BtmLinkKeyCb      p_link_key_callback;         /* 0x08 */
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

/* Callback type for remote-name notifications */
typedef void (tBTM_RMT_NAME_CALLBACK) (BD_ADDR bd_addr, DEV_CLASS dc, BD_NAME bd_name);

/* ------------------------------------------------------------------ */
/*  Local version info (HCI Read Local Version info) at 0x0636.       */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT8   hci_version;      /* 0x0636 */
    UINT16  hci_revision;     /* 0x0638 */
    UINT8   lmp_version;      /* 0x063A */
    UINT16  manufacturer;     /* 0x063C */
    UINT16  lmp_subversion;   /* 0x063E */
} BtmLocalVersionInfo;        /* 0x0636-0x0640 */

/* ------------------------------------------------------------------ */
/*  Local btm_cb surrogate with retail offsets                         */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT8    _pad0[0x20];                        /* 0x00-0x1F */
    UINT8    pin_type;                           /* 0x20 cfg.pin_type */
    UINT8    cfg_pin_code_len;                   /* 0x21 cfg.pin_code_len */
    UINT8    cfg_pin_code[16];                   /* 0x22-0x31 cfg.pin_code */
    UINT8    _pad1[0x636 - 0x32];                /* 0x32-0x635 */
    BtmLocalVersionInfo local_version;           /* 0x636-0x63F */
    UINT8    _pad1b[0x64E - 0x640];              /* 0x640-0x64D */
    UINT8    pairing_state;                      /* 0x64E */
    UINT8    _pad2[0x190C - 0x64F];              /* 0x64F-0x190B */
    BtmApplInfo api;                             /* 0x190C-0x1923 */
    BtmLinkKeyCb p_link_key_callback[BTM_SEC_MAX_RMT_NAME_CALLBACKS]; /* 0x1924-0x192B */
    tBTM_RMT_NAME_CALLBACK *p_rmt_name_callback[BTM_SEC_MAX_RMT_NAME_CALLBACKS]; /* 0x192C-0x1933 */
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

/* Minimal l2cb layout for the L2CAP trace macros (trace level at 0x00). */
typedef struct
{
    UINT8   l2cap_trace_level;              /* 0x000 */
    UINT8   conn_role;                      /* 0x001 */
    UINT16  pad2;                           /* 0x002 */
    UINT32  pad4;                           /* 0x004 */
    UINT8   pad8[0x100];                    /* 0x008 */
} L2Cb;
extern L2Cb l2cb;

/* ------------------------------------------------------------------ */
/*  External functions                                                 */
/* ------------------------------------------------------------------ */
BtmSecDevRec *btm_find_dev(BD_ADDR bd_addr);
BtmSecDevRec *btm_find_dev_by_handle(UINT16 handle);
BtmSecDevRec *btm_sec_alloc_dev(BD_ADDR bd_addr);
UINT8         btm_sec_execute_procedure(BtmSecDevRec *p_dev_rec);
UINT8         btm_sec_start_authorization(BtmSecDevRec *p_dev_rec);

void btm_sec_collision_timeout(void *p_tle);

UINT8  btm_initiate_rem_name(BD_ADDR remote_bda, void *p_cur, UINT8 origin,
                             UINT32 timeout, void *p_cb);
void   l2c_link_hci_conn_req(BD_ADDR bda);
void  *GKI_getpoolbuf(UINT8 pool_id);

void   btu_start_timer(void *p_tle, UINT16 type, UINT32 timeout);
void   btu_stop_timer(void *p_tle);

UINT8  BTM_DeleteStoredLinkKey(BD_ADDR bd_addr, void *p_cb);
UINT8 *BTM_ReadLocalFeatures(void);
void  *l2cu_allocate_lcb(BD_ADDR p_bd_addr);
BOOLEAN l2cu_create_conn(void *p_lcb);

typedef struct
{
    UINT16  clock_offset;              /* 0x00 */
    UINT8   remote_bd_addr[6];         /* 0x02 */
    UINT8   dev_class[3];              /* 0x08 */
    UINT8   page_scan_rep_mode;        /* 0x0B */
    UINT8   page_scan_per_mode;        /* 0x0C */
    UINT8   page_scan_mode;            /* 0x0D */
    UINT8   rssi;                      /* 0x0E */
} tBTM_INQ_INFO;

tBTM_INQ_INFO *BTM_InqDbRead(BD_ADDR p_bda);

void btsnd_hcic_write_pin_type(UINT8 pin_type);
void btsnd_hcic_pin_code_neg_reply(BD_ADDR bda);
BOOLEAN btsnd_hcic_pin_code_req_reply(BD_ADDR bda, UINT8 pin_len, UINT8 *p_pin);
BOOLEAN btsnd_hcic_write_auth_enable(UINT8 flag);
void btsnd_hcic_write_encr_mode(UINT8 mode);
BOOLEAN btsnd_hcic_link_key_req_reply(BD_ADDR bda, UINT8 *link_key);
BOOLEAN btsnd_hcic_link_key_neg_reply(BD_ADDR bda);
BOOLEAN btsnd_hcic_reject_conn(void *p_buf, BD_ADDR bda, UINT8 reason);
BOOLEAN btsnd_hcic_disconnect(UINT16 handle, UINT8 reason);
BOOLEAN btsnd_hcic_auth_request(UINT16 handle);
BOOLEAN btsnd_hcic_set_conn_encrypt(UINT16 handle, BOOLEAN enable);
BOOLEAN btsnd_hcic_create_conn(BD_ADDR bda, UINT16 packet_types,
                               UINT8 page_scan_rep_mode, UINT8 page_scan_mode,
                               UINT16 clock_offset, BOOLEAN allow_switch);

/* ------------------------------------------------------------------ */
/*  btm_sec_find_first_serv -- find the first service record with the  */
/*  specified PSM.                                                     */
/* ------------------------------------------------------------------ */
static __inline BtmSecServRec *btm_sec_find_first_serv(UINT8 conn_type, UINT16 psm)
{
    BtmSecServRec *p_srec = &btm_cb.sec_serv_rec[0];
    int i;

    for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_srec++)
    {
        if ((p_srec->security_flags & BTM_SEC_IN_USE) && (p_srec->psm == psm))
            return (p_srec);
    }
    return (NULL);
}

/* ------------------------------------------------------------------ */
/*  btm_sec_find_next_serv -- find the next service record with the    */
/*  same PSM as p_cur.                                                 */
/* ------------------------------------------------------------------ */
static __inline BtmSecServRec *btm_sec_find_next_serv(BtmSecServRec *p_cur)
{
    BtmSecServRec *p_srec = &btm_cb.sec_serv_rec[0];
    int i;
    BOOLEAN found = FALSE;

    for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_srec++)
    {
        if ((p_srec->security_flags & BTM_SEC_IN_USE) && (p_srec->psm == p_cur->psm))
        {
            if (found)
                return (p_srec);
            if (p_srec == p_cur)
            {
                found = TRUE;
                continue;
            }
        }
    }
    return (NULL);
}

/* ------------------------------------------------------------------ */
/*  btm_sec_find_mx_serv -- find the service record matching the       */
/*  multiplexer protocol and channel.                                  */
/* ------------------------------------------------------------------ */
static __inline BtmSecServRec *btm_sec_find_mx_serv(UINT16 psm, UINT8 is_originator,
                                           UINT32 mx_proto_id, UINT32 mx_chan_id)
{
    int i;
    BtmSecServRec *p_srec;

    p_srec = &btm_cb.sec_serv_rec[0];

    for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_srec++)
    {
        if ((p_srec->security_flags & BTM_SEC_IN_USE)
            && (p_srec->psm == psm)
            && (p_srec->mx_proto_id == mx_proto_id))
        {
            if ((is_originator && (p_srec->orig_mx_chan_id == mx_chan_id))
                || (!is_originator && (p_srec->term_mx_chan_id == mx_chan_id)))
                return (p_srec);
        }
    }
    return (NULL);
}


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
/*  String-pool orphans: the retail .data contains the format strings  */
/*  of BTM_SetSecurityMode / BTM_SetEncryption even though those       */
/*  functions were eliminated from the retail .text.  Reference them   */
/*  here (in retail pool order) so the .data string layout matches.    */
/* ------------------------------------------------------------------ */
char *const btm_sec_pool_secmode_strings[] = {
    "BTM_SetSecurityMode: mode:%d",
    "BTM_SetSecurityMode: Authen Enable -> FALSE",
    "BTM_SetSecurityMode: Authen Enable -> TRUE",
};

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

/* ------------------------------------------------------------------ */
/*  BTM_DeviceAuthorized -- application response to an authorization   */
/*  request received via the authorize callback.                      */
/* ------------------------------------------------------------------ */
void BTM_DeviceAuthorized(BD_ADDR bd_addr, UINT8 res, UINT32 trusted_mask[])
{
    BtmSecDevRec *p_dev_rec;
    UINT8 result;
    UINT8 old_state;

    if ((p_dev_rec = btm_find_dev(bd_addr)) == NULL)
    {
        BTM_TRACE_WARNING6("Security Manager: Attempting Authorization of Unknown Device Address [%02x%02x%02x%02x%02x%02x]",
                           bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
        return;
    }

    BTM_TRACE_EVENT2("Security Manager: authorized status:%d State:%d",
                     res, p_dev_rec ? p_dev_rec->sec_state : 0);

    if (res == BTM_SUCCESS)
    {
        /* Save the trusted device mask */
        p_dev_rec->sec_flags |= BTM_SEC_AUTHORIZED;
        p_dev_rec->trusted_mask[0] = trusted_mask[0];
        p_dev_rec->trusted_mask[1] = trusted_mask[1];
    }

    if (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHORIZING)
        return;

    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;
    old_state = BTM_SEC_STATE_IDLE;

    if (res != BTM_SUCCESS)
    {
        if (p_dev_rec->p_callback)
        {
            /* btm_clr_client_scn: clear the originator's SCN if a multiplexer */
            if (p_dev_rec->is_originator)
            {
                if (p_dev_rec->p_cur_service)
                {
                    if (p_dev_rec->p_cur_service->mx_proto_id == BT_PSM_RFCOMM)
                        p_dev_rec->p_cur_service->orig_mx_chan_id = 0;
                }
            }

            (*p_dev_rec->p_callback)(bd_addr, p_dev_rec->p_ref_data, res);
            p_dev_rec->p_callback = NULL;
        }
    }
    else
    {
        result = btm_sec_execute_procedure(p_dev_rec);

        if (result != BTM_CMD_STARTED && p_dev_rec->p_callback)
        {
            /* btm_clr_client_scn: clear the originator's SCN if a multiplexer */
            if (p_dev_rec->is_originator)
            {
                if (p_dev_rec->p_cur_service)
                {
                    if (p_dev_rec->p_cur_service->mx_proto_id == BT_PSM_RFCOMM)
                        p_dev_rec->p_cur_service->orig_mx_chan_id = 0;
                }
            }

            (*p_dev_rec->p_callback)(bd_addr, p_dev_rec->p_ref_data, result);
            p_dev_rec->p_callback = NULL;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  BTM_SecBond -- initiate bonding with a peer device.                */
/* ------------------------------------------------------------------ */
UINT8 BTM_SecBond(BD_ADDR bd_addr, UINT8 pin_len, UINT8 *p_pin, UINT32 trusted_mask[])
{
    BtmSecDevRec *p_dev_rec;
    tBTM_INQ_INFO *p_inq_info;
    UINT8 r1 = 0;
    UINT8 status;

    BTM_TRACE_API6("BTM_SecBond BDA: %02x:%02x:%02x:%02x:%02x:%02x",
                   bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

    /* Make sure the device is not already bonded */
    if (pin_len > PIN_CODE_LEN)
    {
        BTM_TRACE_ERROR1("BTM_SecBond: Illegal Pin len:%d", pin_len);
        return (BTM_ILLEGAL_VALUE);
    }

    if ((p_dev_rec = btm_find_dev(bd_addr)) == NULL)
    {
        if ((p_dev_rec = btm_sec_alloc_dev(bd_addr)) == NULL)
        {
            BTM_TRACE_ERROR0("BTM_SecBond: no device block");
            return (BTM_NO_RESOURCES);
        }
    }

    /* If already bonded, return */
    if ((p_dev_rec->hci_handle != HCI_INVALID_HANDLE) &&
        (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED))
    {
        BTM_TRACE_WARNING0("BTM_SecBond -> Already Paired");
        return (BTM_SUCCESS);
    }

    /* Remove any cached link key */
    if (BTM_DeleteStoredLinkKey(bd_addr, NULL))
        return (BTM_NO_RESOURCES);

    /* Put the device in the bonding state */
    p_dev_rec->security_required = (UINT8)BTM_SEC_OUT_LEVEL4;
    p_dev_rec->is_originator = TRUE;
    p_dev_rec->trusted_mask[0] = trusted_mask[0];
    p_dev_rec->trusted_mask[1] = trusted_mask[1];

    /* Save the PIN code if we got one */
    btm_cb.pin_code_len = pin_len;
    if (pin_len)
        memcpy(btm_cb.pin_code, p_pin, pin_len);

    p_dev_rec->sec_flags &= ~(BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED | BTM_SEC_LINK_KEY_KNOWN);

    /* If the device is a "phone", set the pin type */
    if ((p_dev_rec->dev_class[1] & 0x1f) == 0x05 && (p_dev_rec->dev_class[2] & 0x40)
        && btm_cb.pin_type != HCI_PIN_TYPE_FIXED)
    {
        btm_cb.pin_type_changed = TRUE;
        btsnd_hcic_write_pin_type(HCI_PIN_TYPE_FIXED);
    }

    btm_cb.disconnect_after_bond_reason = 0;

    /* If the device is already connected, do the bonding now */
    if (p_dev_rec->hci_handle != HCI_INVALID_HANDLE)
    {
        p_dev_rec->sec_flags &= ~BTM_SEC_LINK_KEY_KNOWN;
        p_dev_rec->sec_state = BTM_SEC_STATE_AUTHENTICATING;

        if (!btsnd_hcic_auth_request(p_dev_rec->hci_handle))
            return (BTM_NO_RESOURCES);

        btm_cb.bond_created_connection = FALSE;
        return (BTM_CMD_STARTED);
    }

    /* Make sure the controller is not doing something else */
    if (btm_cb.security_mode != BTM_SEC_MODE_LINK)
    {
        BTM_TRACE_DEBUG0("BTM_SecBond: Authen Enable -> TRUE");

        if (!btsnd_hcic_write_auth_enable(TRUE))
        {
            BTM_TRACE_ERROR0("BTM_SecBond: no buffer");
            return (BTM_NO_RESOURCES);
        }

        btm_cb.security_mode_changed = TRUE;
    }

    btm_cb.bond_created_connection = TRUE;

    r1 = (BTM_ReadLocalFeatures()[0] >> 5) & 1;

    p_dev_rec->sec_acl_connected = FALSE;

    if ((p_inq_info = BTM_InqDbRead(bd_addr)) != NULL)
    {
        status = btsnd_hcic_create_conn(bd_addr, 0x18, p_inq_info->page_scan_rep_mode,
                                        p_inq_info->page_scan_mode,
                                        (UINT16)(p_inq_info->clock_offset | BTM_CLOCK_OFFSET_VALID),
                                        r1);
    }
    else
    {
        status = btsnd_hcic_create_conn(bd_addr, 0x18, HCI_PAGE_SCAN_REP_MODE_R1,
                                        HCI_PAGE_SCAN_MODE_MANDATORY, p_dev_rec->clock_offset, r1);
    }

    if (status)
        return (BTM_CMD_STARTED);

    BTM_TRACE_ERROR0("BTM_SecBond: no buffer");

    /* If we were in a different security mode, restore it */
    if (btm_cb.security_mode_changed)
    {
        btm_cb.security_mode_changed = FALSE;
        BTM_TRACE_DEBUG1("btm_restore_mode: Authen Enable -> %d", (btm_cb.security_mode == BTM_SEC_MODE_LINK));
        btsnd_hcic_write_auth_enable(btm_cb.security_mode == BTM_SEC_MODE_LINK);
    }

    return (BTM_NO_RESOURCES);
}

/* ------------------------------------------------------------------ */
/*  String-pool orphans: BTM_SetEncryption strings (see note above).   */
/* ------------------------------------------------------------------ */
char *const btm_sec_pool_enc_strings[] = {
    "Security Manager: BTM_SetEncryption not connected",
    "Security Manager: BTM_SetEncryption already encrypted",
    "Security Manager: BTM_SetEncryption busy",
    "Security Manager: BTM_SetEncryption Handle:%d State:%d Flags:0x%x Required:0x%x",
};

/* ------------------------------------------------------------------ */
/*  btm_sec_l2cap_access_req -- grant/deny an L2CAP connection        */
/*  on a specific PSM.                                                */
/* ------------------------------------------------------------------ */
UINT8 btm_sec_l2cap_access_req(BD_ADDR bd_addr, UINT16 psm, UINT16 handle,
                               UINT8 is_originator,
                               BtmSecCallback p_callback, void *p_ref_data)
{
    BtmSecDevRec *p_dev_rec;
    BtmSecServRec *p_srec;
    BtmSecServRec *p_srec2;
    int i;
    UINT8 security_required;
    UINT8 result;
    UINT8 found;

    /* Make sure the device is in our database, allocate one if not */
    if ((p_dev_rec = btm_find_dev(bd_addr)) == NULL)
    {
        if ((p_dev_rec = btm_sec_alloc_dev(bd_addr)) == NULL)
        {
            BTM_TRACE_WARNING1("Security Manager: l2cap_access_req PSM:%d no resources", psm);

            if (p_callback)
                (*p_callback)(bd_addr, 0, BTM_NO_RESOURCES);

            return (BTM_NO_RESOURCES);
        }
    }

    p_dev_rec->hci_handle = handle;

    /* Find the service record for the PSM */
    p_srec = btm_sec_find_first_serv(is_originator, psm);

    if (p_srec == NULL)
    {
        BTM_TRACE_WARNING1("Security Manager: l2cap_access_req PSM:%d no application registerd", psm);

        if (p_callback)
            (*p_callback)(bd_addr, 0, BTM_MODE_UNSUPPORTED);

        return (BTM_MODE_UNSUPPORTED);
    }

    /* If not originator of the connection, or the device's service record
       is not set, set it */
    if (is_originator == FALSE || !p_dev_rec->p_cur_service)
        p_dev_rec->p_cur_service = p_srec;

    /* See if there is another service record with the same PSM
       (i.e. the PSM is a multiplexer) */
    p_srec2 = btm_sec_find_next_serv(p_srec);

    if (p_srec2 != NULL)
    {
        BTM_TRACE_EVENT1("Security Manager: l2cap_access_req PSM:%d postponed for multiplexer", psm);

        if (p_callback)
            (*p_callback)(bd_addr, 0, BTM_SUCCESS);

        return (BTM_SUCCESS);
    }

    security_required = p_srec->security_flags;

    p_dev_rec->p_callback = p_callback;
    p_dev_rec->p_ref_data = NULL;
    p_dev_rec->security_required = security_required;
    p_dev_rec->is_originator = is_originator;

    BTM_TRACE_EVENT5("Security Manager: l2cap_access_req PSM:%d Handle:%d State:%d Flags:0x%x Required:0x%x",
                     psm, p_dev_rec->hci_handle, p_dev_rec->sec_state, p_dev_rec->sec_flags,
                     p_dev_rec->security_required);

    if (!is_originator && (security_required & BTM_SEC_AUTHORIZED))
    {
        p_dev_rec->sec_flags &= ~(BTM_SEC_AUTHORIZED | BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED);

        BTM_TRACE_EVENT3("Security Manager: trusted:0x%04x%04x Flags:0x%x",
                         p_dev_rec->trusted_mask[1], p_dev_rec->trusted_mask[0], p_dev_rec->sec_flags);
    }

    result = btm_sec_execute_procedure(p_dev_rec);
    if (result != BTM_CMD_STARTED && p_callback)
    {
        (*p_callback)(bd_addr, p_dev_rec->p_ref_data, result);
        p_dev_rec->p_callback = NULL;
    }

    return (result);
}

/* ------------------------------------------------------------------ */
/*  btm_sec_mx_access_request -- access request for a multiplexer      */
/*  protocol (e.g. RFCOMM) channel.                                    */
/* ------------------------------------------------------------------ */
UINT8 btm_sec_mx_access_request(BD_ADDR bd_addr, UINT16 psm, BOOLEAN is_originator,
                                UINT32 mx_proto_id, UINT32 mx_chan_id,
                                BtmSecCallback p_callback, void *p_ref_data)
{
    BtmSecDevRec *p_dev_rec;
    BtmSecServRec *p_srec;
    int i;
    UINT8 result;

    /* Make sure the device is in our database, allocate one if not */
    if ((p_dev_rec = btm_find_dev(bd_addr)) == NULL)
    {
        if ((p_dev_rec = btm_sec_alloc_dev(bd_addr)) == NULL)
        {
            if (p_callback)
                (*p_callback)(bd_addr, p_ref_data, BTM_NO_RESOURCES);

            return;
        }
    }

    /* Find the service record for the PSM and protocol */
    p_srec = btm_sec_find_mx_serv(psm, is_originator, mx_proto_id, mx_chan_id);

    if (p_srec == NULL)
    {
        if (p_callback)
            (*p_callback)(bd_addr, p_ref_data, BTM_MODE_UNSUPPORTED);

        BTM_TRACE_ERROR3("Security Manager: MX service not found PSM:%d Proto:%d SCN:%d",
                         psm, mx_proto_id, mx_chan_id);

        return;
    }

    p_dev_rec->p_cur_service = p_srec;
    p_dev_rec->security_required = p_srec->security_flags;
    p_dev_rec->is_originator = is_originator;
    p_dev_rec->p_callback = p_callback;
    p_dev_rec->p_ref_data = p_ref_data;

    p_dev_rec->sec_flags &= ~BTM_SEC_AUTHORIZED;

    result = btm_sec_execute_procedure(p_dev_rec);

    if (result == BTM_CMD_STARTED)
        return;

    if (p_callback)
    {
        /* btm_clr_client_scn: clear the originator's SCN if a multiplexer */
        if (p_dev_rec->is_originator)
        {
            if (p_dev_rec->p_cur_service)
            {
                if (p_dev_rec->p_cur_service->mx_proto_id == BT_PSM_RFCOMM)
                    p_dev_rec->p_cur_service->orig_mx_chan_id = 0;
            }
        }

        (*p_callback)(bd_addr, p_ref_data, result);
        p_dev_rec->p_callback = NULL;
    }
}

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

/* ------------------------------------------------------------------ */
/*  btm_sec_rmt_name_request_complete -- remote name request complete  */
/*  event from the HCI layer.                                          */
/* ------------------------------------------------------------------ */
void btm_sec_rmt_name_request_complete(UINT8 *bd_addr, UINT8 *bd_name, UINT8 status)
{
    BtmSecDevRec *p_dev_rec;
    int i;

    if (bd_addr)
    {
        p_dev_rec = btm_find_dev(bd_addr);
    }
    else
    {
        /* Look for the device record that is waiting for the name */
        p_dev_rec = &btm_cb.sec_dev_rec[0];
        for (i = 0; i < BTM_SEC_MAX_DEVICE_RECORDS; i++, p_dev_rec++)
        {
            if ((p_dev_rec->sec_flags & BTM_SEC_IN_USE)
                && (p_dev_rec->sec_state == BTM_SEC_STATE_GETTING_NAME))
            {
                break;
            }
        }
        if (i == BTM_SEC_MAX_DEVICE_RECORDS)
            p_dev_rec = NULL;
    }

    BTM_TRACE_EVENT2("Security Manager: rmt_name_complete status:%d State:%d",
                     status, p_dev_rec ? p_dev_rec->sec_state : 0);

    if (p_dev_rec == NULL)
        return;

    if (status == BTM_SUCCESS)
    {
        memcpy(p_dev_rec->sec_bd_name, bd_name, 64);

        p_dev_rec->sec_flags |= BTM_SEC_NAME_KNOWN;

        for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++)
        {
            if (btm_cb.p_rmt_name_callback[i])
                (*btm_cb.p_rmt_name_callback[i])(bd_addr, p_dev_rec->dev_class,
                                                 (UINT8 *)p_dev_rec->sec_bd_name);
        }
    }

    /* If we were bonding, and the name has been resolved, send the link key */
    if (p_dev_rec->link_key_not_sent)
    {
        if ((p_dev_rec->security_required & BTM_SEC_BOND) && !p_dev_rec->sec_acl_connected)
            return;

        p_dev_rec->link_key_not_sent = FALSE;

        if (btm_cb.api.p_link_key_callback)
            (*btm_cb.api.p_link_key_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                              p_dev_rec->sec_bd_name, p_dev_rec->link_key,
                                              p_dev_rec->link_key_type);

        for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++)
        {
            if (btm_cb.p_link_key_callback[i])
                (*btm_cb.p_link_key_callback[i])(p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                                 p_dev_rec->sec_bd_name, p_dev_rec->link_key,
                                                 p_dev_rec->link_key_type);
        }
    }

    if ((p_dev_rec->security_required & BTM_SEC_BOND) && (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED))
    {
        p_dev_rec->security_required &= ~BTM_SEC_OUT_LEVEL4;
        btsnd_hcic_disconnect(p_dev_rec->hci_handle, HCI_ERR_PEER_USER);
        return;
    }

    if (p_dev_rec->sec_state != BTM_SEC_STATE_GETTING_NAME)
        return;

    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

    if (status != BTM_SUCCESS)
    {
        if (p_dev_rec->p_callback)
        {
            (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, BTM_ERR_PROCESSING);
            p_dev_rec->p_callback = NULL;
        }
    }
    else
    {
        status = btm_sec_execute_procedure(p_dev_rec);

        if (status != BTM_CMD_STARTED && p_dev_rec->p_callback)
        {
            (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, status);
            p_dev_rec->p_callback = NULL;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  btm_sec_auth_complete -- HCI authentication complete event.        */
/* ------------------------------------------------------------------ */
void btm_sec_auth_complete(UINT16 handle, UINT8 status)
{
    BtmSecDevRec *p_dev_rec;
    BtmSecDevRec *p_dev_rec2;
    UINT8 old_state;

    p_dev_rec = btm_find_dev_by_handle(handle);

    BTM_TRACE_EVENT2("Security Manager: auth_complete status:%d State:%d",
                     status, p_dev_rec ? p_dev_rec->sec_state : 0);

    /* If we are bonding, restore the mode */
    if (btm_cb.security_mode_changed)
    {
        btm_cb.security_mode_changed = FALSE;
        BTM_TRACE_DEBUG1("btm_restore_mode: Authen Enable -> %d", (btm_cb.security_mode == BTM_SEC_MODE_LINK));
        btsnd_hcic_write_auth_enable(btm_cb.security_mode == BTM_SEC_MODE_LINK);
    }

    if (btm_cb.pin_type_changed)
    {
        btm_cb.pin_type_changed = FALSE;
        btsnd_hcic_write_pin_type(btm_cb.pin_type);
    }

    /* If we are bonding, and the connection was not created by us, disconnect */
    if (p_dev_rec && (p_dev_rec->security_required & BTM_SEC_BOND) && !btm_cb.bond_created_connection)
    {
        UINT8 reason;

        p_dev_rec->security_required &= ~BTM_SEC_OUT_LEVEL4;

        if (btm_cb.disconnect_after_bond_reason)
        {
            reason = btm_cb.disconnect_after_bond_reason;

            if ((p_dev_rec2 = btm_find_dev_by_handle(handle)) != NULL)
            {
                if (p_dev_rec2->security_required & BTM_SEC_BOND)
                {
                    btm_cb.disconnect_after_bond_reason = reason;
                }
                else
                {
                    old_state = p_dev_rec2->sec_state;
                    if (old_state != BTM_SEC_STATE_SWITCHING_ROLE)
                    {
                        p_dev_rec2->sec_state = BTM_SEC_STATE_SWITCHING_ROLE;
                        if (!btsnd_hcic_disconnect(handle, reason))
                            p_dev_rec2->sec_state = old_state;
                    }
                }
            }
        }
    }

    if (p_dev_rec == NULL)
        return;

    if (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHENTICATING)
    {
        if (btm_cb.api.p_auth_complete_callback && status != HCI_SUCCESS)
        {
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                                   p_dev_rec->sec_bd_name, status);
        }
    }
    else
    {
        /* If the status is COMMAND_DISALLOWED and the link is already encrypted,
           treat it as success */
        if (status == HCI_ERR_COMMAND_DISALLOWED
            && (p_dev_rec->sec_flags & (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED))
               == (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED))
        {
            status = HCI_SUCCESS;
        }

        if (btm_cb.api.p_auth_complete_callback)
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                                   p_dev_rec->sec_bd_name, status);

        p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

        if (p_dev_rec->security_required & BTM_SEC_BOND)
        {
            p_dev_rec->security_required &= ~BTM_SEC_OUT_LEVEL4;
            btsnd_hcic_disconnect(p_dev_rec->hci_handle, HCI_ERR_PEER_USER);
        }
        else if (status != HCI_SUCCESS)
        {
            if (p_dev_rec->p_callback)
            {
                /* btm_clr_client_scn: clear the originator's SCN if a multiplexer */
                if (p_dev_rec->is_originator)
                {
                    if (p_dev_rec->p_cur_service)
                    {
                        if (p_dev_rec->p_cur_service->mx_proto_id == BT_PSM_RFCOMM)
                            p_dev_rec->p_cur_service->orig_mx_chan_id = 0;
                    }
                }

                (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, BTM_ERR_PROCESSING);
                p_dev_rec->p_callback = NULL;
            }

            btsnd_hcic_disconnect(p_dev_rec->hci_handle, HCI_ERR_AUTH_FAILURE);
        }
        else
        {
            p_dev_rec->sec_flags |= BTM_SEC_AUTHENTICATED;

            status = btm_sec_execute_procedure(p_dev_rec);

            if (status != BTM_CMD_STARTED && p_dev_rec->p_callback)
            {
                /* btm_clr_client_scn: clear the originator's SCN if a multiplexer */
                if (p_dev_rec->is_originator)
                {
                    if (p_dev_rec->p_cur_service)
                    {
                        if (p_dev_rec->p_cur_service->mx_proto_id == BT_PSM_RFCOMM)
                            p_dev_rec->p_cur_service->orig_mx_chan_id = 0;
                    }
                }

                (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, status);
                p_dev_rec->p_callback = NULL;
            }
        }
    }
}

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

/* ------------------------------------------------------------------ */
/*  btm_sec_connected -- ACL link connected event.                     */
/* ------------------------------------------------------------------ */
void btm_sec_connected(UINT8 *bda, UINT16 handle, UINT8 status, UINT8 enc_mode)
{
    BtmSecDevRec *p_dev_rec;
    int i;

    /* See if we have a link key for the peer device */
    p_dev_rec = btm_find_dev(bda);

    BTM_TRACE_EVENT3("Security Manager: btm_sec_connected handle:%d status:%d enc_mode:%d",
                     handle, status, enc_mode);

    if (p_dev_rec == NULL)
    {
        /* If the device is not in our database, allocate a record */
        if (status == HCI_SUCCESS)
        {
            if ((p_dev_rec = btm_sec_alloc_dev(bda)) == NULL)
                return;
        }
        else
        {
            return;
        }
    }
    else
    {
        p_dev_rec->timestamp = btm_cb.dev_rec_count++;
        p_dev_rec->sec_acl_connected = TRUE;
    }

    /* If we are bonding, restore the mode */
    if (p_dev_rec->security_required & BTM_SEC_BOND)
    {
        if (btm_cb.security_mode_changed)
        {
            btm_cb.security_mode_changed = FALSE;
            BTM_TRACE_DEBUG1("btm_restore_mode: Authen Enable -> %d", (btm_cb.security_mode == BTM_SEC_MODE_LINK));
            btsnd_hcic_write_auth_enable(btm_cb.security_mode == BTM_SEC_MODE_LINK);
        }

        if (btm_cb.pin_type_changed)
        {
            btm_cb.pin_type_changed = FALSE;
            btsnd_hcic_write_pin_type(btm_cb.pin_type);
        }
    }

    /* If the connection failed, notify the application */
    if (status != HCI_SUCCESS)
    {
        /* If the device was bonding, or the failure is a known one,
           notify the auth complete callback */
        if ((status == HCI_ERR_AUTH_FAILURE || status == HCI_ERR_KEY_MISSING ||
             status == HCI_ERR_HOST_REJECT_SECURITY || status == HCI_ERR_PAIRING_NOT_ALLOWED ||
             status == HCI_ERR_UNIT_KEY_USED || status == HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED ||
             status == HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE || status == HCI_ERR_REPEATED_ATTEMPTS ||
             status == HCI_ERR_LMP_RESPONSE_TIMEOUT) || (p_dev_rec->security_required & BTM_SEC_BOND))
        {
            p_dev_rec->security_required &= ~BTM_SEC_OUT_LEVEL4;
            p_dev_rec->sec_flags &= ~BTM_SEC_LINK_KEY_KNOWN;

            /* If we were waiting for a PIN code, send the negative reply */
            if (btm_cb.sec_pin_code_req_tle.param)
            {
                btu_stop_timer(&btm_cb.sec_pin_code_req_tle);
                btm_cb.sec_pin_code_req_tle.param = 0;
                status = HCI_ERR_HOST_REJECT_SECURITY;
            }

            if (btm_cb.acl_disc_reason == HCI_ERR_HOST_REJECT_SECURITY)
                status = HCI_ERR_HOST_REJECT_SECURITY;

            if (btm_cb.api.p_auth_complete_callback)
                (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                                       p_dev_rec->sec_bd_name, status);
        }
        else
        {
            /* Report the failure to the application */
            if (p_dev_rec->p_callback)
            {
                /* btm_clr_client_scn: clear the originator's SCN if a multiplexer */
                if (p_dev_rec->is_originator)
                {
                    if (p_dev_rec->p_cur_service)
                    {
                        if (p_dev_rec->p_cur_service->mx_proto_id == BT_PSM_RFCOMM)
                            p_dev_rec->p_cur_service->orig_mx_chan_id = 0;
                    }
                }

                if (status == HCI_ERR_CONNECTION_TOUT || status == HCI_ERR_UNSPECIFIED
                    || status == HCI_ERR_PAGE_TIMEOUT)
                    (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, BTM_DEVICE_TIMEOUT);
                else
                    (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, BTM_ERR_PROCESSING);
            }
        }
        return;
    }

    /* If the device is bonded, and the link key has not been sent, send it */
    if ((p_dev_rec->security_required & BTM_SEC_BOND) && p_dev_rec->link_key_not_sent)
    {
        p_dev_rec->link_key_not_sent = FALSE;

        if (btm_cb.api.p_link_key_callback)
            (*btm_cb.api.p_link_key_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                              p_dev_rec->sec_bd_name, p_dev_rec->link_key,
                                              p_dev_rec->link_key_type);

        for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++)
        {
            if (btm_cb.p_link_key_callback[i])
                (*btm_cb.p_link_key_callback[i])(p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                                 p_dev_rec->sec_bd_name, p_dev_rec->link_key,
                                                 p_dev_rec->link_key_type);
        }

        if (btm_cb.api.p_auth_complete_callback)
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                                   p_dev_rec->sec_bd_name, BTM_SUCCESS);

        p_dev_rec->security_required &= ~BTM_SEC_OUT_LEVEL4;
        btsnd_hcic_disconnect(handle, HCI_ERR_PEER_USER);
        return;
    }

    p_dev_rec->hci_handle = handle;
    p_dev_rec->sec_flags &= ~(BTM_SEC_AUTHORIZED | BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED);
    if (enc_mode)
        p_dev_rec->sec_flags |= (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED);

    if (btm_cb.security_mode == BTM_SEC_MODE_LINK)
        p_dev_rec->sec_flags |= BTM_SEC_AUTHENTICATED;

    p_dev_rec->link_key_changed = FALSE;

    /* If the device name is not known, or we are the originator, execute
       the security procedure */
    if (!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) || p_dev_rec->is_originator)
    {
        status = btm_sec_execute_procedure(p_dev_rec);

        if (status != BTM_CMD_STARTED && p_dev_rec->p_callback)
        {
            /* btm_clr_client_scn: clear the originator's SCN if a multiplexer */
            if (p_dev_rec->is_originator)
            {
                if (p_dev_rec->p_cur_service)
                {
                    if (p_dev_rec->p_cur_service->mx_proto_id == BT_PSM_RFCOMM)
                        p_dev_rec->p_cur_service->orig_mx_chan_id = 0;
                }
            }

            (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, status);
            p_dev_rec->p_callback = NULL;
        }
    }
}

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

/* ------------------------------------------------------------------ */
/*  btm_sec_link_key_notification -- HCI link key notification event.  */
/* ------------------------------------------------------------------ */
void btm_sec_link_key_notification(UINT8 *p_bda, UINT8 *p_link_key, UINT8 key_type)
{
    BtmSecDevRec *p_dev_rec;
    int i;

    if ((p_dev_rec = btm_find_dev(p_bda)) == NULL)
    {
        if ((p_dev_rec = btm_sec_alloc_dev(p_bda)) == NULL)
            return;
    }

    BTM_TRACE_EVENT6("btm_sec_link_key_notification()  BDA: %02x:%02x:%02x:%02x:%02x:%02x",
                     p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);
    BTM_TRACE_EVENT1("                                TYPE: %d", key_type);

    /* Save the link key in the device record */
    if (btm_cb.local_version.hci_version < HCI_VERSION_1_1)
        p_dev_rec->link_key_type = BTM_LKEY_TYPE_IGNORE;
    else
        p_dev_rec->link_key_type = key_type;

    p_dev_rec->sec_flags |= BTM_SEC_LINK_KEY_KNOWN;
    memcpy(p_dev_rec->link_key, p_link_key, LINK_KEY_LEN);

    /* If we are not bonding, or the name is known and the connection is up,
       notify the application of the link key */
    if (!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN)
        || ((p_dev_rec->security_required & BTM_SEC_BOND) && !p_dev_rec->sec_acl_connected))
    {
        p_dev_rec->link_key_not_sent = TRUE;
        return;
    }

    if (btm_cb.api.p_link_key_callback)
        (*btm_cb.api.p_link_key_callback)(p_bda, p_dev_rec->dev_class,
                                          p_dev_rec->sec_bd_name, p_link_key,
                                          p_dev_rec->link_key_type);

    for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++)
    {
        if (btm_cb.p_link_key_callback[i])
            (*btm_cb.p_link_key_callback[i])(p_bda, p_dev_rec->dev_class,
                                             p_dev_rec->sec_bd_name, p_link_key,
                                             p_dev_rec->link_key_type);
    }
}

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
/*  btm_sec_pin_code_request -- HCI PIN code request event.            */
/* ------------------------------------------------------------------ */
void btm_sec_pin_code_request(UINT8 *p_bda)
{
    BtmSecDevRec *p_dev_rec;
    int i;
    int dev_rec_count = 0;
    BOOLEAN reject = FALSE;

    BTM_TRACE_EVENT6("btm_sec_pin_code_request()  BDA: %02x:%02x:%02x:%02x:%02x:%02x",
                     p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);

    /* If we are pairing with a fixed pin device, or the pin type is fixed,
       send the configured pin code */
    if (!btm_cb.pairing_disabled && btm_cb.pin_type == HCI_PIN_TYPE_FIXED)
    {
        if (btm_find_dev(p_bda) == NULL)
        {
            for (; dev_rec_count < BTM_SEC_MAX_DEVICE_RECORDS; dev_rec_count++)
            {
                if (!(btm_cb.sec_dev_rec[dev_rec_count].sec_flags & BTM_SEC_IN_USE))
                    break;
            }
            if (dev_rec_count >= BTM_SEC_MAX_DEVICE_RECORDS)
            {
                btsnd_hcic_pin_code_neg_reply(p_bda);
                return;
            }
        }
        btsnd_hcic_pin_code_req_reply(p_bda, btm_cb.cfg_pin_code_len, btm_cb.cfg_pin_code);
        return;
    }

    p_dev_rec = btm_find_dev(p_bda);
    if (p_dev_rec == NULL)
        p_dev_rec = btm_sec_alloc_dev(p_bda);

    memcpy(btm_cb.sec_pin_code_req_bd_addr, p_bda, BD_ADDR_LEN);

    if (p_dev_rec != NULL)
    {
        if (memcmp(p_bda, btm_cb.connecting_bda, BD_ADDR_LEN) == 0)
            memcpy(p_dev_rec->dev_class, btm_cb.connecting_dc, DEV_CLASS_LEN);

        if (p_dev_rec->security_required & BTM_SEC_BOND)
        {
            BTM_TRACE_EVENT0("btm_sec_pin_code_request bonding sending reply");
            btsnd_hcic_pin_code_req_reply(p_bda, btm_cb.pin_code_len, btm_cb.pin_code);

            /* If we were in a different security mode, restore it */
            if (btm_cb.security_mode_changed)
            {
                btm_cb.security_mode_changed = FALSE;
                BTM_TRACE_DEBUG1("btm_sec_pin_code_request: Authen Enable -> %d",
                                 (btm_cb.security_mode == BTM_SEC_MODE_LINK));
                btsnd_hcic_write_auth_enable(btm_cb.security_mode == BTM_SEC_MODE_LINK);
            }

            return;
        }

        if (btm_cb.pairing_disabled || !btm_cb.api.p_pin_callback
            || (!p_dev_rec->is_originator && (p_dev_rec->dev_class[1] & 0x1f) == 0x05
                && (p_dev_rec->dev_class[2] & 0x40)))
        {
            reject = TRUE;
        }
        else
        {
            memcpy(btm_cb.connecting_bda, p_bda, BD_ADDR_LEN);
            memcpy(btm_cb.connecting_dc, p_dev_rec->dev_class, DEV_CLASS_LEN);
            btm_cb.sec_pin_code_req_tle.param = (void *)btm_sec_pin_code_request_timeout;
            btu_start_timer(&btm_cb.sec_pin_code_req_tle, BTU_TTYPE_USER_FUNC, BTM_PIN_CODE_REQUEST_TOUT);
            (*btm_cb.api.p_pin_callback)(p_bda, p_dev_rec->dev_class, p_dev_rec->sec_bd_name);
        }
    }
    else
    {
        reject = TRUE;
    }

    if (reject)
    {
        BTM_TRACE_WARNING3("btm_sec_pin_code_request(): Pairing disabled:%d; PIN callback:%x, Dev Rec:%x!",
                           btm_cb.pairing_disabled, btm_cb.api.p_pin_callback, p_dev_rec);
        btsnd_hcic_pin_code_neg_reply(p_bda);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_sec_execute_procedure -- execute the security procedure for    */
/*  the specified device record.                                       */
/* ------------------------------------------------------------------ */
#pragma auto_inline off
UINT8 btm_sec_execute_procedure(BtmSecDevRec *p_dev_rec)
{
    UINT8 sec_state;
    UINT8 result;
    void *p_lcb;

    BTM_TRACE_EVENT3("btm_sec_execute_procedure: Required:0x%x Flags:0x%x State:%d",
                     p_dev_rec->security_required, p_dev_rec->sec_flags, p_dev_rec->sec_state);

    /* If procedure already in progress, do not execute again */
    if (p_dev_rec->sec_state != BTM_SEC_STATE_IDLE)
        return (BTM_CMD_STARTED);

    /* If the device is not connected, connect */
    if (p_dev_rec->hci_handle == HCI_INVALID_HANDLE)
    {
        /* Check if L2CAP is holding a connection */
        if ((p_lcb = l2cu_allocate_lcb(p_dev_rec->bd_addr)) == NULL)
        {
            L2CAP_TRACE_WARNING0("L2CAP - no LCB for L2CA_conn_req");
            return (BTM_NO_RESOURCES);
        }

        if (!l2cu_create_conn(p_lcb))
            return (BTM_NO_RESOURCES);

        p_dev_rec->is_originator = TRUE;
        return (BTM_CMD_STARTED);
    }

    /* If we don't know the device name yet, get it */
    if (!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) && (p_dev_rec->hci_handle != HCI_INVALID_HANDLE))
    {
        BTM_TRACE_EVENT0("Security Manager: Start get name");

        sec_state = p_dev_rec->sec_state;
        p_dev_rec->sec_state = BTM_SEC_STATE_GETTING_NAME;

        if (btm_initiate_rem_name(p_dev_rec->bd_addr, NULL, BTM_RMT_NAME_SEC, 30, NULL) != BTM_CMD_STARTED)
        {
            p_dev_rec->sec_state = sec_state;
            result = 0;
        }
        else
            result = 1;

        if (result != 0)
        {
            return (BTM_CMD_STARTED);
        }
        else
        {
            p_dev_rec->sec_flags |= BTM_SEC_NAME_KNOWN;
        }
    }

    /* If the device is not authenticated, authenticate */
    if (!(p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED)
        && (((p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_OUT_AUTHENTICATE))
            || (!p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_IN_AUTHENTICATE)))))
    {
        if (p_dev_rec->hci_handle != HCI_INVALID_HANDLE)
        {
            BTM_TRACE_EVENT0("Security Manager: Start authentication");
            p_dev_rec->sec_state = BTM_SEC_STATE_AUTHENTICATING;

            if (!btsnd_hcic_auth_request(p_dev_rec->hci_handle))
                return (BTM_NO_RESOURCES);

            return (BTM_CMD_STARTED);
        }
    }

    /* If the link is not encrypted, encrypt it */
    if (!(p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED)
        && (((p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_OUT_ENCRYPT))
            || (!p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_IN_ENCRYPT)))))
    {
        if (p_dev_rec->hci_handle != HCI_INVALID_HANDLE)
        {
            BTM_TRACE_EVENT0("Security Manager: Start encryption");

            if (btsnd_hcic_set_conn_encrypt(p_dev_rec->hci_handle, TRUE) == FALSE)
                result = 0;
            else
            {
                p_dev_rec->sec_state = BTM_SEC_STATE_ENCRYPTING;
                result = 1;
            }

            if (result == 0)
                return (BTM_NO_RESOURCES);

            return (BTM_CMD_STARTED);
        }
    }

    /* If the device is not authorized, and the service requires authorization,
       check if it is trusted */
    if (!(p_dev_rec->sec_flags & BTM_SEC_AUTHORIZED)
        && (((p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_OUT_AUTHORIZE))
            || (!p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_IN_AUTHORIZE)))))
    {
        /* Check if the device is trusted */
        if (p_dev_rec->trusted_mask[0] == 0xFFFFFFFF)
        {
            if (p_dev_rec->trusted_mask[1] == 0xFFFFFFFF)
                result = 1;
            else
                result = 0;
        }
        else
            result = 0;

        if (result == 0)
        {
            /* If the service is not trusted, ask the application */
            if (p_dev_rec->p_cur_service->service_id < BTM_SEC_MAX_SERVICES)
            {
                if (!(p_dev_rec->trusted_mask[p_dev_rec->p_cur_service->service_id >> 5]
                     & (1 << (p_dev_rec->p_cur_service->service_id & 31))))
                {
                    BTM_TRACE_EVENT0("Security Manager: Start authorization");
                    return (btm_sec_start_authorization(p_dev_rec));
                }
            }
        }
    }

    /* All checks passed, grant access */
    p_dev_rec->security_required &= ~(BTM_SEC_OUT_AUTHORIZE | BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_ENCRYPT
                                    | BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT);

    BTM_TRACE_EVENT2("Security Manager: trusted:0x%04x%04x",
                     p_dev_rec->trusted_mask[1], p_dev_rec->trusted_mask[0]);
    BTM_TRACE_EVENT0("Security Manager: access granted");

    return (BTM_SUCCESS);
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
