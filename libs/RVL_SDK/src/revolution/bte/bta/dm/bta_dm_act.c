// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act
// Replace stubs with high-level C/C++ during decomp.

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
    void *p_cur;                            /* offset 0x04 */
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
};

/* Search SM message events (row index = event & 0xff) */
#define BTA_DM_SEARCH_INQ_CMPL_EVT  0x203
#define BTA_DM_SEARCH_SDP_RES_EVT   0x206
#define BTA_DM_SEARCH_DISC_RES_EVT  0x207
#define BTA_DM_REMT_NAME_EVT        0x204
#define BTA_DM_ACL_CHANGE_EVT       0x105
#define BTA_DM_INQ_RES_EVT          0

/* Search callback events (bta_api.h) */
#define BTA_DM_INQ_CMPL_EVT             1
#define BTA_DM_DISC_RES_EVT             2
#define BTA_DM_DISC_CMPL_EVT            3
#define BTA_DM_SEARCH_CANCEL_CMPL_EVT   4

/* DM security callback events (bta_api.h) */
#define BTA_DM_ACL_CHANGED_EVT          5
#define BTA_DM_ACL_DOWN_EVT             6

/* Search SM message events */
#define BTA_DM_SEARCH_CMPL_EVT          0x208

/* Security callback event structures (mirror bta_api.h tBTA_DM_* layouts) */
struct bta_dm_pin_req_t {
    bd_addr_t bd_addr;              /* offset 0x00 */
    unsigned char bd_name[248];     /* offset 0x06, BD_NAME_LEN = 248 */
    unsigned char dev_class[3];     /* offset 0xfe */
};

struct bta_dm_auth_cmpl_t {
    bd_addr_t bd_addr;              /* offset 0x00 */
    unsigned char bd_name[248];     /* offset 0x06 */
    unsigned char key_present;      /* offset 0xfe */
    unsigned char key[16];          /* offset 0xff */
    unsigned char key_type;         /* offset 0x10f */
};

struct bta_dm_authorize_t {
    bd_addr_t bd_addr;              /* offset 0x00 */
    unsigned char bd_name[248];     /* offset 0x06 */
    unsigned char service;          /* offset 0xfe */
    unsigned char dev_class[3];     /* offset 0xff */
    unsigned char _pad[22];         /* offset 0x102-0x117 */
};

struct bta_dm_sig_strength_t {
    bd_addr_t bd_addr;              /* offset 0x00 */
    unsigned char mask;             /* offset 0x06 */
    unsigned char rssi_value;       /* offset 0x07 */
    unsigned char link_quality_value; /* offset 0x08 */
};

struct btm_rssi_results_t {
    unsigned char status;           /* offset 0x00 */
    unsigned char hci_status;       /* offset 0x01 */
    signed char rssi;               /* offset 0x02 */
    bd_addr_t rem_bda;              /* offset 0x03 */
};

struct btm_link_quality_results_t {
    unsigned char status;           /* offset 0x00 */
    unsigned char hci_status;       /* offset 0x01 */
    unsigned char link_quality;     /* offset 0x02 */
    bd_addr_t rem_bda;              /* offset 0x03 */
};

/* Short BT_HDR: event/len followed by payload */
struct bta_dm_buf_t {
    unsigned short event;           /* offset 0x00 */
    unsigned short len;             /* offset 0x02 */
    unsigned char data[1];          /* offset 0x04 */
};

/* Discover message (tBTA_DM_API_DISCOVER): hdr(8) + bd_addr(6) + pad(2) +
   services(4) + p_cback(4). */
struct bta_dm_discover_msg_t {
    unsigned char hdr[8];
    bd_addr_t bd_addr;              /* offset 0x08 */
    unsigned char _pad[2];          /* offset 0x0e-0x0f */
    unsigned int services;          /* offset 0x10 */
    void *p_cback;                  /* offset 0x14 */
};

/* Stack DISC_RES-style buffer used by the inquiry-complete / remote-name
   handlers: bd_addr + result byte + services mask (0x104; the 0x110 frame
   allocation is completed by the inq_cmpl buffer in bta_dm_inq_cmpl). */
struct bta_dm_disc_res_local_t {
    bd_addr_t bd_addr;          /* offset 0x00 */
    unsigned char result;       /* offset 0x06 */
    unsigned char _pad[0xf9];   /* offset 0x07-0xff */
    unsigned int services;      /* offset 0x100 */
};

/* Minimal tSDP_UUID (tBT_UUID): len (u16) + alignment pad + uu union. */
struct bta_sdp_uuid_t {
    unsigned short len;         /* offset 0x00 */
    unsigned short _pad;        /* offset 0x02 */
    union {
        unsigned short uuid16;
        unsigned int uuid32;
        unsigned char uuid128[16];
    } uu;                       /* offset 0x04 */
};

/* Inquiry result input (tBTM_INQ_RESULTS, EIR disabled layout) */
struct bta_btm_inq_results_t {
    unsigned short clock_offset;        /* offset 0x00 */
    bd_addr_t remote_bd_addr;           /* offset 0x02 */
    unsigned char dev_class[3];         /* offset 0x08 */
    unsigned char page_scan_rep_mode;   /* offset 0x0b */
    unsigned char page_scan_per_mode;   /* offset 0x0c */
    unsigned char page_scan_mode;       /* offset 0x0d */
    signed char rssi;                   /* offset 0x0e */
};

/* Inquiry result reported to the app (tBTA_DM_INQ_RES retail layout) */
struct bta_dm_inq_res_t {
    bd_addr_t bd_addr;              /* offset 0x00 */
    unsigned char dev_class[3];     /* offset 0x06 */
    signed char rssi;               /* offset 0x09 */
    unsigned char device_type;      /* offset 0x0a */
};

/* tBTA_DM_SEARCH union sized to the retail 0x110-byte discovery result */
struct bta_dm_search_res_t {
    struct bta_dm_inq_res_t inq_res;    /* offset 0x00 */
    unsigned char _pad[0x110 - 0x0b];   /* offset 0x0b-0x10f */
};

extern struct bta_dm_search_cb_t bta_dm_search_cb;

/* bta_sys_cb (BSS global, 0x8c bytes) - byte access like bta_sys_main.c */
extern unsigned char bta_sys_cb[];

typedef void (*bta_dm_cback_t)(int, void *);

