// High-level C++ reconstruction for kyoshin/menu/CMenuUpdate
// ~IScnRender stays out of line here: retail's ~CMenuUpdate calls
// __dt__10IScnRenderFv (the inline-empty header form would elide it).
#define ISCN_RENDER_OUT_OF_LINE_DTOR
#include "kyoshin/cf/CfBdat.hpp"
#include <string.h>
#include <types.h>
#include "kyoshin/menu/CMenuUpdate.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
// code80135FDC_getByte_64059: owner decl (u8 return) lives in
// CfGameManagerUnityHelpers.hpp, which isn't self-contained here; local
// caller-shape copy with the owner's exact signature until that header is
// includable (owner: kyoshin/code_80135FDC).
// int return: retail tests the result with a bare cmpwi (no rlwinm mask).
// Scn_QueryUnk80State: retail branches here WITHOUT setting up r3 (stale-register
// call), so this TU keeps a private no-arg declaration and must not include
// libs/monolib/src/scn/CScn_8049603C.hpp (owner: monolib CScn).
extern "C" void* Scn_QueryUnk80State();
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "monolib/util/FixStr.hpp"
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTypes.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <nw4r/lyt/lyt_resourceAccessor.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include "monolib/util/MemManager.hpp"  // canonical mtl::MemManager::allocate

extern u32 lbl_eu_80664198;
// Out-of-line accessor helpers (defined further down; retail calls them via bl)
extern "C" u32 getField10(u8* self);
extern "C" void setField67(u8* self, u8 val);
extern "C" void setField64(u8* self, u8 val);
extern "C" int getField64(u8* self);

// CMenuUpdate_8014274C ctor (retail keeps it out of line: the entry ctor has
// nine call sites in CMenuUpdate's constructor).
extern "C" CMenuUpdate_8014274C* __ct__8014274C(CMenuUpdate_8014274C* self) {
    self->field_0 = 0; self->field_4 = 0; self->field_8 = 0; self->field_C = 0;
    return self;
}

// CMenuUpdate ctor. Member init list reproduces the retail store order (scene
// store before the compiler's vptr stores, mMemRegion's out-of-line ctor after
// them, then the hook pointers).
CMenuUpdate::CMenuUpdate(CScn* scene, u32 type, u32 arg1, u32 arg2, u32 arg3)
    : mScene(scene), mAnim1(NULL), mAnim2(NULL), mActiveAnim(NULL) {
    // 8 entry slots plus one more header-sized slot over the state block
    CMenuUpdate_8014274C* entryEnd;
    CMenuUpdate_8014274C* entry = &mEntries[0];
    entryEnd = (CMenuUpdate_8014274C*)&mState;
    do {
        __ct__8014274C(entry);
        entry++;
    } while (entry < entryEnd);
    __ct__8014274C(entryEnd);

    mMode = 2;
    f32 zero = 0.0f;
    mFloat128 = zero;
    mFloat12C = zero;
    memset(mEntries, 0, 0x80);
    memset(&mEntries[8], 0, 0x10);

    UpsertEntry(type, arg1, arg2, arg3);
}

// CMenuUpdate dtor: member/base destruction in retail order (mMemRegion, then
// the IScnRender / IWorkEvent / IUIWindow bases; the deleting path is the
// compiler's own).
CMenuUpdate::~CMenuUpdate() {}

// Init
void CMenuUpdate::Init() {
    u32 handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
    mMemRegion.createRegion(handle, 0x4000, "CMenuUpdate", 0);

    Class_8045F858 stackObj(&mMemRegion);

    nw4r::lyt::ArcResourceAccessor* accessor =
        static_cast<nw4r::lyt::ArcResourceAccessor*>(CUICfManager_getArcResourceAccessor());
    buildLayout(&mLayout, accessor, "mf70_cf52_sideinfo.brlyt");

    accessor = static_cast<nw4r::lyt::ArcResourceAccessor*>(CUICfManager_getArcResourceAccessor());
    bindLayoutAnimTransform(mLayout, &mAnim1, accessor, (char*)"mf70_cf52_sideinfo_in.brlan");

    accessor = static_cast<nw4r::lyt::ArcResourceAccessor*>(CUICfManager_getArcResourceAccessor());
    bindLayoutAnimTransform(mLayout, &mAnim2, accessor, (char*)"mf70_cf52_sideinfo_kizuna_in.brlan");

    // One-arg call: retail carries &mAnim2 into r4 from the preceding
    // bindLayoutAnimTransform argument setup instead of recomputing it here.
    u8* font = (u8*)getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1);
    u32 result2 = ((CMenuUpdateFontView*)font)->getHandle();
    ApplyFontToTxtPanes((nw4r::lyt::Pane*)((void*)::getField10((u8*)mLayout)), (const nw4r::ut::Font*)((void*)result2));

    // Disable the alt animation, enable the main one, then tick the layout.
    mLayout->SetAnimationEnable(mAnim2, false);
    mLayout->SetAnimationEnable(mAnim1, true);
    mLayout->Animate(0);

    ::setField67((u8*)this, 0);
    // Render callback sits at the IScnRender subobject (+0x70); keep the
    // init-then-adjust form so the null test runs on the raw this pointer.
    u8* renderPtr = (u8*)this;
    if (renderPtr != NULL) {
        renderPtr += 0x70;
    }
    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, renderPtr, 7, 0);
    mMemRegion.validateHeap();
    // stackObj destructor called automatically
}

