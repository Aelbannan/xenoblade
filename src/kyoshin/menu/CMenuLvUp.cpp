// CMenuLvUp - level-up menu task reconstruction.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include "kyoshin/menu/CMenuLvUp.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// forward declarations for scaffold thunk references
void __dt__9CMenuLvUpFv(void*);
void cbRenderBefore__9CMenuLvUpFv(void*);

// ---------------------------------------------------------------------------
// Destructor (D1/D2 merged). The UnkClass_8045F564 storage at +0x60 and the
// CProcess region are destroyed by hand (the retail layout is manual, not
// C++ inheritance); the nested null-checks reproduce the retail D2 guard
// shape around the CProcess call.
// ---------------------------------------------------------------------------
CMenuLvUp::~CMenuLvUp() {
    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion)->~UnkClass_8045F564();
    if (this) {
        if (this) {
            __dt__8CProcessFv(reinterpret_cast<CProcess*>(this), 0);
        }
    }
}

// ---------------------------------------------------------------------------
// Init - allocate the layout region and build the two layout+anim pairs for
// each of the three entries ("grow" animation first, "shrink" second). The
// region guard (Class_8045F858) wraps the whole setup and unregisters the
// region on scope exit.
// ---------------------------------------------------------------------------
void CMenuLvUp::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    reinterpret_cast<UnkClass_8045F564*>(mMemRegion)
        ->createRegion((int)handle, 0x3e00, lbl_eu_8050EC70, 0);
    Class_8045F858 regionGuard(reinterpret_cast<UnkClass_8045F564*>(mMemRegion));

    f32 resetFrame = lbl_eu_80668A00;
    for (u8 i = 0; i < 3; i++) {
        CMenuLvUpEntry* entry = &mEntries[i];

        nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
        buildLayout(&entry->field_0x00, accessor, &lbl_eu_8050EC70[0xa]);
        accessor = func_801355F4();
        bindLayoutAnimTransform(entry->field_0x00, &entry->field_0x04, accessor,
                      &lbl_eu_8050EC70[0x21]);
        entry->field_0x00->SetAnimationEnable(entry->field_0x04, true);
        entry->field_0x04->SetFrame(resetFrame);
        entry->field_0x00->Animate(0);

        accessor = func_801355F4();
        buildLayout(&entry->field_0x08, accessor, &lbl_eu_8050EC70[0x3b]);
        accessor = func_801355F4();
        bindLayoutAnimTransform(entry->field_0x08, &entry->field_0x0C, accessor,
                      &lbl_eu_8050EC70[0x54]);
        entry->field_0x08->SetAnimationEnable(entry->field_0x0C, true);
        entry->field_0x0C->SetFrame(resetFrame);
        entry->field_0x08->Animate(0);
    }

    {
        IScnRender* cb = reinterpret_cast<IScnRender*>(this);
        if (this != 0) {
            cb = reinterpret_cast<IScnRender*>(&mIScnRenderVtbl);
        }
        addRenderCB__4CScnFP10IScnRenderUlUl(mScene, cb, 7, 0);
    }

    reinterpret_cast<UnkClass_8045F564*>(mMemRegion)->func_8045F810();
}

void CMenuLvUp::Term() {
    waitForDrawDone__9CDeviceVIFv();
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = reinterpret_cast<IScnRender*>(&mIScnRenderVtbl);
    }
    removeRenderCB__4CScnFP10IScnRender(mScene, cb);
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
    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion)->func_8045F778();
    lbl_eu_80664900 = 0;
}

