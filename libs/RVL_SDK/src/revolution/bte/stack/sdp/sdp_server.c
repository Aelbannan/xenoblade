/* sdp_server.c - SDP server (BTE stack)
 *
 * High-level C reconstruction of the retail TU
 * RVL_SDK/src/revolution/bte/stack/sdp/sdp_server.
 *
 * Handles incoming SDP client PDUs: service search, service attribute
 * requests and combined search/attribute requests, including the SDP
 * continuation state machinery.
 */

#include <harness_catalog.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned char BOOLEAN;

#ifndef NULL
#define NULL 0
#endif

/* ------------------------------------------------------------------ */
/* Buffer header (see bt_types.h)                                      */
/* ------------------------------------------------------------------ */

typedef struct {
    UINT16 event;
    UINT16 len;
    UINT16 offset;
    UINT16 layer_specific;
} BT_HDR;

/* ------------------------------------------------------------------ */
/* Timer list entry (see gki.h); sizeof == 0x18                        */
/* ------------------------------------------------------------------ */

typedef struct _tle {
    struct _tle *p_next;
    struct _tle *p_prev;
    void *p_cback;
    int ticks;
    void *param;
    UINT16 event;
    UINT8 in_use;
} TIMER_LIST_ENT;

/* ------------------------------------------------------------------ */
/* SDP connection control block (sdp_int.h)                            */
/* ------------------------------------------------------------------ */

#define SDP_MAX_ATTR_LEN 1000

typedef struct {
    UINT8 in_use;                     /* 0x000 */
    UINT8 pad0[7];                    /* 0x001 */
    TIMER_LIST_ENT timer_entry;       /* 0x008 */
    UINT16 rem_mtu_size;              /* 0x020 */
    UINT16 connection_id;             /* 0x022 */
    UINT16 rsp_len;                   /* 0x024 */
    UINT8 rsp_buf[0x476 - 0x26];      /* 0x026 */
    UINT16 cont_offset;               /* 0x476 */
} tCONN_CB;                           /* 0x478 */

/* ------------------------------------------------------------------ */
/* SDP database records / attributes (sdp_int.h)                       */
/* ------------------------------------------------------------------ */

typedef struct {
    UINT32 record_handle;
} tSDP_RECORD;

typedef struct {
    UINT32 len;
    UINT8 *value_ptr;
    UINT16 id;
    UINT16 type;
} tSDP_ATTR;

/* ------------------------------------------------------------------ */
/* UUID / attribute filter sequences extracted from requests           */
/* ------------------------------------------------------------------ */

#define SDP_MAX_UUID_FILTERS 16
#define SDP_MAX_ATTR_FILTERS 16
#define SDP_MAX_RECORDS 20

typedef struct {
    UINT16 len;
    UINT8 value[16];
} tSDP_UUID;

typedef struct {
    UINT16 num_uids;
    tSDP_UUID uuid_entry[SDP_MAX_UUID_FILTERS];
} tSDP_UUID_SEQ;

typedef struct {
    UINT16 start;
    UINT16 end;
} tSDP_ATTR_FILTER;

typedef struct {
    UINT16 num_attr;
    tSDP_ATTR_FILTER attr_entry[SDP_MAX_ATTR_FILTERS];
} tSDP_ATTR_SEQ;

/* ------------------------------------------------------------------ */
/* SDP control block (only the trace level is used here)               */
/* ------------------------------------------------------------------ */

typedef struct {
    UINT8 pad[0x4630];
    UINT8 trace_level;
} tSDP_CB;

extern tSDP_CB sdp_cb;

/* ------------------------------------------------------------------ */
/* Constants                                                           */
/* ------------------------------------------------------------------ */

#define BTU_TTYPE_SDP 5
#define SDP_INACT_TIMEOUT 30

#define SDP_PDU_SERVICE_SEARCH_REQ 2
#define SDP_PDU_SERVICE_SEARCH_RSP 3
#define SDP_PDU_SERVICE_ATTR_REQ 4
#define SDP_PDU_SERVICE_ATTR_RSP 5
#define SDP_PDU_SERVICE_SEARCH_ATTR_REQ 6
#define SDP_PDU_SERVICE_SEARCH_ATTR_RSP 7

