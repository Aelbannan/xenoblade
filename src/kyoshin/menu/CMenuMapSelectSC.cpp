// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelectSC
// Replace stubs with high-level C/C++ during decomp.

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuMapSelectSC.hpp"

// Retail-unmangled callee names (US strips mangling for these func_ helpers);
// declared extern "C" so call sites emit the literal retail reloc names.
// Mirrors the CMenuOption/CMenuGCItem pattern for the same helpers. Return
// types are int (not u8) so callers compare with cmpwi directly (no byte
// mask), matching retail.
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void func_801C4654(CTitleAHelp* self, u32 arg);
extern "C" void setInfoText(CTitleAHelp* self, char* name);
extern "C" void func_801C46DC(CTitleAHelp* self, u32 arg);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" int CFade_IsReady(CFade* self);
extern "C" int CFade_IsVisible(CFade* self);
// (memberized: CFade_FadeIn -> CFade::In; declaration moved into class CFade)
extern "C" void CFade_FadeOut(CFade* self);
extern "C" void CFade_StartLoad(CFade* self);
extern "C" void CFade_Update(CFade* self);
extern "C" void FloorMap_Teardown(CFloorMap* self);
extern "C" u16 FloorMap_GetU16Field5A(CFloorMap* self);
extern "C" int FloorMap_GetField42Gated(CFloorMap* self);
extern "C" void FloorMap_LoadStageLayout(CFloorMap* self);
extern "C" void FloorMap_UpdateFrame(CFloorMap* self);
extern "C" int FloorMap_ClassifyByte41(CFloorMap* self);
extern "C" int FloorMap_CheckRowVisited(CFloorMap* self);
extern "C" u8 FloorMap_GetBdatMaxRow(CFloorMap* self);
extern "C" u8 FloorMap_GetField208Plain(CFloorMap* self);
extern "C" void FloorMap_SetPaneFlag(CFloorMap* self, u32 arg);
extern "C" u8 FloorMap_GetField208Gated(CFloorMap* self);
extern "C" int FloorMap_IsSysWinBusy(CFloorMap* self);
extern "C" void FloorMap_UpdateRowPanePos(CFloorMap* self);
extern "C" void FloorMap_CursorNextFloor(CFloorMap* self);
extern "C" void FloorMap_OpenRowWindow(CFloorMap* self);
extern "C" void FloorMap_ConfirmSelect(CFloorMap* self);
extern "C" void FloorMap_UpdateCursorMove(CFloorMap* self);
extern "C" void FloorMap_UpdateWorldMap(CFloorMap* self);
extern "C" void FloorMap_HandleMapDown(CFloorMap* self);
extern "C" void FloorMap_NormalizeMapPos(CFloorMap* self);
extern "C" void FloorMap_Log2Field208(CFloorMap* self);
extern "C" void FloorMap_Log2Field333C(CFloorMap* self);
extern "C" void FloorMap_ScrollDown(CFloorMap* self);
extern "C" void FloorMap_ScrollUp(CFloorMap* self);
extern "C" void FloorMap_ScrollRight(CFloorMap* self);
extern "C" void FloorMap_ScrollLeft(CFloorMap* self);
extern "C" void FloorMap_MoveToCursorRow(CFloorMap* self);

// Retail symbol keeps its mangled form (CfGameManager static member); the
// call site passes (u16 value, 0) even though the name says Fv.
extern "C" void func_8008413C__Q22cf13CfGameManagerFv(u16 a, u32 b);



// World-map phase helpers (retail-unmangled callee names).
extern "C" void MapSelectSCResetFloorMapIdle(CMenuMapSelectSC* self);
extern "C" void MapSelectSCCommitCursorIdle(CMenuMapSelectSC* self);
extern "C" void MapSelectSCBeginFadeIn(CMenuMapSelectSC* self);
extern "C" void MapSelectSCPrepFloorMap(CMenuMapSelectSC* self);
extern "C" void MapSelectSCShowMapText(CMenuMapSelectSC* self);
extern "C" void MapSelectSCHandleInput(CMenuMapSelectSC* self);

