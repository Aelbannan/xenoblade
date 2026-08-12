// kyoshin/menu/CMenuKeyAssign - key-assign (button mapping) menu (singleton).

#include "kyoshin/menu/CMenuKeyAssign.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include <revolution/gx/GXPixel.h>

#include <stdio.h>

// C-ABI view of the member symbols so the vtable adjustor thunks can address
// them by their retail names with r4 passed through untouched (the member
// definitions below emit those symbols).
extern "C" void __dt__14CMenuKeyAssignFv(void*, int);
extern "C" void cbRenderBefore__14CMenuKeyAssignFv(void*);

// Key-assign free helpers (defined in this TU). Retail emits these under the
// unmangled names, so the declarations/definitions keep C linkage.
extern "C" void func_80115BD8(CMenuKeyAssign* self);
extern "C" void func_80115DB0(CMenuKeyAssign* self, char* str, int idx);
extern "C" void func_801159DC(CMenuKeyAssign* self);

// Open the key-assign menu: state is the remap-column base index (also the
// mField_78 open state), count is the number of columns to show, mode selects
// the special 3-column layout. Retail emits these under the unmangled names.
extern "C" void func_80115060(CMenuKeyAssign* self, int state, int count, int mode);
extern "C" void func_801154D0(CMenuKeyAssign* self, int a, int b, int c, int d, int e, int mode);

// Target: us-8011492c - CMenuKeyAssign::~CMenuKeyAssign
// Complete-object destructor. The UnkClass_8045F564 scratch region at +0x64 is
// destroyed first (member dtor with r4=-1), then the CProcess base guarded by
// the nested double null-check (an MWCC D2-inlined-into-D1 artifact), then the
// conditional operator delete is auto-emitted from the dtor flags.
CMenuKeyAssign::~CMenuKeyAssign() {
    reinterpret_cast<UnkClass_8045F564*>(&mUnkClass[0])->~UnkClass_8045F564();
    if (this != 0) {
        if (this != 0) {
            // C-ABI base dtor call with flags=0 (retail shape).
            __dt__8CProcessFv(reinterpret_cast<CProcess*>(this), 0);
        }
    }
}

void CMenuKeyAssign::Init() {
    // Scratch MEM2 region for the layout file data (guarded by the RAII
    // Class_8045F858 host so the region is released on every exit path).
    // The region address is recomputed at each use (this+0x64) - a held
    // pointer local would claim an extra callee-saved register.
    reinterpret_cast<UnkClass_8045F564*>(&mUnkClass[0])->createRegion(
        (int)mtl::MemManager::getHandleMEM2(), 0x4200, lbl_eu_804FDEE8, 0);
    Class_8045F858 memHost(reinterpret_cast<UnkClass_8045F564*>(&mUnkClass[0]));

    mtl::MemManager::func_80434A4C(false);

    // Build the layout from the arc resource.
    func_80136E84(&mLayout, func_801355F4(), &lbl_eu_804FDEE8[0xf]);

    // Bind the font: push the font object's pane back onto the root pane.
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    void* fontObj = CDeviceFont::func_80452C10(1, mLayout);
    u32 fontResult =
        reinterpret_cast<CMenuKeyAssignFont*>(fontObj)->getFontHandle();
    func_8013676C(rootPane, fontResult);

    // Hide the button panes that are not yet active (shared helper).
    func_80115BD8(this);

    // MI adjust: IScnRender subobject at +0x5c (null-this safe).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    mScn->addRenderCB(cb, 0xa, 0);

    reinterpret_cast<UnkClass_8045F564*>(&mUnkClass[0])->func_8045F810();
}

// Target: us-80114a84 - CMenuKeyAssign::Term
// Wait for VI draw completion, detach the render callback (IScnRender
// subobject at +0x5C, null-this safe), destroy the layout, release the scratch
// region and clear the singleton.
void CMenuKeyAssign::Term() {
    CDeviceVI::waitForDrawDone();

    // MI adjust: IScnRender subobject at +0x5c (null-this safe).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    mScn->removeRenderCB(cb);

    if (mLayout != NULL) {
        delete mLayout;
        mLayout = NULL;
    }

    reinterpret_cast<UnkClass_8045F564*>(&mUnkClass[0])->func_8045F778();

    lbl_eu_80663FA8 = NULL;
}