/* Peer device entry: BD address plus power-management state bytes. */
struct bta_dm_peer_dev_t {
    bd_addr_t bd_addr;      /* offset 0x00 */
    unsigned char in_use;   /* offset 0x06 */
    unsigned char policy;   /* offset 0x07 */
    unsigned char _pad[3];  /* offset 0x08-0x0a */
};

struct bta_dm_cb_t {
    struct bta_dm_peer_dev_t peer_dev[7];           /* offset 0x00-0x4c */
    unsigned char num_devices;                      /* offset 0x4d */
    unsigned char _pad4e[2];                        /* offset 0x4e-0x4f */
    bta_dm_cback_t cback;                           /* offset 0x50 */
    struct bta_dm_timer_t signal_strength_timer;    /* offset 0x54, size 0x18 */
    unsigned char signal_strength_mask;             /* offset 0x6c */
    unsigned char _pad6d[3];
    unsigned short signal_strength_period;          /* offset 0x70 */
    unsigned char disable_timer_active;             /* offset 0x72 */
    unsigned char _pad73;
    struct bta_dm_timer_t disable_timer;            /* offset 0x74, size 0x18 */
    unsigned char _pad8c[0x6c];                     /* offset 0x8c-0xf7 */
    bd_addr_t pin_bd_addr;                          /* offset 0xf8 */
    unsigned char pin_dev_class[3];                 /* offset 0xfe */
    unsigned char keep_acl;                         /* offset 0x101 */
    unsigned char _pad102[2];                       /* offset 0x102-0x103 */
};

extern struct bta_dm_cb_t bta_dm_cb;

extern void bta_sys_stop_timer(struct bta_dm_timer_t *p_tle);
extern void bta_sys_start_timer(struct bta_dm_timer_t *p_tle, unsigned short type, int timeout);
extern void bta_sys_disable();
extern void bta_sys_rm_register(void *p_cback);
extern void bta_sys_compress_register(void *p_cback);
extern unsigned short BTM_GetNumAclLinks(void);
extern int btm_remove_acl(bd_addr_t bd_addr);
extern void BTM_SetDeviceClass(unsigned char *dev_class);
extern int BTM_SecRegister(void *p_cb_info);
extern void BTM_SetDefaultLinkSuperTout(unsigned short timeout);
extern void BTM_WritePageTimeout(unsigned short timeout);
extern void BTM_SetDefaultLinkPolicy(unsigned short settings);
extern int BTM_AclRegisterForChanges(void *p_cback);
extern int BTM_ReadLocalDeviceAddr(void *p_cb);
extern void bta_dm_init_pm();
extern void bta_dm_disable_pm();
extern void L2CA_RegisterCompression(void *p_cback, int compression_level);
extern int WBT_ExtCreateRecord(void);
extern void BTM_DeviceAuthorized(bd_addr_t bd_addr, unsigned char res, unsigned int *p_trusted_mask);
extern unsigned char BTM_ReadRSSI(bd_addr_t bd_addr, void *p_cb);
extern unsigned char BTM_ReadLinkQuality(bd_addr_t bd_addr, void *p_cb);
extern unsigned char BTM_ReadRemoteDeviceName(bd_addr_t bd_addr, void *p_cb);
extern int BTM_SecAddRmtNameNotifyCallback(void *p_callback);
extern void *BTM_InqDbNext(void *p_cur);
extern void *BTM_InqDbFirst(void);
extern char *BTM_SecReadDevName(bd_addr_t bd_addr);
extern unsigned short btm_get_acl_disc_reason_code(void);
extern unsigned char SDP_InitDiscoveryDb(void *p_db, unsigned int len, unsigned short num_uuid,
                                          void *p_uuid_list, unsigned short num_attr,
                                          unsigned short *p_attr_list);
extern unsigned char SDP_ServiceSearchAttributeRequest(unsigned char *p_bd_addr, void *p_db,
                                                       void *p_cb);
extern void *SDP_FindServiceInDb(void *p_db, unsigned short service_uuid,
                                 void *p_start_rec);
extern void *SDP_FindAttributeInRec(void *p_rec, unsigned short attr_id);
extern unsigned char BTM_IsAclConnectionUp(bd_addr_t bd_addr);
extern unsigned char bta_dm_co_get_compress_memory(unsigned char id, unsigned char **pp_memory,
                                                   unsigned int *p_memory_size);
extern int bdcmp(const bd_addr_t a, const bd_addr_t b);
extern void LogMsg_1(unsigned int trace_set_mask, const char *p_str, unsigned int p1);

/* BTA DM compile-time configuration (defined in bta_dm_cfg.c) */
extern struct bta_dm_cfg_retail_t {
    unsigned char dev_class[3];     /* offset 0x00 */
    unsigned char _pad3;
    unsigned short policy_settings; /* offset 0x04 */
    unsigned short page_timeout;    /* offset 0x06 */
    unsigned short link_timeout;    /* offset 0x08 */
    unsigned char avoid_scatter;
    unsigned char _padb;
} bta_dm_cfg;

/* BTM control block (partial: page timeout field at 0x64c) */
extern struct bta_dm_btm_cb_t {
    unsigned char _pad[0x64c];
    unsigned short page_timeout;    /* offset 0x64c */
} btm_cb;

/* Role management config table pointer (bta_dm_cfg.c, 3-byte entries:
   id, app_id, cfg; count packed in entry 0's app_id byte). Volatile so
   the read stays in place below the outer loop's bdcmp call. */
extern unsigned char *volatile p_bta_dm_rm_cfg;
extern unsigned char bta_sys_sendmsg(void *p_msg);
extern void GKI_freebuf(void *p_buf);
extern void *GKI_getbuf(unsigned short size);
extern void *GKI_getpoolbuf(unsigned char pool_id);
extern unsigned short BTM_IsInquiryActive(void);
extern int BTM_CancelInquiry(void);
extern int BTM_CancelRemoteDeviceName(void);
extern void *BTM_InqDbRead(unsigned char *p_bda);
extern unsigned int *btm_read_trusted_mask(unsigned char *bd_addr);
extern void BTM_PINCodeReply(unsigned char *bd_addr, unsigned char res, unsigned char pin_len,
                             unsigned char *p_pin, unsigned char *p_trust_mask);
extern int BTM_SecBond(unsigned char *bd_addr, unsigned char pin_len, unsigned char *p_pin,
                       unsigned int *p_trusted_mask);
