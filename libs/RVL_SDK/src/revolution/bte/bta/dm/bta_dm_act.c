// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act
// Replace stubs with high-level C/C++ during decomp.
//
// NOTE: an earlier session truncated this TU. Every body below was rebuilt
// from the retail binary (hexdiff --asm/--relocs) with Broadcom's
// bta_dm_act.c as the shape reference; no "LOST" regions remain.
// Retail reloc names win over the SDK headers (btm_read_trusted_mask is
// lowercase in the retail build).  Residues are annotated per function.

#include <harness_catalog.h>

#include <revolution/bte/bta/dm/bta_dm_int.h>

/* --- Type definitions and extern declarations --- */

/* Forward declaration for the timer list entry type (defined below) */
struct bta_dm_timer_t;

/* Timer callback type: takes a timer list entry pointer */
typedef void (*bta_dm_timer_cback_t)(struct bta_dm_timer_t *);

/* Remote name notify callback type: (bd_addr, dev_class, bd_name) */
typedef void (*bta_dm_rmt_name_cback_t)(unsigned char *, unsigned char *, unsigned char *);

/* Search control block function pointer type */
/* callback: (tBTA_DM_SEARCH_EVT event, tBTA_DM_SEARCH *p_data) */
typedef void (*bta_dm_search_cback_t)(int, void *);

/* BD address (BD_ADDR from bt_types.h) */
typedef unsigned char bd_addr_t[6];

/* tBTM_INQ_INFO layout (stack/include/btm_api.h); inquiry DB entry */
struct btm_inq_results_t {
    unsigned short clock_offset;        /* 0x00 */
    bd_addr_t remote_bd_addr;           /* 0x02 */
    unsigned char dev_class[3];         /* 0x08 */
    unsigned char page_scan_rep_mode;   /* 0x0b */
    unsigned char page_scan_per_mode;   /* 0x0c */
    unsigned char page_scan_mode;       /* 0x0d */
    signed char rssi;                   /* 0x0e */
};
struct btm_inq_info_t {
    struct btm_inq_results_t results;   /* 0x00 */
    unsigned char appl_knows_rem_name;  /* 0x10 */
};

/* Minimal timer list entry matching the GKI TIMER_LIST_ENT layout (0x18 bytes) */
struct bta_dm_timer_t {
    struct bta_dm_timer_t *p_prev;
    struct bta_dm_timer_t *p_next;
    bta_dm_timer_cback_t p_cback;
    int ticks;
    int start_time;
    unsigned short type;
    unsigned char in_use;
    unsigned char _pad;
};

struct bta_dm_search_cb_t {
    bta_dm_search_cback_t p_search_cback;   /* offset 0x00 */
    struct btm_inq_info_t *p_cur;           /* offset 0x04 */
    unsigned int services;                  /* offset 0x08 (tBTA_SERVICE_MASK) */
    unsigned int services_cached;           /* offset 0x0c */
    unsigned int services_found;            /* offset 0x10 */
    void *p_sdp_db;                         /* offset 0x14 */
    unsigned char _pad18[2];                /* offset 0x18-0x19 */
    bd_addr_t disc_bd_addr;                 /* offset 0x1a */
    char peer_name[0x20];                   /* offset 0x20 */
    struct bta_dm_timer_t search_timer;     /* offset 0x40 */
    struct bta_dm_timer_t svc_timer;        /* offset 0x58 */
    unsigned char services_index;           /* offset 0x70 */
    unsigned char _pad71[3];                /* offset 0x71-0x73 */
    void *p_search_queue;                   /* offset 0x74 */
    unsigned char search_disc_active;       /* offset 0x78 */
    unsigned char search_timer_active;      /* offset 0x79 */
    unsigned char _pad7a[2];                /* offset 0x7a-0x7b */
};

/* tBTA_DM_API_SEARCH / INQUIRY_CMPL / SDP_RESULT / DISC_RESULT message types
   are provided by <revolution/bte/bta/dm/bta_dm_int.h>. */

/* BTM inquiry complete structure as passed to tBTM_CMPL_CB (retail layout:
   status byte at +0, num_resp byte at +1). */
struct bta_btm_inq_cmpl_t {
    unsigned char status;               /* offset 0x00 */
    unsigned char num_resp;             /* offset 0x01 */
    /* ... remainder of this struct was in the LOST region ... */
};

/* tBTA_DM_DISC_RES as compiled in the retail Wii-era stack: BD_NAME_LEN is
   0xfa here, so services lands at +0x100 (retail zeroes bd_name[0] and the
   whole services word). */
struct bta_dm_disc_res_t {
    bd_addr_t bd_addr;              /* 0x000 */
    unsigned char bd_name[0xf8];    /* 0x006 */
    unsigned int services;          /* 0x100 (4-aligned after the name) */
    unsigned char result;           /* 0x104 */
};

/* tBTA_DM_INQ_CMPL */
struct bta_dm_inq_cmpl_res_t {
    unsigned char num_resps;
};

/* tBTA_DM_INQ_RES (Wii-era layout: bd_addr@0, dev_class@6, rssi@9,
   device_type@0x0a) */
struct bta_dm_inq_res_t {
    bd_addr_t bd_addr;              /* 0x00 */
    unsigned char dev_class[3];     /* 0x06 */
    signed char rssi;               /* 0x09 */
    unsigned char device_type;      /* 0x0a */
};

/* tBTA_DM_SEARCH: union of the search callback payloads (retail size 0x110) */
union bta_dm_search_t {
    struct bta_dm_inq_res_t inq_res;
    struct bta_dm_inq_cmpl_res_t inq_cmpl;
    struct bta_dm_disc_res_t disc_res;
    unsigned char _pad[0x110];
};

/* tBTM_REMOTE_DEV_NAME */
struct btm_remote_dev_name_t {
    unsigned short status;          /* 0x00 */
    unsigned short length;          /* 0x02 */
    unsigned char remote_bd_name[0xfa]; /* 0x04 */
};

/* tSDP_UUID (tBT_UUID): len word followed by a 4-byte-aligned uuid union */
struct bta_dm_uuid_t {
    unsigned short len;             /* 0x0 */
    union {
        unsigned short uuid16;
        unsigned int uuid32;
        unsigned char uuid128[16];
    } uu;                           /* 0x4 */
};

/* tBTA_DM_API_DISCOVER (tBTA_DM_API_DISCOVER is declared in bta_dm_int.h) */

/* BT_HDR + trailing data view used when only event / raw data[] are touched. */
struct bta_dm_buf_t {
    unsigned short event;               /* 0x00 */
    unsigned short len;                 /* 0x02 */
    unsigned short offset;              /* 0x04 */
    unsigned short layer_specific;      /* 0x06 */
    unsigned char data[0x108];          /* 0x08.. */
};

#ifndef BTA_DM_DISC_RES_EVT
#define BTA_DM_DISC_RES_EVT            2
#endif
/* App search callback events (tBTA_DM_SEARCH_EVT) */
#define BTA_DM_INQ_RES_EVT             0
#define BTA_DM_INQ_CMPL_EVT            1
#define BTA_DM_DISC_CMPL_EVT           3
#define BTA_DM_SEARCH_CANCEL_CMPL_EVT  4
#ifndef BTA_DM_REMT_NAME_EVT
#define BTA_DM_REMT_NAME_EVT           0x204
#endif
#ifndef BTA_DM_SEARCH_DISC_RES_EVT
#define BTA_DM_SEARCH_DISC_RES_EVT     0x207
#endif
#ifndef BTA_DM_SEARCH_CMPL_EVT
#define BTA_DM_SEARCH_CMPL_EVT         0x206
#endif

/* Globals / helpers that lived in the LOST region - declare only what the
   currently present functions need so the TU can compile. Bodies for the
   LOST functions themselves are NOT invented here. */
extern struct bta_dm_search_cb_t bta_dm_search_cb;
/* tBTM_CB is 0x27C4 bytes in retail (.bss, not small data).  Only the page
   timeout is used here; retail addresses it at +0x64c. */
struct btm_cb_t {
    unsigned char unk_0x0[0x64c];
    unsigned short page_timeout;        /* 0x64c */
    unsigned char unk_0x64e[0x27C4 - 0x64e];
};
extern struct btm_cb_t btm_cb;
extern const unsigned short bta_service_id_to_uuid_lkup_tbl[];
extern void bta_dm_remname_cback(struct btm_remote_dev_name_t *);
extern void bta_dm_service_search_remname_cback(unsigned char *, unsigned char *, unsigned char *);
extern void bta_dm_search_timer_cback(struct bta_dm_timer_t *);
extern void bta_dm_find_services(unsigned char *bd_addr);
extern void bta_dm_discover_next_device(void);

/* DM control block (instance in bta_dm_main.c; layout matches bta_dm_api.c). */
/* tBTA_DM_PEER_DEVICE: retail strides the array by 11 bytes */
struct bta_dm_peer_dev_t {
    unsigned char bd_addr[6];       /* 0x0 */
    unsigned char in_use;           /* 0x6 */
    unsigned char policy;           /* 0x7 */
    unsigned char _pad[3];          /* 0x8-0xa */
};

