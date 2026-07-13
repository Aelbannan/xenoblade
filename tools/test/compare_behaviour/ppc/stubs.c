#include <revolution/gx/GXFrameBuf.h>

static GXRenderModeObj sRenderMode = {
    VI_TVMODE_NTSC_INT,
    640,
    480,
    480,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

GXRenderModeObj* rb_getRenderModeObj__9CDeviceVIFv(void) {
    return &sRenderMode;
}

GXRenderModeObj* dc_getRenderModeObj__9CDeviceVIFv(void) {
    return &sRenderMode;
}

void exit(int code) {
    (void)code;
    for (;;) {
    }
}
