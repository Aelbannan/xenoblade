// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/sdp/sdp_db
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned char BOOLEAN;

#define UUID_DESC_TYPE 3
#define DATA_ELE_SEQ_DESC_TYPE 6

#define SDP_MAX_RECORDS 0x14
#define SDP_MAX_ATTR_PER_RECORD 0x19
#define SDP_MAX_ATTR_LEN 0x50
#define SDP_MAX_PAD_LEN 0x15E
#define SIZE_TWO_BYTES 0x1

typedef struct {
    UINT32 len;            /* 0x00 */
    UINT8 *value_ptr;      /* 0x04 */
    UINT16 attr_id;        /* 0x08 */
    UINT8 type;            /* 0x0A */
    UINT8 _pad2;           /* 0x0B */
} tSDP_ATTRIBUTE;          /* 0x0C */

typedef struct {
    UINT32 record_handle;              /* 0x00 */
    UINT32 attr_data_end;              /* 0x04 */
    UINT16 num_attributes;             /* 0x08 */
    UINT8 _pad2[2];                    /* 0x0A */
    tSDP_ATTRIBUTE attribute[SDP_MAX_ATTR_PER_RECORD]; /* 0x0C */
    UINT8 attr_data[0x298 - 0x0C - SDP_MAX_ATTR_PER_RECORD * 0x0C]; /* 0x138 */
} tSDP_RECORD;                          /* 0x298 */

typedef struct {
    UINT16 num_uuid;         /* 0x00 */
    struct {
        UINT16 len;          /* 0x02 (entry + 0) */
        UINT8 value[0x10];   /* 0x04 (entry + 2) */
    } uuid_entry[1];         /* stride 0x12 */
} tSDP_UUID_SEQ;

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
extern BOOLEAN SDP_DeleteAttribute(UINT32 handle, UINT16 attr_id);extern void LogMsg_2(UINT32 level, const char *fmt, UINT32 arg1, UINT32 arg2);

tSDP_RECORD *sdp_db_service_search(tSDP_RECORD *p_rec, tSDP_UUID_SEQ *p_seq);  // defined below

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

/*******************************************************************************
 **
 ** Function         sdp_db_service_search
 **
 ** Description      Searches the record database for a record whose attribute
 **                  list contains every UUID in the given sequence.  When
 **                  p_rec is NULL the search starts at the first record,
 **                  otherwise it continues from the record after p_rec.
 **
 ** Returns          Pointer to the matching record, or NULL.
 **
 *******************************************************************************/
tSDP_RECORD *sdp_db_service_search(tSDP_RECORD *p_rec, tSDP_UUID_SEQ *p_seq)
{
    UINT16 xx, yy;
    tSDP_RECORD *p_end = &sdp_cb.server_db.record[sdp_cb.server_db.num_records];

    /* If p_rec is NULL, start at the beginning */
    if (p_rec == NULL) {
        p_rec = &sdp_cb.server_db.record[0];
    } else {
        p_rec++;
    }

    for (; p_rec < p_end; p_rec++) {
        for (xx = 0; xx < p_seq->num_uuid; xx++) {
            tSDP_ATTRIBUTE *p_attr = &p_rec->attribute[0];

            for (yy = 0; yy < p_rec->num_attributes; yy++, p_attr++) {
                UINT8 *p_value = p_seq->uuid_entry[xx].value;

                if (p_attr->type == UUID_DESC_TYPE) {
                    if (sdpu_compare_uuid_arrays(p_attr->value_ptr, p_attr->len,
                                                 p_value, p_seq->uuid_entry[xx].len))
                        break;
                } else if (p_attr->type == DATA_ELE_SEQ_DESC_TYPE) {
                    if (find_uuid_in_seq(p_attr->value_ptr, p_attr->len,
                                         p_value, p_seq->uuid_entry[xx].len, 0))
                        break;
                }
            }

            /* This record does not contain uuid[xx]; skip to the next record */
            if (yy == p_rec->num_attributes)
                break;
        }

        /* Every uuid in the sequence matched this record */
        if (xx == p_seq->num_uuid)
            return p_rec;
    }

    return NULL;
}

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
    tSDP_RECORD* record = (tSDP_RECORD*)rec;
    unsigned short num_attr = record->num_attributes;
    tSDP_ATTRIBUTE* attr_ptr = &record->attribute[0];
    unsigned short i = 0;
    while (i < num_attr) {
        unsigned short attr_id = attr_ptr->attr_id;
        if (attr_id >= attr_id_low && attr_id <= attr_id_high) {
            return attr_ptr;
        }
        i++;
        attr_ptr++;
    }
    return 0;
}

