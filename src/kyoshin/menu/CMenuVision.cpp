#include "kyoshin/menu/CMenuVision.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/lib/CLibStaticData.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include <revolution/gx/GXPixel.h>                 // GXSetZMode

// Globals shared across menu units
extern u32 lbl_eu_80663E28;
extern const f32 lbl_eu_80667DC0; // 0.0f
extern const f32 lbl_eu_80667DD8;
extern const f32 lbl_eu_80667DCC;
extern const f32 lbl_eu_80667DDC;
extern u32 lbl_eu_80575858[6];
extern const f32 lbl_eu_80667DC4; // 1.0f
extern const f32 lbl_eu_80667DC8; // scale factor
// MWCC does not mangle global-scope data names.
extern char lbl_eu_80504268[];    // string table base
extern u32 lbl_eu_805041C0[];     // 6 pane-name pointers
extern CMenuVision* lbl_eu_80664388;

namespace cf {
class CBattleManager {
public:
    static CBattleManager* getInstance();
    void* func_800EA444();
};
}

extern u32 func_801355A0();

void func_801AFAD0(CMenuVision*, CMenuVisionEntry*);
extern "C" f32 func_800F4424(void*);

// Read-only view of the current frame stored at AnimTransform+0x10.
struct AnimFrameAccess {
    u8 _pad[0x10];
    f32 frame;
};

extern "C" {
void* findObjectById__Fi(int);
void* func_800EA444(cf::CBattleManager*);
u32 func_800F4784(void*);
void* func_800F477C(void*);
int func_800F4648(void*);
f32 func_800F42AC(void*);
void func_80138078__FUl(u32);
void func_80139B5C(void*, const char*, u32);
void func_80139BF4(void*, const char*, u32, u32);
void func_80137F88(void*, void*);
u16 func_80136254(void*, const char*, u32);
char* func_80138F78(u32);

extern u32 lbl_eu_80662580;
extern u32 lbl_eu_80662584;
extern u32 lbl_eu_80662588;
extern u32 lbl_eu_8066258C;
extern u32 lbl_eu_80662590;
extern u32 lbl_eu_80662594;
extern u32 lbl_eu_80662598;
extern u32 lbl_eu_8066259C;
extern u32 lbl_eu_806625A0;
extern u32 lbl_eu_806625A4;
extern u32 lbl_eu_806625A8;
extern u32 lbl_eu_806625AC;
extern u32 lbl_eu_806625B0;
extern u32 lbl_eu_806625B4;
extern f32 lbl_eu_80667DD4;
extern u32 lbl_eu_80667DD0;
extern void* lbl_eu_806640E0;
extern const char* lbl_eu_80504238[4];
extern const char* lbl_eu_80504248[4];
}
typedef void (*MenuVisionSetDamageText)(nw4r::lyt::Layout*, char*, int);

struct VisionQuad {
    s16 values[4];
};
static inline void menuVisionCopyQuad(VisionQuad& dst, const VisionQuad& src) {
    dst.values[0] = src.values[0];
    dst.values[1] = src.values[1];
    dst.values[2] = src.values[2];
    dst.values[3] = src.values[3];
}



// Damage/status text colour tables (.sbss, filled at startup by
// sinit_801AFCE8).
VisionQuad lbl_eu_80664380;
VisionQuad lbl_eu_80664378;
VisionQuad lbl_eu_80664370;
VisionQuad lbl_eu_80664368;
VisionQuad lbl_eu_80664360;
VisionQuad lbl_eu_80664358;
VisionQuad lbl_eu_80664350;
VisionQuad lbl_eu_80664348;
VisionQuad lbl_eu_80664340;
VisionQuad lbl_eu_80664338;

// Pad accessor for pane float fields at known retail offsets
struct PaneTransAccess {
    u8 _pad[0x4C];
    f32 transX;
    f32 transY;
};

struct PaneScaleAccess {
    u8 _pad[0x44];
    f32 scaleX;
    f32 scaleY;
};

struct MenuVisionBattleData {
    int actor;
    int targetActor;
    u8 _pad08[0x84 - 0x08];
    u32 flags84;
    u32 flags88;
    u8 _pad8C[0x824 - 0x8C];
    u32 flags824;
    u32 value828;
    u32 value82C;
};

struct MenuVisionActorInfo {
    u8 _pad00[0x3C];
    u16 mode3C;
    u8 _pad3E[0x42 - 0x3E];
    u8 mode42;
};

#pragma push
#pragma inline_max_size(10000)

static inline void* menuVisionResource(const char* name) {
    return func_801355F4()->GetResource(0x74696D67, name, 0);
}

static inline u32 menuVisionActorKind(void* actor) {
    return ((u32 (*)(void*))(*(void***)actor)[0xE0 / 4])(actor);
}

static inline MenuVisionActorInfo* menuVisionActorInfo(void* actor) {
    void* value = ((void* (*)(void*))(*(void***)actor)[0x298 / 4])(actor);
    return *(MenuVisionActorInfo**)((u8*)value + 0x50);
}

static inline void menuVisionSetVisible(nw4r::lyt::Pane* pane, bool visible) {
    if (pane != 0) {
        PaneVisAccess* access = reinterpret_cast<PaneVisAccess*>(pane);
        access->visByte &= ~1;
        access->visByte |= visible;
    }
}

static inline void menuVisionSetImage(CMenuVisionEntry& entry, const char* paneName, void* image) {
    nw4r::lyt::Pane* pane = entry.mLayout->GetRootPane()->FindPaneByName(paneName, true);
    menuVisionSetVisible(pane, false);
    if (image != 0) {
        func_80137E7C(entry.mLayout, (char*)paneName, image);
        menuVisionSetVisible(pane, true);
    }
}

static inline void menuVisionBegin(CMenuVisionEntry& entry) {
    entry.mLayout->SetAnimationEnable(entry.mAnim2, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim3, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim4, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim5, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim6, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim7, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim8, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim1, true);
    entry.mAnim1->SetFrame(lbl_eu_80667DC0);
    entry.field_0x28 = 0;
    entry.mState = 1;
    func_80138078__FUl(0x1C3);
}

static inline void menuVisionBeginWithoutSecond(CMenuVisionEntry& entry) {
    entry.mLayout->SetAnimationEnable(entry.mAnim2, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim3, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim4, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim5, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim6, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim7, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim8, false);
    entry.mLayout->SetAnimationEnable(entry.mAnim1, true);
    entry.mAnim1->SetFrame(lbl_eu_80667DC0);
    entry.field_0x28 = 0;
    entry.mState = 1;
    func_80138078__FUl(0x1C3);
}


static inline void menuVisionReplacePaneImage(nw4r::lyt::Pane* pane, void* image) {
    if (image != 0) {
        func_80137F88(pane, image);
        pane->SetVisible(true);
    }
}

// --- CMenuVision ctor/dtor ---

// Null-ptmf word triple view over __ptmf_null (3-word member-function pointer).
struct PtmfWords;

