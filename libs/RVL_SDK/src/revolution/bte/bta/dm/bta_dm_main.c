// Decompiled state machine dispatch tables and executors for BTA device manager.

#include <types.h>
#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/gki/platform/data_types.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/bta/sys/bta_sys.h>

/* --- Forward declarations --- */

struct bta_dm_msg;

/* --- DM state machine constants --- */

#define BTA_DM_NUM_ACTIONS  2
#define BTA_DM_NUM_COLS     3

/* BTA_DM_MAX_EVENT = 13, so BTA_DM_IGNORE = 13 = 0xd */
#define BTA_DM_IGNORE       0xd

/* DM event IDs (offset from BTA_SYS_EVT_START(BTA_ID_DM)) */
enum
{
    BTA_DM_API_ENABLE_EVT,
    BTA_DM_API_DISABLE_EVT,
    BTA_DM_API_SET_NAME_EVT,
    BTA_DM_API_SET_VISIBILITY_EVT,
    BTA_DM_API_SIG_STRENGTH_EVT,
    BTA_DM_ACL_CHANGE_EVT,
    BTA_DM_API_BOND_EVT,
    BTA_DM_API_PIN_REPLY_EVT,
    BTA_DM_API_AUTH_REPLY_EVT,
    BTA_DM_PM_BTM_STATUS_EVT,
    BTA_DM_PM_TIMER_EVT,
    BTA_DM_API_KEEP_ACL_LINKS,
    BTA_DM_API_RESET_HCI,
    BTA_DM_MAX_EVENT
};

/* DM action function pointer type */
typedef void (*tBTA_DM_ACTION)(struct bta_dm_msg *p_data);

/* DM action functions (defined in bta_dm_act.c) */
extern void bta_dm_enable(struct bta_dm_msg *p_data);
extern void bta_dm_disable(struct bta_dm_msg *p_data);
extern void bta_dm_set_dev_name(struct bta_dm_msg *p_data);
extern void bta_dm_set_visibility(struct bta_dm_msg *p_data);
extern void bta_dm_bond(struct bta_dm_msg *p_data);
extern void bta_dm_pin_reply(struct bta_dm_msg *p_data);
extern void bta_dm_auth_reply(struct bta_dm_msg *p_data);
extern void bta_dm_signal_strength(struct bta_dm_msg *p_data);
extern void bta_dm_acl_change(struct bta_dm_msg *p_data);
extern void bta_dm_pm_btm_status(struct bta_dm_msg *p_data);
extern void bta_dm_pm_timer(struct bta_dm_msg *p_data);
extern void bta_dm_keep_acl(struct bta_dm_msg *p_data);
extern void bta_dm_send_hci_reset(struct bta_dm_msg *p_data);

/* DM action table: maps action index -> function pointer */
tBTA_DM_ACTION * const bta_dm_action[] =
{
    bta_dm_enable,
    bta_dm_disable,
    bta_dm_set_dev_name,
    bta_dm_set_visibility,
    bta_dm_bond,
    bta_dm_pin_reply,
    bta_dm_auth_reply,
    bta_dm_signal_strength,
    bta_dm_acl_change,
    bta_dm_pm_btm_status,
    bta_dm_pm_timer,
    bta_dm_keep_acl,
    bta_dm_send_hci_reset,
};

/*
 * DM state table: indexed as table[event][action_slot].
 * Each event maps to up to 2 actions; BTA_DM_IGNORE (0xd) terminates the slot list.
 * The third column is unused padding to match the 3-byte stride in the retail binary.
 */
const UINT8 bta_dm_st_table[][BTA_DM_NUM_COLS] =
{
    { BTA_DM_API_ENABLE_EVT,       BTA_DM_IGNORE, 0 },
    { BTA_DM_API_DISABLE_EVT,      BTA_DM_IGNORE, 0 },
    { BTA_DM_API_SET_NAME_EVT,     BTA_DM_IGNORE, 0 },
    { BTA_DM_API_SET_VISIBILITY_EVT, BTA_DM_IGNORE, 0 },
    { BTA_DM_API_PIN_REPLY_EVT,    BTA_DM_IGNORE, 0 },
    { BTA_DM_API_AUTH_REPLY_EVT,   BTA_DM_IGNORE, 0 },
    { BTA_DM_API_SIG_STRENGTH_EVT, BTA_DM_IGNORE, 0 },
    { BTA_DM_ACL_CHANGE_EVT,       BTA_DM_IGNORE, 0 },
    { BTA_DM_API_BOND_EVT,         BTA_DM_IGNORE, 0 },
    { BTA_DM_PM_BTM_STATUS_EVT,    BTA_DM_IGNORE, 0 },
    { BTA_DM_PM_TIMER_EVT,         BTA_DM_IGNORE, 0 },
    { BTA_DM_API_KEEP_ACL_LINKS,   BTA_DM_IGNORE, 0 },
    { BTA_DM_API_RESET_HCI,        BTA_DM_IGNORE, 0 },
};

