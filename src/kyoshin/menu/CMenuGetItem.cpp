// Auto-scaffolded catalog TU for kyoshin/menu/CMenuGetItem
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuGetItem.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <revolution/GX.h>

#include <stdio.h>

extern "C" void __dt__12CMenuGetItemFv(void*, int);
extern "C" void cbRenderBefore__12CMenuGetItemFv(void*);

// Already-open get-item handlers (stubs below; the factories call them). The
// noinline keeps the retail `bl` at the call sites (empty bodies would be
// inlined away by MWCC); extern "C" keeps the call relocs unmangled.
extern "C" __declspec(noinline) void func_8014A2E8(CMenuGetItem* self, u32 arg);
extern "C" __declspec(noinline) void func_8014A430(CMenuGetItem* self, u32 arg);
extern "C" __declspec(noinline) void func_8014A570(CMenuGetItem* self, int arg0, int arg1);

/*
 * Single get-item screen constructor (retail symbol keeps its unmangled
 * global name). Builds the CProcess base (raw storage, abstract class so the
 * ctor is called via its C-ABI name), installs the temp then composite
 * vtables, copies the null PMF triple into the Move/Draw callback slots, then
 * constructs the UnkClass_8045F564 scratch region and zeroes/initialises the
 * tail fields. `parent` is stored at +0x60 (the render-callback target scene)
 * and `arg` at +0x94; mField_2AF is set to 1 (the single variant marker).
 */
extern "C" CMenuGetItem* __ct__CMenuGetItem(CMenuGetItem* obj, CScn* parent,
                                             u32 arg) {
    __ct__8CProcessFv((CProcess*)obj);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IWorkEvent (+0x24) / IScnRender (+0xac) sub-vtables. The pmf
    // copy reads the null member-function-pointer triple through one base
    // register; post-increment derefs of a const local pointer fold the first
    // access into `lwzu` (btm_sco_init shape). Stores are [1],[0],[2] per
    // group (retail order 0x40,0x3C,0x44 / 0x4C,0x48,0x50).
    obj->mProcessVt = (u32)lbl_eu_8052BF70;
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    obj->ptmfMove[1] = w1;
    obj->ptmfMove[0] = w0;
    u32 w2 = *src++;
    obj->ptmfMove[2] = w2;
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    obj->ptmfDraw[1] = w0;
    obj->ptmfDraw[0] = w1;
    w2 = *src++;
    obj->ptmfDraw[2] = w2;
    obj->mField_54 = 0;
    obj->mField_55 = 0;

    obj->mProcessVt = (u32)lbl_eu_8052EA40;
    obj->mIWorkEventVt = (u32)lbl_eu_8052EA40 + 0x24;
    obj->mIScnRenderVt = (u32)lbl_eu_8052EA40 + 0xac;
    obj->mScn = parent;

    __ct__17UnkClass_8045F564Fv(&obj->mUnkClass[0]);

    obj->mLayout = 0;
    obj->mField_78 = 0;
    obj->mField_88 = 0;
    obj->mField_8C = lbl_eu_80667418;
    obj->mField_90 = 0;
    obj->mField_94 = arg;
    obj->mField_98 = 0;
    obj->mField_9C = 0;
    obj->mField_2AE = 0;
    obj->mField_2AF = 1;
    obj->mField_2B0 = 0;
    return obj;
}

/*
 * Second get-item variant constructor (retail placeholder name __ct__80149878,
 * not yet a target). Same 0x2B4 layout construction as __ct__CMenuGetItem, but
 * the ctor arg is stored at +0x98 (retail store order 0x94=0, 0x98=arg).
 */
extern "C" CMenuGetItem* __ct__80149878(CMenuGetItem* obj, CScn* parent,
                                         u32 arg) {
    __ct__8CProcessFv((CProcess*)obj);

    obj->mProcessVt = (u32)lbl_eu_8052BF70;
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    obj->ptmfMove[1] = w1;
    obj->ptmfMove[0] = w0;
    u32 w2 = *src++;
    obj->ptmfMove[2] = w2;
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    obj->ptmfDraw[1] = w0;
    obj->ptmfDraw[0] = w1;
    w2 = *src++;
    obj->ptmfDraw[2] = w2;
    obj->mField_54 = 0;
    obj->mField_55 = 0;

    obj->mProcessVt = (u32)lbl_eu_8052EA40;
    obj->mIWorkEventVt = (u32)lbl_eu_8052EA40 + 0x24;
    obj->mIScnRenderVt = (u32)lbl_eu_8052EA40 + 0xac;
    obj->mScn = parent;

    __ct__17UnkClass_8045F564Fv(&obj->mUnkClass[0]);

    obj->mLayout = 0;
    obj->mField_78 = 0;
    obj->mField_88 = 0;
    obj->mField_8C = lbl_eu_80667418;
    obj->mField_90 = 0;
    obj->mField_94 = 0;
    obj->mField_98 = arg;
    obj->mField_9C = 0;
    obj->mField_2AE = 0;
    obj->mField_2AF = 1;
    obj->mField_2B0 = 0;
    return obj;
}

