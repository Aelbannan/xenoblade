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
#include <nw4r/lyt.h>
#include <revolution/gx/GXPixel.h>

// Pre-mangled global-view declarations so the offset-adjusted dispatch thunks
// can address the CMenuMiniMap2 member methods by their retail symbol names
// (same pattern as CSysWinSelect.cpp).
void __dt__13CMenuMiniMap2Fv(void* self);
void cbRenderBefore__13CMenuMiniMap2Fv(void* self);

// Offset-adjusted vtable dispatch thunk: the CMenuMiniMap2 subobject sits at
// self-0x58, so adjust and tail-jump straight to the dtor (retail:
// subi r3,r3,0x58 / b __dt__13CMenuMiniMap2Fv).
void func_8011C434(void* self) { ((void(*)(void*))__dt__13CMenuMiniMap2Fv)((char*)self - 0x58); }

void func_80117734(){}

// Named .sdata2 conversion magics: defining them lets MWCC's constant pool
// reuse the retail symbols for the (f32) casts in Move() instead of emitting
// TU-local @N labels (CfResReloadImpl / CSuddenCommu idiom). 80667098 is the
// signed int->float magic (2^52 + 2^31), 806670A8 the unsigned one (2^52).
extern const double lbl_eu_80667098 = 0x4330000080000000ll;
extern const double lbl_eu_806670A8 = 0x4330000000000000ll;

CMMTex::~CMMTex() {}

bool CMMTex::OnFileEvent(CEventFile* pEventFile) {
    u8* data;
    if (mFileHandle == pEventFile->mFileHandle) {
        // Grab the loaded buffer, then clear the handle state in retail order:
        // save mData, null the handle's mData, stash + flag + clear handle.
        data = mFileHandle->mData;
        mFileHandle->mData = 0;
        mData = data;
        mReady = 1;
        mFileHandle = 0;
        return true;
    }
    return false;
}
void __ct__CMiniMap(CMiniMap* self) {}

// mUnk1C is auto-destroyed; MWCC emits the null-this guard and the
// deleting-flag tail (cmpwi flag / ble / operator delete) for the virtual dtor.
CMMClock::~CMMClock() {}

CMiniMap::~CMiniMap() {
    // m834 then m824 auto-destroyed (reverse declaration order).
}

// mMiniMap then mClock auto-destroyed (reverse declaration order), then the
// CProcess base (called with flag 0 so it never deletes); deleting-flag tail
// is auto-emitted.
CMenuMiniMap2::~CMenuMiniMap2() {}

// func_80117C30 - per-frame minimap subobject update (called from Move with
// &mMiniMap). Stub: not a matching target; noinline + extern "C" keep the
// call site in Move emitting the retail addi r3,r31,0x90 / bl func_80117C30
// pair. The func_8003AA34() call (same trick as func_80118058) makes the stub
// impure so MWCC's -ipa scheduler cannot reorder the call site past the pad
// block (retail keeps func_80117C30 before the flag/pad checks).
extern "C" void __declspec(noinline) func_80117C30(CMiniMap* self) {
    (void)self;
    func_8003AA34();
}

// func_80118058 - stub (not a matching target). __declspec(noinline) plus an
// extern call keep it out of Term's body: an empty body lets MWCC's scheduler
// sink Term's `lbl_eu_80663FB0 = 0` store below this call (retail keeps the
// store AFTER the call), while an inlineable body gets merged into Term and
// drops the bl entirely. extern "C" so the call reloc matches retail
// (func_80118058, not func_80118058__FP8CMiniMap).
extern "C" void __declspec(noinline) func_80118058(CMiniMap* self) {
    (void)self;
    func_8003AA34();
}

// ============================================================================
// func_80118854 - CMiniMap marker/map-icon update (retail symbol unmangled).
// Reconstructed from retail ASM; high-level C++ only.
// ============================================================================

#include <nw4r/lyt.h>
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// ---- retail data symbols (small-data / absolute addressing per retail) ----
extern "C" s32 lbl_eu_80663FBC;          // frame counter (periodic cleanup gate)
extern "C" u32 lbl_eu_80664184;          // current BDAT row index
extern "C" void* lbl_eu_80663FB8;        // current BDAT table pointer
extern "C" f32 lbl_eu_80667090;          // zero constant
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
void __dt__80043E88(void* holder, s16);
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
void* func_80138234(void* bdat, u32 row);
void* func_8013902C(int type);
nw4r::lyt::ArcResourceAccessor* func_801355F4();
void func_80137C1C(void* pic, s32 arg);
void func_80116B40(void* self);
void* func_801167EC(void* self);
void func_8011628C(void* self, u32 row);
void func_801160A8(void* self, void* table, void* layout, f32 scale);
void func_80116670(void* self, u32 arg, void* layout, f32 scale);
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
}
// C++-mangled retail helper: actor id -> action source object.
void* func_800B708C(int id);

