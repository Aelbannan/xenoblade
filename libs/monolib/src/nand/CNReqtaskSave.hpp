#pragma once

#include <types.h>

// CNandTask: the active sub-task object installed at CNRequest::field_0x0.
// Only vtable slots 2 and 3 are exercised by the retail code in this unit
// (func_804DAAF8): with -RTTI on, MWCC places two RTTI entries at vtable
// offsets +0/+4, so the first declared virtual lands at +8 and the second at
// +0xC - exactly the retail offsets. The class is never constructed in this
// TU, so MWCC emits no vtable for it.
struct CNandTask {
    virtual int taskSlot2(u8* data);  // vtable[2] (+8): poll one step; 0=running, 1=done, 2=error
    virtual int taskSlot3(u8* data);  // vtable[3] (+0xC): finalize after completion
};

// Forward declarations of opaque task types owned by sibling units.
struct CNReqtaskCheckVtbl; // CNReqtaskCheck sub-task vtable (opaque)
struct CNReqtaskSaveVtbl;  // CNReqtaskSave sub-task vtable (opaque)
struct CNReqtaskLoadVtbl;          // CNReqtaskLoad sub-task vtable (opaque)
struct CNReqtaskRemoveVtbl;        // CNReqtaskRemove sub-task vtable (opaque)
struct CNReqtaskReaddirVtbl;       // CNReqtaskReaddir sub-task vtable (opaque)
struct CNReqtaskReaddirData;       // CNReqtaskReaddir sub-task parameter block (opaque)
struct CNReqtaskSaveBannerVtbl;    // CNReqtaskSaveBanner sub-task vtable (opaque)
struct CNReqtaskSaveBannerData;    // CNReqtaskSaveBanner sub-task parameter block (opaque)
struct CNReqtaskSaveBannerTarget;  // banner-storage data object (opaque)
struct MonoRequestState;           // request record (monolib core)

// CNReqtaskCheck sub-task parameter block (mirrors the layout reconstructed in
// CNReqtaskCheck.cpp): +0x00..+0x08 check arguments, +0x0C async state.
struct CNReqtaskCheckData {
    u32 field_0x0; // +0x00
    u32 field_0x4; // +0x04
    u32 field_0x8; // +0x08
    u8  state;     // +0x0C
};

// CNReqtaskSave sub-task parameter block (see func_804DACE8): +0x00 NAND path,
// +0x10/+0x14 write arguments, +0x18/+0x19 flags, +0x1A async state.
struct CNReqtaskSaveData {
    char path[0x10]; // +0x00
    u32  field_0x10; // +0x10
    u32  field_0x14; // +0x14
    u8   field_0x18; // +0x18
    u8   field_0x19; // +0x19
    u8   state;      // +0x1A
};

// Base task object for the monolib NAND sub-task machinery. The sub-task
// parameter block (0x1C bytes) lives at +0x04; each sub-task unit interprets
// it as its own data layout. The class vtable slot lives at +0x20 and is
// installed by the constructor.
class CNRequest {
public:
    CNRequest();

    CNandTask* field_0x0; // +0x00: active sub-task object (vtable slot address)
    u8 field_0x4[0x1C];   // +0x04: sub-task parameter block
};

// ---- Shared NAND async-request state --------------------------------------
// Owned by monolib_eu_804F9E98 (sbss) / the retail .bss image; the five async
// wrappers in CNReqtaskSave.cpp drive them around each NAND*Async launch.
// MWCC does not mangle global-scope variable names, so plain externs emit the
// retail linker symbols (no extern "C" needed).
extern u8   lbl_eu_806659D0;   // NAND subsystem "busy" flag (1 while an op is in flight)
extern s32  lbl_eu_806659D4;   // last NAND result / callback argument latch

struct NANDCommandBlock;      // revolution/nand/nand.h (opaque here)
extern NANDCommandBlock lbl_eu_8065FE30; // shared async command block (.bss)
struct NANDFileInfo;          // revolution/nand/nand.h (opaque here)
extern NANDFileInfo lbl_eu_8065FEEC;    // fixed save file-info used by the NAND write wrapper (func_804DA628)
