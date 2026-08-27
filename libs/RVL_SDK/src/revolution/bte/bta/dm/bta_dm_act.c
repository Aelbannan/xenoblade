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
