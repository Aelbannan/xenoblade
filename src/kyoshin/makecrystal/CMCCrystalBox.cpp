// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalBox
// Replace stubs with high-level C/C++ during decomp.

// Inline-empty ~IWorkEvent: the retail destructor elides the base call.
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CScrollBar.hpp"

#include "kyoshin/makecrystal/CMCCrystalBox.hpp"

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"

#include <string.h>
#include <nw4r/lyt.h>
void bindLayoutAnimTransform(nw4r::lyt::Layout* layout, nw4r::lyt::AnimTransform** anim, nw4r::lyt::ArcResourceAccessor* accessor, char* name);
void buildLayout(nw4r::lyt::Layout** layout, nw4r::lyt::ArcResourceAccessor* accessor, const char* name);
#include "kyoshin/plugin/ocBdat.hpp"

// Retail symbols: advanceAnimTransform keeps the mangled C++ name, but AnimRewindFrame
// is the unmangled C name (per retail relocs).
u32 advanceAnimTransform(nw4r::lyt::AnimTransform*, float);

void playUISound(u32);

void setLayoutTextBoxNumber(nw4r::lyt::Layout*, char*, u8);

// Layout-anim helpers: retail emits out-of-line `bl` calls to these from the
// anim-completion callbacks; MWCC -O4,p would otherwise inline them into the
// small callers. C linkage carries over from the header declarations.

// Refresh/name-label helpers: retail calls these out-of-line from the update
// callbacks (tickCrystalBoxPageFwd/98, confirmCrystalSelect); keep them non-inlined so the
// callers emit `bl` like retail instead of cascading virtual-call bodies.
__declspec(noinline) char* getCrystalItemName(CMCCrystalData* d, int v);

// More refresh/state helpers called out-of-line (bl) by the update callbacks
// and aggregation paths; without these MWCC -O4,p cascades the callee bodies
// into the callers.
__declspec(noinline) unsigned long getSelectTableWord(unsigned long* table, unsigned int idx);
__declspec(noinline) CMCCrystalBoxParam* copyCrystalParamRet(CMCCrystalBoxParam* dest, const CMCCrystalBoxParam* src);
__declspec(noinline) void sortCrystalIdOrder(CMCCrystalData* d);
__declspec(noinline) void sortCrystalKeyOrder(CMCCrystalData* d);

// Crystal-box param helpers: retail calls these out-of-line (bl) from the
// init/refresh loops; keep them non-inlined so the callers emit bl like
// retail instead of cascading the store/copy bodies into the loops.
__declspec(noinline) CMCCrystalBoxParam* setCrystalBoxParam(CMCCrystalBoxParam*, s16, s16);
__declspec(noinline) void copyCrystalBoxParam(CMCCrystalBoxParam*, const CMCCrystalBoxParam*);

// Retail 0x80219E70: refresh the crystal box display / name.
// -O4,s: retail uses the stmw r30/lmw r30 frame; plain -O4,p
// splits the saves into separate stw's (MWCC_CASES kyoshin-dtor pattern).
__declspec(noinline) void CMCCrystalBox::refreshCrystalBoxName() {
    CMCCrystalData* d = &this->data;
    if (this->field_14A0 == 0) {
        MakeCrystal_CollectByTarget(d, this->field_14EC[(s8)this->field_14F2]);
    }
    setLayoutTextBoxNumber((nw4r::lyt::Layout*)this->mLayout,
                  lbl_eu_8050888C + 0x12e, d->limit != 0 ? d->limit : 1);
    updateCrystalBoxRows();
    setCrystalNameLabel();
    rebuildCrystalSortMenu();
    func_8021A8EC(&this->crystalInfo);
}

// Retail 0x8021A2B8: set the current crystal's name label and mirrored info.
// stmw r28 frame + raw-byte idx multiply are the -O4,s shapes (unit flag).
__declspec(noinline) void CMCCrystalBox::setCrystalNameLabel() {
    // Declared first so MWCC claims r31 for q (first-declared -> highest
    // saved register), matching retail's {d:r29, idx:r30, q:r31}.
    void* q;
    u16 p;
    u8 idx = this->unk2CC + (u8)this->unk2CD * 10;
    CMCCrystalData* d = &this->data;
    char* name = getCrystalItemName(d, idx);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout,
                  lbl_eu_8050888C + 0x18e, name, 0);
    if (func_8021A8EC(&this->crystalInfo) != 0) {
        if ((s8)this->unk2CD < 3) {
            // Retail evaluates MakeCrystal_GetItemObject before MakeCrystal_GetSubCategory and keeps
            // the object pointer result in a saved register across the second
            // call.
            q = MakeCrystal_GetItemObject(d, idx);
            p = MakeCrystal_GetSubCategory(d, idx);
            MCCrystal_ShowItem(&this->crystalInfo, p, q);
        } else {
            MCCrystal_ShowItem(&this->crystalInfo, 0, 0);
        }
    }
}

// Retail 0x80215710: resolve a crystal item by adjusted index, get its name
// from a freshly-created item instance, and format it into the name buffer.
// Single trailing `return 0` (nested ifs) so MWCC merges all failure paths
// into one epilogue `li r3, 0` like retail.
char* getCrystalItemName(CMCCrystalData* d, int v) {
    int cur = d->current;
    u16 adj = (u16)(v + cur * 30);          // row-scaled index
    if ((u32)adj < d->count) {
        void* obj = CItem_kindPtr(d->byte_1002, d->entries[adj].id);
        if (obj != 0 && *(void**)obj != 0) {
            void* inst = CItem_initItemImplInstances(obj);
            char* name = ((CItemImplDispatch*)inst)->getName(obj);
            sprintf(d->name, lbl_eu_8050888C, name);
            return d->name;
        }
    }
    return 0;
}

// Swap the 4-byte payload (s16 + u8) of two crystal-data entries.
// noinline: retail calls this out-of-line (bl) from the bubble sort; without
// it MWCC inlines the body and the swapCrystalDataEntry reloc disappears.
__declspec(noinline) void swapCrystalDataEntry(int unused, void* a, void* b) {
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

// TU-local head view of a CItemExt record (see cf/CItem.hpp): packed word at
// +0x00, category flag byte at +0x07. This TU sees only the stale void*
// CItem_kindPtr decl, so the view names the two compared fields (same bytes).
struct CrystalItemHead { u32 word00; u16 key04; u8 _pad06; u8 flag07; };

// Retail 0x802157E8: bubble-sort the crystal-state entries.
void sortCrystalIdOrder(CMCCrystalData* d) {
    // Declaration order steers MWCC's saved-register homes toward the retail
    // map {i:r31, swapped:r30, j:r29, pa:r28, d:r27, tmp:r26, a:r25,
    // pb:r24, o1:r23, o2:r22, b:r21}.
    u16 i;
    int swapped;
    u16 j;
    CMCCrystalDataEntry* pa;
    u32 a;
    CMCCrystalDataEntry* pb;
    void* o1;
    void* o2;
    u32 b;

    for (i = 0; i < d->count - 1; i++) {
        swapped = 0;
        for (j = 0; j < d->count - 1 - i; j++) {
            pa = &d->entries[j];
            pb = &d->entries[j + 1];
            o1 = CItem_kindPtr(9, pa->id);
            o2 = CItem_kindPtr(9, pb->id);
            // Retail loads *o2 before *o1 and keeps (*o2)>>20 live across
            // the first category call; a's home doubles as that temp.
            a = ((const CrystalItemHead*)o2)->word00 >> 20;
            BdatGetItemId(((const CrystalItemHead*)o1)->word00 >> 20);
            BdatGetItemId(a);
            a = IsSkillItem(o1);
            b = IsSkillItem(o2);
            // A non-zero boxed getter zeroes that side's key.
            if (((CItemImplFacade3*)CItem_initItemImplInstances(o1))->GetBoxed(o1) != 0) {
                a = 0;
            }
            if (((CItemImplFacade3*)CItem_initItemImplInstances(o2))->GetBoxed(o2) != 0) {
                b = 0;
            }
            if (a != 0 || b != 0) {
                if (a > b) {
                    swapCrystalDataEntry((int)d, pa, pb);
                    swapped = 1;
                }
            } else {
                // Both keys zero: fall back to the entry flag byte's bits
                // 2..7 (shift+mask folds to one rlwinm).
                                if ((u32)((((const CrystalItemHead*)o1)->flag07 >> 2) & 0x3F) > (u32)((((const CrystalItemHead*)o2)->flag07 >> 2) & 0x3F)) {
                    swapCrystalDataEntry((int)d, pa, pb);
                    swapped = 1;
                }
            }
        }
        if (swapped == 0) break;
    }
}

// Retail 0x80215974: bubble-sort the crystal-state entries by their %+4
// key using CItem_kindPtr until no swap occurs in a pass.
void sortCrystalKeyOrder(CMCCrystalData* d) {
    u16 o = 0;
    while (o < d->count - 1) {
        int swapped = 0;
        u16 i = 0;
        while (i < (d->count - 1) - o) {
            CMCCrystalDataEntry* pa = &d->entries[i];
            CMCCrystalDataEntry* pb = &d->entries[i + 1];
            // Retail keeps obj1 in a saved register across the second call
            // and loads both %+4 keys only after both resolves.
            void* obj1 = CItem_kindPtr(9, pa->id);
            void* obj2 = CItem_kindPtr(9, pb->id);
            u32 av = ((const CrystalItemHead*)obj1)->key04;
            u32 bv = ((const CrystalItemHead*)obj2)->key04;
            if (av > bv) {
                swapCrystalDataEntry((int)d, pa, pb);
                swapped = 1;
            }
            i++;
        }
        if (swapped == 0) break;
        o++;
    }
}

// Retail 0x80215A48: crystal-box constructor. Members are constructed in
// declaration order (pointer table, cursors, sort menu, crystal info, scroll
// bar, system window), then the selection/page state is cleared and the
// 5-byte enable table memset. stmw r28 frame is the -O4,s shape (unit flag).
CMCCrystalBox::CMCCrystalBox(u8 parentType)
    : mFileHandle0(0), mFileHandle1(0), mFileHandle2(0), mArcAcc0(0), mArcAcc1(0),
      mLayout(0), mAnim0(0), mAnim1(0), mAnim2(0), mAnim3(0), mAnim4(0), mAnim5(0),
      mAnim6(0), mAnim7(0), mAnim8(0), unk60(0), unk64(0), unk68(0), unk69(1),
      mScrollBar(0), sysWin(0), unk2CC(0), unk2CD(0), unk2CE(0), unk2CF(0),
      unk2D0(parentType), unk2D1(0), unk2D2(1), unk2D3(0), unk2D4(0), unk2D5(0) {
    initCrystalBoxParamTbl(&unk2D6);
    MakeCrystal_ResetTable(&data);
    initCrystalBoxScratch(reinterpret_cast<CrystalBoxScratch*>((u8*)this + 0x1480));
    field_14F1 = 0;
    field_14F2 = 0;
    unk1500 = 0;
    unk1501 = 0;
    unk1502 = 0;
    unk1503 = 0;
    unk1504 = 0;
    field_1506 = 0;
    field_1507 = 0;
    memset(&field_14EC[0], 0, 5);
}

// Retail 0x80215C5C: set all fields of a crystal-box param (selected id,
// quantity, spare, two flags) and return the param pointer.
CMCCrystalBoxParam* setCrystalBoxParam(CMCCrystalBoxParam* p, s16 a, s16 b) {
    p->m0 = a;
    p->m2 = b;
    p->m4 = 0;
    p->m6 = 0;
    p->m7 = 0;
    return p;
}

// Retail 0x80215C78: copy an 8-byte crystal-box param (three s16, two u8).
void copyCrystalBoxParam(CMCCrystalBoxParam *dst, const CMCCrystalBoxParam *src) {
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
// walk), then re-init every slot through a stack-temp copy so each
// entry is written by copyCrystalBoxParam (matching retail's two-loop shape).
// noinline: retail calls this out-of-line (bl) from rebuildCrystalBoxSelTable; without
// it MWCC inlines the two-loop body into the caller.
// stmw r29 frame (end:r31, p:r30, base:r29, i reused in r30, tmp at r1+8)
// is the -O4,s shape.
__declspec(noinline) u8* initCrystalBoxParamTbl(u8* base) {
    // end declared first (r31) but initialized after p, matching retail's
    // addi r30 / addi r31 emission order.
    CMCCrystalBoxParam* end;
    CMCCrystalBoxParam* p = (CMCCrystalBoxParam*)(base + 2);
    end = (CMCCrystalBoxParam*)(base + 0x102);
    do {
        setCrystalBoxParam(p, 0, 0);
        p++;
    } while (p < end);

    base[0] = 0;
    base[1] = 0;

    // unsigned counter: retail compares with cmpli but keeps the u8 mask at
    // the i*8 subscript scale (clrlslwi r0,i,24,3).
    unsigned int i = 0;
    do {
        CMCCrystalBoxParam tmp;
        copyCrystalBoxParam(&((CMCCrystalBoxParam*)(base + 2))[(u8)i],
                      setCrystalBoxParam(&tmp, 0, 0));
        i++;
    } while (i < 0x20);

    return base;
}

void* __dt__80213E4C(void* obj, int flags) {
    if (obj != 0 && flags > 0) {
        operator delete(obj);
    }
    return obj;
}

// Retail keeps this as a rolled mtctr/bdnz count-down loop with a u8
// up-counter (clrlslwi/clrlwi masks) -- the -O4,s shape; -O4,p unrolls the
// constant-trip loop. -O4,s forces the rolled form.
// Retail zeroes a 32-byte word array at +0, the +0x20 byte, an 8-byte
// array at +0x21 (filled with 0xFF), +0x29 and +0x6A -- with a rolled
// mtctr/bdnz loop (the -O4,s shape; -O4,p unrolls). The header's +4-shifted
// pad_00/subObjPtrs view doesn't match this region, so raw offsets are used.
__declspec(noinline) void initCrystalBoxScratch(CrystalBoxScratch* scratch) {
    reinterpret_cast<CrystalBoxScratch*>(scratch)->count = 0;
    reinterpret_cast<CrystalBoxScratch*>(scratch)->flag29 = 0;
    reinterpret_cast<CrystalBoxScratch*>(scratch)->flag6A = 0;
    for (u8 i = 0; i < 8; i++) {
        reinterpret_cast<u32*>(scratch)[i] = 0;
        reinterpret_cast<CrystalBoxScratch*>(scratch)->fill[i] = -1;
    }
}

void* __dt__80213ECC(void* obj, int flags) {
    if (obj != 0 && flags > 0) {
        operator delete(obj);
    }
    return obj;
}

// Retail 0x80215D64: destroy all placed member subobjects in reverse order.
// stmw r30 frame is the -O4,s shape (unit flag).
CMCCrystalBox::~CMCCrystalBox() {}

// Retail 0x80215E3C: crystal-box constructor-style init. Reads the three
// resource archives (sort menu, crystal info, common) through the device file
// handles, then re-initialises every embedded sub-object from a stack temp:
// the temp is constructed, its body copied member-wise (skipping the vtable),
// and the temp destroyed. Finishes by reading the sub-object layout archives
// and clearing the shared BDAT cursor. stmw r30 frame is the -O4,s shape.
void CMCCrystalBox::initCrystalBoxResources() {
    // Resource archives: sort menu (+0x24), crystal info (+0x28), common
    // (+0x2C). The first two use the heap handle, the third the common-archive
    // file after KyoshinHeap_GetField44's handle setup.
    this->mFileHandle0 = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)getHandleMEM1__Q23mtl10MemManagerFv(), lbl_eu_8050888C + 0xc, this, 0, 0);
    this->mFileHandle1 = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)getHandleMEM2__Q23mtl10MemManagerFv(), lbl_eu_8050888C + 0x25, this, 0, 0);
    this->mFileHandle2 = (CFileHandle*)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)KyoshinHeap_GetField44(), lbl_eu_8050888C + 0x3e, this, 0, 0);

    // Re-init the sort menu from a default temp: copy every member except the
    // +0x6C s32 array (retail copies the mem region, pointer fields, flags,
    // scrollbar and the three tail bytes).
    this->sortMenu = CSortMenu();

    crystalInfo = CMCCrystalInfo();
    mScrollBar = CScrollBar(0);
    sysWin = CSysWin(0);

    // Read the sub-object layout archives and drive the syswin through its
    // slot-0x88 virtual, then reset the shared BDAT cursor and notify.
    this->sortMenu.Init();
    MCCrystal_LoadLayouts(&this->crystalInfo);
    this->mScrollBar.loadLayoutArc();
    this->sysWin.loadSystemArc();
    lbl_eu_806646D0 = 0;
    CItemBlock_setFlag120EC(9, 0);
}

