#define IWORK_EVENT_INLINE_DTOR
// kyoshin/menu/CMenuKeyAssign - key-assign (button mapping) menu (singleton).
#include "kyoshin/menu/CMenuZeal.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/code_801F3BE0.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/menu/CMenuKeyAssign.hpp"
#include "kyoshin/menu/CMenuArtsSelect.hpp"
#include "kyoshin/cf/CfEnumObject.hpp"  // canonical findObjectById result type
#include "monolib/device/CDeviceVI.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
// Shield CSystemWindow.hpp's C++-linkage SysWinGetSingleton decl: this TU
// uses CfGimmick.hpp's extern "C" flat form (retail symbol is unmangled).
#include "kyoshin/CSystemWindow.hpp"
#include "kyoshin/cf/CfGimmick.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_drawInfo.h>

#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include <revolution/gx/GXPixel.h>

#include <stdio.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/menu/CMenuBattleCommu.hpp"

// Mirror of the font object returned by CDeviceFont::getFontInfo: vtable
// slot 0x24 (index 9, no args) yields the u32 bound into the layout's font
// pane via ApplyFontToTxtPanes. Never instantiated, so no vtable is emitted; a
// genuine virtual call makes MWCC emit retail's r12 dispatch sequence
// (CMenuQstCntFont / CMenuPTGaugeFont scheme).
class CMenuKeyAssignFont {
public:
    virtual ~CMenuKeyAssignFont();  // 0x00 (3 dtor slots)
    virtual void vf0C();            // 0x0C
    virtual void vf10();            // 0x10
    virtual void vf14();            // 0x14
    virtual void vf18();            // 0x18
    virtual void vf1C();            // 0x1C
    virtual void vf20();            // 0x20
    virtual u32 getFontHandle();    // 0x24
};

// Move's view of the arts-state object (MapFx_ResolveOwner result): sub-object at
// +4 whose vtable slot 0x30 returns the actor-id word.
struct CMenuKeyAssignActorSub {
    virtual void vf08();               // +0x08
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual u32* getActorId();         // +0x30
};
struct CMenuKeyAssignActor {
    u8 _00[0x4];
    CMenuKeyAssignActorSub* mSub04;    // +0x04
};

// Move's player view (CfObjectMove): battle sub-object at +0x38 carrying the
// battle-state id at +0x36C and the party count at +0x370.
struct CMenuKeyAssignBattleObj {
    u8 _00[0x36C];
    s32 mState36C;                     // +0x36C
    s32 mCount370;                     // +0x370
};
struct CMenuKeyAssignPlayer {
    u8 _00[0x38];
    CMenuKeyAssignBattleObj* mSub38;   // +0x38
};

// findObjectById result view: +0x64 flags word, +0x91 type byte.
// Kept as one explicit counted view: the real owner (CfEnumObject) is defined
// only in CfObjectEnumList.cpp (VS-OBJ owns the CfObject* headers), and its
// header declaration still lacks the +0x91 type byte.
// findObjectById results are real cf-enum objects. The canonical
// CfEnumObject declaration in kyoshin/cf/CfEnumObject.hpp supplies the
// +0x64 flags word (flags64) and the +0x91 actor-kind byte (type91);
// the former TU-local CActorTargetView duplicated those two fields.
//
//

// C-ABI import used by cbRenderBefore (retail unmangled name; not declared by
// any shared header yet, so declared here).

// C-linkage imports (retail names are unmangled free functions; MWCC mangles
// plain global-scope free functions with a __F suffix, so the declarations
// must sit under extern "C" to emit the retail reloc names).
extern "C" {
// Move's gate helpers (retail unmangled; declared in their own TUs but not by
// any included header).

int CMainMenu_IsOpen();
// Arts-state gate helpers used by Move (retail unmangled; declared in their
// own TUs but not by any included header).
void* CfObjectMove_getSelfIfActive(void* objParam);
void* MapFx_ResolveOwner(void* source);
bool CMenuArtsSelect_isCreated();
}

// C++-mangled import (retail symbol findObjectById__Fi): actor id -> target.
void* findObjectById(int id);

// Layout build helper - retail emits the mangled nw4r form
// buildLayout__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc,
// so declare it as a normal C++ function to let MWCC mangle it (NOT inside
// extern "C").

// Layout draw helper - retail emits the mangled nw4r form
// drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii, so declare it as
// a normal C++ function to let MWCC mangle it (NOT inside extern "C").

// Key-assign free helpers (defined in this TU). Retail emits these under the
// unmangled names, so the declarations/definitions keep C linkage.
// (memberized: hideKeyAssignPanes -> CMenuKeyAssign::hideKeyAssignPanes; declaration moved into class CMenuKeyAssign)
// (memberized: setKeyAssignLabels -> CMenuKeyAssign::setKeyAssignLabels; declaration moved into class CMenuKeyAssign)
// (memberized: openKeyAssignPlayer -> CMenuKeyAssign::openKeyAssignPlayer; declaration moved into class CMenuKeyAssign)

