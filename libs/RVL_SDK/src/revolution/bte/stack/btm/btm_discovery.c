// Decompiled: btm_discovery_db_init and btm_discovery_db_reset.
//
// These functions manage the BTM service-discovery database. The discovery
// state lives at fixed offsets inside the global tBTM_CB (btm_cb). We model
// that slice as a local struct so the code can reference the fields by name
// instead of raw pointer arithmetic.

#include <harness_catalog.h>
#include <string.h>

#include "revolution/BTE/include/bt_target.h"
#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/stack/include/sdp_api.h"
#include "revolution/BTE/stack/btm/btm_int.h"
#include "revolution/BTE/stack/include/btu.h"

/* Discovery slice of tBTM_CB. The retail layout places these fields at fixed
   offsets within the global btm_cb. The leading pad aligns discovery_timer
   to offset 0x658. */
typedef struct
{
    u8              pad[0x658];
    TIMER_LIST_ENT  discovery_timer;        /* 0x658: discovery timeout timer  */
    u16             field_0x670;            /* 0x670: discovery state flag     */
    u8              field_0x672[2];         /* 0x672: padding                  */
    tSDP_UUID       field_0x674;            /* 0x674: UUID filter (0x1002 PnP) */
    u8              field_0x688[0x48];      /* 0x688: padding                  */
    void            (*field_0x6d0)(UINT16);  /* 0x6d0: discovery complete cb    */
    u8              field_0x6d4[0xfa0];     /* 0x6d4: discovery DB buffer      */
    u8              field_0x1674;           /* 0x1674: discovery active flag   */
    u8              field_0x1675[3];        /* 0x1675: padding                 */
    tSDP_DISCOVERY_DB *field_0x1678;        /* 0x1678: pointer to DB buffer    */
} tBTM_DISCOVERY_CB;

/* Initializes the discovery database: clears the discovery slice, sets up a
   single UUID filter (0x1002 = PnP Information), and calls SDP_InitDiscoveryDb. */
void btm_discovery_db_init(void)
{
    tBTM_DISCOVERY_CB *p = (tBTM_DISCOVERY_CB *)&btm_cb;

    p->field_0x1678 = (tSDP_DISCOVERY_DB *)p->field_0x6d4;
    memset(&p->discovery_timer, 0, 0x1020);
    p->field_0x670 = 1;
    p->field_0x674.len = LEN_UUID_16;
    p->field_0x674.uu.uuid16 = 0x1002;
    SDP_InitDiscoveryDb(p->field_0x1678, BTM_DISCOVERY_DB_SIZE, 1,
                       &p->field_0x674, 0, NULL);
}

/* Resets the discovery database: stops the discovery timer, notifies the
   completion callback if a discovery was active, then re-initializes the DB. */
void btm_discovery_db_reset(void)
{
    tBTM_DISCOVERY_CB *p = (tBTM_DISCOVERY_CB *)&btm_cb;

    btu_stop_timer(&p->discovery_timer);
    if (p->field_0x1674) {
        void (*callback)(UINT16) = p->field_0x6d0;
        p->field_0x1674 = 0;
        p->field_0x6d0 = NULL;
        if (callback) {
            callback(0);
        }
    }
    p->field_0x1678 = (tSDP_DISCOVERY_DB *)p->field_0x6d4;
    memset(&p->discovery_timer, 0, 0x1020);
    p->field_0x670 = 1;
    p->field_0x674.len = LEN_UUID_16;
    p->field_0x674.uu.uuid16 = 0x1002;
    SDP_InitDiscoveryDb(p->field_0x1678, BTM_DISCOVERY_DB_SIZE, 1,
                       &p->field_0x674, 0, NULL);
}

void btm_discovery_timeout(void) {}
