// Auto-scaffolded catalog TU for kyoshin/CKizunaTalkList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CKizunaTalkList.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/code_80135FDC.hpp"
void bindLayoutAnimTransform(nw4r::lyt::Layout* layout, nw4r::lyt::AnimTransform** anim, nw4r::lyt::ArcResourceAccessor* accessor, char* name);
void buildLayout(nw4r::lyt::Layout** layout, nw4r::lyt::ArcResourceAccessor* accessor, const char* name);
#include "kyoshin/plugin/ocBdat.hpp"
void playUISound(u32);

// Converted to inline member function in header

extern "C" void* __ct__802726F4(TalkListEntryArray* self, u32 parent);

CKizunaTalkList::CKizunaTalkList() {
    // Retail order: region ctor, POD zeroing, cursor/scrollbar widget ctors,
    // state bytes (mNeedsRebuild = 1), entry-array ctor, then a temporary
    // CScrollbar(1) whose fields are memberwise-copied over the embedded bar.
    __ct__17UnkClass_8045F564Fv((u8*)this + 4);
    mEntryCount = 0;
    mUnknown18 = 0;
    mUnknown1C = 0;
    mpLayout20 = NULL;
    mpAnim24 = NULL;
    mpAnim28 = NULL;
    __ct__CCur18(&mCursor, NULL);
    __ct__CScrollBar(mScrollBar, 0);
    mState84 = 0;
    mState85 = 0;
    mUnknown86 = 0;
    mNeedsRebuild = 1;
    mUnknown88 = 0;
    mUnknown8A = 0;
    __ct__802726F4(&mEntryArray, 0);

    u8 tmpBuf[0x40];
    __ct__CScrollBar(reinterpret_cast<CScrollBar*>(tmpBuf), 1);
    // Member-wise copy of the temp bar's body, skipping the vtable slot.
    reinterpret_cast<CScrollBar*>(mScrollBar)->mMemRegion =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mMemRegion;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mFileHandle =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mFileHandle;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mAccessor =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mAccessor;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mLayout =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mLayout;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mAnimTransform =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mAnimTransform;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mReady =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mReady;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mVisible =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mVisible;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mState =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mState;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mActive =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mActive;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mAnimOffset =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mAnimOffset;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mScrollPosY =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mScrollPosY;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mScrollRatio =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mScrollRatio;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mThumbHeight =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mThumbHeight;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mContentHeight =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mContentHeight;
    reinterpret_cast<CScrollBar*>(mScrollBar)->mDirection =
        reinterpret_cast<CScrollBar*>(tmpBuf)->mDirection;
    __dt__10CScrollBarFv(reinterpret_cast<CScrollBar*>(tmpBuf), -1);
}

// TalkListEntry element destructor (defined below; used by the array teardown).
struct TalkListEntry;
extern "C" TalkListEntry* __dt__80272774(TalkListEntry* entry, int flags);

// Deleting destructor (retail __dt__15CKizunaTalkListFv): teardown the
// 20x0x100 TalkListEntry array at +0x8C, then the embedded sub-objects
// (scrollbar +0x44, CCur18 cursor +0x2C, UnkClass_8045F564 +0x04), and free
// self when the delete flag is set. -O4,s merges the r30/r31
// saves into the retail stmw r30 prologue.
extern "C" CKizunaTalkList* __dt__15CKizunaTalkListFv(CKizunaTalkList* self, int flags) {
    if (self != 0) {
        if ((u8*)self + 0x8C != 0) {
            __destroy_arr((u8*)self + 0x8C, (void*)__dt__80272774, 0x14, 0x100);
        }
        __dt__10CScrollBarFv((u8*)self + 0x44, -1);
        __dt__6CCur18Fv(&self->mCursor, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x04, -1);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// Element destructor of TalkListEntry (used by the array teardown). Entries
// are plain PODs so there is no member teardown; only the backing memory is
// freed when the delete flag is set (retail __dt__80272774). Returns the
// pointer itself, as the deleting-destructor ABI requires.
extern "C" TalkListEntry* __dt__80272774(TalkListEntry* entry, int flags) {
    if (entry != 0 && flags > 0)
        operator delete(entry);
    return entry;
}

// Element constructor (defined below; used as the array element ctor).
void clearTalkEntry(TalkListEntry* entry);

// Construct the TalkListEntry array (retail __ct__802726F4). Constructs all
// 256 entries in place via the element ctor clearTalkEntry, then initialises
// the count to 0 and records the owning object.
extern "C" __declspec(noinline) void* __ct__802726F4(TalkListEntryArray* self, u32 parent) {
    __construct_array(self, (void*)clearTalkEntry, (void*)__dt__80272774, 0x14, 0x100);
    self->mCount = 0;
    self->mParent = parent;
    return self;
}

// Destroy the TalkListEntry array (retail __dt__802727B4), freeing the
// backing memory when the delete flag is set. -O4,s merges the
// register saves into the retail stmw/lmw frame.
extern "C" TalkListEntryArray* __dt__802727B4(TalkListEntryArray* self, int flags) {
    if (self != 0) {
        __destroy_arr(self, (void*)__dt__80272774, 0x14, 0x100);
        if (flags > 0)
            operator delete(self);
    }
    return self;
}

// Initialise the Kizuna talk list (retail initTalkList): load the talk-list
// file and the shared archive, then set up the scroll bar.
void CKizunaTalkList::initTalkList() {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    this->mEntryCount = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)handle, &lbl_eu_8050E990[0x67], this, 0, 0);
    this->mUnknown18 = (u32)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(KyoshinHeap_GetField44(), &lbl_eu_8050E990[0x82], this, 0, 0);
    CScrollBar_loadLayoutArc(&this->mScrollBar);
    this->mUnknown86 = 0;
}

