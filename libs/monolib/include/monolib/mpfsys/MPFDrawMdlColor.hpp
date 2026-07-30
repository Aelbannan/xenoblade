#pragma once

#include <types.h>

namespace mpfsys {

// Draw dispatcher object for textured models with vertex colors.
// The singleton instance body lives in monolibdata (lbl_eu_8056DBC0).
class MPFDrawMdlColor {
public:
    static MPFDrawMdlColor* getInstance();

    // Retail instance layout (initialized by monolibdata2):
    void* m_typeDescriptor; // +0x0
    u32 m_unk04;            // +0x4
    void (*m_drawAll)();    // +0x8 func_80479BA0 (dispatched with data/list args)
    void (*m_drawHalf)();   // +0xC func_80479D40 (dispatched with data/list args)
};

} // namespace mpfsys
