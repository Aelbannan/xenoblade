// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/sdp/sdp_api
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <string.h>

/* Retail sdp build config: no raw-data tail (struct is 0x6C, not 0x7C) and
   SDP_MAX_ATTR_FILTERS 12 (header defaults are TRUE/15). */
#define SDP_RAW_DATA_INCLUDED FALSE
#define SDP_MAX_ATTR_FILTERS 12

#include "revolution/bte/stack/include/sdp_api.h"
#include "revolution/bte/stack/include/sdpdefs.h"

/* Local DI record layout as built by SDP_SetLocalDiRecord: spec_id then the
   tSDP_DI_RECORD fields; the three strings are 0x50 bytes each. */
typedef struct {
    UINT16 vendor;                    /* 0x00 */
    UINT16 vendor_id_source;          /* 0x02 */
    UINT16 product;                   /* 0x04 */
    UINT16 version;                   /* 0x06 */
    BOOLEAN primary_record;           /* 0x08 */
    char client_executable_url[0x50]; /* 0x09 */
    char service_description[0x50];   /* 0x59 */
    char documentation_url[0x50];     /* 0xA9 */
} tSDP_DI_RECORD_LOCAL;                /* 0xFA */

typedef struct {
    UINT16 spec_id;                   /* 0x00 */
    tSDP_DI_RECORD_LOCAL rec;         /* 0x02 */
} tSDP_DI_GET_RECORD_LOCAL;           /* 0xFC */

/* Server-db attribute entry (12-byte stride, see sdp_db.c tSDP_ATTRIBUTE). */
typedef struct {
    UINT32 len;             /* 0x00 */
    UINT8 *value_ptr;       /* 0x04 */
    UINT16 attr_id;         /* 0x08 */
    UINT8 attr_type;        /* 0x0A */
    UINT8 pad;              /* 0x0B */
} tSDP_ATTRIBUTE;

/* sdp_cb view used by this file: DI primary-record handle at 0x121C,
   trace level at 0x4630. */
typedef struct {
    UINT8 _pad[0x121C];              /* 0x00..0x121B */
    UINT32 di_primary_handle;        /* 0x121C */
    UINT8 _pad2[0x4630 - 0x1220];    /* 0x1220..0x462F */
    UINT8 trace_level;               /* 0x4630 */
} tSDP_CB;

/* Connection control block view used by the discovery-request API: only the
   fields this TU writes are named; offsets match the retail sdp_int.h
   layout. */
typedef struct {
    UINT8 _pad[0x410];               /* 0x000 */
    tSDP_DISCOVERY_DB *p_db;         /* 0x410 */
    tSDP_DISC_CMPL_CB *p_cb;         /* 0x414 */
    UINT8 _pad2[0x474 - 0x418];      /* 0x418 */
    UINT8 disc_state;                /* 0x474 */
    BOOLEAN is_attr_search;          /* 0x475 */
} tCONN_CB;

extern tSDP_CB sdp_cb;
extern tCONN_CB *sdp_conn_originate(BD_ADDR bd_addr);

/* Trace / sort helpers (sdp_utils.c) */
extern void LogMsg_4(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1,
                     UINT32 p2, UINT32 p3, UINT32 p4);
extern void sdpu_sort_attr_list(UINT16 num_attr, tSDP_DISCOVERY_DB *p_db);

/* sdp_db.c helpers */
extern void *sdp_db_find_record(UINT32 handle);
extern void *sdp_db_find_attr_in_rec(void *p_rec, UINT16 attr_id_low,
                                     UINT16 attr_id_high);