// ---------------------------------------------------------------------------
// Additional retail-unmangled callee names for the world-map (SC) screen.
// ---------------------------------------------------------------------------
// CScn-side helpers: scene enable flag + render-callback management.
// int return (mismatched vs the (void*, u8) definition) blocks -ipa inlining
// of the 1-line body, keeping the retail `bl Scn_SetPauseFlag` call site.
extern "C" int Scn_SetPauseFlag(CScn* scn, int flag);
// CFloorMap helpers (retail unmangles these member helpers).
// (memberized: FloorMap_SetMapTable -> CFloorMap::SetMapTable; declaration moved into class CFloorMap)
extern "C" void FloorMap_LoadMapFiles(CFloorMap* self);
extern "C" void FloorMap_Draw(CFloorMap* self, nw4r::lyt::DrawInfo* drawInfo);
// CTitleAHelp / CFade draw + reset helpers.
extern "C" void teardown(CTitleAHelp* self);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void CFade_Draw(CFade* self, nw4r::lyt::DrawInfo* drawInfo);
// (memberized: CFade_Unload -> CFade::Unload; declaration moved into class CFade)
// UI/sound + scene-active helpers.
extern "C" void UIWin_ControlWindowLists(u32 a, u32 b);
extern "C" int IsMenuState621F0();
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);
// Ctor/re-init helpers for the embedded widgets (retail-unmangled).
extern "C" void __ct__11CTitleAHelpFPcUc(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __ct__CFloorMap(CFloorMap* self);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dest, void* src);
extern "C" void InfoCfCopySysWin(void* dest, void* src);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" char* BdatTouchStringCell(char* a, char* b, u32 id);
extern "C" void __dt__9CFloorMapFv(CFloorMap* _this, int flags);
extern "C" void __dt__5CFadeFv(CFade* _this, int flags);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* _this, int flags);

// C++-linkage helpers - retail emits the mangled forms
// playUISound__FUl / SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo.
void playUISound(unsigned long op);
void SetWideLocAdjust(nw4r::lyt::DrawInfo* pDrawInfo);

// Data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_80664184;   // current map id (bdat row) -> floor map select
extern u32 lbl_eu_80663E28;   // global mode bitfield (bit 0x400 = world-map gate)
extern u8 lbl_eu_80537130[];  // CMenuMapSelectSC final vtable; IScnRender vtable at +0x24
extern u32 __ptmf_null[3];    // null pointer-to-member-function constant
extern char lbl_eu_8050C5A4[];  // world-map title/help string table base
extern const f32 lbl_eu_806687C0;  // 0.0f - world-map timer reset value
extern const f32 lbl_eu_806687C4;  // 1.0f - world-map timer increment per frame
extern const f32 lbl_eu_806687C8;  // 10.0f - world-map timer cap

#include "kyoshin/CTaskGame.hpp"

#include <new>
#include <cstring>
#include <revolution/gx/GXPixel.h>
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/CFloorMapImports.hpp"

struct S {
    int dummy;
    int a;
    int b;
    int c;
    int d;
    unsigned char e;
    unsigned char f;
};

extern "C" void MapSelectSCCopyWidgetState(S* dst, const S* src);

// Out-of-line constructor (retail keeps it as its own function; the member
// initialisers run in declaration order: parent ref, CTitleAHelp, CFade,
// CFloorMap, then the state byte and timer).
CMenuMapSelectSC::CMenuMapSelectSC(CProcess* parent)
    : mParentRef(parent), mTitleAHelp(0, 0), mState(0), mTimer(lbl_eu_806687C0) {}

CMenuMapSelectSC::~CMenuMapSelectSC() {}