// Retail ctor symbol is unmangled (`__ct__CMenuVision`, C-ABI); kept as a
// C-linkage out-of-line helper so the factory (func_801ACCE0) emits a real bl
// to it, returning `this` in r3 like a real constructor (retail relies on it).
extern "C" __declspec(noinline) CMenuVision* __ct__CMenuVision(CMenuVision* self, CProcess* parent) {
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(self));

    // Retail forms both vtable-group addresses up front, then writes them.
    void* baseVt = (void*)lbl_eu_8052C1C0;
    char* finalVt = lbl_eu_80533538;
    // Retail first stores the plain base vtable group, then overwrites the same
    // slot with the final composite vtable group (both stores are live bytes).
    self->field_0x10 = baseVt;

    // Two null-ptmf callback slots copied as 12-byte word triples.
    const PtmfWords* np = reinterpret_cast<const PtmfWords*>(__ptmf_null);
    void* npFn = np->fn;
    self->cbSlot0.adj1 = np->adj1;
    self->cbSlot0.fn = npFn;
    self->cbSlot0.adj2 = np->adj2;
    void* npFn2 = np->fn;
    self->cbSlot1.adj1 = np->adj1;
    self->cbSlot1.fn = npFn2;
    self->cbSlot1.adj2 = np->adj2;

    self->field_0x54 = 0;
    self->field_0x55 = 0;
    self->field_0x10 = (void*)finalVt;
    self->field_0x58 = (void*)(finalVt + 0x24);
    self->field_0x5C = (void*)(finalVt + 0xac);
    self->mScn = reinterpret_cast<CScn*>(parent);
    __ct__17UnkClass_8045F564Fv(&self->mLayoutMem);

    // Pass 1: clear slot 0 explicitly, then walk slots 1-5.
    CMenuVisionEntry* e = &self->mEntries[1];
    CMenuVisionEntry* end = &self->mEntries[6];
    {
        CMenuVisionEntry& e0 = self->mEntries[0];
        e0.mLayout = 0;
        e0.mAnim1 = 0;
        e0.mAnim2 = 0;
        e0.mAnim3 = 0;
        e0.mAnim4 = 0;
        e0.mAnim5 = 0;
        e0.mAnim6 = 0;
        e0.mAnim7 = 0;
        e0.mAnim8 = 0;
        e0.mState = 0;
        e0.field_0x28 = 0;
        e0.mTimer = lbl_eu_80667DC0;
    }
    while (e < end) {
        e->mLayout = 0;
        e->mAnim1 = 0;
        e->mAnim2 = 0;
        e->mAnim3 = 0;
        e->mAnim4 = 0;
        e->mAnim5 = 0;
        e->mAnim6 = 0;
        e->mAnim7 = 0;
        e->mAnim8 = 0;
        e->mState = 0;
        e->field_0x28 = 0;
        e->mTimer = lbl_eu_80667DC0;
        ++e;
    }
    // Pass 2: stage a cleared slot through an address-taken local, then
    // blit it over each pair.
    for (int i = 0; i < 6; i += 2) {
        CMenuVisionEntry entry;
        CMenuVisionEntry* ep = &entry;
        ep->mLayout = 0;
        ep->mAnim1 = 0;
        ep->mAnim2 = 0;
        ep->mAnim3 = 0;
        ep->mAnim4 = 0;
        ep->mAnim5 = 0;
        ep->mAnim6 = 0;
        ep->mAnim7 = 0;
        ep->mAnim8 = 0;
        ep->mState = 0;
        ep->field_0x28 = 0;
        ep->mTimer = lbl_eu_80667DC0;
        self->mEntries[i] = *ep;
        self->mEntries[i + 1] = *ep;
    }
    return self;
}

// Complete-object destructor, forced-name form: the retail body behaves like
// a plain function (no vptr reset, single member-dtor call) - as a real C++
// virtual dtor MWCC adds the vptr store and double-destroys mLayoutMem.
// The nested null-check reproduces retail's guard pair; the conditional
// operator delete follows the deleting flag.
extern "C" void __dl__FPv(void*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void* __dt__11CMenuVisionFv(void* self, int flag) {
    if (self != 0) {
        __dt__17UnkClass_8045F564Fv((char*)self + 0x64, -1);
        if (self != 0) {
            if (self != 0) {
                __dt__8CProcessFv(reinterpret_cast<CProcess*>(self), 0);
            }
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" unsigned long func_801AC088() {
    return lbl_eu_80664388 != 0;
}

// Bitmask -> vision-slot lookup; returns true when the selected slot is
// currently animating (mState != 0). Bits: 1->0, 2->1, 4->2, 8->3, 0x10->5.
extern "C" bool func_801AC09C(u32 flags) {
    int index;
    if (flags & 1) {
        index = 0;
    } else if (flags & 2) {
        index = 1;
    } else if (flags & 4) {
        index = 2;
    } else if (flags & 8) {
        index = 3;
    } else if (flags & 0x10) {
        index = 5;
    } else {
        return false;
    }
    if (lbl_eu_80664388 != 0 &&
        lbl_eu_80664388->mEntries[index].mState != 0) {
        return true;
    }
    return false;
}

// Returns true while any vision slot is still animating.
extern "C" bool func_801AC124() {
    CMenuVision* menu = lbl_eu_80664388;
    if (menu != 0) {
        for (u8 i = 0; i < 6; i++) {
            if (menu->mEntries[i].mState != 0) {
                return true;
            }
        }
    }
    return false;
}
// Mark the vision screen active (set flag byte at 0x54) when the singleton
// instance exists.
extern "C" void func_801AC1F8() {
    if (lbl_eu_80664388 != 0) {
        lbl_eu_80664388->field_0x54 = 1;
    }
}

void CMenuVision::Term() {
    CDeviceVI::waitForDrawDone();

    // IScnRender subobject lives at +0x5C; the null-checked upcast keeps the
    // retail `mr r4,this; beq; addi r4,this,0x5c` shape.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = reinterpret_cast<IScnRender*>(&field_0x5C);
    }
    mScn->removeRenderCB(renderCB);

    // Release each per-slot layout (virtual deleting-dtor dispatch at vtable
    // slot +8) and re-null the slot; the extra inner null-check is the
    // D2-inlined-into-D1 artifact.
    for (u8 i = 0; i < 6; i++) {
        if (mEntries[i].mLayout != 0) {
            delete mEntries[i].mLayout;
            mEntries[i].mLayout = 0;
        }
    }
    mLayoutMem.func_8045F778();
    lbl_eu_80664388 = 0;
}

// us-801ae288: render-before callback. Same gate chain as Move(); only draws
// while some slot is animating, then renders all six layouts through a stack
// DrawInfo with Z-testing disabled.
void CMenuVision::cbRenderBefore() {
    // Combined first guard via || reproduces retail's short-circuit branch
    // shape (direct bne for clause 1, trampolined b for clause 2); the
    // remaining guards fold to direct branches.
    if (CTaskGame::getInstance()->isFlag01Set() || (lbl_eu_80663E28 & 0x200000)) {
        return;
    }
    if (!func_8013BE50()) {
        return;
    }
    if (lbl_eu_80663E24 & 0x2000000) {
        return;
    }

    bool allIdle = true;
    for (u8 i = 0; i < 6; i++) {
        if (mEntries[i].mState != 0) {
            allIdle = false;
            break;
        }
    }
    if (allIdle) {
        return;
    }

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);

    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls,
    // matching the retail direct calls (a C++ local would virtual-dispatch its
    // scope-exit destructor).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    for (u8 i = 0; i < 6; i++) {
        func_80137038(mEntries[i].mLayout,
                      (nw4r::lyt::DrawInfo*)&drawInfo[0], 0, 1);
    }
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
}

