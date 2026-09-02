// Decompiled: CMenuSymbolMark + CArrow3D
// High-level C++ reconstruction

#include "kyoshin/menu/CMenuSymbolMark.hpp"
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>   // nw4r::lyt::DrawInfo
#include <nw4r/lyt/lyt_layout.h>    // nw4r::lyt::Layout
#include <nw4r/lyt/lyt_arcResourceAccessor.h> // ArcResourceAccessor
#include <nw4r/lyt/lyt_pane.h>      // nw4r::lyt::Pane (FindPaneByName/SetVisible)
#include <revolution/mtx/vec.h>     // Vec, PSVECMag
#include <revolution/mtx.h>         // PSMTXConcat
#include <revolution/GX.h>          // GX render state
#include <nw4r/math.h>              // nw4r::math::VEC3Sub (PS kernel)
#include <nw4r/math/math_triangular.h>  // SinFIdx/CosFIdx/Atan2FIdx
#include <nw4r/math/math_arithmetic.h>  // FrSqrt
#include <nw4r/g3d/res/g3d_resfile.h>   // nw4r::g3d::ResFile/ResTex/ResPltt
#include <monolib/util.hpp>         // ml::FixStr
#include <string.h>
#include <math.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Retail helper imports (unmangled retail symbol names).
extern "C" u32 func_801380A0(u16);
extern "C" u8 func_8009CF8C(u32);
extern "C" u32 func_80138138(u16);
extern "C" u32 func_801361E8(u32, const char*, u32);
extern "C" int func_80138574(void*, u32);
// Naturally-mangled retail import (findObjectById__Fi).
void* findObjectById(int id);
extern u32 lbl_eu_804FE720[];
extern u32 lbl_eu_8052CCA8[];
extern u32 lbl_eu_80573D18[];
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void* region);
// Flat retail helper imports for the timer/list walkers and layout glue.
extern "C" void* func_800B6BEC();
extern "C" void* func_800B6C58();
// Scene pose block comes from the typed view in the header
// (ScnXformBlock, returned by func_80496264).
extern "C" int func_8013A4B4(void* anchor, void* extent, void* pos);
extern "C" void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" void func_80137E7C(nw4r::lyt::Layout*, const char*);
extern "C" char* func_8013639C(void*, const char*, u32);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" void* func_801355BC();
extern "C" void* getPackedFont();
extern "C" void func_8013676C(nw4r::lyt::Pane*, u32);
// Naturally-mangled retail helpers (MWCC emits these exact symbols).
void buildLayout(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void setLayoutTextBoxNumber(nw4r::lyt::Layout*, char*, u8);
extern "C" u32 getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void __ct__8CProcessFv(void* self);
extern "C" __declspec(noinline) void func_8011E778(
    CMenuSymbolMark* self, SymbolMarkEntry* entry,
    nw4r::math::VEC3* worldIn, EntryInputPos* posSrc, u32 flag);
extern const PtmfTriple __ptmf_null;
// Panic/format string blobs and render-item interface vtables.
extern u8 lbl_eu_8052637C[];
extern u8 lbl_eu_80526354[];
extern u8 lbl_eu_8052CDB4[];
extern u8 lbl_eu_8052CD50[];
// Interface vtable blobs installed over the object prefix (+0x10) and the
// manual IWorkEvent/IScnRender subobjects (+0x58/+0x5C).
extern u8 lbl_eu_8052BF70[];
extern u8 lbl_eu_8052CDF8[];


// Zero-body stubs for matching
// Each function will be filled in iteratively

// ---------- func_8011D338 ----------
// Scans menu ids across ranks 1..26 (func_801380A0 bounds per rank) looking
// for an entry whose kind byte is a real unlockable kind and whose table
// lookup returns 1. regionBase is the render-item state block passed by the
// func_8011EFB0 tail (unused here).
extern "C" int func_8011D338(u32 regionBase) {
    (void)regionBase;
    // Both table bases are hoisted into nonvolatile address registers by MWCC.
    // Both table bases are hoisted into nonvolatile address registers by MWCC.
    const char* str = (const char*)&lbl_eu_804FE720[0];
    u32* tbl = lbl_eu_80573D18;
    for (u32 rank = 1; rank < 0x1b; rank++) {
        u32 start = func_801380A0((u16)rank);
        if ((u16)start == 0) {
            continue;
        }
        u32 end = (u16)func_801380A0((u16)(rank + 1));
        for (u32 id = start; (u16)id < end; id++) {
            u32 kind = (u8)func_8009CF8C((u16)id + 0x220);
            u32 check = 0;
            if (kind != 0 && kind != 0xc8 && kind != 0xfe && kind != 0xff) {
                check = 1;
            }
            if (check != 0 &&
                (u8)func_801361E8(tbl[func_80138138((u16)id)],
                                  str + 0x2f, (u16)id) == 1) {
                return 1;
            }
        }
    }
    return 0;
}

// Render-gate globals/callees (declared locally to avoid header conflicts).
extern "C" void* getInstance__9CTaskGameFv();
extern "C" void* isFlag01Set__9CTaskGameFv(void* self);
extern "C" int func_8013BE50();
struct CMenuGimmickGlobal { u8 mPad[0x214]; u32 field_214; };
extern "C" CMenuGimmickGlobal* getUnk80664658();
extern u32 lbl_eu_80663E28;
// C++-linkage helpers - retail emits the mangled forms
// func_80137250__FPQ34nw4r3lyt8DrawInfo / drawLayout__FPQ34nw4r3lyt6Layout...
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
void drawLayout(nw4r::lyt::Layout* layout, nw4r::lyt::DrawInfo* drawInfo,
                   int a, int b);
// Pre-mangled DrawInfo ctor/dtor (raw-storage construction); extern "C" so
// MWCC keeps the retail symbol verbatim instead of appending the signature.
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo,
                                             int flags);
extern "C" void __dt__Q34nw4r3lyt6LayoutFv(void* self, int flags);

// Retail singletons used by the menu create helpers.
extern u32 lbl_eu_80663FC8;                    // installed CMenuSymbolMark pointer
extern const f32 lbl_eu_806670D4;              // timer reset value
extern const f32 lbl_eu_806670E0;              // timer threshold
extern const f32 lbl_eu_806670E8;              // timer tick delta
extern "C" void* getWorkMem__17CWorkThreadSystemFv();
extern "C" CMenuSymbolMark* __ct__CMenuSymbolMark(CMenuSymbolMark* self, CProcess* parent);

extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int flags);
extern "C" void __dt__8CProcessFv(void* self, int flags);
extern "C" void __dl__FPv(void* p);

// ---------- CMenuSymbolMark constructor ----------
// Base CProcess construction and the member initializer list build the manual
// interface subobjects (ptmf triples cleared from __ptmf_null, interface
// vtables at +0x58/+0x5C); the body then zeroes every entry record and pulls
// the menu tables/file pointers.
CMenuSymbolMark::CMenuSymbolMark(CScn* scn, u32 idx)
    : mUnk54(0),
      mUnk55(0),
      mIWorkEventVt((void*)&lbl_eu_8052CDF8[0x24]),
      mIScnRenderVt((void*)&lbl_eu_8052CDF8[0xac]),
      mScn(scn) {
    ((ProcPrefixView*)this)->vt10 = (void*)&lbl_eu_8052BF70[0];
    ptmfMove = __ptmf_null;
    ptmfDraw = __ptmf_null;
    ((ProcPrefixView*)this)->vt10 = (void*)&lbl_eu_8052CDF8[0];
    const char* S = (const char*)&lbl_eu_804FE720[0];
    // Clear every entry record; the world floats copy uninitialized stack
    // homes. Pointer-walk form: MWCC expands this as peel + x8 unroll with a
    // remainder loop (trip count computed by pointer subtraction).
    SymbolMarkEntry clearEntry;
    clearEntry.unk00 = 0;
    clearEntry.unk04 = 0;
    clearEntry.layout = 0;
    clearEntry.flag0 = 0;
    clearEntry.flag1 = 0;
    clearEntry.flag2 = 0;
    clearEntry.unk1C = 0;
    SymbolMarkEntry* pCur = &mEntries[1];
    SymbolMarkEntry* pEnd = &mEntries[16];
    while (pCur < pEnd) {
        *pCur = clearEntry;
        ++pCur;
    }
    mTimer = lbl_eu_806670D4;
    mField_274 = 0;
    mEntryCount = 0;
    mArchiveFP = 0;
    mSomeFP = 0;
    mAnotherFP = 0;
    mSomeValue = 0;
    mSomeValue2 = 0;
    mSomeValue3 = 0;
    mSomeValue4 = 0;
    mField_498 = 0;
    mField_49C = lbl_eu_806670D4;
    mField_6A0 = 0;
    mField_6A4 = lbl_eu_806670D8;
    mField_8A8 = 0;
    mField_8AC = lbl_eu_806670DC;
    mRenderItem = 0;
    // Second entry-record pass: retail emits the entry clear twice; this
    // later copy lowers as an indexed x4-unrolled countdown loop.
    for (int i = 0; i < 16; i++) {
        SymbolMarkEntry& slot = mEntries[i];
        slot = clearEntry;
    }
    // Archive/table setup only when the current map id owns a menu-table row.
    u32 mapId = lbl_eu_80664184;
    u32 mapIdx = mapId - 1;
    if (lbl_eu_8052CCA8[mapIdx] != 0) {
        func_8003AA34();
        mArchiveFP = (u8*)getFP(S + 0x39);
        func_8003AA34();
        mSomeFP = (u8*)getFP(S + 0x46);
        mSomeValue2 = func_8003B1EC(mArchiveFP);
        mSomeValue3 = func_801380A0((u16)mapId);
        mSomeValue = lbl_eu_80573D18[mapIdx];
        mSomeValue4 = func_8003B1EC((u8*)mSomeValue) + mSomeValue3;
    }
    memset(mBuffer, 0, 0x200);
    // Zero both id arrays row-by-row (outer row pointer advances 0x40).
    for (u32 i = 0; i < 8; i++) {
        for (u32 j = 0; j < 0x10; j++) {
            mArray4A0[i * 0x10 + j] = 0;
            mArray6A8[i * 0x10 + j] = 0;
        }
    }
}

// ---------- CMenuSymbolMark destructor ----------
// Retail: if (self == 0) goto end; __dt__17UnkClass_8045F564Fv(&self->mUnkClass, -1);
// if (self != 0) { if (self != 0) { __dt__8CProcessFv(self, 0); } }
// if (flags > 0) __dl__FPv(self); end: return self; - the redundant
// `if (self != 0)` re-check reproduces the retail's dead double-beq
// (CMenuBattleChain/CMenuArtsSelect idiom).
#pragma push
#pragma auto_inline off
extern "C" void* __dt__15CMenuSymbolMarkFv(CMenuSymbolMark* self, int flags) {
    if (self == 0) goto end;
    __dt__17UnkClass_8045F564Fv(&self->mUnkClass, -1);
    if (self != 0) {
        if (self != 0) {
            __dt__8CProcessFv(self, 0);
        }
    }
    if (flags > 0) __dl__FPv(self);
end:
    return self;
}
#pragma pop

