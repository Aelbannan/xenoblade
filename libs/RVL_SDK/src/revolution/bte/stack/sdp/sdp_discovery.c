// Decompiled assembly for RVL_SDK/src/revolution/bte/stack/sdp/sdp_discovery.
//
// The SDP discovery state machine drives client-side service discovery over
// L2CAP. This TU implements the connected/response processing pipeline and
// the scratch-pad record/attribute builders used by the in-memory discovery
// database.

#include <harness_catalog.h>
#include <string.h>

#include "revolution/BTE/include/bt_target.h"
#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/stack/include/sdp_api.h"

/* SDP trace control block (retail global `sdp_cb`, trace level at 0x4630). */
extern unsigned char sdp_cb[];

/* Externals from sdp_utils.c / bt_trace.c. */
extern UINT8 *sdpu_get_len_from_type(UINT8 *p, UINT8 type, UINT32 *p_len);
extern BOOLEAN sdpu_is_base_uuid(UINT8 *p_uuid);
extern void LogMsg_0(UINT32 trace_set_mask, const char *p_str);
extern void LogMsg_1(UINT32 trace_set_mask, const char *p_str, UINT32 p1);
extern void LogMsg_2(UINT32 trace_set_mask, const char *p_str, UINT32 p1,
                    UINT32 p2);

/* Timer list entry (see gki.h); sizeof == 0x18. */
typedef struct _tle {
    struct _tle *p_next;
    struct _tle *p_prev;
    void *p_cback;
    int ticks;
    void *param;
    UINT16 event;
    UINT8 in_use;
} TIMER_LIST_ENT;

/* Service Search Request PDU id (BT core spec, SDP protocol). */
#define SDP_SVC_SEARCH_REQ 2

/* Response PDU ids dispatched by sdp_disc_server_rsp. */
#define SDP_PDU_SERVICE_SEARCH_RSP      3
#define SDP_PDU_SERVICE_ATTR_RSP        5
#define SDP_PDU_SERVICE_SEARCH_ATTR_RSP 7

/* Maximum continuation state length in a response PDU. */
#define SDP_MAX_CONTINUATION_LEN    16

/* sdp_disconnect() reason code used when no buffer can be allocated. */
#define SDP_DISC_ERR_NO_RESOURCES 6

/* SDP M2 timer: type + timeout passed to btu_start_timer(). */
#define BTU_TTYPE_SDP_M2 5
#define SDP_M2_TIMEOUT 30

/* sdp_cb.max_recs_per_search: 16-bit field at sdp_cb+0x462E (big-endian). */
#define SDP_CB_MAX_RECS_PER_SEARCH (*(UINT16 *)(sdp_cb + 0x462E))

/* ------------------------------------------------------------------------- */
/* Local types and constants (no shared sdpint.h header in the writable      */
/* scope, so the minimum pieces needed by this TU are declared here).        */
/* ------------------------------------------------------------------------- */

/* Discovery sub-states used by tCONN_CB::disc_state. */
enum {
    SDP_DISC_WAIT_CONN        = 0,
    SDP_DISC_WAIT_HANDLES     = 1,
    SDP_DISC_WAIT_ATTR        = 2,
    SDP_DISC_WAIT_SEARCH_ATTR = 3,
    SDP_DISC_WAIT_PASS_THRU   = 4,
    SDP_DISC_WAIT_CANCEL      = 5
};

/* Connection control block - fields are listed in offset order to match the
   retail layout. Trailing fields the matched functions do not touch are kept
   in place so future code in the unit can extend the type without disturbing
   the offsets the matched code already relies on. */
typedef struct
{
    UINT8           con_state;                              // 0x000
    UINT8           con_flags;                              // 0x001
    BD_ADDR         device_address;                         // 0x002
    TIMER_LIST_ENT  timer_entry;                            // 0x008
    UINT16          rem_mtu_size;                           // 0x020
    UINT16          connection_id;                          // 0x022
    UINT16          list_len;                               // 0x024
    UINT8           rsp_list[0x3e8];                        // 0x026 scratchpad buffer
    UINT16          _pad_align;                             // 0x40e
    tSDP_DISCOVERY_DB *p_db;                                // 0x410
    tSDP_DISC_CMPL_CB *p_cb;                                // 0x414
    UINT32          handles[SDP_MAX_DISC_SERVER_RECS];       // 0x418
    UINT16          num_handles;                            // 0x46c
    UINT16          cur_handle;                             // 0x46e
    UINT16          transaction_id;                         // 0x470
    UINT16          disconnect_reason;                      // 0x472
    UINT8           disc_state;                             // 0x474
    UINT8           is_attr_search;                         // 0x475
    UINT16          cont_offset;                            // 0x476
} tCONN_CB;

