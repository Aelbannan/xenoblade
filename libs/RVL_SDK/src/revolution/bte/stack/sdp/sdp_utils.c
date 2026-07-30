/* Decompiled SDP utility helpers for RVL_SDK/src/revolution/bte/stack/sdp/sdp_utils.
 *
 * This translation unit implements the Broadcom BTE SDP stack utilities:
 * connection control block (CCB) management, attribute/UUID sequence builders
 * and parsers, UUID comparison helpers, and the SDP error response builder.
 *
 * The retail binary was built from the RVL SDK's private sdp_int.h, which is
 * not shipped with this repository.  The internal structures below mirror the
 * retail layout exactly (offsets recovered from the retail assembly).
 */

#include <harness_catalog.h>
#include <string.h>

#include <revolution/BTE/include/bt_target.h>
#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/stack/include/sdpdefs.h>
#include <revolution/BTE/stack/include/sdp_api.h>
#include <revolution/BTE/stack/include/l2c_api.h>
#include <revolution/BTE/stack/include/btu.h>
#include <revolution/BTE/include/bt_trace.h>

/* ------------------------------------------------------------------------- */
/* CCB / SDP control block (retail sdp_int.h layout)                         */
/* ------------------------------------------------------------------------- */

/* Connection states */
#define CCB_STATE_IDLE          0

/* Maximum number of UUIDs / attribute ranges parsed out of one sequence. */
#define SDPU_MAX_SEQ_ENTRIES    16

/* SDP error response PDU id. */
#define SDP_PDU_ERROR_RESPONSE  0x01

/* The retail RVL SDK used the legacy BTE trace encoding: error messages gate
 * at trace level 2 with trace type bit 0x1, warning messages gate at level 1
 * with type bit 0x0.  The reconstructed repository headers swap both pairs,
 * so the trace gates in this unit are spelled out with retail constants. */
#define SDPU_TRACE_LEVEL_ERROR      2
#define SDPU_TRACE_LEVEL_WARNING    1
#define SDPU_TRACE_TYPE_ERROR       1
#define SDPU_TRACE_TYPE_WARNING     0

/* Connection control block - 0x478 bytes in the retail build.  Only the
 * fields touched by this unit are named; the rest is kept in place so the
 * layout matches the retail offsets exactly. */
typedef struct
{
    UINT8           con_state;                          /* 0x000 */
    UINT8           con_flags;                          /* 0x001 */
    BD_ADDR         device_address;                     /* 0x002 */
    TIMER_LIST_ENT  timer;                              /* 0x008 */
    UINT16          rem_mtu_size;                       /* 0x020 */
    UINT16          connection_id;                      /* 0x022 */
    UINT8           _pad[0x478 - 0x024];                /* 0x024 */
} tCONN_CB;

/* SDP global control block.  The CCB array starts at offset 0x3c and the
 * trace level lives at offset 0x4630 in the retail binary. */
typedef struct
{
    UINT8       _pad0[0x3c];
    tCONN_CB    ccb[SDP_MAX_CONNECTIONS];
    UINT8       _pad1[0x4630 - 0x3c - (SDP_MAX_CONNECTIONS * 0x478)];
    UINT8       trace_level;                            /* 0x4630 */
} tSDP_CB;

extern tSDP_CB sdp_cb;

/* ------------------------------------------------------------------------- */
/* Attribute database types (retail sdp_int.h layout)                        */
/* ------------------------------------------------------------------------- */

/* Server-side attribute description, as stored in the SDP database. */
typedef struct
{
    INT32           attr_len;                           /* 0x00 */
    void            *attr_value;                        /* 0x04 */
    UINT16          attr_id;                            /* 0x08 */
    UINT8           attr_type;                          /* 0x0A */
} tSDP_ATTRIBUTE;

/* One parsed UUID from a search sequence: 18-byte element (UINT16 length
 * followed by the big-endian UUID bytes), stride 0x12. */
