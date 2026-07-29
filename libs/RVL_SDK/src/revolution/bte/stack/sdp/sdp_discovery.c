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
    UINT8           _pad_timer[0x18];                       // 0x008 TIMER_LIST_ENT slot
    UINT16          rem_mtu_size;                           // 0x020
    UINT16          connection_id;                          // 0x022
    UINT16          list_len;                               // 0x024
    UINT8           _pad_rsp[0x3e8];                        // 0x026 scratchpad buffer
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
    UINT8           _pad[0x5c];                             // 0x0c (uuid_filters + num_attr_filters + attr_filters)
    UINT8           *p_free_mem;                            // 0x68 in the retail build
} tSDP_DISC_DB;

/* ------------------------------------------------------------------------- */
/* Forward declarations for symbols defined elsewhere in the unit.           */
/* ------------------------------------------------------------------------- */

void sdp_snd_service_search_req(tCONN_CB *p_ccb, UINT8 cont_len, UINT8 *p_cont);
void process_service_search_attr_rsp(tCONN_CB *p_ccb, UINT8 *p_reply, UINT16 len);

/* ------------------------------------------------------------------------- */
/* Public SDP discovery entry points.                                        */
/* ------------------------------------------------------------------------- */

/* Called by the SDP main state machine once the L2CAP channel is up. Branches
   into either the service-search-attribute path (combined search) or the
   service-search-then-attribute path (classic two-step discovery). The two
   branches are tail calls so the parent returns straight into the helper. */
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

/* Allocates a new record slot from the discovery database scratchpad and
   appends it to the linked list. Sized to match the retail allocation step
   (sizeof(tSDP_DISC_REC) == 0x14 on PPC). */
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

/* ------------------------------------------------------------------------- */
/* Remaining symbols are placeholders that will be matched in follow-on      */
/* sessions. They are kept here as non-inline stubs so the unit links and the */
/* calls in matched functions survive the -O4 inliner.                       */
/* ------------------------------------------------------------------------- */

#pragma dont_inline on

void sdpu_build_uuid_seq(UINT8 *p_out, UINT16 num_uuids, tSDP_UUID *p_uuid_list) {}

void sdp_snd_service_search_req(tCONN_CB *p_ccb, UINT8 cont_len, UINT8 *p_cont) {}

void sdp_disc_server_rsp(tCONN_CB *p_ccb, BT_HDR *p_msg) {}

void process_service_search_rsp(tCONN_CB *p_ccb, UINT8 *p_reply, UINT16 len) {}

void process_service_attr_rsp(tCONN_CB *p_ccb, UINT8 *p_reply, UINT16 len) {}

void process_service_search_attr_rsp(tCONN_CB *p_ccb, UINT8 *p_reply, UINT16 len) {}

UINT8 *save_attr_seq(tCONN_CB *p_ccb, UINT8 *p, UINT8 *p_msg_end) { return NULL; }

UINT8 *add_attr(UINT8 *p, tSDP_DISC_DB *p_db, tSDP_DISC_REC *p_rec,
                UINT16 attr_id, tSDP_DISC_ATTR *p_parent_attr, UINT8 nest_level) { return NULL; }

#pragma dont_inline off
