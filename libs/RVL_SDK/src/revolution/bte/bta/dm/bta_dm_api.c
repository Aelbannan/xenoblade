// Decompiled BTA device-manager API layer (bta_dm_api.c).
//
// The message-driven APIs allocate a GKI buffer, stamp the DM/Search event
// code and parameters into it, and hand it to the state machines via
// bta_sys_sendmsg.  BTA_DmIsDeviceUp / BTA_DmAddDevice / BTA_DmRemoveDevice
// execute directly under the GKI scheduler lock.

#include <harness_catalog.h>
#include <string.h>

#include <revolution/bte/bta/dm/bta_dm_int.h>

/* --- Basic types --- */

typedef unsigned char BOOLEAN;
typedef unsigned char tBTA_STATUS;

#define BTA_SUCCESS  0
#define BTA_FAILURE  1

/* DM security callback (tBTA_DM_SEC_CBACK) */
typedef void (*tBTA_DM_SEC_CBACK)(unsigned char event, void *p_data);

/* --- Registration descriptor (tBTA_SYS_REG) --- */

struct bta_sys_reg_t {
    BOOLEAN (*evt_hdlr)(void *p_msg);
    void (*disable)(void);
};

/* --- bta_dm_cb control block (defined in bta_dm_main.c; layout per
       bta_dm_act.c) --- */

struct bta_dm_timer_t {
    struct bta_dm_timer_t *p_prev;
    struct bta_dm_timer_t *p_next;
    void (*p_cback)(struct bta_dm_timer_t *);
    int ticks;
    int start_time;
    unsigned short type;
    unsigned char in_use;
    unsigned char _pad;
};

struct bta_dm_peer_dev_t {
    unsigned char bd_addr[6];
    unsigned char in_use;
    unsigned char policy;
    unsigned char _pad[3];
};

struct bta_dm_cb_t {
    struct bta_dm_peer_dev_t peer_dev[7];           /* 0x00-0x4c */
    unsigned char num_devices;                      /* 0x4d */
    unsigned char _pad4e[2];                        /* 0x4e-0x4f */
    void (*cback)(int, void *);                     /* 0x50 */
    struct bta_dm_timer_t signal_strength_timer;    /* 0x54, size 0x18 */
    unsigned char signal_strength_mask;             /* 0x6c */
    unsigned char _pad6d[3];                        /* 0x6d-0x6f */
    unsigned short signal_strength_period;          /* 0x70 */
    unsigned char disable_timer_active;             /* 0x72 */
    unsigned char _pad73;                           /* 0x73 */
    struct bta_dm_timer_t disable_timer;            /* 0x74, size 0x18 */
    unsigned char _pad8c[0x6c];                     /* 0x8c-0xf7 */
    unsigned char pin_bd_addr[6];                   /* 0xf8-0xfd */
    unsigned char pin_dev_class[3];                 /* 0xfe-0x100 */
    unsigned char keep_acl;                         /* 0x101 */
    unsigned char _pad102[2];                       /* 0x102-0x103 */
};                                                  /* total 0x104 */

extern struct bta_dm_cb_t bta_dm_cb;

/* --- API message structures ---
   All DM/Search API payload structs (tBTA_DM_API_* msg types, tBTA_DM_INQ,
   the tBTA_DM_MSG union) are provided by <revolution/bte/bta/dm/bta_dm_int.h>. */

/* --- DM/Search API event codes ---
   DM events start at BTA_SYS_EVT_START(BTA_ID_DM) = 0x100 plus the enum
   offset from bta_dm_main.c; Search events start at 0x200. */

enum {
    BTA_DM_API_ENABLE_EVT = 0x100,
    BTA_DM_API_DISABLE_EVT,
    BTA_DM_API_SET_NAME_EVT,
    BTA_DM_API_SET_VISIBILITY_EVT,
    BTA_DM_API_PIN_REPLY_EVT = 0x107,   /* +7: sig strength, acl, bond */
    BTA_DM_API_RESET_HCI_EVT = 0x10C,   /* +12: ... keep acl links */
    BTA_DM_API_SEARCH_EVT = 0x200,      /* BTA_SYS_EVT_START(BTA_ID_DM_SEARCH) */
    BTA_DM_API_SEARCH_CANCEL_EVT,
};

/* --- Registration tables (retail .sdata2) --- */

extern BOOLEAN bta_dm_sm_execute(void *p_msg);
extern BOOLEAN bta_dm_search_sm_execute(void *p_msg);

static const struct bta_sys_reg_t bta_dm_reg = {
    bta_dm_sm_execute,
    NULL
};

static const struct bta_sys_reg_t bta_dm_search_reg = {
    bta_dm_search_sm_execute,
    NULL
};

/* --- Externals --- */

extern void GKI_sched_lock(void);
extern void GKI_sched_unlock(void);
extern void *GKI_getbuf(unsigned short size);
extern void bta_sys_register(unsigned char id, const struct bta_sys_reg_t *p_reg);
extern void bta_sys_sendmsg(void *p_msg);
/* Returns BOOLEAN in the SDK header; declared int here because the retail
   BTA_DmIsDeviceUp copies the raw register result without a byte mask. */