// Target: us-80114b04 - CMenuKeyAssign::Move
// Per-frame input handling: gate prefix (task busy / global bits / scene
// active / presentation flags / menu state), then dispatch on battle vs
// non-battle state, pad bits and the arts-select singleton to open the key-
// assign menu (func_80115060 / func_801154D0 / func_801159DC). The retail
// vf128 arts-gauge call is intentionally OUTSIDE the artsState null guard
// (retail dereferences the possibly-null pointer - mirrored exactly).
void CMenuKeyAssign::Move() {
    CMenuKeyAssign* self = this;
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() != 0) {
        return;
    }
    if (lbl_eu_80663E28 & (1u << 21)) {
        return;
    }
    if (func_8013BE50() == 0) {
        return;
    }
    if (func_8013BEE8() == 0) {
        return;
    }
    if (lbl_eu_80663E24 & 0xafa40000u) {
        return;
    }
    if (cf::CfGameManager::func_800829B8() != 0) {
        return;
    }

    CMenuKeyAssignPlayer* player =
        (CMenuKeyAssignPlayer*)cf::CfGameManager::getPlayer(0);
    CMenuKeyAssignActor* arts = (CMenuKeyAssignActor*)func_8016FE34(player);
    CPad* pad = cf::CfGameManager::getCurrentPad();

    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        // ---- in battle ----
        int battleMode = 0;
        void* bm = getInstance__Q22cf14CBattleManagerFv();
        s16* sub20c8 = (s16*)((u8*)bm + 0x20c8);
        if (sub20c8 != 0 && *sub20c8 != 0) {
            battleMode = 1;
        }
        u8* sub1a8 = (u8*)bm + 0x1a8;
        if (sub1a8 != 0) {
            u8 phase = *(u8*)(sub1a8 + 2);
            bool inRange = phase >= 1 && phase <= 0x18;
            if (inRange) {
                if (func_801042C8() != 0) {
                    func_80115060(self, 0x19, 1, 0);
                    goto done;
                }
                func_80115060(self, 0x18, 1, 0);
                goto done;
            }
        }
        if ((pad->mHeldButtonFlags & 0x10) != 0) {
            int b = func_801B0F8C();
            if (b == 0 && func_8017FD44((void*)b) == 0) {
                func_801154D0(self, 9, 10, 13, 12, 0, 1);
                goto done;
            }
        }
        if (func_800FF738() != 0) {
            func_80115060(self, 0x16, 2, 0);
            goto done;
        }
        if (CMenuArtsSelect_isCreated() != 0) {
            int handled = 0;
            if (arts != 0) {
                u32 id = *arts->mSub04->getActorId();
                if (func_80174C98(arts, &id, 0x803) != 0) {
                    if ((pad->mHeldButtonFlags & 8) != 0) {
                        int b2 = func_801B0F8C();
                        if (battleMode == 0 && b2 == 0 &&
                            func_8017FD44((void*)b2) == 0) {
                            CBattleManagerView* bmv = (CBattleManagerView*)bm;
                            u32 cnt = 0;
                            CListLink* node =
                                ((CListLink*)bmv->mList28)->next;
                            while (node != (CListLink*)bmv->mList28) {
                                node = node->next;
                                cnt++;
                            }
                            if (cnt > 1) {
                                func_801159DC(self);
                            } else {
                                func_80115060(self, 3, 1, 0);
                            }
                            handled = 1;
                            goto artsDone;
                        }
                    }
                    switch (player->mSub38->mState36C) {
                    case 1:
                        func_80115060(self, player->mSub38->mCount370 + 0x1b, 0, 0);
                        handled = 1;
                        break;
                    case 2:
                        func_80115060(self, player->mSub38->mCount370 + 0x1c, 0, 0);
                        handled = 1;
                        break;
                    case 3:
                        func_80115060(self, player->mSub38->mCount370 + 0x1d, 0, 0);
                        handled = 1;
                        break;
                    case 4:
                        if (player->mSub38->mCount370 == 1) {
                            func_80115060(self, 0x1e, 0, 0);
                        } else {
                            func_80115060(self, 0x1f, 0, 0);
                        }
                        handled = 1;
                        break;
                    case 7:
                        func_80115060(self, player->mSub38->mCount370 + 0x25, 0, 0);
                        handled = 1;
                        break;
                    case 8:
                        func_80115060(self, player->mSub38->mCount370 + 0x26, 0, 0);
                        handled = 1;
                        break;
                    default:
                        func_80115060(self, 0x1a, 1, 0);
                        break;
                    }
                    goto artsDone;
                }
            }
            // arts == 0 or func_80174C98 == 0: retail dereferences arts here.
            f32 gauge = ((f32 (*)(void*))((void**)arts)[0x128 / 4])(arts);
            if (gauge <= lbl_eu_80667078) {
                func_801154D0(self, 0x18, 1, 9, 0, 0, 1);
            } else {
                func_801154D0(self, 1, 2, 9, 0, 0, 1);
            }
            handled = 1;
        artsDone:
            if (handled == 0) {
                if (func_801042C8() != 0) {
                    func_80115060(self, 0x19, 1, 0);
                    goto done;
                }
                if (battleMode != 0) {
                    func_801154D0(self, 0x18, 9, 0, 0, 0, 1);
                    goto done;
                }
                func_801154D0(self, 0x18, 1, 9, 0, 0, 1);
                goto done;
            }
        }
        // Arts select not created: target/tech selection chain.
        if (player != 0) {
            void* battle = player->mSub38;
            if (((int (*)(void*, u32))((void**)battle)[0x40 / 4])(battle, 0x40000) != 0) {
                CBattleManagerView* bmv = (CBattleManagerView*)bm;
                u32 cnt = 0;
                CListLink* node = ((CListLink*)bmv->mList08)->next;
                while (node != (CListLink*)bmv->mList08) {
                    node = node->next;
                    cnt++;
                }
                if (cnt == 0) {
                    int id = ((int (*)(void*))((void**)player)[0x4C / 4])(player);
                    if (id != 0) {
                        void* target = func_800B708C(id);
                        if (target != 0) {
                            u32 flags = ((CActorTargetView*)target)->mFlags64;
                            if (flags & 0x10000000) {
                                void* bf = func_800BF324(target);
                                u32 st =
                                    ((u32 (*)(void*))((void**)bf)[0x228 / 4])(bf);
                                switch (st) {
                                case 3:
                                    func_80115060(self, 0x10, 1, 0);
                                    goto done;
                                case 2:
                                    func_80115060(self, 0x11, 2, 0);
                                    goto done;
                                case 9:
                                    func_801154D0(self, 0xe, 0, 0, 0, 0, 0);
                                    goto done;
                                default:
                                    func_80115060(self, 0xf, 1, 0);
                                    goto done;
                                }
                            }
                            if ((flags & 0x4000) || (flags & 0x8000)) {
                                switch (((CActorTargetView*)target)->mType91) {
                                case 6:
                                    func_80115060(self, 0x15, 1, 0);
                                    goto done;
                                case 0xc:
                                    func_80115060(self, 0x14, 1, 0);
                                    goto done;
                                case 9:
                                    func_801154D0(self, 0xe, 0, 0, 0, 0, 0);
                                    goto done;
                                case 0xa:
                                    func_801154D0(self, 0xe, 0, 0, 0, 0, 0);
                                    goto done;
                                case 0xb:
                                    func_801154D0(self, 0xe, 0, 0, 0, 0, 0);
                                    goto done;
                                default:
                                    func_80115060(self, 0x13, 1, 0);
                                    goto done;
                                }
                            }
                            func_801154D0(self, 0xe, 0, 0, 0, 0, 0);
                            goto done;
                        }
                        func_80115060(self, 0xe, 1, 0);
                        goto done;
                    }
                    func_80115060(self, 0xe, 1, 0);
                    goto done;
                }
                func_80115060(self, 0xe, 1, 0);
                goto done;
            }
        }
        // player == 0 / vf40 == 0: retry the target chain (double vf4C).
        if (player != 0) {
            int id = ((int (*)(void*))((void**)player)[0x4C / 4])(player);
            if (id != 0) {
                id = ((int (*)(void*))((void**)player)[0x4C / 4])(player);
                if (id != 0) {
                    if (func_800B708C(id) != 0) {
                        func_801154D0(self, 1, 2, 9, 0, 0, 1);
                        goto done;
                    }
                }
            }
        }
        func_80115060(self, 0xe, 1, 0);
        goto done;
    }

    // ---- not in battle ----
    int battleMode = 0;
    void* bm = getInstance__Q22cf14CBattleManagerFv();
    s16* sub20c8 = (s16*)((u8*)bm + 0x20c8);
    if (sub20c8 != 0 && *sub20c8 != 0) {
        battleMode = 1;
    }
    u8* sub1a8 = (u8*)bm + 0x1a8;
    if (sub1a8 != 0) {
        u8 phase = *(u8*)(sub1a8 + 2);
        bool inRange = phase >= 1 && phase <= 0x18;
        if (inRange) {
            if (func_801042C8() != 0) {
                func_80115060(self, 0x19, 1, 0);
                goto done;
            }
            func_80115060(self, 0x18, 1, 0);
            goto done;
        }
    }
    if ((pad->mHeldButtonFlags & 0x800) != 0) {
        int b = func_801B0F8C();
        if (b == 0 && func_8017FD44((void*)b) == 0) {
            func_801154D0(self, 9, 10, 13, 12, 0, 1);
            goto done;
        }
    }
    if (func_800FF738() != 0) {
        func_80115060(self, 0x16, 2, 0);
        goto done;
    }
    if ((pad->mHeldButtonFlags & 0x1000) != 0) {
        if (battleMode == 0) {
            int b = func_801B0F8C();
            if (b == 0 && func_8017FD44((void*)b) == 0) {
                if (arts != 0) {
                    u32 id = *arts->mSub04->getActorId();
                    if (func_80174C98(arts, &id, 0x803) != 0) {
                        CBattleManagerView* bmv = (CBattleManagerView*)bm;
                        u32 cnt = 0;
                        CListLink* node =
                            ((CListLink*)bmv->mList28)->next;
                        while (node != (CListLink*)bmv->mList28) {
                            node = node->next;
                            cnt++;
                        }
                        if (cnt > 1) {
                            func_801159DC(self);
                            goto done;
                        }
                        func_80115060(self, 3, 1, 0);
                        goto done;
                    }
                }
                if (player != 0) {
                    int pid = ((int (*)(void*))((void**)player)[0x4C / 4])(player);
                    if (pid != 0) {
                        f32 gauge =
                            ((f32 (*)(void*))((void**)arts)[0x128 / 4])(arts);
                        if (gauge <= lbl_eu_80667078) {
                            func_80115060(self, 1, 1, 0);
                            goto done;
                        }
                        func_80115060(self, 1, 2, 0);
                        goto done;
                    }
                }
                func_80115060(self, 0, 0, 0);
                goto done;
            }
        }
    }
    if (CMenuArtsSelect_isCreated() != 0) {
        int handled = 0;
        if (arts != 0) {
            u32 id = *arts->mSub04->getActorId();
            if (func_80174C98(arts, &id, 0x803) != 0) {
                switch (player->mSub38->mState36C) {
                case 1:
                    func_80115060(self, player->mSub38->mCount370 + 0x1b, 0, 0);
                    handled = 1;
                    break;
                case 2:
                    func_80115060(self, player->mSub38->mCount370 + 0x1c, 0, 0);
                    handled = 1;
                    break;
                case 3:
                    func_80115060(self, player->mSub38->mCount370 + 0x1d, 0, 0);
                    handled = 1;
                    break;
                case 4:
                    if (player->mSub38->mCount370 == 1) {
                        func_80115060(self, 0x1e, 0, 0);
                    } else {
                        func_80115060(self, 0x1f, 0, 0);
                    }
                    handled = 1;
                    break;
                case 7:
                    func_80115060(self, player->mSub38->mCount370 + 0x25, 0, 0);
                    handled = 1;
                    break;
                case 8:
                    func_80115060(self, player->mSub38->mCount370 + 0x26, 0, 0);
                    handled = 1;
                    break;
                default:
                    func_80115060(self, 0x1a, 1, 0);
                    break;
                }
            }
        }
        if (handled == 0) {
            if (func_801042C8() != 0) {
                func_80115060(self, 0x19, 1, 0);
                goto done;
            }
            func_80115060(self, 0x18, 1, 0);
            goto done;
        }
    }
    // Arts select not created (field): same target/tech chain as in battle.
    if (player != 0) {
        void* battle = player->mSub38;
        if (((int (*)(void*, u32))((void**)battle)[0x40 / 4])(battle, 0x40000) != 0) {
            CBattleManagerView* bmv = (CBattleManagerView*)bm;
            u32 cnt = 0;
            CListLink* node = ((CListLink*)bmv->mList08)->next;
            while (node != (CListLink*)bmv->mList08) {
                node = node->next;
                cnt++;
            }
            if (cnt == 0) {
                int id = ((int (*)(void*))((void**)player)[0x4C / 4])(player);
                if (id != 0) {
                    void* target = func_800B708C(id);
                    if (target != 0) {
                        u32 flags = ((CActorTargetView*)target)->mFlags64;
                        if (flags & 0x10000000) {
                            void* bf = func_800BF324(target);
                            u32 st =
                                ((u32 (*)(void*))((void**)bf)[0x228 / 4])(bf);
                            switch (st) {
                            case 3:
                                func_80115060(self, 0x10, 1, 0);
                                goto done;
                            case 2:
                                func_80115060(self, 0x11, 2, 0);
                                goto done;
                            case 9:
                                func_801154D0(self, 0xe, 0, 0, 0, 0, 0);
                                goto done;
                            default:
                                func_80115060(self, 0xf, 1, 0);
                                goto done;
                            }
                        }
                        if ((flags & 0x4000) || (flags & 0x8000)) {
                            switch (((CActorTargetView*)target)->mType91) {
                            case 6:
                                func_80115060(self, 0x15, 1, 0);
                                goto done;
                            case 0xc:
                                func_80115060(self, 0x14, 1, 0);
                                goto done;
                            case 9:
                                func_801154D0(self, 0xe, 0, 0, 0, 0, 0);
                                goto done;
                            case 0xa:
                                func_801154D0(self, 0xe, 0, 0, 0, 0, 0);
                                goto done;
                            case 0xb:
                                func_801154D0(self, 0xe, 0, 0, 0, 0, 0);
                                goto done;
                            default:
                                func_80115060(self, 0x13, 1, 0);
                                goto done;
                            }
                        }
                        func_801154D0(self, 0xe, 0, 0, 0, 0, 0);
                        goto done;
                    }
                    func_80115060(self, 0xe, 1, 0);
                    goto done;
                }
                func_80115060(self, 0xe, 1, 0);
                goto done;
            }
            func_80115060(self, 0xe, 1, 0);
            goto done;
        }
    }
    func_80115060(self, 0xe, 1, 0);
