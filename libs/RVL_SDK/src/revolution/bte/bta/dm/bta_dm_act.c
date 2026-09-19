// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act
// Replace stubs with high-level C/C++ during decomp.
//
// !!! RECOVERY NOTICE (session pi-batch-match) !!!
// This file was accidentally truncated by an agent session. The regions below
// marked "LOST" must be restored via `git restore libs/RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act.c`
// (or from the last good commit / another agent's checkout).
// Recovered-verbatim regions from the session transcript are kept intact.

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
    /* ... remainder of this struct was in the LOST region ... */
};

/* Minimal discovery-result payload for the app search callback (fields used
   by the recovered functions below). Full tBTA_DM_DISC_RES lived in the
   LOST region; do not invent the unused members. */
struct bta_dm_disc_res_local_t {
    bd_addr_t bd_addr;
    unsigned int services;
    unsigned char result;
};

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
#ifndef BTA_DM_REMT_NAME_EVT
#define BTA_DM_REMT_NAME_EVT           0x205
#endif
#ifndef BTA_DM_SEARCH_DISC_RES_EVT
#define BTA_DM_SEARCH_DISC_RES_EVT     0x207
#endif
#ifndef BTA_DM_SEARCH_CMPL_EVT
#define BTA_DM_SEARCH_CMPL_EVT         0x206
#endif

/* Globals / helpers that lived in the LOST region — declare only what the
   currently present functions need so the TU can compile. Bodies for the
   LOST functions themselves are NOT invented here. */
extern struct bta_dm_search_cb_t bta_dm_search_cb;
extern struct { unsigned short page_timeout; } btm_cb;
extern const unsigned short bta_service_id_to_uuid_lkup_tbl[];
extern void bta_dm_remname_cback(unsigned char *, unsigned char *, unsigned char *);
extern void bta_dm_service_search_remname_cback(unsigned char *, unsigned char *, unsigned char *);
extern void bta_dm_search_timer_cback(struct bta_dm_timer_t *);
extern void bta_dm_find_services(unsigned char *bd_addr);
extern void bta_dm_discover_next_device(void);

/* Harness stubs: many BTE APIs are undeclared and default to int-returning
   K&R prototypes under MWCC; cast call results at use sites below. */
void *BTM_InqDbNext(void *);
void *GKI_getbuf(unsigned short);
void GKI_freebuf(void *);
void *SDP_FindServiceInDb(void *, unsigned short, void *);
void *SDP_FindAttributeInRec(void *, unsigned short);
void *BTM_SecReadDevName(unsigned char *);
int BTM_ReadRemoteDeviceName(unsigned char *, void *);
int BTM_SecDeleteRmtNameNotifyCallback(void *);
void bdcpy(unsigned char *, unsigned char *);
void bta_sys_start_timer(struct bta_dm_timer_t *, unsigned short, int);
void bta_sys_stop_timer(struct bta_dm_timer_t *);
void bta_sys_sendmsg(void *);
int strncpy(char *, const char *, unsigned long);

/* === LOST REGION: original lines ~85-699 ===
   Contents (per session reads of surrounding context): remainder of the local
   type definitions block and all functions up to bta_dm_rmt_name. Restore from
   git. Functions known to live here include (non-exhaustive): bta_dm_enable,
   bta_dm_disable, bta_dm_set_dev_class, bta_dm_set_name, bta_dm_search,
   bta_dm_search_cancel, bta_dm_discover, bta_dm_inq_cmpl (partial tail seen),
   bta_dm_service_search, bta_dm_search_timer_cback region comments, etc.
   === END LOST REGION === */

/* --- Verbatim-recovered functions (session transcript) --- */

/* Discovery remote-name transaction walk (recovered region begins mid-comment;
   preceding helper functions are in the LOST region above). If the database is
   exhausted, finalize with a DISC_RESULT message, then report the result of
   this message's device to the app. */
