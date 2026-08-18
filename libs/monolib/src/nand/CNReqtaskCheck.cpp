// Decompiled: CNReqtaskCheck
//
// Translation-unit final form.
//
// The CNReqtaskCheck sub-task block lives inside a CNRequest (at offset 4).
// MWCC mangles the names of free functions and data globals; the retail
// symbols here are stripped placeholder names (func_8...) and the data items
// are referenced by their absolute linker labels (lbl_8...). To keep the
// emitted object symbols/labels byte-identical to retail we must keep C
// linkage for exactly those names (exempt in PLAN.md §17.6). Everything that
// is meaningful C++ (the task data layout) is expressed as a real struct.
#include <types.h>

// Vtable of the CNReqtaskCheck task. Its layout is unknown (we only ever
// install it / take its address), so it is used as an opaque pointer type.
struct CNReqtaskCheckVtbl;

// Retail linker names referenced by this unit (C linkage so the emitted
// symbols match the stripped retail names rather than C++ manglings).
extern "C" {
    extern CNReqtaskCheckVtbl* lbl_eu_80665A00; // installed vtable pointer for this task
    extern u32 lbl_eu_8056FDE8[]; // vtable data - array type prevents sda21
    extern u8  lbl_eu_806659D0;    // global NAND "busy" flag
    extern s32 lbl_eu_806659D4;    // global NAND result/error latch

    s32 func_804DA4E0(u32 arg1, u32 arg2, u32 arg3); // NAND check primitive
}

// CNReqtaskCheck task parameter block (the sub-task embedded in CNRequest):
//   +0x00: u32 field_0x0 -- first  check argument
//   +0x04: u32 field_0x4 -- second check argument
//   +0x08: u32 field_0x8 -- third  check argument
//   +0x0C: u8  state     -- async state machine step (0..2)
struct CNReqtaskCheckData {
    u32 field_0x0;  // +0x00
    u32 field_0x4;  // +0x04
    u32 field_0x8;  // +0x08
    u8  state;      // +0x0C
};

// us-804df660: func_804DB348
// Initializes the check task data block (three check arguments and resets the
// state to step 0), then returns the task vtable pointer.
extern "C" CNReqtaskCheckVtbl** func_804DB348(CNReqtaskCheckData* data, u32 arg1, u32 arg2, u32 arg3) {
    CNReqtaskCheckData* d = data;
    d->field_0x0 = arg1;
    d->field_0x4 = arg2;
    d->field_0x8 = arg3;
    d->state = 0;
    return &lbl_eu_80665A00;
}

// us-804df67c: func_804DB364
// Async state machine for the NAND check task, polled by the CNand completion
// pump. Advances one step per call; returns 1 when done, 2 on error, 0 while
// still in progress. Steps:
//   0 -> run the NAND check (func_804DA4E0) with the stored arguments
//   1 -> mark the request complete (return 1 on the following poll)
//   2 -> done (return 1)
extern "C" s32 func_804DB364(CNReqtaskCheckVtbl* vtable_ptr, CNReqtaskCheckData* data) {
    CNReqtaskCheckData* d = data;

    if (lbl_eu_806659D0 != 0) { // NAND subsystem busy
        return 0;
    }

    // If work has begun and the global result latch has gone negative,
    // the pending operation failed.
    if ((s8)d->state > 0) {
        if (lbl_eu_806659D4 < 0) {
            return 2;
        }
    }

    switch ((s8)d->state) {
        case 0: {
            s32 r = func_804DA4E0(d->field_0x0, d->field_0x4, d->field_0x8);
            if (r != 0) {
                return 2;
            }
            d->state = 1;
            goto ret0;
        }
        case 1:
            d->state = 2;
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
// .ctors static initializer: installs the CNReqtaskCheck vtable
// (lbl_eu_8056FDE8) into the task vtable pointer (lbl_eu_80665A00).
//
// Retail keeps the thunk `li r3,&lbl_eu_80665A00@sda21; b func_804DB440`
// as a tail call into the adjacent helper (the annotation originally merged
// the two bodies into one 0x18 symbol). The helper stores the vtable
// address through r3. `char[]` type for the vtable keeps the address
// constant in a lis/addi pair (no sda21 dereference).
extern "C" __declspec(noinline) void func_804DB440(void* dest) {
    *(void**)dest = (void*)lbl_eu_8056FDE8;
}
extern "C" __declspec(noinline) void sinit_804DB420() {
    func_804DB440(&lbl_eu_80665A00);
}
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// func_804DB364 is defined in this TU; func_804DA4CC is foreign (CNReqtaskSave).
extern "C" void func_804DA4CC();
extern "C" u32 lbl_eu_80663B70;   // foreign .sdata
extern "C" u32 lbl_eu_80663BA8[2]; // this unit's sdata

// [.data] 0x8056FDE8-0x8056FE08 (32B): CNReqtaskCheck vtable pair
extern "C" u32 lbl_eu_8056FDE8[4] = {
    (u32)&lbl_eu_80663BA8, 0x00000000,
    (u32)&func_804DB364, (u32)&func_804DA4CC,
};
extern "C" u32 lbl_eu_8056FDF8[4] = {
    (u32)&lbl_eu_80663B70, 0x00000000, 0x00000000, 0x00000000,
};

// [.rodata] 0x80524648-0x80524658 (16B)
extern "C" __declspec(align(4)) const char lbl_eu_80524648[0x10] = {
    0x43,0x4E,0x52,0x65,0x71,0x74,0x61,0x73,0x6B,0x43,0x68,0x65,0x63,0x6B,0x00,0x00,
};

// [.sdata] 0x80663BA8-0x80663BB0 (8B)
extern "C" u32 lbl_eu_80663BA8[2] = { (u32)&lbl_eu_80524648, (u32)&lbl_eu_8056FDF8 };

// [.sbss] 0x80665A00-0x80665A10 (16B) zero-fill. 80665A00 is an 8B retail
// label (the installed-vtable pointer + 1 pad word); keep the extern type.
CNReqtaskCheckVtbl* lbl_eu_80665A00;
u32 lbl_eu_80665A04;
u32 lbl_eu_80665A08[2];
