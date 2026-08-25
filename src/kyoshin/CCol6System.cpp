// Auto-scaffolded catalog TU for kyoshin/CCol6System
// Cleaned-up C++ for CCol6CheckBat; other stubs pending decomp.

#include "kyoshin/CCol6CheckBat.hpp"
// Forward-declare cf::CfGameManager before the include chain pulls in
// CfGameManager.hpp (its UnkClass_8007DAE0::init declaration references the
// type before the namespace block that declares it).
namespace cf {
class CfGameManager;
}
#include "kyoshin/CCol6System.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/device/CDeviceFile.hpp"

#include <string.h>

// CfGameManager.hpp declares CItem_initItemImplInstances as a no-arg extern
// "C" function, so a 1-arg redeclaration would trip MWCC 10197. Route the
// call site through a casted function pointer instead (CItemBoxGrid
// precedent) - same retail symbol/reloc at the call.
#define CItem_initItemImplInstances(item) ((void* (*)(void*))CItem_initItemImplInstances)(item)

// Resolve ml::FixStr<128>::format calls to the explicit specialization that
// CfScript.cpp defines (retail symbol format__Q22ml10FixStr<128>FPCce); a
// plain generic-template call would mangle to ...11FixStr... instead (reloc
// name drift at the call site).
template <> void ml::FixStr<128>::format(const char* fmt, ...);

extern "C" void __dt__8CProcessFv(void* self, int flags);
extern "C" void __dt__10CScrollBarFv(void* self, int flags);

// Forward decls for the layout-build helpers defined below (extern "C" so the
// call sites in func_8015DB08 / func_80160118 bind to the unmangled retail
// symbols). func_8015DB08 is the CCol6Hint file-load completion handler (its
// layout build func_8015DD4C operates on hint offsets 0x9C/0x11C/0x124).
extern "C" void func_8015DD4C(CCol6Hint* self);
extern "C" void func_80160370(CCol6System* self);
extern "C" void func_80160A6C(CCol6System* self, s32 playerIdx);
// Item-box input handlers defined later in this TU (called by Move).
extern "C" void func_80161178(CCol6System* self);
extern "C" void func_8016169C(CCol6System* self);
extern "C" void func_80161C5C(CCol6System* self);
extern "C" void func_80160EE4(u32 unused, s32 arg);
extern "C" void func_80162000(CCol6System* self);
extern "C" void func_80162C40(CCol6System* self);
extern "C" void func_80162DB4(CCol6System* self);
extern "C" void func_80162EF8(CCol6System* self);
extern "C" void func_80163614(CCol6System* self);
extern "C" void func_8016378C(CCol6System* self);
extern "C" void func_801638C0(CCol6System* self);
extern "C" void func_80163AF4(CCol6System* self);
extern "C" void func_8015E0BC(CCol6Hint* self);

// Singleton instance (retail: lbl_eu_80664230).
CCol6CheckBat* gCol6CheckBat;

int lbl_eu_80664230;
int lbl_eu_80664234;
int lbl_eu_80664238;
int lbl_eu_8066235C;

// Composite vtable data for the check-bat / hint factories (retail sizes 0xC4
// / 0xAC; only the symbol names/relocs matter for the function matches).
char lbl_eu_8053011C[0xC4];
char lbl_eu_8053021C[0xAC];
char* lbl_eu_80662358; // .sdata arc-path pointer

void func_8015D0B8() {
    lbl_eu_80664230 = 0;
    lbl_eu_80664234 = 0;
    lbl_eu_80664238 = 0;
    lbl_eu_8066235C = -1;
}

// CCol6CheckBat::~CCol6CheckBat()
// extern "C" free-function form (CSysWin/CMenuQstCnt precedent): the retail
// dtor's outer null-check covers the delete, and the nested double null-check
// is the documented D2-inlined-into-D1 MWCC artifact (same as
// ~CSimpleEveTalkWin / ~CSystemWindow) guarding the CProcess base dtor.
// NOTE: default -O4,p (NOT optimize_for_size) - this retail keeps the
// separate stw r31/stw r30 saves instead of the stmw pair.
extern "C" CCol6CheckBat* __dt__13CCol6CheckBatFv(CCol6CheckBat* self, int flags) {
    if (self != 0) {
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// CCol6CheckBat::Init() - no-op; override satisfies CProcess pure-virtual.
void CCol6CheckBat::Init() {}

// CCol6CheckBat::Term() - clears singleton pointer.
void CCol6CheckBat::Term() {
    *(CCol6CheckBat**)&lbl_eu_80664230 = nullptr;
}

// CCol6CheckBat::Move() - once the game is running, shows the two check-bat
// messages (indices 0x7f/0x80 of the shared string pool) and opens the queued
// window (lbl_eu_8066235C + 0x100) when a window id is queued; either way the
// bat ends up flagged done at +0x64.
void CCol6CheckBat::Move() {
    if (func_8013BE50() == 0) {
        return;
    }

    if (lbl_eu_8066235C >= 0) {
        func_80135998(1);
        char* msg = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[9], 0x7f);
        func_8013D55C(msg, 0, 0);
        msg = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[9], 0x80);
        func_8013D55C(msg, 0, 0);

        func_8013DA60(lbl_eu_8066235C + 0x100, 0, 0);
        lbl_eu_8066235C = -1;
        func_80135998(0);

        // Retail keeps a copy of this store in BOTH arms (the b-over-else
        // emits the extra branch); do not hoist it out of the if/else.
        mFlag64 = 1;
    } else {
        mFlag64 = 1;
    }
}

// Byte-range shim over the CProcess header + owned fields so the factory can
// write the vtable (+0x10), the __ptmf_null callback slots (+0x3C..0x53) and
// the trailing scalars without raw pointer arithmetic. Matches CProcess's
// 0x00-0x3B layout exactly (CProcess.hpp: CProcess : CChildListNode).
struct CCol6CheckBatCtorShim {
    u8 _00[0x10];
    void* vtable;        // 0x10 - CProcess vtable, overwritten by this factory
    u8 _14[0x28];        // 0x14-0x3B - rest of CProcess
    u32 moveCallbacks[3]; // 0x3C-0x47 - __ptmf_null move-callback slots
    u32 drawCallbacks[3]; // 0x48-0x53 - __ptmf_null draw-callback slots
    u32 field54;         // 0x54
    u32 field58;         // 0x58
    u32 field5C;         // 0x5C
    s32 index;           // 0x60 - init -1
    u8 flag64;           // 0x64
    u8 flag65;           // 0x65
    u8 flag66;           // 0x66
    u8 active;           // 0x67 - init 1
    u32 field68;         // 0x68
    u32 field6C;         // 0x6C - lbl_eu_8053021C + 0x24
    u8 flag70;           // 0x70
};

// Type-distinct view of __ptmf_null for the CheckBat ctor (see usage below).
struct CCol6CheckBatPtmfView { unsigned long w0, w1, w2; };

// CCol6CheckBat ctor - self-allocating factory (retail symbol
// __ct__CCol6CheckBat). Returns NULL if the singleton already exists;
// otherwise allocates 0x74 bytes from work memory, constructs the CProcess
// base, fills the callback/field block, stores itself as the singleton and
// registers as a child of `parent`. Mirrors retail control flow: the
// singleton store + Regist run even when the allocation failed (obj stays
// NULL) and the (possibly NULL) object is returned.
CCol6CheckBat* __ct__CCol6CheckBat(CProcess* parent) {
    // Return NULL if the singleton already exists.
    if (lbl_eu_80664230 != 0) {
        return 0;
    }

    CCol6CheckBatCtorShim* obj =
        (CCol6CheckBatCtorShim*)mtl::MemManager::allocate(
            0x74, CWorkThreadSystem::getWorkMem());

    if (obj != 0) {
        __ct__8CProcessFv((CProcess*)obj);
        obj->vtable = (void*)lbl_eu_8052D238;

        // Copy the null member-function pointer into both callback slots.
        // Named-member struct view keeps MWCC from emitting duplicate @l
        // references; retail loads [1],[0],[2] then stores per slot.
        // Distinct member types (unsigned long vs the u32 callback arrays)
        // keep MWCC's type-based aliasing from coupling the loads with the
        // obj stores, matching retail's scheduling.
        const CCol6CheckBatPtmfView* ptmf =
            reinterpret_cast<const CCol6CheckBatPtmfView*>(__ptmf_null);
        u32 ptmfWord1 = ptmf->w1;
        u32 ptmfWord0 = ptmf->w0;
        obj->moveCallbacks[0] = ptmfWord0;
        obj->moveCallbacks[1] = ptmfWord1;
        obj->moveCallbacks[2] = ptmf->w2;
        ptmfWord1 = ptmf->w1;
        ptmfWord0 = ptmf->w0;
        obj->drawCallbacks[0] = ptmfWord0;
        obj->drawCallbacks[1] = ptmfWord1;
        obj->drawCallbacks[2] = ptmf->w2;

        obj->field54 = 0;
        obj->field58 = 0;
        obj->field5C = 0;
        obj->index = -1;
        obj->flag64 = 0;
        obj->flag65 = 0;
        obj->flag66 = 0;
        obj->active = 1;
        obj->field68 = 0;

        obj->vtable = (void*)lbl_eu_8053021C;
        obj->field6C = (u32)(lbl_eu_8053021C + 0x24);
        obj->flag70 = 0;
    }

    lbl_eu_80664230 = (int)(uintptr_t)obj;
    ((CProcess*)obj)->Regist(parent, false);
    return (CCol6CheckBat*)(uintptr_t)lbl_eu_80664230;
}

int func_8015D310() {
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } data;
    data.w = lbl_eu_80667540;
    data.b = lbl_eu_80667544;

    u8 result = func_8009CF8C(0x7fc);

    for (u32 i = 0; i < 5; i++) {
        u8 idx = (u8)i;
        if (result >= data.bytes[idx] && func_8009CF8C(idx + 0x804) == 0) {
            return 1;
        }
    }
    return 0;
}

// func_8015D3A0 - reserve the first free item-box slot whose owned-count
// threshold is met: for each of 5 slots, if the shared counter (id 0x7fc)
// covers the slot's threshold byte and the slot count (id 0x804+i) is zero,
// queue the window index and increment both counters by one/threshold delta.
void func_8015D3A0() {
    // Retail stack layout: t2 (7548/754C) at +0x10 (r31), t1 (7550/7554) at
    // +0x08 (r30); declaring t2 first gives it the higher address.
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } t2, t1;
    t2.w = lbl_eu_80667548;
    t2.b = lbl_eu_8066754C;
    t1.w = lbl_eu_80667550;
    t1.b = lbl_eu_80667554;

    // Retail keeps the running count as a 16-bit variable (re-extended via
    // extsh on every read).
    s16 total = func_8009CF8C(0x7fc);
    for (s32 i = 0; i < 5; i++) {
        u8 need = t2.bytes[i];
        if (total >= need && func_8009CF8C(i + 0x804) == 0) {
            lbl_eu_8066235C = i;
            func_8009D018(i + 0x804, 1);
            // Retail sign-extends the t1 byte before adding to the running
            // total, then re-sign-extends the halfword argument.
            total += (s8)t1.bytes[i];
            func_8009D018(0x7fc, total);
        }
    }
}

// Byte-range shim over the CProcess header + owned fields (0x00..0x70) so the
// ctor can write the vtable slot, the __ptmf_null callback slots and the
// trailing scalars without raw pointer arithmetic (same shape as
// CCol6Invite::Create). The embedded subobjects at 0x74+ go through the real
// CCol6Hint class.
struct CCol6HintCtorShim {
    u8 _00[0x10];
    void* vtable;        // 0x10 - CProcess vtable, overwritten by this ctor
    u8 _14[0x28];        // 0x14-0x3B - rest of CProcess
    u32 callbacks[6];    // 0x3C-0x53 - __ptmf_null callback slots
    u32 field54;         // 0x54
    u32 field58;         // 0x58
    u32 field5C;         // 0x5C
    s32 index;           // 0x60 - init -1
    u8 flag64;           // 0x64
    u8 flag65;           // 0x65
    u8 flag66;           // 0x66
    u8 active;           // 0x67 - init 1
    u32 field68;         // 0x68
    u32 field6C;         // 0x6C - lbl_eu_8053011C + 0x24
    u32 field70;         // 0x70 - lbl_eu_8053011C + 0xac
};

// CCol6Hint ctor - free-function form (retail symbol __ct__CCol6Hint): the
// CProcess base is constructed on the raw object, the vtable slot is written
// manually (temp lbl_eu_8052D238, then the composite vtable lbl_eu_8053011C
// whose sub-tables back the +0x6C/+0x70 interface pointers), the __ptmf_null
// callback slots are copied, and the embedded subobjects (UnkClass_8045F564
// region, CCur18 cursor, CScrollBar) are placement-built before the 0x80-byte
// scratch block is cleared.
// CCol6Hint ctor - extern "C" free-function form (retail emits the bare
// symbol __ct__CCol6Hint; a C++-linkage definition would mangle on the
// factory's call site).
extern "C" CCol6Hint* __ct__CCol6Hint(CCol6Hint* self, CProcess* parent) {
    CCol6HintCtorShim* shim = reinterpret_cast<CCol6HintCtorShim*>(self);
    __ct__8CProcessFv((CProcess*)self);
    shim->vtable = (void*)lbl_eu_8052D238;

    char* finalVt = lbl_eu_8053011C;

    // Copy the null member-function pointer into both callback slots. Retail
    // materializes the __ptmf_null base once via an update-form first load
    // (*p++ folds the symbol's low half into lwzu), then indexes the remaining
    // words relative to the advanced pointer; the composite-vtable offset adds
    // interleave between the reads, and each block stores its second slot
    // first.
    u32* ptmf = __ptmf_null;
    u32 ptmfWord0 = *ptmf++;
    u32 vt6c = (u32)(finalVt + 0x24);
    u32 ptmfWord1 = ptmf[0];
    shim->callbacks[1] = ptmfWord1;
    u32 vt70 = (u32)(finalVt + 0xac);
    shim->callbacks[0] = ptmfWord0;
    u32 ptmfWord2 = ptmf[1];
    shim->callbacks[2] = ptmfWord2;
    ptmfWord0 = ptmf[-1];
    ptmfWord1 = ptmf[0];
    shim->callbacks[4] = ptmfWord1;
    shim->callbacks[3] = ptmfWord0;
    ptmfWord2 = ptmf[1];
    shim->callbacks[5] = ptmfWord2;

    shim->field54 = 0;
    shim->field58 = 0;
    shim->field5C = 0;
    shim->index = -1;
    shim->flag64 = 0;
    shim->flag65 = 0;
    shim->flag66 = 0;
    shim->active = 1;
    shim->field68 = 0;

    shim->vtable = finalVt;
    shim->field6C = vt6c;
    shim->field70 = vt70;

    __ct__17UnkClass_8045F564Fv(
        reinterpret_cast<UnkClass_8045F564*>(self->mMemRegion));
    self->mParentPtr = parent;
    self->mField88 = 0;
    self->mField90 = 0;
    self->mField94 = 0;
    self->mFlag98 = 0;
    self->mState = 0;
    self->mField11C = 0;
    self->mField120 = 0;
    self->mField121 = 0;
    __ct__CCur18(self->mCur18, 0);
    __ct__CScrollBar(&self->mScrollBar, 0);
    memset(self->mUnk9C, 0, 0x80);
    return self;
}

