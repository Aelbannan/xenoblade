// Auto-scaffolded catalog TU for kyoshin/cf/CTaskREvent
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <string.h>
#include "kyoshin/cf/CTaskREvent.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern "C" void __dt__Q22cf11CTaskREventFv(void*, int);
extern "C" void cbRenderBefore__Q22cf11CTaskREventFv(void*);

namespace cf {}
using namespace cf;

// --- CTTask<cf::CTaskREvent> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
template<>
void CTTask<cf::CTaskREvent>::Move() {
    if (mMoveFunc) {
        (static_cast<cf::CTaskREvent*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<cf::CTaskREvent>::Draw() {
    if (mDrawFunc) {
        (static_cast<cf::CTaskREvent*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<cf::CTaskREvent>::~CTTask() {}

class CEventFile;

// Retail ctor symbol is the pre-mangled name __ct__cf_CTaskREvent (a global
// function, not a cf::CTaskREvent member); still a stub - body unmatched.
// Returns the object pointer so func_801665A4 can chain the allocation.
// noinline keeps the call a real bl (an empty inline body would make MWCC
// fold the ctor away and shrink func_801665A4 below the retail size).
__declspec(noinline) CTaskREvent* __ct__cf_CTaskREvent(CTaskREvent* pMem, CScnNw4r* pScene, CView* pView) {
    return pMem;
}

cf::CTaskREvent::~CTaskREvent() {}

// Returns 1 while an event sequence is active: manager present, sequence
// index valid, the +0xB0 gate set, or the +0x6C bit0 flag raised. The final
// `mgr &&` reuses the CR1 null compare from the first guard (retail shape).
int func_80164410() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) return 0;
    if (mgr->field_0x1D4 != -1) return 1;
    if (mgr->field_0xB0 != 0) return 1;
    // Split the `mgr && flag` value into an explicit pointer-test if/else so
    // MWCC materializes the 0/1 word via the CR1 compare (reused from the
    // first guard) and branch-selects the return (MWCC_REFERENCE
    // __wpadGetExtType / HBMDelete patterns).
    u32 flag;
    if (mgr != 0) {
        flag = mgr->field_0x6C & 1;
    } else {
        flag = 0;
    }
    if (flag != 0) {
        return 1;
    } else {
        return 0;
    }
}

// Convert the .sdata u32 counter to float and scale it. MWCC lowers the
// u32->float conversion through the 0x4330 double trick (xoris sign flip +
// subtract 2^52+2^31), which is exactly what the retail body shows.
float func_80164478() {
    return (float)lbl_eu_80662384 * lbl_eu_8066762C;
}

extern "C" u32 func_801644AC() { return (u32)lbl_eu_80662380; }

extern "C" u32 func_801644B4() { return (u32)lbl_eu_80664240; }
void func_801644BC(u32 arg) {
    CEventMgr* mgr = lbl_eu_80664240;
    if (!mgr) return;
    func_80166150(mgr, arg);
}

// func_8016462C: called with a table index by func_801644D8; stub for an
// as-yet-unmatched helper (C linkage inherited from the header declaration
// so the retail plain symbol is emitted). noinline so the call stays a real
// bl (an empty inline body would make MWCC DCE the whole walk loop).
__declspec(noinline) void func_8016462C(int index) {}

// Walks the 32-entry event-id table; for each entry strictly above `lower`
// and at most `upper`, forwards the index to func_8016462C. Only runs when
// `type` equals 0x20.
void func_801644D8(CTaskREvent* self, int type, int upper, int lower) {
    if (type != 0x20) return;
    for (u32 i = 0; i < 0x20; i++) {
        s16 v = lbl_eu_80502F90[i];
        if (v > lower && v <= upper) {
            func_8016462C(i);
        }
    }
}

// Initializes the 32-slot event data table: each slot's u16 id is set to
// 0xC00, its u8 flag cleared, its two 0x10-byte word sub-slots zeroed, and
// its two 0x14-byte byte sub-slots filled with 1.
void func_8016455C(CEventDataTable* self) {
    for (int i = 0; i < 0x20; i++) {
        self->mIds920[i] = 0xC00;
        self->mFlags900[i] = 0;
        CEventDataTable::WordSlot* w = &self->mWords[i][0];
        CEventDataTable::ByteSlot* b = &self->mBytes[i][0];
        CEventDataTable::WordSlot* wEnd = &self->mWords[i][2];
        do {
            w->data[0] = 0;
            b->data[0] = 1;
            b->data[1] = 1;
            b->data[2] = 1;
            b->data[3] = 1;
            b->data[4] = 1;
            w->data[1] = 0;
            b->data[5] = 1;
            b->data[6] = 1;
            b->data[7] = 1;
            b->data[8] = 1;
            b->data[9] = 1;
            w->data[2] = 0;
            b->data[10] = 1;
            b->data[11] = 1;
            b->data[12] = 1;
            b->data[13] = 1;
            b->data[14] = 1;
            w->data[3] = 0;
            b->data[15] = 1;
            b->data[16] = 1;
            b->data[17] = 1;
            b->data[18] = 1;
            b->data[19] = 1;
            w++;
            b++;
        } while (w < wEnd);
    }
}

void func_80164724(){}

void func_80164838(){}

u32 func_80164910() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == nullptr) return 0;
    u32 result = 0;
    if (mgr->field_0x1BC != 0 ||
        (mgr->field_0x1D4 >= 0 && (lbl_eu_80663E28 & 0x01000000) == 0)) {
        result = 1;
    }
    return result;
}

void func_80164954(){}

void func_80164A50(){}

int func_80164C28() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr) {
        return mgr->field_0x6C & 1;
    }
    return 0;
}