// Retail 0x8021611c: per-frame crystal-box state machine. When visible and
// in a non-zero state, dispatch on the state through the jump table, then
// refresh the layout anim, sort menu, crystal info and all cursor sub-views.
void CMCCrystalBox::updateCrystalBoxState() {
    if (this->unk60 == 0) return;
    if (this->unk64 == 0) return;
    // Routing per retail jumptable_eu_80535C20 (15 entries): slot 0 routes
    // to the shared tail (empty case), slots 1..14 hold the handlers in
    // order, so the bound immediate is cmpli 14.
    switch (this->unk64) {
    case 0: break;
    case 1: onCrystalAnimToState2(); break;
    case 2: onCrystalAnimToState3(); break;
    case 3: advanceAnimTransform(this->mAnim2, lbl_eu_80668470); break;
    case 4: onCrystalAnimToState5(); break;
    case 5: onCrystalAnimToIdle(); break;
    case 6: playCrystalAnim14(); this->unk64 = 7; break;
    case 7: onCrystalAnim14Idle(); break;
    case 8: onCrystalAnimToState9(); break;
    case 9: onCrystalAnim9ToState3(); break;
    case 10: onCrystalAnim11State3(); break;
    case 11: onCrystalAnim12State3(); break;
    case 12: onCrystalAnim13State3(); break;
    case 13: onCrystalAnimToState14(); break;
    case 14: onCrystalAnimResetState3(); break;
    }
    // Refresh tail: run the layout virtual at slot 0x38, then update every
    // placed sub-object.
    reinterpret_cast<CLytVf38*>(this->mLayout)->vf_30(0);
    this->sortMenu.Move();
    MCCrystal_StepState(&this->crystalInfo);
    Cur_Update(&this->mCur07);
    Cur_Update(&this->mCur09);
    Cur_Update(&this->mCur18A);
    Cur_Update(&this->mCur11);
    Cur_Update(&this->mCur16);
    Cur_Update(&this->mCur18B);
    this->mScrollBar.UpdateDispatch();
    sysWinDispatchPhase(&this->sysWin);
}

