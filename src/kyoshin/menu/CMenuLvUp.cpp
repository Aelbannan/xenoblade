#define IWORK_EVENT_INLINE_DTOR
// CMenuLvUp - level-up menu task reconstruction.

// Shield the chain's C++-linkage Scn_FindCamItem decl (CfCam.hpp form):
// this TU's local extern "C" MenuLvUpPose* form (CMenuLvUp.hpp:212) would
// otherwise clash under MWCC 10197 (CfObjectMove.cpp idiom).

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"


#include "kyoshin/menu/CMenuLvUp.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Unknown per-entry object, deleted via virtual dispatch (vtable+8, flag 1).
// Only the virtual-dtor shape is required -- the real class lives in the
// retail module that owns it.
class CMenuLvUpEntryObj {
public:
    virtual ~CMenuLvUpEntryObj();
};

// Position components read from the actor->vfn12C(0x64) result (0x10-byte
// stride: +0x0c / +0x1c / +0x2c).
struct MenuLvUpObjPos;

// Opaque view of the findObjectById(id) actor object (the real class lives in
// the retail module that owns it). The two dispatched virtuals sit at +0xac
// (slot 43) and +0x12c (slot 75): declaring them as real virtuals makes MWCC
// emit the retail dispatch shape (`lwz r12,0(r3); lwz r12,slot(r12)`); a
// cast-and-call helper allocates a temp register instead. Never
// instantiated - only called through. field_0x64 is the actor's bit-1
// "action source" flag read during teardown.
class MenuLvUpActor {
public:
    virtual void vfn00() = 0;
    virtual void vfn01() = 0;
    virtual void vfn02() = 0;
    virtual void vfn03() = 0;
    virtual void vfn04() = 0;
    virtual void vfn05() = 0;
    virtual void vfn06() = 0;
    virtual void vfn07() = 0;
    virtual void vfn08() = 0;
    virtual void vfn09() = 0;
    virtual void vfn0A() = 0;
    virtual void vfn0B() = 0;
    virtual void vfn0C() = 0;
    virtual void vfn0D() = 0;
    virtual void vfn0E() = 0;
    virtual void vfn0F() = 0;
    virtual void vfn10() = 0;
    virtual void vfn11() = 0;
    virtual void vfn12() = 0;
    virtual void vfn13() = 0;
    virtual void vfn14() = 0;
    virtual void vfn15() = 0;
    virtual void vfn16() = 0;
    virtual void vfn17() = 0;
    virtual void vfn18() = 0;
    virtual void vfn19() = 0;
    virtual void vfn1A() = 0;
    virtual void vfn1B() = 0;
    virtual void vfn1C() = 0;
    virtual void vfn1D() = 0;
    virtual void vfn1E() = 0;
    virtual void vfn1F() = 0;
    virtual void vfn20() = 0;
    virtual void vfn21() = 0;
    virtual void vfn22() = 0;
    virtual void vfn23() = 0;
    virtual void vfn24() = 0;
    virtual void vfn25() = 0;
    virtual void vfn26() = 0;
    virtual void vfn27() = 0;
    virtual void vfn28() = 0;
    virtual nw4r::math::VEC3* vfnAC() = 0;          // slot 43 (+0xac)
    virtual void vfn44() = 0;
    virtual void vfn45() = 0;
    virtual void vfn46() = 0;
    virtual void vfn47() = 0;
    virtual void vfn48() = 0;
    virtual void vfn49() = 0;
    virtual void vfn4A() = 0;
    virtual void vfn4B() = 0;
    virtual void vfn4C() = 0;
    virtual void vfn4D() = 0;
    virtual void vfn4E() = 0;
    virtual void vfn4F() = 0;
    virtual void vfn50() = 0;
    virtual void vfn51() = 0;
    virtual void vfn52() = 0;
    virtual void vfn53() = 0;
    virtual void vfn54() = 0;
    virtual void vfn55() = 0;
    virtual void vfn56() = 0;
    virtual void vfn57() = 0;
    virtual void vfn58() = 0;
    virtual void vfn59() = 0;
    virtual void vfn5A() = 0;
    virtual void vfn5B() = 0;
    virtual void vfn5C() = 0;
    virtual void vfn5D() = 0;
    virtual void vfn5E() = 0;
    virtual void vfn5F() = 0;
    virtual void vfn60() = 0;
    virtual void vfn61() = 0;
    virtual void vfn62() = 0;
    virtual MenuLvUpObjPos* vfn12C(int index) = 0;  // slot 75 (+0x12c)
    u8 _04[0x64 - 0x04];                            // +0x04..0x63
    u32 field_0x64;                                 // +0x64
};

