// Decompiled: CNReqtaskSave
//
// CNReqtaskSave owns the CNRequest base task object and the NAND error-message
// plumbing shared by all monolib NAND sub-task units. Like its matched siblings
// (CNReqtaskLoad/Check/Remove/Readdir/SaveBanner), the retail symbols are
// stripped placeholder names (func_8..., sinit_8...) and the data items are
// referenced by absolute linker labels (lbl_8...), so C linkage is kept for
// exactly those names to emit byte-identical symbols/relocs (exempt in
// PLAN.md §17.6). Everything that is meaningful C++ (the CNRequest layout,
// sub-task parameter blocks) lives in CNReqtaskSave.hpp.
#include <types.h>
#include <string.h>
#include <revolution/NAND.h>
#include "libs/monolib/include/monolib/util/FixStr.hpp"
#include "libs/monolib/src/nand/CNReqtaskSave.hpp"

// CException (monolib/core) - only its address/return value is used here.
class CException;

// Retail linker names referenced by this unit (C linkage so the emitted
// symbols match the stripped retail names rather than C++ manglings).
extern "C" { // lbl_* and func_* retail names need unmangled emission
    extern CNReqtaskSaveVtbl* lbl_eu_806659E0;   // installed save-task vtable pointer (sinit target)
    extern u32 lbl_eu_8056FD68[];               // save-task vtable data - array type prevents sda21
    extern u8 lbl_eu_806659D8[8];                // save-task "open" flag block (byte 0 read)
    extern const wchar_t* lbl_eu_80663B60;       // NAND error message for -4 / -64
    extern const wchar_t* lbl_eu_80663B64;       // NAND error message for -3 / -2
    extern const char lbl_eu_80524608[];         // "%s%s" format string (.rodata)
    extern char* lbl_eu_80663B80;                // save path prefix (.sdata)
    extern s8 lbl_eu_806659E4;                   // path-buffer initialized flag (retail 1B @sda21)
    extern u8 lbl_eu_80660000[40];               // static FixStr<32> path buffer (.bss)

    // NAND subsystem primitives (stripped retail names -> C linkage).
    // Note: retail func_804DB348 (CNReqtaskCheck unit) and func_804DACE8 (this
    // unit) actually take more arguments; the init helpers here call them with
    // only the task-block pointer, so the in-TU declarations are 1-arg to
    // reproduce the retail call bytes.
    s32 func_804DA69C(void);                                 // NAND close primitive (defined below)
    const char* func_804DAEE8(CNReqtaskSaveData* data);      // save path/handle builder (defined below)
    s32 func_804DA540(const char* path, u8 flag);            // NAND open wrapper (defined below)
    const char* func_804DA98C(u8 id);                        // temp-path builder (defined below)
    void func_804DA97C(void* param);                         // NAND completion callback (defined below)
    CNReqtaskCheckVtbl** func_804DB348(CNReqtaskCheckData* data);  // check sub-task config
    CNReqtaskSaveVtbl**  func_804DACE8(CNReqtaskSaveData* data);   // save sub-task config (defined below)
    CException* logExceptionMessage__10CExceptionFv(const wchar_t* msg); // CException::logExceptionMessage (stripped name)

    // Init sub-task config helpers from the sibling NAND sub-task units. The
    // wrappers below forward their own arguments straight through: the retail
    // call sites only set r3 (the data block) because the args are already in
    // r4-r7, so these full-signature declarations reproduce the call bytes.
    CNReqtaskLoadVtbl**       func_804DAF70(u8* data, const char* path, u32 arg2, u32 arg3, u8 arg4); // load sub-task config
    CNReqtaskRemoveVtbl**     func_804DB240(u8* data, const char* path, u8 arg);                     // remove sub-task config
    CNReqtaskReaddirVtbl**    func_804DB0F0(CNReqtaskReaddirData* d, u32* entries, u32 count, u32* dir, u8 arg); // readdir sub-task config
    char**                    func_eu_804F9E98(MonoRequestState* req, const char* src, u8 flagD, u8 flagE);     // request-record setup
    CNReqtaskSaveBannerVtbl** func_804F4D7C(CNReqtaskSaveBannerData* data, CNReqtaskSaveBannerTarget* arg1, u8 arg2); // save-banner sub-task config

    // The five matched targets below (definitions inherit C linkage).
    void func_804DA4CC(CNandTask* data, CNandTask* dealloc);        // NAND dealloc helper
    CException* func_804DAA58(s32 code);                            // NAND error dispatcher
    bool func_804DAB80(CNRequest* self);                            // init check sub-task
    bool func_804DABBC(CNRequest* self);                            // init save sub-task
    void sinit_804DAF58();                                   // .ctors vtable install

    // The five targets below (definitions inherit C linkage).
    int func_804DABF8(CNRequest* req, u32 a1, u32 a2, u32 a3, u8 flag);      // init load sub-task
    int func_804DAC34(CNRequest* req, u32 a1, u8 flag);                      // init remove sub-task
    int func_804DAC70(CNRequest* req, u32 a1, u32 a2, u32 a3, u8 flag);      // init readdir sub-task
    int func_eu_804DEF20(CNRequest* req, const char* buf, u8 size, u8 a3); // request-record setup wrapper
    int func_804DACAC(CNRequest* req, u32 a1, u8 a2);                       // init save-banner sub-task

    s32 func_804DA4E0(u32 neededBlocks, u32 neededFiles, u32* answer); // NAND check (async)

    // Task poll + save state machine (definitions inherit C linkage).
    int func_804DAAF8(CNRequest* self, bool* out);                 // CNRequest task poll (defined below)
    s32 func_804DAD38(CNReqtaskSaveVtbl* vtable, CNReqtaskSaveData* data); // save state machine (defined below)

    // The five NAND async wrappers below (definitions inherit C linkage).
    // Each stamps the busy flag/result latch, forwards its own arguments to
    // the matching NAND*Async (r3.. pass through untouched), and routes an
    // immediate nonzero return through the error dispatcher.
    s32 func_804DA70C(const char* path, u8 perm, u8 attr);   // NAND create (async)
    s32 func_804DA76C(const char* path);                     // NAND delete (async)
    s32 func_804DA7CC(const char* from, const char* to);     // NAND move (async)
    s32 func_eu_804DEB4C(const char* path, u8 perm, u8 attr); // NAND create-dir (async)
    s32 func_804DA91C(const char* path);                     // NAND change-dir (async)
    s32 func_804DA82C(u32* pos);                             // NAND tell (async)
    s32 func_804DA628(u32 addr, u32 size);                   // NAND write (async) - fixed save file-info (defined below)
    s32 func_804DA5B4(u32 addr, u32 size);                   // NAND read (async) - fixed save file-info (defined below)
    s32 func_804DA898(char* nameList, u32* num, const char* path); // NAND read-dir (async) (defined below)
}

