// Decompiled: CMenuSymbolMark + CArrow3D
// High-level C++ reconstruction

#include "kyoshin/menu/CMenuSymbolMark.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>   // nw4r::lyt::DrawInfo
#include <nw4r/lyt/lyt_layout.h>    // nw4r::lyt::Layout
#include <nw4r/lyt/lyt_pane.h>      // nw4r::lyt::Pane (FindPaneByName/SetVisible)
#include <revolution/mtx/vec.h>     // Vec, PSVECMag
#include <monolib/util.hpp>         // ml::FixStr
#include <string.h>
#include <math.h>

// Retail helper imports (unmangled retail symbol names).
extern "C" u32 func_801380A0(u16);
extern "C" u8 func_8009CF8C(u32);
extern "C" u32 func_80138138(u16);
extern "C" u32 func_801361E8(u32, const char*, u32);
extern "C" int func_80138574(void*, u32);
// Naturally-mangled retail import (func_800B708C__Fi).
void* func_800B708C(int id);
extern u32 lbl_eu_804FE720[];
extern u32 lbl_eu_80573D18[];
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void* region);
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
extern "C" void* func_801355A0();
extern "C" void func_8013676C(nw4r::lyt::Pane*, u32);
// Naturally-mangled retail helpers (MWCC emits these exact symbols).
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136910(nw4r::lyt::Layout*, char*, u8);
extern "C" u32 getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void __ct__8CProcessFv(void* self);
extern "C" __declspec(noinline) void func_8011E778(
    CMenuSymbolMark* self, SymbolMarkEntry* entry, void* pos, void* argC,
    u32 argE);