// Open the key-assign menu: state is the remap-column base index (also the
// mField_78 open state), count is the number of columns to show, mode selects
// the special 3-column layout. Retail emits these under the unmangled names.
// (memberized: openKeyAssignColumns -> CMenuKeyAssign::openKeyAssignColumns; declaration moved into class CMenuKeyAssign)
// (memberized: openKeyAssignButtons -> CMenuKeyAssign::openKeyAssignButtons; declaration moved into class CMenuKeyAssign)

// us-8011492c - CMenuKeyAssign::~CMenuKeyAssign
// Members and bases are destroyed by the compiler.
CMenuKeyAssign::~CMenuKeyAssign() {}

void CMenuKeyAssign::Init() {
    // Scratch MEM2 region for the layout file data (guarded by the RAII
    // Class_8045F858 host so the region is released on every exit path).
    // The region address is recomputed at each use (this+0x64) - a held
    // pointer local would claim an extra callee-saved register.
    mUnkClass.createRegion(
        (int)mtl::MemManager::getHandleMEM2(), 0x4200, "CMenuKeyAssign", 0);
    Class_8045F858 memHost(&mUnkClass);

    mtl::MemManager::setMemInitFlag(false);

    // Build the layout from the arc resource.
    buildLayout(&mLayout, CUICfManager_getArcResourceAccessor(), "mf70_cf40_keyassign.brlyt");

    // Bind the font: push the font object's pane back onto the root pane.
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    void* fontObj = CDeviceFont::getFontInfo(1, mLayout);
    u32 fontResult =
        reinterpret_cast<CMenuKeyAssignFont*>(fontObj)->getFontHandle();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

    // Hide the button panes that are not yet active (shared helper).
    this->hideKeyAssignPanes();

    mScn->addRenderCB(this, 0xa, 0);

    mUnkClass.validateHeap();
}

// us-80114a84 - CMenuKeyAssign::Term
// Wait for VI draw completion, detach the render callback (IScnRender
// subobject at +0x5C, null-this safe), destroy the layout, release the scratch
// region and clear the singleton.
void CMenuKeyAssign::Term() {
    CDeviceVI::waitForDrawDone();

    mScn->removeRenderCB(this);

    if (mLayout != NULL) {
        delete mLayout;
        mLayout = NULL;
    }

    mUnkClass.deleteRegion();

    sInstance = NULL;
}

