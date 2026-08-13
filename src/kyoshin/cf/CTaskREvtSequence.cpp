// Auto-scaffolded catalog TU for kyoshin/cf/CTaskREvtSequence
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <string.h>

// CTaskCulling::func_801A2BD0 (static) is called by Term. Note: this TU does
// NOT include harness_catalog.hpp (which would pull CTaskGameEff.hpp's
// minimal `class CScn` and collide with the real monolib CScn below).
#include "kyoshin/cf/CTaskCulling.hpp"

#include "kyoshin/cf/CTaskREvtSequence.hpp"
// CTaskGameEffAfter.hpp pre-declares the retail __ptmf_test runtime helper
// with C linkage (the out-of-line Move specialization below dispatches its
// callback through it).
#include "kyoshin/cf/CTaskGameEffAfter.hpp"
// CfGameManager::func_80086B5C (static) is called by func_8016841C.
#include "kyoshin/cf/CfGameManager.hpp"
// CDeviceFile::readFile (static) is called by func_801686B0.
#include "monolib/device/CDeviceFile.hpp"
// CDeviceVI::getTargetFramerate (static) is called by func_80168F38.
#include "monolib/device/CDeviceVI.hpp"
// CDeviceSC::getLanguage (static) is called by func_8016ABA8.
#include "monolib/device/CDeviceSC.hpp"
// CPathUtil::getFilePtrFromPath (static) is called by func_8016C450.
#include "monolib/util/CPathUtil.hpp"
// CFileHandle::getData (inline) is used by func_8016ABA8.
#include "monolib/device/CFileHandle.hpp"
// ml::CCol4::white/black are passed to func_8049602C by func_80169050.
#include "monolib/math/CCol4.hpp"
// nw4r g3d resource walk used by func_8016AF4C / func_8016B5A4.
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resdict.h"
#include "nw4r/db/db_assert.h"
// CX streaming-uncompression context (func_8016ABA8 / func_80169050).
#include "revolution/cx/CXStreamingUncompression.h"
extern "C" void __dt__Q22cf17CTaskREvtSequenceFv(void*, int);
extern "C" void cbRenderBefore__Q22cf17CTaskREvtSequenceFv(void*);

namespace cf {
    u32 CTaskREvtSequence::func_8016A354() { return field_0x20; }

    bool CTaskREvtSequence::OnFileEvent(CEventFile* ev) {
        // IWorkEvent dispatch thunk: the IWorkEvent subobject sits at +0x54 of
        // the cf::CTaskREvtSequence; adjust and forward to func_8016ABA8.
        return func_8016ABA8(
            reinterpret_cast<CTaskREvtSequence*>(
                reinterpret_cast<u8*>(this) - 0x54),
            reinterpret_cast<EvtSeqFileEvent*>(ev));
    }
}

// Retail ctor __ct__cf_CTaskREvtSequence(self, const char* name) strlens +
// strcpys the sequence name into +0x60, publishes the global instance
// pointer, and arms the event-sequence flag bits from the name substrings.
// The retail symbol is the pre-mangled global name (not a member), so this is
// a C-linkage free ctor (CTaskREvent precedent).
__declspec(noinline) cf::CTaskREvtSequence* __ct__cf_CTaskREvtSequence(
    cf::CTaskREvtSequence* self, const char* name) {
    // CProcess base ctor, then the interim CTTask vtable at +0x10
    // (overwritten below), then the null PTMF members at +0x3C/+0x48 in the
    // retail store order 0x40,0x3C,0x44 then 0x4C,0x48,0x50.
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(self));
    self->field_0x10 = reinterpret_cast<u32>(lbl_eu_80530BF4);
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    self->field_0x40 = w1;
    self->field_0x3C = w0;
    u32 w2 = *src++;
    self->field_0x44 = w2;
    src = __ptmf_null;
    w0 = *src++;
    w1 = *src++;
    self->field_0x4C = w1;
    self->field_0x48 = w0;
    w2 = *src++;
    self->field_0x50 = w2;
    // Final vtable block: primary at +0x10, secondary sub-vtables at +0x24
    // and +0xAC into the retail vtable block.
    self->field_0x10 = reinterpret_cast<u32>(lbl_eu_80530B0C);
    self->mEvt54[0] = reinterpret_cast<u32>(lbl_eu_80530B0C + 0x24 / 4);
    self->mEvt54[1] = reinterpret_cast<u32>(lbl_eu_80530B0C + 0xAC / 4);
    self->field_0x5C = 0;
    self->field_0xA0 = strlen(name);
    strcpy(self->mPath, name);
    // Field block: zero words, the armed byte at 0x109, the ALLOC_HANDLE
    // sentinel at 0xB0, the fade volume and the sequence-id flag word.
    f32 f0 = lbl_eu_80667658;
    self->field_0xA4 = 0;
    self->field_0xA8 = 0;
    self->field_0xB0 = 0xFFFFFFFF;
    self->field_0xB4 = 0;
    self->field_0xB8 = 0;
    self->field_0xBC = 0;
    self->field_0xC0 = 0;
    self->field_0xC4 = 0;
    self->field_0xC8 = 0;
    self->field_0xCC = 0;
    self->field_0xD0 = 0;
    self->field_0xD4 = 0;
    self->field_0xD8 = 0;
    self->field_0xDC = 0;
    self->field_0xE0 = 0;
    self->field_0xE4 = 0;
    self->field_0xE8 = 0;
    self->field_0xEC = 0;
    self->field_0xF0 = 0;
    self->field_0xF4 = 0;
    self->field_0xF8 = 0;
    self->field_0xFC = 0;
    self->field_0x100 = 0;
    self->field_0x104 = 0;
    self->field_0x108 = 0;
    self->field_0x109 = 1;
    self->field_0x10C = 0;
    self->field_0x110 = 0;
    self->field_0x114 = 0;
    self->field_0x115 = 0;
    self->field_0x116 = 0;
    self->field_0x118 = 0;
    self->field_0x11C = 0;
    self->field_0x120 = 0;
    self->field_0x124 = 0;
    self->field_0x128 = 0;
    self->field_0x12C = f0;
    self->field_0x130 = 1;
    self->field_0x134 = 0;
    self->field_0x138 = 0;
    lbl_eu_80664268 = reinterpret_cast<UnkState_80664268*>(self);
    memset(self->mBuf13C, 0, 0x100);
    self->field_0x23C = 0;
    self->field_0x240 = 0;
    self->field_0x244 = 0;
    self->field_0x248 = 0;
    self->field_0x24C = 0;
    // Name-substring gates: the 0x80503098 pool, a second (menu) marker at
    // +0xB, and a third at +0x16 arm the 0x1000 / 0xA000 flag bits.
    if (strstr(name, lbl_eu_80503098) != 0) {
        self->field_0x5C |= 0x1000;
    }
    if (func_80110A70() != 0 && func_80164910() == 0 &&
        strstr(name, &lbl_eu_80503098[0xB]) != 0) {
        func_80110A70();
        func_80111074();
    }
    if (strstr(name, &lbl_eu_80503098[0x16]) != 0) {
        self->field_0x5C |= 0xA000;
    }
    return self;
}

// Retail dtor __dt__801682AC: release the data buffer (unless the alloc
// handle is invalid), reset the block, then free the block when the delete
// flag is set. Returns self.
void* __dt__801682AC(UnkBlock801682AC* self, int flag) {
    if (self != 0) {
        self->field_0x4 = 0;
        if (self->field_0xC != 0xFFFFFFFF) {
            if (self->field_0x0 != 0) {
                mtl::MemManager::deallocate(self->field_0x0);
                self->field_0x0 = 0;
            }
        }
        self->field_0x0 = 0;
        self->field_0x8 = 0;
        self->field_0xC = 0xFFFFFFFF;
        if (flag > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

cf::CTaskREvtSequence::~CTaskREvtSequence() {
    // MWCC emits the null-check and delete-flag epilogue itself. Clear the
    // shared event-list pointer, then destruct the embedded memory block at
    // +0xA4 (null-guarded member pattern: addic. + beq; release the buffer
    // unless the alloc handle is invalid), then the CProcess base.
    lbl_eu_80664268 = 0;
    if (reinterpret_cast<u8*>(this) + 0xA4 != 0) {
        field_0xA8 = 0;
        if (field_0xB0 != 0xFFFFFFFF) {
            if (field_0xA4 != 0) {
                mtl::MemManager::deallocate(
                    reinterpret_cast<void*>(field_0xA4));
                field_0xA4 = 0;
            }
        }
        field_0xA4 = 0;
        field_0xAC = 0;
        field_0xB0 = 0xFFFFFFFF;
    }
    // Redundant re-check reproduces the retail's dead second beq before the
    // CProcess base dtor call.
    if (this != 0) {
        __dt__8CProcessFv(reinterpret_cast<CProcess*>(this), 0);
    }
}

extern "C" unsigned long func_801683FC() {
    unsigned long* ptr = (unsigned long*)lbl_eu_80664268;
    return (ptr[0x5c/4] >> 14) & 1;
}

extern "C" unsigned long func_8016840C() {
    unsigned long* ptr = (unsigned long*)lbl_eu_80664268;
    return (ptr[0x5c/4] >> 12) & 1;
}

void func_8016841C() {
    // Publish the halfword pair (field_0x116/0x118) to CfGameManager, then to
    // the global-sda consumer; the retail re-reads lbl_eu_80664268 between
    // the two calls (the first call may update it).
    func_80086B5C__Q22cf13CfGameManagerFv(lbl_eu_80664268->field_0x116,
                                          lbl_eu_80664268->field_0x118, 0);
    func_800599E0(getGlobalSda(), lbl_eu_80664268->field_0x116,
                  lbl_eu_80664268->field_0x118, 0);
}

extern "C" u32 func_8016846C(void) {
    void* g = (void*)lbl_eu_80664268;
    return (*(u32*)((u8*)g + 0x5c) >> 5) & 1;
}

extern "C" u32 func_8016847C() { return (u32)lbl_eu_80664268; }
extern "C" void func_80168484(int a) {
    // Toggle the 0x8 / 0x100 bits of the shared state flag word, then fade
    // the BGM volume back in when the gate bit is set. The reassigned local
    // mirrors the retail second load of lbl_eu_80664268 (shared by both
    // branches of the first if).
    UnkState_80664268* p = lbl_eu_80664268;
    p->field_0x5C |= 0x8;
    p = lbl_eu_80664268;
    if (a != 0) {
        p->field_0x5C |= 0x100;
    } else {
        p->field_0x5C &= ~0x100;
    }
    if (a != 0) {
        func_80189318(0, lbl_eu_80667658);
        func_80189424(lbl_eu_80667658);
    }
}

u32 func_801684F4() {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p == 0) {
        return 0;
    }
    return (p->field_0x5C >> 10) & 1;
}

int func_80168514(UnkObj80168514* self) {
    // Event-run guard chain: flag bits 7/8 off, state byte gate + flag bit 16
    // clear, state count >= 5, id word != 0xFFFFFFFF; then compare id+1
    // against state->field_0xF8 (signed). On the high side, dispatch the
    // +0x40 table slot when the +0x38 halfword's bits 16-23 match; otherwise
    // dispatch the +0x3C slot and arm the state byte on a nonzero return.
    u32 flags = self->field_0x18;
    if (flags & 0x80) {
        return 0;
    }
    if (flags & 0x100) {
        return 0;
    }
    UnkState_80664268* st = lbl_eu_80664268;
    if (st->field_0x114 != 0 && (st->field_0x5C & 0x8000) == 0) {
        return 0;
    }
    if ((s32)st->field_0x100 < 5) {
        return 0;
    }
    if (self->field_0x1C->field_0x34 == 0xFFFFFFFF) {
        return 0;
    }
    u32 f8 = st->field_0xF8;
    if ((s32)(self->field_0x1C->field_0x34 + 1) > (s32)f8) {
        u32 h = self->field_0x1C->field_0x38;
        if (h != 0 && (s32)((h >> 8) & 0xFF) == (s32)f8) {
            self->vf_0x40();
        }
        return 0;
    }
    if (self->vf_0x3C()) {
        lbl_eu_80664268->field_0x114 = 1;
    }
    return 1;
}

extern "C" void func_80168610(cf::CTaskREvtSequence* self) {
    // Sequence boot: gate on the sequence system + UI state, publish the id
    // halfwords, install the +0x3C move-callback table, and arm the
    // event-sequence flag bits (2, then 40) once the event manager is up.
    if (func_8012E6DC() != 0) {
        return;
    }
    if (func_80062A00() == 0) {
        return;
    }
    func_800866A0__Q22cf13CfGameManagerFv();
    func_80086D98__Q22cf13CfGameManagerFv(&self->field_0x116, &self->field_0x118);
    self->field_0x134 = (u16)func_8016E08C();
    u32 w1, w0, w2;
    const u32* src = &lbl_eu_80530A40[0];
    w0 = *src++;
    w1 = *src++;
    self->field_0x40 = w1;
    self->field_0x3C = w0;
    w2 = *src++;
    self->field_0x44 = w2;
    if (func_80164910() == 0) {
        return;
    }
    lbl_eu_80663EE0 |= 0x2;
    func_800B9438((void*)4);
    lbl_eu_80663EE0 |= 0x40;
}

extern "C" void func_801686B0(cf::CTaskREvtSequence* self) {
    // Kick the UI-state flag toggle, then asynchronously read the sequence
    // file: the name lives at +0x60 and the IWorkEvent callback sub-object at
    // +0x54 (null-guarded address-of-member). Finally install the 3-word ptmf
    // table with the same store order as Init (0x40, 0x3C, 0x44).
    func_80166788();
    // Upcast to the +0x54 IWorkEvent secondary base; MWCC null-guards the
    // adjustment (retail: cmpwi/mr/beq/addi).
    IWorkEvent* evt = reinterpret_cast<EvtSeqWithEvt*>(self);
    self->field_0xB8 = (u32)CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), self->mPath, evt, 0, 0x800);
    u32 w1, w0, w2;
    const u32* src = &lbl_eu_80530A4C[0];
    w0 = *src++;
    w1 = *src++;
    self->field_0x40 = w1;
    self->field_0x3C = w0;
    w2 = *src++;
    self->field_0x44 = w2;
}