void CMenuMapSelectSC::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the embedded CTitleAHelp via a temporary ---
    char* name = BdatTouchStringCell(lbl_eu_8050C5A4, lbl_eu_8050C5A4 + 8, 1);

    u8 tempTitle[0x38];
    __ct__11CTitleAHelpFPcUc(reinterpret_cast<CTitleAHelp*>(tempTitle), name, 0x4a);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0x64, tempTitle + 0x4);
    CTitleAHelp* tmpTitle = reinterpret_cast<CTitleAHelp*>(tempTitle);
    mTitleAHelp.mFileHandle = tmpTitle->mFileHandle;
    mTitleAHelp.mArcResourceAccessor = tmpTitle->mArcResourceAccessor;
    mTitleAHelp.mLayout = tmpTitle->mLayout;
    mTitleAHelp.mAnimTrans20 = tmpTitle->mAnimTrans20;
    mTitleAHelp.mAnimTrans24 = tmpTitle->mAnimTrans24;
    mTitleAHelp.unk28 = tmpTitle->unk28;
    mTitleAHelp.unk2c = tmpTitle->unk2c;
    mTitleAHelp.mName = tmpTitle->mName;
    mTitleAHelp.unk34 = tmpTitle->unk34;
    mTitleAHelp.unk35 = tmpTitle->unk35;
    mTitleAHelp.unk36 = tmpTitle->unk36;
    mTitleAHelp.unk37 = tmpTitle->unk37;
    __dt__11CTitleAHelpFv(tmpTitle, -1);

    mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded CFade via a temporary ---
    {
        CFade tmpFade;
        __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0x9c, (u8*)&tmpFade + 0x4);
        mFade.mFileHandle = tmpFade.mFileHandle;
        mFade.mArcResAcc = tmpFade.mArcResAcc;
        mFade.mLayout = tmpFade.mLayout;
        mFade.mAnimTrans = tmpFade.mAnimTrans;
        mFade.mIsLoaded = tmpFade.mIsLoaded;
        mFade.mFadeState = tmpFade.mFadeState;
        mFade.mReady = tmpFade.mReady;
        mFade.mVisible = tmpFade.mVisible;
    }

    CFade_StartLoad(&mFade);

    // --- Re-initialise the embedded CFloorMap via a temporary ---
    u8 tempFloor[0x3348];
    __ct__CFloorMap(reinterpret_cast<CFloorMap*>(tempFloor));

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0xc4, tempFloor + 0x4);
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0xd4, tempFloor + 0x14);

    // File handles / buffers / visibility flags (member 0x24-0x5D).
    *(u32*)((u8*)this + 0xe4) = *(u32*)(tempFloor + 0x24);
    *(u32*)((u8*)this + 0xe8) = *(u32*)(tempFloor + 0x28);
    *(u32*)((u8*)this + 0xec) = *(u32*)(tempFloor + 0x2c);
    *(u32*)((u8*)this + 0xf0) = *(u32*)(tempFloor + 0x30);
    *(u32*)((u8*)this + 0xf4) = *(u32*)(tempFloor + 0x34);
    *(u32*)((u8*)this + 0xf8) = *(u32*)(tempFloor + 0x38);
    *(u32*)((u8*)this + 0xfc) = *(u32*)(tempFloor + 0x3c);
    *(u8*)((u8*)this + 0x100) = *(u8*)(tempFloor + 0x40);
    *(u8*)((u8*)this + 0x101) = *(u8*)(tempFloor + 0x41);
    *(u8*)((u8*)this + 0x102) = *(u8*)(tempFloor + 0x42);
    *(u8*)((u8*)this + 0x103) = *(u8*)(tempFloor + 0x43);
    *(f32*)((u8*)this + 0x104) = *(f32*)(tempFloor + 0x44);
    *(f32*)((u8*)this + 0x108) = *(f32*)(tempFloor + 0x48);
    *(f32*)((u8*)this + 0x10c) = *(f32*)(tempFloor + 0x4c);
    *(f32*)((u8*)this + 0x110) = *(f32*)(tempFloor + 0x50);
    *(f32*)((u8*)this + 0x114) = *(f32*)(tempFloor + 0x54);
    *(u8*)((u8*)this + 0x118) = *(u8*)(tempFloor + 0x58);
    *(u16*)((u8*)this + 0x11a) = *(u16*)(tempFloor + 0x5a);
    *(u8*)((u8*)this + 0x11c) = *(u8*)(tempFloor + 0x5c);
    *(u8*)((u8*)this + 0x11d) = *(u8*)(tempFloor + 0x5d);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0x124, tempFloor + 0x64);

    // Cursor / scroll state (member 0x74-0xB5).
    *(u32*)((u8*)this + 0x134) = *(u32*)(tempFloor + 0x74);
    *(u32*)((u8*)this + 0x138) = *(u32*)(tempFloor + 0x78);
    *(u32*)((u8*)this + 0x13c) = *(u32*)(tempFloor + 0x7c);
    *(u32*)((u8*)this + 0x140) = *(u32*)(tempFloor + 0x80);
    *(u8*)((u8*)this + 0x144) = *(u8*)(tempFloor + 0x84);
    *(u8*)((u8*)this + 0x145) = *(u8*)(tempFloor + 0x85);
    *(u8*)((u8*)this + 0x146) = *(u8*)(tempFloor + 0x86);
    *(u8*)((u8*)this + 0x147) = *(u8*)(tempFloor + 0x87);
    *(f32*)((u8*)this + 0x148) = *(f32*)(tempFloor + 0x88);
    *(f32*)((u8*)this + 0x14c) = *(f32*)(tempFloor + 0x8c);
    *(f32*)((u8*)this + 0x150) = *(f32*)(tempFloor + 0x90);
    *(f32*)((u8*)this + 0x154) = *(f32*)(tempFloor + 0x94);
    *(f32*)((u8*)this + 0x158) = *(f32*)(tempFloor + 0x98);
    *(u8*)((u8*)this + 0x15c) = *(u8*)(tempFloor + 0x9c);
    *(u32*)((u8*)this + 0x164) = *(u32*)(tempFloor + 0xa4);
    *(u32*)((u8*)this + 0x168) = *(u32*)(tempFloor + 0xa8);
    *(u32*)((u8*)this + 0x16c) = *(u32*)(tempFloor + 0xac);
    *(u32*)((u8*)this + 0x170) = *(u32*)(tempFloor + 0xb0);
    *(u8*)((u8*)this + 0x174) = *(u8*)(tempFloor + 0xb4);
    *(u8*)((u8*)this + 0x175) = *(u8*)(tempFloor + 0xb5);

    InfoCfCopySysWin((u8*)this + 0x178, tempFloor + 0xb8);

    // Layout pointer array (member 0x130-0x14C) + 20 8-byte entries.
    *(u32*)((u8*)this + 0x1f0) = *(u32*)(tempFloor + 0x130);
    *(u32*)((u8*)this + 0x1f4) = *(u32*)(tempFloor + 0x134);
    *(u32*)((u8*)this + 0x1f8) = *(u32*)(tempFloor + 0x138);
    *(u32*)((u8*)this + 0x1fc) = *(u32*)(tempFloor + 0x13c);
    *(u32*)((u8*)this + 0x200) = *(u32*)(tempFloor + 0x140);
    *(u32*)((u8*)this + 0x204) = *(u32*)(tempFloor + 0x144);
    *(u32*)((u8*)this + 0x208) = *(u32*)(tempFloor + 0x148);
    *(u8*)((u8*)this + 0x20c) = *(u8*)(tempFloor + 0x14c);
    // 20 8-byte entries, copied as a mtctr/bdnz loop in retail.
    for (int i = 0; i < 0x14; i++) {
        *(u32*)((u8*)this + 0x210 + i * 8) = *(u32*)(tempFloor + 0x150 + i * 8);
        *(u32*)((u8*)this + 0x214 + i * 8) = *(u32*)(tempFloor + 0x154 + i * 8);
    }

    // Entry count / draw flags (member 0x1F0-0x208).
    *(u8*)((u8*)this + 0x2b0) = *(u8*)(tempFloor + 0x1f0);
    *(u32*)((u8*)this + 0x2b4) = *(u32*)(tempFloor + 0x1f4);
    *(u8*)((u8*)this + 0x2b8) = *(u8*)(tempFloor + 0x1f8);
    *(u32*)((u8*)this + 0x2bc) = *(u32*)(tempFloor + 0x1fc);
    *(u32*)((u8*)this + 0x2c0) = *(u32*)(tempFloor + 0x200);
    *(u8*)((u8*)this + 0x2c4) = *(u8*)(tempFloor + 0x204);
    *(u8*)((u8*)this + 0x2c5) = *(u8*)(tempFloor + 0x205);
    *(u8*)((u8*)this + 0x2c6) = *(u8*)(tempFloor + 0x206);
    *(u8*)((u8*)this + 0x2c7) = *(u8*)(tempFloor + 0x207);
    *(u8*)((u8*)this + 0x2c8) = *(u8*)(tempFloor + 0x208);

    // Map-data blob (member 0x20C, 0x30C0 bytes): 0x618 8-byte entries.
    for (int j = 0; j < 0x618; j++) {
        *(u32*)((u8*)this + 0x2cc + j * 8) = *(u32*)(tempFloor + 0x20c + j * 8);
        *(u32*)((u8*)this + 0x2d0 + j * 8) = *(u32*)(tempFloor + 0x210 + j * 8);
    }

    *(u8*)((u8*)this + 0x338c) = *(u8*)(tempFloor + 0x32cc);

    // Four 0x18-byte sub-object copies.
    MapSelectSCCopyWidgetState((S*)((u8*)this + 0x3390), (const S*)(tempFloor + 0x3338));
    MapSelectSCCopyWidgetState((S*)((u8*)this + 0x33a8), (const S*)(tempFloor + 0x3350));
    MapSelectSCCopyWidgetState((S*)((u8*)this + 0x33c0), (const S*)(tempFloor + 0x3368));
    MapSelectSCCopyWidgetState((S*)((u8*)this + 0x33d8), (const S*)(tempFloor + 0x3380));

    // Tail fields (member 0x3330-0x344).
    *(u32*)((u8*)this + 0x33f0) = *(u32*)(tempFloor + 0x3330);
    *(u32*)((u8*)this + 0x33f4) = *(u32*)(tempFloor + 0x3334);
    *(u32*)((u8*)this + 0x33f8) = *(u32*)(tempFloor + 0x3338);
    *(u8*)((u8*)this + 0x33fc) = *(u8*)(tempFloor + 0x333c);
    *(u32*)((u8*)this + 0x3400) = *(u32*)(tempFloor + 0x3340);
    *(u32*)((u8*)this + 0x3404) = *(u32*)(tempFloor + 0x3344);

    __dt__9CFloorMapFv(reinterpret_cast<CFloorMap*>(tempFloor), -1);

    // Register the widget as an IScnRender render callback on its parent scene.
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(this, 0xd, 1);
}

