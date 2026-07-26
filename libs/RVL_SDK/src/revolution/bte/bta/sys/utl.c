// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/sys/utl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void utl_freebuf(void** p)
{
    if (*p != NULL) {
        GKI_freebuf(*p);
        *p = NULL;
    }
}
