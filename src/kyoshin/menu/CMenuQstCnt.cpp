// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQstCnt
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include <nw4r/lyt.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTypes.h>

#include "kyoshin/menu/CMenuQstCnt.hpp"

void QstEntry::setQstEntry(short a, short b, unsigned char c, unsigned char d, unsigned char e) {
    f0 = a;
    f2 = b;
    f4 = c;
    f5 = d;
    f6 = e;
}

struct QstMenuData {
    u8 mPad88[0x88];                            // 0x00
    nw4r::lyt::AnimTransform* mAnim88;          // 0x88
    u32 mState8C;                               // 0x8C
    f32 mValue90;                               // 0x90
};

extern void func_80138078(u32);

// The quest-info buffer func_80226FAC / func_802270CC maintain.
struct QstData {
    QstInfo mList[0x400];                       // 0x0000
    u16 field_2000;                             // 0x2000
    s16 field_2002;                             // 0x2002
    u8  field_2004;                             // 0x2004
    u8  field_2005;                             // 0x2005
};

// forward declarations for cross-referenced helpers (defined later in this TU)
QstEntry* copyQstEntry(QstEntry* dst, const QstEntry* src);
QstEntry* copyQstEntry2(QstEntry* dest, const QstEntry* src);
void func_80226BBC(QstMenuData* self);
void func_80226C18(QstMenuData* self);
void func_80226C5C(QstMenuData* self);
void func_80226C88(CMenuQstCnt* self);
void func_80226E54(CMenuQstCnt* self);
QstData* func_80226FAC(QstData* self);
void func_802270CC(QstData* self);

// --- OOL base/subobject + helper/data imports are declared in CMenuQstCnt.hpp ---

CMenuQstCnt* lbl_eu_80664720;

// Constructor (unmangled retail symbol). Runs CProcess's ctor then fills the
// IUIWindow base region by hand (the IUIWindow base ctor is out-of-line in
// retail, so its effects are emitted inline at the head of this ctor).
void __ct__CMenuQstCnt(CMenuQstCnt* self, CScn* scene, short a, short b, unsigned char c, unsigned char d, unsigned char e) {
    __ct__8CProcessFv(self);

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052D238;
    u32* ptmf = __ptmf_null;
    *(u32*)((u8*)self + 0x3C) = ptmf[0];
    *(u32*)((u8*)self + 0x40) = ptmf[1];
    *(u32*)((u8*)self + 0x44) = ptmf[2];
    *(u32*)((u8*)self + 0x48) = ptmf[0];
    *(u32*)((u8*)self + 0x4C) = ptmf[1];
    *(u32*)((u8*)self + 0x50) = ptmf[2];
    self->mLayout = 0;
    self->mField58 = 0;
    self->mField5C = 0;
    self->mField60 = -1;
    self->mField64 = 0;
    self->mField65 = 0;
    self->mField66 = 0;
    self->mField67 = 1;
    self->mField68 = 0;

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_80536128;
    self->mOcc6C = (u32)lbl_eu_80536128 + 0x24;
    self->mOcc70 = (u32)lbl_eu_80536128 + 0xAC;
    self->mScene = scene;

    __ct__17UnkClass_8045F564Fv(&self->mMemRegion[0]);

    self->mAnim88 = 0;
    self->mState8C = 0;
    self->mValue90 = 0.0f;

    for (int i = 0; i < 32; i++) {
        self->mEntries[i].setQstEntry(0, 0, 0, 0, 0);
    }
    self->mSelEntry.setQstEntry(0, 0, 0, 0, 0);
    self->mField19C = 0;

    QstEntry tmp;
    for (int i = 0; i < 32; i++) {
        tmp.setQstEntry(0, 0, 0, 0, 0);
        copyQstEntry(&self->mEntries[i], &tmp);
    }
    tmp.setQstEntry(a, b, c, d, e);
    copyQstEntry(&self->mSelEntry, &tmp);
}

QstEntry* copyQstEntry(QstEntry* dst, const QstEntry* src) {
    dst->f0 = src->f0;
    dst->f2 = src->f2;
    dst->f4 = src->f4;
    dst->f5 = src->f5;
    dst->f6 = src->f6;
    return dst;
}

