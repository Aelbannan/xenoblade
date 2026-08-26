// Auto-scaffolded catalog TU for kyoshin/CMiniMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMiniMap.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/FixStr.hpp"
#include <nw4r/lyt.h>

// Resolve ml::FixStr<128>::format to the explicit specialization defined in
// CfScript.cpp (retail symbol format__Q22ml10FixStr<128>FPCce).
template <> void ml::FixStr<128>::format(const char* fmt, ...);
#include <revolution/gx/GXPixel.h>

// Pre-mangled global-view declarations so the offset-adjusted dispatch thunks
// can address the CMenuMiniMap2 member methods by their retail symbol names
// (same pattern as CSysWinSelect.cpp).
extern "C" void* __dt__13CMenuMiniMap2Fv(CMenuMiniMap2* self, int flags);
void cbRenderBefore__13CMenuMiniMap2Fv(void* self);

// Library D1 imports used by the manual CMenuMiniMap2 destructor.
extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int flags);
extern "C" void __dt__8CProcessFv(void* self, int flags);
extern "C" void __dl__FPv(void* p);

// Offset-adjusted vtable dispatch thunk: the CMenuMiniMap2 subobject sits at
// self-0x58, so adjust and tail-jump straight to the dtor (retail:
// subi r3,r3,0x58 / b __dt__13CMenuMiniMap2Fv).
void func_8011C434(void* self) { ((void(*)(void*))__dt__13CMenuMiniMap2Fv)((char*)self - 0x58); }

// (func_80117734 defined below, after the MiniMapObj view declaration)

// Named .sdata2 conversion magics (defined in another TU; see CMiniMap.hpp).
// Declared extern only: a local const definition lets MWCC const-fold reads
// into anonymous @N pool labels instead of emitting retail-symbol relocs.
// 80667098 = signed int->float magic (2^52 + 2^31), 806670A8 unsigned (2^52).

CMMTex::~CMMTex() {}

bool CMMTex::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        // Grab the loaded buffer, then clear the handle state in retail order:
        // save mData, null the handle's mData, stash + flag + clear handle.
        u8* data = mFileHandle->mData;
        mFileHandle->mData = 0;
        mData = data;
        mReady = 1;
        mFileHandle = 0;
        return true;
    }
    return false;
}
// (constructor defined below, after the retail data/helper declarations)

// mUnk1C is auto-destroyed; MWCC emits the null-this guard and the
// deleting-flag tail (cmpwi flag / ble / operator delete) for the virtual dtor.
CMMClock::~CMMClock() {}

// (destructor defined below, after the retail data/helper declarations)

// Manual complete-object destructor (CMenuPause/CMenuBattleChain idiom):
// retail inlines the CMiniMap and CMMClock subobject destructors (each guarded
// on its subobject address), destroys the CProcess base through the library
// D1 __dt__8CProcessFv under the dead double null-check, and conditionally
// runs operator delete when the deleting flag is set.
// Manual complete-object destructor (CSysWinSave/CMenuPause idiom):
// retail inlines the CMiniMap and CMMClock subobject destructors (each guarded
// on its subobject address), destroys the CProcess base through the library
// D1 __dt__8CProcessFv under the dead double null-check, and conditionally
// runs operator delete when the deleting flag is set.
extern "C" void* __dt__13CMenuMiniMap2Fv(CMenuMiniMap2* _this, int flags) {
    if (_this != 0) {
        CMiniMap* minimap = &_this->mMiniMap;
        if (minimap != 0) {
            __dt__17UnkClass_8045F564Fv(minimap->m834, -1);
            __dt__17UnkClass_8045F564Fv(minimap->m824, -1);
        }
        CMMClock* clock = &_this->mClock;
        if (clock != 0) {
            __dt__17UnkClass_8045F564Fv(&clock->mUnk1C, -1);
        }
        // Duplicate null-check reproduces retail's dead double beq
        // (MWCC D2-inlined-into-D1 artifact).
        if (_this != 0) {
            if (_this != 0) {
                __dt__8CProcessFv(_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// func_80117C30 - per-frame minimap subobject update (called from Move with
// &mMiniMap). Body defined below, after func_8011B05C.
extern "C" void __declspec(noinline) func_80117C30(CMiniMap* self);

#include <nw4r/lyt.h>
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// ---- retail data symbols (small-data / absolute addressing per retail) ----
extern "C" s32 lbl_eu_80663FBC;          // frame counter (periodic cleanup gate)
extern "C" u32 lbl_eu_80664184;          // current BDAT row index
extern "C" void* lbl_eu_80663FB8;        // current BDAT table pointer
extern const f32 lbl_eu_80667090;          // zero constant
extern "C" f32 lbl_eu_806670A0;          // marker clamp scale
extern "C" f32 lbl_eu_806670B8;          // marker clamp scale (2nd)
extern "C" u32 lbl_eu_8052C7B8[];        // pane-name table (periodic cleanup)
extern u16 lbl_eu_8052C740[];          // per-row display-name table (u16)
extern const char* lbl_eu_8052C788[];  // clock pane-name table (textbox labels)
extern "C" void* lbl_eu_80573D18[];      // per-row BDAT entry table
extern "C" char lbl_eu_804FE1FC[];       // string pool (columns/formats/names)
// (u8[] declarations of lbl_eu_80526324/00/CB40/CB1C come from code_80135FDC.hpp)

// ---- cross-TU helpers (retail unmangled symbols) ----
extern "C" {
void func_80043D90(void* holder);
void* func_80043F18(void* holder);
u32 func_8009CF8C(u32 resourceId);
u8 func_801361E8(u32, const char*, u32);
u16 func_80136254(void*, const char*, u32);
s16 func_80136330(void*, const char*, u32);
u32 func_8003B1EC(void*);
void __dt__80043E88(void* holder, int);
void func_800F4A98(void* list, u32 type, u32 filter);
void* __ct__800FB044(void* list, f32, void* obj, int);
void* func_800BFC68__FPQ22cf12CfObjectMove(void* obj);
void* func_800B6CF8(int);
void* func_800B6C58();
void* func_800B6BEC();
void* func_800B6CC4();
u32 func_800F6E98(void* list, u32 index);
void* func_80193804();
void func_800ABC5C(ml::CVec3* out, void* obj);
void func_8045F7E8__17UnkClass_8045F564Fv(void* self);
void __ct__14Class_8045F858FP17UnkClass_8045F564(void* self, void* sub);
void __dt__14Class_8045F858Fv(void* self, int);
u32 func_801380A0(u16 row);
u8 func_80138138(u32 id);
void* func_80138234(void* bdat, u32 row);
void* func_8013902C(int type);
void func_80141DC4(ml::CVec3* out, u32 index);
nw4r::lyt::ArcResourceAccessor* func_801355F4();
void func_80137C1C(void* pic, s32 arg);
void func_80116B40(void* self);
void* func_801167EC(void* self);
void func_8011628C(void* self, u32 row);
void func_801160A8(void* self, void* table, void* layout, f32 scale);
void func_80116670(CMiniMapGimmickView* self, u32 arg, void* layout, f32 scale);
void* func_800BF324(void* obj);
void* createPicture__10CLibLayoutFv();
void* createTextbox__10CLibLayoutFv();
void SetName__Q34nw4r3lyt4PaneFPCc(void* pane, const char* name);
void RemoveChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(void* parent, void* child);
void AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(void* parent, void* child);
void PrependChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(void* parent, void* child);
void deleteTextboxOrPicture__10CLibLayoutFv(void* pane);
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
f32 FrSqrt__Q24nw4r4mathFf(f32);
int sprintf(char*, const char*, ...);
void func_80117734(CMMMapImg* self);
}
// C++-mangled retail helper: actor id -> action source object.
void* func_800B708C(int id);

// ============================================================================
// func_80118854 - CMiniMap marker/map-icon update (retail symbol unmangled).
// Reconstructed from retail ASM; high-level C++ only.
// ============================================================================

// CMiniMap "constructor" (retail unmangled factory-style __ct__CMiniMap).
// Zeroes the load-state block, builds the +0x2C loader subobject, resolves the
// localized map name via sprintf+getFP into the global BDAT cache, then
// initializes the marker/gimmick tables and pulls the per-row pane ids/scale
// from the current BDAT row.
extern "C" void __ct__CMiniMap(CMiniMap* self) {
    // Shared string-pool base (retail keeps it in a callee-saved register).
    char* strings = lbl_eu_804FE1FC;
    CMMSubHead* sub = &self->mSub;
    self->mVtable = (void*)lbl_eu_8052C958;
    self->mFileHandle = 0;
    self->mAccessor = 0;
    self->mLayout0C = 0;
    self->mAnimTrans0 = 0;
    self->mAnimTrans1 = 0;
    self->field_0x18 = 0;
    self->field_0x19 = 1;
    self->mReady = 0;
    self->mFlag1B = 0;
    self->field_0x1C = 0;
    self->field_0x1E = 0;
    self->mField20 = lbl_eu_80667090;
    self->mField24 = 0;
    self->mField28 = 0;

    // Embedded loader subobject at +0x2C (inlined member ctor in retail).
    sub->mVtable = (void*)lbl_eu_8052C9F0;
    sub->mPtr04 = 0;
    sub->mPtr08 = 0;
    self->mField3C.mFlag38 = 0;
    self->mField3C.mFlag39 = 0;
    self->mField3C.mFlag3A = 0;

    // Resolve the localized minimap layout name for the current BDAT row into
    // the global table cache.
    lbl_eu_80663FB8 = 0;
    char buf[0x20];
    sprintf(buf, strings + 0x147, lbl_eu_8052C740[lbl_eu_80664184 - 1]);
    lbl_eu_80663FB8 = getFP__FPCc(buf);

    func_80115FD0((MiniMapTable*)self->mField3C.mW);
    func_801165EC((CMiniMapGimmickView*)self->mField17C.mW);
    new (&self->m824) UnkClass_8045F564();
    new (&self->m834) UnkClass_8045F564();

    // u32->f32 via the shared 0x4330 double-magic idiom (textual reference
    // keeps MWCC's pool on the retail lbl_eu name instead of an @N label).
    union { double d; u32 w[2]; } conv;
    char* bdat = lbl_eu_806640A8;
    u32 row = (u8)lbl_eu_80664184;
    self->field_0x1C = func_80136330(bdat, strings + 0x18F, row);
    self->field_0x1E = func_80136330(bdat, strings + 0x199, row);
    conv.w[1] = func_801361E8((u32)bdat, strings + 0x1A3, row);
    conv.w[0] = 0x43300000;
    self->mField20 = (f32)(conv.d - lbl_eu_806670A8) * lbl_eu_806670B0;
    self->mField24 = (char*)getFP__FPCc(strings + 0x1B0);
    self->mField28 = (u32)getFP__FPCc(lbl_eu_8052C7E8[lbl_eu_80664184 - 1]);
}

// m834 then m824 destroyed in reverse construction order.
CMiniMap::~CMiniMap() {
    __dt__17UnkClass_8045F564Fv(m834, -1);
    __dt__17UnkClass_8045F564Fv(m824, -1);
}

// ============================================================================
// CMMClock::OnFileEvent - build the clock layout + animations once the arc
// file (mFileHandle) reports it is loaded. Mirrors CBattery/CFade's OnFileEvent:
// scratch region, accessor attach, layout + anim transforms, then enable.
// ============================================================================
bool CMMClock::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        mUnk1C.createRegion(mtl::MemManager::getHandleMEM2(), 0x4000,
                            &lbl_eu_804FE1FC[0xE0], 0);
        Class_8045F858 guard(&mUnk1C);
        u8* data = (u8*)mFileHandle->getData();
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, &lbl_eu_804FE1FC[0xE9]);
        func_80136E84(&mLayout, mAccessor, &lbl_eu_804FE1FC[0xED]);
        func_80136F08(mLayout, &mAnimTrans0, mAccessor, &lbl_eu_804FE1FC[0x105]);
        func_80136F08(mLayout, &mAnimTrans1, mAccessor, &lbl_eu_804FE1FC[0x126]);
        mLayout->SetAnimationEnable(mAnimTrans1, false);
        mLayout->SetAnimationEnable(mAnimTrans0, true);
        mLayout->Animate(0);
        mReady = 1;
        mFileHandle = 0;
        mUnk1C.func_8045F810();
        return true;
    }
    return false;
}

// ---- local SI types for retail virtual calls (fake SI iface pattern) ----
struct MiniMapSelf;
struct MiniMapPaneMgr {
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00C();
    virtual void v010(); virtual void v014(); virtual void v018(); virtual void v01C();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02C();
    virtual void v030(); virtual void v034();
    virtual void* v03C(const char* name, int create);  // vtable+0x3C
};
// Layout-side pane manager: the lookup helper sits at vtable+0x3C.
struct MiniMapLayoutMgr {
    virtual void v000(); virtual void v004(); virtual void v008();
    virtual void v00C(); virtual void v010(); virtual void v014();
    virtual void v018(); virtual void v01C(); virtual void v020();
    virtual void v024(); virtual void v028(); virtual void v02C();
    virtual void v030();
    virtual void* v03C(const char* name, int create);
};
struct MiniMapLayout {
    u8 pad[0x10];
    MiniMapLayoutMgr* mgr;       // 0x10 - pane manager (vtable+0x3C lookup)
};

// ============================================================================
// CMiniMap::OnFileEvent - build the minimap layout + animations once the arc
// file (mFileHandle) reports it is loaded, then seed the clock textboxes and
// the shared 'timg' picture onto the layout root and hand the tables to the
// per-frame marker routines (func_801160A8 / func_80116670).
// ============================================================================
bool CMiniMap::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        if (pEventFile->unk0 == 1) {
        Class_8045F858 guard((UnkClass_8045F564*)m824);
        // Take the loaded buffer, then clear the handle's data pointer before
        // building the accessor (retail order).
        u8* data = mFileHandle->mData;
        mFileHandle->mData = 0;
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, &lbl_eu_804FE1FC[0xE9]);

        // Resolve the per-row layout/animation names from the row table.
        s32 rowIdx = (s32)lbl_eu_80664184 - 1;
        u16 row = lbl_eu_8052C740[rowIdx];
        char buf[0x40];
        sprintf(buf, &lbl_eu_804FE1FC[0x219], row);
        func_80136E84(&mLayout0C, mAccessor, buf);
        sprintf(buf, &lbl_eu_804FE1FC[0x239], row);
        func_80136F08(mLayout0C, &mAnimTrans0, mAccessor, buf);
        sprintf(buf, &lbl_eu_804FE1FC[0x25C], row);
        func_80136F08(mLayout0C, &mAnimTrans1, mAccessor, buf);

        mLayout0C->SetAnimationEnable(mAnimTrans1, false);
        mLayout0C->SetAnimationEnable(mAnimTrans0, true);
        mLayout0C->Animate(0);

        // Clock label textboxes, attached to the 'panemapmark' pane.
        nw4r::lyt::Pane* parent = (nw4r::lyt::Pane*)((MiniMapLayout*)mLayout0C)->mgr->v03C(
            &lbl_eu_804FE1FC[0x1FA], 1);
        if (parent != 0) {
            for (u8 i = 0; lbl_eu_8052C788[i] != 0; i++) {
                nw4r::lyt::Pane* tb = (nw4r::lyt::Pane*)createTextbox__10CLibLayoutFv();
                SetName__Q34nw4r3lyt4PaneFPCc(tb, lbl_eu_8052C788[i]);
                *(u8*)((u8*)tb + 0xBB) = (*(u8*)((u8*)tb + 0xBB) & 0xFE) | 1;
                AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(parent, tb);
            }
        }

        // Shared 'timg' picture pinned to the origin.
        nw4r::lyt::Pane* pic;
        if (&lbl_eu_804FE1FC[0x202] == 0) {
            pic = 0;
        } else {
            void* texName = func_8013902C(2);
            if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0) == 0) {
                pic = 0;
            } else {
                pic = (nw4r::lyt::Pane*)createPicture__10CLibLayoutFv();
                SetName__Q34nw4r3lyt4PaneFPCc(pic, &lbl_eu_804FE1FC[0x202]);
                *(f32*)((u8*)pic + 0x2C) = lbl_eu_80667090;
                *(f32*)((u8*)pic + 0x30) = lbl_eu_80667090;
                // Dead origin record (retail stores it to the frame).
                ml::CVec3 pos;
                pos.x = lbl_eu_80667090;
                pos.y = lbl_eu_80667090;
                pos.z = lbl_eu_80667090;
                *(f32*)((u8*)pic + 0x34) = lbl_eu_80667090;
                func_80137C1C(pic, -1);
                *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
            }
        }

        // Scale the map-name textbox from the shared picture's size; retail
        // reads these fields even when the picture failed to load.
        f32 picW = *(f32*)((u8*)pic + 0x4C);
        f32 picH = *(f32*)((u8*)pic + 0x50);
        f32 scaleY = lbl_eu_806670B4 * picH;
        *(f32*)((u8*)pic + 0x2C) = lbl_eu_80667090;
        ml::CVec3 sizeKeep;   // dead frame records (retail stores them)
        sizeKeep.x = picW;
        *(f32*)((u8*)pic + 0x30) = scaleY;
        sizeKeep.y = picH;
        ml::CVec3 scaleKeep;
        scaleKeep.x = lbl_eu_80667090;
        scaleKeep.y = scaleY;
        scaleKeep.z = lbl_eu_80667090;
        *(f32*)((u8*)pic + 0x34) = lbl_eu_80667090;

        nw4r::lyt::Pane* tb = (nw4r::lyt::Pane*)createTextbox__10CLibLayoutFv();
        SetName__Q34nw4r3lyt4PaneFPCc(tb, &lbl_eu_804FE1FC[0x20A]);
        *(u8*)((u8*)tb + 0xBB) = (*(u8*)((u8*)tb + 0xBB) & 0xFE) | 1;
        parent = (nw4r::lyt::Pane*)((MiniMapLayout*)mLayout0C)->mgr->v03C(&lbl_eu_804FE1FC[0x1FA], 1);
        AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(parent, tb);
        AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(tb, pic);

        // Clock-face 'timg' picture.
        nw4r::lyt::Pane* clockPic;
        if (&lbl_eu_804FE1FC[0x212] == 0) {
            clockPic = 0;
        } else {
            void* texName = func_8013902C(1);
            if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0) == 0) {
                clockPic = 0;
            } else {
                clockPic = (nw4r::lyt::Pane*)createPicture__10CLibLayoutFv();
                SetName__Q34nw4r3lyt4PaneFPCc(clockPic, &lbl_eu_804FE1FC[0x212]);
                *(f32*)((u8*)clockPic + 0x2C) = lbl_eu_80667090;
                *(f32*)((u8*)clockPic + 0x30) = lbl_eu_80667090;
                // Dead origin record (retail stores it to the frame).
                ml::CVec3 origin;
                origin.x = lbl_eu_80667090;
                origin.y = lbl_eu_80667090;
                origin.z = lbl_eu_80667090;
                *(f32*)((u8*)clockPic + 0x34) = lbl_eu_80667090;
                func_80137C1C(clockPic, -1);
                *(u8*)((u8*)clockPic + 0xBB) = (*(u8*)((u8*)clockPic + 0xBB) & 0xFE) | 1;
            }
        }
        parent = (nw4r::lyt::Pane*)((MiniMapLayout*)mLayout0C)->mgr->v03C(&lbl_eu_804FE1FC[0x281], 1);
        AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(parent, clockPic);

        func_801160A8(mField3C.mW, lbl_eu_80663FB8, mLayout0C, mField20);
        func_80116670((CMiniMapGimmickView*)mField17C.mW, mField28, mLayout0C, mField20);
        mReady = 1;
        ((UnkClass_8045F564*)m824)->func_8045F810();
        }
        mFileHandle = 0;
        return true;
    }
    return false;
}