// us-804de70c: func_804DA4CC
// NAND dealloc helper (also referenced by CNReqtaskSaveBanner): while the
// save-task "open" flag is set, close the pending NAND handle (func_804DA69C).
// The task arguments are unused here (retail ignores them).
// Retail only tests byte 0 of the 8-byte .sbss flag (lbz @sda21), so the
// block-scope declaration narrows the view to a u8 for this read.
void func_804DA4CC(CNandTask* data, CNandTask* dealloc) {
    if (lbl_eu_806659D8[0] != 0) {
        func_804DA69C();
    }
}

// us-804deccc: func_804DAA58
// NAND error dispatcher: maps a NAND error code to a CException via tail-call.
// -4 / -64 use the general message (lbl_eu_80663B60), -3 / -2 use the
// save-specific message (lbl_eu_80663B64); any other code is returned as-is.
// noinline: the five NAND async wrappers below emit `bl func_804DAA58`;
// without it MWCC inlines this body into each caller.
__declspec(noinline) CException* func_804DAA58(s32 code) {
    if (code == -4) {
        return logExceptionMessage__10CExceptionFv(lbl_eu_80663B60);
    }
    if (code == -3 || code == -2) {
        return logExceptionMessage__10CExceptionFv(lbl_eu_80663B64);
    }
    if (code == -0x40) {
        return logExceptionMessage__10CExceptionFv(lbl_eu_80663B60);
    }
    return (CException*)code;
}

