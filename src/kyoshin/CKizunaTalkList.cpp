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

// data / rodata labels
extern "C" char lbl_eu_8050E990[];            // file-name table (target 7)
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
extern "C" __declspec(noinline) void func_802740E4(CKizunaTalkList* self);

// BindAnim-style virtual dispatch on nw4r::lyt::Layout vtable slot 0x2C.
static void bindAnim(nw4r::lyt::Layout* layout, void* anim, u32 flag) {
    typedef void (*VirtFn)(void*, void*, u32);
    VirtFn fn = ((VirtFn*)*(u32*)layout)[11];
    fn(layout, anim, flag);
}

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

// Initialise the Kizuna talk list (retail func_802732F4): load the talk-list
// file and the shared archive, then set up the scroll bar.
void func_802732F4(CKizunaTalkList* self) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->mEntryCount = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii((u32)handle, &lbl_eu_8050E990[0x67], self, 0, 0);
    self->mUnknown18 = (u32)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(func_800A9D90(), &lbl_eu_8050E990[0x82], self, 0, 0);
    func_801F34F4(&self->mScrollBar);
    self->mUnknown86 = 0;
}

void func_8027336C(){}

// Draw the talk list (retail func_8027340C): draw the layout, scroll bar and
// cursor when the list is visible.
void func_8027340C(CKizunaTalkList* self, nw4r::lyt::DrawInfo* pDrawInfo) {
    if (self->mState84 != 0) {
        func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(self->mpLayout20, pDrawInfo, 0, 1);
        func_801F35B0(&self->mScrollBar, (void*)pDrawInfo);
        func_801D20B0(&self->mCursor, (void*)pDrawInfo);
    }
}

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

void func_80273654(){}

void func_80273710(){}

void func_802737E0(){}

void func_8027387C(){}

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
    bindAnim(self->mpLayout20, self->mpAnim28, 0);
    bindAnim(self->mpLayout20, self->mpAnim24, 1);
}

// Bind the scroll-in animations for the talk list (retail func_80273AD0).
extern "C" __declspec(noinline) void func_80273AD0(CKizunaTalkList* self) {
    bindAnim(self->mpLayout20, self->mpAnim24, 0);
    bindAnim(self->mpLayout20, self->mpAnim28, 1);
}

void func_80273AD0(){}

void func_80273B30(){}

extern "C" __declspec(noinline) void func_802740E4(CKizunaTalkList* self){}

void func_802741B0(){}

void CKizunaTalkList::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_80274458() {
    func_801C4B60(&lbl_eu_806648C0, 0x8c, 0x8c, 0x8c, 0);
    func_801C4B60(&lbl_eu_806648C8, 0x8c, 0x8c, 0x8c, 0xff);
    func_801C4B60(&lbl_eu_806648D0, 0x48, 0x3a, 0x21, 0);
    func_801C4B60(&lbl_eu_806648D8, 0x48, 0x3a, 0x21, 0xff);
}

extern "C" void* func_80273040(void* self, u32 r4) {
    // Use signed compare to match retail cmpwi
    if ((s32)r4 >= 0x100) return 0;
    return (u8*)self + r4 * 0x14;
}