typedef struct
{
    UINT16          len;                                /* 0x00 */
    UINT8           uu[16];                             /* 0x02 */
} tSDPU_UUID;

typedef struct
{
    UINT16          num_uuids;                          /* 0x00 */
    tSDPU_UUID      uuid_entry[SDPU_MAX_SEQ_ENTRIES];   /* 0x02 */
} tSDP_UUID_SEQ;

/* One parsed attribute-id range from an attribute sequence. */
typedef struct
{
    UINT16          start;                              /* 0x00 */
    UINT16          end;                                /* 0x02 */
} tSDPU_ATTR_RANGE;

typedef struct
{
    UINT16          num_attrs;                          /* 0x00 */
    tSDPU_ATTR_RANGE attr[SDPU_MAX_SEQ_ENTRIES];        /* 0x02 */
} tSDP_ATTR_SEQ;

/* Discovery database view used by sdpu_sort_attr_list: attr_list sits at
 * offset 0x4E in the retail tSDP_DISCOVERY_DB. */
typedef struct
{
    UINT8           _pad[0x4E];
    UINT16          attr_list[SDPU_MAX_SEQ_ENTRIES];
} tSDPU_DISC_DB;

/* The Bluetooth "base" UUID 00000000-0000-1000-8000-00805F9B34FB. */
const UINT8 sdp_base_uuid[LEN_UUID_128] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
                                           0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};

/* ------------------------------------------------------------------------- */
/* CCB management                                                            */
/* ------------------------------------------------------------------------- */

/*******************************************************************************
**
** Function         sdpu_find_ccb_by_cid
**
** Description      Search the CCB table for an entry with the passed CID.
**
** Returns          CCB address if found, else NULL.
**
*******************************************************************************/
tCONN_CB *sdpu_find_ccb_by_cid (UINT16 cid)
{
    tCONN_CB *p_ccb;
    int xx;

    for (xx = 0, p_ccb = &sdp_cb.ccb[0]; xx < SDP_MAX_CONNECTIONS; xx++, p_ccb++)
    {
        if ((p_ccb->con_state != CCB_STATE_IDLE) && (p_ccb->connection_id == cid))
            return (p_ccb);
    }

    return (NULL);
}

/*******************************************************************************
**
** Function         sdpu_allocate_ccb
**
** Description      Allocate a new CCB.
**
** Returns          CCB address if OK, else NULL.
**
*******************************************************************************/
tCONN_CB *sdpu_allocate_ccb (void)
{
    tCONN_CB *p_ccb;
    int xx;

    for (xx = 0, p_ccb = &sdp_cb.ccb[0]; xx < SDP_MAX_CONNECTIONS; xx++, p_ccb++)
    {
        if (p_ccb->con_state == CCB_STATE_IDLE)
        {
            memset (p_ccb, 0, sizeof (tCONN_CB));
            p_ccb->timer.param = (TIMER_PARAM_TYPE)p_ccb;
            return (p_ccb);
        }
    }

    return (NULL);
}

/*******************************************************************************
**
** Function         sdpu_release_ccb
**
** Description      Release a CCB.
**
** Returns          void
**
*******************************************************************************/
void sdpu_release_ccb (tCONN_CB *p_ccb)
{
    btu_stop_timer (&p_ccb->timer);
    p_ccb->con_state = CCB_STATE_IDLE;
}

/* ------------------------------------------------------------------------- */
/* Attribute sequence builders                                               */
/* ------------------------------------------------------------------------- */