// us-80114b04 - CMenuKeyAssign::Move
// Per-frame input handling: gate prefix (task busy / global bits / scene
// active / presentation flags / menu state), then dispatch on battle vs
// non-battle state, pad bits and the arts-select singleton to open the key-
// assign menu (openKeyAssignColumns / openKeyAssignButtons / openKeyAssignPlayer). The retail
// vf128 arts-gauge call is intentionally OUTSIDE the artsState null guard
// (retail dereferences the possibly-null pointer - mirrored exactly).
void CMenuKeyAssign::Move() {
    CMenuKeyAssign* self = this;
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() != 0) {
        return;
    }
    if (gmPauseFlags & (1u << 21)) {
        return;
    }
    if (IsMenuState621F0() == 0) {
        return;
    }
    if (GetSysStateFlag20() == 0) {
        return;
    }
    if (gmGameFlags & 0xafa40000u) {
        return;
    }
    if (cf::CfGameManager::isSceneLoading() != 0) {
        return;
    }

    CMenuKeyAssignPlayer* player =
        (CMenuKeyAssignPlayer*)cf::CfGameManager::getPlayer(0);
    CMenuKeyAssignActor* arts = (CMenuKeyAssignActor*)MapFx_ResolveOwner(player);
    CPad* pad = cf::CfGameManager::getCurrentPad();

    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // ---- in battle ----
        int battleMode = 0;
        cf::CBattleManager* bm =
            (cf::CBattleManager*)cf::CBattleManager::getInstance();
        if (bm->unk20C8.unk0 != 0) {
            battleMode = 1;
        }
        if (bm != 0) {
            u8 phase = bm->mChain.mHead.field_2;
            bool inRange = phase >= 1 && phase <= 0x18;
            if (inRange) {
                if (CMenuArtsSelect_IsAvailable() != 0) {
                    self->openKeyAssignColumns(0x19, 1, 0);
                    goto done;
                }
                self->openKeyAssignColumns(0x18, 1, 0);
                goto done;
            }
        }
        if ((pad->mHeldButtonFlags & 0x10) != 0) {
            int b = battleCommuIsActive();
            if (b == 0 && GetZealObjPtr() == 0) {
                self->openKeyAssignButtons(9, 10, 13, 12, 0, 1);
                goto done;
            }
        }
        if (CMainMenu_IsOpen() != 0) {
            self->openKeyAssignColumns(0x16, 2, 0);
            goto done;
        }
        if (CMenuArtsSelect_isCreated() != 0) {
            int handled = 0;
            if (arts != 0) {
                u32 id = *arts->mSub04->getActorId();
                if (testActorStatus(arts, &id, 0x803) != 0) {
                    if ((pad->mHeldButtonFlags & 8) != 0) {
                        int b2 = battleCommuIsActive();
                        if (battleMode == 0 && b2 == 0 &&
                            GetZealObjPtr() == 0) {
                            u32 cnt = bm->mActorList2.size();
                            if (cnt > 1) {
                                self->openKeyAssignPlayer();
                            } else {
                                self->openKeyAssignColumns(3, 1, 0);
                            }
                            handled = 1;
                            goto artsDone;
                        }
                    }
                    switch (player->mSub38->mState36C) {
                    case 1:
                        self->openKeyAssignColumns(player->mSub38->mCount370 + 0x1b, 0, 0);
                        handled = 1;
                        break;
                    case 2:
                        self->openKeyAssignColumns(player->mSub38->mCount370 + 0x1c, 0, 0);
                        handled = 1;
                        break;
                    case 3:
                        self->openKeyAssignColumns(player->mSub38->mCount370 + 0x1d, 0, 0);
                        handled = 1;
                        break;
                    case 4:
                        if (player->mSub38->mCount370 == 1) {
                            self->openKeyAssignColumns(0x1e, 0, 0);
                        } else {
                            self->openKeyAssignColumns(0x1f, 0, 0);
                        }
                        handled = 1;
                        break;
                    case 7:
                        self->openKeyAssignColumns(player->mSub38->mCount370 + 0x25, 0, 0);
                        handled = 1;
                        break;
                    case 8:
                        self->openKeyAssignColumns(player->mSub38->mCount370 + 0x26, 0, 0);
                        handled = 1;
                        break;
                    default:
                        self->openKeyAssignColumns(0x1a, 1, 0);
                        break;
                    }
                    goto artsDone;
                }
            }
            // arts == 0 or testActorStatus == 0: retail dereferences arts here.
            f32 gauge = ((f32 (*)(void*))((void**)arts)[0x128 / 4])(arts);
            if (gauge <= 0.0f) {
                self->openKeyAssignButtons(0x18, 1, 9, 0, 0, 1);
            } else {
                self->openKeyAssignButtons(1, 2, 9, 0, 0, 1);
            }
            handled = 1;
        artsDone:
            if (handled == 0) {
                if (CMenuArtsSelect_IsAvailable() != 0) {
                    self->openKeyAssignColumns(0x19, 1, 0);
                    goto done;
                }
                if (battleMode != 0) {
                    self->openKeyAssignButtons(0x18, 9, 0, 0, 0, 1);
                    goto done;
                }
                self->openKeyAssignButtons(0x18, 1, 9, 0, 0, 1);
                goto done;
            }
        }
        // Arts select not created: target/tech selection chain.
        if (player != 0) {
            void* battle = player->mSub38;
            if (((int (*)(void*, u32))((void**)battle)[0x40 / 4])(battle, 0x40000) != 0) {
                u32 cnt = bm->mActorList1.size();
                if (cnt == 0) {
                    int id = ((int (*)(void*))((void**)player)[0x4C / 4])(player);
                    if (id != 0) {
                        void* target = findObjectById(id);
                        if (target != 0) {
                            u32 flags = ((CfEnumObject*)target)->flags64;
                            if (flags & 0x10000000) {
                                void* bf = CfObjectMove_getSelfIfActive(target);
                                u32 st =
                                    ((u32 (*)(void*))((void**)bf)[0x228 / 4])(bf);
                                switch (st) {
                                case 3:
                                    self->openKeyAssignColumns(0x10, 1, 0);
                                    goto done;
                                case 2:
                                    self->openKeyAssignColumns(0x11, 2, 0);
                                    goto done;
                                case 9:
                                    self->openKeyAssignButtons(0xe, 0, 0, 0, 0, 0);
                                    goto done;
                                default:
                                    self->openKeyAssignColumns(0xf, 1, 0);
                                    goto done;
                                }
                            }
                            if ((flags & 0x4000) || (flags & 0x8000)) {
                                switch (((CfEnumObject*)target)->type91) {
                                case 6:
                                    self->openKeyAssignColumns(0x15, 1, 0);
                                    goto done;
                                case 0xc:
                                    self->openKeyAssignColumns(0x14, 1, 0);
                                    goto done;
                                case 9:
                                    self->openKeyAssignButtons(0xe, 0, 0, 0, 0, 0);
                                    goto done;
                                case 0xa:
                                    self->openKeyAssignButtons(0xe, 0, 0, 0, 0, 0);
                                    goto done;
                                case 0xb:
                                    self->openKeyAssignButtons(0xe, 0, 0, 0, 0, 0);
                                    goto done;
                                default:
                                    self->openKeyAssignColumns(0x13, 1, 0);
                                    goto done;
                                }
                            }
                            self->openKeyAssignButtons(0xe, 0, 0, 0, 0, 0);
                            goto done;
                        }
                        self->openKeyAssignColumns(0xe, 1, 0);
                        goto done;
                    }
                    self->openKeyAssignColumns(0xe, 1, 0);
                    goto done;
                }
                self->openKeyAssignColumns(0xe, 1, 0);
                goto done;
            }
        }
        // player == 0 / vf40 == 0: retry the target chain (double vf4C).
        if (player != 0) {
            int id = ((int (*)(void*))((void**)player)[0x4C / 4])(player);
            if (id != 0) {
                id = ((int (*)(void*))((void**)player)[0x4C / 4])(player);
                if (id != 0) {
                    if (findObjectById(id) != 0) {
                        self->openKeyAssignButtons(1, 2, 9, 0, 0, 1);
                        goto done;
                    }
                }
            }
        }
        self->openKeyAssignColumns(0xe, 1, 0);
        goto done;
    }

    // ---- not in battle ----
    int battleMode = 0;
    cf::CBattleManager* bm =
        (cf::CBattleManager*)cf::CBattleManager::getInstance();
    if (bm->unk20C8.unk0 != 0) {
        battleMode = 1;
    }
    if (bm != 0) {
        u8 phase = bm->mChain.mHead.field_2;
        bool inRange = phase >= 1 && phase <= 0x18;
        if (inRange) {
            if (CMenuArtsSelect_IsAvailable() != 0) {
                self->openKeyAssignColumns(0x19, 1, 0);
                goto done;
            }
            self->openKeyAssignColumns(0x18, 1, 0);
            goto done;
        }
    }
    if ((pad->mHeldButtonFlags & 0x800) != 0) {
        int b = battleCommuIsActive();
        if (b == 0 && GetZealObjPtr() == 0) {
            self->openKeyAssignButtons(9, 10, 13, 12, 0, 1);
            goto done;
        }
    }
    if (CMainMenu_IsOpen() != 0) {
        self->openKeyAssignColumns(0x16, 2, 0);
        goto done;
    }
    if ((pad->mHeldButtonFlags & 0x1000) != 0) {
        if (battleMode == 0) {
            int b = battleCommuIsActive();
            if (b == 0 && GetZealObjPtr() == 0) {
                if (arts != 0) {
                    u32 id = *arts->mSub04->getActorId();
                    if (testActorStatus(arts, &id, 0x803) != 0) {
                        u32 cnt = bm->mActorList2.size();
                        if (cnt > 1) {
                            self->openKeyAssignPlayer();
                            goto done;
                        }
                        self->openKeyAssignColumns(3, 1, 0);
                        goto done;
                    }
                }
                if (player != 0) {
                    int pid = ((int (*)(void*))((void**)player)[0x4C / 4])(player);
                    if (pid != 0) {
                        f32 gauge =
                            ((f32 (*)(void*))((void**)arts)[0x128 / 4])(arts);
                        if (gauge <= 0.0f) {
                            self->openKeyAssignColumns(1, 1, 0);
                            goto done;
                        }
                        self->openKeyAssignColumns(1, 2, 0);
                        goto done;
                    }
                }
                self->openKeyAssignColumns(0, 0, 0);
                goto done;
            }
        }
    }
    if (CMenuArtsSelect_isCreated() != 0) {
        int handled = 0;
        if (arts != 0) {
            u32 id = *arts->mSub04->getActorId();
            if (testActorStatus(arts, &id, 0x803) != 0) {
                switch (player->mSub38->mState36C) {
                case 1:
                    self->openKeyAssignColumns(player->mSub38->mCount370 + 0x1b, 0, 0);
                    handled = 1;
                    break;
                case 2:
                    self->openKeyAssignColumns(player->mSub38->mCount370 + 0x1c, 0, 0);
                    handled = 1;
                    break;
                case 3:
                    self->openKeyAssignColumns(player->mSub38->mCount370 + 0x1d, 0, 0);
                    handled = 1;
                    break;
                case 4:
                    if (player->mSub38->mCount370 == 1) {
                        self->openKeyAssignColumns(0x1e, 0, 0);
                    } else {
                        self->openKeyAssignColumns(0x1f, 0, 0);
                    }
                    handled = 1;
                    break;
                case 7:
                    self->openKeyAssignColumns(player->mSub38->mCount370 + 0x25, 0, 0);
                    handled = 1;
                    break;
                case 8:
                    self->openKeyAssignColumns(player->mSub38->mCount370 + 0x26, 0, 0);
                    handled = 1;
                    break;
                default:
                    self->openKeyAssignColumns(0x1a, 1, 0);
                    break;
                }
            }
        }
        if (handled == 0) {
            if (CMenuArtsSelect_IsAvailable() != 0) {
                self->openKeyAssignColumns(0x19, 1, 0);
                goto done;
            }
            self->openKeyAssignColumns(0x18, 1, 0);
            goto done;
        }
    }
    // Arts select not created (field): same target/tech chain as in battle.
    if (player != 0) {
        void* battle = player->mSub38;
        if (((int (*)(void*, u32))((void**)battle)[0x40 / 4])(battle, 0x40000) != 0) {
            u32 cnt = bm->mActorList1.size();
            if (cnt == 0) {
                int id = ((int (*)(void*))((void**)player)[0x4C / 4])(player);
                if (id != 0) {
                    void* target = findObjectById(id);
                    if (target != 0) {
                        u32 flags = ((CfEnumObject*)target)->flags64;
                        if (flags & 0x10000000) {
                            void* bf = CfObjectMove_getSelfIfActive(target);
                            u32 st =
                                ((u32 (*)(void*))((void**)bf)[0x228 / 4])(bf);
                            switch (st) {
                            case 3:
                                self->openKeyAssignColumns(0x10, 1, 0);
                                goto done;
                            case 2:
                                self->openKeyAssignColumns(0x11, 2, 0);
                                goto done;
                            case 9:
                                self->openKeyAssignButtons(0xe, 0, 0, 0, 0, 0);
                                goto done;
                            default:
                                self->openKeyAssignColumns(0xf, 1, 0);
                                goto done;
                            }
                        }
                        if ((flags & 0x4000) || (flags & 0x8000)) {
                            switch (((CfEnumObject*)target)->type91) {
                            case 6:
                                self->openKeyAssignColumns(0x15, 1, 0);
                                goto done;
                            case 0xc:
                                self->openKeyAssignColumns(0x14, 1, 0);
                                goto done;
                            case 9:
                                self->openKeyAssignButtons(0xe, 0, 0, 0, 0, 0);
                                goto done;
                            case 0xa:
                                self->openKeyAssignButtons(0xe, 0, 0, 0, 0, 0);
                                goto done;
                            case 0xb:
                                self->openKeyAssignButtons(0xe, 0, 0, 0, 0, 0);
                                goto done;
                            default:
                                self->openKeyAssignColumns(0x13, 1, 0);
                                goto done;
                            }
                        }
                        self->openKeyAssignButtons(0xe, 0, 0, 0, 0, 0);
                        goto done;
                    }
                    self->openKeyAssignColumns(0xe, 1, 0);
                    goto done;
                }
                self->openKeyAssignColumns(0xe, 1, 0);
                goto done;
            }
            self->openKeyAssignColumns(0xe, 1, 0);
            goto done;
        }
    }
    self->openKeyAssignColumns(0xe, 1, 0);
