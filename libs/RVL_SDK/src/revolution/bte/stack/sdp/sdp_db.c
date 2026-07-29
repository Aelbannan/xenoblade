// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/sdp/sdp_db
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned char BOOLEAN;

#define UUID_DESC_TYPE 3
#define DATA_ELE_SEQ_DESC_TYPE 6

#define SDP_MAX_RECORDS 0x14

typedef struct {
    UINT32 record_handle;
    UINT8 _pad[0x298 - sizeof(UINT32)];
} tSDP_RECORD;

typedef struct {
    UINT32 di_primary_handle;
    UINT8 brcm_di_registered;
    UINT8 _pad1;
    UINT16 num_records;
    tSDP_RECORD record[1];
} tSDP_DB;

typedef struct {
    UINT8 _pad[0x121c];
    tSDP_DB server_db;
    UINT8 _pad2[0x3174];
    UINT8 trace_level;
} tSDP_CB;

extern tSDP_CB sdp_cb;
extern UINT8 *sdpu_get_len_from_type(UINT8 *p, UINT8 type, UINT32 *p_len);
extern BOOLEAN sdpu_compare_uuid_arrays(UINT8 *p_uuid1, UINT32 len1, UINT8 *p_uuid2, UINT32 len2);
extern BOOLEAN SDP_AddAttribute(UINT32 handle, UINT16 attr_id, UINT8 attr_type, UINT32 attr_len, UINT8 *p_val);
extern void LogMsg_2(UINT32 level, const char *fmt, UINT16 arg1, UINT16 arg2);

void sdp_db_service_search() {}

#pragma dont_inline on

BOOLEAN find_uuid_in_seq(UINT8 *p, UINT32 seq_len, UINT8 *p_uuid, UINT32 uuid_len, int nest_level) {
    UINT8 *p_end = p + seq_len;
    UINT8 type;
    UINT32 len;

    if (nest_level > 3)
        return FALSE;

    while (p < p_end) {
        type = *p++;
        p = sdpu_get_len_from_type(p, type, &len);
        type = type >> 3;

        if (type == UUID_DESC_TYPE) {
            if (sdpu_compare_uuid_arrays(p, len, p_uuid, uuid_len))
                return TRUE;
        } else if (type == DATA_ELE_SEQ_DESC_TYPE) {
            if (find_uuid_in_seq(p, len, p_uuid, uuid_len, nest_level + 1))
                return TRUE;
        }

        p = p + len;
    }

    return FALSE;
}

#pragma dont_inline off

tSDP_RECORD *sdp_db_find_record(UINT32 handle) {
    tSDP_RECORD *p_rec;
    tSDP_RECORD *p_end = sdp_cb.server_db.record + sdp_cb.server_db.num_records;

    for (p_rec = sdp_cb.server_db.record; p_rec < p_end; p_rec++) {
        if (p_rec->record_handle == handle)
            return p_rec;
    }

    return NULL;
}

void *sdp_db_find_attr_in_rec(void *rec, unsigned short attr_id_low, unsigned short attr_id_high) {
    unsigned short num_attr = *(unsigned short *)((char *)rec + 8);
    char *attr_ptr = (char *)rec + 12;
    unsigned short i = 0;
    while (i < num_attr) {
        unsigned short attr_id = *(unsigned short *)(attr_ptr + 8);
        if (attr_id >= attr_id_low && attr_id <= attr_id_high) {
            return attr_ptr;
        }
        i++;
        attr_ptr += 12;
    }
    return 0;
}

UINT32 SDP_CreateRecord(void) {
    tSDP_DB *db = &sdp_cb.server_db;
    UINT16 num_rec = db->num_records;
    UINT32 handle;
    UINT8 buf[4];

    if (num_rec >= SDP_MAX_RECORDS)
        return 0;

    memset(&db->record[num_rec], 0, sizeof(tSDP_RECORD));

    if (num_rec > 0)
        handle = db->record[num_rec - 1].record_handle + 1;
    else
        handle = 0x10000;

    db->record[num_rec].record_handle = handle;

    buf[0] = (UINT8)(handle >> 24);
    buf[1] = (UINT8)(handle >> 16);
    buf[2] = (UINT8)(handle >> 8);
    buf[3] = (UINT8)handle;

    db->num_records = num_rec + 1;
    SDP_AddAttribute(handle, 0, 1, 4, buf);

    return handle;
}

void SDP_DeleteRecord() {}

void SDP_AddServiceClassIdList() {}

void SDP_DeleteAttribute() {}

BOOLEAN SDP_AddUuidSequence(UINT32 handle, UINT16 attr_id, UINT16 num_uuids, UINT16 *p_uuids) {
    UINT8 buf[0xC0];
    UINT8 *p = buf;
    UINT16 i;

    for (i = 0; i < num_uuids; i++) {
        *p++ = 0x19;
        *p++ = (UINT8)(*p_uuids >> 8);
        *p++ = (UINT8)*p_uuids;
        p_uuids++;

        if (p - buf > 0x4d) {
            if (sdp_cb.trace_level >= 2) {
                LogMsg_2(0xa0001, "SDP_AddUuidSequence - too long, add %d uuids of %d", i, num_uuids);
            }
            break;
        }
    }

    return SDP_AddAttribute(handle, attr_id, 6, (UINT32)(p - buf), buf);
}