extern "C" void func_8016872C(cf::CTaskREvtSequence* self) {
    // Load the sequence file: read size/flags from the header object, size the
    // arena chunk, free the old header buffer, then async-read into the new
    // buffer and install the +0x3C move-callback table.
    if (self->field_0xB8 != 0) {
        return;
    }
    UnkFileHeader* hdr = reinterpret_cast<UnkFileHeader*>(self->field_0xC0);
    u32 size = hdr->field_0x18;
    u16 flags = hdr->field_0x46;
    self->field_0x128 = func_80167D40(flags << 11, size);
    if (self->field_0xC0 != 0) {
        mtl::MemManager::deallocate(reinterpret_cast<void*>(self->field_0xC0));
        self->field_0xC0 = 0;
    }
    self->field_0xC4 = (u32)func_80167F6C(reinterpret_cast<void*>(size), 0x20, 0);
    IWorkEvent* evt = reinterpret_cast<EvtSeqWithEvt*>(self);
    self->field_0xBC = (u32)CDeviceFile::readCommonArchiveFile(
        self->field_0xC4, self->mPath, evt, 0, (int)size);
    CDeviceFile::func_8044F154(
        reinterpret_cast<CFileHandle*>(self->field_0xBC), 3);
    u32 w1, w0, w2;
    const u32* src = &lbl_eu_80530A58[0];
    w0 = *src++;
    w1 = *src++;
    self->field_0x40 = w1;
    self->field_0x3C = w0;
    w2 = *src++;
    self->field_0x44 = w2;
}

extern "C" void func_80168800(cf::CTaskREvtSequence* self) {
    // Event-sequence setup, run once the common-archive read has landed
    // (field_0xBC idle): walk the type-tagged tables in the sequence buffer
    // and publish each matched entry address into a scratch word, allocate
    // the realtime-event arena, and build the camera/model/light/movie
    // object lists. The buffer base (field_0xC4) is re-read at every use
    // because the object ctor calls may resize the buffer.
    if (self->field_0xBC != 0) {
        return;
    }
    // Walk 1: type 1 -> field_0xCC.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 1) {
                self->field_0xCC = reinterpret_cast<u32>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    self->field_0x10C = reinterpret_cast<u32>(mtl::MemManager::allocate_head(
        mtl::MemManager::getHandleMEM2(), 0x400, 4));
    // Walk 2: type 2 -> field_0xD0.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 2) {
                self->field_0xD0 = reinterpret_cast<UnkStateTable_D0*>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    // Walk 3: type 3 -> field_0xD4.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 3) {
                self->field_0xD4 = reinterpret_cast<u32>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    // Walk 4: type 5 -> field_0xD8.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 5) {
                self->field_0xD8 = reinterpret_cast<u32>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    // Walk 5: type 4 -> field_0xDC.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 4) {
                self->field_0xDC = reinterpret_cast<u32>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    if (self->field_0xDC == 0) {
        // No camera table: arm the camera-rebuild flag and clear the
        // camera-active bit in the shared state.
        UnkState_80664268* p = lbl_eu_80664268;
        p->field_0x5C |= 0x8;
        p = lbl_eu_80664268;
        p->field_0x5C &= ~0x100;
    }
    // Walk 6: type 0xa -> field_0xE0.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 0xA) {
                self->field_0xE0 = reinterpret_cast<u32>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    // Walk 7: type 0xb -> field_0xE8.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 0xB) {
                self->field_0xE8 = reinterpret_cast<UnkE8Table*>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    // Walk 8: type 0xc -> field_0xEC.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 0xC) {
                self->field_0xEC = reinterpret_cast<u32>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    // Walk 9: type 0xe -> field_0xF0.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 0xE) {
                self->field_0xF0 = reinterpret_cast<UnkBB38Table*>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    // Walk 10: type 0xf -> field_0xF4.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 0xF) {
                self->field_0xF4 = reinterpret_cast<u32>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    // Walk 11: type 6 -> field_0xE4.
    {
        EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
        EvtSeqWalkEntry* t = reinterpret_cast<EvtSeqWalkEntry*>(
            reinterpret_cast<u8*>(wb) + wb->field_0x8);
        while (t->field_0x0 != -1) {
            if (t->field_0x0 == 6) {
                self->field_0xE4 = reinterpret_cast<u32>(t);
                break;
            }
            t = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(t) + t->field_0x4);
        }
    }
    // Count the realtime-event objects: camera + model/effect + light +
    // movie, with the light/movie counts zeroed when their tables are
    // absent.
    EvtSeqWalkBuf* wb = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
    u32 light = self->field_0xEC != 0 ? wb->field_0x34 : 0;
    u32 movie = self->field_0xF4 != 0 ? wb->field_0x48 : 0;
    u32 total = (wb->field_0x28 + (light + movie)) +
                (wb->field_0x20 + wb->field_0x24);
    u32 handle = static_cast<u32>(mtl::MemManager::getHandleMEM2());
    self->field_0xB0 = handle;
    self->field_0xA4 = reinterpret_cast<UnkEvtListEntry**>(
        mtl::MemManager::allocate_head(handle, total * 4, 4));
    self->field_0xA8 = 0;
    self->field_0xAC = total;
    // Camera objects: allocate 0x448-byte blocks, construct from the type-4
    // table entries and append to the list.
    if (self->field_0xDC != 0 &&
        reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x28 != 0) {
        u32 i = 0;
        while (i <
               reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x28) {
            EvtSeqWalkEntry* entry = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(self->field_0xDC) +
                reinterpret_cast<EvtSeqWalkEntry*>(self->field_0xDC)
                        ->field_0x4 *
                    i);
            CREvtCamera* obj = reinterpret_cast<CREvtCamera*>(
                func_80185748(reinterpret_cast<void*>(0x448)));
            if (obj != 0) {
                __ct__CREvtCamera(obj, reinterpret_cast<u32>(entry));
            }
            u32 idx = self->field_0xA8;
            self->field_0xA4[idx] = reinterpret_cast<UnkEvtListEntry*>(obj);
            self->field_0xA8 = idx + 1;
            i++;
        }
    }
    // Type-3 (model/effect) entries: rename the known name strings and
    // resolve the entry through the model helper.
    if (self->field_0xD4 != 0 &&
        reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x20 != 0) {
        u32 i = 0;
        while (i <
               reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x20) {
            EvtSeqWalkEntry* entry = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(self->field_0xD4) +
                reinterpret_cast<EvtSeqWalkEntry*>(self->field_0xD4)
                        ->field_0x4 *
                    i);
            if (func_80087250__Q22cf13CfGameManagerFv() != 0) {
                char* name = reinterpret_cast<char*>(
                    reinterpret_cast<u8*>(entry) + 0x10);
                if (strcmp(name, &lbl_eu_80503098[0x21]) == 0) {
                    strcpy(name, &lbl_eu_80503098[0x2A]);
                    reinterpret_cast<EvtSeqType3Entry*>(entry)->field_0x20 =
                        0x1BE00409;
                    reinterpret_cast<EvtSeqType3Entry*>(entry)->field_0x28 = 3;
                } else if (strcmp(name, &lbl_eu_80503098[0x33]) == 0) {
                    strcpy(name, &lbl_eu_80503098[0x3C]);
                    reinterpret_cast<EvtSeqType3Entry*>(entry)->field_0x20 =
                        0x1C900409;
                    reinterpret_cast<EvtSeqType3Entry*>(entry)->field_0x28 = 3;
                }
            }
            u32 idx = self->field_0xA8;
            self->field_0xA4[idx] = reinterpret_cast<UnkEvtListEntry*>(
                func_801730D0(entry));
            self->field_0xA8 = idx + 1;
            i++;
        }
    }
    // Effect objects (0x144-byte blocks, type-5 table entries).
    if (self->field_0xD8 != 0 &&
        reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x24 != 0) {
        u32 i = 0;
        while (i <
               reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x24) {
            EvtSeqWalkEntry* entry = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(self->field_0xD8) +
                reinterpret_cast<EvtSeqWalkEntry*>(self->field_0xD8)
                        ->field_0x4 *
                    i);
            CREvtEffect* obj = reinterpret_cast<CREvtEffect*>(
                func_80185748(reinterpret_cast<void*>(0x144)));
            if (obj != 0) {
                __ct__CREvtEffect(obj, reinterpret_cast<CREvtParam*>(entry));
            }
            u32 idx = self->field_0xA8;
            self->field_0xA4[idx] = reinterpret_cast<UnkEvtListEntry*>(obj);
            self->field_0xA8 = idx + 1;
            i++;
        }
    }
    // Light objects (0x24-byte blocks, type-0xc table entries).
    if (self->field_0xEC != 0 &&
        reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x34 != 0) {
        u32 i = 0;
        while (i <
               reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x34) {
            EvtSeqWalkEntry* entry = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(self->field_0xEC) +
                reinterpret_cast<EvtSeqWalkEntry*>(self->field_0xEC)
                        ->field_0x4 *
                    i);
            CREvtLight* obj = reinterpret_cast<CREvtLight*>(
                func_80185748(reinterpret_cast<void*>(0x24)));
            if (obj != 0) {
                __ct__CREvtLight(obj, reinterpret_cast<u32>(entry));
            }
            u32 idx = self->field_0xA8;
            self->field_0xA4[idx] = reinterpret_cast<UnkEvtListEntry*>(obj);
            self->field_0xA8 = idx + 1;
            i++;
        }
    }
    // Movie objects (0x1c-byte blocks, type-0xf table entries).
    if (self->field_0xF4 != 0 &&
        reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x48 != 0) {
        u32 i = 0;
        while (i <
               reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x48) {
            EvtSeqWalkEntry* entry = reinterpret_cast<EvtSeqWalkEntry*>(
                reinterpret_cast<u8*>(self->field_0xF4) +
                reinterpret_cast<EvtSeqWalkEntry*>(self->field_0xF4)
                        ->field_0x4 *
                    i);
            CREvtMovie* obj = reinterpret_cast<CREvtMovie*>(
                func_80185748(reinterpret_cast<void*>(0x1C)));
            if (obj != 0) {
                __ct__CREvtMovie(obj,
                                 reinterpret_cast<CREvtMovieScript*>(entry));
            }
            u32 idx = self->field_0xA8;
            self->field_0xA4[idx] = reinterpret_cast<UnkEvtListEntry*>(obj);
            self->field_0xA8 = idx + 1;
            i++;
        }
    }
    // Reload-count / BGM flags from the buffer's flag word.
    u32 flags = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x4C;
    if (flags & 0x1C) {
        if (flags & 0x4) {
            func_8016DF4C(1);
        } else if (flags & 0x8) {
            func_8016DF4C(2);
        } else if (flags & 0x10) {
            func_8016DF4C(0);
        }
        func_8007CE94__Q22cf13CfGameManagerFv();
    }
    // Install the move callback (3-word ptmf at +0x3C) from the
    // camera-state dependent .data table.
    if (self->field_0x5C & 0x8) {
        u32 w1, w0, w2;
        const u32* src = &lbl_eu_80530A64[0];
        w0 = *src++;
        w1 = *src++;
        self->field_0x40 = w1;
        self->field_0x3C = w0;
        w2 = *src++;
        self->field_0x44 = w2;
    } else {
        u32 w1, w0, w2;
        const u32* src = &lbl_eu_80530A70[0];
        w0 = *src++;
        w1 = *src++;
        self->field_0x40 = w1;
        self->field_0x3C = w0;
        w2 = *src++;
        self->field_0x44 = w2;
    }
    if (func_8011C2E8() != 0) {
        func_8011C400();
    }
    if (self->field_0x5C & 0x2000) {
        if (func_801AC088() != 0) {
            func_801AC1F8();
        }
        if (func_80110A70() != 0) {
            func_80110A70();
            func_80111074();
        }
    }
    // Play the movie entries (list slots [total2, total2+movieCount)).
    EvtSeqWalkBuf* wb2 = reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4);
    u32 total2 = (wb2->field_0x34 + wb2->field_0x24) +
                 (wb2->field_0x28 + wb2->field_0x20);
    u32 i = total2;
    u32 scaled = total2 * 4;
    while (i <
           total2 +
               reinterpret_cast<EvtSeqWalkBuf*>(self->field_0xC4)->field_0x48) {
        UnkEvtListEntry* e = *reinterpret_cast<UnkEvtListEntry**>(
            reinterpret_cast<u8*>(self->field_0xA4) + scaled);
        func_80294980(reinterpret_cast<CREvtMovie*>(e));
        i++;
        scaled += 4;
    }
    // Restore the BGM fade volume unless the sequence already uses it.
    EvtSeqCC* cc = reinterpret_cast<EvtSeqCC*>(self->field_0xCC);
    if (lbl_eu_80667658 != cc->field_0x10) {
        func_8048EA38(cc->field_0x10);
    }
}