#define SDP_INVALID_SERV_REC_HDL 2
#define SDP_INVALID_REQ_SYNTAX 3
#define SDP_INVALID_PDU_SIZE 4
#define SDP_INVALID_CONT_STATE 5

#define BT_TRACE_LEVEL_ERROR 1
#define BT_TRACE_LEVEL_WARNING 2

#define SDP_TRACE_ERROR0(m)                                     \
    {                                                           \
        if (sdp_cb.trace_level >= BT_TRACE_LEVEL_ERROR)         \
            LogMsg_0(0x000A0000, (m));                          \
    }
#define SDP_TRACE_WARNING1(m, p1)                               \
    {                                                           \
        if (sdp_cb.trace_level >= BT_TRACE_LEVEL_WARNING)       \
            LogMsg_1(0x000A0001, (m), (UINT32)(p1));            \
    }

/* ------------------------------------------------------------------ */
/* External helpers                                                    */
/* ------------------------------------------------------------------ */

extern void btu_start_timer(TIMER_LIST_ENT *p_tle, UINT16 type, UINT32 timeout);
extern void sdpu_build_n_send_error(tCONN_CB *p_ccb, UINT16 trans_num,
                                    UINT16 error_code, UINT16 error_text);
extern UINT8 *sdpu_extract_uid_seq(UINT8 *p, UINT16 param_len, tSDP_UUID_SEQ *p_seq);
extern UINT8 *sdpu_extract_attr_seq(UINT8 *p, UINT16 param_len, tSDP_ATTR_SEQ *p_seq);
extern UINT8 *sdpu_build_attrib_entry(UINT8 *p_rsp, tSDP_ATTR *p_attr);
extern tSDP_RECORD *sdp_db_service_search(tSDP_RECORD *p_rec, tSDP_UUID_SEQ *p_seq);
extern tSDP_RECORD *sdp_db_find_record(UINT32 handle);
extern tSDP_ATTR *sdp_db_find_attr_in_rec(tSDP_RECORD *p_rec, UINT16 start_attr,
                                          UINT16 end_attr);
extern void *GKI_getpoolbuf(UINT8 pool_id);
extern BOOLEAN L2CA_DataWrite(UINT16 cid, BT_HDR *p_data);
extern void LogMsg_0(UINT32 trace_set_mask, const char *p_str);
extern void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void *memcpy(void *dst, const void *src, UINT32 n);

static void process_service_search(tCONN_CB *p_ccb, UINT16 trans_num,
                                   UINT16 param_len, UINT8 *p_req, UINT8 *p_req_end);
static void process_service_attr_req(tCONN_CB *p_ccb, UINT16 trans_num,
                                     UINT16 param_len, UINT8 *p_req, UINT8 *p_req_end);
static void process_service_search_attr_req(tCONN_CB *p_ccb, UINT16 trans_num,
                                            UINT16 param_len, UINT8 *p_req,
                                            UINT8 *p_req_end);

/*******************************************************************************
**
** Function         sdp_server_handle_client_req
**
** Description      This function is called when a client request is received.
**
*******************************************************************************/
void sdp_server_handle_client_req(tCONN_CB *p_ccb, BT_HDR *p_msg)
{
    UINT8 *p_req;
    UINT8 *p_req_end;
    UINT8 pdu_id;
    UINT16 trans_num;
    UINT16 param_len;

    p_req = (UINT8 *)(p_msg + 1) + p_msg->offset;
    p_req_end = p_req + p_msg->len;

    /* Start the inactivity timer */
    btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_SDP, SDP_INACT_TIMEOUT);

    pdu_id = *p_req++;
    trans_num = (UINT16)(((UINT16)(*p_req) << 8) + *(p_req + 1));
    p_req += 2;
    param_len = (UINT16)(((UINT16)(*p_req) << 8) + *(p_req + 1));
    p_req += 2;

    /* Check that the packet length is valid */
    if ((p_req + param_len) > p_req_end) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_PDU_SIZE, 0);
        return;
    }

    switch (pdu_id) {
    case SDP_PDU_SERVICE_SEARCH_REQ:
        process_service_search(p_ccb, trans_num, param_len, p_req, p_req_end);
        break;

    case SDP_PDU_SERVICE_ATTR_REQ:
        process_service_attr_req(p_ccb, trans_num, param_len, p_req, p_req_end);
        break;

    case SDP_PDU_SERVICE_SEARCH_ATTR_REQ:
        process_service_search_attr_req(p_ccb, trans_num, param_len, p_req, p_req_end);
        break;

    default:
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_REQ_SYNTAX, 0);
        SDP_TRACE_WARNING1("SDP - server got unknown PDU: 0x%x", pdu_id);
        break;
    }
}