extern int BTM_IsDeviceUp(void);
extern BOOLEAN BTM_SecDeleteDevice(unsigned char *bd_addr);
extern BOOLEAN BTM_SecAddDevice(unsigned char *bd_addr, unsigned char *dev_class,
                                unsigned char *bd_name, unsigned char *features,
                                unsigned int *trusted_mask, unsigned char *link_key);
extern void bdcpy(unsigned char *dst, const unsigned char *src);
extern const unsigned int bta_service_id_to_btm_srv_id_lkup_tbl[24];

/*******************************************************************************
**
** Function         BTA_EnableBluetooth
**
** Description      Initializes BTA and prepares BTA and the Bluetooth
**                  protocol stack for use.  Must be called before any other
**                  API function.
**
** Returns          BTA_SUCCESS if successful, BTA_FAILURE if internal failure.
**
*******************************************************************************/
tBTA_STATUS BTA_EnableBluetooth(tBTA_DM_SEC_CBACK *p_cback)
{
    struct bta_dm_enable_msg_t *p_msg;

    memset(&bta_dm_cb, 0, sizeof(bta_dm_cb));

    GKI_sched_lock();
    bta_sys_register(1, &bta_dm_reg);            /* BTA_ID_DM */
    bta_sys_register(2, &bta_dm_search_reg);     /* BTA_ID_DM_SEARCH */
    GKI_sched_unlock();

    if ((p_msg = (struct bta_dm_enable_msg_t *)GKI_getbuf(
             sizeof(struct bta_dm_enable_msg_t))) != NULL) {
        p_msg->hdr.event = BTA_DM_API_ENABLE_EVT;
        p_msg->p_sec_cback = p_cback;
        bta_sys_sendmsg(p_msg);
        return BTA_SUCCESS;
    }

    return BTA_FAILURE;
}

/*******************************************************************************
**
** Function         BTA_DisableBluetooth
**
** Description      Disables BTA and the Bluetooth protocol stack.
**
** Returns          void
**
*******************************************************************************/
void BTA_DisableBluetooth(void)
{
    struct bta_dm_hdr_t *p_msg;

    if ((p_msg = (struct bta_dm_hdr_t *)GKI_getbuf(
             sizeof(struct bta_dm_hdr_t))) != NULL) {
        p_msg->event = BTA_DM_API_DISABLE_EVT;
        bta_sys_sendmsg(p_msg);
    }
}

/*******************************************************************************
**
** Function         BTA_DmIsDeviceUp
**
** Description      Tests whether the Bluetooth module is up and ready.
**                  Direct execution function that may lock task scheduling.
**
** Returns          TRUE if the module is ready, FALSE otherwise.
**
*******************************************************************************/
BOOL BTA_DmIsDeviceUp() {
    BOOL ret;
    GKI_sched_lock();
    ret = BTM_IsDeviceUp();
    GKI_sched_unlock();
    return ret;
}

/*******************************************************************************
**
** Function         BTA_DmSetDeviceName
**
** Description      Sets the Bluetooth name of the local device.
**
** Returns          void
**
*******************************************************************************/
void BTA_DmSetDeviceName(char *p_name)
{
    struct bta_dm_set_name_msg_t *p_msg;

    if ((p_msg = (struct bta_dm_set_name_msg_t *)GKI_getbuf(
             sizeof(struct bta_dm_set_name_msg_t))) != NULL) {
        p_msg->hdr.event = BTA_DM_API_SET_NAME_EVT;
        strncpy(p_msg->name, p_name, sizeof(p_msg->name));
        bta_sys_sendmsg(p_msg);
    }
}

/*******************************************************************************
**
** Function         BTA_DmSetVisibility
**
** Description      Sets the Bluetooth connectable and discoverable modes of
**                  the local device.
**
** Returns          void
**
*******************************************************************************/
void BTA_DmSetVisibility(unsigned short disc_mode, unsigned short conn_mode)
{
    struct bta_dm_set_visibility_msg_t *p_msg;

    if ((p_msg = (struct bta_dm_set_visibility_msg_t *)GKI_getbuf(
             sizeof(struct bta_dm_set_visibility_msg_t))) != NULL) {
        p_msg->hdr.event = BTA_DM_API_SET_VISIBILITY_EVT;
        p_msg->disc_mode = (unsigned char)disc_mode;
        p_msg->conn_mode = (unsigned char)conn_mode;
        bta_sys_sendmsg(p_msg);
    }
}