// Lazy singleton factory: allocate a CMenuVision from the work-thread heap,
// construct it with the given parent, register it on `self`, and stash the
// result in the global singleton slot. Only the first call sticks.
extern "C" CMenuVision* func_801ACCE0(CProcess* self, CProcess* parent) {
    if (lbl_eu_80664388 != 0) {
        return 0;
    }
    CMenuVision* obj = (CMenuVision*)mtl::MemManager::allocate(
        0x194, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuVision(obj, parent);
    }
    lbl_eu_80664388 = obj;
    Regist__8CProcessFP8CProcessb(reinterpret_cast<CProcess*>(obj), self, 0);
    return lbl_eu_80664388;
}

// Restart one slot into the state-5 sustain phase: disable anims 8 and 1-6,
// enable anim7 and rewind it, latch state 5.
static inline void menuVisionStartSustain(CMenuVisionEntry& e) {
    e.mLayout->SetAnimationEnable(e.mAnim8, false);
    e.mLayout->SetAnimationEnable(e.mAnim1, false);
    e.mLayout->SetAnimationEnable(e.mAnim2, false);
    e.mLayout->SetAnimationEnable(e.mAnim3, false);
    e.mLayout->SetAnimationEnable(e.mAnim4, false);
    e.mLayout->SetAnimationEnable(e.mAnim5, false);
    e.mLayout->SetAnimationEnable(e.mAnim6, false);
    e.mLayout->SetAnimationEnable(e.mAnim7, true);
    e.mAnim7->SetFrame(lbl_eu_80667DC0);
    e.mState = 5;
}

// Disable/enable pass only (no SetFrame/state latch).
static inline void menuVisionResetAnims(CMenuVisionEntry& e) {
    e.mLayout->SetAnimationEnable(e.mAnim8, false);
    e.mLayout->SetAnimationEnable(e.mAnim1, false);
    e.mLayout->SetAnimationEnable(e.mAnim2, false);
    e.mLayout->SetAnimationEnable(e.mAnim3, false);
    e.mLayout->SetAnimationEnable(e.mAnim4, false);
    e.mLayout->SetAnimationEnable(e.mAnim5, false);
    e.mLayout->SetAnimationEnable(e.mAnim6, false);
    e.mLayout->SetAnimationEnable(e.mAnim7, true);
}

// Bitmask-triggered slot restart: bits 1/2/4/8/0x10 select slots 0/1/2/3/5
// (bit 0x10 also restarts slot 4). Slots idle (0) or bar-sustaining (4)
// abort the whole call. Each restarted slot gets a timer weighted by how many
// lower slots are also in state 5.
void func_801ACD5C(int flags) {
    if (lbl_eu_80664388 == 0) {
        return;
    }

    if (flags & 1) {
        CMenuVisionEntry& e = lbl_eu_80664388->mEntries[0];
        if (e.mState == 0) {
            return;
        }
        if (e.mState == 4) {
            return;
        }
        menuVisionStartSustain(e);
        e.mTimer = lbl_eu_80667DC0;
    }

    if (flags & 2) {
        CMenuVisionEntry& e = lbl_eu_80664388->mEntries[1];
        if (e.mState == 0) {
            return;
        }
        if (e.mState == 4) {
            return;
        }
        menuVisionStartSustain(e);
        f32 t = lbl_eu_80667DC0;
        if (lbl_eu_80664388->mEntries[0].mState == 5) {
            t = lbl_eu_80667DCC;
        }
        e.mTimer = t;
    }

    if (flags & 4) {
        CMenuVisionEntry& e = lbl_eu_80664388->mEntries[2];
        if (e.mState == 0) {
            return;
        }
        if (e.mState == 4) {
            return;
        }
        menuVisionStartSustain(e);
        f32 t = lbl_eu_80667DC0;
        if (lbl_eu_80664388->mEntries[1].mState == 5) {
            t = t + lbl_eu_80667DC4;
        }
        if (lbl_eu_80664388->mEntries[0].mState == 5) {
            t = t + lbl_eu_80667DC4;
        }
        e.mTimer = lbl_eu_80667DCC * t;
    }

    if (flags & 8) {
        CMenuVisionEntry& e = lbl_eu_80664388->mEntries[3];
        if (e.mState == 0) {
            return;
        }
        if (e.mState == 4) {
            return;
        }
        menuVisionStartSustain(e);
        f32 t = lbl_eu_80667DC0;
        if (lbl_eu_80664388->mEntries[2].mState == 5) {
            t = t + lbl_eu_80667DC4;
        }
        if (lbl_eu_80664388->mEntries[1].mState == 5) {
            t = t + lbl_eu_80667DC4;
        }
        if (lbl_eu_80664388->mEntries[0].mState == 5) {
            t = t + lbl_eu_80667DC4;
        }
        e.mTimer = lbl_eu_80667DCC * t;
    }

    if (flags & 0x10) {
        // Slot 5 restart also rewinds the HP-bar slot 4.
        CMenuVisionEntry& e5 = lbl_eu_80664388->mEntries[5];
        if (e5.mState == 0) {
            return;
        }
        if (e5.mState == 4) {
            return;
        }
        menuVisionStartSustain(e5);
        CMenuVisionEntry& e4 = lbl_eu_80664388->mEntries[4];
        menuVisionResetAnims(e4);
        lbl_eu_80664388->mEntries[4].mAnim7->SetFrame(lbl_eu_80667DC0);
        lbl_eu_80664388->mEntries[4].mState = 5;
        f32 t = lbl_eu_80667DC0;
        if (lbl_eu_80664388->mEntries[3].mState == 5) {
            t = t + lbl_eu_80667DC4;
        }
        if (lbl_eu_80664388->mEntries[2].mState == 5) {
            t = t + lbl_eu_80667DC4;
        }
        if (lbl_eu_80664388->mEntries[1].mState == 5) {
            t = t + lbl_eu_80667DC4;
        }
        if (lbl_eu_80664388->mEntries[0].mState == 5) {
            t = t + lbl_eu_80667DC4;
        }
        t = lbl_eu_80667DCC * t;
        e5.mTimer = t;
        lbl_eu_80664388->mEntries[4].mTimer = t;
    }
}