extern const u32 __ptmf_null[3];
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
// lookup returns 1.
extern "C" int func_8011D338() {
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
extern "C" void* func_800426F0__9CTaskGameFv(void* self);
extern "C" int func_8013BE50();
struct CMenuGimmickGlobal { u8 mPad[0x214]; u32 field_214; };
extern "C" CMenuGimmickGlobal* getUnk80664658();
extern u32 lbl_eu_80663E28;
extern u32 lbl_eu_80663E24;
// C++-linkage helpers - retail emits the mangled forms
// func_80137250__FPQ34nw4r3lyt8DrawInfo / func_80137038__FPQ34nw4r3lyt6Layout...
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
void func_80137038(nw4r::lyt::Layout* layout, nw4r::lyt::DrawInfo* drawInfo,
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
      mIWorkEventVt((void*)&lbl_eu_8052CDF8[0]),
      mIScnRenderVt((void*)&lbl_eu_8052CDF8[9]), // +0x24
      mScn(scn) {
    ((ProcPrefixView*)this)->vt10 = &lbl_eu_8052BF70;
    ptmfMove[0] = __ptmf_null[0];
    ptmfMove[1] = __ptmf_null[1];
    ptmfMove[2] = __ptmf_null[2];
    ptmfDraw[0] = __ptmf_null[0];
    ptmfDraw[1] = __ptmf_null[1];
    ptmfDraw[2] = __ptmf_null[2];
    const char* S = (const char*)&lbl_eu_804FE720[0];
    // Clear every entry record; the un-initialized world floats copy as-is.
    SymbolMarkEntry clearEntry;
    clearEntry.unk00 = 0;
    clearEntry.unk04 = 0;
    clearEntry.layout = 0;
    clearEntry.flag0 = 0;
    clearEntry.flag1 = 0;
    clearEntry.flag2 = 0;
    clearEntry.unk1C = 0;
    for (int i = 0; i < 16; i++) {
        mEntries[i] = clearEntry;
    }
    func_8003AA34();
    mArchiveFP = (u8*)getFP(S + 0x39);
    func_8003AA34();
    mSomeFP = (u8*)getFP(S + 0x46);
    mSomeValue2 = func_8003B1EC(mArchiveFP);
    mSomeValue3 = func_801380A0((u16)((u32)scn + 1));
    mSomeValue = lbl_eu_80573D18[idx];
    mSomeValue4 = func_8003B1EC((u8*)mSomeValue) + mSomeValue3;
    memset(mBuffer, 0, 0x200);
    // Zero both id arrays in lockstep 16-word rows.
    for (u32 i = 0; i < 8; i++) {
        for (u32 j = 0; j < 0x10; j++) {
            mArray4A0[i * 0x10 + j] = 0;
            mArray6A8[i * 0x10 + j] = 0;
        }
    }
    mField_49C = lbl_eu_806670D4;
    mField_6A4 = lbl_eu_806670D8;
    mField_8AC = lbl_eu_806670DC;
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
        func_80136E84((nw4r::lyt::Layout**)&mEntries[i].layout, func_801355F4(),
                      (const char*)&lbl_eu_804FE720[0x63]);
        // Re-read the layout pointer at every use so nothing spans the virtual
        // calls (retail reloads the slot from the entry base each time).
        ((nw4r::lyt::Layout*)mEntries[i].layout)->Animate();
        func_8013676C(((nw4r::lyt::Layout*)mEntries[i].layout)->GetRootPane(),
                      (u32)func_801355A0());
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
        u32* ptmf = (u32*)__ptmf_null;
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
    func_8045F778__17UnkClass_8045F564Fv(&mUnkClass);
    lbl_eu_80663FC8 = 0;
}

// Later-file walkers used by Move.
extern "C" void func_8011EBA8(CMenuSymbolMark* self);
extern "C" void func_8011EC94(CMenuSymbolMark* self);
extern "C" void func_8011EDDC(CMenuSymbolMark* self);
extern "C" void func_8011EFB0(CMenuSymbolMark* self);

// ---------- CMenuSymbolMark::Move ----------
// Releases every entry record and hides its pane via FindPaneByName. Macro so
// both call sites expand inline like the retail copies.
#define HIDE_ALL_ENTRY_PANES(sBase)                                        \
    for (int i = 0; i < 16; i++) {                                         \
        SymbolMarkEntry& clr = mEntries[i];                                \
        clr.unk04 = 0;                                                     \
        clr.flag0 = 0;                                                     \
        clr.flag1 = 0;                                                     \
        clr.unk1C = 0;                                                     \
        LytPaneFindView* pane =                                            \
            (LytPaneFindView*)((LytLayoutFields*)clr.layout)->rootPane;    \
        if (pane != 0) {                                                   \
            LytPaneFindView* found = pane->findPane((sBase) + 0x14, true); \
            if (found != 0) {                                              \
                ((LytPaneFields*)found)->flagBB &= (u8)~1;                 \
            }                                                              \
        }                                                                  \
    }

extern "C" void func_8011EA98(CMenuSymbolMark* self);
extern "C" void func_801209BC(CMenuSymbolMark* self);

// Shared tail of func_801209BC's kind dispatch (also used for kind 0x10 in
// Move): claim an entry slot for the marker and re-add it when the player
// stands within range. Kept as a macro so each call site expands inline like
// the retail copies.
#define CLAIM_AND_ADD_NEARBY(kindId)                                       \
    do {                                                                   \
        bool dup = false;                                                  \
        for (u32 k = 0; k < self->mEntryCount; k++) {                      \
            if (self->mEntries[k].unk04 == actor->name74) {                \
                dup = true;                                                \
                break;                                                     \
            }                                                              \
        }                                                                  \
        if (!dup) {                                                        \
            self->mEntries[self->mEntryCount].unk04 = actor->name74;       \
            void* speed = actorV->_v12C(100);                              \
            Vec* mpos = (Vec*)actorV->_v0AC();                             \
            void* player = cf::CfGameManager::getPlayer(0);                \
            if (player != 0) {                                             \
                Vec* ppos = (Vec*)((CfActorView*)player)->_v0AC();         \
                f32 dx = ppos->x - mpos->x;                                \
                f32 dy = ppos->y - mpos->y;                                \
                f32 dz = ppos->z - mpos->z;                                \
                if (dx * dx + dy * dy + dz * dz < distLimit) {             \
                    func_8011E540(self, (kindId), mpos, speed, 0, 0);      \
                }                                                          \
            }                                                              \
        }                                                                  \
    } while (0)

extern "C" void func_8011E540(CMenuSymbolMark* self, u32 id, Vec* pos,
                              void* argC, u32 argD, u32 argE);

// Per-frame update. When the syswin close flag (0xAFA40000) is set the mark
// display expires: once the timer passes the threshold every entry pane is
// hidden again and the id arrays reset. Otherwise the three id-list refresh
// timers tick; when the main timer passes the threshold all entries are
// released and nearby markers (func_8009CF8C gate) re-add themselves.
void CMenuSymbolMark::Move() {
    if (!(lbl_eu_80663E28 & 0x200000)) {
        return;
    }
    if (mArchiveFP == 0) {
        return;
    }
    if (!func_8013BE50()) {
        return;
    }
    const char* S = (const char*)&lbl_eu_804FE720[0];
    if (lbl_eu_80663E24 & 0xAFA40000) {
        if (mTimer >= lbl_eu_806670E0) {
            mTimer = lbl_eu_806670E4;
            mEntryCount = 0;
            HIDE_ALL_ENTRY_PANES(S);
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
        HIDE_ALL_ENTRY_PANES(S);
        func_8011EFB0(this);
        if (func_8009CF8C(0x3354) == 0) {
            const f32 distLimit = lbl_eu_806670EC;
            CMenuSymbolMark* self = this;
            for (u32 j = 0; j < mField_8A8; j++) {
                CfActorView* actorV = (CfActorView*)func_800B708C(mArray6A8[j]);
                if (actorV == 0 || actorV->_v160() == 0) {
                    continue;
                }
                CfActorFields* actor = (CfActorFields*)actorV;
                CLAIM_AND_ADD_NEARBY(0x10);
            }
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
    if (func_800426F0__9CTaskGameFv(getInstance__9CTaskGameFv()) == 0) {
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
            func_80137038((nw4r::lyt::Layout*)entry.layout,
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
    SymbolMarkEntry& entry = self->mEntries[self->mEntryCount];
    const char* S = (const char*)&lbl_eu_804FE720[0];
    char* SN = (char*)&lbl_eu_804FE720[0];
    if (id == 7) {
        void* player = cf::CfGameManager::getPlayer(0);
        if (player != 0) {
            Vec* ppos = (Vec*)((CfActorView*)player)->_v0AC();
            Vec diff;
            diff.x = pos->x - ppos->x;
            diff.y = pos->y - ppos->y;
            diff.z = pos->z - ppos->z;
            u8 frames = (u8)(int)PSVECMag(&diff);
            func_80136910((nw4r::lyt::Layout*)entry.layout, SN + 0x14, frames);
            SymbolMarkRenderItem* item = self->mRenderItem;
            if (item == 0) {
                return;
            }
            if (item->state70 == 0x1a) {
                func_80136B4C((nw4r::lyt::Layout*)entry.layout, SN + 0x14,
                              SN + 0x1b, 0);
                return;
            }
            func_80136910((nw4r::lyt::Layout*)entry.layout, SN + 0x14, frames);
            return;
        }
    }
    if (id == 5 || id == 9 || id == 0xa || id == 0xc || id == 0xf ||
        id == 0x10 || (id - 0x15) <= 3) {
        entry.flag2 = 1;
    }
    func_8011E778(self, &entry, pos, argC, argE);
    ml::FixStr<32> name;
    name.format(S + 0x86,
                func_8013639C(self->mAnotherFP, S + 0x7b, id));
    void* arc = func_801355F4();
    ArcAccessorView* acc = (ArcAccessorView*)arc;
    if (acc->find("timg", (const char*)name.c_str(), 0) != 0) {
        func_80137E7C((nw4r::lyt::Layout*)entry.layout, S + 0x8d);
    }
    entry.unk00 = id;
    entry.flag0 = 1;
    entry.flag1 = (u8)argD;
    entry.worldX = pos->x;
    entry.worldY = pos->y;
    entry.worldZ = pos->z;
    entry.unk1C = (u32)argC;
    self->mEntryCount++;
}

// ---------- func_8011E778 ----------
// Project the entry anchor into screen space, toggle the root pane's visibility
// flag depending on whether the point left the screen range, then either place
// and scale the pane from the measured edge distance (flag2 set) or run the
// bounded vertical slide (flag2 clear).
extern "C" __declspec(noinline) void func_8011E778(
    CMenuSymbolMark* self, SymbolMarkEntry* entry, void* posObj, void* c,
    u32 d) {
    nw4r::math::VEC3 worldPos;
    if (posObj != 0) {
        EntryInputPos* src = (EntryInputPos*)posObj;
        worldPos.x = src->posX;
        worldPos.y = src->posY;
        worldPos.z = src->posZ;
    }
    ScnXformBlock* pose = func_80496264(self->mScn, -1);
    if (posObj == 0 && d != 0) {
        worldPos.y += lbl_eu_806670F0;
    } else if (posObj == 0) {
        worldPos.y += lbl_eu_806670E8;
    }
    nw4r::math::VEC3 screen;
    func_8049B59C(&screen, pose, &worldPos);
    LytPaneFields* pane = ((LytLayoutFields*)entry->layout)->rootPane;
    // Hidden when both projected coordinates exceed the bound.
    bool offscreen = screen.x > lbl_eu_806670D4 && screen.y > lbl_eu_806670D4;
    if (offscreen) {
        pane->flagBB |= 1;
    } else {
        pane->flagBB &= (u8)~1;
    }
    if (entry->flag2 != 0) {
        // Distance-based placement: transform a unit offset by the pose matrix,
        // project the far edge and derive an inverse-distance scale.
        nw4r::math::VEC3 dir;
        dir.x = lbl_eu_806670E8;
        dir.y = lbl_eu_806670E8;
        dir.z = lbl_eu_806670D4;
        nw4r::math::VEC3 nrm;
        nw4r::math::VEC3TransformNormal(&nrm,
                                        (const nw4r::math::MTX34*)&pose->mtx9C,
                                        &dir);
        nw4r::math::VEC3 edge;
        edge.x = worldPos.x + nrm.x;
        edge.y = worldPos.y + nrm.y;
        edge.z = worldPos.z + nrm.z;
        nw4r::math::VEC3 edgeScreen;
        func_8049B59C(&edgeScreen, pose, &edge);
        f32 invX = lbl_eu_806670E8 / __fabs(edgeScreen.x - screen.x);
        f32 invY = lbl_eu_806670E8 / __fabs(edgeScreen.y - screen.y);
        if (!CDeviceVI::isWideAspectRatio()) {
            invX *= lbl_eu_806670F4;
        }
        f32 baseScale = lbl_eu_806670F8 / lbl_eu_8066A1F8;
        pane->trX = worldPos.x;
        pane->trY = worldPos.y;
        pane->trZ = worldPos.z;
        pane->rotX = pose->proj118[0] * baseScale;
        pane->rotY = pose->proj118[1] * baseScale;
        pane->rotZ = pose->proj120[0] * baseScale;
        pane->scX = invX;
        pane->scY = invY;
    } else {
        // Vertical slide clamped into a band around the screen edge.
        f32 scaledY = (screen.y - lbl_eu_80667100) * lbl_eu_80667104;
        screen.x = screen.x - lbl_eu_806670FC;
        screen.y = scaledY;
        f32 z = lbl_eu_806670D4;
        f32 limit = lbl_eu_80667108 + scaledY;
        if (limit > lbl_eu_80667100) {
            screen.y = scaledY - (limit - lbl_eu_80667100);
        } else {
            f32 g = scaledY - lbl_eu_80667108;
            if (g < lbl_eu_8066710C) {
                screen.y = scaledY + (lbl_eu_8066710C - g);
            }
        }
        pane->trX = screen.x;
        pane->trY = screen.y;
        pane->trZ = z;
    }
}

// ---------- func_8011EA98 ----------
// Per-entry update: resolves the actor bound to the entry (func_800B708C on
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
            CfActorView* actor = (CfActorView*)func_800B708C(entry->unk04);
            if (actor == 0) {
                entry->flag0 = zero;
                continue;
            }
            // unk00 == 9 or 10: gate on the actor's live flag (vtable 0x160).
            if ((entry->unk00 - 9) <= 1 && actor->_v160() == 0) {
                entry->flag0 = zero;
                continue;
            }
            value12c = actor->_v12C(100);
            Vec* valueAC = (Vec*)actor->_v0AC();
            func_8011E778(self, entry, valueAC, value12c, zero);
        } else {
            func_8011E778(self, entry, &entry->worldX,
                          (void*)entry->unk1C, zero);
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
        pos = (Vec*)((CfActorView*)obj)->_v0AC();
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
        u32 next = n + 1;
        u32 val = obj->name74;
        self->mArray6A8[n] = val;
        self->mField_8A8 = next;
        if (next >= 0x80) {
            return;
        }
    }
}

// ---------- func_8011EFB0 ----------
extern "C" void func_8011EFB0(CMenuSymbolMark* self) {
}

// ---------- func_8011F8F8 ----------
// Walks the unlocked-id list (mArray4A0): for each live marker whose measured
// value matches a table row inside the scene bounds box, claim an entry slot
// and add kind-0xa marks when the player stands within range.
extern "C" void func_8011F8F8(CMenuSymbolMark* self) {
    const f32 distLimit = lbl_eu_806670EC;
    const char* S = (const char*)&lbl_eu_804FE720[0];
    for (u32 i = 0; i < self->mField_6A0; i++) {
        CfActorView* actorV = (CfActorView*)func_800B708C(self->mArray4A0[i]);
        if (actorV == 0 || actorV->_v160() == 0) {
            continue;
        }
        CfActorFields* actor = (CfActorFields*)actorV;
        Vec anchor;
        Vec extent;
        for (u32 j = 0; j < self->mSomeValue4; j++) {
            u16 id = (u16)func_80136254((const void*)self->mSomeValue, S + 0xa6,
                                        j);
            if (id != actor->field8C) {
                continue;
            }
            if (func_80138234((const void*)self->mSomeValue, j) == 0) {
                continue;
            }
            Vec* pos = (Vec*)actorV->_v0AC();
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
            bool dup = false;
            for (u32 k = 0; k < self->mEntryCount; k++) {
                if (self->mEntries[k].unk04 == actor->name74) {
                    dup = true;
                    break;
                }
            }
            if (dup) {
                continue;
            }
            self->mEntries[self->mEntryCount].unk04 = actor->name74;
            void* speed = actorV->_v12C(100);
            Vec* pos2 = (Vec*)actorV->_v0AC();
            void* player = cf::CfGameManager::getPlayer(0);
            if (player == 0) {
                continue;
            }
            Vec* ppos = (Vec*)((CfActorView*)player)->_v0AC();
            f32 dx = ppos->x - pos2->x;
            f32 dy = ppos->y - pos2->y;
            f32 dz = ppos->z - pos2->z;
            if (dx * dx + dy * dy + dz * dz >= distLimit) {
                continue;
            }
            func_8011E540(self, 0xa, pos2, speed, 1, 0);
        }
    }
}

// ---------- func_8011FB68 ----------
void func_8011FB68(CMenuSymbolMark* self) {
}

// ---------- func_801209BC ----------
// Companion walker over the second id list (mArray6A8): dispatches on each
// actor's kind byte and re-adds the matching symbol-mark kind when the player
// is close enough.
extern "C" void func_801209BC(CMenuSymbolMark* self) {
    const f32 distLimit = lbl_eu_806670EC;
    for (u32 i = 0; i < self->mField_8A8; i++) {
        CfActorView* actorV = (CfActorView*)func_800B708C(self->mArray6A8[i]);
        if (actorV == 0 || actorV->_v160() == 0) {
            continue;
        }
        CfActorFields* actor = (CfActorFields*)actorV;
        switch (actor->kind91) {
        case 0xE:
            CLAIM_AND_ADD_NEARBY(0x17);
            break;
        case 0xA:
            CLAIM_AND_ADD_NEARBY(0x15);
            break;
        case 0xB:
            CLAIM_AND_ADD_NEARBY(0x16);
            break;
        case 0xF:
            CLAIM_AND_ADD_NEARBY(0x18);
            break;
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
#pragma push
#pragma auto_inline off
void CArrow3D::cbRenderBefore() {
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
extern u8* lbl_eu_80663E14;
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
    func_80136E84((nw4r::lyt::Layout**)&mLayout, func_801355F4(), S);
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
// Note: the dtors are declared 1-arg here (matching the retail thunk ABI - only
// r3 is set before the tail branch; the flags arg is caller-leftover, as retail).
extern "C" void* __dt__15CMenuSymbolMarkFv(CMenuSymbolMark* self, int flags);
extern "C" void cbRenderBefore__15CMenuSymbolMarkFv(CMenuSymbolMark* self);
extern "C" void cbRenderBefore__8CArrow3DFv(CArrow3D* self);

extern "C" void func_8012213C(void* self) {
    __dt__15CMenuSymbolMarkFv((CMenuSymbolMark*)((char*)self - 0x58), 0);
}

extern "C" void func_80122144(void* self) {
    cbRenderBefore__15CMenuSymbolMarkFv((CMenuSymbolMark*)((char*)self - 0x5C));
}

extern "C" void func_8012214C(void* self) {
    __dt__15CMenuSymbolMarkFv((CMenuSymbolMark*)((char*)self - 0x5C), 0);
}

extern "C" void func_80122154(void* self) {
    cbRenderBefore__8CArrow3DFv((CArrow3D*)((char*)self - 0x54));
}

extern "C" void func_8012215C(void* self) {
    __dt__8CArrow3DFv((CArrow3D*)((char*)self - 0x54), 0);
}