// High-level C reconstruction of bta_dm_cfg.c (data-only TU).
// BTA device manager compile-time configuration tables.
// Values and layouts recovered from the retail object
// build/us/obj/RVL_SDK/src/revolution/bte/bta/dm/bta_dm_cfg.o

#include "revolution/bte/bta/include/bta_api.h"
#include "revolution/bte/bta/sys/bta_sys.h"

/* ---------------- local layouts matching the retail build ---------------- */

typedef struct
{
    UINT8   dev_class[3];   /* COD */
    UINT8   pad;            /* padding in retail build */
    UINT16  policy_settings;
    UINT16  page_timeout;   /* 625us units */
    UINT16  link_timeout;   /* 625us units */
    UINT8   avoid_scatter;  /* FALSE = do not avoid scatternet while streaming */
    UINT8   pad2;
} tBTA_DM_CFG_RETAIL;

typedef struct
{
    UINT8   id;             /* BTA service id */
    UINT8   app_id;
    UINT8   cfg;
} tBTA_DM_RM_RETAIL;

typedef struct
{
    UINT8   id;
    UINT8   app_id;
    UINT8   mask;
} tBTA_DM_COMPRESS_RETAIL;

typedef struct
{
    UINT8   id;
    UINT8   app_id;
    UINT8   spec_idx;
    UINT8   pad[5];        /* retail symbol is 8 bytes (align 8) in .sbss2 */
} tBTA_DM_PM_CFG_RETAIL;

typedef struct
{
    UINT8   power_mode;
    UINT8   pad;
    UINT16  timeout;
} tBTA_DM_PM_ACTN_RETAIL;

#define BTA_DM_PM_NUM_EVTS_RETAIL 9

typedef struct
{
    UINT8                   allow_mask;
    UINT8                   pad;
    tBTA_DM_PM_ACTN_RETAIL  actn_tbl[BTA_DM_PM_NUM_EVTS_RETAIL][2];
} tBTA_DM_PM_SPEC_RETAIL;

typedef struct
{
    UINT16  max;
    UINT16  min;
    UINT16  attempt;
    UINT16  timeout;
    UINT8   mode;
    UINT8   pad;
    UINT16  reserved;       /* retail build has 4 trailing bytes per entry */
} tBTM_PM_PWR_MD_RETAIL;

/* ---------------- data ---------------- */

const tBTA_DM_CFG_RETAIL bta_dm_cfg =
{
    {0x40, 0x02, 0x04}, /* COD */
    0,
    0x000F,             /* link policy: master/slave switch | hold | sniff | park */
    0x2000,             /* page timeout: 8192 * 625us */
    0x1388,             /* link timeout: 5000 * 625us */
    0,                  /* avoid_scatter = FALSE */
    0
};

const tBTA_DM_RM_RETAIL bta_dm_rm_cfg[] =
{
    {0x00, 0x02, 0x01},
    {0x0E, 0x01, 0x02},
    {0x0E, 0x02, 0x02},
    {0x12, 0xFF, 0x01},
};

const tBTA_DM_COMPRESS_RETAIL bta_dm_compress_cfg[] =
{
    {0x00, 0x05, 0x00},
    {0x0A, 0xFF, 0x01},
    {0x08, 0xFF, 0x01},
    {0x06, 0xFF, 0x01},
    {0x07, 0xFF, 0x01},
    {0x12, 0xFF, 0x02},
};

const tBTA_DM_PM_CFG_RETAIL bta_dm_pm_cfg =
{
    0, 0, 0, {0}
};

const tBTA_DM_PM_SPEC_RETAIL bta_dm_pm_spec =
{
    0x03,   /* allow_mask */
    0,
    {
        {{0x02, 0, 5000}, {0x00, 0, 0}},
        {{0x10, 0, 0},    {0x00, 0, 0}},
        {{0x00, 0, 0},    {0x00, 0, 0}},
        {{0x00, 0, 0},    {0x00, 0, 0}},
        {{0x00, 0, 0},    {0x00, 0, 0}},
        {{0x02, 0, 5000}, {0x00, 0, 0}},
        {{0x00, 0, 0},    {0x00, 0, 0}},
        {{0x00, 0, 0},    {0x00, 0, 0}},
        {{0x08, 0, 5000}, {0x00, 0, 0}},
    },
};

const tBTM_PM_PWR_MD_RETAIL bta_dm_pm_md[] =
{
    {400, 200, 4,   4, 0x02, 0, 0x0320},
    {400,   0, 0, 0x0300, 0x00, 0, 0x0000},
};

tBTA_DM_RM_RETAIL *p_bta_dm_rm_cfg = (tBTA_DM_RM_RETAIL *)bta_dm_rm_cfg;
tBTA_DM_COMPRESS_RETAIL *p_bta_dm_compress_cfg = (tBTA_DM_COMPRESS_RETAIL *)bta_dm_compress_cfg;
tBTA_DM_PM_CFG_RETAIL *p_bta_dm_pm_cfg = (tBTA_DM_PM_CFG_RETAIL *)&bta_dm_pm_cfg;
tBTA_DM_PM_SPEC_RETAIL *p_bta_dm_pm_spec = (tBTA_DM_PM_SPEC_RETAIL *)&bta_dm_pm_spec;
tBTM_PM_PWR_MD_RETAIL *p_bta_dm_pm_md = (tBTM_PM_PWR_MD_RETAIL *)&bta_dm_pm_md[0];
