// Decompiled: CNReqtaskSaveBanner
//
// Translation-unit final form.
//
// CNReqtaskSaveBanner is a NAND banner-save sub-task embedded in a CNRequest
// (at offset 0). It runs an async state machine that downloads a banner from
// the save-channel streaming buffer, writes it into the NAND save block, and
// cleans up. Like the other monolib NAND sub-task units, the retail object's
// function/data symbols are stripped placeholder names (func_8..., sinit_8...)
// and the data items are referenced by absolute linker labels (lbl_8...), so we
// keep C linkage for exactly those names (exempt in PLAN.md 17.6) to keep the
// emitted object symbols/labels byte-identical to retail. Everything that is
// meaningful C++ (the task data layout) is expressed as a real struct.
#include <types.h>
#include <string.h>
#include <monolib/util/FixStr.hpp>

// The NAND engine's object layouts are not part of this TU and are not
// reconstructed here; these opaque types give the passing pointers a proper
// (non-void) class type without inventing field layouts. They are only ever
// cast/passed through as opaque handles, so no sizeof/field access is emitted.
struct CNReqtaskSaveBannerData;          // sub-task parameter block (defined below)
struct CNReqtaskSaveBannerTarget;        // banner-storage data object (defined below)
struct CNReqtaskSaveBannerVtbl;          // task vtable (installed into lbl_eu_80665A90)
struct CNandBlock;                       // dynamic banner data block
struct CNandStreamBuf;                   // NAND streaming buffer context
struct CNandPath;                        // banner path string (ml::FixStr<32>)
struct CNandTask {                       // NAND task/alloc block
    CNandBlock* field_0;                 // +0x00: pointer to the data block being released
};

// Retail linker names referenced by this unit (C linkage so the emitted
// symbols match the stripped retail names rather than C++ manglings).
extern "C" {
    CNReqtaskSaveBannerVtbl* lbl_eu_80665A90;  // installed task vtable pointer
    extern unsigned char lbl_eu_80570358[];    // vtable data - array type prevents sda21
    u8  lbl_eu_806659D0;        // global NAND "busy" flag
    s32 lbl_eu_806659D4;        // global NAND result/error latch
    u8  lbl_eu_80665A94;        // banner-path string cache init flag
    s32 lbl_eu_80663CD0;        // NAND banner meta: offset
    s32 lbl_eu_80663CD4;        // NAND banner meta: length
    ml::FixStr<32> lbl_eu_80661850;  // banner path string cache
    extern const char lbl_eu_805248A8[];  // path format string

    u32* func_804DA98C(u8 arg);         // NAND banner-id primitive
    s32 func_804DA91C(u32* ptr);        // NAND open-banner primitive
    s32 func_804DA540(CNandStreamBuf* ptr, u32 arg);  // NAND set-buffer primitive
    s32 func_804DA82C(CNandStreamBuf* data);          // NAND read primitive
    s32 func_804DA69C(void);            // NAND close primitive
    CNandPath* func_804F50D0(CNReqtaskSaveBannerData* data);  // build the banner path string (in this file)
    s32 func_804DA70C(CNandPath* data, u32 arg1, u32 arg2);   // NAND write primitive
    s32 func_804F53DC(CNReqtaskSaveBannerTarget* ptr);        // NAND prim-task finish helper
    void func_804DA4CC(CNandTask* data, CNandTask* dealloc);  // NAND dealloc helper
    s32 func_804DA628(u32 addr, u32 size);  // NAND buffer commit primitive
    s32 func_804DA7CC(CNandPath* data, u32* bannerPath);      // NAND banner move primitive
    void __dt__804F5738(CNandBlock* ptr);   // task-block destructor
    void func_804F5080(CNandTask* data, CNandTask* dealloc);  // releases the banner data block
}

