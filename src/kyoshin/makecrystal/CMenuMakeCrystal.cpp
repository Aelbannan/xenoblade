#include "kyoshin/makecrystal/CMenuMakeCrystal.hpp"
#include "kyoshin/CMainMenu.hpp" // CMainMenu_GetInstancePtr
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CBgTex.hpp"
#include "revolution/gx/GXPixel.h"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/core/CPadManager.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// Render-gate mode bitfield (.sbss); bit 21 set means rendering is blocked.
extern u32 gmPauseFlags;

// Data block copied by MakeCrystalCopyParamBlock (fields +0x4..+0x14 only; +0x0 is not
// part of the copy). Two bytes, three words, one byte - meaning unknown.
struct CMakeCrystalCopyBlock {
    u32 field_0x0;   // +0x00 (not copied)
    u8  field_0x4;   // +0x04
    u8  field_0x5;   // +0x05
    u32 field_0x8;   // +0x08
    u32 field_0xC;   // +0x0C
    u32 field_0x10;  // +0x10
    u8  field_0x14;  // +0x14
};

// ---------------------------------------------------------------------------
// Retail-unmangled callees bound literally (US relocs carry stripped names;
// see CTaskGameEvt.cpp / CSysWinScenarioLog.cpp precedent).
// ---------------------------------------------------------------------------
extern "C" {
void waitForDrawDone__9CDeviceVIFv();
void CUICfManager_createQueuedMenus();
void BgTex_Draw_3D7C(void* self, nw4r::lyt::DrawInfo* di);   // CBgTex layout draw
void BgTex_Tick_3D54(void* self);                            // CBgTex animate

void mkCrystalTick(void* self);                            // CMakeCrystalWin tick
void ArtsInfo_SetReadyFlag();
void __ct__6CBgTexFUc(void* self, int arg);
void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* self, const void* src);
void Scn_SetPauseFlag(void*, int);
void BgTex_Release_3D9C(void*);
void mkCrystalTeardown(void*);
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
// Retail symbol keeps its Fv mangling even though it takes the pad index.
// Unmangled local helper (bound literally).
extern "C" void MakeCrystalInitWin(void* self);
void BgTex_Acquire_3C14(void* self);                            // CBgTex init
void MakeCrystalCopyDispArea(void* dst, const void* src);            // big state copy
void MakeCrystalCopyBox(void* dst, const void* src);                  // crystal-box state copy

// a real assignment inlines here until the sibling copies in Init are real
// assignments too (MWCC_CASES inlining Rule B).
extern "C" void __as__11CTitleAHelpFRC11CTitleAHelp(void* dst, void* src);
void MakeCrystalCopyStateRecord(void* dst, const void* src);            // state record copy
void addRenderCB__4CScnFP10IScnRenderUlUl(void* scn, void* render, u32 a, u32 b);
int  CtrlRemote_TouchBitByArg(int id);
void CCharVoiceMan_FlushAll();
void MenuSnd_StopAllSlots_89C88();
void CTaskGame_stopVision();
void* __dt__15CMakeCrystalWinFv(void*, int); // Init's stack temporary
void __dt__6CBgTexFv(void*, int);
void __ct__CMakeCrystalWin(void* self, u32 arg4, u32 arg5);
}

// Real C++ declarations from the owning CMakeCrystalWin.hpp.
u32 mkCrystalGetFlag4361(void* self);
u32 mkCrystalGetWord10(void* self);

// Defined in CMiniMap.cpp (C++ linkage, retail symbol MiniMapSetActiveFlag__Fv).
void MiniMapSetActiveFlag();

// Declared by kyoshin/CMainMenu.hpp.

// UI sound effect (playUISound__FUl).
// MakeCrystalInitWin helper: init the CBgTex sub-object and arm the state byte.
// (declared extern "C" above -- retail symbol is unmangled)