// ---------- CMenuSymbolMark::Init ----------
// Builds the per-entry layouts from the shared archive accessor, registers the
// render callback on the scene, then constructs the shared render-item object
// (a CProcess-derived blob whose ptmf slots are cleared) and registers it.
void CMenuSymbolMark::Init() {
    if (mArchiveFP == 0) {
        return;
    }
    mUnkClass.createRegion((int)getHandleMEM2__Q23mtl10MemManagerFv(), 0x5400,
                           (const char*)&lbl_eu_804FE720[0x53], 0);
    Class_8045F858 regionGuard(&mUnkClass);
    for (u32 i = 0; i < 16; i++) {
        buildLayout((nw4r::lyt::Layout**)&mEntries[i].layout, func_801355F4(),
                      (const char*)&lbl_eu_804FE720[0x63]);
        // Re-read the layout pointer at every use so nothing spans the virtual
        // calls (retail reloads the slot from the entry base each time).
        ((nw4r::lyt::Layout*)mEntries[i].layout)->Animate();
        func_8013676C(((nw4r::lyt::Layout*)mEntries[i].layout)->GetRootPane(),
                      (u32)getPackedFont());
    }
    mField_274 = 1;
    // The `if (this)` idiom splits the IScnRender subobject address
    // (this+0x5C) into its own addi; the call itself is unconditional.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    mScn->addRenderCB(render, 7, 0);
    mUnkClass.func_8045F810();
    SymbolMarkRenderItem* item = (SymbolMarkRenderItem*)mtl::MemManager::allocate(
        0x7c, (u32)getWorkMem__17CWorkThreadSystemFv());
    if (item != 0) {
        // Construct the CProcess base over the raw allocation, then patch the
        // interface vtables and clear the move/draw ptmf triples. Statement
        // order mirrors the retail load/store interleave.
        __ct__8CProcessFv(item);
        item->vt10 = (void*)&lbl_eu_8052CDB4[0];
        u32* ptmf = (u32*)&__ptmf_null;
        u32 ptmf1 = ptmf[1];
        u32 ptmf0 = ptmf[0];
        u32 ptmf2;
        item->ptmfMove[0] = ptmf0;
        item->ptmfMove[1] = ptmf1;
        ptmf2 = ptmf[2];
        item->ptmfMove[2] = ptmf2;
        ptmf1 = ptmf[1];
        ptmf0 = ptmf[0];
        item->ptmfDraw[0] = ptmf0;
        item->ptmfDraw[1] = ptmf1;
        ptmf2 = ptmf[2];
        item->ptmfDraw[2] = ptmf2;
        // Retail stores the CD50 vtable over BOTH interface slots (+0x24 for
        // the second interface); the earlier vt10 store survives as a dead
        // store in the retail bytes.
        item->vt10 = (void*)&lbl_eu_8052CD50[0];
        item->vt54 = (void*)&lbl_eu_8052CD50[0x24];
        item->state70 = 0;
        item->field78 = 0;
    }
    mRenderItem = item;
    reinterpret_cast<CProcess*>(item)->Regist(
        reinterpret_cast<CProcess*>(GetParent()), false);
}

// ---------- CMenuSymbolMark::Term ----------
// Waits for the previous frame, releases the shared render item, unregisters
// the IScnRender callback, deletes every entry layout, then clears the
// global singleton pointer.
void CMenuSymbolMark::Term() {
    CDeviceVI::waitForDrawDone();
    if (mRenderItem != 0) {
        mRenderItem->flag39 = 1;
        mRenderItem = 0;
    }
    // The `if (this)` idiom splits the IScnRender subobject address
    // (this+0x5C) into its own addi; the call itself is unconditional.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    mScn->removeRenderCB(render);
    for (u8 i = 0; i < 16; i++) {
        SymbolMarkEntry& entry = mEntries[i];
        // Polymorphic delete through the Layout type: MWCC emits the null
        // re-check before the deleting-dtor dispatch (dead second beq).
        if (entry.layout != 0) {
            delete (nw4r::lyt::Layout*)entry.layout;
            entry.layout = 0;
        }
    }
    deleteRegion__17UnkClass_8045F564Fv(&mUnkClass);
    lbl_eu_80663FC8 = 0;
}

// Later-file walkers used by Move.
extern "C" void func_8011EBA8(CMenuSymbolMark* self);
extern "C" void func_8011EC94(CMenuSymbolMark* self);
extern "C" void func_8011EDDC(CMenuSymbolMark* self);
extern "C" void func_8011EFB0(CMenuSymbolMark* self);

// ---------- CMenuSymbolMark::Move ----------
// Raw-word view over an entry used by the reset pass: the world floats are
// written as raw words copied from uninitialized stack locals (retail stw).
struct EntryClearView {
    u8 pad00[4];       // unk00
    u32 unk04;
    u8 pad08[8];       // layout
    u8 flag0;
    u8 flag1;
    u8 pad0E[2];
    u32 worldX;
    u32 worldY;
    u32 worldZ;
    u32 unk1C;
};

// Releases every entry record and hides its pane via FindPaneByName. Macro so
// both call sites expand inline like the retail copies.
#define HIDE_ALL_ENTRY_PANES(jx, jy, jz)                                       \
    {                                                                          \
        const char* sBase = (const char*)&lbl_eu_804FE720[0];                  \
        for (u8 i = 0; i < 16; i++) {                                          \
            EntryClearView& clr =                                              \
                reinterpret_cast<EntryClearView&>(mEntries[i]);                \
            clr.unk04 = 0;                                                     \
            clr.flag0 = 0;                                                     \
            clr.flag1 = 0;                                                     \
            clr.worldX = jx;                                                   \
            clr.worldY = jy;                                                   \
            clr.worldZ = jz;                                                   \
            clr.unk1C = 0;                                                     \
            nw4r::lyt::Pane* pane =                                            \
                (nw4r::lyt::Pane*)((LytLayoutFields*)mEntries[i].layout)       \
                    ->rootPane;                                                \
            if (pane != 0) {                                                   \
                nw4r::lyt::Pane* found = pane->FindPaneByName(sBase + 0x14, true);   \
                if (found != 0) {                                              \
                    ((LytPaneFields*)found)->flagBB &= 0xFE;                   \
                }                                                              \
            }                                                                  \
        }                                                                      \
    }

extern "C" void func_8011EA98(CMenuSymbolMark* self);
extern "C" void func_801209BC(CMenuSymbolMark* self);
extern "C" void func_8011FB68(CMenuSymbolMark* self);
extern "C" void func_8011F8F8(CMenuSymbolMark* self);

// One marker-scan loop body (expanded three times with distinct predicates,
// id lists and mark ids - retail keeps separate stack scratch per copy).
// Claim an entry slot for a live marker and add the mark when the player
// stands within distLimit.
#define SCAN_MARKERS(kindTest, kindId, listArr, countField, argD)              \
    do {                                                                       \
        const f32 distLimit = lbl_eu_806670EC;                                 \
        for (u32 mi = 0; mi < self->countField; mi++) {                         \
            cf::CfObject* actorV =                                              \
                (cf::CfObject*)findObjectById(self->listArr[mi]);                 \
            if (actorV == 0 || actorV->CfObject_UnkVirtualFunc68() == 0) {                         \
                continue;                                                      \
            }                                                                  \
            CfActorFields* actor = (CfActorFields*)actorV;                     \
            if (!(kindTest)) {                                                 \
                continue;                                                      \
            }                                                                  \
            u32 name = actor->name74;                                          \
            u8 count = self->mEntryCount;                                      \
            u8 isNew = 1;                                                      \
            for (u8 k = 0; k < count; k++) {                                   \
                if (self->mEntries[k].unk04 == name) {                         \
                    isNew = 0;                                                 \
                    break;                                                     \
                }                                                              \
            }                                                                  \
            if (!isNew) {                                                      \
                continue;                                                      \
            }                                                                  \
            self->mEntries[count].unk04 = name;                                \
            void* speed = actorV->CfObject_UnkVirtualFunc55(100);                                  \
            Vec* pos = (Vec*)actorV->CfObject_UnkVirtualFunc23();                                  \
            void* player = cf::CfGameManager::getPlayer(0);                    \
            if (player == 0) {                                                 \
                continue;                                                      \
            }                                                                  \
            Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();                 \
            Vec delta;                                                         \
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,                     \
                                (nw4r::math::VEC3*)ppos,                       \
                                (nw4r::math::VEC3*)pos);                        \
            Vec dist = delta;                                                  \
            if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&dist) < distLimit) { \
                func_8011E540(self, (kindId), pos, speed, (argD), 0);           \
            }                                                                  \
        }                                                                      \
    } while (0)

extern "C" void func_8011E540(CMenuSymbolMark* self, u32 id, Vec* pos,
                              void* argC, u32 argD, u32 argE);

// Per-frame update. When the syswin close flag (0xAFA40000) is set the mark
// display expires: once the timer passes the threshold every entry pane is
// hidden again and the id arrays reset. Otherwise the three id-list refresh
// timers tick; when the main timer passes the threshold all entries are
// released and nearby markers (func_8009CF8C gate) re-add themselves.
void CMenuSymbolMark::Move() {
    if ((lbl_eu_80663E28 & 0x200000) != 0) {
        return;
    }
    if (mArchiveFP == 0) {
        return;
    }
    if (func_8013BE50() == 0) {
        return;
    }
    // Raw-word scratch copied into the reset entries (uninitialized stack
    // homes in retail; an aggregate so MWCC gives it a memory home).
    struct JunkTrio {
        u32 x, y, z;
    };
    JunkTrio junkMain, junkExp;
    if (lbl_eu_80663E24 & 0xAFA40000) {
        // Display expired: hide everything once the timer passes.
        if (mTimer <= lbl_eu_806670E0) {
            mTimer = lbl_eu_806670E4;
            mEntryCount = 0;
            HIDE_ALL_ENTRY_PANES(junkExp.x, junkExp.y, junkExp.z);
            mField_49C = lbl_eu_806670D4;
            mField_6A0 = 0;
            mField_6A4 = lbl_eu_806670D8;
            mField_8A8 = 0;
            mField_8AC = lbl_eu_806670DC;
        }
        return;
    }
    func_8011EBA8(this);
    func_8011EC94(this);
    func_8011EDDC(this);
    mTimer += lbl_eu_806670E8;
    if (mTimer >= lbl_eu_806670E0) {
        mTimer = lbl_eu_806670D4;
        mEntryCount = 0;
        HIDE_ALL_ENTRY_PANES(junkMain.x, junkMain.y, junkMain.z);
        func_8011EFB0(this);
        if (func_8009CF8C(0x3354) == 0) {
            CMenuSymbolMark* self = this;
            // Nearby special markers (kind 6) from the second id list.
            SCAN_MARKERS(actor->kind91 == 6, 0xF, mArray6A8, mField_8A8, 0);
            func_8011FB68(this);
            func_8011F8F8(this);
            // Measured markers from the first list (state gate v228 == 3).
            SCAN_MARKERS(((cf::CfObjectMove*)actorV)->CfObjectMove_UnkVirtualFunc25() == 3, 0xC, mArray4A0, mField_6A0, 1);
            // Kind-0xC markers from the second id list.
            SCAN_MARKERS(actor->kind91 == 0xC, 0x10, mArray6A8, mField_8A8, 0);
            func_801209BC(this);
        }
    }
    func_8011EA98(this);
}

// ---------- CMenuSymbolMark::cbRenderBefore ----------
// Render gate chain: task instance check, window flag (bit 0x4000), archive
// pointer, scene gate (func_8013BE50), view flag (bit 0x800), syswin flag
// (0xAFA4 mask), entry count, then render all visible entries through a
// DrawInfo.
void CMenuSymbolMark::cbRenderBefore() {
    // Nested guard reproduces retail's beq-body/b-exit branch pair.
    if (isFlag01Set__9CTaskGameFv(getInstance__9CTaskGameFv()) == 0) {
        if (!(lbl_eu_80663E28 & 0x200000)) {
            goto cont;
        }
    }
    return;
cont:
    if (mArchiveFP == 0) return;
    if (func_8013BE50() == 0) return;
    if (getUnk80664658()->field_214 & 0x100000) return;
    if (lbl_eu_80663E24 & 0xAFA40000) return;
    if (mEntryCount == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via the pre-mangled ct/dt calls
    // (a C++ local would inline the empty ctor and virtual-dispatch the dtor).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    for (u32 i = 0; i < 16; i++) {
        if ((u8)i >= mEntryCount) {
            break;
        }
        SymbolMarkEntry& entry = mEntries[(u8)i];
        if (entry.flag0 != 0) {
            drawLayout((nw4r::lyt::Layout*)entry.layout,
                          (nw4r::lyt::DrawInfo*)&drawInfo[0], entry.flag2, 1);
        }
    }
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
}

