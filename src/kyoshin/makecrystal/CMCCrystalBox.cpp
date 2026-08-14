// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/makecrystal/CMCCrystalBox.hpp"

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"

#include <nw4r/lyt.h>

// Retail symbols: func_80137444 keeps the mangled C++ name, but func_80137510
// is the unmangled C name (per retail relocs).
u32 func_80137444(nw4r::lyt::AnimTransform*, float);

void func_80138078(u32);

void func_80136910(nw4r::lyt::Layout*, char*, u8);

// Layout-anim helpers: retail emits out-of-line `bl` calls to these from the
// anim-completion callbacks; MWCC -O4,p would otherwise inline them into the
// small callers. C linkage carries over from the header declarations.
__declspec(noinline) void func_80219094(CMCCrystalBox* self);
__declspec(noinline) void func_8021900C(CMCCrystalBox* self);
__declspec(noinline) void func_8021922C(CMCCrystalBox* self);
__declspec(noinline) void func_802191A4(CMCCrystalBox* self);
__declspec(noinline) void func_8021911C(CMCCrystalBox* self);

// Refresh/name-label helpers: retail calls these out-of-line from the update
// callbacks (func_80216614/98, func_80215408); keep them non-inlined so the
// callers emit `bl` like retail instead of cascading virtual-call bodies.
__declspec(noinline) void func_80218018(CMCCrystalBox* self);
__declspec(noinline) void func_80218460(CMCCrystalBox* self);
__declspec(noinline) void func_8021899C(CMCCrystalBox* self);
__declspec(noinline) char* func_802138B8(CMCCrystalData* d, int v);

// Crystal-box param helpers: retail calls these out-of-line (bl) from the
// init/refresh loops; keep them non-inlined so the callers emit bl like
// retail instead of cascading the store/copy bodies into the loops.
__declspec(noinline) CMCCrystalBoxParam* func_80213E04(CMCCrystalBoxParam*, s16, s16);
__declspec(noinline) void func_80213E20(CMCCrystalBoxParam*, const CMCCrystalBoxParam*);

// Retail 0x80219E70: refresh the crystal box display / name.
// optimize_for_size: retail uses the stmw r30/lmw r30 frame; plain -O4,p
// splits the saves into separate stw's (MWCC_REFERENCE kyoshin-dtor pattern).
#pragma optimize_for_size on
void func_80218018(CMCCrystalBox* self) {
    CMCCrystalData* d = &self->data;
    if (self->field_14A0 == 0) {
        func_80213570(d, self->field_14EC[(s8)self->field_14F2]);
    }
    func_80136910((nw4r::lyt::Layout*)self->subObjPtrs[5],
                  lbl_eu_8050888C + 0x12e, d->limit != 0 ? d->limit : 1);
    func_802180B4(self);
    func_80218460(self);
    func_8021899C(self);
    func_8021A8EC((u8*)self + 0x1ec);
}
#pragma optimize_for_size off

// Retail 0x8021A2B8: set the current crystal's name label and mirrored info.
// stmw r28 frame + raw-byte idx multiply are the -O4,s shapes (pragma).
#pragma optimize_for_size on
void func_80218460(CMCCrystalBox* self) {
    // Declared first so MWCC claims r31 for q (first-declared -> highest
    // saved register), matching retail's {d:r29, idx:r30, q:r31}.
    void* q;
    u16 p;
    u8 idx = self->unk2CC + (u8)self->unk2CD * 10;
    CMCCrystalData* d = &self->data;
    char* name = func_802138B8(d, idx);
    func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5],
                  lbl_eu_8050888C + 0x18e, name, 0);
    if (func_8021A8EC(&self->subObjPtrs[0x72]) != 0) {
        if ((s8)self->unk2CD < 3) {
            // Retail evaluates func_8021384C before func_802137DC and keeps
            // the object pointer result in a saved register across the second
            // call.
            q = func_8021384C(d, idx);
            p = func_802137DC(d, idx);
            func_8021A9A8(&self->subObjPtrs[0x72], p, q);
        } else {
            func_8021A9A8(&self->subObjPtrs[0x72], 0, 0);
        }
    }
}
#pragma optimize_for_size off

// Retail 0x80215710: resolve a crystal item by adjusted index, get its name
// from a freshly-created item instance, and format it into the name buffer.
// Single trailing `return 0` (nested ifs) so MWCC merges all failure paths
// into one epilogue `li r3, 0` like retail.
#pragma optimize_for_size on
char* func_802138B8(CMCCrystalData* d, int v) {
    int cur = d->current;
    u16 adj = (u16)(v + cur * 30);          // row-scaled index
    if ((u32)adj < d->count) {
        void* obj = func_80157C4C(d->byte_1002, d->entries[adj].id);
        if (obj != 0 && *(void**)obj != 0) {
            void* inst = CItem_initItemImplInstances(obj);
            char* name = ((CItemImplDispatch*)inst)->getName(obj);
            sprintf(d->name, lbl_eu_8050888C, name);
            return d->name;
        }
    }
    return 0;
}
#pragma optimize_for_size off

// Swap the 4-byte payload (s16 + u8) of two crystal-data entries.
// noinline: retail calls this out-of-line (bl) from the bubble sort; without
// it MWCC inlines the body and the func_80213964 reloc disappears.
__declspec(noinline) void func_80213964(int unused, void* a, void* b) {
    short b_lo;
    unsigned char b_hi;
    unsigned char a_hi;
    short a_lo;
    a_lo = *(short*)a;
    a_hi = ((unsigned char*)a)[2];
    b_hi = ((unsigned char*)b)[2];
    b_lo = *(short*)b;
    *(short*)a = b_lo;
    ((unsigned char*)a)[2] = b_hi;
    *(short*)b = a_lo;
    ((unsigned char*)b)[2] = a_hi;
}

void func_80213988(){}

// Retail 0x80215974: bubble-sort the crystal-state entries by their %+4
// key using func_80157C4C until no swap occurs in a pass.
void func_80213B1C(CMCCrystalData* d) {
    u16 o = 0;
    while (o < d->count - 1) {
        int swapped = 0;
        u16 i = 0;
        while (i < (d->count - 1) - o) {
            CMCCrystalDataEntry* pa = &d->entries[i];
            CMCCrystalDataEntry* pb = &d->entries[i + 1];
            // Retail keeps obj1 in a saved register across the second call
            // and loads both %+4 keys only after both resolves.
            void* obj1 = func_80157C4C(9, pa->id);
            void* obj2 = func_80157C4C(9, pb->id);
            u32 av = *(u16*)((u8*)obj1 + 4);
            u32 bv = *(u16*)((u8*)obj2 + 4);
            if (av > bv) {
                func_80213964((int)d, pa, pb);
                swapped = 1;
            }
            i++;
        }
        if (swapped == 0) break;
        o++;
    }
}

void __ct__CMCCrystalBox(){}

// Retail 0x80215C5C: set all fields of a crystal-box param (selected id,
// quantity, spare, two flags) and return the param pointer.
CMCCrystalBoxParam* func_80213E04(CMCCrystalBoxParam* p, s16 a, s16 b) {
    p->m0 = a;
    p->m2 = b;
    p->m4 = 0;
    p->m6 = 0;
    p->m7 = 0;
    return p;
}

// Retail 0x80215C78: copy an 8-byte crystal-box param (three s16, two u8).
void func_80213E20(CMCCrystalBoxParam *dst, const CMCCrystalBoxParam *src) {
    unsigned short *d16 = (unsigned short*)dst;
    const unsigned short *s16 = (const unsigned short*)src;
    d16[0] = s16[0];
    d16[1] = s16[1];
    d16[2] = s16[2];
    unsigned char *d8 = (unsigned char*)dst;
    const unsigned char *s8 = (const unsigned char*)src;
    d8[6] = s8[6];
    d8[7] = s8[7];
}

// Retail 0x80215BCC: initialise the 32-slot param table at base+2 (pointer
// walk, do-while), then re-init every slot through a stack-temp copy so each
// entry is written by func_80213E20 (matching retail's two-loop shape).
// noinline: retail calls this out-of-line (bl) from func_8021625C; without
// it MWCC inlines the two-loop body into the caller.
__declspec(noinline) u8* func_80213D74(u8* base) {
    // end declared first so it lands in r31, p in r30, base in r29 (retail
    // register order). The -O4,p prologue keeps this shape; retail's grouped
    // stmw r29 needs #pragma optimize_for_size (out of scope here).
    CMCCrystalBoxParam* end = (CMCCrystalBoxParam*)(base + 0x102);
    CMCCrystalBoxParam* p = (CMCCrystalBoxParam*)(base + 2);
    do {
        func_80213E04(p, 0, 0);
        p++;
    } while (p < end);

    base[0] = 0;
    base[1] = 0;

    u8 i = 0;
    do {
        CMCCrystalBoxParam tmp;
        func_80213E20(&((CMCCrystalBoxParam*)(base + 2))[i],
                      func_80213E04(&tmp, 0, 0));
        i++;
    } while (i < 0x20);

    return base;
}

void* __dt__80213E4C(void* self, int flags) {
    if (self != 0 && flags > 0) {
        operator delete(self);
    }
    return self;
}

void func_80213E8C(CMCCrystalBox* self) {
    u8 i;
    self->unk20 = 0;
    ((u8*)self)[0x29] = 0;
    ((u8*)self)[0x6A] = 0;
    // Retail keeps this as a rolled mtctr/bdnz count-down loop with a u8
    // up-counter (clrlslwi/clrlwi masks). That shape is -O4,s codegen
    // (indexed stwx, no unroll); this unit compiles -O4,p which unrolls the
    // constant-trip loop instead (walls doc #6: needs a unit flag split).
    i = 0;
    do {
        self->subObjPtrs[i] = 0;
        ((u8*)self)[0x21 + i] = 0xFF;
        i++;
    } while (i < 8);
}

void* __dt__80213ECC(void* self, int flags) {
    if (self != 0 && flags > 0) {
        operator delete(self);
    }
    return self;
}

// Retail 0x80215D64: destroy all placed member subobjects in reverse order.
// stmw r30 frame is the -O4,s shape (pragma).
#pragma optimize_for_size on
CMCCrystalBox::~CMCCrystalBox() {
    __dt__7CSysWinFv((u8*)this + 0x290, -1);
    __dt__10CScrollBarFv((u8*)this + 0x250, -1);
    __dt__14CMCCrystalInfoFv((u8*)this + 0x1ec, -1);
    __dt__9CSortMenuFv((u8*)this + 0xfc, -1);
    __dt__6CCur18Fv((u8*)this + 0xe4, -1);
    __dt__6CCur16Fv((u8*)this + 0xcc, -1);
    __dt__6CCur11Fv((u8*)this + 0xb4, -1);
    __dt__6CCur18Fv((u8*)this + 0x9c, -1);
    __dt__6CCur09Fv((u8*)this + 0x84, -1);
    __dt__6CCur07Fv((u8*)this + 0x6c, -1);
    __dt__17UnkClass_8045F564Fv((u8*)this + 0x14, -1);
    __dt__17UnkClass_8045F564Fv((u8*)this + 0x4, -1);
}
#pragma optimize_for_size off