// Returns 1 while an event sequence is fully idle: manager present, game
// manager running, no +0x74/+0x6C active flags, no +0xB0 gate, sequence
// counter clean, and no event word pending. Each guard is materialized as a
// 0/1 word (cntlzw/srwi) and AND-accumulated (retail shape).
int func_80164C48() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) return 0;
    if (cf::CfGameManager::func_8007E1B4() == 0) return 0;
    int result = lbl_eu_80664240->field_0x74 & 1;
    if (result) {
        u32 flag;
        if (lbl_eu_80664240 != 0) {
            flag = lbl_eu_80664240->field_0x6C & 1;
        } else {
            flag = 0;
        }
        result = !flag;
    }
    if (result) {
        result = !lbl_eu_80664240->field_0xB0;
    }
    if (result) {
        result = !func_8016847C();
    }
    if (result) {
        result = !lbl_eu_80664240->field_0x1D0;
    }
    return result;
}

// Tears down the active event sequence: clears the +0xB0 gate object, the
// +0x1D0 word, the +0x6C bit0 flag, notifies the game manager, and clears
// the global event bit when the +0x1BC byte flag is set.
void func_80164CFC() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) return;
    u32 v;
    if (mgr != 0) {
        v = mgr->field_0x6C & 1;
    } else {
        v = 0;
    }
    if (v == 0) return;
    CEventMgrB0* p = mgr->field_0xB0;
    if (p != 0) {
        p->field_0x39 = 1;
        lbl_eu_80664240->field_0xB0 = 0;
        lbl_eu_80664240->field_0x1D0 = 0;
    }
    lbl_eu_80664240->field_0x6C &= ~1;
    if (cf::CfGameManager::func_80083298() != 0) {
        if (&cf::CfGameManager::func_80083298()->field_0xF0 != 0) {
            func_8047BDA0__17UnkClass_8047BB54Fv(
                &cf::CfGameManager::func_80083298()->field_0xF0);
        }
    }
    if (lbl_eu_80664240->field_0x1BC != 0) {
        lbl_eu_80663E24 &= ~0x02000000;
    }
}

// Event-update kick: when the manager is present and its +0xB0 gate is set,
// poke the sequence processor, run the bit7-gated cleanup, then clear the
// +0x1D0 word and the +0x6C bit4 flag (re-reading the global after the calls
// because they may have replaced the manager).
void func_80164DB8() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == 0) return;
    if (mgr->field_0xB0 == 0) return;
    func_80168484(1);
    if ((lbl_eu_80663E28 & 0x01000000) == 0) {
        func_80043BC4();
    }
    lbl_eu_80664240->field_0x1D0 = 0;
    lbl_eu_80664240->field_0x6C &= ~0x10;
}

// Builds the 0x1400-byte string table: 0xAE entries of 0x14 bytes, each
// "lbl_eu_80503008" + suffix from lbl_eu_80530458. The entry index lives in
// mIdx (reloaded per use - the calls may clobber memory).
void cf::CTaskREvent::Init() {
    this->mBuf = (u8*)mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0x1400, 4);
    for (u32 i = 0; i < 0xAE; i++) {
        strcpy((char*)this->mBuf + this->mIdx * 0x14, lbl_eu_80503008);
        strcat((char*)this->mBuf + this->mIdx * 0x14, lbl_eu_80530458[i]);
        this->mIdx++;
    }
}

// Loads an SFD movie through the CRI player when the manager is idle
// (mCri == (CLibCri*)-1). `handle` selects the buffer source: a fresh MEM2
// handle vs the scene alloc handle (func_80495FF0(lbl_eu_80663E14)). The
// finished player is stored back through the global, re-read after the call
// because it may have been replaced.
void func_80164ED0(const char* path, int flag, void* handle) {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr != 0 && (u32)mgr->mCri == 0xFFFFFFFF) {
        u32 buffer;
        if (handle != 0) {
            buffer = mtl::MemManager::getHandleMEM2();
        } else {
            buffer = func_80495FF0(lbl_eu_80663E14);
        }
        CLibCri* cri = func_80459AA8__7CLibCriFv(
            path, mtl::MemManager::getHandleMEM2(), buffer, flag, 0);
        lbl_eu_80664240->mCri = cri;
    }
}