void bta_dm_rmt_name(struct bta_dm_msg *p_data) {
    unsigned char found = 1;
    struct bta_dm_disc_res_local_t disc_res;
    unsigned char *p_rem_addr;
    struct bta_dm_buf_t *p_buf;

    while ((bta_dm_search_cb.p_cur = (void *)BTM_InqDbNext(bta_dm_search_cb.p_cur)) != NULL) {
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
                {
                    p_rem_addr = (unsigned char *)bta_dm_search_cb.p_cur + 2;

                    p_buf = (struct bta_dm_buf_t *)GKI_getbuf(0x110);
                    if (p_buf != NULL) {
                        /* remote BD_ADDR copied into the message payload */
                        bdcpy(p_buf->data + 4, p_rem_addr);
                        p_buf->data[0xa] = 0;
                        p_buf->event = BTA_DM_REMT_NAME_EVT;
                        bta_sys_sendmsg(p_buf);
                    }
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
        bta_dm_discover_next_device();
    } else {
        bta_dm_search_cb.search_timer_active = 1;
        bta_dm_search_cb.search_timer.p_cback = bta_dm_search_timer_cback;
        bta_sys_start_timer(&bta_dm_search_cb.search_timer, 0, 0xbb8);
    }
}

/* Free the SDP database buffer if it exists */
void bta_dm_free_sdp_db() {
    if (bta_dm_search_cb.p_sdp_db != NULL) {
        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;
    }
}

/* === LOST REGION: original lines ~900-1608 ===
   All remaining functions of the unit (queue helpers, API handlers,
   bta_dm_find_services, bta_dm_inq_cmpl, bta_dm_acl_change,
   bta_dm_compress_cback, bta_dm_check_cback, timers, etc.). Restore from git.
   Note: bta_dm_find_services (us-802e2720) and bta_dm_acl_change
   (us-802e31c4) were FULL_MATCH; their exact source text is critical.
   === END LOST REGION === */

// --- restored from git history (base:gone); do not expand beyond these functions ---
// from commit 0183f60318bf needle=bta_dm_disable_conn_down_timer_cback

void bta_dm_disable_conn_down_timer_cback() {
    bta_dm_cb.cback(1, 0);
}

// from commit 0183f60318bf needle=bta_dm_sdp_callback
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

// from commit 365650b84230 needle=bta_dm_auth_reply
// LLM-HARNESS-END: us-802e1a50

// LLM-HARNESS-BEGIN: us-802e1aec
void bta_dm_auth_reply() {}
// LLM-HARNESS-END: us-802e1aec

// LLM-HARNESS-BEGIN: us-802e1bc0
void bta_dm_search_start() {}
// LLM-HARNESS-END: us-802e1bc0

// LLM-HARNESS-BEGIN: us-802e1c20
void bta_dm_search_cancel() {}
// LLM-HARNESS-END: us-802e1c20

// LLM-HARNESS-BEGIN: us-802e1ca4
void bta_dm_discover() {}
// LLM-HARNESS-END: us-802e1ca4

// LLM-HARNESS-BEGIN: us-802e1db8
void bta_dm_inq_cmpl() {}
// LLM-HARNESS-END: us-802e1db8

// LLM-HARNESS-BEGIN: us-802e1fdc
void bta_dm_rmt_name() {}
// LLM-HARNESS-END: us-802e1fdc

// LLM-HARNESS-BEGIN: us-802e2148
void bta_dm_disc_rmt_name() {}
// LLM-HARNESS-END: us-802e2148

// LLM-HARNESS-BEGIN: us-802e21ac
void bta_dm_sdp_result() {}
// LLM-HARNESS-END: us-802e21ac

// LLM-HARNESS-BEGIN: us-802e2414
void bta_dm_search_cmpl() {}
// LLM-HARNESS-END: us-802e2414

// LLM-HARNESS-BEGIN: us-802e242c
void bta_dm_disc_result() {}
// LLM-HARNESS-END: us-802e242c

// LLM-HARNESS-BEGIN: us-802e247c
void bta_dm_search_result() {}
// LLM-HARNESS-END: us-802e247c

// LLM-HARNESS-BEGIN: us-802e2504
void bta_dm_search_timer_cback() {}
// LLM-HARNESS-END: us-802e2504

// LLM-HARNESS-BEGIN: us-802e2550
void bta_dm_free_sdp_db() {}
// LLM-HARNESS-END: us-802e2550

// LLM-HARNESS-BEGIN: us-802e2594
void bta_dm_queue_search() {}
// LLM-HARNESS-END: us-802e2594

// LLM-HARNESS-BEGIN: us-802e25dc
void bta_dm_queue_disc() {}
// LLM-HARNESS-END: us-802e25dc

// LLM-HARNESS-BEGIN: us-802e2624
void bta_dm_search_clear_queue() {}
// LLM-HARNESS-END: us-802e2624

// LLM-HARNESS-BEGIN: us-802e2668
void bta_dm_search_cancel_cmpl() {}
// LLM-HARNESS-END: us-802e2668

// LLM-HARNESS-BEGIN: us-802e26ac
void bta_dm_search_cancel_transac_cmpl() {}
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_bond
// LLM-HARNESS-END: us-802e1998

// LLM-HARNESS-BEGIN: us-802e19e0
void bta_dm_bond() {}
// LLM-HARNESS-END: us-802e19e0

// LLM-HARNESS-BEGIN: us-802e1a50
void bta_dm_pin_reply() {}
// LLM-HARNESS-END: us-802e1a50

// LLM-HARNESS-BEGIN: us-802e1aec
void bta_dm_auth_reply() {}
// LLM-HARNESS-END: us-802e1aec

// LLM-HARNESS-BEGIN: us-802e1bc0
void bta_dm_search_start() {}
// LLM-HARNESS-END: us-802e1bc0

// LLM-HARNESS-BEGIN: us-802e1c20
void bta_dm_search_cancel() {}
// LLM-HARNESS-END: us-802e1c20

// LLM-HARNESS-BEGIN: us-802e1ca4
void bta_dm_discover() {}
// LLM-HARNESS-END: us-802e1ca4

// LLM-HARNESS-BEGIN: us-802e1db8
void bta_dm_inq_cmpl() {}
// LLM-HARNESS-END: us-802e1db8

// LLM-HARNESS-BEGIN: us-802e1fdc
void bta_dm_rmt_name() {}
// LLM-HARNESS-END: us-802e1fdc

// LLM-HARNESS-BEGIN: us-802e2148
void bta_dm_disc_rmt_name() {}
// LLM-HARNESS-END: us-802e2148

// LLM-HARNESS-BEGIN: us-802e21ac
void bta_dm_sdp_result() {}
// LLM-HARNESS-END: us-802e21ac

// LLM-HARNESS-BEGIN: us-802e2414
void bta_dm_search_cmpl() {}
// LLM-HARNESS-END: us-802e2414

// LLM-HARNESS-BEGIN: us-802e242c
void bta_dm_disc_result() {}
// LLM-HARNESS-END: us-802e242c

// LLM-HARNESS-BEGIN: us-802e247c
void bta_dm_search_result() {}
// LLM-HARNESS-END: us-802e247c

// LLM-HARNESS-BEGIN: us-802e2504
void bta_dm_search_timer_cback() {}
// LLM-HARNESS-END: us-802e2504

// LLM-HARNESS-BEGIN: us-802e2550
void bta_dm_free_sdp_db() {}
// LLM-HARNESS-END: us-802e2550

// LLM-HARNESS-BEGIN: us-802e2594
void bta_dm_queue_search() {}
// LLM-HARNESS-END: us-802e2594

// LLM-HARNESS-BEGIN: us-802e25dc
void bta_dm_queue_disc() {}
// LLM-HARNESS-END: us-802e25dc

// LLM-HARNESS-BEGIN: us-802e2624
void bta_dm_search_clear_queue() {}
// LLM-HARNESS-END: us-802e2624

// LLM-HARNESS-BEGIN: us-802e2668
void bta_dm_search_cancel_cmpl() {}
// LLM-HARNESS-END: us-802e2668

// LLM-HARNESS-BEGIN: us-802e26ac
void bta_dm_search_cancel_transac_cmpl() {}
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_cancel_rmt_name
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_immediate_disable
// LLM-HARNESS-END: us-802e395c

// LLM-HARNESS-BEGIN: us-802e39e0
void bta_dm_immediate_disable() {}
// LLM-HARNESS-END: us-802e39e0

// LLM-HARNESS-BEGIN: us-802e39fc
void bta_dm_reset_complete() {}
// LLM-HARNESS-END: us-802e39fc

// LLM-HARNESS-BEGIN: us-802e3a00
void bta_dm_send_hci_reset() {}

// from commit 365650b84230 needle=bta_dm_keep_acl
// LLM-HARNESS-END: us-802e3854

// LLM-HARNESS-BEGIN: us-802e395c
void bta_dm_keep_acl() {}
// LLM-HARNESS-END: us-802e395c

// LLM-HARNESS-BEGIN: us-802e39e0
void bta_dm_immediate_disable() {}
// LLM-HARNESS-END: us-802e39e0

// LLM-HARNESS-BEGIN: us-802e39fc
void bta_dm_reset_complete() {}
// LLM-HARNESS-END: us-802e39fc

// LLM-HARNESS-BEGIN: us-802e3a00
void bta_dm_send_hci_reset() {}

// from commit 365650b84230 needle=bta_dm_pin_reply
// LLM-HARNESS-END: us-802e19e0

// LLM-HARNESS-BEGIN: us-802e1a50
void bta_dm_pin_reply() {}
// LLM-HARNESS-END: us-802e1a50

// LLM-HARNESS-BEGIN: us-802e1aec
void bta_dm_auth_reply() {}
// LLM-HARNESS-END: us-802e1aec

// LLM-HARNESS-BEGIN: us-802e1bc0
void bta_dm_search_start() {}
// LLM-HARNESS-END: us-802e1bc0

// LLM-HARNESS-BEGIN: us-802e1c20
void bta_dm_search_cancel() {}
// LLM-HARNESS-END: us-802e1c20

// LLM-HARNESS-BEGIN: us-802e1ca4
void bta_dm_discover() {}
// LLM-HARNESS-END: us-802e1ca4

// LLM-HARNESS-BEGIN: us-802e1db8
void bta_dm_inq_cmpl() {}
// LLM-HARNESS-END: us-802e1db8

// LLM-HARNESS-BEGIN: us-802e1fdc
void bta_dm_rmt_name() {}
// LLM-HARNESS-END: us-802e1fdc

// LLM-HARNESS-BEGIN: us-802e2148
void bta_dm_disc_rmt_name() {}
// LLM-HARNESS-END: us-802e2148

// LLM-HARNESS-BEGIN: us-802e21ac
void bta_dm_sdp_result() {}
// LLM-HARNESS-END: us-802e21ac

// LLM-HARNESS-BEGIN: us-802e2414
void bta_dm_search_cmpl() {}
// LLM-HARNESS-END: us-802e2414

// LLM-HARNESS-BEGIN: us-802e242c
void bta_dm_disc_result() {}
// LLM-HARNESS-END: us-802e242c

// LLM-HARNESS-BEGIN: us-802e247c
void bta_dm_search_result() {}
// LLM-HARNESS-END: us-802e247c

// LLM-HARNESS-BEGIN: us-802e2504
void bta_dm_search_timer_cback() {}
// LLM-HARNESS-END: us-802e2504

// LLM-HARNESS-BEGIN: us-802e2550
void bta_dm_free_sdp_db() {}
// LLM-HARNESS-END: us-802e2550

// LLM-HARNESS-BEGIN: us-802e2594
void bta_dm_queue_search() {}
// LLM-HARNESS-END: us-802e2594

// LLM-HARNESS-BEGIN: us-802e25dc
void bta_dm_queue_disc() {}
// LLM-HARNESS-END: us-802e25dc

// LLM-HARNESS-BEGIN: us-802e2624
void bta_dm_search_clear_queue() {}
// LLM-HARNESS-END: us-802e2624

// LLM-HARNESS-BEGIN: us-802e2668
void bta_dm_search_cancel_cmpl() {}
// LLM-HARNESS-END: us-802e2668

// LLM-HARNESS-BEGIN: us-802e26ac
void bta_dm_search_cancel_transac_cmpl() {}
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_queue_disc
// LLM-HARNESS-END: us-802e2594

// LLM-HARNESS-BEGIN: us-802e25dc
void bta_dm_queue_disc() {}
// LLM-HARNESS-END: us-802e25dc

// LLM-HARNESS-BEGIN: us-802e2624
void bta_dm_search_clear_queue() {}
// LLM-HARNESS-END: us-802e2624

// LLM-HARNESS-BEGIN: us-802e2668
void bta_dm_search_cancel_cmpl() {}
// LLM-HARNESS-END: us-802e2668

// LLM-HARNESS-BEGIN: us-802e26ac
void bta_dm_search_cancel_transac_cmpl() {}
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_queue_search
// LLM-HARNESS-END: us-802e2550

// LLM-HARNESS-BEGIN: us-802e2594
void bta_dm_queue_search() {}
// LLM-HARNESS-END: us-802e2594

// LLM-HARNESS-BEGIN: us-802e25dc
void bta_dm_queue_disc() {}
// LLM-HARNESS-END: us-802e25dc

// LLM-HARNESS-BEGIN: us-802e2624
void bta_dm_search_clear_queue() {}
// LLM-HARNESS-END: us-802e2624

// LLM-HARNESS-BEGIN: us-802e2668
void bta_dm_search_cancel_cmpl() {}
// LLM-HARNESS-END: us-802e2668

// LLM-HARNESS-BEGIN: us-802e26ac
void bta_dm_search_cancel_transac_cmpl() {}
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_search_cancel_cmpl
// LLM-HARNESS-END: us-802e2624

// LLM-HARNESS-BEGIN: us-802e2668
void bta_dm_search_cancel_cmpl() {}
// LLM-HARNESS-END: us-802e2668

// LLM-HARNESS-BEGIN: us-802e26ac
void bta_dm_search_cancel_transac_cmpl() {}
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_search_cancel_notify
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_search_cancel_transac_cmpl
// LLM-HARNESS-END: us-802e2668

// LLM-HARNESS-BEGIN: us-802e26ac
void bta_dm_search_cancel_transac_cmpl() {}
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_search_clear_queue
// LLM-HARNESS-END: us-802e25dc

// LLM-HARNESS-BEGIN: us-802e2624
void bta_dm_search_clear_queue() {}
// LLM-HARNESS-END: us-802e2624

// LLM-HARNESS-BEGIN: us-802e2668
void bta_dm_search_cancel_cmpl() {}
// LLM-HARNESS-END: us-802e2668

// LLM-HARNESS-BEGIN: us-802e26ac
void bta_dm_search_cancel_transac_cmpl() {}
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_search_start
// LLM-HARNESS-END: us-802e1aec

// LLM-HARNESS-BEGIN: us-802e1bc0
void bta_dm_search_start() {}
// LLM-HARNESS-END: us-802e1bc0

// LLM-HARNESS-BEGIN: us-802e1c20
void bta_dm_search_cancel() {}
// LLM-HARNESS-END: us-802e1c20

// LLM-HARNESS-BEGIN: us-802e1ca4
void bta_dm_discover() {}
// LLM-HARNESS-END: us-802e1ca4

// LLM-HARNESS-BEGIN: us-802e1db8
void bta_dm_inq_cmpl() {}
// LLM-HARNESS-END: us-802e1db8

// LLM-HARNESS-BEGIN: us-802e1fdc
void bta_dm_rmt_name() {}
// LLM-HARNESS-END: us-802e1fdc

// LLM-HARNESS-BEGIN: us-802e2148
void bta_dm_disc_rmt_name() {}
// LLM-HARNESS-END: us-802e2148

// LLM-HARNESS-BEGIN: us-802e21ac
void bta_dm_sdp_result() {}
// LLM-HARNESS-END: us-802e21ac

// LLM-HARNESS-BEGIN: us-802e2414
void bta_dm_search_cmpl() {}
// LLM-HARNESS-END: us-802e2414

// LLM-HARNESS-BEGIN: us-802e242c
void bta_dm_disc_result() {}
// LLM-HARNESS-END: us-802e242c

// LLM-HARNESS-BEGIN: us-802e247c
void bta_dm_search_result() {}
// LLM-HARNESS-END: us-802e247c

// LLM-HARNESS-BEGIN: us-802e2504
void bta_dm_search_timer_cback() {}
// LLM-HARNESS-END: us-802e2504

// LLM-HARNESS-BEGIN: us-802e2550
void bta_dm_free_sdp_db() {}
// LLM-HARNESS-END: us-802e2550

// LLM-HARNESS-BEGIN: us-802e2594
void bta_dm_queue_search() {}
// LLM-HARNESS-END: us-802e2594

// LLM-HARNESS-BEGIN: us-802e25dc
void bta_dm_queue_disc() {}
// LLM-HARNESS-END: us-802e25dc

// LLM-HARNESS-BEGIN: us-802e2624
void bta_dm_search_clear_queue() {}
// LLM-HARNESS-END: us-802e2624

// LLM-HARNESS-BEGIN: us-802e2668
void bta_dm_search_cancel_cmpl() {}
// LLM-HARNESS-END: us-802e2668

// LLM-HARNESS-BEGIN: us-802e26ac
void bta_dm_search_cancel_transac_cmpl() {}
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 365650b84230 needle=bta_dm_send_hci_reset
// LLM-HARNESS-END: us-802e39fc

// LLM-HARNESS-BEGIN: us-802e3a00
void bta_dm_send_hci_reset() {}

// from commit 365650b84230 needle=bta_dm_set_dev_name
// LLM-HARNESS-END: us-802e18d4

// LLM-HARNESS-BEGIN: us-802e1990
int bta_dm_set_dev_name() { return 0; }

// from commit 365650b84230 needle=bta_dm_set_visibility
// LLM-HARNESS-END: us-802e1990

// LLM-HARNESS-BEGIN: us-802e1998
void bta_dm_set_visibility() {}
// LLM-HARNESS-END: us-802e1998

// LLM-HARNESS-BEGIN: us-802e19e0
void bta_dm_bond() {}
// LLM-HARNESS-END: us-802e19e0

// LLM-HARNESS-BEGIN: us-802e1a50
void bta_dm_pin_reply() {}
// LLM-HARNESS-END: us-802e1a50

// LLM-HARNESS-BEGIN: us-802e1aec
void bta_dm_auth_reply() {}
// LLM-HARNESS-END: us-802e1aec

// LLM-HARNESS-BEGIN: us-802e1bc0
void bta_dm_search_start() {}
// LLM-HARNESS-END: us-802e1bc0

// LLM-HARNESS-BEGIN: us-802e1c20
void bta_dm_search_cancel() {}
// LLM-HARNESS-END: us-802e1c20

// LLM-HARNESS-BEGIN: us-802e1ca4
void bta_dm_discover() {}
// LLM-HARNESS-END: us-802e1ca4

// LLM-HARNESS-BEGIN: us-802e1db8
void bta_dm_inq_cmpl() {}
// LLM-HARNESS-END: us-802e1db8

// LLM-HARNESS-BEGIN: us-802e1fdc
void bta_dm_rmt_name() {}
// LLM-HARNESS-END: us-802e1fdc

// LLM-HARNESS-BEGIN: us-802e2148
void bta_dm_disc_rmt_name() {}
// LLM-HARNESS-END: us-802e2148

// LLM-HARNESS-BEGIN: us-802e21ac
void bta_dm_sdp_result() {}
// LLM-HARNESS-END: us-802e21ac

// LLM-HARNESS-BEGIN: us-802e2414
void bta_dm_search_cmpl() {}
// LLM-HARNESS-END: us-802e2414

// LLM-HARNESS-BEGIN: us-802e242c
void bta_dm_disc_result() {}
// LLM-HARNESS-END: us-802e242c

// LLM-HARNESS-BEGIN: us-802e247c
void bta_dm_search_result() {}
// LLM-HARNESS-END: us-802e247c

// LLM-HARNESS-BEGIN: us-802e2504
void bta_dm_search_timer_cback() {}
// LLM-HARNESS-END: us-802e2504

// LLM-HARNESS-BEGIN: us-802e2550
void bta_dm_free_sdp_db() {}
// LLM-HARNESS-END: us-802e2550

// LLM-HARNESS-BEGIN: us-802e2594
void bta_dm_queue_search() {}
// LLM-HARNESS-END: us-802e2594

// LLM-HARNESS-BEGIN: us-802e25dc
void bta_dm_queue_disc() {}
// LLM-HARNESS-END: us-802e25dc

// LLM-HARNESS-BEGIN: us-802e2624
void bta_dm_search_clear_queue() {}
// LLM-HARNESS-END: us-802e2624

// LLM-HARNESS-BEGIN: us-802e2668
void bta_dm_search_cancel_cmpl() {}
// LLM-HARNESS-END: us-802e2668

// LLM-HARNESS-BEGIN: us-802e26ac
void bta_dm_search_cancel_transac_cmpl() {}
// LLM-HARNESS-END: us-802e26ac

// LLM-HARNESS-BEGIN: us-802e2708
void bta_dm_search_cancel_notify() {}
// LLM-HARNESS-END: us-802e2708

// LLM-HARNESS-BEGIN: us-802e2720
void bta_dm_find_services() {}
// LLM-HARNESS-END: us-802e2720

// LLM-HARNESS-BEGIN: us-802e28c0
void bta_dm_discover_next_device() {}
// LLM-HARNESS-END: us-802e28c0

// LLM-HARNESS-BEGIN: us-802e29a0
void bta_dm_sdp_callback() {}
// LLM-HARNESS-END: us-802e29a0

// LLM-HARNESS-BEGIN: us-802e29e8
void bta_dm_inq_results_cb() {}
// LLM-HARNESS-END: us-802e29e8

// LLM-HARNESS-BEGIN: us-802e2a78
void bta_dm_inq_cmpl_cb() {}
// LLM-HARNESS-END: us-802e2a78

// LLM-HARNESS-BEGIN: us-802e2ac4
void bta_dm_service_search_remname_cback() {}
// LLM-HARNESS-END: us-802e2ac4

// LLM-HARNESS-BEGIN: us-802e2b04
void bta_dm_remname_cback() {}
// LLM-HARNESS-END: us-802e2b04

// LLM-HARNESS-BEGIN: us-802e2b90
void bta_dm_disc_remname_cback() {}
// LLM-HARNESS-END: us-802e2b90

// LLM-HARNESS-BEGIN: us-802e2c18
void bta_dm_cancel_rmt_name() {}
// LLM-HARNESS-END: us-802e2c18

// LLM-HARNESS-BEGIN: us-802e2c1c
void bta_dm_authorize_cback() {}
// LLM-HARNESS-END: us-802e2c1c

// LLM-HARNESS-BEGIN: us-802e2ce4
void bta_dm_pinname_cback() {}
// LLM-HARNESS-END: us-802e2ce4

// LLM-HARNESS-BEGIN: us-802e2da4
void bta_dm_pin_cback() {}
// LLM-HARNESS-END: us-802e2da4

// LLM-HARNESS-BEGIN: us-802e2ed4
int bta_dm_link_key_request_cback() { return 0; }

// from commit 702909a6fbbd needle=bta_dm_authentication_complete_cback
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

// from commit 702909a6fbbd needle=bta_dm_inq_cmpl_cb
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

// from commit 702909a6fbbd needle=bta_dm_inq_results_cb

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

// from commit 95ad18ac12e1 needle=bta_dm_acl_change_cback
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

// from commit 95ad18ac12e1 needle=bta_dm_disc_remname_cback
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

// from commit 95ad18ac12e1 needle=bta_dm_l2cap_server_compress_cback
   pointers are used here. */
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

// from commit 95ad18ac12e1 needle=bta_dm_local_addr_cback

void bta_dm_local_addr_cback(void *addr) {
    if (bta_dm_cb.cback != NULL) {
        bta_dm_cb.cback(0, addr);
    }

// from commit 95ad18ac12e1 needle=bta_dm_pin_cback
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

// from commit 95ad18ac12e1 needle=bta_dm_pinname_cback
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

// from commit 95ad18ac12e1 needle=bta_dm_rm_cback
   the role-management config table. */
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

// from commit 95ad18ac12e1 needle=bta_dm_signal_strength
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

// --- restored from git history (base:gone repo-search) ---
// from commit e48802b591c4 path=libs/RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act.c needle=bta_dm_acl_change
   disable is pending, complete the disable. */
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