/* --- Search state machine constants --- */

#define BTA_DM_SEARCH_NUM_ACTIONS  2
#define BTA_DM_SEARCH_NUM_COLS     3
#define BTA_DM_SEARCH_NEXT_STATE   2

/* BTA_DM_SEARCH_MAX_ACTION = 18, so BTA_DM_SEARCH_IGNORE = 18 = 0x12 */
#define BTA_DM_SEARCH_IGNORE       0x12

/* Search action enum */
enum
{
    BTA_DM_API_SEARCH,
    BTA_DM_API_SEARCH_CANCEL,
    BTA_DM_API_DISCOVER,
    BTA_DM_INQUIRY_CMPL,
    BTA_DM_REMT_NAME,
    BTA_DM_SDP_RESULT,
    BTA_DM_SEARCH_CMPL,
    BTA_DM_FREE_SDP_DB,
    BTA_DM_DISC_RESULT,
    BTA_DM_SEARCH_RESULT,
    BTA_DM_QUEUE_SEARCH,
    BTA_DM_QUEUE_DISC,
    BTA_DM_SEARCH_CLEAR_QUEUE,
    BTA_DM_SEARCH_CANCEL_CMPL,
    BTA_DM_SEARCH_CANCEL_NOTIFY,
    BTA_DM_SEARCH_CANCEL_TRANSAC_CMPL,
    BTA_DM_DISC_RMT_NAME,
    BTA_DM_API_DI_DISCOVER,
    BTA_DM_SEARCH_MAX_ACTION
};

/* Search state enum */
enum
{
    BTA_DM_SEARCH_IDLE,
    BTA_DM_SEARCH_ACTIVE,
    BTA_DM_SEARCH_CANCELLING,
    BTA_DM_DISCOVER_ACTIVE,
};

/* Search action function pointer type */
typedef void (*tBTA_DM_SEARCH_ACTION)(struct bta_dm_msg *p_data);

/* Search action functions (defined in bta_dm_act.c) */
extern void bta_dm_search_start(struct bta_dm_msg *p_data);
extern void bta_dm_search_cancel(struct bta_dm_msg *p_data);
extern void bta_dm_discover(struct bta_dm_msg *p_data);
extern void bta_dm_inq_cmpl(struct bta_dm_msg *p_data);
extern void bta_dm_rmt_name(struct bta_dm_msg *p_data);
extern void bta_dm_sdp_result(struct bta_dm_msg *p_data);
extern void bta_dm_search_cmpl(struct bta_dm_msg *p_data);
extern void bta_dm_free_sdp_db(struct bta_dm_msg *p_data);
extern void bta_dm_disc_result(struct bta_dm_msg *p_data);
extern void bta_dm_search_result(struct bta_dm_msg *p_data);
extern void bta_dm_queue_search(struct bta_dm_msg *p_data);
extern void bta_dm_queue_disc(struct bta_dm_msg *p_data);
extern void bta_dm_search_clear_queue(struct bta_dm_msg *p_data);
extern void bta_dm_search_cancel_cmpl(struct bta_dm_msg *p_data);
extern void bta_dm_search_cancel_notify(struct bta_dm_msg *p_data);
extern void bta_dm_search_cancel_transac_cmpl(struct bta_dm_msg *p_data);
extern void bta_dm_disc_rmt_name(struct bta_dm_msg *p_data);
extern void bta_dm_cancel_rmt_name(struct bta_dm_msg *p_data);