struct bta_dm_cb_t {
    struct bta_dm_peer_dev_t peer_dev[7];           /* 0x00-0x4c */
    unsigned char num_devices;                      /* 0x4d */
    unsigned char _pad4e[2];                        /* 0x4e-0x4f */
    void (*cback)(int, void *);                     /* 0x50 */
    struct bta_dm_timer_t signal_strength_timer;    /* 0x54 */
    unsigned char signal_strength_mask;             /* 0x6c */
    unsigned char _pad6d[3];                        /* 0x6d-0x6f */
    unsigned short signal_strength_period;          /* 0x70 */
    unsigned char disable_timer_active;             /* 0x72 */
    unsigned char _pad73;                           /* 0x73 */
    struct bta_dm_timer_t disable_timer;            /* 0x74 */
    unsigned char _pad8c[0x6c];                     /* 0x8c-0xf7 */
    unsigned char pin_bd_addr[6];                   /* 0xf8-0xfd */
    unsigned char pin_dev_class[3];                 /* 0xfe-0x100 */
    unsigned char keep_acl;                         /* 0x101 */
    unsigned char _pad102[2];                       /* 0x102-0x103 */
};

extern struct bta_dm_cb_t bta_dm_cb;

/* Minimal event payload / helper types used by recovered bodies below. */
/* BD_NAME_LEN is 0xf8 in the retail Wii-era stack (see tBTA_DM_PIN_REQ: the
   device class follows the name at 0xfe). */
#define BTA_DM_BD_NAME_LEN 0xf8

/* tBTA_DM_AUTH_CMPL */
struct bta_dm_auth_cmpl_t {
    unsigned char bd_addr[6];               /* 0x000 */
    unsigned char bd_name[BTA_DM_BD_NAME_LEN]; /* 0x006 */
    unsigned char key_present;              /* 0x0fe */
    unsigned char key[0x10];                /* 0x0ff */
    unsigned char key_type;                 /* 0x10f */
};

/* tBTA_DM_AUTHORIZE */
struct bta_dm_authorize_t {
    unsigned char bd_addr[6];               /* 0x000 */
    unsigned char bd_name[BTA_DM_BD_NAME_LEN]; /* 0x006 */
    unsigned char service;                  /* 0x0fe */
};

/* tBTA_DM_PIN_REQ */
struct bta_dm_pin_req_t {
    unsigned char bd_addr[6];               /* 0x000 */
    unsigned char bd_name[0x20];            /* 0x006 (0xf8 in the union) */
    unsigned char _pad26[0xd8];             /* 0x026-0x0fd */
    unsigned char dev_class[3];             /* 0x0fe */
    unsigned char min_16_digit;             /* 0x101 */
};

struct bta_dm_sig_strength_t {
    unsigned char bd_addr[6];
    unsigned char mask;
    signed char rssi_value;
    unsigned char link_quality_value;
};

/* tBTM_RSSI_RESULTS / tBTM_LINK_QUALITY_RESULTS (btm_api.h: status,
   hci_status, value, rem_bda) */
struct btm_rssi_results_t {
    unsigned char status;           /* 0x00 */
    unsigned char hci_status;       /* 0x01 */
    signed char rssi;               /* 0x02 */
    unsigned char rem_bda[6];       /* 0x03 */
};

struct btm_link_quality_results_t {
    unsigned char status;           /* 0x00 */
    unsigned char hci_status;       /* 0x01 */
    unsigned char link_quality;     /* 0x02 */
    unsigned char rem_bda[6];       /* 0x03 */
};

struct bta_dm_compress_srvc_t {
    unsigned char in_use;
    unsigned char bd_addr[6];
    unsigned char server_id;
    // +0x08: unnamed in recovered sources; retail strides elements by 9
    // (BSS holds 0x2d = 5*9, and the init memset covers 0x2d).
    unsigned char unk8;
};

/* tBTA_DM_SEC: union of the security callback payloads (retail size 0x110) */
union bta_dm_sec_t {
    struct {
        unsigned char bd_addr[6];
        unsigned char reason;
    } acl_change;
    struct bta_dm_pin_req_t pin_req;
    struct bta_dm_auth_cmpl_t auth_cmpl;
    unsigned char _pad[0x110];
};

/* tBTA_DM_CFG (retail field order: policy settings, page timeout, link
   supervision timeout) */
struct bta_dm_cfg_t {
    unsigned char dev_class[3];     /* 0x00 */
    unsigned short policy_settings; /* 0x04 */
    unsigned short page_timeout;    /* 0x06 */
    unsigned short link_timeout;    /* 0x08 */
};

extern struct bta_dm_cfg_t bta_dm_cfg;
extern unsigned char bta_dm_conn_srvcs[];
extern struct bta_dm_compress_srvc_t bta_dm_compress_srvcs[5];
extern unsigned char bta_security[];
extern const unsigned int bta_service_id_to_btm_srv_id_lkup_tbl[];
extern unsigned char *p_bta_dm_rm_cfg;
extern unsigned char appl_trace_level;
extern unsigned char bta_sys_cb[];
extern int bdcmp(unsigned char *, unsigned char *);
extern void LogMsg_0(unsigned short, char *);
extern void LogMsg_1(unsigned short, char *, int);
extern void *GKI_getpoolbuf(int);
extern void btsnd_hcic_write_scan_enable(void *, int);
extern void BTM_SendHciReset(void *);
extern unsigned short BTM_GetNumAclLinks(void);
extern void btm_remove_acl(unsigned char *);
extern int btm_get_acl_disc_reason_code(void);
extern unsigned char bta_dm_co_get_compress_memory(unsigned char, unsigned char **, unsigned int *);
extern void BTM_ReadRSSI(unsigned char *, void *);
extern void BTM_ReadLinkQuality(unsigned char *, void *);
extern void BTM_SetDeviceClass(unsigned char *);
extern void BTM_SecRegister(void *);
extern void BTM_SetDefaultLinkSuperTout(unsigned short);
extern void BTM_WritePageTimeout(unsigned short);
extern void BTM_SetDefaultLinkPolicy(unsigned short);
extern void BTM_AclRegisterForChanges(void *);
extern void BTM_ReadLocalDeviceAddr(void *);
extern void bta_sys_rm_register(void *);
extern void bta_sys_compress_register(void *);
extern void bta_dm_init_pm(void);
extern void bta_dm_disable_pm(void);
extern void L2CA_RegisterCompression(void *, int);
extern void L2CA_SetIdleTimeoutByBdAddr(const unsigned char *, unsigned short);
extern void bta_sys_disable(void);
extern void BTM_SetDiscoverability(int, int, int);
extern void BTM_SetConnectability(int, int, int);
extern void WBT_ExtCreateRecord(void);
extern const unsigned char BT_BD_ANY[6];
extern void *memset(void *, int, unsigned long);
extern void *memcpy(void *, const void *, unsigned long);
extern void BTM_SetLocalDeviceName(char *);
extern unsigned int *btm_read_trusted_mask(unsigned char *);
extern void BTM_SecBond(unsigned char *, unsigned char, unsigned char *, unsigned int *);
extern void BTM_PINCodeReply(unsigned char *, unsigned char, unsigned char,
                             unsigned char *, unsigned int *);
extern void BTM_DeviceAuthorized(unsigned char *, unsigned char, unsigned int *);
extern void L2CA_SetIdleTimeout(int, unsigned short, int);
extern void BTM_ClearInqDb(unsigned char *);
extern unsigned short BTM_IsInquiryActive(void);
extern void BTM_CancelInquiry(void);
extern unsigned char BTM_CancelRemoteDeviceName(void);
extern void BTM_StartInquiry(void *, void *, void *);
extern void *BTM_InqDbFirst(void);
extern void *BTM_InqDbNext(void *);
extern void *BTM_InqDbRead(unsigned char *);
extern unsigned char BTM_IsAclConnectionUp(unsigned char *);
extern unsigned char BTM_SecAddRmtNameNotifyCallback(void *);
extern int BTM_SecDeleteRmtNameNotifyCallback(void *);
extern void SDP_InitDiscoveryDb(void *, unsigned int, unsigned short, void *,
                                unsigned short, unsigned short *);
extern unsigned char SDP_ServiceSearchAttributeRequest(unsigned char *, void *,
                                                      void *);

/* Retail (Wii-era) BTM_SEC_MAX_SERVICES is 33..64, so the trusted-mask array
   is 2 words (the memcpy/memset in the bond/pin/auth paths move 8 bytes). */
#define BTM_SEC_SERVICE_ARRAY_SIZE 2
#define BTA_MAX_SERVICE_ID 23

#ifndef BTA_DM_ACL_CHANGE_EVT
#define BTA_DM_ACL_CHANGE_EVT 0x105
#endif
#ifndef BTA_DM_ACL_CHANGED_EVT
#define BTA_DM_ACL_CHANGED_EVT 5
#endif
#ifndef BTA_DM_ACL_DOWN_EVT
#define BTA_DM_ACL_DOWN_EVT 6
#endif
#ifndef BTA_DM_SEARCH_INQ_CMPL_EVT
#define BTA_DM_SEARCH_INQ_CMPL_EVT 0x203
#endif
#define BTA_DM_SEARCH_RESULT_EVT 0x208
#ifndef BTA_DM_SEARCH_SDP_RES_EVT
#define BTA_DM_SEARCH_SDP_RES_EVT 0x206
#endif

void bta_dm_pinname_cback(struct btm_remote_dev_name_t *p_remote_name);
void bta_dm_signal_strength_timer_cback(struct bta_dm_timer_t *p_tle);
void bta_dm_compress_cback(void);
void bta_dm_rssi_cback(struct btm_rssi_results_t *p_rssi);
void bta_dm_link_quality_cback(void *p_result);


/* Harness stubs: many BTE APIs are undeclared and default to int-returning
   K&R prototypes under MWCC; cast call results at use sites below. */