/*******************************************************************************
**
** Function         sdpu_build_attrib_seq
**
** Description      Build an attribute ID sequence from the passed list.
**                  If p_attr is NULL a single "all attributes" range is used.
**
** Returns          Pointer to the next free byte in the output buffer.
**
*******************************************************************************/
UINT8 *sdpu_build_attrib_seq (UINT8 *p_out, UINT16 *p_attr, UINT16 num_attrs)
{
    UINT16   xx;
    UINT16   seq_len;

    /* First thing is the data element sequence header. */
    if (p_attr == NULL)
        seq_len = 5;
    else
        seq_len = num_attrs * 3;

    if (seq_len > 255)
    {
        *p_out++ = (DATA_ELE_SEQ_DESC_TYPE << 3) | SIZE_IN_NEXT_WORD;
        *p_out++ = (UINT8)(seq_len >> 8);
        *p_out++ = (UINT8)seq_len;
    }
    else
    {
        *p_out++ = (DATA_ELE_SEQ_DESC_TYPE << 3) | SIZE_IN_NEXT_BYTE;
        *p_out++ = (UINT8)seq_len;
    }

    if (p_attr == NULL)
    {
        /* No attributes specified: return everything (0x0000 - 0xFFFF). */
        *p_out++ = (UINT_DESC_TYPE << 3) | SIZE_FOUR_BYTES;
        *p_out++ = 0;
        *p_out++ = 0;
        *p_out++ = 0xff;
        *p_out++ = 0xff;
    }
    else
    {
        for (xx = 0; xx < num_attrs; xx++)
        {
            *p_out++ = (UINT_DESC_TYPE << 3) | SIZE_TWO_BYTES;
            *p_out++ = (UINT8)(*p_attr >> 8);
            *p_out++ = (UINT8)*p_attr;
            p_attr++;
        }
    }

    return (p_out);
}

/*******************************************************************************
**
** Function         sdpu_build_attrib_entry
**
** Description      Build one attribute entry (attribute id + value).
**
** Returns          Pointer to the next free byte in the output buffer.
**
*******************************************************************************/
UINT8 *sdpu_build_attrib_entry (UINT8 *p_out, tSDP_ATTRIBUTE *p_attr)
{
    UINT8   *p_data;
    INT32    xx;

    /* First thing is the attribute ID (unsigned 16-bit integer). */
    *p_out++ = (UINT_DESC_TYPE << 3) | SIZE_TWO_BYTES;
    *p_out++ = (UINT8)(p_attr->attr_id >> 8);
    *p_out++ = (UINT8)p_attr->attr_id;

    switch (p_attr->attr_type)
    {
    case TEXT_STR_DESC_TYPE:        /* These types carry an explicit length */
    case DATA_ELE_SEQ_DESC_TYPE:
    case DATA_ELE_ALT_DESC_TYPE:
    case URL_DESC_TYPE:
        *p_out++ = (UINT8)((p_attr->attr_type << 3) | SIZE_IN_NEXT_BYTE);
        *p_out++ = (UINT8)p_attr->attr_len;
        p_data = p_out;
        break;

    default:
        switch (p_attr->attr_len)
        {
        case 1:
            *p_out++ = (UINT8)((p_attr->attr_type << 3) | SIZE_ONE_BYTE);
            p_data = p_out;
            break;
        case 2:
            *p_out++ = (UINT8)((p_attr->attr_type << 3) | SIZE_TWO_BYTES);
            p_data = p_out;
            break;
        case 4:
            *p_out++ = (UINT8)((p_attr->attr_type << 3) | SIZE_FOUR_BYTES);
            p_data = p_out;
            break;
        case 8:
            *p_out++ = (UINT8)((p_attr->attr_type << 3) | SIZE_EIGHT_BYTES);
            p_data = p_out;
            break;
        case 16:
            *p_out++ = (UINT8)((p_attr->attr_type << 3) | SIZE_SIXTEEN_BYTES);
            p_data = p_out;
            break;
        default:
            *p_out++ = (UINT8)((p_attr->attr_type << 3) | SIZE_IN_NEXT_BYTE);
            *p_out++ = (UINT8)p_attr->attr_len;
            p_data = p_out;
            break;
        }
        break;
    }

    for (xx = 0; xx < p_attr->attr_len; xx++)
        *p_data++ = ((UINT8 *)p_attr->attr_value)[xx];

    return (p_data);
}

