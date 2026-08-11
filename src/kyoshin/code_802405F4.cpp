// Auto-scaffolded catalog TU for kyoshin/code_802405F4
// Replace stubs with high-level C/C++ during decomp.

// CMapSel.hpp carries a legacy (void*, float) pseudo-import for func_80137510;
// code_80135FDC.hpp (included below) declares the canonical
// (nw4r::lyt::AnimTransform*, float). Skip the legacy decl so the two
// extern "C" declarations don't clash (10197). Must be defined before any
// header that pulls in CMapSel.hpp (code_802405F4.hpp includes it).
#define KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
#include <types.h>

#include "kyoshin/code_802405F4.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include <monolib/scn/CScn.hpp>
#include <monolib/math/CVec3.hpp>
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CMapSel.hpp"
#include "kyoshin/CFade.hpp"

// Copy helpers for member sub-object initialization
// func_801BE108: copies CBgTex field-by-field from src to dest
extern void func_801BE108(CBgTex* dest, CBgTex* src);
// func_801BE16C: copies CTitleAHelp field-by-field from src to dest
extern void func_801BE16C(CTitleAHelp* dest, CTitleAHelp* src);

// Init helpers for sub-objects (defined later in this TU)
void func_80241640(CMapSel* dest, CMapSel* src);
void func_8024189C(CFade* dest, CFade* src);
void func_80241920(CFloorMap* dest, CFloorMap* src);
extern void func_8024343C(CMapSel* mapSel);
extern void func_8024439C(CFade* fade);

// Camera/transform position setters
extern void func_8049F168(void* obj, ml::CVec3* pos);
extern void func_8049F204(void* obj, ml::CVec3* pos);

// String table base for MNU_item / MNU_kyeassign lookups
extern "C" {
extern char lbl_eu_8050B498[];
// CfGameManager unity helpers bridge
extern void func_8008294C__Q22cf13CfGameManagerFv(bool enable);
}

// Entry within an array of map landmark positions (stride 0x188, 8 entries)
struct MapPointEntry {
    u8 flag;            // +0x000 - active flag (non-zero = enabled)
    u8 _01[0x16F];      // +0x001 to +0x16F
    ml::CVec3 pos;      // +0x170 - primary position
    ml::CVec3 pos2;     // +0x17C - secondary position
};

void func_80240614(){}

void func_80240878(){}

void func_802408D4(){}

void func_80240A64(u8* base) {
    base[0x0] = 0;
    *(int *)(base + 0x4) = 0;
    base[0x188] = 0;
    *(int *)(base + 0x18c) = 0;
    base[0x310] = 0;
    *(int *)(base + 0x314) = 0;
    base[0x498] = 0;
    *(int *)(base + 0x49c) = 0;
    base[0x620] = 0;
    *(int *)(base + 0x624) = 0;
    base[0x7a8] = 0;
    *(int *)(base + 0x7ac) = 0;
    base[0x930] = 0;
    *(int *)(base + 0x934) = 0;
    base[0xab8] = 0;
    *(int *)(base + 0xabc) = 0;
}

void func_80240AAC(){}

// Accumulates positions from active map point entries and applies them
// to a camera or transform object via func_8049F168 / func_8049F204.
void func_80240B10(MapPointEntry* entries, void* target) {
    if (cf::CfGameManager::func_800829B8()) {
        return;
    }

    ml::CVec3 accumPos = ml::CVec3::zero;
    ml::CVec3 accumPos2 = ml::CVec3::zero;
    bool anyActive = false;

    // 8 entries, processed as 4 pairs
    for (int i = 0; i < 4; i++) {
        MapPointEntry* entryA = &entries[i * 2];
        MapPointEntry* entryB = &entries[i * 2 + 1];

        if (entryA->flag != 0) {
            accumPos += entryA->pos;
            accumPos2 += entryA->pos2;
            anyActive = true;
        }
        if (entryB->flag != 0) {
            accumPos += entryB->pos;
            accumPos2 += entryB->pos2;
            anyActive = true;
        }
    }

    if (anyActive) {
        func_8049F168(target, &accumPos);
        func_8049F204(target, &accumPos2);
    }
}