/*******************************************************************************
**
** Function         BTA_DmSearch
**
** Description      Searches for peer Bluetooth devices: performs an inquiry,
**                  gets the remote name of each device found, and optionally
**                  does service discovery for the specified services.
**
** Returns          void
**
*******************************************************************************/
void BTA_DmSearch(struct bta_dm_inq_t *p_dm_inq, unsigned int services,
                  void *p_cback)
{
    struct bta_dm_search_msg_t *p_msg;

    if ((p_msg = (struct bta_dm_search_msg_t *)GKI_getbuf(
             sizeof(struct bta_dm_search_msg_t))) != NULL) {
        p_msg->hdr.event = BTA_DM_API_SEARCH_EVT;
        memcpy(&p_msg->inq_params, p_dm_inq, sizeof(struct bta_dm_inq_t));
        p_msg->services = services;
        p_msg->p_cback = p_cback;
        bta_sys_sendmsg(p_msg);
    }
}

/*******************************************************************************
**
** Function         BTA_DmSearchCancel
**
** Description      Cancels a search initiated by BTA_DmSearch().
**
** Returns          void
**
*******************************************************************************/
void BTA_DmSearchCancel(void)
{
    struct bta_dm_hdr_t *p_msg;

    if ((p_msg = (struct bta_dm_hdr_t *)GKI_getbuf(
             sizeof(struct bta_dm_hdr_t))) != NULL) {
        p_msg->event = BTA_DM_API_SEARCH_CANCEL_EVT;
        bta_sys_sendmsg(p_msg);
    }
}

/*******************************************************************************
**
** Function         BTA_DmPinReply
**
** Description      Provides a PIN when one is requested by DM during a
**                  bonding procedure.
**
** Returns          void
**
*******************************************************************************/
void BTA_DmPinReply(unsigned char *bd_addr, unsigned char accept,
                    unsigned char pin_len, unsigned char *p_pin)
{
    struct bta_dm_pin_reply_msg_t *p_msg;

    if ((p_msg = (struct bta_dm_pin_reply_msg_t *)GKI_getbuf(
             sizeof(struct bta_dm_pin_reply_msg_t))) != NULL) {
        p_msg->hdr.event = BTA_DM_API_PIN_REPLY_EVT;
        bdcpy(p_msg->bd_addr, bd_addr);
        p_msg->accept = accept;
        if (accept) {
            p_msg->pin_len = pin_len;
            memcpy(p_msg->p_pin, p_pin, pin_len);
        }
        bta_sys_sendmsg(p_msg);
    }
}

/*******************************************************************************
**
** Function         BTA_DmAddDevice
**
** Description      Adds a device to the security database list of peer
**                  devices.  Direct execution function that may lock task
**                  scheduling.
**
** Returns          BTA_SUCCESS if successful, BTA_FAILURE if operation failed.
**
*******************************************************************************/
tBTA_STATUS BTA_DmAddDevice(unsigned char *bd_addr, unsigned char *link_key,
                            unsigned int trusted_mask, unsigned char is_trusted)
{
    unsigned int trusted_mask_array[2];
    unsigned int srv_id;
    unsigned char i = 0;
    BOOLEAN status;

    memset(trusted_mask_array, 0, sizeof(trusted_mask_array));

    /* Convert the BTA service-id bitmask into the BTM security trusted-mask
       array.  Each BTM service id maps to word (id >> 5) and bit (id & 0x1f);
       the bit is expressed the way the retail source did: id - (id & 0x1FE0),
       which MWCC lowers to rlwinm + subf (equivalent to id & 0x1f). */
    if (is_trusted) {
        for (; trusted_mask && (i < 23); i++) {
            if (trusted_mask & (1 << i)) {
                trusted_mask &= ~(1 << i);
                srv_id = bta_service_id_to_btm_srv_id_lkup_tbl[i];
                trusted_mask_array[(srv_id >> 5) & 0xFF] |=
                    (1 << (srv_id - (srv_id & 0x1FE0)));
            }
        }
    }

    GKI_sched_lock();
    status = BTM_SecAddDevice(bd_addr, NULL, NULL, NULL, trusted_mask_array,
                              link_key);
    GKI_sched_unlock();

    return !status;
}

/*******************************************************************************
**
** Function         BTA_DmRemoveDevice
**
** Description      Removes a device from the security database.  Direct
**                  execution function that may lock task scheduling.
**
** Returns          BTA_SUCCESS if successful, BTA_FAILURE if operation failed.
**
*******************************************************************************/
tBTA_STATUS BTA_DmRemoveDevice(unsigned char *bd_addr)
{
    BOOLEAN status;

    GKI_sched_lock();
    status = BTM_SecDeleteDevice(bd_addr);
    GKI_sched_unlock();

    return !status;
}

/*******************************************************************************
**
** Function         BTA_DmSendHciReset
**
** Description      Sends an HCI_RESET command to the Bluetooth controller.
**
** Returns          void
**
*******************************************************************************/
void BTA_DmSendHciReset(void)
{
    struct bta_dm_hdr_t *p_msg;

    if ((p_msg = (struct bta_dm_hdr_t *)GKI_getbuf(
             sizeof(struct bta_dm_hdr_t))) != NULL) {
        p_msg->event = BTA_DM_API_RESET_HCI_EVT;
        bta_sys_sendmsg(p_msg);
    }
}
