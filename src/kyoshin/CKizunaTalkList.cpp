// Auto-scaffolded catalog TU for kyoshin/CKizunaTalkList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CKizunaTalkList.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/code_80135FDC.hpp"

// External helpers (C-linkage retail symbols) for file loading, scrollbar /
// cursor widgets and sound effects.
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 func_800A9D90();
extern "C" void func_801F34F4(void*);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F367C(void*);
extern "C" void func_801F369C(void*);
extern "C" void func_801F36BC(void*, int, int);
extern "C" void func_801F3850(void*, u16);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D216C(void*, int);
extern "C" void func_80138078__FUl(u32);
extern "C" void func_801C4B60(void*, u32, u32, u32, u32);
extern "C" void __dl__FPv(void*);
extern "C" void __construct_array(void*, void* ctor, void* dtor, int size, int n);
extern "C" void __destroy_arr(void*, void* dtor, int size, int n);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);

// Layout-build helpers used by CKizunaTalkList::OnFileEvent (mangled retail
// symbol names so the reloc targets line up).
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool value);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor* self, void* data, const char* name);
extern "C" void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(nw4r::lyt::Layout** dst, nw4r::lyt::ArcResourceAccessor* acc, const char* name);
extern "C" void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(nw4r::lyt::Layout* lay, nw4r::lyt::AnimTransform** dst, nw4r::lyt::ArcResourceAccessor* acc, char* name);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);
extern "C" void* func_801355F4();
extern "C" void __ct__CCur18(void* self, void* param);
extern "C" void func_8003AA78__5CBdatFUlPv(u32 value, void* data);
extern "C" void func_8003AA34();
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void* self, void* base);
extern "C" void __dt__14Class_8045F858Fv(void* self, int dealloc);
extern "C" void* getFP__FPCc(const char* name);
extern "C" int sprintf(char* str, const char* fmt, ...);

// Data/build helpers used by func_80272810.
extern "C" u32 func_8003B1EC(void* fp);
extern "C" u32 func_8009CF8C(u32);
extern "C" u16 func_80136254(const void* fp, const char* name, u16 id);
extern "C" u16 func_8013A7D0(u8 a, u8 b);
extern "C" u32 func_8027305C(TalkListEntryArray* self, u8 v);
extern "C" void* lbl_eu_806640A8;                            // .sbss table pointer

// data / rodata labels
extern "C" char lbl_eu_8050E990[];            // file-name table (target 7)
extern "C" void* lbl_eu_80664090;            // .sbss shared character table (target 7)
extern "C" void* lbl_eu_806648B8;             // .sbss loaded file pointer (target 8)
extern "C" void* lbl_eu_806648C0;             // .sbss colour entries (target 8)
extern "C" void* lbl_eu_806648C8;
extern "C" void* lbl_eu_806648D0;
extern "C" void* lbl_eu_806648D8;
extern "C" f32 lbl_eu_806689C8;               // .sdata2 float constants (target 10)
extern "C" f32 lbl_eu_806689CC;
extern "C" f32 lbl_eu_806689D0;

