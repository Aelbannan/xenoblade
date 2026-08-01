// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

/* --- Type definitions and extern declarations --- */

/* Forward declarations for struct types used to avoid void* */
struct bta_dm_msg;
struct bta_dm_timer_t;

/* Timer callback type: takes a timer list entry pointer */
typedef void (*bta_dm_timer_cback_t)(struct bta_dm_timer_t *);

/* Remote name notify callback type: (bd_addr, dev_class, bd_name) */
typedef void (*bta_dm_rmt_name_cback_t)(unsigned char *, unsigned char *, unsigned char *);

/* Search control block function pointer type */
/* callback: (tBTA_DM_SEARCH_EVT event, tBTA_DM_SEARCH *p_data) */
typedef void (*bta_dm_search_cback_t)(int, void *);

struct bta_dm_search_cb_t {
    bta_dm_search_cback_t p_search_cback; /* offset 0x00 */
    unsigned char _pad4[0x4];           /* offset 0x04-0x07 */
    unsigned int services;              /* offset 0x08 (tBTA_SERVICE_MASK) */
    unsigned char _padC[0x8];           /* offset 0x0c-0x13 */
    void *p_sdp_db;                     /* offset 0x14 */
    unsigned char _pad18[0x08];         /* offset 0x18-0x1f */
    char peer_name[0x20];               /* offset 0x20 */
    unsigned char _pad40[0x34];         /* offset 0x40-0x73 */
    void *p_search_queue;               /* offset 0x74 */
    unsigned char search_disc_active;   /* offset 0x78 */
    unsigned char _pad79[0x3];          /* offset 0x79-0x7b */
};

/* tBTA_DM_API_SEARCH message: BT_HDR(8) + inq_params(10) + services + p_cback.
   Built by BTA_DmSearch() in bta_dm_api.c (msg size 0x1c). */
struct bta_dm_search_msg_t {
    unsigned char hdr[8];               /* offset 0x00 (BT_HDR) */
    unsigned char inq_params[0xA];      /* offset 0x08 (tBTA_DM_INQ, partial) */
    unsigned char _pad12[2];            /* offset 0x12-0x13 */
    unsigned int services;              /* offset 0x14 */
    void *p_cback;                      /* offset 0x18 */
};

/* Inquiry complete message (BTA_DM_INQUIRY_CMPL_EVT = 0x203) */
struct bta_dm_inq_cmpl_msg_t {
    unsigned short event;               /* offset 0x00 */
    unsigned char _hdr[6];              /* offset 0x02-0x07 */
    unsigned char num;                  /* offset 0x08 */
};

/* SDP result message (BTA_DM_SDP_RESULT_EVT = 0x206) */
struct bta_dm_sdp_result_msg_t {
    unsigned short event;               /* offset 0x00 */
    unsigned char _hdr[6];              /* offset 0x02-0x07 */
    unsigned short sdp_status;          /* offset 0x08 */
};

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

/* Search callback events (bta_api.h) */
#define BTA_DM_DISC_RES_EVT             2
#define BTA_DM_DISC_CMPL_EVT            3
#define BTA_DM_SEARCH_CANCEL_CMPL_EVT   4

/* BD address (BD_ADDR from bt_types.h) */
typedef unsigned char bd_addr_t[6];

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

extern struct bta_dm_search_cb_t bta_dm_search_cb;

/* bta_sys_cb (BSS global, 0x8c bytes) - byte access like bta_sys_main.c */
extern unsigned char bta_sys_cb[];

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

typedef void (*bta_dm_cback_t)(int, void *);

struct bta_dm_cb_t {
    unsigned char _pad0[0x4d];          /* offset 0x00-0x4c (peer device list) */
    unsigned char num_devices;          /* offset 0x4d */
    unsigned char _pad4e[2];            /* offset 0x4e-0x4f */
    bta_dm_cback_t cback;                          /* offset 0x50 */
    struct bta_dm_timer_t signal_strength_timer;   /* offset 0x54, size 0x18 */
    unsigned char signal_strength_mask;            /* offset 0x6c */
    unsigned char _pad6d[3];
    unsigned short signal_strength_period;         /* offset 0x70 */
    unsigned char _pad72[0x86];         /* offset 0x72-0xf7 */
    bd_addr_t pin_bd_addr;              /* offset 0xf8 */
    unsigned char pin_dev_class[3];     /* offset 0xfe */
    unsigned char _pad101[3];           /* offset 0x101-0x103 */
};

extern struct bta_dm_cb_t bta_dm_cb;