UINT32 SDP_CreateRecord(void) {
    tSDP_DB *db = &sdp_cb.server_db;
    UINT32 handle;
    UINT8 buf[4];

    if (db->num_records < SDP_MAX_RECORDS) {
        memset(&db->record[db->num_records], 0, sizeof(tSDP_RECORD));

        if (db->num_records > 0)
            handle = db->record[db->num_records - 1].record_handle + 1;
        else
            handle = 0x10000;

        db->record[db->num_records].record_handle = handle;

        buf[0] = (UINT8)(handle >> 24);
        buf[1] = (UINT8)(handle >> 16);
        db->num_records = db->num_records + 1;
        buf[2] = (UINT8)(handle >> 8);
        buf[3] = (UINT8)handle;

        SDP_AddAttribute(handle, 0, 1, 4, buf);

        handle = db->record[db->num_records - 1].record_handle;
    } else {
        handle = 0;
    }

    return handle;
}

BOOLEAN SDP_DeleteRecord(UINT32 handle) {
    UINT16 xx, yy;
    tSDP_RECORD *p_rec = &sdp_cb.server_db.record[0];

    if (handle == 0 || sdp_cb.server_db.num_records == 0) {
        sdp_cb.server_db.num_records = 0;
        sdp_cb.server_db.di_primary_handle = 0;
        sdp_cb.server_db.brcm_di_registered = 0;
        return TRUE;
    }

    for (xx = 0; xx < sdp_cb.server_db.num_records; xx++, p_rec++) {
        if (p_rec->record_handle == handle) {
            /* Found the record; shift every subsequent record down by one */
            for (; xx < sdp_cb.server_db.num_records; xx++, p_rec++) {
                *p_rec = *(p_rec + 1);

                for (yy = 0; yy < p_rec->num_attributes; yy++)
                    p_rec->attribute[yy].value_ptr -= sizeof(tSDP_RECORD);
            }

            sdp_cb.server_db.num_records--;

            if (sdp_cb.server_db.di_primary_handle == handle) {
                sdp_cb.server_db.di_primary_handle = 0;
                sdp_cb.server_db.brcm_di_registered = 0;
            }

            return TRUE;
        }
    }

    return FALSE;
} 

BOOLEAN SDP_AddServiceClassIdList(UINT32 handle, UINT16 num_services, UINT16 *p_service_ids) {
    UINT8 buff[SDP_MAX_ATTR_LEN * 2 + 8];
    UINT16 num = 0;
    UINT8 *p = buff;

    for (num = 0; num < num_services; num++) {
        *p++ = (UINT8)((UUID_DESC_TYPE << 3) | SIZE_TWO_BYTES);
        *p++ = (UINT8)(*p_service_ids >> 8);
        *p++ = (UINT8)*p_service_ids;
        p_service_ids++;
    }

    return SDP_AddAttribute(handle, 1, DATA_ELE_SEQ_DESC_TYPE, (UINT32)(p - buff), buff);
}

BOOLEAN SDP_AddUuidSequence(UINT32 handle, UINT16 attr_id, UINT16 num_uuids, UINT16 *p_uuids) {
    UINT16 xx;
    UINT8 buff[SDP_MAX_ATTR_LEN * 2];
    UINT8 *p = buff;
    int max_len = SDP_MAX_ATTR_LEN - 3;

    /* First, build the sequence */
    for (xx = 0; xx < num_uuids; xx++, p_uuids++) {
        *p++ = (UINT8)((UUID_DESC_TYPE << 3) | SIZE_TWO_BYTES);
        *p++ = (UINT8)(*p_uuids >> 8);
        *p++ = (UINT8)*p_uuids;

        if ((p - buff) > max_len) {
            if (sdp_cb.trace_level >= 2) {
                LogMsg_2(0xa0001, "SDP_AddUuidSequence - too long, add %d uuids of %d", xx, num_uuids);
            }
            break;
        }
    }

    return SDP_AddAttribute(handle, attr_id, DATA_ELE_SEQ_DESC_TYPE, (UINT32)(p - buff), buff);
}