void *BTM_InqDbNext(void *);
void *GKI_getbuf(unsigned short);
void GKI_freebuf(void *);
void *SDP_FindServiceInDb(void *, unsigned short, void *);
void *SDP_FindAttributeInRec(void *, unsigned short);
void *BTM_SecReadDevName(unsigned char *);
int BTM_ReadRemoteDeviceName(unsigned char *, void *);
void bdcpy(unsigned char *, unsigned char *);
void bta_sys_start_timer(struct bta_dm_timer_t *, unsigned short, int);
void bta_sys_stop_timer(struct bta_dm_timer_t *);
void bta_sys_sendmsg(void *);
int strncpy(char *, const char *, unsigned long);

void bta_dm_enable(struct bta_dm_msg *p_data);
void bta_dm_disable(struct bta_dm_msg *p_data);
void bta_dm_disable_timer_cback(struct bta_dm_timer_t *p_tle);
void bta_dm_set_dev_name(struct bta_dm_msg *p_data);
void bta_dm_set_visibility(struct bta_dm_msg *p_data);
void bta_dm_bond(struct bta_dm_msg *p_data);
void bta_dm_pin_reply(struct bta_dm_msg *p_data);
void bta_dm_auth_reply(struct bta_dm_msg *p_data);
void bta_dm_search_start(struct bta_dm_msg *p_data);
void bta_dm_search_cancel(struct bta_dm_msg *p_data);
void bta_dm_discover(struct bta_dm_msg *p_data);
void bta_dm_inq_cmpl(struct bta_dm_msg *p_data);
void bta_dm_rmt_name(struct bta_dm_msg *p_data);
void bta_dm_disc_rmt_name(struct bta_dm_msg *p_data);
void bta_dm_sdp_result(struct bta_dm_msg *p_data);
void bta_dm_search_cmpl(struct bta_dm_msg *p_data);
void bta_dm_disc_result(struct bta_dm_msg *p_data);
void bta_dm_search_result(struct bta_dm_msg *p_data);
void bta_dm_search_timer_cback(struct bta_dm_timer_t *t);
void bta_dm_free_sdp_db();
void bta_dm_queue_search(struct bta_dm_msg *p_data);
void bta_dm_queue_disc(struct bta_dm_msg *p_data);
void bta_dm_search_clear_queue(struct bta_dm_msg *p_data);
void bta_dm_search_cancel_cmpl(struct bta_dm_msg *p_data);
void bta_dm_search_cancel_transac_cmpl(struct bta_dm_msg *p_data);
void bta_dm_search_cancel_notify(struct bta_dm_msg *p_data);
void bta_dm_find_services(unsigned char *bd_addr);
void bta_dm_discover_next_device(void);
void bta_dm_sdp_callback(unsigned short status);
void bta_dm_inq_results_cb(struct btm_inq_results_t *p_results,
                           unsigned char *p_eir);
void bta_dm_inq_cmpl_cb(struct bta_btm_inq_cmpl_t *p_results);
void bta_dm_service_search_remname_cback(unsigned char *a, unsigned char *b, unsigned char *c);
void bta_dm_remname_cback(struct btm_remote_dev_name_t *p_remote_name);
void bta_dm_disc_remname_cback(struct btm_remote_dev_name_t *p_remote_name);
void bta_dm_cancel_rmt_name(struct bta_dm_msg *p_data);
int bta_dm_authorize_cback(bd_addr_t bd_addr, unsigned char *dev_class,
                           unsigned char *bd_name, unsigned char *p_service_name,
                           unsigned char service_id, unsigned char is_authorized);
void bta_dm_pinname_cback(struct btm_remote_dev_name_t *p_remote_name);
unsigned char bta_dm_pin_cback(bd_addr_t bd_addr, unsigned char *dev_class,
                               unsigned char *bd_name);
int bta_dm_link_key_request_cback(void);
unsigned char bta_dm_new_link_key_cback(bd_addr_t bd_addr, unsigned char *dev_class,
                                        unsigned char *bd_name, unsigned char *key,
                                        unsigned char key_type);
unsigned char bta_dm_authentication_complete_cback(bd_addr_t bd_addr, unsigned char *dev_class,
                                                   unsigned char *bd_name, unsigned char success);
void bta_dm_local_addr_cback(void *addr);
void bta_dm_signal_strength(struct bta_dm_msg *p_data);
void bta_dm_signal_strength_timer_cback(struct bta_dm_timer_t *p_tle);
void bta_dm_acl_change_cback(bd_addr_t bd_addr, unsigned char *p_dc, unsigned char *p_bdn,
                             unsigned char *p_features, unsigned char is_new);
void bta_dm_acl_change(struct bta_dm_msg *p_data);
void bta_dm_disable_conn_down_timer_cback(struct bta_dm_timer_t *p_tle);
void bta_dm_rssi_cback(struct btm_rssi_results_t *p_rssi);
void bta_dm_link_quality_cback(void *p_result);
unsigned char bta_dm_l2cap_server_compress_cback(
        bd_addr_t bd_addr, unsigned char server_id, unsigned char client_id,
        unsigned char *p_data, unsigned int data_len, unsigned char *p_data2,
        unsigned int data_len2, unsigned char **pp_memory,
        unsigned int *p_memory_size);
void bta_dm_compress_cback(void);
void bta_dm_rm_cback(int status, unsigned char id, unsigned char app_id,
                     bd_addr_t peer_addr);
void bta_dm_keep_acl(struct bta_dm_msg *p_data);
void bta_dm_immediate_disable(struct bta_dm_msg *p_data);
void bta_dm_reset_complete(void);
void bta_dm_send_hci_reset(struct bta_dm_msg *p_data);

// from commit ca16562e8851 needle=bta_dm_enable

/* --- Function implementations --- */

// Retail .data/.bss/.sdata2 have trailing pad bytes; declared first so the
// reverse-order emitter places them last in their sections.

void bta_dm_enable(struct bta_dm_msg *p_data) {
    unsigned char dev_class[3];

    memset(&bta_dm_search_cb, 0, 0x7c);
    memset(bta_dm_conn_srvcs, 0, 0x2e);
    memcpy(dev_class, bta_dm_cfg.dev_class, 3);
    BTM_SetDeviceClass(dev_class);
    bta_dm_cb.cback = p_data->enable.p_sec_cback;
    BTM_SecRegister((void *)&bta_security);
    BTM_SetDefaultLinkSuperTout(bta_dm_cfg.link_timeout);
    BTM_WritePageTimeout(bta_dm_cfg.page_timeout);
    BTM_SetDefaultLinkPolicy(bta_dm_cfg.policy_settings);
    BTM_AclRegisterForChanges(bta_dm_acl_change_cback);
    BTM_ReadLocalDeviceAddr(bta_dm_local_addr_cback);
    bta_sys_rm_register(bta_dm_rm_cback);
    bta_sys_compress_register(bta_dm_compress_cback);
    bta_dm_init_pm();
    memset(bta_dm_compress_srvcs, 0, 0x2d);
    bta_sys_compress_register(bta_dm_compress_cback);
    L2CA_RegisterCompression(bta_dm_l2cap_server_compress_cback, 0);
    WBT_ExtCreateRecord();
}

// --- restored from git history (base:gone repo-search) ---
// from commit 4116edb5dec8 path=libs/RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act.c needle=bta_dm_disable

void bta_dm_disable(struct bta_dm_msg *p_data) {
    L2CA_SetIdleTimeoutByBdAddr(BT_BD_ANY, 0);
    bta_sys_disable();
    BTM_SetDiscoverability(0, 0, 0);
    BTM_SetConnectability(0, 0, 0);
    bta_dm_disable_pm();
    if (BTM_GetNumAclLinks() == 0) {
        bta_dm_cb.disable_timer.p_cback = bta_dm_disable_conn_down_timer_cback;
        bta_sys_start_timer(&bta_dm_cb.disable_timer, 0, 1000);
    } else {
        bta_dm_cb.disable_timer_active = 1;
        bta_dm_cb.disable_timer.p_cback = bta_dm_disable_timer_cback;
        bta_sys_start_timer(&bta_dm_cb.disable_timer, 0, 5000);
    }
}

// from commit 95ad18ac12e1 needle=bta_dm_disable_timer_cback
/* Disable timeout expired: if ACL links are still up (and we are not
   keeping them), drop every peer device, otherwise report the disable. */
void bta_dm_disable_timer_cback(struct bta_dm_timer_t *p_tle) {
    unsigned char i;

    if (appl_trace_level >= 4)
        LogMsg_0(0x503, " bta_dm_disable_timer_cback  ");
    if (BTM_GetNumAclLinks() != 0 && bta_dm_cb.keep_acl == 0) {
        for (i = 0; i < bta_dm_cb.num_devices; i++) {
            btm_remove_acl(bta_dm_cb.peer_dev[i].bd_addr);
        }
    } else {
        bta_dm_cb.disable_timer_active = 0;
        bta_dm_cb.cback(1, NULL);
    }
}

void bta_dm_set_dev_name(struct bta_dm_msg *p_data) {
    BTM_SetLocalDeviceName(p_data->set_name.name);
}

void bta_dm_set_visibility(struct bta_dm_msg *p_data) {
    BTM_SetDiscoverability(p_data->set_visibility.disc_mode, 0, 0);
    BTM_SetConnectability(p_data->set_visibility.conn_mode, 0, 0);
}

void bta_dm_bond(struct bta_dm_msg *p_data) {
    unsigned int trusted_mask[BTM_SEC_SERVICE_ARRAY_SIZE];
    unsigned int *current_trusted_mask;

    current_trusted_mask = btm_read_trusted_mask(p_data->bond.bd_addr);

    if (current_trusted_mask)
        memcpy(trusted_mask, current_trusted_mask, sizeof(trusted_mask));
    else
        memset(trusted_mask, 0, sizeof(trusted_mask));

    BTM_SecBond(p_data->bond.bd_addr, p_data->bond.pin_len, p_data->bond.p_pin,
                trusted_mask);
}