// CCol6Hint dtor (extern "C" free-function form, default -O4,p - retail keeps
// separate stw r31/stw r30 saves): sub-object dtors in +0x13C (CScrollBar),
// +0x124 (CCur18), +0x74 (UnkClass_8045F564) order with flags -1, then the
// CProcess base dtor behind the double-null guard (retail re-checks r30 and
// emits two beq's - the D2-inlined-into-D1 artifact), then flags-based delete.
extern "C" void* __dt__9CCol6HintFv(CCol6Hint* self, int flags) {
    if (self != 0) {
        __dt__10CScrollBarFv(&self->mScrollBar, -1);
        __dt__6CCur18Fv(self->mCur18, -1);
        __dt__17UnkClass_8045F564Fv(self->mMemRegion, -1);
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// CCol6Hint::Init() - load the hint arc file through the embedded IWorkEvent
// interface (the +0x6C subobject; null-this-safe), then install the scroll bar
// by constructing a temp (direction 3), memberwise-copying everything except
// the vptr into the embedded bar, and kicking off the arc read.
void CCol6Hint::Init() {
    // Null-this guard: MWCC keeps r31 = this and conditionally adds the
    // subobject offset, so mirror that single-branch shape here.
    char* selfBytes = reinterpret_cast<char*>(this);
    if (selfBytes != 0) {
        selfBytes += offsetof(CCol6Hint, mField6C);
    }
    IWorkEvent* evt = reinterpret_cast<IWorkEvent*>(selfBytes);
    mField88 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), lbl_eu_80662358, evt, 0, 0);

    // Raw-storage temp so the retail (short) CScrollBar ctor symbol binds at
    // the call site (CScrollBar.hpp's member ctor would emit the mangled
    // __ct__10CScrollBarFb name instead of retail's __ct__CScrollBar). The
    // explicit ~CScrollBar() call emits the 1-arg __dt__10CScrollBarFv form.
    u8 sbStorage[0x40];
    __ct__CScrollBar(reinterpret_cast<CScrollBar*>(sbStorage), 3);
    // Memberwise copy skipping the vptr (retail copies +0x04..+0x3C).
    mScrollBar.mMemRegion.unk0 =
        reinterpret_cast<CScrollBar*>(sbStorage)->mMemRegion.unk0;
    mScrollBar.mMemRegion.unk4 =
        reinterpret_cast<CScrollBar*>(sbStorage)->mMemRegion.unk4;
    mScrollBar.mMemRegion.unk8 =
        reinterpret_cast<CScrollBar*>(sbStorage)->mMemRegion.unk8;
    mScrollBar.mMemRegion.unkC =
        reinterpret_cast<CScrollBar*>(sbStorage)->mMemRegion.unkC;
    mScrollBar.mFileHandle = reinterpret_cast<CScrollBar*>(sbStorage)->mFileHandle;
    mScrollBar.mAccessor = reinterpret_cast<CScrollBar*>(sbStorage)->mAccessor;
    mScrollBar.mLayout = reinterpret_cast<CScrollBar*>(sbStorage)->mLayout;
    mScrollBar.mAnimTransform =
        reinterpret_cast<CScrollBar*>(sbStorage)->mAnimTransform;
    mScrollBar.mReady = reinterpret_cast<CScrollBar*>(sbStorage)->mReady;
    mScrollBar.mVisible = reinterpret_cast<CScrollBar*>(sbStorage)->mVisible;
    mScrollBar.mState = reinterpret_cast<CScrollBar*>(sbStorage)->mState;
    mScrollBar.mActive = reinterpret_cast<CScrollBar*>(sbStorage)->mActive;
    mScrollBar.mAnimOffset = reinterpret_cast<CScrollBar*>(sbStorage)->mAnimOffset;
    mScrollBar.mScrollPosY = reinterpret_cast<CScrollBar*>(sbStorage)->mScrollPosY;
    mScrollBar.mScrollRatio = reinterpret_cast<CScrollBar*>(sbStorage)->mScrollRatio;
    mScrollBar.mThumbHeight = reinterpret_cast<CScrollBar*>(sbStorage)->mThumbHeight;
    mScrollBar.mContentHeight =
        reinterpret_cast<CScrollBar*>(sbStorage)->mContentHeight;
    mScrollBar.mDirection = reinterpret_cast<CScrollBar*>(sbStorage)->mDirection;
    reinterpret_cast<CScrollBar*>(sbStorage)->~CScrollBar();

    func_801F34F4(&mScrollBar);
}

void CCol6Hint::Term() {
    func_801390E0(&mField88);

    // The `if (this)` is the MWCC idiom that splits mr r4 / beq / addi +0x70
    // for the IScnRender subobject passed to removeRenderCB.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mField70);
    }
    reinterpret_cast<CScn*>(mParentPtr)->removeRenderCB(render);

    mFlag98 = 0;
    func_801F35DC(&mScrollBar);
    reinterpret_cast<CCol6CursorView*>(&mCur18)->vf3();

    // D2-inlined double null-check artifact around the anim-host release:
    // retail re-tests mField54 before the +0x08 virtual call.
    if (mAnimHost != 0) {
        if (mAnimHost != 0) {
            mAnimHost->v00(1);
        }
        mAnimHost = 0;
    }

    func_80139124(reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mField8C));
    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion)->func_8045F778();
    lbl_eu_80664234 = 0;
}

// CCol6Hint::Move() - hint-bubble state machine (states 0-5) driving the
// layout anim host (mField54), the scroll bar and the cursor:
//   0 start, 1 slide-in, 2 show label, 3 handoff, 4 slide-out, 5 pick target.
// Every state falls through to the common tail (anim-host update, cursor
// update, scroll-bar update).
void CCol6Hint::Move() {
    // Three-term short-circuit gate: retail emits beq/beq for the first two
    // conditions and an inverted bne-body / b-over-exit pair for the last.
    if (func_8013BE50() == 0 || mFlag98 == 0 ||
        CScrollBar_isVisible(&mScrollBar) == 0) {
        return;
    }
    switch (mState) {
        case 0:
            mState = 1;
            func_80138078(0x6d);
            break;

        case 1:
            // Wait for the out anim to finish, then swap both anims and
            // kick the scroll bar in (open sound).
            if (func_80137444(mField90, lbl_eu_80667558) == 0) break;
            mAnimHost->setAnim(mField90, 0);
            mAnimHost->setAnim(mField94, 1);
            mState = 2;
            {
                ml::CVec3 vec3(lbl_eu_8066755C, lbl_eu_80667560, lbl_eu_80667564);
                func_801F3670(&mScrollBar, &vec3);
            }
            func_801F36BC(&mScrollBar, 5, (u16)mField11C);
            func_801F367C(&mScrollBar);
            func_801F367C(&mScrollBar);
            break;

        case 2:
            // Scroll bar fully in: label the pane with the player index and
            // position the cursor between the label panes.
            if (func_80137444(mField94, lbl_eu_80667558) == 0) break;
            if (func_801F3668(&mScrollBar) == 0) break;
            mState = 3;
            func_801D216C(&mCur18, 1);
            {
                char buf[0x20];
                sprintf(buf, &lbl_eu_80502050[0xe], (s8)mField120 + 1);
                nw4r::lyt::Pane* pane1 =
                    reinterpret_cast<CCol6PaneView*>(mAnimHost->mPane10)
                        ->setText(buf, 1);
                nw4r::lyt::Pane* pane2 =
                    reinterpret_cast<CCol6PaneView*>(mAnimHost->mPane10)
                        ->setText(&lbl_eu_80502050[0x17], 1);
                nw4r::math::VEC3 vec;
                func_80137924(&vec, pane1, pane2, mAnimHost->mPane10);
                reinterpret_cast<CCol6Cur18View*>(&mCur18)->vf04(&vec);
            }
            break;

        case 3:
            func_8015E0BC(this);
            break;

        case 4:
            // Scroll bar sliding out: swap the anims back and wait for done.
            if (func_80137510(mField94, lbl_eu_80667558) == 0) break;
            if (func_801F3668(&mScrollBar) == 0) break;
            mAnimHost->setAnim(mField94, 0);
            mAnimHost->setAnim(mField90, 1);
            mState = 5;
            break;

        case 5:
            // Out anim done: resolve the hint target for the current player
            // pair (mField120 + mField121) and post the index.
            if (func_80137510(mField90, lbl_eu_80667558) == 0) break;
            {
                s32 idx = (s8)mField120 + (s8)mField121;
                u32 entry = mUnk9C[idx];
                if (entry != 0) {
                    mIndex = (u8)func_8013600C(
                        &lbl_eu_80502050[0x26], &lbl_eu_80502050[0x33], entry);
                } else {
                    mIndex = 0xff;
                }
            }
            mFlag64 = 1;
            break;

        default:
            break;
        }

        // Common tail: per-frame updates.
        mAnimHost->update(0);
        func_801D202C(&mCur18);
        func_801F3540(&mScrollBar);
}

// func_8015DB08 - CCol6Hint file-load completion handler: create the scratch
// region + host, detach the arc buffer into a fresh accessor, build the hint
// layout (func_8015DD4C), flag it ready and register the render callback with
// the parent scene (the +0x84 slot holds the stored parent pointer).
extern "C" int func_8015DB08(CCol6Hint* self, CEventFile* event) {
    // Retail branches to a late failure label (bne) rather than skipping the
    // body with a forward beq.
    if (self->mField88 != event->mFileHandle) goto fail;

    // Host's scope ends before the failure label so the fail path skips the
    // Class_8045F858 destructor (retail has no dtor call on that path).
    {
        // Retail holds three values in callee-saved regs across the body
        // (self r29, detached buffer r30, string-pool base r31); name them
        // as locals. The pool base is materialized after getHandleMEM2.
        u8* data;

        mtl::ALLOC_HANDLE mem = mtl::MemManager::getHandleMEM2();
        char* pool = lbl_eu_80502050;
        reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion)
            ->createRegion(mem, 0x20000, &pool[0x38], 0);
        Class_8045F858 host(
            reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion));

        CFileHandle* handle = self->mField88;
        data = handle->mData;
        handle->mData = 0;
        mtl::MemManager::func_80434A4C(false);

        self->mField8C =
            (u32)(uintptr_t)CLibLayout::createArcResourceAccessor();
        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mField8C)
            ->Attach(data, &pool[0x42]);
        func_8015DD4C(self);
        self->mFlag98 = 1;

        IScnRender* render = reinterpret_cast<IScnRender*>(self);
        if (self != 0) {
            render = reinterpret_cast<IScnRender*>(&self->mField70);
        }
        reinterpret_cast<CScn*>(self->mParentPtr)->addRenderCB(render, 0xd, 0);

        self->mField88 = 0;
        reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion)
            ->func_8045F810();
    }
    return 1;
fail:
    return 0;
}

// CCol6Hint::cbRenderBefore() - draw the hint layout, scroll bar and cursor
// through a stack DrawInfo. The gate chain mirrors retail: task-game ready /
// mode bit 0x400000 / game running / arc loaded / bar visible; the goto-body
// form emits the bne-end + beq-body/b-end pair (CSysWinSelect precedent).
void CCol6Hint::cbRenderBefore() {
    // First two gates use the if-&&-goto-body / goto-end chain (bne end +
    // beq body/b end); the running/loaded checks are early returns; the
    // visibility check positively wraps the draw block.
    if (func_800426F0__9CTaskGameFv(getInstance__9CTaskGameFv()) == 0 &&
        (lbl_eu_80663E28 & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    // Splitting running/loaded checks from the visibility check keeps the
    // visibility branch as bne-draw / b-over-return (a flat ||-chain would
    // collapse it to a single beq like the other gates).
    if (func_8013BE50() != 0 && mFlag98 != 0) {
        if (CScrollBar_isVisible(&mScrollBar) != 0) {
            goto draw;
        }
    }
    return;
draw:
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);

    // Raw-storage DrawInfo built/destroyed via the C-ABI ct/dt calls (a C++
    // local would virtual-dispatch its scope-exit destructor). The pointer is
    // re-formed at each use so MWCC keeps it in r1-relative addis instead of
    // a callee-saved register.
    u8 drawInfo[0x60];
    __ct__Q34nw4r3lyt8DrawInfoFv(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
    func_80137250(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
    func_80137038(reinterpret_cast<nw4r::lyt::Layout*>(mAnimHost),
                  reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]), 0, 1);
    func_801F35B0(&mScrollBar, reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
    func_801D20B0(&mCur18, reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]));
    __dt__Q34nw4r3lyt8DrawInfoFv(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo[0]), -1);
}

// IUIWindow return type for the window factories below (declared extern "C"
// in CUIWindowManager.hpp).
class IUIWindow;

// func_8015DCD0 - hint-window factory: allocate 0x17c bytes of work memory,
// construct the CCol6Hint into it and register it under pParent. The store
// to the singleton global and Regist run even when allocation failed (obj
// stays NULL); returns the (possibly NULL) singleton.
extern "C" IUIWindow* func_8015DCD0(CProcess* pParent, CScn* pScene) {
    if (lbl_eu_80664234 != 0) {
        return 0;
    }

    CCol6Hint* obj = reinterpret_cast<CCol6Hint*>(
        mtl::MemManager::allocate(0x17c, CWorkThreadSystem::getWorkMem()));
    if (obj != 0) {
        // Reassign the (self-returning) ctor result so MWCC keeps obj in r3
        // across the call instead of spilling it to a callee-saved register.
        obj = __ct__CCol6Hint(obj, reinterpret_cast<CProcess*>(pScene));
    }
    lbl_eu_80664234 = (int)(uintptr_t)obj;
    reinterpret_cast<CProcess*>((uintptr_t)lbl_eu_80664234)->Regist(pParent, false);
    return reinterpret_cast<IUIWindow*>((uintptr_t)lbl_eu_80664234);
}

// func_8015DD4C - builds the CCol6Hint layout: attaches the arc, loads the two
// anim transforms, binds the device font into the root pane, rebuilds the
// embedded cursor from a stack temp, labels the count panes, then scans the
// item-box threshold tables to fill the per-slot entries (mUnk9C) and label
// the six slots with the resolved strings.
extern "C" void func_8015DD4C(CCol6Hint* self) {
    // Build the layout into the +0x54 slot and load the two anim transforms
    // (the +0x54 anim host IS the layout object).
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        reinterpret_cast<nw4r::lyt::Layout**>(&self->mAnimHost),
        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mField8C),
        &lbl_eu_80502050[0x46]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost),
        &self->mField90,
        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mField8C),
        &lbl_eu_80502050[0x56]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost),
        &self->mField94,
        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mField8C),
        &lbl_eu_80502050[0x69]);

    // Bind the device font into the layout's root pane.
    nw4r::lyt::Pane* rootPane = reinterpret_cast<CCol6LayoutView*>(
        reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost))->mpRootPane;
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        1, reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost));
    func_8013676C(rootPane,
                  reinterpret_cast<CCol6FontView*>(fontObj)->vf7());

    // Start the two anims (setAnim at +0x2C) and tick once (+0x38).
    reinterpret_cast<CCol6AnimHostView*>(self->mAnimHost)
        ->setAnim(self->mField94, 0);
    reinterpret_cast<CCol6AnimHostView*>(self->mAnimHost)
        ->setAnim(self->mField90, 1);
    reinterpret_cast<CCol6AnimHostView*>(self->mAnimHost)->update(0);

    // Rebuild the embedded cursor from a stack temp (data fields only).
    u8 tempC[0x18];
    __ct__CCur18(tempC, func_801355F4());
    CCol6Cur18Data* mc = reinterpret_cast<CCol6Cur18Data*>(self->mCur18);
    CCol6Cur18Data* tc = reinterpret_cast<CCol6Cur18Data*>(tempC);
    mc->f_04 = tc->f_04;
    mc->f_08 = tc->f_08;
    mc->f_0c = tc->f_0c;
    mc->f_10 = tc->f_10;
    mc->f_14 = tc->f_14;
    mc->f_15 = tc->f_15;
    __dt__6CCur18Fv(tempC, -1);
    reinterpret_cast<CCol6Cur18View*>(self->mCur18)->vf02();

    // Label the two count panes.
    char* s1 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x1c);
    func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost),
                  &lbl_eu_80502050[0x81], s1, 0);
    char* s2 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0xb);
    func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost),
                  &lbl_eu_80502050[0x8b], s2, 0);

    // Label the item pane and clear its visibility flag.
    nw4r::lyt::Pane* pane = reinterpret_cast<CCol6PaneView*>(
        reinterpret_cast<CCol6LayoutView*>(
            reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost))
            ->mpRootPane)
        ->setText(&lbl_eu_80502050[0x94], 1);
    reinterpret_cast<CCol6PaneFlagView*>(pane)->mFlag &= 0xFE;

    func_8003AA34();
    void* fp = getFP__FPCc(&lbl_eu_80502050[0x26]);
    func_8003B1EC(fp);

    // Two 7-byte threshold tables (u32 + u16 + u8, read as byte arrays).
    struct {
        u32 w;
        u16 h;
        u8 b;
    } tbl1, tbl2;
    tbl1.w = lbl_eu_80667568;
    tbl1.h = lbl_eu_8066756C;
    tbl1.b = lbl_eu_8066756E;
    tbl2.w = lbl_eu_80667570;
    tbl2.h = lbl_eu_80667574;
    tbl2.b = lbl_eu_80667576;

    self->mField11C = 0;

    // For each of the 7 count buckets, find the largest value whose owned
    // count fits the bucket, storing it into the entry table.
    for (u32 i = 0; i < 7; i++) {
        u8 val1 = ((u8*)&tbl1)[(u8)i];
        u32 rid = (u8)i + 0x7fc;
        u8 val2 = ((u8*)&tbl2)[(u8)i];
        while (val1 >= val2) {
            s32 c = func_801361E8(fp, &lbl_eu_80502050[0x9e], val1);
            if ((c & 0xff) <= (s32)func_8009CF8C(rid)) {
                self->mUnk9C[self->mField11C] = val1;
                self->mField11C++;
                break;
            }
            val1--;
        }
    }

    // Reserved-slot entries (0x804 count == 1).
    for (u32 i = 0; i < 5; i++) {
        if (func_8009CF8C((u8)i + 0x804) == 1) {
            self->mUnk9C[self->mField11C] = i + 0x2e;
            self->mField11C++;
        }
    }

    self->mField11C++;

    // Label the six slots: entry table row = i + mField121; an empty row uses
    // the fallback string when the previous row is populated, else no text.
    for (s32 i = 0; i < 6; i++) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_80502050[0xe], i + 1);
        char* str = 0;
        u32 idx = i + (s8)self->mField121;
        s32 entry = (s32)self->mUnk9C[idx];
        if (entry != 0) {
            str = func_80136190(&lbl_eu_80502050[0x26],
                                &lbl_eu_80502050[0xa3], entry);
        } else if (self->mUnk9C[idx - 1] != 0) {
            str = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 5);
        }
        func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost),
                      buf, str, 0);
    }
}