done:
    ((void (*)(void*, u32))((void**)self->mLayout)[0x38 / 4])(self->mLayout, 0);
}

void CMenuKeyAssign::Draw() {}

// us-80115964 - CMenuKeyAssign::cbRenderBefore
// Gate prefix (task busy / global bits / scene-active / presentation flags)
// then draw the embedded layout through a stack DrawInfo.
void CMenuKeyAssign::cbRenderBefore(CScn*) {
    CTaskGame::getInstance();
    // Combined short-circuit produces retail's double branch for the bit-21
    // gate (bne return; beq continue; b return) - same shape as CMenuGCItem::Move.
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & (1u << 21))) {
        return;
    }
    if (!IsMenuState621F0()) {
        return;
    }
    if (getUnk80664658()->mFlags & (1u << 20)) {
        return;
    }
    if (!GetSysStateFlag20()) {
        return;
    }
    if (gmGameFlags & 0xafa40000u) {
        return;
    }
    if (cf::CfGameManager::isSceneLoading()) {
        return;
    }

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawLayout(mLayout, &drawInfo, 0, 1);
}

// Constructor: retail builds the object inside the factory (no separate ctor
// function). Member order matches the retail store order (scene, scratch
// region, layout, open state).
inline CMenuKeyAssign::CMenuKeyAssign(CScn* scene) : mScn(scene), mLayout(0), mField_78(0) {}

