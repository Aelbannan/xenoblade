#pragma once

#include <types.h>

class UnkClass_8047BB54 {
public:
    void initMpfSystem();
    void checkMpfFlags();
    void resetMpfInstance();
    void initMpfDrawBuffer();
    void* getMpfStatus();
    void mpfCullMarkLayers();
    void mpfDrawLayerOpa();
    void mpfDrawLayerXlu();

    // TODO: add fields
    void setMpfFloatParam();
    void forwardMpfCallA();
    void forwardMpfCallB();
    void forwardMpfCallC();
    void flushMpfBuffer();
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// mpfsys::UnkClass_80471EC8 methods (defined in monolib/src/mpfsys)
extern "C" void setColorMode__Q26mpfsys17UnkClass_80471EC8Fv(void* self);
extern "C" void clearFlag__Q26mpfsys17UnkClass_80471EC8Fv(void* self);
extern "C" void setFlag__Q26mpfsys17UnkClass_80471EC8Fv(void* self);
extern "C" void markDirty__Q26mpfsys17UnkClass_80471EC8Fv(void* self);

