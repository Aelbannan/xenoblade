// Decompiled: monolib/src/nand/CNand
//
// CNand is a NAND request manager. It owns a ring buffer of 8 CNRequest
// objects (0x24 bytes each, offsets 0x000..0x11F) plus bookkeeping fields:
//   +0x120 s16  mHead    -- consume index (ring head)
//   +0x122 s16  mTail    -- produce index (ring tail)
//   +0x124 u8   mStatus  -- last completion status byte
//   +0x125 u8   mFlag    -- async/enable flag forwarded to request setup
//   +0x128 void*mTable   -- pointer to the shared callback table (lbl_eu_8056FD48)
//
// The constructor/destructor and the request-enqueue helpers below carry
// retail placeholder symbol names (e.g. __ct__CNand, func_804DA248) that are
// not standard MWCC manglings, so they are emitted as extern "C" stand-alone
// functions -- the same fragment-anchored pattern used elsewhere in monolib
// (see scn/code_804BF59C.cpp). CNand/CNRequest are plain data layouts; the
// extern "C" __dt__* functions play the role of the deleting destructors.

#include <types.h>
#include <decomp.h>
#include <PowerPC_EABI_Support/Runtime/MWCPlusLib.h>

struct CNRequest {
    void* mTask;        // 0x00: active task vtable pointer (NULL when idle)
    u8 mData[0x20];     // 0x04..0x23: task-specific parameter block
};

struct CNand {
    CNRequest mReq[8];  // 0x000..0x11F
    s16 mHead;          // 0x120
    s16 mTail;          // 0x122
    u8 mStatus;         // 0x124
    u8 mFlag;           // 0x125
    void* mTable;       // 0x128
};

// --- runtime / external symbols -------------------------------------------
extern "C" {
void __dl__FPv(void*);

// Shared data objects (retail linker names).
extern u8 lbl_eu_8056FD48[];   // .data 0x10  callback table stored in mTable
extern u8 lbl_eu_805245B0[];   // .rodata 0x11 path/string buffer
extern u8 lbl_eu_8065FD00[];   // .bss 0x130  the CNand singleton

// CNRequest constructor callback, defined in the CNReqtaskSave unit.
void __ct__CNRequest(void* self);

// CNRequest task-setup helpers, defined in the CNReqtask* units. Each stores a
// task vtable into the request and returns TRUE on success.
int func_804DAAF8(CNRequest* req, u8* statusOut);
int func_804DAB80(CNRequest* req, u32 a1, u32 a2, u32 a3);
int func_804DABBC(CNRequest* req, u32 a1, u32 a2, u32 a3, u32 a5, u8 flag);
int func_804DABF8(CNRequest* req, u32 a1, u32 a2, u32 a3, u8 flag);
int func_804DAC34(CNRequest* req, u32 a1, u8 flag);
int func_804DAC70(CNRequest* req, u32 a1, u32 a2, u32 a3, u8 flag);
int func_804DACAC(CNRequest* req, u32 a1, u32 a2);
int func_eu_804DEF20(CNRequest* req, void* buf, u32 size, u32 a3);
}

// --- destructors ----------------------------------------------------------