/*******************************************************************************
**
** Function         process_service_search
**
** Description      This function handles a service search request.
**
*******************************************************************************/
static void process_service_search(tCONN_CB *p_ccb, UINT16 trans_num,
                                   UINT16 param_len, UINT8 *p_req, UINT8 *p_req_end)
{
    UINT16 max_handles;
    UINT16 num_handles;
    UINT16 max_recs;
    UINT16 xx;
    UINT16 start_rec;
    UINT16 end_rec;
    UINT8 cont = 0;
    tSDP_RECORD *p_rec;
    UINT32 handles[SDP_MAX_RECORDS];
    tSDP_UUID_SEQ uid_seq;
    BT_HDR *p_buf;
    UINT8 *p_rsp;
    UINT8 *p_rsp_start;
    UINT8 *p_param_len;

    /* Extract the UUID sequence to search for */
    p_req = sdpu_extract_uid_seq(p_req, param_len, &uid_seq);
    if (p_req == NULL || uid_seq.num_uids == 0) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_REQ_SYNTAX, 0);
        return;
    }

    /* Get the maximum handles to return */
    max_handles = (UINT16)(((UINT16)(*p_req) << 8) + *(p_req + 1));
    p_req += 2;
    if (max_handles > SDP_MAX_RECORDS)
        max_handles = SDP_MAX_RECORDS;

    /* Now, find all the matching records in the database */
    num_handles = 0;
    for (xx = 0, p_rec = NULL; xx < max_handles; xx++) {
        p_rec = sdp_db_service_search(p_rec, &uid_seq);
        if (p_rec == NULL)
            break;

        handles[xx] = p_rec->record_handle;
    }

    num_handles = xx;

    /* Check for a continuation state */
    if (*p_req == 0) {
        start_rec = 0;
    } else {
        if (*p_req != 2) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, 0);
            return;
        }

        start_rec = (UINT16)(((UINT16)p_req[1] << 8) + p_req[2]);
        p_req += 3;
        if (start_rec != p_ccb->cont_offset) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, 0);
            return;
        }
    }

    num_handles -= start_rec;

    /* Work out how many records fit in one response */
    max_recs = (UINT16)((p_ccb->rem_mtu_size - 12) / 4 + 1);
    if (num_handles > max_recs) {
        cont = 1;
        p_ccb->cont_offset += max_recs;
    } else {
        max_recs = num_handles;
    }

    p_buf = (BT_HDR *)GKI_getpoolbuf(2);
    if (p_buf == NULL) {
        SDP_TRACE_ERROR0("SDP - no buf for search rsp");
        return;
    }

    /* Build the response */
    p_buf->offset = 9;
    p_rsp_start = (UINT8 *)(p_buf + 1) + p_buf->offset;
    p_rsp = p_rsp_start;

    *p_rsp++ = SDP_PDU_SERVICE_SEARCH_RSP;
    *p_rsp++ = (UINT8)(trans_num >> 8);
    *p_rsp++ = (UINT8)trans_num;

    p_param_len = p_rsp;
    p_rsp += 2;

    *p_rsp++ = (UINT8)(num_handles >> 8);
    *p_rsp++ = (UINT8)num_handles;
    *p_rsp++ = (UINT8)(max_recs >> 8);
    *p_rsp++ = (UINT8)max_recs;

    /* Copy the record handles into the response */
    end_rec = (UINT16)(start_rec + max_recs);
    for (xx = start_rec; xx < end_rec; xx++) {
        *p_rsp++ = (UINT8)(handles[xx] >> 24);
        *p_rsp++ = (UINT8)(handles[xx] >> 16);
        *p_rsp++ = (UINT8)(handles[xx] >> 8);
        *p_rsp++ = (UINT8)handles[xx];
    }

    /* Append the continuation state */
    if (cont) {
        *p_rsp++ = 2;
        *p_rsp++ = (UINT8)(p_ccb->cont_offset >> 8);
        *p_rsp++ = (UINT8)p_ccb->cont_offset;
    } else {
        *p_rsp++ = 0;
    }

    p_param_len[0] = (UINT8)(((UINT16)(p_rsp - p_param_len) - 2) >> 8);
    p_param_len[1] = (UINT8)((UINT16)(p_rsp - p_param_len) - 2);
    p_buf->len = (UINT16)(p_rsp - p_rsp_start);

    L2CA_DataWrite(p_ccb->connection_id, p_buf);
}