QstEntry* copyQstEntry2(QstEntry* dest, const QstEntry* src) {
    dest->f0 = src->f0;
    dest->f2 = src->f2;
    dest->f4 = src->f4;
    dest->f5 = src->f5;
    dest->f6 = src->f6;
    return dest;
}

extern "C" void __dt__11CMenuQstCntFv(CMenuQstCnt* self, int flags) {
    if (self == 0) return;
    __dt__17UnkClass_8045F564Fv(&self->mMemRegion[0], -1);
    __dt__9IUIWindowFv(self, 0);
    if (flags > 0) __dl__FPv(self);
}

// C++-linkage retail symbol (func_801355A0__Fv): returns a u32 message/flag
// value used to fill the text fields below.
u32 func_801355A0();

void CMenuQstCnt::Init() {
    // Set up the scoped MEM2 region guard, then build the layout/animations.
    reinterpret_cast<UnkClass_8045F564*>(mMemRegion)
        ->createRegion((int)mtl::MemManager::getHandleMEM2(), 0x4000,
                       (const char*)lbl_eu_80509A10, 0);
    Class_8045F858 regionGuard(
        reinterpret_cast<UnkClass_8045F564*>(mMemRegion));

    func_80136E84(&mLayout, CUICfManager::func_801355F4(),
                  &lbl_eu_80509A10[0xc]);
    func_80136F08(mLayout, &mAnim88, CUICfManager::func_801355F4(),
                  (char*)&lbl_eu_80509A10[0x25]);

    // Bind the font and hand the loaded font object over to the root pane.
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    void* fontObj = CDeviceFont::func_80452C10(1, mLayout);
    u32 fontResult =
        ((u32 (*)(void*))(((void**)fontObj)[0x24 / 4]))(fontObj);
    func_8013676C(rootPane, fontResult);

    // First three text fields are filled straight from the message table.
    func_801368C0(mLayout, &lbl_eu_80509A10[0x41], func_801355A0());
    func_801368C0(mLayout, &lbl_eu_80509A10[0x4d], func_801355A0());
    func_801368C0(mLayout, &lbl_eu_80509A10[0x59], func_801355A0());

    func_80136B4C(mLayout, &lbl_eu_80509A10[0x64], &lbl_eu_80509A10[0x73], 0);
    func_80136B4C(mLayout, &lbl_eu_80509A10[0x74], &lbl_eu_80509A10[0x73], 0);

    // Helper returns formatted text for the two quest-log fields (the first,
    // 12-entry form, is computed and discarded); the remaining field uses it.
    func_80136190(&lbl_eu_80509A10[0x7b], &lbl_eu_80509A10[0x85], 12);
    func_80136B4C(mLayout, &lbl_eu_80509A10[0x8a], &lbl_eu_80509A10[0x73], 0);

    char* text =
        (char*)func_80136190(&lbl_eu_80509A10[0x7b], &lbl_eu_80509A10[0x85], 11);
    func_80136B4C(mLayout, &lbl_eu_80509A10[0x59], text, 0);

    // Start the open animation from frame 0 and tick it once.
    mLayout->SetAnimationEnable(mAnim88, true);
    mAnim88->SetFrame(lbl_eu_80668568);
    mLayout->Animate(0);

    mField67 = 0;
    func_80226E54(this);

    mScene->addRenderCB(reinterpret_cast<IScnRender*>(&mOcc70), 0xa, 0);

    reinterpret_cast<UnkClass_8045F564*>(mMemRegion)->func_8045F810();
    mField19C = 1;
}

void CMenuQstCnt::Term() {
    waitForDrawDone__9CDeviceVIFv();
    void* renderPtr = this ? (u8*)this + 0x70 : 0;
    removeRenderCB__4CScnFP10IScnRender(mScene, renderPtr);
    if (mLayout) {
        void* vtab = *(void**)mLayout;
        ((void (*)(void*, bool))((void**)vtab)[0x08 / 4])(mLayout, 1);
        mLayout = 0;
    }
    func_8045F778__17UnkClass_8045F564Fv(&mMemRegion[0]);
    lbl_eu_80664720 = 0;
}