/* Search action table: maps action index -> function pointer */
tBTA_DM_SEARCH_ACTION * const bta_dm_search_action[] =
{
    bta_dm_search_start,
    bta_dm_search_cancel,
    bta_dm_discover,
    bta_dm_inq_cmpl,
    bta_dm_rmt_name,
    bta_dm_sdp_result,
    bta_dm_search_cmpl,
    bta_dm_free_sdp_db,
    bta_dm_disc_result,
    bta_dm_search_result,
    bta_dm_queue_search,
    bta_dm_queue_disc,
    bta_dm_search_clear_queue,
    bta_dm_search_cancel_cmpl,
    bta_dm_search_cancel_notify,
    bta_dm_search_cancel_transac_cmpl,
    bta_dm_disc_rmt_name,
    bta_dm_cancel_rmt_name,
};

/* Search control block (defined in bta_dm_act.c) */
struct bta_dm_search_cb_t
{
    void *p_search_cback;       /* offset 0x00 */
    UINT8 _pad4[0x14];          /* offset 0x04-0x17 */
    UINT16 state;               /* offset 0x18 */
    UINT8 _pad1a[0x06];         /* offset 0x1a-0x1f */
    char peer_name[0x20];       /* offset 0x20 */
    UINT8 _pad40[0x34];         /* offset 0x40-0x73 */
    void *p_search_queue;       /* offset 0x74 */
};

extern struct bta_dm_search_cb_t bta_dm_search_cb;

/*
 * Search state tables: one per state, indexed as table[event][col].
 * Column 0 = action 1, column 1 = action 2, column 2 = next state.
 * BTA_DM_SEARCH_IGNORE (0x12) in an action slot terminates the action list.
 */