/* Signal strength data struct layout (tBTA_API_DM_SIG_STRENGTH) */
struct bta_dm_sig_strength_data_t {
    unsigned char _hdr[8];  /* BT_HDR */
    unsigned char mask;     /* offset 0x08 */
    unsigned char _pad9;    /* offset 0x09 */
    unsigned short period;  /* offset 0x0a */
    unsigned char start;    /* offset 0x0c */
    unsigned char _padd;    /* offset 0x0d */
};

extern void bta_sys_stop_timer(struct bta_dm_timer_t *p_tle);
extern unsigned char bta_sys_sendmsg(void *p_msg);
extern void GKI_freebuf(void *p_buf);
extern void *GKI_getbuf(unsigned short size);
extern void *GKI_getpoolbuf(unsigned char pool_id);
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
void bta_dm_inq_results_cb();
void bta_dm_inq_cmpl_cb(struct bta_btm_inq_cmpl_t *p_results);
void bta_dm_discover_next_device();
void bta_dm_reset_complete();

/* --- BTE helper externs (bd.h / bta_sys.h / bte_appl.h) --- */
extern void bdcpy(bd_addr_t a, const bd_addr_t b);
extern void *memcpy(void *p_dst, const void *p_src, unsigned long n);
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

struct bta_dm_compress_srvc_t bta_dm_compress_srvcs[5];

/* Connected services table: byte 0 = count, 9-byte entries, service id at +7 */
unsigned char bta_dm_conn_srvcs[0x2e];

extern unsigned char *p_bta_dm_compress_cfg;

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

/* --- Function implementations --- */

void bta_dm_enable() {}

void bta_dm_disable() {}

void bta_dm_disable_timer_cback() {}

extern void BTM_SetLocalDeviceName(void*);
void bta_dm_set_dev_name(void* self) { ((void(*)(void*))BTM_SetLocalDeviceName)((char*)self + 0x8); }

/* Set discoverability and connectability from the incoming message */
void bta_dm_set_visibility(struct bta_dm_msg *p_data) {
    unsigned char *msg = (unsigned char *)p_data;
    BTM_SetDiscoverability(msg[0x8], 0, 0);
    BTM_SetConnectability(msg[0x9], 0, 0);
}

void bta_dm_bond() {}

void bta_dm_pin_reply() {}

void bta_dm_auth_reply() {}

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
    BTM_StartInquiry(p_search->inq_params, p_results_cb, p_cmpl_cb);
}

void bta_dm_search_cancel() {}

void bta_dm_discover() {}

void bta_dm_inq_cmpl() {}

void bta_dm_rmt_name() {}

void bta_dm_disc_rmt_name() {}

void bta_dm_sdp_result() {}

/* Target 1: dispatches BTA_DM_DISC_CMPL_EVT (3) with NULL data to the search callback */
void bta_dm_search_cmpl(struct bta_dm_msg *p_data) {
    bta_dm_search_cb.p_search_cback(3, NULL);
}

/* Discovery result: reports BTA_DM_DISC_RES_EVT to the app, then sends a
   queued DISC_RESULT message (0x207) to the search state machine. */
void bta_dm_disc_result(struct bta_dm_msg *p_data) {
    struct bta_dm_buf_t *p_buf;

    bta_dm_search_cb.p_search_cback(BTA_DM_DISC_RES_EVT, (void *)((unsigned char *)p_data + 0x8));
    p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
    if (p_buf != NULL) {
        p_buf->event = BTA_DM_SEARCH_DISC_RES_EVT;
        bta_sys_sendmsg(p_buf);
    }
}

void bta_dm_search_result() {}

/* Search discovery timer expired: trace, clear the discovery-active flag
   and continue with the next device in the discovery queue. */
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

void bta_dm_find_services() {}

/* Kept non-inlinable: -ipa file would otherwise inline the empty body and
   drop the call site from callers (search_timer_cback, search_result). */
#pragma auto_inline off
void bta_dm_discover_next_device(struct bta_dm_msg *p_data) {}
#pragma auto_inline on

/* SDP discovery complete callback: forwards the status to the search state
   machine as a SDP_RESULT message (0x206). */
void bta_dm_sdp_callback(unsigned short status) {
    struct bta_dm_sdp_result_msg_t *p_msg;

    p_msg = (struct bta_dm_sdp_result_msg_t *)GKI_getbuf(0xA);
    if (p_msg != NULL) {
        p_msg->event = BTA_DM_SEARCH_SDP_RES_EVT;
        p_msg->sdp_status = status;
        bta_sys_sendmsg(p_msg);
    }
}

void bta_dm_inq_results_cb() {}

/* Inquiry complete callback: forwards the number of responses to the search
   state machine as a INQUIRY_CMPL message (0x203). */
