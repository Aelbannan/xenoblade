// Decompiled: CNReqtaskReaddir
//
// Translation-unit final form.
//
// CNReqtaskReaddir is the async NAND readdir sub-task embedded in a CNRequest.
// The retail symbols for this unit are stripped (func_8... placeholders) and the
// data globals are referenced by their absolute linker labels (lbl_8...), so C
// linkage must be kept for exactly those names to emit byte-identical symbols
// (exempt in PLAN.md 17.6). The task payload is expressed as a real struct.
#include <types.h>

// Opaque vtable of the readdir task (we only ever install it / take its address).
struct CNReqtaskReaddirVtbl;
struct CNReqtaskReaddirData;  // defined below; forward decl for the extern block

// Retail linker names referenced by this unit (C linkage so the emitted
// symbols match the stripped retail names rather than C++ manglings).
extern "C" {
    extern CNReqtaskReaddirVtbl* lbl_eu_806659F0; // installed task vtable pointer
    extern char lbl_eu_8056FDA8[];         // task vtable data - array type avoids sda21
    extern u8  lbl_eu_806659D0;            // global NAND "busy" flag
    extern s32 lbl_eu_806659D4;            // global NAND result/error latch

    u32* func_804DA98C(u8 arg);                               // NAND entry-buffer primitive
    s32  func_804DA898(u32* entries, u32* dir, u32* result);  // NAND readdir primitive
}

// CNReqtaskReaddir task payload (the sub-task embedded in CNRequest):
//   +0x00: u32* mBuf    -- entry/result buffer filled by the readdir primitive
//   +0x04: u32  mCount  -- max entry count
//   +0x08: u32* mDir    -- NAND directory handle
//   +0x0C: u8   mType   -- entry type filter byte
//   +0x0D: u8   mState  -- async state machine step (0..3)
struct CNReqtaskReaddirData {
    u32* mBuf;    // +0x00
    u32  mCount;  // +0x04
    u32* mDir;    // +0x08
    u8   mType;   // +0x0C
    u8   mState;  // +0x0D
};

// us-804df3b8: func_804DB0F0
// Configures the CNReqtaskReaddir sub-task: records the entry buffer, max count,
// directory handle and type filter, resets the async state, clears the entry
// buffer and directory, then returns the task vtable pointer.
extern "C" CNReqtaskReaddirVtbl** func_804DB0F0(CNReqtaskReaddirData* d, u32* entries, u32 count, u32* dir, u8 arg) {
    d->mBuf = entries;
    d->mCount = count;
    d->mDir = dir;
    d->mType = arg;
    d->mState = 0;
    *(u8*)entries = 0;
    *dir = 0;
    return &lbl_eu_806659F0;
}

// us-804df3e0: func_804DB114
// Async NAND readdir state machine, polled by the CNand completion pump.
// Advancing one step per call; returns 1 when finished, 2 on error, and 0 while
// still in progress. Steps:
//   0 -> begin listing the directory (func_804DA898)
//   1 -> read entries into mBuf (func_804DA898)
//   2..3 -> finish the listing and report the result
extern "C" s32 func_804DB114(CNReqtaskReaddirVtbl* vtable_ptr, CNReqtaskReaddirData* d) {
    if (lbl_eu_806659D0 != 0) { // NAND subsystem busy
        return 0;
    }

    // If the listing has begun, inspect the global result latch for a leftover
    // failure before advancing the state machine.
    s32 state = (s8)d->mState;
    if (state > 0) {
        if (state != 1) {
            if (lbl_eu_806659D4 < 0) return 2;
        } else {
            s32 err = lbl_eu_806659D4;
            if (err == -12) {
                d->mState = 3;
            } else if (err < 0) {
                return 2;
            } else {
                u32* dir = d->mDir;
                u32 cnt = d->mCount;
                if (cnt < ((u32*)dir)[0] * 13) return 2;
            }
        }
    }

    switch ((s8)d->mState) {
        case 0: {
            u32* r = func_804DA98C(d->mType);
            s32 res = func_804DA898(0, d->mDir, (u32*)(u32)r);
            if (res != 0) {
                return 2;
            }
            d->mState = 1;
            goto ret0;
        }
        case 1: {
            u32* r = func_804DA98C(d->mType);
            s32 res = func_804DA898(d->mBuf, d->mDir, (u32*)(u32)r);
            if (res != 0) {
                return 2;
            }
            d->mState = 2;
            goto ret0;
        }
        case 2:
            d->mState = 3;
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
// .ctors static initializer: installs the CNReqtaskReaddir vtable
// (lbl_eu_8056FDA8) into the task vtable pointer (lbl_eu_806659F0).
//
// Retail keeps the thunk `li r3,&lbl_eu_806659F0@sda21; b func_804DB23C`
// as a tail call into the adjacent helper (the annotation originally merged
// the two bodies into one 0x18 symbol). The helper stores the vtable
// address through r3. `char[]` type for the vtable keeps the address
// constant in a lis/addi pair (no sda21 dereference).
extern "C" __declspec(noinline) void func_804DB23C(void* dest) {
    *(void**)dest = (void*)lbl_eu_8056FDA8;
}
extern "C" __declspec(noinline) void sinit_804DB228() {
    func_804DB23C(&lbl_eu_806659F0);
}