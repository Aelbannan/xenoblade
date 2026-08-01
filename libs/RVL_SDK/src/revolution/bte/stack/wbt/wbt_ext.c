// Broadcom BTE WBT extension: create the local DI (Device ID) record.
// Retail SDK used SDP_MAX_ATTR_LEN=80 (struct 250 bytes matching 0xFA memset).
// handle is volatile: the SDP calls take &handle and can modify it; MWCC keeps
// the failure-path handle=0 store only when handle is volatile (the 0 must live
// in r31 across the calls) - matches retail byte-for-byte (FULL_MATCH).
#define SDP_MAX_ATTR_LEN 80

#include <string.h>
#include <revolution/bte/stack/include/sdp_api.h>

/* Broadcom extension attribute ID (not in standard headers) */
#ifndef ATTR_ID_EXT_BRCM_VERSION
#define ATTR_ID_EXT_BRCM_VERSION 0x8001
#endif

BOOLEAN WBT_ExtCreateRecord(void)
{
    tSDP_DI_GET_RECORD di_record;
    volatile UINT32 handle;
    tSDP_DI_RECORD *rec;
    UINT8  attr_val[2];
    UINT16 uuid;

    handle = 0;
    uuid = 0x1002;

    if (SDP_GetLocalDiRecord(&di_record, (UINT32 *)&handle) != SDP_SUCCESS)
    {
        rec = &di_record.rec;
        memset(rec, 0, sizeof(tSDP_DI_RECORD));
        rec->vendor = 0x000F;
        rec->vendor_id_source = 0x0001;
        rec->primary_record = TRUE;

        if (SDP_SetLocalDiRecord(rec, (UINT32 *)&handle) != SDP_SUCCESS)
        {
            handle = 0;
            return FALSE;
        }
    }

    SDP_AddUuidSequence(handle, ATTR_ID_BROWSE_GROUP_LIST, 1, &uuid);

    attr_val[0] = 0;
    attr_val[1] = 1;
    SDP_AddAttribute(handle, ATTR_ID_EXT_BRCM_VERSION, UINT_DESC_TYPE, 2, attr_val);

    return TRUE;
}
