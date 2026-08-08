#pragma once

#include <types.h>
#include <revolution/GX.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// CDeviceVI::getRenderModeObj (framebuffer display mode). extern "C" pins the
// retail mangled name so the call reloc matches.
extern "C" GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