// Retail 0x80215E3C: crystal-box constructor-style init. Reads the three
// resource archives (sort menu, crystal info, common) through the device file
// handles, then re-initialises every embedded sub-object from a stack temp:
// the temp is constructed, its body copied member-wise (skipping the vtable),
// and the temp destroyed. Finishes by reading the sub-object layout archives
// and clearing the shared BDAT cursor. stmw r30 frame is the -O4,s shape.
#pragma optimize_for_size on
void func_80213FE4(CMCCrystalBox* self) {
    // Resource archives: sort menu (+0x24), crystal info (+0x28), common
    // (+0x2C). The first two use the heap handle, the third the common-archive
    // file after func_800A9D90's handle setup.
    self->subObjPtrs[0] = (nw4r::lyt::AnimTransform*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)getHandleMEM1__Q23mtl10MemManagerFv(), lbl_eu_8050888C + 0xc, self, 0, 0);
    self->subObjPtrs[1] = (nw4r::lyt::AnimTransform*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)getHandleMEM2__Q23mtl10MemManagerFv(), lbl_eu_8050888C + 0x25, self, 0, 0);
    self->subObjPtrs[2] = (nw4r::lyt::AnimTransform*)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)func_800A9D90(), lbl_eu_8050888C + 0x3e, self, 0, 0);

    // Re-init the sort menu from a default temp: copy every member except the
    // +0x6C s32 array (retail copies the mem region, pointer fields, flags,
    // scrollbar and the three tail bytes).
    CSortMenu tmp;
    __ct__CSortMenu(&tmp);
    __ct__UnkClass_8011C974(&self->sortMenu.mUnk04[0], &tmp.mUnk04[0]);
    self->sortMenu.mFileHandle = tmp.mFileHandle;
    self->sortMenu.mArcResAcc = tmp.mArcResAcc;
    self->sortMenu.mpLayout = tmp.mpLayout;
    self->sortMenu.mpAnimTrans0 = tmp.mpAnimTrans0;
    self->sortMenu.mpAnimTrans1 = tmp.mpAnimTrans1;
    self->sortMenu.field_0x28 = tmp.field_0x28;
    self->sortMenu.field_0x29 = tmp.field_0x29;
    self->sortMenu.field_0x2A = tmp.field_0x2A;
    self->sortMenu.field_0x2B = tmp.field_0x2B;
    func_8011C998(self->sortMenu.mScrollBar, tmp.mScrollBar);
    // The +0x6C s32 array (0x80 bytes) is copied via a rolled 16-iteration
    // 8-byte loop (retail mtctr/bdnz with lwzu/stwu pair per iteration).
    {
        s32* d = self->sortMenu.mArray;
        const s32* s = tmp.mArray;
        for (int k = 0; k < 16; k++) {
            d[0] = s[0];
            d[1] = s[1];
            d += 2;
            s += 2;
        }
    }
    self->sortMenu.mCount = tmp.mCount;
    self->sortMenu.mPage = tmp.mPage;
    self->sortMenu.mSubPage = tmp.mSubPage;
    __dt__9CSortMenuFv(&tmp, -1);

    // Re-init the crystal-info sub-object from a default temp.
    CMCCrystalInfo tmp2;
    __ct__CMCCrystalInfo(&tmp2);
    __ct__UnkClass_8011C974(&self->crystalInfo.mMemRegion1[0], &tmp2.mMemRegion1[0]);
    __ct__UnkClass_8011C974(&self->crystalInfo.mMemRegion2[0], &tmp2.mMemRegion2[0]);
    self->crystalInfo.mFileHandle1 = tmp2.mFileHandle1;
    self->crystalInfo.mFileHandle2 = tmp2.mFileHandle2;
    self->crystalInfo.mArcResAccessor = tmp2.mArcResAccessor;
    self->crystalInfo.mField30 = tmp2.mField30;
    self->crystalInfo.mLayout = tmp2.mLayout;
    self->crystalInfo.mAnimTransform1 = tmp2.mAnimTransform1;
    self->crystalInfo.mAnimTransform2 = tmp2.mAnimTransform2;
    self->crystalInfo.mAnimTransform3 = tmp2.mAnimTransform3;
    self->crystalInfo.mAnimTransform4 = tmp2.mAnimTransform4;
    self->crystalInfo.mEnabled = tmp2.mEnabled;
    self->crystalInfo.mState = tmp2.mState;
    self->crystalInfo.mIsActive = tmp2.mIsActive;
    self->crystalInfo.mField51 = tmp2.mField51;
    // The four unaligned tail words are copied word-wise (retail lwz pair +
    // reversed stw pair per 8-byte chunk).
    *(u32*)&self->crystalInfo.field52[0] = *(u32*)&tmp2.field52[0];
    *(u32*)&self->crystalInfo.field56[0] = *(u32*)&tmp2.field56[0];
    *(u32*)&self->crystalInfo.field5A[0] = *(u32*)&tmp2.field5A[0];
    *(u32*)&self->crystalInfo.field5E[0] = *(u32*)&tmp2.field5E[0];
    __dt__14CMCCrystalInfoFv(&tmp2, -1);

    // Re-init the scroll bar and the system window from default temps.
    CScrollBarData sbTmp;
    __ct__CScrollBar(&sbTmp, 0);
    func_8011C998((u8*)self + 0x250, &sbTmp);
    __dt__10CScrollBarFv(&sbTmp, -1);

    CSysWinData swTmp;
    __ct__CSysWin(&swTmp, 0);
    CSysWinData* sw = reinterpret_cast<CSysWinData*>((u8*)self + 0x290);
    __ct__UnkClass_8011C974(sw->mMemRegion, swTmp.mMemRegion);
    sw->mFileHandle = swTmp.mFileHandle;
    sw->mTagProcessor = swTmp.mTagProcessor;
    sw->mArcAccessor = swTmp.mArcAccessor;
    sw->mLayout = swTmp.mLayout;
    sw->mAnimTrans = swTmp.mAnimTrans;
    sw->field_28 = swTmp.field_28;
    sw->field_2C = swTmp.field_2C;
    sw->field_30 = swTmp.field_30;
    sw->field_34 = swTmp.field_34;
    sw->field_35 = swTmp.field_35;
    sw->field_36 = swTmp.field_36;
    sw->field_37 = swTmp.field_37;
    sw->field_38 = swTmp.field_38;
    sw->field_39 = swTmp.field_39;
    __dt__7CSysWinFv(&swTmp, -1);

    // Read the sub-object layout archives and drive the syswin through its
    // slot-0x88 virtual, then reset the shared BDAT cursor and notify.
    func_801D3064(&self->sortMenu);
    func_8021A718(&self->crystalInfo);
    func_801F34F4((u8*)self + 0x250);
    reinterpret_cast<CSysWinVf88*>((u8*)self + 0x290)->vf_88();
    lbl_eu_806646D0 = 0;
    func_80157824(9, 0);
}
#pragma optimize_for_size off

// Retail 0x8021611c: per-frame crystal-box state machine. When visible and
// in a non-zero state, dispatch on the state through the jump table, then
// refresh the layout anim, sort menu, crystal info and all cursor sub-views.
// Note: retail's table bound is cmpli 14 (15 entries, trailing empty case 14);
// MWCC bounds at the last non-empty case (cmpli 13) - same fixed artifact as
// func_802228B8 in CMCCrystalList (cmpli 7 vs 8).
void func_802142C4(CMCCrystalBox* self) {
    if (self->unk60 == 0) return;
    if (self->unk64 == 0) return;
    switch (self->unk64) {
    case 0: func_80216B7C(self); break;
    case 1: func_80216BC8(self); break;
    case 2: func_80137444(self->subObjPtrs[8], lbl_eu_80668470); break;
    case 3: func_80216C3C(self); break;
    case 4: func_80216C88(self); break;
    case 5: func_80219464(self); self->unk64 = 7; break;
    case 6: func_80216CE0(self); break;
    case 7: func_80216D38(self); break;
    case 8: func_80216D84(self); break;
    case 9: func_80216DD8(self); break;
    case 10: func_80216E1C(self); break;
    case 11: func_80216E6C(self); break;
    case 12: func_80216EB0(self); break;
    case 14: break;   // empty slot; retail keeps 15 table entries
    case 13: func_80216EFC(self);   // no break: falls out into the tail
    }
    // Refresh tail: run the layout virtual at slot 0x38, then update every
    // placed sub-object.
    reinterpret_cast<CLytVf38*>(self->subObjPtrs[5])->vf_30(0);
    func_801D3160((u8*)self + 0xfc);
    func_8021A780((u8*)self + 0x1ec);
    func_801D202C((u8*)self + 0x6c);
    func_801D202C((u8*)self + 0x84);
    func_801D202C((u8*)self + 0x9c);
    func_801D202C((u8*)self + 0xb4);
    func_801D202C((u8*)self + 0xcc);
    func_801D202C((u8*)self + 0xe4);
    func_801F3540((u8*)self + 0x250);
    func_8022B748((u8*)self + 0x290);
}

void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// Retail 0x80216260: draw the crystal box sub-views.
// stmw r30 frame + r30-before-r31 copy order are the -O4,s shapes (pragma).
#pragma optimize_for_size on
void func_80214408(CMCCrystalBox* self, nw4r::lyt::DrawInfo* di) {
    if (self->unk60 == 0) return;
    if (self->unk64 == 0) return;
    func_80137038((nw4r::lyt::Layout*)self->subObjPtrs[5], di, 0, 1);
    func_8021A840((u8*)self + 0x1ec, di);
    func_801D31F8((u8*)self + 0xfc, di);
    u8 lim = self->data.limit;
    u8 active = lim != 0 ? lim : 1;
    if (active > 1) {
        func_801D20B0((u8*)self + 0x84, di);   // CCur09
    }
    func_801D20B0((u8*)self + 0x6c, di);       // CCur07
    func_801D20B0((u8*)self + 0x9c, di);
    func_801D20B0((u8*)self + 0xb4, di);
    func_801D20B0((u8*)self + 0xcc, di);
    func_801D20B0((u8*)self + 0xe4, di);
    func_801F35B0((u8*)self + 0x250, di);      // scrollbar
    func_8022B7C8((u8*)self + 0x290, di);      // syswin
}
#pragma optimize_for_size off

// Retail 0x8021634c: teardown of the whole crystal box. Closes the file
// handles for the first three resource sub-objects, frees the layout/arc
// resources, then destroys every placed member sub-object.
void func_802144F4(CMCCrystalBox* self) {
    func_8003AA8C__5CBdatFUl(2);
    func_801390E0__FPP11CFileHandle((void**)&self->subObjPtrs[0]);
    func_801390E0__FPP11CFileHandle((void**)&self->subObjPtrs[1]);
    func_801390E0__FPP11CFileHandle((void**)&self->subObjPtrs[2]);

    if (self->unk60 != 0) {
        nw4r::lyt::AnimTransform* layout = self->subObjPtrs[5];
        self->unk60 = 0;
        if (layout != 0) {
            if (layout != 0) {
                // Retail dispatches the deleting-dtor-style virtual at vtable
                // slot 0x08 with flags=1 (double check survives as double beq).
                reinterpret_cast<CLytVf08*>(layout)->vf_00(1);
            }
            self->subObjPtrs[5] = 0;
        }
        func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(self->subObjPtrs[3]);
        func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(self->subObjPtrs[4]);
        func_8045F778__17UnkClass_8045F564Fv((u8*)self + 0x4);
        func_8045F778__17UnkClass_8045F564Fv((u8*)self + 0x14);
        func_801D3258((u8*)self + 0xfc);
        func_8021A860((u8*)self + 0x1ec);
        // Deactivate each placed cursor sub-object via its slot-0x0C virtual.
        reinterpret_cast<CCurVf0C*>((u8*)self + 0x6c)->vf_04();
        reinterpret_cast<CCurVf0C*>((u8*)self + 0x84)->vf_04();
        reinterpret_cast<CCurVf0C*>((u8*)self + 0x9c)->vf_04();
        reinterpret_cast<CCurVf0C*>((u8*)self + 0xb4)->vf_04();
        reinterpret_cast<CCurVf0C*>((u8*)self + 0xcc)->vf_04();
        reinterpret_cast<CCurVf0C*>((u8*)self + 0xe4)->vf_04();
        func_801F35DC((u8*)self + 0x250);
        func_8022B7F4((u8*)self + 0x290);
    }
}

u8 func_80214634(CMCCrystalBox* self) {
    if (func_801D32DC((u8*)self + 0xfc) == 0) return 0;
    if (func_8021A8EC((u8*)self + 0x1ec) == 0) return 0;
    if (CScrollBar_isVisible((u8*)self + 0x250) == 0) return 0;
    if (CSysWin_isReady((u8*)self + 0x290) != 0) return self->unk68;
    return 0;
}

u8 getByte_69_802146C0(void* self) { return static_cast<CMCCrystalBox*>(self)->unk69; }

u8 func_802146C8(CMCCrystalBox* self) {
    if (self->unk2D1 != 0) return 1;
    if (self->unk1500 != 0) return 1;
    return self->unk2CE;
}

void syswinGetUnk34_802146F8(void* self) { CSysWin_getUnk34((char*)self + 0x290); }

// Retail 0x80216558: initialise the crystal box into state 1, mirror the
// current item, and point the scrollbar at the selected vec.
void func_80214700(CMCCrystalBox* self) {
    if (self->unk64 != 0) return;
    self->unk64 = 1;
    self->unk69 = 0;
    func_8021899C(self);
    func_8021A8F4(&self->crystalInfo);
    CMCCrystalData* d = &self->data;
    u8 idx = self->unk2CC + self->unk2CD * 10;
    func_8021A9A8(&self->crystalInfo,
                  func_802137DC(d, idx), func_8021384C(d, idx));
    func_80218B10(self);
    float vec[3];
    code80135FDC_setVec3(vec, lbl_eu_80668474, lbl_eu_80668478,
                         lbl_eu_8066845C);
    func_801F3670((u8*)self + 0x250, vec);
    func_801F36BC((u8*)self + 0x250, 8, 0);
    func_801F3850((u8*)self + 0x250, 0);
    func_801F367C((u8*)self + 0x250);
    func_8021900C(self);
}

void func_802147F4(CMCCrystalBox* self) {
    if (self->unk64 != 3) return;
    if (self->unk2CE != 0) return;
    self->unk64 = 4;
    self->unk69 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
    func_801D216C((u8*)self + 0x84, 0);
    func_801D216C((u8*)self + 0xcc, 0);
    func_801D216C((u8*)self + 0xb4, 0);
    func_8021A918((u8*)self + 0x1ec);
    func_801F369C((u8*)self + 0x250);
    func_80219094(self);
}