CMenuKeyAssign* CMenuKeyAssign::sInstance;

// Only one key-assign menu exists at a time.
CMenuKeyAssign* CMenuKeyAssign::create(CProcess* parent, CScn* scene) {
    if (sInstance != 0) {
        return 0;
    }
    CMenuKeyAssign* menu = new (CWorkThreadSystem::getWorkMem()) CMenuKeyAssign(scene);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

// Open the key-assign menu for `count` remap columns starting at `state`
// (target us-80115b3c). Hides the panes, loads the remap-column names via
// BdatGetU16ByTableKey/BdatTouchStringCell, finds the "timg" texture, sizes
// the pane from the texture dimensions and assigns the button labels.
void CMenuKeyAssign::openKeyAssignColumns(int state, int count, int mode) {
    f32 sx, sy;
    f32 sx2, sy2;
    f32 sx3, sy3;

    if ((s32)this->mField_78 == state) {
        return;
    }
    this->mField_78 = (u32)state;
    this->hideKeyAssignPanes();
    if (state == 0) {
        return;
    }

    const char* name = "fileID_1";
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        name = "fileID_2";
    }

    if (count > 0) {
        int i;
        for (i = 0; i < count; i++) {
            int idx = state + i;
            u32 timg = 0x74696d67;
            u16 r = BdatGetU16ByTableKey("MNU_kyeassign", name, idx);
            ml::FixStr<32> str(true);
            f32 sx, sy;
            char* texName = BdatTouchStringCell("SYS_filelist", "filename", r);
            str.format("%s.tpl", texName);
            nw4r::lyt::ArcResourceAccessor* acc = CUICfManager_getArcResourceAccessor();
            TPLPalette* tex = (TPLPalette*)acc->GetResource(timg, str.mString, NULL);
            if (tex != 0) {
                int flag = 0;
                if (count == 3 && mode != 0 && i == 2) {
                    flag = 1;
                }
                str.format("pic_pos%02d", (flag ? 4 : i) + 1);
                nw4r::lyt::Pane* pane =
                    this->mLayout->GetRootPane()->FindPaneByName(str.mString, true);
                PaneSetTexPalette(pane, (TPLPalette*)tex);
                u16 w = tex->descriptorArray->textureHeader->width;
                u16 h = tex->descriptorArray->textureHeader->height;
                sx = (f32)(u32)w;
                sy = (f32)(u32)h;
                pane->SetSize(nw4r::lyt::Size(sx, sy));
                pane->SetVisible(true);
            }
            if (i < 4) {
                char* s2 = BdatTouchStringCell("MNU_kyeassign", "help", idx);
                this->setKeyAssignLabels(s2, i);
            }
        }
    } else {
        // count <= 0: two fixed panes (name index 26, then `state`).
        {
            u16 r = BdatGetU16ByTableKey("MNU_kyeassign", name, 26);
            ml::FixStr<32> str2(true);
            f32 sx2, sy2;
            char* texName = BdatTouchStringCell("SYS_filelist", "filename", r);
            str2.format("%s.tpl", texName);
            nw4r::lyt::ArcResourceAccessor* acc = CUICfManager_getArcResourceAccessor();
            TPLPalette* tex = (TPLPalette*)acc->GetResource(0x74696d67, str2.mString, NULL);
            if (tex != 0) {
                str2.format("pic_pos%02d", 1);
                nw4r::lyt::Pane* pane =
                    this->mLayout->GetRootPane()->FindPaneByName(str2.mString, true);
                PaneSetTexPalette(pane, (TPLPalette*)tex);
                u16 w = tex->descriptorArray->textureHeader->width;
                u16 h = tex->descriptorArray->textureHeader->height;
                sx2 = (f32)(u32)w;
                sy2 = (f32)(u32)h;
                pane->SetSize(nw4r::lyt::Size(sx2, sy2));
                pane->SetVisible(true);
            }
            char* s2 = BdatTouchStringCell("MNU_kyeassign", "help", 26);
            this->setKeyAssignLabels(s2, 0);
        }
        {
            u16 r = BdatGetU16ByTableKey("MNU_kyeassign", name, state);
            ml::FixStr<32> str3(true);
            f32 sx3, sy3;
            char* texName = BdatTouchStringCell("SYS_filelist", "filename", r);
            str3.format("%s.tpl", texName);
            nw4r::lyt::ArcResourceAccessor* acc = CUICfManager_getArcResourceAccessor();
            TPLPalette* tex = (TPLPalette*)acc->GetResource(0x74696d67, str3.mString, NULL);
            if (tex != 0) {
                str3.format("pic_pos%02d", 2);
                nw4r::lyt::Pane* pane =
                    this->mLayout->GetRootPane()->FindPaneByName(str3.mString, true);
                PaneSetTexPalette(pane, (TPLPalette*)tex);
                u16 w = tex->descriptorArray->textureHeader->width;
                u16 h = tex->descriptorArray->textureHeader->height;
                sx3 = (f32)(u32)w;
                sy3 = (f32)(u32)h;
                pane->SetSize(nw4r::lyt::Size(sx3, sy3));
                pane->SetVisible(true);
            }
            char* s2 = BdatTouchStringCell("MNU_kyeassign", "help", state);
            this->setKeyAssignLabels(s2, 1);
        }
    }
}