// Term
void CMenuUpdate::Term() {
    waitForDrawDone__9CDeviceVIFv();
    // Render callback sits at the IScnRender subobject (+0x70); keep the
    // init-then-adjust form so the null test runs on the raw this pointer.
    u8* renderPtr = (u8*)this;
    if (renderPtr != NULL) {
        renderPtr += 0x70;
    }
    removeRenderCB__4CScnFP10IScnRender(mScene, (IScnRender*)renderPtr);
    // Polymorphic delete: dispatches the virtual dtor at vtable+0x8 with the
    // deleting flag 1; the delete expansion's own null-check is the dead
    // second beq in retail.
    if (mLayout != NULL) {
        delete mLayout;
        mLayout = NULL;
    }
    mMemRegion.deleteRegion();
    lbl_eu_80664198 = 0;
}

// Move
void CMenuUpdate::Move() {
    void* taskGame = getInstance__9CTaskGameFv();
    // Short-circuit OR gate: first test -> bne exit, res flag -> beq continue /
    // b exit (CMenuShopBuy::Move shape).
    if (isFlag01Set__9CTaskGameFv(taskGame) || testResInfoFlag(0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    if (MenuUpdate_CheckBootGates(this)) return;

    switch ((int)mMode) {
    case 0: MenuUpdate_EnterMode1Chime(this); break;
    case 1: MenuUpdate_MaintainActive(this); break;
    case 2: MenuUpdate_GuardFirstEntry(this); break;
    }

    // Layout::Animate(0) - virtual slot 0x38
    mLayout->Animate(0);
}

// cbRenderBefore
void CMenuUpdate::cbRenderBefore() {
    void* taskGame = getInstance__9CTaskGameFv();
    // Short-circuit OR gate (CMenuShopBuy::cbRenderBefore shape).
    if (isFlag01Set__9CTaskGameFv(taskGame) || testResInfoFlag(0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    if (MenuUpdate_CheckBootGates(this)) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);

    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawLayout(mLayout, &drawInfo, 0, 1);
    // No explicit dtor: the implicit member destruction emits the retail's
    // single __dt__ call (an explicit call would double it).
}

// Helper accessors
extern "C" u32 getField10(u8* self) { return *(u32*)(self + 0x10); }
extern "C" void setField67(u8* self, u8 val) { self[0x67] = val; }
extern "C" u32 func_80142954(u8* self) { return *(u32*)(self + 0x10); }
extern "C" void func_8014295C(u8* self, u8 val) { self[0x67] = val; }
extern "C" void func_80142C98(u8* self, u8 val) { self[0x64] = val; }
extern "C" void setField64(u8* self, u8 val) { self[0x64] = val; }
extern "C" u8 func_80143F4C(u8* self) { return self[0x64]; }
extern "C" int getField64(u8* self) { return self[0x64]; }
extern "C" void* getGlobalA10() { return (void*)lbl_eu_80664A10; }
extern "C" int getGlobal0E0() { return (u32)::cf::CfBdat::spBtlBuffListFileData; }
extern "C" void MenuUpdate_MarkByte15Ready(void* self) { ((u8*)self)[0x15] = 1; }
extern "C" void MenuUpdate_SignalGlobalField64() { if (lbl_eu_80664198) { ((void(*)(void*, u8))setField64)((void*)(u32)lbl_eu_80664198, 1); } }
extern "C" void* noop_80142D5C(void* self) { return self; }
extern "C" void* setVec2(void* self, f32 a, f32 b) { *(f32*)self = a; *(f32*)((u8*)self + 4) = b; return self; }
extern "C" void setFieldFloat10(void* self, f32 val) { *(f32*)((u8*)self + 0x10) = val; }
extern "C" void func_80143AC8(void* self, f32 a, f32 b) { setVec2(self, a, b); }
extern "C" void func_80143AD4(void* self, f32 val) { setFieldFloat10(self, val); }
// Returns self (see __ct__8014274C note).
extern "C" CMenuUpdate_8014274C* init_8014274C(CMenuUpdate_8014274C* self, u32 v0, u32 v1, u32 v2, u32 v3) {
    self->field_0 = v0; self->field_4 = v1; self->field_8 = v2; self->field_C = v3;
    return self;
}
extern "C" void func_80143F38(CMenuUpdate_8014274C* self, u32 v0, u32 v1, u32 v2, u32 v3) {
    init_8014274C(self, v0, v1, v2, v3);
}
extern "C" void MenuUpdate_CopyQuadWord(u32* dest, const u32* src) {
    const u32* s = src; u32* d = dest;
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3];
}

// MenuUpdate_SpawnProcessForMode - process factory. Mode 2 flags a global; mode 6 checks a
// save-flag resource and marks it dirty. Reuses the existing process if one is
// registered, otherwise allocates a CMenuUpdate (0x130 bytes from work mem).
// The ctor result redefines obj so the pointer stays in r3 across the call.
extern "C" void* MenuUpdate_SpawnProcessForMode(void* self, u32 pScene, int mode, int arg1, int arg2, int arg3) {
    if (mode == 2) { MenuUpdate_MarkByte15Ready(getGlobalA10()); }
    if (mode == 6) {
        int val = getGlobal0E0();
        u16 result = BdatGetU16Direct((void*)val, "help", arg1);
        if (!result) return 0;
        if (CtrlRemote_TouchBitByArg(arg1 + 0x3214)) {
            if (GetSysStateFlag22()) {
                return 0;
            }
        } else {
            CtrlRemote_SetSharedBit(arg1 + 0x3214, 1);
        }
    }
    if (lbl_eu_80664198 != 0) {
        ((CMenuUpdate*)(void*)(u32)lbl_eu_80664198)->UpsertEntry(mode, arg1, arg2, arg3);
        return 0;
    }
    void* workMem = getWorkMem__17CWorkThreadSystemFv();
    CMenuUpdate* obj = new (mtl::MemManager::allocate(0x130, (u32)workMem))
        CMenuUpdate((CScn*)pScene, mode, arg1, arg2, arg3);
    lbl_eu_80664198 = (u32)obj;
    Regist__8CProcessFP8CProcessb((void*)(u32)lbl_eu_80664198, self, 0);
    return (void*)(u32)lbl_eu_80664198;
}

// MenuUpdate_FormatNotifyText - format a texture name into a FixStr<32> buffer and bind it
// into the layout. The FixStr default-init is the out-of-line CfGameManager
// helper (retail shape).
extern "C" void MenuUpdate_FormatNotifyText(void* self, void* name, void* fmtArg) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    if (!name) return;
    if (!fmtArg) return;

    u8 buf[0x24];
    // FixStr<32> default-init resolves to the retail CfGameManager-scope helper
    reinterpret_cast<cf::CfGameManager*>(buf)->clearTempTextBuffer();
    reinterpret_cast<ml::FixStr<32>*>(buf)->format(
        "%s.tpl", fmtArg);
    void* str = noop_80142D5C(buf);

    nw4r::lyt::ResourceAccessor* accessor =
        (nw4r::lyt::ResourceAccessor*)CUICfManager_getArcResourceAccessor();
    void* text = accessor->GetResource(0x74696D67, (const char*)str, NULL);
    if (text) PaneSetTexPaletteByName(obj->mLayout, (const char*)name, (TPLPalette*)text);
}

// MenuUpdate_RebindLayout - (re)bind the window layout's textures, animations and pane
// visibility for the current state, then start the active animation.
// Resource/pane names are string literals (pooled in @stringBase0); mLayout is
// reloaded from the object before every use.
extern "C" void MenuUpdate_RebindLayout(void* self) {
    u32 h;
    u32 w;
    nw4r::lyt::Pane* paneA;
    nw4r::lyt::Pane* paneB;
    nw4r::lyt::Pane* paneC;
    nw4r::lyt::Pane* paneD;
    CMenuUpdate* obj = (CMenuUpdate*)self;

    obj->mLayout->UnbindAllAnimation();

    // Shared icon texture: message id picked by controller type.
    char* icon = MakeTplNameSysFile(
        BdatGetU16ByTableKey("MNU_kyeassign",
                      cf::CfGameManager::isClassicController(-1) ? "fileID_2"
                                                           : "fileID_1",
                      0x2a));
    void* tex = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())
                    ->GetResource(0x74696D67 /* 'timg' */, icon, NULL);
    if (tex != NULL) {
        PaneSetTexPaletteByName(obj->mLayout, "pic_button_q", (TPLPalette*)tex);
        PaneSetTexPaletteByName(obj->mLayout, "pic_button_g", (TPLPalette*)tex);
        PaneSetTexPaletteByName(obj->mLayout, "pic_button_b", (TPLPalette*)tex);

        // Texture object carries pixel dimensions through a two-level chain.
        // u32 locals so the int->float conversion uses the 0x4330 double trick.
        h = ((CMenuUpdateTexObj*)tex)->mChain->mDims->mH;
        w = ((CMenuUpdateTexObj*)tex)->mChain->mDims->mW;

        nw4r::lyt::Pane* pane = ((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                                    ->FindPaneByName("pic_button_q", true);
        if (pane != NULL) {
            float v[2];
            writePanePos(pane, setVec2(v, h, w));
        }
        pane = ((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                   ->FindPaneByName("pic_button_g", true);
        if (pane != NULL) {
            float v[2];
            writePanePos(pane, setVec2(v, h, w));
        }
        pane = ((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                   ->FindPaneByName("pic_button_b", true);
        if (pane != NULL) {
            float v[2];
            writePanePos(pane, setVec2(v, h, w));
        }
    }

    // Bind the three state animations and hide all four state panes.
    char* animName =
        BdatTouchStringCell("MNU_kyeassign", "help",
                      0x2a);
    LayoutSetTextBoxFmtValue(obj->mLayout, "txt_kakunin_q", animName, 0);
    LayoutSetTextBoxFmtValue(obj->mLayout, "txt_kakunin_g", animName, 0);
    LayoutSetTextBoxFmtValue(obj->mLayout, "txt_kakunin_b", animName, 0);

    paneA = ((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                                 ->FindPaneByName("nul_kizuna", true);
    paneB = ((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                                 ->FindPaneByName("nul_quest", true);
    paneC = ((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                                 ->FindPaneByName("nul_gram", true);
    paneD = ((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                                 ->FindPaneByName("nul_buf", true);
    setPaneVisible(paneA, 0);
    setPaneVisible(paneB, 0);
    setPaneVisible(paneC, 0);
    setPaneVisible(paneD, 0);

    void* res;

    switch (obj->mState) {
    case 1:
        setPaneVisible(paneB, 1);
        obj->mActiveAnim = obj->mAnim1;
        break;

    case 2: {
        setPaneVisible(paneC, 1);
        res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())
                  ->GetResource(0x74696D67, "mf70_cf52_sideinfo03.tpl", NULL);
        if (res != NULL) {
            PaneSetTexPaletteByName(obj->mLayout, "pic_icon_g", (TPLPalette*)res);
        }
        obj->mActiveAnim = obj->mAnim1;
        break;
    }

    case 4: {
        const char* faceArea = "mf00_com12_face_area";
        char* s = BdatTouchStringCell("SYS_filelist", "filename", obj->mSubState + 0x6f);
        MenuUpdate_FormatNotifyText(obj, (void*)"pic_face00", (void*)faceArea);
        MenuUpdate_FormatNotifyText(obj, (void*)"pic_face01", s);

        // Sub-type specific texture over the first text slot.
        res = NULL;
        switch (obj->mSubType) {
        case 1:
            res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())
                      ->GetResource(0x74696D67, "mf00_com12_face_area01.tpl", NULL);
            break;
        case 2:
            res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())
                      ->GetResource(0x74696D67, "mf00_com12_face_area02.tpl", NULL);
            break;
        case 3:
            res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())
                      ->GetResource(0x74696D67, "mf00_com12_face_area03.tpl", NULL);
            break;
        case 4:
            res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())
                      ->GetResource(0x74696D67, "mf00_com12_face_area04.tpl", NULL);
            break;
        case 5:
            res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())
                      ->GetResource(0x74696D67, "mf00_com12_face_area05.tpl", NULL);
            break;
        }
        if (res != NULL) {
            PaneSetTexPaletteByName(obj->mLayout, "pic_face00", (TPLPalette*)res);
        }

        // Counter sign selects the countdown vs count-up gauge group; the
        // magnitude picks the number-strip texture bound to the gauge pane.
        if (obj->mCounter < 0) {
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("nul_anim_b", true),
                          1);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("nul_anim_g", true),
                          0);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("pic_kiz_b", true),
                          0);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("pic_rp_b", true),
                          1);
            if (obj->mCounter <= -200) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_rp03.tpl", NULL);
            } else if (obj->mCounter <= -100) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_rp02.tpl", NULL);
            } else {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_rp01.tpl", NULL);
            }
            if (res != NULL) {
                PaneSetTexPaletteByName(obj->mLayout, "pic_rp_b", (TPLPalette*)res);
            }
        } else {
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("nul_anim_b", true),
                          0);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("nul_anim_g", true),
                          1);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("pic_kiz_g", true),
                          0);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("pic_rp_g", true),
                          1);
            if (obj->mCounter >= 500) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_rp00.tpl", NULL);
            } else if (obj->mCounter >= 300) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_rp04.tpl", NULL);
            } else if (obj->mCounter >= 200) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_rp03.tpl", NULL);
            } else if (obj->mCounter >= 100) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_rp02.tpl", NULL);
            } else {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_rp01.tpl", NULL);
            }
            if (res != NULL) {
                PaneSetTexPaletteByName(obj->mLayout, "pic_rp_g", (TPLPalette*)res);
            }
        }
        setPaneVisible(paneA, 1);
        obj->mActiveAnim = obj->mAnim2;
        break;
    }

    case 5: {
        // Wide-vs-narrow number strip chosen by whether 32 digits fit.
        // wideFlag initializes before the len call (retail hoists li r26,0).
        u32 wideFlag = 0;
        u32 len = BdatGetU16ByTableKey("JNL_PCrelatelist",
                                "flg_s1", 5);
        if ((u32)CtrlRemote_TouchBitByArg(0x20) >= (u32)(u16)len) {
            wideFlag = 1;
        }

        // Face-texture columns of JNL_PClinelist, narrow/wide variant per slot.
        const char* face1Cols[2] = {"face_n01", "face_m01"};
        const char* face2Cols[2] = {"face_n02", "face_m02"};
        u16 face1 = BdatGetU16ByTableKey("JNL_PClinelist", face1Cols[wideFlag], obj->mSubType);
        u16 face2 = BdatGetU16ByTableKey("JNL_PClinelist", face2Cols[wideFlag], obj->mSubType);
        char* s1 = BdatTouchStringCell("SYS_filelist", "filename", face1);
        char* s2 = BdatTouchStringCell("SYS_filelist", "filename", face2);
        MenuUpdate_FormatNotifyText(obj, (void*)"pic_face00", s1);
        MenuUpdate_FormatNotifyText(obj, (void*)"pic_face01", s2);
        PaneSetVtxColorByName(obj->mLayout, "pic_face00", -1);

        if (obj->mCounter < 0) {
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("nul_anim_b", true),
                          1);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("nul_anim_g", true),
                          0);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("pic_kiz_b", true),
                          1);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("pic_rp_b", true),
                          0);
            if (obj->mCounter <= -50) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_kizp04.tpl", NULL);
            } else if (obj->mCounter <= -16) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_kizp02.tpl", NULL);
            } else {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_kizp01.tpl", NULL);
            }
            if (res != NULL) {
                PaneSetTexPaletteByName(obj->mLayout, "pic_kiz_b", (TPLPalette*)res);
            }
        } else {
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("nul_anim_b", true),
                          0);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("nul_anim_g", true),
                          1);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("pic_kiz_g", true),
                          1);
            setPaneVisible(((nw4r::lyt::Pane*)getField10((u8*)obj->mLayout))
                              ->FindPaneByName("pic_rp_g", true),
                          0);
            if (obj->mCounter >= 300) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_kizp00.tpl", NULL);
            } else if (obj->mCounter >= 50) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_kizp04.tpl", NULL);
            } else if (obj->mCounter >= 21) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_kizp03.tpl", NULL);
            } else if (obj->mCounter >= 16) {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_kizp02.tpl", NULL);
            } else {
                res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())->GetResource(0x74696D67, "mf00_com00_kizp01.tpl", NULL);
            }
            if (res != NULL) {
                PaneSetTexPaletteByName(obj->mLayout, "pic_kiz_g", (TPLPalette*)res);
            }
        }
        setPaneVisible(paneA, 1);
        obj->mActiveAnim = obj->mAnim2;
        break;
    }

    case 6:
        LayoutSetTextBoxFmtValue(obj->mLayout, "txt_buf",
                      BdatGetPtrDirect((const void*)getGlobal0E0(),
                                    "name", obj->mSubState),
                      0);
        setPaneVisible(paneD, 1);
        obj->mActiveAnim = obj->mAnim1;
        break;

    case 3: {
        setPaneVisible(paneC, 1);
        res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())
                  ->GetResource(0x74696D67, "mf00_reg00_tut.tpl", NULL);
        if (res != NULL) {
            PaneSetTexPaletteByName(obj->mLayout, "pic_icon_g", (TPLPalette*)res);
        }
        obj->mActiveAnim = obj->mAnim1;
        break;
    }
    }

    // Start the active animation from its initial frame.
    obj->mLayout->BindAnimation(obj->mActiveAnim);
    obj->mLayout->SetAnimationEnable(obj->mActiveAnim, true);
    setFieldFloat10(obj->mActiveAnim, 0.0f);
    obj->mLayout->Animate(0);
}

