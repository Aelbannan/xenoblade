// Auto-scaffolded catalog TU for monolib/src/core/monolib_eu_804F9E98
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <string.h>

// Global symbols referenced by this TU
void* lbl_eu_80665A98;           // sbss:0x448 - pointer initialized by sinit
extern char lbl_eu_80570410[];   // data:0x44A8 - label as char[] to prevent sda21 addressing
u8 lbl_eu_806659D0;              // sbss:0x380 - guard flag (u8; retail uses cmpwi)
s32 lbl_eu_806659D4;             // sbss:0x384

// External function declarations
extern "C" s32 func_804DA9C4(void* r3, u8 r4);
extern "C" s32 func_eu_804DEB4C(s32 r3, u8 r4, u32 r5);

// us-804f9e98: func_eu_804F9E98  size=0x48
// Copies src string into dest, then stores a at dest[0xd], b at dest[0xe],
// 0 at dest[0xf], and returns &lbl_eu_80665A98.
extern "C" void** func_eu_804F9E98(void* dest, const void* src, u8 a, u8 b) {
    char* d = (char*)dest;
    u8 y = b;
    u8 x = a;
    strcpy(d, (const char*)src);
    d[0xd] = x;
    d[0xe] = y;
    d[0xf] = 0;
    return &lbl_eu_80665A98;
}

// us-804f9ee0: func_eu_804F9EE0  size=0xC4
// State machine: reads signed state byte from r4[0xf] and dispatches.
// Returns 0=busy, 1=done, 2=error.
extern "C" int func_eu_804F9EE0(void* r3, void* r4) {
    if (lbl_eu_806659D0 != 0) return 0;
    
    switch (*(s8*)((u8*)r4 + 0xf)) {
        case 0: {
            s32 result = func_804DA9C4(r4, ((u8*)r4)[0xe]);
            if (func_eu_804DEB4C(result, ((u8*)r4)[0xd], 0) != 0) return 2;
            *(s8*)((u8*)r4 + 0xf) = 1;
            break;
        }
        case 1: {
            s32 val = lbl_eu_806659D4;
            if (val != 0 && val != -6) return 2;
            *(s8*)((u8*)r4 + 0xf) = 2;
            break;
        }
        case 2:
            return 1;
        default:
            break;
    }
    return 0;
}

// us-804f9fa4: sinit_eu_804F9FA4  size=0x18
// Static initializer: stores address of lbl_eu_80570410 into lbl_eu_80665A98
extern "C" void sinit_eu_804F9FA4() {
    void* volatile *p = (void* volatile*)&lbl_eu_80665A98;
    *p = lbl_eu_80570410;
}