// Opaque view of the nw4r root pane's trailing flag byte at +0xbb (retail
// clears its low bit during per-entry teardown).
struct MenuLvUpPaneFlag {
    u8 _00[0xbb];
    u8 field_0xbb; // +0xbb
};

// Opaque view of the Scn_FindCamItem(scene, -1) camera/pose block (retail reads
// the two anchor VEC3s at +0x10c and +0x138).
struct MenuLvUpPose {
    u8 _00[0x10c];
    nw4r::math::VEC3 field_0x10c; // +0x10c
    u8 _118[0x138 - 0x118];
    nw4r::math::VEC3 field_0x138; // +0x138
};

// Position components read from the actor->vfn12C(0x64) result (0x10-byte
// stride: +0x0c / +0x1c / +0x2c).
struct MenuLvUpObjPos {
    u8 _00[0x0c];
    f32 field_0x0c; // +0x0c
    u8 _10[0x1c - 0x10];
    f32 field_0x1c; // +0x1c
    u8 _20[0x2c - 0x20];
    f32 field_0x2c; // +0x2c
};

// Retail-symbol imports. Data keeps its plain name; functions need C linkage
// so MWCC emits the unmangled retail names (same scheme as CMenuBattleEnd.hpp
// / CMenuCollepedia.hpp). teardownLvUpEntry / placeLvUpEntry / driveLvUpEntryState are
// the per-entry helpers driven by Move.
extern "C" {
void registerLvUpAction(CMenuLvUp* a, CMenuLvUp* b, int c);
void teardownLvUpEntry(CMenuLvUp* self, CMenuLvUpEntry* entry);
void placeLvUpEntry(CMenuLvUp* self, CMenuLvUpEntry* entry);
void driveLvUpEntryState(CMenuLvUp* self, CMenuLvUpEntry* entry);
void* Scn_FindCamItem(void* scene, int index);
void CamMan_ProjectToScreen(nw4r::math::VEC3* out, MenuLvUpPose* pose,
                   const nw4r::math::VEC3* in);
void CCharVoiceMan_PlayLevelUpVoice(int id);
void waitForDrawDone__9CDeviceVIFv();
void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);
void addRenderCB__4CScnFP10IScnRenderUlUl(CScn*, IScnRender*, u32, u32);
bool isSceneLoading__Q22cf13CfGameManagerFv();
}

// C++-linkage imports (retail symbols are the Itanium-mangled forms).
void* findObjectById(int id);

// C-linkage data imports (retail names; global-scope plain externs keep the
// unmangled symbol -- MWCC does not mangle global variables).
extern char lbl_eu_8050EC70[];   // layout/animation name string table
extern f32 lbl_eu_80668A00;      // anim frame reset value
extern f32 lbl_eu_80668A04;      // anim frame target for advanceAnimTransform
extern f32 lbl_eu_80668A08;      // screen-position post-scale offset x
extern f32 lbl_eu_80668A0C;      // screen-position post-scale offset y
extern f32 lbl_eu_80668A10;      // screen-position post-scale factor y
extern u32 lbl_eu_80663E28;      // task-state flag word (bit 21 gates the menu)


CMenuLvUp::~CMenuLvUp() {}