// ---- local SI types for retail virtual calls (fake SI iface pattern): ----
// (MiniMapPaneMgr / MiniMapLayoutMgr / MiniMapLayout are defined above
// CMiniMap::OnFileEvent so it can use the mgr v03C lookups.)
struct MiniMapSelf {
    void* vtable;                    // 0x00
    u8 pad04[0x0C - 0x04];
    struct MiniMapMgr {
        u8 pad[0x10];
        MiniMapPaneMgr* m10;         // 0x10
    }* m0C;                          // 0x0C
    u8 pad10[0x1B - 0x10];
    u8 m1B;                          // 0x1B (func_8011B05C result flag)
    s16 m1C;                         // 0x1C (func_8011B05C rotation)
    s16 m1E;                         // 0x1E (func_8011B05C rotation)
    f32 m20;                         // 0x20 (marker grid scale)
    u32 m24;                         // 0x24 (BDAT table)
    u8 pad28[0x38 - 0x28];
    u8 m38;                          // 0x38 (func_8011B05C gate flag)
    u8 pad39[0x3C - 0x39];
    void* m3C;                       // 0x3C
    u8 pad40[0x17C - 0x40];
    u8 m17C[0x6B8];                  // 0x17C..0x834
    u8 m834[0x80];                   // 0x834
};
struct MiniMapObj {
    virtual void v000(); virtual void v004(); virtual void* v008(int);
    virtual void v00C(); virtual void v010(); virtual void v014(); virtual void v018();
    virtual void* v01C(); virtual void v020(); virtual void v024(); virtual void v028();
    virtual void v02C(); virtual void v030(); virtual void v034(); virtual void v038();
    virtual void v03C(); virtual void v040(); virtual void v044(); virtual void v048();
    virtual void v04C(); virtual void v050(); virtual void v054(); virtual void v058();
    virtual void v05C(); virtual void v060(); virtual void v064(); virtual void v068();
    virtual void v06C(); virtual void v070(); virtual void v074(); virtual void v078();
    virtual void v07C(); virtual void v080(); virtual void v084(); virtual void v088();
    virtual void v08C(); virtual void v090(); virtual void v094(); virtual void v098();
    virtual void v09C(); virtual void v0A0(); virtual void v0A4();
    virtual ml::CVec3* GetPos();     // vtable+0xAC
    virtual void v0B0(); virtual void v0B4(); virtual void v0B8(); virtual void v0BC();
    virtual void v0C0(); virtual void v0C4(); virtual void v0C8(); virtual f32 v0CC();
    virtual void v0D0(); virtual void v0D4(); virtual void v0D8(); virtual void v0DC();
    virtual void v0E0(); virtual void v0E4(); virtual void v0E8(); virtual void v0EC();
    virtual void v0F0(); virtual void v0F4(); virtual void v0F8(); virtual void v0FC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134(); virtual void v138(); virtual void v13C();
    virtual void v140(); virtual void v144(); virtual void v148(); virtual void v14C();
    virtual void v150(); virtual void v154(); virtual void v158(); virtual void v15C();
    virtual void* v160();            // vtable+0x160 (valid check)
    virtual void v164(); virtual void v168(); virtual void v16C(); virtual void v170();
    virtual void v174(); virtual void v178(); virtual void v17C(); virtual void v180();
    virtual void v184(); virtual void v188(); virtual void v18C(); virtual void v190();
    virtual void v194(); virtual void v198(); virtual void v19C(); virtual void v1A0();
    virtual void v1A4(); virtual void v1A8(); virtual void v1AC(); virtual void v1B0();
    virtual void v1B4(); virtual void v1B8(); virtual void v1BC(); virtual void v1C0();
    virtual void v1C4(); virtual void v1C8(); virtual void v1CC(); virtual void v1D0();
    virtual void v1D4(); virtual void v1D8(); virtual void v1DC(); virtual void v1E0();
    virtual void v1E4(); virtual void v1E8(); virtual void v1EC(); virtual void v1F0();
    virtual void v1F4(); virtual void v1F8(); virtual void v1FC(); virtual void v200();
    virtual void v204(); virtual void v208(); virtual void v20C(); virtual void v210();
    virtual void v214(); virtual void v218(); virtual void v21C(); virtual void v220();
    virtual void v224();
    virtual int v228();              // vtable+0x228 (mode)
    u8 pad[0x64 - 0x04];
    u32 m64;                         // 0x64 flags
    u8 pad68[0x74 - 0x68];
    u32 m74;                         // 0x74 id (lwz)
    u8 pad78[0x8C - 0x78];
    u16 m8C;                         // 0x8C id (lhz)
    u8 pad8E[0x91 - 0x8E];
    s8 m91;                          // 0x91 kind
    u8 pad92[0xBB - 0x92];
    u8 mBB;                          // 0xBB flag
};

// ============================================================================
// func_80117734 - map-image loader request (retail symbol unmangled).
// Cancels any pending load and frees the previous buffer, selects the BDAT row
// whose floor height is above the player's y, resolves the unlock-gated
// resource variant, then requests the formatted image path into MEM2.
// (Body defined below, after the MiniMapSubObj view declaration.)
// ============================================================================

struct MiniMapDtorIf {
    virtual void v008(int);          // vtable+0x8 deleting dtor
};
struct MiniMapEnumHolder {
    void* list;                      // 0x00
    u32 handle;                      // 0x04
};
struct MiniMapEnumList {
    u8 pad[0x620];
    u32 count;                       // 0x620
};
struct MiniMapListNode {
    MiniMapListNode* next;           // 0x00
    u8 pad04[0x08 - 0x04];
    void* object;                    // 0x08
};
struct MiniMapList {
    void* field_00;                  // 0x00
    MiniMapListNode* head;           // 0x04
};
struct MiniMapCleanupNode {
    MiniMapCleanupNode* next;        // 0x00
    u8 pad04[0xB8 - 0x04];
    char name[0x40];                 // 0xB8
};
struct MiniMapCleanupList {
    u8 pad[0x14];
    MiniMapCleanupNode* first;       // 0x14 (sentinel == &first)
};
struct MiniMapCase2Elem {
    f32 x;                           // 0x00
    f32 y;                           // 0x04
    f32 z;                           // 0x08
    u8 pad0C[0x1C - 0x0C];
    u16 m1C;                         // 0x1C id
    u8 pad1E[0x4C - 0x1E];           // stride 76
};

// Typed reset views over the marker-table (+0x3C) / gimmick-view (+0x178)
// regions used only by the teardown below (integer/float split matches the
// retail store widths).
struct MiniMapTermTable {
    void* mBdat;     // 0x3C
    u32 mPad40;
    u32 mLive;       // 0x44
    f32 mScale;      // 0x48
};
struct MiniMapTermView {
    void* mBdat;                     // 0x178
    u32 mField04;                    // 0x17C
    u32 mField08;                    // 0x180
    f32 mField0C;                    // 0x184
    u8 mPad188[0x10];
    u32 mField10;                    // 0x188
    u32 mField14;                    // 0x18C
    u32 mField18;                    // 0x190
    u8 mPad194[0x594 - 0x194];
    u32 mField594;                   // 0x70C
};

// Per-row marker-pane teardown (runs when the minimap layout is torn down).
// Clears every registered marker/clock pane, drops the fixed clock labels,
// resets the marker/gimmick tables, releases the layout + accessor + UnkClass
// scratch regions, and frees the cached 'timg' resource.
extern "C" void __declspec(noinline) func_80118058(CMiniMap* self) {
    self->mReady = 0;
    func_801390E0(&self->mFileHandle);
    if (self->mLayout0C != 0) {
        MiniMapCleanupList* list;
        MiniMapCleanupNode* node;
        MiniMapCleanupNode* end;
        MiniMapCleanupNode* next;
        MiniMapLayout* lay;
        void* pane;
        const char* pf;
        const char* ps;
        // Registered marker lists (per-row pane-name table).
        for (u8 i = 0; lbl_eu_8052C7B8[i] != 0; i++) {
            list = (MiniMapCleanupList*)(
                (MiniMapLayout*)self->mLayout0C)->mgr->v03C((const char*)lbl_eu_8052C7B8[i], 1);
            node = list->first;
            end = (MiniMapCleanupNode*)&list->first;
            lay = (MiniMapLayout*)self->mLayout0C;
            while (node != end) {
                next = node->next;
                if (!node) {
                    Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 573,
                                               (const char*)lbl_eu_8052CB1C);
                }
                pane = lay->mgr->v03C(node->name, 1);
                if (pane && *(void**)((u8*)pane + 0x0C)) {
                    RemoveChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                        *(void**)((u8*)pane + 0x0C), pane);
                    ((MiniMapDtorIf*)pane)->v008(-1);
                    deleteTextboxOrPicture__10CLibLayoutFv(pane);
                }
                node = next;
            }
        }
        // Fixed clock-label textboxes created by OnFileEvent.
        pane = ((MiniMapLayout*)self->mLayout0C)->mgr->v03C(&lbl_eu_804FE1FC[0x202], 1);
        if (pane && *(void**)((u8*)pane + 0x0C)) {
            RemoveChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                *(void**)((u8*)pane + 0x0C), pane);
            ((MiniMapDtorIf*)pane)->v008(-1);
            deleteTextboxOrPicture__10CLibLayoutFv(pane);
        }
        pane = ((MiniMapLayout*)self->mLayout0C)->mgr->v03C(&lbl_eu_804FE1FC[0x20A], 1);
        if (pane && *(void**)((u8*)pane + 0x0C)) {
            RemoveChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                *(void**)((u8*)pane + 0x0C), pane);
            ((MiniMapDtorIf*)pane)->v008(-1);
            deleteTextboxOrPicture__10CLibLayoutFv(pane);
        }
        pane = ((MiniMapLayout*)self->mLayout0C)->mgr->v03C(&lbl_eu_804FE1FC[0x212], 1);
        if (pane && *(void**)((u8*)pane + 0x0C)) {
            RemoveChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                *(void**)((u8*)pane + 0x0C), pane);
            ((MiniMapDtorIf*)pane)->v008(-1);
            deleteTextboxOrPicture__10CLibLayoutFv(pane);
        }
        // 'panemapmark' holder list (layout pointer hoisted like retail).
        list = (MiniMapCleanupList*)(
            (MiniMapLayout*)self->mLayout0C)->mgr->v03C(&lbl_eu_804FE1FC[0x1FA], 1);
        node = list->first;
        end = (MiniMapCleanupNode*)&list->first;
        lay = (MiniMapLayout*)self->mLayout0C;
        pf = (const char*)lbl_eu_8052CB40;
        ps = (const char*)lbl_eu_8052CB1C;
        while (node != end) {
            next = node->next;
            if (!node) {
                Panic__Q24nw4r2dbFPCciPCce(pf, 573, ps);
            }
            pane = lay->mgr->v03C(node->name, 1);
            if (pane && *(void**)((u8*)pane + 0x0C)) {
                RemoveChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                    *(void**)((u8*)pane + 0x0C), pane);
                ((MiniMapDtorIf*)pane)->v008(-1);
                deleteTextboxOrPicture__10CLibLayoutFv(pane);
            }
            node = next;
        }

        // Reset marker-table and gimmick-view working state.
        MiniMapTermTable* table = (MiniMapTermTable*)self->mField3C.mW;
        MiniMapTermView* view = (MiniMapTermView*)self->mField17C.mW;
        table->mBdat = 0;
        view->mBdat = 0;
        table->mLive = 0;
        table->mScale = lbl_eu_80667090;
        view->mField04 = 0;
        view->mField08 = 0;
        view->mField0C = lbl_eu_80667090;
        view->mField18 = 0;
        view->mField10 = 0;
        view->mField14 = 0;
        view->mField594 = 0;
        self->mPad820 = 0;

        // Release the layout (dead double null-check mirrors ~CMiniMap).
        if (self->mLayout0C != 0) {
            if (self->mLayout0C != 0) {
                ((MiniMapDtorIf*)self->mLayout0C)->v008(1);
            }
            self->mLayout0C = 0;
        }
    }
    func_80139124(self->mAccessor);
    ((UnkClass_8045F564*)&self->m824)->func_8045F778();
    ((UnkClass_8045F564*)&self->m834)->func_8045F778();
    func_801390E0((CFileHandle**)&self->mSub.mPtr04);
    if (self->mSub.mPtr08 != NULL) {
        deallocate__Q23mtl10MemManagerFPv(self->mSub.mPtr08);
        self->mSub.mPtr08 = NULL;
    }
}

