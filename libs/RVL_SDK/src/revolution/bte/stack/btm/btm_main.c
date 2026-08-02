// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/btm/btm_main
// Replace stubs with high-level C/C++ during decomp.

#include <string.h>

/* btm_cb: retail .bss at 0x805BC2F8 (0x27C4 bytes) - owned by this TU's
   .bss split; referenced as `extern BtmCb btm_cb` from the other btm TUs. */
unsigned char btm_cb[0x27C4];

void btm_init()
{
    memset(btm_cb, 0, 0x27C4);
    btm_cb[0x27C0] = 0;
    btm_inq_db_init();
    btm_acl_init();
    btm_sec_init(2);
    btm_sco_init();
    btm_dev_init();
}