void bta_dm_inq_cmpl_cb(struct bta_btm_inq_cmpl_t *p_results) {
    struct bta_dm_inq_cmpl_msg_t *p_msg;

    p_msg = (struct bta_dm_inq_cmpl_msg_t *)GKI_getbuf(0x110);
    if (p_msg != NULL) {
        p_msg->event = BTA_DM_SEARCH_INQ_CMPL_EVT;
        p_msg->num = p_results->num_resp;
        bta_sys_sendmsg(p_msg);
    }
}

/* Target 5: copies the remote device name into the search CB peer_name field,
   then unregisters itself as the remote name notify callback (one-shot). */
void bta_dm_service_search_remname_cback(unsigned char *bd_addr, unsigned char *dc, unsigned char *bd_name) {
    strncpy(bta_dm_search_cb.peer_name, (const char *)bd_name, 0x1f);
    BTM_SecDeleteRmtNameNotifyCallback(&bta_dm_service_search_remname_cback);
}

void bta_dm_remname_cback() {}

void bta_dm_disc_remname_cback() {}

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

void bta_dm_pin_cback() {}

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
    struct bta_dm_sig_strength_data_t *d = (struct bta_dm_sig_strength_data_t *)p_data;
    if (d->start) {
        bta_dm_cb.signal_strength_mask = d->mask;
        bta_dm_cb.signal_strength_period = d->period;
        bta_dm_signal_strength_timer_cback(NULL);
    } else {
        bta_sys_stop_timer(&bta_dm_cb.signal_strength_timer);
    }
}

void bta_dm_signal_strength_timer_cback(struct bta_dm_timer_t *p_tle) {
    /* The real implementation reads signal_strength_mask from bta_dm_cb,
       iterates bits calling BTM_ReadRSSI / BTM_ReadLinkQuality,
       and restarts the timer with bta_sys_start_timer if period > 0.
       p_tle is unused (reads timer from bta_dm_cb directly). */
    bta_sys_stop_timer(p_tle);
}

void bta_dm_acl_change_cback() {}

void bta_dm_acl_change() {}

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

void bta_dm_l2cap_server_compress_cback() {}

/* Target: us-802e361c (0x238): compress server/client registration callback.
   action 0 = open (add pair to bta_dm_compress_srvcs), 1 = close (remove). */
void bta_dm_compress_cback(unsigned char action, unsigned char server_id,
                           unsigned char client_id, bd_addr_t bd_addr) {
    unsigned char *p_cfg;
    unsigned char num;
    unsigned char i;
    unsigned char j;
    unsigned char k;
    unsigned char num_conn;
    unsigned char found;
    struct bta_dm_compress_srvc_t *p_srvc;
    unsigned char *p_entry;

    if (action == 0) {
        p_cfg = p_bta_dm_compress_cfg;
        num = p_cfg[1];
        for (i = 1; i <= num; i++) {
            p_entry = p_cfg + 3 * i;
            if (p_entry[1] == client_id || p_entry[1] == 0xff) {
                if (p_entry[0] == server_id && p_entry[2] == 1) {
                    /* if any state-2 server is already in the connected list, skip */
                    found = 0;
                    num_conn = bta_dm_conn_srvcs[0];
                    for (j = 1; j <= p_cfg[1]; j++) {
                        if (p_cfg[3 * j + 2] == 2) {
                            for (k = 0; k < num_conn; k++) {
                                if (bta_dm_conn_srvcs[9 * k + 7] == p_cfg[3 * j])
                                    found = 1;
                            }
                        }
                    }
                    if (!found) {
                        p_srvc = &bta_dm_compress_srvcs[i - 1];
                        p_srvc->client_id = client_id;
                        p_srvc->server_id = server_id;
                        bdcpy(p_srvc->bd_addr, bd_addr);
                        p_srvc->in_use = 1;
                        if (appl_trace_level >= 4)
                            LogMsg_3(0x503,
                                     "bta_dm_compress_cback open app_id %d, BTA id %d, state %d",
                                     p_srvc->client_id, p_srvc->server_id, p_srvc->in_use);
                    }
                    break;
                }
            }
        }
    } else if (action == 1) {
        p_cfg = p_bta_dm_compress_cfg;
        num = p_cfg[1];
        for (i = 1; i <= num; i++) {
            p_entry = p_cfg + 3 * i;
            if (p_entry[1] == client_id || p_entry[1] == 0xff) {
                if (p_entry[0] == server_id) {
                    p_srvc = &bta_dm_compress_srvcs[i - 1];
                    p_srvc->in_use = 0;
                    if (appl_trace_level >= 4)
                        LogMsg_3(0x503,
                                 "bta_dm_compress_cback close app_id %d, BTA id %d, state %d",
                                 p_srvc->client_id, p_srvc->server_id, p_srvc->in_use);
                    break;
                }
            }
        }
    }
}

void bta_dm_rm_cback() {}

void bta_dm_keep_acl() {}

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