void bta_dm_pin_reply(struct bta_dm_msg *p_data) {
    unsigned int trusted_mask[BTM_SEC_SERVICE_ARRAY_SIZE];
    unsigned int *current_trusted_mask;

    current_trusted_mask = btm_read_trusted_mask(p_data->pin_reply.bd_addr);

    if (current_trusted_mask)
        memcpy(trusted_mask, current_trusted_mask, sizeof(trusted_mask));
    else
        memset(trusted_mask, 0, sizeof(trusted_mask));

    if (p_data->pin_reply.accept) {
        BTM_PINCodeReply(p_data->pin_reply.bd_addr, 0, p_data->pin_reply.pin_len,
                         p_data->pin_reply.p_pin, trusted_mask);
    } else {
        BTM_PINCodeReply(p_data->pin_reply.bd_addr, 0xb, 0, NULL, trusted_mask);
    }
}

void bta_dm_auth_reply(struct bta_dm_msg *p_data) {
    unsigned int trusted_mask[BTM_SEC_SERVICE_ARRAY_SIZE];
    unsigned char btm_mask_index = 0;
    unsigned int *current_trusted_mask;

    current_trusted_mask = btm_read_trusted_mask(p_data->auth_reply.bd_addr);

    if (current_trusted_mask)
        memcpy(trusted_mask, current_trusted_mask, sizeof(trusted_mask));
    else
        memset(trusted_mask, 0, sizeof(trusted_mask));

    if (p_data->auth_reply.response != 2 /* BTA_DM_NOT_AUTH */) {
        if (p_data->auth_reply.response == 0 /* BTA_DM_AUTH_PERM */) {
            if (p_data->auth_reply.service <= BTA_MAX_SERVICE_ID) {
                /* convert BTA service id to BTM mask */
                btm_mask_index =
                    (unsigned char)(bta_service_id_to_btm_srv_id_lkup_tbl
                                        [p_data->auth_reply.service] /
                                    32);
                trusted_mask[btm_mask_index] |= (unsigned int)(
                    1 << (bta_service_id_to_btm_srv_id_lkup_tbl
                              [p_data->auth_reply.service] -
                          (unsigned int)(btm_mask_index * 32)));
            }
        }
        BTM_DeviceAuthorized(p_data->auth_reply.bd_addr, 0, trusted_mask);
    } else {
        BTM_DeviceAuthorized(p_data->auth_reply.bd_addr, 0xb, trusted_mask);
    }
}

void bta_dm_search_start(struct bta_dm_msg *p_data) {
    BTM_ClearInqDb(NULL);
    /* save search params */
    bta_dm_search_cb.p_search_cback = p_data->search.p_cback;
    bta_dm_search_cb.services = p_data->search.services;

    BTM_StartInquiry(&p_data->search.inq_params, bta_dm_inq_results_cb,
                     bta_dm_inq_cmpl_cb);
}

void bta_dm_search_cancel(struct bta_dm_msg *p_data) {
    struct bta_dm_buf_t *p_msg;

    BTM_ClearInqDb(NULL);

    if (BTM_IsInquiryActive() != 0) {
        BTM_CancelInquiry();
        bta_dm_search_cancel_notify(p_data);

        p_msg = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
        if (p_msg != NULL) {
            p_msg->event = BTA_DM_SEARCH_DISC_RES_EVT;
            bta_sys_sendmsg(p_msg);
        }
    } else if (bta_dm_search_cb.services == 0) {
        /* no service search going on: cancel the pending remote name request */
        BTM_CancelRemoteDeviceName();
    }
}

void bta_dm_discover(struct bta_dm_msg *p_data) {
    struct bta_dm_buf_t *p_buf;

    /* save the search condition */
    bta_dm_search_cb.p_search_cback = p_data->discover.p_cback;
    bta_dm_search_cb.services = p_data->discover.services;
    bta_dm_search_cb.services_cached = bta_dm_search_cb.services;
    bta_dm_search_cb.services_index = 0;
    bta_dm_search_cb.services_found = 0;
    bta_dm_search_cb.peer_name[0] = 0;
    BTM_SecAddRmtNameNotifyCallback(bta_dm_service_search_remname_cback);
    bdcpy(bta_dm_search_cb.disc_bd_addr, p_data->discover.bd_addr);

    if (bta_dm_search_cb.services != 0) {
        bta_dm_find_services(bta_dm_search_cb.disc_bd_addr);
    } else {
        if (btm_cb.page_timeout < 0x7530) {
            bta_sys_start_timer(&bta_dm_search_cb.svc_timer, 0x205,
                                btm_cb.page_timeout + 0x64);
        }

        if ((unsigned char)BTM_ReadRemoteDeviceName(
                bta_dm_search_cb.disc_bd_addr,
                (void *)bta_dm_disc_remname_cback) == 1) {
            return;
        }

        bta_sys_stop_timer(&bta_dm_search_cb.svc_timer);
        p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
        if (p_buf != NULL) {
            bdcpy(p_buf->data, bta_dm_search_cb.disc_bd_addr);
            p_buf->data[6] = 0;
            p_buf->event = BTA_DM_REMT_NAME_EVT;
            bta_sys_sendmsg(p_buf);
        }
    }
}

void bta_dm_inq_cmpl(struct bta_dm_msg *p_data) {
    struct bta_dm_disc_res_t disc_res;
    struct bta_dm_inq_cmpl_res_t inq_cmpl;
    struct bta_dm_buf_t *p_msg;
    unsigned char found = 1;

    if ((bta_dm_search_cb.p_cur = (struct btm_inq_info_t *)BTM_InqDbFirst()) != NULL) {
        found = 0;

        if (bta_dm_search_cb.services != 0) {
            if (BTM_IsAclConnectionUp(bta_dm_search_cb.p_cur->results.remote_bd_addr) != 0) {
                bta_dm_search_cb.search_disc_active = 0;
            } else {
                bta_dm_search_cb.search_disc_active = 1;
            }
            bta_dm_search_cb.search_timer_active = 0;
            BTM_SecAddRmtNameNotifyCallback(bta_dm_service_search_remname_cback);
            bta_dm_search_cb.services_index = 0;
            bta_dm_search_cb.services_found = 0;
            bta_dm_search_cb.services_cached = bta_dm_search_cb.services;
            bta_dm_search_cb.peer_name[0] = 0;
            bdcpy(bta_dm_search_cb.disc_bd_addr,
                  bta_dm_search_cb.p_cur->results.remote_bd_addr);
            bta_dm_find_services(
                bta_dm_search_cb.p_cur->results.remote_bd_addr);
        } else {
            found = 1;

            while (bta_dm_search_cb.p_cur != NULL) {
                if (bta_dm_search_cb.p_cur->appl_knows_rem_name != 0) {
                    bdcpy(disc_res.bd_addr,
                          bta_dm_search_cb.p_cur->results.remote_bd_addr);
                    disc_res.bd_name[0] = 0;
                    disc_res.services = 0;
                    bta_dm_search_cb.p_search_cback(BTA_DM_DISC_RES_EVT, &disc_res);
                    bta_dm_search_cb.p_cur =
                        (struct btm_inq_info_t *)BTM_InqDbNext(bta_dm_search_cb.p_cur);
                } else {
                    if (btm_cb.page_timeout < 0x7530) {
                        bta_sys_start_timer(&bta_dm_search_cb.svc_timer, 0x205,
                                            btm_cb.page_timeout + 0x64);
                    }
                    if ((unsigned char)BTM_ReadRemoteDeviceName(
                            bta_dm_search_cb.p_cur->results.remote_bd_addr,
                            (void *)bta_dm_remname_cback) != 1) {
                        unsigned char *p_rem_addr;

                        bta_sys_stop_timer(&bta_dm_search_cb.svc_timer);
                        p_rem_addr =
                            bta_dm_search_cb.p_cur->results.remote_bd_addr;
                        p_msg = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
                        if (p_msg != NULL) {
                            bdcpy(p_msg->data, p_rem_addr);
                            p_msg->data[6] = 0;
                            p_msg->event = BTA_DM_REMT_NAME_EVT;
                            bta_sys_sendmsg(p_msg);
                        }
                    }
                    found = 0;
                    break;
                }
            }
        }
    }

    if (found != 0) {
        /* no more devices to discover: report search complete */
        bta_dm_search_cb.services = 0;

        p_msg = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
        if (p_msg != NULL) {
            p_msg->event = BTA_DM_SEARCH_DISC_RES_EVT;
            bta_sys_sendmsg(p_msg);
        }
    }

    /* NOTE(residual): retail's frame reserves ~0x100 bytes more stack than
       any local we can identify, so its disc-result local sits at sp+8 while
       ours lands at sp+0x10; every field offset matches. */
    inq_cmpl.num_resps = p_data->inq_cmpl.num;
    bta_dm_search_cb.p_search_cback(BTA_DM_INQ_CMPL_EVT, &inq_cmpl);
}

/* Search-machine and API handlers below: enable/disable, the name and
   visibility setters, bond/pin/auth replies, search start/cancel, discover,
   the inquiry-complete walk and the search timer.  Message payload layouts
   come from bta_dm_int.h; the callback payloads (tBTA_DM_SEARCH /
   tBTA_DM_SEC) are laid out locally because the Wii-era BD_NAME_LEN (0xf8)
   differs from the AOSP header, and the disc result's service mask is
   4-aligned to 0x100 after the name.
   The .sdata2 attribute pair used by find_services is not reproduced yet. */