done:
    ((void (*)(void*, u32))((void**)self->mLayout)[0x38 / 4])(self->mLayout, 0);
}

void Draw__14CMenuKeyAssignFv(void) {}

// Target: us-80115964 - CMenuKeyAssign::cbRenderBefore
// Gate prefix (task busy / global bits / scene-active / presentation flags)
// then draw the embedded layout through a stack DrawInfo.
void CMenuKeyAssign::cbRenderBefore() {
    CTaskGame::getInstance();
    // Combined short-circuit produces retail's double branch for the bit-21
    // gate (bne return; beq continue; b return) - same shape as CMenuGCItem::Move.
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & (1u << 21))) {
        return;
    }
    if (!func_8013BE50()) {
        return;
    }
    if (getUnk80664658()->field_214 & (1u << 20)) {
        return;
    }
    if (!func_8013BEE8()) {
        return;
    }
    if (lbl_eu_80663E24 & 0xafa40000u) {
        return;
    }
    if (cf::CfGameManager::func_800829B8()) {
        return;
    }

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo (CMenuGCItem idiom): a C++ local would also emit a
    // scope-exit (virtual) destructor on top of the explicit call.
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137038(mLayout, (nw4r::lyt::DrawInfo*)&drawInfo[0], 0, 1);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0], -1);
}

