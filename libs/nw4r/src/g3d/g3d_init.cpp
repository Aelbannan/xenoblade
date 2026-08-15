#include <nw4r/g3d.h>
#include <nw4r/ut.h>

#include <revolution/GX.h>
#include <revolution/OS.h>
#include <revolution/VI.h>

// Retail keeps the G3D version string and its .sdata pointer in the shared
// pool (pointer at lbl_eu_806634B0 -> string at 0x80569828, outside this
// split slice); reference the pointer by name instead of defining the
// version record locally (no .data/.sdata here).
extern "C" const char* lbl_eu_806634B0;

namespace nw4r {
namespace g3d {

void G3dInit(bool enableLockedCache) {
    OSRegisterVersion(lbl_eu_806634B0);

    if (enableLockedCache) {
        ut::LC::Enable();
    } else {
        ut::LC::Disable();
    }

    InitFastCast();

    GXRenderModeObj* pMode;
    switch (VIGetTvFormat()) {
    case VI_TVFORMAT_NTSC: {
        pMode = &GXNtsc480IntDf;
        break;
    }

    case VI_TVFORMAT_PAL: {
        pMode = &GXPal528IntDf;
        break;
    }

    case VI_TVFORMAT_EURGB60: {
        pMode = &GXEurgb60Hz480IntDf;
        break;
    }

    case VI_TVFORMAT_MPAL: {
        pMode = &GXMpal480IntDf;
        break;
    }

    default: {
        pMode = &GXNtsc480IntDf;
        break;
    }
    }

    G3DState::SetRenderModeObj(*pMode);
}

void G3dReset() {
    G3DState::Invalidate();
}

} // namespace g3d
} // namespace nw4r