void drawLayout(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// Retail 0x80216260: draw the crystal box sub-views.
// stmw r30 frame + r30-before-r31 copy order are the -O4,s shapes (unit flag).
void CMCCrystalBox::drawCrystalBox(nw4r::lyt::DrawInfo* di) {
    if (this->unk60 == 0) return;
    if (this->unk64 == 0) return;
    drawLayout((nw4r::lyt::Layout*)this->mLayout, di, 0, 1);
    MCCrystal_DrawLayout(&this->crystalInfo, di);
    this->sortMenu.Draw(di);
    u8 lim = this->data.limit;
    u8 active = lim != 0 ? lim : 1;
    if (active > 1) {
        Cur_DrawLayout(&this->mCur09, di);   // CCur09
    }
    Cur_DrawLayout(&this->mCur07, di);       // CCur07
    Cur_DrawLayout(&this->mCur18A, di);
    Cur_DrawLayout(&this->mCur11, di);
    Cur_DrawLayout(&this->mCur16, di);
    Cur_DrawLayout(&this->mCur18B, di);
    this->mScrollBar.draw(di);      // scrollbar
    sysWinDrawLayout(&this->sysWin, di);      // syswin
}

// Retail 0x8021634c: teardown of the whole crystal box. Closes the file
// handles for the first three resource sub-objects, frees the layout/arc
// resources, then destroys every placed member sub-object.
void CMCCrystalBox::teardownCrystalBox() {
    CBdat::getEntry(2);
    closeFileHandle__FPP11CFileHandle((void**)&this->mFileHandle0);
    closeFileHandle__FPP11CFileHandle((void**)&this->mFileHandle1);
    closeFileHandle__FPP11CFileHandle((void**)&this->mFileHandle2);

    if (this->unk60 != 0) {
        this->unk60 = 0;
        if (this->mLayout != 0) {
            delete this->mLayout;
            this->mLayout = 0;
        }
        releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(this->mArcAcc0);
        releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(this->mArcAcc1);
        deleteRegion__17UnkClass_8045F564Fv(&this->mMemRegion1);
        deleteRegion__17UnkClass_8045F564Fv(&this->mMemRegion2);
        this->sortMenu.Term();
        MCCrystal_Teardown(&this->crystalInfo);
        // Deactivate each placed cursor sub-object via its slot-0x0C virtual.
        this->mCur07.cleanup();
        this->mCur09.cleanup();
        this->mCur18A.cleanup();
        this->mCur11.cleanup();
        this->mCur16.cleanup();
        this->mCur18B.cleanup();
        this->mScrollBar.Teardown();
        sysWinTermLayout(&this->sysWin);
    }
}

int CMCCrystalBox::isCrystalBoxReady() {
    if (this->sortMenu.IsReady() == 0) return 0;
    if (func_8021A8EC(&this->crystalInfo) == 0) return 0;
    if (this->mScrollBar.isVisible() == 0) return 0;
    if (CSysWin_isReady(&this->sysWin) != 0) return this->unk68;
    return 0;
}

int CMCCrystalBox::getCrystalBoxActive() { return this->unk69; }

u8 CMCCrystalBox::isCrystalBoxBusy() {
    if (this->unk2D1 != 0) return 1;
    if (this->unk1500 != 0) return 1;
    return this->unk2CE;
}

int CMCCrystalBox::getCrystalBoxSysWin() { return CSysWin_getUnk34(&this->sysWin); }

// Retail 0x80216558: initialise the crystal box into state 1, mirror the
// current item, and point the scrollbar at the selected vec.
// _savegpr_28/_restgpr_28 out-of-line saves are the -O4,s shape (unit flag).
void CMCCrystalBox::openCrystalBox() {
    if (this->unk64 != 0) return;
    this->unk64 = 1;
    this->unk69 = 0;
    rebuildCrystalSortMenu();
    MCCrystal_OpenFromClosed(&this->crystalInfo);
    // idx materialized before d's address so MWCC assigns idx:r30, d:r29
    // (retail order).
    u8 idx = this->unk2CC + this->unk2CD * 10;
    CMCCrystalData* d = &this->data;
    // Nested-call form: MWCC evaluates MakeCrystal_GetItemObject (rightmost arg) first
    // like retail.
    MCCrystal_ShowItem(&this->crystalInfo,
                  MakeCrystal_GetSubCategory(d, idx), MakeCrystal_GetItemObject(d, idx));
    rebuildCrystalGridTable();
    float vec[3];
    // Retail reuses setVec3's returned dest pointer as the vec argument.
    this->mScrollBar.InitRootPane(reinterpret_cast<const nw4r::math::VEC3*>(code80135FDC_setVec3((nw4r::math::VEC3*)vec, lbl_eu_80668474,
                                                     lbl_eu_80668478,
                                                     lbl_eu_8066845C)));
    // Distinct expression shapes per call keep MWCC from CSE-ing the
    // +0x250 base address into one register (retail recomputes it).
    this->mScrollBar.UpdateThumb(8, 0);
    this->mScrollBar.PlaceThumb(0);
    this->mScrollBar.requestScrollIn();
    playCrystalAnim6();
}

void CMCCrystalBox::enterCrystalBoxState4() {
    if (this->unk64 != 3) return;
    if (this->unk2CE != 0) return;
    this->unk64 = 4;
    this->unk69 = 0;
    Cur_SetVisible(&this->mCur07, 0);
    Cur_SetVisible(&this->mCur09, 0);
    Cur_SetVisible(&this->mCur16, 0);
    Cur_SetVisible(&this->mCur11, 0);
    MCCrystal_PlayOutro(&this->crystalInfo);
    this->mScrollBar.requestScrollOut();
    playCrystalAnim7();
}

// Retail 0x802166E4: set state 6 and reset all sub-views.
void CMCCrystalBox::enterCrystalBoxState6() {
    if (this->unk64 != 3) return;
    if (this->unk2CE != 0) return;
    this->unk64 = 6;
    this->unk69 = 0;
    Cur_SetVisible(&this->mCur07, 0);
    Cur_SetVisible(&this->mCur09, 0);
    Cur_SetVisible(&this->mCur16, 0);
    Cur_SetVisible(&this->mCur11, 0);
    this->mScrollBar.requestScrollOut();
    MCCrystal_PlayAnim4(&this->crystalInfo);
    playCrystalAnim10();
    playUISound(0x6d);
}

void CMCCrystalBox::enterCrystalBoxState8() {
    if (this->unk64 != 3) return;
    if (this->unk2CE != 0) return;
    this->unk64 = 8;
    this->unk69 = 0;
    Cur_SetVisible(&this->mCur07, 0);
    Cur_SetVisible(&this->mCur09, 0);
    Cur_SetVisible(&this->mCur16, 0);
    Cur_SetVisible(&this->mCur11, 0);
    MCCrystal_PlayAnim3A(&this->crystalInfo);
    playCrystalAnim9();
    playUISound(0x6d);
}

void CMCCrystalBox::enterCrystalBoxState13() {
    if (this->unk64 != 3) return;
    if (this->unk2CE != 0) return;
    this->unk64 = 0xd;
    this->unk69 = 0;
    Cur_SetVisible(&this->mCur07, 0);
    Cur_SetVisible(&this->mCur09, 0);
    Cur_SetVisible(&this->mCur16, 0);
    Cur_SetVisible(&this->mCur11, 0);
    MCCrystal_PlayAnim3B(&this->crystalInfo);
    playCrystalAnim10();
}

// Retail 0x802168AC: cursor-up update callback. While the grid counter
// animation runs, step unk1501/unk1502 backwards through the 8x8 page; when
// the sort sub-menu is open scroll it up one entry; otherwise step the item
// cursor (unk2CD) down with per-window-kind wrap rules and refresh.
void CMCCrystalBox::stepCrystalBoxCursorPrev() {
    if (CSysWin_getUnk34(&this->sysWin) != 0) return;
    if (this->unk1500 != 0) {
        u8 v = this->unk1501 - 1;
        this->unk1501 = v;
        if ((s8)v >= 0) {
            // still inside the row block
        } else {
            this->unk1501 = 0;
            u8 w = this->unk1502 - 1;
            this->unk1502 = w;
            if ((s8)w < 0) {
                if ((s8)this->unk1503 >= 8) {
                    this->unk1501 = 7;
                    this->unk1502 = this->unk1503 - 8;
                } else {
                    u8 t = this->unk1503 - 1;
                    this->unk1501 = t;
                    this->unk1502 = 0;
                    if ((s8)t < 0) {
                        this->unk1501 = 0;
                    }
                }
            }
        }
        rebuildCrystalGridTable();
        refreshCrystalBoxCursors();
        this->mScrollBar.PlaceThumb((u16)(s8)this->unk1502);
        playUISound(1);
    } else if (this->unk2CE != 0) {
        // Sort sub-menu active: scroll up one entry and move the cursor.
        // Distinct expression shapes per call keep MWCC from merging the
        // this+0xFC address computations (retail recomputes each one).
        this->sortMenu.CursorUp();
        nw4r::math::VEC3 pos = this->sortMenu.GetCursorPos();
        this->mCur18A.setRootPaneTranslate(&pos);
        playUISound(1);
    } else {
        if (this->unk2D1 != 0) return;
        // One guard condition wrapping the step body: retail's short-circuit
        // chain jumps into the step code (beq/blt) or over it to the exit
        // (the trailing `b`), so the guards must share a single `if`.
        if (this->data.count == 0 ||
            (this->field_14A0 != 8 && (int)getCrystalSelTopCount() == 0 &&
             (this->field_14A0 < 2 || MakeCrystal_AllFlagsSet(&this->data) == 0))) {
            // Step the item cursor down, wrapping -3 back to 2.
            s8 v = this->unk2CD - 1;
            this->unk2CD = v;
            if ((s8)this->unk2CD < -2) {
                this->unk2CD = 2;
            }
            if (*(volatile u8*)&this->field_14A0 > 1) { // per-arm reload
                if ((s8)this->unk2CD == -1) this->unk2CD = -2;
            } else if (this->field_14A0 == 1) {
                if ((s8)this->unk2CD < 0) this->unk2CD = 2;
            } else {
                if ((s8)this->unk2CD == -2) this->unk2CD = 2;
            }
            refreshCrystalBoxCursors();
            setCrystalNameLabel();
            rebuildCrystalGridTable();
            playUISound(1);
        }
    }
}

// Retail 0x80216AD4: update-callback when the sort sub-menu is open and the
// counter animation is running. unk1501/unk1502 are the row/column cursor of
// the 8x8 grid; unk1503 is the page size. The >=8 branch advances by one row
// (wraps at the page size), the <8 branch advances within the row. When the
// sort sub-menu is active instead, scroll it down and drive the +0x9C cursor
// through its slot-0x10 virtual; otherwise step the item cursor (unk2CD),
// resolving the new row from the crystal table, and refresh.
void CMCCrystalBox::stepCrystalBoxCursorNext() {
    if (CSysWin_getUnk34(&this->sysWin) != 0) return;
    if (this->unk1500 != 0) {
        s8 limit = (s8)this->unk1503;
        if (limit >= 8) {
            u8 v = (u8)(this->unk1501 + 1);
            this->unk1501 = v;
            if ((s8)v >= 8) {
                this->unk1501 = 7;
                this->unk1502 = this->unk1502 + 1;
                if ((s8)this->unk1502 > limit - 8) {
                    this->unk1501 = 0;
                    this->unk1502 = 0;
                }
            }
        } else {
            u8 v = (u8)(this->unk1501 + 1);
            this->unk1501 = v;
            if ((s8)v >= limit) {
                this->unk1501 = 0;
                this->unk1502 = 0;
            }
        }
        rebuildCrystalGridTable();
        refreshCrystalBoxCursors();
        this->mScrollBar.PlaceThumb((u16)(s8)this->unk1502);
        playUISound(1);
    } else if (this->unk2CE != 0) {
        // Sort sub-menu active: scroll down one entry and move the cursor.
        this->sortMenu.CursorDown();
        nw4r::math::VEC3 pos = this->sortMenu.GetCursorPos();
        this->mCur18A.setRootPaneTranslate(&pos);
        playUISound(1);
    } else {
        // All guards share one condition: retail's short-circuit chain jumps
        // into the step code (beq/blt) or over it to the exit (trailing `b`).
        if (this->unk2D1 == 0 &&
            (this->data.count == 0 ||
             (this->field_14A0 != 8 && (int)getCrystalSelTopCount() == 0 &&
              (this->field_14A0 < 2 || MakeCrystal_AllFlagsSet(&this->data) == 0)))) {
            // Step the item cursor (s8 unk2CD), wrapping at 2.
            s8 v = this->unk2CD + 1;
            this->unk2CD = v;
            if ((s8)this->unk2CD > 2) {
                this->unk2CD = -2;
            }
            if (this->field_14A0 >= 2) {
                if ((s8)this->unk2CD == -1) this->unk2CD = 0;
            } else if (this->field_14A0 >= 1) {
                if ((s8)this->unk2CD == -2) this->unk2CD = 0;
            } else {
                if ((s8)this->unk2CD == -2) this->unk2CD = -1;
            }
            refreshCrystalBoxCursors();
            setCrystalNameLabel();
            rebuildCrystalGridTable();
            playUISound(1);
        }
    }
}

// Retail 0x80216D14: update-callback mirror of stepCrystalBoxCursorNext with the
// counter animation stepped in the opposite direction: unk1502 decrements by
// the page size (8) and the row cursor wraps through the page; when unk2D1 is
// set, the window-kind byte (unk2D3) selects between the -0x2D4/-0x2D5
// decrement paths and drives the +0x64 state / anim; otherwise the item
// cursor is stepped back (unk2CD) with the -0x2CC countdown.
void CMCCrystalBox::stepCrystalBoxPageBack() {
    if (CSysWin_getUnk34(&this->sysWin) != 0) return;
    if (this->unk1500 != 0) {
        if ((s8)this->unk1503 >= 8) {
            u8 w = (u8)(this->unk1502 - 8);
            this->unk1502 = w;
            if ((s8)w < 0) {
                // keep w+7 as an int expression: retail stores the unmasked
                // sum and only masks for the sign test
                this->unk1501 = w + 7;
                this->unk1502 = 0;
                if ((s8)(u8)(w + 7) < 0) {
                    this->unk1501 = 0;
                }
            }
        } else {
            this->unk1501 = 0;
            this->unk1502 = 0;
        }
        rebuildCrystalGridTable();
        refreshCrystalBoxCursors();
        this->mScrollBar.PlaceThumb((u16)this->unk1502s);
        playUISound(1);
    } else if (this->unk2CE != 0) {
        // Sort sub-menu active: page up one page and move the cursor.
        this->sortMenu.PageUp();
        nw4r::math::VEC3 pos = this->sortMenu.GetCursorPos();
        this->mCur18A.setRootPaneTranslate(&pos);
        playUISound(1);
    } else {
        // retail tests the raw byte here (no sign extension)
        if (this->unk2D1 != 0) {
            // Window-kind path: unk2D3 0 decrements the -0x2D4 byte, unk2D3
            // 1 (with enough room) decrements -0x2D5 and re-syncs them, then
            // enters state 11 and plays back animation 12.
            if ((s8)this->unk2D3 == 2) return;
            u8 g = code80135FDC_getByte_64077();
            if ((s8)this->unk2D3 == 0) {
                int v = this->unk2D4 - 1;
                this->unk2D4 = v;
                if ((s8)(u8)v < 0) {
                    this->unk2D4 = g - 1;
                }
                showCrystalBoxStatus();
                playUISound(1);
            } else if ((s8)this->unk2D3 == 1) {
                if ((u8)g <= 2) return;
                // int temps: retail masks after the store (clrlwi+extsb)
                int v = this->unk2D5 - 1;
                this->unk2D5 = v;
                if ((s8)(u8)v < 0) {
                    this->unk2D5 = g - 1;
                }
                if ((s8)this->unk2D5 == (s8)this->unk2D4) {
                    int v2 = this->unk2D5 - 1;
                    this->unk2D5 = v2;
                    if ((s8)(u8)v2 < 0) {
                        this->unk2D5 = g - 1;
                    }
                }
                showCrystalBoxStatus();
                this->unk64 = 11;
                playCrystalAnim12();
                playUISound(1);
            }
        } else {
            // Item cursor path: skip the -2 sentinel; at -1 the countdown
            // reloads the whole list (no sound), otherwise -0x2CC counts
            // down to 9.
            if ((s8)this->unk2CD == -2) return;
            if ((s8)this->unk2CD == -1) {
                tickCrystalBoxPageBack();
                refreshCrystalBoxCursors();
            } else {
                int c = this->unk2CC;
                if ((s8)c == 0) {
                    this->unk2CC = 9;
                    confirmCrystalUnselect();
                    refreshCrystalBoxCursors();
                } else {
                    int v = c - 1;
                    this->unk2CC = v;
                    if ((s8)(u8)v < 0) {
                        this->unk2CC = 9;
                    }
                    refreshCrystalBoxCursors();
                    setCrystalNameLabel();
                    rebuildCrystalGridTable();
                }
                playUISound(1);
            }
        }
    }
}

// Retail 0x80216F9C: update-callback mirror of stepCrystalBoxPageBack with the
// counter animation stepped FORWARD: unk1502 increments by the page size (8)
// with carry into unk1501; the window-kind path increments -0x2D4/-0x2D5
// (wrapping at the byte from code80135FDC_getByte_64077) before entering
// state 11; the item-cursor path counts -0x2CC up to 9 then wraps to 0.
void CMCCrystalBox::pageCrystalBoxCursorFwd() {
    if (CSysWin_getUnk34(&this->sysWin) != 0) return;
    if (this->unk1500 != 0) {
        s8 lim = (s8)this->unk1503;
        if (lim >= 8) {
            u8 w = (u8)(this->unk1502 + 8);
            this->unk1502 = w;
            if ((s8)w > lim - 8) {
                u8 t = (u8)((u8)w - (lim - 8));
                this->unk1501 = t;
                this->unk1502 = (u8)(this->unk1503 - 8);
                if ((s8)t >= 8) {
                    this->unk1501 = 7;
                }
            }
        } else {
            u8 t = (u8)(this->unk1503 - 1);
            this->unk1501 = t;
            this->unk1502 = 0;
            if ((s8)t < 0) {
                this->unk1501 = 0;
            }
        }
        rebuildCrystalGridTable();
        refreshCrystalBoxCursors();
        this->mScrollBar.PlaceThumb((u16)(s8)this->unk1502);
        playUISound(1);
    } else if (this->unk2CE != 0) {
        // Sort sub-menu active: page down one page and move the cursor.
        this->sortMenu.PageDown();
        nw4r::math::VEC3 pos = this->sortMenu.GetCursorPos();
        this->mCur18A.setRootPaneTranslate(&pos);
        playUISound(1);
    } else {
        if (this->unk2D1 == 0) {
            // Item cursor path: at -1 reload the list, otherwise -0x2CC
            // counts up and wraps at 10.
            if ((s8)this->unk2CD == -2) return;
            if ((s8)this->unk2CD != -1) {
                u8 c = this->unk2CC;
                if (c == 9) {
                    this->unk2CC = 0;
                    confirmCrystalSelect();
                    refreshCrystalBoxCursors();
                } else {
                    u8 v = (u8)(c + 1);
                    this->unk2CC = v;
                    if ((s8)v >= 10) {
                        this->unk2CC = 0;
                    }
                    refreshCrystalBoxCursors();
                    setCrystalNameLabel();
                    rebuildCrystalGridTable();
                }
                playUISound(1);
            } else {
                tickCrystalBoxPageFwd();
                refreshCrystalBoxCursors();
            }
        } else {
            // Window-kind path: unk2D3 0 increments -0x2D4 (wrapping at g);
            // unk2D3 1 increments -0x2D5 (also wrapping) and re-syncs the
            // pair when equal, then enters state 11 and plays animation 12.
            if (this->unk2D3 == 2) return;
            u8 g = code80135FDC_getByte_64077();
            if ((s8)this->unk2D3 == 0) {
                u8 v = (u8)(this->unk2D4 + 1);
                this->unk2D4 = v;
                if ((s8)v >= (u8)g) {
                    this->unk2D4 = 0;
                }
                showCrystalBoxStatus();
                playUISound(1);
            } else if ((s8)this->unk2D3 == 1) {
                if ((u8)g <= 2) return;
                u8 v = (u8)(this->unk2D5 + 1);
                this->unk2D5 = v;
                if ((s8)v >= (u8)g) {
                    this->unk2D5 = 0;
                }
                s8 d5 = (s8)this->unk2D5;
                s8 d4 = (s8)this->unk2D4;
                if (d5 == d4) {
                    u8 v2 = (u8)(d5 + 1);
                    this->unk2D5 = v2;
                    if ((s8)v2 >= (u8)g) {
                        this->unk2D5 = 0;
                    }
                }
                showCrystalBoxStatus();
                this->unk64 = 11;
                playCrystalAnim12();
                playUISound(1);
            }
        }
    }
}

__declspec(noinline) void CMCCrystalBox::confirmCrystalSelect() {
    if (CSysWin_getUnk34(&this->sysWin) != 0) return;
    if (this->unk2CE != 0) return;
    if (this->unk2D1 != 0) return;
    if ((s8)this->unk2CD == -2) return;

    MakeCrystal_AdvanceCursor(&this->data);
    updateCrystalBoxRows();
    setCrystalNameLabel();
    rebuildCrystalGridTable();
    playUISound(0xa);
}

__declspec(noinline) void CMCCrystalBox::confirmCrystalUnselect() {
    if (CSysWin_getUnk34(&this->sysWin) != 0) return;
    if (this->unk2CE != 0) return;
    if (this->unk2D1 != 0) return;
    if ((s8)this->unk2CD == -2) return;

    func_802137B4(&this->data);
    updateCrystalBoxRows();
    setCrystalNameLabel();
    rebuildCrystalGridTable();
    playUISound(0xa);
}
// Retail 0x80217370: open/close the sort sub-menu. When a sub-menu is active
// (unk2CE) it is closed; otherwise the sort menu is opened: two layout panes
// are formatted, copied into the sort menu, the countdown label refreshed, and
// the +0x9C cursor is driven through its slot-0x10 virtual. stmw r28 frame is
// the -O4,s shape (unit flag).
void CMCCrystalBox::toggleCrystalSortMenu() {
    if (CSysWin_getUnk34(&this->sysWin) != 0) return;
    if (this->unk1500 != 0) return;
    if (this->unk2D1 != 0) return;
    if ((s8)this->unk2CD == -2) return;
    if (this->unk2CE != 0) {
        if (this->sortMenu.IsIdle() == 0) return;
        refreshCrystalBoxCursors();
        Cur_SetVisible(&this->mCur07, 1);
        Cur_SetVisible(&this->mCur18A, 0);
        this->sortMenu.Close();
        this->unk2CE = 0;
        playUISound(6);
    } else {
        if (this->sortMenu.IsIdle() == 0) return;
        char* base = lbl_eu_8050888C;
        CLytVf3C* sub = ((CrystalBoxSubObj5*)this->mLayout)->layout;
        // MWCC evaluates the two inline vf_3C argument calls right-to-left,
        // matching retail's pane-fetch order (base+0x63 before base+0x5a).
        nw4r::math::VEC3 local;
        ScalePaneWorldPos(&local, sub->vf_3C(base + 0x5a, 1),
                      sub->vf_3C(base + 0x63, 1), (nw4r::lyt::Pane*)sub);
        this->sortMenu.SetPos(&local);
        this->sortMenu.SetSelect((u8)(this->field_1506 + this->field_1507));
        nw4r::math::VEC3 pos = this->sortMenu.GetCursorPos();
        this->mCur18A.setRootPaneTranslate(&pos);
        Cur_SetVisible(&this->mCur18A, 1);
        Cur_SetVisible(&this->mCur07, 0);
        this->sortMenu.Open();
        this->unk2CE = 1;
        this->unk2CF = 0;
        playUISound(3);
    }
}

// Retail 0x80217518: per-frame crystal-box update callback. While the system
// window is open, keep its animation alive (unk1500 grid); otherwise close
// the sort menu, step the window-kind counter, or scan the crystal table for
// the selected slot. The scan block is duplicated in source because retail
// holds two inline copies (one for the count!=0 gate, one for the a0
// fallback). Ends by refreshing the page-2 label and beeping 6.
// stmw r22 frame is the -O4,s shape (unit flag).
void CMCCrystalBox::advanceCrystalBoxSortMenu(int a) {
    if (this->sortMenu.IsIdle() == 0) return;
    if (CSysWin_getUnk34(&this->sysWin) != 0) {
        if (CSysWin_isActive(&this->sysWin) == 0) {
            return;
        }
        sysWinAdvancePhase3(&this->sysWin);
        if (this->unk1500 != 0) {
            Cur_SetVisible(&this->mCur18B, 1);
        }
    } else if (this->unk1500 != 0) {
        this->unk1500 = 0;
        this->unk1501 = 0;
        this->unk1502 = 0;
        refreshCrystalBoxCursors();
        rebuildCrystalGridTable();
    } else if (this->unk2CE != 0) {
        refreshCrystalBoxCursors();
        Cur_SetVisible(&this->mCur07, 1);
        Cur_SetVisible(&this->mCur18A, 0);
        this->sortMenu.Close();
        this->unk2CE = 0;
    } else if (this->unk2D1 != 0) {
        if (this->unk64 != 3) return;
        if ((s8)this->unk2D3 == 0) {
            this->unk2D1 = 0;
            this->unk69 = 0;
        } else {
            this->unk2D3 = (u8)(this->unk2D3 - 1);
            showCrystalBoxStatus();
            refreshCrystalBoxCursors();
            if ((s8)this->unk2D3 == 0) {
                this->unk64 = 12;
                playCrystalAnim13();
            }
            setPaneVisible((*(CLytVf3C**)((u8*)this->mLayout + 0x10))
                              ->vf_3C(lbl_eu_8050888C + 0x72, 1),
                          1);
        }
    } else {
        u16 count = this->data.count;
        u8 a0 = this->field_14A0;
        u8* selTable = (u8*)this + 0x1480;
        if (count != 0 &&
            (a0 == 8 || (int)getCrystalSelTopCount() != 0 ||
             MakeCrystal_AllFlagsSet(&this->data) != 0)) {
            // scan #1: walk the 10 rows x 30 columns for the selected slot
            void* sel = (void*)getCrystalSelectEntry(selTable);
            CMCCrystalData* d = &this->data;
            u8 cur = (u8)d->current;
            u8 last = (u8)((u8)(cur + 1) - 1);
            int found = 0;
            u8 row;
            for (row = 0; row < 10; row++) {
                d->current = row;
                u8 col;
                for (col = 0; col < 30; col++) {
                    if (MakeCrystal_GetItemObject(d, col) == sel) {
                        MakeCrystal_SetFlag((void*)d, col, 0);
                        this->unk2CD = col != 0 ? col / 10 : 0;
                        this->unk2CC = col != 0 ? col - this->unk2CD * 10 : 0;
                        refreshCrystalBoxCursors();
                        d->current = row;
                        found = 1;
                        break;
                    }
                }
                if (found != 0) break;
            }
            releaseCrystalBoxSlot((CrystalBoxScratch*)selTable);
            if (found == 0) d->current = last;
            updateCrystalBoxRows();
            setCrystalNameLabel();
            placeCrystalBoxSlots();
            rebuildCrystalGridTable();
        } else {
            if (a0 == 0) {
                this->unk69 = 0;
            } else {
                // scan #2: same search for the a0 != 0 fallback
                void* sel = (void*)getCrystalSelectEntry(selTable);
            CMCCrystalData* d = &this->data;
            u8 cur = (u8)d->current;
            u8 last = (u8)((u8)(cur + 1) - 1);
            int found = 0;
            u8 row;
            for (row = 0; row < 10; row++) {
                d->current = row;
                u8 col;
                for (col = 0; col < 30; col++) {
                    if (MakeCrystal_GetItemObject(d, col) == sel) {
                        MakeCrystal_SetFlag((void*)d, col, 0);
                        this->unk2CD = col != 0 ? col / 10 : 0;
                        this->unk2CC = col != 0 ? col - this->unk2CD * 10 : 0;
                        refreshCrystalBoxCursors();
                        d->current = row;
                        found = 1;
                        break;
                    }
                }
                if (found != 0) break;
            }
            releaseCrystalBoxSlot((CrystalBoxScratch*)selTable);
            if (found == 0) d->current = last;
            updateCrystalBoxRows();
            setCrystalNameLabel();
            placeCrystalBoxSlots();
            rebuildCrystalGridTable();
            }
        }
        setPaneVisible((*(CLytVf3C**)((u8*)this->mLayout + 0x10))
                          ->vf_3C(lbl_eu_8050888C + 0x7c, 1),
                      selTable[0x20] < 2);
    }
    if (a != 0) playUISound(6);
}

// Retail 0x80217940: return the 8-slot selection-table word selected by the
// header (count byte at +0x20, head index at +0x29, slot index byte at
// 0x20+head). Used by the crystal-scan in advanceCrystalBoxSortMenu.
int getCrystalSelectEntry(void* obj) {
    CrystalBoxScratch* p = (CrystalBoxScratch*)obj;
    if (p->count == 0) return 0;
    return *(int*)((u8*)p + 4 * (s8)((u8*)p)[0x20 + p->flag29]);
}

// Retail tail-calls the aggregator (b buildCrystalBoxIdList); keep it out-of-line.

void releaseCrystalBoxSlot(CrystalBoxScratch* scratch) {
    // Free-slot bookkeeping on the scratch selection table: unclaim the slot
    // owned at [0x20+slotIdx] and decrement the header counter. Reads after
    // the store are cached in locals so MWCC reloads them once (aliasing).
    if (reinterpret_cast<CrystalBoxScratch*>(scratch)->count == 0) return;

    int* table = (int*)scratch;
    signed char owner = (s8)((u8*)scratch)[0x20 + ((u8*)scratch)[0x29]];
    if (table[owner] == 0) return;

    table[owner] = 0;
    u8 cnt = ((u8*)scratch)[0x20];
    u8 t = ((u8*)scratch)[0x29];
    reinterpret_cast<CrystalBoxScratch*>(scratch)->count = cnt - 1;
    ((s8*)scratch)[0x21 + t] = -1;
    ((u8*)scratch)[0x29] = t - 1;
    buildCrystalBoxIdList(scratch);
}

// stmw r26 frame + rolled constant-trip loops are the -O4,s shape (unit flag).
// Retail 0x802179D0: aggregate the 8 item-list slots of the scratch head into
// (id, quantity) pairs, bubble-sort them by descending quantity, then publish
// the id list into the head itself (ids at +0x2A, count byte at +0x6A).
__declspec(noinline) void buildCrystalBoxIdList(CrystalBoxScratch* scratch) {
    MCAggPair pairs[32];
    MCAggPair* p;
    u8 n;

    // Zero-fill the transient pair table (pointer walk, matching retail).
    p = pairs;
    do {
        p->id = 0;
        p->count = 0;
        p++;
    } while (p < &pairs[32]);

    n = 0;

    // The head's first 8 words are the placed item pointers.
    for (u8 i = 0; i < 8; i++) {
        void* item = ((void**)scratch)[i];
        if (item == 0) continue;
        for (u8 j = 0; j < 4; j++) {
            // Call the virtual directly on the returned instance pointer:
            // retail never names the facade temp (it would claim a
            // callee-saved register).
            u16 id = ((CItemImplFacadeAgg*)CItem_initItemImplInstances(item))
                         ->GetName(item, j);
            if (id == 0) continue;
            // Raw 32-bit result: retail folds it into the count unmasked.
            s32 cnt =
                ((CItemImplFacadeAgg*)CItem_initItemImplInstances(item))->GetFlag(item, j);
            int found = 0;
            // u8 bounds replicate retail's masked counters (n==0 still runs
            // the (u8)(n-1) == 0xFF sort passes harmlessly).
            for (u8 k = 0; k < n; k++) {
                if (pairs[k].id == id) {
                    // 32-bit accumulate through an int temp: keeps MWCC from
                    // normalizing cnt into the 16-bit domain before the add.
                    s32 total = pairs[k].count;
                    total += cnt;
                    pairs[k].count = total;
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                pairs[n].id = id;
                pairs[n].count = cnt;
                n++;
            }
        }
    }

    // Sort bounds stay int-typed: retail subtracts from the masked count
    // without re-masking (r6 = n-1 raw).
    int m = n - 1;
    for (u8 i = 0; i < m; i++) {
        int lim = m - i;
        for (u8 j = 0; j < lim; j++) {
            if (pairs[j].count < pairs[j + 1].count) {
                // Halfword-wise exchange through explicit element pointers:
                // keeps the temp in registers while matching retail size.
                MCAggPair* a = &pairs[j];
                MCAggPair* b = &pairs[j + 1];
                u16 tid = a->id;
                u16 tcnt = a->count;
                a->id = b->id;
                a->count = b->count;
                b->id = tid;
                b->count = tcnt;
            }
        }
    }

    // Publish: ids packed at +0x2A, count byte at +0x6A.
    u8* base = (u8*)scratch;
    base[0x6A] = 0;
    if (n != 0) {
        *(u16*)(base + 0x2A) = pairs[0].id;
        base[0x6A] = 1;
        for (u8 k = 1; k < n; k++) {
            u8 flag = base[0x6A];
            ((u16*)(base + 0x2A))[flag] = pairs[k].id;
            base[0x6A] = flag + 1;
        }
    }
}

void handleCrystalBoxConfirm(){}

u8 CMCCrystalBox::getCrystalBoxKindA() { return this->unk2D4; }

u8 CMCCrystalBox::getCrystalBoxKindB() { return this->unk2D5; }

// Retail 0x802180B4: rebuild the selected-crystal table. Re-initialises the
// 32-slot transient sort table (via initCrystalBoxParamTbl), aggregates the 8-slot
// selection table (+0x1480) into quantity-pair entries, bubble-sorts them by
// quantity, and copies the result into the +0x2D8 sub-table, then refreshes
// the current-item byte and drives the item-implementation virtuals.
// stmw r23 frame is the -O4,s shape (unit flag).
u8* CMCCrystalBox::rebuildCrystalBoxSelTable() {
    // Stack layout (retail): sel block (head+32 slots) at +0x138, sort table
    // at +0x38, then the six temps at +0x30..+0x8.
    struct SelTable {
        u8 head[2];                   // +0x00
        CMCCrystalBoxParam slots[32]; // +0x02
    };
    struct ParamBlock { CMCCrystalBoxParam slots[32]; };
    SelTable selT;                    // +0x138 (initialised by initCrystalBoxParamTbl)
    CMCCrystalBoxParam sortTable[32]; // +0x38
    CMCCrystalBoxParam t1;            // +0x30 (bubble-sort temp)
    CMCCrystalBoxParam pa;            // +0x28 (re-init temp)
    CMCCrystalBoxParam tmp2;          // +0x20 (add-entry temp)
    CMCCrystalBoxParam t2;            // +0x18 (bubble-sort temp)
    CMCCrystalBoxParam t3;            // +0x10 (bubble-sort temp)
    CMCCrystalBoxParam tmp3;          // +0x8  (fill-loop temp)

    // Exactly nine working variables mirroring retail's saved-register
    // homes; each serves several phases (like retail's register reuse):
    // count:r31, tb:r30 (clear sentinel / selection-table base),
    // wk:r28 (sort-table walk / item ptr / bubble limit), ii:r27,
    // jj:r26, nm:r25 (item name / bubble outer), sa:r24, sb:r23.
    u8 count;
    unsigned long tb;
    unsigned long wk;
    unsigned int ii;
    unsigned int jj;
    u16 nm;
    CMCCrystalBoxParam* sa;
    CMCCrystalBoxParam* sb;

    // Initialise the selection table (head + 32 slots) and copy the slots
    // into the +0x2D8 sub-table.
    initCrystalBoxParamTbl(selT.head);
    this->unk2D6 = selT.head[0];
    this->unk2D7 = selT.head[1];
    // 0x100-byte block assignment lowers to MWCC's rolled update-form
    // word-copy loop.
    *(ParamBlock*)this->subTable = *(ParamBlock*)selT.slots;

    // Clear the sort table twice: a pointer walk, then a slot re-init via a
    // stack temp so each entry is written by copyCrystalBoxParam.
    wk = (unsigned long)sortTable;
    tb = (unsigned long)&selT;
    count = 0;
    do {
        setCrystalBoxParam((CMCCrystalBoxParam*)wk, 0, 0);
        wk += 8;
    } while (wk < tb);
    ii = 0;
    do {
        copyCrystalBoxParam(&sortTable[ii], setCrystalBoxParam(&pa, 0, 0));
        ii++;
    } while (ii < 0x20);

    // Aggregate the 8-slot selection table: for each placed item, walk its
    // 4 sub-items, accumulate quantities of matching crystal ids in the sort
    // table, and append new ids.
    tb = (unsigned long)((u8*)this + 0x1480);
    for (ii = 0; ii < 8; ii++) {
        wk = getSelectTableWord((unsigned long*)tb, (u8)ii);
        if (wk == 0 || *(unsigned long*)wk == 0) continue;
        for (jj = 0; jj < 4; jj++) {
            CItemImplFacade2* inst = (CItemImplFacade2*)CItem_initItemImplInstances((unsigned long*)wk);
            nm = inst->GetName((unsigned long*)wk, (u8)jj);
            if ((s16)nm <= 0) continue;
            u16 flag = inst->GetFlag((unsigned long*)wk, (u8)jj);
            int found = 0;
            for (u8 k = 0; k < count; k++) {
                if (sortTable[k].m0 == (s16)nm) {
                    sortTable[k].m2 += flag;
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                copyCrystalBoxParam(&sortTable[count],
                              setCrystalBoxParam(&tmp2, (s16)nm, (s16)flag));
                count++;
            }
        }
    }

    // Bubble-sort the sort table by quantity (ascending).
    wk = (unsigned long)((u8)count - 1);
    for (nm = 0; nm < (u8)wk; nm++) {
        for (jj = 0; jj < (unsigned int)(wk - (u8)nm); jj++) {
            if (sortTable[jj].m2 < sortTable[jj + 1].m2) {
                sa = &sortTable[jj];
                sb = &sortTable[jj + 1];
                copyCrystalParamRet(&t1, sa);
                copyCrystalBoxParam(sa, copyCrystalParamRet(&t2, sb));
                copyCrystalBoxParam(sb, copyCrystalParamRet(&t3, &t1));
            }
        }
    }

    // Copy the sorted table into the +0x2D8 sub-table, bumping the cursor.
    for (u8 fk = 0; fk < (u8)count; fk++) {
        setCrystalBoxParam(&tmp3, sortTable[fk].m0, sortTable[fk].m2);
        u8 cur = this->unk2D6;
        this->unk2D6 = cur + 1;
        copyCrystalBoxParam(&this->subTable[cur], &tmp3);
    }

    // Refresh the current-item byte from the first placed item, then drive
    // each placed item's slot-0x10 virtual.
    for (ii = 0; ii < 8; ii++) {
        wk = getSelectTableWord((unsigned long*)tb, (u8)ii);
        if (wk == 0 || *(unsigned long*)wk == 0) continue;
        CItemImplFacade2* inst = (CItemImplFacade2*)CItem_initItemImplInstances((unsigned long*)wk);
        this->unk2D7 = (u8)inst->GetCount((unsigned long*)wk);
        break;
    }
    for (ii = 0; ii < 8; ii++) {
        wk = getSelectTableWord((unsigned long*)tb, (u8)ii);
        if (wk == 0 || *(unsigned long*)wk == 0) continue;
        CItemImplFacade2* inst = (CItemImplFacade2*)CItem_initItemImplInstances((unsigned long*)wk);
        inst->vf_08((unsigned long*)wk);
    }

    return &this->unk2D6;
}

// Table lookup used by the 8-slot selection loop (placeCrystalBoxSlots). Retail calls
// it out-of-line; keep it non-inlined so the caller emits `bl getSelectTableWord`
// instead of the compiler eliminating the read chain.
__declspec(noinline) unsigned long getSelectTableWord(unsigned long* table, unsigned int idx) {
    if (idx >= 8) {
        return 0;
    }
    return table[(unsigned char)idx];
}

CMCCrystalBoxParam* copyCrystalParamRet(CMCCrystalBoxParam *dest, const CMCCrystalBoxParam *src) {
    unsigned short *d = (unsigned short *)dest;
    const unsigned short *s = (const unsigned short *)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    unsigned char *db = (unsigned char *)dest;
    const unsigned char *sb = (const unsigned char *)src;
    db[6] = sb[6];
    db[7] = sb[7];
    return dest;
}

__declspec(noinline) void CMCCrystalBox::tickCrystalBoxPageFwd() {
    if (this->unk2CE != 0) return;
    if (this->unk2D1 != 0) return;

    // Countdown timer at +0x14F2 increments; when it exceeds +0x14F1 it wraps to 0.
    u8 v = this->field_14F2 + 1;
    this->field_14F2 = v;
    if ((s8)v >= this->field_14F1) {
        this->field_14F2 = 0;
    }

    refreshCrystalPagePair(0);
    refreshCrystalBoxName();
    playUISound(0x70);
}

__declspec(noinline) void CMCCrystalBox::tickCrystalBoxPageBack() {
    if (this->unk2CE != 0) return;
    if (this->unk2D1 != 0) return;

    // Countdown timer at +0x14F2; when it wraps sub-range, reload from +0x14F1.
    u8 v = this->field_14F2 - 1;
    this->field_14F2 = v;
    if ((s8)v < 0) {
        this->field_14F2 = this->field_14F1 - 1;
    }

    refreshCrystalPagePair(1);
    refreshCrystalBoxName();
    playUISound(0x70);
}

// Retail 0x8021B7EC: refresh the two countdown labels. Picks the pair of
// indices (current ticker + the neighbour), formats "N+1" into two panes via
// the layout sub-object's slot-0x3C virtual, and sets each pane's visibility
// from whether the crystal entry is present / selected.
__declspec(noinline) void CMCCrystalBox::refreshCrystalPagePair(int dir) {
    u8 pair[2];
    char bufSel[0x20];   // fmt +0x86 (retail sp+0x30)
    char bufOther[0x20]; // fmt +0x95 (retail sp+0x10)
    u8 t0 = this->field_14F2;
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
        pair[1] = this->field_14F1 - 1;
    } else if (sc >= this->field_14F1) {
        pair[1] = 0;
    }
    // Retail keeps the counter wide (unmasked cmpli against 2) but masks it
    // for the pair[] subscript.
    for (u32 i = 0; i < 2; i++) {
        // val is formed inline at both call sites; retail keeps only the raw
        // pair[] byte live across the sprintf calls.
        sprintf(bufSel, lbl_eu_8050888C + 0x86, (s8)pair[(u8)i] + 1);
        sprintf(bufOther, lbl_eu_8050888C + 0x95, (s8)pair[(u8)i] + 1);
        // Both flags are initialised eagerly and re-assigned in each branch,
        // with the entry lookup between the two initialisers (retail order).
        int flagA = 0;
        s32 idx = (s8)pair[(u8)i];
        int flagB = 0;
        if (this->field_14EC[idx] != 0) {
            if (idx == (s8)this->field_14F2) {
                flagA = 1;
                flagB = 0;
            } else {
                flagA = 0;
                flagB = 1;
            }
        }
        // Retail reloads layout and the +0x10 sub-object per call (no CSE).
        setPaneVisible((((CrystalBoxSubObj5*)this->mLayout)->layout)->vf_3C(bufSel, 1), flagA);
        setPaneVisible((((CrystalBoxSubObj5*)this->mLayout)->layout)->vf_3C(bufOther, 1), flagB);
    }
}

// Retail 0x80218570: initialise the five countdown page labels. Fills the
// page-enable table (1..5) and the page count, then formats each page
// number into two pane-name buffers and toggles each pane's visibility
// from whether that page is enabled / is the selected one. Ends by
// refreshing the neighbouring-page pair (refreshCrystalPagePair).
__declspec(noinline) void CMCCrystalBox::initCrystalPageLabels() {
    char* base = lbl_eu_8050888C;
    char bufA[0x20];
    char bufB[0x20];
    int val;
    int selVis;
    int otherVis;
    u8 idx;
    this->field_14EC[0] = 1;
    this->field_14EC[1] = 2;
    this->field_14EC[2] = 3;
    this->field_14EC[3] = 4;
    this->field_14EC[4] = 5;
    this->field_14F1 = 5;
    // Retail's loop tests the counter at the bottom (cmplwi/blt).
    u8 i;
    for (i = 0; i < 5; i++) {
        val = (u8)i + 1;
        sprintf(bufA, base + 0x86, val);
        sprintf(bufB, base + 0x95, val);
        idx = i;
        int selVis = 0;
        int otherVis = 0;
        if (this->field_14EC[idx] != 0) {
            if (idx == (s8)this->field_14F2) {
                selVis = 1;
                otherVis = 0;
            } else {
                selVis = 0;
                otherVis = 1;
            }
        }
        // Retail reloads the layout and its +0x10 sub-object per call.
        setPaneVisible(
            (((CrystalBoxSubObj5*)this->mLayout)->layout)->vf_3C(bufA, 1),
            selVis);
        setPaneVisible(
            (((CrystalBoxSubObj5*)this->mLayout)->layout)->vf_3C(bufB, 1),
            otherVis);
    }
    refreshCrystalPagePair(0);
}

// Retail 0x802186A8: crystal-box update with a system-window (CSysWin at
// +0x290) tie-in. When the sort menu is idle and the cursor is not in the
// -1 state, either refresh the open window (field_14A0 == 0 with a pending
// crystal) or open/update it with the current item's name; otherwise fall
// back to opening the window for the first time.
// stmw r30 frame is the -O4,s shape (unit flag).
void CMCCrystalBox::updateCrystalBoxSysWin() {
    if (this->sortMenu.IsOpen() != 0) return;
    if ((s8)this->unk2CD == -1) return;

    if (this->field_14A0 == 0) {
        if (CSysWin_getUnk34(&this->sysWin) != 0) {
            // Window already open: keep it alive while the crystal animates.
            if (this->unk1500 == 0) return;
            if (CSysWin_isActive(&this->sysWin) == 0) return;
            sysWinAdvancePhase3(&this->sysWin);
            if (this->unk1500 == 0) return;
            Cur_SetVisible(&this->mCur18B, 1);
            return;
        }
        // No window: only open it when a crystal exists at the current slot.
        if (MakeCrystal_GetItemObject(&this->data,
                          (u8)(this->unk2CC + (u8)this->unk2CD * 10)) == 0) {
            playUISound(5);
            return;
        }
    }

    if (this->unk1500 != 0) {
        if (CSysWin_getUnk34(&this->sysWin) != 0) {
            // Window already open: keep it alive while the crystal animates.
            if (CSysWin_isActive(&this->sysWin) == 0) return;
            sysWinAdvancePhase3(&this->sysWin);
            if (this->unk1500 == 0) return;
            Cur_SetVisible(&this->mCur18B, 1);
            return;
        }
        if (this->unk1504 != 0) {
            // Open the window with the current item name; unk1504 doubles as
            // the 3-arg BdatGetPtrDirect key (retail hoists the r5 load above the
            // branch that also tests it).
            char* msg = BdatGetPtrDirect((const void*)lbl_eu_806646D0,
                                      lbl_eu_8050888C + 0xa4, this->unk1504);
            sysWinSwitchKindPane(&this->sysWin, 0);
            sysWinSetKindContent(&this->sysWin, (u32)msg, 0);
            sysWinApplyKind(&this->sysWin, 1);
            sysWinOpenPhase1(&this->sysWin);
            Cur_SetVisible(&this->mCur18B, 0);
            return;
        }
        playUISound(5);
        return;
    }

    // First open: mark active and refresh the list.
    if (CSysWin_getUnk34(&this->sysWin) != 0) return;
    this->unk1500 = 1;
    this->unk1501 = 0;
    this->unk1502 = 0;
    refreshCrystalBoxCursors();
    rebuildCrystalGridTable();
    playUISound(2);
}

// Retail 0x80218878: return the current crystal-state action id (0x33-0x36)
// or 0x30+active depending on which sub-view / flag is pending.
// The trailing `0x30 + (field_14A0 != 0)` is the -O4,s setnz idiom
// (subic/subfe); the unit -O4,s flag selects that codegen.
int CMCCrystalBox::getCrystalBoxAction() {
    if (CSysWin_getUnk34(&this->sysWin) != 0) return 0x0;
    if (this->sortMenu.IsOpen() != 0) return 0x32;
    if (this->unk1500 != 0) return 0x36;
    if ((s8)this->unk2CD == -1) return 0x35;
    if ((s8)this->unk2CD == -2) return 0x34;
    u8 idx = this->unk2CC + (u8)this->unk2CD * 10;
    if (MakeCrystal_GetFlag(&this->data, idx) != 0) return 0x33;
    return 0x30 + (this->field_14A0 != 0);
}

void CMCCrystalBox::deselectCrystalBoxItem() {
    if (CSysWin_getUnk34(&this->sysWin) != 0) return;
    if (this->unk1500 != 0) return;
    if (this->unk2CE != 0) return;
    if (this->unk2D1 != 0) return;
    if (this->field_14A0 != 0) return;
    this->unk2CD = -1;
    refreshCrystalBoxCursors();
    setCrystalNameLabel();
    rebuildCrystalGridTable();
    playUISound(0x2);
}

// Retail 0x802189D4: when sub-object 6's animation finishes, enter state 2
// and play back animation 7.
__declspec(noinline) void CMCCrystalBox::onCrystalAnimToState2() {
    if (advanceAnimTransform(this->mAnim0, lbl_eu_80668470) != 0) {
        this->unk64 = 2;
        playCrystalAnim7();
    }
}

__declspec(noinline) void CMCCrystalBox::onCrystalAnimToState3() {
    if (advanceAnimTransform(this->mAnim1, lbl_eu_80668470) == 0) return;
    this->unk64 = 3;
    playCrystalAnim8();
    this->unk69 = 1;
    Cur_SetVisible(&this->mCur07, 1);
    Cur_SetVisible(&this->mCur09, 1);
    refreshCrystalBoxCursors();
}

// Retail 0x80218A94: when sub-object 7's animation finishes, play back
// animation 6 and enter state 5.
__declspec(noinline) void CMCCrystalBox::onCrystalAnimToState5() {
    if (AnimRewindFrame(this->mAnim1, lbl_eu_80668470) != 0) {
        playCrystalAnim6();
        this->unk64 = 5;
    }
}

__declspec(noinline) void CMCCrystalBox::onCrystalAnimToIdle() {
    if (AnimRewindFrame(this->mAnim0, lbl_eu_80668470) == 0) return;
    this->unk69 = 1;
    this->unk64 = 0;
    Cur_SetVisible(&this->mCur07, 0);
}

__declspec(noinline) void CMCCrystalBox::onCrystalAnim14Idle() {
    if (advanceAnimTransform(this->mAnim8, lbl_eu_80668470) == 0) return;
    this->unk69 = 1;
    this->unk64 = 0;
    Cur_SetVisible(&this->mCur07, 0);
}

// Retail 0x80218B90: when sub-object 9's animation finishes, enter state 9
// and play back animation 10.
__declspec(noinline) void CMCCrystalBox::onCrystalAnimToState9() {
    if (advanceAnimTransform(this->mAnim3, lbl_eu_80668470) != 0) {
        this->unk64 = 9;
        playCrystalAnim10();
    }
}

__declspec(noinline) void CMCCrystalBox::onCrystalAnim9ToState3() {
    if (advanceAnimTransform(this->mAnim4, lbl_eu_80668470) == 0) return;
    this->unk69 = 1;
    refreshCrystalBoxCursors();
    this->unk64 = 3;
}

// Retail 0x80218C30: when sub-object 11's animation finishes, enter state 3.
__declspec(noinline) void CMCCrystalBox::onCrystalAnim11State3() {
    if (advanceAnimTransform(this->mAnim5, lbl_eu_80668470) != 0) {
        this->unk64 = 3;
    }
}

__declspec(noinline) void CMCCrystalBox::onCrystalAnim12State3() {
    if (advanceAnimTransform(this->mAnim6, lbl_eu_80668470) == 0) return;
    this->unk64 = 3;
    this->mAnim6->SetFrame(lbl_eu_8066845C);
}

// Retail 0x80218CC4: when sub-object 13's animation finishes, enter state 3.
__declspec(noinline) void CMCCrystalBox::onCrystalAnim13State3() {
    if (advanceAnimTransform(this->mAnim7, lbl_eu_80668470) != 0) {
        this->unk64 = 3;
    }
}

__declspec(noinline) void CMCCrystalBox::onCrystalAnimToState14() {
    if (AnimRewindFrame(this->mAnim4, lbl_eu_80668470) == 0) return;
    this->unk64 = 0xe;
    playCrystalAnim9();
}

__declspec(noinline) void CMCCrystalBox::onCrystalAnimResetState3() {
    if (AnimRewindFrame(this->mAnim3, lbl_eu_80668470) == 0) return;
    this->unk69 = 1;
    this->data.current = 0;   // +0x13DC
    refreshCrystalBoxName();
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->Animate(0);
    playCrystalAnim8();
    refreshCrystalBoxCursors();
    Cur_SetVisible(&this->mCur09, 1);
    this->unk64 = 3;
}

// Retail 0x80216F8C: (re)load the crystal box after its archive lands.
// Gated on the layout being live and a flag inside the loaded resource;
// refreshes the crystal-state row, re-inits the box-head scratch (the
// same helper the ctor uses at +0x1480), copies the scratch into the
// page-slot tables, and enters the visible state.
void CMCCrystalBox::loadCrystalBoxArchive() {
    // First two gates short-circuit; the BDAT gate uses the goto-body /
    // goto-end split (CCol6System::cbRenderBefore precedent) for retail's
    // [cmp][bne body][b exit][body] layout.
    if (this->mLayout == 0) goto end;
    if (this->mArcAcc1 == 0) goto end;
    if (lbl_eu_806646D0 != 0) goto reload;
    goto end;
end:
    return;
reload:;
    CrystalBoxScratch scr;
    BuildSlotItemLists(1);
    initCrystalPageLabels();
    MakeCrystal_CollectByTarget(&this->data, this->field_14EC[(s8)this->field_14F2]);

    initCrystalBoxScratch(&scr);

    // Struct-wrapped assignments: MWCC lowers these to the rolled
    // mtctr/bdnz word-pair loops retail emits (base-4 pointer walk).
    {
        struct BoxWords { u32 w[8]; };
        struct PageSlots { u16 w[0x20]; };
        *(BoxWords*)((u8*)this + 0x1480) = *(BoxWords*)&scr.zeros[0];
        this->field_14A0 = scr.count;
        // Double-buffer both fill words before storing (retail r6/r3 pairing).
        u32 loFill = *(u32*)&scr.fill[0];
        u32 hiFill = *(u32*)&scr.fill[4];
        *(u32*)&this->pad_14A1[4] = hiFill;
        *(u32*)&this->pad_14A1[0] = loFill;
        this->pad_14A1[8] = scr.flag29;
        *(PageSlots*)&this->field_14AA[0] = *(PageSlots*)&scr.pageSrc[0];
    }
    this->field_14EA = scr.flag6A;

    refreshCrystalBoxName();
    placeCrystalBoxSlots();
    this->unk68 = 1;
    this->unk60 = 1;
}

// Retail 0x80218EF0: resolve the message texture for one crystal slot and
// publish it into the box. When the slot holds an item, its kind picks the
// message id; otherwise the crystal id is looked up through the BDAT table.
// stmw r27 frame is the -O4,s shape (unit flag).
__declspec(noinline) void CMCCrystalBox::setCrystalSlotTexture(u16 id, unsigned int* item, u8 i) {
    // Retail materialises the null-checked item into a saved register via a
    // conditional move (cmpwi on the param reg + mr/li pair) before the gate.
    unsigned int* it = item != 0 ? item : 0;
    const char* msg = 0;
    if (it != 0) {
        CItemImplFacade3* inst = (CItemImplFacade3*)CItem_initItemImplInstances(it);
        if (inst->GetBoxed(it) == 0) {
            if (IsSkillItem(it) != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc1)
                          ->GetResource(0x74696D67, MakeTplNameSysFile(0x155), 0);
            } else {
                const char* m = 0;
                switch ((((const CrystalItemHead*)it)->flag07 >> 2) & 0x3F) {
                case 4: m = MakeTplNameSysFile(0x144); break;
                case 5: m = MakeTplNameSysFile(0x145); break;
                case 6: m = MakeTplNameSysFile(0x146); break;
                case 7: m = MakeTplNameSysFile(0x147); break;
                case 8: m = MakeTplNameSysFile(0x148); break;
                case 9: m = MakeTplNameSysFile(0x149); break;
                }
                if (m != 0) {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              this->mArcAcc1)
                          ->GetResource(0x74696D67, m, 0);
                } else {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              this->mArcAcc0)
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
                }
            }
        } else {
            const char* m = 0;
            switch ((((const CrystalItemHead*)it)->flag07 >> 2) & 0x3F) {
            case 4: m = MakeTplNameSysFile(0x144); break;
            case 5: m = MakeTplNameSysFile(0x145); break;
            case 6: m = MakeTplNameSysFile(0x146); break;
            case 7: m = MakeTplNameSysFile(0x147); break;
            case 8: m = MakeTplNameSysFile(0x148); break;
            case 9: m = MakeTplNameSysFile(0x149); break;
            }
            if (m != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc1)
                          ->GetResource(0x74696D67, m, 0);
            } else {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc0)
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        }
    }
    if (msg == 0) {
        if (id != 0) {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      this->mArcAcc1)
                      ->GetResource(
                          0x74696D67,
                          MakeTplNameSysFile((u16)BdatGetU16Direct(lbl_eu_806640EC,
                                                       lbl_eu_8050888C + 0xbc,
                                                       id)),
                          0);
            if (msg == 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc0)
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        } else {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      this->mArcAcc0)
                      ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
        }
    }
    if (msg != 0) {
        char buf[0x20];
        sprintf(buf, lbl_eu_8050888C + 0xc6, i + 1);
        PaneSetTexPaletteByName((nw4r::lyt::Layout*)this->mLayout, (const char*)buf, (TPLPalette*)((char*)msg));
    }
}