// Stack temporaries used by Init(). The retail frame is 0x43A0 bytes; these
// mirror the local objects at their frame offsets so the callee calls see the
// same sub-object layout.
// CBgTex-shaped temp (frame +0x08): UnkClass_8011C974 sub-object at +0x04,
// then three words and three bytes copied out to this+0x74..+0x7E.
struct MCBgTemp {
    u8 _00[0x04];       // +0x00
    u8 unk[0x10];       // +0x04: UnkClass_8011C974 sub-object
    u32 field_0x14;
    u32 field_0x18;
    u8 field_0x1C;
    u8 field_0x1D;
    u8 field_0x1E;
};
// CMakeCrystalWin-shaped temp (frame +0x28): four words at +0x08 copied to
// this+0x88..+0x94, remaining bytes consumed by ItemEx_CopyTitle.
struct MCWinTemp {
    u8 _00[0x08];
    u32 field_0x08;     // frame +0x30
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
    u8 tail[0x38];      // frame +0x40..+0x77
};
// Trailing state bytes of CMenuMakeCrystal written by Init.
struct MCTailState {
    u8 _00[0x74];
    u32 field_0x74;
    u32 field_0x78;
    u8 field_0x7C;
    u8 field_0x7D;
    u8 field_0x7E;
    u8 _7F[0x43E0 - 0x7F];
    u8 ready0;          // 0x43E0
    u8 ready1;          // 0x43E1
    u8 _e2[3];
    u8 gate;            // 0x43E5
};
// Bytes at frame +0x4388/+0x4389 inside the record temp.
struct MCRecTail {
    u8 _00[0x30];
    u8 b30;
    u8 b31;
};

// Builds the IUICf task base (CProcess + composite vtables + null PTMF callbacks
// are done by the compiler), stores the owning scene, constructs CBgTex and the
// raw CMakeCrystalWin block, then arms the state bytes.
CMenuMakeCrystal::CMenuMakeCrystal(CScn* scene) : mScene(scene), mBgTex(0) {
    __ct__CMakeCrystalWin(&mCrystalWin, 0, 0);
    mField43E4 = 0;
    mField43E5 = 1;
}

CMenuMakeCrystal::~CMenuMakeCrystal() {}

void CMenuMakeCrystal::Init() {
    CCharVoiceMan_FlushAll();
    MenuSnd_StopAllSlots_89C88();
    CTaskGame_stopVision();
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // Build a temporary CBgTex, clone its UnkClass sub-object into this+0x64
    // plus its scalar fields into this+0x74..+0x7E, then destroy the temp.
    MCBgTemp bg;
    __ct__6CBgTexFUc(&bg, 0);
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(reinterpret_cast<u8*>(this) + 0x64, bg.unk);
    reinterpret_cast<MCTailState*>(this)->field_0x74 = bg.field_0x14;
    reinterpret_cast<MCTailState*>(this)->field_0x78 = bg.field_0x18;
    reinterpret_cast<MCTailState*>(this)->field_0x7C = bg.field_0x1C;
    reinterpret_cast<MCTailState*>(this)->field_0x7D = bg.field_0x1D;
    reinterpret_cast<MCTailState*>(this)->field_0x7E = bg.field_0x1E;
    __dt__6CBgTexFv(&bg, -1);

    BgTex_Acquire_3C14(reinterpret_cast<u8*>(this) + 0x60);

    // Build a temporary CMakeCrystalWin seeded from this object, harvest its
    // four words into this+0x88..+0x94, hand the rest to ItemEx_CopyTitle, then
    // destroy the temp.
    MCWinTemp win;
    __ct__CMakeCrystalWin(
        &win, reinterpret_cast<u32>(this),
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x5C));
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x88) = win.field_0x08;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x8C) = win.field_0x0C;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x90) = win.field_0x10;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x94) = win.field_0x14;

    // Large uninitialized stack buffers copied wholesale into the window state
    // (retail copies them before any initialization). Kept as one aggregate so
    // the stack slots stay sequential after the win temp.
    struct {
        u8 mid[0x2DD8];
        u8 upper[0x1508];
        u8 rec[0x32];
    } bufs;

    __as__11CTitleAHelpFRC11CTitleAHelp(reinterpret_cast<u8*>(this) + 0x98, win.tail);
    MakeCrystalCopyDispArea(reinterpret_cast<u8*>(this) + 0xD0, bufs.mid);
    MakeCrystalCopyBox(reinterpret_cast<u8*>(this) + 0x2EA8, bufs.upper);
    MakeCrystalCopyStateRecord(reinterpret_cast<u8*>(this) + 0x43B0, bufs.rec);
    reinterpret_cast<MCTailState*>(this)->ready0 = bufs.rec[0x30];
    reinterpret_cast<MCTailState*>(this)->ready1 = bufs.rec[0x31];
    __dt__15CMakeCrystalWinFv(&win, -1);

    // Register the render callback (the IScnRender sub-object at +0x58) with
    // the owning scene.
    void* render = this;
    if (this != NULL) {
        render = reinterpret_cast<u8*>(this) + 0x58;
    }
    addRenderCB__4CScnFP10IScnRenderUlUl(
        *reinterpret_cast<void**>(reinterpret_cast<u8*>(this) + 0x5C), render,
        0xD, 1);
    Scn_SetPauseFlag(*reinterpret_cast<void**>(reinterpret_cast<u8*>(this) + 0x5C),
                  0);

    reinterpret_cast<MCTailState*>(this)->gate = (CtrlRemote_TouchBitByArg(0x3386) != 0);
    MiniMapSetActiveFlag();
}

