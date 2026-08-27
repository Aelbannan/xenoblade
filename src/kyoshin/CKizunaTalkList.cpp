// Auto-scaffolded catalog TU for kyoshin/CKizunaTalkList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CKizunaTalkList.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/code_80135FDC.hpp"

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
    __ct__CCur18(mCursor, NULL);
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
// self when the delete flag is set. optimize_for_size merges the r30/r31
// saves into the retail stmw r30 prologue.
#pragma optimize_for_size on
extern "C" CKizunaTalkList* __dt__15CKizunaTalkListFv(CKizunaTalkList* self, int flags) {
    if (self != 0) {
        if ((u8*)self + 0x8C != 0) {
            __destroy_arr((u8*)self + 0x8C, (void*)__dt__80272774, 0x14, 0x100);
        }
        __dt__10CScrollBarFv((u8*)self + 0x44, -1);
        __dt__6CCur18Fv((u8*)self + 0x2C, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x04, -1);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
#pragma optimize_for_size off

// Element destructor of TalkListEntry (used by the array teardown). Entries
// are plain PODs so there is no member teardown; only the backing memory is
// freed when the delete flag is set (retail __dt__80272774). Returns the
// pointer itself, as the deleting-destructor ABI requires.
extern "C" TalkListEntry* __dt__80272774(TalkListEntry* entry, int flags) {
    if (entry != 0 && flags > 0)
        __dl__FPv(entry);
    return entry;
}

// Element constructor (defined below; used as the array element ctor).
void func_8027274C(TalkListEntry* entry);

// Construct the TalkListEntry array (retail __ct__802726F4). Constructs all
// 256 entries in place via the element ctor func_8027274C, then initialises
// the count to 0 and records the owning object.
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
extern "C" __declspec(noinline) void* __ct__802726F4(TalkListEntryArray* self, u32 parent) {
    __construct_array(self, (void*)func_8027274C, (void*)__dt__80272774, 0x14, 0x100);
    self->mCount = 0;
    self->mParent = parent;
    return self;
}
#pragma optimize_for_size off

// Destroy the TalkListEntry array (retail __dt__802727B4), freeing the
// backing memory when the delete flag is set. optimize_for_size merges the
// register saves into the retail stmw/lmw frame.
#pragma optimize_for_size on
extern "C" TalkListEntryArray* __dt__802727B4(TalkListEntryArray* self, int flags) {
    if (self != 0) {
        __destroy_arr(self, (void*)__dt__80272774, 0x14, 0x100);
        if (flags > 0)
            __dl__FPv(self);
    }
    return self;
}
#pragma optimize_for_size off

#pragma optimize_for_size on
// Initialise the Kizuna talk list (retail func_802732F4): load the talk-list
// file and the shared archive, then set up the scroll bar.
void func_802732F4(CKizunaTalkList* self) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->mEntryCount = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)handle, &lbl_eu_8050E990[0x67], self, 0, 0);
    self->mUnknown18 = (u32)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(func_800A9D90(), &lbl_eu_8050E990[0x82], self, 0, 0);
    func_801F34F4(&self->mScrollBar);
    self->mUnknown86 = 0;
}
#pragma optimize_for_size off