/*******************************************************************************
**
** Function         sdpu_build_n_send_error
**
** Description      Build an SDP error response PDU and send it over L2CAP.
**
** Returns          void
**
*******************************************************************************/
void sdpu_build_n_send_error (tCONN_CB *p_ccb, UINT16 trans_num, UINT16 error_code,
                              char *p_error_text)
{
    BT_HDR  *p_buf;
    UINT8   *p;
    UINT8   *p_start;
    UINT8   *p_len;

    if (sdp_cb.trace_level >= SDPU_TRACE_LEVEL_ERROR)
        LogMsg_2 (TRACE_CTRL_GENERAL | TRACE_LAYER_SDP | TRACE_ORG_STACK | SDPU_TRACE_TYPE_ERROR,
                  "SDP - sdpu_build_n_send_error  code: 0x%x  CID: 0x%x",
                  (UINT32)error_code, (UINT32)p_ccb->connection_id);

    /* Get a buffer to hold the response. */
    if ((p_buf = (BT_HDR *)GKI_getpoolbuf (L2CAP_CMD_POOL_ID)) == NULL)
    {
        if (sdp_cb.trace_level >= SDPU_TRACE_LEVEL_WARNING)
            LogMsg_0 (TRACE_CTRL_GENERAL | TRACE_LAYER_SDP | TRACE_ORG_STACK | SDPU_TRACE_TYPE_WARNING,
                      "SDP - no buf for err msg");
        return;
    }

    p_buf->offset = 9;                      /* retail L2CAP_MIN_OFFSET */
    p       = (UINT8 *)(p_buf + 1) + p_buf->offset;
    p_start = p;
    p_len   = p + 3;                        /* error parameter length field */

    *p++ = SDP_PDU_ERROR_RESPONSE;
    *p++ = (UINT8)(trans_num >> 8);
    *p++ = (UINT8)trans_num;
    p   += 2;                               /* skip the parameter length */
    *p++ = (UINT8)(error_code >> 8);
    *p++ = (UINT8)error_code;

    /* Append the optional error text. */
    if (p_error_text != NULL)
    {
        int xx;

        for (xx = 0; xx < (int)strlen (p_error_text); xx++)
            *p++ = *p_error_text++;
    }

    /* Fill in the lengths and send. */
    p_len[0]   = (UINT8)(((p - p_len) - 2) >> 8);
    p_len[1]   = (UINT8)((p - p_len) - 2);
    p_buf->len = (UINT16)(p - p_start);

    L2CA_DataWrite (p_ccb->connection_id, p_buf);
}

/* ------------------------------------------------------------------------- */
/* Sequence parsers                                                          */
/* ------------------------------------------------------------------------- */