void CMenuQstCnt::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    if (lbl_eu_80663E28 & (1u << 21)) {
        goto done;
    }
    if (!code80135FDC_getByte_621F0()) {
        goto done;
    }
    if (!(lbl_eu_80663E24 & (1u << 24)) && (lbl_eu_80663E24 & 0xAFE40000u)) {
        goto done;
    }
    if (cf::CfGameManager::func_800829B8()) {
        goto done;
    }
    if (lbl_eu_80663E24 & (1u << 9)) {
        goto done;
    }
    {
        void* cam = func_8049603C((void*)lbl_eu_80663E14);
        if (lbl_eu_8066856C - *(f32*)((u8*)cam + 0xC) < lbl_eu_8066856C) {
            goto done;
        }
    }
    if (func_8011CD5C()) {
        goto done;
    }
    if (func_80293C10()) {
        goto done;
    }
    if (func_8029A658()) {
        goto done;
    }
    if (func_801B481C()) {
        goto done;
    }
    if (func_80124B78()) {
        goto done;
    }
    switch (mState8C) {
    case 0: func_80226BBC((QstMenuData*)this); break;
    case 1: func_80226C18((QstMenuData*)this); break;
    case 2: func_80226C5C((QstMenuData*)this); break;
    case 3: func_80226C88(this); break;
    }
    {
        void* vtab = *(void**)mLayout;
        ((void (*)(void*, bool))((void**)vtab)[0x38 / 4])(mLayout, 0);
    }
done:
    ;
}

void CMenuQstCnt::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    if (lbl_eu_80663E28 & (1u << 21)) {
        goto done;
    }
    if (!code80135FDC_getByte_621F0()) {
        goto done;
    }
    if (!(lbl_eu_80663E24 & (1u << 24)) && (lbl_eu_80663E24 & 0xAFE40000u)) {
        goto done;
    }
    if (cf::CfGameManager::func_800829B8()) {
        goto done;
    }
    {
        void* cam = func_8049603C((void*)lbl_eu_80663E14);
        if (lbl_eu_8066856C - *(f32*)((u8*)cam + 0xC) < lbl_eu_8066856C) {
            goto done;
        }
    }
    if (func_80293C10()) {
        goto done;
    }
    if (func_8029A658()) {
        goto done;
    }
    if (func_801B481C()) {
        goto done;
    }
    if (func_80124B78()) {
        goto done;
    }
    if (mField64 != 0) {
        goto done;
    }
    {
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        func_80137038(mLayout, &drawInfo, 0, 1);
        __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo, -1);
    }
done:
    ;
}

CMenuQstCnt* func_802269D8(CProcess* parent, CScn* scene, short a, short b, unsigned char c, unsigned char d, unsigned char e) {
    CMenuQstCnt* g = lbl_eu_80664720;
    if (g) {
        if (g->mField64 == 0) {
            int changed = (g->mSelEntry.f0 != (u16)a) || (g->mSelEntry.f2 != (u16)b);
            if (!changed) {
                switch (g->mState8C) {
                case 0:
                    g->mSelEntry.f4 = c;
                    if (g->mField19C) func_80226E54(g);
                    break;
                case 1:
                    g->mSelEntry.f4 = c;
                    func_80226E54(g);
                    break;
                case 2:
                    g->mSelEntry.f4 = c;
                    g->mValue90 = 0.0f;
                    func_80226E54(g);
                    break;
                default:
                    changed = 1;
                    break;
                }
            }
            if (changed) {
                for (int i = 0; i < 0x20; i++) {
                    if (g->mEntries[i].f0 == 0) {
                        QstEntry tmp;
                        tmp.setQstEntry(a, b, c, d, e);
                        copyQstEntry(&g->mEntries[i], &tmp);
                        break;
                    }
                }
            }
        }
        return 0;
    }
    u32 mem = getWorkMem__17CWorkThreadSystemFv();
    CMenuQstCnt* obj = (CMenuQstCnt*)allocate__Q23mtl10MemManagerFUlUl(0x1A0, mem);
    if (obj) {
        __ct__CMenuQstCnt(obj, scene, a, b, c, d, e);
    }
    lbl_eu_80664720 = obj;
    Regist__8CProcessFP8CProcessb(obj, parent, 0);
    return lbl_eu_80664720;
}