/* --- Verbatim-recovered functions (session transcript) --- */

/* Discovery remote-name transaction walk (recovered region begins mid-comment;
   preceding helper functions are in the LOST region above). If the database is
   exhausted, finalize with a DISC_RESULT message, then report the result of
   this message's device to the app. */
void bta_dm_rmt_name(struct bta_dm_msg *p_data) {
    unsigned char found = 1;
    struct bta_dm_disc_res_t disc_res;
    struct bta_dm_buf_t *p_buf;

    while ((bta_dm_search_cb.p_cur =
                (struct btm_inq_info_t *)BTM_InqDbNext(bta_dm_search_cb.p_cur)) != NULL) {
        if (bta_dm_search_cb.p_cur->appl_knows_rem_name != 0) {
            bdcpy(disc_res.bd_addr, bta_dm_search_cb.p_cur->results.remote_bd_addr);
            disc_res.bd_name[0] = 0;
            disc_res.services = 0;
            bta_dm_search_cb.p_search_cback(BTA_DM_DISC_RES_EVT, &disc_res);
        } else {
            if (btm_cb.page_timeout < 0x7530) {
                bta_sys_start_timer(&bta_dm_search_cb.svc_timer, 0x205,
                                    btm_cb.page_timeout + 0x64);
            }
            if ((unsigned char)BTM_ReadRemoteDeviceName(
                    bta_dm_search_cb.p_cur->results.remote_bd_addr,
                    (void *)bta_dm_remname_cback) != 1) {
                unsigned char *p_rem_addr;

                bta_sys_stop_timer(&bta_dm_search_cb.svc_timer);
                p_rem_addr = bta_dm_search_cb.p_cur->results.remote_bd_addr;
                p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
                if (p_buf != NULL) {
                    bdcpy(p_buf->data, p_rem_addr);
                    p_buf->data[6] = 0;
                    p_buf->event = BTA_DM_REMT_NAME_EVT;
                    bta_sys_sendmsg(p_buf);
                }
            }
            found = 0;
            break;
        }
    }
    if (found != 0) {
        p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
        if (p_buf != NULL) {
            p_buf->event = BTA_DM_SEARCH_DISC_RES_EVT;
            bta_sys_sendmsg(p_buf);
        }
    }
    bta_dm_search_cb.p_search_cback(BTA_DM_DISC_RES_EVT, p_data->disc.bd_addr);
}

/* Discovery remote-name transaction complete: report a discovery result
   with no services to the app and send the search machine a DISC_RESULT. */
void bta_dm_disc_rmt_name(struct bta_dm_msg *p_data) {
    struct bta_dm_buf_t *p_buf;

    p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
    if (p_buf != NULL) {
        p_buf->event = BTA_DM_SEARCH_DISC_RES_EVT;
        bta_sys_sendmsg(p_buf);
    }
    p_data->disc.services = 0;
    bta_dm_search_cb.p_search_cback(BTA_DM_DISC_RES_EVT, p_data->disc.bd_addr);
}

/* SDP discovery result: on success, check whether the searched service was
   found in the DB (an HDP record needs the 0x8001 attribute present) and set
   the services_found bit; then free the SDP DB and continue with the next
   service or report a discovery-complete message. On failure, stop the
   discovery and report the result to the search state machine. */
void bta_dm_sdp_result(struct bta_dm_msg *p_data) {
    unsigned char found;
    struct bta_dm_disc_msg_t *p_msg;
    void *p_rec;
    unsigned short uuid;
    unsigned short status;
    unsigned char *p_name;

    found = 0;
    status = p_data->sdp_result.sdp_status;
    p_rec = NULL;

    if (status == 0 || status == 0xfff0 || status == 0xfff4) {
        /* successful SDP transaction for the current service */
        uuid = ((unsigned short *)bta_service_id_to_uuid_lkup_tbl)[bta_dm_search_cb.services_index - 1];
        if (status != 0xfff4) {
            p_rec = (void *)SDP_FindServiceInDb(bta_dm_search_cb.p_sdp_db, uuid, NULL);
            if (p_rec == NULL) {
                goto sdp_db_cleanup;
            }
        }
        if (uuid == 0x1200) {
            if (p_rec != NULL) {
                if (SDP_FindAttributeInRec(p_rec, 0x8001) != NULL) {
                    found = 1;
                }
            }
        } else {
            found = 1;
        }
        if (found != 0) {
            bta_dm_search_cb.services_found |=
                1u << (bta_dm_search_cb.services_index - 1);
        }
    sdp_db_cleanup:
        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;
        if (bta_dm_search_cb.services_cached != 0) {
            bta_dm_find_services(bta_dm_search_cb.disc_bd_addr);
            return;
        }
        BTM_SecDeleteRmtNameNotifyCallback(bta_dm_service_search_remname_cback);
        p_msg = (struct bta_dm_disc_msg_t *)GKI_getbuf(0x110);
        if (p_msg != NULL) {
            ((struct bta_dm_buf_t *)p_msg)->event = BTA_DM_SEARCH_RESULT_EVT;
            p_msg->result = 0;
            p_msg->services = bta_dm_search_cb.services_found;
            bdcpy(p_msg->bd_addr, bta_dm_search_cb.disc_bd_addr);
            p_name = (unsigned char *)bta_dm_search_cb.peer_name;
            if (bta_dm_search_cb.peer_name[0] == 0) {
                unsigned char *p_sec_name;
                p_sec_name = (unsigned char *)BTM_SecReadDevName(bta_dm_search_cb.disc_bd_addr);
                if (p_sec_name != NULL) {
                    p_name = p_sec_name;
                }
            }
            strncpy((char *)p_msg->bd_name, (const char *)p_name, 0x20);
            bta_sys_sendmsg(p_msg);
        }
    } else {
        /* SDP failed for the current service */
        if (status == 0xfff1 || (unsigned short)(status + 0xa) <= 1) {
            bta_dm_search_cb.search_disc_active = 0;
        }
        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;
        BTM_SecDeleteRmtNameNotifyCallback(bta_dm_service_search_remname_cback);
        p_msg = (struct bta_dm_disc_msg_t *)GKI_getbuf(0x110);
        if (p_msg != NULL) {
            ((struct bta_dm_buf_t *)p_msg)->event = BTA_DM_SEARCH_RESULT_EVT;
            p_msg->result = 1;
            p_msg->services = bta_dm_search_cb.services_found;
            bdcpy(p_msg->bd_addr, bta_dm_search_cb.disc_bd_addr);
            p_name = (unsigned char *)bta_dm_search_cb.peer_name;
            if (bta_dm_search_cb.peer_name[0] == 0) {
                unsigned char *p_sec_name;
                p_sec_name = (unsigned char *)BTM_SecReadDevName(bta_dm_search_cb.disc_bd_addr);
                if (p_sec_name != NULL) {
                    p_name = p_sec_name;
                }
            }
            strncpy((char *)p_msg->bd_name, (const char *)p_name, 0x20);
            bta_sys_sendmsg(p_msg);
        }
    }
}

/* Target 1: dispatches BTA_DM_DISC_CMPL_EVT (3) with NULL data to the search callback */
void bta_dm_search_cmpl(struct bta_dm_msg *p_data) {
    bta_dm_search_cb.p_search_cback(3, NULL);
}

/* Discovery result: reports BTA_DM_DISC_RES_EVT to the app, then sends a
   queued DISC_RESULT message (0x207) to the search state machine. */
void bta_dm_disc_result(struct bta_dm_msg *p_data) {
    struct bta_dm_buf_t *p_buf;

    bta_dm_search_cb.p_search_cback(BTA_DM_DISC_RES_EVT, (void *)p_data->disc.bd_addr);
    p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
    if (p_buf != NULL) {
        p_buf->event = BTA_DM_SEARCH_DISC_RES_EVT;
        bta_sys_sendmsg(p_buf);
    }
}

void bta_dm_search_result(struct bta_dm_msg *p_data) {
    if (p_data->disc.services != 0) {
        bta_dm_search_cb.p_search_cback(BTA_DM_DISC_RES_EVT,
                                        p_data->disc.bd_addr);
    }
    if (bta_dm_search_cb.search_disc_active == 0) {
        bta_dm_search_cb.search_disc_active = 0;
        bta_dm_discover_next_device();
    } else {
        bta_dm_search_cb.search_timer_active = 1;
        bta_dm_search_cb.search_timer.p_cback = bta_dm_search_timer_cback;
        bta_sys_start_timer(&bta_dm_search_cb.search_timer, 0, 0xbb8);
    }
}

void bta_dm_search_timer_cback(struct bta_dm_timer_t *p_tle) {
    if (appl_trace_level >= 4)
        LogMsg_0(0x503, " bta_dm_search_timer_cback  ");
    bta_dm_search_cb.search_disc_active = 0;
    bta_dm_discover_next_device();
}

/* Free the SDP database buffer if it exists */
void bta_dm_free_sdp_db() {
    if (bta_dm_search_cb.p_sdp_db != NULL) {
        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;
    }
}

void bta_dm_queue_search(struct bta_dm_msg *p_data) {
    bta_dm_search_cb.p_search_queue = GKI_getbuf(0x1c);
    memcpy(bta_dm_search_cb.p_search_queue, p_data, 0x1c);
}

void bta_dm_queue_disc(struct bta_dm_msg *p_data) {
    bta_dm_search_cb.p_search_queue = GKI_getbuf(0x18);
    memcpy(bta_dm_search_cb.p_search_queue, p_data, 0x18);
}