void func_80168F38(cf::CTaskREvtSequence* self) {
    // Walk the realtime-event list once calling vf_0x24 on every entry, then
    // (unless a 60-frame cadence expired) a second walk that aborts the whole
    // function on a vf_0x18()==0 entry; finally reset the voice manager, run
    // the func_80169DD0 advance and install the +0x3C ptmf table. `scaled` is
    // an explicit induction variable so the back-edge increments sit in
    // source order (n++, scaled+=4) like retail.
    u32 scaled = 0;
    u32 n = 0;
    while (n < self->field_0xA8) {
        UnkEvtListEntry* e =
            *(UnkEvtListEntry**)((u8*)self->field_0xA4 + scaled);
        e->vf_0x24();
        n++;
        scaled += 4;
    }
    // `flag` (retail r29) is initialized before the frame-rate call so it
    // lives across it in a callee-saved register; the second walk gets a
    // fresh counter and reuses the shared scaled.
    u32 flag = 0;
    u32 fps60 = CDeviceVI::getTargetFramerate() * 0x3c;
    self->field_0x138 = self->field_0x138 + 1;
    if (self->field_0x138 > fps60) {
        flag = 1;
    }
    if (flag == 0) {
        n = 0;
        u32 m = 0;
        scaled = 0;
        while (m < self->field_0xA8) {
            UnkEvtListEntry* e =
                *(UnkEvtListEntry**)((u8*)self->field_0xA4 + scaled);
            if (e->vf_0x18() == 0) {
                return;
            }
            m++;
            scaled += 4;
        }
    }
    func_802A1500();
    func_80169DD0(self, self->field_0xF8);
    // Install the move callback (3-word ptmf at +0x3C) from the .data table.
    // Pointer-walk (*src++) so MWCC folds the base addi into the first load
    // (retail emits lwzu + lwz / stw,stw / lwz,stw). Value locals are declared
    // before the pointer so the allocator assigns w0/w1 the low scratch regs.
    u32 w1, w0, w2;
    const u32* src = &lbl_eu_80530A7C[0];
    w0 = *src++;
    w1 = *src++;
    self->field_0x40 = w1;
    self->field_0x3C = w0;
    w2 = *src++;
    self->field_0x44 = w2;
    self->field_0x114 = 0;
}

int func_80169048(void* self) { return 1; }

void func_80169050(cf::CTaskREvtSequence* self) {
    // CX stream pump + event-state update. While the chunk-read flag (0x40)
    // is set, drain the 0x250 context in 0x2000-byte steps; on EOF clear the
    // 0x20 gate and move the walk cursor to the arena base. Then (unless the
    // 0x8 flag path already re-armed the move callback) arm the 0x20000
    // bit and, when the 0x200 gate is clear, walk the field_0xE8 table
    // arming the 0x1 bit of the event-window halfword for type-6 entries.
    if ((self->field_0x5C & 0x4) == 0) {
        return;
    }
    if (self->field_0x5C & 0x40) {
        for (;;) {
            s32 n = CXReadUncompLH(
                reinterpret_cast<CXUncompContextLH*>(self->mCxBuffer),
                reinterpret_cast<const void*>(self->field_0x124), 0x2000);
            if (n > 0) {
                self->field_0x124 += 0x2000;
            } else {
                self->field_0x5C &= ~0x20;
                self->field_0x120 =
                    reinterpret_cast<UnkSeq120*>(self->field_0x11C);
                break;
            }
        }
    }
    u32 flag1;
    if (self->field_0x5C & 0x8) {
        // Color the scene window by the buffer's type halfword, then install
        // the +0x3C move-callback table (0x80530A88).
        EvtSeqC4Buf* buf = reinterpret_cast<EvtSeqC4Buf*>(self->field_0xC4);
        s16 t = buf->field_0x44;
        if (t == 2) {
            func_8049602C(lbl_eu_80663E14, 0,
                          reinterpret_cast<EvtSeqVec4*>(&ml::CCol4::white));
        } else if (t != 3) {
            func_8049602C(lbl_eu_80663E14, 0,
                          reinterpret_cast<EvtSeqVec4*>(&ml::CCol4::black));
        }
        u32 w1, w0, w2;
        const u32* src = &lbl_eu_80530A88[0];
        w0 = *src++;
        w1 = *src++;
        self->field_0x40 = w1;
        self->field_0x3C = w0;
        w2 = *src++;
        self->field_0x44 = w2;
        flag1 = 1;
    } else {
        flag1 = 0;
    }
    if (flag1 != 0) {
        return;
    }
    self->field_0x5C |= 0x20000;
    if (self->field_0x5C & 0x200) {
        return;
    }
    u32 flag2 = 0;
    {
        u32 w1, w0, w2;
        const u32* src = &lbl_eu_80530A94[0];
        w0 = *src++;
        w1 = *src++;
        self->field_0x40 = w1;
        self->field_0x3C = w0;
        w2 = *src++;
        self->field_0x44 = w2;
    }
    if (self->field_0xE8 != 0) {
        u16 h = lbl_eu_806642E0;
        u32 j = 0;
        do {
            UnkE8Table* table = self->field_0xE8;
            UnkE8Table* entry = reinterpret_cast<UnkE8Table*>(
                reinterpret_cast<u8*>(table) + table->field_0x4 * j);
            if (entry->field_0xA == 3) {
                if (entry->field_0x8 == 0) {
                    flag2 = 1;
                }
                flag1 = 1;
            } else if (entry->field_0xA == 6) {
                h |= 1;
                lbl_eu_806642E0 = h;
            }
            j++;
        } while (j <
                 reinterpret_cast<EvtSeqC4Buf*>(self->field_0xC4)
                     ->field_0x38);
    }
    // Fade the BGM back in unless both walk flags were raised while the event
    // manager was already ready; then arm the presentation flag and rebuild
    // the voice/UI system once the manager is up.
    if (!(flag1 != 0 && flag2 != 0 && func_80164910() == 0)) {
        func_80189318(0, lbl_eu_8066765C);
        func_80189424(lbl_eu_8066765C);
        func_8007C0F8__Q22cf13CfGameManagerFv();
    }
    if (func_80164910() != 0) {
        lbl_eu_80663E28 |= 0x800000;
        func_8007FE1C__Q22cf13CfGameManagerFv(0x1000, 1);
        func_801AAC78(1);
    }
}

void func_8016925C(){}

int func_801696C4(void* self) { return 0; }

void func_801696CC(){}

void func_80169A34() {}