// Per-frame update while visible (retail updateTalkList): advance the state
// machine, then refresh the layout and the embedded widgets.
// Retail symbols are unmangled (advanceTalkAnim24 etc.), so these helpers are
// declared with C linkage even though they take the list as a parameter.
extern "C" __declspec(noinline) void advanceTalkAnim24(CKizunaTalkList* self);
extern "C" __declspec(noinline) void advanceTalkAnim28(CKizunaTalkList* self);
extern "C" __declspec(noinline) void rewindTalkAnim28(CKizunaTalkList* self);
extern "C" __declspec(noinline) void rewindTalkAnim24(CKizunaTalkList* self);

void CKizunaTalkList::updateTalkList() {
    if (this->mState84 != 0) {
        switch (this->mState85) {
        case 1:
            advanceTalkAnim24(this);
            break;
        case 2:
            advanceTalkAnim28(this);
            break;
        case 4:
            rewindTalkAnim28(this);
            break;
        case 5:
            rewindTalkAnim24(this);
            break;
        }

        // Layout virtual slot 0x38/4: disable animation.
        ((void (*)(void*, int))(*(void***)this->mpLayout20)[0x38 / 4])(this->mpLayout20, 0);
        Cur_Update(&this->mCursor);
        CScrollBar_UpdateDispatch(&this->mScrollBar);
    }
}

// Draw the talk list (retail drawTalkList): draw the layout, scroll bar and
// cursor when the list is visible.
void CKizunaTalkList::drawTalkList(nw4r::lyt::DrawInfo* pDrawInfo) {
    if (this->mState84 != 0) {
        drawLayout(this->mpLayout20, pDrawInfo, 0, 1);
        CScrollBar_draw(&this->mScrollBar, (void*)pDrawInfo);
        Cur_DrawLayout(&this->mCursor, (void*)pDrawInfo);
    }
}

// Teardown (retail teardownTalkList): release the BDAT query state, file handles,
// layout/accessor, cursor and scroll bar, and clear the shared-file pointer.
void CKizunaTalkList::teardownTalkList() {
    CBdat::getEntry(2);
    closeFileHandle((CFileHandle**)&this->mEntryCount);
    closeFileHandle((CFileHandle**)&this->mUnknown18);
    this->mState84 = 0;
    if (this->mpLayout20 != NULL) {
        // Layout virtual slot 0x08/4.
        ((void (*)(void*, int))((void**)this->mpLayout20)[0x08 / 4])(this->mpLayout20, 1);
        this->mpLayout20 = NULL;
    }
    releaseArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)this->mUnknown1C);
    this->mUnknown1C = 0;

    this->mCursor.cleanup();
    CScrollBar_Teardown(&this->mScrollBar);
    deleteRegion((UnkClass_8045F564*)((u8*)this + 4));
    lbl_eu_806648B8 = NULL;
}

// Populate a talk-list entry with the given fields (retail fillTalkEntry).
// noinline: retail keeps these as out-of-line calls from buildTalkList.
__declspec(noinline) void fillTalkEntry(TalkListEntry* entry, u32 a, u32 b,
                                        u32 c, u32 d, s8 in12, s16 in10,
                                        u8 in13) {
    entry->field_00 = a;
    entry->field_04 = b;
    entry->field_08 = c;
    entry->field_0C = d;
    entry->field_10 = in10;   // s16
    entry->field_12 = in12;   // s8, init to -1
    entry->field_13 = in13;   // u8
}

// Zero-initialise a talk-list entry (retail clearTalkEntry).
__declspec(noinline) void clearTalkEntry(TalkListEntry* entry) {
    entry->field_00 = 0;
    entry->field_04 = 0;
    entry->field_08 = 0;
    entry->field_0C = 0;
    entry->field_10 = 0;
    entry->field_12 = -1;
    entry->field_13 = 0;
}

// Copy one talk-list entry into another (retail copyTalkEntry).
__declspec(noinline) void copyTalkEntry(TalkListEntry* dst, TalkListEntry* src) {
    dst->field_00 = src->field_00;
    dst->field_04 = src->field_04;
    dst->field_08 = src->field_08;
    dst->field_0C = src->field_0C;
    dst->field_10 = src->field_10;
    dst->field_12 = src->field_12;
    dst->field_13 = src->field_13;
}