// ---------- func_8011E4C4 ----------
extern "C" void* func_8011E4C4(CProcess* scn, CProcess* parent) {
    if (lbl_eu_80663FC8 != 0) {
        return 0;
    }
    CMenuSymbolMark* obj = (CMenuSymbolMark*)mtl::MemManager::allocate(
        2228, (u32)getWorkMem__17CWorkThreadSystemFv());
    if (obj != 0) {
        obj = __ct__CMenuSymbolMark(obj, parent);
    }
    lbl_eu_80663FC8 = (u32)obj;
    reinterpret_cast<CProcess*>(obj)->Regist(scn, false);
    return (void*)lbl_eu_80663FC8;
}

// ---------- func_8011E540 ----------
// Adds a symbol-mark entry. Ids other than 7 run the generic path: flag the
// special kinds, update via func_8011E778, build the pane name string and
// bind its texture before storing the entry. Id 7 instead just re-frames the
// next entry's layout animation from the player distance.
extern "C" void func_8011E540(CMenuSymbolMark* self, u32 id, Vec* pos, void* argC,
                   u32 argD, u32 argE) {
    if (self->mEntryCount >= 0x10) {
        return;
    }
    SymbolMarkEntry* entry = &self->mEntries[self->mEntryCount];
    if (id == 7) {
        cf::CfObject* player = (cf::CfObject*)cf::CfGameManager::getPlayer(0);
        if (player != 0) {
            Vec* ppos = (Vec*)player->CfObject_UnkVirtualFunc23();
            // PS-kernel subtraction + element copy reproduces the retail
            // psq_l/ps_sub block and the stfs copy before PSVECMag.
            nw4r::math::VEC3 delta;
            nw4r::math::VEC3Sub(&delta, (const nw4r::math::VEC3*)pos,
                                (const nw4r::math::VEC3*)ppos);
            Vec diff;
            diff.x = delta.x;
            diff.y = delta.y;
            diff.z = delta.z;
            s32 frames = (s32)PSVECMag(&diff);
            const char* S = (const char*)&lbl_eu_804FE720[0];
            setLayoutTextBoxNumber((nw4r::lyt::Layout*)entry->layout, (char*)S + 0x14,
                          frames);
            SymbolMarkRenderItem* item = self->mRenderItem;
            if (item == 0 || item->state70 == 0) {
                return;
            }
            // State gate reads a separate global; the tracked layout pointer
            // (item->state70) feeds both tail calls directly.
            if (lbl_eu_80664184 == 0x1a) {
                func_80136B4C((nw4r::lyt::Layout*)item->state70, (char*)S + 0x14,
                              (char*)S + 0x1b, 0);
                return;
            }
            setLayoutTextBoxNumber((nw4r::lyt::Layout*)item->state70, (char*)S + 0x14,
                          frames);
            return;
        }
    }
    if (id == 5 || id == 9 || id == 0xa || id == 0xc || id == 0xf ||
        id == 0x10 || (id - 0x15) <= 3) {
        entry->flag2 = 1;
    }
    func_8011E778(self, entry, (nw4r::math::VEC3*)pos,
                  (EntryInputPos*)argC, argE);
    // String pool base materialized again only after the projection helper.
    const char* S = (const char*)&lbl_eu_804FE720[0];
    char* markerName = func_8013639C(self->mAnotherFP, S + 0x7b, id);
    ml::FixStr<32> name;
    name.format(S + 0x86, markerName);
    nw4r::lyt::ArcResourceAccessor* acc = (nw4r::lyt::ArcResourceAccessor*)func_801355F4();
    if (acc->GetResource(0x74696d67 /* "timg" */,
                  (const char*)name.c_str(), 0) != 0) {
        func_80137E7C((nw4r::lyt::Layout*)entry->layout, S + 0x8d);
    }
    entry->unk00 = id;
    entry->flag0 = 1;
    entry->flag1 = (u8)argD;
    entry->worldX = pos->x;
    entry->worldY = pos->y;
    entry->worldZ = pos->z;
    entry->unk1C = (u32)argC;
    self->mEntryCount++;
}

// ---------- func_8011E778 ----------
// Project the entry anchor into screen space, toggle the root pane's visibility
// flag depending on whether the point left the screen range, then either place
// and scale the pane from the measured edge distance (flag2 set) or run the
// bounded vertical slide (flag2 clear). When posSrc is null the world position
// comes straight from worldIn (with a fixed y lift chosen by flag).
extern "C" __declspec(noinline) void func_8011E778(
    CMenuSymbolMark* self, SymbolMarkEntry* entry,
    nw4r::math::VEC3* worldIn, EntryInputPos* posSrc, u32 flag) {
// Re-derive the root pane at every use: the pane stores can alias the layout
// pointer slot, so MWCC reloads it for each store group (matches retail).
#define SYM_ROOT_PANE() (((LytPaneFields*)((LytLayoutFields*)entry->layout)->rootPane))
    nw4r::math::VEC3 worldPos;
    nw4r::math::VEC3 screen;
    nw4r::math::VEC3 t;
    nw4r::math::VEC3 posTmp;
    nw4r::math::VEC3 edge;
    nw4r::math::VEC3* world = worldIn;
    if (posSrc != 0) {
        posTmp.z = posSrc->posZ;
        posTmp.y = posSrc->posY;
        posTmp.x = posSrc->posX;
        world = &posTmp;
    }
    worldPos = *world;
    ScnXformBlock* pose = (ScnXformBlock*)func_80496264(self->mScn, -1);
    if (posSrc == 0) {
        if (flag != 0) {
            worldPos.y += lbl_eu_806670F0;
        } else {
            worldPos.y += lbl_eu_806670E8;
        }
    }
    func_8049B59C(&screen, pose, &worldPos);
    // Hidden only when both projected coordinates exceed the bound.
    LytPaneFields* pane = ((LytLayoutFields*)entry->layout)->rootPane;
    if (screen.x <= lbl_eu_806670D4 || screen.y <= lbl_eu_806670D4) {
        pane->flagBB &= 0xFE;
    } else {
        pane->flagBB |= 1;
    }
    if (entry->flag2 != 0) {
        // Distance-based placement: transform a unit offset by the pose matrix,
        // project the far edge and derive an inverse-distance scale.
        t.x = lbl_eu_806670E8;
        t.y = lbl_eu_806670E8;
        t.z = lbl_eu_806670D4;
        VEC3TransformNormal(&t, (const nw4r::math::MTX34*)&pose->mtx9C, &t);
        edge = worldPos + t;
        func_8049B59C(&screen, pose, &edge);
        f32 scaleX = lbl_eu_806670E8 / __fabs(screen.x - t.x);
        f32 scaleY = lbl_eu_806670E8 / __fabs(screen.y - t.y);
        if (!CDeviceVI::isWideAspectRatio()) {
            scaleX *= lbl_eu_806670F4;
        }
        f32 baseScale = lbl_eu_806670F8 / lbl_eu_8066A1F8;
        SYM_ROOT_PANE()->trX = worldPos.x;
        SYM_ROOT_PANE()->trY = worldPos.y;
        SYM_ROOT_PANE()->trZ = worldPos.z;
        nw4r::math::VEC3 sc =
            *reinterpret_cast<const nw4r::math::VEC3*>(&pose->proj118[0]) *
            baseScale;
        SYM_ROOT_PANE()->rotX = sc.x;
        SYM_ROOT_PANE()->rotY = sc.y;
        SYM_ROOT_PANE()->rotZ = sc.z;
        SYM_ROOT_PANE()->scX = scaleX;
        SYM_ROOT_PANE()->scY = scaleY;
    } else {
        // Vertical slide clamped into a band around the screen edge.
        f32 slideY = (screen.y - lbl_eu_80667100) * lbl_eu_80667104;
        f32 shiftedX = screen.x - lbl_eu_806670FC;
        f32 z = lbl_eu_806670D4;
        f32 limit = lbl_eu_80667108 + slideY;
        screen.y = slideY;
        screen.x = shiftedX;
        if (limit > lbl_eu_80667100) {
            screen.y = slideY - (limit - lbl_eu_80667100);
        } else {
            f32 g = slideY - lbl_eu_80667108;
            if (g < lbl_eu_8066710C) {
                screen.y = slideY + (lbl_eu_8066710C - g);
            }
        }
        pane->trX = screen.x;
        pane->trY = screen.y;
        pane->trZ = z;
    }
}

// ---------- func_8011EA98 ----------
// Per-entry update: resolves the actor bound to the entry (findObjectById on
// unk04) when flag1 is set; entries without an actor are released. Live
// actors are polled (vtable 0x160 gate for ids 9-10) and their measured
// values (vtable 0x12C / 0xAC) are forwarded to func_8011E778.
extern "C" void func_8011EA98(CMenuSymbolMark* self) {
    void* value12c;
    u32 zero = 0;
    u8 i;
    for (i = 0; i < self->mEntryCount; i++) {
        SymbolMarkEntry* entry = &self->mEntries[i];
        if (entry->flag1 != 0) {
            cf::CfObject* actor = (cf::CfObject*)findObjectById(entry->unk04);
            if (actor == 0) {
                entry->flag0 = zero;
                continue;
            }
            // unk00 == 9 or 10: gate on the actor's live flag (vtable 0x160).
            if ((entry->unk00 - 9) <= 1 && actor->CfObject_UnkVirtualFunc68() == 0) {
                entry->flag0 = zero;
                continue;
            }
            value12c = actor->CfObject_UnkVirtualFunc55(100);
            Vec* valueAC = (Vec*)actor->CfObject_UnkVirtualFunc23();
            func_8011E778(self, entry, (nw4r::math::VEC3*)valueAC,
                          (EntryInputPos*)value12c, zero);
        } else {
            func_8011E778(self, entry, (nw4r::math::VEC3*)&entry->worldX,
                          (EntryInputPos*)entry->unk1C, zero);
        }
    }
}

// ---------- func_8011EBA8 ----------
// Timer tick: accumulates into mField_49C until it passes the threshold,
// then resets and rebuilds the unlocked-id list in mBuffer from the archive.
extern "C" void func_8011EBA8(CMenuSymbolMark* self) {
    // Increment applied first; body runs only past the threshold.
    self->mField_49C = self->mField_49C + lbl_eu_806670E8;
    if (self->mField_49C < lbl_eu_806670E0) {
        return;
    }
    self->mField_49C = lbl_eu_806670D4;
    self->mField_498 = 0;
    memset(self->mBuffer, 0, 0x200);
    // Archive pointer and string base are hoisted into nonvolatile regs.
    const char* names = (const char*)&lbl_eu_804FE720[0];
    void* archive = self->mArchiveFP;
    for (u32 id = 1; id <= self->mSomeValue2; id++) {
        if (func_80138574(archive, id) == 0) {
            continue;
        }
        if ((u8)func_801361E8((u32)archive, names + 0x96, id) == 0) {
            continue;
        }
        u32 n = self->mField_498;
        self->mBuffer[n] = id;
        u32 next = n + 1;
        self->mField_498 = next;
        if (next >= 0x80) {
            return;
        }
    }
}

// ---------- func_8011EC94 ----------
// Timer tick over mField_6A4/mField_6A0; once the threshold passes it resets,
// zeroes the id array and rebuilds it from the marker list returned by
// func_800B6BEC.
extern "C" void func_8011EC94(CMenuSymbolMark* self) {
    // Global loaded before the object field to match MWCC's operand order.
    f32 t = self->mField_6A4 + lbl_eu_806670E8;
    self->mField_6A4 = t;
    if (t < lbl_eu_806670E0) {
        return;
    }
    self->mField_6A4 = lbl_eu_806670D4;
    self->mField_6A0 = 0;
    // Retail expands the inner 32-element clear fully and keeps the 4-iteration
    // outer loop as a countdown on an advancing row pointer (+0x80 per pass).
    u32* row = &self->mArray4A0[0];
    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 0x20; j++) {
            row[j] = 0;
        }
        row += 0x20;
    }
    if (cf::CfGameManager::getPlayer(0) == 0) {
        return;
    }
    CfObjList* list = (CfObjList*)func_800B6BEC();
    for (CfObjListNode* node = list->sentinel->next; node != list->sentinel;
         node = node->next) {
        CfMarkerFields* obj = (CfMarkerFields*)node->object;
        if (obj == 0) {
            continue;
        }
        u32 n = self->mField_6A0;
        self->mArray4A0[n] = obj->name74;
        n++;
        self->mField_6A0 = n;
        if (n >= 0x80) {
            return;
        }
    }
}