// MenuUpdate_UpsertEntry - insert/select a window entry.
// r4 = entry type, r5..r7 = payload words. Selects an existing matching entry
// or appends into the first free slot; the general path preserves leading
// entries, clears the table, and re-appends around the new entry.
void CMenuUpdate::UpsertEntry(int type, u32 arg1, u32 arg2, u32 arg3) {
    // Local re-declarations in this order pin MWCC's parameter-save order.
    u32 sArg2 = arg2;
    CMenuUpdate* self = this;
    int sType = type;
    u32 sArg1 = arg1;
    u32 sArg3 = arg3;

    // Free-slot scan over entries[0..6] only (entry 7 is never tested).
    int found = 0;
    if (self->mEntries[0].field_0 == 0) found = 1;
    else if (self->mEntries[1].field_0 == 0) found = 1;
    else if (self->mEntries[2].field_0 == 0) found = 1;
    else if (self->mEntries[3].field_0 == 0) found = 1;
    else if (self->mEntries[4].field_0 == 0) found = 1;
    else if (self->mEntries[5].field_0 == 0) found = 1;
    else if (self->mEntries[6].field_0 == 0) found = 1;
    if (!found) return;

    if (sType < 4) {
        if (self->mState == sType) return;
        // Counted loop (mtctr shape): match-type test precedes the free-slot
        // insert; inserting fills the slot then clears the one-shot flag.
        for (u8 i = 0; i < 8; i++) {
            CMenuUpdate_8014274C* e = &self->mEntries[i];
            if (e->field_0 == sType) return;
            if (e->field_0 == 0) {
                CMenuUpdate_8014274C t;
                MenuUpdate_CopyQuadWord((u32*)e, (u32*)init_8014274C(&t, sType, sArg1, sArg2, sArg3));
                if (!::getField64((u8*)self)) return;
                ::setField64((u8*)self, 0);
                return;
            }
        }
        return;
    }

    if (sType == 6) {
        if (self->mSubState == (int)sArg1) return;
        // Duplicate-substate probe (unrolled x4 by the compiler).
        u8 i;
        for (i = 0; i < 8; i++) {
            CMenuUpdate_8014274C* e = &self->mEntries[i];
            if (e->field_0 == 6 && e->field_4 == (int)sArg1) return;
        }
        for (i = 0; i < 8; i++) {
            CMenuUpdate_8014274C* e = &self->mEntries[i];
            if (e->field_0 == 0) {
                CMenuUpdate_8014274C t;
                MenuUpdate_CopyQuadWord((u32*)e, (u32*)init_8014274C(&t, sType, sArg1, sArg2, sArg3));
                if (!::getField64((u8*)self)) return;
                ::setField64((u8*)self, 0);
                return;
            }
        }
        return;
    }

    // General insert: rebuild the table as [kept tail][new entry][prefix].
    // Retail zeroes two scratch tables with eight out-of-line ctor calls each.
    CMenuUpdate_8014274C keepA[8]; // sp+0xc8..
    __ct__8014274C(&keepA[0]);
    __ct__8014274C(&keepA[1]);
    __ct__8014274C(&keepA[2]);
    __ct__8014274C(&keepA[3]);
    __ct__8014274C(&keepA[4]);
    __ct__8014274C(&keepA[5]);
    __ct__8014274C(&keepA[6]);
    __ct__8014274C(&keepA[7]);

    // Save the trailing active-window entries (types 4/5) into keepA.
    u8 nA = 0;
    u8 i;
    for (i = 0; i < 8; i++) {
        CMenuUpdate_8014274C* e = &self->mEntries[i];
        if (e->field_0 == 4 || e->field_0 == 5) {
            MenuUpdate_CopyQuadWord((u32*)&keepA[nA].field_0, (u32*)&e->field_0);
            nA++;
        }
    }

    CMenuUpdate_8014274C keepB[8]; // sp+0x48..
    __ct__8014274C(&keepB[0]);
    __ct__8014274C(&keepB[1]);
    __ct__8014274C(&keepB[2]);
    __ct__8014274C(&keepB[3]);
    __ct__8014274C(&keepB[4]);
    __ct__8014274C(&keepB[5]);
    __ct__8014274C(&keepB[6]);
    __ct__8014274C(&keepB[7]);

    // Save the leading non-empty run (starting after the kept tail) keepB.
    u8 nB = 0;
    u8 idx = nA;
    for (; idx < 8; idx++) {
        if (self->mEntries[idx].field_0 == 0) break;
        MenuUpdate_CopyQuadWord((u32*)&keepB[nB].field_0, (u32*)&self->mEntries[idx].field_0);
        nB++;
    }

    // Blank the whole table through a per-slot zeroed temporary.
    for (u8 k = 0; k < 8; k++) {
        CMenuUpdate_8014274C clr;
        __ct__8014274C(&clr);
        MenuUpdate_CopyQuadWord((u32*)&self->mEntries[k].field_0, (u32*)&clr.field_0);
    }

    // Re-file: kept tail, the new entry, then the saved prefix run. The
    // write index reuses nA itself (retail keeps both in one register).
    u8 j;
    for (j = 0; j < nA; j++) {
        MenuUpdate_CopyQuadWord((u32*)&self->mEntries[j].field_0, (u32*)&keepA[j].field_0);
    }

    CMenuUpdate_8014274C neu; // sp+0x8
    MenuUpdate_CopyQuadWord((u32*)&self->mEntries[nA].field_0,
                  (u32*)init_8014274C(&neu, sType, sArg1, sArg2, sArg3));
    nA++;

    for (j = 0; j < nB; j++) {
        MenuUpdate_CopyQuadWord((u32*)&self->mEntries[nA + j].field_0, (u32*)&keepB[j].field_0);
    }
}