BOOLEAN SDP_InitDiscoveryDb(tSDP_DISCOVERY_DB *p_db, UINT32 len, UINT16 num_uuid,
                            tSDP_UUID *p_uuid_list, UINT16 num_attr,
                            UINT16 *p_attr_list) {
    UINT16 xx;

    if ((p_db == NULL) || (len < sizeof(tSDP_DISCOVERY_DB)) ||
        (num_attr > SDP_MAX_ATTR_FILTERS) || (num_uuid > SDP_MAX_UUID_FILTERS)) {
        if (sdp_cb.trace_level >= 1) {
            LogMsg_4(0xA0000,
                     "SDP_InitDiscoveryDb Illegal param: p_db 0x%x, len %d, num_uuid %d, num_attr %d",
                     (UINT32)p_db, len, (UINT32)num_uuid, (UINT32)num_attr);
        }
        return FALSE;
    }

    memset(p_db, 0, len);

    p_db->mem_size = len - sizeof(tSDP_DISCOVERY_DB);
    p_db->mem_free = len - sizeof(tSDP_DISCOVERY_DB);
    p_db->p_first_rec = NULL;
    p_db->p_free_mem = (UINT8 *)(p_db + 1);

    for (xx = 0; xx < num_uuid; xx++) {
        p_db->uuid_filters[xx] = *p_uuid_list++;
    }

    p_db->num_uuid_filters = num_uuid;

    for (xx = 0; xx < num_attr; xx++) {
        p_db->attr_filters[xx] = *p_attr_list++;
    }

    sdpu_sort_attr_list(num_attr, p_db);
    p_db->num_attr_filters = num_attr;
    return TRUE;
}

BOOLEAN SDP_ServiceSearchRequest(UINT8 *p_bd_addr, tSDP_DISCOVERY_DB *p_db,
                                 tSDP_DISC_CMPL_CB *p_cb) {
    tCONN_CB *p_ccb;

    /* find the SDP connection control block */
    if ((p_ccb = sdp_conn_originate(p_bd_addr)) == NULL) {
        return FALSE;
    }

    /* retail stores SDP_DISC_WAIT_CONN (0) here */
    p_ccb->disc_state = 0;
    p_ccb->p_db = p_db;
    p_ccb->p_cb = p_cb;

    return TRUE;
}

BOOLEAN SDP_ServiceSearchAttributeRequest(UINT8 *p_bd_addr, tSDP_DISCOVERY_DB *p_db,
                                          tSDP_DISC_CMPL_CB *p_cb) {
    tCONN_CB *p_ccb;

    /* find the SDP connection control block */
    if ((p_ccb = sdp_conn_originate(p_bd_addr)) == NULL) {
        return FALSE;
    }

    /* retail stores SDP_DISC_WAIT_CONN (0) here and marks the attribute
       search flag */
    p_ccb->disc_state = 0;
    p_ccb->p_db = p_db;
    p_ccb->p_cb = p_cb;
    p_ccb->is_attr_search = TRUE;

    return TRUE;
}

tSDP_DISC_ATTR *SDP_FindAttributeInRec(tSDP_DISC_REC *p_rec, UINT16 attr_id) {
    tSDP_DISC_ATTR *p_attr = p_rec->p_first_attr;

    while (p_attr) {
        if (p_attr->attr_id == attr_id) {
            return p_attr;
        }

        p_attr = p_attr->p_next_attr;
    }

    return NULL;
}

tSDP_DISC_REC *SDP_FindServiceInDb(tSDP_DISCOVERY_DB *p_db, UINT16 attr_id,
                                 tSDP_DISC_REC *p_start_rec) {
    tSDP_DISC_REC *p_rec;
    tSDP_DISC_ATTR *p_attr;

    /* Look through the records in the database */
    if (p_start_rec == NULL) {
        p_rec = p_db->p_first_rec;
    } else {
        p_rec = p_start_rec->p_next_rec;
    }

    /* Loop through the records */
    while (p_rec) {
        /* Look through the attributes */
        p_attr = p_rec->p_first_attr;
        while (p_attr) {
            /* If the attribute ID matches, return the record */
            if (p_attr->attr_id == ATTR_ID_SERVICE_CLASS_ID_LIST &&
                SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) ==
                    DATA_ELE_SEQ_DESC_TYPE) {
                for (p_attr = p_attr->attr_value.v.p_sub_attr; p_attr;
                     p_attr = p_attr->p_next_attr) {
                    if (SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) ==
                            UUID_DESC_TYPE &&
                        SDP_DISC_ATTR_LEN(p_attr->attr_len_type) ==
                            LEN_UUID_16 &&
                        p_attr->attr_value.v.u16 == attr_id) {
                        return p_rec;
                    }
                }
                break;
            } else if (p_attr->attr_id == ATTR_ID_SERVICE_ID &&
                       SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) ==
                           UUID_DESC_TYPE &&
                       SDP_DISC_ATTR_LEN(p_attr->attr_len_type) ==
                           LEN_UUID_16 &&
                       p_attr->attr_value.v.u16 == attr_id) {
                return p_rec;
            }
            p_attr = p_attr->p_next_attr;
        }
        p_rec = p_rec->p_next_rec;
    }

    return NULL;
}