// us-804dedf4: func_804DAB80
// CNRequest helper: initialises the CNReqtaskCheck sub-task block at +0x04 and
// records the returned task vtable at +0x00; returns whether init succeeded.
bool func_804DAB80(CNRequest* self) {
    CNReqtaskCheckVtbl** v = func_804DB348((CNReqtaskCheckData*)self->field_0x4);
    self->field_0x0 = (CNandTask*)v;
    return v != 0;
}

// us-804dee30: func_804DABBC
// CNRequest helper: initialises the CNReqtaskSave sub-task block at +0x04 and
// records the returned task vtable at +0x00; returns whether init succeeded.
bool func_804DABBC(CNRequest* self) {
    CNReqtaskSaveVtbl** v = func_804DACE8((CNReqtaskSaveData*)self->field_0x4);
    self->field_0x0 = (CNandTask*)v;
    return v != 0;
}

// us-804df214: sinit_804DAF58
// .ctors static initializer: installs the CNReqtaskSave vtable
// (lbl_eu_8056FD68) into the task vtable pointer (lbl_eu_806659E0).
//
// Retail keeps the thunk `li r3,&lbl_eu_806659E0@sda21; b func_804DAF60`
// as a tail call into the adjacent helper (the annotation originally merged
// the two bodies into one 0x18 symbol).  The helper stores the vtable
// address through r3.  `char[]` type for the vtable keeps the address
// constant in a lis/addi pair (no sda21 dereference).
extern "C" __declspec(noinline) void func_804DAF60(void* dest) {
    *(void**)dest = (void*)lbl_eu_8056FD68;
}
extern "C" __declspec(noinline) void sinit_804DAF58() {
    func_804DAF60(&lbl_eu_806659E0);
}

// us-804de720: func_804DA4E0
// NAND check (async) wrapper; same shape as func_804DA70C: stamps the busy
// flag, clears the result latch, launches NANDCheckAsync with the completion
// callback and shared command block (needed blocks/files/answer forwarded
// from the caller), routes an immediate nonzero return through the error
// dispatcher and clears the busy flag. The raw NAND result is returned.
// (Upstream of any optimize_for_size "off" pair, so the unit-level -O4,s
// already produces the retail stmw r30 frame here.)
s32 func_804DA4E0(u32 neededBlocks, u32 neededFiles, u32* answer) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDCheckAsync(neededBlocks, neededFiles, answer,
                             (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    }
    return ret;
}

// us-804de780: func_804DA540
// NAND open (async) wrapper for the save flow's open step: stamps the busy
// flag, clears the result latch, launches NANDOpenAsync on the fixed save
// file-info (lbl_eu_8065FEEC) with the completion callback and shared command
// block. On success latches the "save file open" flag (lbl_eu_806659D8 byte 0);
// on an immediate error routes through func_804DAA58 and clears the busy flag.
// The raw NAND result is returned either way. flag is u8 so forwarding it to
// NANDOpenAsync's u8 mode emits no rlwinm truncation (retail does mr r5,r4).
// noinline: retail func_804DAD38 emits `bl func_804DA540`; without it MWCC
// inlines this wrapper into the state machine.
__declspec(noinline) s32 func_804DA540(const char* path, u8 flag) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDOpenAsync(path, &lbl_eu_8065FEEC, flag,
                            (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    // Written with the error path as the if-body: MWCC lowers this to the
    // retail `cmpwi/beq` shape branching to the success store.
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    } else {
        lbl_eu_806659D8[0] = 1;
    }
    return ret;
}

// us-804de7f4: func_804DA5B4
// NAND read (async) wrapper; same shape as func_804DA628 but reads from the
// fixed save file-info (lbl_eu_8065FEEC): stamps the busy flag, clears the
// shared result latch, launches NANDReadAsync with the caller's buffer
// address/size, completion callback and command block, routes an immediate
// nonzero return through func_804DAA58 and clears the busy flag.
__declspec(noinline) s32 func_804DA5B4(u32 addr, u32 size) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDReadAsync(&lbl_eu_8065FEEC, (u8*)addr, size,
                            (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    }
    return ret;
}

