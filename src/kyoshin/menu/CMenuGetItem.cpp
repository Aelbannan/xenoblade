#define IWORK_EVENT_INLINE_DTOR
// kyoshin/menu/CMenuGetItem - get-item screen controller.

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
void bindLayoutAnimTransform(nw4r::lyt::Layout* layout, nw4r::lyt::AnimTransform** anim, nw4r::lyt::ArcResourceAccessor* accessor, char* name);
void buildLayout(nw4r::lyt::Layout** layout, nw4r::lyt::ArcResourceAccessor* accessor, const char* name);

// Get-item string table (layout name, anim name, pane names, sprintf formats).
extern char lbl_eu_805018D0[];
// cbRenderBefore gate flag (bit 21 skips the draw).
extern u32 lbl_eu_80663E28;
// GetItem singleton instance (retail SDA symbol).
extern CMenuGetItem* lbl_eu_806641A8;

// View of the nw4r Pane bytes this TU touches (retail nw4r keeps the visible
// flag in the low bit of the byte at +0xBB; the repo's nw4r headers don't
// model that field).
struct PaneFlagView {
    u8 _00[0xBB];
    u8 flags; // 0xBB - bit 0 = visible
};

// View of the CProcess remove flag (CProcess keeps mIsRemove private at +0x39
// with no accessor; the factories read it through this view).
struct ProcessRemoveView {
    u8 _00[0x39];
    u8 mIsRemove; // 0x39
};

// View of the AnimTransform float this TU touches (current-frame counter at
// +0x10; repo nw4r AnimTransform is a skeleton without members).
struct AnimFrameView {
    u8 _00[0x10];
    f32 frame; // 0x10
};

// C-linkage imports (retail names are unmangled free functions in this TU;
// MWCC mangles plain global-scope free functions with a __F suffix, so the
// declarations must sit under extern "C" to emit the retail reloc names).
extern "C" u8 isLandTelopActive();
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
extern "C" void* BuildItemNameStr(u32);
// Pre-mangled static member name (CDeviceFont::getFontInfo(u32, Layout*));
// extern "C" keeps the literal identifier (no double mangling).
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);

// Already-open get-item handlers (stubs below; the factories call them). The
// noinline keeps the retail `bl` at the call sites (empty bodies would be
// inlined away by MWCC); extern "C" keeps the call relocs unmangled.
extern "C" __declspec(noinline) void CMenuGetItem_RefreshSingle(CMenuGetItem* self, u32 arg);
extern "C" __declspec(noinline) void CMenuGetItem_RefreshVariant(CMenuGetItem* self, u32 arg);
extern "C" __declspec(noinline) void CMenuGetItem_AppendEntry(CMenuGetItem* self, int arg0, int arg1);

// ---------------------------------------------------------------------------
// Constructors: retail keeps each as a separate out-of-line function (the
// factories below call them). The IUICf base (CTTask, null PMF slots, fields
// 0x54/0x55), the vtables and the scratch region are set up by the compiler;
// the members initialise in declaration order.
// ---------------------------------------------------------------------------
CMenuGetItem::CMenuGetItem(CScn* parent, u32 arg)
    : mScn(parent), mLayout(nullptr), mField_78(nullptr), mField_88(0),
      mField_8C(0.0f), mField_90(0), mField_94(arg), mField_98(0), mField_9C(0),
      mField_2AE(0), mField_2AF(1), mField_2B0(0) {}

CMenuGetItem::CMenuGetItem(CScn* parent, s32 arg)
    : mScn(parent), mLayout(nullptr), mField_78(nullptr), mField_88(0),
      mField_8C(0.0f), mField_90(0), mField_94(0), mField_98(arg), mField_9C(0),
      mField_2AE(0), mField_2AF(1), mField_2B0(0) {}

CMenuGetItem::CMenuGetItem(CScn* parent, u32 arg0, u32 arg1)
    : mScn(parent), mLayout(nullptr), mField_78(nullptr), mField_88(0),
      mField_8C(0.0f), mField_90(0), mField_94(0), mField_98(0), mField_9C(arg0),
      mField_9D(arg1), mField_2AE(0), mField_2AF(0), mField_2B0(0) {}

