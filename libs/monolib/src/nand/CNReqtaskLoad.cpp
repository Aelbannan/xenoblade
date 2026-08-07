// Decompiled: CNReqtaskLoad
#include <types.h>
#include <string.h>

// Global symbols (retail linker names; C linkage so the emitted object symbols
// match the stripped retail names rather than C++ manglings).
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

// CNReqtaskLoad data layout (the task parameter block embedded in a CNRequest):
//   +0x00: char path[]  -- NAND load path (written by strcpy)
//   +0x10: u32 mBuffer  -- destination buffer address (fn1/Buffer arg1, DCFlushRange arg1)
//   +0x14: u32 mSize    -- buffer size (fn1 arg2, DCFlushRange arg2)
//   +0x18: u8  mFlag    -- request flag forwarded to the NAND open call
//   +0x19: u8  mState   -- async state machine step 0..4

struct CNReqtaskLoadData {
    char path[0x10];  // +0x00
    u32 mBuffer;      // +0x10
    u32 mSize;        // +0x14
    u8  mFlag;        // +0x18
    u8  mState;       // +0x19
};

// us-804df22c: func_804DAF70
// Configures the CNReqtaskLoad sub-task: records the path/buffer/size/flag and
// resets the async state to 0, then returns the task vtable pointer.
void** func_804DAF70(void* data, const char* path, u32 arg2, u32 arg3, u8 arg4) {
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
// .ctors static initializer: installs the CNReqtaskLoad vtable
// (lbl_eu_8056FD88) into the vtable pointer (lbl_eu_806659E8).
//
// KNOWN CEILING (MWCC_REFERENCE "b .+4 sinit barrier"): retail emits a 24-byte
// `li r3,dest@sda21; b .+4 (scheduler barrier); lis/addi src; stw r4,0(r3); blr`
// shape that is not reproducible from high-level C: MWCC always folds the store
// to `stw rX,dest@sda21(r0)` and never emits the `b .+4`. 17+ source forms and
// 6 MWCC versions were ruled out; the readable 20-byte folded-store form below is
// the documented endpoint for all five monolib NAND sinits. Returning p keeps
// &lbl_eu_806659E8 live in r3 (closest match). No assembly is added per policy.
void** sinit_804DB0D8() {
    void** p = &lbl_eu_806659E8;
    void* v = (void*)lbl_eu_8056FD88;
    *p = v;
    return p;
}