void func_80169A38(cf::CTaskREvtSequence* self) {
    // Main event-sequence update: run the CX pump + event dispatch
    // (func_8016B860), then (unless CfGameManager is busy) color the scene
    // window and install a move-callback table when the 0x8 flag is set,
    // otherwise advance the event walk (fade the scene, bump the counters,
    // compare the walk index against the table entry) and dispatch the
    // realtime-event list. `less` (retail r31) is the signed walk-index <
    // entry-limit comparison used twice at the end.
    func_8016B860(self);
    if (cf::CfGameManager::func_800829B8() != 0) {
        return;
    }
    u32 flag1;
    if (self->field_0x5C & 0x8) {
        EvtSeqC4Buf* buf = reinterpret_cast<EvtSeqC4Buf*>(self->field_0xC4);
        s16 t = buf->field_0x44;
        if (t == 2) {
            func_8049602C(lbl_eu_80663E14, 0,
                          reinterpret_cast<EvtSeqVec4*>(&ml::CCol4::white));
        } else if (t != 3) {
            func_8049602C(lbl_eu_80663E14, 0,
                          reinterpret_cast<EvtSeqVec4*>(&ml::CCol4::black));
        }
        u32 w1, w0, w2;
        const u32* src = &lbl_eu_80530ADC[0];
        w0 = *src++;
        flag1 = 1;
        w1 = *src++;
        self->field_0x40 = w1;
        self->field_0x3C = w0;
        w2 = *src++;
        self->field_0x44 = w2;
    } else {
        flag1 = 0;
    }
    if (flag1 != 0) {
        return;
    }
    // flag2: co-op presentation bit (1 when the shared state is absent).
    u32 flag2;
    if (lbl_eu_80664268 == 0) {
        flag2 = 1;
    } else {
        flag2 = (lbl_eu_80664268->field_0x5C >> 18) & 1;
    }
    u32 less;
    if (flag2 != 0) {
        func_80496294(lbl_eu_80663E14, lbl_eu_80667668);
        less = 0;
    } else {
        func_80496294(lbl_eu_80663E14, lbl_eu_8066766C);
        UnkStateTable_D0* d0 = self->field_0xD0;
        UnkStateTable_D0* entry = reinterpret_cast<UnkStateTable_D0*>(
            reinterpret_cast<u8*>(d0) + d0->field_0x4 * self->field_0xF8);
        if (func_80496288(lbl_eu_80663E14) > lbl_eu_80667658) {
            self->field_0x104 += 1;
            self->field_0x100 += 1;
        }
        s32 f100 = (s32)self->field_0x100;
        s32 eC = (s32)entry->field_0xC;
        less = f100 < eC;
    }
    if (less != 0) {
        u32 w1, w0, w2;
        const u32* src = &lbl_eu_80530AE8[0];
        w0 = *src++;
        w1 = *src++;
        self->field_0x40 = w1;
        self->field_0x3C = w0;
        w2 = *src++;
        self->field_0x44 = w2;
    } else {
        if ((s32)self->field_0x100 >= 1) {
            if ((self->field_0x5C & 0x400000) == 0) {
                u32 n = 0;
                u32 scaled = 0;
                while (n < self->field_0xA8) {
                    UnkEvtListEntry* e = *(UnkEvtListEntry**)(
                        (u8*)self->field_0xA4 + scaled);
                    e->vf_0x10();
                    n++;
                    scaled += 4;
                }
                self->field_0x5C |= 0x400000;
            }
        }
    }
    {
        u32 n = 0;
        u32 scaled = 0;
        while (n < self->field_0xA8) {
            UnkEvtListEntry* e = *(UnkEvtListEntry**)(
                (u8*)self->field_0xA4 + scaled);
            e->vf_0x24();
            n++;
            scaled += 4;
        }
    }
    {
        UnkStateTable_D0* d0 = self->field_0xD0;
        UnkStateTable_D0* entry = reinterpret_cast<UnkStateTable_D0*>(
            reinterpret_cast<u8*>(d0) + d0->field_0x4 * self->field_0xF8);
        if ((u32)self->field_0x100 >= (u32)(entry->field_0xC - 0xF)) {
            if ((self->field_0x5C & 0x200000) == 0) {
                EvtSeqC4Buf* buf = reinterpret_cast<EvtSeqC4Buf*>(self->field_0xC4);
                s16 t = buf->field_0x44;
                if (t == 2) {
                    func_8049602C(lbl_eu_80663E14, 0xF,
                                  reinterpret_cast<EvtSeqVec4*>(&ml::CCol4::white));
                } else if (t != 3) {
                    func_8049602C(lbl_eu_80663E14, 0xF,
                                  reinterpret_cast<EvtSeqVec4*>(&ml::CCol4::black));
                }
                self->field_0x5C |= 0x200000;
            }
        }
    }
    if (less != 0) {
        func_8016BB38(self);
    }
    self->field_0x114 = 0;
}

void func_80169CD0(cf::CTaskREvtSequence* self) {
    // Publish the id halfwords to CfGameManager and the global-sda consumer,
    // arm the reload counter, reset the scene fade value, and (once the event
    // manager is up and the presentation flag is clear) push a flat fade vec
    // into the scene. Then run the sequence teardown chain and, if the camera
    // manager holds a live player object, restore its fov/position floats.
    func_80086B5C__Q22cf13CfGameManagerFv(self->field_0x116,
                                          self->field_0x118, 0);
    func_800599E0(getGlobalSda(), self->field_0x116, self->field_0x118, 0);
    func_8016DF4C(self->field_0x134);
    EvtSeqScnView* scene = reinterpret_cast<EvtSeqScnView*>(lbl_eu_80663E14);
    scene->field_0x5C->field_0xD4 = lbl_eu_80667670;
    if (func_80164910() != 0 && (lbl_eu_80663E28 & 0x01000000) == 0) {
        EvtSeqVec4 v = { lbl_eu_80667658, lbl_eu_80667658, lbl_eu_80667658,
                         lbl_eu_80667658 };
        func_8049602C(lbl_eu_80663E14, 0xF, &v);
    }
    // Cast keeps MWCC from inlining the empty sibling stub (tiny body).
    ((void(*)(void*))func_8016A480)(self);
    func_801644B4();
    func_80164CFC();
    // The retail re-fetches the camera manager between each gate; keep the
    // calls separate so MWCC emits the four real bl's.
    CfEvtCamManager* mgr = func_800821F8__Q22cf13CfGameManagerFv();
    if (mgr != 0) {
        CfEvtCamManager* mgr2 = func_800821F8__Q22cf13CfGameManagerFv();
        if (mgr2->field_0xC != 0) {
            func_800821F8__Q22cf13CfGameManagerFv()->vf_0x3C(lbl_eu_80667674);
            CfEvtCamManager* mgr4 = func_800821F8__Q22cf13CfGameManagerFv();
            // Local keeps the player object in one register (retail reuses r3
            // across the two float stores and the func_8049EB60 call).
            UnkEvtPlayer* player = mgr4->field_0xC;
            player->field_0x1EC = lbl_eu_80667678;
            player->field_0x1F0 = lbl_eu_8066767C;
            func_8049EB60(player);
        }
    }
}

void func_80169DD0(cf::CTaskREvtSequence* self, u32 idx) {
    // Event-sequence advance: if the sequence file handle is idle, clear the
    // 0x4 flag, select the entry at `idx` in the field_0xD0 table, and either
    // advance the field_0x120 walk cursor or (re)load the sequence file into
    // the arena. `aligned` rounds the entry size up to a 0x800 boundary.
    // When `idx` is nonzero, also refresh matching realtime-event entries
    // (name-data word +0x30 == idx) via their vf_0x2C slot.
    if (self->field_0xFC != 0) {
        return;
    }
    u32 flags = self->field_0x5C & ~0x4;
    self->field_0x5C = flags;
    UnkStateTable_D0* d0 = self->field_0xD0;
    UnkStateTable_D0* entry =
        (UnkStateTable_D0*)((u8*)d0 + d0->field_0x4 * idx);
    UnkSeq120* adv = self->field_0x120;
    if (adv != 0 && adv->field_0x28 != 0) {
        self->field_0x120 = (UnkSeq120*)((u8*)adv + adv->field_0x28);
        self->field_0x5C = flags | 0x4;
    } else {
        u32 alloc = func_80168028(self->field_0x130 == 0);
        self->field_0x11C = alloc;
        s32 size = entry->field_0x10;
        s32 rem = size % 0x800;
        s32 aligned = size;
        if (rem != 0) {
            aligned = size + (0x800 - rem);
        }
        u32 endAddr = alloc + self->field_0x128 - aligned;
        IWorkEvent* evt = reinterpret_cast<EvtSeqWithEvt*>(self);
        self->field_0xFC = (u32)CDeviceFile::readCommonArchiveFile(
            endAddr, self->mPath, evt, entry->field_0x14,
            entry->field_0x10);
        CDeviceFile::func_8044F154(
            reinterpret_cast<CFileHandle*>(self->field_0xFC), 2);
    }
    if (idx != 0) {
        s32 k = 0;
        u32 scaled = 0;
        while (k < lbl_eu_80664268->field_0x110) {
            UnkEvtListEntry* e = *(UnkEvtListEntry**)(
                (u8*)lbl_eu_80664268->field_0x10C + scaled);
            if ((u32)idx == e->field_0x1C->field_0x30) {
                e->vf_0x2C();
            }
            scaled += 4;
            k++;
        }
    }
}

void func_80169F24() {}