/*******************************************************************************
**
** Function         sdpu_extract_uid_seq
**
** Description      Extract a UUID sequence (service search pattern) from a
**                  received PDU.
**
** Returns          Pointer to the byte after the sequence, or NULL on error.
**
*******************************************************************************/
UINT8 *sdpu_extract_uid_seq (UINT8 *p, UINT16 param_len, tSDP_UUID_SEQ *p_seq)
{
    UINT8   *p_end;
    UINT32   seq_len;
    INT32    uuid_len;
    UINT8    type;
    int      xx;

    p_seq->num_uuids = 0;

    /* The sequence header must be a data element sequence. */
    type = *p++;
    if ((type >> 3) != DATA_ELE_SEQ_DESC_TYPE)
        return (NULL);

    switch (type & 7)
    {
    case SIZE_TWO_BYTES:
        seq_len = 2;
        break;
    case SIZE_FOUR_BYTES:
        seq_len = 4;
        break;
    case SIZE_SIXTEEN_BYTES:
        seq_len = 16;
        break;
    case SIZE_IN_NEXT_BYTE:
        seq_len = *p++;
        break;
    case SIZE_IN_NEXT_WORD:
        seq_len = (UINT16)((*p << 8) + *(p + 1));
        p += 2;
        break;
    case SIZE_IN_NEXT_LONG:
        seq_len = (*p << 24) + (*(p + 1) << 16) + (*(p + 2) << 8) + *(p + 3);
        p += 4;
        break;
    default:
        return (NULL);
    }

    if (seq_len >= param_len)
        return (NULL);

    p_end = p + seq_len;

    /* Now pull out each UUID element. */
    while (p < p_end)
    {
        type = *p++;
        if ((type >> 3) != UUID_DESC_TYPE)
            return (NULL);

        switch (type & 7)
        {
        case SIZE_TWO_BYTES:
            uuid_len = 2;
            break;
        case SIZE_FOUR_BYTES:
            uuid_len = 4;
            break;
        case SIZE_SIXTEEN_BYTES:
            uuid_len = 16;
            break;
        case SIZE_IN_NEXT_BYTE:
            uuid_len = *p++;
            break;
        case SIZE_IN_NEXT_WORD:
            uuid_len = (UINT16)((*p << 8) + *(p + 1));
            p += 2;
            break;
        case SIZE_IN_NEXT_LONG:
            uuid_len = (*p << 24) + (*(p + 1) << 16) + (*(p + 2) << 8) + *(p + 3);
            p += 4;
            break;
        default:
            return (NULL);
        }

        if ((uuid_len == LEN_UUID_16) || (uuid_len == LEN_UUID_32) || (uuid_len == LEN_UUID_128))
        {
            p_seq->uuid_entry[p_seq->num_uuids].len = (UINT16)uuid_len;

            for (xx = 0; xx < uuid_len; xx++)
                p_seq->uuid_entry[p_seq->num_uuids].uu[xx] = *p++;

            p_seq->num_uuids++;
            if (p_seq->num_uuids >= SDPU_MAX_SEQ_ENTRIES)
                return (NULL);
        }
        else
            return (NULL);
    }

    /* The sequence must have consumed exactly the advertised length. */
    if (p != p_end)
        return (NULL);

    return (p);
}

/*******************************************************************************
**
** Function         sdpu_extract_attr_seq
**
** Description      Extract an attribute ID sequence (attribute ID list) from
**                  a received PDU.
**
** Returns          Pointer to the byte after the sequence.
**
*******************************************************************************/
UINT8 *sdpu_extract_attr_seq (UINT8 *p, UINT16 param_len, tSDP_ATTR_SEQ *p_seq)
{
    UINT8   *p_end;
    UINT32   seq_len;
    INT32    attr_len;
    UINT8    type;

    p_seq->num_attrs = 0;

    /* The sequence header must be a data element sequence. */
    type = *p;
    if ((type >> 3) != DATA_ELE_SEQ_DESC_TYPE)
        return (p + 1);

    switch (type & 7)
    {
    case SIZE_IN_NEXT_BYTE:
        seq_len = *(p + 1);
        p += 2;
        break;
    case SIZE_IN_NEXT_WORD:
        seq_len = (UINT16)((*(p + 1) << 8) + *(p + 2));
        p += 3;
        break;
    case SIZE_IN_NEXT_LONG:
        seq_len = (*(p + 1) << 24) + (*(p + 2) << 16) + (*(p + 3) << 8) + *(p + 4);
        p += 5;
        break;
    default:
        return (p + 1);
    }

    if (seq_len > param_len)
        return (p);

    p_end = p + seq_len;

    /* Each element is either a single attribute ID or an ID range. */
    while (p < p_end)
    {
        type = *p++;
        if ((type >> 3) != UINT_DESC_TYPE)
            return (p);

        switch (type & 7)
        {
        case SIZE_TWO_BYTES:
            attr_len = 2;
            break;
        case SIZE_FOUR_BYTES:
            attr_len = 4;
            break;
        case SIZE_IN_NEXT_BYTE:
            attr_len = *p++;
            break;
        case SIZE_IN_NEXT_WORD:
            attr_len = (UINT16)((*p << 8) + *(p + 1));
            p += 2;
            break;
        case SIZE_IN_NEXT_LONG:
            attr_len = (*p << 24) + (*(p + 1) << 16) + (*(p + 2) << 8) + *(p + 3);
            p += 4;
            break;
        default:
            return (NULL);
        }

        if (attr_len == 2)
        {
            p_seq->attr[p_seq->num_attrs].start = (UINT16)((*p << 8) + *(p + 1));
            p_seq->attr[p_seq->num_attrs].end   = p_seq->attr[p_seq->num_attrs].start;
            p += 2;
        }
        else if (attr_len == 4)
        {
            p_seq->attr[p_seq->num_attrs].start = (UINT16)((*p << 8) + *(p + 1));
            p_seq->attr[p_seq->num_attrs].end   = (UINT16)((*(p + 2) << 8) + *(p + 3));
            p += 4;
        }
        else
            return (NULL);

        p_seq->num_attrs++;
        if (p_seq->num_attrs >= SDPU_MAX_SEQ_ENTRIES)
            return (NULL);
    }

    return (p);
}