// us-804de868: func_804DA628
// NAND write (async) wrapper for the save flow: stamps the busy flag, clears
// the shared result latch, then launches NANDWriteAsync against the fixed save
// file-info (lbl_eu_8065FEEC) with the buffer address/size handed in from the
// caller (callback and command block go in r6/r7). On an immediate nonzero
// return the error is routed through func_804DAA58 and the busy flag is
// cleared. The raw NAND result is returned either way (kept in r30 across the
// dispatcher call). noinline: retail func_804DAD38 emits `bl func_804DA628`;
// without it MWCC inlines this wrapper into the state machine.
__declspec(noinline) s32 func_804DA628(u32 addr, u32 size) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDWriteAsync(&lbl_eu_8065FEEC, (const void*)addr, size,
                             (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    }
    return ret;
}

// NAND close primitive: stamp the busy flag, clear the result latch, launch
// NANDCloseAsync (retail passes the callback first, then the block and file
// info globals), route an immediate nonzero return through func_804DAA58,
// then clear the busy flag and hand back the raw NAND result.
#pragma optimize_for_size on
s32 __declspec(noinline) func_804DA69C(void) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    // Retail argument order: file-info, completion callback, command block.
    s32 ret = NANDCloseAsync(
        reinterpret_cast<NANDFileInfo*>(&lbl_eu_8065FEEC),
        (NANDAsyncCallback)func_804DA97C,
        &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    } else {
        // Success path clears the save-open flag byte, not the busy flag.
        lbl_eu_806659D8[0] = 0;
    }
    return ret;
}
#pragma optimize_for_size off

// us-804de94c: func_804DA70C
// NAND create (async) wrapper: stamps the busy flag, clears the shared result
// latch, then launches NANDCreateAsync with the completion callback
// (func_804DA97C) and the shared command block; path/perm/attr arrive in
// r3/r4/r5 from the caller. On an immediate nonzero return the error is
// routed through func_804DAA58 and the busy flag is cleared. The raw NAND
// result is returned either way (kept in r30 across the dispatcher call).
// noinline: retail func_804DAD38 emits `bl func_804DA70C`; without it MWCC
// inlines this wrapper into the state machine.
#pragma push
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
__declspec(noinline) s32 func_804DA70C(const char* path, u8 perm, u8 attr) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDCreateAsync(path, perm, attr, (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    }
    return ret;
}
#pragma pop

// us-804de9ac: func_804DA76C
// NAND delete (async) wrapper; same shape as func_804DA70C but only the path
// arrives from the caller (callback/command block go in r4/r5).
#pragma push
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
s32 func_804DA76C(const char* path) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDDeleteAsync(path, (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    }
    return ret;
}
#pragma pop

// us-804dea0c: func_804DA7CC
// NAND move (async) wrapper; same shape as func_804DA70C but the source and
// destination paths arrive from the caller (callback/command block in r5/r6).
// noinline: retail func_804DAD38 emits `bl func_804DA7CC`; without it MWCC
// inlines this wrapper into the state machine.
#pragma push
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
__declspec(noinline) s32 func_804DA7CC(const char* from, const char* to) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDMoveAsync(from, to, (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    }
    return ret;
}
#pragma pop

// us-804dea6c: func_804DA82C
// NAND tell (async) wrapper; same shape as func_804DA70C: stamps the busy
// flag, clears the result latch, launches NANDTellAsync on the fixed save
// file-info (lbl_eu_8065FEEC) with the caller's position out-pointer,
// completion callback and shared command block. On an immediate nonzero
// return the error is routed through func_804DAA58 and the busy flag is
// cleared. The raw NAND result is returned either way.
#pragma push
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
__declspec(noinline) s32 func_804DA82C(u32* pos) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDTellAsync(&lbl_eu_8065FEEC, pos,
                            (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    }
    return ret;
}
#pragma pop

// us-804dead8: func_804DA898
// NAND read-dir (async) wrapper; same shape as func_804DA70C. The caller (the
// CNReqtaskReaddir unit) hands over the entry-name buffer, count and directory
// path in that order, and the wrapper re-orders them into NANDReadDirAsync's
// (path, nameList, num) call (retail: r3=arg3, r4=arg1, r5=arg2). noinline
// keeps this wrapper from being folded into same-TU callers.
#pragma push
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
__declspec(noinline) s32 func_804DA898(char* nameList, u32* num, const char* path) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDReadDirAsync(path, nameList, num,
                               (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    }
    return ret;
}
#pragma pop