extern "C" void func_801AD504(int flags) {
    if (lbl_eu_80664388 == 0) {
        return;
    }

    MenuVisionBattleData* battle = static_cast<MenuVisionBattleData*>(func_800EA444(cf::CBattleManager::getInstance()));
    if (battle == 0) {
        return;
    }

    if (flags & 1) {
        CMenuVision* menu = lbl_eu_80664388;
        CMenuVisionEntry& entry = menu->mEntries[0];
        if (entry.mState != 0) {
            return;
        }
        void* actorBase;
        void* image = 0;
        void* actor = findObjectById__Fi(battle->actor);
        if (actor == 0) {
            return;
        }
        if (*(u32*)((u8*)actor + 0x64) & 4) {
            void* actorBase = actor;
            if (actor != 0) {
                actorBase = (u8*)actor - 0x3E9C;
            }
            if (battle->flags824 & 0x20000) {
                image = menuVisionResource(lbl_eu_80504268 + 0x21D);
            } else {
                switch (menuVisionActorKind(actorBase)) {
                case 3: case 4: case 5: case 6: case 8: case 12:
                    image = menuVisionResource(lbl_eu_80504268 + 0x232); break;
                case 1:
                    if (*(u16*)((u8*)actorBase + 0x3F28) == 0x1A1) {
                        image = menuVisionResource(lbl_eu_80504268 + 0x247);
                    } else {
                        image = menuVisionResource(lbl_eu_80504268 + 0x25C);
                    }
                    break;
                case 9:
                    image = menuVisionResource(lbl_eu_80504268 + 0x271); break;
                case 7:
                    image = menuVisionResource(lbl_eu_80504268 + 0x286); break;
                case 10:
                    image = menuVisionResource(lbl_eu_80504268 + 0x29B); break;
                case 2:
                    switch (*(u16*)((u8*)actorBase + 0x3F28)) {
                    case 0x3D: case 0x3E: case 0x4B5:
                    case 0x5FF: case 0x600: case 0x601: case 0x961:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2B0); break;
                    case 0x10C: case 0x1A0: case 0x1A1: case 0x602:
                    case 0x835: case 0x8A8: case 0x8AC: case 0x962: case 0x963:
                        image = menuVisionResource(lbl_eu_80504268 + 0x247); break;
                    case 0x77A: case 0x843: case 0x844: case 0x964:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2C5); break;
                    case 0x656: case 0x84B: case 0x9C5: case 0x9C6: case 0x9C7:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2DA); break;
                    case 0x652: case 0x653:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2EF); break;
                    case 0x59C:
                        image = menuVisionResource(lbl_eu_80504268 + 0x304); break;
                    case 0x845:
                        image = menuVisionResource(lbl_eu_80504268 + 0x319); break;
                    }
                    break;
                case 11:
                    switch (*(u16*)((u8*)actorBase + 0x3F28)) {
                    case 0x965: image = menuVisionResource(lbl_eu_80504268 + 0x271); break;
                    case 0x961: image = menuVisionResource(lbl_eu_80504268 + 0x2B0); break;
                    case 0x962: case 0x963: image = menuVisionResource(lbl_eu_80504268 + 0x247); break;
                    case 0x964: image = menuVisionResource(lbl_eu_80504268 + 0x2C5); break;
                    case 0x524: case 0x525: image = menuVisionResource(lbl_eu_80504268 + 0x32E); break;
                    case 0x905: image = menuVisionResource(lbl_eu_80504268 + 0x343); break;
                    case 0x9C9: image = menuVisionResource(lbl_eu_80504268 + 0x358); break;
                    case 0x969: case 0x96A: case 0x96C: image = menuVisionResource(lbl_eu_80504268 + 0x36D); break;
                    case 0x96B: image = menuVisionResource(lbl_eu_80504268 + 0x382); break;
                    case 0x967: case 0x968: image = menuVisionResource(lbl_eu_80504268 + 0x397); break;
                    default: image = menuVisionResource(lbl_eu_80504268 + 0x3AC); break;
                    }
                    break;
                }
            }
        }
        menuVisionSetImage(entry, lbl_eu_80504268 + 0x3C1, image);
        menuVisionBegin(entry);
    }

    if (flags & 2) {
        CMenuVision* menu = lbl_eu_80664388;
        CMenuVisionEntry& entry = menu->mEntries[1];
        u32 font = func_800F4784(battle);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1AB, (char*)font, 0);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1B8, (char*)font, 0);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1C5, (char*)font, 0);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1D2, (char*)font, 0);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1DF, (char*)font, 0);
        func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1ED, (char*)font, 0);

        u32 color1 = lbl_eu_80662580;
        u32 color2 = lbl_eu_80662584;
        u32 color3 = lbl_eu_80662588;
        void* actor = findObjectById__Fi(battle->actor);
        if (actor != 0) {
            actor = (u8*)actor - 0x3E9C;
        }
        if (actor == 0) {
            return;
        }
        MenuVisionActorInfo* info = menuVisionActorInfo(actor);
        if (info == 0) {
            return;
        }
        switch ((int)info->mode42) {
        case 1:
            color1 = lbl_eu_80662594;
            color2 = lbl_eu_806625A4;
            color3 = lbl_eu_806625B4;
            break;
        case 10:
            color1 = lbl_eu_8066258C;
            color2 = lbl_eu_80662598;
            color3 = lbl_eu_806625A8;
            break;
        default:
            switch ((int)info->mode3C) {
            case 1:
                color1 = lbl_eu_80662590;
                color2 = lbl_eu_8066259C;
                color3 = lbl_eu_806625AC;
                break;
            case 2:
            case 3:
                color1 = lbl_eu_8066258C;
                color2 = lbl_eu_80662598;
                color3 = lbl_eu_806625A8;
                break;
            }
            break;
        }
        func_80139B5C(entry.mLayout, lbl_eu_80504268 + 0x1AB, color1);
        func_80139B5C(entry.mLayout, lbl_eu_80504268 + 0x1B8, color1);
        func_80139B5C(entry.mLayout, lbl_eu_80504268 + 0x1C5, color1);
        func_80139B5C(entry.mLayout, lbl_eu_80504268 + 0x1D2, color1);
        func_80139BF4(entry.mLayout, lbl_eu_80504268 + 0x1ED, color2, color3);

        u16 range = *(u16*)((u8*)func_800F477C(battle) + 0x5E);
        void* image;
        switch (range) {
        case 4:
        case 6:
            image = menuVisionResource(lbl_eu_80504268 + 0x3D0);
            break;
        case 1:
            image = menuVisionResource(lbl_eu_80504268 + 0x3E5);
            break;
        case 5:
            image = menuVisionResource(lbl_eu_80504268 + 0x3FA);
            break;
        default:
            image = menuVisionResource(lbl_eu_80504268 + 0x40F);
            break;
        }
        if (image != 0) {
            func_80137E7C(entry.mLayout, (char*)(lbl_eu_80504268 + 0x422), image);
        }
        menuVisionBeginWithoutSecond(entry);
    }

    if (flags & 4) {
        CMenuVision* menu = lbl_eu_80664388;
        CMenuVisionEntry& entry = menu->mEntries[2];
        nw4r::lyt::Pane* pane =
            entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x42A, true);
        pane->SetVisible(true);
        void* image = 0;
        if (battle->flags824 & 0x20000) {
            image = menuVisionResource(lbl_eu_80504268 + 0x439);
        } else {
            void* actor = findObjectById__Fi(battle->targetActor);
            if (actor == 0) {
                return;
            }
            u32 actorFlags = *(u32*)((u8*)actor + 0x64);
            if (actorFlags & 4) {
                void* actorBase = actor;
                if (actor != 0) {
                    actorBase = (u8*)actor - 0x3E9C;
                }
                switch (menuVisionActorKind(actorBase)) {
                case 3: case 4: case 5: case 6: case 8: case 12:
                    image = menuVisionResource(lbl_eu_80504268 + 0x232); break;
                case 1:
                    if (*(u16*)((u8*)actorBase + 0x3F28) == 0x1A1) {
                        image = menuVisionResource(lbl_eu_80504268 + 0x247);
                    } else {
                        image = menuVisionResource(lbl_eu_80504268 + 0x25C);
                    }
                    break;
                case 9:
                    image = menuVisionResource(lbl_eu_80504268 + 0x271); break;
                case 7:
                    image = menuVisionResource(lbl_eu_80504268 + 0x286); break;
                case 10:
                    image = menuVisionResource(lbl_eu_80504268 + 0x29B); break;
                case 2:
                    switch (*(u16*)((u8*)actorBase + 0x3F28)) {
                    case 0x3D: case 0x3E: case 0x4B5:
                    case 0x5FF: case 0x600: case 0x601: case 0x961:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2B0); break;
                    case 0x10C: case 0x1A0: case 0x1A1: case 0x602:
                    case 0x835: case 0x8A8: case 0x8AC: case 0x962: case 0x963:
                        image = menuVisionResource(lbl_eu_80504268 + 0x247); break;
                    case 0x77A: case 0x843: case 0x844: case 0x964:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2C5); break;
                    case 0x656: case 0x84B: case 0x9C5: case 0x9C6: case 0x9C7:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2DA); break;
                    case 0x652: case 0x653:
                        image = menuVisionResource(lbl_eu_80504268 + 0x2EF); break;
                    case 0x59C:
                        image = menuVisionResource(lbl_eu_80504268 + 0x304); break;
                    case 0x845:
                        image = menuVisionResource(lbl_eu_80504268 + 0x319); break;
                    }
                    break;
                case 11:
                    switch (*(u16*)((u8*)actorBase + 0x3F28)) {
                    case 0x965: image = menuVisionResource(lbl_eu_80504268 + 0x271); break;
                    case 0x961: image = menuVisionResource(lbl_eu_80504268 + 0x2B0); break;
                    case 0x962: case 0x963: image = menuVisionResource(lbl_eu_80504268 + 0x247); break;
                    case 0x964: image = menuVisionResource(lbl_eu_80504268 + 0x2C5); break;
                    case 0x524: case 0x525: image = menuVisionResource(lbl_eu_80504268 + 0x32E); break;
                    case 0x905: image = menuVisionResource(lbl_eu_80504268 + 0x343); break;
                    case 0x9C9: image = menuVisionResource(lbl_eu_80504268 + 0x358); break;
                    case 0x969: case 0x96A: case 0x96C: image = menuVisionResource(lbl_eu_80504268 + 0x36D); break;
                    case 0x96B: image = menuVisionResource(lbl_eu_80504268 + 0x382); break;
                    case 0x967: case 0x968: image = menuVisionResource(lbl_eu_80504268 + 0x397); break;
                    default: image = menuVisionResource(lbl_eu_80504268 + 0x3AC); break;
                    }
                    break;
                }
            } else if (actorFlags & 2) {
                if (actor != 0) {
                    actor = (u8*)actor - 0x3E9C;
                }
                switch (*(u16*)((u8*)actor + 0x3F28)) {
                case 1: image = menuVisionResource(lbl_eu_80504268 + 0x44E); break;
                case 2: image = menuVisionResource(lbl_eu_80504268 + 0x462); break;
                case 3: image = menuVisionResource(lbl_eu_80504268 + 0x476); break;
                case 4: image = menuVisionResource(lbl_eu_80504268 + 0x48A); break;
                case 5: image = menuVisionResource(lbl_eu_80504268 + 0x49E); break;
                case 6: image = menuVisionResource(lbl_eu_80504268 + 0x4B2); break;
                case 7: image = menuVisionResource(lbl_eu_80504268 + 0x4C6); break;
                case 8: image = menuVisionResource(lbl_eu_80504268 + 0x4DA); break;
                case 9: image = menuVisionResource(lbl_eu_80504268 + 0x4EE); break;
                case 10: image = menuVisionResource(lbl_eu_80504268 + 0x502); break;
                case 11: image = menuVisionResource(lbl_eu_80504268 + 0x516); break;
                }
            }
        }
        menuVisionSetImage(entry, lbl_eu_80504268 + 0x42A, image);
        menuVisionBegin(entry);
    }

    if (flags & 8) {
        CMenuVision* menu = lbl_eu_80664388;
        CMenuVisionEntry& entry = menu->mEntries[3];
        nw4r::lyt::Pane* status = entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x52A, true);
        nw4r::lyt::Pane* statusAlt = entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x537, true);
        nw4r::lyt::Pane* panic = entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x546, true);
        entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x1FB, true)->SetVisible(false);
        entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x550, true)->SetVisible(false);
        entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x55D, true)->SetVisible(false);
        status->SetVisible(false);
        statusAlt->SetVisible(false);
        panic->SetVisible(false);

        if (battle->flags824 & 0x20000) {
            entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x550, true)->SetVisible(true);
        } else {
            entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x1FB, true)->SetVisible(true);
            if ((battle->flags84 & 1) || (battle->flags88 & 0x100) || func_800F4648(battle) > 0) {
                ((MenuVisionSetDamageText)func_80136910__FPQ34nw4r3lyt6LayoutPcUc)(
                    entry.mLayout, (char*)(lbl_eu_80504268 + 0x1FB), func_800F4648(battle));
                nw4r::math::VEC2 scale;
                scale.x = lbl_eu_80667DD4;
                scale.y = lbl_eu_80667DD4;
                PaneScaleAccess* pane = reinterpret_cast<PaneScaleAccess*>(
                    entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x1FB, true));
                pane->scaleX = scale.x;
                pane->scaleY = scale.y;
            } else {
                char* text = func_80136190(lbl_eu_80504268 + 0x56B, lbl_eu_80504268 + 0x576, 11);
                func_80136B4C(entry.mLayout, lbl_eu_80504268 + 0x1FB, text, 0);
                nw4r::math::VEC2 scale;
                scale.x = lbl_eu_80667DC4;
                scale.y = lbl_eu_80667DC4;
                PaneScaleAccess* pane = reinterpret_cast<PaneScaleAccess*>(
                    entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x1FB, true));
                pane->scaleX = scale.x;
                pane->scaleY = scale.y;
            }

            VisionQuad first = lbl_eu_80664338;
            VisionQuad second = lbl_eu_80664340;
            u32 color1 = lbl_eu_80662584;
            u32 color2 = lbl_eu_80662588;
            void* actor = findObjectById__Fi(battle->actor);
            if (actor != 0) {
                actor = (u8*)actor - 0x3E9C;
            }
            if (actor == 0) {
                return;
            }
            MenuVisionActorInfo* info = menuVisionActorInfo(actor);
            if (info == 0) {
                return;
            }
            switch ((int)info->mode42) {
            case 1:
                menuVisionCopyQuad(first, lbl_eu_80664378);
                menuVisionCopyQuad(second, lbl_eu_80664380);
                color1 = lbl_eu_806625A4;
                color2 = lbl_eu_806625B4;
                break;
            case 10:
                menuVisionCopyQuad(first, lbl_eu_80664368);
                menuVisionCopyQuad(second, lbl_eu_80664370);
                color1 = lbl_eu_806625A0;
                color2 = lbl_eu_806625B0;
                break;
            default:
                switch ((int)info->mode3C) {
                case 1:
                    menuVisionCopyQuad(first, lbl_eu_80664358);
                    menuVisionCopyQuad(second, lbl_eu_80664360);
                    color1 = lbl_eu_8066259C;
                    color2 = lbl_eu_806625AC;
                    break;
                case 2:
                    menuVisionCopyQuad(first, lbl_eu_80664348);
                    menuVisionCopyQuad(second, lbl_eu_80664350);
                    color1 = lbl_eu_80662598;
                    color2 = lbl_eu_806625A8;
                    break;
                }
                break;
            }
            func_80139BF4(entry.mLayout, lbl_eu_80504268 + 0x1FB, color1, color2);
            func_80139A18(entry.mLayout, lbl_eu_80504268 + 0x1FB, &first, &second);

            u32 current = battle->value828;
            u32 previous = battle->value82C;
            if (current == previous) {
                previous = 0;
            }
            u8 ids[4];
            *(u32*)ids = lbl_eu_80667DD0;
            bool known = false;
            if (previous == ids[0]) {
                known = true;
            } else if (previous == ids[1]) {
                known = true;
            } else if (previous == ids[2]) {
                known = true;
            } else if (previous == ids[3]) {
                known = true;
            }
            if (known) {
                if (previous != 0) {
                    const char* images[4] = {lbl_eu_80504238[0], lbl_eu_80504238[1],
                                             lbl_eu_80504238[2], lbl_eu_80504238[3]};
                    unsigned int i = 0;
                    do {
                        if (previous == ids[(u8)i]) {
                            void* image = menuVisionResource(images[(u8)i]);
                            if (image != 0) {
                                func_80137F88(panic, image);
                                panic->SetVisible(true);
                            }
                            break;
                        }
                        i++;
                    } while (i < 4);
                    if (!(reinterpret_cast<PaneVisAccess*>(panic)->visByte & 1)) {
                        u16 id = func_80136254(lbl_eu_806640E0, lbl_eu_80504268 + 0x57B, previous);
                        if (id != 0) {
                            menuVisionReplacePaneImage(status, menuVisionResource(func_80138F78(id)));
                        }
                    }
                }
                if (current != 0) {
                    u16 id = func_80136254(lbl_eu_806640E0, lbl_eu_80504268 + 0x57B, current);
                    if (id != 0) {
                        menuVisionReplacePaneImage(statusAlt, menuVisionResource(func_80138F78(id)));
                    }
                }
            } else {
                if (current != 0) {
                    const char* images[4] = {lbl_eu_80504248[0], lbl_eu_80504248[1],
                                             lbl_eu_80504248[2], lbl_eu_80504248[3]};
                    unsigned int i = 0;
                    do {
                        if (current == ids[(u8)i]) {
                            void* image = menuVisionResource(images[(u8)i]);
                            if (image != 0) {
                                func_80137F88(panic, image);
                                panic->SetVisible(true);
                            }
                            break;
                        }
                        i++;
                    } while (i < 4);
                    if (!(reinterpret_cast<PaneVisAccess*>(panic)->visByte & 1)) {
                        u16 id = func_80136254(lbl_eu_806640E0, lbl_eu_80504268 + 0x57B, current);
                        if (id != 0) {
                            menuVisionReplacePaneImage(status, menuVisionResource(func_80138F78(id)));
                        }
                    }
                }
                if (previous != 0) {
                    u16 id = func_80136254(lbl_eu_806640E0, lbl_eu_80504268 + 0x57B, previous);
                    if (id != 0) {
                        menuVisionReplacePaneImage(statusAlt, menuVisionResource(func_80138F78(id)));
                    }
                }
            }

            if (battle->targetActor != 0) {
                void* target = findObjectById__Fi(battle->targetActor);
                if (target != 0) {
                    target = (u8*)target - 0x3E9C;
                }
                if (target != 0 && (battle->flags824 & 0x400)) {
                    menuVisionSetVisible(entry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x55D, true), true);
                }
            }
        }
        menuVisionBegin(entry);
    }

    if (flags & 0x10) {
        CMenuVision* menu = lbl_eu_80664388;
        CMenuVisionEntry& entry = menu->mEntries[5];
        int value = (int)func_800F42AC(battle);
        if (value > 999) {
            value = 999;
        } else if (value < 0) {
            value = 0;
        }
        ((MenuVisionSetDamageText)func_80136910__FPQ34nw4r3lyt6LayoutPcUc)(
            entry.mLayout, (char*)(lbl_eu_80504268 + 0x208), value);
        menuVisionBegin(entry);

        CMenuVisionEntry& barEntry = menu->mEntries[4];
        nw4r::lyt::Pane* bar = barEntry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x215, true);
        PaneTransAccess* trans = reinterpret_cast<PaneTransAccess*>(bar);
        nw4r::math::VEC2 position;
        position.x = trans->transX;
        position.y = trans->transY;
        position.x = lbl_eu_80667DC8 * func_800F4424(battle);
        bar = barEntry.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x215, true);
        trans = reinterpret_cast<PaneTransAccess*>(bar);
        trans->transX = position.x;
        trans->transY = position.y;
        menuVisionBegin(barEntry);
    }
}