// ---------------------------------------------------------------------------
// Init - allocate the layout region and build the two layout+anim pairs for
// each of the three entries ("grow" animation first, "shrink" second). The
// region guard (Class_8045F858) wraps the whole setup and unregisters the
// region on scope exit.
// ---------------------------------------------------------------------------
void CMenuLvUp::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mMemRegion.createRegion((int)handle, 0x3e00, lbl_eu_8050EC70, 0);
    Class_8045F858 regionGuard(&mMemRegion);

    f32 resetFrame = lbl_eu_80668A00;
    for (u8 i = 0; i < 3; i++) {
        CMenuLvUpEntry* entry = &mEntries[i];

        nw4r::lyt::ArcResourceAccessor* accessor = CUICfManager_getArcResourceAccessor();
        buildLayout(&entry->field_0x00, accessor, &lbl_eu_8050EC70[0xa]);
        accessor = CUICfManager_getArcResourceAccessor();
        bindLayoutAnimTransform(entry->field_0x00, &entry->field_0x04, accessor,
                      &lbl_eu_8050EC70[0x21]);
        entry->field_0x00->SetAnimationEnable(entry->field_0x04, true);
        entry->field_0x04->SetFrame(resetFrame);
        entry->field_0x00->Animate(0);

        accessor = CUICfManager_getArcResourceAccessor();
        buildLayout(&entry->field_0x08, accessor, &lbl_eu_8050EC70[0x3b]);
        accessor = CUICfManager_getArcResourceAccessor();
        bindLayoutAnimTransform(entry->field_0x08, &entry->field_0x0C, accessor,
                      &lbl_eu_8050EC70[0x54]);
        entry->field_0x08->SetAnimationEnable(entry->field_0x0C, true);
        entry->field_0x0C->SetFrame(resetFrame);
        entry->field_0x08->Animate(0);
    }

    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, this, 7, 0);

    mMemRegion.validateHeap();
}

void CMenuLvUp::Term() {
    waitForDrawDone__9CDeviceVIFv();
    removeRenderCB__4CScnFP10IScnRender(mScene, this);
    // Release the per-entry objects (virtual deleting-dtor dispatch, flag 1)
    // and re-null the slots.
    for (u8 i = 0; i < 3; i++) {
        if (mEntries[i].field_0x00 != 0) {
            delete mEntries[i].field_0x00;
            mEntries[i].field_0x00 = 0;
        }
        if (mEntries[i].field_0x08 != 0) {
            delete mEntries[i].field_0x08;
            mEntries[i].field_0x08 = 0;
        }
    }
    mMemRegion.deleteRegion();
    CMenuLvUp::sInstance = 0;
}

// ---------------------------------------------------------------------------
// Move - per-frame driver. The guard chain (same shape as cbRenderBefore)
// gates on the task state; when the menu is active (mField11C) each entry's
// state machine runs: 1/2 advance their anim to the target frame then reset,
// 3 hands off to driveLvUpEntryState. The active flag is then re-derived from the
// three entry states.
// ---------------------------------------------------------------------------
void CMenuLvUp::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (!IsMenuState621F0()) return;
    if (lbl_eu_80663E24 & 0xbfe40000) return;
    if (isSceneLoading__Q22cf13CfGameManagerFv()) return;

    if (mField11C != 0) {
        f32 resetFrame = lbl_eu_80668A00;
        for (u8 i = 0; i < 3; i++) {
            CMenuLvUpEntry* entry = &mEntries[i];
            teardownLvUpEntry(this, entry);
            placeLvUpEntry(this, entry);

            switch (entry->field_0x14) {
            case 1:
                if (advanceAnimTransform(entry->field_0x04, lbl_eu_80668A04) != 0) {
                    entry->field_0x14 = 3;
                    entry->field_0x04->SetFrame(resetFrame);
                }
                break;
            case 2:
                if (advanceAnimTransform(entry->field_0x0C, lbl_eu_80668A04) != 0) {
                    entry->field_0x14 = 3;
                    entry->field_0x0C->SetFrame(resetFrame);
                }
                break;
            case 3:
                driveLvUpEntryState(this, entry);
                break;
            default:
                break;
            }

            entry->field_0x00->Animate(0);
            entry->field_0x08->Animate(0);
        }
        // Re-derive the active flag from the three entry states (retail sums
        // them with the neg/or/srwi != 0 idiom).
        mField11C = (mEntries[0].field_0x14 + mEntries[1].field_0x14 +
                     mEntries[2].field_0x14) != 0;
    }

    if (mField55 != 0) {
        mField54 = 1;
    }
}