int func_80226B94() {
    // subic/subfe is-zero idiom: returns whether the global menu object is unallocated
    return lbl_eu_80664720 == 0;
}

void invalidateQstFlag() {
    if (lbl_eu_80664720 != 0) {
        lbl_eu_80664720->mField64 = 1;
    }
}

// Quest-log gating: when the CF-game resource gate reports the scene is open
// (not mid-load) and the input state allows it, play the sound and set state 1.
void func_80226BBC(QstMenuData* self) {
    if (func_80144FF0() == 0 || cf::CfGameManager::func_800829B8() || func_80145030()) {
        func_80138078(0x1f);
        self->mState8C = 1;
    }
}

// Advance the open/close animation at 0x88 by one frame; when it finishes the
// animation (func_80137444 returns 1), move to state 2.
void func_80226C18(QstMenuData* self) {
    if (func_80137444(self->mAnim88, 1.0f) != 0) {
        self->mState8C = 2;
    }
}

// Per-frame timer: 0x90 counts frames (1 per call); once it reaches 90 frames
// it clamps to state 3. Uses >= (cror eq,gt,eq; bnelr returns while below).
void func_80226C5C(QstMenuData* self) {
    self->mValue90 += 1.0f;
    if (self->mValue90 >= 90.0f) {
        self->mState8C = 3;
    }
}

void func_80226C88(CMenuQstCnt* self) {
    if (func_80137510(self->mAnim88, lbl_eu_8066856C) == 0) return;
    if (self->mEntries[0].f0 == 0) {
        self->mField64 = 1;
        return;
    }
    self->mValue90 = 0.0f;
    self->mState8C = 0;
    QstEntry cur;
    copyQstEntry(&self->mSelEntry, copyQstEntry2(&cur, &self->mEntries[0]));
    QstEntry tmp0;
    tmp0.setQstEntry(0, 0, 0, 0, 0);
    copyQstEntry(&self->mEntries[0], &tmp0);
    func_80226E54(self);

    QstEntry scratch[32];
    for (int i = 0; i < 32; i++) scratch[i].setQstEntry(0, 0, 0, 0, 0);
    for (int i = 0; i < 32; i++) {
        QstEntry t;
        t.setQstEntry(0, 0, 0, 0, 0);
        copyQstEntry(&scratch[i], &t);
    }
    int j = 0;
    for (int i = 0; i < 32; i++) {
        if (self->mEntries[i].f0 != 0) {
            QstEntry t;
            copyQstEntry2(&t, &self->mEntries[i]);
            copyQstEntry(&scratch[j], &t);
            j++;
        }
    }
    for (j = 0; j < 32; j++) {
        QstEntry t;
        copyQstEntry2(&t, &scratch[j]);
        copyQstEntry(&self->mEntries[j], &t);
    }
}

void func_80226E54(CMenuQstCnt* self) {
    if (self->mSelEntry.f0 == 0) return;
    if (self->mSelEntry.f4 > self->mSelEntry.f5) {
        self->mSelEntry.f4 = self->mSelEntry.f5;
    }
    u32 v = func_80138138(self->mSelEntry.f0);
    char* s = func_8013639C((void*)lbl_eu_80573D18[v], &lbl_eu_80509A10[0x92], self->mSelEntry.f0);
    func_80136B4C(self->mLayout, &lbl_eu_80509A10[0x64], s, 0);
    if (self->mSelEntry.f6 == 0) {
        s = func_8013639C(lbl_eu_80664094, &lbl_eu_80509A10[0x85], self->mSelEntry.f2);
    } else {
        s = func_801394D4(self->mSelEntry.f2);
    }
    func_80136B4C(self->mLayout, &lbl_eu_80509A10[0x74], s, 0);
    if (self->mSelEntry.f6 == 0) {
        s = (char*)func_80136190(&lbl_eu_80509A10[0x7b], &lbl_eu_80509A10[0x85], 12);
    } else {
        s = (char*)func_80136190(&lbl_eu_80509A10[0x7b], &lbl_eu_80509A10[0x85], 13);
    }
    func_80136B4C(self->mLayout, &lbl_eu_80509A10[0x8a], s, 0);
    func_80136910(self->mLayout, &lbl_eu_80509A10[0x41], self->mSelEntry.f4);
    func_80136910(self->mLayout, &lbl_eu_80509A10[0x4d], self->mSelEntry.f5);
}