// Static initializer: seed the ten .sbss colour quads above.

#pragma pop

// Reset every active vision slot to state 4 (bar-sustain anim): disable
// anims 1-3 and 5-8, enable anim4 and rewind it to frame 0. When `sel` is
// non-zero only the matching slot index is reset.
void func_801AF934(int sel) {
    if (lbl_eu_80664388 == 0) {
        return;
    }
    if (func_800EA444(cf::CBattleManager::getInstance()) == 0) {
        return;
    }

    const f32 startFrame = lbl_eu_80667DC0;
    for (u8 i = 0; i < 6; i++) {
        CMenuVisionEntry& e = lbl_eu_80664388->mEntries[i];
        if (e.mState == 0) {
            continue;
        }
        // sel == 0 means "reset all"; otherwise only slot `sel` is reset.
        if ((signed char)sel != 0 && (signed char)sel != (int)i) {
            continue;
        }
        e.mLayout->SetAnimationEnable(e.mAnim5, false);
        e.mLayout->SetAnimationEnable(e.mAnim6, false);
        e.mLayout->SetAnimationEnable(e.mAnim7, false);
        e.mLayout->SetAnimationEnable(e.mAnim8, false);
        e.mLayout->SetAnimationEnable(e.mAnim1, false);
        e.mLayout->SetAnimationEnable(e.mAnim2, false);
        e.mLayout->SetAnimationEnable(e.mAnim3, false);
        e.mLayout->SetAnimationEnable(e.mAnim4, true);
        e.mAnim4->SetFrame(startFrame);
        e.mState = 4;
    }
}