// Retail 0x802166E4: set state 6 and reset all sub-views.
void func_8021488C(CMCCrystalBox* self) {
    if (self->unk64 != 3) return;
    if (self->unk2CE != 0) return;
    self->unk64 = 6;
    self->unk69 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
    func_801D216C((u8*)self + 0x84, 0);
    func_801D216C((u8*)self + 0xcc, 0);
    func_801D216C((u8*)self + 0xb4, 0);
    func_801F369C((u8*)self + 0x250);
    func_8021A93C((u8*)self + 0x1ec);
    func_8021922C(self);
    func_80138078(0x6d);
}

void func_8021492C(CMCCrystalBox* self) {
    if (self->unk64 != 3) return;
    if (self->unk2CE != 0) return;
    self->unk64 = 8;
    self->unk69 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
    func_801D216C((u8*)self + 0x84, 0);
    func_801D216C((u8*)self + 0xcc, 0);
    func_801D216C((u8*)self + 0xb4, 0);
    func_8021A960((u8*)self + 0x1ec);
    func_802191A4(self);
    func_80138078(0x6d);
}

void func_802149C4(CMCCrystalBox* self) {
    if (self->unk64 != 3) return;
    if (self->unk2CE != 0) return;
    self->unk64 = 0xd;
    self->unk69 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
    func_801D216C((u8*)self + 0x84, 0);
    func_801D216C((u8*)self + 0xcc, 0);
    func_801D216C((u8*)self + 0xb4, 0);
    func_8021A984((u8*)self + 0x1ec);
    func_8021922C(self);
}

void func_80214A54(){}

// Retail 0x80216AD4: update-callback when the sort sub-menu is open and the
// counter animation is running. unk1501/unk1502 are the row/column cursor of
// the 8x8 grid; unk1503 is the page size. The >=8 branch advances by one row
// (wraps at the page size), the <8 branch advances within the row. When the
// sort sub-menu is active instead, scroll it down and drive the +0x9C cursor
// through its slot-0x10 virtual; otherwise step the item cursor (unk2CD),
// resolving the new row from the crystal table, and refresh.
void func_80214C7C(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((u8*)self + 0x290) != 0) return;
    if (self->unk1500 != 0) {
        s8 limit = (s8)self->unk1503;
        if (limit >= 8) {
            u8 v = (u8)(self->unk1501 + 1);
            self->unk1501 = v;
            if ((s8)v >= 8) {
                self->unk1501 = 7;
                u8 w = (u8)(self->unk1502 + 1);
                self->unk1502 = w;
                if ((s8)w > limit - 8) {
                    self->unk1501 = 0;
                    self->unk1502 = 0;
                }
            }
        } else {
            u8 v = (u8)(self->unk1501 + 1);
            self->unk1501 = v;
            if ((s8)v >= limit) {
                self->unk1501 = 0;
                self->unk1502 = 0;
            }
        }
        func_80218B10(self);
        func_8021852C(self);
        func_801F3850((u8*)self + 0x250, (u16)(s8)self->unk1502);
        func_80138078(1);
    } else if (self->unk2CE != 0) {
        // Sort sub-menu active: scroll down one entry and move the cursor.
        func_801D3698((u8*)self + 0xfc);
        char buf[12];
        func_801D3454(buf, (u8*)self + 0xfc);
        ((CCurVf10*)((u8*)self + 0x9c))->vf_08(buf);
        func_80138078(1);
    } else {
        if (self->unk2D1 != 0) return;
        if (self->data.count != 0) {
            if (self->field_14A0 == 8) return;
            if (func_80219AF0(self) != 0) return;
        }
        // Disjunction form: the unk2CD step block is the TRUE target (retail
        // `blt .C58; beq .C58; b return`), the return the fall-through.
        if (self->field_14A0 < 2 || func_80213748(&self->data) == 0) {
        } else {
            return;
        }
        // Step the item cursor (s8 unk2CD), wrapping at 2.
        s8 v = (s8)(self->unk2CD + 1);
        self->unk2CD = (u8)v;
        if (v > 2) {
            self->unk2CD = -2;
        }
        if (self->field_14A0 >= 2) {
            if ((s8)self->unk2CD == -1) self->unk2CD = 0;
        } else if (self->field_14A0 >= 1) {
            if ((s8)self->unk2CD == -2) self->unk2CD = 0;
        } else {
            if ((s8)self->unk2CD == -2) self->unk2CD = -1;
        }
        func_8021852C(self);
        func_80218460(self);
        func_80218B10(self);
        func_80138078(1);
    }
}

// Retail 0x80216D14: update-callback mirror of func_80214C7C with the
// counter animation stepped in the opposite direction: unk1502 decrements by
// the page size (8) and the row cursor wraps through the page; when unk2D1 is
// set, the window-kind byte (unk2D3) selects between the -0x2D4/-0x2D5
// decrement paths and drives the +0x64 state / anim; otherwise the item
// cursor is stepped back (unk2CD) with the -0x2CC countdown.
void func_80214EBC(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((u8*)self + 0x290) != 0) return;
    if (self->unk1500 != 0) {
        if ((s8)self->unk1503 >= 8) {
            u8 w = (u8)(self->unk1502 - 8);
            self->unk1502 = w;
            if ((s8)w < 0) {
                u8 t = (u8)(w + 7);
                self->unk1501 = t;
                self->unk1502 = 0;
                if ((s8)t < 0) {
                    self->unk1501 = 0;
                }
            }
        } else {
            self->unk1501 = 0;
            self->unk1502 = 0;
        }
        func_80218B10(self);
        func_8021852C(self);
        func_801F3850((u8*)self + 0x250, (u16)(s8)self->unk1502);
        func_80138078(1);
    } else if (self->unk2CE != 0) {
        // Sort sub-menu active: page up one page and move the cursor.
        func_801D3724((u8*)self + 0xfc);
        char buf[12];
        func_801D3454(buf, (u8*)self + 0xfc);
        ((CCurVf10*)((u8*)self + 0x9c))->vf_08(buf);
        func_80138078(1);
    } else {
        if (self->unk2D1 == 0) {
            // Item cursor path: skip the -2 sentinel; at -1 the countdown
            // reloads the whole list, otherwise -0x2CC counts down to 9.
            if ((s8)self->unk2CD == -2) return;
            if ((s8)self->unk2CD != -1) {
                s8 c = (s8)self->unk2CC;
                if (c != 0) {
                    u8 v = (u8)(c - 1);
                    self->unk2CC = v;
                    if ((s8)v < 0) {
                        self->unk2CC = 9;
                    }
                    func_8021852C(self);
                    func_80218460(self);
                    func_80218B10(self);
                } else {
                    self->unk2CC = 9;
                    func_80215490(self);
                    func_8021852C(self);
                }
                func_80138078(1);
            } else {
                func_80216698(self);
                func_8021852C(self);
            }
        } else {
            // Window-kind path: unk2D3 0 decrements the -0x2D4 byte, unk2D3
            // 1 (with enough room) decrements -0x2D5 and re-syncs them, then
            // enters state 11 and plays back animation 12.
            if (self->unk2D3 == 2) return;
            u8 g = code80135FDC_getByte_64077();
            if ((s8)self->unk2D3 == 0) {
                u8 v = (u8)(self->unk2D4 - 1);
                self->unk2D4 = v;
                if ((s8)v < 0) {
                    self->unk2D4 = (u8)(g - 1);
                }
                func_802194EC(self);
                func_80138078(1);
            } else if ((s8)self->unk2D3 == 1) {
                if ((u8)g <= 2) return;
                u8 v = (u8)(self->unk2D5 - 1);
                self->unk2D5 = v;
                if ((s8)v < 0) {
                    self->unk2D5 = (u8)(g - 1);
                }
                s8 d5 = (s8)self->unk2D5;
                s8 d4 = (s8)self->unk2D4;
                if (d5 == d4) {
                    u8 v2 = (u8)(d5 - 1);
                    self->unk2D5 = v2;
                    if ((s8)v2 < 0) {
                        self->unk2D5 = (u8)(g - 1);
                    }
                }
                func_802194EC(self);
                self->unk64 = 11;
                func_80219348(self);
                func_80138078(1);
            }
        }
    }
}

// Retail 0x80216F9C: update-callback mirror of func_80214EBC with the
// counter animation stepped FORWARD: unk1502 increments by the page size (8)
// with carry into unk1501; the window-kind path increments -0x2D4/-0x2D5
// (wrapping at the byte from code80135FDC_getByte_64077) before entering
// state 11; the item-cursor path counts -0x2CC up to 9 then wraps to 0.
void func_80215144(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((u8*)self + 0x290) != 0) return;
    if (self->unk1500 != 0) {
        s8 lim = (s8)self->unk1503;
        if (lim >= 8) {
            u8 w = (u8)(self->unk1502 + 8);
            self->unk1502 = w;
            if ((s8)w > lim - 8) {
                u8 t = (u8)((u8)w - (lim - 8));
                self->unk1501 = t;
                self->unk1502 = (u8)(self->unk1503 - 8);
                if ((s8)t >= 8) {
                    self->unk1501 = 7;
                }
            }
        } else {
            u8 t = (u8)(self->unk1503 - 1);
            self->unk1501 = t;
            self->unk1502 = 0;
            if ((s8)t < 0) {
                self->unk1501 = 0;
            }
        }
        func_80218B10(self);
        func_8021852C(self);
        func_801F3850((u8*)self + 0x250, (u16)(s8)self->unk1502);
        func_80138078(1);
    } else if (self->unk2CE != 0) {
        // Sort sub-menu active: page down one page and move the cursor.
        func_801D377C((u8*)self + 0xfc);
        char buf[12];
        func_801D3454(buf, (u8*)self + 0xfc);
        ((CCurVf10*)((u8*)self + 0x9c))->vf_08(buf);
        func_80138078(1);
    } else {
        if (self->unk2D1 == 0) {
            // Item cursor path: at -1 reload the list, otherwise -0x2CC
            // counts up and wraps at 10.
            if ((s8)self->unk2CD == -2) return;
            if ((s8)self->unk2CD != -1) {
                u8 c = self->unk2CC;
                if (c == 9) {
                    self->unk2CC = 0;
                    func_80215408(self);
                    func_8021852C(self);
                } else {
                    u8 v = (u8)(c + 1);
                    self->unk2CC = v;
                    if ((s8)v >= 10) {
                        self->unk2CC = 0;
                    }
                    func_8021852C(self);
                    func_80218460(self);
                    func_80218B10(self);
                }
                func_80138078(1);
            } else {
                func_80216614(self);
                func_8021852C(self);
            }
        } else {
            // Window-kind path: unk2D3 0 increments -0x2D4 (wrapping at g);
            // unk2D3 1 increments -0x2D5 (also wrapping) and re-syncs the
            // pair when equal, then enters state 11 and plays animation 12.
            if (self->unk2D3 == 2) return;
            u8 g = code80135FDC_getByte_64077();
            if ((s8)self->unk2D3 == 0) {
                u8 v = (u8)(self->unk2D4 + 1);
                self->unk2D4 = v;
                if ((s8)v >= (u8)g) {
                    self->unk2D4 = 0;
                }
                func_802194EC(self);
                func_80138078(1);
            } else if ((s8)self->unk2D3 == 1) {
                if ((u8)g <= 2) return;
                u8 v = (u8)(self->unk2D5 + 1);
                self->unk2D5 = v;
                if ((s8)v >= (u8)g) {
                    self->unk2D5 = 0;
                }
                s8 d5 = (s8)self->unk2D5;
                s8 d4 = (s8)self->unk2D4;
                if (d5 == d4) {
                    u8 v2 = (u8)(d5 + 1);
                    self->unk2D5 = v2;
                    if ((s8)v2 >= (u8)g) {
                        self->unk2D5 = 0;
                    }
                }
                func_802194EC(self);
                self->unk64 = 11;
                func_80219348(self);
                func_80138078(1);
            }
        }
    }
}

void func_80215408(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((char*)self + 0x290) != 0) return;
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;
    if ((s8)self->unk2CD == -2) return;

    func_80213788((char*)self + 0x3d8);
    func_802180B4(self);
    func_80218460(self);
    func_80218B10(self);
    func_80138078(0xa);
}