// Shared marker-placement tail (inlined twice by retail with distinct locals).
// copy #1: diff1 @ sp+636, name1 @ sp+872, pane parent "panemapmark".
// copy #2: diff2 @ sp+624, name2 @ sp+840.
extern "C" void func_80118854(MiniMapSelf* self) {
    MiniMapEnumHolder holder;
    MiniMapEnumList* list;
    MiniMapObj* obj;
    f32 clamp;
    u8 type;
    u16 mapId;
    void* gimmickView;
    f32 zero = lbl_eu_80667090;
    ml::CVec3 diff1;
    ml::CVec3 diff2;
    char name1[32];
    char name2[32];
    char buf3[32];
    char buf4[32];
    char buf5[32];
    char buf6[32];
    char buf7[32];
    char buf8[32];

    if (!self->m0C) return;
    if (!cf::CfGameManager::getPlayer(0)) return;
    obj = (MiniMapObj*)func_800BFC68__FPQ22cf12CfObjectMove(cf::CfGameManager::getPlayer(0));
    func_80043D90(&holder);
    list = (MiniMapEnumList*)func_80043F18(&holder);
    func_800F4A98(list, 0xB00, 0);
    {
        void* playerPos = ((MiniMapObj*)cf::CfGameManager::getPlayer(0))->GetPos();
        __ct__800FB044(func_80043F18(&holder), lbl_eu_806670B8 * self->m20, playerPos, 0);
    }
    if (lbl_eu_80663FBC++ > 30) {
        lbl_eu_80663FBC = 0;
        for (u8 i = 0; lbl_eu_8052C7B8[i] != 0; i++) {
            MiniMapCleanupList* clist =
                (MiniMapCleanupList*)self->m0C->m10->v03C((const char*)lbl_eu_8052C7B8[i], 1);
            MiniMapCleanupNode* node = clist->first;
            while (node != (MiniMapCleanupNode*)&clist->first) {
                if (!node) {
                    Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 573,
                                               (const char*)lbl_eu_8052CB1C);
                }
                MiniMapCleanupNode* next = node->next;
                void* pane = self->m0C->m10->v03C(node->name, 1);
                if (pane && *(void**)((u8*)pane + 0x0C)) {
                    RemoveChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                        *(void**)((u8*)pane + 0x0C), pane);
                    ((MiniMapDtorIf*)pane)->v008(-1);
                    deleteTextboxOrPicture__10CLibLayoutFv(pane);
                }
                node = next;
            }
        }
        func_80116B40((u8*)self + 0x17C);
        func_8045F7E8__17UnkClass_8045F564Fv((u8*)self + 0x834);
    }
    {
        u8 guard[0x10];
        __ct__14Class_8045F858FP17UnkClass_8045F564(guard, (u8*)self + 0x834);
        {
            u16 row = (u16)func_8009CF8C(32);
            if (row != 0) {
                if ((u8)func_801361E8(self->m24, &lbl_eu_804FE1FC[649], row) ==
                    (u8)lbl_eu_80664184) {
                    clamp = lbl_eu_806670A0 * self->m20;
                    type = func_801361E8(self->m24, &lbl_eu_804FE1FC[64], row);
                    mapId = func_80136254((void*)(u32)self->m24, &lbl_eu_804FE1FC[69], row);
                    if (mapId == 0) {
                        diff1.x = lbl_eu_80667090;
                        name1[0] = 0;
                        diff1.y = lbl_eu_80667090;
                        diff1.z = lbl_eu_80667090;
                        if ((int)type == 1) {
                            MiniMapList* glist = (MiniMapList*)func_800B6CF8(1);
                            for (MiniMapListNode* node = glist->head->next;
                                 node != glist->head; node = node->next) {
                                MiniMapObj* g = (MiniMapObj*)node->object;
                                f32 objY = g->GetPos()->y;
                                u8 rowA = 0;
                                if (cf::CfObjectMove* player =
                                        cf::CfGameManager::getPlayer(0)) {
                                                                        ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                                    ml::CVec3 ppos;
                                    ppos.x = pposPtr->x;
                                    ppos.y = pposPtr->y;
                                    ppos.z = pposPtr->z;
                                    f32 playerY = ppos.y;
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 i = 1; i <= count; i++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], i);
                                        if ((f32)v > playerY) { rowA = i; break; }
                                    }
                                }
                                int found = 0;
                                {
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 j = 1; j <= count; j++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], j);
                                        if ((f32)v > objY) {
                                            if (j == rowA) found = 1;
                                            break;
                                        }
                                    }
                                }
                                if (found) {
                                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *g->GetPos();
                                    f32 dx = diff.x;
                                    f32 dy = diff.y;
                                    f32 dz = diff.z;
                                    diff1 = diff;
                                    sprintf(name1, &lbl_eu_804FE1FC[653], g->m8C);
                                }
                            }
                        }
                        if (name1[0] != 0) {
                            f32 len2 = nw4r::math::VEC3LenSq(diff1);
                            if (len2 > clamp * clamp) {
                                if (len2 < 0.0f) {
                                    Warning__Q24nw4r2dbFPCciPCce(
                                        (const char*)lbl_eu_80526324, 0x273,
                                        (const char*)lbl_eu_80526300);
                                }
                                f32 len = (len2 <= 0.0f)
                                              ? 0.0f
                                              : len2 * FrSqrt__Q24nw4r4mathFf(len2);
                                f32 k = clamp / len;
                                diff1.x *= k;
                                diff1.z *= k;
                            }
                            s32 gx = -(s32)(diff1.x / self->m20);
                            s32 gz = (s32)(diff1.z / self->m20);
                            void* pane = self->m0C->m10->v03C(name1, 1);
                            if (pane) {
                                f32 fgx = (f32)gx;
                                f32 fgz = (f32)gz;
                                *(f32*)((u8*)pane + 0x2C) = fgx;
                                *(f32*)((u8*)pane + 0x30) = fgz;
                                *(f32*)((u8*)pane + 0x34) = zero;
                            } else {
                                void* pic = 0;
                                void* texName = func_8013902C(7);
                                if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                    pic = createPicture__10CLibLayoutFv();
                                    SetName__Q34nw4r3lyt4PaneFPCc(pic, name1);
                                    *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                    *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                    *(f32*)((u8*)pic + 0x34) = zero;
                                    func_80137C1C(pic, -1);
                                    *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                                }
                                if (pic) {
                                    void* parent = self->m0C->m10->v03C(
                                        &lbl_eu_804FE1FC[105], 1);
                                    AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                        parent, pic);
                                }
                            }
                        }
                        goto mode32;
                    } else {
                        name2[0] = 0;
                        diff2.x = lbl_eu_80667090;
                        diff2.y = lbl_eu_80667090;
                        diff2.z = lbl_eu_80667090;
                        switch (type) {
                        case 2: {
                            MiniMapCase2Elem* arr = (MiniMapCase2Elem*)func_80193804();
                            s32 count = *(s32*)((u8*)arr + 0x9800);
                            for (MiniMapCase2Elem* el = arr; el < arr + count; el++) {
                                if (el->m1C != mapId) continue;
                                f32 objY = el->y;
                                u8 rowA = 0;
                                if (cf::CfObjectMove* player =
                                        cf::CfGameManager::getPlayer(0)) {
                                                                        ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                                    ml::CVec3 ppos;
                                    ppos.x = pposPtr->x;
                                    ppos.y = pposPtr->y;
                                    ppos.z = pposPtr->z;
                                    f32 playerY = ppos.y;
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 i = 1; i <= count; i++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], i);
                                        if ((f32)v > playerY) { rowA = i; break; }
                                    }
                                }
                                int found = 0;
                                {
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 j = 1; j <= count; j++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], j);
                                        if ((f32)v > objY) {
                                            if (j == rowA) found = 1;
                                            break;
                                        }
                                    }
                                }
                                if (found) {
                                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() -
                                                     ml::CVec3(el->x, el->y, el->z);
                                    f32 dx = diff.x;
                                    f32 dy = diff.y;
                                    f32 dz = diff.z;
                                    diff2 = diff;
                                    sprintf(name2, &lbl_eu_804FE1FC[137], el->m1C);
                                    goto converge;
                                }
                            }
                            break;
                        }
                        case 3: {
                            MiniMapList* glist = (MiniMapList*)func_800B6C58();
                            for (MiniMapListNode* node = glist->head->next;
                                 node != glist->head; node = node->next) {
                                MiniMapObj* g = (MiniMapObj*)node->object;
                                if ((g->m64 & 0x4000) == 0 && (g->m64 & 0x8000) == 0)
                                    continue;
                                if (g->m8C != mapId) continue;
                                f32 objY = g->GetPos()->y;
                                u8 rowA = 0;
                                if (cf::CfObjectMove* player =
                                        cf::CfGameManager::getPlayer(0)) {
                                                                        ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                                    ml::CVec3 ppos;
                                    ppos.x = pposPtr->x;
                                    ppos.y = pposPtr->y;
                                    ppos.z = pposPtr->z;
                                    f32 playerY = ppos.y;
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 i = 1; i <= count; i++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], i);
                                        if ((f32)v > playerY) { rowA = i; break; }
                                    }
                                }
                                int found = 0;
                                {
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 j = 1; j <= count; j++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], j);
                                        if ((f32)v > objY) {
                                            if (j == rowA) found = 1;
                                            break;
                                        }
                                    }
                                }
                                if (found) {
                                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *g->GetPos();
                                    f32 dx = diff.x;
                                    f32 dy = diff.y;
                                    f32 dz = diff.z;
                                    diff2 = diff;
                                    sprintf(name2, &lbl_eu_804FE1FC[125], g->m74);
                                    goto converge;
                                }
                            }
                            break;
                        }
                        case 4: {
                            MiniMapList* glist = (MiniMapList*)func_800B6BEC();
                            for (MiniMapListNode* node = glist->head->next;
                                 node != glist->head; node = node->next) {
                                MiniMapObj* g = (MiniMapObj*)node->object;
                                if (g->m8C != mapId) continue;
                                f32 objY = g->GetPos()->y;
                                u8 rowA = 0;
                                if (cf::CfObjectMove* player =
                                        cf::CfGameManager::getPlayer(0)) {
                                                                        ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                                    ml::CVec3 ppos;
                                    ppos.x = pposPtr->x;
                                    ppos.y = pposPtr->y;
                                    ppos.z = pposPtr->z;
                                    f32 playerY = ppos.y;
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 i = 1; i <= count; i++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], i);
                                        if ((f32)v > playerY) { rowA = i; break; }
                                    }
                                }
                                int found = 0;
                                {
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 j = 1; j <= count; j++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], j);
                                        if ((f32)v > objY) {
                                            if (j == rowA) found = 1;
                                            break;
                                        }
                                    }
                                }
                                if (found) {
                                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *g->GetPos();
                                    f32 dx = diff.x;
                                    f32 dy = diff.y;
                                    f32 dz = diff.z;
                                    diff2 = diff;
                                    sprintf(name2, &lbl_eu_804FE1FC[664], g->m74);
                                    goto converge;
                                }
                            }
                            break;
                        }
                        case 5: {
                            MiniMapList* glist = (MiniMapList*)func_800B6BEC();
                            for (MiniMapListNode* node = glist->head->next;
                                 node != glist->head; node = node->next) {
                                MiniMapObj* g = (MiniMapObj*)node->object;
                                if (g->m8C != mapId) continue;
                                f32 objY = g->GetPos()->y;
                                u8 rowA = 0;
                                if (cf::CfObjectMove* player =
                                        cf::CfGameManager::getPlayer(0)) {
                                                                        ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();
                                    ml::CVec3 ppos;
                                    ppos.x = pposPtr->x;
                                    ppos.y = pposPtr->y;
                                    ppos.z = pposPtr->z;
                                    f32 playerY = ppos.y;
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 i = 1; i <= count; i++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], i);
                                        if ((f32)v > playerY) { rowA = i; break; }
                                    }
                                }
                                int found = 0;
                                {
                                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                                    for (u8 j = 1; j <= count; j++) {
                                        s16 v = func_80136330(lbl_eu_80663FB8,
                                                              &lbl_eu_804FE1FC[48], j);
                                        if ((f32)v > objY) {
                                            if (j == rowA) found = 1;
                                            break;
                                        }
                                    }
                                }
                                if (found) {
                                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *g->GetPos();
                                    f32 dx = diff.x;
                                    f32 dy = diff.y;
                                    f32 dz = diff.z;
                                    diff2 = diff;
                                    sprintf(name2, &lbl_eu_804FE1FC[677], g->m74);
                                    goto converge;
                                }
                            }
                            break;
                        }
                        default:
                            break;
                        }
                    converge:
                        if (name2[0] != 0) {
                            f32 len2 = nw4r::math::VEC3LenSq(diff2);
                            if (len2 > clamp * clamp) {
                                if (len2 < 0.0f) {
                                    Warning__Q24nw4r2dbFPCciPCce(
                                        (const char*)lbl_eu_80526324, 0x273,
                                        (const char*)lbl_eu_80526300);
                                }
                                f32 len = (len2 <= 0.0f)
                                              ? 0.0f
                                              : len2 * FrSqrt__Q24nw4r4mathFf(len2);
                                f32 k = clamp / len;
                                diff2.x *= k;
                                diff2.z *= k;
                            }
                            s32 gx = -(s32)(diff2.x / self->m20);
                            s32 gz = (s32)(diff2.z / self->m20);
                            void* pane = self->m0C->m10->v03C(name2, 1);
                            if (pane) {
                                f32 fgx = (f32)gx;
                                f32 fgz = (f32)gz;
                                *(f32*)((u8*)pane + 0x2C) = fgx;
                                *(f32*)((u8*)pane + 0x30) = fgz;
                                *(f32*)((u8*)pane + 0x34) = zero;
                            } else {
                                void* pic = 0;
                                void* texName = func_8013902C(7);
                                if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                    pic = createPicture__10CLibLayoutFv();
                                    SetName__Q34nw4r3lyt4PaneFPCc(pic, name2);
                                    *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                    *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                    *(f32*)((u8*)pic + 0x34) = zero;
                                    func_80137C1C(pic, -1);
                                    *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                                }
                                if (pic) {
                                    void* parent = self->m0C->m10->v03C(
                                        &lbl_eu_804FE1FC[105], 1);
                                    AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                        parent, pic);
                                }
                            }
                        }
                        goto mode32;
                    }
                }
            }
        }

        // ---- mode-32 section ----
    mode32:
        {
            gimmickView = func_801167EC((u8*)self + 0x17C);
            {
#undef __cntlzw
                u32 cf8cVal = (u32)func_8009CF8C(0x3334);
                u32 cntlz = (u32)__cntlzw(cf8cVal);
                if ((cntlz >> 5) != 0) {
                for (u32 i = 0;
                     i < ((MiniMapEnumList*)func_80043F18(&holder))->count; i++) {
                                        u32 id = (u32)func_800F6E98(func_80043F18(&holder), i);
                                        MiniMapObj* o = (MiniMapObj*)func_800B708C((s32)id);
                    if (!o) continue;
                    if (!o->v160()) continue;
                    if ((o->m64 & 0x4000) == 0 && (o->m64 & 0x8000) == 0) continue;
                    f32 objY = o->GetPos()->y;
                    u8 rowA = 0;
                    if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                                ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();

                                                ml::CVec3 ppos;

                                                ppos.x = pposPtr->x;

                                                ppos.y = pposPtr->y;

                                                ppos.z = pposPtr->z;

                                                f32 playerY = ppos.y;
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 i = 1; i <= count; i++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], i);
                            if ((f32)v > playerY) { rowA = i; break; }
                        }
                    }
                    int found = 0;
                    {
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 j = 1; j <= count; j++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], j);
                            if ((f32)v > objY) {
                                if (j == rowA) found = 1;
                                break;
                            }
                        }
                    }
                    if (!found) continue;
                    {
                        ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *o->GetPos();
                        f32 dx = diff.x;
                        f32 dy = diff.y;
                        f32 dz = diff.z;
                        sprintf(buf3, &lbl_eu_804FE1FC[125], id);
                        s32 gx = -(s32)(dx / self->m20);
                        s32 gz = (s32)(dz / self->m20);
                        void* pane = self->m0C->m10->v03C(buf3, 1);
                        if (pane) {
                            f32 fgx = (f32)gx;
                            f32 fgz = (f32)gz;
                            *(f32*)((u8*)pane + 0x2C) = fgx;
                            *(f32*)((u8*)pane + 0x30) = fgz;
                            *(f32*)((u8*)pane + 0x34) = zero;
                        } else if (o->m91 == 6) {
                            void* pic = 0;
                            void* texName = func_8013902C(15);
                            if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                pic = createPicture__10CLibLayoutFv();
                                SetName__Q34nw4r3lyt4PaneFPCc(pic, buf3);
                                *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                *(f32*)((u8*)pic + 0x34) = zero;
                                func_80137C1C(pic, -1);
                                *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                            }
                            if (pic) {
                                void* parent = self->m0C->m10->v03C(
                                    &lbl_eu_804FE1FC[692], 1);
                                AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                    parent, pic);
                            }
                        }
                    }
                }
            }
            }
        }

        // ---- row-table section ----
        {
            u8 rowB = (u8)lbl_eu_80664184;
            void* bdat2 = (void*)func_801380A0(rowB);
            void* bdatEntry = lbl_eu_80573D18[rowB - 1];
            u16 rowCount = (u16)func_8003B1EC(bdatEntry);
            if (gimmickView != 0) {
                for (u32 i = 0;
                     i < ((MiniMapEnumList*)func_80043F18(&holder))->count; i++) {
                                        MiniMapObj* o = (MiniMapObj*)func_800B708C((s32)func_800F6E98(func_80043F18(&holder), i));
                    if (!o) continue;
                    if ((o->m64 & 0x8) == 0) continue;
                    f32 objY = o->GetPos()->y;
                    u8 rowA = 0;
                    if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                                ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();

                                                ml::CVec3 ppos;

                                                ppos.x = pposPtr->x;

                                                ppos.y = pposPtr->y;

                                                ppos.z = pposPtr->z;

                                                f32 playerY = ppos.y;
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 i = 1; i <= count; i++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], i);
                            if ((f32)v > playerY) { rowA = i; break; }
                        }
                    }
                    int found = 0;
                    {
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 j = 1; j <= count; j++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], j);
                            if ((f32)v > objY) {
                                if (j == rowA) found = 1;
                                break;
                            }
                        }
                    }
                    if (!found) continue;
                    for (u16 k = (u16)rowB;
                         (s32)(u16)k < (s32)((u16)rowB + rowCount); k++) {
                        if (func_80136254(bdat2, &lbl_eu_804FE1FC[702], (u16)k) !=
                            o->m8C)
                            continue;
                        if (!func_80138234(bdat2, (u16)k)) continue;
                        {
                            ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *o->GetPos();
                            f32 dx = diff.x;
                            f32 dy = diff.y;
                            f32 dz = diff.z;
                            sprintf(buf4, &lbl_eu_804FE1FC[115], o->m74);
                            s32 gx = -(s32)(dx / self->m20);
                            s32 gz = (s32)(dz / self->m20);
                            void* pane = self->m0C->m10->v03C(buf4, 1);
                            if (pane) {
                                f32 fgx = (f32)gx;
                                f32 fgz = (f32)gz;
                                *(f32*)((u8*)pane + 0x2C) = fgx;
                                *(f32*)((u8*)pane + 0x30) = fgz;
                                *(f32*)((u8*)pane + 0x34) = zero;
                            } else {
                                void* pic = 0;
                                void* texName = func_8013902C(10);
                                if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                    pic = createPicture__10CLibLayoutFv();
                                    SetName__Q34nw4r3lyt4PaneFPCc(pic, buf4);
                                    *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                    *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                    *(f32*)((u8*)pic + 0x34) = zero;
                                    func_80137C1C(pic, -1);
                                    *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                                }
                                if (pic) {
                                    void* parent = self->m0C->m10->v03C(
                                        &lbl_eu_804FE1FC[709], 1);
                                    AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                        parent, pic);
                                }
                            }
                        }
                    }
                }
            }

            // ---- sub-actor section (vt228()==3) ----
            for (u32 i = 0;
                 i < ((MiniMapEnumList*)func_80043F18(&holder))->count; i++) {
                                    u32 id = (u32)func_800F6E98(func_80043F18(&holder), i);
                                    MiniMapObj* o = (MiniMapObj*)func_800B708C((s32)id);
                if (!o) continue;
                if (!o->v160()) continue;
                if ((o->m64 & 0x8) == 0) continue;
                if (((MiniMapObj*)func_800BF324(o))->v228() != 3) continue;
                f32 objY = o->GetPos()->y;
                u8 rowA = 0;
                if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                        ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();

                                        ml::CVec3 ppos;

                                        ppos.x = pposPtr->x;

                                        ppos.y = pposPtr->y;

                                        ppos.z = pposPtr->z;

                                        f32 playerY = ppos.y;
                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                    for (u8 i = 1; i <= count; i++) {
                        s16 v = func_80136330(lbl_eu_80663FB8,
                                              &lbl_eu_804FE1FC[48], i);
                        if ((f32)v > playerY) { rowA = i; break; }
                    }
                }
                int found = 0;
                {
                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                    for (u8 j = 1; j <= count; j++) {
                        s16 v = func_80136330(lbl_eu_80663FB8,
                                              &lbl_eu_804FE1FC[48], j);
                        if ((f32)v > objY) {
                            if (j == rowA) found = 1;
                            break;
                        }
                    }
                }
                if (!found) continue;
                {
                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *o->GetPos();
                    f32 dx = diff.x;
                    f32 dy = diff.y;
                    f32 dz = diff.z;
                    sprintf(buf5, &lbl_eu_804FE1FC[115], id);
                    s32 gx = -(s32)(dx / self->m20);
                    s32 gz = (s32)(dz / self->m20);
                    void* pane = self->m0C->m10->v03C(buf5, 1);
                    if (pane) {
                        f32 fgx = (f32)gx;
                        f32 fgz = (f32)gz;
                        *(f32*)((u8*)pane + 0x2C) = fgx;
                        *(f32*)((u8*)pane + 0x30) = fgz;
                        *(f32*)((u8*)pane + 0x34) = zero;
                    } else {
                        void* pic = 0;
                        void* texName = func_8013902C(12);
                        if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                            pic = createPicture__10CLibLayoutFv();
                            SetName__Q34nw4r3lyt4PaneFPCc(pic, buf5);
                            *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                            *(f32*)((u8*)pic + 0x30) = (f32)gz;
                            *(f32*)((u8*)pic + 0x34) = zero;
                            func_80137C1C(pic, -1);
                            *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                        }
                        if (pic) {
                            void* parent = self->m0C->m10->v03C(
                                &lbl_eu_804FE1FC[718], 1);
                            AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                parent, pic);
                        }
                    }
                }
            }

            // ---- m91==12 section ----
            for (u32 i = 0;
                 i < ((MiniMapEnumList*)func_80043F18(&holder))->count; i++) {
                                    u32 id = (u32)func_800F6E98(func_80043F18(&holder), i);
                                    MiniMapObj* o = (MiniMapObj*)func_800B708C((s32)id);
                if (!o) continue;
                if (!o->v160()) continue;
                if ((o->m64 & 0x4000) == 0 && (o->m64 & 0x8000) == 0) continue;
                if (o->m91 != 12) continue;
                f32 objY = o->GetPos()->y;
                u8 rowA = 0;
                if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                        ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();

                                        ml::CVec3 ppos;

                                        ppos.x = pposPtr->x;

                                        ppos.y = pposPtr->y;

                                        ppos.z = pposPtr->z;

                                        f32 playerY = ppos.y;
                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                    for (u8 i = 1; i <= count; i++) {
                        s16 v = func_80136330(lbl_eu_80663FB8,
                                              &lbl_eu_804FE1FC[48], i);
                        if ((f32)v > playerY) { rowA = i; break; }
                    }
                }
                int found = 0;
                {
                    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                    for (u8 j = 1; j <= count; j++) {
                        s16 v = func_80136330(lbl_eu_80663FB8,
                                              &lbl_eu_804FE1FC[48], j);
                        if ((f32)v > objY) {
                            if (j == rowA) found = 1;
                            break;
                        }
                    }
                }
                if (!found) continue;
                {
                    ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *o->GetPos();
                    f32 dx = diff.x;
                    f32 dy = diff.y;
                    f32 dz = diff.z;
                    sprintf(buf6, &lbl_eu_804FE1FC[125], id);
                    s32 gx = -(s32)(dx / self->m20);
                    s32 gz = (s32)(dz / self->m20);
                    void* pane = self->m0C->m10->v03C(buf6, 1);
                    if (pane) {
                        f32 fgx = (f32)gx;
                        f32 fgz = (f32)gz;
                        *(f32*)((u8*)pane + 0x2C) = fgx;
                        *(f32*)((u8*)pane + 0x30) = fgz;
                        *(f32*)((u8*)pane + 0x34) = zero;
                    } else {
                        void* pic = 0;
                        void* texName = func_8013902C(16);
                        if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                            pic = createPicture__10CLibLayoutFv();
                            SetName__Q34nw4r3lyt4PaneFPCc(pic, buf6);
                            *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                            *(f32*)((u8*)pic + 0x30) = (f32)gz;
                            *(f32*)((u8*)pic + 0x34) = zero;
                            func_80137C1C(pic, -1);
                            *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                        }
                        if (pic) {
                            void* parent = self->m0C->m10->v03C(
                                &lbl_eu_804FE1FC[727], 1);
                            AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                parent, pic);
                        }
                    }
                }
            }

            // ---- type-dispatch section (17/18/20) ----
            if (gimmickView != 0) {
                for (u32 i = 0;
                     i < ((MiniMapEnumList*)func_80043F18(&holder))->count; i++) {
                                        u32 id = (u32)func_800F6E98(func_80043F18(&holder), i);
                                        MiniMapObj* o = (MiniMapObj*)func_800B708C((s32)id);
                    if (!o) continue;
                    if (!o->v160()) continue;
                    f32 objY = o->GetPos()->y;
                    u8 rowA = 0;
                    if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                                ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();

                                                ml::CVec3 ppos;

                                                ppos.x = pposPtr->x;

                                                ppos.y = pposPtr->y;

                                                ppos.z = pposPtr->z;

                                                f32 playerY = ppos.y;
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 i = 1; i <= count; i++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], i);
                            if ((f32)v > playerY) { rowA = i; break; }
                        }
                    }
                    int found = 0;
                    {
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 j = 1; j <= count; j++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], j);
                            if ((f32)v > objY) {
                                if (j == rowA) found = 1;
                                break;
                            }
                        }
                    }
                    if (!found) continue;
                    {
                        ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - *o->GetPos();
                        f32 dx = diff.x;
                        f32 dy = diff.y;
                        f32 dz = diff.z;
                        sprintf(buf7, &lbl_eu_804FE1FC[115], id);
                        s32 gx = -(s32)(dx / self->m20);
                        s32 gz = (s32)(dz / self->m20);
                        void* pane = self->m0C->m10->v03C(buf7, 1);
                        if (pane) {
                            f32 fgx = (f32)gx;
                            f32 fgz = (f32)gz;
                            *(f32*)((u8*)pane + 0x2C) = fgx;
                            *(f32*)((u8*)pane + 0x30) = fgz;
                            *(f32*)((u8*)pane + 0x34) = zero;
                        } else {
                            s32 type2 = 0;
                            u32 arg2 = 0xFFFFFFFF;
                            if ((__cntlzw(o->m64 & 0x80000000) >> 5) == 0) {
                                type2 = 17;
                                arg2 = 0xFF8C1EFF;
                            } else if ((__cntlzw(o->m64 & 0x8) >> 5) == 0) {
                                int mode = o->v228();
                                if (mode == 2) {
                                    type2 = 18;
                                    arg2 = 0x3232C8FF;
                                } else if (mode == 9 || mode == 13) {
                                    type2 = 17;
                                    arg2 = 0xFF8C1EFF;
                                }
                            } else if ((__cntlzw(o->m64 & 0x10) >> 5) == 0) {
                                type2 = 20;
                                arg2 = 0xEE1100FF;
                            }
                            void* pic = 0;
                            if (type2 != 0) {
                                void* texName = func_8013902C(type2);
                                if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                    pic = createPicture__10CLibLayoutFv();
                                    SetName__Q34nw4r3lyt4PaneFPCc(pic, buf7);
                                    *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                    *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                    *(f32*)((u8*)pic + 0x34) = zero;
                                    func_80137C1C(pic, arg2);
                                    *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                                }
                            }
                            if (pic) {
                                void* parent = self->m0C->m10->v03C(
                                    &lbl_eu_804FE1FC[739], 1);
                                AppendChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                    parent, pic);
                            }
                        }
                    }
                }
            }

            // ---- reslist section ----
            {
                MiniMapList* rlist = (MiniMapList*)func_800B6CC4();
                for (MiniMapListNode* node = rlist->head->next;
                     node != rlist->head; node = node->next) {
                    MiniMapObj* o = (MiniMapObj*)node->object;
                    f32 objY = o->GetPos()->y;
                    u8 rowA = 0;
                    if (cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0)) {
                                                ml::CVec3* pposPtr = ((MiniMapObj*)player)->GetPos();

                                                ml::CVec3 ppos;

                                                ppos.x = pposPtr->x;

                                                ppos.y = pposPtr->y;

                                                ppos.z = pposPtr->z;

                                                f32 playerY = ppos.y;
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 i = 1; i <= count; i++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], i);
                            if ((f32)v > playerY) { rowA = i; break; }
                        }
                    }
                    int found = 0;
                    {
                        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
                        for (u8 j = 1; j <= count; j++) {
                            s16 v = func_80136330(lbl_eu_80663FB8,
                                                  &lbl_eu_804FE1FC[48], j);
                            if ((f32)v > objY) {
                                if (j == rowA) found = 1;
                                break;
                            }
                        }
                    }
                    if (!found) continue;
                    {
                        f32 radius = lbl_eu_806670B8 * self->m20;
                        ml::CVec3 pos;
                        func_800ABC5C(&pos, o);
                        ml::CVec3 diff = *((MiniMapObj*)((u8*)obj + 0x3E9C))->GetPos() - pos;
                        f32 dx = diff.x;
                        f32 dy = diff.y;
                        f32 dz = diff.z;
                        f32 len2 = nw4r::math::VEC3LenSq(diff);
                        if (len2 > radius * radius) {
                            s32 gx = -(s32)(dx / self->m20);
                            s32 gz = (s32)(dz / self->m20);
                            sprintf(buf8, &lbl_eu_804FE1FC[747], o->m74);
                            void* pane = self->m0C->m10->v03C(buf8, 1);
                            if (pane) {
                                f32 fgx = (f32)gx;
                                f32 fgz = (f32)gz;
                                *(f32*)((u8*)pane + 0x2C) = fgx;
                                *(f32*)((u8*)pane + 0x30) = fgz;
                                *(f32*)((u8*)pane + 0x34) = zero;
                            } else {
                                void* pic = 0;
                                void* texName = func_8013902C(3);
                                if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                                    pic = createPicture__10CLibLayoutFv();
                                    SetName__Q34nw4r3lyt4PaneFPCc(pic, buf8);
                                    *(f32*)((u8*)pic + 0x2C) = (f32)gx;
                                    *(f32*)((u8*)pic + 0x30) = (f32)gz;
                                    *(f32*)((u8*)pic + 0x34) = zero;
                                    func_80137C1C(pic, -1);
                                    *(u8*)((u8*)pic + 0xBB) = (*(u8*)((u8*)pic + 0xBB) & 0xFE) | 1;
                                }
                                if (pic) {
                                    void* parent = self->m0C->m10->v03C(
                                        &lbl_eu_804FE1FC[762], 1);
                                    PrependChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(
                                        parent, pic);
                                }
                            }
                        }
                    }
                }
            }
        }

        func_8011628C((u8*)self + 0x3C, lbl_eu_80664184);
        __dt__14Class_8045F858Fv(guard, -1);
        __dt__80043E88(&holder, -1);
    }
}