/*******************************************************************************
**
** Function         process_service_attr_req
**
** Description      This function handles a service attribute request.
**
*******************************************************************************/
static void process_service_attr_req(tCONN_CB *p_ccb, UINT16 trans_num,
                                     UINT16 param_len, UINT8 *p_req, UINT8 *p_req_end)
{
    UINT32 rec_handle;
    UINT16 max_list_len;
    UINT16 xx;
    tSDP_RECORD *p_rec;
    tSDP_ATTR *p_attr;
    tSDP_ATTR_SEQ attr_seq;
    UINT8 *p_rsp;
    UINT16 total_len;
    BT_HDR *p_buf;
    UINT8 *p_rsp_start;
    UINT8 *p_param_len;

    /* The record handle comes first */
    if ((p_req + 4) > p_req_end) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_SERV_REC_HDL, 0);
        return;
    }

    rec_handle = ((UINT32)(*p_req) << 24) + ((UINT32)*(p_req + 1) << 16) +
                 ((UINT32)*(p_req + 2) << 8) + (UINT32)*(p_req + 3);

    /* Then the maximum attribute list length */
    max_list_len = (UINT16)(((UINT16)*(p_req + 4) << 8) + *(p_req + 5));
    if (max_list_len > (p_ccb->rem_mtu_size - 10))
        max_list_len = (UINT16)(p_ccb->rem_mtu_size - 10);

    /* Extract the attribute sequence */
    p_req = sdpu_extract_attr_seq(p_req + 6, param_len, &attr_seq);
    if (p_req == NULL || attr_seq.num_attr == 0 || p_req > p_req_end) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_REQ_SYNTAX, 0);
        return;
    }

    /* Find the record in the database */
    p_rec = sdp_db_find_record(rec_handle);
    if (p_rec == NULL) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_SERV_REC_HDL, 0);
        return;
    }

    /* Check for a continuation state */
    if (*p_req != 0) {
        if (*p_req != 2) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, 0);
            return;
        }

        if ((UINT16)(((UINT16)p_req[1] << 8) + p_req[2]) != p_ccb->cont_offset) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, 0);
            return;
        }
    } else {
        /* Build the attribute list into the connection buffer */
        p_ccb->cont_offset = 0;
        p_rsp = p_ccb->rsp_buf + 3;

        for (xx = 0; xx < attr_seq.num_attr; xx++) {
            p_attr = sdp_db_find_attr_in_rec(p_rec, attr_seq.attr_entry[xx].start,
                                            attr_seq.attr_entry[xx].end);
            if (p_attr != NULL) {
                /* Make sure the attribute fits in the buffer */
                if (p_attr->len >
                    (UINT16)(SDP_MAX_ATTR_LEN - (UINT16)(p_rsp - p_ccb->rsp_buf)) - 6)
                    break;

                p_rsp = sdpu_build_attrib_entry(p_rsp, p_attr);

                /* For a range, continue from the last attribute returned */
                if (attr_seq.attr_entry[xx].start != attr_seq.attr_entry[xx].end) {
                    attr_seq.attr_entry[xx].start = (UINT16)(p_attr->id + 1);
                    xx--;
                }
            }
        }

        /* Prepend the data element sequence header */
        total_len = (UINT16)(p_rsp - p_ccb->rsp_buf);
        p_ccb->rsp_len = total_len;
        if (total_len > 0xFF) {
            p_ccb->rsp_buf[0] = 0x36;
            p_ccb->rsp_buf[1] = (UINT8)((total_len - 3) >> 8);
            p_ccb->rsp_buf[2] = (UINT8)(total_len - 3);
        } else {
            p_ccb->cont_offset = 1;
            p_ccb->rsp_buf[1] = 0x35;
            p_ccb->rsp_buf[2] = (UINT8)(total_len - 3);
            p_ccb->rsp_len = (UINT16)(total_len - 1);
        }
    }

    p_buf = (BT_HDR *)GKI_getpoolbuf(2);
    if (p_buf == NULL) {
        SDP_TRACE_ERROR0("SDP - no buf for search rsp");
        return;
    }

    /* Build the response */
    p_buf->offset = 9;
    p_rsp_start = (UINT8 *)(p_buf + 1) + p_buf->offset;
    p_rsp = p_rsp_start;

    *p_rsp++ = SDP_PDU_SERVICE_ATTR_RSP;
    *p_rsp++ = (UINT8)(trans_num >> 8);
    *p_rsp++ = (UINT8)trans_num;

    p_param_len = p_rsp;
    p_rsp += 2;

    if (p_ccb->rsp_len <= max_list_len)
        max_list_len = p_ccb->rsp_len;

    *p_rsp++ = (UINT8)(max_list_len >> 8);
    *p_rsp++ = (UINT8)max_list_len;

    memcpy(p_rsp, p_ccb->rsp_buf + p_ccb->cont_offset, max_list_len);
    p_rsp += max_list_len;

    p_ccb->rsp_len = (UINT16)(p_ccb->rsp_len - max_list_len);
    p_ccb->cont_offset = (UINT16)(p_ccb->cont_offset + max_list_len);

    /* Append the continuation state */
    if (p_ccb->rsp_len != 0) {
        *p_rsp++ = 2;
        *p_rsp++ = (UINT8)(p_ccb->cont_offset >> 8);
        *p_rsp++ = (UINT8)p_ccb->cont_offset;
    } else {
        *p_rsp++ = 0;
    }

    p_param_len[0] = (UINT8)(((p_rsp - p_param_len) - 2) >> 8);
    p_param_len[1] = (UINT8)((p_rsp - p_param_len) - 2);
    p_buf->len = (UINT16)(p_rsp - p_rsp_start);

    L2CA_DataWrite(p_ccb->connection_id, p_buf);
}