extern int L2CA_SetIdleTimeout(unsigned short cid, unsigned short timeout, unsigned char is_orig);
extern int L2CA_SetIdleTimeoutByBdAddr(const unsigned char *bd_addr, unsigned short timeout);
extern const unsigned char BT_BD_ANY[6];
extern unsigned char BTM_SetDiscoverability(unsigned short inq_mode, unsigned short window,
                                            unsigned short interval);
extern unsigned char BTM_SetConnectability(unsigned short page_mode, unsigned short window,
                                           unsigned short interval);
extern void BTM_ClearInqDb(void *p_bda);
extern unsigned char BTM_StartInquiry(void *p_inqparms, void *p_results_cb, void *p_cmpl_cb);
extern void BTM_SendHciReset(void *p_cb);
extern void btsnd_hcic_write_scan_enable(void *p_buf, unsigned char scan_enable);
extern int BTM_SecDeleteRmtNameNotifyCallback(bta_dm_rmt_name_cback_t p_callback);
extern void LogMsg_0(unsigned int trace_set_mask, const char *p_str);
void bta_dm_signal_strength_timer_cback(struct bta_dm_timer_t *p_tle);
void bta_dm_search_timer_cback(struct bta_dm_timer_t *p_tle);
void bta_dm_discover_next_device(void);
void bta_dm_inq_results_cb();
void bta_dm_inq_cmpl_cb(struct bta_btm_inq_cmpl_t *p_results);
void bta_dm_reset_complete();
void bta_dm_acl_change_cback(bd_addr_t bd_addr, unsigned char *p_dc, unsigned char *p_bdn,
                             unsigned char *p_features, unsigned char is_new);
void bta_dm_local_addr_cback(void *addr);
void bta_dm_rm_cback(int status, unsigned char id, unsigned char app_id,
                     bd_addr_t peer_addr);
void bta_dm_compress_cback(unsigned char action, unsigned char server_id,
                           unsigned char client_id, bd_addr_t bd_addr);
unsigned char bta_dm_l2cap_server_compress_cback(
        bd_addr_t bd_addr, unsigned char server_id, unsigned char client_id,
        unsigned char *p_data, unsigned int data_len, unsigned char *p_data2,
        unsigned int data_len2, unsigned char **pp_memory,
        unsigned int *p_memory_size);
void bta_dm_service_search_remname_cback(unsigned char *bd_addr, unsigned char *dc,
                                         unsigned char *bd_name);
void bta_dm_remname_cback(void *p_data);
void bta_dm_disc_remname_cback(void *p_data);
void bta_dm_pinname_cback(void *p_data);
unsigned char bta_dm_pin_cback(bd_addr_t bd_addr, unsigned char *dev_class,
                               unsigned char *bd_name);
void bta_dm_disable_timer_cback(struct bta_dm_timer_t *p_tle);
void bta_dm_disable_conn_down_timer_cback();
void bta_dm_rssi_cback(struct btm_rssi_results_t *p_rssi);
void bta_dm_link_quality_cback(void *p_result);
void bta_dm_sdp_callback(unsigned short status);
void bta_dm_find_services(bd_addr_t bd_addr);

/* --- BTE helper externs (bd.h / bta_sys.h / bte_appl.h) --- */
extern void bdcpy(bd_addr_t a, const bd_addr_t b);
extern void *memcpy(void *p_dst, const void *p_src, unsigned long n);
extern void *memset(void *p_dst, int c, unsigned long n);
extern char *strncpy(char *p_dst, const char *p_src, unsigned long n);
extern unsigned char appl_trace_level;
extern void LogMsg_3(unsigned int trace_set_mask, const char *p_str, unsigned int p1,
                     unsigned int p2, unsigned int p3);

/* Compress config: entry index i at byte offset 3*i, count at byte 1 */
struct bta_dm_compress_srvc_t {
    bd_addr_t bd_addr;      /* offset 0x00 */
    unsigned char server_id; /* offset 0x06 */
    unsigned char client_id; /* offset 0x07 */
    unsigned char in_use;   /* offset 0x08 */
};

// .bss pad: 3 zero bytes after bta_dm_compress_srvcs. Declared before it (and
// as scalars) so the reverse-order .bss emission packs them at 0x2D..0x30.
#pragma sdata_threshold 0
unsigned char bta_dm_act_bss_pad3;
unsigned char bta_dm_act_bss_pad2;
unsigned char bta_dm_act_bss_pad1;
#pragma sdata_threshold 8
struct bta_dm_compress_srvc_t bta_dm_compress_srvcs[5];

/* Connected services table: byte 0 = count, 9-byte entries, service id at +7
   (defined in bta_dm_pm.c, retail bss owner) */
extern unsigned char bta_dm_conn_srvcs[0x2e];

extern unsigned char *p_bta_dm_compress_cfg;

/* BTA service id -> SDP UUID lookup (retail rodata, 12 u32 pairs) */
const unsigned int bta_service_id_to_uuid_lkup_tbl[12] = {
    0x12001101, 0x11031111, 0x11021108, 0x111e1105,
    0x11061109, 0x11101104, 0x1118111b, 0x11151116,
    0x1117112d, 0x110b110e, 0x11241304, 0x112f0000,
};

/* BTA service id -> BTM service id lookup (bta_dm_act.c retail rodata) */
const unsigned int bta_service_id_to_btm_srv_id_lkup_tbl[24] = {
    0x00000000, 0x00000001, 0x00000003, 0x0000000b,
    0x00000002, 0x0000000c, 0x0000001d, 0x00000006,
    0x00000007, 0x00000009, 0x0000000a, 0x00000004,
    0x00000016, 0x00000023, 0x00000019, 0x0000001b,
    0x0000001a, 0x00000028, 0x00000025, 0x00000027,
    0x00000020, 0x00000025, 0x0000002c, 0x00000000,
};

/* DM security callbacks table (retail rodata, registered via BTM_SecRegister) */
struct bta_security_t {
    void *p_authorize_callback;
    void *p_pin_callback;
    void *p_link_key_callback;
    void *p_link_key_notify_callback;
    void *p_auth_complete_callback;
    void *p_bond_callback;
};

extern const struct bta_security_t bta_security;

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

/* Disable timeout expired: if ACL links are still up (and we are not
   keeping them), drop every peer device, otherwise report the disable. */