void MakeCrystalCopyDispArea(){}

void func_802116D4(){}

// Copy fields +0x4..+0x14 from src to dst (byte/word/byte copy).
void MakeCrystalCopyParamBlock(CMakeCrystalCopyBlock* dst, const CMakeCrystalCopyBlock* src) {
    dst->field_0x4 = src->field_0x4;
    dst->field_0x5 = src->field_0x5;
    dst->field_0x8 = src->field_0x8;
    dst->field_0xC = src->field_0xC;
    dst->field_0x10 = src->field_0x10;
    dst->field_0x14 = src->field_0x14;
}

void MakeCrystalCopyBox(){}

// Copy-construct the window-state record: run the UnkClass_8011C974 base
// copy-ctor at +0x04, then clone the five words (+0x14..+0x24) and six
// bytes (+0x28..+0x2D) verbatim.
struct MCStateWords {
    u32 w0;
    u32 w1;
    u32 w2;
    u32 w3;
    u32 w4;
};
struct MCStateBytes6 {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 b5;
};
struct MCStateRecord {
    u8 _00[0x04];
    u8 base[0x10];   // 0x04-0x13: UnkClass_8011C974 sub-object
    MCStateWords words;    // 0x14-0x27
    MCStateBytes6 bytes;   // 0x28-0x2D
};

MCStateRecord* MakeCrystalCopyStateRecord(MCStateRecord* dst, volatile MCStateRecord* src) {
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(dst->base, (const void*)src->base);
    dst->words.w0 = src->words.w0;
    dst->words.w1 = src->words.w1;
    dst->words.w2 = src->words.w2;
    dst->words.w3 = src->words.w3;
    dst->words.w4 = src->words.w4;
    dst->bytes.b0 = src->bytes.b0;
    dst->bytes.b1 = src->bytes.b1;
    dst->bytes.b2 = src->bytes.b2;
    dst->bytes.b3 = src->bytes.b3;
    dst->bytes.b4 = src->bytes.b4;
    dst->bytes.b5 = src->bytes.b5;
    return dst;
}

#include "monolib/scn/CScn.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// Typed view over the +0x58..+0x80 sub-objects (exact layout, explicit
// pads): render anchor, owning scene, CBgTex block, CMakeCrystalWin head.
struct MCSubObjView {
    u8 _pad58[0x58];
    u8 render58[0x4];   // 0x58-0x5B IScnRender sub-object (address taken)
    CScn* scene5C;      // 0x5C owning scene (loaded)
    u8 bg60[0x20];      // 0x60-0x7F CBgTex sub-object (address taken)
    u8 win80;           // 0x80 CMakeCrystalWin sub-object (address taken)
};