// Open the key-assign menu for up to 5 remap buttons (target us-80115fac). The
// five indices a..e are the button ids; `mode` (6th arg) selects the special
// 3-column layout. mField_78 stores (a+b+c+d+e)*100 as the open state. `count`
// is the index of the first zero id (5 when all non-zero); the ids are kept in
// a stack array because the loop indexes them by pointer.
void CMenuKeyAssign::openKeyAssignButtons(int a, int b, int c, int d, int e, int mode) {
    int state = (e + (d + b)) + (c + a);
    state *= 100;

    if (this->mField_78 == (u32)state) {
        return;
    }
    this->mField_78 = (u32)state;
    this->hideKeyAssignPanes();
    if (state == 0) {
        return;
    }

    const char* name = "fileID_1";
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        name = "fileID_2";
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
        int i;
        for (i = 0; i < count; i++) {
            int idx = args[i];
            u16 r = BdatGetU16ByTableKey("MNU_kyeassign", name, idx);
            ml::FixStr<32> str(true);
            char* texName = BdatTouchStringCell("SYS_filelist", "filename", r);
            str.format("%s.tpl", texName);
            nw4r::lyt::ArcResourceAccessor* acc = CUICfManager_getArcResourceAccessor();
            TPLPalette* tex = (TPLPalette*)acc->GetResource(0x74696d67, str.mString, NULL);
            if (tex != 0) {
                int flag = 0;
                if (count >= 3 && mode != 0 && i == 2) {
                    flag = 1;
                }
                str.format("pic_pos%02d", (flag ? 4 : i) + 1);
                nw4r::lyt::Pane* pane =
                    this->mLayout->GetRootPane()->FindPaneByName(str.mString, true);
                PaneSetTexPalette(pane, (TPLPalette*)tex);
                u16 w = tex->descriptorArray->textureHeader->width;
                u16 h = tex->descriptorArray->textureHeader->height;
                f32 sx = (f32)(u32)w;
                f32 sy = (f32)(u32)h;
                pane->SetSize(nw4r::lyt::Size(sx, sy));
                pane->SetVisible(true);
            }
            if (i < 4) {
                char* s2 = BdatTouchStringCell("MNU_kyeassign", "help", idx);
                this->setKeyAssignLabels(s2, i);
            }
        }
    } else {
        // count == 0: two fixed panes (name index 26, then `state`).
        {
            u16 r = BdatGetU16ByTableKey("MNU_kyeassign", name, 26);
            ml::FixStr<32> str2(true);
            char* texName = BdatTouchStringCell("SYS_filelist", "filename", r);
            str2.format("%s.tpl", texName);
            nw4r::lyt::ArcResourceAccessor* acc = CUICfManager_getArcResourceAccessor();
            TPLPalette* tex = (TPLPalette*)acc->GetResource(0x74696d67, str2.mString, NULL);
            if (tex != 0) {
                str2.format("pic_pos%02d", 1);
                nw4r::lyt::Pane* pane =
                    this->mLayout->GetRootPane()->FindPaneByName(str2.mString, true);
                PaneSetTexPalette(pane, (TPLPalette*)tex);
                u16 w = tex->descriptorArray->textureHeader->width;
                u16 h = tex->descriptorArray->textureHeader->height;
                f32 sx = (f32)(u32)w;
                f32 sy = (f32)(u32)h;
                pane->SetSize(nw4r::lyt::Size(sx, sy));
                pane->SetVisible(true);
            }
            char* s2 = BdatTouchStringCell("MNU_kyeassign", "help", 26);
            this->setKeyAssignLabels(s2, 0);
        }
        {
            u16 r = BdatGetU16ByTableKey("MNU_kyeassign", name, state);
            ml::FixStr<32> str3(true);
            char* texName = BdatTouchStringCell("SYS_filelist", "filename", r);
            str3.format("%s.tpl", texName);
            nw4r::lyt::ArcResourceAccessor* acc = CUICfManager_getArcResourceAccessor();
            TPLPalette* tex = (TPLPalette*)acc->GetResource(0x74696d67, str3.mString, NULL);
            if (tex != 0) {
                str3.format("pic_pos%02d", 2);
                nw4r::lyt::Pane* pane =
                    this->mLayout->GetRootPane()->FindPaneByName(str3.mString, true);
                PaneSetTexPalette(pane, (TPLPalette*)tex);
                u16 w = tex->descriptorArray->textureHeader->width;
                u16 h = tex->descriptorArray->textureHeader->height;
                f32 sx = (f32)(u32)w;
                f32 sy = (f32)(u32)h;
                pane->SetSize(nw4r::lyt::Size(sx, sy));
                pane->SetVisible(true);
            }
            char* s2 = BdatTouchStringCell("MNU_kyeassign", "help", state);
            this->setKeyAssignLabels(s2, 1);
        }
    }
}