// func_8015E0BC - CCol6Hint::Move state-3 pad handler: held up/down switch the
// state and reposition the scroll position; the four short-press buttons
// scroll the hint selection with wrap (single row down/up via the row pair,
// page down/up via the page pair), re-labelling the six slot panes and
// repositioning the cursor each time.
extern "C" void func_8015E0BC(CCol6Hint* self) {
    CCol6PadData* pad = (CCol6PadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int sels, confirm, up2, down2, up, down;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        up      = (pad->mField04 >> 21) & 1;
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x2001) != 0;
        up2     = (pad->mField104 & 0x4002) != 0;
        down2   = ((pad->mField104 & 0x8000) | (pad->mField104 & 0x10000000)) != 0;
        down    = (pad->mField04 >> 22) & 1;
    } else {
        up      = (pad->mField04 >> 4) & 1;
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x2001) != 0;
        up2     = (pad->mField104 & 0x4002) != 0;
        down2   = ((pad->mField104 & 0x8000) | (pad->mField104 & 0x10000000)) != 0;
        down    = (pad->mField04 >> 5) & 1;
    }

    if (up) {
        self->mState = 4;
        func_801D216C(&self->mCur18, 0);
        func_801F369C(&self->mScrollBar);
        func_80138078(3);
    } else if (down) {
        self->mState = 4;
        func_801D216C(&self->mCur18, 0);
        func_801F369C(&self->mScrollBar);
        if (self->mField11C >= 5) {
            self->mField120 = 4;
            self->mField121 = (u8)(self->mField11C - 5);
        } else {
            self->mField120 = (u8)(self->mField11C - 1);
            self->mField121 = 0;
        }
        func_80138078(6);
    } else if (sels) {
        // Row down with wrap: decrement the row pair, wrapping to the
        // (mField11C-based) page end when both go negative.
        self->mField120 = (u8)(self->mField120 - 1);
        if ((s8)self->mField120 < 0) {
            self->mField120 = 0;
            self->mField121 = (u8)(self->mField121 - 1);
            if ((s8)self->mField121 < 0) {
                if (self->mField11C >= 5) {
                    self->mField120 = 4;
                    self->mField121 = (u8)(self->mField11C - 5);
                } else {
                    self->mField120 = (u8)(self->mField11C - 1);
                    self->mField121 = 0;
                }
            }
        }
        // Label the six slots and reposition the cursor.
        for (s32 i = 0; i < 6; i++) {
            char buf[0x20];
            sprintf(buf, &lbl_eu_80502050[0xe], i + 1);
            char* str = 0;
            s32 idx = i + (s8)self->mField121;
            s32 entry = (s32)self->mUnk9C[idx];
            // Nested form: retail tests entry==0 on the fall-through path.
            if (entry == 0) {
                if (self->mUnk9C[idx - 1] != 0) {
                    str = func_80136190(lbl_eu_80502050,
                                        &lbl_eu_80502050[0x9], 5);
                }
            } else {
                str = func_80136190(&lbl_eu_80502050[0x26],
                                    &lbl_eu_80502050[0xa3], entry);
            }
            func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost),
                          buf, str, 0);
        }
        char buf2[0x20];
        sprintf(buf2, &lbl_eu_80502050[0xe], (s8)self->mField120 + 1);
        // Root pane re-derived for every use - keeping it in a named local
        // across the virtual calls costs an extra callee-saved register.
        nw4r::lyt::Pane* pane1 = reinterpret_cast<CCol6PaneView*>(
            reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)->mpRootPane)
            ->setText(buf2, 1);
        nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(
            reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)->mpRootPane)
            ->setText(&lbl_eu_80502050[0x17], 1);
        nw4r::math::VEC3 vec;
        func_80137924(&vec, pane1, pane2,
                      reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)
                          ->mpRootPane);
        reinterpret_cast<CCol6Cur18View*>(self->mCur18)->vf04(&vec);
        func_801F3850(&self->mScrollBar, (u16)(s8)self->mField121);
        func_80138078(1);
    } else if (down2) {
        // Row up with wrap: increment the row pair, resetting to 0/0 when the
        // page end is reached.
        if (self->mField11C >= 5) {
            self->mField120 = (u8)(self->mField120 + 1);
            if ((s8)self->mField120 >= 5) {
                self->mField120 = 4;
                self->mField121 = (u8)(self->mField121 + 1);
                if ((s8)self->mField121 >= (s32)self->mField11C - 4) {
                    self->mField120 = 0;
                    self->mField121 = 0;
                }
            }
        } else {
            self->mField120 = (u8)(self->mField120 + 1);
            if ((s8)self->mField120 >= (s32)self->mField11C) {
                self->mField120 = 0;
            }
        }
        for (s32 i = 0; i < 6; i++) {
            char buf[0x20];
            sprintf(buf, &lbl_eu_80502050[0xe], i + 1);
            char* str = 0;
            s32 idx = i + (s8)self->mField121;
            s32 entry = (s32)self->mUnk9C[idx];
            if (entry == 0) {
                if (self->mUnk9C[idx - 1] != 0) {
                    str = func_80136190(lbl_eu_80502050,
                                        &lbl_eu_80502050[0x9], 5);
                }
            } else {
                str = func_80136190(&lbl_eu_80502050[0x26],
                                    &lbl_eu_80502050[0xa3], entry);
            }
            func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost),
                          buf, str, 0);
        }
        char buf2[0x20];
        sprintf(buf2, &lbl_eu_80502050[0xe], (s8)self->mField120 + 1);
        nw4r::lyt::Pane* pane1 = reinterpret_cast<CCol6PaneView*>(
            reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)->mpRootPane)
            ->setText(buf2, 1);
        nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(
            reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)->mpRootPane)
            ->setText(&lbl_eu_80502050[0x17], 1);
        nw4r::math::VEC3 vec;
        func_80137924(&vec, pane1, pane2,
                      reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)
                          ->mpRootPane);
        reinterpret_cast<CCol6Cur18View*>(self->mCur18)->vf04(&vec);
        func_801F3850(&self->mScrollBar, (u16)(s8)self->mField121);
        func_80138078(1);
    } else if (confirm) {
        // Page down: jump one page back with wrap.
        if (self->mField11C >= 5) {
            self->mField121 = (u8)(self->mField121 - 5);
            if ((s8)self->mField121 < 0) {
                self->mField120 = (u8)((u8)self->mField121 + 4);
                self->mField121 = 0;
                if ((s8)self->mField120 < 0) {
                    self->mField120 = 0;
                }
            }
        } else {
            self->mField120 = 0;
            self->mField121 = 0;
        }
        for (s32 i = 0; i < 6; i++) {
            char buf[0x20];
            sprintf(buf, &lbl_eu_80502050[0xe], i + 1);
            char* str = 0;
            s32 idx = i + (s8)self->mField121;
            s32 entry = (s32)self->mUnk9C[idx];
            if (entry == 0) {
                if (self->mUnk9C[idx - 1] != 0) {
                    str = func_80136190(lbl_eu_80502050,
                                        &lbl_eu_80502050[0x9], 5);
                }
            } else {
                str = func_80136190(&lbl_eu_80502050[0x26],
                                    &lbl_eu_80502050[0xa3], entry);
            }
            func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost),
                          buf, str, 0);
        }
        char buf2[0x20];
        sprintf(buf2, &lbl_eu_80502050[0xe], (s8)self->mField120 + 1);
        nw4r::lyt::Pane* pane1 = reinterpret_cast<CCol6PaneView*>(
            reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)->mpRootPane)
            ->setText(buf2, 1);
        nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(
            reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)->mpRootPane)
            ->setText(&lbl_eu_80502050[0x17], 1);
        nw4r::math::VEC3 vec;
        func_80137924(&vec, pane1, pane2,
                      reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)
                          ->mpRootPane);
        reinterpret_cast<CCol6Cur18View*>(self->mCur18)->vf04(&vec);
        func_801F3850(&self->mScrollBar, (u16)(s8)self->mField121);
        func_80138078(1);
    } else if (up2) {
        // Page up: jump one page forward with wrap.
        if (self->mField11C >= 5) {
            self->mField121 = (u8)(self->mField121 + 5);
            if ((s8)self->mField121 > (s32)self->mField11C - 5) {
                self->mField120 = (u8)((u8)self->mField121 -
                                      ((s32)self->mField11C - 5));
                self->mField121 = (u8)(self->mField11C - 5);
                if ((s8)self->mField120 >= 5) {
                    self->mField120 = 4;
                }
            }
        } else {
            self->mField120 = (u8)(self->mField11C - 1);
            self->mField121 = 0;
            if ((s8)self->mField120 < 0) {
                self->mField120 = 0;
            }
        }
        for (s32 i = 0; i < 6; i++) {
            char buf[0x20];
            sprintf(buf, &lbl_eu_80502050[0xe], i + 1);
            char* str = 0;
            s32 idx = i + (s8)self->mField121;
            s32 entry = (s32)self->mUnk9C[idx];
            if (entry == 0) {
                if (self->mUnk9C[idx - 1] != 0) {
                    str = func_80136190(lbl_eu_80502050,
                                        &lbl_eu_80502050[0x9], 5);
                }
            } else {
                str = func_80136190(&lbl_eu_80502050[0x26],
                                    &lbl_eu_80502050[0xa3], entry);
            }
            func_80136B4C(reinterpret_cast<nw4r::lyt::Layout*>(self->mAnimHost),
                          buf, str, 0);
        }
        char buf2[0x20];
        sprintf(buf2, &lbl_eu_80502050[0xe], (s8)self->mField120 + 1);
        nw4r::lyt::Pane* pane1 = reinterpret_cast<CCol6PaneView*>(
            reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)->mpRootPane)
            ->setText(buf2, 1);
        nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(
            reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)->mpRootPane)
            ->setText(&lbl_eu_80502050[0x17], 1);
        nw4r::math::VEC3 vec;
        func_80137924(&vec, pane1, pane2,
                      reinterpret_cast<CCol6LayoutView*>(self->mAnimHost)
                          ->mpRootPane);
        reinterpret_cast<CCol6Cur18View*>(self->mCur18)->vf04(&vec);
        func_801F3850(&self->mScrollBar, (u16)(s8)self->mField121);
        func_80138078(1);
    }
}

// Byte-range shim over the CProcess header + owned fields (0x00..0x70) so the
// ctor can write the vtable slot, the __ptmf_null callback slots and the
// trailing scalars without raw pointer arithmetic (same shape as the
// CCol6Hint / CCol6CheckBat ctors). The embedded subobjects at 0x74+ go
// through the real CCol6System class.
struct CCol6SystemCtorShim {
    u8 _00[0x10];
    void* vtable;        // 0x10 - CProcess vtable, overwritten by this ctor
    u8 _14[0x28];        // 0x14-0x3B - rest of CProcess
    u32 callbacks[6];    // 0x3C-0x53 - __ptmf_null callback slots
    u32 field54;         // 0x54
    u32 field58;         // 0x58
    u32 field5C;         // 0x5C
    s32 index;           // 0x60 - init -1
    u8 flag64;           // 0x64
    u8 flag65;           // 0x65
    u8 flag66;           // 0x66
    u8 active;           // 0x67 - init 1
    u32 field68;         // 0x68
    u32 field6C;         // 0x6C - lbl_eu_8053001C + 0x24
    u32 field70;         // 0x70 - lbl_eu_8053001C + 0xac
};

// CCol6System ctor - free-function form (retail symbol __ct__CCol6System): the
// CProcess base is constructed on the raw object, the vtable slot is written
// manually (temp lbl_eu_8052D238, then the composite vtable lbl_eu_8053001C
// whose sub-tables back the +0x6C/+0x70 interface pointers), the __ptmf_null
// callback slots are copied, and the embedded subobjects (UnkClass_8045F564
// region, two CCur18 cursors, two CSysWin windows) are placement-built before
// the camera/vector block is initialized from the lbl_eu_80667564 float.
// CCol6System ctor - extern "C" free-function form (retail bare symbol,
// same reason as __ct__CCol6Hint).
extern "C" CCol6System* __ct__CCol6System(CCol6System* self, CProcess* parent) {
    CCol6SystemCtorShim* shim = reinterpret_cast<CCol6SystemCtorShim*>(self);
    __ct__8CProcessFv((CProcess*)self);
    shim->vtable = (void*)lbl_eu_8052D238;

    char* finalVt = lbl_eu_8053001C;

    // Copy the null member-function pointer into both callback slots (same
    // shape as the CCol6Hint ctor: retail materializes the __ptmf_null base
    // once via an update-form first load (*p++ folds the symbol's low half
    // into lwzu), then indexes the remaining words relative to the advanced
    // pointer; each block stores its second slot first).
    u32* ptmf = __ptmf_null;
    u32 ptmfWord0 = *ptmf++;
    u32 vt6c = (u32)(finalVt + 0x24);
    u32 ptmfWord1 = ptmf[0];
    shim->callbacks[1] = ptmfWord1;
    u32 vt70 = (u32)(finalVt + 0xac);
    shim->callbacks[0] = ptmfWord0;
    u32 ptmfWord2 = ptmf[1];
    shim->callbacks[2] = ptmfWord2;
    ptmfWord0 = ptmf[-1];
    ptmfWord1 = ptmf[0];
    shim->callbacks[4] = ptmfWord1;
    shim->callbacks[3] = ptmfWord0;
    ptmfWord2 = ptmf[1];
    shim->callbacks[5] = ptmfWord2;

    shim->field54 = 0;
    shim->field58 = 0;
    shim->field5C = 0;
    shim->index = -1;
    shim->flag64 = 0;
    shim->flag65 = 0;
    shim->flag66 = 0;
    shim->active = 1;
    shim->field68 = 0;

    shim->vtable = finalVt;
    shim->field6C = vt6c;
    shim->field70 = vt70;

    __ct__17UnkClass_8045F564Fv(
        reinterpret_cast<UnkClass_8045F564*>(self->mMemRegion));
    self->mScn = reinterpret_cast<CScn*>(parent);
    self->mFileHandle = 0;
    self->mAnim90 = 0;
    self->mAnim94 = 0;
    self->mAnim98 = 0;
    self->mAnim9C = 0;
    self->mFlagA0 = 0;
    self->mFlagA1 = 0;
    self->mFieldA2 = 0;
    self->mFieldA4 = 0;
    self->mPadA5[0] = 0;
    self->mPadA5[1] = 0;
    __ct__CCur18(self->mCur1, 0);
    __ct__CCur18(self->mCur2, 0);
    __ct__CSysWin(self->mSysWin1, 0);
    __ct__CSysWin(self->mSysWin2, 0);

    // Camera/vector block: every CVec3 is built from a stack temp
    // CVec3(f,f,f) and copied memberwise (MWCC routes the bits through GPRs,
    // lfs/stfs-to-stack + lwz/stw), while the trailing float/byte pairs are
    // stored directly.
    f32 f = lbl_eu_80667564;
    // Camera/vector block: every CVec3 is built from a stack temp
    // CVec3(f,f,f) and copied memberwise (MWCC routes the bits through GPRs,
    // lfs/stfs-to-stack + lwz/stw), while the trailing float/byte pairs are
    // stored directly.
    // NOTE (open item): retail assigns the eight anon-temp slots non-
    // monotonically (sp+20,2c,08,14 then +50,5c,38,44 - two passes of
    // [e2,e3,e0,e1] over one contiguous 0x60 block); every source form tried
    // (statement reorder, named locals both decl orders, local CVec3[8],
    // placement-new element ctors, record-pointer loop) yields descending
    // slots or extra out-of-line/round-trip code instead.
    self->mCamPos = ml::CVec3(f, f, f);
    self->mCamLookAt = ml::CVec3(f, f, f);
    self->mVec168 = ml::CVec3(f, f, f);
    self->mVec174 = ml::CVec3(f, f, f);
    self->mField180 = f;
    self->mField184 = 0;
    self->mVec188 = ml::CVec3(f, f, f);
    self->mVec194 = ml::CVec3(f, f, f);
    self->mVec1A0 = ml::CVec3(f, f, f);
    self->mVec1AC = ml::CVec3(f, f, f);
    self->mField1B8 = f;
    self->mField1BC = 0;
    return self;
}