// MenuUpdate_CheckBootGates - pause/transition guard; decrements a fade timer in 0x12C
extern "C" int MenuUpdate_CheckBootGates(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    if (!code80135FDC_getByte_64059() && !isGlobalCamFlagSet(0x1000000) &&
        isGlobalCamFlagSet((int)0xBFE40000)) {
        return 1;
    }
    if (MenuTutorialIsCreated()) obj->mFloat12C = 4.0f;
    if (isSceneLoading__Q22cf13CfGameManagerFv()) return 1;
    if (isSceneActive__Q22cf13CfGameManagerFv()) return 1;
    if (!isSceneReadyForInput__Q22cf13CfGameManagerFv()) return 1;
    if (isKizunaTalkActive()) return 1;
    if (SkipTimer_IsActiveFlag()) return 1;
    // Clamp: subtract the fade step into the field, then clamp.
    // 0.0f is const-qualified so the load may hoist above the
    // member store, reproducing retail's sub/store/cmp/store order.
    obj->mFloat12C = obj->mFloat12C - 1.0f;
    if (obj->mFloat12C < 0.0f) {
        obj->mFloat12C = 0.0f;
    }
    return 0;
}

// MenuUpdate_EnterMode1Chime
extern "C" void MenuUpdate_EnterMode1Chime(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    playUISound(0x1F);
    obj->mMode = 1;
}