/*******************************************************************************
**
** Function         sdpu_get_len_from_type
**
** Description      Compute the byte length of a data element from its
**                  descriptor type byte.
**
** Returns          Pointer to the first byte of the element data.
**
*******************************************************************************/
UINT8 *sdpu_get_len_from_type (UINT8 *p, UINT8 type, UINT32 *p_len)
{
    switch (type & 7)
    {
    case SIZE_ONE_BYTE:
        *p_len = 1;
        break;
    case SIZE_TWO_BYTES:
        *p_len = 2;
        break;
    case SIZE_FOUR_BYTES:
        *p_len = 4;
        break;
    case SIZE_EIGHT_BYTES:
        *p_len = 8;
        break;
    case SIZE_SIXTEEN_BYTES:
        *p_len = 16;
        break;
    case SIZE_IN_NEXT_BYTE:
        *p_len = *p++;
        break;
    case SIZE_IN_NEXT_WORD:
        *p_len = (UINT16)((*p << 8) + *(p + 1));
        p += 2;
        break;
    case SIZE_IN_NEXT_LONG:
        *p_len = (UINT16)((*p << 24) + (*(p + 1) << 16) + (*(p + 2) << 8) + *(p + 3));
        p += 4;
        break;
    }

    return (p);
}

/* ------------------------------------------------------------------------- */
/* UUID helpers                                                              */
/* ------------------------------------------------------------------------- */

/*******************************************************************************
**
** Function         sdpu_is_base_uuid
**
** Description      Check whether the last 12 bytes of a 128-bit UUID match
**                  the Bluetooth base UUID.
**
** Returns          TRUE if the base UUID matches.
**
*******************************************************************************/
BOOLEAN sdpu_is_base_uuid (UINT8 *p_uuid)
{
    UINT16 xx;

    for (xx = 4; xx < LEN_UUID_128; xx++)
    {
        if (p_uuid[xx] != sdp_base_uuid[xx])
            return (FALSE);
    }

    return (TRUE);
}

