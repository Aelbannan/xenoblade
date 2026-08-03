/******************************************************************************
 *
 *  bta_dm_int.h — BTA device-manager message types (tBTA_DM_MSG).
 *
 *  Port of Broadcom's bta/bta/dm/bta_dm_int.h message union for the Wii-era
 *  (RVL SDK) BTE stack. The Wii-era layout differs from later AOSP bluedroid:
 *  every DM/Search message is an 8-byte BT_HDR followed by the payload at
 *  offset 0x08, and all payloads share one 0x110-byte GKI buffer
 *  (see bta_dm_api.c). Member offsets below are reverse-engineered from the
 *  retail binary and match the local structs previously declared in
 *  bta_dm_api.c / bta_dm_act.c / bta_dm_pm.c; this header consolidates them.
 *
 ******************************************************************************/
#ifndef RVL_SDK_BTA_DM_INT_H
#define RVL_SDK_BTA_DM_INT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* --- BT_HDR (bt_types.h) --- */

struct bta_dm_hdr_t {
    unsigned short event;           /* 0x00 */
    unsigned short len;             /* 0x02 */
    unsigned short offset;          /* 0x04 */
    unsigned short layer_specific;  /* 0x06 */
};

/* --- DM API message payloads (BT_HDR + payload at 0x08) --- */

/* tBTA_DM_API_ENABLE */
struct bta_dm_enable_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    void *p_sec_cback;              /* 0x08 */
};

/* tBTA_DM_API_SET_DEV_NAME */
struct bta_dm_set_name_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    char name[32];                  /* 0x08 */
};

/* tBTA_DM_API_SET_VISIBILITY (padded to the 0x110-byte tBTA_DM_MSG) */
struct bta_dm_set_visibility_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned char disc_mode;        /* 0x08 */
    unsigned char conn_mode;        /* 0x09 */
    unsigned char _pad[0x106];      /* 0x0a-0x10f */
};

/* tBTA_DM_API_BOND: (bd_addr, pin_len, p_pin) per BTM_SecBond() */
struct bta_dm_bond_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned char bd_addr[6];       /* 0x08-0x0d */
    unsigned char pin_len;          /* 0x0e */
    unsigned char p_pin[16];        /* 0x0f-0x1e */
};

/* tBTA_DM_API_PIN_REPLY */
struct bta_dm_pin_reply_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned char bd_addr[6];       /* 0x08-0x0d */
    unsigned char accept;           /* 0x0e */
    unsigned char pin_len;          /* 0x0f */
    unsigned char p_pin[16];        /* 0x10-0x1f */
};

/* tBTA_DM_API_AUTH_REPLY */
struct bta_dm_auth_reply_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned char bd_addr[6];       /* 0x08-0x0d */
    unsigned char srv_id;           /* 0x0e */
    unsigned char accept;           /* 0x0f */
};

/* tBTA_DM_API_SEARCH */
struct bta_dm_inq_t {
    unsigned char mode;             /* 0x00 */
    unsigned char duration;         /* 0x01 */
    unsigned char max_resps;        /* 0x02 */
    unsigned char report_dup;       /* 0x03 */
    unsigned char filter_cond[6];   /* 0x04-0x09 */
};

struct bta_dm_search_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    struct bta_dm_inq_t inq_params; /* 0x08-0x11 */
    unsigned char _pad[2];          /* 0x12-0x13 */
    unsigned int services;          /* 0x14 */
    void *p_cback;                  /* 0x18 */
};

/* tBTA_DM_API_SIG_STRENGTH */
struct bta_dm_sig_strength_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned char mask;             /* 0x08 */
    unsigned char _pad9;            /* 0x09 */
    unsigned short period;          /* 0x0a */
    unsigned char start;            /* 0x0c */
    unsigned char _padd;            /* 0x0d */
};

/* --- Search SM message payloads --- */