/*
 * Multi get-item variant constructor (retail placeholder name __ct__80149970).
 * Same base construction as __ct__CMenuGetItem, but takes two extra args
 * stored at +0x9C/+0x9D, keeps mField_94 and mField_2AF at 0.
 */
extern "C" CMenuGetItem* __ct__80149970(CMenuGetItem* obj, CScn* parent,
                                         u32 arg0, u32 arg1) {
    __ct__8CProcessFv((CProcess*)obj);

    obj->mProcessVt = (u32)lbl_eu_8052BF70;
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    obj->ptmfMove[1] = w1;
    obj->ptmfMove[0] = w0;
    u32 w2 = *src++;
    obj->ptmfMove[2] = w2;
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    obj->ptmfDraw[1] = w0;
    obj->ptmfDraw[0] = w1;
    w2 = *src++;
    obj->ptmfDraw[2] = w2;
    obj->mField_54 = 0;
    obj->mField_55 = 0;

    obj->mProcessVt = (u32)lbl_eu_8052EA40;
    obj->mIWorkEventVt = (u32)lbl_eu_8052EA40 + 0x24;
    obj->mIScnRenderVt = (u32)lbl_eu_8052EA40 + 0xac;
    obj->mScn = parent;

    __ct__17UnkClass_8045F564Fv(&obj->mUnkClass[0]);

    obj->mLayout = 0;
    obj->mField_78 = 0;
    obj->mField_88 = 0;
    obj->mField_8C = lbl_eu_80667418;
    obj->mField_90 = 0;
    obj->mField_94 = 0;
    obj->mField_98 = 0;
    obj->mField_9C = arg0;
    obj->mField_9D = arg1;
    obj->mField_2AE = 0;
    obj->mField_2AF = 0;
    obj->mField_2B0 = 0;
    return obj;
}

CMenuGetItem::~CMenuGetItem() {
    // Scratch region + CProcess base destruction are driven by hand (retail
    // shape: member dtor with r4=-1, then the base dtor guarded by the nested
    // double null-check, an MWCC D2-inlined-into-D1 artifact). The conditional
    // operator delete is auto-emitted from the dtor flags.
    reinterpret_cast<UnkClass_8045F564*>(&mUnkClass[0])->~UnkClass_8045F564();
    if (this != 0) {
        if (this != 0) {
            // C-ABI base dtor call with flags=0 (retail shape).
            __dt__8CProcessFv(reinterpret_cast<CProcess*>(this), 0);
        }
    }
}