// CNReqtaskSaveBanner sub-task parameter block (embedded in CNRequest at 0):
//   +0x00: f0   -- pointer to the NAND data object (unk324/unk325 flags)
//   +0x04: u8   state -- async state machine step (0..0xB)
//   +0x08: u32  f8    -- streaming buffer context handed to the NAND read
//   +0x0C: u8   fC    -- banner id (index) used to build the target path
struct CNReqtaskSaveBannerData {
    CNReqtaskSaveBannerTarget* f0;  // banner-storage data object (unk324/unk325)
    u8 state;
    u8 pad5[3];
    u32 f8;
    u8 fC;
};

// us-804f92d8: func_804F4D7C
// Configures the CNReqtaskSaveBanner sub-task: records the NAND data object and
// banner id, resets the async state to step 0, then returns the task vtable
// pointer.
CNReqtaskSaveBannerVtbl** func_804F4D7C(CNReqtaskSaveBannerData* data, CNReqtaskSaveBannerTarget* arg1, u8 arg2) {
    data->f0 = arg1;
    data->fC = arg2;
    data->state = 0;
    return &lbl_eu_80665A90;
}

// us-804f9638: func_804F50D0
// Lazily initialises the banner-path string cache (once), then formats the
// current NAND banner meta (offset/length) into it and returns the path.
CNandPath* func_804F50D0(CNReqtaskSaveBannerData* data) {
    if ((s8)lbl_eu_80665A94 == 0) {
        lbl_eu_80661850.clear();  // zero mString[0] and mLength (offset 0x20)
        lbl_eu_80665A94 = 1;
    }
    lbl_eu_80661850.format(lbl_eu_805248A8, lbl_eu_80663CD0, lbl_eu_80663CD4);
    return (CNandPath*)&lbl_eu_80661850;
}

// us-804f95e8: func_804F5080
// Releases the dynamically-sizeable banner data block (destructor + dealloc)
// before handing the task block back to the NAND allocator.
void func_804F5080(CNandTask* data, CNandTask* dealloc) {
    if (dealloc != 0) {
        CNandBlock* block = dealloc->field_0;
        if (block != 0) {
            __dt__804F5738(block);
        }
    }
    func_804DA4CC(data, dealloc);
}

// The banner-storage data object pointed to by d->f0. Only its last
// two flag bytes are sampled by the state machine:
//   +0x324: u8 unk324 -- busy/in-use flag (write pending)
//   +0x325: u8 unk325 -- presence flag (banner present in the block)
struct CNReqtaskSaveBannerTarget {
    u8 pad[0x324];
    u8 unk324;
    u8 unk325;
};