// MenuUpdate_MaintainActive
extern "C" void MenuUpdate_MaintainActive(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;

    if (isGlobalCamFlagSet(0x1000000) && obj->mState != 4 && obj->mState != 5) {
        // Active-window maintenance: compact the table around the active
        // entries (types 4/5) and re-file the saved header block after them.
        obj->mMode = 2;
        setFieldFloat10(obj->mActiveAnim, 0.0f);

        u8 iA, nAct, nRest, iB, k, w, j;
        CMenuUpdate_8014274C act[8]; // sp+0xa8
        __ct__8014274C(&act[0]);
        __ct__8014274C(&act[1]);
        __ct__8014274C(&act[2]);
        __ct__8014274C(&act[3]);
        __ct__8014274C(&act[4]);
        __ct__8014274C(&act[5]);
        __ct__8014274C(&act[6]);
        __ct__8014274C(&act[7]);
        nAct = 0;
        iA = 0;
        for (; iA < 8; iA++) {
            s32 f = obj->mEntries[iA].field_0;
            if (f != 4 && f != 5) break;
            MenuUpdate_CopyQuadWord((u32*)&act[nAct++], (u32*)&obj->mEntries[iA].field_0);
        }

        CMenuUpdate_8014274C rest[8]; // sp+0x28
        __ct__8014274C(&rest[0]);
        __ct__8014274C(&rest[1]);
        __ct__8014274C(&rest[2]);
        __ct__8014274C(&rest[3]);
        __ct__8014274C(&rest[4]);
        __ct__8014274C(&rest[5]);
        __ct__8014274C(&rest[6]);
        __ct__8014274C(&rest[7]);
        iB = nAct;
        nRest = 0;
        while (iB < 8) {
            if (obj->mEntries[iB].field_0 == 0) break;
            MenuUpdate_CopyQuadWord((u32*)&rest[nRest++], (u32*)&obj->mEntries[iB].field_0);
            iB++;
        }

        for (k = 0; k < 8; k++) {
            CMenuUpdate_8014274C clr;
            MenuUpdate_CopyQuadWord((u32*)&obj->mEntries[k].field_0,
                          (u32*)__ct__8014274C(&clr));
        }

        w = 0;
        for (j = 0; j < nAct; j++) {
            MenuUpdate_CopyQuadWord((u32*)&obj->mEntries[w++].field_0, (u32*)&act[j].field_0);
        }
        // Re-file the saved header block right after the active entries.
        MenuUpdate_CopyQuadWord((u32*)&obj->mEntries[w++].field_0, (u32*)&obj->mState);
        for (j = 0; j < nRest; j++) {
            MenuUpdate_CopyQuadWord((u32*)&obj->mEntries[w++].field_0, (u32*)&rest[j].field_0);
        }

        CMenuUpdate_8014274C clrState;
        MenuUpdate_CopyQuadWord((u32*)&obj->mState, (u32*)__ct__8014274C(&clrState));
    } else {
        // Confirm-button handling on the state-machine side.
        CMenuUpdatePadData* pad = (CMenuUpdatePadData*)cf::CfGameManager::getCurrentPad();
        if (pad->mPressedButtonFlags & 0x200) {
            if (code80135FDC_getByte_64059() || CUICfManager_hasInUseSlot() || hasQuestWindow() ||
                QstCnt_HasInstance_6B94()) {
                playUISound(5);
            } else {
                CfRes_getE14();
                f32 v = MenuUpdate_QueryUnk80Float();
                // Retail tests each failure condition with a cror merge
                // ((v >= C4), (mFloat12C <= 0)) and exits to the common tail
                // when either negated test holds.
                if (!(v >= 1.0f) || !(obj->mFloat12C <= 0.0f)) {
                } else {
                    switch (obj->mState) {
                    case 1: CUICfManager_queueQuestLogMenu(obj->mSubState & 0xFFFF); break;
                    case 2: CUICfManager_queueKizunagramMenu(0); break;
                    case 6: UIWin_CreateSysWinBuff(obj->mSubState & 0xFFFF); break;
                    case 3: CUICfManager_queueTutorialListMenu(obj->mSubState & 0xFFFF); break;
                    }
                }
            }
        }
        if (advanceAnimTransform(obj->mActiveAnim, 1.0f))
            obj->mMode = 2;
    }
}