void CMenuMakeCrystal::Term() {
    waitForDrawDone__9CDeviceVIFv();
    CUICfManager_createQueuedMenus();
    Scn_SetPauseFlag(reinterpret_cast<MCSubObjView*>(this)->scene5C, 1);
    void* render = this;
    if (this) render = reinterpret_cast<MCSubObjView*>(this)->render58;
    reinterpret_cast<MCSubObjView*>(this)->scene5C->removeRenderCB((IScnRender*)render);
    BgTex_Release_3D9C(reinterpret_cast<MCSubObjView*>(this)->bg60);
    mkCrystalTeardown(&reinterpret_cast<MCSubObjView*>(this)->win80);
    sInstance = nullptr;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

// State byte at +0x43E4 plus its "ready" flag at +0x43E5.
struct MCStateBytes {
    u8 _00[0x43E4];
    u8 state;     // 0x43E4
    u8 ready;     // 0x43E5
};

// Callback-era flag byte at +0x54 (set when the menu finishes).
struct MCDoneFlag {
    u8 _00[0x54];
    u8 done;
};

void CMenuMakeCrystal::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    u8* self = reinterpret_cast<u8*>(this);
    MCStateBytes* st = reinterpret_cast<MCStateBytes*>(this);
    MCDoneFlag* done = reinterpret_cast<MCDoneFlag*>(this);

    if (reinterpret_cast<CMakeCrystalWin*>(&reinterpret_cast<MCSubObjView*>(self)->win80)->mkCrystalIsBusyState() == 0) {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        u32 btn;
        // Button bit depends on the controller type (classic vs Wii).
        if (cf::CfGameManager::isClassicController(-1) != 0) {
            btn = (pad->mPressedButtonFlags >> 23) & 1;
        } else {
            btn = (pad->mPressedButtonFlags >> 10) & 1;
        }
        if (btn != 0) {
            if (CMainMenu_GetInstancePtr() != 0) {
                ArtsInfo_SetReadyFlag();
                playUISound(6);
            }
            st->state = 3;
        }
    }

    switch (st->state) {
    case 0:
        MakeCrystalInitWin(this);
        break;
    case 1:
        st->state = 2;
        break;
    case 2:
        if (mkCrystalGetFlag4361(self + 0x80) != 0) {
            st->state = 3;
        }
        break;
    case 3:
        done->done = 1;
        break;
    }

    BgTex_Tick_3D54(self + 0x60);
    mkCrystalTick(self + 0x80);
}

void CMenuMakeCrystal::cbRenderBefore() {
    if (CTaskGame::getInstance()->isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0)
        return;
    GXSetZMode(GX_DISABLE, GX_NEVER, GX_DISABLE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(reinterpret_cast<u8*>(this) + 0x60, &drawInfo);
}

// Singleton instance of the crystal-crafting menu.
CMenuMakeCrystal* CMenuMakeCrystal::sInstance;

// Factory: create the singleton under `parent`. Returns NULL when it already
// exists; otherwise allocates the object from work memory, constructs it,
// registers it and returns the stored singleton.
CMenuMakeCrystal* CMenuMakeCrystal::create(CProcess* parent, CScn* scene) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuMakeCrystal* menu = new (CWorkThreadSystem::getWorkMem()) CMenuMakeCrystal(scene);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

bool MakeCrystalIsCreated() { return CMenuMakeCrystal::sInstance != nullptr; }
void* MakeCrystalGetWinData() {
    unsigned char* p = reinterpret_cast<unsigned char*>(CMenuMakeCrystal::sInstance);
    if (p != 0) {
        return reinterpret_cast<void*>(mkCrystalGetWord10(p + 0x80));
    }
    return 0;
}

extern "C" int BgTex_IsLoaded_3E34(void*);
extern "C" void mkCrystalBuildWin(void*);

// Init the +0x60 sub-object when it reports active, then set the +0x43E4 flag.
extern "C" void MakeCrystalInitWin(void* self) {
    if (BgTex_IsLoaded_3E34((u8*)self + 0x60) != 0) {
        mkCrystalBuildWin((u8*)self + 0x80);
        *(u8*)((u8*)self + 0x43E4) = 1;
    }
}