// us-804f92f0: func_804F4D90
// Async NAND banner-save state machine, polled by the CNand completion pump.
// Advancing one step per call; returns 1 when fully saved, 2 on error, 0
// while still in progress. Steps:
//   0  -> resolve/open the target banner id (func_804DA98C + func_804DA91C)
//   1  -> set the NAND write buffer (func_804DA540)
//   2  -> read the streaming banner data in (func_804DA82C)
//   3  -> wait for that read to finish (func_804DA69C puff); branch on buffer
//   4  -> done (return 1)
//   5  -> build the save path and write it to NAND (func_804DA70C)
//   6  -> wait for the write task to complete (func_804F53DC)
//   7  -> rewrite banner via a second set-buffer pass (func_804DA540)
//   8  -> commit the header/buffer to NAND (func_804DA628)
//   9  -> wait for the commit to finish (func_804DA69C)
//   0xA -> destroy the in-progress block, move the banner into place
//          (func_804DA7CC)
//   0xB -> done (return 1)
s32 func_804F4D90(void* vtable_ptr, void* data) {
    CNReqtaskSaveBannerData* d = (CNReqtaskSaveBannerData*)data;
    CNReqtaskSaveBannerTarget* t = (CNReqtaskSaveBannerTarget*)d->f0;
    if (t->unk324 != 0) {
        return 0;
    }
    if (lbl_eu_806659D0 != 0) {
        return 0;
    }

    if ((s8)d->state > 0) {
        if ((s8)d->state == 2) {
            if (lbl_eu_806659D4 == -12) {
                d->state = 5;
            } else if (lbl_eu_806659D4 == -15 || lbl_eu_806659D4 == -5) {
                d->state = 5;
            } else if (lbl_eu_806659D4 < 0) {
                return 2;
            }
        } else {
            if ((s8)d->state == 7) {
                if (t->unk325 != 0) return 2;
            } else {
                if (lbl_eu_806659D4 < 0) return 2;
            }
        }
    }

    switch ((s8)d->state) {
        s32 r;
        void* ctx;
        void* p;
        u32 addr;
        u32 size;
        u32* bannerPath;

        case 0:
            bannerPath = func_804DA98C(d->fC);
            if (bannerPath == 0) return 2;
            r = func_804DA91C(bannerPath);
            if (r != 0) return 2;
            d->state = 1;
            break;

        case 1:
            r = func_804DA540((CNandStreamBuf*)lbl_eu_80663CD4, 1);
            if (r != 0) return 2;
            d->state = 2;
            break;

        case 2:
            ctx = &d->f8;
            r = func_804DA82C((CNandStreamBuf*)ctx);
            if (r != 0) return 2;
            d->state = 3;
            break;

        case 3:
            r = func_804DA69C();
            if (r != 0) return 2;
            if (d->f8 == 0) {
                d->state = 5;
            } else {
                d->state = 4;
            }
            break;

        case 4:
            return 1;

        case 5: {
            CNandPath* pathStr = func_804F50D0((CNReqtaskSaveBannerData*)data);
            r = func_804DA70C(pathStr, 0x34, 0);
            if (r != 0 && r != -6) return 2;
            d->state = 6;
            break;
        }

        case 6:
            r = func_804F53DC(d->f0);
            if (r == 0) return 2;
            d->state = 7;
            break;

        case 7: {
            CNandPath* pathStr = func_804F50D0((CNReqtaskSaveBannerData*)data);
            r = func_804DA540((CNandStreamBuf*)pathStr, 2);
            if (r != 0) return 2;
            d->state = 8;
            break;
        }

        case 8: {
            struct BannerBuf { u32 pad0; u32 f4; u32 f8; }* b = (struct BannerBuf*)d->f0;
            addr = b->f4;
            size = b->f8;
            r = func_804DA628(addr, size);
            if (r != 0) return 2;
            d->state = 9;
            break;
        }

        case 9:
            r = func_804DA69C();
            if (r != 0) return 2;
            d->state = 0xA;
            break;

        case 0xA: {
            p = d->f0;
            __dt__804F5738((CNandBlock*)p);
            bannerPath = func_804DA98C(d->fC);
            CNandPath* pathStr = func_804F50D0((CNReqtaskSaveBannerData*)data);
            r = func_804DA7CC(pathStr, bannerPath);
            if (r != 0) return 2;
            d->state = 0xB;
            break;
        }

        case 0xB:
            d->state = 4;
            break;

        default:
            break;
    }
    return 0;
}

// us-804f96a8: sinit_804F5140
// .ctors static initializer: installs the CNReqtaskSaveBanner vtable
// (lbl_eu_80570358) into the vtable pointer (lbl_eu_80665A90).
//
// KNOWN CEILING (MWCC_REFERENCE "b .+4 sinit barrier"): retail emits a 24-byte
// `li r3,dest@sda21; b .+4 (scheduler barrier); lis/addi src; stw r4,0(r3); blr`
// shape that is not reproducible from high-level C: MWCC always folds the
// store to `stw rX,dest@sda21(r0)` and never emits the `b .+4`. This is the
// documented readable 20-byte folded-store endpoint shared by the monolib NAND
// sinits. Returning p keeps &lbl_eu_80665A90 live in r3 (closest match). No
// assembly is added per policy.
CNReqtaskSaveBannerVtbl** sinit_804F5140() {
    CNReqtaskSaveBannerVtbl** p = &lbl_eu_80665A90;
    CNReqtaskSaveBannerVtbl* v = (CNReqtaskSaveBannerVtbl*)lbl_eu_80570358;
    *p = v;
    return p;
}
