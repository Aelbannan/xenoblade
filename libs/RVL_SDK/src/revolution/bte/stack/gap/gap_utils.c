// High-level C reconstruction of RVL_SDK/src/revolution/bte/stack/gap/gap_utils

#include <revolution/BTE/stack/include/btm_api.h>

/* GAP return codes (not present in the RVL SDK headers; values recovered
 * from the retail gap_convert_btm_status jump table / data section). */
#define GAP_SUCCESS          0x0000
#define GAP_CMD_INITIATED    0x010B
#define GAP_ERR_BUSY         0x0103
#define GAP_ERR_ILLEGAL_PARAM 0x0109
#define GAP_ERR_NOT_READY    0x010C
#define GAP_ERR_BAD_BD_ADDR  0x010D
#define GAP_ERR_PROCESSING   0x0114
#define GAP_DEVICE_TIMEOUT   0x0115

/* BTM event dispatcher: index selects the GAP control block (0 or 1),
 * p_msg is the BTM event message. gap_btm_cback0/1 tail-call into it. */
#pragma push
#pragma auto_inline off
void btm_cback(UINT16 index, void *p_msg) {}
#pragma pop

void gap_btm_cback0(void *p_msg)
{
    btm_cback(0, p_msg);
}

void gap_btm_cback1(void *p_msg)
{
    btm_cback(1, p_msg);
}

void gap_find_addr_name_cb() {}

void gap_find_addr_inq_cb() {}

UINT16 gap_convert_btm_status(tBTM_STATUS btm_status)
{
    switch (btm_status) {
    case BTM_SUCCESS:
        return GAP_SUCCESS;
    case BTM_CMD_STARTED:
        return GAP_CMD_INITIATED;
    case BTM_BUSY:
        return GAP_ERR_BUSY;
    case BTM_MODE_UNSUPPORTED:
    case BTM_ILLEGAL_VALUE:
        return GAP_ERR_ILLEGAL_PARAM;
    case BTM_WRONG_MODE:
        return GAP_ERR_NOT_READY;
    case BTM_UNKNOWN_ADDR:
        return GAP_ERR_BAD_BD_ADDR;
    case BTM_DEVICE_TIMEOUT:
        return GAP_DEVICE_TIMEOUT;
    default:
        return GAP_ERR_PROCESSING;
    }
}
