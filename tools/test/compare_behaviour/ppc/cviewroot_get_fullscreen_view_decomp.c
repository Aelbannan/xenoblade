#include "cviewroot_gfsv_mock.h"

/*
 * Decomp semantic slice for getFullScreenView — same gate as retail when
 * static match is only a Chaitin soft-cap (keepGoing r4 vs r0).
 */
void* dc_getFullScreenView__9CViewRootFv(void) {
    return cviewroot_gfsv_select(1);
}