/* Local mirror of tSDP_DISCOVERY_DB matching the retail layout. The
   sdp_api.h header ships with SDP_RAW_DATA_INCLUDED==TRUE which adds
   raw_data/raw_size/raw_used fields after p_free_mem, shifting the offsets
   the retail code relies on. The retail was compiled without those fields,
   so we redeclare a layout that matches it exactly. Only the fields touched
   by add_record are named; the rest is a single padding field sized to land
   p_free_mem at offset 0x68. */
typedef struct
{
    UINT32          mem_size;                               // 0x00
    UINT32          mem_free;                               // 0x04
    tSDP_DISC_REC   *p_first_rec;                           // 0x08
    UINT8           _pad[0x40];                             // 0x0c (num_uuid_filters + uuid_filters)
    UINT16          num_attr_filters;                       // 0x4c
    UINT16          attr_filters[13];                       // 0x4e (retail SDP_MAX_ATTR_FILTERS = 13)
    UINT8           *p_free_mem;                            // 0x68 in the retail build
} tSDP_DISC_DB;

/* ------------------------------------------------------------------------- */
/* Forward declarations for symbols defined elsewhere in the unit.           */
/* ------------------------------------------------------------------------- */

void sdp_snd_service_search_req(tCONN_CB *p_ccb, UINT8 cont_len, UINT8 *p_cont);
void process_service_search_rsp(tCONN_CB *p_ccb, UINT8 *p_reply, UINT16 len);
void process_service_attr_rsp(tCONN_CB *p_ccb, UINT8 *p_reply, UINT16 len);
void process_service_search_attr_rsp(tCONN_CB *p_ccb, UINT8 *p_reply, UINT16 len);
UINT8 *sdpu_build_attrib_seq(UINT8 *p_out, UINT16 *p_attr, UINT16 num_attrs);
UINT8 *save_attr_seq(tCONN_CB *p_ccb, UINT8 *p, UINT8 *p_msg_end);
UINT8 *sdpu_build_uuid_seq(UINT8 *p_out, UINT16 num_uuids, tSDP_UUID *p_uuid_list);
void sdp_disc_connected(tCONN_CB *p_ccb);
void sdp_disc_server_rsp(tCONN_CB *p_ccb, BT_HDR *p_msg);
tSDP_DISC_REC *add_record(tSDP_DISC_DB *p_db, BD_ADDR p_bda);

/* Externals from sdp_main.c / GKI / L2CAP. */
extern void sdp_disconnect(tCONN_CB *p_ccb, UINT16 reason);
extern void btu_stop_timer(void *p_tle);
extern void *GKI_getpoolbuf(UINT8 pool_id);
extern BOOLEAN L2CA_DataWrite(UINT16 cid, BT_HDR *p_data);
extern void btu_start_timer(TIMER_LIST_ENT *p_tle, UINT16 type, UINT32 timeout);

/* ------------------------------------------------------------------------- */
/* Public SDP discovery entry points.                                        */
/* ------------------------------------------------------------------------- */

/* Called by the SDP main state machine once the L2CAP channel is up. Branches
   into either the service-search-attribute path (combined search) or the
   service-search-then-attribute path (classic two-step discovery). The two
   branches are tail calls so the parent returns straight into the helper. */

#pragma dont_inline on

/* sdp_cb.max_attr_list_size: 16-bit field at sdp_cb+0x462C (big-endian). */
#define SDP_CB_MAX_ATTR_LIST_SIZE (*(UINT16 *)(sdp_cb + 0x462C))

/* Retail build constants (differ from the public bt_target.h/sdp_api.h, so
   the unit redefines them to the retail values). */
#undef SDP_MAX_LIST_BYTE_COUNT
#undef SDP_POOL_ID
#define SDP_MAX_LIST_BYTE_COUNT     1000
#define SDP_POOL_ID                 2
#define SDP_PDU_SERVICE_ATTR_REQ    4
#define SDP_INACT_TIMEOUT           30
#define SDP_L2CAP_MIN_OFFSET        9  /* L2CAP_OFFSET_WO_L2HDR (retail) */
#define BTU_TTYPE_SDP               5