// Retail 0x8021928C: mirror of setCrystalSlotTexture with the countdown pane label
// at a different offset. stmw r27 frame is the -O4,s shape (unit flag).
void CMCCrystalBox::setCrystalSlotTexAlt(u16 id, unsigned int* item, u8 i) {
    unsigned int* it = item != 0 ? item : 0;
    const char* msg = 0;
    if (it != 0) {
        CItemImplFacade3* inst = (CItemImplFacade3*)CItem_initItemImplInstances(it);
        if (inst->GetBoxed(it) == 0) {
            if (IsSkillItem(it) != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc1)
                          ->GetResource(0x74696D67, MakeTplNameSysFile(0x155), 0);
            } else {
                const char* m = 0;
                switch ((((const CrystalItemHead*)it)->flag07 >> 2) & 0x3F) {
                case 4: m = MakeTplNameSysFile(0x144); break;
                case 5: m = MakeTplNameSysFile(0x145); break;
                case 6: m = MakeTplNameSysFile(0x146); break;
                case 7: m = MakeTplNameSysFile(0x147); break;
                case 8: m = MakeTplNameSysFile(0x148); break;
                case 9: m = MakeTplNameSysFile(0x149); break;
                }
                if (m != 0) {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              this->mArcAcc1)
                              ->GetResource(0x74696D67, m, 0);
                } else {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              this->mArcAcc0)
                              ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
                }
            }
        } else {
            const char* m = 0;
            switch ((((const CrystalItemHead*)it)->flag07 >> 2) & 0x3F) {
            case 4: m = MakeTplNameSysFile(0x144); break;
            case 5: m = MakeTplNameSysFile(0x145); break;
            case 6: m = MakeTplNameSysFile(0x146); break;
            case 7: m = MakeTplNameSysFile(0x147); break;
            case 8: m = MakeTplNameSysFile(0x148); break;
            case 9: m = MakeTplNameSysFile(0x149); break;
            }
            if (m != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc1)
                          ->GetResource(0x74696D67, m, 0);
            } else {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc0)
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        }
    }
    if (msg == 0) {
        if (id != 0) {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      this->mArcAcc1)
                      ->GetResource(
                          0x74696D67,
                          MakeTplNameSysFile((u16)BdatGetU16Direct(lbl_eu_806640EC,
                                                       lbl_eu_8050888C + 0xbc,
                                                       id)),
                          0);
            if (msg == 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc0)
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        } else {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      this->mArcAcc0)
                      ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
        }
    }
    if (msg != 0) {
        char buf[0x20];
        sprintf(buf, lbl_eu_8050888C + 0xd4, i + 1);
        PaneSetTexPaletteByName((nw4r::lyt::Layout*)this->mLayout, (const char*)buf, (TPLPalette*)((char*)msg));
    }
}