// ---------- func_8011EDDC ----------
// Same timer shape as func_8011EC94 but over the second array pair; rebuilds
// from the func_800B6C58 list, keeping only objects whose +0x64 flags carry
// bit 0x10000 or 0x20000 and whose measured position lands inside the scene
// bounds box tested by func_8013A4B4.
extern "C" void func_8011EDDC(CMenuSymbolMark* self) {
    f32 t = self->mField_8AC + lbl_eu_806670E8;
    self->mField_8AC = t;
    if (t < lbl_eu_806670E0) {
        return;
    }
    self->mField_8AC = lbl_eu_806670D4;
    self->mField_8A8 = 0;
    // Retail's ctr=4 / 32-stw run is MWCC's x32 unroll of a flat zero loop.
    for (u32 i = 0; i < 0x80; i++) {
        self->mArray6A8[i] = 0;
    }
    if (cf::CfGameManager::getPlayer(0) == 0) {
        return;
    }
    CfObjList* list = (CfObjList*)func_800B6C58();
    Vec* pos;
    for (CfObjListNode* node = list->sentinel->next; node != list->sentinel;
         node = node->next) {
        CfMarkerFields* obj = (CfMarkerFields*)node->object;
        if (obj == 0) {
            continue;
        }
        u32 flags = obj->flags64;
        if (!(flags & 0x4000)) {
            if (!(flags & 0x8000)) {
                continue;
            }
        }
        // Anchor position of the object (vtable slot 0xAC); the scene bounds
        // query runs inside the loop, after the position is captured.
        pos = (Vec*)((cf::CfObject*)obj)->CfObject_UnkVirtualFunc23();
        ScnObjPositions* scn =
            (ScnObjPositions*)func_80496264(self->mScn, -1);
        Vec extent;
        Vec anchor;
        anchor.x = scn->posX;
        anchor.y = scn->posY;
        anchor.z = scn->posZ;
        extent.x = scn->extX;
        extent.y = scn->extY;
        extent.z = scn->extZ;
        if (func_8013A4B4(&anchor, &extent, pos) == 0) {
            continue;
        }
        u32 n = self->mField_8A8;
        self->mArray6A8[n] = obj->name74;
        u32 next = n + 1;
        self->mField_8A8 = next;
        if (next >= 0x80) {
            return;
        }
    }
}

// ---------- func_8011EFB0 ----------
// Shared post-add sequence: flag the render item live, snapshot the anchor
// position into item+0x60, refresh the unlock state (func_8011D338 over
// item+0x70) and toggle the two marker panes to the resulting bool.
#define SYMBOL_ITEM_TAIL(selfP, srcExpr)                                       \
    do {                                                                       \
        SymbolMarkRenderItem* it = (selfP)->mRenderItem;                       \
        it->field6C = 1;                                                       \
        const u32* tailSrc = (const u32*)(srcExpr);                            \
        if (tailSrc != 0) {                                                    \
            it->field60 = tailSrc[0];                                          \
            it->field64 = tailSrc[1];                                          \
            it->field68 = tailSrc[2];                                          \
        }                                                                      \
        u8 ok = (func_8011D338((u32)&it->state70) == 0);                       \
        it->field6C = ok;                                                      \
        it->field74 = ok;                                                      \
        const char* Sb = (const char*)&lbl_eu_804FE720[0];                     \
        nw4r::lyt::Layout* lay2 = (nw4r::lyt::Layout*)it->state70;             \
        lay2->GetRootPane()->FindPaneByName(Sb + 0x14, true)->SetVisible(ok);  \
        lay2->GetRootPane()->FindPaneByName(Sb + 0x1C, true)->SetVisible(ok);  \
    } while (0)

// Entry-slot ownership scan: true when no entry is bound to `nameV` yet.
#define SYMBOL_ENTRY_FREE(selfP, nameV)                                        \
    ({                                                                         \
    })

extern "C" void func_8011EFB0(CMenuSymbolMark* self) {
    const char* S = (const char*)&lbl_eu_804FE720[0];
    SymbolMarkRenderItem* item = self->mRenderItem;
    // Reset the live flags and hide both marker panes before the re-scan.
    item->field6C = 0;
    item->field74 = 0;
    // Re-derive the layout/root pane at every use; nothing spans the calls.
    ((nw4r::lyt::Layout*)item->state70)
        ->GetRootPane()
        ->FindPaneByName(S + 0x14, true)
        ->SetVisible(false);
    ((nw4r::lyt::Layout*)item->state70)
        ->GetRootPane()
        ->FindPaneByName(S + 0x1C, true)
        ->SetVisible(false);

    int kind = func_8009CF8C(0x20);
    if (kind <= 0) {
        return;
    }
    // Only when the current map id matches the archive row for this kind.
    u8 curMap = (u8)lbl_eu_80664184;
    if ((u8)func_801361E8((u32)self->mSomeFP, S + 0x9B, kind) != curMap) {
        return;
    }
    u32 sw = func_801361E8((u32)self->mSomeFP, S + 0x96, kind);
    u32 meas = func_80136254((const void*)self->mSomeFP, S + 0x9F, kind);
    switch ((u8)sw) {
    case 1: {
        // Live-marker list: first actor without an owned slot claims one and
        // adds a kind-7 mark.
        CfObjList* list = (CfObjList*)func_800B6CF8(sw);
        for (CfObjListNode* node = list->sentinel->next;
             node != list->sentinel; node = node->next) {
            cf::CfObject* actorV = (cf::CfObject*)node->object;
            CfActorFields* actor = (CfActorFields*)actorV;
            u32 name = actor->name74;
            u8 count = self->mEntryCount;
            bool isNew = true;
            for (u8 k = 0; k < count; k++) {
                if (self->mEntries[k].unk04 == name) {
                    isNew = false;
                    break;
                }
            }
            if (!isNew) {
                return;
            }
            self->mEntries[count].unk04 = name;
            void* speed = actorV->CfObject_UnkVirtualFunc55(100);
            Vec* pos = (Vec*)actorV->CfObject_UnkVirtualFunc23();
            func_8011E540(self, 7, pos, speed, 0, 0);
            SYMBOL_ITEM_TAIL(self, actorV->CfObject_UnkVirtualFunc23());
            return;
        }
        return;
    }
    case 2: {
        if ((u16)meas == 0) {
            return;
        }
        // Scene menu table: scan fixed rows for a matching measured id; the
        // matched row itself feeds the mark position and anchor snapshot.
        SceneMenuTable* tbl = (SceneMenuTable*)func_80193804();
        MenuTableRow* rowEnd =
            (MenuTableRow*)((u8*)tbl + tbl->rowCount9800 * 0x4C);
        MenuTableRow* row = (MenuTableRow*)tbl;
        while (row != rowEnd) {
            u16 id = row->field1C;
            if (id != (u16)meas) {
                row++;
                continue;
            }
            u8 count = self->mEntryCount;
            bool isNew = true;
            for (u8 k = 0; k < count; k++) {
                if (self->mEntries[k].unk04 == id) {
                    isNew = false;
                    break;
                }
            }
            if (!isNew) {
                return;
            }
            self->mEntries[count].unk04 = id;
            func_8011E540(self, 7, (Vec*)row, 0, 0, 0);
            SYMBOL_ITEM_TAIL(self, row);
            return;
        }
        return;
    }
    case 3: {
        if ((u16)meas == 0) {
            return;
        }
        CfObjList* list = (CfObjList*)func_800B6C58();
        for (CfObjListNode* node = list->sentinel->next;
             node != list->sentinel; node = node->next) {
            CfMarkerFields* obj = (CfMarkerFields*)node->object;
            u32 flags = obj->flags64;
            if (!(flags & 0x4000)) {
                if (!(flags & 0x8000)) {
                    continue;
                }
            }
            CfActorFields* actor = (CfActorFields*)obj;
            if (actor->field8C != (u16)meas) {
                continue;
            }
            cf::CfObject* actorV = (cf::CfObject*)obj;
            u32 name = actor->name74;
            u8 count = self->mEntryCount;
            bool isNew = true;
            for (u8 k = 0; k < count; k++) {
                if (self->mEntries[k].unk04 == name) {
                    isNew = false;
                    break;
                }
            }
            if (!isNew) {
                return;
            }
            self->mEntries[count].unk04 = name;
            void* speed = actorV->CfObject_UnkVirtualFunc55(100);
            Vec* pos = (Vec*)actorV->CfObject_UnkVirtualFunc23();
            func_8011E540(self, 7, pos, speed, 0, 0);
            SYMBOL_ITEM_TAIL(self, actorV->CfObject_UnkVirtualFunc23());
            return;
        }
        return;
    }
    case 4: {
        if ((u16)meas == 0) {
            return;
        }
        CfObjList* list = (CfObjList*)func_800B6BEC();
        for (CfObjListNode* node = list->sentinel->next;
             node != list->sentinel; node = node->next) {
            CfActorFields* actor = (CfActorFields*)node->object;
            if (actor->field8C != (u16)meas) {
                continue;
            }
            cf::CfObject* actorV = (cf::CfObject*)actor;
            u32 name = actor->name74;
            u8 count = self->mEntryCount;
            bool isNew = true;
            for (u8 k = 0; k < count; k++) {
                if (self->mEntries[k].unk04 == name) {
                    isNew = false;
                    break;
                }
            }
            if (!isNew) {
                return;
            }
            self->mEntries[count].unk04 = name;
            void* speed = actorV->CfObject_UnkVirtualFunc55(100);
            Vec* pos = (Vec*)actorV->CfObject_UnkVirtualFunc23();
            func_8011E540(self, 7, pos, speed, 1, 0);
            SYMBOL_ITEM_TAIL(self, actorV->CfObject_UnkVirtualFunc23());
            return;
        }
        return;
    }
    case 5: {
        if ((u16)meas == 0) {
            return;
        }
        CfObjList* list = (CfObjList*)func_800B6BEC();
        for (CfObjListNode* node = list->sentinel->next;
             node != list->sentinel; node = node->next) {
            CfActorFields* actor = (CfActorFields*)node->object;
            if (actor->field8C != (u16)meas) {
                continue;
            }
            cf::CfObject* actorV = (cf::CfObject*)actor;
            u32 name = actor->name74;
            u8 count = self->mEntryCount;
            bool isNew = true;
            for (u8 k = 0; k < count; k++) {
                if (self->mEntries[k].unk04 == name) {
                    isNew = false;
                    break;
                }
            }
            if (!isNew) {
                return;
            }
            self->mEntries[count].unk04 = name;
            void* speed = actorV->CfObject_UnkVirtualFunc55(100);
            Vec* pos = (Vec*)actorV->CfObject_UnkVirtualFunc23();
            func_8011E540(self, 7, pos, speed, 1, 0);
            SYMBOL_ITEM_TAIL(self, actorV->CfObject_UnkVirtualFunc23());
            return;
        }
        return;
    }
    default:
        return;
    }
}