// us-804deb4c: func_eu_804DEB4C
// NAND create-dir (async) wrapper; same shape as func_804DA70C (callback and
// command block in r6/r7).
#pragma push
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
s32 func_eu_804DEB4C(const char* path, u8 perm, u8 attr) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDCreateDirAsync(path, perm, attr, (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    }
    return ret;
}
#pragma pop

// us-804debac: func_804DA91C
// NAND change-dir (async) wrapper; same shape as func_804DA76C (callback and
// command block in r4/r5).
#pragma push
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
s32 func_804DA91C(const char* path) {
    lbl_eu_806659D0 = 1;
    lbl_eu_806659D4 = 0;
    s32 ret = NANDChangeDirAsync(path, (NANDAsyncCallback)func_804DA97C, &lbl_eu_8065FE30);
    if (ret != 0) {
        func_804DAA58(ret);
        lbl_eu_806659D0 = 0;
    }
    return ret;
}
#pragma pop

extern "C" void func_804DA97C(void* param) {
    lbl_eu_806659D0 = 0;
    lbl_eu_806659D4 = (s32)param;
}

// Temp-path builder (stub; symbol kept for the func_804DAD38 move step).
// noinline: retail func_804DAD38 emits `bl func_804DA98C`; without it MWCC
// inlines this placeholder and changes the caller.
extern "C" long NANDGetHomeDir(char*);
extern "C" int sprintf(char*, const char*, ...);
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
__declspec(noinline) const char* func_804DA98C(u8 id) {
    extern char lbl_eu_8065FF78[];
    extern const char lbl_eu_805245D4[];
    long st = NANDGetHomeDir(lbl_eu_8065FF78);
    func_804DAA58((s32)st);
    if (id != 0)
        strcat(lbl_eu_8065FF78, (char*)lbl_eu_805245D4);
    return (const char*)lbl_eu_8065FF78;
}
#pragma optimize_for_size off

// Format the temp path into lbl_eu_8065FFBC via sprintf with the id-driven
// sub-path and the object pointer.
#pragma optimize_for_size on  // -O4,s stmw frame
extern "C" char* func_804DA9C4(u32 a1, u8 a2) {
    extern char lbl_eu_8065FFBC[];
    extern const char lbl_eu_805245D4[];
    sprintf(lbl_eu_8065FFBC, (char*)lbl_eu_805245D4 + 7, func_804DA98C(a2), a1);
    return lbl_eu_8065FFBC;
}
#pragma optimize_for_size off

// Set the NAND error message string for error code -4 / -64
void setGeneralNandErrorMessage__FPCw(const wchar_t* msg) {
    extern const wchar_t* lbl_eu_80663B60;
    lbl_eu_80663B60 = msg;
}

// Set the NAND error message string for error code -3
void setSaveNandErrorMessage__FPCw(const wchar_t* msg) {
    extern const wchar_t* lbl_eu_80663B64;
    lbl_eu_80663B64 = msg;
}

extern "C" void setExtraNandErrorMessage__FPCw(const wchar_t* msg){
    extern const wchar_t* lbl_eu_80663B68;
    lbl_eu_80663B68 = msg;
}

// CNRequest ctor: install the vtable label at +0x20, clear +0 and memset
// the +4..+0x20 sub-object region (28 bytes).
extern "C" void* __ct__CNRequest(void* self) {
    extern u32 lbl_eu_8056FD58[];
    *(u32*)((u8*)self + 0x20) = (u32)lbl_eu_8056FD58;
    *(u32*)((u8*)self + 0x00) = 0;
    memset((u8*)self + 4, 0, 28);
    return self;
}

// us-804ded6c: func_804DAAF8
// CNRequest task poll, called by the CNand completion pump (CNand.cpp): runs
// the installed sub-task one step through its vtable slot 2 (0=running,
// 1=done, 2=error). When the task has finished, the completion status byte
// (done == 1) is stored through `out`, the task is finalized via vtable slot
// 3, the task slot is cleared and 1 is returned; while the task is still
// running, 0 is returned and `out` is untouched.
#pragma push
#pragma optimize_for_size on
int func_804DAAF8(CNRequest* self, bool* out) {
    int ret = self->field_0x0->taskSlot2(self->field_0x4);
    if (ret != 0) {
        *out = (ret == 1);
        self->field_0x0->taskSlot3(self->field_0x4);
        self->field_0x0 = 0;
        return 1;
    }
    return 0;
}
#pragma pop