// ============================================================================
// func_8011B05C - per-frame map-view content update: position the map
// material's texture SRT at the player, then orient/scale the clock panes.
// Reconstructed from retail ASM (setup -> SRT math -> pane loops).
// ============================================================================

// Material view: flags word at +0x3C (bits 4-7 and bit 28 are validated with
// Panic before use); the SRT/TexMap arrays are reached via nw4r::lyt types.
struct MiniMapB05CMat {
    u8 pad_00[0x3C];                 // 0x00..0x3C
    u32 field_3C;                    // 0x3C - flags (bits 4-7, bit 28)
};

// Linked list walked by the pane-sweep loops (next at +0, translate at +0x34).
struct MiniMapB05CNode {
    MiniMapB05CNode* next;           // 0x00
    u8 pad_04[0x34 - 0x04];          // 0x04..0x34
    ml::CVec3 ofs;                   // 0x34 - per-node translate offset
};

// View of the v01C() result (battle target pos): y at +0x4.
struct MiniMapB05CVec4 {
    u8 pad_00[0x4];                  // 0x00
    f32 field_4;                     // 0x04
};

// Pane data view: intrusive child-list head at +0x14 and the translate
// triple at +0x38.
struct MiniMapB05CPaneView {
    void* vtable;                    // 0x00
    u8 pad_04[0x14 - 0x04];
    void* first;                     // 0x14 - child list head (next ptr)
    u8 pad_18[0x38 - 0x18];
    ml::CVec3 tr;                    // 0x38 - pane translate
};