void func_80169F28(cf::CTaskREvtSequence* self) {
    // Event-sequence dispatch. Cache the walk header's table pointers into the
    // object's scratch words (0x23C-0x24C), then walk the four id lists
    // (camera/effect/light/movie) feeding each realtime-event object: the id
    // word selects a 0x14-byte entry whose +0xC offset locates the object's
    // data inside the sequence buffer (0 when the id word is -1). `n` is the
    // accumulated list index carried across the loops; the list base is
    // re-read from the object each iteration because the setup calls may
    // resize it. Locals are declared in reverse-alloc order (k r31, off r30,
    // entry r29, walk r28, n r27, i r26, self r25) to match retail.
    u32 k = 0;                // r31: list byte index
    u32 off = 0;              // r30: id-list byte offset
    UnkStateTable_D0* entry;  // r29: selected table entry
    UnkSeq120* walk;          // r28: walk header
    u32 n = 0;                // r27: accumulated list index
    u32 i = 0;                // r26: loop 1 counter
    self->field_0x100 = 0;
    UnkStateTable_D0* table = self->field_0xD0;
    walk = self->field_0x120;
    u32 stride = table->field_0x4;
    u32 idx = self->field_0xF8;

    self->field_0x24C = reinterpret_cast<u32>(
        reinterpret_cast<u8*>(walk) + walk->field_0x14);
    self->field_0x23C = reinterpret_cast<u32>(
        reinterpret_cast<u8*>(walk) + walk->field_0x18 + 0xC);
    self->field_0x240 = reinterpret_cast<u32>(
        reinterpret_cast<u8*>(walk) + walk->field_0x1C + 0xC);
    self->field_0x244 = reinterpret_cast<u32>(
        reinterpret_cast<u8*>(walk) + walk->field_0x20 + 0xC);
    self->field_0x248 = reinterpret_cast<u32>(
        reinterpret_cast<u8*>(walk) + walk->field_0x24 + 0xC);

    entry = reinterpret_cast<UnkStateTable_D0*>(
        reinterpret_cast<u8*>(table) + stride * idx);

    // List 1 (camera): field_0x244 id list, 2-word elements. The list entry
    // is loaded after the id check; the +0x43C store re-reads it from the
    // object (retail reloads both words after the setup call).
    while (i <
           reinterpret_cast<UnkStateC4Obj*>(self->field_0xC4)->field_0x28) {
        u32 word = *(u32*)((u8*)self->field_0x244 + off);
        u32 arg;
        if (word == 0xFFFFFFFF) {
            arg = 0;
        } else {
            UnkStateEntry_E4* e2 = &reinterpret_cast<UnkStateEntry_E4*>(
                (u8*)self->field_0x24C)[word];
            arg = reinterpret_cast<u32>(reinterpret_cast<u8*>(walk) +
                                        e2->field_0x0C);
        }
        UnkEvtListEntry* e =
            *(UnkEvtListEntry**)((u8*)self->field_0xA4 + k);
        func_80180664(reinterpret_cast<CREvtCamera*>(e), (void*)arg,
                      (void*)((u8*)self->field_0x244 + off));
        if (entry->field_0xC == 0xFFFFFFFF) {
            entry->field_0xC = reinterpret_cast<EvtSeqObj43C*>(
                                   *(UnkEvtListEntry**)((u8*)self->field_0xA4 +
                                                        k))
                                   ->field_0x43C;
        }
        off += 8;
        i++;
        n++;
        k += 4;
    }

    // List 2 (effect): field_0x23C id list, 2-word elements; the dispatch is
    // a virtual call at vtable+0x34.
    {
        u32 i2 = 0;       // r31
        u32 off2 = 0;     // r30
        u32 k2 = n * 4;   // r26
        while (i2 <
               reinterpret_cast<UnkStateC4Obj*>(self->field_0xC4)->field_0x20) {
            u32 word = *(u32*)((u8*)self->field_0x23C + off2);
            u32 arg;
            if (word == 0xFFFFFFFF) {
                arg = 0;
            } else {
                UnkStateEntry_E4* e2 = &reinterpret_cast<UnkStateEntry_E4*>(
                    (u8*)self->field_0x24C)[word];
                arg = reinterpret_cast<u32>(reinterpret_cast<u8*>(walk) +
                                            e2->field_0x0C);
            }
            UnkEvtListEntry* e =
                *(UnkEvtListEntry**)((u8*)self->field_0xA4 + k2);
            e->vf_0x34((void*)arg);
            n++;
            k2 += 4;
            off2 += 8;
            i2++;
        }
    }

    // List 3 (light): field_0x240 id list, 3-word elements.
    {
        u32 i3 = 0;       // r31
        u32 off3 = 0;     // r30
        u32 k3 = n * 4;   // r26
        while (i3 <
               reinterpret_cast<UnkStateC4Obj*>(self->field_0xC4)->field_0x24) {
            u32 word = *(u32*)((u8*)self->field_0x240 + off3);
            u32 arg;
            if (word == 0xFFFFFFFF) {
                arg = 0;
            } else {
                UnkStateEntry_E4* e2 = &reinterpret_cast<UnkStateEntry_E4*>(
                    (u8*)self->field_0x24C)[word];
                arg = reinterpret_cast<u32>(reinterpret_cast<u8*>(walk) +
                                            e2->field_0x0C);
            }
            UnkEvtListEntry* e =
                *(UnkEvtListEntry**)((u8*)self->field_0xA4 + k3);
            func_80185378(reinterpret_cast<CREvtEffect*>(e), (void*)arg,
                          (void*)((u8*)self->field_0x240 + off3));
            n++;
            k3 += 4;
            off3 += 0xC;
            i3++;
        }
    }

    // List 4 (movie): field_0x248 id list, 1-word elements; only when the
    // 0xEC gate is set.
    if (self->field_0xEC != 0) {
        u32 i4 = 0;       // r31
        u32 off4 = 0;     // r30
        u32 k4 = n * 4;   // r26
        while (i4 <
               reinterpret_cast<UnkStateC4Obj*>(self->field_0xC4)->field_0x34) {
            u32 word = *(u32*)((u8*)self->field_0x248 + off4);
            u32 arg;
            if (word == 0xFFFFFFFF) {
                arg = 0;
            } else {
                UnkStateEntry_E4* e2 = &reinterpret_cast<UnkStateEntry_E4*>(
                    (u8*)self->field_0x24C)[word];
                arg = reinterpret_cast<u32>(reinterpret_cast<u8*>(walk) +
                                            e2->field_0x0C);
            }
            UnkEvtListEntry* e =
                *(UnkEvtListEntry**)((u8*)self->field_0xA4 + k4);
            func_801C36C4(reinterpret_cast<CREvtLight*>(e),
                          reinterpret_cast<const char*>(arg),
                          reinterpret_cast<u32>((u8*)self->field_0x248 + off4));
            n++;
            k4 += 4;
            off4 += 4;
            i4++;
        }
    }

    // Final walk: reset every remaining list entry (movie stop).
    {
        u32 k5 = n * 4;   // r26
        u32 i5 = 0;       // r27
        while (i5 <
               reinterpret_cast<UnkStateC4Obj*>(self->field_0xC4)->field_0x48) {
            UnkEvtListEntry* e =
                *(UnkEvtListEntry**)((u8*)self->field_0xA4 + k5);
            func_80294BA4(reinterpret_cast<CREvtMovie*>(e));
            k5 += 4;
            i5++;
        }
    }

    // Apply the entry's scene / fade flags: arm the 0x4 bit, clear the 0x4
    // bit, restore the BGM volume and arm the culling state, then clear the
    // low 12 bits of the object flag word.
    if (entry->field_0x38 & 1) {
        u32* f = reinterpret_cast<EvtSeqScn7C*>(lbl_eu_80663E14)->field_0x7C;
        *f |= 4;
    }
    if (entry->field_0x38 & 2) {
        u32* f = reinterpret_cast<EvtSeqScn7C*>(lbl_eu_80663E14)->field_0x7C;
        *f &= ~4;
    }
    if (lbl_eu_80667658 != entry->field_0x3C) {
        func_8048EA38(entry->field_0x3C);
    }
    cf::CTaskCulling::func_801A2BD0((entry->field_0x38 >> 3) & 1);
    self->field_0x5C &= ~0xFFF;
}

u32 func_8016A24C(int idx) {
    UnkState_80664268* p = lbl_eu_80664268;
    UnkStateEntry_E4* list = p->field_0xE4;
    if (list == 0) {
        return 0;
    }
    return p->field_0xC4 + list[idx].field_0x0C;
}

extern "C" void* func_8016A27C(void* nameData) {
    // Walk the realtime-event list; the first entry whose name-data id matches
    // `nameData` is asked to resolve (vtable+0x14), and the resolved object's
    // vtable+0xA8 result is returned. The walk bounds are re-read from the
    // shared state in the loop header each iteration (retail reloads
    // lbl_eu_80664268 there and reuses it for the entry load). `scaled` is an
    // explicit induction variable so the back-edge increments sit in source
    // order (i++, scaled+=4) like retail.
    u32 scaled;
    u32 i;
    UnkEvtListEntry* entry;
    UnkState_80664268* st = lbl_eu_80664268;
    UnkStateC4Obj* obj = reinterpret_cast<UnkStateC4Obj*>(st->field_0xC4);
    i = obj->field_0x28;
    scaled = i << 2;
    while (i < reinterpret_cast<UnkStateC4Obj*>(lbl_eu_80664268->field_0xC4)
                    ->field_0x20 +
               reinterpret_cast<UnkStateC4Obj*>(lbl_eu_80664268->field_0xC4)
                    ->field_0x28) {
        entry = reinterpret_cast<UnkEvtListEntry*>(
            *(u32*)((u8*)lbl_eu_80664268->field_0xA4 + scaled));
        if ((u32)nameData == entry->field_0x1C->field_0x0C) {
            if (entry->vf_0x14() != 0) {
                UnkEvtResolved* res =
                    reinterpret_cast<UnkEvtResolved*>(entry->vf_0x14());
                return res->vf_0xA8();
            }
        }
        i++;
        scaled += 4;
    }
    return 0;
}

u32 func_8016A35C() {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p == 0) {
        return 0;
    }
    return p->field_0x100;
}

u32 func_8016A378() {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p == 0) {
        return 0;
    }
    UnkStateTable_D0* arr = p->field_0xD0;
    u32 idx = p->field_0xF8;
    // Dynamic byte stride: entry address = arr + arr->field_0x4 * idx.
    return ((UnkStateTable_D0*)((u8*)arr + arr->field_0x4 * idx))->field_0xC;
}

u32 func_8016A3A8() {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p == 0) {
        return 0;
    }
    return p->field_0x104;
}

u32 func_8016A3C4() {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p == 0) {
        return 0;
    }
    return p->field_0xF8;
}

void cf::CTaskREvtSequence::Init() {
    field_0x12C = func_8048EA40();
    // Install the move callback (3-word ptmf at +0x3C) from the .data table.
    // Pointer-walk (*src++) so MWCC folds the base addi into the first load
    // (retail emits lwzu + lwz / stw,stw / lwz,stw). Value locals are declared
    // before the pointer so the allocator assigns w0/w1 the low scratch regs
    // (retail: lwzu r3 / lwz r0 / lis r4).
    u32 w1, w0, w2;
    const u32* src = &lbl_eu_80530AF4[0];
    w0 = *src++;
    w1 = *src++;
    field_0x40 = w1;
    field_0x3C = w0;
    w2 = *src++;
    field_0x44 = w2;
}

void cf::CTaskREvtSequence::Term() {
    // Clear the scene's byte flag, then run the sequence teardown callback if
    // the 0x115 gate is clear. func_8016A480's stub body is empty, so call it
    // through a cast to stop MWCC from inlining/eliding the call.
    func_eu_8049AB50((u8*)lbl_eu_80663E14, 0);
    if (field_0x115 == 0) {
        ((void(*)(void*))func_8016A480)(this);
    }
    CTaskCulling::func_801A2BD0(0);
    lbl_eu_80663EE0 &= ~0x40;
    func_8048EA38(field_0x12C);
}

void func_8016A480(void* self){}



bool func_8016ABA8(cf::CTaskREvtSequence* self, EvtSeqFileEvent* ev) {
    // Async file-event handler for the three CDeviceFile handles (0xB8 direct
    // read, 0xBC common-archive read, 0xFC sequence-chunk read). A load event
    // (field_00 == 1) releases the handle's buffer, publishes the language
    // entry / fade state, or arms the CX streaming-uncompression context.
    if ((u32)ev->field_04 == self->field_0xB8) {
        if (ev->field_00 == 1) {
            self->field_0x5C |= 0x1;
            CFileHandle* h = reinterpret_cast<CFileHandle*>(self->field_0xB8);
            self->field_0xC0 = reinterpret_cast<u32>(h->getData());
            // Cast keeps MWCC from inlining the empty sibling stub.
            ((void(*)(u32, u32, u32))func_8016C450)(ev->field_0C,
                                                    ev->field_10, 0x800);
        }
        self->field_0xB8 = 0;
        return true;
    }
    if ((u32)ev->field_04 == self->field_0xBC) {
        if (ev->field_00 == 1) {
            self->field_0x5C |= 0x2;
            // Cast keeps MWCC from inlining the empty sibling stub.
            ((void(*)(u32, u32, u32))func_8016C450)(ev->field_0C,
                                                    ev->field_10,
                                                    0xFFFFFFFF);
            s32 lang = CDeviceSC::getLanguage();
            if ((u8)lang > 5) {
                lang = 1;
            }
            EvtSeqC4Buf* buf = reinterpret_cast<EvtSeqC4Buf*>(self->field_0xC4);
            u32 v = buf->field_0x50[lang].field_0x0;
            if (lang <= 5 && v < ev->field_14) {
                self->field_0x5C |= 0x10;
                // Upcast to the +0x58 IScnRender secondary base (MWCC
                // null-guards the adjustment).
                IScnRender* cb = reinterpret_cast<EvtSeqWithRender*>(self);
                lbl_eu_80663E14->addRenderCB(cb, 0x12, 0);
                func_802618D8(func_8049627C(lbl_eu_80663E14, -1));
                func_80261944(self->field_0xC4 + v);
            }
        }
        self->field_0xBC = 0;
        return true;
    }
    if ((u32)ev->field_04 == self->field_0xFC) {
        if (ev->field_00 == 1) {
            self->field_0x5C |= 0x4;
            self->field_0x130 = self->field_0x130 == 0 ? 1 : 0;
            memset(self->mCxBuffer, 0, 0x8a8);
            CXInitUncompContextLH(
                reinterpret_cast<CXUncompContextLH*>(self->mCxBuffer),
                reinterpret_cast<void*>(self->field_0x11C));
            self->field_0x5C |= 0x40;
            self->field_0x124 =
                reinterpret_cast<EvtSeqHandle8*>(self->field_0xFC)->field_0x8;
        }
        self->field_0xFC = 0;
        return true;
    }
    return false;
}