// Factory constructor (retail unmangled symbol __ct__CMenuKeyAssign). When the
// singleton already exists it returns 0; otherwise it allocates + constructs
// the 0x7c-byte object, registers it under the parent process and returns the
// singleton. `parent` (r3) feeds CProcess::Regist, `scene` (r4) is stored at
// +0x60 as the render-callback owning scene.
extern "C" CMenuKeyAssign* __ct__CMenuKeyAssign(CProcess* parent, CScn* scene) {
    if (lbl_eu_80663FA8 != 0) {
        return 0;
    }
    CMenuKeyAssign* obj = (CMenuKeyAssign*)mtl::MemManager::allocate(
        0x7c, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        __ct__8CProcessFv((CProcess*)obj);

        // vtable fixups: temp (CProcess) vtable first, then the composite
        // vtable and the IWorkEvent (+0x24) / IScnRender (+0xac) sub-vtables.
        // The null-PMF triple is copied through one base pointer (retail
        // load order [1],[0],[2] per group from a single base register).
        obj->mProcessVt = (u32)lbl_eu_8052BF70;
        const u32* src = __ptmf_null;
        u32 w1 = src[1];
        u32 w0 = src[0];
        obj->ptmfMove[0] = w0;
        obj->ptmfMove[1] = w1;
        u32 w2 = src[2];
        obj->ptmfMove[2] = w2;
        w1 = src[1];
        w0 = src[0];
        obj->ptmfDraw[0] = w0;
        obj->ptmfDraw[1] = w1;
        w2 = src[2];
        obj->ptmfDraw[2] = w2;
        obj->mField_54 = 0;
        obj->mField_55 = 0;

        obj->mProcessVt = (u32)lbl_eu_8052C640;
        obj->mIWorkEventVt = (u32)lbl_eu_8052C640 + 0x24;
        obj->mIScnRenderVt = (u32)lbl_eu_8052C640 + 0xac;
        obj->mScn = scene;

        __ct__17UnkClass_8045F564Fv(&obj->mUnkClass[0]);

        obj->mLayout = 0;
        obj->mField_78 = 0;
    }
    lbl_eu_80663FA8 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);
    return lbl_eu_80663FA8;
}