void func_80164F6C() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr == nullptr) return;
    CLibCri* cri = mgr->mCri;
    if ((u32)cri == 0xFFFFFFFF) return;
    cri->func_80459AAC();
    // Store through the global again: the call above may have changed it,
    // so retail re-reads lbl_eu_80664240 instead of reusing the local.
    lbl_eu_80664240->mCri = (CLibCri*)-1;
}

int func_80164FB4() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (mgr) {
        CLibCri* cri = mgr->mCri;
        if ((u32)cri != 0xFFFFFFFF) {
            return func_80459AC4__7CLibCriFv(cri);
        }
    }
    return 0;
}

int func_80164FE8(void) {
    int* ptr = (int*)lbl_eu_80664240;
    if (!ptr) return 0;
    int val = *(int*)((char*)ptr + 0x1e0);
    return (unsigned)((-1 - val) | (val + 1)) >> 31;
}

void func_80165014() {
    CEventMgr* mgr = lbl_eu_80664240;
    if (!mgr) return;
    CLibCri* cri = mgr->mCri;
    if ((u32)cri == 0xFFFFFFFF) return;
    cri->func_80459AC0();
}

extern "C" void func_8016C2E4();
extern "C" void func_80165038() { func_8016C2E4(); }

void cf::CTaskREvent::Term() {}

void cf::CTaskREvent::Move() {}

void cf::CTaskREvent::Draw() {
    func_80165DF4(this, 0);
    func_80166050(this, 0);
}

// Stubs for as-yet-unmatched helpers; noinline so Draw's calls are not
// folded away (retail Draw emits real bl's).
__declspec(noinline) void func_80165DF4(CTaskREvent* self, int arg) {}

__declspec(noinline) void func_80166050(CTaskREvent* self, int arg) {}

u32 func_80166150(CEventMgr* self, u32 arg) { return 0; }

// Render-before hook: if the event manager's CRI player is idle, copy the
// current view's rect into our player. Skips when this task or the manager
// holds a live CRI controller.
void cf::CTaskREvent::cbRenderBefore() {
    if ((u32)this->mCri == 0xFFFFFFFF) return;
    CView* view = getCurrentView__5CViewFv();
    CEventMgr* mgr = lbl_eu_80664240;
    u32 result;
    // Nested if/else materializes the CRI-player result into r3 with a
    // per-branch `li r3,0` (retail shape); a flat `&&`/else chain merges the
    // zero-blocks and shrinks the function.
    if (mgr != 0) {
        if ((u32)mgr->mCri != 0xFFFFFFFF) {
            result = func_80459AC4__7CLibCriFv(mgr->mCri);
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }
    if (result != 0) return;
    ml::CRect rect;
    func_8043EA88__5CViewFRQ22ml5CRectP5CView(rect, view);
    func_80459ACC__7CLibCriFv(this->mCri, rect);
}

void func_801662E8(void* self){}

void func_801663A8(void* self){}

// Allocates a CTaskREvent (size 0x1F8) from the work-thread heap and
// registers it into pParent. Returns the task (or 0 if allocation failed).
CTaskREvent* func_801665A4(CProcess* pParent, CScnNw4r* pScene, CView* pView) {
    u8* mem = static_cast<u8*>(
        mtl::MemManager::allocate(0x1f8, CWorkThreadSystem::getWorkMem()));
    CTaskREvent* task = reinterpret_cast<CTaskREvent*>(mem);
    if (mem != 0) {
        task = __ct__cf_CTaskREvent(task, pScene, pView);
    }
    task->Regist(pParent, false);
    return task;
}

void viAfterDrawDone__11CDeviceVICbFv() {}

void viBeforeDrawDone__11CDeviceVICbFv() {}

void func_801666C4(void* self) { ((void(*)(void*))func_801662E8)((char*)self - 0x54); }

void func_801666CC(void* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x54); }

void func_801666D4(void* self) { ((void(*)(void*))func_801644D8)((char*)self - 0x58); }

void func_801666DC(void* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x58); }

void func_801666E4(void* self) { ((void(*)(void*))cbRenderBefore__Q22cf11CTaskREventFv)((char*)self - 0x5c); }

extern "C" void func_801666EC(u8* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x5c); }

void cf::CTaskREvent::OnFileEvent() { func_801663A8(this); }

extern "C" void func_801666FC(u8* self) { ((void(*)(void*))__dt__Q22cf11CTaskREventFv)((char*)self - 0x60); }

// CTTask<cf::CTaskREvent> specializations provided by header