extern "C" void* func_8016AD44(void* self) {
    // Walk the realtime-event list (bounds clamped to the state limit); return
    // the first type-3 entry, other than self, whose name string matches
    // self's name (func_801727D0 -> field_0x1C + 0x10).
    UnkState_80664268* st = lbl_eu_80664268;
    UnkStateC4Obj* obj = reinterpret_cast<UnkStateC4Obj*>(st->field_0xC4);
    u32 limit = st->field_0xA8;
    // MWCC allocates callee-saved registers in REVERSE declaration order:
    // entry (declared last) -> r27, i -> r28, end -> r29, name -> r30,
    // scaled -> r31, matching the retail build exactly. `scaled` is an
    // explicit source induction variable so its back-edge increment sits in
    // source order AFTER `i++` (retail: addi i; addi scaled).
    u32 scaled;
    const char* nameEntry;
    u32 end;
    u32 i;
    UnkEvtListEntry* entry;
    i = obj->field_0x28;
    end = i + obj->field_0x20;
    if (end > limit) {
        end = limit;
    }
    scaled = i << 2;
    while (i < end) {
        UnkState_80664268* cur = lbl_eu_80664268;
        entry = reinterpret_cast<UnkEvtListEntry*>(
            *(u32*)((u8*)cur->field_0xA4 + scaled));
        if (entry->field_0x14 == 3 &&
            entry != reinterpret_cast<UnkEvtListEntry*>(self)) {
            nameEntry = (const char*)func_801727D0(entry);
            if (strcmp((const char*)func_801727D0(self), nameEntry) == 0) {
                return entry;
            }
        }
        i++;
        scaled += 4;
    }
    return 0;
}

int func_8016ADF8(UnkEvtListEntry* self) {
    // Walk the realtime-event list (bounds clamped to the state limit); return
    // 1 for the first type-3 entry, other than self, whose name matches self's
    // name (func_801727D0 -> field_0x1C + 0x10) and that is busy (0x48/0x40).
    // Register allocation mirrors func_8016AD44: first-declared local gets the
    // highest callee-saved reg (scaled r31 .. entry r27, self r26). `scaled` is
    // an explicit induction variable so its back-edge increment sits after i++.
    u32 scaled;
    const char* nameEntry;
    u32 end;
    u32 i;
    UnkEvtListEntry* entry;
    UnkState_80664268* st = lbl_eu_80664268;
    UnkStateC4Obj* obj = reinterpret_cast<UnkStateC4Obj*>(st->field_0xC4);
    u32 limit = st->field_0xA8;
    i = obj->field_0x28;
    end = i + obj->field_0x20;
    if (end > limit) {
        end = limit;
    }
    scaled = i << 2;
    while (i < end) {
        UnkState_80664268* cur = lbl_eu_80664268;
        entry = reinterpret_cast<UnkEvtListEntry*>(
            *(u32*)((u8*)cur->field_0xA4 + scaled));
        if (entry->field_0x14 == 3 && entry != self) {
            nameEntry = (const char*)func_801727D0(entry);
            if (strcmp((const char*)func_801727D0(self), nameEntry) == 0) {
                int busy =
                    entry->field_0x48 != 0 || entry->field_0x40 != 0;
                if (busy) {
                    return 1;
                }
            }
        }
        i++;
        scaled += 4;
    }
    return 0;
}

extern "C" cf::CTaskREvtSequence* func_8016AED4(CProcess* parent, const char* name) {
    // Factory: allocate a CTaskREvtSequence block, construct it with the
    // sequence name, register it under `parent`, and return it.
    cf::CTaskREvtSequence* obj = (cf::CTaskREvtSequence*)mtl::MemManager::allocate(
        0xaf8, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // Cast keeps MWCC from inlining the sibling ctor stub (tiny body).
        obj = (cf::CTaskREvtSequence*)((void*(*)(void*, const char*))__ct__cf_CTaskREvtSequence)(obj, name);
    }
    ((CProcess*)obj)->Regist(parent, false);
    return obj;
}

int func_8016AF4C(u8* data, const char* name, s32* out) {
    // Walk the ResFile's character-animation entries and resolve `name`
    // through the entry's user-data dictionary; the first hit publishes the
    // resolved entry data's first word into `out` and returns 1. The nw4r
    // API layers carry their own DB_ASSERT-style null checks (retail emits
    // them as dead branches after each guard). Locals are declared so MWCC
    // assigns userData/result r31, numAnmChr r30 and the loop counter r29.
    u8* userData;
    int numAnmChr;
    int i;
    numAnmChr =
        GetResAnmChrNumEntries__Q34nw4r3g3d7ResFileCFv(data + 0xC);
    for (i = 0; i < numAnmChr; i++) {
        nw4r::g3d::ResAnmChr anmChr(func_8049E708(data, i));
        userData = reinterpret_cast<u8*>(anmChr.GetResUserData());
        if (userData == 0) {
            continue;
        }
        if (userData == 0) {
            nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC,
                            lbl_eu_80530CF0, lbl_eu_806623C4);
        }
        u8* dic = reinterpret_cast<u8*>(userData) + 4;
        nw4r::g3d::ResDic resDic(dic);
        if ((reinterpret_cast<u32>(dic) & 3) != 0) {
            nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
        }
        u8* result = reinterpret_cast<u8*>(resDic[name]);
        if ((reinterpret_cast<u32>(result) & 3) != 0) {
            nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
        }
        if (result == 0) {
            return 0;
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623C8);
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623C8);
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623C8);
        }
        EvtSeqResEntry* entry = reinterpret_cast<EvtSeqResEntry*>(result);
        if (entry->field_0xC != 0) {
            nw4r::db::Panic(lbl_eu_80530E1C, 0x36, lbl_eu_80530DD8);
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530D94, 0x26, lbl_eu_80530D78,
                            lbl_eu_80530D68, lbl_eu_806623C8);
        }
        EvtSeqIdWord* p = 0;
        if (entry->field_0x4 != 0) {
            p = reinterpret_cast<EvtSeqIdWord*>(
                reinterpret_cast<u8*>(result) + entry->field_0x4);
        }
        *out = static_cast<s32>(p->field_0x0);
        return 1;
    }
    return 0;
}

int func_8016B164(u8* data, const char* name, s32* out, s32* out2) {
    // Same ResFile walk as func_8016AF4C, but with a second output: the
    // resolved entry's +0x8 word is published into `out2` (between the dead
    // null-check and the entry-type check), then the resolved data pointer
    // (or 0 when field_0x4 is null) goes into `out`. `userData` is reused for
    // the dictionary result so MWCC keeps both in one callee-saved register
    // (retail reuses r31).
    u8* userData;
    int numAnmChr;
    int i;
    numAnmChr =
        GetResAnmChrNumEntries__Q34nw4r3g3d7ResFileCFv(data + 0xC);
    for (i = 0; i < numAnmChr; i++) {
        nw4r::g3d::ResAnmChr anmChr(func_8049E708(data, i));
        userData = reinterpret_cast<u8*>(anmChr.GetResUserData());
        if (userData == 0) {
            continue;
        }
        if (userData == 0) {
            nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC,
                            lbl_eu_80530CF0, lbl_eu_806623C4);
        }
        u8* dic = reinterpret_cast<u8*>(userData) + 4;
        nw4r::g3d::ResDic resDic(dic);
        if ((reinterpret_cast<u32>(dic) & 3) != 0) {
            nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
        }
        userData = reinterpret_cast<u8*>(resDic[name]);
        if ((reinterpret_cast<u32>(userData) & 3) != 0) {
            nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
        }
        if (userData == 0) {
            return 0;
        }
        if (userData == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623CC);
        }
        EvtSeqResEntry* entry = reinterpret_cast<EvtSeqResEntry*>(userData);
        *out2 = entry->field_0x8;
        if (userData == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623CC);
        }
        if (entry->field_0xC != 0) {
            nw4r::db::Panic(lbl_eu_80530E1C, 0x36, lbl_eu_80530DD8);
        }
        if (userData == 0) {
            nw4r::db::Panic(lbl_eu_80530D94, 0x26, lbl_eu_80530D78,
                            lbl_eu_80530D68, lbl_eu_806623C8);
        }
        EvtSeqIdWord* p = 0;
        if (entry->field_0x4 != 0) {
            p = reinterpret_cast<EvtSeqIdWord*>(
                reinterpret_cast<u8*>(userData) + entry->field_0x4);
        }
        *out = static_cast<s32>(p->field_0x0);
        return 1;
    }
    return 0;
}

int func_8016B384(u8* data, const char* name, f32* out) {
    // Same ResFile walk as func_8016B5A4, but the matched entry must be a
    // type-1 user-data record and the resolved data's first float is
    // published into `out`.
    u8* userData;
    int numAnmChr;
    int i;
    numAnmChr =
        GetResAnmChrNumEntries__Q34nw4r3g3d7ResFileCFv(data + 0xC);
    for (i = 0; i < numAnmChr; i++) {
        nw4r::g3d::ResAnmChr anmChr(func_8049E708(data, i));
        userData = reinterpret_cast<u8*>(anmChr.GetResUserData());
        if (userData == 0) {
            continue;
        }
        if (userData == 0) {
            nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC,
                            lbl_eu_80530CF0, lbl_eu_806623C4);
        }
        u8* dic = reinterpret_cast<u8*>(userData) + 4;
        nw4r::g3d::ResDic resDic(dic);
        if ((reinterpret_cast<u32>(dic) & 3) != 0) {
            nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
        }
        u8* result = reinterpret_cast<u8*>(resDic[name]);
        if ((reinterpret_cast<u32>(result) & 3) != 0) {
            nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
        }
        if (result == 0) {
            return 0;
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623CC);
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623CC);
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623CC);
        }
        EvtSeqResEntry* entry = reinterpret_cast<EvtSeqResEntry*>(result);
        if (entry->field_0xC != 1) {
            nw4r::db::Panic(lbl_eu_80530E74, 0x3D, lbl_eu_80530E30);
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530D94, 0x26, lbl_eu_80530D78,
                            lbl_eu_80530D68, lbl_eu_806623C8);
        }
        u32 addr = 0;
        if (entry->field_0x4 != 0) {
            addr = reinterpret_cast<u32>(reinterpret_cast<u8*>(result) +
                                         entry->field_0x4);
        }
        *out = *reinterpret_cast<f32*>(addr);
        return 1;
    }
    return 0;
}

int func_8016B5A4(u8* data, const char* name, s32* out) {
    // Same ResFile walk as func_8016AF4C, but the matched entry must be a
    // type-2 user-data record and the resolved data pointer (not its first
    // word) is published into `out`. Locals follow func_8016AF4C's register
    // plan (userData/result r31, numAnmChr r30, counter r29).
    u8* userData;
    int numAnmChr;
    int i;
    numAnmChr =
        GetResAnmChrNumEntries__Q34nw4r3g3d7ResFileCFv(data + 0xC);
    for (i = 0; i < numAnmChr; i++) {
        nw4r::g3d::ResAnmChr anmChr(func_8049E708(data, i));
        userData = reinterpret_cast<u8*>(anmChr.GetResUserData());
        if (userData == 0) {
            continue;
        }
        if (userData == 0) {
            nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC,
                            lbl_eu_80530CF0, lbl_eu_806623C4);
        }
        u8* dic = reinterpret_cast<u8*>(userData) + 4;
        nw4r::g3d::ResDic resDic(dic);
        if ((reinterpret_cast<u32>(dic) & 3) != 0) {
            nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
        }
        u8* result = reinterpret_cast<u8*>(resDic[name]);
        if ((reinterpret_cast<u32>(result) & 3) != 0) {
            nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
        }
        if (result == 0) {
            return 0;
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623CC);
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623CC);
        }
        EvtSeqResEntry* entry = reinterpret_cast<EvtSeqResEntry*>(result);
        if (entry->field_0xC != 2) {
            nw4r::db::Panic(lbl_eu_80530ECC, 0x44, lbl_eu_80530E88);
        }
        if (result == 0) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_806623CC);
        }
        u32 addr = 0;
        if (entry->field_0x4 != 0) {
            addr = reinterpret_cast<u32>(reinterpret_cast<u8*>(result) +
                                         entry->field_0x4);
        }
        *out = static_cast<s32>(addr);
        return 1;
    }
    return 0;
}