void func_80215490(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((char*)self + 0x290) != 0) return;
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;
    if ((s8)self->unk2CD == -2) return;

    func_802137B4((char*)self + 0x3d8);
    func_802180B4(self);
    func_80218460(self);
    func_80218B10(self);
    func_80138078(0xa);
}
// Retail 0x80217370: open/close the sort sub-menu. When a sub-menu is active
// (unk2CE) it is closed; otherwise the sort menu is opened: two layout panes
// are formatted, copied into the sort menu, the countdown label refreshed, and
// the +0x9C cursor is driven through its slot-0x10 virtual. stmw r28 frame is
// the -O4,s shape (pragma).
#pragma optimize_for_size on
void func_80215518(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((char*)self + 0x290) != 0) return;
    if (self->unk1500 != 0) return;
    if (self->unk2D1 != 0) return;
    if ((s8)self->unk2CD == -2) return;
    if (self->unk2CE != 0) {
        if (func_801D3328(&self->sortMenu) == 0) return;
        func_8021852C(self);
        func_801D216C((u8*)self + 0x6c, 1);
        func_801D216C((u8*)self + 0x9c, 0);
        func_801D3408(&self->sortMenu);
        self->unk2CE = 0;
        func_80138078(6);
    } else {
        if (func_801D3328(&self->sortMenu) == 0) return;
        void* a;
        char* base = lbl_eu_8050888C;
        CLytVf3C* sub = *(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10);
        a = sub->vf_3C(base + 0x63, 1);
        char local[12];
        func_80137924(local, sub->vf_3C(base + 0x5a, 1), a, sub);
        func_801D3430(&self->sortMenu, local);
        func_801D353C(&self->sortMenu, (u8)(self->field_1506 + self->field_1507));
        char local2[12];
        func_801D3454(local2, &self->sortMenu);
        ((CCurVf10*)((u8*)self + 0x9c))->vf_08(local2);
        func_801D216C(&self->pad_6C[0x30], 1);
        func_801D216C((u8*)self + 0x6c, 0);
        func_801D3330(&self->sortMenu);
        self->unk2CE = 1;
        self->unk2CF = 0;
        func_80138078(3);
    }
}
#pragma optimize_for_size off

// Retail 0x80217518: per-frame crystal-box update callback. While the system
// window is open, keep its animation alive (unk1500 grid); otherwise close
// the sort menu, step the window-kind counter, or scan the crystal table for
// the selected slot. The scan block is duplicated in source because retail
// holds two inline copies (one for the count!=0 gate, one for the a0
// fallback). Ends by refreshing the page-2 label and beeping 6.
// stmw r22 frame is the -O4,s shape (pragma).
#pragma optimize_for_size on
void func_802156C0(CMCCrystalBox* self, int a) {
    if (func_801D3328(&self->sortMenu) == 0) return;
    if (CSysWin_getUnk34(&self->sysWin) != 0) {
        if (CSysWin_isActive(&self->sysWin) == 0) {
            return;
        }
        func_8022B8E4(&self->sysWin);
        if (self->unk1500 != 0) {
            func_801D216C((u8*)self + 0xe4, 1);
        }
    } else if (self->unk1500 != 0) {
        self->unk1500 = 0;
        self->unk1501 = 0;
        self->unk1502 = 0;
        func_8021852C(self);
        func_80218B10(self);
    } else if (self->unk2CE != 0) {
        func_8021852C(self);
        func_801D216C((u8*)self + 0x6c, 1);
        func_801D216C((u8*)self + 0x9c, 0);
        func_801D3408(&self->sortMenu);
        self->unk2CE = 0;
    } else if (self->unk2D1 != 0) {
        if (self->unk64 != 3) return;
        if ((s8)self->unk2D3 == 0) {
            self->unk2D1 = 0;
            self->unk69 = 0;
        } else {
            self->unk2D3 = (u8)(self->unk2D3 - 1);
            func_802194EC(self);
            func_8021852C(self);
            if ((s8)self->unk2D3 == 0) {
                self->unk64 = 12;
                func_802193D0(self);
            }
            func_80124270((*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                              ->vf_3C(lbl_eu_8050888C + 0x72, 1),
                          1);
        }
    } else {
        u16 count = self->data.count;
        u8 a0 = self->field_14A0;
        u8* selTable = (u8*)self + 0x1480;
        if (count != 0 &&
            (a0 == 8 || func_80219AF0(self) != 0 ||
             func_80213748(&self->data) != 0)) {
            // scan #1: walk the 10 rows x 30 columns for the selected slot
            void* sel = (void*)func_80215AE8(selTable);
            CMCCrystalData* d = &self->data;
            u8 cur = (u8)d->current;
            u8 last = (u8)((u8)(cur + 1) - 1);
            int found = 0;
            u8 row;
            for (row = 0; row < 10; row++) {
                d->current = row;
                u8 col;
                for (col = 0; col < 30; col++) {
                    if (func_8021384C(d, col) == sel) {
                        func_802136E0((void*)d, col, 0);
                        self->unk2CD = col != 0 ? col / 10 : 0;
                        self->unk2CC = col != 0 ? col - self->unk2CD * 10 : 0;
                        func_8021852C(self);
                        d->current = row;
                        found = 1;
                        break;
                    }
                }
                if (found != 0) break;
            }
            func_80215B18((CMCCrystalBox*)selTable);
            if (found == 0) d->current = last;
            func_802180B4(self);
            func_80218460(self);
            func_80218A80(self);
            func_80218B10(self);
        } else {
            if (a0 == 0) {
                self->unk69 = 0;
            } else {
                // scan #2: same search for the a0 != 0 fallback
                void* sel = (void*)func_80215AE8(selTable);
            CMCCrystalData* d = &self->data;
            u8 cur = (u8)d->current;
            u8 last = (u8)((u8)(cur + 1) - 1);
            int found = 0;
            u8 row;
            for (row = 0; row < 10; row++) {
                d->current = row;
                u8 col;
                for (col = 0; col < 30; col++) {
                    if (func_8021384C(d, col) == sel) {
                        func_802136E0((void*)d, col, 0);
                        self->unk2CD = col != 0 ? col / 10 : 0;
                        self->unk2CC = col != 0 ? col - self->unk2CD * 10 : 0;
                        func_8021852C(self);
                        d->current = row;
                        found = 1;
                        break;
                    }
                }
                if (found != 0) break;
            }
            func_80215B18((CMCCrystalBox*)selTable);
            if (found == 0) d->current = last;
            func_802180B4(self);
            func_80218460(self);
            func_80218A80(self);
            func_80218B10(self);
            }
        }
        func_80124270((*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                          ->vf_3C(lbl_eu_8050888C + 0x7c, 1),
                      selTable[0x20] < 2);
    }
    if (a != 0) func_80138078(6);
}
#pragma optimize_for_size off

// Retail 0x80217940: return the 8-slot selection-table word selected by the
// header (count byte at +0x20, head index at +0x29, slot index byte at
// 0x20+head). Used by the crystal-scan in func_802156C0.
int func_80215AE8(void* self) {
    u8* p = (u8*)self;
    if (p[0x20] == 0) return 0;
    return *(int*)(p + 4 * (s8)p[0x20 + p[0x29]]);
}

void func_80215B18(CMCCrystalBox* self) {
    // Free-slot bookkeeping: unclaim the slot owned at [0x20+slotIdx] and
    // decrement the header counter. Reads after the store are cached in locals
    // so MWCC reloads them once (aliasing).
    if (self->unk20 == 0) return;

    int* table = (int*)self;
    signed char owner = (s8)((u8*)self)[0x20 + ((u8*)self)[0x29]];
    if (table[owner] == 0) return;

    table[owner] = 0;
    u8 cnt = self->unk20;
    u8 t = ((u8*)self)[0x29];
    self->unk20 = cnt - 1;
    ((s8*)self + 0x21)[t] = -1;
    ((u8*)self)[0x29] = t - 1;
    func_80215B78(self);
}

void func_80215D98(){}

u8 getByte_2D4_8021624C(void* self) { return static_cast<CMCCrystalBox*>(self)->unk2D4; }

u8 getByte_2D5_80216254(void* self) { return static_cast<CMCCrystalBox*>(self)->unk2D5; }

// Retail 0x802180B4: rebuild the selected-crystal table. Re-initialises the
// 32-slot transient sort table (via func_80213D74), aggregates the 8-slot
// selection table (+0x1480) into quantity-pair entries, bubble-sorts them by
// quantity, and copies the result into the +0x2D8 sub-table, then refreshes
// the current-item byte and drives the item-implementation virtuals.
// stmw r23 frame is the -O4,s shape (pragma).
#pragma optimize_for_size on
u8* func_8021625C(CMCCrystalBox* self) {
    // Stack layout (retail): temps at +0x8..+0x30, sort table at +0x38,
    // then the head/sel pair at +0x138 (sel is the func_80213D74 table).
    CMCCrystalBoxParam tmp3;      // +0x8  (fill-loop temp)
    CMCCrystalBoxParam t3;        // +0x10 (bubble-sort temp)
    CMCCrystalBoxParam t2;        // +0x18 (bubble-sort temp)
    CMCCrystalBoxParam tmp2;      // +0x20 (add-entry temp)
    CMCCrystalBoxParam pa;        // +0x28 (re-init temp)
    CMCCrystalBoxParam t1;        // +0x30 (bubble-sort temp)
    CMCCrystalBoxParam sortTable[32];  // +0x38
    u8 head[2];                   // +0x138
    CMCCrystalBoxParam sel[32];   // +0x13A (initialised by func_80213D74)

    // Initialise the selection table (head + 32 slots) and copy the slots
    // into the +0x2D8 sub-table.
    func_80213D74(head);
    self->unk2D6 = head[0];
    self->unk2D7 = head[1];
    {
        s32* d = reinterpret_cast<s32*>(self->subTable);
        const s32* s = reinterpret_cast<const s32*>(sel);
        for (int i = 0; i < 0x20; i++) {
            d[0] = s[0];
            d[1] = s[1];
            d += 2;
            s += 2;
        }
    }

    // Clear the sort table twice: a pointer walk, then a slot re-init via a
    // stack temp so each entry is written by func_80213E20.
    {
        CMCCrystalBoxParam* p = sortTable;
        do {
            func_80213E04(p, 0, 0);
            p++;
        } while (p < &sortTable[32]);
    }
    {
        u8 i = 0;
        do {
            func_80213E20(&sortTable[i], func_80213E04(&pa, 0, 0));
            i++;
        } while (i < 0x20);
    }

    // Aggregate the 8-slot selection table: for each placed item, walk its
    // 4 sub-items, accumulate quantities of matching crystal ids in the sort
    // table, and append new ids.
    u8 count = 0;
    for (u8 i = 0; i < 8; i++) {
        unsigned long* item = (unsigned long*)func_802165CC((unsigned long*)((u8*)self + 0x1480), i);
        if (item == 0 || *item == 0) continue;
        for (u8 j = 0; j < 4; j++) {
            CItemImplFacade2* inst = (CItemImplFacade2*)CItem_initItemImplInstances(item);
            u16 name = inst->GetName(item, j);
            if ((s16)name <= 0) continue;
            u8 flag = inst->GetFlag(item, j);
            int found = 0;
            for (u8 k = 0; k < count; k++) {
                if (sortTable[k].m0 == (s16)name) {
                    sortTable[k].m2 += flag;
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                func_80213E20(&sortTable[count],
                              func_80213E04(&tmp2, (s16)name, (s16)flag));
                count++;
            }
        }
    }

    // Bubble-sort the sort table by quantity (ascending).
    {
        int lim = (int)((u8)count - 1);
        for (u8 i = 0; (int)i < lim; i++) {
            for (u8 j = 0; (int)j < lim - i; j++) {
                if (sortTable[j].m2 < sortTable[j + 1].m2) {
                    func_802165E8(&t1, &sortTable[j]);
                    func_80213E20(&sortTable[j],
                                  func_802165E8(&t2, &sortTable[j + 1]));
                    func_80213E20(&sortTable[j + 1],
                                  func_802165E8(&t3, &t1));
                }
            }
        }
    }

    // Copy the sorted table into the +0x2D8 sub-table, bumping the cursor.
    for (u8 i = 0; i < (u8)count; i++) {
        func_80213E04(&tmp3, sortTable[i].m0, sortTable[i].m2);
        u8 cur = self->unk2D6;
        self->unk2D6 = cur + 1;
        func_80213E20(&self->subTable[cur], &tmp3);
    }

    // Refresh the current-item byte from the first placed item, then drive
    // each placed item's slot-0x10 virtual.
    for (u8 i = 0; i < 8; i++) {
        unsigned long* item = (unsigned long*)func_802165CC((unsigned long*)((u8*)self + 0x1480), i);
        if (item == 0 || *item == 0) continue;
        CItemImplFacade2* inst = (CItemImplFacade2*)CItem_initItemImplInstances(item);
        self->unk2D7 = (u8)inst->GetCount(item);
        break;
    }
    for (u8 i = 0; i < 8; i++) {
        unsigned long* item = (unsigned long*)func_802165CC((unsigned long*)((u8*)self + 0x1480), i);
        if (item == 0 || *item == 0) continue;
        CItemImplFacade2* inst = (CItemImplFacade2*)CItem_initItemImplInstances(item);
        inst->vf_08(item);
    }

    return &self->unk2D6;
}
#pragma optimize_for_size off

// Table lookup used by the 8-slot selection loop (func_80218A80). Retail calls
// it out-of-line; keep it non-inlined so the caller emits `bl func_802165CC`
// instead of the compiler eliminating the read chain.
__declspec(noinline) unsigned long func_802165CC(unsigned long* table, unsigned int idx) {
    if (idx >= 8) {
        return 0;
    }
    return table[(unsigned char)idx];
}

void copyCrystalBoxParam_802165E8(CMCCrystalBoxParam *dest, const CMCCrystalBoxParam *src) {
    unsigned short *d = (unsigned short *)dest;
    const unsigned short *s = (const unsigned short *)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    unsigned char *db = (unsigned char *)dest;
    const unsigned char *sb = (const unsigned char *)src;
    db[6] = sb[6];
    db[7] = sb[7];
}

void func_80216614(CMCCrystalBox* self) {
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;

    // Countdown timer at +0x14F2 increments; when it exceeds +0x14F1 it wraps to 0.
    u8 v = self->field_14F2 + 1;
    self->field_14F2 = v;
    if ((s8)v >= self->field_14F1) {
        self->field_14F2 = 0;
    }

    func_80219994(self, 0);
    func_80218018(self);
    func_80138078(0x70);
}

void func_80216698(CMCCrystalBox* self) {
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;

    // Countdown timer at +0x14F2; when it wraps sub-range, reload from +0x14F1.
    u8 v = self->field_14F2 - 1;
    self->field_14F2 = v;
    if ((s8)v < 0) {
        self->field_14F2 = self->field_14F1 - 1;
    }

    func_80219994(self, 1);
    func_80218018(self);
    func_80138078(0x70);
}

// Retail 0x8021B7EC: refresh the two countdown labels. Picks the pair of
// indices (current ticker + the neighbour), formats "N+1" into two panes via
// the layout sub-object's slot-0x3C virtual, and sets each pane's visibility
// from whether the crystal entry is present / selected.
void func_80219994(CMCCrystalBox* self, int dir) {
    u8 pair[2];
    char buf1[0x20];
    char buf2[0x20];
    u8 t0 = self->field_14F2;
    pair[0] = t0;
    s8 sv = (s8)t0;
    // dir selects the neighbour: 0 -> sv-1, else sv+1. Retail hoists the
    // cmpwi r4,0 to the top and the subi runs eager (discarded when dir!=0).
    s32 t = sv - 1;
    if (dir != 0) {
        t = sv + 1;
    }
    u8 t1 = (u8)t;
    pair[1] = t1;
    s8 sc = (s8)t1;
    if (sc < 0) {
        pair[1] = self->field_14F1 - 1;
    } else if (sc >= self->field_14F1) {
        pair[1] = 0;
    }
    u8 i = 0;
    do {
        s32 val = (s8)pair[i] + 1;
        sprintf(buf2, lbl_eu_8050888C + 0x86, val);
        sprintf(buf1, lbl_eu_8050888C + 0x95, val);
        s32 idx = (s8)pair[i];
        int flagA = 0;
        int flagB = 0;
        if (self->field_14EC[idx] != 0) {
            if (idx == (s8)self->field_14F2) {
                flagA = 1;
            } else {
                flagB = 1;
            }
        }
        // Retail reloads layout and the +0x10 sub-object per call (no CSE).
        func_80124270((*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))->vf_3C(buf2, 1), flagA);
        func_80124270((*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))->vf_3C(buf1, 1), flagB);
        i++;
    } while (i < 2);
}

void func_80216718(){}

// Retail 0x802186A8: crystal-box update with a system-window (CSysWin at
// +0x290) tie-in. When the sort menu is idle and the cursor is not in the
// -1 state, either refresh the open window (field_14A0 == 0 with a pending
// crystal) or open/update it with the current item's name; otherwise fall
// back to opening the window for the first time.
// stmw r30 frame is the -O4,s shape (pragma).
#pragma optimize_for_size on
void func_80216850(CMCCrystalBox* self) {
    if (func_801D3320(&self->sortMenu) != 0) return;
    if ((s8)self->unk2CD == -1) return;

    if (self->field_14A0 == 0) {
        if (CSysWin_getUnk34(&self->sysWin) != 0) {
            // Window already open: keep it alive while the crystal animates.
            if (self->unk1500 == 0) return;
            if (CSysWin_isActive(&self->sysWin) == 0) return;
            func_8022B8E4(&self->sysWin);
            if (self->unk1500 == 0) return;
            func_801D216C((u8*)self + 0xe4, 1);
            return;
        }
        // No window: only open it when a crystal exists at the current slot.
        if (func_8021384C(&self->data,
                          (u8)(self->unk2CC + (u8)self->unk2CD * 10)) == 0) {
            func_80138078(5);
            return;
        }
    }

    if (self->unk1500 != 0) {
        if (CSysWin_getUnk34(&self->sysWin) != 0) {
            // Window already open: keep it alive while the crystal animates.
            if (CSysWin_isActive(&self->sysWin) == 0) return;
            func_8022B8E4(&self->sysWin);
            if (self->unk1500 == 0) return;
            func_801D216C((u8*)self + 0xe4, 1);
            return;
        }
        if (self->unk1504 != 0) {
            // Open the window with the current item name; unk1504 doubles as
            // the 3-arg func_8013639C key (retail hoists the r5 load above the
            // branch that also tests it).
            char* msg = func_8013639C((const void*)lbl_eu_806646D0,
                                      lbl_eu_8050888C + 0xa4, self->unk1504);
            func_8022B90C(&self->sysWin, 0);
            func_8022B9B4(&self->sysWin, (u32)msg, 0);
            func_8022BFC8(&self->sysWin, 1);
            func_8022B8B8(&self->sysWin);
            func_801D216C((u8*)self + 0xe4, 0);
            return;
        }
        func_80138078(5);
        return;
    }

    // First open: mark active and refresh the list.
    if (CSysWin_getUnk34(&self->sysWin) != 0) return;
    self->unk1500 = 1;
    self->unk1501 = 0;
    self->unk1502 = 0;
    func_8021852C(self);
    func_80218B10(self);
    func_80138078(2);
}
#pragma optimize_for_size off

// Retail 0x80218878: return the current crystal-state action id (0x33-0x36)
// or 0x30+active depending on which sub-view / flag is pending.
// The trailing `0x30 + (field_14A0 != 0)` is the -O4,s setnz idiom
// (subic/subfe); the optimize_for_size pragma selects that codegen.
#pragma optimize_for_size on
int func_80216A20(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((u8*)self + 0x290) != 0) return 0x0;
    if (func_801D3320((u8*)self + 0xfc) != 0) return 0x32;
    if (self->unk1500 != 0) return 0x36;
    if ((s8)self->unk2CD == -1) return 0x35;
    if ((s8)self->unk2CD == -2) return 0x34;
    u8 idx = self->unk2CC + (u8)self->unk2CD * 10;
    if (func_80213710((u8*)self + 0x3d8, idx) != 0) return 0x33;
    return 0x30 + (self->field_14A0 != 0);
}
#pragma optimize_for_size off