extern "C" void MapSelectSCCopyWidgetState(S* dst, const S* src) {
    // Retail copies only +4..+15 (four words) and +20/+21 (two bytes).
    // The leading dummy word is padding so field offsets match; it is
    // never stored. No extra guard: that emitted a 2-insn sdata preamble.
    dst->a = src->a;
    dst->b = src->b;
    dst->c = src->c;
    dst->d = src->d;
    dst->e = src->e;
    dst->f = src->f;
}

void CMenuMapSelectSC::Term() {
    CDeviceVI::waitForDrawDone();
    Scn_SetPauseFlag(reinterpret_cast<CScn*>(mParentRef), 1);

    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(this);

    teardown(&mTitleAHelp);
    (&mFade)->Unload();
    FloorMap_Teardown(&mFloorMap);

    sInstance = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuMapSelectSC::Move() {
    CTaskGame::getInstance();
    // Same gate idiom as cbRenderBefore (see comment there).
    if (CTaskGame::isFlag01Set() == 0 &&
        (lbl_eu_80663E28 & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    switch (mState) {
    case 0:
        MapSelectSCBeginFadeIn(this);
        break;
    case 1:
        MapSelectSCPrepFloorMap(this);
        break;
    case 2:
        MapSelectSCShowMapText(this);
        break;
    case 3:
        if (CFade_IsVisible(&mFade) != 0) {
            mState = 4;
        }
        break;
    case 4:
        MapSelectSCHandleInput(this);
        break;
    case 5:
        MapSelectSCResetFloorMapIdle(this);
        break;
    case 6:
        MapSelectSCCommitCursorIdle(this);
        break;
    case 7:
        mField54 = 1;
        break;
    }
    updateHelp(&mTitleAHelp);
    CFade_Update(&mFade);
    FloorMap_UpdateFrame(&mFloorMap);
}

void CMenuMapSelectSC::cbRenderBefore() {
    CTaskGame::getInstance();
    // Gate: skip when the task is busy or the global mode bit (0x200000) is
    // set. The combined `A == 0 && B == 0 -> goto body` chain with the exit
    // label BEFORE the body label reproduces retail's branch-over-branch
    // (`bne end`, then `beq body; b end`).
    if (CTaskGame::isFlag01Set() == 0 &&
        (lbl_eu_80663E28 & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    if (IsMenuState621F0() == 0) {
        goto end;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    FloorMap_Draw(&mFloorMap, &drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
    CFade_Draw(&mFade, &drawInfo);
}

CMenuMapSelectSC* CMenuMapSelectSC::sInstance;

// Create the world-map (SC) singleton: returns 0 when it already exists,
// otherwise allocates the object (0x3410 bytes), constructs it in place, stores
// the singleton and registers it as a CProcess child of `parent`.
extern "C" CMenuMapSelectSC* MapSelectSCCreate(CProcess* parent, CProcess* arg) {
    if (CMenuMapSelectSC::sInstance != 0) {
        return 0;
    }

    playUISound(4);
    CMenuMapSelectSC* obj = (CMenuMapSelectSC*)mtl::MemManager::allocate(
        0x3410, CWorkThreadSystem::getWorkMem());
    obj = new (obj) CMenuMapSelectSC(arg);
    CMenuMapSelectSC::sInstance = obj;
    obj->Regist(parent, false);
    return CMenuMapSelectSC::sInstance;
}

// (sInstance != 0)
extern "C" bool MapSelectSCIsCreated() { return CMenuMapSelectSC::sInstance != 0; }

// World-map phase 0: wait for the title/help bar and fade to be ready, then
// start the fade-in animation and advance to phase 1.
extern "C" void MapSelectSCBeginFadeIn(CMenuMapSelectSC* self) {
    if (isInitialized(&self->mTitleAHelp) && CFade_IsReady(&self->mFade)) {
        (&self->mFade)->In();
        self->mState = 1;
    }
}

// World-map phase 2: once the fade has finished hiding, prepare the floor map
// for the current map id, start its file loads, advance to phase 3 and play
// the confirm sound.
extern "C" void MapSelectSCPrepFloorMap(CMenuMapSelectSC* self) {
    if (CFade_IsVisible(&self->mFade) != 0) {
        Scn_SetPauseFlag(reinterpret_cast<CScn*>(self->mParentRef), 0);
        (&self->mFloorMap)->SetMapTable((u8)lbl_eu_80664184);
        FloorMap_LoadMapFiles(&self->mFloorMap);
        self->mState = 2;
        UIWin_ControlWindowLists(1, 0);
    }
}

// World-map phase 2: once the scrollbar is visible, populate the title/help
// bar text for the current map, start its animation and advance to phase 3.
extern "C" void MapSelectSCShowMapText(CMenuMapSelectSC* self) {
    if (FloorMap_GetField42Gated(&self->mFloorMap) != 0) {
        func_801C4654(&self->mTitleAHelp, 0);
        char* name = BdatTouchStringCell(lbl_eu_8050C5A4, lbl_eu_8050C5A4 + 8, 3);
        setInfoText(&self->mTitleAHelp, name);
        if (CtrlRemote_TouchBitByArg(0x3212) != 0) {
            func_801C46DC(&self->mTitleAHelp, 1);
        }
        func_801C41E8(&self->mTitleAHelp, 0);
        func_801C412C(&self->mTitleAHelp);
        FloorMap_LoadStageLayout(&self->mFloorMap);
        playUISound(0x6d);
        CFade_FadeOut(&self->mFade);
        self->mState = 3;
    }
}

// World-map phase 4: floor-map input handler. Accumulates a per-frame timer,
// then dispatches the current pad state to the cursor/selection helpers. The
// pad layout differs between Classic (bits 21-23/25-26) and Wiimote/Nunchuk
// (bits 4-5/10/12) controllers.
extern "C" void MapSelectSCHandleInput(CMenuMapSelectSC* self) {
    if (FloorMap_ClassifyByte41(&self->mFloorMap) != 0) {
        return;
    }

    // Explicit intermediate store keeps the unconditional stfs of the
    // incremented value ahead of the clamp compare (retail order:
    // fadds; stfs; fcmpo; ble; stfs).
    f32 timerCap = lbl_eu_806687C8;
    f32 t = self->mTimer + lbl_eu_806687C4;
    self->mTimer = t;
    if (t > timerCap) {
        self->mTimer = timerCap;
    }

    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Classic controller layout.
        if (FloorMap_CheckRowVisited(&self->mFloorMap) != 0) {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4b);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4c);
            }
        } else {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4d);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4e);
            }
        }
        FloorMap_SetPaneFlag(&self->mFloorMap, 0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        if ((pad->mTurboPressButtonFlags & 0x02000000) != 0 &&
            FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 &&
            FloorMap_GetField208Gated(&self->mFloorMap) != 0 &&
            FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
            FloorMap_UpdateRowPanePos(&self->mFloorMap);
            return;
        }
        if ((pad->mTurboPressButtonFlags & 0x04000000) != 0 &&
            FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 &&
            FloorMap_GetField208Gated(&self->mFloorMap) != 0 &&
            FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
            FloorMap_CursorNextFloor(&self->mFloorMap);
            return;
        }

        u32 pressed = pad->mPressedButtonFlags;
        if ((pressed & 0x00200000) != 0) {
            FloorMap_OpenRowWindow(&self->mFloorMap);
            if (FloorMap_GetU16Field5A(&self->mFloorMap) != 0) {
                (&self->mFade)->In();
                self->mState = 6;
            }
        } else if ((pressed & 0x00400000) != 0) {
            if (self->mFloorMap.GetActiveFlag58() != 0) {
                FloorMap_ConfirmSelect(&self->mFloorMap);
            } else if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                (&self->mFade)->In();
                self->mState = 5;
                playUISound(6);
            } else {
                FloorMap_OpenRowWindow(&self->mFloorMap);
            }
        } else if ((pressed & 0x00800000) != 0) {
            if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                (&self->mFade)->In();
                self->mState = 5;
                playUISound(6);
            }
        } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
            FloorMap_UpdateCursorMove(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
            FloorMap_UpdateWorldMap(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
            FloorMap_HandleMapDown(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
            FloorMap_NormalizeMapPos(&self->mFloorMap);
        } else if ((pressed & 0x01000000) != 0) {
            FloorMap_Log2Field208(&self->mFloorMap);
        } else if ((pressed & 0x200) != 0) {
            FloorMap_Log2Field333C(&self->mFloorMap);
        } else {
            // D-pad held: each direction moves the cursor once per press
            // transition; when no direction is held, confirm the selection.
            u32 flag = 1;
            if ((pad->mHeldButtonFlags & 0x8000) != 0) {
                FloorMap_ScrollDown(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x10000) != 0) {
                FloorMap_ScrollUp(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x2000) != 0) {
                FloorMap_ScrollRight(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x4000) != 0) {
                FloorMap_ScrollLeft(&self->mFloorMap);
                flag = 0;
            }
            if (flag != 0) {
                FloorMap_MoveToCursorRow(&self->mFloorMap);
            }
        }
    } else {
        // Wiimote/Nunchuk layout.
        if (FloorMap_CheckRowVisited(&self->mFloorMap) != 0) {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4b);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4c);
            }
        } else {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4d);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4e);
            }
        }
        FloorMap_SetPaneFlag(&self->mFloorMap, 0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        if ((pad->mHeldButtonFlags & 0x1000) != 0 &&
            FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 &&
            FloorMap_GetField208Gated(&self->mFloorMap) != 0 &&
            FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
            func_801C41E8(&self->mTitleAHelp, 0x4f);
            if (self->mTimer > lbl_eu_806687C4) {
                playUISound(2);
            }
            self->mTimer = lbl_eu_806687C0;
            FloorMap_SetPaneFlag(&self->mFloorMap, 1);
            if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
                FloorMap_UpdateRowPanePos(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
                FloorMap_CursorNextFloor(&self->mFloorMap);
            }
        } else {
            u32 pressed = pad->mPressedButtonFlags;
            if ((pressed & 0x10) != 0) {
                FloorMap_OpenRowWindow(&self->mFloorMap);
                if (FloorMap_GetU16Field5A(&self->mFloorMap) != 0) {
                    (&self->mFade)->In();
                    self->mState = 6;
                }
            } else if ((pressed & 0x20) != 0) {
                if (self->mFloorMap.GetActiveFlag58() != 0) {
                    FloorMap_ConfirmSelect(&self->mFloorMap);
                } else if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                    (&self->mFade)->In();
                    self->mState = 5;
                    playUISound(6);
                } else {
                    FloorMap_OpenRowWindow(&self->mFloorMap);
                }
            } else if ((pressed & 0x400) != 0) {
                if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                    (&self->mFade)->In();
                    self->mState = 5;
                    playUISound(6);
                }
            } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
                FloorMap_UpdateCursorMove(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
                FloorMap_UpdateWorldMap(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
                FloorMap_HandleMapDown(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
                FloorMap_NormalizeMapPos(&self->mFloorMap);
            } else if ((pressed & 0x800) != 0) {
                FloorMap_Log2Field208(&self->mFloorMap);
            } else if ((pressed & 0x200) != 0) {
                FloorMap_Log2Field333C(&self->mFloorMap);
            } else {
                u32 flag = 1;
                if ((pad->mHeldButtonFlags & 0x8000) != 0) {
                    FloorMap_ScrollDown(&self->mFloorMap);
                    flag = 0;
                }
                if ((pad->mHeldButtonFlags & 0x10000) != 0) {
                    FloorMap_ScrollUp(&self->mFloorMap);
                    flag = 0;
                }
                if ((pad->mHeldButtonFlags & 0x2000) != 0) {
                    FloorMap_ScrollRight(&self->mFloorMap);
                    flag = 0;
                }
                if ((pad->mHeldButtonFlags & 0x4000) != 0) {
                    FloorMap_ScrollLeft(&self->mFloorMap);
                    flag = 0;
                }
                if (flag != 0) {
                    FloorMap_MoveToCursorRow(&self->mFloorMap);
                }
            }
        }
    }
}

// World-map phase 6: wait for the fade to finish hiding, then reset the
// floor map and move to phase 7 (idle).
extern "C" void MapSelectSCResetFloorMapIdle(CMenuMapSelectSC* self) {
    if (CFade_IsVisible(&self->mFade)) {
        FloorMap_Teardown(&self->mFloorMap);
        self->mState = 7;
    }
}

// World-map phase 6 variant: after the fade completes, hand the selected
// floor-map cursor index to the game manager and move to phase 7 (idle).
extern "C" void MapSelectSCCommitCursorIdle(CMenuMapSelectSC* self) {
    if (CFade_IsVisible(&self->mFade)) {
        func_8008413C__Q22cf13CfGameManagerFv(FloorMap_GetU16Field5A(&self->mFloorMap), 0);
        self->mState = 7;
    }
}