extern void BTM_SetLocalDeviceName(void*);
void bta_dm_set_dev_name(struct bta_dm_msg *p_data) { ((void(*)(void*))BTM_SetLocalDeviceName)(p_data->set_name.name); }

/* Set discoverability and connectability from the incoming message */
void bta_dm_set_visibility(struct bta_dm_msg *p_data) {
    BTM_SetDiscoverability(p_data->set_visibility.disc_mode, 0, 0);
    BTM_SetConnectability(p_data->set_visibility.conn_mode, 0, 0);
}

void bta_dm_bond(struct bta_dm_msg *p_data) {
    unsigned char trusted_mask[8];
    unsigned int *p_mask;

    p_mask = btm_read_trusted_mask(p_data->bond.bd_addr);
    if (p_mask != NULL) {
        memcpy(trusted_mask, p_mask, 8);
    } else {
        memset(trusted_mask, 0, 8);
    }
    BTM_SecBond(p_data->bond.bd_addr, p_data->bond.pin_len, p_data->bond.p_pin,
                (unsigned int *)trusted_mask);
}

/* Bond/pin reply with a remote device: read the trusted mask for the
   address, then pass it down to the security manager. */
void bta_dm_pin_reply(struct bta_dm_msg *p_data) {
    unsigned char trusted_mask[8];
    unsigned int *p_mask;

    p_mask = btm_read_trusted_mask(p_data->pin_reply.bd_addr);
    if (p_mask != NULL) {
        memcpy(trusted_mask, p_mask, 8);
    } else {
        memset(trusted_mask, 0, 8);
    }
    if (p_data->pin_reply.accept != 0) {
        BTM_PINCodeReply(p_data->pin_reply.bd_addr, 0,
                         p_data->pin_reply.pin_len, p_data->pin_reply.p_pin,
                         trusted_mask);
    } else {
        BTM_PINCodeReply(p_data->pin_reply.bd_addr, 0xb, 0, NULL, trusted_mask);
    }
}

/* Authorization reply: set the trusted mask bit for the requested service
   (if any) and pass the result down to the security manager. */
void bta_dm_auth_reply(struct bta_dm_msg *p_data) {
    unsigned char trusted_mask[8];
    unsigned int *p_mask;

    p_mask = btm_read_trusted_mask(p_data->auth_reply.bd_addr);
    if (p_mask != NULL) {
        memcpy(trusted_mask, p_mask, 8);
    } else {
        memset(trusted_mask, 0, 8);
    }

    if (p_data->auth_reply.accept != 2) {
        if (p_data->auth_reply.accept == 0 && p_data->auth_reply.srv_id <= 0x17) {
            unsigned int srv_id =
                bta_service_id_to_btm_srv_id_lkup_tbl[p_data->auth_reply.srv_id];
            ((unsigned int *)trusted_mask)[(srv_id >> 5) & 0xff] |=
                1u << (srv_id - (srv_id & 0x1fe0));
        }
        BTM_DeviceAuthorized(p_data->auth_reply.bd_addr, 0,
                             (unsigned int *)trusted_mask);
    } else {
        BTM_DeviceAuthorized(p_data->auth_reply.bd_addr, 0xb,
                             (unsigned int *)trusted_mask);
    }
}

/* Starts an inquiry: clears the inquiry DB, stores the search callback and
   services from the API message, then starts the inquiry with the local
   result/completion callbacks. */
void bta_dm_search_start(struct bta_dm_msg *p_data) {
    struct bta_dm_search_msg_t *p_search = (struct bta_dm_search_msg_t *)p_data;
    void *p_results_cb = (void *)bta_dm_inq_results_cb;
    void *p_cmpl_cb = (void *)bta_dm_inq_cmpl_cb;

    BTM_ClearInqDb(NULL);
    bta_dm_search_cb.p_search_cback = (bta_dm_search_cback_t)p_search->p_cback;
    bta_dm_search_cb.services = p_search->services;
    BTM_StartInquiry(&p_search->inq_params, p_results_cb, p_cmpl_cb);
}

void bta_dm_search_cancel(struct bta_dm_msg *p_data) {
    struct bta_dm_buf_t *p_buf;

    BTM_ClearInqDb(NULL);
    if (BTM_IsInquiryActive() != 0) {
        BTM_CancelInquiry();
        bta_dm_search_cb.p_search_cback(BTA_DM_SEARCH_CANCEL_CMPL_EVT, NULL);
        p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
        if (p_buf != NULL) {
            p_buf->event = BTA_DM_SEARCH_DISC_RES_EVT;
            bta_sys_sendmsg(p_buf);
        }
    } else if (bta_dm_search_cb.services == 0) {
        BTM_CancelRemoteDeviceName();
    }
}

/* Discovery: stash the callback and services, register the remote-name
   notify callback, then either start the service search or (when no
   services are requested) do a plain remote name request. */
void bta_dm_discover(struct bta_dm_msg *p_data) {
    struct bta_dm_discover_msg_t *p_msg = (struct bta_dm_discover_msg_t *)p_data;
    struct bta_dm_buf_t *p_buf;

    bta_dm_search_cb.p_search_cback = (bta_dm_search_cback_t)p_msg->p_cback;
    bta_dm_search_cb.services = p_msg->services;
    bta_dm_search_cb.services_cached = bta_dm_search_cb.services;
    bta_dm_search_cb.services_index = 0;
    bta_dm_search_cb.services_found = 0;
    bta_dm_search_cb.peer_name[0] = 0;
    BTM_SecAddRmtNameNotifyCallback(bta_dm_service_search_remname_cback);
    bdcpy(bta_dm_search_cb.disc_bd_addr, p_msg->bd_addr);

    if (bta_dm_search_cb.services != 0) {
        bta_dm_find_services(bta_dm_search_cb.disc_bd_addr);
    } else {
        if (btm_cb.page_timeout < 0x7530) {
            bta_sys_start_timer(&bta_dm_search_cb.svc_timer, 0x205,
                                btm_cb.page_timeout + 0x64);
        }
        if (BTM_ReadRemoteDeviceName(bta_dm_search_cb.disc_bd_addr,
                                     (void *)bta_dm_disc_remname_cback) != 1) {
            bta_sys_stop_timer(&bta_dm_search_cb.svc_timer);
            p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
            if (p_buf != NULL) {
                bdcpy(p_buf->data + 4, bta_dm_search_cb.disc_bd_addr);
                p_buf->data[0xa] = 0;
                p_buf->event = BTA_DM_REMT_NAME_EVT;
                bta_sys_sendmsg(p_buf);
            }
        }
    }
}

