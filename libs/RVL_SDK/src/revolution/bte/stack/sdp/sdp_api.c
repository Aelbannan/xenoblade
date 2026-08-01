// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/sdp/sdp_api
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <string.h>

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
    UINT8 client_executable_url[0x50]; /* 0x09 */
    UINT8 service_description[0x50];   /* 0x59 */
    UINT8 documentation_url[0x50];     /* 0xA9 */
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

extern tSDP_CB sdp_cb;

/* sdp_db.c helpers */
extern void *sdp_db_find_record(UINT32 handle);
extern void *sdp_db_find_attr_in_rec(void *p_rec, UINT16 attr_id_low,
                                     UINT16 attr_id_high);

BOOLEAN SDP_InitDiscoveryDb(tSDP_DISCOVERY_DB *p_db, UINT32 len, UINT16 num_uuid,
                            tSDP_UUID *p_uuid_list, UINT16 num_attr,
                            UINT16 *p_attr_list) {
    return 0;
}

BOOLEAN SDP_ServiceSearchRequest(UINT8 *p_bd_addr, tSDP_DISCOVERY_DB *p_db,
                                 tSDP_DISC_CMPL_CB *p_cb) {
    return 0;
}

BOOLEAN SDP_ServiceSearchAttributeRequest(UINT8 *p_bd_addr, tSDP_DISCOVERY_DB *p_db,
                                          tSDP_DISC_CMPL_CB *p_cb) {
    return 0;
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
    tSDP_DISC_ATTR *p_attr2;

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
                for (p_attr2 = p_attr->attr_value.v.p_sub_attr; p_attr2;
                     p_attr2 = p_attr2->p_next_attr) {
                    if (SDP_DISC_ATTR_TYPE(p_attr2->attr_len_type) ==
                            UUID_DESC_TYPE &&
                        SDP_DISC_ATTR_LEN(p_attr2->attr_len_type) ==
                            LEN_UUID_16 &&
                        p_attr2->attr_value.v.u16 == attr_id) {
                        return p_rec;
                    }
                }
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

    if (p_start_rec == NULL)
        p_rec = p_db->p_first_rec;
    else
        p_rec = p_start_rec->p_next_rec;

    /* Loop through records, looking for UUID match */
    while (p_rec) {
        p_attr = p_rec->p_first_attr;

        /* Loop through attributes of the record */
        while (p_attr) {
            /* Check the attribute ID */
            if (p_attr->attr_id == ATTR_ID_SERVICE_CLASS_ID_LIST) {
                /* Check if the attribute value is a data element sequence */
                if (SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) ==
                    DATA_ELE_SEQ_DESC_TYPE) {
                    /* Loop through the attribute values */
                    for (p_attr = p_attr->attr_value.v.p_sub_attr; p_attr;
                         p_attr = p_attr->p_next_attr) {
                        /* Check if the attribute value is a UUID */
                        if (SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) ==
                            UUID_DESC_TYPE) {
                            /* Compare the UUID */
                            if (sdpu_compare_uuid_with_attr(p_uuid, p_attr)) {
                                return p_rec;
                            }
                        }
                    }
                    break;
                }
            } else if (p_attr->attr_id == ATTR_ID_SERVICE_ID) {
                /* Check if the attribute value is a UUID */
                if (SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) ==
                    UUID_DESC_TYPE) {
                    /* Compare the UUID */
                    if (sdpu_compare_uuid_with_attr(p_uuid, p_attr)) {
                        return p_rec;
                    }
                }
            }
            p_attr = p_attr->p_next_attr;
        }
        p_rec = p_rec->p_next_rec;
    }

    return NULL;
}

UINT16 SDP_SetLocalDiRecord(tSDP_DI_RECORD *device_info, UINT32 *p_handle) { return 0; }

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