// us-804dee6c: func_804DABF8
// CNRequest helper: initialises the CNReqtaskLoad sub-task block at +0x04
// (forwarding the NAND path/buffer/size/flag) and records the returned task
// vtable at +0x00; returns whether init succeeded.
// optimize_for_size: retail converts the returned pointer to bool with the
// int-style addic/subfe pair (-O4,s lowering of "!= 0"); the preceding
// "off" pragma pairs in this TU otherwise revert this function to the
// pointer-style neg/or/rlwinm form.
#pragma push
#pragma optimize_for_size on
int func_804DABF8(CNRequest* req, u32 a1, u32 a2, u32 a3, u8 flag) {
    CNReqtaskLoadVtbl** v = func_804DAF70((u8*)req->field_0x4, (const char*)a1, a2, a3, flag);
    req->field_0x0 = (CNandTask*)v;
    return v != 0;
}
#pragma pop

// us-804deea8: func_804DAC34
// CNRequest helper: initialises the CNReqtaskRemove sub-task block at +0x04
// (forwarding the NAND path and remove flags) and records the returned task
// vtable at +0x00; returns whether init succeeded.
#pragma push
#pragma optimize_for_size on
int func_804DAC34(CNRequest* req, u32 a1, u8 flag) {
    CNReqtaskRemoveVtbl** v = func_804DB240((u8*)req->field_0x4, (const char*)a1, flag);
    req->field_0x0 = (CNandTask*)v;
    return v != 0;
}
#pragma pop

// us-804deee4: func_804DAC70
// CNRequest helper: initialises the CNReqtaskReaddir sub-task block at +0x04
// (forwarding the entry buffer, count, directory handle and type filter) and
// records the returned task vtable at +0x00; returns whether init succeeded.
#pragma push
#pragma optimize_for_size on
int func_804DAC70(CNRequest* req, u32 a1, u32 a2, u32 a3, u8 flag) {
    CNReqtaskReaddirVtbl** v = func_804DB0F0((CNReqtaskReaddirData*)req->field_0x4, (u32*)a1, a2, (u32*)a3, flag);
    req->field_0x0 = (CNandTask*)v;
    return v != 0;
}
#pragma pop

// us-804def20: func_eu_804DEF20
// CNRequest helper: builds the request record at +0x04 via the monolib core
// setup helper (forwarding the source string and two flag bytes) and records
// the returned global string pointer at +0x00; returns whether it succeeded.
// The two flag params are u8 (not u32) so forwarding them to the u8 callee
// params emits no rlwinm truncation (retail passes them straight through).
#pragma push
#pragma optimize_for_size on
int func_eu_804DEF20(CNRequest* req, const char* buf, u8 size, u8 a3) {
    char** v = func_eu_804F9E98((MonoRequestState*)req->field_0x4, buf, size, a3);
    req->field_0x0 = (CNandTask*)v;
    return v != 0;
}
#pragma pop

// us-804def5c: func_804DACAC
// CNRequest helper: initialises the CNReqtaskSaveBanner sub-task block at
// +0x04 (forwarding the banner-storage object and banner id) and records the
// returned task vtable at +0x00; returns whether init succeeded.
// a2 is u8 (not u32) so forwarding it to the u8 callee param emits no
// rlwinm truncation (retail passes it straight through).
#pragma push
#pragma optimize_for_size on
int func_804DACAC(CNRequest* req, u32 a1, u8 a2) {
    CNReqtaskSaveBannerVtbl** v = func_804F4D7C((CNReqtaskSaveBannerData*)req->field_0x4, (CNReqtaskSaveBannerTarget*)a1, a2);
    req->field_0x0 = (CNandTask*)v;
    return v != 0;
}
#pragma pop