// ---------------------------------------------------------------------------
// cbRenderBefore - draws the three entries' layouts through a stack DrawInfo.
// ---------------------------------------------------------------------------
void CMenuLvUp::cbRenderBefore() {
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (!IsMenuState621F0()) return;
    if (lbl_eu_80663E24 & 0xbfe40000) return;
    if (isSceneLoading__Q22cf13CfGameManagerFv()) return;
    if (mField11C == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    for (u8 i = 0; i < 3; i++) {
        drawLayout(mEntries[i].field_0x00, &drawInfo, 0, 1);
        drawLayout(mEntries[i].field_0x08, &drawInfo, 0, 1);
    }
}

// ---------------------------------------------------------------------------
// Constructor (retail: the single allocating function __ct__CMenuLvUp). The
// IUICf/CTTask base chain and the IWorkEvent/IScnRender sub-vtables are set
// up by the compiler; the scene pointer is stored at +0x70 and the three
// entry slots are zeroed. Defined inline here, used only by create().
// ---------------------------------------------------------------------------
inline CMenuLvUp::CMenuLvUp(CScn* scene) : mScene(scene) {
    // Zero every entry; the queued-state list is cleared with a pointer loop.
    CMenuLvUpEntry* p = &mEntries[0];
    do {
        p->field_0x00 = 0;
        p->field_0x04 = 0;
        p->field_0x08 = 0;
        p->field_0x0C = 0;
        p->field_0x10 = 0;
        p->field_0x14 = 0;
        p->field_0x15 = 0;
        p->field_0x16[0] = 0;
        // Residual: retail's unroll guard recomputes the start pointer
        // (begin <= end test); this spelling is the closest found so far.
        for (u8* q = &p->field_0x16[1]; q < &p->field_0x16[0x20]; q++) {
            *q = 0;
        }
        p++;
    } while (p < &mEntries[3]);
    mField11C = 0;
}

CMenuLvUp* CMenuLvUp::sInstance;

// Only one level-up menu exists at a time. A request made while an instance
// exists is dropped (returns nullptr).
CMenuLvUp* CMenuLvUp::create(CProcess* parent, CScn* scene) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuLvUp* menu = new (CWorkThreadSystem::getWorkMem()) CMenuLvUp(scene);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

extern "C" void fireLvUpCombo(CMenuLvUp* self, u32 flag) {
    if (CMenuLvUp::sInstance != 0) {
        registerLvUpAction(CMenuLvUp::sInstance, self, 1);
        registerLvUpAction(CMenuLvUp::sInstance, self, 2);
        if (flag != 0) {
            registerLvUpAction(CMenuLvUp::sInstance, self, 3);
        }
    }
}

extern "C" void fireLvUpMode2(CMenuLvUp* self) {
    if (CMenuLvUp::sInstance != 0) {
        registerLvUpAction(CMenuLvUp::sInstance, self, 2);
    }
}

extern "C" void fireLvUpMode4(CMenuLvUp* self) {
    if (CMenuLvUp::sInstance != 0) {
        registerLvUpAction(CMenuLvUp::sInstance, self, 4);
    }
}

// ---------------------------------------------------------------------------
// registerLvUpAction - register a menu action for an actor. When an entry already
// tracks the actor id its queued-state list (entry+0x16, 32 bytes) is
// compacted (non-zero states packed to the front) and the new state is
// appended; otherwise the first empty entry claims the id with state 3 and
// the new state as its first queue element. The menu-active flag is set.
// ---------------------------------------------------------------------------
void registerLvUpAction(CMenuLvUp* a, CMenuLvUp* b, int c) {
    for (u8 i = 0; i < 3; i++) {
        CMenuLvUpEntry* entry = &a->mEntries[i];
        if (entry->field_0x10 == (u32)b) {
            u8 list[0x20];
            list[0] = 0;
            u8* q = &list[1];
            u8* end = &list[0x20];
            if (q < end) {
                for (; q < end; q++) {
                    *q = 0;
                }
            }
            u8 count = 0;
            for (u8 j = 0; j < 0x20; j++) {
                if (entry->field_0x16[j] != 0) {
                    list[count] = entry->field_0x16[j];
                    count++;
                }
            }
            if (count < 0x20) {
                for (u8 i = 0; i < 0x20; i++) {
                    entry->field_0x16[i] = list[i];
                }
                entry->field_0x16[count] = (u8)c;
            }
            return;
        }
    }
    for (u8 i = 0; i < 3; i++) {
        if (a->mEntries[i].field_0x10 == 0) {
            a->mEntries[i].field_0x10 = (u32)b;
            a->mEntries[i].field_0x14 = 3;
            a->mEntries[i].field_0x16[0] = (u8)c;
            a->mField11C = 1;
            return;
        }
    }
}

// ---------------------------------------------------------------------------
// driveLvUpEntryState - drive the next queued state for an entry. Scans the 32-byte
// queued-state list (0 = empty) for the first non-zero entry; states 1-4
// prepare the matching layout pane set (visible flags) and play a UI sound,
// state 1 also triggers the actor's level-up voice. The processed state is
// recorded at +0x15 and its slot cleared. An exhausted list resets the entry
// (actor id and state cleared).
// ---------------------------------------------------------------------------
void driveLvUpEntryState(CMenuLvUp* self, CMenuLvUpEntry* entry) {
    for (u8 i = 0; i < 0x20; i++) {
        // Retail keeps the slot ADDRESS (entry+i) in a callee-saved register
        // across the state handling (the index itself only feeds i++).
        u8* slot = &entry->field_0x16[i];
        u8 v = *slot;
        switch (v) {
        case 0:
            continue;
        case 1:
            entry->field_0x14 = 1;
            playUISound(0x4a);
            CCharVoiceMan_PlayLevelUpVoice((int)entry->field_0x10);
            break;
        case 2:
            entry->field_0x14 = 2;
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x70], true)
                ->SetVisible(true);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x7a], true)
                ->SetVisible(true);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x84], true)
                ->SetVisible(true);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x8d], true)
                ->SetVisible(false);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x96], true)
                ->SetVisible(false);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x9f], true)
                ->SetVisible(false);
            playUISound(0x93);
            break;
        case 3:
            entry->field_0x14 = 2;
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x70], true)
                ->SetVisible(false);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x7a], true)
                ->SetVisible(false);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x84], true)
                ->SetVisible(false);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x8d], true)
                ->SetVisible(true);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x96], true)
                ->SetVisible(true);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x9f], true)
                ->SetVisible(true);
            playUISound(0x78);
            break;
        case 4:
            entry->field_0x14 = 2;
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x70], true)
                ->SetVisible(true);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x7a], true)
                ->SetVisible(true);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x84], true)
                ->SetVisible(true);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x8d], true)
                ->SetVisible(false);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x96], true)
                ->SetVisible(true);
            entry->field_0x08->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050EC70[0x9f], true)
                ->SetVisible(false);
            playUISound(0x92);
            break;
        }
        entry->field_0x15 = *slot;
        *slot = 0;
        goto done;
    }
    entry->field_0x10 = 0;
    entry->field_0x14 = 0;