// extern "C" free-function form (CCol6Hint precedent): sub-dtors in retail
// order (mSysWin2/mSysWin1/mCur2/mCur1/mMemRegion, flags -1), double-null-
// guarded CProcess base dtor, flags-based delete; default -O4,p.
extern "C" void* __dt__11CCol6SystemFv(CCol6System* self, int flags) {
    if (self != 0) {
        __dt__7CSysWinFv(&self->mSysWin2, -1);
        __dt__7CSysWinFv(&self->mSysWin1, -1);
        __dt__6CCur18Fv(self->mCur2, -1);
        __dt__6CCur18Fv(self->mCur1, -1);
        __dt__17UnkClass_8045F564Fv(self->mMemRegion, -1);
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// CCol6System::Init() - load the System.arc file through the embedded IWorkEvent
// interface (the +0x6C subobject; null-this-safe), then rebuild both embedded
// CSysWin bodies: construct a stack temp, memberwise-copy the non-vtable fields,
// destroy the temp, and dispatch the layout-build virtual at vtable +0x88.
void CCol6System::Init() {
    // Null-this-safe IWorkEvent subobject: default to `this` (0 when null) and
    // conditionally add +0x6C (retail: mr r30,r31 / beq / addi r30,r3,0x6c).
    IWorkEvent* evt = reinterpret_cast<IWorkEvent*>(this);
    if (this != 0) {
        evt = reinterpret_cast<IWorkEvent*>(&mField6C);
    }
    mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), lbl_eu_80662358, evt, 0, 0);

    // Rebuild window 1 (direction 2): copy every non-vtable field so MWCC
    // emits the retail lwz/stw + lbz/stb sequence (skips +0x29..0x2B padding).
    u8 tempW[0x3C];
    __ct__CSysWin(tempW, 2);
    CSysWinData* w1 = reinterpret_cast<CSysWinData*>(mSysWin1);
    CSysWinData* t1 = reinterpret_cast<CSysWinData*>(tempW);
    w1->f_04 = t1->f_04;
    w1->f_08 = t1->f_08;
    w1->f_0c = t1->f_0c;
    w1->f_10 = t1->f_10;
    w1->f_14 = t1->f_14;
    w1->f_18 = t1->f_18;
    w1->f_1c = t1->f_1c;
    w1->f_20 = t1->f_20;
    w1->f_24 = t1->f_24;
    w1->f_28 = t1->f_28;
    w1->f_2c = t1->f_2c;
    w1->f_30 = t1->f_30;
    w1->f_34 = t1->f_34;
    w1->f_35 = t1->f_35;
    w1->f_36 = t1->f_36;
    w1->f_37 = t1->f_37;
    w1->f_38 = t1->f_38;
    w1->f_39 = t1->f_39;
    __dt__7CSysWinFv(tempW, -1);
    reinterpret_cast<CSysWinView*>(mSysWin1)->v20();

    // Rebuild window 2 the same way (direction 0).
    u8 tempW2[0x3C];
    __ct__CSysWin(tempW2, 0);
    CSysWinData* w2 = reinterpret_cast<CSysWinData*>(mSysWin2);
    CSysWinData* t2 = reinterpret_cast<CSysWinData*>(tempW2);
    w2->f_04 = t2->f_04;
    w2->f_08 = t2->f_08;
    w2->f_0c = t2->f_0c;
    w2->f_10 = t2->f_10;
    w2->f_14 = t2->f_14;
    w2->f_18 = t2->f_18;
    w2->f_1c = t2->f_1c;
    w2->f_20 = t2->f_20;
    w2->f_24 = t2->f_24;
    w2->f_28 = t2->f_28;
    w2->f_2c = t2->f_2c;
    w2->f_30 = t2->f_30;
    w2->f_34 = t2->f_34;
    w2->f_35 = t2->f_35;
    w2->f_36 = t2->f_36;
    w2->f_37 = t2->f_37;
    w2->f_38 = t2->f_38;
    w2->f_39 = t2->f_39;
    __dt__7CSysWinFv(tempW2, -1);
    reinterpret_cast<CSysWinView*>(mSysWin2)->v20();
}

// CCol6System::Term() - wait for the VI draw to finish, detach the render
// callback, release both embedded windows/cursors and the layout/accessor/
// scratch region, then clear the CCol6System singleton global.
void CCol6System::Term() {
    CDeviceVI::waitForDrawDone();
    func_801390E0(&mFileHandle);

    // The `if (this)` is the MWCC idiom that splits mr r4 / beq / addi +0x70
    // for the IScnRender subobject passed to removeRenderCB.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScn->removeRenderCB(render);

    mFlagA0 = 0;
    reinterpret_cast<CCol6CursorView*>(&mCur1)->vf3();
    reinterpret_cast<CCol6CursorView*>(&mCur2)->vf3();
    func_8022B7F4(&mSysWin1);
    func_8022B7F4(&mSysWin2);

    // The outer guard plus delete's own null-check reproduce retail's two beq's
    // on the same CR0 test (MWCC D2-inlined-into-D1 artifact).
    if (mpLayout != 0) {
        delete mpLayout;
        mpLayout = 0;
    }

    func_80139124(mArcAccessor);
    mArcAccessor = 0;
    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion)->func_8045F778();
    lbl_eu_80664238 = 0;
}

// Cursor/window position sync, shared by the anim-complete states of Move().
// Retail inlines this helper at each call site (each copy gets its own stack
// slots for pos/buf/vec), which drives Move()'s 0x1f0 frame layout.
inline void CCol6System::updateCursorPos() {
    if (CSysWin_getUnk34(&mSysWin1) != 0) {
        nw4r::math::VEC3 pos;
        func_8022C1B4(reinterpret_cast<u8*>(&pos), &mSysWin1, mPadA5[1]);
        reinterpret_cast<CCol6Cur18View*>(mCur2)->vf04(&pos);
    } else {
        char buf[0x20];
        if (mFieldA4 >= 6) {
            sprintf(buf, &lbl_eu_80502050[0xa9], (s8)mPadA5[0] + 1);
        } else {
            sprintf(buf, &lbl_eu_80502050[0xe], (s8)mPadA5[0] + 1);
        }
        nw4r::lyt::Pane* rootPane =
            reinterpret_cast<CCol6LayoutView*>(mpLayout)->mpRootPane;
        nw4r::lyt::Pane* pane1 =
            reinterpret_cast<CCol6PaneView*>(rootPane)->setText(buf, 1);
        nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(rootPane)
            ->setText(&lbl_eu_80502050[0x17], 1);
        nw4r::math::VEC3 vec;
        func_80137924(&vec, pane1, pane2, rootPane);
        reinterpret_cast<CCol6Cur18View*>(mCur1)->vf04(&vec);
    }
}

// CCol6System::Move() - main state machine (mFieldA4 switch, states 0..0x27):
// anim-wait states drive the four layout anims and reposition the cursors,
// input states dispatch to the item-box handlers, and the camera/fade states
// post the camera moves through the CfGameManager API. All states fall
// through to the common tail (anim-host tick, cursor/window updates).
void CCol6System::Move() {
    if (func_8013BE50() == 0) return;
    if (mFlagA0 == 0) return;
    if (CSysWin_isReady(&mSysWin1) == 0) return;
    // Retail splits gate 4 into a two-instruction bne-body / b-exit pair
    // (same shape as cbRenderBefore), so use the goto split here too.
    if (CSysWin_isReady(&mSysWin2) != 0) goto body;
    return;
body:

    switch (mFieldA4) {
    case 0:
        mFieldA4 = 1;
        func_80138078(0x6d);
        break;

    case 1:
        // Out anim finished: swap the pair and open the box.
        if (func_80137444(mAnim90, lbl_eu_80667558) == 0) break;
        mFieldA4 = 2;
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim98, 0);
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim9C, 0);
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim90, 0);
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim94, 1);
        break;

    case 2:
        // In anim finished: position the player-selection cursor.
        if (func_80137444(mAnim94, lbl_eu_80667558) == 0) break;
        mFieldA4 = 3;
        updateCursorPos();
        break;

    case 3:
        func_80161178(this);
        break;

    case 4:
        // In anim done: swap back and close.
        if (func_80137510(mAnim94, lbl_eu_80667558) == 0) break;
        mFieldA4 = 5;
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim94, 0);
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim98, 0);
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim9C, 0);
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim90, 1);
        break;

    case 5:
        // Out anim done: resolve the target index from the player slot.
        if (func_80137510(mAnim90, lbl_eu_80667558) == 0) break;
        mIndex = 0xff;
        if ((s8)mPadA5[0] == 2) mIndex = 1;
        else if ((s8)mPadA5[0] == 1) mIndex = 2;
        mPadA5[0] = 0;
        mFlag64 = 1;
        break;

    case 6:
        // Second-layer out anim: switch to the box anim and reset the cursor.
        if (func_80137444(mAnim98, lbl_eu_80667558) == 0) break;
        mFieldA4 = 7;
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim90, 0);
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim94, 0);
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim98, 0);
        reinterpret_cast<CCol6AnimHostView*>(mpLayout)
            ->setAnim(mAnim9C, 1);
        mPadA5[0] = 0;
        updateCursorPos();
        break;

    case 7:
        func_8016169C(this);
        break;

    case 8:
        // Box anim done: resolve target.
        if (func_80137444(mAnim9C, lbl_eu_80667558) == 0) break;
        mIndex = 0xff;
        mFlag64 = 1;
        break;

    case 9:
        // Close the box: reset the player and position the cursor.
        if (func_80137510(mAnim98, lbl_eu_80667558) == 0) break;
        mFieldA4 = 3;
        mPadA5[0] = 0;
        updateCursorPos();
        break;

    case 0xa:
        // Window 1 active: switch to the slot-selection state.
        if (CSysWin_isActive(&mSysWin1) == 0) break;
        mFieldA4 = 0xb;
        mPadA5[1] = 0;
        updateCursorPos();
        break;

    case 0xb:
        func_80161C5C(this);
        break;

    case 0xc:
        // Window 1 active: confirm posts the item-box sync, up re-opens.
        if (CSysWin_isActive(&mSysWin1) == 0) break;
        if ((s8)mPadA5[1] == 0) {
            mFieldA4 = 0xd;
            func_80135464(0, 0, lbl_eu_80667578, lbl_eu_80667578,
                          lbl_eu_80667578);
            func_80160EE4((u32)(uintptr_t)this, (s8)mPadA5[0]);
        } else {
            mFieldA4 = 7;
            updateCursorPos();
        }
        break;

    case 0xd:
        func_80162000(this);
        break;

    case 0xe:
        // Fade idle: resolve target and flag.
        if (func_80113E1C() != 0) break;
        mIndex = 0xff;
        mFlag64 = 1;
        break;

    case 0xf:
        // Sound done: fade out and move the camera to the stored position.
        if (func_801BFAE4(mFieldA2) != 0) break;
        mFieldA4 = 0x10;
        func_80135464(2, 0, lbl_eu_80667578, lbl_eu_80667578,
                      lbl_eu_80667578);
        func_80082008__Q22cf13CfGameManagerFv(9, 0, 0, 0, 0);
        func_80082088__Q22cf13CfGameManagerFv(
            0, &mCamPos, &mCamLookAt, 0, lbl_eu_80667564);
        func_80082060__Q22cf13CfGameManagerFv();
        break;

    case 0x10:
        mFieldA4 = 0x11;
        func_80082008__Q22cf13CfGameManagerFv(
            9, 0, (u32)(s32)mField180, mField184, 0);
        func_80082088__Q22cf13CfGameManagerFv(
            (u16)(s32)mField180, &mVec168, &mVec174, mField184,
            lbl_eu_80667564);
        func_80082060__Q22cf13CfGameManagerFv();
        break;

    case 0x11:
        if (func_80113E1C() != 0) break;
        mFieldA4 = 0x12;
        break;

    case 0x12:
        if (func_80082104__Q22cf13CfGameManagerFv() != 0) break;
        mFieldA4 = 0x13;
        func_80082008__Q22cf13CfGameManagerFv(9, 0, 0, 0, 0);
        func_80082088__Q22cf13CfGameManagerFv(
            0, &mVec188, &mVec194, 0, lbl_eu_80667564);
        func_80082060__Q22cf13CfGameManagerFv();
        break;

    case 0x13:
        if (func_80082104__Q22cf13CfGameManagerFv() != 0) break;
        mFieldA4 = 0x14;
        func_80082008__Q22cf13CfGameManagerFv(
            9, 0, (u32)(s32)mField1B8, mField1BC, 0);
        func_80082088__Q22cf13CfGameManagerFv(
            (u16)(s32)mField1B8, &mVec1A0, &mVec1AC, mField1BC,
            lbl_eu_80667564);
        func_80082060__Q22cf13CfGameManagerFv();
        break;

    case 0x14:
        func_80162C40(this);
        break;

    case 0x15:
        // Fade idle: fade out and clear the block flags.
        if (func_80113E1C() == 0) break;
        if (func_80113E24(func_80113E1C()) != 0) break;
        mFieldA4 = 0xe;
        func_80135464(2, 0, lbl_eu_80667578, lbl_eu_80667578,
                      lbl_eu_80667578);
        func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
        break;

    case 0x16:
        func_80162DB4(this);
        break;

    case 0x17:
        mFieldA4 = 0x18;
        func_80082008__Q22cf13CfGameManagerFv(
            9, 0, (u32)(s32)mField180, mField184, 0);
        func_80082088__Q22cf13CfGameManagerFv(
            (u16)(s32)mField180, &mVec168, &mVec174, mField184,
            lbl_eu_80667564);
        func_80082060__Q22cf13CfGameManagerFv();
        break;

    case 0x18:
        if (func_80113E1C() != 0) break;
        mFieldA4 = 0x19;
        break;

    case 0x19:
        if (func_80082104__Q22cf13CfGameManagerFv() != 0) break;
        mFieldA4 = 0x1a;
        func_80082008__Q22cf13CfGameManagerFv(9, 0, 0, 0, 0);
        func_80082088__Q22cf13CfGameManagerFv(
            0, &mVec188, &mVec194, 0, lbl_eu_80667564);
        func_80082060__Q22cf13CfGameManagerFv();
        break;

    case 0x1a:
        if (func_80082104__Q22cf13CfGameManagerFv() != 0) break;
        mFieldA4 = 0x1b;
        func_80082008__Q22cf13CfGameManagerFv(
            9, 0, (u32)(s32)mField1B8, mField1BC, 0);
        func_80082088__Q22cf13CfGameManagerFv(
            (u16)(s32)mField1B8, &mVec1A0, &mVec1AC, mField1BC,
            lbl_eu_80667564);
        func_80082060__Q22cf13CfGameManagerFv();
        break;

    case 0x1b:
        // Window-2 join banner for the current player count.
        if (func_80082104__Q22cf13CfGameManagerFv() != 0) break;
        mFieldA4 = 0x20;
        {
            u8 cnt = (u8)func_8009CF8C(0x802);
            char* s1 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                     cnt + 0x67);
            char* s2 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                     cnt + 0x6c);
            ml::FixStr<128> buf;
            buf.format(&lbl_eu_80502050[0xb5], s1, s2);
            func_8022B9B4(&mSysWin2, buf.mString, 0);
            func_8022B8B8(&mSysWin2);
            func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
                1, 0x134, 0, 0, lbl_eu_80667558);
        }
        break;

    case 0x1c:
        func_80162EF8(this);
        break;

    case 0x1d:
        // Window 2 active: pad-confirm closes it; otherwise reopen.
        if (CSysWin_getUnk34(&mSysWin2) != 0) {
            if (CSysWin_isActive(&mSysWin2) != 0) {
                CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
                u32 cond;
                if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                    cond = (pad->mPressedButtonFlags & 0x200000) != 0;
                } else {
                    cond = (pad->mPressedButtonFlags & 0x10) != 0;
                }
                if (cond != 0) func_8022B8E4(&mSysWin2);
            }
        } else {
            mFieldA4 = 7;
        }
        break;

    case 0x1e:
        func_80163614(this);
        break;

    case 0x1f:
        func_8016378C(this);
        break;

    case 0x20:
        func_801638C0(this);
        break;

    case 0x21:
        if (CSysWin_getUnk34(&mSysWin2) != 0) {
            if (CSysWin_isActive(&mSysWin2) != 0) {
                CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
                u32 cond;
                if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                    cond = (pad->mPressedButtonFlags & 0x200000) != 0;
                } else {
                    cond = (pad->mPressedButtonFlags & 0x10) != 0;
                }
                if (cond != 0) func_8022B8E4(&mSysWin2);
            }
        } else {
            mFieldA4 = 0x24;
            char* s = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                     0x7c);
            u16 id = func_8013606C(&lbl_eu_80502050[0xbd],
                                   &lbl_eu_80502050[0xc9],
                                   (s8)mPadA5[0] + 1);
            func_8013E2E0(id, 0, 0, 0, 0, 1, 0, 1, 0);
        }
        break;

    case 0x22:
        func_80163AF4(this);
        break;

    case 0x23:
        if (CSysWin_getUnk34(&mSysWin2) != 0) {
            if (CSysWin_isActive(&mSysWin2) != 0) {
                CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
                u32 cond;
                if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                    cond = (pad->mPressedButtonFlags & 0x200000) != 0;
                } else {
                    cond = (pad->mPressedButtonFlags & 0x10) != 0;
                }
                if (cond != 0) func_8022B8E4(&mSysWin2);
            }
        } else {
            mFieldA4 = 0x26;
            char* s = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                     0x7c);
            u16 id = func_8013606C(&lbl_eu_80502050[0xbd],
                                   &lbl_eu_80502050[0xc9], 5);
            func_8013E2E0(id, 0, 0, 0, 0, 1, 0, 1, 0);
        }
        break;

    case 0x24:
        // Per-slot threshold scan (same shape as CCol6Invite::Move).
        if (func_801B481C() != 0) break;
        {
            union {
                struct {
                    u32 w;
                    u8 b;
                };
                u8 bytes[5];
            } data;
            data.w = lbl_eu_80667540;
            data.b = lbl_eu_80667544;

            u8 result = (u8)func_8009CF8C(0x7fc);

            u8 found = 0;
            for (u32 i = 0; i < 5; i++) {
                u8 idx = (u8)i;
                if (result >= data.bytes[idx] &&
                    func_8009CF8C(idx + 0x804) == 0) {
                    found = 1;
                    break;
                }
            }

            if (found != 0) {
                union {
                    struct {
                        u32 w;
                        u8 b;
                    };
                    u8 bytes[5];
                } data2;
                union {
                    struct {
                        u32 w;
                        u8 b;
                    };
                    u8 bytes[5];
                } data3;
                data2.w = lbl_eu_80667548;
                data2.b = lbl_eu_8066754C;
                data3.w = lbl_eu_80667550;
                data3.b = lbl_eu_80667554;

                s16 val = (s16)func_8009CF8C(0x7fc);
                for (u32 i = 0; i < 5; i++) {
                    if (val >= data2.bytes[i] &&
                        func_8009CF8C(i + 0x804) == 0) {
                        lbl_eu_8066235C = (s32)i;
                        func_8009D018(i + 0x804, 1);
                        val = (s16)(val + (s8)data3.bytes[i]);
                        func_8009D018(0x7fc, val);
                    }
                }
            }
            mFieldA4 = 0x15;
            func_80135464(0, 0, lbl_eu_80667578, lbl_eu_80667578,
                          lbl_eu_80667578);
        }
        break;

    case 0x25:
        if (CSysWin_getUnk34(&mSysWin2) != 0) {
            if (CSysWin_isActive(&mSysWin2) != 0) {
                CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
                u32 cond;
                if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                    cond = (pad->mPressedButtonFlags & 0x200000) != 0;
                } else {
                    cond = (pad->mPressedButtonFlags & 0x10) != 0;
                }
                if (cond != 0) func_8022B8E4(&mSysWin2);
            }
        } else {
            mFieldA4 = 0x28;
            char* s = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                     0x80);
            func_8022B9B4(&mSysWin2, s, 0);
            func_8022B8B8(&mSysWin2);
        }
        break;

    case 0x26:
    case 0x27:
    case 0x28:
        // Window 2 active: pad-confirm closes it; otherwise fade out.
        if (CSysWin_getUnk34(&mSysWin2) != 0) {
            if (CSysWin_isActive(&mSysWin2) != 0) {
                CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
                u32 cond;
                if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                    cond = (pad->mPressedButtonFlags & 0x200000) != 0;
                } else {
                    cond = (pad->mPressedButtonFlags & 0x10) != 0;
                }
                if (cond != 0) func_8022B8E4(&mSysWin2);
            }
        } else {
            mFieldA4 = 0x15;
            func_80135464(0, 0, lbl_eu_80667578, lbl_eu_80667578,
                          lbl_eu_80667578);
        }
        break;

    default:
        break;
    }

    // Common tail: per-frame updates.
    reinterpret_cast<CCol6AnimHostView*>(mpLayout)->update(0);
    func_801D202C(&mCur1);
    func_801D202C(&mCur2);
    func_8022B748(&mSysWin1);
    func_8022B748(&mSysWin2);
}