/* Inquiry complete: walk the inquiry DB. With services requested, kick off
   the service search for the first device. Without services, report every
   device whose name is already known; for the first unknown name, start a
   remote-name request and let the result message continue the search. If no
   name request was started, finalize the search with a DISC_RESULT message. */
void bta_dm_inq_cmpl(struct bta_dm_msg *p_data) {
    unsigned char found = 1;
    unsigned char inq_cmpl[0x108];
    struct bta_dm_disc_res_local_t disc_res;
    struct bta_dm_buf_t *p_buf;

    bta_dm_search_cb.p_cur = BTM_InqDbFirst();
    if (bta_dm_search_cb.p_cur != NULL) {
        found = 0;
        if (bta_dm_search_cb.services != 0) {
            if (BTM_IsAclConnectionUp((unsigned char *)bta_dm_search_cb.p_cur + 2) != 0) {
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
                  (unsigned char *)bta_dm_search_cb.p_cur + 2);
            bta_dm_find_services((unsigned char *)bta_dm_search_cb.p_cur + 2);
        } else {
            found = 1;
            while (bta_dm_search_cb.p_cur != NULL) {
                if (((unsigned char *)bta_dm_search_cb.p_cur)[0x10] != 0) {
                    bdcpy(disc_res.bd_addr, (unsigned char *)bta_dm_search_cb.p_cur + 2);
                    disc_res.result = 0;
                    disc_res.services = 0;
                    bta_dm_search_cb.p_search_cback(BTA_DM_DISC_RES_EVT, &disc_res);
                } else {
                    if (btm_cb.page_timeout < 0x7530) {
                        bta_sys_start_timer(&bta_dm_search_cb.svc_timer, 0x205,
                                            btm_cb.page_timeout + 0x64);
                    }
                    if (BTM_ReadRemoteDeviceName((unsigned char *)bta_dm_search_cb.p_cur + 2,
                                                 (void *)bta_dm_remname_cback) != 1) {
                        unsigned char *p_rem_addr;

                        bta_sys_stop_timer(&bta_dm_search_cb.svc_timer);
                        p_rem_addr = (unsigned char *)bta_dm_search_cb.p_cur + 2;
                        p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
                        if (p_buf != NULL) {
                            bdcpy(p_buf->data + 4, p_rem_addr);
                            p_buf->data[0xa] = 0;
                            p_buf->event = BTA_DM_REMT_NAME_EVT;
                            bta_sys_sendmsg(p_buf);
                        }
                    }
                    found = 0;
                    break;
                }
                bta_dm_search_cb.p_cur = BTM_InqDbNext(bta_dm_search_cb.p_cur);
            }
        }
    }
    if (found != 0) {
        bta_dm_search_cb.services = 0;
        p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
        if (p_buf != NULL) {
            p_buf->event = BTA_DM_SEARCH_DISC_RES_EVT;
            bta_sys_sendmsg(p_buf);
        }
    }
    inq_cmpl[0] = p_data->inq_cmpl.num;
    bta_dm_search_cb.p_search_cback(BTA_DM_INQ_CMPL_EVT, inq_cmpl);
}

/* Remote-name request (no services): walk the inquiry DB reporting devices
   with known names; for the first unknown name start a remote-name request.
   If the database is exhausted, finalize with a DISC_RESULT message, then
   report the result of this message's device to the app. */