void bta_dm_search_clear_queue(struct bta_dm_msg *p_data) {
    if (bta_dm_search_cb.p_search_queue != NULL) {
        GKI_freebuf(bta_dm_search_cb.p_search_queue);
        bta_dm_search_cb.p_search_queue = NULL;
    }
}

void bta_dm_search_cancel_cmpl(struct bta_dm_msg *p_data) {
    if (bta_dm_search_cb.p_search_queue != NULL) {
        bta_sys_sendmsg(bta_dm_search_cb.p_search_queue);
        bta_dm_search_cb.p_search_queue = NULL;
    }
}

void bta_dm_search_cancel_transac_cmpl(struct bta_dm_msg *p_data) {
    if (bta_dm_search_cb.p_sdp_db != NULL) {
        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;
    }
    bta_dm_search_cb.p_search_cback(4, NULL);
}

void bta_dm_search_cancel_notify(struct bta_dm_msg *p_data) {
    bta_dm_search_cb.p_search_cback(4, NULL);
}

/* SDP attribute IDs requested for every service (retail .sdata2 pair) */
const unsigned short bta_dm_sdp_attr_protocol_desc_list = 0x0001;
const unsigned short bta_dm_sdp_attr_unknown_8001 = 0x8001;

void bta_dm_find_services(unsigned char *bd_addr) {
    struct bta_dm_disc_msg_t *p_msg;
    struct bta_dm_uuid_t uuid;
    unsigned short attr_list[2];
    unsigned short num_attr;
    unsigned char *p_name;

    attr_list[0] = bta_dm_sdp_attr_protocol_desc_list;
    attr_list[1] = bta_dm_sdp_attr_unknown_8001;
    num_attr = 1;

    while (bta_dm_search_cb.services_index < 23) {
        if ((bta_dm_search_cb.services_cached &
             (1 << bta_dm_search_cb.services_index)) == 0) {
            bta_dm_search_cb.services_index++;
            continue;
        }

        bta_dm_search_cb.p_sdp_db = GKI_getbuf(250);
        if (bta_dm_search_cb.p_sdp_db == NULL) {
            bta_dm_search_cb.services_index++;
            continue;
        }

        uuid.uu.uuid16 =
            bta_service_id_to_uuid_lkup_tbl[bta_dm_search_cb.services_index];
        bta_dm_search_cb.services_cached &=
            ~(1 << bta_dm_search_cb.services_index);

        if (uuid.uu.uuid16 == 0x1200) {
            num_attr = 2;
        }
        uuid.len = 2;

        SDP_InitDiscoveryDb(bta_dm_search_cb.p_sdp_db, 250, 1, &uuid, num_attr,
                            attr_list);

        if (SDP_ServiceSearchAttributeRequest(bd_addr,
                                              bta_dm_search_cb.p_sdp_db,
                                              bta_dm_sdp_callback) == 0) {
            /* request failed: drop the db and give up on this search */
            GKI_freebuf(bta_dm_search_cb.p_sdp_db);
            bta_dm_search_cb.p_sdp_db = NULL;
            bta_dm_search_cb.services_index = 23;
            bta_dm_search_cb.services_index++;
            continue;
        }

        bta_dm_search_cb.services_index++;
        return;
    }

    p_msg = (struct bta_dm_disc_msg_t *)GKI_getbuf(0x110);
    if (p_msg != NULL) {
        p_msg->hdr.event = BTA_DM_SEARCH_RESULT_EVT;
        p_msg->services = bta_dm_search_cb.services_found;
        bdcpy(p_msg->bd_addr, bta_dm_search_cb.disc_bd_addr);

        p_name = (unsigned char *)bta_dm_search_cb.peer_name;
        if (bta_dm_search_cb.peer_name[0] == 0) {
            unsigned char *p_sec_name;

            p_sec_name = (unsigned char *)BTM_SecReadDevName(
                bta_dm_search_cb.disc_bd_addr);
            if (p_sec_name != NULL) {
                p_name = p_sec_name;
            }
        }
        strncpy((char *)p_msg->bd_name, (const char *)p_name, 0x20);
        bta_sys_sendmsg(p_msg);
    }
}

void bta_dm_discover_next_device(void) {
    struct bta_dm_buf_t *p_msg;

    if ((bta_dm_search_cb.p_cur = (struct btm_inq_info_t *)BTM_InqDbNext(
             bta_dm_search_cb.p_cur)) != NULL) {
        if (BTM_IsAclConnectionUp(bta_dm_search_cb.p_cur->results.remote_bd_addr) != 0) {
            bta_dm_search_cb.search_disc_active = 0;
        } else {
            bta_dm_search_cb.search_disc_active = 1;
        }
        bta_dm_search_cb.search_timer_active = 0;
        BTM_SecAddRmtNameNotifyCallback(bta_dm_service_search_remname_cback);
        bta_dm_search_cb.services_index = 0;
        bta_dm_search_cb.services_found = 0;
        bta_dm_search_cb.peer_name[0] = 0;
        bta_dm_search_cb.services_cached = bta_dm_search_cb.services;
        bdcpy(bta_dm_search_cb.disc_bd_addr,
              bta_dm_search_cb.p_cur->results.remote_bd_addr);
        bta_dm_find_services(bta_dm_search_cb.p_cur->results.remote_bd_addr);
    } else {
        bta_dm_search_cb.services = 0;

        p_msg = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
        if (p_msg != NULL) {
            p_msg->event = BTA_DM_SEARCH_DISC_RES_EVT;
            bta_sys_sendmsg(p_msg);
        }
    }
}

// from commit 0183f60318bf needle=bta_dm_sdp_callback
/* SDP discovery complete callback: forwards the status to the search state
   machine as a SDP_RESULT message (0x206). */
void bta_dm_sdp_callback(unsigned short status) {
    struct bta_dm_sdp_result_msg_t *p_msg;

    p_msg = (struct bta_dm_sdp_result_msg_t *)GKI_getbuf(0xA);
    if (p_msg != NULL) {
        p_msg->hdr.event = BTA_DM_SEARCH_SDP_RES_EVT;
        p_msg->sdp_status = status;
        bta_sys_sendmsg(p_msg);
    }
}

void bta_dm_inq_results_cb(struct btm_inq_results_t *p_results,
                           unsigned char *p_eir) {
    union bta_dm_search_t data;
    struct btm_inq_info_t *p_info;

    bdcpy(data.inq_res.bd_addr, p_results->remote_bd_addr);
    memcpy(data.inq_res.dev_class, p_results->dev_class, 3);
    data.inq_res.rssi = p_results->rssi;

    p_info = (struct btm_inq_info_t *)BTM_InqDbRead(p_results->remote_bd_addr);
    if (p_info != NULL) {
        data.inq_res.device_type = 0;
    }

    bta_dm_search_cb.p_search_cback(BTA_DM_INQ_RES_EVT, &data);

    if (p_info != NULL) {
        p_info->appl_knows_rem_name = data.inq_res.device_type;
    }
}

// from commit 702909a6fbbd needle=bta_dm_inq_cmpl_cb
/* Inquiry complete callback: forwards the number of responses to the search
   state machine as a INQUIRY_CMPL message (0x203). */
void bta_dm_inq_cmpl_cb(struct bta_btm_inq_cmpl_t *p_results) {
    struct bta_dm_inq_cmpl_msg_t *p_msg;

    p_msg = (struct bta_dm_inq_cmpl_msg_t *)GKI_getbuf(0x110);
    if (p_msg != NULL) {
        p_msg->hdr.event = BTA_DM_SEARCH_INQ_CMPL_EVT;
        p_msg->num = p_results->num_resp;
        bta_sys_sendmsg(p_msg);
    }
}

void bta_dm_service_search_remname_cback(bd_addr_t bd_addr,
                                         unsigned char *dev_class,
                                         unsigned char *bd_name) {
    strncpy(bta_dm_search_cb.peer_name, (const char *)bd_name, 0x1f);
    BTM_SecDeleteRmtNameNotifyCallback(bta_dm_service_search_remname_cback);
}

void bta_dm_remname_cback(struct btm_remote_dev_name_t *p_remote_name) {
    struct bta_dm_buf_t *p_msg;

    bta_sys_stop_timer(&bta_dm_search_cb.svc_timer);

    p_msg = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
    if (p_msg != NULL) {
        bdcpy(p_msg->data, bta_dm_search_cb.p_cur->results.remote_bd_addr);
        strncpy((char *)p_msg->data + 6,
                (const char *)p_remote_name->remote_bd_name, 0x20);
        p_msg->event = BTA_DM_REMT_NAME_EVT;
        bta_sys_sendmsg(p_msg);
    }
}
// from commit 95ad18ac12e1 needle=bta_dm_disc_remname_cback
/* Remote name request complete during a discovery: same as above but the
   address comes from the discovery control block. */
void bta_dm_disc_remname_cback(struct btm_remote_dev_name_t *p_remote_name) {
    struct bta_dm_buf_t *p_buf;

    bta_sys_stop_timer(&bta_dm_search_cb.svc_timer);
    p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
    if (p_buf != NULL) {
        bdcpy(p_buf->data, bta_dm_search_cb.disc_bd_addr);
        strncpy((char *)p_buf->data + 6,
                (const char *)p_remote_name->remote_bd_name, 0x20);
        p_buf->event = BTA_DM_REMT_NAME_EVT;
        bta_sys_sendmsg(p_buf);
    }
}
void bta_dm_cancel_rmt_name(struct bta_dm_msg *p_data) { BTM_CancelRemoteDeviceName(); }
// from commit 702909a6fbbd needle=bta_dm_authorize_cback
/* Target: us-802e2c1c (0xc8): BTM authorization callback. Looks up the BTA
   service id for the requested BTM service and reports BTA_DM_AUTHORIZE_EVT. */
