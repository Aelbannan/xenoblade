// Auto-scaffolded catalog TU for monolib/src/core/monolib_eu_804F9E98
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <string.h>

// Global symbols referenced by this TU
void* lbl_eu_80665A98;   // sbss:0x448 - pointer initialized by sinit
void* lbl_eu_80570410;   // data:0x44A8
u8 lbl_eu_806659D0;      // sbss:0x380
s32 lbl_eu_806659D4;     // sbss:0x384

// External function declarations
extern "C" s32 func_804DA9C4(void* r3, u8 r4);
extern "C" s32 func_eu_804DEB4C(s32 r3, u8 r4, u32 r5);

// us-804f9e98: func_eu_804F9E98
// Copies a string into dest, sets bytes at offsets 0xd/0xe/0xf, returns &lbl_eu_80665A98
void** func_eu_804F9E98(void* dest, const void* src, u8 r5, u8 r6) {
    strcpy((char*)dest, (const char*)src);
    ((u8*)dest)[0xd] = r5;
    ((u8*)dest)[0xe] = r6;
    ((u8*)dest)[0xf] = 0;
    return &lbl_eu_80665A98;
}

// us-804f9ee0: func_eu_804F9EE0
// State machine: reads state from r4[0xf] and dispatches
int func_eu_804F9EE0(void* r3, void* r4) {
    if (lbl_eu_806659D0 != 0) return 0;
    u8 state = ((u8*)r4)[0xf];
    switch (state) {
        case 0: {
            s32 result = func_804DA9C4(r4, ((u8*)r4)[0xe]);
            if (func_eu_804DEB4C(result, ((u8*)r4)[0xd], 0) != 0) return 2;
            ((u8*)r4)[0xf] = 1;
            return 0;
        }
        case 1: {
            s32 val = lbl_eu_806659D4;
            if (val != 0 && val != -6) return 2;
            ((u8*)r4)[0xf] = 2;
            return 0;
        }
        case 2:
            return 1;
        default:
            return 0;
    }
}

// us-804f9fa4: sinit_eu_804F9FA4
// Static initializer: sets lbl_eu_80665A98 = &lbl_eu_80570410
void sinit_eu_804F9FA4() {
    lbl_eu_80665A98 = &lbl_eu_80570410;
}