// Item-kind check: vtable word bits 16-19 == 9 and byte at +7 has flag 1.
__declspec(noinline) int isCrystalKindGem(void* item) {
    u32 word = ((const CrystalItemHead*)item)->word00;
    return ((word >> 16) & 0xF) == 9 && (u32)(((const CrystalItemHead*)item)->flag07 & 3) == 1;
}

// Retail 0x80219628: resolve the message texture for a crystal slot, then
// publish both the message and the remaining-capacity string. The slot-0x80
// item state picks the "in box" vs "not yet" message sets; a placed item's
// sub-count drives the capacity label. stmw r26 frame is the -O4,s shape
// (unit flag).
__declspec(noinline) void CMCCrystalBox::setCrystalSlotCapacity(u16 id, unsigned int* item, u8 i) {
    unsigned int* it = item != 0 ? item : 0;
    const char* msg = 0;
    if (it != 0) {
        CItemImplFacade3* inst = (CItemImplFacade3*)CItem_initItemImplInstances(it);
        if (inst->GetBoxed(it) == 0) {
            if (IsSkillItem(it) != 0 || isCrystalKindGem(it) != 0 ||
                ((((const CrystalItemHead*)it)->word00 >> 16) & 0xF) == 9) {
                const char* m = 0;
                switch ((u16)((CItemImplFacade3*)CItem_initItemImplInstances(it))
                            ->GetCount(it)) {
                case 1: m = MakeTplNameSysFile(0x191); break;
                case 2: m = MakeTplNameSysFile(0x190); break;
                case 3: m = MakeTplNameSysFile(0x18f); break;
                case 4: m = MakeTplNameSysFile(0x18e); break;
                case 5: m = MakeTplNameSysFile(0x18d); break;
                }
                if (m != 0) {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              this->mArcAcc1)
                              ->GetResource(0x74696D67, m, 0);
                } else {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              this->mArcAcc0)
                              ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
                }
            }
        } else {
            const char* m = 0;
            switch ((u16)((CItemImplFacade3*)CItem_initItemImplInstances(it))
                        ->GetCount(it)) {
            case 1: m = MakeTplNameSysFile(0x19c); break;
            case 2: m = MakeTplNameSysFile(0x19b); break;
            case 3: m = MakeTplNameSysFile(0x19a); break;
            case 4: m = MakeTplNameSysFile(0x199); break;
            case 5: m = MakeTplNameSysFile(0x198); break;
            }
            if (m != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc1)
                          ->GetResource(0x74696D67, m, 0);
            } else {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc0)
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        }
    }
    if (msg == 0) {
        if (id != 0) {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      this->mArcAcc1)
                      ->GetResource(
                          0x74696D67,
                          MakeTplNameSysFile((u16)BdatGetU16Direct(lbl_eu_806640EC,
                                                       lbl_eu_8050888C + 0xe2,
                                                       id)),
                          0);
            if (msg == 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc0)
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        } else {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      this->mArcAcc0)
                      ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
        }
    }
    if (msg != 0) {
        char buf[0x20];
        int i1 = i + 1;
        sprintf(buf, lbl_eu_8050888C + 0xe7, i1);
        PaneSetTexPaletteByName((nw4r::lyt::Layout*)this->mLayout, (const char*)buf, (TPLPalette*)((char*)msg));
        const char* cap = 0;
        if (it != 0) {
            u32 cnt = ((CItemImplFacade3*)CItem_initItemImplInstances(it))
                          ->GetCount(it);
            cap = BdatTouchStringCell(lbl_eu_8050888C + 0xf4, lbl_eu_8050888C + 0xfd,
                                30 - ((u8)cnt - 1));
        }
        sprintf(buf, lbl_eu_8050888C + 0x102, i1);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, buf, (char*)cap, 0);
    }
}