void CMenuGetItem::Init() {
    // Scratch MEM2 region for the layout file data (guarded by the RAII
    // Class_8045F858 host so the region is released on every exit path).
    // The region address is recomputed at each use (this+0x64) - a held
    // pointer local would claim an extra callee-saved register.
    reinterpret_cast<UnkClass_8045F564*>(&mUnkClass[0])->createRegion(
        (int)mtl::MemManager::getHandleMEM2(), 0x1000, lbl_eu_805018D0, 0);
    Class_8045F858 memHost(reinterpret_cast<UnkClass_8045F564*>(&mUnkClass[0]));

    mField_90 = func_80144FC8();

    {
        // String base kept in a block-scoped local for the build-up phase;
        // the switch below re-materializes the address (retail shape).
        char* base = lbl_eu_805018D0;

        // Build the layout and its first anim transform from the arc resource.
        func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
            &mLayout, func_801355F4(), &base[0xd]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            mLayout, &mField_78, func_801355F4(), &base[0x26]);

        // Bind the font: push the font object's pane back onto the root pane.
        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mLayout);
        u32 fontResult = ((u32 (*)(void*))(((void**)fontObj)[0x24 / 4]))(fontObj);
        func_8013676C(rootPane, fontResult);

        mLayout->SetAnimationEnable(mField_78, true);
        reinterpret_cast<AnimFrameView*>(mField_78)->frame = lbl_eu_80667418;
        mLayout->Animate(0);

        // Resolve the three variant panes by name.
        mField_7C = mLayout->GetRootPane()->FindPaneByName(&base[0x42], true);
        mField_80 = mLayout->GetRootPane()->FindPaneByName(&base[0x4c], true);
        mField_84 = mLayout->GetRootPane()->FindPaneByName(&base[0x58], true);
    }

    if (mField_2AF == 0) {
        // if-set, not ternary: MWCC branchless-selects constant ternaries.
        u8 fix = 2;
        if (mField_9D != 0) {
            fix = 3;
        }
        mField_2AF = fix;
    }

    // Resolve the title string for the active variant.
    const char* title = NULL;
    switch (mField_2AF) {
    case 1:
        title = (const char*)mField_98;
        if (title == NULL) {
            title = (const char*)func_801394D4((u16)mField_94);
        }
        break;
    case 2:
        title = func_80136190(&lbl_eu_805018D0[0x64], &lbl_eu_805018D0[0x6f],
                              mField_9C + 0x53);
        break;
    case 3:
        title = func_80136190(&lbl_eu_805018D0[0x74], &lbl_eu_805018D0[0x82],
                              mField_9C);
        mField_2B0 = func_8013606C(&lbl_eu_805018D0[0x74], &lbl_eu_805018D0[0x88],
                                   mField_9C);
        break;
    }

    // Hide the variant panes that do not match the active variant
    // (v = variant - 1; the low bit of +0xBB is the visible flag).
    u8 v = mField_2AF - 1;
    if (mField_7C != NULL) {
        reinterpret_cast<PaneFlagView*>(mField_7C)->flags =
            (reinterpret_cast<PaneFlagView*>(mField_7C)->flags & 0xFE) | (v > 0);
    }
    if (mField_80 != NULL) {
        reinterpret_cast<PaneFlagView*>(mField_80)->flags =
            (reinterpret_cast<PaneFlagView*>(mField_80)->flags & 0xFE) | (v > 1);
    }
    if (mField_84 != NULL) {
        reinterpret_cast<PaneFlagView*>(mField_84)->flags =
            (reinterpret_cast<PaneFlagView*>(mField_84)->flags & 0xFE) | (v > 2);
    }

    func_80136B4C(mLayout, &lbl_eu_805018D0[0x8c], (char*)title, 0);

    // MI adjust: IScnRender subobject at +0x5c (null-this safe).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    mScn->addRenderCB(cb, 7, 0);

    reinterpret_cast<UnkClass_8045F564*>(&mUnkClass[0])->func_8045F810();
}

void CMenuGetItem::Term() {
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

    if (lbl_eu_806641A8 == this) {
        lbl_eu_806641A8 = NULL;
    }
}

void CMenuGetItem::Move() {}