void bta_dm_rmt_name(struct bta_dm_msg *p_data) {
    unsigned char found = 1;
    struct bta_dm_buf_t *p_buf;
    struct bta_dm_disc_res_local_t disc_res;
    unsigned char *p_rem_addr;

    while ((bta_dm_search_cb.p_cur = BTM_InqDbNext(bta_dm_search_cb.p_cur)) != NULL) {
        if (((unsigned char *)bta_dm_search_cb.p_cur)[0x10] != 0) {
            bdcpy(disc_res.bd_addr, (unsigned char *)bta_dm_search_cb.p_cur + 2);
            disc_res.result = 0;
            disc_res.services = 0;
            bta_dm_search_cb.p_search_cback(BTA_DM_DISC_RES_EVT, &disc_res);
        } else {
            if (btm_cb.page_timeout < 0x7530) {
                bta_sys_start_timer(&bta_dm_search_cb.svc_timer, 0x205,
                                    btm_cb.page_timeout + 0x64);
            }
            if (BTM_ReadRemoteDeviceName((unsigned char *)bta_dm_search_cb.p_cur + 2,
                                         (void *)bta_dm_remname_cback) != 1) {
                bta_sys_stop_timer(&bta_dm_search_cb.svc_timer);
                p_rem_addr = (unsigned char *)bta_dm_search_cb.p_cur + 2;
                p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
                if (p_buf != NULL) {
                    bdcpy(p_buf->data + 4, p_rem_addr);
                    p_buf->data[0xa] = 0;
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
    unsigned char *p_name;
    void *p_rec;
    unsigned short uuid;
    unsigned short status;

    found = 0;
    status = p_data->sdp_result.sdp_status;
    p_rec = NULL;

    if (status == 0 || status == 0xfff0 || status == 0xfff4) {
        /* successful SDP transaction for the current service */
        uuid = ((unsigned short *)bta_service_id_to_uuid_lkup_tbl)[bta_dm_search_cb.services_index - 1];
        if (status != 0xfff4) {
            p_rec = SDP_FindServiceInDb(bta_dm_search_cb.p_sdp_db, uuid, NULL);
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
            ((struct bta_dm_buf_t *)p_msg)->event = BTA_DM_SEARCH_CMPL_EVT;
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
            ((struct bta_dm_buf_t *)p_msg)->event = BTA_DM_SEARCH_CMPL_EVT;
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

/* Search discovery timer expired: trace, clear the discovery-active flag
   and continue with the next device in the discovery queue. */

/* Free the SDP database buffer if it exists */
void bta_dm_free_sdp_db() {
    if (bta_dm_search_cb.p_sdp_db != NULL) {
        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;
    }
}

/* Queue an API_SEARCH message (0x1c bytes) for later dispatch. */
void bta_dm_queue_search(struct bta_dm_msg *p_data) {
    bta_dm_search_cb.p_search_queue = GKI_getbuf(0x1c);
    memcpy(bta_dm_search_cb.p_search_queue, p_data, 0x1c);
}

/* Queue an API_DISCOVER message (0x18 bytes) for later dispatch. */
void bta_dm_queue_disc(struct bta_dm_msg *p_data) {
    bta_dm_search_cb.p_search_queue = GKI_getbuf(0x18);
    memcpy(bta_dm_search_cb.p_search_queue, p_data, 0x18);
}

/* Free the search queue buffer if it exists */
void bta_dm_search_clear_queue() {
    if (bta_dm_search_cb.p_search_queue != NULL) {
        GKI_freebuf(bta_dm_search_cb.p_search_queue);
        bta_dm_search_cb.p_search_queue = NULL;
    }
}

/* Send the queued search cancel message and clear the queue pointer */
void bta_dm_search_cancel_cmpl() {
    if (bta_dm_search_cb.p_search_queue != NULL) {
        bta_sys_sendmsg(bta_dm_search_cb.p_search_queue);
        bta_dm_search_cb.p_search_queue = NULL;
    }
}

/* Search cancel transaction complete: free the SDP DB, then report
   BTA_DM_SEARCH_CANCEL_CMPL_EVT to the app. */
void bta_dm_search_cancel_transac_cmpl(struct bta_dm_msg *p_data) {
    if (bta_dm_search_cb.p_sdp_db != NULL) {
        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;
    }
    bta_dm_search_cb.p_search_cback(BTA_DM_SEARCH_CANCEL_CMPL_EVT, NULL);
}

/* Target 2: dispatches BTA_DM_SEARCH_CANCEL_CMPL_EVT (4) with NULL data to the search callback */
void bta_dm_search_cancel_notify(struct bta_dm_msg *p_data) {
    bta_dm_search_cb.p_search_cback(4, NULL);
}

/* Kept non-inlinable: -ipa file would otherwise inline the body into
   callers (discover, discover_next_device, inq_cmpl, sdp_result). */
#pragma auto_inline off
/* Walk the service list, starting at services_index, and start an SDP
   service search for each service still marked in services_cached. When a
   search fails, stop the walk; when the list is exhausted, report a
   discovery-complete message for the peer device. */
void bta_dm_find_services(bd_addr_t bd_addr) {
    unsigned char *p_name;
    unsigned short attr_list[2] = {0x0001, 0x8001};
    struct bta_sdp_uuid_t uuid;
    struct bta_dm_disc_msg_t *p_msg;
    unsigned char num_attrs;

    num_attrs = 1;

    while (bta_dm_search_cb.services_index < 0x17) {
        if ((bta_dm_search_cb.services_cached &
             (1u << bta_dm_search_cb.services_index)) != 0) {
            bta_dm_search_cb.p_sdp_db = GKI_getbuf(0xfa);
            if (bta_dm_search_cb.p_sdp_db != NULL) {
                bta_dm_search_cb.services_cached &=
                    ~(1u << bta_dm_search_cb.services_index);
                uuid.len = 2;
                uuid.uu.uuid16 = ((unsigned short *)bta_service_id_to_uuid_lkup_tbl)[bta_dm_search_cb.services_index];
                if (uuid.uu.uuid16 == 0x1200) {
                    num_attrs = 2;
                }
                SDP_InitDiscoveryDb(bta_dm_search_cb.p_sdp_db, 0xfa, 1, &uuid,
                                    num_attrs, attr_list);
                if (SDP_ServiceSearchAttributeRequest(
                        bd_addr, bta_dm_search_cb.p_sdp_db,
                        (void *)bta_dm_sdp_callback) == 0) {
                    GKI_freebuf(bta_dm_search_cb.p_sdp_db);
                    bta_dm_search_cb.p_sdp_db = NULL;
                    bta_dm_search_cb.services_index = 0x17;
                } else {
                    bta_dm_search_cb.services_index++;
                    return;
                }
            }
        }
        bta_dm_search_cb.services_index++;
    }

    /* all services done: report a discovery-complete message */
    if (bta_dm_search_cb.services_index >= 0x17) {
        p_msg = (struct bta_dm_disc_msg_t *)GKI_getbuf(0x110);
        if (p_msg != NULL) {
        ((struct bta_dm_buf_t *)p_msg)->event = BTA_DM_SEARCH_CMPL_EVT;
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
#pragma auto_inline on

/* Kept non-inlinable: -ipa file would otherwise inline the empty body and
   drop the call site from callers (search_timer_cback, search_result). */
/* Advance to the next inquiry-db device and start its service search;
   when the database is exhausted, report a discovery result to the
   search state machine. */
#pragma auto_inline off
void bta_dm_discover_next_device(void) {
    struct bta_dm_buf_t *p_buf;

    bta_dm_search_cb.p_cur = BTM_InqDbNext(bta_dm_search_cb.p_cur);
    if (bta_dm_search_cb.p_cur != NULL) {
        if (BTM_IsAclConnectionUp((unsigned char *)bta_dm_search_cb.p_cur + 2) != 0) {
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
              (unsigned char *)bta_dm_search_cb.p_cur + 2);
        bta_dm_find_services((unsigned char *)bta_dm_search_cb.p_cur + 2);
    } else {
        bta_dm_search_cb.services = 0;
        p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
        if (p_buf != NULL) {
            p_buf->event = BTA_DM_SEARCH_DISC_RES_EVT;
            bta_sys_sendmsg(p_buf);
        }
    }
}
#pragma auto_inline on

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

/* Inquiry result callback: copy the address/class/rssi into a search result
   and report BTA_DM_INQ_RES_EVT to the app. Marks the inquiry DB entry as
   known-name when the record exists. */
void bta_dm_inq_results_cb(void *p_inq_results, void *p_eir) {
    struct bta_btm_inq_results_t *p_inq = (struct bta_btm_inq_results_t *)p_inq_results;
    struct bta_dm_search_res_t results;
    void *p_inq_info;

    bdcpy(results.inq_res.bd_addr, p_inq->remote_bd_addr);
    memcpy(results.inq_res.dev_class, p_inq->dev_class, 3);
    results.inq_res.rssi = p_inq->rssi;
    p_inq_info = BTM_InqDbRead(p_inq->remote_bd_addr);
    if (p_inq_info != NULL) {
        results.inq_res.device_type = 0;
    }
    bta_dm_search_cb.p_search_cback(BTA_DM_INQ_RES_EVT, &results);
    if (p_inq_info != NULL) {
        ((unsigned char *)p_inq_info)[0x10] = results.inq_res.device_type;
    }
}

/* Inquiry complete callback: forwards the number of responses to the search
   state machine as a INQUIRY_CMPL message (0x203). */
void bta_dm_inq_cmpl_cb(struct bta_btm_inq_cmpl_t *p_results) {
    struct bta_dm_msg *p_msg;

    p_msg = (struct bta_dm_msg *)GKI_getbuf(0x110);
    if (p_msg != NULL) {
        p_msg->inq_cmpl.hdr.event = BTA_DM_SEARCH_INQ_CMPL_EVT;
        p_msg->inq_cmpl.num = p_results->num_resp;
        bta_sys_sendmsg(p_msg);
    }
}

/* Target 5: copies the remote device name into the search CB peer_name field,
   then unregisters itself as the remote name notify callback (one-shot). */
void bta_dm_service_search_remname_cback(unsigned char *bd_addr, unsigned char *dc, unsigned char *bd_name) {
    strncpy(bta_dm_search_cb.peer_name, (const char *)bd_name, 0x1f);
    BTM_SecDeleteRmtNameNotifyCallback(&bta_dm_service_search_remname_cback);
}

/* Remote name request complete during a service search: stop the service
   search timer and forward the result to the app as a REMT_NAME message. */
void bta_dm_remname_cback(void *p_data) {
    struct bta_dm_buf_t *p_buf;

    bta_sys_stop_timer(&bta_dm_search_cb.svc_timer);
    p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
    if (p_buf != NULL) {
        bdcpy(p_buf->data + 4, (unsigned char *)bta_dm_search_cb.p_cur + 2);
        strncpy((char *)p_buf->data + 0xa, (const char *)p_data + 4, 0x20);
        p_buf->event = BTA_DM_REMT_NAME_EVT;
        bta_sys_sendmsg(p_buf);
    }
}

/* Remote name request complete during a discovery: same as above but the
   address comes from the discovery control block. */
void bta_dm_disc_remname_cback(void *p_data) {
    struct bta_dm_buf_t *p_buf;

    bta_sys_stop_timer(&bta_dm_search_cb.svc_timer);
    p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
    if (p_buf != NULL) {
        bdcpy(p_buf->data + 4, bta_dm_search_cb.disc_bd_addr);
        strncpy((char *)p_buf->data + 0xa, (const char *)p_data + 4, 0x20);
        p_buf->event = BTA_DM_REMT_NAME_EVT;
        bta_sys_sendmsg(p_buf);
    }
}

void bta_dm_cancel_rmt_name(void) { BTM_CancelRemoteDeviceName(); }

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

/* Target: us-802e2ce4 (0xc0): PIN request remote-name callback. Builds a
   tBTA_DM_PIN_REQ from the stored pin bd addr/class and the name buffer. */
void bta_dm_pinname_cback(void *p_data) {
    struct bta_dm_pin_req_t pin_req;
    struct bta_dm_buf_t *p_buf = (struct bta_dm_buf_t *)p_data;

    bdcpy(pin_req.bd_addr, bta_dm_cb.pin_bd_addr);
    pin_req.dev_class[0] = bta_dm_cb.pin_dev_class[0];
    pin_req.dev_class[1] = bta_dm_cb.pin_dev_class[1];
    pin_req.dev_class[2] = bta_dm_cb.pin_dev_class[2];

    if (p_buf != NULL && p_buf->event == 0) {
        memcpy(pin_req.bd_name, p_buf->data, (p_buf->len < 0x20) ? p_buf->len : 0x20);
        pin_req.bd_name[0x20] = 0;
    } else {
        pin_req.bd_name[0] = 0;
    }

    bta_dm_cb.cback(2, &pin_req);
}

/* tBTA_DM_SEC-style union: pin/auth/authorize report payloads share the
   largest member (0x118) so the stack frame matches the retail build. */
struct bta_dm_acl_change_t {
    bd_addr_t bd_addr;      /* offset 0x00 */
    unsigned char reason;   /* offset 0x06 */
};

union bta_dm_sec_t {
    struct bta_dm_pin_req_t pin_req;
    struct bta_dm_auth_cmpl_t auth_cmpl;
    struct bta_dm_authorize_t authorize;
    struct bta_dm_sig_strength_t sig_strength;
    struct bta_dm_acl_change_t acl_change;
};

void bta_dm_compress_cback(unsigned char action, unsigned char server_id,
                           unsigned char client_id, bd_addr_t bd_addr) {
    unsigned char *p_cfg;
    struct bta_dm_compress_srvc_t *p_srvc;
    unsigned char j;
    unsigned char k;
    unsigned char found;
    unsigned char i;
    unsigned char num;

    if (action == 0) {
        p_cfg = p_bta_dm_compress_cfg;
        num = p_cfg[1];
        for (i = 1; i <= num; i++) {
            if (client_id == p_cfg[3 * i + 1] || p_cfg[3 * i + 1] == 0xff) {
                if (server_id == p_cfg[3 * i] && p_cfg[3 * i + 2] == 1) {
                    /* if any state-2 server is already in the connected list, skip */
                    found = 0;
                    for (j = 1; j <= p_cfg[1]; j++) {
                        if (p_cfg[3 * j + 2] == 2) {
                            for (k = 0; k < bta_dm_conn_srvcs[0]; k++) {
                                if (bta_dm_conn_srvcs[9 * k + 7] == p_cfg[3 * j])
                                    found = 1;
                            }
                        }
                    }
                    if (!found) {
                        bta_dm_compress_srvcs[i - 1].client_id = client_id;
                        bta_dm_compress_srvcs[i - 1].server_id = server_id;
                        bdcpy(bta_dm_compress_srvcs[i - 1].bd_addr, bd_addr);
                        bta_dm_compress_srvcs[i - 1].in_use = 1;
                        if (appl_trace_level >= 4)
                            LogMsg_3(0x503,
                                     "bta_dm_compress_cback open app_id %d, BTA id %d, state %d",
                                     bta_dm_compress_srvcs[i - 1].client_id,
                                     bta_dm_compress_srvcs[i - 1].server_id,
                                     bta_dm_compress_srvcs[i - 1].in_use);
                    }
                    break;
                }
            }
        }
    } else if (action == 1) {
        p_cfg = p_bta_dm_compress_cfg;
        for (i = 1; i <= p_cfg[1]; i++) {
            if (client_id == p_cfg[3 * i + 1] || p_cfg[3 * i + 1] == 0xff) {
                if (server_id == p_cfg[3 * i]) {
                    p_srvc = &bta_dm_compress_srvcs[i - 1];
                    p_srvc->in_use = 0;
                    if (appl_trace_level >= 4)
                        LogMsg_3(0x503,
                                 "bta_dm_compress_cback close app_id %d, BTA id %d, state %d\x00\x00\x00\x00\x00",
                                 p_srvc->client_id, p_srvc->server_id, p_srvc->in_use);
                    break;
                }
            }
        }
    }
}

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
        if (BTM_ReadRemoteDeviceName(bd_addr, (void *)bta_dm_pinname_cback) == 1) {
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

void bta_dm_search_timer_cback(struct bta_dm_timer_t *p_tle) {
    if (appl_trace_level >= 4)
        LogMsg_0(0x503, " bta_dm_search_timer_cback  ");
    bta_dm_search_cb.search_disc_active = 0;
    bta_dm_discover_next_device();
}

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



/* PIN request callback: if the remote name is not yet known, kick off a
   remote name request and report the PIN request once the name arrives;
   otherwise report it immediately. */

int bta_dm_link_key_request_cback(void) { return 0xb; }

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

/* Target: us-802e2f78 (0x84): authentication complete. Reports
   BTA_DM_AUTH_CMPL_EVT on success (no key present). */
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

void bta_dm_local_addr_cback(void *addr) {
    if (bta_dm_cb.cback != NULL) {
        bta_dm_cb.cback(0, addr);
    }
}

/* Target 4: starts or stops signal strength monitoring.
   When start=1, stores the mask/period and tail-calls the timer callback
   to trigger the first reading. When start=0, stops the ongoing timer. */
void bta_dm_signal_strength(struct bta_dm_msg *p_data) {
    if (p_data->sig_strength.start) {
        bta_dm_cb.signal_strength_mask = p_data->sig_strength.mask;
        bta_dm_cb.signal_strength_period = p_data->sig_strength.period;
        bta_dm_signal_strength_timer_cback(NULL);
    } else {
        bta_sys_stop_timer(&bta_dm_cb.signal_strength_timer);
    }
}

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

/* ACL database change callback: forward a BTA_DM_ACL_CHANGE_EVT message
   with the changed address and add/remove flag. */
void bta_dm_acl_change_cback(bd_addr_t bd_addr, unsigned char *p_dc, unsigned char *p_bdn,
                             unsigned char *p_features, unsigned char is_new) {
    struct bta_dm_buf_t *p_buf;

    p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x12);
    if (p_buf != NULL) {
        bdcpy(p_buf->data + 7, bd_addr);
        p_buf->data[6] = is_new;
        p_buf->event = BTA_DM_ACL_CHANGE_EVT;
        bta_sys_sendmsg(p_buf);
    }
}

/* ACL change event: track peer devices in bta_dm_cb and report the change
   to the application. New devices are appended to the peer list; dropped
   devices are removed (shifting the list down). While a discovery is active
   for the dropped device, restart it; when the last ACL link drops while a
   disable is pending, complete the disable. */

void bta_dm_disable_conn_down_timer_cback() {
    bta_dm_cb.cback(1, 0);
}

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

/* L2CAP server compression callback: locate the registered compression
   service for the peer address and ask the callout for compression memory.
   The connection engine invokes this with (bd_addr, ids, buffers,
   pp_memory, p_memory_size); only the address and the two memory output
   pointers are used here. */

/* Target: us-802e361c (0x238): compress server/client registration callback.
   action 0 = open (add pair to bta_dm_compress_srvcs), 1 = close (remove). */

/* Role-management connection callback (BTA_SYS_CONN_OPEN): mark the peer
   device in use and raise its power policy to the best matching entry in
   the role-management config table. */
void bta_dm_rm_cback(int status, unsigned char id, unsigned char app_id,
                     bd_addr_t peer_addr) {
    unsigned char *p_cfg;
    unsigned char i;
    unsigned char j;
    if (status == 0) {
        for (i = 0; i < bta_dm_cb.num_devices; i++) {
            if (bdcmp(bta_dm_cb.peer_dev[i].bd_addr, peer_addr) == 0) {
                bta_dm_cb.peer_dev[i].in_use = 1;
                p_cfg = p_bta_dm_rm_cfg;
                for (j = 1; j <= p_cfg[1]; j++) {
                    if (p_cfg[3 * j + 1] == app_id || p_cfg[3 * j + 1] == 0xff) {
                        if (p_cfg[3 * j] == id) {
                            unsigned char cfg_val = *(p_cfg + 3 * j + 2);
                            if (cfg_val > bta_dm_cb.peer_dev[i].policy) {
                                bta_dm_cb.peer_dev[i].policy = cfg_val;
                            }
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}

/* Keep the L2CAP idle timeout disabled while a connection is open. */
void bta_dm_keep_acl(struct bta_dm_msg *p_data) {
    unsigned short keep;

    keep = p_data->hdr.layer_specific;
    bta_dm_cb.keep_acl = (unsigned char)keep;
    if ((unsigned char)keep != 0) {
        L2CA_SetIdleTimeoutByBdAddr(BT_BD_ANY, 0xffff);
        L2CA_SetIdleTimeout(0, 0xffff, 1);
    } else {
        L2CA_SetIdleTimeoutByBdAddr(BT_BD_ANY, 2);
        L2CA_SetIdleTimeout(0, 2, 1);
    }
}

/* Target 3: dispatches BTA_DM_DISABLE_EVT (1) with NULL data via the DM callback */
void bta_dm_immediate_disable(void) {
    bta_dm_cb.cback(1, NULL);
}

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

/* DM security callbacks table (retail rodata, registered via BTM_SecRegister) */
const struct bta_security_t bta_security = {
    (void *)bta_dm_authorize_cback,
    (void *)bta_dm_pin_cback,
    (void *)bta_dm_new_link_key_cback,
    (void *)bta_dm_link_key_request_cback,
    (void *)bta_dm_authentication_complete_cback,
    NULL,
};