UINT8 *add_attr(UINT8 *p, tSDP_DISC_DB *p_db, tSDP_DISC_REC *p_rec,
                UINT16 attr_id, tSDP_DISC_ATTR *p_parent_attr, UINT8 nest_level)
{
    tSDP_DISC_ATTR *p_attr;
    tSDP_DISC_ATTR *p_tail;
    UINT8 *p_val;
    UINT8 *p_end;
    UINT32 len;
    UINT16 type;
    UINT8 type_byte;
    UINT8 nest;
    UINT8 flag;
    UINT32 size;
    UINT32 value;
    INT32 i;

    type_byte = *p;
    flag = nest_level & 0x80;
    nest = nest_level & 0x7F;
    p_val = sdpu_get_len_from_type(p + 1, type_byte, &len);
    len &= 0xFFF;
    type = (UINT8)((type_byte >> 3) & 0xF);

    size = (len > 4) ? len + 8 : 12;
    size = (size + 3) & ~3;
    if (p_db->mem_free < size) {
        return NULL;
    }

    p_attr = (tSDP_DISC_ATTR *)p_db->p_free_mem;
    p_attr->attr_id = attr_id;
    p_attr->attr_len_type = (UINT16)((type << 12) | (UINT16)len);
    p_attr->p_next_attr = NULL;

    switch (type) {
    case 0:
        break;

    case 1:
        /* Embedded descriptor list: a 2-byte 0x0004 element announces a
           nested sequence; allocate the header here and recurse once. */
        if (flag != 0 && len == 2 &&
            (UINT16)((p_val[0] << 8) + p_val[1]) == 4) {
            p_db->p_free_mem += 12;
            p_db->mem_free -= 12;
            size = 0;
            p_end = p_val + len + 2;
            if (nest >= 5) {
                if (sdp_cb[0x4630] >= 1) {
                    LogMsg_0(0xA0000, "SDP - attr nesting too deep");
                }
                return p_end;
            }
            p_val = add_attr(p_val + 2, p_db, p_rec, 4, p_attr,
                             (UINT8)(nest + 1));
            break;
        }
        /* fall through to the UINT/2's-complement length switch */
    case 2:
        switch (len) {
        case 1:
            p_attr->attr_value.v.u8 = p_val[0];
            p_val += 1;
            break;
        case 2:
            p_attr->attr_value.v.u16 =
                (UINT16)((p_val[0] << 8) + p_val[1]);
            p_val += 2;
            break;
        case 4:
            p_attr->attr_value.v.u32 =
                (UINT32)(((UINT32)p_val[0] << 24) + ((UINT32)p_val[1] << 16)) +
                ((UINT32)((UINT32)p_val[2] << 8) + p_val[3]);
            p_val += 4;
            break;
        default:
            {
                UINT8 *p_dst = p_attr->attr_value.v.array;
                for (i = 0; i < (INT32)len; i++) {
                    *p_dst++ = *p_val++;
                }
            }
            break;
        }
        break;

    case 3:
        switch (len) {
        case 2:
            p_attr->attr_value.v.u16 =
                (UINT16)((p_val[0] << 8) + p_val[1]);
            p_val += 2;
            break;
        case 4:
            value = (UINT32)(((UINT32)p_val[0] << 24) +
                             ((UINT32)p_val[1] << 16)) +
                    (p_val[3] + (UINT32)((UINT32)p_val[2] << 8));
            p_attr->attr_value.v.u32 = value;
            p_val += 4;
            if (value < 0x10000) {
                len = 2;
                p_attr->attr_len_type = (UINT16)((type << 12) | len);
                p_attr->attr_value.v.u16 =
                    (UINT16)p_attr->attr_value.v.u32;
            }
            break;
        case 16:
            if (sdpu_is_base_uuid(p_val)) {
                if (p_val[0] == 0 && p_val[1] == 0) {
                    p_attr->attr_len_type =
                        (UINT16)((p_attr->attr_len_type & ~0x0FFF) | 2);
                    p_attr->attr_value.v.u16 =
                        (UINT16)((p_val[2] << 8) + p_val[3]);
                    p_val += 16;
                } else {
                    p_attr->attr_len_type =
                        (UINT16)((p_attr->attr_len_type & ~0x0FFF) | 4);
                    p_attr->attr_value.v.u32 =
                        (UINT32)(((UINT32)p_val[0] << 24) +
                                 ((UINT32)p_val[1] << 16)) +
                        (p_val[3] + (UINT32)((UINT32)p_val[2] << 8));
                    p_val += 16;
                }
            } else {
                UINT8 *p_dst = p_attr->attr_value.v.array;
                for (i = 0; i < (INT32)len; i++) {
                    *p_dst++ = *p_val++;
                }
            }
            break;
        default:
            if (sdp_cb[0x4630] >= 2) {
                LogMsg_1(0xA0001, "SDP - bad len in UUID attr: %d", len);
            }
            return p_val + len;
        }
        break;

    case 6:
    case 7:
        /* Data element sequence: allocate the header now, then parse the
           children into it recursively. */
        p_db->p_free_mem += 12;
        p_db->mem_free -= 12;
        size = 0;
        p_end = p_val + len;
        if (nest >= 5) {
            if (sdp_cb[0x4630] >= 1) {
                LogMsg_0(0xA0000, "SDP - attr nesting too deep");
            }
            return p_end;
        }
        if (flag != 0 || attr_id == 0x0D) {
            nest |= 0x80;
        }
        while (p_val < p_end) {
            p_val = add_attr(p_val, p_db, p_rec, 0, p_attr,
                             (UINT8)(nest + 1));
            if (p_val == NULL) {
                return NULL;
            }
        }
        break;

    case 4:
    case 8:
        {
            UINT8 *p_dst = p_attr->attr_value.v.array;
            for (i = 0; i < (INT32)len; i++) {
                *p_dst++ = *p_val++;
            }
        }
        break;

    case 5:
        switch (len) {
        case 1:
            p_attr->attr_value.v.u8 = p_val[0];
            p_val += 1;
            break;
        default:
            if (sdp_cb[0x4630] >= 2) {
                LogMsg_1(0xA0001, "SDP - bad len in boolean attr: %d", len);
            }
            return p_val + len;
        }
        break;
    }

    /* Account for the allocated size and link the new attribute into the
       parent record's attribute list (or the record's head when there is no
       parent). */
    p_db->p_free_mem += size;
    p_db->mem_free -= size;
    if (p_parent_attr == NULL) {
        if (p_rec->p_first_attr == NULL) {
            p_rec->p_first_attr = p_attr;
        } else {
            p_tail = p_rec->p_first_attr;
            while (p_tail->p_next_attr != NULL) {
                p_tail = p_tail->p_next_attr;
            }
            p_tail->p_next_attr = p_attr;
        }
    } else {
        if (p_parent_attr->attr_value.v.p_sub_attr == NULL) {
            p_parent_attr->attr_value.v.p_sub_attr = p_attr;
        } else {
            p_tail = p_parent_attr->attr_value.v.p_sub_attr;
            while (p_tail->p_next_attr != NULL) {
                p_tail = p_tail->p_next_attr;
            }
            p_tail->p_next_attr = p_attr;
        }
    }

    return p_val;
}
tSDP_DISC_REC *add_record(tSDP_DISC_DB *p_db, BD_ADDR p_bda)
{
    tSDP_DISC_REC *p_rec;

    if (p_db->mem_free < sizeof(tSDP_DISC_REC))
        return NULL;

    p_rec = (tSDP_DISC_REC *) p_db->p_free_mem;
    p_db->p_free_mem += sizeof(tSDP_DISC_REC);
    p_db->mem_free   -= sizeof(tSDP_DISC_REC);

    p_rec->p_first_attr = NULL;
    p_rec->p_next_rec   = NULL;
    memcpy(p_rec->remote_bd_addr, p_bda, BD_ADDR_LEN);

    if (p_db->p_first_rec == NULL) {
        p_db->p_first_rec = p_rec;
    } else {
        tSDP_DISC_REC *p_rec1 = p_db->p_first_rec;
        while (p_rec1->p_next_rec != NULL)
            p_rec1 = p_rec1->p_next_rec;
        p_rec1->p_next_rec = p_rec;
    }

    return p_rec;
}