done:
    return;
}

// Vtable slot dispatch helper (same style as CMenuEnemyState.cpp): call a
// virtual through the vtable without declaring the callee's own class.
template <typename Fn>
static inline Fn vslot(void* obj, u32 offset) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void***>(obj))[offset / 4]);
}

// ---------------------------------------------------------------------------
// placeLvUpEntry - position and cull the entry's layouts at the actor's screen
// location. Both root panes are shown, then the actor's world position
// (preferring the vfn12C(0x64) anchor, else the vfnAC position) is projected
// through the scene camera and post-scaled into the pane translate. A facing
// test on the camera anchors hides the panes when the actor is out of view.
// ---------------------------------------------------------------------------
void placeLvUpEntry(CMenuLvUp* self, CMenuLvUpEntry* entry) {
    if (entry->field_0x10 == 0) return;

    // MWCC allocates locals in reverse declaration order to ascending slots:
    // screen +0x44, world +0x38, a +0x2c, b +0x20, c +0x14, anchorPos +0x8.
    nw4r::math::VEC3 screen;
    nw4r::math::VEC3 world;
    nw4r::math::VEC3 a;
    nw4r::math::VEC3 b;
    nw4r::math::VEC3 c;
    nw4r::math::VEC3 anchorPos;

    nw4r::lyt::Layout* l1 = entry->field_0x00;
    f32 v = lbl_eu_80668A00;
    l1->GetRootPane()->SetVisible(true);
    entry->field_0x08->GetRootPane()->SetVisible(true);

    screen.x = v;
    screen.y = v;
    screen.z = v;

    MenuLvUpActor* actor = (MenuLvUpActor*)findObjectById((int)entry->field_0x10);
    if (actor != 0) {
        const nw4r::math::VEC3* src;
        MenuLvUpObjPos* anchor = actor->vfn12C(0x64);
        if (anchor != 0) {
            // Load all three strided components first (retail's z,y,x load
            // order), then store into anchorPos in x,y,z order.
            f32 az = anchor->field_0x2c;
            f32 ay = anchor->field_0x1c;
            f32 ax = anchor->field_0x0c;
            anchorPos.x = ax;
            anchorPos.y = ay;
            anchorPos.z = az;
            src = &anchorPos;
        } else {
            src = actor->vfnAC();
        }
        world.x = src->x;
        world.y = src->y;
        world.z = src->z;

        MenuLvUpPose* pose = (MenuLvUpPose*)Scn_FindCamItem(self->mScene, -1);
        CamMan_ProjectToScreen(&screen, pose, &world);
        screen.x -= lbl_eu_80668A08;
        screen.y -= lbl_eu_80668A0C;
        screen.y *= lbl_eu_80668A10;
        screen.z = lbl_eu_80668A00;

        a.x = pose->field_0x10c.x;
        a.y = pose->field_0x10c.y;
        a.z = pose->field_0x10c.z;
        b.x = pose->field_0x138.x;
        b.y = pose->field_0x138.y;
        b.z = pose->field_0x138.z;
        c = *actor->vfnAC();
        if (CheckViewAngle60(&a, &b, &c) == 0) {
            entry->field_0x00->GetRootPane()->SetVisible(false);
            entry->field_0x08->GetRootPane()->SetVisible(false);
        }
    }

    entry->field_0x00->GetRootPane()->SetTranslate(screen);
    entry->field_0x08->GetRootPane()->SetTranslate(screen);
}