tSDP_DISC_REC *SDP_FindServiceUUIDInDb(tSDP_DISCOVERY_DB *p_db, tBT_UUID *p_uuid,
                                     tSDP_DISC_REC *p_start_rec) {
    /* Defined in sdp_utils.c */
    extern BOOLEAN sdpu_compare_uuid_with_attr(tBT_UUID *puuid1,
                                               tSDP_DISC_ATTR *p_attr);

    tSDP_DISC_REC *p_rec;
    tSDP_DISC_ATTR *p_attr;

    /* Look through the records in the database */
    if (p_start_rec == NULL)
        p_rec = p_db->p_first_rec;
    else
        p_rec = p_start_rec->p_next_rec;

    /* Loop through records, looking for UUID match */
    while (p_rec) {
        /* Loop through attributes of the record */
        p_attr = p_rec->p_first_attr;
        while (p_attr) {
            /* Check the attribute ID and that the value is a data element
               sequence */
            if (p_attr->attr_id == ATTR_ID_SERVICE_CLASS_ID_LIST &&
                SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) ==
                    DATA_ELE_SEQ_DESC_TYPE) {
                /* Loop through the attribute values */
                for (p_attr = p_attr->attr_value.v.p_sub_attr; p_attr;
                     p_attr = p_attr->p_next_attr) {
                    /* Check if the attribute value is a UUID */
                    if (SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) ==
                            UUID_DESC_TYPE &&
                        sdpu_compare_uuid_with_attr(p_uuid, p_attr)) {
                        return (p_rec);
                    }
                }
                break;
            } else if (p_attr->attr_id == ATTR_ID_SERVICE_ID &&
                       SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) ==
                           UUID_DESC_TYPE &&
                       sdpu_compare_uuid_with_attr(p_uuid, p_attr)) {
                return (p_rec);
            }
            p_attr = p_attr->p_next_attr;
        }
        p_rec = p_rec->p_next_rec;
    }

    return (NULL);
}

