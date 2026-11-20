#pragma once

#include <types.h>

struct UnkSceneDataObj;

class UnkClass_8047BB54 {
public:
    static UnkSceneDataObj* spInstance; // retail .sbss symbol spInstance__17UnkClass_8047BB54
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
extern "C" void markDirty__Q26mpfsys17UnkClass_80471EC8Fv(void* self);