/*******************************************************************************
**
** Function         sdpu_compare_uuid_arrays
**
** Description      Compare two raw UUIDs, expanding shorter UUIDs against
**                  the Bluetooth base UUID as needed.
**
** Returns          TRUE if the UUIDs are identical.
**
*******************************************************************************/
BOOLEAN sdpu_compare_uuid_arrays (UINT8 *p_uuid1, UINT32 len1, UINT8 *p_uuid2, UINT16 len2)
{
    UINT8   uuid2_128[LEN_UUID_128];
    UINT8   uuid1_128[LEN_UUID_128];

    if (len1 == len2)
    {
        if (len1 == LEN_UUID_16)
            return ((p_uuid1[0] == p_uuid2[0]) && (p_uuid1[1] == p_uuid2[1]));
        else if (len1 == LEN_UUID_32)
            return ((p_uuid1[0] == p_uuid2[0]) && (p_uuid1[1] == p_uuid2[1]) &&
                    (p_uuid1[2] == p_uuid2[2]) && (p_uuid1[3] == p_uuid2[3]));
        else
            return (memcmp (p_uuid1, p_uuid2, len1) == 0);
    }

    if (len1 > len2)
    {
        /* A 32-bit UUID whose top two bytes are zero equals a 16-bit UUID. */
        if (len1 == LEN_UUID_32)
            return ((p_uuid1[0] == 0) && (p_uuid1[1] == 0) &&
                    (p_uuid1[2] == p_uuid2[0]) && (p_uuid1[3] == p_uuid2[1]));

        memcpy (uuid1_128, p_uuid1, LEN_UUID_128);
        memcpy (uuid2_128, sdp_base_uuid, LEN_UUID_128);

        if (len2 == LEN_UUID_32)
            memcpy (uuid2_128, p_uuid2, len2);
        else
            memcpy (&uuid2_128[2], p_uuid2, len2);

        return (memcmp (uuid1_128, uuid2_128, LEN_UUID_128) == 0);
    }

    if (len2 == LEN_UUID_32)
        return ((p_uuid2[0] == 0) && (p_uuid2[1] == 0) &&
                (p_uuid2[2] == p_uuid1[0]) && (p_uuid2[3] == p_uuid1[1]));

    memcpy (uuid2_128, p_uuid2, LEN_UUID_128);
    memcpy (uuid1_128, sdp_base_uuid, LEN_UUID_128);

    if (len1 == LEN_UUID_32)
        memcpy (uuid1_128, p_uuid1, len1);
    else
        memcpy (&uuid1_128[2], p_uuid1, len1);

    return (memcmp (uuid1_128, uuid2_128, LEN_UUID_128) == 0);
}

/*******************************************************************************
**
** Function         sdpu_compare_uuid_with_attr
**
** Description      Compare a tBT_UUID with a discovered attribute holding a
**                  UUID value.
**
** Returns          TRUE if the UUIDs are identical.
**
*******************************************************************************/
BOOLEAN sdpu_compare_uuid_with_attr (tBT_UUID *puuid1, tSDP_DISC_ATTR *p_attr)
{
    UINT32 uuid_len;

    uuid_len = puuid1->len;

    if (uuid_len != SDP_DISC_ATTR_LEN (p_attr->attr_len_type))
        return (FALSE);

    if (uuid_len == LEN_UUID_16)
        return (puuid1->uu.uuid16 == p_attr->attr_value.v.u16);
    else if (uuid_len == LEN_UUID_32)
        return (puuid1->uu.uuid32 == p_attr->attr_value.v.u32);
    else
        return (memcmp (puuid1->uu.uuid128, p_attr->attr_value.v.array, LEN_UUID_128) == 0);
}

/*******************************************************************************
**
** Function         sdpu_sort_attr_list
**
** Description      Sort the attribute ID list in ascending order, restarting
**                  the scan whenever a swap is made.
**
** Returns          void
**
*******************************************************************************/
void sdpu_sort_attr_list (UINT16 num_attr, tSDPU_DISC_DB *p_db)
{
    UINT16 xx;
    UINT16 temp;

    if (num_attr <= 1)
        return;

    for (xx = 0; xx < num_attr - 1; xx++)
    {
        if (p_db->attr_list[xx] > p_db->attr_list[xx + 1])
        {
            temp                    = p_db->attr_list[xx];
            p_db->attr_list[xx]     = p_db->attr_list[xx + 1];
            p_db->attr_list[xx + 1] = temp;
            xx = -1;
        }
    }
}