QstData* func_80226FAC(QstData* self) {
    __construct_array(self, (void*)initQstInfo, (void*)__dt__80227030, 8, 0x400);
    self->field_2000 = 0;
    self->field_2002 = 0;
    self->field_2004 = 0xff;
    self->field_2005 = 0xff;
    return self;
}

extern "C" void initQstInfo(QstInfo* ptr) {
    ptr->f0 = 0;
    ptr->f2 = 0;
    ptr->f3 = 0;
    ptr->f4 = 0;
    ptr->f5 = 0;
    ptr->f6 = 0;
    ptr->f7 = 0;
}

extern "C" void __dt__80227030(){}

extern "C" void __dt__80227070(){}

void func_802270CC(QstData* self) {
    self->field_2000 = 0;
    for (int cat = 1; cat < 0x1B; cat++) {
        u32 start = func_801380A0(cat);
        if ((start & 0xffff) == 0) continue;
        u32 end = func_801380A0(cat + 1);
        for (u32 i = start; i < end; i++) {
            u8 kind = (u8)func_8009CF8C(i + 0x220);
            if (kind == 0 || kind == 0xC8) continue;
            int r22;
            if (kind >= 0xFE) {
                r22 = 3;
            } else if (kind == 1) {
                r22 = 1;
            } else {
                r22 = 2;
            }
            u32 v = func_80138138(i);
            void* tbl = (void*)lbl_eu_80573D18[v];
            u32 res = func_801361E8((u32)tbl, &lbl_eu_80509AB4[0], i);
            if ((res & 0xff) == 2) continue;
            u32 r21 = (res & 0xff) == 1;
            if (r22 == 3) r21 = 0;
            u8 x = (u8)func_801361E8((u32)lbl_eu_806640A8, &lbl_eu_80509AB4[0xa], cat);
            u16 idx = self->field_2000;
            self->field_2000 = idx + 1;
            QstInfo src;
            src.f0 = (u16)i;
            src.f2 = r22;
            src.f3 = cat;
            src.f4 = kind;
            src.f5 = r21;
            src.f6 = (u8)res;
            src.f7 = x;
            copyQstInfo(&self->mList[idx], &src);
        }
    }
}