// Mangled linker names used by adjustor thunks below

void func_801AFE04(void* self) { ((void(*)(void*))__dt__11CMenuVisionFv)((char*)self - 0x58); }

void func_801AFE0C(void* self) { ((void(*)(void*))cbRenderBefore__11CMenuVisionFv)((char*)self - 0x5c); }

void func_801AFE14(void* self) { ((void(*)(void*))__dt__11CMenuVisionFv)((char*)self - 0x5c); }

// Static initializer: seed the ten .sbss colour quads above.
void sinit_801AFCE8() {
    // Component order matches retail's register allocation.
    // Component order [3],[2],[1],[0] matches retail's register allocation.
    lbl_eu_80664338.values[3] = 0;
    lbl_eu_80664338.values[2] = 0x8a;
    lbl_eu_80664338.values[1] = 0x73;
    lbl_eu_80664338.values[0] = 0x3c;
    lbl_eu_80664340.values[3] = 0xff;
    lbl_eu_80664340.values[2] = 0xfa;
    lbl_eu_80664340.values[1] = 0xf0;
    lbl_eu_80664340.values[0] = 0xf0;
    lbl_eu_80664348.values[3] = 0;
    lbl_eu_80664348.values[2] = 0xd2;
    lbl_eu_80664348.values[1] = 0x14;
    lbl_eu_80664348.values[0] = 0x40;
    lbl_eu_80664350.values[3] = 0xff;
    lbl_eu_80664350.values[2] = 0xff;
    lbl_eu_80664350.values[1] = 0xff;
    lbl_eu_80664350.values[0] = 0xff;
    lbl_eu_80664358.values[3] = 0;
    lbl_eu_80664358.values[2] = 0x1d;
    lbl_eu_80664358.values[1] = 0x2d;
    lbl_eu_80664358.values[0] = 0xd4;
    lbl_eu_80664360.values[3] = 0xff;
    lbl_eu_80664360.values[2] = 0xff;
    lbl_eu_80664360.values[1] = 0xff;
    lbl_eu_80664360.values[0] = 0xff;
    lbl_eu_80664368.values[3] = 0;
    lbl_eu_80664368.values[2] = 0x10;
    lbl_eu_80664368.values[1] = 0x9f;
    lbl_eu_80664368.values[0] = 0x10;
    lbl_eu_80664370.values[3] = 0xff;
    lbl_eu_80664370.values[2] = 0xff;
    lbl_eu_80664370.values[1] = 0xff;
    lbl_eu_80664370.values[0] = 0xff;
    lbl_eu_80664378.values[3] = 0;
    lbl_eu_80664378.values[2] = 0xf;
    lbl_eu_80664378.values[1] = 0xf;
    lbl_eu_80664378.values[0] = 0xf;
    lbl_eu_80664380.values[3] = 0xff;
    lbl_eu_80664380.values[2] = 0xff;
    lbl_eu_80664380.values[1] = 0xff;
    lbl_eu_80664380.values[0] = 0xff;
}

