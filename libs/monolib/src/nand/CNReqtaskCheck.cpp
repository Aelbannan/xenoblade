// Decompiled: CNReqtaskCheck
#include <types.h>
#include <string.h>

// Global symbols
extern "C" {
    void* lbl_eu_80665A00;
    extern void* lbl_eu_8056FDE8;
    u8 lbl_eu_806659D0;
    s32 lbl_eu_806659D4;

    s32 func_804DA4E0(u32 arg1, u32 arg2, u32 arg3);
}

// CNReqtaskCheck data layout (at offset 4 of CNRequest):
//   +0x00: u32 field_0
//   +0x04: u32 field_4
//   +0x08: u32 field_8
//   +0x0C: u8  state

// us-804df660: func_804DB348
// Initializes check task data and returns vtable pointer
void** func_804DB348(void* data, u32 arg1, u32 arg2, u32 arg3) {
    ((u32*)data)[0] = arg1;
    ((u32*)data)[1] = arg2;
    ((u32*)data)[2] = arg3;
    ((u8*)data)[0xC] = 0;
    return &lbl_eu_80665A00;
}

// us-804df67c: func_804DB364
// State machine for NAND check task
s32 func_804DB364(void* vtable_ptr, void* data) {
    if (lbl_eu_806659D0 != 0) {
        return 0;
    }

    if ((s8)((u8*)data)[0xC] > 0) {
        if (lbl_eu_806659D4 < 0) {
            return 2;
        }
    }

    switch ((s8)((u8*)data)[0xC]) {
        case 0: {
            s32 r = func_804DA4E0(
                ((u32*)data)[0],
                ((u32*)data)[1],
                ((u32*)data)[2]);
            if (r != 0) {
                return 2;
            }
            ((u8*)data)[0xC] = 1;
            goto ret0;
        }
        case 1:
            ((u8*)data)[0xC] = 2;
            goto ret0;
        case 2:
            return 1;
        default:
            break;
    }
ret0:
    return 0;
}

// us-804df738: sinit_804DB420
// Static initializer: sets vtable pointer for CNReqtaskCheck
void sinit_804DB420() {
    void* val = (void*)&lbl_eu_8056FDE8;
    void** dest = &lbl_eu_80665A00;
    *dest = val;
}