// MenuUpdate_QueryUnk80Float
extern "C" f32 MenuUpdate_QueryUnk80Float() {
    Unk_8049603C* mgr = (Unk_8049603C*)Scn_QueryUnk80State();
    f32 val = mgr->field_C;
    return 1.0f - val;
}

// MenuUpdate_GuardFirstEntry
// MenuUpdate_GuardFirstEntry - idle-state handler (mode 2): closes the front window and
// compacts the remaining non-empty entries toward the front.
extern "C" void MenuUpdate_GuardFirstEntry(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    if (isGlobalCamFlagSet(0x1000000) && obj->mEntries[0].field_0 != 4 &&
        obj->mEntries[0].field_0 != 5)
        return;
    if (CUICfManager_hasInUseSlot() || hasQuestWindow() || QstCnt_HasInstance_6B94())
        return;

    if (obj->mEntries[0].field_0 == 0) {
        setField64((u8*)self, 1);
        return;
    }

    obj->mFloat128 = 0.0f;
    obj->mMode = 0;
    // Stash the closing entry in the header block, then blank slot 0.
    MenuUpdate_CopyQuadWord((u32*)&obj->mState, (u32*)&obj->mEntries[0].field_0);
    {
        CMenuUpdate_8014274C clr;
        MenuUpdate_CopyQuadWord((u32*)&obj->mEntries[0].field_0, (u32*)__ct__8014274C(&clr));
    }
    MenuUpdate_RebindLayout(self);

    // Compact entries[1..7] down over the freed front slots.
    CMenuUpdate_8014274C tmp[8]; // sp+0x18
    __ct__8014274C(&tmp[0]);
    __ct__8014274C(&tmp[1]);
    __ct__8014274C(&tmp[2]);
    __ct__8014274C(&tmp[3]);
    __ct__8014274C(&tmp[4]);
    __ct__8014274C(&tmp[5]);
    __ct__8014274C(&tmp[6]);
    __ct__8014274C(&tmp[7]);
    u8 i, n;
    n = 0;
    i = 1;
    for (; i < 8; i++) {
        if (obj->mEntries[i].field_0 == 0) break;
        MenuUpdate_CopyQuadWord((u32*)&tmp[n++], (u32*)&obj->mEntries[i].field_0);
    }
    s32* dst = &obj->mEntries[0].field_0;
    s32* src = &tmp[0].field_0;
    for (int k = 0; k < 8; k++) {
        MenuUpdate_CopyQuadWord((u32*)dst, (u32*)src);
        src += 4;
        dst += 4;
    }
}

// This-adjusting thunks for the IWorkEvent (+0x6C) / IScnRender (+0x70)
// sub-vtables are compiler-emitted now that the bases are real classes.