// Open the key-assign menu for `count` remap columns starting at `state`
// (target us-80115b3c). Hides the panes, loads the remap-column names via
// func_8013606C/func_80136190, finds the "timg" texture and sizes the pane
// from the texture dimensions (u16 -> f32 via the 0x43300000 magic double),
// then assigns the button labels through func_80115DB0. The F64Conv pairs are
// function-scope (their 0x43300000 words are stored once, before the guards).
extern "C" void func_80115060(CMenuKeyAssign* self, int state, int count, int mode) {
    // F64Conv pairs are function-scope (their 0x43300000 words are stored once,
    // before the guards).
    F64Conv ch, cw;
    f32 sx, sy;
    f32 sx2, sy2;
    f32 sx3, sy3;

    cw.w[0] = 0x43300000;
    ch.w[0] = 0x43300000;

    if ((s32)self->mField_78 == state) {
        return;
    }
    self->mField_78 = (u32)state;
    func_80115BD8(self);
    if (state == 0) {
        return;
    }

    const char* base = lbl_eu_804FDEE8;
    const char* name = &base[0x29];
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        name = &base[0x32];
    }

    if (count > 0) {
        // The 0x43300000 magic double is loaded once into a loop-scoped local
        // (retail keeps it in f31 across the loop calls).
        f64 magic = lbl_eu_80667080;
        int i;
        for (i = 0; i < count; i++) {
            int idx = state + i;
            u32 timg = 0x74696d67;
            u16 r = func_8013606C(&base[0x3b], name, idx);
            ml::FixStr<32> str(true);
            f32 sx, sy;
            str.format(&base[0x5f], func_80136190(&base[0x49], &base[0x56], r));
            nw4r::lyt::ArcResourceAccessor* acc = func_801355F4();
            TexView* tex = (TexView*)acc->GetResource(timg, str.mString, NULL);
            if (tex != 0) {
                int flag = 0;
                if (count == 3 && mode != 0 && i == 2) {
                    flag = 1;
                }
                str.format(&base[0x66], (flag ? 4 : i) + 1);
                nw4r::lyt::Pane* pane =
                    self->mLayout->GetRootPane()->FindPaneByName(str.mString, true);
                func_80137F88(pane, tex);
                u16 w = tex->mHeader->mDims[1];
                u16 h = tex->mHeader->mDims[0];
                cw.w[1] = (u32)w;
                ch.w[1] = (u32)h;
                sx = (f32)(cw.d - magic);
                sy = (f32)(ch.d - magic);
                PaneSizeFlagView* pv = (PaneSizeFlagView*)pane;
                pv->sizeX = sx;
                pv->sizeY = sy;
                pv->flags = (pv->flags & 0xFE) | 1;
            }
            if (i < 4) {
                char* s2 = func_80136190(&base[0x3b], &base[0x72], idx);
                func_80115DB0(self, s2, i);
            }
        }
    } else {
        // count <= 0: two fixed panes (name index 26, then `state`).
        {
            u16 r = func_8013606C(&base[0x3b], name, 26);
            ml::FixStr<32> str2(true);
            f32 sx2, sy2;
            str2.format(&base[0x5f], func_80136190(&base[0x49], &base[0x56], r));
            nw4r::lyt::ArcResourceAccessor* acc = func_801355F4();
            TexView* tex = (TexView*)acc->GetResource(0x74696d67, str2.mString, NULL);
            if (tex != 0) {
                str2.format(&base[0x66], 1);
                nw4r::lyt::Pane* pane =
                    self->mLayout->GetRootPane()->FindPaneByName(str2.mString, true);
                func_80137F88(pane, tex);
                u16 w = tex->mHeader->mDims[1];
                u16 h = tex->mHeader->mDims[0];
                cw.w[1] = (u32)w;
                ch.w[1] = (u32)h;
                sx2 = (f32)(cw.d - lbl_eu_80667080);
                sy2 = (f32)(ch.d - lbl_eu_80667080);
                PaneSizeFlagView* pv = (PaneSizeFlagView*)pane;
                pv->sizeX = sx2;
                pv->sizeY = sy2;
                pv->flags = (pv->flags & 0xFE) | 1;
            }
            char* s2 = func_80136190(&base[0x3b], &base[0x72], 26);
            func_80115DB0(self, s2, 0);
        }
        {
            u16 r = func_8013606C(&base[0x3b], name, state);
            ml::FixStr<32> str3(true);
            f32 sx3, sy3;
            str3.format(&base[0x5f], func_80136190(&base[0x49], &base[0x56], r));
            nw4r::lyt::ArcResourceAccessor* acc = func_801355F4();
            TexView* tex = (TexView*)acc->GetResource(0x74696d67, str3.mString, NULL);
            if (tex != 0) {
                str3.format(&base[0x66], 2);
                nw4r::lyt::Pane* pane =
                    self->mLayout->GetRootPane()->FindPaneByName(str3.mString, true);
                func_80137F88(pane, tex);
                u16 w = tex->mHeader->mDims[1];
                u16 h = tex->mHeader->mDims[0];
                cw.w[1] = (u32)w;
                ch.w[1] = (u32)h;
                sx3 = (f32)(cw.d - lbl_eu_80667080);
                sy3 = (f32)(ch.d - lbl_eu_80667080);
                PaneSizeFlagView* pv = (PaneSizeFlagView*)pane;
                pv->sizeX = sx3;
                pv->sizeY = sy3;
                pv->flags = (pv->flags & 0xFE) | 1;
            }
            char* s2 = func_80136190(&base[0x3b], &base[0x72], state);
            func_80115DB0(self, s2, 1);
        }
    }
}

