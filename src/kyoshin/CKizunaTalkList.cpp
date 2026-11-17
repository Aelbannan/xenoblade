// Auto-scaffolded catalog TU for kyoshin/CKizunaTalkList
// Replace stubs with high-level C/C++ during decomp.

#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CKizunaTalkList.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/code_80135FDC.hpp"
void bindLayoutAnimTransform(nw4r::lyt::Layout* layout, nw4r::lyt::AnimTransform** anim, nw4r::lyt::ArcResourceAccessor* accessor, char* name);
void buildLayout(nw4r::lyt::Layout** layout, nw4r::lyt::ArcResourceAccessor* accessor, const char* name);
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/CKizunaTalkListImports.hpp"
#include "monolib/device/CDeviceFont.hpp"
extern "C" void setGXColorS10(void*, s16, s16, s16, s16);
void playUISound(u32);

// Entry table constructor: all 256 entries are default-constructed
// (cleared, subtype -1), the count is reset and the owning resource accessor
// recorded. Retail emits the inline entry ctor/dtor and the implicit table
// dtor right after this function.
TalkListEntryArray::TalkListEntryArray(nw4r::lyt::ArcResourceAccessor* parent) {
    mCount = 0;
    mParent = parent;
}

CKizunaTalkList::CKizunaTalkList(int)
    : mMemRegion(), mFileHandle(NULL), mBdatHandle(NULL), mAccessor(NULL), mpLayout20(NULL),
      mpAnim24(NULL), mpAnim28(NULL), mCursor(NULL), mScrollBar(0), mState84(0), mState85(0),
      mUnknown86(0), mNeedsRebuild(1), mUnknown88(0), mUnknown8A(0), mEntryArray(NULL) {
    // Re-initialise the bar as a vertical one. CScrollBar is still a
    // non-polymorphic view (hand vptr member), so its implicit operator= would
    // copy the vptr; retail's `mScrollBar = CScrollBar(1);` skips it. Copy the
    // body of a real temporary member by member until CScrollBar is real.
    CScrollBar tmp(1);
    mScrollBar.mMemRegion = tmp.mMemRegion;
    mScrollBar.mFileHandle = tmp.mFileHandle;
    mScrollBar.mAccessor = tmp.mAccessor;
    mScrollBar.mLayout = tmp.mLayout;
    mScrollBar.mAnimTransform = tmp.mAnimTransform;
    mScrollBar.mReady = tmp.mReady;
    mScrollBar.mVisible = tmp.mVisible;
    mScrollBar.mState = tmp.mState;
    mScrollBar.mActive = tmp.mActive;
    mScrollBar.mAnimOffset = tmp.mAnimOffset;
    mScrollBar.mScrollPosY = tmp.mScrollPosY;
    mScrollBar.mScrollRatio = tmp.mScrollRatio;
    mScrollBar.mThumbHeight = tmp.mThumbHeight;
    mScrollBar.mContentHeight = tmp.mContentHeight;
    mScrollBar.mDirection = tmp.mDirection;
}

CKizunaTalkList::~CKizunaTalkList() {}

// Initialise the Kizuna talk list (retail initTalkList): load the talk-list
// file and the shared archive, then set up the scroll bar.
void CKizunaTalkList::initTalkList() {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    this->mFileHandle = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)handle, &lbl_eu_8050E990[0x67], this, 0, 0);
    this->mBdatHandle = (CFileHandle*)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(KyoshinHeap_GetField44(), &lbl_eu_8050E990[0x82], this, 0, 0);
    this->mScrollBar.loadLayoutArc();
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

        this->mpLayout20->Animate(0);
        Cur_Update(&this->mCursor);
        this->mScrollBar.UpdateDispatch();
    }
}

// Draw the talk list (retail drawTalkList): draw the layout, scroll bar and
// cursor when the list is visible.
void CKizunaTalkList::drawTalkList(nw4r::lyt::DrawInfo* pDrawInfo) {
    if (this->mState84 != 0) {
        drawLayout(this->mpLayout20, pDrawInfo, 0, 1);
        this->mScrollBar.draw(pDrawInfo);
        Cur_DrawLayout(&this->mCursor, (void*)pDrawInfo);
    }
}