// ============================================================================
// CMMClock::OnFileEvent - build the clock layout + animations once the arc
// file (mFileHandle) reports it is loaded. Mirrors CBattery/CFade's OnFileEvent:
// scratch region, accessor attach, layout + anim transforms, then enable.
// ============================================================================
bool CMMClock::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle != pEventFile->mFileHandle) return false;
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

// ============================================================================
// CMiniMap::OnFileEvent - build the minimap layout + animations once the arc
// file (mFileHandle) reports it is loaded, then seed the clock textboxes and
// the shared 'timg' picture onto the layout root and hand the tables to the
// per-frame marker routines (func_801160A8 / func_80116670).
// ============================================================================
bool CMiniMap::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle != pEventFile->mFileHandle) return false;
    if (pEventFile->unk0 == 1) {
        Class_8045F858 guard(&m824);
        u8* data = (u8*)mFileHandle->getData();
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, &lbl_eu_804FE1FC[0xE9]);

        // Resolve the per-row layout/animation names from the row table.
        u16 row = lbl_eu_8052C740[lbl_eu_80664184 - 1];
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
        nw4r::lyt::Pane* parent = mLayout0C->GetRootPane()->FindPaneByName(
            &lbl_eu_804FE1FC[0x1FA], true);
        if (parent != 0) {
            for (u8 i = 0; lbl_eu_8052C788[i] != 0; i++) {
                nw4r::lyt::Pane* tb = (nw4r::lyt::Pane*)createTextbox__10CLibLayoutFv();
                tb->SetName(lbl_eu_8052C788[i]);
                tb->SetVisible(true);
                parent->AppendChild(tb);
            }
        }

        // Shared 'timg' picture pinned to the origin.
        const char* name202 = &lbl_eu_804FE1FC[0x202];
        nw4r::lyt::Pane* pic;
        if (name202 != 0) {
            void* texName = func_8013902C(2);
            if (func_801355F4()->GetResource(0x74696D67, (const char*)texName, 0)) {
                pic = (nw4r::lyt::Pane*)createPicture__10CLibLayoutFv();
                pic->SetName(name202);
                pic->SetSRTElement(0, 0.0f);
                pic->SetSRTElement(1, 0.0f);
                // Dead origin record (retail stores it to the frame).
                ml::CVec3 pos;
                pos.x = 0.0f;
                pos.y = 0.0f;
                pos.z = 0.0f;
                pic->SetSRTElement(2, 0.0f);
                func_80137C1C(pic, -1);
                pic->SetVisible(true);
            } else {
                pic = 0;
            }
        } else {
            pic = 0;
        }
        parent = mLayout0C->GetRootPane()->FindPaneByName(
            &lbl_eu_804FE1FC[0x281], true);
        parent->AppendChild(pic);

        func_801160A8(mField3C, lbl_eu_80663FB8, mLayout0C, mField20);
        func_80116670(mField17C, mField28, mLayout0C, mField20);
        mReady = 1;
        m824.func_8045F810();
    }
    mFileHandle = 0;
    return true;
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

// Material view: SRT array head + flags word at +0x3C (bits 4-7 and 28 are
// validated with Panic before use).
struct MiniMapB05CMat {
    f32 srt[5];                      // 0x00..0x14 (TexSRT array head)
    u8 pad_14[0x3C - 0x14];          // 0x14..0x3C
    u32 field_3C;                    // 0x3C - flags (bits 4-7, bit 28)
};

// TexMap head read by the SRT sizing (widths/heights are u16).
struct MiniMapB05CTexMap {
    u32 field_00;                    // 0x00
    u32 field_04;                    // 0x04
    u16 field_08;                    // 0x08 - width
    u16 field_0A;                    // 0x0A - height
    f32 field_0C;                    // 0x0C
    f32 field_10;                    // 0x10
    u16 field_14;                    // 0x14
    u16 field_16;                    // 0x16
    u32 field_18;                    // 0x18
};