// Copy one talk-list entry into another (retail cloneTalkEntry).
__declspec(noinline) void cloneTalkEntry(TalkListEntry* dst,
                                        const TalkListEntry* src) {
    dst->field_00 = src->field_00;
    dst->field_04 = src->field_04;
    dst->field_08 = src->field_08;
    dst->field_0C = src->field_0C;
    dst->field_10 = src->field_10;
    dst->field_12 = src->field_12;
    dst->field_13 = src->field_13;
}

// Virtual slot 3: resource lookup (this, "timg", name, 0).
typedef u32 (*KtlResFn)(void*, const char*, const char*, u32);

// Pure-virtual view of the BDAT/resource-list object whose vtable slot at
// offset 0x0C resolves a "timg" resource by name. MWCC prepends
// offset-to-top + RTTI entries, so vtable offset = (index + 2) * 4;
// index 1 -> +0x0C. All-pure so MWCC emits no vtable for the view itself.
class KtlResView {
public:
    virtual void v00() = 0;                                              // 0x08
    virtual void* v01(const char* type, const char* name, u32 unk) = 0; // 0x0C
};

// Whether an icon/texture row exists for the given subtype, by asking the
// owning layout's shared resource accessor for the "timg" resource (retail
// findTalkIcon). v==0 queries the parent accessor directly; v!=0 re-resolves
// the icon name through the shared character table first. noinline keeps
// buildTalkList emitting a real bl (retail keeps it out-of-line).
__declspec(noinline) u32 findTalkIcon(TalkListEntryArray* self, u8 v) {
    if (v == 0) {
        char* strs = lbl_eu_8050E990;
        void* found = ((KtlResView*)(void*)self->mParent)->v01(
            (const char*)0x74696d67 /* "timg" */, strs + 0x46, 0);
        if (found != 0)
            return (u32)found;
        return 0;
    }
    u16 id = BdatGetU16Direct(lbl_eu_80664090, &lbl_eu_8050E990[0x5e], v);
    char* name = MakeTplNameSysFile(id);
    void* resAcc = CUICfManager_getArcResourceAccessor();
    void* found = ((KtlResView*)resAcc)->v01(
        (const char*)0x74696d67 /* "timg" */, name, 0);
    if (found != 0)
        return (u32)found;
    return 0;
}