UINT16 SDP_SetLocalDiRecord(tSDP_DI_RECORD *device_info, UINT32 *p_handle) {
    UINT16 result = SDP_SUCCESS;
    UINT32 handle;
    UINT16 spec_id = 0x1200;
    UINT8 p_val[2];
    tSDP_DI_RECORD_LOCAL *p_info = (tSDP_DI_RECORD_LOCAL *)device_info;

    *p_handle = 0;

    if (p_info == NULL) {
        return SDP_ILLEGAL_PARAMETER;
    }

    if ((p_info->primary_record == TRUE) && (sdp_cb.di_primary_handle != 0)) {
        handle = sdp_cb.di_primary_handle;
    } else {
        handle = SDP_CreateRecord();
        if (handle == 0) {
            return SDP_NO_RESOURCES;
        }
    }

    *p_handle = handle;

    if (!SDP_AddServiceClassIdList(handle, 1, &spec_id)) {
        result = SDP_DI_REG_FAILED;
    }

    if (result == SDP_SUCCESS) {
        p_val[0] = 0;
        p_val[1] = 9;
        if (SDP_AddAttribute(handle, ATTR_ID_SPECIFICATION_ID, UINT_DESC_TYPE, 2, &p_val[0]) == FALSE) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result == SDP_SUCCESS) {
        if (p_info->client_executable_url[0] != '\0') {
            if (((strlen((const char *)p_info->client_executable_url) + 1) > 0x50) || !SDP_AddAttribute(handle, ATTR_ID_CLIENT_EXE_URL, URL_DESC_TYPE,
                                      (UINT32)(strlen((const char *)p_info->client_executable_url) + 1),
                                      (UINT8 *)p_info->client_executable_url)) {
                    result = SDP_DI_REG_FAILED;
            }
        }
    }

    if (result == SDP_SUCCESS) {
        if (p_info->service_description[0] != '\0') {
            if (((strlen((const char *)p_info->service_description) + 1) > 0x50) || !SDP_AddAttribute(handle, ATTR_ID_SERVICE_DESCRIPTION, TEXT_STR_DESC_TYPE,
                                      (UINT32)(strlen((const char *)p_info->service_description) + 1),
                                      (UINT8 *)p_info->service_description)) {
                    result = SDP_DI_REG_FAILED;
            }
        }
    }

    if (result == SDP_SUCCESS) {
        if (p_info->documentation_url[0] != '\0') {
            if (((strlen((const char *)p_info->documentation_url) + 1) > 0x50) || !SDP_AddAttribute(handle, ATTR_ID_DOCUMENTATION_URL, URL_DESC_TYPE,
                                      (UINT32)(strlen((const char *)p_info->documentation_url) + 1),
                                      (UINT8 *)p_info->documentation_url)) {
                    result = SDP_DI_REG_FAILED;
            }
        }
    }

    if (result == SDP_SUCCESS) {
        p_val[0] = (UINT8)(p_info->vendor >> 8);
        p_val[1] = (UINT8)p_info->vendor;
        if (SDP_AddAttribute(handle, ATTR_ID_VENDOR_ID, UINT_DESC_TYPE, 2, &p_val[0]) == FALSE) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result == SDP_SUCCESS) {
        p_val[0] = (UINT8)(p_info->product >> 8);
        p_val[1] = (UINT8)p_info->product;
        if (SDP_AddAttribute(handle, ATTR_ID_PRODUCT_ID, UINT_DESC_TYPE, 2, &p_val[0]) == FALSE) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result == SDP_SUCCESS) {
        p_val[0] = (UINT8)(p_info->version >> 8);
        p_val[1] = (UINT8)p_info->version;
        if (SDP_AddAttribute(handle, ATTR_ID_PRODUCT_VERSION, UINT_DESC_TYPE, 2, &p_val[0]) == FALSE) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result == SDP_SUCCESS) {
        if (SDP_AddAttribute(handle, ATTR_ID_PRIMARY_RECORD, BOOLEAN_DESC_TYPE, 1,
                              &p_info->primary_record) == FALSE) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result == SDP_SUCCESS) {
        p_val[0] = (UINT8)(p_info->vendor_id_source >> 8);
        p_val[1] = (UINT8)p_info->vendor_id_source;
        if (SDP_AddAttribute(handle, ATTR_ID_VENDOR_ID_SOURCE, UINT_DESC_TYPE, 2, &p_val[0]) == FALSE) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result != SDP_SUCCESS) {
        SDP_DeleteRecord(handle);
    } else if (p_info->primary_record == TRUE) {
        sdp_cb.di_primary_handle = handle;
    }

    return result;
}