// Open the key-assign menu for up to 5 remap buttons (target us-80115fac). The
// five indices a..e are the button ids; `mode` (6th arg) selects the special
// 3-column layout. mField_78 stores (a+b+c+d+e)*100 as the open state. `count`
// is the index of the first zero id (5 when all non-zero); the ids are kept in
// a stack array because the loop indexes them by pointer.
extern "C" void func_801154D0(CMenuKeyAssign* self, int a, int b, int c, int d, int e, int mode) {
    F64Conv cw, ch;
    cw.w[0] = 0x43300000;
    ch.w[0] = 0x43300000;

    int state = (e + (d + b)) + (c + a);
    state *= 100;

    if (self->mField_78 == (u32)state) {
        return;
    }
    self->mField_78 = (u32)state;
    func_80115BD8(self);
    if (state == 0) {
        return;
    }

    const char* base = lbl_eu_804FDEE8;
    const char* name = &base[0x29];
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        name = &base[0x32];
    }

    int count = 0;
    int args[5] = {a, b, c, d, e};
    if (a == 0) {
        count = 0;
    } else if (b == 0) {
        count = 1;
    } else if (c == 0) {
        count = 2;
    } else if (d == 0) {
        count = 3;
    } else if (e == 0) {
        count = 4;
    } else if (count == 0) {
        count = 5;
    }

    if (count > 0) {
        // The 0x43300000 magic double is loaded once into a loop-scoped local
        // (retail keeps it in f31 across the loop calls).
        f64 magic = lbl_eu_80667080;
        int i;
        for (i = 0; i < count; i++) {
            int idx = args[i];
            u16 r = func_8013606C(&base[0x3b], name, idx);
            ml::FixStr<32> str(true);
            str.format(&base[0x5f], func_80136190(&base[0x49], &base[0x56], r));
            nw4r::lyt::ArcResourceAccessor* acc = func_801355F4();
            TexView* tex = (TexView*)acc->GetResource(0x74696d67, str.mString, NULL);
            if (tex != 0) {
                int flag = 0;
                if (count >= 3 && mode != 0 && i == 2) {
                    flag = 1;
                }
                str.format(&base[0x66], (flag ? 4 : i) + 1);
                nw4r::lyt::Pane* pane =
                    self->mLayout->GetRootPane()->FindPaneByName(str.mString, true);
                func_80137F88(pane, tex);
                u16 w = tex->mHeader->mDims[1];
                u16 h = tex->mHeader->mDims[0];
                cw.w[1] = (u32)w;
                ch.w[1] = (u32)h;
                f32 sx = (f32)(cw.d - magic);
                f32 sy = (f32)(ch.d - magic);
                PaneSizeFlagView* pv = (PaneSizeFlagView*)pane;
                pv->sizeX = sx;
                pv->sizeY = sy;
                pv->flags = (pv->flags & 0xFE) | 1;
            }
            if (i < 4) {
                char* s2 = func_80136190(&base[0x3b], &base[0x72], idx);
                func_80115DB0(self, s2, i);
            }
        }
    } else {
        // count == 0: two fixed panes (name index 26, then `state`).
        {
            u16 r = func_8013606C(&base[0x3b], name, 26);
            ml::FixStr<32> str2(true);
            str2.format(&base[0x5f], func_80136190(&base[0x49], &base[0x56], r));
            nw4r::lyt::ArcResourceAccessor* acc = func_801355F4();
            TexView* tex = (TexView*)acc->GetResource(0x74696d67, str2.mString, NULL);
            if (tex != 0) {
                str2.format(&base[0x66], 1);
                nw4r::lyt::Pane* pane =
                    self->mLayout->GetRootPane()->FindPaneByName(str2.mString, true);
                func_80137F88(pane, tex);
                u16 w = tex->mHeader->mDims[1];
                u16 h = tex->mHeader->mDims[0];
                cw.w[1] = (u32)w;
                ch.w[1] = (u32)h;
                f32 sx = (f32)(cw.d - lbl_eu_80667080);
                f32 sy = (f32)(ch.d - lbl_eu_80667080);
                PaneSizeFlagView* pv = (PaneSizeFlagView*)pane;
                pv->sizeX = sx;
                pv->sizeY = sy;
                pv->flags = (pv->flags & 0xFE) | 1;
            }
            char* s2 = func_80136190(&base[0x3b], &base[0x72], 26);
            func_80115DB0(self, s2, 0);
        }
        {
            u16 r = func_8013606C(&base[0x3b], name, state);
            ml::FixStr<32> str3(true);
            str3.format(&base[0x5f], func_80136190(&base[0x49], &base[0x56], r));
            nw4r::lyt::ArcResourceAccessor* acc = func_801355F4();
            TexView* tex = (TexView*)acc->GetResource(0x74696d67, str3.mString, NULL);
            if (tex != 0) {
                str3.format(&base[0x66], 2);
                nw4r::lyt::Pane* pane =
                    self->mLayout->GetRootPane()->FindPaneByName(str3.mString, true);
                func_80137F88(pane, tex);
                u16 w = tex->mHeader->mDims[1];
                u16 h = tex->mHeader->mDims[0];
                cw.w[1] = (u32)w;
                ch.w[1] = (u32)h;
                f32 sx = (f32)(cw.d - lbl_eu_80667080);
                f32 sy = (f32)(ch.d - lbl_eu_80667080);
                PaneSizeFlagView* pv = (PaneSizeFlagView*)pane;
                pv->sizeX = sx;
                pv->sizeY = sy;
                pv->flags = (pv->flags & 0xFE) | 1;
            }
            char* s2 = func_80136190(&base[0x3b], &base[0x72], state);
            func_80115DB0(self, s2, 1);
        }
    }
}

