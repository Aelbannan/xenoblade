// ICamControlGc - bitfield property getters
// Each calls a virtual function (vtable+0x8) and extracts one flag bit.

#include "kyoshin/harness_catalog.hpp"

struct ICamControlGcIf { virtual void* getObject(); };

extern "C" int func_80274DD8(ICamControlGcIf* self) {
    void* obj = self->getObject();
    return (*(int*)obj >> 19) & 1;
}

extern "C" int func_80274E0C(ICamControlGcIf* self) {
    void* obj = self->getObject();
    return (*(int*)obj >> 20) & 1;
}

extern "C" int func_80274E40(ICamControlGcIf* self) {
    void* obj = self->getObject();
    return (*(int*)obj >> 17) & 1;
}

extern "C" int func_80274E74(ICamControlGcIf* self) {
    void* obj = self->getObject();
    return (*(int*)obj >> 18) & 1;
}
