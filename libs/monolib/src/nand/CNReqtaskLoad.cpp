// Decompiled: CNReqtaskLoad
#include <types.h>
#include <string.h>

// Global symbols
extern "C" {
    void* lbl_eu_806659E8;
    extern char lbl_eu_8056FD88[];  // vtable data - array type prevents sda21
    u8 lbl_eu_806659D0;
    s32 lbl_eu_806659D4;

    char* strcpy(char* dest, const char* src);
    s32 func_804DA9C4(void* data, u8 arg);
    s32 func_804DA540(void* ptr, u32 arg);
    s32 func_804DA5B4(u32 arg1, u32 arg2);
    s32 func_804DA69C(void);
    void DCFlushRange(void* addr, u32 size);
}

// CNReqtaskLoad data layout:
//   +0x00: char path[]  (written by strcpy)
//   +0x10: u32 field_10 (size?)
//   +0x14: u32 field_14 (address?)
//   +0x18: u8  field_18
//   +0x19: u8  state

struct CNReqtaskLoadData {
    char path[0x10];  // +0x00
    u32 field_10;     // +0x10
    u32 field_14;     // +0x14
    u8 field_18;      // +0x18
    u8 state;         // +0x19
};

// us-804df22c: func_804DAF70
// Initializes load task data and returns vtable pointer
void** func_804DAF70(void* data, const char* path, u32 arg2, u32 arg3, u8 arg4) {
    CNReqtaskLoadData* d = (CNReqtaskLoadData*)data;
    strcpy(d->path, path);
    d->field_10 = arg2;
    d->field_14 = arg3;
    d->field_18 = arg4;
    d->state = 0;
    return &lbl_eu_806659E8;
}

// us-804df27c: func_804DAFB8
// State machine for NAND load task
s32 func_804DAFB8(void* vtable_ptr, void* data) {
    if (lbl_eu_806659D0 != 0) {
        return 0;
    }

    if ((s8)((u8*)data)[0x19] > 0) {
        if (lbl_eu_806659D4 < 0) {
            return 2;
        }
    }

    switch ((s8)((u8*)data)[0x19]) {
        case 0: {
            s32 ret = func_804DA9C4(data, ((u8*)data)[0x18]);
            s32 r = func_804DA540((void*)(u32)ret, 1);
            if (r != 0) {
                return 2;
            }
            ((u8*)data)[0x19] = 1;
            goto ret0;
        }
        case 1: {
            s32 r = func_804DA5B4(((u32*)data)[4], ((u32*)data)[5]);
            if (r != 0) {
                return 2;
            }
            ((u8*)data)[0x19] = 2;
            goto ret0;
        }
        case 2: {
            s32 r = func_804DA69C();
            if (r != 0) {
                return 2;
            }
            ((u8*)data)[0x19] = 3;
            goto ret0;
        }
        case 3: {
            DCFlushRange((void*)((u32*)data)[4], ((u32*)data)[5]);
            ((u8*)data)[0x19] = 4;
            goto ret0;
        }
        case 4:
            return 1;
        default:
            break;
    }
ret0:
    return 0;
}

// us-804df3a0: sinit_804DB0D8
// Static initializer: sets vtable pointer for CNReqtaskLoad.
// Returning p keeps &lbl_eu_806659E8 live in r3 (best-effort 20-byte form).
void** sinit_804DB0D8() {
    void** p = &lbl_eu_806659E8;
    void* v = (void*)lbl_eu_8056FD88;
    *p = v;
    return p;
}
