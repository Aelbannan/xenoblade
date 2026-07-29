// Auto-scaffolded catalog TU for monolib/src/scn/CMdlMaterial
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CMdlMaterial.hpp"

// CMdlMaterial::~CMdlMaterial() — D0 deleting destructor.
// Body is empty; MWCC generates the conditional operator delete via the r4 flag.
CMdlMaterial::~CMdlMaterial() {}

void func_804E54B8(){}

void func_804E5990(){}

// Sub-object destructor for an unnamed internal class.
// Manages a dynamically-allocated buffer at offset 0x08 with an ownership
// flag at offset 0x10: when flag==0 the buffer is owned (delete[]'d here),
// when flag!=0 the buffer is merely released without freeing.
struct MdlSub {
    u8 pad_0x00[8];     // vtable + padding
    void* buf;           // 0x08
    u8 pad_0x0C[4];
    u8 flag;             // 0x10 — buffer ownership flag
};

extern "C" void __dt__804E5DE0(MdlSub* arg) {
    if (!arg->flag) {
        if (arg->buf) {
            delete[] static_cast<char*>(arg->buf);
            arg->buf = nullptr;
        }
    } else {
        arg->buf = nullptr;
    }
}

void func_804E5E38(){}

void func_804E5FD4(){}

void func_804E6158(){}

void func_804E6358(){}

void func_804E64B0(){}
