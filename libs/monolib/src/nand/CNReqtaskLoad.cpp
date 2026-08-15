// Decompiled: CNReqtaskLoad
//
// Translation-unit final form.
//
// The CNReqtaskLoad sub-task block lives inside a CNRequest (at offset 4).
// MWCC mangles the names of free functions and data globals; the retail
// symbols here are stripped placeholder names (func_8...) and the data items
// are referenced by their absolute linker labels (lbl_8...). To keep the
// emitted object symbols/labels byte-identical to retail we must keep C
// linkage for exactly those names (exempt in PLAN.md 17.6). Everything that
// is meaningful C++ (the task data layout) is expressed as a real struct.
#include <types.h>
#include <string.h>
#include "revolution/os/OSCache.h"  // DCFlushRange (C linkage)

// Vtable of the CNReqtaskLoad task. Its layout is unknown (we only ever
// install it / take its address), so it is used as an opaque pointer type.
struct CNReqtaskLoadVtbl;
struct CNReqtaskLoadData;  // defined below; forward decl for the extern block

// Retail linker names referenced by this unit (C linkage so the emitted
// symbols match the stripped retail names rather than C++ manglings).
extern "C" {
    extern CNReqtaskLoadVtbl* lbl_eu_806659E8; // installed vtable pointer for this task
    extern char lbl_eu_8056FD88[];      // vtable data - array type prevents sda21
    extern u8  lbl_eu_806659D0;         // global NAND "busy" flag
    extern s32 lbl_eu_806659D4;         // global NAND result/error latch

    // NAND subsystem primitives (defined in the CNReqtaskSave unit). These are
    // stripped retail placeholder names, so they need C linkage to emit the same
    // unmangled reloc. `strcpy` comes from <string.h> and `DCFlushRange` from
    // <revolution/os/OSCache.h> (both already included above).
    s32 func_804DA9C4(CNReqtaskLoadData* data, u8 arg);  // NAND open primitive
    s32 func_804DA540(u8* ptr, u32 arg);  // NAND set-buffer primitive
    s32 func_804DA5B4(u32 arg1, u32 arg2);  // NAND read primitive
    s32 func_804DA69C(void);                // NAND close primitive
}

// CNReqtaskLoad task parameter block (the sub-task embedded in CNRequest):
//   +0x00: char path[0x10] -- NAND load path (written by strcpy)
//   +0x10: u32 mBuffer     -- destination buffer address (NAND read / DCFlushRange dest)
//   +0x14: u32 mSize       -- buffer size (NAND read / DCFlushRange size)
//   +0x18: u8  mFlag       -- request flag forwarded to the NAND open call
//   +0x19: u8  mState      -- async state machine step (0..4)
struct CNReqtaskLoadData {
    char path[0x10]; // +0x00
    u32 mBuffer;     // +0x10
    u32 mSize;       // +0x14
    u8  mFlag;       // +0x18
    u8  mState;      // +0x19
};

// us-804df22c: func_804DAF70
// Configures the CNReqtaskLoad sub-task: records the path/buffer/size/flag and
// resets the async state to step 0, then returns the task vtable pointer.
//
// Note: the first parameter is an opaque byte handle (`u8*`), matching how the
// NAND open primitive hands the caller back an unwrapped task buffer. The typed
// local `d` gives the rest of the body clean struct access.
extern "C" CNReqtaskLoadVtbl** func_804DAF70(u8* data, const char* path, u32 arg2, u32 arg3, u8 arg4) {
    CNReqtaskLoadData* d = (CNReqtaskLoadData*)data;
    strcpy(d->path, path);
    d->mBuffer = arg2;
    d->mSize = arg3;
    d->mFlag = arg4;
    d->mState = 0;
    return &lbl_eu_806659E8;
}

// us-804df27c: func_804DAFB8
// Async NAND load state machine, polled by the CNand completion pump.
// Advancing one step per call; returns 1 when fully loaded, 2 on error,
// 0 while still in progress. Steps:
//   0 -> open the file (func_804DA9C4 + func_804DA540)
//   1 -> read into mBuffer (func_804DA5B4)
//   2 -> close the file (func_804DA69C)
//   3 -> flush dcache on the read buffer (DCFlushRange)
//   4 -> done (return 1)
extern "C" s32 func_804DAFB8(CNReqtaskLoadVtbl* vtable_ptr, CNReqtaskLoadData* data) {
    CNReqtaskLoadData* d = data;

    if (lbl_eu_806659D0 != 0) { // NAND subsystem busy
        return 0;
    }

    // If the load has begun and the global result latch has gone negative,
    // the pending operation failed.
    if ((s8)d->mState > 0) {
        if (lbl_eu_806659D4 < 0) {
            return 2;
        }
    }

    switch ((s8)d->mState) {
        case 0: {
            s32 ret = func_804DA9C4(d, d->mFlag);
            s32 r = func_804DA540((u8*)(u32)ret, 1);
            if (r != 0) {
                return 2;
            }
            d->mState = 1;
            goto ret0;
        }
        case 1: {
            s32 r = func_804DA5B4(d->mBuffer, d->mSize);
            if (r != 0) {
                return 2;
            }
            d->mState = 2;
            goto ret0;
        }
        case 2: {
            s32 r = func_804DA69C();
            if (r != 0) {
                return 2;
            }
            d->mState = 3;
            goto ret0;
        }
        case 3: {
            DCFlushRange((u8*)d->mBuffer, d->mSize);
            d->mState = 4;
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
// .ctors static initializer: installs the CNReqtaskLoad vtable
// (lbl_eu_8056FD88) into the task vtable pointer (lbl_eu_806659E8).
//
// Retail keeps the thunk `li r3,&lbl_eu_806659E8@sda21; b func_804DB0E0`
// as a tail call into the adjacent helper (the annotation originally merged
// the two bodies into one 0x18 symbol). The helper stores the vtable
// address through r3. `char[]` type for the vtable keeps the address
// constant in a lis/addi pair (no sda21 dereference).
extern "C" __declspec(noinline) void func_804DB0E0(void* dest) {
    *(void**)dest = (void*)lbl_eu_8056FD88;
}
extern "C" __declspec(noinline) void sinit_804DB0D8() {
    func_804DB0E0(&lbl_eu_806659E8);
}