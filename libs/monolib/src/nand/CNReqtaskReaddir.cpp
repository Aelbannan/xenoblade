// Decompiled: CNReqtaskReaddir
#include <types.h>

// Global symbols
extern "C" {
    void* lbl_eu_806659F0;
    extern char lbl_eu_8056FDA8[];  // vtable data - array type prevents sda21
    u8 lbl_eu_806659D0;
    s32 lbl_eu_806659D4;

    u32* func_804DA98C(u8 arg);
    s32 func_804DA898(u32* entries, u32* dir, u32* result);
}

// CNReqtaskReaddir data layout:
//   +0x00: u32* field_0 (entries/entry buffer)
//   +0x04: u32  field_4 (max count)
//   +0x08: u32* field_8 (directory handle pointer)
//   +0x0C: u8   field_0C (entry type filter)
//   +0x0D: u8   state

// us-804df3b8: func_804DB0F0
// Initializes readdir task data and returns vtable pointer
void** func_804DB0F0(void* data, u32* entries, u32 count, u32* dir, u8 arg) {
    ((u32*)data)[0] = (u32)entries;
    ((u32*)data)[1] = count;
    ((u32*)data)[2] = (u32)dir;
    ((u8*)data)[0x0C] = arg;
    ((u8*)data)[0x0D] = 0;
    *(u8*)entries = 0;
    *dir = 0;
    return &lbl_eu_806659F0;
}

// us-804df3e0: func_804DB114
// State machine for NAND readdir task
s32 func_804DB114(void* vtable_ptr, void* data) {
    if (lbl_eu_806659D0 != 0) {
        return 0;
    }

    s32 state = (s8)((u8*)data)[0x0D];
    if (state > 0) {
        if (state != 1) {
            if (lbl_eu_806659D4 < 0) return 2;
        } else {
            s32 err = lbl_eu_806659D4;
            if (err == -12) {
                ((u8*)data)[0x0D] = 3;
            } else if (err < 0) {
                return 2;
            } else {
                u32* dir = ((u32**)data)[2];
                u32 cnt = ((u32*)data)[1];
                if (cnt < ((u32*)dir)[0] * 13) return 2;
            }
        }
    }

    switch ((s8)((u8*)data)[0x0D]) {
        case 0: {
            u32* r = func_804DA98C(((u8*)data)[0x0C]);
            s32 res = func_804DA898(0, ((u32**)data)[2], (u32*)(u32)r);
            if (res != 0) {
                return 2;
            }
            ((u8*)data)[0x0D] = 1;
            goto ret0;
        }
        case 1: {
            u32* r = func_804DA98C(((u8*)data)[0x0C]);
            s32 res = func_804DA898(((u32**)data)[0], ((u32**)data)[2], (u32*)(u32)r);
            if (res != 0) {
                return 2;
            }
            ((u8*)data)[0x0D] = 2;
            goto ret0;
        }
        case 2:
            ((u8*)data)[0x0D] = 3;
            goto ret0;
        case 3:
            return 1;
        default:
            break;
    }
ret0:
    return 0;
}

// us-804df534: sinit_804DB228
// Static initializer: sets vtable pointer for CNReqtaskReaddir.
// Returning p keeps &lbl_eu_806659F0 live in r3 (best-effort 20-byte form).
void** sinit_804DB228() {
    void** p = &lbl_eu_806659F0;
    void* v = (void*)lbl_eu_8056FDA8;
    *p = v;
    return p;
}