// CNRequest deleting destructor (__dt__9CNRequestFv).
extern "C" void* __dt__9CNRequestFv(CNRequest* self, int deleting) {
    if (self != nullptr) {
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// CNand deleting destructor (__dt__5CNandFv): destroys the 8-request array
// then frees the object when the deleting flag is set.
extern "C" void* __dt__5CNandFv(CNand* self, int deleting) {
    if (self != nullptr) {
        __destroy_arr(self, reinterpret_cast<ConstructorDestructor*>(&__dt__9CNRequestFv), 0x24, 8);
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// --- constructor ----------------------------------------------------------

// CNand constructor (__ct__CNand).
extern "C" DECOMP_DONT_INLINE CNand* __ct__CNand(CNand* self) {
    self->mTable = lbl_eu_8056FD48;
    __construct_array(self,
                      reinterpret_cast<ConstructorDestructor>(&__ct__CNRequest),
                      reinterpret_cast<ConstructorDestructor>(&__dt__9CNRequestFv),
                      0x24, 8);
    self->mHead = 0;
    self->mTail = 0;
    self->mFlag = 0;
    return self;
}

// --- helpers --------------------------------------------------------------

// Reset the ring head/tail and mark the manager ready (func_804DA1B4).
extern "C" void func_804DA1B4(CNand* self) {
    self->mHead = 0;
    self->mTail = 0;
    self->mStatus = 1;
}

// Ring buffer producer: claim the next free request slot (func_804DA47C).
// Returns NULL when advancing the tail would collide with the head (full).
extern "C" DECOMP_DONT_INLINE CNRequest* func_804DA47C(CNand* self) {
    s16 next = (s16)((self->mTail + 1) % 8);
    if (next == self->mHead) {
        return nullptr;
    }
    CNRequest* req = &self->mReq[self->mTail];
    self->mTail = next;
    return req;
}

// Completion pump (func_804DA1CC): while the head request is active, poll it;
// on success record its status byte and advance the head, else stop.
extern "C" void func_804DA1CC(CNand* self) {
    u8 status;
    while (self->mReq[self->mHead].mTask != nullptr) {
        if (func_804DAAF8(&self->mReq[self->mHead], &status) == 0) {
            break;
        }
        self->mStatus = status;
        self->mHead = (s16)((self->mHead + 1) % 8);
    }
}

// Enqueue a check request (func_804DA248).
extern "C" int func_804DA248(CNand* self, u32 a1, u32 a2, u32 a3) {
    CNRequest* req = func_804DA47C(self);
    if (req == nullptr) {
        return 0;
    }
    return func_804DAB80(req, a1, a2, a3);
}

// Enqueue a save request, optionally preceded by a directory-create request
// and followed by a flush request (func_804DA29C).
extern "C" int func_804DA29C(CNand* self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5) {
    if (self->mFlag != 0) {
        CNRequest* req = func_804DA47C(self);
        if (req == nullptr) {
            return 0;
        }
        if (func_eu_804DEF20(req, lbl_eu_805245B0, 0x34, 0) == 0) {
            return 0;
        }
    }

    CNRequest* req2 = func_804DA47C(self);
    if (req2 == nullptr) {
        return 0;
    }
    if (func_804DABBC(req2, a1, a2, a3, a5, self->mFlag) == 0) {
        return 0;
    }

    if (a4 != 0) {
        CNRequest* req3 = func_804DA47C(self);
        if (req3 == nullptr) {
            return 0;
        }
        if (func_804DACAC(req3, a4, 0) == 0) {
            return 0;
        }
    }

    return 1;
}

// Enqueue a load request (func_804DA34C).
extern "C" int func_804DA34C(CNand* self, u32 a1, u32 a2, u32 a3) {
    CNRequest* req = func_804DA47C(self);
    if (req == nullptr) {
        return 0;
    }
    return func_804DABF8(req, a1, a2, a3, self->mFlag);
}

// Enqueue a remove request (func_804DA3A0).
extern "C" int func_804DA3A0(CNand* self, u32 a1) {
    CNRequest* req = func_804DA47C(self);
    if (req == nullptr) {
        return 0;
    }
    return func_804DAC34(req, a1, self->mFlag);
}

// Enqueue a readdir request (func_804DA3E4).
extern "C" int func_804DA3E4(CNand* self, u32 a1, u32 a2, u32 a3) {
    CNRequest* req = func_804DA47C(self);
    if (req == nullptr) {
        return 0;
    }
    return func_804DAC70(req, a1, a2, a3, self->mFlag);
}

// Enqueue a flush request (func_804DA438).
extern "C" int func_804DA438(CNand* self, u32 a1) {
    CNRequest* req = func_804DA47C(self);
    if (req == nullptr) {
        return 0;
    }
    return func_804DACAC(req, a1, 0);
}

// Enqueue a banner/load request against the shared path buffer (func_eu_804DE660).
extern "C" int func_eu_804DE660(CNand* self, u32 a1, u32 a2) {
    CNRequest* req = func_804DA47C(self);
    if (req == nullptr) {
        return 0;
    }
    return func_804DABF8(req, (u32)(lbl_eu_805245B0 + 6), a1, a2, 0);
}

// --- static initializer ---------------------------------------------------

// .ctors hook: construct the CNand singleton (sinit_804DA4C0).
extern "C" CNand* sinit_804DA4C0(void) {
    return __ct__CNand(reinterpret_cast<CNand*>(lbl_eu_8065FD00));
}