// ---------- func_8011F8F8 ----------
// Walks the unlocked-id list (mArray4A0): for each live marker whose measured
// value matches a table row inside the scene bounds box, claim an entry slot
// and add kind-0xa marks when the player stands within range.
extern "C" void func_8011F8F8(CMenuSymbolMark* self) {
    const f32 distLimit = lbl_eu_806670EC;
    // Function-scope diff buffers; MWCC pins their addresses pre-loop
    // (retail anchors sp+8 / sp+0x14 across the whole body).
    Vec scratch;
    Vec delta;
    Vec anchor;
    Vec extent;
    for (u32 i = 0; i < self->mField_6A0; i++) {
        cf::CfObject* actorV = (cf::CfObject*)findObjectById(self->mArray4A0[i]);
        if (actorV == 0 || actorV->CfObject_UnkVirtualFunc68() == 0) {
            continue;
        }
        CfActorFields* actor = (CfActorFields*)actorV;
        for (u32 j = self->mSomeValue3; j < self->mSomeValue4; j++) {
            u16 id =
                (u16)func_80136254((const void*)self->mSomeValue,
                                   (const char*)&lbl_eu_804FE720[0xa6], j);
            if (id != actor->field8C) {
                continue;
            }
            if (func_80138234((const void*)self->mSomeValue, j) == 0) {
                continue;
            }
            Vec* pos = (Vec*)actorV->CfObject_UnkVirtualFunc23();
            ScnObjPositions* scn =
                (ScnObjPositions*)func_80496264(self->mScn, -1);
            anchor.x = scn->posX;
            anchor.y = scn->posY;
            anchor.z = scn->posZ;
            extent.x = scn->extX;
            extent.y = scn->extY;
            extent.z = scn->extZ;
            if (func_8013A4B4(&anchor, &extent, pos) == 0) {
                continue;
            }
            // Skip if this actor already owns an entry slot (isNew: 0 on
            // match, sunk li 1 on scan-exhausted path - retail r0 flag).
            u32 name = actor->name74;
            u32 count = self->mEntryCount;
            u32 isNew = 1;
            for (u32 k = 0; k < count; k++) {
                if (self->mEntries[k].unk04 == name) {
                    isNew = 0;
                    break;
                }
            }
            if (isNew == 0) {
                continue;
            }
            self->mEntries[count].unk04 = name;
            void* speed = actorV->CfObject_UnkVirtualFunc55(100);
            Vec* pos2 = (Vec*)actorV->CfObject_UnkVirtualFunc23();
            void* player = cf::CfGameManager::getPlayer(0);
            if (player == 0) {
                continue;
            }
            Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,
                                (nw4r::math::VEC3*)ppos,
                                (nw4r::math::VEC3*)pos2);
            scratch = delta;
            if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&scratch) >=
                distLimit) {
                continue;
            }
            func_8011E540(self, 0xa, pos2, speed, 1, 0);
        }
    }
}

// ---------- func_8011FB68 ----------
// Refresh pass over the unlocked-id list (mBuffer): first syncs the two
// shared marker panes with the unlock state, then for each buffered id
// resolves its dispatch kind from the archive keys and scans the matching
// live list / scene table to claim entry slots and drop symbol marks when
// the player stands within range.
// Retail imports used only here.
extern "C" void func_80141DC4(Vec* out, int id);
extern "C" s16 func_80136330(const void* fp, const char* str, u32 id);
extern "C" u32 func_8013C038(u16 id);
extern "C" void* func_801F4E68(CMenuGimmickGlobal* mgr, u16 id);

// Bounds test: build the anchor/extent pair from the scene pose block and
// query func_8013A4B4 against the world point.
#define SYM_IN_SCENE_BOUNDS(scnPose, posPtr)                                   \
    ({                                                                          \
        ScnObjPositions* scnQ = (ScnObjPositions*)(scnPose);                     \
        Vec ancQ, extQ;                                                          \
        ancQ.x = scnQ->posX;                                                     \
        ancQ.y = scnQ->posY;                                                     \
        ancQ.z = scnQ->posZ;                                                     \
        extQ.x = scnQ->extX;                                                     \
        extQ.y = scnQ->extY;                                                     \
        extQ.z = scnQ->extZ;                                                     \
        func_8013A4B4(&ancQ, &extQ, (posPtr)) != 0;                              \
    })

// Entry-slot dup scan: false when some entry already owns nameV.
#define SYM_ENTRY_DUP(selfP, nameV)                                            \
    ({                                                                          \
        bool dupQ = false;                                                       \
        u8 cntQ = (selfP)->mEntryCount;                                          \
        for (u8 kQ = 0; kQ < cntQ; kQ++) {                                       \
            if ((selfP)->mEntries[kQ].unk04 == (u32)(nameV)) {                   \
                dupQ = true;                                                     \
                break;                                                           \
            }                                                                    \
        }                                                                        \
        dupQ;                                                                    \
    })

// Signed s16 -> double conversion through MWCC's 0x4330/extsh/xoris bit
// trick, subtracting the shared retail magic so the pool label names
// lbl_eu_80667110 instead of an anonymous TU-local constant.
static double SymConvS16ToF64(s16 x, const f64& bias) {
    union {
        double d;
        u32 w[2];
    } u;
    u.w[0] = 0x43300000;
    u.w[1] = (u32)x ^ 0x80000000;
    return u.d - bias;
}

void func_8011FB68(CMenuSymbolMark* self) {
    SymbolMarkRenderItem* item = self->mRenderItem;
    // String pool base scoped to the pane-sync block only - retail frees this
    // register afterwards and re-materializes the base inside the id loop.
    const char* sHead = (const char*)&lbl_eu_804FE720[0];
    // Sync the shared marker panes: always hide the S+0x26 pane; show it
    // (and hide S+0x14) only while the unlock scan reports a live target.
    // Every FindPaneByName result is consumed immediately - no pane locals
    // survive the block (keeps MWCC's nonvolatile allocation aligned).
    ((nw4r::lyt::Layout*)item->state70)
        ->GetRootPane()
        ->FindPaneByName(sHead + 0x26, true)
        ->SetVisible(false);
    if (!((nw4r::lyt::Layout*)item->state70)
             ->GetRootPane()
             ->FindPaneByName(sHead + 0x14, true)
             ->IsVisible()) {
        if (func_8011D338((u32)&item->state70) != 0) {
            ((nw4r::lyt::Layout*)item->state70)
                ->GetRootPane()
                ->FindPaneByName(sHead + 0x26, true)
                ->SetVisible(true);
            ((nw4r::lyt::Layout*)item->state70)
                ->GetRootPane()
                ->FindPaneByName(sHead + 0x14, true)
                ->SetVisible(false);
        }
    }
    // Fresh copies hoisted out of the id loop (retail pins both in
    // nonvolatiles before the loop header).
    const char* S = (const char*)&lbl_eu_804FE720[0];
    // Loaded before the distance limit so MWCC pins the magic double into
    // f31 ahead of the f30 limit load (retail pool order).
    const f64 convBias = lbl_eu_80667110;
    void* archive = self->mArchiveFP;
    const f32 distLimit = lbl_eu_806670EC;
    for (u32 i = 0; i < self->mField_498; i++) {
        u32 id = self->mBuffer[i];
        // Per-id dispatch keys out of the archive.
        u32 key = func_801361E8((u32)archive, S + 0x96, id);
        u32 meas = func_80136254(archive, S + 0x9f, id);
        u32 measB = func_80136254(archive, S + 0xad, id);
        void* row = (void*)lbl_eu_80573D18[func_80138138((u16)measB)];
        u32 kind = (u8)func_801361E8((u32)row, S + 0x2f, (u16)measB);
        bool kindNonzero = kind != 0;
        bool kindOk = false;
        if (!kindNonzero) {
            // Fallback key: retry the kind lookup through the alternate id.
            measB = func_80136254(archive, S + 0xb6, id);
            if ((u16)measB != 0) {
                kind = (u8)func_801361E8((u32)row, S + 0x2f, (u16)measB);
                kindNonzero = kind != 0;
                if (!kindNonzero && func_8009CF8C((u16)measB + 0x220) == 0) {
                    kindOk = true;
                }
            }
        }
        switch ((u8)key) {
        case 1:
        case 2: {
            Vec anchor, extent, delta, distV;
            // Actor lists: kind-5 marks claim unconditionally; otherwise a
            // distance-gated 9/10 mark. Case 1 walks mArray4A0, case 2
            // mArray6A8 (argD mirrors the case number's flag).
            const bool isFirst = (u8)key == 1;
            u32* arr = isFirst ? self->mArray4A0 : self->mArray6A8;
            u32 nActors = isFirst ? self->mField_6A0 : self->mField_8A8;
            const u32 argD = isFirst ? 1 : 0;
            for (u32 j = 0; j < nActors; j++) {
                cf::CfObject* actorV = (cf::CfObject*)findObjectById(arr[j]);
                if (actorV == 0) {
                    continue;
                }
                if (!kindNonzero && actorV->CfObject_UnkVirtualFunc68() == 0) {
                    continue;
                }
                CfActorFields* actor = (CfActorFields*)actorV;
                if (actor->field8C != (u16)meas) {
                    continue;
                }
                Vec* pos = (Vec*)actorV->CfObject_UnkVirtualFunc23();
                if (!SYM_IN_SCENE_BOUNDS(func_80496264(self->mScn, -1), pos)) {
                    continue;
                }
                u32 name = actor->name74;
                if (SYM_ENTRY_DUP(self, name)) {
                    continue;
                }
                self->mEntries[self->mEntryCount].unk04 = name;
                if (kindNonzero) {
                    void* speed = actorV->CfObject_UnkVirtualFunc55(100);
                    func_8011E540(self, 5, pos, speed, argD, 0);
                } else {
                    u32 markId = kindOk ? 0xa : 9;
                    void* speed = actorV->CfObject_UnkVirtualFunc55(100);
                    Vec* pos2 = (Vec*)actorV->CfObject_UnkVirtualFunc23();
                    void* player = cf::CfGameManager::getPlayer(0);
                    if (player == 0) {
                        goto next_id;
                    }
                    Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
                    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,
                                        (nw4r::math::VEC3*)ppos,
                                        (nw4r::math::VEC3*)pos2);
                    distV = delta;
                    if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&distV) >=
                        distLimit) {
                        goto next_id;
                    }
                    func_8011E540(self, markId, pos2, speed, argD, 0);
                }
                break;
            }
            break;
        }
        case 3: {
            Vec anchor, extent, delta, distV;
            // Scene menu table rows: match on the row's measured-id halfword,
            // the row itself serving as both position source and entry name.
            MenuTableRow* tbl = (MenuTableRow*)func_80193804();
            MenuTableRow* rowEnd =
                (MenuTableRow*)((u8*)tbl + ((SceneMenuTable*)tbl)->rowCount9800 * 0x4C);
            for (; tbl != rowEnd; tbl++) {
                u16 rowId = tbl->field1C;
                if (rowId != (u16)meas) {
                    continue;
                }
                Vec* pos = (Vec*)tbl;
                if (!SYM_IN_SCENE_BOUNDS(func_80496264(self->mScn, -1), pos)) {
                    continue;
                }
                if (SYM_ENTRY_DUP(self, rowId)) {
                    continue;
                }
                self->mEntries[self->mEntryCount].unk04 = rowId;
                if (kindNonzero) {
                    func_8011E540(self, 5, pos, 0, 0, 0);
                } else {
                    u32 markId = kindOk ? 0xa : 9;
                    void* player = cf::CfGameManager::getPlayer(0);
                    if (player == 0) {
                        goto next_id;
                    }
                    Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
                    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,
                                        (nw4r::math::VEC3*)ppos,
                                        (nw4r::math::VEC3*)pos);
                    distV = delta;
                    if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&distV) >=
                        distLimit) {
                        goto next_id;
                    }
                    func_8011E540(self, markId, pos, 0, 0, 0);
                }
                break;
            }
            break;
        }
        case 4: {
            Vec anchor, extent, delta, distV;
            // Gimmick world position resolved by measured id.
            Vec worldPos;
            func_80141DC4(&worldPos, (u16)meas);
            if (SYM_ENTRY_DUP(self, id)) {
                break;
            }
            if (!SYM_IN_SCENE_BOUNDS(func_80496264(self->mScn, -1), &worldPos)) {
                break;
            }
            self->mEntries[self->mEntryCount].unk04 = id;
            if (kindNonzero) {
                func_8011E540(self, 5, &worldPos, 0, 0, 0);
            } else {
                u32 markId = kindOk ? 0xa : 9;
                void* player = cf::CfGameManager::getPlayer(0);
                if (player == 0) {
                    goto next_id;
                }
                Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
                nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,
                                    (nw4r::math::VEC3*)ppos,
                                    (nw4r::math::VEC3*)&worldPos);
                distV = delta;
                if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&distV) >=
                    distLimit) {
                    goto next_id;
                }
                func_8011E540(self, markId, &worldPos, 0, 0, 0);
            }
            break;
        }
        case 5: {
            Vec anchor, extent, delta, distV;
            // BDAT coordinate triple keyed off the static file table.
            if (SYM_ENTRY_DUP(self, id)) {
                break;
            }
            void* fp = getFP(S + 0xc0);
            func_80136254(fp, S + 0xcf, id);
            if (func_8009CF8C(id + 0x2b9c) != 0) {
                break;
            }
            Vec rawPos;
            rawPos.x = SymConvS16ToF64(func_80136330(fp, S + 0xd6, id), convBias);
            rawPos.y = SymConvS16ToF64(func_80136330(fp, S + 0xdb, id), convBias);
            rawPos.z = SymConvS16ToF64(func_80136330(fp, S + 0xe0, id), convBias);
            if (!SYM_IN_SCENE_BOUNDS(func_80496264(self->mScn, -1), &rawPos)) {
                break;
            }
            self->mEntries[self->mEntryCount].unk04 = id;
            if (kindNonzero) {
                Vec posA = rawPos;
                func_8011E540(self, 5, &posA, 0, 0, 0);
            } else {
                u32 markId = kindOk ? 0xa : 9;
                void* player = cf::CfGameManager::getPlayer(0);
                if (player == 0) {
                    goto next_id;
                }
                Vec posB = rawPos;
                Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
                nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,
                                    (nw4r::math::VEC3*)ppos,
                                    (nw4r::math::VEC3*)&posB);
                distV = delta;
                if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&distV) >=
                    distLimit) {
                    goto next_id;
                }
                func_8011E540(self, markId, &posB, 0, 0, 0);
            }
            break;
        }
        case 6: {
            Vec anchor, extent, delta, distV;
            // Gimmick record triple from the minimap gimmick manager.
            if (SYM_ENTRY_DUP(self, id)) {
                break;
            }
            if (func_8013C038((u16)id) != 0) {
                break;
            }
            struct FloatTrio {
                f32 x, y, z;
            };
            FloatTrio rec = *(FloatTrio*)func_801F4E68(getUnk80664658(), (u16)id);
            Vec recPos;
            recPos.x = rec.x;
            recPos.y = rec.y;
            recPos.z = rec.z;
            if (!SYM_IN_SCENE_BOUNDS(func_80496264(self->mScn, -1), &recPos)) {
                break;
            }
            self->mEntries[self->mEntryCount].unk04 = id;
            if (kindNonzero) {
                Vec posA = recPos;
                func_8011E540(self, 5, &posA, 0, 0, 0);
            } else {
                u32 markId = kindOk ? 0xa : 9;
                void* player = cf::CfGameManager::getPlayer(0);
                if (player == 0) {
                    goto next_id;
                }
                Vec posB = recPos;
                Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
                nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,
                                    (nw4r::math::VEC3*)ppos,
                                    (nw4r::math::VEC3*)&posB);
                distV = delta;
                if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&distV) >=
                    distLimit) {
                    goto next_id;
                }
                func_8011E540(self, markId, &posB, 0, 0, 0);
            }
            break;
        }
        default:
            break;
        }
    next_id:;
    }
}