/*******************************************************************************
**
** Function         process_service_search_attr_req
**
** Description      This function handles a service search attribute request.
**
*******************************************************************************/
static void process_service_search_attr_req(tCONN_CB *p_ccb, UINT16 trans_num,
                                            UINT16 param_len, UINT8 *p_req,
                                            UINT8 *p_req_end)
{
    UINT16 max_list_len;
    UINT16 xx;
    UINT8 truncated = 0;
    tSDP_UUID_SEQ uid_seq;
    tSDP_ATTR_SEQ attr_seq;
    tSDP_ATTR_SEQ attr_seq_orig;
    tSDP_RECORD *p_rec;
    tSDP_ATTR *p_attr;
    UINT8 *p_rsp;
    UINT8 *p_rec_start;
    UINT16 total_len;
    BT_HDR *p_buf;
    UINT8 *p_rsp_start;
    UINT8 *p_param_len;

    /* Extract the UUID sequence to search for */
    p_req = sdpu_extract_uid_seq(p_req, param_len, &uid_seq);
    if (p_req == NULL || uid_seq.num_uids == 0) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_REQ_SYNTAX, 0);
        return;
    }

    /* Get the maximum attribute list length */
    max_list_len = (UINT16)(((UINT16)(*p_req) << 8) + *(p_req + 1));
    if (max_list_len > (p_ccb->rem_mtu_size - 10))
        max_list_len = (UINT16)(p_ccb->rem_mtu_size - 10);

    /* Extract the attribute sequence */
    p_req = sdpu_extract_attr_seq(p_req + 2, param_len, &attr_seq);
    if (p_req == NULL || attr_seq.num_attr == 0) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_REQ_SYNTAX, 0);
        return;
    }

    /* The attribute list is modified during the search; keep a copy */
    attr_seq_orig = attr_seq;

    /* Check for a continuation state */
    if (*p_req != 0) {
        if (*p_req != 2) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, 0);
            return;
        }

        if ((UINT16)(((UINT16)p_req[1] << 8) + p_req[2]) != p_ccb->cont_offset) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, 0);
            return;
        }
    } else {
        p_ccb->cont_offset = 0;
        p_rsp = p_ccb->rsp_buf + 3;

        /* Build the attribute lists for every matching record */
        p_rec = sdp_db_service_search(NULL, &uid_seq);
        while (p_rec != NULL) {
            attr_seq = attr_seq_orig;
            p_rec_start = p_rsp;
            p_rsp += 3;

            for (xx = 0; xx < attr_seq.num_attr; xx++) {
                p_attr = sdp_db_find_attr_in_rec(p_rec, attr_seq.attr_entry[xx].start,
                                                 attr_seq.attr_entry[xx].end);
                if (p_attr != NULL) {
                    /* Make sure the attribute fits in the buffer */
                    if (p_attr->len >
                        (UINT16)(SDP_MAX_ATTR_LEN - (UINT16)(p_rsp - p_ccb->rsp_buf)) - 6) {
                        truncated = 1;
                        break;
                    }

                    p_rsp = sdpu_build_attrib_entry(p_rsp, p_attr);

                    /* For a range, continue from the last attribute returned */
                    if (attr_seq.attr_entry[xx].start != attr_seq.attr_entry[xx].end) {
                        attr_seq.attr_entry[xx].start = (UINT16)(p_attr->id + 1);
                        xx--;
                    }
                }
            }

            if (truncated)
                break;

            /* Prepend the per-record data element sequence header */
            if ((UINT16)(p_rsp - p_rec_start - 3) != 0) {
                p_rec_start[0] = 0x36;
                p_rec_start[1] = (UINT8)((UINT16)(p_rsp - p_rec_start - 3) >> 8);
                p_rec_start[2] = (UINT8)(p_rsp - p_rec_start - 3);
            } else {
                p_rsp = p_rec_start;
            }

            p_rec = sdp_db_service_search(p_rec, &uid_seq);
        }
    }

    /* Prepend the outer data element sequence header */
    total_len = (UINT16)(p_rsp - p_ccb->rsp_buf);
    p_ccb->rsp_len = total_len;
    if (total_len > 0xFF) {
        p_ccb->rsp_buf[0] = 0x36;
        p_ccb->rsp_buf[1] = (UINT8)((total_len - 3) >> 8);
        p_ccb->rsp_buf[2] = (UINT8)(total_len - 3);
    } else {
        p_ccb->cont_offset = 1;
        p_ccb->rsp_buf[1] = 0x35;
        p_ccb->rsp_buf[2] = (UINT8)(total_len - 3);
        p_ccb->rsp_len = (UINT16)(total_len - 1);
    }

    p_buf = (BT_HDR *)GKI_getpoolbuf(2);
    if (p_buf == NULL) {
        SDP_TRACE_ERROR0("SDP - no buf for search rsp");
        return;
    }

    /* Build the response */
    p_buf->offset = 9;
    p_rsp_start = (UINT8 *)(p_buf + 1) + p_buf->offset;
    p_rsp = p_rsp_start;

    *p_rsp++ = SDP_PDU_SERVICE_SEARCH_ATTR_RSP;
    *p_rsp++ = (UINT8)(trans_num >> 8);
    *p_rsp++ = (UINT8)trans_num;

    p_param_len = p_rsp;
    p_rsp += 2;

    if (p_ccb->rsp_len <= max_list_len)
        max_list_len = p_ccb->rsp_len;

    *p_rsp++ = (UINT8)(max_list_len >> 8);
    *p_rsp++ = (UINT8)max_list_len;

    memcpy(p_rsp, p_ccb->rsp_buf + p_ccb->cont_offset, max_list_len);
    p_rsp += max_list_len;

    p_ccb->rsp_len = (UINT16)(p_ccb->rsp_len - max_list_len);
    p_ccb->cont_offset = (UINT16)(p_ccb->cont_offset + max_list_len);

    /* Append the continuation state */
    if (p_ccb->rsp_len != 0) {
        *p_rsp++ = 2;
        *p_rsp++ = (UINT8)(p_ccb->cont_offset >> 8);
        *p_rsp++ = (UINT8)p_ccb->cont_offset;
    } else {
        *p_rsp++ = 0;
    }

    p_param_len[0] = (UINT8)(((p_rsp - p_param_len) - 2) >> 8);
    p_param_len[1] = (UINT8)((p_rsp - p_param_len) - 2);
    p_buf->len = (UINT16)(p_rsp - p_rsp_start);

    L2CA_DataWrite(p_ccb->connection_id, p_buf);
}