// ---------------------------------------------------------------------------
// Move - per-frame driver. The guard chain (same shape as cbRenderBefore)
// gates on the task state; when the menu is active (mField11C) each entry's
// state machine runs: 1/2 advance their anim to the target frame then reset,
// 3 hands off to func_802764A0. The active flag is then re-derived from the
// three entry states.
// ---------------------------------------------------------------------------
void CMenuLvUp::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (!func_8013BE50()) return;
    if (lbl_eu_80663E24 & 0xbfe40000) return;
    if (isSceneLoading__Q22cf13CfGameManagerFv()) return;

    if (mField11C != 0) {
        f32 resetFrame = lbl_eu_80668A00;
        for (u8 i = 0; i < 3; i++) {
            CMenuLvUpEntry* entry = &mEntries[i];
            func_80276B14(this, entry);
            func_802768E0(this, entry);

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
                func_802764A0(this, entry);
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
// cbRenderBefore - draws the three entries' layouts through a raw-storage
// DrawInfo (built/destroyed via the C-ABI ct/dt calls so the scope-exit
// destructor is not virtual-dispatched; same scheme as CSystemWindow).
// ---------------------------------------------------------------------------
void CMenuLvUp::cbRenderBefore() {
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (!func_8013BE50()) return;
    if (lbl_eu_80663E24 & 0xbfe40000) return;
    if (isSceneLoading__Q22cf13CfGameManagerFv()) return;
    if (mField11C == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    for (u8 i = 0; i < 3; i++) {
        drawLayout(mEntries[i].field_0x00, (nw4r::lyt::DrawInfo*)&drawInfo[0], 0, 1);
        drawLayout(mEntries[i].field_0x08, (nw4r::lyt::DrawInfo*)&drawInfo[0], 0, 1);
    }
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
}

// ---------------------------------------------------------------------------
// Allocating factory (retail unmangled symbol __ct__CMenuLvUp). If an
// instance already exists the request is dropped (returns 0); otherwise the
// 0x120-byte object is allocated on the work heap, CProcess's ctor runs, the
// CProcess-style fields + dispatch vtables are filled by hand, and the new
// task is registered under `parent`. The scene pointer is stored at +0x70.
// ---------------------------------------------------------------------------
extern "C" CMenuLvUp* __ct__CMenuLvUp(CProcess* parent, CScn* scene) {
    CMenuLvUp* obj;
    if (lbl_eu_80664900 != 0) {
        return 0;
    }

    obj = (CMenuLvUp*)mtl::MemManager::allocate(
        0x120, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        __ct__8CProcessFv((CProcess*)obj);

        obj->mVtable10 = (u32)lbl_eu_8052BF70;
        obj->mPtMf3C[0] = __ptmf_null[0];
        obj->mPtMf3C[1] = __ptmf_null[1];
        obj->mPtMf3C[2] = __ptmf_null[2];
        obj->mPtMf48[0] = __ptmf_null[0];
        obj->mPtMf48[1] = __ptmf_null[1];
        obj->mPtMf48[2] = __ptmf_null[2];
        obj->mField54 = 0;
        obj->mField55 = 0;
        obj->mVtable10 = (u32)lbl_eu_805380C8;
        obj->mIWorkEventVtbl = (u32)lbl_eu_805380C8 + 0x24;
        obj->mIScnRenderVtbl = (u32)lbl_eu_805380C8 + 0xac;

        __ct__17UnkClass_8045F564Fv(
            reinterpret_cast<UnkClass_8045F564*>(&obj->mMemRegion[0]));
        obj->mScene = scene;

        // Zero every entry: head words/bytes explicitly, tail via memset
        // (retail emits the pointer-count mtctr/bdnz expansion, i.e. the
        // runtime-length form, for the 0x17..0x36 region).
        for (CMenuLvUpEntry* p = &obj->mEntries[0]; p < &obj->mEntries[3]; p++) {
            p->field_0x00 = 0;
            p->field_0x04 = 0;
            p->field_0x08 = 0;
            p->field_0x0C = 0;
            p->field_0x10 = 0;
            p->field_0x14 = 0;
            p->field_0x15 = 0;
            p->field_0x16[0] = 0;
            u8* dst = &p->field_0x16[1];
            u8* end = &p->field_0x16[0x20];
            memset(dst, 0, end - dst);
        }
        obj->mField11C = 0;
    }

    lbl_eu_80664900 = obj;
    Regist__8CProcessFP8CProcessb((CProcess*)obj, parent, 0);
    return lbl_eu_80664900;
}

void func_80276148(CMenuLvUp* self, u32 flag) {
    if (lbl_eu_80664900 != 0) {
        func_80276200(lbl_eu_80664900, self, 1);
        func_80276200(lbl_eu_80664900, self, 2);
        if (flag != 0) {
            func_80276200(lbl_eu_80664900, self, 3);
        }
    }
}

void func_802761C0(CMenuLvUp* self) {
    if (lbl_eu_80664900 != 0) {
        func_80276200(lbl_eu_80664900, self, 2);
    }
}

void func_802761E0(CMenuLvUp* self) {
    if (lbl_eu_80664900 != 0) {
        func_80276200(lbl_eu_80664900, self, 4);
    }
}

// ---------------------------------------------------------------------------
// func_80276200 - register a menu action for an actor. When an entry already
// tracks the actor id its queued-state list (entry+0x16, 32 bytes) is
// compacted (non-zero states packed to the front) and the new state is
// appended; otherwise the first empty entry claims the id with state 3 and
// the new state as its first queue element. The menu-active flag is set.
// ---------------------------------------------------------------------------
void func_80276200(CMenuLvUp* a, CMenuLvUp* b, int c) {
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
// func_802764A0 - drive the next queued state for an entry. Scans the 32-byte
// queued-state list (0 = empty) for the first non-zero entry; states 1-4
// prepare the matching layout pane set (visible flags) and play a UI sound,
// state 1 also triggers the actor's level-up voice. The processed state is
// recorded at +0x15 and its slot cleared. An exhausted list resets the entry
// (actor id and state cleared).
// ---------------------------------------------------------------------------
void func_802764A0(CMenuLvUp* self, CMenuLvUpEntry* entry) {
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
            func_802A2E68((int)entry->field_0x10);
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
// func_802768E0 - position and cull the entry's layouts at the actor's screen
// location. Both root panes are shown, then the actor's world position
// (preferring the vfn12C(0x64) anchor, else the vfnAC position) is projected
// through the scene camera and post-scaled into the pane translate. A facing
// test on the camera anchors hides the panes when the actor is out of view.
// ---------------------------------------------------------------------------
void func_802768E0(CMenuLvUp* self, CMenuLvUpEntry* entry) {
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

    MenuLvUpActor* actor = findObjectById((int)entry->field_0x10);
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

        MenuLvUpPose* pose = func_80496264(self->mScene, -1);
        func_8049B59C(&screen, pose, &world);
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
        if (func_8013A4B4(&a, &b, &c) == 0) {
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
extern "C" void func_80276B14(CMenuLvUp* self, CMenuLvUpEntry* entry) {
    if (entry->field_0x10 != 0) {
        MenuLvUpActor* src = findObjectById((int)entry->field_0x10);
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

void func_80276C18(void* self) { ((void(*)(void*))__dt__9CMenuLvUpFv)((char*)self - 0x58); }

void func_80276C20(void* self) { ((void(*)(void*))cbRenderBefore__9CMenuLvUpFv)((char*)self - 0x5c); }

void func_80276C28(void* self) { ((void(*)(void*))__dt__9CMenuLvUpFv)((char*)self - 0x5c); }
