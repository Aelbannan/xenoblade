// Decompiled btm_sec.c — high-level C reconstruction of the BTM security manager.
// Retail layout notes: the Wii binary's btm_cb differs from btm_int.h's tBTM_CB
// (24 service records of 0x3C, 16 device records of 0x88, PIN_CODE_LEN 6,
// pin_code_len is 32-bit). Local structs below mirror the retail offsets.

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
#define BTM_CLOCK_OFFSET_VALID          0x8000
#define BTM_PAIR_STATE_IDLE             0
#define BTM_PAIR_STATE_GET_REM_NAME     1

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
typedef struct
{
    void   *p_cur_service;       /* 0x00 */
    void   *p_callback;          /* 0x04 */
    void   *p_ref_data;          /* 0x08 */
    UINT32  timestamp;           /* 0x0C */
    UINT32  trusted_mask[2];     /* 0x10 */
    UINT16  hci_handle;          /* 0x18 */
    UINT16  clock_offset;        /* 0x1A */
    UINT8   bd_addr[6];          /* 0x1C */
    UINT8   dev_class[3];        /* 0x22 */
    UINT8   link_key[16];        /* 0x25 */
    char    sec_bd_name[65];     /* 0x35 */
    UINT8   sec_flags;           /* 0x76 */
    UINT8   features[8];         /* 0x77 */
    UINT8   sec_state;           /* 0x7F */
    UINT8   is_originator;       /* 0x80 */
    UINT8   role_master;         /* 0x81 */
    UINT16  security_required;   /* 0x82 */
    UINT8   link_key_not_sent;   /* 0x84 */
    UINT8   link_key_type;       /* 0x85 */
    UINT8   _pad[2];             /* 0x86-0x87 */
} BtmSecDevRec;                  /* total 0x88 */

/* ------------------------------------------------------------------ */
/*  Security application registration info (6 callback pointers)      */
/* ------------------------------------------------------------------ */
typedef struct
{
    void   *p_authorize_callback;        /* 0x00 */
    void   *p_auth_complete_callback;    /* 0x04 */
    void   *p_pin_callback;              /* 0x08 */
    void   *p_link_key_callback;         /* 0x0C */
    void   *p_link_key_req_callback;     /* 0x10 */
    void   *p_pairing_complete_callback; /* 0x14 */
} BtmApplInfo;                           /* total 0x18 */

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
    UINT8    _pad3[0x192C - 0x1924];             /* 0x1924-0x192B */
    void    *p_rmt_name_callback[BTM_SEC_MAX_RMT_NAME_CALLBACKS]; /* 0x192C-0x1933 */
    UINT8    _pad4[0x194C - 0x1934];             /* 0x1934-0x194B */
    UINT32   pin_code_len;                       /* 0x194C-0x194F */
    UINT8    _pad5[0x1954 - 0x1950];             /* 0x1950-0x1953 */
    UINT8    pin_code[BTM_PIN_CODE_LEN];         /* 0x1954-0x1959 */
    UINT8    _pad6[0x1978 - 0x195A];             /* 0x195A-0x1977 */
    UINT8    security_mode;                      /* 0x1978 */
    UINT8    _pad7[0x1990 - 0x1979];             /* 0x1979-0x198F */
    BtmSecServRec sec_serv_rec[BTM_SEC_MAX_SERVICE_RECORDS]; /* 0x1990-0x1F2F */
    BtmSecDevRec  sec_dev_rec[16];               /* 0x1F30-0x27AF */
    UINT8    _pad8[0x27B4 - 0x27B0];             /* 0x27B0-0x27B3 */
    UINT8    connecting_bda[6];                  /* 0x27B4-0x27B9 */
    UINT8    _pad9[0x27C0 - 0x27BA];             /* 0x27BA-0x27BF */
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

typedef struct
{
    UINT16 clock_offset;                     /* 0x00 tBTM_INQ_RESULTS.clock_offset */
} tBTM_INQ_INFO;

