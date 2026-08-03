// High-level C reconstruction of RVL_SDK/src/revolution/bte/stack/gap/gap_utils
//
// NOTE: the retail bte TU was compiled with `-ipa off`, which emits functions
// in REVERSE source order.  The functions below are therefore written
// back-to-front (last retail function first) so the emitted .text order and
// the .data string-pool order match the retail object.

#include <revolution/BTE/stack/include/btm_api.h>
#include <string.h>

/* GAP return codes (values recovered from the retail jump tables; these are
 * the BTE GAP_ERR_* statuses with GAP_ERR_GROUP = 0x100). */
#define BT_PASS                 0x0000
#define GAP_DEVICE_DISCOVERY     0x0111
#define GAP_EOINQDB             0x0102
#define GAP_ERR_BUSY            0x0103
#define GAP_ERR_ILL_PARM        0x0109
#define GAP_CMD_INITIATED       0x010B
#define GAP_DEVICE_NOT_UP       0x010C
#define GAP_BAD_BD_ADDR         0x010D
#define GAP_ERR_PROCESSING      0x0114
#define GAP_ERR_TIMEOUT         0x0115

/* FindAddr completion event passed to the upper-layer callback. */
#define GAP_EVT_FIND_ADDR_COMPLETE 0x0005

typedef void (tGAP_CALLBACK)(UINT16 event, void *p_data);

/* Result block filled in by the FindAddr-by-name search. */
typedef struct
{
    UINT16      status;         /* 0x00 */
    BD_ADDR     bd_addr;        /* 0x02 */
    UINT8       devname[0x41];  /* 0x08 */
} tGAP_FINDADDR_RESULTS;        /* 0x4A total */

/* FindAddr control block (gap_cb+0x2C). */
typedef struct
{
    tGAP_CALLBACK *p_cback;     /* 0x00 */
    tBTM_INQ_INFO *p_cur_inq;   /* 0x04 */
    tGAP_FINDADDR_RESULTS results; /* 0x08 */
    BOOLEAN       in_use;       /* 0x52 */
} tGAP_FINDADDR_CB;             /* 0x54 total */

/* GAP client control block (16 bytes each; indexed by BTM callback id). */
typedef struct
{
    UINT16          gap_handle;  /* 0x00 */
    UINT16          pad0;        /* 0x02 */
    tGAP_CALLBACK  *p_cback;     /* 0x04 */
    UINT16          ccb_idx;     /* 0x08 */
    UINT16          pad1;        /* 0x0A */
    UINT16          event;       /* 0x0C */
    UINT8           pad2;        /* 0x0E */
    BOOLEAN         in_use;      /* 0x0F */
} tGAP_CCB;                      /* 0x10 */

/* Mini result record built for the Inquiry Complete callback. */
typedef struct
{
    UINT16          status;      /* 0x00 */
    UINT8           num_resp;    /* 0x02 */
} tGAP_INQ_CMPL;                 /* 0x04 */

/* Retail-layout view of the gap_cb global used by this unit. */
typedef struct
{
    tGAP_CCB    ccb[2];                    /* 0x00 */
    void        *btm_cback[2];             /* 0x20 */
    UINT8       trace_level;               /* 0x28 */
    UINT8       reserved2[3];              /* 0x29 */
    tGAP_FINDADDR_CB findaddr_cb;          /* 0x2C */
    tBTM_INQ_INFO *cur_inqptr;             /* 0x80 */
    u8 tail[0x3B0 - 0x84];                 /* 0x84..0x3B0 (retail tail + 4 align pad, unused here) */
} tGAP_CB;                                 /* 0x3B0 total = retail .bss 0x3AC + 4 pad */

tGAP_CB gap_cb; /* retail .bss 0x3AC + 4 pad; defined here (extern in other units) */

void btm_cback(UINT16 index, void *p_msg);
void gap_find_addr_name_cb(tBTM_REMOTE_DEV_NAME *p);

UINT16 gap_convert_btm_status(tBTM_STATUS btm_status);

UINT16 gap_convert_btm_status(tBTM_STATUS btm_status)
{
    switch (btm_status) {
    case BTM_SUCCESS:
        return BT_PASS;
    case BTM_CMD_STARTED:
        return GAP_CMD_INITIATED;
    case BTM_BUSY:
        return GAP_ERR_BUSY;
    case BTM_MODE_UNSUPPORTED:
    case BTM_ILLEGAL_VALUE:
        return GAP_ERR_ILL_PARM;
    case BTM_WRONG_MODE:
        return GAP_DEVICE_NOT_UP;
    case BTM_UNKNOWN_ADDR:
        return GAP_BAD_BD_ADDR;
    case BTM_DEVICE_TIMEOUT:
        return GAP_ERR_TIMEOUT;
    default:
        return GAP_ERR_PROCESSING;
    }
}