// Open the key-assign menu for one of the 4 player indices (state 3 = already
// open). Loads the remap-column names via BdatGetU16ByTableKey/BdatTouchStringCell,
// finds the corresponding "timg" texture, sizes the target pane and assigns
// the button labels through setKeyAssignLabels.
void CMenuKeyAssign::openKeyAssignPlayer() {
    if ((s32)this->mField_78 == 3) {
        return;
    }
    this->mField_78 = 3;
    this->hideKeyAssignPanes();

    const char* name = "fileID_1";
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        name = "fileID_2";
    }

    for (u8 i = 0; i < 4; i++) {
        u8 mapped = i;
        if (i == 2) {
            mapped = 4;
        } else if (i == 3) {
            mapped = 5;
        }
        u8 idx = i + 3;
        u16 r = BdatGetU16ByTableKey("MNU_kyeassign", name, idx);
        ml::FixStr<32> str(true);
        char* texName = BdatTouchStringCell("SYS_filelist", "filename", r);
        str.format("%s.tpl", texName);
        nw4r::lyt::ArcResourceAccessor* acc = CUICfManager_getArcResourceAccessor();
        TPLPalette* tex = (TPLPalette*)acc->GetResource(0x74696d67, str.mString, NULL);
        if (tex != 0) {
            str.format("pic_pos%02d", mapped + 1);
            nw4r::lyt::Pane* pane =
                this->mLayout->GetRootPane()->FindPaneByName(str.mString, true);
            PaneSetTexPalette(pane, (TPLPalette*)tex);
            f32 sx = (f32)(u32)tex->descriptorArray->textureHeader->width;
            f32 sy = (f32)(u32)tex->descriptorArray->textureHeader->height;
            pane->SetSize(nw4r::lyt::Size(sx, sy));
            pane->SetVisible(true);
        }
        if (mapped == 4) {
            mapped = 2;
        }
        char* s2 = BdatTouchStringCell("MNU_kyeassign", "help", idx);
        this->setKeyAssignLabels(s2, mapped);
    }
}