// Object view tuned so v01C/GetPos/v0CC land at vtable+0x1C/+0xAC/+0xCC
// (MWCC prepends a hidden two-slot dtor pair to these memberless views).
struct MiniMapB05CObjView {
    virtual void _d000(); virtual void _d004(); virtual void _d008();
    virtual void _d00C(); virtual void _d010();
    virtual void* v01C();
    virtual void _d020(); virtual void _d024(); virtual void _d028();
    virtual void _d02C(); virtual void _d030(); virtual void _d034();
    virtual void _d038(); virtual void _d03C(); virtual void _d040();
    virtual void _d044(); virtual void _d048(); virtual void _d04C();
    virtual void _d050(); virtual void _d054(); virtual void _d058();
    virtual void _d05C(); virtual void _d060(); virtual void _d064();
    virtual void _d068(); virtual void _d06C(); virtual void _d070();
    virtual void _d074(); virtual void _d078(); virtual void _d07C();
    virtual void _d080(); virtual void _d084(); virtual void _d088();
    virtual void _d08C(); virtual void _d090(); virtual void _d094();
    virtual void _d098(); virtual void _d09C(); virtual void _d0A0();
    virtual void _d0A4(); virtual void _d0A8();
    virtual ml::CVec3* GetPos();     // vtable+0xAC
    virtual void _d0B0(); virtual void _d0B4(); virtual void _d0B8();
    virtual void _d0BC(); virtual void _d0C0(); virtual void _d0C4();
    virtual void _d0C8(); virtual void _d0CC(); virtual void _d0D0();
    virtual f32 v0CC();              // vtable+0xCC
};

// Pane interface view with vtable slots 0x3C (child lookup) / 0x68 (material).
// (MWCC prepends a hidden two-slot dtor pair to these views.)
struct MiniMapB05CIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void* vf3C(const char* name, int create);
    virtual void _v040(); virtual void _v044(); virtual void _v048(); virtual void _v04C();
    virtual void _v050(); virtual void _v054(); virtual void _v058(); virtual void _v05C();
    virtual void _v060(); virtual void _v064(); virtual void* vf68();
};

// Pane-manager view whose lookup helper sits at vtable+0x3C.
struct MiniMapB05CMgr {
    virtual void _v000(); virtual void _v004(); virtual void _v008(); virtual void _v00C();
    virtual void _v010(); virtual void _v014(); virtual void _v018(); virtual void _v01C();
    virtual void _v020(); virtual void _v024(); virtual void _v028(); virtual void _v02C();
    virtual void _v030();
    virtual void* vfFind(const char* name, int create);
};

// Best-effort reconstruction (elided retail loop body).
extern "C" void func_8011B05C(MiniMapSelf* self) {
    // 'panemapmark' holder pane + its material (pane manager v03C -> vt+0x68).
    MiniMapB05CIf* pane = (MiniMapB05CIf*)(
        (MiniMapB05CMgr*)self->m0C->m10)->vfFind(&lbl_eu_804FE1FC[0x1FA], 1);
    MiniMapB05CMat* mat = (MiniMapB05CMat*)pane->vf68();
    const nw4r::lyt::Material* matC = (const nw4r::lyt::Material*)mat;

    if (((mat->field_3C >> 24) & 0xF) == 0) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CBC0, 0x9C,
                                   (const char*)lbl_eu_8052CB8C);
    }

    // Snapshot the map material's texture SRT and TexMap into locals; the SRT
    // is written back after the position math below.
    nw4r::lyt::TexSRT srt = *matC->GetTexSRTAry();

    if ((mat->field_3C >> 28) == 0) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB80, 0x79,
                                   (const char*)lbl_eu_8052CB4C);
    }
    nw4r::lyt::TexMap tmap = *matC->GetTexMapAry();
    nw4r::lyt::Size tsize = tmap.GetSize();

    f64 scale = lbl_eu_80667090;
    ml::CVec3 pos;
    MiniMapObj* player = (MiniMapObj*)cf::CfGameManager::getPlayer(0);
    if (player != NULL) {
        pos = *(((MiniMapB05CObjView*)player)->GetPos());
        scale = lbl_eu_806670BC *
                ((MiniMapB05CObjView*)player)->v0CC() / lbl_eu_8066A1F8;
    } else {
        pos.set(lbl_eu_80667090, lbl_eu_80667090, lbl_eu_80667090);
    }

    // Map the player position into texture space. m1C/m1E are per-axis grid
    // scales and m20 the marker grid size; retail computes this with a
    // fctiwz/fmadds block over truncated grid coordinates.
    f32 ex = pos.x / self->m20;
    f64 rotE = (f64)self->m1E;
    f32 ez = pos.z / self->m20;
    f64 rotC = (f64)self->m1C;
    s32 gxi = (s32)ex;
    s32 gzi = (s32)ez;
    s32 twi = (s32)tsize.width;
    s32 thi = (s32)tsize.height;

    if (((mat->field_3C >> 4) & 0xF) == 0) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CC00, 0xA2,
                                   (const char*)lbl_eu_8052CBCC);
    }
    srt.translate.x =
        (lbl_eu_806670A4 / tsize.width *
             (lbl_eu_806670B4 * tsize.width + (f32)rotC) -
         lbl_eu_806670B4) +
        (lbl_eu_806670A4 / (f32)rotC *
             (lbl_eu_806670B4 * (f32)twi + (f32)gxi) -
         lbl_eu_806670B4);
    srt.translate.y =
        (lbl_eu_806670A4 / tsize.height *
             (lbl_eu_806670B4 * tsize.height + (f32)rotE) -
         lbl_eu_806670B4) +
        (lbl_eu_806670A4 / (f32)thi *
             (lbl_eu_806670B4 * (f32)thi + (f32)gzi) -
         lbl_eu_806670B4);

    nw4r::lyt::TexSRT* dst = ((nw4r::lyt::Material*)mat)->GetTexSRTAry();
    dst->translate = srt.translate;
    dst->rotate = srt.rotate;
    dst->scale = srt.scale;

    // Clock pane group: reposition it, then sweep the clock label panes'
    // child lists (either the idle layout or the battle-target tracking one).
    MiniMapB05CIf* group = (MiniMapB05CIf*)(
        (MiniMapB05CMgr*)self->m0C->m10)->vfFind(&lbl_eu_804FE1FC[0x212], 1);
    ml::CVec3 gv(lbl_eu_80667090, lbl_eu_80667090, scale);

    if (func_8013BE58() == 0) {
        gv.z = scale + lbl_eu_806670BC;
        ((MiniMapB05CPaneView*)group)->tr = gv;
        ((MiniMapB05CPaneView*)pane)->tr = ml::CVec3(
            lbl_eu_80667090, lbl_eu_80667090, lbl_eu_80667090);
        for (u8 i = 0; lbl_eu_8052C788[i] != 0; i++) {
            MiniMapB05CIf* p =
                (MiniMapB05CIf*)pane->vf3C(lbl_eu_8052C788[i], 1);
            if (p == NULL) continue;
            MiniMapB05CPaneView* pv = (MiniMapB05CPaneView*)p;
            for (MiniMapB05CNode* n = (MiniMapB05CNode*)pv->first;
                 n != (MiniMapB05CNode*)&pv->first; n = n->next) {
                ml::CVec3 tmp(lbl_eu_80667090, lbl_eu_80667090,
                              lbl_eu_80667090);
                if (n == NULL) {
                    Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40,
                                               0x23D,
                                               (const char*)lbl_eu_8052CB1C);
                }
                n->ofs = tmp;
            }
        }
    } else if (cf::CfGameManager::getInstance()->func_800821F8() != NULL) {
        // Battle target present: pin the clock panes to the target position.
        MiniMapObj* tgt =
            (MiniMapObj*)cf::CfGameManager::getInstance()->func_800821F8();
        f32 ty = ((MiniMapB05CVec4*)(
            (MiniMapB05CObjView*)tgt)->v01C())->field_4;
        f64 t = lbl_eu_806670BC * ty / lbl_eu_8066A1F8;
        f32 zz = lbl_eu_806670BC + (scale - t);
        ((MiniMapB05CPaneView*)group)->tr =
            ml::CVec3(lbl_eu_80667090, lbl_eu_80667090, zz);
        ((MiniMapB05CPaneView*)pane)->tr.x = gv.x;
        ((MiniMapB05CPaneView*)pane)->tr.y = gv.y;
        ((MiniMapB05CPaneView*)pane)->tr.z = -(f32)t;
        gv.z = (f32)(-t) * lbl_eu_806670C0;
        for (u8 i = 0; lbl_eu_8052C788[i] != 0; i++) {
            MiniMapB05CIf* p =
                (MiniMapB05CIf*)pane->vf3C(lbl_eu_8052C788[i], 1);
            if (p == NULL) continue;
            MiniMapB05CPaneView* pv = (MiniMapB05CPaneView*)p;
            for (MiniMapB05CNode* n = (MiniMapB05CNode*)pv->first;
                 n != (MiniMapB05CNode*)&pv->first; n = n->next) {
                if (n == NULL) {
                    Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40,
                                               0x23D,
                                               (const char*)lbl_eu_8052CB1C);
                }
                n->ofs.x = gv.x;
                n->ofs.y = gv.y;
                n->ofs.z = gv.z;
            }
        }
    }

    // Minimap 'timg' pane: pin to the current battle target position.
    MiniMapB05CIf* pic = (MiniMapB05CIf*)(
        (MiniMapB05CMgr*)self->m0C->m10)->vfFind(&lbl_eu_804FE1FC[0x20A], 1);
    if (pic != NULL) {
        if (cf::CfGameManager::getInstance()->func_800821F8() != NULL) {
            MiniMapObj* tgt = (MiniMapObj*)(
                cf::CfGameManager::getInstance()->func_800821F8());
            f32 ty = ((MiniMapB05CVec4*)(
                (MiniMapB05CObjView*)tgt)->v01C())->field_4;
            f32 t = lbl_eu_806670BC * ty / lbl_eu_8066A1F8;
            ((MiniMapB05CPaneView*)pic)->tr =
                ml::CVec3(lbl_eu_80667090, lbl_eu_80667090, t);
        }
    }
    self->m1B = (self->m38 != 0) ? 1 : 0;
}


// Init: register the IScnRender subobject as a scene render callback, build
// fresh CMMClock/CMiniMap temps on the stack and copy them over the process
// members (retail assigns whole objects - MWCC elides the vtable words), then
// request the clock arc / minimap arc files into MEM2 and re-seed the loader
// subobject before asking it for the map image.
void CMenuMiniMap2::Init() {
    // Buffers/subobject temps declared up front; filled at their use sites.
    char nameBuf[0x40];
    char rowBuf[0x20];
    CMMSub sub;

    // The `if (this)` is the MWCC idiom that splits mr r4 / beq / addi +0x5c
    // for the IScnRender subobject passed to addRenderCB.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScn->addRenderCB(render, 0xA, 0);

    // Hand-built CMMClock temp: vtable + zeroed fields + gate bytes, embedded
    // UnkClass constructed explicitly so its bl lands after the field stores.
    CMMClockInit clockInit;
    clockInit.mVtable = (void*)lbl_eu_8052CA88;
    clockInit.mFileHandle = NULL;
    clockInit.mAccessor = NULL;
    clockInit.mLayout = NULL;
    clockInit.mAnimTrans0 = NULL;
    clockInit.mAnimTrans1 = NULL;
    clockInit.field_0x18 = 0;
    clockInit.field_0x19 = 1;
    clockInit.mReady = 0;
    UnkClass_8045F564* pClockUnk = new (&clockInit.mUnk1C) UnkClass_8045F564();

    // Whole-object member copy into mClock (vtable word skipped).
    mClock.mFileHandle = clockInit.mFileHandle;
    mClock.mAccessor = clockInit.mAccessor;
    mClock.mLayout = clockInit.mLayout;
    mClock.mAnimTrans0 = clockInit.mAnimTrans0;
    mClock.mAnimTrans1 = clockInit.mAnimTrans1;
    mClock.field_0x18 = clockInit.field_0x18;
    mClock.field_0x19 = clockInit.field_0x19;
    mClock.mReady = clockInit.mReady;
    mClock.mUnk1C = *pClockUnk;
    __dt__17UnkClass_8045F564Fv(pClockUnk, -1);

    if (mClock.mReady == 0) {
        mClock.mFileHandle = CDeviceFile::readFile(
            mtl::MemManager::getHandleMEM2(), &lbl_eu_804FE1FC[0xCC],
            (IWorkEvent*)&mClock, 0, 0);
    }

    // Fresh CMiniMap temp; the scope-exit dtor emits the two UnkClass D1 calls.
    {
        CMiniMap map;
        __ct__CMiniMap(&map);

        // Whole-object member copy into mMiniMap (both vtable words skipped).
        mMiniMap.mFileHandle = map.mFileHandle;
        mMiniMap.mAccessor = map.mAccessor;
        mMiniMap.mLayout0C = map.mLayout0C;
        mMiniMap.mAnimTrans0 = map.mAnimTrans0;
        mMiniMap.mAnimTrans1 = map.mAnimTrans1;
        mMiniMap.field_0x18 = map.field_0x18;
        mMiniMap.field_0x19 = map.field_0x19;
        mMiniMap.mReady = map.mReady;
        mMiniMap.mFlag1B = map.mFlag1B;
        mMiniMap.field_0x1C = map.field_0x1C;
        mMiniMap.field_0x1E = map.field_0x1E;
        mMiniMap.mField20 = map.mField20;
        mMiniMap.mField24 = map.mField24;
        mMiniMap.mField28 = map.mField28;
        mMiniMap.mSub.mPtr04 = map.mSub.mPtr04;
        mMiniMap.mSub.mPtr08 = map.mSub.mPtr08;
        mMiniMap.mField3C = map.mField3C;
        mMiniMap.mField17C = map.mField17C;
        mMiniMap.m824[0] = map.m824[0];
        mMiniMap.m824[1] = map.m824[1];
        mMiniMap.m824[2] = map.m824[2];
        mMiniMap.m824[3] = map.m824[3];
        mMiniMap.m834[0] = map.m834[0];
        mMiniMap.m834[1] = map.m834[1];
        mMiniMap.m834[2] = map.m834[2];
        mMiniMap.m834[3] = map.m834[3];
    }

    if (mMiniMap.mReady == 0) {
        mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
        // Retail issues a second, unused MEM2 probe before the regions.
        mtl::MemManager::getHandleMEM2();
        ((UnkClass_8045F564*)&mMiniMap.m824)->createRegion(
            handle, 0x4000, &lbl_eu_804FE1FC[0x1BD], 0);
        ((UnkClass_8045F564*)&mMiniMap.m834)->createRegion(
            handle, -0x8000, &lbl_eu_804FE1FC[0x1C6], 0);

        sprintf(nameBuf, &lbl_eu_804FE1FC[0x1D4], lbl_eu_8052C740[lbl_eu_80664184 - 1]);
        mMiniMap.mFileHandle = CDeviceFile::readFile(
            mtl::MemManager::getHandleMEM2(), nameBuf,
            (IWorkEvent*)&mMiniMap, 0, 0);

        // Re-seed the loader subobject from scratch, refresh the global BDAT
        // pane-name cache for the current row, then request the map image.
        sub.mVtable = (void*)lbl_eu_8052C9F0;
        sub.mPtr04 = NULL;
        sub.mPtr08 = NULL;
        sub.mFlag0C = 0;
        sub.mFlag0D = 0;
        sub.mFlag0E = 0;
        lbl_eu_80663FB8 = NULL;
        sprintf(rowBuf, &lbl_eu_804FE1FC[0x147], lbl_eu_8052C740[lbl_eu_80664184 - 1]);
        lbl_eu_80663FB8 = getFP__FPCc(rowBuf);
        mMiniMap.mSub.mPtr04 = sub.mPtr04;
        mMiniMap.mSub.mPtr08 = sub.mPtr08;
        mMiniMap.mField3C.mFlag38 = sub.mFlag0C;
        mMiniMap.mField3C.mFlag39 = sub.mFlag0D;
        mMiniMap.mField3C.mFlag3A = sub.mFlag0E;
        func_80117734((CMMMapImg*)&mMiniMap.mSub);
    }
}