void gap_find_addr_inq_cb(tBTM_INQUIRY_CMPL *p)
{
    tGAP_FINDADDR_CB *p_cb = &gap_cb.findaddr_cb;
    tGAP_FINDADDR_RESULTS *p_result = &p_cb->results;
    UINT16 code;

    if (!p_cb->in_use)
        return;

    if (gap_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
    {
        LogMsg_2(TRACE_CTRL_GENERAL | TRACE_LAYER_GAP | TRACE_ORG_STACK | TRACE_TYPE_EVENT,
                 "   GAP: FindAddrByName Inq Cmpl Evt (Status 0x%04x, Result(s) %d)",
                 (UINT32)p->status, (UINT32)p->num_resp);
    }

    if (p->status == BTM_SUCCESS)
    {
        p_result->status = GAP_DEVICE_DISCOVERY;
        if ((p_cb->p_cur_inq = BTM_InqDbFirst()) != NULL)
        {
            if (BTM_ReadRemoteDeviceName(p_cb->p_cur_inq->results.remote_bd_addr,
                                         (tBTM_CMPL_CB *)&gap_find_addr_name_cb) == BTM_CMD_STARTED)
            {
                return;
            }

            switch (p->status)
            {
            case BTM_SUCCESS:
                code = BT_PASS;
                break;
            case BTM_CMD_STARTED:
                code = GAP_CMD_INITIATED;
                break;
            case BTM_BUSY:
                code = GAP_ERR_BUSY;
                break;
            case BTM_MODE_UNSUPPORTED:
            case BTM_ILLEGAL_VALUE:
                code = GAP_ERR_ILL_PARM;
                break;
            case BTM_WRONG_MODE:
                code = GAP_DEVICE_NOT_UP;
                break;
            case BTM_UNKNOWN_ADDR:
                code = GAP_BAD_BD_ADDR;
                break;
            case BTM_DEVICE_TIMEOUT:
                code = GAP_ERR_TIMEOUT;
                break;
            default:
                code = GAP_ERR_PROCESSING;
                break;
            }
            p_result->status = code;
        }
        else
        {
            p_result->status = GAP_EOINQDB;
        }
    }
    else
    {
        switch (p->status)
        {
        case BTM_SUCCESS:
            code = BT_PASS;
            break;
        case BTM_CMD_STARTED:
            code = GAP_CMD_INITIATED;
            break;
        case BTM_BUSY:
            code = GAP_ERR_BUSY;
            break;
        case BTM_MODE_UNSUPPORTED:
        case BTM_ILLEGAL_VALUE:
            code = GAP_ERR_ILL_PARM;
            break;
        case BTM_WRONG_MODE:
            code = GAP_DEVICE_NOT_UP;
            break;
        case BTM_UNKNOWN_ADDR:
            code = GAP_BAD_BD_ADDR;
            break;
        case BTM_DEVICE_TIMEOUT:
            code = GAP_ERR_TIMEOUT;
            break;
        default:
            code = GAP_ERR_PROCESSING;
            break;
        }
        p_result->status = code;
    }

    if (p_cb->p_cback)
    {
        (*p_cb->p_cback)(GAP_EVT_FIND_ADDR_COMPLETE, p_result);
    }

    p_cb->in_use = FALSE;
    p_cb->p_cback = NULL;
}

void gap_find_addr_name_cb(tBTM_REMOTE_DEV_NAME *p)
{
    tGAP_FINDADDR_CB *p_cb = &gap_cb.findaddr_cb;
    tGAP_FINDADDR_RESULTS *p_result = &p_cb->results;

    if (!p_cb->in_use)
        return;

    if (p->status == BTM_SUCCESS)
    {
        if (gap_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        {
            LogMsg_2(TRACE_CTRL_GENERAL | TRACE_LAYER_GAP | TRACE_ORG_STACK | TRACE_TYPE_EVENT,
                     "   GAP: FindAddrByName Rem Name Cmpl Evt (Status 0x%04x, Name [%s])",
                     (UINT32)p->status, (UINT32)p->remote_bd_name);
        }

        if (!strncmp((char *)p_result->devname, (char *)p->remote_bd_name,
                     strlen((char *)p_result->devname)))
        {
            memcpy(p_result->bd_addr, p_cb->p_cur_inq->results.remote_bd_addr, BD_ADDR_LEN);
            p_result->status = BT_PASS;
        }
        else
        {
            if ((p_cb->p_cur_inq = BTM_InqDbNext(p_cb->p_cur_inq)) != NULL)
            {
                if (BTM_ReadRemoteDeviceName(p_cb->p_cur_inq->results.remote_bd_addr,
                                             (tBTM_CMPL_CB *)&gap_find_addr_name_cb) == BTM_CMD_STARTED)
                {
                    return;
                }
                else
                {
                    p_result->status = gap_convert_btm_status((tBTM_STATUS)p->status);
                }
            }
            else
            {
                p_result->status = GAP_EOINQDB;
            }
        }
    }
    else
    {
        if (gap_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        {
            LogMsg_1(TRACE_CTRL_GENERAL | TRACE_LAYER_GAP | TRACE_ORG_STACK | TRACE_TYPE_EVENT,
                     "   GAP: FindAddrByName Rem Name Cmpl Evt (Status 0x%04x)",
                     (UINT32)p->status);
        }
        p_result->status = gap_convert_btm_status((tBTM_STATUS)p->status);
    }

    if (p_cb->p_cback)
    {
        (*p_cb->p_cback)(GAP_EVT_FIND_ADDR_COMPLETE, p_result);
    }

    p_cb->in_use = FALSE;
    p_cb->p_cback = NULL;
}

void gap_btm_cback1(void *p_msg)
{
    btm_cback(1, p_msg);
}

void gap_btm_cback0(void *p_msg)
{

    btm_cback(0, p_msg);
}

/* BTM event dispatcher: index selects the GAP control block (0 or 1),
 * p_msg is the BTM event message. gap_btm_cback0/1 tail-call into it. */
#pragma push
#pragma auto_inline off
/* Retail keeps 2 dead strings (bdaddr/COD logging from the original inq results
   callback) at .data 0xF0/0x134; referenced from .init so they land right after
   btm_cback's data (first in -ipa off reverse order) without extra .text. */
void fake_function(...);
__declspec(section ".init") void FORCEACTIVEgap_utils_keep(void) {
    fake_function("GAP Inquiry Results Callback (bdaddr [%02x%02x%02x%02x%02x%02x])",
                  "                             (COD [%02x%02x%02x], clkoff 0x%04x)");
}

void btm_cback(UINT16 index, void *p_msg)
{
    tGAP_CCB *p_ccb;

    if (index >= 2)
    {
        return;
    }

    p_ccb = &gap_cb.ccb[index];
    if (!p_ccb->in_use)
    {
        return;
    }

    switch (p_ccb->event)
    {
    case 2: /* BTM_INQUIRY_COMPLETE_EVT */
    {
        tBTM_INQUIRY_CMPL *p = (tBTM_INQUIRY_CMPL *)p_msg;
        tGAP_INQ_CMPL inq;

        inq.num_resp = p->num_resp;
        inq.status = (p->status == BTM_SUCCESS) ? BT_PASS : GAP_ERR_PROCESSING;
        p_msg = &inq;

        if (gap_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        {
            LogMsg_2(TRACE_CTRL_GENERAL | TRACE_LAYER_GAP | TRACE_ORG_STACK | TRACE_TYPE_EVENT,
                     "   GAP Inquiry Complete Event (Status 0x%04x, Result(s) %d)",
                     inq.status, inq.num_resp);
        }
        break;
    }

    case 3: /* BTM_DISCOVERY_COMPLETE_EVT */
    {
        UINT16 result = *(UINT16 *)p_msg;

        if (result != 0)
        {
            if (gap_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
            {
                LogMsg_1(TRACE_CTRL_GENERAL | TRACE_LAYER_GAP | TRACE_ORG_STACK | TRACE_TYPE_EVENT,
                         "   GAP Discovery Complete Event(SDP Result: 0x%04x)", result);
            }
        }
        else
        {
            if (gap_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
            {
                LogMsg_0(TRACE_CTRL_GENERAL | TRACE_LAYER_GAP | TRACE_ORG_STACK | TRACE_TYPE_EVENT,
                         "   GAP Discovery Successfully Completed");
            }
        }
        break;
    }

    case 4: /* BTM_REMOTE_DEVICE_NAME_EVT */
    {
        tBTM_REMOTE_DEV_NAME *p = (tBTM_REMOTE_DEV_NAME *)p_msg;
        UINT16 code;

        switch ((UINT8)p->status)
        {
        case BTM_SUCCESS:
            code = BT_PASS;
            break;
        case BTM_CMD_STARTED:
            code = GAP_CMD_INITIATED;
            break;
        case BTM_BUSY:
            code = GAP_ERR_BUSY;
            break;
        case BTM_MODE_UNSUPPORTED:
        case BTM_ILLEGAL_VALUE:
            code = GAP_ERR_ILL_PARM;
            break;
        case BTM_WRONG_MODE:
            code = GAP_DEVICE_NOT_UP;
            break;
        case BTM_UNKNOWN_ADDR:
            code = GAP_BAD_BD_ADDR;
            break;
        case BTM_DEVICE_TIMEOUT:
            code = GAP_ERR_TIMEOUT;
            break;
        default:
            code = GAP_ERR_PROCESSING;
            break;
        }

        p->status = code;

        if (gap_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        {
            LogMsg_1(TRACE_CTRL_GENERAL | TRACE_LAYER_GAP | TRACE_ORG_STACK | TRACE_TYPE_EVENT,
                     "   GAP Remote Name Complete Event (status 0x%04x)", code);
        }
        break;
    }
    }

    if (p_ccb->p_cback)
    {
        p_ccb->p_cback(p_ccb->event, p_msg);
    }

    if (p_ccb)
    {
        p_ccb->p_cback = NULL;
        p_ccb->in_use = FALSE;
    }
}
#pragma pop