void CMenuGetItem::cbRenderBefore() {
    CTaskGame::getInstance();
    // Combined short-circuit produces retail's double branch for the bit-21
    // gate (bne return; beq continue; b return) - same shape as CMenuGCItem::Move.
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & (1u << 21))) {
        return;
    }
    if (!func_8013BE50()) {
        return;
    }
    if (lbl_eu_80663E24 & 0xbfe40000u) {
        return;
    }
    if (cf::CfGameManager::isSceneLoading()) {
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

/*
 * Single get-item factory (func_8014A064). If an instance already exists and
 * is not marked for removal, clear its +0x54 flag, forward the arg to
 * func_8014A2E8 and return 0; otherwise allocate a 0x2B4 block from work
 * memory, construct it, store it in the singleton and register it under
 * `registParent`, returning the stored instance.
 */
CMenuGetItem* func_8014A064(CProcess* registParent, CScn* parent, u32 arg) {
    CMenuGetItem* inst = lbl_eu_806641A8;
    if (inst != 0 && inst->mIsRemove == 0) {
        if (inst->mField_54 != 0) {
            inst->mField_54 = 0;
        }
        func_8014A2E8(lbl_eu_806641A8, arg);
        return 0;
    }

    CMenuGetItem* obj = (CMenuGetItem*)mtl::MemManager::allocate(
        sizeof(CMenuGetItem), CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // Reassign from the ctor return so obj stays in volatile r3 (retail
        // never spills it to a callee-saved register).
        obj = __ct__CMenuGetItem(obj, parent, arg);
    }
    lbl_eu_806641A8 = obj;
    ((CProcess*)obj)->Regist(registParent, 0);
    return lbl_eu_806641A8;
}

/*
 * Second get-item variant factory (func_8014A11C) - same shape as
 * func_8014A064 but constructs via __ct__80149878 and forwards to
 * func_8014A430.
 */
CMenuGetItem* func_8014A11C(CProcess* registParent, CScn* parent, u32 arg) {
    CMenuGetItem* inst = lbl_eu_806641A8;
    if (inst != 0 && inst->mIsRemove == 0) {
        if (inst->mField_54 != 0) {
            inst->mField_54 = 0;
        }
        func_8014A430(lbl_eu_806641A8, arg);
        return 0;
    }

    CMenuGetItem* obj = (CMenuGetItem*)mtl::MemManager::allocate(
        sizeof(CMenuGetItem), CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__80149878(obj, parent, arg);
    }
    lbl_eu_806641A8 = obj;
    ((CProcess*)obj)->Regist(registParent, 0);
    return lbl_eu_806641A8;
}

/*
 * Multi get-item variant factory (func_8014A1D4) - same shape with one extra
 * arg; constructs via __ct__80149970 and forwards both args to func_8014A570.
 */
CMenuGetItem* func_8014A1D4(CProcess* registParent, CScn* parent, u32 arg0,
                            u32 arg1) {
    CMenuGetItem* inst = lbl_eu_806641A8;
    if (inst != 0 && inst->mIsRemove == 0) {
        if (inst->mField_54 != 0) {
            inst->mField_54 = 0;
        }
        func_8014A570(lbl_eu_806641A8, arg0, arg1);
        return 0;
    }

    CMenuGetItem* obj = (CMenuGetItem*)mtl::MemManager::allocate(
        sizeof(CMenuGetItem), CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__80149970(obj, parent, arg0, arg1);
    }
    lbl_eu_806641A8 = obj;
    ((CProcess*)obj)->Regist(registParent, 0);
    return lbl_eu_806641A8;
}

extern "C" unsigned long func_8014A2A0(void) { return lbl_eu_806641A8 != 0; }

u8 func_8014A2B4() {
    if (lbl_eu_806641A8 != NULL) {
        return lbl_eu_806641A8->mField_90;
    }
    return 0;
}

void func_8014A2D0() {
    if (lbl_eu_806641A8 != NULL) {
        lbl_eu_806641A8->mField_54 = 1;
    }
}

extern "C" void func_8014A2E8(CMenuGetItem* self, u32 arg) {}

extern "C" void func_8014A430(CMenuGetItem* self, u32 arg) {}

extern "C" void func_8014A570(CMenuGetItem* self, int arg0, int arg1) {
    // Stage the non-empty entries into a local buffer, then write them back
    // and append the new entry. The buffer caps at 8 entries (the mItems
    // array is 8 * 0x42 bytes); when full the append is dropped entirely.
    GetItemEntry buf[8];
    u8 count = 0;
    const char* name;
    int type;
    for (u8 i = 0; i < self->mField_2AE; i++) {
        GetItemEntry* item = &self->mItems[i];
        if (item->type != 0) {
            GetItemEntry* dst = &buf[count];
            dst->type = item->type;
            count++;
            sprintf(dst->name, &lbl_eu_805018D0[0x95], item->name);
            dst->field_0x41 = item->field_0x41;
        }
    }

    if (count < 8) {
        self->mField_2AE = count;
        for (u8 i = 0; i < count; i++) {
            GetItemEntry* item = &self->mItems[i];
            item->type = buf[i].type;
            sprintf(item->name, &lbl_eu_805018D0[0x95], buf[i].name);
            item->field_0x41 = buf[i].field_0x41;
        }

        // Append the new entry (variant selected by arg1: 3 = item name,
        // 2 = party member name). The type select is an if-set, not a
        // ternary - MWCC branchless-selects constant ternaries (neg/or/
        // rlwinm) while retail uses a branch here.
        u8 cnt = self->mField_2AE;
        GetItemEntry* item = &self->mItems[cnt];
        self->mField_2AE = ++cnt;
        item->field_0x41 = arg0;
        type = 2;
        if (arg1 != 0) {
            type = 3;
        }
        item->type = (u8)type;
        if (arg1 != 0) {
            name = func_80136190(&lbl_eu_805018D0[0x74], &lbl_eu_805018D0[0x82],
                                 arg0);
        } else {
            name = func_80136190(&lbl_eu_805018D0[0x64], &lbl_eu_805018D0[0x6f],
                                 arg0 + 0x53);
        }
        sprintf(item->name, &lbl_eu_805018D0[0x95], name);
    }
}

void func_8014A6F8(){}

void func_8014A854(void* self) { ((void(*)(void*))__dt__12CMenuGetItemFv)((char*)self - 0x58); }

void func_8014A85C(void* self) { ((void(*)(void*))cbRenderBefore__12CMenuGetItemFv)((char*)self - 0x5c); }

void func_8014A864(void* self) { ((void(*)(void*))__dt__12CMenuGetItemFv)((char*)self - 0x5c); }