void func_80240C98(){}

void func_8024125C(){}

void func_80241344(){}

// CMenuMapSelect::~CMenuMapSelect() - virtual destructor (D1/D2 merged; the
// class header declares the virtual dtor, the vtable entry binds to this
// extern "C" symbol). Destroys subobjects in reverse declaration order:
// CFloorMap at +0x194, CFade at +0x16C, CMapSel at +0xB8, CTitleAHelp at +0x80,
// CBgTex at +0x60, then the CProcess base, then conditionally frees memory.
// The nested CProcess null-checks reproduce MWCC's D2-inlined-into-D1
// artifacts (same shape as __dt__9CMainMenuFv in CMainMenu.cpp).
extern "C" void __dt__9CFloorMapFv(void*, int);
extern "C" void __dt__5CFadeFv(void*, int);
extern "C" void __dt__7CMapSelFv(void*, int);
extern "C" void __dt__11CTitleAHelpFv(void*, int);
extern "C" void __dt__6CBgTexFv(void*, int);
extern "C" void __dt__8CProcessFv(void*, int);
extern "C" void* __dt__14CMenuMapSelectFv(CMenuMapSelect* _this, int flags) {
    if (_this) {
        __dt__9CFloorMapFv((char*)_this + 0x194, -1);
        __dt__5CFadeFv((char*)_this + 0x16C, -1);
        __dt__7CMapSelFv((char*)_this + 0xB8, -1);
        __dt__11CTitleAHelpFv((char*)_this + 0x80, -1);
        __dt__6CBgTexFv((char*)_this + 0x60, -1);
        if (_this) {
            if (_this) {
                __dt__8CProcessFv(_this, 0);
            }
        }
        if (flags > 0) {
            __dl__FPv(_this);
        }
    }
    return _this;
}

void CMenuMapSelect::Init() {
    func_8008294C__Q22cf13CfGameManagerFv(1);

    {
        CBgTex bgTex(0);
        func_801BE108(&mBgTex, &bgTex);
    } // bgTex destructor runs here
    mBgTex.func_801C3C14();

    char* helpStr = (char*)func_80136190(lbl_eu_8050B498, lbl_eu_8050B498 + 8, 1);
    {
        CTitleAHelp titleHelp(helpStr, 0x4a);
        func_801BE16C(&mTitleHelp, &titleHelp);
    } // titleHelp destructor runs here
    mTitleHelp.CTitleAHelp_load();

    {
        CMapSel mapSel;
        func_80241640(&mMapSel, &mapSel);
    } // mapSel destructor runs here
    func_8024343C(&mMapSel);

    {
        CFade fade;
        func_8024189C(&mFade, &fade);
    } // fade destructor runs here
    func_8024439C(&mFade);

    {
        CFloorMap floorMap;
        func_80241920(&mFloorMap, &floorMap);
    } // floorMap destructor runs here

    char* cb = (char*)this;
    if (this != 0) cb = (char*)this + 0x58;
    mScn->addRenderCB((IScnRender*)cb, 0xd, 1);
}

// Copy helpers for member sub-object initialization (retail calls these from
// Init; empty stubs must not be inlined away at -O4,p, so disable auto-inline
// for their definitions).
#pragma push
#pragma auto_inline off
void func_80241640(CMapSel* dest, CMapSel* src){}

void func_8024189C(CFade* dest, CFade* src){}

void func_80241920(CFloorMap* dest, CFloorMap* src){}
#pragma pop

void CMenuMapSelect::Term() {}

void CMenuMapSelect::Move() {}

void CMenuMapSelect::cbRenderBefore() {}