// ---------- func_801209BC ----------
extern "C" void func_801209BC(CMenuSymbolMark* self) {
    const f32 distLimit = lbl_eu_806670EC;
    for (u32 i = 0; i < self->mField_8A8; i++) {
        cf::CfObject* actorV = (cf::CfObject*)findObjectById(self->mArray6A8[i]);
        if (actorV == 0 || actorV->CfObject_UnkVirtualFunc68() == 0) {
            continue;
        }
        CfActorFields* actor = (CfActorFields*)actorV;
        switch (actor->kind91) {
        case 0xE: {
            // Claim an entry slot unless this actor already owns one.
            u32 name = actor->name74;
            u8 count = self->mEntryCount;
            bool isNew = true;
            for (u8 k = 0; k < count; k++) {
                if (self->mEntries[k].unk04 == name) {
                    isNew = false;
                    break;
                }
            }
            if (!isNew) {
                continue;
            }
            self->mEntries[count].unk04 = name;
            void* speed = actorV->CfObject_UnkVirtualFunc55(100);
            Vec* apos = (Vec*)actorV->CfObject_UnkVirtualFunc23();
            void* player = cf::CfGameManager::getPlayer(0);
            if (player == 0) {
                continue;
            }
            Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
            Vec delta;
            Vec scratch;
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,
                                (nw4r::math::VEC3*)ppos,
                                (nw4r::math::VEC3*)apos);
            scratch = delta;
            if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&scratch) >=
                distLimit) {
                continue;
            }
            func_8011E540(self, 0x17, apos, speed, 0, 0);
            break;
        }
        case 0xA: {
            u32 name = actor->name74;
            u8 count = self->mEntryCount;
            bool isNew = true;
            for (u8 k = 0; k < count; k++) {
                if (self->mEntries[k].unk04 == name) {
                    isNew = false;
                    break;
                }
            }
            if (!isNew) {
                continue;
            }
            self->mEntries[count].unk04 = name;
            void* speed = actorV->CfObject_UnkVirtualFunc55(100);
            Vec* apos = (Vec*)actorV->CfObject_UnkVirtualFunc23();
            void* player = cf::CfGameManager::getPlayer(0);
            if (player == 0) {
                continue;
            }
            Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
            Vec delta;
            Vec scratch;
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,
                                (nw4r::math::VEC3*)ppos,
                                (nw4r::math::VEC3*)apos);
            scratch = delta;
            if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&scratch) >=
                distLimit) {
                continue;
            }
            func_8011E540(self, 0x15, apos, speed, 0, 0);
            break;
        }
        case 0xB: {
            u32 name = actor->name74;
            u8 count = self->mEntryCount;
            bool isNew = true;
            for (u8 k = 0; k < count; k++) {
                if (self->mEntries[k].unk04 == name) {
                    isNew = false;
                    break;
                }
            }
            if (!isNew) {
                continue;
            }
            self->mEntries[count].unk04 = name;
            void* speed = actorV->CfObject_UnkVirtualFunc55(100);
            Vec* apos = (Vec*)actorV->CfObject_UnkVirtualFunc23();
            void* player = cf::CfGameManager::getPlayer(0);
            if (player == 0) {
                continue;
            }
            Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
            Vec delta;
            Vec scratch;
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,
                                (nw4r::math::VEC3*)ppos,
                                (nw4r::math::VEC3*)apos);
            scratch = delta;
            if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&scratch) >=
                distLimit) {
                continue;
            }
            func_8011E540(self, 0x16, apos, speed, 0, 0);
            break;
        }
        case 0xF: {
            u32 name = actor->name74;
            u8 count = self->mEntryCount;
            bool isNew = true;
            for (u8 k = 0; k < count; k++) {
                if (self->mEntries[k].unk04 == name) {
                    isNew = false;
                    break;
                }
            }
            if (!isNew) {
                continue;
            }
            self->mEntries[count].unk04 = name;
            void* speed = actorV->CfObject_UnkVirtualFunc55(100);
            Vec* apos = (Vec*)actorV->CfObject_UnkVirtualFunc23();
            void* player = cf::CfGameManager::getPlayer(0);
            if (player == 0) {
                continue;
            }
            Vec* ppos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
            Vec delta;
            Vec scratch;
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&delta,
                                (nw4r::math::VEC3*)ppos,
                                (nw4r::math::VEC3*)apos);
            scratch = delta;
            if (nw4r::math::VEC3LenSq((nw4r::math::VEC3*)&scratch) >=
                distLimit) {
                continue;
            }
            func_8011E540(self, 0x18, apos, speed, 0, 0);
            break;
        }
        default:
            break;
        }
    }
}

// ---------- CArrow3D destructor ----------
// Retail: if (self == 0) goto end; if (self != 0) { if (self != 0) {
// __dt__8CProcessFv(self, 0); } } if (flags > 0) __dl__FPv(self);
// end: return self; - the redundant `if (self != 0)` re-check reproduces the
// retail's dead double-beq (CMenuBattleChain/CMenuArtsSelect idiom).
#pragma push
#pragma auto_inline off
extern "C" void* __dt__8CArrow3DFv(CArrow3D* self, int flags) {
    if (self == 0) goto end;
    if (self != 0) {
        __dt__8CProcessFv(self, 0);
    }
    if (flags > 0) __dl__FPv(self);
end:
    return self;
}
#pragma pop

// ---------- CTTask<CArrow3D>::Move ----------
// MWCC lowers the PTMF dispatch to the retail __ptmf_test/__ptmf_scall calls
// (r3=this, r12=&ptmf) - same shape as CTaskGameEff/CMainMenu. (The old
// explicit __ptmf_test/__ptmf_scall calls passed the ptmf in r4 and never
// matched retail.) static_cast mirrors CTTask.hpp's inline body.
template<>
void CTTask<CArrow3D>::Move() {
    if (mMoveFunc) {
        (static_cast<CArrow3D*>(this)->*mMoveFunc)();
    }
}

// ---------- CTTask<CArrow3D>::Draw ----------
template<>
void CTTask<CArrow3D>::Draw() {
    if (mDrawFunc) {
        (static_cast<CArrow3D*>(this)->*mDrawFunc)();
    }
}

// ---------- CTTask<CArrow3D>::~CTTask ----------
// Referenced by the explicit base-dtor call in CArrow3D::~CArrow3D; emits the
// retail __dt__17CTTask<8CArrow3D>Fv body.
template<>
CTTask<CArrow3D>::~CTTask() {}

// ---------- CArrow3D::cbRenderBefore ----------
// Renders the target arrow: after a long gate chain (battle/menu state, alive
// players) it projects the arrow position relative to the first player,
// derives yaw/pitch/roll angles from the player-relative direction, builds a
// model matrix, then replays the resource display lists twice (first with
// color update off for the light pass, then normally).
// Render-gate imports.
extern "C" int func_8013C008();
extern "C" void updateViewRoot__9CViewRootFv();
extern "C" void resetGXStateA__8CGXCacheFv(void* cache);
extern "C" void func_8044BB20__8CGXCacheFv(void* cache, void* proj, f32 a,
                                           f32 b, f32 c);
// CGXCache singleton (static data member, symbol cacheInstance__9CDeviceGX).
extern void* cacheInstance__9CDeviceGX;
// Warning/Panic string blobs.
extern u8 lbl_eu_80526324[];
extern u8 lbl_eu_80526300[];
extern u8 lbl_eu_805262F0[];
extern u8 lbl_eu_805262C8[];
extern u8 lbl_eu_8052CF10[];
extern u8 lbl_eu_8052CEF4[];
extern u8 lbl_eu_8052CF6C[];
extern u8 lbl_eu_8052CF50[];
extern u8 lbl_eu_8052CF3C[];
extern u8 lbl_eu_8052CF20[];
// Panic vararg module-name globals (addresses only).
extern u32 lbl_eu_80661E98;
extern u32 lbl_eu_80661EA0;
extern u32 lbl_eu_80661EA8;
extern u32 lbl_eu_80661EB0;
extern u32 lbl_eu_80661EB4;
// Chan-mat color seed word + view matrix pair (wide/normal).
extern u32 lbl_eu_806670D0;
extern const f32 lbl_eu_804FE6D8[12];
// Current scene pointer (also used by Term/Init).
// Light color floats (rgb triple) used by both light and chan setup.
extern const f32 lbl_eu_8052CD38[8];
// Retail ml statics (data_defs.cpp).
// zero__Q22ml5CVec3 provided by CfObjectModel.hpp
extern u32 identity__Q22ml6CMat34[12];