// Save sub-task config. Retail func_804DACE8 takes 6 arguments
// (data, path, write args, flags): strcpy(data, path), stores the four
// write/flags args at +0x10..0x19, zeroes +0x1A, returns &lbl_eu_806659E0.
// The in-TU 1-arg declaration reproduces func_804DABBC's retail call bytes
// (r4-r8 are caller leftovers in retail too). noinline keeps the caller's
// `bl func_804DACE8` from being inlined.
__declspec(noinline) CNReqtaskSaveVtbl** func_804DACE8(
    CNReqtaskSaveData* data, void* path, u32 a, u32 b, u32 c, u32 d) {
    char* dst = (char*)data;
    strcpy(dst, (const char*)path);
    *(u32*)(dst + 0x10) = a;
    *(u32*)(dst + 0x14) = b;
    *(u8*)(dst + 0x18) = (u8)c;
    *(u8*)(dst + 0x19) = (u8)d;
    *(u8*)(dst + 0x1A) = 0;
    return &lbl_eu_806659E0;
}

// us-804deff0: func_804DAD38
// Async save state machine (the CNReqtaskSave task's vtable-slot-2 poll),
// advanced one step per call; returns 1 when the save is complete, 2 on
// error, 0 while still in progress. `vtable` (this) is unused by the retail
// body - the data block alone drives the machine. Steps:
//   0 -> open the save file (func_804DAEE8 + func_804DA540)
//   1 -> wait for the open result: 0 = write the buffer (func_804DA628),
//        -12 (no such file) = create it (func_804DA70C) and retry via state 6,
//        anything else = error
//   2 -> close the file (func_804DA69C)
//   3 -> move the temp file to the final path (func_804DA7CC)
//   4 -> mark done
//   5 -> report completion (return 1)
//   6 -> retry the open (back to state 0)
// -O4,s (optimize_for_size): retail emits the stmw r30 frame and the compact
// switch layout; without the pragma MWCC compiles this at -O4 and diverges.
#pragma push
#pragma optimize_for_size on
s32 func_804DAD38(CNReqtaskSaveVtbl* vtable, CNReqtaskSaveData* data) {
    CNReqtaskSaveData* d = data;

    if (lbl_eu_806659D0 != 0) { // NAND subsystem busy
        return 0;
    }

    // Once the save has begun (state >= 2), a negative global result latch
    // means the pending operation failed.
    if ((s8)d->state >= 2) {
        if (lbl_eu_806659D4 < 0) {
            return 2;
        }
    }

    switch ((s8)d->state) {
        case 0: {
            s32 r = func_804DA540(func_804DAEE8(d), 2);
            if (r != 0) {
                return 2;
            }
            d->state = 1;
            goto ret0;
        }
        case 1: {
            // Open result dispatch: 0 = write the buffer, -12 (no such file)
            // = create it and retry the open via state 6, anything else fails
            // the save.
            s32 last = lbl_eu_806659D4;
            if (last != 0) {
                if (last == -12) {
                    // No such file: create it (func_804DA70C) and retry the
                    // open via state 6.
                    s32 r = func_804DA70C(func_804DAEE8(d), d->field_0x18, 0);
                    if (r != 0) {
                        return 2;
                    }
                    d->state = 6;
                } else {
                    return 2;
                }
            } else {
                // Open succeeded: write the buffer.
                s32 r = func_804DA628(d->field_0x10, d->field_0x14);
                if (r != 0) {
                    return 2;
                }
                d->state = 2;
            }
            goto ret0;
        }
        case 2: {
            s32 r = func_804DA69C();
            if (r != 0) {
                return 2;
            }
            d->state = 3;
            goto ret0;
        }
        case 3: {
            // Evaluate the temp path first (retail keeps it in r31 across the
            // func_804DAEE8 call that produces the source path).
            const char* to = func_804DA98C(d->field_0x19);
            if (func_804DA7CC(func_804DAEE8(d), to) != 0) {
                return 2;
            }
            d->state = 4;
            goto ret0;
        }
        case 4:
            d->state = 5;
            goto ret0;
        case 5:
            return 1;
        case 6:
            d->state = 0;
            goto ret0;
        default:
            break;
    }
ret0:
    return 0;
}
#pragma pop