// Hide button panes by name (called from Init and openKeyAssignPlayer). Formats the
// pane name from the loop index; the 5 extra panes are skipped for i==5
// (retail shape - the i==5 pane is the one openKeyAssignPlayer re-labels).
void CMenuKeyAssign::hideKeyAssignPanes() {
    char buf[0x20];
    int i;
    for (i = 1; i <= 6; i++) {
        sprintf(buf, "pic_pos%02d", i);
        nw4r::lyt::Pane* p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
        p->SetVisible(false);
        if (i != 5) {
            sprintf(buf, "txt_pos%02d", i);
            p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
            p->SetVisible(false);
            sprintf(buf, "txt_pos%02d_l", i);
            p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
            p->SetVisible(false);
            sprintf(buf, "txt_pos%02d_r", i);
            p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
            p->SetVisible(false);
            sprintf(buf, "txt_pos%02d_o", i);
            p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
            p->SetVisible(false);
            sprintf(buf, "txt_pos%02d_u", i);
            p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
            p->SetVisible(false);
        }
    }
}

// Assign the button label for one column: write the text into the pane name
// from the sprintf'd buffer, then unhide that pane (SetVisible(true) via the
// +0xBB flag byte). Called for each of the 5 remappable columns.
void CMenuKeyAssign::setKeyAssignLabels(char* str, int idx) {
    char buf[0x20];
    int v = idx + 1;
    sprintf(buf, "txt_pos%02d", v);
    LayoutSetTextBoxFmtValue(this->mLayout, buf, str, 0);
    nw4r::lyt::Pane* p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
    p->SetVisible(true);
    sprintf(buf, "txt_pos%02d_l", v);
    LayoutSetTextBoxFmtValue(this->mLayout, buf, str, 0);
    p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
    p->SetVisible(true);
    sprintf(buf, "txt_pos%02d_r", v);
    LayoutSetTextBoxFmtValue(this->mLayout, buf, str, 0);
    p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
    p->SetVisible(true);
    sprintf(buf, "txt_pos%02d_o", v);
    LayoutSetTextBoxFmtValue(this->mLayout, buf, str, 0);
    p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
    p->SetVisible(true);
    sprintf(buf, "txt_pos%02d_u", v);
    LayoutSetTextBoxFmtValue(this->mLayout, buf, str, 0);
    p = this->mLayout->GetRootPane()->FindPaneByName(buf, true);
    p->SetVisible(true);
}