tBTM_INQ_INFO *BTM_InqDbRead(BD_ADDR p_bda);

void btsnd_hcic_write_pin_type(UINT8 pin_type);
void btsnd_hcic_pin_code_neg_reply(BD_ADDR bda);
void btsnd_hcic_write_auth_enable(UINT8 flag);
void btsnd_hcic_write_encr_mode(UINT8 mode);

/* ------------------------------------------------------------------ */
/*  BTM_SecRegister — register the application security callbacks.    */
/* ------------------------------------------------------------------ */
BOOLEAN BTM_SecRegister(BtmApplInfo *p_cb_info)
{
    btm_cb.api = *p_cb_info;
    BTM_TRACE_EVENT0("BTM_Sec: application registered");
    return TRUE;
}

/* ------------------------------------------------------------------ */
/*  BTM_SecAddRmtNameNotifyCallback — add a remote name callback.     */
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
/*  BTM_SecDeleteRmtNameNotifyCallback — remove a remote name cb.     */
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
/*  BTM_SetPinType — set the PIN type, code and length for pairing.   */
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
/*  BTM_SetSecurityLevel — register a service's security requirement. */
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
/*  btm_sec_init — initialise the security manager.                   */
/* ------------------------------------------------------------------ */
void btm_sec_init(UINT8 sec_mode)
{
    btm_cb.security_mode = sec_mode;
    memset(btm_cb.pin_code, 0xff, BTM_PIN_CODE_LEN);
}

/* ------------------------------------------------------------------ */
/*  btm_sec_dev_reset — re-enable authentication if security mode 3.  */
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
/*  btm_sec_is_bonding — check if the device is currently bonding.    */
/* ------------------------------------------------------------------ */
BOOLEAN btm_sec_is_bonding(UINT16 handle)
{
    BtmSecDevRec *p_dev_rec = btm_find_dev_by_handle(handle);

    if (p_dev_rec && (p_dev_rec->role_master & 0x40))
        return TRUE;
    return FALSE;
}

/* ------------------------------------------------------------------ */
/*  btm_sec_update_clock_offset — update clock offset of a device.    */
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
/*  btm_sec_pin_code_request_timeout — PIN code request timed out.    */
/* ------------------------------------------------------------------ */
void btm_sec_pin_code_request_timeout(void *p_tle)
{
    BTM_TRACE_EVENT0("btm_sec_pin_code_request_timeout()");

    btm_cb.pin_code_len = 0;
    memset(btm_cb.pin_code, 0xff, BTM_PIN_CODE_LEN);
    btsnd_hcic_pin_code_neg_reply(btm_cb.connecting_bda);
}

/* ------------------------------------------------------------------ */
/*  btm_read_trusted_mask — return the trusted service mask.          */
/* ------------------------------------------------------------------ */
UINT32 *btm_read_trusted_mask(BD_ADDR bd_addr)
{
    BtmSecDevRec *p_dev_rec = btm_find_dev(bd_addr);

    if (p_dev_rec)
        return p_dev_rec->trusted_mask;

    return NULL;
}

void BTM_PINCodeReply() {}

void BTM_DeviceAuthorized() {}

void BTM_SecBond() {}

void btm_sec_l2cap_access_req() {}

void btm_sec_mx_access_request() {}

void btm_sec_conn_req() {}

void btm_sec_abort_access_req() {}

void btm_sec_rmt_name_request_complete() {}

void btm_sec_auth_complete() {}

void btm_sec_mkey_comp_event() {}

void btm_sec_encrypt_change() {}

void btm_sec_connected() {}

void btm_sec_disconnect() {}

void btm_sec_disconnected() {}

void btm_sec_link_key_notification() {}

void btm_sec_link_key_request() {}

void btm_sec_pin_code_request() {}

void btm_sec_execute_procedure() {}

void btm_sec_start_authorization() {}

void btm_sec_collision_timeout() {}