// --- CMenuVision::Move ---
void CMenuVision::Move() {
    // Early-out guards: game paused, UI-suppression bits, or no active entry.
    if (CTaskGame::getInstance()->isFlag01Set()) {
        goto L_ret;
    }
    if ((lbl_eu_80663E28 & 0x200000) != 0) {
        goto L_ret;
    }
    goto L_continue;
L_ret:
    return;
L_continue:
    if (!func_8013BE50()) {
        goto L_ret;
    }
    if (lbl_eu_80663E24 & 0x2000000) {
        goto L_ret;
    }

    // Scan the 6 slots for any entry currently animating (mState != 0).
    bool noActive = true;
    for (u8 i = 0; i < 6; i++) {
        if (mEntries[i].mState != 0) {
            noActive = false;
            break;
        }
    }
    if (noActive) {
        goto L_ret;
    }

    void* bmObj;
    for (u8 i = 0; i < 6; i++) {
        CMenuVisionEntry* e = &mEntries[i];

        switch (e->mState) {
        case 1:
            // Intro anim (mAnim1) finished -> switch to the sustain phase.
            if (func_80137444(e->mAnim1, lbl_eu_80667DC4)) {
                e->mState = 2;
            }
            break;

        case 2: {
            bmObj = func_800EA444(cf::CBattleManager::getInstance());
            if (bmObj != NULL) {
                // While the HP-bar pane is visible, track the target position.
                nw4r::lyt::Pane* pane1 = e->mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x195, true);
                if (reinterpret_cast<PaneVisAccess*>(pane1)->visByte & 1) {
                    nw4r::lyt::Pane* posPane = e->mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x215, true);
                    PaneTransAccess* trans = reinterpret_cast<PaneTransAccess*>(posPane);
                    nw4r::math::VEC2 position;
                    position.x = trans->transX;
                    position.y = trans->transY;
                    position.x = lbl_eu_80667DC8 * func_800F4424(bmObj);
                    posPane = e->mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x215, true);
                    trans = reinterpret_cast<PaneTransAccess*>(posPane);
                    trans->transX = position.x;
                    trans->transY = position.y;
                }
            } else {
                // Battle ended: stop every anim except mAnim2 and play it from 0.
                e->mLayout->SetAnimationEnable(e->mAnim3, false);
                e->mLayout->SetAnimationEnable(e->mAnim4, false);
                e->mLayout->SetAnimationEnable(e->mAnim5, false);
                e->mLayout->SetAnimationEnable(e->mAnim6, false);
                e->mLayout->SetAnimationEnable(e->mAnim7, false);
                e->mLayout->SetAnimationEnable(e->mAnim8, false);
                e->mLayout->SetAnimationEnable(e->mAnim1, false);
                e->mLayout->SetAnimationEnable(e->mAnim2, true);
                e->mAnim2->SetFrame(lbl_eu_80667DC0);
                e->mState = 3;
            }
            break;
        }

        case 3:
            // mAnim2 finished -> idle.
            if (func_80137444(e->mAnim2, lbl_eu_80667DC4)) {
                e->mState = 0;
            }
            break;

        case 4:
            // mAnim4 finished -> idle.
            if (func_80137444(e->mAnim4, lbl_eu_80667DC4)) {
                e->mState = 0;
            }
            break;

        case 5:
            func_801AFAD0(this, e);
            break;
        }

        e->mLayout->Animate(0);
    }
}