// Retail 0x80219A64: mirror of setCrystalSlotCapacity with the two countdown pane
// labels at different offsets. stmw r26 frame is the -O4,s shape (unit flag).
void CMCCrystalBox::setCrystalSlotCapAlt(u16 id, unsigned int* item, u8 i) {
    unsigned int* it = item != 0 ? item : 0;
    const char* msg = 0;
    if (it != 0) {
        CItemImplFacade3* inst = (CItemImplFacade3*)CItem_initItemImplInstances(it);
        if (inst->GetBoxed(it) == 0) {
            if (IsSkillItem(it) != 0 || isCrystalKindGem(it) != 0 ||
                ((((const CrystalItemHead*)it)->word00 >> 16) & 0xF) == 9) {
                const char* m = 0;
                switch ((u16)((CItemImplFacade3*)CItem_initItemImplInstances(it))
                            ->GetCount(it)) {
                case 1: m = MakeTplNameSysFile(0x191); break;
                case 2: m = MakeTplNameSysFile(0x190); break;
                case 3: m = MakeTplNameSysFile(0x18f); break;
                case 4: m = MakeTplNameSysFile(0x18e); break;
                case 5: m = MakeTplNameSysFile(0x18d); break;
                }
                if (m != 0) {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              this->mArcAcc1)
                              ->GetResource(0x74696D67, m, 0);
                } else {
                    msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                              this->mArcAcc0)
                              ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
                }
            }
        } else {
            const char* m = 0;
            switch ((u16)((CItemImplFacade3*)CItem_initItemImplInstances(it))
                        ->GetCount(it)) {
            case 1: m = MakeTplNameSysFile(0x19c); break;
            case 2: m = MakeTplNameSysFile(0x19b); break;
            case 3: m = MakeTplNameSysFile(0x19a); break;
            case 4: m = MakeTplNameSysFile(0x199); break;
            case 5: m = MakeTplNameSysFile(0x198); break;
            }
            if (m != 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc1)
                          ->GetResource(0x74696D67, m, 0);
            } else {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc0)
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        }
    }
    if (msg == 0) {
        if (id != 0) {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      this->mArcAcc1)
                      ->GetResource(
                          0x74696D67,
                          MakeTplNameSysFile((u16)BdatGetU16Direct(lbl_eu_806640EC,
                                                       lbl_eu_8050888C + 0xe2,
                                                       id)),
                          0);
            if (msg == 0) {
                msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                          this->mArcAcc0)
                          ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
            }
        } else {
            msg = (const char*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                      this->mArcAcc0)
                      ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
        }
    }
    if (msg != 0) {
        char buf[0x20];
        int i1 = i + 1;
        sprintf(buf, lbl_eu_8050888C + 0x10e, i1);
        PaneSetTexPaletteByName((nw4r::lyt::Layout*)this->mLayout, (const char*)buf, (TPLPalette*)((char*)msg));
        const char* cap = 0;
        if (it != 0) {
            u32 cnt = ((CItemImplFacade3*)CItem_initItemImplInstances(it))
                          ->GetCount(it);
            cap = BdatTouchStringCell(lbl_eu_8050888C + 0xf4, lbl_eu_8050888C + 0xfd,
                                30 - ((u8)cnt - 1));
        }
        sprintf(buf, lbl_eu_8050888C + 0x11e, i1);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, buf, (char*)cap, 0);
    }
}

// Retail 0x80219F0C: rebuild the crystal-box display. Depending on the row
// count, the per-row number panes and the countdown label are refreshed, the
// selection table is repopulated from the current page, and every slot pane
// is re-labelled with the message resources from setCrystalSlotTexture / setCrystalSlotCapacity.
// _savegpr_22 frame is the -O4,s shape (unit flag).
__declspec(noinline) void CMCCrystalBox::updateCrystalBoxRows() {
    CMCCrystalData* d = &this->data;
    // Single masked materialization of the row count (retail keeps one
    // clrlwi'd copy in a callee-saved register).
    u8 active = (d->limit != 0) ? d->limit : 1;
    // Declared topmost so MWCC's reverse-declaration stack allocation
    // reproduces retail: vec@0x8, buf2@0x18, buf@0x38, cvts@0x58/0x60.
    union {
        double d;
        u32 w[2];
    } selCvt, freeCvt;
    char buf[0x20];
    char buf2[0x20];
    float vec[3];
    if (active > 1) {
        // Row-count header pane plus the ten per-row number panes.
        setPaneVisible(((CrystalBoxSubObj5*)this->mLayout)->layout->vf_3C(
                          lbl_eu_8050888C + 0x137, 1),
                      1);
        for (u8 n = 0; n < 10; n++) {
            sprintf(buf, lbl_eu_8050888C + 0x13e, n + 1);
            setPaneVisible(((CrystalBoxSubObj5*)this->mLayout)->layout->vf_3C(
                              buf, 1),
                          n < active);
        }
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)this->mLayout,
                      lbl_eu_8050888C + 0x14d, (u8)(d->current + 1));
        nw4r::lyt::Pane* pane =
            ((CrystalBoxSubObj5*)this->mLayout)->layout->vf_3C(
                lbl_eu_8050888C + 0x156, 1);
        vec[0] = this->field_14F4;
        vec[1] = this->field_14F8;
        vec[2] = this->field_14FC;
        // Slide the highlight one row step per selected crystal and up one
        // step per unused row. The int -> double conversions go through the
        // retail sdata2 magic constants (signed path 2^52+2^31 for the
        // current row, unsigned path 2^52 for the free-row count).
        selCvt.w[0] = 0x43300000u;
        selCvt.w[1] = (u32)((u8)(d->current + 1) - 1) ^ 0x80000000u;
        freeCvt.w[0] = 0x43300000u;
        freeCvt.w[1] = (u32)(u8)(10 - active);
        double selStep = (selCvt.d - lbl_eu_80668468) * lbl_eu_8066847C;
        double freeStep = (freeCvt.d - lbl_eu_80668460) * lbl_eu_8066847C;
        vec[0] = vec[0] + (selStep + freeStep);
        copyVEC3(&((PaneTranslateMirror*)pane)->mX, vec);
    } else {
        setPaneVisible(((CrystalBoxSubObj5*)this->mLayout)->layout->vf_3C(
                          lbl_eu_8050888C + 0x137, 1),
                      0);
    }
    for (u8 i = 0; i < 0x1E; i++) {
        u16 v = MakeCrystal_GetSubCategory(d, i);
        unsigned int* obj =
            reinterpret_cast<unsigned int*>(MakeCrystal_GetItemObject(d, i));
        setCrystalSlotTexture(v, obj, i);
        setCrystalSlotCapacity(v, obj, i);
        u8 i1 = i + 1;
        sprintf(buf2, lbl_eu_8050888C + 0x164, i1);
        // Unsigned counts: retail compares the slot state with cmplw.
        // Inline slot-state call keeps the result out of a callee-saved
        // register (retail passes it straight through).
        setPaneVisible(((CrystalBoxSubObj5*)this->mLayout)->layout->vf_3C(
                          buf2, 1),
                      MakeCrystal_GetFlag(d, i));
        // Grey out slots whose crystal is not among the currently held page
        // slots (field_14AA); the second state call returning 0 means the
        // slot is empty this page.
        u8 found = 0;
        int cnt2 = MakeCrystal_GetFlag(d, i);
        if (cnt2 == 0) {
            u8 k = 0;
            while (k < this->field_14EA) {
                u16 id2 =
                    (k < this->field_14EA) ? this->field_14AA[k] : 0;
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
        setPaneVisible(((CrystalBoxSubObj5*)this->mLayout)->layout->vf_3C(
                          buf2, 1),
                      found);
        sprintf(buf2, lbl_eu_8050888C + 0x17f, i1);
        setPaneVisible(((CrystalBoxSubObj5*)this->mLayout)->layout->vf_3C(
                          buf2, 1),
                      found);
    }
}

// Retail 0x8021A7F4: refresh the sort-menu label from the current crystal id.
// stmw r28 frame + check-at-top loop are the -O4,s shapes (unit flag).
__declspec(noinline) void CMCCrystalBox::rebuildCrystalSortMenu() {
    int arr[3];
    const unsigned long* src = lbl_eu_80508870;
    arr[0] = *src++;
    arr[1] = *src++;
    arr[2] = *src++;
    unsigned long id = CItemBlock_getFlag120EC(9);
    int idx = (int)id & 0xFFFF;
    char* s1 = (char*)BdatTouchStringCell(lbl_eu_8050888C + 0xf4, lbl_eu_8050888C + 0xfd, arr[idx]);
    LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout,
                  lbl_eu_8050888C + 0x1ca, s1, 0);
    if (this->sortMenu.IsReady() != 0) {
        this->sortMenu.Clear();
        u8 i = 0;
        while (1) {
            if (arr[i] <= 0) break;
            this->sortMenu.Add(
                          (char*)BdatTouchStringCell(lbl_eu_8050888C + 0xf4,
                                        lbl_eu_8050888C + 0xfd, arr[i]));
            i++;
        }
        this->sortMenu.SetSelect(this->field_1506 + this->field_1507);
    }
}

// Retail 0x8021A8D8: walk the 8-slot selection table at +0x1480 and feed each
// entry (id = high 16 bits of the first word) to the two placement handlers.
__declspec(noinline) void CMCCrystalBox::placeCrystalBoxSlots() {
    unsigned int i;
    for (i = 0; i < 8; i++) {
        unsigned int* item = (unsigned int*)getSelectTableWord((unsigned long*)((u8*)this + 0x1480), (unsigned char)i);
        unsigned int raw;
        if (item != 0) {
            raw = item[0] >> 20;
        } else {
            raw = 0;
        }
        unsigned short id = (unsigned short)raw;
        setCrystalSlotTexAlt(id, item, (unsigned char)i);
        setCrystalSlotCapAlt(id, item, (unsigned char)i);
    }
}

// Retail 0x8021A384: refresh the crystal-box labels and sub-cursors. When
// the sort grid is active (unk1500), the grid cursor row is formatted into
// the +0xE4 cursor; otherwise the sort-menu / countdown / normal paths
// format a pane pair and drive the +0x6C/+0xCC cursor; the window-kind
// (unk2D1) paths scale a quad by a pane float and drive +0xB4/+0xCC.
// _savegpr_29 frame is the -O4,s shape (unit flag).
__declspec(noinline) void CMCCrystalBox::refreshCrystalBoxCursors() {
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
    struct PaneW44 { u8 pad[0x44]; f32 w44; };  // +0x44 scale float (per-statement view)
    if (this->unk1500 != 0) {
        MCCrystal_AlignPane(buf2C, &this->crystalInfo, this->unk1501);
        this->mCur18B.setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf2C));
        Cur_SetVisible(&this->mCur18B, 1);
        Cur_SetVisible(&this->mCur07, 0);
        Cur_SetVisible(&this->mCur16, 0);
        Cur_SetVisible(&this->mCur11, 0);
    } else {
        Cur_SetVisible(&this->mCur18B, 0);
        if (this->unk2D1 == 0) {
            if ((s8)this->unk2CD == -2) {
                // sort-menu path: page label + the two panes into +0xCC.
                sprintf(bufD0, lbl_eu_8050888C + 0x197);
                nw4r::lyt::Pane* paneA = (((CrystalBoxSubObj5*)this->mLayout)->layout)
                                             ->vf_3C(bufD0, 1);
                nw4r::lyt::Pane* paneB = (((CrystalBoxSubObj5*)this->mLayout)->layout)
                                             ->vf_3C(lbl_eu_8050888C + 0x63, 1);
                ScalePaneWorldPos((nw4r::math::VEC3*)buf20, paneA, paneB, (nw4r::lyt::Pane*)(((CrystalBoxSubObj5*)this->mLayout)->layout));
                this->mCur16.setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf20));
                Cur_SetVisible(&this->mCur16, 1);
                Cur_SetVisible(&this->mCur07, 0);
            } else if ((s8)this->unk2CD == -1) {
                // countdown path: ticker + 1 into +0x6C.
                sprintf(bufB0, lbl_eu_8050888C + 0x1a2, (s8)this->field_14F2 + 1);
                nw4r::lyt::Pane* paneA = (((CrystalBoxSubObj5*)this->mLayout)->layout)
                                             ->vf_3C(bufB0, 1);
                nw4r::lyt::Pane* paneB = (((CrystalBoxSubObj5*)this->mLayout)->layout)
                                             ->vf_3C(lbl_eu_8050888C + 0x63, 1);
                ScalePaneWorldPos((nw4r::math::VEC3*)buf14, paneA, paneB, (nw4r::lyt::Pane*)(((CrystalBoxSubObj5*)this->mLayout)->layout));
                this->mCur07.setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf14));
                Cur_SetVisible(&this->mCur07, 1);
                Cur_SetVisible(&this->mCur16, 0);
            } else {
                // normal path: the current crystal index + 1 into +0x6C.
                sprintf(buf90, lbl_eu_8050888C + 0xe7,
                        (s8)this->unk2CC + (s8)this->unk2CD * 10 + 1);
                nw4r::lyt::Pane* paneA = (((CrystalBoxSubObj5*)this->mLayout)->layout)
                                             ->vf_3C(buf90, 1);
                nw4r::lyt::Pane* paneB = (((CrystalBoxSubObj5*)this->mLayout)->layout)
                                             ->vf_3C(lbl_eu_8050888C + 0x63, 1);
                ScalePaneWorldPos((nw4r::math::VEC3*)buf8, paneA, paneB, (nw4r::lyt::Pane*)(((CrystalBoxSubObj5*)this->mLayout)->layout));
                this->mCur07.setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf8));
                Cur_SetVisible(&this->mCur07, 1);
                Cur_SetVisible(&this->mCur16, 0);
            }
        } else {
            if ((s8)this->unk2D3 < 2) {
                // window-kind counter path: scale the quad by the pane float.
                sprintf(buf70, lbl_eu_8050888C + 0x1ae, (s8)this->unk2D3 + 1);
                nw4r::lyt::Pane* paneA = (((CrystalBoxSubObj5*)this->mLayout)->layout)
                                             ->vf_3C(buf70, 1);
                AccumPaneWorldPos((nw4r::math::VEC3*)((char*)buf44), paneA);
                nw4r::lyt::Pane* paneB = (((CrystalBoxSubObj5*)this->mLayout)->layout)
                                             ->vf_3C(lbl_eu_8050888C + 0x63, 1);
                buf44[0] *= ((PaneW44*)paneB)->w44;
                this->mCur11.setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf44));
                Cur_SetVisible(&this->mCur11, 1);
                Cur_SetVisible(&this->mCur16, 0);
                flag = 0;
                if (code80135FDC_getByte_64077() > 2 || (s8)this->unk2D3 != 1) {
                    flag = 1;
                }
                Cur_ShowTwoPanes(&this->mCur11, flag);
            } else {
                // window-kind done path: +0xCC cursor.
                sprintf(buf50, lbl_eu_8050888C + 0x1bd);
                nw4r::lyt::Pane* paneA = (((CrystalBoxSubObj5*)this->mLayout)->layout)
                                             ->vf_3C(buf50, 1);
                AccumPaneWorldPos((nw4r::math::VEC3*)((char*)buf38), paneA);
                nw4r::lyt::Pane* paneB = (((CrystalBoxSubObj5*)this->mLayout)->layout)
                                             ->vf_3C(lbl_eu_8050888C + 0x63, 1);
                buf38[0] *= ((PaneW44*)paneB)->w44;
                this->mCur16.setRootPaneTranslate(reinterpret_cast<const nw4r::math::VEC3*>(buf38));
                Cur_SetVisible(&this->mCur16, 1);
                Cur_SetVisible(&this->mCur11, 0);
            }
        }
    }
}