// Per-frame update while visible (retail func_8027336C): advance the state
// machine, then refresh the layout and the embedded widgets.
// Retail symbols are unmangled (func_80273938 etc.), so these helpers are
// declared with C linkage even though they take the list as a parameter.
extern "C" __declspec(noinline) void func_80273938(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_80273984(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_802739D8(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_80273A24(CKizunaTalkList* self);

#pragma optimize_for_size on
void func_8027336C(CKizunaTalkList* self) {
    if (self->mState84 != 0) {
        switch (self->mState85) {
        case 1:
            func_80273938(self);
            break;
        case 2:
            func_80273984(self);
            break;
        case 4:
            func_802739D8(self);
            break;
        case 5:
            func_80273A24(self);
            break;
        }

        // Layout virtual slot 0x38/4: disable animation.
        ((void (*)(void*, int))(*(void***)self->mpLayout20)[0x38 / 4])(self->mpLayout20, 0);
        func_801D202C(&self->mCursor);
        func_801F3540(&self->mScrollBar);
    }
}
#pragma optimize_for_size off

#pragma optimize_for_size on
// Draw the talk list (retail func_8027340C): draw the layout, scroll bar and
// cursor when the list is visible.
void func_8027340C(CKizunaTalkList* self, nw4r::lyt::DrawInfo* pDrawInfo) {
    if (self->mState84 != 0) {
        func_80137038(self->mpLayout20, pDrawInfo, 0, 1);
        func_801F35B0(&self->mScrollBar, (void*)pDrawInfo);
        func_801D20B0(&self->mCursor, (void*)pDrawInfo);
    }
}
#pragma optimize_for_size off

#pragma optimize_for_size on
// Teardown (retail func_8027346C): release the BDAT query state, file handles,
// layout/accessor, cursor and scroll bar, and clear the shared-file pointer.
void func_8027346C(CKizunaTalkList* self) {
    func_8003AA8C__5CBdatFUl(2);
    func_801390E0((CFileHandle**)&self->mEntryCount);
    func_801390E0((CFileHandle**)&self->mUnknown18);
    self->mState84 = 0;
    if (self->mpLayout20 != NULL) {
        // Layout virtual slot 0x08/4.
        ((void (*)(void*, int))((void**)self->mpLayout20)[0x08 / 4])(self->mpLayout20, 1);
        self->mpLayout20 = NULL;
    }
    func_80139124((nw4r::lyt::ArcResourceAccessor*)self->mUnknown1C);
    self->mUnknown1C = 0;

    // Cursor virtual slot 0x0c/4 on the embedded cursor.
    ((void (*)(void*))((void**)*(void**)&self->mCursor[0])[0x0c / 4])(&self->mCursor[0]);
    func_801F35DC(&self->mScrollBar);
    func_8045F778((UnkClass_8045F564*)((u8*)self + 4));
    lbl_eu_806648B8 = NULL;
}
#pragma optimize_for_size off

// Populate a talk-list entry with the given fields (retail func_80272FA8).
// noinline: retail keeps these as out-of-line calls from func_80272810.
__declspec(noinline) void func_80272FA8(TalkListEntry* entry, u32 a, u32 b,
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

// Zero-initialise a talk-list entry (retail func_8027274C).
__declspec(noinline) void func_8027274C(TalkListEntry* entry) {
    entry->field_00 = 0;
    entry->field_04 = 0;
    entry->field_08 = 0;
    entry->field_0C = 0;
    entry->field_10 = 0;
    entry->field_12 = -1;
    entry->field_13 = 0;
}

// Copy one talk-list entry into another (retail func_80272FC8).
__declspec(noinline) void func_80272FC8(TalkListEntry* dst, TalkListEntry* src) {
    dst->field_00 = src->field_00;
    dst->field_04 = src->field_04;
    dst->field_08 = src->field_08;
    dst->field_0C = src->field_0C;
    dst->field_10 = src->field_10;
    dst->field_12 = src->field_12;
    dst->field_13 = src->field_13;
}

// Copy one talk-list entry into another (retail func_80273004).
__declspec(noinline) void func_80273004(TalkListEntry* dst,
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
// func_8027305C). v==0 queries the parent accessor directly; v!=0 re-resolves
// the icon name through the shared character table first. noinline keeps
// func_80272810 emitting a real bl (retail keeps it out-of-line).
__declspec(noinline) u32 func_8027305C(TalkListEntryArray* self, u8 v) {
    if (v == 0) {
        char* strs = lbl_eu_8050E990;
        void* found = ((KtlResView*)(void*)self->mParent)->v01(
            (const char*)0x74696d67 /* "timg" */, strs + 0x46, 0);
        if (found != 0)
            return (u32)found;
        return 0;
    }
    u16 id = func_80136254(lbl_eu_80664090, &lbl_eu_8050E990[0x5e], v);
    char* name = func_80138F78(id);
    void* resAcc = func_801355F4();
    void* found = ((KtlResView*)resAcc)->v01(
        (const char*)0x74696d67 /* "timg" */, name, 0);
    if (found != 0)
        return (u32)found;
    return 0;
}

// Build the talk-list entry table from the affinity talk data (retail
// func_80272810). Reads the shared character database handed off in
// lbl_eu_806648B8, collects the candidate character IDs that have talk
// entries, then runs four passes to assemble type-1/type-2/type-3/name0
// entries and finally bubble-sorts the table by entry subtype (field_13).
void func_80272810(TalkListEntryArray* arr) {
    void* fp = lbl_eu_806648B8;
    u16 charCount = (u16)func_8003B1EC(fp);
    char* fmt12 = func_80136190(&lbl_eu_8050E990[0], &lbl_eu_8050E990[0xb], 0x12);
    func_80136190(&lbl_eu_8050E990[0], &lbl_eu_8050E990[0xb], 0x13); // result unused in retail

    arr->mCount = 0;
    u16 collect[0x100];
    memset(collect, 0, 0x200);
    u16 ccount = 0;
    TalkListEntry tmpB, tmpC, tmpD, tmpE, tmpSort;

    // Pass A: collect IDs of characters that have a talk flag set.
    for (u16 c = 1; (u32)(u16)c <= (u32)charCount; c++) {
        if (func_8009CF8C((u32)((u16)c + 0x3440)) != 0)
            collect[ccount++] = c;
    }

    // Pass B: type-1 entries (affinity value below the BDAT threshold).
    for (u16 k = 0; (u32)k < (u32)ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (func_8009CF8C((u32)(id + 0x24b0)) != 0) continue;

        u16 a = func_80136254(fp, &lbl_eu_8050E990[0x10], id);
        if ((u32)a > func_8009CF8C(0x20)) continue;

        u16 n4 = func_80136254(fp, &lbl_eu_8050E990[0x15], id);
        u8 m1 = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)func_8009CF8C(0x20);
        if ((u32)v >= 0x2a && (u32)v < 0x113) {
            if ((u32)m1 == 3) continue;
            if ((u32)m2 == 3) continue;
        } else if ((u32)v >= 0x113) {
            if ((u32)m1 == 3) m1 = 8;
            if ((u32)m2 == 3) m2 = 8;
        }
        if ((u32)n4 > (u32)func_8013A7D0(m1, m2)) continue;

        u32 v1 = func_8027305C(arr, m1);
        u32 v2 = func_8027305C(arr, m2);
        u8 x = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x30], id);
        char* cond = func_8013639C((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x3b], id);

        func_80272FA8(&tmpB, (u32)fmt12, (u32)cond, v1, v2, (s8)1, (s16)n4, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        func_80273004(&arr->mEntries[c], &tmpB);
        collect[k] = 0;
    }

    // Pass C: type-2 entries (same guard/filter logic, no BDAT-threshold
    // affinity comparison).
    for (u16 k = 0; (u32)k < (u32)ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (func_8009CF8C((u32)(id + 0x24b0)) != 0) continue;

        u16 a = func_80136254(fp, &lbl_eu_8050E990[0x10], id);
        if ((u32)a > func_8009CF8C(0x20)) continue;

        u16 n4 = func_80136254(fp, &lbl_eu_8050E990[0x15], id);
        u8 m1 = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)func_8009CF8C(0x20);
        if ((u32)v >= 0x2a && (u32)v < 0x113) {
            if ((u32)m1 == 3) continue;
            if ((u32)m2 == 3) continue;
        } else if ((u32)v >= 0x113) {
            if ((u32)m1 == 3) m1 = 8;
            if ((u32)m2 == 3) m2 = 8;
        }

        u32 v1 = func_8027305C(arr, m1);
        u32 v2 = func_8027305C(arr, m2);
        u8 x = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x30], id);
        char* cond = func_8013639C((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x3b], id);

        func_80272FA8(&tmpC, (u32)fmt12, (u32)cond, v1, v2, (s8)2, (s16)n4, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        func_80273004(&arr->mEntries[c], &tmpC);
        collect[k] = 0;
    }

    // Pass D: type-3 entries - same guard as B/C but no affinity-value or
    // icon checks; the icon rows are queried with subtype 0 and the stored
    // affinity value is -1.
    for (u16 k = 0; (u32)k < (u32)ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (func_8009CF8C((u32)(id + 0x24b0)) != 0) continue;

        u8 m1 = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)func_8009CF8C(0x20);
        if ((u32)v >= 0x2a && (u32)v < 0x113) {
            if ((u32)m1 == 3) continue;
            if ((u32)m2 == 3) continue;
        }

        u32 v1 = func_8027305C(arr, 0);
        u32 v2 = func_8027305C(arr, 0);
        u8 x = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x30], id);
        char* cond = func_8013639C((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x3b], id);

        func_80272FA8(&tmpD, (u32)fmt12, (u32)cond, v1, v2, (s8)3, (s16)-1, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        func_80273004(&arr->mEntries[c], &tmpD);
        collect[k] = 0;
    }

    // Pass E: flagged-character entries (talk flag set), titled by character
    // name instead of the shared format string.
    for (u16 k = 0; (u32)k < (u32)ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (func_8009CF8C((u32)(id + 0x24b0)) == 0) continue;

        char* name0 = func_8013639C(fp, &lbl_eu_8050E990[0x40], id);
        u16 n4 = func_80136254(fp, &lbl_eu_8050E990[0x15], id);
        u8 m1 = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)func_8009CF8C(0x20);
        if ((u32)v >= 0x113) {
            if ((u32)m1 == 3) m1 = 8;
            if ((u32)m2 == 3) m2 = 8;
        }

        u32 v1 = func_8027305C(arr, m1);
        u32 v2 = func_8027305C(arr, m2);
        u8 x = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x30], id);
        char* cond = func_8013639C((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)func_801361E8((u32)fp, &lbl_eu_8050E990[0x3b], id);

        func_80272FA8(&tmpE, (u32)name0, (u32)cond, v1, v2, (s8)0, (s16)n4, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        func_80273004(&arr->mEntries[c], &tmpE);
        collect[k] = 0;
    }

    // Bubble-sort the assembled table ascending by subtype (field_13).
    for (u16 outer = 0; (s32)outer < (s32)(arr->mCount - 1); outer++) {
        int swapped = 0;
        for (u16 inner = 0; (s32)inner < (s32)(arr->mCount - 1 - outer); inner++) {
            TalkListEntry* e1 = &arr->mEntries[inner];
            TalkListEntry* e2 = &arr->mEntries[inner + 1];
            if ((u32)e1->field_13 > (u32)e2->field_13) {
                func_8027274C(&tmpSort);
                func_80272FC8(&tmpSort, e1);
                func_80272FC8(e1, e2);
                func_80272FC8(e2, &tmpSort);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// Whether the talk-list has anything to show (retail func_80273518): returns
// the unknown-0x86 state byte when the scroll bar is visible, else 0.
u8 func_80273518(CKizunaTalkList* self) {
    if (CScrollBar_isVisible(&self->mScrollBar)) {
        return self->mUnknown86;
    }
    return 0;
}


// Begin showing the talk list (retail func_80273564): when idle, mark state 1,
// bind the entry-depart/entry animations and configure the scroll bar.
void func_80273564(CKizunaTalkList* self) {
    if (self->mState85 == 0) {
        self->mState85 = 1;
        self->mNeedsRebuild = 0;
        func_80273A70(self);
        func_802740E4(self);
        f32 vec[3];
        vec[0] = lbl_eu_806689C8;
        vec[1] = lbl_eu_806689CC;
        vec[2] = lbl_eu_806689D0;
        func_801F3670(&self->mScrollBar, vec);
        func_801F36BC(&self->mScrollBar, 7, self->mEntryArray.mCount);
        func_801F367C(&self->mScrollBar);
    }
}

// Advance from display to dismissal (retail func_802735F0): when the list is
// shown (state 3), step to 4 and make the cursor leave the list.
void func_802735F0(CKizunaTalkList* self) {
    if (self->mState85 == 3u) {
        self->mState85 = 4;
        self->mNeedsRebuild = 0;
        func_80273AD0(self);
        func_801D216C(&self->mCursor, 0);
        func_801F369C(&self->mScrollBar);
        func_80138078__FUl(6);
    }
}

void func_80273654(CKizunaTalkList* self) {
    // Scroll up one position through the talk entries.
    u8 count = self->mEntryArray.mCount;
    u8 dec = self->mUnknown88 - 1;
    self->mUnknown88 = (s8)dec;
    if ((s8)dec < 0) {
        self->mUnknown88 = 0;
        s16 pos = self->mUnknown8A - 1;
        self->mUnknown8A = pos;
        if (pos < 0) {
            if (count >= 7) {
                self->mUnknown88 = 6;
                self->mUnknown8A = (s16)(count - 7);
            } else {
                u8 n = count - 1;
                self->mUnknown88 = (s8)n;
                self->mUnknown8A = 0;
                if ((s8)n < 0)
                    self->mUnknown88 = 0;
            }
        }
    }
    func_80273B30(self);
    func_802740E4(self);
    func_801F3850(&self->mScrollBar, (u16)self->mUnknown8A);
    func_80138078__FUl(1);
}

void func_80273710(CKizunaTalkList* self) {
    // Scroll down one position through the talk entries.
    u8 count = self->mEntryArray.mCount;
    if (count >= 7) {
        self->mUnknown88 = self->mUnknown88 + 1;
        if (self->mUnknown88 >= 7) {
            self->mUnknown88 = 6;
            int limit = count - 7;
            self->mUnknown8A = self->mUnknown8A + 1;
            if (self->mUnknown8A > limit) {
                self->mUnknown88 = 0;
                self->mUnknown8A = 0;
            }
        }
    } else {
        self->mUnknown88 = self->mUnknown88 + 1;
        if (self->mUnknown88 >= count) {
            self->mUnknown88 = 0;
            self->mUnknown8A = 0;
        }
    }
    func_80273B30(self);
    func_802740E4(self);
    func_801F3850(&self->mScrollBar, (u16)self->mUnknown8A);
    func_80138078__FUl(1);
}

void func_802737E0(CKizunaTalkList* self) {
    // Scroll up by a page of seven entries.
    u8 count = self->mEntryArray.mCount;
    if (count >= 7) {
        s16 pos = self->mUnknown8A - 7;
        self->mUnknown8A = pos;
        if (pos < 0) {
            self->mUnknown88 = (s8)(pos + 6);
            self->mUnknown8A = 0;
            if (self->mUnknown88 < 0)
                self->mUnknown88 = 0;
        }
    } else {
        self->mUnknown88 = 0;
        self->mUnknown8A = 0;
    }
    func_80273B30(self);
    func_802740E4(self);
    func_801F3850(&self->mScrollBar, (u16)self->mUnknown8A);
    func_80138078__FUl(1);
}

void func_8027387C(CKizunaTalkList* self) {
    // Scroll down by a page of seven entries, clamping to the list end.
    u8 count = self->mEntryArray.mCount;
    if (count >= 7) {
        self->mUnknown8A = self->mUnknown8A + 7;
        int limit = count - 7;
        if (self->mUnknown8A > limit) {
            self->mUnknown88 = self->mUnknown8A - limit;
            self->mUnknown8A = limit;
            if (self->mUnknown88 >= 7)
                self->mUnknown88 = 6;
        }
    } else {
        self->mUnknown88 = count - 1;
        self->mUnknown8A = 0;
        if (self->mUnknown88 < 0)
            self->mUnknown88 = 0;
    }
    func_80273B30(self);
    func_802740E4(self);
    func_801F3850(&self->mScrollBar, (u16)self->mUnknown8A);
    func_80138078__FUl(1);
}

extern "C" __declspec(noinline) void func_80273938(CKizunaTalkList* self) {
    // Advance the entry-show animation; when it completes, step the state
    // machine forward (retail func_80273938).
    if (func_80137444(self->mpAnim24, lbl_eu_806689D4) != 0) {
        self->mState85 = 2;
        func_80273AD0(self);
    }
}

extern "C" __declspec(noinline) void func_80273984(CKizunaTalkList* self) {
    if (func_80137444(self->mpAnim28, 2.0f) != 0) {
        self->mState85 = 3;
        func_802740E4(self);
        self->mNeedsRebuild = 1;
    }
}

extern "C" __declspec(noinline) void func_802739D8(CKizunaTalkList* self) {
    if (func_80137510(self->mpAnim28, lbl_eu_806689D4) != 0) {
        self->mState85 = 5;
        func_80273A70(self);
    }
}

extern "C" __declspec(noinline) void func_80273A24(CKizunaTalkList* self) {
    if (func_80137510(self->mpAnim24, lbl_eu_806689D4) != 0) {
        self->mState85 = 0;
        self->mNeedsRebuild = 1;
    }
}

// Bind the scroll-out animations for the talk list (retail func_80273A70).
// Bind the scroll-out animations for the talk list (retail func_80273A70).
extern "C" __declspec(noinline) void func_80273A70(CKizunaTalkList* self) {
    self->mpLayout20->SetAnimationEnable(self->mpAnim28, 0);
    self->mpLayout20->SetAnimationEnable(self->mpAnim24, 1);
}

// Bind the scroll-in animations for the talk list (retail func_80273AD0).
extern "C" __declspec(noinline) void func_80273AD0(CKizunaTalkList* self) {
    self->mpLayout20->SetAnimationEnable(self->mpAnim24, 0);
    self->mpLayout20->SetAnimationEnable(self->mpAnim28, 1);
}

void func_80273AD0(){}

// Rebuild/render the visible list. For each of the (up to) 7 visible rows it
// pulls the entry, binds the pane names (built with sprintf row suffixes),
// resolves the affinity-level animation string and the row highlight colour,
// and drives the pane visibility flag (retail func_80273B30).
extern "C" __declspec(noinline) void func_80273B30(CKizunaTalkList* self) {
    char buf[0x10];
    char* lbl = lbl_eu_8050E990;
    int i = 0;
    do {
        int base = i + self->mUnknown8A;                       // current row index + cursor
        TalkListEntry* entry = (TalkListEntry*)func_80273040(&self->mEntryArray, (u8)base);
        u8 dispIdx = (u8)(i + 1);                              // 1-based display index

        // Row number ("00/01") style panes x3, each with an optional highlight.
        sprintf(buf, &lbl[0x9e], dispIdx);
        void* pane = ((void* (*)(void*))(((void**)*(void**)((char*)self->mpLayout20 + 0x10))[0x3c / 4]))(*(void**)((char*)self->mpLayout20 + 0x10));
        if (pane) {
            int z = (int)(self->mEntryArray.mCount ^ ((u8)i + base));
            func_80124270(pane, (u32)(((z >> 1) - (z & self->mEntryArray.mCount)) >> 31));
        }

        sprintf(buf, &lbl[0xae], dispIdx);
        pane = ((void* (*)(void*))(((void**)*(void**)((char*)self->mpLayout20 + 0x10))[0x3c / 4]))(*(void**)((char*)self->mpLayout20 + 0x10));
        if (pane) {
            int z = (int)(self->mEntryArray.mCount ^ ((u8)i + base));
            func_80124270(pane, (u32)(((z >> 1) - (z & self->mEntryArray.mCount)) >> 31));
        }

        sprintf(buf, &lbl[0xbe], dispIdx);
        pane = ((void* (*)(void*))(((void**)*(void**)((char*)self->mpLayout20 + 0x10))[0x3c / 4]))(*(void**)((char*)self->mpLayout20 + 0x10));
        if (pane) {
            int z = (int)(self->mEntryArray.mCount ^ ((u8)i + base));
            func_80124270(pane, (u32)(((z >> 1) - (z & self->mEntryArray.mCount)) >> 31));
        }

        // Title / description text.
        sprintf(buf, &lbl[0xcb], dispIdx);
        func_80136B4C(self->mpLayout20, buf, (char*)entry->field_00, 0u);
        sprintf(buf, &lbl[0xd7], dispIdx);
        func_80136B4C(self->mpLayout20, buf, (char*)entry->field_04, 0u);

        if (entry->field_12 != -1) {
            sprintf(buf, &lbl[0xe3], dispIdx);
            func_80137E7C(self->mpLayout20, buf, (void*)entry->field_08);
            sprintf(buf, &lbl[0xf1], dispIdx);
            func_80137E7C(self->mpLayout20, buf, (void*)entry->field_0C);
        }

        sprintf(buf, &lbl[0xff], dispIdx);
        pane = ((void* (*)(void*))(((void**)*(void**)((char*)self->mpLayout20 + 0x10))[0x3c / 4]))(*(void**)((char*)self->mpLayout20 + 0x10));
        if (pane) {
            int z = (int)(self->mEntryArray.mCount ^ ((u8)i + base));
            func_80124270(pane, (u32)(((z >> 1) - (z & self->mEntryArray.mCount)) >> 31));
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
            func_80137E7C(self->mpLayout20, buf, anim);

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
        func_80139A18(self->mpLayout20, buf, c0, c1);
        sprintf(buf, &lbl[0x19f], dispIdx);
        pane = ((void* (*)(void*))(((void**)*(void**)((char*)self->mpLayout20 + 0x10))[0x3c / 4]))(*(void**)((char*)self->mpLayout20 + 0x10));
        if (pane)
            func_80124270(pane, vis);

        i++;
    } while (i < 7);
}

// Rebuild/render the cursor row highlight (retail func_802740E4): resolve the
// two layout panes named after the current row, build the cursor target
// position from them and hand it to the cursor via its vtable slot 4.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802740E4(CKizunaTalkList* self) {
    nw4r::math::VEC3 pos;
    u32 _pad;
    char name[0x28];
    char* lbl = lbl_eu_8050E990;
    sprintf(name, lbl + 0xcb, (int)self->mUnknown88 + 1);
    func_801D216C(&self->mCursor, 1);

    // Layout object re-read for every access so nothing extra gets
    // enregistered across the virtual calls.
    nw4r::lyt::Pane* paneA =
        self->mpLayout20->GetRootPane()->FindPaneByName(name, true);
    nw4r::lyt::Pane* paneB =
        self->mpLayout20->GetRootPane()->FindPaneByName(lbl + 0x1aa, true);

    func_80137924(&pos, paneA, paneB, self->mpLayout20->GetRootPane());

    ((CCur18View*)&self->mCursor[0])->v02(&pos);
}
#pragma optimize_for_size off

// Rebuild the visible talk list (retail func_802741B0): requires both the
// shared BDAT data and the layout accessor. Builds a fresh local table via
// __ct__802726F4, copies it over the member array, then rebuilds/sorts.
void func_802741B0(CKizunaTalkList* self) {
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
        func_80273004(dst, src);
        dst++;
        src++;
    } while (dst < end);
    self->mEntryArray.mCount = tmp.mCount;
    self->mEntryArray.mParent = tmp.mParent;
    __destroy_arr(&tmp, (void*)__dt__80272774, 0x14, 0x100);

    func_80272810(&self->mEntryArray);
    self->mUnknown86 = 1;
    self->mState84 = 1;
    func_80273B30(self);
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
        func_80434A4C__Q23mtl10MemManagerFb(false);

        // Attach the layout archive to a fresh ArcResource accessor.
        this->mUnknown1C = (u32)createArcResourceAccessor__10CLibLayoutFv();
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc((nw4r::lyt::ArcResourceAccessor*)this->mUnknown1C, fileData, &strBase[0x1c9]);

        // Create the layout and its two animation transforms.
        func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(&this->mpLayout20, (nw4r::lyt::ArcResourceAccessor*)this->mUnknown1C, &strBase[0x1cd]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(this->mpLayout20, &this->mpAnim24, (nw4r::lyt::ArcResourceAccessor*)this->mUnknown1C, &strBase[0x1e2]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(this->mpLayout20, &this->mpAnim28, (nw4r::lyt::ArcResourceAccessor*)this->mUnknown1C, &strBase[0x1fa]);

        // Bind the font pane onto the layout.
        void* rootPane = *(void**)((char*)this->mpLayout20 + 0x10);
        void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, this->mpLayout20);
        u32 fontResult = ((u32 (*)(void*))(((void**)fontObj)[0x24 / 4]))(fontObj);
        func_8013676C((nw4r::lyt::Pane*)rootPane, fontResult);

        func_80273A70(this);

        // Layout virtual slot 14 (offset 0x38): disable animation.
        ((void (*)(void*, int))(((void**)this->mpLayout20)[0x38 / 4]))(this->mpLayout20, 0);

        // Create the CCur18 cursor on the stack and copy its body into the
        // embedded cursor (+0x2c), skipping the vtable at +0.
        void* ccur18Accessor = (void*)func_801355F4();
        u8 cur18Temp[0x18];
        __ct__CCur18(cur18Temp, ccur18Accessor);
        *(u32*)((u8*)this + 0x30) = *(u32*)(cur18Temp + 4);
        *(u32*)((u8*)this + 0x34) = *(u32*)(cur18Temp + 8);
        *(u32*)((u8*)this + 0x38) = *(u32*)(cur18Temp + 0xc);
        *(u32*)((u8*)this + 0x3c) = *(u32*)(cur18Temp + 0x10);
        *(u8*)((u8*)this + 0x40) = *(u8*)(cur18Temp + 0x14);
        *(u8*)((u8*)this + 0x41) = *(u8*)(cur18Temp + 0x15);
        __dt__6CCur18Fv(cur18Temp, -1);

        // Cursor virtual slot 2 (offset 0x08) on the embedded cursor.
        ((void (*)(void*))(((void**)(*(u32*)((u8*)this + 0x2c)))[0x08 / 4]))((u8*)this + 0x2c);

        func_802741B0(this);

        this->mEntryCount = 0;
        ((UnkClass_8045F564*)((u8*)this + 4))->func_8045F810();
        __dt__14Class_8045F858Fv(temp8, -1);
        return 1;
    }

    if (this->mUnknown18 == (u32)pEventFile->mFileHandle) {
        // Shared-archive hit: release the previously buffered shared file data.
        CFileHandle* fh2 = (CFileHandle*)this->mUnknown18;
        void* data2 = fh2->getData();
        func_8003AA78__5CBdatFUlPv(2, data2);
        func_8003AA34();
        lbl_eu_806648B8 = (void*)getFP__FPCc(&lbl_eu_8050E990[0x217]);
        func_802741B0(this);
        this->mUnknown18 = 0;
        return 1;
    }

    return 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_80274458() {
    func_801C4B60(&lbl_eu_806648C0, 0x8c, 0x8c, 0x8c, 0);
    func_801C4B60(&lbl_eu_806648C8, 0x8c, 0x8c, 0x8c, 0xff);
    func_801C4B60(&lbl_eu_806648D0, 0x48, 0x3a, 0x21, 0);
    func_801C4B60(&lbl_eu_806648D8, 0x48, 0x3a, 0x21, 0xff);
}

extern "C" __declspec(noinline) void* func_80273040(void* self, u32 r4) {
    // Use signed compare to match retail cmpwi
    if ((s32)r4 >= 0x100) return 0;
    return (u8*)self + r4 * 0x14;
}