// func_80160118 - second layout file-load completion handler: create the
// scratch region + host, detach the arc buffer into a fresh accessor, build
// the layout (func_80160370), flag it ready and register the render callback.
extern "C" int func_80160118(CCol6System* self, CEventFile* event) {
    // Same shape as func_8015DB08: late failure label (no dtor on that
    // path), host scoped to the success block, pool base materialized after
    // getHandleMEM2.
    if (self->mFileHandle != event->mFileHandle) goto fail;

    {
        u8* data;

        mtl::ALLOC_HANDLE mem = mtl::MemManager::getHandleMEM2();
        char* pool = lbl_eu_80502050;
        reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion)
            ->createRegion(mem, 0x20000, &pool[0xd0], 0);
        Class_8045F858 host(
            reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion));

        CFileHandle* handle = self->mFileHandle;
        data = handle->mData;
        handle->mData = 0;
        mtl::MemManager::func_80434A4C(false);

        self->mArcAccessor = CLibLayout::createArcResourceAccessor();
        self->mArcAccessor->Attach(data, &pool[0x42]);
        func_80160370(self);
        self->mFlagA0 = 1;

        IScnRender* render = reinterpret_cast<IScnRender*>(self);
        if (self != 0) {
            render = reinterpret_cast<IScnRender*>(&self->mScnRender);
        }
        self->mScn->addRenderCB(render, 0xd, 0);

        self->mFileHandle = 0;
        reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion)
            ->func_8045F810();
    }
    return 1;
fail:
    return 0;
}

// cbRenderBefore - draw both embedded windows through a stack DrawInfo.
// Gates 1-3 are single beq-to-exit branches; gate 4 must stay as the
// two-instruction bne-body / b-exit pair, so use the goto-body/goto-end
// split (CCol6Hint::cbRenderBefore precedent).
void CCol6System::cbRenderBefore() {
    if (func_8013BE50() == 0) goto end;
    if (mFlagA0 == 0) goto end;
    if (CSysWin_isReady(&mSysWin1) == 0) goto end;
    if (CSysWin_isReady(&mSysWin2) != 0) goto draw;
    goto end;
end:
    return;
draw:
    GXSetZMode((GXBool)0, GX_NEVER, (GXBool)0);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250(&drawInfo);
    if (mFlagA1 == 0) {
        func_80137038(mpLayout, &drawInfo, 0, 1);
        func_801D20B0(&mCur1, &drawInfo);
        func_8022B7C8(&mSysWin1, &drawInfo);
        func_801D20B0(&mCur2, &drawInfo);
    }
    func_8022B7C8(&mSysWin2, &drawInfo);
}

// func_801602F4 - item-box system-window factory (same shape as
// func_8015DCD0, but constructs a 0x240-byte CCol6System and uses its own
// singleton global lbl_eu_80664238).
extern "C" IUIWindow* func_801602F4(CProcess* pParent, CScn* pScene) {
    if (lbl_eu_80664238 != 0) {
        return 0;
    }

    CCol6System* obj = reinterpret_cast<CCol6System*>(
        mtl::MemManager::allocate(0x240, CWorkThreadSystem::getWorkMem()));
    if (obj != 0) {
        obj = __ct__CCol6System(obj, reinterpret_cast<CProcess*>(pScene));
    }
    lbl_eu_80664238 = (int)(uintptr_t)obj;
    reinterpret_cast<CProcess*>((uintptr_t)lbl_eu_80664238)->Regist(pParent, false);
    return reinterpret_cast<IUIWindow*>((uintptr_t)lbl_eu_80664238);
}

// func_80160370 - builds the second CCol6System layout: attaches the arc
// ("out" + four anims), binds the device font, labels the 21 count panes when
// the font value is present, rebuilds both embedded cursors, labels the
// player-count rows and summary panes, then refreshes the per-player item box
// (func_80160A6C) and writes the reserved-slot count string.
extern "C" void func_80160370(CCol6System* self) {
    self->mPadA5[0] = 0;

    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &self->mpLayout, self->mArcAccessor, &lbl_eu_80502050[0x46]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->mpLayout, &self->mAnim90, self->mArcAccessor,
        &lbl_eu_80502050[0x56]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->mpLayout, &self->mAnim94, self->mArcAccessor,
        &lbl_eu_80502050[0x69]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->mpLayout, &self->mAnim98, self->mArcAccessor,
        &lbl_eu_80502050[0xdc]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->mpLayout, &self->mAnim9C, self->mArcAccessor,
        &lbl_eu_80502050[0xf1]);

    // Bind the device font into the layout's root pane.
    nw4r::lyt::Pane* rootPane =
        reinterpret_cast<CCol6LayoutView*>(self->mpLayout)->mpRootPane;
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
        1, self->mpLayout);
    func_8013676C(rootPane, reinterpret_cast<CCol6FontView*>(fontObj)->vf7());

    // If the font value is present, label all 21 count panes with it.
    u32 fontVal = func_801355BC();
    if (fontVal != 0) {
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x105], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x10f], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x11d], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x12b], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x135], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x13f], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x149], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x153], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x161], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x16f], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x17d], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x18b], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x199], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x1a4], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x1af], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x1c0], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x1d1], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x1e2], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x1f3], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x204], fontVal);
        func_801368C0(self->mpLayout, &lbl_eu_80502050[0x212], fontVal);
    }

    // Start the four anims (setAnim at +0x2C) and tick once (+0x38).
    reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
        ->setAnim(self->mAnim94, 0);
    reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
        ->setAnim(self->mAnim98, 0);
    reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
        ->setAnim(self->mAnim9C, 0);
    reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
        ->setAnim(self->mAnim90, 1);
    reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)->update(0);

    // Rebuild cursor 1 (mCur1 at +0xA8) from a stack temp (data fields only).
    u8 tempC[0x18];
    __ct__CCur18(tempC, func_801355F4());
    CCol6Cur18Data* mc1 = reinterpret_cast<CCol6Cur18Data*>(self->mCur1);
    CCol6Cur18Data* tc1 = reinterpret_cast<CCol6Cur18Data*>(tempC);
    mc1->f_04 = tc1->f_04;
    mc1->f_08 = tc1->f_08;
    mc1->f_0c = tc1->f_0c;
    mc1->f_10 = tc1->f_10;
    mc1->f_14 = tc1->f_14;
    mc1->f_15 = tc1->f_15;
    __dt__6CCur18Fv(tempC, -1);
    reinterpret_cast<CCol6Cur18View*>(self->mCur1)->vf02();

    // Rebuild cursor 2 (mCur2 at +0xC0) from a stack temp (data fields only).
    u8 tempC2[0x18];
    __ct__CCur18(tempC2, func_801355F4());
    CCol6Cur18Data* mc2 = reinterpret_cast<CCol6Cur18Data*>(self->mCur2);
    CCol6Cur18Data* tc2 = reinterpret_cast<CCol6Cur18Data*>(tempC2);
    mc2->f_04 = tc2->f_04;
    mc2->f_08 = tc2->f_08;
    mc2->f_0c = tc2->f_0c;
    mc2->f_10 = tc2->f_10;
    mc2->f_14 = tc2->f_14;
    mc2->f_15 = tc2->f_15;
    __dt__6CCur18Fv(tempC2, -1);
    reinterpret_cast<CCol6Cur18View*>(self->mCur2)->vf02();

    // Label the two count panes.
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x81],
                  func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x1c),
                  0);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x8b],
                  func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x1),
                  0);

    // Three slot labels: formatted pane name, then the per-slot name char.
    for (s32 i = 0; i < 3; i++) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_80502050[0xe], i + 1);
        func_80136B4C(self->mpLayout, buf,
                      func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                    i + 2),
                      0);
    }

    // Blank rows and static summary labels.
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x220],
                  &lbl_eu_80502050[0x227], 0);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x228],
                  &lbl_eu_80502050[0x227], 0);
    char* s1e = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x1e);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x22f], s1e, 0);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x237],
                  func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 5), 0);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x241],
                  func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x43),
                  0);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x24e],
                  func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x47),
                  0);

    char* s46 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x46);

    // Four player-count rows: count pane + two sub-labels per row.
    for (s32 i = 0; i < 4; i++) {
        u8 idx = (u8)(i + 1);
        u32 c = func_8009CF8C(i + 0x7fe);
        char buf[0x20];
        sprintf(buf, &lbl_eu_80502050[0x25e], idx);
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mpLayout, buf, c);
        sprintf(buf, &lbl_eu_80502050[0x26a], idx);
        func_80136B4C(self->mpLayout, buf, s46, 0);
        sprintf(buf, &lbl_eu_80502050[0x275], idx);
        func_80136B4C(self->mpLayout, buf,
                      func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                    i + 0x3d),
                      0);
    }

    func_80160A6C(self, (s8)self->mPadA5[0]);

    // Remaining summary rows.
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x281],
                  func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x15),
                  0);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x289],
                  func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x16),
                  0);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x105],
                  func_80136190(&lbl_eu_80502050[0x293],
                                &lbl_eu_80502050[0x9], 0x16),
                  0);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x298],
                  func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x17),
                  0);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x2a2],
                  func_80136190(&lbl_eu_80502050[0x293],
                                &lbl_eu_80502050[0x9], 0x17),
                  0);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x2ac],
                  func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x18),
                  0);

    // Player counts on the two slots.
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mpLayout,
                                            &lbl_eu_80502050[0x10f],
                                            func_8009CF8C(0x7fc));
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mpLayout,
                                            &lbl_eu_80502050[0x11d],
                                            func_8009CF8C(0x7fd));

    // Reserved-slot count string: 0x19 if any reserved slot is populated.
    s32 cnt = 0x1a;
    for (u32 i = 0; i < 5; i++) {
        u8 idx = (u8)i;
        if (func_8009CF8C(idx + 0x804) == 1) {
            cnt = 0x19;
            break;
        }
    }
    char* s2b6 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], cnt);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x2b6], s2b6, 0);
}

// func_80160A6C - refresh the item-box layout for a player slot: write the
// four player-count labels, then either (count < 5) fill the per-slot item
// panes from the item table, or blank them; an out-of-range slot blanks
// everything.
extern "C" void func_80160A6C(CCol6System* self, s32 playerIdx) {
    // Four player-count labels (0x7fe + 0..3).
    for (s32 i = 0; i < 4; i++) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_80502050[0x25e], i + 1);
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mpLayout, buf,
                                                func_8009CF8C(i + 0x7fe));
    }

    if (playerIdx < 4) {
        s32 f = (s32)func_8009CF8C((u32)(playerIdx + 0x7fe));
        char* str = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x293],
                                  f + playerIdx * 6 + 0x1f);
        func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x2c0], str, 0);

        if (f < 5) {
            u32 n = (u32)(f + playerIdx * 5 + 1);
            u16 id = func_8013606C(&lbl_eu_80502050[0x2cc],
                                   &lbl_eu_80502050[0x2dd], n);
            func_80136910__FPQ34nw4r3lyt6LayoutPcUc(
                self->mpLayout, &lbl_eu_80502050[0x1a4], id * 100);
            char* s3 = func_80136190(&lbl_eu_80502050[0x2e3],
                                     &lbl_eu_80502050[0x9], 3);
            func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x199], s3, 0);
            func_80136910__FPQ34nw4r3lyt6LayoutPcUc(
                self->mpLayout, &lbl_eu_80502050[0x212], func_801571FC());
            char* s4 = func_80136190(&lbl_eu_80502050[0x2e3],
                                     &lbl_eu_80502050[0x9], 3);
            func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x204], s4, 0);

            // Per-slot rows: 4 format strings + a lookup for the count value.
            for (s32 i = 0; i < 5; i++) {
                u8 idx = (u8)(i + 1);
                char buf148[0x20];
                sprintf(buf148, &lbl_eu_80502050[0x2ec], idx);
                char buf128[0x20];
                sprintf(buf128, &lbl_eu_80502050[0x2fd], idx);
                char buf108[0x20];
                sprintf(buf108, &lbl_eu_80502050[0x30d], idx);
                char bufE8[0x20];
                sprintf(bufE8, &lbl_eu_80502050[0x320], idx);
                u16 id2 = func_8013606C(&lbl_eu_80502050[0x2cc], bufE8, n);
                if (id2 != 0) {
                    func_801392E4(id2);
                    func_80139358(id2);
                    char* name = func_801394D4(id2);
                    char bufC8[0x20];
                    sprintf(bufC8, &lbl_eu_80502050[0x32b], idx);
                    u8 v = (u8)func_8013600C(&lbl_eu_80502050[0x2cc], bufC8,
                                             n);
                    func_80136B4C(self->mpLayout, buf148, name, 0);
                    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mpLayout,
                                                            buf128, v);
                    u32 cnt = func_80158068(id2);
                    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mpLayout,
                                                            buf108, cnt);
                } else {
                    func_80136B4C(self->mpLayout, buf148,
                                  &lbl_eu_80502050[0x227], 0);
                    func_80136B4C(self->mpLayout, buf128,
                                  &lbl_eu_80502050[0x227], 0);
                    func_80136B4C(self->mpLayout, buf108,
                                  &lbl_eu_80502050[0x227], 0);
                }
            }
        } else {
            func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x199],
                          &lbl_eu_80502050[0x227], 0);
            func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x1a4],
                          &lbl_eu_80502050[0x227], 0);
            func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x204],
                          &lbl_eu_80502050[0x227], 0);
            func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x212],
                          &lbl_eu_80502050[0x227], 0);
            for (s32 i = 0; i < 5; i++) {
                u8 idx = (u8)(i + 1);
                char bufA8[0x20];
                sprintf(bufA8, &lbl_eu_80502050[0x2ec], idx);
                char buf88[0x20];
                sprintf(buf88, &lbl_eu_80502050[0x2fd], idx);
                char buf68[0x20];
                sprintf(buf68, &lbl_eu_80502050[0x30d], idx);
                func_80136B4C(self->mpLayout, bufA8,
                              &lbl_eu_80502050[0x227], 0);
                func_80136B4C(self->mpLayout, buf88,
                              &lbl_eu_80502050[0x227], 0);
                func_80136B4C(self->mpLayout, buf68,
                              &lbl_eu_80502050[0x227], 0);
            }
        }
    } else {
        func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x2c0],
                      &lbl_eu_80502050[0x227], 0);
        func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x199],
                      &lbl_eu_80502050[0x227], 0);
        func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x1a4],
                      &lbl_eu_80502050[0x227], 0);
        func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x204],
                      &lbl_eu_80502050[0x227], 0);
        func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x212],
                      &lbl_eu_80502050[0x227], 0);
        for (s32 i = 0; i < 5; i++) {
            u8 idx = (u8)(i + 1);
            char buf48[0x20];
            sprintf(buf48, &lbl_eu_80502050[0x2ec], idx);
            char buf28[0x20];
            sprintf(buf28, &lbl_eu_80502050[0x2fd], idx);
            char buf8[0x20];
            sprintf(buf8, &lbl_eu_80502050[0x30d], idx);
            func_80136B4C(self->mpLayout, buf48, &lbl_eu_80502050[0x227], 0);
            func_80136B4C(self->mpLayout, buf28, &lbl_eu_80502050[0x227], 0);
            func_80136B4C(self->mpLayout, buf8, &lbl_eu_80502050[0x227], 0);
        }
    }
}