// ---------------------------------------------------------------------------
// Per-entry teardown: if the entry's actor id resolves to a live action
// source (findObjectById) whose bit 1 is set, leave the entry alone; otherwise
// clear the id, drop the root panes' visible flag bit, and wipe the state
// bytes.
// ---------------------------------------------------------------------------
extern "C" void teardownLvUpEntry(CMenuLvUp* self, CMenuLvUpEntry* entry) {
    if (entry->field_0x10 != 0) {
        MenuLvUpActor* src = (MenuLvUpActor*)findObjectById((int)entry->field_0x10);
        if (src != 0 && (src->field_0x64 & 2)) {
            return;
        }

        entry->field_0x10 = 0;
        reinterpret_cast<MenuLvUpPaneFlag*>(entry->field_0x00->GetRootPane())
            ->field_0xbb &= 0xFE;
        reinterpret_cast<MenuLvUpPaneFlag*>(entry->field_0x08->GetRootPane())
            ->field_0xbb &= 0xFE;
        {
            // Retail fully unrolls this constant-trip byte wipe (0x15..0x35)
            // into 33 ascending stb; subscript loops keep the byte stores.
            entry->field_0x15 = 0;
            entry->field_0x16[0] = 0;
            for (int j = 1; j < 0x20; j++) {
                entry->field_0x16[j] = 0;
            }
        }
        entry->field_0x14 = 0;
    }
}