// ---------------------------------------------------------------------------
// Dtor: the scratch region and the IUICf/IWorkEvent/IScnRender bases are
// destroyed by the compiler.
// ---------------------------------------------------------------------------
CMenuGetItem::~CMenuGetItem() {}

void CMenuGetItem::Init() {
    // Scratch MEM2 region for the layout file data (guarded by the RAII
    // Class_8045F858 host so the region is released on every exit path).
    // The region address is recomputed at each use (this+0x64) - a held
    // pointer local would claim an extra callee-saved register.
    mMemRegion.createRegion(
        (int)mtl::MemManager::getHandleMEM2(), 0x1000, lbl_eu_805018D0, 0);
    Class_8045F858 memHost(&mMemRegion);

    mField_90 = isLandTelopActive();

    {
        // String base kept in a block-scoped local for the build-up phase;
        // the switch below re-materializes the address (retail shape).
        char* base = lbl_eu_805018D0;

        // Build the layout and its first anim transform from the arc resource.
        buildLayout(&mLayout, CUICfManager_getArcResourceAccessor(), &base[0xd]);
        bindLayoutAnimTransform(mLayout, &mField_78, CUICfManager_getArcResourceAccessor(), &base[0x26]);

        // Bind the font: push the font object's pane back onto the root pane.
        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mLayout);
        u32 fontResult = ((u32 (*)(void*))(((void**)fontObj)[0x24 / 4]))(fontObj);
        ApplyFontToTxtPanes(rootPane, fontResult);

        mLayout->SetAnimationEnable(mField_78, true);
        reinterpret_cast<AnimFrameView*>(mField_78)->frame = 0.0f;
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
            title = (const char*)BuildItemNameStr((u16)mField_94);
        }
        break;
    case 2:
        title = BdatTouchStringCell(&lbl_eu_805018D0[0x64], &lbl_eu_805018D0[0x6f],
                              mField_9C + 0x53);
        break;
    case 3:
        title = BdatTouchStringCell(&lbl_eu_805018D0[0x74], &lbl_eu_805018D0[0x82],
                              mField_9C);
        mField_2B0 = BdatGetU16ByTableKey(&lbl_eu_805018D0[0x74], &lbl_eu_805018D0[0x88],
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

    LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_805018D0[0x8c], (char*)title, 0);

    // MI adjust: IScnRender subobject at +0x5c (null-this safe).
    mScn->addRenderCB(this, 7, 0);

    mMemRegion.validateHeap();
}

