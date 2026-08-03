// High-level C reconstruction of bta_hh_cfg.c (data-only TU).
// BTA HID host compile-time configuration.
// Values recovered from the retail object
// build/us/obj/RVL_SDK/src/revolution/bte/bta/hh/bta_hh_cfg.o

#include "revolution/bte/bta/include/bta_hh_api.h"

/* Local layout matching the retail build (16-byte config). */
typedef struct
{
    UINT8               max_devt_spt;   /* max number of types of devices supported */
    UINT8               reserved[3];
    tBTA_HH_SPT_TOD    *p_devt_list;    /* supported types of device list */
    UINT16              sdp_db_size;    /* HH SDP discovery database size */
    UINT16              reserved2;
    UINT32              reserved3;
} tBTA_HH_CFG_RETAIL;

tBTA_HH_SPT_TOD p_devt_list[] =
{
    {0x20, 0x01},   /* hid, keyboard */
    {0x10, 0x02},   /* hid, mouse   */
    {0x01, 0x03},   /* hid, joystick */
    {0x31, 0x02},   /* hid, combo   */
};

const tBTA_HH_CFG_RETAIL bta_hh_cfg =
{
    4,              /* max_devt_spt */
    {0, 0, 0},
    p_devt_list,
    0x400,          /* sdp_db_size */
    0,
    0
};

/* Retail .sdata slice is 0x10: p_devt_list then an 8-byte p_bta_hh_cfg
   { &bta_hh_cfg, 0 }. Other TUs see `extern tBTA_HH_CFG *p_bta_hh_cfg` and
   load the pointer from offset 0, so the trailing pad word is unused. */
typedef struct {
    tBTA_HH_CFG *p_cfg; /* 0x00 */
    u32          _pad;  /* 0x04 */
} tBTA_HH_CFG_PTR_RETAIL;

tBTA_HH_CFG_PTR_RETAIL p_bta_hh_cfg = {(tBTA_HH_CFG *)&bta_hh_cfg, 0};