struct CMCCrystalRec5 { unsigned short m0; unsigned short m2; unsigned char m4; };

struct CMCCrystalRec5Ex {
    unsigned short a;
    unsigned short b;
    unsigned char c;
};

// Retail keeps these tiny record copies out-of-line (every merge/sort step is
// a bl); C linkage carries the unmangled retail symbol names.
extern "C" __declspec(noinline) void copyCrystalRec5(CMCCrystalRec5* dst, const CMCCrystalRec5* src) {
    *dst = *src;
}

extern "C" __declspec(noinline) void copyCrystalRec5Ex(CMCCrystalRec5Ex* dst, CMCCrystalRec5Ex* src) {
    unsigned short a = src->a;
    unsigned short b = src->b;
    unsigned char c = src->c;
    dst->a = a;
    dst->b = b;
    dst->c = c;
}


// Retail 0x80218B10: rebuild the 8x8 crystal-grid aggregation table. Clears a
// 32-slot transient table of {id, qty, flag} records, merges every item found
// in the 8 page slots (name id via vtable slot 0x4C, quantity via 0x64) plus
// the currently selected crystal (when unk2CD >= 0), bubble-sorts by quantity
// then by flag, and pushes the first 8 entries (offset by the scroll row
// unk1502, highlighting the cursor row unk1501) into the crystal-info pane.
__declspec(noinline) void CMCCrystalBox::rebuildCrystalGridTable() {
    // Declaration order fixes the stack-slot layout (MWCC allocates locals in
    // reverse declaration order): sort2 temps C/B/A at +0x08/+0x10/+0x40,
    // sort1 temps C/B/A at +0x18/+0x20/+0x48, append temps at +0x28/+0x30,
    // fill tmp at +0x38, table at +0x50.
    CMCCrystalRec5 table[32];
    CMCCrystalRec5Ex sort1A;
    CMCCrystalRec5Ex sort2A;
    CMCCrystalRec5 tmp;
    CMCCrystalRec5 pageTmp;
    CMCCrystalRec5 selTmp;
    CMCCrystalRec5Ex sort1B;
    CMCCrystalRec5Ex sort1C;
    CMCCrystalRec5Ex sort2B;
    CMCCrystalRec5Ex sort2C;
    u8 count = 0;
    u32 flag = 0;
    // Shared loop state: retail reuses one register per role across every
    // phase (walk/fill index, page+sort j, merge k, obj, id/qty), so declare
    // them once here to mirror the liveness pattern.
    CMCCrystalRec5* z;
    CMCCrystalRec5* e;
    unsigned long* slots;
    u8 i;
    u8 j;
    u8 k;
    void* obj;
    CItemImplFacadeGrid* inst;
    u16 id;
    int qty;
    int found;

    // Rolling-pointer zero walk over the whole array (retail shape).
    z = table;
    do {
        z->m0 = 0;
        z->m2 = 0;
        z->m4 = 0;
        ++z;
    } while (z < table + 32);
    for (i = 0; i < 32; i++) {
        tmp.m0 = 0;
        tmp.m2 = 0;
        tmp.m4 = flag;
        copyCrystalRec5(&table[i], &tmp);
    }

    // Merge items from all 8 page slots (this+0x1480 sub-table).
    slots = (unsigned long*)((u8*)this + 0x1480);
    for (j = 0; j < 8; j++) {
        obj = (void*)getSelectTableWord(slots, j);
        if (obj == 0 || *(void**)obj == 0) {
            continue;
        }
        for (k = 0; k < 4; k++) {
            inst = (CItemImplFacadeGrid*)CItem_initItemImplInstances(obj);
            id = inst->GetName(obj, k);
            if (id != 0) {
                inst = (CItemImplFacadeGrid*)CItem_initItemImplInstances(obj);
                qty = inst->GetQuantity(obj, k);
                // Merge into an existing record with the same id, else append.
                found = 0;
                for (u8 m = 0; m < count; m++) {
                    if (table[m].m0 == id) {
                        table[m].m2 += qty;
                        found = 1;
                        break;
                    }
                }
                if (found == 0) {
                    pageTmp.m2 = qty;
                    pageTmp.m0 = id;
                    pageTmp.m4 = flag;
                    copyCrystalRec5(&table[count], &pageTmp);
                    count++;
                }
            }
        }
    }

    // Merge the selected crystal (row unk2CD, column unk2CC); its records are
    // marked with flag=1 so they sort after the plain aggregates.
    if ((s8)this->unk2CD >= 0) {
        CMCCrystalData* d = &this->data;
        u8 idx = this->unk2CC + this->unk2CD * 10;
        if (MakeCrystal_GetFlag(d, idx) == 0) {
            obj = MakeCrystal_GetItemObject(d, idx);
            if (obj != 0 && *(void**)obj != 0) {
                flag = 1;
                for (k = 0; k < 4; k++) {
                    inst = (CItemImplFacadeGrid*)CItem_initItemImplInstances(obj);
                    id = inst->GetName(obj, k);
                    if (id != 0) {
                        inst = (CItemImplFacadeGrid*)CItem_initItemImplInstances(obj);
                        qty = inst->GetQuantity(obj, k);
                        found = 0;
                        for (u8 m = 0; m < count; m++) {
                            if (table[m].m0 == id) {
                                table[m].m2 += qty;
                                table[m].m4 = flag;
                                found = 1;
                                break;
                            }
                        }
                        if (found == 0) {
                            selTmp.m2 = qty;
                            selTmp.m0 = id;
                            selTmp.m4 = flag;
                            copyCrystalRec5(&table[count], &selTmp);
                            count++;
                        }
                    }
                }
            }
        }
    }

    // Bubble sort by quantity (descending), then by flag byte.
    u8 limit = count - 1;
    for (i = 0; i < limit; i++) {
        for (j = 0; j < limit - i; j++) {
            CMCCrystalRec5Ex* pa = (CMCCrystalRec5Ex*)&table[j];
            CMCCrystalRec5Ex* pb = (CMCCrystalRec5Ex*)&table[j + 1];
            if (pa->b < pb->b) {
                copyCrystalRec5Ex(&sort1A, pa);
                copyCrystalRec5Ex(&sort1B, pb);
                copyCrystalRec5((CMCCrystalRec5*)pa, (CMCCrystalRec5*)&sort1B);
                copyCrystalRec5Ex(&sort1C, &sort1A);
                copyCrystalRec5((CMCCrystalRec5*)pb, (CMCCrystalRec5*)&sort1C);
            }
        }
    }
    for (i = 0; i < limit; i++) {
        for (j = 0; j < limit - i; j++) {
            CMCCrystalRec5Ex* pa = (CMCCrystalRec5Ex*)&table[j];
            CMCCrystalRec5Ex* pb = (CMCCrystalRec5Ex*)&table[j + 1];
            if (pa->c < pb->c) {
                copyCrystalRec5Ex(&sort2A, pa);
                copyCrystalRec5Ex(&sort2B, pb);
                copyCrystalRec5((CMCCrystalRec5*)pa, (CMCCrystalRec5*)&sort2B);
                copyCrystalRec5Ex(&sort2C, &sort2A);
                copyCrystalRec5((CMCCrystalRec5*)pb, (CMCCrystalRec5*)&sort2C);
            }
        }
    }

    // Push up to 8 rows (starting at the scroll offset unk1502) into the info
    // pane, recording the id under the cursor row unk1501.
    MCCrystal_ClearSlots(&this->crystalInfo);
    this->unk1504 = 0;
    for (i = 0; i < 8; i++) {
        int idx = i + (s8)this->unk1502;
        if (idx >= (int)count) {
            break;
        }
        e = &table[idx];
        if ((int)i == (int)(s8)this->unk1501) {
            this->unk1504 = e->m0;
        }
        MCCrystal_FillSlot(&this->crystalInfo, i, e->m0, e->m2, e->m4);
    }
    this->mScrollBar.UpdateThumb(8, count);
    this->mScrollBar.PlaceThumb((u16)(s8)this->unk1502);
    this->unk1503 = count;
}

// CMCCrystalBox holds a nw4r::lyt::Layout* at +0x38 (== mLayout) with the
// anim transform to play at +0x3C..+0x5F. These play back a bound animation.
__declspec(noinline) void CMCCrystalBox::playCrystalAnim6() {
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->BindAnimation(this->mAnim0);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->SetAnimationEnable(this->mAnim0, true);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->Animate(0);
}

__declspec(noinline) void CMCCrystalBox::playCrystalAnim7() {
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->BindAnimation(this->mAnim1);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->SetAnimationEnable(this->mAnim1, true);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->Animate(0);
}

__declspec(noinline) void CMCCrystalBox::playCrystalAnim8() {
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->BindAnimation(this->mAnim2);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->SetAnimationEnable(this->mAnim2, true);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->Animate(0);
}

__declspec(noinline) void CMCCrystalBox::playCrystalAnim9() {
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->BindAnimation(this->mAnim3);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->SetAnimationEnable(this->mAnim3, true);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->Animate(0);
}

__declspec(noinline) void CMCCrystalBox::playCrystalAnim10() {
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->BindAnimation(this->mAnim4);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->SetAnimationEnable(this->mAnim4, true);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->Animate(0);
}

void CMCCrystalBox::playCrystalAnim11() {
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->BindAnimation(this->mAnim5);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->SetAnimationEnable(this->mAnim5, true);
    this->mAnim5->SetFrame(lbl_eu_8066845C);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->Animate(0);
}

__declspec(noinline) void CMCCrystalBox::playCrystalAnim12() {
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->BindAnimation(this->mAnim6);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->SetAnimationEnable(this->mAnim6, true);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->Animate(0);
}

void CMCCrystalBox::playCrystalAnim13() {
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->BindAnimation(this->mAnim7);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->SetAnimationEnable(this->mAnim7, true);
    this->mAnim7->SetFrame(lbl_eu_8066845C);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->Animate(0);
}

__declspec(noinline) void CMCCrystalBox::playCrystalAnim14() {
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->BindAnimation(this->mAnim8);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->SetAnimationEnable(this->mAnim8, true);
    reinterpret_cast<nw4r::lyt::Layout*>(this->mLayout)->Animate(0);
}

// Retail 0x8021B344: refresh the window-kind counter labels. When unk2D3 is
// 0 the current counter value formats two numeric panes plus a message;
// when it is 1, three window rows ((value-1)*3 + 0x40..0x42) are formatted,
// the crystal name is resolved through the BDAT table, a rank pane is chosen
// from the id thresholds, and a scale/position pane is sprintf'd with id/30
// and id/20. _savegpr_28 frame is the -O4,s shape (unit flag).
__declspec(noinline) void CMCCrystalBox::showCrystalBoxStatus() {
    if ((s8)this->unk2D3 == 0) {
        int v = GetCollectedFlagByte(this->unk2D4);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x1e6,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, (u8)v + 10), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x1f5,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, (u8)v + 18), 0);
        char* msg = MakeTplNameSysFile((u16)BdatGetU16Direct(lbl_eu_80664090, lbl_eu_8050888C + 0x204, (u8)v));
        void* res = (void*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(this->mArcAcc0)
                        ->GetResource(0x74696D67, msg, 0);
        if (res != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x20f, (TPLPalette*)res);
        }
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x21c,
                      lbl_eu_8050888C + 0x22b, 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x22c,
                      lbl_eu_8050888C + 0x22b, 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x23b,
                      lbl_eu_8050888C + 0x22b, 0);
        void* res2 = (void*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(this->mArcAcc0)
                         ->GetResource(0x74696D67, lbl_eu_8050888C + 0xa9, 0);
        if (res2 != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x24a, (TPLPalette*)res2);
        }
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x257,
                      lbl_eu_8050888C + 0x22b, 0);
    } else if ((s8)this->unk2D3 == 1) {
        int v = GetCollectedFlagByte(this->unk2D5);
        int row = ((u8)v - 1) * 3;
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x21c,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, row + 0x40), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x22c,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, row + 0x41), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x23b,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, row + 0x42), 0);
        char* msg = MakeTplNameSysFile((u16)BdatGetU16Direct(lbl_eu_80664090, lbl_eu_8050888C + 0x204, (u8)v));
        void* res = (void*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(this->mArcAcc0)
                        ->GetResource(0x74696D67, msg, 0);
        if (res != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x24a, (TPLPalette*)res);
        }
        u8 d4 = (u8)GetCollectedFlagByte(this->unk2D4);
        u8 d5 = (u8)GetCollectedFlagByte(this->unk2D5);
        u16 id = LookupRelateValue(d4, d5);
        // u16-param view: retail emits clrlwi 16 on the id argument here.
        setLayoutTextBoxNumber__FPQ34nw4r3lyt6LayoutPcUc((nw4r::lyt::Layout*)this->mLayout,
                                                lbl_eu_8050888C + 0x257, id);
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
        void* res2 = (void*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(this->mArcAcc0)
                         ->GetResource(0x74696D67, s, 0);
        if (res2 != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x2cd, (TPLPalette*)res2);
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
        void* res3 = (void*)reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(this->mArcAcc0)
                         ->GetResource(0x74696D67, s2, 0);
        if (res3 != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x351, (TPLPalette*)res3);
        }
        // Countdown pane: scale id into a screen coordinate, then format
        // id/30 and id/20 into the +0x365 label. Manual uint->double
        // conversion (the 0x43300000 / 2^52 trick) keeps lbl_eu_80668460 a
        // named import; the struct-wrapped buffer sorts before the cvt temp
        // so their stack slots match retail (sp+0x08 / sp+0x28). Divisions
        // stay after the BdatTouchStringCell call (retail order).
        struct { u8 b[0x20]; } buf;
        struct {
            u32 hi;
            u32 lo;
        } cvt;
        cvt.lo = id;
        cvt.hi = 0x43300000;
        u16 val = (u16)(lbl_eu_80668484 * (*(double*)&cvt - lbl_eu_80668460)
                        + lbl_eu_80668480);
        char* num = BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1,
                                  0x30);
        sprintf((char*)buf.b, lbl_eu_8050888C + 0x35e, (u8)(val / 30), num,
                (u8)(val / 20));
        LayoutSetTextBoxText((nw4r::lyt::Layout*)this->mLayout, lbl_eu_8050888C + 0x365,
                      (char*)buf.b, 0);
    }
}