// Open the key-assign menu for one of the 4 player indices (state 3 = already
// open). Loads the remap-column names via func_8013606C/func_80136190, finds
// the corresponding "timg" texture and sizes the target pane from the texture
// dimensions (u16 -> f32 via the 0x43300000 magic double), then assigns the
// button labels through func_80115DB0.
extern "C" void func_801159DC(CMenuKeyAssign* self) {
    if (self->mField_78 == 3) {
        return;
    }
    self->mField_78 = 3;
    func_80115BD8(self);

    const char* base = lbl_eu_804FDEE8;
    const char* name = &base[0x29];
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        name = &base[0x32];
    }

    u8 i;
    for (i = 0; i < 4; i++) {
        u8 mapped = i;
        if (i == 2) {
            mapped = 4;
        } else if (i == 3) {
            mapped = 5;
        }
        u8 idx = i + 3;
        u16 r = func_8013606C(&base[0x3b], name, idx);
        ml::FixStr<32> str(false);
        str.format(&base[0x5f], func_80136190(&base[0x49], &base[0x56], r));
        void* acc = func_801355F4();
        TexView* tex =
            (TexView*)((void* (*)(void*, u32, void*, u32))((void**)acc)[0xc / 4])(
                acc, 0x74696d67, str.mString, 0);
        if (tex != 0) {
            str.format(&base[0x66], mapped + 1);
            nw4r::lyt::Pane* pane =
                self->mLayout->GetRootPane()->FindPaneByName(str.mString, true);
            func_80137F88(pane, tex);
            u16 w = tex->mHeader->mDims[1];
            u16 h = tex->mHeader->mDims[0];
            // u16 -> f32 via MWCC's 0x43300000 magic double pair.
            F64Conv cw, ch;
            cw.w[0] = 0x43300000;
            ch.w[0] = 0x43300000;
            cw.w[1] = (u32)w;
            ch.w[1] = (u32)h;
            PaneSizeFlagView* pv = (PaneSizeFlagView*)pane;
            pv->sizeX = (f32)(cw.d - lbl_eu_80667080);
            pv->sizeY = (f32)(ch.d - lbl_eu_80667080);
            pv->flags = (pv->flags & 0xFE) | 1;
        }
        if (mapped == 4) {
            mapped = 2;
        }
        char* s2 = func_80136190(&base[0x3b], &base[0x72], idx);
        func_80115DB0(self, s2, mapped);
    }
}

