// Decompiled: CNReqtaskRemove
//
// Translation-unit final form.
//
// The CNReqtaskRemove sub-task block lives inside a CNRequest (at offset 4).
// MWCC mangles the names of free functions and data globals; the retail
// symbols here are stripped placeholder names (func_8...) and the data items
// are referenced by their absolute linker labels (lbl_8...). To keep the
// emitted object symbols/labels byte-identical to retail we must keep C
// linkage for exactly those names (exempt in PLAN.md 17.6). Everything that
// is meaningful C++ (the task data layout) is expressed as a real struct.
#include <types.h>
#include <string.h>

// Vtable of the CNReqtaskRemove task. Its layout is unknown (we only ever
// install it / take its address), so it is used as an opaque pointer type.
struct CNReqtaskRemoveVtbl;
struct CNReqtaskRemoveData;  // defined below; forward decl for the extern block

// Retail linker names referenced by this unit (C linkage so the emitted
// symbols match the stripped retail names rather than C++ manglings).
extern "C" {
    extern CNReqtaskRemoveVtbl* lbl_eu_806659F8; // installed vtable pointer for this task (defined below)
    extern u32 lbl_eu_8056FDC8[4];  // vtable data - array type prevents sda21 (defined below)
    extern u8   lbl_eu_806659D0;    // global NAND "busy" flag
    extern s32  lbl_eu_806659D4;    // global NAND result/error latch
    extern u32 lbl_eu_8056FDD8[4];  // 2nd vtable (defined below)
    extern u32 lbl_eu_80663BA0[2];  // .sdata typeinfo (defined below)
    extern u32 lbl_eu_80663B70;     // foreign .sdata typeinfo
    extern const char lbl_eu_80524638[0x10];
    void func_804DA4CC();           // foreign vtable slot

    s32 func_804DA9C4(CNReqtaskRemoveData* data, u8 arg);  // NAND remove request setup primitive
    s32 func_804DA76C(u8* ptr);                            // NAND remove request execute primitive
}

// CNReqtaskRemove task parameter block (the sub-task embedded in CNRequest):
//   +0x00: char path[13]  NAND path of the file to remove (written by strcpy).
//   +0x0D: s8   state     remove state machine step (see enum below).
//   +0x0E: u8   field_E   remove type / flags passed through to the remove request.
enum CNReqtaskRemoveState {
    kRemoveIdle         = 0,   // request created, removal not yet started
    kRemoveInProgress   = 1,   // removal issued, awaiting completion
    kRemoveDone         = 2,   // removal finished
};

struct CNReqtaskRemoveData {
    char path[13];  // +0x00 NAND path of the file to remove
    s8   state;     // +0x0D remove state machine step
    u8   field_E;   // +0x0E remove type / flags
};

// us-804df54c: func_804DB240
// Configures the CNReqtaskRemove sub-task: copies the target path, records the
// remove flags, resets the state to idle, then returns the task vtable pointer.
//
// Note: the first parameter stays an opaque byte pointer (`u8*`, not
// `CNReqtaskRemoveData*`) on purpose - keeping it distinct from the typed local
// `d` preserves the two-register colouring that matches retail; the
// byte-identical form comes from `u8*` + a typed local.
extern "C" CNReqtaskRemoveVtbl** func_804DB240(u8* data, const char* path, u8 arg) {
    CNReqtaskRemoveData* d = (CNReqtaskRemoveData*)data;
    u8 flags = arg;                       // hoisted into r30 before strcpy
    strcpy(d->path, path);
    d->field_E = flags;
    d->state = kRemoveIdle;
    return &lbl_eu_806659F8;
}

// us-804df58c: func_804DB278
// Async state machine for the NAND remove task, polled by the CNand completion
// pump. Advances one step per call; returns 1 when the removal is done, 2 on
// error, 0 while still in progress. Steps:
//   0 -> start the removal (func_804DA9C4 + func_804DA76C)
//   1 -> mark the request done (return 1 on the following poll)
//   2 -> done (return 1)
extern "C" s32 func_804DB278(CNReqtaskRemoveVtbl* vtable_ptr, CNReqtaskRemoveData* d) {
    if (lbl_eu_806659D0 != 0) { // NAND subsystem busy
        return 0;
    }

    // If a remove has begun and the global result latch has gone negative,
    // the pending operation failed.
    if (d->state > 0) {
        if (lbl_eu_806659D4 < 0) {
            return 2;
        }
    }

    switch (d->state) {
        case kRemoveIdle: {           // start the removal
            s32 r = func_804DA76C((u8*)(u32)func_804DA9C4(d, d->field_E));
            if (r != 0) {
                return 2;
            }
            d->state = kRemoveInProgress;
            goto ret0;
        }
        case kRemoveInProgress:
            d->state = kRemoveDone;
            goto ret0;
        case kRemoveDone:
            return 1;
        default:
            break;
    }
ret0:
    return 0;
}

// us-804df648: sinit_804DB330
// .ctors static initializer: installs the CNReqtaskRemove vtable
// (lbl_eu_8056FDC8) into the task vtable pointer (lbl_eu_806659F8).
//
// Retail keeps the thunk `li r3,&lbl_eu_806659F8@sda21; b func_804DB350`
// as a tail call into the adjacent helper (the annotation originally merged
// the two bodies into one 0x18 symbol). The helper stores the vtable
// address through r3. `char[]` type for the vtable keeps the address
// constant in a lis/addi pair (no sda21 dereference).
extern "C" __declspec(noinline) void func_804DB350(void* dest) {
    *(void**)dest = (void*)lbl_eu_8056FDC8;
}
extern "C" __declspec(noinline) void sinit_804DB330() {
    func_804DB350(&lbl_eu_806659F8);
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056FDC8-0x8056FDE8 (32 bytes) — two vtables.
extern "C" u32 lbl_eu_8056FDC8[4] = { (u32)&lbl_eu_80663BA0, 0x00000000, (u32)&func_804DB278, (u32)&func_804DA4CC };
extern "C" u32 lbl_eu_8056FDD8[4] = { (u32)&lbl_eu_80663B70, 0x00000000, 0x00000000, 0x00000000 };

// [.sdata] 0x80663BA0-0x80663BA8 (8 bytes) typeinfo {name,parent}.
extern "C" u32 lbl_eu_80663BA0[2] = { (u32)&lbl_eu_80524638, (u32)&lbl_eu_8056FDD8 };

// [.rodata] 0x80524638-0x80524648 (16 bytes) "CNReqtaskRemove" RTTI name.
extern "C" const char lbl_eu_80524638[0x10] = {
    0x43,0x4E,0x52,0x65,0x71,0x74,0x61,0x73,0x6B,0x52,0x65,0x6D,0x6F,0x76,0x65,0x00 };

// [.sbss] 0x806659F8-0x80665A00 (8 bytes) — the task vtable pointer + pad.
extern "C" {
    CNReqtaskRemoveVtbl* lbl_eu_806659F8;  // definition (zero-init, sbss word 0)
    u32 lbl_eu_806659F8_pad;               // 2nd sbss word
}