// func_80160EE4 - item-box sync ladder: for a given item slot (<4) with a
// populated table row, format per-slot sub-ids, resolve the item instances and
// reconcile counts, calling the instance-sync virtual or the delta adjuster
// when the counted stack differs from the table value. The first parameter
// (r3) is unused by the retail body - the slot index arrives in r4.
void func_80160EE4(u32 unused, s32 arg) {
    if (arg >= 4) return;
    s32 f = (s32)func_8009CF8C((u32)(arg + 0x7fe));
    if (f >= 5) return;
    // Retail accumulates arg*4 into a temp (compound +=), then forms
    // n = f + temp + 1 so the adds land in n's callee-saved home register.
    s32 tmp = arg * 4;
    tmp += arg;
    s32 n = f + tmp + 1;
    u16 id = (u16)func_8013606C(
        &lbl_eu_80502050[0x2cc], &lbl_eu_80502050[0x2dd],
        (u32)n);
    func_80157184((s32)func_801571FC() - (s32)id * 100);

    char buf1[0x20];
    char buf2[0x20];
    for (s32 i = 0; i < 5; i++) {
        u8 idx = (u8)(i + 1);
        sprintf(buf1, &lbl_eu_80502050[0x320], idx);
        u16 id2 = (u16)func_8013606C(
            &lbl_eu_80502050[0x2cc], buf1, (u32)n);
        if (id2 == 0) continue;
        sprintf(buf2, &lbl_eu_80502050[0x32b], idx);
        u8 v = (u8)func_8013600C(
            &lbl_eu_80502050[0x2cc], buf2, (u32)n);
        void* item = func_801587E8(id2);
        if (item == 0) continue;
        s32 count = (s32)func_80158068(id2);
        if (count == (s32)v) {
            CCol6ItemInstancesView* inst =
                reinterpret_cast<CCol6ItemInstancesView*>(
                    CItem_initItemImplInstances(item));
            inst->vf4(item);
        } else {
            func_80158118(item, id2, (u32)(count - v));
        }
    }
}

// func_80161024 - validate the item box for the player index at +0xA5: the
// total owned count must cover the needed quantity (table id * 100), and each
// of the five sub-slots must have its item present with a sufficient count.
extern "C" int func_80161024(CCol6System* self) {
    u8 f = (u8)func_8009CF8C((u32)((s8)self->mPadA5[0] + 0x7fe));
    u16 id = func_8013606C(
        &lbl_eu_80502050[0x2cc], &lbl_eu_80502050[0x2dd],
        (u32)((s32)f + (s8)self->mPadA5[0] * 5 + 1));
    if ((u32)func_801571FC() < (u32)id * 100) {
        return 0;
    }

    char buf1[0x20];
    char buf2[0x20];
    for (s32 i = 0; i < 5; i++) {
        u8 sub = (u8)(i + 1);
        sprintf(buf1, &lbl_eu_80502050[0x320], sub);
        u16 id2 = (u16)func_8013606C(
            &lbl_eu_80502050[0x2cc], buf1,
            (u32)((s32)f + (s8)self->mPadA5[0] * 5 + 1));
        if (id2 == 0) continue;
        sprintf(buf2, &lbl_eu_80502050[0x32b], sub);
        u8 v = func_8013600C(
            &lbl_eu_80502050[0x2cc], buf2,
            (u32)((s32)f + (s8)self->mPadA5[0] * 5 + 1));
        void* item = func_801587E8(id2);
        if (item == 0) return 0;
        if ((s32)func_80158068(id2) < (s32)v) return 0;
    }
    return 1;
}

// func_80161178 - item-box input handling: up opens the box for player 1 (or
// flips the anim pair for other players), down flips the anim pair, and the
// select/confirm pad bits scroll the player index with wrap.
void func_80161178(CCol6System* self) {
    CCol6PadData* pad = (CCol6PadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int up, down, sels, confirm;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 21) & 1;
        down    = (pad->mField04 >> 22) & 1;
    } else {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 4) & 1;
        down    = (pad->mField04 >> 5) & 1;
    }

    if (up) {
        if ((s8)self->mPadA5[0] == 0) {
            // Player 1: open the box (all four anims on) and refresh it.
            self->mFieldA4 = 6;
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim9C, 0);
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim90, 0);
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim94, 0);
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim98, 1);
            func_80160A6C(self, 0);
            func_80138078(0x6d);
        } else {
            // Other players: switch the anim pair.
            self->mFieldA4 = 4;
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim98, 0);
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim9C, 0);
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim90, 0);
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim94, 1);
        }
        func_801D216C(&self->mCur1, 0);
        if (self->mPadA5[0] == 2) self->mPadA5[0] = 3;
        func_80138078(3);
    } else if (down) {
        self->mFieldA4 = 4;
        reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
            ->setAnim(self->mAnim98, 0);
        reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
            ->setAnim(self->mAnim9C, 0);
        reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
            ->setAnim(self->mAnim90, 0);
        reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
            ->setAnim(self->mAnim94, 1);
        func_801D216C(&self->mCur1, 0);
        self->mPadA5[0] = 3;
        func_80138078(6);
    } else if (sels) {
        // Player index down (wrap to 2).
        self->mPadA5[0] = self->mPadA5[0] - 1;
        if ((s8)self->mPadA5[0] < 0) self->mPadA5[0] = 2;
        if (CSysWin_getUnk34(&self->mSysWin1) != 0) {
            u8 tmp[0x18];
            func_8022C1B4(tmp, &self->mSysWin1, self->mPadA5[1]);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur2)->vf04(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, &lbl_eu_80502050[0xa9],
                        (s8)self->mPadA5[0] + 1);
            } else {
                sprintf(buf, &lbl_eu_80502050[0xe], (s8)self->mPadA5[0] + 1);
            }
            nw4r::lyt::Pane* pane1 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(buf, 1);
            nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(&lbl_eu_80502050[0x17], 1);
            nw4r::math::VEC3 vec;
            func_80137924(
                &vec, pane1, pane2,
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)->mpRootPane);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur1)->vf04(&vec);
        }
        func_80138078(1);
    } else if (confirm != 0) {
        // Player index up (wrap to 0).
        self->mPadA5[0] = self->mPadA5[0] + 1;
        if ((s8)self->mPadA5[0] > 2) self->mPadA5[0] = 0;
        if (CSysWin_getUnk34(&self->mSysWin1) != 0) {
            u8 tmp[0x18];
            func_8022C1B4(tmp, &self->mSysWin1, self->mPadA5[1]);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur2)->vf04(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, &lbl_eu_80502050[0xa9],
                        (s8)self->mPadA5[0] + 1);
            } else {
                sprintf(buf, &lbl_eu_80502050[0xe], (s8)self->mPadA5[0] + 1);
            }
            nw4r::lyt::Pane* pane1 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(buf, 1);
            nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(&lbl_eu_80502050[0x17], 1);
            nw4r::math::VEC3 vec;
            func_80137924(
                &vec, pane1, pane2,
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)->mpRootPane);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur1)->vf04(&vec);
        }
        func_80138078(1);
    }
}

// func_8016169C - item-box input handling (5-player): up on player 5 opens
// the box; other up/down inputs switch players or open the per-player box;
// select/confirm scroll the player index with wrap.
void func_8016169C(CCol6System* self) {
    CCol6PadData* pad = (CCol6PadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int up, down, sels, confirm;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 21) & 1;
        down    = (pad->mField04 >> 22) & 1;
    } else {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 4) & 1;
        down    = (pad->mField04 >> 5) & 1;
    }

    if (up) {
        s32 pid = (s8)self->mPadA5[0];
        if (pid == 4) {
            // Player 5: open the box (all four anims on).
            self->mFieldA4 = 9;
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim90, 0);
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim94, 0);
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim9C, 0);
            reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
                ->setAnim(self->mAnim98, 1);
            func_801D216C(&self->mCur1, 0);
            func_80138078(3);
        } else if (func_8009CF8C(pid + 0x7fe) < 5) {
            if (func_80161024(self) != 0) {
                // Fill window 1 with the player's box name and open it.
                char* str1 = func_80136190(lbl_eu_80502050,
                                           &lbl_eu_80502050[0x9],
                                           pid + 0x48);
                char* str2 = func_80136190(lbl_eu_80502050,
                                           &lbl_eu_80502050[0x9], 7);
                char* str3 = func_80136190(lbl_eu_80502050,
                                           &lbl_eu_80502050[0x9], 8);
                func_8022B9B4(&self->mSysWin1, str1, 0);
                func_8022BF6C(&self->mSysWin1, str2, str3);
                func_8022BFC8(&self->mSysWin1, 0);
                func_8022B8B8(&self->mSysWin1);
                func_801D216C(&self->mCur1, 1);
                self->mFieldA4 = 0xa;
                func_80138078(3);
            } else {
                func_80138078(5);
            }
        } else {
            // Box full: show the window-2 message.
            char* str = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                      0xc);
            func_8022B9B4(&self->mSysWin2, str, 0);
            func_8022BFC8(&self->mSysWin2, 1);
            func_8022B8B8(&self->mSysWin2);
            self->mFieldA4 = 0x1d;
            func_80138078(5);
        }
    } else if (down) {
        self->mFieldA4 = 9;
        reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
            ->setAnim(self->mAnim90, 0);
        reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
            ->setAnim(self->mAnim94, 0);
        reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
            ->setAnim(self->mAnim9C, 0);
        reinterpret_cast<CCol6AnimHostView*>(self->mpLayout)
            ->setAnim(self->mAnim98, 1);
        func_801D216C(&self->mCur1, 0);
        func_80138078(6);
    } else if (sels) {
        // Player index down (wrap to 4) and refresh the box.
        self->mPadA5[0] = self->mPadA5[0] - 1;
        if ((s8)self->mPadA5[0] < 0) self->mPadA5[0] = 4;
        func_80160A6C(self, (s8)self->mPadA5[0]);
        if (CSysWin_getUnk34(&self->mSysWin1) != 0) {
            u8 tmp[0x18];
            func_8022C1B4(tmp, &self->mSysWin1, self->mPadA5[1]);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur2)->vf04(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, &lbl_eu_80502050[0xa9],
                        (s8)self->mPadA5[0] + 1);
            } else {
                sprintf(buf, &lbl_eu_80502050[0xe], (s8)self->mPadA5[0] + 1);
            }
            nw4r::lyt::Pane* pane1 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(buf, 1);
            nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(&lbl_eu_80502050[0x17], 1);
            nw4r::math::VEC3 vec;
            func_80137924(
                &vec, pane1, pane2,
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)->mpRootPane);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur1)->vf04(&vec);
        }
        func_80138078(1);
    } else if (confirm != 0) {
        // Player index up (wrap to 0) and refresh the box.
        self->mPadA5[0] = self->mPadA5[0] + 1;
        if ((s8)self->mPadA5[0] > 4) self->mPadA5[0] = 0;
        func_80160A6C(self, (s8)self->mPadA5[0]);
        if (CSysWin_getUnk34(&self->mSysWin1) != 0) {
            u8 tmp[0x18];
            func_8022C1B4(tmp, &self->mSysWin1, self->mPadA5[1]);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur2)->vf04(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, &lbl_eu_80502050[0xa9],
                        (s8)self->mPadA5[0] + 1);
            } else {
                sprintf(buf, &lbl_eu_80502050[0xe], (s8)self->mPadA5[0] + 1);
            }
            nw4r::lyt::Pane* pane1 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(buf, 1);
            nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(&lbl_eu_80502050[0x17], 1);
            nw4r::math::VEC3 vec;
            func_80137924(
                &vec, pane1, pane2,
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)->mpRootPane);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur1)->vf04(&vec);
        }
        func_80138078(1);
    }
}

// func_80161C5C - item-box window interaction: the select/confirm pad bits
// scroll the slot counter (with wrap), the up/down buttons open the box for
// the current player; window-open updates reposition the cursor.
void func_80161C5C(CCol6System* self) {
    CCol6PadData* pad = (CCol6PadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int up, down, sels, confirm;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 21) & 1;
        down    = (pad->mField04 >> 22) & 1;
    } else {
        sels    = (pad->mField104 & 0x8004) != 0;
        confirm = (pad->mField104 & 0x10000) | (pad->mField104 & 0x8);
        up      = (pad->mField04 >> 4) & 1;
        down    = (pad->mField04 >> 5) & 1;
    }

    if (sels) {
        // Scroll the slot counter down (wrap to 1), reposition the cursor.
        self->mPadA5[1] = self->mPadA5[1] - 1;
        if ((s8)self->mPadA5[1] < 0) self->mPadA5[1] = 1;
        if (CSysWin_getUnk34(&self->mSysWin1) != 0) {
            u8 tmp[0x18];
            func_8022C1B4(tmp, &self->mSysWin1, self->mPadA5[1]);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur2)->vf04(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, &lbl_eu_80502050[0xa9],
                        (s8)self->mPadA5[0] + 1);
            } else {
                sprintf(buf, &lbl_eu_80502050[0xe], (s8)self->mPadA5[0] + 1);
            }
            nw4r::lyt::Pane* pane1 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(buf, 1);
            nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(&lbl_eu_80502050[0x17], 1);
            nw4r::math::VEC3 vec;
            func_80137924(
                &vec, pane1, pane2,
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)->mpRootPane);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur1)->vf04(&vec);
        }
        func_80138078(1);
    } else if (confirm != 0) {
        // Scroll the slot counter up (wrap to 0).
        self->mPadA5[1] = self->mPadA5[1] + 1;
        if ((s8)self->mPadA5[1] > 1) self->mPadA5[1] = 0;
        if (CSysWin_getUnk34(&self->mSysWin1) != 0) {
            u8 tmp[0x18];
            func_8022C1B4(tmp, &self->mSysWin1, self->mPadA5[1]);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur2)->vf04(
                reinterpret_cast<nw4r::math::VEC3*>(tmp));
        } else {
            char buf[0x20];
            if (self->mFieldA4 >= 6) {
                sprintf(buf, &lbl_eu_80502050[0xa9],
                        (s8)self->mPadA5[0] + 1);
            } else {
                sprintf(buf, &lbl_eu_80502050[0xe], (s8)self->mPadA5[0] + 1);
            }
            nw4r::lyt::Pane* pane1 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(buf, 1);
            nw4r::lyt::Pane* pane2 = reinterpret_cast<CCol6PaneView*>(
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)
                    ->mpRootPane)
                ->setText(&lbl_eu_80502050[0x17], 1);
            nw4r::math::VEC3 vec;
            func_80137924(
                &vec, pane1, pane2,
                reinterpret_cast<CCol6LayoutView*>(self->mpLayout)->mpRootPane);
            reinterpret_cast<CCol6Cur18View*>(&self->mCur1)->vf04(&vec);
        }
        func_80138078(1);
    } else if (up) {
        // Open the box for the current player (reset cursor 2).
        self->mFieldA4 = 0xc;
        func_801D216C(&self->mCur2, 0);
        func_8022B8E4(&self->mSysWin1);
        func_80138078(3);
    } else if (down) {
        self->mFieldA4 = 0xc;
        self->mPadA5[1] = 1;
        func_801D216C(&self->mCur2, 0);
        func_8022B8E4(&self->mSysWin1);
        func_80138078(6);
    }
}

