// Auto-scaffolded catalog TU for kyoshin/CKizunaTalkList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CKizunaTalkList.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/code_80135FDC.hpp"

// Forward decls for state-transition helpers (defined below / matched later).
void func_80273AD0(CKizunaTalkList* self);
void func_80273A70(CKizunaTalkList* self);
void func_802740E4(CKizunaTalkList* self);

// Converted to inline member function in header

void __ct__CKizunaTalkList(){}

CKizunaTalkList::~CKizunaTalkList() {}

void func_802732F4(){}

void func_8027336C(){}

void func_8027340C(){}

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


void func_80273564(){}

void func_802735F0(){}

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

void func_80273A70(CKizunaTalkList* self){}

void func_80273AD0(CKizunaTalkList* self){}

void func_80273AD0(){}

void func_80273B30(){}

void func_802740E4(CKizunaTalkList* self){}

void func_802741B0(){}

void CKizunaTalkList::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_80274458(){}

extern "C" void* func_80273040(void* self, u32 r4) {
    // Use signed compare to match retail cmpwi
    if ((s32)r4 >= 0x100) return 0;
    return (u8*)self + r4 * 0x14;
}