// Teardown (retail teardownTalkList): release the BDAT query state, file handles,
// layout/accessor, cursor and scroll bar, and clear the shared-file pointer.
void CKizunaTalkList::teardownTalkList() {
    CBdat::getEntry(2);
    closeFileHandle(&this->mFileHandle);
    closeFileHandle(&this->mBdatHandle);
    this->mState84 = 0;
    if (this->mpLayout20 != NULL) {
        delete this->mpLayout20;
        this->mpLayout20 = NULL;
    }
    releaseArcResourceAccessor(this->mAccessor);
    this->mAccessor = NULL;

    this->mCursor.cleanup();
    this->mScrollBar.Teardown();
    deleteRegion(&this->mMemRegion);
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
        arr->mEntries[c] = tmpB;
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
        arr->mEntries[c] = tmpC;
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
        arr->mEntries[c] = tmpD;
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
        arr->mEntries[c] = tmpE;
        collect[k] = 0;
    }

    // Bubble-sort the assembled table ascending by subtype (field_13).
    for (u16 outer = 0; (s32)outer < (s32)(arr->mCount - 1); outer++) {
        int swapped = 0;
        for (u16 inner = 0; (s32)inner < (s32)(arr->mCount - 1 - outer); inner++) {
            TalkListEntry* e1 = &arr->mEntries[inner];
            TalkListEntry* e2 = &arr->mEntries[inner + 1];
            if ((u32)e1->field_13 > (u32)e2->field_13) {
                tmpSort = TalkListEntry();
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
bool CKizunaTalkList::hasVisibleTalks() {
    if (this->mScrollBar.isVisible()) {
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
        this->mScrollBar.InitRootPane(reinterpret_cast<const nw4r::math::VEC3*>(vec));
        this->mScrollBar.UpdateThumb(7, this->mEntryArray.mCount);
        this->mScrollBar.requestScrollIn();
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
        this->mScrollBar.requestScrollOut();
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
    this->mScrollBar.PlaceThumb((u16)this->mUnknown8A);
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
    this->mScrollBar.PlaceThumb((u16)this->mUnknown8A);
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
    this->mScrollBar.PlaceThumb((u16)this->mUnknown8A);
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
    this->mScrollBar.PlaceThumb((u16)this->mUnknown8A);
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
extern "C" __declspec(noinline) void bindScrollOutAnims(CKizunaTalkList* self) {
    self->mpLayout20->SetAnimationEnable(self->mpAnim28, 0);
    self->mpLayout20->SetAnimationEnable(self->mpAnim24, 1);
}

// Bind the scroll-in animations for the talk list (retail bindScrollInAnims).
extern "C" __declspec(noinline) void bindScrollInAnims(CKizunaTalkList* self) {
    self->mpLayout20->SetAnimationEnable(self->mpAnim24, 0);
    self->mpLayout20->SetAnimationEnable(self->mpAnim28, 1);
}


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

        void* anim = self->mAccessor->GetResource(0x74696d67, lvlStr, 0);
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
// a temporary table, assigns it over the member array, then rebuilds/sorts.
void rebuildTalkList(CKizunaTalkList* self) {
    if (self->mAccessor == NULL || lbl_eu_806648B8 == NULL)
        return;

    self->mEntryArray = TalkListEntryArray(self->mAccessor);
    buildTalkList(&self->mEntryArray);
    self->mUnknown86 = 1;
    self->mState84 = 1;
    renderTalkRows(self);
}

// ---------------------------------------------------------------------------
// Handle a file-load completion event (retail OnFileEvent__15CKizunaTalkListFP10CEventFile).
// The event's file handle matches either the talk-list layout archive
// (mFileHandle) or the shared archive (mBdatHandle). On the layout archive we
// tear down/rebuild the whole scene: region, ArcResource accessor, layout,
// animation transforms, font pane, and the embedded cursor. Returns 1 on the
// matching archive, 0 otherwise.
bool CKizunaTalkList::OnFileEvent(CEventFile* pEventFile) {
    if (this->mFileHandle == pEventFile->mFileHandle) {
        u32 handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();

        // Create the region backing the base memory area, then scope a guard
        // object so the region is torn down on exit.
        this->mMemRegion.createRegion((int)handle, 0x10000, &lbl_eu_8050E990[0x1b9], 0);
        Class_8045F858 regionScope(&this->mMemRegion);

        CFileHandle* fh = this->mFileHandle;
        void* fileData = fh->getData();
        setMemInitFlag__Q23mtl10MemManagerFb(false);

        // Attach the layout archive to a fresh ArcResource accessor.
        this->mAccessor = CLibLayout::createArcResourceAccessor();
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(this->mAccessor, fileData, &lbl_eu_8050E990[0x1c9]);

        // Create the layout and its two animation transforms.
        buildLayout(&this->mpLayout20, this->mAccessor, &lbl_eu_8050E990[0x1cd]);
        bindLayoutAnimTransform(this->mpLayout20, &this->mpAnim24, this->mAccessor, &lbl_eu_8050E990[0x1e2]);
        bindLayoutAnimTransform(this->mpLayout20, &this->mpAnim28, this->mAccessor, &lbl_eu_8050E990[0x1fa]);

        // Bind the font pane onto the layout.
        nw4r::lyt::Pane* rootPane = this->mpLayout20->GetRootPane();
        u32 fontResult = (u32)static_cast<IDeviceFontInfo*>(
                             CDeviceFont::getFontInfo(1, this->mpLayout20))->getFont();
        ApplyFontToTxtPanes(rootPane, fontResult);

        bindScrollOutAnims(this);

        this->mpLayout20->Animate(0);

        mCursor = CCur18(CUICfManager_getArcResourceAccessor());
        mCursor.initLayout();

        rebuildTalkList(this);

        this->mFileHandle = NULL;
        this->mMemRegion.validateHeap();
        return 1;
    }

    if (this->mBdatHandle == pEventFile->mFileHandle) {
        // Shared-archive hit: release the previously buffered shared file data.
        CFileHandle* fh2 = this->mBdatHandle;
        void* data2 = fh2->getData();
        CBdat::setBdatEntry(2, data2);
        Bdat_GetTable_AA34();
        lbl_eu_806648B8 = (void*)getFP(&lbl_eu_8050E990[0x217]);
        rebuildTalkList(this);
        this->mBdatHandle = NULL;
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