int func_8016B788(u8* data) {
    // Gate on the global sda, then resolve a name string from the 0x80503098
    // pool at +0x5D and branch on the resulting status value against the
    // region code from func_80059C14 (0/1/2): != 0 / != 1 / != 2 / == 2.
    if (getGlobalSda() == 0) {
        return 0;
    }
    s32 out = 0;
    const char* name = (const char*)lbl_eu_80503098;
    // Cast keeps MWCC from inlining the empty sibling stub (tiny body).
    ((void(*)(u8*, const char*, s32*))func_8016AF4C)(data, name + 0x5D, &out);
    int v = func_80059C14(getGlobalSda());
    switch (out) {
    case 1:
        return v != 0;
    case 2:
        return v != 1;
    case 3:
        return v != 2;
    case 4:
        return v == 2;
    }
    return 0;
}

void func_8016B860(cf::CTaskREvtSequence* self) {
    // Event-sequence update: arm the minimap/event gates, pump the CX stream,
    // then walk the field_0xE8 table for the entry matching field_0x104 and
    // dispatch its type (1/2/3/5/7) to the menu sound/BGM helpers. The frame
    // duration is scaled by getSecPerFrame when non-negative; the int->float
    // conversions go through the 2^52 magic constant (lbl_eu_80667680).
    if (func_8011C2E8() != 0) {
        func_8011C400();
    }
    if (func_80164910() != 0) {
        func_8007FE1C__Q22cf13CfGameManagerFv(0x1000, 1);
    }
    if (self->field_0x5C & 0x40) {
        s32 n = CXReadUncompLH(
            reinterpret_cast<CXUncompContextLH*>(self->mCxBuffer),
            reinterpret_cast<const void*>(self->field_0x124), 0x2000);
        if (n <= 0) {
            self->field_0x5C &= ~0x20;
            self->field_0x120 = reinterpret_cast<UnkSeq120*>(self->field_0x11C);
        } else {
            self->field_0x124 += 0x2000;
        }
    }
    func_80261960(self->field_0x104);
    if (self->field_0xE8 == 0) {
        return;
    }
    if (cf::CfGameManager::func_800829B8() != 0) {
        return;
    }
    u32 i;
    for (i = 0; i < reinterpret_cast<EvtSeqC4Buf*>(self->field_0xC4)->field_0x38;
         i++) {
        UnkE8Table* entry = reinterpret_cast<UnkE8Table*>(
            reinterpret_cast<u8*>(self->field_0xE8) +
            self->field_0xE8->field_0x4 * i);
        u16 id = entry->field_0x8;
        if (id == 0) {
            continue;
        }
        if (id != self->field_0x104) {
            continue;
        }
        f32 dur;
        if (entry->field_0xC < 0) {
            dur = lbl_eu_8066766C;
        } else {
            f32 fps = CDeviceVI::getSecPerFrame();
            dur = (f32)(s32)entry->field_0xC * fps;
        }
        u8 type = entry->field_0xA;
        switch (type) {
        case 1:
            func_80189034(reinterpret_cast<const char*>(
                              reinterpret_cast<u8*>(entry) + 0x10),
                          1, dur, lbl_eu_8066766C);
            self->field_0x108 = 1;
            break;
        case 2:
            if (entry->field_0xE == 0) {
                func_80188D34(reinterpret_cast<const char*>(
                                  reinterpret_cast<u8*>(entry) + 0x10),
                              1, dur, lbl_eu_8066766C);
            } else {
                if (entry->field_0xE == 1) {
                    u32 c1 = (lbl_eu_80664268 != 0) &&
                             (cf::CfGameManager::func_80086DBC() == 4);
                    if (c1 == 0) {
                        func_80188D34(reinterpret_cast<const char*>(
                                          reinterpret_cast<u8*>(entry) + 0x10),
                                      1, dur, lbl_eu_8066766C);
                    }
                }
                if (entry->field_0xE == 2) {
                    u32 c2 = (lbl_eu_80664268 != 0) &&
                             (cf::CfGameManager::func_80086DBC() == 4);
                    if (c2 != 0) {
                        func_80188D34(reinterpret_cast<const char*>(
                                          reinterpret_cast<u8*>(entry) + 0x10),
                                      1, dur, lbl_eu_8066766C);
                    }
                }
            }
            func_8007C0F8__Q22cf13CfGameManagerFv();
            self->field_0x108 = 1;
            break;
        case 3:
            func_80189318(0, dur);
            break;
        case 5:
            func_8018896C(0, 0, lbl_eu_80667688 * (f32)(s32)entry->field_0x10,
                          dur);
            func_8018896C(1, 0, lbl_eu_80667688 * (f32)(s32)entry->field_0x10,
                          dur);
            break;
        case 7:
            func_801AACA8(1);
            break;
        }
    }
}

void func_8016BB38(cf::CTaskREvtSequence* self) {
    // Outer walk over type-1 event entries (bounds from self->field_0xC4);
    // for each with a nonzero +0x3C gate, inner-walk the field_0xF0 table
    // (dynamic stride at +0x4) matching the name string against the entry's
    // name; on a match refresh the +0x9 LOD byte (if 0xFF) and arm the +0xA
    // task. Locals: i r30, entry r29, j r28, e r27, self r31.
    if (self->field_0xF0 == 0) {
        return;
    }
    u32 i;
    UnkEvtListEntry* entry;
    u32 j;
    UnkBB38Table* e;
    i = 0;
    while (i < reinterpret_cast<UnkStateC4Obj*>(self->field_0xC4)->field_0x20) {
        entry = reinterpret_cast<UnkEvtListEntry*>(
            *(u32*)((u8*)self->field_0xA4 +
                    ((reinterpret_cast<UnkStateC4Obj*>(self->field_0xC4)
                          ->field_0x28 +
                      i)
                     << 2)));
        if (entry->field_0x14 == 1) {
            if (entry->field_0x3C == 0) {
                return;
            }
            j = 0;
            while (j <
                   reinterpret_cast<UnkStateC4Obj*>(self->field_0xC4)
                       ->field_0x3C) {
                e = reinterpret_cast<UnkBB38Table*>(
                    (u8*)self->field_0xF0 +
                    self->field_0xF0->field_0x4 * j);
                if (strcmp(&e->mName[0],
                           (const char*)func_801727D0(entry)) == 0) {
                    if (e->field_0x9 == 0xFF) {
                        e->field_0x9 =
                            (u8)func_80462E1C__8CTaskLODFv(e->field_0xA);
                    }
                    func_80462D5C__8CTaskLODFv(e->field_0xA);
                }
                j++;
            }
        }
        i++;
    }
}

void func_8016BC1C(UnkEvtListEntry* self) {
    // Insert `self` into the realtime-event list, sorted by the name-data
    // priority word (+0x30): find an existing same-name entry with a HIGHER
    // priority and replace it in place, otherwise append at the end. Either
    // way run a bubble-sort pass over the whole list afterwards. The outer
    // walk uses the cached state pointer while the sorts re-read the global.
    // Declaration order follows retail's register assignment: self r31,
    // scaled r30, count r29, eName r28, selfName r27, state r26, i r25.
    u32 scaled;                     // r30 (byte index)
    s32 count;                      // r29
    UnkState_80664268* state;       // r26
    s32 i;                          // r25 (entry counter)
    i = 0;
    scaled = 0;
    state = lbl_eu_80664268;
    count = state->field_0x110;
    while (i < count) {
        // Load order mirrors retail: base, self's name data, then the entry
        // (lwzx), then the entry's name data.
        UnkEvtNameData* selfName = self->field_0x1C;
        UnkEvtListEntry* e =
            *(UnkEvtListEntry**)((u8*)state->field_0x10C + scaled);
        UnkEvtNameData* eName = e->field_0x1C;
        if (strcmp(&eName->mName[0], &selfName->mName[0]) == 0) {
            if (eName->field_0x30 > selfName->field_0x30) {
                state->field_0x10C[i] = self;
                // Bubble-sort pass 1 (list unchanged in length).
                int swapped;
                s32 j;
                u32 scaledJ;
                do {
                    swapped = 0;
                    j = 0;
                    scaledJ = 0;
                    while (j < (s32)lbl_eu_80664268->field_0x110 - 1) {
                        UnkEvtListEntry* a = *(UnkEvtListEntry**)(
                            (u8*)lbl_eu_80664268->field_0x10C + scaledJ);
                        UnkEvtListEntry* b = *(UnkEvtListEntry**)(
                            (u8*)lbl_eu_80664268->field_0x10C + scaledJ + 4);
                        if (a->field_0x1C->field_0x30 >
                            b->field_0x1C->field_0x30) {
                            *(UnkEvtListEntry**)((u8*)lbl_eu_80664268
                                                     ->field_0x10C +
                                                 scaledJ + 4) = a;
                            swapped = 1;
                            *(UnkEvtListEntry**)((u8*)lbl_eu_80664268
                                                     ->field_0x10C +
                                                 scaledJ) = b;
                        }
                        scaledJ += 4;
                        j++;
                    }
                } while (swapped != 0);
                return;
            }
        }
        scaled += 4;
        i++;
    }
    // Append at the end of the list, then bubble-sort pass 2.
    lbl_eu_80664268->field_0x10C[state->field_0x110] = self;
    state->field_0x110 = state->field_0x110 + 1;
    int swapped;
    s32 j;
    u32 scaledJ;
    do {
        swapped = 0;
        j = 0;
        scaledJ = 0;
        while (j < (s32)lbl_eu_80664268->field_0x110 - 1) {
            UnkEvtListEntry* a = *(UnkEvtListEntry**)(
                (u8*)lbl_eu_80664268->field_0x10C + scaledJ);
            UnkEvtListEntry* b = *(UnkEvtListEntry**)(
                (u8*)lbl_eu_80664268->field_0x10C + scaledJ + 4);
            if (a->field_0x1C->field_0x30 > b->field_0x1C->field_0x30) {
                *(UnkEvtListEntry**)((u8*)lbl_eu_80664268->field_0x10C +
                                     scaledJ + 4) = a;
                swapped = 1;
                *(UnkEvtListEntry**)((u8*)lbl_eu_80664268->field_0x10C +
                                     scaledJ) = b;
            }
            scaledJ += 4;
            j++;
        }
    } while (swapped != 0);
}

void func_8016BDA8(){}