#pragma push
#pragma auto_inline off
void CArrow3D::cbRenderBefore() {
    if (isFlag01Set__9CTaskGameFv(getInstance__9CTaskGameFv()) != 0) {
        return;
    }
    if (lbl_eu_80663E28 & 0x200000) {
        return;
    }
    if (lbl_eu_80663E24 & 0x400000) {
        return;
    }
    if ((u32)(u8)func_8009CF8C(0x20) <= 4) {
        return;
    }
    if (func_8013BE50() == 0) {
        return;
    }
    if (func_8013C008() == 0) {
        return;
    }
    if (lbl_eu_80663E24 & 0xAFA40200) {
        return;
    }
    if (getUnk80664658()->field_214 & 0x100000) {
        return;
    }
    // Battle active? Count nodes in the battle list at +8; any entry aborts.
    {
        void* bm = getInstance__Q22cf14CBattleManagerFv();
        u32 sentinel = ((u32*)bm)[2];
        u32 node = *(u32*)sentinel;
        u32 count = 0;
        while (node != sentinel) {
            node = *(u32*)node;
            count++;
        }
        if (count != 0) {
            return;
        }
    }
    // Proceed only while some player still reports hp > 0 (query through the
    // object reached at player - 0x3e9c, vtable slot 0x128).
    bool anyAlive = false;
    for (int p = 0; p < 3; p++) {
        void* pl = cf::CfGameManager::getPlayer(p);
        if (pl != 0) {
            cf::CActorParam* gate = (cf::CActorParam*)((char*)pl - 0x3e9c);
            if (gate->CActorParam_UnkVirtualFunc37() > 0.0f) {
                anyAlive = true;
                break;
            }
        }
    }
    if (!anyAlive) {
        return;
    }
    if (mFlag6C == 0) {
        goto restore;
    }
    {
        void* player = cf::CfGameManager::getPlayer(0);
        if (player == 0) {
            return;
        }
        ScnXformBlock* pose = (ScnXformBlock*)func_80496264(
            reinterpret_cast<CScn*>(lbl_eu_80663E14), -1);
        Vec* pos = (Vec*)( (cf::CfObject*)player)->CfObject_UnkVirtualFunc23();
        // Player-relative horizontal offset (y ignored by the length test).
        float dx = pos->x - mPosX;
        float dy = pos->y - mPosY;
        float dz = pos->z - mPosZ;
        float lenSq = dx * dx + dz * dz;
        if (lenSq == 0.0f) {
            nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                              (const char*)lbl_eu_80526300);
        }
        // Spherical angles toward the player (fidx units).
        float norm = lenSq;
        if (lenSq != 0.0f) {
            norm = lenSq * nw4r::math::FrSqrt(lenSq);
        }
        float yaw = lbl_eu_80667118 *
                    nw4r::math::Atan2FIdx(dy, norm); // elevation
        float azim = lbl_eu_80667118 * nw4r::math::Atan2FIdx(dx, -dz);
        // Wrap the azimuth into (-2pi, 2pi] around the scene heading.
        float roll = -azim - (lbl_eu_8066A1F8 + pose->proj120[1]);
        if (roll >= lbl_eu_8066A1F8) {
            roll -= lbl_eu_8066A1FC;
        }
        if (roll <= -lbl_eu_8066A1F8) {
            roll += lbl_eu_8066A1FC;
        }
        // Horizontal direction toward the player (normalized xz).
        nw4r::math::VEC3 hv;
        hv.x = dx;
        hv.y = 0.0f;
        hv.z = dz;
        float hLen = hv.x * hv.x + hv.z * hv.z;
        if (hLen == 0.0f) {
            hv = *reinterpret_cast<const nw4r::math::VEC3*>(&zero__Q22ml5CVec3);
        } else {
            PSVECNormalize((Vec*)&hv, (Vec*)&hv);
        }
        // Scene forward vector (xz-normalized after the transform).
        nw4r::math::VEC3 fv;
        fv.x = 0.0f;
        fv.y = 0.0f;
        fv.z = 1.0f;
        VEC3TransformNormal(&fv, (const nw4r::math::MTX34*)&pose->mtx9C[0],
                            &fv);
        fv.y = 0.0f;
        float fLen = fv.x * fv.x + fv.z * fv.z;
        if (fLen == 0.0f) {
            fv = *reinterpret_cast<const nw4r::math::VEC3*>(&zero__Q22ml5CVec3);
        } else {
            PSVECNormalize((Vec*)&fv, (Vec*)&fv);
        }
        float dot = fv.x * hv.x + fv.y * hv.y + fv.z * hv.z;
        if (dot < 0.0f) {
            yaw = -yaw;
        }
        yaw += pose->proj118[0];
        // Clamp the cosine into the acos domain.
        const float loBound = lbl_eu_80667104;
        if (dot < loBound) {
            dot = loBound;
        } else if (dot > lbl_eu_806670E8) {
            dot = lbl_eu_806670E8;
        }
        if (dot < loBound || dot > lbl_eu_806670E8) {
            nw4r::db::Warning((const char*)lbl_eu_805262F0, 0xef,
                              (const char*)lbl_eu_805262C8);
        }
        float pitch = (lbl_eu_806670E0 * lbl_eu_8066A210) *
                      nw4r::math::SinFIdx((f32)acos(dot) * lbl_eu_8066711C);
        if (roll < 0.0f) {
            pitch *= lbl_eu_80667104;
        }
        // Camera projection from the GX cache, then pick the view matrix.
        void* cache = cacheInstance__9CDeviceGX;
        resetGXStateA__8CGXCacheFv(cache);
        f32 proj44[16];
        func_8044BB20__8CGXCacheFv(cache, &proj44[0], lbl_eu_806670E8,
                                   lbl_eu_80667120, lbl_eu_80667124);
        GXSetProjection((const f32 (*)[4])&proj44[0], (GXProjectionType)0);
        const f32* viewBase = &lbl_eu_804FE6D8[0];
        const f32* view = CDeviceVI::isWideAspectRatio() ? viewBase : viewBase + 3;
        // Model matrix: pitch, then yaw, then roll concatenations.
        float m1[12];
        float sP = nw4r::math::SinFIdx(lbl_eu_8066711C * pitch);
        float cP = nw4r::math::CosFIdx(lbl_eu_8066711C * pitch);
        m1[0] = cP;
        m1[1] = -sP;
        m1[2] = 1.0f;
        m1[3] = 1.0f;
        m1[4] = sP;
        m1[5] = 1.0f;
        m1[6] = 1.0f;
        m1[7] = 1.0f;
        m1[8] = 1.0f;
        m1[9] = 1.0f;
        m1[10] = 1.0f;
        m1[11] = 1.0f;
        float sY = nw4r::math::SinFIdx(lbl_eu_8066711C * (-yaw));
        float cY = nw4r::math::CosFIdx(lbl_eu_8066711C * (-yaw));
        float m2[12];
        m2[0] = 1.0f;
        m2[1] = 0.0f;
        m2[2] = 0.0f;
        m2[3] = 0.0f;
        m2[4] = cY;
        m2[5] = -(-sY);
        m2[6] = 0.0f;
        m2[7] = 0.0f;
        m2[8] = sY;
        m2[9] = cY;
        m2[10] = 0.0f;
        m2[11] = 0.0f;
        PSMTXConcat((f32(*)[4])&m2[0], (f32(*)[4])&m1[0], (f32(*)[4])&m1[0]);
        float sR = nw4r::math::SinFIdx(lbl_eu_8066711C * roll);
        float cR = nw4r::math::CosFIdx(lbl_eu_8066711C * roll);
        float m3[12];
        m3[0] = cR;
        m3[1] = 0.0f;
        m3[2] = sR;
        m3[3] = 0.0f;
        m3[4] = 0.0f;
        m3[5] = 1.0f;
        m3[6] = 0.0f;
        m3[7] = 0.0f;
        m3[8] = -sR;
        m3[9] = 0.0f;
        m3[10] = cR;
        m3[11] = 0.0f;
        PSMTXConcat((f32(*)[4])&m3[0], (f32(*)[4])&m1[0], (f32(*)[4])&m1[0]);
        // Translate into camera space using the selected view columns.
        m1[11] += view[2];
        m1[7] += view[1];
        m1[3] += view[0];
        GXSetCurrentMtx(0);
        GXLoadPosMtxImm((const f32(*)[4])&m1[0], 0);
        GXLoadNrmMtxImm((const f32(*)[4])&identity__Q22ml6CMat34[0], 0);
        // Fixed vertex format: pos + uv + color (plus CI index when present).
        GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)9, (GXCompCnt)1, (GXCompType)4, 0);
        GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)0xa, (GXCompCnt)0, (GXCompType)4,
                        0);
        GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)0xb, (GXCompCnt)0, (GXCompType)5,
                        0);
        GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)0xd, (GXCompCnt)1, (GXCompType)4,
                        0);
        GXSetAlphaCompare((GXCompare)6, 1, (GXAlphaOp)0, (GXCompare)3, 0xff);
        GXSetBlendMode((GXBlendMode)1, (GXBlendFactor)4, (GXBlendFactor)5,
                       (GXLogicOp)0);
        GXSetZMode(GX_TRUE, (GXCompare)7, GX_TRUE);
        GXSetCullMode((GXCullMode)1);
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        GXSetNumChans(1);
        GXSetChanCtrl((GXChannelID)0, GX_TRUE, (GXColorSrc)0, (GXColorSrc)1,
                      (GXLightID)1, (GXDiffuseFn)2, (GXAttnFn)2);
        // Light color: scaled rgb triple + full alpha.
        {
            const f32* lc = &lbl_eu_8052CD38[0];
            float k = lbl_eu_80667128;
            GXLightObj lightBuf;
            u32 col;
            col = (u32)(s32)(k * lc[0]);           // r
            col |= (u32)(s32)(k * lc[1]) << 8;     // g
            col |= (u32)(s32)(k * lc[2]) << 16;    // b
            col |= (u32)(s32)(k * 1.0f) << 24;     // a
            GXInitLightColor(&lightBuf, *(GXColor*)&col);
            GXInitLightPos(&lightBuf, 0.0f, 0.0f, lbl_eu_8066712C);
            GXLoadLightObjImm(&lightBuf, (GXLightID)1);
        }
        ArrowResPtrView* rp = (ArrowResPtrView*)mDataPtr;
        u8* handle = mDataHandle;
        GXSetArray((GXAttr)9, handle + rp->arrPosOff, 0xC);
        GXSetArray((GXAttr)0xa, handle + rp->arrUvOff, 0xC);
        GXSetArray((GXAttr)0xb, handle + rp->arrColOff, 4);
        bool hasIdx = rp->arrIdxOff != 0;
        if (hasIdx) {
            GXSetArray((GXAttr)0xd, handle + rp->arrIdxOff, 8);
        }
        if (hasIdx) {
            GXSetTevOrder((GXTevStageID)0, (GXTexCoordID)0, (GXTexMapID)0,
                          (GXChannelID)0);
            GXSetTexCoordGen2((GXTexCoordID)0, (GXTexGenType)1,
                              (GXTexGenSrc)4, 0x3c, GX_FALSE, 0x7d);
            GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xf,
                            (GXTevColorArg)0xa, (GXTevColorArg)8,
                            (GXTevColorArg)0xf);
            GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)7,
                            (GXTevAlphaArg)5, (GXTevAlphaArg)4,
                            (GXTevAlphaArg)7);
        } else {
            GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xf,
                            (GXTevColorArg)0xf, (GXTevColorArg)0xf,
                            (GXTevColorArg)0xa);
            GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)7,
                            (GXTevAlphaArg)7, (GXTevAlphaArg)7,
                            (GXTevAlphaArg)5);
        }
        GXSetNumTevStages(1);
        GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0,
                        (GXTevScale)0, GX_TRUE, (GXTevRegID)0);
        GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0,
                        (GXTevScale)0, GX_TRUE, (GXTevRegID)0);
        GXClearVtxDesc();
        GXSetVtxDesc((GXAttr)9, (GXAttrType)3);
        GXSetVtxDesc((GXAttr)0xa, (GXAttrType)3);
        GXSetVtxDesc((GXAttr)0xb, (GXAttrType)3);
        if (hasIdx) {
            GXSetVtxDesc((GXAttr)0xd, (GXAttrType)3);
        }
        // Channel material color: seeded word with scaled rgb bytes patched in.
        {
            const f32* lc = &lbl_eu_8052CD38[0];
            float k = lbl_eu_80667128;
            u32 matCol = lbl_eu_806670D0;
            ((u8*)&matCol)[0] = (u8)(s32)(k * lc[3]);
            ((u8*)&matCol)[1] = (u8)(s32)(k * lc[4]);
            ((u8*)&matCol)[2] = (u8)(s32)(k * lc[5]);
            GXSetChanMatColor((GXChannelID)4, *(GXColor*)&matCol);
            u32 ambCol = matCol;
            GXSetChanAmbColor((GXChannelID)4, *(GXColor*)&ambCol);
        }
        GXSetColorUpdate(GX_FALSE);
        // Two passes over the display-list entries; the second re-enables
        // color update and picks a per-entry Z mode.
        for (u32 pass = 0; pass < 2; pass++) {
            if (pass != 0) {
                GXSetColorUpdate(GX_TRUE);
            }
            ArrowDLEntry* dl = (ArrowDLEntry*)(handle + rp->dlListOff);
            for (u32 e = 0; e < rp->dlCount; e++, dl++) {
                if (pass != 0) {
                    if (dl->flags & 1) {
                        GXSetZMode(GX_TRUE, (GXCompare)3, GX_FALSE);
                    } else {
                        GXSetZMode(GX_TRUE, (GXCompare)3, GX_TRUE);
                    }
                }
                nw4r::g3d::ResFile resFile(mAlignedData);
                nw4r::g3d::ResTex tex = resFile.GetResTex((int)dl->texIdx);
                if (tex.ptr() == 0) {
                    nw4r::db::Panic((const char*)lbl_eu_8052CF10, 0x26,
                                    (const char*)lbl_eu_8052CEF4,
                                    &lbl_eu_80661E98, &lbl_eu_80661EA0);
                }
                GXTexObj texObj;
                if (tex.ref().flag & 1) {
                    // Color-index texture: bind the palette as a TLUT first.
                    nw4r::g3d::ResFile parent = tex.GetParent();
                    nw4r::g3d::ResPltt pltt = parent.GetResPltt((int)dl->pltIdx);
                    void* img;
                    u16 w, h;
                    GXCITexFmt ciFmt;
                    f32 minLod, maxLod;
                    GXBool mipMap;
                    tex.GetTexObjCIParam(&img, &w, &h, &ciFmt, &minLod,
                                         &maxLod, &mipMap);
                    u32 tmem = 0;
                    if ((u32)ciFmt == 0xa) {
                        tmem = 0x10;
                    }
                    if (pltt.ptr() == 0) {
                        nw4r::db::Panic((const char*)lbl_eu_8052CF6C, 0x2a,
                                        (const char*)lbl_eu_8052CF50,
                                        &lbl_eu_80661EA8, &lbl_eu_80661EB4);
                    }
                    u16 entries = pltt.ref().numEntries;
                    if (pltt.ptr() == 0) {
                        nw4r::db::Panic((const char*)lbl_eu_8052CF6C, 0x2a,
                                        (const char*)lbl_eu_8052CF50,
                                        &lbl_eu_80661EA8, &lbl_eu_80661EB4);
                    }
                    u32 fmtWord = (u32)pltt.ref().fmt;
                    if (pltt.ptr() == 0) {
                        nw4r::db::Panic((const char*)lbl_eu_8052CF3C, 0x2a,
                                        (const char*)lbl_eu_8052CF20,
                                        &lbl_eu_80661EA8, &lbl_eu_80661EB0);
                    }
                    void* pltImg = pltt.ref().toPlttData != 0
                                       ? (void*)((u8*)&pltt.ref() +
                                                 pltt.ref().toPlttData)
                                       : 0;
                    GXTlutObj tlutObj;
                    GXInitTlutObj(&tlutObj, pltImg, (GXTlutFmt)fmtWord,
                                  entries);
                    GXLoadTlut(&tlutObj, tmem);
                    GXInitTexObjCI(&texObj, img, w, h, (GXTexFmt)ciFmt,
                                   (GXTexWrapMode)0, (GXTexWrapMode)0,
                                   (GXBool)mipMap, tmem);
                    if ((u8)mipMap != 0) {
                        GXInitTexObjLOD(&texObj, (GXTexFilter)5,
                                        (GXTexFilter)1, minLod, maxLod,
                                        lbl_eu_80667130, GX_TRUE, GX_FALSE,
                                        (GXAnisotropy)0);
                    } else {
                        GXInitTexObjLOD(&texObj, (GXTexFilter)1,
                                        (GXTexFilter)1, 0.0f, 0.0f, 0.0f,
                                        GX_FALSE, GX_FALSE, (GXAnisotropy)0);
                    }
                } else {
                    void* img;
                    u16 w, h;
                    GXTexFmt fmt;
                    f32 minLod, maxLod;
                    GXBool mipMap;
                    tex.GetTexObjParam(&img, &w, &h, &fmt, &minLod, &maxLod,
                                       &mipMap);
                    GXInitTexObj(&texObj, img, w, h, fmt, (GXTexWrapMode)0,
                                 (GXTexWrapMode)0, (GXBool)mipMap);
                    if ((u8)mipMap != 0) {
                        GXInitTexObjLOD(&texObj, (GXTexFilter)5,
                                        (GXTexFilter)1, minLod, maxLod,
                                        lbl_eu_80667130, GX_TRUE, GX_FALSE,
                                        (GXAnisotropy)0);
                    } else {
                        GXInitTexObjLOD(&texObj, (GXTexFilter)1,
                                        (GXTexFilter)1, 0.0f, 0.0f, 0.0f,
                                        GX_FALSE, GX_FALSE, (GXAnisotropy)0);
                    }
                }
                GXLoadTexObj(&texObj, (GXTexMapID)0);
                GXCallDisplayList(handle + dl->dlOff, dl->dlSize);
            }
        }
    }