UINT8 *save_attr_seq(tCONN_CB *p_ccb, UINT8 *p, UINT8 *p_msg_end)
{
    UINT32 seq_len;
    UINT32 attr_len;
    UINT16 attr_id;
    UINT8 type;
    UINT8 *p_seq_end;
    tSDP_DISC_REC *p_rec;

    type = *p++;

    if (type >> 3 != 6)
    {
        if (sdp_cb[0x4630] >= 2)
            LogMsg_1(0xA0001, "SDP - Wrong type: 0x%02x in attr_rsp", type);
        return NULL;
    }

    p = sdpu_get_len_from_type(p, type, &seq_len);
    if (p + seq_len > p_msg_end)
    {
        if (sdp_cb[0x4630] >= 2)
            LogMsg_1(0xA0001, "SDP - Bad len in attr_rsp %d", seq_len);
        return NULL;
    }

    p_rec = (tSDP_DISC_REC *)add_record((tSDP_DISC_DB *)p_ccb->p_db,
                                        p_ccb->device_address);
    if (p_rec == NULL)
    {
        if (sdp_cb[0x4630] >= 2)
            LogMsg_0(0xA0001, "SDP - DB full");
        return NULL;
    }

    p_seq_end = p + seq_len;

    while (p < p_seq_end)
    {
        type = *p++;
        p = sdpu_get_len_from_type(p, type, &attr_len);

        if (type >> 3 != 1 || attr_len != 2)
        {
            if (sdp_cb[0x4630] >= 2)
                LogMsg_2(0xA0001,
                         "SDP - Bad type: 0x%02x or len: %d in attr_rsp", type,
                         attr_len);
            return NULL;
        }

        attr_id = (UINT16)((p[0] << 8) + p[1]);
        p += 2;

        p = (UINT8 *)add_attr(p, (tSDP_DISC_DB *)p_ccb->p_db, p_rec, attr_id,
                              NULL, 0);
        if (p == NULL)
        {
            if (sdp_cb[0x4630] >= 2)
                LogMsg_0(0xA0001, "SDP - DB full");
            return NULL;
        }
    }

    return p;
}
void process_service_search_attr_rsp(tCONN_CB *p_ccb, UINT8 *p_reply, UINT16 len) {}
void process_service_attr_rsp(tCONN_CB *p_ccb, UINT8 *p_reply, UINT16 len)
{
    UINT8 *p_start;
    UINT8 *p_param_len;
    UINT16 param_len;
    UINT16 list_byte_count;
    BOOLEAN cont_request_needed = FALSE;

    if (p_reply) {
        p_reply += 4;

        BE_STREAM_TO_UINT16(list_byte_count, p_reply);

        if (p_ccb->list_len + list_byte_count > SDP_MAX_LIST_BYTE_COUNT) {
            sdp_disconnect(p_ccb, SDP_INVALID_PDU_SIZE);
            return;
        }

        memcpy(&p_ccb->rsp_list[p_ccb->list_len], p_reply, list_byte_count);
        p_ccb->list_len += list_byte_count;
        p_reply += list_byte_count;

        if (*p_reply) {
            if (*p_reply > SDP_MAX_CONTINUATION_LEN) {
                sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
                return;
            }
            cont_request_needed = TRUE;
        } else {
            if (!save_attr_seq(p_ccb, p_ccb->rsp_list, p_ccb->rsp_list + p_ccb->list_len)) {
                sdp_disconnect(p_ccb, SDP_DB_FULL);
                return;
            }
            p_ccb->list_len = 0;
            p_ccb->cur_handle++;
        }
    }

    if (p_ccb->cur_handle < p_ccb->num_handles) {
        BT_HDR *p_msg = (BT_HDR *)GKI_getpoolbuf(SDP_POOL_ID);
        UINT8 *p;

        if (!p_msg) {
            sdp_disconnect(p_ccb, SDP_NO_RESOURCES);
            return;
        }

        p_msg->offset = SDP_L2CAP_MIN_OFFSET;
        p = p_start = (UINT8 *)(p_msg + 1) + SDP_L2CAP_MIN_OFFSET;

        UINT8_TO_BE_STREAM(p, SDP_PDU_SERVICE_ATTR_REQ);
        UINT16_TO_BE_STREAM(p, p_ccb->transaction_id);
        p_ccb->transaction_id++;

        p_param_len = p;
        p += 2;

        UINT32_TO_BE_STREAM(p, p_ccb->handles[p_ccb->cur_handle]);
        UINT16_TO_BE_STREAM(p, SDP_CB_MAX_ATTR_LIST_SIZE);

        if (p_ccb->p_db->num_attr_filters)
            p = sdpu_build_attrib_seq(p, p_ccb->p_db->attr_filters,
                                      p_ccb->p_db->num_attr_filters);
        else
            p = sdpu_build_attrib_seq(p, NULL, 0);

        if (cont_request_needed) {
            memcpy(p, p_reply, *p_reply + 1);
            p += *p_reply + 1;
        } else {
            UINT8_TO_BE_STREAM(p, 0);
        }

        param_len = (UINT16)(p - p_param_len - 2);
        UINT16_TO_BE_STREAM(p_param_len, param_len);

        p_msg->len = (UINT16)(p - p_start);

        L2CA_DataWrite(p_ccb->connection_id, p_msg);

        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_SDP, SDP_INACT_TIMEOUT);
    } else {
        sdp_disconnect(p_ccb, SDP_SUCCESS);
    }
}
void process_service_search_rsp(tCONN_CB *p_ccb, UINT8 *p_reply, UINT16 len)
{
    UINT16 num_handles;
    UINT16 xx;
    UINT8 *p = p_reply + 8;

    /* Number of records in this response */
    num_handles = (UINT16)((p_reply[6] << 8) + p_reply[7]);
    xx = p_ccb->num_handles;
    num_handles = (UINT16)(xx + num_handles);
    p_ccb->num_handles = num_handles;

    if (num_handles == 0) {
        if (sdp_cb[0x4630] >= 2) {
            LogMsg_0(0xA0001, "SDP - Rcvd ServiceSearchRsp, no matches");
        }
        sdp_disconnect(p_ccb, SDP_NO_RECS_MATCH);
        return;
    }

    if (num_handles > SDP_CB_MAX_RECS_PER_SEARCH) {
        p_ccb->num_handles = SDP_CB_MAX_RECS_PER_SEARCH;
    }

    /* Copy the new handles into the connection block */
    for (; xx < p_ccb->num_handles; xx++) {
        BE_STREAM_TO_UINT32(p_ccb->handles[xx], p);
    }

    if (*p) {
        if (*p > SDP_MAX_CONTINUATION_LEN) {
            sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
            return;
        }
        sdp_snd_service_search_req(p_ccb, *p, p + 1);
    } else {
        p_ccb->disc_state = SDP_DISC_WAIT_ATTR;
        process_service_attr_rsp(p_ccb, NULL, 0);
    }
}
void sdp_disc_server_rsp(tCONN_CB *p_ccb, BT_HDR *p_msg)
{
    UINT32 len;
    UINT8 pdu_id;
    UINT8 err = TRUE;
    UINT8 *p;

    /* Stop the M2 timer */
    btu_stop_timer(&p_ccb->timer_entry);

    p = (UINT8 *)p_msg + p_msg->offset;
    pdu_id = p[8];

    len = p_msg->len - 1;
    p_msg->len = len;

    switch (pdu_id) {
    case SDP_PDU_SERVICE_SEARCH_RSP:
        if (p_ccb->disc_state == SDP_DISC_WAIT_HANDLES) {
            process_service_search_rsp(p_ccb, p + 9, len);
            err = FALSE;
        }
        break;

    case SDP_PDU_SERVICE_ATTR_RSP:
        if (p_ccb->disc_state == SDP_DISC_WAIT_ATTR) {
            process_service_attr_rsp(p_ccb, p + 9, len);
            err = FALSE;
        }
        break;

    case SDP_PDU_SERVICE_SEARCH_ATTR_RSP:
        if (p_ccb->disc_state == SDP_DISC_WAIT_SEARCH_ATTR) {
            process_service_search_attr_rsp(p_ccb, p + 9, len);
            err = FALSE;
        }
        break;
    }

    if (err) {
        if (sdp_cb[0x4630] >= 2) {
            LogMsg_2(0xA0001, "SDP - Unexp. PDU: %d in state: %d", pdu_id,
                     p_ccb->disc_state);
        }
        sdp_disconnect(p_ccb, SDP_GENERIC_ERROR);
    }
}
void sdp_disc_connected(tCONN_CB *p_ccb)
{
    if (p_ccb->is_attr_search) {
        p_ccb->disc_state = SDP_DISC_WAIT_SEARCH_ATTR;
        process_service_search_attr_rsp(p_ccb, NULL, 0);
    } else {
        p_ccb->num_handles = 0;
        sdp_snd_service_search_req(p_ccb, 0, NULL);
    }
}
void sdp_snd_service_search_req(tCONN_CB *p_ccb, UINT8 cont_len, UINT8 *p_cont)
{
    UINT8 *p;
    UINT8 *p_start;
    UINT8 *p_rsp_start;
    BT_HDR *p_buf;

    /* Service search PDU */
    p_buf = (BT_HDR *)GKI_getpoolbuf(2);
    if (p_buf == NULL) {
        sdp_disconnect(p_ccb, SDP_DISC_ERR_NO_RESOURCES);
        return;
    }

    p_buf->offset = 9;
    p_rsp_start = (UINT8 *)(p_buf + 1) + p_buf->offset;
    p = p_rsp_start;

    /* First, build the header */
    *p++ = SDP_SVC_SEARCH_REQ;
    *p++ = (UINT8)(p_ccb->transaction_id >> 8);
    *p++ = (UINT8)p_ccb->transaction_id;
    p_ccb->transaction_id++;

    /* Remember where the parameter length goes */
    p_start = p;
    p += 2;

    /* Build the UUID sequence */
    p = sdpu_build_uuid_seq(p, p_ccb->p_db->num_uuid_filters,
                            p_ccb->p_db->uuid_filters);

    /* Add max service record count */
    *p++ = (UINT8)(SDP_CB_MAX_RECS_PER_SEARCH >> 8);
    *p++ = (UINT8)SDP_CB_MAX_RECS_PER_SEARCH;

    /* Add continuation state */
    *p++ = cont_len;
    if (cont_len && p_cont) {
        memcpy(p, p_cont, cont_len);
        p += cont_len;
    }

    /* Set the parameter length */
    p_start[0] = (UINT8)(((UINT32)(p - p_start - 2)) >> 8);
    p_start[1] = (UINT8)(p - p_start - 2);

    /* Set the SDP state */
    p_ccb->disc_state = SDP_DISC_WAIT_HANDLES;

    /* Set the length of the SDP data */
    p_buf->len = (UINT16)(p - p_rsp_start);

    /* Send the buffer */
    L2CA_DataWrite(p_ccb->connection_id, p_buf);

    /* Start the M2 timeout */
    btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_SDP_M2, SDP_M2_TIMEOUT);
}
UINT8 *sdpu_build_uuid_seq(UINT8 *p_out, UINT16 num_uuids, tSDP_UUID *p_uuid_list)
{
    UINT16 xx;
    UINT8 *p_len;

    /* First thing is the data element header */
    UINT8_TO_BE_STREAM(p_out, (DATA_ELE_SEQ_DESC_TYPE << 3) | SIZE_IN_NEXT_BYTE);

    /* Remember where the length goes. Leave space for it. */
    p_len = p_out;
    p_out += 1;

    /* Now, loop through and put in all the UUID(s) */
    for (xx = 0; xx < num_uuids; xx++, p_uuid_list++) {
        if (p_uuid_list->len == LEN_UUID_16) {
            /* 16-bit UUID */
            UINT8_TO_BE_STREAM(p_out, (UUID_DESC_TYPE << 3) | SIZE_TWO_BYTES);
            UINT16_TO_BE_STREAM(p_out, p_uuid_list->uu.uuid16);
        } else if (p_uuid_list->len == LEN_UUID_32) {
            /* 32-bit UUID */
            UINT8_TO_BE_STREAM(p_out, (UUID_DESC_TYPE << 3) | SIZE_FOUR_BYTES);
            UINT32_TO_BE_STREAM(p_out, p_uuid_list->uu.uuid32);
        } else {
            /* 128-bit UUID */
            UINT8_TO_BE_STREAM(p_out, (UUID_DESC_TYPE << 3) | SIZE_SIXTEEN_BYTES);
            ARRAY_TO_BE_STREAM(p_out, p_uuid_list->uu.uuid128, p_uuid_list->len);
        }
    }

    /* Now, put in the length */
    xx = (UINT16)(p_out - p_len - 1);
    UINT8_TO_BE_STREAM(p_len, xx);

    return p_out;
}
#pragma dont_inline off