// Linked list walked by the pane-sweep loops (next at +0, 3 f32s at +0x34).
struct MiniMapB05CNode {
    MiniMapB05CNode* next;           // 0x00
    u8 pad_04[0x34 - 0x04];          // 0x04..0x34
    f32 field_34;                    // 0x34
    f32 field_38;                    // 0x38
    f32 field_3C;                    // 0x3C
};

// View of the v01C() result (battle target pos): y at +0x4.
struct MiniMapB05CVec4 {
    u8 pad_00[0x4];                  // 0x00
    f32 field_4;                     // 0x04
};

// Pane view with vtable slots 0x68 (material) / 0x3C (child lookup).
struct MiniMapB05CIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void* vf3C(const char* name, int create);
    virtual void _v040(); virtual void _v044(); virtual void _v048(); virtual void _v04C();
    virtual void _v050(); virtual void _v054(); virtual void _v058(); virtual void _v05C();
    virtual void _v060(); virtual void _v064(); virtual void* vf68();
};

// Best-effort reconstruction (elided retail loop body).
extern "C" void func_8011B05C(MiniMapSelf* self) {
    // Clock pane + material lookup (pane manager v03C -> vtable+0x68).
    MiniMapPaneMgr* paneMgr = self->m0C->m10;
    MiniMapB05CIf* pane = (MiniMapB05CIf*)paneMgr->v03C(&lbl_eu_804FE1FC[0x1FA], 1);
    MiniMapB05CMat* mat = (MiniMapB05CMat*)pane->vf68();

    if (((mat->field_3C >> 4) & 0xF) == 0) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CBC0, 0x9C,
                                   (const char*)lbl_eu_8052CB8C);
    }
    const nw4r::lyt::TexSRT* srt =
        ((const nw4r::lyt::Material*)mat)->GetTexSRTAry();
    f32 srtBuf[5];
    srtBuf[0] = srt->translate.x;
    srtBuf[1] = srt->translate.y;
    srtBuf[2] = srt->rotate;
    srtBuf[3] = srt->scale.x;
    srtBuf[4] = srt->scale.y;

    if ((mat->field_3C >> 28) == 0) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB80, 0x79,
                                   (const char*)lbl_eu_8052CB4C);
    }
    MiniMapB05CTexMap* tex =
        (MiniMapB05CTexMap*)((const nw4r::lyt::Material*)mat)->GetTexMapAry();
    f32 texW = (f32)tex->field_08;
    f32 texH = (f32)tex->field_0A;
    f32 texW2 = tex->field_0C;
    f32 texH2 = tex->field_10;

    f32 scale = lbl_eu_80667090;
    ml::CVec3 pos;
    MiniMapObj* player = (MiniMapObj*)cf::CfGameManager::getPlayer(0);
    if (player != 0) {
        ml::CVec3* ppos = ((MiniMapObj*)player)->GetPos();
        pos.x = ppos->x;
        pos.y = ppos->y;
        pos.z = ppos->z;
        ((MiniMapObj*)player)->v0CC();
        scale = lbl_eu_806670BC * ((MiniMapObj*)player)->v0CC() /
                lbl_eu_8066A1F8;
    } else {
        pos.x = lbl_eu_80667090;
        pos.y = lbl_eu_80667090;
        pos.z = lbl_eu_80667090;
    }

    // SRT rotation math: player offset / map scale, rotated by the m1C/m1E
    // angles, scaled by the texture size (see retail fctiwz/fmadds block).
    f32 f9 = texW;
    f32 f8 = texH;
    f32 f2 = pos.x / self->m20;
    f32 f1 = pos.z / self->m20;
    f32 f13 = lbl_eu_806670A4;
    f32 f10 = lbl_eu_806670B4;
    f64 rotC = (f64)self->m1C;
    f64 rotE = (f64)self->m1E;
    s32 gx = (s32)f2;
    s32 gz = (s32)f1;
    f32 f12 = f13 / f9;
    f32 f7 = f13 / f8;
    f32 f5 = f13 / (f32)gx;
    f32 f2b = f13 / (f32)gz;
    f32 r0 = f10 * f9 + (f32)rotC;
    f32 r1 = f10 * (f32)rotE + (f32)gx;
    f32 r2 = f10 * f8 + (f32)rotC;
    f32 r3 = f10 * (f32)rotE + (f32)gz;
    srtBuf[0] = f12 * r0 - f10 + (f5 * r1 - f10);
    srtBuf[1] = f7 * r2 - f10 + (f2b * r3 - f10);

    if (((mat->field_3C >> 4) & 0xF) == 0) {
        Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CC00, 0xA2,
                                   (const char*)lbl_eu_8052CBCC);
    }
    nw4r::lyt::TexSRT* srt2 =
        const_cast<nw4r::lyt::TexSRT*>(
            ((const nw4r::lyt::Material*)mat)->GetTexSRTAry());
    srt2->translate.x = srtBuf[0];
    srt2->translate.y = srtBuf[1];
    srt2->rotate = srtBuf[2];
    srt2->scale.x = srtBuf[3];
    srt2->scale.y = srtBuf[4];

    // Clock pane group: reposition/scale the pane and sweep its child list.
    MiniMapB05CIf* group = (MiniMapB05CIf*)paneMgr->v03C(&lbl_eu_804FE1FC[0x212], 1);
    f32 gx0 = lbl_eu_80667090;
    f32 gy0 = lbl_eu_80667090;
    f32 gz0 = scale;
    ((nw4r::lyt::Pane*)group)->SetSRTElement(0, gx0);
    ((nw4r::lyt::Pane*)group)->SetSRTElement(1, gy0);
    ((nw4r::lyt::Pane*)group)->SetSRTElement(2, gz0);

    if (func_8013BE58() == 0) {
        f32 f29 = lbl_eu_80667090;
        f32 f30 = lbl_eu_80667090;
        f32 f31 = scale;
        for (u8 i = 0; lbl_eu_8052C788[i] != 0; i++) {
            nw4r::lyt::Pane* p2 =
                (nw4r::lyt::Pane*)paneMgr->v03C(lbl_eu_8052C788[i], 1);
            if (p2 == 0) continue;
            MiniMapB05CNode* node = (MiniMapB05CNode*)((u8*)p2 + 0x14);
            MiniMapB05CNode* end = node;
            node = node->next;
            while (node != end) {
                if (node == 0) {
                    Panic__Q24nw4r2dbFPCciPCce((const char*)lbl_eu_8052CB40, 0x23D,
                                               (const char*)lbl_eu_8052CB1C);
                }
                node->field_34 = f29;
                node->field_38 = f30;
                node->field_3C = f31;
                node = node->next;
            }
        }
    }

    // Minimap 'timg' pane: pin to the current battle target position.
    MiniMapB05CIf* pic =
        (MiniMapB05CIf*)paneMgr->v03C(&lbl_eu_804FE1FC[0x20A], 1);
    if (pic != 0) {
        if (cf::CfGameManager::getInstance()->func_800821F8() != 0) {
            void* tgt = cf::CfGameManager::getInstance()->func_800821F8();
            MiniMapB05CVec4* tv = (MiniMapB05CVec4*)((MiniMapObj*)tgt)->v01C();
            f32 ty = tv->field_4;
            f32 f1 = lbl_eu_806670BC * ty / lbl_eu_8066A1F8;
            ((nw4r::lyt::Pane*)pic)->SetSRTElement(0, 0.0f);
            ((nw4r::lyt::Pane*)pic)->SetSRTElement(1, 0.0f);
            ((nw4r::lyt::Pane*)pic)->SetSRTElement(2, f1);
        }
    }
    self->m1B = (self->m38 != 0) ? 1 : 0;
}