// Forward decls for state-transition helpers (defined below / matched later).
// extern "C" + noinline keeps callers emitting real unmangled bl branches
// (retail keeps them as separate out-of-line functions in this TU).
extern "C" __declspec(noinline) void func_80273AD0(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_80273A70(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_80273B30(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_802740E4(CKizunaTalkList* self);

// Converted to inline member function in header

void __ct__CKizunaTalkList(){}

CKizunaTalkList::~CKizunaTalkList() {}

// Element destructor of TalkListEntry (used by the array teardown). No-op
// body: entries are plain PODs, teardown is just a per-element call.
void __dt__80272774(TalkListEntry* entry) {}

// Element constructor (defined below; used as the array element ctor).
void func_8027274C(TalkListEntry* entry);

// Construct the TalkListEntry array (retail __ct__802726F4). Constructs all
// 256 entries in place via the element ctor func_8027274C, then initialises
// the count to 0 and records the owning object.
void __ct__802726F4(TalkListEntryArray* self, u32 parent) {
    __construct_array(self, (void*)func_8027274C, (void*)__dt__80272774, 0x14, 0x100);
    self->mCount = 0;
    self->mParent = parent;
}

// Destroy the TalkListEntry array (retail __dt__802727B4), freeing the
// backing memory when the delete flag is set.
void __dt__802727B4(TalkListEntryArray* self, int flags) {
    if (self != 0) {
        __destroy_arr(self, (void*)__dt__80272774, 0x14, 0x100);
        if (flags >= 0)
            __dl__FPv(self);
    }
}

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

void func_8027336C(){}

#pragma optimize_for_size on
// Draw the talk list (retail func_8027340C): draw the layout, scroll bar and
// cursor when the list is visible.
void func_8027340C(CKizunaTalkList* self, nw4r::lyt::DrawInfo* pDrawInfo) {
    if (self->mState84 != 0) {
        func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(self->mpLayout20, pDrawInfo, 0, 1);
        func_801F35B0(&self->mScrollBar, (void*)pDrawInfo);
        func_801D20B0(&self->mCursor, (void*)pDrawInfo);
    }
}
#pragma optimize_for_size off

void func_8027346C(){}

// Populate a talk-list entry with the given fields (retail func_80272FA8).
void func_80272FA8(TalkListEntry* entry, u32 a, u32 b, u32 c, u32 d, s8 in12,
                   s16 in10, u8 in13) {
    entry->field_00 = a;
    entry->field_04 = b;
    entry->field_08 = c;
    entry->field_0C = d;
    entry->field_10 = in10;   // s16
    entry->field_12 = in12;   // s8, init to -1
    entry->field_13 = in13;   // u8
}

// Zero-initialise a talk-list entry (retail func_8027274C).
void func_8027274C(TalkListEntry* entry) {
    entry->field_00 = 0;
    entry->field_04 = 0;
    entry->field_08 = 0;
    entry->field_0C = 0;
    entry->field_10 = 0;
    entry->field_12 = -1;
    entry->field_13 = 0;
}

// Copy one talk-list entry into another (retail func_80272FC8).
void func_80272FC8(TalkListEntry* dst, TalkListEntry* src) {
    dst->field_00 = src->field_00;
    dst->field_04 = src->field_04;
    dst->field_08 = src->field_08;
    dst->field_0C = src->field_0C;
    dst->field_10 = src->field_10;
    dst->field_12 = src->field_12;
    dst->field_13 = src->field_13;
}

// Copy one talk-list entry into another (retail func_80273004).
void func_80273004(TalkListEntry* dst, const TalkListEntry* src) {
    dst->field_00 = src->field_00;
    dst->field_04 = src->field_04;
    dst->field_08 = src->field_08;
    dst->field_0C = src->field_0C;
    dst->field_10 = src->field_10;
    dst->field_12 = src->field_12;
    dst->field_13 = src->field_13;
}

// Whether an icon/texture row exists for the given subtype, by asking the
// owning layout's shared resource accessor for the "timg" resource (retail
// func_8027305C). v==0 queries the parent accessor directly; v!=0 re-resolves
// the icon name through the shared character table first.
extern "C" char* func_80138F78(u16 idx);
extern "C" u32 func_8027305C(TalkListEntryArray* self, u8 v) {
    if (v == 0) {
        void* iconList = (void*)self->mParent;
        u32 ok = ((u32 (*)(void*, u32, const char*, u32))(((void**)iconList)[3]))(
            iconList, 0x74696d67, &lbl_eu_8050E990[0x46], 0);
        return ok != 0;
    }
    u16 id = func_80136254(lbl_eu_80664090, &lbl_eu_8050E990[0x5e], v);
    char* name = func_80138F78(id);
    void* resAcc = (void*)func_801355F4();
    u32 ok = ((u32 (*)(void*, u32, const char*, u32))(((void**)resAcc)[3]))(
        resAcc, 0x74696d67, name, 0);
    return ok != 0;
}

// Build the talk-list entry table from the affinity talk data (retail
// func_80272810). Reads the shared character database handed off in
// lbl_eu_806648B8, collects the candidate character IDs that have talk
// entries, then runs four passes to assemble type-1/type-2/name0 entries and
// finally bubble-sorts the table by entry subtype (field_13).
extern "C" void func_80272810(TalkListEntryArray* arr) {
    void* fp = lbl_eu_806648B8;
    u16 charCount = (u16)func_8003B1EC(fp);
    char* fmt12 = func_80136190(&lbl_eu_8050E990[0], &lbl_eu_8050E990[0xb], 0x12);
    char* fmt13 = func_80136190(&lbl_eu_8050E990[0], &lbl_eu_8050E990[0xb], 0x13);

    arr->mCount = 0;
    u16 collect[0x100];
    memset(collect, 0, 0x200);
    u16 ccount = 0;
    TalkListEntry tmpB, tmpC, tmpD, tmpSort;

    // Pass A: collect IDs of characters that have a talk flag set.
    for (u16 c = 1; (u16)c <= charCount; c++) {
        if (func_8009CF8C((u32)((u16)c + 0x3440)) != 0)
            collect[ccount++] = c;
    }

    // Pass B: type-1 entries.
    for (u16 k = 0; k < ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (func_8009CF8C((u32)(id + 0x24b0)) != 0) continue;

        u16 a = func_80136254(fp, &lbl_eu_8050E990[0x10], id);
        if ((u32)a > func_8009CF8C(0x20)) continue;

        u16 n4 = func_80136254(fp, &lbl_eu_8050E990[0x15], id);
        u8 m1 = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)func_8009CF8C(0x20);
        if (v >= 0x2a && v < 0x113) {
            if (m1 == 3) continue;
            if (m2 == 3) continue;
        } else if (v >= 0x113) {
            if (m1 == 3) m1 = 8;
            if (m2 == 3) m2 = 8;
        }
        if ((u32)n4 > func_8013A7D0(m1, m2)) continue;

        u32 v1 = func_8027305C(arr, m1);
        u32 v2 = func_8027305C(arr, m2);
        u8 x = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x30], id);
        char* cond = func_8013639C((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x3b], id);

        TalkListEntry* e = &tmpB;
        func_80272FA8(e, (u32)fmt12, (u32)cond, v1, v2, (s8)1, (s16)n4, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        func_80273004(&arr->mEntries[c], e);
        collect[k] = 0;
    }

    // Pass C: type-2 entries (same guard/filter logic).
    for (u16 k = 0; k < ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (func_8009CF8C((u32)(id + 0x24b0)) != 0) continue;

        u8 m1 = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)func_8009CF8C(0x20);
        if (v >= 0x2a && v < 0x113) {
            if (m1 == 3) continue;
            if (m2 == 3) continue;
        } else if (v >= 0x113) {
            if (m1 == 3) m1 = 8;
            if (m2 == 3) m2 = 8;
        }

        u16 n4 = func_80136254(fp, &lbl_eu_8050E990[0x15], id);
        u32 v1 = func_8027305C(arr, m1);
        u32 v2 = func_8027305C(arr, m2);
        u8 x = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x30], id);
        char* cond = func_8013639C((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x3b], id);

        TalkListEntry* e = &tmpC;
        func_80272FA8(e, (u32)fmt12, (u32)cond, v1, v2, (s8)2, (s16)n4, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        func_80273004(&arr->mEntries[c], e);
        collect[k] = 0;
    }

    // Pass D: flagged-character entries (keep only when talk flag is unset).
    for (u16 k = 0; k < ccount; k++) {
        u16 id = collect[k];
        if (id == 0) continue;
        if (func_8009CF8C((u32)(id + 0x24b0)) == 0) continue;

        char* name0 = func_8013639C(fp, &lbl_eu_8050E990[0x40], (int)id);
        u16 n4 = func_80136254(fp, &lbl_eu_8050E990[0x15], id);
        u8 m1 = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x1e], id);
        u8 m2 = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x27], id);
        u16 v = (u16)func_8009CF8C(0x20);
        if (v >= 0x113) {
            if (m1 == 3) m1 = 8;
            if (m2 == 3) m2 = 8;
        }

        u32 v1 = func_8027305C(arr, m1);
        u32 v2 = func_8027305C(arr, m2);
        u8 x = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x30], id);
        char* cond = func_8013639C((const void*)lbl_eu_806640A8, &lbl_eu_8050E990[0x36], (int)x);
        u8 y = (u8)func_801361E8(fp, &lbl_eu_8050E990[0x3b], id);

        TalkListEntry* e = &tmpD;
        func_80272FA8(e, (u32)name0, (u32)cond, v1, v2, (s8)0, (s16)n4, y);
        u8 c = arr->mCount;
        arr->mCount = c + 1;
        func_80273004(&arr->mEntries[c], e);
        collect[k] = 0;
    }

    // Bubble-sort the assembled table ascending by subtype (field_13).
    for (u16 outer = 0; outer + 1 < arr->mCount; outer++) {
        int swapped = 0;
        for (u16 inner = 0; inner < arr->mCount - 1 - outer; inner++) {
            TalkListEntry* e1 = &arr->mEntries[inner];
            TalkListEntry* e2 = &arr->mEntries[inner + 1];
            if (e1->field_13 > e2->field_13) {
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

extern "C" u8 CScrollBar_isVisible(void*);

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
        u8 inc = self->mUnknown88 + 1;
        self->mUnknown88 = (s8)inc;
        if ((s8)inc >= 7) {
            self->mUnknown88 = 6;
            s16 pos = self->mUnknown8A + 1;
            self->mUnknown8A = pos;
            if ((s16)pos > (s16)(count - 7)) {
                self->mUnknown88 = 0;
                self->mUnknown8A = 0;
            }
        }
    } else {
        u8 inc = self->mUnknown88 + 1;
        self->mUnknown88 = (s8)inc;
        if ((s8)inc >= (s8)count) {
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
        s16 limit = (s16)(count - 7);
        s16 pos = self->mUnknown8A + 7;
        self->mUnknown8A = pos;
        if (pos > limit) {
            self->mUnknown88 = (s8)(pos - limit);
            self->mUnknown8A = limit;
            if ((s8)(pos - limit) >= 7)
                self->mUnknown88 = 6;
        }
    } else {
        u8 n = count - 1;
        self->mUnknown88 = (s8)n;
        self->mUnknown8A = 0;
        if ((s8)n < 0)
            self->mUnknown88 = 0;
    }
    func_80273B30(self);
    func_802740E4(self);
    func_801F3850(&self->mScrollBar, (u16)self->mUnknown8A);
    func_80138078__FUl(1);
}

void func_80273938(CKizunaTalkList* self) {
    // Advance the entry-show animation; when it completes, step the state
    // machine forward (retail func_80273938).
    if (func_80137444(self->mpAnim24, 2.0f) != 0) {
        self->mState85 = 2;
        func_80273AD0(self);
    }
}

void func_80273984(CKizunaTalkList* self) {
    if (func_80137444(self->mpAnim28, 2.0f) != 0) {
        self->mState85 = 3;
        func_802740E4(self);
        self->mNeedsRebuild = 1;
    }
}

void func_802739D8(CKizunaTalkList* self) {
    if (func_80137510(self->mpAnim28, 2.0f) != 0) {
        self->mState85 = 5;
        func_80273A70(self);
    }
}

void func_80273A24(CKizunaTalkList* self) {
    if (func_80137510(self->mpAnim24, 2.0f) != 0) {
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

extern "C" void func_80124270(void* obj, int value);
extern "C" __declspec(noinline) void* func_80273040(void* self, u32 r4);

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

extern "C" __declspec(noinline) void func_802740E4(CKizunaTalkList* self){}

// Changes the embedded cursor.
void func_802741B0(CKizunaTalkList* self) {}

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