restore:
    resetGXStateA__8CGXCacheFv(cacheInstance__9CDeviceGX);
    updateViewRoot__9CViewRootFv();
    if (mLayout != 0) {
        // Raw-storage DrawInfo (same shape as CMenuSymbolMark::cbRenderBefore).
        u8 drawInfo[0x54];
        __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
        func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
        drawLayout((nw4r::lyt::Layout*)mLayout,
                      (nw4r::lyt::DrawInfo*)&drawInfo[0], 0, 1);
        __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
    }
}
#pragma pop

// ---------- CArrow3D::Move (retail empty override, kept as extern "C" stub;
// the class deliberately declares no Move() so the vtable keeps the
// CTTask<CArrow3D>::Move dispatch - see CMenuSymbolMark.hpp) ----------
extern "C" void Move__8CArrow3DFv() {
}

// ---------- CArrow3D::Term ----------
// Wait for the draw to finish, release the layout (vtable+0x8 with flags=1),
// then remove the IScnRender callback at +0x54 from the current scene.
extern "C" void removeRenderCB__4CScnFP10IScnRender(void* self, void* cb);
void CArrow3D::Term() {
    CDeviceVI::waitForDrawDone();
    // delete through the Layout type: the retail dispatches vtable slot 2
    // with the deleting flag 1 (polymorphic delete), and the delete's own
    // null-check is the dead second beq (func_80285ABC pattern).
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)mLayout;
    if (layout) {
        delete layout;
        mLayout = 0;
    }
    // The `if (this)` + reassign idiom splits the IScnRender subobject
    // address (this+0x54) into its own addi; the call is unconditional
    // (CCol6System::Term pattern).
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    removeRenderCB__4CScnFP10IScnRender(lbl_eu_80663E14, render);
}

// ---------- CArrow3D::Init ----------
// Registers the arrow's IScnRender callback, loads the static layout data
// (asserting its 32-byte alignment), builds the layout and hides two panes by
// name.
void CArrow3D::Init() {
    const char* S = (const char*)&lbl_eu_804FE720[0];
    // The `if (this)` idiom splits the IScnRender subobject address
    // (this+0x54) into its own addi; the call itself is unconditional.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    reinterpret_cast<CScn*>(lbl_eu_80663E14)->addRenderCB(render, 9, 0);
    u8* handle = 0;
    CLibStaticData::getStaticFileData(S, (StaticDataHandle**)&handle, 0);
    mDataHandle = handle;
    mDataPtr = handle;
    mPosX = lbl_eu_806670D4;
    mPosY = lbl_eu_806670D4;
    mPosZ = lbl_eu_806670D4;
    mFlag6C = 0;
    StaticDataObj* sd = (StaticDataObj*)handle;
    if (sd->dataOffset != 0) {
        u8* aligned = handle + sd->dataOffset;
        if ((u32)aligned & 0x1F) {
            nw4r::db::Panic((const char*)lbl_eu_8052637C, 0x3c,
                            (const char*)lbl_eu_80526354, 0);
        }
        mAlignedData = aligned;
    }
    buildLayout((nw4r::lyt::Layout**)&mLayout, func_801355F4(), S);
    nw4r::lyt::Layout* lay = (nw4r::lyt::Layout*)mLayout;
    func_8013676C(lay->GetRootPane(), (u32)func_801355BC());
    func_80136B4C(lay, (char*)S + 0x14, (char*)S + 0x1b, 0);
    lay->GetRootPane()->FindPaneByName(S + 0x14, true)->SetVisible(false);
    lay->GetRootPane()->FindPaneByName(S + 0x26, true)->SetVisible(false);
}

// ---------- Thunk functions ----------
// These are vtable thunks for multiple inheritance.
// When called through the IWorkEvent vtable (offset 0x58) or IScnRender vtable (offset 0x5C),
// the this pointer needs adjustment before calling the actual implementation.
// Retail emits 8-byte tail calls (addi this-adjust + branch). The C++ member-scoped
// call form would inline the tiny bodies, so call the retail symbols directly as
// extern "C" functions (same pattern as the CMenuMapSelect/CMapSel dtors).
// Note: thunks call the dtors through a one-arg pointer view (matching the
// retail thunk ABI - only r3 is set before the tail branch; the flags arg is
// caller-leftover, as retail).
extern "C" void* __dt__15CMenuSymbolMarkFv(CMenuSymbolMark* self, int flags);
extern "C" void cbRenderBefore__15CMenuSymbolMarkFv(CMenuSymbolMark* self);
extern "C" void cbRenderBefore__8CArrow3DFv(CArrow3D* self);

extern "C" void func_8012213C(void* self) {
    // One-arg pointer view: retail thunk leaves r4 (delete flag) untouched.
    ((void (*)(CMenuSymbolMark*))__dt__15CMenuSymbolMarkFv)((CMenuSymbolMark*)((char*)self - 0x58));
}

extern "C" void func_80122144(void* self) {
    cbRenderBefore__15CMenuSymbolMarkFv((CMenuSymbolMark*)((char*)self - 0x5C));
}

extern "C" void func_8012214C(void* self) {
    ((void (*)(CMenuSymbolMark*))__dt__15CMenuSymbolMarkFv)((CMenuSymbolMark*)((char*)self - 0x5C));
}

extern "C" void func_80122154(void* self) {
    cbRenderBefore__8CArrow3DFv((CArrow3D*)((char*)self - 0x54));
}

extern "C" void func_8012215C(void* self) {
    ((void (*)(CArrow3D*))__dt__8CArrow3DFv)((CArrow3D*)((char*)self - 0x54));
}