void CMenuGetItem::Term() {
    CDeviceVI::waitForDrawDone();

    // MI adjust: IScnRender subobject at +0x5c (null-this safe).
    mScn->removeRenderCB(this);

    if (mLayout != NULL) {
        delete mLayout;
        mLayout = NULL;
    }

    mMemRegion.deleteRegion();

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
    if (!IsMenuState621F0()) {
        return;
    }
    if (lbl_eu_80663E24 & 0xbfe40000u) {
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

/*
 * Single get-item factory (CMenuGetItem_CreateSingle). If an instance already exists and
 * is not marked for removal, clear its +0x54 flag, forward the arg to
 * CMenuGetItem_RefreshSingle and return 0; otherwise allocate a 0x2B4 block from work
 * memory, construct it, store it in the singleton and register it under
 * `registParent`, returning the stored instance.
 */
extern "C" CMenuGetItem* CMenuGetItem_CreateSingle(CProcess* registParent, CScn* parent, u32 arg) {
    CMenuGetItem* inst = lbl_eu_806641A8;
    if (inst != 0 && reinterpret_cast<ProcessRemoveView*>(inst)->mIsRemove == 0) {
        if (inst->mField54 != 0) {
            inst->mField54 = 0;
        }
        CMenuGetItem_RefreshSingle(lbl_eu_806641A8, arg);
        return 0;
    }

    CMenuGetItem* obj = (CMenuGetItem*)mtl::MemManager::allocate(
        sizeof(CMenuGetItem), CWorkThreadSystem::getWorkMem());
    // Reassign from the ctor return so obj stays in volatile r3 (retail
    // never spills it to a callee-saved register).
    obj = new (obj) CMenuGetItem(parent, arg);
    lbl_eu_806641A8 = obj;
    ((CProcess*)obj)->Regist(registParent, 0);
    return lbl_eu_806641A8;
}

/*
 * Second get-item variant factory (CMenuGetItem_CreateVariant) - same shape as
 * CMenuGetItem_CreateSingle but constructs the variant and forwards to
 * CMenuGetItem_RefreshVariant.
 */
extern "C" CMenuGetItem* CMenuGetItem_CreateVariant(CProcess* registParent, CScn* parent, u32 arg) {
    CMenuGetItem* inst = lbl_eu_806641A8;
    if (inst != 0 && reinterpret_cast<ProcessRemoveView*>(inst)->mIsRemove == 0) {
        if (inst->mField54 != 0) {
            inst->mField54 = 0;
        }
        CMenuGetItem_RefreshVariant(lbl_eu_806641A8, arg);
        return 0;
    }

    CMenuGetItem* obj = (CMenuGetItem*)mtl::MemManager::allocate(
        sizeof(CMenuGetItem), CWorkThreadSystem::getWorkMem());
    obj = new (obj) CMenuGetItem(parent, static_cast<s32>(arg));
    lbl_eu_806641A8 = obj;
    ((CProcess*)obj)->Regist(registParent, 0);
    return lbl_eu_806641A8;
}

/*
 * Multi get-item variant factory (CMenuGetItem_CreateMulti) - same shape with one extra
 * arg; constructs the multi variant and forwards both args to CMenuGetItem_AppendEntry.
 */
extern "C" CMenuGetItem* CMenuGetItem_CreateMulti(CProcess* registParent, CScn* parent, u32 arg0,
                            u32 arg1) {
    CMenuGetItem* inst = lbl_eu_806641A8;
    if (inst != 0 && reinterpret_cast<ProcessRemoveView*>(inst)->mIsRemove == 0) {
        if (inst->mField54 != 0) {
            inst->mField54 = 0;
        }
        CMenuGetItem_AppendEntry(lbl_eu_806641A8, arg0, arg1);
        return 0;
    }

    CMenuGetItem* obj = (CMenuGetItem*)mtl::MemManager::allocate(
        sizeof(CMenuGetItem), CWorkThreadSystem::getWorkMem());
    obj = new (obj) CMenuGetItem(parent, arg0, arg1);
    lbl_eu_806641A8 = obj;
    ((CProcess*)obj)->Regist(registParent, 0);
    return lbl_eu_806641A8;
}

extern "C" unsigned long CMenuGetItem_IsActive(void) { return lbl_eu_806641A8 != 0; }

extern "C" u8 CMenuGetItem_GetField90() {
    if (lbl_eu_806641A8 != NULL) {
        return lbl_eu_806641A8->mField_90;
    }
    return 0;
}

extern "C" void CMenuGetItem_SetField54() {
    if (lbl_eu_806641A8 != NULL) {
        lbl_eu_806641A8->mField54 = 1;
    }
}

extern "C" void CMenuGetItem_RefreshSingle(CMenuGetItem* self, u32 arg) {}

extern "C" void CMenuGetItem_RefreshVariant(CMenuGetItem* self, u32 arg) {}

extern "C" void CMenuGetItem_AppendEntry(CMenuGetItem* self, int arg0, int arg1) {
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
            name = BdatTouchStringCell(&lbl_eu_805018D0[0x74], &lbl_eu_805018D0[0x82],
                                 arg0);
        } else {
            name = BdatTouchStringCell(&lbl_eu_805018D0[0x64], &lbl_eu_805018D0[0x6f],
                                 arg0 + 0x53);
        }
        sprintf(item->name, &lbl_eu_805018D0[0x95], name);
    }
}

void CMenuGetItem_TickDisplay(){}