void func_80216AEC(CMCCrystalBox* self) {
    if (CSysWin_getUnk34((u8*)self + 0x290) != 0) return;
    if (self->unk1500 != 0) return;
    if (self->unk2CE != 0) return;
    if (self->unk2D1 != 0) return;
    if (self->field_14A0 != 0) return;
    self->unk2CD = -1;
    func_8021852C(self);
    func_80218460(self);
    func_80218B10(self);
    func_80138078(0x2);
}

// Retail 0x802189D4: when sub-object 6's animation finishes, enter state 2
// and play back animation 7.
__declspec(noinline) void func_80216B7C(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[6], lbl_eu_80668470) != 0) {
        self->unk64 = 2;
        func_80219094(self);
    }
}

__declspec(noinline) void func_80216BC8(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[7], lbl_eu_80668470) == 0) return;
    self->unk64 = 3;
    func_8021911C(self);
    self->unk69 = 1;
    func_801D216C((u8*)self + 0x6c, 1);
    func_801D216C((u8*)self + 0x84, 1);
    func_8021852C(self);
}

// Retail 0x80218A94: when sub-object 7's animation finishes, play back
// animation 6 and enter state 5.
__declspec(noinline) void func_80216C3C(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[7], lbl_eu_80668470) != 0) {
        func_8021900C(self);
        self->unk64 = 5;
    }
}

__declspec(noinline) void func_80216C88(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[6], lbl_eu_80668470) == 0) return;
    self->unk69 = 1;
    self->unk64 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
}

__declspec(noinline) void func_80216CE0(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[14], lbl_eu_80668470) == 0) return;
    self->unk69 = 1;
    self->unk64 = 0;
    func_801D216C((u8*)self + 0x6c, 0);
}

// Retail 0x80218B90: when sub-object 9's animation finishes, enter state 9
// and play back animation 10.
__declspec(noinline) void func_80216D38(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[9], lbl_eu_80668470) != 0) {
        self->unk64 = 9;
        func_8021922C(self);
    }
}

__declspec(noinline) void func_80216D84(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[10], lbl_eu_80668470) == 0) return;
    self->unk69 = 1;
    func_8021852C(self);
    self->unk64 = 3;
}

// Retail 0x80218C30: when sub-object 11's animation finishes, enter state 3.
__declspec(noinline) void func_80216DD8(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[11], lbl_eu_80668470) != 0) {
        self->unk64 = 3;
    }
}

__declspec(noinline) void func_80216E1C(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[12], lbl_eu_80668470) == 0) return;
    self->unk64 = 3;
    self->subObjPtrs[12]->SetFrame(lbl_eu_8066845C);
}

// Retail 0x80218CC4: when sub-object 13's animation finishes, enter state 3.
__declspec(noinline) void func_80216E6C(CMCCrystalBox* self) {
    if (func_80137444(self->subObjPtrs[13], lbl_eu_80668470) != 0) {
        self->unk64 = 3;
    }
}

__declspec(noinline) void func_80216EB0(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[10], lbl_eu_80668470) == 0) return;
    self->unk64 = 0xe;
    func_802191A4(self);
}

__declspec(noinline) void func_80216EFC(CMCCrystalBox* self) {
    if (func_80137510(self->subObjPtrs[9], lbl_eu_80668470) == 0) return;
    self->unk69 = 1;
    self->data.current = 0;   // +0x13DC
    func_80218018(self);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
    func_8021911C(self);
    func_8021852C(self);
    func_801D216C((u8*)self + 0x84, 1);
    self->unk64 = 3;
}

void func_80216F8C(CMCCrystalBox* self){}

// Retail 0x80218EF0: resolve the message texture for one crystal slot and
// publish it into the box. When the slot holds an item, its kind picks the
// message id; otherwise the crystal id is looked up through the BDAT table.
// stmw r27 frame is the -O4,s shape (pragma).
#pragma optimize_for_size on
void func_80217098(CMCCrystalBox* self, u16 id, unsigned int* item, u8 i) {
    // Retail materialises the null-checked item into a saved register via a
    // conditional move (cmpwi on the param reg + mr/li pair) before the gate.
    unsigned int* it = item != 0 ? item : 0;
    const char* msg = 0;
    if (it != 0) {
        CItemImplFacade3* inst = (CItemImplFacade3*)CItem_initItemImplInstances(it);
        if (inst->GetBoxed(it) == 0) {
            if (func_801C6E90(it) != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[4])
                          ->GetResource(0x74696D67, func_80138F78(0x155), 0);
            } else {
                const char* m = 0;
                switch ((((u8*)it)[7] >> 2) & 0x3F) {
                case 4: m = func_80138F78(0x144); break;
                case 5: m = func_80138F78(0x145); break;
                case 6: m = func_80138F78(0x146); break;
                case 7: m = func_80138F78(0x147); break;
                case 8: m = func_80138F78(0x148); break;
                case 9: m = func_80138F78(0x149); break;
                }
                if (m != 0) {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              self->subObjPtrs[4])
                              ->GetResource(0x74696D67, m, 0);
                } else {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              self->subObjPtrs[3])
                              ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
                }
            }
        } else {
            const char* m = 0;
            switch ((((u8*)it)[7] >> 2) & 0x3F) {
            case 4: m = func_80138F78(0x144); break;
            case 5: m = func_80138F78(0x145); break;
            case 6: m = func_80138F78(0x146); break;
            case 7: m = func_80138F78(0x147); break;
            case 8: m = func_80138F78(0x148); break;
            case 9: m = func_80138F78(0x149); break;
            }
            if (m != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[4])
                          ->GetResource(0x74696D67, m, 0);
            } else {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[3])
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        }
    }
    if (msg == 0) {
        if (id != 0) {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      self->subObjPtrs[4])
                      ->GetResource(
                          0x74696D67,
                          func_80138F78(func_80136254(lbl_eu_806640EC,
                                                       lbl_eu_8050888C + 0xbc,
                                                       id)),
                          0);
            if (msg == 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[3])
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        } else {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      self->subObjPtrs[3])
                      ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
        }
    }
    if (msg != 0) {
        char buf[0x20];
        sprintf(buf, lbl_eu_8050888C + 0xc6, i + 1);
        func_80137E7C((nw4r::lyt::Layout*)self->subObjPtrs[5], buf, (char*)msg);
    }
}
#pragma optimize_for_size off

