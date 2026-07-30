#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/stack/include/hiddefs.h>

/* LogMsg_0 is declared in bt_trace.h, but that header has unmet dependencies.
   Declare it directly here. */
extern void LogMsg_0(UINT32 trace_set_mask, const char *p_str);

/* L2CA_ConnectReq from l2c_api.h */
extern UINT16 L2CA_ConnectReq(UINT16 psm, void *p_bd_addr);

/* HID Device control block - defined in hidd_mgmt.c */
typedef struct
{
    UINT8       reserved_BD_ADDR[6]; /* +0x00: BD_ADDR */
    UINT8       pad1[0x0A];          /* +0x06 */
    UINT8       in_use;              /* +0x10 */
    UINT8       conn_state;          /* +0x11 */
    UINT8       pad2[0x02];          /* +0x12 */
    UINT16      conn_result;         /* +0x14 */
    UINT16      disc_reason;         /* +0x16 */
    UINT8       pad3[0x129];         /* +0x18 */
    UINT8       trace_level;         /* +0x141 */
    UINT8       pad4[0x02];          /* +0x142 */
} tHIDD_CB;

extern tHIDD_CB hd_cb;

/*******************************************************************************
**
** Function         hidd_conn_initiate
**
** Description      This function is called to initiate a HID connection.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
UINT8 hidd_conn_initiate(void)
{
    tHIDD_CB *p_cb;
    UINT16 result;
    UINT8 one;

    p_cb = &hd_cb;
    one = 1;

    if (p_cb->trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_0(0x001e0003, "HID - Originate started");
    }

    p_cb->conn_result = 0;
    p_cb->disc_reason = 0;
    p_cb->conn_state = one;

    result = L2CA_ConnectReq(HID_PSM_CONTROL, (void *)&hd_cb);
    p_cb->conn_result = result;

    if (result == 0)
    {
        if (((tHIDD_CB *)&hd_cb)->trace_level >= BT_TRACE_LEVEL_WARNING)
        {
            LogMsg_0(0x001e0001, "HID - Originate failed");
        }
        return HID_ERR_L2CAP_FAILED;
    }

    p_cb->in_use = one;
    return HID_SUCCESS;
}