// func_80162000 - open the item box: bump the player count and the two
// summary counters from the per-player table, seed the TaskLOD/audio slots
// (v1/v2/v3 chains identical to func_8047BB54's own seeding), write the camera
/// vector block from the s16-keyed lookup, then refresh the layout labels
// (0x10f/0x11d/0x2b6) and the item box for the current player.
extern "C" void func_80162000(CCol6System* self) {
    CCol6Fade* fade = func_80113E1C();
    if (fade == 0) return;
    if (func_80113E24(fade) != 0) return;

    // Bump this player's count and the two summary counters.
    u32 f = func_8009CF8C((u32)((s8)self->mPadA5[0] + 0x7fe));
    func_8009D018((u32)((s8)self->mPadA5[0] + 0x7fe), f + 1);
    u32 n = f + (s8)self->mPadA5[0] * 5 + 1;
    u8 a = (u8)func_8013600C(&lbl_eu_80502050[0x334], &lbl_eu_80502050[0x33f], n);
    func_8009D018(0x7fc, func_8009CF8C(0x7fc) + a);
    u8 b = (u8)func_8013600C(&lbl_eu_80502050[0x334], &lbl_eu_80502050[0x346], n);
    func_8009D018(0x7fd, func_8009CF8C(0x7fd) + b);

    // Seed the TaskLOD/audio slots for this player's three id ranges.
    UnkClass_80083298View* gm = func_80083298__Q22cf13CfGameManagerFv();
    if (gm != 0) {
        u8 v1 = (u8)(((s8)self->mPadA5[0] + 1) * 10 + f + 1);
        func_80462D04__8CTaskLODFv(v1);
        func_804BCC30(func_804BC9EC__Fv(), v1);
        func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, v1);

        u8 v2 = (u8)(f + 0x3d);
        func_80462D04__8CTaskLODFv(v2);
        func_804BCC30(func_804BC9EC__Fv(), v2);
        func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, v2);

        u8 v3 = (u8)(f + 0x47);
        func_80462D5C__8CTaskLODFv(v3);
        func_804BCC3C(func_804BC9EC__Fv(), v3);
        func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, v3);

        if (v2 == 0x3d) {
            func_80462D04__8CTaskLODFv(0x55);
            func_804BCC30(func_804BC9EC__Fv(), 0x55);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x55);
        }
        if (v2 == 0x3e) {
            func_80462D04__8CTaskLODFv(0x56);
            func_804BCC30(func_804BC9EC__Fv(), 0x56);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x56);
        }
        if (v2 == 0x3d) {
            func_80462D04__8CTaskLODFv(0x57);
            func_804BCC30(func_804BC9EC__Fv(), 0x57);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x57);
        }
        if (v2 == 0x3d) {
            func_80462D04__8CTaskLODFv(0x58);
            func_804BCC30(func_804BC9EC__Fv(), 0x58);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x58);
        }
        if (func_8009CF8C(0x20) >= 0x16e) {
            func_80462D04__8CTaskLODFv(0x59);
            func_804BCC30(func_804BC9EC__Fv(), 0x59);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x59);
            if (v1 == 0xe) {
                func_80462D04__8CTaskLODFv(0x5a);
                func_804BCC30(func_804BC9EC__Fv(), 0x5a);
                func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5a);
            }
        }
        if (v2 == 0x3d) {
            func_80462D04__8CTaskLODFv(0x5b);
            func_804BCC30(func_804BC9EC__Fv(), 0x5b);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5b);
        }
        if (v2 == 0x3d) {
            func_80462D04__8CTaskLODFv(0x5c);
            func_804BCC30(func_804BC9EC__Fv(), 0x5c);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5c);
        }
        if (v2 == 0x3e) {
            func_80462D04__8CTaskLODFv(0x5d);
            func_804BCC30(func_804BC9EC__Fv(), 0x5d);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5d);
        }
        if (v2 == 0x3f) {
            func_80462D04__8CTaskLODFv(0x5e);
            func_804BCC30(func_804BC9EC__Fv(), 0x5e);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5e);
        }
        if (v2 == 0x40) {
            func_80462D04__8CTaskLODFv(0x5f);
            func_804BCC30(func_804BC9EC__Fv(), 0x5f);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5f);
        }
        if (v2 == 0x40) {
            func_80462D04__8CTaskLODFv(0x60);
            func_804BCC30(func_804BC9EC__Fv(), 0x60);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x60);
        }
        if (v2 == 0x41) {
            func_80462D04__8CTaskLODFv(0x61);
            func_804BCC30(func_804BC9EC__Fv(), 0x61);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x61);
        }
        if (v2 == 0x41) {
            func_80462D04__8CTaskLODFv(0x62);
            func_804BCC30(func_804BC9EC__Fv(), 0x62);
            func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x62);
        }

        if (func_80462E1C__8CTaskLODFv(0x2d)) {
            func_80462D5C__8CTaskLODFv(0x50);
            func_804BCC3C(func_804BC9EC__Fv(), 0x50);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x50);
        }
        if (func_80462E1C__8CTaskLODFv(0xe)) {
            func_80462D5C__8CTaskLODFv(0x51);
            func_804BCC3C(func_804BC9EC__Fv(), 0x51);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x51);
        }
        if (func_80462E1C__8CTaskLODFv(0x2a)) {
            func_80462D5C__8CTaskLODFv(0x52);
            func_804BCC3C(func_804BC9EC__Fv(), 0x52);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x52);
        }
        if (func_80462E1C__8CTaskLODFv(0x22)) {
            func_80462D5C__8CTaskLODFv(0x53);
            func_804BCC3C(func_804BC9EC__Fv(), 0x53);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x53);
        }
        if (func_80462E1C__8CTaskLODFv(0x23)) {
            func_80462D5C__8CTaskLODFv(0x54);
            func_804BCC3C(func_804BC9EC__Fv(), 0x54);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x54);
        }
        if (func_80462E1C__8CTaskLODFv(0x3e)) {
            func_80462D5C__8CTaskLODFv(0x55);
            func_804BCC3C(func_804BC9EC__Fv(), 0x55);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x55);
        }
        if (func_80462E1C__8CTaskLODFv(0x2d)) {
            func_80462D5C__8CTaskLODFv(0x57);
            func_804BCC3C(func_804BC9EC__Fv(), 0x57);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x57);
        }
        if (func_80462E1C__8CTaskLODFv(0x2d)) {
            func_80462D5C__8CTaskLODFv(0x58);
            func_804BCC3C(func_804BC9EC__Fv(), 0x58);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x58);
        }
        if (func_80462E1C__8CTaskLODFv(0xe)) {
            func_80462D5C__8CTaskLODFv(0x59);
            func_804BCC3C(func_804BC9EC__Fv(), 0x59);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x59);
        }
        if (func_80462E1C__8CTaskLODFv(0xb)) {
            func_80462D5C__8CTaskLODFv(0x5b);
            func_804BCC3C(func_804BC9EC__Fv(), 0x5b);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5b);
        }
        if (func_80462E1C__8CTaskLODFv(0x15)) {
            func_80462D5C__8CTaskLODFv(0x5c);
            func_804BCC3C(func_804BC9EC__Fv(), 0x5c);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5c);
        }
        if (func_80462E1C__8CTaskLODFv(0xc)) {
            func_80462D5C__8CTaskLODFv(0x5d);
            func_804BCC3C(func_804BC9EC__Fv(), 0x5d);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5d);
        }
        if (func_80462E1C__8CTaskLODFv(0xd)) {
            func_80462D5C__8CTaskLODFv(0x5e);
            func_804BCC3C(func_804BC9EC__Fv(), 0x5e);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5e);
        }
        if (func_80462E1C__8CTaskLODFv(0xe)) {
            func_80462D5C__8CTaskLODFv(0x5f);
            func_804BCC3C(func_804BC9EC__Fv(), 0x5f);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x5f);
        }
        if (func_80462E1C__8CTaskLODFv(0x2c)) {
            func_80462D5C__8CTaskLODFv(0x60);
            func_804BCC3C(func_804BC9EC__Fv(), 0x60);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x60);
        }
        if (func_80462E1C__8CTaskLODFv(0xf)) {
            func_80462D5C__8CTaskLODFv(0x61);
            func_804BCC3C(func_804BC9EC__Fv(), 0x61);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x61);
        }
        if (func_80462E1C__8CTaskLODFv(0x19)) {
            func_80462D5C__8CTaskLODFv(0x62);
            func_804BCC3C(func_804BC9EC__Fv(), 0x62);
            func_8047BD94__17UnkClass_8047BB54Fv(&gm->field_0xF0, 0x62);
        }
    }

    // Remaining audio/quest seeds from the per-player table.
    u8 c1 = (u8)func_8013600C(&lbl_eu_80502050[0x334], &lbl_eu_80502050[0x351], n);
    u8 c2 = (u8)func_8013600C(&lbl_eu_80502050[0x334], &lbl_eu_80502050[0x35e], n);
    if (c1 != 0) func_8009D018(c1 + 0x278a, 1);
    if (c2 != 0) func_8009D018(c2 + 0x798, 1);
    u16 d = func_8013606C(&lbl_eu_80502050[0x334], &lbl_eu_80502050[0x36a], n);
    if (d != 0) func_8009D018(d + 0x220, 0xc8);
    u16 e = func_8013606C(&lbl_eu_80502050[0x334], &lbl_eu_80502050[0x376], n);
    self->mFieldA2 = func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
        1, e, 0, 0, lbl_eu_80667558);

    // Camera/vector block: every value is the s16 lookup result cast to float
    // (MWCC emits the 2^52 int->double conversion for (f32)(s16)); two slots
    // are byte-sized lookups via func_8013600C.
    self->mCamPos.x = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x380], n);
    self->mCamPos.y = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x38a], n);
    self->mCamPos.z = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x394], n);
    self->mCamLookAt.x = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x39e], n);
    self->mCamLookAt.y = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x3a9], n);
    self->mVec168.x = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x3b4], n);
    self->mVec168.y = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x3be], n);
    self->mVec168.z = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x3c8], n);
    self->mVec174.x = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x3d2], n);
    self->mVec174.y = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x3dd], n);
    self->mField180 = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x3e8], n);
    self->mField184 = (u8)func_8013600C(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x3f4], n);
    self->mVec188.x = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x400], n);
    self->mVec188.y = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x40a], n);
    self->mVec188.z = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x414], n);
    self->mVec194.x = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x41e], n);
    self->mVec194.y = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x429], n);
    self->mVec1A0.x = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x434], n);
    self->mVec1A0.y = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x43e], n);
    self->mVec1A0.z = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x448], n);
    self->mVec1AC.x = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x452], n);
    self->mVec1AC.y = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x45d], n);
    self->mField1B8 = (f32)(s16)func_80136130(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x468], n);
    self->mField1BC = (u8)func_8013600C(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x474], n);

    // Player counts on the two slots.
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mpLayout,
                                            &lbl_eu_80502050[0x10f],
                                            func_8009CF8C(0x7fc));
    func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mpLayout,
                                            &lbl_eu_80502050[0x11d],
                                            func_8009CF8C(0x7fd));

    // Reserved-slot count string: 0x19 if any reserved slot is populated.
    s32 cnt = 0x1a;
    for (u32 i = 0; i < 5; i++) {
        u8 idx = (u8)i;
        if (func_8009CF8C(idx + 0x804) == 1) {
            cnt = 0x19;
            break;
        }
    }
    char* s2b6 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], cnt);
    func_80136B4C(self->mpLayout, &lbl_eu_80502050[0x2b6], s2b6, 0);

    func_80160A6C(self, (s8)self->mPadA5[0]);
    self->mFlagA1 = 1;
    self->mFieldA4 = 0xf;
}

// func_80162C40 - fill and open window 2 for the current player: resolve the
// count/name strings for the player index at +0xA5, format the banner, set
// the window text, activate it and play the open sound.
void func_80162C40(CCol6System* self) {
    if (func_80082104__Q22cf13CfGameManagerFv() != 0) {
        return;
    }

    self->mFieldA4 = 0x1c;
    char* tbl = lbl_eu_80502050;
    // Single expression so MWCC keeps p*5 in a scratch register and folds
    // the count add into the named temp (retail add r28, r0, r3 shape).
    u32 f = func_8009CF8C((u32)((s8)self->mPadA5[0] + 0x7fe));
    u32 n = (s8)self->mPadA5[0] * 5 + f;
    u8 a = (u8)func_8013600C(&tbl[0x334], &tbl[0x33f], n);
    u8 b = (u8)func_8013600C(&tbl[0x334], &tbl[0x346], n);

    char* str0 = func_80136190(tbl, &tbl[0x9], (s8)self->mPadA5[0] + 0x4e);
    char* str1 =
        func_80136190(tbl, &tbl[0x9], (s8)self->mPadA5[0] * 5 + f + 0x53);
    char* str2 = func_80136190(tbl, &tbl[0x9], 0x78);
    char* str3 = func_80136190(tbl, &tbl[0x9], 0x79);

    ml::FixStr<256> buf;
    func_eu_801651A0(buf.mString, &tbl[0x480], str0, str1, str2, a, str3, b);
    func_8022B9B4(&self->mSysWin2, buf.mString, 0);
    func_8022BFC8(&self->mSysWin2, 1);
    func_8022B8B8(&self->mSysWin2);
    func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
        1, 0x134, 0, 0, lbl_eu_80667558);
}

// func_80162DB4 - "open item box" transition: when the fade is idle, set the
// window-2 state byte, play the open sound, move the camera to the stored
// position/look-at, then post the item-box entry messages for the current
// player-count bucket (0x802), optionally re-seeding the task/LOD audio.
void func_80162DB4(CCol6System* self) {
    CCol6Fade* fade = func_80113E1C();
    if (fade == 0) {
        return;
    }
    if (func_80113E24(fade) != 0) {
        return;
    }

    self->mFieldA4 = 0x17;
    f32 vol = lbl_eu_80667578;
    func_80135464(2, 0, vol, vol, vol);
    func_80082008__Q22cf13CfGameManagerFv(9, 0, 0, 0, 0);
    func_80082088__Q22cf13CfGameManagerFv(
        0, &self->mCamPos, &self->mCamLookAt, 0, lbl_eu_80667564);
    func_80082060__Q22cf13CfGameManagerFv();

    UnkClass_80083298View* gm;
    u8 count = (u8)func_8009CF8C(0x802);
    s32 n = (s32)count + 0x14;
    gm = func_80083298__Q22cf13CfGameManagerFv();
    if (gm != 0) {
        u8 v = (u8)(count + 0x32);
        func_80462D04__8CTaskLODFv(v);
        func_804BCC30(func_804BC9EC__Fv(), v);
        func_8047BD8C__17UnkClass_8047BB54Fv(&gm->field_0xF0, v);
    }

    u8 a = func_8013600C(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x351], (u32)n);
    u8 b = func_8013600C(
        &lbl_eu_80502050[0x334], &lbl_eu_80502050[0x35e], (u32)n);
    if (a != 0) {
        func_8009D018((u32)a + 0x278a, 1);
    }
    if (b != 0) {
        func_8009D018((u32)b + 0x798, 1);
    }
}