int bta_dm_authorize_cback(bd_addr_t bd_addr, unsigned char *dev_class,
                           unsigned char *bd_name, unsigned char *p_service_name,
                           unsigned char service_id, unsigned char is_authorized) {
    unsigned char service;
    struct bta_dm_authorize_t auth_req;

    service = 1;
    bdcpy(auth_req.bd_addr, bd_addr);
    strncpy((char *)auth_req.bd_name, (char *)bd_name, 0x20);

    while (service < 24) {
        if (bta_service_id_to_btm_srv_id_lkup_tbl[service] == service_id) {
            auth_req.service = service;
            break;
        }
        service++;
    }

    if (bta_dm_cb.cback != NULL && service <= 23) {
        bta_dm_cb.cback(4, &auth_req);
        return 1;
    }
    return 0xb;
}
// from commit 95ad18ac12e1 needle=bta_dm_pinname_cback
/* Target: us-802e2ce4 (0xc0): PIN request remote-name callback. Builds a
   tBTA_DM_PIN_REQ from the stored pin bd addr/class and the name buffer. */
void bta_dm_pinname_cback(struct btm_remote_dev_name_t *p_remote_name) {
    struct bta_dm_pin_req_t pin_req;

    bdcpy(pin_req.bd_addr, bta_dm_cb.pin_bd_addr);
    pin_req.dev_class[0] = bta_dm_cb.pin_dev_class[0];
    pin_req.dev_class[1] = bta_dm_cb.pin_dev_class[1];
    pin_req.dev_class[2] = bta_dm_cb.pin_dev_class[2];

    if (p_remote_name != NULL && p_remote_name->status == 0) {
        memcpy(pin_req.bd_name, p_remote_name->remote_bd_name,
               (p_remote_name->length < 0x20) ? p_remote_name->length : 0x20);
        pin_req.bd_name[0x20] = 0;
    } else {
        pin_req.bd_name[0] = 0;
    }

    bta_dm_cb.cback(2, &pin_req);
}
/* PIN request callback: if the remote name is empty, kick a name request;
   otherwise report it immediately. */
unsigned char bta_dm_pin_cback(bd_addr_t bd_addr, unsigned char *dev_class,
                               unsigned char *bd_name) {
    union bta_dm_sec_t sec_event;

    if (bta_dm_cb.cback == NULL) {
        return 0xb;
    }
    if (bd_name[0] == 0) {
        bdcpy(bta_dm_cb.pin_bd_addr, bd_addr);
        bta_dm_cb.pin_dev_class[0] = dev_class[0];
        bta_dm_cb.pin_dev_class[1] = dev_class[1];
        bta_dm_cb.pin_dev_class[2] = dev_class[2];
        if ((unsigned char)BTM_ReadRemoteDeviceName(
                bd_addr, (void *)bta_dm_pinname_cback) == 1) {
            return 1;
        }
        if (appl_trace_level >= 2)
            LogMsg_0(0x501, " bta_dm_pin_cback() -> Failed to start Remote Name Request  ");
    }
    bdcpy(sec_event.pin_req.bd_addr, bd_addr);
    sec_event.pin_req.dev_class[0] = dev_class[0];
    sec_event.pin_req.dev_class[1] = dev_class[1];
    sec_event.pin_req.dev_class[2] = dev_class[2];
    strncpy((char *)sec_event.pin_req.bd_name, (const char *)bd_name, 0x20);
    sec_event.pin_req.bd_name[0x20] = 0;
    bta_dm_cb.cback(2, &sec_event);
    return 1;
}
// from commit 702909a6fbbd needle=bta_dm_link_key_request_cback

int bta_dm_link_key_request_cback(void) { return 0xb; }
// from commit 702909a6fbbd needle=bta_dm_new_link_key_cback
/* Target: us-802e2edc (0x9c): new link key notification. Reports
   BTA_DM_AUTH_CMPL_EVT with the new key and key type. */
unsigned char bta_dm_new_link_key_cback(bd_addr_t bd_addr, unsigned char *dev_class,
                                        unsigned char *bd_name, unsigned char *key,
                                        unsigned char key_type) {
    struct bta_dm_auth_cmpl_t auth_cmpl;

    bdcpy(auth_cmpl.bd_addr, bd_addr);
    memcpy(auth_cmpl.bd_name, bd_name, 0x1f);
    auth_cmpl.bd_name[0x1f] = 0;
    memcpy(auth_cmpl.key, key, 0x10);
    auth_cmpl.key_present = 1;
    auth_cmpl.key_type = 1;

    if (bta_dm_cb.cback != NULL)
        bta_dm_cb.cback(3, &auth_cmpl);

    return 1;
}
// --- restored from git history (continued; empty LLM-HARNESS stub spam removed) ---

unsigned char bta_dm_authentication_complete_cback(bd_addr_t bd_addr, unsigned char *dev_class,
                                                   unsigned char *bd_name, unsigned char success) {
    struct bta_dm_auth_cmpl_t auth_cmpl;

    if (success) {
        bdcpy(auth_cmpl.bd_addr, bd_addr);
        memcpy(auth_cmpl.bd_name, bd_name, 0x1f);
        auth_cmpl.bd_name[0x1f] = 0;
        auth_cmpl.key_type = 0;
        auth_cmpl.key_present = 0;

        if (bta_dm_cb.cback != NULL)
            bta_dm_cb.cback(3, &auth_cmpl);
    }
    return 0;
}
// from commit 95ad18ac12e1 needle=bta_dm_local_addr_cback

void bta_dm_local_addr_cback(void *addr) {
    if (bta_dm_cb.cback != NULL) {
        bta_dm_cb.cback(0, addr);
    }
}
// from commit 95ad18ac12e1 needle=bta_dm_signal_strength
/* Start/stop periodic signal-strength polling; start=1 kicks the first read. */
void bta_dm_signal_strength(struct bta_dm_msg *p_data) {
    struct bta_dm_sig_strength_msg_t *d = &p_data->sig_strength;
    if (d->start) {
        bta_dm_cb.signal_strength_mask = d->mask;
        bta_dm_cb.signal_strength_period = d->period;
        bta_dm_signal_strength_timer_cback(NULL);
    } else {
        bta_sys_stop_timer(&bta_dm_cb.signal_strength_timer);
    }
}
// from commit 95ad18ac12e1 needle=bta_dm_signal_strength_timer_cback

void bta_dm_signal_strength_timer_cback(struct bta_dm_timer_t *p_tle) {
    unsigned char i;

    if (bta_dm_cb.signal_strength_mask & 1) {
        for (i = 0; i < bta_dm_cb.num_devices; i++) {
            BTM_ReadRSSI(bta_dm_cb.peer_dev[i].bd_addr, (void *)bta_dm_rssi_cback);
        }
    }
    if (bta_dm_cb.signal_strength_mask & 2) {
        for (i = 0; i < bta_dm_cb.num_devices; i++) {
            BTM_ReadLinkQuality(bta_dm_cb.peer_dev[i].bd_addr,
                                (void *)bta_dm_link_quality_cback);
        }
    }
    if (bta_dm_cb.signal_strength_period != 0) {
        bta_dm_cb.signal_strength_timer.p_cback = bta_dm_signal_strength_timer_cback;
        bta_sys_start_timer(&bta_dm_cb.signal_strength_timer, 0,
                            bta_dm_cb.signal_strength_period * 1000);
    }
}
// from commit 95ad18ac12e1 needle=bta_dm_acl_change_cback
/* ACL database change callback: forward a BTA_DM_ACL_CHANGE_EVT message
   with the changed address and add/remove flag. */