/* BTA_DM_INQUIRY_CMPL_EVT */
struct bta_dm_inq_cmpl_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned char num;              /* 0x08 */
};

/* BTA_DM_SDP_RESULT_EVT */
struct bta_dm_sdp_result_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned short sdp_status;      /* 0x08 */
};

/* BTA_DM_DISC_RESULT / BTA_DM_REMT_NAME / search-complete payload
   (tBTA_DM_SEARCH result; 0x110-byte allocation) */
struct bta_dm_disc_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned char bd_addr[6];       /* 0x08-0x0d */
    unsigned char bd_name[0xfa];    /* 0x0e-0x107 */
    unsigned int services;          /* 0x108 */
    unsigned char result;           /* 0x10c */
};

/* --- DM callback-built message payloads --- */

/* BTA_DM_ACL_CHANGE_EVT: (is_new, bd_addr) per bta_dm_acl_change_cback() */
struct bta_dm_acl_change_msg_t {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned char _pad[2];          /* 0x08-0x09 */
    unsigned char is_new;           /* 0x0a */
    unsigned char bd_addr[6];       /* 0x0b-0x10 */
};

/* --- Power-management message payloads --- */

/* PM timer message: BT_HDR + peer BD address */
struct bta_dm_pm_msg {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned char bd_addr[6];       /* 0x08-0x0d */
};

/* PM BTM-status message: BT_HDR + bd_addr + BTM PM callback payload */
struct bta_dm_pm_btm_status {
    struct bta_dm_hdr_t hdr;        /* 0x00 */
    unsigned char bd_addr[6];       /* 0x08-0x0d */
    unsigned char status;           /* 0x0e */
    unsigned short value;           /* 0x10 */
    unsigned char mode;             /* 0x12 */
};

/* --- tBTA_DM_MSG ---
   Union of every DM/Search message payload; each member embeds the 8-byte
   BT_HDR so the payload starts at 0x08. The largest member (0x110) matches
   the GKI allocation used for all DM API messages. */

struct bta_dm_msg {
    union {
        struct bta_dm_hdr_t hdr;                        /* 0x00 BT_HDR */
        struct bta_dm_enable_msg_t enable;              /* tBTA_DM_API_ENABLE */
        struct bta_dm_set_name_msg_t set_name;          /* tBTA_DM_API_SET_DEV_NAME */
        struct bta_dm_set_visibility_msg_t set_visibility; /* tBTA_DM_API_SET_VISIBILITY */
        struct bta_dm_bond_msg_t bond;                  /* tBTA_DM_API_BOND */
        struct bta_dm_pin_reply_msg_t pin_reply;        /* tBTA_DM_API_PIN_REPLY */
        struct bta_dm_auth_reply_msg_t auth_reply;      /* tBTA_DM_API_AUTH_REPLY */
        struct bta_dm_search_msg_t search;              /* tBTA_DM_API_SEARCH */
        struct bta_dm_sig_strength_msg_t sig_strength;  /* tBTA_DM_API_SIG_STRENGTH */
        struct bta_dm_inq_cmpl_msg_t inq_cmpl;          /* BTA_DM_INQUIRY_CMPL_EVT */
        struct bta_dm_sdp_result_msg_t sdp_result;      /* BTA_DM_SDP_RESULT_EVT */
        struct bta_dm_disc_msg_t disc;                  /* BTA_DM_DISC_RESULT */
        struct bta_dm_acl_change_msg_t acl_change;      /* BTA_DM_ACL_CHANGE_EVT */
        struct bta_dm_pm_msg pm;                        /* PM timer */
        struct bta_dm_pm_btm_status pm_btm_status;      /* PM BTM status */
    };
};

/* Retail-name alias (Broadcom bta_dm_int.h) */
typedef struct bta_dm_msg tBTA_DM_MSG;

#ifdef __cplusplus
}
#endif

#endif /* RVL_SDK_BTA_DM_INT_H */