// Term: detach the render callback, wait for the VI draw to finish, cancel the
// clock's file request, free the clock layout/accessor/scratch region and the
// minimap subobject, then clear the singleton.
void CMenuMiniMap2::Term() {
    // The `if (this)` is the MWCC idiom that splits mr r4 / beq / addi +0x5c
    // for the IScnRender subobject passed to removeRenderCB.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mScnRender);
    }
    mScn->removeRenderCB(render);
    CDeviceVI::waitForDrawDone();

    mClock.mReady = 0;
    if (mClock.mFileHandle != 0) {
        CDeviceFile::cancel(mClock.mFileHandle);
        mClock.mFileHandle = 0;
    }
    if (mClock.mLayout != 0) {
        // `delete` emits its own null-check, so the single outer if gives the
        // retail two `beq`s (skip-all + skip-delete) around the vtable dtor.
        delete mClock.mLayout;
        mClock.mLayout = 0;
    }
    func_80139124(mClock.mAccessor);
    mClock.mUnk1C.func_8045F778();
    func_80118058(&mMiniMap);
    lbl_eu_80663FB0 = 0;
}

// Player view with GetPos at the retail vtable slot (+0xAC): MWCC reserves a
// two-slot dtor pair ahead of these SI-view virtuals, so GetPos needs 41
// declared entries before it (v000..v0A0).
struct MiniMapPlayer {
    virtual void v000(); virtual void v004(); virtual void v008();
    virtual void v00C(); virtual void v010(); virtual void v014();
    virtual void v018(); virtual void v01C(); virtual void v020();
    virtual void v024(); virtual void v028(); virtual void v02C();
    virtual void v030(); virtual void v034(); virtual void v038();
    virtual void v03C(); virtual void v040(); virtual void v044();
    virtual void v048(); virtual void v04C(); virtual void v050();
    virtual void v054(); virtual void v058(); virtual void v05C();
    virtual void v060(); virtual void v064(); virtual void v068();
    virtual void v06C(); virtual void v070(); virtual void v074();
    virtual void v078(); virtual void v07C(); virtual void v080();
    virtual void v084(); virtual void v088(); virtual void v08C();
    virtual void v090(); virtual void v094(); virtual void v098();
    virtual void v09C(); virtual void v0A0();
    virtual ml::CVec3* GetPos();     // vtable+0xAC
};

// ============================================================================
// func_80117C30 - per-frame minimap subobject update: while visible (states
// 2/5/7) recompute the player's floor-height band and the row's unlock flags,
// run the layout fade state machine on field_0x18, then refresh the markers.
// ============================================================================
extern "C" void __declspec(noinline) func_80117C30(CMiniMap* self) {
    if (self->mLayout0C == NULL) return;

    u8 state = self->field_0x18;
    if (state == 2 || state == 5 || state == 7) {
        // Player's height band = first BDAT floor-threshold row above player y.
        u8 band = 0;
        MiniMapPlayer* player = (MiniMapPlayer*)cf::CfGameManager::getPlayer(0);
        if (player == NULL) {
            band = 0;
        } else {
            ml::CVec3 ppos = *player->GetPos();
            u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
            f32 py = ppos.y;
            for (u8 i = 1; i <= count; i++) {
                s16 v = func_80136330(lbl_eu_80663FB8, &lbl_eu_804FE1FC[0x30], i);
                // Signed int->double cast via the 0x4330/extsh/xoris bit trick,
                // subtracting the shared retail magic so the pool label names
                // lbl_eu_80667098 instead of an anonymous TU-local constant.
                union {
                    f64 d;
                    u32 w[2];
                } conv;
                conv.w[0] = 0x43300000;
                conv.w[1] = (u32)(s32)v ^ 0x80000000;
                if (conv.d - lbl_eu_80667098 > py) {
                    band = i;
                    break;
                }
            }
        }
        u8 curBand = self->mField3C.mFlag39;
        if (band != curBand) {
            // Band changed -> re-run the fade-in to rebuild markers.
            self->field_0x18 = 4;
        } else {
            // Unlock-gated display flags for the current band; any change vs
            // the cached flag also kicks a fade-in.
            u16 unlockA = func_80136254(lbl_eu_80663FB8, &lbl_eu_804FE1FC[0x157],
                                         curBand);
            u16 unlockB = func_80136254(lbl_eu_80663FB8, &lbl_eu_804FE1FC[0x15D],
                                         curBand);
            u8 gateC = (u8)func_801361E8((u32)lbl_eu_80663FB8,
                                         &lbl_eu_804FE1FC[0x164], curBand);
            u8 newFlag;
            if (unlockA != 0 && (u32)func_8009CF8C(0x20) >= unlockA) {
                if (unlockB == 0) {
                    newFlag = 1;
                } else if ((u32)func_8009CF8C(unlockB + 0x220) >= gateC) {
                    newFlag = 2;
                } else {
                    newFlag = 1;
                }
            } else if (unlockB == 0) {
                newFlag = 0;
            } else if ((u32)func_8009CF8C(unlockB + 0x220) >= gateC) {
                newFlag = 1;
            } else {
                newFlag = 0;
            }
            if (newFlag != self->mField3C.mFlag3A) {
                self->field_0x18 = 4;
            }
        }
    }

    switch (self->field_0x18) {
    case 1:
        // Fade-in done -> switch to the loop animation.
        if (func_80137444(self->mAnimTrans0, lbl_eu_806670A4) != 0) {
            self->field_0x18 = 2;
            self->field_0x19 = 1;
            self->mLayout0C->SetAnimationEnable(self->mAnimTrans0, false);
            self->mLayout0C->SetAnimationEnable(self->mAnimTrans1, true);
            self->mLayout0C->Animate(0);
        }
        break;
    case 3:
        // Fade-out done -> idle.
        if (func_80137510(self->mAnimTrans0, lbl_eu_806670A4) != 0) {
            self->field_0x18 = 0;
            self->field_0x19 = 1;
        }
        break;
    case 4:
        // Fade-in finished: bind the shared 'timg' picture onto 'panemapmark'.
        if (func_80137444(self->mAnimTrans1, lbl_eu_806670A4) != 0) {
            self->field_0x18 = 6;
            void* res =
                self->mAccessor->GetResource(0x74696D67, &lbl_eu_804FE1FC[0x1E7], 0);
            if (res != NULL) {
                func_80137E7C(self->mLayout0C, &lbl_eu_804FE1FC[0x1FA], res);
            }
        }
        break;
    case 5:
        if (func_80137510(self->mAnimTrans1, lbl_eu_806670A4) != 0) {
            self->field_0x18 = 2;
        }
        break;
    case 6:
        // Kick off the map-image load request for the current row.
        self->field_0x18 = 7;
        func_80117734((CMMMapImg*)&self->mSub);
        break;
    case 7:
        // Map image ready -> show it (uses the cached timg when present).
        if (self->mField3C.mFlag38 == 0) break;
        self->field_0x18 = 5;
        {
            void* res = self->mSub.mPtr08;
            if (res == NULL) {
                res = self->mAccessor->GetResource(0x74696D67,
                                                   &lbl_eu_804FE1FC[0x1E7], 0);
                if (res == NULL) break;
            }
            func_80137E7C(self->mLayout0C, &lbl_eu_804FE1FC[0x1FA], res);
        }
        break;
    }

    func_80118854((MiniMapSelf*)self);
    func_8011B05C((MiniMapSelf*)self);
    self->mLayout0C->Animate(0);
}

// Move: per-frame minimap process update. Early-exit gate chain, then a
// four-state machine on mField8D4 (0 off / 1 fade-in / 2 active / 3 fade-out)
// that arms the clock+minimap animations, followed by the clock layout state
// machine (mClock.field_0x18) that advances the clock hand to the game time.
void CMenuMiniMap2::Move() {
    // Gate chain: game is rendering, no menu overlay, no cutscene - two
    // short-circuit OR guards so MWCC emits the retail branch alternation
    // (first test bne-exit, second test beq-continue / b-exit).
    if (CTaskGame::getInstance()->func_800426F0() ||
        (lbl_eu_80663E28 & 0x200000)) return;
    if (func_80242354() || func_80251550()) return;

    switch (mField8D4) {
    case 0: {
        // Idle -> fade in: no menu overlay, clock and minimap loaded, and the
        // clock-show flag (mField38) plus minimap ready bit both set.
        if (lbl_eu_80663E24 & 0xBFE40000) break;
        if (mClock.mReady == 0) break;
        if ((mMiniMap.mField3C.mFlag38 != 0 ? mMiniMap.mReady : 0) == 0) break;
        // Load the cached resource first (retail hoists lwz r5,0xc4 above the
        // store sequence); the fresh GetResource result merges into it.
        u8* res = mMiniMap.mSub.mPtr08;
        mField8D4 = 1;
        mClock.field_0x18 = 1;
        mClock.field_0x19 = 0;
        mMiniMap.field_0x18 = 1;
        mMiniMap.field_0x19 = 0;
        // Short-circuit && so MWCC branches past the res-merge copy on the
        // cached-resource path (retail: bne merge; fetch; beq exit; mr).
        if (res == 0 &&
            (res = (u8*)mMiniMap.mAccessor->GetResource(0x74696D67,
                                                        &lbl_eu_804FE1FC[0x1E7],
                                                        0)) == 0)
            break;
        func_80137E7C(mMiniMap.mLayout0C, &lbl_eu_804FE1FC[0x1FA], res);
        break;
    }
    case 1:
        // Fade-in done once both sub-layouts report finished.
        if (mClock.field_0x19 != 0 && mMiniMap.field_0x19 != 0) {
            mField8D4 = 2;
        }
        break;
    case 2:
        // Active -> fade out: camera is in the map mode and event flag 0x1000
        // is cleared, so restart both animations.
        cf::CfGameManager::getInstance();
        if (func_8006EF04(0x10000000) != 0) {
            mField8D4 = 3;
            mClock.field_0x18 = 3;
            mClock.field_0x19 = 0;
            if (mClock.mLayout != 0) {
                mClock.mLayout->SetAnimationEnable(mClock.mAnimTrans1, 0);
                mClock.mLayout->SetAnimationEnable(mClock.mAnimTrans0, 1);
                mClock.mLayout->Animate(0);
            }
            mMiniMap.field_0x18 = 3;
            mMiniMap.field_0x19 = 0;
            if (mMiniMap.mLayout0C != 0) {
                mMiniMap.mLayout0C->SetAnimationEnable(mMiniMap.mAnimTrans1, 0);
                mMiniMap.mLayout0C->SetAnimationEnable(mMiniMap.mAnimTrans0, 1);
                mMiniMap.mLayout0C->Animate(0);
            }
        }
        break;
    case 3:
        // Fade-out done -> back to idle.
        if (mClock.mReady != 0) {
            if ((mMiniMap.mField3C.mFlag38 != 0 ? mMiniMap.mReady : 0) != 0) {
                mField8D4 = 0;
            }
        }
        break;
    }

    // Clock layout animation state machine (only while the clock layout is
    // attached). State 1 fades in and snaps the hand to the game time,
    // state 2 keeps the hand tracking time, state 3 fades out. The advance
    // body is hand-duplicated (retail inlines it twice, each copy with its
    // own u16 a/b stack pair: case 1 at sp+8/0xa, case 2 at sp+0xc/0xe).
    if (mClock.mLayout != 0) {
        switch (mClock.field_0x18) {
        case 1: {
            u16 a;
            u16 b;
            if (func_80137444(mClock.mAnimTrans0, lbl_eu_806670A4) != 0) {
                mClock.field_0x18 = 2;
                mClock.field_0x19 = 1;
                mClock.mLayout->SetAnimationEnable(mClock.mAnimTrans0, 0);
                mClock.mLayout->SetAnimationEnable(mClock.mAnimTrans1, 1);
                mClock.mLayout->Animate(0);
                a = 0;
                b = 0;
                func_8006A234(&a, &b);
                f32 frame =
                    lbl_eu_80661E48 * (f32)(a * 60 + b) + lbl_eu_80663FB4;
                f32 maxF = (f32)((u16)mClock.mAnimTrans1->GetFrameSize());
                if (frame > maxF) frame -= maxF;
                mClock.mAnimTrans1->SetFrame(frame);
            }
            break;
        }
        case 2: {
            u16 a;
            u16 b;
            a = 0;
            b = 0;
            func_8006A234(&a, &b);
            f32 frame = lbl_eu_80661E48 * (f32)(a * 60 + b) + lbl_eu_80663FB4;
            f32 maxF = (f32)((u16)mClock.mAnimTrans1->GetFrameSize());
            if (frame > maxF) frame -= maxF;
            mClock.mAnimTrans1->SetFrame(frame);
            break;
        }
        case 3:
            if (func_80137510(mClock.mAnimTrans0, lbl_eu_806670A4) != 0) {
                mClock.field_0x18 = 0;
                mClock.field_0x19 = 1;
            }
            break;
        }
        mClock.mLayout->Animate(0);
    }

    func_80117C30(&mMiniMap);
    if (lbl_eu_80663E24 & 0xBFE40000) return;
    if (func_801AC124()) return;
    // Active state: Classic/Wiimote A dismisses the minimap.
    if (mField8D4 == 2) {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        int btn;
        // Pre-mangled form with an int arg: retail emits `li r3,-1` before the
        // call, which the cf::CfGameManager s16 inline wrapper would drop.
        if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
            btn = (pad->mPressedButtonFlags >> 10) & 1;
        } else {
            btn = (pad->mPressedButtonFlags >> 6) & 1;
        }
        if (btn) func_80134538();
    }
    func_8013ACFC();
    if (mField55 != 0) mField54 = 1;
}

// cbRenderBefore: draw the clock layout and the minimap's sub-layout under a
// fresh DrawInfo when the game is in a renderable state.
void CMenuMiniMap2::cbRenderBefore() {
    // Single OR so MWCC emits the short-circuit branches: A -> bne exit,
    // B -> beq continue / b exit (same shape as CSystemWindow::Move).
    if (CTaskGame::getInstance()->func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (!func_8013BE50()) return;
    if (!func_8013BE88()) return;
    if (getUnk80664658()->field_214 & 0x100000) return;
    if (lbl_eu_80663E24 & 0xAFA40000) return;
    if (func_801AC124()) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250(&drawInfo);
    if (mClock.mLayout != 0) {
        func_80137038(mClock.mLayout, &drawInfo, 0, 1);
    }
    if (mMiniMap.mLayout0C != 0 && mMiniMap.mFlag1B != 0) {
        func_80137038(mMiniMap.mLayout0C, &drawInfo, 0, 1);
    }
}

// Singleton factory for the minimap process (retail unmangled symbol; the
// CMenuMiniMap2 ctor body is inlined here). Allocates the 0x8D8-byte object,
// fills the CProcess storage + IUIWindow region by hand (temp vtable, null PMF
// triple copy, composite vtable + sub-vtable slots), hand-builds CMMClock and
// CMiniMap, registers under `parent` and returns the stored instance.
CMenuMiniMap2* __ct__8011C1B8(CProcess* parent, CScn* scene) {
    if (lbl_eu_80663FB0 != 0) {
        return 0;
    }
    CMenuMiniMap2CreateView* obj = (CMenuMiniMap2CreateView*)mtl::MemManager::allocate(
        sizeof(CMenuMiniMap2CreateView), CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        __ct__8CProcessFv((CProcess*)obj);

        // Temp (CProcess) vtable first, then the null PMF triple copy, then
        // the composite vtable and the +0x24/+0xAC sub-vtable slots.
        obj->mVtable10 = (u32)lbl_eu_8052BF70;
        u32* ptmf = __ptmf_null;
        obj->mPtMf3C[0] = ptmf[0];
        obj->mPtMf3C[1] = ptmf[1];
        obj->mPtMf3C[2] = ptmf[2];
        obj->mPtMf48[0] = ptmf[0];
        obj->mPtMf48[1] = ptmf[1];
        obj->mPtMf48[2] = ptmf[2];
        obj->mField54 = 0;
        obj->mField55 = 0;
        obj->mVtable10 = (u32)lbl_eu_8052C858;
        obj->mFn58 = (u32)lbl_eu_8052C858 + 0x24;
        obj->mScnRender = (u32)lbl_eu_8052C858 + 0xAC;
        obj->mScn = scene;

        // Hand-build the CMMClock: vtable + zeroed fields + flag bytes, then
        // the embedded UnkClass region and the CMiniMap subobject.
        *(u32*)&obj->mClock = (u32)lbl_eu_8052CA88;
        obj->mClock.mFileHandle = 0;
        obj->mClock.mAccessor = 0;
        obj->mClock.mLayout = 0;
        obj->mClock.mAnimTrans0 = 0;
        obj->mClock.mAnimTrans1 = 0;
        obj->mClock.field_0x18 = 0;
        obj->mClock.field_0x19 = 1;
        obj->mClock.mReady = 0;
        new (&obj->mClock.mUnk1C) UnkClass_8045F564();
        __ct__CMiniMap(&obj->mMiniMap);
        obj->mField8D4 = 0;
    }
    lbl_eu_80663FB0 = (u32)obj;
    ((CProcess*)obj)->Regist(parent, false);
    return (CMenuMiniMap2*)lbl_eu_80663FB0;
}

extern u32 lbl_eu_80663F20;
extern u32 lbl_eu_80663FB0;
s32 func_8011C2E8() {
    u32 v = lbl_eu_80663FB0;
    return ((-v) | v) >> 31;
}

// Cast-only SI for the minimap sub-object virtual calls (slots 0x2C/0x38)
struct MiniMapIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void vf2C(void* a, u32 b);
    virtual void _v030(); virtual void _v034(); virtual void vf38(void* a);
};
extern "C" s32 func_8011C2FC(void) {
    void* g = (void*)lbl_eu_80663FB0;
    if (!g) return;
    ((u8*)g)[0x8d4] = 3;
    ((u8*)g)[0x7c] = 3;
    ((u8*)g)[0x7d] = 0;
    if (*(void**)((u8*)g + 0x70)) {
        ((MiniMapIf*)*(void**)((u8*)g + 0x70))->vf2C(*(void**)((u8*)g + 0x78), 0);
        ((MiniMapIf*)*(void**)((u8*)g + 0x70))->vf2C(*(void**)((u8*)g + 0x74), 1);
        ((MiniMapIf*)*(void**)((u8*)g + 0x70))->vf38(0);
    }
    ((u8*)g)[0xa8] = 3;
    ((u8*)g)[0xa9] = 0;
    if (*(void**)((u8*)g + 0x9c)) {
        ((MiniMapIf*)*(void**)((u8*)g + 0x9c))->vf2C(*(void**)((u8*)g + 0xa4), 0);
        ((MiniMapIf*)*(void**)((u8*)g + 0x9c))->vf2C(*(void**)((u8*)g + 0xa0), 1);
        ((MiniMapIf*)*(void**)((u8*)g + 0x9c))->vf38(0);
    }
}