// Retail 0x8021928C: mirror of func_80217098 with the countdown pane label
// at a different offset. stmw r27 frame is the -O4,s shape (pragma).
#pragma optimize_for_size on
void func_80217434(CMCCrystalBox* self, u16 id, unsigned int* item, u8 i) {
    unsigned int* it = item != 0 ? item : 0;
    const char* msg = 0;
    if (it != 0) {
        CItemImplFacade3* inst = (CItemImplFacade3*)CItem_initItemImplInstances(it);
        if (inst->GetBoxed(it) == 0) {
            if (func_801C6E90(it) != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[4])
                          ->GetResource(0x74696D67, func_80138F78(0x155), 0);
            } else {
                const char* m = 0;
                switch ((((u8*)it)[7] >> 2) & 0x3F) {
                case 4: m = func_80138F78(0x144); break;
                case 5: m = func_80138F78(0x145); break;
                case 6: m = func_80138F78(0x146); break;
                case 7: m = func_80138F78(0x147); break;
                case 8: m = func_80138F78(0x148); break;
                case 9: m = func_80138F78(0x149); break;
                }
                if (m != 0) {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              self->subObjPtrs[4])
                              ->GetResource(0x74696D67, m, 0);
                } else {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              self->subObjPtrs[3])
                              ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
                }
            }
        } else {
            const char* m = 0;
            switch ((((u8*)it)[7] >> 2) & 0x3F) {
            case 4: m = func_80138F78(0x144); break;
            case 5: m = func_80138F78(0x145); break;
            case 6: m = func_80138F78(0x146); break;
            case 7: m = func_80138F78(0x147); break;
            case 8: m = func_80138F78(0x148); break;
            case 9: m = func_80138F78(0x149); break;
            }
            if (m != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[4])
                          ->GetResource(0x74696D67, m, 0);
            } else {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[3])
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        }
    }
    if (msg == 0) {
        if (id != 0) {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      self->subObjPtrs[4])
                      ->GetResource(
                          0x74696D67,
                          func_80138F78(func_80136254(lbl_eu_806640EC,
                                                       lbl_eu_8050888C + 0xbc,
                                                       id)),
                          0);
            if (msg == 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[3])
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        } else {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      self->subObjPtrs[3])
                      ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
        }
    }
    if (msg != 0) {
        char buf[0x20];
        sprintf(buf, lbl_eu_8050888C + 0xd4, i + 1);
        func_80137E7C((nw4r::lyt::Layout*)self->subObjPtrs[5], buf, (char*)msg);
    }
}
#pragma optimize_for_size off

// Item-kind check: vtable word bits 16-19 == 9 and byte at +7 has flag 1.
__declspec(noinline) u8 func_80217BDC(void* self) {
    u32 word = *(u32*)self;
    return ((word >> 16) & 0xF) == 9 && (u32)(((u8*)self)[7] & 3) == 1;
}

// Retail 0x80219628: resolve the message texture for a crystal slot, then
// publish both the message and the remaining-capacity string. The slot-0x80
// item state picks the "in box" vs "not yet" message sets; a placed item's
// sub-count drives the capacity label. stmw r26 frame is the -O4,s shape
// (pragma).
#pragma optimize_for_size on
void func_802177D0(CMCCrystalBox* self, u16 id, unsigned int* item, u8 i) {
    unsigned int* it = item != 0 ? item : 0;
    const char* msg = 0;
    if (it != 0) {
        CItemImplFacade3* inst = (CItemImplFacade3*)CItem_initItemImplInstances(it);
        if (inst->GetBoxed(it) == 0) {
            if (func_801C6E90(it) != 0 || func_80217BDC(it) != 0 ||
                ((it[0] >> 16) & 0xF) == 9) {
                const char* m = 0;
                switch ((u16)((CItemImplFacade3*)CItem_initItemImplInstances(it))
                            ->GetCount(it)) {
                case 1: m = func_80138F78(0x191); break;
                case 2: m = func_80138F78(0x190); break;
                case 3: m = func_80138F78(0x18f); break;
                case 4: m = func_80138F78(0x18e); break;
                case 5: m = func_80138F78(0x18d); break;
                }
                if (m != 0) {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              self->subObjPtrs[4])
                              ->GetResource(0x74696D67, m, 0);
                } else {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              self->subObjPtrs[3])
                              ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
                }
            }
        } else {
            const char* m = 0;
            switch ((u16)((CItemImplFacade3*)CItem_initItemImplInstances(it))
                        ->GetCount(it)) {
            case 1: m = func_80138F78(0x19c); break;
            case 2: m = func_80138F78(0x19b); break;
            case 3: m = func_80138F78(0x19a); break;
            case 4: m = func_80138F78(0x199); break;
            case 5: m = func_80138F78(0x198); break;
            }
            if (m != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[4])
                          ->GetResource(0x74696D67, m, 0);
            } else {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[3])
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        }
    }
    if (msg == 0) {
        if (id != 0) {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      self->subObjPtrs[4])
                      ->GetResource(
                          0x74696D67,
                          func_80138F78(func_80136254(lbl_eu_806640EC,
                                                       lbl_eu_8050888C + 0xe2,
                                                       id)),
                          0);
            if (msg == 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[3])
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        } else {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      self->subObjPtrs[3])
                      ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
        }
    }
    if (msg != 0) {
        char buf[0x20];
        int i1 = i + 1;
        sprintf(buf, lbl_eu_8050888C + 0xe7, i1);
        func_80137E7C((nw4r::lyt::Layout*)self->subObjPtrs[5], buf, (char*)msg);
        const char* cap = 0;
        if (it != 0) {
            cap = func_80136190(lbl_eu_8050888C + 0xf4, lbl_eu_8050888C + 0xfd,
                                30 - ((u8)((CItemImplFacade3*)CItem_initItemImplInstances(it))
                                         ->GetCount(it) - 1));
        }
        sprintf(buf, lbl_eu_8050888C + 0x102, i1);
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], buf, (char*)cap, 0);
    }
}
#pragma optimize_for_size off

// Retail 0x80219A64: mirror of func_802177D0 with the two countdown pane
// labels at different offsets. stmw r26 frame is the -O4,s shape (pragma).
#pragma optimize_for_size on
void func_80217C0C(CMCCrystalBox* self, u16 id, unsigned int* item, u8 i) {
    unsigned int* it = item != 0 ? item : 0;
    const char* msg = 0;
    if (it != 0) {
        CItemImplFacade3* inst = (CItemImplFacade3*)CItem_initItemImplInstances(it);
        if (inst->GetBoxed(it) == 0) {
            if (func_801C6E90(it) != 0 || func_80217BDC(it) != 0 ||
                ((it[0] >> 16) & 0xF) == 9) {
                const char* m = 0;
                switch ((u16)((CItemImplFacade3*)CItem_initItemImplInstances(it))
                            ->GetCount(it)) {
                case 1: m = func_80138F78(0x191); break;
                case 2: m = func_80138F78(0x190); break;
                case 3: m = func_80138F78(0x18f); break;
                case 4: m = func_80138F78(0x18e); break;
                case 5: m = func_80138F78(0x18d); break;
                }
                if (m != 0) {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              self->subObjPtrs[4])
                              ->GetResource(0x74696D67, m, 0);
                } else {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              self->subObjPtrs[3])
                              ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
                }
            }
        } else {
            const char* m = 0;
            switch ((u16)((CItemImplFacade3*)CItem_initItemImplInstances(it))
                        ->GetCount(it)) {
            case 1: m = func_80138F78(0x19c); break;
            case 2: m = func_80138F78(0x19b); break;
            case 3: m = func_80138F78(0x19a); break;
            case 4: m = func_80138F78(0x199); break;
            case 5: m = func_80138F78(0x198); break;
            }
            if (m != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[4])
                          ->GetResource(0x74696D67, m, 0);
            } else {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[3])
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        }
    }
    if (msg == 0) {
        if (id != 0) {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      self->subObjPtrs[4])
                      ->GetResource(
                          0x74696D67,
                          func_80138F78(func_80136254(lbl_eu_806640EC,
                                                       lbl_eu_8050888C + 0xe2,
                                                       id)),
                          0);
            if (msg == 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          self->subObjPtrs[3])
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        } else {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      self->subObjPtrs[3])
                      ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
        }
    }
    if (msg != 0) {
        char buf[0x20];
        int i1 = i + 1;
        sprintf(buf, lbl_eu_8050888C + 0x10e, i1);
        func_80137E7C((nw4r::lyt::Layout*)self->subObjPtrs[5], buf, (char*)msg);
        const char* cap = 0;
        if (it != 0) {
            cap = func_80136190(lbl_eu_8050888C + 0xf4, lbl_eu_8050888C + 0xfd,
                                30 - ((u8)((CItemImplFacade3*)CItem_initItemImplInstances(it))
                                         ->GetCount(it) - 1));
        }
        sprintf(buf, lbl_eu_8050888C + 0x11e, i1);
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], buf, (char*)cap, 0);
    }
}
#pragma optimize_for_size off

