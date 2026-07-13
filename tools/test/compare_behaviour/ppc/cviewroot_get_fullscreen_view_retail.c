#include "cviewroot_gfsv_mock.h"

/*
 * Retail semantic slice for getFullScreenView (full CViewRoot.o needs
 * CDesktop/getFrame2ViewOffset/CDeviceVI and crashes mwldeppc).
 * Matches host oracle in host/cviewroot_get_fullscreen_view.cpp.
 */
void* rb_getFullScreenView__9CViewRootFv(void) {
    return cviewroot_gfsv_select(0);
}