UINT16 SDP_GetLocalDiRecord(tSDP_DI_GET_RECORD *p_device_info, UINT32 *p_handle) {
    UINT16 result = SDP_NO_DI_RECORD_FOUND;
    tSDP_DI_GET_RECORD_LOCAL *p_info = (tSDP_DI_GET_RECORD_LOCAL *)p_device_info;
    void *p_rec;
    tSDP_ATTRIBUTE *p_attr;
    UINT8 *p_temp;
    INT32 templen;
    int jk;

    if (*p_handle == 0) {
        *p_handle = sdp_cb.di_primary_handle;
    }

    if ((p_rec = sdp_db_find_record(*p_handle)) != NULL) {
        memset(p_device_info, 0, sizeof(tSDP_DI_RECORD_LOCAL));
        result = SDP_SUCCESS;

        /* Retrieve the Specification ID */
        if ((p_attr = (tSDP_ATTRIBUTE *)sdp_db_find_attr_in_rec(
                 p_rec, ATTR_ID_SPECIFICATION_ID,
                 ATTR_ID_SPECIFICATION_ID)) != NULL) {
            p_temp = p_attr->value_ptr;
            p_info->spec_id = (UINT16)((p_temp[0] << 8) + p_temp[1]);
        }

        /* Retrieve the Vendor ID */
        if ((p_attr = (tSDP_ATTRIBUTE *)sdp_db_find_attr_in_rec(
                 p_rec, ATTR_ID_VENDOR_ID, ATTR_ID_VENDOR_ID)) != NULL) {
            p_temp = p_attr->value_ptr;
            p_info->rec.vendor = (UINT16)((p_temp[0] << 8) + p_temp[1]);
        }

        /* Retrieve the Product ID */
        if ((p_attr = (tSDP_ATTRIBUTE *)sdp_db_find_attr_in_rec(
                 p_rec, ATTR_ID_PRODUCT_ID, ATTR_ID_PRODUCT_ID)) != NULL) {
            p_temp = p_attr->value_ptr;
            p_info->rec.product = (UINT16)((p_temp[0] << 8) + p_temp[1]);
        }

        /* Retrieve the Version ID */
        if ((p_attr = (tSDP_ATTRIBUTE *)sdp_db_find_attr_in_rec(
                 p_rec, ATTR_ID_PRODUCT_VERSION,
                 ATTR_ID_PRODUCT_VERSION)) != NULL) {
            p_temp = p_attr->value_ptr;
            p_info->rec.version = (UINT16)((p_temp[0] << 8) + p_temp[1]);
        }

        /* Retrieve the Vendor ID Source ID */
        if ((p_attr = (tSDP_ATTRIBUTE *)sdp_db_find_attr_in_rec(
                 p_rec, ATTR_ID_VENDOR_ID_SOURCE,
                 ATTR_ID_VENDOR_ID_SOURCE)) != NULL) {
            p_temp = p_attr->value_ptr;
            p_info->rec.vendor_id_source = (UINT16)((p_temp[0] << 8) + p_temp[1]);
        }

        /* Retrieve the Primary Record */
        if ((p_attr = (tSDP_ATTRIBUTE *)sdp_db_find_attr_in_rec(
                 p_rec, ATTR_ID_PRIMARY_RECORD,
                 ATTR_ID_PRIMARY_RECORD)) != NULL) {
            p_info->rec.primary_record = *p_attr->value_ptr;
        }

        /* Retrieve the Client Executable URL */
        if ((p_attr = (tSDP_ATTRIBUTE *)sdp_db_find_attr_in_rec(
                 p_rec, ATTR_ID_CLIENT_EXE_URL,
                 ATTR_ID_CLIENT_EXE_URL)) != NULL) {
            templen = (INT32)((p_attr->len < 0x50) ? p_attr->len : 0x50);
            p_temp = p_attr->value_ptr;
            for (jk = 0; jk < templen; jk++) {
                ((UINT8 *)p_info->rec.client_executable_url)[jk] = *p_temp++;
            }
        }

        /* Retrieve the Service Description */
        if ((p_attr = (tSDP_ATTRIBUTE *)sdp_db_find_attr_in_rec(
                 p_rec, ATTR_ID_SERVICE_DESCRIPTION,
                 ATTR_ID_SERVICE_DESCRIPTION)) != NULL) {
            templen = (INT32)((p_attr->len < 0x50) ? p_attr->len : 0x50);
            p_temp = p_attr->value_ptr;
            for (jk = 0; jk < templen; jk++) {
                ((UINT8 *)p_info->rec.service_description)[jk] = *p_temp++;
            }
        }

        /* Retrieve the Documentation URL */
        if ((p_attr = (tSDP_ATTRIBUTE *)sdp_db_find_attr_in_rec(
                 p_rec, ATTR_ID_DOCUMENTATION_URL,
                 ATTR_ID_DOCUMENTATION_URL)) != NULL) {
            templen = (INT32)((p_attr->len < 0x50) ? p_attr->len : 0x50);
            p_temp = p_attr->value_ptr;
            for (jk = 0; jk < templen; jk++) {
                ((UINT8 *)p_info->rec.documentation_url)[jk] = *p_temp++;
            }
        }
    } else {
        *p_handle = 0;
    }

    return result;
}

UINT8 SDP_SetTraceLevel(UINT8 new_level) {
    if (new_level != 0xff) {
        sdp_cb.trace_level = (unsigned char)new_level;
    }
    return sdp_cb.trace_level;
}