// Build the talk-list entry table from the affinity talk data (retail
// buildTalkList). Reads the shared character database handed off in
// lbl_eu_806648B8, collects the candidate character IDs that have talk
// entries, then runs four passes to assemble type-1/type-2/type-3/name0
// entries and finally bubble-sorts the table by entry subtype (field_13).
void buildTalkList(TalkListEntryArray* arr) {
    void* fp = lbl_eu_806648B8;
    u16 charCount = (u16)Bdat_GetMaxRow_B1EC(fp);
    char* fmt12 = BdatTouchStringCell(&lbl_eu_8050E990[0], &lbl_eu_8050E990[0xb], 0x12);
    BdatTouchStringCell(&lbl_eu_8050E990[0], &lbl_eu_8050E990[0xb], 0x13); // result unused in retail

    arr->mCount = 0;
    u16 collect[0x100];
    memset(collect, 0, 0x200);
    u16 ccount = 0;
    TalkListEntry tmpB, tmpC, tmpD, tmpE, tmpSort;

    // Pass A: collect IDs of characters that have a talk flag set.
    for (u16 c = 1; (u32)(u16)c <= (u32)charCount; c++) {
        if (CtrlRemote_TouchBitByArg((u32)((u16)c + 0x3440)) != 0)
            collect[ccount++] = c;
    }

    // Pass B: type-1 entries (affinity value below the BDAT threshold).
    for (u16 k = 0; (u32)k < (u32)ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (CtrlRemote_TouchBitByArg((u32)(id + 0x24b0)) != 0) continue;

        u16 a = BdatGetU16Direct(fp, &lbl_eu_8050E990[0x10], id);
        if ((u32)a > CtrlRemote_TouchBitByArg(0x20)) continue;

        u16 n4 = BdatGetU16Direct(fp, &lbl_eu_8050E990[0x15], id);
        u8 m1 = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)CtrlRemote_TouchBitByArg(0x20);
        if ((u32)v >= 0x2a && (u32)v < 0x113) {
            if ((u32)m1 == 3) continue;
            if ((u32)m2 == 3) continue;
        } else if ((u32)v >= 0x113) {
            if ((u32)m1 == 3) m1 = 8;
            if ((u32)m2 == 3) m2 = 8;
        }
        if ((u32)n4 > (u32)LookupRelateValue(m1, m2)) continue;

        u32 v1 = findTalkIcon(arr, m1);
        u32 v2 = findTalkIcon(arr, m2);
        u8 x = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x30], id);
        char* cond = BdatGetPtrDirect((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x3b], id);

        fillTalkEntry(&tmpB, (u32)fmt12, (u32)cond, v1, v2, (s8)1, (s16)n4, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        cloneTalkEntry(&arr->mEntries[c], &tmpB);
        collect[k] = 0;
    }

    // Pass C: type-2 entries (same guard/filter logic, no BDAT-threshold
    // affinity comparison).
    for (u16 k = 0; (u32)k < (u32)ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (CtrlRemote_TouchBitByArg((u32)(id + 0x24b0)) != 0) continue;

        u16 a = BdatGetU16Direct(fp, &lbl_eu_8050E990[0x10], id);
        if ((u32)a > CtrlRemote_TouchBitByArg(0x20)) continue;

        u16 n4 = BdatGetU16Direct(fp, &lbl_eu_8050E990[0x15], id);
        u8 m1 = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)CtrlRemote_TouchBitByArg(0x20);
        if ((u32)v >= 0x2a && (u32)v < 0x113) {
            if ((u32)m1 == 3) continue;
            if ((u32)m2 == 3) continue;
        } else if ((u32)v >= 0x113) {
            if ((u32)m1 == 3) m1 = 8;
            if ((u32)m2 == 3) m2 = 8;
        }

        u32 v1 = findTalkIcon(arr, m1);
        u32 v2 = findTalkIcon(arr, m2);
        u8 x = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x30], id);
        char* cond = BdatGetPtrDirect((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x3b], id);

        fillTalkEntry(&tmpC, (u32)fmt12, (u32)cond, v1, v2, (s8)2, (s16)n4, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        cloneTalkEntry(&arr->mEntries[c], &tmpC);
        collect[k] = 0;
    }

    // Pass D: type-3 entries - same guard as B/C but no affinity-value or
    // icon checks; the icon rows are queried with subtype 0 and the stored
    // affinity value is -1.
    for (u16 k = 0; (u32)k < (u32)ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (CtrlRemote_TouchBitByArg((u32)(id + 0x24b0)) != 0) continue;

        u8 m1 = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)CtrlRemote_TouchBitByArg(0x20);
        if ((u32)v >= 0x2a && (u32)v < 0x113) {
            if ((u32)m1 == 3) continue;
            if ((u32)m2 == 3) continue;
        }

        u32 v1 = findTalkIcon(arr, 0);
        u32 v2 = findTalkIcon(arr, 0);
        u8 x = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x30], id);
        char* cond = BdatGetPtrDirect((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x3b], id);

        fillTalkEntry(&tmpD, (u32)fmt12, (u32)cond, v1, v2, (s8)3, (s16)-1, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        cloneTalkEntry(&arr->mEntries[c], &tmpD);
        collect[k] = 0;
    }

    // Pass E: flagged-character entries (talk flag set), titled by character
    // name instead of the shared format string.
    for (u16 k = 0; (u32)k < (u32)ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (CtrlRemote_TouchBitByArg((u32)(id + 0x24b0)) == 0) continue;

        char* name0 = BdatGetPtrDirect(fp, &lbl_eu_8050E990[0x40], id);
        u16 n4 = BdatGetU16Direct(fp, &lbl_eu_8050E990[0x15], id);
        u8 m1 = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)CtrlRemote_TouchBitByArg(0x20);
        if ((u32)v >= 0x113) {
            if ((u32)m1 == 3) m1 = 8;
            if ((u32)m2 == 3) m2 = 8;
        }

        u32 v1 = findTalkIcon(arr, m1);
        u32 v2 = findTalkIcon(arr, m2);
        u8 x = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x30], id);
        char* cond = BdatGetPtrDirect((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)BdatGetU8Direct((u32)fp, &lbl_eu_8050E990[0x3b], id);

        fillTalkEntry(&tmpE, (u32)name0, (u32)cond, v1, v2, (s8)0, (s16)n4, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        cloneTalkEntry(&arr->mEntries[c], &tmpE);
        collect[k] = 0;
    }

    // Bubble-sort the assembled table ascending by subtype (field_13).
    for (u16 outer = 0; (s32)outer < (s32)(arr->mCount - 1); outer++) {
        int swapped = 0;
        for (u16 inner = 0; (s32)inner < (s32)(arr->mCount - 1 - outer); inner++) {
            TalkListEntry* e1 = &arr->mEntries[inner];
            TalkListEntry* e2 = &arr->mEntries[inner + 1];
            if ((u32)e1->field_13 > (u32)e2->field_13) {
                clearTalkEntry(&tmpSort);
                copyTalkEntry(&tmpSort, e1);
                copyTalkEntry(e1, e2);
                copyTalkEntry(e2, &tmpSort);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Whether the talk-list has anything to show (retail hasVisibleTalks): returns
// the unknown-0x86 state byte when the scroll bar is visible, else 0.
u8 CKizunaTalkList::hasVisibleTalks() {
    if (CScrollBar_isVisible(&this->mScrollBar)) {
        return this->mUnknown86;
    }
    return 0;
}


// Begin showing the talk list (retail showTalkList): when idle, mark state 1,
// bind the entry-depart/entry animations and configure the scroll bar.
void CKizunaTalkList::showTalkList() {
    if (this->mState85 == 0) {
        this->mState85 = 1;
        this->mNeedsRebuild = 0;
        bindScrollOutAnims(this);
        moveTalkCursor(this);
        f32 vec[3];
        vec[0] = lbl_eu_806689C8;
        vec[1] = lbl_eu_806689CC;
        vec[2] = lbl_eu_806689D0;
        CScrollBar_InitRootPane(&this->mScrollBar, vec);
        CScrollBar_UpdateThumb(&this->mScrollBar, 7, this->mEntryArray.mCount);
        CScrollBar_requestScrollIn(&this->mScrollBar);
    }
}

// Advance from display to dismissal (retail hideTalkList): when the list is
// shown (state 3), step to 4 and make the cursor leave the list.
void CKizunaTalkList::hideTalkList() {
    if (this->mState85 == 3u) {
        this->mState85 = 4;
        this->mNeedsRebuild = 0;
        bindScrollInAnims(this);
        Cur_SetVisible(&this->mCursor, 0);
        CScrollBar_requestScrollOut(&this->mScrollBar);
        playUISound(6);
    }
}

void CKizunaTalkList::scrollTalkListUp() {
    // Scroll up one position through the talk entries.
    u8 count = this->mEntryArray.mCount;
    u8 dec = this->mUnknown88 - 1;
    this->mUnknown88 = (s8)dec;
    if ((s8)dec < 0) {
        this->mUnknown88 = 0;
        s16 pos = this->mUnknown8A - 1;
        this->mUnknown8A = pos;
        if (pos < 0) {
            if (count >= 7) {
                this->mUnknown88 = 6;
                this->mUnknown8A = (s16)(count - 7);
            } else {
                u8 n = count - 1;
                this->mUnknown88 = (s8)n;
                this->mUnknown8A = 0;
                if ((s8)n < 0)
                    this->mUnknown88 = 0;
            }
        }
    }
    renderTalkRows(this);
    moveTalkCursor(this);
    CScrollBar_PlaceThumb(&this->mScrollBar, (u16)this->mUnknown8A);
    playUISound(1);
}

void CKizunaTalkList::scrollTalkListDown() {
    // Scroll down one position through the talk entries.
    u8 count = this->mEntryArray.mCount;
    if (count >= 7) {
        this->mUnknown88 = this->mUnknown88 + 1;
        if (this->mUnknown88 >= 7) {
            this->mUnknown88 = 6;
            int limit = count - 7;
            this->mUnknown8A = this->mUnknown8A + 1;
            if (this->mUnknown8A > limit) {
                this->mUnknown88 = 0;
                this->mUnknown8A = 0;
            }
        }
    } else {
        this->mUnknown88 = this->mUnknown88 + 1;
        if (this->mUnknown88 >= count) {
            this->mUnknown88 = 0;
            this->mUnknown8A = 0;
        }
    }
    renderTalkRows(this);
    moveTalkCursor(this);
    CScrollBar_PlaceThumb(&this->mScrollBar, (u16)this->mUnknown8A);
    playUISound(1);
}

void CKizunaTalkList::pageTalkListUp() {
    // Scroll up by a page of seven entries.
    u8 count = this->mEntryArray.mCount;
    if (count >= 7) {
        s16 pos = this->mUnknown8A - 7;
        this->mUnknown8A = pos;
        if (pos < 0) {
            this->mUnknown88 = (s8)(pos + 6);
            this->mUnknown8A = 0;
            if (this->mUnknown88 < 0)
                this->mUnknown88 = 0;
        }
    } else {
        this->mUnknown88 = 0;
        this->mUnknown8A = 0;
    }
    renderTalkRows(this);
    moveTalkCursor(this);
    CScrollBar_PlaceThumb(&this->mScrollBar, (u16)this->mUnknown8A);
    playUISound(1);
}

void CKizunaTalkList::pageTalkListDown() {
    // Scroll down by a page of seven entries, clamping to the list end.
    u8 count = this->mEntryArray.mCount;
    if (count >= 7) {
        this->mUnknown8A = this->mUnknown8A + 7;
        int limit = count - 7;
        if (this->mUnknown8A > limit) {
            this->mUnknown88 = this->mUnknown8A - limit;
            this->mUnknown8A = limit;
            if (this->mUnknown88 >= 7)
                this->mUnknown88 = 6;
        }
    } else {
        this->mUnknown88 = count - 1;
        this->mUnknown8A = 0;
        if (this->mUnknown88 < 0)
            this->mUnknown88 = 0;
    }
    renderTalkRows(this);
    moveTalkCursor(this);
    CScrollBar_PlaceThumb(&this->mScrollBar, (u16)this->mUnknown8A);
    playUISound(1);
}

extern "C" __declspec(noinline) void advanceTalkAnim24(CKizunaTalkList* self) {
    // Advance the entry-show animation; when it completes, step the state
    // machine forward (retail advanceTalkAnim24).
    if (advanceAnimTransform(self->mpAnim24, lbl_eu_806689D4) != 0) {
        self->mState85 = 2;
        bindScrollInAnims(self);
    }
}

extern "C" __declspec(noinline) void advanceTalkAnim28(CKizunaTalkList* self) {
    if (advanceAnimTransform(self->mpAnim28, 2.0f) != 0) {
        self->mState85 = 3;
        moveTalkCursor(self);
        self->mNeedsRebuild = 1;
    }
}

extern "C" __declspec(noinline) void rewindTalkAnim28(CKizunaTalkList* self) {
    if (AnimRewindFrame(self->mpAnim28, lbl_eu_806689D4) != 0) {
        self->mState85 = 5;
        bindScrollOutAnims(self);
    }
}

extern "C" __declspec(noinline) void rewindTalkAnim24(CKizunaTalkList* self) {
    if (AnimRewindFrame(self->mpAnim24, lbl_eu_806689D4) != 0) {
        self->mState85 = 0;
        self->mNeedsRebuild = 1;
    }
}

// Bind the scroll-out animations for the talk list (retail bindScrollOutAnims).
// Bind the scroll-out animations for the talk list (retail bindScrollOutAnims).
extern "C" __declspec(noinline) void bindScrollOutAnims(CKizunaTalkList* self) {
    self->mpLayout20->SetAnimationEnable(self->mpAnim28, 0);
    self->mpLayout20->SetAnimationEnable(self->mpAnim24, 1);
}

// Bind the scroll-in animations for the talk list (retail bindScrollInAnims).
extern "C" __declspec(noinline) void bindScrollInAnims(CKizunaTalkList* self) {
    self->mpLayout20->SetAnimationEnable(self->mpAnim24, 0);
    self->mpLayout20->SetAnimationEnable(self->mpAnim28, 1);
}

void bindScrollInAnims(){}

// Rebuild/render the visible list. For each of the (up to) 7 visible rows it
// pulls the entry, binds the pane names (built with sprintf row suffixes),
// resolves the affinity-level animation string and the row highlight colour,
// and drives the pane visibility flag (retail renderTalkRows).
extern "C" __declspec(noinline) void renderTalkRows(CKizunaTalkList* self) {
    char buf[0x10];
    char* lbl = lbl_eu_8050E990;
    int i = 0;
    do {
        int base = i + self->mUnknown8A;                       // current row index + cursor
        TalkListEntry* entry = (TalkListEntry*)talkEntryAt(&self->mEntryArray, (u8)base);
        u8 dispIdx = (u8)(i + 1);                              // 1-based display index

        // Row number ("00/01") style panes x3, each with an optional highlight.
        sprintf(buf, &lbl[0x9e], dispIdx);
        void* pane = ((void* (*)(void*))(((void**)*(void**)((char*)self->mpLayout20 + 0x10))[0x3c / 4]))(*(void**)((char*)self->mpLayout20 + 0x10));
        if (pane) {
            int z = (int)(self->mEntryArray.mCount ^ ((u8)i + base));
            setPaneVisible(pane, (u32)(((z >> 1) - (z & self->mEntryArray.mCount)) >> 31));
        }

        sprintf(buf, &lbl[0xae], dispIdx);
        pane = ((void* (*)(void*))(((void**)*(void**)((char*)self->mpLayout20 + 0x10))[0x3c / 4]))(*(void**)((char*)self->mpLayout20 + 0x10));
        if (pane) {
            int z = (int)(self->mEntryArray.mCount ^ ((u8)i + base));
            setPaneVisible(pane, (u32)(((z >> 1) - (z & self->mEntryArray.mCount)) >> 31));
        }

        sprintf(buf, &lbl[0xbe], dispIdx);
        pane = ((void* (*)(void*))(((void**)*(void**)((char*)self->mpLayout20 + 0x10))[0x3c / 4]))(*(void**)((char*)self->mpLayout20 + 0x10));
        if (pane) {
            int z = (int)(self->mEntryArray.mCount ^ ((u8)i + base));
            setPaneVisible(pane, (u32)(((z >> 1) - (z & self->mEntryArray.mCount)) >> 31));
        }

        // Title / description text.
        sprintf(buf, &lbl[0xcb], dispIdx);
        LayoutSetTextBoxFmtValue(self->mpLayout20, buf, (char*)entry->field_00, 0u);
        sprintf(buf, &lbl[0xd7], dispIdx);
        LayoutSetTextBoxFmtValue(self->mpLayout20, buf, (char*)entry->field_04, 0u);

        if (entry->field_12 != -1) {
            sprintf(buf, &lbl[0xe3], dispIdx);
            PaneSetTexPaletteByName(self->mpLayout20, buf, (void*)entry->field_08);
            sprintf(buf, &lbl[0xf1], dispIdx);
            PaneSetTexPaletteByName(self->mpLayout20, buf, (void*)entry->field_0C);
        }

        sprintf(buf, &lbl[0xff], dispIdx);
        pane = ((void* (*)(void*))(((void**)*(void**)((char*)self->mpLayout20 + 0x10))[0x3c / 4]))(*(void**)((char*)self->mpLayout20 + 0x10));
        if (pane) {
            int z = (int)(self->mEntryArray.mCount ^ ((u8)i + base));
            setPaneVisible(pane, (u32)(((z >> 1) - (z & self->mEntryArray.mCount)) >> 31));
        }

        // Resolve the affinity-level animation string.
        const char* lvlStr;
        int s = entry->field_10;
        if (s >= 0x1388)
            lvlStr = &lbl[0x187];
        else if (s >= 0xbb8)
            lvlStr = &lbl[0x16f];
        else if (s >= 0x7d0)
            lvlStr = &lbl[0x157];
        else if (s >= 0x3e8)
            lvlStr = &lbl[0x13f];
        else if (s >= 0x0)
            lvlStr = &lbl[0x127];
        else
            lvlStr = &lbl[0x10f];

        void* anim = ((void* (*)(void*, u32, const char*, int))(((void**)self->mUnknown1C)[0x0c / 4]))((void*)self->mUnknown1C, 0x74696d67, lvlStr, 0);
        if (anim)
            PaneSetTexPaletteByName(self->mpLayout20, buf, anim);

        // Row highlight colour / visibility from the entry subtype.
        GXColorS10* c0;
        GXColorS10* c1;
        int vis;
        if (entry->field_12 == 0) {
            c0 = (GXColorS10*)&lbl_eu_806648D0;
            c1 = (GXColorS10*)&lbl_eu_806648D8;
            vis = 1;
        } else if (entry->field_12 == 1) {
            c0 = (GXColorS10*)&lbl_eu_806648D0;
            c1 = (GXColorS10*)&lbl_eu_806648D8;
            vis = 0;
        } else if (entry->field_12 == 2) {
            c0 = (GXColorS10*)&lbl_eu_806648C0;
            c1 = (GXColorS10*)&lbl_eu_806648C8;
            vis = 0;
        } else {   // == 3
            c0 = (GXColorS10*)&lbl_eu_806648C0;
            c1 = (GXColorS10*)&lbl_eu_806648C8;
            vis = 0;
        }

        sprintf(buf, &lbl[0xbe], dispIdx);
        PaneMatSetTevColorsByName(self->mpLayout20, buf, c0, c1);
        sprintf(buf, &lbl[0x19f], dispIdx);
        pane = ((void* (*)(void*))(((void**)*(void**)((char*)self->mpLayout20 + 0x10))[0x3c / 4]))(*(void**)((char*)self->mpLayout20 + 0x10));
        if (pane)
            setPaneVisible(pane, vis);

        i++;
    } while (i < 7);
}

// Rebuild/render the cursor row highlight (retail moveTalkCursor): resolve the
// two layout panes named after the current row, build the cursor target
// position from them and hand it to the cursor via its vtable slot 4.
extern "C" __declspec(noinline) void moveTalkCursor(CKizunaTalkList* self) {
    nw4r::math::VEC3 pos;
    u32 _pad;
    char name[0x28];
    char* lbl = lbl_eu_8050E990;
    sprintf(name, lbl + 0xcb, (int)self->mUnknown88 + 1);
    Cur_SetVisible(&self->mCursor, 1);

    // Layout object re-read for every access so nothing extra gets
    // enregistered across the virtual calls.
    nw4r::lyt::Pane* paneA =
        self->mpLayout20->GetRootPane()->FindPaneByName(name, true);
    nw4r::lyt::Pane* paneB =
        self->mpLayout20->GetRootPane()->FindPaneByName(lbl + 0x1aa, true);

    ScalePaneWorldPos(&pos, paneA, paneB, self->mpLayout20->GetRootPane());

    self->mCursor.setRootPaneTranslate(&pos);
}

// Rebuild the visible talk list (retail rebuildTalkList): requires both the
// shared BDAT data and the layout accessor. Builds a fresh local table via
// __ct__802726F4, copies it over the member array, then rebuilds/sorts.
void rebuildTalkList(CKizunaTalkList* self) {
    u32 accessor = self->mUnknown1C;
    if (accessor == 0)
        return;
    if (lbl_eu_806648B8 == NULL)
        return;

    TalkListEntryArray tmp;
    __ct__802726F4(&tmp, accessor);

    TalkListEntry* dst = self->mEntryArray.mEntries;
    TalkListEntry* src = tmp.mEntries;
    TalkListEntry* end = self->mEntryArray.mEntries + 256;
    do {
        cloneTalkEntry(dst, src);
        dst++;
        src++;
    } while (dst < end);
    self->mEntryArray.mCount = tmp.mCount;
    self->mEntryArray.mParent = tmp.mParent;
    __destroy_arr(&tmp, (void*)__dt__80272774, 0x14, 0x100);

    buildTalkList(&self->mEntryArray);
    self->mUnknown86 = 1;
    self->mState84 = 1;
    renderTalkRows(self);
}

// ---------------------------------------------------------------------------
// Handle a file-load completion event (retail OnFileEvent__15CKizunaTalkListFP10CEventFile).
// The event's file handle matches either the talk-list layout archive
// (mEntryCount) or the shared archive (mUnknown18). On the layout archive we
// tear down/rebuild the whole scene: region, ArcResource accessor, layout,
// animation transforms, font pane, and the embedded cursor. Returns 1 on the
// matching archive, 0 otherwise.
bool CKizunaTalkList::OnFileEvent(CEventFile* pEventFile) {
    if (this->mEntryCount == (u32)pEventFile->mFileHandle) {
        u32 handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
        char* strBase = lbl_eu_8050E990;

        // Create the region backing the base memory area, then scope a guard
        // object so the region is torn down on exit.
        ((UnkClass_8045F564*)((u8*)this + 4))->createRegion((int)handle, 0x10000, &strBase[0x1b9], 0);
        u8 temp8[0x8];
        __ct__14Class_8045F858FP17UnkClass_8045F564(temp8, (u8*)this + 4);

        CFileHandle* fh = (CFileHandle*)this->mEntryCount;
        void* fileData = fh->getData();
        setMemInitFlag__Q23mtl10MemManagerFb(false);

        // Attach the layout archive to a fresh ArcResource accessor.
        this->mUnknown1C = (u32)CLibLayout::createArcResourceAccessor();
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc((nw4r::lyt::ArcResourceAccessor*)this->mUnknown1C, fileData, &strBase[0x1c9]);

        // Create the layout and its two animation transforms.
        buildLayout(&this->mpLayout20, (nw4r::lyt::ArcResourceAccessor*)this->mUnknown1C, &strBase[0x1cd]);
        bindLayoutAnimTransform(this->mpLayout20, &this->mpAnim24, (nw4r::lyt::ArcResourceAccessor*)this->mUnknown1C, &strBase[0x1e2]);
        bindLayoutAnimTransform(this->mpLayout20, &this->mpAnim28, (nw4r::lyt::ArcResourceAccessor*)this->mUnknown1C, &strBase[0x1fa]);

        // Bind the font pane onto the layout.
        void* rootPane = *(void**)((char*)this->mpLayout20 + 0x10);
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, this->mpLayout20);
        u32 fontResult = ((u32 (*)(void*))(((void**)fontObj)[0x24 / 4]))(fontObj);
        ApplyFontToTxtPanes((nw4r::lyt::Pane*)rootPane, fontResult);

        bindScrollOutAnims(this);

        // Layout virtual slot 14 (offset 0x38): disable animation.
        ((void (*)(void*, int))(((void**)this->mpLayout20)[0x38 / 4]))(this->mpLayout20, 0);

        // Create the CCur18 cursor on the stack and copy its body into the
        // embedded cursor (+0x2c), skipping the vtable at +0.
        CCur18 cur18Temp;
        __ct__CCur18(&cur18Temp, CUICfManager_getArcResourceAccessor());
        mCursor = cur18Temp;
        __dt__6CCur18Fv(&cur18Temp, -1);
        mCursor.initLayout();

        rebuildTalkList(this);

        this->mEntryCount = 0;
        ((UnkClass_8045F564*)((u8*)this + 4))->validateHeap();
        __dt__14Class_8045F858Fv(temp8, -1);
        return 1;
    }

    if (this->mUnknown18 == (u32)pEventFile->mFileHandle) {
        // Shared-archive hit: release the previously buffered shared file data.
        CFileHandle* fh2 = (CFileHandle*)this->mUnknown18;
        void* data2 = fh2->getData();
        CBdat::setBdatEntry(2, data2);
        Bdat_GetTable_AA34();
        lbl_eu_806648B8 = (void*)getFP(&lbl_eu_8050E990[0x217]);
        rebuildTalkList(this);
        this->mUnknown18 = 0;
        return 1;
    }

    return 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_80274458() {
    setGXColorS10(&lbl_eu_806648C0, 0x8c, 0x8c, 0x8c, 0);
    setGXColorS10(&lbl_eu_806648C8, 0x8c, 0x8c, 0x8c, 0xff);
    setGXColorS10(&lbl_eu_806648D0, 0x48, 0x3a, 0x21, 0);
    setGXColorS10(&lbl_eu_806648D8, 0x48, 0x3a, 0x21, 0xff);
}

extern "C" __declspec(noinline) void* talkEntryAt(void* self, u32 idx) {
    // Use signed compare to match retail cmpwi
    if ((s32)idx >= 0x100) return 0;
    return (u8*)self + idx * 0x14;
}

// Rebuild-pending flag (retail func_8027355C): the pending-rebuild byte at
// +0x87, read as an int so external callers keep a plain cmpwi on the result.
// Member alias: CKizunaTalkList::needsRebuild.
int CKizunaTalkList::needsTalkRebuild() {
    return this->mNeedsRebuild;
}