void bta_dm_acl_change_cback(bd_addr_t bd_addr, unsigned char *p_dc, unsigned char *p_bdn,
                             unsigned char *p_features, unsigned char is_new) {
    struct bta_dm_acl_change_msg_t *p_msg;

    p_msg = (struct bta_dm_acl_change_msg_t *)GKI_getbuf(0x12);
    if (p_msg != NULL) {
        bdcpy(p_msg->bd_addr, bd_addr);
        p_msg->is_new = is_new;
        p_msg->hdr.event = BTA_DM_ACL_CHANGE_EVT;
        bta_sys_sendmsg(p_msg);
    }
}
// --- restored from git history (base:gone repo-search) ---
// from commit e48802b591c4 path=libs/RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act.c needle=bta_dm_acl_change
/* ACL up/down: track peer devices; if disable is pending, complete it. */
void bta_dm_acl_change(struct bta_dm_msg *p_data) {
    union bta_dm_sec_t sec_event;
    unsigned char *p_bd_addr = p_data->acl_change.bd_addr;
    unsigned char i;

    if (p_data->acl_change.is_new != 0) {
        /* device connected */
        for (i = 0; i < bta_dm_cb.num_devices; i++) {
            if (bdcmp(bta_dm_cb.peer_dev[i].bd_addr, p_bd_addr) == 0) {
                break;
            }
        }
        if (i == bta_dm_cb.num_devices) {
            bdcpy(bta_dm_cb.peer_dev[bta_dm_cb.num_devices].bd_addr, p_bd_addr);
            bta_dm_cb.num_devices++;
        }
        bta_dm_cb.peer_dev[i].in_use = 1;
        bta_dm_cb.peer_dev[i].policy = 0;
        bdcpy(sec_event.acl_change.bd_addr, p_bd_addr);
        bta_dm_cb.cback(BTA_DM_ACL_CHANGED_EVT, &sec_event);
    } else {
        /* device disconnected */
        for (i = 0; i < bta_dm_cb.num_devices; i++) {
            if (bdcmp(bta_dm_cb.peer_dev[i].bd_addr, p_bd_addr) == 0) {
                while (i < bta_dm_cb.num_devices) {
                    memcpy(&bta_dm_cb.peer_dev[i], &bta_dm_cb.peer_dev[i + 1],
                           sizeof(struct bta_dm_peer_dev_t));
                    i++;
                }
                break;
            }
        }
        bta_dm_cb.num_devices--;
        if (bta_dm_search_cb.search_disc_active != 0 &&
            bdcmp(bta_dm_search_cb.disc_bd_addr, p_bd_addr) == 0) {
            bta_dm_search_cb.search_disc_active = 0;
            if (bta_dm_search_cb.search_timer_active != 0) {
                if (appl_trace_level >= 4)
                    LogMsg_0(0x503, " timer stopped  ");
                bta_sys_stop_timer(&bta_dm_search_cb.search_timer);
                bta_dm_discover_next_device();
            }
        }
        if (bta_dm_cb.disable_timer_active != 0) {
            if (BTM_GetNumAclLinks() == 0) {
                bta_dm_cb.disable_timer_active = 0;
                bta_sys_stop_timer(&bta_dm_cb.disable_timer);
                bta_dm_cb.disable_timer.p_cback = bta_dm_disable_conn_down_timer_cback;
                bta_sys_start_timer(&bta_dm_cb.disable_timer, 0, 1000);
            }
        }
        bdcpy(sec_event.acl_change.bd_addr, p_bd_addr);
        sec_event.acl_change.reason = (unsigned char)btm_get_acl_disc_reason_code();
        bta_dm_cb.cback(BTA_DM_ACL_DOWN_EVT, &sec_event);
    }
}
/* The remainder of the unit: BTM callback adapters (pin/name/key/auth),
   ACL change, RSSI and link quality, the SDP walk (find_services) and the
   role-management / compression hooks.  bta_dm_compress_cback is still a
   stub: its BTA compression state machine was never recovered.  The rest
   match the retail instruction stream, with any residual register-colour
   differences recorded per function.
   === END LOST REGION === */

// --- restored from git history (base:gone); do not expand beyond these functions ---
// from commit 0183f60318bf needle=bta_dm_disable_conn_down_timer_cback

void bta_dm_disable_conn_down_timer_cback(struct bta_dm_timer_t *p_tle) {
    (void)p_tle;
    bta_dm_cb.cback(1, 0);
}
// from commit 95ad18ac12e1 needle=bta_dm_rssi_cback
/* Target: us-802e3460 (0x6c): RSSI read result callback. Reports
   BTA_DM_SIG_STRENGTH_EVT when the read succeeded. */
void bta_dm_rssi_cback(struct btm_rssi_results_t *p_rssi) {
    struct bta_dm_sig_strength_t sig_strength;

    if (p_rssi->status == 0) {
        bdcpy(sig_strength.bd_addr, p_rssi->rem_bda);
        sig_strength.mask = 1;
        sig_strength.rssi_value = p_rssi->rssi;
        (*bta_dm_cb.cback)(7, &sig_strength);
    }
}
// from commit 95ad18ac12e1 needle=bta_dm_link_quality_cback
/* Target: us-802e34cc (0x6c): link quality read result callback. Reports
   BTA_DM_SIG_STRENGTH_EVT when the read succeeded. */
void bta_dm_link_quality_cback(void *p_result) {
    struct btm_link_quality_results_t *p_lq = (struct btm_link_quality_results_t *)p_result;
    struct bta_dm_sig_strength_t sig_strength;

    if (p_lq->status == 0) {
        bdcpy(sig_strength.bd_addr, p_lq->rem_bda);
        sig_strength.mask = 2;
        sig_strength.link_quality_value = p_lq->link_quality;
        bta_dm_cb.cback(7, &sig_strength);
    }
}
// from commit 95ad18ac12e1 needle=bta_dm_l2cap_server_compress_cback
/* L2CAP server compress callback: look up the peer and allocate compress
   memory via the co layer. Unused client/data pointers are accepted. */
unsigned char bta_dm_l2cap_server_compress_cback(
        bd_addr_t bd_addr, unsigned char server_id, unsigned char client_id,
        unsigned char *p_data, unsigned int data_len, unsigned char *p_data2,
        unsigned int data_len2, unsigned char **pp_memory,
        unsigned int *p_memory_size) {
    unsigned char result = 0;
    int i;

    for (i = 0; i < 5; i++) {
        if (bta_dm_compress_srvcs[i].in_use == 1 &&
            bdcmp(bta_dm_compress_srvcs[i].bd_addr, bd_addr) == 0) {
            if (appl_trace_level >= 4)
                LogMsg_1(0x503, "bta_dm_l2cap_server_compress_cback, BTA ID %d",
                         bta_dm_compress_srvcs[i].server_id);
            result = bta_dm_co_get_compress_memory(
                bta_dm_compress_srvcs[i].server_id, pp_memory, p_memory_size);
            break;
        }
    }
    return result;
}
/* NOTE(unrecovered): retail's 0x238-byte BTA compression state machine (open/
   close of the L2CAP compression channel per app id) is not reconstructed yet;
   this stub also accounts for the .data (0x79 bytes of trace strings) and the
   .sdata2 attribute pair still missing from the unit's data sections. */
void bta_dm_compress_cback(void) {}
// from commit 95ad18ac12e1 needle=bta_dm_rm_cback
/* Role-management callback: update peer policy from the RM config table. */
void bta_dm_rm_cback(int status, unsigned char id, unsigned char app_id,
                     bd_addr_t peer_addr) {
    unsigned char num;
    unsigned char *p_cfg;
    unsigned char i;
    unsigned char j;
    if (status == 0) {
        for (i = 0; i < bta_dm_cb.num_devices; i++) {
            if (bdcmp(bta_dm_cb.peer_dev[i].bd_addr, peer_addr) == 0) {
                bta_dm_cb.peer_dev[i].in_use = 1;
                p_cfg = p_bta_dm_rm_cfg;
                num = p_cfg[1];
                j = 1;
                while (j <= num) {
                    if (p_cfg[3 * j + 1] == app_id || p_cfg[3 * j + 1] == 0xff) {
                        if (p_cfg[3 * j] == id) {
                            unsigned char cfg_val = p_cfg[3 * j + 2];
                            if (cfg_val > bta_dm_cb.peer_dev[i].policy) {
                                bta_dm_cb.peer_dev[i].policy = cfg_val;
                            }
                            break;
                        }
                    }
                    j++;
                }
                break;
            }
        }
    }
}
void bta_dm_keep_acl(struct bta_dm_msg *p_data) {
    bta_dm_cb.keep_acl = (unsigned char)p_data->hdr.layer_specific;

    if (bta_dm_cb.keep_acl != 0) {
        L2CA_SetIdleTimeoutByBdAddr(BT_BD_ANY, 0xffff);
        L2CA_SetIdleTimeout(0, 0xffff, 1);
    } else {
        L2CA_SetIdleTimeoutByBdAddr(BT_BD_ANY, 2);
        L2CA_SetIdleTimeout(0, 2, 1);
    }
}
void bta_dm_immediate_disable(struct bta_dm_msg *p_data) {
    bta_dm_cb.cback(1, NULL);
}
// from commit 702909a6fbbd needle=bta_dm_reset_complete

void bta_dm_reset_complete(void) {}
/* Send an HCI reset: mark reset pending, disable scanning until the reset
   completes, then ask BTM to send the reset command. */
void bta_dm_send_hci_reset(struct bta_dm_msg *p_data) {
    void *p_buf;

    bta_sys_cb[0x7E] = 1;
    p_buf = GKI_getpoolbuf(2);
    if (p_buf != NULL) {
        btsnd_hcic_write_scan_enable(p_buf, 0);
    }
    BTM_SendHciReset((void *)bta_dm_reset_complete);
}

// Retail data owned by this TU (link undefineds; contents from the retail
// split object). .rodata order matches retail: uuid table, btm table,
// security block (nm offsets 0x00/0x30/0x90).
const unsigned short bta_service_id_to_uuid_lkup_tbl[24] = {
    0x1200, 0x1101, 0x1103, 0x1111, 0x1102, 0x1108, 0x111e, 0x1105,
    0x1106, 0x1109, 0x1110, 0x1104, 0x1118, 0x111b, 0x1115, 0x1116,
    0x1117, 0x112d, 0x110b, 0x110e, 0x1124, 0x1304, 0x112f, 0x0000,
};
const unsigned int bta_service_id_to_btm_srv_id_lkup_tbl[24] = {
    0x00000000,
    0x00000001,
    0x00000003,
    0x0000000b,
    0x00000002,
    0x0000000c,
    0x0000001d,
    0x00000006,
    0x00000007,
    0x00000009,
    0x0000000a,
    0x00000004,
    0x00000016,
    0x00000023,
    0x00000019,
    0x0000001b,
    0x0000001a,
    0x00000028,
    0x00000025,
    0x00000027,
    0x00000020,
    0x00000025,
    0x0000002c,
    0x00000000,
};
// All-zero const aggregates migrate to .sbss2; the non-const zero array
// honours declspec and stays in .rodata (CCharVoice lbl_eu_805106D4 recipe).
extern __declspec(section ".rodata") unsigned char bta_security[0x18] = { 0 };
struct bta_dm_compress_srvc_t bta_dm_compress_srvcs[5];
