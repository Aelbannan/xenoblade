// High-level C reconstruction of bta_sys_conn.c
// BTA system connection state notification

#include "revolution/bte/bta/sys/bta_sys.h"

/* bta_sys_cb is a BSS global (0x8C bytes) defined in another TU.
 * Layout at the offsets used by this file:
 *   0x80: rm_register callback   (tBTA_SYS_CONN_CBACK*)
 *   0x84: pm_register callback   (tBTA_SYS_CONN_CBACK*)
 *   0x88: compress_register callback (tBTA_SYS_CONN_CBACK*)
 */
typedef struct {
    UINT8 _pad[0x80];
    tBTA_SYS_CONN_CBACK *rm_cback;
    tBTA_SYS_CONN_CBACK *pm_cback;
    tBTA_SYS_CONN_CBACK *compress_cback;
} tBTA_SYS_CONN_CB;

extern tBTA_SYS_CONN_CB bta_sys_cb;

void bta_sys_rm_register(tBTA_SYS_CONN_CBACK *p_cback)
{
    bta_sys_cb.rm_cback = p_cback;
}

void bta_sys_compress_register(tBTA_SYS_CONN_CBACK *p_cback)
{
    bta_sys_cb.compress_cback = p_cback;
}

void bta_sys_pm_register(tBTA_SYS_CONN_CBACK *p_cback)
{
    bta_sys_cb.pm_cback = p_cback;
}

void bta_sys_conn_open(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
    if (bta_sys_cb.rm_cback) {
        bta_sys_cb.rm_cback(BTA_SYS_CONN_OPEN, id, app_id, peer_addr);
    }

    if (bta_sys_cb.pm_cback) {
        bta_sys_cb.pm_cback(BTA_SYS_CONN_OPEN, id, app_id, peer_addr);
    }

    if (bta_sys_cb.compress_cback) {
        bta_sys_cb.compress_cback(BTA_SYS_CONN_OPEN, id, app_id, peer_addr);
    }
}

void bta_sys_conn_close(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
    if (bta_sys_cb.rm_cback) {
        bta_sys_cb.rm_cback(BTA_SYS_CONN_CLOSE, id, app_id, peer_addr);
    }

    if (bta_sys_cb.pm_cback) {
        bta_sys_cb.pm_cback(BTA_SYS_CONN_CLOSE, id, app_id, peer_addr);
    }

    if (bta_sys_cb.compress_cback) {
        bta_sys_cb.compress_cback(BTA_SYS_CONN_CLOSE, id, app_id, peer_addr);
    }
}

void bta_sys_sco_close(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
    if (bta_sys_cb.pm_cback) {
        bta_sys_cb.pm_cback(BTA_SYS_SCO_CLOSE, id, app_id, peer_addr);
    }
}

void bta_sys_idle(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
    if (bta_sys_cb.pm_cback) {
        bta_sys_cb.pm_cback(BTA_SYS_CONN_IDLE, id, app_id, peer_addr);
    }
}

void bta_sys_busy(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
    if (bta_sys_cb.pm_cback) {
        bta_sys_cb.pm_cback(BTA_SYS_CONN_BUSY, id, app_id, peer_addr);
    }
}