/* IDLE state: accepts API_SEARCH, API_SEARCH_CANCEL, API_DISCOVER, SDP_RESULT, API_DI_DISCOVER */
static const UINT8 bta_dm_search_idle_st_table[][BTA_DM_SEARCH_NUM_COLS] =
{
    /* API_SEARCH */          { BTA_DM_API_SEARCH,              BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_ACTIVE },
    /* API_SEARCH_CANCEL */   { BTA_DM_SEARCH_CANCEL_NOTIFY,    BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* API_DISCOVER */        { BTA_DM_API_DISCOVER,            BTA_DM_SEARCH_IGNORE, BTA_DM_DISCOVER_ACTIVE },
    /* INQUIRY_CMPL */        { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* REMT_NAME */           { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* SDP_RESULT */          { BTA_DM_FREE_SDP_DB,             BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* SEARCH_CMPL */         { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* DISCOVERY_RESULT */    { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* API_DI_DISCOVER */     { BTA_DM_API_DI_DISCOVER,         BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_ACTIVE },
};

/* SEARCH_ACTIVE state */
static const UINT8 bta_dm_search_search_active_st_table[][BTA_DM_SEARCH_NUM_COLS] =
{
    /* API_SEARCH */          { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_ACTIVE },
    /* API_SEARCH_CANCEL */   { BTA_DM_API_SEARCH_CANCEL,       BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_CANCELLING },
    /* API_DISCOVER */        { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_ACTIVE },
    /* INQUIRY_CMPL */        { BTA_DM_INQUIRY_CMPL,            BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_ACTIVE },
    /* REMT_NAME */           { BTA_DM_REMT_NAME,               BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_ACTIVE },
    /* SDP_RESULT */          { BTA_DM_SDP_RESULT,              BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_ACTIVE },
    /* SEARCH_CMPL */         { BTA_DM_SEARCH_CMPL,             BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* DISCOVERY_RESULT */    { BTA_DM_SEARCH_RESULT,           BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_ACTIVE },
    /* API_DI_DISCOVER */     { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_ACTIVE },
};

/* SEARCH_CANCELLING state */
static const UINT8 bta_dm_search_search_cancelling_st_table[][BTA_DM_SEARCH_NUM_COLS] =
{
    /* API_SEARCH */          { BTA_DM_QUEUE_SEARCH,            BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_CANCELLING },
    /* API_SEARCH_CANCEL */   { BTA_DM_SEARCH_CLEAR_QUEUE,      BTA_DM_SEARCH_CANCEL_NOTIFY, BTA_DM_SEARCH_CANCELLING },
    /* API_DISCOVER */        { BTA_DM_QUEUE_DISC,              BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_CANCELLING },
    /* INQUIRY_CMPL */        { BTA_DM_SEARCH_CANCEL_CMPL,      BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* REMT_NAME */           { BTA_DM_SEARCH_CANCEL_TRANSAC_CMPL, BTA_DM_SEARCH_CANCEL_CMPL, BTA_DM_SEARCH_IDLE },
    /* SDP_RESULT */          { BTA_DM_SEARCH_CANCEL_TRANSAC_CMPL, BTA_DM_SEARCH_CANCEL_CMPL, BTA_DM_SEARCH_IDLE },
    /* SEARCH_CMPL */         { BTA_DM_SEARCH_CANCEL_CMPL,      BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* DISCOVERY_RESULT */    { BTA_DM_SEARCH_CANCEL_CMPL,      BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* API_DI_DISCOVER */     { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_CANCELLING },
};

/* DISCOVER_ACTIVE state */
static const UINT8 bta_dm_search_disc_active_st_table[][BTA_DM_SEARCH_NUM_COLS] =
{
    /* API_SEARCH */          { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_DISCOVER_ACTIVE },
    /* API_SEARCH_CANCEL */   { BTA_DM_SEARCH_CANCEL_NOTIFY,    BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_CANCELLING },
    /* API_DISCOVER */        { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_DISCOVER_ACTIVE },
    /* INQUIRY_CMPL */        { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_DISCOVER_ACTIVE },
    /* REMT_NAME */           { BTA_DM_DISC_RMT_NAME,           BTA_DM_SEARCH_IGNORE, BTA_DM_DISCOVER_ACTIVE },
    /* SDP_RESULT */          { BTA_DM_SDP_RESULT,              BTA_DM_SEARCH_IGNORE, BTA_DM_DISCOVER_ACTIVE },
    /* SEARCH_CMPL */         { BTA_DM_SEARCH_CMPL,             BTA_DM_SEARCH_IGNORE, BTA_DM_SEARCH_IDLE },
    /* DISCOVERY_RESULT */    { BTA_DM_DISC_RESULT,             BTA_DM_SEARCH_IGNORE, BTA_DM_DISCOVER_ACTIVE },
    /* API_DI_DISCOVER */     { BTA_DM_SEARCH_IGNORE,           BTA_DM_SEARCH_IGNORE, BTA_DM_DISCOVER_ACTIVE },
};

/* Table of per-state state tables, indexed by current state */
typedef const UINT8 (*tBTA_DM_ST_TBL)[BTA_DM_SEARCH_NUM_COLS];

const tBTA_DM_ST_TBL bta_dm_search_st_tbl[] =
{
    bta_dm_search_idle_st_table,
    bta_dm_search_search_active_st_table,
    bta_dm_search_search_cancelling_st_table,
    bta_dm_search_disc_active_st_table,
};

/*
 * bta_dm_sm_execute: DM state machine event handler.
 * Iterates over action slots in the DM state table for the given event.
 * Stops at the first BTA_DM_IGNORE slot.
 */
BOOLEAN bta_dm_sm_execute(BT_HDR *p_msg)
{
    const UINT8 (*state_table)[BTA_DM_NUM_COLS] = bta_dm_st_table;
    UINT8 action;
    int i;

    for (i = 0; i < BTA_DM_NUM_ACTIONS; i++)
    {
        action = state_table[p_msg->event & 0xff][i];
        if (action == BTA_DM_IGNORE)
            break;
        bta_dm_action[action]((struct bta_dm_msg *)p_msg);
    }

    return TRUE;
}

/*
 * bta_dm_search_sm_execute: Search state machine event handler.
 * Looks up the per-state table for the current state, updates the state
 * from the NEXT_STATE column, then executes action slots until BTA_DM_SEARCH_IGNORE.
 */
BOOLEAN bta_dm_search_sm_execute(BT_HDR *p_msg)
{
    tBTA_DM_ST_TBL state_table;
    UINT8 action;
    int i;

    /* Look up the state table for the current state */
    state_table = bta_dm_search_st_tbl[bta_dm_search_cb.state];

    /* Update state from the NEXT_STATE column (column 2) of row[event] */
    bta_dm_search_cb.state = state_table[p_msg->event & 0xff][BTA_DM_SEARCH_NEXT_STATE];

    /* Execute action functions */
    for (i = 0; i < BTA_DM_SEARCH_NUM_ACTIONS; i++)
    {
        action = state_table[p_msg->event & 0xff][i];
        if (action == BTA_DM_SEARCH_IGNORE)
            break;
        bta_dm_search_action[action]((struct bta_dm_msg *)p_msg);
    }

    return TRUE;
}
