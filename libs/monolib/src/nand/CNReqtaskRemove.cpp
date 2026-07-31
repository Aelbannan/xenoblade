// Decompiled: CNReqtaskRemove
#include <types.h>
#include <string.h>

// Global symbols
extern "C" {
    void* lbl_eu_806659F8;
    extern char lbl_eu_8056FDC8[];  // vtable data - array type prevents sda21
    u8 lbl_eu_806659D0;
    s32 lbl_eu_806659D4;

    char* strcpy(char* dest, const char* src);
    s32 func_804DA9C4(void* data, u8 arg);
    s32 func_804DA76C(void* ptr);
}

// CNReqtaskRemove data layout (embedded in CNRequest):
//   +0x00: char path[] (nand path, written by strcpy)
//   +0x0D: u8  state
//   +0x0E: u8  field_E (arg/flag)

// us-804df54c: func_804DB240
// Initializes remove task data and returns vtable pointer
struct CNReqtaskRemoveData {
    char path[13];
    u8 state;    // +0x0D
    u8 field_E;  // +0x0E
};

void** func_804DB240(void* data, const char* path, u8 arg) {
    CNReqtaskRemoveData* d = (CNReqtaskRemoveData*)data;
    u8 a = arg;
    strcpy(d->path, path);
    d->field_E = a;
    d->state = 0;
    return &lbl_eu_806659F8;
}

// us-804df58c: func_804DB278
// State machine for NAND remove task
s32 func_804DB278(void* vtable_ptr, void* data) {
    if (lbl_eu_806659D0 != 0) {
        return 0;
    }

    if ((s8)((u8*)data)[0x0D] > 0) {
        if (lbl_eu_806659D4 < 0) {
            return 2;
        }
    }

    switch ((s8)((u8*)data)[0x0D]) {
        case 0: {
            s32 r = func_804DA76C((void*)(u32)func_804DA9C4(data, ((u8*)data)[0x0E]));
            if (r != 0) {
                return 2;
            }
            ((u8*)data)[0x0D] = 1;
            goto ret0;
        }
        case 1:
            ((u8*)data)[0x0D] = 2;
            goto ret0;
        case 2:
            return 1;
        default:
            break;
    }
ret0:
    return 0;
}

// us-804df648: sinit_804DB330
// Static initializer: sets vtable pointer for CNReqtaskRemove.
// Returning p keeps &lbl_eu_806659F8 live in r3 (best-effort 20-byte form).
void** sinit_804DB330() {
    void** p = &lbl_eu_806659F8;
    void* v = (void*)lbl_eu_8056FDC8;
    *p = v;
    return p;
}