// Retail 0x80219AF0: aggregate the placed crystal items at +0x1480 into
// (id, quantity) pairs, bubble-sort by descending quantity, and return
// (first pair count - 100). Retail's epilogue extends the result to 64 bits
// (subfc/subfze), so the prototype returns s64; call sites truncate to int.
// stmw r26 frame is the -O4,s shape (unit flag).
// Retail keeps this out-of-line (bl from the update callbacks); noinline
// preserves that.
__declspec(noinline) s64 CMCCrystalBox::getCrystalSelTopCount() {
    // Register-coloring notes (retail): z:r31 is 0 through the init pass and
    // becomes the outer loop counter; n:r30; tableBase:r29 (the init-pass
    // index k dies before tableBase is computed and shares its register);
    // item:r28; j:r27; this:r26.
    MCAggPair pairs[32];
    u8 z;
    u8 n;
    u8* tableBase;
    void* item;
    u8 j;

    // n is cleared before the zero-fill pass (retail's li r30,0 precedes the
    // sth loop).
    n = 0;

    // Zero-fill the transient pair table (pointer walk, matching retail).
    MCAggPair* p = pairs;
    do {
        p->id = 0;
        p->count = 0;
        p++;
    } while (p < &pairs[32]);

    // Redundant helper-based zero fill (retail does both passes).
    z = 0;
    for (u8 k = 0; k < 0x20; k++) {
        MCAggPair tmpInit;
        tmpInit.id = z;
        tmpInit.count = z;
        copyCrystalAggPair((CMCCrystalBoxParam*)&pairs[k], (CMCCrystalBoxParam*)&tmpInit);
    }

    tableBase = (u8*)this + 0x1480;
    for (z = 0; z < 8; z++) {
        item = (void*)getSelectTableWord((unsigned long*)tableBase, z);
        if (item == NULL || *(u32*)item == 0) continue;
        for (j = 0; j < 4; j++) {
            u16 id =
                ((CItemImplFacade2*)CItem_initItemImplInstances(item))->GetName(item, j);
            if (id > 0) {
                // Raw 32-bit result folded into the halfword stores.
                s32 cnt = ((CItemImplFacade2*)CItem_initItemImplInstances(item))
                              ->GetFlag(item, j);
                int found = 0;
                for (u8 q = 0; q < n; q++) {
                    if (pairs[q].id == id) {
                        s32 total = pairs[q].count;
                        total += cnt;
                        pairs[q].count = total;
                        found = 1;
                        break;
                    }
                }
                if (found == 0) {
                    MCAggPair tmpAdd;  // append temp
                    tmpAdd.id = id;
                    tmpAdd.count = cnt;
                    // Post-increment in the subscript: retail bumps n between
                    // the address computation and the call.
                    copyCrystalAggPair((CMCCrystalBoxParam*)&pairs[n++],
                                  (CMCCrystalBoxParam*)&tmpAdd);
                }
            }
        }
    }

    // Sort bounds stay int-typed: retail subtracts from the masked count
    // without re-masking (r6 = n-1 raw).
    int m = n - 1;
    for (u8 si = 0; si < m; si++) {
        int lim = m - si;
        for (u8 sj = 0; sj < lim; sj++) {
            if (pairs[sj].count < pairs[sj + 1].count) {
                // Halfword-wise exchange through explicit element pointers.
                MCAggPair* a = &pairs[sj];
                MCAggPair* b = &pairs[sj + 1];
                u16 tid = a->id;
                u16 tcnt = a->count;
                a->id = b->id;
                a->count = b->count;
                b->id = tid;
                b->count = tcnt;
            }
        }
    }

    return (s64)pairs[0].count - 100;
}

void copyShortPair_80219D10(void* dst, void* src){
    *(unsigned short*)dst = *(unsigned short*)src;
    *(unsigned short*)((char*)dst + 2) = *(unsigned short*)((char*)src + 2);
}

// Retail 0x8021BB7C: file-load completion callback. The box issues three async
// reads (main crystal-box layout +0x24, crystal-info arc +0x28, BDAT table
// +0x2C); when the event's handle matches one of them, the matching branch
// builds that resource and returns 1, otherwise 0.
// _savegpr_28 frame (retail) is the -O4,s shape (unit flag).
bool CMCCrystalBox::OnFileEvent(CEventFile* event) {
    CFileHandle* evt = event->mFileHandle;
    if ((CFileHandle*)mFileHandle0 == evt) {
        // --- main crystal-box layout arc ---
        mMemRegion1.createRegion(
            mtl::MemManager::getHandleMEM2(), 0x40000,
            lbl_eu_8050888C + 0x371, 0);
        Class_8045F858 host(&mMemRegion1);
        char buf[0x20];
        CFileHandle* fh = (CFileHandle*)mFileHandle0;
        void* data = fh->getData();
        mtl::MemManager::setMemInitFlag(0);
        mArcAcc0 = CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)mArcAcc0)
            ->Attach(data, lbl_eu_8050888C + 0x37f);
        buildLayout((nw4r::lyt::Layout**)&mLayout, (nw4r::lyt::ArcResourceAccessor*)mArcAcc0, lbl_eu_8050888C + 0x383);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, &mAnim0, (nw4r::lyt::ArcResourceAccessor*)mArcAcc0, lbl_eu_8050888C + 0x39a);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, &mAnim1, (nw4r::lyt::ArcResourceAccessor*)mArcAcc0, lbl_eu_8050888C + 0x3b4);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, &mAnim2, (nw4r::lyt::ArcResourceAccessor*)mArcAcc0, lbl_eu_8050888C + 0x3d3);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, &mAnim3, (nw4r::lyt::ArcResourceAccessor*)mArcAcc0, lbl_eu_8050888C + 0x3f3);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, &mAnim4, (nw4r::lyt::ArcResourceAccessor*)mArcAcc0, lbl_eu_8050888C + 0x40e);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, &mAnim5, (nw4r::lyt::ArcResourceAccessor*)mArcAcc0, lbl_eu_8050888C + 0x42d);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, &mAnim6, (nw4r::lyt::ArcResourceAccessor*)mArcAcc0, lbl_eu_8050888C + 0x44c);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, &mAnim7, (nw4r::lyt::ArcResourceAccessor*)mArcAcc0, lbl_eu_8050888C + 0x46c);
        bindLayoutAnimTransform((nw4r::lyt::Layout*)mLayout, &mAnim8, (nw4r::lyt::ArcResourceAccessor*)mArcAcc0, lbl_eu_8050888C + 0x48b);

        // Bind the shared font into the layout's root pane.
        nw4r::lyt::Pane* rootPane = *(nw4r::lyt::Pane**)((u8*)mLayout + 0x10);
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
            1, (nw4r::lyt::Layout*)mLayout);
        u32 fontHandle = ((CDeviceFontVtblView*)fontObj)->vf7();
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontHandle);
        // Row/column count labels from the two shared counters.
        u32 cnt = getPackedFont();
        if (cnt != 0) {
            setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(
                (nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x14d, cnt);
            setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(
                (nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x12e, cnt);
            setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(
                (nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x365, cnt);
        }
        u32 cnt2 = CUICfManager_getPackedFont9C();
        if (cnt2 != 0) {
            for (u8 i = 1; i <= 30; i++) {
                sprintf(buf, lbl_eu_8050888C + 0x102, i);
                setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(
                    (nw4r::lyt::Layout*)mLayout, buf, cnt2);
            }
            for (u8 i = 1; i <= 8; i++) {
                sprintf(buf, lbl_eu_8050888C + 0x11e, i);
                setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(
                    (nw4r::lyt::Layout*)mLayout, buf, cnt2);
            }
            setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(
                (nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x257, cnt2);
        }

        playCrystalAnim6();
        ((CLytVf38*)mLayout)->vf_30(0);

        // Static labels for the window-kind counter rows.
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x4a6,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 2), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x1bd,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 4), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x4b3,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 3), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x4bf,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 0x2e), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x4c8,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 0x2f), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x4d1,
                      BdatTouchStringCell(lbl_eu_8050888C + 0xf4, lbl_eu_8050888C + 0xfd, 4), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x1ca,
                      lbl_eu_8050888C + 0x22b, 0);

        // Window title: pick the message by game-manager flag, resolve the
        // timg texture and publish it onto the pane.
        const char* msgName = isClassicController__Q22cf13CfGameManagerFv(-1)
                                  ? lbl_eu_8050888C + 0x4dd
                                  : lbl_eu_8050888C + 0x4e6;
        u16 id = (u16)BdatGetU16ByTableKey(lbl_eu_8050888C + 0x4ef, msgName, 0x49);
        void* msg = MakeTplNameSysFile(id);
        void* res = ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())
                        ->GetResource(0x74696D67, (const char*)msg, 0);
        if (res != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mLayout,
                          lbl_eu_8050888C + 0x4fd, (TPLPalette*)res);
            // Mirror the timg-resource u16 position header onto the pane. The
            // double deref of res+8 rides as the vf_3C extra arg (r6) and is
            // reused as the base of the two u16 reads (retail schedule).
            u32* d = *(u32**)*(u32**)((u8*)res + 8);
            u16 w2 = *(u16*)((u8*)d + 2);
            u16 w0 = *(u16*)d;
            nw4r::lyt::Pane* pane =
                ((CLytVf3C3*)(*(void**)((u8*)mLayout + 0x10)))
                    ->vf_3C(lbl_eu_8050888C + 0x4fd, 1, d);
            if (pane != 0) {
                f32 fx;
                f32 fy;
                // Dead local pair (store-only, so MWCC keeps the two stfs to
                // +0x10/+0x14 like retail instead of eliminating them).
                u64 pos;
                ((PanePosMirror*)pane)->m4C = fx = (f32)w2;
                ((f32*)&pos)[0] = fx;
                ((PanePosMirror*)pane)->m50 = fy = (f32)w0;
                ((f32*)&pos)[1] = fy;
            }
        }

        BuildSlotItemLists(1);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x257,
                      lbl_eu_8050888C + 0x22b, 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x1e6,
                      lbl_eu_8050888C + 0x22b, 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x1f5,
                      lbl_eu_8050888C + 0x22b, 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x21c,
                      lbl_eu_8050888C + 0x22b, 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x22c,
                      lbl_eu_8050888C + 0x22b, 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x23b,
                      lbl_eu_8050888C + 0x22b, 0);

        int v = GetCollectedFlagByte(0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x509,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 9), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x516,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1, 0xa), 0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x1e6,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1,
                                    (u8)v + 10),
                      0);
        LayoutSetTextBoxFmtValue((nw4r::lyt::Layout*)mLayout, lbl_eu_8050888C + 0x1f5,
                      BdatTouchStringCell(lbl_eu_8050888C + 0x1d5, lbl_eu_8050888C + 0x1e1,
                                    (u8)v + 0x12),
                      0);

        void* msg2 = MakeTplNameSysFile(
            (u16)BdatGetU16Direct(lbl_eu_80664090, lbl_eu_8050888C + 0x204, (u8)v));
        void* res2 = ((nw4r::lyt::ArcResourceAccessor*)mArcAcc0)
                         ->GetResource(0x74696D67, (const char*)msg2, 0);
        if (res2 != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mLayout,
                          lbl_eu_8050888C + 0x20f, (TPLPalette*)res2);
        }

        // Mirror the window-size pane's translate onto field_14F4.
        nw4r::lyt::Pane* sizePane =
            ((CLytVf3C*)(*(void**)((u8*)mLayout + 0x10)))
                ->vf_3C(lbl_eu_8050888C + 0x523, 1);
        copyVEC3(&field_14F4, (u8*)sizePane + 0x2c);

        // Re-init all six cursor sub-objects from temps (accessor or the
        // shared resource manager), then poke each member's slot-0x08 virtual.
        {
            CCur07 tmpC0(mArcAcc0);
            MenuShopSellCopyCursor(&mCur07, &tmpC0);
        }
        mCur07.initLayout();

        {
            CCur09 tmpA8(mArcAcc0);
            MenuShopSellCopyCursor(&mCur09, &tmpA8);
        }
        mCur09.initLayout();

        // Two direction vectors for the +0x84 cursor's quad: build each with
        // setVec3 into a temp, then copy to the passed-by-address destination.
        nw4r::math::VEC3 s1;
        nw4r::math::VEC3 v1 = *reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
            (nw4r::math::VEC3*)((float*)&s1), lbl_eu_80668488, lbl_eu_8066848C, lbl_eu_8066845C));
        nw4r::math::VEC3 s2;
        nw4r::math::VEC3 v2 = *reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
            (nw4r::math::VEC3*)((float*)&s2), lbl_eu_80668490, lbl_eu_8066848C, lbl_eu_8066845C));
        Cur_SetTwoPanes09(&this->mCur09, &v2, &v1);

        {
            CCur18 tmp90(CUICfManager_getArcResourceAccessor());
            MenuShopSellCopyCursor(&mCur18A, &tmp90);
        }
        mCur18A.initLayout();

        {
            CCur11 tmp78(mArcAcc0);
            MenuShopSellCopyCursor(&mCur11, &tmp78);
        }
        mCur11.initLayout();

        {
            CCur16 tmp60(mArcAcc0);
            MenuShopSellCopyCursor(&mCur16, &tmp60);
        }
        mCur16.initLayout();

        {
            CCur18 tmp48(CUICfManager_getArcResourceAccessor());
            MenuShopSellCopyCursor(&mCur18B, &tmp48);
        }
        mCur18B.initLayout();

        loadCrystalBoxArchive();
        mFileHandle0 = 0;
        mMemRegion1.validateHeap();
        return true;
    }
    if ((CFileHandle*)mFileHandle1 == evt) {
        // --- crystal-info arc ---
        mMemRegion2.createRegion(
            mtl::MemManager::getHandleMEM2(), 0x100,
            lbl_eu_8050888C + 0x530, 0);
        Class_8045F858 host2(&mMemRegion2);
        CFileHandle* fh2 = (CFileHandle*)mFileHandle1;
        void* data2 = fh2->getData();
        mtl::MemManager::setMemInitFlag(0);
        mArcAcc1 = CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)mArcAcc1)
            ->Attach(data2, lbl_eu_8050888C + 0x37f);
        loadCrystalBoxArchive();
        mFileHandle1 = 0;
        mMemRegion2.validateHeap();
        return true;
    }
    if ((CFileHandle*)mFileHandle2 == evt) {
        // --- BDAT table file ---
        CFileHandle* fh3 = (CFileHandle*)mFileHandle2;
        void* data3 = fh3->getData();
        Bdat_GetTable_AA34();
        if (getFP(lbl_eu_8050888C + 0x541) == 0) {
            CBdat::setBdatEntry(2, data3);
        }
        Bdat_GetTable_AA34();
        lbl_eu_806646D0 = (unsigned long)getFP(lbl_eu_8050888C + 0x550);
        loadCrystalBoxArchive();
        mFileHandle2 = 0;
        return true;
    }
    return false;
}

// noinline: retail calls this out-of-line from getCrystalSelTopCount; without it
// MWCC's IPO folds both call sites away.
extern "C" __declspec(noinline) void copyCrystalAggPair(CMCCrystalBoxParam* dst, const CMCCrystalBoxParam* src) {
    dst->m0 = (unsigned short)src->m0;
    dst->m2 = (unsigned short)src->m2;
}