u32 func_8016C118(u32 resId) {
    // Resolve a packed resource id to the matching object's packed id: walk
    // the CfGameManager object list, and for each node whose container has a
    // live model object (+0x4594 / +0x4598), derive the object's packed id
    // from its name and compare the minute field (out1 / 10) against the
    // request's; return the first match. Fall back to
    // func_8007DE94(..., 5) and return the original id if that does not match.
    u32 a0, a1, a2, a3;
    func_800AA318(resId, &a0, &a1, &a2, &a3);
    EvtSeqMgrView* mgr = func_80086B04__Q22cf13CfGameManagerFv();
    UnkNode4594* node = mgr->field_0x4->field_0x0;
    while (node != func_80086B04__Q22cf13CfGameManagerFv()->field_0x4) {
        UnkContainer4594* container = (UnkContainer4594*)node->field_0x8;
        if (container != 0) {
            container = (UnkContainer4594*)((u8*)container - 0x3E9C);
        }
        UnkObj4594* obj = container->field_0x4594;
        if (obj != 0) {
            u32 b0, b1, b2, b3;
            u32 packed = func_800AA714(obj->vf_0x18());
            func_800AA318(packed, &b0, &b1, &b2, &b3);
            if (b1 / 10 == a1 / 10) {
                return packed;
            }
        }
        obj = container->field_0x4598;
        if (obj != 0) {
            u32 b0, b1, b2, b3;
            u32 packed = func_800AA714(obj->vf_0x18());
            func_800AA318(packed, &b0, &b1, &b2, &b3);
            if (b1 / 10 == a1 / 10) {
                return packed;
            }
        }
        node = node->field_0x0;
    }
    u32 c0, c1, c2, c3;
    u32 fallback = func_8007DE94__Q22cf13CfGameManagerFv(a1 / 10, 5);
    func_800AA318(fallback, &c0, &c1, &c2, &c3);
    u32 result = resId;
    if (c1 / 10 == a1 / 10) {
        result = fallback;
    }
    return result;
}

void cf::CTaskREvtSequence::cbRenderBefore() {
    if (lbl_eu_80663E28 & 0x01000000) {
        return;
    }
    func_80261A80();
}

void func_8016C2C8() {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p != 0) {
        p->field_0x5C |= 0x200;
    }
}

void func_8016C2E4() {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p != 0) {
        p->field_0x5C &= ~0x200;
    }
}

int func_8016C300(UnkEvtListEntry* self) {
    // Event-run gate: if bit 11 of the state flag word is set, run the walk
    // over type-2 entries whose packed token (name object +0x20) unpacks to a
    // 3/8 payload id (return 1), otherwise set bit 0x800 and stop.
    // Residual (open): retail colors obj/limit/i/st/end into r4-r8; this
    // build lands obj/limit/st/i in r4-r7 (st<->i swap), 17 pure reg-swaps.
    UnkStateC4Obj* obj;
    u32 limit;
    s32 i;
    u32 end;
    UnkState_80664268* st;
    u32 out0, out1, out2, out3;
    if (lbl_eu_80664268->field_0x5C & 0x800) {
        return 1;
    }
    st = lbl_eu_80664268;
    obj = reinterpret_cast<UnkStateC4Obj*>(st->field_0xC4);
    limit = st->field_0xA8;
    i = obj->field_0x28;
    end = i + obj->field_0x20;
    if (end > limit) {
        end = limit;
    }
    for (; i < end; i++) {
        UnkEvtListEntry* entry =
            reinterpret_cast<UnkEvtListEntry*>(st->field_0xA4[i]);
        if (entry->field_0x14 == 2 && entry != self) {
            func_800AA318(
                reinterpret_cast<UnkEvtName2*>(entry->field_0x1C)->field_0x20,
                &out0, &out1, &out2, &out3);
            if (out1 == 3 || out1 == 8) {
                return 1;
            }
            lbl_eu_80664268->field_0x5C |= 0x800;
            break;
        }
    }
    return 0;
}

u32 func_8016C3DC() {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p == 0) {
        return 0;
    }
    UnkStateC4* q = reinterpret_cast<UnkStateC4*>(p->field_0xC4);
    if (q == 0) {
        return 0;
    }
    return (q->field_0x4C >> 1) & 1;
}

u32 func_8016C410() {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p == 0) {
        return 0;
    }
    return cf::CfGameManager::func_80086DBC() == 4;
}

void func_8016C450(const char* path, EvtC450Buf* buf, s32 limit) {
    // Filename-strip + byte cipher. The path's file portion is copied into a
    // local key buffer with its extension stripped (searching for the
    // lbl_eu_806623C0 marker), then `buf`'s data area
    // (buf[0 .. buf->field_0x10 + 0x800)) is XOR-encrypted byte by byte:
    // byte = seed ^ ~(byte ^ key[keyIdx]), seed++ per byte; keyIdx wraps at
    // the key length (and at 256). The first 132 bytes go through a fixed
    // 33x4 loop, then the remainder runs until the end marker / limit.
    char nameBuf[0x20];
    s32 nameLen;
    char stripped[0x20];
    s32 strippedLen;
    const char* filePtr;
    int extLen;
    int extOff;
    int dotLen;
    char* p;
    char* pEnd;
    u8* dst;
    u32 seed;
    u32 keyIdx;
    u32 byteCount;
    u32 end;

    nameBuf[0] = 0;
    nameLen = 0;
    filePtr = ml::CPathUtil::getFilePtrFromPath(path);
    nameLen = strlen(filePtr);
    strcpy(nameBuf, filePtr);
    extLen = nameLen;
    if (extLen == 0) {
        extOff = -1;
    } else {
        dotLen = strlen(lbl_eu_806623C0);
        p = (nameBuf - 1) + extLen;
        pEnd = nameBuf - 1;
        while (p != pEnd) {
            if (strncmp(p, lbl_eu_806623C0, dotLen) == 0) {
                extOff = (int)(p - nameBuf);
                goto found_ext;
            }
            p--;
        }
        extOff = -1;
    }
found_ext:
    if (extOff + 1 > 1) {
        stripped[0] = 0;
        strippedLen = 0;
        if (extLen != 0) {
            if (extOff != -1) {
                strncpy(stripped, nameBuf, extOff);
                stripped[extOff] = 0;
                strippedLen = strlen(stripped);
            }
            nameLen = strlen(stripped);
            strcpy(nameBuf, stripped);
        }
    }
    // Fixed first phase: 33 iterations x 4 bytes = 132 bytes.
    seed = 0xC2;
    keyIdx = 0;
    byteCount = 0;
    dst = reinterpret_cast<u8*>(buf);
    for (byteCount = 0; byteCount < 0x84; byteCount += 4) {
        u8 k0 = (u8)keyIdx;
        keyIdx = keyIdx + 1;
        u8 b0 = dst[0];
        u8 nk0 = (u8)keyIdx;
        u8 kb0 = (u8)nameBuf[k0];
        dst[0] = (u8)(seed ^ (u8)~(b0 ^ kb0));
        seed = seed + 1;
        if ((s32)nk0 >= nameLen) {
            keyIdx = 0;
        }
        u8 k1 = (u8)keyIdx;
        keyIdx = keyIdx + 1;
        u8 b1 = dst[1];
        u8 nk1 = (u8)keyIdx;
        u8 kb1 = (u8)nameBuf[k1];
        dst[1] = (u8)(seed ^ (u8)~(b1 ^ kb1));
        seed = seed + 1;
        if ((s32)nk1 >= nameLen) {
            keyIdx = 0;
        }
        u8 k2 = (u8)keyIdx;
        keyIdx = keyIdx + 1;
        u8 b2 = dst[2];
        u8 nk2 = (u8)keyIdx;
        u8 kb2 = (u8)nameBuf[k2];
        dst[2] = (u8)(seed ^ (u8)~(b2 ^ kb2));
        seed = seed + 1;
        if ((s32)nk2 >= nameLen) {
            keyIdx = 0;
        }
        u8 k3 = (u8)keyIdx;
        keyIdx = keyIdx + 1;
        u8 b3 = dst[3];
        u8 nk3 = (u8)keyIdx;
        u8 kb3 = (u8)nameBuf[k3];
        dst[3] = (u8)(seed ^ (u8)~(b3 ^ kb3));
        seed = seed + 1;
        if ((s32)nk3 >= nameLen) {
            keyIdx = 0;
        }
        dst += 4;
    }
    // Remainder until the end marker, stopping at `limit` when non-negative.
    end = (u32)buf->field_0x10 + 0x800;
    dst = reinterpret_cast<u8*>(buf) + byteCount;
    while (byteCount < end) {
        if (limit >= 0 && (s32)byteCount >= limit) {
            break;
        }
        u8 k0 = (u8)keyIdx;
        keyIdx = keyIdx + 1;
        u8 b0 = dst[0];
        u8 nk0 = (u8)keyIdx;
        u8 kb0 = (u8)nameBuf[k0];
        dst[0] = (u8)(seed ^ (u8)~(b0 ^ kb0));
        seed = seed + 1;
        if ((s32)nk0 >= nameLen) {
            keyIdx = 0;
        }
        byteCount++;
        dst++;
    }
}

void func_8016C6EC(bool flag) {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p == 0) {
        return;
    }
    if (flag) {
        p->field_0x5C |= 0x40000;
    } else {
        p->field_0x5C &= ~0x40000;
    }
}

u32 func_8016C720() {
    UnkState_80664268* p = lbl_eu_80664268;
    if (p == 0) {
        return 0;
    }
    return (p->field_0x5C >> 19) & 1;
}

int func_eu_8016DA48(u8* gate) {
    UnkState_80664268* p = reinterpret_cast<UnkState_80664268*>(gate);
    UnkStateTable_D0* table;
    u32 f100;
    u32 entry;
    f100 = p->field_0x100;
    if ((int)f100 <= 1 && (int)p->field_0xF8 > 0) {
        return 0;
    }
    // Walk the 0xD0 table: entry address = table + stride * field_0xF8.
    table = p->field_0xD0;
    entry = ((UnkStateTable_D0*)((u8*)table + table->field_0x4 * p->field_0xF8))->field_0xC;
    // Retail falls through to `return 0`; the branch target returns 1.
    if (f100 >= entry - 3) {
        return 0;
    }
    return 1;
}

void func_8016C7D8(void* self) { ((void(*)(void*))__dt__Q22cf17CTaskREvtSequenceFv)((char*)self - 0x54); }

void func_8016C7E0(void* self) { ((void(*)(void*))cbRenderBefore__Q22cf17CTaskREvtSequenceFv)((char*)self - 0x58); }

extern "C" void func_8016C7E8(u8* self) { ((void(*)(void*))__dt__Q22cf17CTaskREvtSequenceFv)((char*)self - 0x58); }

// Out-of-line CTTask<cf::CTaskREvtSequence> specialization: dispatch the
// +0x3C move callback (tested through the retail __ptmf_test helper) via the
// __ptmf_scall machinery. CTaskREvtSequence is modelled as a flat overlay of
// the CTTask head, so cast the base pointer back with reinterpret_cast.
template<>
void CTTask<cf::CTaskREvtSequence>::Move() {
    if (__ptmf_test(&mMoveFunc)) {
        (reinterpret_cast<cf::CTaskREvtSequence*>(this)->*mMoveFunc)();
    }
}

// Dispatch the +0x48 draw callback through __ptmf_test/__ptmf_scall (retail
// Draw__32CTTask<Q22cf17CTaskREvtSequence>Fv, same shape as Move at +0x3C).
template<>
void CTTask<cf::CTaskREvtSequence>::Draw() {
    if (__ptmf_test(&mDrawFunc)) {
        (reinterpret_cast<cf::CTaskREvtSequence*>(this)->*mDrawFunc)();
    }
}

// Base destructor: destruct CProcess and free the block if the delete flag is
// set (retail __dt__32CTTask<Q22cf17CTaskREvtSequence>Fv).
template<>
CTTask<cf::CTaskREvtSequence>::~CTTask() {}