void CMenuMiniMap2::Init() {}

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

// Clock-hand advance shared by the layout state machine's case 1/2 bodies
// (inlined twice by MWCC - each inline copy gets its own a/b slots, matching
// retail's two distinct u16 pairs).
static inline void clockHandAdvance(CMenuMiniMap2* self) {
    u16 b = 0, a = 0;
    func_8006A234(&a, &b);
    f32 frame = lbl_eu_80661E48 * (f32)(a * 60 + b) + lbl_eu_80663FB4;
    f32 max = (f32)self->mClock.mAnimTrans1->GetFrameSize();
    if (frame > max) frame -= max;
    self->mClock.mAnimTrans1->SetFrame(frame);
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
        if ((mMiniMap.mField38 != 0 ? mMiniMap.mReady : 0) == 0) break;
        // Load the cached resource first (retail hoists lwz r5,0xc4 above the
        // store sequence); the fresh GetResource result merges into it.
        void* res = mMiniMap.mField34;
        mField8D4 = 1;
        mClock.field_0x18 = 1;
        mClock.field_0x19 = 0;
        mMiniMap.field_0x18 = 1;
        mMiniMap.field_0x19 = 0;
        if (res == 0) {
            res = mMiniMap.mAccessor->GetResource(0x74696D67,
                                                  &lbl_eu_804FE1FC[0x1E7], 0);
            if (res == 0) break;
        }
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
            if ((mMiniMap.mField38 != 0 ? mMiniMap.mReady : 0) != 0) {
                mField8D4 = 0;
            }
        }
        break;
    }

    // Clock layout animation state machine (only while the clock layout is
    // attached). State 1 fades in and snaps the hand to the game time,
    // state 2 keeps the hand tracking time, state 3 fades out.
    if (mClock.mLayout != 0) {
        switch (mClock.field_0x18) {
        case 1: {
            u16 a, b;
            if (func_80137444(mClock.mAnimTrans0, lbl_eu_806670A4) != 0) {
                mClock.field_0x18 = 2;
                mClock.field_0x19 = 1;
                mClock.mLayout->SetAnimationEnable(mClock.mAnimTrans0, 0);
                mClock.mLayout->SetAnimationEnable(mClock.mAnimTrans1, 1);
                mClock.mLayout->Animate(0);
                clockHandAdvance(this);
            }
            break;
        }
        case 2:
            clockHandAdvance(this);
            break;
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
bool func_8011C2E8() {
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
extern "C" void func_8011C2FC(void) {
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
void func_80115FD0(MiniMapTable* self) {
    f32 zero = lbl_eu_80667090;
    self->field_00 = 0;
    self->field_08 = 0;
    self->field_0C = zero;
    self->field_13C = 0;
    func_8003AA34();
    self->field_04 = (u32)getFP__FPCc(lbl_eu_804FE1FC);
    for (u32 i = 0; i < 3; i++) {
        for (u32 j = 0; j < 5; j++) {
            self->field_10[i][j] = 0;
            self->field_100[i][j] = 0;
            self->field_2E[i][j] = 0;
            self->field_3D[i][j] = 0;
        }
    }
}
extern "C" void __declspec(noinline) func_801160A8(void* self, void* table, void* layout, f32 scale) {}
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
            if (table->field_3D[0][j] != 0) {
                table->field_13C->RemoveChild(table->field_100[0][j]);
                table->field_3D[0][j] = 0;
            }
        } else {
            // In range: attach the pane on first use, then place it.
            if (table->field_3D[0][j] == 0) {
                table->field_13C->PrependChild(table->field_100[0][j]);
                table->field_100[0][j]->SetVisible(true);
                table->field_3D[0][j] = 1;
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
extern "C" void __declspec(noinline) func_80116670(void* self, u32 arg, void* layout, f32 scale) {}
extern "C" void __declspec(noinline) func_801168A0(CMiniMapGimmickView* self) {}
extern "C" void __declspec(noinline) func_80116B40(void* self) {}

// func_801165EC - reset the minimap gimmick-view tables (retail unmangled
// name; the CMiniMapGimmickView subobject sits at minimap+0x17C).
void func_801165EC(CMiniMapGimmickView* self) {
    f32 zero = lbl_eu_80667090;
    self->field_0x00 = 0;
    self->field_0x04 = 0;
    self->field_0x08 = zero;
    self->field_0x0C = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x590 = 0;
    self->field_0x6A4 = 0;
    for (u32 i = 0; i < 25; i++) {
        for (u32 j = 0; j < 8; j++) {
            self->field_0x338[i * 8 + j] = 0;
            self->field_0x400[i * 8 + j] = 0;
        }
    }
}

// func_801167EC - fill one marker-visibility table entry per call; returns 1
// when the row budget ran out (func_80118854 uses the result as a bool).
u32 func_801167EC(CMiniMapGimmickView* self) {
    u32 ret = 0;
    u32 count = self->field_0x0C;
    u32 i = self->field_0x10;
    if (i < count) {
        u8* arr = (self->field_0x590 != 0) ? self->field_0x338
                                           : self->field_0x400;
        u32 end = i + count;
        u8* p = arr + i;
        while (i < end) {
            *p = (u8)func_80138574(self->field_0x00, i + 1);
            self->field_0x10++;
            if (self->field_0x10 >= self->field_0x0C) {
                ret = 1;
                break;
            }
            i++;
            p++;
        }
    } else {
        ret = 1;
    }
    func_801168A0(self);
    return ret;
}