// Retail 0x80219F0C: rebuild the crystal-box display. Depending on the row
// count, the per-row number panes and the countdown label are refreshed, the
// selection table is repopulated from the current page, and every slot pane
// is re-labelled with the message resources from func_80217098 / func_802177D0.
// _savegpr_22 frame is the -O4,s shape (pragma).
#pragma optimize_for_size on
void func_802180B4(CMCCrystalBox* self) {
    u8 active = self->data.limit;
    if (active == 0) active = 1;
    CMCCrystalData* d = &self->data;
    char buf1[0x20];
    char buf2[0x20];
    float vec[3];
    if (active > 1) {
        func_80124270((*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                          ->vf_3C(lbl_eu_8050888C + 0x137, 1),
                      1);
        for (u8 n = 0; n < 10; n++) {
            sprintf(buf1, lbl_eu_8050888C + 0x13e, n + 1);
            func_80124270((*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                              ->vf_3C(buf1, 1),
                          n < active);
        }
        func_80136910((nw4r::lyt::Layout*)self->subObjPtrs[5],
                      lbl_eu_8050888C + 0x14d, (u8)(d->current + 1));
        nw4r::lyt::Pane* pane =
            (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                ->vf_3C(lbl_eu_8050888C + 0x156, 1);
        vec[0] = self->field_14F4;
        vec[1] = self->field_14F8;
        vec[2] = self->field_14FC;
        vec[0] += lbl_eu_8066847C * (double)((u8)(d->current + 1) - 1)
                + lbl_eu_8066847C * (double)(u8)(10 - active);
        copyVEC3(&((PaneTranslateMirror*)pane)->mX, vec);
    } else {
        func_80124270((*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                          ->vf_3C(lbl_eu_8050888C + 0x137, 1),
                      0);
    }
    for (u8 i = 0; i < 0x1E; i++) {
        u16 v = func_802137DC(d, i);
        unsigned int* obj =
            reinterpret_cast<unsigned int*>(func_8021384C(d, i));
        func_80217098(self, v, obj, i);
        func_802177D0(self, v, obj, i);
        int i1 = i + 1;
        sprintf(buf2, lbl_eu_8050888C + 0x164, i1);
        int f1 = func_80213710(d, i);
        func_80124270((*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                          ->vf_3C(buf2, 1),
                      f1);
        u8 found = 0;
        if (func_80213710(d, i) == 0) {
            u8 k = 0;
            while (k < self->field_14EA) {
                u16 id2 = (k < self->field_14EA) ? self->field_14AA[k] : 0;
                if (obj != 0) {
                    u8 j = 0;
                    do {
                        CItemImplFacade2* inst =
                            (CItemImplFacade2*)CItem_initItemImplInstances(obj);
                        if (id2 == inst->GetName(obj, j)) {
                            found = 1;
                            break;
                        }
                        j++;
                    } while (j < 4);
                    if (found != 0) break;
                }
                k++;
            }
        }
        sprintf(buf2, lbl_eu_8050888C + 0x170, i1);
        func_80124270((*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                          ->vf_3C(buf2, 1),
                      found);
        sprintf(buf2, lbl_eu_8050888C + 0x17f, i1);
        func_80124270((*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                          ->vf_3C(buf2, 1),
                      found);
    }
}
#pragma optimize_for_size off

// Retail 0x8021A7F4: refresh the sort-menu label from the current crystal id.
// stmw r28 frame + check-at-top loop are the -O4,s shapes (pragma).
#pragma optimize_for_size on
void func_8021899C(CMCCrystalBox* self) {
    int arr[3];
    const unsigned long* src = lbl_eu_80508870;
    arr[0] = *src++;
    arr[1] = *src++;
    arr[2] = *src++;
    unsigned long id = func_8015780C(9);
    int idx = (int)id & 0xFFFF;
    char* s1 = (char*)func_80136190(lbl_eu_8050888C + 0xf4, lbl_eu_8050888C + 0xfd, arr[idx]);
    func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5],
                  lbl_eu_8050888C + 0x1ca, s1, 0);
    if (func_801D32DC(&self->sortMenu) != 0) {
        func_801D350C(&self->sortMenu);
        u8 i = 0;
        while (1) {
            if (arr[i] <= 0) break;
            func_801D3518(&self->sortMenu,
                          (char*)func_80136190(lbl_eu_8050888C + 0xf4,
                                        lbl_eu_8050888C + 0xfd, arr[i]));
            i++;
        }
        func_801D353C(&self->sortMenu, self->field_1506 + self->field_1507);
    }
}
#pragma optimize_for_size off

// Retail 0x8021A8D8: walk the 8-slot selection table at +0x1480 and feed each
// entry (id = high 16 bits of the first word) to the two placement handlers.
void func_80218A80(CMCCrystalBox* self) {
    unsigned int i;
    for (i = 0; i < 8; i++) {
        unsigned int* item = (unsigned int*)func_802165CC((unsigned long*)((u8*)self + 0x1480), (unsigned char)i);
        unsigned int raw;
        if (item != 0) {
            raw = item[0] >> 20;
        } else {
            raw = 0;
        }
        unsigned short id = (unsigned short)raw;
        func_80217434(self, id, item, (unsigned char)i);
        func_80217C0C(self, id, item, (unsigned char)i);
    }
}

// Retail 0x8021A384: refresh the crystal-box labels and sub-cursors. When
// the sort grid is active (unk1500), the grid cursor row is formatted into
// the +0xE4 cursor; otherwise the sort-menu / countdown / normal paths
// format a pane pair and drive the +0x6C/+0xCC cursor; the window-kind
// (unk2D1) paths scale a quad by a pane float and drive +0xB4/+0xCC.
// _savegpr_29 frame is the -O4,s shape (pragma).
#pragma optimize_for_size on
void func_8021852C(CMCCrystalBox* self) {
    // Buffers declared in retail stack order (first-declared -> highest slot).
    char bufD0[0x20];   // sort-menu sprintf
    char bufB0[0x20];   // countdown sprintf
    char buf90[0x20];   // normal sprintf
    char buf70[0x20];   // window-kind <2 sprintf
    char buf50[0x20];   // window-kind >=2 sprintf
    f32  buf44[3];      // window-kind <2 quad
    f32  buf38[3];      // window-kind >=2 quad
    char buf2C[0xC];    // sort-grid cursor row
    char buf20[0xC];    // sort-menu quad
    char buf14[0xC];    // countdown quad
    char buf8[0xC];     // normal quad
    int flag;           // window-kind <2 guard (retail keeps it in r30)
    if (self->unk1500 != 0) {
        func_8021AED0(buf2C, &self->crystalInfo, self->unk1501);
        ((CCurVf10*)((u8*)self + 0xe4))->vf_08(buf2C);
        func_801D216C(&self->pad_6C[0x78], 1);
        func_801D216C(&self->pad_6C[0x00], 0);
        func_801D216C(&self->pad_6C[0x60], 0);
        func_801D216C(&self->pad_6C[0x48], 0);
    } else {
        func_801D216C(&self->pad_6C[0x78], 0);
        if (self->unk2D1 == 0) {
            if ((s8)self->unk2CD == -2) {
                // sort-menu path: page label + the two panes into +0xCC.
                sprintf(bufD0, lbl_eu_8050888C + 0x197);
                nw4r::lyt::Pane* paneA = (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                                             ->vf_3C(bufD0, 1);
                nw4r::lyt::Pane* paneB = (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                                             ->vf_3C(lbl_eu_8050888C + 0x63, 1);
                func_80137924(buf20, paneA, paneB, *(void**)((u8*)self->subObjPtrs[5] + 0x10));
                ((CCurVf10*)((u8*)self + 0xcc))->vf_08(buf20);
                func_801D216C(&self->pad_6C[0x60], 1);
                func_801D216C(&self->pad_6C[0x00], 0);
            } else if ((s8)self->unk2CD == -1) {
                // countdown path: ticker + 1 into +0x6C.
                sprintf(bufB0, lbl_eu_8050888C + 0x1a2, (s8)self->field_14F2 + 1);
                nw4r::lyt::Pane* paneA = (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                                             ->vf_3C(bufB0, 1);
                nw4r::lyt::Pane* paneB = (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                                             ->vf_3C(lbl_eu_8050888C + 0x63, 1);
                func_80137924(buf14, paneA, paneB, *(void**)((u8*)self->subObjPtrs[5] + 0x10));
                ((CCurVf10*)((u8*)self + 0x6c))->vf_08(buf14);
                func_801D216C(&self->pad_6C[0x00], 1);
                func_801D216C(&self->pad_6C[0x60], 0);
            } else {
                // normal path: the current crystal index + 1 into +0x6C.
                sprintf(buf90, lbl_eu_8050888C + 0xe7,
                        (s8)self->unk2CC + (s8)self->unk2CD * 10 + 1);
                nw4r::lyt::Pane* paneA = (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                                             ->vf_3C(buf90, 1);
                nw4r::lyt::Pane* paneB = (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                                             ->vf_3C(lbl_eu_8050888C + 0x63, 1);
                func_80137924(buf8, paneA, paneB, *(void**)((u8*)self->subObjPtrs[5] + 0x10));
                ((CCurVf10*)((u8*)self + 0x6c))->vf_08(buf8);
                func_801D216C(&self->pad_6C[0x00], 1);
                func_801D216C(&self->pad_6C[0x60], 0);
            }
        } else {
            if ((s8)self->unk2D3 < 2) {
                // window-kind counter path: scale the quad by the pane float.
                sprintf(buf70, lbl_eu_8050888C + 0x1ae, (s8)self->unk2D3 + 1);
                nw4r::lyt::Pane* paneA = (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                                             ->vf_3C(buf70, 1);
                func_801375A0((char*)buf44, paneA);
                nw4r::lyt::Pane* paneB = (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                                             ->vf_3C(lbl_eu_8050888C + 0x63, 1);
                buf44[0] *= *(f32*)((u8*)paneB + 0x44);
                ((CCurVf10*)((u8*)self + 0xb4))->vf_08(buf44);
                func_801D216C(&self->pad_6C[0x48], 1);
                func_801D216C(&self->pad_6C[0x60], 0);
                flag = 0;
                if (code80135FDC_getByte_64077() > 2 || (s8)self->unk2D3 != 1) {
                    flag = 1;
                }
                func_801D2670(&self->pad_6C[0x48], flag);
            } else {
                // window-kind done path: +0xCC cursor.
                sprintf(buf50, lbl_eu_8050888C + 0x1bd);
                nw4r::lyt::Pane* paneA = (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                                             ->vf_3C(buf50, 1);
                func_801375A0((char*)buf38, paneA);
                nw4r::lyt::Pane* paneB = (*(CLytVf3C**)((u8*)self->subObjPtrs[5] + 0x10))
                                             ->vf_3C(lbl_eu_8050888C + 0x63, 1);
                buf38[0] *= *(f32*)((u8*)paneB + 0x44);
                ((CCurVf10*)((u8*)self + 0xcc))->vf_08(buf38);
                func_801D216C(&self->pad_6C[0x60], 1);
                func_801D216C(&self->pad_6C[0x48], 0);
            }
        }
    }
}
#pragma optimize_for_size off

struct CMCCrystalRec5 { unsigned short m0; unsigned short m2; unsigned char m4; };
void copyCrystalRec5_80218FD4(CMCCrystalRec5* dst, const CMCCrystalRec5* src) {
    *dst = *src;
}

struct CMCCrystalRec5Ex {
    unsigned short a;
    unsigned short b;
    unsigned char c;
};

void copyCrystalRec5Ex_80218FF0(CMCCrystalRec5Ex* dst, CMCCrystalRec5Ex* src) {
    unsigned short a = src->a;
    unsigned short b = src->b;
    unsigned char c = src->c;
    dst->a = a;
    dst->b = b;
    dst->c = c;
}

// CMCCrystalBox holds a nw4r::lyt::Layout* at +0x38 (== subObjPtrs[5]) with the
// anim transform to play at +0x3C..+0x5F. These play back a bound animation.
void func_8021900C(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[6]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[6], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_80219094(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[7]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[7], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_8021911C(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[8]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[8], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_802191A4(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[9]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[9], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_8021922C(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[10]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[10], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_802192B4(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[11]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[11], true);
    self->subObjPtrs[11]->SetFrame(lbl_eu_8066845C);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_80219348(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[12]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[12], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

void func_802193D0(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[13]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[13], true);
    self->subObjPtrs[13]->SetFrame(lbl_eu_8066845C);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

__declspec(noinline) void func_80219464(CMCCrystalBox* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->BindAnimation(self->subObjPtrs[14]);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->SetAnimationEnable(self->subObjPtrs[14], true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->subObjPtrs[5])->Animate(0);
}

// Retail 0x8021B344: refresh the window-kind counter labels. When unk2D3 is
// 0 the current counter value formats two numeric panes plus a message;
// when it is 1, three window rows ((value-1)*3 + 0x40..0x42) are formatted,
// the crystal name is resolved through the BDAT table, a rank pane is chosen
// from the id thresholds, and a scale/position pane is sprintf'd with id/30
// and id/20. _savegpr_28 frame is the -O4,s shape (pragma).
#pragma optimize_for_size on
__declspec(noinline) void func_802194EC(CMCCrystalBox* self) {
    if ((s8)self->unk2D3 == 0) {
        int v = func_801392B4(self->unk2D4);
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x1e6,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, (u8)v + 10), 0);
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x1f5,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, (u8)v + 18), 0);
        char* msg = func_80138F78((u16)func_80136254(lbl_eu_80664090, lbl_eu_8050888C + 0x204, (u8)v));
        void* res = (void*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->subObjPtrs[3])
                        ->GetResource(0x74696D67, msg, 0);
        if (res != 0) {
            func_80137E7C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x20f, res);
        }
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x21c,
                      lbl_eu_8050888C + 0x22b, 0);
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x22c,
                      lbl_eu_8050888C + 0x22b, 0);
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x23b,
                      lbl_eu_8050888C + 0x22b, 0);
        void* res2 = (void*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->subObjPtrs[3])
                         ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
        if (res2 != 0) {
            func_80137E7C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x24a, res2);
        }
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x257,
                      lbl_eu_8050888C + 0x22b, 0);
    } else if ((s8)self->unk2D3 == 1) {
        int v = func_801392B4(self->unk2D5);
        int row = ((u8)v - 1) * 3;
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x21c,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, row + 0x40), 0);
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x22c,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, row + 0x41), 0);
        func_80136B4C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x23b,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, row + 0x42), 0);
        char* msg = func_80138F78((u16)func_80136254(lbl_eu_80664090, lbl_eu_8050888C + 0x204, (u8)v));
        void* res = (void*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->subObjPtrs[3])
                        ->GetResource(0x74696D67, msg, 0);
        if (res != 0) {
            func_80137E7C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x24a, res);
        }
        u8 d4 = (u8)func_801392B4(self->unk2D4);
        u8 d5 = (u8)func_801392B4(self->unk2D5);
        u16 id = func_8013A7D0(d4, d5);
        func_80136910((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x257, id);
        // Rank message thresholds (1000/2000/3000/5000/10001).
        const char* s = 0;
        if (id < 0xBB8) {
            if (id < 0x7D0) {
                if (id < 0x3E8) {
                    s = lbl_eu_8050888C + 0x264;
                } else {
                    s = lbl_eu_8050888C + 0x279;
                }
            } else {
                s = lbl_eu_8050888C + 0x28e;
            }
        } else if (id < 0x2711) {
            if (id < 0x1388) {
                s = lbl_eu_8050888C + 0x2a3;
            } else {
                s = lbl_eu_8050888C + 0x2b8;
            }
        }
        void* res2 = (void*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->subObjPtrs[3])
                         ->GetResource(0x74696D67, s, 0);
        if (res2 != 0) {
            func_80137E7C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x2cd, res2);
        }
        // Second rank message set with the complementary thresholds.
        const char* s2 = 0;
        if (id < 0xBB8) {
            if (id < 0x7D0) {
                if (id < 0x3E8) {
                    s2 = lbl_eu_8050888C + 0x2d9;
                } else {
                    s2 = lbl_eu_8050888C + 0x2f1;
                }
            } else {
                s2 = lbl_eu_8050888C + 0x309;
            }
        } else if (id < 0x2711) {
            if (id < 0x1388) {
                s2 = lbl_eu_8050888C + 0x321;
            } else {
                s2 = lbl_eu_8050888C + 0x339;
            }
        }
        void* res3 = (void*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->subObjPtrs[3])
                         ->GetResource(0x74696D67, s2, 0);
        if (res3 != 0) {
            func_80137E7C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x351, res3);
        }
        // Countdown pane: scale id into a screen coordinate, then format
        // id/30 and id/20 into the +0x365 label.
        int val = (int)((double)id * lbl_eu_80668484 + lbl_eu_80668480);
        char buf[0x20];
        u16 v16 = (u16)val;
        sprintf(buf, lbl_eu_8050888C + 0x35e, (u8)(v16 / 30),
                func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 0x30),
                (u8)(v16 / 20));
        func_80136A1C((nw4r::lyt::Layout*)self->subObjPtrs[5], lbl_eu_8050888C + 0x365, buf, 0);
    }
}
#pragma optimize_for_size off

__declspec(noinline) int func_80219AF0(CMCCrystalBox* self){ return 0; }

void copyShortPair_80219D10(void* dst, void* src){
    *(unsigned short*)dst = *(unsigned short*)src;
    *(unsigned short*)((char*)dst + 2) = *(unsigned short*)((char*)src + 2);
}

// Retail 0x8021BB7C: file-load completion callback. The box issues three async
// reads (main crystal-box layout +0x24, crystal-info arc +0x28, BDAT table
// +0x2C); when the event's handle matches one of them, the matching branch
// builds that resource and returns 1, otherwise 0.
// _savegpr_28 frame (retail) is the -O4,s shape (pragma).
#pragma optimize_for_size on
bool CMCCrystalBox::OnFileEvent(CEventFile* event) {
    CFileHandle* evt = event->mFileHandle;
    if ((CFileHandle*)subObjPtrs[0] == evt) {
        // --- main crystal-box layout arc ---
        reinterpret_cast<UnkClass_8045F564*>(&pad_00[0])->createRegion(
            mtl::MemManager::getHandleMEM2(), 0x40000,
            lbl_eu_8050888C + 0x371, 0);
        Class_8045F858 host(reinterpret_cast<UnkClass_8045F564*>(&pad_00[0]));
        char buf[0x20];
        CFileHandle* fh = (CFileHandle*)subObjPtrs[0];
        void* data = fh->getData();
        mtl::MemManager::func_80434A4C(0);
        subObjPtrs[3] = (nw4r::lyt::AnimTransform*)CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3])
            ->Attach(data, lbl_eu_8050888C + 0x37f);
        func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
            (nw4r::lyt::Layout**)&subObjPtrs[5],
            (nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3],
            lbl_eu_8050888C + 0x383);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)subObjPtrs[5], &subObjPtrs[6],
            (nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3],
            lbl_eu_8050888C + 0x39a);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)subObjPtrs[5], &subObjPtrs[7],
            (nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3],
            lbl_eu_8050888C + 0x3b4);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)subObjPtrs[5], &subObjPtrs[8],
            (nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3],
            lbl_eu_8050888C + 0x3d3);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)subObjPtrs[5], &subObjPtrs[9],
            (nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3],
            lbl_eu_8050888C + 0x3f3);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)subObjPtrs[5], &subObjPtrs[10],
            (nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3],
            lbl_eu_8050888C + 0x40e);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)subObjPtrs[5], &subObjPtrs[11],
            (nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3],
            lbl_eu_8050888C + 0x42d);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)subObjPtrs[5], &subObjPtrs[12],
            (nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3],
            lbl_eu_8050888C + 0x44c);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)subObjPtrs[5], &subObjPtrs[13],
            (nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3],
            lbl_eu_8050888C + 0x46c);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)subObjPtrs[5], &subObjPtrs[14],
            (nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3],
            lbl_eu_8050888C + 0x48b);

        // Bind the shared font into the layout's root pane.
        nw4r::lyt::Pane* rootPane = *(nw4r::lyt::Pane**)((u8*)subObjPtrs[5] + 0x10);
        void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
            1, (nw4r::lyt::Layout*)subObjPtrs[5]);
        u32 fontHandle = ((CDeviceFontVtblView*)fontObj)->vf7();
        func_8013676C(rootPane, fontHandle);
        // Row/column count labels from the two shared counters.
        u32 cnt = func_801355A0();
        if (cnt != 0) {
            func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(
                (nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x14d, cnt);
            func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(
                (nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x12e, cnt);
            func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(
                (nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x365, cnt);
        }
        u32 cnt2 = func_801355BC();
        if (cnt2 != 0) {
            for (u8 i = 1; i <= 30; i++) {
                sprintf(buf, lbl_eu_8050888C + 0x102, i);
                func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(
                    (nw4r::lyt::Layout*)subObjPtrs[5], buf, cnt2);
            }
            for (u8 i = 1; i <= 8; i++) {
                sprintf(buf, lbl_eu_8050888C + 0x11e, i);
                func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(
                    (nw4r::lyt::Layout*)subObjPtrs[5], buf, cnt2);
            }
            func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(
                (nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x257, cnt2);
        }

        func_8021900C(this);
        ((CLytVf38*)subObjPtrs[5])->vf_30(0);

        // Static labels for the window-kind counter rows.
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x4a6,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 2), 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x1bd,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 4), 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x4b3,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 3), 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x4bf,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 0x2e), 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x4c8,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 0x2f), 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x4d1,
                      func_80136190(lbl_eu_8050888C + 0xf4, lbl_eu_8050888C + 0xfd, 4), 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x1ca,
                      lbl_eu_8050888C + 0x22b, 0);

        // Window title: pick the message by game-manager flag, resolve the
        // timg texture and publish it onto the pane.
        const char* msgName = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                                  ? lbl_eu_8050888C + 0x4dd
                                  : lbl_eu_8050888C + 0x4e6;
        u16 id = (u16)func_8013606C(lbl_eu_8050888C + 0x4ef, msgName, 0x49);
        void* msg = func_80138F78(id);
        void* res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                        ->GetResource(0x74696D67, (const char*)msg, 0);
        if (res != 0) {
            func_80137E7C((nw4r::lyt::Layout*)subObjPtrs[5],
                          lbl_eu_8050888C + 0x4fd, res);
            // Mirror the timg-resource u16 position header onto the pane. The
            // double deref of res+8 rides as the vf_3C extra arg (r6) and is
            // reused as the base of the two u16 reads (retail schedule).
            u32* d = *(u32**)*(u32**)((u8*)res + 8);
            u16 w2 = *(u16*)((u8*)d + 2);
            u16 w0 = *(u16*)d;
            nw4r::lyt::Pane* pane =
                ((CLytVf3C3*)(*(void**)((u8*)subObjPtrs[5] + 0x10)))
                    ->vf_3C(lbl_eu_8050888C + 0x4fd, 1, d);
            if (pane != 0) {
                f32 fx;
                f32 fy;
                // Dead local pair (8-aligned so MWCC places it at +0x10, the
                // first 8-aligned slot above the two 4-byte RAII hosts); retail
                // interleaves the two stores between the pane writes.
                u64 pos;
                ((PanePosMirror*)pane)->m4C = fx = (f32)w2;
                ((f32*)&pos)[0] = fx;
                ((f32*)&pos)[1] = fy = (f32)w0;
                ((PanePosMirror*)pane)->m50 = fy;
            }
        }

        func_80139198(1);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x257,
                      lbl_eu_8050888C + 0x22b, 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x1e6,
                      lbl_eu_8050888C + 0x22b, 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x1f5,
                      lbl_eu_8050888C + 0x22b, 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x21c,
                      lbl_eu_8050888C + 0x22b, 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x22c,
                      lbl_eu_8050888C + 0x22b, 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x23b,
                      lbl_eu_8050888C + 0x22b, 0);

        int v = func_801392B4(0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x509,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 9), 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x516,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 0xa), 0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x1e6,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1,
                                    (u8)v + 10),
                      0);
        func_80136B4C((nw4r::lyt::Layout*)subObjPtrs[5], lbl_eu_8050888C + 0x1f5,
                      func_80136190(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1,
                                    (u8)v + 0x12),
                      0);

        void* msg2 = func_80138F78(
            (u16)func_80136254(lbl_eu_80664090, lbl_eu_8050888C + 0x204, (u8)v));
        void* res2 = ((nw4r::lyt::ArcResourceAccessor*)subObjPtrs[3])
                         ->GetResource(0x74696D67, (const char*)msg2, 0);
        if (res2 != 0) {
            func_80137E7C((nw4r::lyt::Layout*)subObjPtrs[5],
                          lbl_eu_8050888C + 0x20f, res2);
        }

        // Mirror the window-size pane's translate onto field_14F4.
        nw4r::lyt::Pane* sizePane =
            ((CLytVf3C*)(*(void**)((u8*)subObjPtrs[5] + 0x10)))
                ->vf_3C(lbl_eu_8050888C + 0x523, 1);
        copyVEC3(&field_14F4, (u8*)sizePane + 0x2c);

        // Re-init all six cursor sub-objects from temps (accessor or the
        // shared resource manager), then poke each member's slot-0x08 virtual.
        u8 tmpC0[0x18];
        __ct__CCur07(tmpC0, (void*)subObjPtrs[3]);
        func_8018B0FC((u8*)this + 0x6c, tmpC0);
        __dt__6CCur07Fv(tmpC0, -1);
        ((CCurVf0C*)((u8*)this + 0x6c))->vf_00();

        u8 tmpA8[0x18];
        __ct__CCur09(tmpA8, (void*)subObjPtrs[3]);
        func_8018B0FC((u8*)this + 0x84, tmpA8);
        __dt__6CCur09Fv(tmpA8, -1);
        ((CCurVf0C*)((u8*)this + 0x84))->vf_00();

        // Two direction vectors for the +0x84 cursor's quad.
        float v1[3];
        code80135FDC_setVec3(v1, lbl_eu_80668488, lbl_eu_8066848C,
                             lbl_eu_8066845C);
        u32 t1[3];
        t1[0] = *(u32*)&v1[0];
        t1[1] = *(u32*)&v1[1];
        t1[2] = *(u32*)&v1[2];
        float v2[3];
        code80135FDC_setVec3(v2, lbl_eu_80668490, lbl_eu_8066848C,
                             lbl_eu_8066845C);
        u32 t2[3];
        t2[0] = *(u32*)&v2[0];
        t2[1] = *(u32*)&v2[1];
        t2[2] = *(u32*)&v2[2];
        func_801D24E8((u8*)this + 0x84, t2, t1);

        u8 tmp90[0x18];
        __ct__CCur18(tmp90, func_801355F4());
        func_8018B0FC((u8*)this + 0x9c, tmp90);
        __dt__6CCur18Fv(tmp90, -1);
        ((CCurVf0C*)((u8*)this + 0x9c))->vf_00();

        u8 tmp78[0x18];
        __ct__CCur11(tmp78, (void*)subObjPtrs[3]);
        func_8018B0FC((u8*)this + 0xb4, tmp78);
        __dt__6CCur11Fv(tmp78, -1);
        ((CCurVf0C*)((u8*)this + 0xb4))->vf_00();

        u8 tmp60[0x18];
        __ct__CCur16(tmp60, (void*)subObjPtrs[3]);
        func_8018B0FC((u8*)this + 0xcc, tmp60);
        __dt__6CCur16Fv(tmp60, -1);
        ((CCurVf0C*)((u8*)this + 0xcc))->vf_00();

        u8 tmp48[0x18];
        __ct__CCur18(tmp48, func_801355F4());
        func_8018B0FC((u8*)this + 0xe4, tmp48);
        __dt__6CCur18Fv(tmp48, -1);
        ((CCurVf0C*)((u8*)this + 0xe4))->vf_00();

        func_80216F8C(this);
        subObjPtrs[0] = 0;
        reinterpret_cast<UnkClass_8045F564*>(&pad_00[0])->func_8045F810();
        return true;
    }
    if ((CFileHandle*)subObjPtrs[1] == evt) {
        // --- crystal-info arc ---
        reinterpret_cast<UnkClass_8045F564*>(&pad_00[0x10])->createRegion(
            mtl::MemManager::getHandleMEM2(), 0x100,
            lbl_eu_8050888C + 0x530, 0);
        Class_8045F858 host2(reinterpret_cast<UnkClass_8045F564*>(&pad_00[0x10]));
        CFileHandle* fh2 = (CFileHandle*)subObjPtrs[1];
        void* data2 = fh2->getData();
        mtl::MemManager::func_80434A4C(0);
        subObjPtrs[4] = (nw4r::lyt::AnimTransform*)CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)subObjPtrs[4])
            ->Attach(data2, lbl_eu_8050888C + 0x37f);
        func_80216F8C(this);
        subObjPtrs[1] = 0;
        reinterpret_cast<UnkClass_8045F564*>(&pad_00[0x10])->func_8045F810();
        return true;
    }
    if ((CFileHandle*)subObjPtrs[2] == evt) {
        // --- BDAT table file ---
        CFileHandle* fh3 = (CFileHandle*)subObjPtrs[2];
        void* data3 = fh3->getData();
        func_8003AA34();
        if (getFP__FPCc(lbl_eu_8050888C + 0x541) == 0) {
            func_8003AA78__5CBdatFUlPv(2, data3);
        }
        func_8003AA34();
        lbl_eu_806646D0 = (unsigned long)getFP__FPCc(lbl_eu_8050888C + 0x550);
        func_80216F8C(this);
        subObjPtrs[2] = 0;
        return true;
    }
    return false;
}
#pragma optimize_for_size off

void func_80219D10(CMCCrystalBoxParam* dst, const CMCCrystalBoxParam* src) {
    dst->m0 = (unsigned short)src->m0;
    dst->m2 = (unsigned short)src->m2;
}