// Hide button panes by name (called from Init and func_801159DC). Formats the
// pane name from the loop index; the 5 extra panes are skipped for i==5
// (retail shape - the i==5 pane is the one func_801159DC re-labels).
extern "C" void func_80115BD8(CMenuKeyAssign* self) {
    char buf[0x20];
    int i;
    for (i = 1; i <= 6; i++) {
        sprintf(buf, &lbl_eu_804FDEE8[0x66], i);
        PaneFlagView* p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        p->flags &= 0xFE;
        if (i != 5) {
            sprintf(buf, &lbl_eu_804FDEE8[0x77], i);
            p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            p->flags &= 0xFE;
            sprintf(buf, &lbl_eu_804FDEE8[0x83], i);
            p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            p->flags &= 0xFE;
            sprintf(buf, &lbl_eu_804FDEE8[0x91], i);
            p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            p->flags &= 0xFE;
            sprintf(buf, &lbl_eu_804FDEE8[0x9f], i);
            p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            p->flags &= 0xFE;
            sprintf(buf, &lbl_eu_804FDEE8[0xad], i);
            p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
            p->flags &= 0xFE;
        }
    }
}

// Assign the button label for one column: write the text into the pane name
// from the sprintf'd buffer, then unhide that pane (SetVisible(true) via the
// +0xBB flag byte). Called for each of the 5 remappable columns.
extern "C" void func_80115DB0(CMenuKeyAssign* self, char* str, int idx) {
    char buf[0x20];
    int v = idx + 1;
    sprintf(buf, &lbl_eu_804FDEE8[0x77], v);
    func_80136B4C(self->mLayout, buf, str, 0);
    PaneFlagView* p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    p->flags = (p->flags & 0xFE) | 1;
    sprintf(buf, &lbl_eu_804FDEE8[0x83], v);
    func_80136B4C(self->mLayout, buf, str, 0);
    p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    p->flags = (p->flags & 0xFE) | 1;
    sprintf(buf, &lbl_eu_804FDEE8[0x91], v);
    func_80136B4C(self->mLayout, buf, str, 0);
    p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    p->flags = (p->flags & 0xFE) | 1;
    sprintf(buf, &lbl_eu_804FDEE8[0x9f], v);
    func_80136B4C(self->mLayout, buf, str, 0);
    p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    p->flags = (p->flags & 0xFE) | 1;
    sprintf(buf, &lbl_eu_804FDEE8[0xad], v);
    func_80136B4C(self->mLayout, buf, str, 0);
    p = (PaneFlagView*)self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    p->flags = (p->flags & 0xFE) | 1;
}

// Target: us-80116a94 - IWorkEvent vtable adjustor thunk (subobject at +0x58).
// Called through the subobject vtable destructor slot, `this` points at
// CMenuKeyAssign + 0x58, so subtract 0x58 before forwarding to the full-object
// member. The retail emits exactly subi r3,r3,0x58; b <member>.
void func_80115FB8(void* self) { ((void(*)(void*))__dt__14CMenuKeyAssignFv)((char*)self - 0x58); }

// IScnRender vtable cbRenderBefore adjustor thunk (subobject at +0x5C).
void func_80115FC0(void* self) { ((void(*)(void*))cbRenderBefore__14CMenuKeyAssignFv)((char*)self - 0x5c); }

// Target: us-80116aa4 - IScnRender vtable adjustor thunk (subobject at +0x5C).
// Same shape as func_80115FB8 but for the IScnRender subobject.
void func_80115FC8(void* self) { ((void(*)(void*))__dt__14CMenuKeyAssignFv)((char*)self - 0x5c); }