BOOLEAN SDP_AddAttribute(UINT32 handle, UINT16 attr_id, UINT8 attr_type,
                         UINT32 attr_len, UINT8 *p_val) {
    UINT16 xx, yy, zz;
    tSDP_RECORD *p_rec;
    tSDP_ATTRIBUTE *p_attr;

    p_rec = &sdp_cb.server_db.record[0];
    for (xx = 0; xx < sdp_cb.server_db.num_records; xx++, p_rec++) {
        if (p_rec->record_handle == handle) {
            /* found the record - check the attribute */
            p_attr = &p_rec->attribute[0];
            for (zz = 0; zz < p_rec->num_attributes; zz++, p_attr++) {
                if (p_attr->attr_id == attr_id) {
                    /* delete the attribute */
                    SDP_DeleteAttribute(handle, attr_id);
                    break;
                }
                if (p_attr->attr_id > attr_id) {
                    break;
                }
            }

            if (p_rec->num_attributes == SDP_MAX_ATTR_PER_RECORD) {
                return FALSE;
            }

            if (zz == p_rec->num_attributes) {
                /* add the attribute at the end */
                p_attr = &p_rec->attribute[p_rec->num_attributes];
            } else {
                /* shift the attributes up by one to make room */
                for (yy = p_rec->num_attributes; yy > zz; yy--) {
                    p_rec->attribute[yy] = p_rec->attribute[yy - 1];
                }
            }

            p_rec->num_attributes++;
            p_attr->attr_id = attr_id;
            p_attr->len = attr_len;
            p_attr->type = attr_type;

            if (p_rec->attr_data_end + attr_len >= SDP_MAX_PAD_LEN) {
                if (sdp_cb.trace_level >= 2) {
                    LogMsg_2(0xA0001,
                             "SDP_AddAttribute: attr_len:%d too long. truncate to (%d)",
                             attr_len, SDP_MAX_PAD_LEN - p_rec->attr_data_end);
                }
                attr_len = SDP_MAX_PAD_LEN - p_rec->attr_data_end;
                p_val[SDP_MAX_PAD_LEN - p_rec->attr_data_end] = 0;
                p_val[SDP_MAX_PAD_LEN + 1 - p_rec->attr_data_end] = 0;
            }

            if (attr_len != 0 && p_val != NULL) {
                memcpy(&p_rec->attr_data[p_rec->attr_data_end], p_val, attr_len);
                p_attr->value_ptr = &p_rec->attr_data[p_rec->attr_data_end];
                p_rec->attr_data_end += attr_len;
            }

            if (handle == sdp_cb.server_db.di_primary_handle &&
                attr_id == 0x8001) {
                sdp_cb.server_db.brcm_di_registered = 1;
            }
            return TRUE;
        }
    }

    return FALSE;
}

BOOLEAN SDP_DeleteAttribute(UINT32 handle, UINT16 attr_id)
{
    UINT16 xx;
    tSDP_RECORD *p_rec = &sdp_cb.server_db.record[0];

    for (xx = 0; xx < sdp_cb.server_db.num_records; xx++, p_rec++)
    {
        if (p_rec->record_handle == handle)
        {
            tSDP_ATTRIBUTE *p_attr;

            p_attr = &p_rec->attribute[0];

            if (sdp_cb.trace_level >= 3)
            {
                LogMsg_2(0xA0002, "Deleting attr_id 0x%04x for handle 0x%x", attr_id, handle);
            }

            for (xx = 0; xx < p_rec->num_attributes; xx++, p_attr++)
            {
                if (p_attr->attr_id == attr_id)
                {
                    UINT32 len = p_attr->len;
                    UINT8 *p_value = p_attr->value_ptr;

                    if (len != 0)
                    {
                        UINT16 i;

                        for (i = 0; i < p_rec->num_attributes; i++)
                        {
                            if (p_rec->attribute[i].value_ptr > p_value)
                            {
                                p_rec->attribute[i].value_ptr -= len;
                            }
                        }
                    }

                    p_rec->num_attributes--;

                    for (; xx < p_rec->num_attributes; xx++, p_attr++)
                    {
                        *p_attr = *(p_attr + 1);
                    }

                    if (len != 0)
                    {
                        UINT16 i;
                        UINT16 cnt = (UINT16)(p_rec->attr_data_end -
                                               (UINT32)((p_value + len) - p_rec->attr_data));

                        for (i = 0; i < cnt; i++, p_value++)
                        {
                            p_value[0] = *(len + p_value);
                        }

                        p_rec->attr_data_end -= len;
                    }

                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}
