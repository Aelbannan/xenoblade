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
extern "C" void __dt__Q22cf17CTaskREvtSequenceFv(void*, int);
extern "C" void cbRenderBefore__Q22cf17CTaskREvtSequenceFv(void*);

// Forward decls for the batch targets (func_80168F38 calls func_80169DD0
// before its definition in this file).
void func_80169CD0(cf::CTaskREvtSequence* self);
void func_80168F38(cf::CTaskREvtSequence* self);
void func_80169DD0(cf::CTaskREvtSequence* self, u32 idx);
void func_8016BC1C(UnkEvtListEntry* self);
u32 func_8016C118(u32 resId);

namespace cf {
    u32 CTaskREvtSequence::func_8016A354() { return field_0x20; }

    void CTaskREvtSequence::OnFileEvent() {
        extern void func_8016ABA8(void*);
        ((void(*)(void*))func_8016ABA8)((char*)this - 0x54);
    }
}

// Retail ctor __ct__cf_CTaskREvtSequence(self, const char* name) strlens +
// strcpys the sequence name into +0x60 and sets up the CTTask vtables; the
// real 0x218-byte body is not part of this target set. Stub returns self.
void* __ct__cf_CTaskREvtSequence(void* self, const char* name) { return self; }

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

void func_80168800(){}

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
    n = 0;
    u32 fps60 = CDeviceVI::getTargetFramerate() * 0x3c;
    self->field_0x138 = self->field_0x138 + 1;
    if (self->field_0x138 > fps60) {
        n = 1;
    }
    if (n == 0) {
        scaled = 0;
        while (n < self->field_0xA8) {
            UnkEvtListEntry* e =
                *(UnkEvtListEntry**)((u8*)self->field_0xA4 + scaled);
            if (e->vf_0x18() == 0) {
                return;
            }
            n++;
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

void func_80169050(){}

void func_8016925C(){}

int func_801696C4(void* self) { return 0; }

void func_801696CC(){}

void func_80169A34() {}

void func_80169A38(){}

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
    if (func_80164910() != 0 && (lbl_eu_80663E28 & 0x80) == 0) {
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
            mgr4->field_0xC->field_0x1EC = lbl_eu_80667678;
            mgr4->field_0xC->field_0x1F0 = lbl_eu_8066767C;
            func_8049EB60(mgr4->field_0xC);
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
        IWorkEvent* evt = reinterpret_cast<EvtSeqWithEvt*>(self);
        self->field_0xFC = (u32)CDeviceFile::readCommonArchiveFile(
            alloc + self->field_0x128 - aligned, self->mPath, evt,
            entry->field_0x14, entry->field_0x10);
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

void func_80169F28(){}

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



void func_8016ABA8(void* self){}

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

void func_8016AF4C(u8* data, const char* name, s32* out){}

void func_8016B164(){}

void func_8016B384(){}

void func_8016B5A4(){}

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

void func_8016B860(){}

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
    UnkState_80664268* state = lbl_eu_80664268;
    s32 count = state->field_0x110;
    s32 i = 0;
    u32 scaled = 0;
    while (i < count) {
        UnkEvtListEntry* e =
            *(UnkEvtListEntry**)((u8*)state->field_0x10C + scaled);
        if (strcmp(&e->field_0x1C->mName[0],
                   &self->field_0x1C->mName[0]) == 0) {
            if (e->field_0x1C->field_0x30 > self->field_0x1C->field_0x30) {
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

void func_8016C450(){}

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