// us-804df1a4: func_804DAEE8
// Save path builder: formats "<prefix><sub-path>" ("%s%s", prefix from the
// shared .sdata pointer, sub-path from the task data block) into the unit's
// static FixStr<32> buffer (lbl_eu_80660000) and returns it. On the first
// call (initialized flag still clear) the buffer is cleared and the flag
// latched to 1; later calls skip straight to the format.
// noinline: retail func_804DAD38 emits `bl func_804DAEE8`; without it MWCC
// inlines this placeholder and changes the caller.
__declspec(noinline) const char* func_804DAEE8(CNReqtaskSaveData* data) {
    if (lbl_eu_806659E4 == 0) {
        reinterpret_cast<ml::FixStr<32>&>(lbl_eu_80660000).clear();
        lbl_eu_806659E4 = 1;
    }
    reinterpret_cast<ml::FixStr<32>&>(lbl_eu_80660000)
        .format(lbl_eu_80524608, lbl_eu_80663B80, data);
    return reinterpret_cast<const ml::FixStr<32>&>(lbl_eu_80660000).c_str();
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// func_804DAD38 / func_804DA4CC are defined in this TU with C++ linkage
// (params mangled); reference the retail unmangled names via namespace aliases.
namespace SaveBlob {
extern "C" void func_804DAD38();
extern "C" void func_804DA4CC();
}
extern "C" u32 lbl_eu_80663B70;   // foreign .sdata
extern "C" u32 lbl_eu_80663B88[2]; // this unit's sdata
extern "C" const char lbl_eu_805245F8[]; // rodata sub-string (sdata reloc target)

// [.data] 0x8056FD68-0x8056FD88 (32B): CNReqtaskSave vtable pair
extern "C" u32 lbl_eu_8056FD68[4] = {
    (u32)&lbl_eu_80663B88, 0x00000000,
    (u32)&SaveBlob::func_804DAD38, (u32)&SaveBlob::func_804DA4CC,
};
extern "C" u32 lbl_eu_8056FD78[4] = {
    (u32)&lbl_eu_80663B70, 0x00000000, 0x00000000, 0x00000000,
};

// [.rodata] 0x805245D4-0x80524610 (60B) -- one block (referenced by .text);
// the sub-string labels stay extern for the sdata reloc name (lbl_eu_805245F8).
extern "C" __declspec(align(4)) const char lbl_eu_805245D4[60] = {
    0x2F,0x73,0x68,0x61,0x72,0x65,0x00,0x25,0x73,0x2F,0x25,0x73,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x43,0x4E,0x52,0x65,0x71,0x75,0x65,0x73,0x74,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x43,0x4E,0x52,0x65,0x71,0x74,0x61,0x73,0x6B,0x53,0x61,0x76,
    0x65,0x00,0x00,0x00,0x25,0x73,0x25,0x73,0x00,0x00,0x00,0x00,
};

// [.sdata] 0x80663B88-0x80663B90 (8B)
extern "C" u32 lbl_eu_80663B88[2] = { (u32)&lbl_eu_805245F8, (u32)&lbl_eu_8056FD78 };

// [.bss] 0x8065FE30-0x80660038 (0x208 = 520B) zero-fill
NANDCommandBlock lbl_eu_8065FE30;   // 188B (.bss)
NANDFileInfo lbl_eu_8065FEEC;    // 140B (.bss)
char lbl_eu_8065FF78[68];   // 68B (.bss)
char lbl_eu_8065FFBC[68];   // 68B (.bss)
u8 lbl_eu_80660000[40];
u8 lbl_eu_80660028[16];

// [.sbss] 0x806659D0-0x806659E8 (24B) zero-fill. Per-symbol padding gives the
// retail layout (D0+3pad, D4, D8(8B align8), E0, E4+3pad); align 8 via D8.
u8 lbl_eu_806659D0;
s32 lbl_eu_806659D4;
// Retail only reads byte 0 of this flag (lbz @sda21 in func_804DA4CC), so the
// symbol is declared/defined as u8; the remaining 7 bytes of the retail
// .sbss block are kept as an explicit pad so lbl_eu_806659E0 stays at +0x10.
// One 8-byte symbol: retail sizes lbl_eu_806659D8 as 0x8, and a separate
// pad[7] gets 4-aligned by MWCC (pushing lbl_eu_806659E0 to +0x14 -> .sbss
// 0x1C). Only byte 0 is ever read (lbz @sda21).
u8 lbl_eu_806659D8[8];
CNReqtaskSaveVtbl* lbl_eu_806659E0;
s8 lbl_eu_806659E4;    // retail 1B at +0x14 (signed: read back via extsb.)