void func_8011C400()
{
    if (lbl_eu_80663FB0 != 0)
    {
        *((unsigned char*)lbl_eu_80663FB0 + 0x54) = 1;
    }
}

// Offset-adjusted dispatch thunks: the CMenuMiniMap2 subobject sits at self-0x5C.
void func_8011C43C(void* self) { ((void(*)(void*))cbRenderBefore__13CMenuMiniMap2Fv)((char*)self - 0x5c); }

void func_8011C444(void* self) { ((void(*)(void*))__dt__13CMenuMiniMap2Fv)((char*)self - 0x5c); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8011C418() {
    lbl_eu_80663FB4 = lbl_eu_806670CC * (lbl_eu_806670A0 * lbl_eu_80661E48);
}

// func_80115FD0 - initialize the 3x5 minimap table object (retail unmangled
// name). Sets the scalar head/tail fields, resolves the BDAT table pointer via
// getFP, then zeroes the u16/u32/byte columns row by row (inner 5-wide loops
// fully unrolled by MWCC, outer 3-row loop kept as the counted loop).
MiniMapTable* func_80115FD0(MiniMapTable* self) {
    f32 zero = lbl_eu_80667090;
    self->field_00 = 0;
    self->field_08 = 0;
    self->field_0C = zero;
    self->field_13C = 0;
    func_8003AA34();
    self->field_04 = (u32)getFP__FPCc(lbl_eu_804FE1FC);
    // Inner 5-wide sweep: the two flat flag columns share one running byte
    // row offset, so MWCC recomputes their address as base+index with the
    // column delta in the displacement, while the u16/pane rows get pointer
    // induction vars (+0xA / +0x14 per row).
    u32 idx = 0;
    for (u32 i = 0; i < 3; i++) {
        for (u32 j = 0; j < 5; j++) {
            self->field_10[i][j] = 0;
            self->field_100[i][j] = 0;
            self->field_2E[idx + j] = 0;
            self->field_3D[idx + j] = 0;
        }
        idx += 5;
    }
    return self;
}
// ============================================================================
// func_801160A8 - build one marker picture pane per BDAT row whose 'map kind'
// column matches the current row index: resolve its texture, create/name the
// picture, reset its SRT to the zero constant, then record pane/id/position/
// flag into the marker tables (max 15 entries; hitting 16 ends the scan).
// ============================================================================
void func_801160A8(MiniMapTable* self, void* table, void* layout, f32 scale) {
    self->field_08 = (u32)layout;
    self->field_04 = (u32)table;
    self->field_0C = scale;

    // Marker parent pane, looked up once through the layout's pane manager.
    self->field_13C = (nw4r::lyt::Pane*)((MiniMapLayout*)layout)->mgr->v03C(&lbl_eu_804FE1FC[0xd], 1);

    func_8003AA34();
    u16 count = func_8003B1EC((void*)self->field_04);
    // Loaded after the row count (retail keeps the constant off the fast path).
    f32 zero = lbl_eu_80667090;
    u8 n = 0;
    for (u16 i = 1; i <= count; i++) {
        if ((int)lbl_eu_80664184 != (int)(u8)func_801361E8((u32)self->field_04,
                                   &lbl_eu_804FE1FC[0x16], i))
            continue;
        if ((u8)func_801361E8((u32)self->field_04,
                              &lbl_eu_804FE1FC[0x1c], i) == 2)
            continue;

        char buf[0x20];
        sprintf(buf, &lbl_eu_804FE1FC[0x25], (u32)i);
        void* texName = func_8013902C(0xe);
        nw4r::lyt::Pane* pane;
        ml::CVec3 pos;
        if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0) == 0) {
            pane = 0;
        } else {
            pane = (nw4r::lyt::Pane*)createPicture__10CLibLayoutFv();
            SetName__Q34nw4r3lyt4PaneFPCc(pane, buf);
            *(f32*)((u8*)pane + 0x2C) = zero;
            *(f32*)((u8*)pane + 0x30) = zero;
            // Dead origin record (retail stores it to the frame).
            pos.x = zero;
            pos.y = zero;
            pos.z = zero;
            *(f32*)((u8*)pane + 0x34) = zero;
            func_80137C1C(pane, -1);            *(u8*)((u8*)pane + 0xBB) = (*(u8*)((u8*)pane + 0xBB) & 0xFE) | 1;
        }
        if (pane == 0) continue;
        self->field_100[0][n] = pane;
        self->field_10[0][n] = i;
        func_80141DC4(&pos, i);
        self->field_4C[0][n] = pos;
        self->field_3D[n] = 0;
        if (n >= 15) return;
        n++;
    }
}
// func_8011628C - per-frame marker placement: for each live MiniMapTable
// marker decide (by the player height band vs the marker band) whether the
// marker pane is attached to the layout root, then pin it to its grid cell.
// Retail keeps the player as (container-0x3E9C) so the CfObjectMove subobject
// at +0x3E9C is addressed with a constant displacement.
extern "C" void __declspec(noinline) func_8011628C(void* self, u32 row) {
    MiniMapTable* table = (MiniMapTable*)self;
    if (table->field_08 == 0) return;

    MiniMapObj* player = (MiniMapObj*)cf::CfGameManager::getPlayer(0);
    if (player != 0) {
        player = (MiniMapObj*)((u8*)player - 0x3E9C);
    }
    if (player == 0) return;

    // Find the player's height band (first row whose threshold exceeds it).
    u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
    u8 best = 0;
    for (u8 i = 1; i <= count; i++) {
        s16 v = func_80136330(lbl_eu_80663FB8, &lbl_eu_804FE1FC[0x30], i);
        if ((f32)v > ((MiniMapObj*)((u8*)player + 0x3E9C))->GetPos()->y) {
            best = i;
            break;
        }
    }

    f32 scale = lbl_eu_80667094 * table->field_0C;
    for (u8 j = 0; j < 16; j++) {
        u16 markerId = table->field_10[0][j];
        if (markerId == 0) return;
        if (table->field_100[0][j] == 0) continue;
        if (func_8009CF8C(markerId + 0x20c8) == 0) continue;

        // Marker in the player's band -> the pane is shown and positioned.
        bool found = false;
        for (u8 k = 1; k <= count; k++) {
            s16 v = func_80136330(lbl_eu_80663FB8, &lbl_eu_804FE1FC[0x30], k);
            if ((f32)v > table->field_4C[0][j].y) {
                if (best == k) found = true;
                break;
            }
        }

        // diff via operator-: VEC3Sub temp + set() copy, so MWCC allocates the
        // temp as a temporary (retail: temp@sp+8, out@sp+0x20).
        ml::CVec3 diff = *((MiniMapObj*)((u8*)player + 0x3E9C))->GetPos() -
                         table->field_4C[0][j];
        f32 lenSq = nw4r::math::VEC3LenSq(diff);
        if (lenSq > scale * scale || !found) {
            // Out of range: detach the marker pane from the root.
            if (table->field_3D[j] != 0) {
                table->field_13C->RemoveChild(table->field_100[0][j]);
                table->field_3D[j] = 0;
            }
        } else {
            // In range: attach the pane on first use, then place it.
            if (table->field_3D[j] == 0) {
                table->field_13C->PrependChild(table->field_100[0][j]);
                table->field_100[0][j]->SetVisible(true);
                table->field_3D[j] = 1;
            }
            nw4r::lyt::Pane* pane = table->field_100[0][j];
            s32 gx = -(s32)(diff.x / table->field_0C);
            s32 gz = (s32)(diff.z / table->field_0C);
            pane->SetSRTElement(0, (f32)gx);
            pane->SetSRTElement(1, (f32)gz);
            // Dead position record (retail stores it to the frame at sp+0x14).
            ml::CVec3 pos;
            pos.x = (f32)gx;
            pos.y = (f32)gz;
            pos.z = 0.0f;
            pane->SetSRTElement(2, 0.0f);
        }
    }
}
// ============================================================================
// func_80116670 - initialize the gimmick-view tables (minimap+0x17C) from the
// BDAT gimmick table: stash table/layout/scale, resolve the marker parent pane
// via the layout's pane manager (+0x10), then fill the per-row enable/id/valid
// entries for rows 1..count and seed both name-lookup tables.
// ============================================================================
extern "C" void func_80116670(CMiniMapGimmickView* self, u32 table, void* layout, f32 scale) {
    self->field_0x00 = (const char*)table;
    self->field_0x04 = (u32)layout;
    self->field_0x08 = scale;

    // Marker parent pane, looked up once through the layout's pane manager.
    self->field_0x14 = (u32)((MiniMapLayout*)layout)->mgr->v03C(&lbl_eu_804FE1FC[0x37], 1);

    self->field_0x0C = (s32)func_8003B1EC((void*)table);

    for (s32 i = 0; i < self->field_0x0C; i++) {
        self->field_0x18[i] = func_801361E8((u32)self->field_0x00,
                                            &lbl_eu_804FE1FC[0x40], i + 1);
        if (self->field_0x18[i] != 0) {
            self->field_0xE0[i] = func_80136254((void*)self->field_0x00,
                                                &lbl_eu_804FE1FC[0x45], i + 1);
            u16 id = func_80136254((void*)self->field_0x00,
                                   &lbl_eu_804FE1FC[0x4C], i + 1);
            void* entry = lbl_eu_80573D18[func_80138138(id)];
            u8 valid = (func_801361E8((u32)entry, &lbl_eu_804FE1FC[0x55], id) != 0);
            self->field_0x270[i] = valid;
            if (!valid) {
                // Reassign id from the alternate column and retry the same
                // entry lookup (retail reuses the masked register).
                id = func_80136254((void*)self->field_0x00,
                                   &lbl_eu_804FE1FC[0x5F], i + 1);
                if (id != 0) {
                    self->field_0x270[i] =
                        (func_801361E8((u32)entry, &lbl_eu_804FE1FC[0x55], id) != 0);
                }
            }
        }
    }

    for (s32 j = 0; j < self->field_0x0C; j++) {
        u8 res = (u8)func_80138574(self->field_0x00, j + 1);
        self->field_0x338[j] = res;
        self->field_0x400[j] = res;
    }
}
// ============================================================================
// func_801168A0 - update gimmick-view marker rows from the player's position.
// For each live row: diff = player pos - row pos. Rows with the update flag
// set are radius-clamped (Warning/FrSqrt idiom shared with the Move sections);
// rows without it just drop out of range (clear the entry's enable bit).
// Both paths then store integer-truncated grid coords into the pane record.
struct MiniMapSubObj {
    virtual void v000(); virtual void v004(); virtual void v008();
    virtual void v00C(); virtual void v010(); virtual void v014(); virtual void v018();
    virtual void v01C(); virtual void v020(); virtual void v024(); virtual void v028();
    virtual void v02C(); virtual void v030(); virtual void v034(); virtual void v038();
    virtual void v03C(); virtual void v040(); virtual void v044(); virtual void v048();
    virtual void v04C(); virtual void v050(); virtual void v054(); virtual void v058();
    virtual void v05C(); virtual void v060(); virtual void v064(); virtual void v068();
    virtual void v06C(); virtual void v070(); virtual void v074(); virtual void v078();
    virtual void v07C(); virtual void v080(); virtual void v084(); virtual void v088();
    virtual void v08C(); virtual void v090(); virtual void v094(); virtual void v098();
    virtual void v09C(); virtual void v0A0();
    virtual ml::CVec3* GetPos();     // vtable+0xAC
};

// ============================================================================
// func_80117734 - map-image loader request (retail symbol unmangled).
// Cancels any pending load and frees the previous buffer, selects the BDAT row
// whose floor height is above the player's y, resolves the unlock-gated
// resource variant, then requests the formatted image path into MEM2.
// ============================================================================
extern "C" void func_80117734(CMMMapImg* self) {
    func_801390E0(&self->mFileHandle);
    if (self->mData != NULL) {
        deallocate__Q23mtl10MemManagerFPv(self->mData);
        self->mData = NULL;
    }

    u8 best = 0;
    MiniMapSubObj* player = (MiniMapSubObj*)cf::CfGameManager::getPlayer(0);
    if (player == NULL) {
        best = 0;
    } else {
        ml::CVec3 ppos = *player->GetPos();
        u8 count = (u8)func_8003B1EC(lbl_eu_80663FB8);
        f32 py = ppos.y;
        for (u8 i = 1; i <= count; i++) {
            s16 v = func_80136330(lbl_eu_80663FB8, &lbl_eu_804FE1FC[0x30], i);
            // Signed int->double cast: MWCC emits the retail 0x4330/extsh/
            // xoris store pair plus fsubs against lbl_eu_80667098.
            if ((f64)v > py) {
                best = i;
                break;
            }
        }
    }
    self->mRow = best;
    if (best == 0) {
        self->mRow = 1;
    }

    char* strings = lbl_eu_804FE1FC;
    u8 row = self->mRow;
    u16 va = func_80136254(lbl_eu_80663FB8, strings + 0x157, row);
    u16 vb = func_80136254(lbl_eu_80663FB8, strings + 0x15d, row);
    u32 vc = func_801361E8((u32)lbl_eu_80663FB8, strings + 0x164, row);

    // Unlock-gated resource variant: 2 needs both progress caps met,
    // 1 only the second, 0 neither.
    s32 mode;
    if (va != 0 && func_8009CF8C(0x20) >= va) {
        if (vb == 0 || func_8009CF8C(vb + 0x220) < vc) {
            mode = 1;
        } else {
            mode = 2;
        }
    } else {
        if (vb != 0 && func_8009CF8C(vb + 0x220) >= vc) {
            mode = 1;
        } else {
            mode = 0;
        }
    }

    const char* variant;
    switch (mode) {
    case 1:
        variant = strings + 0x170;
        break;
    case 2:
        variant = strings + 0x178;
        break;
    default:
        variant = strings + 0x169;
        break;
    }

    u16 nameId = func_80136254(lbl_eu_80663FB8, variant, self->mRow);
    ml::FixStr<128> path;
    path.format(strings + 0x180, func_80138F78(nameId));

    self->mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), path.mString,
        reinterpret_cast<IWorkEvent*>(self), 0, 0);
    self->mReady = 0;
}