// --- CMenuVision::Init ---
void CMenuVision::Init() {
    char* strBase = lbl_eu_80504268;
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mLayoutMem.createRegion(handle, 0x2C000, strBase, 0);

    Class_8045F858 regionGuard(&mLayoutMem);

    // Copy 6 pane-name pointers from lbl_eu_805041C0 to local array
    u32 paneNames[6];
    u32* namePtr = lbl_eu_805041C0;
    paneNames[0] = *namePtr++;
    paneNames[1] = namePtr[0];
    paneNames[2] = namePtr[1];
    paneNames[3] = namePtr[2];
    paneNames[4] = namePtr[3];
    paneNames[5] = namePtr[4];

    nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();

    for (u8 i = 0; i < 6; i++) {
        CMenuVisionEntry& e = mEntries[i];

        func_80136E84(&e.mLayout, accessor, lbl_eu_80504268 + 0xb);
        func_80136F08(e.mLayout, &e.mAnim1, accessor, lbl_eu_80504268 + 0x2b);
        func_80136F08(e.mLayout, &e.mAnim2, accessor, lbl_eu_80504268 + 0x4e);
        func_80136F08(e.mLayout, &e.mAnim3, accessor, lbl_eu_80504268 + 0x72);
        func_80136F08(e.mLayout, &e.mAnim4, accessor, lbl_eu_80504268 + 0x9a);
        func_80136F08(e.mLayout, &e.mAnim5, accessor, lbl_eu_80504268 + 0xc5);
        func_80136F08(e.mLayout, &e.mAnim6, accessor, lbl_eu_80504268 + 0xef);
        func_80136F08(e.mLayout, &e.mAnim7, accessor, lbl_eu_80504268 + 0x119);
        func_80136F08(e.mLayout, &e.mAnim8, accessor, lbl_eu_80504268 + 0x13f);

        // Disable anims 2-8, enable anim1, reset to frame 0
        e.mLayout->SetAnimationEnable(e.mAnim2, false);
        e.mLayout->SetAnimationEnable(e.mAnim3, false);
        e.mLayout->SetAnimationEnable(e.mAnim4, false);
        e.mLayout->SetAnimationEnable(e.mAnim5, false);
        e.mLayout->SetAnimationEnable(e.mAnim6, false);
        e.mLayout->SetAnimationEnable(e.mAnim7, false);
        e.mLayout->SetAnimationEnable(e.mAnim8, false);
        e.mLayout->SetAnimationEnable(e.mAnim1, true);
        e.mAnim1->SetFrame(lbl_eu_80667DC0);
        e.mLayout->Animate(0);

        // Hide 6 standard panes by clearing bit 0 of visByte
        nw4r::lyt::Pane* p = e.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x169, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = e.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x174, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = e.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x17f, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = e.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x18a, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = e.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x195, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        p = e.mLayout->GetRootPane()->FindPaneByName(lbl_eu_80504268 + 0x1a2, true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;

        // Show the entry-specific pane (retail clears bit 0 first, then sets it)
        p = e.mLayout->GetRootPane()->FindPaneByName((const char*)paneNames[i], true);
        reinterpret_cast<PaneVisAccess*>(p)->visByte &= ~1;
        reinterpret_cast<PaneVisAccess*>(p)->visByte |= 1;

        // Per-index special setup
        if (i == 1) {
            CMenuVisionFontView* fontObj = reinterpret_cast<CMenuVisionFontView*>(
                getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1));
            u32 fontVal = fontObj->getFontHandle();

            func_801368C0(e.mLayout, lbl_eu_80504268 + 0x1ab, fontVal);
            func_801368C0(e.mLayout, lbl_eu_80504268 + 0x1b8, fontVal);
            func_801368C0(e.mLayout, lbl_eu_80504268 + 0x1c5, fontVal);
            func_801368C0(e.mLayout, lbl_eu_80504268 + 0x1d2, fontVal);
            func_801368C0(e.mLayout, lbl_eu_80504268 + 0x1df, fontVal);
            func_801368C0(e.mLayout, lbl_eu_80504268 + 0x1ed, fontVal);
        } else if (i == 3) {
            u32 val = func_801355A0();
            func_801368C0(e.mLayout, lbl_eu_80504268 + 0x1fb, val);
        } else if (i == 5) {
            u32 val = func_801355A0();
            func_801368C0(e.mLayout, lbl_eu_80504268 + 0x208, val);
        }
    }

    // Register render-before callback (null-safe IScnRender subobject at +0x5C)
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&field_0x5C);
    }
    mScn->addRenderCB(cb, 0xF, 0);
}

// State-5 per-frame update: counts down the slot timer; once it lapses and
// the damage anim (mAnim7) is still running below its end frame, picks the
// first visible status pane, records the matching effect id, and latches the
// slot's field_0x28 so the update runs only once.
void func_801AFAD0(CMenuVision*, CMenuVisionEntry* entry) {
    entry->mTimer -= lbl_eu_80667DC4;
    if (entry->mTimer >= lbl_eu_80667DC0) {
        return;
    }
    // Cue-frame jingle once the timer lapses to the exact cue value.
    if (!(lbl_eu_80667DD8 == entry->mTimer)) {
    } else {
        func_80138078__FUl(0x1c4);
    }
    if (func_80137444(entry->mAnim7, lbl_eu_80667DC4)) {
        entry->mState = 0;
    }
    if (!(reinterpret_cast<AnimFrameAccess*>(entry->mAnim7)->frame >= lbl_eu_80667DDC)) {
        return;
    }
    if (entry->field_0x28 != 0) {
        return;
    }

    nw4r::lyt::Pane* root = entry->mLayout->GetRootPane();
    int index = -1;
    int kind = 0;
    nw4r::lyt::Pane* pane;
    pane = root->FindPaneByName(lbl_eu_80504268 + 0x169, true);
    if (reinterpret_cast<PaneVisAccess*>(pane)->visByte & 1) {
        index = 0; kind = 4;
    } else {
    pane = root->FindPaneByName(lbl_eu_80504268 + 0x174, true);
    if (reinterpret_cast<PaneVisAccess*>(pane)->visByte & 1) {
        index = 1; kind = 5;
    } else {
    pane = root->FindPaneByName(lbl_eu_80504268 + 0x17f, true);
    if (reinterpret_cast<PaneVisAccess*>(pane)->visByte & 1) {
        index = 2; kind = 6;
    } else {
    pane = root->FindPaneByName(lbl_eu_80504268 + 0x18a, true);
    if (reinterpret_cast<PaneVisAccess*>(pane)->visByte & 1) {
        index = 3; kind = 7;
    } else {
    pane = root->FindPaneByName(lbl_eu_80504268 + 0x195, true);
    if (reinterpret_cast<PaneVisAccess*>(pane)->visByte & 1) {
        index = 4; kind = 8;
    } else {
    pane = root->FindPaneByName(lbl_eu_80504268 + 0x1a2, true);
    if (reinterpret_cast<PaneVisAccess*>(pane)->visByte & 1) {
        index = 5; kind = 9;
    }
    }}}}}
    if (index < 0) {
        return;
    }
    u32 eff = (u32)createNpcActor__Q22cf13CfGameManagerFv(kind);
    lbl_eu_80575858[index] = eff;
    entry->field_0x28 = 1;
}