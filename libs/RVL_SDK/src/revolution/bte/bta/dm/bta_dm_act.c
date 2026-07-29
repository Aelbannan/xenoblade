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
    unsigned char _pad4[0x10];           /* offset 0x04-0x13 */
    void *p_sdp_db;                     /* offset 0x14 */
    unsigned char _pad18[0x08];         /* offset 0x18-0x1f */
    char peer_name[0x20];               /* offset 0x20 */
    unsigned char _pad40[0x34];         /* offset 0x40-0x73 */
    void *p_search_queue;               /* offset 0x74 */
};

extern struct bta_dm_search_cb_t bta_dm_search_cb;

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
    unsigned char _pad0[0x50];
    bta_dm_cback_t cback;                          /* offset 0x50 */
    struct bta_dm_timer_t signal_strength_timer;   /* offset 0x54, size 0x18 */
    unsigned char signal_strength_mask;            /* offset 0x6c */
    unsigned char _pad6d[3];
    unsigned short signal_strength_period;         /* offset 0x70 */
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
extern void GKI_freebuf(void *p_buf);
extern unsigned char bta_sys_sendmsg(void *p_msg);
extern void BTM_SetDiscoverability(unsigned char mode, unsigned char, unsigned char);
extern void BTM_SetConnectability(unsigned char mode, unsigned char, unsigned char);
extern int BTM_SecDeleteRmtNameNotifyCallback(bta_dm_rmt_name_cback_t p_callback);
void bta_dm_signal_strength_timer_cback(struct bta_dm_timer_t *p_tle);

/* --- Function implementations --- */

void bta_dm_enable() {}

void bta_dm_disable() {}

void bta_dm_disable_timer_cback() {}

extern void BTM_SetLocalDeviceName(void*);
void bta_dm_set_dev_name(void* self) { ((void(*)(void*))BTM_SetLocalDeviceName)((char*)self + 0x8); }

/* Set discoverability and connectability from the incoming message */
void bta_dm_set_visibility(void *p_data) {
    unsigned char *msg = (unsigned char *)p_data;
    BTM_SetDiscoverability(msg[0x8], 0, 0);
    BTM_SetConnectability(msg[0x9], 0, 0);
}

void bta_dm_bond() {}

void bta_dm_pin_reply() {}

void bta_dm_auth_reply() {}

void bta_dm_search_start() {}

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

void bta_dm_disc_result() {}

void bta_dm_search_result() {}

void bta_dm_search_timer_cback() {}

/* Free the SDP database buffer if it exists */
void bta_dm_free_sdp_db() {
    if (bta_dm_search_cb.p_sdp_db != NULL) {
        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;
    }
}

void bta_dm_queue_search() {}

void bta_dm_queue_disc() {}

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

void bta_dm_search_cancel_transac_cmpl() {}

/* Target 2: dispatches BTA_DM_SEARCH_CANCEL_CMPL_EVT (4) with NULL data to the search callback */
void bta_dm_search_cancel_notify(struct bta_dm_msg *p_data) {
    bta_dm_search_cb.p_search_cback(4, NULL);
}

void bta_dm_find_services() {}

void bta_dm_discover_next_device() {}

void bta_dm_sdp_callback() {}

void bta_dm_inq_results_cb() {}

void bta_dm_inq_cmpl_cb() {}

/* Target 5: copies the remote device name into the search CB peer_name field,
   then unregisters itself as the remote name notify callback (one-shot). */
void bta_dm_service_search_remname_cback(unsigned char *bd_addr, unsigned char *dc, unsigned char *bd_name) {
    strncpy(bta_dm_search_cb.peer_name, (const char *)bd_name, 0x1f);
    BTM_SecDeleteRmtNameNotifyCallback(&bta_dm_service_search_remname_cback);
}

void bta_dm_remname_cback() {}

void bta_dm_disc_remname_cback() {}

void bta_dm_cancel_rmt_name(void) { BTM_CancelRemoteDeviceName(); }

void bta_dm_authorize_cback() {}

void bta_dm_pinname_cback() {}

void bta_dm_pin_cback() {}

int bta_dm_link_key_request_cback(void) { return 0xb; }

void bta_dm_new_link_key_cback() {}

void bta_dm_authentication_complete_cback() {}

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

void bta_dm_rssi_cback() {}

void bta_dm_link_quality_cback() {}

void bta_dm_l2cap_server_compress_cback() {}

void bta_dm_compress_cback() {}

void bta_dm_rm_cback() {}

void bta_dm_keep_acl() {}

/* Target 3: dispatches BTA_DM_DISABLE_EVT (1) with NULL data via the DM callback */
void bta_dm_immediate_disable(void) {
    bta_dm_cb.cback(1, NULL);
}

void bta_dm_reset_complete(void) {}

void bta_dm_send_hci_reset() {}