extern "C" void __declspec(noinline) func_801168A0(CMiniMapGimmickView* self) {
    // The position source is the +0x3E9C member of the player object; retail
    // null-tests the container-of pointer before dereferencing.
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player != NULL) {
        player = (cf::CfObjectMove*)((u8*)player - 0x3E9C);
    }
    if (player == NULL) {
        return;
    }

    ml::CVec3* pposPtr = ((MiniMapSubObj*)((u8*)player + 0x3E9C))->GetPos();

    // Retail computes the scale-derived constants between the position fetch
    // and the copy into locals.
    f32 gridStep = self->field_0x08 * lbl_eu_80667094;   // one grid cell (world units)

    f32 radius = self->field_0x08 * lbl_eu_806670A0;     // clamp radius (flagged rows)

    ml::CVec3 ppos;
    ppos.x = pposPtr->x;
    ppos.y = pposPtr->y;
    ppos.z = pposPtr->z;

    for (u32 i = 0; i < self->field_0x6A4; i++) {
        ml::CVec3 diff = ppos - self->field_0x5D4[i];

        if (self->field_0x694[i] != 0) {
            f32 mag = PSVECMag((const Vec*)&diff);
            if (mag > radius) {
                f32 len2 = nw4r::math::VEC3LenSq(diff);
                // Negated form compiles to the cror/beq branch shape.
                if (!(len2 < lbl_eu_80667090)) {
                    Warning__Q24nw4r2dbFPCciPCce(
                        (const char*)lbl_eu_80526324, 0x273,
                        (const char*)lbl_eu_80526300);
                }
                f32 len = (len2 <= lbl_eu_80667090)
                              ? lbl_eu_80667090
                              : len2 * FrSqrt__Q24nw4r4mathFf(len2);
                f32 k = radius / len;
                diff.x *= k;
                diff.z *= k;
            }
        } else if (nw4r::math::VEC3LenSq(diff) > gridStep * gridStep) {
            // Row fell off the visible grid: hide its pane record and skip.
            self->field_0x594_entries[i]->field_BB &= 0xFE;
            continue;
        }

        CMMMapEntry* entry = self->field_0x594_entries[i];
        s32 gx = (s32)(diff.x / self->field_0x08);
        s32 gz = (s32)(diff.z / self->field_0x08);
        f32 fgx = (f32)gx;
        f32 fgz = (f32)gz;
        f32 zero = lbl_eu_80667090;
        entry->field_2C = fgx;
        entry->field_30 = fgz;
        entry->field_34 = zero;
    }
}
// func_80116B40 - rebuild the gimmick-view marker panes each frame. For every
// enabled row (enable array selected by field_0x590) dispatch on its type byte:
// scan the matching object source (two global lists, the case-2 element array,
// a BDAT position probe, or the global gimmick table), find the first entry
// whose id and height band match the player's band, and record its position.
// For rows that produced a record, create/name a 'timg' picture pane (kind
// selects the resource name; 0 skips creation) and prepend it to the layout.
extern "C" void __declspec(noinline) func_80116B40(void* self) {
    char* strings = lbl_eu_804FE1FC;
    CMiniMapGimmickView* view = (CMiniMapGimmickView*)self;
    if (view->field_0x04 == 0) return;

    MiniMapObj* player = (MiniMapObj*)cf::CfGameManager::getPlayer(0);
    if (player == NULL) return;

    u32 collapsed = view->field_0x590;
    view->field_0x10 = 0;
    view->field_0x590 = (collapsed == 0);
    view->field_0x6A4 = 0;

    // Clock pane lookup through the BDAT table object's pane manager
    // (vtable+0x3C); flag it visible up front.
    MiniMapLayoutMgr* tblMgr = *(MiniMapLayoutMgr**)((u8*)view->field_0x04 + 0x10);
    nw4r::lyt::Pane* clockPane = (nw4r::lyt::Pane*)tblMgr->v03C(&strings[0x69], 1);
    if (clockPane != NULL) {
        *(u8*)((u8*)clockPane + 0xBB) = (*(u8*)((u8*)clockPane + 0xBB) & 0xFE) | 1;
    }

    // Player height band: first row whose threshold exceeds the player's y.
    u8 rowCount = (u8)func_8003B1EC(lbl_eu_80663FB8);
    u32 band = 0;
    for (u32 k = 1; k <= rowCount; k++) {
        s16 v = func_80136330(lbl_eu_80663FB8, strings + 0x30, k);
        ml::CVec3* pppos = ((MiniMapObj*)((u8*)player + 0x3E9C))->GetPos();
        if ((f64)v - lbl_eu_80667098 > pppos->y) {
            band = k;
            break;
        }
    }

    char buf[0x20];
    for (s32 i = 0; i < view->field_0x0C; i++) {
        u8* enArr = (view->field_0x590 != 0) ? view->field_0x400 : view->field_0x338;
        if (enArr[i] == 0) continue;
        if (view->field_0x18[i] == 0) continue;

        s16 dispId = view->field_0xE0[i];
        bool found = false;
        switch (view->field_0x18[i]) {
        case 1: {
            // Global move-object list: match display id (+ valid check when
            // the row is flagged), then require the object to sit in the
            // player's height band.
            MiniMapList* list = (MiniMapList*)func_800B6BEC();
            for (MiniMapListNode* node = list->head->next; node != list->head;
                 node = node->next) {
                MiniMapObj* o = (MiniMapObj*)node->object;
                if (o->m8C != (u16)dispId) continue;
                if (view->field_0x270[i] != 0 || o->v160() != 0) {
                    ml::CVec3* opos;
                    bool hit = false;
                    for (u32 k = 1; k <= rowCount; k++) {
                        s16 v = func_80136330(lbl_eu_80663FB8, strings + 0x30, k);
                        opos = o->GetPos();
                        if ((f64)v - lbl_eu_80667098 > opos->y) {
                            if (band == k) hit = true;
                            break;
                        }
                    }
                    if (!hit) break;
                    u32 cnt = view->field_0x6A4;
                    view->field_0x5D4[cnt].x = opos->x;
                    view->field_0x5D4[cnt].y = opos->y;
                    view->field_0x5D4[cnt].z = opos->z;
                    sprintf(buf, strings + 0x73, o->m74);
                    found = true;
                    break;
                }
            }
            break;
        }
        case 2: {
            // Second list, filtered by flags bits 0x10000/0x20000.
            MiniMapList* list = (MiniMapList*)func_800B6C58();
            for (MiniMapListNode* node = list->head->next; node != list->head;
                 node = node->next) {
                MiniMapObj* o = (MiniMapObj*)node->object;
                u32 flg = o->m64;
                if (!(flg & 0x20000) && !(flg & 0x10000)) continue;
                if (o->m8C != (u16)dispId) continue;
                if (view->field_0x270[i] != 0 || o->v160() != 0) {
                    ml::CVec3* opos;
                    bool hit = false;
                    for (u32 k = 1; k <= rowCount; k++) {
                        s16 v = func_80136330(lbl_eu_80663FB8, strings + 0x30, k);
                        opos = o->GetPos();
                        if ((f64)v - lbl_eu_80667098 > opos->y) {
                            if (band == k) hit = true;
                            break;
                        }
                    }
                    if (!hit) break;
                    u32 cnt = view->field_0x6A4;
                    view->field_0x5D4[cnt].x = opos->x;
                    view->field_0x5D4[cnt].y = opos->y;
                    view->field_0x5D4[cnt].z = opos->z;
                    sprintf(buf, strings + 0x7d, o->m74);
                    found = true;
                    break;
                }
            }
            break;
        }
        case 3: {
            // Fixed-stride element array (count word at base+0x9800).
            MiniMapCase2Elem* arr = (MiniMapCase2Elem*)func_80193804();
            s32 count = *(s32*)((u8*)arr + 0x9800);
            for (MiniMapCase2Elem* el = arr; el < arr + count; el++) {
                if (el->m1C != (u16)dispId) continue;
                bool hit = false;
                for (u32 k = 1; k <= rowCount; k++) {
                    s16 v = func_80136330(lbl_eu_80663FB8, strings + 0x30, k);
                    if ((f64)v - lbl_eu_80667098 > el->y) {
                        if (band == k) hit = true;
                        break;
                    }
                }
                if (!hit) break;
                u32 cnt = view->field_0x6A4;
                view->field_0x5D4[cnt].x = el->x;
                view->field_0x5D4[cnt].y = el->y;
                view->field_0x5D4[cnt].z = el->z;
                sprintf(buf, strings + 0x89, el->m1C);
                found = true;
                break;
            }
            break;
        }
        case 4: {
            // BDAT position probe: band test against the probed y.
            bool hit = false;
            ml::CVec3 probe;
            for (u32 k = 1; k <= rowCount; k++) {
                s16 v = func_80136330(lbl_eu_80663FB8, strings + 0x30, k);
                func_80141DC4(&probe, dispId);
                if ((f64)v - lbl_eu_80667098 > probe.y) {
                    if (band == k) hit = true;
                    break;
                }
            }
            if (!hit) break;
            ml::CVec3 pos;
            func_80141DC4(&pos, dispId);
            u32 cnt = view->field_0x6A4;
            view->field_0x5D4[cnt].x = pos.x;
            view->field_0x5D4[cnt].y = pos.y;
            view->field_0x5D4[cnt].z = pos.z;
            sprintf(buf, strings + 0x25, i + 1);
            found = true;
            break;
        }
        case 5: {
            // Per-row BDAT table: three coordinate columns compared against
            // the band threshold column.
            char* tbl = (char*)getFP__FPCc(strings + 0x93);
            if (func_80136254(tbl, strings + 0xa2, dispId) == 0) break;
            s32 cx = func_80136330(tbl, strings + 0xa9, dispId);
            s32 cy = func_80136330(tbl, strings + 0xae, dispId);
            s32 cz = func_80136330(tbl, strings + 0xb3, dispId);
            bool hit = false;
            for (u32 k = 1; k <= rowCount; k++) {
                s16 v = func_80136330(lbl_eu_80663FB8, strings + 0x30, k);
                if (v > cy) {
                    if (band == k) hit = true;
                    break;
                }
            }
            if (!hit) break;
            u32 cnt = view->field_0x6A4;
            // int -> float via the shared 0x4330 double-magic constant.
            view->field_0x5D4[cnt].x = (f32)((f64)cx - lbl_eu_80667098);
            view->field_0x5D4[cnt].y = (f32)((f64)cy - lbl_eu_80667098);
            view->field_0x5D4[cnt].z = (f32)((f64)cz - lbl_eu_80667098);
            sprintf(buf, strings + 0xb8, dispId);
            found = true;
            break;
        }
        case 6: {
            // Global gimmick table position (skipped entirely for filtered ids).
            if (func_8013C038(dispId)) break;
            bool hit = false;
            for (u32 k = 1; k <= rowCount; k++) {
                s16 v = func_80136330(lbl_eu_80663FB8, strings + 0x30, k);
                CMMGimmickPos* p = func_801F4E68(getUnk80664658(), dispId);
                if ((f64)v - lbl_eu_80667098 > p->y) {
                    if (band == k) hit = true;
                    break;
                }
            }
            if (!hit) break;
            CMMGimmickPos* p = func_801F4E68(getUnk80664658(), dispId);
            u32 cnt = view->field_0x6A4;
            view->field_0x5D4[cnt].x = p->x;
            view->field_0x5D4[cnt].y = p->y;
            view->field_0x5D4[cnt].z = p->z;
            sprintf(buf, strings + 0xc2, dispId);
            found = true;
            break;
        }
        default:
            break;
        }
        if (!found) continue;

        // Create the marker picture: pane name from the layout table, kind
        // selects the resource-name lookup (0 = skip creation entirely).
        u16 nameId = func_80136254((void*)view->field_0x00, strings + 0x4c, i + 1);
        u32 cnt = view->field_0x6A4;
        u32 kind = 9;
        view->field_0x4C8[cnt] = 0;
        if ((u16)nameId == func_8009ECF0()) {
            kind = 8;
            view->field_0x4C8[cnt] = 1;
        }

        view->field_0x694[cnt] = view->field_0x270[i];
        if (view->field_0x270[i] == 0) {
            // Row inactive: hide the stale pane unless its resource is gone.
            u16 pid = func_80136254((void*)view->field_0x00, strings + 0x5f, i + 1);
            if (pid != 0 && func_8009CF8C(pid + 0x220) == 0) kind = 0xa;
        } else {
            if (clockPane == NULL) {
                kind = 5;
            } else {
                *(u8*)((u8*)clockPane + 0xBB) = (*(u8*)((u8*)clockPane + 0xBB) & 0xFE) | 1;
            }
        }

        // Skip if a pane with this name already exists under the root.
        nw4r::lyt::Pane* root = (nw4r::lyt::Pane*)view->field_0x14;
        if (((MiniMapPaneMgr*)root)->v03C(buf, 1) != NULL) continue;

        nw4r::lyt::Pane* pane = NULL;
        f32 zero = lbl_eu_80667090;
        if (kind != 0) {
            char* texName = (char*)func_8013902C(kind);
            if (func_801355F4()->GetResource(0x74696D67, texName, 0) == NULL) {
                kind = 0;
            } else {
                pane = (nw4r::lyt::Pane*)createPicture__10CLibLayoutFv();
                SetName__Q34nw4r3lyt4PaneFPCc(pane, buf);
                *(f32*)((u8*)pane + 0x2C) = zero;
                *(f32*)((u8*)pane + 0x30) = zero;
                *(f32*)((u8*)pane + 0x34) = zero;
                func_80137C1C(pane, -1);
                *(u8*)((u8*)pane + 0xBB) = (*(u8*)((u8*)pane + 0xBB) & 0xFE) | 1;
            }
        }
        if (pane == NULL) continue;
        PrependChild__Q34nw4r3lyt4PaneFPQ34nw4r3lyt4Pane(root, pane);
        view->field_0x594_entries[cnt] = (CMMMapEntry*)pane;
        view->field_0x6A4 = cnt + 1;
    }

    func_801168A0(view);
}

// func_801165EC - reset the minimap gimmick-view tables (retail unmangled
// name; the CMiniMapGimmickView subobject sits at minimap+0x17C).
void func_801165EC(CMiniMapGimmickView* self) {
    f32 zero = lbl_eu_80667090;
    self->field_0x04 = 0;
    u32 off = 0;
    self->field_0x00 = 0;
    self->field_0x08 = zero;
    self->field_0x0C = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x590 = 0;
    self->field_0x6A4 = 0;
    // Byte-offset cursor; trip count stays 25.
    for (u32 k = 0; k < 25; k++, off += 8) {
        self->field_0x338[off + 0] = 0;
        self->field_0x400[off + 0] = 0;
        self->field_0x338[off + 1] = 0;
        self->field_0x400[off + 1] = 0;
        self->field_0x338[off + 2] = 0;
        self->field_0x400[off + 2] = 0;
        self->field_0x338[off + 3] = 0;
        self->field_0x400[off + 3] = 0;
        self->field_0x338[off + 4] = 0;
        self->field_0x400[off + 4] = 0;
        self->field_0x338[off + 5] = 0;
        self->field_0x400[off + 5] = 0;
        self->field_0x338[off + 6] = 0;
        self->field_0x400[off + 6] = 0;
        self->field_0x338[off + 7] = 0;
        self->field_0x400[off + 7] = 0;
    }
}

// func_801167EC - fill one marker-visibility table entry per call; returns 1
// when the row budget ran out (func_80118854 uses the result as a bool).
u32 func_801167EC(CMiniMapGimmickView* self) {
    // Declaration order tuned for MWCC reverse-declaration reg allocation
    // (ret=r30, end=r29, i=r28).
    u32 ret = 0;
    s32 end;
    s32 count = self->field_0x0C;
    s32 i = self->field_0x10;
    if (i >= count) {
        ret = 1;
    } else {
        u8* tab = ((self->field_0x590 != 0) ? self->field_0x338
                                            : self->field_0x400);
        end = i + count;
        while (i < end) {
            *(tab + i) = (u8)func_80138574(self->field_0x00, i + 1);
            self->field_0x10 += 1;
            if (self->field_0x10 >= self->field_0x0C) {
                ret = 1;
                break;
            }
            i++;
        }
    }
    func_801168A0(self);
    return ret;
}