// func_80162EF8 - window-2 player-count ladder: while window 2 is active,
// respond to the pad-confirm button; otherwise scan the player-count buckets
// (0x7fe..0x801 must all reach the 0x802 total), write the per-bucket camera/
// vector block from the s16-keyed lookup table, then post the 0x16 fade, the
// "player N joined" banner (count >= 5) or the per-slot threshold scan.
extern "C" void func_80162EF8(CCol6System* self) {
    if (CSysWin_getUnk34(&self->mSysWin2) != 0) {
        if (CSysWin_isActive(&self->mSysWin2) != 0) {
            CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
            u32 cond;
            if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                cond = (pad->mPressedButtonFlags & 0x200000) != 0;
            } else {
                cond = (pad->mPressedButtonFlags & 0x10) != 0;
            }
            if (cond != 0) {
                func_8022B8E4(&self->mSysWin2);
            }
        }
    } else {
        // Player count for the current slot and the total bucket (0x802).
        s32 f = (s32)func_8009CF8C((u32)((s8)self->mPadA5[0] + 0x7fe));
        s32 ok = 1;
        s32 big = 0;
        u8 total = (u8)func_8009CF8C(0x802) + 1;

        // All four player slots must reach the total, else ok = 0.
        for (u32 i = 0; i < 4; i++) {
            if (func_8009CF8C((u8)i + 0x7fe) < (u32)total) {
                ok = 0;
                break;
            }
        }

        if (ok != 0) {
            func_8009D018(0x802, total);
            // String-pool base held in a register for the whole block (retail
            // materializes it once; the per-pane offsets stay plain addis).
            char* base = lbl_eu_80502050;
            u32 n = (u32)total + 0x14;

            // Camera/vector block: every value is the s16 lookup result cast to
            // float (MWCC emits the 2^52 int->double conversion for (f32)(s16));
            // two slots are byte-sized lookups via func_8013600C.
            self->mCamPos.x = (f32)(s16)func_80136130(
                &base[0x334], &base[0x380], n);
            self->mCamPos.y = (f32)(s16)func_80136130(
                &base[0x334], &base[0x38a], n);
            self->mCamPos.z = (f32)(s16)func_80136130(
                &base[0x334], &base[0x394], n);
            self->mCamLookAt.x = (f32)(s16)func_80136130(
                &base[0x334], &base[0x39e], n);
            self->mCamLookAt.y = (f32)(s16)func_80136130(
                &base[0x334], &base[0x3a9], n);
            self->mVec168.x = (f32)(s16)func_80136130(
                &base[0x334], &base[0x3b4], n);
            self->mVec168.y = (f32)(s16)func_80136130(
                &base[0x334], &base[0x3be], n);
            self->mVec168.z = (f32)(s16)func_80136130(
                &base[0x334], &base[0x3c8], n);
            self->mVec174.x = (f32)(s16)func_80136130(
                &base[0x334], &base[0x3d2], n);
            self->mVec174.y = (f32)(s16)func_80136130(
                &base[0x334], &base[0x3dd], n);
            self->mField180 = (f32)(s16)func_80136130(
                &base[0x334], &base[0x3e8], n);
            self->mField184 = (u8)func_8013600C(
                &base[0x334], &base[0x3f4], n);
            self->mVec188.x = (f32)(s16)func_80136130(
                &base[0x334], &base[0x400], n);
            self->mVec188.y = (f32)(s16)func_80136130(
                &base[0x334], &base[0x40a], n);
            self->mVec188.z = (f32)(s16)func_80136130(
                &base[0x334], &base[0x414], n);
            self->mVec194.x = (f32)(s16)func_80136130(
                &base[0x334], &base[0x41e], n);
            self->mVec194.y = (f32)(s16)func_80136130(
                &base[0x334], &base[0x429], n);
            self->mVec1A0.x = (f32)(s16)func_80136130(
                &base[0x334], &base[0x434], n);
            self->mVec1A0.y = (f32)(s16)func_80136130(
                &base[0x334], &base[0x43e], n);
            self->mVec1A0.z = (f32)(s16)func_80136130(
                &base[0x334], &base[0x448], n);
            self->mVec1AC.x = (f32)(s16)func_80136130(
                &base[0x334], &base[0x452], n);
            self->mVec1AC.y = (f32)(s16)func_80136130(
                &base[0x334], &base[0x45d], n);
            self->mField1B8 = (f32)(s16)func_80136130(
                &base[0x334], &base[0x468], n);
            self->mField1BC = (u8)func_8013600C(
                &base[0x334], &base[0x474], n);
        }

        // Banner if this player's count clears the 5-player threshold.
        if (f >= 5) big = 1;

        if (ok != 0) {
            self->mFieldA4 = 0x16;
            func_80135464(0, 0, lbl_eu_80667578, lbl_eu_80667578,
                          lbl_eu_80667578);
        } else if (big != 0) {
            // "player N joined" banner in window 2.
            self->mFieldA4 = 0x23;
            char* s1 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                     (s8)self->mPadA5[0] + 0x72);
            char* s2 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                     0x77);
            ml::FixStr<128> buf;
            buf.format(&lbl_eu_80502050[0xb5], s1, s2);
            func_8022B9B4(&self->mSysWin2, buf.mString, 0);
            func_8022B8B8(&self->mSysWin2);
            func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
                1, 0x134, 0, 0, lbl_eu_80667558);
        } else {
            // Per-slot threshold scan (same shape as CCol6Invite::Move).
            union {
                struct {
                    u32 w;
                    u8 b;
                };
                u8 bytes[5];
            } data;
            data.w = lbl_eu_80667540;
            data.b = lbl_eu_80667544;

            u8 result = (u8)func_8009CF8C(0x7fc);

            u8 found = 0;
            for (u32 i = 0; i < 5; i++) {
                u8 idx = (u8)i;
                if (result >= data.bytes[idx] &&
                    func_8009CF8C(idx + 0x804) == 0) {
                    found = 1;
                    break;
                }
            }

            if (found != 0) {
                union {
                    struct {
                        u32 w;
                        u8 b;
                    };
                    u8 bytes[5];
                } data2;
                union {
                    struct {
                        u32 w;
                        u8 b;
                    };
                    u8 bytes[5];
                } data3;
                data2.w = lbl_eu_80667548;
                data2.b = lbl_eu_8066754C;
                data3.w = lbl_eu_80667550;
                data3.b = lbl_eu_80667554;

                s16 val = (s16)func_8009CF8C(0x7fc);
                for (u32 i = 0; i < 5; i++) {
                    if (val >= data2.bytes[i] &&
                        func_8009CF8C(i + 0x804) == 0) {
                        lbl_eu_8066235C = (s32)i;
                        func_8009D018(i + 0x804, 1);
                        val = (s16)(val + (s8)data3.bytes[i]);
                        func_8009D018(0x7fc, val);
                    }
                }
                self->mFieldA4 = 0x15;
                func_80135464(0, 0, lbl_eu_80667578, lbl_eu_80667578,
                              lbl_eu_80667578);
            } else {
                self->mFieldA4 = 0x15;
                func_80135464(0, 0, lbl_eu_80667578, lbl_eu_80667578,
                              lbl_eu_80667578);
            }
        }
    }
}

// func_80163614 - window-2 interaction / player-count ladder: while window 2
// is active, respond to the pad-confirm button (mode-dependent bit), posting
// the window's close action; otherwise scan the player-count buckets and show
// the invite banner for the first fully-populated bucket.
void func_80163614(CCol6System* self) {
    if (CSysWin_getUnk34(&self->mSysWin2) != 0) {
        if (CSysWin_isActive(&self->mSysWin2) != 0) {
            CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
            u32 cond;
            if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                cond = (pad->mPressedButtonFlags & 0x200000) != 0;
            } else {
                cond = (pad->mPressedButtonFlags & 0x10) != 0;
            }
            if (cond != 0) {
                func_8022B8E4(&self->mSysWin2);
            }
        }
    } else {
        self->mFieldA4 = 0x21;
        // Find the highest bucket i in 5..1 whose four player slots
        // (0x7fe + 0..3) are all populated; default 0 (none).
        u32 sel = 0;
        s32 i = 5;
        while (i >= 1) {
            s32 cand = (s8)i;
            s32 ok = 1;
            for (u32 j = 0; j < 4; j++) {
                if ((u32)func_8009CF8C((u8)j + 0x7fe) < (u32)cand) {
                    ok = 0;
                    break;
                }
            }
            if (ok != 0) {
                sel = (u8)i;
                break;
            }
            i--;
        }
        if (sel != 5) {
            // Banner: "player N joined" (chars 0x76/0x77 are the slot names).
            char* s76 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x76);
            char* s77 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x77);
            ml::FixStr<128> buf;
            buf.format(&lbl_eu_80502050[0xb5], s76, s77);
            func_8022B9B4(&self->mSysWin2, buf.mString, 0);
            func_8022B8B8(&self->mSysWin2);
            func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
                1, 0x134, 0, 0, lbl_eu_80667558);
        }
    }
}

// func_8016378C - window 2 interaction: while the second system window is
// active, respond to the pad-confirm button (mode-dependent bit), posting the
// window's close action; otherwise run the player-count ladder scan and post
// the join message for the first count bucket that is fully populated.
void func_8016378C(CCol6System* self) {
    if (CSysWin_getUnk34(&self->mSysWin2) != 0) {
        if (CSysWin_isActive(&self->mSysWin2) != 0) {
            CCol6Pad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
            u32 cond;
            if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                cond = (pad->mPressedButtonFlags & 0x200000) != 0;
            } else {
                cond = (pad->mPressedButtonFlags & 0x10) != 0;
            }
            if (cond != 0) {
                func_8022B8E4(&self->mSysWin2);
            }
        }
    } else {
        self->mFieldA4 = 0x22;
        // Find the highest bucket i in 5..1 whose four player slots
        // (0x7fe + 0..3) are all populated; default 0 (none).
        u32 sel = 0;
        s32 i = 5;
        while (i >= 1) {
            s32 cand = (s8)i;
            s32 ok = 1;
            for (u32 j = 0; j < 4; j++) {
                if ((u32)func_8009CF8C((u8)j + 0x7fe) < (u32)cand) {
                    ok = 0;
                    break;
                }
            }
            if (ok != 0) {
                sel = (u8)i;
                break;
            }
            i--;
        }
        if (sel != 5) {
            u16 id = func_8013606C(&lbl_eu_80502050[0xbd],
                                   &lbl_eu_80502050[0xc9], sel + 5);
            func_8013E2E0((u32)id, 0, 0, 0, 0, 1, 0, 1, 0);
        }
    }
}

// func_801638C0 - per-player window-2 ladder: when the task is idle and the
// current player's count clears the 5-player threshold, post the join banner
// (player-specific string char); otherwise scan the per-slot threshold tables
// and (re)assign the join-slot global, then fade the screen.
void func_801638C0(CCol6System* self) {
    if (func_801B481C() != 0) {
        return;
    }

    bool ge = (s32)func_8009CF8C((u32)((s8)self->mPadA5[0] + 0x7fe)) >= 5;
    if (ge != 0) {
        // Banner: "player N joined" (char 0x72+N is the player name).
        self->mFieldA4 = 0x23;
        char* s1 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9],
                                 (s8)self->mPadA5[0] + 0x72);
        char* s2 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x77);
        ml::FixStr<128> buf;
        buf.format(&lbl_eu_80502050[0xb5], s1, s2);
        func_8022B9B4(&self->mSysWin2, buf.mString, 0);
        func_8022B8B8(&self->mSysWin2);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
            1, 0x134, 0, 0, lbl_eu_80667558);
        return;
    }

    // 5-byte threshold table: u32 word + trailing byte (big-endian byte
    // order, read left-to-right).
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } data;
    data.w = lbl_eu_80667540;
    data.b = lbl_eu_80667544;

    u8 result = (u8)func_8009CF8C(0x7fc);

    u8 found = 0;
    for (u32 i = 0; i < 5; i++) {
        u8 idx = (u8)i;
        if (result >= data.bytes[idx] && func_8009CF8C(idx + 0x804) == 0) {
            found = 1;
            break;
        }
    }

    if (found != 0) {
        // Per-slot tables: pairs of (u32 word, trailing byte) read
        // left-to-right as five bytes each.
        union {
            struct {
                u32 w;
                u8 b;
            };
            u8 bytes[5];
        } data2;
        union {
            struct {
                u32 w;
                u8 b;
            };
            u8 bytes[5];
        } data3;
        data2.w = lbl_eu_80667548;
        data2.b = lbl_eu_8066754C;
        data3.w = lbl_eu_80667550;
        data3.b = lbl_eu_80667554;

        s16 val = (s16)func_8009CF8C(0x7fc);
        for (u32 i = 0; i < 5; i++) {
            if (val >= data2.bytes[i] && func_8009CF8C(i + 0x804) == 0) {
                lbl_eu_8066235C = (s32)i;
                func_8009D018(i + 0x804, 1);
                val = (s16)(val + (s8)data3.bytes[i]);
                func_8009D018(0x7fc, val);
            }
        }
        self->mFieldA4 = 0x15;
        func_80135464(0, 0, lbl_eu_80667578, lbl_eu_80667578, lbl_eu_80667578);
    } else {
        // Retail keeps a copy of this store/fade in BOTH arms (the b-over-else
        // emits the extra branch); do not hoist it out of the if/else.
        self->mFieldA4 = 0x15;
        func_80135464(0, 0, lbl_eu_80667578, lbl_eu_80667578, lbl_eu_80667578);
    }
}

// func_80163AF4 - window-2 player-count ladder: when the task is idle and the
// player count (0x802) clears the 5-player threshold, post the join banner;
// otherwise scan the per-slot threshold tables and (re)assign the join-slot
// global, then fade the screen (same scan shape as CCol6Invite::Move).
void func_80163AF4(CCol6System* self) {
    if (func_801B481C() != 0) {
        return;
    }

    if ((s32)func_8009CF8C(0x802) >= 5) {
        // Banner: "player N joined" (chars 0x76/0x77 are the slot names).
        self->mFieldA4 = 0x25;
        char* s76 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x76);
        char* s77 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x77);
        ml::FixStr<128> buf;
        buf.format(&lbl_eu_80502050[0xb5], s76, s77);
        func_8022B9B4(&self->mSysWin2, buf.mString, 0);
        func_8022B8B8(&self->mSysWin2);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
            1, 0x134, 0, 0, lbl_eu_80667558);
        return;
    }

    // 5-byte threshold table: u32 word + trailing byte (big-endian byte
    // order, read left-to-right).
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } data;
    data.w = lbl_eu_80667540;
    data.b = lbl_eu_80667544;

    u8 result = (u8)func_8009CF8C(0x7fc);

    // Retail keeps the "slot found" latch out of the callee-save pool: single
    // assignment per path via goto, so MWCC colors it into scratch r0.
    u32 found;
    u32 i;
    for (i = 0; i < 5; i++) {
        // (u8)index temp blocks MWCC's pointer strength-reduction; retail walks
        // the table base+index via lbzx.
        u8 idx = (u8)i;
        if (result >= data.bytes[idx] && func_8009CF8C(idx + 0x804) == 0) {
            found = 1;
            goto slotFound;
        }
    }
    found = 0;
slotFound:

    if (found != 0) {
        // Per-slot tables: pairs of (u32 word, trailing byte) read
        // left-to-right as five bytes each. (data3 declared first: MWCC
        // stacks same-size locals so the last-declared lands lowest.)
        union {
            struct {
                u32 w;
                u8 b;
            };
            u8 bytes[5];
        } data3;
        union {
            struct {
                u32 w;
                u8 b;
            };
            u8 bytes[5];
        } data2;
        data2.w = lbl_eu_80667548;
        data2.b = lbl_eu_8066754C;
        data3.w = lbl_eu_80667550;
        data3.b = lbl_eu_80667554;

        s16 val = (s16)func_8009CF8C(0x7fc);
        for (u32 j = 0; j < 5; j++) {
            if (val >= data2.bytes[j] && func_8009CF8C(j + 0x804) == 0) {
                lbl_eu_8066235C = (s32)j;
                func_8009D018(j + 0x804, 1);
                val = (s16)(val + (s8)data3.bytes[j]);
                func_8009D018(0x7fc, val);
            }
        }
        self->mFieldA4 = 0x15;
        func_80135464(0, 0, lbl_eu_80667578, lbl_eu_80667578, lbl_eu_80667578);
    } else {
        // Retail keeps a copy of this store/fade in BOTH arms (the b-over-else
        // emits the extra branch); do not hoist it out of the if/else.
        self->mFieldA4 = 0x15;
        func_80135464(0, 0, lbl_eu_80667578, lbl_eu_80667578, lbl_eu_80667578);
    }
}

// CCol6Invite::~CCol6Invite()
// extern "C" free-function form (same retail shape as ~CCol6CheckBat):
// outer null-check covers the delete; nested double null-check (D2-inlined-
// into-D1 artifact) guards the CProcess base dtor; default -O4,p.
extern "C" void* __dt__11CCol6InviteFv(CCol6Invite* self, int flags) {
    if (self != 0) {
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// CCol6Invite::Init() - set up the invite banner: resolve the three banner
// strings, bump the two invite counters in the global flag memory by the
// instance's byte args, then format the banner message and post it.
void CCol6Invite::Init() {
    char* str0 = func_8013639C((const void*)lbl_eu_80664098,
                               &lbl_eu_80502050[0x9], mArg2);

    // Compound assignment keeps the running value in one register: mask,
    // add the byte flag in place, then mask for the store.
    u32 val = func_8009CF8C(0x7fc);
    val &= 0xFF;
    val += mArg3;
    func_8009D018(0x7fc, (u8)val);
    val = func_8009CF8C(0x7fd);
    val &= 0xFF;
    val += mArg4;
    func_8009D018(0x7fd, (u8)val);

    char* str1 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x7e);
    char* str2 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x78);
    char* str3 = func_80136190(lbl_eu_80502050, &lbl_eu_80502050[0x9], 0x79);

    ml::FixStr<128> buf;
    buf.format(&lbl_eu_80502050[0x480], str0, str1, str2, mArg3, str3, mArg4);
    func_8013D55C(buf.mString, 0, 0);
}

// CCol6Invite::Move() - while no system menu is closing, re-run the invite
// slot scan: if the current player count (0x7fc) clears the table threshold
// and the matching slot is populated, walk the per-slot table to (re)assign
// the join-slot global (0x8066235C) and adjust the count by the signed
// per-slot delta; the 0x64 flag always ends set.
void CCol6Invite::Move() {
    if (func_80124B78() != 0) {
        return;
    }

    // 5-byte threshold tables (u32 word + trailing byte), read left-to-right
    // as five bytes each.
    union {
        struct {
            u32 w;
            u8 b;
        };
        u8 bytes[5];
    } t1, t3, t2;
    t1.w = lbl_eu_80667540;
    t1.b = lbl_eu_80667544;

    u8 found;
    for (u32 cnt = (u8)func_8009CF8C(0x7fc), i = 0; i < 5; i++) {
        if (cnt >= t1.bytes[(u8)i] && func_8009CF8C((u8)i + 0x804) == 0) {
            found = 1;
            goto scanned;
        }
    }
    found = 0;
scanned:

    if (found != 0) {
        t2.w = lbl_eu_80667548;
        t2.b = lbl_eu_8066754C;
        t3.w = lbl_eu_80667550;
        t3.b = lbl_eu_80667554;

        // Second scan: signed running total kept as s16 (re-extended on
        // every read); indexed table access (MWCC strength-reduces to
        // incrementing pointers, matching retail - same shape as
        // func_8015D3A0).
        s16 total = func_8009CF8C(0x7fc);
        s32 j;
        u8* p = t2.bytes;
        for (j = 0; j < 5; j++) {
            if (total >= *p && func_8009CF8C(j + 0x804) == 0) {
                lbl_eu_8066235C = j;
                func_8009D018(j + 0x804, 1);
                total += (s8)t3.bytes[j];
                func_8009D018(0x7fc, total);
            }
            p++;
        }
        mFlag64 = 1;
    } else {
        mFlag64 = 1;
    }
}