void func_80227260(QstData* self, u32 kind, u32 filter) {
    // Select/re-select the sort column. kind==4 uses the quest-level filter
    // (field_2005); otherwise kind is the column (field_2004). If the selection
    // didn't change, there is nothing to re-sort.
    if (kind == 4) {
        if (self->field_2005 == filter) return;
        self->field_2004 = kind;
        self->field_2005 = filter;
    } else {
        if (self->field_2004 == kind) return;
        self->field_2004 = kind;
        self->field_2005 = 0;
    }
    self->field_2002 = -1;   // "unsorted" marker until the count is recomputed

    // Pass 1: bubble sort quest entries by quest id (f0) descending.
    for (u16 i = 0; i < self->field_2000 - 1; i++) {
        bool changed = true;
        QstInfo t1, t2, t3;
        for (u16 j = 0; j < self->field_2000 - 1 - i; j++) {
            QstInfo* pa = &self->mList[j];
            QstInfo* pb = &self->mList[j + 1];
            if (pa->f0 > pb->f0) {
                func_80227660(&t1, pa);
                func_80227660(&t2, pb);
                copyQstInfo(pa, &t2);
                func_8022769C(&t3, &t1);
                copyQstInfo(pb, &t3);
                changed = false;
            }
        }
        if (changed) break;
    }

    // Pass 2: bubble sort by flag byte f7 descending.
    for (u16 i = 0; i < self->field_2000 - 1; i++) {
        bool changed = true;
        QstInfo t1, t2, t3;
        for (u16 j = 0; j < self->field_2000 - 1 - i; j++) {
            QstInfo* pa = &self->mList[j];
            QstInfo* pb = &self->mList[j + 1];
            if (pa->f7 > pb->f7) {
                func_80227660(&t1, pa);
                func_80227660(&t2, pb);
                copyQstInfo(pa, &t2);
                func_8022769C(&t3, &t1);
                copyQstInfo(pb, &t3);
                changed = false;
            }
        }
        if (changed) break;
    }

    // Pass 3: bubble sort by f5 ascending.
    for (u16 i = 0; i < self->field_2000 - 1; i++) {
        bool changed = true;
        QstInfo t1, t2, t3;
        for (u16 j = 0; j < self->field_2000 - 1 - i; j++) {
            QstInfo* pa = &self->mList[j];
            QstInfo* pb = &self->mList[j + 1];
            if (pa->f5 < pb->f5) {
                func_80227660(&t1, pa);
                func_80227660(&t2, pb);
                copyQstInfo(pa, &t2);
                func_8022769C(&t3, &t1);
                copyQstInfo(pb, &t3);
                changed = false;
            }
        }
        if (changed) break;
    }

    // When a column filter is active, bubble the matching entries to the front
    // (sort by match flag ascending, so matches land first), then count them.
    if (kind != 0) {
        for (u16 i = 0; i < self->field_2000 - 1; i++) {
            bool changed = true;
            QstInfo t1, t2, t3;
            for (u16 j = 0; j < self->field_2000 - 1 - i; j++) {
                QstInfo* pa = &self->mList[j];
                QstInfo* pb = &self->mList[j + 1];
                u8 m1 = (kind == 4) ? (pa->f3 == filter)
                                    : (pa->f2 == kind);
                u8 m2 = (kind == 4) ? (pb->f3 == filter)
                                    : (pb->f2 == kind);
                if (m1 < m2) {
                    func_80227660(&t1, pa);
                    func_80227660(&t2, pb);
                    copyQstInfo(pa, &t2);
                    func_8022769C(&t3, &t1);
                    copyQstInfo(pb, &t3);
                    changed = false;
                }
            }
            if (changed) break;
        }

        self->field_2002 = 0;
        for (u16 k = 0; k < self->field_2000; k++) {
            u8 m = (kind == 4) ? (self->mList[k].f3 == filter)
                               : (self->mList[k].f2 == kind);
            if (m == 0) break;
            self->field_2002++;
        }
    }
}


unsigned short selectQstIndex(unsigned char* p) {
    QstData* data = reinterpret_cast<QstData*>(p);
    short r = data->field_2002;
    // field_2002 (signed) is negative -> use the u16 count instead. Keep the
    // temp as short so the single u16 conversion/clrlwi happens at the join.
    if (r < 0)
        r = (short)data->field_2000;
    return (unsigned short)r;
}

QstInfo* func_802276F4(QstInfo* base, unsigned short idx) {
    // clrlslwi 16,3 = (idx & 0xFFFF) << 3 -> pointer into an 8-byte QstInfo array
    if (idx >= 0x400) return 0;
    return base + idx;
}

void func_80227710(){}

// --- IScnRender vtable this-adjusting thunks ---
// The IScnRender subobject sits at +0x70 within CMenuQstCnt; vtable dispatch
// through that subobject passes 'this' pointing at +0x70, so each thunk
// subtracts it back and tail-calls the real implementation.
// Retail: subi r3, r3, 0x70; b cbRenderBefore__11CMenuQstCntFv
void renderQstBase_70(CMenuQstCnt* sub) {
    reinterpret_cast<CMenuQstCnt*>(reinterpret_cast<char*>(sub) - 0x70)->cbRenderBefore();
}

// Retail: subi r3, r3, 0x6c; b __dt__11CMenuQstCntFv
void dtorQstBase_6C(u8* sub, int flags) {
    __dt__11CMenuQstCntFv((CMenuQstCnt*)(sub - 0x6C), flags);
}

// Retail: subi r3, r3, 0x70; b __dt__11CMenuQstCntFv
void dtorQstBase_70(u8* sub, int flags) {
    __dt__11CMenuQstCntFv((CMenuQstCnt*)(sub - 0x70), flags);
}